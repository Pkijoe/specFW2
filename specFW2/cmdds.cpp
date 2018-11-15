//===========================================================================
//
//	Module Name:	cmdDS.cpp
//
//	Function:		This routine has DU,DV,SN
//
//	Original Author:	Ellen C. Deutsch Paz
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdds.cpp 4     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

// ENTER UV DETECTOR SERIAL NUMBER - 6 BYTES
unsigned int CParserThread::cmdDU()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "DU00");

	m_NvRam.OutputB(UVDETSN		, *m_pCmdPtr++);
	m_NvRam.OutputB(UVDETSN +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(UVDETSN +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(UVDETSN +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(UVDETSN +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(UVDETSN +  5, *m_pCmdPtr++);

	m_nBytesRead += 6;

	return status;
}
//===========================================================================

// ENTER VIS DETECTOR SERIAL NUMBER - 6 BYTES
unsigned int CParserThread::cmdDV()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "DV00");

	m_NvRam.OutputB(VISDETSN	 , *m_pCmdPtr++);
	m_NvRam.OutputB(VISDETSN +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(VISDETSN +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(VISDETSN +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(VISDETSN +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(VISDETSN +  5, *m_pCmdPtr++);

	m_nBytesRead += 6;

	return status;
}
//===========================================================================

// OUTPUT UV & VIS DETECTOR SERIAL NUMBER - 6 BYTES EACH
unsigned int CParserThread::cmdSN()
{
	WORD	status(NO_ERRORS);
//						 "01234567890123456789012"
	strcpy(m_nDataOutBuf, "SN00UVxxxxxx VISxxxxxx ");

	m_nDataOutBuf[ 6] = (BYTE) m_NvRam.InputB(UVDETSN     );
	m_nDataOutBuf[ 7] = (BYTE) m_NvRam.InputB(UVDETSN +  1);
	m_nDataOutBuf[ 8] = (BYTE) m_NvRam.InputB(UVDETSN +  2);
	m_nDataOutBuf[ 9] = (BYTE) m_NvRam.InputB(UVDETSN +  3);
	m_nDataOutBuf[10] = (BYTE) m_NvRam.InputB(UVDETSN +  4);
	m_nDataOutBuf[11] = (BYTE) m_NvRam.InputB(UVDETSN +  5);
	m_nDataOutBuf[12] = (BYTE)  SPACE;

	if (m_bSingleChannel)
	{
		m_nDataOutBuf[16] = '0';
		m_nDataOutBuf[17] = '0';
		m_nDataOutBuf[18] = '0';
		m_nDataOutBuf[19] = 'A';
		m_nDataOutBuf[20] = '0';
		m_nDataOutBuf[21] = '0';
	}
	else
	{
		m_nDataOutBuf[16] = (BYTE) m_NvRam.InputB(VISDETSN     );
		m_nDataOutBuf[17] = (BYTE) m_NvRam.InputB(VISDETSN +  1);
		m_nDataOutBuf[18] = (BYTE) m_NvRam.InputB(VISDETSN +  2);
		m_nDataOutBuf[19] = (BYTE) m_NvRam.InputB(VISDETSN +  3);
		m_nDataOutBuf[20] = (BYTE) m_NvRam.InputB(VISDETSN +  4);
		m_nDataOutBuf[21] = (BYTE) m_NvRam.InputB(VISDETSN +  5);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdds.cpp $
 * 
 * 4     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 3     4/04/05 10:44 Frazzitl
 * Fixing problem with motor timing - only temporary
 * 
 * 2     3/30/05 2:59p Frazzitl
 * Fixed some startup problems
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
