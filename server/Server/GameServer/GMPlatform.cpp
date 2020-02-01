#include "StdAfx.h"
#include "GMPlatform.h"
#include "GameServerApp.h"
#include "SessionManager.h"
#include "leudgrid/Cryptography.h"
#include "GameServerLib/GMCommand.h"
#include "GameServerLib/GameServerLib.h"
#include <fstream>
#include "GameServerLib/UtilString.h"

GMAcceptor::GMAcceptor()
{
	m_Listen_port = "9994";
	m_Listen_ip = "0.0.0.0";
	m_Backlog = "128";
	mbOnService = true;
	mAcceptor_num=0;
}

void GMAcceptor::OnAccepted( SOCKET sock )
{
	if( ! mbOnService )
	{
		closesocket(sock);
		return ;
	}
	char ip[128];
	LEUD::TCPSession::GetPeerIP(sock,ip);
	if( std::find(mIPs.begin(),mIPs.end(),ip) != mIPs.end() )
	{
		CGameServerApp* app = static_cast<CGameServerApp*>(m_app) ;
		GMNetSession* session = new GMNetSession(app);
		session->mMD5Key = mMD5Key;
		session->m_app = app;
		app->m_pSessionManager->AddMonitorSession(sock, session);
		mAcceptor_num ++;
	}
	else
	{
		closesocket(sock);
		return;
	}
}
bool GMAcceptor::LoadAllowIP()
{
	char temp[156];
	CGameServerApp* app = static_cast<CGameServerApp*>(m_app) ;
	GetPrivateProfileString("path","config_path","d:\\",temp,256,app->m_ConfigFile.c_str());
	std::string mConfigPath = temp;
	if( mConfigPath.substr(mConfigPath.length()-1) != "\\" )
	{
		mConfigPath += "\\";
	}
	std::string mapinfofile = mConfigPath+"gmallowip.txt";
	std::ifstream ifs(mapinfofile.c_str());
	std::string line;
	if( ifs )
	{
		mIPs.clear();
		while( std::getline(ifs,line) )
		{
			if( line.length()> 0 )
			{
				if( line.substr(0,1) == "#" || line.substr(0,1) == ";" )
				{
					continue;
				}
				char temp[1024];
				OGSLib::UtilString::act_2_utf8(line.c_str(),line.length(),temp,1024);
				line = temp;
				mIPs.push_back(line);
			}
		}
	}
	return true;
}

BOOL GMAcceptor::Start(LEUD::Application* app)
{
	CGameServerApp* ap = static_cast<CGameServerApp*>(app) ;
	m_app = ap;
	char temp[256];
	::GetPrivateProfileString("master","listen_ip","0.0.0.0",temp,256,ap->m_ConfigFile.c_str());
	m_Listen_ip = temp;
	::GetPrivateProfileString("master","port","9994",temp,256,ap->m_ConfigFile.c_str());
	m_Listen_port = temp;
	::GetPrivateProfileString("master","backlog","128",temp,256,ap->m_ConfigFile.c_str());
	m_Backlog = temp;
	::GetPrivateProfileString("master","key","oncientkey",temp,256,ap->m_ConfigFile.c_str());
	mMD5Key = temp;
	LoadAllowIP();
	return LEUD::TCPAcceptEventQueue::Start(app);
}
GMNetSession::GMNetSession(CGameServerApp* app)
{
}
GMNetSession::~GMNetSession(void)
{
}
void GMNetSession::OnClose( DWORD Error )
{
	Close();
	delete this;
}
void GMNetSession::OnMessage( const char* msg,size_t size )
{
	if( size < 2 ) return;
	if( msg[size-1] == 10 ){
		size --;
	}
	if( msg[size-1] == 13 ){
		size --;
	}
	if( m_app && m_app->m_pGameServerLib)
	{
		char c[512];
		memset(c,0,512);
		if( size < 512 )
		{
//#ifdef _DEBUG
//			memcpy(c,msg,size);
//#else
			if( size > 32 )
			{
				unsigned char md5[16];
				unsigned char src[256];
				memset(src,0,256);
				memcpy(src,msg+32,size-32);
				memcpy(src+size-32,mMD5Key.c_str(),mMD5Key.length());
				LEUD::CryptographyMD5::Do(md5,src,size-32+mMD5Key.length());
				char temp[256];
				_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
				temp[255]=0;
				bool ok = true;
				for(int i = 0;i < 32 ;i ++)
				{
					if( msg[i] != temp[i] )
					{
						ok=false;
						break;
					}
				}
				if( ok )
				{
					memcpy(c,msg+32,size-32);
				}
			}
//#endif
		}
		if( strlen(c) > 0 )
		{
			//printf("%s",c);
			std::string cmd(c);
			std::string res = m_app->m_pGameServerLib->GetGMCommand()->DoSystemCommand(cmd,1000);
			//printf("%s",res.c_str());
			Stream512_t* stream=new Stream512_t;
			(*stream).In(res.c_str(),res.size());
			_snprintf_s(c,_countof(c),"\n",511);
			c[511]=0;
			stream->In(c,strlen(c));
			Send(stream);
			stream->Release();
		}
		else
		{
			Stream512_t* stream=new Stream512_t;
			std::string err("md5 error");
			(*stream).In(err.c_str(),err.length());
			_snprintf_s(c,_countof(c),"\n",511);
			c[511]=0;
			stream->In(c,strlen(c));
			Send(stream);
			stream->Release();
		}
	}
}
size_t GMNetSession::BreakMessage( Stream_t* stream )
{
	return 0;
}
size_t GMNetSession::BreakMessage( LEUD::StreamFix& stream )
{
	return stream.findchar('\n');
}
LEUD::PoolMemory<GMNetSession>	LEUD::PoolMemoryBase<GMNetSession>::memPool("GMNetSession");
