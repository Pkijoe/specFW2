//===========================================================================
//
//	Module Name:	SpecFW.cpp
//
// 	Function:		Main header file for the SpecFW application.
//
//	Author:			J Orlando
//
//	Copyright (c) 20018,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#pragma once

/* jmo
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
*/

#include "resource.h"       // main symbols
#include "TcpServer.h"

// CspecFW2App:
// See specFW2.cpp for the implementation of this class
//

class CspecFW2App : public CWinApp
{
public:
	CspecFW2App();

	static CTcpServer    *m_Server;

	//.............................................................................
	// Thread pointer information
	CStringArray		cstraThreadName;	// Array of thread names
	CPtrArray			paThreadId;			// Array of thread IDs
	CUIntArray			unaLevel;			// Array of entry levels	

	LPCRITICAL_SECTION	lpCriticalSection1; // Critical section ID 

	void	RegisterThread(CString cstrName, DWORD dwID);
	void	EnterCriticalSection1(LPCRITICAL_SECTION lpCriticalSection);
	void	LeaveCriticalSection1(LPCRITICAL_SECTION lpCriticalSection);
	//.............................................................................
	WORD GetDebugFlag() { return m_nDebug; };
	void SetDebugFlag(WORD wDebug) { m_nDebug = wDebug; };

protected:
	unsigned short	m_nDebug;	// Debug output flag (Copy of ParserThread parameter)

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
public:

//	DECLARE_MESSAGE_MAP()
};

extern CspecFW2App theApp;
