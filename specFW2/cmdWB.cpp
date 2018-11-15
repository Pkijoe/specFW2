//===========================================================================
//
//	Module Name:	cmdWB.cpp
//
// 	Function:	  	This routine writes a byte to any BRAM location.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdWB.cpp 1     8/10/05 12:58 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdWB()
{
	WORD			status(NO_ERRORS);
	unsigned long	addr;
	BYTE			bval;

	strcpy(m_nDataOutBuf, "WB00");

	b2a4(&addr);				// Read 4 Byte Hex Address in ASCII
	m_pCmdPtr++;				// Ignore '=' between values
	m_nBytesRead++;
	bval = b2h2();				// Read 2 Byte Hex Value in ASCII

	// Check address and value for validity
	if (addr > BRAM_END)		// Last BRAM Location
	{	// Offset to great
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		m_NvRam.OutputB((WORD) addr, bval);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdWB.cpp $
 * 
 * 1     8/10/05 12:58 Frazzitl
 * Added functions to read and write bytes from and to BRAM

$NoKeywords: $

** End of Rev History **/
