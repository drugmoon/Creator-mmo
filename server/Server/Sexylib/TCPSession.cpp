#include "stdafx.h"
#include "leudgrid/TCPSession.h"

namespace LEUD
{
	BOOL TCPSession::GetPeerName( SOCKET sock,int& ip,short& port ){
		SOCKADDR_IN sockaddr;
		memset(&sockaddr,0,sizeof(SOCKADDR));

		int nSockAddrLen = sizeof(sockaddr);
		int result = getpeername( sock,(SOCKADDR*)&sockaddr,&nSockAddrLen);
		if( ! result )
		{
			port = ntohs(sockaddr.sin_port);
			ip = ntohl(sockaddr.sin_addr.S_un.S_addr);
		}
		return result;
	}
	BOOL TCPSession::GetLocalName( SOCKET sock,int& ip,short& port ){
		SOCKADDR_IN sockaddr;
		memset(&sockaddr,0,sizeof(SOCKADDR));

		int nSockAddrLen = sizeof(sockaddr);
		int result = getsockname( sock,(SOCKADDR*)&sockaddr,&nSockAddrLen);
		if( ! result )
		{
			port = ntohs(sockaddr.sin_port);
			ip = ntohl(sockaddr.sin_addr.S_un.S_addr);
		}
		return result;
	}
	void TCPSession::GetPeerIP( SOCKET sock, char* ret )
	{
		int ip;
		short port;
		TCPSession::GetPeerName(sock,ip,port);
		in_addr in;in.S_un.S_addr=htonl(ip);
		strncpy_s(ret,64,inet_ntoa(in),64);
		ret[64]=0;
	}
	void TCPSession::Open( SOCKET sock ){
		m_Socket = sock;
	}
	void TCPSession::Close(){
		if(m_Socket == INVALID_SOCKET)return;
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
	void TCPSession::Shutdown(){
		if(m_Socket == INVALID_SOCKET)return;
		linger lg;lg.l_onoff = 1;lg.l_linger = 0;
		setsockopt(m_Socket,SOL_SOCKET,SO_LINGER,(const char*)&lg,sizeof(int));
		closesocket(m_Socket);
	}
	void TCPSessionManageEQ::setEQ( LEUD::EventQueue* EQ )
	{
		m_EQ = EQ;
	}
	void TCPSessionManageEQ::setManager( TCPSessionManager* manager)
	{
		m_SessionManager = manager;
	}
}
