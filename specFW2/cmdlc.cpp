//===========================================================================
//
//	Module Name:	cmdLC.cpp
//
// 	Function:		This routine sets up the read table.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdlc.cpp 2     4/28/05 10:44 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdLN()
{
	WORD	status(NO_ERRORS);
	WORD	cnt(0);
	WORD	addr(0);
	short	n;

	m_nArraysUV		= 0;
	m_nArraysVIS	= 0;
	m_nTotalArrays	= 0;

	strcpy(m_nDataOutBuf, "LN00");

	cnt = b2c();						// Number of Subarrays to read
	if (cnt == 0)
	{
		status = ERR_PARA;
		memcpy(&m_nDataOutBuf[2], "07", 2);
	}
	else
	{
		for (n = 0; n < cnt; n++)
		{
			addr = b2c();
			if (m_cOperationMode == AUTO)
			{	// All subarrays are read via the VIS detector
				m_ReadTableVIS[m_nArraysVIS++].array = m_pVegaArray[addr];
				m_nTotalArrays++;
			}
			else
			{
				if (m_pVegaArray[addr]->detector == UV)
				{
					m_ReadTableUV[m_nArraysUV++].array = m_pVegaArray[addr];
					m_nTotalArrays++;
				}
				else if (m_pVegaArray[addr]->detector == VIS)
				{
					m_ReadTableVIS[m_nArraysVIS++].array = m_pVegaArray[addr];
					m_nTotalArrays++;
				}
				else if ( m_pVegaArray[addr]->detector == SPARE)
				{	// SPARE SUB-ARRAYS ILLEGAL DURING NORMAL MODE
					status = ERR_ARRAY;
					memcpy(&m_nDataOutBuf[2], "06", 2);
					break;
				}
			}
		}
	}

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdlc.cpp $
 * 
 * 2     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 1     3/17/05 11:17 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
