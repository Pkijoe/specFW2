//===========================================================================
//
//	Module Name:	limit.cpp
//
//	Function:		Function limit checks new value.
//
//	Original Author:	TJWatko
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/limit.cpp 2     6/06/05 3:36p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::Limit(long min, long * newVal, long max)
{
	if (*newVal < min)
		*newVal = min;
	else if (*newVal > max)
		*newVal = max;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/limit.cpp $
 * 
 * 2     6/06/05 3:36p Frazzitl
 * Removed warnings during build
 * 
 * 1     3/17/05 11:22 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
