//===========================================================================
//
//	Module Name:	cmdTM.cpp
//
// 	Function:  		This routine accepts the four parameters used by the
//		 			Transient Signal Mode command.  Once initialized,
// 					the routine executes the command.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdtm.cpp 25    1/04/06 9:31a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "CommandServer.h"

unsigned int CParserThread::cmdTM()
{
	unsigned int	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "TM00");

	if (!m_bCCDPower)
	{
		// no detector power
		status = CCD_POWER_OFF;
		memcpy(&m_nDataOutBuf[2], "DO", 2);
	}
	else
	{
		m_nLastBlockSent = 0;
		m_bAbortFlag = false;

		//TMdddddiiiiirrrrrfffffm where	ddddd = m_wTMdelay time
		//								iiiii = integration time
		//								rrrrr = number of reads
		//								fffff = frequency
		//								m     = m_cTMmode

		// Delay Time (0 - 60000)
		m_wTMdelay =  b2s();
		if (m_wTMdelay > 60000)						// 60000 msec max
		{
			status = ERR_PARA1;
			memcpy(&m_nDataOutBuf[2], "71", 2);
			return status;
		}

		// Integration Time (1 -> 60000)
		m_wTMitime = b2s();
		if (m_wTMitime < 1 || m_wTMitime > 60000)					// 60000 msec max
		{
			status = ERR_PARA2;
			memcpy(&m_nDataOutBuf[2], "72", 2);
			return status;
		}
		else
		{
			m_nIntegrationTime = m_wTMitime;
			m_bClearC = true;
		}
		
		// Number of Reads (2 -> 65535)
		m_nTransientReads = b2s();
		if (m_nTransientReads < 2 || m_nTransientReads > 65535)		// 65535 reads max
		{	// 65535 readings allowed because of Block number
			status = ERR_PARA3;
			memcpy(&m_nDataOutBuf[2], "73", 2);
			return status;
		}

		// Read Frequency (m_wTMitime -> 60000)
		m_wTMfreq = b2s();
		if (m_wTMfreq < 10 || m_wTMfreq > 60000)						// 60000 msec max
		{
			status = ERR_PARA4;
			memcpy(&m_nDataOutBuf[2], "74", 2);
			return status;
		}
		if (m_wTMfreq <= m_nIntegrationTime)
		{	// make m_wTMfreq > integration time
			m_wTMfreq = (WORD) (2 * m_nIntegrationTime);
			if (m_wTMfreq > 60000)
				m_wTMfreq = 60000;
		}

		//===================================================================
		// Read Mode (0 = Normal, 1 = Triggered)
		//===================================================================
		m_cTMmode = (char) (*m_pCmdPtr - ASCII0);
		m_pCmdPtr++;
		m_nBytesRead++;
		if (m_cTMmode < 0 || m_cTMmode > 1)
			m_cTMmode = 0;

		// Construct an array of pointers to each data block read
		m_paBlockNo.RemoveAll();
		delete [] m_pReadData;						// initialized later in moveData()
		m_bTransientOverflow = false;
		m_dwTTCounter = 0;						// Reset TT counter

		// Start TM thread that collects all thre data while allowing the TT command access
		DWORD	dwID;
		m_hTMThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) TMThread, this, CREATE_SUSPENDED, &dwID);
		if (m_hTMThread)
		{
			theApp.RegisterThread(L"Transient", dwID);
			::SetThreadPriority(m_hTMThread, THREAD_PRIORITY_NORMAL);
			::ResumeThread(m_hTMThread);
		}
		else
		{
			printf("TM Thread creation failed\r\n");
		}
	}
	return status;
}


