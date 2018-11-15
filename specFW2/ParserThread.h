//===========================================================================
//
//	Module Name:	ParserThread.h
//
// 	Function:		Header file for CParserThread
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================


#pragma once


#include <stdlib.h>

#include "Hardware.h"
#include "IOFunctions.h"
#include "CameraIOFunctions.h"
#include "heat.h"
#include "temp.h"
#include "Zodiac.h"

#define	MAX_DATA_BYTES	524288	 // 0.5 MB buffer size   
#define MAX_OUT_BYTES	  32768

#define WM_REPLY_TO_COMMAND	WM_USER+1

//===========================================================================
// Byte translation Macro

#define REVWORD(w) (\
                    (((WORD)LOBYTE((WORD)(w))) << 8) | \
                     ((WORD)HIBYTE((WORD)(w)))         \
                   )
 
#define  REVDWORD(dw) (\
                       (((DWORD)REVWORD(LOWORD((DWORD)(dw)))) << 16) | \
                        ((DWORD)REVWORD(HIWORD((DWORD)(dw))))          \
                      ) 

//===========================================================================

class CCommandServer;

class CARRAY : public CObject
{
public:
	CARRAY() { };
	CARRAY(BYTE add, BYTE pix, BYTE ori, BYTE det, BYTE loc)
	{ address = add; pixels = pix; orient = ori; detector = det; location = loc; };
	~CARRAY() { };

	BYTE	address;	// 8-bit detector sub-array address
	BYTE	pixels;		// number of pixels in sub-array
	BYTE	orient;		// sub-array orientation: 0-normal, 1-inverse
	BYTE	detector;	// detector used: 0 - UV, 1 - VIS
	BYTE	location;	// 8-bit location in Table - for Vega

    CARRAY &operator=( CARRAY &array );
};
//===========================================================================

class CTABLE : public CObject
{
public:
	CTABLE() { };
	~CTABLE() { };

	CARRAY	*array;		// -> ARRAY array_tbl
};
//===========================================================================

class CArrayData: public CObject
{
public:
	BYTE	m_nDetector;
	BYTE	m_nAddress;
	BYTE	m_nPixels;
	BYTE	m_nOrientation;
	BYTE	m_nLocation;
	DWORD	m_dwEvenData[48];
	DWORD	m_dwOddData[48];
};
//===========================================================================

/////////////////////////////////////////////////////////////////////////////
// CParserThread thread
class CParserThread : public CWinThread
{
	DECLARE_DYNCREATE( CParserThread )
protected:

// Attributes
public:
	CRITICAL_SECTION	m_CriticalSection;			// General protection (all threads)
	CRITICAL_SECTION	m_CriticalSectionClock;		// Clock access protection (Start up)
	CRITICAL_SECTION	m_CriticalSectionSpecLog;	// m_SpecLog access protection

	// Status, Heater, and Init threads
	static UINT WINAPI			statusThread	  (LPVOID pParam);
	static UINT WINAPI			statusThreadUV	  (LPVOID pParam);
	static UINT WINAPI			statusThreadVIS	  (LPVOID pParam);
	static UINT WINAPI			heaterThread	  (LPVOID pParam);
	static UINT WINAPI			initHwThread  	  (LPVOID pParam);
	static UINT WINAPI			readCtThread	  (LPVOID pParam);
	static UINT WINAPI			opticsThread	  (LPVOID pParam);
	static UINT WINAPI			watchdogThreadUV  (LPVOID pParam);
	static UINT WINAPI			watchdogThreadVIS (LPVOID pParam);
	static UINT WINAPI			TMThread		  (LPVOID pParam);

	void				SuspendThreads();
	void				ResumeThreads();

	HANDLE				m_hStatusThread;
	HANDLE				m_hStatusThreadUV;
	HANDLE				m_hStatusThreadVIS;
	HANDLE				m_hHeaterThread;
	HANDLE				m_hInitHwThread;
	HANDLE				m_hReadCtThread;
	HANDLE				m_hOpticsThread;
	HANDLE				m_hWatchdogThreadUV;
	HANDLE				m_hWatchdogThreadVIS;
	HANDLE				m_hTMThread;

	HANDLE				m_hStatusEvent;
	HANDLE				m_hStatusEventUV;
	HANDLE				m_hStatusEventVIS;
	HANDLE				m_hHeaterEvent;
	HANDLE				m_hInitHwEvent;
	HANDLE				m_hReadCtEvent;
	HANDLE				m_hOpticsEvent;
	short				m_nNoStatusCount;

