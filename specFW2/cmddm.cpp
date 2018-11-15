//===========================================================================
//
//	Module Name:	cmdDM.cpp
//
//	Function:		This routine is used to select the Vega chip mode.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmddm.cpp 6     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdDM()
{
	WORD			status(NO_ERRORS);
	char			val;

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters

	strcpy(m_nDataOutBuf, "DM00");

   	val	= *m_pCmdPtr++;
	m_nBytesRead++;

	if (val != ZODIAC && val != VEGA && val != VEGA_SCA)
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else if (m_cDetectorType == ZODIAC && val != ZODIAC)
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else if (m_cDetectorType != ZODIAC && m_cDetectorType != VEGA)
	{
		status = ERR_CMD;
		memcpy(&m_nDataOutBuf[2], "01", 2);
	}
	else
	{
		m_NvRam.OutputB(CHIP_TYPE_CODE_1, 'C');
		m_NvRam.OutputB(CHIP_TYPE_CODE_2, 'T');
		m_cChipMode	= val;
		m_NvRam.OutputB(CHIP_MODE, val);
	}
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Remove protection
	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmddm.cpp $
 * 
 * 6     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 5     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 4     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 3     4/12/05 3:33p Frazzitl
 * Fixing problem with Chip Mode
 * 
 * 2     4/12/05 10:32a Frazzitl
 * Fixed problem with setting Chip mode
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
