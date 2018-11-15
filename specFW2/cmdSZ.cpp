//===========================================================================
//
//	Module Name:	cmdSZ.cpp
//
//	Function:		(Spectrometer Log) Get an item from the spectrometer log
//
//	Original Author:	TNash
//
// Copyright © 2008  PerkinElmer Inc,  All rights reserved.
//
//===========================================================================
/***** Last check-in info *****

$Header: /IcarusBased/SpecFW/cmdSZ.cpp 1     4/04/08 1:48p Nashth $

(The remainder of the revision history is located at the bottom of this file)

*******************************/
#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdSZ()
{
	WORD	status(NO_ERRORS);

	theApp.EnterCriticalSection1(&m_CriticalSection); 

	char* psz1 = RemoveFromSpecLog();
	if ( !psz1 )
	{
		strcpy(m_nDataOutBuf, "SZ00None");	// No error 
	}
	else
	{
		sprintf(m_nDataOutBuf, "SZ00%s", psz1);
		delete [] psz1;

	}
	theApp.LeaveCriticalSection1(&m_CriticalSection); 
	return status;
}
/***** Revision History *****

$Log: /IcarusBased/SpecFW/cmdSZ.cpp $
 * 
 * 1     4/04/08 1:48p Nashth
 * Implementation of spectrometer history log... First pass.
  
$NoKeywords: $

***** End of Rev History ****/