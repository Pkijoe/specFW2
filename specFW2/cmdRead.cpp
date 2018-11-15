//===========================================================================
//
//	Module Name:	cmdRead.cpp
//
// 	Function:		This routine is a C++ version of cmdRO assembler version.
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/cmdRead.cpp 62    7/10/07 4:05p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"
#include "debug.h"

#define	STATUS_NO_READ_ACQ			0x0008

unsigned int CParserThread::cmdRead(WORD nDelay)
{
	WORD	status(NO_ERRORS);
//	char	cDetector(UV);
	WORD	unTimingModeRegUV(0);					// Ramp Read Mode - For Testing
	WORD	unTimingModeRegVIS(0);					// Ramp Read Mode - For testing
	WORD	unArrayAddress(0);
	bool	bMoreArraysUV(true);
	bool	bMoreArraysVIS(true);
	bool	bLastArrayUV(false);
	bool	bLastArrayVIS(false);
	bool	bIntTime(true);
	short	nArray(0);
	short	nArrayUV(0);
	short	nArrayVIS(0);
	WORD	n;
	WORD	nPixels;
	WORD	wTemp;
	WORD	wTemp2;

	if (m_bRampMode == false)
	{
		//printf("THN cmdRead()... RampMode false\r\n");
		unTimingModeRegUV	= NORMAL_READ_MODE;		// Normal Read Mode - Not Ramp
		unTimingModeRegVIS	= NORMAL_READ_MODE;		// Normal Read Mode - Not Ramp
	}

	unTimingModeRegUV  |= CAMERA_READY;	// Not Software Reset
	unTimingModeRegVIS |= CAMERA_READY;	// Not Software Reset

	// Determine resolution of the detector
	if (m_cUVres == HIGH)
		unTimingModeRegUV	|= HIGH_RESOLUTION_MODE;	// High Res - UV
	if (m_cVISres == HIGH)
		unTimingModeRegVIS	|= HIGH_RESOLUTION_MODE;	// High Res - VIS

	// Determine operating mode
	if (m_bTestMode == true)
	{
		//printf("THN cmdRead()... Test Mode\r\n");
		unTimingModeRegUV  |= TEST_MODE;				// Test Mode
		unTimingModeRegVIS |= TEST_MODE;				// Test Mode

		if (m_bOverscanMode == true)
		{
			unTimingModeRegUV  |= OVERSCAN_MODE;		// Test Mode - Overscan
			unTimingModeRegVIS |= OVERSCAN_MODE;		// Test Mode - Overscan
			if (m_nArraysUV > 0)
				m_CameraIO.UVOutputW(DELAY_TIME_UV, 0);				// UV delay = 0
			if (m_nArraysVIS > 0)
				m_CameraIO.VISOutputW(DELAY_TIME_VIS, 0);			// VIS delay = 0
		}
		else											// Test Mode - Delay Mode
		{
			if (m_nArraysUV > 0)
				m_CameraIO.UVOutputW(DELAY_TIME_UV, nDelay);		// UV delay
			if (m_nArraysVIS > 0)
				m_CameraIO.VISOutputW(DELAY_TIME_VIS, nDelay);		// VIS delay
			bIntTime = false;
		}
	}
	else
	{
		if (m_nArraysUV > 0)
			m_CameraIO.UVOutputW(DELAY_TIME_UV, 0);					// UV delay = 0
		if (m_nArraysVIS > 0)
			m_CameraIO.VISOutputW(DELAY_TIME_VIS, 0);				// VIS delay = 0
	}

	// Send the setup mode
	if (m_nArraysUV > 0)
		m_CameraIO.UVOutputW(TIME_MODE_UV, unTimingModeRegUV);
	if (m_nArraysVIS > 0)
		m_CameraIO.VISOutputW(TIME_MODE_VIS, unTimingModeRegVIS);

	// Set the Integration Time
	if (bIntTime == true)
	{
		//printf("THN cmdRead()... Setting integration time\r\n");
		if (m_nArraysUV > 0)
			m_CameraIO.UVOutputW(INT_TIME_UV, m_nIntegrationTime);
		if (m_nArraysVIS > 0)
			m_CameraIO.VISOutputW(INT_TIME_VIS, m_nIntegrationTime);
	}
	else	// Delay mode - no Integration time - only 500 msec delay
	{
		//printf("THN cmdRead()... No integration time, set to zero \r\n");
		if (m_nArraysUV > 0)
			m_CameraIO.UVOutputW(INT_TIME_UV, 0);
		if (m_nArraysVIS > 0)
			m_CameraIO.VISOutputW(INT_TIME_VIS, 0);
	}

	// Set the subarray address to read and send Start Integration
	// If m_cOperationMode == AUTO, there will be only VIS subarrays
	if (m_nArraysUV)
	{
		//printf("THN cmdRead()... UV arrays\r\n");
		nArrayUV = 0;
		bMoreArraysUV = true;
		if (m_nArraysUV > 1)
		{	// More Arrays - set COMMAND_ARRAY_READ_BIT bit high
			unTimingModeRegUV |= COMMAND_ARRAY_READ_BIT;
			bLastArrayUV  = false;
		}
		else
		{	// One Array - make sure COMMAND_ARRAY_READ_BIT bit low
			unTimingModeRegUV &= ~COMMAND_ARRAY_READ_BIT;
			bLastArrayUV  = true;
		}
		m_CameraIO.UVOutputW(TIME_MODE_UV, unTimingModeRegUV);
		SmallDelay(); // needed for slower camera to be ready?
		unArrayAddress = m_ReadTableUV[0].array->address;
		m_CameraIO.UVOutputW(ARRAY_ADDRESS_UV, unArrayAddress);
	}
	else
		bMoreArraysUV = false;

	if (m_nArraysVIS)
	{
		//printf("THN cmdRead()... VIS arrays\r\n");
		nArrayVIS = 0;
		bMoreArraysVIS = true;
		if (m_nArraysVIS > 1)
		{	// More m_nArrays - set COMMAND_ARRAY_READ_BIT bit high
			unTimingModeRegVIS |= COMMAND_ARRAY_READ_BIT;
			bLastArrayVIS  = false;
		}
		else
		{	// One Array - make sure COMMAND_ARRAY_READ_BIT bit low
			unTimingModeRegVIS &= ~COMMAND_ARRAY_READ_BIT;
			bLastArrayVIS  = true;
		}
		m_CameraIO.VISOutputW(TIME_MODE_VIS, unTimingModeRegVIS);
		SmallDelay(); // needed for slower camera to be ready?
		unArrayAddress	= m_ReadTableVIS[0].array->address;
		m_CameraIO.VISOutputW(ARRAY_ADDRESS_VIS, unArrayAddress);
	}
	else
		bMoreArraysVIS = false;

// TLF - Eliminating cancel & abort from all read commands
//		 It seems to cause a lockup problem
//	if (m_bCancelFlag == true || m_bAbortFlag == true)
//	{
//		//printf("THN cmdRead()... Cancel or Abort 1\r\n");
//		// Never leave detector is RESET mode - put in READY mode
//		m_CameraIO.UVOutputW(TIME_MODE_UV, CAMERA_RESET);
//		if (m_bSingleChannel == false)
//			m_CameraIO.VISOutputW(TIME_MODE_VIS, CAMERA_RESET);
//		Sleep(1);
//		m_CameraIO.UVOutputW(TIME_MODE_UV, CAMERA_READY);
//		if (m_bSingleChannel == false)
//			m_CameraIO.VISOutputW(TIME_MODE_VIS, CAMERA_READY);
//		return CMD_CANCEL;
//	}

	// Send the Start Integration command - pulsing the bit
	if (m_nArraysUV > 0)
		m_CameraIO.UVOutputW( TIME_MODE_UV,  (WORD) (unTimingModeRegUV  |  COMMAND_START_INT_BIT));	// UV Start
	if (m_nArraysVIS > 0)
		m_CameraIO.VISOutputW(TIME_MODE_VIS, (WORD) (unTimingModeRegVIS |  COMMAND_START_INT_BIT));	// VIS Start
	SmallDelay();
	SmallDelay();
	if (m_nArraysUV > 0)
		m_CameraIO.UVOutputW( TIME_MODE_UV,  (WORD) (unTimingModeRegUV  & ~COMMAND_START_INT_BIT));	// UV Start
	if (m_nArraysVIS > 0)
		m_CameraIO.VISOutputW(TIME_MODE_VIS, (WORD) (unTimingModeRegVIS & ~COMMAND_START_INT_BIT));	// VIS Start

	// Get system a break if long Integration Time
	if (m_nIntegrationTime > 2)
		Sleep(m_nIntegrationTime - 2);

	//printf("THN cmdRead()... SuspendThreads()\r\n");
SuspendThreads();
	//printf("THN cmdRead()... SuspendThreads()... Post call\r\n");

// TLF - Initial read sequence method
// This should be much more efficient if the reads were done in separate threads
// This would only be necessary if there are both UV and VIS subarrays to read
// If only one type (UV or VIS), starting a thread might be slower
// Start reading the pixel data, one pixel at a time
	//.......................................................................................................
	// The following set true indicates what type of detector read error
	bool	bUVPixelReadError(false);
	bool	bUVReadError(false);
	bool	bUVStatusError(false);
	bool	bUVStatusErrorInt(false);
	bool	bUVAddressOverflow(false);
	bool	bUVDataOverflow(false);
	bool	bVISReadError(false);
	bool	bVISPixelReadError(false);
	bool	bVISStatusError(false);
	bool	bVISStatusErrorInt(false);
	bool	bVISAddressOverflow(false);
	bool	bVISDataOverflow(false);
	//........................................................................................................
	// The following must be set when a detector read error occurs
	// Capture first error per entry into this function
	WORD	wErrUVorVIS(0);				// 0=No error, 1=UV error, 2=VIS error 
	WORD	wErrSA(0);					// Error: Sub array number 
	WORD	wErrPixels(0);				// Error: Number of pixels
	WORD	wErrPixel(0);				// Error: Reading pixel number
	bool	bErrHiRes(false);			// Error: Low res readout mode = false, High res readout mode = true
	bool	bErrOverScan(false);		// Error: Reading overscan pixels
	bool	bErrExtraPixels(false);		// Error: Reading extra pixels
	bool	bErrBitSet(false);			// Error: Instantaneous setting when a camera register error bit is read
	//........................................................................................................
	short	addr;
	char	nBuffer[9];
	for (nArray = 0; nArray < m_nTotalArrays; )
	{
		// Pass UV Array if more available
		if (bMoreArraysUV == true)
		{	// Save subarray info if first time through only
			if (m_bFirstSum == true)
			{
				m_pArraysUV[nArrayUV]->m_nDetector = (BYTE) m_ReadTableUV[nArrayUV].array->detector;
				m_pArraysUV[nArrayUV]->m_nAddress  = (BYTE) m_ReadTableUV[nArrayUV].array->address;
				m_pArraysUV[nArrayUV]->m_nLocation = (BYTE) m_ReadTableUV[nArrayUV].array->location;
				if (m_bOverscanMode == true)
					m_pArraysUV[nArrayUV]->m_nPixels = (BYTE) (48 * (m_cUVres + 1));
				else
					m_pArraysUV[nArrayUV]->m_nPixels = (BYTE) (m_ReadTableUV[nArrayUV].array->pixels * (m_cUVres + 1));
				m_pArraysUV[nArrayUV]->m_nOrientation = (BYTE) m_ReadTableUV[nArrayUV].array->orient;
			}
			// Remember the number of whole pixels in subarray
			nPixels = m_ReadTableUV[nArrayUV].array->pixels;
			// Wait until some pixel data available
			int	nCnt(0);
			do
			{
				wTemp = m_IO.InputW(STATUS_REGISTER);
				if (++nCnt > 1000000)	// TLF - Increase from 65535
				{
					bUVReadError = true;
					bErrBitSet = GetRegErrorBitsUV(wTemp, bUVReadError, bUVStatusError,		bUVStatusErrorInt, 
																		bUVAddressOverflow,	bUVDataOverflow);
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	1;	//UV
						wErrSA			=	m_pArraysUV[nArrayUV]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	(m_cUVres == HIGH) ? true : false;
						bErrOverScan	=	false;
						bErrExtraPixels	=	false;
						wErrPixel		=	0;
					}
				}
				Sleep(0);	// Giving watchdog a chance if it needs it
			} while (!(wTemp & STATUS_DATA_READY_UV) && bUVReadError == false);
			// It's available - read everything there
			// If high res, read even pixels; if low res, read pixels and store in m_dwEvenData[n]
			nCnt = 0;
			for (n = 0; n < nPixels; n++)
			{
				if (wTemp & STATUS_DATA_READY_UV)
				{	// Check fo any Status error before reading data

					bErrBitSet = GetRegErrorBitsUV(wTemp, false, bUVStatusError,		bUVStatusErrorInt, 
																 bUVAddressOverflow,	bUVDataOverflow);
					// TLF - Now read data
					wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
					wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
					if (m_bFirstSum == true)
						m_pArraysUV[nArrayUV]->m_dwEvenData[n]  = wTemp2;
					else
						m_pArraysUV[nArrayUV]->m_dwEvenData[n] += wTemp2;
					wTemp = m_IO.InputW(STATUS_REGISTER);
				}
				else
				{
					// TLF - Don't set unread data to zero - read anyway later
//					m_pArraysUV[nArrayUV]->m_dwEvenData[n] = 0;
					++nCnt;
					if ( bUVReadError == false )
						bUVPixelReadError = true;
					bErrBitSet = GetRegErrorBitsUV(wTemp, bUVPixelReadError, bUVStatusError,		bUVStatusErrorInt, 
																			 bUVAddressOverflow,	bUVDataOverflow);
					// TLF - Now read data even without Data Ready
					wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
					wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
					if (m_bFirstSum == true)
						m_pArraysUV[nArrayUV]->m_dwEvenData[n]  = wTemp2;
					else
						m_pArraysUV[nArrayUV]->m_dwEvenData[n] += wTemp2;
					wTemp = m_IO.InputW(STATUS_REGISTER);
				}
				if ( bErrBitSet && (wErrUVorVIS == 0) )
				{
					wErrUVorVIS		=	1;	//UV
					wErrSA			=	m_pArraysUV[nArrayUV]->m_nAddress;
					wErrPixels		=	nPixels;
					bErrHiRes		=	(m_cUVres == HIGH) ? true : false;
					bErrOverScan	=	false;
					bErrExtraPixels	=	false;
					if ( bErrHiRes )
						wErrPixel = (WORD) (n * 2);	// High Res Even
					else
						wErrPixel		=	n;  // Low Res  
				}
			}
			if (nCnt > 0)
			{
				if (m_nDebug & DEBUG_MISC)
				{
					GetTimeStamp(&nBuffer[0]);

					addr = m_pArraysUV[nArrayUV]->m_nAddress;

					printf("%8s - Subarray: %d, Lost Pixels: %d\r\n", nBuffer, addr, nCnt);
				}
			}
			if (m_nDebug & DEBUG_DATA)
			{
				if (m_nDebug & DEBUG_MISC)
					printf("%.5d   %.5d   %.5d   %.5d   %.5d   %.5d   %.5d   %.5d\r\n", 
						m_pArraysUV[nArrayUV]->m_dwEvenData[0],
						m_pArraysUV[nArrayUV]->m_dwEvenData[1],
						m_pArraysUV[nArrayUV]->m_dwEvenData[2],
						m_pArraysUV[nArrayUV]->m_dwEvenData[3],
						m_pArraysUV[nArrayUV]->m_dwEvenData[4],
						m_pArraysUV[nArrayUV]->m_dwEvenData[5],
						m_pArraysUV[nArrayUV]->m_dwEvenData[6],
						m_pArraysUV[nArrayUV]->m_dwEvenData[7]	);
			}
			if (m_bOverscanMode == true)
			{
				for (n = nPixels; n < 48; n++)
				{
					if (wTemp & STATUS_DATA_READY_UV)
					{	// Check fo any Status error before reading data
						bErrBitSet = GetRegErrorBitsUV(wTemp, false,	bUVStatusError,		bUVStatusErrorInt, 
																		bUVAddressOverflow,	bUVDataOverflow);
						// TLF - Now read data
						wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
						if (m_bFirstSum == true)
							m_pArraysUV[nArrayUV]->m_dwEvenData[n]  = wTemp2;
						else
							m_pArraysUV[nArrayUV]->m_dwEvenData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					else
					{
						// TLF - Don't set unread data to zero - read anyway later
//						m_pArraysUV[nArrayUV]->m_dwEvenData[n] = 0;
						++nCnt;
						if ( bUVReadError == false )
							bUVPixelReadError = true;
						bErrBitSet = GetRegErrorBitsUV(wTemp, bUVPixelReadError,	bUVStatusError,		bUVStatusErrorInt, 
																					bUVAddressOverflow,	bUVDataOverflow);

						// TLF - Now read data even without Data Ready
						wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
						if (m_bFirstSum == true)
							m_pArraysUV[nArrayUV]->m_dwEvenData[n]  = wTemp2;
						else
							m_pArraysUV[nArrayUV]->m_dwEvenData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	1;	//UV
						wErrSA			=	m_pArraysUV[nArrayUV]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	(m_cUVres == HIGH) ? true : false;
						bErrOverScan	=	true;
						bErrExtraPixels	=	false;
						if ( bErrHiRes )
							wErrPixel = (WORD)(n * 2);		// High Res Even
						else
							wErrPixel		=	n;  // Low Res  
					}
				}
			}
			// If high res, now read odd pixels
			nCnt = 0;
			if (m_cUVres)
			{
				for (n = 0; n < nPixels; n++)
				{
					if (wTemp & STATUS_DATA_READY_UV)
					{	// Check fo any Status error before reading data
						bErrBitSet = GetRegErrorBitsUV(wTemp, false,	bUVStatusError,		bUVStatusErrorInt, 
																		bUVAddressOverflow,	bUVDataOverflow);
						// TLF - Now read data
						wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
						if (m_bFirstSum == true)
							m_pArraysUV[nArrayUV]->m_dwOddData[n]  = wTemp2;
						else
							m_pArraysUV[nArrayUV]->m_dwOddData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					else
					{
						// TLF - Don't set unread data to zero - read anyway later
//						m_pArraysUV[nArrayUV]->m_dwOddData[n] = 0;
						++nCnt;
						if ( bUVReadError == false )
							bUVPixelReadError = true;
						bErrBitSet = GetRegErrorBitsUV(wTemp, bUVPixelReadError,	bUVStatusError,		bUVStatusErrorInt, 
																					bUVAddressOverflow,	bUVDataOverflow);
						// TLF - Now read data even without Data Ready
						wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
						if (m_bFirstSum == true)
							m_pArraysUV[nArrayUV]->m_dwEvenData[n]  = wTemp2;
						else
							m_pArraysUV[nArrayUV]->m_dwEvenData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	1;	//UV
						wErrSA			=	m_pArraysUV[nArrayUV]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	true;
						bErrOverScan	=	false;
						bErrExtraPixels	=	false;
						wErrPixel = (WORD)((n * 2) + 1);	// High Res Odd
					}
				}
				if (nCnt > 0)
				{
					if (m_nDebug & DEBUG_MISC)
					{
						GetTimeStamp(&nBuffer[0]);

						addr = m_pArraysUV[nArrayUV]->m_nAddress;

						printf("%8s - Subarray: %d, Lost Pixels: %d\r\n", nBuffer, addr, nCnt);
					}
				}
				if (m_bOverscanMode == true)
				{
					for (n = nPixels; n < 48; n++)
					{
						if (wTemp & STATUS_DATA_READY_UV)
						{	// Check fo any Status error before reading data
							bErrBitSet = GetRegErrorBitsUV(wTemp, false,	bUVStatusError,		bUVStatusErrorInt, 
																			bUVAddressOverflow,	bUVDataOverflow);
							// TLF - Now read data
							wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
							wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
							if (m_bFirstSum == true)
								m_pArraysUV[nArrayUV]->m_dwOddData[n]  = wTemp2;
							else
								m_pArraysUV[nArrayUV]->m_dwOddData[n] += wTemp2;
							wTemp = m_IO.InputW(STATUS_REGISTER);
						}
						else
						{
							// TLF - Don't set unread data to zero - read anyway later
//							m_pArraysUV[nArrayUV]->m_dwOddData[n] = 0;
							++nCnt;
							if ( bUVReadError == false )
								bUVPixelReadError = true;
							bErrBitSet = GetRegErrorBitsUV(wTemp, bUVPixelReadError,	bUVStatusError,		bUVStatusErrorInt, 
																						bUVAddressOverflow,	bUVDataOverflow);
							// TLF - Now read data even without Data Ready
							wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
							wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
							if (m_bFirstSum == true)
								m_pArraysUV[nArrayUV]->m_dwEvenData[n]  = wTemp2;
							else
								m_pArraysUV[nArrayUV]->m_dwEvenData[n] += wTemp2;
							wTemp = m_IO.InputW(STATUS_REGISTER);
						}
						if ( bErrBitSet && (wErrUVorVIS == 0) )
						{
							wErrUVorVIS		=	1;	//UV
							wErrSA			=	m_pArraysUV[nArrayUV]->m_nAddress;
							wErrPixels		=	nPixels;
							bErrHiRes		=	true;
							bErrOverScan	=	true;
							bErrExtraPixels	=	false;
							wErrPixel = (WORD)((n * 2) + 1);	// High Res Odd
						}
					}
				}
			}
			if (wTemp & STATUS_DATA_READY_UV)
			{	// This should never happen - should generate an error
				short nCnt(0);
				do
				{	// Dummy reads
					// Check fo any Status error before reading data
					bErrBitSet = GetRegErrorBitsUV(wTemp, false,	bUVStatusError,		bUVStatusErrorInt, 
																	bUVAddressOverflow,	bUVDataOverflow);
					// TLF - Now read data
					wTemp  = m_CameraIO.UVInputW(ARRAY_DATA_UV);
					wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
					wTemp  = m_IO.InputW(STATUS_REGISTER);
					nCnt++;
					//..............................................................
					//  Set error on first extra pixel read
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	1;	//UV
						wErrSA			=	m_pArraysUV[nArrayUV]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	true;
						bErrOverScan	=	true;
						bErrExtraPixels	=	true;
						wErrPixel = nCnt;	// Extra pixel counter
					}
				} while (!(wTemp & STATUS_DATA_READY_UV));
				if (m_nDebug & DEBUG_MISC)
				{
					GetTimeStamp(&nBuffer[0]);

					addr = m_pArraysUV[nArrayUV]->m_nAddress;

					printf("%8s - Subarray: %d, Extra Pixels: %d\r\n", nBuffer, addr, nCnt);
				}
			}

			nArray   += 1;		// Increase count of total arrays read
			nArrayUV += 1;		// Increase count of UV arrays read
			if (nArrayUV + 1 < m_nArraysUV)
			{	// More Arrays - just pass next Array Address
				unArrayAddress = m_ReadTableUV[nArrayUV].array->address;
				m_CameraIO.UVOutputW(ARRAY_ADDRESS_UV, unArrayAddress);
			}
			else if (nArrayUV + 1 == m_nArraysUV)
			{	// Last Array - set COMMAND_ARRAY_READ_BIT bit low & pass last Array Address
				unTimingModeRegUV &= ~COMMAND_ARRAY_READ_BIT;
				m_CameraIO.UVOutputW(TIME_MODE_UV, unTimingModeRegUV);
				unArrayAddress = m_ReadTableUV[nArrayUV].array->address;
				m_CameraIO.UVOutputW(ARRAY_ADDRESS_UV, unArrayAddress);
			}
			else
			{	// No more UV Arrays
				bMoreArraysUV = false;
			}
			Sleep(0);	// Giving watchdog a chance if it needs it
		}

		// Pass VIS Array if more available
		if (bMoreArraysVIS == true)
		{	// Save subarray info if first time through only
			if (m_bFirstSum == true)
			{
				m_pArraysVIS[nArrayVIS]->m_nDetector = (BYTE) m_ReadTableVIS[nArrayVIS].array->detector;
				m_pArraysVIS[nArrayVIS]->m_nAddress  = (BYTE) m_ReadTableVIS[nArrayVIS].array->address;
				m_pArraysVIS[nArrayVIS]->m_nLocation = (BYTE) m_ReadTableVIS[nArrayVIS].array->location;
				if (m_bOverscanMode == true)
					m_pArraysVIS[nArrayVIS]->m_nPixels = (BYTE) (48 * (m_cVISres + 1));
				else
					m_pArraysVIS[nArrayVIS]->m_nPixels = (BYTE) (m_ReadTableVIS[nArrayVIS].array->pixels * (m_cVISres + 1));
				m_pArraysVIS[nArrayVIS]->m_nOrientation = (BYTE) m_ReadTableVIS[nArrayVIS].array->orient;
			}
			// Remember the number of whole pixels in subarray
			nPixels = m_ReadTableVIS[nArrayVIS].array->pixels;
			// Wait until some pixel data available
			int	nCnt(0);
			do
			{
				wTemp = m_IO.InputW(STATUS_REGISTER);
				if (++nCnt > 1000000)	// TLF - Increase from 65535
				{
					bVISReadError = true;
					bErrBitSet = GetRegErrorBitsVIS(wTemp, bVISReadError,	bVISStatusError,		bVISStatusErrorInt, 
																			bVISAddressOverflow,	bVISDataOverflow);
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	2;	//VIS
						wErrSA			=	m_pArraysVIS[nArrayVIS]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	(m_cVISres == HIGH) ? true : false;
						bErrOverScan	=	false;
						bErrExtraPixels	=	false;
						wErrPixel		=	0;
					}
				}
				Sleep(0);	// Giving watchdog a chance if it needs it
			} while (!(wTemp & STATUS_DATA_READY_VIS) && bVISReadError == false);
			// It's available - read everything there
			// If high res, read even pixels; if low res, read pixels and store in m_dwEvenData[n]
			nCnt = 0;
			for (n = 0; n < nPixels; n++)
			{
				if (wTemp & STATUS_DATA_READY_VIS)
				{	// Check fo any Status error before reading data
					bErrBitSet = GetRegErrorBitsVIS(wTemp, false,		bVISStatusError,		bVISStatusErrorInt, 
																		bVISAddressOverflow,	bVISDataOverflow);
					// TLF - Now read data
					wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
					wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
					if (m_bFirstSum == true)
						m_pArraysVIS[nArrayVIS]->m_dwEvenData[n]  = wTemp2;
					else
						m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] += wTemp2;
					wTemp = m_IO.InputW(STATUS_REGISTER);
				}
				else
				{
					// TLF - Don't set unread data to zero - read anyway later
//					m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] = 0;
					++nCnt;
					if ( bVISReadError == false )
						bVISPixelReadError = true;
					bErrBitSet = GetRegErrorBitsVIS(wTemp, bVISPixelReadError,	bVISStatusError,		bVISStatusErrorInt, 
																				bVISAddressOverflow,	bVISDataOverflow);
					// TLF - Now read data even without Data Ready
					wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
					wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
					if (m_bFirstSum == true)
						m_pArraysVIS[nArrayVIS]->m_dwEvenData[n]  = wTemp2;
					else
						m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] += wTemp2;
					wTemp = m_IO.InputW(STATUS_REGISTER);
				}
				if ( bErrBitSet && (wErrUVorVIS == 0) )
				{
					wErrUVorVIS		=	2;	//VIS
					wErrSA			=	m_pArraysVIS[nArrayVIS]->m_nAddress;
					wErrPixels		=	nPixels;
					bErrHiRes		=	(m_cVISres == HIGH) ? true : false;
					bErrOverScan	=	false;
					bErrExtraPixels	=	false;
					if ( bErrHiRes )
						wErrPixel = (WORD) (n * 2);	// High Res Even
					else
						wErrPixel		=	n;  // Low Res  
				}
			}
			if (nCnt > 0)
			{
				if (m_nDebug & DEBUG_MISC)
				{
					GetTimeStamp(&nBuffer[0]);

					addr = m_pArraysVIS[nArrayVIS]->m_nAddress;

					printf("%8s - Subarray: %d, Lost Pixels: %d\r\n", nBuffer, addr, nCnt);
				}
			}
			if (m_bOverscanMode == true)
			{
				for (n = nPixels; n < 48; n++)
				{
					if (wTemp & STATUS_DATA_READY_VIS)
					{	// Check fo any Status error before reading data
						bErrBitSet = GetRegErrorBitsVIS(wTemp, false,	bVISStatusError,		bVISStatusErrorInt, 
																		bVISAddressOverflow,	bVISDataOverflow);
						// TLF - Now read data
						wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
						if (m_bFirstSum == true)
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n]  = wTemp2;
						else
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					else
					{
						// TLF - Don't set unread data to zero - read anyway later
//						m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] = 0;
						++nCnt;
						if ( bVISReadError == false )
							bVISPixelReadError = true;
						bErrBitSet = GetRegErrorBitsVIS(wTemp, bVISPixelReadError,	bVISStatusError,		bVISStatusErrorInt, 
																					bVISAddressOverflow,	bVISDataOverflow);
						// TLF - Now read data even without Data Ready
						wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
						if (m_bFirstSum == true)
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n]  = wTemp2;
						else
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	2;	//VIS
						wErrSA			=	m_pArraysVIS[nArrayVIS]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	(m_cVISres == HIGH) ? true : false;
						bErrOverScan	=	true;
						bErrExtraPixels	=	false;
						if ( bErrHiRes )
							wErrPixel = (WORD)(n * 2);		// High Res Even
						else
							wErrPixel		=	n;  // Low Res  
					}
				}
			}
			// If high res, now read odd pixels
			nCnt = 0;
			if (m_cVISres)
			{
				for (n = 0; n < nPixels; n++)
				{
					if (wTemp & STATUS_DATA_READY_VIS)
					{	// Check fo any Status error before reading data
						// TLF - Added check of the Status Error bits
						if (!(wTemp & STATUS_ERROR_VIS))
							bVISStatusError = true;
						bErrBitSet = GetRegErrorBitsVIS(wTemp, bVISStatusError,	bVISStatusError,		bVISStatusErrorInt, 
																				bVISAddressOverflow,	bVISDataOverflow);
						// TLF - Now read data
						wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
						if (m_bFirstSum == true)
							m_pArraysVIS[nArrayVIS]->m_dwOddData[n]  = wTemp2;
						else
							m_pArraysVIS[nArrayVIS]->m_dwOddData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					else
					{
						// TLF - Don't set unread data to zero - read anyway later
//						m_pArraysVIS[nArrayVIS]->m_dwOddData[n] = 0;
						++nCnt;
						if ( bVISReadError == false )
							bVISPixelReadError = true;
						bErrBitSet = GetRegErrorBitsVIS(wTemp, bVISPixelReadError,	bVISStatusError,		bVISStatusErrorInt, 
																					bVISAddressOverflow,	bVISDataOverflow);
						// TLF - Now read data even without Data Ready
						wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
						if (m_bFirstSum == true)
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n]  = wTemp2;
						else
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	2;	//VIS
						wErrSA			=	m_pArraysVIS[nArrayVIS]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	true;
						bErrOverScan	=	false;
						bErrExtraPixels	=	false;
						wErrPixel = (WORD)((n * 2) + 1);	// High Res Odd
					}
				}
				if (nCnt > 0)
				{
					if (m_nDebug & DEBUG_MISC)
					{
						GetTimeStamp(&nBuffer[0]);

						addr = m_pArraysVIS[nArrayVIS]->m_nAddress;

						printf("%8s - Subarray: %d, Lost Pixels: %d\r\n", nBuffer, addr, nCnt);
					}
				}
			}
			if (m_bOverscanMode == true)
			{
				for (n = nPixels; n < 48; n++)
				{
					if (wTemp & STATUS_DATA_READY_VIS)
					{	// Check fo any Status error before reading data
						bErrBitSet = GetRegErrorBitsVIS(wTemp, false,	bVISStatusError,		bVISStatusErrorInt, 
																		bVISAddressOverflow,	bVISDataOverflow);
						// TLF - Now read data
						wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
						if (m_bFirstSum == true)
							m_pArraysVIS[nArrayVIS]->m_dwOddData[n]  = wTemp2;
						else
							m_pArraysVIS[nArrayVIS]->m_dwOddData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
					else
					{
						// TLF - Don't set unread data to zero - read anyway later
//						m_pArraysVIS[nArrayVIS]->m_dwOddData[n] = 0;
						++nCnt;
						if ( bVISReadError == false )
							bVISPixelReadError = true;
						bErrBitSet = GetRegErrorBitsVIS(wTemp, bVISPixelReadError,	bVISStatusError,		bVISStatusErrorInt, 
																					bVISAddressOverflow,	bVISDataOverflow);
						// TLF - Now read data even without Data Ready
						wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
						wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
						if (m_bFirstSum == true)
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n]  = wTemp2;
						else
							m_pArraysVIS[nArrayVIS]->m_dwEvenData[n] += wTemp2;
						wTemp = m_IO.InputW(STATUS_REGISTER);
					}
				}
				if ( bErrBitSet && (wErrUVorVIS == 0) )
				{
					wErrUVorVIS		=	2;	//VIS
					wErrSA			=	m_pArraysVIS[nArrayVIS]->m_nAddress;
					wErrPixels		=	nPixels;
					bErrHiRes		=	true;
					bErrOverScan	=	true;
					bErrExtraPixels	=	false;
					wErrPixel = (WORD)((n * 2) + 1);	// High Res Odd
				}
			}
			if (wTemp & STATUS_DATA_READY_VIS)
			{	// This should never happen - should generate an error
				short nCnt(0);
				do
				{	// Dummy reads
					// Check fo any Status error before reading data
					bErrBitSet = GetRegErrorBitsVIS(wTemp, false,	bVISStatusError,		bVISStatusErrorInt, 
																	bVISAddressOverflow,	bVISDataOverflow);
					// TLF - Now read data
					wTemp  = m_CameraIO.VISInputW(ARRAY_DATA_VIS);
					wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
					wTemp  = m_IO.InputW(STATUS_REGISTER);
					nCnt++;
					//..............................................................
					//  Set error on first extra pixel read
					if ( bErrBitSet && (wErrUVorVIS == 0) )
					{
						wErrUVorVIS		=	2;	//VIS
						wErrSA			=	m_pArraysVIS[nArrayVIS]->m_nAddress;
						wErrPixels		=	nPixels;
						bErrHiRes		=	true;
						bErrOverScan	=	true;
						bErrExtraPixels	=	true;
						wErrPixel = nCnt;	// Extra pixel counter
					}
				} while (!(wTemp & STATUS_DATA_READY_VIS));
				if (m_nDebug & DEBUG_MISC)
				{
					GetTimeStamp(&nBuffer[0]);

					addr = m_pArraysVIS[nArrayVIS]->m_nAddress;

					printf("%8s - Subarray: %d, Extra Pixels: %d\r\n", nBuffer, addr, nCnt);
				}
			}

			nArray    += 1;		// Increase count of total arrays read
			nArrayVIS += 1;		// Increase count of VIS arrays read
			if (nArrayVIS + 1 < m_nArraysVIS)
			{	// More Arrays - just pass next Array Address
				unArrayAddress = m_ReadTableVIS[nArrayVIS].array->address;
				m_CameraIO.VISOutputW(ARRAY_ADDRESS_VIS, unArrayAddress);
			}
			else if (nArrayVIS + 1 == m_nArraysVIS)
			{	// Last Array - set COMMAND_ARRAY_READ_BIT bit low & pass last Array Address
				unTimingModeRegVIS &= ~COMMAND_ARRAY_READ_BIT;
				m_CameraIO.VISOutputW(TIME_MODE_VIS, unTimingModeRegVIS);
				unArrayAddress = m_ReadTableVIS[nArrayVIS].array->address;
				m_CameraIO.VISOutputW(ARRAY_ADDRESS_VIS, unArrayAddress);
			}
			else
			{	// No more VIS Arrays
				bMoreArraysVIS = false;
			}
			Sleep(0);	// Giving watchdog a chance if it needs it
		}
	}
	//printf("THN cmdRead()... Post all sub arrays read loop\r\n");

	if (m_bFirstSum == true)
		m_bFirstSum = false;	// First only once

	if (m_bLastSum == true)
	{
		// For SumData only - average readings
		if (m_bAverage == true)
			average(m_nNoReads);

		// Copy data to storage area (&m_nDataBuf[9])
		//  Bytes 0 through 8 are for command reply and byte count
		if (m_bTransient)
		{
			copyTransientData();
		}
		else
		{	// OK to increment m_nDataBlock - not transient
			m_nDataBlock++;
			copyData();
			m_lBlockAddress[m_nDataBlock] = m_lCurrentByte;
		}

		if (m_bTransient)
		{
			moveData();
			// Don't increment m_nDataBlock until data has moved if transient
			m_nDataBlock++;
			//THN ... Same for mlBlockAddess[...] since m_lCurrent byte is reset in movedata()
			m_lBlockAddress[m_nDataBlock] = m_lCurrentByte;
		}
	}

	status = GetReadStatusCode(bUVReadError,    bUVPixelReadError,  bVISReadError,       bVISPixelReadError,
							   bUVStatusError,  bUVStatusErrorInt,  bUVAddressOverflow,  bUVDataOverflow,
							   bVISStatusError, bVISStatusErrorInt, bVISAddressOverflow, bVISDataOverflow);
	// Attempt to store error bits if an error occurred this call.
	if ( wErrUVorVIS )
		StoreDetectorReadInfo(status, wErrUVorVIS, wErrSA, wErrPixels, bErrHiRes, bErrOverScan, bErrExtraPixels, wErrPixel);

	//printf("THN cmdRead()... ResumeThreads()\r\n");
