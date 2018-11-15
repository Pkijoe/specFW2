//===========================================================================
//
//	Module Name:	CommandServerCommands.cpp
//
// 	Function:		This routine is used during diagnostics.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/CommandServerCmds.cpp 28    3/02/06 9:42a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"
#include "CommandServer.h"
#include "Debug.h"

void CParserThread::OnDoParseThis( WPARAM wParam, LPARAM lParam )
{
	m_pCmdPtr		= (char*) wParam;
	m_pCmdPtrSaved	= (char*) wParam;

	// Fixed QPR #SITR6BUQC3 by checking if pointer is not NULL before using
	if (m_pCmdPtr == NULL)
	{	// Cannot process a command if command pointer is NULL
		return;
	}

	WORD			nCmdPtrLen = (WORD) lParam;
	int				nLen;
	unsigned int	status(NO_ERRORS);
	m_nBytesRead = 0;


	*(m_pCmdPtr + nCmdPtrLen) = (char) END;

	if ((status = Parser()) == NO_ERRORS)
	{
		if (m_nCmd == CMD_EX)
		{	// We are done so exit
			// We're done - close communications thread
			m_ServingParent->ExitServerThread();
			return;
		}
		status = Decoder();
	}

	// since this was started from the server, pass him a message with my reply so he can respond.
	ASSERT(m_ServingParent);
	if (m_nCmd == CMD_TA)
	{	
		if (m_nDebug & DEBUG_COMMANDS)
		{	// Output the command response to the HyperTerminal or display window
			char	cBuffer[6];
			memcpy(&cBuffer[0], &m_nDataBuf[0], 4);
			cBuffer[4] = NULL;
			printf("Command OUT: %s\r\n", cBuffer);
		}
		nLen = m_lCurrentByte | BLOCKED_READ_MASK;
		m_ServingParent->CommandParsingDone(&m_nDataBuf[0], nLen);
	}
	else if (m_nCmd == CMD_TT)
	{
		if (m_nDebug & DEBUG_COMMANDS)
		{	// Output the command response to the HyperTerminal or display window
			char	cBuffer[6];
			memcpy(&cBuffer[0], &m_nDataTransient[0], 4);
			cBuffer[4] = NULL;
			printf("Command OUT: %s\r\n", cBuffer);
		}
		nLen = m_lTransientBytes | BLOCKED_READ_MASK;
		m_ServingParent->CommandParsingDone(&m_nDataTransient[0], nLen);
	}
	else if (m_nCmd == CMD_TM)
	{	// Just for testing and placing a break point - same as else
		nLen = strlen(&m_nDataOutBuf[0]);
		if (nLen >= 4)		// No response less than 4 characters
		{
			if (m_nDebug & DEBUG_COMMANDS)
			{	// Output the command response to the HyperTerminal or display window
				char	cBuffer[6];
				memcpy(&cBuffer[0], &m_nDataOutBuf[0], 4);
				cBuffer[4] = NULL;
				printf("Command OUT: %s\r\n", cBuffer);
			}
			m_nDataOutBuf[nLen] = 0;
			m_ServingParent->CommandParsingDone(&m_nDataOutBuf[0], nLen + 1);
		}
		else if (m_nDebug & DEBUG_COMMANDS)
		{	// Output the command response to the HyperTerminal or display window
			printf("Command OUT: ERROR\r\n");
		}
	}
	else if (m_nCmd == CMD_CC)
	{
//		// No response to a 'CC' command unless in Transient Signal mode (TM command)
//		if (m_bTransient == true)
//		{
//			nLen = strlen(&m_nDataOutBuf[0]);
//			if (nLen >= 4)		// No response less than 4 characters
//			{
//				m_nDataOutBuf[nLen] = 0;
//				m_ServingParent->CommandParsingDone(&m_nDataOutBuf[0], nLen + 1);
//			}
//		}
		// No response to a 'CC' command ever
		;
	}
	else
	{
		nLen = strlen(&m_nDataOutBuf[0]);
		if (nLen >= 4)		// No response less than 4 characters
		{
			if (m_nDebug & DEBUG_COMMANDS)
			{	// Output the command response to the HyperTerminal or display window
				char	cBuffer[6];
				memcpy(&cBuffer[0], &m_nDataOutBuf[0], 4);
				cBuffer[4] = NULL;
				printf("Command OUT: %s\r\n", cBuffer);
			}
			m_nDataOutBuf[nLen] = 0;
			m_ServingParent->CommandParsingDone(&m_nDataOutBuf[0], nLen + 1);
		}
		else if (m_nDebug & DEBUG_COMMANDS)
		{	// Output the command response to the HyperTerminal or display window
			printf("Command OUT: ERROR\r\n");
		}
	}
//	if (m_nCmd == CMD_RC)
//	{
//		m_pReadCtThread = AfxBeginThread(readCtThread, (LPVOID) this, THREAD_PRIORITY_NORMAL);
//	}
}
//===========================================================================

