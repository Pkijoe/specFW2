//===========================================================================
//
//	Module Name:	cmdMD.cpp
//
// 	Function:	  	This routine selects the mode of operation:
//					SETUP-0,NOT_ON-1,COLD-2,HOT-A,READY-R,DIAG-X,AUTO-Z
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdMD()
{
	WORD	status(NO_ERRORS);
	char	cMode;

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
	strcpy(m_nDataOutBuf, "MD00");

	cMode = *m_pCmdPtr++;
	m_nBytesRead++;

	if (m_cStartupMode > NOT_ON && m_cStartupMode < READY)
	{
		status = ERR_CMD;
		memcpy(&m_nDataOutBuf[2], "01", 2);
	}
	else
	{
		switch (cMode)
		{
//			case SETUP:
//			case AUTO:
			case DIAG:
				m_cOperationMode = cMode;
				m_cStartupMode = cMode;
				break;
			case MFG:
				//...........................................................
				// Mfg mode entry placed into the Spectrometer log.
				{
					char* szStr = new char[40];
					GetDateTimeStamp(szStr);
					sprintf(&szStr[19], " Mode: Mfg (DCM)\0");
					AddToSpecLog(szStr);
				}
				//...........................................................
				m_cOperationMode = cMode;
				m_cStartupMode = cMode;
				break;
			case READY:
				//...........................................................
				// Ready mode entry placed into the Spectrometer log.
				{
					char* szStr = new char[40];
					GetDateTimeStamp(szStr);
					sprintf(&szStr[19], " Mode: Ready (DCM)\0");
					AddToSpecLog(szStr);
				}
				//...........................................................
				m_cOperationMode = cMode;
				m_cStartupMode = cMode;
				break;
			case COLD:
				//...........................................................
				// Cold mode entry placed into the Spectrometer log.
				{
					char* szStr = new char[40];
					GetDateTimeStamp(szStr);
					sprintf(&szStr[19], " Mode: Cold (DCM)\0");
					AddToSpecLog(szStr);
				}
				//...........................................................
				m_cInitFlag = YES;	/* Intialization to start */
				if ( m_bCCDPower )	/* DETECTOR POWER OFF IF ON */
				{
					m_bCCDPower  = false;
				}

				m_cOperationMode = READY;
				m_cStartupMode = cMode;
				break;
			case HOT:
				//...........................................................
				// Hot mode entry placed into the Spectrometer log.
				{
					char* szStr = new char[40];
					GetDateTimeStamp(szStr);
					sprintf(&szStr[19], " Mode: Hot (DCM)\0");
					AddToSpecLog(szStr);
				}
				//...........................................................
				m_cInitFlag = YES;	/* Intialization to start */
				if ( m_bCCDPower )	/* DETECTOR POWER OFF IF ON */
				{
					m_bCCDPower  = false;
				}

				m_cOperationMode = READY;
				m_cStartupMode = cMode;
				break;
			default:
				status = ERR_PARA;
				memcpy(&m_nDataOutBuf[2], "07", 2);
				break;
		}
	}

	if (status == NO_ERRORS)
	{
		// Just in case system is in Sleep mode - disable checking
		m_bSleepFlag = false;

		m_nWarning	= 0;				// Clear ALL Warnings
		m_nFatal	= 0;				// Clear ALL Fatal messages
		if (m_cOperationMode == AUTO)
		{
			m_nOPmode		= TEST;		// TEST MODE
			m_nTestMode		= NORMAL;	// TEST MODE, NORMAL
			m_bTestMode		= false;
			m_bOverscanMode	= false;
		}
		else
		{
			m_nOPmode		= NORMAL;	// NORMAL MODE, NOT TESTING
			m_nTestMode		= NORMAL;	// NORMAL
			m_bTestMode		= false;
			m_bOverscanMode	= false;
		}
	}
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***
01/28/17	KR	  CBF-143 - Remove unwanted thermal commands

$Log: /IcarusBased/SpecFW/cmdmd.cpp $
 * 
 * 8     4/07/08 5:09p Nashth
 * Added Mfg mode to History log.
 * 
 * 7     4/07/08 10:41a Nashth
 * Second attempt at Spectrometer history log.
 * 
 * 6     3/31/08 5:31p Nashth
 * Implementation of spectrometer history log... First pass.
 * 
 * 5     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 4     9/30/05 9:23a Frazzitl
 * Changed variable names to make more readable
 * 
 * 3     8/18/05 13:49 Frazzitl
 * Implemented Sleep mode.  Time is checked in CommandServer if enabled.
 * At correct time, the system must be restarted - not yet implemented.
 * Disabled if any legal Mode Command is accepted.
 * 
 * 2     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
