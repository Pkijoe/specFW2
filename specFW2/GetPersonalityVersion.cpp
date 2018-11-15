//===========================================================================
//
//	Module Name:	GetPersonalityVersion.cpp
//
// 	Function:		This routine is a special function that gets the
//					Personality Board's version number
//
//	Original Author:	T Frazzini	May 2005
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/GetPersonalityVersion.cpp 3     8/24/05 8:49a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::GetPersonalityVersion()
{
	printf("---Personality Board version info---\r\n");

	char	buffer[256];
	WORD	wVersion   = m_IO.InputW(SVSTAT_CS);
			wVersion  &= BOARD_VERSION;
			wVersion >>= 11;
	int		nLen = sprintf(buffer, "0x%x", wVersion);

//	float	fVersion = (float) wVersion / (float) 100.0;
//	int		nLen = sprintf(buffer, "%.2f", fVersion);

	m_sPersonalityVersion = buffer;
	if ( nLen > 253 )
		nLen = 253;     // Do not exceed 256 char buffer size
	buffer[nLen    ] = '\r';
	buffer[nLen + 1] = '\n';
	buffer[nLen + 2] = '\0';
	printf(buffer);
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/GetPersonalityVersion.cpp $
 * 
 * 3     8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 2     6/10/05 9:12a Frazzitl
 * Corrected hex output of version number
 * 
 * 1     6/06/05 2:32p Frazzitl
 * Added functions to read the Personality Board version number and it's
 * FPGA version number.

$NoKeywords: $

** End of Rev History **/
