//===========================================================================
//
//	Module Name:	SpecFWDlg.cpp
//
// 	Function:		Dummy dialog implementation.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "SpecFW2.h"
#include "SpecFWDlg.h"
#include "KillSubscriber.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

#define	HINT_START_REMOTE	0x0101

/////////////////////////////////////////////////////////////////////////////
// CSpecFWDlg dialog

CSpecFWDlg::CSpecFWDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpecFWDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecFWDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bRunning = false;

	CommandServer = (CCommandServer*) ::AfxBeginThread( RUNTIME_CLASS( CCommandServer ), THREAD_PRIORITY_BELOW_NORMAL );
	ASSERT( CommandServer );
	theApp.RegisterThread(L"Afx CommandServer", (DWORD)CommandServer); // Assumption: ptr->Thread == hThread == dwThreadID

}
//===========================================================================

CSpecFWDlg::~CSpecFWDlg()
{
	CommandServer->ExitServerThread();
	delete CommandServer;
}
//===========================================================================

void CSpecFWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecFWDlg)
	DDX_Control(pDX, IDC_BUTTON_STARTSTOP, m_StartStopButton);
	//}}AFX_DATA_MAP
}
//===========================================================================

BEGIN_MESSAGE_MAP(CSpecFWDlg, CDialog)
	//{{AFX_MSG_MAP(CSpecFWDlg)
	ON_MESSAGE   (HINT_START_REMOTE,	OnHintStartAppRemote)
	ON_BN_CLICKED(IDC_BUTTON_STARTSTOP, OnButtonStartstop	)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecFWDlg message handlers

BOOL CSpecFWDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	SubscribeToBeKilled("SpecFW2", (HWND) m_hWnd);

//	PostMessage(HINT_START_REMOTE, NULL, NULL);
	OnButtonStartstop(); // hit with a hammer to start this thing
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//===========================================================================

LRESULT CSpecFWDlg::OnHintStartAppRemote(WPARAM wParam, LPARAM lParam)
{
	short	nValue((short) wParam);
	short	nTemp( (short) lParam);
	do
	{
		// Break here to change value of nValue
		nValue	= 0;
		nTemp	= nValue;
		switch (nValue)
		{
			case 1:
				CommandServer->CreateServerThread();
				bRunning = true;
				m_StartStopButton.SetWindowText(L"Stop Firmware");
				break;
			case 2:
				CommandServer->ExitServerThread();
				m_StartStopButton.SetWindowText(L"Start Firmware");
				bRunning = false;
				break;
			default:
				break;
		}
		for (nTemp = 0; nTemp < 10; nTemp++)
			Suspend();
	} while (nValue != 99);

	CDialog::OnOK();

	return 0;
}
//===========================================================================

void CSpecFWDlg::OnButtonStartstop() 
{
	if (bRunning)
	{	
		CommandServer->ExitServerThread();
		m_StartStopButton.SetWindowText(L"Start Firmware");
		bRunning = false;
	}
	else
	{
		CommandServer->CreateServerThread();
		bRunning = true;
		m_StartStopButton.SetWindowText(L"Stop Firmware");
	}
}
//===========================================================================

BOOL CSpecFWDlg::Suspend()
{
	BOOL bMode(TRUE);
	MSG  msg;

	if (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		bMode = ::AfxGetApp()->PumpMessage();
	}
	return bMode;
}
//===========================================================================


/*** Revision History ***/
