//===========================================================================
//
//	Module Name:	cmdiD.cpp
//
// 	Function:		This routine gets the Debug options parameter.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmd1d.cpp 3     11/29/05 11:35a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd1D()
{
	WORD	status(NO_ERRORS);
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters

	strcpy(m_nDataOutBuf, "1D00xxxx");		// 4 character string

	h2b4(m_nDebug, 4);
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmd1d.cpp $
 * 
 * 3     11/29/05 11:35a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 2     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 1     3/17/05 11:12 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
