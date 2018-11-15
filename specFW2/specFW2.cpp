//===========================================================================
//
//	Module Name:	SpecFW.cpp
//
// 	Function:		Defines the class behaviors for the application.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "stdafx.h"
#include "specFW2.h"
#include "SpecFWDlg.h"
#include "debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CspecFW2App


// CspecFW2App construction

CspecFW2App::CspecFW2App()
	: CWinApp()
{
	lpCriticalSection1 = 0;
}

// The one and only CspecFW2App object

CspecFW2App theApp;

// CspecFW2App initialization

BOOL CspecFW2App::InitInstance()
{
/*  removed jmo
if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

*/
	// Initialize the debug flag from RAM
	SetDebugFlag(DEBUG_CS);  // Initial value until after read from RAM
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	//.............................................................................
	// The following allows only one instance of this firmware application to run

	HANDLE	hActive = ::CreateEvent(NULL, TRUE, TRUE, _T("PESpecFWActiveEvent"));
	if (hActive)
	{	// Check if event already exists
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			::CloseHandle(hActive);
			return FALSE;			// Terminate if an instance is already running
		}
	}
	else
	{
		return FALSE;
	}

	//.............................................................................

	CSpecFWDlg dlg;
	m_pMainWnd = &dlg;

	// This allows this application to be closed so that an update can be done
	// This is part of the download procedure
	// Wrong place - put in OnInitDialog() of CSpecFWDlg
	//	SubscribeToBeKilled("SpecFW", (HWND) m_pMainWnd);

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	//printf("Exited dialog... Closing application\r\n");
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//-----------------------------------------------------------------------------
//  All threads must be registered immediately after creation by calling this function.
//  Call with a name for the thread and the threads ID or handle
void CspecFW2App::RegisterThread(CString cstrName, DWORD dwID)
{
	cstraThreadName.Add(cstrName);
	paThreadId.Add((void*)dwID);
	unaLevel.Add(0);
	if (GetDebugFlag() & DEBUG_CS)
		printf("RegisterThread(%S, %u)\r\n", cstrName, dwID);
}
//-----------------------------------------------------------------------------
//  All registered threads must call this function to enter the main critical section
//  for debug display
//
void CspecFW2App::EnterCriticalSection1(LPCRITICAL_SECTION lpCriticalSection)
{
	void*	pvThread;			 // Thread ID of this thread
	UINT	unLevel = 0;		 // Level
	CString	cstrName("Unknown"); // Thread name

	if ((lpCriticalSection1 != 0) && (lpCriticalSection != lpCriticalSection1))
		ASSERT(0); // Called for other than critical section 1
	else
		lpCriticalSection1 = lpCriticalSection;

	pvThread = (void*)GetCurrentThreadId();

	for (int i = 0; i < paThreadId.GetSize(); i++)
	{
		if (paThreadId[i] == pvThread)
		{
			unaLevel[i]++;
			unLevel = unaLevel[i]; // Get the level and name
			cstrName = cstraThreadName[i];
		}
	}
	if (GetDebugFlag() & DEBUG_CS)
		printf("CS Entered... Level %u, %S\r\n", unLevel, cstrName);
	EnterCriticalSection(lpCriticalSection);  // Enter the critical section
}
//-----------------------------------------------------------------------------
//  All registered threads must call this function to exit the main critical section
//  for debug display
//
void CspecFW2App::LeaveCriticalSection1(LPCRITICAL_SECTION lpCriticalSection)
{
	if ((lpCriticalSection1 != 0) && (lpCriticalSection != lpCriticalSection1))
		ASSERT(0); // Called for other than critical section 1
	else
		lpCriticalSection1 = lpCriticalSection;
	void*	pvThread;			 // Thread ID of this thread
	UINT	unLevel = 0;		 // Level
	CString	cstrName("Unknown"); // Thread name

	if ((lpCriticalSection1 != 0) && (lpCriticalSection != lpCriticalSection1))
		ASSERT(0); // Called for other than critical section 1
	else
		lpCriticalSection1 = lpCriticalSection;

	pvThread = (void*)GetCurrentThreadId();

	for (int i = 0; i<paThreadId.GetSize(); i++)
	{
		if (paThreadId[i] == pvThread)
		{
			unaLevel[i]--;
			unLevel = unaLevel[i]; // Get the level and name
			cstrName = cstraThreadName[i];
		}
	}

	if (GetDebugFlag() & DEBUG_CS)
		printf("CS Exited.... Level %u, %S\r\n", unLevel, cstrName);
	LeaveCriticalSection(lpCriticalSection);  // Leave the critical section
}
//===========================================================================


/*** Revision History ***/

