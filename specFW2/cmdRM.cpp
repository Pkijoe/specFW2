//===========================================================================
//
//	Module Name:	cmdRM.cpp
//
// 	Function:	  	This routine can enable Ramp mode for testing.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdRM.cpp 1     5/26/05 8:56 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdRM()
{
	unsigned int	status(NO_ERRORS);
	char	cMode;

	strcpy(m_nDataOutBuf, "RM00");

	cMode = *m_pCmdPtr++;
	m_nBytesRead++;

	if (cMode == '0')
		m_bRampMode = false;
	else if (cMode == '1')
		m_bRampMode = true;
	else
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdRM.cpp $
 * 
 * 1     5/26/05 8:56 Frazzitl
 * Added ability to select ramp mode

$NoKeywords: $

** End of Rev History **/
