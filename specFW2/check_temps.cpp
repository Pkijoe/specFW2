//===========================================================================
//
//	Module Name:	check_temps.cpp
//
//	Function:		Main heater mode logic and sensor fault detection and accommodation.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "ParserThread.h"
#include "Debug.h"
#include "zodiac.h"
#include <math.h> 

#define TP_SENSOR_FAILED 		1							// Bit mask
#define BP_SENSOR_FAILED 		2							// Bit mask
#define TS_SENSOR_FAILED 		4							// Bit mask
#define BS_SENSOR_FAILED 		8							// Bit mask

#define TP_SENSOR 				0							// Sensor index
#define BP_SENSOR 				1							// Sensor index
#define TS_SENSOR 				2							// Sensor index
#define BS_SENSOR 				3							// Sensor index

#define TEMP_SENSOR_VARIANCE_ERROR_THRESHOLD	2.0			// Maximum variance from average allowable
#define TEMP_SENSOR_RANGE_MIN_ERROR_THRESHOLD	10.0		// Minimum in range temperature allowable
#define TEMP_SENSOR_RANGE_MAX_ERROR_THRESHOLD	60.0		// Maximum in range temperature allowable
#define FAILURE_COUNTER_WARNING_LEVEL			10			// Value at which warnings get generated

static	DWORD	s_dwStartTime = 0;
static  WORD	sensor_warning_status = 0;	// All sensors pass on power-up

struct ts_t {
	float temperature;
	unsigned short range_fail_count;
	unsigned short variance_fail_count;	
};

static ts_t tsensor[4]= {
{0,0,0},	
{0,0,0},
{0,0,0},	
{0,0,0},	
};	


//void CParserThread::Calibrate_Temp_Sensors()
//{
//	// now two instead of four?
	
//}

// NOTE: This fault detection scheme assummes only one out of the four sensor can fail and will not work correctly 
//       in the case of multiple sensor failures.
//       Funtion returns true with no errors.

