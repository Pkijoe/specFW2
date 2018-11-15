//===========================================================================
//
//	Module Name:	MotorTables.cpp
//
// 	Function:		This routine sets all the motor constants used
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "ParserThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CParserThread::ReadMotorTables()
{
	// Slit Motor
	m_nSlitDelta	= 1500;
	m_nSlitHome		=  0;
	m_nSlitHiStp[0]	=  0;	m_nSlitHiStp[1]	=  0;	m_nSlitHiStp[2]	=  5;	m_nSlitHiStp[3]	=  5;
	m_nSlitHiStp[4]	= 10;	m_nSlitHiStp[5]	= 10;	m_nSlitHiStp[6]	= 15;	m_nSlitHiStp[7]	= 15;
	m_nSlitHiStp[8]	= 20;	m_nSlitHiStp[9]	= 30;
	m_nSlitLoStp[0]	=  0;	m_nSlitLoStp[1]	=  5;	m_nSlitLoStp[2]	= 10;	m_nSlitLoStp[3]	= 15;
	m_nSlitLoStp[4]	= 20;	m_nSlitLoStp[5]	= 25;	m_nSlitLoStp[6]	= 30;	m_nSlitLoStp[7]	= 35;
	m_nSlitBdStp	=  0;
	
	// View X & Y Motors

	//=======================================================================
	// Axial View X Motor & X Direction:	1 step = 0.125mm, 1 position = 0.50mm
	// Mid Point:	Posiion 25, Step 50, 0.0mm
	// Range:	-10.0mm to +10.0mm
	//=======================================================================
	m_nViewXStp_Axial[ 0] =  20;	m_nViewXStp_Axial[ 1] =  20;	m_nViewXStp_Axial[ 2] =  20;	m_nViewXStp_Axial[ 3] =  20;	m_nViewXStp_Axial[ 4] =  20;
	m_nViewXStp_Axial[ 5] =  20;	m_nViewXStp_Axial[ 6] =  24;	m_nViewXStp_Axial[ 7] =  28;	m_nViewXStp_Axial[ 8] =  32;	m_nViewXStp_Axial[ 9] =  36;
	m_nViewXStp_Axial[10] =  40;	m_nViewXStp_Axial[11] =  44;	m_nViewXStp_Axial[12] =  48;	m_nViewXStp_Axial[13] =  52;	m_nViewXStp_Axial[14] =  56;
	m_nViewXStp_Axial[15] =  60;	m_nViewXStp_Axial[16] =  64;	m_nViewXStp_Axial[17] =  68;	m_nViewXStp_Axial[18] =  72;	m_nViewXStp_Axial[19] =  76;
	m_nViewXStp_Axial[20] =  80;	m_nViewXStp_Axial[21] =  84;	m_nViewXStp_Axial[22] =  88;	m_nViewXStp_Axial[23] =  92;	m_nViewXStp_Axial[24] =  96;
	m_nViewXStp_Axial[25] = 100;	m_nViewXStp_Axial[26] = 104;	m_nViewXStp_Axial[27] = 108;	m_nViewXStp_Axial[28] = 112;	m_nViewXStp_Axial[29] = 116;
	m_nViewXStp_Axial[30] = 120;	m_nViewXStp_Axial[31] = 124;	m_nViewXStp_Axial[32] = 128;	m_nViewXStp_Axial[33] = 132;	m_nViewXStp_Axial[34] = 136;
	m_nViewXStp_Axial[35] = 140;	m_nViewXStp_Axial[36] = 144;	m_nViewXStp_Axial[37] = 148;	m_nViewXStp_Axial[38] = 152;	m_nViewXStp_Axial[39] = 156;
	m_nViewXStp_Axial[40] = 160;	m_nViewXStp_Axial[41] = 164;	m_nViewXStp_Axial[42] = 168;	m_nViewXStp_Axial[43] = 172;	m_nViewXStp_Axial[44] = 176;
	m_nViewXStp_Axial[45] = 180;	m_nViewXStp_Axial[46] = 180;	m_nViewXStp_Axial[47] = 180;	m_nViewXStp_Axial[48] = 180;	m_nViewXStp_Axial[49] = 180;
	m_nViewXStp_Axial[50] = 180;
	
	//=======================================================================
	// Axial View Y Motor & Y Direction:  1 step = 0.375mm, 1 position = 0.75mm
	// Mid Point:	Posiion 30, Step 60, 15.0mm
	// Range:	0.0mm to +30.0mm
	//=======================================================================
	m_nViewYStp_Axial[ 0] =  20;	m_nViewYStp_Axial[ 1] =  20;	m_nViewYStp_Axial[ 2] =  20;	m_nViewYStp_Axial[ 3] =  20;	m_nViewYStp_Axial[ 4] =  20;
	m_nViewYStp_Axial[ 5] =  20;	m_nViewYStp_Axial[ 6] =  20;	m_nViewYStp_Axial[ 7] =  20;	m_nViewYStp_Axial[ 8] =  20;	m_nViewYStp_Axial[ 9] =  20;
	m_nViewYStp_Axial[10] =  20;	m_nViewYStp_Axial[11] =  22;	m_nViewYStp_Axial[12] =  24;	m_nViewYStp_Axial[13] =  26;	m_nViewYStp_Axial[14] =  28;
	m_nViewYStp_Axial[15] =  30;	m_nViewYStp_Axial[16] =  32;	m_nViewYStp_Axial[17] =  34;	m_nViewYStp_Axial[18] =  36;	m_nViewYStp_Axial[19] =  38;
	m_nViewYStp_Axial[20] =  40;	m_nViewYStp_Axial[21] =  42;	m_nViewYStp_Axial[22] =  44;	m_nViewYStp_Axial[23] =  46;	m_nViewYStp_Axial[24] =  48;
	m_nViewYStp_Axial[25] =  50;	m_nViewYStp_Axial[26] =  52;	m_nViewYStp_Axial[27] =  54;	m_nViewYStp_Axial[28] =  56;	m_nViewYStp_Axial[29] =  58;
	m_nViewYStp_Axial[30] =  60;	m_nViewYStp_Axial[31] =  62;	m_nViewYStp_Axial[32] =  64;	m_nViewYStp_Axial[33] =  66;	m_nViewYStp_Axial[34] =  68;
	m_nViewYStp_Axial[35] =  70;	m_nViewYStp_Axial[36] =  72;	m_nViewYStp_Axial[37] =  74;	m_nViewYStp_Axial[38] =  76;	m_nViewYStp_Axial[39] =  78;
	m_nViewYStp_Axial[40] =  80;	m_nViewYStp_Axial[41] =  82;	m_nViewYStp_Axial[42] =  84;	m_nViewYStp_Axial[43] =  86;	m_nViewYStp_Axial[44] =  88;
	m_nViewYStp_Axial[45] =  90;	m_nViewYStp_Axial[46] =  92;	m_nViewYStp_Axial[47] =  94;	m_nViewYStp_Axial[48] =  96;	m_nViewYStp_Axial[49] =  98;
	m_nViewYStp_Axial[50] = 100;	m_nViewYStp_Axial[51] = 100;	m_nViewYStp_Axial[52] = 100;	m_nViewYStp_Axial[53] = 100;	m_nViewYStp_Axial[54] = 100;
	m_nViewYStp_Axial[55] = 100;	m_nViewYStp_Axial[56] = 100;	m_nViewYStp_Axial[57] = 100;	m_nViewYStp_Axial[58] = 100;	m_nViewYStp_Axial[59] = 100;
	m_nViewYStp_Axial[60] = 100;
	
	//=======================================================================
	// Radial View X Motor & X Direction:	1 half step = 0.061mm, 1 position = 0.244mm
	// Mid Point:	Posiion 25, Step 100, 0.0mm
	// Range:	-6.0mm to +6.0mm
	//=======================================================================

	m_nViewXStp_Radial[0]  = 0;		m_nViewXStp_Radial[1]  = 4;		m_nViewXStp_Radial[2]  = 8;		m_nViewXStp_Radial[3]  = 12;	m_nViewXStp_Radial[4]  = 16;
	m_nViewXStp_Radial[5]  = 20;	m_nViewXStp_Radial[6]  = 24;	m_nViewXStp_Radial[7]  = 28;	m_nViewXStp_Radial[8]  = 32;	m_nViewXStp_Radial[9]  = 36;
	m_nViewXStp_Radial[10] = 40;	m_nViewXStp_Radial[11] = 44;	m_nViewXStp_Radial[12] = 48;	m_nViewXStp_Radial[13] = 52;	m_nViewXStp_Radial[14] = 56;
	m_nViewXStp_Radial[15] = 60;	m_nViewXStp_Radial[16] = 64;	m_nViewXStp_Radial[17] = 68;	m_nViewXStp_Radial[18] = 72;	m_nViewXStp_Radial[19] = 76;
	m_nViewXStp_Radial[20] = 80;	m_nViewXStp_Radial[21] = 84;	m_nViewXStp_Radial[22] = 88;	m_nViewXStp_Radial[23] = 92;	m_nViewXStp_Radial[24] = 96;
	m_nViewXStp_Radial[25] = 100;	m_nViewXStp_Radial[26] = 104;	m_nViewXStp_Radial[27] = 108;	m_nViewXStp_Radial[28] = 112;	m_nViewXStp_Radial[29] = 116;
	m_nViewXStp_Radial[30] = 120;	m_nViewXStp_Radial[31] = 124;	m_nViewXStp_Radial[32] = 128;	m_nViewXStp_Radial[33] = 132;	m_nViewXStp_Radial[34] = 136;
	m_nViewXStp_Radial[35] = 140;	m_nViewXStp_Radial[36] = 144;	m_nViewXStp_Radial[37] = 148;	m_nViewXStp_Radial[38] = 152;	m_nViewXStp_Radial[39] = 156;
	m_nViewXStp_Radial[40] = 160;	m_nViewXStp_Radial[41] = 164;	m_nViewXStp_Radial[42] = 168;	m_nViewXStp_Radial[43] = 172;	m_nViewXStp_Radial[44] = 176;
	m_nViewXStp_Radial[45] = 180;	m_nViewXStp_Radial[46] = 184;	m_nViewXStp_Radial[47] = 188;	m_nViewXStp_Radial[48] = 192;	m_nViewXStp_Radial[49] = 196;
	m_nViewXStp_Radial[50] = 200;	


	//=======================================================================
	// Radial View Y Motor & Y Direction:  1 half step = 0.2mm, 1 position = 0.4mm
	// Mid Point:	Posiion 37, Step 74, 15.0mm
	// Range:	0.0mm to +24.0mm
	//=======================================================================
	m_nViewYStp_Radial[ 0] =  0;	m_nViewYStp_Radial[ 1] =  2;	m_nViewYStp_Radial[ 2] =  4;	m_nViewYStp_Radial[ 3] =  6;	m_nViewYStp_Radial[ 4] =  8;
	m_nViewYStp_Radial[ 5] =  10;	m_nViewYStp_Radial[ 6] =  12;	m_nViewYStp_Radial[ 7] =  14;	m_nViewYStp_Radial[ 8] =  16;	m_nViewYStp_Radial[ 9] =  18;
	m_nViewYStp_Radial[10] =  20;	m_nViewYStp_Radial[11] =  22;	m_nViewYStp_Radial[12] =  24;	m_nViewYStp_Radial[13] =  26;	m_nViewYStp_Radial[14] =  28;
	m_nViewYStp_Radial[15] =  30;	m_nViewYStp_Radial[16] =  32;	m_nViewYStp_Radial[17] =  34;	m_nViewYStp_Radial[18] =  36;	m_nViewYStp_Radial[19] =  38;
	m_nViewYStp_Radial[20] =  40;	m_nViewYStp_Radial[21] =  42;	m_nViewYStp_Radial[22] =  44;	m_nViewYStp_Radial[23] =  46;	m_nViewYStp_Radial[24] =  48;
	m_nViewYStp_Radial[25] =  50;	m_nViewYStp_Radial[26] =  52;	m_nViewYStp_Radial[27] =  54;	m_nViewYStp_Radial[28] =  56;	m_nViewYStp_Radial[29] =  58;
	m_nViewYStp_Radial[30] =  60;	m_nViewYStp_Radial[31] =  62;	m_nViewYStp_Radial[32] =  64;	m_nViewYStp_Radial[33] =  66;	m_nViewYStp_Radial[34] =  68;
	m_nViewYStp_Radial[35] =  70;	m_nViewYStp_Radial[36] =  72;	m_nViewYStp_Radial[37] =  74;	m_nViewYStp_Radial[38] =  76;	m_nViewYStp_Radial[39] =  78;
	m_nViewYStp_Radial[40] =  80;	m_nViewYStp_Radial[41] =  82;	m_nViewYStp_Radial[42] =  84;	m_nViewYStp_Radial[43] =  86;	m_nViewYStp_Radial[44] =  88;
	m_nViewYStp_Radial[45] =  90;	m_nViewYStp_Radial[46] =  92;	m_nViewYStp_Radial[47] =  94;	m_nViewYStp_Radial[48] =  96;	m_nViewYStp_Radial[49] =  98;
	m_nViewYStp_Radial[50] = 100;	m_nViewYStp_Radial[51] = 102;	m_nViewYStp_Radial[52] = 104;	m_nViewYStp_Radial[53] = 106;	m_nViewYStp_Radial[54] = 108;
	m_nViewYStp_Radial[55] = 110;	m_nViewYStp_Radial[56] = 112;	m_nViewYStp_Radial[57] = 114;	m_nViewYStp_Radial[58] = 116;	m_nViewYStp_Radial[59] = 118;
	m_nViewYStp_Radial[60] = 120;	
}
//===========================================================================


/*** Revision History ***

01/05/16    JO      Initial additions and changes for Century B.
06/14/16    JO      CBF-7 - Configure new Radial View X and Y Tables.

$Log: /WinLab/SpecFW/MotorTables.cpp $
 * 
 * 1     4/29/05 8:37 Frazzitl
 * Separated out from CParserThread initialization to simplify readability

$NoKeywords: $

** End of Rev History **/
