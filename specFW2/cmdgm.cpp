//===========================================================================
//
//	Module Name:	cmdGM.cpp
//
//	Function:		This routine returns the current Detector Chip Mode:
//					ZODIAC ( 0 ), VEGA ( 1 ), VEGA SCA ( 2 ).
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdgm.cpp 4     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGM()
{
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "GM00x");			// 1 character string

	memcpy(&m_nDataOutBuf[4], &m_cChipMode, 1);
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdgm.cpp $
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
 * 2     4/12/05 3:33p Frazzitl
 * Fixing problem with Chip Mode
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
