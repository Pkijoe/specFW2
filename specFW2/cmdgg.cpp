//===========================================================================
//
//	Module Name:	cmdGG.cpp
//
//	Function:		This routine returns the current m_bGrating position:
//					Normal ( "0" ) or Enabled ( "1" ).
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdgg.cpp 1     3/17/05 11:15 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdGG()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "GG00x");			// 1 character string

	memcpy(&m_nDataOutBuf[4], &m_cGrating, 1);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdgg.cpp $
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
