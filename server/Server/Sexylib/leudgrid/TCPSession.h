#pragma once
namespace LEUD
{
	class TCPSession{
	protected:
		SOCKET m_Socket;
	public:
		TCPSession():m_Socket(INVALID_SOCKET){
		}
		virtual ~TCPSession(){
		}
	public:
		operator SOCKET( ){
			return m_Socket;
		}
		virtual HANDLE GetHandle(){
			return reinterpret_cast<HANDLE>(m_Socket);
		}
	public:
		virtual void Open( SOCKET sock );
		virtual void Shutdown();
	public:
		virtual void Close();
	public:
		static BOOL GetPeerName( SOCKET sock,int& ip,short& port );
		static BOOL GetLocalName( SOCKET sock,int& ip,short& port );
		static void GetPeerIP( SOCKET sock,char* ret );
	};
	class	EventQueue;
	class	TCPSessionManager;
	class TCPSessionManageEQ : public TCPSession
	{
	protected:
		EventQueue* m_EQ;
		TCPSessionManager* m_SessionManager;
	public:
		void setEQ( EventQueue* EQ );
		void setManager( TCPSessionManager* manger);
		virtual void NetBreak(){};
		virtual void flush(){};
		virtual void flushstream(){};
	};
}