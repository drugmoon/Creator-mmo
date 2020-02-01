#include "stdafx.h"
#include "leudgrid/TCPSessionManager.h"
#include "leudgrid/MonitorIOCP.h"
#include "leudgrid/EventQueue.h"
#include "leudgrid/Application.h"
#include "leudgrid/TCPSession.h"

namespace LEUD
{
	void* TCPSessionManager::FindSession( SOCKET sock ){
		SessionMap::iterator pos = m_Sessions.find( sock );
		if( pos != m_Sessions.end() ){
			return pos->second;
		}
		return 0;
	}
	void  TCPSessionManager::AddMonitorSession( SOCKET sock,TCPSessionManageEQ* session ){
		m_Monitor->AddHandle((HANDLE)sock,0);
		AddSession(sock,session);
	}
	void	TCPSessionManager::AddSession( SOCKET sock,TCPSessionManageEQ* session ){
		m_Sessions[sock] = session;
		session->setManager(this);
		session->setEQ(this->m_EQ);
		session->Open(sock);
	}
	void	TCPSessionManager::RemoveSession( SOCKET sock ){
		m_Sessions.erase(m_Sessions.find(sock));
	}
	size_t TCPSessionManager::GetSessionNum()
	{
		return m_Sessions.size();
	}

	TCPSessionManager::TCPSessionManager()
	{
		m_EQ=0;
		m_app=0;
		m_Monitor=0;
		m_WorkThreadNum = 3;
	}
	TCPSessionManager::~TCPSessionManager()
	{
	}
	void TCPSessionManager::Start(LEUD::Application* app)
	{
		m_app = app;
		m_EQ = new EventQueue;
		m_app->AddEventHandle(m_EQ->m_Event);
		m_Monitor = new MonitorIOCP;
		m_Monitor->Start(m_WorkThreadNum);
	}
	void TCPSessionManager::Stop()
	{
		for( SessionMap::iterator pos = m_Sessions.begin();
			pos != m_Sessions.end();
			++ pos )
		{
			TCPSession* session = (TCPSession*)pos->second;
			session->Shutdown();
		}
		if( m_EQ )
		{
			while( m_EQ->EventSize() > 0 || m_Sessions.size() > 0 ){
				m_app->OnUpdate();
				Sleep(100);
			}
		}
		if( m_Monitor )m_Monitor->Stop();
		delete m_Monitor;
		m_Monitor = 0;
		delete m_EQ;
		m_EQ = 0;
		m_app = 0;
	}
	size_t TCPSessionManager::Update()
	{
		if( m_EQ ) return m_EQ->HandEvent(this);
		return 0;
	}
	LEUD::EventQueue* TCPSessionManager::GetEQ()
	{
		return m_EQ;
	}
}