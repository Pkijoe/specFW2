//===========================================================================
//
//	Module Name:	convert.cpp
//
// 	Function:		This file contains the data conversion routines:
//					c2b - fill the IEEE buffer with a binary char (3 char)
//					i2b - fill the IEEE buffer with a binary int (3 char)
//					c4c2b - fill IEEE buffer with binary int (4 char)
//					s2b - fill the IEEE buffer with a binary int (5 char)
//					long2b - fill IEEE buffer with binary long (6 char)
//					l2b - fill the IEEE buffer with a binary long (8 char)
//					h2b - fill the IEEE buffer with a hex int (6 char)
//					h2b4 - fill the IEEE buffer with a hex int (4 char)
//					h2b2 - fill the IEEE buffer with a hex byte (2 char)
//					b2c - returns a binary int equal to the IEEE buffer (3 char)
//					b2c4 - return a binary int equal to the IEEE buffer (4 char)
//					b2s - returns a binary int equal to the IEEE buffer (5 char)
//					b2l - returns a binary long equal to the IEEE buffer (6 char)
//					b2h2 - passes a hex byte equal to the IEEE buffer (2 char)
//					b2h - passes a hex long equal to the IEEE buffer (4 char)
//					b2a - passes a hex long equal to the IEEE buffer (6 char)
//					b2a4 - passes a hex long equal to the IEEE buffer (4 char)
//					c4h4 - returns a WORD equal to the 4 hex characters in the buffer (4 char)
//
//	Original Author:	T Frazzini	Aug 1995
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/Convert.cpp 7     11/21/05 11:25a Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "ParserThread.h"

