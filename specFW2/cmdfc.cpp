//===========================================================================
//
//	Module Name:	cmdFC.cpp
//
//	Function:		This routine sets the current clock time in the
//					following format:  d:mm-dd-yy@hh:mm:ss
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdfc.cpp 9     6/29/07 5:08p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdFC()
{
	WORD	wStatus(NO_ERRORS);
	BYTE	bDummy;
	BYTE	bBCD1;
	BYTE	bBCD2;
	BYTE	bBCDval;
	BYTE	byDay;
	BYTE	byYear;
	BYTE	byMonth;
	BYTE	byDate;
	BYTE	byHour;
	BYTE	byMin;
	BYTE	bySec;

	// "D:mm-dd-yy@hh:mm:ss");

	// Now write the clock data
	bBCDval	= (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current DAY (bBCD)
	byDay	= bBCDval;

	bDummy	 = *m_pCmdPtr++;					// skip ASCII COLON (0x3a)
	bBCD1	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current MONTH (msb)
	bBCD2	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current MONTH (lsb)
	bBCDval	 = (BYTE) (bBCD1 << 4);
	bBCDval	|= bBCD2;
	bBCDval  &= 0x1f;							// Just for safety
	byMonth	= bBCDval;

	bDummy	 = *m_pCmdPtr++;					// skip ASCII DASH (0x2d)
	bBCD1	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current DATE (msb)
	bBCD2	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current DATE (lsb)
	bBCDval	 = (BYTE) (bBCD1 << 4);
	bBCDval	|= bBCD2;
	bBCDval  &= 0x3f;							// Just for safety
	byDate	= bBCDval;

	bDummy	 = *m_pCmdPtr++;					// skip ASCII DASH (0x2d)
	bBCD1	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current YEAR (msb)
	bBCD2	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current YEAR (lsb)
	bBCDval	 = (BYTE) (bBCD1 << 4);
	bBCDval	|= bBCD2;
	byYear	= bBCDval;

	bDummy	 = *m_pCmdPtr++;					// skip ASCII '@' SIGN (0x40)
	bBCD1	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current HOUR (msb)
	bBCD2	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current HOUR (lsb)
	bBCDval	 = (BYTE) (bBCD1 << 4);
	bBCDval	|= bBCD2;
	bBCDval  &= 0x3f;							// Just for safety
	byHour	= bBCDval;

	bDummy	 = *m_pCmdPtr++;					// skip ASCII COLON (0x3a)
	bBCD1	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current MINUTE (msb)
	bBCD2	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current MINUTE (lsb)
	bBCDval	 = (BYTE) (bBCD1 << 4);
	bBCDval	|= bBCD2;
	bBCDval  &= 0x7f;							// Just for safety
	byMin	= bBCDval;

	bDummy	 = *m_pCmdPtr++;					// skip ASCII COLON (0x3a)
	bBCD1	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current SECOND (msb)
	bBCD2	 = (BYTE) (*m_pCmdPtr++ - ASCII0);	// Current SECOND (lsb)
	bBCDval	 = (BYTE) (bBCD1 << 4);
	bBCDval	|= bBCD2;
	bBCDval  &= 0x7f;							// Just for safety
	bySec	= bBCDval;

	SetClockTime(byDay, byYear, byMonth, byDate, byHour, byMin, bySec); // Write into the clock

	m_nBytesRead += 19;
	strcpy(m_nDataOutBuf, "FC00");

	return wStatus;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdfc.cpp $
 * 
 * 9     6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 8     11/11/05 8:25a Frazzitl
 * Added special clock critical section around all clock functions.
 * 
 * 7     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 6     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 5     4/13/05 13:15 Frazzitl
 * Miscellaneous changes primarily for status processing
 * 
 * 4     4/08/05 10:21 Frazzitl
 * Still trying to get System Clock to work
 * 
 * 3     4/07/05 1:32p Frazzitl
 * Fixed problem with clock
 * 
 * 2     4/04/05 10:01 Frazzitl
 * Corrected the operation of the System Clock, both reading and writing.
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
