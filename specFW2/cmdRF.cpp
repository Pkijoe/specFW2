//===========================================================================
//
//	Module Name:	cmdRF.cpp
//
// 	Function:	  	This is a version of cmdRO for FIAS (Transient Signal)
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdRF.cpp 10    7/09/07 10:56a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdRF()
{
	unsigned int	status(NO_ERRORS);

// This could cause a problem so don't allow
//	strcpy(m_nDataOutBuf, "RF00");

	if (!m_bCCDPower)
	{	// no detector power
		status = CCD_POWER_OFF;
// This could cause a problem so don't allow
//		memcpy(&m_nDataOutBuf[2], "DO", 2);
	}
	else if (m_bTransientOverflow == true)
	{	// read overflow
		status = ERR_OVERFLOW;
// This could cause a problem so don't allow
//		memcpy(&m_nDataOutBuf[2], "23", 2);
	}
	else
	{
		m_nNoReads		= 1;
		m_nDelayTime	= 0;
		m_bFirstSum		= true;
		m_bLastSum		= true;
		m_bAverage		= false;
		m_bTransient	= true;
		m_bCancelFlag	= false;

		status = cmdRead();
		if (status != NO_ERRORS)
		{
			WORD	 wRevStatus = (WORD) REVWORD(status);
			status = wRevStatus;
			memcpy(&m_nDataOutBuf[2], &status, 2);
		}
	}
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdRF.cpp $
 * 
 * 10    7/09/07 10:56a Nashth
 * Updated return codes from functions that call cmdRead.
 * 
 * 9     5/22/07 8:27a Frazzitl
 * Added additional errors
 * 
 * 8     5/21/07 10:48a Frazzitl
 * Added error checking even with good reads.
 * 
 * 7     5/21/07 8:23a Frazzitl
 * Added new errors to better describe Read Errors
 * 
 * 6     10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 5     10/31/05 10:44a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 4     10/12/05 10:44a Frazzitl
 * Corrected possible data overflow problem - QPR #TLFI6H4L7N
 * 
 * 3     9/16/05 2:23p Nashth
 * Fixing TT command
 * 
 * 2     9/14/05 3:27p Frazzitl
 * Fixed problem with executing certain code once per TM command
 * 
 * 1     9/09/05 7:40a Frazzitl
 * File used specifically for Transient Signal (FIAS) reads

$NoKeywords: $

** End of Rev History **/
