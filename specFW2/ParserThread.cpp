//===========================================================================
//
//	Module Name:	ParserThread.cpp
//
// 	Function:		This routine handles all commands to the Spectrometer
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================

#include "StdAfx.h"
#include "SpecFW2.h"
#include "ParserThread.h"
#include "Debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParserThread
UINT CParserThread::tm_CParserThreadParseThis		= ::RegisterWindowMessage( L"CParserThread OnDoParseThis" );
UINT CParserThread::tm_CParserThreadParseThisInit	= ::RegisterWindowMessage( L"CParserThread OnDoParseThisInit" );
UINT CParserThread::tm_CParserThreadParseThisFatal	= ::RegisterWindowMessage( L"CParserThread OnDoParseThisFatal" );

IMPLEMENT_DYNCREATE( CParserThread, CWinThread )

CParserThread::CParserThread( void )
{
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalSectionClock);
	InitializeCriticalSection(&m_CriticalSectionSpecLog);

	m_nDataOutBuf	= new char[MAX_OUT_BYTES];		// Output buffer for command responses
	m_nDataBuf		= new char[MAX_DATA_BYTES];		// Output buffer for data
	m_nDataTransient= new char[MAX_OUT_BYTES];		// Output buffer for Transient
	m_lBlockAddress	= new unsigned long[MAX_BLOCKS];
	m_pReadData = new BYTE[1];	// Just a default until actually used


	// Fixed QPR #SITR6BUPY9 by setting pointers to NULL in constructor
	// Also mentioned in QPR #SITR6BUQC3
	m_pCmdPtr			= NULL;
	m_pCmdPtrSaved		= NULL;

	m_hWatchdogThreadUV	= NULL;
	m_hWatchdogThreadVIS= NULL;
	m_hOpticsThread		= NULL;
	m_hStatusThread		= NULL;
	m_hStatusThreadUV	= NULL;
	m_hStatusThreadVIS	= NULL;
	m_hHeaterThread		= NULL;
	m_hInitHwThread		= NULL;
	m_hReadCtThread		= NULL;
	m_hTMThread			= NULL;


	m_hStatusEvent		= NULL;
	m_hStatusEvent		= ::CreateEvent(NULL,TRUE, FALSE, _T("StatusThreadEvent"));
	m_hStatusEventUV	= NULL;
	m_hStatusEventUV	= ::CreateEvent(NULL,TRUE, FALSE, _T("StatusThreadEventUV"));
	m_hStatusEventVIS	= NULL;
	m_hStatusEventVIS	= ::CreateEvent(NULL,TRUE, FALSE, _T("StatusThreadEventVIS"));
	m_hHeaterEvent		= NULL;
	m_hHeaterEvent		= ::CreateEvent(NULL,TRUE, FALSE, _T("HeaterThreadEvent"));
	m_hInitHwEvent		= NULL;
	m_hInitHwEvent		= ::CreateEvent(NULL,TRUE, FALSE, _T("InitHwThreadEvent"));
	m_hReadCtEvent		= NULL;
	m_hOpticsEvent		= NULL;
	m_nNoStatusCount	= 0;
	m_nDebug			= 0;
	m_bRampMode			= false;

	CreateVegaDetector();
	ReadMotorTables();
	ReadTECoolerTemperatureTable();
	ReadMiscellaneousConstants();
	ReadCoarseTemperatureTable();
	m_sKernelVersion		= L"Unknown";
// jmo	GetKernelVersion();
	m_sKernelInformation	= L"Unknown";
// jmo	GetKernelInformation();
	m_sCPLDVersion			= L"Unknown";
// jmo	GetCPLDVersion();
	m_sPersonalityVersion	= L"Unknown";
