//===========================================================================
//
//	Module Name:	cmdSC.cpp
//
// 	Function:	  	This routine sets the code of "SC" in BATTERY RAM
//		 			allowing SINGLE CHANNEL operation.  Also can remove it.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdsc.cpp 11    1/13/06 1:37p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSC()
{
	WORD	status(NO_ERRORS);
	char	mode;

	theApp.EnterCriticalSection1(&m_CriticalSection);

	strcpy(m_nDataOutBuf, "SC00");

	mode = (char) (*m_pCmdPtr++ - ASCII0);
	m_nBytesRead++;

	if (mode == YES)
	{
		m_NvRam.OutputB(SINGLE_CHANNEL_1, 'S');
		m_NvRam.OutputB(SINGLE_CHANNEL_2, 'C');
		m_bSingleChannel = true;
	}
	else if (mode == NO)
	{
		m_NvRam.OutputB(SINGLE_CHANNEL_1, 'D');
		m_NvRam.OutputB(SINGLE_CHANNEL_2, 'C');
		m_bSingleChannel = false;
	}
	else
	{
		status	 = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdsc.cpp $
 * 
 * 11    1/13/06 1:37p Frazzitl
 * Removed dead code as recommended in a QPR
 * 
 * 10    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 9     8/02/05 2:42p Nashth
 * VIS status thread enabled and controlled via cmdsc. for QPR THNJ6EVKJC.
 * 
 * 8     7/01/05 15:37 Frazzitl
 * Fixed the Tub heater problem
 * 
 * 7     6/29/05 13:59 Frazzitl
 * Possible problem removed
 * 
 * 5     5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 4     5/24/05 3:46p Frazzitl
 * Corrected exit from threads and added the elimination of the VIS status
 * thread if single channel instrument.
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:19 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
