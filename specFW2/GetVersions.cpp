//===========================================================================
//
//	Module Name:	GetVersions.cpp
//
//	Function:		Implementation file of CGetVersions class which obtains
//					all relevant version information of the system.
//
//	Original Author:	Thomas L Frazzini
//
//	Copyright (c) 2005-2006,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/GetVersions.cpp 7     1/18/06 1:20p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "GetVersions.h"
#include "FileInfo.h"
#include "FileVersionInfo.h"

CGetVersions::CGetVersions()		// standard constructor
{
	m_sVersionInfo = L"Unknown";
}

int ReadLine(	HANDLE hFile, 
				LPVOID lpBuffer, 
				DWORD nNumberOfBytesToRead, 
				LPDWORD lpNumberOfBytesRead)
{
	DWORD	dwBytesRead = nNumberOfBytesToRead;
	DWORD	iLength(0);
	DWORD	i;
	char	inBuffer[255];

	// Find first '\n' and replace by '\0'
	for (i = 0; i < sizeof(inBuffer); i++)
	{
		// Read one character
		ReadFile(	hFile,
					(LPVOID) &inBuffer[i],
					1,
					&dwBytesRead,
					NULL);

		if (dwBytesRead ==0) 
		{
			if (i == 0)		// EOF
			{
				strcpy(inBuffer, "");
				iLength = (DWORD) -1;
				break;
			}
			else			// EOL, no '\r'
			{
				inBuffer[i] = '\0';
				iLength = i;
				break;
			}
		}
		else if ( (inBuffer[i] == '\r') )
		{
			inBuffer[i] = '\0';
			iLength = i;
			break;
		}
		else if ( (inBuffer[i] == '\n') )
		{
			inBuffer[i] = '\0';
			iLength = i;
			break;
		}
	}

	// Set return values
	strcpy((char *)lpBuffer, inBuffer);
	*lpNumberOfBytesRead = iLength; 

	return (iLength);
}
//===========================================================================

#define CPLD				0xa4000000UL
#define CPLD_REV_OFFSET		0x300			// Where the CPLD rev. is stored (16 bits)
#define SIZE_CPLD_AREA		0x1000
extern "C" BOOL VirtualCopy(LPVOID lpvDest, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect);

unsigned short CGetVersions::cpldRev(void)
{
	unsigned short cpldVersion;
	BOOL		   bRet;

	char* CPLD_VirtualMem = (char*)VirtualAlloc(0,SIZE_CPLD_AREA,MEM_RESERVE,PAGE_NOACCESS);
	bRet = VirtualCopy(CPLD_VirtualMem , (PVOID) (CPLD>>8), SIZE_CPLD_AREA, PAGE_READWRITE|PAGE_NOCACHE|PAGE_PHYSICAL);

	cpldVersion = CPLD_VirtualMem[CPLD_REV_OFFSET];

	VirtualFree(CPLD_VirtualMem,SIZE_CPLD_AREA,MEM_RESERVE);

	return(cpldVersion);
}
//===========================================================================

struct VS_VERSIONINFO 
{ 
	WORD				wLength;
	WORD				wValueLength;
	WORD				wType;
	WCHAR				szKey[255];
	WORD				Padding1[255];
	VS_FIXEDFILEINFO	Value;
	WORD				Padding2[255];
	WORD				Children;
};
//===========================================================================

//===========================================================================
//	@func retrieves version info as four short integers 
//
//	@rdesc true if successful 
//===========================================================================

