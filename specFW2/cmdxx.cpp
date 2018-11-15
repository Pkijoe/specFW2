//===========================================================================
//
//	Module Name:	cmdXX.cpp
//
// 	Function:	  	This routine is used during diagnostics.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdXX()
{
	unsigned int	status(NO_ERRORS);
	WORD			word;

	char	chr0(0);
	char	chr1;
	char	chr2;

	if (m_nCmd == CMD_XA)
	{
		chr0 = 'A';
		strcpy(m_nDataOutBuf, "XA00");
	}
	else
	
	if (m_nCmd == CMD_XU)
	{
		chr0 = 'U';
		strcpy(m_nDataOutBuf, "XU00");
	}
	else if (m_nCmd == CMD_XV)
	{
		chr0 = 'V';
		strcpy(m_nDataOutBuf, "XV00");
	}

	chr1 = *m_pCmdPtr++;
	chr2 = *m_pCmdPtr++;
	m_nBytesRead += 2;

	switch (chr1)
	{
		case 'B':
			readBRAM();						// CHECK BATTERY RAM
			if (m_nBRAMerrors)				// ERRORS IN READING DATA FROM BRAM
			{
				status = ERR_FITNESS;
				memcpy(&m_nDataOutBuf[2], "18", 2);
			}
			break;
		case 'C':
			break;
		case 'E':
			break;
		case 'I':
			break;
		case 'L':							// FOR AUTOTESTER ONLY
			if (chr2 == '0')				// TURN OFF LED LAMP
			{
//				m_IO.OutputW(LED_LEVEL_REG, 0);
			}
			else if (chr2 == '1')			// TURN ON LED LAMP
			{
//				m_IO.OutputW(LED_LEVEL_REG, m_nLEDLevel);
//				Sleep(10);
			}
			break;
		case 'M':
			if (chr2 == '0')				// TURN OFF DETECTOR POWER
			{
				m_nPowerRegisterUV  &= ~POWER_ON;
				m_CameraIO.UVOutputW( POWER_SHUTDOWN_UV,  m_nPowerRegisterUV);
				if (m_bSingleChannel == false)
				{
					m_nPowerRegisterVIS &= ~POWER_ON;
					m_CameraIO.VISOutputW(POWER_SHUTDOWN_VIS, m_nPowerRegisterVIS);
				}
				m_bCCDPower = false;
			}
			else if (chr2 == '1')			// TURN ON DETECTOR POWER
			{
				m_nPowerRegisterUV  |= POWER_ON;
				m_nPowerRegisterUV  &= ~LOW_SPEED;
				m_CameraIO.UVOutputW( POWER_SHUTDOWN_UV,  m_nPowerRegisterUV);
				if (m_bSingleChannel == false)
				{
					m_nPowerRegisterVIS |= POWER_ON;
					m_nPowerRegisterVIS &= ~LOW_SPEED;
					m_CameraIO.VISOutputW(POWER_SHUTDOWN_VIS, m_nPowerRegisterVIS);
				}
				m_bCCDPower = true;
				m_CameraIO.UVOutputW( TIME_MODE_UV,  CAMERA_READY);
				if (m_bSingleChannel == false)
				{
					m_CameraIO.VISOutputW(TIME_MODE_VIS, CAMERA_READY);
				}
			}
			break;
		case 'R':
			break;
		case 'S':
			status = cmdCS();				// CONFIGURE SLIT
			break;
		case 'T':
			if (m_cOperationMode == AUTO)
			{
				if (chr2 == '0')
				{
					status	 = ERR_CMD;
					memcpy(&m_nDataOutBuf[2], "01", 2);
				}
				else if (chr2 == '1')
				{
					m_nOPmode	= TEST;		// SET TEST MODE
					m_nTestMode = NORMAL;	// NORMAL SUB-ARRAYS & PIXELS
					m_bTestMode	= true;
					m_bOverscanMode	= false;
				}
				else if (chr2 == '2')
				{
					m_nOPmode	= TEST;		// SET TEST MODE
					m_nTestMode = OVERSCAN;	// ALL SUB-ARRAYS & 48 PIXELS
					m_bTestMode	= true;
					m_bOverscanMode	= true;
				}
			}
			else
			{
				if (chr2 == '0')
				{
					m_nOPmode	= NORMAL;	// SET NORMAL MODE
					m_nTestMode = NORMAL;	// NORMAL SUB-ARRAYS & PIXELS
					m_bTestMode	= false;
					m_bOverscanMode	= false;
				}
				else if (chr2 == '1')
				{
					m_nOPmode	= TEST;		// SET TEST MODE
					m_nTestMode = NORMAL;	// NORMAL SUB-ARRAYS & PIXELS
					m_bTestMode	= true;
					m_bOverscanMode	= false;
				}
				else if (chr2 == '2')
				{
					m_nOPmode	= TEST;		// SET TEST MODE
					m_nTestMode = OVERSCAN;	// ALL SUB-ARRAYS & 48 PIXELS
					m_bTestMode	= true;
					m_bOverscanMode	= true;
				}
			}
			break;
		case 'V':
			if (chr2 == 'X')
				status = cmdCX();			// CONFIGURE VIEW X
			else if (chr2 == 'Y')
				status = cmdCY();			// CONFIGURE VIEW Y
			break;
		case 'Z':
			word = m_nDigitalControl;
			if (chr0 == 'A')
			{
				// SELECT THERMO COOLER FOR BOTH UV & VIS
				word &= ~(TEC_EN_VIS | TEC_EN_UV);
				if (chr2 == '0')
					;
				else if (chr2 == '1')
					word |= TEC_EN_UV;
				else if (chr2 == '2')
					word |= TEC_EN_VIS;
				else if (chr2 == '3')
					word |= (TEC_EN_VIS | TEC_EN_UV);
			}
			else if (chr0 == 'V')
			{
				// SELECT THERMO COOLER FOR VIS
				if (chr2 == '0')
					word &= ~TEC_EN_VIS;
				else if (chr2 == '1')
					word |= TEC_EN_VIS;
			}
			else if (chr0 == 'U')
			{
				// SELECT THERMO COOLER FOR UV
				if (chr2 == '0')
					word &= ~TEC_EN_UV;
				else if (chr2 == '1')
					word |= TEC_EN_UV;
			}
			m_nDigitalControl = word;
			m_IO.OutputW(DIGITAL_CMD_REG, m_nDigitalControl);
			break;
		default:
			status = ERR_PARA;
			memcpy(&m_nDataOutBuf[2], "07", 2);
			break;
	}

	return status;
}
//===========================================================================

/*** Revision History ***

12/18/15    JO      Initial additions and changes for Century B.
01/18/16    JO      Removed XA command since it was disabled elsewhere and because
                    the a new command need it. 
03/21/16    JO      Restored XA command code to diagostics.  

$Log: /IcarusBased/SpecFW/cmdxx.cpp $
 * 
 * 11    7/05/05 13:14 Frazzitl
 * Fixed problem with turning on power
 * 
 * 10    6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 9     6/17/05 10:09a Frazzitl
 * Added feature to handle Single Channel instruments.
 * 
 * 8     6/07/05 8:25a Frazzitl
 * Separated camera I/O into UV and VIS routines to accommodate individual
 * status checking
 * 
 * 7     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 6     5/25/05 8:44a Frazzitl
 * Initialized registers and enabled "high speed " operation on power up
 * 
 * 5     5/16/05 16:02 Frazzitl
 * Stopped light led inside tub indicating power on
 * 
 * 4     5/13/05 14:46 Frazzitl
 * Fixed problem reading commands
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
