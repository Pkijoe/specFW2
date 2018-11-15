//===========================================================================
//
//	Module Name:	Zodiac.h
//
// 	Function:	  	This file contains the data structures and
//		 			definitions used in the zodiac firmware.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================

#if !defined(ZODIAC_H_FILE_)
#define ZODIAC_H_FILE_

// COMMAND CODES

#define	CMD_0C			0x3043	// Get Heater ON/OFF

#define	CMD_1D			0x3144	// Get Debug option flags
#define CMD_2D			0x3244	// Get Hardware version flags

#define CMD_0H			0x3048	// Get Heater Mode
#define	CMD_1H			0x3148	// Get Line Frequency

#define	CMD_1V			0x3156	// Get the Kernel image version
#define CMD_2V			0x3256	// Get the complete Kernel version info

#define CMD_3T			0x3354	// Get Temperatures(8)
#define CMD_3V			0x3356	// Get HPLD version number
#define CMD_4V			0x3456	// Get Personality version number
#define CMD_5V			0x3556	// Get Detector version number

#define CMD_AU			0x4155	// Set Active Upper (Top) Sensor
#define CMD_AL			0x414c	// Set Active Lower (Bottom) Sensor
#define	CMD_AR			0x4152	// Set Auto Reset Time Interval
#define CMD_BS			0x4253	// Set MAX STEPS for SLIT in BRAM
#define CMD_BX			0x4258	// Set MAX STEPS for VIEW X in BRAM
#define CMD_BY			0x4259	// Set MAX STEPS for VIEW Y in BRAM

#define	CMD_C0			0x4330	// Set Heater ON/OFF
#define	CMD_C1			0x4331	// Set ServoControl wait cycles
#define	CMD_C2			0x4332	// Set HeaterControl wait cycles
#define CMD_CA			0x4341	// Calibrate Sensors
#define CMD_CB			0x4342	// Get current block -- added RDW
#define CMD_CC			0x4343	// Cancel Command
#define CMD_CD			0x4344	// Converter Data
#define CMD_CH			0x4348	// Chiller command
#define CMD_CM			0x434d	// Clear Memory
#define CMD_CP			0x4350	// Clear Pointer - same as CMD_CM
#define CMD_CR			0x4352	// Chart Recorder
#define CMD_CS			0x4353	// Configure SLIT
#define CMD_CT			0x4354	// Set Chip Type (Zodiac or Vega)
#define CMD_CX			0x4358	// Configure VIEW X
#define CMD_CY			0x4359	// Configure VIEW Y

#define	CMD_D0			0x4430	// Set heater parameters to defaults
#define	CMD_D1			0x4431	// Set Debug option flags
#define CMD_D2			0x4432	// Set Hardware version flags
#define CMD_DA			0x4441	// Program D/A Converters
#define CMD_DE			0x4445	// Get Detector Error string
#define CMD_DF			0x4446	// Set Data Format
#define	CMD_DK			0x444b	// Download Kernel Image - not used
#define CMD_DM			0x444d	// Set Detector Mode if Vega
#define CMD_DU			0x4455	// Set s/n UV detector
#define CMD_DV			0x4456	// Set s/n VIS detector
#define CMD_EC			0x4543	// Get Error Count
#define CMD_ES			0x4553	// Get ENVIRONMENTAL Status
#define CMD_EX			0x4558	// Exit application
#define CMD_F1			0x4631	// Set Tub heater PI integrator power
#define CMD_FC			0x4643	// Set CLOCK Time
#define CMD_FS			0x4653	// Set KEY STEPS for SLIT in BRAM
#define CMD_FX			0x4658	// Set KEY STEPS for VIEW X in BRAM
#define CMD_FY			0x4659	// Set KEY STEPS for VIEW Y in BRAM
#define CMD_GB			0x4742	// Get Byte from BRAM
#define CMD_GC			0x4743	// Get CLOCK Time
#define CMD_GD			0x4744	// Get DUAL TORCH mode
#define CMD_GE			0x4745	// Get Error Messages
#define CMD_GG			0x4747	// Get GRATING
#define CMD_GL			0x474c	// Get Looker Status
#define CMD_GM			0x474d	// Get Detector Mode
#define CMD_GS			0x4753	// Get Single Channel Mode (YES or NO)
#define CMD_GT			0x4754	// Get TIME to READY value
#define CMD_GV			0x4756	// Get View Type (Normal or Dual)

