//===========================================================================
//
//	Module Name:	cmdGE.cpp
//
//	Function:		This routine transmits Error Messages to the host computer.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdge.cpp 8     6/16/06 2:47p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGE()
{
	theApp.EnterCriticalSection1(&m_CriticalSection);
	WORD	status(NO_ERRORS);
	WORD	i;
	long	lBytesError; // Error buffer byte count

	strcpy(m_nDataOutBuf, "GE00");

	if (m_nErrorCnt > 0)
	{
		lBytesError	= m_nErrorCnt;
		lBytesError *= 4;

		for (i = 0; i < lBytesError; i++)
		{
// TLF - Fixing bug that keeps the Error Code from being returned
// The ErrorMsg() function starts filling in Error Codes at location 4 of the m_nErrorOut buffer
// Therefore, this routine must start reading the same buffer at location 4 when i = 0
//			m_nDataOutBuf[i + 4] = m_nErrorOut[i];
			m_nDataOutBuf[i + 4] = m_nErrorOut[i + 4];
		}
		m_nDataOutBuf[lBytesError + 4] = NULL;
	}
	else
	{
		status = ERR_DATA;	// signal "no data"
		memcpy(&m_nDataOutBuf[2], "04", 2);
		m_nDataOutBuf[4] = NULL;
	}

	m_nErrorCnt	= 0;
	theApp.LeaveCriticalSection1(&m_CriticalSection);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdge.cpp $
 * 
 * 8     6/16/06 2:47p Frazzitl
 * Fixed problem where the buffer containing the Error Code was being read
 * from the wrong starting point.
 * 
 * 7     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 6     8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 5     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 4     5/31/05 18:08 Frazzitl
 * Fixed problem with error handling
 * 
 * 3     5/31/05 4:08p Frazzitl
 * Fixed problem with GE command and removed the BytesOut variable
 * 
 * 2     3/30/05 10:48 Frazzitl
 * Fixed problem with pointer
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
