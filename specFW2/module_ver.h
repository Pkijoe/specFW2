#pragma once

#ifndef _Module_Ver_h
#define _Module_Ver_h

// Every subdirectory with a DSP will get a MODULE_VER.H which will be individually maintained for each module and have the following:

#define MOD_FILEVERSION 2,0,0,PROD_BUILDNUMBER
//#define MOD_sFILEVERSION "2,0,0,\0" PROD_sBUILDNUMBER

#define MOD_sFILEDESCRIPTION "AAICP\0" 
#define MOD_sINTERNALNAME    "AAICP\0"
#define MOD_sORIGFILENAME    "SPECFW_AAICP.exe\0"



#endif

//===========================================================================


/*** Revision History ***

09/16/16    KR      Initial changes for Century B from 1.0.1.xxx to 2.0.0.xxx.
02/22/17	KR		Miscellaneous clean up and remove build warning

  ** End of Rev History **/