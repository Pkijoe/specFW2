//===========================================================================
//
//	Module Name:	cmdST.cpp
//
// 	Function:	  	This routine returns the status of the instrument.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdST()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);
	WORD	tval;
//						  "0         1         2         3         4         5         6         7         8         9" 
//						  "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
	strcpy(m_nDataOutBuf, "ST00VERvvvvv          REVx0xHUxHVxLNITxxxxx1PSxxPXxxxPYxxxPXxxxPYxxxTxSHxLPxDPxMDx-xx");

	// Firmware Version
	m_nDataOutBuf[ 7] = m_cVersionNo[ 0];
	m_nDataOutBuf[ 8] = m_cVersionNo[ 1];
	m_nDataOutBuf[ 9] = m_cVersionNo[ 2];
	m_nDataOutBuf[10] = m_cVersionNo[ 3];
	m_nDataOutBuf[11] = m_cVersionNo[ 4];
	m_nDataOutBuf[12] = m_cVersionNo[ 5];
	m_nDataOutBuf[13] = m_cVersionNo[ 6];
	m_nDataOutBuf[14] = m_cVersionNo[ 7];
	m_nDataOutBuf[15] = m_cVersionNo[ 8];
	m_nDataOutBuf[16] = m_cVersionNo[ 9];
	m_nDataOutBuf[17] = m_cVersionNo[10];
	m_nDataOutBuf[18] = m_cVersionNo[11];
	m_nDataOutBuf[19] = m_cVersionNo[12];
	m_nDataOutBuf[20] = m_cVersionNo[13];
	m_nDataOutBuf[21] = m_cVersionNo[14];
	// Board Revision
	m_nDataOutBuf[25] = (char) (m_cRevMPC + ASCII0);	// Revision Level - MPC Board
	m_nDataOutBuf[27] = (char) (m_cRevCDS + ASCII0);	// Revision Level - CDS Board
	// UV Resolution
	m_nDataOutBuf[30] = (char) (m_cUVres  + ASCII0);
	// VIS Resolution
	m_nDataOutBuf[33] = (char) (m_cVISres + ASCII0);
	// Integration Time
	s2b(m_nIntegrationTime, 38);
	// Slit Position
	m_nDataOutBuf[46] = m_cSlitType;
	m_nDataOutBuf[47] = (char) (m_cSlitPos + ASCII0);
	
	c2b(m_cViewXPos_Axial, 50);		// Axial View X Position
	c2b(m_cViewYPos_Axial, 55);		// Axial View Y Position
	c2b(m_cViewXPos_Radial, 60);	// Radial View X Position
	c2b(m_cViewYPos_Radial, 65);	// Radial View Y Position
	
	// Toroid position status	
	m_nDataOutBuf[69] = (char) (m_cToroid_Pos_Status + ASCII0);	
	
	// Shutter position status
	m_nDataOutBuf[72] = (char) (m_cShutter_Pos_Status + ASCII0);

	// Hg Lamp Enabled
	if (m_bLPmode)
		m_nDataOutBuf[75] = '1';
	else
		m_nDataOutBuf[75] = '0';
	// Detector Power
	if (m_bCCDPower)
		m_nDataOutBuf[78] = '1';
	else
		m_nDataOutBuf[78] = '0';
	// Startup Mode
	m_nDataOutBuf[81] = m_cStartupMode;
	// Code
	tval	     = (WORD) (0xff00 & m_nFatalCode);
	tval	   >>= 8;
	m_nDataOutBuf[83] = (char) tval;
	tval	     = (WORD) (0x00ff & m_nFatalCode);
	m_nDataOutBuf[84] = (char) tval;
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}

/////////////////////////////////
// Contact closure status 
// command: <RT> <contact number, 1 byte>
// returns: <RT> <status, 2 bytes> <contact number, 1 byte> <contact status, 1 byte>
//
// contact status is 0 or 1 for open/closed, respectively.
//
// for instance
// receive: RT1
// reply:	RT0011 - contact 1 is closed.

unsigned int CParserThread::cmdRT()
{
	WORD	status(NO_ERRORS);
	char	val;

	val	= *m_pCmdPtr++;
	m_nBytesRead++;

	if (val == '1')
	{	// reading contact closure status contact 1
		strcpy(m_nDataOutBuf, "RT00xx");		// 2 character string
		if ( m_IO.InputW(SVSTAT_CS) & TRIGGER_IN )	// see if it is closed
			memcpy(&m_nDataOutBuf[4], "10", 2);
		else
			memcpy(&m_nDataOutBuf[4], "11", 2);
	}
	else
	{
		status = ERR_PARA;
		strcpy(m_nDataOutBuf, "RT07");
	}

	return status;
}
//===========================================================================

/*** Revision History ***


01/26/16    JO      Initial version for Century B.


$Log: /IcarusBased/SpecFW/cmdst.cpp $
 * 
 * 8     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 7     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 6     5/23/05 11:50a Frazzitl
 * Changed version number from 4 to 15 characters to support standard
 * version format = XX.xx.yyy.bbbb
 * 
 * 5     5/13/05 14:31 Frazzitl
 * Fixed Statrtup mode return
 * 
 * 4     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 3     3/30/05 14:27 Frazzitl
 * Initializing variables
 * 
 * 2     3/30/05 2:59p Frazzitl
 * Fixed some startup problems
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
