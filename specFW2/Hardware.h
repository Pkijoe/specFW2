//===========================================================================
//
//	Module Name:	Hardware.h
//
//	Function:		This file contains the ROM, RAM, and hardware
//					locations used by the Zodiac firmware.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================

//===========================================================================
// READ MODE REGISTERS - READ & WRITE
//===========================================================================
#define	TIME_MODE_UV		(WORD)	0x0100		// 0xa6000100 - Write only
#define	TIME_MODE_VIS		(WORD)	0x0200		// 0xa6000200 - Write only
#define	CAMERA_RESET				0x0000
#define	CAMERA_READY				0x0001
#define	HIGH_RESOLUTION_MODE		0x0002
#define TEST_MODE					0x0004
#define OVERSCAN_MODE				0x0008
#define COMMAND_START_INT_BIT		0x0010
#define COMMAND_ARRAY_READ_BIT		0x0020
#define NORMAL_READ_MODE			0x0800		// If Low - Ramp Mode
#define	INT_TIME_UV			(WORD)	0x0108		// 0xa6000108 - Write only
#define	INT_TIME_VIS		(WORD)	0x0208		// 0xa6000208 - Write only
#define	DELAY_TIME_UV		(WORD)	0x0110		// 0xa6000110 - Write only
#define	DELAY_TIME_VIS		(WORD)	0x0210		// 0xa6000210 - Write only
#define	POWER_SHUTDOWN_UV	(WORD)	0x0118		// 0xa6000118 - WORD m_nPowerRegisterUV
#define	POWER_SHUTDOWN_VIS	(WORD)	0x0218		// 0xa6000218 - WORD m_nPowerRegisterVIS
#define	CLEAR_POWER					0x0000
#define	POWER_ON					0x0003
#define	ADC_RESET					0x0004
#define LOW_SPEED					0x0800
#define	LED_LAMP_1					0x1000
#define	LED_LAMP_2					0x2000
#define	LED_LAMP_3					0x4000
#define	LED_LAMP_4					0x8000
#define	LED_LAMPS_ON				0xf000
#define	SYSTEM_ERROR_UV		(WORD)	0x0120		// 0xa6000120 - Read
#define	SYSTEM_ERROR_VIS	(WORD)	0x0220		// 0xa6000220 - Read
#define	ADDRESS_OVERFLOW			0x0001
#define	DATA_OVERFLOW				0x0002
#define	DIAG_ADC_ENABLE_UV	(WORD)	0x0130		// 0xa6000130 - Write
#define	DIAG_ADC_ENABLE_VIS	(WORD)	0x0230		// 0xa6000230 - Write
#define	ADC_ENABLED					0x0001
#define WATCHDOG_INIT_UV	(WORD)	0x0172		// 0xa6000172 - Write
#define WATCHDOG_INIT_VIS	(WORD)	0x0272		// 0xa6000272 - Write
#define WATCHDOG_INIT				0x0004		// QPR #TLFI6H3JA2
#define WATCHDOG_UV			(WORD)	0x0174		// 0xa6000174 - Write
#define WATCHDOG_VIS		(WORD)	0x0274		// 0xa6000274 - Write

#define	ADC_RECEIVE_REG_UV	(WORD)	0x0140		// 0xa6000140 - Read
#define	ADC_RECEIVE_REG_VIS	(WORD)	0x0240		// 0xa6000240 - Read
#define	ADC_TRANSMIT_REG_UV	(WORD)	0x0142		// 0xa6000142 - Write
#define	ADC_TRANSMIT_REG_VIS (WORD)	0x0242		// 0xa6000242 - Write
#define	ADC_STATUS_REG_UV	(WORD)	0x0144		// 0xa6000144 - Read
#define	ADC_STATUS_REG_VIS	(WORD)	0x0244		// 0xa6000244 - Read
#define	ADC_CONTROL_REG_UV	(WORD)	0x0146		// 0xa6000146 - Write
#define	ADC_CONTROL_REG_VIS	(WORD)	0x0246		// 0xa6000246 - Write