bool CParserThread::Sensor_Fault_Detection()
{
	bool status = true;
    int done = 0, loop, itemp, s_index[4] = {0,1,2,3};
	float sensor_average = 0, variance[4], ftemp = 0;
	float variance_min = 0, variance_max = 0;
	

	// Get the temperature data
	tsensor[TP_SENSOR].temperature = (float) channel2temp(TEMP_TOP_PRI) / 100.0f;		// Tub Top Primary Temperature
	tsensor[BP_SENSOR].temperature = (float) channel2temp(TEMP_BOT_PRI) / 100.0f;		// Tub Bottom Primary Temperature	
	tsensor[TS_SENSOR].temperature = (float) channel2temp(TEMP_TOP_SEC) / 100.0f;		// Tub Top Secondary Temperature
	tsensor[BS_SENSOR].temperature = (float) channel2temp(TEMP_BOT_SEC) / 100.0f;		// Tub Bottom Secondary Temperature
	
	// ---------------------------------- Range testing ----------------------------------
	
	for (loop = 0;  loop < 4; loop++)
	{
	   // Increment the range fail count if out of range for each sensor otherwise decrement it
	   
	   if (tsensor[loop].temperature < TEMP_SENSOR_RANGE_MIN_ERROR_THRESHOLD ||
		   tsensor[loop].temperature > TEMP_SENSOR_RANGE_MAX_ERROR_THRESHOLD)
		{
		   if (tsensor[loop].range_fail_count < FAILURE_COUNTER_WARNING_LEVEL) tsensor[loop].range_fail_count++;
		}
	   else
	       if (tsensor[loop].range_fail_count > 0) tsensor[loop].range_fail_count--;
	}	

	// Skip further testing if we get a failure
	
	for (loop = 0;  loop < 4; loop++)
	{	
		if (tsensor[loop].range_fail_count == FAILURE_COUNTER_WARNING_LEVEL) goto report_failures;
	}
	
	// ---------------------------------- Variance testing ----------------------------------
	
	// Calculate average
	sensor_average = (float) ((tsensor[0].temperature + tsensor[1].temperature + tsensor[2].temperature + tsensor[3].temperature) / 4.0);		
	
	// Calculate variances
	for (loop = 0;  loop < 4; loop++)
	{
	   variance[loop] = (float) (fabs(sensor_average - tsensor[loop].temperature));  
	}	
	
	// Sort to find max variance - ascending order
	
	do
	{
    	done = TRUE;
    	for (loop = 0;  loop < 3; loop++)
    	{
    	   if (variance[loop] > variance[loop + 1])  // Check if swap needed
    	   {
    		   ftemp = variance[loop + 1];
    		   itemp = s_index[loop + 1];
    		   variance[loop + 1] = variance[loop];
    		   s_index[loop + 1] = s_index[loop];
    		   variance[loop] = ftemp;
    		   s_index[loop] = itemp;
    		   done = FALSE;						// Done when no swapping occurs
    	   }
    	}
	}
    while (!done);

	// At this point the min variance is at array index 0 and max is at array index 3.
	// s_index[3] holds the sensor array index of the sensor with the max variance.
	
	variance_min = variance[0];
	variance_max = variance[3];
	
	// Check to see if a sensor is out of range, if so count the failures
	
	if ((variance_max - variance_min) > TEMP_SENSOR_VARIANCE_ERROR_THRESHOLD)
	{
		// Increment the sensor's fail count
		
		if (tsensor[s_index[3]].variance_fail_count < FAILURE_COUNTER_WARNING_LEVEL)
		{
			tsensor[s_index[3]].variance_fail_count++;
		}

	}
	else	// Since there are no errors in any sensor, decrement each sensor's variance fail count	
	{
		for (loop = 0;  loop < 4; loop++)
		{
			if (tsensor[loop].variance_fail_count > 0) tsensor[loop].variance_fail_count--;
		}	
	}
	
	// ---------------------------------- Report failures as warnings ----------------------------------
report_failures:;

	if (tsensor[TP_SENSOR].range_fail_count == FAILURE_COUNTER_WARNING_LEVEL ||		// Tub Top Primary 
	    tsensor[TP_SENSOR].variance_fail_count == FAILURE_COUNTER_WARNING_LEVEL)
	{
		m_Sensor_Top_Primary_Status = FAIL;
		status = false;
		
		if (!(sensor_warning_status & TP_SENSOR_FAILED))	// Only one warning can be sent per reset event
		{
			sensor_warning_status |= TP_SENSOR_FAILED;
			ErrorMsg((int) NO, TOP_PRIMARY_SENSOR_FAILED);
		}		
	}

	if (tsensor[BP_SENSOR].range_fail_count == FAILURE_COUNTER_WARNING_LEVEL ||		// Tub Bottom Primary 
	    tsensor[BP_SENSOR].variance_fail_count == FAILURE_COUNTER_WARNING_LEVEL)
	{
		m_Sensor_Bottom_Primary_Status = FAIL;
		status = false;
		
        if (!(sensor_warning_status & BP_SENSOR_FAILED))	// Only one warning can be sent per reset event
		{
			sensor_warning_status |= BP_SENSOR_FAILED;
			ErrorMsg((int) NO, BOTTOM_PRIMARY_SENSOR_FAILED);
		}
	}

	if (tsensor[TS_SENSOR].range_fail_count == FAILURE_COUNTER_WARNING_LEVEL ||		// Tub Top Secondary  
	    tsensor[TS_SENSOR].variance_fail_count == FAILURE_COUNTER_WARNING_LEVEL)
	{
        m_Sensor_Top_Secondary_Status = FAIL;
		status = false;
		
	    if (!(sensor_warning_status & TS_SENSOR_FAILED))	// Only one warning can be sent per reset event
        {
        	sensor_warning_status |= TS_SENSOR_FAILED;
	    	ErrorMsg((int) NO, TOP_SECONDARY_SENSOR_FAILED);
        }
	}

	if (tsensor[BS_SENSOR].range_fail_count == FAILURE_COUNTER_WARNING_LEVEL ||		// Tub Bottom Secondary  
	    tsensor[BS_SENSOR].variance_fail_count == FAILURE_COUNTER_WARNING_LEVEL)
	{
		m_Sensor_Bottom_Secondary_Status = FAIL;
		status = false;
		
	    if (!(sensor_warning_status & BS_SENSOR_FAILED))
	    {
	    	sensor_warning_status |= BS_SENSOR_FAILED;
	    	ErrorMsg((int) NO, BOTTOM_SECONDARY_SENSOR_FAILED);
	}   }
	
	// ---------------------------------- Clear the sensor failure status if the count returns to zero ----------------------------------	
	
	if (tsensor[TP_SENSOR].range_fail_count == 0 &&		// Tub Top Primary 
	    tsensor[TP_SENSOR].variance_fail_count == 0)
	{
		m_Sensor_Top_Primary_Status = PASS;
	}
	
	if (tsensor[BP_SENSOR].range_fail_count == 0 &&		// Tub Bottom Primary 
	    tsensor[BP_SENSOR].variance_fail_count == 0)
	{
		m_Sensor_Bottom_Primary_Status = PASS;
	}
	
	if (tsensor[TS_SENSOR].range_fail_count == 0 &&		// Tub Top Secondary  
	    tsensor[TS_SENSOR].variance_fail_count == 0)
	{
        m_Sensor_Top_Secondary_Status = PASS;	
	}
	
	if (tsensor[BS_SENSOR].range_fail_count == 0 &&		// Tub Bottom Secondary  
	    tsensor[BS_SENSOR].variance_fail_count == 0)
	{
		m_Sensor_Bottom_Secondary_Status = PASS;
	}		

	if (m_nDebug & (DEBUG_FDR | DEBUG_FDV))   // Common debug
	{
	  printf("COM1 TP= %4.2f  BP= %4.2f  TS= %4.2f  BS= %4.2f  avg= %4.2f \n",
	  tsensor[TP_SENSOR].temperature,		  
	  tsensor[BP_SENSOR].temperature,		    
	  tsensor[TS_SENSOR].temperature,            
	  tsensor[BS_SENSOR].temperature,
	  sensor_average );
	  
	  printf("COM2 ST= %4d  ST= %4d  ST= %4d  ST= %4d sws= %4d \n",
	  m_Sensor_Top_Primary_Status,		  
	  m_Sensor_Bottom_Primary_Status,		    
	  m_Sensor_Top_Secondary_Status,            
	  m_Sensor_Bottom_Secondary_Status,
	  sensor_warning_status);
	}	

	if (m_nDebug & DEBUG_FDR)   // Range debug
	{
	      printf("RNG  FC= %4d  FC= %4d  FC= %4d  FC= %4d  \n",
	      tsensor[TP_SENSOR].range_fail_count,		  
	      tsensor[BP_SENSOR].range_fail_count,		    
	      tsensor[TS_SENSOR].range_fail_count,            
	      tsensor[BS_SENSOR].range_fail_count);
	}
    
	if (m_nDebug & DEBUG_FDV)	// Variance debug
	{
	  printf("VAR  FC= %4d  FC= %4d  FC= %4d  FC= %4d  MN= %4.2f  MX= %4.2f  \n",			
	      tsensor[TP_SENSOR].variance_fail_count,		  
	      tsensor[BP_SENSOR].variance_fail_count,		    
	      tsensor[TS_SENSOR].variance_fail_count,            
	      tsensor[BS_SENSOR].variance_fail_count,
		  variance_min,
		  variance_max);
	}


	
	return (status);
}

	
void CParserThread::check_temps()
{
	// Just in case we go directly to STABILIZATION without entering WARMUP
	// This insures the 15 minute timer for STABILIZATION will work correctly
	
	if (s_dwStartTime == 0)
		s_dwStartTime = GetTickCount();

	// Determine if the temperature sensors are working

	//**********************************************************************************

	// See if we are starting for the first time and need to run the temperature sensor calibration function automatically
	if (m_Calibration_Flag == 0)		
	{
//		Calibrate_Temp_Sensors();		// Perform sensor calibration
		m_Calibration_Flag = 1;
		m_NvRam.OutputL(CALIBRATE_SENSORS_INIT, MAGIC_NUMBER);	// Turn off any future automatic sensor calibration
	}
	
	m_Temp_Sensors_OK = Sensor_Fault_Detection();		// Determine if temperature sensors are working properly

	//**********************************************************************************

	//                                 Heater Mode Logic
	
	float tub_temperature = Get_Tub_Temp();	
	
	if (m_NvRam.InputB(HEATER_CONTROL) == '1')						// Execute logic only if heater is on"
	{
		// If we exit MFG or READY op-mode then exit LID_OFF mode
		
	    if (((m_cOperationMode != MFG) & (m_cOperationMode != READY)) && m_nHeater_mode == MODE_LID_OFF)
	     {	
	    		setHeaterMode(MODE_WARMUP);							// Exit LID_OFF mode and enter WARMUP mode
	    		m_bReached_temp = false;
	    		if (m_nDebug & DEBUG_MISC)
	    			printf("Heat Mode changing from LID-OFF to WARM-UP\r\n");					
	     }	
	   
		if (m_cOperationMode == MFG || m_cOperationMode == READY)   	// LID_OFF mode only works if in MFG and READY op-mode
		{ 
			// Check if we are in LID_OFF mode and lid has been put back on.  If so, enter WARMUP mode.
		    
		    if (m_nHeater_mode == MODE_LID_OFF)
		      {	
		         if ((m_IO.InputW(STATUS_REG2) & LID_STATUS) == 0)		// Check lid status;  0 = ON (connected)
		    	 {
		    		setHeaterMode(MODE_WARMUP);							// Exit LID_OFF mode and enter WARMUP mode
		    		m_bReached_temp = false;
		    		if (m_nDebug & DEBUG_MISC)
		    			printf("Heat Mode changing from LID-OFF to WARM-UP\r\n");					
		    	 }
		      }
		    else  // Check if lid has been removed. If so, enter LID_OFF mode.
		     {
		         if ((m_IO.InputW(STATUS_REG2) & LID_STATUS))			// Check lid status;  0 = ON (connected)
		    	 {

		    		setHeaterMode(MODE_LID_OFF);						// Enter LID_OFF mode

		    		if (m_nDebug & DEBUG_MISC)
		    			printf("Heat Mode changing to LID-OFF\r\n");					
		    	 }
	   	     }
		 }
		 
		// If in WARMUP mode and the Tub Temp >= TEMP_STABILIZATION go to STABILIZATION mode

		if (m_nHeater_mode == MODE_WARMUP)
		{
			if (tub_temperature >= TEMP_STABILIZATION)
			{
				if (++m_cWarm2StableCount >= 12)	// We need 12 in a row to confirm we got there
				{
					m_bReached_temp = true;
					setHeaterMode(MODE_STABILIZATION);
					s_dwStartTime = GetTickCount();
					if (m_nDebug & DEBUG_MISC)
						printf("Heat Mode changing from WARM-UP to STABILIZATION\r\n");
				}
			}
			else
			{
				m_cWarm2StableCount	= 0;
			}
		}
		else if (m_nHeater_mode == MODE_STABILIZATION)
		{
			// If in STABILIZATION mode and STABILIZATION_TIME has passed, go to NORMAL mode
			
			if ((GetTickCount() - s_dwStartTime) >= STABILIZATION_TIME)
			{
				setHeaterMode(MODE_NORMAL);
				if (m_nDebug & DEBUG_MISC)
					printf("Heat Mode changing from STABILIZATION to NORMAL\r\n");
			}
		}
		
		if ((m_nHeater_mode == MODE_NORMAL) || (m_nHeater_mode == MODE_STABILIZATION))
		{
			// If in STABILIZATION or NORMAL mode and the Tub Temp < TEMP_STABILIZATION go to WARMUP mode
			
			if (tub_temperature < TEMP_STABILIZATION)
			{
				if (++m_cNormal2WarmCount >= 12)	// We need 12 in a row to confirm we got there
				{	
					setHeaterMode(MODE_WARMUP);		// Fell out of normal mode
					m_bReached_temp = false;
					if (m_nDebug & DEBUG_MISC)
						printf("Heat Mode changing to WARM-UP\r\n");					
				}
			}
			else
			{
				m_cNormal2WarmCount	= 0;
			}
		}

	}
}

