//===========================================================================
//
//	Module Name:	CommandServer.cpp
//
// 	Function:		This routine handles the TcpIp interface
//
//	Original Author:	T Frazzini
//
//	Copyright (c) 2005,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================

/*** Last check-in info ***

$Header: /IcarusBased/SpecFW/CommandServer.cpp 37    6/29/07 5:08p Nashth $

(The remainder of the revision history is located at the bottom of this file)

***************************/

#include "StdAfx.h"
#include "SpecFW2.h"
#include "CommandServer.h"
#include "Debug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define COMMAND_PORT	6000	// port number
#define NUM_CONNECTION	2		// number of connections accepted (1 is probably about right)

UINT CCommandServer::tm_CCommandServerReplyToCommand = ::RegisterWindowMessage( L"CCommandServer OnReplyToCommand" );

//////////////////////////////////////////////////////////////////////
//////// added a pump //////////////////////////////////
BEGIN_MESSAGE_MAP(CCommandServer, CWinThread)
	//{{AFX_MSG_MAP(CCommandServer)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(tm_CCommandServerReplyToCommand, OnReplyToCommand)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CCommandServer , CWinThread )

CCommandServer::CCommandServer(): CTcpServer ( COMMAND_PORT,NUM_CONNECTION,0 )
{
	m_pParserThread = (CParserThread*) ::AfxBeginThread( RUNTIME_CLASS( CParserThread ), THREAD_PRIORITY_NORMAL );	// THREAD_PRIORITY_ABOVE_NORMAL
	ASSERT( m_pParserThread );
	theApp.RegisterThread(L"Parser", (DWORD)m_pParserThread);  // Assumption: ptr->Thread == hThread == dwThreadID
}
//===========================================================================

CCommandServer::~CCommandServer()
{
}
//===========================================================================

int CCommandServer::Run() // CThread is derived from CWinThread.
{
   while(1)
   {
      // just idle.
		Sleep(1000);
   }
   return 0;
}
//===========================================================================

BOOL CCommandServer::InitInstance( void )
{
	return TRUE;
}
//===========================================================================

int CCommandServer::ExitInstance( void )
{

	return CWinThread::ExitInstance( );
}
//===========================================================================

void CCommandServer::CreateServerThread ( void )
{
	DWORD   threadID;
	m_pParserThread->SetParent(this);

	threadHANDLE = ::CreateThread(NULL, 0, (unsigned long (__cdecl *)(void*))ServerThread, this, CREATE_SUSPENDED, &threadID);
	if ( threadHANDLE != NULL )
	{
		theApp.RegisterThread(L"CommandServer", threadID); 
		::SetThreadPriority(threadHANDLE, THREAD_PRIORITY_NORMAL);
		::ResumeThread(threadHANDLE);
	}
}
//===========================================================================

