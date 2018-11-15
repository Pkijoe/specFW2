//===========================================================================
//
//	Module Name:	cfgMisc.cpp
//
// 	Function:		These routines are used by cfgHdwr that initializes the
//					hardware if it is not in the READY state.
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
//===========================================================================
// Sets current time into m_lCurrentTime ... also returns value of m_lCurrentTime
//
//  m_lCurrentTime includes (hours, minutes, seconds, and optionally days) in units of seconds	
//   Up to one day from the start time day 'm_cONdate' is optionally included when 'flg' is 
//   other than zero. Note: m_lCurrentTime and the return value is set to zero if there is a
//   difference of more than 1 day from m_cONdate.
//

long CParserThread::getTime( int flg )
{									
	long	ltime;

	ltime = GetTimeStamp(NULL);


	// Optionally include the number of days (in seconds) from the "turn on" time
	//  (Note: If the date differs from the "turn on" date by more then 1 day,
	//          m_lCurrentTime and the return time is set to zero.
	EnterCriticalSection(&m_CriticalSectionClock); 
	if ( flg )
	{
		BYTE byDummy;
		BYTE byDate;

		GetRawTimeValues(byDummy, byDummy, byDate, byDummy, byDummy, byDummy); // Get the date 

		if ( byDate != m_cONdate ) // Does date match the On time Date
		{
			if ( ( byDate - m_cONdate ) == 1 )
				ltime  += ( 24 * 3600 );
			else
				ltime = 0;
		}
	}
	m_lCurrentTime = ltime;
	LeaveCriticalSection(&m_CriticalSectionClock); // Release Clock protection

	return ltime;
}
//===========================================================================


void CParserThread::WaitDelay( int nTime )
{
	Sleep(nTime);
}

//=========================================================================================
//	Data locations used by next three routines assuming 4 consecutive reads
//
//  NOTE:  There is an offset of 8 bytes in the data buffer for the command reply
//         and byte count prior to start of data described below.
//
//					Dual Channel	Single Channel		Size
//	Block 1 (Bias):		  0				  0				Block Header:  4 bytes
//	Array 1:			  4				  4				Array Header:  6 bytes
//	Pixel Data			 10				 10				4 * Number of Pixels (always 16)
//	Array 2:			 74				 --				Array Header:  6 bytes
//	Pixel Data:			 80				 --				4 * 16
//
//	Block 2 (DC):		144				 74				Block Header:  4 bytes
//	Array 1:			148				 78				Array Header:  6 bytes
//	Pixel Data:			154				 84				4 * 16
//	Array 2:			218				---				Array Header:  6 bytes
//	Pixel Data:			224				---				4 * 16
//
//	Block 3 (Hg):		288				148				Block Header:  4 bytes
//	Array 1:			292				152				Array Header:  6 bytes
//	Pixel Data:			298				158				4 * 16
//	Array 2:			362				---				Array Header:  6 bytes
//	Pixel Data:			368				---				4 * 16
//
//	Block 4 (Shutter):	432				222				Block Header:  4 bytes
//	Array 1:			436				226				Array Header:  6 bytes
//	Pixel Data:			442				232				4 * 16
//	Array 2:			506				---				Array Header:  6 bytes
//	Pixel Data:			512				---				4 * 16
//
//	End of Data			576				296
//=========================================================================================

