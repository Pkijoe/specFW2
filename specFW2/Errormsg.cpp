//===========================================================================
//
//	Module Name:	ErrorMsg.cpp
//
//	Function:		This routine outputs unsolicited ERROR messages.
//					This routine also shuts the instrument off in
//					an orderly way, if required by the ERROR.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005-2007,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

void CParserThread::ErrorMsg(unsigned int fatal, unsigned int message)
{
	int messageSave = message;
	PrintErrorMsg(fatal, message);  // Debug display 

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	unsigned long	 location;

	// Messages not allowed through in MFG mode ... Except for camera off warning messages
	if ( (m_cOperationMode == AUTO || m_cOperationMode == MFG) &&
	 	 (message != ERR_CAMERA_OFF_UV) &&       
		 (message != ERR_CAMERA_OFF_VIS)
	   )
	{
		theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
		printf("Operation mode == AUTO or MFG ... Ignore error\r\n"); 
		return;			// NO UNSOLICITED MESSAGES IN AUTO MODE
	}

	if (m_cInitFlag == FATAL)
	{
		theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
		printf("InitFlag == FATAL ... Ignore error\r\n"); 
		return;			// Already in Fatal mode
	}

	switch (message)
	{
		case ERR_ABORT_CMD:
		case ERR_CMD:
		case ERR_FORMAT:
		case ERR_END:
		case ERR_DATA:
		case ERR_BLK:
		case ERR_ARRAY:
		case ERR_PARA:

		case ERR_PIXEL_READ:

		case ERR_FITNESS:
		case ERR_SLIT:
		case ERR_SLIT_LOOKERS:
		case ERR_SLIT_HOME:
		case ERR_SLIT_LIMIT:
		case ERR_SLIT_STEPS:
		
		case ERROR_TOROID_MOVE_TO_HOME:				 		 
		case ERROR_TOROID_MOVE_OFF_HOME:			 
		case ERROR_SHUTTER_MOTOR_MOVE_OFF_SENSOR:		 
		case ERROR_SHUTTER_MOTOR_MOVE_TO_SENSOR:		    
		case ERROR_SHUTTER_MOTOR_MOVE_TO_HOME:		       
		case ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS:

//------------------------------------------------		
		case ERR_VIEWX_AXIAL:
		case ERR_VIEWX_LOOKERS_AXIAL:		
		case ERR_VIEWX_HOME_AXIAL: 		
		case ERR_VIEWX_LIMIT_AXIAL:		
		case ERR_VIEWX_STEPS_AXIAL:
		
		case ERR_VIEWY_AXIAL:
		case ERR_VIEWY_LOOKERS_AXIAL:		
		case ERR_VIEWY_HOME_AXIAL:	 	
		case ERR_VIEWY_LIMIT_AXIAL:		
		case ERR_VIEWY_STEPS_AXIAL:		

		case ERR_VIEWX_RADIAL:
		case ERR_VIEWX_LOOKERS_RADIAL:
		case ERR_VIEWX_HOME_RADIAL: 		
		case ERR_VIEWX_LIMIT_RADIAL:		
 		case ERR_VIEWX_STEPS_RADIAL:
		
		case ERR_VIEWY_RADIAL: 
		case ERR_VIEWY_LOOKERS_RADIAL:		
		case ERR_VIEWY_HOME_RADIAL:	 	
		case ERR_VIEWY_LIMIT_RADIAL:		
		case ERR_VIEWY_STEPS_RADIAL:		
//------------------------------------------------
		case ERR_LOOKER:
		case HIGH_UV_DARK_CURRENT:
		case HIGH_VIS_DARK_CURRENT:
		case HIGH_DARK_CURRENT:
		case NO_UV_MERCURY_LAMP:
		case NO_VIS_MERCURY_LAMP:
		case NO_MERCURY_LAMP:
		case UV_SHUTTER_FAILURE:
		case VIS_SHUTTER_FAILURE:
		case SHUTTER_FAILURE:
		case CCD_POWER_OFF:
		case CCD_POWER_ON:
		case TOP_PRIMARY_SENSOR_FAILED:
		case BOTTOM_PRIMARY_SENSOR_FAILED:
		case TOP_SECONDARY_SENSOR_FAILED:
		case BOTTOM_SECONDARY_SENSOR_FAILED:

		case ERR_CAMERA_OFF_UV:			// Warning Msg
		case ERR_CAMERA_OFF_VIS:		// Warning Msg
			break;
		case ERR_ARGON_FLOW:
			if (!(ARGON_FLG & m_nFatal))
				m_nFatal |= ARGON_FLG;
			break;
		case MSG_ARGON_FLOW:
			m_nFatal &= ~ARGON_FLG;
			break;
		case WARN_NITROGEN_FLOW:
			if (!(NITROGEN_FLG & m_nWarning))
				m_nWarning |= NITROGEN_FLG;
			break;
		case MSG_NITROGEN_FLOW:
			m_nWarning &= ~NITROGEN_FLG;
			break;
		case ERR_WATER_LEAK:
			if (!(WATER_LEAK_FLG & m_nFatal))
				m_nFatal |= WATER_LEAK_FLG;
			break;
		case MSG_WATER_LEAK:
			m_nFatal &= ~WATER_LEAK_FLG;
			break;
		case ERR_24VDC_MIN:
		case ERR_24VDC_MAX:
			if (!(PS24VDC_FLG & m_nFatal))
				m_nFatal |= PS24VDC_FLG;
			break;
		case WARN_24VDC_LOW:
		case WARN_24VDC_HIGH:
			if (PS24VDC_FLG & m_nFatal)
				m_nFatal   &= ~PS24VDC_FLG;
			if (!(PS24VDC_FLG & m_nWarning))
				m_nWarning |= PS24VDC_FLG;
			break;
		case MSG_24VDC_OK:
			m_nWarning &= ~PS24VDC_FLG;
			m_nFatal   &= ~PS24VDC_FLG;
			break;
		case ERR_UV_COOLER_MIN:
		case ERR_UV_COOLER_MAX:
		case ERR_UV_COOLER_READOUT:
			if (!(UV_COOLER_FLG & m_nFatal))
				m_nFatal |= UV_COOLER_FLG;
			break;
		case WARN_UV_COOLER_LOW:
			if (UV_COOLER_FLG & m_nFatal)
				m_nFatal   &= ~UV_COOLER_FLG;
			if (!(UV_COOLER_FLG & m_nWarning))
				m_nWarning |= UV_COOLER_FLG;
			break;
		case MSG_UV_COOLER_OK:
			m_nWarning &= ~UV_COOLER_FLG;
			m_nFatal   &= ~UV_COOLER_FLG;
			break;
		case ERR_VIS_COOLER_MIN:
		case ERR_VIS_COOLER_MAX:
		case ERR_VIS_COOLER_READOUT:
			if (!(VIS_COOLER_FLG & m_nFatal))
				m_nFatal |= VIS_COOLER_FLG;
			break;
		case WARN_VIS_COOLER_LOW:
			if (VIS_COOLER_FLG & m_nFatal)
				m_nFatal   &= ~VIS_COOLER_FLG;
			if (!(VIS_COOLER_FLG & m_nWarning))
				m_nWarning |= VIS_COOLER_FLG;
			break;
		case MSG_VIS_COOLER_OK:
			m_nWarning &= ~VIS_COOLER_FLG;
			m_nFatal   &= ~VIS_COOLER_FLG;
			break;
		case ERR_UV_CCD_MAX:
		case ERR_UV_CCD_MIN:
		case ERR_UV_CCD_HEATING:
			if (!(UV_CCD_FLG & m_nFatal))
				m_nFatal |= UV_CCD_FLG;
			break;
		case WARN_UV_CCD_HIGH:
			if (UV_CCD_FLG & m_nFatal)
				m_nFatal   &= ~UV_CCD_FLG;
			if (!(UV_CCD_FLG & m_nWarning))
				m_nWarning |= UV_CCD_FLG;
			break;
		case MSG_UV_CCD_OK:
			m_nWarning &= ~UV_CCD_FLG;
			m_nFatal   &= ~UV_CCD_FLG;
			break;
		case ERR_VIS_CCD_MAX:
		case ERR_VIS_CCD_MIN:
		case ERR_VIS_CCD_HEATING:
			if (!(VIS_CCD_FLG & m_nFatal))
				m_nFatal |= VIS_CCD_FLG;
			break;
		case WARN_VIS_CCD_HIGH:
			if (VIS_CCD_FLG & m_nFatal)
				m_nFatal   &= ~VIS_CCD_FLG;
			if (!(VIS_CCD_FLG & m_nWarning))
				m_nWarning |= VIS_CCD_FLG;
			break;
		case MSG_VIS_CCD_OK:
			m_nWarning &= ~VIS_CCD_FLG;
			m_nFatal   &= ~VIS_CCD_FLG;
			break;
		case WARN_FAN_TEMP_LOW:
		case WARN_FAN_TEMP_HIGH:
			if (!(FAN_FLG & m_nWarning))
				m_nWarning |= FAN_FLG;
			break;
		case MSG_FAN_TEMP_OK:
			m_nWarning &= ~FAN_FLG;
			break;
		case ERR_CASTING_TEMP_FAILED:
			if (!(CASTING_FLG & m_nFatal))
				m_nFatal |= CASTING_FLG;
			break;
		default:
		{
			break;  // Msg displayed in PrintErrorMsg
		}
	}

	if ( m_nErrorCnt < MAX_ERROR) // Increment to MAX_ERROR; not beyond
		++m_nErrorCnt;

	if (fatal == YES && m_cOperationMode != DIAG)
	{
		ShutDown(message);
		theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
		return;
	}

	//.................................................................
	if ( fatal == YES )
		printf("Fatal mode Not taken... Error logged\r\n"); 

	location = 4 * m_nErrorCnt;
	if (m_nErrorCnt == MAX_ERROR)
			message = ERROR_BUFFER_FULL;   // Mark error buffer full

		if (fatal == YES)
		{
			m_nErrorOut[location    ] = FATAL_ERROR;
			m_nErrorOut[location + 1] = FATAL_ERROR;
		}
		else if (fatal == NO)
		{
			m_nErrorOut[location    ] = WARNING_MSG;
			m_nErrorOut[location + 1] = WARNING_MSG;
		}
		else if (fatal == OK)
		{
			m_nErrorOut[location    ] = OK_MESSAGE;
			m_nErrorOut[location + 1] = OK_MESSAGE;
		}
		else if (fatal == ABORT)
		{
			m_nErrorOut[location    ] = CMD_XX;	// Use as NO COMMAND pending
			m_nErrorOut[location + 1] = CMD_XX;	// Use as NO COMMAND pending
		}
		else
		{
			m_nErrorOut[location    ] = OK_MESSAGE;
			m_nErrorOut[location + 1] = OK_MESSAGE;
		}

		char	cErr2 = (char) (message & 0xff);
		message &= 0xff00;
		message >>= 8;
		char	cErr1 = (char) message;
		m_nErrorOut[location + 2] = cErr1;
		m_nErrorOut[location + 3] = cErr2;
		//...........................................................
		// Warnings (Heater Sensor) entry placed into the Spectrometer log.
		{
			switch (messageSave)
			{
			case TOP_PRIMARY_SENSOR_FAILED:
			case BOTTOM_PRIMARY_SENSOR_FAILED:
			case TOP_SECONDARY_SENSOR_FAILED:
			case BOTTOM_SECONDARY_SENSOR_FAILED:
				char	cErr2 = (char) (messageSave & 0xff);
				messageSave &= 0xff00;
				messageSave >>= 8;
				char	cErr1 = (char) messageSave;
				char* szStr = new char[40];
				GetDateTimeStamp(szStr);
				sprintf(&szStr[19], " Warning: %c%c\0", cErr1, cErr2);
				AddToSpecLog(szStr);
				break;
			}
		}
		//...........................................................

	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
}
//===========================================================================
void CParserThread::PrintErrorMsg(unsigned int fatal, unsigned int message)
{
	CString cstrMessage;		// Message text
	bool	bFatal= false;		// Fatal msg indication
	bool	bWarn = false;		// Warning msg indication
	CString cstrFatalWarn;		// Fatal/Warning msg
	CString cstrFatalMsg;
	CString cstrWarnMsg;
	CString cstrFatalArg;		// Passed parameter: Fatal/Warning/OK/Abort/Other
	CString cstrFatal;

	switch (message)
	{
		case ERR_ABORT_CMD:
			cstrMessage = L"ERR_ABORT_CMD";
			break;
		case ERR_CMD:
			cstrMessage = L"ERR_CMD";
			break;
		case ERR_FORMAT:
			cstrMessage = L"ERR_FORMAT";
			break;
		case ERR_END:
			cstrMessage = L"ERR_END";
			break;
		case ERR_DATA:
			cstrMessage = L"ERR_DATA";
			break;
		case ERR_BLK:
			cstrMessage = L"ERR_BLK";
			break;
		case ERR_ARRAY:
			cstrMessage = L"ERR_ARRAY";
			break;
		case ERR_PARA:
			cstrMessage = L"ERR_PARA";
			break;
		case ERR_PIXEL_READ:
			cstrMessage = L"ERR_PIXEL_READ";
			break;
		case ERR_FITNESS:
			cstrMessage = L"ERR_FITNESS";
			break;
		case ERR_SLIT:
			cstrMessage = L"ERR_SLIT";
			break;
		case ERR_SLIT_LOOKERS:
			cstrMessage = L"ERR_SLIT_LOOKERS";
			break;
		case ERR_SLIT_HOME:
			cstrMessage = L"ERR_SLIT_HOME";
			break;
		case ERR_SLIT_LIMIT:
			cstrMessage = L"ERR_SLIT_LIMIT";
			break;
		case ERR_SLIT_STEPS:
			cstrMessage = L"ERR_SLIT_STEPS";
			break;

//---------------------------------------------------------------			
		case ERR_VIEWX_AXIAL:
			cstrMessage = L"ERR_VIEWX_AXIAL";
			break;
		case ERR_VIEWX_LOOKERS_AXIAL:
			cstrMessage = L"ERR_VIEWX_LOOKERS_AXIAL";
			break;
		case ERR_VIEWX_HOME_AXIAL:
			cstrMessage = L"ERR_VIEWX_HOME_AXIAL";
			break;
		case ERR_VIEWX_LIMIT_AXIAL:
			cstrMessage = L"ERR_VIEWX_LIMIT_AXIAL";
			break;
		case ERR_VIEWX_STEPS_AXIAL:
			cstrMessage = L"ERR_VIEWX_STEPS_AXIAL";
			break;
		case ERR_VIEWY_AXIAL:
			cstrMessage = L"ERR_VIEWY_AXIAL";
			break;
		case ERR_VIEWY_LOOKERS_AXIAL:
			cstrMessage = L"ERR_VIEWY_LOOKERS_AXIAL";
			break;
		case ERR_VIEWY_HOME_AXIAL:
			cstrMessage = L"ERR_VIEWY_HOME_AXIAL";
			break;
		case ERR_VIEWY_LIMIT_AXIAL:
			cstrMessage = L"ERR_VIEWY_LIMIT_AXIAL";
			break;
		case ERR_VIEWY_STEPS_AXIAL:
			cstrMessage = L"ERR_VIEWY_STEPS_AXIAL";
			break;
		case ERR_VIEWX_RADIAL:
			cstrMessage = L"ERR_VIEWX_RADIAL";
			break;
		case ERR_VIEWX_LOOKERS_RADIAL:
			cstrMessage = L"ERR_VIEWX_LOOKERS_RADIAL";
			break;
		case ERR_VIEWX_HOME_RADIAL:
			cstrMessage = L"ERR_VIEWX_HOME_RADIAL";
			break;
		case ERR_VIEWX_LIMIT_RADIAL:
			cstrMessage = L"ERR_VIEWX_LIMIT_RADIAL";
			break;
		case ERR_VIEWX_STEPS_RADIAL:
			cstrMessage = L"ERR_VIEWX_STEPS_RADIAL";
			break;
		case ERR_VIEWY_RADIAL:
			cstrMessage = L"ERR_VIEWY_RADIAL";
			break;
		case ERR_VIEWY_LOOKERS_RADIAL:
			cstrMessage = L"ERR_VIEWY_LOOKERS_RADIAL";
			break;
		case ERR_VIEWY_HOME_RADIAL:
			cstrMessage = L"ERR_VIEWY_HOME_RADIAL";
			break;
		case ERR_VIEWY_LIMIT_RADIAL:
			cstrMessage = L"ERR_VIEWY_LIMIT_RADIAL";
			break;
		case ERR_VIEWY_STEPS_RADIAL:
			cstrMessage = L"ERR_VIEWY_STEPS_RADIAL";
			break;			
//----------------------------------------------------------------	

		case ERR_LOOKER:
			cstrMessage = L"ERR_LOOKER";
			break;
		case HIGH_UV_DARK_CURRENT:
			cstrMessage = L"HIGH_UV_DARK_CURRENT";
			break;
		case HIGH_VIS_DARK_CURRENT:
			cstrMessage = L"HIGH_VIS_DARK_CURRENT";
			break;
		case HIGH_DARK_CURRENT:
			cstrMessage = L"HIGH_DARK_CURRENT";
			break;
		case NO_UV_MERCURY_LAMP:
			cstrMessage = L"NO_UV_MERCURY_LAMP";
			break;
		case NO_VIS_MERCURY_LAMP:
			cstrMessage = L"NO_VIS_MERCURY_LAMP";
			break;
		case NO_MERCURY_LAMP:
			cstrMessage = L"NO_MERCURY_LAMP";
			break;
		case UV_SHUTTER_FAILURE:
			cstrMessage = L"UV_SHUTTER_FAILURE";
			break;
		case VIS_SHUTTER_FAILURE:
			cstrMessage = L"VIS_SHUTTER_FAILURE";
			break;
		case SHUTTER_FAILURE:
			cstrMessage = L"SHUTTER_FAILURE";
			break;
		case CCD_POWER_OFF:
			cstrMessage = L"CCD_POWER_OFF";
			break;
		case CCD_POWER_ON:
			cstrMessage = L"CCD_POWER_ON";
			break;
		case TOP_PRIMARY_SENSOR_FAILED:
			cstrMessage = L"TOP_PRIMARY_SENSOR_FAILED";
			break;
		case BOTTOM_PRIMARY_SENSOR_FAILED :
			cstrMessage = L"BOTTOM_PRIMARY_SENSOR_FAILED ";
			break;
		case TOP_SECONDARY_SENSOR_FAILED:
			cstrMessage = L"TOP_SECONDARY_SENSOR_FAILED";
			break;
		case BOTTOM_SECONDARY_SENSOR_FAILED:
			cstrMessage = L"BOTTOM_SECONDARY_SENSOR_FAILED";
			break;
		case ERROR_TOROID_MOVE_TO_HOME:	
			cstrMessage = L"ERROR_TOROID_MOVE_TO_HOME";
			break;		
		case ERROR_TOROID_MOVE_OFF_HOME:
			cstrMessage = L"ERROR_TOROID_MOVE_OFF_HOME";
			break;		
		case ERROR_SHUTTER_MOTOR_MOVE_OFF_SENSOR:
			cstrMessage = L"ERROR_SHUTTER_MOTOR_MOVE_OFF_SENSOR";
			break;		
		case ERROR_SHUTTER_MOTOR_MOVE_TO_SENSOR:
			cstrMessage = L"ERROR_SHUTTER_MOTOR_MOVE_TO_SENSOR";
			break;		
		case ERROR_SHUTTER_MOTOR_MOVE_TO_HOME:
			cstrMessage = L"ERROR_SHUTTER_MOTOR_MOVE_TO_HOME";
			break;		
		case ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS:
			cstrMessage = L"ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS";
			break;		
		case ERR_ARGON_FLOW:
			cstrMessage = L"ERR_ARGON_FLOW";
			if (!(ARGON_FLG & m_nFatal))
				m_nFatal |= ARGON_FLG;
			cstrFatalWarn = L"ARGON_FLG";
			bFatal = true;
			break;
		case MSG_ARGON_FLOW:
			cstrMessage = L"MSG_ARGON_FLOW";
			m_nFatal &= ~ARGON_FLG;
			cstrFatalWarn = L"~ARGON_FLG";
			bFatal = true;
			break;
		case WARN_NITROGEN_FLOW:
			cstrMessage = L"WARN_NITROGEN_FLOW";
			if (!(NITROGEN_FLG & m_nWarning))
				m_nWarning |= NITROGEN_FLG;
			cstrFatalWarn = L"NITROGEN_FLG";
			bWarn = true;
			break;
		case MSG_NITROGEN_FLOW:
			cstrMessage = L"MSG_NITROGEN_FLOW";
			m_nWarning &= ~NITROGEN_FLG;
			cstrFatalWarn = L"~NITROGEN_FLG";
			bWarn = true;
			break;
		case ERR_WATER_LEAK:
			cstrMessage = L"ERR_WATER_LEAK";
			if (!(WATER_LEAK_FLG & m_nFatal))
				m_nFatal |= WATER_LEAK_FLG;
			cstrFatalWarn = L"WATER_LEAK_FLG";
			bFatal = true;
			break;
		case MSG_WATER_LEAK:
			cstrMessage = L"MSG_WATER_LEAK";
			m_nFatal &= ~WATER_LEAK_FLG;
			cstrFatalWarn = L"~WATER_LEAK_FLG";
			bFatal = true;
			break;
		case ERR_24VDC_MIN:
		case ERR_24VDC_MAX:
			cstrMessage = L"ERR_24VDC_MIN or MAX";
			if (!(PS24VDC_FLG & m_nFatal))
				m_nFatal |= PS24VDC_FLG;
			cstrFatalWarn = L"PS24VDC_FLG";
			bFatal = true;
			break;
		case WARN_24VDC_LOW:
		case WARN_24VDC_HIGH:
			cstrMessage = L"WARN_24VDC_LOW or HIGH";
			if (PS24VDC_FLG & m_nFatal)
				m_nFatal   &= ~PS24VDC_FLG;
			if (!(PS24VDC_FLG & m_nWarning))
				m_nWarning |= PS24VDC_FLG;
			cstrFatalWarn = L"~+PS24VDC_FLG";  //becomes a warning
			bWarn = true;
			break;
		case MSG_24VDC_OK:
			cstrMessage = L"MSG_24VDC_OK";
			m_nWarning &= ~PS24VDC_FLG;
			m_nFatal   &= ~PS24VDC_FLG;
			cstrFatalWarn = L"~PS24VDC_FLG";
			bFatal = true;
			bWarn = true;
			break;
		case ERR_UV_COOLER_MIN:
		case ERR_UV_COOLER_MAX:
		case ERR_UV_COOLER_READOUT:
			cstrMessage = L"ERR_UV_COOLER_MIN or MAX or READOUT";
			if (!(UV_COOLER_FLG & m_nFatal))
				m_nFatal |= UV_COOLER_FLG;
			cstrFatalWarn = L"UV_COOLER_FLG";
			bFatal = true;
			break;
		case WARN_UV_COOLER_LOW:
			cstrMessage = L"WARN_UV_COOLER_LOW";
			if (UV_COOLER_FLG & m_nFatal)
				m_nFatal   &= ~UV_COOLER_FLG;
			if (!(UV_COOLER_FLG & m_nWarning))
				m_nWarning |= UV_COOLER_FLG;
			cstrFatalWarn = L"~+UV_COOLER_FLG"; //becomes a warning
			bWarn = true;
			break;
		case MSG_UV_COOLER_OK:
			cstrMessage = L"MSG_UV_COOLER_OK";
			m_nWarning &= ~UV_COOLER_FLG;
			m_nFatal   &= ~UV_COOLER_FLG;
			cstrFatalWarn = L"~UV_COOLER_FLG";
			bFatal = true;
			bWarn = true;
			break;
		case ERR_VIS_COOLER_MIN:
		case ERR_VIS_COOLER_MAX:
		case ERR_VIS_COOLER_READOUT:
			cstrMessage = L"ERR_VIS_COOLER_MIN or MAX or READOUT";
			if (!(VIS_COOLER_FLG & m_nFatal))
				m_nFatal |= VIS_COOLER_FLG;
			cstrFatalWarn = L"VIS_COOLER_FLG";
			bFatal = true;
			break;
		case WARN_VIS_COOLER_LOW:
			cstrMessage = L"WARN_VIS_COOLER_LOW";
			if (VIS_COOLER_FLG & m_nFatal)
				m_nFatal   &= ~VIS_COOLER_FLG;
			if (!(VIS_COOLER_FLG & m_nWarning))
				m_nWarning |= VIS_COOLER_FLG;
			cstrFatalWarn = L"~+VIS_COOLER_FLG"; //becomes a warning
			bWarn = true;
			break;
		case MSG_VIS_COOLER_OK:
			cstrMessage = L"MSG_VIS_COOLER_OK";
			m_nWarning &= ~VIS_COOLER_FLG;
			m_nFatal   &= ~VIS_COOLER_FLG;
			cstrFatalWarn = L"~VIS_COOLER_FLG";
			bFatal = true;
			bWarn = true;
			break;
		case ERR_UV_CCD_MAX:
		case ERR_UV_CCD_MIN:
		case ERR_UV_CCD_HEATING:
			cstrMessage = L"ERR_UV_CCD_MAX or MIN or HEATING";
			if (!(UV_CCD_FLG & m_nFatal))
				m_nFatal |= UV_CCD_FLG;
			cstrFatalWarn = L"UV_CCD_FLG";
			bFatal = true;
			break;
		case WARN_UV_CCD_HIGH:
			cstrMessage = L"WARN_UV_CCD_HIGH";
			if (UV_CCD_FLG & m_nFatal)
				m_nFatal   &= ~UV_CCD_FLG;
			if (!(UV_CCD_FLG & m_nWarning))
				m_nWarning |= UV_CCD_FLG;
			cstrFatalWarn = L"~+UV_CCD_FLG"; //becomes a warning
			bWarn = true;
			break;
		case MSG_UV_CCD_OK:
			cstrMessage = L"MSG_UV_CCD_OK";
			m_nWarning &= ~UV_CCD_FLG;
			m_nFatal   &= ~UV_CCD_FLG;
			cstrFatalWarn = L"~UV_CCD_FLG";
			bFatal = true;
			bWarn = true;
			break;
		case ERR_VIS_CCD_MAX:
		case ERR_VIS_CCD_MIN:
		case ERR_VIS_CCD_HEATING:
			cstrMessage = L"ERR_VIS_CCD_MAX or MIN or HEATING";
			if (!(VIS_CCD_FLG & m_nFatal))
				m_nFatal |= VIS_CCD_FLG;
			cstrFatalWarn = L"VIS_CCD_FLG";
			bFatal = true;
			break;
		case WARN_VIS_CCD_HIGH:
			cstrMessage = L"WARN_VIS_CCD_HIGH";
			if (VIS_CCD_FLG & m_nFatal)
				m_nFatal   &= ~VIS_CCD_FLG;
			if (!(VIS_CCD_FLG & m_nWarning))
				m_nWarning |= VIS_CCD_FLG;
			cstrFatalWarn = L"~+VIS_CCD_FLG"; //becomes a warning
			bWarn = true;
			break;
		case MSG_VIS_CCD_OK:
			cstrMessage = L"MSG_VIS_CCD_OK";
			m_nWarning &= ~VIS_CCD_FLG;
			m_nFatal   &= ~VIS_CCD_FLG;
			cstrFatalWarn = L"~+VIS_CCD_FLG";
			bFatal = true;
			bWarn = true;
			break;
		case WARN_FAN_TEMP_LOW:
		case WARN_FAN_TEMP_HIGH:
			cstrMessage = L"WARN_FAN_TEMP_LOW or HIGH";
			if (!(FAN_FLG & m_nWarning))
				m_nWarning |= FAN_FLG;
			cstrFatalWarn = L"FAN_FLG";
			bWarn = true;
			break;
		case MSG_FAN_TEMP_OK:
			cstrMessage = L"MSG_FAN_TEMP_OK";
			m_nWarning &= ~FAN_FLG;
			cstrFatalWarn = L"~FAN_FLG";
			bWarn = true;
			break;
		case ERR_CASTING_TEMP_FAILED:
			cstrMessage = L"ERR_CASTING_TEMP_FAILED";
			if (!(CASTING_FLG & m_nFatal))
				m_nFatal |= CASTING_FLG;
			cstrFatalWarn = L"CASTING_FLG";
			bFatal = true;
			break;
		case ERR_CAMERA_OFF_UV:
			cstrMessage = L"ERR_CAMERA_OFF_UV";
			cstrFatalWarn = L"Turning back on";
			bWarn = true;
			break;
		case ERR_CAMERA_OFF_VIS:
			cstrMessage = L"ERR_CAMERA_OFF_VIS";
			cstrFatalWarn = L"Turning back on";
			bWarn = true;
			break;
		default:
		{
			short	nTemp   = (short) (0xff00 & m_nCmd);
					nTemp >>= 8;
			char	cChar1  = (char) nTemp;
			char	cChar2  = (char) (0x00ff & m_nCmd);
			printf("ErrorMsg: Unknown Message number (%8.8X). Recent Command:0x%x == (%c%c)\r\n", message, m_nCmd, cChar1, cChar2);
			return;
		}
	}
	
	switch (fatal)
	{
	case YES:
		cstrFatalArg = L"FATAL_ERROR";
		cstrFatal = L"YES";
		break;
	case NO:
		cstrFatalArg = L"WARNING_MSG";
		cstrFatal = L"NO";
		break;
	case OK:
		cstrFatalArg = L"OK_MESSAGE";
		cstrFatal = L"YES";
		break;
	case ABORT:
		cstrFatalArg = L"CMD_XX";
		cstrFatal = L"OK";
		break;
	default:
		cstrFatalArg = L"Unknown";
		cstrFatal = L"default";
		break;
	}
	cstrFatalMsg = bFatal ? "Yes" : "No";
	cstrWarnMsg  = bWarn  ? "Yes" : "No";
	char	cErr2 = (char) (message & 0xff);
	message &= 0xff00;
	message >>= 8;
	char	cErr1 = (char) message;

	wprintf(L"ErrorMsg(%c%c, %s) (%s, %s)\r\n     Fatal: %s  Warn: %s  %s\r\n", cErr1, cErr2, cstrFatal, cstrMessage,
												cstrFatalArg, cstrFatalMsg,
												cstrWarnMsg,  cstrFatalWarn);
//...........................................................................................................												cstrWarnMsg,  cstrFatalWarn); 
{ // THN January 2007 ... Display time fatal was entered to debug output 
		char	nBuffer[9];
		GetTimeStamp(&nBuffer[0]); //Get time into nBuffer
		printf("%s%8s\r\n", "Time fatal mode entered: ", nBuffer);
}
//...........................................................................................................
}
//  Gets here as the result of an error or Sleep mode is commanded entered
//  by the sleep command (SM). Error enters Fatal mode.
//
//===========================================================================
void CParserThread::ShutDown(unsigned int message)
{
	unsigned long	 location;

	// DO AN ORDERLY SHUTDOWN
	m_cInitFlag = FATAL;
	if (message != NO_ERRORS)
	{
		m_cStartupMode = FATAL;
		m_nFatalCode   = (WORD) message;
	}
	else
	{
		m_cStartupMode = SLEEP;
		m_nFatalCode   = NO_ERRORS;
	}
	m_cOperationMode = DIAG;

	// DETECTOR POWER OFF
	if (m_bCCDPower)
	{
		m_nPowerRegisterUV &= ~POWER_ON;
		m_CameraIO.UVOutputW( POWER_SHUTDOWN_UV,  m_nPowerRegisterUV);
		if (m_bSingleChannel == false)
		{
			m_nPowerRegisterVIS &= ~POWER_ON;
			m_CameraIO.VISOutputW(POWER_SHUTDOWN_VIS, m_nPowerRegisterVIS);
		}
		m_bCCDPower = false;
	}

	// UV COOLER and VIS COOLER OFF
	m_nDigitalControl &= ~(TEC_EN_UV | TEC_EN_VIS);
	m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);

	// TURN OFF WATER FLOW
	m_nSolenoidRegister &= ~WATER_EN;
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);

	if (message != ERR_UV_CCD_MAX && message != ERR_VIS_CCD_MAX)
	{
		// NITROGEN FLOW LOW
		m_nSolenoidRegister &= ~NITROGEN_HI;
		m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);

		// ARGON FLOW OFF & LOW
		m_nSolenoidRegister &= ~ARGON_EN;
		m_nSolenoidRegister &= ~ARGON_HI;
		m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);

		m_bCCDPower = false;
	}
	else
		m_bCCDPower = true;

	//  Fatal mode entered
	if (message != NO_ERRORS)
	{
		if ( m_nErrorCnt < MAX_ERROR) // Increment to MAX_ERROR; not beyond
			++m_nErrorCnt;
		location = 4 * m_nErrorCnt;
		if (m_nErrorCnt == MAX_ERROR)
				message = ERROR_BUFFER_FULL;   // Mark error buffer full

			m_nErrorOut[location    ] = FATAL_ERROR;
			m_nErrorOut[location + 1] = FATAL_ERROR;
			char	cErr2 = (char) (message & 0xff);
			message &= 0xff00;
			message >>= 8;
			char	cErr1 = (char) message;
			m_nErrorOut[location + 2] = cErr1;
			m_nErrorOut[location + 3] = cErr2;
			//...........................................................
			// Fatal mode entry placed into the Spectrometer log.
			char* szStr = new char[40];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Fatal: %c%c\0", cErr1, cErr2);
			AddToSpecLog(szStr);
			//...........................................................
	}
	// Sleep mode entered (Also called standby mode)
	else
	{
		m_bSleepFlag = true;
		SaveOffTime(ARGON_TIME); // Save current time in argon off time area
		
		//...........................................................
		// Sleep mode entry placed into the Spectrometer log.
		char* szStr = new char[40];
		GetDateTimeStamp(szStr);
		sprintf(&szStr[19], " Mode: Sleep\0");
		AddToSpecLog(szStr);
		//...........................................................
	}
}
//===========================================================================
// Return a threadsafe value of critical parameter m_nWarning
//
unsigned short CParserThread::Getm_nWarning( )
{
	unsigned short nWarning;
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters	
	nWarning = m_nWarning;
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
	return nWarning;
}
//===========================================================================
// Return a threadsafe value of critical parameter m_nFatal
//
unsigned short CParserThread::Getm_nFatal( )
{
	unsigned short nFatal;
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters	
	nFatal = m_nFatal;
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
	return nFatal;
}
//===========================================================================
void	CParserThread::Setm_cInitFlag( char cInitFlag )
{
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters	
	m_cInitFlag = cInitFlag;
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
}
//===========================================================================

