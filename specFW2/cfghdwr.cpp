//===========================================================================
//
//	Module Name:	cfgHdwr.cpp
//
// 	Function:		This routine is a special function that initializes the
//					hardware if it is not in the READY state.  The routine
//					determines if a COLD or HOT start should be performed.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "Debug.h"

long CParserThread::calc_warmup_time(void)		// Calculate warmup time
{
	long lTub, warmup_time;
	
	lTub = (long) (Get_Tub_Temp() * 100.0);		// Tub Temp * 100
															
	if (lTub < TUB_TEMP_COLD_THRESHOLD)
	  {
	    warmup_time = (TUB_WARM_UP_RATE * (TUB_TEMP_COLD_THRESHOLD - lTub)) / 100;	
	  }
	else
		warmup_time = 0;						// Tub already at temperature

	return(warmup_time);
}


unsigned int CParserThread::Configure_Motors(unsigned int slit_flag)	
{
	char 			strTimeStamp[20];
	unsigned int	status(NO_ERRORS);
	char			temp_ViewType;	

	if (m_argon_flag) goto exit_function;	// Skip the motors configuration
	
	if (slit_flag)
	{
	   this->GetDateTimeStamp(strTimeStamp);			// Date: Time string
	   printf("%s  cfgHdwr cmdCS()... HOT state, Configure Slit\r\n", strTimeStamp);
	   if ((status = cmdCS()) != NO_ERRORS)				// Configure SLIT
	   {
	   	 ErrorMsg((int) YES, status);
	  	 goto exit_function;
	   }
	}
	
	temp_ViewType = m_cViewType;   // Save the current viewtype
	
	m_cViewType = AXIAL_TYPE;      // Set the viewtype to AXIAL
	this->GetDateTimeStamp(strTimeStamp);			// Date: Time string			
	printf("%s  cfgHdwr cmdCX()... HOT state, Configure AXIAL View X Motor\r\n", strTimeStamp);				
	if ((status = cmdCX()) != NO_ERRORS)
	{
		ErrorMsg((int) YES, status);
		goto exit_function;
	}
	
	m_cViewType = RADIAL_TYPE;      // Set the viewtype to RADIAL
	this->GetDateTimeStamp(strTimeStamp);			// Date: Time string			
	printf("%s  cfgHdwr cmdCX()... HOT state, Configure RADIAL View X Motor\r\n", strTimeStamp);				
	if ((status = cmdCX()) != NO_ERRORS)
	{
		ErrorMsg((int) YES, status);
		goto exit_function;
	}			
	
	m_cViewType = AXIAL_TYPE;      // Set the viewtype to AXIAL	
	this->GetDateTimeStamp(strTimeStamp);			// Date: Time string			
	printf("%s  cfgHdwr cmdCY()... HOT state, Configure AXIAL View Y Motor\r\n", strTimeStamp);
	if ((status = cmdCY()) != NO_ERRORS)	
	{
		ErrorMsg((int) YES, status);
		goto exit_function;
	}
	
	m_cViewType = RADIAL_TYPE;      // Set the viewtype to RADIAL
	this->GetDateTimeStamp(strTimeStamp);			// Date: Time string			
	printf("%s  cfgHdwr cmdCY()... HOT state, Configure RADIAL View Y Motor\r\n", strTimeStamp);
	if ((status = cmdCY()) != NO_ERRORS)	
	{
		ErrorMsg((int) YES, status);
		goto exit_function;
	}
	
	m_cViewType = temp_ViewType;   // Restore the current viewtype
	this->GetDateTimeStamp(strTimeStamp);			// Date: Time string
	printf("%s  cfgHdwr cmdHA()... Configure RADIAL Toroid Motor\r\n", strTimeStamp);
	if ((status = cmdHA()) != NO_ERRORS)	
	{
		ErrorMsg((int) YES, status);
		goto exit_function;
	}
	
	this->GetDateTimeStamp(strTimeStamp);			// Date: Time string			
	printf("%s  cfgHdwr cmdHB()... Configure Shutter Motor\r\n", strTimeStamp);
	if ((status = cmdHB()) != NO_ERRORS)	
	{
		ErrorMsg((int) YES, status);
		goto exit_function;
	}
	
exit_function:;

	return (status); 		
}


