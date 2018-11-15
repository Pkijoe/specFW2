//===========================================================================
//
//	Module Name:	cmdSL.cpp
//
// 	Function:  		The routine accepts the number of reads to sum.
// 					It than calls READ SUM routine to sum data.
//					Unlike cmdSD, it leaves the data in a LONG format,
//					thereby requiring a special application routine to
//					read the data.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdsl.cpp 14    7/09/07 10:56a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSL()
{
	unsigned int	status(NO_ERRORS);
	WORD			i;

	strcpy(m_nDataOutBuf, "SL00");

	m_nNoReads = b2s();

	if (!m_bCCDPower)
	{	// no detector power
		status = CCD_POWER_OFF;
		memcpy(&m_nDataOutBuf[2], "DO", 2);
	}
	else if (m_nNoReads == 0 || m_nNoReads > MAX_READS)
	{
		status	 = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		m_nDelayTime	= 0;
		m_bFirstSum		= true;
		m_bLastSum		= false;
		m_bAverage		= false;
		m_bTransient	= false;
		m_bCancelFlag	= false;

		for (i = 0; i < m_nNoReads; i++)
		{
			if (i == m_nNoReads - 1)
				m_bLastSum = true;
			status = cmdRead();
			// This should not be required, but it allows other threads time
			Sleep(0);
			if (status != NO_ERRORS)
			{
				WORD	 wRevStatus = (WORD) REVWORD(status);
				status = wRevStatus;
				memcpy(&m_nDataOutBuf[2], &status, 2);
				return status;
			}
			else
			{
				if (m_bCancelFlag == true)
				{
					memcpy(&m_nDataOutBuf[2], "20", 2);	// Read cancelled
					return status;
				}
			}
		}
	}
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdsl.cpp $
 * 
 * 14    7/09/07 10:56a Nashth
 * Updated return codes from functions that call cmdRead.
 * 
 * 13    7/06/07 4:51p Nashth
 * Added an information error string when a detector read error occurs as
 * per QPR: THNJ74CQCW.
 * 
 * 12    5/22/07 8:27a Frazzitl
 * Added additional errors
 * 
 * 11    5/21/07 10:48a Frazzitl
 * Added error checking even with good reads.
 * 
 * 10    5/21/07 8:24a Frazzitl
 * Added new errors to better describe Read Errors
 * 
 * 9     4/21/06 1:10p Frazzitl
 * Added feature to check for and generate an error if either some or all
 * pixels are not received from the detector for a given subarray.
 * 
 * 8     10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 7     10/31/05 10:44a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 6     9/14/05 8:01a Frazzitl
 * Removed commented areas from original code.
 * 
 * 5     9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 4     5/27/05 10:30 Frazzitl
 * Corrected problem with sum long and sum data
 * 
 * 3     5/17/05 15:23 Frazzitl
 * Added check for cancel
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
