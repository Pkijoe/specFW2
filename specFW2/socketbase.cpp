/************************************************************************/
/*he                                                                    */
// Copyright © 2006  PerkinElmer Inc,  All rights reserved.
/*he    Copyright (C) 1998-2000 by Embedded System Engineering GmbH     */
/*he         Muehlbachstr.20, 78351 Bodman-Ludwigshafen/Bodensee        */
/*he           Tel.: (07773) 9388-0   Fax: (07773) 9388-18              */
/*he                E-mail: Info@ese-ecs.de                             */
/*he                Home page: www.ese-ecs.de                           */
/*he                                                                    */
/*he        product : ESE-Sourcen                                       */
/*he                                                                    */
/*he        module  : socketbase.cpp                                    */
/*he                                                                    */
/*he        author  : wiesttk                                           */
/*he                                                                    */
/*he        date    : 2000/07/24                                        */
/*he                                                                    */
/************************************************************************/
/************************************************************************/
/*te                                                                    */
/*te        module description : socketbase.cpp                         */
/*te                                                                    */
/************************************************************************/
/************************************************************************/
/*up                                                                    */
/*up        last modify        : socketbase.cpp                         */
/*up                                                                    */
/*up $NoKeywords                                                        */
/************************************************************************/
/***** Last check-in info *****

$Header: /IcarusBased/SpecFW/socketbase.cpp 2     6/27/06 4:32p Nashth $

(The remainder of the revision history is located at the bottom of this file)

*******************************/
/*in        include files      : socketbase.cpp                         */
#pragma warning(disable:4201) // Remove non-standard extention warnings in Microsoft headers
#pragma warning(disable:4100) // Remove unreferenced formal parameter warnings in Microsoft headers

#include "stdafx.h"

#if defined _WIN32 && !defined _WIN32_WCE
#include <stdio.h>
#include <winsock.h>
#endif
#if defined _WIN32_WCE
#include <winsock.h>
#endif

#include "socketbase.h"
#pragma warning(default:4201)
#pragma warning(default:4100)

/*de        defines            : socketbase.cpp                         */

/*if        import functions   : socketbase.cpp                         */

/*id        import data        : socketbase.cpp                         */

/*ef        export functions   : socketbase.cpp                         */

/*ed        export data        : socketbase.cpp                         */

/*sf        static functions   : socketbase.cpp                         */

/*sd        static data        : socketbase.cpp                         */

/*end                                                                   */

// socket base class
Socket::Socket ( int _debug  )
{
#if defined _WIN32 || defined _WIN32_WCE
WORD								wVersionRequested;
WSADATA							wsaData;

	log_errors = 0;
	log_infos = 0;
	debug = 0;
	error = 0;
	_debug = _debug; // Remove compiler warning

	/* init winsock DLL */
#if defined _WIN32 && !defined _WIN32_WCE
	wVersionRequested = MAKEWORD( 2, 0 );
#endif
#if defined _WIN32_WCE
	wVersionRequested = MAKEWORD( 1, 0 );
#endif
	WSAStartup( wVersionRequested, &wsaData );
	if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
	{
		/* Tell the user that we couldn't find a usable */
		/* WinSock DLL.                                  */
    error = -1;
		if ( debug & log_errors )
			syslog ("WSAStartup() failed: %s");
	}
	else
  {
#if defined _WIN32 && !defined _WIN32_WCE
    if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 )
#endif
#if defined _WIN32_WCE
    if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 0 )
#endif
    {
      /* Tell the user that we couldn't find a usable */
      /* WinSock DLL.                                  */
      WSACleanup( );
	    error = -1;
			if ( debug & log_errors )
				syslog ("Wrong winsock version: %s");
    }
    /* The WinSock DLL is acceptable. Proceed. */
  }
#else
	debug = _debug;
	error = 0;
#endif
	instance = 0;
}

Socket::Socket ( Socket *pSocket )
{
  // debug
	debug       = pSocket->debug;
	error       = pSocket->error;
	instance    = 1;
}

Socket::~Socket ( void )
{
	if ( instance == 0 )
	{
#if defined _WIN32 || defined _WIN32_WCE
		if ( WSACleanup() != 0 )
			syslog ( "WSACleanup failed: %s" );
#endif
	}
}

#if defined _WIN32_WCE || defined PSOS_OS || defined NET_OS
// not supported by pna, netos, windows ce
struct servent		*Socket::getservbyname ( const char *name, const char *proto )
{
struct servent		*ps;
int								found;

