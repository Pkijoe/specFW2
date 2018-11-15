//===========================================================================
//
//	Module Name:	delay.cpp
//
// 	Function:		This file delays for specified time and checks for limit:
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

// The drive type can indicates which motor to set an error for upon error

short CParserThread::delay( unsigned int nsDrive )
{
	WORD			SVstatus;
	unsigned short	unsWaitTime;
	short			nsRetCode;  // Return value  

	theApp.EnterCriticalSection1(&m_CriticalSection); 
	unsWaitTime = m_nWaitTime;
	theApp.LeaveCriticalSection1(&m_CriticalSection); 

	Sleep(unsWaitTime);   // Outside critical section 

	theApp.EnterCriticalSection1(&m_CriticalSection); 
	SVstatus = m_IO.InputW(SVSTAT_CS);
	if (SVstatus & m_nLooker)
		m_cLookerCnt++;
	else
		m_cLookerCnt = 0;

	if (m_cLookerCnt > 1)	// Must have two in a row to count as an error
	{
		if ( nsDrive == SLIT )
			nsRetCode = ERR_SLIT_LOOKERS;
		else if ( nsDrive == VIEWX_AXIAL )
			nsRetCode = ERR_VIEWX_LOOKERS_AXIAL ;
		else if ( nsDrive == VIEWX_RADIAL  )
			nsRetCode = ERR_VIEWX_LOOKERS_RADIAL;		
		else if ( nsDrive == VIEWY_AXIAL )
			nsRetCode = ERR_VIEWY_LOOKERS_AXIAL;
		else if ( nsDrive == VIEWY_RADIAL )
			nsRetCode = ERR_VIEWY_LOOKERS_RADIAL;		
		else nsRetCode = ERR_LOOKER; // Should never happen
	}
	else
		nsRetCode = NO_ERRORS;

	theApp.LeaveCriticalSection1(&m_CriticalSection); 
	return nsRetCode;
}
//===========================================================================

/*** Revision History ***

12/15/15    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/delay.cpp $
 * 
 * 6     7/31/07 2:47p Nashth
 * Motor errors <LE> now returned as a specific motor error <S0>, <X0>, or
 * <Y0> as per QPR: THNJ75ML7T.
 * 
 * 5     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 4     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 3     4/04/05 12:56 Frazzitl
 * more needed to be done
 * 
 * 2     4/04/05 10:44 Frazzitl
 * Fixing problem with motor timing - only temporary
 * 
 * 1     3/17/05 11:22 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
