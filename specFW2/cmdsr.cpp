//===========================================================================
//
//	Module Name:	cmdSR.cpp
//
// 	Function:		This routine performs a software reset.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdsr.cpp 5     6/28/07 3:25p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSR()
{
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical data
	strcpy(m_nDataOutBuf, "SR00");

	if (m_cOperationMode == AUTO)
	{
		m_nOPmode	= TEST;		// TEST MODE
		m_nTestMode = NORMAL;	// TEST MODE, NORMAL
	}
	else
	{
		m_nOPmode	= NORMAL;	// NORMAL MODE, NOT TESTING
		m_nTestMode = NORMAL;	// NORMAL
	}

	m_nDataBlock = 0;
	printf("cmdSR cmdCM()... Clear memory, Reset pointers\r\n");
	cmdCM();					// CLEAR MEMORY, RESET POINTERS

	m_nIntegrationTime = 100;	// DEFAULT INTEGRATION TIME - 100 MSEC
	m_bClearC = true;			// CLEAR 'C' MODE OFF

	m_nTotalArrays	= 1;
	m_nArraysUV		= 1;
	m_nArraysVIS	= 0;
	m_ReadTableUV[0].array = m_pVegaArray[0];

	m_cUVres	= LOW;			// SET UV RESOLUTION LOW
	m_cVISres	= HIGH;			// SET VIS RESOLUTION HIGH

	m_bCancelFlag = true;
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdsr.cpp $
 * 
 * 5     6/28/07 3:25p Nashth
 * Initialized LN sub array list correctly as per QPR: THNJ747JS7.
 * 
 * 4     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 3     8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 2     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