#define	ARRAY_ADDRESS_UV	(WORD)	0x0150		// 0xa6000150 - Write
#define	ARRAY_ADDRESS_VIS	(WORD)	0x0250		// 0xa6000250 - Write
#define	ARRAY_DATA_UV		(WORD)	0x0160		// 0xa6000160 - Read
#define	ARRAY_DATA_VIS		(WORD)	0x0260		// 0xa6000260 - Read
#define	ARRAY_DATA_LATCH_UV	(WORD)	0x0520		// 0xa6000520 - Read
#define	ARRAY_DATA_LATCH_VIS (WORD)	0x0528		// 0xa6000528 - Read
#define	CAMERA_RESET_REG	(WORD)	0x0522		// 0xa6000522 - Write
#define	RESET_UV					0x8000
#define	RESET_VIS					0x0080
#define RESET_MUX					0x0001
#define	CAMERAS_READY				0x0000
#define	FPGA_VERSION_NO_UV	(WORD)	0x0128		// 0xa6000128 - Read
#define	FPGA_VERSION_NO_VIS	(WORD)	0x0228		// 0xa6000228 - Read

//===========================================================================
// FPGA VERSION REGISTER - READ ONLY
//===========================================================================
#define	FPGA_VERSION_NO		(WORD)	0x0500		// 0xa6000500 - Read
#define BOARD_VERSION		(WORD)	0xF000		// Bits 12..15 indicate the Board Version

// Bit 0 to Bit 11: FPGA Version in Binary
// Bit12 to Bit 15: Spectrometer Control Board Version


//===========================================================================
// MOTOR CONTROL REGISTERS - WRITE ONLY
//===========================================================================
#define AXIAL_MOTOR_CMD_REG	(WORD)	0x0502		// 0xa6000502 - WORD m_nMotorControl
#define SLIT_MOTOR_CMD_REG	(WORD)	0x0502		// 0xa6000502 - WORD m_nMotorControl
#define RADIAL_MOTOR_CMD_REG (WORD)	0x050E		// 0xa600050E - WORD m_nMotorControl

// Note:  The X and Y bit position definitions are the same for Axial and Radial

#define	VIEWX_DIR					0x0001		// Set View X Direction (HI = home)
#define	VIEWX_STEP					0x0002		// Move View Y one half-step (rising edge)
#define	VIEWY_DIR					0x0004		// Set View Y direction
#define	VIEWY_STEP					0x0008		// Move View Y one half-step (rising edge)
#define	SLIT_DIR					0x0010		// Set SLIT direction
#define	SLIT_STEP					0x0020		// Move SLIT one full-step (rising edge)
#define	VIEWX_ENABLE				0x0040		// Enable View X Motor
#define	VIEWY_ENABLE				0x0080		// Enable View Y Motor
#define	SLIT_ENABLE					0x0100		// Enable Slit Motor

//===========================================================================
// Axial and Radial X & Y MOTOR CONTROLS
//===========================================================================
#define	XOUTON						0x0002		// Pulse high for CW step
#define	XOUTOFF						0x0000		// Pulse low for CW step
#define	XINON						0x0003		// Pulse high for CCW step
#define XINOFF						0x0001		// Pulse low for CCW step
#define YOUTON						0x0008		// Pulse high for CW step
#define YOUTOFF						0x0000		// Pulse low for CW step
#define YINON						0x000c		// Pulse high for CCW step
#define	YINOFF						0x0004		// Pulse low for CCW step

//===========================================================================
// SLIT MOTOR CONTROLS
//===========================================================================
#define	CWON						0x20		// Pulse high for CW step
#define	CWOFF						0x00		// Pulse low for CW step
#define	CCWON						0x30		// Pulse high for CCW step
#define	CCWOFF						0x10		// Pulse low for CCW step
#define	RAMP_SLIT					  75
#define	HSTP_SLIT					6000		// Keep the same for Axial and Radial for now
#define	RAMP_VIEWX					  50		// Keep the same for Axial and Radial for now
#define	HSTP_VIEWX					1000		// Keep the same for Axial and Radial for now
#define	RAMP_VIEWY					  50		// Keep the same for Axial and Radial for now
#define HSTP_VIEWY					1000		// Keep the same for Axial and Radial for now
#define V_BACKLASH					0x02
#define S_BACKLASH					  50		// Change from 25 to 50 for Century B
#define CLEARVALVE					0x0000		// Turn all valves off

//===========================================================================
// DETECTOR STATUS REGISTER - READ ONLY
//===========================================================================
#define	STATUS_REGISTER		(WORD)	0x0504		// 0xa6000504
#define	STATUS_DATA_READY_VIS		0x0001
#define	STATUS_ERROR_VIS			0x0002
#define	STATUS_ERROR_INT_VIS		0x0004
#define	STATUS_DATA_READY_UV		0x0010
#define	STATUS_ERROR_UV				0x0020
#define	STATUS_ERROR_INT_UV			0x0040

