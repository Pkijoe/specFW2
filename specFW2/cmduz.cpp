//===========================================================================
//
//	Module Name:	cmdUZ.cpp
//
// 	Function:		These functions get the key positions for the shutter.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdUB()  // Get the shutter key radial position
{
	WORD	status(NO_ERRORS), tempw;

	strcpy(m_nDataOutBuf, "UB00xxxx");		// 4 character string
	
 	tempw = m_NvRam.InputW(SHUTTER_POS_RADIAL); 
	c4c2b(tempw, 4);
	
	return status;
}

unsigned int CParserThread::cmdUC()  // Get the shutter key closed position
{
	WORD	status(NO_ERRORS), tempw;

	strcpy(m_nDataOutBuf, "UC00xxx");		// 3 character string

 	tempw = m_NvRam.InputW(SHUTTER_POS_CLOSED);	
	i2b(tempw, 4);

	return status;
}


//===========================================================================

/*** Revision History ***

01/18/16    JO      Initial version for Century B.
01/21/16    JO      Modified toroid function to handle four digit position.
08/02/16    JO      CBF-26 Removed unused toroid commands and references.

** End of Rev History **/
