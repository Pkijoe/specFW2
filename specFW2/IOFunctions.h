//===========================================================================
//
//	Module Name:	IOFunctions.h
//
//	Function:		Header file of Input and Outut functions.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================

#pragma once

class HEXIO : public CWnd
{
// Construction
public:
	HEXIO();
	BYTE	* m_AddrVirtualMem;

	void	OutputW(WORD address, WORD value);
	WORD	InputW( WORD address);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HEXIO)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~HEXIO();

	// Generated message map functions
protected:
	//{{AFX_MSG(HEXIO)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//===========================================================================
class BYTEIO : public CWnd
{
// Construction
public:
	BYTEIO();
	BYTE	* m_AddrVirtualMem;

	void	OutputB(WORD address, BYTE value);
	BYTE	InputB( WORD address);

	void	OutputW(WORD address, WORD value);
	WORD	InputW( WORD address);

	void	OutputL(WORD address, long value);
	long	InputL( WORD address);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BYTEIO)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~BYTEIO();

	// Generated message map functions
protected:
	//{{AFX_MSG(BYTEIO)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//===========================================================================



/*** Revision History ***/
