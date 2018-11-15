//===========================================================================
//
//	Module Name:	cmdSM.cpp
//
// 	Function:	  	The routine is used to initiate SLEEP MODE.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdsm.cpp 5     6/29/07 5:08p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSM()
{
	WORD	status(NO_ERRORS);
	char	dummy;
	char	bcd1;
	char	bcd2;
	char	bcdval;
	char	bcdMonth, bcdDate, bcdYear, bcdHour, bcdMinute; // Values for debug print

	strcpy(m_nDataOutBuf, "SM00");

	bcd1 	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup MONTH (msb)
	m_nBytesRead++;
	bcd2 	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup MONTH (lsb)
	m_nBytesRead++;
	bcdval	  = (char) (bcd1 << 4);
	bcdval	 &= 0x10;
	bcdval	 |= bcd2;
	bcdMonth = bcdval;

	dummy  	  = *m_pCmdPtr++;			// ASCII DASH (0x2d)
	m_nBytesRead++;

	bcd1 	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup DATE (msb)
	m_nBytesRead++;
	bcd2 	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup DATE (lsb)
	m_nBytesRead++;
	bcdval	  = (char) (bcd1 << 4);
	bcdval	 |= bcd2;
	bcdDate = bcdval;

	dummy  	  = *m_pCmdPtr++;			// ASCII DASH (0x2d)
	m_nBytesRead++;

	bcd1 	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup YEAR (msb)
	m_nBytesRead++;
	bcd2 	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup YEAR (lsb)
	m_nBytesRead++;
	bcdval	  = (char) (bcd1 << 4);
	bcdval	 |= bcd2;
	bcdYear = bcdval;

	dummy  	  = *m_pCmdPtr++;			// ASCII '@' SIGN (0x40)
	m_nBytesRead++;

	bcd1	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup HOUR (msb)
	m_nBytesRead++;
	bcd2	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup HOUR (lsb)
	m_nBytesRead++;
	bcdval	  = (char) (bcd1 << 4);
	bcdval	 &= 0x30;
	bcdval	 |= bcd2;
	bcdHour = bcdval;

	dummy	  = *m_pCmdPtr++;			// ASCII COLON (0x3a)
	m_nBytesRead++;

	bcd1	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup MINUTE (msb)
	m_nBytesRead++;
	bcd2	  = (char) (*m_pCmdPtr++ - ASCII0);	// Wakeup MINUTE (lsb)
	m_nBytesRead++;
	bcdval	  = (char) (bcd1 << 4);
	bcdval	 |= bcd2;
	bcdMinute = bcdval;

	SetWakeupTimeValues(bcdYear, bcdMonth, bcdDate, bcdHour, bcdMinute);

	m_bSleepFlag  = true;

	ShutDown(status);

	printf("Sleep until: Month:Date:Year:Hour:Minute = %02x:%02x:%02x:%02x:%02x\r\n",
		         bcdMonth, bcdDate, bcdYear, bcdHour, bcdMinute);
	printf("Status = %04x\r\n", status);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdsm.cpp $
 * 
 * 5     6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 4     10/28/05 10:29a Nashth
 * Printfs added to debug QPR CANU6H3NLT.
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
