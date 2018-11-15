//===========================================================================
//
//	Module Name:	cmdLM.cpp
//
// 	Function:	  	This routine finds the slit and view LIMIT position.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdLS()
{
	WORD	SVstatus;
	WORD	steps;
	WORD	slittemp;
	WORD	status(NO_ERRORS);

	m_nLooker = 0;
	m_nMotorControl = 0;

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "LS00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical parameters

	// CHECK FOR EITHER DETECTOR
	SVstatus  = m_IO.InputW(SVSTAT_CS);
	if ((SVstatus & SLIT_LIMIT) || (m_nSlitSno != 0))
	{
		status = ERR_SLIT_HOME;
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}

	// FIND LIMIT
	steps	  = 0;
	m_nWaitTime  = MAX_S_TIME;
	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_nMotorControl |= CCWOFF;
	slittemp = (WORD) (m_nMotorControl | CCWON);
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	while (steps < HSTP_SLIT)
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		status = delay( SLIT );
		steps += 1;			// COUNT STEPS TO LIMIT
		SVstatus = m_IO.InputW(SVSTAT_CS);
		if (SVstatus & SLIT_LIMIT)
			goto setlimit;
	}
	m_nMotorControl  = 0;
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	m_nSlitStep  = steps;
	m_nSlitSno   = steps;
	status = ERR_SLIT_LIMIT;
	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;

setlimit:
	// NO NEED TO TAKE OUT BACKLASH HERE
	m_nMaxSlitStep = steps;

	// CHECK FOR MINIMUM NUMBER OF STEPS REQUIRED
	if (steps < MIN_S_STEPS)
	{
		status = ERR_SLIT_STEPS;
		ErrorMsg((int) NO, status);
	}

	m_cSlitType = LIMIT;
	m_cSlitPos  = 0;
	m_nSlitStep = steps;
	m_cSlitTno  = LIMIT;
	m_cSlitPno  = 0;
	m_nSlitSno  = steps;

	// SAVE CURRENT VALUE FOR SLIT
	m_NvRam.OutputW(CURR_SLIT, steps);
	m_NvRam.OutputB(CURR_SLIT_TYPE, (char) LIMIT);
	m_NvRam.OutputB(CURR_SLIT_POS, 0);

	m_nMotorControl  = 0;
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);

	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

unsigned int CParserThread::cmdLX()
{
	WORD	SVstatus;
	WORD	steps;
	WORD	slittemp;
	WORD	status(NO_ERRORS);

	m_nLooker = 0;
	m_nMotorControl = 0;

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "LX00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Protect critical parameters

	// CHECK FOR EITHER DETECTOR
	SVstatus  = m_IO.InputW(SVSTAT_CS);
	m_nMotorControl |= VIEWX_ENABLE;
	
	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    if (!m_bLimitXFlag)
	    {
	    	// IF NOT SPECIAL CASE, ALLOW ONLY IF HOME
	    	if ((SVstatus & VIEWX_LIMIT_AXIAL) || (m_nViewXno_Axial != 0))
	    	{
	    		m_nMotorControl  = 0;
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = ERR_VIEWX_HOME_AXIAL;
	    		ReturnStatus((WORD)status); // Place return code into reply buffer
	    		return(status);
	    	}
	    }
        
	    // FIND LIMIT
	    steps = 0;
	    m_nWaitTime  = MAX_V_TIME;
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    m_nMotorControl |= XOUTOFF;
	    slittemp = (WORD) (m_nMotorControl | XOUTON);
	    m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    while (steps < HSTP_VIEWX)
	    {
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_AXIAL );
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_AXIAL );
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_AXIAL );
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_AXIAL );
	    	steps += 2;		// ALWAYS MOVE TWO STEPS
	    	SVstatus = m_IO.InputW(SVSTAT_CS);
	    	if (SVstatus & VIEWX_LIMIT_AXIAL)
	    	{
	    		if (!m_bLimitXFlag)
	    			goto setlimitxA;
	    		else
	    			goto limitxflagA;
	    	}
	    }
	    m_nMotorControl  = 0;
	    m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    m_nViewXStep_Axial = steps;
	    m_nViewXno_Axial   = steps;
	    status = ERR_VIEWX_LIMIT_AXIAL;
	    ReturnStatus((WORD)status); // Place return code into reply buffer
	    return status;
        
setlimitxA:
	    // NO NEED TO TAKE OUT BACKLASH HERE
	    m_nMaxXSteps_Axial = steps;
        
	    // CHECK FOR MINIMUM NUMBER OF STEPS REQUIRED
	    if (steps < MIN_X_STEPS_AXIAL)
	    {
	    	status = ERR_VIEWX_LIMIT_AXIAL;
	    	ErrorMsg((int) NO, status);
	    }
        
