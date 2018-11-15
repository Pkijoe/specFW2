//===========================================================================
//
//	Module Name:	cmdES.cpp
//
//	Function:		This routine returns the environmental status data
//					of the instrument.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdES()
{
	WORD	status(NO_ERRORS);
	double	dval;
	long	lval;
	int		sval;

	theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters

	// 24vdc,UV TEC,VIS TEC,UV Temp,VIS Temp,PCB Temp,Tub Top temp, Tub Bottom temp, Argon,Nitrogen,WaterLeak,Tub Temp,Motor init status,Lid status,Fan status
	//                    "          1         2         3         4         5"        
	//                    "0123456789012345678901234567890123456789012345678901234567"
	strcpy(m_nDataOutBuf, "ES00+xxx+xxx+xxx+xxx+xxx+xxx+xxxx+xxxxFAxFNxWLx+xxxxMxLxFx");

	// All values below are placed into scaled integers (tenths of their values).

	lval	 = (long) m_nPS24vdc;
	lval    *= 240;
	lval    /= 130;
	sval	 = (unsigned int) lval;
	i2b(sval, 5);					// 24 Vdc Power Supply

	lval	 = (long) m_nUVcooler;
	lval     = (long) ((double) lval * 0.39412); // 0.039412 x 10 for scaled integer (v x 10)

	sval	 = (unsigned int) lval;
	i2b(sval, 9);					// UV Cooler Voltage
	
	lval	 = (long) m_nVIScooler;
	lval     = (long) ((double) lval * 0.39412); // 0.039412 x 10 for scaled integer (v x 10)

	sval	 = (unsigned int) lval;
	i2b(sval, 13);					// VIS Cooler Voltage

	sval	 = GetTECoolerTemperature(m_nTempccdUV) / 10;
	if (sval < 0)
	{
		m_nDataOutBuf[16] = '-';
		sval *= -1;
	}
	i2b(sval, 17);					// UV CCD Temperature

	sval	 = GetTECoolerTemperature(m_nTempccdVIS) / 10;
	if (sval < 0)
	{
		m_nDataOutBuf[20] = '-';
		sval *= -1;
	}
	i2b(sval, 21);					// VIS CCD Temperature

	// Electronics PCB temperature Temp = 0.0007684 x counts^2 - 0.50445 x counts + 77.136 
	lval	 = (long) m_nTempFan;
	dval = ((double)lval * (double)lval * 0.0007684) - (0.50445 * (double)lval) + 77.136;  
	lval = (long) (dval * 10.0);  // Scale by 10
	sval	= (unsigned int) lval;
	i2b(sval, 25);					// Electronics PCB Temperature
	
	lval = (long) channel2temp(TEMP_TOP_PRI);  // Tub Top Primary Temperature - 2 digit precision
	if (lval > 9999)
		lval = 9999;
	if (lval < 0)
	{
		m_nDataOutBuf[28] = '-';
		lval *= -1;
	}
	sval	 = (unsigned int) lval;
	c4c2b(sval, 29);						   // Tub Top Temperature

	lval = (long) channel2temp(TEMP_BOT_PRI);  // Tub Bottom Primary Temperature - 2 digit precision
	if (lval > 9999)
		lval = 9999;
	if (lval < 0)
	{
		m_nDataOutBuf[33] = '-';
		lval *= -1;
	}
	sval	 = (unsigned int) lval;
	c4c2b(sval, 34);					 	   // Tub Bottom Temperature
	
	
	if (m_IO.InputW(SVSTAT_CS) & ARGON_FLOW)
		m_nDataOutBuf[40] = '0';		// No
	else
		m_nDataOutBuf[40] = '1';		// Yes

	if (m_IO.InputW(SVSTAT_CS) & NITROGEN_FLOW)
		m_nDataOutBuf[43] = '0';		// No
	else
		m_nDataOutBuf[43] = '1';		// Yes

	if ((m_IO.InputW(SVSTAT_CS) & WATER_LEAK))
		m_nDataOutBuf[46] = '0';		// No
	else
		m_nDataOutBuf[46] = '1';		// Yes
	
	lval = (long) (Get_Tub_Temp() * 100.0);  // Tub  Temperature - 2 digit precision
	if (lval > 9999)
		lval = 9999;
	if (lval < 0)
	{
		m_nDataOutBuf[47] = '-';
		lval *= -1;
	}
	sval	 = (unsigned int) lval;
	c4c2b(sval, 48);					// Tub  Temperature
	
    if (m_Init_Motors_Status)			// Motors init status
		m_nDataOutBuf[53] = '1';		// Yes
	else
		m_nDataOutBuf[53] = '0';		// No

    if (m_IO.InputW(STATUS_REG2) & LID_STATUS)		// Lid status;  0 = ON (connected)
		m_nDataOutBuf[55] = '0';					// Off
	else
		m_nDataOutBuf[55] = '1';					// On

    if (m_IO.InputW(STATUS_REG2) & FAN_STATUS)		// Fan status;  0 = OK (pass)
		m_nDataOutBuf[57] = '0';					// Fail
	else
		m_nDataOutBuf[57 ] = '1';					// Pass

	theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection

	return status;
}
//===========================================================================

/*** Revision History ***


04/22/16    JO      Added 4-digit temps for fin and tub tacked on to end of string for
					increased precision.  Allows for backward compatibility with old DCM.
10/03/16	KR		CBF-56 Redefine thermal 
10/27/16    JO      CBF-65 - Code changes to support Syngistix release 3.0.


$Log: /IcarusBased/SpecFW/cmdes.cpp $
 * 
 * 21    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 20    10/06/05 4:34p Nashth
 * Removed bit inversion for Water_Leak test.
 * 
 * 19    10/06/05 1:35p Frazzitl
 * Changed Byte read to Word read
 * 
 * 18    10/06/05 1:32p Frazzitl
 * Fixed problem in QPR THNJ6GWPCL.  The problem was the incorrect reading
 * of the Water Leak status because the incorrect I/O function was being
 * used.
 * 
 * 17    10/05/05 12:48p Frazzitl
 * Updated Address Map and TE Cooler voltage calculation constant and
 * related parameters
 * 
 * 16    9/28/05 4:51p Nashth
 * Corrected the signal polarity of the Water Leak signal
 * 
 * 15    9/02/05 11:34a Nashth
 * Corrected scaling on Fan temp (now called electronics temp) to a factor
 * of 10 rather than 1000.
 * 
 * 14    8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 13    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 12    7/22/05 8:42 Frazzitl
 * Removed references to Die Temp - changed to ccd Temp
 * 
 * 11    6/29/05 13:56 Frazzitl
 * Fixed Ambient Temp display
 * 
 * 8     6/06/05 12:38p Frazzitl
 * Fixed problem with Water Leak signal - High indicates NO Leak
 * 
 * 7     5/13/05 9:05 Frazzitl
 * Added ability to read detector temperature
 * 
 * 6     4/22/05 15:18 Frazzitl
 * Improved Status reads
 * 
 * 5     4/21/05 8:59 Frazzitl
 * Fixed problem with negative number
 * 
 * 4     4/20/05 17:09 Frazzitl
 * Added the new equation values for 24 vdc and the TE coolers
 * 
 * 3     4/19/05 4:44p Frazzitl
 * Fixed reading A/D values and corrected Water Leak display
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:15 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
