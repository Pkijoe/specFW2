//===========================================================================
//
//	Module Name:	GetStatus2.cpp
//
//	Function:	  	Interrupt Service Routines for the 60 Hz A/D status.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005-2006,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::GetStatusInit()
{
	if (m_nLineCount < 65000)
		 m_nLineCount += 1;
	else
		 m_nLineCount  = 0;
	
//theApp.EnterCriticalSection1(&m_CriticalSection);

	if (m_IO.InputW(SVSTAT_CS) & ARGON_FLOW)
	{
		if (!(ARGON_FLG & m_nFatal))
		{
			if (++m_cArgonFlowCnt > 60)
				ErrorMsg((int) YES, ERR_ARGON_FLOW);
		}
	}
	else if ((ARGON_FLG & m_nFatal) && (m_cOperationMode == DIAG))
	{
		m_cArgonFlowCnt = 0;
		ErrorMsg((int) OK, MSG_ARGON_FLOW);
	}
	else
		m_cArgonFlowCnt = 0;

	if (m_IO.InputW(SVSTAT_CS) & NITROGEN_FLOW)
	{
		if (!(NITROGEN_FLG & m_nWarning))
		{
			if (++m_cNitrogenFlowCnt > 60)
		 		ErrorMsg((int) NO, WARN_NITROGEN_FLOW);
		}
	}
	else if (NITROGEN_FLG & m_nWarning)
	{
		m_cNitrogenFlowCnt = 0;
		ErrorMsg((int) OK, MSG_NITROGEN_FLOW);
	}
	else
		m_cNitrogenFlowCnt = 0;

 	if (!(m_IO.InputW(SVSTAT_CS) & WATER_LEAK))
	{
		if (!(WATER_LEAK_FLG & m_nFatal))
		{
		 	if (++m_cWaterLeakCnt > 60)
	 			ErrorMsg((int) YES, ERR_WATER_LEAK);
		}
	}
	else if ((WATER_LEAK_FLG & m_nFatal) && (m_cOperationMode == DIAG))
	{
		m_cWaterLeakCnt = 0;
		ErrorMsg((int) OK, MSG_WATER_LEAK);
	}
	else
		m_cWaterLeakCnt = 0;

	switch (m_nStatusNo)
	{
		case 1:					
			if (m_nPS24vdc < PS24VDC_LOW)
			{
				if (m_nPS24vdc < PS24VDC_MIN)
				{
					if (!(PS24VDC_FLG & m_nFatal))
					{
						if (++m_cPS24vdcCnt > 8)
							ErrorMsg((int) YES, ERR_24VDC_MIN);
					}
				}
				else
				{
					if (!(PS24VDC_FLG & m_nWarning))
					{
						if (++m_cPS24vdcCnt > 8)
							ErrorMsg((int) NO, WARN_24VDC_LOW);
					}
				}
			}
			else if (m_nPS24vdc > PS24VDC_HIGH)
			{
				if (m_nPS24vdc > PS24VDC_MAX)
				{
					if (!(PS24VDC_FLG & m_nFatal))
					{
						if (++m_cPS24vdcCnt > 8)
							ErrorMsg((int) YES, ERR_24VDC_MAX);
					}
				}
				else
				{
					if (!(PS24VDC_FLG & m_nWarning))
					{
						if (++m_cPS24vdcCnt > 8)
							ErrorMsg((int) NO, WARN_24VDC_HIGH);
					}
				}
			}
			else if ((PS24VDC_FLG & m_nWarning) && (m_nPS24vdc > PS24VDC_LOW_OK) &&
					(m_nPS24vdc < PS24VDC_HIGH_OK))
			{
				m_cPS24vdcCnt = 0;
				ErrorMsg((int) OK, MSG_24VDC_OK);
			}
			else if ((m_cOperationMode == DIAG) && (PS24VDC_FLG & m_nFatal) &&
					(m_nPS24vdc > PS24VDC_LOW_OK) && (m_nPS24vdc < PS24VDC_HIGH_OK))
			{
				m_cPS24vdcCnt = 0;
				ErrorMsg((int) OK, MSG_24VDC_OK);
			}
			else
				m_cPS24vdcCnt = 0;
			m_nStatusNo = 2;		
			break;
		case 2:					
			if (m_cStartupMode > HOT2)
			{
				if (m_nUVcooler > UV_COOLER_MAX)
				{
					if (!(UV_COOLER_FLG & m_nFatal))
					{
						if (++m_cUVcoolerCnt > 8)
							ErrorMsg((int) YES, ERR_UV_COOLER_MAX);
					}
				}
				else if (m_nUVcooler < UV_COOLER_LOW)
				{
					if (m_nUVcooler < UV_COOLER_MIN)
					{
						if (!(UV_COOLER_FLG & m_nFatal))
						{
							if (++m_cUVcoolerCnt > 8)
								ErrorMsg((int) YES, ERR_UV_COOLER_MIN);
						}
					}
					else
					{
						if (!(UV_COOLER_FLG & m_nWarning))
						{
							if (++m_cUVcoolerCnt > 8)
								ErrorMsg((int) NO, WARN_UV_COOLER_LOW);
						}
					}
				}
				else if ((UV_COOLER_FLG & m_nWarning) &&
					(m_nUVcooler > UV_COOLER_LOW_OK) &&
					(m_nUVcooler < UV_COOLER_HIGH_OK))
				{
					m_cUVcoolerCnt = 0;
					ErrorMsg((int) OK, MSG_UV_COOLER_OK);
				}
				else if ((m_cOperationMode == DIAG) && (UV_COOLER_FLG & m_nFatal) &&
					(m_nUVcooler > UV_COOLER_LOW_OK) &&
					(m_nUVcooler < UV_COOLER_HIGH_OK))
				{
					m_cUVcoolerCnt = 0;
					ErrorMsg((int) OK, MSG_UV_COOLER_OK);
				}
				else
					m_cUVcoolerCnt = 0;
			}
			m_nStatusNo = 3;		
			break;
		case 3:					
			if (m_bSingleChannel)
			{	
				m_nVIScooler = VIS_COOLER_OK;
			}
			if (m_cStartupMode > HOT2)
			{
				if (m_nVIScooler > VIS_COOLER_MAX)
				{
					if (!(VIS_COOLER_FLG & m_nFatal))
					{
						if (++m_cVIScoolerCnt > 8)
							ErrorMsg((int) YES, ERR_VIS_COOLER_MAX);
					}
				}
				else if (m_nVIScooler < VIS_COOLER_LOW)
				{
					if (m_nVIScooler < VIS_COOLER_MIN)
					{
						if (!(VIS_COOLER_FLG & m_nFatal))
						{
							if (++m_cVIScoolerCnt > 8)
								ErrorMsg((int) YES, ERR_VIS_COOLER_MIN);
						}
					}
					else
					{
						if (!(VIS_COOLER_FLG & m_nWarning))
						{
							if (++m_cVIScoolerCnt > 8)
								ErrorMsg((int) NO, WARN_VIS_COOLER_LOW);
						}
					}
				}
				else if ((VIS_COOLER_FLG & m_nWarning) &&
					(m_nVIScooler > VIS_COOLER_LOW_OK) &&
					(m_nVIScooler < VIS_COOLER_HIGH_OK))
				{
					m_cVIScoolerCnt = 0;
					ErrorMsg((int) OK, MSG_VIS_COOLER_OK);
				}
				else if ((m_cOperationMode == DIAG) &&
					(VIS_COOLER_FLG & m_nFatal) && (m_nVIScooler > VIS_COOLER_LOW_OK)
					&& (m_nVIScooler < VIS_COOLER_HIGH_OK))
				{
					m_cVIScoolerCnt = 0;
					ErrorMsg((int) OK, MSG_VIS_COOLER_OK);
				}
				else
					m_cVIScoolerCnt = 0;
			}
			m_nStatusNo  = 4;		
			break;
		case 4:					
			if (m_cStartupMode > HOT3)
			{
	 			if (m_nTempccdUV < m_nTempUVMin)
 							m_nTempUVMin = m_nTempccdUV;
				if (m_nTempccdUV < UV_CCD_TEMP_MIN)		// 19 (-55)
				{
					if (!(UV_CCD_FLG & m_nFatal))
					{
						if (++m_cTempccdUVCnt > 4)
						//..............................................
						// THN Jan 2007 ... Comment following lint to remove the too cold fatal test
						ErrorMsg((int) YES, ERR_UV_CCD_MIN);
						//..............................................
					}
				}
				else if (m_nTempccdUV > UV_CCD_TEMP_HIGH)	// 107 (-10)
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
					if (m_nTempccdUV > UV_CCD_TEMP_MAX)
					{
						if (!(UV_CCD_FLG & m_nFatal))
						{
							if (++m_cTempccdUVCnt > 4)
								ErrorMsg((int) YES, ERR_UV_CCD_MAX);
						}
					}
					else
					{
						if (!(UV_CCD_FLG & m_nWarning))
						{
							if (++m_cTempccdUVCnt > 4)
								ErrorMsg((int) NO, WARN_UV_CCD_HIGH);
						}
					}
				}
				else if ((UV_CCD_FLG & m_nWarning) &&
					(m_nTempccdUV < UV_CCD_TEMP_HIGH_OK))
				{
					m_cTempccdUVCnt = 0;
					ErrorMsg((int) OK, MSG_UV_CCD_OK);
				}
				else if ((m_cOperationMode == DIAG) && (UV_CCD_FLG & m_nFatal) &&
					(m_nTempccdUV < UV_CCD_TEMP_HIGH_OK))
				{
					m_cTempccdUVCnt = 0;
					ErrorMsg((int) OK, MSG_UV_CCD_OK);
				}
				else
					m_cTempccdUVCnt = 0;
				m_nTempUVLast = m_nTempccdUV;
			}
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
			check_temps();
			m_nStatusNo   = 7;		
			break;
		case 7:					
			if ((m_nTempFan < FAN_TEMP_HIGH) && (!(FAN_FLG & m_nWarning)))
			{
				if (++m_cTempFanCnt > 8)
					ErrorMsg((int) NO, WARN_FAN_TEMP_HIGH);
			}
			else if ((m_nTempFan > FAN_TEMP_LOW) && (!(FAN_FLG & m_nWarning)))
			{
				if (++m_cTempFanCnt > 8)
					ErrorMsg((int) NO, WARN_FAN_TEMP_LOW);
			}
			else if ((FAN_FLG & m_nWarning) && (m_nTempFan < FAN_TEMP_LOW_OK) &&
				(m_nTempFan > FAN_TEMP_HIGH_OK))
			{
				m_cTempFanCnt = 0;
				ErrorMsg((int) OK, MSG_FAN_TEMP_OK);
			}
			else
				m_cTempFanCnt = 0;
			m_nStatusNo    = 8;	
			break;
		case 8:					
			if (m_cOperationMode == AUTO)
				m_nTempccdUV  = 0;
			else
			{
				if (m_bSingleChannel)
				{	
					m_nTempccdVIS = VIS_CCD_TEMP_OK;
				}
				if (m_cStartupMode > HOT3)
				{
					if (m_nTempccdVIS < m_nTempVISMin)
								m_nTempVISMin = m_nTempccdVIS;
					if (m_nTempccdVIS < VIS_CCD_TEMP_MIN)			//  19 (-55)
					{
						if (!(VIS_CCD_FLG & m_nFatal))
						{
							if (++m_cTempccdVISCnt > 4)
							//..............................................
							// THN Jan 2007 ... Comment following lint to remove the too cold fatal test
							ErrorMsg((int) YES, ERR_VIS_CCD_MIN);
							//..............................................
						}
					}
					else if (m_nTempccdVIS > VIS_CCD_TEMP_HIGH)	// 107 (-10)
					{
							if (m_nTempccdVIS > m_nTempVISLast)
								++m_cVISflag;
							else if (m_nTempccdVIS < m_nTempVISLast)
								m_cVISflag = 0;
							if (m_cVISflag > 3)
							{
								if (!(VIS_CCD_FLG & m_nFatal))
									ErrorMsg((int) YES, ERR_VIS_CCD_HEATING);
							}
							if ( (m_nTempccdVIS - m_nTempVISMin) > 20) 	// ~ 10 degrees
							{
								if (!(VIS_CCD_FLG & m_nFatal))
									ErrorMsg((int) YES, ERR_VIS_CCD_HEATING);
							}
						if (m_nTempccdVIS > VIS_CCD_TEMP_MAX)		// 107 (-10)
						{
							if (!(VIS_CCD_FLG & m_nFatal))
							{
								if (++m_cTempccdVISCnt > 4)
									ErrorMsg((int) YES, ERR_VIS_CCD_MAX);
							}
						}
						else
						{
							if (!(VIS_CCD_FLG & m_nWarning))
							{
								if (++m_cTempccdVISCnt > 4)
									ErrorMsg((int) NO, WARN_VIS_CCD_HIGH);
							}
						}
					}
					else if ((VIS_CCD_FLG & m_nWarning) &&
						(m_nTempccdVIS < VIS_CCD_TEMP_HIGH_OK))
					{
						m_cTempccdVISCnt = 0;
						ErrorMsg((int) OK, MSG_VIS_CCD_OK);
					}
					else if ((m_cOperationMode == DIAG) && (VIS_CCD_FLG & m_nFatal)
						&& (m_nTempccdVIS < VIS_CCD_TEMP_HIGH_OK))
					{
						m_cTempccdVISCnt = 0;
						ErrorMsg((int) OK, MSG_VIS_CCD_OK);
					}
					else
						m_cTempccdVISCnt = 0;
					m_nTempVISLast = m_nTempccdVIS;
				}
			}
			m_nStatusNo    = 1;	
			break;
		default:				
			m_nStatusNo = 1;		
			break;
	}
