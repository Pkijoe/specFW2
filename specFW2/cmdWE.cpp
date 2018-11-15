//===========================================================================
//
//	Module Name:	cmdWE.cpp
//
// 	Function:	  	This routine enables/disables the Watchdog timer.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdWE.cpp 1     10/12/05 12:52p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdWE()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "WE00");

	m_cWatchdogEnable = (char) (*m_pCmdPtr++ - ASCII0);
	m_nBytesRead++;

	if (m_cWatchdogEnable != YES && m_cWatchdogEnable != NO)
	{
		m_cWatchdogEnable = YES;	// Keep enabled
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdWE.cpp $
 * 
 * 1     10/12/05 12:52p Frazzitl
 * Added ability to turn Watchdog timer On or Off - for testing only.

$NoKeywords: $

** End of Rev History **/
