//===========================================================================
//
//	Module Name:	cmd1H.cpp
//
// 	Function:		This routine gets the Line Frequency parameter.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmd1h.cpp 2     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd1H()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "1H00xx");		// 2 character string

	m_nDataOutBuf[4] = (char) ((m_lLineFrequency / 10) + ASCII0);
	m_nDataOutBuf[5] = (char) ((m_lLineFrequency % 10) + ASCII0);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmd1h.cpp $
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:12 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
