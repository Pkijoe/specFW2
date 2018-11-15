//===========================================================================
//
//	Module Name:	cmd1V.cpp
//
// 	Function:		This routine gets the Kernel image version.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmd1v.cpp 4     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd1V()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);  // Protecrt critical parameters

	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "1V00");

	int	nLen = m_sKernelVersion.GetLength();

	for (int n = 0; n < nLen; n++)
		m_nDataOutBuf[n + 4] = (char) m_sKernelVersion.GetAt(n);

	m_nDataOutBuf[nLen + 4] = NULL;
	theApp.LeaveCriticalSection1(&m_CriticalSection);  // Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmd1v.cpp $
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
 * 2     5/05/05 9:57 Frazzitl
 * Added routine to get Kernel version number.
 * 
 * 1     3/17/05 11:12 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
