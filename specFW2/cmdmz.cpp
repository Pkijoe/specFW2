//===========================================================================
//
//	Module Name:	cmdMZ.cpp
//
// 	Function:	  	These functions move the shutter/toroid motors to an
//                  absolute position in the range of 1..MAX_STEPS. 
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016, PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdMB()    // Move Shutter to an absolute position
{
	WORD	status(NO_ERRORS);
    WORD    abs_pos;
	
	strcpy(m_nDataOutBuf, "MB00");
	
	abs_pos = b2c();                                                 // Get the absolute position from command buffer
	status = (WORD) mv_abs_shutter(abs_pos);

	return status;
}


unsigned int CParserThread::mv_abs_shutter(WORD abs_pos)   			// Move shutter to an absolute position
{
	WORD	status(NO_ERRORS), cmd_reg;
    WORD    steps, direction = 0;								    // Set the direction bit (away from home)
	int     isteps;
	DWORD	tstart, tm = 0;
	
	if (abs_pos > m_Shutter_Pos_Max)  
	  {
		status = ERR_PARA1;
		goto exit;   
	  }
	  
	if (!m_Shutter_Init_Flag)				 // Issue home command if shutter has not been initialized.
	  {
		if ((status = (WORD) cmdHB()) != NO_ERRORS)	
		{
		  ErrorMsg((int) YES, status);
		  goto exit;
		}		
	  }
	  
	isteps = (int) abs_pos - (int) m_nShutter_Pos;                   // Calculate number of steps to move
    steps = (WORD) abs(isteps);

	if (steps == 0)  goto exit;                                      // Nothing to do - by by
	if (isteps < 0) direction = SHUTTER_DIR_HOME;             	     // Set the direction bit (towards home)

	m_IO.OutputW(SHUTTER_MOTOR_STEP_REG, steps); 					 // Load the step register
		
	cmd_reg = m_IO.InputW(SHUTTER_RT_MOTOR_CMD_REG);           		 // Read the command register
	cmd_reg &= SHUTTER_CMD_CLEAR;						    	 	 // Clear the shutter bits
 
	Sleep(1);														 // Sync to FPGA state machine 

	cmd_reg |= direction;							  	 			 // Set the direction bit
    cmd_reg |= SHUTTER_MOTOR_RUN;							  		 // Set the RUN bit
  	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 				 // Load the command register - move the motor
		
	Sleep(10);														 // This delay is required ! 	

	tstart = GetTickCount();
	while ((m_IO.InputW(STATUS_REG2) & SHUTTER_MOTOR_BUSY) &&
	      ((tm = GetTickCount() - tstart) < TIMEOUT_SHUTTER_MOTOR_MOVE_TO_ABS_POS));	// Wait for the motor to finish
		  Sleep(10);
	
	if (tm >= TIMEOUT_SHUTTER_MOTOR_MOVE_TO_ABS_POS)			// See if we timed out
	  {
		ErrorMsg((int) YES, ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS);
		status = ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS;
		goto exit;
	  }	
	  
	cmd_reg &= SHUTTER_CMD_CLEAR;									 // Clear the shutter bits

	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 				 // Load the command register
	
    m_nShutter_Pos = abs_pos;										 // Set the shutter position
	set_shutter_position_status();								   	 // Set the shutter position status		
exit:;
		
	return status;	
}	


void CParserThread::set_shutter_position_status()
{
	WORD tempw, temp2w;	

	tempw = m_NvRam.InputW(SHUTTER_POS_RADIAL);	
	temp2w = m_NvRam.InputW(SHUTTER_POS_CLOSED);	
	if (m_nShutter_Pos == 1) m_cShutter_Pos_Status = SHUTTER_AT_AXIAL_POS;
	else if (m_nShutter_Pos == tempw) m_cShutter_Pos_Status = SHUTTER_AT_RADIAL_POS;
	else if (m_nShutter_Pos == temp2w) m_cShutter_Pos_Status = SHUTTER_AT_CLOSED_POS;	
	else m_cShutter_Pos_Status = SHUTTER_AT_OTHER_POS;	
}	


//===========================================================================

/*** Revision History ***

01/15/16    JO      Initial version for Century B.
01/18/16    JO      Added mv_abs_toroid and mv_abs_shutter so they can be re-used elsewhere.
01/21/16    JO      Modified toroid function to handle four digit position.
01/26/16    JO      Added functions set_toroid_position_status() and set_shutter_position_status().
08/05/16	KR		Set the shutter enable bit prior to move CBF-35
07/28/16    JO      CBF-26 Removed unused toroid commands and references.
09/20/16	KR		CBF-42 Back out CBF-35 with changes to FPGA for shutter enable
09/28/16	KR		CBF-55 Modify the shutter definition
01/27/17	JO		CBF-142 Add time-outs and error handing to code.
02/02/17    JO      CBF-147 Add get/set BRAM commands for maximum shutter position.
02/23/17    JO      CBF-154 Add shutter home logic to mv_abs_shutter() and assocciated files.

** End of Rev History **/
