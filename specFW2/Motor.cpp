//===========================================================================
//
//	Module Name:	Motor.cpp
//
//	Function:  		This routine moves the slit and view drives to the
//					desired postion by using one of three other routines:
//					mv_motor  - does the basic move
//					ramp_up   - move from slowest speed to final speed
//					ramp_down - move from current speed to slowest speed
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::mv_slit()
{
	unsigned int	status(NO_ERRORS);
	WORD			steps;
	unsigned int	dummy;

	if (m_nSlitStep < m_nSlitSno)				// Move backward toward HOME
	{
		// MOVE BACK (CW) + BACKLASH
		m_nLooker = SLIT_HOME;
		steps  = (WORD) (m_nSlitSno - m_nSlitStep + S_BACKLASH);
		if ((status = mv_motor(steps, (int) SLIT, (int) DECREASE)) == NO_ERRORS)
		{
			m_nSlitSno = m_nSlitStep;
			// MOVE AWAY (CCW) - NO BACKLASH
			steps   = S_BACKLASH;
			m_nLooker	= SLIT_LIMIT;
			if ((status = mv_motor(steps, (int) SLIT, (int) INCREASE)) == NO_ERRORS)
				m_nSlitSno = m_nSlitStep;
		}
	}							  
	else if (m_nSlitStep > m_nSlitSno)		// Move forward away from HOME
	{
		// MOVE AWAY (CCW) - NO BACKLASH
		m_nLooker = SLIT_LIMIT;
		steps  = (WORD) (m_nSlitStep - m_nSlitSno);
		if ((status = mv_motor(steps, (int) SLIT, (int) INCREASE)) == NO_ERRORS)
			m_nSlitSno = m_nSlitStep;
	}

	if (status == NO_ERRORS)
	{
		// SAVE CURRENT VALUE FOR SLIT
		m_NvRam.OutputW(CURR_SLIT, m_nSlitSno);
		// SAVE CURRENT TYPE & POSITION
		m_NvRam.OutputB(CURR_SLIT_TYPE, m_cSlitTno);
		m_NvRam.OutputB(CURR_SLIT_POS, m_cSlitPno);
	}
	else
		dummy = cmdCS();				// Reset position - KEY

	return status;
}
//===========================================================================

unsigned int CParserThread::mv_viewx()
{
	unsigned int	status(NO_ERRORS);
	WORD			steps;
	unsigned int	dummy;

	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
		if (m_nViewXStep_Axial < m_nViewXno_Axial)			// Move backward toward HOME
		{
			// MOVE DOWN (IN) + BACKLASH
			m_nLooker = VIEWX_HOME_AXIAL;
			steps  = (WORD) (m_nViewXno_Axial - m_nViewXStep_Axial + V_BACKLASH);
			if ((status = mv_motor(steps, (int) VIEWX_AXIAL, (int) DECREASE)) == NO_ERRORS)
			{
				m_nViewXno_Axial = m_nViewXStep_Axial;
				// MOVE UP (OUT) - NO BACKLASH
				steps	= V_BACKLASH;
				m_nLooker	= VIEWX_LIMIT_AXIAL;
				if ((status = mv_motor(steps, (int) VIEWX_AXIAL, (int) INCREASE)) == NO_ERRORS)
					m_nViewXno_Axial = m_nViewXStep_Axial;
			}
		}
		else if (m_nViewXStep_Axial > m_nViewXno_Axial)		// Move forward away from HOME
		{
			// MOVE UP (OUT) - NO BACKLASH
			m_nLooker = VIEWX_LIMIT_AXIAL;
			steps  = (WORD) (m_nViewXStep_Axial - m_nViewXno_Axial);
			if ((status = mv_motor(steps, (int) VIEWX_AXIAL, (int) INCREASE)) == NO_ERRORS)
				m_nViewXno_Axial = m_nViewXStep_Axial;
		}
	    
		if (status == NO_ERRORS)
		{
			// SAVE CURRENT VALUE FOR VIEW X
			m_NvRam.OutputW(CURR_VIEWX_AXIAL, m_nViewXno_Axial);
			// SAVE CURRENT POSITION
			m_NvRam.OutputB(CURR_VIEWX_POS_AXIAL, m_cViewXPno_Axial);
		}
		else
			dummy = cmdCX();				// Reset position - KEY

 	  }
	else  // RADIAL_TYPE
	  {	
		if (m_nViewXStep_Radial < m_nViewXno_Radial)			// Move backward toward HOME
		{
			// MOVE DOWN (IN) + BACKLASH
			m_nLooker = VIEWX_HOME_RADIAL;
			steps  = (WORD) (m_nViewXno_Radial - m_nViewXStep_Radial + V_BACKLASH);
			if ((status = mv_motor(steps, (int) VIEWX_RADIAL, (int) DECREASE)) == NO_ERRORS)
			{
				m_nViewXno_Radial = m_nViewXStep_Radial;
				// MOVE UP (OUT) - NO BACKLASH
				steps	= V_BACKLASH;
				m_nLooker	= VIEWX_LIMIT_RADIAL;
				if ((status = mv_motor(steps, (int) VIEWX_RADIAL, (int) INCREASE)) == NO_ERRORS)
					m_nViewXno_Radial = m_nViewXStep_Radial;
			}
		}
		else if (m_nViewXStep_Radial > m_nViewXno_Radial)		// Move forward away from HOME
		{
			// MOVE UP (OUT) - NO BACKLASH
			m_nLooker = VIEWX_LIMIT_RADIAL;
			steps  = (WORD) (m_nViewXStep_Radial - m_nViewXno_Radial);
			if ((status = mv_motor(steps, (int) VIEWX_RADIAL, (int) INCREASE)) == NO_ERRORS)
				m_nViewXno_Radial = m_nViewXStep_Radial;
		}
	    
		if (status == NO_ERRORS)
		{
			// SAVE CURRENT VALUE FOR VIEW X
			m_NvRam.OutputW(CURR_VIEWX_RADIAL, m_nViewXno_Radial);
			// SAVE CURRENT POSITION
			m_NvRam.OutputB(CURR_VIEWX_POS_RADIAL, m_cViewXPno_Radial);
		}
		else
			dummy = cmdCX();				// Reset position - KEY
	  
	  } // end else	
	
	return status;
}

