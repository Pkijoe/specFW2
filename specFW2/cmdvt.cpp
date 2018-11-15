//===========================================================================
//
//	Module Name:	cmdVT.cpp
//
// 	Function:	  	This routine is used to select the Normal or m_cViewMode
// 		    		mode of operation.  m_cViewMode switches the X & Y motors.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdVT()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "VT00");

	m_cViewType = (char) (*m_pCmdPtr++ - ASCII0);
	m_nBytesRead++;

	if (m_cViewType != AXIAL_TYPE && m_cViewType != RADIAL_TYPE)
	{
		m_cViewType	= m_NvRam.InputB(VIEW_TYPE);
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		m_NvRam.OutputB(VIEW_TYPE, m_cViewType);
		status = NO_ERRORS;
	}

	return status;
}
//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.

$Log: /WinLab/SpecFW/cmdvt.cpp $
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
