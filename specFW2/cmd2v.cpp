//===========================================================================
//
//	Module Name:	cmd2V.cpp
//
// 	Function:		This routine gets the complete Kernel version information.
//
//	Original Author:	TLFrazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmd2v.cpp 7     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd2V()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "2V00");

	int	nLen = m_sKernelInformation.GetLength();

	for (int n = 0; n < nLen; n++)
		m_nDataOutBuf[n + 4] = (char) m_sKernelInformation.GetAt(n);

	m_nDataOutBuf[nLen + 4] = NULL;
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmd2v.cpp $
 * 
 * 7     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 6     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 5     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 4     6/08/05 10:22a Frazzitl
 * Added classes needed to obtain Kernel Information and Version Numbers
 * 
 * 3     6/06/05 8:45a Frazzitl
 * Changed comment on function of this routine.
 * 
 * 2     5/18/05 8:23a Frazzitl
 * Modified what command does
 * 
 * 1     3/17/05 11:12 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
