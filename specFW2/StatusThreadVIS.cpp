//===========================================================================
//
//	Module Name:	StatusThreadVIS.cpp
//
// 	Function:		This routine checks the status of the VIS Detector electronics
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/StatusThreadVIS.cpp 33    6/07/07 2:53p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "Debug.h"

/////////////////////////////////////////////////////////////////////////////
// statusThreadVIS

UINT WINAPI CParserThread::statusThreadVIS(LPVOID pParam)
{
	CParserThread *pThis = (CParserThread *) pParam;

	short	nChannel(0);
	short	nCount(0);
	WORD	wChannelData;
	WORD	wErrReading(0);

	for ( ; ; )
	{
		Sleep(50);
		
		theApp.EnterCriticalSection1(&pThis->m_CriticalSection);
		//..........................................................
		//  If single camera then continue the loop and 
		//  go back to sleep. 
		//
		if ( pThis->m_bSingleChannel == true )
		{
			theApp.LeaveCriticalSection1(&pThis->m_CriticalSection);
			continue;
		}
		//..........................................................

		// Special debugging to check timing of each thread
		if (pThis->m_nDebug & DEBUG_THREADS)
		{
			pThis->StoreThreadData(3);
		}
		//......................................................................
		// Only read channel zero ... all other channels are not used
		if ( nChannel == 0 )
		wErrReading = pThis->readDetectorADC(VIS, nChannel, wChannelData);
		else wChannelData = 0;
		//......................................................................

		switch( nChannel )
		{
			case 0:
				if ( wErrReading )
				{
					// Trigger a Fatal V4 error
					if (!(VIS_COOLER_FLG & pThis->m_nFatal))
						pThis->ErrorMsg( ( int ) YES, ERR_VIS_COOLER_READOUT );
				}
				else
				{
					if (wChannelData >= 0 && wChannelData <=255)
					{
						//..........................................................................
						// THN Jan 2007 ... Fix for Camera off at Connection 
						//  If the camera has shutdown, and the camera was on
						pThis->m_wTempccdRawVIS = wChannelData;

						if ( (wChannelData == 0) && pThis->m_bCCDPower )
						{
							//........................................................
							//    Record warning message 
							if ( ! pThis->m_bTurnCameraBackOnVIS )
									pThis->ErrorMsg( ( int ) NO, ERR_CAMERA_OFF_VIS );
							//........................................................
							printf("VIS trigger\r\n");
							pThis->m_bTurnCameraBackOnVIS = true;  // Signal restart camera and watchdog
						}
						// Only average data when Not in turn back on state or data is not zero
						else if ( (! pThis->m_bTurnCameraBackOnVIS) || (wChannelData != 0) )
						{
							//printf("VIS Average\r\n");
							pThis->m_nTempccdVIS	= pThis->Average(wChannelData, pThis->m_nTempccdVIS);	// Die Temperature
						}
						else
						{
							//printf("VIS No Average\r\n");
						}
						//..........................................................................
					}

					if ( (pThis->m_nDebug & DEBUG_DETECTOR) && (pThis->m_nTempccdVIS < VIS_CCD_TEMP_MIN) )
					{
						char nBuffer[9];
						pThis->GetTimeStamp(&nBuffer[0]);
						printf("VIS Detector: %8s  Value: %d, ValueX: %3d, Counter: %d, Fatal: %d\r\n", nBuffer, wChannelData, 
							                       pThis->m_nTempccdVIS, pThis->m_cTempccdVISCnt, VIS_CCD_FLG & pThis->m_nFatal);
					}
				}
			break;
			case 1:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_n5vdcPosVIS	= wChannelData;		// +5 Vdc
			break;
			case 2:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_nVahPosVIS		= wChannelData;		// +Vah
			break;
			case 3:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_n12vdcPosVIS	= wChannelData;		// +12 Vdc
			break;
			case 4:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_n15vdcPosVIS	= wChannelData;		// +15 Vdc
			break;
			case 5:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_n15vdcNegVIS	= wChannelData;		// -15 Vdc
			break;
			case 6:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_nVahNegVIS		= wChannelData;		// -Vah
			break;
			case 7:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_n5vdcNegVIS	= wChannelData;		// -5 Vdc
			break;
			default:
				if (wChannelData >= 0 && wChannelData <=255)
					pThis->m_n5vdcNegVIS	= wChannelData;		// -5 Vdc
			break;
		}

		nChannel++;
		if (nChannel > 7)
		{
			nChannel = 0;

			if (pThis->m_nDebug & DEBUG_DETECTOR)
			{
				if (nCount >= 80)		// 10 seconds, 480 for 60 seconds
				{
					char	nBuffer[9];
					pThis->GetTimeStamp(&nBuffer[0]); //Get time into nBuffer
					double dCcdTemp = (double) pThis->GetTECoolerTemperature( pThis->m_nTempccdVIS) / 100.0;
					printf("VIS Detector: %8s - %.3d => %.1f\r\n", nBuffer, pThis->m_nTempccdVIS, dCcdTemp);
					nCount = 0;
				}
				else
				{
					nCount++;
				}
			}
		}
		theApp.LeaveCriticalSection1(&pThis->m_CriticalSection);
	}
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /IcarusBased/SpecFW/StatusThreadVIS.cpp $
 * 
 * 33    6/07/07 2:53p Nashth
 * Added a warning message should either Camera shutdown as per QPR:
 * THNJ73XQ2S.
 * 
 * 32    3/07/07 4:17p Nashth
 * Attempt of improving the fix to QPR: THNJ6XXKR5...
 * Should the camera turn itself off at ANY time, then the
 * firmware will turn it back on.
 * 
 * 31    2/01/07 4:48p Nashth
 * A few modifications from prior update.
 * 
 * 30    2/01/07 4:15p Nashth
 * A few modifications for prior update.
 * 
 * 29    2/01/07 2:56p Nashth
 * Second pass at keeping the camera on during a 'long' connection as per
 * QPR: THNJ6XXLK3.
 * 
 * 28    1/30/07 4:08p Nashth
 * First pass at keeping the camera on during a 'long' connection as per
 * QPR: THNJ6XXLK3.
 * 
 * 27    1/16/07 5:21p Nashth
 * Detector Too Cold check removed and only detector cooler temperature
 * read from mux.
 * 
 * 26    1/10/07 1:15p Nashth
 * Utilized unused Fatal mode error codes U4 and V4  for UV and VIS
 * detector temperature readout errors as per QPR: CANU6WMPKS.
 * 
 * 25    1/02/07 5:09p Nashth
 * Additional debug displays only when count value is less than 19.
 * 
 * 24    1/02/07 4:48p Nashth
 * Added more Detector temperature debug.
 * 
 * 23    2/22/06 2:13p Frazzitl
 * Changed the debugging of the thread to use a circular buffer in BRAM.
 * 
 * 22    2/22/06 11:55a Frazzitl
 * Fixed formatting slightly
 * 
 * 21    2/22/06 8:34a Frazzitl
 * Fixed problem of printing CStrings using a printf statement.
 * 
 * 20    2/21/06 2:11p Frazzitl
 * Added debug code to display access to each primary thread as well as
 * the stopping of the watchdog.
 * 
 * 19    1/13/06 1:38p Frazzitl
 * Removed dead code as recommended in a QPR
 * 
 * 18    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 17    11/14/05 1:41p Frazzitl
 * Added function to average A/D readings for smoother data.  Also added a
 * delay to be sure data is real before used in GetStatus() routines.
 * 
 * 16    11/11/05 8:25a Frazzitl
 * Added special clock critical section around all clock functions.
 * 
 * 15    11/10/05 2:38p Frazzitl
 * Corrected problem with time output and added date & time output to
 * HyperTerminal.
 * 
 * 14    8/02/05 2:42p Nashth
 * VIS status thread enabled and controlled via cmdsc. for QPR THNJ6EVKJC.
 * 
 * 13    7/22/05 8:42 Frazzitl
 * Removed references to Die Temp - changed to ccd Temp
 * 
 * 12    7/12/05 17:12 Frazzitl
 * minor fix
 * 
 * 11    7/06/05 16:35 Frazzitl
 * Corrected minor problems
 * 
 * 10    6/17/05 10:00a Frazzitl
 * Removed debug logging now that everything is working properly.
 * 
 * 9     5/26/05 4:55p Frazzitl
 * Added some debugging if crashes continue - will later remove
 * 
 * 8     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 7     5/26/05 9:12a Frazzitl
 * Added changes recommended by Remco to fix crashing problem
 * 
 * 6     5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 5     5/24/05 17:13 Frazzitl
 * WaitForSingleObject did not work as thought - caused slow response
 * 
 * 4     5/24/05 3:46p Frazzitl
 * Corrected exit from threads and added the elimination of the VIS status
 * thread if single channel instrument.
 * 
 * 3     5/23/05 4:59p Frazzitl
 * Attempting to speed up command response by haing Status threads exit
 * quickly if a command is pending
 * 
 * 2     5/23/05 12:59 Frazzitl
 * Minor change
 * 
 * 1     5/13/05 13:21 Frazzitl
 * Split status threads into separate files

$NoKeywords: $

** End of Rev History **/
