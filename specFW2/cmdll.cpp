//===========================================================================
//
//	Module Name:	cmdLL.cpp
//
// 	Function:	  	The routine is used to save the BRIGHTNESS LEVEL
//		 			for the LED used by the AUTOTESTER.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdll.cpp 2     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdLL()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "LL00");

	m_nLEDLevel = b2s();		// get value

	if (m_nLEDLevel > MAX_OUTPUT)
	{
		m_nLEDLevel = 1000;
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	// Save value in BRAM
	m_NvRam.OutputW(LED_LEVEL, m_nLEDLevel);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdll.cpp $
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