bool GetVersion(LPCTSTR szFileName,		// @parm name of file to check
				short* maj_version,		// @parm pointer to receive major version number
				short* min_version,		// @parm pointer to receiveminor version number
				short* revision,		// @parm pointer to receive revision number
				short* build_number		// @parm pointer to receive build number
				) 
{ 
	// CE doesn't include verion info api

	HMODULE hmodule;
	if ( NULL == szFileName )
	{
		// CE FindResource() does not accept a NULL first parameter
		hmodule = GetModuleHandle( NULL );
	}
	else
	{
		hmodule = ::LoadLibrary( szFileName );
		if ( NULL == hmodule )
			return false;
	}

	bool	ret = false;

	HRSRC hrsrc = ::FindResource(hmodule, MAKEINTRESOURCE( VS_VERSION_INFO), RT_VERSION );
	if ( hrsrc != NULL )
	{
		HGLOBAL hglobal = ::LoadResource( hmodule, hrsrc );
		if ( hglobal != NULL )
		{
			DWORD* pdw = (DWORD*) GlobalLock( hglobal );
			WORD sz = ((VS_VERSIONINFO*) pdw)->wLength;
			VS_FIXEDFILEINFO* pInfo = (VS_FIXEDFILEINFO*)pdw;
			while ( pInfo->dwSignature != 0xfeef04bd )
			{
				pdw++;
				pInfo = (VS_FIXEDFILEINFO*)  pdw;
				if ( sz < sizeof( DWORD ) )
					break;
				sz -= sizeof( DWORD );
			}

			if ( pInfo->dwSignature == 0xfeef04bd )
			{
				*maj_version = HIWORD( pInfo->dwFileVersionMS );
				*min_version = LOWORD( pInfo->dwFileVersionMS);
				*revision =  HIWORD( pInfo->dwFileVersionLS);
				*build_number =  LOWORD( pInfo->dwFileVersionLS);
				ret = true;
			}
			GlobalFree( hglobal );
		}
	}

	if ( NULL != szFileName )
		::FreeLibrary( hmodule );
	return ret; 
}
//===========================================================================

typedef  int (*PTR_INT_SHORTP)(short*);
PTR_INT_SHORTP getPersonalityCPLDVersion;

