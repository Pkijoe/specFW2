//===========================================================================
//
//	Module Name:	cmdGT.cpp
//
//	Function:		This routine returns the current time in seconds
//					for Initialization to be completed.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdgt.cpp 3     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGT()
{
	WORD	status(NO_ERRORS);
	long	stime;
	long	mtime;
	WORD	itime;
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	EnterCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters

	stime = m_lKeyTime;

	if (stime != 0)
	{
		mtime = (stime / 60);
		if (stime != (mtime * 60))
			mtime += 1;
	}
	else
		mtime = 0;

	if (mtime > 65535)
		itime = 65535;
	else
		itime = (WORD) mtime;

	if (m_bHolding)
	{
		status = WARN_TIME_ON_HOLD;
		strcpy(m_nDataOutBuf, "GTTH");
	}
	else
	{
		strcpy(m_nDataOutBuf, "GT00xxxxx");	// 5 character string
		s2b(itime, 4);
	}

	LeaveCriticalSection(&m_CriticalSectionClock);  // Release protection
	theApp.LeaveCriticalSection1(&m_CriticalSection);  // Release protection
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdgt.cpp $
 * 
 * 3     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 2     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