void CParserThread::OnDoParseThisInit( WPARAM wParam, LPARAM lParam )
{
	m_pCmdPtr		= (char*) wParam;
	m_pCmdPtrSaved	= (char*) wParam;

	// Fixed QPR #SITR6BUQC3 by checking if pointer is not NULL before using
	if (m_pCmdPtr == NULL)
	{	// Cannot process a command if command pointer is NULL
		return;
	}

	UINT			nCmdPtrLen = (UINT) lParam;
	int				nLen;
	unsigned int	status(NO_ERRORS);

	m_nBytesRead = 0;

	*(m_pCmdPtr + nCmdPtrLen) = (char) END;

	if ((status = ParserInit()) == NO_ERRORS)
	{
		if (m_nCmd == CMD_EX)
		{	// We are done so exit
			// We're done - close communications thread
			m_ServingParent->ExitServerThread();
			return;
		}
		status = Decoder();
	}

	// since this was started from the server, pass him a message with my reply so he can respond.
	ASSERT(m_ServingParent);
	if (m_nCmd == CMD_TA)
	{	
		if (m_nDebug & DEBUG_COMMANDS)
		{	// Output the command response to the HyperTerminal or display window
			char	cBuffer[6];
			memcpy(&cBuffer[0], &m_nDataBuf[0], 4);
			cBuffer[4] = NULL;
			printf("Command OUT: %s\r\n", cBuffer);
		}
		nLen = m_lCurrentByte | BLOCKED_READ_MASK;
		m_ServingParent->CommandParsingDone(&m_nDataBuf[0], nLen);
	}
	else
	{
		nLen = strlen(&m_nDataOutBuf[0]);
		if (nLen >= 4)		// No response less than 4 characters
		{
			if (m_nDebug & DEBUG_COMMANDS)
			{	// Output the command response to the HyperTerminal or display window
				char	cBuffer[6];
				memcpy(&cBuffer[0], &m_nDataOutBuf[0], 4);
				cBuffer[4] = NULL;
				printf("Command OUT: %s\r\n", cBuffer);
			}
			m_nDataOutBuf[nLen] = 0;
			m_ServingParent->CommandParsingDone(&m_nDataOutBuf[0], nLen + 1);
		}
	}
}
//===========================================================================

