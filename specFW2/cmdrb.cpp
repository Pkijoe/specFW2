//===========================================================================
//
//	Module Name:	cmdRB.cpp
//
//	Function:		This routine is used to reread Battery RAM.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdrb.cpp 2     6/29/05 15:43 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdRB()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "RB00");

	readBRAM();

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdrb.cpp $
 * 
 * 2     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