	unsigned int		Parser();
	unsigned int		ParserInit();
	unsigned int		ParserSleep();
	unsigned int		ParserFatal();
	unsigned int		Decoder();
	// Executable Commands
	unsigned int		cmd0C();
	unsigned int		cmd1D();
	unsigned int		cmd2D();
	unsigned int		cmd0H();
	unsigned int		cmd1H();
	unsigned int		cmd3T();
	unsigned int		cmd1V();
	unsigned int		cmd2V();
	unsigned int		cmd3V();
	unsigned int		cmd4V();
	unsigned int		cmd5V();
	unsigned int		cmdBS();
	unsigned int		cmdBX();
	unsigned int		cmdBY();
	unsigned int		cmdC0();
	unsigned int		cmdC1();
	unsigned int		cmdC2();
	unsigned int		cmdCB();
	unsigned int		cmdCD();
	unsigned int		cmdCH();
	unsigned int		cmdCM();
	unsigned int		cmdCP();
	unsigned int		cmdCS();
	unsigned int		cmdCT();
	unsigned int		cmdCX();
	unsigned int		cmdCY();
	unsigned int		cmdD0();
	unsigned int		cmdD1();
	void				DisplayThreadData();
	void				ClearThreadBuffer();
	void				StoreThreadData(short nThread);
	unsigned int		cmdD2();
	unsigned int		cmdDE();
	unsigned int		cmdDF();
	unsigned int		cmdDM();

	unsigned int		cmdDU();
	unsigned int		cmdDV();
	unsigned int		cmdEC();
	unsigned int		cmdES();
	unsigned int		cmdF1();
	unsigned int		cmdFC();
	unsigned int		cmdFS();
	unsigned int		cmdFX();
	unsigned int		cmdFY();
	unsigned int		cmdGB();
	unsigned int		cmdGC();
	unsigned int		cmdGD();
	unsigned int		cmdGE();
	unsigned int		cmdGG();
	unsigned int		cmdGL();
	unsigned int		cmdGM();
	unsigned int		cmdGS();
	unsigned int		cmdGT();
	unsigned int		cmdGV();
	unsigned int		cmdH0();
	unsigned int		cmdH1();
	unsigned int		cmdHG();
	unsigned int		cmdHK();
	unsigned int		cmdHO();
	unsigned int		cmdHS();
	unsigned int		cmdHU();
	unsigned int		cmdHV();
	unsigned int		cmdHX();
	unsigned int		cmdHY();
	unsigned int		cmdI1();
	unsigned int		cmdI2();
	unsigned int		cmdI3();
	unsigned int		cmdI4();
	unsigned int		cmdID();
	unsigned int		cmdIG();
	unsigned int		cmdIS();
	unsigned int		cmdIT();
	unsigned int		cmdLG();
	unsigned int		cmdLL();
	unsigned int		cmdLN();
	unsigned int		cmdLP();
	unsigned int		cmdLS();
	unsigned int		cmdLX();
	unsigned int		cmdLY();
	unsigned int		cmdMC();
	unsigned int		cmdMD();
	unsigned int		cmdMS();
	unsigned int		cmdMW();
	unsigned int		cmdMX();
	unsigned int		cmdMY();
	unsigned int		cmdNG();
	unsigned int		cmdOH();
	unsigned int		cmdPI();
	unsigned int		cmdPS();
	unsigned int		cmdPX();
	unsigned int		cmdPY();
	unsigned int		cmdRB();
	unsigned int		cmdRC();
	unsigned int		cmdRD();
	unsigned int		cmdRF();
	unsigned int		cmdRI();
	unsigned int		cmdRM();
	unsigned int		cmdRO();
	unsigned int		cmdRR();
	unsigned int		cmdRT();
	unsigned int		cmdRU();
	unsigned int		cmdRV();
	unsigned int		cmdSA();
	unsigned int		cmdSB();
	unsigned int		cmdSC();
	unsigned int		cmdSD();
	unsigned int		cmdSE();
	unsigned int		cmdSG();
	unsigned int		cmdSI();
	unsigned int		cmdSK();
	unsigned int		cmdSL();
	unsigned int		cmdSM();
	unsigned int		cmdSN();
	unsigned int		cmdSR();
	unsigned int		cmdST();
	unsigned int		cmdSU();
	unsigned int		cmdSV();
	unsigned int		cmdSY();
	unsigned int		cmdSZ();
	unsigned int		cmdTA();
	unsigned int		cmdTB();
	unsigned int		cmdTC();
	unsigned int		cmdTM();
	unsigned int		cmdTR();
	unsigned int		cmdTV();
	unsigned int		cmdTX();
	unsigned int		cmdTT();
	unsigned int		cmdUM();
	unsigned int		cmdVC();
	unsigned int		cmdVS();
	unsigned int		cmdVT();
	unsigned int		cmdVV();
	unsigned int		cmdVX();
	unsigned int		cmdVY();
	unsigned int		cmdWB();
	unsigned int		cmdWE();
	unsigned int		cmdXA();
	unsigned int		cmdXU();
	unsigned int		cmdXV();
	unsigned int		cmdXX();
	
// New commands added for Century B	start

