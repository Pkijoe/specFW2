//===========================================================================
//
//	Module Name:	ServoControl.cpp
//
//	Function:		Function determines the lid and tub power outputs
//                  using a PID algorithm.
//
//	Original Author:	Joe Orlando, Neal Green
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "ParserThread.h"
#include "Debug.h"
#include "zodiac.h"
#include <math.h>

void format_date(char *str)
{
   char s2[20];

   strcpy(s2, str);
   str[0] = s2[2];		
   str[1] = s2[3];   
   str[2] = '/';  
   str[3] = s2[5]; 
   str[4] = s2[6];
   str[5] = '/';  
   str[6] = '2';
   str[7] = '0';   
   str[8] = s2[8];   
   str[9] = s2[9];   
 }

//**********************************************************************************************
 
#define KP_TUB				100.0f			// Proportional Constant Value
#define KI_TUB				0.170f			// Integral Constant Value

class PID						// PID Algorithm 
{
	public:
		float KP;				// Proportional Constant	
		float KI;				// Integral Constant
		float Ierror;			// Integral Error
		float Pout;				// Proportional power external
		float Iout;				// Integrator power external
		
		PID(float kp, float ki):		// Constructor		
			KP(kp), 
			KI(ki), 
			Ierror(0.0)
		{ }							

		~PID() { }						// Destructor

//---------------------------------------------------------	
		float calculate (float setpoint, float actual)
		{
			// Calculate the error signal.
			float error = setpoint - actual;

			// Init proportional power and integral window.
			float Pp = 0.0;		// 0%
			float Tw = 100.0;	// 100%

			// Calculate the proportional power and integral window.
			if ( KP > 0.0 )
			{
				Pout = Pp = KP * error;
				Tw = 100.0f / KP;   // Integral window is 100% / KP in units of degrees;  Used for anti-windup of integrator term.
			}

			// Integrate error and integral power.
			float Pi = 0.0;
			if (KI > 0.0 && fabs(error) <= Tw)		// Check if the error is outside of the window.
			{
				// Not outside the window; Update the integrator.
				float ierror = Ierror + error;  	// Update integrator term.
				float pi = KI * ierror;				// Update integral power term.
				float p = pi + Pp;					// Update total power term.
				if ( (p > 0.0 || error > 0.0) && (p < 100.0 || error < 0.0))
				{
					Ierror = ierror;				// Use the updated integrator term.
					Pi = pi;						// Use the updated power term in the power calculation.
				}
				else
				{
					Pi = KI * Ierror;				// Freeze the integrator term.
				}									// Use the last value that was inside the window for the integral power calculation.
			}
			else
			{
				// Outside the window; Reset the integrator.
				Ierror = 0;
			}

			// Limit total power to 0%..100%.
			Iout = Pi;
			float Ptotal = Pp + Pi;

			if (Ptotal > 100.0) Ptotal = 100.0;
			else if (Ptotal < 0.0) Ptotal = 0.0;

			return Ptotal;
		} // End calculate
		
//---------------------------------------------------------		
		void set_integrator_power(float I_power)
		{
			Ierror = I_power / KI;
		} 
		
//---------------------------------------------------------			
		float get_integrator_power()
		{
			return(Iout);
		} 
		
//---------------------------------------------------------					
		float get_proportional_power()
		{
			return(Pout);
		} 	

	
}; // End of PID class


//**********************************************************************************************

// Initial values
const float PERIOD = 1.0f;		// PID period (seconds).

float tub_setpoint;
float lid_setpoint;
float PID_Period = PERIOD;
PID pid_tub (KP_TUB, KI_TUB);

