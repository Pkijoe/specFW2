//===========================================================================
//
//	Module Name:	GetCPLDVersion.cpp
//
// 	Function:		This routine is a special function that gets the
//					Personality Board's FPGA version number
//
//	Original Author:	T Frazzini	May 2005
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/GetCPLDVersion.cpp 2     6/10/05 9:12a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::GetCPLDVersion()
{
	printf("---FPGA version info---\r\n");

	char	buffer[10];
	WORD	wVersion = m_IO.InputW(FPGA_VERSION_NO);
	int		nLen = sprintf(buffer, "0x%x", wVersion);


//	float	fVersion = (float) wVersion / (float) 100.0;
//	int		nLen = sprintf(buffer, "%.2f", fVersion);

	m_sCPLDVersion = buffer;

	buffer[nLen    ] = '\r';
	buffer[nLen + 1] = '\n';
	printf(buffer);
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/GetCPLDVersion.cpp $
 * 
 * 2     6/10/05 9:12a Frazzitl
 * Corrected hex output of version number
 * 
 * 1     6/06/05 2:32p Frazzitl
 * Added functions to read the Personality Board version number and it's
 * FPGA version number.

$NoKeywords: $

** End of Rev History **/