	found = FALSE;

	if ( !strcmp ( name, "ftp" ) && !strcmp ( proto, "tcp" ) )
	{
		found = TRUE;
		servEntry.s_name    = "ftp";
		servEntry.s_aliases = NULL;
		servEntry.s_port    = htons ( 21 );
		servEntry.s_proto   = "tcp";
	}

	if ( found == TRUE )
		ps = &servEntry;
	else
		ps = NULL;

return ps;
}
#endif

void	Socket::set_addr( struct sockaddr_in *_addr, u_short _port, u_long _inet )
{
int   i;

   _addr->sin_family				= AF_INET;
	 _addr->sin_port					= htons(_port);
	 _addr->sin_addr.s_addr	= htonl(_inet);
   for (i=0;i<8;i++)
	 	_addr->sin_zero[i] = 0;
}

//
// returns port in host byte order
//
u_short		Socket::atoport ( char *service, char *proto )
{
u_short 				_port = 0;
struct servent	*serv;
int						  errpos;

	/* First try to read it from /etc/services */
	serv = getservbyname ( service, proto );
	if (serv != NULL)
	{
		port = serv->s_port;
	}
	else
	{
		/* Not in services, maybe a number? */
    errpos = sscanf ( service, "%d", &_port );
		if ( errpos != 1 )
			return (u_short) -1; /* Invalid port address */
	}

return ntohs(_port);
}

//
// returns address in host byte order
//
unsigned long		Socket::atoaddr ( char *address )
{
struct hostent  *host;
struct in_addr  saddr;
unsigned long   addr;

  addr = 0;
  /* First try it as aaa.bbb.ccc.ddd. */
  saddr.s_addr = inet_addr(address);
  if (saddr.s_addr != (unsigned long)-1)
  {
    addr = saddr.s_addr;
  }
  else
  {
    host = gethostbyname(address);
    if (host != NULL)
    {
      addr = ((struct in_addr *)*host->h_addr_list)->s_addr;
    }
  }

return ntohl(addr);
}

int		Socket::createSocket ( int _reuse, int _sock_type )
{
int			reuse_addr = 1;

  sock_type = _sock_type;
	/* create a socket */
	if (( s = socket(AF_INET, sock_type, 0) ) == -1 )
	{
		if ( debug & log_errors )
  		syslog ( "socket() failed: %s" );
		s = INVALID_SOCKET;
	}
	else if ( (_reuse == Socket::reuse) && (setsockopt ( s, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr, sizeof(reuse_addr)) == -1) )
	{
		if ( debug & log_errors )
  		syslog ( "setsockopt(SO_REUSEADDR) failed %s" );
		s = closeSocket ( s, Socket::linger );
	}

return s;
}

int   Socket::closeSocket ( int _linger )
{
  s = closeSocket ( s, _linger );

return s;
}

int   Socket::closeSocket ( int _s, int _linger )
{
struct linger	linger;
unsigned long wopt;
#if !defined SD_RECEIVE
#define SD_RECEIVE   0x00
#endif

	linger.l_onoff  = 1;  // turn on linger
  linger.l_linger = 10; // wait 10 sec bevore closing socket
  if ( _s != INVALID_SOCKET )
  {
  	 /* set socket options (blocking) */
     wopt = 0;
#if defined _WIN32 || defined _WIN32_WCE || PSOS_OS
	  if ( ioctlsocket ( _s, FIONBIO, (u_long*)&wopt ) == -1 )
#elif defined NET_OS
		if ( setsockopt ( _s, SO_NONBLOCK, &wopt, sizeof(wopt) ) == -1 )
#endif
  	{
	  	if ( debug & log_errors )
        syslog ( "ioctl(FIONBIO) in socketClose() failed: %s" );
	  }
    if ( shutdown ( _s, SD_RECEIVE ) == -1 )
    {
#if defined _WIN32 || defined _WIN32_WCE
      if ( WSAGetLastError() != WSAENOTCONN )
#endif
      {
			  if ( debug & log_errors )
				  syslog ( "shutdown() failed: %s" );
      }
    }
    if ( (_linger == Socket::linger) && (setsockopt ( _s, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger) ) == -1) )
    {
      if ( debug & log_errors )
        syslog ( "setsockopt(SO_LINGER) failed: %s" );
    }
    if ( closesocket ( _s ) < 0 )
    {
			if ( debug & log_errors )
				syslog ( "close() failed: %s" );
    }
  }

