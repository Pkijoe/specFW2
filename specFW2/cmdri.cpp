//===========================================================================
//
//	Module Name:	cmdRI.cpp
//
// 	Function:	  	The cmdRI is used to Re-Initialize the hardware.(Hardware Reset)
//                   
//					The cmdJI is used to Just-Initialize by restarting the reconfigured 
//					hardware cfghdwr().
//
//	Original Author:	T Frazzini
//  Modified by:        T Nash
//
//	Copyright (c) 2005,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================

 #include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdRI()		// Re-Initialize command
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	CString	sPath;
	CString sCmdLine; 
	WORD	wStatus(NO_ERRORS);
    int		nRetCode;
    PROCESS_INFORMATION processInfo;

	sPath =    L"\\Windows\\IcarusReset.exe";     // Launch the Reset command as per QPR: THNJ6R6KKU
	sCmdLine = L"2";	// Request a 2 second delay to allow a command reply msg
						// to be sent prior to the reset.

	strcpy(m_nDataOutBuf, "RI00");

	m_cInitFlag = FATAL;  // Fatal if the cmd does not succeed

    nRetCode =  CreateProcess( sPath, sCmdLine, NULL, NULL, NULL,
                         NULL,NULL, NULL,
                         NULL, &processInfo);
	if ( nRetCode == 0 )
		wStatus = CMD_CANCEL; // Unknown error ... set Cancel error  
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return wStatus;
}

unsigned int CParserThread::cmdJI()	// Just-Initialized by restarting the configured hardware.
{
	theApp.EnterCriticalSection1(&m_CriticalSection);

	WORD	wStatus(NO_ERRORS);
  
	strcpy(m_nDataOutBuf, "JI00");
	
//****************************************************************************
//for testing only without host computer	
	
//SaveOffTime(ARGON_TIME); // Save current time in argon off time area

//****************************************************************************


	m_bHWInitComplete = false;
	m_bSleepFlag = false;
	m_cStartupMode = COLD;
	m_cOperationMode = READY;	
	m_argon_flag = 1;	
	m_restart_flag = 1;				// Launch cfghdrw()	from initThread.
	GetOnTime();					// Restart time

	printf("\n**********  Command JI - Restarting hardware configuration  **********\n\n");

	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return wStatus;
}
//===========================================================================

/*** Revision History ***

01/18/17	KR/JO		CBF-133	Add cmdJI() functionality.
01/26/17	KR			CBF-141 Add GetOnTime() to cmdJI to get the restart time
 

$Log: /IcarusBased/SpecFW/cmdri.cpp $
 * 
 * 8     6/27/06 11:03a Nashth
 * Invoked IcarusReset from the windows path as per QPR: THNJ6R6KKU.
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
 * 5     7/20/05 4:36p Nashth
 * Added a two second delay cmd line request to IcarusReset to allow time
 * for the command reply to return for the RI command. 
 * 
 * 4     7/19/05 4:57p Nashth
 * Updated to launch IcarusReset.exe on an RI cmd.
 * 
 * 3     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 2     5/31/05 4:08p Frazzitl
 * Fixed problem with GE command and removed the BytesOut variable
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
