//===========================================================================
//
//	Module Name:	GetTECoolerTemperature.cpp
//
// 	Function:		This routine handles the TE Cooler temperature conversion
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/GetTECoolerTemperature.cpp 4     8/24/05 8:49a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"
// Converts counts to Cooler temperature in degrees C 
// into a scaled x 100 short integer 

short CParserThread::GetTECoolerTemperature(short nCounts)
{
	double	dTemp  = 0.511 * nCounts - 64.8;
			dTemp *= 100;
	short	nTemp  = (short) dTemp;
	return	nTemp;
//	return( m_nTECoolerTemp[nCounts] );
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /IcarusBased/SpecFW/GetTECoolerTemperature.cpp $
 * 
 * 4     8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 3     7/12/05 16:10 Frazzitl
 * Updated equation
 * 
 * 2     5/17/05 15:19 Frazzitl
 * Changed from lookup table to equation
 * 
 * 1     5/13/05 9:06 Frazzitl
 * Functions needed to read detector temperature

$NoKeywords: $

** End of Rev History **/
