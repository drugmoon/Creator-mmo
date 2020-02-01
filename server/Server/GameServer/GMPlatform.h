#pragma once

#include "leudgrid/TCPAcceptEventQueue.h"
#include "leudgrid/TCPSessionMini.h"

class CGameServerApp;

class GMAcceptor :
	public LEUD::TCPAcceptEventQueue
{
	int mAcceptor_num;
	std::string mMD5Key;
	std::vector<std::string> mIPs;
public:
	bool mbOnService;
	GMAcceptor(void);
	virtual void OnAccepted( SOCKET sock );
public:
	bool LoadAllowIP();
	virtual BOOL Start(LEUD::Application* app);
};
class GMNetSession :
	public LEUD::TCPSessionMini,
	public LEUD::PoolMemoryBase< GMNetSession >
{
public:
	CGameServerApp*	m_app;
	std::string mMD5Key;
	GMNetSession(CGameServerApp* app);
	~GMNetSession(void);
	virtual void OnClose( DWORD Error );
	virtual void OnMessage( const char* msg,size_t size );
	virtual size_t BreakMessage( Stream_t* stream );
	virtual size_t BreakMessage( LEUD::StreamFix& stream );
	virtual void EncodeMessage( char* buf,size_t size ){}
	virtual void DecodeMessage( char* buf,size_t size ){}
	virtual void MakeHead( char* buf,unsigned short size ){}
};