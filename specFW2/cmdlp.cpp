//===========================================================================
//
//	Module Name:	cmdLP.cpp
//
// 	Function:	  	This routine turns the MERCURY lamp ON/OFF.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdlp.cpp 2     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdLP()
{
	WORD	status(NO_ERRORS);
	char	tmode;

	strcpy(m_nDataOutBuf, "LP00");

	tmode = (char) (*m_pCmdPtr++ - ASCII0);
	m_nBytesRead++;

	if (tmode == YES)
	{
		m_bLPmode	  = true;
		m_nDigitalControl |= HGLAMP_EN;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
	}
	else if (tmode == NO)
	{
		m_bLPmode	  = false;
		m_nDigitalControl &= ~HGLAMP_EN;
		m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
	}
	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdlp.cpp $
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