UINT WINAPI CParserThread::TMThread(LPVOID pParam)
{
	CParserThread  *pThis = (CParserThread *) pParam;
	WORD			i;
	unsigned int	status(NO_ERRORS);
	DWORD			dwStartTime;
	DWORD			dwDeltaTime;

	pThis->m_bTransient = true;
	pThis->m_bFirstTransient = true;				// Set true so certain code executes only once

	if (pThis->m_cTMmode == 1)
	{	// Wait for External Start
		while ( pThis->m_IO.InputW(SVSTAT_CS) & TRIGGER_IN )
		{
			if (pThis->m_bAbortFlag)
			{
				pThis->m_bAbortFlag = false;
				status = ERR_ABORT_CMD;
				return status;						// Return doesn't mean anything - the TM00 already sent
			}
			//................................................................................................
			//if ( pThis->m_dwTTCounter > 2 )	//THN Fudge for a dummy read trigger ... Used for debugging
			//	break;						//  as if the trigger came in after the 3rd TT was received
			//								//  Comment these two lines out to remove
			//...............................................................................................
		}
	}
	if (pThis->m_wTMdelay != 0)
	{	// Setup Delay Time
		dwStartTime = GetTickCount();
		dwDeltaTime = pThis->m_wTMdelay;

		do
		{
			Sleep(0);								// Give it a rest
		} while (GetTickCount() - dwStartTime < dwDeltaTime);		// Wait until done
	}
	// Ready to start reading data

	// Get new Start Time that will anchor each individual read period
	dwStartTime = GetTickCount();

	for (i = 1; i <= pThis->m_nTransientReads; i++)
	{
		status = pThis->cmdRF();
		if (status != NO_ERRORS)
			return status;							// Return doesn't mean anything - the TM00 already sent

		if (i != pThis->m_nTransientReads)
		{
			dwDeltaTime = pThis->m_wTMfreq * i;
			// WAIT FOR SAMPLING INTERVAL
			do
			{
				Sleep(0);							// Give it a rest
			} while (GetTickCount() - dwStartTime < dwDeltaTime);	// Wait until done

		}
		if (pThis->m_bAbortFlag)
		{
			pThis->m_bAbortFlag = false;
			status = ERR_ABORT_CMD;
			return status;							// Return doesn't mean anything - the TM00 already sent
		}

	}
	return status;									// Return doesn't mean anything - the TM00 already sent
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdtm.cpp $
 * 
 * 25    1/04/06 9:31a Nashth
 * Removed code that circumvented waiting for a read trigger when
 * performing a transient read as per QPR THNJ6KQJT4.
 * 
 * 24    12/29/05 3:46p Nashth
 * Cleaned up some comments and commented code.
 * 
 * 23    12/29/05 11:18a Nashth
 * FIAS data collection is no longer read time limited as per QPR:
 * DAYS6GVP3X.
 * 
 * 22    12/27/05 1:14p Nashth
 * Comments added for possible future code modification.
 * Also, added special debug code to fake out a FIAS read trigger 
 * input after receipt of the third TT command.
 * 
 * 21    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 20    11/01/05 9:46a Frazzitl
 * Correct return from TM command thread which really has no meaning.  The
 * TM00 has already been sent, and the CC00 has been sent if the command
 * was cancelled.
 * 
 * 19    10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 18    10/31/05 10:44a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 17    10/12/05 10:44a Frazzitl
 * Corrected possible data overflow problem - QPR #TLFI6H4L7N
 * 
 * 16    10/06/05 11:21a Frazzitl
 * Removed Timer functions and replaced with GetTickCount() function to
 * handle Transient mode timing.
 * 
 * 15    10/05/05 2:04p Frazzitl
 * Fixed problem with monitoring timer for Transient command
 * 
 * 14    10/04/05 3:23p Frazzitl
 * Fixed timing for Transient Signal function.
 * 
 * 13    9/30/05 9:18a Frazzitl
 * Removed another warning
 * 
 * 12    9/26/05 11:38a Nashth
 * Changed Transient Mode command from a normal command to a thread
 * 
 * 11    9/16/05 2:23p Nashth
 * Fixing TT command
 * 
 * 10    9/14/05 3:27p Frazzitl
 * Fixed problem with executing certain code once per TM command
 * 
 * 9     9/14/05 8:01a Frazzitl
 * Removed commented areas from original code.
 * 
 * 8     9/14/05 7:21a Frazzitl
 * Remove unneeded problem command (memset).
 * 
 * 7     9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 6     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 5     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 4     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 3     4/12/05 10:36a Frazzitl
 * nothing important - still needs work
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