void CGetVersions::GetVersionInfo()
{
	HINSTANCE curInstance = NULL;

	CString versionString = L"---CPLD version info----\r\n";
	m_sVersionInfo = versionString;

	short cpld = cpldRev();
	versionString.Format(L"Icarus board: %x\r\n",cpld);
	m_sVersionInfo += versionString;

	curInstance = LoadLibrary(L"\\Applications\\GetPersVersion.dll");
	if (curInstance)
	{
		getPersonalityCPLDVersion = NULL;
		getPersonalityCPLDVersion =  (int (__cdecl *)(short*))GetProcAddress( curInstance, L"fnGetPersVersion"); 
		if (getPersonalityCPLDVersion)
		{
			if (getPersonalityCPLDVersion(&cpld) == S_OK)
			{
				versionString.Format(L"Personality board: %x\r\n",cpld);
				m_sVersionInfo += versionString;
			}
			else
			{	// an error.
				versionString.Format(L"Personality board: unknown (getPersonalityCPLDVersion returned an error)\r\n");
				m_sVersionInfo += versionString;
			}
		}
		else
		{
			versionString.Format(L"Personality board: unknown (getPersonalityCPLDVersion not implemented)\r\n");
			m_sVersionInfo += versionString;
		}
		FreeLibrary( curInstance ); // Release the dll
	}
	else
	{	// if the application's file doesn't exist, it isn't an error.
		versionString.Format(L"Personality board: unknown (\\Applications\\GetPersVersion.dll not present)\r\n");
		m_sVersionInfo += versionString;
	}

	// --------- KERNEL
	versionString = L"---Kernel version info----\r\n";
	m_sVersionInfo += versionString;

	TCHAR* tkernelVersion = new TCHAR[255];
	char* kernelVersion = new char[255];
	HKEY hKey = NULL;
	DWORD dwType;
	DWORD dwSize;
	long hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"Ident" , 0, 0, &hKey);
	CString sKernelVersion;
	
	if (ERROR_SUCCESS == hRes) 
	{
		dwSize = 255;
		hRes = RegQueryValueEx (hKey, TEXT("Version"), 0, &dwType,(LPBYTE)tkernelVersion,&dwSize);
		if (ERROR_SUCCESS == hRes) 
		{
            WideCharToMultiByte(CP_ACP,                 // CodePage
                                0,		               // dwFlags
                                tkernelVersion,            // lpWideCharStr
                                -1,                     // cchWideChar
                                kernelVersion,         // lpMultiByteStr
								255,
                                NULL,                   // lpDefaultChar,
                                NULL);                  // lpUsedDefaultChar

			sKernelVersion.Format(L"%s\r\n",tkernelVersion);
			m_sVersionInfo += sKernelVersion;
		}
		else
		{
			sKernelVersion = L"Unknown\r\n";
			m_sVersionInfo += sKernelVersion;
		}
		delete [] kernelVersion;
	}
	else
	{
		sKernelVersion = L"Unknown\r\n";
		m_sVersionInfo += sKernelVersion;
	}

	versionString = L"---Application version info---\r\n";
	m_sVersionInfo += versionString;
	CFile versionFile;
	CFileStatus fileStatus;

	versionFile.GetStatus(L"\\Applications\\VersionInfo.txt",fileStatus);

	HANDLE hInFile = CreateFile(L"\\Applications\\VersionInfo.txt",
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

	if (hInFile == INVALID_HANDLE_VALUE)
	{
		versionString = L"No \\Applications\\VersionInfo.txt file present\r\n";
		m_sVersionInfo += versionString;
	}
	else
	{
#define INSTRING_SIZE	1024

		char    *inString = new char[INSTRING_SIZE];
		DWORD	dwRead;
		bool	bBytesRead;

		bBytesRead = (ReadLine(	hInFile,(LPVOID) inString,INSTRING_SIZE,&dwRead) >= 0);
		while ( bBytesRead )
		{
			if (dwRead)
			{
				versionString.Format(L"%S\r\n",inString);
				m_sVersionInfo += versionString;
			}
			bBytesRead = (ReadLine(	hInFile,(LPVOID) inString,INSTRING_SIZE,&dwRead) >= 0);
		}
		CloseHandle( hInFile );  // Close the VersionInfo.txt file
		delete [] inString;
	}

	CFileInfoArray fia;
	// create a file/directory walker..
	fia.AddDir(	L"\\Applications\\",	// entire directory
				"*.*",					// Filemask (all files)
				TRUE,					// Recurse subdirs
				CFileInfoArray::AP_SORTBYNAME | CFileInfoArray::AP_SORTASCENDING, // Sort by name and ascending
				FALSE );			    // Don't add entries for dirs

	fia.AddDir(	L"\\Windows\\",			// entire directory
				"*.*",					// Filemask (all files)
				TRUE,					// Recurse subdirs
				CFileInfoArray::AP_SORTBYNAME | CFileInfoArray::AP_SORTASCENDING, // Sort by name and ascending
				FALSE );				// Don't add entries for dirs

	int numberOfFiles = fia.GetSize();
	versionString.Format(L"%d file(s) - the following have internal version information\r\n",numberOfFiles);
	for (int i=0; i < numberOfFiles; i++)
	{
		CString fileName = fia[i].GetFilePath();

		DWORD dwDummy;
		LPTSTR ptr = fileName.GetBuffer(255);
		DWORD dwInfoSize = GetFileVersionInfoSize(ptr,&dwDummy);
		if (dwInfoSize)
		{
			CFileVersionInfo fileVersionInfo;
			fileVersionInfo.Create(fileName);
			CString versionInfo = fileVersionInfo.GetProductVersion();
			versionString.Format(L"%40s version: %10s\r\n",fileName, versionInfo);
			m_sVersionInfo += versionString;
		}
	}
}
//===========================================================================

#include "WINERROR.H"
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /IcarusBased/SpecFW/GetVersions.cpp $
 * 
 * 7     1/18/06 1:20p Nashth
 * Repaired a problem with the last fix... not yet released.
 * 
 * 6     1/16/06 2:44p Nashth
 * Made an address and offset correction to the Icarus Board information
 * area as per QPR: THNJ6L4M4X.
 * 
 * 5     10/05/05 12:48p Frazzitl
 * Updated Address Map and TE Cooler voltage calculation constant and
 * related parameters
 * 
 * 4     8/05/05 4:33p Nashth
 * Fixed a never ending While loop to solve QPR: THNJ6EYRYG
 * 
 * 3     8/03/05 3:29p Nashth
 *   Released DLL after use and closed a file after use as per: THNJ6EWPWE
 * 
 * 2     6/17/05 10:08a Frazzitl
 * Removed commented out areas.
 * 
 * 1     6/08/05 10:22a Frazzitl
 * Added classes needed to obtain Kernel Information and Version Numbers

$NoKeywords: $

** End of Rev History **/
