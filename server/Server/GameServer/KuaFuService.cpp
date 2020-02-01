#include "stdafx.h"
#include "KuaFuService.h"
#include "gameserverapp.h"
#include "Protocol/Client_GS.h"
#include "SessionManager.h"
#include "GameServerLib/GameSession.h"
#include "GameServerLib/DomainData.h"
#include "DBManager.h"
#ifdef WIN32
#include "MSTCPiP.h"
#endif
#include "leudgrid/Cryptography.h"
#include "CliNetSession.h"

#define     KEEPALIVEINTERVAL                       1000
#define     KEEPALIVETIME                           18000

class KuaFuConnecter : public LEUD::TCPConnectEventQueue
{
public:
	KuaFuService* mService;
	bool mbOnService;
	std::string mServerKey;
	KuaFuConnecter()
	{
		mbOnService=true;
		mService = 0;
	}
	virtual ~KuaFuConnecter()
	{
		mService = 0;
	}
	virtual void OnConnect(SOCKET sock)
	{
		if( ! mbOnService ){closesocket(sock);return ;}
		if(mService){mService->OnConnect(sock);}
		else{closesocket(sock);}
	}
	virtual void OnFail(DWORD Error)
	{
		if(mService){mService->OnConfail(Error);}
	}
	virtual void OnClose( DWORD error )
	{
	}
	virtual void OnMessage( Stream_t* stream )
	{
	}
	virtual size_t BreakMessage( Stream_t* stream )
	{
		return 0;
	}
};
KuaFuNetSession::KuaFuNetSession()
{
	mNetCloseBreak=false;
	mAuthed=false;
}
KuaFuNetSession::~KuaFuNetSession()
{
	for(std::map<std::string,KuaFuDBRes*>::iterator pos = mKuaFuDBResMap.begin();
		pos != mKuaFuDBResMap.end();pos++ )
	{
		sexy_db_mysql::mem_db_res::destroy(pos->second->mDBRes);
		delete pos->second;
	}
	mKuaFuDBResMap.clear();
}
void KuaFuNetSession::OnClose( DWORD Error )
{
	//printf("%d\n",Error);
	if(m_app&&m_app->m_KuaFuService){
		KuaFuServerData* d=m_app->m_KuaFuService->FindServer(this->mIP.c_str(),this->mPort.c_str());
		if(d){
			d->mSock=INVALID_SOCKET;
			d->mSession=0;
		}
	}
	Close();
	delete this;
}
void KuaFuNetSession::DBResLoadError(const char* loginid,const char* charname){
	OGSLib::GameSession* gs=m_app->m_pGameServerLib->FindSessionWithCharID(charname);
	if(gs){
		gs->GetListener()->notifySessionDataLoaded(0,3);
	}
	//mDBResLoginID="";
	//mDBResCharName="";
	//mDBResLoadFields=mDBResLoadFieldsMax=0;
	//mDBResLoadRows=mDBResLoadRowsMax=0;
}
class DataFetcher_MemDB : public DataFetcher{
public:
	sexy_db_mysql::mem_db_res* res;
	sexy_db_mysql::mem_db_row* row;
	virtual const char* fetch_field(const char* fn,int& len){
		return row->fetch_column(res->fetch_column_idx(fn),&len);
	}
};
size_t KuaFuNetSession::OnMessageOne(const char* msg,size_t size){
	LEUD::StreamFix instream(msg,size,size);
	unsigned short cmdtype;
	instream>>cmdtype;
	instream.seek(instream.seek()-sizeof(unsigned short));
	switch( cmdtype )
	{
	case GS_Client::AuthenticateRES::MSG_ID:
		{
			GS_Client::AuthenticateRES res;
			instream>>res;
			if(instream){
				if(res.result==res.result_succ){
					mAuthed=true;
				}
			}
		}
		break;
	case GS_Client::KuaFuGetCharDataRes::MSG_ID://///玩家在此方法中真正进入了跨服服务器(角色创建出来了)
		{
			GS_Client::KuaFuGetCharDataRes res;
			memset(mDBResD,0,1024*12);
			res.d=mDBResD;
			instream>>res;
			if(instream)
			{
				if(0!=res.result)
				{
					DBResLoadError(res.loginid.c_str(),res.charname.c_str());
				}
				else
				{
					std::map<std::string,KuaFuDBRes*>::iterator pos;
					pos = mKuaFuDBResMap.find(res.loginid);
					if(pos == mKuaFuDBResMap.end())
					{
						if(res.step >= 0)
						{
							KuaFuDBRes* kfdbr = new KuaFuDBRes;
							kfdbr->mDBResLoadFields=0;
							kfdbr->mDBResLoadFieldsMax=res.fields_size;
							kfdbr->mDBResLoadRows=0;
							kfdbr->mDBResLoadRowsMax=res.rows_size;
							kfdbr->mDBResLoginID=res.loginid;
							kfdbr->mDBResCharName=res.charname;
							kfdbr->mDBRes.set_data_info(res.num_fields,res.num_rows,res.fields_size,res.rows_size,res.row_pos);
							mKuaFuDBResMap[res.loginid] = kfdbr;
							pos = mKuaFuDBResMap.find(res.loginid);
						}
					}
					if(pos != mKuaFuDBResMap.end())
					{
						if(res.step>=1000&&res.step<2000){
							pos->second->mDBResLoadFields+=res.len;
							pos->second->mDBRes.set_data_fields(res.pos,res.d,res.len);
						}
						if(res.step>=2000){
							pos->second->mDBResLoadRows+=res.len;
							pos->second->mDBRes.set_data_rows(res.pos,res.d,res.len);
						}
						if(res.step==-1)
						{
							if(pos->second->mDBResLoadFields == pos->second->mDBResLoadFieldsMax && pos->second->mDBResLoadRows == pos->second->mDBResLoadRowsMax)
							{
								OGSLib::DomainData chr;
								DataFetcher_MemDB f;f.res=&pos->second->mDBRes;f.row=pos->second->mDBRes.fetch_next_row();
								m_app->m_pDBManager->UndumpKuafuCharacter(chr,&f);
								chr.exit_map[0]=0;chr.exit_x=0;chr.exit_y=0;
								m_app->m_pGameServerLib->OnLoadCharacter(0,&chr,pos->second->mDBResLoginID.c_str(),pos->second->mDBResCharName.c_str());
								sexy_db_mysql::mem_db_res::destroy(pos->second->mDBRes);
								delete pos->second;
								mKuaFuDBResMap.erase(pos);
							}
							else
							{
								DBResLoadError(res.loginid.c_str(),res.charname.c_str());
							}
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}
	return instream.seek();
}
void KuaFuNetSession::OnMessage( const char* msg,size_t size ){
	const char* m = msg+Client_GS::HeadLen();
	size_t s = size-Client_GS::HeadLen();
	for(int i=0;i<100;i++){
		size_t l = OnMessageOne(m,s);
		if(l<=0){
			break;
		}
		m=m+l;
		s=s-l;
		if(m[0]==0x03&&m[1]==0x03){
			m=m+2;
			s=s-2;
		}else{
			break;
		}
		if(s<=0){
			break;
		}
	}
}
size_t KuaFuNetSession::BreakMessage( LEUD::StreamFix& stream )
{
	if( stream.size() > Client_GS::HeadLen() )
	{
		char buf[20];
		stream.Out(buf,Client_GS::HeadLen());
		if( stream )
		{
			unsigned short len = Client_GS::FindHead(buf);
			return len;
		}		
	}
	return 0;
}
void KuaFuNetSession::EncodeMessage( char* buf,size_t size )
{
	int * bb = (int*) buf;
	size_t s = size/4;
	size_t end = s/2;
	int t = 0;
	for( size_t i = 0;i < end;i ++ )
	{
		t = bb[i];
		bb[i] = bb[s-1-i]^0xA6E839CD;
		bb[s-1-i] = t^0xA6E839CD;
	}
	if( s % 2 == 1 )
	{
		bb[end] = bb[end]^0xA6E839CD;
	}
}
void KuaFuNetSession::DecodeMessage( char* buf,size_t size )
{
	int * bb = (int*) buf;
	size_t s = size/4;
	size_t end = s/2;
	int t = 0;
	for( size_t i = 0;i < end;i ++ )
	{
		t = bb[i];
		bb[i] = bb[s-1-i]^0xA6E839CD;
		bb[s-1-i] = t^0xA6E839CD;
	}
	if( s % 2 == 1 )
	{
		bb[end] = bb[end]^0xA6E839CD;
	}
}
void KuaFuNetSession::MakeHead( char* buf,unsigned short size )
{
	GS_Client::MakeHead(buf,size);
}
void KuaFuNetSession::SetGsParam(const char* k,const char* v){
	Client_GS::KuaFuSetParamReq req;
	req.mParamKey=k;req.mParamValue=v;
	SendMsg(req);
}
void KuaFuNetSession::LoadChar(const char* ticket,const char* loginid,const char* charid,const char* seedname){
	Client_GS::KuaFuGetCharDataReq req;
	req.mTicket=ticket;
	req.mLoginID=loginid;
	req.mCharID=charid;
	req.mSeedName=seedname;
	//mDBResCharName=charid;
	//mDBResLoginID=loginid;
	SendMsg(req);
	flushstream();
}
void KuaFuNetSession::SavePlayerData(const char* charid,const char* seekName,int pkType,int value){
	Client_GS::KuaFuPlayerDataReq req;
	req.charID=charid;
	req.seekName=seekName;
	req.pkType=pkType;
	req.value=value;
	SendMsg(req);
}
void KuaFuNetSession::ComeBackData(const std::string& winArea,const std::string& winGuild,const char* loginid,const char* charid,const char* seekName,int vcoin,int result){
	Client_GS::KuaFuComeBackDataReq req;
	req.winArea = winArea;
	req.winGuild = winGuild;
	req.charID = charid;
	req.seekName = seekName;
	req.account = loginid;
	req.vcoin = vcoin;
	req.result = result;
	SendMsg(req);
	flushstream();
}
KuaFuService::KuaFuService(CGameServerApp* app)
{
	m_app = app;
    mConnectServerKey = 0;
}
KuaFuService::~KuaFuService()
{
	mConnectServerKey =0;
}
void KuaFuService::Start()
{
	mConnecter = new KuaFuConnecter;
	mConnecter->mService = this;
	mConnecter->Start(m_app);
}
void KuaFuService::StopInport()
{
	mConnecter->mbOnService=false;
	mConnecter->mService = 0;
}
void KuaFuService::Stop()
{
	mConnecter->Stop();
	delete mConnecter;mConnecter = 0;
	m_app=0;
}
void KuaFuService::Update()
{
	if(mConnecter)mConnecter->Update();
}
void KuaFuService::OnConnect(SOCKET sock)
{
	for(std::vector<KuaFuServerData>::iterator pos = mServers.begin();
		pos != mServers.end();
		++pos)
	{
		if(pos->mServerName == mConnecter->mServerKey )
		{
			pos->mSock = sock;
			pos->mFailedNum=0;
			if(!pos->mSession)
			{
				KuaFuNetSession* session = new KuaFuNetSession();
				session->m_app = m_app;
				session->mServerName=pos->mServerName;
				session->mIP=pos->mIP;
				session->mPort=pos->mPort;
				pos->mSession=session;
			}
#ifdef WIN32
			struct tcp_keepalive tcpin;
			tcpin.onoff=1;
			tcpin.keepaliveinterval=KEEPALIVEINTERVAL;
			tcpin.keepalivetime=KEEPALIVETIME;
			DWORD dwSize=0;
			int err=WSAIoctl(sock,SIO_KEEPALIVE_VALS,&tcpin,sizeof(tcpin), NULL,0,&dwSize,NULL,NULL);
#else
#if defined(FD_CLOEXEC)
	fcntl(sock, F_SETFD,fcntl(sock, F_GETFD, 0) | FD_CLOEXEC);
#endif
	//int nodelay=1;setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay, sizeof(nodelay));
	int fd_flags = fcntl(sock, F_GETFL, 0);
#if defined(O_NONBLOCK)
	fd_flags |= O_NONBLOCK;
//#elif defined(O_NDELAY)
//	fd_flags |= O_NDELAY;
//#elif defined(FNDELAY)
//	fd_flags |= O_FNDELAY;
#endif
	fcntl(sock, F_SETFL, fd_flags);
#endif
			m_app->m_pSessionManager->AddMonitorSession(sock, pos->mSession);
			Client_GS::AuthenticateREQ req;
			req.AuthType=110;
			req.AuthSeed=0;
			req.PartformID=0;
			std::string loginid=m_app->m_ServerName;
			unsigned char md5[16];
			unsigned char src[256];
			memset(src,0,256);
			_snprintf_s((char*)src,_countof(src),255,"%s%s",loginid.c_str(),m_app->mKuaFuMD5Pad.c_str());
			LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
			_snprintf_s(req.mSessionID,_countof(req.mSessionID),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%s",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15],
                        loginid.c_str()); // MD5 值后面附加服务器名称
            pos->mSession->Send_Message(req);
            for (auto it = pos->mCallBackSessions.begin(); it != pos->mCallBackSessions.end();)
            {
                if (*it)
                {
                    (*it)->notifyAuthResult(GS_Client::AuthenticateRES::result_succ);
                    ++it;
                }
                else
                {
                    it = pos->mCallBackSessions.erase(it);
                }
            }
			if(pos->mServerName.find(std::string("test").c_str()) != std::string::npos)
			{
				printf("connect success!\n");
			}
            break;
		}
	}
	mConnecter->mServerKey = "";
	CheckAndConnect();
}
void KuaFuService::OnConfail(DWORD Error)
{
	printf("%d\n",Error);
	KuaFuServerData* d=FindServer(mConnecter->mServerKey.c_str());
	if(d){
		d->mFailedNum++;
	}
	mConnecter->mServerKey = "";
	CheckAndConnect();
}
void KuaFuService::CheckAndConnect()
{
	if(mConnecter&&mConnecter->mbOnService&&mConnecter->mService&&mConnecter->mServerKey.empty())
	{
		time_t t=time(0);
		for(size_t i = 0;i<mServers.size();i++)
		{
			mConnectServerKey++;
			mConnectServerKey = mConnectServerKey % mServers.size();
			KuaFuServerData* d = &(mServers[mConnectServerKey]);
			if(d->mSock==INVALID_SOCKET && t > (d->mFailedNum)*60+d->mConnTick)
			{
				d->mConnTick=t;
				mConnecter->mServerKey = d->mServerName;
				mConnecter->Connect(d->mIP.c_str(),atoi(d->mPort.c_str()));
				break;
			}
		}
	}
}
int KuaFuService::AddServer(const char* servername,const char* ip,const char* port,CCliNetSession* callbacksession)
{
	KuaFuServerData* _kuafuServerData = FindServer(ip,port);
	if(_kuafuServerData && _kuafuServerData->mSock != INVALID_SOCKET)
	{
		_kuafuServerData->mCallBackSessions.push_back(callbacksession);
		return 1;
	}
	if(_kuafuServerData)
	{
		_kuafuServerData->mCallBackSessions.push_back(callbacksession);
		_kuafuServerData->mFailedNum = 0;
	}
	else
	{
		KuaFuServerData kfsd;
		kfsd.mServerName=servername;
		kfsd.mIP=ip;
		kfsd.mPort=port;
		kfsd.mConnTick=0;
		kfsd.mFailedNum=0;
		kfsd.mCallBackSessions.push_back(callbacksession);
		kfsd.mSock = INVALID_SOCKET;
		kfsd.mSession=0;
		mServers.push_back(kfsd);
		CheckAndConnect();
	}
	return 0;
}
int KuaFuService::AddServer(const char* ip,const char* port)
{
	char sn[512];sprintf(sn,"test_%s%s_%d",ip,port,rand()%9000+1000);
	KuaFuServerData* _kuafuServerData = FindServer(ip,port);
	if(_kuafuServerData && _kuafuServerData->mSock != INVALID_SOCKET)
	{
		printf("connect success!\n");
		return 1;
	}
	if(_kuafuServerData)
	{
		_kuafuServerData->mFailedNum = 0;
	}
	else
	{
		KuaFuServerData kfsd;
		kfsd.mServerName=sn;
		kfsd.mIP=ip;
		kfsd.mPort=port;
		kfsd.mConnTick=0;
		kfsd.mFailedNum=0;
		kfsd.mSock = INVALID_SOCKET;
		kfsd.mSession=0;
		mServers.push_back(kfsd);
		CheckAndConnect();
	}
	return 0;
}
KuaFuServerData* KuaFuService::FindServer(const char* ip,const char* port){
	for(std::vector<KuaFuServerData>::iterator pos=mServers.begin();
		pos!=mServers.end();
		++pos){
			if(pos->mIP.compare(ip)==0&&pos->mPort.compare(port)==0){
				return &(*pos);
			}
	}
	return 0;
}
KuaFuServerData* KuaFuService::FindServer(const char* servername){
	for(std::vector<KuaFuServerData>::iterator pos=mServers.begin();
		pos!=mServers.end();
		++pos){
			if(pos->mServerName.compare(servername)==0){
				return &(*pos);
			}
	}
	return 0;
}
void KuaFuService::RemoveCallBackSession(const char* ip,const char* port,CCliNetSession* callbacksession)
{
	for(std::vector<KuaFuServerData>::iterator pos=mServers.begin();
		pos!=mServers.end();++pos)
	{
		if(pos->mIP.compare(ip)==0&&pos->mPort.compare(port)==0)
		{
			KuaFuServerData* _kuafuServerData = &(*pos);
			if(_kuafuServerData)
			{
				for(std::vector<CCliNetSession*>::iterator _cCliNetSession = _kuafuServerData->mCallBackSessions.begin();
					_cCliNetSession != _kuafuServerData->mCallBackSessions.end();_cCliNetSession++)
				{
					if((*_cCliNetSession) == callbacksession)
					{
						_kuafuServerData->mCallBackSessions.erase(_cCliNetSession);
						break;
					}
				}
				//if(_kuafuServerData->mCallBackSessions.size() == 0)
				//{
					//_kuafuServerData->mSession->Shutdown();
					//mServers.erase(pos);
				//}
			}
			return;
		}
	}
}

template <> LEUD::PoolMemory<KuaFuNetSession>	LEUD::PoolMemoryBase<KuaFuNetSession>::memPool("KuaFuNetSession");
