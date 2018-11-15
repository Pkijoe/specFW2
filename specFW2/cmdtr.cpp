//===========================================================================
//
//	Module Name:	cmdTR.cpp
//
// 	Function:	  	The routine passes the TEMPERATURE SENSOR coefficients
//		 			stored in BRAM.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/cmdtr.cpp 1     3/17/05 11:20 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdTR()
{
	WORD	status(NO_ERRORS);
	WORD	sensor;
	char	msign;
	int		mval;
	char	bsign;
	int		bval;

	sensor	= *m_pCmdPtr++;		// get Sensor Number
	m_nBytesRead++;

	switch (sensor)
	{
		case TEMP_UV_CCD:
			msign	= MINUS_SIGN;
			mval	= 227;		// FIXED SLOPE VALUE = -2.27
			bsign	= PLUS_SIGN;
			bval	= 534;		// FIXED OFFSET VALUE = 53.4
			break;

		case TEMP_VIS_CCD:
			msign	= MINUS_SIGN;
			mval	= 227;		// FIXED SLOPE VALUE = -2.27
			bsign	= PLUS_SIGN;
			bval	= 534;		// FIXED OFFSET VALUE = 53.4
			break;
		
		case TEMP_FAN:
			msign	= PLUS_SIGN;
			mval	= 512;		// FIXED SLOPE VALUE = 5.12
			bsign	= PLUS_SIGN;
			bval	= 16;		// FIXED OFFSET VALUE = 1.6
			break;
			
		case TEMP_PLENUM:
			msign	= PLUS_SIGN;
			mval	= 512;		// FIXED SLOPE VALUE = 5.12
			bsign	= PLUS_SIGN;
			bval	= 16;		// FIXED OFFSET VALUE = 1.6
			break;
			
		case TEMP_CASTING:
			msign	= PLUS_SIGN;
			mval	= 512;		// FIXED SLOPE VALUE = 5.12
			bsign	= PLUS_SIGN;
			bval	= 16;		// FIXED OFFSET VALUE = 1.6
			break;
			
		default:
			status	= ERR_PARA;
			strcpy(m_nDataOutBuf, "TR07");
			return status;
	}

	strcpy(m_nDataOutBuf, "TR00xxxxxxxx");	// 8 character string

	m_nDataOutBuf[4] = msign;
	i2b(mval, 5);

	m_nDataOutBuf[8] = bsign;
	i2b(bval, 9);

	return status;
}
//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/cmdtr.cpp $
 * 
 * 1     3/17/05 11:20 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
