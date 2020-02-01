#include "stdafx.h"
#include "leudgrid/MonitorIOCP.h"
#include "leudgrid/ActionBase.h"
#include "leudgrid/Performance.h"

namespace LEUD
{
	MonitorIOCP::MonitorIOCP() : m_IOCP(INVALID_HANDLE_VALUE),m_Threads(0),m_ThreadNum(0){
	}
	BOOL	MonitorIOCP::Start( DWORD NumberOfThreads,DWORD NumberOfConcurrentThreads ){
		if( INVALID_HANDLE_VALUE == m_IOCP )
		{
			m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, NumberOfConcurrentThreads);
			m_ThreadNum = NumberOfThreads;
			m_Threads = new HANDLE[m_ThreadNum];
			//SYSTEM_INFO  sysinfo;
			//GetSystemInfo(&sysinfo);
			//m_ThreadNum = sysinfo.dwNumberOfProcessors * 2 + 2;
			UINT   ThreadID;
			for (DWORD i = 0; i < m_ThreadNum; i++)
			{
				m_Threads[i] = (HANDLE)_beginthreadex(NULL, 0, MonitorIOCP_WorkerThread, this, 0, &ThreadID );
#ifdef _PERFORMANCE
				printf("%5d MonitorIOCP_WorkerThread\n",ThreadID);
#endif
			}
			return TRUE;
		}
		return FALSE;
	}
	void    MonitorIOCP::PostIOCPStatus(DWORD dwRead,ULONG_PTR key,LPOVERLAPPED lpov)
	{
		PostQueuedCompletionStatus(m_IOCP, dwRead, key, lpov);
	}
	BOOL	MonitorIOCP::Stop( ){
		if( INVALID_HANDLE_VALUE != m_IOCP )
		{
			for (DWORD i = 0; i < m_ThreadNum; i++) 
			{
				PostQueuedCompletionStatus(m_IOCP, 0, NULL, NULL);
			}
			WaitForMultipleObjects(m_ThreadNum, m_Threads, TRUE, INFINITE);
			m_ThreadNum = 0;
			delete[] m_Threads;
			m_Threads = NULL;

			BOOL    bResult;
			DWORD   dwNumRead;
			DWORD	dwKey;
			LPOVERLAPPED lpOverlapped;
			for(;;)
			{
				bResult = GetQueuedCompletionStatus(m_IOCP,&dwNumRead,&dwKey,&lpOverlapped,0);
				if( lpOverlapped ){
					if( bResult ){
						((ActionBase*)lpOverlapped)->OnComplete( dwNumRead,dwKey );
					}else{
						((ActionBase*)lpOverlapped)->OnFaile( GetLastError() );
					}
				}
				if( bResult == FALSE ) break;
			}
			CloseHandle( m_IOCP );
			m_IOCP = INVALID_HANDLE_VALUE;
			return TRUE;
		}
		return FALSE;
	}
	BOOL	MonitorIOCP::AddHandle( HANDLE Handle, DWORD Key ){
		if( INVALID_HANDLE_VALUE != m_IOCP )
		{
			CreateIoCompletionPort( Handle, m_IOCP,Key, 0);
			return TRUE;
		}
		return FALSE;
	}
	unsigned int __stdcall MonitorIOCP::MonitorIOCP_WorkerThread( void* data ){
#ifdef _PERFORMANCE
		PerfSigleNode pn;
		pn.start("MonitorIOCP_WorkerThread");
#endif
		MonitorIOCP* Monitor = (MonitorIOCP*)data;
		HANDLE  hIOCP = Monitor->m_IOCP;
		BOOL    bResult;
		DWORD   dwNumRead;
		DWORD	dwKey;
		LPOVERLAPPED lpOverlapped;
		for(;;)
		{
			bResult = GetQueuedCompletionStatus(hIOCP,&dwNumRead,&dwKey,&lpOverlapped,INFINITE);
#ifdef _PERFORMANCE
			pn.Call();
#endif
			if( lpOverlapped ){
				if( bResult ){
					__try{
						((ActionBase*)lpOverlapped)->OnComplete( dwNumRead,dwKey );
					}__except(UnhandledExceptionFilter(GetExceptionInformation())){
					}
				}else{
					__try{
						((ActionBase*)lpOverlapped)->OnFaile( GetLastError() );
					}__except(UnhandledExceptionFilter(GetExceptionInformation())){
					}
				}
			}else{
				if( bResult ){
					return 0;
				}else{
					continue;
				}
			}
#ifdef _PERFORMANCE
			pn.Return();
#endif
		}
#ifdef _PERFORMANCE
		pn.stop();
#endif
	}
}