ResumeThreads();
	//printf("THN cmdRead()... ResumeThreads()... Post call\r\n");

	return status;
}
//----------------------------------------------------------------------------------------------------------
// Get and set the UV Camera detector error bits and return true if any bit gets set or if
// bOtherErr boolean was set.
// wTemp must be equal to the value of the STATUS_REGISTER when called.
 
bool CParserThread::GetRegErrorBitsUV(WORD wTemp, bool bOtherErr,	bool& bUVStatusError, bool& bUVStatusErrorInt, 
																	bool& bUVAddressOverflow, bool& bUVDataOverflow)
{
	WORD	wTemp2;
	bool	bRetCode(false); // Return code set if any error bit is set


	if (!(wTemp & STATUS_ERROR_UV))
	{
		bUVStatusError	= true;
		bRetCode		= true;
	}
	if (!(wTemp & STATUS_ERROR_INT_UV))
	{
		bUVStatusErrorInt	= true;
		bRetCode			= true;
		wTemp  = m_CameraIO.InputW(SYSTEM_ERROR_UV);
		wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_UV);
		if (wTemp2 & ADDRESS_OVERFLOW)
			bUVAddressOverflow = true;
		if (wTemp2 & DATA_OVERFLOW)
			bUVDataOverflow = true;
	}
	bRetCode |= bOtherErr;
	return bRetCode;
}
//----------------------------------------------------------------------------------------------------------
// Get and set the VIS Camera detector error bits and return true if any bit gets set or if
// bOtherErr boolean was set.
// wTemp must be equal to the value of the STATUS_REGISTER when called.
 