limitxflagA:
	    m_nViewXStep_Axial = steps;
	    m_nViewXno_Axial   = steps;

    	m_cViewXPos_Axial  = MAX_VIEW_X_AXIAL + 1;
    	m_cViewXPno_Axial  = MAX_VIEW_X_AXIAL + 1;
        
	    // SAVE CURRENT VALUE FOR VIEW X
	    m_NvRam.OutputW(CURR_VIEWX_AXIAL, steps);
	    m_NvRam.OutputB(CURR_VIEWX_POS_AXIAL, (char) (MAX_VIEW_X_AXIAL + 1));
        
	    m_nMotorControl  = 0;
	    m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    ReturnStatus((WORD)status); // Place return code into reply buffer
	    return status;
	  }
    else  // RADIAL_TYPE	  
	  {
		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    if (!m_bLimitXFlag)
	    {
	    	// IF NOT SPECIAL CASE, ALLOW ONLY IF HOME
	    	if ((SVstatus & VIEWX_LIMIT_RADIAL) || (m_nViewXno_Radial != 0))
	    	{
	    		m_nMotorControl  = 0;
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = ERR_VIEWX_HOME_RADIAL;
	    		ReturnStatus((WORD)status); // Place return code into reply buffer
	    		return(status);
	    	}
	    }
        
	    // FIND LIMIT
	    steps = 0;
	    m_nWaitTime  = MAX_V_TIME;
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    m_nMotorControl |= XOUTOFF;
	    slittemp = (WORD) (m_nMotorControl | XOUTON);
	    m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    while (steps < HSTP_VIEWX)
	    {
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_RADIAL );
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_RADIAL );
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_RADIAL );
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_RADIAL );
	    	steps += 2;		// ALWAYS MOVE TWO STEPS
	    	SVstatus = m_IO.InputW(SVSTAT_CS);
	    	if (SVstatus & VIEWX_LIMIT_RADIAL)
	    	{
	    		if (!m_bLimitXFlag)
	    			goto setlimitxR;
	    		else
	    			goto limitxflagR;
	    	}
	    }
	    m_nMotorControl  = 0;
	    m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    m_nViewXStep_Radial = steps;
	    m_nViewXno_Radial   = steps;
	    status = ERR_VIEWX_LIMIT_RADIAL;
	    ReturnStatus((WORD)status); // Place return code into reply buffer
	    return status;
        
setlimitxR:
	    // NO NEED TO TAKE OUT BACKLASH HERE
	    m_nMaxXSteps_Radial = steps;
        
	    // CHECK FOR MINIMUM NUMBER OF STEPS REQUIRED
	    if (steps < MIN_X_STEPS_RADIAL)
	    {
	    	status = ERR_VIEWX_LIMIT_RADIAL;
	    	ErrorMsg((int) NO, status);
	    }
        
limitxflagR:
	    m_nViewXStep_Radial = steps;
	    m_nViewXno_Radial   = steps;

    	m_cViewXPos_Radial  = MAX_VIEW_X_RADIAL + 1;
    	m_cViewXPno_Radial  = MAX_VIEW_X_RADIAL + 1;
        
	    // SAVE CURRENT VALUE FOR VIEW X
	    m_NvRam.OutputW(CURR_VIEWX_RADIAL, steps);
	    m_NvRam.OutputB(CURR_VIEWX_POS_RADIAL, (char) (MAX_VIEW_X_RADIAL + 1));
        
	    m_nMotorControl  = 0;
	    m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    ReturnStatus((WORD)status); // Place return code into reply buffer
	    return status;
	
	  } // end else
} // end function

//===========================================================================

unsigned int CParserThread::cmdLY()
{
	WORD	SVstatus;
	WORD	steps;
	WORD	slittemp;
	WORD	status(NO_ERRORS);

	m_nLooker = 0;
	m_nMotorControl = 0;

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical data
	strcpy(m_nDataOutBuf, "LY00");
	theApp.LeaveCriticalSection1(&m_CriticalSection);   // Remove protection

	// CHECK FOR EITHER DETECTOR
	SVstatus  = m_IO.InputW(SVSTAT_CS);
	m_nMotorControl |= VIEWY_ENABLE;

	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	if (!m_bLimitYFlag)
    	{
    		// IF NOT SPECIAL CASE, ALLOW ONLY IF HOME
    		if ((SVstatus & VIEWY_LIMIT_AXIAL) || (m_nViewYno_Axial != 0))
    		{
    			m_nMotorControl  = 0;
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = ERR_VIEWY_HOME_AXIAL;
    
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    	}
    
    	// FIND LIMIT
    	steps	  = 0;
    	m_nWaitTime  = MAX_V_TIME;
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YOUTOFF;
    	slittemp = (WORD) (m_nMotorControl | YOUTON);
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	while (steps < HSTP_VIEWY)
    	{
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_AXIAL );
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_AXIAL );
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_AXIAL );
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_AXIAL );
    		steps += 2;		// ALWAYS MOVE TWO STEPS
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    		if (SVstatus & VIEWY_LIMIT_AXIAL)
    		{
    			if (!m_bLimitYFlag)
    				goto setlimityA;
    			else
    				goto limityflagA;
    		}
    	}
    	m_nMotorControl  = 0;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	m_nViewYStep_Axial = steps;
    	m_nViewYno_Axial   = steps;
    	status = ERR_VIEWY_LIMIT_AXIAL;
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
    