return INVALID_SOCKET;
}

int		Socket::setBlocking ( void )
{
  s = setBlocking ( s );

return s;
}

int		Socket::setBlocking ( int _s )
{
int		wopt = 0;

	/* set socket options (non blocking) */
#if defined _WIN32 || defined _WIN32_WCE || PSOS_OS
	if ( ioctlsocket ( _s, FIONBIO, (u_long*)&wopt ) == -1 )
#elif defined NET_OS
		if ( setsockopt ( _s, SO_NONBLOCK, &wopt, sizeof(wopt) ) == -1 )
#endif
	{
		if ( debug & log_errors )
      syslog ( "ioctl(FIONBIO) failed: %s" );
		_s = closeSocket ( _s, Socket::linger );
	}

return _s;
}

int		Socket::setNonBlocking ( void )
{
  s = setNonBlocking ( s );

return s;
}

int		Socket::setNonBlocking ( int _s )
{
int		wopt = 1;

	/* set socket options (non blocking) */
#if defined _WIN32 || defined _WIN32_WCE || PSOS_OS
	if ( ioctlsocket ( _s, FIONBIO, (u_long*)&wopt ) == -1 )
#elif defined NET_OS
		if ( setsockopt ( _s, SO_NONBLOCK, &wopt, sizeof(wopt) ) == -1 )
#endif
	{
		if ( debug & log_errors )
      syslog ( "ioctl(FIONBIO) failed: %s" );
		_s = closeSocket ( _s, Socket::linger );
	}

return _s;
}

void  Socket::setInet ( u_long _inet )
{
  peer_inet = _inet;
}

void  Socket::setInet ( char *_inet )
{
	peer_inet = atoaddr ( _inet );
}

void  Socket::setPort ( u_short _port )
{
  port = _port;
}

void  Socket::setPeerAddr ( u_short _port )
{
	set_addr ( &peer_addr, _port, peer_inet );
}

void  Socket::setPeerAddr ( u_long _inet )
{
	set_addr ( &peer_addr, port, _inet );
}

void  Socket::setPeerAddr ( u_long _inet, u_short _port )
{
	set_addr ( &peer_addr, _port, _inet );
}

void  Socket::setPeerAddr ( char *_inet, u_short _port )
{
	peer_inet = atoaddr ( _inet );
	set_addr ( &peer_addr, _port, peer_inet );
}

void  Socket::setPeerAddr ( void )
{
	set_addr ( &peer_addr, port, peer_inet );
}

int		Socket::bindSocket ( void )
{
  s = bindSocket ( s, port );

return s;
}

int		Socket::bindSocket ( int _s )
{
  _s = bindSocket ( _s, port );

return _s;
}

int		Socket::bindSocket ( int _s, int _port )
{
struct sockaddr_in  my_addr;

	/* bind the socket to my address */
	set_addr ( &my_addr, (u_short)_port, INADDR_ANY );
	if ( bind ( _s, (struct sockaddr*)&my_addr, sizeof(my_addr) ) == -1 )
	{
		if ( debug & log_errors )
			syslog ( "bind() failed: %s" );
		_s = closeSocket ( _s, Socket::linger );
	}

return _s;
}

int		Socket::connectSocket ( struct timeval *_timeout )
{
  s = connectSocket ( s, _timeout );

return s;
}

int		Socket::connectSocket ( int _s, struct timeval *_timeout )
{
return connectSocket ( _s, _timeout, port, peer_inet );
}

