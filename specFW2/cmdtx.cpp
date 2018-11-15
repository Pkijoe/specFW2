//===========================================================================
//
//	Module Name:	cmdTX.cpp
//
// 	Function:	  	This routine transmits sub_array data to the host computer.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdtx.cpp 16    12/29/05 3:46p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

#define BUFFER_EMPTY			0
#define BUFFER_NOT_EMPTY		1
#define BUFFER_EMPTY_NOTHING_TO_SEND   2

unsigned int CParserThread::cmdTA()
{
	WORD			status(NO_ERRORS);
	long			lBytes(0);

	strcpy(m_nDataBuf, "TA00xxxx");

	if (!m_bDataFlag)
	{
		status = ERR_DATA;
		memcpy(&m_nDataBuf[2], "04", 2);
		lBytes = REVDWORD(lBytes);			// could be zero if no data available
		memcpy(&m_nDataBuf[4], &lBytes, 4);
		return status;
	}

	lBytes = m_lBlockAddress[m_nDataBlock] - m_lBlockAddress[0];
	lBytes = REVDWORD(lBytes);			// could be zero if no data available
	memcpy(&m_nDataBuf[4], &lBytes, 4);
	// Data is actually place in string later

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdTB()
{
	WORD			status(NO_ERRORS);
	long			lBytes(0);
	long			m(8);
	unsigned long	n;

	strcpy(m_nDataOutBuf, "TB00xxxx");

	if (!m_bDataFlag)
	{
		status = ERR_DATA;
		memcpy(&m_nDataOutBuf[2], "04", 2);
		lBytes = REVDWORD(lBytes);			// could be zero if no data available
		memcpy(&m_nDataOutBuf[4], &lBytes, 4);
		return status;
	}

	m_nBlockOut = b2s();
	if (m_nBlockOut == 0 || m_nBlockOut >= m_nDataBlock)
	{
		status = ERR_BLK;
		memcpy(&m_nDataOutBuf[2], "05", 2);
		lBytes = REVDWORD(lBytes);			// could be zero if no data available
		memcpy(&m_nDataOutBuf[4], &lBytes, 4);
		return status;
	}
	else
	{
		lBytes = m_lBlockAddress[m_nBlockOut] - m_lBlockAddress[m_nBlockOut - 1];
		lBytes = REVDWORD(lBytes);		// could be zero if no data available
		memcpy(&m_nDataOutBuf[4], &lBytes, 4);

		for (n = m_lBlockAddress[m_nBlockOut - 1]; n < m_lBlockAddress[m_nBlockOut]; n++)
		{
			m_nDataOutBuf[m++] = (char) m_nDataBuf[n];
		}
	}

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdTT()
{
	WORD	status(NO_ERRORS);
	long	numBytes(0);
	WORD	blocksToSend;
	WORD	localCurrentBlock;
	WORD	localLastBlock;

	strcpy(m_nDataTransient, "TT00xxxx");

	localCurrentBlock = m_nDataBlock;	// maintain a local copy of these, because there's always a chance that 
	localLastBlock = m_nLastBlockSent;	// the interrups scribbles while we're sending..
	m_dwTTCounter++;					// Increment the TT counter

	if (localCurrentBlock == localLastBlock)
	{
		status = ERR_DATA;				// signal "no data"
		memcpy(&m_nDataTransient[2], "04", 2);
		numBytes = REVDWORD(numBytes);	// zero if no data available
		memcpy(&m_nDataTransient[4], &numBytes, 4);
		m_lTransientBytes = 8;
		return status;
	}
	// Check for overflow - if so, set flag to stop TM command
	if (localCurrentBlock - localLastBlock > m_nFullBlocks)
	{
		status = ERR_OVERFLOW;			// signal "overflow"
		memcpy(&m_nDataTransient[2], "23", 2);
		numBytes = REVDWORD(numBytes);	// zero if bad data
		memcpy(&m_nDataTransient[4], &numBytes, 4);
		m_lTransientBytes = 8;
		m_bTransientOverflow = true;
		return status;
	}

	// send all blocks we've collected thus far.
	numBytes = (localCurrentBlock - localLastBlock) * m_lBlockSize;

	if (numBytes + 8 > MAX_OUT_BYTES)
	{	// More data than buffer can handle - calculate max number of blocks that fit
		blocksToSend = (WORD) (( MAX_OUT_BYTES - 8) / m_lBlockSize);
		numBytes = blocksToSend * m_lBlockSize;
	}
	else
	{	// All blocks will fit
		blocksToSend = (WORD) (localCurrentBlock - localLastBlock);
	}

	m_lTransientBytes = numBytes + 8;

	BYTE *	pOut  = (BYTE *) &m_nDataTransient[8];
	// Note: m_paBlockNo contains the starting address of each data block
	// The following is needed because of the use of a circular buffer of full data blocks
	// The 'next' data block could be back at the start of the buffer
	for (WORD nBlock = 1; nBlock <= blocksToSend; nBlock++)
	{	// First get the location of the data block to transfer
		//...........................................................................................
		BYTE *	pData = (BYTE *) m_paBlockNo.GetAt(0);  // Always take from the first position...
		m_paBlockNo.RemoveAt(0);                         // and then delete now that we have the ptr
		//...........................................................................................
		// Now transfer all bytes in that data block
		for (long lByte = 0; lByte < m_lBlockSize; lByte++)
			*pOut++ = *pData++;
	}

	numBytes = REVDWORD(numBytes);
	memcpy(&m_nDataTransient[4], &numBytes, 4);

	m_nLastBlockSent = (WORD) (localLastBlock + blocksToSend);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdtx.cpp $
 * 
 * 16    12/29/05 3:46p Nashth
 * Cleaned up some comments and commented code.
 * 
 * 15    12/29/05 11:18a Nashth
 * FIAS data collection is no longer read time limited as per QPR:
 * DAYS6GVP3X.
 * 
 * 14    12/27/05 1:14p Nashth
 * Comments added for possible future code modification.
 * Also, added special debug code to fake out a FIAS read trigger 
 * input after receipt of the third TT command.
 * 
 * 13    10/12/05 10:44a Frazzitl
 * Corrected possible data overflow problem - QPR #TLFI6H4L7N
 * 
 * 12    10/04/05 10:12a Nashth
 * Fixed problem with Transient Signal mode
 * 
 * 11    9/26/05 11:40a Nashth
 * Changed normal Output buffer to a new Transient buffer to eliminate
 * problems caused by other commands received during a TM command
 * 
 * 10    9/09/05 7:39a Frazzitl
 * Removed unneeded variables
 * 
 * 9     9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 8     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 7     6/17/05 9:20 Frazzitl
 * Fixed problem with transmitting a block of data
 * 
 * 6     6/06/05 3:36p Frazzitl
 * Removed warnings during build
 * 
 * 5     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 4     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 3     4/13/05 13:15 Frazzitl
 * Miscellaneous changes primarily for status processing
 * 
 * 2     4/12/05 10:37a Frazzitl
 * Changed name of output buffer for data
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
