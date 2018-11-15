//===========================================================================
//
//	Module Name:	cmdSE.cpp
//
// 	Function:	  	This routine turns the SERVO ENABLE Bit ON/OFF.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdse.cpp 2     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSE()
{
	WORD	status(NO_ERRORS);
	char	smode;

	strcpy(m_nDataOutBuf, "SE00");

	smode	= (char) (*m_pCmdPtr++ - ASCII0);
	m_nBytesRead++;

	if (smode == YES)
	{
		m_bSEmode	 = true;
	}
	else if (smode == NO)
	{
		m_bSEmode	 = false;
	}
	else
	{
		status	 = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdse.cpp $
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:19 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
