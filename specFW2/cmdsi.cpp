//===========================================================================
//
//	Module Name:	cmdSI.cpp
//
//	Function:		This routine stores the instrument serial number
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdsi.cpp 5     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

// ENTER INSTRUMENT SERIAL NUMBER - 11 BYTES
unsigned int CParserThread::cmdSI()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "SI00");

	m_NvRam.OutputB(INSTRUMENT_SN	  , (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  1, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  2, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  3, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  4, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  5, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  6, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  7, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  8, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN +  9, (BYTE) (*m_pCmdPtr++));
	m_NvRam.OutputB(INSTRUMENT_SN + 10, (BYTE) (*m_pCmdPtr++));

	m_nBytesRead += 11;

	return(status);
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdsi.cpp $
 * 
 * 5     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 4     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 3     4/04/05 10:44 Frazzitl
 * Fixing problem with motor timing - only temporary
 * 
 * 2     3/30/05 3:44p Frazzitl
 * Fixed problem
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