//===========================================================================
// ADC COMMAND REGISTER	- READ ONLY
//===========================================================================
#define	ADC_COMMAND_REG		(WORD)	0x0508		// 0xa6000508
#define	ADC_DATA_REG		(WORD)	0x050a		// 0xa600050a
#define PS24VDC						0x0001
#define	AMBIENT_INSIDE				0x0002
#define	THERMISTOR_COARSE			0x0004
#define	THERMISTOR_FINE				0x0008
#define	AMBIENT_OUTSIDE				0x0010
#define	TEC_POWER_UV				0x0020
#define	TEC_POWER_VIS				0x0040
#define	GROUND						0x0080

//===========================================================================
// DISCRETE DIGITAL COMMAND REGISTER - WRITE ONLY
//===========================================================================
#define DIGITAL_CMD_REG		(WORD)	0x050c		// 0xa600050c - WORD m_nDigitalControl
#define TEC_EN_UV					0x0001		// Enable UV thermo-electro cooler
#define TEC_EN_VIS					0x0002		// Enable VIS thermo-electro cooler
#define HTR_TUB_EN					0x0004		// Enable heat to tub heater
#define	HTR_LID_EN					0x0008		// Enable heat to lid heater
#define	DETECTOR_HEATER				0x0010
#define	EXT2_ENABLE					0x0020		// External (Output) Trigger #2
#define	CHILLER_ENABLED				0x0040		// Same as External (Output)  Trigger #1
#define	EXT1_ENABLE					0x0040		// External (Output)  Trigger #1
#define	HGLAMP_EN					0x0080		// Enable Hg Lamp

//===========================================================================
// HEATERS PWM ENABLE REGISTER - R/W
//===========================================================================
#define	HEATERS_ENABLE_REG		(WORD)	0x0530		// 0x0A6000530
#define PWM_DISABLE						0x0000		// Disable heaters PWM
#define TUB_PWM_ENABLE					0x0001		// Enable tub heater PWM
#define LID_PWM_ENABLE					0x0002		// Enable lid heater PWM

//===========================================================================
// TUB HEATER PWM PERIOD REGISTER - R/W
//===========================================================================
#define	TUB_HEATER_PWM_PERIOD_REG		(WORD)	0x0532		// A6000532
															// The period is expressed in milliseconds
//===========================================================================
// LID HEATER PWM PERIOD REGISTER - R/W
//===========================================================================
#define	LID_HEATER_PWM_PERIOD_REG		(WORD)	0x0536		// A6000536
															// The period is expressed in milliseconds
//===========================================================================
// TUB HEATER PWM DUTY REGISTER - R/W
//===========================================================================
#define	TUB_HEATER_PWM_DUTY_REG			(WORD)	0x0534		// A6000534
															// The duty cycle (on-time) is expressed in milliseconds
//===========================================================================
// LID HEATER PWM DUTY REGISTER - R/W
//===========================================================================
#define	LID_HEATER_PWM_DUTY_REG			(WORD)	0x0538		// A6000538
															// The duty cycle (on-time) is expressed in milliseconds
															
//===========================================================================
// Shutter and Radial Toroid Motor Control- read/write
//===========================================================================
#define SHUTTER_RT_MOTOR_CMD_REG (WORD)		0x0510	    // 0xA6000510 - Shutter and Radial Toroid Motor Control 
#define	TOROID_MOTOR_ENABLE					0x0001		// Enable Radial Toroid Motor (1 = enable)
#define	TOROID_DIR_HOME						0x0002		// Set Radial Toroid Motor Direction (1 = home)
#define	TOROID_FLAG_OVERRIDE				0x0004		// Set Radial Toroid Motor Overide flag which allows stepping while
														// the home switch is active (1 = active)
#define	TOROID_CMD_CLEAR					0x00F0		// Mask to clear the register										
#define	SHUTTER_MOTOR_RUN					0x0010		// Start the Shutter Motor running(1 = run; 0 = reset for next move)
#define	SHUTTER_DIR_HOME                    0x0020      // Set Shutter Motor Direction (1 = home)
#define	SHUTTER_FLAG_OVERRIDE				0x0040		// Set Shutter Motor Overide flag which allows stepping while
														// the home switch is active (1 = active)
