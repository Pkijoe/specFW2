//===========================================================================
//
//	Module Name:	cmdF1.cpp
//
// 	Function:		This routine sets the tub heater PI integrator power.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdF1()
{
	WORD			status(NO_ERRORS);
	unsigned long	ipower;
	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
	
	strcpy(m_nDataOutBuf,"F100");
	ipower = b2c();					// Get 3 digits
	if (ipower > 100) ipower = 100;
	if (ipower < 0) ipower = 0;
	
	Set_Integrator_Power((float) ipower);
	
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	return status;
}
//===========================================================================

/*** Revision History ***

12/05/16    JO    	CBF-104 - Add manual integrator preset functionality via DSM commands and debug console printout.
12/15/16    JO    	CBF-121 - Lid warmup ramp and manual tub heater integrator power preset.


$Log: /IcarusBased/SpecFW/cmdf1.cpp $
 * 
 * 5     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 4     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
