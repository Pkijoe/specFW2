//===========================================================================
//
//	Module Name:	cmdC0.cpp
//
//	Function:		This routine is used to select Heater control ON or OFF.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdc0.cpp 4     9/30/05 9:23a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdC0()
{
	WORD	status(NO_ERRORS);
	char	val;

	strcpy(m_nDataOutBuf, "C000");

 	val	= *m_pCmdPtr++;
	m_nBytesRead++;

	if (val == '0' || val == '1')
	{
		m_NvRam.OutputB(HEATER_CONTROL, val);
		m_bReached_temp = false;
	}
	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdc0.cpp $
 * 
 * 4     9/30/05 9:23a Frazzitl
 * Changed variable names to make more readable
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:23 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:13 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
