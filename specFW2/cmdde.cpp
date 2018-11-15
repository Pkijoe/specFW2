//===========================================================================
//
//	Module Name:	cmdDE.cpp
//
//	Function:		(Detecor Error) Get Detector Read Error information.
//
//	Original Author:	TNash
//
// Copyright © 2007  PerkinElmer Inc,  All rights reserved.
//
//===========================================================================
/***** Last check-in info *****

$Header: /IcarusBased/SpecFW/cmdde.cpp 3     7/09/07 10:35a Nashth $

(The remainder of the revision history is located at the bottom of this file)

*******************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdDE()
{
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection); 
	if ( !m_bDetectorReadErr )
	{
		strcpy(m_nDataOutBuf, "DE00None");	// No error 
	}
	else
	{
		WORD	wRevStatus = (WORD) REVWORD(m_wStatus);
		char	szStatus[3]; 	// Status string reverse byte order
		char	szUVIS[4];		// VIS or UV
		char	szReso[5];		// High or Low
		char	szExtra[6];		// Extra or "     "
		char	szOver[4];		// Yes or No
		
		memcpy(szStatus, &wRevStatus, 2);
		szStatus[2] = '\0'; 

		if		( m_wErrUVorVIS == 1 )
			 strcpy(szUVIS, "UV ");
		else strcpy(szUVIS, "VIS");

		if		( m_bErrHiRes )
			 strcpy(szReso, "High");
		else strcpy(szReso, "Low ");

		if ( m_bErrExtraPixels )
			 strcpy(szExtra, "Extra ");
		else strcpy(szExtra, "      ");

		if ( m_bErrOverScan )
			 strcpy(szOver, "Yes");
		else strcpy(szOver, "No ");

		sprintf(m_nDataOutBuf, "DE00 %s <%s> %s Sub=%3d Pixels=%3d %s %sPixel=%3d Overscan=%s", 
			     m_szDateTimeBuf, szStatus, szUVIS, m_wErrSA, m_wErrPixels, szReso, szExtra, m_wErrPixel, szOver);

		m_bDetectorReadErr = false;	// Reset the error for next time
	}
	theApp.LeaveCriticalSection1(&m_CriticalSection); 
	return status;
}
//===========================================================================
/***** Revision History *****

$Log: /IcarusBased/SpecFW/cmdde.cpp $
 * 
 * 3     7/09/07 10:35a Nashth
 * Adjusted format statement.
 * 
 * 2     7/09/07 10:19a Nashth
 * slight change to format statement.
 * 
 * 1     7/09/07 9:25a Nashth
  
$NoKeywords: $

***** End of Rev History ****/