unsigned short CParserThread::cal_dc() 
{
	WORD			 wStatus(NO_ERRORS); // Return code
	WORD			 wFlg;
	int				 i;
	unsigned long	 ulVal;
	unsigned long	 aulBIAS[16];  // Bias pixel data buffer
	unsigned long	 aulDC[16];	   // DC pixel data buffer

	// Protect clock and clock parameters
	EnterCriticalSection(&m_CriticalSectionClock); 
	//printf("THN cal_dc()... Entered\r\n");
	wFlg = 0;
	if ( m_bSingleChannel == true ) // Single camera instrument
	{
		batula((BYTE*)&m_nDataBuf[10+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[84+8], &aulDC[0],   16); // Get DC   data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
		}
		printf("cal_dc() UV, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_dc() UV, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);

		// Check ALL pixels (skipping first and last in array) for (DC - Bias <= 2000)
		for ( i = 1; i < 15; i++ )
		{
			if ( aulDC[i] < aulBIAS[i])
					aulDC[i] = 0;
			else
			{
				ulVal = aulDC[i] - aulBIAS[i];
				ulVal >>= 2;
				aulDC[i] = REVDWORD(ulVal);    // Turn the bytes around 
				if ( ulVal > 2000 )
					wFlg = 1;
			}
		}
		ulatba(&aulDC[0], (BYTE*)&m_nDataBuf[84+8],   16); // Return modified DC data to the buffer
		
		printf("cal_dc() UV, Resulting DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    REVDWORD(aulDC[0]), REVDWORD(aulDC[1]), REVDWORD(aulDC[2]), REVDWORD(aulDC[3]),
				REVDWORD(aulDC[4]), REVDWORD(aulDC[5]), REVDWORD(aulDC[6]), REVDWORD(aulDC[7]), 
				REVDWORD(aulDC[8]), REVDWORD(aulDC[9]), REVDWORD(aulDC[10]), REVDWORD(aulDC[11]),
				REVDWORD(aulDC[12]), REVDWORD(aulDC[13]), REVDWORD(aulDC[14]), REVDWORD(aulDC[15]));

	}
	else	// Dual channel instrument
	{
		batula((BYTE*)&m_nDataBuf[ 10+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[154+8], &aulDC[0],   16); // Get DC   data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
		}
		printf("cal_dc() UV, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_dc() UV, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);

		// Check ALL pixels (skipping first and last in array) for (DC - Bias <= 2000)
		for ( i = 1; i < 15; i++ )
		{
			if ( aulDC[i] < aulBIAS[i])
					aulDC[i] = 0;
			else
			{

				ulVal = aulDC[i] - aulBIAS[i];
				ulVal >>= 2;
				aulDC[i] = REVDWORD(ulVal);
				if ( ulVal > 2000 )
					wFlg = 1;
			}
		}
		ulatba(&aulDC[0], (BYTE*)&m_nDataBuf[154+8],   16); // Return modified DC data to the buffer
		printf("cal_dc() UV, Resulting DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulDC[0]), REVDWORD(aulDC[1]), REVDWORD(aulDC[2]), REVDWORD(aulDC[3]),
				REVDWORD(aulDC[4]), REVDWORD(aulDC[5]), REVDWORD(aulDC[6]), REVDWORD(aulDC[7]), 
				REVDWORD(aulDC[8]), REVDWORD(aulDC[9]), REVDWORD(aulDC[10]), REVDWORD(aulDC[11]),
				REVDWORD(aulDC[12]), REVDWORD(aulDC[13]), REVDWORD(aulDC[14]), REVDWORD(aulDC[15]));
			    
		batula((BYTE*)&m_nDataBuf[ 80+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[224+8], &aulDC[0],   16); // Get DC   data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
		}
		printf("cal_dc() VI, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_dc() VI, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);

		// Check ALL pixels (skipping first and last in array) for (DC - Bias <= 2000)
		for ( i = 1; i < 15; i++ )
		{
			if ( aulDC[i] < aulBIAS[i])
					aulDC[i] = 0;
			else
			{

				ulVal = aulDC[i] - aulBIAS[i];
				ulVal >>= 2;
				aulDC[i] = REVDWORD(ulVal);
				if ( ulVal > 2000 )
					if ( wFlg < 2 )  // Only do this once
						wFlg += 2;
			}
		}
		ulatba(&aulDC[0], (BYTE*)&m_nDataBuf[224+8],   16); // Return modified DC data to the buffer
		printf("cal_dc() VI, Resulting DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulDC[0]), REVDWORD(aulDC[1]), REVDWORD(aulDC[2]), REVDWORD(aulDC[3]),
				REVDWORD(aulDC[4]), REVDWORD(aulDC[5]), REVDWORD(aulDC[6]), REVDWORD(aulDC[7]), 
				REVDWORD(aulDC[8]), REVDWORD(aulDC[9]), REVDWORD(aulDC[10]), REVDWORD(aulDC[11]),
				REVDWORD(aulDC[12]), REVDWORD(aulDC[13]), REVDWORD(aulDC[14]), REVDWORD(aulDC[15]));
	}

	if ( wFlg == 1 )
		wStatus = HIGH_UV_DARK_CURRENT;
	else if ( wFlg == 2 )
		wStatus = HIGH_VIS_DARK_CURRENT;
	else if ( wFlg == 3 )
		wStatus = HIGH_DARK_CURRENT;

		// Protect clock and clock parameters
	LeaveCriticalSection(&m_CriticalSectionClock); 
	return wStatus;
}
//===========================================================================
//  BYTE array to unsigned long array
//
//   The destination array must be large enough to hold nCnt number of longs 
//	and the source array must be large enough to contain nCnt*4 bytes.
//
void CParserThread::batula(
						   BYTE*			pbytArray,  // Source ptr to an array of bytes
						   unsigned long*	pulArray,	// Destination ptr to an array of unsigned longs
						   int				nCnt		// Number of unsigned longs to copy 
						   )
{
	memcpy(pulArray, pbytArray, nCnt*4);
}
//===========================================================================
//  unsigned long array to BYTE array
//
//   The destination array must be large enough to hold nCnt*4 number of bytes 
//	and the source array must be large enough to contain nCnt longs.
//
void CParserThread::ulatba(
						   unsigned long*	pulArray,	// Source ptr to an array of unsigned longs
						   BYTE*			pbytArray,  // Destination ptr to an array of bytes
						   int				nCnt		// Number of unsigned longs to copy 
						   )
{
	memcpy(pbytArray, pulArray, nCnt*4);
}
//===========================================================================
const WORD NO_UV_HG	((WORD)1);		// Bit 1: No UV Hg detected	
const WORD NO_VI_HG	((WORD)2);		// Bit 2: No VI Hg Detected

unsigned short CParserThread::cal_peak()
{
	WORD			 wStatus(NO_ERRORS);
	WORD			 wFlg; // Two bit flag indicates state of Hg reading on UV and VI detectors
	unsigned int	 i;
	unsigned long	 ulVal;
	unsigned long	 aulBIAS[16];  // Bias pixel data buffer
	unsigned long	 aulDC[16];	   // DC pixel data buffer
	unsigned long	 aulHg[16];	   // Hg pixel data buffer
	unsigned long	 ulMaxUV = 0;	// Max pix value UV	
	unsigned long	 ulMaxVI = 0;	// Max pix value VIS
	//printf("THN cal_peak()... Entered\r\n");
	// Protect clock and clock parameters
	EnterCriticalSection(&m_CriticalSectionClock); 

	if ( m_bSingleChannel == true )
	{
		wFlg = NO_UV_HG;    // Init UV detector as nothinf detected (There is no VI detector)

		batula((BYTE*)&m_nDataBuf[ 10+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[ 84+8], &aulDC[0],   16); // Get DC   data from buffer
		batula((BYTE*)&m_nDataBuf[158+8], &aulHg[0],   16); // Get Hg   data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
			aulHg[i]   = REVDWORD(aulHg[i]);
		}
		printf("cal_peak() UV, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_peak() UV, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);
		printf("cal_peak() UV, Hg:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulHg[0], aulHg[1], aulHg[2], aulHg[3], aulHg[4], aulHg[5], aulHg[6], aulHg[7], 
				aulHg[8], aulHg[9], aulHg[10], aulHg[11], aulHg[12], aulHg[13], aulHg[14], aulHg[15]);

		// Check for ANY pixel where (Read - DC - Bias > 50000)
		for ( i = 1; i < 15; i++ )
		{
			ulVal   = aulDC[i];
			ulVal >>= 2;			// divide by 4 (250 msec Int time)
			if (aulHg[i] < ulVal) 
				aulHg[i] = 0;
			else
			{
				ulVal = aulHg[i] - ulVal;
				if (ulVal < aulBIAS[i])
					aulHg[i]  = 0;
				else
				{
					ulVal    -= aulBIAS[i];
					aulHg[i]  = REVDWORD(ulVal);
					if (ulVal > 50000)			// Any pixel over 50000 counts is good
					{
						if (wFlg & NO_UV_HG)    // Be certain UV fail bit is off
							wFlg &= ~NO_UV_HG;   

					}
					if ( ulVal > ulMaxUV )		// Save max pixel value
						ulMaxUV = ulVal;
				}
			}
		}
		ulatba(&aulHg[0], (BYTE*)&m_nDataBuf[158+8],   16); // Return modified Hg data to the buffer
		printf("cal_peak() UV, Resulting Hg:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulHg[0]), REVDWORD(aulHg[1]), REVDWORD(aulHg[2]), REVDWORD(aulHg[3]),
				REVDWORD(aulHg[4]), REVDWORD(aulHg[5]), REVDWORD(aulHg[6]), REVDWORD(aulHg[7]), 
				REVDWORD(aulHg[8]), REVDWORD(aulHg[9]), REVDWORD(aulHg[10]), REVDWORD(aulHg[11]),
				REVDWORD(aulHg[12]), REVDWORD(aulDC[13]), REVDWORD(aulDC[14]), REVDWORD(aulHg[15]));

		//...........................................................
		// Hg UV intensity entry placed into the Spectrometer log.
		{
			char* szStr = new char[60];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Hg intensity: UV  %lu\0", ulMaxUV);
			AddToSpecLog(szStr);
		}
		//...........................................................
	}
	else  // Two cameras
	{
		wFlg = NO_UV_HG + NO_VI_HG;  // Init both detectors as nothing detected

		batula((BYTE*)&m_nDataBuf[ 10+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[154+8], &aulDC[0],   16); // Get DC   data from buffer
		batula((BYTE*)&m_nDataBuf[298+8], &aulHg[0],   16); // Get Hg   data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
			aulHg[i]   = REVDWORD(aulHg[i]);
		}
		printf("cal_peak() UV, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_peak() UV, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);
		printf("cal_peak() UV, Hg:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulHg[0], aulHg[1], aulHg[2], aulHg[3], aulHg[4], aulHg[5], aulHg[6], aulHg[7], 
				aulHg[8], aulHg[9], aulHg[10], aulHg[11], aulHg[12], aulHg[13], aulHg[14], aulHg[15]);

		// Check for ANY pixel where (Read - DC - Bias > 50000)
		for ( i = 1; i < 15; i++ )
		{
			ulVal   = aulDC[i];
			ulVal >>= 2;			// divide by 4 (250 msec Int time)
			if (aulHg[i] < ulVal)
				aulHg[i] = 0;
			else
			{
				ulVal = aulHg[i] - ulVal;
				if (ulVal < aulBIAS[i])
					aulHg[i]  = 0;
				else
				{
					ulVal    -= aulBIAS[i];
					aulHg[i]  = REVDWORD(ulVal);
					if (ulVal > 50000)			// Any pixel over 50000 counts is good
					{
						if (wFlg & NO_UV_HG)    // Be certain UV fail bit is off
							wFlg &= ~NO_UV_HG;   
					}
					if ( ulVal > ulMaxUV )		// Save max pixel value
						ulMaxUV = ulVal;
				}
			}
		}
		ulatba(&aulHg[0], (BYTE*)&m_nDataBuf[298+8],   16); // Return modified Hg data to the buffer
		printf("cal_peak() UV, Resulting Hg:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulHg[0]), REVDWORD(aulHg[1]), REVDWORD(aulHg[2]), REVDWORD(aulHg[3]),
				REVDWORD(aulHg[4]), REVDWORD(aulHg[5]), REVDWORD(aulHg[6]), REVDWORD(aulHg[7]), 
				REVDWORD(aulHg[8]), REVDWORD(aulHg[9]), REVDWORD(aulHg[10]), REVDWORD(aulHg[11]),
				REVDWORD(aulHg[12]), REVDWORD(aulDC[13]), REVDWORD(aulDC[14]), REVDWORD(aulHg[15]));


		batula((BYTE*)&m_nDataBuf[ 80+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[224+8], &aulDC[0],   16); // Get DC   data from buffer
		batula((BYTE*)&m_nDataBuf[368+8], &aulHg[0],   16); // Get Hg   data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
			aulHg[i]   = REVDWORD(aulHg[i]);
		}
		printf("cal_peak() VI, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_peak() VI, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);
		printf("cal_peak() VI, Hg:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulHg[0], aulHg[1], aulHg[2], aulHg[3], aulHg[4], aulHg[5], aulHg[6], aulHg[7], 
				aulHg[8], aulHg[9], aulHg[10], aulHg[11], aulHg[12], aulHg[13], aulHg[14], aulHg[15]);

		// Check for ANY pixel where (Read - DC - Bias > 50000)
		for ( i = 1; i < 15; i++ )
		{
			ulVal   = aulDC[i];
			ulVal >>= 2;
			if (aulHg[i] < ulVal)
				aulHg[i] = 0;
			else
			{
				ulVal = aulHg[i] - ulVal;
				if (ulVal < aulBIAS[i])
					aulHg[i] = 0;
				else
				{
					ulVal    -= aulBIAS[i];
					aulHg[i]  = REVDWORD(ulVal);
					if (ulVal > 10000)			// Any pixel over 10000 counts is good
					{
						if (wFlg & NO_VI_HG)    // Be certain VI fail bit is off
							wFlg &= ~NO_VI_HG;   
					}
					if ( ulVal > ulMaxVI )		// Save max pixel value
						ulMaxVI = ulVal;
				}
			}
		}
		ulatba(&aulHg[0], (BYTE*)&m_nDataBuf[368+8],   16); // Return modified Hg data to the buffer
		printf("cal_peak() VI, Resulting Hg:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulHg[0]), REVDWORD(aulHg[1]), REVDWORD(aulHg[2]), REVDWORD(aulHg[3]),
				REVDWORD(aulHg[4]), REVDWORD(aulHg[5]), REVDWORD(aulHg[6]), REVDWORD(aulHg[7]), 
				REVDWORD(aulHg[8]), REVDWORD(aulHg[9]), REVDWORD(aulHg[10]), REVDWORD(aulHg[11]),
				REVDWORD(aulHg[12]), REVDWORD(aulHg[13]), REVDWORD(aulHg[14]), REVDWORD(aulHg[15]));
		//...........................................................
		// Hg UV intensity entry placed into the Spectrometer log.
		{
			char* szStr = new char[60];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Hg intensity: UV  %lu\0", ulMaxUV);
			AddToSpecLog(szStr);
		}
		//...........................................................
		//...........................................................
		// Hg VIS intensity entry placed into the Spectrometer log.
		{
			char* szStr = new char[60];
			GetDateTimeStamp(szStr);
			sprintf(&szStr[19], " Hg intensity: VIS %lu\0", ulMaxVI);
			AddToSpecLog(szStr);
		}
		//...........................................................
	}
	
	if      (wFlg == (NO_UV_HG | NO_VI_HG))
				wStatus = NO_MERCURY_LAMP;
	else if (wFlg == NO_UV_HG)
				wStatus = NO_UV_MERCURY_LAMP;
	else if (wFlg == NO_VI_HG)
				wStatus = NO_VIS_MERCURY_LAMP;
	// Protect clock and clock parameters
	LeaveCriticalSection(&m_CriticalSectionClock); 
	return wStatus;
}
//===========================================================================

unsigned short CParserThread::cal_open()
{
	WORD			 wStatus(NO_ERRORS);
	WORD			 wFlg; // Two bit flag indicates state of Shutter opening for UV and VI detectors
						   // 1=Bad UV/2=Bad VI/3=Bad both/0=AOK 
	unsigned int	 i;
	unsigned long	 ulVal;
	unsigned long	 aulBIAS[16];  // Bias pixel data buffer
	unsigned long	 aulDC[16];	   // DC pixel data buffer
	unsigned long	 aulSh[16];	   // Shutter pixel data buffer

	//printf("THN cal_open()... Entered\r\n");
		// Protect clock and clock parameters
	EnterCriticalSection(&m_CriticalSectionClock); 
	wFlg = 0;
	if ( m_bSingleChannel == true )
	{
		batula((BYTE*)&m_nDataBuf[ 10+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[ 84+8], &aulDC[0],   16); // Get DC   data from buffer
		batula((BYTE*)&m_nDataBuf[232+8], &aulSh[0],   16); // Get Shutter data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
			aulSh[i]   = REVDWORD(aulSh[i]);
		}
		printf("cal_open() UV, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_open() UV, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);
		printf("cal_open() UV, Shutter:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulSh[0], aulSh[1], aulSh[2], aulSh[3], aulSh[4], aulSh[5], aulSh[6], aulSh[7], 
				aulSh[8], aulSh[9], aulSh[10], aulSh[11], aulSh[12], aulSh[13], aulSh[14], aulSh[15]);

		// Check ALL pixels for (Read - DC - Bias <= 10000)
		for (i = 1; i < 15; i++)
		{
			ulVal   = aulDC[i];
			ulVal >>= 2;			// divide by 4 (250 msec Int time)
			if (aulSh[i] < ulVal)
				aulSh[i] = 0;
			else
			{
				ulVal = aulSh[i] - ulVal;
				if (ulVal < aulBIAS[i])
					aulSh[i] = 0;
				else
				{
					ulVal    -= aulBIAS[i];
					aulSh[i]  = REVDWORD(ulVal);
					if (ulVal > 10000)
						wFlg = 1;
				}
			}
		}
		ulatba(&aulSh[0], (BYTE*)&m_nDataBuf[232+8],   16); // Return modified Shutter data to the buffer
		printf("cal_open() UV, Resulting Shutter:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulSh[0]), REVDWORD(aulSh[1]), REVDWORD(aulSh[2]), REVDWORD(aulSh[3]),
				REVDWORD(aulSh[4]), REVDWORD(aulSh[5]), REVDWORD(aulSh[6]), REVDWORD(aulSh[7]), 
				REVDWORD(aulSh[8]), REVDWORD(aulSh[9]), REVDWORD(aulSh[10]), REVDWORD(aulSh[11]),
				REVDWORD(aulSh[12]), REVDWORD(aulSh[13]), REVDWORD(aulSh[14]), REVDWORD(aulSh[15]));
	}
	else  // Dual channel... Two detectors
	{
		batula((BYTE*)&m_nDataBuf[ 10+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[154+8], &aulDC[0],   16); // Get DC   data from buffer
		batula((BYTE*)&m_nDataBuf[442+8], &aulSh[0],   16); // Get Shutter data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
			aulSh[i]   = REVDWORD(aulSh[i]);
		}
		printf("cal_open() UV, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_open() UV, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);
		printf("cal_open() UV, Shutter:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulSh[0], aulSh[1], aulSh[2], aulSh[3], aulSh[4], aulSh[5], aulSh[6], aulSh[7], 
				aulSh[8], aulSh[9], aulSh[10], aulSh[11], aulSh[12], aulSh[13], aulSh[14], aulSh[15]);

		// Check ALL pixels for (Read - DC - Bias <= 10000)
		for (i = 1; i < 15; i++)
		{
			ulVal   = aulDC[i];
			ulVal >>= 2;			// divide by 4 (250 msec Int time)
			if (aulSh[i] < ulVal)
				aulSh[i] = 0;
			else
			{
				aulDC[i];
				ulVal = aulSh[i] - ulVal;
				if (ulVal < aulBIAS[i])
					aulSh[i]  = 0;
				else
				{
					ulVal    -= aulBIAS[i];
					aulSh[i]  = REVDWORD(ulVal);
					if (ulVal > 10000)
						wFlg = 1;
				}
			}
		}
		ulatba(&aulSh[0], (BYTE*)&m_nDataBuf[442+8],   16); // Return modified Shutter data to the buffer
		printf("cal_open() UV, Resulting Shutter:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulSh[0]), REVDWORD(aulSh[1]), REVDWORD(aulSh[2]), REVDWORD(aulSh[3]),
				REVDWORD(aulSh[4]), REVDWORD(aulSh[5]), REVDWORD(aulSh[6]), REVDWORD(aulSh[7]), 
				REVDWORD(aulSh[8]), REVDWORD(aulSh[9]), REVDWORD(aulSh[10]), REVDWORD(aulSh[11]),
				REVDWORD(aulSh[12]), REVDWORD(aulSh[13]), REVDWORD(aulSh[14]), REVDWORD(aulSh[15]));
			    
		batula((BYTE*)&m_nDataBuf[ 80+8], &aulBIAS[0], 16); // Get BIAS data from buffer
		batula((BYTE*)&m_nDataBuf[224+8], &aulDC[0],   16); // Get DC   data from buffer
		batula((BYTE*)&m_nDataBuf[512+8], &aulSh[0],   16); // Get Shutter data from buffer
		for ( i = 0; i < 16; i++ )
		{
			aulDC[i]   = REVDWORD(aulDC[i]);    // Turn the bytes around
			aulBIAS[i] = REVDWORD(aulBIAS[i]);
			aulSh[i]   = REVDWORD(aulSh[i]);
		}
		printf("cal_open() VI, BIAS:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulBIAS[0], aulBIAS[1], aulBIAS[2], aulBIAS[3], aulBIAS[4], aulBIAS[5], aulBIAS[6], aulBIAS[7], 
				aulBIAS[8], aulBIAS[9], aulBIAS[10], aulBIAS[11], aulBIAS[12], aulBIAS[13], aulBIAS[14], aulBIAS[15]);
		printf("cal_open() VI, DC:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulDC[0], aulDC[1], aulDC[2], aulDC[3], aulDC[4], aulDC[5], aulDC[6], aulDC[7], 
				aulDC[8], aulDC[9], aulDC[10], aulDC[11], aulDC[12], aulDC[13], aulDC[14], aulDC[15]);
		printf("cal_open() VI, Shutter:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
			    aulSh[0], aulSh[1], aulSh[2], aulSh[3], aulSh[4], aulSh[5], aulSh[6], aulSh[7], 
				aulSh[8], aulSh[9], aulSh[10], aulSh[11], aulSh[12], aulSh[13], aulSh[14], aulSh[15]);

		// Check ALL pixels for (Read - DC - Bias <= 10000)
		for ( i = 1; i < 15; i++ )
		{
			ulVal   = aulDC[i];
			ulVal >>= 2;
			if (aulSh[i] < ulVal)
				aulSh[i] = 0;
			else
			{
				aulDC[i];
				ulVal = aulSh[i] - ulVal;
				if (ulVal < aulBIAS[i])
					aulSh[i]  = 0;
				else
				{
					ulVal    -= aulBIAS[i];
					aulSh[i]  = REVDWORD(ulVal);
					if (ulVal > 10000)
					{
						if (wFlg  < 2)
							wFlg += 2;
					}
				}
			}
		}
		ulatba(&aulSh[0], (BYTE*)&m_nDataBuf[512+8],   16); // Return modified Shutter data to the buffer
		printf("cal_open() VI, Resulting Shutter:\r\n %u %u %u %u %u %u %u %u\r\n %u %u %u %u %u %u %u %u\r\n",
				REVDWORD(aulSh[0]), REVDWORD(aulSh[1]), REVDWORD(aulSh[2]), REVDWORD(aulSh[3]),
				REVDWORD(aulSh[4]), REVDWORD(aulSh[5]), REVDWORD(aulSh[6]), REVDWORD(aulSh[7]), 
				REVDWORD(aulSh[8]), REVDWORD(aulSh[9]), REVDWORD(aulSh[10]), REVDWORD(aulSh[11]),
				REVDWORD(aulSh[12]), REVDWORD(aulSh[13]), REVDWORD(aulSh[14]), REVDWORD(aulSh[15]));    
	}

	if (wFlg == 1)
		wStatus = UV_SHUTTER_FAILURE;
	else if (wFlg == 2)
		wStatus = VIS_SHUTTER_FAILURE;
	else if (wFlg == 3)
		wStatus = SHUTTER_FAILURE;

	// Protect clock and clock parameters
	LeaveCriticalSection(&m_CriticalSectionClock); 
	return wStatus;
}
//===========================================================================

/*** Revision History ***

10/18/16    JO      CBF-54 - Change heater related names/definitions, fix broken heater commands.


$Log: /IcarusBased/SpecFW/cfgmisc.cpp $
 * 
 * 22    4/07/08 10:41a Nashth
 * Second attempt at Spectrometer history log.
 * 
 * 21    3/31/08 5:31p Nashth
 * Implementation of spectrometer history log... First pass.
 * 
 * 20    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 19    12/02/05 9:59a Frazzitl
 * Corrected problem with GT (Get Time to Ready) command.
 * 
 * 18    11/29/05 5:13p Nashth
 * Commented out some printfs used for debug.
 * 
 * 17    11/29/05 11:35a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 16    11/22/05 10:10a Nashth
 * Added some Printf statements for debugging... Some with THN within the
 * Print Text... These can eventually be removed or made debug flag
 * optional.
 * 
 * 15    8/24/05 11:32a Nashth
 * hw init... reads from data buffer now allow for 8 byte offset.
 * 
 * 14    8/24/05 8:49a Nashth
 * Most recent updates for hwinit.
 * 
 * 13    8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 12    7/20/05 2:48p Nashth
 * Added a few comments and removed some dead code.
 * 
 * 11    7/18/05 14:41 Frazzitl
 * Implemented changes required to read and check data during hardware
 * initialization
 * 
 * 10    7/08/05 9:57 Frazzitl
 * Fixed problem with system clock updating.
 * 
 * 9     6/29/05 16:03 Frazzitl
 * Minor changes
 * 
 * 8     5/26/05 16:07 Frazzitl
 * Implemented delay in Camera IO read/write routines
 * 
 * 7     5/17/05 13:51 Frazzitl
 * Implemented delay routine
 * 
 * 6     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 5     4/20/05 11:27 Frazzitl
 * Made more easily readable
 * 
 * 4     4/15/05 13:23 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 3     4/07/05 1:32p Frazzitl
 * Fixed problem with clock
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:11 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
