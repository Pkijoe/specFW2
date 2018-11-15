//===========================================================================
//
//	Module Name:	cmdI1.cpp
//
//	Function:		The routine performs initialize one call
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdi1.cpp 1     3/17/05 11:16 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdI1()
{
//	WORD	tval;
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "I100");

//	tval = (m_nZcmd | BOTH_CLKS);
//	m_cCommand |= 0x8c;			// GLOBAL on UV & VIS, VIS selected
//	OutputW(COMMAND, m_cCommand);
//
//	OutputW(Z_CMD, m_nZcmd);	// enable UV & VIS detectors
//	OutputW(Z_CMD, tval);		// send out 2 pixel clocks to VIS
//	OutputW(Z_CMD, m_nZcmd);
//	OutputW(Z_CMD, tval);
//	OutputW(Z_CMD, m_nZcmd);
//
//	m_cCommand &= 0x7f;			// GLOBAL on UV & VIS, UV selected
//	OutputW(COMMAND, m_cCommand);
//
//	OutputW(Z_CMD, m_nZcmd);	// enable UV & VIS detectors
//	OutputW(Z_CMD, tval);		// send out 2 pixel clocks to UV
//	OutputW(Z_CMD, m_nZcmd);
//	OutputW(Z_CMD, tval);
//	OutputW(Z_CMD, m_nZcmd);
//
//	m_nZcmd |= CLR_BOTH;		// Set CLEAR C Mode
//	OutputW(Z_CMD, m_nZcmd);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdi1.cpp $
 * 
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