bool CParserThread::GetRegErrorBitsVIS(WORD wTemp, bool bOtherErr, bool& bVISStatusError, bool& bVISStatusErrorInt, 
									  bool& bVISAddressOverflow, bool& bVISDataOverflow)
{
	WORD	wTemp2;
	bool	bRetCode(false); // Return code set if any error bit is set


	if (!(wTemp & STATUS_ERROR_VIS))
	{
		bVISStatusError	= true;
		bRetCode		= true;
	}
	if (!(wTemp & STATUS_ERROR_INT_VIS))
	{
		bVISStatusErrorInt	= true;
		bRetCode			= true;
		wTemp  = m_CameraIO.InputW(SYSTEM_ERROR_VIS);
		wTemp2 = m_IO.InputW(ARRAY_DATA_LATCH_VIS);
		if (wTemp2 & ADDRESS_OVERFLOW)
			bVISAddressOverflow = true;
		if (wTemp2 & DATA_OVERFLOW)
			bVISDataOverflow = true;
	}
	bRetCode |= bOtherErr;
	return bRetCode;
}
//----------------------------------------------------------------------------------------------------------
//  Store the Detector Read information when the m_bDetectorReadErr is false
//   Parameters are stored in class member variables until uploaded by software command.
//   Note: m_bDetectorReadErr is reset to false upon upload to software.