void CParserThread::OnDoParseThisFatal( WPARAM wParam, LPARAM lParam )
{
	m_pCmdPtr		= (char*) wParam;
	m_pCmdPtrSaved	= (char*) wParam;

	// Fixed QPR #SITR6BUQC3 by checking if pointer is not NULL before using
	if (m_pCmdPtr == NULL)
	{	// Cannot process a command if command pointer is NULL
		return;
	}

	UINT			nCmdPtrLen = (UINT) lParam;
	int				nLen;
	unsigned int	status(NO_ERRORS);
	
	m_nBytesRead = 0;

	*(m_pCmdPtr + nCmdPtrLen) = (char) END;

	if ((status = ParserFatal()) == NO_ERRORS)
	{
		if (m_nCmd == CMD_EX)
		{	// We are done so exit
			// We're done - close communications thread
			m_ServingParent->ExitServerThread();
			return;
		}
		status = Decoder();
	}

	// since this was started from the server, pass him a message with my reply so he can respond.
	ASSERT(m_ServingParent);
	if (m_nCmd == CMD_TA)
	{	
		if (m_nDebug & DEBUG_COMMANDS)
		{	// Output the command response to the HyperTerminal or display window
			char	cBuffer[6];
			memcpy(&cBuffer[0], &m_nDataBuf[0], 4);
			cBuffer[4] = NULL;
			printf("Command OUT: %s\r\n", cBuffer);
		}
		nLen = m_lCurrentByte | BLOCKED_READ_MASK;
		m_ServingParent->CommandParsingDone(&m_nDataBuf[0], nLen);
	}
	else
	{
		nLen = strlen(&m_nDataOutBuf[0]);
		if (nLen >= 4)		// No response less than 4 characters
		{
			if (m_nDebug & DEBUG_COMMANDS)
			{	// Output the command response to the HyperTerminal or display window
				char	cBuffer[6];
				memcpy(&cBuffer[0], &m_nDataOutBuf[0], 4);
				cBuffer[4] = NULL;
				printf("Command OUT: %s\r\n", cBuffer);
			}
			m_nDataOutBuf[nLen] = 0;
			m_ServingParent->CommandParsingDone(&m_nDataOutBuf[0], nLen + 1);
		}
	}
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /IcarusBased/SpecFW/CommandServerCmds.cpp $
 * 
 * 28    3/02/06 9:42a Frazzitl
 * Added debugging to the Init & Fatal functions also.
 * 
 * 27    3/02/06 9:20a Frazzitl
 * Added debugging to help find loss of communications problem.
 * 
 * 26    11/01/05 2:00p Frazzitl
 * Eliminated all responses to the CC command
 * 
 * 25    10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 24    10/31/05 10:44a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 23    9/26/05 10:51a Frazzitl
 * Removed code for handling Transient Mode command before making TM
 * command a thread.
 * 
 * 22    9/26/05 11:37a Nashth
 * Added changes required by changing Transient Mode command to a thread
 * 
 * 21    9/16/05 2:23p Nashth
 * Fixing TT command
 * 
 * 20    9/16/05 7:24a Frazzitl
 * Removed two commands that were only temporary
 * 
 * 19    9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 18    8/23/05 10:13 Frazzitl
 * Fixed 4 QPR's generated from the code review (SITR6BUQC3).
 * 
 * 17    7/01/05 11:39 Frazzitl
 * Removed Critical Section around entire command sequence.
 * 
 * 16    6/20/05 16:25 Frazzitl
 * Sends command replys from TA commands in Blocked mode.
 * 
 * 15    5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 14    5/26/05 9:12a Frazzitl
 * Added changes recommended by Remco to fix crashing problem
 * 
 * 13    5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 12    5/23/05 4:59p Frazzitl
 * Attempting to speed up command response by haing Status threads exit
 * quickly if a command is pending
 * 
 * 11    5/17/05 15:19 Frazzitl
 * Minor fix
 * 
 * 10    5/06/05 15:53 Frazzitl
 * New implementation of threads
 * 
 * 9     4/29/05 10:29 Frazzitl
 * Changing implementation of threads
 * 
 * 8     4/28/05 14:16 Frazzitl
 * Getting basic firmware to run
 * 
 * 7     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 6     4/13/05 13:15 Frazzitl
 * Miscellaneous changes primarily for status processing
 * 
 * 5     4/12/05 10:37a Frazzitl
 * Changed name of output buffer for data
 * 
 * 4     3/30/05 2:59p Frazzitl
 * Fixed some startup problems
 * 
 * 3     3/30/05 10:48 Frazzitl
 * Fixed problem with pointer
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
