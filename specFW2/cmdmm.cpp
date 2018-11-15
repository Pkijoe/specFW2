//===========================================================================
//
//	Module Name:	cmdMM.cpp
//
//	Function:	  	This routine accepts the passed direction and
//					a relative number of steps and drives the motor.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdMS()
{
	unsigned int	status(NO_ERRORS);
	char			sdir;
	WORD			sstep;

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical data
	strcpy(m_nDataOutBuf, "MS00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical data

	sdir	= *m_pCmdPtr++;
	m_nBytesRead++;
	sstep	= b2s();

	if (sdir == INCREASE)
	{
		if ((m_nSlitSno + sstep) > m_nMaxSlitStep)
		{
			status = ERR_PARA2;
		}
		else
			m_nSlitStep = (WORD) (m_nSlitSno + sstep);
	}
	else if (sdir == DECREASE)
	{
		if (m_nSlitSno < sstep)
		{
			status = ERR_PARA2;
		}
		else
			m_nSlitStep = (WORD) (m_nSlitSno - sstep);
	}
	else
	{
		status = ERR_PARA1;
	}

	if (status == NO_ERRORS)
		status = mv_slit();

	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

unsigned int CParserThread::cmdMX()
{
	unsigned int	status(NO_ERRORS);
	char			xdir;
	WORD			xstep;
	WORD			xtmp;

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical data
	strcpy(m_nDataOutBuf, "MX00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical data

	xdir	= *m_pCmdPtr++;
	xstep	= b2s();
	xtmp	= (WORD) (xstep / 2);
	if (xstep != (2 * xtmp))	// Could be ODD number
		xstep += 1;				// Make EVEN number
		
	// Write to the Axial or Radial variables based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	{
		if (xdir == INCREASE)
		{
			if ((m_nViewXno_Axial + xstep) > m_nMaxXSteps_Axial)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewXStep_Axial = (WORD) (m_nViewXno_Axial + xstep);
		}
		else if (xdir == DECREASE)
		{
			if (m_nViewXno_Axial < xstep)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewXStep_Axial = (WORD) (m_nViewXno_Axial - xstep);
		}
		else
		{
			status = ERR_PARA1;
		}

		if (status == NO_ERRORS)
			status = mv_viewx();
	}
	else   // RADIAL_TYPE
	{
		if (xdir == INCREASE)
		{
			if ((m_nViewXno_Radial + xstep) > m_nMaxXSteps_Radial)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewXStep_Radial = (WORD) (m_nViewXno_Radial + xstep);
		}
		else if (xdir == DECREASE)
		{
			if (m_nViewXno_Radial < xstep)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewXStep_Radial = (WORD) (m_nViewXno_Radial - xstep);
		}
		else
		{
			status = ERR_PARA1;
		}

		if (status == NO_ERRORS)
			status = mv_viewx();
	}

	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

unsigned int CParserThread::cmdMY()
{
	unsigned int	status(NO_ERRORS);
	char			ydir;
	WORD			ystep;
	WORD			ytmp;

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical data
	strcpy(m_nDataOutBuf, "MY00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical data

	ydir	= *m_pCmdPtr++;
	ystep	= b2s();
	ytmp	= (WORD) (ystep / 2);
	if (ystep != (2 * ytmp))	// Could be ODD number
		ystep += 1;				// Make EVEN number
		
	// Write to the Axial or Radial variables based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	{
		if (ydir == INCREASE)
		{
			if ((m_nViewYno_Axial + ystep) > m_nMaxYSteps_Axial)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewYStep_Axial = (WORD) (m_nViewYno_Axial + ystep);
		}
		else if (ydir == DECREASE)
		{
			if (m_nViewYno_Axial < ystep)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewYStep_Axial = (WORD) (m_nViewYno_Axial - ystep);
		}
		else
		{
			status = ERR_PARA1;
		}

		if (status == NO_ERRORS)
			status = mv_viewy();
	}
	else
	{
		if (ydir == INCREASE)
		{
			if ((m_nViewYno_Radial + ystep) > m_nMaxYSteps_Radial)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewYStep_Radial = (WORD) (m_nViewYno_Radial + ystep);
		}
		else if (ydir == DECREASE)
		{
			if (m_nViewYno_Radial < ystep)
			{
				status = ERR_PARA2;
			}
			else
				m_nViewYStep_Radial = (WORD) (m_nViewYno_Radial - ystep);
		}
		else
		{
			status = ERR_PARA1;
		}

		if (status == NO_ERRORS)
			status = mv_viewy();
	}

	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

/*** Revision History ***

1/5/16    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/cmdmm.cpp $
 * 
 * 3     6/26/07 3:46p Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
