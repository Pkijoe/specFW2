//===========================================================================
//
//	Module Name:	SpecFWDlg.cpp
//
// 	Function:		Dummy dialog header file.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================


#pragma once

#include "CommandServer.h"

/////////////////////////////////////////////////////////////////////////////
// CSpecFWDlg dialog

class CSpecFWDlg : public CDialog
{
// Construction
public:
	CSpecFWDlg(CWnd* pParent = NULL);	// standard constructor
	~CSpecFWDlg();

	CCommandServer *CommandServer;

	BOOL	Suspend();

// Dialog Data
	//{{AFX_DATA(CSpecFWDlg)
	enum { IDD = IDD_SPECFW_DIALOG };
	CButton	m_StartStopButton;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecFWDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	bool	bRunning;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSpecFWDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStartstop();
	afx_msg LRESULT OnHintStartAppRemote(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/*** Revision History ***/


