//===========================================================================
//
//	Module Name:	cmdHO.cpp
//
// 	Function:		This routine gets the optics counter hours.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdho.cpp 4     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdHO()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);
	
	strcpy(m_nDataOutBuf, "HO00");

	sprintf(&m_nDataOutBuf[4], "%05d", (m_lOptics_hours / 60));
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdho.cpp $
 * 
 * 4     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 3     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 2     3/30/05 10:48 Frazzitl
 * Fixed problem with pointer
 * 
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