void CParserThread::StoreDetectorReadInfo(WORD status, WORD wErrUVorVIS, WORD wErrSA, WORD wErrPixels,
										  bool bErrHiRes, bool bErrOverScan, bool bErrExtraPixels, WORD wErrPixel)
{
	if ( !m_bDetectorReadErr)
	{

		m_bDetectorReadErr	= true; 
		GetDateTimeStamp(m_szDateTimeBuf);
		m_wStatus			= status;
		m_wErrUVorVIS		= wErrUVorVIS;
		m_wErrSA			= wErrSA;
		m_wErrPixels		= wErrPixels;
		m_bErrHiRes			= bErrHiRes;
		m_bErrOverScan		= bErrOverScan;	 
		m_bErrExtraPixels	= bErrExtraPixels;
		m_wErrPixel			= wErrPixel;	
	}
}
//----------------------------------------------------------------------------------------------------------
//  Return two char status code for read operation
//
WORD CParserThread::GetReadStatusCode(bool bUVReadError,    bool bUVPixelReadError,  bool bVISReadError,       bool bVISPixelReadError,
									  bool bUVStatusError,  bool bUVStatusErrorInt,  bool bUVAddressOverflow,  bool bUVDataOverflow,
									  bool bVISStatusError, bool bVISStatusErrorInt, bool bVISAddressOverflow, bool bVISDataOverflow)