//theApp.LeaveCriticalSection1(&m_CriticalSection);
}
//===========================================================================

void CParserThread::GetStatusFatal()
{
	if (m_nLineCount < 65000)
		 m_nLineCount += 1;
	else
		 m_nLineCount  = 0;

//theApp.EnterCriticalSection1(&m_CriticalSection);

	switch (m_nStatusNo)
	{
		case 1:					
			m_nStatusNo = 2;		
			break;
		case 2:					
			m_nStatusNo = 3;		
			break;
		case 3:					
			m_nStatusNo  = 4;		
			break;
		case 4:
			m_nStatusNo = 5;		
			break;
		case 5:					
			m_nStatusNo = 6;		
			break;
		case 6:					
			// do temperature stuff
			check_temps();
			m_nStatusNo   = 7;		
			break;
		case 7:					
			m_nStatusNo    = 8;	
			break;
		case 8:					
			m_nStatusNo = 1;		
			break;
		default:				
			m_nStatusNo = 1;		
			break;
	}
//theApp.LeaveCriticalSection1(&m_CriticalSection);
}
//===========================================================================

/*** Revision History ***

11/22/16    JO      CBF-78 - Add "Lid Off mode" functionality.


$Log: /IcarusBased/SpecFW/Getstat2.cpp $
 * 
 * 19    2/01/07 4:48p Nashth
 * A few modifications from prior update.
 * 
 * 16    1/19/07 3:41a Nashth
 * Redacted removal of fatal mode checks for detector temperature Too Low,
 * and left comments in place for easy reversal should that be required.
 * 
 * 15    1/17/07 4:59p Nashth
 * Cooler minimum check also removed for VIS camera in addition to UV
 * camera.
 * 
 * 14    1/16/07 5:21p Nashth
 * Detector Too Cold check removed and only detector cooler temperature
 * read from mux.
 * 
 * 13    12/19/06 2:49p Nashth
 * Initialized minimum detector temperature variables to 255, and fixed a
 * number of constraints for determining the minimum value and entering
 * Fatal mode for detector tempareatures as per QPR: CANU6WMPKS.
 * 
 * 12    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 11    7/07/05 10:31 Frazzitl
 * Another fix
 * 
 * 10    6/29/05 16:21 Frazzitl
 * Implemented Status checking
 * 
 * 9     6/17/05 2:52p Frazzitl
 * Corrected the temperature equation constants and the status comparisons
 * 
 * 8     6/17/05 10:14a Frazzitl
 * Corrected comparisons because of changes in the equation for
 * determining the Ambient temperatures.
 * 
 * 7     6/06/05 12:38p Frazzitl
 * Fixed problem with Water Leak signal - High indicates NO Leak
 * 
 * 6     5/26/05 9:12a Frazzitl
 * Added changes recommended by Remco to fix crashing problem
 * 
 * 5     4/28/05 14:16 Frazzitl
 * Getting basic firmware to run
 * 
 * 4     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 3     4/19/05 4:44p Frazzitl
 * Fixed reading A/D values and corrected Water Leak display
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/28/05 9:49 Frazzitl
 * Missing files

$NoKeywords: $

** End of Rev History **/