#define	SHUTTER_MOTOR_DISABLE				0x0080		// Disable the current to the shutter motor winding (1 = disable) 
#define	SHUTTER_CMD_CLEAR					0x000F		// Mask to clear the register

// Bit 3 Not used
// Bits 8..15 not used

//===========================================================================
// Shutter Motor Step Register - read/write
//===========================================================================
#define SHUTTER_MOTOR_STEP_REG (WORD)	0x0512	    // 0xA6000512 - Shutter Motor Step Register 

// Bits 0..15: Number of steps that Shutter will move when enable is asserted
// Full Range is about 600 steps so number of steps should be limited.
// Shutter will have three positions - Axial (home), Radial, and Closed.
// The Radial, and Closed positions need to be determined for each instrument
// and stored in BRAM

//===========================================================================
// Radial Toroid Motor Step Register - read/write
//===========================================================================
#define TOROID_STEP_REG (WORD)			0x0514	   	// 0xA6000514 - Shutter Motor Step Register 

// Bits 0..15: Number of steps Radial Toroid Motor will move when enable is asserted.
// The Toroid Motor will have two positions.  For Radial measurements it will be positioned in the optical path.  
// For Axial (home) measurements, the radial toroid assembly will be moved out of the optical path.
// The The Radial position needs to be determined for each instrument and stored in BRAM.

//===========================================================================
// Status Register 2 - read only
//===========================================================================
#define STATUS_REG2 (WORD)				0x0516	    // 0xA6000516 - Status Register 2
#define	TOROID_MOTOR_BUSY				0x0001		// Toroid Motor Busy (1 = active)
#define	TOROID_MOTOR_DONE				0x0002		// Toroid Motor Done (1 = active)
#define	TOROID_MOTOR_HOME				0x0004		// Toroid Motor Home (1 = active)
//	Bit 3: Not Used
#define	SHUTTER_MOTOR_BUSY				0x0010		// Shutter Motor Busy (1 = active)
#define	SHUTTER_MOTOR_DONE				0x0020		// Shutter Motor Done (1 = active)
#define	SHUTTER_MOTOR_HOME				0x0040		// Shutter Motor Home (1 = active)
#define	FAN_STATUS						0x0080		// Fan status  (0 = ok)
#define	LID_STATUS						0x0100		// Lid status  (0 = connected)

//	Bits 9..15: Not Used

//===========================================================================
// Diagnostic Register - read/write
//===========================================================================
#define DIAG_REG (WORD)					0x0518	    // 0xA6000518 
#define	TP19_MASK						0x0001		// Bit 0 TP19
#define	TP53_MASK						0x0002		// Bit 1 TP53
#define	TP56_MASK						0x0004		// Bit 2 TP56
#define	TP37_MASK						0x0008		// Bit 3 TP37

//===========================================================================
// ADC MULTIPLEXER - WRITE ONLY
//===========================================================================

#define	ADC_MULTIPLEXER_BASE		(WORD) 0x0400	// 0xa6000400
#define	ADC_MULTIPLEXER_TOP_PRI		(WORD) 0x0400	// 0xa6000400
#define	ADC_MULTIPLEXER_BOT_PRI		(WORD) 0x0402	// 0xa6000402
#define	ADC_MULTIPLEXER_TOP_SEC		(WORD) 0x0404	// 0xa6000404
#define	ADC_MULTIPLEXER_BOT_SEC		(WORD) 0x0406	// 0xa6000406
//===========================================================================
// SOLENOID ACTIVATION REGISTER - WRITE ONLY
//===========================================================================
#define	SOLENOID_REGISTER	(WORD)	0x0480		// 0xa6000480 - WORD m_nSolenoidRegister
#define	ARGON_EN					0x0001		// Enable Argon low flow
#define	ND_FILTER_EN				0x0002		// Enables the Neutral Density Filter for the VIS camera
#define	ARGON_HI					0x0004		// Enable Argon hi flow
//#define	SHUTTER_EN				0x0008		// Close Argon controlled source shutter ################ remove with CBF-24
#define	TOROID_POSITION				0x0008		// Toroid position control bit; 0 = HOME (Axial); 1 = Radial
#define	AVI_EN						0x0010		// Enable AVI Solenoid
#define	NITROGEN_HI					0x0020		// Enable Nitrogen hi flow
#define	WATER_EN					0x0040		// Enable Water into SPectrometer
#define	GRATING_EN					0x0080		// Enable Grating Solenoid

