#include "StdAfx.h"
#include "CliAcceptor.h"
#include "GameServerApp.h"
#include "CliNetSession.h"
#include "SessionManager.h"
#include "MSTCPiP.h"
#define     KEEPALIVEINTERVAL                       1000
#define     KEEPALIVETIME                           18000

CCliAcceptor::CCliAcceptor()
{
	m_Listen_port = "9994";
	m_Listen_ip = "0.0.0.0";
	m_Backlog = "128";
	mbOnService = true;
	mAcceptor_num=0;
}

void CCliAcceptor::OnAccepted( SOCKET sock )
{
	if( ! mbOnService )
	{
		closesocket(sock);
		return ;
	}
	CGameServerApp* app = static_cast<CGameServerApp*>(m_app) ;
#ifdef LIMIT_RELEASE
	if( app->m_pSessionManager->GetSessionNum() > 20 )
	{
		closesocket(sock);
		return ;
	}
#endif
	CCliNetSession* session = new CCliNetSession(app);
	struct tcp_keepalive tcpin;
	tcpin.onoff=1;
	tcpin.keepaliveinterval=KEEPALIVEINTERVAL;
	tcpin.keepalivetime=KEEPALIVETIME;
	DWORD dwSize=0;
	int err=WSAIoctl(sock,SIO_KEEPALIVE_VALS,&tcpin,sizeof(tcpin), NULL,0,&dwSize,NULL,NULL);
	app->m_pSessionManager->AddMonitorSession(sock, session);
	mAcceptor_num ++;
}
BOOL CCliAcceptor::Start(LEUD::Application* app)
{
	CGameServerApp* ap = static_cast<CGameServerApp*>(app) ;
	char temp[256];
	::GetPrivateProfileString("net","listen_ip","0.0.0.0",temp,256,ap->m_ConfigFile.c_str());
	m_Listen_ip = temp;
	::GetPrivateProfileString("net","port","9994",temp,256,ap->m_ConfigFile.c_str());
	m_Listen_port = temp;
	::GetPrivateProfileString("net","backlog","128",temp,256,ap->m_ConfigFile.c_str());
	m_Backlog = temp;
	return LEUD::TCPAcceptEventQueue::Start(app);
}
