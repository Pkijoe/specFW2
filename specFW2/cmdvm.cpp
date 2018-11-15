//===========================================================================
//
//	Module Name:	cmdVM.cpp
//
// 	Function:	  	This routine passes the current steps from HOME (0).
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdVS()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "VS00xxxxx");	// 5 character string

	s2b(m_nSlitSno, 4);					// CURRENT SLIT POSITION

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdVX()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "VX00xxxxx");	// 5 character string
	
	// Send the Axial or Radial variables based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
         s2b(m_nViewXno_Axial, 4);				// CURRENT VIEW X POSITION
 	  }
	else  // RADIAL_TYPE
	  {	
         s2b(m_nViewXno_Radial, 4);				// CURRENT VIEW X POSITION
	  } 

	return status;
}
//===========================================================================

unsigned int CParserThread::cmdVY()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "VY00xxxxx");	// 5 character string
	
	// Send the Axial or Radial variables based on the current "View Type"
	
	if (m_cViewType == AXIAL_TYPE) 
	  {
		s2b(m_nViewYno_Axial, 4);				// CURRENT VIEW Y POSITION
 	  }
	else  // RADIAL_TYPE
	  {	
		s2b(m_nViewYno_Radial, 4);				// CURRENT VIEW Y POSITION
	  } 

	return status;
}
//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.


$Log: /WinLab/SpecFW/cmdvm.cpp $
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
