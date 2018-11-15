//===========================================================================
//
//	Module Name:	cmdCM.cpp
//
//	Function:		This routine configures the slit and view motors.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================
//
// Description:
//	Module Name:	cmdCM
//	Function:		This routine configures the slit and view motors.
//
// Author:
//	Thomas L. Frazzini
//
// Version Data:
//	Level:	4.31
//	Date:	01-01-97
//

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdCS()
{
	unsigned int	status(NO_ERRORS);
	unsigned int	SVstatus;
	unsigned short	steps;
	unsigned int	i;
	unsigned short	slittemp;
	bool			bSetSlit;		// True = Set the slit
	bool			bMoveOffHome;	// True = Move one step from home 

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "CS00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection 

	SVstatus  = m_IO.InputW(SVSTAT_CS);
	m_nMotorControl = 0;

	// FIND HOME
	if ((status = cmdHS()) != NO_ERRORS)  //  Critical section in cmdHS
	{
		m_nMotorControl = 0;
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "CS00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection 

	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_nMotorControl |= CCWOFF;
	slittemp = (unsigned short) (m_nMotorControl | CCWON);
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);

	// FIND LIMIT
	steps = (unsigned short) (m_nMaxSlitStep - 50);
	m_nLooker = SLIT_LIMIT;
	
	if ((status = mv_motor(steps, (int) SLIT, (int) INCREASE)) != NO_ERRORS) // Critical section in mv_motor
	{
		m_nMotorControl  = 0;
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}
	if (!(SVstatus & SLIT_LIMIT))
	{
		m_nWaitTime  = MAX_S_TIME;
		m_nMotorControl  = 0;
		m_nMotorControl |= SLIT_ENABLE;
		m_nMotorControl |= CCWOFF;
		slittemp = (unsigned short) (m_nMotorControl | CCWON);
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		bSetSlit = false;  
		while (steps < m_nMaxSlitStep + 50)
		{
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
			status = delay( SLIT );
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
			status = delay( SLIT );
			steps += 1;	// ONLY TAKE ONE STEP
			SVstatus = m_IO.InputW(SVSTAT_CS);
			if (SVstatus & SLIT_LIMIT)
			{	
				bSetSlit = true;  // Normal exit ... Set the slit
				break;   
			}
		}
		if ( ! bSetSlit )				// Error exit
		{
			m_nMotorControl  = 0;
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
			status	  = ERR_SLIT_STEPS;
			ReturnStatus((WORD)status); // Place return code into reply buffer
			return status;
		}
	}

//  setslit: Set the slit ... MOVE BACK TO HOME
	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	steps = (unsigned short) (m_nMaxSlitStep - 50);
	m_nLooker = SLIT_HOME;
	if ((status = mv_motor(steps, (int) SLIT, (int) DECREASE)) != NO_ERRORS)  // Critical section in mv_motor
	{
		m_nMotorControl  = 0;
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
	}
	if (!(SVstatus & SLIT_HOME))
	{
		m_nWaitTime  = MAX_S_TIME;
		m_nMotorControl  = 0;
		m_nMotorControl |= SLIT_ENABLE;
		m_nMotorControl |= CWOFF;
		slittemp = (unsigned short) (m_nMotorControl | CWON);
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		bMoveOffHome = false;
		while (steps < m_nMaxSlitStep + 50)
		{
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
			status = delay( SLIT );
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
			status = delay( SLIT );
			steps += 1;	// ONLY TAKE ONE STEP
			SVstatus = m_IO.InputW(SVSTAT_CS);
			if (SVstatus & SLIT_HOME)
			{
				for (i = 1; i <= S_BACKLASH; i++) // EVEN NUMBER OF STEPS
				{
					m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
					status = delay( SLIT );
					m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
					status = delay( SLIT );
				}
				bMoveOffHome = true;
				break;
			}
		}
		if ( ! bMoveOffHome )    // Error exit
		{
			m_nMotorControl  = 0;
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
			status	  = ERR_SLIT_STEPS;
			ReturnStatus((WORD)status); // Place return code into reply buffer
			return status;
		}
	}

// offhome:	Move off the home position ... GET TO FIRST STEP PASSED HOME
	m_nMotorControl  = 0;
	m_nMotorControl |= SLIT_ENABLE;
	m_nMotorControl |= CCWOFF;
	slittemp = (unsigned short) (m_nMotorControl | CCWON);
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	while (SVstatus & SLIT_HOME)
	{
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
		status = delay( SLIT );
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		status = delay( SLIT );
		SVstatus = m_IO.InputW(SVSTAT_CS);
	}
	// MOVE TO DEFAULT POSITION - LOW RESOLUTION, POSITION 0
	m_nSlitStep = m_nSlitLow;
	m_cSlitType = LOWRES;
	m_cSlitTno	 = LOWRES;
	m_cSlitPos  = 0;
	m_cSlitPno  = 0;
	status	 = mv_slit();   // Critical section in mv_slit

	m_nMotorControl  = 0;
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//-----------------------------------------------------------------------------------
unsigned int CParserThread::cmdCX()
{
	unsigned int	status;
	unsigned int	SVstatus;
	unsigned short	steps;
	unsigned int	i;
	unsigned short	slittemp;
	
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters 
	strcpy(m_nDataOutBuf, "CX00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection 

	SVstatus  = m_IO.InputW(SVSTAT_CS);
	m_nMotorControl = 0;

	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
	    // FIND HOME
	    if ((status = cmdHX()) != NO_ERRORS)
	    {
	    	// FIND LIMIT BY SINGLE STEPS
	    	m_bLimitXFlag = true;
	    	if ((status = cmdLX()) != NO_ERRORS)
	    	{
	    		m_bLimitXFlag = false;
	    		m_nMotorControl  = 0;
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		ReturnStatus((WORD)status); // Place return code into reply buffer
	    		return status;
	    	}
	    	m_bLimitXFlag = false;
	    	if ((status = cmdHX()) != NO_ERRORS)
	    	{
	    		m_nMotorControl  = 0;
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		ReturnStatus((WORD)status); // Place return code into reply buffer
	    		return status;
	    	}
	    }
        
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    m_nMotorControl |= XINOFF;
	    slittemp = (unsigned short) (m_nMotorControl | XINON);
	    m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
        
	    // FIND LIMIT
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    steps = (unsigned short) (m_nMaxXSteps_Axial - 20);
	    m_nLooker	  = VIEWX_LIMIT_AXIAL;
	    if ((status = mv_motor(steps, (int) VIEWX_AXIAL, (int) INCREASE)) != NO_ERRORS)
	    {
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    if (!(SVstatus & VIEWX_LIMIT_AXIAL))
	    {
	    	m_nWaitTime  = MAX_V_TIME;
	    	m_nMotorControl |= XOUTOFF;
	    	slittemp = (unsigned short) (m_nMotorControl | XOUTON);
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	while (steps < m_nMaxXSteps_Axial + 20)
	    	{
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_AXIAL );
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_AXIAL );
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_AXIAL );
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_AXIAL );
	    		steps += 2;	// ALWAYS MOVE TWO STEPS
	    		SVstatus = m_IO.InputW(SVSTAT_CS);
	    		if (SVstatus & VIEWX_LIMIT_AXIAL)
	    			goto setxA;
	    	}
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status	  = ERR_VIEWX_STEPS_AXIAL;
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
        
setxA:		// MOVE BACK TO HOME
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    steps = (unsigned short) (m_nMaxXSteps_Axial - 20);
	    m_nLooker = VIEWX_HOME_AXIAL;
	    if ((status = mv_motor(steps, (int) VIEWX_AXIAL, (int) DECREASE)) != NO_ERRORS)
	    {
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    if (!(SVstatus & VIEWX_HOME_AXIAL))
	    {
	    	m_nWaitTime  = MAX_V_TIME;
	    	m_nMotorControl |= XINOFF;
	    	slittemp = (unsigned short) (m_nMotorControl | XINON);
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	while (steps < m_nMaxXSteps_Axial + 20)
	    	{
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_AXIAL );
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_AXIAL );
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_AXIAL );
	    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_AXIAL );
	    		steps += 2;	// ALWAYS MOVE TWO STEPS
	    		SVstatus = m_IO.InputW(SVSTAT_CS);
	    		if (SVstatus & VIEWX_HOME_AXIAL)
	    		{
	    			for (i = 1; i <= V_BACKLASH; i++) // EVEN NUMBER OF STEPS
	    			{
	    				m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    				status = delay( VIEWX_AXIAL );
	    				m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    				status = delay( VIEWX_AXIAL );
	    				m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    				status = delay( VIEWX_AXIAL );
	    				m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    				status = delay( VIEWX_AXIAL );
	    			}
	    			goto offxhomeA;
	    		}
	    	}
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status	  = ERR_VIEWX_STEPS_AXIAL;
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
        
