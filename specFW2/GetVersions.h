//===========================================================================
//
//	Module Name:	GetVersions.h
//
//	Function:		Header file of CGetVersions class which obtains
//					all relevant version information of the system.
//
//	Original Author:	Thomas L Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/GetVersions.h 1     6/08/05 10:22a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#if !defined(AFX_GETVERSIONS_H__0CB512B9_3E25_4B37_A335_16092EB35069__INCLUDED_)
#define AFX_GETVERSIONS_H__0CB512B9_3E25_4B37_A335_16092EB35069__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// CGetVersions class

class CGetVersions : public CObject
{
// Construction
public:
	CGetVersions();		// standard constructor
	virtual			~CGetVersions(){};

// Implementation
public:
	void			GetVersionInfo();
	CString			VersionInfo(){	return m_sVersionInfo; }

protected:
	unsigned short	cpldRev(void);

//	CListBox		m_VersionInfo;
	CString			m_sVersionInfo;
};
//===========================================================================

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETVERSIONS_H__0CB512B9_3E25_4B37_A335_16092EB35069__INCLUDED_)

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /WinLab/SpecFW/GetVersions.h $
 * 
 * 1     6/08/05 10:22a Frazzitl
 * Added classes needed to obtain Kernel Information and Version Numbers

$NoKeywords: $

** End of Rev History **/