// jmo	GetPersonalityVersion();

	// Must be able to handle single channel instruments prior to setup by manufacturing
	// Must also know before reading Detector FPGA Version
	if ( m_NvRam.InputB(SINGLE_CHANNEL_1) == 'S' && m_NvRam.InputB(SINGLE_CHANNEL_2) == 'C' )
		m_bSingleChannel = true;
	else if ( m_NvRam.InputB(SINGLE_CHANNEL_1) == 'D' && m_NvRam.InputB(SINGLE_CHANNEL_2) == 'C' )
		m_bSingleChannel = false;
	else
		m_bSingleChannel = true;

	m_sDetectorFPGAVersionUV	= L"Unknown";
	m_sDetectorFPGAVersionVIS	= L"Unknown";
	GetDetectorFPGAVersion();

	m_bFirstSum		= true;		// Indicates first read of summed reads
	m_bLastSum		= true;		// Indicates last read of summed reads
	m_bCancelFlag	= false;	// Indicates user cancel
	m_bTestMode		= false;	// Normal Mode
	m_bOverscanMode	= false;	// Normal Subarrays & Pixels
	m_nArraysUV		= 0;
	m_nArraysVIS    = 0;
	m_nTotalArrays	= 0;		// Number of arrays to read from LN command
	m_bDataAvailable= false;	// Indicates data is available for transfer
	m_nDataBlock	= 0;		// First Block Number
	m_lCurrentByte	= 8;		// Starting location of m_nDataOutBuf
	m_lBlockAddress[0] = 8;		// Making room for "TA00xxxx"

	int	 n;
	for (n = 0; n < MAX_VEGA_UV; n++)
		m_pArraysUV[n] = new CArrayData;
	for (n = 0; n < MAX_VEGA_VIS; n++)
		m_pArraysVIS[n] = new CArrayData;
//	for (n = 0; n < MAX_VEGA_SPARES; n++)
//		m_pArraysSpare[n] = new CArrayData;

	// Variables used for compatability
	m_cViewType		= 0;
//	m_cViewMode		= 0;
	m_cGrating		= '0';
	m_cChipMode		= VEGA;
	m_bSingleChannel= true;
	m_cShutterClosed= YES;
	m_cDetectorType	= VEGA;

	// Initialization Values
	m_restart_flag = 0;
	m_argon_flag = 0;
	m_Init_Motors_Status = 0;		// Motors startup uninitialized
	m_cUVres		= LOW;
	m_cVISres		= HIGH;
	m_nIntegrationTime = 100;
	m_cSlitType		= LOWRES;
	m_cSlitPos		= 0;
	m_cViewXPos_Axial		= 120;
	m_cViewYPos_Axial		= 80;
	m_cViewXPos_Radial		= 120;
	m_cViewYPos_Radial		= 80;	
	m_Shutter_Pos_Max		= 0;
	m_Shutter_Init_Flag		= false;	// Shutter initialization flag. Initialized = true.
	m_bLPmode		= false;
	m_bCCDPower		= false;
	m_nFatalCode	= NO_ERRORS;
	m_nPS24vdc		= 124;
	m_nUVcooler		= 166;
	m_nVIScooler	= 166;
	m_nTempccdUV	= 212;
	m_nTempccdVIS	= 212;
	m_nTempFan		= 124;

	m_bDetectorReadErr	= false;	// Indicates there was an error and information below describes the error
	m_wStatus			= 0;		// Two character code from GetReadStatusCode()
	m_wErrUVorVIS		= 0;		// 0=No error, 1=UV error, 2=VIS error 
	m_wErrSA			= 0;		// Error: Sub array number 
	m_wErrPixels		= 0;		// Error: Number of pixels
	m_wErrPixel			= 0;		// Error: Reading pixel number
	m_bErrHiRes			= false;	// Error: Low res readout mode = false, High res readout mode = true
	m_bErrOverScan		= false;	// Error: Reading overscan pixels
	m_bErrExtraPixels	= false;	// Error: Reading extra pixels
	m_bErrBitSet		= false;	// Error: Instantaneous setting when a camera register error bit is read


	m_TempTopPri		= 0;	// Primary sensor top			
	m_InterpolateTopPri	= 0;	// Primary sensor top interpolation bits	
	m_TempTopSec		= 0;	// Secondary sensor top			
	m_InterpolateTopSec	= 0;	// Secondary sensor top interpolation bits		
	m_TempBotPri		= 0;	// Primary sensor bottom			
	m_InterpolateBotPri	= 0;	// Primary sensor bottom interpolation bits	
	m_TempBotSec		= 0;	// Secondary sensor bottom			
	m_InterpolateBotSec	= 0;	// Secondary sensor bottom interpolation bits	
	
	m_Sensor_Top_Primary_Status		= 0;		// Top Primary sensor status
	m_Sensor_Bottom_Primary_Status	= 0;		// Bottom Primary sensor status
	m_Sensor_Top_Secondary_Status	= 0;		// Top Secondary sensor status
	m_Sensor_Bottom_Secondary_Status= 0;		// Bottom Secondary sensor status
	m_Calibration_Flag	= 0;					// Calibration Flag

    m_Temp_Sensors_OK   = true; // False if there is a fatal temperature sensor problem.
	m_Startup_flag		= true;	// Flag for detecting a startup condition - Used for integrator preset functionality
	
	m_bTurnCameraBackOnUV	= false;
	m_bTurnCameraBackOnVIS	= false;

	m_bHWInitComplete	= false;

	m_dwHeaterModeSetTime = GetTickCount(); // Initialize to current time

	cfgSys();

	m_IO.OutputW(CAMERA_RESET_REG, RESET_UV | RESET_VIS | RESET_MUX);
	Sleep(1);
	m_IO.OutputW(CAMERA_RESET_REG, CAMERAS_READY);
}

