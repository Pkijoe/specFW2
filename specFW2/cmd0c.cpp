//===========================================================================
//
//	Module Name:	cmd0C.cpp
//
//	Function:		This routine returns the Heater control state (ON/OFF).
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmd0c.cpp 4     11/29/05 11:35a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd0C()
{
	WORD	status(NO_ERRORS);
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters

	strcpy(m_nDataOutBuf, "0C00x");			// 1 character string

	m_nDataOutBuf[4] = m_NvRam.InputB(HEATER_CONTROL);
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmd0c.cpp $
 * 
 * 4     11/29/05 11:35a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 3     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:12 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
