//===========================================================================
//
//	Module Name:	OpticsThread.cpp
//
// 	Function:		This routine compiles time Optics exposed to plasma
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

/////////////////////////////////////////////////////////////////////////////
// statusThreadUV

UINT WINAPI CParserThread::opticsThread(LPVOID pParam)
{
	CParserThread *pThis = (CParserThread *) pParam;

	int		nMinCnt(0);

	for ( ; ; )
	{
		Sleep(60000);	// One minute sleep

theApp.EnterCriticalSection1(&pThis->m_CriticalSection);

// CBF-17 - Removed for now.  Must be revisited with CBF-24
		if ((pThis->m_nPlasma_lit == 1)) //&& (pThis->m_nSolenoidRegister & SHUTTER_EN))
		{
			++nMinCnt;
			if (nMinCnt >= 60)
			{	// Reached one more hour
				++pThis->m_lOptics_hours;
				if (pThis->m_lOptics_hours > OPTICS_HOURS_MAX)
					pThis->m_lOptics_hours = OPTICS_HOURS_DEFAULT;
				pThis->m_IO.OutputW(OPTICS_HOURS, (unsigned short) pThis->m_lOptics_hours);
				nMinCnt = 0;
			}
		}

theApp.LeaveCriticalSection1(&pThis->m_CriticalSection);
	}
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

7/05/16    JO      CBF-17 - Removed code that refernced SHUTTER_EN bit for now.  Must be revisited with CBF-24.	

$Log: /IcarusBased/SpecFW/OpticsThread.cpp $
 * 
 * 3     1/13/06 1:37p Frazzitl
 * Removed dead code as recommended in a QPR
 * 
 * 2     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 1     7/01/05 10:13 Frazzitl
 * New thread used to monitor the optics exposure to the plasma.

$NoKeywords: $

** End of Rev History **/