offxhomeA:	// GET TO FIRST STEP PASSED HOME
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    m_nMotorControl |= XOUTOFF;
	    slittemp = (unsigned short) (m_nMotorControl | XOUTON);
	    m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    while (SVstatus & VIEWX_HOME_AXIAL)
	    {
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_AXIAL );
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_AXIAL );
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_AXIAL );
	    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_AXIAL );
	    	SVstatus = m_IO.InputW(SVSTAT_CS);
	    }
	    // MOVE TO DEFAULT POSITION - XPEAK

    	m_nViewXStep_Axial = m_nViewXPeak_Axial;
    	m_cViewXPos_Axial  = XPEAK_AXIAL;
    	m_cViewXPno_Axial  = XPEAK_AXIAL;

	    status	  = mv_viewx();
        
	    m_nMotorControl  = 0;
	    m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
	    ReturnStatus((WORD)status); // Place return code into reply buffer
	    return status;
 	  }
	else  // RADIAL_TYPE
	  {
	    // FIND HOME
	    if ((status = cmdHX()) != NO_ERRORS)
	    {
	    	// FIND LIMIT BY SINGLE STEPS
	    	m_bLimitXFlag = true;
	    	if ((status = cmdLX()) != NO_ERRORS)
	    	{
	    		m_bLimitXFlag = false;
	    		m_nMotorControl  = 0;
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		ReturnStatus((WORD)status); // Place return code into reply buffer
	    		return status;
	    	}
	    	m_bLimitXFlag = false;
	    	if ((status = cmdHX()) != NO_ERRORS)
	    	{
	    		m_nMotorControl  = 0;
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		ReturnStatus((WORD)status); // Place return code into reply buffer
	    		return status;
	    	}
	    }
        
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    m_nMotorControl |= XINOFF;
	    slittemp = (unsigned short) (m_nMotorControl | XINON);
	    m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
        
	    // FIND LIMIT
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    steps = (unsigned short) (m_nMaxXSteps_Radial - 20);
	    m_nLooker	  = VIEWX_LIMIT_RADIAL;
	    if ((status = mv_motor(steps, (int) VIEWX_RADIAL, (int) INCREASE)) != NO_ERRORS)
	    {
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    if (!(SVstatus & VIEWX_LIMIT_RADIAL))
	    {
	    	m_nWaitTime  = MAX_V_TIME;
	    	m_nMotorControl |= XOUTOFF;
	    	slittemp = (unsigned short) (m_nMotorControl | XOUTON);
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	while (steps < m_nMaxXSteps_Radial + 20)
	    	{
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_RADIAL );
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_RADIAL );
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_RADIAL );
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_RADIAL );
	    		steps += 2;	// ALWAYS MOVE TWO STEPS
	    		SVstatus = m_IO.InputW(SVSTAT_CS);
	    		if (SVstatus & VIEWX_LIMIT_RADIAL)
	    			goto setxR;
	    	}
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status	  = ERR_VIEWX_STEPS_RADIAL;
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
        
