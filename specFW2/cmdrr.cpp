//===========================================================================
//
//	Module Name:	cmdRR.cpp
//
// 	Function:	  	This routine returns the ram test status bytes.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdrr.cpp 3     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdRR()
{
	WORD	status(NO_ERRORS);
	
	strcpy(m_nDataOutBuf, "RR00xxxxxxxx");

	m_nDataOutBuf[ 4] = (char) (m_NvRam.InputB(DRAM0_ERROR) + ASCII0);
	m_nDataOutBuf[ 5] = (char) (m_NvRam.InputB(DRAM1_ERROR) + ASCII0);
	m_nDataOutBuf[ 6] = (char) (m_NvRam.InputB(DRAM2_ERROR) + ASCII0);
	m_nDataOutBuf[ 7] = (char) (m_NvRam.InputB(DRAM3_ERROR) + ASCII0);
	m_nDataOutBuf[ 8] = (char) (m_NvRam.InputB(DRAM4_ERROR) + ASCII0);
	m_nDataOutBuf[ 9] = (char) (m_NvRam.InputB(DRAM5_ERROR) + ASCII0);
	m_nDataOutBuf[10] = (char) (m_NvRam.InputB(DRAM6_ERROR) + ASCII0);
	m_nDataOutBuf[11] = (char) (m_NvRam.InputB(DRAM7_ERROR) + ASCII0);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdrr.cpp $
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
