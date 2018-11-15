//===========================================================================
//
//	Module Name:	cmdTC.cpp
//
//	Function:		This routine returns the current Detector Chip Type:
//					ZODIAC ( "0" ) or VEGA ( "1" ).
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdtc.cpp 4     6/29/05 15:43 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdTC()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "TC00x");			// 1 character string

	m_nDataOutBuf[4] = m_cDetectorType;

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdtc.cpp $
 * 
 * 4     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 3     4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