void CParserThread::ServoControl()
{
	long seconds = 0, hours = 0;
	static bool isinit = false;
	static DWORD tick_ref = 0;
	static WORD paused_tub_power;
	float tub_weighted_temp = 0, tub_power = 0, lid_power = 0;
	WORD tub_PWM_duty, lid_PWM_duty;
	int temperature_is_in_range = FALSE;
	
	if (!isinit)
	{
	   isinit = true;
	   tick_ref = ::GetTickCount();
	}

	// Time keeping.
	DWORD tick = ::GetTickCount() - tick_ref;
    seconds = (long) tick / 1000;
	hours = seconds / 3600;

	// Set the tub weighted temperature
	tub_weighted_temp = Get_Tub_Temp();  
	
	if (tub_weighted_temp >= HEATERS_RANGE_LOW && tub_weighted_temp <= HEATERS_RANGE_HIGH)
	{
	   temperature_is_in_range = TRUE;
	}

	if (m_NvRam.InputB(HEATER_CONTROL) == '1' && temperature_is_in_range)		// Execute only if heater is turned on and temperature is in range.
	{
		m_IO.OutputW(HEATERS_ENABLE_REG, (TUB_PWM_ENABLE | LID_PWM_ENABLE));	// Enable tub and lid PWM
		m_IO.OutputW(TUB_HEATER_PWM_PERIOD_REG, HEATER_PWM_PERIOD);				// Set the tub PWM period 
		m_IO.OutputW(LID_HEATER_PWM_PERIOD_REG, HEATER_PWM_PERIOD);				// Set the lid PWM period 	
		
		if (m_nHeater_mode != MODE_LID_OFF)			// Not in a special heater mode - operate as usual
		  {
            // Integrator preset logic
            if (m_Startup_flag)						// Only do this once at startup
            {
                if (tub_weighted_temp > INTEGRATOR_WINDOW_THRESHOLD)					// Preset the integrator only if it's active
                {
                   float ipreset = (float) ((m_NvRam.InputW(INTEGRATOR_PRESET) / 100.0));	// Get the last stored integrator value
                   pid_tub.set_integrator_power(ipreset);								// Preset the integrator
                   m_Startup_flag = false;
                }
            }
			  
			 // Set target temps.
			 tub_setpoint = TUB_TEMPERATURE_SETPOINT;

			 // Power to deliver.
			 tub_power = pid_tub.calculate(tub_setpoint, tub_weighted_temp);			// Control the tub power thru PI algorithm 
           
			 if (tub_weighted_temp < LID_POWER_REDUCTION_TEMPERATURE)
               {
                  lid_power = LID_POWER_MAX;    
               }
             else
               {
                 lid_power = LID_POWER_LEVEL;    		// Lid power is held constant
               }
			
			 // To PWM generator
			 tub_PWM_duty = paused_tub_power = (WORD) (tub_power * 10);  // Duty cycle units are in miliseconds;  100% = 1000
			 lid_PWM_duty = (WORD) (lid_power * 10);
		  }
		  else											// Lid off - special heater mode								
		  {
			 //  Set tub power to the last updated PID value before entering lid off mode
			 //  and set lid power to 0
			 
			 // To PWM generator
			 tub_PWM_duty = paused_tub_power;			//  Set tub power to last PI value
			 lid_PWM_duty = 0;			 				//  Set lid power to 0
		  }
		  
		m_IO.OutputW(TUB_HEATER_PWM_DUTY_REG, tub_PWM_duty);				// Set the tub PWM duty cycle
		m_IO.OutputW(LID_HEATER_PWM_DUTY_REG, lid_PWM_duty);				// Set the lid PWM duty cycle

		m_lPI_out  = (long) (tub_PWM_duty / 10);		// Scale power for downstream output 0..100
		m_lPI_out2 = (long) (lid_PWM_duty / 10);		


		// Console output
		char strTimeStamp[20];
		this->GetDateTimeStamp(strTimeStamp);		// Date: Time string

		format_date(strTimeStamp);

		// Read temp sensors - scaled by 100 to preserve two decimal places.

		float tub_top_primary 		= (float) channel2temp(TEMP_TOP_PRI) / 100.0f;		// Tub Top Primary Temperature
		float tub_top_secondary 	= (float) channel2temp(TEMP_TOP_SEC) / 100.0f;		// Tub Top Secondary Temperature
		float tub_bottom_primary 	= (float) channel2temp(TEMP_BOT_PRI) / 100.0f;		// Tub Bottom Primary Temperature
		float tub_bottom_secondary	= (float) channel2temp(TEMP_BOT_SEC) / 100.0f;		// Tub Bottom Secondary Temperature

		// Electronics PCB temperature Temp = 0.0007684 x counts^2 - 0.50445 x counts + 77.136 
		//float pcb_temperature = ((float)m_nTempFan * (float)m_nTempFan * 0.0007684) - (0.50445 * (float)m_nTempFan) + 77.136; 
		//float amb_temperature = ((float)m_nTempAmbient * (float)m_nTempAmbient * 0.0007684) - (0.50445 * (float)m_nTempAmbient) + 77.136; 
		
		if (m_nDebug & DEBUG_SERVO)
		{
		  printf("SERVO %s, %d, %5.1f, %5.1f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f \n",
			strTimeStamp,
            seconds,
            tub_power,
            lid_power,
            tub_top_primary,
            tub_top_secondary,
            tub_bottom_primary,
            tub_bottom_secondary,
            tub_weighted_temp); 
		}

		float Ipower = pid_tub.get_integrator_power();
		float Ppower = pid_tub.get_proportional_power();
		
		if (m_nDebug & DEBUG_PI)
		{
		  printf("PI %s, %5.1f, %5.1f, %5.1f \n",			
		    strTimeStamp,
            tub_power,
			Ipower,
			Ppower);
		}
		
        if (Ipower > 0)			// Update the stored integrator value only if it's active
        {
           m_NvRam.OutputW(INTEGRATOR_PRESET, (WORD) (Ipower * 100));	// Scaled by 100 to preserve fraction
        }
			
	} // End if HEATER_CONTROL...
    else
	{
		m_IO.OutputW(HEATERS_ENABLE_REG, PWM_DISABLE);	// Disable tub and lid PWM
		m_lPI_out  = 0;									// Output to displays downstream
		m_lPI_out2 = 0;
	}
}

