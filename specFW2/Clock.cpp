//===========================================================================
//
//	Module Name:	Clock.cpp
//
// 	Function:		This routine accesses the hardware clock to obtain date and time information
//
//	Original Author:	T Nash
//
//	Copyright (c) 2007,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "Debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-----------------------------------------------------------------------------
// Sets a char string in szTimeBuf; Format xx:xx:xx when szTimeBuf is other than Null.
//
// Note: The caller MUST allocate at least nine (9) chars for szTimeBuf (If not Null).
//
//  Returns number of seconds on clock from hours minutes and seconds.
//
long CParserThread::GetTimeStamp(char* szTimeBuf)
{
 BYTE	bcdval;
 BYTE	bcd1;
 BYTE	bcd2;
 long	lbcd1;
 long	lbcd2;
 long	lTemp;
 long	lTotal = 0;
 char	szBuf[9];

 strcpy(szBuf, "xx:xx:xx");
 szBuf[8] = NULL;

 EnterCriticalSection(&m_CriticalSectionClock);
 m_NvRam.OutputB(CLKCMD, CLKREAD);
 bcdval = m_NvRam.InputB(CLKHOUR);
 bcd1   = (BYTE) (0x30 & bcdval);
 bcd1 >>= 4;
 lbcd1	= bcd1 * 10;
 bcd1  += ASCII0;							// Current HOUR (msb)
 bcd2   = (BYTE) (0x0f & bcdval);
 lbcd2	= bcd2;
 bcd2  += ASCII0;							// Current HOUR (lsb)
 szBuf[0] = bcd1;
 szBuf[1] = bcd2;
 lTemp  = lbcd1 + lbcd2;
 if (lTemp < 0 || lTemp > 23)
	lTemp = 12;
 lTotal   = lTemp * 3600;
 bcdval = m_NvRam.InputB(CLKMIN);
 bcd1   = (BYTE) (0x70 & bcdval);
 bcd1 >>= 4;
 lbcd1	= bcd1 * 10;
 bcd1  += ASCII0;							// Current MIN  (msb)
 bcd2   = (BYTE) (0x0f & bcdval);
 lbcd2	= bcd2;
 bcd2  += ASCII0;							// Current MIN (lsb)
 szBuf[3] = bcd1;
 szBuf[4] = bcd2;
 lTemp  = lbcd1 + lbcd2;
 if (lTemp < 0 || lTemp > 59)
	lTemp = 0;
 lTotal += lTemp * 60;
 bcdval = m_NvRam.InputB(CLKSEC);
 bcd1   = (BYTE) (0x70 & bcdval);
 bcd1 >>= 4;
 lbcd1	= bcd1 * 10;
 bcd1  += ASCII0;							// Current SEC  (msb)
 bcd2   = (BYTE) (0x0f & bcdval);
 lbcd2	= bcd2;
 bcd2  += ASCII0;							// Current SEC  (lsb)
 szBuf[6] = bcd1;
 szBuf[7] = bcd2;
 lTemp  = lbcd1 + lbcd2;
 if (lTemp < 0 || lTemp > 59)
	lTemp = 0;
 lTotal += lTemp;
 m_NvRam.OutputB(CLKCMD, CLKSTART);
 LeaveCriticalSection(&m_CriticalSectionClock);
 if ( szTimeBuf != NULL )
			 strcpy(szTimeBuf, szBuf);
 return lTotal;
}
//-----------------------------------------------------------------------------
// Sets a char string in szDateTimeBuf; Format x:xx-xx-xx@xx:xx:xx when szTimeBuf is other than Null.
//                                             01234567890123456789
//
// Note: The caller MUST allocate at least twenty (20) chars for szDateTimeBuf (If not Null).
//
void CParserThread::GetDateTimeStamp(char* szDateTimeBuf)
{
	BYTE	bBCD1;
	BYTE	bBCD2;
	BYTE	bBCDval;
	char	szBuf[20];

	strcpy(szBuf, "x:xx-xx-xx@xx:xx:xx");
	szBuf[19] = NULL;

	EnterCriticalSection(&m_CriticalSectionClock);

	// Set the READ bit high before reading clock
	bBCDval = CLKREAD;
	m_NvRam.OutputB(CLKCMD, bBCDval);
	// Now read the clock data
	bBCD1   = (BYTE) (m_NvRam.InputB(CLKDAY) + ASCII0);	// Current DAY (bBCD)
	szBuf[ 0] = bBCD1;
	bBCDval = m_NvRam.InputB(CLKMONTH);			// Current MONTH (bBCD,bBCD)
	bBCD1   = (BYTE) (0x10 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  += ASCII0;							// Current MONTH (msb)
	bBCD2   = (BYTE) (0x0f & bBCDval);
	bBCD2  += ASCII0;							// Current MONTH (lsb)
	szBuf[ 2] = bBCD1;
	szBuf[ 3] = bBCD2;
	bBCDval = m_NvRam.InputB(CLKDATE);			// Current DATE (bBCD,BDC)
	bBCD1   = (BYTE) (0x30 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  += ASCII0;							// Current DATE (msb)
	bBCD2   = (BYTE) (0x0f & bBCDval);
	bBCD2  += ASCII0;							// Current DATE (lsb)
	szBuf[ 5] = bBCD1;
	szBuf[ 6] = bBCD2;
	bBCDval = m_NvRam.InputB(CLKYEAR);			// Current YEAR (bBCD,bBCD)
	bBCD1   = (BYTE) (0xf0 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  += ASCII0;							// Current YEAR (msb)
	bBCD2   = (BYTE) (0x0f & bBCDval);
	bBCD2  += ASCII0;							// Current YEAR (lsb)
	szBuf[ 8] = bBCD1;
	szBuf[ 9] = bBCD2;
	bBCDval = m_NvRam.InputB(CLKHOUR);			// Current HOUR (bBCD,bBCD)
	bBCD1   = (BYTE) (0x30 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  += ASCII0;							// Current HOUR (msb)
	bBCD2   = (BYTE) (0x0f & bBCDval);
	bBCD2  += ASCII0;							// Current HOUR (lsb)
	szBuf[11] = bBCD1;
	szBuf[12] = bBCD2;
	bBCDval = m_NvRam.InputB(CLKMIN);			// Current MINUTE (bBCD,bBCD)
	bBCD1   = (BYTE) (0x70 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  += ASCII0;							// Current MINUTE (msb)
	bBCD2   = (BYTE) (0x0f & bBCDval);
	bBCD2  += ASCII0;							// Current MINUTE (lsb)
	szBuf[14] = bBCD1;
	szBuf[15] = bBCD2;
	bBCDval = m_NvRam.InputB(CLKSEC);			// Current SECOND (bBCD,bBCD)
	bBCD1   = (BYTE) (0x70 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  += ASCII0;							// Current SECOND (msb)
	bBCD2   = (BYTE) (0x0f & bBCDval);
	bBCD2  += ASCII0;							// Current SECOND (lsb)
	szBuf[17] = bBCD1;
	szBuf[18] = bBCD2;
	// Clear the READ bit to allow clock updating
	bBCDval = CLKSTART;
	m_NvRam.OutputB(CLKCMD, bBCDval);

	LeaveCriticalSection(&m_CriticalSectionClock);
	if ( szDateTimeBuf != NULL )
			strcpy(szDateTimeBuf, szBuf);
}
//-----------------------------------------------------------------------------
// Gets the time the system was turned off ... time from clock as stored in BRAM.
//  Stored in GetStat.
//
//  Returns turn off time in seconds (Total clock time saved).
//
//   Also sets member variables:	m_cOFFyear, m_cOFFmonth, m_cOFFdate, m_cOFFhour,
//									m_cOFFminute, m_cOFFsecond
//

long CParserThread::GetOffTime(WORD time_type)
{
	BYTE	bBCD1;
	BYTE	bBCD2;
	BYTE	bBCDval;
	long	lOffTime;

	EnterCriticalSection(&m_CriticalSectionClock);
	
	if (time_type == NORMAL_TIME)
	{
		// Get time system was turned off
		m_cOFFyear   = m_NvRam.InputB(SCLKYEAR);	// Turn OFF YEAR
		m_cOFFmonth  = m_NvRam.InputB(SCLKMONTH);	// Turn OFF MONTH
		m_cOFFdate   = m_NvRam.InputB(SCLKDATE);	// Turn OFF DATE
		m_cOFFhour   = m_NvRam.InputB(SCLKHOUR);	// Turn OFF HOUR
		m_cOFFminute = m_NvRam.InputB(SCLKMIN);		// Turn OFF MINUTE
		m_cOFFsecond = m_NvRam.InputB(SCLKSEC);		// Turn OFF SECOND
	}
	else  // ARGON_TIME
	{
		// Get time system was put into sleep mode for an argon tank change etc.
		m_cOFFyear   = m_NvRam.InputB(ARGON_SCLKYEAR);	// Turn OFF YEAR
		m_cOFFmonth  = m_NvRam.InputB(ARGON_SCLKMONTH);	// Turn OFF MONTH
		m_cOFFdate   = m_NvRam.InputB(ARGON_SCLKDATE);	// Turn OFF DATE
		m_cOFFhour   = m_NvRam.InputB(ARGON_SCLKHOUR);	// Turn OFF HOUR
		m_cOFFminute = m_NvRam.InputB(ARGON_SCLKMIN);		// Turn OFF MINUTE
		m_cOFFsecond = m_NvRam.InputB(ARGON_SCLKSEC);		// Turn OFF SECOND
	}
	
	bBCDval = m_cOFFsecond;
	bBCD1   = (BYTE) (0x70 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  *= 10;
	bBCD2   = (BYTE) (0x0f & bBCDval);
	lOffTime = (long) (bBCD1 + bBCD2);

	bBCDval = m_cOFFminute;
	bBCD1   = (BYTE) (0x70 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  *= 10;
	bBCD2   = (BYTE) (0x0f & bBCDval);
	lOffTime += (long) (bBCD1 + bBCD2) * 60;

	bBCDval = m_cOFFhour;
	bBCD1   = (BYTE) (0x30 & bBCDval);
	bBCD1 >>= 4;
	bBCD1  *= 10;
	bBCD2   = (BYTE) (0x0f & bBCDval);
	lOffTime += (long) (bBCD1 + bBCD2) * 3600;
	LeaveCriticalSection(&m_CriticalSectionClock);  

	return lOffTime;
}
//-----------------------------------------------------------------------------
// Gets the current time from the clock in seconds.
//
//  Returns time in seconds (Current clock time).
//
//   Also sets member variables:	m_cONyear, m_cONmonth, m_cONdate, m_cONhour,
//									m_cONminute, m_cONsecond, and m_lONtime
//

long CParserThread::GetOnTime()
{
	BYTE	bBCD1;
	BYTE	bBCD2;
	BYTE	bBCDval;
	long	lTemp;
	long	lReturnTime; // Return value

	// Protect clock and clock parameters
	EnterCriticalSection(&m_CriticalSectionClock); 

	m_NvRam.OutputB(CLKCMD, CLKREAD);
	m_cONyear   = m_NvRam.InputB(CLKYEAR);
	m_cONmonth  = m_NvRam.InputB(CLKMONTH);
	m_cONdate   = m_NvRam.InputB(CLKDATE);
	m_cONhour   = m_NvRam.InputB(CLKHOUR);
	m_cONminute = m_NvRam.InputB(CLKMIN);
	m_cONsecond = m_NvRam.InputB(CLKSEC);
	m_NvRam.OutputB(CLKCMD, CLKSTART);

	bBCDval = m_cONsecond;
	bBCD1   = (BYTE) ( 0x70 & bBCDval );
	bBCD1 >>= 4;
	bBCD1  *= 10;
	bBCD2   = (BYTE) ( 0x0f & bBCDval );
	lTemp  = (long) ( bBCD1 + bBCD2);
	if (lTemp < 0 || lTemp > 59)
		lTemp = 0;
	lReturnTime = lTemp;

	bBCDval = m_cONminute;
	bBCD1   = (BYTE) ( 0x70 & bBCDval );
	bBCD1 >>= 4;
	bBCD1  *= 10;
	bBCD2   = (BYTE) ( 0x0f & bBCDval );
	lTemp  = (long) ( bBCD1 + bBCD2);
	if (lTemp < 0 || lTemp > 59)
		lTemp = 0;
	lReturnTime += lTemp * 60;

	bBCDval = m_cONhour;
	bBCD1   = (BYTE) ( 0x30 & bBCDval );
	bBCD1 >>= 4;
	bBCD1  *= 10;
	bBCD2   = (BYTE) ( 0x0f & bBCDval );
	lTemp  = (long) ( bBCD1 + bBCD2);
	if (lTemp < 0 || lTemp > 23)
		lTemp = 12;
	lReturnTime += lTemp * 3600;
	m_lONtime = lReturnTime;
	LeaveCriticalSection(&m_CriticalSectionClock); // Release Clock protection

	return lReturnTime;
}
//-----------------------------------------------------------------------------
// Gets the current time from the clock and returns each parameter separately 
// in BYTE parameters.

void CParserThread::GetTimeValues(BYTE& byYear, BYTE& byMonth, BYTE& byDate, BYTE& byHour, BYTE& byMin, BYTE& bySec) 
{
	BYTE	byXYear;
	BYTE	byXMonth;
	BYTE	byXDate;
	BYTE	byXHour;
	BYTE	byXMin;
	BYTE	byXSec;
	EnterCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
	m_NvRam.OutputB(CLKCMD, CLKREAD);	// Enable TE & disable ALL Interrupts
	byXYear		= m_NvRam.InputB(CLKYEAR);
	byXMonth	= m_NvRam.InputB(CLKMONTH);
	byXDate		= m_NvRam.InputB(CLKDATE);
	byXHour		= m_NvRam.InputB(CLKHOUR);
	byXMin		= m_NvRam.InputB(CLKMIN);
	byXSec		 = m_NvRam.InputB(CLKSEC);
	m_NvRam.OutputB(CLKCMD, CLKSTART);	// Disable TE & ALL Interrupts
	byXMonth &= 0x1f;
	byXDate  &= 0x3f;
	byXHour  &= 0x3f;
	byXMin   &= 0x7f;
	byXSec   &= 0x7f;
	LeaveCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters

	byYear = byXYear;
	byMonth = byXMonth;
	byDate = byXDate;
	byHour = byXHour;
	byMin = byXMin;
	bySec = byXSec;
}
//-----------------------------------------------------------------------------
// Gets the current time from the clock and returns each parameter separately 
// in BYTE parameters without masking.

void CParserThread::GetRawTimeValues(BYTE& byYear, BYTE& byMonth, BYTE& byDate, BYTE& byHour, BYTE& byMin, BYTE& bySec) 
{
	EnterCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
	m_NvRam.OutputB(CLKCMD, CLKREAD);	// Enable TE & disable ALL Interrupts
	byYear		= m_NvRam.InputB(CLKYEAR);
	byMonth		= m_NvRam.InputB(CLKMONTH);
	byDate		= m_NvRam.InputB(CLKDATE);
	byHour		= m_NvRam.InputB(CLKHOUR);
	byMin		= m_NvRam.InputB(CLKMIN);
	bySec		 = m_NvRam.InputB(CLKSEC);
	m_NvRam.OutputB(CLKCMD, CLKSTART);	// Disable TE & ALL Interrupts
	LeaveCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
}
//-----------------------------------------------------------------------------
// Gets the wakeup time that was saved in the BRAM 
// into BYTE parameters 

void CParserThread::GetWakeupTimeValues(BYTE& byYearW, BYTE& byMonthW, BYTE& byDateW, BYTE& byHourW, BYTE& byMinW) 
{
	EnterCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
	byYearW		= m_NvRam.InputB(WCLKYEAR);
	byMonthW	= m_NvRam.InputB(WCLKMONTH);
	byDateW		= m_NvRam.InputB(WCLKDATE);
	byHourW		= m_NvRam.InputB(WCLKHOUR);
	byMinW		= m_NvRam.InputB(WCLKMIN);
	LeaveCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
}
//-----------------------------------------------------------------------------
// Saves the wakeup time into the BRAM 
// from BYTE parameters.

void CParserThread::SetWakeupTimeValues(BYTE byYearW, BYTE byMonthW, BYTE byDateW, BYTE byHourW, BYTE byMinW) 
{
	EnterCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
	m_NvRam.OutputB(WCLKYEAR,  byYearW);
	m_NvRam.OutputB(WCLKMONTH, byMonthW);
	m_NvRam.OutputB(WCLKDATE,  byDateW);
	m_NvRam.OutputB(WCLKHOUR,  byHourW);
	m_NvRam.OutputB(WCLKMIN,   byMinW);
	LeaveCriticalSection(&m_CriticalSectionClock); // Protect critical clock parameters
}

//-----------------------------------------------------------------------------
// Stores the current clock time into the BRAM off time area.
//
void CParserThread::SaveOffTime(WORD time_type)
{
	EnterCriticalSection(&m_CriticalSectionClock);
	
	if (time_type == NORMAL_TIME)	// Save to the NORMAL_TIME area
	{
		m_NvRam.OutputB(CLKCMD,		CLKREAD					);
		m_NvRam.OutputB(SCLKDAY,	m_NvRam.InputB(CLKDAY)	);
		m_NvRam.OutputB(SCLKMONTH,	m_NvRam.InputB(CLKMONTH));
		m_NvRam.OutputB(SCLKDATE,	m_NvRam.InputB(CLKDATE)	);
		m_NvRam.OutputB(SCLKYEAR,	m_NvRam.InputB(CLKYEAR)	);
		m_NvRam.OutputB(SCLKHOUR,	m_NvRam.InputB(CLKHOUR)	);
		m_NvRam.OutputB(SCLKMIN,	m_NvRam.InputB(CLKMIN)	);
		m_NvRam.OutputB(SCLKSEC,	m_NvRam.InputB(CLKSEC)	);
		m_NvRam.OutputB(CLKCMD,		CLKSTART				);
	}
	else	// ARGON_TIME - Save to the ARGON_TIME area
	{
		m_NvRam.OutputB(CLKCMD,				CLKREAD					);
		m_NvRam.OutputB(ARGON_SCLKDAY,		m_NvRam.InputB(CLKDAY)	);
		m_NvRam.OutputB(ARGON_SCLKMONTH,	m_NvRam.InputB(CLKMONTH));
		m_NvRam.OutputB(ARGON_SCLKDATE,		m_NvRam.InputB(CLKDATE)	);
		m_NvRam.OutputB(ARGON_SCLKYEAR,		m_NvRam.InputB(CLKYEAR)	);
		m_NvRam.OutputB(ARGON_SCLKHOUR,		m_NvRam.InputB(CLKHOUR)	);
		m_NvRam.OutputB(ARGON_SCLKMIN,		m_NvRam.InputB(CLKMIN)	);
		m_NvRam.OutputB(ARGON_SCLKSEC,		m_NvRam.InputB(CLKSEC)	);
		m_NvRam.OutputB(CLKCMD,				CLKSTART				);
	}

	LeaveCriticalSection(&m_CriticalSectionClock);
}
//-----------------------------------------------------------------------------
// Set the given time into the clock.
//
void CParserThread::SetClockTime(BYTE byDay, BYTE byYear, BYTE byMonth, BYTE byDate, BYTE byHour, BYTE byMin, BYTE bySec)
{
	EnterCriticalSection(&m_CriticalSectionClock);
	// Set the WRITE bit high before writing to clock
	m_NvRam.OutputB(CLKCMD, CLKWRITE);
	// Now write the clock data
	m_NvRam.OutputB(CLKDAY,		byDay);			// Current day of week
	m_NvRam.OutputB(CLKMONTH,   byMonth);		// Current MONTH
	m_NvRam.OutputB(CLKDATE,	byDate);		// Current DATE
	m_NvRam.OutputB(CLKYEAR,	byYear);		// Current YEAR
	m_NvRam.OutputB(CLKHOUR,	byHour);		// Current HOUR
	m_NvRam.OutputB(CLKMIN,		byMin);			// Current MINUTE
	m_NvRam.OutputB(CLKSEC,		bySec);			// Current SECOND
	// Clear the WRITE bit to allow clock updating
	m_NvRam.OutputB(CLKCMD, CLKSTART);
	LeaveCriticalSection(&m_CriticalSectionClock);
}

/***** Revision History *****

01/16/17    JO      CBF-133 - Add cmd_JI functionality.


$Log: /IcarusBased/SpecFW/Clock.cpp $
 * 
 * 2     7/09/07 9:46a Nashth
 * Edited the comments at the top of each function.
 * 
 * 1     7/03/07 3:58p Nashth
  
$NoKeywords: $

***** End of Rev History ****/