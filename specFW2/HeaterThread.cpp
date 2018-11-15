//===========================================================================
//
//	Module Name:	HeaterThread.cpp
//
// 	Function:		This routine controls the tub heater of the Spectrometer
//					the function determines how to set up the timer to turn the
//					heaters on & off . It starts with the m_lServoWattage, corrects
//					for any excess (or insufficient) power delivered during the
//					previous cycle, and determines integers for the number of
//					half-cycles "on" and "off". This function runs every
//					m_lHeaterCycleNominal seconds unless a long data read forces
//					it to be delayed.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "Debug.h"

UINT WINAPI CParserThread::heaterThread(LPVOID pParam)
{
	extern float PID_Period; // defined in ServoControl for now.
	
	CParserThread *pThis = (CParserThread *) pParam;

	while (1)
	{	// wait PID_Period seconds
		Sleep((DWORD)(1000.0 * PID_Period));   // Keep PID_Period for now; Remove when development is done.
		pThis->ServoControl();

	} // end while
}

//===========================================================================

/*** Revision History ***

02/18/16    JO      Initial heater changes for Century B.
07/21/16	JO		Updated for new heater design - CBF-16, CBF-32.

$Log: /IcarusBased/SpecFW/HeaterThread.cpp $
 * 
 * 19    2/22/06 2:13p Frazzitl
 * Changed the debugging of the thread to use a circular buffer in BRAM.
 * 
 * 18    2/22/06 11:54a Frazzitl
 * Fixed formatting slightly
 * 
 * 17    2/22/06 8:33a Frazzitl
 * Fixed problem of printing CStrings using a printf statement.
 * 
 * 16    2/21/06 2:11p Frazzitl
 * Added debug code to display access to each primary thread as well as
 * the stopping of the watchdog.
 * 
 * 15    1/13/06 1:37p Frazzitl
 * Removed dead code as recommended in a QPR
 * 
 * 14    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 13    11/11/05 2:57p Frazzitl
 * Corrected problem with printf staement used for debugging
 * 
 * 12    11/09/05 1:05p Frazzitl
 * Added debug capabilities and implemented feature that keeps heater in
 * Stablization mode for 15 minutes before going to Normal mode.
 * 
 * 11    6/17/05 10:01a Frazzitl
 * Removed debug logging now that everything is working properly.
 * 
 * 10    5/26/05 4:55p Frazzitl
 * Added some debugging if crashes continue - will later remove
 * 
 * 9     5/26/05 9:12a Frazzitl
 * Added changes recommended by Remco to fix crashing problem
 * 
 * 8     5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 7     5/24/05 17:13 Frazzitl
 * WaitForSingleObject did not work as thought - caused slow response
 * 
 * 6     5/24/05 3:46p Frazzitl
 * Corrected exit from threads and added the elimination of the VIS status
 * thread if single channel instrument.
 * 
 * 5     5/23/05 4:59p Frazzitl
 * Attempting to speed up command response by haing Status threads exit
 * quickly if a command is pending
 * 
 * 4     5/10/05 11:50 Frazzitl
 * Fixed problem with threads
 * 
 * 3     5/06/05 15:53 Frazzitl
 * New implementation of threads
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 15:08 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
