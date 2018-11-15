//===========================================================================
//
//	Module Name:	GetStatus.cpp
//
//	Function:		Interrupt Service Routines for the 60 Hz A/D status.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005-2006,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

void CParserThread::GetStatus()
{

	if ( m_nLineCount < 65000 )
		 m_nLineCount += 1;
	else
		 m_nLineCount  = 0;

//theApp.EnterCriticalSection1(&m_CriticalSection);

	if ( m_IO.InputW(SVSTAT_CS) & ARGON_FLOW)
	{
		if ( ! ( ARGON_FLG & m_nFatal ) )
		{
			if ( ++m_cArgonFlowCnt > 60 )
				ErrorMsg( ( int ) YES, ERR_ARGON_FLOW );
		}
	}
	else if ( ( ARGON_FLG & m_nFatal ) && ( m_cOperationMode == DIAG ) )
	{
		m_cArgonFlowCnt = 0;
		ErrorMsg( ( int ) OK, MSG_ARGON_FLOW );
	}
	else
		m_cArgonFlowCnt = 0;

	if ( m_IO.InputW(SVSTAT_CS) & NITROGEN_FLOW )
	{
		if ( ! ( NITROGEN_FLG & m_nWarning ) )
		{
			if ( ++m_cNitrogenFlowCnt > 60 )
				ErrorMsg( ( int ) NO, WARN_NITROGEN_FLOW );
		}
	}
	else if ( NITROGEN_FLG & m_nWarning )
	{
		m_cNitrogenFlowCnt = 0;
		ErrorMsg( ( int ) OK, MSG_NITROGEN_FLOW );
	}
	else
		m_cNitrogenFlowCnt = 0;

	if (!(m_IO.InputW(SVSTAT_CS) & WATER_LEAK ))
	{
		if ( ! ( WATER_LEAK_FLG & m_nFatal ) )
		{
			if ( ++m_cWaterLeakCnt > 60 )
				ErrorMsg( ( int ) YES, ERR_WATER_LEAK );
		}
	}
	else if ( ( WATER_LEAK_FLG & m_nFatal ) && ( m_cOperationMode == DIAG ) )
	{
		m_cWaterLeakCnt = 0;
		ErrorMsg( ( int ) OK, MSG_WATER_LEAK );
	}
	else
		m_cWaterLeakCnt = 0;

	switch ( m_nStatusNo )
	{
		case 1:		// 24 vdc
			if ( m_nPS24vdc < PS24VDC_LOW )
			{
				if ( m_nPS24vdc < PS24VDC_MIN )
				{
					if ( ! ( PS24VDC_FLG & m_nFatal ) )
					{
						if ( ++m_cPS24vdcCnt > 8 )
							ErrorMsg( ( int ) YES, ERR_24VDC_MIN );
					}
				}
				else
				{
					if ( ! ( PS24VDC_FLG & m_nWarning ) )
					{
						if ( ++m_cPS24vdcCnt > 8 )
							ErrorMsg( ( int ) NO, WARN_24VDC_LOW );
					}
				}
			}
			else if ( m_nPS24vdc > PS24VDC_HIGH )
			{
				if ( m_nPS24vdc > PS24VDC_MAX )
				{
					if ( ! ( PS24VDC_FLG & m_nFatal ) )
					{
						if ( ++m_cPS24vdcCnt > 8 )
							ErrorMsg( ( int ) YES, ERR_24VDC_MAX );
					}
				}
				else
				{
					if ( ! ( PS24VDC_FLG & m_nWarning ) )
					{
						if ( ++m_cPS24vdcCnt > 8 )
							ErrorMsg( ( int ) NO, WARN_24VDC_HIGH );
					}
				}
			}
			else if ( ( PS24VDC_FLG & m_nWarning ) && ( m_nPS24vdc > PS24VDC_LOW_OK )
					&& ( m_nPS24vdc < PS24VDC_HIGH_OK ) )
			{
				m_cPS24vdcCnt = 0;
				ErrorMsg( ( int ) OK, MSG_24VDC_OK );
			}
			else if ( ( m_cOperationMode == DIAG ) && ( PS24VDC_FLG & m_nFatal ) &&
					(m_nPS24vdc > PS24VDC_LOW_OK) && (m_nPS24vdc < PS24VDC_HIGH_OK))
			{
				m_cPS24vdcCnt = 0;
				ErrorMsg( ( int ) OK, MSG_24VDC_OK );
			}
			else
				m_cPS24vdcCnt = 0;
			m_nStatusNo = 2;		
			break;
		case 2:		// UV TEC Cooler vdc
			if (m_nWater_toggle == FALSE)
			{
				if ( m_nUVcooler > UV_COOLER_MAX )
				{
					if ( ! ( UV_COOLER_FLG & m_nFatal ) )
					{
						if ( ++m_cUVcoolerCnt > 8 )
							ErrorMsg( ( int ) YES, ERR_UV_COOLER_MAX );
					}
				}
				else if ( m_nUVcooler < UV_COOLER_LOW )
				{
					if ( m_nUVcooler < UV_COOLER_MIN )
					{
						if ( ! ( UV_COOLER_FLG & m_nFatal ) )
						{
							if ( ++m_cUVcoolerCnt > 8 )
								ErrorMsg( ( int ) YES, ERR_UV_COOLER_MIN );
						}
					}
					else
					{
						if ( ! ( UV_COOLER_FLG & m_nWarning ) )
						{
							if ( ++m_cUVcoolerCnt > 8 )
								ErrorMsg( ( int ) NO, WARN_UV_COOLER_LOW );
						}
					}
				}
				else if ((UV_COOLER_FLG & m_nWarning) &&
						(m_nUVcooler > UV_COOLER_LOW_OK) &&
						(m_nUVcooler < UV_COOLER_HIGH_OK))
				{
					m_cUVcoolerCnt = 0;
					ErrorMsg( ( int ) OK, MSG_UV_COOLER_OK );
				}
				else if ((m_cOperationMode == DIAG) && (UV_COOLER_FLG & m_nFatal) &&
					(m_nUVcooler > UV_COOLER_LOW_OK) &&
					(m_nUVcooler < UV_COOLER_HIGH_OK))
				{
					m_cUVcoolerCnt = 0;
					ErrorMsg( ( int ) OK, MSG_UV_COOLER_OK );
				}
				else
					m_cUVcoolerCnt = 0;
			}
			m_nStatusNo = 3;		
			break;
		case 3:		// VIS TEC Cooler vdc
			if (m_bSingleChannel)
			{	
				m_nVIScooler = VIS_COOLER_OK;
			}
			if (m_nWater_toggle == FALSE)
			{
				if ( m_nVIScooler > VIS_COOLER_MAX )
				{
					if ( ! ( VIS_COOLER_FLG & m_nFatal ) )
					{
						if ( ++m_cVIScoolerCnt > 8 )
							ErrorMsg( ( int ) YES, ERR_VIS_COOLER_MAX );
					}
				}
				else if ( m_nVIScooler < VIS_COOLER_LOW )
				{
					if ( m_nVIScooler < VIS_COOLER_MIN )
					{
						if ( ! ( VIS_COOLER_FLG & m_nFatal ) )
						{
							if ( ++m_cVIScoolerCnt > 8 )
								ErrorMsg( ( int ) YES, ERR_VIS_COOLER_MIN );
						}
					}
					else
					{
						if ( ! ( VIS_COOLER_FLG & m_nWarning ) )
						{
							if ( ++m_cVIScoolerCnt > 8 )
								ErrorMsg( ( int ) NO, WARN_VIS_COOLER_LOW );
						}
					}
				}
				else if ((VIS_COOLER_FLG & m_nWarning) &&
						(m_nVIScooler > VIS_COOLER_LOW_OK) &&
						(m_nVIScooler < VIS_COOLER_HIGH_OK))
				{
					m_cVIScoolerCnt = 0;
					ErrorMsg( ( int ) OK, MSG_VIS_COOLER_OK );
				}
				else if ((m_cOperationMode == DIAG) && (VIS_COOLER_FLG & m_nFatal) &&
						(m_nVIScooler > VIS_COOLER_LOW_OK) &&
						(m_nVIScooler < VIS_COOLER_HIGH_OK))
				{
					m_cVIScoolerCnt = 0;
					ErrorMsg( ( int ) OK, MSG_VIS_COOLER_OK );
				}
				else
					m_cVIScoolerCnt = 0;
			}
			m_nStatusNo  = 4;		
			break;
		case 4:		// UV Detector Temperature
 			if (m_nTempccdUV < m_nTempUVMin)
 					m_nTempUVMin = m_nTempccdUV;
			if ( m_nTempccdUV < UV_CCD_TEMP_MIN )		// 19 (-55)
			{
				if ( ! ( UV_CCD_FLG & m_nFatal ) )
				{
					if ( ++m_cTempccdUVCnt > 4 )
						//..............................................
						// THN Jan 2007 ... Comment following lint to remove the too cold fatal test
						ErrorMsg( ( int ) YES, ERR_UV_CCD_MIN );
						//..............................................
				}
			}
			else if ( m_nTempccdUV > UV_CCD_TEMP_HIGH )	// 107 (-10)
			{
				if (m_nTempccdUV > m_nTempUVLast)
					++m_cUVflag;
				else if (m_nTempccdUV < m_nTempUVLast)
					m_cUVflag = 0;
				if (m_cUVflag > 3)
				{
					if (!(UV_CCD_FLG & m_nFatal))
						ErrorMsg((int) YES, ERR_UV_CCD_HEATING);
				}
				if ( (m_nTempccdUV - m_nTempUVMin) > 20)		// ~ 10 degrees
				{
					if (!(UV_CCD_FLG & m_nFatal))
						ErrorMsg((int) YES, ERR_UV_CCD_HEATING);
				}
				if ( m_nTempccdUV > UV_CCD_TEMP_MAX )
				{
					if ( ! ( UV_CCD_FLG & m_nFatal ) )
					{
						if ( ++m_cTempccdUVCnt > 4 )
							ErrorMsg( ( int ) YES, ERR_UV_CCD_MAX );
					}
				}
				else
				{
					if ( ! ( UV_CCD_FLG & m_nWarning ) )
					{
						if ( ++m_cTempccdUVCnt > 4 )
							ErrorMsg( ( int ) NO, WARN_UV_CCD_HIGH );
					}
				}
			}
			else if ((UV_CCD_FLG & m_nWarning) &&
					(m_nTempccdUV < UV_CCD_TEMP_HIGH_OK))
			{
				m_cTempccdUVCnt = 0;
				ErrorMsg( ( int ) OK, MSG_UV_CCD_OK );
			}
			else if ((m_cOperationMode == DIAG) && (UV_CCD_FLG & m_nFatal) &&
					(m_nTempccdUV < UV_CCD_TEMP_HIGH_OK))
			{
				m_cTempccdUVCnt = 0;
				ErrorMsg( ( int ) OK, MSG_UV_CCD_OK );
			}
			else
				m_cTempccdUVCnt = 0;
			m_nTempUVLast = m_nTempccdUV;
			m_nStatusNo = 5;		
			break;
		case 5:					
//			m_nTempCoarseTop;		// Coarse temperature at top
//			m_nTempCoarseBottom;	// Coarse temperature at bottom
//			m_nTempCoarseFin;		// Coarse temperature at fin
//			m_nTempCoarseTub;		// Coarse temperature at tub
			m_nStatusNo = 6;		
			break;
		case 6:					
//			m_nTempFineTop;			// Fine tenperature at top
//			m_nTempFineBottom;		// Fine tenperature at bottom
//			m_nTempFineFin;			// Fine tenperature at fin
//			m_nTempFineTub;			// Fine tenperature at tub
			// do temperature stuff
//			if (m_nTempCoarseTop < 255 && m_nTempCoarseBottom < 255 && m_nTempCoarseFin < 255 && m_nTempCoarseTub < 255)
			check_temps();
			m_nStatusNo   = 7;		
			break;
		case 7:					
			if ( ( m_nTempFan < FAN_TEMP_HIGH ) && ( ! ( FAN_FLG & m_nWarning ) ) )
			{
				if ( ++m_cTempFanCnt > 8 )
					ErrorMsg( ( int ) NO, WARN_FAN_TEMP_HIGH );
			}
			else if ((m_nTempFan > FAN_TEMP_LOW) && (!(FAN_FLG & m_nWarning)))
			{
				if ( ++m_cTempFanCnt > 8 )
					ErrorMsg( ( int ) NO, WARN_FAN_TEMP_LOW );
			}
			else if ((FAN_FLG & m_nWarning) && (m_nTempFan < FAN_TEMP_LOW_OK) &&
					(m_nTempFan > FAN_TEMP_HIGH_OK))
			{
				m_cTempFanCnt = 0;
				ErrorMsg( ( int ) OK, MSG_FAN_TEMP_OK );
			}
			else
				m_cTempFanCnt = 0;
			m_nStatusNo    = 8;	
			break;
		case 8:		// VIS Detector Temperature
			if ( m_cOperationMode == AUTO )
				m_nTempccdUV  = 0;
			else
			{
				if (m_bSingleChannel)
				{	
					m_nTempccdVIS = VIS_CCD_TEMP_OK;
				}
				if (m_nTempccdVIS < m_nTempVISMin)
							m_nTempVISMin = m_nTempccdVIS;
				if ( m_nTempccdVIS < VIS_CCD_TEMP_MIN )			//  19 (-55)
				{
					if ( ! ( VIS_CCD_FLG & m_nFatal ) )
					{
						if ( ++m_cTempccdVISCnt > 4 )
						//..............................................
						// THN Jan 2007 ... Comment following lint to remove the too cold fatal test
						ErrorMsg( ( int ) YES, ERR_VIS_CCD_MIN );
						//..............................................
					}
				}
				else if ( m_nTempccdVIS > VIS_CCD_TEMP_HIGH )	// 107 (-10)
				{
						if (m_nTempccdVIS > m_nTempVISLast)
							++m_cVISflag;
						else if (m_nTempccdVIS < m_nTempVISLast)
							m_cVISflag = 0;
						if (m_cVISflag > 3)
						{
							if (!( VIS_CCD_FLG & m_nFatal))
								ErrorMsg((int) YES, ERR_VIS_CCD_HEATING);
						}
						if ( (m_nTempccdVIS - m_nTempVISMin) > 20 )		// ~ 10 degrees
						{
							if (!( VIS_CCD_FLG & m_nFatal))
								ErrorMsg((int) YES, ERR_VIS_CCD_HEATING);
						}
					if ( m_nTempccdVIS > VIS_CCD_TEMP_MAX )		// 107 (-10)
					{
						if ( ! ( VIS_CCD_FLG & m_nFatal ) )
						{
							if ( ++m_cTempccdVISCnt > 4 )
								ErrorMsg( ( int ) YES, ERR_VIS_CCD_MAX );
						}
					}
					else
					{
						if ( ! ( VIS_CCD_FLG & m_nWarning ) )
						{
							if ( ++m_cTempccdVISCnt > 4 )
								ErrorMsg( ( int ) NO, WARN_VIS_CCD_HIGH );
						}
					}
				}
				else if ((VIS_CCD_FLG & m_nWarning) &&
						(m_nTempccdVIS < VIS_CCD_TEMP_HIGH_OK))
				{
					m_cTempccdVISCnt = 0;
					ErrorMsg( ( int ) OK, MSG_VIS_CCD_OK );
				}
				else if ((m_cOperationMode == DIAG) && (VIS_CCD_FLG & m_nFatal) &&
						(m_nTempccdVIS < VIS_CCD_TEMP_HIGH_OK))
				{
					m_cTempccdVISCnt = 0;
					ErrorMsg( ( int ) OK, MSG_VIS_CCD_OK );
				}
				else
					m_cTempccdVISCnt = 0;
			}
			m_nTempVISLast = m_nTempccdVIS;
			m_nStatusNo    = 1;
			if (m_bHWInitComplete == true)
				savetime();
			break;
		default:				
			m_nStatusNo = 1;		
			break;
	}
//theApp.LeaveCriticalSection1(&m_CriticalSection);
}
//===========================================================================

