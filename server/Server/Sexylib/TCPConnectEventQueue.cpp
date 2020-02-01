#include "stdafx.h"
#include "leudgrid/TCPConnectEventQueue.h"
#include "leudgrid/EventBase.h"
#include "leudgrid/EventQueue.h"
#include "leudgrid/TCPConnect.h"
#include "leudgrid/Application.h"
#include "leudgrid/MonitorIOCP.h"
#include "leudgrid/StreamBlock.h"
#include "leudgrid/StreamBlock.inl"

namespace LEUD
{
	class TCPConnectEventQueueConnectEvent : public LEUD::EventBase
	{
		SOCKET m_sock;
	public:
		TCPConnectEventQueueConnectEvent(SOCKET sock){
			m_sock = sock;
		}
		virtual void OnHanding(unsigned int queue_id,void* param){
			TCPConnectEventQueue* ceq = (TCPConnectEventQueue*)param;
			ceq->OnConnect(m_sock);
			delete this;
		}
	};
	class TCPConnectEventQueueFailEvent : public LEUD::EventBase
	{
		DWORD mError;
	public:
		TCPConnectEventQueueFailEvent(DWORD Error){
			mError = Error;
		}
		virtual void OnHanding(unsigned int queue_id,void* param){
			TCPConnectEventQueue* ceq = (TCPConnectEventQueue*)param;
			ceq->OnFail(mError);
			delete this;
		}
	};
	class TCPConnectEventQueueConnecter : public LEUD::TCPConnect
	{
		LEUD::EventQueue* m_EQ;
	public:
		TCPConnectEventQueueConnecter( LEUD::EventQueue* eq ){
			m_EQ = eq;
		}
		virtual void DoConnect( SOCKET sock ){
			m_EQ->PushEvent( new TCPConnectEventQueueConnectEvent(sock) );
		}
		virtual void DoConnectFail( DWORD Error ){
			m_EQ->PushEvent( new TCPConnectEventQueueFailEvent(Error) );
		}
	};
	TCPConnectEventQueue::TCPConnectEventQueue()
	{
		m_EQ=0;
		m_Connecter=0;
		m_app=0;
	}
	TCPConnectEventQueue::~TCPConnectEventQueue()
	{
	}
	void TCPConnectEventQueue::Start(Application* app)
	{
		m_app = app;
		m_EQ = new EventQueue;
		m_app->AddEventHandle(m_EQ->m_Event);
		m_Connecter = new TCPConnectEventQueueConnecter(m_EQ);
		m_Connecter->Start();
	}
	void TCPConnectEventQueue::Stop()
	{
		m_Connecter->Stop();
		m_app->RemoveEventHandle(m_EQ->m_Event);
		while( m_EQ->EventSize() > 0 ){
			m_app->OnUpdate();
			Sleep(100);
		}
		delete m_Connecter;m_Connecter = 0;
		delete m_EQ;m_EQ = 0;
		m_app = 0;
	}
	void TCPConnectEventQueue::Connect(const char* ip,unsigned short port, unsigned short localport)
	{
		unsigned int Connect_ip = inet_addr(ip);
		unsigned short Connect_port = htons(port);
		unsigned short local_port = htons(localport);
		m_Connecter->Connect(Connect_ip,Connect_port,local_port);
	}
	void TCPConnectEventQueue::Connect(const char* ip,unsigned short port)
	{
		unsigned int Connect_ip = inet_addr(ip);
		unsigned short Connect_port = htons(port);
		m_Connecter->Connect(Connect_ip,Connect_port,0);
	}
	void TCPConnectEventQueue::Update()
	{
		if( m_EQ )m_EQ->HandEvent(this);
	}
}