#define	CMD_H1			0x4831	// Set Line Frequency
#define CMD_HA			0x4841	// Move Toroid to the home (axial) position
#define CMD_HB			0x4842	// Move Shutter to the home (axial) position
#define CMD_HG			0x4847	// Go to KEY Hg positions (VIEW X & Y)
#define CMD_HK			0x484b	// Set KEY Hg STEPS for VIEW X & Y in BRAM
#define CMD_HO			0x484f	// Get Optics Counter Hours
#define CMD_HS			0x4853	// Find SLIT HOME
#define CMD_HU			0x4855	// Set UV Resolution
#define CMD_HV			0x4856	// Set VIS Resolutuon
#define CMD_HX			0x4858	// Find VIEW X HOME
#define CMD_HY			0x4859	// Find VIEW Y HOME
#define CMD_I1			0x4931	// First initialization command - not used
#define CMD_I2			0x4932	// Second initialization command
#define CMD_I3			0x4933	// Third initialization command
#define CMD_I4			0x4934	// Fourth initialization command
#define	CMD_ID			0x4944	// Get ID
#define	CMD_IM			0x494D	// Initialize motors command
#define CMD_IS			0x4953	// Get instrument serial number
#define CMD_IT			0x4954	// Set Integration Time
#define CMD_JI			0x4a49	// Just Initialize (Restart configured hardware)
#define CMD_KA			0x4b41  // Move Toroid to key radial position 
#define CMD_KB			0x4b42  // Move Shutter to key radial position
#define CMD_KC			0x4b43  // Move Shutter to key closed position
#define CMD_LA			0x4c41	// Get Active Lower (Bottom) Sensor
#define CMD_LL			0x4c4c	// Lamp Level - Autotester
#define CMD_LN			0x4c4e	// LIST N Subarrays
#define CMD_LP			0x4c50	// Hg LAMP
#define CMD_LS			0x4c53	// Find SLIT LIMIT
#define CMD_LX			0x4c58	// Find VIEW X LIMIT
#define CMD_LY			0x4c59	// Find VIEW Y LIMIT
#define CMD_MA			0x4d41	// Move Toroid to absolute position 
#define CMD_MB			0x4d42	// Move Shutter to absolute position 
#define CMD_MC			0x4d43	// READ a Location
#define CMD_MD			0x4d44	// Startup MODE
#define CMD_MS			0x4d53	// Move SLIT in STEPS
#define CMD_MW			0x4d57	// WRITE to a Location
#define CMD_MX			0x4d58	// Move VIEW X in STEPS
#define CMD_MY			0x4d59	// Move VIEW Y in STEPS
#define CMD_NA			0x4e41	// Get Toroid current position 
#define CMD_NB			0x4e42	// Get Shutter current position 
#define CMD_NX			0x4e58	// Set NEW DUAL KEY STEPS for VIEW X in BRAM
#define CMD_NY			0x4e59	// Set NEW DUAL KEY STEPS for VIEW Y in BRAM
#define CMD_OH			0x4f48	// Set Optics Counter Hours
#define CMD_PA			0x5041  // Move Toroid and Shutter together to their axial (home) positions
#define CMD_PR			0x5052  // Move Toroid and Shutter together to their key radial positions
#define CMD_PK			0x504B  // Park motors
#define CMD_PI			0x5049	// Set Plasma Status (On or Off)
#define CMD_PS			0x5053	// Position SLIT
#define CMD_PX			0x5058	// Position VIEW X
#define CMD_PY			0x5059	// Position VIEW Y
#define CMD_QA			0x5141	// Get TOROID_MAX_STEPS
#define CMD_QB			0x5142	// Get shutter position MAX
#define CMD_BQ			0x4251	// Set shutter position MAX
#define	CMD_RA			0x5241	// Get Auto Reset Time Interval
#define CMD_RB			0x5242	// Read Battery RAM - cfgSys
#define CMD_RC			0x5243	// READ CONTINUOUSLY
#define CMD_RD			0x5244	// READ DELAY - similar to READ ONCE
#define CMD_RF	 		0x5246	// READ FIAS - similar to READ ONCE
#define CMD_RI			0x5249	// Re-initialize Hardware
#define CMD_RM			0x524d	// Ramp Mode - Sarnoff test
#define CMD_RO			0x524f	// READ ONCE
#define CMD_RR	 		0x5252	// READ RAM test results
#define CMD_RT			0x5254	// Read contact closure command
#define CMD_RU			0x5255	// Read UV detector coefficiants
#define CMD_RV			0x5256	// Read VIS detector coefficants

#define CMD_SA			0x5341	// SIZE ALL
#define CMD_SB			0x5342	// SIZE BLOCK
#define CMD_SC			0x5343	// Set Single Channel Mode
#define CMD_SD			0x5344	// SUM DATA
#define CMD_SE			0x5345	// SERVO ENABLE
#define CMD_SG			0x5347	// Set GRATING
#define CMD_SH			0x5348	// SHUTTER Command
#define CMD_SI			0x5349	// Set Instrument serial number
#define CMD_SK			0x534b	// Set New Key Slit Position
#define CMD_SL			0x534c	// SUM LONG
#define CMD_SM			0x534d	// Sleep Mode
#define CMD_SN			0x534e	// Read the s/n of the detectors both UV and VIS
#define CMD_SR			0x5352	// Software Reset
#define CMD_ST			0x5354	// Status
#define CMD_SU			0x5355	// Set UV detector coefficants
#define CMD_SV			0x5356	// Set VIS detector coefficants
#define CMD_SY			0x5359	// SYNC Command - not used - always off
#define CMD_SZ			0x535a	// Get Spectrometer log item

#define CMD_TA			0x5441	// TRANSMIT ALL
#define CMD_TB			0x5442	// TRANSMIT BLOCK
#define CMD_TC			0x5443	// Get Detector Chip Type
#define CMD_TM			0x544d	// Transient Signal Mode
#define CMD_TR			0x5452	// TEMPERATURE CONSTANTS - READ
#define CMD_TT			0x5454	// TRANSMIT all BLOCK thus far
#define CMD_TV			0x5456	// TEMPERATURE VALUES

#define CMD_UA			0x5541	// Get Active Upper (Top) Sensor
#define CMD_UB			0x5542	// Get Shutter key radial position 
#define CMD_UC			0x5543	// Get Shutter key closed position 
#define CMD_UM			0x554d	// Unsolicited Messages Mode - not used - always off
#define CMD_VC			0x5643	// VALVE CONTROL
#define CMD_VS			0x5653	// Read CURRENT SLIT STEPS
#define CMD_VT			0x5654	// Set View Type (Normal or Dual)
#define CMD_VV			0x5656	// Read CURRENT VALVE STATUS
#define CMD_VX			0x5658	// Read CURRENT VIEW X STEPS
#define CMD_VY			0x5659	// Read CURRENT VIEW Y STEPS

#define CMD_WB			0x5742	// Set Byte in BRAM
#define CMD_WE			0x5745	// Watchdog Enable - for testing only
#define CMD_XA			0x5841	// DIAGNOSTIC ALL
#define CMD_XB			0x5842  // Set Shutter key radial position
#define CMD_XC			0x5843  // Set Shutter key closed position
#define CMD_XD			0x5844  // Set Toroid key radial position 
#define CMD_XU			0x5855	// DIAGNOSTIC UV
#define CMD_XV			0x5856	// DIAGNOSTIC VIS
#define CMD_XX			0x58	// ILLEGAL COMMAND

// ERROR CODES

#define NO_ERROR0	  		0x0000
#define NO_ERRORS  			0x3030	// (00)
#define ERR_CMD				0x3031	// (01) illegal command
#define ERR_FORMAT			0x3032	// (02) illegal number/type of parameter
#define ERR_END				0x3033	// (03) not last command
#define ERR_DATA			0x3034	// (04) no data available
#define ERR_BLK				0x3035	// (05) wrong block number
#define ERR_ARRAY			0x3036	// (06) wrong sub-array number
#define ERR_PARA			0x3037	// (07) illegal parameter

