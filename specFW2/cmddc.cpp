//===========================================================================
//
//	Module Name:	cmdDC.cpp
//
//	Function:		This routine has RU,RV,SU,SV
//
//	Original Author:	Ellen C. Deutsch Paz
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmddc.cpp 2     4/20/05 11:28 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

// OUTPUT UV COEFFICIENTS, 3 EACH, IN THE FORM:  +####E+##
unsigned int CParserThread::cmdRU()
{
	WORD	status(NO_ERRORS);
	char	cUVCoefficient1[9];
	char	cUVCoefficient2[9];
	char	cUVCoefficient3[9];

	strcpy(m_nDataOutBuf, "RU00UVxxxxxxxxx xxxxxxxxx xxxxxxxxx ");

	cUVCoefficient1[0] = m_NvRam.InputB(UVCOEF1     );
	cUVCoefficient1[1] = m_NvRam.InputB(UVCOEF1 +  1);
	cUVCoefficient1[2] = m_NvRam.InputB(UVCOEF1 +  2);
	cUVCoefficient1[3] = m_NvRam.InputB(UVCOEF1 +  3);
	cUVCoefficient1[4] = m_NvRam.InputB(UVCOEF1 +  4);
	cUVCoefficient1[5] = m_NvRam.InputB(UVCOEF1 +  5);
	cUVCoefficient1[6] = m_NvRam.InputB(UVCOEF1 +  6);
	cUVCoefficient1[7] = m_NvRam.InputB(UVCOEF1 +  7);
	cUVCoefficient1[8] = m_NvRam.InputB(UVCOEF1 +  8);

	cUVCoefficient2[0] = m_NvRam.InputB(UVCOEF2     );
	cUVCoefficient2[1] = m_NvRam.InputB(UVCOEF2 +  1);
	cUVCoefficient2[2] = m_NvRam.InputB(UVCOEF2 +  2);
	cUVCoefficient2[3] = m_NvRam.InputB(UVCOEF2 +  3);
	cUVCoefficient2[4] = m_NvRam.InputB(UVCOEF2 +  4);
	cUVCoefficient2[5] = m_NvRam.InputB(UVCOEF2 +  5);
	cUVCoefficient2[6] = m_NvRam.InputB(UVCOEF2 +  6);
	cUVCoefficient2[7] = m_NvRam.InputB(UVCOEF2 +  7);
	cUVCoefficient2[8] = m_NvRam.InputB(UVCOEF2 +  8);

	cUVCoefficient3[0] = m_NvRam.InputB(UVCOEF3     );
	cUVCoefficient3[1] = m_NvRam.InputB(UVCOEF3 +  1);
	cUVCoefficient3[2] = m_NvRam.InputB(UVCOEF3 +  2);
	cUVCoefficient3[3] = m_NvRam.InputB(UVCOEF3 +  3);
	cUVCoefficient3[4] = m_NvRam.InputB(UVCOEF3 +  4);
	cUVCoefficient3[5] = m_NvRam.InputB(UVCOEF3 +  5);
	cUVCoefficient3[6] = m_NvRam.InputB(UVCOEF3 +  6);
	cUVCoefficient3[7] = m_NvRam.InputB(UVCOEF3 +  7);
	cUVCoefficient3[8] = m_NvRam.InputB(UVCOEF3 +  8);

	memcpy(&m_nDataOutBuf[ 6], &cUVCoefficient1[0], 9);
	memcpy(&m_nDataOutBuf[16], &cUVCoefficient2[0], 9);
	memcpy(&m_nDataOutBuf[26], &cUVCoefficient3[0], 9);

	return status;
}
//===========================================================================

// OUTPUT VIS COEFFICIENTS, 3 EACH, IN THE FORM:  +####E+##
unsigned int CParserThread::cmdRV()
{
	WORD	status(NO_ERRORS);
	char	cVISCoefficient1[9];
	char	cVISCoefficient2[9];
	char	cVISCoefficient3[9];

	strcpy(m_nDataOutBuf, "RV00VISxxxxxxxxx xxxxxxxxx xxxxxxxxx ");

	cVISCoefficient1[0] = m_NvRam.InputB(VISCOEF1     );
	cVISCoefficient1[1] = m_NvRam.InputB(VISCOEF1 +  1);
	cVISCoefficient1[2] = m_NvRam.InputB(VISCOEF1 +  2);
	cVISCoefficient1[3] = m_NvRam.InputB(VISCOEF1 +  3);
	cVISCoefficient1[4] = m_NvRam.InputB(VISCOEF1 +  4);
	cVISCoefficient1[5] = m_NvRam.InputB(VISCOEF1 +  5);
	cVISCoefficient1[6] = m_NvRam.InputB(VISCOEF1 +  6);
	cVISCoefficient1[7] = m_NvRam.InputB(VISCOEF1 +  7);
	cVISCoefficient1[8] = m_NvRam.InputB(VISCOEF1 +  8);

	cVISCoefficient2[0] = m_NvRam.InputB(VISCOEF2     );
	cVISCoefficient2[1] = m_NvRam.InputB(VISCOEF2 +  1);
	cVISCoefficient2[2] = m_NvRam.InputB(VISCOEF2 +  2);
	cVISCoefficient2[3] = m_NvRam.InputB(VISCOEF2 +  3);
	cVISCoefficient2[4] = m_NvRam.InputB(VISCOEF2 +  4);
	cVISCoefficient2[5] = m_NvRam.InputB(VISCOEF2 +  5);
	cVISCoefficient2[6] = m_NvRam.InputB(VISCOEF2 +  6);
	cVISCoefficient2[7] = m_NvRam.InputB(VISCOEF2 +  7);
	cVISCoefficient2[8] = m_NvRam.InputB(VISCOEF2 +  8);

	cVISCoefficient3[0] = m_NvRam.InputB(VISCOEF3     );
	cVISCoefficient3[1] = m_NvRam.InputB(VISCOEF3 +  1);
	cVISCoefficient3[2] = m_NvRam.InputB(VISCOEF3 +  2);
	cVISCoefficient3[3] = m_NvRam.InputB(VISCOEF3 +  3);
	cVISCoefficient3[4] = m_NvRam.InputB(VISCOEF3 +  4);
	cVISCoefficient3[5] = m_NvRam.InputB(VISCOEF3 +  5);
	cVISCoefficient3[6] = m_NvRam.InputB(VISCOEF3 +  6);
	cVISCoefficient3[7] = m_NvRam.InputB(VISCOEF3 +  7);
	cVISCoefficient3[8] = m_NvRam.InputB(VISCOEF3 +  8);

	memcpy(&m_nDataOutBuf[ 7], &cVISCoefficient1[0], 9);
	memcpy(&m_nDataOutBuf[17], &cVISCoefficient2[0], 9);
	memcpy(&m_nDataOutBuf[27], &cVISCoefficient3[0], 9);

	return status;
}
//===========================================================================

