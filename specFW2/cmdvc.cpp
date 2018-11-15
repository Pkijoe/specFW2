//===========================================================================
//
//	Module Name:	cmdVC.cpp
//
// 	Function:	  	The routine is used to control the 8 VALVES.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdvc.cpp 3     4/26/05 15:49 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdVC()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "VC00");

	m_pCmdPtr++;								// Skip
	if (*m_pCmdPtr++ == '1')					// Water On/Off
	{
		m_nDigitalControl |= CHILLER_ENABLED;
		m_nSolenoidRegister |= WATER_EN;
	}
	else
	{
		m_nDigitalControl &= ~CHILLER_ENABLED;
		m_nSolenoidRegister &= ~WATER_EN;
	}
	m_pCmdPtr++;								// Skip
	m_pCmdPtr++;								// Skip
	if (*m_pCmdPtr++ == '1')					// Nitrogen
		m_nSolenoidRegister |= NITROGEN_HI;
	else
		m_nSolenoidRegister &= ~NITROGEN_HI;
	m_pCmdPtr++;								// Skip
	if (*m_pCmdPtr++ == '1')					// Argon Flow High/Low
		m_nSolenoidRegister |= ARGON_HI;
	else
		m_nSolenoidRegister &= ~ARGON_HI;
	if (*m_pCmdPtr++ == '1')					// Argon on
		m_nSolenoidRegister |= ARGON_EN;
	else
		m_nSolenoidRegister &= ~ARGON_EN;

	m_nBytesRead += 8;

	m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);	// Finally OUTPUT to port

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdvc.cpp $
 * 
 * 3     4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