#define ERR_SLIT			0x3039	// (09) Slit Motor Error
#define ERR_VIEWX_AXIAL		0x3130	// (10) Axial View X Motor Error
#define ERR_VIEWY_AXIAL		0x3131	// (11) Axial View Y Motor Error

#define ERR_FITNESS			0x3138	// (18) BRAM data fitness error
#define CMD_CANCEL			0x3230	// (20) Command Cancelled

#define ERR_OVERFLOW		0x3233	// (23) Transient Signal read overflow	(not used in software)
#define ERR_PIXEL_READ		0x3234	// (24) Detector Pixel Read error		(not used in software)

#define ERR_VIEWX_RADIAL	0x3235	// (25) Radial View X Motor Error
#define ERR_VIEWY_RADIAL	0x3236	// (26) Radial View Y Motor Error

// NEW READ ERRORS

#define ERR_READ_UV			0x3330	// (30)	Read Error
#define ERR_READ_UV_1		0x3331	// (31)	Read Error + Status Error
#define ERR_READ_UV_2		0x3332	// (32)	Read Error + Status Error Int
#define ERR_READ_UV_3		0x3333	// (33)	Read Error + Status Error Int + Status Error
#define ERR_READ_UV_4		0x3334	// (34)	Read Error + Address Overflow
#define ERR_READ_UV_5		0x3335	// (35)	Read Error + Address Overflow + Status Error
#define ERR_READ_UV_6		0x3336	// (36)	Read Error + Address Overflow + Status Error Int
#define ERR_READ_UV_7		0x3337	// (37)	Read Error + Address Overflow + Status Error Int + Status Error
#define ERR_READ_UV_8		0x3338	// (38)	Read Error + Data Overflow
#define ERR_READ_UV_9		0x3339	// (39)	Read Error + Data Overflow + Status Error
#define ERR_READ_UV_A		0x3341	// (3A)	Read Error + Data Overflow + Status Error Int
#define ERR_READ_UV_B		0x3342	// (3B)	Read Error + Data Overflow + Status Error Int + Status Error
#define ERR_READ_UV_C		0x3343	// (3C)	Read Error + Data Overflow + Address Overflow
#define ERR_READ_UV_D		0x3344	// (3D)	Read Error + Data Overflow + Address Overflow + Status Error
#define ERR_READ_UV_E		0x3345	// (3E)	Read Error + Data Overflow + Address Overflow + Status Error Int
#define ERR_READ_UV_F		0x3346	// (3F)	Read Error + Data Overflow + Address Overflow + Status Error Int + Status Error

#define ERR_PIXEL_READ_UV	0x3430	// (40)	Pixel Read Error
#define ERR_PIXEL_READ_UV_1	0x3431	// (41)	Pixel Read Error + Status Error
#define ERR_PIXEL_READ_UV_2	0x3432	// (42)	Pixel Read Error + Status Error Int
#define ERR_PIXEL_READ_UV_3	0x3433	// (43)	Pixel Read Error + Status Error Int + Status Error
#define ERR_PIXEL_READ_UV_4	0x3434	// (44)	Pixel Read Error + Address Overflow
#define ERR_PIXEL_READ_UV_5	0x3435	// (45)	Pixel Read Error + Address Overflow + Status Error
#define ERR_PIXEL_READ_UV_6	0x3436	// (46)	Pixel Read Error + Address Overflow + Status Error Int
#define ERR_PIXEL_READ_UV_7	0x3437	// (47)	Pixel Read Error + Address Overflow + Status Error Int + Status Error
#define ERR_PIXEL_READ_UV_8	0x3438	// (48)	Pixel Read Error + Data Overflow
#define ERR_PIXEL_READ_UV_9	0x3439	// (49)	Pixel Read Error + Data Overflow + Status Error
#define ERR_PIXEL_READ_UV_A	0x3441	// (4A)	Pixel Read Error + Data Overflow + Status Error Int
#define ERR_PIXEL_READ_UV_B	0x3442	// (4B)	Pixel Read Error + Data Overflow + Status Error Int + Status Error
#define ERR_PIXEL_READ_UV_C	0x3443	// (4C)	Pixel Read Error + Data Overflow + Address Overflow
#define ERR_PIXEL_READ_UV_D	0x3444	// (4D)	Pixel Read Error + Data Overflow + Address Overflow + Status Error
#define ERR_PIXEL_READ_UV_E	0x3445	// (4E)	Pixel Read Error + Data Overflow + Address Overflow + Status Error Int
#define ERR_PIXEL_READ_UV_F	0x3446	// (4F)	Pixel Read Error + Data Overflow + Address Overflow + Status Error Int + Status Error

#define ERR_READ_VIS		0x3530	// (50)	Read Error
#define ERR_READ_VIS_1		0x3531	// (51)	Read Error + Status Error
#define ERR_READ_VIS_2		0x3532	// (52)	Read Error + Status Error Int
#define ERR_READ_VIS_3		0x3533	// (53)	Read Error + Status Error Int + Status Error
#define ERR_READ_VIS_4		0x3534	// (54)	Read Error + Address Overflow
#define ERR_READ_VIS_5		0x3535	// (55)	Read Error + Address Overflow + Status Error
#define ERR_READ_VIS_6		0x3536	// (56)	Read Error + Address Overflow + Status Error Int
#define ERR_READ_VIS_7		0x3537	// (57)	Read Error + Address Overflow + Status Error Int + Status Error
#define ERR_READ_VIS_8		0x3538	// (58)	Read Error + Data Overflow
#define ERR_READ_VIS_9		0x3539	// (59)	Read Error + Data Overflow + Status Error
#define ERR_READ_VIS_A		0x3541	// (5A)	Read Error + Data Overflow + Status Error Int
#define ERR_READ_VIS_B		0x3542	// (5B)	Read Error + Data Overflow + Status Error Int + Status Error
#define ERR_READ_VIS_C		0x3543	// (5C)	Read Error + Data Overflow + Address Overflow
#define ERR_READ_VIS_D		0x3544	// (5D)	Read Error + Data Overflow + Address Overflow + Status Error
#define ERR_READ_VIS_E		0x3545	// (5E)	Read Error + Data Overflow + Address Overflow + Status Error Int
#define ERR_READ_VIS_F		0x3546	// (5F)	Read Error + Data Overflow + Address Overflow + Status Error Int + Status Error

