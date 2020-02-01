#include "stdafx.h"
#include "leudgrid/TCPAcceptEventQueue.h"
#include "leudgrid/EventBase.h"
#include "leudgrid/TCPAccept.h"
#include "leudgrid/EventQueue.h"
#include "leudgrid/Application.h"
#include "leudgrid/MemoryPool.h"

namespace LEUD
{
	class ClientConnectEvent : public LEUD::EventBase,public LEUD::PoolMemoryBase< ClientConnectEvent >
	{
		SOCKET m_sock;
	public:
		ClientConnectEvent(SOCKET sock):m_sock(sock){
		}
		virtual void OnHanding(unsigned int queue_id,void* param){
			TCPAcceptEventQueue* aeq = (TCPAcceptEventQueue*)param;
			aeq->OnAccepted( m_sock );
			delete this;
		}
	};
	class TCPAcceptEventQueueAccepter : public LEUD::TCPAcceptThread
	{
		LEUD::EventQueue* m_EQ;
	public:
		TCPAcceptEventQueueAccepter(LEUD::EventQueue* eq):m_EQ(eq){
		}
		virtual void DoAccept( SOCKET sock )
		{
			m_EQ->PushEvent(new ClientConnectEvent(sock));
		};
	};

	TCPAcceptEventQueue::TCPAcceptEventQueue()
	{
		m_EQ = 0;
		m_Accepter = 0;
		m_app = 0;
	}
	TCPAcceptEventQueue::~TCPAcceptEventQueue()
	{
	}
	BOOL TCPAcceptEventQueue::Start(Application* app)
	{
		m_app = app;
		m_EQ = new LEUD::EventQueue;
		m_app->AddEventHandle(m_EQ->m_Event);
		m_Accepter = new TCPAcceptEventQueueAccepter(m_EQ);
		unsigned short iListenPort = htons(atoi(m_Listen_port.c_str()));
		unsigned int   iListenIP   = inet_addr(m_Listen_ip.c_str());
		int			   iBacklog	   = atoi(m_Backlog.c_str());
		return m_Accepter->Start(iBacklog,iListenIP,iListenPort);
	}
	BOOL TCPAcceptEventQueue::Stop()
	{
		BOOL ret=true;
		if( m_Accepter )ret = m_Accepter->Stop();
		if( m_EQ )
		{
			while( m_EQ->EventSize() > 0 ){
				m_app->OnUpdate();
				Sleep(100);
			}
		}
		delete m_Accepter;
		m_Accepter = 0;
		delete m_EQ;
		m_EQ = 0;
		m_app = 0;
		return ret;
	}
	size_t TCPAcceptEventQueue::Update()
	{
		if( m_EQ ) return m_EQ->HandEvent(this);
		return 0;
	}
}
LEUD::PoolMemory<LEUD::ClientConnectEvent>	LEUD::PoolMemoryBase<LEUD::ClientConnectEvent>::memPool("ClientConnectEvent");
