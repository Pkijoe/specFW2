//===========================================================================
//
//	Module Name:	IOFunctions.cpp
//
//	Function:		Implementation of Input and Outut functions.
//
//	Original Author:	Thomas L Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/IOFunctions.cpp 4     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "IOFunctions.h"

#define ADDR_8BITAREA 0xB8000000
#define SIZE_8BITAREA 0x8000

#define ADDR_16BITAREA 0xA6000000
#define SIZE_16BITAREA (4096)

extern "C" BOOL VirtualCopy(LPVOID lpvDest, LPVOID lpvSrc, DWORD cbSize, DWORD fdwProtect);

//===========================================================================
// HEXIO

HEXIO::HEXIO () 

{
	m_AddrVirtualMem = (BYTE*)VirtualAlloc(0, SIZE_16BITAREA, MEM_RESERVE, PAGE_NOACCESS);

	VirtualCopy(m_AddrVirtualMem, (PVOID) (ADDR_16BITAREA>>8), SIZE_16BITAREA, PAGE_READWRITE | PAGE_NOCACHE | PAGE_PHYSICAL);
}
//===========================================================================

HEXIO::~HEXIO()
{
}
//===========================================================================

VOID HEXIO::OutputW(WORD offset, WORD data) // to get to A6000300, the offset will be 0x300
{
	WORD *myword;

	myword = (WORD*) &m_AddrVirtualMem[offset];
	*myword = data;
}
//===========================================================================

WORD HEXIO::InputW(WORD offset)
{
	WORD *myword;
	WORD data;
	myword = (WORD*) &m_AddrVirtualMem[offset];
	data = *myword;
	return data;
}
//===========================================================================

BEGIN_MESSAGE_MAP(HEXIO, CWnd)
	//{{AFX_MSG_MAP(HEXIO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===========================================================================
// HEXIO message handlers

/////////////////////////////////////////////////////////////////////////////

//===========================================================================
// BYTEIO

BYTEIO::BYTEIO()
{
	m_AddrVirtualMem = (BYTE*)VirtualAlloc(0, SIZE_8BITAREA, MEM_RESERVE, PAGE_NOACCESS);

	VirtualCopy(m_AddrVirtualMem, (PVOID) (ADDR_8BITAREA>>8), SIZE_8BITAREA, PAGE_READWRITE | PAGE_NOCACHE | PAGE_PHYSICAL);
}
//===========================================================================

BYTEIO::~BYTEIO()
{
}
//===========================================================================

VOID BYTEIO::OutputB(WORD offset, BYTE data) // to get to B0000300, the offset will be 0x300
{
	BYTE *myword;

	myword = (BYTE*) &m_AddrVirtualMem[offset];
	*myword = data;
}
//===========================================================================

BYTE BYTEIO::InputB(WORD offset)
{
	BYTE *myword;
	BYTE data;
	myword = (BYTE*) &m_AddrVirtualMem[offset];
	data = *myword;
	return data;
}
//===========================================================================

VOID BYTEIO::OutputW(WORD offset, WORD data) // to get to B0000300, the offset will be 0x300
{
	WORD *myword;

	myword = (WORD*) &m_AddrVirtualMem[offset];
	*myword = data;
}
//===========================================================================

WORD BYTEIO::InputW(WORD offset)
{
	WORD *myword;
	WORD data;
	myword = (WORD*) &m_AddrVirtualMem[offset];
	data = *myword;
	return data;
}
//===========================================================================

VOID BYTEIO::OutputL(WORD offset, long data) // to get to B0000300, the offset will be 0x300
{
//	long *myword;
//
//	myword = (long*) &m_AddrVirtualMem[offset];
//	*myword = data;
	WORD	wTemp;
	long	lTemp = data;
	lTemp  &= 0xffff0000;
	lTemp >>= 16;
	wTemp = (WORD) lTemp;
	OutputW(offset, wTemp);		// most significant word part

	lTemp = data;
	lTemp &= 0x0000ffff;
	wTemp = (WORD) lTemp;
	OutputW((WORD) (offset + 2), wTemp);	// least significant word part
}
//===========================================================================

long BYTEIO::InputL(WORD offset)
{
//	long *myword;
	long data;
//	myword = (long*) &m_AddrVirtualMem[offset];
//	data = *myword;

	long	lMSW = InputW(offset);		// most significant word part
	long	lLSW = InputW((WORD) (offset + 2));	// least significant word part
	data   = lMSW;
	data <<= 16;
	data  |= lLSW;

	return data;
}
//===========================================================================

BEGIN_MESSAGE_MAP(BYTEIO, CWnd)
	//{{AFX_MSG_MAP(BYTEIO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===========================================================================
// BYTEIO message handlers

//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /WinLab/SpecFW/IOFunctions.cpp $
 * 
 * 4     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 3     4/07/05 1:32p Frazzitl
 * Fixed problem with clock
 * 
 * 2     4/01/05 15:20 Frazzitl
 * Corrected Battery RAM location
 * 
 * 1     3/17/05 11:22 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