	unsigned int		cmdHA();    // Move Toroid to the home (axial) position  
	unsigned int		cmdHB();	// Move Shutter to the home (axial) position
	unsigned int		cmdMB();	// Move Shutter to absolute position
	unsigned int		cmdQB();	// Get Shutter position MAX
	unsigned int		cmdBQ();	// Set Shutter position MAX	
	unsigned int		cmdNA();    // Get Toroid current position 
	unsigned int		cmdNB();	// Get Shutter current position 
	unsigned int		cmdUB();	// Get Shutter key radial position
	unsigned int		cmdUC();	// Get Shutter key closed position
	unsigned int		cmdXB();    // Set Shutter key radial position
	unsigned int		cmdXC();	// Set Shutter key closed position
	unsigned int		cmdKA();	// Move Toroid to radial position 
	unsigned int		cmdKB();	// Move Shutter to key radial position
	unsigned int		cmdKC();    // Move Shutter to key closed position
	unsigned int		cmdPA();	// Move Toroid and Shutter together to thier axial (home) positions
	unsigned int		cmdPR();	// Move Toroid and Shutter together to thier key radial positions
	unsigned int		cmdPK();	// Park motors
	unsigned int		cmdIM();	// Init motors
	unsigned int		cmdUA();	// Get Active Upper (Top) Sensor
	unsigned int 		cmdLA();	// Get Active Lower (Bottom) Sensor
	unsigned int 		cmdAU();	// Set Active Upper (Top) Sensor
	unsigned int 		cmdAL();	// Set Active Lower (Bottom) Sensor
	unsigned int 		cmdCA();	// Calibrate Sensors
	unsigned int		cmdJI();	// Just Initialize from Standby Mode

// New commands added for Century B	end
	
	VOID				ReturnStatus(WORD wStatus);
	unsigned int		cmdRead(WORD nDelay = 0);
	bool				continueWait(DWORD dwBegin, DWORD dwDelay = 5000);
	void				copyData();
	void				copyTransientData();
	void				moveData();
	void				average(WORD reads);
	WORD				readADC(WORD  wDebug, short nChannel, short nMUX, bool bChange);
	WORD				readDetectorADC(short nDetector, short nChannel, WORD &wDataVal);
	WORD				Average(WORD, WORD);
	void				ErrorMsg(unsigned int, unsigned int);
	void				PrintErrorMsg(unsigned int, unsigned int);
	void				ShutDown(unsigned int);
	short				delay(unsigned int nsDrive);
	bool				GetRegErrorBitsUV(WORD wTemp, bool bOtherErr, bool& bUVStatusError, bool& bUVStatusErrorInt, 
										  bool& bUVAddressOverflow, bool& bUVDataOverflow);
	bool				GetRegErrorBitsVIS(WORD wTemp, bool bOtherErr, bool& bVISStatusError, bool& bVISStatusErrorInt, 
										  bool& bVISAddressOverflow, bool& bVISDataOverflow);
	WORD				GetReadStatusCode(bool bUVReadError,    bool bUVPixelReadError,  bool bVISReadError,       bool bVISPixelReadError,
										  bool bUVStatusError,  bool bUVStatusErrorInt,  bool bUVAddressOverflow,  bool bUVDataOverflow,
										  bool bVISStatusError, bool bVISStatusErrorInt, bool bVISAddressOverflow, bool bVISDataOverflow);
	void				StoreDetectorReadInfo(WORD status, WORD wErrUVorVIS, WORD wErrSA, WORD wErrPixels,
										  bool bErrHiRes, bool bErrOverScan, bool bErrExtraPixels, WORD wErrPixel);

	// Conversion Functions
	void				c2b(char, unsigned int);
	void				i2b(unsigned int, unsigned int);
	void				c4c2b(unsigned int, unsigned int);
	void				s2b(unsigned int, unsigned int);
	void				long2b(long, unsigned int);
	void				l2b(long, unsigned int);
	void				h2b(long, unsigned int);
	void				h2b4(long, unsigned int);
	void				h2b2(long, unsigned int);
	unsigned short		b2c();
	unsigned short		b2s();
	unsigned short		b2h();
	BYTE				b2h2();
	void				b2a(unsigned long *lval);
	void				b2a4(unsigned long *lval);
	char				c2h();
	unsigned short		b2c4();
	WORD				c4h4();
	unsigned long		b2l();
	// Clock Functions
	long				GetTimeStamp(char* szTimeBuf); // calling array must be (9) bytes minimum
	void				GetDateTimeStamp(char* szDateTimeBuf);
	long				GetOffTime(WORD time_type);		// NORMAL_TIME or ARGON_TIME
	long				GetOnTime();					
	void				GetTimeValues(BYTE& byYear, BYTE& byMonth, BYTE& byDate, BYTE& byHour, BYTE& byMin, BYTE& bySec); 
	void				GetWakeupTimeValues(BYTE& byYear, BYTE& byMonth, BYTE& byDate, BYTE& byHour, BYTE& byMin); 
	void				SetWakeupTimeValues(BYTE byYearW, BYTE byMonthW, BYTE byDateW, BYTE byHourW, BYTE byMinW);
	void				GetRawTimeValues(BYTE& byYear, BYTE& byMonth, BYTE& byDate, BYTE& byHour, BYTE& byMin, BYTE& bySec);
	void				SetClockTime(BYTE byDay, BYTE byYear, BYTE byMonth, BYTE byDate, BYTE byHour, BYTE byMin, BYTE bySec);
	void				SaveOffTime(WORD time_type);	// NORMAL_TIME or ARGON_TIME

