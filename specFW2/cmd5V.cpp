//===========================================================================
//
//	Module Name:	cmd5V.cpp
//
// 	Function:		This routine gets the Detector CPLD version number.
//
//	Original Author:	TLFrazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmd5V.cpp 3     5/05/06 2:06p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd5V()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "5V00");

	int	nLenUV	= m_sDetectorFPGAVersionUV .GetLength();
	int	nLenVIS	= m_sDetectorFPGAVersionVIS.GetLength();

	for (int n = 0; n < nLenUV; n++)
		m_nDataOutBuf[n + 4] = (char) m_sDetectorFPGAVersionUV.GetAt(n);

	if (m_bSingleChannel == false)
	{
		m_nDataOutBuf[nLenUV + 4] = ',';
		for (int n = 0; n < nLenVIS; n++)
			m_nDataOutBuf[n + nLenUV + 5] = (char) m_sDetectorFPGAVersionVIS.GetAt(n);

		m_nDataOutBuf[nLenUV + nLenVIS + 5] = NULL;
	}
	else
	{
		m_nDataOutBuf[nLenUV + 4] = NULL;
	}
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmd5V.cpp $
 * 
 * 3     5/05/06 2:06p Frazzitl
 * Changed firmware to allow different detector FPGA versions in the same
 * instrument.
 * 
 * 2     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 1     10/12/05 3:31p Frazzitl
 * Added functions to get Detector board FPGA version number
 
$NoKeywords: $

** End of Rev History **/
