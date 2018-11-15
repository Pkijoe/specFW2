//===========================================================================
//
//	Module Name:	cfgSys.cpp
//
//	Function:		This routine initializes the firmware.
//					It also clears the Data Storage Area.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005-2006,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"


//-------------------------------------------------------------------------------------
//  cfgSys ... called from CParserThread C'tor 
//             (Critical Sections are not yet initialized... Nothing here can use them)  

short CParserThread::cfgSys()
{
	unsigned int	status(NO_ERRORS);

	m_lWatchdog_enable = NO;

	m_cInitFlag		= YES;		// Must be YES or savetime() will be called which changes the OffTime
								// This will cause a problem when choosing COLD or HOT Startups
//	m_cInitFlag		= NO;
	m_nFatalCode	= NO_ERRORS;
	m_cStartupMode	= NOT_ON;	// NOT_ON set for initial 10 minute loop
	//...........................................................
	// NotOn mode entry placed into the Spectrometer log.
	{
		char* szStr = new char[40];
		GetDateTimeStamp(szStr);
		sprintf(&szStr[19], " Mode: Not On\0");
		AddToSpecLog(szStr);
	}
	//...........................................................
	m_cOperationMode= DIAG;		// DIAG operation mode thru startup
	m_bAbortFlag	= false;
	m_bCCDPower		= false;
	m_nOPmode		= NORMAL;
	m_nTestMode		= NORMAL;
	m_bTestMode		= false;
	m_bOverscanMode	= false;
	m_bCCDPower		= false;

	// Initialize all Register variables
	m_nPowerRegisterUV	= 0;
	m_nPowerRegisterVIS	= 0;
	m_nDigitalControl	= 0;
	m_nSolenoidRegister	= 0;

	m_CameraIO.UVOutputW(ARRAY_ADDRESS_UV,  0x00);
	m_CameraIO.VISOutputW(ARRAY_ADDRESS_VIS, 0x00);

	m_bSleepFlag	= false;			
	m_nLineCount	= 1000;			
	m_nErrorCnt		= 0;			
	m_nWarning		= 0;			
	m_nFatal		= 0;
	m_lHeat_warnings= 0;

	m_nSolenoidRegister	= 0;			
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);

	m_NvRam.OutputB(ARGON_VALVE_OK, ASCII0);
	if ( ! (BOOL)(m_IO.InputW(SVSTAT_CS) & ARGON_FLOW) )
		m_NvRam.OutputB(ARGON_VALVE_OK, (BYTE) (m_NvRam.InputB(ARGON_VALVE_OK) + 1));

	m_nSolenoidRegister  = ( ARGON_EN | ARGON_HI | NITROGEN_HI );
	m_nMotorControl = 0;
	m_bSHmode  = false;
	m_bSEmode  = false;
	m_bLPmode  = false;			
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, m_nMotorControl);
			
	if ( m_IO.InputW(SVSTAT_CS) & ARGON_FLOW )
		m_NvRam.OutputB(ARGON_VALVE_OK, (BYTE) (m_NvRam.InputB(ARGON_VALVE_OK) + 2));

	m_cCurrentPurge= m_NvRam.InputB(LAST_PURGE);
	if (m_cCurrentPurge != LOW && m_cCurrentPurge != HIGH)
	{
		m_cCurrentPurge = HIGH;
		m_NvRam.OutputB(LAST_PURGE, HIGH);
	}

