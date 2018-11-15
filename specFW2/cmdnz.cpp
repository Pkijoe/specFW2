//===========================================================================
//
//	Module Name:	cmdNZ.cpp
//
// 	Function:		These functions get the toroid and shutter current positions.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdNA()  // Get the toroid current position
{
	WORD	status(NO_ERRORS);

	if (m_cToroid_Pos_Status == TOROID_AT_AXIAL_POS)
	  {
	    strcpy(m_nDataOutBuf, "NA000000");		// 4 character string
	  }
	else  // Send out TOROID_AT_RADIAL_POS
	  {
	    
		strcpy(m_nDataOutBuf, "NA000001");		// 4 character string
	  }

	return status;
}

unsigned int CParserThread::cmdNB()  // Get the shutter current position
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "NB00xxx");		// 3 character string

	i2b((WORD) m_nShutter_Pos, 4);

	return status;
}
	
//===========================================================================

/*** Revision History ***

01/18/16    JO      Initial version for Century B.
01/21/16    JO      Modified toroid function to handle four digit position.
07/06/16    JO      Modified the "get toroid current position" command - CBF-17.


** End of Rev History **/
