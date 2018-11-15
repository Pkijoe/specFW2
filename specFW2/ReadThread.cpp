//===========================================================================
//
//	Module Name:	ReadThread.cpp
//
// 	Function:		This read routine is used during diagnostics.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/ReadThread.cpp 10    1/13/06 1:38p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

/////////////////////////////////////////////////////////////////////////////
// readCtThread

UINT WINAPI CParserThread::readCtThread(LPVOID pParam)
{
	CParserThread *pThis = (CParserThread *) pParam;

	unsigned int	nStatus(NO_ERRORS);

	for ( ; ; )
	{
		pThis->cmdCM();
		pThis->m_nDelayTime	= 0;
		pThis->m_bFirstSum	= true;
		pThis->m_bLastSum	= true;
		pThis->m_bAverage	= false;
		pThis->m_bTransient	= false;
		pThis->m_nNoReads	= 1;

		nStatus = pThis->cmdRead();
		// This should not be required, but it allows other threads time
		Sleep(0);
		if (nStatus != NO_ERRORS)
		{
			return nStatus;
		}
	}
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /IcarusBased/SpecFW/ReadThread.cpp $
 * 
 * 10    1/13/06 1:38p Frazzitl
 * Removed dead code as recommended in a QPR
 * 
 * 9     9/14/05 8:01a Frazzitl
 * Removed commented areas from original code.
 * 
 * 8     9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 7     5/27/05 10:30 Frazzitl
 * Corrected problem with sum long and sum data
 * 
 * 6     5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 5     5/24/05 3:46p Frazzitl
 * Corrected exit from threads and added the elimination of the VIS status
 * thread if single channel instrument.
 * 
 * 4     5/06/05 15:53 Frazzitl
 * New implementation of threads
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
