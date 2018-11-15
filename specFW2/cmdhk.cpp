//===========================================================================
//
//	Module Name:	cmdHK.cpp
//
// 	Function:	  	This routine sets the current view motor locations
//		 			as the key Hg values saved in BATTERY RAM.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdHK()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "HK00");
	
	// This command only works in Axial.

		m_nViewXHg_Axial	= m_nViewXno_Axial;
	    m_nViewYHg_Axial	= m_nViewYno_Axial;
        
	    // SAVE KEY VALUE FOR VIEW X
	    m_NvRam.OutputW(HOME_2_HG_X_AXIAL, m_nViewXno_Axial);
        
	    // SAVE KEY VALUE FOR VIEW Y
	    m_NvRam.OutputW(HOME_2_HG_Y_AXIAL, m_nViewYno_Axial);
        
	    return status;
}
//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.


$Log: /WinLab/SpecFW/cmdhk.cpp $
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:16 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
