/***** Last check-in info *****

$Header: /IcarusBased/SpecFW/TcpServer.cpp 4     1/19/07 10:18a Nashth $

(The remainder of the revision history is located at the bottom of this file)

*******************************/

// Copyright © 2006  PerkinElmer Inc,  All rights reserved.


#include "stdafx.h"
#include <winsock.h>
#include "socketbase.h"
#include "TcpServer.h"


CTcpServer::CTcpServer ( int _port, int _maxConnections, int _debug ) : Socket ( _debug )
{
	int		i;

	debug          = _debug;
	port           = _port;
	maxConnections = _maxConnections;
	connectList    = new int[maxConnections];
	for ( i=0; i<maxConnections; i++ )
	{
		connectList[i] = INVALID_SOCKET;
	}
}

CTcpServer::~CTcpServer ( void )
{
	stopServer ();
	if ( connectList )
		delete connectList;
}

void CTcpServer::stopServer ( void )
{
	int		i;

	for ( i=0; i<maxConnections; i++)
	{
		connectList[i] = closeSocket ( connectList[i], Socket::linger );
	}
	socket = closeSocket ( socket, Socket::linger );
}

int CTcpServer::startServer ( int _reuse )
{
	for ( ;; )
	{
		socket = createSocket ( _reuse );
		if ( socket == INVALID_SOCKET )
		{
			break;
		}
		socket = setNonBlocking ( socket );
		if ( socket == INVALID_SOCKET )
		{
			break;
		}

		/* wait for connection from client */
		set_addr ( &peer_addr, (u_short)port, INADDR_ANY );
		if ( bind( socket, (sockaddr*)&peer_addr, sizeof(peer_addr) ) == -1 )
		{
			if ( debug & log_errors )
				syslog ( "bind() failed: %s" );
			socket = closeSocket ( socket, Socket::linger );
			break;
		}
		if ( listen( socket, maxConnections ) == -1 )
		{
			if ( debug & log_errors )
				syslog ( "listen() failed %s" );
			socket = closeSocket ( socket, Socket::linger );
			break;
		}
		if ( debug & log_infos )
		{
			syslog ( "Non blocking Tcp server successfully started." );
			syslog ( "Waiting for connections ..." );
		}
		break;
	}
	return socket;
}

void CTcpServer::buildSelectList ( int *hs )
{
	int		i;

	FD_ZERO ( &readfds );
	FD_SET ( socket, &readfds );
	for ( i=0; i<maxConnections; i++ )
	{
		if ( connectList[i] != INVALID_SOCKET )
		{
			FD_SET ( connectList[i], &readfds );
			if ( connectList[i] > *hs )
				*hs = connectList[i];
		}
	}
}

