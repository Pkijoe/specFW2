//===========================================================================
//
//	Module Name:	cmdFM.cpp
//
//	Function:		This routine sets the current slit and view motor
//					locations as the key value saved in BATTERY RAM.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdFS()
{
	unsigned int	status(NO_ERRORS);
	WORD			SVstatus;
	WORD			steps;
	WORD			i;
	WORD			slittemp;

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "FS00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection

	m_nLooker	= 0;
	m_nMotorControl = 0;

	// CHECK FOR EITHER DETECTOR
	SVstatus  = m_IO.InputW(SVSTAT_CS);
	if (SVstatus & SLIT_HOME)
	{
		status = ERR_SLIT_HOME;
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}
	else if (SVstatus & SLIT_LIMIT)
	{
		status = ERR_SLIT_LIMIT;
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}

	// TAKE OUT ANY POSSIBLE BACKLASH
	m_nWaitTime  = MAX_S_TIME;
	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_nMotorControl |= CWOFF;
	slittemp = (WORD) (m_nMotorControl | CWON);
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	for (i = 1; i <= S_BACKLASH; i++) // STEP TOWARD HOME
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		status = delay( SLIT );
	}
	m_nWaitTime  = MAX_S_TIME;
	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_nMotorControl |= CCWOFF;
	slittemp = (WORD) (m_nMotorControl | CCWON);
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	for (i = 1; i <= S_BACKLASH; i++) // STEP AWAY FROM HOME
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		status = delay( SLIT );
	}

	// FIND HOME
	m_nWaitTime  = MAX_S_TIME;
	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_nMotorControl |= CWOFF;
	slittemp = (WORD) (m_nMotorControl | CWON);
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	steps	  = 0;
	while (steps < HSTP_SLIT)
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		status = delay( SLIT );
		steps += 1;				// COUNT STEPS TO HOME
		SVstatus = m_IO.InputW(SVSTAT_CS);
		if (SVstatus & SLIT_HOME)
		{						// MOVE INTO HOME DETECTOR
			for (i = 1; i <= S_BACKLASH; i++) // EVEN NUMBER OF STEPS
			{
				steps += 1;		// COUNT THESE STEPS ALSO
				m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
				status = delay( SLIT );
				m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
				status = delay( SLIT );
			}
			goto sethome;
		}
	}
	m_nMotorControl  = 0;
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	status = ERR_SLIT_HOME;
	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;

sethome:
	// GET TO FIRST STEP PASSED HOME
	m_nWaitTime  = MAX_S_TIME;
	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_nMotorControl |= CCWOFF;
	slittemp = (WORD) (m_nMotorControl | CCWON);
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	while (SVstatus & SLIT_HOME)
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		status = delay( SLIT );
		steps -= 1;				// SUBTRACT THESE STEPS
		SVstatus = m_IO.InputW(SVSTAT_CS);
		if (steps == 0)			// DETECTOR DID NOT TURN OFF
		{
			status = ERR_SLIT_HOME;
			m_nMotorControl  = 0;
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
			ReturnStatus((WORD)status); // Place return code into reply buffer
			return status;
		}
	}

	// CHECK FOR MINIMUM NUMBER OF STEPS REQUIRED
	if (steps < m_nSlitDelta)
	{
		status = ERR_SLIT_STEPS;
		m_nMotorControl  = 0;
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}

	steps	 -= 1;
	m_nSlitLow  = steps;
	m_nSlitHigh = (WORD) (m_nSlitLow - m_nSlitDelta);
	m_nSlitBarn = (WORD) (m_nSlitLow + m_nSlitDelta + m_nSlitBdStp);

	// SAVE KEY VALUE FOR SLIT
	m_NvRam.OutputW(HOME_2_SLIT, steps);

	// SAVE CURRENT VALUE FOR SLIT
	m_NvRam.OutputB(CURR_SLIT, 0);
	m_NvRam.OutputB(CURR_SLIT_TYPE, HOME);
	m_NvRam.OutputB(CURR_SLIT_POS, 0);

	m_cSlitType  = HOME;
	m_cSlitPos   = 0;
	m_cSlitTno   = HOME;
	m_cSlitPno   = 0;
	m_nSlitStep  = 0;
	m_nSlitSno   = 0;

	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);

	// FIND SLIT LIMIT POSITION
	if ((status = cmdLS()) != NO_ERRORS)
	{
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}

	status = cmdBS();

	// MOVE BACK TO KEY POSITION
	m_nSlitStep = (WORD) (m_nSlitLow + m_nSlitLoStp[0]);
	m_cSlitType = LOWRES;
	m_cSlitPos = 0;
	m_cSlitTno = LOWRES;
	m_cSlitPno = 0;
	status = mv_slit();
	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

