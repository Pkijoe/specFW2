//===========================================================================
//
//	Module Name:	cmdGC.cpp
//
//	Function:		This routine returns the current clock time in the
//					following format:  d:mm-dd-yy@hh:mm:ss
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdgc.cpp 11    6/29/07 5:08p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGC()
{
	WORD	wStatus(NO_ERRORS);

	//					  "01234567890123456789012"
	strcpy(m_nDataOutBuf, "GC00x:xx-xx-xx@xx:xx:xx");

	GetDateTimeStamp(&m_nDataOutBuf[4]);

	// Add just for output to Hyperterminal
	m_nDataOutBuf[23] = '\r';
	m_nDataOutBuf[24] = '\n';
	m_nDataOutBuf[25] = NULL;
	printf(m_nDataOutBuf);
	// Now remove extra characters
	m_nDataOutBuf[23] = NULL;
	m_nDataOutBuf[24] = NULL;
	m_nDataOutBuf[25] = NULL;

	return wStatus;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdgc.cpp $
 * 
 * 11    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 10    11/11/05 8:25a Frazzitl
 * Added special clock critical section around all clock functions.
 * 
 * 9     11/10/05 2:38p Frazzitl
 * Corrected problem with time output and added date & time output to
 * HyperTerminal.
 * 
 * 8     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
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
