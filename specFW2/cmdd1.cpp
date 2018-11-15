//===========================================================================
//
//	Module Name:	cmdD1.cpp
//
// 	Function:		This routine sets the Debug options parameter.
//
//	Original Author:	TJ Watko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdd1.cpp 15    1/30/07 4:08p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "Debug.h"

static	int		s_nCount = 0;
static	DWORD	s_nLastCount = 0;

unsigned int CParserThread::cmdD1()
{
	WORD	status(NO_ERRORS);
	unsigned short	nDebug;

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters

	strcpy(m_nDataOutBuf, "D100");

	nDebug = c4h4(); // Get four characters into hex bytes

	if (nDebug & DEBUG_THREADS)
	{	// If DEBUG_THREADS set, first display the data previously set in circular buffer
		// then clear circular buffer before collecting new data
		// Once m_nDebug is set with DEBUG_THREADS, the data collection immediately starts
		DisplayThreadData();
		ClearThreadBuffer();
	}
	m_nDebug = nDebug;

	// Save debug options in BRAM, but do not set DEBUG_THREADS if set
	m_NvRam.OutputW(DEBUG_FLAG, (WORD)(nDebug & ~DEBUG_THREADS));
	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Remove protection
	theApp.SetDebugFlag(m_nDebug);  // Set the debug word in the main App

	return status;
}
//===========================================================================

//===========================================================================
// This routine displays thread debug data saved in the circular buffer
//===========================================================================
void CParserThread::DisplayThreadData()
{
	short	nThread;
	DWORD	nThreadTime;
	DWORD	nThreadDiff;
	char	nBuffer[10];

	// Retrieve all 16 points from the Circular buffer, starting with the latest
	for (short n = 0; n < 16; n++)
	{
		// Decrement count immediatedly since count has been increment
		// to next position after the lastest data had been saved.
		// This will insure the 16 data items are in correct order.
		--s_nCount;
		// If count less than zero, set count to 15
		if (s_nCount < 0)
			s_nCount = 15;

		nThread		= m_NvRam.InputB((WORD)(THREAD_DATA + s_nCount    ));
		nThreadTime	= m_NvRam.InputL((WORD)(THREAD_TIME + s_nCount * 4));
		nThreadDiff	= m_NvRam.InputL((WORD)(THREAD_LAST + s_nCount * 4));

		// Convert the thread number to a text buffer
		switch(nThread)
		{
			case 1:		strcpy(nBuffer, "Status     ");
			break;
			case 2:		strcpy(nBuffer, "StatusUV   ");
			break;
			case 3:		strcpy(nBuffer, "StatusVIS  ");
			break;
			case 4:		strcpy(nBuffer, "Heater     ");
			break;
			case 5:		strcpy(nBuffer, "Duty       ");
			break;
			case 6:		strcpy(nBuffer, "WatchdogUV ");
			break;
			case 7:		strcpy(nBuffer, "WatchdogVIS");
			break;
			default:	strcpy(nBuffer, "Default    ");
			break;
		}
		nBuffer[9] = NULL;

		// Output each data point
		printf("%9s,%2i,%8i,%4i\r\n", nBuffer, nThread, nThreadTime, nThreadDiff);
	}
}
//===========================================================================

//===========================================================================
// This routine clears the circular data buffer
//===========================================================================
void CParserThread::ClearThreadBuffer()
{
	for (WORD nCount = 0; nCount < 16; nCount++)
	{
		// Clear all 16 positions plus reset the count to zero
		m_NvRam.OutputB((WORD)(THREAD_DATA + nCount    ), 0);
		m_NvRam.OutputL((WORD)(THREAD_TIME + nCount * 4), 0);
		m_NvRam.OutputL((WORD)(THREAD_LAST + nCount * 4), 0);
		s_nCount = 0;
	}
}
//===========================================================================

//===========================================================================
// This routine saves, in a circular buffer, thread debug data
// and the corresponding time as the system Tick count
//===========================================================================
void CParserThread::StoreThreadData(short nThread)
{
	DWORD	nTime = ::GetTickCount();
	DWORD	nDiff = nTime - s_nLastCount;	// First time in, s_nLastCount = 0

	// Difference should always be small so if greater than 9999, set to zero
	if (nDiff > 9999)			// Use 9999 because it fits in output format
		nDiff = 0;

	// Save the data in the current count position, then increment count
	m_NvRam.OutputB((WORD)(THREAD_DATA + s_nCount    ), (BYTE) nThread);
	m_NvRam.OutputL((WORD)(THREAD_TIME + s_nCount * 4), (long) nTime);
	m_NvRam.OutputL((WORD)(THREAD_LAST + s_nCount * 4), (long) nDiff);

	++s_nCount;					// count is now ready for next data item
	s_nLastCount = nTime;		// save current time for next entry

	// The Circular buffer is only 16 points long (0 to 15),
	// so reset count if greater than 15
	if (s_nCount >= 16)
		s_nCount  = 0;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdd1.cpp $
 * 
 * 15    1/30/07 4:08p Nashth
 * First pass at keeping the camera on during a 'long' connection as per
 * QPR: THNJ6XXLK3.
 * 
 * 14    5/31/06 2:20p Nashth
 * Removed temporary test.
 * 
 * 12    2/27/06 9:13a Frazzitl
 * Fixed formatting error
 * 
 * 11    2/24/06 9:04a Frazzitl
 * Corrected a problem with the order of the debug data displayed, plus
 * added a time equal to the current time less that previous current time.
 * 
 * 10    2/22/06 3:29p Frazzitl
 * Fixed a formatting problem
 * 
 * 9     2/22/06 2:13p Frazzitl
 * Changed the debugging of the thread to use a circular buffer in BRAM.
 * 
 * 8     11/29/05 4:47p Nashth
 * Set a debug flag for DEBUG_CS (Critical Sections).
 * 
 * 7     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 6     11/21/05 11:22a Nashth
 * Repaired D1 command as per QPR: THNJ6JCL5D.
 * 
 * 5     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 4     5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
