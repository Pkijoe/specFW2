// Copyright © 2005, PerkinElmer Inc. All rights reserved.
// This .h file should contain all the versioning info
 
/****************************** Last check-in info ***************************

$Header: /IcarusBased/ICP/SpecFW/Versioning/product_ver.h 87    8/21/08 9:50a Builder $
	
(The remainder of the revision history is located at the bottom of this file)

*****************************************************************************/

// #pragma once - Commented out #pragma to allow InstallShield to use if


// 
#ifndef _Product_Ver_h
#define _Product_Ver_h

// Product-Wide Definitions go here
#define PROD_BUILDNUMBER 9001
#define PROD_sBUILDNUMBER "9001"

// ----------------- This is the only place versioning code should be -----------

#define NOT_FOR_RELEASE 2 

#define NLMajorVersion 2
#define NLMinorVersion 0
#define NLMaintVersion 0

#define sNLMajorVersion "2."
#define sNLMinorVersion "0."
#define sNLMaintVersion "0."

#define NLName "AAICP_Icarus"

#define NL_Version  "Ver2.0.0"  
#define PROGRAM_GROUP_NAME "AAICP_Icarus Ver2.0.0" 

#ifdef _DEBUG
	#ifdef _UNICODE
		#define PROD_sCOMMENTS "Unicode Debug"
	#else
		#define PROD_sCOMMENTS "Debug Mode Build"
	#endif
#else
	#ifdef _UNICODE
		#define PROD_sCOMMENTS "Unicode Release"
	#else
		#define PROD_sCOMMENTS "Release Mode Build"
	#endif
#endif

#define MOD_FILEVERSION 2,0,0,PROD_BUILDNUMBER
#define MOD_sFILEVERSION "2.0.0.\0" PROD_BUILDNUMBER 

// ----------------- End of this is the only place versioning code should be -----------
// Used by InstallShield
#define TM_APP_NAME			NLName 
#define TM_APP_NAME_LONG	NLName 

#define NLPatchVersion PROD_BUILDNUMBER

#define PROD_sPRODUCTNAME NLName "\0"
#define PROD_PRODUCTVERSION NLMajorVersion,NLMinorVersion,NLMaintVersion,PROD_BUILDNUMBER
#define PROD_sPRODUCTVERSION sNLMajorVersion sNLMinorVersion sNLMaintVersion PROD_sBUILDNUMBER


#define PROD_sCOMPANYNAME "PerkinElmer Inc\0"
#define PROD_sCOPYRIGHT "Copyright (C) 2017, PerkinElmer Inc\0"
#define PROD_sTRADEMARKS "AAICP_Icarus\0"
#define PROD_sPRIVATEBUILD "Private Build"
#define PROD_sSPECIALBUILD  "Special Build"
#define PROD_FILEFLAGSMASK 0x3fL
#define PROD_DEBUGFILEFLAGS 0x1L
#define PROD_RELEASEFILEFLAGS 0x0L


// DJG March 8, 2002:
//  Currently, the RC doesn't get the _UNICODE defined for the UNICODE builds


#endif 
//===========================================================================