CParserThread::~CParserThread( void )
{
	short	n;
	for (n = 0; n < MAX_VEGA_UV; n++)
	{
		if (m_pArraysUV[n])
			delete	m_pArraysUV[n];
	}
	for (n = 0; n < MAX_VEGA_VIS; n++)
	{
		if (m_pArraysVIS[n])
			delete	m_pArraysVIS[n];
	}
//	for (n = 0; n < MAX_VEGA_SPARES; n++)
//	{
//		if (m_pArraysSpare[n])
//			delete	m_pArraysSpare[n];
//	}
	for (n = 0; n < MAX_ARRAY; n++)
	{
		delete m_pVegaArray[n];
	}

	if (m_hStatusEvent)
		::CloseHandle(m_hStatusEvent);
	if (m_hStatusEventUV)
		::CloseHandle(m_hStatusEventUV);
	if (m_hStatusEventVIS)
		::CloseHandle(m_hStatusEventVIS);
	if (m_hHeaterEvent)
		::CloseHandle(m_hHeaterEvent);
	if (m_hInitHwEvent)
		::CloseHandle(m_hInitHwEvent);
//	if (m_hReadCtEvent)
//		::CloseHandle(m_hReadCtEvent);
//	if (m_hOpticsEvent)
//		::CloseHandle(m_hOpticsEvent);

	delete [] m_nDataOutBuf;	// Output buffer for command responses
	delete [] m_nDataBuf;
	delete [] m_nDataTransient;
	delete [] m_lBlockAddress;
	delete [] m_pReadData;
	m_paBlockNo.RemoveAll();

	while (m_SpecLog.GetCount())
	{
		delete [] m_SpecLog.RemoveHead();
	}
}

BOOL CParserThread::InitInstance( void )
{
	// The StatusThread monitors the Spectrometer operating conditions
	// This includes the Gas flows, Power Supply & TE Cooler voltages, and Instrument & Detector temperatures
	// This thread can cause the Spectrometer to go into "Fatal" mode
	DWORD	dwID;

	m_hWatchdogThreadUV = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) watchdogThreadUV, this, CREATE_SUSPENDED, &dwID);
	if (m_hWatchdogThreadUV)
	{
		theApp.RegisterThread(L"WatchdogUV", dwID);
		::SetThreadPriority(m_hWatchdogThreadUV, THREAD_PRIORITY_NORMAL);
		::ResumeThread(m_hWatchdogThreadUV);
	}
	else
	{
		printf("WatchdogUV Thread creation failed\r\n");
	}
	Sleep(100);

	m_hWatchdogThreadVIS = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) watchdogThreadVIS, this, CREATE_SUSPENDED, &dwID);
	if (m_hWatchdogThreadVIS)
	{
		theApp.RegisterThread(L"WatchdogVIS", dwID);
		::SetThreadPriority(m_hWatchdogThreadVIS, THREAD_PRIORITY_NORMAL);
		::ResumeThread(m_hWatchdogThreadVIS);
	}
	else
	{
		printf("WatchdogVIS Thread creation failed\r\n");
	}
	Sleep(100);