//===========================================================================

unsigned int CParserThread::mv_viewy()
{
	unsigned int	status(NO_ERRORS);
	WORD			steps;
	unsigned int	dummy;
	
	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
		if (m_nViewYStep_Axial < m_nViewYno_Axial)			// Move backward toward HOME
		{
			// MOVE DOWN (IN) + BACKLASH
			m_nLooker = VIEWY_HOME_AXIAL;
			steps  = (WORD) (m_nViewYno_Axial - m_nViewYStep_Axial + V_BACKLASH);
			if ((status = mv_motor(steps, (int) VIEWY_AXIAL, (int) DECREASE)) == NO_ERRORS)
			{
				m_nViewYno_Axial = m_nViewYStep_Axial;
				// MOVE UP (OUT) - NO BACKLASH
				steps	= V_BACKLASH;
				m_nLooker	= VIEWY_LIMIT_AXIAL;
				if ((status = mv_motor(steps, (int) VIEWY_AXIAL, (int) INCREASE)) == NO_ERRORS)
					m_nViewYno_Axial = m_nViewYStep_Axial;
			}
		}
		else if (m_nViewYStep_Axial > m_nViewYno_Axial)		// Move forward away from HOME
		{
			// MOVE UP (OUT) - NO BACKLASH
			m_nLooker = VIEWY_LIMIT_AXIAL;
			steps  = (WORD) (m_nViewYStep_Axial - m_nViewYno_Axial);
			if ((status = mv_motor(steps, (int) VIEWY_AXIAL, (int) INCREASE)) == NO_ERRORS)
				m_nViewYno_Axial = m_nViewYStep_Axial;
		}
	    
		if (status == NO_ERRORS)
		{
			// SAVE CURRENT VALUE FOR VIEW Y
			m_NvRam.OutputW(CURR_VIEWY_AXIAL, m_nViewYno_Axial);
			// SAVE CURRENT POSITION
			m_NvRam.OutputB(CURR_VIEWY_POS_AXIAL, m_cViewYPno_Axial);
		}
		else
			dummy = cmdCY();				// Reset position - KEY
	    
		return status;
 	  }
	else  // RADIAL_TYPE
	  {	
		if (m_nViewYStep_Radial < m_nViewYno_Radial)			// Move backward toward HOME
		{
			// MOVE DOWN (IN) + BACKLASH
			m_nLooker = VIEWY_HOME_RADIAL;
			steps  = (WORD) (m_nViewYno_Radial - m_nViewYStep_Radial + V_BACKLASH);
			if ((status = mv_motor(steps, (int) VIEWY_RADIAL, (int) DECREASE)) == NO_ERRORS)
			{
				m_nViewYno_Radial = m_nViewYStep_Radial;
				// MOVE UP (OUT) - NO BACKLASH
				steps	= V_BACKLASH;
				m_nLooker	= VIEWY_LIMIT_RADIAL;
				if ((status = mv_motor(steps, (int) VIEWY_RADIAL, (int) INCREASE)) == NO_ERRORS)
					m_nViewYno_Radial = m_nViewYStep_Radial;
			}
		}
		else if (m_nViewYStep_Radial > m_nViewYno_Radial)		// Move forward away from HOME
		{
			// MOVE UP (OUT) - NO BACKLASH
			m_nLooker = VIEWY_LIMIT_RADIAL;
			steps  = (WORD) (m_nViewYStep_Radial - m_nViewYno_Radial);
			if ((status = mv_motor(steps, (int) VIEWY_RADIAL, (int) INCREASE)) == NO_ERRORS)
				m_nViewYno_Radial = m_nViewYStep_Radial;
		}
	    
		if (status == NO_ERRORS)
		{
			// SAVE CURRENT VALUE FOR VIEW Y
			m_NvRam.OutputW(CURR_VIEWY_RADIAL, m_nViewYno_Radial);
			// SAVE CURRENT POSITION
			m_NvRam.OutputB(CURR_VIEWY_POS_RADIAL, m_cViewYPno_Radial);
		}
		else
			dummy = cmdCY();				// Reset position - KEY
	    
		return status;
		
	  } // end else
}
//===========================================================================

