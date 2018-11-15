//===========================================================================
//
//	Module Name:	cmd3V.cpp
//
// 	Function:		This routine gets the CPLD version number.
//
//	Original Author:	TLFrazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmd3V.cpp 4     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd3V()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "3V00");

	int	nLen = m_sCPLDVersion.GetLength();

	for (int n = 0; n < nLen; n++)
		m_nDataOutBuf[n + 4] = (char) m_sCPLDVersion.GetAt(n);

	m_nDataOutBuf[nLen + 4] = NULL;
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmd3V.cpp $
 * 
 * 4     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 3     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 2     6/06/05 11:57a Frazzitl
 * Added functions to read the CPLD and Personality Board version numbers.
 * 
 * 1     6/06/05 8:27a Frazzitl
 * Added new functions to return CPLD and Personality Board version
 * numbers.  Currently only a stub is implemented.
 
$NoKeywords: $

** End of Rev History **/
