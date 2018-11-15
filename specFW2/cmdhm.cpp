//===========================================================================
//
//	Module Name:	cmdHM.cpp
//
// 	Function:	  	This routine finds the slit and view HOME position.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdHS()
{
	WORD	SVstatus;
	WORD	steps;
	WORD	i;
	WORD	slittemp;
	WORD	status(NO_ERRORS);
	bool	bSetHome;			// True = perform set home 

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
	strcpy(m_nDataOutBuf, "HS00");
	theApp.LeaveCriticalSection1(&m_CriticalSection);  // Release protection
	
	m_nLooker = 0;
	m_nMotorControl = 0;

	// SAVE CURRENT VALUE FOR SLIT
	m_NvRam.OutputW(CURR_SLIT, 0);
	m_NvRam.OutputB(CURR_SLIT_TYPE, HOME);
	m_NvRam.OutputB(CURR_SLIT_POS, 0);

	m_cSlitType = HOME;
	m_cSlitPos  = 0;
	m_nSlitStep = 0;
	m_cSlitTno  = HOME;
	m_cSlitPno  = 0;
	m_nSlitSno  = 0;

	// CHECK FOR EITHER DETECTOR
	bSetHome = false;
	SVstatus  = m_IO.InputW(SVSTAT_CS);
	if ((SVstatus & SLIT_HOME) || (SVstatus & SLIT_LIMIT))
	{
		if ((SVstatus & SLIT_HOME) && (SVstatus & SLIT_LIMIT))
		{
			status = ERR_SLIT_LOOKERS;
			ReturnStatus((WORD)status); // Place return code into reply buffer
			return status;
		}
		else
		{
			if (SVstatus & SLIT_HOME)
				bSetHome = true;
		}
	}

	if ( ! bSetHome )
	{
		// FIND HOME
		steps = 0;
		m_nWaitTime  = MAX_S_TIME;
		m_nMotorControl  = 0;
		m_nMotorControl |= SLIT_ENABLE;
		m_nMotorControl |= CWOFF;
		slittemp = (WORD) (m_nMotorControl | CWON);
		m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
		while (steps < HSTP_SLIT)
		{
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, slittemp);
			status = delay( SLIT );
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
			status = delay( SLIT );
			steps += 1;			// ONLY MOVE ONE STEP
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
				bSetHome = true;
				break;
			}
		}
		if ( ! bSetHome )    // Error exit
		{
			m_nMotorControl  = 0;
			m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
			status = ERR_SLIT_HOME;
			ReturnStatus((WORD)status); // Place return code into reply buffer
			return status;
		}
	}
//  sethome: ... Goto First Step Passed Home ...  GET TO FIRST STEP PASSED HOME
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
		SVstatus = m_IO.InputW(SVSTAT_CS);
	}
	m_nMotorControl  = 0;
	m_IO.OutputW(SLIT_MOTOR_CMD_REG, m_nMotorControl);
	ReturnStatus((WORD)status); // Place return code into reply buffer
	return status;
}
//===========================================================================

