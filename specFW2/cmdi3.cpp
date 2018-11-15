//===========================================================================
//
//	Module Name:	cmdI3.cpp
//
//	Function:		The routine does intialize three call
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdi3.cpp 8     11/29/05 5:13p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdI3()
{
	//printf("THN cmdI3()... Entry 1\r\n");
	unsigned int	status(NO_ERRORS);
	//printf("THN cmdI3()... theApp.EnterCriticalSection1()\r\n");
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical data
	//printf("THN cmdI3()... strcpy()\r\n");
	strcpy(m_nDataOutBuf, "I300");
	//printf("THN cmdI3()... theApp.LeaveCriticalSection1()\r\n");
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	//=====================================
	// CHANGE INTEGRATION TIME TO 250 MSEC
	//=====================================
	m_nIntegrationTime = 250;

	//============================
	// READ THE DATA INTO BLOCK 3
	//============================

	//printf("THN cmdI3()... cmdRO()\r\n");
	if ((status = cmdRO()) != NO_ERRORS)
		return status;

	//==============================================
	// MINIMUM READING OF 100,000 electrons / pixel
	//==============================================
	//printf("THN cmdI3()... cal_peak()\r\n");
	if ((status = cal_peak()) != NO_ERRORS)
	{
		if (status == NO_MERCURY_LAMP)
			m_bTestFlag = true;						// Must be both UV & VIS
		return status;
	}
	else
		m_bTestFlag = false;

	//===========================
	// SET INTEGRATION TIME TO 0
	//===========================
	m_nIntegrationTime = 0;

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdi3.cpp $
 * 
 * 8     11/29/05 5:13p Nashth
 * Commented out some printfs used for debug.
 * 
 * 7     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 6     11/22/05 3:59p Nashth
 * More printfs added for debugging.
 * 
 * 5     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 4     7/18/05 14:41 Frazzitl
 * Implemented changes required to read and check data during hardware
 * initialization
 * 
 * 3     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
