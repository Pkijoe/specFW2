//===========================================================================
//
//	Module Name:	cmdXZ.cpp
//
// 	Function:		These functions set the key positions for the shutter.
//
//	Original Author:	Joe Orlando
//
//	Copyright (c) 2016,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

unsigned int CParserThread::cmdXB()  // Set the shutter key radial position
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "XB00");		

	m_NvRam.OutputW(SHUTTER_POS_RADIAL, m_nShutter_Pos);
	m_cShutter_Pos_Status = SHUTTER_AT_RADIAL_POS;			// Set the shutter position status
	return status;
}

unsigned int CParserThread::cmdXC()  // Set the shutter key closed position
{
	WORD	status(NO_ERRORS);

	strcpy(m_nDataOutBuf, "XC00");	

	m_NvRam.OutputW(SHUTTER_POS_CLOSED, m_nShutter_Pos);
	m_cShutter_Pos_Status = SHUTTER_AT_CLOSED_POS;			// Set the shutter position status
	return status;
}


//===========================================================================

/*** Revision History ***

01/18/16    JO      Initial version for Century B.
01/26/16    JO      Added setting of the variables m_cShutter_Pos_Status & m_cToroid_Pos_Status.
03/21/16    JO      Restored XA command code to diagostics.  Changed "Set Toroid key radial position"
					command code to XD.
08/02/16    JO      CBF-26 Removed unused toroid commands and references.					

** End of Rev History **/
