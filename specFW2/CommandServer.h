//===========================================================================
//
//	Module Name:	CommandServer.h
//
// 	Function:		TcpIp interface header file.
//
//	Author:			J. Orlando
//
//	Copyright (c) 2018,  PerkinElmer, LAS. All rights reserved.
//
//===========================================================================


#include "TcpServer.h"
#include "ParserThread.h"

class CCommandServer : public CTcpServer
{
	DECLARE_DYNCREATE( CCommandServer )
protected:
public:
	virtual	~CCommandServer();
	CCommandServer();
	int Run();

static void	ServerThread( void * param );

// Attributes
	HANDLE	threadHANDLE;
// Operations
	void	CreateServerThread ( void );
	void	ExitServerThread ( void );
	void OnReplyToCommand(WPARAM wParam, LPARAM lParam);
	
	static UINT tm_CCommandServerReplyToCommand;

protected:
	CParserThread *m_pParserThread; 
public:
	//{{AFX_MSG(CCommandServer)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg UINT CommandParsingDone(char *reply, long lBytes);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	int m_SlotIndex;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandServer)
	virtual BOOL CCommandServer::InitInstance( void );
	virtual int CCommandServer::ExitInstance( void );
	//}}AFX_VIRTUAL


};

/*** Revision History ***/


