//===========================================================================
//
//	Module Name:	TECoolerTemperatureTable.cpp
//
// 	Function:		This routine is used for TE Cooler temperature conversion
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/TECoolerTemperatureTable.cpp 2     5/13/05 10:58 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::ReadTECoolerTemperatureTable()
{
	m_nTECoolerTemp[  0] = -4885;
	m_nTECoolerTemp[  1] = -4833;
	m_nTECoolerTemp[  2] = -4782;
	m_nTECoolerTemp[  3] = -4731;
	m_nTECoolerTemp[  4] = -4679;
	m_nTECoolerTemp[  5] = -4628;
	m_nTECoolerTemp[  6] = -4576;
	m_nTECoolerTemp[  7] = -4525;
	m_nTECoolerTemp[  8] = -4474;
	m_nTECoolerTemp[  9] = -4422;
	m_nTECoolerTemp[ 10] = -4371;
	m_nTECoolerTemp[ 11] = -4320;
	m_nTECoolerTemp[ 12] = -4268;
	m_nTECoolerTemp[ 13] = -4217;
	m_nTECoolerTemp[ 14] = -4165;
	m_nTECoolerTemp[ 15] = -4114;
	m_nTECoolerTemp[ 16] = -4063;
	m_nTECoolerTemp[ 17] = -4011;
	m_nTECoolerTemp[ 18] = -3960;
	m_nTECoolerTemp[ 19] = -3908;
	m_nTECoolerTemp[ 20] = -3857;
	m_nTECoolerTemp[ 21] = -3806;
	m_nTECoolerTemp[ 22] = -3754;
	m_nTECoolerTemp[ 23] = -3703;
	m_nTECoolerTemp[ 24] = -3652;
	m_nTECoolerTemp[ 25] = -3600;
	m_nTECoolerTemp[ 26] = -3549;
	m_nTECoolerTemp[ 27] = -3497;
	m_nTECoolerTemp[ 28] = -3446;
	m_nTECoolerTemp[ 29] = -3395;
	m_nTECoolerTemp[ 30] = -3343;
	m_nTECoolerTemp[ 31] = -3292;
	m_nTECoolerTemp[ 32] = -3241;
	m_nTECoolerTemp[ 33] = -3189;
	m_nTECoolerTemp[ 34] = -3138;
	m_nTECoolerTemp[ 35] = -3086;
	m_nTECoolerTemp[ 36] = -3035;
	m_nTECoolerTemp[ 37] = -2984;
	m_nTECoolerTemp[ 38] = -2932;
	m_nTECoolerTemp[ 39] = -2881;
	m_nTECoolerTemp[ 40] = -2830;
	m_nTECoolerTemp[ 41] = -2778;
	m_nTECoolerTemp[ 42] = -2727;
	m_nTECoolerTemp[ 43] = -2675;
	m_nTECoolerTemp[ 44] = -2624;
	m_nTECoolerTemp[ 45] = -2573;
	m_nTECoolerTemp[ 46] = -2521;
	m_nTECoolerTemp[ 47] = -2470;
	m_nTECoolerTemp[ 48] = -2418;
	m_nTECoolerTemp[ 49] = -2367;
	m_nTECoolerTemp[ 50] = -2316;
	m_nTECoolerTemp[ 51] = -2264;
	m_nTECoolerTemp[ 52] = -2213;
	m_nTECoolerTemp[ 53] = -2162;
	m_nTECoolerTemp[ 54] = -2110;
	m_nTECoolerTemp[ 55] = -2059;
	m_nTECoolerTemp[ 56] = -2007;
	m_nTECoolerTemp[ 57] = -1956;
	m_nTECoolerTemp[ 58] = -1905;
	m_nTECoolerTemp[ 59] = -1853;
	m_nTECoolerTemp[ 60] = -1802;
	m_nTECoolerTemp[ 61] = -1751;
	m_nTECoolerTemp[ 62] = -1699;
	m_nTECoolerTemp[ 63] = -1648;
	m_nTECoolerTemp[ 64] = -1596;
	m_nTECoolerTemp[ 65] = -1545;
	m_nTECoolerTemp[ 66] = -1494;
	m_nTECoolerTemp[ 67] = -1442;
	m_nTECoolerTemp[ 68] = -1391;
	m_nTECoolerTemp[ 69] = -1340;
	m_nTECoolerTemp[ 70] = -1288;
	m_nTECoolerTemp[ 71] = -1237;
	m_nTECoolerTemp[ 72] = -1185;
	m_nTECoolerTemp[ 73] = -1134;
	m_nTECoolerTemp[ 74] = -1083;
	m_nTECoolerTemp[ 75] = -1031;
	m_nTECoolerTemp[ 76] =  -980;
	m_nTECoolerTemp[ 77] =  -929;
	m_nTECoolerTemp[ 78] =  -877;
	m_nTECoolerTemp[ 79] =  -826;
	m_nTECoolerTemp[ 80] =  -774;
	m_nTECoolerTemp[ 81] =  -723;
	m_nTECoolerTemp[ 82] =  -672;
	m_nTECoolerTemp[ 83] =  -620;
	m_nTECoolerTemp[ 84] =  -569;
	m_nTECoolerTemp[ 85] =  -517;
	m_nTECoolerTemp[ 86] =  -466;
	m_nTECoolerTemp[ 87] =  -415;
	m_nTECoolerTemp[ 88] =  -363;
	m_nTECoolerTemp[ 89] =  -312;
	m_nTECoolerTemp[ 90] =  -261;
	m_nTECoolerTemp[ 91] =  -209;
	m_nTECoolerTemp[ 92] =  -158;
	m_nTECoolerTemp[ 93] =  -106;
	m_nTECoolerTemp[ 94] =   -55;
	m_nTECoolerTemp[ 95] =    -4;
	m_nTECoolerTemp[ 96] =    48;
	m_nTECoolerTemp[ 97] =    99;
	m_nTECoolerTemp[ 98] =   150;
	m_nTECoolerTemp[ 99] =   202;
	m_nTECoolerTemp[100] =   253;
	m_nTECoolerTemp[101] =   305;
	m_nTECoolerTemp[102] =   356;
	m_nTECoolerTemp[103] =   407;
	m_nTECoolerTemp[104] =   459;
	m_nTECoolerTemp[105] =   510;
	m_nTECoolerTemp[106] =   561;
	m_nTECoolerTemp[107] =   613;
	m_nTECoolerTemp[108] =   664;
	m_nTECoolerTemp[109] =   716;
	m_nTECoolerTemp[110] =   767;
	m_nTECoolerTemp[111] =   818;
	m_nTECoolerTemp[112] =   870;
	m_nTECoolerTemp[113] =   921;
	m_nTECoolerTemp[114] =   973;
	m_nTECoolerTemp[115] =  1024;
	m_nTECoolerTemp[116] =  1075;
	m_nTECoolerTemp[117] =  1127;
	m_nTECoolerTemp[118] =  1178;
	m_nTECoolerTemp[119] =  1229;
	m_nTECoolerTemp[120] =  1281;
	m_nTECoolerTemp[121] =  1332;
	m_nTECoolerTemp[122] =  1384;
	m_nTECoolerTemp[123] =  1435;
	m_nTECoolerTemp[124] =  1486;
	m_nTECoolerTemp[125] =  1538;
	m_nTECoolerTemp[126] =  1589;
	m_nTECoolerTemp[127] =  1640;
	m_nTECoolerTemp[128] =  1692;
	m_nTECoolerTemp[129] =  1743;
	m_nTECoolerTemp[130] =  1795;
	m_nTECoolerTemp[131] =  1846;
	m_nTECoolerTemp[132] =  1897;
	m_nTECoolerTemp[133] =  1949;
	m_nTECoolerTemp[134] =  2000;
	m_nTECoolerTemp[135] =  2051;
	m_nTECoolerTemp[136] =  2103;
	m_nTECoolerTemp[137] =  2154;
	m_nTECoolerTemp[138] =  2206;
	m_nTECoolerTemp[139] =  2257;
	m_nTECoolerTemp[140] =  2308;
	m_nTECoolerTemp[141] =  2360;
	m_nTECoolerTemp[142] =  2411;
	m_nTECoolerTemp[143] =  2462;
	m_nTECoolerTemp[144] =  2514;
	m_nTECoolerTemp[145] =  2565;
	m_nTECoolerTemp[146] =  2617;
	m_nTECoolerTemp[147] =  2668;
	m_nTECoolerTemp[148] =  2719;
	m_nTECoolerTemp[149] =  2771;
	m_nTECoolerTemp[150] =  2822;
	m_nTECoolerTemp[151] =  2874;
	m_nTECoolerTemp[152] =  2925;
	m_nTECoolerTemp[153] =  2976;
	m_nTECoolerTemp[154] =  3028;
	m_nTECoolerTemp[155] =  3079;
	m_nTECoolerTemp[156] =  3130;
	m_nTECoolerTemp[157] =  3182;
	m_nTECoolerTemp[158] =  3233;
	m_nTECoolerTemp[159] =  3285;
	m_nTECoolerTemp[160] =  3336;
	m_nTECoolerTemp[161] =  3387;
	m_nTECoolerTemp[162] =  3439;
	m_nTECoolerTemp[163] =  3490;
	m_nTECoolerTemp[164] =  3541;
	m_nTECoolerTemp[165] =  3593;
	m_nTECoolerTemp[166] =  3644;
	m_nTECoolerTemp[167] =  3696;
	m_nTECoolerTemp[168] =  3747;
	m_nTECoolerTemp[169] =  3798;
	m_nTECoolerTemp[170] =  3850;
	m_nTECoolerTemp[171] =  3901;
	m_nTECoolerTemp[172] =  3952;
	m_nTECoolerTemp[173] =  4004;
	m_nTECoolerTemp[174] =  4055;
	m_nTECoolerTemp[175] =  4107;
	m_nTECoolerTemp[176] =  4158;
	m_nTECoolerTemp[177] =  4209;
	m_nTECoolerTemp[178] =  4261;
	m_nTECoolerTemp[179] =  4312;
	m_nTECoolerTemp[180] =  4364;
	m_nTECoolerTemp[181] =  4415;
	m_nTECoolerTemp[182] =  4466;
	m_nTECoolerTemp[183] =  4518;
	m_nTECoolerTemp[184] =  4569;
	m_nTECoolerTemp[185] =  4620;
	m_nTECoolerTemp[186] =  4672;
	m_nTECoolerTemp[187] =  4723;
	m_nTECoolerTemp[188] =  4775;
	m_nTECoolerTemp[189] =  4826;
	m_nTECoolerTemp[190] =  4877;
	m_nTECoolerTemp[191] =  4929;
	m_nTECoolerTemp[192] =  4980;
	m_nTECoolerTemp[193] =  5031;
	m_nTECoolerTemp[194] =  5083;
	m_nTECoolerTemp[195] =  5134;
	m_nTECoolerTemp[196] =  5186;
	m_nTECoolerTemp[197] =  5237;
	m_nTECoolerTemp[198] =  5288;
	m_nTECoolerTemp[199] =  5340;
	m_nTECoolerTemp[200] =  5391;
	m_nTECoolerTemp[201] =  5442;
	m_nTECoolerTemp[202] =  5494;
	m_nTECoolerTemp[203] =  5545;
	m_nTECoolerTemp[204] =  5597;
	m_nTECoolerTemp[205] =  5648;
	m_nTECoolerTemp[206] =  5699;
	m_nTECoolerTemp[207] =  5751;
	m_nTECoolerTemp[208] =  5802;
	m_nTECoolerTemp[209] =  5853;
	m_nTECoolerTemp[210] =  5905;
	m_nTECoolerTemp[211] =  5956;
	m_nTECoolerTemp[212] =  6008;
	m_nTECoolerTemp[213] =  6059;
	m_nTECoolerTemp[214] =  6110;
	m_nTECoolerTemp[215] =  6162;
	m_nTECoolerTemp[216] =  6213;
	m_nTECoolerTemp[217] =  6265;
	m_nTECoolerTemp[218] =  6316;
	m_nTECoolerTemp[219] =  6367;
	m_nTECoolerTemp[220] =  6419;
	m_nTECoolerTemp[221] =  6470;
	m_nTECoolerTemp[222] =  6521;
	m_nTECoolerTemp[223] =  6573;
	m_nTECoolerTemp[224] =  6624;
	m_nTECoolerTemp[225] =  6676;
	m_nTECoolerTemp[226] =  6727;
	m_nTECoolerTemp[227] =  6778;
	m_nTECoolerTemp[228] =  6830;
	m_nTECoolerTemp[229] =  6881;
	m_nTECoolerTemp[230] =  6932;
	m_nTECoolerTemp[231] =  6984;
	m_nTECoolerTemp[232] =  7035;
	m_nTECoolerTemp[233] =  7087;
	m_nTECoolerTemp[234] =  7138;
	m_nTECoolerTemp[235] =  7089;
	m_nTECoolerTemp[236] =  7241;
	m_nTECoolerTemp[237] =  7292;
	m_nTECoolerTemp[238] =  7343;
	m_nTECoolerTemp[239] =  7395;
	m_nTECoolerTemp[240] =  7446;
	m_nTECoolerTemp[241] =  7498;
	m_nTECoolerTemp[242] =  7549;
	m_nTECoolerTemp[243] =  7600;
	m_nTECoolerTemp[244] =  7652;
	m_nTECoolerTemp[245] =  7703;
	m_nTECoolerTemp[246] =  7755;
	m_nTECoolerTemp[247] =  7806;
	m_nTECoolerTemp[248] =  7857;
	m_nTECoolerTemp[249] =  7909;
	m_nTECoolerTemp[250] =  7960;
	m_nTECoolerTemp[251] =  8011;
	m_nTECoolerTemp[252] =  8063;
	m_nTECoolerTemp[253] =  8114;
	m_nTECoolerTemp[254] =  8166;
	m_nTECoolerTemp[255] =  8217;
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /WinLab/SpecFW/TECoolerTemperatureTable.cpp $
 * 
 * 2     5/13/05 10:58 Frazzitl
 * Updated table values
 * 
 * 1     5/13/05 9:06 Frazzitl
 * Functions needed to read detector temperature

$NoKeywords: $

** End of Rev History **/