setxR:		// MOVE BACK TO HOME
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    steps = (unsigned short) (m_nMaxXSteps_Radial - 20);
	    m_nLooker = VIEWX_HOME_RADIAL;
	    if ((status = mv_motor(steps, (int) VIEWX_RADIAL, (int) DECREASE)) != NO_ERRORS)
	    {
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    if (!(SVstatus & VIEWX_HOME_RADIAL))
	    {
	    	m_nWaitTime  = MAX_V_TIME;
	    	m_nMotorControl |= XINOFF;
	    	slittemp = (unsigned short) (m_nMotorControl | XINON);
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	while (steps < m_nMaxXSteps_Radial + 20)
	    	{
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_RADIAL );
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_RADIAL );
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    		status = delay( VIEWX_RADIAL );
	    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    		status = delay( VIEWX_RADIAL );
	    		steps += 2;	// ALWAYS MOVE TWO STEPS
	    		SVstatus = m_IO.InputW(SVSTAT_CS);
	    		if (SVstatus & VIEWX_HOME_RADIAL)
	    		{
	    			for (i = 1; i <= V_BACKLASH; i++) // EVEN NUMBER OF STEPS
	    			{
	    				m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    				status = delay( VIEWX_RADIAL );
	    				m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    				status = delay( VIEWX_RADIAL );
	    				m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    				status = delay( VIEWX_RADIAL );
	    				m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    				status = delay( VIEWX_RADIAL );
	    			}
	    			goto offxhomeR;
	    		}
	    	}
	    	m_nMotorControl  = 0;
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status	  = ERR_VIEWX_STEPS_RADIAL;
	    	ReturnStatus((WORD)status); // Place return code into reply buffer
	    	return status;
	    }
        