	// Motor Functions
	unsigned int		mv_slit();
	unsigned int		mv_viewx();
	unsigned int		mv_viewy();
	unsigned int		mv_motor(unsigned int, unsigned int, unsigned int);
	unsigned int		mv_abs_shutter(WORD);
	unsigned int		mv_abs_shutter_toroid(WORD, WORD);	
	void				set_shutter_position_status();	
	unsigned int		ramp_up(unsigned int, unsigned int, unsigned int);
	unsigned int		ramp_down(unsigned int, unsigned int, unsigned int);
	unsigned int		Configure_Motors(unsigned int);
	long 				calc_warmup_time();	
	
	// Heater Functions
	void				Sample();
	void				check_temps();
	short				channel2temp(short);

	void				Limit(long, long *, long);
	void				ServoControl();
	float				Get_Tub_Temp();
    void 				Set_Integrator_Power(float IP);	
	void				Calibrate_Temp_Sensors();
	bool 				Sensor_Fault_Detection();
	// Status Functions
	void				GetStatus();
	void				GetStatusInit();
	void				GetStatusFatal();
	void				savetime();
	// Miscellaneous Functions
	long				getTime(int);
	void				WaitDelay(int);
	unsigned short		cal_dc();
	void				batula(
						   BYTE*			pbytArray,  // Source ptr to an array of bytes
						   unsigned long*	pulArray,	// Destination ptr to an array of unsigned longs
						   int				nCnt		// Number of unsigned longs to copy 
						   );
	void				ulatba(
						   unsigned long*	pulArray,	// Source ptr to an array of unsigned longs
						   BYTE*			pbytArray,  // Destination ptr to an array of bytes
						   int				nCnt		// Number of unsigned longs to copy 
						   );
	unsigned short		cal_peak();
	unsigned short		cal_open();
	unsigned int		cfgHdwr();
	short				cfgSys();
	void				cfgMisc();
	void				setDefaultHeaterParameters(bool bProtect=true);
	void				readBRAM(bool bProtect=true);
	void				ReadCoarseTemperatureTable();
	void				GetKernelVersion();
// jmo	void				GetKernelInformation();
	void				GetCPLDVersion();
	void				GetPersonalityVersion();
	void				GetDetectorFPGAVersion();
	void				ReadTECoolerTemperatureTable();
	short				GetTECoolerTemperature(short nCounts);
	void				WriteLogEntry(char* prefix, char* pText);

	void				SmallDelay();
	BOOL				Suspend();
	//.........................................................
	// Protective functions
	unsigned short		Getm_nWarning( );
	unsigned short		Getm_nFatal( );
	void				Setm_cInitFlag( char cInitFlag );
	//.........................................................
	// Spectrometer log access functions
	void				AddToSpecLog(char* sZ);
	char*				RemoveFromSpecLog();
	//.........................................................
	void				setHeaterMode(short nsHM);
	void				getHeaterModeString(char* pBuff);
	//.........................................................
	HEXIO				m_IO;
	BYTEIO				m_NvRam;
	CameraHEXIO			m_CameraIO;

// Attributes
	char *				m_pCmdPtr;
	char *				m_pCmdPtrSaved;
	char *				m_nDataOutBuf;		// Output buffer for command responses
	char *				m_nDataTransient;

	CCommandServer *	m_ServingParent;

	CString	m_sKernelVersion;
	CString	m_sKernelInformation;
	CString	m_sCPLDVersion;
	CString	m_sPersonalityVersion;
	CString	m_sDetectorFPGAVersionUV;
	CString	m_sDetectorFPGAVersionVIS;

	char	m_cPowerMode;

	char * 	m_nDataBuf;
	UINT	m_nBytesRead;
	long	m_lCurrentByte;
	unsigned long *		m_lBlockAddress;

	bool	m_bCancelFlag;
	bool	m_bTestMode;
	bool	m_bOverscanMode;
	bool	m_bccdPower;
	WORD	m_nNoReads;
	WORD	m_nDelayTime;
	WORD	m_nLEDLevel;
	long	m_lTotalBytes;
	long	m_lDataLocation;
	bool	m_bFirstSum;
	bool	m_bLastSum;
	bool	m_bAverage;
	bool	m_bTransient;
	bool	m_bFirstTransient;
	bool	m_bDataAvailable;
	//............................................................................................................
	// Detector Read Error Information
	bool	m_bDetectorReadErr;	// Indicates there was an error and information below describes the error
	WORD	m_wStatus;			// Two character code from GetReadStatusCode()
	WORD	m_wErrUVorVIS;		// 0=No error, 1=UV error, 2=VIS error 
	WORD	m_wErrSA;			// Error: Sub array number 
	WORD	m_wErrPixels;		// Error: Number of pixels
	WORD	m_wErrPixel;		// Error: Reading pixel number
	bool	m_bErrHiRes;		// Error: Low res readout mode = false, High res readout mode = true
	bool	m_bErrOverScan;		// Error: Reading overscan pixels
	bool	m_bErrExtraPixels;	// Error: Reading extra pixels
	bool	m_bErrBitSet;		// Error: Instantaneous setting when a camera register error bit is read
	char	m_szDateTimeBuf[20];// When the error information was stored
	//............................................................................................................
	CPtrList	m_SpecLog;	// -> To spectrometer log strings
	//............................................................................................................
	// Version Information
	char			m_cSerial[ 6 ];
	char			m_cVersionNo[ 15 ];
	char			m_cMonth[ 2 ];
	char			m_cDay[ 2 ];
	char			m_cYear[ 2 ];
	unsigned short	m_nHardware_version;	// Flags