//---------------------------------------------------------------------------
//  setHeaterMode  ... Saves the tick count when m_nHeater_mode state changes
//
void CParserThread::setHeaterMode(short nsHM)
{
	if ( m_nHeater_mode != nsHM )
	{
		m_nHeater_mode = nsHM;
		m_dwHeaterModeSetTime = GetTickCount();
		
		// Place temps into spectrometer log
		{
			float tub_top_primary_temp		= (float) channel2temp(TEMP_TOP_PRI) / 100.0f;
			float tub_bottom_primary_temp	= (float) channel2temp(TEMP_BOT_PRI) / 100.0f;
			float tub_top_secondary_temp	= (float) channel2temp(TEMP_TOP_SEC) / 100.0f;
			float tub_bottom_secondary_temp	= (float) channel2temp(TEMP_BOT_SEC) / 100.0f;	
			float tub_weighted_temp = Get_Tub_Temp();

			float CcdTempUV	= (float) GetTECoolerTemperature( m_nTempccdUV)  / 100.0f;
			float CcdTempVI	= (float) GetTECoolerTemperature( m_nTempccdVIS) / 100.0f;
			char* szStr = new char[120];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Temps: %6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f \0", 
									tub_top_primary_temp,
									tub_bottom_primary_temp,
									tub_top_secondary_temp,
									tub_bottom_secondary_temp,
									tub_weighted_temp, 
									CcdTempUV, CcdTempVI);
			AddToSpecLog(szStr);
		}
		
		// Place Heater control mode into spectrometer log
		{
			char szBuffer[12];
			getHeaterModeString(szBuffer);
			char* szStr = new char[50];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Heater: %s\0", szBuffer);
			AddToSpecLog(szStr);
		}		
	}
}

//===========================================================================

/*** Revision History ***

09/13/16    JO      CBF-47 - Update thermal startup and operational state machine.
09/27/16    JO      Fixed spelling error from CBF-47.
10/20/16    JO      CBF-54 - Change heater related names/definitions, fix broken heater commands.
11/17/16    JO      CBF-78 - Add "Lid Off mode" functionality.
12/02/16    JO      CBF-106 - Move thermal setpoint definitions to zodiac.h.
12/06/16	KR		CBF-109	- Add the temperature sensors readout to spectrometer log
01/12/17    JO    	CBF-125 - Add temperature sensor fault detection.
01/18/17    JO    	CBF-96  - Re-enable lid off mode.
03/27/17    JO    	CBF-162 - Allow lid-off mode to operate in READY mode in addition to MFG mode

** End of Rev History **/