/*** Revision History ***

01/11/17    JO    	CBF-125 - Add temperature sensor fault detection.
01/16/17    JO      CBF-133 - Add cmd_JI functionality.
01/27/17	JO		CBF-142 - Add time-outs and error handing to code.
01/31/17	KR		CBF-145 - Remove unused commands and errors
02/03/17	KR		CBF-151 - Additional fixes to run the commands properly & remove ES warning

$Log: /IcarusBased/SpecFW/Errormsg.cpp $
 * 
 * 30    4/07/08 1:24p Nashth
 * Small format fix for Fatal: message for History log.
 * 
 * 29    4/07/08 10:41a Nashth
 * Second attempt at Spectrometer history log.
 * 
 * 28    3/31/08 5:31p Nashth
 * Implementation of spectrometer history log... First pass.
 * 
 * 27    6/07/07 4:34p Nashth
 * Allow camera off warnings during Mfg mode.
 * 
 * 26    6/07/07 2:53p Nashth
 * Added a warning message should either Camera shutdown as per QPR:
 * THNJ73XQ2S.
 * 
 * 25    1/10/07 1:15p Nashth
 * Utilized unused Fatal mode error codes U4 and V4  for UV and VIS
 * detector temperature readout errors as per QPR: CANU6WMPKS.
 * 
 * 24    1/02/07 4:48p Nashth
 * Added more Detector temperature debug.
 * 
 * 23    6/14/06 8:46a Frazzitl
 * Fixed QPR CANU6QPLCG by defining the T6, TUB TEMP TOO LOW, error
 * message.
 * 
 * 22    5/31/06 1:53p Nashth
 * 
 * 21    5/31/06 1:37p Nashth
 * Modified printf error messages as per QPR:  THNJ6QBNGV.
 * 
 * 20    4/21/06 1:10p Frazzitl
 * Added feature to check for and generate an error if either some or all
 * pixels are not received from the detector for a given subarray.
 * 
 * 19    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 18    9/30/05 9:46a Frazzitl
 * Added new error messages indicating the top or bottom heater plate has
 * been turned off.
 * 
 * 17    8/24/05 13:35 Frazzitl
 * Enabled Shutdown mode on Fatal errors, and removed unused parameter
 * 
 * 16    8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 15    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 14    7/06/05 16:35 Frazzitl
 * Corrected minor problems
 * 
 * 13    7/05/05 10:30 Frazzitl
 * Fixed error message output
 * 
 * 12    7/01/05 15:38 Frazzitl
 * Fixed the Tub heater problem
 * 
 * 11    6/29/05 16:14 Frazzitl
 * More minor changes
 * 
 * 10    6/17/05 10:09a Frazzitl
 * Added feature to handle Single Channel instruments.
 * 
 * 9     6/07/05 8:25a Frazzitl
 * Separated camera I/O into UV and VIS routines to accommodate individual
 * status checking
 * 
 * 8     5/31/05 18:08 Frazzitl
 * Fixed problem with error handling
 * 
 * 7     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 6     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 5     4/20/05 17:00 Frazzitl
 * Corrected some variable problems
 * 
 * 4     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 3     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:22 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