// Special case for Slit where current location becomes new Key position
// This is used by application to compensate for temperature & barametric drift
// Added 12-12-2002 by T. Frazzini
unsigned int CParserThread::cmdSK()
{
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "SK00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical parameters

	m_nSlitLow	= m_nSlitSno;
	m_nSlitHigh = (WORD) (m_nSlitLow - m_nSlitDelta);
	m_nSlitBarn = (WORD) (m_nSlitLow + m_nSlitDelta + m_nSlitBdStp);

// Best not to save new Key position in BRAM so no chance of being lost
//	// SAVE KEY VALUE FOR SLIT
//	*HOME_2_SLIT  = m_nSlitSno;

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdFX()
{
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "FX00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical parameters
	
	// Write to the Axial or Radial variables based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
		{
		  m_nViewXPeak_Axial = m_nViewXno_Axial;

	      // SAVE KEY VALUE FOR VIEW X Axial
	      m_NvRam.OutputW(HOME_2_PEAK_AXIAL, m_nViewXno_Axial);
	
		}
		else  // RADIAL_TYPE
		{
		  m_nViewXPeak_Radial = m_nViewXno_Radial;

	      // SAVE KEY VALUE FOR VIEW X Radial
	      m_NvRam.OutputW(HOME_2_PEAK_RADIAL, m_nViewXno_Radial);
		}

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdFY()
{
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "FY00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical parameters

	// Write to the Axial or Radial variables based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
		{
		  m_nViewY15mm_Axial = m_nViewYno_Axial;

	      // SAVE KEY VALUE FOR VIEW Y Axial
	      m_NvRam.OutputW(HOME_2_15MM_AXIAL, m_nViewYno_Axial);
	
		}
		else  // RADIAL_TYPE
		{
		  m_nViewY15mm_Radial = m_nViewYno_Radial;

	      // SAVE KEY VALUE FOR VIEW Y Radial
	      m_NvRam.OutputW(HOME_2_15MM_RADIAL, m_nViewYno_Radial);
		}

	return status;
}
//===========================================================================

/*** Revision History ***

12/16/15    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/cmdfm.cpp $
 * 
 * 13    7/31/07 2:47p Nashth
 * Motor errors <LE> now returned as a specific motor error <S0>, <X0>, or
 * <Y0> as per QPR: THNJ75ML7T.
 * 
 * 12    6/26/07 3:46p Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
 * 
 * 11    8/01/05 3:06p Nashth
 * Corrected OutputW for HOME_2_SLIT, HOME_2_PEAK, and HOME_2_15MM to use
 * the NvRam class rather then m_IO.
 * 
 * 10    6/29/05 13:45 Frazzitl
 * Optimized Slit Motor
 * 
 * 7     5/20/05 14:53 Frazzitl
 * Sped up motors
 * 
 * 6     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 5     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 4     4/14/05 9:08 Frazzitl
 * Fixed motor problems
 * 
 * 3     4/06/05 2:31p Frazzitl
 * Better motor movement
 * 
 * 2     4/04/05 10:44 Frazzitl
 * Fixing problem with motor timing - only temporary
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
