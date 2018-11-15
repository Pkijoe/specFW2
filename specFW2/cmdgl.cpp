//===========================================================================
//
//	Module Name:	cmdGL.cpp
//
//	Function:		This routine returns the current state of the six Motor Lookers.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGL()
{
	WORD	status(NO_ERRORS);
	WORD	SVstatus, SVstatus2;
	char	cStatus;
	
	strcpy(m_nDataOutBuf, "GL00xxxxxxxxxxxx");

	SVstatus	= m_IO.InputW(SVSTAT_CS);
	SVstatus2	= m_IO.InputW(STATUS_REG2);	
	
	cStatus = ASCII0;
	if (SVstatus & SLIT_HOME)
		cStatus += 1;
	m_nDataOutBuf[4] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & SLIT_LIMIT)
		cStatus += 1;
	m_nDataOutBuf[5] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWX_HOME_AXIAL)
		cStatus += 1;
	m_nDataOutBuf[6] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWX_LIMIT_AXIAL)
		cStatus += 1;
	m_nDataOutBuf[7] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWY_HOME_AXIAL)
		cStatus += 1;
	m_nDataOutBuf[8] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWY_LIMIT_AXIAL)
		cStatus += 1;
	m_nDataOutBuf[9] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWX_HOME_RADIAL)
		cStatus += 1;
	m_nDataOutBuf[10] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWX_LIMIT_RADIAL)
		cStatus += 1;
	m_nDataOutBuf[11] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWY_HOME_RADIAL)
		cStatus += 1;
	m_nDataOutBuf[12] = cStatus;

	cStatus = ASCII0;
	if (SVstatus & VIEWY_LIMIT_RADIAL)
		cStatus += 1;
	m_nDataOutBuf[13] = cStatus;
	
	cStatus = ASCII0;              // CBF-149 - Toroid looker status.
	if (SVstatus2 & TOROID_MOTOR_HOME)
	cStatus += 1;
	m_nDataOutBuf[14] = cStatus;
	
	cStatus = ASCII0;              // CBF-26 - this was missing...	
	if (SVstatus2 & SHUTTER_MOTOR_HOME)
		cStatus += 1;
	m_nDataOutBuf[15] = cStatus;
	
	return status;
}
//===========================================================================

/*** Revision History ***

12/15/15    JO      Initial additions and changes for Century B.
07/28/16    JO      CBF-26 - Removed unused toroid commands and references.
02/02/17    JO      CBF-149 - Add toroid home sensor status bit.


$Log: /WinLab/SpecFW/cmdgl.cpp $
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
