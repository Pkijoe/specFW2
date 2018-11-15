//===========================================================================
//
//	Module Name:	StatusThread.cpp
//
// 	Function:		This routine checks the status of one of the Spectrometer's
//					three threads:  Instrument Status
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "Debug.h"
#include "Heat.h"

WORD	wInterpolation_bits;
short	tMUX = 0;

//===========================================================================
//	Function:	1.	Runs continually in order to monitor the spectrometers
//					operating parameters (voltages & temperatures)
//				2.	Calls the proper GetStatus routine that determines if the
//					operating parameters are within acceptable limits
//				3.	Allows the output of Tub temperature to facilitate debugging
//	Return:		No actual return except when exiting threat during app closure
//===========================================================================
static int nDebugCntUVCoolerVDC = {0};
UINT WINAPI CParserThread::statusThread(LPVOID pParam)
{
	CParserThread *pThis = (CParserThread *) pParam;

	short	nChannel = 0;
	WORD	wChannelData, sum;
	short	nCheckCount = 0;
    int loop;
//unsigned short temp;
	
	for ( ; ; )
	{
		Sleep(25);

theApp.EnterCriticalSection1(&pThis->m_CriticalSection);

		// Special debugging to check timing of each thread
		if (pThis->m_nDebug & DEBUG_THREADS)
		{
			pThis->StoreThreadData(1);
		}

		// The channel is called twice because of the particular operating mode of the ADC
		// TLF - Last variable used to indicate whether to change Mux address after read of channel 3
		//		 The flag is basically ignored for any other channel
		//		 When the flag is true, the mux address is changed to the next address to allow the
		//		 mux time to respond to the change in address - it responds slow so it needs extra time


		if (nChannel == 2)   // Average 16 of the four coarse channel temps - each has 12 bits
		{
		  
//  temp = pThis->m_IO.InputW(DIAG_REG);
//  temp |= TP19_MASK;               // TP19 = 1
//  pThis->m_IO.OutputW(DIAG_REG, temp);	
		
		  sum = 0;
		  for (loop = 0; loop <= 15; loop++)
		    {
		       wChannelData = pThis->readADC(pThis->m_nDebug, nChannel, 0, false);
		       pThis->Suspend();  // ~40us delay

		       wChannelData = pThis->readADC(pThis->m_nDebug, nChannel, 0, true);
	   
//if (tMUX == 1)
//{
//  temp = pThis->m_IO.InputW(DIAG_REG);
//  temp &= ~TP19_MASK;               // TP19 = 0
//  pThis->m_IO.OutputW(DIAG_REG, temp);
//}

			   sum = (WORD) (sum + wChannelData);  // Sum the 12 bit temperature data
			}
			
		  wChannelData = (WORD)( sum >> 4);   				    	// Divide by 16
		  wInterpolation_bits = (WORD) (wChannelData & 0x0003);     // Keep just the lowest two bits.
          wChannelData >>= 2;									    // Use the 10 bit data on coarse temps channel; 
		}
		else  // Process as usual without averaging
		{
		  wChannelData = pThis->readADC(pThis->m_nDebug, nChannel, 0, false);
		  pThis->Suspend();
		  wChannelData = pThis->readADC(pThis->m_nDebug, nChannel, 0, true);
		}

//------------------------------------------------------------------------------------  CBF-37 End

		// Fixed switch statement per QPRs SITR6BVQ8X & SITR6BVQDX
		switch( nChannel )
		{
			case 0:		// +24 Vdc
				pThis->m_nPS24vdc = pThis->Average(wChannelData, pThis->m_nPS24vdc);
			break;
			case 1:		// Ambient Inside (FAN)
				pThis->m_nTempFan = pThis->Average(wChannelData, pThis->m_nTempFan);
			break;
			case 2:		// Coarse (Primary, Secondary)
				switch( tMUX)
				{
					case 0:	pThis->m_TempTopPri			= pThis->Average(wChannelData, pThis->m_TempTopPri);
							pThis->m_InterpolateTopPri	= wInterpolation_bits;
					break;
					case 1:	pThis-> m_TempBotPri		= pThis->Average(wChannelData, pThis-> m_TempBotPri);
							pThis->m_InterpolateBotPri	= wInterpolation_bits;
					break;
					case 2:	pThis->m_TempTopSec			= pThis->Average(wChannelData, pThis->m_TempTopSec);
							pThis->m_InterpolateTopSec	= wInterpolation_bits;
					break;
					case 3:	pThis->m_TempBotSec			= pThis->Average(wChannelData, pThis->m_TempBotSec);
							pThis->m_InterpolateBotSec	= wInterpolation_bits;
					break;
				}
			break;
			case 3:		// Currently unused
			break;
			case 4:		// Ambient Outside (?)
				pThis->m_nTempAmbient = pThis->Average(wChannelData, pThis->m_nTempAmbient);
			break;
			case 5:		// VIS TE Cooloer Vdc
				pThis->m_nVIScooler = pThis->Average(wChannelData, pThis->m_nVIScooler);
			break;
			case 6:		// UV TE Cooler Vdc
				pThis->m_nUVcooler = pThis->Average(wChannelData, pThis->m_nUVcooler);
			break;
			case 7:		// Ground
				pThis->m_nGround = pThis->Average(wChannelData, pThis->m_nGround);
			break;
			default:
				nChannel = 0;
			break;
		}

		// Increment after each read
		nChannel++;

		// Increment counter that holds off GetStatus() until real data available
		if (nCheckCount < 256)
			nCheckCount++;

		if (nChannel > 7)
		{
			nChannel = 0;
		}

		// Make sure data has been read enough times before allowing call to GetStatus()
		// so call to check_temps() has real data to work with.
		if (nCheckCount >= 256)
		{
			if (pThis->m_cInitFlag == NO)
				pThis->GetStatus();
			else if (pThis->m_cInitFlag == YES)
				pThis->GetStatusInit();
			else
				pThis->GetStatusFatal();
		}
theApp.LeaveCriticalSection1(&pThis->m_CriticalSection);
	}
}
//===========================================================================

