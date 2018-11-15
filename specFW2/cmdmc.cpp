//===========================================================================
//
//	Module Name:	cmdMC.cpp
//
// 	Function:	  	This routine reads any location and transmits the
//		 			value read at that location.
//					Currently this works on Battery RAM only.
//					Changes required to access other Registers
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdmc.cpp 2     4/08/05 10:18 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdMC()
{
	WORD			status(NO_ERRORS);
	WORD			nValue;
	unsigned long	addr;
	bool			bWord(true);

	strcpy(m_nDataOutBuf, "MC0000xxxx");

	b2a(&addr);					// Read 6 Byte Hex Address in ASCII

	if (addr & 0x0001)
		bWord = false;			// Must be odd address, so read only byte

	// Check address for validity
	if (addr > 0xffff)
	{	// Offset to great
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		if (bWord)
			nValue = m_NvRam.InputW((WORD) addr);
		else
			nValue = m_NvRam.InputB((WORD) addr);
		h2b4(nValue, 6);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdmc.cpp $
 * 
 * 2     4/08/05 10:18 Frazzitl
 * Fixed Rad & Write memory to handle Battery RAM
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
