//===========================================================================
//
//	Module Name:	MiscellaneousConstants.cpp
//
// 	Function:		This routine sets all the miscellaneous constants used
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/MiscellaneousConstants.cpp 3     5/23/05 11:50a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

#define   USEVERFROM  2
#include "product_ver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CParserThread::ReadMiscellaneousConstants()
{
	// Hardware Versions
	m_cRevMPC = 3;
	m_cRevCDS = 3;

	// Serial NO: 1234567890A
	m_cSerialNo[ 0]	= '1';	m_cSerialNo[ 1]	= '2';	m_cSerialNo[ 2]	= '3';
	m_cSerialNo[ 3]	= '4';	m_cSerialNo[ 4]	= '5';	m_cSerialNo[ 5]	= '6';
	m_cSerialNo[ 6]	= '7';	m_cSerialNo[ 7]	= '8';	m_cSerialNo[ 8]	= '9';
	m_cSerialNo[ 9]	= '0';	m_cSerialNo[10]	= 'A';

	// UV Coefficients: +7300E-10 +7300E-10 +7300E-10 
	m_cUVCoefficient1[0] = '+';	m_cUVCoefficient1[1] = '7';	m_cUVCoefficient1[2] = '3';
	m_cUVCoefficient1[3] = '0';	m_cUVCoefficient1[4] = '0';	m_cUVCoefficient1[5] = 'E';
	m_cUVCoefficient1[6] = '-';	m_cUVCoefficient1[7] = '1';	m_cUVCoefficient1[8] = '0';
	m_cUVCoefficient2[0] = '+';	m_cUVCoefficient2[1] = '7';	m_cUVCoefficient2[2] = '3';
	m_cUVCoefficient2[3] = '0';	m_cUVCoefficient2[4] = '0';	m_cUVCoefficient2[5] = 'E';
	m_cUVCoefficient2[6] = '-';	m_cUVCoefficient2[7] = '1';	m_cUVCoefficient2[8] = '0';
	m_cUVCoefficient3[0] = '+';	m_cUVCoefficient3[1] = '7';	m_cUVCoefficient3[2] = '3';
	m_cUVCoefficient3[3] = '0';	m_cUVCoefficient3[4] = '0';	m_cUVCoefficient3[5] = 'E';
	m_cUVCoefficient3[6] = '-';	m_cUVCoefficient3[7] = '1';	m_cUVCoefficient3[8] = '0';

	// VIS Coefficients: +7300E-10 +7300E-10 +7300E-10 
	m_cVISCoefficient1[0] = '+';	m_cVISCoefficient1[1] = '7';	m_cVISCoefficient1[2] = '3';
	m_cVISCoefficient1[3] = '0';	m_cVISCoefficient1[4] = '0';	m_cVISCoefficient1[5] = 'E';
	m_cVISCoefficient1[6] = '-';	m_cVISCoefficient1[7] = '1';	m_cVISCoefficient1[8] = '0';
	m_cVISCoefficient2[0] = '+';	m_cVISCoefficient2[1] = '7';	m_cVISCoefficient2[2] = '3';
	m_cVISCoefficient2[3] = '0';	m_cVISCoefficient2[4] = '0';	m_cVISCoefficient2[5] = 'E';
	m_cVISCoefficient2[6] = '-';	m_cVISCoefficient2[7] = '1';	m_cVISCoefficient2[8] = '0';
	m_cVISCoefficient3[0] = '+';	m_cVISCoefficient3[1] = '7';	m_cVISCoefficient3[2] = '3';
	m_cVISCoefficient3[3] = '0';	m_cVISCoefficient3[4] = '0';	m_cVISCoefficient3[5] = 'E';
	m_cVISCoefficient3[6] = '-';	m_cVISCoefficient3[7] = '1';	m_cVISCoefficient3[8] = '0';

	// UV Detector Serial No: 123456
	m_cUVSerialNo[0] = '1';
	m_cUVSerialNo[1] = '2';
	m_cUVSerialNo[2] = '3';
	m_cUVSerialNo[3] = '4';
	m_cUVSerialNo[4] = '5';
	m_cUVSerialNo[5] = '6';

	// VIS Detector Serial No: 123456
	m_cVISSerialNo[0] = '1';
	m_cVISSerialNo[1] = '2';
	m_cVISSerialNo[2] = '3';
	m_cVISSerialNo[3] = '4';
	m_cVISSerialNo[4] = '5';
	m_cVISSerialNo[5] = '6';

	// Version Information
	m_cSerial[0]	= 'Z';
	m_cSerial[1]	= 'O';
	m_cSerial[2]	= 'D';
	m_cSerial[3]	= 'I';
	m_cSerial[4]	= 'A';
	m_cSerial[5]	= 'C';

	// Version Information
	CString	sVersion(PROD_sPRODUCTVERSION);
	int		nLen = sVersion.GetLength();

	for (int n = 0; n < nLen; n++)
		m_cVersionNo[n]	= (char) sVersion.GetAt(n);

	if (nLen < 15)
	{
		for (int n = nLen; n < 15; n++)
			m_cVersionNo[n]	= ' ';
	}

	// Verison Month
	m_cMonth[0]		= '0';
	m_cMonth[1]		= '5';

	// Version Day
	m_cDay[0]		= '2';
	m_cDay[1]		= '3';

	// Version Year
	m_cYear[0]		= '0';
	m_cYear[0]		= '5';

}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

01/28/17	KR	  CBF-143 - Remove thermal commands

$Log: /WinLab/SpecFW/MiscellaneousConstants.cpp $
 * 
 * 3     5/23/05 11:50a Frazzitl
 * Changed version number from 4 to 15 characters to support standard
 * version format = XX.xx.yyy.bbbb
 * 
 * 2     5/06/05 15:53 Frazzitl
 * New implementation of threads
 * 
 * 1     4/29/05 8:37 Frazzitl
 * Separated out from CParserThread initialization to simplify readability

$NoKeywords: $

** End of Rev History **/
