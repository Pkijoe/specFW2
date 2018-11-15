//===========================================================================
//
//	Module Name:	Parser.cpp
//
//	Function:		This routine determines if the COMMAND STRING
//					contains any formatting errors.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::Parser()
{
	WORD	wStatus(NO_ERRORS);
	WORD	wCnt;

	while (*m_pCmdPtr != (char) END && wStatus == NO_ERRORS)
	{
		m_nCmd   = *m_pCmdPtr++;		// get one command
		m_nCmd <<= 8;
		m_nCmd  |= *m_pCmdPtr++;
		m_nBytesRead += 2;

		switch (m_nCmd)
		{
// New commands added for Century B	start
			case CMD_HA:
			case CMD_HB:
			case CMD_QB:			
			case CMD_NA:
			case CMD_NB:
			case CMD_UB:
			case CMD_UC:
			case CMD_XB:
			case CMD_XC:
			case CMD_KA:
			case CMD_KB:
			case CMD_KC:
			case CMD_PA:
			case CMD_PR:
			case CMD_PK:
			case CMD_IM:
			case CMD_UA:			// Get Active Upper Sensor 
			case CMD_LA:			// Get Active Lower Sensor 
			case CMD_JI:			// Just-Initialize (Restart configured hardware)

	
// End new commands added for Century B	end		

			case CMD_BS:
			case CMD_BX:
			case CMD_BY:
			case CMD_CM:
			case CMD_CP:
			case CMD_CS:
			case CMD_CX:
			case CMD_CY:
			case CMD_D0:
			case CMD_FS:
			case CMD_FX:
			case CMD_FY:
			case CMD_HG:
			case CMD_HK:
			case CMD_HS:
			case CMD_HX:
			case CMD_HY:
			case CMD_I1:
			case CMD_I2:
			case CMD_I3:
			case CMD_I4:
			case CMD_IS:
			case CMD_LS:
			case CMD_LX:
			case CMD_LY:
			case CMD_NX:
			case CMD_NY:
			case CMD_RB:
			case CMD_RO:
			case CMD_SK:
			case CMD_SR:
			case CMD_VS:
			case CMD_VX:
			case CMD_VY:
			case CMD_TC:
				// no parameters just pneumonics
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_C0:
			case CMD_C1:
			case CMD_C2:
			case CMD_CH:
			case CMD_CT:
			case CMD_DF:
			case CMD_DM:
			case CMD_HU:
			case CMD_HV:
			case CMD_LP:
			case CMD_PI:
			case CMD_RM:
			case CMD_RT:
			case CMD_SC:
			case CMD_SE:
			case CMD_SG:
			case CMD_SH:
			case CMD_SY:
			case CMD_UM:
			case CMD_VT:
			case CMD_WE:
				m_pCmdPtr++;		// one byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_AU:			// Set Active Upper Sensor
			case CMD_AL:			// Set Active Lower Sensor
			case CMD_CA:			// Calibrate Sensors
			case CMD_H1:
			case CMD_PS:
			case CMD_XA:
			case CMD_XU:
			case CMD_XV:
				m_pCmdPtr += 2;		// two byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
// New commands added for Century B	start
			case CMD_MB:			// three byte parameter
			case CMD_BQ:
// New commands added for Century B	end			
			case CMD_F1:
			case CMD_PX:
			case CMD_AR:
			case CMD_PY:
				m_pCmdPtr += 3;		// three byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_D1:
			case CMD_D2:
				m_pCmdPtr += 4;		// four byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_LL:
			case CMD_OH:
			case CMD_RD:
			case CMD_SD:
			case CMD_SL:
				m_pCmdPtr += 5;		// five byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_DA:
			case CMD_DU:
			case CMD_DV:
			case CMD_IT:
			case CMD_MS:
			case CMD_MX:
			case CMD_MY:
				m_pCmdPtr += 6;		// six byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_WB:
				m_pCmdPtr += 7;		// seven byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_VC:
				m_pCmdPtr += 8;		// eight byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_MW:
			case CMD_SI:
				m_pCmdPtr += 11;	// eleven byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_SU:
			case CMD_SV:
				m_pCmdPtr += 27;	// Twenty seven byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_FC:
				m_pCmdPtr += 19;
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_TM:
				m_pCmdPtr += 21;  // should be 21 -- was 20 when we didn't do triggered reads {"TM000000001000015002001"}
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_LN:
				wCnt = b2c();
				if (wCnt == 0)
					wStatus = ERR_PARA;
				else
				{
					while (wCnt--)
						m_pCmdPtr += 3;
					if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
						wStatus = ERR_FORMAT;
				}
				break;

		// THESE COMMANDS MUST BE THE LAST COMMAND IN THE STRING
			
			case CMD_CB:
			case CMD_CC:
			case CMD_CD:
			case CMD_0C:
			case CMD_1D:
			case CMD_2D:
			case CMD_0H:
			case CMD_1H:
			case CMD_3T:
			case CMD_1V:
			case CMD_2V:
			case CMD_3V:
			case CMD_4V:
			case CMD_5V:
			case CMD_DE:
			case CMD_EC:		//  3 bytes
			case CMD_RA:		//  3 bytes
			case CMD_ES:		// 27 bytes
			case CMD_GC:		// 19 bytes
			case CMD_GD:
			case CMD_GE:
			case CMD_GG:
			case CMD_GL:
			case CMD_GM:
			case CMD_GS:
			case CMD_GT:		//  5 bytes
			case CMD_GV:
			case CMD_HO:
			case CMD_ID:
			case CMD_RC:
			case CMD_RI:
			case CMD_SA:		//  8 bytes
			case CMD_ST:		// 46 bytes
			case CMD_TA:
			case CMD_TT:
			case CMD_VV:
			case CMD_RR:
			case CMD_RU:
			case CMD_RV:
			case CMD_SN:
			case CMD_SZ:
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_MD:
			case CMD_TR:		//  8 bytes
				m_pCmdPtr++;
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_GB:		// 4 bytes
				m_pCmdPtr += 4;
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_SB:
			case CMD_TB:
				m_pCmdPtr += 5;
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_MC:		// 6 bytes
				m_pCmdPtr += 6;
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_SM:
				m_pCmdPtr += 14;	// fourteen byte parameter
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_CR:
				m_pCmdPtr += 31;
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			default:				// UNKNOWN COMMAND
			{
				short	nTemp   = (short) (0xff00 & m_nCmd);
						nTemp >>= 8;
				char	cChar1  = (char) nTemp;
				char	cChar2  = (char) (0x00ff & m_nCmd);
				printf("Parser ERR_CMD 0x%x (%c%c)\r\n", m_nCmd, cChar1, cChar2);
				wStatus = ERR_CMD;

				theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
				m_nDataOutBuf[0] = cChar1;
				m_nDataOutBuf[1] = cChar2;
				m_nDataOutBuf[2] = '0';
				m_nDataOutBuf[3] = '1';
				m_nDataOutBuf[4] = NULL;
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				break;
			}
		}

		if (*m_pCmdPtr == (char) SPACE)
		{
			while (*m_pCmdPtr == (char) SPACE)
				m_pCmdPtr++;
		}
	}

	if (m_bAbortFlag)
	{
		m_nDataBlock = 0;
		m_bAbortFlag = false;
		wStatus = ERR_ABORT_CMD;
	}

	return wStatus;
}
//===========================================================================