offxhomeR:	// GET TO FIRST STEP PASSED HOME
	    m_nMotorControl  = 0;
	    m_nMotorControl |= VIEWX_ENABLE;
	    m_nMotorControl |= XOUTOFF;
	    slittemp = (unsigned short) (m_nMotorControl | XOUTON);
	    m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    while (SVstatus & VIEWX_HOME_RADIAL)
	    {
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_RADIAL );
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_RADIAL );
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
	    	status = delay( VIEWX_RADIAL );
	    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    	status = delay( VIEWX_RADIAL );
	    	SVstatus = m_IO.InputW(SVSTAT_CS);
	    }
	    // MOVE TO DEFAULT POSITION - XPEAK

	   	m_nViewXStep_Radial = m_nViewXPeak_Radial;
	   	m_cViewXPos_Radial  = XPEAK_RADIAL;
	   	m_cViewXPno_Radial  = XPEAK_RADIAL;

	    status	  = mv_viewx();
        
	    m_nMotorControl  = 0;
	    m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
	    ReturnStatus((WORD)status); // Place return code into reply buffer
	    return status;
		
	  } // end else
} // end function


unsigned int CParserThread::cmdCY()
{
	unsigned int	status;
	unsigned int	SVstatus;
	unsigned short	steps;
	unsigned int	i;
	unsigned short	slittemp;
	
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "CY00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection

	SVstatus  = m_IO.InputW(SVSTAT_CS);
	m_nMotorControl = 0;
	
	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
     	// FIND HOME
    	if ((status = cmdHY()) != NO_ERRORS)
    	{
    		// FIND LIMIT BY SINGLE STEPS
    		m_bLimitYFlag = true;
    		if ((status = cmdLY()) != NO_ERRORS)
    		{
    			m_bLimitYFlag = false;
    			m_nMotorControl  = 0;
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    		m_bLimitYFlag = false;
    		if ((status = cmdHY()) != NO_ERRORS)
    		{
    			m_nMotorControl  = 0;
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    	}
    
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YINOFF;
    	slittemp = (unsigned short) (m_nMotorControl | YINON);
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    
    	// FIND LIMIT
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	steps = (unsigned short) (m_nMaxYSteps_Axial - 20);
    	m_nLooker	  = VIEWY_LIMIT_AXIAL;
    	if ((status = mv_motor(steps, (int) VIEWY_AXIAL, (int) INCREASE)) != NO_ERRORS)
    	{
    		m_nMotorControl  = 0;
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	if (!(SVstatus & VIEWY_LIMIT_AXIAL))
    	{
    		m_nWaitTime  = MAX_V_TIME;
    		m_nMotorControl |= YOUTOFF;
    		slittemp = (unsigned short) (m_nMotorControl | YOUTON);
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		while (steps < m_nMaxYSteps_Axial + 20)
    		{
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_AXIAL );
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_AXIAL );
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_AXIAL );
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_AXIAL );
    			steps += 2;	// ALWAYS MOVE TWO STEPS
    			SVstatus = m_IO.InputW(SVSTAT_CS);
    			if (SVstatus & VIEWY_LIMIT_AXIAL)
    				goto setyA;
    		}
    		m_nMotorControl  = 0;
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status	  = ERR_VIEWY_STEPS_AXIAL;
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    
setyA:		// MOVE BACK TO HOME
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	steps = (unsigned short) (m_nMaxYSteps_Axial - 20);
    	m_nLooker	  = VIEWY_HOME_AXIAL;
    	if ((status = mv_motor(steps, (int) VIEWY_AXIAL, (int) DECREASE)) != NO_ERRORS)
    	{
    		m_nMotorControl  = 0;
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	if (!(SVstatus & VIEWY_HOME_AXIAL))
    	{
    		m_nWaitTime  = MAX_V_TIME;
    		m_nMotorControl |= YINOFF;
    		slittemp = (unsigned short) (m_nMotorControl | YINON);
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		while (steps < m_nMaxYSteps_Axial + 20)
    		{
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_AXIAL );
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_AXIAL );
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_AXIAL );
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_AXIAL );
    			steps += 2;	// ALWAYS MOVE TWO STEPS
    			SVstatus = m_IO.InputW(SVSTAT_CS);
    			if (SVstatus & VIEWY_HOME_AXIAL)
    			{
    				for (i = 1; i <= V_BACKLASH; i++) // EVEN NUMBER OF STEPS
    				{
    					m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    					status = delay( VIEWY_AXIAL );
    					m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    					status = delay( VIEWY_AXIAL );
    					m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    					status = delay( VIEWY_AXIAL );
    					m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    					status = delay( VIEWY_AXIAL );
    				}
    				goto offyhomeA;
    			}
    		}
    		m_nMotorControl  = 0;
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status	  = ERR_VIEWY_STEPS_AXIAL;
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    
offyhomeA:	// GET TO FIRST STEP PASSED HOME
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YOUTOFF;
    	slittemp = (unsigned short) (m_nMotorControl | YOUTON);
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	while (SVstatus & VIEWY_HOME_AXIAL)
    	{
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_AXIAL );
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_AXIAL );
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_AXIAL );
    		m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_AXIAL );
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    	}
    	// MOVE TO DEFAULT POSITION - Y15MM

    	m_nViewYStep_Axial = m_nViewY15mm_Axial;
    	m_cViewYPos_Axial  = Y15MM_AXIAL;
    	m_cViewYPno_Axial  = Y15MM_AXIAL;

    	status	  = mv_viewy();
    
    	m_nMotorControl  = 0;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
 	  }
	else  // RADIAL_TYPE
	  {	
     	// FIND HOME
    	if ((status = cmdHY()) != NO_ERRORS)
    	{
    		// FIND LIMIT BY SINGLE STEPS
    		m_bLimitYFlag = true;
    		if ((status = cmdLY()) != NO_ERRORS)
    		{
    			m_bLimitYFlag = false;
    			m_nMotorControl  = 0;
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    		m_bLimitYFlag = false;
    		if ((status = cmdHY()) != NO_ERRORS)
    		{
    			m_nMotorControl  = 0;
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    	}
    
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YINOFF;
    	slittemp = (unsigned short) (m_nMotorControl | YINON);
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    
    	// FIND LIMIT
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	steps = (unsigned short) (m_nMaxYSteps_Radial - 20);
    	m_nLooker	  = VIEWY_LIMIT_RADIAL;
    	if ((status = mv_motor(steps, (int) VIEWY_RADIAL, (int) INCREASE)) != NO_ERRORS)
    	{
    		m_nMotorControl  = 0;
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	if (!(SVstatus & VIEWY_LIMIT_RADIAL))
    	{
    		m_nWaitTime  = MAX_V_TIME;
    		m_nMotorControl |= YOUTOFF;
    		slittemp = (unsigned short) (m_nMotorControl | YOUTON);
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		while (steps < m_nMaxYSteps_Radial + 20)
    		{
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_RADIAL );
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_RADIAL );
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_RADIAL );
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_RADIAL );
    			steps += 2;	// ALWAYS MOVE TWO STEPS
    			SVstatus = m_IO.InputW(SVSTAT_CS);
    			if (SVstatus & VIEWY_LIMIT_RADIAL)
    				goto setyR;
    		}
    		m_nMotorControl  = 0;
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status	  = ERR_VIEWY_STEPS_RADIAL;
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    
setyR:		// MOVE BACK TO HOME
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	steps = (unsigned short) (m_nMaxYSteps_Radial - 20);
    	m_nLooker	  = VIEWY_HOME_RADIAL;
    	if ((status = mv_motor(steps, (int) VIEWY_RADIAL, (int) DECREASE)) != NO_ERRORS)
    	{
    		m_nMotorControl  = 0;
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	if (!(SVstatus & VIEWY_HOME_RADIAL))
    	{
    		m_nWaitTime  = MAX_V_TIME;
    		m_nMotorControl |= YINOFF;
    		slittemp = (unsigned short) (m_nMotorControl | YINON);
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		while (steps < m_nMaxYSteps_Radial + 20)
    		{
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_RADIAL );
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_RADIAL );
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    			status = delay( VIEWY_RADIAL );
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = delay( VIEWY_RADIAL );
    			steps += 2;	// ALWAYS MOVE TWO STEPS
    			SVstatus = m_IO.InputW(SVSTAT_CS);
    			if (SVstatus & VIEWY_HOME_RADIAL)
    			{
    				for (i = 1; i <= V_BACKLASH; i++) // EVEN NUMBER OF STEPS
    				{
    					m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    					status = delay( VIEWY_RADIAL );
    					m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    					status = delay( VIEWY_RADIAL );
    					m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    					status = delay( VIEWY_RADIAL );
    					m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    					status = delay( VIEWY_RADIAL );
    				}
    				goto offyhomeR;
    			}
    		}
    		m_nMotorControl  = 0;
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status	  = ERR_VIEWY_STEPS_RADIAL;
    		ReturnStatus((WORD)status); // Place return code into reply buffer
    		return status;
    	}
    
