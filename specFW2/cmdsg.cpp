//===========================================================================
//
//	Module Name:	cmdSG.cpp
//
//	Function:		This routine is used to set the GRATING for Vega-SCA.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdsg.cpp 3     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSG()
{
	WORD	status(NO_ERRORS);
	char	val;

	strcpy(m_nDataOutBuf, "SG00");

	val	= (char) (*m_pCmdPtr++ - ASCII0);
	m_nBytesRead++;

	if (val == YES)
	{
		m_NvRam.OutputB(GRATING, '1');
		m_cGrating = '1';
		m_nSolenoidRegister |= GRATING_EN;	// GRATING ENABLED
		m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
	}
	else if (val == NO)
	{
		m_NvRam.OutputB(GRATING, '0');
		m_cGrating = '0';
		m_nSolenoidRegister &= ~GRATING_EN;	// GRATING DISABLED
		m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
	}
	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdsg.cpp $
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:19 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
