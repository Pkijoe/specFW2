//===========================================================================
//
//	Module Name:	cmdCD.cpp
//
//	Function:		This routine returns the converter status data
//					of the instrument.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"


unsigned int CParserThread::cmdCD()
{
	WORD	status(NO_ERRORS);
	long	lval;
	int		sval;
	double	dval;
	double	dval2;

	// 24vdc,Fan Temp,Top Primary Temp,Top Secondary Temp,Bottom Primary Temp,Bottom Secondary Temp,Ambient Temp,UV TEC,VIS TEC,Ground
    //                    "01234567890123456789012345678901234567890123"
	strcpy(m_nDataOutBuf, "CD00+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx+xxx");

	lval	 = (long) m_nPS24vdc;
	lval    *= 240;
	lval    /= 130;
	sval	 = (unsigned int) lval;
	i2b(sval, 5);					// 24 Vdc Power Supply

	// Ambient - Inside = 0.0007684 * x^2 - 0.50445 * x + 77.136
	dval	 = (double) m_nTempFan;
	dval2	 = dval * dval;
	dval2	*= 0.0007684;
	dval2	-= (dval * 0.50445);
	dval2	+= 77.136;
	dval2	*= 10;
	sval	= (unsigned int) dval2;
	i2b(sval, 9);					// Fan Temperature

	lval	 = (long) channel2temp(TEMP_TOP_PRI) / 10;
	if (lval > 999)
		lval = 999;
	if (lval < 0)
		lval = 0;
	sval	 = (unsigned int) lval;
	i2b(sval, 13);					// Tub Top Primary Temperature (Previously Top Temperature)
	
	lval	 = (long) channel2temp(TEMP_BOT_PRI) / 10;
	if (lval > 999)
		lval = 999;
	if (lval < 0)
		lval = 0;
	sval	 = (unsigned int) lval;
	i2b(sval, 17);					// Tub Bottom PrimaryTemperature (Previously Bottom Temperature)
	
	lval	 = (long) channel2temp(TEMP_TOP_SEC) / 10;
	if (lval > 999)
		lval = 999;
	if (lval < 0)
		lval = 0;
	sval	 = (unsigned int) lval;
	i2b(sval, 21);					// Tub Top Secondary  Temperature (Previously Fin Temperature)
	
	lval	 = (long) channel2temp(TEMP_BOT_SEC) / 10;
	if (lval > 999)
		lval = 999;
	if (lval < 0)
		lval = 0;
	sval	 = (unsigned int) lval;
	i2b(sval, 25);					// Tub Bottom Secondary Temperature (Previously Other Temperature)
	
	// Ambient - Outside = 0.0007692 * x^2 - 0.50469 * x + 77.158
	dval	 = (double) m_nTempAmbient;
	dval2	 = dval * dval;
	dval2	*= 0.0007684;
	dval2	-= (dval * 0.50445);
	dval2	+= 77.136;
	dval2	*= 10;
	sval	= (unsigned int) dval2;
	i2b(sval, 29);					// Ambient Temperature

	lval	 = (long) m_nUVcooler;
	lval    *= 50;
	lval    /= 186;
	sval	 = (unsigned int) lval;
	i2b(sval, 33);					// UV Cooler Voltage
	
	lval	 = (long) m_nVIScooler;
	lval    *= 50;
	lval    /= 186;
	sval	 = (unsigned int) lval;
	i2b(sval, 37);					// VIS Cooler Voltage

	lval	 = m_nGround;
	sval	 = (unsigned int) lval;
	i2b(sval, 41);					// Ground

	return status;
}

//===========================================================================

/*** Revision History ***


10/03/16	KR		CBF-56 Redefine thermal 
01/30/17	KR		CBF-143 Correct readout assignment


$Log: /IcarusBased/SpecFW/cmdcd.cpp $
 * 
 * 1     11/04/05 11:34a Nashth
 * Added for correct operation of CC command.
 * 
 * 9     10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 8     10/31/05 10:43a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 7     9/26/05 11:25a Nashth
 * Made bool consistent
 * 
 * 6     9/16/05 1:26p Frazzitl
 * Fixing TT command
 * 
 * 5     6/17/05 2:52p Frazzitl
 * Corrected the temperature equation constants and the status comparisons
 * 
 * 4     6/17/05 10:10a Frazzitl
 * Added equation for handling ambient temperatures.
 * 
 * 3     4/29/05 12:49 Frazzitl
 * Trying to get the threat to work properly
 * 
 * 2     4/27/05 13:28 Frazzitl
 * Fixed wrong command id
 * 
 * 1     4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature

$NoKeywords: $

** End of Rev History **/
