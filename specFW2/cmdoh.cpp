//===========================================================================
//
//	Module Name:	cmdOH.cpp
//
// 	Function:		This routine sets the Optics Counter Hours.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdoh.cpp 3     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdOH()
{
	WORD			status(NO_ERRORS);
	unsigned long   temp_long;

temp_long = 4;

	strcpy(m_nDataOutBuf, "OH00");

	// get downloaded value
	sscanf((char *) m_pCmdPtr, "%5d", &temp_long);
	m_pCmdPtr += 5;
	m_nBytesRead += 5;
	if ((temp_long * 60) > OPTICS_HOURS_MAX)
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		m_lOptics_hours = temp_long * 60;
		m_NvRam.OutputL(OPTICS_HOURS, temp_long * 60);
	}
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdoh.cpp $
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
