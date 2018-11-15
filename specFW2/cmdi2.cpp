//===========================================================================
//
//	Module Name:	cmdI2.cpp
//
//	Function:		The routine does intialize two call
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdi2.cpp 7     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdI2()
{
	unsigned int	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical data
	strcpy(m_nDataOutBuf, "I200");
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	//=========================================
	// SET RESOLUTION TO LOW FOR BOTH UV & VIS
	//=========================================
	m_cUVres	= LOW;
	m_cVISres	= LOW;

	//================================
	// SET LIST TO SUBARRAYS 84 & 149
	//================================
	m_nTotalArrays	= 1;
	m_nArraysUV		= 1;
	m_ReadTableUV[0].array = m_pVegaArray[84];
	if (m_bSingleChannel == false)
	{
		m_ReadTableVIS[0].array = m_pVegaArray[149];
		m_nTotalArrays	= 2;
		m_nArraysVIS	= 1;
	}

	//===========================
	// SET INTEGRATION TIME TO 0
	//===========================
	m_nIntegrationTime	= 0;
	m_bClearC = true;

	//==============
	// CLEAR MEMORY
	//==============
	printf("cmdI2 cmdCM()... Clear memory, Reset Pointers\r\n");
	if ((status = cmdCM()) != NO_ERRORS)
		return status;

	//============================
	// READ THE DATA INTO BLOCK 1
	//============================
	printf("cmdI2 cmdRO()... Read data into Block 1\r\n");
	if ((status = cmdRO()) != NO_ERRORS)
		return status;

	//======================================
	// CHANGE INTEGRATION TIME TO 4000 MSEC
	//======================================
	m_nIntegrationTime = 4000;

	//============================
	// READ THE DATA INTO BLOCK 2
	//============================
	printf("cmdI2 cmdRO()... Read data into Block 2\r\n");
	if ((status = cmdRO()) != NO_ERRORS)
		return status;

	//============================================
	// MAXIMUM READING OF 2,000 electrons / pixel
	//============================================
	printf("cmdI2 cal_dc()... Check for max reading of 2000 electrons/pixel\r\n");
	if ((status = cal_dc()) != NO_ERRORS)
		return status;

	//===========================
	// SET INTEGRATION TIME TO 0
	//===========================
	m_nIntegrationTime = 0;

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdi2.cpp $
 * 
 * 7     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 6     8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
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
