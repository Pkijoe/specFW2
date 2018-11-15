//===========================================================================
//
//	Module Name:	flash.h
//
//	Original Author:	TJWatko
//
//	Copyright (c) 2005,  PerkinElmer, Inc. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /WinLab/SpecFW/flash.h 1     3/17/05 11:22 Frazzitl $

(The remainder of the revision history is located at the bottom of this file)

***************************/

// Flash program area
#define	MAX_FLASH_FUNCTION_SIZE		1024

// Flash addresses
#define	FLASH1	(char *)	0xaaaa		// First write address for program enable
#define	FLASH2	(char *)	0x5555		// Second write address for program enable

// Flash Write Data
#define	FLASH_UNLOCK_1			0xaa
#define	FLASH_UNLOCK_2			0x55
#define	FLASH_SECTOR_ERASE_1	0x80
#define	FLASH_SECTOR_ERASE_2	0xAA
#define	FLASH_SECTOR_ERASE_DATA	0x30
#define	FLASH_PROGRAM			0xa0

// Data polling bits
#define	DQ7		0x80
#define	DQ5		0x20

// Kernel sector addresses
#define	KERNEL_1_SA			(volatile short *) 0x0
#define	KERNEL_2_SA			(volatile short *) 0x10000

// Application sector addresses
#define	APPLICATION_1_SA	(volatile short *) 0x20000
#define	APPLICATION_2_SA	(volatile short *) 0x30000
#define	APPLICATION_3_SA	(volatile short *) 0x40000

// Data Acquisition sector addresses
#define	DATA_ACQ_1_SA		(volatile short *) 0x50000
#define	DATA_ACQ_2_SA		(volatile short *) 0x60000
#define	DATA_ACQ_3_SA		(volatile short *) 0x70000

// Data sector addresses
#define	DATA_1_SA			(volatile short *) 0x78000
#define	DATA_2_SA			(volatile short *) 0x7a000
#define	DATA_3_SA			(volatile short *) 0x7c000

void	chkFLASH();							// Determine if FLASH or overlay RAM
short	DacqEras();							// Erase 2 Data Acquisition Sectors
//char	kernel();
short	KernEras();							// Erase 2 Kernel Sectors
void	kerntask();
short  	MaskInt(short);						// Make interrupt levels
short  	MoveProg(short (*)());				// Move program or erase function to RAM
short  	RunProg(short *, short *, short);	// Execute program or erase function in RAM
short  	SectEras(short *);					// Erase sector at specified address
short  	SectProg(short *, short *, short);	// Program at specified add with array of words with specified length
short	SRdecode(char *, short *, char *, short *); 	// Decode S Record
short	SRecord(long);						// Read S Records and program, with offset
void	start();

// 
#define	NO_DATA_0	(short *) 0x0

// SRECORD Return values
#define	SRECORD_OK			1
#define	SRECORD_ERROR		2
#define	SRECORD_CHECKSUM	3
#define	SRECORD_IGNORE		4
#define	SRECORD_END			5
#define	SRECORD_BEGIN		6

extern	short	Flash;				// YES if running in flash

//===========================================================================

/*** Revision History ***

$Log: /WinLab/SpecFW/flash.h $
 * 
 * 1     3/17/05 11:22 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
