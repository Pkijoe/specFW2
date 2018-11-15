//===========================================================================
//
//	Module Name:	cmdC1.cpp
//
// 	Function:		This routine sets the SerControl Cycles parameter.
//					This command has been changed to control the External Relay #2.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdc1.cpp 5     11/29/05 12:37p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdC1()
{
	WORD	status(NO_ERRORS);
	char	val;

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
	strcpy(m_nDataOutBuf, "C200");

	// get downloaded value
 	val	= *m_pCmdPtr++;
	if (val == '0')
	{
		m_nDigitalControl &= ~EXT1_ENABLE;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
	}
	else if (val == '1')
	{
		m_nDigitalControl |= EXT1_ENABLE;
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

$Log: /IcarusBased/SpecFW/cmdc1.cpp $
 * 
 * 5     11/29/05 12:37p Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 4     11/28/05 1:03p Frazzitl
 * Changed C1 & C2 commands to control the External relays
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:23 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:13 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
