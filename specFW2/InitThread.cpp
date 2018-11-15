//===========================================================================
//
//	Module Name:	InitThread.cpp
//
// 	Function:		This routine checks the Init of the Spectrometer
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/InitThread.cpp 11    6/29/07 5:08p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

/////////////////////////////////////////////////////////////////////////////
// initHwThread

UINT WINAPI CParserThread::initHwThread(LPVOID pParam)
{
	CParserThread	*pThis = (CParserThread *) pParam;
	unsigned int	nStatus(NO_ERRORS);
	long			lTimeOn;	// Automatic variables for Time On and
	long			lTimeNow;   // the current time
	//...................................................................
	// local definitions for protected instance parameters 
	//  All initialized to zero for Good Practice 
	char		cStartupMode = 0;
	//...................................................................
	pThis->Setm_cInitFlag( YES );    //  The full command set is Not allowed 
	//......................................................................................
	// Wait 10 minutes or until m_cStartupMode in not NOT_ON
	//
	//   Exit loop With Startup Cold and Operation Ready, unless
	//   MD command sets them otherwise...
	//     Both equal to SETUP, AUTO MFG, DIAG, or READY.
	//	   or Startup = COLD/HOT and Operation = READY
	//       (Note: SETUP No longer used)
	//
	//
	
	lTimeOn = pThis->GetOnTime();

	do
	{
		Sleep(1000);
		// Get current time relative to m_lONtime 
		lTimeNow = pThis->getTime(YES);  // m_lCurrentTime
		//....................................................
		// If the clock time changes by more than 1 day then
		// someone reprogrammed the clock while in this loop
		// ... Reset the On time and restart this 10 minute loop
		if ( lTimeNow == 0 )
			lTimeOn = pThis->GetOnTime();
		//....................................................
		else if ((lTimeNow - lTimeOn) > MAX_NOT_ON_TIME)
		{
			// Set these atomically
			theApp.EnterCriticalSection1(&pThis->m_CriticalSection);
			pThis->m_cStartupMode = COLD;
			pThis->m_cOperationMode = READY;
			theApp.LeaveCriticalSection1(&pThis->m_CriticalSection);
		}

		theApp.EnterCriticalSection1(&pThis->m_CriticalSection); 
		cStartupMode = pThis->m_cStartupMode;
		theApp.LeaveCriticalSection1(&pThis->m_CriticalSection);

	} while (cStartupMode == NOT_ON);
	//......................................................................................
	//  Wait here if other than COLD/HOT (for MD command SETUP, AUTO, MFG, DIAG, or READY)

	if (cStartupMode != COLD && cStartupMode != HOT)
	{
		pThis->Setm_cInitFlag( NO );  // Full Command set allowed
		do
		{
			Sleep(1000);
			theApp.EnterCriticalSection1(&pThis->m_CriticalSection); 
			cStartupMode = pThis->m_cStartupMode;
			theApp.LeaveCriticalSection1(&pThis->m_CriticalSection);

		} while (cStartupMode != COLD && cStartupMode != HOT);
	}
	//......................................................................................
	// Start Hardware init for COLD/HOT mode

	while (1)
	{
		if (cStartupMode == COLD || cStartupMode == HOT)  //THN State will always be Cold or Hot here. Why check?
		{
			pThis->Setm_cInitFlag( YES );  // Full command set is Not allowed
			nStatus = pThis->cfgHdwr();
		}
		//......................................................................................
		if (nStatus == NO_ERRORS)
			pThis->Setm_cInitFlag( NO );    // Full command set is allowed
		else if (nStatus == CMD_CANCEL)
			pThis->Setm_cInitFlag( NO );	// Full command set is allowed
		else
			pThis->Setm_cInitFlag( FATAL );   // Fatal command set allowed  
	
		//------------------------------------------------------
		
		pThis->m_restart_flag = 0;				// Disable any previous events
	    
		while (pThis->m_restart_flag == 0)		// Stay in this loop until we get a hot start command - "cmd_JI"
		{
			Sleep (1000);				
			nStatus = NO_ERRORS;
		}
	}
	return nStatus;
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***


1/16/17    JO      CBF-133 - Add cmd_JI functionality.




$Log: /IcarusBased/SpecFW/InitThread.cpp $
 * 
 * 11    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 10    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 9     9/09/05 4:21p Nashth
 * Fixed entering fatal mode after power on problem as per QPR:
 * THNJ6G3RHV.
 * 
 * 8     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 7     7/20/05 2:48p Nashth
 * Added a few comments and removed some dead code.
 * 
 * 6     6/29/05 16:20 Frazzitl
 * Made more like the original
 * 
 * 5     6/17/05 10:15a Frazzitl
 * Added the passing of the this pointer to the hdwrCfg routine.
 * 
 * 4     5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 3     5/06/05 15:53 Frazzitl
 * New implementation of threads
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     4/01/05 15:17 Frazzitl
 * Added the initialization thread

$NoKeywords: $

** End of Rev History **/
