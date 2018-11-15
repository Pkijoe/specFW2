//===========================================================================
//
//	Module Name:	cmdHZ.cpp
//
// 	Function:	  	These functions move the shutter/toroid motors to the
//                  home position. 
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016, PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdHA()    // Move Toroid to the home (axial) position 
{                                    
	WORD	status(NO_ERRORS);
	DWORD	tstart;
	
	strcpy(m_nDataOutBuf, "HA00");

	// Clear the toroid position control bit; 0 = HOME (Axial)  - Move the toroid
	
	m_nSolenoidRegister &= ~TOROID_POSITION;
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);	

	// Wait here until the position is confirmed by the HOME sensor.
	
	tstart = GetTickCount();
	while ((!(m_IO.InputW(STATUS_REG2) & TOROID_MOTOR_HOME)) &&
		  ((GetTickCount() - tstart) < TIMEOUT_TOROID_MOTOR_TO_HOME));  

	if (m_IO.InputW(STATUS_REG2) & TOROID_MOTOR_HOME)   	  // If at the HOME sensor
	  {
		 m_cToroid_Pos_Status = TOROID_AT_AXIAL_POS; 		  // Set the Toroid status position
	  }
    else
	 {
		ErrorMsg((int) YES, ERROR_TOROID_MOVE_TO_HOME);
		status = ERROR_TOROID_MOVE_TO_HOME;
	 }
	
    return status;
}

