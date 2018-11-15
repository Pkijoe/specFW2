//===========================================================================
//
//	Module Name:	cmdVV.cpp
//
// 	Function:	  	The routine is used to get the status of the 8 VALVES.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdvv.cpp 4     11/29/05 11:30a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdVV()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "VV00xxxxxx");	// 6 character string

	WORD	valve = m_nSolenoidRegister;

	if (valve & WATER_EN)
		m_nDataOutBuf[4] = ON  + ASCII0;
	else
		m_nDataOutBuf[4] = OFF + ASCII0;

	if (m_nDigitalControl & HGLAMP_EN)
		m_nDataOutBuf[5] = ON  + ASCII0;
	else
		m_nDataOutBuf[5] = OFF + ASCII0;

	if (valve & TOROID_POSITION)
		m_nDataOutBuf[6] = ON  + ASCII0;
	else
		m_nDataOutBuf[6] = OFF + ASCII0;

	if (valve & NITROGEN_HI)
		m_nDataOutBuf[7] = ON  + ASCII0;
	else
		m_nDataOutBuf[7] = OFF + ASCII0;
	
	if (valve & ARGON_HI)
		m_nDataOutBuf[8] = ON  + ASCII0;
	else
		m_nDataOutBuf[8] = OFF + ASCII0;
	
	if (valve & ARGON_EN)
		m_nDataOutBuf[9] = ON  + ASCII0;
	else
		m_nDataOutBuf[9] = OFF + ASCII0;
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

07/25/16	JO		Updated for new radial toroid design to use TOROID_POSITION - CBF-33.


$Log: /IcarusBased/SpecFW/cmdvv.cpp $
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
 * 2     4/12/05 15:22 Frazzitl
 * Fixed minor HgLamp problem
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
