//===========================================================================
//
//	Module Name:	CameraIOFunctions.h
//
//	Function:		Header file of Camera Input and Outut functions.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "IOFunctions.h"

//===========================================================================
class CameraHEXIO : public HEXIO
{
// Construction
public:
	CameraHEXIO();
	BYTE	* m_AddrVirtualMem;

	void	UVOutputW(	WORD address, WORD value);
	WORD	UVInputW(	WORD address			);
	void	VISOutputW(	WORD address, WORD value);
	WORD	VISInputW(	WORD address			);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides

// Implementation
public:
	virtual ~CameraHEXIO();

	// Generated message map functions
protected:

	DECLARE_MESSAGE_MAP()
};
//===========================================================================

/*** Revision History ***/

