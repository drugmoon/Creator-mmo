#pragma once
#include "StreamBlockPRD.h"
namespace LEUD
{
	class EventQueue;
	class MonitorIOCP;
	class Application;
	class TCPConnectEventQueueConnecter;
	class TCPConnectEventQueueConnectEvent;
	class TCPConnectEventQueue
	{
		friend TCPConnectEventQueueConnecter;
		friend TCPConnectEventQueueConnectEvent;
	private:
		EventQueue* m_EQ;
		TCPConnectEventQueueConnecter* m_Connecter;
	protected:
		Application* m_app;
	public:
		TCPConnectEventQueue();
		virtual ~TCPConnectEventQueue();
		virtual void Start(Application* app);
		virtual void Stop();
		virtual void Update();
		virtual void Connect(const char* ip,unsigned short port, unsigned short localport);
		virtual void Connect(const char* ip,unsigned short port);
	public:
		virtual void OnConnect(SOCKET sock)=0;
		virtual void OnFail(DWORD Error)=0;
	};

}