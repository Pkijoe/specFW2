//===========================================================================
//
//	Module Name:	cmdGB.cpp
//
// 	Function:	  	This routine reads any BRAM location and transmits the
//		 			byte value read at that location.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdGB.cpp 1     8/10/05 12:58 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGB()
{
	WORD			status(NO_ERRORS);
	BYTE			bValue;
	unsigned long	addr;

	strcpy(m_nDataOutBuf, "GB00xx");

	b2a4(&addr);				// Read 4 Byte Hex Address in ASCII

	// Check address for validity
	if (addr > BRAM_END)			// Last BRAM Location
	{	// Offset to great
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		bValue = m_NvRam.InputB((WORD) addr);
		h2b2(bValue, 4);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdGB.cpp $
 * 
 * 1     8/10/05 12:58 Frazzitl
 * Added functions to read and write bytes from and to BRAM

$NoKeywords: $

** End of Rev History **/