// READ UV COEFFICIENTS, 3 EACH, IN THE FORM:  +####E+##
unsigned int CParserThread::cmdSU()
{
	WORD	status(NO_ERRORS);
	
	strcpy(m_nDataOutBuf, "SU00");			// 4 character string

	if (*(m_pCmdPtr     ) != MINUS_SIGN && *(m_pCmdPtr     ) != PLUS_SIGN ||
		*(m_pCmdPtr +  5) != 'E' ||
		*(m_pCmdPtr +  6) != MINUS_SIGN && *(m_pCmdPtr +  6) != PLUS_SIGN)
	{
		status = ERR_PARA1;
		memcpy(&m_nDataOutBuf[2], "71", 2);
		return status;
	}

	if (*(m_pCmdPtr +  9) != MINUS_SIGN && *(m_pCmdPtr +  9) != PLUS_SIGN ||
		*(m_pCmdPtr + 14) != 'E' ||
		*(m_pCmdPtr + 15) != MINUS_SIGN && *(m_pCmdPtr + 15) != PLUS_SIGN)
	{
		status = ERR_PARA2; 
		memcpy(&m_nDataOutBuf[2], "72", 2);
		return status;
	}

	if (*(m_pCmdPtr + 18) != MINUS_SIGN && *(m_pCmdPtr + 18) != PLUS_SIGN || 
		*(m_pCmdPtr + 23) != 'E' ||
		*(m_pCmdPtr + 24) != MINUS_SIGN && *(m_pCmdPtr + 24) != PLUS_SIGN)
	{
		status = ERR_PARA3;
		memcpy(&m_nDataOutBuf[2], "73", 2);
		return status;
	}

	m_NvRam.OutputB(UVCOEF1		, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  5, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  6, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  7, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF1 +  8, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2		, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  5, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  6, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  7, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF2 +  8, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3		, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  5, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  6, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  7, *m_pCmdPtr++);
	m_NvRam.OutputB(UVCOEF3 +  8, *m_pCmdPtr++);

	m_nBytesRead += 27;

	return status;
}
//===========================================================================

// READ VIS COEFFICIENTS, 3 EACH, IN THE FORM:  +####E+##
unsigned int CParserThread::cmdSV()
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "SV00");			// 4 character string

	if (*(m_pCmdPtr     ) != MINUS_SIGN && *(m_pCmdPtr     ) != PLUS_SIGN ||
		*(m_pCmdPtr +  5) != 'E' ||
		*(m_pCmdPtr +  6) != MINUS_SIGN && *(m_pCmdPtr +  6) != PLUS_SIGN)
	{
		status = ERR_PARA1;
		memcpy(&m_nDataOutBuf[2], "71", 2);
		return status;
	}

	if (*(m_pCmdPtr +  9) != MINUS_SIGN && *(m_pCmdPtr +  9) != PLUS_SIGN || 
		*(m_pCmdPtr + 14) != 'E' ||
		*(m_pCmdPtr + 15) != MINUS_SIGN && *(m_pCmdPtr + 15) != PLUS_SIGN)
	{
		status = ERR_PARA2; 
		memcpy(&m_nDataOutBuf[2], "72", 2);
		return status;
	}

	if (*(m_pCmdPtr + 18) != MINUS_SIGN && *(m_pCmdPtr + 18) != PLUS_SIGN || 
		*(m_pCmdPtr + 23) != 'E' ||
		*(m_pCmdPtr + 24) != MINUS_SIGN && *(m_pCmdPtr + 24) != PLUS_SIGN)
	{
		status = ERR_PARA3;
		memcpy(&m_nDataOutBuf[2], "73", 2);
		return status;
	}

	m_NvRam.OutputB(VISCOEF1	 , *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  5, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  6, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  7, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF1 +  8, *m_pCmdPtr++);

	m_NvRam.OutputB(VISCOEF2	 , *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  5, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  6, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  7, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF2 +  8, *m_pCmdPtr++);

	m_NvRam.OutputB(VISCOEF3	 , *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  1, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  2, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  3, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  4, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  5, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  6, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  7, *m_pCmdPtr++);
	m_NvRam.OutputB(VISCOEF3 +  8, *m_pCmdPtr++);

	m_nBytesRead += 27;

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmddc.cpp $
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:14 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