void CCommandServer::ServerThread ( void * param )
{
	CCommandServer *localCommandServer = (CCommandServer *) param;

#define COMMAND_BUFLEN	1024
	char	RecBuf[COMMAND_BUFLEN];
	int		numBytesReceived;
	char	 CmdBuf[COMMAND_BUFLEN];
	BYTE	bYear,  bYearWake;
	BYTE	bMonth, bMonthWake;
	BYTE	bDate,  bDateWake;
	BYTE	bHour,  bHourWake;
	BYTE	bMin,   bMinWake;
	BYTE	bSec;
	bool	bSocketState;				// True if a valid socket was found; else false;
	bool	bTerminating	= false;	// Marks awaitng to terminate

	// start tcp server
	if ( localCommandServer->startServer () != INVALID_SOCKET )
	{
		int bufIdx = 0;
		for ( ; ; )
		{
			// check for new data
			numBytesReceived = localCommandServer->receiveData ( RecBuf, sizeof(RecBuf)/sizeof(RecBuf[0]), localCommandServer->m_SlotIndex, bSocketState );

			if ( numBytesReceived > 0 )
			{
				// potentially, a command can cross two packets
				// so we have to deal with that. 
				if (bufIdx > COMMAND_BUFLEN)
					bufIdx = 0;	// just in case, so we won't step in the weeds when someone sends us 1K of info without a null
								// may want to deal with this by sending back a nack or something..

				memcpy(&CmdBuf[bufIdx], RecBuf,numBytesReceived);	// copy whatever was sent to our parser buffer
				bufIdx += numBytesReceived;	// update total length

				if (localCommandServer->m_pParserThread->m_nDebug & DEBUG_COMMANDS)
				{	// Output the command received to the HyperTerminal or display window
					char	cBuffer[COMMAND_BUFLEN];
					memcpy(&cBuffer[0], RecBuf, numBytesReceived);
					cBuffer[numBytesReceived] = NULL;
					printf("Command IN:  %s\r\n", cBuffer);
				}

				// since all commands are always replied to it is safe to assume
				// that another command will not follow the null. 
				// Since that is assumed true, one can thus assume that
				// if the last character is null, a command string needs to be executed
				//
//				if (RecBuf[numBytesReceived - 1] == 0 ) // if last character sent was a null
				{
//					char * parseBuffer= new char [bufIdx + 1];	// allocate a buffer to parse, to be deleted by whatever executes the command.
//																// this is done, because potentially CmdBuf can be overwritten inadvertently whenever something comes down the pike (shouldn't happen, but just in case)
//					memcpy(parseBuffer, CmdBuf, bufIdx);
					localCommandServer->m_pParserThread->ParseThis(CmdBuf, bufIdx);
					bufIdx = 0;
				}
			}
			else
			{
				if (localCommandServer->m_pParserThread->m_bSleepFlag)
				{
					localCommandServer->m_pParserThread->GetTimeValues(bYear, bMonth, bDate, bHour, bMin, bSec);
					localCommandServer->m_pParserThread->GetWakeupTimeValues(bYearWake, bMonthWake, bDateWake, bHourWake, bMinWake); 
			
					if (bYearWake == bYear)
					{
						if (bMonthWake == bMonth)
						{
							if (bDateWake == bDate)
							{
								if (bHourWake == bHour)
								{
									if (bMinWake <= bMin)
									{
										// WAKEUP SIMILAR TO REINITIALIZE COMMAND
										localCommandServer->m_pParserThread->m_cInitFlag = FATAL;
										// TLF - Restart - similar to PSOS Call to start
										
										long	nRetCode;
										CString	sPath;
										CString sCmdLine; 
										PROCESS_INFORMATION processInfo;
										sPath =    L"\\Applications\\LaunchSpecFW.exe"; // Launch the ReLauncher
										sCmdLine = L"30"; // Request a delay, allowing time for this app to end
								
										if ( ! bTerminating )  // Only through here once
										{
											//printf("Awake from sleep...CreateProcess(...)\r\n");
											do
											{
												nRetCode = CreateProcess( sPath, sCmdLine, NULL, NULL, NULL,
																		    NULL,NULL, NULL,
																			NULL, &processInfo);
												if ( ! nRetCode )
												{
												  Sleep( 1000 );
												}
											} while ( ! nRetCode );   // Remain in Fatal if the relauncher cannot be started

											theApp.m_pMainWnd->PostMessage(WM_QUIT, NULL, NULL);
											bTerminating = true;  
										}
									}
								}
							}
						}
					}
				}
				Sleep(1);
			}
		}
	}
	else
	{
		AfxMessageBox(TEXT("Error invalid socket"));
	}
}
//===========================================================================

void CCommandServer::ExitServerThread ( void )
{
	if ( threadHANDLE )
	{
		TerminateThread ( threadHANDLE, 0 );
		WaitForSingleObject ( threadHANDLE, 5000 );
		CloseHandle(threadHANDLE);
	}
}
//===========================================================================

