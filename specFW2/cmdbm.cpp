//===========================================================================
//
//	Module Name:	cmdBM.cpp
//
//	Function:		This routine sets the current max step value as
//					the maximum value saved in BATTERY RAM.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdBS()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "BS00");

	/* SAVE MAX VALUE FOR SLIT */
	m_NvRam.OutputW(MAX_SLIT, m_nMaxSlitStep);

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdBX()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "BX00");

	// Write to the Axial or Radial based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
         /* SAVE MAX VALUE FOR VIEW X */
	     m_NvRam.OutputW(MAX_VIEWX_AXIAL, m_nMaxXSteps_Axial);
 	  }
	else  // RADIAL_TYPE
	  {	
        /* SAVE MAX VALUE FOR VIEW X */
	    m_NvRam.OutputW(MAX_VIEWX_RADIAL, m_nMaxXSteps_Radial);	  
	  } 

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdBY()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "BY00");
	
	// Write to the Axial or Radial based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
	    /* SAVE MAX VALUE FOR VIEW Y */
	    m_NvRam.OutputW(MAX_VIEWY_AXIAL, m_nMaxYSteps_Axial);
 	  }
	else  // RADIAL_TYPE
	  {	
	    /* SAVE MAX VALUE FOR VIEW Y */
	    m_NvRam.OutputW(MAX_VIEWY_RADIAL, m_nMaxYSteps_Radial);  
	  } 

	return status;
}
//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.

$Log: /WinLab/SpecFW/cmdbm.cpp $
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:13 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
