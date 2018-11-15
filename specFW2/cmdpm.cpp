//===========================================================================
//
//	Module Name:	cmdPM.cpp
//
// 	Function:  		This routine accepts the passed position number and
//		 			points to the corresponding value in the table.
//					It then drives the motor to the proper position.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdPS()
{
	unsigned int	status(NO_ERRORS);
	char			stype;
	char			sstep;

	strcpy(m_nDataOutBuf, "PS00");

	stype	= *m_pCmdPtr++;
	m_nBytesRead++;
	sstep	= (char) (*m_pCmdPtr++ - ASCII0);
	m_nBytesRead++;

	switch (stype)
	{
		case HOME:
			if (sstep != 0)
			{
				status = ERR_PARA2;
			}
			else
				m_nSlitStep = m_nSlitHome;
			break;
		case HIGHRES:
			if (sstep > MAX_SLIT_HIGH)
			{
				status = ERR_PARA2;
			}
			else
				m_nSlitStep = (WORD) (m_nSlitHigh + m_nSlitHiStp[sstep]);
			break;
		case LOWRES:
			if (sstep > MAX_SLIT_LOW)
			{
				status = ERR_PARA2;
			}
			else
				m_nSlitStep = (WORD) (m_nSlitLow + m_nSlitLoStp[sstep]);
			break;
		case BARNDOOR:
			if (sstep != 0)
			{
				status = ERR_PARA2;
			}
			else
				m_nSlitStep = m_nSlitBarn;
			break;
		case LIMIT:
			if (sstep != 0)
			{
				status = ERR_PARA2;
			}
			else
				m_nSlitStep = m_nMaxSlitStep;
			break;
		default:
			status = ERR_PARA1;
			break;
	}

	if (status == NO_ERRORS)
	{
		m_cSlitType = stype;
		m_cSlitTno	= stype;
		m_cSlitPos  = sstep;
		m_cSlitPno  = sstep;
		status	 = mv_slit();
	}

	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

unsigned int CParserThread::cmdPX()
{
	unsigned int	status(NO_ERRORS);
	WORD			xtemp;
	char			xstep;

	strcpy(m_nDataOutBuf, "PX00");

	xtemp = b2c();
	
	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == RADIAL_TYPE)
	{
		if (xtemp > MAX_VIEW_X_RADIAL)			// CHECK FOR LEGAL POSITION
		{
			status = ERR_PARA;
		}
		else
		{
			xstep = (char) xtemp;
			xtemp = (WORD) (m_nViewXStp_Radial[xstep] + m_nViewXPeak_Radial - m_nViewXStp_Radial[XPEAK_RADIAL]);
			if ((xtemp < 0) || (xtemp > m_nMaxXSteps_Radial))
			{
				status = ERR_PARA;
			}
			else
			{
				m_nViewXStep_Radial = xtemp;
				m_cViewXPos_Radial  = xstep;
				m_cViewXPno_Radial  = xstep;
				status		 = mv_viewx();
			}
		}
	}
	else  // AXIAL_TYPE
	{
		if (xtemp > MAX_VIEW_X_AXIAL)			// CHECK FOR LEGAL POSITION
		{
			status = ERR_PARA;
		}
		else
		{
			xstep = (char) xtemp;
			xtemp = (WORD) (m_nViewXStp_Axial[xstep] + m_nViewXPeak_Axial - m_nViewXStp_Axial[XPEAK_AXIAL]);
			if ((xtemp < 0) || (xtemp > m_nMaxXSteps_Axial))
			{
				status = ERR_PARA;
			}
			else
			{
				m_nViewXStep_Axial = xtemp;
				m_cViewXPos_Axial  = xstep;
				m_cViewXPno_Axial  = xstep;
				status		 = mv_viewx();
			}
		}
	}
	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

unsigned int CParserThread::cmdPY()
{
	unsigned int	status(NO_ERRORS);
	WORD			ytemp;
	char			ystep;

	strcpy(m_nDataOutBuf, "PY00");

	ytemp = b2c();
	
	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == RADIAL_TYPE)
	{
		if (ytemp > MAX_VIEW_Y_RADIAL)			// CHECK FOR LEGAL POSITION
		{
			status = ERR_PARA;
		}
		else
		{
			ystep = (char) ytemp;
			ytemp = (WORD) (m_nViewYStp_Radial[ystep] + m_nViewY15mm_Radial - m_nViewYStp_Radial[Y15MM_RADIAL]);
			if ((ytemp < 0) || (ytemp > m_nMaxYSteps_Radial))
			{
				status = ERR_PARA;
			}
			else
			{
				m_nViewYStep_Radial = ytemp;
				m_cViewYPos_Radial  = ystep;
				m_cViewYPno_Radial  = ystep;
				status		 = mv_viewy();
			}
		}
	}
	else  // AXIAL_TYPE
	{
		if (ytemp > MAX_VIEW_Y_AXIAL)			// CHECK FOR LEGAL POSITION
		{
			status = ERR_PARA;
		}
		else
		{
			ystep = (char) ytemp;
			ytemp = (WORD) (m_nViewYStp_Axial[ystep] + m_nViewY15mm_Axial - m_nViewYStp_Axial[Y15MM_AXIAL]);
			if ((ytemp < 0) || (ytemp > m_nMaxYSteps_Axial))
			{
				status = ERR_PARA;
			}
			else
			{
				m_nViewYStep_Axial = ytemp;
				m_cViewYPos_Axial  = ystep;
				m_cViewYPno_Axial  = ystep;
				status		 = mv_viewy();
			}
		}
	}
	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.

$Log: /IcarusBased/SpecFW/cmdpm.cpp $
 * 
 * 6     7/18/07 9:26a Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
 * 
 * 5     7/17/07 5:30p Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
 * 
 * 4     5/08/06 10:29a Frazzitl
 * Fixed problem with command not responding with the correct string
 * length.
 * 
 * 3     5/05/06 2:31p Frazzitl
 * Fixed problem caused by casting the read value to a char before it was
 * checked against its limits.
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
