//===========================================================================
//
//	Module Name:	cmdSY.cpp
//
// 	Function:	  	This routine is used to turn ON or OFF the syncing
//	 		    	of the START INTEGRATION with the 60 Hz interrupt signal.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdsy.cpp 3     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSY()
{
	WORD	status(NO_ERRORS);

	m_pCmdPtr++;	// just ignore character since command for compatability only
	m_nBytesRead++;

	strcpy(m_nDataOutBuf, "SY00");

	m_bSyncMode = false;
	m_NvRam.OutputB(SYNC_MODE, m_bSyncMode);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdsy.cpp $
 * 
 * 3     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
