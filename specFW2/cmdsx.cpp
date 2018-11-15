//===========================================================================
//
//	Module Name:	cmdSX.cpp
//
// 	Function:	  	This routine transmits sub_array data size information to the host computer.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdsx.cpp 9     9/09/05 7:33a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSA()
{
	WORD	status(NO_ERRORS);
	long	lBytes(0);
//	unsigned short	 words;
//	unsigned long	 bwords;

	strcpy(m_nDataOutBuf, "SA00xxxxxxxx");

	if (!m_bDataFlag)
	{
		status = ERR_DATA;
		memcpy(&m_nDataOutBuf[2], "04", 2);
		return status;
	}

//	bwords	= 2 + ((m_lBlockAddress[m_nDataBlock] - m_lBlockAddress[0]) / 2);
	lBytes = (m_lCurrentByte + 4) / 2;	// because we want number of words
	l2b(lBytes, 4);
//	s2b(lBytes, 4);

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdSB()
{
	WORD	status(NO_ERRORS);
	long	lBytes(0);
	unsigned short	 words;

	if (!m_bDataFlag)
	{
		status = ERR_DATA;
		strcpy(m_nDataOutBuf, "SB04");
		return status;
	}

	m_nBlockOut = b2s();				// Number of Block to send
	{
		words  	= (unsigned short) (2 + ((m_lBlockAddress[m_nBlockOut] - m_lBlockAddress[m_nBlockOut - 1]) / 2));
	}
	if (m_nBlockOut == 0 || m_nBlockOut >= m_nDataBlock)
	{
		status	= ERR_BLK;
		strcpy(m_nDataOutBuf, "SB05");
		return status;
	}
	else
	{
		strcpy(m_nDataOutBuf, "SB00xxxxx");
		lBytes = m_lBlockAddress[m_nBlockOut] - m_lBlockAddress[m_nBlockOut - 1];
		lBytes = (lBytes + 4) / 2;			// because we want number of words
		s2b((WORD) lBytes, 4);
	}

	return status;
}
//===========================================================================

//////////////////////////////
// Query what block is ready 
// added RDW
unsigned int CParserThread::cmdCB()
{
	return NO_ERRORS;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdsx.cpp $
 * 
 * 9     9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 8     5/31/05 4:08p Frazzitl
 * Fixed problem with GE command and removed the BytesOut variable
 * 
 * 7     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 6     5/24/05 16:20 Frazzitl
 * SA should return 8 bytes, not 5
 * 
 * 5     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 4     4/22/05 15:18 Frazzitl
 * Improved Status reads
 * 
 * 3     4/12/05 10:35a Frazzitl
 * Fixed Size All output problem
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