// This should not be called here - only when D0 commands to set defaults
//	setDefaultHeaterParameters(false);
	readBRAM(false);

	m_bCCDPower	= false;

	m_nWaitTime	= MAX_S_TIME;

	m_bLimitXFlag	= false;
	m_bLimitYFlag	= false;
	m_cLookerCnt	= 0;

	m_nStatusNo	= 1;			

	m_cArgonFlowCnt		= 0;		
	m_cNitrogenFlowCnt	= 0;
	m_cWaterLeakCnt		= 0;
	m_cPS24vdcCnt		= 0;
	m_cUVcoolerCnt		= 0;
	m_cVIScoolerCnt		= 0;
	m_cTempccdUVCnt		= 0;
	m_cTempccdVISCnt	= 0;
	m_cTempFanCnt		= 0;
	m_bCheckWaterFlag	= false;	
	m_cWaterDay			= m_NvRam.InputB(CLKDATE);
	m_cFinLowCount		= 0;
	m_cFinHighCount		= 0;
	m_cTubLowCount		= 0;
	m_cTubHighCount		= 0;
	m_cFinOverCount		= 0;
	m_cTubOverCount		= 0;
	m_cTopOverCount		= 0;
	m_cBottomOverCount	= 0;
	m_cWarm2StableCount	= 0;
	m_cNormal2WarmCount	= 0;
	m_cTubWarmupCount	= 0;
	m_cUVCoolerCount	= 0;
	m_cVISCoolerCount	= 0;
	m_nTempUVMin		= 255;
	m_nTempVISMin		= 255;

	m_bHolding			= false;
	
	m_bReached_temp = true;		// Need this if going directly to STABILIZATION mode
	m_nHeater_mode = MODE_STABILIZATION;
	printf("Heat Mode starting in STABILIZATION\r\n");

	m_lDebug_print = 0;

	m_nPlasma_lit = 0;

	status = cmdSR();

	m_bNewDataFormatFlag = false;

	return NO_ERRORS;

}
//===========================================================================
//  Critical Section protection can be turned off... must be when called from C'Tor
//
void CParserThread::setDefaultHeaterParameters(bool bProtect)
{
	if ( bProtect) 
		theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	m_NvRam.OutputW(INTEGRATOR_PRESET, INTEGRATOR_PRESET_DEFAULT);
	m_lLineFrequency = LINE_FREQUENCY_DEFAULT;

	m_NvRam.OutputB(DEBUG_FLAG, 0);
	m_nDebug = 0;
	m_NvRam.OutputB(HARDWARE_VER, 1);
	m_nHardware_version = 1;
	m_NvRam.OutputB(HEATER_CONTROL, '1');

	m_NvRam.OutputL(OPTICS_HOURS, OPTICS_HOURS_DEFAULT);
	m_lOptics_hours = OPTICS_HOURS_DEFAULT;
	if ( bProtect) 
		theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
}
//===========================================================================
//  Critical Section protection can be turned off... must be when called from C'Tor
//
void CParserThread::readBRAM(bool bProtect)
{
	WORD tempw;
	
	if ( bProtect) 
		theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	// Always No Unsolicited messages
	m_bUnsolicitedMsg = false;

	// Sync mode also does not mean anything
	m_bSyncMode	= false;

	m_nBRAMerrors = 0;

//*********************************************************************************	
	// Check to see if we need to do an automatic sensor calibration
	// No cal needed if magic number is present - already done
	
	if (m_NvRam.InputL(CALIBRATE_SENSORS_INIT) != MAGIC_NUMBER)	
	 {	
		m_Calibration_Flag = 1;
		
		// Set the active sensors to primary by default
		m_NvRam.OutputB(SENSOR_UPPER_ACTIVE, PRIMARY);
		m_NvRam.OutputB(SENSOR_LOWER_ACTIVE, PRIMARY);
	 }
	else
		m_Calibration_Flag = 0;
//***********************************************************************************

	m_nMaxSlitStep = m_NvRam.InputW(MAX_SLIT);
	if ( m_nMaxSlitStep < 3120 || m_nMaxSlitStep > 6000 )
	{
		m_NvRam.OutputW(MAX_SLIT, 0x1400);
		m_nMaxSlitStep = 5120;
		m_nBRAMerrors += 1;
	}
	
	m_nMaxXSteps_Axial = m_NvRam.InputW(MAX_VIEWX_AXIAL);
	if ( m_nMaxXSteps_Axial < 100 || m_nMaxXSteps_Axial > 1000 )
	{
		m_NvRam.OutputW(MAX_VIEWX_AXIAL, 0x0100);
		m_nMaxXSteps_Axial = 256;
		m_nBRAMerrors += 1;
	}
	
	m_nMaxXSteps_Radial = m_NvRam.InputW(MAX_VIEWX_RADIAL);
	if ( m_nMaxXSteps_Radial < 100 || m_nMaxXSteps_Radial > 1000 )
	{
		m_NvRam.OutputW(MAX_VIEWX_RADIAL, 0x0100);
		m_nMaxXSteps_Radial = 256;
		m_nBRAMerrors += 1;
	}
		
	m_nMaxYSteps_Axial = m_NvRam.InputW(MAX_VIEWY_AXIAL);
	if ( m_nMaxYSteps_Axial < 100 || m_nMaxYSteps_Axial > 1000 )
	{
		m_NvRam.OutputW(MAX_VIEWY_AXIAL, 0x0100);
		m_nMaxYSteps_Axial = 256;
		m_nBRAMerrors += 1;
	}
	
	m_nMaxYSteps_Radial = m_NvRam.InputW(MAX_VIEWY_RADIAL);
	if ( m_nMaxYSteps_Radial < 100 || m_nMaxYSteps_Radial > 1000 )
	{
		m_NvRam.OutputW(MAX_VIEWY_RADIAL, 0x0100);
		m_nMaxYSteps_Radial = 256;
		m_nBRAMerrors += 1;
	}

	m_nSlitLow = m_NvRam.InputW(HOME_2_SLIT);
	if ( m_nSlitLow < 1500 || m_nSlitLow > 1900 )
	{
		m_NvRam.OutputW(HOME_2_SLIT, 0x0602);
		m_nSlitLow = 1536;
		m_nBRAMerrors += 1;
	}
	m_nSlitHigh  = (WORD) (m_nSlitLow - m_nSlitDelta);
	m_nSlitBarn  = (WORD) (m_nSlitLow + m_nSlitDelta + m_nSlitBdStp);
	
	m_nViewXPeak_Axial = m_NvRam.InputW(HOME_2_PEAK_AXIAL);
	if ( m_nViewXPeak_Axial < 10 || m_nViewXPeak_Axial > ( m_nMaxXSteps_Axial - 10 ) )
	{
		m_NvRam.OutputW(HOME_2_PEAK_AXIAL, 0x0080);
		m_nViewXPeak_Axial = 128;
		m_nBRAMerrors += 1;
	}
	
	m_nViewXPeak_Radial = m_NvRam.InputW(HOME_2_PEAK_RADIAL);
	if ( m_nViewXPeak_Radial < 10 || m_nViewXPeak_Radial > ( m_nMaxXSteps_Radial - 10 ) )
	{
		m_NvRam.OutputW(HOME_2_PEAK_RADIAL, 0x0080);
		m_nViewXPeak_Radial = 128;
		m_nBRAMerrors += 1;
	}
	
	m_nViewY15mm_Axial = m_NvRam.InputW(HOME_2_15MM_AXIAL);
	if ( m_nViewY15mm_Axial < 10 || m_nViewY15mm_Axial > ( m_nMaxYSteps_Axial - 10 ) )
	{
		m_NvRam.OutputW(HOME_2_15MM_AXIAL, 0x0080);
		m_nViewY15mm_Axial = 128;
		m_nBRAMerrors += 1;
	}
	
	m_nViewY15mm_Radial = m_NvRam.InputW(HOME_2_15MM_RADIAL);
	if ( m_nViewY15mm_Radial < 10 || m_nViewY15mm_Radial > ( m_nMaxYSteps_Radial - 10 ) )
	{
		m_NvRam.OutputW(HOME_2_15MM_RADIAL, 0x0080);
		m_nViewY15mm_Radial = 128;
		m_nBRAMerrors += 1;
	}

	if ((m_NvRam.InputB(VIEW_TYPE) == AXIAL_TYPE ) || (m_NvRam.InputB(VIEW_TYPE) == RADIAL_TYPE))
		m_cViewType = m_NvRam.InputB(VIEW_TYPE);
	else
	{
		m_NvRam.OutputB(VIEW_TYPE, AXIAL_TYPE);
		m_cViewType = AXIAL_TYPE;
	}
	
	m_Shutter_Pos_Max = m_NvRam.InputW(SHUTTER_POSITION_MAX);
	if (m_Shutter_Pos_Max < 1 || m_Shutter_Pos_Max > SHUTTER_MAX_STEPS)
	{
		m_NvRam.OutputW(SHUTTER_POSITION_MAX, SHUTTER_DEFAULT_POS_MAX);
		m_Shutter_Pos_Max = SHUTTER_DEFAULT_POS_MAX;
		m_nBRAMerrors += 1;
	}

	tempw = m_NvRam.InputW(SHUTTER_POS_RADIAL);
	if (tempw < 1 || tempw > m_Shutter_Pos_Max)
	{
		m_NvRam.OutputW(SHUTTER_POS_RADIAL, SHUTTER_DEFAULT_RADIAL_POS);
		m_nBRAMerrors += 1;
	}
	
	tempw = m_NvRam.InputW(SHUTTER_POS_CLOSED);
	if (tempw < 1 || tempw > m_Shutter_Pos_Max)
	{
		m_NvRam.OutputW(SHUTTER_POS_CLOSED, m_Shutter_Pos_Max);
		m_nBRAMerrors += 1;
	}
	
	if ( m_NvRam.InputB(SINGLE_CHANNEL_1) == 'S' && m_NvRam.InputB(SINGLE_CHANNEL_2) == 'C' )
		m_bSingleChannel = true;
	else if ( m_NvRam.InputB(SINGLE_CHANNEL_1) == 'D' && m_NvRam.InputB(SINGLE_CHANNEL_2) == 'C' )
		m_bSingleChannel = false;
	else
		m_bSingleChannel = true;

	if (m_NvRam.InputB(CHIP_TYPE_CODE_1) == 'C' && m_NvRam.InputB(CHIP_TYPE_CODE_2) == 'T')
	{
		m_cDetectorType	= m_NvRam.InputB(CHIP_TYPE);
		m_cChipMode	= m_NvRam.InputB(CHIP_MODE);
		if (m_cDetectorType != ZODIAC && m_cDetectorType != VEGA)
			m_cDetectorType	= VEGA;
		if (m_cChipMode != ZODIAC && m_cChipMode != VEGA && m_cChipMode != VEGA_SCA)
			m_cChipMode	= VEGA;
		else if (m_cDetectorType == ZODIAC && m_cChipMode != ZODIAC)
			m_cChipMode	= ZODIAC;
	}
	else
	{
		m_NvRam.OutputB(CHIP_TYPE_CODE_1, 'C');
		m_NvRam.OutputB(CHIP_TYPE_CODE_2, 'T');
		m_NvRam.OutputB(CHIP_TYPE, VEGA);
		m_cDetectorType	= VEGA;
		m_NvRam.OutputB(CHIP_MODE, VEGA);
		m_cChipMode	= VEGA;
	}

	if (m_cChipMode == VEGA_SCA)		// VegaSCA mode only
	{
		if (m_NvRam.InputB(GRATING) == '1' || m_NvRam.InputB(GRATING) == '0')
		{
			m_cGrating = m_NvRam.InputB(GRATING);
		}
		else
		{
			m_NvRam.OutputB(GRATING, '0');
			m_cGrating = '0';
		}
	}
	else					// Always Normal
	{
		m_NvRam.OutputB(GRATING, '0');
		m_cGrating = '0';
	}
	
	m_nViewXHg_Axial = m_NvRam.InputW(HOME_2_HG_X_AXIAL);
	if (m_nViewXHg_Axial > m_nMaxXSteps_Axial)
	{
		m_NvRam.OutputW(HOME_2_HG_X_AXIAL, 0);
		m_nViewXHg_Axial = 0;
		m_nBRAMerrors += 1;
	}
	
	m_nViewYHg_Axial = m_NvRam.InputW(HOME_2_HG_Y_AXIAL);
	if (m_nViewYHg_Axial > m_nMaxYSteps_Axial)
	{
		m_NvRam.OutputW(HOME_2_HG_Y_AXIAL, 0);
		m_nViewYHg_Axial = 0;
		m_nBRAMerrors += 1;
	}
	
	m_nSlitSno = m_NvRam.InputW(CURR_SLIT);
	if (m_nSlitSno > m_nMaxSlitStep)
	{
		m_NvRam.OutputW(CURR_SLIT, 0);
		m_nSlitSno = 0;
		m_nBRAMerrors += 1;
	}
	
	m_nViewXno_Axial = m_NvRam.InputW(CURR_VIEWX_AXIAL);
	if (m_nViewXno_Axial > m_nMaxXSteps_Axial)
	{
		m_NvRam.OutputW(CURR_VIEWX_AXIAL, 0);
		m_nViewXno_Axial = 0;
		m_nBRAMerrors += 1;
	}
	
	m_nViewXno_Radial = m_NvRam.InputW(CURR_VIEWX_RADIAL);
	if (m_nViewXno_Radial > m_nMaxXSteps_Radial)
	{
		m_NvRam.OutputW(CURR_VIEWX_RADIAL, 0);
		m_nViewXno_Radial = 0;
		m_nBRAMerrors += 1;
	}
	
	m_nViewYno_Axial = m_NvRam.InputW(CURR_VIEWY_AXIAL);
	if (m_nViewYno_Axial > m_nMaxYSteps_Axial)
	{
		m_NvRam.OutputW(CURR_VIEWY_AXIAL, 0);
		m_nViewYno_Axial = 0;
		m_nBRAMerrors += 1;
	}
	
	m_nViewYno_Radial = m_NvRam.InputW(CURR_VIEWY_RADIAL);
	if (m_nViewYno_Radial > m_nMaxYSteps_Radial)
	{
		m_NvRam.OutputW(CURR_VIEWY_RADIAL, 0);
		m_nViewYno_Radial = 0;
		m_nBRAMerrors += 1;
	}
	
	m_cSlitTno	= m_NvRam.InputB(CURR_SLIT_TYPE);
	m_cSlitPno	= m_NvRam.InputB(CURR_SLIT_POS);
	m_cViewXPno_Axial	= m_NvRam.InputB(CURR_VIEWX_POS_AXIAL);
	m_cViewYPno_Axial	= m_NvRam.InputB(CURR_VIEWY_POS_AXIAL);
	m_cViewXPno_Radial	= m_NvRam.InputB(CURR_VIEWX_POS_RADIAL);
	m_cViewYPno_Radial	= m_NvRam.InputB(CURR_VIEWY_POS_RADIAL);

	m_cSlitType	= m_cSlitTno;
	if ( m_cSlitType < HOME || m_cSlitType > BARNDOOR )
	{
		m_NvRam.OutputB(CURR_SLIT_TYPE, HOME);
		m_cSlitType = HOME;
		m_cSlitTno  = HOME;
		m_nBRAMerrors += 1;
	}

	m_cSlitPos = m_cSlitPno;
	if (m_cSlitPos  > 9  )
	{
		m_NvRam.OutputB(CURR_SLIT_POS, 0);
	 	m_cSlitPos = 0;
		m_cSlitPno = 0;	
		m_nBRAMerrors += 1;
	}

	m_cViewXPos_Axial = m_cViewXPno_Axial;
	if (m_cViewXPos_Axial > 60 )
	{
		m_NvRam.OutputB(CURR_VIEWX_POS_AXIAL, 0);
		m_cViewXPos_Axial = 0;
		m_cViewXPno_Axial = 0;
		m_nBRAMerrors += 1;
	}

	m_cViewXPos_Radial = m_cViewXPno_Radial;
	if (m_cViewXPos_Radial > 60 )
	{
		m_NvRam.OutputB(CURR_VIEWX_POS_RADIAL, 0);
		m_cViewXPos_Radial = 0;
		m_cViewXPno_Radial = 0;
		m_nBRAMerrors += 1;
	}	

	m_cViewYPos_Axial = m_cViewYPno_Axial;
	if (m_cViewYPos_Axial > 60 )
	{
		m_NvRam.OutputB(CURR_VIEWY_POS_AXIAL, 0);
		m_cViewYPos_Axial		= 0;
		m_cViewYPno_Axial		= 0;
		m_nBRAMerrors += 1;
	}
	
	m_cViewYPos_Radial = m_cViewYPno_Radial;
	if (m_cViewYPos_Radial > 60 )
	{
		m_NvRam.OutputB(CURR_VIEWY_POS_RADIAL, 0);
		m_cViewYPos_Radial		= 0;
		m_cViewYPno_Radial		= 0;
		m_nBRAMerrors += 1;
	}
	
	// Tub Temperature Control Variables
	long m_lIntegratorPreset = m_NvRam.InputW(INTEGRATOR_PRESET);
	if ((m_lIntegratorPreset < INTEGRATOR_PRESET_MIN) ||
		 (m_lIntegratorPreset > INTEGRATOR_PRESET_MAX))
	 {
		 m_NvRam.OutputW(INTEGRATOR_PRESET, INTEGRATOR_PRESET_DEFAULT);
		 m_lIntegratorPreset = INTEGRATOR_PRESET_DEFAULT;
		 m_nBRAMerrors++;
	 }
 
	 char lid_selection = m_NvRam.InputB(LID_POWER_SELECT);
	 
	 if ( lid_selection != '0' && lid_selection != '1')
	 {
		m_NvRam.OutputB(LID_POWER_SELECT, '0');   // Default it to '0'
	 } 
	 
	m_lLineFrequency = m_NvRam.InputW(LINE_FREQUENCY);
	if ((m_lLineFrequency != 50) && (m_lLineFrequency != 60))
	{
		m_NvRam.OutputW(LINE_FREQUENCY, LINE_FREQUENCY_DEFAULT);
		m_lLineFrequency = LINE_FREQUENCY_DEFAULT;
		m_nBRAMerrors++;
	}

	m_nDebug = m_NvRam.InputW(DEBUG_FLAG);
	theApp.SetDebugFlag(m_nDebug);  // Set the debug word in the main App

	m_nHardware_version = m_NvRam.InputW(HARDWARE_VER);
	if (m_nHardware_version > 1)
		{
		m_NvRam.OutputW(HARDWARE_VER, 1);
		m_nHardware_version = 1;
		m_nBRAMerrors++;
		}

	if ((m_NvRam.InputB(HEATER_CONTROL) != '0') && (m_NvRam.InputB(HEATER_CONTROL) != '1'))
		{
		m_NvRam.OutputB(HEATER_CONTROL, '1');
		m_nBRAMerrors++;
		}

	m_lOptics_hours = m_NvRam.InputL(OPTICS_HOURS);
	if ((m_lOptics_hours < 0) || ((m_lOptics_hours * 60) > OPTICS_HOURS_MAX))
	{
		m_NvRam.OutputL(OPTICS_HOURS, OPTICS_HOURS_DEFAULT);
		m_lOptics_hours = OPTICS_HOURS_DEFAULT;
		m_nBRAMerrors++;
	}

	WORD wTemp  = m_NvRam.InputW(AUTO_RESET_TIME);
	if ( wTemp > 999 )	// Out of range in BRAM ?
	{
		wTemp = 240;	// Yes... Set to 24 hours in BRAM and object
		m_NvRam.OutputW(AUTO_RESET_TIME, wTemp); // Save value in BRAM
	}

	m_bTurnCameraBackOnUV	= false;
	m_bTurnCameraBackOnVIS	= false;

	if ( bProtect) 
		theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
}
//===========================================================================