int		Socket::connectSocket ( int _s, struct timeval *_timeout, int _port, unsigned long _inet )
{
	/* connect the socket to an address */
	set_addr ( &peer_addr, (u_short)_port, _inet );
  if ( _timeout == NULL )
  {
	  if ( connect ( _s, (struct sockaddr*)&peer_addr, sizeof(peer_addr) ) == -1 )
	  {
		  if ( debug & log_errors )
			  syslog ( "connect() failed: %s" );
		  _s = closeSocket ( _s, Socket::linger );
	  }
  }
  else
  {
  struct timeval	wait;
  fd_set          writefds;
  int             rs;

    // set to nonblocking
    setNonBlocking(_s);
	  connect ( _s, (struct sockaddr*)&peer_addr, sizeof(peer_addr) );
    setBlocking(_s);
    // set select sockets
	  FD_ZERO ( &writefds );
	  FD_SET ( _s, &writefds );
	  /* handle incoming messages */
	  wait = *_timeout;
	  rs = select ( FD_SETSIZE, (fd_set*)0, &writefds, (fd_set*)0, &wait );
	  if ( rs < 0 )
	  {
      if ( debug & log_errors )
  		  syslog ( "select() failed: %s" );
		  _s = closeSocket ( _s, Socket::linger );
	  }
	  else if ( rs == 0 )
  	{
      if ( debug & log_errors )
  		  syslog ( "select() timeout: %s" );
		  _s = closeSocket ( _s, Socket::linger );
	  }
	  else
	  {
		  /* check new connection */
		  if ( !FD_ISSET ( _s, &writefds ) )
  	  {
        if ( debug & log_errors )
  		    syslog ( "select() no data for me: %s" );
		    _s = closeSocket ( _s, Socket::linger );
	    }
		}
	}

return _s;
}

int		Socket::acceptConnection ( int &_s, fd_set *_socks, struct timeval *_timeout, struct sockaddr_in *_addr )
{
int							    addrlen;
int                 readsocks;
int                 newSock;
struct timeval	    timeout;

  // check data socket
  FD_ZERO( _socks );
  FD_SET( _s, _socks );
  timeout = *_timeout;

  readsocks = select( FD_SETSIZE, _socks, (fd_set *)0, (fd_set *)0, &timeout);
  if (readsocks < 0)
  {
    if ( getDebug() & log_errors )
  	  syslog ( "select(data_socks) failed: %s" );
    newSock = _s = closeSocket ( _s, Socket::linger );
  }
  else if ( readsocks == 0 )
  {
    // retry
	  if ( getDebug() & log_errors )
		  syslog ( "select(data_sock) timeout: %s" );
    newSock = _s = closeSocket ( _s, Socket::linger );
  }
	else
  {
    addrlen = sizeof(struct sockaddr_in);
    newSock = accept(_s, (struct sockaddr *)_addr, &addrlen);
    if ( newSock < 0 )
    {
			if ( getDebug() & log_errors )
				syslog ( "accept() failed: %s" );
	    newSock = closeSocket ( newSock, Socket::linger );
    }
    else
    {
			if ( getDebug() & log_infos )
				syslog ( "accept() from: %d.%d.%d.%d,%d ...\r\n", (ntohl(_addr->sin_addr.s_addr)>>24)&0xff, (ntohl(_addr->sin_addr.s_addr)>>16)&0xff, (ntohl(_addr->sin_addr.s_addr)>>8)&0xff, ntohl(_addr->sin_addr.s_addr)&0xff, ntohs(_addr->sin_port) );
    }
  }

return newSock;
}

int		Socket::receiveData ( struct timeval *_timeout )
{
return receiveData ( s, sizeof(buffer), _timeout );
}

int		Socket::receiveData ( int _s, int len, struct timeval *_timeout )
{
return receiveData ( _s, buffer, len, _timeout );
}