	// Register Variables
	unsigned short	 m_nSolenoidRegister;	// Gases & Other Solenoids
	unsigned short	 m_nMotorControl;		// Motor Controls
	unsigned short	 m_nDigitalControl;		// Miscellaneous Controls
	unsigned short	 m_nPowerRegisterUV;	// Power Shutdown Register - UV
	unsigned short	 m_nPowerRegisterVIS;	// Power Shutdown Register - VIS
	unsigned short	 m_nFPGA_VersionNo;

	// Subarray Variables
	char			m_cUVres;
	char			m_cVISres;
	WORD			m_nIntegrationTime;
	bool			m_bClearC;				// clear C flag
	BYTE			m_nArraysUV;
	BYTE			m_nArraysVIS;
	BYTE			m_nTotalArrays;

	CARRAY		*	m_pVegaArray[MAX_ARRAY];

	CTABLE			m_ReadTableUV[MAX_VEGA_UV];
	CTABLE			m_ReadTableVIS[MAX_VEGA_VIS];
	CTABLE			m_ReadTableSpare[MAX_VEGA_SPARES];

	CArrayData	*	m_pArraysUV[MAX_VEGA_UV];
	CArrayData	*	m_pArraysVIS[MAX_VEGA_VIS];
	CArrayData	*	m_pArraysSpare[MAX_VEGA_SPARES];

	// Status Variables
	char			m_cStartupMode;			// 0=SETUP,1=NOT_ON,2=COLD,A=HOT,R=READY,X=DIAG,Y=MFG,Z=AUTO
	char			m_cOperationMode;
	unsigned short	m_nPS24vdc;				// +24 VDC Power Supply
	unsigned short	m_nUVcooler;			// UV  COOLER Voltage
	unsigned short	m_nVIScooler;			// VIS COOLER Voltage
	unsigned short	m_nTempccdUV;			// UV  CCD Temperature  (Averaged)
	unsigned short	m_nTempccdVIS;			// VIS CCD Temperature (Averaged)
	WORD			m_wTempccdRawUV;		// UV  CCD Temperature... Not Averaged
	WORD			m_wTempccdRawVIS;		// VIS CCD Temperature... Not Averaged 	
	unsigned short	m_nTempUVMin;			// Minimum UV  Temperature
	unsigned short	m_nTempVISMin;			// Minimum VIS Temperature
	unsigned short	m_nTempUVLast;			// Latest UV  Temperature
	unsigned short	m_nTempVISLast;			// Latest VIS Temperature
			 char	m_cUVflag;				// Count of bad UV  Temperatures
			 char	m_cVISflag;				// Count of bad VIS Temperatures
	unsigned short	m_nTempFan;				// FAN Temperature
	unsigned short	m_nTempAmbient;			// Ambient Temperature Outside
	unsigned short	m_nGround;				// Power Supply Ground
	bool			m_bReached_temp;		// Flag for cold start
	short			m_nHeater_mode;			// 0 - Warmup
	DWORD			m_dwHeaterModeSetTime;	// Tick count when heater mode is set
	short			m_nCoarse_table[1024];  // Changed for 12 bit converter upgrade
	short			m_nTECoolerTemp[256];
	long			m_lServoCycleActual;	// Elapsed time since last ServoControl()
	long			m_lOptics_hours;		// Hours optics exposed to UV (actually seconds)
	unsigned short	m_nStatusNo;			// Number of last read A/D status
	char			m_cArgonFlowCnt;		// Argon Flow Failure Count
	char			m_cNitrogenFlowCnt;		// Nitrogen Flow Failure Count
	char			m_cWaterLeakCnt;		// Water Leak Failure Count
	char			m_cPS24vdcCnt;			// +24 VDC Power Supply Failure Count
	char			m_cUVcoolerCnt;			// UV COOLER Voltage Failure Count
	char			m_cVIScoolerCnt;		// VIS COOLER Voltage Failure Count
	char			m_cTempccdUVCnt;		// UV CCD Temperature Failure Count
	char			m_cTempccdVISCnt;		// VIS CCD Temperature Failure Count
	char			m_cTempFanCnt;			// FAN Temperature Failure Count
	char			m_cWaterDay;			// Day of last Water Valve Cycle
	bool			m_bCheckWaterFlag;		// Check time for Water Valve Cycle
	short			m_nWater_toggle;
	char			m_cFinLowCount;
	char			m_cFinHighCount;
	char			m_cTubLowCount;
	char			m_cTubHighCount;
	char			m_cFinOverCount;
	char			m_cTubOverCount;
	char			m_cTopOverCount;
	char			m_cBottomOverCount;
	char			m_cWarm2StableCount;
	char			m_cNormal2WarmCount;
	char			m_cTubWarmupCount;
	char			m_cUVCoolerCount;
	char			m_cVISCoolerCount;