void CParserThread::c2b(char cVal, unsigned int nCnt)
{
	unsigned int	nVal = cVal;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (nVal >= 100)
	{
		m_nDataOutBuf[nCnt] += 1;
		nVal -= 100;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (nVal >= 10)
	{
		m_nDataOutBuf[nCnt] += 1;
		nVal -= 10;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = (char) (ASCII0 + nVal);
}
//===========================================================================

void CParserThread::i2b(unsigned int nVal, unsigned int nCnt)
{
	m_nDataOutBuf[nCnt] = ASCII0;
	while (nVal >= 100)
	{
		m_nDataOutBuf[nCnt] += 1;
		nVal -= 100;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (nVal >= 10)
	{
		m_nDataOutBuf[nCnt] += 1;
		nVal -= 10;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = (char) (ASCII0 + nVal);
}
//===========================================================================

void CParserThread::c4c2b(unsigned int nVal, unsigned int nCnt)
{
	long	lVal = nVal;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (lVal >= 1000)
	{
		m_nDataOutBuf[nCnt] += 1;
		lVal -= 1000;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (lVal >= 100)
	{
		m_nDataOutBuf[nCnt] += 1;
		lVal -= 100;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (lVal >= 10)
	{
		m_nDataOutBuf[nCnt] += 1;
		lVal -= 10;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = (char) (ASCII0 + lVal);
}
//===========================================================================

// Write five numeric characters to the Response buffer
void CParserThread::s2b(unsigned int nVal, unsigned int nCnt)
{
	long	lVal = nVal;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (lVal >= 10000)
	{
		m_nDataOutBuf[nCnt] += 1;
		lVal -= 10000;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (lVal >= 1000)
	{
		m_nDataOutBuf[nCnt] += 1;
		lVal -= 1000;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (lVal >= 100)
	{
		m_nDataOutBuf[nCnt] += 1;
		lVal -= 100;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = ASCII0;
	while (lVal >= 10)
	{
		m_nDataOutBuf[nCnt] += 1;
		lVal -= 10;
	}
	nCnt++;

	m_nDataOutBuf[nCnt] = (char) (ASCII0 + lVal);
}
//===========================================================================

void CParserThread::long2b(long lVal, unsigned int nCnt)
{
	if (lVal > 999999)
	{
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
	}
	else
	{
		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 100000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 100000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 10000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 10000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 1000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 1000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 100)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 100;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 10)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 10;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = (char) (ASCII0 + lVal);
	}
}
//===========================================================================

void CParserThread::l2b(long lVal, unsigned int nCnt)
{
	if (lVal > 99999999)
	{
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
		m_nDataOutBuf[nCnt++] = 'X';
	}
	else
	{
		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 10000000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 10000000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 1000000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 1000000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 100000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 100000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 10000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 10000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 1000)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 1000;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 100)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 100;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = ASCII0;
		while (lVal >= 10)
		{
			m_nDataOutBuf[nCnt] += 1;
			lVal -= 10;
		}
		nCnt++;

		m_nDataOutBuf[nCnt] = (char) (ASCII0 + lVal);
	}
}
//===========================================================================

void CParserThread::h2b(long lVal, unsigned int nCnt)
{
	long	tVal;

	lVal &= 0x00ffffff;
	if (lVal >= 0x100000)
	{
		tVal = lVal;
		tVal  &= 0x00f00000;
		tVal >>= 20;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x000fffff;
	nCnt++;

	if (lVal >= 0x10000)
	{
		tVal	= lVal;
		tVal   &= 0x000f0000;
		tVal  >>= 16;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x0000ffff;
	nCnt++;

	if (lVal >= 0x1000)
	{
		tVal	= lVal;
		tVal   &= 0x0000f000;
		tVal  >>= 12;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x00000fff;
	nCnt++;

	if (lVal >= 0x100)
	{
		tVal	= lVal;
		tVal   &= 0x00000f00;
		tVal  >>= 8;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x000000ff;
	nCnt++;

	if (lVal >= 0x10)
	{
		tVal	= lVal;
		tVal   &= 0x000000f0;
		tVal  >>= 4;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x0000000f;
	nCnt++;

	if (lVal >= 1)
	{
		if (lVal > 0 && lVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + lVal);
		else if (lVal >= 0xa && lVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + lVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
}
//===========================================================================

// Write four hex characters to the Response buffer
void CParserThread::h2b4(long lVal, unsigned int nCnt)
{
	long	 tVal;

	lVal &= 0x0000ffff;
	if (lVal >= 0x1000)
	{
		tVal	= lVal;
		tVal   &= 0x0000f000;
		tVal  >>= 12;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x00000fff;
	nCnt++;

	if (lVal >= 0x100)
	{
		tVal	= lVal;
		tVal   &= 0x00000f00;
		tVal  >>= 8;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x000000ff;
	nCnt++;

	if (lVal >= 0x10)
	{
		tVal	= lVal;
		tVal   &= 0x000000f0;
		tVal  >>= 4;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x0000000f;
	nCnt++;

	if (lVal >= 1)
	{
		if (lVal > 0 && lVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + lVal);
		else if (lVal >= 0xa && lVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + lVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
}
//===========================================================================

// Write two hex characters to the Response buffer
void CParserThread::h2b2(long lVal, unsigned int nCnt)
{
	long	 tVal;

	lVal &= 0x0000ff;

	if (lVal >= 0x10)
	{
		tVal	= lVal;
		tVal   &= 0x000000f0;
		tVal  >>= 4;
		if (tVal > 0 && tVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + tVal);
		else if (tVal >= 0xa && tVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + tVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
	lVal &= 0x0000000f;
	nCnt++;

	if (lVal >= 1)
	{
		if (lVal > 0 && lVal <= 9)
			m_nDataOutBuf[nCnt] = (char) (ASCII0 + lVal);
		else if (lVal >= 0xa && lVal <= 0xf)
			m_nDataOutBuf[nCnt] = (char) ('A' - 10 + lVal);
		else
			m_nDataOutBuf[nCnt] = ASCII0;
	}
	else
		m_nDataOutBuf[nCnt] = ASCII0;
}
//===========================================================================

WORD CParserThread::b2c()
{
	int		cVal;
	int		val;

	cVal = 0;

	val  = c2h();
	if (val)
		cVal += (100 * val);
	
	val  = c2h();
	if (val)
		cVal += (10 * val);
	
	val  = c2h();
	if (val)
		cVal += val;

	return (WORD) cVal;
}
//===========================================================================
WORD CParserThread::b2c4()
{
	int		nVal;
	int		val;

	nVal = 0;

	val  = c2h();
	if (val)
		nVal += (1000 * val);

	val  = c2h();
	if (val)
		nVal += (100 * val);

	val  = c2h();
	if (val)
		nVal += (10 * val);

	val  = c2h();
	if (val)
		nVal += val;

	return (WORD) nVal;
}
//===========================================================================
// Convert 4 Hex chars in buffer to a binary WORD 
//
WORD CParserThread::c4h4()
{
	int		nVal;
	int		val;

	nVal = 0;

	val  = c2h();
	if (val)
		nVal += (val << 12);

	val  = c2h();
	if (val)
		nVal += (val << 8);

	val  = c2h();
	if (val)
		nVal += (val << 4);

	val  = c2h();
	if (val)
		nVal += val;

	return (WORD) nVal;
}
//===========================================================================
// Read five numeric characters from the Command buffer
WORD CParserThread::b2s()
{
	int		nVal;
	int		val;

	nVal = 0;

	val  = c2h();
	if (val)
		nVal += (10000 * val);

	val  = c2h();
	if (val)
		nVal += (1000 * val);

	val  = c2h();
	if (val)
		nVal += (100 * val);

	val  = c2h();
	if (val)
		nVal += (10 * val);

	val  = c2h();
	if (val)
		nVal += val;

	return (WORD) nVal;
}
//===========================================================================

unsigned long CParserThread::b2l()
{
	unsigned long	 nVal;
	unsigned long	 val;

	nVal = 0;

	val  = c2h();
	if (val)
		nVal += (100000 * val);

	val  = c2h();
	if (val)
		nVal += (10000 * val);

	val  = c2h();
	if (val)
		nVal += (1000 * val);

	val  = c2h();
	if (val)
		nVal += (100 * val);

	val  = c2h();
	if (val)
		nVal += (10 * val);

	val  = c2h();
	if (val)
		nVal += val;

	return nVal;
}
//===========================================================================

// Read two hex characters from the Command buffer
BYTE CParserThread::b2h2()
{
	int		nVal;
	int		val;

	nVal = 0;

	val  = c2h();
	if (val)
		nVal += (0x0010 * val);

	val  = c2h();
	if (val)
		nVal += val;

	return (BYTE) nVal;
}
//===========================================================================

// Read four hex characters from the Command buffer
WORD CParserThread::b2h()
{
	int		nVal;
	int		val;

	nVal = 0;

	val  = c2h();
	if (val)
		nVal += (0x1000 * val);

	val  = c2h();
	if (val)
		nVal += (0x0100 * val);

	val  = c2h();
	if (val)
		nVal += (0x0010 * val);

	val  = c2h();
	if (val)
		nVal += val;

	return (WORD) nVal;
}
//===========================================================================

// Read six hex characters from the Command buffer
void CParserThread::b2a(unsigned long *lVal)
{
	unsigned long	 val;

	*lVal = 0;

	val  = c2h();
	if (val)
		*lVal += (0x100000 * val);

	val  = c2h();
	if (val)
		*lVal += (0x10000 * val);

	val  = c2h();
	if (val)
		*lVal += (0x1000 * val);

	val  = c2h();
	if (val)
		*lVal += (0x100 * val);

	val  = c2h();
	if (val)
		*lVal += (0x10 * val);

	val  = c2h();
	if (val)
		*lVal += val;
}
//===========================================================================

// Read four hex characters from the Command buffer
void CParserThread::b2a4(unsigned long *lVal)
{
	unsigned long	 val;

	*lVal = 0;
	
	val  = c2h();
	if (val)
		*lVal += (0x1000 * val);

	val  = c2h();
	if (val)
		*lVal += (0x100 * val);

	val  = c2h();
	if (val)
		*lVal += (0x10 * val);

	val  = c2h();
	if (val)
		*lVal += val;
}
//===========================================================================

// Read a hex character from the Command buffer
char CParserThread::c2h()
{
	char	val;
	char	chr;

	chr = *m_pCmdPtr++;
	if (chr >= '0' && chr <= '9')		// LEGAL 'VALUE' ?
		val   = (char) (chr - ASCII0);
	else if (chr >= 'A' && chr <= 'F')
		val   = (char) (chr - 'A' + 0xa);
	else if (chr >= 'a' && chr <= 'f')
		val   = (char) (chr - 'a' + 0xa);
	else
		val   = 0;
	
	m_nBytesRead++;

	return(val);
}
//===========================================================================

/*** Revision History ***

$Log: /IcarusBased/SpecFW/Convert.cpp $
 * 
 * 7     11/21/05 11:25a Nashth
 * Corrected number of bits shifted in last fix.
 * 
 * 6     11/21/05 11:22a Nashth
 * Repaired D1 command as per QPR: THNJ6JCL5D.
 * 
 * 5     8/10/05 11:42 Frazzitl
 * Added new commands to read and write BRAM bytes.
 * 
 * 4     6/06/05 3:36p Frazzitl
 * Removed warnings during build
 * 
 * 3     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 2     3/30/05 2:59p Frazzitl
 * Fixed some startup problems
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
