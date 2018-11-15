//===========================================================================
//
//	Module Name:	WriteLogEntry.cpp
//
// 	Function:		This routine Writes debug data to a temporary file
//
//	Original Author:	T Frazzini	May 2005
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/WriteLogEntry.cpp 3     6/17/05 10:02a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"


void CParserThread::WriteLogEntry(char* prefix, char* pText)
{
	CFile			F;
	CFileException	CFE;
	TCHAR			LogPath[MAX_PATH];
	char			TT[80];

	// No null pointers, man
	if((pText == (char*) NULL) || (strlen(pText) == 0)) return;

	// Construct todays log filename
	CTime	theTime = CTime::GetCurrentTime();
	swprintf(LogPath,L"\\My Documents\\%S.log", prefix);

	// Open existing or create new; seek to end, write text and close file
	if(F.Open(LogPath, (CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite), &CFE))
	{
		sprintf(TT, "Entry: ");
		F.SeekToEnd();
		F.Write(TT, strlen(TT));
		F.Write(pText, strlen(pText));
		F.Write("\r\n", 2);
		F.Close();
	}
	return;
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/WriteLogEntry.cpp $
 * 
 * 3     6/17/05 10:02a Frazzitl
 * Removed commented out areas that did not do what was expected.
 * 
 * 2     5/27/05 10:30 Frazzitl
 * Corrected problem with sum long and sum data
 * 
 * 1     5/16/05 15:48 Frazzitl
 * Added debug feature for logging to a file

$NoKeywords: $

** End of Rev History **/