	// Motor Variables
	unsigned short	m_nSlitHigh;			// HIGH SLIT, POS 0, Relative from HOME
	unsigned short	m_nSlitLow;				// LOW SLIT, POS 0, Relative from HOME
	unsigned short	m_nSlitBarn;			// BARN DOOR, Centered, Relative from HOME
	char			m_cSlitType;			// new SLIT type
	char			m_cSlitTno;				// current SLIT type
	char			m_cSlitPos;				// new SLIT position
	char			m_cSlitPno;				// current SLIT position
	unsigned short	m_nSlitStep;			// new SLIT step
	unsigned short	m_nSlitSno;				// current SLIT step
	unsigned short	m_nMaxSlitStep;			// MAXIMUM STEPS - FROM BRAM
	
	unsigned short	m_Init_Motors_Status;	// Motors init status; 1 = initialized; 0 = uninitialized	
	unsigned short	m_nViewXPeak_Axial;		// Key VIEW X location
	unsigned short	m_nViewXHg_Axial;		// Key VIEW X Hg location
	unsigned short	m_nViewY15mm_Axial;		// Key VIEW Y location
	unsigned short	m_nViewYHg_Axial;		// Key VIEW Y Hg location
	char			m_cViewXPos_Axial;		// new VIEW X position
	char			m_cViewXPno_Axial;		// current VIEW X position
	unsigned short	m_nViewXStep_Axial;		// new VIEW X step number
	unsigned short	m_nViewXno_Axial;		// current VIEW X step number
	unsigned short	m_nMaxXSteps_Axial;		// MAXIMUM STEPS - FROM BRAM
	char			m_cViewYPos_Axial;		// new VIEW Y position
	char			m_cViewYPno_Axial;		// current VIEW Y position
	unsigned short	m_nViewYStep_Axial;		// new VIEW Y step number
	unsigned short	m_nViewYno_Axial;		// current VIEW Y step number
	unsigned short	m_nMaxYSteps_Axial;		// MAXIMUM STEPS - FROM BRAM
	bool			m_bLimitXFlag;			// Special case for LIMIT X command
	bool			m_bLimitYFlag;			// Special case for LIMIT Y command	
	unsigned short	m_nViewXStp_Axial[ MAX_VIEW_X_AXIAL + 1 ];
	unsigned short	m_nViewYStp_Axial[ MAX_VIEW_Y_AXIAL + 1 ];	
	unsigned short	m_nViewXPeak_Radial;	// Key VIEW X location
	unsigned short	m_nViewY15mm_Radial;	// Key VIEW Y location
	char			m_cViewXPos_Radial;		// new VIEW X position
	char			m_cViewXPno_Radial;		// current VIEW X position
	unsigned short	m_nViewXStep_Radial;	// new VIEW X step number
	unsigned short	m_nViewXno_Radial;		// current VIEW X step number
	unsigned short	m_nMaxXSteps_Radial;	// MAXIMUM STEPS - FROM BRAM
	char			m_cViewYPos_Radial;		// new VIEW Y position
	char			m_cViewYPno_Radial;		// current VIEW Y position
	unsigned short	m_nViewYStep_Radial;	// new VIEW Y step number
	unsigned short	m_nViewYno_Radial;		// current VIEW Y step number
	unsigned short	m_nMaxYSteps_Radial;	// MAXIMUM STEPS - FROM BRAM
	unsigned short	m_nViewXStp_Radial[ MAX_VIEW_X_RADIAL + 1 ];
	unsigned short	m_nViewYStp_Radial[ MAX_VIEW_Y_RADIAL + 1 ];
	
	unsigned short	m_nWaitTime;			// time between step pulses
	unsigned short	m_nLooker;				// LOOKER to check during moves
	char			m_cLookerCnt;			// Count of Looker errors - 2 for error
//	char			m_cViewMode;			// Dual View Spectrometer: DUALVIEW_NEW (2) only
	char			m_cViewType;			// Axial = 0 (old Normal) or Radial = 1 (old dualview)
	bool			m_bSingleChannel;		// Single Channel Spectrometer: 0=NO, 1=YES

	char			m_cGrating;
	bool			m_bLPmode;				// Hg Lamp Mode
	unsigned char	*m_pChannel_address;	// Address of channel to read
	unsigned short	m_nSlitDelta;
	unsigned short	m_nSlitHome;
	unsigned short	m_nSlitHiStp[ 10 ];
	unsigned short	m_nSlitLoStp[ 8 ];
	unsigned short	m_nSlitBdStp;
	
	unsigned short	m_nShutter_Pos;	        // Shutter current position (1..SHUTTER_MAX_STEPS)
	unsigned short	m_Shutter_Pos_Max;	    // Shutter maximum position (set in BRAM) (1..SHUTTER_MAX_STEPS)
	char			m_cShutter_Pos_Status;	// Shutter current status position
	bool			m_Shutter_Init_Flag;	// Shutter initialization flag. Initialized = true.
	char			m_cToroid_Pos_Status;	// Toroid current status position	
	
	// Heater variables