#define ERR_PIXEL_READ_VIS	 0x3630	// (60)	Pixel Read Error
#define ERR_PIXEL_READ_VIS_1 0x3631	// (61)	Pixel Read Error + Status Error
#define ERR_PIXEL_READ_VIS_2 0x3632	// (62)	Pixel Read Error + Status Error Int
#define ERR_PIXEL_READ_VIS_3 0x3633	// (63)	Pixel Read Error + Status Error Int + Status Error
#define ERR_PIXEL_READ_VIS_4 0x3634	// (64)	Pixel Read Error + Address Overflow
#define ERR_PIXEL_READ_VIS_5 0x3635	// (65)	Pixel Read Error + Address Overflow + Status Error
#define ERR_PIXEL_READ_VIS_6 0x3636	// (66)	Pixel Read Error + Address Overflow + Status Error Int
#define ERR_PIXEL_READ_VIS_7 0x3637	// (67)	Pixel Read Error + Address Overflow + Status Error Int + Status Error
#define ERR_PIXEL_READ_VIS_8 0x3638	// (68)	Pixel Read Error + Data Overflow
#define ERR_PIXEL_READ_VIS_9 0x3639	// (69)	Pixel Read Error + Data Overflow + Status Error
#define ERR_PIXEL_READ_VIS_A 0x3641	// (6A)	Pixel Read Error + Data Overflow + Status Error Int
#define ERR_PIXEL_READ_VIS_B 0x3642	// (6B)	Pixel Read Error + Data Overflow + Status Error Int + Status Error
#define ERR_PIXEL_READ_VIS_C 0x3643	// (6C)	Pixel Read Error + Data Overflow + Address Overflow
#define ERR_PIXEL_READ_VIS_D 0x3644	// (6D)	Pixel Read Error + Data Overflow + Address Overflow + Status Error
#define ERR_PIXEL_READ_VIS_E 0x3645	// (6E)	Pixel Read Error + Data Overflow + Address Overflow + Status Error Int
#define ERR_PIXEL_READ_VIS_F 0x3646	// (6F)	Pixel Read Error + Data Overflow + Address Overflow + Status Error Int + Status Error

// SPECIFIC PARAMETER ERRORS

#define ERR_PARA1			0x3731	// (71) illegal parameter #1
#define ERR_PARA2			0x3732	// (72) illegal parameter #2
#define ERR_PARA3			0x3733	// (73) illegal parameter #3
#define ERR_PARA4			0x3734	// (74) illegal parameter #4
#define ERR_PARA5			0x3735	// (75) illegal parameter #5
#define ERR_PARA6			0x3736	// (76) illegal parameter #6
#define ERR_PARA7			0x3737	// (77) illegal parameter #7
#define ERR_PARA8			0x3738	// (78) illegal parameter #8
#define ERR_PARA9			0x3739	// (79) illegal parameter #9
#define ERR_PARA10			0x3741	// (7a) illegal parameter #10
#define ERR_PARA11			0x3742	// (7b) illegal parameter #11
#define ERR_PARA12			0x3743	// (7c) illegal parameter #12
#define ERR_PARA13			0x3744	// (7d) illegal parameter #13
#define ERR_PARA14			0x3745	// (7e) illegal parameter #14
#define ERR_PARA15			0x3746	// (7f) illegal parameter #15

#define ERR_OK_READ_UV_1	0x3831	// (81)	Status Error
#define ERR_OK_READ_UV_2	0x3832	// (82)	Status Error Int
#define ERR_OK_READ_UV_3	0x3833	// (83)	Status Error Int + Status Error
#define ERR_OK_READ_UV_4	0x3834	// (84)	Address Overflow
#define ERR_OK_READ_UV_5	0x3835	// (85)	Address Overflow + Status Error
#define ERR_OK_READ_UV_6	0x3836	// (86)	Address Overflow + Status Error Int
#define ERR_OK_READ_UV_7	0x3837	// (87)	Address Overflow + Status Error Int + Status Error
#define ERR_OK_READ_UV_8	0x3838	// (88)	Data Overflow
#define ERR_OK_READ_UV_9	0x3839	// (89)	Data Overflow + Status Error
#define ERR_OK_READ_UV_A	0x3841	// (8A)	Data Overflow + Status Error Int
#define ERR_OK_READ_UV_B	0x3842	// (8B)	Data Overflow + Status Error Int + Status Error
#define ERR_OK_READ_UV_C	0x3843	// (8C)	Data Overflow + Address Overflow
#define ERR_OK_READ_UV_D	0x3844	// (8D)	Data Overflow + Address Overflow + Status Error
#define ERR_OK_READ_UV_E	0x3845	// (8E)	Data Overflow + Address Overflow + Status Error Int
#define ERR_OK_READ_UV_F	0x3846	// (8F)	Data Overflow + Address Overflow + Status Error Int + Status Error

#define ERR_OK_READ_VIS_1	0x3931	// (91)	Status Error
#define ERR_OK_READ_VIS_2	0x3932	// (92)	Status Error Int
#define ERR_OK_READ_VIS_3	0x3933	// (93)	Status Error Int + Status Error
#define ERR_OK_READ_VIS_4	0x3934	// (94)	Address Overflow
#define ERR_OK_READ_VIS_5	0x3935	// (95)	Address Overflow + Status Error
#define ERR_OK_READ_VIS_6	0x3936	// (96)	Address Overflow + Status Error Int
#define ERR_OK_READ_VIS_7	0x3937	// (97)	Address Overflow + Status Error Int + Status Error
#define ERR_OK_READ_VIS_8	0x3938	// (98)	Data Overflow
#define ERR_OK_READ_VIS_9	0x3939	// (99)	Data Overflow + Status Error
#define ERR_OK_READ_VIS_A	0x3941	// (9A)	Data Overflow + Status Error Int
#define ERR_OK_READ_VIS_B	0x3942	// (9B)	Data Overflow + Status Error Int + Status Error
#define ERR_OK_READ_VIS_C	0x3943	// (9C)	Data Overflow + Address Overflow
#define ERR_OK_READ_VIS_D	0x3944	// (9D)	Data Overflow + Address Overflow + Status Error
#define ERR_OK_READ_VIS_E	0x3945	// (9E)	Data Overflow + Address Overflow + Status Error Int
#define ERR_OK_READ_VIS_F	0x3946	// (9F)	Data Overflow + Address Overflow + Status Error Int + Status Error

