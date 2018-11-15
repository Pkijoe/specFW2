//===========================================================================
//
//	Module Name:	cmdCX.cpp
//
//	Function:		The routine performs four functions:
//					1. Clears the block pointer array.
//					2. Resets the data pointer, m_nDataOutBuf.
//					3. Resets the Data Block Number, m_nDataBlock.
//					4. Clears the data storage area.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdcx.cpp 10    11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

//////////////
// clear memory
unsigned int CParserThread::cmdCM()
{
	WORD	status(NO_ERRORS);
	unsigned long	*ptr;
	unsigned long	 blk;
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters

	strcpy(m_nDataOutBuf, "CM00");

	if (m_nDataBlock > 0)					// CLEAR RAM SPACE & BLOCK NUMBERS USED
	{
		if (m_lCurrentByte < 8)
			m_lCurrentByte = 8;
		for (ptr = (unsigned long *) &m_nDataBuf[0]; ptr <= (unsigned long *) &m_nDataBuf[m_lCurrentByte]; ptr++)
			*ptr = 0;

		for (blk = 1; blk < m_nDataBlock; blk++)
			m_lBlockAddress[blk] = 0;
	}
	else if (m_nDataBlock == 0)				// CLEAR ALL RAM SPACE & BLOCK NUMBERS
	{
		for (ptr = (unsigned long *) &m_nDataBuf[0]; ptr <= (unsigned long *) (&m_nDataBuf[MAX_DATA_BYTES - 1]); ptr++)
			*ptr = 0;

		for (blk = 1; blk < MAX_BLOCKS; blk++)
			m_lBlockAddress[blk] = 0;
	}

	m_lBlockAddress[0] = 8;
	m_nDataBlock    = 0;
	m_bDataFlag     = false;
	m_lCurrentByte	= 8;					// Starting location of m_nDataOutBuf

	theApp.LeaveCriticalSection1(&m_CriticalSection);  // Release critical paarameters
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdcx.cpp $
 * 
 * 10    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 9     9/09/05 7:32a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 8     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 7     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 6     6/17/05 10:04a Frazzitl
 * 
 * 5     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 4     5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 3     5/25/05 9:06 Frazzitl
 * Removed unused code that was causing a problem
 * 
 * 2     4/12/05 10:42a Frazzitl
 * nothing important
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