/*** Revision History ***

12/15/15    JO    Initial additions and changes for Century B.
07/28/16    JO    CBF-26  Removed unused toroid commands and references.
12/13/16	KR	  CBF-115 Add INTEGRATOR_PRESET address and variables
12/14/16    JO    CBF-117 Add lid power set to 100% or 30% during warm-up selected by F1 command.
				   Fixed errors from CBF-115 - m_lIntegratorPreset declared in wrong section; Typos.	
01/09/17    JO    CBF-125 - Add temperature sensor fault detection.
01/24/17	KR	  CBF-137 - Remove heater configured stuff (m_bSensor_working) & Old Temp sensors
01/28/17	KR	  CBF-143 - Remove thermal commands
02/02/17    JO    CBF-147 - Add get/set BRAM commands for maximum shutter position.


$Log: /IcarusBased/SpecFW/cfgsys.cpp $
 * 
 * 43    4/07/08 10:41a Nashth
 * Second attempt at Spectrometer history log.
 * 
 * 42    3/31/08 5:31p Nashth
 * Implementation of spectrometer history log... First pass.
 * 
 * 41    3/07/07 4:17p Nashth
 * Attempt of improving the fix to QPR: THNJ6XXKR5...
 * Should the camera turn itself off at ANY time, then the
 * firmware will turn it back on.
 * 
 * 40    1/31/07 4:26a Nashth
 * Removed two compiler warnings.
 * 
 * 39    1/30/07 4:08p Nashth
 * First pass at keeping the camera on during a 'long' connection as per
 * QPR: THNJ6XXLK3.
 * 
 * 38    1/23/07 10:09a Nashth
 * Added two functions AR<000> sets Auto Reset Time and
 * RA that retreives Auto Reset Time. Units = tenths of hours.
 * 
 * 37    12/19/06 2:49p Nashth
 * Initialized minimum detector temperature variables to 255, and fixed a
 * number of constraints for determining the minimum value and entering
 * Fatal mode for detector tempareatures as per QPR: CANU6WMPKS.
 * 
 * 36    7/24/06 9:25a Frazzitl
 * Added code so that a single reading will not cause an error message or
 * change in the heater mode to occur.  At least three sequential readings
 * are required.
 * 
 * 35    12/21/05 10:23a Frazzitl
 * Fixed problem with Tub temperature & read reset
 * 
 * 34    11/29/05 4:47p Nashth
 * Set a debug flag for DEBUG_CS (Critical Sections).
 * 
 * 33    11/29/05 11:35a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 32    11/21/05 9:11a Nashth
 * Set initial state of TUB heating control to WARM-UP, as is done with
 * the Optima 4000 firmware.
 * 
 * 31    11/11/05 4:11p Frazzitl
 * Removed call to load defaults each time firmware starts.  Should only
 * be called by D0 command.
 * 
 * 30    11/11/05 2:56p Frazzitl
 * Corrected problem with OffTime variable because of savetime() being
 * called before hardware initialization completed.
 * 
 * 29    11/10/05 2:03p Frazzitl
 * Added changes to output the Heater Mode to the HyperTerminal
 * 
 * 28    10/27/05 7:34a Frazzitl
 * Fixed QPR THNJ6HJSN3
 * 
 * 27    10/06/05 1:32p Frazzitl
 * Fixed problem in QPR THNJ6GWPCL.  The problem was the incorrect reading
 * of the Water Leak status because the incorrect I/O function was being
 * used.
 * 
 * 26    10/03/05 1:42p Frazzitl
 * Fixed problem with heater plates turning off and not coming on again.
 * 
 * 25    9/30/05 9:23a Frazzitl
 * Changed variable names to make more readable
 * 
 * 24    9/26/05 11:24a Nashth
 * Removed unneeded variable
 * 
 * 23    9/14/05 8:01a Frazzitl
 * Removed commented areas from original code.
 * 
 * 22    8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 21    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 20    7/13/05 11:28 Frazzitl
 * Added function to set default heater parameters - now implemented on
 * startup since it seems some are being lost.
 * 
 * 19    7/05/05 10:33 Frazzitl
 * Not ready for Hardware Initialization yet
 * 
 * 18    7/01/05 15:28 Frazzitl
 * Fixed the Tub heating problem
 * 
 * 17    6/29/05 16:11 Frazzitl
 * More minor changes
 * 
 * 16    5/26/05 9:39 Frazzitl
 * Temporarily place instrument in Ready mode
 * 
 * 15    5/25/05 8:44a Frazzitl
 * Initialized registers and enabled "high speed " operation on power up
 * 
 * 14    5/20/05 14:53 Frazzitl
 * Sped up motors
 * 
 * 13    4/28/05 14:16 Frazzitl
 * Getting basic firmware to run
 * 
 * 12    4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 11    4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature
 * 
 * 10    4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 9     4/19/05 3:23p Frazzitl
 * Fixed problems with Tub constants and UV & VIS resolution 
 * 
 * 8     4/15/05 13:23 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 7     4/12/05 3:33p Frazzitl
 * Fixing problem with Chip Mode
 * 
 * 6     4/08/05 12:45 Frazzitl
 * Starting to read Battery RAM parameters on startup
 * 
 * 5     4/08/05 10:18 Frazzitl
 * Removed old Y2K fix.
 * 
 * 4     4/06/05 2:31p Frazzitl
 * Better motor movement
 * 
 * 3     4/04/05 10:44 Frazzitl
 * Fixing problem with motor timing - only temporary
 * 
 * 2     3/30/05 10:48 Frazzitl
 * Fixed problem with pointer
 * 
 * 1     3/17/05 11:11 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