//*********************************************************
float CParserThread::Get_Tub_Temp()
{
	float tub_lower;
	// float tub_upper;
	
	// Get the temperature data from the active sensors
	
	if (m_NvRam.InputB(SENSOR_LOWER_ACTIVE) == PRIMARY)	
		tub_lower = (float) channel2temp(TEMP_BOT_PRI) / 100.0f;	// Tub Bottom Primary Temperature
	else	
		tub_lower = (float) channel2temp(TEMP_BOT_SEC) / 100.0f;	// Tub Bottom Secondary Temperature	

/*  This section removed for two sensor solution
	if (m_NvRam.InputB(SENSOR_UPPER_ACTIVE) == PRIMARY)	
		tub_upper = (float) channel2temp(TEMP_TOP_PRI) / 100.0f;		// Tub Top Primary Temperature
	else
		tub_upper = (float) channel2temp(TEMP_TOP_SEC) / 100.0f;		// Tub Top Secondary Temperature		
	
	//  Calculate the Tub Weighted Temperature
	
	float temp = (tub_upper * 0.5f) + (tub_lower * 0.5f);			// Tub Weighted Temperature (50% of tub top and 50% of tub bottom)
*/	
	return tub_lower;
	
} // End of Get_Tub_Temp

//*********************************************************
void CParserThread::Set_Integrator_Power(float IP)
{
   pid_tub.set_integrator_power(IP);
}

//===========================================================================

/*** Revision History ***

02/18/16    JO      Initial heater changes for Century B.
07/21/16	JO		CBF-16 - Updated for new heater design
07/26/16	JO		CBF-32 - Updated following code review and testing
08/25/16	JO		CBF-40 - Updated following testing
09/13/16    JO      CBF-47 - Update thermal startup and operational state machine.
09/27/16    JO      Fixed comment error from CBF-47
10/03/16	KR		CBF-56 - Redefine Thermal definition
11/10/16    JO      CBF-78 - Add "Lid Off mode" functionality.
11/30/16    KR      CBF-91 - Add pcb temp output to ServoControl.cpp
12/01/16    JO    	CBF-102 - Last minute updates - add ambient temp sensor to printf.
12/05/16    JO    	CBF-104 - Add manual integrator preset functionality via DSM commands and debug console printout.
12/13/16    JO      CBF-113 - Add "Automatic Integrator Preset" functionality. Comments cleanup.
12/14/16    JO    	CBF-117 - Add lid power set to 100% or 30% during warm-up selected by F1 command.
12/15/16    JO    	CBF-121 - Lid warmup ramp via T4 command and manual tub heater integrator power preset.
01/09/17    JO    	CBF-126 - Add active sensor feature.
					 Fixed format_date to use year from input string.
					 Remove cmd_T4 control of 100% lid power feature.
01/30/17	KR		CBF-143 - Remove unused temperature output in DEUG_SERVO
02/03/17	KR		CBF-151 - Additional fixes to run the commands properly & remove ES warning
03/15/17    JO      CBF-158 - Change heaters to use PWM generated by FPGA.
03/21/17    JO      CBF-160 - Limit heaters operation to between +5 deg C to +50 deg C.


$Log: /IcarusBased/SpecFW/ServoControl.cpp $
 *
 * 6     11/11/05 2:57p Frazzitl
 * Corrected problem with printf staement used for debugging
 *
 * 5     11/09/05 1:05p Frazzitl
 * Added debug capabilities and implemented feature that keeps heater in
 * Stablization mode for 15 minutes before going to Normal mode.
 *
 * 4     9/30/05 9:23a Frazzitl
 * Changed variable names to make more readable
 *
 * 3     6/06/05 3:36p Frazzitl
 * Removed warnings during build
 *
 * 2     4/15/05 14:13 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 *
 * 1     3/17/05 11:23 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