int		Socket::receiveData ( int _s, void *_buffer, int len, struct timeval *_timeout )
{
/* number of bytes read */
int				n = 0, peer_len;
u_long		haddr;

  if ( _timeout == NULL )
  {
    if ( sock_type == SOCK_DGRAM )
    {
      peer_len = sizeof(peer_addr);
      if ( (n = recvfrom ( _s, (char*)_buffer, len, 0, (sockaddr *)&peer_addr, &peer_len ) ) == -1)
      {
        if ( debug & log_errors )
          syslog ( "recvfrom() failed: %s" );
      }
      else
      {
        if ( n == 0 )
        {
          /* closed connection */
          if ( debug & log_errors )
            syslog ( "Connection closed." );
        }
        else
        {
          ((char*)_buffer)[n] = 0x00;
          if ( debug & log_infos )
          {
            haddr = ntohl ( peer_addr.sin_addr.s_addr );
            printf ( "%s (%d.%d.%d.%d)\n", _buffer,
              (haddr&0xff000000) >> 24,
              (haddr&0x00ff0000) >> 16,
              (haddr&0x0000ff00) >> 8,
              (haddr&0x000000ff) );
          }
        }
			}
    }
    else
    {
      if ( (n = recv ( _s, (char*)_buffer, len, 0 ) ) == -1)
      {
        if ( debug & log_errors )
          syslog ( "recv() failed: %s" );
      }
      else
      {
        if ( n == 0 )
        {
          /* closed connection */
          if ( debug & log_errors )
            syslog ( "Connection closed." );
        }
        else
        {
          ((char *)_buffer)[n] = 0x00;
          if ( debug & log_infos )
          {
            haddr = ntohl ( peer_addr.sin_addr.s_addr );
            syslog ( "%s (%d.%d.%d.%d)\n", _buffer,
              (haddr&0xff000000) >> 24,
              (haddr&0x00ff0000) >> 16,
              (haddr&0x0000ff00) >> 8,
              (haddr&0x000000ff) );
          }
        }
      }
    }
  }
  else
  {
  struct timeval	wait;
  fd_set          readfds;
  int             rs;

    // set to nonblocking
    // setNonBlocking();
    // set select sockets
	  FD_ZERO ( &readfds );
	  FD_SET ( _s, &readfds );
	  /* handle incoming messages */
	  wait = *_timeout;
	  rs = select ( FD_SETSIZE, &readfds, (fd_set*)0, (fd_set*)0, &wait );
	  if ( rs < 0 )
	  {
      if ( debug & log_errors )
  		  syslog ( "select() failed: %s" );
	  }
	  else if ( rs == 0 )
  	{
      if ( debug & log_errors )
  		  syslog ( "select() timeout: %s" );
	  }
	  else
	  {
		  /* check new data */
		  if ( !FD_ISSET ( _s, &readfds ) )
  	  {
        if ( debug & log_errors )
  		    syslog ( "select() no data for me: %s" );
	    }
      else
      {
        if ( sock_type == SOCK_DGRAM )
        {
          peer_len = sizeof(peer_addr);
          n = recvfrom ( _s, (char*)_buffer, len, 0, (sockaddr *)&peer_addr, &peer_len );
          ((char*)_buffer)[n] = 0x00;
          if ( debug & log_infos )
          {
            haddr = ntohl ( peer_addr.sin_addr.s_addr );
            printf ( "%s (%d.%d.%d.%d)\n", _buffer,
              (haddr&0xff000000) >> 24,
              (haddr&0x00ff0000) >> 16,
              (haddr&0x0000ff00) >> 8,
              (haddr&0x000000ff) );
          }
        }        else
        {
          n = recv ( _s, (char*)_buffer, len, 0 );
          ((char*)_buffer)[n] = 0x00;
          if ( debug & log_infos )
          {
            haddr = ntohl ( peer_addr.sin_addr.s_addr );
            syslog ( "%s (%d.%d.%d.%d)\n", _buffer,
              (haddr&0xff000000) >> 24,
              (haddr&0x00ff0000) >> 16,
              (haddr&0x0000ff00) >> 8,
              (haddr&0x000000ff) );
          }
        }
      }
		}
  }

return n;
}

int		Socket::sendData ( void )
{
return sendData ( s, strlen(buffer)+1 );
}

int		Socket::sendData ( int len )
{
return sendData ( s, buffer, len );
}

int		Socket::sendData ( char *_buffer )
{
return sendData ( s, _buffer, strlen(_buffer)+1 );
}

int		Socket::sendData ( char *_buffer, int len )
{
return sendData ( s, _buffer, len );
}

int		Socket::sendData ( int _s, int len )
{
return sendData ( _s, buffer, len );
}

int		Socket::sendData ( int _s, void *_buffer, int len )
{
/* number of bytes sent */
int				n;

  if ( sock_type == SOCK_DGRAM )
  {
    if ( (n = sendto ( _s, (char*)_buffer, len, 0, (sockaddr*)&peer_addr, sizeof(peer_addr) ) ) == -1)
    {
      if ( debug & log_errors )
        syslog ( "sendto() failed: %s" );
    }
    else
    {
      if ( debug & log_infos )
      {
        syslog ( "%s\n", _buffer );
      }
    }
  }
  else
  {
    if ( (n = send ( _s, (char*)_buffer, len, 0 ) ) == -1)
    {
      if ( debug & log_errors )
        syslog ( "send() failed: %s" );
    }
    else
    {
      if ( debug & log_infos )
      {
        syslog ( "%s\n", _buffer );
      }
    }
  }

return n;
}
/***** Revision History *****

$Log: /IcarusBased/SpecFW/socketbase.cpp $
 * 
 * 2     6/27/06 4:32p Nashth
 * Removed compiler warnings.
  
$NoKeywords: $

***** End of Rev History ****/