void CTcpServer::newConnection ( void )
{
	struct	timeval	wait;
	int		rs;
	// sockets
	int		highSocket;
	int		listenSocket;
	int		i;

	/* handle incoming messages */
	wait.tv_sec  = 0;
	wait.tv_usec = 100;
	highSocket   = socket;
	buildSelectList ( &highSocket );
	rs = select ( FD_SETSIZE, &readfds, (fd_set*)0, (fd_set*)0, &wait );
	if ( rs < 0 )
	{
		if ( debug & log_errors )
			syslog ( "select() failed: %s" );
	}
	else if ( rs == 0 )
	{
	}
	else
	{
		/* check new connection */
		if ( FD_ISSET ( socket, &readfds ) )
		{
			peer_len = sizeof(peer_addr);
			if ( (listenSocket = accept ( socket, (sockaddr*)&peer_addr, &peer_len )) == -1 )
			{
				if ( debug & log_errors )
					syslog ( "accept() failed: %s" );
			}
			else
			{
				/* set socket options (non blocking) */
				if ( setNonBlocking ( listenSocket ) != INVALID_SOCKET )
				{
					for ( i=0; i<maxConnections; i++)
					{
						if ( connectList[i] == INVALID_SOCKET )
						{
							connectList[i] = listenSocket;
							if ( debug & log_infos )
								syslog ( "Connection accepted: FD=%d; Slot=%d", connectList[i], i );
							break;
						}
					}

					if ( i == maxConnections )
					{
						if ( debug & log_errors )
							syslog ( "No room left for new client.");
						closeSocket ( listenSocket, Socket::linger );
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------
//  Sets bSocketState true when at least one socket was valid; else false.
//
int CTcpServer::receiveData ( char *buffer, int size, int &index, bool &bSocketState )
{
	int		i;
	int		ret = 0;

	// check for new connections
	newConnection ();

	bSocketState = false; // Set for No valid sockets

	/* Now check connectlist for available data */
	for ( i=0; i<maxConnections; i++ )
	{
		if ( connectList[i] != INVALID_SOCKET )
		{
			bSocketState = true; // At least one socket is connected
			if ( FD_ISSET ( connectList[i], &readfds ) )
			{
				/* receive message from client */
				ret = recv ( connectList[i], buffer, size, 0 );
				if ( ret < 0)
				{
					if ( debug & log_errors )
					{
						syslog ( "recv() failed: %s" );
						syslog ( "Connection closed : FD=%d; Slot=%d", connectList[i], i );
					}
					connectList[i] = closeSocket ( connectList[i], Socket::linger );
				}
				else if ( ret == 0 )
				{
					if ( debug & log_errors )
					{
						syslog ( "Connection closed : FD=%d; Slot=%d", connectList[i], i );
					}
					connectList[i] = closeSocket ( connectList[i], Socket::linger );
				}
				else
				{
					buffer[ret] = 0;
					if ( debug & log_data )
					{
						syslog ( "receive: %s\n", buffer );
					}
					index = i;
					break;
				}
			}
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
//  sendData
//
//	index:	Index into list of clients currently connected
//  buffer: Ptr of buffer to send
//  size:   number of bytes of buffer to send
//
//	Returns the number of bytes sent or a negative error code
//
//  Note that send will continue to be called until all of buffer is sent as indicated by size  
//   
//  READ_SIZE_MASK when or'd into the size parameter will perform a blocked send command.
//  The mode is returned to unblocked prior to exit.

int CTcpServer::sendData ( int index, char *buffer, int size )
{ // REMCO and Tom N.
	int		nRet = 0;				// Return code... either positive byte count or negative error
	int		nBytesSent = 0;			// Accumulation of bytes sent
	int     nBytesLeft;				// Bytes remaining to send
	bool	bBlockFlag;				// Blocking flag ... send is performed in Socket Block mode


	bBlockFlag = (size & BLOCKED_READ_MASK) ? true : false;
	size = size & READ_SIZE_MASK;			// Remove flag from size parameter
	nBytesLeft = size;

	if ( bBlockFlag )
		setBlocking ( connectList[index] );	// Set socket for blocked I/O

	while(nBytesLeft > 0)
	{
		nRet = send ( connectList[index], buffer+nBytesSent, nBytesLeft, 0 );
		if ( nRet < 0 )
		{
			if ( debug & log_errors )
			{
				syslog ( "send() failed: %s" );
				syslog ( "Connection closed: FD=%d Slot=%d", connectList[index], index );
			}
			connectList[index] = closeSocket ( connectList[index], Socket::linger );
			break; // Exit read loop on error
		}
		else  // Successful send ... or portion of a send
		{
			nBytesSent += nRet;    // Accumulate bytes sent
			nBytesLeft -= nRet;	   // Decrement  bytes yet to send
			nRet = nBytesSent;     // Update return value with number of bytes sent
		}
	}
	if (nBytesSent != size)
	{
		if ( debug & log_errors )
		{
			syslog( "Bytes sent do not agree with Bytes to send: %d != %d, Return code = %d", nBytesSent, size, nRet);
		}
	}
	if (nRet > -1)
		nRet = nBytesSent;

	if ( bBlockFlag )
		setNonBlocking ( connectList[index] );	// Return socket to unblocked state

	return nRet;
}

/***** Revision History *****/