//===========================================================================
// GENERAL STATUS REGISTER - READ ONLY
//===========================================================================
#define	SVSTAT_CS			(WORD)	0x04a0		// 0xa60004a0 - Status bit register (short *)
#define	VIEWX_HOME_AXIAL			0x0001		// Axial X-View motor at home
#define	VIEWX_LIMIT_AXIAL			0x0002		// Axial X-View motor at limit
#define VIEWY_HOME_AXIAL			0x0004		// Axial Y-View motor at home
#define VIEWY_LIMIT_AXIAL			0x0008		// Axial Y-View motor at limit
#define	SLIT_HOME					0x0010		// Slit motor at home
#define	SLIT_LIMIT					0x0020		// Slit motor at limit
#define	WATER_FLOW					0x0040		// New - Indicate Water is flowing
#define	WATER_LEAK					0x0080		// Water leak has occurred
#define	TRIGGER_IN					0x0100		// External trigger to start integrations
#define	NITROGEN_FLOW				0x0200		// Nitrogen flowing start bit
#define	ARGON_FLOW					0x0400		// Argon flowing status bit
// Bit 11 Not used
#define	VIEWX_HOME_RADIAL			0x1000		// Radial X-View motor at home (1 = active)
#define	VIEWX_LIMIT_RADIAL			0x2000		// Radial X-View motor at limit (1 = active)
#define VIEWY_HOME_RADIAL			0x4000		// Radial Y-View motor at home (1 = active)
#define VIEWY_LIMIT_RADIAL			0x8000		// Radial Y-View motor at limit (1 = active)


//===========================================================================
// COMMON MOTOR DRIVES
//===========================================================================
#define CW							   1
#define CCW							   0
#define MAX_S_TIME					   2		// 2 msec
#define MAX_V_TIME					   2		// 2 msec
#define DELTA_TIME					   5		// 10 usec
#define MIN_S_STEPS					3120		// 1.8 motor (wave mode)

#define MIN_X_STEPS_AXIAL			 200		// 0.1mm per half-step (20mm max)
#define MIN_X_STEPS2_AXIAL			 300		// 0.1mm per half-step (20mm max)
#define MIN_Y_STEPS_AXIAL			 240		// 0.25mm per half-step (60mm max)
#define MIN_Y_STEPS2_AXIAL			 120		// 0.25mm per half-step (60mm max)

#define MIN_X_STEPS_RADIAL			 200		// 0.1mm per half-step (20mm max)
#define MIN_X_STEPS2_RADIAL			 300		// 0.1mm per half-step (20mm max)
#define MIN_Y_STEPS_RADIAL			 240		// 0.25mm per half-step (60mm max)
#define MIN_Y_STEPS2_RADIAL			 120		// 0.25mm per half-step (60mm max)

//===========================================================================
// SYSTEM CLOCK & BATTERY RAM STORAGE
//===========================================================================

//===========================================================================
// SYSTEM CLOCK - Offsets from 0xb8000000
//===========================================================================
#define CLKSEC				(WORD)	0x7ff9		//0x7ff9	// SYSTEM CLOCK - SECONDS
#define CLKMIN				(WORD)	0x7ffa		//0x7ffa	// SYSTEM CLOCK - MINUTES
#define CLKHOUR				(WORD)	0x7ffb		//0x7ffb	// SYSTEM CLOCK - HOURS
#define CLKDAY				(WORD)	0x7ffc		//0x7ffc	// SYSTEM CLOCK - DAY OF WEEK
#define CLKDATE				(WORD)	0x7ffd		//0x7ffd	// SYSTEM CLOCK - DATE
#define CLKMONTH			(WORD)	0x7ffe		//0x7ffe	// SYSTEM CLOCK - MONTH
#define CLKYEAR				(WORD)	0x7fff		//0x7fff	// SYSTEM CLOCK - YEAR
#define CLKCMD				(WORD)	0x7ff8		//0x7ff8	// SYSTEM CLOCK - COMMAND REGISTER
#define	CLKREAD						0x40		// Bit 6 - Read
#define CLKWRITE					0x80		// Bit 7 - Write
#define CLKSTART					0x00		// Clear Bits 6 & 7