void CCommandServer::OnReplyToCommand(WPARAM wParam, LPARAM lParam)
{
	long	lSent = sendData( m_SlotIndex, (char*) wParam, (long) lParam );

	long lParamValue = lParam & READ_SIZE_MASK;

	if (lSent != (long) lParamValue)
	{
		syslog( "Bytes sent do not agree with Bytes to send: %d != %d", lSent, lParamValue);
	}
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////
// Parsing on whatever was sent last is done, so this member is called by the 
// parser. It passes a message to itself indicating to send an appropriate reply
UINT CCommandServer::CommandParsingDone(char *reply, long lBytes)
{
#if 0
	// I don't think the following is really needed, but I'll leave it
	// in case we need to shovel it all through the message pump.
	UINT lastError;
	UINT status = CWinThread::PostThreadMessage( tm_CCommandServerReplyToCommand, (WPARAM) reply, (LPARAM) lBytes );
	if (status == 0)
		lastError = GetLastError();
	return status;
#else
	// just send the reply directly
	OnReplyToCommand((WPARAM) reply, (LPARAM) lBytes);
	return S_OK;
#endif
}
//===========================================================================

/////////////////////////////////////////////////////////////////////////////

/*** Revision History ***

$Log: /IcarusBased/SpecFW/CommandServer.cpp $
 * 
 * 37    6/29/07 5:08p Nashth
 * Placed hardware clock access functions into sub functions in
 * preperation for additional detector read diagnostic as per QPR:
 * THNJ74CQCW.
 * 
 * 36    3/07/07 4:17p Nashth
 * Attempt of improving the fix to QPR: THNJ6XXKR5...
 * Should the camera turn itself off at ANY time, then the
 * firmware will turn it back on.
 * 
 * 35    2/13/07 5:14p Nashth
 * LaunchSpecFW now functioning correctly with SpecFW.
 * 
 * 34    2/12/07 1:47p Nashth
 * First pass attempt at relaunching SpecFW at completion of sleep.
 * 
 * 33    2/02/07 3:26p Nashth
 * Final pass at keeping the camera on during a 'long' connection as per
 * QPR: THNJ6XXLK3.
 * 
 * 32    2/02/07 2:45p Nashth
 * WatchDog task thread priority set back to normal and debug printf
 * statements commented out.
 * 
 * 31    2/01/07 2:02p Nashth
 * Small correction to previous update.
 * 
 * 30    1/31/07 4:26a Nashth
 * Removed two compiler warnings.
 * 
 * 29    1/30/07 4:08p Nashth
 * First pass at keeping the camera on during a 'long' connection as per
 * QPR: THNJ6XXLK3.
 * 
 * 28    1/23/07 10:09a Nashth
 * Added two functions AR<000> sets Auto Reset Time and
 * RA that retreives Auto Reset Time. Units = tenths of hours.
 * 
 * 27    1/22/07 2:35p Nashth
 * Modified to reset every 24 hours from time of last command received,
 * rather than invalid socket. Will cover non-gracefull exits such as PC
 * power off or Ethernet cable unplugged. These two cases leave the
 * firmware socket open.
 * 
 * 26    1/19/07 10:18a Nashth
 * Set to Reboot (RI) every 24 hours from time of PC disconnect when in
 * READY mode while disconnected.
 * 
 * 25    6/27/06 11:03a Nashth
 * Invoked IcarusReset from the windows path as per QPR: THNJ6R6KKU.
 * 
 * 24    3/02/06 9:20a Frazzitl
 * Added debugging to help find loss of communications problem.
 * 
 * 23    11/29/05 11:30a Nashth
 * Trace printfs added to Main critical section for Enter/Leave for
 * debugging.
 * 
 * 22    11/11/05 8:30a Frazzitl
 * Added special clock critical section around all clock functions.
 * 
 * 21    10/28/05 5:36p Nashth
 * Fixed a typo in a debug printf.
 * 
 * 20    10/28/05 3:27p Nashth
 * Added a sleep to allow the IcarusReset process a chance to run such
 * that sleep mode will terminate as per APR CANU6H3NLT.
 * 
 * 19    10/28/05 10:29a Nashth
 * Printfs added to debug QPR CANU6H3NLT.
 * 
 * 18    9/20/05 4:21p Nashth
 * Added reference to THNJ6DNPLE to entry 16 below that waas previously
 * omitted.
 * 
 * 17    9/02/05 12:00p Nashth
 * Fixed Electronics temperature scaling as per QPR: THNJ6FULN9
 * 
 * 16    8/31/05 15:33 Frazzitl
 * Added call to Reinitialize function (cmdRI) to restart firmware.
 *          as per THNJ6DNPLE.
 * 
 * 15    8/18/05 13:49 Frazzitl
 * Implemented Sleep mode.  Time is checked in CommandServer if enabled.
 * At correct time, the system must be restarted - not yet implemented.
 * Disabled if any legal Mode Command is accepted.
 * 
 * 14    7/08/05 9:57 Frazzitl
 * Fixed problem with system clock updating.
 * 
 * 13    6/20/05 16:26 Frazzitl
 * Displays into syslog if the byte count for a command reply does not
 * equal what should have been written.
 * 
 * 12    5/25/05 16:28 Frazzitl
 * Trying to get the threads to work properly when run stand-alone
 * 
 * 11    5/17/05 13:52 Frazzitl
 * Renamed ParserThread handle
 * 
 * 10    5/16/05 17:39 Frazzitl
 * Removed 100 sleep to speed up response
 * 
 * 9     4/29/05 12:49 Frazzitl
 * Trying to get the threat to work properly
 * 
 * 8     4/28/05 10:44 Frazzitl
 * Fixed most warning generated when compiler set to Level 4 - per code
 * review
 * 
 * 7     4/22/05 15:18 Frazzitl
 * Improved Status reads
 * 
 * 6     4/20/05 11:28 Frazzitl
 * Made more easily readable
 * 
 * 5     4/08/05 8:30 Frazzitl
 * Temporary change for testing
 * 
 * 4     4/07/05 1:32p Frazzitl
 * Fixed problem with clock
 * 
 * 3     3/30/05 2:59p Frazzitl
 * Fixed some startup problems
 * 
 * 2     3/24/05 15:17 Frazzitl
 * More improvements
 * 
 * 1     3/17/05 11:21 Frazzitl
 * Initial version of Optima Spectrometer firmware using the Icarus board,
 * TcpIp, and the new Sarnoff detector.

$NoKeywords: $

** End of Rev History **/
