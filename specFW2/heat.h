//===========================================================================
//
//	Module Name:	heat.h
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================

// Heater states
#define	HEATER_OFF			0
#define	HEATER_SHUT_DOWN	1
#define	HEATER_ON			2

// Heater Modes
#define	MODE_WARMUP			'0'
#define	MODE_STABILIZATION	'1'
#define	MODE_NORMAL			'2'
#define MODE_LID_OFF		'3'

// Defaults
#define LINE_FREQUENCY_DEFAULT		60
#define INTEGRATOR_PRESET_DEFAULT	5000	// Scaled by 100

// Valid ranges
#define INTEGRATOR_PRESET_MIN		1000		// Scaled by 100
#define INTEGRATOR_PRESET_MAX		9000		// Scaled by 100

// Heater system warnings
#define	TEMP_TUB_LOW				0x0040
#define	TEMP_TUB_HIGH				0x0080
#define	TEMP_BOTTOM_OVER			0x0100
#define	TEMP_TUB_OVER				0x0200


/*** Revision History ***/