#define SCLKSEC				(WORD)	0x0101		// SAVED CLOCK - SECONDS
#define SCLKMIN				(WORD)	0x0102		// SAVED CLOCK - MINUTES
#define SCLKHOUR			(WORD)	0x0103		// SAVED CLOCK - HOURS
#define SCLKDAY				(WORD)	0x0104		// SAVED CLOCK - DAY OF WEEK
#define SCLKDATE			(WORD)	0x0105		// SAVED CLOCK - DATE
#define SCLKMONTH			(WORD)	0x0106		// SAVED CLOCK - MONTH
#define SCLKYEAR			(WORD)	0x0107		// SAVED CLOCK - YEAR

#define ARGON_SCLKSEC		(WORD)	0x0109		// SAVED CLOCK - SECONDS
#define ARGON_SCLKMIN		(WORD)	0x010A		// SAVED CLOCK - MINUTES
#define ARGON_SCLKHOUR		(WORD)	0x010B		// SAVED CLOCK - HOURS
#define ARGON_SCLKDAY		(WORD)	0x010C		// SAVED CLOCK - DAY OF WEEK
#define ARGON_SCLKDATE		(WORD)	0x010D		// SAVED CLOCK - DATE
#define ARGON_SCLKMONTH		(WORD)	0x010E		// SAVED CLOCK - MONTH
#define ARGON_SCLKYEAR		(WORD)	0x010F		// SAVED CLOCK - YEAR

//===========================================================================
// BATTERY RAM (BRAM) STORAGE - Offsets from 0xb8000000
//===========================================================================
#define MAX_SLIT			(WORD)	0x0022		// MAX SLIT STEPS (short *)
#define MAX_VIEWX_AXIAL		(WORD)	0x0024		// MAX AXIAL VIEW X STEPS (short *)
#define MAX_VIEWY_AXIAL		(WORD)	0x0026		// MAX AXIAL VIEW Y STEPS (short *)

#define HOME_2_SLIT			(WORD)	0x0028		// HOME TO LOW RES SLIT (short *)
#define HOME_2_PEAK_AXIAL	(WORD)	0x002a		// HOME TO VIEW X PEAK (short *)
#define HOME_2_15MM_AXIAL	(WORD)	0x002c		// HOME TO VIEW Y 15MM (short *)

#define CURR_SLIT			(WORD)	0x002e		// CURRENT SLIT (short *)
#define CURR_VIEWX_AXIAL	(WORD)	0x0030		// CURRENT AXIAL VIEW X (short *)
#define CURR_VIEWY_AXIAL	(WORD)	0x0032		// CURRENT AXIAL VIEW Y (short *)

#define CURR_SLIT_TYPE		(WORD)	0x0034		// CURRENT SLIT TYPE (char *)
#define CURR_SLIT_POS		(WORD)	0x0035		// CURRENT SLIT POSITION (char *)
#define CURR_VIEWX_POS_AXIAL (WORD)	0x0036		// CURRENT AXIAL VIEW POSITION (char *)
#define CURR_VIEWY_POS_AXIAL (WORD)	0x0037		// CURRENT AXIAL VIEW POSITION (char *)

#define HOME_2_HG_X_AXIAL	(WORD)	0x0038		// HOME TO VIEW X Hg (short *)
#define HOME_2_HG_Y_AXIAL	(WORD)	0x003a		// HOME TO VIEW Y Hg (short *)

#define LAST_PURGE			(WORD)	0x003c		// LAST PURGE REMEMBERED (char *)
#define	NITROGEN			(WORD)	0x003d		// NITROGREN FLOW RATE (char *)
#define SYNC_MODE			(WORD)	0x003e		// SYNC MODE SETTING (char *)
#define ARGON_VALVE_OK		(WORD)	0x003f		// ARGON VALVE STATUS AT STARTUP (char *)
#define VALVE				(WORD)	0x0040		// LAST VALVE SETTING REMEMBERED (short *)

#define RAM_ERROR			(WORD)	0x0042	 	// LOCATION OF RAM ERROR (long *)

#define LED_LEVEL			(WORD)	0x0046		// BRIGHTNESS LEVEL FOR LED VIA D/A #2 (short *)