// UNSOLICITED ERROR CODES & CHECK VALUES

#define FATAL_ERROR			0x21	// FATAL ERROR TO REPLACE COMMAND (!!)
#define WARNING_MSG			0x40	// WARNING TO REPLACE COMMAND (@@)
#define OK_MESSAGE			0x23	// OK MESSAGE TO REPLACE COMMAND (##)

#define ERR_ABORT_CMD		0x4143	// (AC) ABORT COMMAND
#define ERR_ARGON_FLOW		0x4146	// (AF) Argon Flow error
#define MSG_ARGON_FLOW		0x414f	// (AO) Argon Flow OK message

// HANDLING ERROR CODES FOR MOTORS 

#define ERROR_TOROID_MOVE_TO_HOME				0x4230	// (B0) 		 
#define ERROR_TOROID_MOVE_OFF_HOME				0x4231	// (B1) 

#define ERROR_SHUTTER_MOTOR_MOVE_OFF_SENSOR		0x4232	// (B2) 
#define ERROR_SHUTTER_MOTOR_MOVE_TO_SENSOR		0x4233	// (B3)    
#define ERROR_SHUTTER_MOTOR_MOVE_TO_HOME		0x4234	// (B4)       
#define ERROR_SHUTTER_MOTOR_MOVE_TO_ABS_POS		0x4235	// (B5)
 
#define ERROR_BUFFER_FULL			0x4246	// (BF) ERROR BUFFER FULL
#define ERR_CASTING_TEMP_FAILED		0x4346	// (CF) Casting Warmup Failure  

#define HIGH_UV_DARK_CURRENT		0x4431	// (D1) HIGH UV Dark Current Warning
#define HIGH_VIS_DARK_CURRENT		0x4432	// (D2) HIGH VIS Dark Current Warning
#define HIGH_DARK_CURRENT			0x4433	// (D3) HIGH UV & VIS Dark Current Warning

#define CCD_POWER_OFF				0x444f	// (DO) Detector Power OFF
#define CCD_POWER_ON				0x4450	// (DP) Detector Power ON

#define ERR_FATAL_MODE				0x464d	// (FM) Command received while in FATAL MODE

#define WARN_FAN_TEMP_LOW			0x464c	// (FL) Fan Temperature HIGH
#define WARN_FAN_TEMP_HIGH			0x4648	// (FH) Fan Temperature HIGH
#define MSG_FAN_TEMP_OK				0x4630	// (F0) Fan Temperature OK


#define NO_UV_MERCURY_LAMP			0x4c31	// (L1) No UV Mercury Lamp Warning
#define NO_VIS_MERCURY_LAMP			0x4c32	// (L2) No VIS Mercury Lamp Warning
#define NO_MERCURY_LAMP				0x4c33	// (L3) No UV & VIS Mercury Lamp Warning
#define ERR_LOOKER					0x4c45	// (LE) Looker Error

#define WARN_NITROGEN_FLOW			0x4e46	// (NF) Nitrogen Flow error
#define MSG_NITROGEN_FLOW			0x4e4f	// (NO) Nitrogen Flow OK message
	
#define ERR_24VDC_MIN				0x5031	// (P1) 24 Volt Power Supply error
#define WARN_24VDC_LOW				0x5032	// (P2) 24 Volt Power Supply warning
#define WARN_24VDC_HIGH				0x5033	// (P3) 24 Volt Power Supply warning
#define ERR_24VDC_MAX				0x5034	// (P4) 24 Volt Power Supply error
#define MSG_24VDC_OK				0x5035	// (P5) 24 Volt Power Supply OK message

#define ERR_SLIT_LOOKERS  			0x5330	// (S0) Slit Motor Looker Error
#define ERR_SLIT_HOME	  			0x5331	// (S1) Slit Motor Home Error
#define ERR_SLIT_LIMIT 				0x5332	// (S2) Slit Motor Limit Error
#define ERR_SLIT_STEPS	 			0x5333	// (S3) Slit Motor Total Steps Error
#define UV_SHUTTER_FAILURE			0x5334	// (S4) UV Shutter Failure Warning
#define VIS_SHUTTER_FAILURE			0x5335	// (S5) VIS Shutter Failure Warning
#define SHUTTER_FAILURE				0x5336	// (S6) UV & VIS Shutter Failure Warning

#define WARN_TIME_ON_HOLD				0x5448	// (TH) Countdown Time on HOLD
#define	TOP_PRIMARY_SENSOR_FAILED		0x5430	// (T0) Warning
#define	BOTTOM_PRIMARY_SENSOR_FAILED 	0x5431	// (T1)	Warning
#define	TOP_SECONDARY_SENSOR_FAILED		0x5432	// (T2)	Warning
#define	BOTTOM_SECONDARY_SENSOR_FAILED	0x5433	// (T3)	Warning

#define MSG_UV_COOLER_OK			0x5530	// (U0) UV Detector Cooler Voltage OK message
#define ERR_UV_COOLER_MIN			0x5531	// (U1) UV Detector Cooler Voltage error
#define WARN_UV_COOLER_LOW			0x5532	// (U2) UV Detector Cooler Voltage warning	
#define ERR_UV_COOLER_MAX			0x5533	// (U3) UV Detector Cooler Voltage error
#define ERR_UV_COOLER_READOUT		0x5534	// (U4) UV Detector Temperature Readout error
#define ERR_UV_CCD_MAX				0x5535	// (U5) UV Detector Maximum Temperature error
#define WARN_UV_CCD_HIGH			0x5536	// (U6) UV Detector Temperature warning
#define MSG_UV_CCD_OK				0x5537	// (U7) UV Detector Temperature OK message
#define ERR_UV_CCD_MIN				0x5538	// (U8) UV Detector Minimum Temperature error
#define ERR_UV_CCD_HEATING			0x5539	// (U9) UV Detector Heating error

