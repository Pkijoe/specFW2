//===========================================================================
//
//	Module Name:	cmdRD.cpp
//
// 	Function:	  	This routine is a special version of cmdRO.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdrd.cpp 12    7/09/07 10:56a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdRD()
{
	unsigned int	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "RD00");

	m_nDelayTime = b2s();

	if (!m_bCCDPower)
	{	// no detector power
		status = CCD_POWER_OFF;
		memcpy(&m_nDataOutBuf[2], "DO", 2);
	}
	if (m_nDelayTime > MAX_DELAY_TIME)
	{
		m_nDelayTime = 0;
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		m_nNoReads		= 1;
		m_bFirstSum		= true;
		m_bLastSum		= true;
		m_bAverage		= false;
		m_bTransient	= false;
		m_bCancelFlag	= false;

		status = cmdRead(m_nDelayTime);
		if (status != NO_ERRORS)
		{
			WORD	 wRevStatus = (WORD) REVWORD(status);
			status = wRevStatus;
			memcpy(&m_nDataOutBuf[2], &status, 2);
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
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdrd.cpp $
 * 
 * 12    7/09/07 10:56a Nashth
 * Updated return codes from functions that call cmdRead.
 * 
 * 11    7/06/07 4:51p Nashth
 * Added an information error string when a detector read error occurs as
 * per QPR: THNJ74CQCW.
 * 
 * 10    5/22/07 8:27a Frazzitl
 * Added additional errors
 * 
 * 9     5/21/07 10:48a Frazzitl
 * Added error checking even with good reads.
 * 
 * 8     5/21/07 8:23a Frazzitl
 * Added new errors to better describe Read Errors
 * 
 * 7     4/21/06 1:10p Frazzitl
 * Added feature to check for and generate an error if either some or all
 * pixels are not received from the detector for a given subarray.
 * 
 * 6     10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 5     10/31/05 10:43a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 4     9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 3     5/17/05 15:23 Frazzitl
 * Added check for cancel
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