offyhomeR:	// GET TO FIRST STEP PASSED HOME
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YOUTOFF;
    	slittemp = (unsigned short) (m_nMotorControl | YOUTON);
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	while (SVstatus & VIEWY_HOME_RADIAL)
    	{
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_RADIAL );
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    	}
    	// MOVE TO DEFAULT POSITION - Y15MM

    	m_nViewYStep_Radial = m_nViewY15mm_Radial;
    	m_cViewYPos_Radial  = Y15MM_RADIAL;
    	m_cViewYPno_Radial  = Y15MM_RADIAL;

    	status	  = mv_viewy();
    
    	m_nMotorControl  = 0;
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
		
	  } // end else
  } // end function	  
  
//-----------------------------------------------------------------------------
//  Reverse the byte endians of a word and place it into position 2 and 3 of
//  the reply string m_nDataOutBuf.
//
VOID CParserThread::ReturnStatus(WORD wStatus)
{
	WORD	wRevStatus = (WORD) REVWORD(wStatus);
	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	memcpy(&m_nDataOutBuf[2], &wRevStatus, sizeof(WORD));
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection 
}
//===========================================================================

/*** Revision History ***

12/15/15    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/cmdcm.cpp $
 * 
 * 14    7/31/07 2:47p Nashth
 * Motor errors <LE> now returned as a specific motor error <S0>, <X0>, or
 * <Y0> as per QPR: THNJ75ML7T.
 * 
 * 13    6/26/07 3:46p Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
 * 
 * 12    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 11    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 10    6/29/05 13:44 Frazzitl
 * Optimized Slit Motor
 * 
 * 9     5/20/05 14:53 Frazzitl
 * Sped up motors
 * 
 * 8     5/20/05 8:50a Frazzitl
 * Fixing motor problems
 * 
 * 7     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 6     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 5     4/14/05 9:58 Frazzitl
 * Slight problem with slit
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
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