unsigned int CParserThread::cmdHX()
{
	WORD	SVstatus;
	WORD	steps;
	WORD	i;
	WORD	slittemp;
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical data
	strcpy(m_nDataOutBuf, "HX00");
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection

	m_nLooker = 0;
	m_nMotorControl = 0;

	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
     	// SAVE CURRENT VALUE FOR VIEW X
    	m_NvRam.OutputW(CURR_VIEWX_AXIAL, 0);
    	m_NvRam.OutputB(CURR_VIEWX_POS_AXIAL, 0);
    	
    	m_nViewXStep_Axial = 0;
    	m_nViewXno_Axial   = 0;
    	m_cViewXPos_Axial  = 0;
    
    	// CHECK FOR EITHER DETECTOR
    	SVstatus  = m_IO.InputW(SVSTAT_CS);
    	m_nMotorControl |= VIEWX_ENABLE;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	if ((SVstatus & VIEWX_HOME_AXIAL) || (SVstatus & VIEWX_LIMIT_AXIAL))
    	{
    		if ((SVstatus & VIEWX_HOME_AXIAL) && (SVstatus & VIEWX_LIMIT_AXIAL))
    		{
    			m_nMotorControl  = 0;
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = ERR_VIEWX_LOOKERS_AXIAL;
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    		else
    		{
    			if (SVstatus & VIEWX_HOME_AXIAL)
    				goto sethomexA;
    		}
    	}
    
    	// FIND HOME
    	steps = 0;
    	m_nWaitTime  = MAX_V_TIME;
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWX_ENABLE;
    	m_nMotorControl |= XINOFF;
    	slittemp = (WORD) (m_nMotorControl | XINON);
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
    		steps += 2;			// ALWAYS MOVE TWO STEPS
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    		if (SVstatus & VIEWX_HOME_AXIAL)
    		{
    			for (i = 1; i <= V_BACKLASH; i++) // ALWAYS EVEN
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
    			goto sethomexA;
    		}
    	}
    	m_nMotorControl  = 0;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	status = ERR_VIEWX_HOME_AXIAL;
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
    
sethomexA:	// GET TO FIRST STEP PASSED HOME
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWX_ENABLE;
    	m_nMotorControl |= XOUTOFF;
    	slittemp = (WORD) (m_nMotorControl | XOUTON);
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
    	m_nMotorControl  = 0;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	ReturnStatus((WORD)status); // Place return code into reply buffer
		return status;
      }	
    else  // RADIAL_TYPE
	  {		
        // SAVE CURRENT VALUE FOR VIEW X
        m_NvRam.OutputW(CURR_VIEWX_RADIAL, 0);
        m_NvRam.OutputB(CURR_VIEWX_POS_RADIAL, 0);
        
        m_nViewXStep_Radial = 0;
        m_nViewXno_Radial   = 0;
        m_cViewXPos_Radial  = 0;
        
        // CHECK FOR EITHER DETECTOR
        SVstatus  = m_IO.InputW(SVSTAT_CS);
        m_nMotorControl |= VIEWX_ENABLE;
        m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
        if ((SVstatus & VIEWX_HOME_RADIAL) || (SVstatus & VIEWX_LIMIT_RADIAL))
        {
        	if ((SVstatus & VIEWX_HOME_RADIAL) && (SVstatus & VIEWX_LIMIT_RADIAL))
        	{
        		m_nMotorControl  = 0;
        		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
        		status = ERR_VIEWX_LOOKERS_RADIAL;
        		ReturnStatus((WORD)status); // Place return code into reply buffer
        		return status;
        	}
        	else
        	{
        		if (SVstatus & VIEWX_HOME_RADIAL)
        			goto sethomexR;
        	}
        }
        
        // FIND HOME
        steps = 0;
        m_nWaitTime  = MAX_V_TIME;
        m_nMotorControl  = 0;
        m_nMotorControl |= VIEWX_ENABLE;
        m_nMotorControl |= XINOFF;
        slittemp = (WORD) (m_nMotorControl | XINON);
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
        	steps += 2;			// ALWAYS MOVE TWO STEPS
        	SVstatus = m_IO.InputW(SVSTAT_CS);
        	if (SVstatus & VIEWX_HOME_RADIAL)
        	{
        		for (i = 1; i <= V_BACKLASH; i++) // ALWAYS EVEN
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
        		goto sethomexR;
        	}
        }
        m_nMotorControl  = 0;
        m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
        status = ERR_VIEWX_HOME_RADIAL;
        ReturnStatus((WORD)status); // Place return code into reply buffer
        return status;
        
sethomexR:	// GET TO FIRST STEP PASSED HOME
        m_nMotorControl  = 0;
        m_nMotorControl |= VIEWX_ENABLE;
        m_nMotorControl |= XOUTOFF;
        slittemp = (WORD) (m_nMotorControl | XOUTON);
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
        m_nMotorControl  = 0;
        m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
        ReturnStatus((WORD)status); // Place return code into reply buffer
        return status;
      } // end else
		
} // end function

//===========================================================================

