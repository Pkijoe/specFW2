//===========================================================================
//
//	Module Name:	cmdQZ.cpp
//
// 	Function:		These functions get/set the Shutter max steps parameters in BRAM.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdQB()  // Get the Shutter max steps
{
	WORD	status(NO_ERRORS), tempw;

	strcpy(m_nDataOutBuf, "QB00xxx");		// 3 character string

 	tempw = m_NvRam.InputW(SHUTTER_POSITION_MAX);	
	i2b(tempw, 4);

	return status;
}

unsigned int CParserThread::cmdBQ()  // Set the Shutter max steps
{
	WORD	status(NO_ERRORS), tempw;

	strcpy(m_nDataOutBuf, "BQ00");		

 	tempw = b2c();  // 3 digit string from buffer
	
	if (tempw >= 1 && tempw <= SHUTTER_MAX_STEPS)		// Set it only if within range
	{
		m_NvRam.OutputW(SHUTTER_POSITION_MAX, tempw);	
		m_Shutter_Pos_Max = tempw;
	}
	
	return status;
}

//===========================================================================

/*** Revision History ***

01/18/16    JO      Initial version for Century B.
01/21/16    JO      Modified toroid function to handle four digit position.
08/02/16    JO      CBF-26 Removed unused toroid commands and references.
02/02/17    JO      CBF-147 Add get/set BRAM commands for maximum shutter position.


** End of Rev History **/
