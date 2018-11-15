//===========================================================================
//
//	Module Name:	cmdPZ.cpp
//
// 	Function:	  	These functions move the shutter and toroid motors together to the
//                  axial and radial positions. 
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016, PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdPA()    // Move Toroid and Shutter to the axial position 
{                                    
	WORD	status(NO_ERRORS);
    WORD    tempt, temps;

	strcpy(m_nDataOutBuf, "PA00");
	
	tempt = 0; // Axial position = home position = 0
	temps = 1;
	
	status = (WORD) mv_abs_shutter_toroid(temps, tempt);

    return status;
}

unsigned int CParserThread::cmdPR()    // Move Toroid and Shutter to the radial position
{
	WORD	status(NO_ERRORS);
    WORD    temps, tempt;
	
	strcpy(m_nDataOutBuf, "PR00");
	
	tempt = 1;  // Radial position
	temps = m_NvRam.InputW(SHUTTER_POS_RADIAL);
	
	status = (WORD) mv_abs_shutter_toroid(temps, tempt);

	return status;
}

unsigned int CParserThread::mv_abs_shutter_toroid(WORD shutter_pos, WORD toroid_pos)	// Move shutter and toroid to an absolute position
{
	WORD	status(NO_ERRORS), cmd_reg;
    WORD    s_steps, s_direction = 0;		 // Set the direction bit (away from home)
	DWORD	tstart, tm = 0;
	int     s_isteps;
	
	if ((shutter_pos > m_Shutter_Pos_Max) || (shutter_pos == m_nShutter_Pos))		// CBF-52 If shutter already in position, don't move it
	  {
		goto exit;   // exit for now
	  }
	  
	if (!m_Shutter_Init_Flag)				 // Issue home command if shutter has not been initialized.
	  {
		if ((status = (WORD) cmdHB()) != NO_ERRORS)	
		{
		  ErrorMsg((int) YES, status);
		  goto exit;
		}		
	  }

	if (toroid_pos == 1)  // Move toroid to radial
	  {
	    // Set the toroid position control bit; 1 = radial position - Move the toroid
		m_nSolenoidRegister |= TOROID_POSITION;
		m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
	    m_cToroid_Pos_Status = TOROID_AT_RADIAL_POS; 		    	 // Set the toroid status position - Assume it got there		
	  }
	else  // Move toroid to home (axial)
	  {
		// Clear the toroid position control bit; 0 = HOME (Axial)  - Move the toroid
		m_nSolenoidRegister &= ~TOROID_POSITION;
		m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
		m_cToroid_Pos_Status = TOROID_AT_AXIAL_POS; 		 		 // Set the Toroid status position		
	  }
	  
	s_isteps = (int) shutter_pos - (int) m_nShutter_Pos;             // Calculate shutter number of steps to move
    s_steps = (WORD) abs(s_isteps);
	if (s_isteps < 0) s_direction = SHUTTER_DIR_HOME;             	 // Set the shutter direction bit (towards home)

	m_IO.OutputW(SHUTTER_MOTOR_STEP_REG, s_steps); 					 // Load the shutter step register

	cmd_reg = 0;						   	 						 // Clear the register

	cmd_reg |= s_direction;							 				 // Set the direction bit
    cmd_reg |= SHUTTER_MOTOR_RUN;		 							 // Set the RUN bit
  	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 				 // Load the command register - move the motor
		
	Sleep(10);	// This delay is required ! 	

	tstart = GetTickCount();
	while ((m_IO.InputW(STATUS_REG2) & SHUTTER_MOTOR_BUSY) &&
	      ((tm = GetTickCount() - tstart) < TIMEOUT_SHUTTER_MOTOR_MOVE_TO_ABS_POS));	// Wait for the motor to finish
		  Sleep(10);
	
	if (tm >= TIMEOUT_SHUTTER_MOTOR_MOVE_TO_ABS_POS)				 // See if we timed out
	  {
		ErrorMsg((int) YES, ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS);
		status = ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS;
		goto exit;
	  }	
		
	cmd_reg = 0;						   	 						 // Clear the register 									
	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 				 // Load the command register
	
    m_nShutter_Pos = shutter_pos;									 // Set the shutter position		
	set_shutter_position_status();								   	 // Set the shutter position status		
exit:;
										
    return status;	
}	

//===========================================================================

/*** Revision History ***

01/19/16    JO      Initial version for Century B.
01/26/16    JO      Added calls to set_toroid_position_status() & set_shutter_position_status().
07/07/16    JO      CBF-17 - Incorporate pneumatic toriod into Century B firmware
08/05/16	KR		CBF-35 Add shutter enable
09/20/16	KR		CBF-42 Back out CBF-35 with changes to FPGA for shutter enable
09/20/16	KR		CBF-52 If shutter already in position, don't move it
09/28/16	KR		CBF-55 Modify the shutter definition 
01/27/17	JO		CBF-142 Add time-outs and error handing to code.
02/02/17    JO      CBF-147 Add get/set BRAM commands for maximum shutter position.
02/23/17    JO      CBF-154 Add shutter home logic to mv_abs_shutter() and assocciated files.


** End of Rev History **/
