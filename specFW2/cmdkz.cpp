//===========================================================================
//
//	Module Name:	cmdKZ.cpp
//
// 	Function:		These functions move the shutter and toroid to their key positions.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdKA()  // Move to the toroid the radial position
{
	WORD	status(NO_ERRORS);
	DWORD	tstart;
	
	strcpy(m_nDataOutBuf, "KA00");

	// Set the toroid position control bit; 1 = radial position - Move the toroid
	
	m_nSolenoidRegister |= TOROID_POSITION;
	m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);	

	// Wait here until the position is confirmed as "moved off" the HOME sensor.
	// This is all we have.  There is no position sensor for the radial position
	
	tstart = GetTickCount();
	while ((m_IO.InputW(STATUS_REG2) & TOROID_MOTOR_HOME) &&
		  ((GetTickCount() - tstart) < TIMEOUT_TOROID_MOVE_OFF_HOME)); 
		  
	if ((m_IO.InputW(STATUS_REG2) & TOROID_MOTOR_HOME) == 0)    // If off the HOME sensor
	 {
	    m_cToroid_Pos_Status = TOROID_AT_RADIAL_POS; 		    // Set the toroid status position - Assume it got there
	 }
    else
	 {
		ErrorMsg((int) YES, ERROR_TOROID_MOVE_OFF_HOME);
		status = ERROR_TOROID_MOVE_OFF_HOME;
	 }

	return status;
}

unsigned int CParserThread::cmdKB()  // Move to the shutter key radial position
{
	WORD	status(NO_ERRORS), tempw;

	strcpy(m_nDataOutBuf, "KB00");
	
 	tempw = m_NvRam.InputW(SHUTTER_POS_RADIAL); 
	status = (WORD) mv_abs_shutter(tempw);

	return status;
}

unsigned int CParserThread::cmdKC()  // Move to the shutter key closed position
{
	WORD	status(NO_ERRORS), tempw;

	strcpy(m_nDataOutBuf, "KC00");

 	tempw = m_NvRam.InputW(SHUTTER_POS_CLOSED);	
	status = (WORD) mv_abs_shutter(tempw);

	return status;
}


//===========================================================================

/*** Revision History ***

01/19/16    JO      Initial version for Century B.
07/07/16    JO      CBF-17 - Incorporate pneumatic toriod into Century B firmware
01/27/17	JO		CBF-142 Add time-outs and error handing to code.
					 Fixed bug in cmdKA().
02/23/17	JO		CBF-155 - Miscellaneous clean up.
					 
					 
					 
** End of Rev History **/