setlimityA:
    	// NO NEED TO TAKE OUT BACKLASH HERE
    	m_nMaxYSteps_Axial = steps;
    
    	// CHECK FOR MINIMUM NUMBER OF STEPS REQUIRED
    	if (steps < MIN_Y_STEPS_AXIAL)
    	{
    		status = ERR_VIEWY_LIMIT_AXIAL;
    		ErrorMsg((int) NO, status);
    	}
    
limityflagA:
    	m_nViewYStep_Axial = steps;
    	m_nViewYno_Axial   = steps;

		m_cViewYPos_Axial  = MAX_VIEW_Y_AXIAL + 1;
   		m_cViewYPno_Axial  = MAX_VIEW_Y_AXIAL + 1;
    
    	// SAVE CURRENT VALUE FOR VIEW Y
    	m_NvRam.OutputW(CURR_VIEWY_AXIAL, steps);
    	m_NvRam.OutputB(CURR_VIEWY_POS_AXIAL, (char) (MAX_VIEW_Y_AXIAL + 1));
    
    	m_nMotorControl  = 0;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
 	  }
	else  // RADIAL_TYPE
	  {	
		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	if (!m_bLimitYFlag)
    	{
    		// IF NOT SPECIAL CASE, ALLOW ONLY IF HOME
    		if ((SVstatus & VIEWY_LIMIT_RADIAL) || (m_nViewYno_Radial != 0))
    		{
    			m_nMotorControl  = 0;
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = ERR_VIEWY_HOME_RADIAL;
    
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    	}
    
    	// FIND LIMIT
    	steps	  = 0;
    	m_nWaitTime  = MAX_V_TIME;
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YOUTOFF;
    	slittemp = (WORD) (m_nMotorControl | YOUTON);
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	while (steps < HSTP_VIEWY)
    	{
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_RADIAL);
    		steps += 2;		// ALWAYS MOVE TWO STEPS
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    		if (SVstatus & VIEWY_LIMIT_RADIAL)
    		{
    			if (!m_bLimitYFlag)
    				goto setlimityR;
    			else
    				goto limityflagR;
    		}
    	}
    	m_nMotorControl  = 0;
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	m_nViewYStep_Radial = steps;
    	m_nViewYno_Radial   = steps;
    	status = ERR_VIEWY_LIMIT_RADIAL;
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
    
setlimityR:
    	// NO NEED TO TAKE OUT BACKLASH HERE
    	m_nMaxYSteps_Radial = steps;
    
    	// CHECK FOR MINIMUM NUMBER OF STEPS REQUIRED
    	if (steps < MIN_Y_STEPS_RADIAL)
    	{
    		status = ERR_VIEWY_LIMIT_RADIAL;
    		ErrorMsg((int) NO, status);
    	}
    
limityflagR:
    	m_nViewYStep_Radial = steps;
    	m_nViewYno_Radial   = steps;

   		m_cViewYPos_Radial  = MAX_VIEW_Y_RADIAL + 1;
   		m_cViewYPno_Radial  = MAX_VIEW_Y_RADIAL + 1;
    
    	// SAVE CURRENT VALUE FOR VIEW Y
    	m_NvRam.OutputW(CURR_VIEWY_RADIAL, steps);
    	m_NvRam.OutputB(CURR_VIEWY_POS_RADIAL, (char) (MAX_VIEW_Y_RADIAL + 1));
    
    	m_nMotorControl  = 0;
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
	  
	  } // end else		
}  // end function

//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/cmdlm.cpp $
 * 
 * 13    7/31/07 2:47p Nashth
 * Motor errors <LE> now returned as a specific motor error <S0>, <X0>, or
 * <Y0> as per QPR: THNJ75ML7T.
 * 
 * 12    6/26/07 3:46p Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
 * 
 * 11    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 10    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 9     6/29/05 13:46 Frazzitl
 * Optimized Slit Motor
 * 
 * 8     5/20/05 14:53 Frazzitl
 * Sped up motors
 * 
 * 7     5/20/05 8:50a Frazzitl
 * Fixing motor problems
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
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
