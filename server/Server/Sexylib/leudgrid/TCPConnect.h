#pragma once

namespace LEUD
{
	class TCPConnect{
	protected:
		unsigned long m_Address;
		unsigned short m_Port;
		unsigned short m_LocalPort;
		DWORD		m_Sleep;
		HANDLE		m_ThreadHandle;
		HANDLE		m_StopEvent;
		HANDLE		m_WakeEvent;
		bool		m_Started;
	public:
		TCPConnect();
		TCPConnect( unsigned long address, unsigned short port );
		TCPConnect( unsigned long address, unsigned short port, unsigned short localport );
	public:
		void Connect( );
		void Connect( unsigned long address, unsigned short port );
		void Connect( unsigned long address, unsigned short port, unsigned short localport );
		void Start();
		void Stop();
	public:
		virtual void DoConnect( SOCKET sock )=0;
		virtual void DoConnectFail( DWORD Error )=0;
	private:
		static 	unsigned int __stdcall TCPConnect_ConnectThread( void* data );
	};
}