#define MSG_VIS_COOLER_OK			0x5630	// (V0) VIS Detector Cooler Voltage OK message
#define ERR_VIS_COOLER_MIN			0x5631	// (V1) VIS Detector Cooler Voltage error
#define WARN_VIS_COOLER_LOW			0x5632	// (V2) VIS Detector Cooler Voltage warning
#define ERR_VIS_COOLER_MAX			0x5633	// (V3) VIS Detector Cooler Voltage error
#define ERR_VIS_COOLER_READOUT		0x5634	// (V4) VIS Detector Temperature Readout error

#define ERR_VIS_CCD_MAX				0x5635	// (V5) VIS Detector Temperature error
#define WARN_VIS_CCD_HIGH			0x5636	// (V6) VIS Detector Temperature warning
#define MSG_VIS_CCD_OK				0x5637	// (V7) VIS Detector Temperature OK message
#define ERR_VIS_CCD_MIN				0x5638	// (V8) VIS Detector Minimum Temperature error
#define ERR_VIS_CCD_HEATING			0x5639	// (V9) VIS Detector Heating error

#define ERR_CAMERA_OFF_UV			0x5558	// (UX) Detected camera self shutdown
#define ERR_CAMERA_OFF_VIS			0x5658	// (VX) Detected camera self shutdown

#define ERR_WATER_LEAK				0x574c	// (WL) Water Leakage error
#define MSG_WATER_LEAK				0x5730	// (W0) Water Leakage OK message

#define ERR_VIEWX_LOOKERS_AXIAL		0x5830	// (X0) View X Motor Error
#define ERR_VIEWX_HOME_AXIAL 		0x5831	// (X1) View X Motor Error
#define ERR_VIEWX_LIMIT_AXIAL		0x5832	// (X2) View X Motor Error
#define ERR_VIEWX_STEPS_AXIAL		0x5833	// (X3) View X Motor Error
#define ERR_VIEWY_LOOKERS_AXIAL 	0x5930	// (Y0) View Y Motor Error
#define ERR_VIEWY_HOME_AXIAL	 	0x5931	// (Y1) View Y Motor Error
#define ERR_VIEWY_LIMIT_AXIAL		0x5932	// (Y2) View Y Motor Error
#define ERR_VIEWY_STEPS_AXIAL		0x5933	// (Y3) View Y Motor Error

#define ERR_VIEWX_LOOKERS_RADIAL	0x6830	// (h0) View X Motor Error
#define ERR_VIEWX_HOME_RADIAL 		0x6831	// (h1) View X Motor Error
#define ERR_VIEWX_LIMIT_RADIAL		0x6832	// (h2) View X Motor Error
#define ERR_VIEWX_STEPS_RADIAL		0x6833	// (h3) View X Motor Error
#define ERR_VIEWY_LOOKERS_RADIAL 	0x6930	// (i0) View Y Motor Error
#define ERR_VIEWY_HOME_RADIAL	 	0x6931	// (i1) View Y Motor Error
#define ERR_VIEWY_LIMIT_RADIAL		0x6932	// (i2) View Y Motor Error
#define ERR_VIEWY_STEPS_RADIAL		0x6933	// (i3) View Y Motor Error


#define UV_COOLER_FLG		0x0001	// WARNING/FATAL PENDING INDICATOR
#define VIS_COOLER_FLG		0x0002	// WARNING/FATAL PENDING INDICATOR
#define UV_CCD_FLG			0x0004	// WARNING/FATAL PENDING INDICATOR
#define VIS_CCD_FLG			0x0008	// WARNING/FATAL PENDING INDICATOR
#define FAN_FLG				0x0010	// WARNING PENDING INDICATOR
#define PLENUM_FLG			0x0020	// WARNING PENDING INDICATOR
#define CASTING_FLG			0x0040	// WARNING PENDING INDICATOR
#define PS5VDC_FLG			0x0080	// WARNING/FATAL PENDING INDICATOR
#define PS24VDC_FLG			0x0100	// WARNING/FATAL PENDING INDICATOR
#define ARGON_FLG			0x0200	// FATAL PENDING INDICATOR
#define NITROGEN_FLG		0x0400	// WARNING PENDING INDICATOR
#define WATER_LEAK_FLG		0x0800	// FATAL PENDING INDICATOR
#define INT_60HZ_FLG		0x1000	// FATAL PENDING INDICATOR


// VOLTAGES ARE STORED IN RAW COUNTS			// Voltage = Counts * 0.1846 (24.0 = 130 counts)
#define PS24VDC_MIN			(unsigned char) 104	// Minimum level of 24 Volt Power Supply (20% 19.2)
#define PS24VDC_LOW			(unsigned char) 117	// Low level of 24 Volt Power Supply (10% - 21.6)
#define PS24VDC_LOW_OK		(unsigned char) 120	// ~22.2 volts
#define PS24VDC_HIGH_OK		(unsigned char) 134	// ~24.8 volts
#define PS24VDC_HIGH		(unsigned char) 136	// High level of 24 Volt Power Supply (5% - 25.2)
#define PS24VDC_MAX			(unsigned char) 143	// Maximum level of 24 Volt Power Supply (10% - 26.4)

												// Voltage = Counts * 0.039412									
#define UV_COOLER_MIN		(unsigned char) 127	// Minimum UV COOLER voltage (~5.0 Volts)
#define UV_COOLER_LOW		(unsigned char) 140	// Low Level UV COOLER voltage (~5.5 Volts)
#define UV_COOLER_LOW_OK	(unsigned char) 159	// (~6.1 volts) As per QPR: THNJ6R6K2Q
#define UV_COOLER_HIGH_OK	(unsigned char) 178	// (~7.0 volts)
#define UV_COOLER_MAX		(unsigned char) 203	// Maximum UV COOLER voltage (~8.0 Volts)*/

												// Voltage = Counts * 0.039412