unsigned int CParserThread::cmdHY()
{
	WORD	SVstatus;
	WORD	steps;
	WORD	i;
	WORD	slittemp;
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical data
	strcpy(m_nDataOutBuf, "HY00");
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection

	m_nLooker = 0;
	m_nMotorControl = 0;
	
	// Write to the Axial or Radial motors based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
		// SAVE CURRENT VALUE FOR VIEW Y
    	m_NvRam.OutputW(CURR_VIEWY_AXIAL, 0);
    	m_NvRam.OutputB(CURR_VIEWY_POS_AXIAL, 0);
    	
    	m_nViewYStep_Axial = 0;
    	m_nViewYno_Axial   = 0;
    	m_cViewYPos_Axial  = 0;
    	
    	// CHECK FOR EITHER DETECTOR
    	SVstatus  = m_IO.InputW(SVSTAT_CS);
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	if ((SVstatus & VIEWY_HOME_AXIAL) || (SVstatus & VIEWY_LIMIT_AXIAL))
    	{
    		if ((SVstatus & VIEWY_HOME_AXIAL) && (SVstatus & VIEWY_LIMIT_AXIAL))
    		{
    			m_nMotorControl  = 0;
    			m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = ERR_VIEWY_LOOKERS_AXIAL;
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    		else
    		{
    			if (SVstatus & VIEWY_HOME_AXIAL)
    				goto sethomeyA;
    		}
    	}
    
    	// FIND HOME
    	steps = 0;
    	m_nWaitTime  = MAX_V_TIME;
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YINOFF;
    	slittemp = (WORD) (m_nMotorControl | YINON);
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
    		steps += 2;			// ALWAYS MOVE TWO STEPS
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    		if (SVstatus & VIEWY_HOME_AXIAL)
    		{
    			for (i = 1; i <= V_BACKLASH; i++) // ALWAYS EVEN
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
    			goto sethomeyA;
    		}
    	}
    	m_nMotorControl  = 0;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	status = ERR_VIEWY_HOME_AXIAL;
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
    
sethomeyA:	// GET TO FIRST STEP PASSED HOME
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YOUTOFF;
    	slittemp = (WORD) (m_nMotorControl | YOUTON);
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
    	m_nMotorControl  = 0;
    	m_IO.OutputW(AXIAL_MOTOR_CMD_REG, m_nMotorControl);
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;    
	  }
	else	// RADIAL_TYPE
	  {
		// SAVE CURRENT VALUE FOR VIEW Y
    	m_NvRam.OutputW(CURR_VIEWY_RADIAL, 0);
    	m_NvRam.OutputB(CURR_VIEWY_POS_RADIAL, 0);
    	
    	m_nViewYStep_Radial = 0;
    	m_nViewYno_Radial   = 0;
    	m_cViewYPos_Radial  = 0;
    	
    	// CHECK FOR EITHER DETECTOR
    	SVstatus  = m_IO.InputW(SVSTAT_CS);
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	if ((SVstatus & VIEWY_HOME_RADIAL) || (SVstatus & VIEWY_LIMIT_RADIAL))
    	{
    		if ((SVstatus & VIEWY_HOME_RADIAL) && (SVstatus & VIEWY_LIMIT_RADIAL))
    		{
    			m_nMotorControl  = 0;
    			m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    			status = ERR_VIEWY_LOOKERS_RADIAL;
    			ReturnStatus((WORD)status); // Place return code into reply buffer
    			return status;
    		}
    		else
    		{
    			if (SVstatus & VIEWY_HOME_RADIAL)
    				goto sethomeyR;
    		}
    	}
    
    	// FIND HOME
    	steps = 0;
    	m_nWaitTime  = MAX_V_TIME;
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YINOFF;
    	slittemp = (WORD) (m_nMotorControl | YINON);
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
    		status = delay( VIEWY_RADIAL );
    		steps += 2;			// ALWAYS MOVE TWO STEPS
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    		if (SVstatus & VIEWY_HOME_RADIAL)
    		{
    			for (i = 1; i <= V_BACKLASH; i++) // ALWAYS EVEN
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
    			goto sethomeyR;
    		}
    	}
    	m_nMotorControl  = 0;
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	status = ERR_VIEWY_HOME_RADIAL;
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;
    
sethomeyR:	// GET TO FIRST STEP PASSED HOME
    	m_nMotorControl  = 0;
    	m_nMotorControl |= VIEWY_ENABLE;
    	m_nMotorControl |= YOUTOFF;
    	slittemp = (WORD) (m_nMotorControl | YOUTON);
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	while (SVstatus & VIEWY_HOME_RADIAL)
    	{
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_RADIAL );
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, slittemp);
    		status = delay( VIEWY_RADIAL);
    		m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    		status = delay( VIEWY_RADIAL );
    		SVstatus = m_IO.InputW(SVSTAT_CS);
    	}
    	m_nMotorControl  = 0;
    	m_IO.OutputW(RADIAL_MOTOR_CMD_REG, m_nMotorControl);
    	ReturnStatus((WORD)status); // Place return code into reply buffer
    	return status;    
      } // end else
	
} // end function

//===========================================================================

/*** Revision History ***

12/15/15    JO      Initial additions and changes for Century B.


$Log: /IcarusBased/SpecFW/cmdhm.cpp $
 * 
 * 12    7/31/07 2:47p Nashth
 * Motor errors <LE> now returned as a specific motor error <S0>, <X0>, or
 * <Y0> as per QPR: THNJ75ML7T.
 * 
 * 11    6/26/07 3:46p Nashth
 * Added code to place all return codes for motor movements into reply
 * message buffer per QPR: THNJ74CP9G.
 * 
 * 10    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 9     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 8     6/29/05 13:45 Frazzitl
 * Optimized Slit Motor
 * 
 * 7     5/20/05 14:54 Frazzitl
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
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
