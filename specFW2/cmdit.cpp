//===========================================================================
//
//	Module Name:	cmdIT.cpp
//
// 	Function:	  	The routine accepts the integration time value.
//		 			This routine also sets the "Clear C" flag.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdit.cpp 1     3/17/05 11:17 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdIT()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "IT00");

	m_nIntegrationTime = b2s();	// get value
	m_pCmdPtr++;				// ignore Clear C character
	m_nBytesRead++;
	m_bClearC = true;

	if ( m_nIntegrationTime > MAX_INT_TIME )
	{
		m_nIntegrationTime = 100;
		status	= ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdit.cpp $
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