// USED DURING COLD & HOT STARTS ONLY

unsigned int CParserThread::ParserInit()
{
	WORD	wStatus(NO_ERRORS);

	while (*m_pCmdPtr != (char) END && wStatus == NO_ERRORS)
	{
		m_nCmd   = *m_pCmdPtr++;		// get one command
		m_nCmd <<= 8;
		m_nCmd  |= *m_pCmdPtr++;
		m_nBytesRead += 2;

		switch (m_nCmd)
		{
			case CMD_UA:			// Get Active Upper Sensor 
			case CMD_LA:			// Get Active Lower Sensor 
			case CMD_AU:			// Set Active Upper Sensor
			case CMD_AL:			// Set Active Lower Sensor
			case CMD_CA:			// Calibrate Sensors
			case CMD_CH:
			case CMD_UM:
			case CMD_WE:
				m_pCmdPtr++;		// one byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_H1:
				m_pCmdPtr += 2;	//  2 byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_AR:
			case CMD_F1:
				m_pCmdPtr += 3;	//  3 byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_D1:
				m_pCmdPtr += 4;	//  4 byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;


		// THESE COMMANDS MUST BE THE LAST COMMAND IN THE STRING

			case CMD_0C:
			case CMD_1D:		//	0 bytes
			case CMD_2D:
			case CMD_0H:
			case CMD_3T:
			case CMD_1V:
			case CMD_2V:
			case CMD_3V:
			case CMD_4V:
			case CMD_5V:
			case CMD_DE:
			case CMD_EC:		//  3 bytes
			case CMD_RA:		//  3 bytes
			case CMD_ES:		// 27 bytes
			case CMD_GC:		// 19 bytes
			case CMD_GD:
			case CMD_GE:
			case CMD_GS:
			case CMD_GT:		//  5 bytes
			case CMD_GV:
			case CMD_HO:
			case CMD_ID:
			case CMD_RI:
			case CMD_JI:		// Just-Initialize (Restart configured hardware)			
			case CMD_ST:		// 46 bytes
			case CMD_SZ:
			case CMD_VV:
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_MD:
				if ( m_cStartupMode == NOT_ON )
				{
					m_pCmdPtr++;
					if (*m_pCmdPtr == (char) SPACE)
						m_pCmdPtr++;
					if (*m_pCmdPtr != (char) END)
						wStatus = ERR_END;
					break;
				}
			default:				// UNKNOWN COMMAND
			{
				short	nTemp   = (short) (0xff00 & m_nCmd);
						nTemp >>= 8;
				char	cChar1  = (char) nTemp;
				char	cChar2  = (char) (0x00ff & m_nCmd);
				printf("ParserInit ERR_CMD 0x%x (%c%c)\r\n", m_nCmd, cChar1, cChar2);
				wStatus = ERR_CMD;

				theApp.EnterCriticalSection1(&m_CriticalSection); // Protect critical parameters
				m_nDataOutBuf[0] = cChar1;
				m_nDataOutBuf[1] = cChar2;
				m_nDataOutBuf[2] = '0';
				m_nDataOutBuf[3] = '1';
				m_nDataOutBuf[4] = NULL;
				theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
				break;
			}
		}

		if (*m_pCmdPtr == (char) SPACE)
		{
			while (*m_pCmdPtr == (char) SPACE)
				m_pCmdPtr++;
		}
	}

	if (m_bAbortFlag)
	{
		m_nDataBlock = 0;
		m_bAbortFlag = false;
		wStatus = ERR_ABORT_CMD;
	}
	return(wStatus);
}
//===========================================================================