// It was agreed that the Optics thread would no longer be implemented
// This thread was used to keep track of the hours the optics were exposed to the plasma
//	m_hOpticsThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) opticsThread, this, CREATE_SUSPENDED, &dwID);
//	if (m_hOpticsThread)
//	{
//		::SetThreadPriority(m_hOpticsThread, THREAD_PRIORITY_NORMAL);
//		::ResumeThread(m_hOpticsThread);
//	}
//	else
//	{
//		printf("Optics Thread creation failed\r\n");
//	}
//	Sleep(100);

	m_hStatusThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) statusThread, this, CREATE_SUSPENDED, &dwID);
	if (m_hStatusThread)
	{
		theApp.RegisterThread(L"Status", dwID);
		::SetThreadPriority(m_hStatusThread, THREAD_PRIORITY_NORMAL);
		::ResumeThread(m_hStatusThread);
	}
	else
	{
		printf("Status Thread creation failed\r\n");
	}
	Sleep(2000);

	m_hStatusThreadUV = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) statusThreadUV, this, CREATE_SUSPENDED, &dwID);
	if (m_hStatusThreadUV)
	{
		theApp.RegisterThread(L"StatusUV", dwID);
		::SetThreadPriority(m_hStatusThreadUV, THREAD_PRIORITY_NORMAL);
		::ResumeThread(m_hStatusThreadUV);
	}
	else
	{
		printf("UV Status Thread creation failed\r\n");
	}
	Sleep(500);

	// THN... Can change while working ... Apply in thread 
	m_nNoStatusCount = 0;
	m_hStatusThreadVIS = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) statusThreadVIS, this, CREATE_SUSPENDED, &dwID);
	if (m_hStatusThreadVIS)
	{
		theApp.RegisterThread(L"StatusVIS", dwID);
		::SetThreadPriority(m_hStatusThreadVIS, THREAD_PRIORITY_NORMAL);
		::ResumeThread(m_hStatusThreadVIS);
	}
	else
	{
		printf("VIS Status Thread creation failed\r\n");
	}
	Sleep(500);

	// Check if all temperatures have been read and are reasonable)
	if (m_TempTopPri == 0 || m_TempTopSec == 0 || m_TempBotPri == 0 || m_TempBotSec == 0)
	{
		int	n(0);
		do
		{
			Sleep(1000);
			++n;
		} while ((m_TempTopPri == 0 || m_TempTopSec == 0 || m_TempBotPri == 0 || m_TempBotSec == 0) && (n < 10));
	}

	// The HeaterThread monitors and controls the Heater temperatures
	m_hHeaterThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) heaterThread, this, CREATE_SUSPENDED, &dwID);
	if (m_hHeaterThread)
	{
		theApp.RegisterThread(L"Heater", dwID);
		::SetThreadPriority(m_hHeaterThread, THREAD_PRIORITY_NORMAL);
		::ResumeThread(m_hHeaterThread);
	}
	else
	{
		printf("Heater Thread creation failed\r\n");
	}
	Sleep(500);

	// The InitThread performs the initialization sequence need to get the Spectrometer into a "Ready" state
	// This includes controlling the Gas flows, monitoring the Tub temperatures, checks the motors,
	// turns on the TC Coolers, powers the Detectors, and checks the operation of the Detectors, Shutter, and Hg Lamp.
	// If everything is correct, it places the Spectrometer in "Ready" mode, else in "Fatal" mode.

	m_hInitHwThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) initHwThread, this, CREATE_SUSPENDED, &dwID);
	if (m_hInitHwThread)
	{
		theApp.RegisterThread(L"Init", dwID);
		::SetThreadPriority(m_hInitHwThread, THREAD_PRIORITY_NORMAL);
		::ResumeThread(m_hInitHwThread);
	}
	else
	{
		printf("Initialization Thread creation failed\r\n");
	}

	return TRUE;
}