#define DRAM0_ERROR			(WORD)	0x0048		// chkRAM error bytes (char *)
#define DRAM1_ERROR			(WORD)	0x0049	 	// chkRAM error bytes (char *)
#define DRAM2_ERROR			(WORD)	0x004a	 	// chkRAM error bytes (char *)
#define DRAM3_ERROR			(WORD)	0x004b		// chkRAM error bytes (char *)
#define DRAM4_ERROR			(WORD)	0x004c		// chkRAM error bytes (char *)
#define DRAM5_ERROR			(WORD)	0x004d		// chkRAM error bytes (char *)
#define DRAM6_ERROR			(WORD)	0x004e		// chkRAM error bytes (char *)
#define DRAM7_ERROR			(WORD)	0x004f		// chkRAM error bytes (char *)

#define VISDETSN			(WORD)	0x0060		// vis detector s/n msb (6 bytes) (char *)
#define UVDETSN				(WORD)	0x0066		// uv detector s/n msb (6 bytes) (char *)
#define VISCOEF1			(WORD)	0x006c		// vis coefficant C0 (9 bytes) (char *)
#define VISCOEF2			(WORD)	0x0075		// vis coefficant C1 (9 bytes) (char *)
#define VISCOEF3			(WORD)	0x007e		// vis coefficant C2 (9 bytes) (char *)
#define UVCOEF1				(WORD)	0x0087		// uv coefficant C0  (9 bytes) (char *)
#define UVCOEF2				(WORD)	0x0090		// uv coefficant C1  (9 bytes) (char *)
#define UVCOEF3				(WORD)	0x0099		// uv coefficant C2  (9 bytes) (char *)
#define SERVICID			(WORD)	0x00a2		// service id location (6 bytes) (char *)

#define WCLKMIN				(WORD)	0x00a8		// WAKEUP CLOCK - MINUTES (char *)
#define WCLKHOUR			(WORD)	0x00a9		// WAKEUP CLOCK - HOURS (char *)
#define WCLKDATE			(WORD)	0x00aa		// WAKEUP CLOCK - DATE (char *)
#define WCLKMONTH			(WORD)	0x00ab		// WAKEUP CLOCK - MONTH (char *)
#define WCLKYEAR			(WORD)	0x00ac		// WAKEUP CLOCK - YEAR (char *)

#define DUAL_VIEW_CODE_1	(WORD)	0x00ad		// DUAL VIEW CODE - "D" (char *)
#define DUAL_VIEW_CODE_2	(WORD)	0x00ae		// VIEW CODE - "V" (char *)
#define DUAL_VIEW_CODE_3	(WORD)	0x00af		// VIEW CODE - "-" (char *)
#define DUAL_VIEW			(WORD)	0x00b0		// DUAL VIEW SETTING (char *)
#define HOME_2_PEAK2		(WORD)	0x00b2		// HOME TO DUAL VIEW X PEAK (short *)
#define HOME_2_15MM2		(WORD)	0x00b4		// HOME TO DUAL VIEW Y 15MM (short *)

#define SINGLE_CHANNEL_1	(WORD)	0x00b6		// SINGLE CHANNEL CODE - "S" (char *)
#define SINGLE_CHANNEL_2	(WORD)	0x00b7		// SINGLE CHANNEL CODE - "C" (char *)

#define CHIP_TYPE_CODE_1	(WORD)	0x00b8		// CHIP TYPE CODE - "C" (char *)
#define CHIP_TYPE_CODE_2	(WORD)	0x00b9		// CHIP TYPE CODE - "T" (char *)
#define CHIP_TYPE			(WORD)	0x00ba		// CHIP TYPE SETTING (char *)
#define CHIP_MODE			(WORD)	0x00bb		// CHIP MODE SETTING (char *)
#define GRATING				(WORD)	0x00bc		// GRATING SETTING - 0 / 1 (char *)

#define UNSOLICITED_MSG		(WORD)	0x00bd		// UNSOLICITED MESSAGES - 0 / 1 (char *)

#define VIEW_TYPE			(WORD)	0x00c2		// VIEW TYPE SETTING (char *)

// Heat Parameters
#define	LINE_FREQUENCY		(WORD)	0x00c4		// Line frequency (50/60) (short *)

#define	DEBUG_FLAG			(WORD)	0x00fa		// Debug options flags (short *)
#define	HARDWARE_VER		(WORD)	0x00fc		// Hardware version flag (unsigned short *)
#define HEATER_CONTROL		(WORD)	0x00fe		// Heater Control ON/OFF (char *)