unsigned int CParserThread::ParserFatal()
{
	WORD	wStatus(NO_ERRORS);

	while (*m_pCmdPtr != (char) END && wStatus == NO_ERRORS)
	{
		m_nCmd   = *m_pCmdPtr++;		// get one command
		m_nCmd <<= 8;
		m_nCmd  |= *m_pCmdPtr++;
		m_nBytesRead += 2;

		switch (m_nCmd)
		{
			case CMD_CH:
			case CMD_UM:
			case CMD_WE:
				m_pCmdPtr++;		// one byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_AR:
				m_pCmdPtr += 3;		//  3 byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_D1:
				m_pCmdPtr += 4;		//  4 byte parameter
				if (*m_pCmdPtr != (char) END && *m_pCmdPtr != (char) SPACE)
					wStatus = ERR_FORMAT;
				break;
			case CMD_1D:
			case CMD_1V:
			case CMD_2V:
			case CMD_3V:
			case CMD_4V:
			case CMD_5V:
			case CMD_EC:
			case CMD_RA:		//  3 bytes
			case CMD_DE:
			case CMD_ES:
			case CMD_GE:
			case CMD_ID:
			case CMD_RI:
			case CMD_JI:			
			case CMD_ST:
			case CMD_SZ:
				if (*m_pCmdPtr == (char) SPACE)
					m_pCmdPtr++;
				if (*m_pCmdPtr != (char) END)
					wStatus = ERR_END;
				break;
			case CMD_GT:
				if (m_bSleepFlag)
				{
					BYTE	nYear,  nYearW;
					BYTE	nMonth, nMonthW;
					BYTE	nDate,  nDateW;
					BYTE	nHour,  nHourW;
					BYTE	nMin,   nMinW;
					BYTE	nSec;

					GetTimeValues(nYear, nMonth, nDate, nHour, nMin, nSec); 
					GetWakeupTimeValues(nYearW, nMonthW, nDateW, nHourW, nMinW);
					EnterCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
					// Must be same day before we compare times
					if (nYearW == nYear && nMonthW == nMonth && nDateW == nDate)
					{
						m_lKeyTime = (nHourW - nHour) * 3600 + (nMinW - nMin) * 60;
						if (m_lKeyTime < 0 || m_lKeyTime > 86400)
							m_lKeyTime = 86400;
					}
					else
						m_lKeyTime = 86400;

					if (*m_pCmdPtr == (char) SPACE)
						m_pCmdPtr++;
					if (*m_pCmdPtr != (char) END)
						wStatus = ERR_END;
					LeaveCriticalSection(&m_CriticalSectionClock); // Release clock protection
				}
				else
				{
					wStatus	= ERR_FATAL_MODE;
				}
				break;
			default:				// UNKNOWN COMMAND
			{
				short	nTemp   = (short) (0xff00 & m_nCmd);
						nTemp >>= 8;
				char	cChar1  = (char) nTemp;
				char	cChar2  = (char) (0x00ff & m_nCmd);
				printf("ParserFatal ERR_CMD 0x%x (%c%c)\r\n", m_nCmd, cChar1, cChar2);
				wStatus = ERR_FATAL_MODE;
				theApp.EnterCriticalSection1(&m_CriticalSection);	// Protect critical parameters
				m_nDataOutBuf[0] = cChar1;
				m_nDataOutBuf[1] = cChar2;
				m_nDataOutBuf[2] = 'F';
				m_nDataOutBuf[3] = 'M';
				m_nDataOutBuf[4] = NULL;
				theApp.LeaveCriticalSection1(&m_CriticalSection);	// Release protection
				break;
			}
		}

		if (*m_pCmdPtr == (char) SPACE)
		{
			while (*m_pCmdPtr == (char) SPACE)
				m_pCmdPtr++;
		}
	}

	if (m_bAbortFlag)
	{
		m_nDataBlock = 0;
		m_bAbortFlag = false;
		wStatus = ERR_ABORT_CMD;
	}
	return(wStatus);
}
//===========================================================================

