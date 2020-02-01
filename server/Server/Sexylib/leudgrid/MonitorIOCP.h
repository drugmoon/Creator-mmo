#pragma once
namespace LEUD
{
	class MonitorIOCP{
	private:
		HANDLE	m_IOCP;
		HANDLE* m_Threads;
		DWORD	m_ThreadNum;
	public:
		MonitorIOCP();
		BOOL	Start( DWORD NumberOfThreads,DWORD NumberOfConcurrentThreads=0 );
		BOOL	Stop( );
		BOOL	AddHandle( HANDLE Handle, DWORD Key );
		void    PostIOCPStatus(DWORD dwRead,ULONG_PTR key,LPOVERLAPPED lpov);
	private:
		static 	unsigned int __stdcall MonitorIOCP_WorkerThread( void* data );
	};
}