unsigned int CParserThread::mv_motor(unsigned int steps, unsigned int drive, unsigned int direction)
{
	unsigned int	status(NO_ERRORS);
	WORD			pulse_on(0);
	WORD			pulse_off(0);
	WORD			step;
	WORD			ramp1;
	WORD			ramp2;
	WORD			rampsteps(0);

	if (drive == SLIT)
	{
		m_nMotorControl |= SLIT_ENABLE;
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		rampsteps = RAMP_SLIT;
		if (direction == INCREASE)
		{
			pulse_on  = (WORD) (m_nMotorControl | CCWON);
			pulse_off = (WORD) (m_nMotorControl | CCWOFF);
		}
		else
		{
			pulse_on  = (WORD) (m_nMotorControl | CWON);
			pulse_off = (WORD) (m_nMotorControl | CWOFF);
		}
	}
	else if (drive == VIEWX_AXIAL)
	{
		m_nMotorControl |= VIEWX_ENABLE;
		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
		rampsteps = RAMP_VIEWX;
 		if (direction == INCREASE)
		{
			pulse_on  = (WORD) (m_nMotorControl | XOUTON);
			pulse_off = (WORD) (m_nMotorControl | XOUTOFF);
		}
		else
		{
			pulse_on  = (WORD) (m_nMotorControl | XINON);
			pulse_off = (WORD) (m_nMotorControl | XINOFF);
		}
	}
	else if (drive == VIEWX_RADIAL)
	{
		m_nMotorControl |= VIEWX_ENABLE;
		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
		rampsteps = RAMP_VIEWX;
 		if (direction == INCREASE)
		{
			pulse_on  = (WORD) (m_nMotorControl | XOUTON);
			pulse_off = (WORD) (m_nMotorControl | XOUTOFF);
		}
		else
		{
			pulse_on  = (WORD) (m_nMotorControl | XINON);
			pulse_off = (WORD) (m_nMotorControl | XINOFF);
		}
	}
	
	else if (drive == VIEWY_AXIAL)
	{
		m_nMotorControl |= VIEWY_ENABLE;
		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
		rampsteps = RAMP_VIEWY;
 		if (direction == INCREASE)
		{
			pulse_on  = (WORD) (m_nMotorControl | YOUTON);
			pulse_off = (WORD) (m_nMotorControl | YOUTOFF);
		}
		else
		{
			pulse_on  = (WORD) (m_nMotorControl | YINON);
			pulse_off = (WORD) (m_nMotorControl | YINOFF);
		}
	}
	else if (drive == VIEWY_RADIAL)
	{
		m_nMotorControl |= VIEWY_ENABLE;
		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
		rampsteps = RAMP_VIEWY;
 		if (direction == INCREASE)
		{
			pulse_on  = (WORD) (m_nMotorControl | YOUTON);
			pulse_off = (WORD) (m_nMotorControl | YOUTOFF);
		}
		else
		{
			pulse_on  = (WORD) (m_nMotorControl | YINON);
			pulse_off = (WORD) (m_nMotorControl | YINOFF);
		}
	}	

	if (drive == VIEWX_AXIAL || drive == VIEWY_AXIAL)
	{
		for (step = 1; step <= steps; step++)
		{
			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, pulse_on);
			status	= delay( drive );
			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, pulse_off);
			status	= delay( drive );
			if (status != NO_ERRORS)
				return status;
		}
	}
	else if (drive == VIEWX_RADIAL || drive == VIEWY_RADIAL)
	{
		for (step = 1; step <= steps; step++)
		{
			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, pulse_on);
			status	= delay( drive );
			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, pulse_off);
			status	= delay( drive );
			if (status != NO_ERRORS)
				return status;
		}
	}	
	//  Slit motor 
	else if ((WORD) steps < (WORD) (2 * rampsteps))
	{
		ramp1 = (WORD) (steps / 2);
		if ((WORD) (2 * ramp1) != (WORD) steps)			// odd number of steps
			ramp1 += 1;
		status	= ramp_up(ramp1, pulse_on, pulse_off);
		if (status == NO_ERRORS)
		{
			ramp2 = (WORD) (steps - ramp1);
//			if (ramp2 != ramp1)
//				m_nWaitTime -= DELTA_TIME;
			status	= ramp_down(ramp2, pulse_on, pulse_off);
		}
	}
	//  Slit motor 
	else
	{
		status	= ramp_up(rampsteps, pulse_on, pulse_off);
		if (status == NO_ERRORS)
		{
			for (step = (WORD) (rampsteps + 1); step <= (steps - rampsteps); step++)
			{
				m_IO.OutputW(SLIT_MOTOR_CMD_REG, pulse_on);
				status	= delay(  SLIT  );
				m_IO.OutputW(SLIT_MOTOR_CMD_REG, pulse_off);
				status	= delay(  SLIT  );
				if (status != NO_ERRORS)
					return(status);
			}
			if (status == NO_ERRORS)
				status	= ramp_down(rampsteps, pulse_on, pulse_off);
		}
	}
	if (drive == SLIT)
	{
		m_nMotorControl &= ~SLIT_ENABLE;
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	}
	else if (drive == VIEWX_AXIAL)
	{
		m_nMotorControl &= ~VIEWX_ENABLE;
		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	}
	else if (drive == VIEWX_RADIAL)
	{
		m_nMotorControl &= ~VIEWX_ENABLE;
		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	}	
	else if (drive == VIEWY_AXIAL)
	{
		m_nMotorControl &= ~VIEWY_ENABLE;
		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	}
	else if (drive == VIEWY_RADIAL)
	{
		m_nMotorControl &= ~VIEWY_ENABLE;
		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	}	
	return status;
}
//===========================================================================
// Applies to slit motor only
unsigned int CParserThread::ramp_up(unsigned int steps, unsigned int pon, unsigned int poff)
{
	WORD	status(NO_ERRORS);
	WORD	i;

	m_nWaitTime = MAX_S_TIME;

	for (i = 1; i <= steps; i++)
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, (WORD) pon);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, (WORD) poff);
		status = delay( SLIT );
