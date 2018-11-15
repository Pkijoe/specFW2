//===========================================================================
//
//	Module Name:	VegaDetector.cpp
//
// 	Function:		This routine contains the Vega detector parameters
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/VegaDetector.cpp 3     6/06/05 3:36p Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::CreateVegaDetector()
{
	m_pVegaArray[  0] = new CARRAY(  0, 16, NORMAL,  UV	,   0);		// UV SubArray    #0:	Hg
	m_pVegaArray[  1] = new CARRAY(  1, 16, NORMAL,  UV	,   1);		// UV SubArray    #1:	P
	m_pVegaArray[  2] = new CARRAY(  2, 16, NORMAL,  UV	,   2);		// UV SubArray    #2:	S
	m_pVegaArray[  3] = new CARRAY(  3, 16, NORMAL,  UV	,   3);		// UV SubArray    #3:	Pb
	m_pVegaArray[  4] = new CARRAY(  4, 24, NORMAL,  UV	,   4);		// UV SubArray    #4:	Sn,S
	m_pVegaArray[  5] = new CARRAY(  5, 16, NORMAL,  UV	,   5);		// UV SubArray    #5:	S
	m_pVegaArray[  6] = new CARRAY(  6, 32, INVERSE, UV	,   6);		// UV SubArray    #6:	Al
	m_pVegaArray[  7] = new CARRAY(  7, 16, NORMAL,  UV	,   7);		// UV SubArray    #7:	Bl
	m_pVegaArray[  8] = new CARRAY(  8, 16, NORMAL,  UV	,   8);		// UV SubArray    #8:	As
	m_pVegaArray[  9] = new CARRAY(  9, 24, NORMAL,  UV	,   9);		// UV SubArray    #9:	B
	m_pVegaArray[ 10] = new CARRAY( 10, 24, INVERSE, UV	,  10);		// UV SubArray    #10:	As,Re
	m_pVegaArray[ 11] = new CARRAY( 11, 16, NORMAL,  UV	,  11);		// UV SubArray    #11:	Br
	m_pVegaArray[ 12] = new CARRAY( 12, 16, NORMAL,  UV	,  12);		// UV SubArray    #12:	As
	m_pVegaArray[ 13] = new CARRAY( 13, 32, INVERSE, UV	,  13);		// UV SubArray    #13:	I
	m_pVegaArray[ 14] = new CARRAY( 14, 16, INVERSE, UV	,  14);		// UV SubArray    #14:	Ti
	m_pVegaArray[ 15] = new CARRAY( 15, 16, NORMAL,  UV	,  15);		// UV SubArray    #15:	I
	m_pVegaArray[ 16] = new CARRAY( 16, 16, NORMAL,  UV	,  16);		// UV SubArray    #16:	Se
	m_pVegaArray[ 17] = new CARRAY( 17, 16, INVERSE, UV	,  17);		// UV SubArray    #17:	Ir
	m_pVegaArray[ 18] = new CARRAY( 18, 16, NORMAL,  UV	,  18);		// UV SubArray    #18:	C
	m_pVegaArray[ 19] = new CARRAY( 19, 16, NORMAL,  UV	,  19);		// UV SubArray    #19:	Ge
	m_pVegaArray[ 20] = new CARRAY( 20, 16, NORMAL,  UV	,  20);		// UV SubArray    #20:	Cr
	m_pVegaArray[ 21] = new CARRAY( 21, 16, NORMAL,  UV	,  21);		// UV SubArray    #21:	Mo
	m_pVegaArray[ 22] = new CARRAY( 22, 16, INVERSE, UV	,  22);		// UV SubArray    #22:	Mo
	m_pVegaArray[ 23] = new CARRAY( 23, 16, INVERSE, UV	,  23);		// UV SubArray    #23:	Se
	m_pVegaArray[ 24] = new CARRAY( 24, 24, NORMAL,  UV	,  24);		// UV SubArray    #24:	Cr,Zn
	m_pVegaArray[ 25] = new CARRAY( 25, 16, NORMAL,  UV	,  25);		// UV SubArray    #25:	Zn
	m_pVegaArray[ 26] = new CARRAY( 26, 16, INVERSE, UV	,  26);		// UV SubArray    #26:	Au
	m_pVegaArray[ 27] = new CARRAY( 27, 16, INVERSE, UV	,  27);		// UV SubArray    #27:	Mo
	m_pVegaArray[ 28] = new CARRAY( 28, 16, NORMAL,  UV	,  28);		// UV SubArray    #28:	Sb
	m_pVegaArray[ 29] = new CARRAY( 29, 24, INVERSE, UV	,  29);		// UV SubArray    #29:	Re,Pt
	m_pVegaArray[ 30] = new CARRAY( 30, 16, NORMAL,  UV	,  30);		// UV SubArray    #30:	Hg
	m_pVegaArray[ 31] = new CARRAY( 31, 16, INVERSE, UV	,  31);		// UV SubArray    #31:	Se
	m_pVegaArray[ 32] = new CARRAY( 32, 16, NORMAL,  UV	,  32);		// UVSC SubArray  #0:	Sr
	m_pVegaArray[ 33] = new CARRAY( 33, 16, INVERSE, UV	,  33);		// UV SubArray    #32:	Ga
	m_pVegaArray[ 34] = new CARRAY( 34, 16, INVERSE, UV	,  34);		// UV SubArray    #33:	Lu
	m_pVegaArray[ 35] = new CARRAY( 35, 16, INVERSE, UV	,  35);		// UV SubArray    #34:	Sb
	m_pVegaArray[ 36] = new CARRAY( 36, 16, INVERSE, UV	,  36);		// UV SubArray    #35:	Ir
	m_pVegaArray[ 37] = new CARRAY( 37, 16, INVERSE, UV	,  37);		// UV SubArray    #36:	P
	m_pVegaArray[ 38] = new CARRAY( 38, 16, NORMAL,  UV	,  38);		// UV SubArray    #37:	W
	m_pVegaArray[ 39] = new CARRAY( 39, 16, INVERSE, UV	,  39);		// UV SubArray    #38:	Zn
	m_pVegaArray[ 40] = new CARRAY( 40, 40, NORMAL,  UV	,  40);		// UV SubArray    #39:	Te,Pt,Cd
	m_pVegaArray[ 41] = new CARRAY( 41, 16, INVERSE, UV	,  41);		// UV SubArray    #40:	Si
	m_pVegaArray[ 42] = new CARRAY( 42, 16, NORMAL,  UV	,  42);		// UV SubArray    #41:	Pb
	m_pVegaArray[ 43] = new CARRAY( 43, 16, NORMAL,  UV	,  43);		// UV SubArray    #42:	Cu
	m_pVegaArray[ 44] = new CARRAY( 44, 16, INVERSE, UV	,  44);		// UV SubArray    #43:	P
	m_pVegaArray[ 45] = new CARRAY( 45, 16, NORMAL,  UV	,  45);		// UV SubArray    #44:	Ni
	m_pVegaArray[ 46] = new CARRAY( 46, 24, INVERSE, UV	,  46);		// UV SubArray    #45:	B
	m_pVegaArray[ 47] = new CARRAY( 47,  0, NORMAL,  SPARE,47);		// Spare Address  #0:	NONE
	m_pVegaArray[ 48] = new CARRAY( 48, 16, NORMAL,  UV	,  48);		// UV SubArray    #46:	Ni
	m_pVegaArray[ 49] = new CARRAY( 49, 16, NORMAL,  UV	,  49);		// UV SubArray    #47:	Sb
	m_pVegaArray[ 50] = new CARRAY( 50, 24, INVERSE, UV	,  50);		// UV SubArray    #48:	Cd,Te
	m_pVegaArray[ 51] = new CARRAY( 51, 16, INVERSE, UV	,  51);		// UV SubArray    #49:	Ni
	m_pVegaArray[ 52] = new CARRAY( 52, 16, NORMAL,  UV	,  52);		// UV SubArray    #50:	Ni
	m_pVegaArray[ 53] = new CARRAY( 53, 16, NORMAL,  UV	,  53);		// UV SubArray    #51:	Cu
	m_pVegaArray[ 54] = new CARRAY( 54, 16, NORMAL,  UV	,  54);		// UV SubArray    #52:	Pb
	m_pVegaArray[ 55] = new CARRAY( 55, 16, INVERSE, UV	,  55);		// UV SubArray    #53:	W
	m_pVegaArray[ 56] = new CARRAY( 56, 24, INVERSE, UV	,  56);		// UV SubArray    #54:	Os,Bi
	m_pVegaArray[ 57] = new CARRAY( 57, 16, INVERSE, UV	,  57);		// UV SubArray    #55:	Re
	m_pVegaArray[ 58] = new CARRAY( 58, 16, INVERSE, UV	,  58);		// UV SubArray    #56:	Bi
	m_pVegaArray[ 59] = new CARRAY( 59, 16, INVERSE, UV	,  59);		// UV SubArray    #57:	Os
	m_pVegaArray[ 60] = new CARRAY( 60, 40, NORMAL,  UV	,  60);		// UV SubArray    #58:	Ba,In
	m_pVegaArray[ 61] = new CARRAY( 61, 16, INVERSE, UV	,  61);		// UV SubArray    #59:	Ta
	m_pVegaArray[ 62] = new CARRAY( 62, 16, INVERSE, UV	,  62);		// UV SubArray    #60:	Co
	m_pVegaArray[ 63] = new CARRAY( 63, 16, INVERSE, UV	,  63);		// UV SubArray    #61:	Cd
	m_pVegaArray[ 64] = new CARRAY( 64, 16, NORMAL,  UV	,  64);		// UV SubArray    #62:	Ag
	m_pVegaArray[ 65] = new CARRAY( 65, 16, NORMAL,  UV	,  65);		// UV SubArray    #63:	Te
	m_pVegaArray[ 66] = new CARRAY( 66, 16, NORMAL,  UV	,  66);		// UV SubArray    #64:	Co
	m_pVegaArray[ 67] = new CARRAY( 67, 16, INVERSE, UV	,  67);		// UV SubArray    #65:	Co
	m_pVegaArray[ 68] = new CARRAY( 68, 16, NORMAL,  UV	,  68);		// UV SubArray    #66:	Fe
	m_pVegaArray[ 69] = new CARRAY( 69, 16, NORMAL,  UV	,  69);		// UV SubArray    #67:	Sn
	m_pVegaArray[ 70] = new CARRAY( 70, 32, NORMAL,  UV	,  70);		// UV SubArray    #68:	Fe
	m_pVegaArray[ 71] = new CARRAY( 71, 16, INVERSE, UV	,  71);		// UV SubArray    #69:	Ni
	m_pVegaArray[ 72] = new CARRAY( 72, 16, NORMAL,  UV	,  72);		// UV SubArray    #70:	Hf
	m_pVegaArray[ 73] = new CARRAY( 73, 16, NORMAL,  UV	,  73);		// UV SubArray    #71:	Al
	m_pVegaArray[ 74] = new CARRAY( 74, 16, INVERSE, UV	,  74);		// UV SubArray    #72:	Be
	m_pVegaArray[ 75] = new CARRAY( 75, 16, NORMAL,  UV	,  75);		// UV SubArray    #73:	Sn
	m_pVegaArray[ 76] = new CARRAY( 76, 16, INVERSE, UV	,  76);		// UV SubArray    #74:	Os
	m_pVegaArray[ 77] = new CARRAY( 77, 16, INVERSE, UV	,  77);		// UV SubArray    #75:	Ta
	m_pVegaArray[ 78] = new CARRAY( 78, 16, INVERSE, UV	,  78);		// UV SubArray    #76:	Co
	m_pVegaArray[ 79] = new CARRAY( 79, 24, INVERSE, UV	,  79);		// UV SubArray    #77:	Rh,Ba
	m_pVegaArray[ 80] = new CARRAY( 80,  0, NORMAL,  SPARE,80);		// Spare Address  #1:	NONE
	m_pVegaArray[ 81] = new CARRAY( 81, 32, NORMAL,  UV	,  81);		// UV SubArray    #78:	Pb,Lu
	m_pVegaArray[ 82] = new CARRAY( 82, 32, NORMAL,  UV	,  82);		// UV SubArray    #79:	B
	m_pVegaArray[ 83] = new CARRAY( 83, 16, INVERSE, UV	,  83);		// UV SubArray    #80:	Sb
	m_pVegaArray[ 84] = new CARRAY( 84, 16, NORMAL,  UV	,  84);		// UV SubArray    #81:	Hg
	m_pVegaArray[ 85] = new CARRAY( 85, 16, NORMAL,  UV	,  85);		// UV SubArray    #82:	Au
	m_pVegaArray[ 86] = new CARRAY( 86, 16, INVERSE, UV	,  86);		// UV SubArray    #83:	Ta
	m_pVegaArray[ 87] = new CARRAY( 87, 16, INVERSE, UV	,  87);		// UV SubArray    #84:	Fe
	m_pVegaArray[ 88] = new CARRAY( 88, 16, INVERSE, UV	,  88);		// UV SubArray    #85:	Ru
	m_pVegaArray[ 89] = new CARRAY( 89, 16, INVERSE, UV	,  89);		// UV SubArray    #86:	Zr
	m_pVegaArray[ 90] = new CARRAY( 90, 16, NORMAL,  UV	,  90);		// UV SubArray    #87:	Mn
	m_pVegaArray[ 91] = new CARRAY( 91, 16, NORMAL,  UV	,  91);		// UV SubArray    #88:	Si
	m_pVegaArray[ 92] = new CARRAY( 92, 24, INVERSE, UV	,  92);		// UV SubArray    #89:	Pd,W
	m_pVegaArray[ 93] = new CARRAY( 93, 16, INVERSE, UV	,  93);		// UV SubArray    #90:	Fe
	m_pVegaArray[ 94] = new CARRAY( 94, 32, INVERSE, UV	,  94);		// UV SubArray    #91:	Mn
	m_pVegaArray[ 95] = new CARRAY( 95,  0, NORMAL,  SPARE,95);		// Spare Address  #2:	NONE
	m_pVegaArray[ 96] = new CARRAY( 96, 32, INVERSE, UV	,  96);		// UV SubArray    #92:	Au,Cr
	m_pVegaArray[ 97] = new CARRAY( 97, 16, INVERSE, UV	,  97);		// UV SubArray    #93:	Mo
	m_pVegaArray[ 98] = new CARRAY( 98, 24, INVERSE, UV	,  98);		// UV SubArray    #94:	Be,Ge
	m_pVegaArray[ 99] = new CARRAY( 99, 16, NORMAL,  UV	,  99);		// UV SubArray    #95:	Mg
	m_pVegaArray[100] = new CARRAY(100, 16, NORMAL,  UV	, 100);		// UV SubArray    #96: 	Mn
	m_pVegaArray[101] = new CARRAY(101, 16, NORMAL,  UV	, 101);		// UV SubArray    #97: 	Pt
	m_pVegaArray[102] = new CARRAY(102, 24, NORMAL,  UV	, 102);		// UV SubArray    #98:  Mn,Mg
	m_pVegaArray[103] = new CARRAY(103, 16, NORMAL,  UV	, 103);		// UV SubArray    #99:	Nb
	m_pVegaArray[104] = new CARRAY(104, 16, NORMAL,  UV	, 104);		// UV SubArray    #100:	Cr
	m_pVegaArray[105] = new CARRAY(105, 16, NORMAL,  UV	, 105);		// UV SubArray    #101:	Mg
	m_pVegaArray[106] = new CARRAY(106, 16, NORMAL,  UV	, 106);		// UV SubArray    #102:	V
	m_pVegaArray[107] = new CARRAY(107, 16, INVERSE, UV	, 107);		// UV SubArray    #103:	Ti
	m_pVegaArray[108] = new CARRAY(108, 16, NORMAL,  UV	, 108);		// UV SubArray    #104:	Cr
	m_pVegaArray[109] = new CARRAY(109, 16, INVERSE, UV	, 109);		// UV SubArray    #105:	Hf
	m_pVegaArray[110] = new CARRAY(110, 16, INVERSE, UV	, 110);		// UV SubArray    #106:	Fe
	m_pVegaArray[111] = new CARRAY(111, 16, INVERSE, UV	, 111);		// UV SubArray    #107:	Hf
	m_pVegaArray[112] = new CARRAY(112, 16, NORMAL,  UV	, 112);		// UV SubArray    #108:	V
	m_pVegaArray[113] = new CARRAY(113, 16, NORMAL,  UV	, 113);		// UV SubArray    #109:	Mg
	m_pVegaArray[114] = new CARRAY(114, 16, NORMAL,  UV	, 114);		// UV SubArray    #110:	Nb
	m_pVegaArray[115] = new CARRAY(115, 16, INVERSE, UV	, 115);		// UV SubArray    #111:	Yb
	m_pVegaArray[116] = new CARRAY(116, 32, INVERSE, UV	, 116);		// UVVIS SubArray #0:	Mn
	m_pVegaArray[117] = new CARRAY(117, 10, NORMAL,  VIS, 117);		// VIS SubArray   #0:	Cl
	m_pVegaArray[118] = new CARRAY(118, 16, NORMAL,  UV	, 118);		// UV SubArray    #112:	Ga
	m_pVegaArray[119] = new CARRAY(119, 16, NORMAL,  UV	, 119);		// UV SubArray    #113:	Pb
	m_pVegaArray[120] = new CARRAY(120, 16, NORMAL,  UV	, 120);		// UV SubArray    #114:	V
	m_pVegaArray[121] = new CARRAY(121, 16, NORMAL,  UV	, 121);		// UV SubArray    #115:	Mn
	m_pVegaArray[122] = new CARRAY(122, 16, NORMAL,  UV	, 122);		// UV SubArray    #116:	Lu
	m_pVegaArray[123] = new CARRAY(123, 16, INVERSE, UV	, 123);		// UV SubArray    #117:	Th
	m_pVegaArray[124] = new CARRAY(124, 16, INVERSE, UV	, 124);		// UV SubArray    #118:	Sn
	m_pVegaArray[125] = new CARRAY(125, 10, NORMAL,  VIS, 125);		// VIS SubArray   #1:	K
	m_pVegaArray[126] = new CARRAY(126, 16, INVERSE, UV	, 126);		// UV SubArray    #119:	Si
	m_pVegaArray[127] = new CARRAY(127, 16, NORMAL,  UV	, 127);		// UV Subarray    #120:	Pt
	m_pVegaArray[128] = new CARRAY(128, 10, NORMAL,  VIS, 128);		// VIS SubArray   #2:	Br
	m_pVegaArray[129] = new CARRAY(129, 32, NORMAL,  UV	, 129);		// UV SubArray    #121:	Al,V,Nb
	m_pVegaArray[130] = new CARRAY(130, 10, NORMAL,  VIS, 130);		// VIS SubArray   #3:	Cl
	m_pVegaArray[131] = new CARRAY(131, 16, INVERSE, UV	, 131);		// UVSC SubArray  #1:	Li
	m_pVegaArray[132] = new CARRAY(132, 16, NORMAL,  UV	, 132);		// UV SubArray    #122:	V
	m_pVegaArray[133] = new CARRAY(133, 16, NORMAL,  UV	, 133);		// UV SubArray    #123:	Hg
	m_pVegaArray[134] = new CARRAY(134, 10, INVERSE, VIS, 134);		// VIS SubArray   #4:	U
	m_pVegaArray[135] = new CARRAY(135, 16, NORMAL,  UV	, 135);		// UV SubArray    #124: Bi
	m_pVegaArray[136] = new CARRAY(136, 16, INVERSE, UV	, 136);		// UV SubArray    #125:	V
	m_pVegaArray[137] = new CARRAY(137, 16, NORMAL,  UV	, 137);		// UV SubArray    #126:	Ca
	m_pVegaArray[138] = new CARRAY(138, 10, NORMAL,  VIS, 138);		// VIS SubArray   #5:	U
	m_pVegaArray[139] = new CARRAY(139, 16, INVERSE, VIS, 139);		// VIS SubArray   #6:	He,Ne
	m_pVegaArray[140] = new CARRAY(140, 16, INVERSE, UV	, 140);		// UV SubArray    #127:	Ge
	m_pVegaArray[141] = new CARRAY(141, 16, INVERSE, UV	, 141);		// UV SubArray    #128:	Al
	m_pVegaArray[142] = new CARRAY(142,  0, NORMAL,  SPARE, 142);	// Spare Address  #3:	NONE
	m_pVegaArray[143] = new CARRAY(143,  0, NORMAL,  SPARE, 143);	// Spare Address  #4:	NONE
	m_pVegaArray[144] = new CARRAY(144, 24, NORMAL,  UV	, 144);		// UV SubArray    #129:	Be,Tm
	m_pVegaArray[145] = new CARRAY(145, 16, NORMAL,  UV	, 145);		// UV SubArray    #130:	Cu
	m_pVegaArray[146] = new CARRAY(146, 16, NORMAL,  UV	, 146);		// UV SubArray    #131:	Ca
	m_pVegaArray[147] = new CARRAY(147, 16, NORMAL,  UV	, 147);		// UV SubArray    #132:	Ag
	m_pVegaArray[148] = new CARRAY(148, 10, NORMAL,  VIS, 148);		// VIS SubArray   #7:	Na
	m_pVegaArray[149] = new CARRAY(149, 16, NORMAL,  VIS, 149);		// VIS SubArray   #8:	Hg
	m_pVegaArray[150] = new CARRAY(150, 10, INVERSE, VIS, 150);		// VIS SubArray   #9:	Na
	m_pVegaArray[151] = new CARRAY(151, 16, NORMAL,  UV	, 151);		// UV SubArray    #133:	Yb
	m_pVegaArray[152] = new CARRAY(152, 24, INVERSE, UV	, 152);		// UV SubArray    #134:	Y,Pd
	m_pVegaArray[153] = new CARRAY(153, 16, NORMAL,  UV	, 153);		// UV SubArray    #135:	Cu
	m_pVegaArray[154] = new CARRAY(154, 16, INVERSE, UV	, 154);		// UV SubArray    #136:	Na
	m_pVegaArray[155] = new CARRAY(155, 16, INVERSE, UV	, 155);		// UV SubArray    #137:	In
	m_pVegaArray[156] = new CARRAY(116, 32, INVERSE, VIS, 156);		// UVVIS SubArray #0:	Mn
	m_pVegaArray[157] = new CARRAY(203, 24, NORMAL,  VIS, 157);		// UVVIS SubArray #1:	Sc
	m_pVegaArray[158] = new CARRAY(205, 24, INVERSE, VIS, 158);		// UVVIS SubArray #2:	Cr
	m_pVegaArray[159] = new CARRAY(230, 32, NORMAL,  VIS, 159);		// UVVIS SubArray #3:	Al,Dy
	m_pVegaArray[160] = new CARRAY(160, 16, INVERSE, UV	, 160);		// UV SubArray    #138:	Ho
	m_pVegaArray[161] = new CARRAY(161, 16, INVERSE, UV	, 161);		// UV SubArray    #139:	Gd
	m_pVegaArray[162] = new CARRAY(162, 16, NORMAL,  UV	, 162);		// UV SubArray    #140:	Er,Ti
	m_pVegaArray[163] = new CARRAY(163, 16, NORMAL,  UV	, 163);		// UV SubArray    #141:	Rh
	m_pVegaArray[164] = new CARRAY(164, 16, NORMAL,  UV	, 164);		// UV SubArray    #142:	Ag
	m_pVegaArray[165] = new CARRAY(165, 16, INVERSE, UV	, 165);		// UV SubArray    #143:	Zr
	m_pVegaArray[166] = new CARRAY(166, 16, INVERSE, UV	, 166);		// UV SubArray    #144:	Zr
	m_pVegaArray[167] = new CARRAY(167, 16, NORMAL,  UV	, 167);		// UV SubArray    #145:	Zn
	m_pVegaArray[168] = new CARRAY(168, 16, NORMAL,  UV	, 168);		// UV SubArray    #146:	Ho
	m_pVegaArray[169] = new CARRAY(169, 32, INVERSE, UV	, 169);		// UV SubArray    #147:	Ti,Gd
	m_pVegaArray[170] = new CARRAY(170, 16, INVERSE, UV	, 170);		// UV SubArray    #148:	Pu
	m_pVegaArray[171] = new CARRAY(171, 16, INVERSE, UV	, 171);		// UV SubArray    #149:	Ho
	m_pVegaArray[172] = new CARRAY(172, 16, INVERSE, UV	, 172);		// UV SubArray    #150:	Pd
	m_pVegaArray[173] = new CARRAY(173, 16, INVERSE, UV	, 173);		// UVSC SubArray  #2:	Li
	m_pVegaArray[174] = new CARRAY(174, 10, NORMAL,  VIS, 174);		// VIS SubArray   #10:	Ba
	m_pVegaArray[175] = new CARRAY(175, 24, INVERSE, UV	, 175);		// UV SubArray    #151:	Ti,Gd
	m_pVegaArray[176] = new CARRAY(176, 16, INVERSE, UV	, 176);		// UV SubArray    #152:	Dy
	m_pVegaArray[177] = new CARRAY(177, 16, NORMAL,  UV	, 177);		// UV SubArray    #153:	Sm
	m_pVegaArray[178] = new CARRAY(178, 16, NORMAL,  UV	, 178);		// UV SubArray    #154:	Zr
	m_pVegaArray[179] = new CARRAY(179, 16, NORMAL,  VIS, 179);		// VIS SubArray   #11:	Ba,Cs
	m_pVegaArray[180] = new CARRAY(180, 10, INVERSE, VIS, 180);		// VIS SubArray   #12:	Ce
	m_pVegaArray[181] = new CARRAY(181, 10, NORMAL,  VIS, 181);		// VIS SubArray   #13:	Pu
	m_pVegaArray[182] = new CARRAY(182, 24, INVERSE, UV	, 182);		// UV SubArray    #155:	Ru,Er
	m_pVegaArray[183] = new CARRAY(183, 10, NORMAL,  VIS, 183);		// VIS SubArray   #14:	Cs
	m_pVegaArray[184] = new CARRAY(184, 16, NORMAL,  UV	, 184);		// UV SubArray    #156:	Tb
	m_pVegaArray[185] = new CARRAY(185, 10, NORMAL,  VIS, 185);		// VIS SubArray   #15:	In
	m_pVegaArray[186] = new CARRAY(186, 10, NORMAL,  VIS, 186);		// VIS SubArray   #16:	U
	m_pVegaArray[187] = new CARRAY(187, 10, INVERSE, VIS, 187);		// VIS SubArray   #17:	Sr
	m_pVegaArray[188] = new CARRAY(188, 16, NORMAL,  UV	, 188);		// UV SubArray    #157:	Sc
	m_pVegaArray[189] = new CARRAY(189, 16, NORMAL,  UV	, 189);		// UV SubArray    #158:	Tm
	m_pVegaArray[190] = new CARRAY(190, 16, INVERSE, UV	, 190);		// UV SubArray    #159:	Ti
	m_pVegaArray[191] = new CARRAY(191, 16, INVERSE, UV	, 191);		// UV SubArray    #160:	Ni
	m_pVegaArray[192] = new CARRAY(192, 16, INVERSE, UV	, 192);		// UV SubArray    #161:	Y
	m_pVegaArray[193] = new CARRAY(193, 16, NORMAL,  UV	, 193);		// UV SubArray    #162:	Y
	m_pVegaArray[194] = new CARRAY(194, 16, NORMAL,  UV	, 194);		// UV SubArray    #163:	U
	m_pVegaArray[195] = new CARRAY(195, 16, NORMAL,  UV	, 195);		// UV SubArray    #164:	Cd
	m_pVegaArray[196] = new CARRAY(196, 16, INVERSE, UV	, 196);		// UV SubArray    #165:	Sc
	m_pVegaArray[197] = new CARRAY(197, 10, NORMAL,  VIS, 197);		// VIS SubArray   #18:	Nd
	m_pVegaArray[198] = new CARRAY(198, 10, NORMAL,  VIS, 198);		// VIS SubArray   #19:	U
	m_pVegaArray[199] = new CARRAY(199, 16, NORMAL,  UV	, 199);		// UV SubArray    #166:	Ti
	m_pVegaArray[200] = new CARRAY(200, 10, INVERSE, VIS, 200);		// VIS SubArray   #20:	Sc
	m_pVegaArray[201] = new CARRAY(201, 10, NORMAL,  VIS, 201);		// VIS SubArray   #21:	Sm
	m_pVegaArray[202] = new CARRAY(202, 16, NORMAL,  UV	, 202);		// UV SubArray    #167: Pu
	m_pVegaArray[203] = new CARRAY(203, 24, NORMAL,  UV	, 203);		// UVVIS SubArray #1:	Sc
	m_pVegaArray[204] = new CARRAY(204, 16, INVERSE, UV	, 204);		// UV SubArray    #168:	Pd
	m_pVegaArray[205] = new CARRAY(205, 24, INVERSE, UV	, 205);		// UVVIS SubArray #2:	Cr
	m_pVegaArray[206] = new CARRAY(206, 16, INVERSE, VIS, 206);		// VIS SubArray   #22:	Hg
	m_pVegaArray[207] = new CARRAY(207,  0, NORMAL,  SPARE, 207);	// Spare Address  #9:	NONE
	m_pVegaArray[208] = new CARRAY(208, 16, NORMAL,  VIS, 208);		// VIS SubArray   #23:	Ar,Rb
	m_pVegaArray[209] = new CARRAY(209, 24, NORMAL,  VIS, 209);		// VIS SubArray   #24:	Eu,Ba,Ce
	m_pVegaArray[210] = new CARRAY(210, 10, NORMAL,  VIS, 210);		// VIS SubArray   #25:	Sr
	m_pVegaArray[211] = new CARRAY(211, 24, INVERSE, VIS, 211);		// VIS SubArray   #26:	Nd
	m_pVegaArray[212] = new CARRAY(212, 10, INVERSE, VIS, 212);		// VIS SubArray   #27:	Ce
	m_pVegaArray[213] = new CARRAY(213, 10, NORMAL,  VIS, 213);		// VIS SubArray   #28:	Pr
	m_pVegaArray[214] = new CARRAY(214, 10, INVERSE, VIS, 214);		// VIS SubArray   #29:	Pr
	m_pVegaArray[215] = new CARRAY(215, 10, INVERSE, VIS, 215);		// VIS SubArray   #30:	Ca
	m_pVegaArray[216] = new CARRAY(216, 10, NORMAL,  VIS, 216);		// VIS SubArray   #31:	Sr
	m_pVegaArray[217] = new CARRAY(217, 24, NORMAL,  VIS, 217);		// VIS SubArray   #32:	La,U
	m_pVegaArray[218] = new CARRAY(218, 10, INVERSE, VIS, 218);		// VIS SubArray   #33:	Ga
	m_pVegaArray[219] = new CARRAY(219, 16, NORMAL,  UV	, 219);		// UV SubArray    #169:	Yb
	m_pVegaArray[220] = new CARRAY(220, 10, NORMAL,  VIS, 220);		// VIS SubArray   #34:	Ce
	m_pVegaArray[221] = new CARRAY(221, 10, NORMAL,  VIS, 221);		// VIS SubArray   #35:	Sm
	m_pVegaArray[222] = new CARRAY(222,  0, NORMAL,  SPARE, 222);	// Spare Address  #10:	NONE
	m_pVegaArray[223] = new CARRAY(223,  0, NORMAL,  SPARE, 223);	// Spare Address  #11:	NONE
	m_pVegaArray[224] = new CARRAY(224, 16, NORMAL,  UV	, 224);		// UV SubArray    #170:	Tb
	m_pVegaArray[225] = new CARRAY(225, 16, INVERSE, VIS, 225);		// VIS SubArray   #36:	Hg,K
	m_pVegaArray[226] = new CARRAY(226, 40, NORMAL,  UV	, 226);		// UV SubArray    #171:	Eu
	m_pVegaArray[227] = new CARRAY(227, 16, NORMAL,  UV	, 227);		// UV SubArray    #172:	La
	m_pVegaArray[228] = new CARRAY(228, 10, NORMAL,  VIS, 228);		// VIS SubArray   #37:	Pr
	m_pVegaArray[229] = new CARRAY(229,  0, NORMAL,  SPARE, 229);	// Spare Address  #12:	NONE
	m_pVegaArray[230] = new CARRAY(230, 32, NORMAL,  UV	, 230);		// UVVIS SubArray #3:	Al,Dy
	m_pVegaArray[231] = new CARRAY(231, 10, NORMAL,  VIS, 231);		// VIS SubArray   #38:	Nd
	m_pVegaArray[232] = new CARRAY(232, 16, INVERSE, VIS, 232);		// VIS SubArray   #39:	Al,Dy
	m_pVegaArray[233] = new CARRAY(233, 10, INVERSE, VIS, 233);		// VIS SubArray   #40:	Th
	m_pVegaArray[234] = new CARRAY(234, 16, NORMAL,  UV	, 234);		// UV SubArray    #173:	Eu
	m_pVegaArray[235] = new CARRAY(235, 10, NORMAL,  VIS, 235);		// VIS SubArray   #41:	Mn
	m_pVegaArray[236] = new CARRAY(236, 10, NORMAL,  VIS, 236);		// VIS SubArray   #42:	Al
	m_pVegaArray[237] = new CARRAY(237, 10, INVERSE, VIS, 237);		// VIS SubArray   #43:	Ca
	m_pVegaArray[238] = new CARRAY(238, 16, NORMAL,  UV	, 238);		// UVSC SubArray  #3:	K
	m_pVegaArray[239] = new CARRAY(239,  0, NORMAL,  SPARE, 239);	// Spare Address  #13:	NONE
	m_pVegaArray[240] = new CARRAY(240, 16, INVERSE, UV	, 240);		// UV SubArray    #174:	Pr
	m_pVegaArray[241] = new CARRAY(241,  0, NORMAL,  SPARE, 241);	// Spare Address  #14:	NONE
	m_pVegaArray[242] = new CARRAY(242,  0, NORMAL,  SPARE, 242);	// Spare Address  #15:	NONE
	m_pVegaArray[243] = new CARRAY(243, 10, NORMAL,  VIS, 243);		// VIS SubArray   #44:	Tb
	m_pVegaArray[244] = new CARRAY(244, 10, INVERSE, VIS, 244);		// VIS SubArray   #45:	U
	m_pVegaArray[245] = new CARRAY(245, 10, NORMAL,  VIS, 245);		// VIS SubArray   #46:	La
	m_pVegaArray[246] = new CARRAY(246, 16, NORMAL,  UV	, 246);		// UV SubArray    #175:	Nd
	m_pVegaArray[247] = new CARRAY(247,  0, NORMAL,  SPARE, 247);	// Spare Address  #16:	NONE
	m_pVegaArray[248] = new CARRAY(248, 10, NORMAL,  VIS, 248);		// VIS SubArray   #47:	Sm
	m_pVegaArray[249] = new CARRAY(249,  0, NORMAL,  SPARE, 249);	// Spare Address  #17:	NONE
	m_pVegaArray[250] = new CARRAY(250,  0, NORMAL,  SPARE, 250);	// Spare Address  #18:	NONE
	m_pVegaArray[251] = new CARRAY(251, 10, NORMAL,  VIS, 251);		// VIS SubArray   #48:	Eu
	m_pVegaArray[252] = new CARRAY(252,  0, NORMAL,  SPARE, 252);	// Spare Address  #19:	NONE
	m_pVegaArray[253] = new CARRAY(253, 16, INVERSE, UV	, 253);		// UV SubArray    #176:	Mn
	m_pVegaArray[254] = new CARRAY(254, 16, NORMAL,  UV	, 254);		// UV SubArray    #177:	Ca
	m_pVegaArray[255] = new CARRAY(255, 16, INVERSE, VIS, 255);		// UVVIS SubArray #4:	OPAQUE SHIELD
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /WinLab/SpecFW/VegaDetector.cpp $
 * 
 * 3     6/06/05 3:36p Frazzitl
 * Removed warnings during build
 * 
 * 2     4/20/05 16:59 Frazzitl
 * Added comment fields

$NoKeywords: $

** End of Rev History **/
