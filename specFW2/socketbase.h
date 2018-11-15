
#ifndef _socketbase_h_
#define _socketbase_h_

/*
#if defined _WIN32 && !defined _WIN32_WCE
#include <stdio.h>
#include <winsock.h>
#endif
*/

#if defined _WIN32_WCE
#include <winsock.h>
#endif

#include "stdafx.h"


#if defined _WIN32_WCE
  #if defined SOCKETS_DLL_EXPORTS
    #define SOCKETS_DLL_API __declspec(dllexport)
  #else
    #define SOCKETS_DLL_API //__declspec(dllimport)
  #endif
#else
  #define SOCKETS_DLL_API
#endif


#ifdef __cplusplus

// socket base class
class SOCKETS_DLL_API Socket : public CWinThread
{
public:
	Socket ( int _debug );
	Socket ( Socket *psocket );
	~Socket ( void );

void syslog ( const char *, ... ) { };// debug here,,,
	int		createSocket ( int _reuse=Socket::noreuse, int _sock_type=SOCK_STREAM );
  int   getSocket ( void ) { return s; }
	int   closeSocket ( int _linger=Socket::nolinger );
	int   closeSocket ( int _s, int _linger=Socket::nolinger );
	int		setBlocking ( void );
	int		setBlocking ( int _s );
	int		setNonBlocking ( void );
	int		setNonBlocking ( int _s );
  int   connectSocket ( struct timeval *_timeout=NULL );
  int   connectSocket ( int _s, struct timeval *_timeout=NULL );
  int		connectSocket ( int _s, struct timeval *_timeout, int _port, unsigned long _inet );
  int		bindSocket ( void );
  int		bindSocket ( int _s );
  int		bindSocket ( int _s, int _port );
  int		acceptConnection ( int &_s, fd_set *_socks, struct timeval *_timeout, struct sockaddr_in *_addr );
  int   receiveData ( struct timeval *_timeout=NULL );
  int   receiveData ( int _s, int len, struct timeval *_timeout=NULL );
  int   receiveData ( int _s, void *_buffer, int len, struct timeval *_timeout=NULL );
  int   sendData ( void );
  int   sendData ( int len );
  int   sendData ( char *_buffer );
  int   sendData ( char *_buffer, int len );
  int   sendData ( int _s, int len );
  int   sendData ( int _s, void *_buffer, int len );
  char  *getBuffer ( void ) { return buffer; }
  int   getDebug ( void ) { return debug; }
  int   getError ( void ) { return error; }
  void  setError ( int _error ) { error = _error; }
  void  setPort ( u_short _port );
  void  setInet ( u_long _inet );
  void  setInet ( char *_inet );
  void  setPeerAddr ( u_short _port );
  void  setPeerAddr ( u_long _inet );
  void  setPeerAddr ( u_long _inet, u_short _port );
  void  setPeerAddr ( char *_inet, u_short _port );
  void  setPeerAddr ( void );

  enum
  {
    nolinger = 0,
    linger   = 1,
    noreuse  = 0,
    reuse    = 1
  };

	int			            debug;
	int log_infos;
	int                  error;
	int									instance;
	int log_errors;
	int log_data;

protected:
	void         	set_addr( struct sockaddr_in *addr, u_short _port, u_long _inet );
	u_short      	atoport ( char *service, char *proto );
	u_long	     	atoaddr ( char *address );
#if defined _WIN32_WCE || defined PSOS_OS
	struct servent	*getservbyname ( const char *name, const char *proto );
#endif

private:

  struct sockaddr_in  peer_addr;
  struct sockaddr_in  my_addr;
  u_short             port;
  u_long              peer_inet;
  int                 sock_type;
  int                 s;
  char                buffer[2048];
#if defined _WIN32_WCE || defined PSOS_OS
	struct servent			 servEntry;
#endif
};

#endif

#endif /* _socketbase_h_ */
