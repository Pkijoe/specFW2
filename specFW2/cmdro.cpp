//===========================================================================
//
//	Module Name:	cmdRO.cpp
//
// 	Function:	  	This routine is an assembler version of cmdRO.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdro.cpp 15    7/09/07 10:56a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdRO()
{
	unsigned int	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "RO00");

	if (!m_bCCDPower)
	{	// no detector power
		status = CCD_POWER_OFF;
		memcpy(&m_nDataOutBuf[2], "DO", 2);
	}
	else
	{
		m_nNoReads		= 1;
		m_nDelayTime	= 0;
		m_bFirstSum		= true;
		m_bLastSum		= true;
		m_bAverage		= false;
		m_bTransient	= false;
		m_bCancelFlag	= false;

		//printf("THN cmdRO()... cmdRead()\r\n");
		status = cmdRead();
		if (status != NO_ERRORS)
		{
			WORD	 wRevStatus = (WORD) REVWORD(status);
			status = wRevStatus;
			memcpy(&m_nDataOutBuf[2], &status, 2);
		}
		else
		{
			//printf("THN cmdRO()... cmdRead()... Post call (No Error)\r\n");
			if (m_bCancelFlag == true)
				memcpy(&m_nDataOutBuf[2], "20", 2);	// Read cancelled
		}
	}
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdro.cpp $
 * 
 * 15    7/09/07 10:56a Nashth
 * Updated return codes from functions that call cmdRead.
 * 
 * 14    7/06/07 4:51p Nashth
 * Added an information error string when a detector read error occurs as
 * per QPR: THNJ74CQCW.
 * 
 * 13    5/22/07 8:27a Frazzitl
 * Added additional errors
 * 
 * 12    5/21/07 10:48a Frazzitl
 * Added error checking even with good reads.
 * 
 * 11    5/21/07 8:23a Frazzitl
 * Added new errors to better describe Read Errors
 * 
 * 10    4/21/06 1:10p Frazzitl
 * Added feature to check for and generate an error if either some or all
 * pixels are not received from the detector for a given subarray.
 * 
 * 9     11/29/05 5:13p Nashth
 * Commented out some printfs used for debug.
 * 
 * 8     11/22/05 10:10a Nashth
 * Added some Printf statements for debugging... Some with THN within the
 * Print Text... These can eventually be removed or made debug flag
 * optional.
 * 
 * 7     10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 6     10/31/05 10:44a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 5     9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 4     5/17/05 15:23 Frazzitl
 * Added check for cancel
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/12/05 10:34a Frazzitl
 * Fixed problem with Read Once
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
