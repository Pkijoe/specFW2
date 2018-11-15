//===========================================================================
//
//	Module Name:	cmdCH.cpp
//
// 	Function:		This routine sets the chiller on or off.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdch.cpp 4     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdCH()
{
	WORD	status(NO_ERRORS);
	char	val;

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
	strcpy(m_nDataOutBuf, "CH00");

	// get downloaded value
 	val	= *m_pCmdPtr++;
	if (val == '0')
	{
		m_nDigitalControl &= ~CHILLER_ENABLED;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
	}
	else if (val == '1')
	{
		m_nDigitalControl |= CHILLER_ENABLED;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
	}
	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	m_nBytesRead += 1;
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdch.cpp $
 * 
 * 4     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 3     11/28/05 1:04p Frazzitl
 * Correct problem with not incrementing the number of bytes read
 * 
 * 2     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
