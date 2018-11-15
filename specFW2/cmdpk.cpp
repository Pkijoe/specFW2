//===========================================================================
//
//	Module Name:	cmdPK.cpp
//
// 	Function:		This routine parks the shutter and the XY axial/radial motors.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

#define STEPS_FOR_PARKING 512
#define STEPS_FOR_PARKING_X_PAST_LIMIT 300

unsigned int CParserThread::cmdPK()
{
	WORD			status(NO_ERRORS), cmd_reg;
	WORD			status1 = NO_ERRORS, status2 = NO_ERRORS, status3 = NO_ERRORS, status4 = NO_ERRORS;
	
	strcpy(m_nDataOutBuf, "PK00");
	
	// Move the shutter past home by 20 steps or more from any position
	// The motor should rest against the stop and lock when done.
	
	cmd_reg = m_IO.InputW(SHUTTER_RT_MOTOR_CMD_REG);             // Read the command register		
	cmd_reg &= SHUTTER_CMD_CLEAR;						    	 // Clear the shutter bits
	cmd_reg |= SHUTTER_DIR_HOME;								 // Set the DIRECTION bit (move towards home)		
	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register
	
	m_IO.OutputW(SHUTTER_MOTOR_STEP_REG, SHUTTER_MAX_STEPS
                 + SHUTTER_STEPS_PAST_HOME + SHUTTER_AWAY_FROM_HOME);	// Load the step register with a larger than needed value		
	Sleep(1);													 // This delay is needed to keep the FPGA happy.  The exact reason is unknown. 							 
    cmd_reg |= SHUTTER_MOTOR_RUN;								 // Set RUN bit
	cmd_reg |= SHUTTER_FLAG_OVERRIDE;							 // Set the OVERIDE bit (allows moving while on the sensor)
	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register - move the motor		

	Sleep(10);  // This delay is required !

	while ((m_IO.InputW(STATUS_REG2) & SHUTTER_MOTOR_BUSY))   	 // Wait for the motor to finish  //###########################
		Sleep(10);

	cmd_reg &= SHUTTER_CMD_CLEAR;								 // Clear the shutter bits
	m_IO.OutputW(SHUTTER_RT_MOTOR_CMD_REG, cmd_reg); 			 // Load the command register

	m_nLooker = 0;  // Don't stop at any of the lookers

	status1 = (unsigned short) mv_motor(STEPS_FOR_PARKING + STEPS_FOR_PARKING_X_PAST_LIMIT, (int) VIEWX_AXIAL, (int) INCREASE);  // Park the X axial motor
	if (status1 != NO_ERRORS) status = status1;
	
    status2 = (unsigned short) mv_motor(STEPS_FOR_PARKING + STEPS_FOR_PARKING_X_PAST_LIMIT, (int) VIEWX_RADIAL, (int) INCREASE); // Park the X radial motor	
	if (status2 != NO_ERRORS) status = status2;
	
	status3 = (unsigned short) mv_motor(STEPS_FOR_PARKING, (int) VIEWY_AXIAL, (int) INCREASE);  // Park the Y axial motor
	if (status3 != NO_ERRORS) status = status3;
	
	status4 = (unsigned short) mv_motor(STEPS_FOR_PARKING, (int) VIEWY_RADIAL, (int) INCREASE); // Park the Y radial motor	
	if (status4 != NO_ERRORS) status = status4;
	
	return status;  
}


unsigned int CParserThread::cmdIM()
{
	unsigned int	status(NO_ERRORS);
	
	strcpy(m_nDataOutBuf, "IM00");
	status = Configure_Motors(TRUE);	// Configure all motors including the slit
	if (status == NO_ERRORS) m_Init_Motors_Status = TRUE;	// Set the motors init status flag true if successful
	return status; 
}


//===========================================================================

/*** Revision History ***

07/13/16    JO      Initial version for Century B.  CBF-29.
07/13/16    JO      Update cmdpk with single motor moves on the x-axis.  CBF-30.
09/28/16	KR		CBF-55 Modify the shutter definition
10/27/16    JO      CBF-65 - Code changes to support Syngistix release 3.0.
11/09/16    JO		CBF-76 - IM command does not set "m_Init_Motors_Status " flag when executed.
01/30/17	JO		CBF-142 Add time-outs and error handing to code.


** End of Rev History **/
