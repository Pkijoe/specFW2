//===========================================================================
//
//	Module Name:	cmdD2.cpp
//
// 	Function:		This routine sets the Hardware version parameter.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdd2.cpp 3     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdD2()
{
	WORD	status(NO_ERRORS);
	long	return_long;
	short	temp_short;

	strcpy(m_nDataOutBuf, "D200");

temp_short = 4;

	// get downloaded value
	return_long = sscanf((char *) m_pCmdPtr, "%hx", &temp_short);
	m_pCmdPtr += 4;
	m_nBytesRead += 4;

	m_nHardware_version = temp_short;
	m_NvRam.OutputW(HARDWARE_VER, temp_short);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdd2.cpp $
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