void CParserThread::savetime()
{
	BYTE	 bcdval;
	BYTE	 bcd;


	SaveOffTime(NORMAL_TIME); // Save current time in off time area

	//=================================================================================
	//	The following is used to toggle the water valve to keep it from freezing open
	//		Once each day at 4:00:00am, if the system is READY,
	//			the water valve is close
	//		If the valve has been closed, at 4:00:04am and the system is READY,
	//		the water valve is re-opened
	//	This should exercise the valve with causing a measurable change to the system
	//=================================================================================

	if (m_cWaterDay != m_NvRam.InputB(SCLKDATE))
	{	// This insures it happens only once a day
		m_cWaterDay = m_NvRam.InputB(SCLKDATE);
		m_bCheckWaterFlag = true;
		m_nWater_toggle = FALSE;
	}
	if (m_bCheckWaterFlag)
	{	// This insures the check is done only until the valve is toggled
		bcdval = m_NvRam.InputB(SCLKHOUR);
		bcd = (BYTE) (0x30 & bcdval);
		if (bcd == 0)
		{
			bcd	= (BYTE) (0x0f & bcdval);
			if (bcd == 4)						
			{
				bcdval = m_NvRam.InputB(SCLKMIN);
				bcd = (BYTE) (0x70 & bcdval);
				if (bcd == 0)
				{
					bcd	= (BYTE) (0x0f & bcdval);
					if (bcd == 0)				
					{
						bcdval = m_NvRam.InputB(SCLKSEC);
						bcd = (BYTE) (0x70 & bcdval);
						if (bcd == 0)
						{
							bcd = (BYTE) (0x0f & bcdval);
							if (bcd >= 0 && m_nWater_toggle == FALSE)		// 4:00:00 each day
							{	// Turn Water OFF only if in READY mode
								if (m_cStartupMode == READY)
								{
									m_nSolenoidRegister &= ~WATER_EN;
									m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
									m_nWater_toggle = TRUE;
								}
								else
								{
									m_bCheckWaterFlag = false;
								}
							}
							else if (bcd >= 4 && m_nWater_toggle == TRUE)	// 04:00:04 each day
							{	// Turn Water ON only if in READY mode
								m_bCheckWaterFlag   = false;
								if (m_cStartupMode == READY)
								{
									m_nSolenoidRegister |= WATER_EN;
									m_IO.OutputW(SOLENOID_REGISTER, m_nSolenoidRegister);
									m_nWater_toggle = FALSE;
								}
							}
						}
					}
				}
			}
		}
	}
}
//===========================================================================

