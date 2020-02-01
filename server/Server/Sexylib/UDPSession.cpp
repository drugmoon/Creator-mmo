#include "stdafx.h"
#include "leudgrid/UDPSession.h"


namespace LEUD
{
	BOOL	UDPSesson::Start( )
	{
		if( INVALID_SOCKET == m_Socket ){
			m_Socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
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
			return TRUE;
		}
		return FALSE;
	}
	BOOL	UDPSesson::Start( unsigned long address, unsigned short port )
	{
		m_Address = address;
		m_Port = port;
		return Start();
	}
	BOOL	UDPSesson::Start( unsigned short port )
	{
		m_Port = port;
		return Start();
	}
	BOOL	UDPSesson::Stop( )
	{
		if( INVALID_SOCKET != m_Socket ){
			closesocket( m_Socket );
			m_Socket = INVALID_SOCKET;
		}
		return TRUE;
	}
}
