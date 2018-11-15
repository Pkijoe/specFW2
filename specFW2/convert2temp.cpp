//===========================================================================
//
//	Module Name:	convert2temp.cpp
//
//	Function:	Temperature conversion functions.
//				short	channel2temp(short)
//					Returns degrees C scaled by 100 for the specified channel.
//					The channel is specified as:
//						TEMP_TOP_PRI
//						TEMP_TOP_SEC
//						TEMP_BOT_PRI
//						TEMP_BOT_SEC
//					The temperature returned comes from the 10 bit Coarse table (upper 10 bits)
//                  with interpolation done with the lower 2 bits.
//
//	Original Author:	J. Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

short CParserThread::channel2temp(short channel)
{
	unsigned short	temp_index, interp = 0, temperature = 0;
	short			nRetVal = -1;	// Return value

	theApp.EnterCriticalSection1(&m_CriticalSection);  // Protect critical parameters

	if (channel == TEMP_TOP_PRI)
	{
		temp_index = m_TempTopPri;
		interp = m_InterpolateTopPri;
	}
	else if (channel == TEMP_TOP_SEC)
	{
		temp_index = m_TempTopSec;
		interp = m_InterpolateTopSec;
	}
	else if (channel == TEMP_BOT_PRI)
	{
		temp_index = m_TempBotPri;
		interp = m_InterpolateBotPri;
	}
	else if (channel == TEMP_BOT_SEC)
	{
		temp_index = m_TempBotSec;
		interp = m_InterpolateBotSec;
	}
	else
	{	// bad channel
		temp_index	= 0;
		nRetVal	= 0;    
	}

	if (nRetVal != 0)
	{
	   
       if (temp_index == 1023)   // Handle end of table; No interpolation.
	   {
		  temperature = m_nCoarse_table[temp_index]; 
	   }
	   else  // Interpolate
	   {
		  float delta = (float) (m_nCoarse_table[temp_index] - m_nCoarse_table[temp_index + 1]);

		  switch (interp)             // Subrtact the interplated delta since the values decrease as temp_index increases.
		  {
			 case 0: temperature = m_nCoarse_table[temp_index];
			 break;
			 
			 case 1: temperature = (unsigned short) (((float) m_nCoarse_table[temp_index]) - (0.25 * delta));  // 25% interpolation
			 break;			 
			 
			 case 2: temperature = (unsigned short) (((float) m_nCoarse_table[temp_index]) - (0.50 * delta));  // 50% interpolation
			 break;			 

			 case 3: temperature = (unsigned short) (((float) m_nCoarse_table[temp_index]) - (0.75 * delta));  // 75% interpolation
			 break;			 
		  }
	   }   
		
		nRetVal = temperature;
	}
	
	theApp.LeaveCriticalSection1(&m_CriticalSection); // Release protection
	return nRetVal;
}
//===========================================================================

/*** Revision History ***

08/18/16    JO      CBF-37 - Modify code for using 12 bit AD converter for temperature sensors.
09/30/16    JO      CBF-54 - Change heater related names/definitions, fix broken heater commands.

$Log: /IcarusBased/SpecFW/convert2temp.cpp $
 * 
 * 11    5/15/06 11:22a Frazzitl
 * Fixed problem caused by integer arithmetic averaging, and also
 * corrected problem with the display of fime temps in the 3T command.
 * 
 * 10    2/16/06 3:52p Frazzitl
 * Changes conversion from coarse to fine table slightly to increase
 * smoothness.
 * 
 * 9     11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 8     8/10/05 11:41 Frazzitl
 * Removed warning message during compile
 * 
 * 7     8/01/05 3:11p Nashth
 * Protected critical parameters via critical section to allow the HW Init
 * thread to run.  Also, started the HW Init thread.  However the system
 * continues to come up READY.
 * 
 * 6     6/29/05 15:43 Frazzitl
 * Miscellaneous fixes
 * 
 * 5     6/06/05 3:36p Frazzitl
 * Removed warnings during build
 * 
 * 4     5/11/05 11:56 Frazzitl
 * 
 * 3     4/22/05 15:18 Frazzitl
 * Improved Status reads
 * 
 * 2     4/15/05 13:24 Frazzitl
 * Cleaned up unused features from old spectrometer firmware
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
