
#ifndef _CTcpServer_h_
#define _CTcpServer_h_

#include "socketbase.h"

// Used as a flag for receiveData
#define BLOCKED_READ_MASK	0x80000000
#define READ_SIZE_MASK		0x7fffffff

#ifdef __cplusplus

// non blocking tcp server
class CTcpServer : public Socket
{
public:
	CTcpServer::CTcpServer ( int _port, int _maxConnections, int _debug );
	CTcpServer::~CTcpServer ( void );

	void	stopServer ( void );
	int		startServer ( int _reuse=Socket::noreuse );
	int		receiveData ( char *buffer, int size, int &index, bool &bSocketState );
	int		sendData ( int index, char *buffer, int size );

protected:
	void	buildSelectList ( int *hs );
	void	newConnection ( void );

private:
	int			debug;
	int			port;
	int			maxConnections;
	int			*connectList;
	int			socket;
	struct 		sockaddr_in	peer_addr;
	int			peer_len;
    fd_set  	readfds;
};

#endif

#endif /* _CTcpServer_h_ */
