//===========================================================================
//
//	Module Name:	FileVersionInfo.h
//
//	Original Author:	Remco Weidema
//
//	Copyright (c) 2005, PerkinElmer, Inc. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/FileVersionInfo.h 1     6/08/05 10:22a Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#if !defined(AFX_FILEVERSION_H__F828004C_7680_40FE_A08D_7BB4FF05B4CC__INCLUDED_)
#define AFX_FILEVERSION_H__F828004C_7680_40FE_A08D_7BB4FF05B4CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <winver.h>

class CFileVersionInfo
{
// construction/destruction
public:
	CFileVersionInfo();
	virtual ~CFileVersionInfo();

// operations
public:
	BOOL Create(HMODULE hModule = NULL);
	BOOL Create(LPCTSTR lpszFileName);

// attribute operations
public:
	WORD GetFileVersion(int nIndex) const;
	WORD GetProductVersion(int nIndex) const;
	DWORD GetFileFlagsMask() const;
	DWORD GetFileFlags() const;
	DWORD GetFileOs() const;
	DWORD GetFileType() const;
	DWORD GetFileSubtype() const;
	CTime GetFileDate() const;

	CString GetCompanyName() const;
	CString GetFileDescription() const;
	CString GetFileVersion() const;
	CString GetInternalName() const;
	CString GetLegalCopyright() const;
	CString GetOriginalFileName() const;
	CString GetProductName() const;
	CString GetProductVersion() const;
	CString GetComments() const;
	CString GetLegalTrademarks() const;
	CString GetPrivateBuild() const;
	CString GetSpecialBuild() const;

// implementation helpers
protected:
	virtual void Reset();
	BOOL GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, BOOL bPrimaryEnough = FALSE);

// attributes
private:
	VS_FIXEDFILEINFO m_FileInfo;

	CString m_strCompanyName;
	CString m_strFileDescription;
	CString m_strFileVersion;
	CString m_strInternalName;
	CString m_strLegalCopyright;
	CString m_strOriginalFileName;
	CString m_strProductName;
	CString m_strProductVersion;
	CString m_strComments;
	CString m_strLegalTrademarks;
	CString m_strPrivateBuild;
	CString m_strSpecialBuild;
};

#endif // !defined(AFX_FILEVERSION_H__F828004C_7680_40FE_A08D_7BB4FF05B4CC__INCLUDED_)

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /WinLab/SpecFW/FileVersionInfo.h $
 * 
 * 1     6/08/05 10:22a Frazzitl
 * Added classes needed to obtain Kernel Information and Version Numbers

$NoKeywords: $

** End of Rev History **/
