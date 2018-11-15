//===========================================================================
//
//	Module Name:	GetKernelInformation.cpp
//
// 	Function:		This routine is a special function that Asked the
//					Icarus board for the kernel version information
//
//	Original Author:	T Frazzini	May 2005
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/GetKernelInformation.cpp 3     6/29/05 15:43 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"
#include "GetVersions.h"

void CParserThread::GetKernelInformation()
{
	CGetVersions			classGetVersions;
							classGetVersions.GetVersionInfo();
	m_sKernelInformation = 	classGetVersions.VersionInfo();
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/GetKernelInformation.cpp $
 * 
 * 3     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 2     5/23/05 16:36 Frazzitl
 * Commented out incorrect code
 * 
 * 1     5/18/05 8:24a Frazzitl
 * New command, but still needs work

$NoKeywords: $

** End of Rev History **/