//		m_nWaitTime -= DELTA_TIME;			// Speed up linearly
		if (status != NO_ERRORS)
			return status;
	}

	return status;
}
//===========================================================================
// Applies to slit motor only
unsigned int CParserThread::ramp_down(unsigned int steps, unsigned int pon, unsigned int poff)
{
	WORD	status(NO_ERRORS);
	WORD	i;
	
	for (i = 1; i <= steps; i++)
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, (WORD) pon);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, (WORD) poff);
		status = delay( SLIT );
//		m_nWaitTime += DELTA_TIME;			// Slow down linearly
		if (status != NO_ERRORS)
			return(status);
	}
	m_nWaitTime = MAX_S_TIME;

	return status;
}
//===========================================================================

/*** Revision History ***

1/5/16    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/Motor.cpp $
 * 
 * 10    7/31/07 2:47p Nashth
 * Motor errors <LE> now returned as a specific motor error <S0>, <X0>, or
 * <Y0> as per QPR: THNJ75ML7T.
 * 
 * 9     6/29/05 13:47 Frazzitl
 * Optimized Slit Motor
 * 
 * 8     5/20/05 14:53 Frazzitl
 * Sped up motors
 * 
 * 7     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 6     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 5     4/07/05 8:02 Frazzitl
 * Fixing Slit problems
 * 
 * 4     4/05/05 14:40 Frazzitl
 * Corrected motor control
 * 
 * 3     4/05/05 3:33p Frazzitl
 * 
 * 2     4/04/05 10:44 Frazzitl
 * Fixing problem with motor timing - only temporary
 * 
 * 1     3/17/05 11:22 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
