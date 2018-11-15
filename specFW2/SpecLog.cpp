//===========================================================================
//
//	Module Name:	SpecLog.cpp
//
//	Function:		This routine controls access to the Spectrometer Log
//
//	Original Author:	T Nash		March 2008
//
//	Copyright (c) 2008,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================
/***** Last check-in info *****

$Header: /IcarusBased/SpecFW/SpecLog.cpp 1     4/04/08 1:48p Nashth $

(The remainder of the revision history is located at the bottom of this file)

*******************************/
#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
//----------------------------------------------------------------------------
//	AddToSpecLog	Places the given string at the tail of the Spectrometer log
//
//	Note: The caller MUST allocate the passed string dynamically... Other functions will delete
//
#define SPEC_LOG_MAX_ITEMS	1000
//
void	CParserThread::AddToSpecLog(char* szStr)
{
	EnterCriticalSection(&m_CriticalSectionSpecLog);

	// Check for maximum size of this log in firmware
	if ( m_SpecLog.GetCount() >= SPEC_LOG_MAX_ITEMS )
	{
		// Size exceeded... Remove last item from list,
		// check for an existing deletion count to add to,
		// or enter a deletion count of 1.
		//  The given item "szStr" is destroyed without saving 
		int		nDel = 1; // Init to 1 item deleted
		int		nUpdateSize;
		char	szTmp[50];
		char *  pStr = (char*)m_SpecLog.RemoveTail();
		char *  p1   = strstr(pStr, "Deleted");

		// Construct Deleted 999 string into szTmp
		if ( p1 )
		// Existing Deleted keyword found ... strip off and retain value
		{
			nDel += atoi(p1+7); 
			*p1   = '\0';  // Place a Null at the D of Deleted
		}
		int n1 = sprintf(szTmp, "Deleted %u\0", nDel);  
		// Construct updated message containing the Deleted keyword
		nUpdateSize	= strlen(pStr) + n1 + 1; // Size of original string + Deleted string + Null
		char * pUpdate = new char[nUpdateSize];
		strcpy(pUpdate, pStr);
		strcat(pUpdate, szTmp);
		// Clean up the old message and the passed message
		delete [] pStr;
		delete [] szStr;
		// Place updated message back onto tail of list
		m_SpecLog.AddTail(pUpdate);
	}
	else
	// Add string to pointer list
	{
		m_SpecLog.AddTail(szStr);
	}

	LeaveCriticalSection(&m_CriticalSectionSpecLog);
}
//----------------------------------------------------------------------------
//  RemoveFromSpecLog	Removes a sz char string from the head of the list
//                      and returns it's pointer.
//
//  Note: The caller MUST delete the returned string if not NULL.


char*  CParserThread::RemoveFromSpecLog()
{
	char * pStr = NULL;
	EnterCriticalSection(&m_CriticalSectionSpecLog);

	if ( m_SpecLog.GetCount() > 0 )
	{
		pStr = (char*)m_SpecLog.RemoveHead();
	}

	LeaveCriticalSection(&m_CriticalSectionSpecLog);

	return pStr;
}
/***** Revision History *****

$Log: /IcarusBased/SpecFW/SpecLog.cpp $
 * 
 * 1     4/04/08 1:48p Nashth
 * Implementation of spectrometer history log... First pass.
  
$NoKeywords: $

***** End of Rev History ****/
