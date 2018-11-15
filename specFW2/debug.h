//===========================================================================
//
//	Module Name:	debug.h
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer. All rights reserved.
//
//===========================================================================

//#define	DEBUG_ANALOG	0x0001		// Not used
#define	DEBUG_HEATER	0x0002
#define	DEBUG_SERVO		0x0004
#define	DEBUG_TEMPS	   	0x0008
#define DEBUG_PI	 	0x0010		
#define DEBUG_FDR		0x0020		
#define DEBUG_FDV		0x0040		
//#define DEBUG_RAW_ANALOG 0x0080	// Not used
#define DEBUG_DETECTOR	0x0100
#define DEBUG_DATA		0x0200
#define DEBUG_MISC		0x0400		// Miscellaneous
#define DEBUG_CS		0x0800		// Critical Section trace
#define DEBUG_THREADS	0x1000		// Thread timimg
#define DEBUG_TEMP_MUX	0x2000		// Four position temperature multiplexor reads
#define DEBUG_COMMANDS	0x4000		// Print commands during input & output

#define HV_OLD_DAQ		1

//#define DEBUG_DELAY_MIN		1	// CBF-143
//#define DEBUG_DELAY_MAX		600
//#define DEBUG_DELAY_DEFAULT	60

// Event definitions for Sample task
#define	DEBUG_DELAYED		0x01
#define	NEW_DELAY			0x02
#define OPTICS_TIMER_START	0x04
#define OPTICS_TIMER_STOP	0x08
#define	OPTICS_TIMER_EVENT	0x10
#define	STABILIZED			0x20 

//===========================================================================

/*** Revision History ***/