#define INTEGRATOR_PRESET	(WORD)	0x0112		// Integrator Preset (short *)
#define LID_POWER_SELECT	(WORD)	0x0116		// Lid power selector during warmup; '0' = LID_POWER_LEVEL; '1' = 100%

//===============================================
// NOTE THE NEXT AVAILABLE LOCATION IS $b8000154
//===============================================

//===========================================================
// NOTE THAT LOCATIONS $b8000101 TO $b800011f ARE USED ABOVE    
// IN ASSOCIATION WITH THE SYSTEM CLOCK - CURRENT TIME SAVED
// THEREFORE THE NEXT ADDRESS AVAILABLE IS $b8000121	    
//===========================================================

#define INSTRUMENT_SN		(WORD)	0x0120		// Instrument serial number (11 bytes)

//==========================================
// More heater control algorithm parameters
//==========================================

#define OPTICS_HOURS		(WORD)	0x014c		// Optics hours (seconds) (long *)

#define AUTO_RESET_TIME		(WORD)	0x0254		// Reset interval when not connected to PC and READY
												// Range: 0=Off / 1 to 999 in tenths of hours
#define THREAD_DATA			(WORD)	0x0156		// Thread debug data (16 bytes)
#define	THREAD_TIME			(WORD)	0x0176		// Tick Count of Thread debug data (16 dwords)
#define THREAD_LAST			(WORD)	0x01c6		// Tick Count Difference form last entry (16 dwords)

#define SENSOR_UPPER_ACTIVE					(WORD)	0X0168	// Upper Sensor Activated	(short *)
#define SENSOR_LOWER_ACTIVE					(WORD)	0X016A	// Lower Sensor Activated	(short *)
#define	CALIBRATE_SENSOR_TOP_PRIMARY		(WORD)	0X016C	// Calibrate Top Primary Sensor	(short *)
#define	CALIBRATE_SENSOR_BOTTOM_PRIMARY		(WORD)	0X016E	// Calibrate Bottom Primary Sensor	(short *)
#define	CALIBRATE_SENSOR_TOP_SECONDARY		(WORD)	0X0170	// Calibrate Top Secondary Sensor	(short *)
#define	CALIBRATE_SENSOR_BOTTOM_SECONDARY	(WORD)	0X0172	// Calibrate Bottom Secondary Sensor	(short *)
#define CALIBRATE_SENSOR_AVERAGE			(WORD)	0X0174	// Calibrate Sensor Average	(short *)

// BRAM for Century B functionality starts at 0x0300

#define MAX_VIEWX_RADIAL		(WORD)	0x0300		// MAX RADIAL VIEW X STEPS (short *)
#define MAX_VIEWY_RADIAL		(WORD)	0x0302		// MAX RADIAL VIEW Y STEPS (short *)
#define CURR_VIEWX_RADIAL		(WORD)	0x0304		// CURRENT RADIAL VIEW X (short *)
#define CURR_VIEWY_RADIAL		(WORD)	0x0306		// CURRENT RADIAL VIEW Y (short *)
#define CURR_VIEWX_POS_RADIAL 	(WORD)	0x0308		// CURRENT RADIAL VIEW POSITION (char *)
#define CURR_VIEWY_POS_RADIAL 	(WORD)	0x030A		// CURRENT RADIAL VIEW POSITION (char *)
#define HOME_2_PEAK_RADIAL		(WORD)	0x030C		// HOME TO VIEW X PEAK (short *)
#define HOME_2_15MM_RADIAL		(WORD)	0x030E		// HOME TO VIEW Y 15MM (short *)

#define TOROID_POS_RADIAL		(WORD)	0x0310		// Not used.  Keep for now.  This space is availble.
#define SHUTTER_POS_RADIAL		(WORD)	0x0312		// Key shutter radial position
#define SHUTTER_POS_CLOSED		(WORD)	0x0314		// Key shutter closed position
#define SHUTTER_POSITION_MAX	(WORD)	0x0316		// Max shutter position

// NOTE THE NEXT AVAILABLE LOCATION IS $300406
//=============================================

#define BRAM_END			    (WORD)	0x0405


//=============================================
// NOTE PARAMETER OUTSIDE OF BRAM
//=============================================
#define	CALIBRATE_SENSORS_INIT	(LONG)	0x0410		// Initiate sensors calibration


/*** Revision History ***/