//===========================================================================
//	Function:	Reads the spectrometer ADC channel requested by the
//				status thread
//	Return:		The requested ADC channel output equivalent of the desired
//				temperature or voltage
//===========================================================================
WORD CParserThread::readADC(WORD  wDebug,	// debug flags
							short nChannel,	// 8 position Mux channel 
							short dummy,	// No longer used
							bool bChange)	// Real read, advance to next Mux position
{
	WORD	wChannelCmd;
	WORD	wMUXAddr(0); 
	WORD	wChannelData(0);

	// Channel Command
	// 100x xx11 0011 0000, where xxx is the channel address
	// 1000 0011 0011 0000 - 0x8330 - Channel 0
	// 1000 0111 0011 0000 - 0x8730 - Channel 1
	// 1000 1011 0011 0000 - 0x8b30 - Channel 2
	// 1000 1111 0011 0000 - 0x8f30 - Channel 3
	// 1001 0011 0011 0000 - 0x9330 - Channel 4
	// 1001 0111 0011 0000 - 0x9730 - Channel 5
	// 1001 1011 0011 0000 - 0x9b30 - Channel 6
	// 1001 1111 0011 0000 - 0x9f30 - Channel 7

	switch( nChannel )
	{
		case 0:		wChannelCmd = 0x8330;				// +24 Vdc
		break;
		case 1:		wChannelCmd = 0x8730;				// Ambient Inside (FAN)
		break;
		case 2:		wChannelCmd = 0x8b30;				// Coarse temperature channels
		break;
		case 3:		wChannelCmd = 0x8f30;				// Currently unused
		break;
		case 4:		wChannelCmd = 0x9330;				// Ambient Outside (?)
		break;
		case 5:		wChannelCmd = 0x9730;				// UV TE Cooler Vdc
		break;
		case 6:		wChannelCmd = 0x9b30;				// VIS TE Cooler Vdc
		break;
		case 7:		wChannelCmd = 0x9f30;				// Ground
		break;
		default:	wChannelCmd = 0x8330;				// +24 Vdc
		break;
	}
	m_IO.OutputW(ADC_COMMAND_REG, wChannelCmd);			// Select MUX Channel
	Suspend();
	wChannelData = m_IO.InputW(ADC_DATA_REG);
	wChannelData &= 0x0fff;		    	// Keep only the lower 12 bits
	
	if (nChannel != 2)
	  {
		 wChannelData >>= 4;		    // Keep only the upper 8 bits
	  }
	  
	// Change mux position to the next input. Doing it here will give it time to stabilize.
	if (nChannel == 3 && bChange == true)
	{
   		tMUX++;
		if (tMUX > 3) tMUX = 0;
		switch( tMUX )
		{
			case 0:		wMUXAddr = ADC_MULTIPLEXER_TOP_PRI;		// Top Primary
			break;                                
			case 1:		wMUXAddr = ADC_MULTIPLEXER_BOT_PRI;		// Bottom Primary
			break;                                
			case 2:		wMUXAddr = ADC_MULTIPLEXER_TOP_SEC;		// Top Secondary
			break;                                
			case 3:		wMUXAddr = ADC_MULTIPLEXER_BOT_SEC;		// Bottom Seconary
			break;
		}
		
		m_IO.OutputW(wMUXAddr, 0);						// Select MUX Channel
	}

	return wChannelData;
}
//===========================================================================