/*** Revision History ***

03/21/16    JO      Restored XA command code to diagostics.  Changed "Set Toroid key radial position"
					 command code to XD.
07/11/16    JO      CBF-18 - Add and implement Park motors command "PK".
08/02/16    JO      CBF-26 Removed unused toroid commands and references.
10/27/16    JO      CBF-65 - Code changes to support Syngistix release 3.0.	
12/15/16    JO    	CBF-121 - Lid warmup ramp via T4 command and manual tub heater integrator power preset.
01/09/17    KR      CBF-126 - Create get/set commands for active sensors
01/18/17	KR/JO   CBF-133	Add cmdJI() functionality.
01/28/17	KR		CBF-143 - Remove thermal commands
01/31/17	KR		CBF-145 - Remove unused commands and errors
02/02/17    JO      CBF-147 - Add get/set BRAM commands for maximum shutter position.
02/03/17	KR		CBF-151 - Additional fixes to run the commands properly & remove ES warning

$Log: /IcarusBased/SpecFW/Parser.cpp $
 * 
 * 31    3/31/08 5:31p Nashth
 * Implementation of spectrometer history log... First pass.
 * 
 * 30    7/06/07 4:51p Nashth
 * Added an information error string when a detector read error occurs as
 * per QPR: THNJ74CQCW.
 * 
 * 29    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 28    1/23/07 11:11a Nashth
 * Added two functions AR<000> sets Auto Reset Time and
 * RA that retreives Auto Reset Time. Units = tenths of hours.
 * 
 * 27    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 26    11/28/05 1:03p Frazzitl
 * Changed C1 & C2 commands to control the External relays
 * 
 * 25    10/27/05 11:54a Frazzitl
 * Fixed m_lKeyTime used for the GT command
 * 
 * 24    10/17/05 9:37a Frazzitl
 * Added functions needed to read Detector FPGA version number.
 * 
 * 23    10/12/05 2:01p Frazzitl
 * Added ability to execute the WE command during initialization and fatal
 * mode.
 * 
 * 22    10/12/05 12:51p Frazzitl
 * Added ability to turn Watchdog timer On or Off - for testing only.
 * 
 * 21    9/26/05 3:14p Nashth
 * Added the new NGO3 commands 1v, 2v, 3v, and 4v to the acceptable list
 * for Fatal mode as per QPR: CANU6GGKEM.  
 * 
 * 20    9/26/05 11:35a Nashth
 * Fixed problem with Abort flag
 * 
 * 19    9/16/05 1:26p Frazzitl
 * Fixing TT command
 * 
 * 18    9/09/05 4:21p Nashth
 * Fixed entering fatal mode after power on problem as per QPR:
 * THNJ6G3RHV.
 * 
 * 17    9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 16    8/10/05 11:42 Frazzitl
 * Added new commands to read and write BRAM bytes.
 * 
 * 15    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 14    7/08/05 9:57 Frazzitl
 * Fixed problem with system clock updating.
 * 
 * 13    7/06/05 16:35 Frazzitl
 * Corrected minor problems
 * 
 * 12    7/05/05 10:30 Frazzitl
 * Fixed error message output
 * 
 * 11    6/29/05 14:05 Frazzitl
 * Fixed minor problem
 * 
 * 9     6/06/05 8:28a Frazzitl
 * Added new functions to return CPLD and Personality Board version
 * numbers.
 * 
 * 8     5/26/05 8:26 Frazzitl
 * Temporary
 * 
 * 7     4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature
 * 
 * 6     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 5     4/13/05 13:15 Frazzitl
 * Miscellaneous changes primarily for status processing
 * 
 * 4     4/07/05 1:32p Frazzitl
 * Fixed problem with clock
 * 
 * 3     3/30/05 2:59p Frazzitl
 * Fixed some startup problems
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 15:08 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
