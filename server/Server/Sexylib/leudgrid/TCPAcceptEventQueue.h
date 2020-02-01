#pragma once

namespace LEUD
{
	class Application;
	class TCPAcceptEventQueueAccepter;
	class EventQueue;
	class TCPAcceptEventQueue
	{
	protected:
		LEUD::EventQueue* m_EQ;
		TCPAcceptEventQueueAccepter* m_Accepter;
	protected:
		Application* m_app;
		std::string m_Listen_ip;
		std::string	m_Listen_port;
		std::string m_Backlog;
	public:
		TCPAcceptEventQueue();
		virtual ~TCPAcceptEventQueue();
		virtual BOOL Start(Application* app);
		virtual BOOL Stop();
		virtual size_t Update();
	public:
		virtual void OnAccepted( SOCKET sock )=0;
	};

}