unsigned int CParserThread::cfgHdwr()
{
	unsigned int	status(NO_ERRORS);
	WORD			down_long;
	short			hardware_init = NO;
	short			window_timer;
	short			filter[10];
	short			filter_index;
	short			filter_size;
	long			filter_sum;
	short			filter_average;
	short			old_average;
	short			loop_cycles;
	long			lOldTime;
	long			lMaxWarmup = MAX_WARM_UP;
	//.......................................................
	// local definitions for protected instance parameters 
	//  All initialized to zero for Good Practice
	char			cStartupMode = 0;
	short			nHeater_mode   = 0;
	short			nTubTemp	= 0;
	BYTE			bytLastPurge= 0;
	long			lTimeNow	= 0;
	long			lONtime		= 0;
	unsigned short	nWarning	= 0;
	unsigned short	nUVcooler	= 0;
	unsigned short	nVIScooler	= 0;
	unsigned short	nTempccdUV	= 0;
	unsigned short	nTempccdVIS	= 0;
	bool			bStartedInColdState = false;  // Indicates initial start point
	char 			strTimeStamp[20];	
	//.......................................................
	m_nLooker = 0;
	

	//=====================================
	// Put Argon & Nitrogen valves on High
	//=====================================
	m_nSolenoidRegister |= (ARGON_EN | ARGON_HI | NITROGEN_HI);
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
	m_cCurrentPurge = HIGH;

	//====================================
	// Check time from turn off until now
	//====================================

	// Get time system was turned off - Retrieve into member variables: m_cOFFyear through m_cOFFsecond	

	if (m_argon_flag)
	{
		m_lOFFtime = GetOffTime(ARGON_TIME);	
	}else
	{
		m_lOFFtime = GetOffTime(NORMAL_TIME); 		
	}

	printf("ONtime %x OFFtime %x\r\n", m_lONtime, m_lOFFtime);
	printf("ONyear %x ONmonth %x ONdate %x\r\n", m_cONyear, m_cONmonth, m_cONdate);
	printf("OFFyear %x OFFmonth %x OFFdate %x\r\n", m_cOFFyear, m_cOFFmonth, m_cOFFdate);

	if ((m_cONyear == m_cOFFyear) && (m_cONmonth == m_cOFFmonth))
	{
		if (m_cONdate == m_cOFFdate)
		{
			if ((m_lONtime - m_lOFFtime) > MAX_OFF_TIME)
				down_long = YES;
			else
				down_long = NO;
		}
		else
		{
			if ((m_cONdate - m_cOFFdate) == 1)
			{
				if (m_lONtime > m_lOFFtime)
					down_long = YES;
				else if ((m_lONtime + (24 * 3600) - m_lOFFtime) > MAX_OFF_TIME)
					down_long = YES;
				else
					down_long = NO;
			}
			else
				down_long = YES;
		}
	}
	else
		down_long = YES;

	//================================
	// Turn on Water to the detectors
	// And also the chiller
	//================================
	m_nSolenoidRegister |= WATER_EN;
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
	m_nDigitalControl |= CHILLER_ENABLED;
	m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
	//...........................................................
	// Cold mode entry placed into the Spectrometer log.
	if (m_cStartupMode == COLD)
	{
		char* szStr = new char[40];
		GetDateTimeStamp(szStr);
		sprintf(&szStr[19], " Mode: Cold\0");
		AddToSpecLog(szStr);
	}
	//...........................................................
ColdStart:
	theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
	if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
	{
		theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release critical parameters
		return m_nFatalCode;
	}
	cStartupMode = m_cStartupMode;
	theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection

	if (cStartupMode == COLD)
	{
		//............................................................................
		//  Define parameters used in this block
		theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
		nTubTemp = (short) (Get_Tub_Temp() * 100.0);		// Tub temperature
		bytLastPurge = m_NvRam.InputB(LAST_PURGE);
		theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
		bStartedInColdState = true;							// Started in cold state
		//............................................................................
		this->GetDateTimeStamp(strTimeStamp);				// Date: Time string
		printf("%s  cfgHdwr going to COLD\r\n", strTimeStamp);
		
		if (nTubTemp < TUB_TEMP_COLD_THRESHOLD || down_long == YES) //|| bytLastPurge == HIGH)   Remove the high purge condition
		{
			theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Protect critical parameters
				return m_nFatalCode;
			}
			m_cStartupMode = COLD1;
			//...........................................................
			// Cold1 mode entry placed into the Spectrometer log.
			char* szStr = new char[40];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Mode: Cold1\0");
			AddToSpecLog(szStr);
			//...........................................................
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			this->GetDateTimeStamp(strTimeStamp);				// Date: Time string			
			printf("%s  cfgHdwr in COLD1\r\n", strTimeStamp);
			theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
			m_NvRam.OutputB(LAST_PURGE, HIGH);					// Set High Purge
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			//====================================
			// Get current time related to ONtime
			//====================================
			lOldTime = getTime(YES);  // m_lCurrentTime

			lMaxWarmup = calc_warmup_time();
			
			EnterCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
//			m_lKeyTime = MAX_PURGE_TIME + m_lONtime - lOldTime + MIN_PURGE_TIME + MIN_COOLER_TIME + lMaxWarmup;
//################################################################################################################################
// For now use the (lMaxWarmup / 2) term to provide a better estimate for syngistix

			m_lKeyTime = MAX_PURGE_TIME + m_lONtime - lOldTime + MIN_PURGE_TIME + MIN_COOLER_TIME + (lMaxWarmup / 2);
			
//################################################################################################################################			
			
			LeaveCriticalSection(&m_CriticalSectionClock); // Release protection
			
			//==============================================
			// Configure Motors & move to Default positions
			//==============================================
			if (hardware_init == NO)
			  {
			     status = Configure_Motors(TRUE);				// Configure all motors including the slit
			     if (status != NO_ERRORS) return (status);      // Exit since we went fatal
			   
			     hardware_init = YES;   						// Mark as completed 
			  }
			
			//======================================
			// Wait for minimum Casting temperature
			//======================================
			EnterCriticalSection(&m_CriticalSectionClock);			// Protect critical clock parameters
			m_bHolding = false;
			LeaveCriticalSection(&m_CriticalSectionClock);			// Release clock protection
			theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				return m_nFatalCode;
			}
			m_cStartupMode = COLD2;
			//...........................................................
			// Cold2 mode entry placed into the Spectrometer log.
			{
				char* szStr = new char[40];
				GetDateTimeStamp(szStr);
				sprintf(&szStr[19], " Mode: Cold2\0");
				AddToSpecLog(szStr);
			}
			//...........................................................
			theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
			this->GetDateTimeStamp(strTimeStamp);					// Date: Time string
			printf("%s  cfgHdwr in COLD2\r\n", strTimeStamp);
			window_timer = 0;
			filter_index = 0;
			filter_sum = 0;
			filter_average = 6000;
			filter_size = 0;
			loop_cycles = 10;

			short	nCheckCount(0);

			do
			{
				Sleep(1000);		// wait 1 second
				//........................................................................
				//  Define parameters used in this loop
				theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
				if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
				{
					theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
					return m_nFatalCode;
				}
				nHeater_mode = m_nHeater_mode;
				nTubTemp = (short) (Get_Tub_Temp() * 100.0);			// Get tub temperature
				nWarning = m_nWarning;
				theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
				//........................................................................
				// check if in WARM_UP mode
				if (nHeater_mode == MODE_WARMUP)
				{
					if (filter_size == 10)
						filter_sum -= filter[filter_index];
					else
						filter_size++;
					filter[filter_index] = nTubTemp;
					filter_sum += filter[filter_index];
					filter_index++;
					if (filter_index >= 10)
						filter_index = 0;
					loop_cycles--;
					if (loop_cycles <= 0)
					{
						lOldTime = lTimeNow;  // Assigned below in previous iteration 
						
						lMaxWarmup = calc_warmup_time();
	
// TLF - changing 15 minutes to 5 minutes
//						loop_cycles = 900;	// 15 minutes
						loop_cycles = 300;	//  5 minutes
						old_average = filter_average;
						filter_average = (short) (filter_sum / filter_size);
//						printf("new %hd old %hd sum %d index %hd size %hd\r\n",
//						filter_average, old_average, filter_sum, filter_index, filter_size);
						++nCheckCount;
						if (filter_average <= old_average && nCheckCount >= 4)
						{
							// TLF - Added some additional debugging
							// Moved debugging so a single bad reading will be displayed on HyperTerminal
							if (m_nDebug & DEBUG_TEMPS)
							{
								char	nBuffer[9];
								GetTimeStamp(&nBuffer[0]); // Get (9) char string xx:xx:xx

								printf("%8s - Tub not warming after 5 minutes: Old temp = %4i, New temp = %4i\r\n", nBuffer, old_average, filter_average);
							}
							// TLF - Since the Tub Tub changes every 800 msec, 3 bad readins is enough
							if (++m_cTubWarmupCount >= 3)
							{
								ErrorMsg((int) YES, ERR_CASTING_TEMP_FAILED);
								return(status);
							}
						}
						else
						{
							m_cTubWarmupCount = 0;
						}
					}
				}

				//====================================
				// Get current time related to ONtime
				//====================================
				lTimeNow = getTime(YES);  // m_lCurrentTime // Note: lTimeNow is used above (subsequent iteration)

				EnterCriticalSection(&m_CriticalSectionClock);		// Protect critical clock parameters
				if (!m_bHolding)
				{
					// Don't include the amount of time already ON - this adds an error
					
//					m_lKeyTime = MAX_PURGE_TIME + lOldTime - lTimeNow + MIN_PURGE_TIME + MIN_COOLER_TIME + lMaxWarmup;
//################################################################################################################################
// For now use the (lMaxWarmup / 2) term to provide a better estimate for syngistix

			m_lKeyTime = MAX_PURGE_TIME + lOldTime - lTimeNow + MIN_PURGE_TIME + MIN_COOLER_TIME + (lMaxWarmup / 2);
			
//################################################################################################################################						
				}
				if (m_lKeyTime < MIN_PURGE_TIME + MIN_COOLER_TIME)
					m_lKeyTime = MIN_PURGE_TIME + MIN_COOLER_TIME;

				// Check if taking too long
				if ((!m_bHolding) && ((lTimeNow - m_lONtime) > (MAX_PURGE_TIME + MAX_WARM_UP)))
				{
					m_bHolding = true;
					m_lKeyTime = 900;
					if (!(CASTING_FLG & nWarning))
					{
//						ErrorMsg((int) NO, status);
// TLF - If the Tub has not yet reached temperature, there is something wrong - so quit
						LeaveCriticalSection(&m_CriticalSectionClock); // Release protection
						ErrorMsg((int) YES, ERR_CASTING_TEMP_FAILED);
						return(status);
					}
				}
				LeaveCriticalSection(&m_CriticalSectionClock);			// Release clock protection
			}
			while ((nHeater_mode != MODE_NORMAL) && m_Temp_Sensors_OK);

			//=============================
			// Wait for maximum Purge time
			//=============================
			theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				return m_nFatalCode;
			}
			m_cStartupMode = COLD3;
			//...........................................................
			// Cold3 mode entry placed into the Spectrometer log.
			{
				char* szStr = new char[40];
				GetDateTimeStamp(szStr);
				sprintf(&szStr[19], " Mode: Cold3\0");
				AddToSpecLog(szStr);
			}
			//...........................................................
			theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
			this->GetDateTimeStamp(strTimeStamp);					// Date: Time string			
			printf("%s  cfgHdwr in COLD3\r\n", strTimeStamp);
			do
			{
				Sleep(1000);		// wait 1 second

				theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
				if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
				{
					theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
					return m_nFatalCode;
				}
				theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
				//====================================
				// Get current time related to ONtime
				//====================================
				lTimeNow = getTime(YES);  // m_lCurrentTime
				
				EnterCriticalSection(&m_CriticalSectionClock);		// Protect critical clock parameters
				lONtime = m_lONtime;
				if (!m_bHolding)
					m_lKeyTime = MAX_PURGE_TIME + lONtime - lTimeNow + MIN_PURGE_TIME + MIN_COOLER_TIME;
				LeaveCriticalSection(&m_CriticalSectionClock);		// Release protection	
			}
			while ((lTimeNow - lONtime) < MAX_PURGE_TIME);

			theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)	
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				return m_nFatalCode;
			}
			m_cStartupMode = HOT;
			//...........................................................
			// Hot mode entry placed into the Spectrometer log.
			{
				char* szStr = new char[40];
				GetDateTimeStamp(szStr);
				sprintf(&szStr[19], " Mode: Hot\0");
				AddToSpecLog(szStr);
			}
			//...........................................................
			theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		}
		else
		{
			//=========================
			// Cold Start not required
			//=========================
			theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				return m_nFatalCode;
			}
			m_cStartupMode = HOT;
			//...........................................................
			// Hot mode entry placed into the Spectrometer log.
			char* szStr = new char[40];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Mode: Hot\0");
			AddToSpecLog(szStr);
			//...........................................................
			theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		}
	}  // Endif (cStartupMode == COLD)

	theApp.EnterCriticalSection1(&m_CriticalSection);			// Protect critical parameters
	if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
	{
		theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		return m_nFatalCode;
	}
	cStartupMode = m_cStartupMode;
	theApp.LeaveCriticalSection1(&m_CriticalSection);			// Release protection
	
	if (cStartupMode == HOT)
	{
		this->GetDateTimeStamp(strTimeStamp);					// Date: Time string		
		printf("%s  cfgHdwr going to HOT\r\n", strTimeStamp);
		theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
		if ( nTubTemp < TUB_TEMP_HOT_THRESHOLD || (! bStartedInColdState) )
		{
			//=====================
			// Cold Start required
			//=====================
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				return m_nFatalCode;
			}
			m_cStartupMode = COLD;
			//...........................................................
			// Cold mode entry placed into the Spectrometer log.
			{
				char* szStr = new char[40];
				GetDateTimeStamp(szStr);
				sprintf(&szStr[19], " Mode: Cold\0");
				AddToSpecLog(szStr);
			}
			//...........................................................
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			this->GetDateTimeStamp(strTimeStamp);				// Date: Time string
			printf("%s  cfgHdwr back to COLD\r\n", strTimeStamp);
			goto ColdStart;
		}

		//========================
		// Get new turned on time
		//========================
		EnterCriticalSection(&m_CriticalSectionClock);			// Protect critical clock parameters
		GetOnTime();
		if (m_bHolding)
			m_bHolding = false;

		//====================================
		// Get current time related to ONtime
		//====================================
		getTime(YES);  // m_lCurrentTime
		
		m_lKeyTime = MIN_PURGE_TIME + m_lONtime - m_lCurrentTime + MIN_COOLER_TIME;
		LeaveCriticalSection(&m_CriticalSectionClock);			// Release clock protection
		theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		
		//================================================
		// Reconfigure Motors & move to Default positions
		//================================================
		
		if (hardware_init == NO)
		  {
			 status = Configure_Motors(TRUE);			// Configure all motors including the slit
			 if (status != NO_ERRORS) return (status);  // Exit since we went fatal
			 
			 hardware_init = YES;           			// Mark as completed 
		  }

		//=============================
		// Wait for minimum Purge time
		//=============================
		theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
		if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
		{
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			return m_nFatalCode;
		}
		m_cStartupMode = HOT1;
		//...........................................................
		// Hot1 mode entry placed into the Spectrometer log.
		{
			char* szStr = new char[40];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Mode: Hot1\0");
			AddToSpecLog(szStr);
		}
		//...........................................................
		theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		this->GetDateTimeStamp(strTimeStamp);					// Date: Time string		
		printf("%s  cfgHdwr in HOT1\r\n", strTimeStamp);
		do
		{
			Sleep(1000);		// wait 1 second

			theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				return m_nFatalCode;
			}
			theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
			//====================================
			// Get current time related to ONtime
			//====================================
			lTimeNow = getTime(YES);  // m_lCurrentTime

			EnterCriticalSection(&m_CriticalSectionClock);			// Protect critical clock parameters
			lONtime = m_lONtime;
			m_lKeyTime = MIN_PURGE_TIME + lONtime - lTimeNow + MIN_COOLER_TIME;
			LeaveCriticalSection(&m_CriticalSectionClock);			//Release protection
		}
		while ((lTimeNow - lONtime) < MIN_PURGE_TIME);

		//==========================
		// Turn Detector Coolers on
		//==========================
		if (m_bSingleChannel == true)
			m_nDigitalControl |= TEC_EN_UV;
		else
			m_nDigitalControl |= (TEC_EN_UV | TEC_EN_VIS);
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);

		//========================
		// Get new turned on time
		//========================
		GetOnTime();
		//==================================
		// Give Coolers a chance to respond
		//==================================
		WaitDelay((int) 15000);

		//===============================
		// Wait for minimum cooling time
		//===============================
		theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
		if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
		{
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			return m_nFatalCode;
		}
		m_cStartupMode = HOT2;
		//...........................................................
		// Hot2 mode entry placed into the Spectrometer log.
		{
			char* szStr = new char[40];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Mode: Hot2\0");
			AddToSpecLog(szStr);
		}
		//...........................................................
		theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		this->GetDateTimeStamp(strTimeStamp);					// Date: Time string		
		printf("%s  cfgHdwr in HOT2\r\n", strTimeStamp);
		do
		{
			Sleep(1000);		// wait 1 second

			theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);// Release protection
				return m_nFatalCode;
			}
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			//====================================
			// Get current time related to ONtime
			//====================================
			lTimeNow = getTime(YES);  // m_lCurrentTime

			EnterCriticalSection(&m_CriticalSectionClock);		// Protect critical clock parameters
			lONtime = m_lONtime;
			m_lKeyTime = MIN_COOLER_TIME + lONtime - lTimeNow;
			LeaveCriticalSection(&m_CriticalSectionClock);		// Release protection

			//===================================
			// Check UV & VIS TE Cooler voltages
			//  (Note: m_nUVcooler is set in the GetStatusInitThread)
			//===================================
			theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
			nUVcooler = m_nUVcooler;
			nVIScooler = m_nVIScooler;
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection

			++m_cUVCoolerCount;
			if (nUVcooler > UV_COOLER_MAX && m_cUVCoolerCount >= 3)
			{
				if (!(UV_COOLER_FLG & Getm_nFatal( )))
				{
					ErrorMsg((int) YES, ERR_UV_COOLER_MAX);
					return(ERR_UV_COOLER_MAX);
				}
			}
			else if (nUVcooler < UV_COOLER_LOW && m_cUVCoolerCount >= 3)
			{
				if (nUVcooler < UV_COOLER_MIN)
				{
					if (!(UV_COOLER_FLG & Getm_nFatal( )))
					{
						ErrorMsg((int) YES, ERR_UV_COOLER_MIN);
						return(ERR_UV_COOLER_MIN);
					}
				}
				else
				{
					if (!(UV_COOLER_FLG & Getm_nWarning()))
						ErrorMsg((int) NO, WARN_UV_COOLER_LOW);
				}
			}
			else if ((UV_COOLER_FLG & Getm_nWarning())
					&& (nUVcooler > UV_COOLER_LOW_OK)
					&& (nUVcooler < UV_COOLER_HIGH_OK))
			{
				ErrorMsg((int) OK, MSG_UV_COOLER_OK);
				m_cUVCoolerCount = 0;
			}
			else
			{
				m_cUVCoolerCount = 0;
			}

			if (m_bSingleChannel == false)
			{
				++m_cVISCoolerCount;
				if (nVIScooler > VIS_COOLER_MAX && m_cVISCoolerCount >= 3)
				{
					if (!(VIS_COOLER_FLG & Getm_nFatal( )))
					{
						ErrorMsg((int) YES, ERR_VIS_COOLER_MAX);
						return(ERR_VIS_COOLER_MAX);
					}
				}
				else if (nVIScooler < VIS_COOLER_LOW && m_cVISCoolerCount >= 3)
				{
					if (nVIScooler < VIS_COOLER_MIN)
					{
						if (!(VIS_COOLER_FLG & Getm_nFatal( )))
						{
							ErrorMsg((int) YES, ERR_VIS_COOLER_MIN);
							return(ERR_VIS_COOLER_MIN);
						}
					}
					else
					{
						if (!(VIS_COOLER_FLG & Getm_nWarning()))
							ErrorMsg((int) NO, WARN_VIS_COOLER_LOW);
					}
				}
				else if ((VIS_COOLER_FLG & Getm_nWarning())
						&& (nVIScooler > VIS_COOLER_LOW_OK)
						&& (nVIScooler < VIS_COOLER_HIGH_OK))
				{
					ErrorMsg((int) OK, MSG_VIS_COOLER_OK);
					m_cVISCoolerCount = 0;
				}
				else
				{
					m_cVISCoolerCount = 0;
				}
			}
		}
		while ((lTimeNow - lONtime) < MIN_COOLER_TIME);

		//========================
		// Turn Detector power on
		//========================
		theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
		m_nPowerRegisterUV  |= POWER_ON;
		m_nPowerRegisterUV  &= ~LOW_SPEED;
		m_CameraIO.UVOutputW( POWER_SHUTDOWN_UV,  m_nPowerRegisterUV);
		if (m_bSingleChannel == false)
		{
			m_nPowerRegisterVIS |= POWER_ON;
			m_nPowerRegisterVIS &= ~LOW_SPEED;
			m_CameraIO.VISOutputW(POWER_SHUTDOWN_VIS, m_nPowerRegisterVIS);
		}
		m_bCCDPower = true;
		m_CameraIO.UVOutputW( TIME_MODE_UV, CAMERA_READY);
		if (m_bSingleChannel == false)
		{
			m_CameraIO.VISOutputW(TIME_MODE_VIS, CAMERA_READY);
		}

		//====================================
		// Put Argon & Nitrogen valves on Low
		//====================================
		m_nSolenoidRegister    &= ~(ARGON_HI | NITROGEN_HI);
		m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
		m_cCurrentPurge = LOW;
		m_NvRam.OutputB(LAST_PURGE, LOW);
		theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection

		//========================
		// Get new turned on time
		//========================

		EnterCriticalSection(&m_CriticalSectionClock);	// Protect critical clock parameters
		GetOnTime();
		m_lKeyTime = 1;
		LeaveCriticalSection(&m_CriticalSectionClock); // Release protection
		//====================================
		// Give Detectors a chance to respond
		//====================================
		WaitDelay((int) 1000);

		//===============================
		// Wait for Cooler temperature value
		//===============================
		theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
		if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
		{
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			return m_nFatalCode;
		}
		m_cStartupMode = HOT3;
		//...........................................................
		// Hot3 mode entry placed into the Spectrometer log.
		{
			char* szStr = new char[40];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Mode: Hot3\0");
			AddToSpecLog(szStr);
		}
		//...........................................................
		theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		
		this->GetDateTimeStamp(strTimeStamp);					// Date: Time string
		printf("%s  cfgHdwr in HOT3\r\n", strTimeStamp);
		do
		{
			Sleep(1000);		// wait 1 second

			theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
			if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
			{
				theApp.LeaveCriticalSection1(&m_CriticalSection);// Release protection
				return m_nFatalCode;
			}
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			//====================================
			// Get current time related to ONtime
			//====================================
			lTimeNow = getTime(YES);  // m_lCurrentTime

			EnterCriticalSection(&m_CriticalSectionClock);		//	Protect Critical clock parameters	
			lONtime = m_lONtime;
			LeaveCriticalSection(&m_CriticalSectionClock);		//	Release protection
			//===================================
			// Check for CCD cooling temperature
			//===================================

			theApp.EnterCriticalSection1(&m_CriticalSection);			//	Protect Critical parameters	
			nTempccdUV  = m_nTempccdUV;
			nTempccdVIS = m_nTempccdVIS;

			if ( nTempccdUV < UV_CCD_TEMP_MIN )
			{
				if ( ! ( UV_CCD_FLG & Getm_nFatal( ) ) )
				{
					ErrorMsg( ( int ) YES, ERR_UV_CCD_MIN );
					theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
					return(ERR_UV_CCD_MIN);
				}
			}
			else if ( nTempccdUV > UV_CCD_TEMP_HIGH )
			{
				if (nTempccdUV < m_nTempUVMin)
							m_nTempUVMin = nTempccdUV;
				if (nTempccdUV > m_nTempUVLast)
					++m_cUVflag;
				else if (nTempccdUV < m_nTempUVLast)
					m_cUVflag = 0;
				m_nTempUVLast = nTempccdUV;
				if (m_cUVflag > 3)
				{
					if ( !( UV_CCD_FLG & Getm_nFatal( ) ) )
					{
						ErrorMsg( ( int ) YES, ERR_UV_CCD_HEATING );
						theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
						return(ERR_UV_CCD_HEATING);
					}
				}
				if ( (m_nTempUVLast - m_nTempUVMin) > 3)		// ~ 1.8 degrees
				{
					if ( !( UV_CCD_FLG & Getm_nFatal( ) ) )
					{
						ErrorMsg( ( int ) YES, ERR_UV_CCD_HEATING );
						theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
						return(ERR_UV_CCD_HEATING);
					}
				}
				if (nTempccdUV > UV_CCD_TEMP_START)				// -5 degrees
				{
					if ( !( UV_CCD_FLG & Getm_nFatal( ) ) )
					{
						ErrorMsg( ( int ) YES, ERR_UV_CCD_MAX );
						theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
						return(ERR_UV_CCD_MAX);
					}
				}
				else
				{
					if ( !( UV_CCD_FLG & Getm_nWarning() ) )
						ErrorMsg( ( int ) NO, WARN_UV_CCD_HIGH );
				}
			}
			else if ((UV_CCD_FLG & Getm_nWarning()) &&
					(nTempccdUV < UV_CCD_TEMP_HIGH_OK))
				ErrorMsg( ( int ) OK, MSG_UV_CCD_OK );

			if (m_bSingleChannel == false)
			{
				if ( nTempccdVIS < VIS_CCD_TEMP_MIN )
				{
					if ( ! ( VIS_CCD_FLG & Getm_nFatal( ) ) )
					{
						ErrorMsg( ( int ) YES, ERR_VIS_CCD_MIN );
						theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
						return(ERR_VIS_CCD_MIN);
					}
				}
				else if ( nTempccdVIS > VIS_CCD_TEMP_HIGH )
				{
					if (nTempccdVIS < m_nTempVISMin)
									m_nTempVISMin = nTempccdVIS;
					if (nTempccdVIS > m_nTempVISLast)
						++m_cVISflag;
					else if (nTempccdVIS < m_nTempVISLast)
						m_cVISflag = 0;
					m_nTempVISLast = nTempccdVIS;
					if (m_cVISflag > 3)
					{
						if ( !( VIS_CCD_FLG & Getm_nFatal( ) ) )
						{
							ErrorMsg( ( int ) YES, ERR_VIS_CCD_HEATING );
							theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
							return(ERR_VIS_CCD_HEATING);
						}
					}
					if ( (m_nTempVISLast - m_nTempVISMin) > 3)		// ~ 1.8 degrees
					{
						if ( !( VIS_CCD_FLG & Getm_nFatal( ) ) )
						{
							ErrorMsg( ( int ) YES, ERR_VIS_CCD_HEATING );
							theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
							return(ERR_VIS_CCD_HEATING);
						}
					}
					if (nTempccdVIS > VIS_CCD_TEMP_START)	// -5 degrees
					{
						if ( !( VIS_CCD_FLG & Getm_nFatal( ) ) )
						{
							ErrorMsg( ( int ) YES, ERR_VIS_CCD_MAX );
							theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
							return(ERR_VIS_CCD_MAX);
						}
					}
					else
					{
						if ( !( VIS_CCD_FLG & Getm_nWarning() ) )
							ErrorMsg( ( int ) NO, WARN_VIS_CCD_HIGH );
					}
				}
				else if ( ( VIS_CCD_FLG & Getm_nWarning() ) && ( nTempccdVIS < VIS_CCD_TEMP_HIGH_OK ) )
					ErrorMsg( ( int ) OK, MSG_VIS_CCD_OK );
			}
			theApp.LeaveCriticalSection1(&m_CriticalSection);				// Release protection
		}
		while ( ( nTempccdUV  > UV_CCD_TEMP_HIGH ||
				( nTempccdVIS > VIS_CCD_TEMP_HIGH && m_bSingleChannel == true ) ) &&
				( lTimeNow - lONtime ) < MAX_COOLER_TIME );

		if ( nTempccdUV > UV_CCD_TEMP_HIGH  )
		{
			if ( !( UV_CCD_FLG & Getm_nFatal( ) ) )
			{
				ErrorMsg( ( int ) YES, ERR_UV_CCD_HEATING );
				return(ERR_UV_CCD_HEATING);
			}
		}
		else if ( nTempccdVIS > VIS_CCD_TEMP_HIGH && m_bSingleChannel == false )
		{
			if ( !( VIS_CCD_FLG & Getm_nFatal( ) ) )
			{
				ErrorMsg( ( int ) YES, ERR_VIS_CCD_HEATING );
				return(ERR_VIS_CCD_HEATING);
			}
		}
		theApp.EnterCriticalSection1(&m_CriticalSection);				// Protect critical parameters
		if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
		{
			theApp.LeaveCriticalSection1(&m_CriticalSection);			// Release protection
			return m_nFatalCode;
		}
		m_cStartupMode = HOT4;
		//...........................................................
		// Hot4 mode entry placed into the Spectrometer log.
		{
			char* szStr = new char[40];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Mode: Hot4\0");
			AddToSpecLog(szStr);
		}
		//...........................................................
		this->GetDateTimeStamp(strTimeStamp);						// Date: Time string
		printf("%s  cfgHdwr in HOT4\r\n", strTimeStamp);
		//========================================================
		// If VegaSCA mode, (Optima 5200), be sure grating is in Normal position
		//========================================================
		if (m_cChipMode == VEGA_SCA)								// CHECK FOR VegaSCA
		{
			this->GetDateTimeStamp(strTimeStamp);					// Date: Time string
			printf("cfgHdwr (ChipMode == VEGA_SCA) (5200)\r\n");
			m_cGrating = '0';
			m_nSolenoidRegister &= ~GRATING_EN;						// GRATING DISABLED
			m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
		}
		//========================================================
		// Be sure Shutter is closed & Mercury lamp is turned off
		//========================================================
		m_nDigitalControl &= ~HGLAMP_EN;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);

		cmdKC();   //  CBF-31 - Move shutter to closed position. 
		
		theApp.LeaveCriticalSection1(&m_CriticalSection);			// Release protection
		//==================================
		// Give Shutter a chance to respond
		//==================================
		WaitDelay((int) 1000);

		//=====================================
		// Move View Motors to Key Hg position
		//=====================================
		this->GetDateTimeStamp(strTimeStamp);						// Date: Time string
		printf("%s  cfgHdwr cmdHG()... Move to Hg key position\r\n", strTimeStamp);
		if ((status = cmdHG()) != NO_ERRORS) 
		{
			ErrorMsg((int) YES, status);
			return(status);
		}

		//============================================
		// Read Bias & Dark Current and check results
		//============================================
		this->GetDateTimeStamp(strTimeStamp);					// Date: Time string		
		printf("%s  cfgHdwr cmdI2()... Read BIAS & Dark Current ... Check Results\r\n", strTimeStamp);
		if ((status = cmdI2()) != NO_ERRORS)
			ErrorMsg((int) NO, status);
		
		//======================
		// Turn on Mercury lamp
		//======================
		m_nDigitalControl |= HGLAMP_EN;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);

		//==================================
		// Give Hg lamp a chance to respond
		//==================================
		WaitDelay((int) 5000);

		//================================
		// Read Hg arrays & check results
		//================================
		this->GetDateTimeStamp(strTimeStamp);					// Date: Time string
		printf("%s  cfgHdwr cmdI3()... Read Hg arrays & check results\r\n", strTimeStamp);
		if ((status = cmdI3()) != NO_ERRORS)
			ErrorMsg((int) NO, status);

		//==============
		// Open Shutter
		//==============

		cmdPA();   //  CBF-31 - Move shutter and toroid to axial position. 
		
		//==================================
		// Give Shutter a chance to respond
		//==================================
		WaitDelay((int) 5000);

		theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
		if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
		{
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			return m_nFatalCode;
		}
		theApp.LeaveCriticalSection1(&m_CriticalSection);		// Release protection
		//======================================
		// Read Hg arrays again & check results
		//======================================
		this->GetDateTimeStamp(strTimeStamp);			// Date: Time string		
		printf("%s  cfgHdwr cmdI4()... Read Hg arrays again & check results\r\n", strTimeStamp);
		if ((status = cmdI4()) != NO_ERRORS)
			ErrorMsg((int) NO, status);
		
		this->GetDateTimeStamp(strTimeStamp);			// Date: Time string
		printf("%s  cfgHdwr cmdI4()... Post call\r\n", strTimeStamp);
		//=======================================
		// Close Shutter & turn off Mercury lamp
		//=======================================
		m_nDigitalControl &= ~HGLAMP_EN;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
		
		cmdKC();   //  CBF-31 - Move shutter to closed position.
		
		//============================================
		// Give Shutter & Hg lamp a chance to respond
		//============================================
		this->GetDateTimeStamp(strTimeStamp);			// Date: Time string
		printf("%s  cfgHdwr cmdI4()... Post call... WaitDelay((int) 1000)\r\n", strTimeStamp);
		WaitDelay((int) 1000);

		//=====================================================
		// Reconfigure View Motors & move to default positions
		//=====================================================
		
		status = Configure_Motors(FALSE);			// Configure all motors except the slit
		if (status != NO_ERRORS) return (status);	// Exit since we went fatal
		
		m_Init_Motors_Status = YES;   				// Mark as completed 

		//=====================================================
		// Perform Software Reset to obtain default conditions
		//=====================================================
		this->GetDateTimeStamp(strTimeStamp);			// Date: Time string
		printf("%s  cfgHdwr cmdSR()... Software Reset\r\n", strTimeStamp);
		status = cmdSR();
		
		this->GetDateTimeStamp(strTimeStamp);			// Date: Time string
		printf("%s  cfgHdwr going to READY\r\n", strTimeStamp);

		theApp.EnterCriticalSection1(&m_CriticalSection);		// Protect critical parameters
		if (m_cStartupMode == FATAL || m_cInitFlag == FATAL)
		{
			theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
			return m_nFatalCode;
		}
		EnterCriticalSection(&m_CriticalSectionClock);			// Protect critical clock parameters
		m_cStartupMode = m_cOperationMode;
		//...........................................................
		// Ready mode entry placed into the Spectrometer log.
		if ( m_cStartupMode == READY )
		{
			{
				char* szStr = new char[40];
				GetDateTimeStamp(szStr);
				sprintf(&szStr[19], " Mode: Ready\0");
				AddToSpecLog(szStr);
			}
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
		//...........................................................
		m_lKeyTime = 0;
		m_cInitFlag = NO;
		m_bHWInitComplete = true;
		m_argon_flag = 0;
		LeaveCriticalSection(&m_CriticalSectionClock);		// Release clock protection
		theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
		
	}  // End if (cStartupMode == HOT)

	return(status);
}
//-----------------------------------------------------------------------------------
//  GetHeaterModeString		Get the Heater mode as a sz string
//
//	Note: The caller MUST provide a char buffer of at least 12 chars
//
void CParserThread::getHeaterModeString(char* pBuff)
{
	char szBuffer[12];
	switch(m_nHeater_mode)
	{
	case MODE_WARMUP:			strcpy(szBuffer, "Warm-up    ");
		break;
	case MODE_STABILIZATION:	strcpy(szBuffer, "Stabilizing");
		break;
	case MODE_NORMAL:			strcpy(szBuffer, "Normal     ");
		break;
	case MODE_LID_OFF:			strcpy(szBuffer, "Lid-Off    ");
		break;
	default:					strcpy(szBuffer, "Other      ");
		break;
	}

	strcpy(pBuff, szBuffer);
}
//===========================================================================

