//===========================================================================
//
//	Module Name:	cmdRC.cpp
//
// 	Function:	  	This routine performs a continuous read.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdrc.cpp 5     9/09/05 7:36a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"
#include "CommandServer.h"

unsigned int CParserThread::cmdRC()
{
	unsigned int	status(NO_ERRORS);
	WORD			length;

	strcpy(m_nDataOutBuf, "RC00");

	if (!m_bCCDPower)
	{	// no detector power
		status = CCD_POWER_OFF;
		memcpy(&m_nDataOutBuf[2], "DO", 2);
	}
	else
	{
		status	= cmdCM();			// Clear Memory before READ
		m_nNoReads		= 1;
		m_nDelayTime	= 0;
		m_bFirstSum		= true;
		m_bLastSum		= false;
		m_bAverage		= false;
		m_bTransient	= false;
		m_bCancelFlag	= false;
		if (!m_bNewDataFormatFlag)
			status	= cmdRO();
		else
			status	= cmdRO();

		if (status == NO_ERRORS)
		{
			// ECHO BACK COMMAND RESPONSE
			m_nDataOutBuf[4] = 0;
			m_ServingParent->CommandParsingDone(&m_nDataOutBuf[0], 5);
		}
		else
		{
			memcpy(&m_nDataOutBuf[2], "12", 2);	// Detector Read error
			return status;
		}

		for ( ; ; )
		{
// TLF - Fix this
			length = 0;
			if (length > 0)
			{
				if (m_nCmd == CMD_SR)
				{
					// STOP READ ON SYSTEM RESET
					strcpy(m_nDataOutBuf, "SR00");
					return status;
				}
				// ANY OTHER COMMAND ILLEGAL DURING READ CONTINUOUS
				strcpy(m_nDataOutBuf, "XX01");
				m_nCmd	= CMD_RC;
			}
			m_nDataBlock= 1;
			status	= cmdRO();
			if (status != NO_ERRORS)
			{
				strcpy(m_nDataOutBuf, "RC12");
				return status;
			}
		}
	}
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdrc.cpp $
 * 
 * 5     9/09/05 7:36a Frazzitl
 * Added required include file
 * 
 * 4     9/09/05 7:32a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/12/05 10:33a Frazzitl
 * rename a variable
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
