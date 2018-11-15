//===========================================================================
//
//	Module Name:	cmdMW.cpp
//
// 	Function:	  	This routine writes to any legal location.
//					Currently this works on Battery RAM only.
//					Changes required to access other Registers
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdmw.cpp 3     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdMW()
{
	WORD			status(NO_ERRORS);
	unsigned long	addr;
	WORD			sval;
	bool			bWord(true);

	strcpy(m_nDataOutBuf, "MW00");

	b2a(&addr);					// Read 6 Byte Hex Address in ASCII
	m_pCmdPtr++;				// Ignore '=' between values
	m_nBytesRead++;
	sval = b2h();				// Read 4 Byte Hex Value in ASCII

	if (addr & 0x0001)
		bWord = false;			// Must be odd address, so read only byte

	// Check address and value for validity
	if (addr > 0xffff)
	{	// Offset to great
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		if (bWord)
		{
			m_NvRam.OutputW((WORD) addr, sval);
		}
		else
		{
			if (sval > 0xff)
			{	// value greater than a Byte
				status = ERR_PARA;
				memcpy(&m_nDataOutBuf[2], "07", 2);
			}
			else
			{
				m_NvRam.OutputB((WORD) addr, (BYTE) sval);
			}
		}
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdmw.cpp $
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/08/05 10:18 Frazzitl
 * Fixed Rad & Write memory to handle Battery RAM
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
