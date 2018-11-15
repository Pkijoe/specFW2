//===========================================================================
//
//	Module Name:	GetKernelVersion.cpp
//
// 	Function:		This routine is a special function that Asked the
//					Icarus board for the kernel version number
//
//	Original Author:	T Frazzini	May 2005
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/GetKernelVersion.cpp 1     5/05/05 10:55 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::GetKernelVersion()
{
	CString	versionString = L"---Kernel version info----";

	printf("---Kernel version info---\r\n");

	TCHAR*	tkernelVersion = new TCHAR[255];
	char*	kernelVersion = new char[255];
	HKEY	hKey = NULL;
	DWORD	dwType;
	DWORD	dwSize;
	long	hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"Ident", 0, 0, &hKey);

	if (hRes == ERROR_SUCCESS) 
	{
		dwSize = 255;
		hRes = RegQueryValueEx (hKey, TEXT("Version"), 0, &dwType,(LPBYTE)tkernelVersion, &dwSize);
		if (hRes == ERROR_SUCCESS) 
		{
			WideCharToMultiByte(CP_ACP,			// CodePage
								0,				// dwFlags
								tkernelVersion,	// lpWideCharStr
								-1,				// cchWideChar
								kernelVersion,	// lpMultiByteStr
								255,
								NULL,			// lpDefaultChar,
								NULL);			// lpUsedDefaultChar

			printf((char*) kernelVersion);
			printf("\r\n");
			m_sKernelVersion.Format(L"%s", tkernelVersion);     
		}
		else
		{
			printf("Unknown\r\n");
			m_sKernelVersion = L"Unknown";
		}
		delete [] kernelVersion;
	}
	else
	{
		printf("Unknown\r\n");
		m_sKernelVersion = L"Unknown";
	}
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/GetKernelVersion.cpp $
 * 
 * 1     5/05/05 10:55 Frazzitl
 * Added get Kernel version routine

$NoKeywords: $

** End of Rev History **/