/*** Revision History ***

1/11/16    JO      Initial additions and changes for Century B.
1/28/16    JO      Added toroid and shutter motor initializations.
7/05/16    JO      CBF-17 - Removed code that refernced SHUTTER_EN bit for now.  Must be revisited with CBF-31.
7/18/16    JO      CBF-31 - Add code to open/close shutter and toroid where required.
09/13/16   JO      CBF-47 - Update thermal startup and operational state machine.
10/20/16   JO      CBF-54 - Change heater related names/definitions, fix broken heater commands.
10/25/16   JO      CBF-67 - Add timestamps to console output.
10/27/16   JO      CBF-65 - Code changes to support Syngistix release 3.0.
11/30/16   KR	   CBF-97 - Add MODE_LID_OFF to getHeaterModeString()
11/30/16   KR	   CBF-99 - Add all the heater sensors to cfghdwr for spectrometer log	
12/01/16   JO      CBF-102 - Last minute updates - thermal setpoints.
12/02/16   JO      CBF-106 - Move thermal setpoint definitions to zodiac.h.
12/08/16   KR	   CBF-112 - Remove High purge condition in COLD startup mode
12/13/16   JO      CBF-113 - Fix bug that computes m_lKeyTime - Add this change to second 
                    calculation on line 400 (approx.). See line 271 as a reference.
01/17/17   JO      CBF-133 - Add cmd_JI functionality.					
 
$Log: /IcarusBased/SpecFW/cfghdwr.cpp $
 * 
 * 34    4/07/08 10:41a Nashth
 * Second attempt at Spectrometer history log.
 * 
 * 33    3/31/08 5:31p Nashth
 * Implementation of spectrometer history log... First pass.
 * 
 * 32    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 31    12/19/06 2:49p Nashth
 * Initialized minimum detector temperature variables to 255, and fixed a
 * number of constraints for determining the minimum value and entering
 * Fatal mode for detector tempareatures as per QPR: CANU6WMPKS.
 * 
 * 30    8/08/06 2:26p Frazzitl
 * Fixed problem with Fatal error occurring during initialization.
 * 
 * 29    7/26/06 3:28p Frazzitl
 * Waiting ten minutes before checking to see if the tub temperature is
 * increasing.
 * 
 * 28    7/25/06 1:42p Frazzitl
 * Moved debug output so bad readings are displayed even if they don't
 * cause an error message.
 * 
 * 27    7/24/06 3:51p Frazzitl
 * Tub temp got changed from counts to actual temps but the ">" sign did
 * not get changed to the "<" sign for determining if the system should do
 * a Cold start.
 * 
 * 26    7/24/06 9:25a Frazzitl
 * Added code so that a single reading will not cause an error message or
 * change in the heater mode to occur.  At least three sequential readings
 * are required.
 * 
 * 25    12/02/05 9:58a Frazzitl
 * Eliminated the possibility of never getting out of Initialization when
 * Tub temp can't be reached.
 * 
 * 24    11/29/05 3:09p Nashth
 * Changed an incorrect call to LeaveCriticalSection for the main critical
 * section to the correct Clock critical section as per QPR: TLFI6HYRAX
 * 
 * 23    11/29/05 11:35a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 22    11/22/05 10:10a Nashth
 * Added some Printf statements for debugging... Some with THN within the
 * Print Text... These can eventually be removed or made debug flag
 * optional.
 * 
 * 21    11/11/05 2:56p Frazzitl
 * Corrected problem with OffTime variable because of savetime() being
 * called before hardware initialization completed.
 * 
 * 20    9/30/05 9:23a Frazzitl
 * Changed variable names to make more readable
 * 
 * 19    9/20/05 4:13p Nashth
 * Added THNJ6DNPHZ to prior comment as it was omitted previously.
 * 
 * 18    8/24/05 8:49a Nashth
 * Most recent updates for hwinit as per THNJ6DNPHZ.
 * 
 * 17    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 16    7/20/05 2:48p Nashth
 * Added a few comments and removed some dead code.
 * 
 * 15    7/18/05 14:41 Frazzitl
 * Implemented changes required to read and check data during hardware
 * initialization
 * 
 * 14    7/05/05 13:14 Frazzitl
 * Fixed problem with turning on power
 * 
 * 13    6/29/05 16:09 Frazzitl
 * More minor changes
 * 
 * 12    6/29/05 16:03 Frazzitl
 * Minor changes
 * 
 * 11    6/17/05 2:52p Frazzitl
 * Corrected the temperature equation constants and the status comparisons
 * 
 * 10    6/17/05 10:04a Frazzitl
 * Thread required the this pointer.
 * 
 * 9     6/07/05 8:25a Frazzitl
 * Separated camera I/O into UV and VIS routines to accommodate individual
 * status checking
 * 
 * 8     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 7     5/25/05 8:44a Frazzitl
 * Initialized registers and enabled "high speed " operation on power up
 * 
 * 6     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 5     4/20/05 17:00 Frazzitl
 * Corrected some variable problems
 * 
 * 4     4/20/05 11:27 Frazzitl
 * Made more easily readable
 * 
 * 3     4/15/05 13:23 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 2     4/12/05 3:33p Frazzitl
 * Fixing problem with Chip Mode
 * 
 * 1     3/17/05 11:11 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
