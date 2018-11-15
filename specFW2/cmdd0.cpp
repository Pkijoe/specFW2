//===========================================================================
//
//	Module Name:	cmdD0.cpp
//
//	Function:		This routine heater parameters to default values.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdd0.cpp 6     7/13/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdD0()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "D000");

	setDefaultHeaterParameters();

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdd0.cpp $
 * 
 * 6     7/13/05 11:28 Frazzitl
 * Added function to set default heater parameters - now implemented on
 * startup since it seems some are being lost.
 * 
 * 5     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 4     4/19/05 3:23p Frazzitl
 * Fixed problems with Tub constants and UV & VIS resolution 
 * 
 * 3     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 2     4/06/05 11:11a Frazzitl
 * Fixed some incorrect BRAM calls
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
