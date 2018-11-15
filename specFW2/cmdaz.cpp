//===========================================================================
//
//	Module Name:	cmdAZ.cpp
//
// 	Function:		This routine activate the thermal sensor and run fault detection
//
//	Original Author:	 KRowberry
//
//	Copyright (c) 2017,  PerkinElmer, DAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdAU()	// Set Upper Active Sensor
{
	WORD	status(NO_ERRORS);
	char	val;
	strcpy(m_nDataOutBuf, "AU00");	
	val = *m_pCmdPtr++;
	m_nBytesRead++;

	//Set the Upper Sensor as the Top Primary Sensor (0) or Top Secondary Sensor
	if  ((val == '0')||(val == '1'))
	{
		m_NvRam.OutputB(SENSOR_UPPER_ACTIVE,(unsigned char)(val - ASCII0));
	}	
    else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);	
	}	
	return status;
}

unsigned int CParserThread::cmdAL() // Set Lower Active Sensor
{
	WORD	status(NO_ERRORS);
	char	val;
	strcpy(m_nDataOutBuf, "AL00");
	val = *m_pCmdPtr++;
	m_nBytesRead++;

	//Set the Lower Sensor as the Bottom Primary Sensor (0) or Bottom Secondary Sensor
	if  ((val == '0')||(val == '1'))
	{
		m_NvRam.OutputB(SENSOR_LOWER_ACTIVE,(unsigned char)(val - ASCII0));
	}	
    	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);	
	}	
	return status;
}

unsigned int CParserThread::cmdUA() // Get Upper Active Sensor
{
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "UA00x");						// 1 character string
	
	m_nDataOutBuf[4] = (char) (m_NvRam.InputB(SENSOR_UPPER_ACTIVE) + ASCII0);

	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}

unsigned int CParserThread::cmdLA()  // Get Lower Active Sensor
{
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "LA00x");						// 1 character string
	
	m_nDataOutBuf[4] = (char) (m_NvRam.InputB(SENSOR_LOWER_ACTIVE) + ASCII0);

	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}


unsigned int CParserThread::cmdCA()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "CA00x");						// 1 character string
	
//	Calibrate_Temp_Sensors();

	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}

//===========================================================================

/*** Revision History ***
01/09/17    KR      CBF-126 - Create get/set commands for active sensors

** End of Rev History **/