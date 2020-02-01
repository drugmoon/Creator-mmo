#include "StdAfx.h"
#include "OCenterManager.h"
#include "CliNetSession.h"
#include "GameServerApp.h"
#include "SessionManager.h"
#include "leudgrid/MemoryPool.h"
#include "leudgrid/ToolsConsole.h"


class OCenterEventQueueThread : public LEUD::EventQueueThread
{
public:
	OCenterManager* m_DBManager;
	OCenterEventQueueThread(OCenterManager* dbm)
	{
		m_DBManager = dbm;
		m_Name = "OCenterEventQueueThread";
	}
	virtual ~OCenterEventQueueThread()
	{
		m_DBManager=0;
	}
public:
	virtual void OnUpdate()
	{
		LEUD::EventQueueThread::OnUpdate();
		m_DBManager->PingDB();
	}
};
class PushOCenterEventClientAuth : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushOCenterEventClientAuth >
{
public:
	OCenterManager* manager;
	std::string session_id;
	std::string loginIP;
	SOCKET sock;
	std::string account;
	//int authseed;
	//int authid;
	int tim;
	int result;
	PushOCenterEventClientAuth()
	{
		result = 0;
		tim = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->ClientAuth(session_id,loginIP,account,tim/*,authseed,authid*/);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			CCliNetSession* se = (CCliNetSession*)manager->m_App->m_pSessionManager->FindSession(sock);
			if( se )
			{
				se->OnAuthResult(result,session_id,account,tim);
			}
        }
        delete this;
	}
};
void OCenterManager::push_client_auth(SOCKET sock,const std::string& sessionID,const std::string& loginIP/*,int authseed,int authid*/)
{
	if( m_pDBThread )
	{
		PushOCenterEventClientAuth* e = new PushOCenterEventClientAuth;
		e->manager = this;
		//e->authseed = authseed;
		e->sock = sock;
		e->session_id = sessionID;
		if( m_check_ip )
		{
			e->loginIP = loginIP;
		}
		//e->authid = authid;
		m_pDBThread->PushEvent(e);
	}
}
class PushOCenterEventUpdateTicket : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushOCenterEventUpdateTicket >
{
public:
	OCenterManager* manager;
	std::string session_id;
	int authid;
	SOCKET sock;
	int result;
	PushOCenterEventUpdateTicket()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->update_ticket(session_id,authid);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			CCliNetSession* se = (CCliNetSession*)manager->m_App->m_pSessionManager->FindSession(sock);
			if( se )
			{
				se->OnUpdateTicket(result);
			}
        }
        delete this;
	}
};
void OCenterManager::push_update_ticket(SOCKET sock,const std::string& sessionID,int authid)
{
	if( m_pDBThread )
	{
		PushOCenterEventUpdateTicket* e = new PushOCenterEventUpdateTicket;
		e->manager = this;
		e->session_id = sessionID;
		e->authid = authid;
		e->sock = sock;
		m_pDBThread->PushEvent(e);
	}
}
class PushOCenterEventLogCongzi : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushOCenterEventLogCongzi >
{
public:
	OCenterManager* manager;
	std::string account;
	int number;
	PushOCenterEventLogCongzi()
	{
		number = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			manager->LogCongzi(account,number);
        }
        delete this;
	}
};
void OCenterManager::push_log_congzi(const std::string& account,int number)
{
	if( m_pDBThread )
	{
		PushOCenterEventLogCongzi* e = new PushOCenterEventLogCongzi;
		e->manager = this;
		e->account=account;
		e->number=number;
		m_pDBThread->PushEvent(e);
	}
}
class PushOCenterEventFreshVcoin : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushOCenterEventFreshVcoin >
{
public:
	OCenterManager* manager;
	std::string account;
	std::string chrname;
	int result;
	int czhaccu;
	int gs;
	PushOCenterEventFreshVcoin()
	{
		result = 0;
		czhaccu = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			czhaccu = manager->FreshVcoinAccu(account,gs);
			result = manager->FreshVcoin(account,chrname,gs);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			manager->m_App->m_pGameServerLib->OnFreshVcoin(account,result,czhaccu,(gs>0));
        }
        delete this;
	}
};
void OCenterManager::push_fresh_vcoin(const std::string& account,const std::string& chrname)
{
	if( m_pDBThread )
	{
		PushOCenterEventFreshVcoin* e = new PushOCenterEventFreshVcoin;
		e->manager = this;
		e->account = account;
		e->chrname = chrname;
		m_pDBThread->PushEvent(e);
	}
}
OCenterManager::OCenterManager(CGameServerApp* app)
{
	m_App=app;
	m_BufferLen = 1024*16;
	m_Buffer1 = new char[m_BufferLen];
	m_Buffer2 = new char[m_BufferLen];
	m_PingTime = 0;
	m_pDBModule = 0;
	m_pDBThread = 0;
	m_pEventQueue = 0;
	m_check_ip = true;
}
OCenterManager::~OCenterManager(void)
{
	delete[] m_Buffer1;m_Buffer1=0;
	delete[] m_Buffer2;m_Buffer2=0;
}
void OCenterManager::PingDB()
{
	if( m_PingTime +30 < time(0) )
	{
		m_PingTime = time(0);
		if( m_pDBModule )
		{
			m_pDBModule->Ping();
		}
	}
}
void OCenterManager::checkIP(bool checkip)
{
	m_check_ip = checkip;
}
void OCenterManager::Start()
{
	m_pDBModule = new LEUD::DBModuleMysql;
	char temp[256];
	::GetPrivateProfileString("ocenter","ip","127.0.0.1",temp,256,m_App->m_ConfigFile.c_str());
	std::string ip = temp;
	::GetPrivateProfileString("ocenter","port","3306",temp,256,m_App->m_ConfigFile.c_str());
	std::string port = temp;
	::GetPrivateProfileString("ocenter","db","octgame",temp,256,m_App->m_ConfigFile.c_str());
	std::string db = temp;
	::GetPrivateProfileString("ocenter","user","root",temp,256,m_App->m_ConfigFile.c_str());
	std::string user = temp;
	::GetPrivateProfileString("ocenter","pwd","123456",temp,256,m_App->m_ConfigFile.c_str());
	std::string pwd = temp;
	if( 0 == GetPrivateProfileInt("ocenter","checkip",1,m_App->m_ConfigFile.c_str()) )
	{
		m_check_ip = false;
	}
	if( ! m_pDBModule->Create(ip,port,db,user,pwd) )
	{
		printf("OCenter DB connect failed\n");
		return;
	}
	DBUpVersion();
	m_pEventQueue = new LEUD::EventQueue;
	m_pEventQueue->m_ID = 200;
	m_pDBThread = new OCenterEventQueueThread(this);
	m_pDBThread->m_ID = 100;
	m_pDBThread->Start(0,0);
}
void OCenterManager::Stop()
{
	m_pDBThread->Stop();
	m_pEventQueue->HandEvent(0);
	m_pEventQueue->HandEvent(0);
	m_pEventQueue->HandEvent(0);
	delete m_pDBThread;m_pDBThread=0;
	delete m_pEventQueue;m_pEventQueue=0;
	m_pDBThread=0;
	m_pEventQueue=0;
	m_pDBModule->Destroy();
	delete m_pDBModule;m_pDBModule=0;
	m_pDBModule = 0;
}
void OCenterManager::Update()
{
	//if( m_pDBThread )
	//{
	//	m_pDBThread->Wake();
	//}
	if( m_pEventQueue )
	{
		m_pEventQueue->HandEvent(0);
	}
}
void OCenterManager::DBUpVersion()
{
	int len=0;my_ulonglong eff=0;const char* sql;char temp[256];
	int version=0;
	{
		MYSQL_RES* res = m_pDBModule->QueryStore("select * from cendbver");
		if(res){
			MYSQL_ROW row= m_pDBModule->FetchRow(res);
			if( row ){
				version=atoi(m_pDBModule->FetchField(row,res,"versio",len));
			}
		}
		printf("fetch center db version %d\n",version);
	}
	if(0==version){
		sql="CREATE TABLE `cendbver` (`versio`  int NULL ,PRIMARY KEY (`versio`) )";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true);

		version=200;sprintf(temp,"insert into cendbver values ('100')",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(200==version){

		sql = "ALTER TABLE `pay_info` ADD COLUMN  `gs` INTEGER UNSIGNED NOT NULL DEFAULT 0 AFTER `paytime`";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		sql = "ALTER TABLE `pay_log` ADD COLUMN  `gs` INTEGER UNSIGNED NOT NULL DEFAULT 0 AFTER `paytime`";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=201;sprintf(temp,"update cendbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(201==version){

		sql = "ALTER TABLE `login_info` ADD COLUMN  `pid` INTEGER UNSIGNED DEFAULT NULL AFTER `onlineip`";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		sql = "ALTER TABLE `login_info` ADD COLUMN  `idfa` varchar(255) DEFAULT NULL AFTER `pid`";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=202;sprintf(temp,"update cendbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(202==version){

		sql = "ALTER TABLE `login_log` ADD COLUMN  `pid` INTEGER UNSIGNED DEFAULT NULL AFTER `onlineip`";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		sql = "ALTER TABLE `login_log` ADD COLUMN  `idfa` varchar(255) DEFAULT NULL AFTER `pid`";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=203;sprintf(temp,"update cendbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	//if(204==version){
	//	sql = "ALTER TABLE `login_info` ADD COLUMN  `pid` INTEGER UNSIGNED DEFAULT NULL AFTER `onlineip`";
	//	LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
	//	sql = "ALTER TABLE `login_info` ADD COLUMN  `idfa` varchar(255) DEFAULT NULL AFTER `pid`";
	//	LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

	//	version=205;sprintf(temp,"insert into cendbver values ('100')",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	//}
}
int OCenterManager::update_ticket(const std::string& sessionID,int authid)
{
	char temp[513];
	m_pDBModule->EscapeString(temp,sessionID.c_str(),(unsigned long)sessionID.length());
	std::string sid = temp;

	//printf("sss,%s,%d\n",sid.c_str(),authid);
	std::stringstream ss;
	ss<<"update login_info set logintime=date_sub(now(),interval 7 minute) where ticket='"<<sid<<"'";
	m_pDBModule->QueryStore(ss.str().c_str(),true);

	return 0;
}
int OCenterManager::ClientAuth(const std::string& sessionID,const std::string& loginIP,std::string& account,int &tim/*,int authseed,int authid*/)
{
	char temp[513];
	m_pDBModule->EscapeString(temp,sessionID.c_str(),(unsigned long)sessionID.length());
	std::string sid = temp;

	//printf("sss,%s,%s,%d,%d\n",sid.c_str(),loginIP.c_str(),authseed,authid);
	int ret=1;
	std::string acc;
	std::stringstream ss;
	ss<<"select username,status,date_add(`logintime`,interval 10 minute) >= now() as tim from login_info where ticket='"<<sid<<"'";
	//if( loginIP.length() > 0 )
	//{
	//	ss<<" and onlineip='"<<loginIP<<"'";
	//}
	//ss<<" and date_add(`logintime`,interval 10 minute) >= now()";
	MYSQL_RES* res = m_pDBModule->QueryStore(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row = m_pDBModule->FetchRow(res);
		if( row )
		{
			int len;
			//bool ok = false;
			//const char* t = m_pDBModule->FetchField(row,res,"status",len);
			//if( t )
			//{
			//	int st = atoi(t);
			//	if( st == 0 || authseed == st+(st%10+1)*7 )
			//	{
			//		ok = true;
			//	}
			//}
			//else
			//{
			//	ok = true;
			//}
			//if( ok )
			{
				tim=0;
				const char* te = m_pDBModule->FetchField(row,res,"tim",len);
				if( te )
				{
					tim = atoi(te);
				}
				account = m_pDBModule->FetchField(row,res,"username",len);
				ret = 0;
			}
			//else
			//{
			//	ret = 3;
			//}
		}
		m_pDBModule->FreeResult(res);
	}
	//if( ret == 0 )
	//{
	//	std::stringstream ss;
	//	ss<<"update login_info set status='"<<authseed<<"' where id='"<<authid<<"' and ticket='"<<sid<<"'";
	//	m_pDBModule->QueryStore(ss.str().c_str(),true);
	//}
	return ret;
}
int OCenterManager::FreshVcoin(const std::string& account,const std::string& chrname,int &gs)
{
	int ret = 0;
	char temp[513];
	m_pDBModule->EscapeString(temp,account.c_str(),(unsigned long)account.length());
	std::string acid = temp;

	std::map<int,int> vclist;

	std::stringstream ss;
	ss<<"select * from pay_info where paytouser='"<<acid<<"' and status != 100";
	MYSQL_RES* res = m_pDBModule->QueryStore(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			int id = atoi(m_pDBModule->FetchField(row,res,"id",len));
			int gs = atoi(m_pDBModule->FetchField(row,res,"gs",len));
			int num = atoi(m_pDBModule->FetchField(row,res,"paygold",len));
			vclist[id] = num;
		}
		m_pDBModule->FreeResult(res);
	}
	for( std::map<int,int>::iterator pos = vclist.begin();
		pos != vclist.end();
		++ pos )
	{
		ret += pos->second;
		std::stringstream ss;
		ss<<"update pay_info set status=100,chrname='"<<chrname<<"' where id='"<<pos->first<<"'";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	return ret;
}
int OCenterManager::FreshVcoinAccu(const std::string& account,int &gs)
{
	int ret = 0;
	char temp[513];
	m_pDBModule->EscapeString(temp,account.c_str(),(unsigned long)account.length());
	std::string acid = temp;

	std::map<int,int> vclist;

	std::stringstream ss;
	ss<<"select * from pay_info where paytouser='"<<acid<<"' and status != 100";
	MYSQL_RES* res = m_pDBModule->QueryStore(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			int id = atoi(m_pDBModule->FetchField(row,res,"id",len));
			int gs = atoi(m_pDBModule->FetchField(row,res,"gs",len));
			int num = atoi(m_pDBModule->FetchField(row,res,"paygold",len));
			const char* dummy;
			int accFlags = 1;
			dummy = m_pDBModule->FetchField(row,res,"flags",len);
			if( dummy ){ accFlags = atoi(dummy);}
			if (accFlags == 1)
			{
				vclist[id] = num;
			}
		}
		m_pDBModule->FreeResult(res);
	}
	for( std::map<int,int>::iterator pos = vclist.begin();
		pos != vclist.end();
		++ pos )
	{
		ret += pos->second;
	}
	return ret;
}
int OCenterManager::LogCongzi(const std::string& account,int number)
{
	std::stringstream ss;
	ss<<"insert into pay_log set paytouser='"<<account<<"',paygold=1,paymoney='"<<number<<"',paytime=now(),paynum=1";
	m_pDBModule->QueryStore(ss.str().c_str(),true);
	return 0;
}

LEUD::PoolMemory<PushOCenterEventClientAuth>	LEUD::PoolMemoryBase<PushOCenterEventClientAuth>::memPool("PushOCenterEventClientAuth");
LEUD::PoolMemory<PushOCenterEventFreshVcoin>	LEUD::PoolMemoryBase<PushOCenterEventFreshVcoin>::memPool("PushOCenterEventFreshVcoin");
LEUD::PoolMemory<PushOCenterEventLogCongzi>		LEUD::PoolMemoryBase<PushOCenterEventLogCongzi>::memPool("PushOCenterEventLogCongzi");
LEUD::PoolMemory<PushOCenterEventUpdateTicket>	LEUD::PoolMemoryBase<PushOCenterEventUpdateTicket>::memPool("PushOCenterEventUpdateTicket");