WORD CParserThread::Average(WORD newValue, WORD oldValue)
{
// Old firmware did Average in integer to eliminate multiply function
//	WORD	result = (((newValue << 1) + newValue + oldValue) >> 2);
// No problem now, so make it more readable
	WORD	result = (WORD) ((3 * newValue + oldValue) / 4);
	return	result;
}
//===========================================================================

/*** Revision History ***

11/17/16    JO      CBF-78 - Add "Lid Off mode" functionality.
01/16/17    JO      CBF-133 - Add cmd_JI functionality.

$Log: /IcarusBased/SpecFW/Getstat.cpp $
 * 
 * 36    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 35    2/01/07 4:48p Nashth
 * A few modifications from prior update.
 * 
 * 31    1/19/07 3:41a Nashth
 * Redacted removal of fatal mode checks for detector temperature Too Low,
 * and left comments in place for easy reversal should that be required.
 * 
 * 30    1/17/07 4:59p Nashth
 * Cooler minimum check also removed for VIS camera in addition to UV
 * camera.
 * 
 * 29    1/16/07 5:21p Nashth
 * Detector Too Cold check removed and only detector cooler temperature
 * read from mux.
 * 
 * 28    12/19/06 2:49p Nashth
 * Initialized minimum detector temperature variables to 255, and fixed a
 * number of constraints for determining the minimum value and entering
 * Fatal mode for detector tempareatures as per QPR: CANU6WMPKS.
 * 
 * 27    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 26    11/14/05 1:40p Frazzitl
 * Added function to average A/D readings for smoother data.  Also added a
 * delay to be sure data is real before used in GetStatus() routines.
 * 
 * 25    11/11/05 2:56p Frazzitl
 * Corrected problem with OffTime variable because of savetime() being
 * called before hardware initialization completed.
 * 
 * 24    11/11/05 8:25a Frazzitl
 * Added special clock critical section around all clock functions.
 * 
 * 23    7/21/05 8:21 Frazzitl
 * This fixed the strange behavior of the Tub temperature caused by the
 * water valve being turned off, but never turned back on.
 * 
 * 22    7/08/05 9:57 Frazzitl
 * Fixed problem with system clock updating.
 * 
 * 21    7/07/05 10:31 Frazzitl
 * Another fix
 * 
 * 20    6/29/05 16:21 Frazzitl
 * Implemented Status checking
 * 
 * 19    6/17/05 2:52p Frazzitl
 * Corrected the temperature equation constants and the status comparisons
 * 
 * 18    6/17/05 10:13a Frazzitl
 * Corrected comparisons because of changes in the equation for
 * determining the Ambient temperatures.
 * 
 * 17    6/06/05 12:38p Frazzitl
 * Fixed problem with Water Leak signal - High indicates NO Leak
 * 
 * 16    5/26/05 9:12a Frazzitl
 * Added changes recommended by Remco to fix crashing problem
 * 
 * 15    5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 14    5/23/05 16:37 Frazzitl
 * Commented out code not yet tested
 * 
 * 13    5/11/05 11:57 Frazzitl
 * Temporarily eliminated error messages while getting the tub and
 * detector temps to work properly.
 * 
 * 12    5/10/05 11:54 Frazzitl
 * Fixed problem with threads
 * 
 * 11    4/29/05 12:49 Frazzitl
 * Trying to get the threat to work properly
 * 
 * 10    4/28/05 14:16 Frazzitl
 * Getting basic firmware to run
 * 
 * 9     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 8     4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature
 * 
 * 7     4/22/05 15:18 Frazzitl
 * Improved Status reads
 * 
 * 6     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 5     4/19/05 4:44p Frazzitl
 * Fixed reading A/D values and corrected Water Leak display
 * 
 * 4     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 3     4/08/05 8:30 Frazzitl
 * Temporary change for testing
 * 
 * 2     4/07/05 1:32p Frazzitl
 * Fixed problem with clock
 * 
 * 1     3/28/05 9:49 Frazzitl
 * Missing files

$NoKeywords: $

** End of Rev History **/
