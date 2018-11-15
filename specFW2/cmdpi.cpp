//===========================================================================
//
//	Module Name:	cmdPI.cpp
//
//	Function:		This routine is used to inform the firmware if the
//					plasma is off (0) or on (1).
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdpi.cpp 2     6/29/05 15:43 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdPI()
{
	WORD	status(NO_ERRORS);
	char	val;

	strcpy(m_nDataOutBuf, "PI00");

 	val	= *m_pCmdPtr++;
	m_nBytesRead++;
	if (val == '0' || val == '1')
	{
		m_nPlasma_lit	= (short) (val - ASCII0);
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

$Log: /IcarusBased/SpecFW/cmdpi.cpp $
 * 
 * 2     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
