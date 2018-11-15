//===========================================================================
//
//	Module Name:	Decoder.cpp
//
// 	Function:		This routine decodes the COMMAND STRING and
//		 			calls the proper subroutine to execute the command.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::Decoder()
{
	unsigned int	wStatus(NO_ERRORS);

	m_pCmdPtr = m_pCmdPtrSaved;
	m_nBytesRead = 0;

	while (*m_pCmdPtr != (char) END && wStatus == NO_ERRORS && !m_bAbortFlag)
	{
		m_nCmd   = *m_pCmdPtr++;	// get one command
		m_nCmd <<= 8;
		m_nCmd  |= *m_pCmdPtr++;
		m_nBytesRead += 2;

		switch (m_nCmd)
		{
			case CMD_0C:
				wStatus	= cmd0C();
				if (m_bAbortFlag)
					break;
				return(wStatus);

			case CMD_1D:
				wStatus	= cmd1D();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_2D:
				wStatus	= cmd2D();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_0H:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmd0H();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_1H:
				wStatus	= cmd1H();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_3T:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmd3T();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_1V:
				wStatus	= cmd1V();
				return(wStatus);
			case CMD_2V:
				wStatus	= cmd2V();
				return(wStatus);
			case CMD_3V:
				wStatus	= cmd3V();
				return(wStatus);
			case CMD_4V:
				wStatus	= cmd4V();
				return(wStatus);
			case CMD_5V:
				wStatus	= cmd5V();
				return(wStatus);
			case CMD_BS:
				wStatus	= cmdBS();
				break;
			case CMD_BX:
				wStatus	= cmdBX();
				break;
			case CMD_BY:
				wStatus	= cmdBY();
				break;
			case CMD_C0:
				wStatus	= cmdC0();
				break;
			case CMD_C1:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdC1();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_C2:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdC2();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_CB:
				wStatus = cmdCB();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_CD:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus = cmdCD();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_CH:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdCH();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_CM:
			case CMD_CP:
				wStatus	= cmdCM();
				break;
			case CMD_CS:
				wStatus	= cmdCS();
				break;
			case CMD_CT:
				wStatus	= cmdCT();
				break;
			case CMD_CX:
				wStatus	= cmdCX();
				break;
			case CMD_CY:
				wStatus	= cmdCY();
				break;
			case CMD_D0:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdD0();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_D1:
				wStatus	= cmdD1();
				break;
			case CMD_D2:
				wStatus	= cmdD2();
				break;
			case CMD_DE:
				wStatus	= cmdDE();
				break;
			case CMD_DF:
				wStatus	= cmdDF();
				break;

			case CMD_DM:
				wStatus	= cmdDM();
				break;
			case CMD_DU:
				wStatus	= cmdDU();
				break;
			case CMD_DV:
				wStatus	= cmdDV();
				break;
			case CMD_EC:
				wStatus	= cmdEC();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_ES:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdES();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_F1:
				wStatus	= cmdF1();
				break;
			case CMD_FC:
				wStatus	= cmdFC();
				break;
			case CMD_FS:
				wStatus	= cmdFS();
				break;
			case CMD_FX:
				wStatus	= cmdFX();
				break;
			case CMD_FY:
				wStatus	= cmdFY();
				break;
			case CMD_GB:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdGB();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_GC:
				wStatus	= cmdGC();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GD:
				wStatus	= cmdGD();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GE:
				wStatus	= cmdGE();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GG:
				wStatus	= cmdGG();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GL:
				wStatus	= cmdGL();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GM:
				wStatus	= cmdGM();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GS:
				wStatus	= cmdGS();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GT:
				wStatus	= cmdGT();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_GV:
				wStatus	= cmdGV();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_H1:
				wStatus	= cmdH1();
				break;
			case CMD_HG:
				wStatus	= cmdHG();
				break;
			case CMD_HK:
				wStatus	= cmdHK();
				break;
			case CMD_HO:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdHO();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				return(wStatus);
				break;
			case CMD_HS:
				wStatus	= cmdHS();
				break;
			case CMD_HX:
				wStatus	= cmdHX();
				break;
			case CMD_HY:
				wStatus	= cmdHY();
				break;
			case CMD_HU:
				wStatus	= cmdHU();
				break;
			case CMD_HV:
				wStatus	= cmdHV();
				break;
			case CMD_I1:
				wStatus	= cmdI1();
				break;
			case CMD_I2:
				wStatus	= cmdI2();
				break;
			case CMD_I3:
				wStatus	= cmdI3();
				break;
			case CMD_I4:
				wStatus	= cmdI4();
				break;
			case CMD_ID:
				wStatus	= cmdID();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_IS:
				wStatus	= cmdIS();
				if (m_bAbortFlag)
					break;
				return(wStatus);
				break;
			case CMD_IT:
				wStatus	= cmdIT();
				break;
			case CMD_JI:
				wStatus	= cmdJI();
				break;
			case CMD_LL:
				wStatus	= cmdLL();
				break;
			case CMD_LN:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdLN();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_LP:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdLP();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_LS:
				wStatus	= cmdLS();
				break;
			case CMD_LX:
				wStatus	= cmdLX();
				break;
			case CMD_LY:
				wStatus	= cmdLY();
				break;
			case CMD_MC:
				wStatus	= cmdMC();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_MD:
				wStatus	= cmdMD();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_MS:
				wStatus	= cmdMS();
				break;
			case CMD_MW:
				wStatus	= cmdMW();
				break;
			case CMD_MX:
				wStatus	= cmdMX();
				break;
			case CMD_MY:
				wStatus	= cmdMY();
				break;
			case CMD_OH:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdOH();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_PI:
				wStatus	= cmdPI();
				break;
			case CMD_PS:
				wStatus	= cmdPS();
				break;
			case CMD_PX:
				wStatus	= cmdPX();
				break;
			case CMD_PY:
				wStatus	= cmdPY();
				break;
			case CMD_RB:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdRB();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_RC:
				wStatus	= cmdRC();
				break;
			case CMD_RD:
				wStatus	= cmdRD();
				break;
			case CMD_RF:
				wStatus	= cmdRF();
				break;
			case CMD_RI:
				wStatus	= cmdRI();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_RM:
				wStatus = cmdRM();
				break;
			case CMD_RO:
				wStatus	= cmdRO();
				break;
			case CMD_RR:
				wStatus	= cmdRR();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_RT:
				wStatus	= cmdRT();
				return(wStatus);
			case CMD_RU:
				wStatus	= cmdRU();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_RV:
				wStatus	= cmdRV();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_SA:
				wStatus	= cmdSA();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_SB:
				wStatus	= cmdSB();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_SC:
				wStatus	= cmdSC();
				break;
			case CMD_SD:
				wStatus	= cmdSD();
				break;
			case CMD_SE:
				wStatus	= cmdSE();
				break;
			case CMD_SG:
				wStatus	= cmdSG();
				break;
			case CMD_SI:
				wStatus	= cmdSI();
				break;
			case CMD_SK:
				wStatus	= cmdSK();
				break;
			case CMD_SL:
				wStatus	= cmdSL();
				break;
			case CMD_SM:
				wStatus	= cmdSM();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_SN:
				wStatus	= cmdSN();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_SR:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdSR();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_ST:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdST();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_SU:
				wStatus	= cmdSU();
				break;
			case CMD_SV:
				wStatus	= cmdSV();
				break;
			case CMD_SY:
				wStatus	= cmdSY();
				break;
 			case CMD_SZ:
				wStatus	= cmdSZ();
				break;
			case CMD_TA:
				wStatus	= cmdTA();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_TB:
				wStatus	= cmdTB();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_TT:
				wStatus	= cmdTT();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_TC:
				wStatus	= cmdTC();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_TM:
				wStatus	= cmdTM();
				break;
			case CMD_TR:
				wStatus	= cmdTR();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_UM:
				wStatus	= cmdUM();
				break;
			case CMD_VC:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdVC();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_VS:
				wStatus	= cmdVS();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_VT:
				wStatus	= cmdVT();
				break;
			case CMD_VV:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdVV();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_VX:
				wStatus	= cmdVX();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_VY:
				wStatus	= cmdVY();
				if (m_bAbortFlag)
					break;
				return(wStatus);
			case CMD_WB:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdWB();
theApp.LeaveCriticalSection1(&m_CriticalSection);
				break;
			case CMD_WE:
				wStatus	= cmdWE();
				break;
			case CMD_XA:
			case CMD_XU:
			case CMD_XV:
theApp.EnterCriticalSection1(&m_CriticalSection);
				wStatus	= cmdXX();
theApp.LeaveCriticalSection1(&m_CriticalSection);
			 	break;		
				
// New commands added for Century B	start
			case CMD_HA:
				wStatus	= cmdHA();
				break;
			case CMD_HB:
				wStatus	= cmdHB();
				break;
			case CMD_MB:
				wStatus	= cmdMB();
				break;
			case CMD_QB:
				wStatus	= cmdQB();
				break;
			case CMD_BQ:
				wStatus	= cmdBQ();				
				break;
			case CMD_NA:
				wStatus	= cmdNA();
				break;
			case CMD_NB:
				wStatus	= cmdNB();
				break;
			case CMD_UB:
				wStatus	= cmdUB();
				break;
			case CMD_UC:
				wStatus	= cmdUC();
				break;
			case CMD_XB:
				wStatus	= cmdXB();
				break;
			case CMD_XC:
				wStatus	= cmdXC();
				break;
			case CMD_KA:
				wStatus	= cmdKA();
				break;
			case CMD_KB:
				wStatus	= cmdKB();
				break;
			case CMD_KC:
				wStatus	= cmdKC();
				break;
			case CMD_PA:
				wStatus	= cmdPA();
				break;
			case CMD_PR:
				wStatus	= cmdPR();
				break;
			case CMD_PK:
				wStatus	= cmdPK();
				break;				
			case CMD_IM:
				wStatus	= cmdIM();
				break;	
			case CMD_UA:
				wStatus	= cmdUA();
				break;
			case CMD_LA:
				wStatus	= cmdLA();
				break;
			case CMD_AU:
				wStatus	= cmdAU();
				break;
			case CMD_AL:
				wStatus	= cmdAL();
				break;
			case CMD_CA:
				wStatus	= cmdCA();
				break;
		
// New commands added for Century B	end
			
			default:
			{
				short	nTemp   = (short) (0xff00 & m_nCmd);
						nTemp >>= 8;
				char	cChar1  = (char) nTemp;
				char	cChar2  = (char) (0x00ff & m_nCmd);
				printf("Decoder ERR_CMD 0x%x (%c%c)\r\n", m_nCmd, cChar1, cChar2);
				wStatus = ERR_CMD;
				theApp.EnterCriticalSection1(&m_CriticalSection);
				m_nDataOutBuf[0] = cChar1;
				m_nDataOutBuf[1] = cChar2;
				m_nDataOutBuf[2] = '0';
				m_nDataOutBuf[3] = '1';
				m_nDataOutBuf[4] = NULL;
				theApp.LeaveCriticalSection1(&m_CriticalSection);
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

/*** Revision History ***

12/18/15    JO  Initial additions and changes for Century B.
03/21/16    JO	Restored XA command code to diagostics.  Changed "Set Toroid key radial position"
					 command code to XD.
07/11/16    JO  CBF-18  - Add and implement Park motors command "PK".					
08/02/16    JO  CBF-26    Removed unused toroid commands and references.
10/27/16    JO  CBF-65  - Code changes to support Syngistix release 3.0.
01/28/17	KR	CBF-143 - Remove thermal commands
01/31/17	KR	CBF-145 - Remove unused commands and errors
02/02/17    JO  CBF-147 - Add get/set BRAM commands for maximum shutter position.
02/03/17	KR	CBF-151 - Additional fixes to run the commands properly & remove ES warning
					
$Log: /IcarusBased/SpecFW/Decoder.cpp $
 * 
 * 26    3/31/08 5:31p Nashth
 * Implementation of spectrometer history log... First pass.
 * 
 * 25    7/06/07 4:51p Nashth
 * Added an information error string when a detector read error occurs as
 * per QPR: THNJ74CQCW.
 * 
 * 24    1/30/07 4:08p Nashth
 * First pass at keeping the camera on during a 'long' connection as per
 * QPR: THNJ6XXLK3.
 * 
 * 23    1/23/07 1:07p Nashth
 * 
 * 22    1/23/07 11:11a Nashth
 * Added two functions AR<000> sets Auto Reset Time and
 * RA that retreives Auto Reset Time. Units = tenths of hours.
 * 
 * 21    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 20    11/04/05 11:34a Nashth
 * Adjusted for correct operation of CC command.
 * 
 * 19    10/17/05 9:36a Frazzitl
 * Added functions needed to read Detector FPGA version number.
 * 
 * 18    10/12/05 12:51p Frazzitl
 * Added ability to turn Watchdog timer On or Off - for testing only.
 * 
 * 17    9/26/05 11:35a Nashth
 * Fixed problem with abort flag
 * 
 * 16    9/16/05 1:26p Frazzitl
 * Fixing TT command
 * 
 * 15    9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 14    8/10/05 11:42 Frazzitl
 * Added new commands to read and write BRAM bytes.
 * 
 * 13    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 12    7/06/05 16:35 Frazzitl
 * Corrected minor problems
 * 
 * 11    7/05/05 10:30 Frazzitl
 * Fixed error message output
 * 
 * 10    7/01/05 15:37 Frazzitl
 * Fixed the Tub heater problem
 * 
 * 9     6/29/05 14:01 Frazzitl
 * Fixed minor problem
 * 
 * 8     6/06/05 8:28a Frazzitl
 * Added new functions to return CPLD and Personality Board version
 * numbers.
 * 
 * 7     5/26/05 8:26 Frazzitl
 * Temporary
 * 
 * 6     4/28/05 14:16 Frazzitl
 * Getting basic firmware to run
 * 
 * 5     4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature
 * 
 * 4     4/13/05 13:15 Frazzitl
 * Miscellaneous changes primarily for status processing
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
