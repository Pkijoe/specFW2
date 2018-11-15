//===========================================================================
//
//	Module Name:	cmdHR.cpp
//
// 	Function:	  	This routine sets the resolution of each detector.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdhr.cpp 2     4/19/05 3:23p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdHU()
{
	WORD	status(NO_ERRORS);
	char	cRes;

	cRes  = *m_pCmdPtr++;
	cRes -= ASCII0;
	m_nBytesRead++;

	strcpy(m_nDataOutBuf, "HU00");

	if (cRes == HIGH)
		m_cUVres = HIGH;			// Set UV High
	else if (cRes == LOW)
		m_cUVres = LOW;				// Set UV LOW
	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdHV()
{
	WORD	status(NO_ERRORS);
	char	cRes;

	cRes  = *m_pCmdPtr++;
	cRes -= ASCII0;
	m_nBytesRead++;

	strcpy(m_nDataOutBuf, "HV00");

	if (cRes == HIGH)
		m_cVISres = HIGH;			// Set VIS High
	else if (cRes == LOW)
		m_cVISres = LOW;			// Set VIS Low
	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdhr.cpp $
 * 
 * 2     4/19/05 3:23p Frazzitl
 * Fixed problems with Tub constants and UV & VIS resolution 
 * 
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
