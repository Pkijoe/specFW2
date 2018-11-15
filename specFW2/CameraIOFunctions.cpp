//===========================================================================
//
//	Module Name:	CameraIOFunctions.cpp
//
//	Function:		Implementation of Camera Input and Outut functions.
//
//	Original Author:	Thomas L Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/CameraIOFunctions.cpp 5     6/29/05 15:43 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "CameraIOFunctions.h"
#include "Hardware.h"

#define ADDR_16BITAREA 0xA6000000
#define SIZE_16BITAREA (4096)

extern "C" BOOL VirtualCopy(LPVOID lpvDest, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect);

//===========================================================================
// CameraHEXIO

CameraHEXIO::CameraHEXIO () 

{
	m_AddrVirtualMem = (BYTE*)VirtualAlloc(0, SIZE_16BITAREA, MEM_RESERVE, PAGE_NOACCESS);

	VirtualCopy(m_AddrVirtualMem, (PVOID) (ADDR_16BITAREA>>8), SIZE_16BITAREA, PAGE_READWRITE | PAGE_NOCACHE | PAGE_PHYSICAL);
}
//===========================================================================

CameraHEXIO::~CameraHEXIO()
{
}
//===========================================================================

VOID CameraHEXIO::UVOutputW(WORD offset, WORD data) // to get to A6000300, the offset will be 0x300
{
	WORD *myword;
	volatile WORD dummy;

	myword = (WORD*) &m_AddrVirtualMem[offset];
	*myword = data;
	myword = (WORD*) &m_AddrVirtualMem[STATUS_REGISTER];	//point to status reg
	dummy = *myword;
	short	nCnt(0);
	do {
		dummy = *myword;				
		if (++nCnt > 10)
			return;
	} while (!(dummy & STATUS_ERROR_UV));					//should check ack after new FPGA
}
//===========================================================================

WORD CameraHEXIO::UVInputW(WORD offset)
{
	WORD *myword;
	WORD data, dummy;
	myword = (WORD*) &m_AddrVirtualMem[offset];
	data = *myword;
	myword = (WORD*) &m_AddrVirtualMem[STATUS_REGISTER];	//point to status reg
	dummy = *myword;
	short	nCnt(0);
	do {
		dummy = *myword;
		if (++nCnt > 10)
			return data;
	} while (!(dummy & STATUS_ERROR_UV));					//should check ack after new FPGA
	return data;
}
//===========================================================================

VOID CameraHEXIO::VISOutputW(WORD offset, WORD data) // to get to A6000300, the offset will be 0x300
{
	WORD *myword;
	volatile WORD dummy;

	myword = (WORD*) &m_AddrVirtualMem[offset];
	*myword = data;
	myword = (WORD*) &m_AddrVirtualMem[STATUS_REGISTER];	//point to status reg
	dummy = *myword;
	short	nCnt(0);
	do {
		dummy = *myword;				
		if (++nCnt > 10)
			return;
	} while (!(dummy & STATUS_ERROR_VIS));					//should check ack after new FPGA
}
//===========================================================================

WORD CameraHEXIO::VISInputW(WORD offset)
{
	WORD *myword;
	WORD data, dummy;
	myword = (WORD*) &m_AddrVirtualMem[offset];
	data = *myword;
	myword = (WORD*) &m_AddrVirtualMem[STATUS_REGISTER];	//point to status reg
	dummy = *myword;
	short	nCnt(0);
	do {
		dummy = *myword;
		if (++nCnt > 10)
			return data;
	} while (!(dummy & STATUS_ERROR_VIS));					//should check ack after new FPGA
	return data;
}
//===========================================================================

BEGIN_MESSAGE_MAP(CameraHEXIO, CWnd)
	//{{AFX_MSG_MAP(CameraHEXIO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===========================================================================
// CameraHEXIO message handlers

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /IcarusBased/SpecFW/CameraIOFunctions.cpp $
 * 
 * 5     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 4     6/17/05 9:58a Frazzitl
 * Fixed problem caused by a channel not being present and locking up the
 * system - added a timeout check.
 * 
 * 3     6/07/05 8:25a Frazzitl
 * Separated camera I/O into UV and VIS routines to accommodate individual
 * status checking
 * 
 * 2     6/03/05 11:28 Frazzitl
 * Modified the code so that the compiler does not strip out the hardware
 * reads that are used for timing delays - Sunita
 * 
 * 1     5/26/05 16:07 Frazzitl
 * Separated camera IO form normal IO in order to add a delay

$NoKeywords: $

** End of Rev History **/