void CParserThread::SuspendThreads()
{
	if (m_hStatusThread)
		::SuspendThread(m_hStatusThread);
	if (m_hStatusThreadUV)
		::SuspendThread(m_hStatusThreadUV);
	if (m_hStatusThreadVIS)
		::SuspendThread(m_hStatusThreadVIS);
	if (m_hHeaterThread)
		::SuspendThread(m_hHeaterThread);
//	if (m_hOpticsThread)
//		::SuspendThread(m_hOpticsThread);
}

void CParserThread::ResumeThreads()
{
	if (m_hStatusThread)
		::ResumeThread(m_hStatusThread);
	if (m_hStatusThreadUV)
		::ResumeThread(m_hStatusThreadUV);
	if (m_hStatusThreadVIS)
		::ResumeThread(m_hStatusThreadVIS);
	if (m_hHeaterThread)
		::ResumeThread(m_hHeaterThread);
//	if (m_hOpticsThread)
//		::ResumeThread(m_hOpticsThread);
}

int CParserThread::ExitInstance( void )
{
	if (m_hStatusEvent)
		WaitForSingleObject(m_hStatusEvent,		1000);
	if (m_hStatusEventUV)
		WaitForSingleObject(m_hStatusEventUV,	1000);
	if (m_hStatusEventVIS)
		WaitForSingleObject(m_hStatusEventVIS,	1000);
	if (m_hHeaterEvent)
		WaitForSingleObject(m_hHeaterEvent,		1000);
	if (m_hInitHwEvent)
		WaitForSingleObject(m_hInitHwEvent,		1000);
//	if (m_hReadCtEvent)
//		WaitForSingleObject(m_hReadCtEvent,		1000);
//	if (m_hOpticsEvent)
//		WaitForSingleObject(m_hOpticsEvent,		1000);

	DeleteCriticalSection(&m_CriticalSection);
	DeleteCriticalSection(&m_CriticalSectionClock);
	DeleteCriticalSection(&m_CriticalSectionSpecLog);

	return CWinThread::ExitInstance( );
}

bool CParserThread::ParseThis( char* buffer, int length )
{
	if (m_cInitFlag == NO)
	{
		// Cancel read ... Sets appropriate flag only ... No Reply message to PC
		//                 Not valid in fatal or init modes
		//  Note: CC must be in the first position of the message and 
		//        the only command on the message (remainder of message is ignored)
		if ( (buffer[0] == 'C') && (buffer[1] == 'C') )
		{
			if (m_bTransient == true)
				m_bAbortFlag = true;	// Used to abort a TM command
			else
				m_bCancelFlag = true;	// Used to cancel all other types of reads

			return true;
		}
		else
		return( CWinThread::PostThreadMessage( tm_CParserThreadParseThis,		(WPARAM) buffer, (LPARAM) length ) ? true : false ); 
	}
	else if (m_cInitFlag == YES)
		return( CWinThread::PostThreadMessage( tm_CParserThreadParseThisInit,	(WPARAM) buffer, (LPARAM) length ) ? true : false ); 
	else
		return( CWinThread::PostThreadMessage( tm_CParserThreadParseThisFatal,	(WPARAM) buffer, (LPARAM) length ) ? true : false ); 
};

BEGIN_MESSAGE_MAP( CParserThread, CWinThread )
	//{{AFX_MSG_MAP( CParserThread )
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(tm_CParserThreadParseThis,		OnDoParseThis		)
	ON_REGISTERED_THREAD_MESSAGE(tm_CParserThreadParseThisInit,	OnDoParseThisInit	)
	ON_REGISTERED_THREAD_MESSAGE(tm_CParserThreadParseThisFatal,OnDoParseThisFatal	)
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// CParserThread message handlers

UINT CParserThread::SetParent(CCommandServer* parent)
{
	m_ServingParent = parent;
	return S_OK;
}
//===========================================================================