unsigned int CParserThread::cmdHB()    // Move Shutter to the home (axial) position
{
	WORD	status(NO_ERRORS), cmd_reg;
	DWORD	tstart, tm = 0;
	
	strcpy(m_nDataOutBuf, "HB00");
	
	// Check to see if we are at the HOME position by reading the sensor.
	// If so, move off the sensor by one revolution, then in either case,
	// move towards HOME onto the sensor.  Finally, move off the sensor by
	// SHUTTER_AWAY_FROM_HOME steps and set the current step position to one.

	if (m_IO.InputW(STATUS_REG2) & SHUTTER_MOTOR_HOME)   			 // At the HOME position
	  {
		// Move off the HOME sensor one revolution 
		
		m_IO.OutputW(SHUTTER_MOTOR_STEP_REG, SHUTTER_STEPS_PER_REV); // Load the step register
		
		cmd_reg = m_IO.InputW(SHUTTER_RT_MOTOR_CMD_REG);             // Read the command register
		cmd_reg &= SHUTTER_CMD_CLEAR;						    	 // Clear the shutter bits - DIRECTION bit (move away from home)		
        cmd_reg |= SHUTTER_MOTOR_RUN;								 // Set the RUN bit
		cmd_reg |= SHUTTER_FLAG_OVERRIDE;							 // Set the OVERIDE bit (allows moving while on the sensor)
		m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register - move the motor
		
		Sleep(10);	// This delay is required ! 
		
		tstart = GetTickCount();
		while ((m_IO.InputW(STATUS_REG2) & SHUTTER_MOTOR_BUSY) &&
		      ((tm = GetTickCount() - tstart) < TIMEOUT_SHUTTER_MOTOR_MOVE_OFF_SENSOR));	// Wait for the motor to finish
			  Sleep(10);
		
		if (tm >= TIMEOUT_SHUTTER_MOTOR_MOVE_OFF_SENSOR)			// See if we timed out
		  {
			ErrorMsg((int) YES, ERROR_SHUTTER_MOTOR_MOVE_OFF_SENSOR);
			status = ERROR_SHUTTER_MOTOR_MOVE_OFF_SENSOR;
			goto cmd_exit;
		  }
	  }
		
		// Move onto the HOME sensor		
		cmd_reg = m_IO.InputW(SHUTTER_RT_MOTOR_CMD_REG);             // Read the command register		
		cmd_reg &= SHUTTER_CMD_CLEAR;						    	 // Clear the shutter bits
		cmd_reg |= SHUTTER_DIR_HOME;								 // Set the DIRECTION bit (move towards home)		
		m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register
		
		m_IO.OutputW(SHUTTER_MOTOR_STEP_REG, SHUTTER_MAX_STEPS
                     + SHUTTER_STEPS_PER_REV);				         // Load the step register with a larger than needed value		
		Sleep(1);													 // Sync to FPGA state machine 							 
        cmd_reg |= SHUTTER_MOTOR_RUN;								 // Set the RUN bit
		m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register - move the motor		

		Sleep(10);	// Sync to the FPGA state machine
		
		tstart = GetTickCount();
		while ((m_IO.InputW(STATUS_REG2) & SHUTTER_MOTOR_BUSY) &&
		      ((tm = GetTickCount() - tstart) < TIMEOUT_SHUTTER_MOTOR_MOVE_TO_SENSOR)); 	// Wait for the motor to finish
			  Sleep(10);
			  
		if (tm >= TIMEOUT_SHUTTER_MOTOR_MOVE_TO_SENSOR)				// See if we timed out
		  {
			ErrorMsg((int) YES, ERROR_SHUTTER_MOTOR_MOVE_TO_SENSOR);
			status = ERROR_SHUTTER_MOTOR_MOVE_TO_SENSOR;
			goto cmd_exit;
		  }
		
		cmd_reg &= SHUTTER_CMD_CLEAR;								 // Clear the shutter bits
		m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register
		
		Sleep (100);												 // Give it time to settle
		
		// Move off the HOME sensor by SHUTTER_AWAY_FROM_HOME steps 
		
		m_IO.OutputW(SHUTTER_MOTOR_STEP_REG, SHUTTER_AWAY_FROM_HOME); // Load the step register

		cmd_reg = m_IO.InputW(SHUTTER_RT_MOTOR_CMD_REG);             // Read the command register
		cmd_reg &= SHUTTER_CMD_CLEAR;						    	 // Clear the shutter bits - DIRECTION bit (move away from home)		
        cmd_reg |= SHUTTER_MOTOR_RUN;								 // Set the RUN bit
		cmd_reg |= SHUTTER_FLAG_OVERRIDE;							 // Set the OVERIDE bit (allows moving while on the sensor)
		m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register - move the motor
		
		Sleep(10);	// Sync to the FPGA state machine 
		
		tstart = GetTickCount();
		while ((m_IO.InputW(STATUS_REG2) & SHUTTER_MOTOR_BUSY) &&
		      ((tm = GetTickCount() - tstart) < TIMEOUT_SHUTTER_MOTOR_MOVE_TO_HOME)); 	// Wait for the motor to finish
			  Sleep(10);
			  
		if (tm >= TIMEOUT_SHUTTER_MOTOR_MOVE_TO_HOME)				// See if we timed out
		  {
			ErrorMsg((int) YES, ERROR_SHUTTER_MOTOR_MOVE_TO_HOME);
			status = ERROR_SHUTTER_MOTOR_MOVE_TO_HOME;
			goto cmd_exit;
		  }
			
		cmd_reg &= SHUTTER_CMD_CLEAR;								 // Clear the shutter bits	
		m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register
	    m_nShutter_Pos = 1;											 // Set the Shutter position
		m_cShutter_Pos_Status = SHUTTER_AT_AXIAL_POS;				 // Set the Shutter status position
		m_Shutter_Init_Flag	= true;									 // Shutter initialization flag. Initialized = true.
		
cmd_exit:;		
	return status;
}
//===========================================================================

/*** Revision History ***

01/13/16    JO      Initial version for Century B.
07/06/16    JO      Added new home position control code; CBF-17.
08/05/16	KR		Add Shutter Motor Enable CBF-35
09/20/16	KR		CBF-42 Back out CBF-35 with changes to FPGA for shutter enable
09/28/16	KR		CBF-55 Modify the shutter definition
01/27/17	JO		CBF-142 Add time-outs and error handing to code.
02/23/17    JO      CBF-154 Add shutter home logic to mv_abs_shutter() and associated files.


** End of Rev History **/