/*** Revision History ***

02/24/17    KR		CBF-155 - Miscellaneous clean up and remove build warning


/** Revision History
$Log: /IcarusBased/ICP/SpecFW/Versioning/product_ver.h $
 * 9014	6/28/16 Manual Build
 * 
 * 87    8/21/08 9:50a Builder
 * AutoBuild
 * 
 * 86    8/20/08 9:55a Builder
 * AutoBuild
 * 
 * 85    8/20/08 9:53a Builder
 * AutoBuild
 * 
 * 84    8/20/08 9:41a Builder
 * AutoBuild
 * 
 * 83    4/10/08 2:10p Builder
 * AutoBuild
 * 
 * 82    2/22/08 3:58p Builder
 * AutoBuild
 * 
 * 81    2/19/08 3:40p Builder
 * AutoBuild
 * 
 * 80    2/06/08 2:31p Builder
 * AutoBuild
 * 
 * 79    2/06/08 2:09p Builder
 * AutoBuild
 * 
 * 78    7/31/07 2:49p Builder
 * AutoBuild
 * 
 * 77    7/18/07 10:01a Builder
 * AutoBuild
 * 
 * 76    7/17/07 5:53p Builder
 * AutoBuild
 * 
 * 75    7/17/07 12:45p Builder
 * AutoBuild
 * 
 * 74    5/29/07 11:14a Builder
 * AutoBuild
 * 
 * 73    5/29/07 11:08a Builder
 * AutoBuild
 * 
 * 72    5/29/07 10:45a Builder
 * AutoBuild
 * 
 * 71    5/29/07 10:27a Nashth
 * Update version number (Product and Module) to 1.0.1
 * 
 * 70    3/08/07 11:06a Builder
 * AutoBuild
 * 
 * 69    2/15/07 10:14a Builder
 * AutoBuild
 * 
 * 68    2/15/07 8:28a Builder
 * AutoBuild
 * 
 * 67    2/15/07 8:03a Builder
 * AutoBuild
 * 
 * 66    2/05/07 11:41a Builder
 * AutoBuild
 * 
 * 65    2/05/07 11:11a Builder
 * AutoBuild
 * 
 * 64    1/15/07 9:43a Builder
 * AutoBuild
 * 
 * 63    12/20/06 10:52a Builder
 * AutoBuild
 * 
 * 62    11/09/06 8:56a Builder
 * AutoBuild
 * 
 * 61    10/03/06 9:31a Builder
 * AutoBuild
 * 
 * 60    9/26/06 9:40a Builder
 * AutoBuild
 * 
 * 59    9/18/06 1:36p Builder
 * AutoBuild
 * 
 * 58    9/18/06 12:36p Builder
 * AutoBuild
 * 
 * 57    9/18/06 11:22a Builder
 * AutoBuild
 * 
 * 56    7/31/06 9:49a Builder
 * AutoBuild
 * 
 * 55    7/18/06 11:43a Builder
 * AutoBuild
 * 
 * 54    6/29/06 1:49p Builder
 * AutoBuild
 * 
 * 53    6/27/06 11:28a Builder
 * AutoBuild
 * 
 * 52    6/27/06 10:45a Builder
 * AutoBuild
 * 
 * 51    6/19/06 11:49a Builder
 * AutoBuild
 * 
 * 50    6/16/06 1:22p Builder
 * AutoBuild
 * 
 * 49    6/14/06 2:59p Builder
 * AutoBuild
 * 
 * 48    5/31/06 2:29p Builder
 * AutoBuild
 * 
 * 47    5/08/06 1:29p Builder
 * AutoBuild
 * 
 * 46    4/25/06 2:10p Builder
 * AutoBuild
 * 
 * 45    4/19/06 8:38a Builder
 * AutoBuild
 * 
 * 44    3/10/06 2:31p Builder
 * AutoBuild
 * 
 * 43    3/09/06 11:07a Builder
 * AutoBuild
 * 
 * 42    2/27/06 10:33a Builder
 * AutoBuild
 * 
 * 41    1/18/06 3:03p Builder
 * AutoBuild
 * 
 * 40    1/12/06 10:02a Builder
 * AutoBuild
 * 
 * 39    12/30/05 9:44a Builder
 * AutoBuild
 * 
 * 38    12/21/05 1:09p Builder
 * AutoBuild
 * 
 * 37    12/05/05 10:09a Builder
 * AutoBuild
 * 
 * 36    11/30/05 12:26p Builder
 * AutoBuild
 * 
 * 35    11/16/05 12:26p Builder
 * AutoBuild
 * 
 * 34    11/07/05 9:28a Builder
 * AutoBuild
 * 
 * 33    10/20/05 3:13p Builder
 * AutoBuild
 * 
 * 32    10/10/05 1:45p Builder
 * AutoBuild
 * 
 * 31    10/07/05 2:51p Builder
 * AutoBuild
 * 
 * 30    10/03/05 3:40p Builder
 * AutoBuild
 * 
 * 29    9/06/05 3:06p Builder
 * AutoBuild
 * 
 * 28    8/31/05 10:27a Builder
 * AutoBuild
 * 
 * 27    8/31/05 10:23a Builder
 * AutoBuild
 * 
 * 26    8/31/05 9:49a Builder
 * AutoBuild
 * 
 * 25    8/31/05 9:30a Builder
 * AutoBuild
 * 
 * 24    8/25/05 10:31a Builder
 * AutoBuild
 * 
 * 23    8/03/05 3:32p Builder
 * AutoBuild
 * 
 * 22    8/02/05 2:52p Builder
 * AutoBuild
 * 
 * 21    8/01/05 3:52p Builder
 * AutoBuild
 * 
 * 20    8/01/05 3:45p Builder
 * AutoBuild
 * 
 * 19    7/28/05 8:54a Builder
 * AutoBuild
 * 
 * 18    7/21/05 3:39p Builder
 * AutoBuild
 * 
 * 17    7/18/05 9:11a Builder
 * AutoBuild
 * 
 * 16    7/13/05 12:01p Builder
 * AutoBuild
 * 
 * 15    7/08/05 11:09a Builder
 * AutoBuild
 * 
 * 14    7/08/05 10:41a Nashth
 * 
 * 13    6/27/05 2:43p Nashth
 * Switched major release number to 2.0  (From 1.0).
 * 
 * 12    6/24/05 2:55p Builder
 * AutoBuild
 * 
 * 11    6/24/05 2:52p Builder
 * AutoBuild
 * 
 * 10    6/23/05 10:04a Builder
 * AutoBuild
 * 
 * 9     6/14/05 7:10a Builder
 * AutoBuild
 * 
 * 8     6/14/05 6:59a Builder
 * AutoBuild
 * 
 * 7     5/27/05 4:11p Builder
 * AutoBuild
 * 
 * 6     4/28/05 10:39a Builder
 * AutoBuild
 * 
 * 5     4/28/05 9:03a Builder
 * AutoBuild
 * 
 * 4     4/28/05 8:56a Builder
 * AutoBuild
 * 
 * 3     4/28/05 8:32a Builder
 * AutoBuild
 * 
 * 2     4/28/05 8:25a Builder
 * AutoBuild
 * 
 * 1     4/27/05 1:43p Galvezdj
$NoKeywords: $
** End of Rev History **/ 


