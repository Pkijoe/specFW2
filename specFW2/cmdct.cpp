//===========================================================================
//
//	Module Name:	cmdCT.cpp
//
//	Function:		This routine is used to select the Zodiac or
//					Vega chip type.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdct.cpp 2     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdCT()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "CT00");

 	m_cDetectorType	= *m_pCmdPtr++;
	m_nBytesRead++;

	if (m_cDetectorType != VEGA)
	{	// Only VEGA Detector allowed here
		m_cDetectorType = VEGA;
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	m_NvRam.OutputB(CHIP_TYPE, m_cDetectorType);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdct.cpp $
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