{
	WORD status(NO_ERRORS);
	
	if (bUVReadError == true)
	{
		if		(!bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV;
		else if ( bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV_1;
		else if (!bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV_2;
		else if ( bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV_3;
		else if (!bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV_4;
		else if ( bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV_5;
		else if (!bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV_6;
		else if ( bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_READ_UV_7;
		else if (!bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_8;
		else if ( bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_9;
		else if (!bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_A;
		else if ( bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_B;
		else if (!bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_C;
		else if ( bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_D;
		else if (!bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_E;
		else if ( bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_READ_UV_F;
	}
	else if (bUVPixelReadError == true)
	{
		if		(!bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV;
		else if ( bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_1;
		else if (!bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_2;
		else if ( bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_3;
		else if (!bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_4;
		else if ( bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_5;
		else if (!bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_6;
		else if ( bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_7;
		else if (!bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_8;
		else if ( bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_9;
		else if (!bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_A;
		else if ( bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_B;
		else if (!bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_C;
		else if ( bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_D;
		else if (!bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_E;
		else if ( bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_PIXEL_READ_UV_F;
	}
	else if (bVISReadError == true)
	{
		if		(!bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS;
		else if ( bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS_1;
		else if (!bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS_2;
		else if ( bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS_3;
		else if (!bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS_4;
		else if ( bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS_5;
		else if (!bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS_6;
		else if ( bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_READ_VIS_7;
		else if (!bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_8;
		else if ( bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_9;
		else if (!bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_A;
		else if ( bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_B;
		else if (!bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_C;
		else if ( bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_D;
		else if (!bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_E;
		else if ( bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_READ_VIS_F;
	}
	else if (bVISPixelReadError == true)
	{
		if		(!bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS;
		else if ( bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_1;
		else if (!bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_2;
		else if ( bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_3;
		else if (!bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_4;
		else if ( bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_5;
		else if (!bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_6;
		else if ( bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_7;
		else if (!bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_8;
		else if ( bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_9;
		else if (!bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_A;
		else if ( bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_B;
		else if (!bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_C;
		else if ( bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_D;
		else if (!bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_E;
		else if ( bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_PIXEL_READ_VIS_F;
	}
	else
	{
		if		( bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_OK_READ_UV_1;
		else if (!bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_OK_READ_UV_2;
		else if ( bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_OK_READ_UV_3;
		else if (!bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_OK_READ_UV_4;
		else if ( bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_OK_READ_UV_5;
		else if (!bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_OK_READ_UV_6;
		else if ( bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow && !bUVDataOverflow)
			status = ERR_OK_READ_UV_7;
		else if (!bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_8;
		else if ( bUVStatusError && !bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_9;
		else if (!bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_A;
		else if ( bUVStatusError &&  bUVStatusErrorInt && !bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_B;
		else if (!bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_C;
		else if ( bUVStatusError && !bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_D;
		else if (!bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_E;
		else if ( bUVStatusError &&  bUVStatusErrorInt &&  bUVAddressOverflow &&  bUVDataOverflow)
			status = ERR_OK_READ_UV_F;
		else if ( bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_OK_READ_VIS_1;
		else if (!bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_OK_READ_VIS_2;
		else if ( bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_OK_READ_VIS_3;
		else if (!bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_OK_READ_VIS_4;
		else if ( bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_OK_READ_VIS_5;
		else if (!bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_OK_READ_VIS_6;
		else if ( bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow && !bVISDataOverflow)
			status = ERR_OK_READ_VIS_7;
		else if (!bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_8;
		else if ( bVISStatusError && !bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_9;
		else if (!bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_A;
		else if ( bVISStatusError &&  bVISStatusErrorInt && !bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_B;
		else if (!bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_C;
		else if ( bVISStatusError && !bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_D;
		else if (!bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_E;
		else if ( bVISStatusError &&  bVISStatusErrorInt &&  bVISAddressOverflow &&  bVISDataOverflow)
			status = ERR_OK_READ_VIS_F;
	}
	return status;
}
//===========================================================================
void CParserThread::SmallDelay()
{
	m_IO.InputW(FPGA_VERSION_NO);
}
//===========================================================================
//  continueWait 
//
//  Returns false when the current system tick time is greater than 
//	dwBegin by more than dwDelay; else returns false.
//
//	Note: GetTickCount wraps around every 49.7 days.
//        The math will work out with only one wrap around :: (49.7 x 2) 
//
bool CParserThread::continueWait(
								 DWORD dwBegin,  // Start tick count in ms
								 DWORD dwDelay	 // Time interval in ms
								)
{
	DWORD	dwNow = ::GetTickCount();

	if ( (dwNow - dwBegin) >= dwDelay )
		return false;	// Delay interval exceeded
	else
		return true;	// Delay interval not exceeded
}
//===========================================================================

void CParserThread::average(unsigned short readings)
{
	short	n, m;
	BYTE	cPixels;

	if (m_nArraysVIS)
	{
		if (m_cVISres)
		{
			for (n = 0; n < m_nArraysVIS; n++)
			{
				cPixels = (BYTE) (m_pArraysVIS[n]->m_nPixels / 2);
				for (m = 0; m < cPixels / 2; m++)
				{
					m_pArraysVIS[n]->m_dwEvenData[m] /= readings;
					m_pArraysVIS[n]->m_dwOddData[m]  /= readings;
				}
			}
		}
		else
		{
			for (n = 0; n < m_nArraysVIS; n++)
			{
				cPixels = m_pArraysVIS[n]->m_nPixels;
				for (m = 0; m < cPixels; m++)
				{
					m_pArraysVIS[n]->m_dwEvenData[m] /= readings;
				}
			}
		}
	}

	if (m_nArraysUV)
	{
		if (m_cUVres)
		{
			for (n = 0; n < m_nArraysUV; n++)
			{
				cPixels = (BYTE) (m_pArraysUV[n]->m_nPixels / 2);
				for (m = 0; m < cPixels; m++)
				{
					m_pArraysUV[n]->m_dwEvenData[m] /= readings;
					m_pArraysUV[n]->m_dwOddData[m]  /= readings;
				}
			}
		}
		else
		{
			for (n = 0; n < m_nArraysUV; n++)
			{
				cPixels = m_pArraysUV[n]->m_nPixels;
				for (m = 0; m < cPixels; m++)
				{
					m_pArraysUV[n]->m_dwEvenData[m] /= readings;
				}
			}
		}
	}
}
//===========================================================================

void CParserThread::copyData()
{
	short	n, m;
	BYTE	cAddr;
	BYTE	cTemp;
	BYTE	cPixels;
	WORD	wREV;
	DWORD	dwREV;

	wREV =  (WORD) REVWORD(m_nDataBlock);
	memcpy(&m_nDataBuf[m_lCurrentByte], &wREV,  2);					// Insert Block Number
	m_lCurrentByte += 2;

	cTemp = (BYTE) m_nTotalArrays;
	memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);				// Insert Number of Arrays

	cTemp = 1;
	memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);				// Insert dummy value

	if (m_nArraysUV)
	{
		for (n = 0; n < m_nArraysUV; n++)
		{
			cAddr = m_pArraysUV[n]->m_nLocation;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cAddr, 1);			// Insert Array Address
			cTemp = m_pArraysUV[n]->m_nPixels;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);			// Insert Number of Pixels
			wREV =  (WORD) REVWORD(m_nIntegrationTime);
			memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);				// Insert Integration Time
			m_lCurrentByte += 2;
			wREV =  (WORD) REVWORD(m_nNoReads);
			memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);				// Insert Number of Reads
			m_lCurrentByte += 2;
			cPixels = (BYTE) m_ReadTableUV[n].array->pixels;							// Actual number of pixels
			if (m_cUVres == HIGH)
			{	// High Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysUV[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							dwREV = m_pArraysUV[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;

							dwREV = m_pArraysUV[n]->m_dwOddData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
					else
					{	// Reverse the order in the data area from the oder saved originally
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order has Odd pixel first
							dwREV = m_pArraysUV[n]->m_dwOddData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;

							dwREV = m_pArraysUV[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{	// Fill in Even pixels first
						dwREV = m_pArraysUV[n]->m_dwEvenData[m];
						dwREV = REVDWORD(dwREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);			// Insert Pixel data
						m_lCurrentByte += 4;

						dwREV = m_pArraysUV[n]->m_dwOddData[m];
						dwREV = REVDWORD(dwREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);			// Insert Pixel data
						m_lCurrentByte += 4;
					}
				}
			}
			else
			{	// Low Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysUV[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							dwREV = m_pArraysUV[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
					else
					{
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order has Odd pixel first
							dwREV = m_pArraysUV[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{
						dwREV = m_pArraysUV[n]->m_dwEvenData[m];
						dwREV = REVDWORD(dwREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);			// Insert Pixel data
						m_lCurrentByte += 4;
					}
				}
			}
			Sleep(0);	// Giving watchdog a chance if it needs it
		}
	}

	if (m_nArraysVIS)
	{
		for (n = 0; n < m_nArraysVIS; n++)
		{
			cAddr = m_pArraysVIS[n]->m_nLocation;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cAddr, 1);			// Insert Array Address
			cTemp = m_pArraysVIS[n]->m_nPixels;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);			// Insert Number of Pixels
			wREV =  (WORD) REVWORD(m_nIntegrationTime);
			memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);				// Insert Integration Time
			m_lCurrentByte += 2;
			wREV =  (WORD) REVWORD(m_nNoReads);
			memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);				// Insert Number of Reads
			m_lCurrentByte += 2;
			cPixels = (BYTE) m_ReadTableVIS[n].array->pixels;							// Actual number of pixels
			if (m_cVISres == HIGH)
			{	// High Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysVIS[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							dwREV = m_pArraysVIS[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;

							dwREV = m_pArraysVIS[n]->m_dwOddData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
					else
					{	// Reverse the order in the data area from the oder saved originally
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order has Odd pixel first
							dwREV = m_pArraysVIS[n]->m_dwOddData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;

							dwREV = m_pArraysVIS[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{	// Fill in Even pixels first
						dwREV = m_pArraysVIS[n]->m_dwEvenData[m];
						dwREV = REVDWORD(dwREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);			// Insert Pixel data
						m_lCurrentByte += 4;

						dwREV = m_pArraysVIS[n]->m_dwOddData[m];
						dwREV = REVDWORD(dwREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);			// Insert Pixel data
						m_lCurrentByte += 4;
					}
				}
			}
			else
			{	// Low Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysVIS[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							dwREV = m_pArraysVIS[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
					else
					{
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order has Odd pixel first
							dwREV = m_pArraysVIS[n]->m_dwEvenData[m];
							dwREV = REVDWORD(dwREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);		// Insert Pixel data
							m_lCurrentByte += 4;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{
						dwREV = m_pArraysVIS[n]->m_dwEvenData[m];
						dwREV = REVDWORD(dwREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &dwREV, 4);			// Insert Pixel data
						m_lCurrentByte += 4;
					}
				}
			}
			Sleep(0);	// Giving watchdog a chance if it needs it
		}
	}
	m_bDataFlag = true;
}
//===========================================================================

void CParserThread::copyTransientData()
{
	short	n, m;
	BYTE	cAddr;
	BYTE	cTemp;
	BYTE	cPixels;
	WORD	wREV;

	wREV =  (WORD) REVWORD(m_nDataBlock + 1);
	memcpy(&m_nDataBuf[m_lCurrentByte], &wREV,  2);					// Insert Block Number
	m_lCurrentByte += 2;

	cTemp = (BYTE) m_nTotalArrays;
	memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);				// Insert Number of Arrays

	cTemp = 1;
	memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);				// Insert dummy value

	if (m_nArraysUV)
	{
		for (n = 0; n < m_nArraysUV; n++)
		{
			cAddr = m_pArraysUV[n]->m_nLocation;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cAddr, 1);			// Insert Array Address
			cTemp = m_pArraysUV[n]->m_nPixels;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);			// Insert Number of Pixels
			cPixels = (BYTE) m_ReadTableUV[n].array->pixels;			// Actual number of pixels
			if (m_cUVres == HIGH)
			{	// High Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysUV[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							wREV = (WORD) m_pArraysUV[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;

							wREV = (WORD) m_pArraysUV[n]->m_dwOddData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
					else
					{	// Reverse the order in the data area from the oder saved originally
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order has Odd pixel first
							wREV = (WORD) m_pArraysUV[n]->m_dwOddData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;

							wREV = (WORD) m_pArraysUV[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{	// Fill in Even pixels first
						wREV = (WORD) m_pArraysUV[n]->m_dwEvenData[m];
						wREV = (WORD) REVWORD(wREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);			// Insert Pixel data
						m_lCurrentByte += 2;

						wREV = (WORD) m_pArraysUV[n]->m_dwOddData[m];
						wREV = (WORD) REVWORD(wREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);			// Insert Pixel data
						m_lCurrentByte += 2;
					}
				}
			}
			else
			{	// Low Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysUV[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							wREV = (WORD) m_pArraysUV[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
					else
					{
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order
							wREV = (WORD) m_pArraysUV[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{
						wREV = (WORD) m_pArraysUV[n]->m_dwEvenData[m];
						wREV = (WORD) REVWORD(wREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);			// Insert Pixel data
						m_lCurrentByte += 2;
					}
				}
			}
			Sleep(0);	// Giving watchdog a chance if it needs it
		}
	}

	if (m_nArraysVIS)
	{
		for (n = 0; n < m_nArraysVIS; n++)
		{
			cAddr = m_pArraysVIS[n]->m_nLocation;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cAddr, 1);			// Insert Array Address
			cTemp = m_pArraysVIS[n]->m_nPixels;
			memcpy(&m_nDataBuf[m_lCurrentByte++], &cTemp, 1);			// Insert Number of Pixels
			cPixels = (BYTE) m_ReadTableVIS[n].array->pixels;			// Actual number of pixels
			if (m_cVISres == HIGH)
			{	// High Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysVIS[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							wREV = (WORD) m_pArraysVIS[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;

							wREV = (WORD) m_pArraysVIS[n]->m_dwOddData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
					else
					{	// Reverse the order in the data area from the oder saved originally
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order has Odd pixel first
							wREV = (WORD) m_pArraysVIS[n]->m_dwOddData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;

							wREV = (WORD) m_pArraysVIS[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{	// Fill in Even pixels first
						wREV = (WORD) m_pArraysVIS[n]->m_dwEvenData[m];
						wREV = (WORD) REVWORD(wREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);			// Insert Pixel data
						m_lCurrentByte += 2;

						wREV = (WORD) m_pArraysVIS[n]->m_dwOddData[m];
						wREV = (WORD) REVWORD(wREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);			// Insert Pixel data
						m_lCurrentByte += 2;
					}
				}
			}
			else
			{	// Low Res mode
				// If in Overscan mode, actual number of pixels is different than pixels read
				if (cPixels > 0)
				{	// Handle actual pixels first
					if (m_pArraysVIS[n]->m_nOrientation == NORMAL)
					{	// Just fill in data area in the order saved originally
						for (m = 0; m < cPixels; m++)
						{	// Normal order has Even pixel first
							wREV = (WORD) m_pArraysVIS[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
					else
					{
						for (m = (short) (cPixels - 1); m >= 0; m--)
						{	// Inverse order
							wREV = (WORD) m_pArraysVIS[n]->m_dwEvenData[m];
							wREV = (WORD) REVWORD(wREV);
							memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);		// Insert Pixel data
							m_lCurrentByte += 2;
						}
					}
				}
				if (m_bOverscanMode == true)
				{	// Take care of any additional pixels from Overscan mode
					for (m = cPixels; m < 48; m++)
					{
						wREV = (WORD) m_pArraysVIS[n]->m_dwEvenData[m];
						wREV = (WORD) REVWORD(wREV);
						memcpy(&m_nDataBuf[m_lCurrentByte], &wREV, 2);			// Insert Pixel data
						m_lCurrentByte += 2;
					}
				}
			}
			Sleep(0);	// Giving watchdog a chance if it needs it
		}
	}
	m_bDataFlag = true;
}
//===========================================================================

void CParserThread::moveData()
{
	if (m_bFirstTransient == true)
	{
		m_bFirstTransient = false;

		m_lBlockAddress[0] = 8;	// Making room for "TA00xxxx"
		m_lBlockAddress[1] = m_lCurrentByte; 

		// Determine the size of one data block - one read
		m_lBlockSize = m_lBlockAddress[1] - m_lBlockAddress[0];

		if (m_lBlockSize * m_nTransientReads < MAX_DATA_BYTES)
		{	// No need for circular buffer
			m_nFullBlocks = m_nTransientReads;
		}
		else
		{	// Circular buffer needed to handle all the data
			m_nFullBlocks = (WORD) (MAX_DATA_BYTES / m_lBlockSize);
		}
		m_pReadData = new BYTE[m_lBlockSize * m_nFullBlocks];
		m_nBlockCount = 0;
		m_nLastBlockSent = 0;
	}

	m_nBlockCount += 1;

	if (m_nBlockCount >  m_nFullBlocks)
	{
		m_nBlockCount = (WORD) (m_nBlockCount - m_nFullBlocks);

		// Start over from the top as if it were the first read
		m_lCurrentByte = 8;
	}
	// Starts at 0 and increases by m_lBlockSize & then starts over if needed
	BYTE *	pData = &m_pReadData[(m_nBlockCount - 1) * m_lBlockSize];	// Start of transient buffer
	//................................................................................................
	 m_paBlockNo.Add(pData);  // THN Adds a block to the End of the array
	//................................................................................................
	// Copy data from original location to transient buffer location
	for (long lByte = 0; lByte < m_lBlockSize; lByte++)
		*pData++ =  m_nDataBuf[m_lBlockAddress[m_nDataBlock] + lByte];
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/cmdRead.cpp $
 * 
 * 62    7/10/07 4:05p Nashth
 * Small correction to error code pickup.
 * 
 * 61    7/06/07 4:51p Nashth
 * Added an information error string when a detector read error occurs as
 * per QPR: THNJ74CQCW.
 * 
 * 60    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 59    5/23/07 8:27a Frazzitl
 * One more time.
 * 
 * 58    5/23/07 7:39a Frazzitl
 * Added internal buffer read for errors.
 * 
 * 57    5/22/07 2:38p Frazzitl
 * More bit polarity problems
 * 
 * 56    5/22/07 10:56a Frazzitl
 * Corrected Error Status Polarity
 * 
 * 55    5/22/07 8:27a Frazzitl
 * Added additional errors
 * 
 * 54    5/21/07 10:48a Frazzitl
 * Added error checking even with good reads.
 * 
 * 53    5/21/07 8:23a Frazzitl
 * Added new errors to better describe Read Errors
 * 
 * 52    3/07/07 3:32p Nashth
 * continueWait repaired as required for QPR: THNJ6XXKR5.
 * 
 * 51    5/19/06 1:03p Frazzitl
 * Added changes to display on the HyperTerminal the Time and Subarray
 * Address when a missing pixel error is encountered.
 * 
 * 50    4/21/06 1:10p Frazzitl
 * Added feature to check for and generate an error if either some or all
 * pixels are not received from the detector for a given subarray.
 * 
 * 49    1/17/06 2:24p Frazzitl
 * Removed 'cancel' and 'abort' from all read commands
 * 
 * 48    12/29/05 3:46p Nashth
 * Cleaned up some comments and commented code.
 * 
 * 47    12/29/05 11:18a Nashth
 * FIAS data collection is no longer read time limited as per QPR:
 * DAYS6GVP3X.
 * 
 * 46    12/29/05 8:23a Nashth
 * Setup first two m_lBlockAddress values initially to determine the block
 * size correctly.
 * 
 * 45    12/28/05 4:29p Nashth
 * Fixed a problem with circular buffer m_nReadData not being pointed to
 * correctly when circular when m_lBlockData should be reset.
 * 
 * 44    12/27/05 1:14p Nashth
 * Comments added for possible future code modification.
 * Also, added special debug code to fake out a FIAS read trigger 
 * input after receipt of the third TT command.
 * 
 * 43    12/21/05 10:28a Frazzitl
 * Temporarily remove fix for Cancel
 * 
 * 42    12/21/05 10:23a Frazzitl
 * Fixed problem with Tub temperature & read reset
 * 
 * 41    12/02/05 12:50p Nashth
 * Put detector back in READY mode after placing it in RESET after a
 * cancel.  Removed the RESET mode on completion of a read.
 * 
 * 40    11/29/05 5:13p Nashth
 * Commented out some printfs used for debug.
 * 
 * 39    11/22/05 10:10a Nashth
 * Added some Printf statements for debugging... Some with THN within the
 * Print Text... These can eventually be removed or made debug flag
 * optional.
 * 
 * 38    11/17/05 1:27p Frazzitl
 * Made printf statement require a debug flag in order to execute.
 * 
 * 37    11/16/05 4:00p Frazzitl
 * Corrected problem cause by lloking for cancel during Integration time.
 * 
 * 36    11/10/05 9:06a Frazzitl
 * Added code to enable cancelling of a read, including during the
 * integration time.
 * 
 * 35    11/01/05 3:31p Frazzitl
 * Added code to catch problem with reading too few pixels
 * 
 * 34    10/31/05 2:25p Frazzitl
 * Added QPR number THNJ6HPPXY to comment regarding the addition of a
 * cancel feature.
 * 
 * 33    10/31/05 10:43a Frazzitl
 * Implemented 'cancel' function for all types of reads - THNJ6HPPXY
 * 
 * 32    9/29/05 10:11a Nashth
 * Removed debug code after fixing Transient problem
 * 
 * 31    9/26/05 12:30p Frazzitl
 * Added more debugging code that can be removed later
 * 
 * 30    9/26/05 11:41a Nashth
 * Added changes to help debug Transient Mode problem - more to come
 * 
 * 29    9/14/05 3:17p Nashth
 * Changed one REVDWORD to REVWORD in copyTransientData.
 * 
 * 28    9/14/05 9:08a Frazzitl
 * Corrected pointer problem
 * 
 * 27    9/14/05 8:37a Frazzitl
 * Fixed problem with pointer to data
 * 
 * 26    9/13/05 9:09a Frazzitl
 * Added routine to copy Transient data which passes WORD data instead of
 * long data.
 * 
 * 25    9/13/05 9:10a Carrassm
 * Added a delay because VIS camera on prototype 1 was not processing the
 * command properly without the dealy
 * 
 * 24    9/09/05 7:33a Frazzitl
 * Transient Signal (FIAS) mode has been changed completely for
 * implementation with a single Icarus board.
 * 
 * 23    8/24/05 11:32a Nashth
 * hw init... reads from data buffer now allow for 8 byte offset.
 * 
 * 22    7/12/05 17:13 Frazzitl
 * Give other threads a time slot.
 * 
 * 21    7/06/05 16:35 Frazzitl
 * Corrected minor problems
 * 
 * 20    7/05/05 11:25 Frazzitl
 * Clarifying code
 * 
 * 19    7/01/05 15:37 Frazzitl
 * Fixed the Tub heater problem
 * 
 * 18    6/29/05 16:12 Frazzitl
 * Improved speed
 * 
 * 17    6/17/05 7:54a Frazzitl
 * 
 * 16    6/07/05 8:25a Frazzitl
 * Separated camera I/O into UV and VIS routines to accommodate individual
 * status checking
 * 
 * 15    6/03/05 11:29 Frazzitl
 * Removed the sleep in read command - Sunita
 * 
 * 14    5/27/05 11:14 Frazzitl
 * 
 * 13    5/27/05 10:30 Frazzitl
 * Corrected problem with sum long and sum data
 * 
 * 12    5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 11    5/26/05 8:26 Frazzitl
 * Temporary
 * 
 * 10    5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 9     5/18/05 9:30 Frazzitl
 * Removed unneeded code
 * 
 * 8     5/16/05 15:47 Frazzitl
 * Added debug feature for logging to a file
 * 
 * 7     5/16/05 16:02 Frazzitl
 * Added code to catch out-of-sync reads
 * 
 * 6     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 5     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 4     4/12/05 10:41a Frazzitl
 * Changed name of output buffer for data and allowed real data conversion
 * instead of constant ramp
 * 
 * 3     4/11/05 8:13 Frazzitl
 * Fixed basic error in "read" command
 * 
 * 2     3/25/05 7:17 Frazzitl
 * Added time to ShortDelay()
 * 
 * 1     3/17/05 11:18 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
