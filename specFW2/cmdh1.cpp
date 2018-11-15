//===========================================================================
//
//	Module Name:	cmdH1.cpp
//
// 	Function:		This routine sets the Heater parameter.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdh1.cpp 5     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdH1()
{
	WORD	status(NO_ERRORS);
	long	return_long;
	long	temp_long;
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters

	strcpy(m_nDataOutBuf, "H100");

	// get downloaded value
	return_long = sscanf((char *) m_pCmdPtr, "%d", &temp_long);
	m_pCmdPtr += 2;	// bump past end
	m_nBytesRead += 2;
	if ((return_long < 0) || ((temp_long != 50) && (temp_long != 60)))
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		m_lLineFrequency = temp_long;
		m_NvRam.OutputW(LINE_FREQUENCY, (short) temp_long);
	}
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdh1.cpp $
 * 
 * 5     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 4     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