BOOL CParserThread::Suspend()
{
	BOOL bMode(TRUE);
	MSG  msg;

	if (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		bMode = ::AfxGetApp()->PumpMessage();
	}
	return bMode;
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
UINT WINAPI CParserThread::watchdogThreadUV(LPVOID pParam)
{
	CParserThread *pThis = (CParserThread *) pParam;

	for ( ; ; )
	{
		// Wait until detector power is turned on
		do
		{
			Sleep(1000);
		} while (pThis->m_bCCDPower == false);

		// Initialize Watchdog Timer
		pThis->m_CameraIO.UVOutputW(WATCHDOG_INIT_UV, WATCHDOG_INIT);

		// Enable Watchdog on startup
		pThis->m_cWatchdogEnable = YES;
		pThis->m_bTurnCameraBackOnUV = false; //Reset Back On flag

		// Write to Watchdog Timer at least once every 3 seconds
		do
		{
			Sleep(1000);
			//....................................................
			// Switch camera back on restart watchdog
			if ( pThis->m_bTurnCameraBackOnUV )
			{
				//========================
				// TURN DETECTOR POWER ON
				//========================
				pThis->m_nPowerRegisterUV  |= POWER_ON;
				pThis->m_nPowerRegisterUV  &= ~LOW_SPEED;
				pThis->m_CameraIO.UVOutputW( POWER_SHUTDOWN_UV,  pThis->m_nPowerRegisterUV);
				pThis->m_CameraIO.UVOutputW( TIME_MODE_UV,  CAMERA_READY);
				//printf("Dog UV Pre-Break\r\n");
				Sleep(3000); // Wait 3 seconds before restarting watchdog
				//printf("Dog UV Break\r\n");
				break;
			}
			//....................................................
			if (pThis->m_cWatchdogEnable == YES)
			{
				pThis->m_CameraIO.UVOutputW(WATCHDOG_UV, 1);
			}
		} while (pThis->m_bCCDPower == true);

		// Special debugging to check timing of each thread
		if (pThis->m_nDebug & DEBUG_THREADS)
		{
			pThis->StoreThreadData(6);
		}
	}
}
//-----------------------------------------------------------------------------
UINT WINAPI CParserThread::watchdogThreadVIS(LPVOID pParam)
{
	CParserThread *pThis = (CParserThread *) pParam;

	for ( ; ; )
	{
		// Wait until detector power is turned on
		do
		{
			Sleep(1000);
		} while (pThis->m_bCCDPower == false);

		// Initialize Watchdog Timer
		if (pThis->m_bSingleChannel == false)
			pThis->m_CameraIO.VISOutputW(WATCHDOG_INIT_VIS, WATCHDOG_INIT);

		// Enable Watchdog on startup
		pThis->m_cWatchdogEnable = YES;
		pThis->m_bTurnCameraBackOnVIS = false; //Reset Back On flag

		// Write to Watchdog Timer at least once every 3 seconds
		do
		{
			Sleep(1000);
			//....................................................
			// Switch camera back on restart watchdog
			if ( pThis->m_bTurnCameraBackOnVIS )
			{
				//========================
				// TURN DETECTOR POWER ON
				//========================
				if (pThis->m_bSingleChannel == false)
				{
					pThis->m_nPowerRegisterVIS |= POWER_ON;
					pThis->m_nPowerRegisterVIS &= ~LOW_SPEED;
					pThis->m_CameraIO.VISOutputW(POWER_SHUTDOWN_VIS, pThis->m_nPowerRegisterVIS);
				}
				if (pThis->m_bSingleChannel == false)
				{
					pThis->m_CameraIO.VISOutputW(TIME_MODE_VIS, CAMERA_READY);
				}
				//printf("Dog VIS Pre-Break\r\n");
				Sleep(3000); // Wait 3 seconds before restarting watchdog
				//printf("Dog VIS Break\r\n");
				break;
			}
			//....................................................
			if (pThis->m_cWatchdogEnable == YES)
			{
				if (pThis->m_bSingleChannel == false)
					pThis->m_CameraIO.VISOutputW(WATCHDOG_VIS, 1);
			}
		} while (pThis->m_bCCDPower == true);

		// Special debugging to check timing of each thread
		if (pThis->m_nDebug & DEBUG_THREADS)
		{
			pThis->StoreThreadData(7);
		}
	}
}
//===========================================================================

/*** Revision History ***/