#define VIS_COOLER_MIN		(unsigned char) 127	// Minimum VIS COOLER voltage (~5.0 Volts)*/
#define VIS_COOLER_LOW		(unsigned char) 140	// Low Level VIS COOLER voltage (~5.5 Volts)*/
#define VIS_COOLER_LOW_OK	(unsigned char) 159	// (~6.1 volts) As per QPR: THNJ6R6K2Q
#define VIS_COOLER_OK		(unsigned char) 165	// (~6.5 volts) - Used in Single Channel ONLY
#define VIS_COOLER_HIGH_OK	(unsigned char) 178	// (~7.0 volts)
#define VIS_COOLER_MAX		(unsigned char) 203	// Maximum VIS COOLER voltage (~8.0 Volts)*/

												// Temperature = Counts * 0.511 - 64.8
												// Temperature Range:  +65.5 to -64.8
#define UV_CCD_TEMP_HEATING	(unsigned char)	205	// UV DETECTOR not cooling (+40 C)
#define UV_CCD_TEMP_START	(unsigned char) 117	// Maximum Starting UV DETECTOR temperature (- 5 C)
#define UV_CCD_TEMP_MAX		(unsigned char) 107	// Maximum UV DETECTOR temperature (-10 C)

#define UV_CCD_TEMP_HIGH	(unsigned char) 107	// High Level UV DETECTOR temperature (-10 C)
#define UV_CCD_TEMP_HIGH_OK	(unsigned char)  72	// -28 C
#define UV_CCD_TEMP_MIN		(unsigned char)  19	// Minimum UV DETECTOR temperature (-55 C)


#define VIS_CCD_TEMP_HEATING (unsigned char) 205 // VIS DETECTOR not cooling (+40 C)
#define VIS_CCD_TEMP_START	(unsigned char) 117	// Maximum Starting VIS DETECTOR temperature (- 5 C)
#define VIS_CCD_TEMP_MAX	(unsigned char) 107	// Maximum VIS DETECTOR temperature (-10 C)

#define VIS_CCD_TEMP_HIGH	(unsigned char) 107	// High Level VIS DETECTOR temperature (-10 C)
#define VIS_CCD_TEMP_HIGH_OK (unsigned char) 72	// -28 C
#define VIS_CCD_TEMP_OK		(unsigned char)  48	// counts = -40 C - Used in Single Channel ONLY
#define VIS_CCD_TEMP_MIN	(unsigned char)  19	// Minimum VIS DETECTOR temperature (-55 C)

												// Temperature = 0.0007684 * x^2 - 0.50445 * x + 77.136
												// Temperature Range:  -1.533 to 77.136

#define FAN_TEMP_LOW		(unsigned char) 195	// Minimum Fan temperature (7.99 C)
#define FAN_TEMP_LOW_OK		(unsigned char) 185	// 10.11 C
#define FAN_TEMP_HIGH_OK	(unsigned char)  52	// 52.98 C
#define FAN_TEMP_HIGH		(unsigned char)  47	// Maximum FAN temperature (55.12 C)

#define MAX_OFF_TIME		1800	// 30 minutes (value in seconds)
#define MIN_PURGE_TIME		600		// 10 minutes (value in seconds)
#define MAX_PURGE_TIME		3600	// 1 hours (value in seconds)
#define MIN_COOLER_TIME		120		// 2 Minutes (value in seconds)
#define MAX_COOLER_TIME		600		// 10 Minutes (value in seconds)
#define MAX_NOT_ON_TIME		600		// 10 minutes to automatic COLD start

#define MAX_WARM_UP			43200	// 12 hours (value in seconds)

// ARRAY SIZES

#define MAX_IN		  	1024
#define MAX_BYTES		1024
#define MAX_ERROR		1024
#define MAX_IEEE	  	8192
#define MAX_OUT	 		16000 // was 33000
#define MAX_ARRAY	 	256
#define MAX_ZOD_UV 		173
#define MAX_ZOD_VIS	 	53
#define MAX_ZOD_SPARES	30
#define MAX_VEGA_UV		192
#define MAX_VEGA_VIS 	56
#define MAX_VEGA_SPARES	24
#define MAX_BLOCKS		65535
#define VISUVCOEF		9

// RECORDER VALUES

#define MAX_GAIN		10000
#define RAW				0x52	// 'R' - RAW MODE
#define SMOOTH			0x53	// 'S' - SMOOTH MODE
#define SUM				0x54	// 'T' - TOTAL SUM MODE
#define AVE				0x41	// 'A' - AVE MODE
#define PEAK			0x50	// 'P' - PEAK MODE
#define MAX_OUTPUT		4095	// MAXIMUM D/A OUTPUT (10 Volts)

// MOTOR VALUES

#define SLIT			1
#define VIEWX_AXIAL		2
#define VIEWY_AXIAL		3
#define VIEWX_RADIAL	4	
#define VIEWY_RADIAL	5

// MOTOR LIMITS

#define MAX_SLIT_LOW  	7
#define MAX_SLIT_HIGH  	9

#define MAX_VIEW_X_AXIAL  		50
#define MAX_VIEW_Y_AXIAL  		60
#define XPEAK_AXIAL				25
#define Y15MM_AXIAL				30

#define MAX_VIEW_X_RADIAL 	 	50
#define MAX_VIEW_Y_RADIAL 	 	60
#define XPEAK_RADIAL			25
#define Y15MM_RADIAL			37

 
#define TOROID_AT_AXIAL_POS		0		// Toroid is at Axial position
#define TOROID_AT_RADIAL_POS	1		// Toroid is at Radial position
#define TOROID_AT_OTHER_POS		3		// Toroid is at Other position (none of the above)

#define SHUTTER_STEPS_PAST_HOME 20		// Shutter number of steps from home sensor to park position
#define SHUTTER_MAX_STEPS		720		// Shutter max number of steps from sensor
#define SHUTTER_STEPS_PER_REV	200		// Shutter steps per revolution
#define SHUTTER_AWAY_FROM_HOME  10      // Steps to move past sensor to HOME position 1 //CBF-35 
#define SHUTTER_AT_AXIAL_POS	0		// Shutter is at Axial position
#define SHUTTER_AT_RADIAL_POS	1		// Shutter is at Radial position
#define SHUTTER_AT_CLOSED_POS	2		// Shutter is at Closed position
#define SHUTTER_AT_OTHER_POS	3		// Shutter is at Other position (none of the above)

#define SHUTTER_DEFAULT_RADIAL_POS	(SHUTTER_MAX_STEPS / 2)	// Shutter default radial position
#define SHUTTER_DEFAULT_POS_MAX		700						// Shutter default max position

