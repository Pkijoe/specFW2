//===========================================================================
//
//	Module Name:	cmd3T.cpp
//
// 	Function:		This routine gets the temperatures from the four thermisters.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2017,  PerkinElmer, DAS. All rights reserved.
//
//===========================================================================


#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"

unsigned int CParserThread::cmd3T()
{
	WORD	status(NO_ERRORS);

/*	0         1         2         3         4         5         6
    01234567890123456789012345678901234567890123456789012345678901234567890
    3T00 TT.XX, TT.XX, TT.XX, TT.XX, TT.XX,TXXX,LXXX,TP0,BP0,TS0,BS0
*/

	strcpy(m_nDataOutBuf, "3T00");
	
	float tub_top_primary_temp		= (float) channel2temp(TEMP_TOP_PRI) / 100.0f;
	float tub_bottom_primary_temp	= (float) channel2temp(TEMP_BOT_PRI) / 100.0f;
	float tub_top_secondary_temp	= (float) channel2temp(TEMP_TOP_SEC) / 100.0f;
	float tub_bottom_secondary_temp	= (float) channel2temp(TEMP_BOT_SEC) / 100.0f;	
	float tub_weighted_temp = Get_Tub_Temp();

	sprintf(&m_nDataOutBuf[4],  "%6.2f,", tub_top_primary_temp);	
	sprintf(&m_nDataOutBuf[11], "%6.2f,", tub_bottom_primary_temp);
	sprintf(&m_nDataOutBuf[18], "%6.2f,", tub_top_secondary_temp);	
	sprintf(&m_nDataOutBuf[25], "%6.2f,", tub_bottom_secondary_temp);
	sprintf(&m_nDataOutBuf[32], "%6.2f,", tub_weighted_temp);
	sprintf(&m_nDataOutBuf[39], "T%3ld,", m_lPI_out);		// Tub Power 
	sprintf(&m_nDataOutBuf[44], "L%3ld,", m_lPI_out2);		// Lid Power

	if(m_Sensor_Top_Primary_Status == 0)		// Determine no failure
	{
		sprintf(&m_nDataOutBuf[49], "TP1,");	// Status==0 indicates GOOD sensor 
	}
	else
	{
		sprintf(&m_nDataOutBuf[49], "TP0,");	// Status==1 indicates NO GOOD sensor
	}

	if(m_Sensor_Bottom_Primary_Status == 0)		// Determine no failure
	{
		sprintf(&m_nDataOutBuf[53], "BP1,");	// Status==0 indicates GOOD sensor
	}
	else
	{
		sprintf(&m_nDataOutBuf[53], "BP0,");	// Status==1 indicates NO GOOD sensor
	}

	if(m_Sensor_Top_Secondary_Status == 0)		// Determine no failure
	{
		sprintf(&m_nDataOutBuf[57], "TS1,");	// Status==0 indicates GOOD sensor
	}
	else
	{
		sprintf(&m_nDataOutBuf[57], "TS0,");	// Status==1 indicates NO GOOD sensor
	}
	
	if(m_Sensor_Bottom_Secondary_Status == 0)	// Determine no failure
	{
		sprintf(&m_nDataOutBuf[61], "BS1,");	// Status==0 indicates GOOD sensor
	}
	else
	{
		sprintf(&m_nDataOutBuf[61], "BS0,");	// Status==1 indicates NO GOOD sensor
	}


	return status;
}
//===========================================================================

/*** Revision History ***

10/04/16    JO      CBF-54 - Change heater related names/definitions, fix broken heater commands.
10/26/16	KR		CBF-69 - Add changes to Syngistix release 3.0
11/29/16	KR		CBF-94 - Add tub and lid power to cmd3T
01/09/17    KR      CBF-126 - Create get/set commands for active sensors
01/25/17	KR		CBF-138 - Change the sensor status indicator to 1 for good and 0 for bad 

** End of Rev History **/
