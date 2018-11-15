//===========================================================================
//
//	Module Name:	cmdIS.cpp
//
//	Function:		This routine has IS to get instrument serial number
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdis.cpp 3     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

// Output Instrument Serial Number - 11 Bytes
unsigned int CParserThread::cmdIS()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "IS00xxxxxxxxxxx");	// 11 character string

	m_nDataOutBuf[ 4] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN     );
	m_nDataOutBuf[ 5] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  1);
	m_nDataOutBuf[ 6] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  2);
	m_nDataOutBuf[ 7] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  3);
	m_nDataOutBuf[ 8] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  4);
	m_nDataOutBuf[ 9] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  5);
	m_nDataOutBuf[10] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  6);
	m_nDataOutBuf[11] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  7);
	m_nDataOutBuf[12] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  8);
	m_nDataOutBuf[13] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN +  9);
	m_nDataOutBuf[14] = (BYTE) m_NvRam.InputB(INSTRUMENT_SN + 10);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdis.cpp $
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/04/05 10:44 Frazzitl
 * Fixing problem with motor timing - only temporary
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
