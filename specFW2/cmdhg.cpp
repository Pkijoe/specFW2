//===========================================================================
//
//	Module Name:	cmdHG.cpp
//
// 	Function:	  	This routine moves the VIEW X & Y motors to the
//		 			KEY positions for the Hg lamp.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdHG()
{
	unsigned int	status(NO_ERRORS);
	WORD			stemp;

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical data
	strcpy(m_nDataOutBuf, "HG00");
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remoce protection
	
	// This command only works in Axial.
	
		stemp = m_nViewXHg_Axial;
	    m_nViewXStep_Axial = stemp;
	    m_cViewXPos_Axial  = 0;
	    m_cViewXPno_Axial  = 0;
		
	    if ((status = mv_viewx()) != NO_ERRORS)
	    {
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
        
	    stemp = m_nViewYHg_Axial;
	    m_nViewYStep_Axial = stemp;
	    m_cViewYPos_Axial  = 0;
	    m_cViewYPno_Axial  = 0;
	    status = mv_viewy();
	    
	    ReturnStatus((WORD)status); // Place return code into reply buffer
	    return status;
 }
//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/cmdhg.cpp $
 * 
 * 5     7/18/07 9:26a Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
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
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
