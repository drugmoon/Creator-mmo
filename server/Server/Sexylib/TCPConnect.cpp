#include "stdafx.h"
#include "leudgrid/TCPConnect.h"

namespace LEUD
{
	TCPConnect::TCPConnect() : m_Address(0),m_Port(0),m_LocalPort(0),m_Sleep(0),m_ThreadHandle(INVALID_HANDLE_VALUE),m_Started(false){
	}
	TCPConnect::TCPConnect( unsigned long address, unsigned short port ) : m_Address(address),m_Port(port),m_LocalPort(0),m_Sleep(0),m_ThreadHandle(INVALID_HANDLE_VALUE),m_Started(true){
	}
	TCPConnect::TCPConnect( unsigned long address, unsigned short port, unsigned short localport ) : m_Address(address),m_Port(port),m_LocalPort(localport),m_Sleep(0),m_ThreadHandle(INVALID_HANDLE_VALUE),m_Started(true){
	}
	void TCPConnect::Connect( ){
		if( ! m_Started ) return;
		SetEvent( m_WakeEvent );
	}
	void TCPConnect::Connect( unsigned long address, unsigned short port ){
		m_Address = address;
		m_Port = port;
		Connect();
	}
	void TCPConnect::Connect( unsigned long address, unsigned short port, unsigned short localport ){
		m_Address = address;
		m_Port = port;
		m_LocalPort = localport;
		Connect();
	}
	void TCPConnect::Start()
	{
		if( m_Started ) return;
		m_StopEvent = CreateEvent(NULL,FALSE,FALSE,"");
		m_WakeEvent = CreateEvent(NULL,FALSE,FALSE,"");
		unsigned int ThreadID;
		m_ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, TCPConnect_ConnectThread, this, 0, &ThreadID );
		m_Started=true;
	}
	void TCPConnect::Stop(){
		m_Started = false;
		SetEvent(m_StopEvent);
		WaitForSingleObject(m_ThreadHandle,INFINITE);
		CloseHandle(m_StopEvent);
		CloseHandle(m_WakeEvent);
	}
	unsigned int __stdcall TCPConnect::TCPConnect_ConnectThread( void* data ){
		TCPConnect* This = (TCPConnect*)data;
		if( ! This ){
			return 1;
		}
		HANDLE event[2];
		event[0] = This->m_StopEvent;
		event[1] = This->m_WakeEvent;
		while(WAIT_OBJECT_0 != WaitForMultipleObjects(2,event,FALSE,INFINITE)){
			Sleep( This->m_Sleep );
			SOCKET sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			if( INVALID_SOCKET != sock ){
				bool ok = false;
				if( This->m_LocalPort ){
					sockaddr_in service;
					service.sin_family = AF_INET;
					service.sin_addr.s_addr = INADDR_ANY;
					service.sin_port = This->m_LocalPort;
					if ( bind( sock,(SOCKADDR*)&service, sizeof(service) )  == SOCKET_ERROR ) {
						closesocket(sock);
						__try{
							This->DoConnectFail( WSAGetLastError() );
						}__except(UnhandledExceptionFilter(GetExceptionInformation())){
						}
					}else{
						ok = true;
					}
				}else{
					ok = true;
				}
				if( ok ){
					sockaddr_in service;
					service.sin_family = AF_INET;
					service.sin_addr.s_addr = This->m_Address;
					service.sin_port = This->m_Port;
					if ( connect( sock,(SOCKADDR*)&service,sizeof(service) ) == SOCKET_ERROR ){
						closesocket(sock);
						__try{
							This->DoConnectFail( WSAGetLastError() );
						}__except(UnhandledExceptionFilter(GetExceptionInformation())){
						}
					}else{
						__try{
							This->DoConnect( sock );
						}__except(UnhandledExceptionFilter(GetExceptionInformation())){
						}
					}
				}
			}else{
				__try{
					This->DoConnectFail( WSAGetLastError() );
				}__except(UnhandledExceptionFilter(GetExceptionInformation())){
				}
			}
		}
		return 0;
	}
}
