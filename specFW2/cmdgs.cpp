//===========================================================================
//
//	Module Name:	cmdGS.cpp
//
//	Function:		This routine returns the current Single Channel Mode:
//					NO (0) or YES (1).
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdgs.cpp 3     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGS()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);
	char	cSingleChannel;

	strcpy(m_nDataOutBuf, "GS00x");		// 1 character string

	if (m_bSingleChannel)
		cSingleChannel = '1';	// Yes
	else
		cSingleChannel = '0';	// No

	memcpy(&m_nDataOutBuf[4], &cSingleChannel, 1);
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdgs.cpp $
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