	unsigned short	m_TempTopPri;			// Primary sensor top
	unsigned short	m_InterpolateTopPri;	// Primary sensor top interpolation bits
	unsigned short	m_TempTopSec;			// Secondary sensor top
	unsigned short	m_InterpolateTopSec;	// Secondary sensor top interpolation bits	
	unsigned short	m_TempBotPri;			// Primary sensor bottom
	unsigned short	m_InterpolateBotPri;	// Primary sensor bottom interpolation bits
	unsigned short	m_TempBotSec;			// Secondary sensor bottom
	unsigned short	m_InterpolateBotSec;	// Secondary sensor bottom interpolation bits

	unsigned short	m_Sensor_Top_Primary_Status;		// Top Primary Sensor status
	unsigned short	m_Sensor_Bottom_Primary_Status;		// Bottom Primary Sensor status
	unsigned short	m_Sensor_Top_Secondary_Status;		// Top Secondary Sensor status
	unsigned short	m_Sensor_Bottom_Secondary_Status;	// Bottom Secondary Sensor status

	unsigned short	m_Calibration_Flag;		// Calibration Flag
	
    bool            m_Startup_flag;			// Flag for detecting a startup condition - Used for integrator preset functionality
	long			m_lPI_out;				// PI tub output power in percent (0..100)
	long			m_lPI_out2;				// PI lid output power in percent (0..100)
	bool			m_Temp_Sensors_OK;		// False if there is a fatal temperature sensor problem.	


	long			m_lHeat_warnings;		// Bit map of outstanding warnings
	long			m_lSensor_failed_flags[4];
	long			m_lTimeOn[2];			// Number of half-cycles that heaters are on
	long			m_lTimeOff[2];			// Number of half-cycles that heaters are off
	long			m_lServoTimer;
	long			m_lServoWattage[2];		// Heat rate "requested" by ServoControl routine

	long			m_lLineFrequency;		// 50 or 60
	long			m_lHCCalls;				// time remaining to execute HeaterControl() before calling ServoControl()
	long			m_lHeaterCycleActual;	// Elapsed time since last HeaterControl()
	long			m_lHeaterTimer;
	long			m_lDelivHeat[2];		// Amount of heat actually delivered by heaters
	long			m_lRequestHeat[2];		// "Requested" heat (ServoWattate * m_lHeaterCycleActual)
	long			m_lDutyCycle[2];		// Ratio of m_lTimeOn / (m_lTimeOn + m_lTimeOff) for heater power
	long			m_lHeaterWattage[2];	// Heat rate calculated by HeaterControl routine (m_lServoWattage corrected by HeatBuf)
	long			m_lHeatBuffer[2];		// Cumulative storage for adding m_lRequestHeat and subtracting DeliveredHeat


	//......................................................................................................
	// Clock Variables  ... Protected by m_CriticalSectionClock

	char			m_cOFFyear;				// YEAR of Turn OFF 
	char			m_cOFFmonth;			// MONTH of Turn OFF 
	char			m_cOFFdate;				// DATE of Turn OFF 
	char			m_cOFFhour;				// HOUR of Turn OFF 
	char			m_cOFFminute;			// MINUTE of Turn OFF 
	char			m_cOFFsecond;			// SECOND of Turn OFF 
	char			m_cONyear;				// YEAR of Turn ON 
	char			m_cONmonth;				// MONTH of Turn ON 
	char			m_cONdate;				// DATE of Turn ON 
	char			m_cONhour;				// HOUR of Turn ON 
	char			m_cONminute;			// MINUTE of Turn ON 
	char			m_cONsecond;			// SECOND of Turn ON 
	long			m_lOFFtime;				// Turn OFF Time in Seconds
	long			m_lONtime;				// Turn ON Time in Seconds
	long			m_lCurrentTime;			// Current Time in Seconds
	long			m_lKeyTime;				// Displayable Time in Seconds (Time to system Ready)
	bool			m_bHolding;				// Initialization count-down on HOLD
											//  (Purged for more than 1 hour, but Tub Not at temperature)
	//......................................................................................................
 	char			m_cCurrentPurge;		// Current position of Purge Valves

	// Transient Signal Mode Variables
	WORD			m_wTMdelay;
	WORD			m_wTMitime;
	WORD			m_wTMfreq;
	char			m_cTMmode;

