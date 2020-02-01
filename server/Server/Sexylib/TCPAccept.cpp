#include "stdafx.h"
#include "leudgrid/TCPAccept.h"
#include "leudgrid/Performance.h"

namespace LEUD
{
	TCPAcceptThread::TCPAcceptThread(unsigned long address,unsigned short port):TCPAccept(address,port),m_Thread(0){
	}
	TCPAcceptThread::TCPAcceptThread( ):TCPAccept(0,0),m_Thread(0){
	}
	TCPAcceptThread::~TCPAcceptThread( ){
	}
	BOOL	TCPAcceptThread::Start( int backlog ){
		if( INVALID_SOCKET == m_Socket ){
			m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			if( m_Socket == INVALID_SOCKET )
			{
				return FALSE;
			}
			sockaddr_in service;
			service.sin_family = AF_INET;
			service.sin_addr.s_addr = m_Address;
			service.sin_port = m_Port;
			if ( bind( m_Socket,(SOCKADDR*)&service, sizeof(service) )  == SOCKET_ERROR ) {
				closesocket(m_Socket);
				m_Socket = INVALID_SOCKET;
				return FALSE;
			}
			if (listen( m_Socket, backlog ) == SOCKET_ERROR) {
				closesocket(m_Socket);
				m_Socket = INVALID_SOCKET;
				return FALSE;
			}
			UINT   ThreadID;
			m_Thread = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, this, 0, &ThreadID );
#ifdef _PERFORMANCE
			printf("%5d TCPAcceptThread::Start\n",ThreadID);
#endif
			return TRUE;
		}
		return FALSE;
	}
	BOOL	TCPAcceptThread::Start( int backlog,unsigned long address, unsigned short port ){
		m_Address = address;
		m_Port = port;
		return Start(backlog);
	}
	BOOL	TCPAcceptThread::Stop( ){
		if( INVALID_SOCKET != m_Socket ){
			closesocket( m_Socket );
			m_Socket = INVALID_SOCKET;
			WaitForSingleObject(m_Thread,INFINITE);
		}
		return TRUE;
	}
	unsigned int __stdcall TCPAcceptThread::ThreadFun( void* data ){
#ifdef _PERFORMANCE
		PerfSigleNode pn;
		pn.start("TCPAcceptThread::ThreadFun");
#endif
		TCPAcceptThread* This = (TCPAcceptThread*)data;
		sockaddr_in service;int addrlen;
		SOCKET s;
		for(;;){
			addrlen =sizeof(service);
			if( INVALID_SOCKET != (s=accept(This->m_Socket,(SOCKADDR*)&service,&addrlen))){
				__try{
#ifdef _PERFORMANCE
					pn.Call();
#endif
					This->DoAccept( s );
#ifdef _PERFORMANCE
					pn.Return();
#endif
				}__except(UnhandledExceptionFilter(GetExceptionInformation())){
				}
			}else{
				break;
			}
		}
#ifdef _PERFORMANCE
		pn.stop();
#endif
		return 0;
	}
}