//===========================================================================
//	Function:	Reads the specified detector ADC channel requested by the
//				UV or VIS status thread.
//
//	Return:		0 for AOK; Else 1 for unable to read SPI status register Done bit	
//
//  Note:		m_nNoStatusCount incremented on every failed read (any channel)
//              m_nNoStatusCount reset to zero on any good read (any channel) 
//
//  Arguments:  nDetector: UV for UV detector; else VIS detector
//              nChannel:  0 through 7 for the respective MUX channel 
//                          (See comments below for details)
//              wDataVal : The requested channel output equivalent of the desired
//						   temperature or voltage
//===========================================================================
WORD CParserThread::readDetectorADC(short nDetector, short nChannel, WORD &wDataVal)
{
	WORD	wTemp;
	WORD	wDataHi;
	WORD	wChannelCmd;
	int		wChannelData(0);
	DWORD	dwBegin;

	switch( nChannel )
	{
		case 0:		wChannelCmd = 0x00;		// Die Temperature
		break;
		case 1:		wChannelCmd = 0x40;		// +5 Vdc
		break;
		case 2:		wChannelCmd = 0x10;		// +Vah
		break;
		case 3:		wChannelCmd = 0x50;		// +12 Vdc
		break;
		case 4:		wChannelCmd = 0x20;		// +15 Vdc
		break;
		case 5:		wChannelCmd = 0x60;		// -15 Vdc
		break;
		case 6:		wChannelCmd = 0x30;		// -Vah
		break;
		case 7:		wChannelCmd = 0x70;		// -5 Vdc
		break;
		default:
		break;
	}
	wChannelCmd |= 0x8f;

	if (nDetector == UV)
	{
		m_CameraIO.UVOutputW(DIAG_ADC_ENABLE_UV, 1);				// Chip select
		m_CameraIO.UVOutputW(ADC_TRANSMIT_REG_UV, wChannelCmd);		// Send command

		dwBegin = ::GetTickCount();
		do
		{
			wTemp = m_CameraIO.UVInputW(ADC_STATUS_REG_UV);
			wTemp = m_IO.InputW(ARRAY_DATA_LATCH_UV);
			if (!continueWait(dwBegin, 100))
			{
				m_nNoStatusCount++;
				return 1;
			}
		} while (!(wTemp & 0x80));									// Check bit 7

		wTemp = m_CameraIO.UVInputW(ADC_RECEIVE_REG_UV);			// Dummy read
		m_CameraIO.UVOutputW(ADC_TRANSMIT_REG_UV, 0);

		dwBegin = ::GetTickCount();
		do
		{
			wTemp = m_CameraIO.UVInputW(ADC_STATUS_REG_UV);
			wTemp = m_IO.InputW(ARRAY_DATA_LATCH_UV);
			if (!continueWait(dwBegin, 100))
			{
				m_nNoStatusCount++;
				return 1;
			}
		} while (!(wTemp & 0x80));									// Check bit 7

		wTemp = m_CameraIO.UVInputW(ADC_RECEIVE_REG_UV);			// Dummy read
		wDataHi = m_IO.InputW(ARRAY_DATA_LATCH_UV);
		m_CameraIO.UVOutputW(ADC_TRANSMIT_REG_UV, 0);

		dwBegin = ::GetTickCount();
		do
		{
			wTemp = m_CameraIO.UVInputW(ADC_STATUS_REG_UV);
			wTemp = m_IO.InputW(ARRAY_DATA_LATCH_UV);
			if (!continueWait(dwBegin, 100))
			{
				m_nNoStatusCount++;
				return 1;
			}
		} while (!(wTemp & 0x80));									// Check bit 7

		wTemp = m_CameraIO.UVInputW(ADC_RECEIVE_REG_UV);			// Dummy read
		wChannelData = m_IO.InputW(ARRAY_DATA_LATCH_UV);
		m_CameraIO.UVOutputW(DIAG_ADC_ENABLE_UV, 0);				// Chip de-select
	}
	else	// nDetector == VIS
	{
		m_CameraIO.VISOutputW(DIAG_ADC_ENABLE_VIS, 1);				// Chip select
		m_CameraIO.VISOutputW(ADC_TRANSMIT_REG_VIS, wChannelCmd);	// Send command

		dwBegin = ::GetTickCount();
		do
		{
			wTemp = m_CameraIO.VISInputW(ADC_STATUS_REG_VIS);
			wTemp = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
			if (!continueWait(dwBegin, 100))
			{
				m_nNoStatusCount++;
				return 1;
			}
		} while (!(wTemp & 0x80));									// Check bit 7

		wTemp = m_CameraIO.VISInputW(ADC_RECEIVE_REG_VIS);			// Dummy read
		m_CameraIO.VISOutputW(ADC_TRANSMIT_REG_VIS, 0);

		dwBegin = ::GetTickCount();
		do
		{
			wTemp = m_CameraIO.VISInputW(ADC_STATUS_REG_VIS);
			wTemp = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
			if (!continueWait(dwBegin, 100))
			{
				m_nNoStatusCount++;
				return 1;
			}
		} while (!(wTemp & 0x80));									// Check bit 7

		wTemp = m_CameraIO.VISInputW(ADC_RECEIVE_REG_VIS);			// Dummy read
		wDataHi = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
		m_CameraIO.VISOutputW(ADC_TRANSMIT_REG_VIS, 0);

		dwBegin = ::GetTickCount();
		do
		{
			wTemp = m_CameraIO.VISInputW(ADC_STATUS_REG_VIS);
			wTemp = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
			if (!continueWait(dwBegin, 100))
			{
				m_nNoStatusCount++;
				return 1;
			}
		} while (!(wTemp & 0x80));									// Check bit 7

		wTemp = m_CameraIO.VISInputW(ADC_RECEIVE_REG_VIS);			// Dummy read
		wChannelData = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
		m_CameraIO.VISOutputW(DIAG_ADC_ENABLE_VIS, 0);				// Chip de-select
	}

	wDataHi      <<= 2;												// Left Shift 2
	wChannelData >>= 6;												// Right Shift 6
	wChannelData  += wDataHi;										// Sum results together

	wDataVal		 = (WORD) wChannelData;
	m_nNoStatusCount = 0;											// Reset error counter

	return 0; // AOK return
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

08/16/16    JO      CBF-37 - Modify code for using 12 bit AD converter for temperature sensors.
10/12/16    JO      CBF-54 - Change heater related names/definitions, fix broken heater commands.
11/21/16    JO      CBF-84 - Fix channel 2 mux assignments in function readADC.
11/28/16    JO 		CBF-88 - Change mux timing to allow maximum settling time before next read.


$Log: /IcarusBased/SpecFW/StatusThread.cpp $
 * 
 * 55    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 54    1/10/07 1:15p Nashth
 * Utilized unused Fatal mode error codes U4 and V4  for UV and VIS
 * detector temperature readout errors as per QPR: CANU6WMPKS.
 * 
 * 53    2/22/06 2:13p Frazzitl
 * Changed the debugging of the thread to use a circular buffer in BRAM.
 * 
 * 52    2/22/06 3:01p Nashth
 * Slight correction to Mux Temp Debug.
 * 
 * 51    2/22/06 2:30p Nashth
 * Added debug in ReadADC for Temperature MUX readings.
 * 
 * 50    2/22/06 11:54a Frazzitl
 * Fixed formatting slightly
 * 
 * 49    2/22/06 8:33a Frazzitl
 * Fixed problem of printing CStrings using a printf statement.
 * 
 * 48    2/21/06 2:11p Frazzitl
 * Added debug code to display access to each primary thread as well as
 * the stopping of the watchdog.
 * 
 * 47    2/13/06 8:58a Frazzitl
 * Fixed possible problem reading temperature values during status thread
 * 
 * 46    1/04/06 10:02a Frazzitl
 * Added comments as required by QPR #GEPR6C2SHC
 * 
 * 45    12/21/05 10:23a Frazzitl
 * Fixed problem with Tub temperature & read reset
 * 
 * 44    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 43    11/14/05 1:41p Frazzitl
 * Added function to average A/D readings for smoother data.  Also added a
 * delay to be sure data is real before used in GetStatus() routines.
 * 
 * 42    11/11/05 8:25a Frazzitl
 * Added special clock critical section around all clock functions.
 * 
 * 41    11/10/05 2:38p Frazzitl
 * Corrected problem with time output and added date & time output to
 * HyperTerminal.
 * 
 * 40    9/21/05 8:04a Frazzitl
 * Fixed switch statement per QPRs SITR6BVQ8X & SITR6BVQDX
 * 
 * 39    9/20/05 4:02p Nashth
 * Added THNJ6FLJN3 to comment on line below that was previously omitted.
 * 
 * 38    8/24/05 8:49a Nashth
 * Most recent updates for hwinit.  Fix for THNJ6FLJN3 was made.
 * 
 * 37    8/10/05 11:41 Frazzitl
 * Removed warning message during compile
 * 
 * 36    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 35    7/21/05 9:00 Frazzitl
 * Changed operation performed in default section of a switch statement.
 * 
 * 34    7/07/05 10:31 Frazzitl
 * Another fix
 * 
 * 33    7/06/05 16:35 Frazzitl
 * Corrected minor problems
 * 
 * 32    6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 31    6/17/05 10:00a Frazzitl
 * Removed debug logging now that everything is working properly.
 * 
 * 30    6/07/05 8:25a Frazzitl
 * Separated camera I/O into UV and VIS routines to accommodate individual
 * status checking
 * 
 * 29    6/06/05 3:36p Frazzitl
 * Removed warnings during build
 * 
 * 28    5/26/05 4:55p Frazzitl
 * Added some debugging if crashes continue - will later remove
 * 
 * 27    5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 26    5/26/05 9:12a Frazzitl
 * Added changes recommended by Remco to fix crashing problem
 * 
 * 25    5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 24    5/24/05 17:13 Frazzitl
 * WaitForSingleObject did not work as thought - caused slow response
 * 
 * 23    5/24/05 3:46p Frazzitl
 * Corrected exit from threads and added the elimination of the VIS status
 * thread if single channel instrument.
 * 
 * 22    5/23/05 4:59p Frazzitl
 * Attempting to speed up command response by haing Status threads exit
 * quickly if a command is pending
 * 
 * 21    5/23/05 16:38 Frazzitl
 * Added code to eliminate hangups
 * 
 * 20    5/13/05 13:29 Frazzitl
 * Split status threads into separate files
 * 
 * 19    5/13/05 10:35 Frazzitl
 * Monitoring Sarnoff A/D
 * 
 * 18    5/13/05 8:30 Frazzitl
 * Added ability to see Sarnoff A/D output
 * 
 * 17    5/13/05 9:05 Frazzitl
 * Added ability to read detector temperature
 * 
 * 16    5/11/05 11:58 Frazzitl
 * Added access to GetStatus which is needed to control tub temperature
 * 
 * 15    5/10/05 11:59 Frazzitl
 * Fixed problem with Status thread & made more readable
 * 
 * 14    5/06/05 10:50 Frazzitl
 * Trying to fix problem of no delay between IO reads and writes
 * 
 * 13    4/29/05 12:49 Frazzitl
 * Trying to get the threat to work properly
 * 
 * 12    4/29/05 10:29 Frazzitl
 * Changing implementation of threads
 * 
 * 11    4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 10    4/26/05 15:49 Frazzitl
 * Working on controlling the tub temperature
 * 
 * 9     4/22/05 15:18 Frazzitl
 * Improved Status reads
 * 
 * 8     4/20/05 15:49 Frazzitl
 * 
 * 7     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 6     4/19/05 4:44p Frazzitl
 * Fixed reading A/D values and corrected Water Leak display
 * 
 * 5     4/15/05 14:13 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 4     4/13/05 14:48 Frazzitl
 * Made more readable
 * 
 * 3     4/13/05 13:16 Frazzitl
 * Miscellaneous changes primarily for status processing
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:24 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
