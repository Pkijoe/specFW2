//===========================================================================
//
//	Module Name:	GetDetectorFPGAVersion.cpp
//
// 	Function:		This routine is a special function that gets the
//					Detector Board's FPGA version number
//
//	Original Author:	T Frazzini	May 2005
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/GetFPGADetectorVersion.cpp 5     5/05/06 2:06p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::GetDetectorFPGAVersion()
{
	printf("---Detector FPGA version info---\r\n");

	int		nLenUV(0);
	int		nLenVIS(0);
	char	bufferUV[100];
	char	bufferVIS[100];

	WORD	wVersionVIS;
	WORD	wVersionUV  = m_CameraIO.UVInputW( FPGA_VERSION_NO_UV );
			wVersionUV	= m_IO.InputW(ARRAY_DATA_LATCH_UV);

	if (m_bSingleChannel == false)
	{
			wVersionVIS = m_CameraIO.VISInputW(FPGA_VERSION_NO_VIS);
			wVersionVIS = m_IO.InputW(ARRAY_DATA_LATCH_VIS);

			nLenUV	= sprintf(bufferUV,  "%.1f", (float) wVersionUV	 / 10);
			m_sDetectorFPGAVersionUV	= bufferUV;

			nLenVIS	= sprintf(bufferVIS, "%.1f", (float) wVersionVIS / 10);
			m_sDetectorFPGAVersionVIS	= bufferVIS;
	}
	else
	{
			nLenUV	= sprintf(bufferUV,  "%.1f", (float) wVersionUV  / 10);
			m_sDetectorFPGAVersionUV	= bufferUV;
	}

	if (nLenUV > (sizeof(bufferUV) - 3) )
		nLenUV =  sizeof(bufferUV) - 3;
	bufferUV[nLenUV    ] = '\r';
	bufferUV[nLenUV + 1] = '\n';
	bufferUV[nLenUV + 2] = '\0';
	printf(bufferUV);

	if (m_bSingleChannel == false)
	{
		if (nLenVIS > (sizeof(bufferVIS) - 3) )
			nLenVIS =  sizeof(bufferVIS) - 3;
		bufferVIS[nLenVIS    ] = '\r';
		bufferVIS[nLenVIS + 1] = '\n';
		bufferVIS[nLenVIS + 2] = '\0';
		printf(bufferVIS);
	}
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/GetFPGADetectorVersion.cpp $
 * 
 * 5     5/05/06 2:06p Frazzitl
 * Changed firmware to allow different detector FPGA versions in the same
 * instrument.
 * 
 * 4     10/27/05 4:26p Frazzitl
 * Fixed problem with Single Channel instruments.
 * 
 * 3     10/20/05 12:19p Nashth
 * Fixed buffer overrun problem with buffer.
 * 
 * 2     10/17/05 9:37a Frazzitl
 * Added functions needed to read Detector FPGA version number.
 * 
 * 1     10/12/05 3:31p Frazzitl
 * Added functions to get Detector board FPGA version number

$NoKeywords: $

** End of Rev History **/
