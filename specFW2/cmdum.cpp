//===========================================================================
//
//	Module Name:	cmdUM.cpp
//
// 	Function:	  	This routine accepts the Unsolicited Messages mode.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdum.cpp 5     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdUM()
{
	WORD	status(NO_ERRORS);
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters

	m_pCmdPtr++;	// just ignore character since command for compatability only
	m_nBytesRead++;

	strcpy(m_nDataOutBuf, "UM00");

	m_bUnsolicitedMsg = false;
	m_NvRam.OutputB(UNSOLICITED_MSG, false);

	m_nErrorCnt		= 0;

	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdum.cpp $
 * 
 * 5     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 4     8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 3     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