	// Misc Variables
	unsigned short	m_nBRAMerrors;			// BRAM errors found during cfgsys on startup
	unsigned short	m_nErrorCnt;			// Number of errors in m_nErrorOut[] buffer
	char			*m_pErrorPtr;			// ptr -> m_nErrorOut[] byte location
	char			m_nErrorOut[MAX_ERROR*4];	// Unsolicited Response buffer
	bool			m_bTestFlag;
	char			m_cInitFlag;			// initialization flag  (YES / NO / FATAL)
	bool			m_bHWInitComplete;		// Hardware Initialization Complete flag
	bool			m_bCCDPower;			// CCD POWER enabled: ON /OFF
	unsigned short	m_nWarning;				// Misc Warning Indicator
	unsigned short	m_nFatal;				// Misc Fatal Indicator
	unsigned short	m_nOPmode;				// Diag: 0=NORMAL, 1=TEST
	unsigned short	m_nTestMode;			// Test Mode:  0=NORMAL, 1=ON
	short			m_nPlasma_lit;			// Plasma lit flag 0 - off, 1 - on
	bool			m_bUnsolicitedMsg;		// Unsolicited Messages Enabled / Disabled
	bool			m_bSEmode;				// SERVO ENABLE Mode
	bool			m_bSleepFlag;			// Sleep mode active: YES / NO
	char			m_cRevMPC;				// MPC Revision Level
	char			m_cRevCDS;				// CDS Revision Level
	unsigned short	m_nFatalCode;			// '00' or FATAL CODE for Status Command
	bool			m_bSyncMode;			// START INTEGRATION SYNC Mode: 0=OFF, 1=ON
	bool			m_bAbortFlag;			// Abort any command string
	unsigned long	m_lWatchdog_ok;
	unsigned long	m_lWatchdog_enable;
	unsigned short	m_nLineCount;			// Used to calculate Line Frequency
	bool			m_bSHmode;				// SHUTTER Mode
	unsigned short	m_nDebug;				// Debug output flag
	unsigned long	m_lDebug_print;			// Flag for DEBUG_DEGREES delay
	DWORD			m_dwLastDebugTime;
	bool			m_bRampMode;
	char			m_cWatchdogEnable;		// Enable/Disable Watchdog timer
	bool			m_bTurnCameraBackOnUV;	// True will restart camera in watchdog thread
	bool			m_bTurnCameraBackOnVIS;	// True will restart camera in watchdog thread

	WORD			m_nTransientReads;
	BYTE *			m_pReadData;
	WORD			m_nFullBlocks;
	long			m_lBlockSize;
	WORD			m_nBlockCount;
	long			m_lTransientBytes;
	CPtrArray		m_paBlockNo;				// Array of ptrs to transient blocks in m_pReadData 
												//  (dynamic... includes list of blocks not transfered by TT cmd)
	unsigned short	m_nDataBlock;				// current Data Block Number
			 bool	m_bDataFlag;				// data available flag
	unsigned short	m_nBlockOut;				// block number to output
	unsigned short	m_nLastBlockSent;			// last blocknumber sent to the host
	bool			m_bTransientOverflow;		// indicates a problem - stop reads
	DWORD			m_dwTTCounter;				// Counts # TTs received during a transient read
	unsigned short  m_restart_flag;				// Restart Flag for Just Initialize (JI)
	unsigned short  m_argon_flag;				// Argon Tank Change Flag

	// Variables used for compatability
	char	m_cDetectorType;		// 0=Zodiac, 1=Vega
	char	m_cChipMode;			// 0=Zodiac, 1=Vega, 2=Vega SCA
	char	m_bNewDataFormatFlag;	// data format flag: Always True - New
	char	m_cShutterClosed;
	char	m_cSerialNo[11];
	char	m_cUVCoefficient1[9];
	char	m_cUVCoefficient2[9];
	char	m_cUVCoefficient3[9];
	char	m_cVISCoefficient1[9];
	char	m_cVISCoefficient2[9];
	char	m_cVISCoefficient3[9];
	char	m_cUVSerialNo[6];
	char	m_cVISSerialNo[6];

	WORD	m_n5vdcPosUV;		// +5 Vdc
	WORD	m_n5vdcPosVIS;		// +5 Vdc
	WORD	m_nVahPosUV;		// +Vah
	WORD	m_nVahPosVIS;		// +Vah
	WORD	m_n12vdcPosUV;		// +12 Vdc
	WORD	m_n12vdcPosVIS;		// +12 Vdc
	WORD	m_n15vdcPosUV;		// +15 Vdc
	WORD	m_n15vdcPosVIS;		// +15 Vdc
	WORD	m_n15vdcNegUV;		// -15 Vdc
	WORD	m_n15vdcNegVIS;		// -15 Vdc
	WORD	m_nVahNegUV;		// -Vah
	WORD	m_nVahNegVIS;		// -Vah
	WORD	m_n5vdcNegUV;		// -5 Vdc
	WORD	m_n5vdcNegVIS;		// -5 Vdc


// Operations
public:
	UINT	SetParent(CCommandServer* parent);

	void	CreateVegaDetector();
	void	ReadMotorTables();
	void	ReadMiscellaneousConstants();

	bool	ParseThis( char* buffer, int length );

	CParserThread( void );           // protected constructor used by dynamic creation
	virtual	~CParserThread( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParserThread)
	public:
	virtual BOOL InitInstance( void );
	virtual int ExitInstance( void );
	//}}AFX_VIRTUAL

// Implementation
private:
	static UINT	tm_CParserThreadParseThis;
	static UINT	tm_CParserThreadParseThisInit;
	static UINT	tm_CParserThreadParseThisFatal;
	WORD		m_nCmd;

	// Generated message map functions
	//{{AFX_MSG(CParserThread)
	afx_msg void OnDoParseThis(			WPARAM wParam, LPARAM lParam );
	afx_msg void OnDoParseThisInit(		WPARAM wParam, LPARAM lParam );
	afx_msg void OnDoParseThisFatal(	WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/*** Revision History ***/