// while loop time-outs
#define SECONDS									1000			// 1 second in OS ticks
#define TIMEOUT_TOROID_MOTOR_TO_HOME			(5 * SECONDS)	// 5 seconds 	
#define TIMEOUT_TOROID_MOVE_OFF_HOME			(5 * SECONDS)	// 5 seconds
#define TIMEOUT_SHUTTER_MOTOR_MOVE_TO_HOME		(5 * SECONDS)	// 5 seconds
#define TIMEOUT_SHUTTER_MOTOR_MOVE_OFF_SENSOR	(5 * SECONDS)	// 5 seconds
#define TIMEOUT_SHUTTER_MOTOR_MOVE_TO_SENSOR	(10 * SECONDS)	// 5 seconds
#define TIMEOUT_SHUTTER_MOTOR_MOVE_TO_ABS_POS	(10 * SECONDS)	// 10 seconds

#define MAX_INT_TIME 	60000
#define MAX_DELAY_TIME 	60000
#define MAX_TM_READS 	10000
#define MAX_TM_INTERVAL	60000

// MOTOR LOCATIONS

#define	HOME			0x30
#define	HIGHRES			0x31
#define	LOWRES			0x32
#define	BARNDOOR		0x33
#define LIMIT			0x34
#define INCREASE		0x30
#define DECREASE		0x31

// DATA OUTPUT SIZES

#define SIZE_ALL_HIGH  	15960
#define SIZE_ALL_LOW  	 8664
#define SIZE_ALL_HL  	 9928
#define SIZE_ALL_LH  	14696
#define SIZE_UV_HIGH 	13114
#define SIZE_UV_LOW 	 7082
#define SIZE_VIS_HIGH  	 2858
#define SIZE_VIS_LOW  	 1594
#define TEST_ALL_HIGH  	50700
#define TEST_ALL_LOW  	26124
#define TEST_UV_HIGH 	34266
#define TEST_UV_LOW 	17658
#define TEST_VIS_HIGH  	10506
#define TEST_VIS_LOW  	 5418

// STARTUP MODES

#define SETUP			0x30	// 0
#define NOT_ON			0x31	// 1
#define COLD			0x32	// 2
#define COLD1			0x33	// 3
#define COLD2			0x34	// 4
#define COLD3			0x35	// 5
#define HOT				0x41	// A
#define HOT1			0x42	// B
#define HOT2			0x43	// C
#define HOT3			0x44	// D
#define HOT4			0x45	// E
#define READY			0x52	// R
#define DIAG			0x58	// X
#define MFG				0x59	// Y
#define AUTO			0x5a	// Z
#define FATAL			0x66	// f
#define SLEEP			0x73	// s
#define GO				1
#define OK				2
#define ABORT			3

// MISCELLANEOUS DEFINITIONS

//#define NULL			0
#define YES				1
#define NO				0
#define	ON				1
#define	OFF				0
#define	TRUE			1
#define	FALSE			0
#define PASS			0
#define FAIL			1
#define PRIMARY			0
#define SECONDARY		1
#define NORMAL_TIME     0
#define ARGON_TIME		1

#define ASCII0			0x30
#define UV			  	0
#define VIS			  	1
#define SPARE		  	2
#define UVVIS			3
#define UVSC			4
#define LOW				0
#define HIGH			1
#define EVEN			0x30
#define ODD				0x31
#define INVERSE 	 	1
#define NORMAL			0
#define AXIAL_TYPE		0
#define TEST			1
#define OVERSCAN		1
#define DUALVIEW		1
#define RADIAL_TYPE		1
#define	DUALVIEW_NEW	2
#define CLKCOLON		0x3a	// ASCII COLON (:)
#define CLKDASH			0x2d	// ASCII DASH (-)
#define CLKAT			0x40	// ASCII AT SIGN (@)
#define END		(BYTE)	0xff
#define SPACE			0x20
#define CLR_TM			24		// Clear C Time (~100 usec)
#define MAX_READS		65535
#define TEMP_UV_CCD		0x31
#define TEMP_VIS_CCD	0x32
#define TEMP_FAN		0x33
#define TEMP_PLENUM		0x34
#define TEMP_CASTING	0x35
#define PLUS_SIGN		0x2b
#define MINUS_SIGN		0x2d
#define ZODIAC			0x30
#define VEGA			0x31
#define VEGA_SCA		0x32

// Optics Hours stuff
#define OPTICS_HOURS_MAX	60000
#define OPTICS_HOURS_DEFAULT	0

// Thermal Control Settings

#define LID_POWER_LEVEL 					3.0f								// Lid power - set at 3% 
#define LID_POWER_MAX						100.0f								// Maximum lid power is 100%
#define TUB_TEMPERATURE_SETPOINT			39.00f								// PI temperature setpoint - set 39 deg 
#define INTEGRATOR_WINDOW_THRESHOLD			(TUB_TEMPERATURE_SETPOINT - 1)		// Temperature at which the integrator term becomes active.
#define LID_POWER_REDUCTION_TEMPERATURE		(TUB_TEMPERATURE_SETPOINT - 1)		// Temperature at which the lid power is reduced for entry to stabilization
#define TUB_TEMP_COLD_THRESHOLD				3850								// Cold threshold temperature scaled by 100 - set 3850 
#define TUB_TEMP_HOT_THRESHOLD				3850								// Hot threshold temperature scaled by 100 - set 3850  
#define TUB_WARM_UP_RATE 					750									// Seconds per degree warmup rate - 12.5 minutes
																				// Worst case number at min line voltage and min ambient temperature
#define TEMP_STABILIZATION					38.5f								// Stabilization threshold temperature - set 38.5f											

#define MINUTES 							60000
#define STABILIZATION_TIME 					(15 * MINUTES)													
													
#define MAGIC_NUMBER						98766789L							// Magic number for calibration

#define HEATER_PWM_PERIOD					1000								// PWM period in milliseconds

#define HEATERS_RANGE_LOW					5.0f								// Low end of heaters operational range	
#define HEATERS_RANGE_HIGH					50.0f								// High end of heaters operational range
												
#endif

//===========================================================================

/*** Revision History ***/

