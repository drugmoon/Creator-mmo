#pragma once
namespace LEUD
{
	class EventQueue;
	class Application;
	class MonitorIOCP;
	class TCPSessionManageEQ;
	class TCPSessionManager
	{
	protected:
		EventQueue* m_EQ;
		Application* m_app;
		MonitorIOCP* m_Monitor;
		typedef std::map< SOCKET,TCPSessionManageEQ* > SessionMap;
		SessionMap m_Sessions;
		int m_WorkThreadNum;
	public:
		void* FindSession( SOCKET sock );
		virtual void  AddSession( SOCKET sock,TCPSessionManageEQ* session );
		void  AddMonitorSession( SOCKET sock,TCPSessionManageEQ* session );
		virtual void  RemoveSession( SOCKET sock );
		size_t GetSessionNum();
		MonitorIOCP* GetMonitor(){return m_Monitor;}
	public:
		TCPSessionManager();
		virtual ~TCPSessionManager();
		virtual void Start(Application* app);
		virtual void Stop();
		virtual size_t Update();
	public:
		LEUD::EventQueue* GetEQ();
	};

}