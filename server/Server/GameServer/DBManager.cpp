#include "StdAfx.h"
#include "Dbmanager.h"
#include "GameServerApp.h"
#include "CliNetSession.h"
#include "SessionManager.h"
#include "Protocol/Client_GS.h"
#include "GameServerLib/DomainData.h"
#include "GameServerLib/SubSkill.h"
#include "GameServerLib/SubItem.h"
#include "GameServerLib/SubTask.h"
#include "GameServerLib/SubGift.h"
#include "GameServerLib/SubMail.h"
#include "GameServerLib/SubFriend.h"
#include "GameServerLib/GuildGuild.h"
#include "GameServerLib/ManagerGuild.h"
#include "GameServerLib/ManagerConsignment.h"
#include "GameServerLib/CoupleCouple.h"
#include "GameServerLib/ManagerCouple.h"
#include "GameServerLib/SubStatus.h"
#include "GameServerLib/ManagerAutoRun.h"
#include "GameServerLib/ManagerMail.h"
#include "GameServerLib/SubAchieve.h"
#include "leudgrid/EventQueueThread.h"
#include "leudgrid/MemoryPool.h"
#include "leudgrid/ToolsConsole.h"
#include "GameServerLib/DomainPlay.h"
#include "GameServerLib/ManagerRelationCache.h"
#include "GameServerLib/ManagerItemDef.h"

//#include "kuafumanager.h"

class DBEventQueueThread : public LEUD::EventQueueThread
{
public:
	DBManager* m_DBManager;
	DBEventQueueThread(DBManager* dbm)
	{
		m_DBManager = dbm;
		m_Name = "DBEventQueueThread";
	}
	virtual ~DBEventQueueThread()
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

class PushDBEventFreshGift : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventFreshGift >
{
public:
	DBManager* manager;
	char seedname[256];
	std::vector<OGSLib::SubGift> giftarray;
	int result;
	PushDBEventFreshGift()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->DoFreshGift(seedname, giftarray);
			manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
            return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnFreshGift(seedname, 0, &giftarray);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnFreshGift(seedname, 1, 0);
			}
        }
        delete this;
	}
};
void DBManager::push_fresh_gift(const char* seedname)
{
	PushDBEventFreshGift* e = new PushDBEventFreshGift;
	strncpy_s(e->seedname, _countof(e->seedname), seedname, 255);
	e->seedname[255] = 0;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
}

class PushDBEventLoadCharacter : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadCharacter >
{
public:
	DBManager* manager;
	char account[256];
	char chrname[256];
	char seedname[256];
	char table[256];
	OGSLib::DomainData chr;
	int GUID;
	int result;
	PushDBEventLoadCharacter()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->DoLoadCharacter(account,chrname,seedname,chr,table);
			manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
            return;
		}
		if( queue_id == 200 )
		{
			if(GUID>0)
			{
				if( result == 0 )
				{
					manager->m_App->m_pGameServerLib->OnLoadCharacter(GUID,0,&chr);
				}
				else
				{
					manager->m_App->m_pGameServerLib->OnLoadCharacter(GUID,1,0);
				}
			}
			else
			{
#ifndef LIMIT_RELEASE_2
				//void* se = manager->m_App->m_pSessionManager->FindSession(manager->m_App->m_KuaFuManager->GetSock(GUID));
				//if(se)
				//{
				//	CCliNetSession* session = static_cast<CCliNetSession*>(se);
				//	if( session)
				//	{
				//		session->KuaFuSendCharData(result,&chr);
				//	}
				//}
				//else
				//{
				//	manager->m_App->m_KuaFuManager->RemSock(GUID);
				//}
#endif
			}
        }
        delete this;
	}
};
int DBManager::push_load_character(const char* account,const char* chrname,const char* seedname,const char* client_ip,int GUID,const char* table)
{
	PushDBEventLoadCharacter* e = new PushDBEventLoadCharacter;
	strncpy_s(e->account,_countof(e->account),account,255);
	e->account[255]=0;
	strncpy_s(e->chrname,_countof(e->chrname),chrname,255);
	e->chrname[255]=0;
	strncpy_s(e->seedname,_countof(e->seedname),seedname,255);
	e->seedname[255]=0;
	strncpy_s(e->table,_countof(e->table),table,255);
	e->table[255]=0;
	strncpy_s(e->chr.mLoginIP,_countof(e->chr.mLoginIP),client_ip,255);
	e->chr.mLoginIP[255]=0;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}
class PushDBEventSaveCharacter : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveCharacter >
{
public:
	DBManager* manager;
	std::string table;
	OGSLib::DomainData chr;
	bool quit;
	int GUID;
	int result;
	PushDBEventSaveCharacter()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id==100)
		{
			if( table== "demo")
			{
				bool exist = false;
				std::stringstream ss;
				ss<<"select id from demo where chrname='"<<chr.mChrName<<"' and server_id='"<<manager->m_App->m_ServerID<<"' ";
				MYSQL_RES* res = manager->m_pDBModule->QueryStore(ss.str().c_str());
				if( res )
				{
					MYSQL_ROW row = manager->m_pDBModule->FetchRow(res);
					if( row )
					{
						exist = true;
					}
					manager->m_pDBModule->FreeResult(res);
				}
				if( ! exist )
				{
					std::stringstream ss;
					ss<<"insert into demo set chrname='"<<chr.mChrName
						<<"',account='"<<chr.mAccount
						<<"',job='"<<chr.job
						<<"',gender='"<<chr.gender
						<<"',lv='"<<chr.level
						<<"',server_id='"<<manager->m_App->m_ServerID
						<<"',create_time=now()";
					manager->m_pDBModule->QueryStore(ss.str().c_str(),true);
				}
			}
			result = manager->DoSaveCharacter(&chr,quit,table.c_str());
			manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
            return;
		}
		if( queue_id==200)
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnSaveCharacter(GUID,0);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnSaveCharacter(GUID,1);
			}
        }
        delete this;
	}
};
int DBManager::push_save_character(OGSLib::DomainData* character,bool quit,int GUID,const char* table)
{
	PushDBEventSaveCharacter* e = new PushDBEventSaveCharacter;
	e->chr = *character;
	e->quit = quit;
	e->table = table;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}
class PushDBEventSaveKuafuCharacter : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveKuafuCharacter >
{
public:
	DBManager* manager;
	std::string charID;
	std::string account;
	int vcoin;
	int kuafuResult;
	int result;
	PushDBEventSaveKuafuCharacter()
	{
		vcoin = 0;
		kuafuResult = 0;
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id==100)
		{
			result = manager->DoSaveKuafuCharacter(charID,account,vcoin,kuafuResult);
			//manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
		}
		//if( queue_id==200)
		//{
		//	if( result == 0 )
		//	{
		//	}
		//	else
		//	{
		//	}
		//	delete this;
		//}
		delete this;
	}
};
int DBManager::push_save_kuafu_character(std::string& charID,std::string& account,int vcoin,int kuafuResult)
{
	PushDBEventSaveKuafuCharacter* e = new PushDBEventSaveKuafuCharacter;
	e->charID = charID;
	e->account =  account;
	e->vcoin = vcoin;
	e->kuafuResult = kuafuResult;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}
class PushDBEventLoadGuild : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadGuild >
{
public:
	DBManager* manager;
	OGSLib::ManagerGuild guild_manager;
	int GUID;
	int result;
	PushDBEventLoadGuild():guild_manager(0)
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->LoadGuilds(guild_manager);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnLoadGuilds(GUID,0,guild_manager);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnLoadGuilds(GUID,1,guild_manager);
			}
			manager->m_App->m_GuildsLoaded=true;
        }
        delete this;
	}
};
int DBManager::push_load_guild(int GUID)
{
	PushDBEventLoadGuild* e = new PushDBEventLoadGuild;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
	return 0;
}
class PushDBEventSaveGuild : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveGuild >
{
public:
	DBManager* manager;
	OGSLib::ManagerGuild guild_manager;
	int GUID;
	int result;
	PushDBEventSaveGuild():guild_manager(0)
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->SaveGuilds(guild_manager);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnSaveGuilds(GUID,0);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnSaveGuilds(GUID,1);
			}
			guild_manager.dest(false);
        }
        delete this;
	}
};
int DBManager::push_save_guild(const OGSLib::ManagerGuild& guild_manager,int GUID)
{
	PushDBEventSaveGuild* e = new PushDBEventSaveGuild;
	e->GUID = GUID;
	e->manager = this;
	OGSLib::ManagerGuild::GuildMap m;
	for( OGSLib::ManagerGuild::GuildMap::const_iterator guild_pos = guild_manager.GetGuilds().begin();
		guild_pos != guild_manager.GetGuilds().end();
		++ guild_pos )
	{
		OGSLib::Guild* g = new OGSLib::Guild;
		*g = (*guild_pos->second);
		m[g->SeedID()] = g;
	}
	e->guild_manager.OnLoadGuilds(m);
	m_pDBThread->PushEvent(e);
	return 0;
}

class PushDBEventLoadConsignment : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadConsignment >
{
public:
	DBManager* manager;
	std::vector<OGSLib::ConsignableItemInfo*> info_list;
	std::map<std::string, int> user_vcoin_map;
	int GUID;
	int result;
	PushDBEventLoadConsignment()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
 			result = manager->DoLoadConsignment(info_list, user_vcoin_map);
			manager->m_pEventQueue->PushEvent(this);
			return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnLoadConsignment(GUID,0,info_list, user_vcoin_map);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnLoadConsignment(GUID,1,info_list, user_vcoin_map);
			}
		}
		delete this;
	}
};

int DBManager::push_load_consignment(int GUID)
{
	PushDBEventLoadConsignment* e = new PushDBEventLoadConsignment;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}

class PushDBEventSaveConsignment : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveConsignment >
{
public:
	DBManager* manager;
	std::vector<OGSLib::ConsignableItemInfo *> item_list;
	std::map<std::string, int> user_vcoin_map;
	int GUID;
	int result;
	PushDBEventSaveConsignment()
	{
		result = 0;
	}
	void dest_data()
	{
		for (size_t i = 0; i < item_list.size(); ++i)
		{
			SAFE_DELETE(item_list[i]);
		}
		item_list.clear();
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->DoSaveConsignment(item_list, user_vcoin_map);
			manager->m_pEventQueue->PushEvent(this);
			return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnSaveConsignment(GUID,0);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnSaveConsignment(GUID,1);
			}
			dest_data();
		}
		delete this;
	}
};
int DBManager::push_save_consignment(OGSLib::ManagerConsignment& consignment_manager,int GUID)
{
	PushDBEventSaveConsignment* e = new PushDBEventSaveConsignment;
	e->GUID = GUID; 
	e->manager = this;
	
	OGSLib::ManagerConsignment::ConsignableItemList info_list = consignment_manager.GetConsignableItemInfoList();
	OGSLib::ManagerConsignment::ConsignableItemList::iterator it = info_list.begin();
	while (it != info_list.end())
	{
		OGSLib::ConsignableItemInfo * item_info = new OGSLib::ConsignableItemInfo;
		*item_info = *(*it);
		e->item_list.push_back(item_info);
		++ it;
	}
	OGSLib::ManagerConsignment::ConsignableItemList over_time_list = consignment_manager.GetOverTimeItemInfoList();
	it = over_time_list.begin();
	while (it != over_time_list.end())
	{
		OGSLib::ConsignableItemInfo * item_info = new OGSLib::ConsignableItemInfo;
		*item_info = *(*it);
		e->item_list.push_back(item_info);
		++ it;
	}

	e->user_vcoin_map = consignment_manager.GetVCoinMap();

	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}

class PushDBEventLoadCouple : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadCouple >
{
public:
	DBManager* manager;
	OGSLib::ManagerCouple couple_manager;
	int GUID;
	int result;
	PushDBEventLoadCouple():couple_manager(0)
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->LoadCouples(couple_manager);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnLoadCouples(GUID,0,couple_manager);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnLoadCouples(GUID,1,couple_manager);
			}
			manager->m_App->m_CouplesLoaded=true;
        }
        delete this;
	}
};
int DBManager::push_load_couple(int GUID)
{
	PushDBEventLoadCouple* e = new PushDBEventLoadCouple;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
	return 0;
}
class PushDBEventSaveCouple : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveCouple >
{
public:
	DBManager* manager;
	OGSLib::ManagerCouple couple_manager;
	int GUID;
	int result;
	PushDBEventSaveCouple():couple_manager(0)
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->SaveCouples(couple_manager);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnSaveCouples(GUID,0);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnSaveCouples(GUID,1);
			}
			couple_manager.dest(false);
        }
        delete this;
	}
};
int DBManager::push_save_couple(const OGSLib::ManagerCouple& couple_manager,int GUID)
{
	PushDBEventSaveCouple* e = new PushDBEventSaveCouple;
	e->GUID = GUID;
	e->manager = this;
	OGSLib::ManagerCouple::CoupleMap m;
	for( OGSLib::ManagerCouple::CoupleMap::const_iterator couple_pos = couple_manager.GetCouples().begin();
		couple_pos != couple_manager.GetCouples().end();
		++ couple_pos )
	{
		OGSLib::Couple* g = new OGSLib::Couple;
		*g = (*couple_pos->second);
		m[g->CoupleID()] = g;
	}
	e->couple_manager.OnLoadCouples(m);
	m_pDBThread->PushEvent(e);
	return 0;
}
class PushDBEventLoadServerVar : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadServerVar >
{
public:
	DBManager* manager;
	int GUID;
	int result;
	std::map<std::string,std::string> vars;
	PushDBEventLoadServerVar()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->LoadServerVar(vars);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnLoadServerVar(GUID,0,vars);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnLoadServerVar(GUID,1,vars);
			}
			manager->m_App->m_ServerVarLoaded=true;
        }
        delete this;
	}
};
int DBManager::push_load_server_var(int GUID)
{
	PushDBEventLoadServerVar* e = new PushDBEventLoadServerVar;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
	return 0;
}
class PushDBEventLoadAuto : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadAuto >
{
public:
	DBManager* manager;
	int GUID;
	int result;
	std::vector<OGSLib::AutoRunDB> mRes;
	PushDBEventLoadAuto()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->LoadAuto(&mRes);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			manager->m_App->m_pGameServerLib->OnLoadAuto(GUID,result,&mRes);
        }
        delete this;
	}
};
int DBManager::push_load_auto(int GUID)
{
	PushDBEventLoadAuto* e = new PushDBEventLoadAuto;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
	return 0;
}
class PushDBEventStoreSql : public LEUD::EventBase,public LEUD::PoolMemoryBase<PushDBEventStoreSql>
{
public:
	DBManager* manager;
	std::string sql;
	void* arg;
	int result;
	long effect_row_num;
	long new_insert_id;
	sexy_db_mysql::mem_db_res res;
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->game_store_sql(sql.c_str(),res,&effect_row_num,&new_insert_id);
			manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
            return;
		}
		if( queue_id == 200 )
		{
			manager->m_App->OnStoreSql(arg,sql.c_str(),result,res,effect_row_num,new_insert_id);
			sexy_db_mysql::mem_db_res::destroy(res);
        }
        delete this;
	}
};
int DBManager::push_store_sql(void* arg,const char* sql){
	PushDBEventStoreSql* e = new PushDBEventStoreSql;
	e->arg=arg;
	e->sql = sql;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}
class PushDBEventSaveServerVar : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveServerVar >
{
public:
	DBManager* manager;
	int GUID;
	int result;
	std::map<std::string,std::string> vars;
	PushDBEventSaveServerVar()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id==100 )
		{
			result = manager->SaveServerVar(vars);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id==200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnSaveServerVar(GUID,0);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnSaveServerVar(GUID,1);
			}
        }
        delete this;
	}
};
int DBManager::push_save_server_var(const std::map<std::string,std::string>& vars,int GUID)
{
	PushDBEventSaveServerVar* e = new PushDBEventSaveServerVar;
	e->GUID = GUID;
	e->manager = this;
	e->vars = vars;
	m_pDBThread->PushEvent(e);
	return 0;
}
class PushDBEventListCharacter : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventListCharacter >
{
public:
	DBManager* manager;
	SOCKET sock;
	std::string account;
	GS_Client::ListCharacterRes resp;
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			manager->ClientListCharacter(sock,account,resp);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			CCliNetSession* se = (CCliNetSession*)manager->m_App->m_pSessionManager->FindSession(sock);
			if( se )
			{
				se->OnListCharacter(resp);
			}
        }
        delete this;
	}
};
void DBManager::push_list_character(SOCKET sock,std::string account)
{
	PushDBEventListCharacter* e = new PushDBEventListCharacter;
	e->sock = sock;
	e->account = account;
	e->manager = this;
	m_pDBThread->PushEvent(e);
}
class PushDBEventCreateCharacter : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventCreateCharacter >
{
public:
	DBManager* manager;
	SOCKET sock;
	std::string account;
	std::string chrname;
	std::string idfa;
	int pid;
	int job;
	int gender;
	int svrid;
	std::string youke;
	GS_Client::CreateCharacterRes res;
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			manager->ClientCreateCharacter(sock,account,chrname,idfa,pid,job,gender,svrid,youke,res);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			CCliNetSession* se = (CCliNetSession*)manager->m_App->m_pSessionManager->FindSession(sock);
			if( se )
			{
				se->OnCreateCharacter(res);
			}
        }
        delete this;
	}
};
void DBManager::push_create_character(SOCKET sock,std::string account,std::string chrname,std::string idfa,int pid,int job,int gender,int svr,std::string youke)
{
	PushDBEventCreateCharacter* e = new PushDBEventCreateCharacter;
	e->sock = sock;
	e->account = account;
	e->chrname = chrname;
	e->idfa = idfa;
	e->pid = pid;
	e->job = job;
	e->gender = gender;
	e->svrid = svr;
	e->youke = youke;
	e->manager = this;
	m_pDBThread->PushEvent(e);
}
class PushDBEventDeleteCharacter : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventDeleteCharacter >
{
public:
	DBManager* manager;
	std::string account;
	std::string chrname;
	SOCKET sock;
	GS_Client::DeleteCharacterRes res;
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			manager->ClientDeleteCharacter(sock,account,chrname,res);
			manager->m_pEventQueue->PushEvent(this);
            return;
		}
		if( queue_id == 200 )
		{
			CCliNetSession* se = (CCliNetSession*)manager->m_App->m_pSessionManager->FindSession(sock);
			if( se )
			{
				se->OnDeleteCharacter(res);
			}
        }
        delete this;
	}
};
void DBManager::push_delete_character(SOCKET sock,std::string& account,const char* chrname)
{
	PushDBEventDeleteCharacter* e = new PushDBEventDeleteCharacter;
	e->sock = sock;
	e->account = account;
	e->chrname = chrname;
	e->manager = this;
	m_pDBThread->PushEvent(e);
}
class PushDBEventExeSql : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventExeSql >
{
public:
	DBManager* manager;
	std::string sql;
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		manager->game_exe_sql(sql.c_str());
		delete this;
	}
};
int DBManager::push_exe_sql(const char* sql)
{
	PushDBEventExeSql* e = new PushDBEventExeSql;
	e->sql = sql;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
		m_pDBThread->Wake();
#endif
	return 0;
}
class PushDBEventAuthModifyName : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventAuthModifyName >
{
public:
	DBManager* manager;
	std::string seed;
	std::string chrid;
	int result;
	PushDBEventAuthModifyName()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			manager->ClientAuthModifyName(chrid,result);
			manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
            return;
		}
		if( queue_id == 200 )
		{
			manager->m_App->m_pGameServerLib->OnModifyName(seed.c_str(),chrid.c_str(), 0);
        }
        delete this;
	}
};
void DBManager::push_auth_modifyname(const char* seed,const char* chrname)
{
	PushDBEventAuthModifyName* e = new PushDBEventAuthModifyName;
	e->seed = seed;
	e->chrid = chrname;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
}

class PushDBEventLoadMail : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadMail >
{
public:
	DBManager* manager;
	int GUID;
	int result;
	std::vector<OGSLib::Mail*> mailVector;
	int mailValidDays;
	PushDBEventLoadMail()
	{
		result = 0;
		mailValidDays = 15;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->DoLoadMails(mailVector,mailValidDays);
			manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
            return;
		}
		if( queue_id == 200 )
		{
			if( result )
			{
				manager->m_App->m_pGameServerLib->OnLoadMails(result,1,mailVector);
			}
			//else
			//{
			//	manager->m_App->m_pGameServerLib->OnLoadMails(GUID,1,mailVector);
			//}
        }
        delete this;
	}
};
void DBManager::push_load_mail(int GUID,int mailValidDays)
{
	PushDBEventLoadMail* e = new PushDBEventLoadMail;
	e->GUID = GUID;
	e->mailValidDays = mailValidDays;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
}
class PushDBEventSaveMail : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveMail >
{
public:
	DBManager* manager;
	int GUID;
	int result;
	OGSLib::ManagerMail mail_manager;
	PushDBEventSaveMail():mail_manager(0)
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id==100 )
		{
			result = manager->OnSaveMails(mail_manager.GetMails());
			manager->m_pEventQueue->PushEvent(this);
#ifndef WIN32
			manager->m_App->Wake();
#endif
            return;
		}
		if( queue_id==200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnSaveMails(GUID,0);
			}
			//else
			//{
			//	manager->m_App->m_pGameServerLib->OnSaveMails(GUID,1);
			//}
			mail_manager.dest(false);
        }
        delete this;
	}
};
int DBManager::push_save_mail(OGSLib::ManagerMail& mail_manager,int GUID)
{
	PushDBEventSaveMail* e = new PushDBEventSaveMail;
	e->GUID = GUID; 
	e->manager = this;
	for(auto pos = mail_manager.GetMails().begin(); pos != mail_manager.GetMails().end(); ++pos)
	{
		OGSLib::Mail* mail = new OGSLib::Mail;
		*mail = **pos;
		e->mail_manager.GetMails().push_back(mail);
	}
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}

DBManager::DBManager(CGameServerApp* app)
{
	m_App=app;
	m_BufferLen = 1024*1024;
	m_Buffer1 = new char[m_BufferLen];
	m_Buffer2 = new char[m_BufferLen];
	m_PingTime = 0;
	m_pDBModule = 0;
	m_pDBThread = 0;
	m_pEventQueue = 0;
}

DBManager::~DBManager(void)
{
	delete[] m_Buffer1;m_Buffer1=0;
	delete[] m_Buffer2;m_Buffer2=0;
}

bool DBManager::Start()
{
	m_pDBModule = new LEUD::DBModuleMysql;
	char temp[256];
	::GetPrivateProfileString("db","ip","127.0.0.1",temp,256,m_App->m_ConfigFile.c_str());
	std::string ip = temp;
	::GetPrivateProfileString("db","port","3306",temp,256,m_App->m_ConfigFile.c_str());
	std::string port = temp;
	::GetPrivateProfileString("db","db","octgame",temp,256,m_App->m_ConfigFile.c_str());
	std::string db = temp;
	::GetPrivateProfileString("db","user","root",temp,256,m_App->m_ConfigFile.c_str());
	std::string user = temp;
	::GetPrivateProfileString("db","pwd","123456",temp,256,m_App->m_ConfigFile.c_str());
	std::string pwd = temp;
	if( ! m_pDBModule->Create(ip,port,db,user,pwd) )
	{
		printf("Game DB connect failed\n");
		return false;
	}
	{
		std::stringstream ss;
		ss<<"update player set online=0 where online=100 and server_id='"<<m_App->m_ServerID<<"' ";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	{
		std::stringstream ss;
		//ss<<"update demo set online=0 where online=100 and server_id='"<<m_App->m_ServerID<<"' ";
		ss<<"delete from demo where server_id='"<<m_App->m_ServerID<<"'";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	DBUpVersion();
	m_pEventQueue = new LEUD::EventQueue;
	m_pEventQueue->m_ID = 200;
	m_pDBThread = new DBEventQueueThread(this);
	m_pDBThread->m_ID = 100;
	m_pDBThread->Start(0,0);
	return true;
}
void DBManager::DBUpVersion()
{
	int len=0;my_ulonglong eff=0;const char* sql;char temp[256];
	int version=0;
	{
		MYSQL_RES* res = m_pDBModule->QueryStore("select * from dbver");
		if(res){
			MYSQL_ROW row= m_pDBModule->FetchRow(res);
			if( row ){
				version=atoi(m_pDBModule->FetchField(row,res,"versio",len));
			}
		}
		printf("fetch db version %d\n",version);
	}
	if(0==version){
		sql="CREATE TABLE `dbver` (`versio`  int NULL ,PRIMARY KEY (`versio`) )";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true);

		version=200;sprintf(temp,"insert into dbver values ('100')",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(200>=version){
		version=201;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(201>=version){
		sprintf(temp,"ALTER TABLE `guild2` ADD COLUMN  `items` blob DEFAULT NULL AFTER `param`",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
		version=202;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(202==version){
		sql = "ALTER TABLE `player` ADD COLUMN `kuafu_result` INTEGER UNSIGNED DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `kuafu_result` INTEGER UNSIGNED DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=203;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(203==version){
		sql = "ALTER TABLE `player` ADD COLUMN `inner_power` INTEGER UNSIGNED DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `inner_power` INTEGER UNSIGNED DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=204;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==204)///记录玩家已经拥有的装备(用于角色登录界面的显示)
	{
		sql = "ALTER TABLE `player` ADD COLUMN `equip_have` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `equip_have` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=205;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==205)///记录玩家已经拥有的装备(用于角色登录界面的显示)
	{
		sql = "ALTER TABLE `player` ADD COLUMN `item_chattrade` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `item_chattrade` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=206;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==206)///创建寄售表
	{
		sql = "CREATE TABLE `consignment` (`id` int(11) NOT NULL AUTO_INCREMENT,`server_id` int(10) NOT NULL,`chr_seed_name` varchar(255) NOT NULL,`item` blob NOT NULL,`price` int(10) NOT NULL,`end_time` bigint(20) NOT NULL,`type` int(10) NOT NULL,PRIMARY KEY (`id`))";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=207;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==207)///创建社交表,删除角色表friend列
	{
		sql = "CREATE TABLE `relationship` (`id` int(11) NOT NULL AUTO_INCREMENT,`server_id` int(11) NOT NULL,`seed_name` varchar(255) NOT NULL,`rel_seed_name` varchar(255) NOT NULL,`rel_type` int(11) NOT NULL,PRIMARY KEY (`id`))";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		// 
		sql = "ALTER TABLE `player` DROP COLUMN `friend`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		
		sql = "ALTER TABLE `demo` DROP COLUMN `friend`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		version=208;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==208)///创建社交表,删除角色表friend列
	{
		sql = "CREATE TABLE `relation_cache` (`id` int(11) NOT NULL AUTO_INCREMENT,`server_id` int(11) NOT NULL,`name` varchar(255) NOT NULL,`oper_name` varchar(255) NOT NULL,`title` int(11) NOT NULL,`time` int(11) NOT NULL,PRIMARY KEY (`id`))";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=209;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==209)///创建社交表,删除角色表friend列
	{
		sql = "CREATE TABLE `consignment_vcoin` (  `id` int(11) NOT NULL AUTO_INCREMENT,  `server_id` int(11) NOT NULL,  `seed_name` varchar(255) NOT NULL,  `vcoin` int(11) NOT NULL,  `type` int(11) NOT NULL,  PRIMARY KEY (`id`))";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=210;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==210)///行会表增加红包列,物品操作记录列
	{
		sql = "ALTER TABLE `guild2` ADD COLUMN `red_packet_info` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		
		sql = "ALTER TABLE `guild2` ADD COLUMN `item_op_records` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=211;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==211)///行会删除标记列
	{
		sql = "ALTER TABLE `guild2` ADD COLUMN `del_flag` INT DEFAULT 0;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `guildinfo` ADD COLUMN `del_flag` INT DEFAULT 0;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=212;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==212)///行会战
	{
		sql = "ALTER TABLE `guild2` ADD COLUMN `guildwar` blob DEFAULT NULL;";

		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		version=213;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==213)///渠道id
	{
		sql = "ALTER TABLE `player` ADD COLUMN `pid` INTEGER UNSIGNED DEFAULT NULL AFTER `account`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `pid` INTEGER UNSIGNED DEFAULT NULL AFTER `account`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=214;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==214)///设备码
	{
		sql = "ALTER TABLE `player` ADD COLUMN `idfa` varchar(255) DEFAULT NULL AFTER `account`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `idfa` varchar(255) DEFAULT NULL AFTER `account`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=215;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==215)///设备码
	{
		sql = "ALTER TABLE `player` ADD COLUMN `buff` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `buff` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=216;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==216)///修改邮件表主键自增
	{
		sql = "ALTER TABLE `mail` DROP COLUMN `id`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `mail` ADD COLUMN `id` INTEGER UNSIGNED NOT NULL AUTO_INCREMENT FIRST,ADD PRIMARY KEY(id);";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=217;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==217)///GS元宝
	{
		sql = "ALTER TABLE `player` ADD COLUMN `vcoings` INTEGER UNSIGNED DEFAULT 0 AFTER `vcoin`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `vcoings` INTEGER UNSIGNED DEFAULT 0 AFTER `vcoin`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=218;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==218)///GS元宝
	{
		sql = "ALTER TABLE `mail` DROP COLUMN `seedname`";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);
		
		sql = "ALTER TABLE `mail` ADD COLUMN `seed_id` BIGINT DEFAULT 0 AFTER `id`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `mail` ADD COLUMN `deleted` INTEGER UNSIGNED DEFAULT 0 AFTER `seed_id`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=219;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==219)///
	{
		sql = "ALTER TABLE `player` ADD COLUMN `mainTaskState` INTEGER UNSIGNED DEFAULT 0 AFTER `buff`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=220;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==220)///GS邮件
	{
		sql = "ALTER TABLE `mail` ADD COLUMN `gs` INTEGER UNSIGNED DEFAULT 0 AFTER `date`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=221;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==221)///渠道id
	{
		sql = "ALTER TABLE `player` ADD COLUMN `svrid` INTEGER UNSIGNED DEFAULT NULL AFTER `pid`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `svrid` INTEGER UNSIGNED DEFAULT NULL AFTER `pid`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=222;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version == 222)///记录玩家衣橱物品
	{
		sql = "ALTER TABLE `player` ADD COLUMN `item_fashion` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed(); printf("%s\n", sql); LEUD::ConsoleColorIntensity(); m_pDBModule->QueryStore(sql, true, &eff); printf("effect %d rows\n", eff);

		sql = "ALTER TABLE `demo` ADD COLUMN `item_fashion` blob DEFAULT NULL;";
		LEUD::ConsoleColorRed(); printf("%s\n", sql); LEUD::ConsoleColorIntensity(); m_pDBModule->QueryStore(sql, true, &eff); printf("effect %d rows\n", eff);

		version = 223; sprintf(temp, "update dbver set versio='%d'", version); printf("%s\n", temp); m_pDBModule->QueryStore(temp, true);
	}
	if(version==223)///
	{
		sql = "ALTER TABLE `demo` ADD COLUMN `mainTaskState` INTEGER UNSIGNED DEFAULT 0 AFTER `buff`;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=224;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}
	if(version==224)///
	{
		sql = "ALTER TABLE `relationship` ADD INDEX `index_1` (`server_id`) ,ADD INDEX `index_2` (`seed_name`) ;";
		LEUD::ConsoleColorRed();printf("%s\n",sql);LEUD::ConsoleColorIntensity();m_pDBModule->QueryStore(sql,true,&eff);printf("effect %d rows\n",eff);

		version=225;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	}


	//if(108 == version ){
	//	std::map<std::string,std::string> mPersonVars;
	//	std::map<std::string,std::string> mGuildVars;
	//	{
	//		std::stringstream str1;
	//		str1<<"select seedname,chrname from player";
	//		MYSQL_RES* res = m_pDBModule->QueryUse(str1.str().c_str());
	//		if( res )
	//		{
	//			MYSQL_ROW row;
	//			while( row = m_pDBModule->FetchRow(res) )
	//			{
	//				int len;
	//				std::string gname;
	//				std::string gid;
	//				std::string chrname;
	//				std::string seedid;
	//				const char* tp;
	//				tp = m_pDBModule->FetchField(row,res,"seedname",len);
	//				if( tp ){seedid = tp;}
	//				tp = m_pDBModule->FetchField(row,res,"chrname",len);
	//				if( tp ){chrname = tp;}
	//				if( !chrname.empty() && !seedid.empty() ){
	//					mPersonVars[chrname]=seedid;
	//				}
	//			}
	//		}
	//		m_pDBModule->FreeResult(res);
	//	}
	//	std::stringstream ss;
	//	ss<<"select * from guild2 where server_id='"<<m_App->m_ServerID<<"'";
	//	int GuildSeed = 1000;
	//	OGSLib::ManagerGuild::GuildMap gm;
	//	MYSQL_RES* res = m_pDBModule->QueryStore(ss.str().c_str());
	//	if( res )
	//	{
	//		MYSQL_ROW row;
	//		while( row = m_pDBModule->FetchRow(res) )
	//		{
	//			int len;
	//			std::string tguildname;
	//			std::string tguildid;
	//			OGSLib::Guild* guild = new OGSLib::Guild();
	//			const char* tp;
	//			GuildSeed ++;
	//			tguildname = m_pDBModule->FetchField(row,res,"guild_name",len);
	//			tp = m_pDBModule->FetchField(row,res,"guild_id",len);
	//			if(! tp ){
	//				std::string hf = "merge";
	//				int num = rand()%9000 + 1000;
	//				std::stringstream seedid; seedid<<hf<<num<<GuildSeed;
	//				tguildid = seedid.str();
	//			}
	//			else{
	//				tguildid = tp;
	//			}
	//			mGuildVars[tguildname]=tguildid;
	//			{
	//				const char* data = m_pDBModule->FetchField(row,res,"members",len);
	//				if( data && len > 0 )
	//				{
	//					LEUD::StreamFix s(data,len,len);
	//					int version;
	//					int size;
	//					s>>version>>size;

	//					if( s && version==102 && size >0 )
	//					{
	//						LEUD::StreamFix js(m_Buffer1,m_BufferLen,0);
	//						int version=103;
	//						js<<version;
	//						js<<size;

	//						for(int i = 0;i < size;i++)
	//						{
	//							std::string nick_name;
	//							int title;
	//							int fight = 0;
	//							int lastout = 0;
	//							int guildpt = 0;
	//							s>>nick_name;
	//							s>>title;
	//							s>>fight;
	//							s>>lastout;
	//							std::string seed_name = mPersonVars[nick_name];
	//							if( s && !nick_name.empty() )
	//							{
	//								js<<seed_name
	//									<<nick_name
	//									<<title
	//									<<fight
	//									<<lastout
	//									<<guildpt;
	//							}
	//						}
	//						unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)js.size());
	//						std::stringstream jstrs;
	//						jstrs<<"update guild2 set guild_id='"<<tguildid<<"',members='"<<m_Buffer2<<"' where guild_name='"<<tguildname<<"'";
	//						m_pDBModule->QueryStore(jstrs.str().c_str(),true);
	//					}
	//				}
	//			}
	//		}
	//	}
	//	m_pDBModule->FreeResult(res);
	//	{		
	//		for(std::map<std::string,std::string>::iterator pos = mGuildVars.begin();
	//			pos != mGuildVars.end();
	//			++ pos )
	//		{
	//			std::stringstream jstrs;
	//			jstrs<<"update player set guildid='"<<pos->second<<"' where guild='"<<pos->first<<"'";
	//			m_pDBModule->QueryStore(jstrs.str().c_str(),true);
	//		}
	//	}
	//	LEUD::ConsoleColorRed();printf("update guild2 and player db ....\n");LEUD::ConsoleColorIntensity();
	//	version=109;sprintf(temp,"update dbver set versio='%d'",version);printf("%s\n",temp);m_pDBModule->QueryStore(temp,true);
	//}
}
void DBManager::Stop()
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
void DBManager::Update()
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
void DBManager::PingDB()
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
int DBManager::DoFreshGift(const char* chrname,std::vector<OGSLib::SubGift>& giftarray)
{
	_snprintf_s(m_Buffer1,1024*1024,1024*1024,"select * from gift where chrname='%s' and server_id='%d' and used=1",chrname,m_App->m_ServerID);
	MYSQL_RES* res = m_pDBModule->QueryStore(m_Buffer1);
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			OGSLib::SubGift gift;
			gift.mDBID=atoi(m_pDBModule->FetchField(row,res,"id",len));
			gift.mUsed=atoi(m_pDBModule->FetchField(row,res,"used",len));
			const char* dummy;
			dummy = m_pDBModule->FetchField(row,res,"bind",len);
			if( dummy ){ gift.mBind = atoi(dummy); }
			dummy = m_pDBModule->FetchField(row,res,"num",len);
			if( dummy ){ gift.mNum = atoi(dummy); }
			gift.mTypeID=atoi(m_pDBModule->FetchField(row,res,"typeid",len));
			strncpy_s(gift.mMsg,m_pDBModule->FetchField(row,res,"msg",len),63);
			gift.mMsg[63]=0;
			giftarray.push_back(gift);
		}
		m_pDBModule->FreeResult(res);
	}
	return 0;
}

int DBManager::OnSaveMails(std::vector<OGSLib::Mail*>& mailList)
{
	char temp[1024];
	char desp[1024];desp[0]=0;
	bool isEmpty = true;
	//int mail_id = 0;
	{
		std::stringstream ss;
		ss<<"update mail set deleted = 1 where deleted=0";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}

	for(auto pos = mailList.begin(); pos != mailList.end(); ++pos)
	{
		std::stringstream ss;
		if((*pos)->mDataStatus == 0)
		{
			ss<<"insert into mail set ";
			ss << "seed_id='" << (*pos)->mID;
			ss << "',deleted=0";
            m_pDBModule->EscapeString(temp, (*pos)->mTitle.c_str(), (unsigned long)(*pos)->mTitle.length());
			ss<<",title='"<<temp;
            m_pDBModule->EscapeString(temp, (*pos)->mContent.c_str(), (unsigned long)(*pos)->mContent.length());
			ss<<"',content='"<<temp;
			ss << "',gs='" << (*pos)->mGS;
            ss << "',date='" << (*pos)->mDate;
            m_pDBModule->EscapeString(temp, (*pos)->mOwners.c_str(), (unsigned long)(*pos)->mOwners.length());
			ss<<"',owners='"<<temp;
			{
				LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
				int version=100;
				s<<version;
                s << static_cast<int>((*pos)->mItems.size());
                for (std::map<int, int>::iterator item_pos = (*pos)->mItems.begin();
                     item_pos != (*pos)->mItems.end();
					++ item_pos )
				{
					s<<item_pos->first<<item_pos->second;
				}
				unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
				ss<<"',items='"<<m_Buffer2<<"';";
			}
			isEmpty = false;
			m_pDBModule->QueryStore(ss.str().c_str(),true);
		}
        else if ((*pos)->mDataStatus == 1)
		{
            ss << "delete from mail where seed_id='" << (*pos)->mID << "';";
			isEmpty = false;
			m_pDBModule->QueryStore(ss.str().c_str(),true);
		}
	}
	{
		std::stringstream ssSql;
		ssSql<<"delete from mail where deleted=1";
		m_pDBModule->QueryStore(ssSql.str().c_str(),true);
	}
	if(!isEmpty)
	{
		//m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	return 0;
}
int DBManager::DoLoadMails(std::vector<OGSLib::Mail*>& mailVector,int mailValidDays)
{
	int mail_gen = 0;
	{
		std::stringstream ssSql;
		ssSql<<"delete from mail where "<<time(0)<<" - date >= '"<<mailValidDays*24*60*60<<"'";
		m_pDBModule->QueryStore(ssSql.str().c_str(),true);
	}
	{
		std::stringstream ss;
		ss<<"select * from mail where deleted = 0";
		MYSQL_RES* res = m_pDBModule->QueryUse(ss.str().c_str());
		if( res )
		{
			MYSQL_ROW row;
			while( row = m_pDBModule->FetchRow(res) )
			{
				OGSLib::Mail* mail=new OGSLib::Mail();
				int len;
				const char* tp;
				//tp = m_pDBModule->FetchField(row,res,"id",len);
				//if( tp ) mail->mDate = atoi(tp);
				tp = m_pDBModule->FetchField(row,res,"seed_id",len);
				char * chEnd = NULL;
				if(tp) mail->mID = _strtoi64(tp, &chEnd, 10);
// 				if(mail->mID>mail_gen)
// 				{
// 					mail_gen = mail->mID;
// 				}
				tp = m_pDBModule->FetchField(row,res,"owners",len);
				if( tp ) mail->mOwners = std::string(tp);
				tp = m_pDBModule->FetchField(row,res,"title",len);
				if( tp ) mail->mTitle = std::string(tp);
				tp = m_pDBModule->FetchField(row,res,"content",len);
				if( tp ) mail->mContent = std::string(tp);
				tp = m_pDBModule->FetchField(row,res,"date",len);
				if(tp) mail->mDate = atoi(tp);
				tp = m_pDBModule->FetchField(row,res,"gs",len);
				if(tp) mail->mGS = atoi(tp);
				{
					const char* data = m_pDBModule->FetchField(row,res,"items",len);
					if( data && len > 0 )
					{
						LEUD::StreamFix s(data,len,len);
						int version;
						int size;
						s>>version>>size;
						if( s && version==100 && size >0 )
						{
							for(int i = 0;i < size;i++)
							{
								int typeID;int num;
								s>>typeID>>num;
								if( s )
								{
									mail->mItems[typeID] = num;
								}
							}
						}
					}
				}
				mailVector.push_back(mail);
			}
			m_pDBModule->FreeResult(res);
		}
	}
	return 1;
}
class DataFetcher_DB : public DataFetcher{
public:
	LEUD::DBModuleMysql* m_pDBModule;
	MYSQL_RES* res;
	MYSQL_ROW  row;
	virtual const char* fetch_field(const char* fn,int& len){
		return m_pDBModule->FetchField(row,res,fn,len);
	}
};
void DBManager::UndumpKuafuCharacter(OGSLib::DomainData& chr,DataFetcher* fetcher){
	int len;
	const char* dummy;
	chr.level = atoi(fetcher->fetch_field("lv",len));
	chr.exp = _atoi64(fetcher->fetch_field("exp",len));
	chr.vcoin = atoi(fetcher->fetch_field("vcoin",len));
	chr.vcoings = atoi(fetcher->fetch_field("vcoings",len));
	chr.job = atoi(fetcher->fetch_field("job",len));
	chr.gender = atoi(fetcher->fetch_field("gender",len));
	dummy = fetcher->fetch_field("guild",len);
	if( dummy ){ strncpy_s(chr.mGuildName,_countof(chr.mGuildName),dummy,255);chr.mGuildName[255]=0;}
	dummy = fetcher->fetch_field("seedname",len);
	if( dummy ){ strncpy_s(chr.mSeedName,_countof(chr.mSeedName),dummy,255);chr.mSeedName[255]=0;}
	else{
		chr.mSeedName[0]=0;
	}
	{
		const char* data = fetcher->fetch_field("item",len);
		if( data && len > 0 )
		{
			LEUD::StreamFix s(data,len,len);
			int version;
			int size;
			s>>version>>size;
			chr.mItemDataVersion = version;
			if( s && size >0 )
			{
				if( version == 115 )
				{
					{
						const char* KEYS[4]={"item_bag","item_depot1","item_depot2","item_lottery"};
						for(short key=0;key<4;key++)
						{
							const char* data = fetcher->fetch_field(KEYS[key],len);
							if( data && len > 0 )
							{
								LEUD::StreamFix s(data,len,len);
								int size;
								s>>size;
								if( s && size >0 )
								{
									for(int i = 0;i < size;i++)
									{
										OGSLib::SubItem ii;
										s>>ii.mPosition
											>>ii.mTypeID
											>>ii.mDuration
											>>ii.mDuraMax
											>>ii.mItemFlags
											>>ii.mLuck
											>>ii.mLevel
											>>ii.mNumber
											>>ii.mAddAC
											>>ii.mAddMAC
											>>ii.mAddDC
											>>ii.mAddMC
											>>ii.mAddSC
											>>ii.mUpdAC
											>>ii.mUpdMAC
											>>ii.mUpdDC
											>>ii.mUpdMC
											>>ii.mUpdSC
											>>ii.mCreateTime
											>>ii.mIdentId
											>>ii.mProtect
											>>ii.mAddHp
											>>ii.mAddMp
											>>ii.mUpdMaxCount
											>>ii.mUpdFailedCount
											>>ii.mSellPriceType
											>>ii.mSellPrice
											>>ii.mZLevel
                                            >>ii.mLock;
										if( s )
										{
											if(ii.mNumber<=0)ii.mNumber = 1;
											OGSLib::ItemDef* id = m_App->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(ii.mTypeID);
											if (id)
											{
												ii.mSubType = id->SubType;
											}
											chr.mItemList.push_back(ii);
										}
									}
								}
							}
						}
					}
				}
				if( version == 113 )
				{
					{
						const char* KEYS[4]={"item_bag","item_depot1","item_depot2","item_lottery"};
						for(short key=0;key<4;key++)
						{
							const char* data = fetcher->fetch_field(KEYS[key],len);
							if( data && len > 0 )
							{
								LEUD::StreamFix s(data,len,len);
								int size;
								s>>size;
								if( s && size >0 )
								{
									for(int i = 0;i < size;i++)
									{
										OGSLib::SubItem ii;
										s>>ii.mPosition
											>>ii.mTypeID
											>>ii.mDuration
											>>ii.mDuraMax
											>>ii.mItemFlags
											>>ii.mLuck
											>>ii.mLevel
											>>ii.mNumber
											>>ii.mAddAC
											>>ii.mAddMAC
											>>ii.mAddDC
											>>ii.mAddMC
											>>ii.mAddSC
											>>ii.mUpdAC
											>>ii.mUpdMAC
											>>ii.mUpdDC
											>>ii.mUpdMC
											>>ii.mUpdSC
											>>ii.mCreateTime
											>>ii.mIdentId
											>>ii.mProtect
											>>ii.mAddHp
											>>ii.mAddMp
											>>ii.mUpdMaxCount
											>>ii.mUpdFailedCount
											>>ii.mSellPriceType
											>>ii.mSellPrice;
										if( s )
										{
											if(ii.mNumber<=0)ii.mNumber = 1;
											OGSLib::ItemDef* id = m_App->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(ii.mTypeID);
											if (id)
											{
												ii.mSubType = id->SubType;
											}
											chr.mItemList.push_back(ii);
										}
									}
								}
							}
						}
					}
				}
				if( version == 112 )
				{
					{
						const char* KEYS[4]={"item_bag","item_depot1","item_depot2","item_lottery"};

						for(short key=0;key<4;key++)
						{
							const char* data = fetcher->fetch_field(KEYS[key],len);
							if( data && len > 0 )
							{
								LEUD::StreamFix s(data,len,len);
								int size;
								s>>size;
								if( s && size >0 )
								{
									for(int i = 0;i < size;i++)
									{
										OGSLib::SubItem ii;
										s>>ii.mPosition
											>>ii.mTypeID
											>>ii.mDuration
											>>ii.mDuraMax
											>>ii.mItemFlags
											>>ii.mLuck
											>>ii.mLevel
											>>ii.mNumber
											>>ii.mAddAC
											>>ii.mAddMAC
											>>ii.mAddDC
											>>ii.mAddMC
											>>ii.mAddSC
											>>ii.mUpdAC
											>>ii.mUpdMAC
											>>ii.mUpdDC
											>>ii.mUpdMC
											>>ii.mUpdSC
											>>ii.mCreateTime
											>>ii.mIdentId
											>>ii.mProtect
											>>ii.mAddHp
											>>ii.mAddMp
											>>ii.mUpdFailedCount
											>>ii.mSellPriceType
											>>ii.mSellPrice;
										if( s )
										{
											if(ii.mNumber<=0)ii.mNumber = 1;
											OGSLib::ItemDef* id = m_App->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(ii.mTypeID);
											if (id)
											{
												ii.mSubType = id->SubType;
											}
											chr.mItemList.push_back(ii);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	{
		const char* data = fetcher->fetch_field("skill",len);
		if( data && len > 0 )
		{
			LEUD::StreamFix s(data,len,len);
			int version;
			int size;
			s>>version>>size;
			if( s && version==101 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					OGSLib::SubSkill si;
					s>>si.mTypeID>>si.mLevel>>si.mExp;
					if( s )
					{
						chr.mSkillList.push_back(si);
					}
				}
			}
			else if( s && version==102 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					OGSLib::SubSkill si;
					s>>si.mTypeID>>si.mLevel>>si.mExp>>si.mParam1;
					if( s )
					{
						chr.mSkillList.push_back(si);
					}
				}
			}
		}
	}
	//{
	//	const char* data = m_pDBModule->FetchField(row,res,"reincarnation",len);
	//	if( data && len > 0 )
	//	{
	//		LEUD::StreamFix s(data,len,len);
	//		int version;
	//		s>>version;
	//		if( s && version==100 )
	//		{
	//			s>>chr.ReincarnateAttributes()->mDCMax
	//				>>chr.ReincarnateAttributes()->mMCMax
	//				>>chr.ReincarnateAttributes()->mSCMax
	//				>>chr.ReincarnateAttributes()->mACMax
	//				>>chr.ReincarnateAttributes()->mMACMax
	//				>>chr.ReincarnateAttributes()->mAccuracy
	//				>>chr.ReincarnateAttributes()->mDodge
	//				>>chr.ReincarnateAttributes()->mMaxHp
	//				>>chr.ReincarnateAttributes()->mMaxMp
	//				>>chr.ReincarnateAttributes()->mDropProb
	//				>>chr.ReincarnateAttributes()->mDoubleAttProb;
	//		}
	//	}
	//}
	{
		chr.mShortcutMap.clear();
		const char* data = fetcher->fetch_field("shortcut",len);
		if( data && len > 0 )
		{
			LEUD::StreamFix s(data,len,len);
			int version;
			int size;
			s>>version>>size;
			if( s && version==100 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					OGSLib::DomainData::Shortcut sc;
					s>>sc.shortcut_id
						>>sc.type
						>>sc.param;
					if( s )
					{
						chr.mShortcutMap[sc.shortcut_id]=sc;
					}
				}
			}
		}
	}
	{
		const char* data = fetcher->fetch_field("mail",len);
		if( data && len > 0 )
		{
			LEUD::StreamFix s(data,len,len);
			int version;
			int size;
			s>>version>>size;
			if( s && version==100 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					OGSLib::SubMail sm;
					int size2=0;
					s>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mTitle>>sm.mContent>>size2;
					sm.mDeleted = 0;
					for(int j=0;j<size2;j++)
					{
						int id;int number;
						s>>id>>number;
						sm.mItems[id]=number;
					}
					if( s )
					{
						chr.mMailList[sm.mMailDate] = sm;
					}
				}
			}
			if( s && version==101 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					OGSLib::SubMail sm;
					int size2=0;
					s>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>sm.mTitle>>sm.mContent>>size2;
					for(int j=0;j<size2;j++)
					{
						int id;int number;
						s>>id>>number;
						sm.mItems[id]=number;
					}
					if( s )
					{
						chr.mMailList[sm.mMailDate] = sm;
					}
				}
			}
			if( s && version==102 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					OGSLib::SubMail sm;
					int size2=0;
					s>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>size2;
					for(int j=0;j<size2;j++)
					{
						int id;int number;
						s>>id>>number;
						sm.mItems[id]=number;
					}
					if( s )
					{
						chr.mMailList[sm.mMailDate] = sm;
					}
				}
			}
			if( s && version==103 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					OGSLib::SubMail sm;
					int size2=0;
					s>>sm.mID>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>size2;
					for(int j=0;j<size2;j++)
					{
						int id;int number;
						s>>id>>number;
						sm.mItems[id]=number;
					}
					if( s )
					{
						chr.mMailList[sm.mID] = sm;
					}
				}
			}
		}
	}
	{
		chr.mParamMap.clear();
		const char* data = fetcher->fetch_field("var",len);
		if( data && len > 0 )
		{
			LEUD::StreamFix s(data,len,len);
			int version;
			int size;
			s>>version>>size;
			if( s && version==101 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					int k;
					std::string v;
					s>>k>>v;
					if( ! v.empty() )
					{
						chr.mParamMap[k]=v;
					}
				}
			}
		}
	}
	{
		chr.mNumParamMap.clear();
		const char* data = fetcher->fetch_field("intvar",len);
		if( data && len > 0 )
		{
			LEUD::StreamFix s(data,len,len);
			int version;
			int size;
			s>>version>>size;
			if( s && version==101 && size >0 )
			{
				for(int i = 0;i < size;i++)
				{
					int k;
					int v;
					s>>k>>v;
					chr.mNumParamMap[k]=v;
				}
			}
		}
	}
	{
		chr.mStatusMap.clear();
		const char* data = fetcher->fetch_field("status",len);
		if( data && len > 0 )
		{
			LEUD::StreamFix s(data,len,len);
			int version;
			int size;
			s>>version>>size;
			if( s && version==102 && size > 0 )
			{
				for(int i =0;i<size;i++)
				{
					int id;
					int param;
					int duration;
					int gap;
					int flags;
					s>>id
						>>param
						>>duration
						>>gap
						>>flags;
					OGSLib::SubStatus ss;
					ss.StatusID = id;
					ss.Param = param;
					ss.Duration = duration;
					ss.Gap = gap;
					ss.Flags = flags;
					ss.Tick = 0;
					chr.mStatusMap[ss.StatusID] = ss;
				}
			}
		}
	}
	{
		// 保存buff
	}
}
int DBManager::DoLoadCharacter(const char* act,const char* cnm,const char* snm,OGSLib::DomainData& chr,const char* table)
{
	char temp[513];
	char account[256];
	m_pDBModule->EscapeString(temp,act,(unsigned long)strlen(act));
	strncpy_s(account,_countof(account),temp,255);
	char chrname[256];
	m_pDBModule->EscapeString(temp,cnm,(unsigned long)strlen(cnm));
	strncpy_s(chrname,_countof(chrname),temp,255);
	char seedname[256];
	m_pDBModule->EscapeString(temp,snm,(unsigned long)strlen(snm));
	strncpy_s(seedname,_countof(seedname),temp,255);

	if( 0==strcmp(table,"player") )
	{
		this->LoadChinaLimit(account,&chr.china_limit_online_time_count,&chr.china_limit_offline_time_tag,&chr.china_id_ok,chr.china_nm,chr.china_id);
		if( chr.china_id_ok == 101 )
		{
			chr.china_limit_offline_time_tag = 0;
			chr.china_limit_online_time_count = 0;
		}
		else
		{
			if( chr.china_limit_offline_time_tag + 5*60*60  < time(0) )
			{
				chr.china_limit_offline_time_tag = 0;
				chr.china_limit_online_time_count = 0;
			}
		}
	}
	else
	{
		chr.china_id_ok = 0;
		chr.china_nm[0] = 0;
		chr.china_id[0] = 0;
		chr.china_limit_online_time_count = 0;
		chr.china_limit_offline_time_tag = 0;
	}

	int loadresult=0;
	strncpy_s(chr.mAccount,_countof(chr.mAccount),act,255);
	chr.mAccount[255]=0;
	strncpy_s(chr.mChrName,_countof(chr.mChrName),cnm,255);
	chr.mChrName[255]=0;
	std::stringstream ss;
	ss<<"update "<<table<<" set online=100,last_login_time=now(),last_login_ip='"<<chr.mLoginIP<<"' where chrname='"<<chrname;
#ifndef LIMIT_RELEASE
	ss<<"' and account='"<<account;
#endif
	ss<<"' and server_id='"<<m_App->m_ServerID<<"'";// and online=0";
	my_ulonglong et;
	m_pDBModule->QueryStore(ss.str().c_str(),true,&et);
	if( et == 1 )
	{
		std::stringstream ss;
		ss<<"select *,unix_timestamp(create_time) as ct,unix_timestamp(last_login_time) as llt,unix_timestamp(last_logout_time) as lot from "<<table<<" where chrname='"<<chrname;
#ifndef LIMIT_RELEASE
		ss<<"' and account='"<<account;
#endif
		ss<<"' and server_id='"<<m_App->m_ServerID<<"' and online=100";
		MYSQL_RES* res = m_pDBModule->QueryStore(ss.str().c_str());
		if( res )
		{
			MYSQL_ROW row = m_pDBModule->FetchRow(res);
			if( row )
			{
				int len;
				if( m_App->m_ServerID == atoi(m_pDBModule->FetchField(row,res,"server_id",len)) )
				{
					chr.level = atoi(m_pDBModule->FetchField(row,res,"lv",len));
					chr.exp = _atoi64(m_pDBModule->FetchField(row,res,"exp",len));
					chr.money = _atoi64(m_pDBModule->FetchField(row,res,"money",len));
					chr.moneybind = _atoi64(m_pDBModule->FetchField(row,res,"money_bind",len));
					chr.vcoin = atoi(m_pDBModule->FetchField(row,res,"vcoin",len));
					chr.vcoings = atoi(m_pDBModule->FetchField(row,res,"vcoings",len));
					chr.vcoinbind = atoi(m_pDBModule->FetchField(row,res,"vcoin_bind",len));
					chr.vcoinaccu = atoi(m_pDBModule->FetchField(row,res,"vcoin_accu",len));
					chr.job = atoi(m_pDBModule->FetchField(row,res,"job",len));
					chr.gender = atoi(m_pDBModule->FetchField(row,res,"gender",len));
					chr.pkvalue = atoi(m_pDBModule->FetchField(row,res,"pkvalue",len));
					chr.talk_prohibited = atoi(m_pDBModule->FetchField(row,res,"chatoff",len));
					chr.depot_slot_add = atoi(m_pDBModule->FetchField(row,res,"depotslotadd",len));
					chr.bag_slot_add = atoi(m_pDBModule->FetchField(row,res,"bagslotadd",len));
					chr.mMainTaskState = atoi(m_pDBModule->FetchField(row,res,"mainTaskState",len));
					const char* dummy;
					//dummy = m_pDBModule->FetchField(row,res,"vip_level",len);
					//if(dummy){chr.viplevel=atoi(dummy);}
					//dummy = m_pDBModule->FetchField(row,res,"capacity",len);
					//if(dummy){chr.mCapacity=atoi(dummy);}
					//dummy=m_pDBModule->FetchField(row,res,"reinlevel",len);
					//if(dummy){chr.mReincarnatedLevel=atoi(dummy);}
					//dummy = m_pDBModule->FetchField(row,res,"tradelimit",len);
					//if( dummy ){ chr.mLockTradeLimit = atoi(dummy);}

					dummy=m_pDBModule->FetchField(row,res,"guildpt",len);
					if(dummy){chr.mGuildPt=atoi(dummy);}
					dummy=m_pDBModule->FetchField(row,res,"guildspt",len);
					if(dummy){chr.mGuildSpt=atoi(dummy);}

					dummy=m_pDBModule->FetchField(row,res,"inner_power",len);
					if(dummy){chr.cur_power=atoi(dummy);}
					//dummy = m_pDBModule->FetchField(row,res,"freedirectfly",len);
					//if( dummy ){ chr.mFreeDirectFly = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"lastvcoinday",len);
					if( dummy ){ chr.mLastVcoinUsedDay = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"vcoinbyday",len);
					if( dummy ){ chr.mVcoinUsedByDay = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"vcoinbymonth",len);
					if( dummy ){ chr.mVcoinUsedByMonth = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"lastczhimonth",len);
					if( dummy ){ chr.mLastCongzhiMonth = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"czhibymonth",len);
					if( dummy ){ chr.mVcoinAccuByMonth = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"guild",len);
					if( dummy ){ strncpy_s(chr.mGuildName,_countof(chr.mGuildName),dummy,255);chr.mGuildName[255]=0;}
					dummy = m_pDBModule->FetchField(row,res,"guildid",len);
					if( dummy ){ strncpy_s(chr.mGuildID,_countof(chr.mGuildID),dummy,255);chr.mGuildID[255]=0;}
					dummy = m_pDBModule->FetchField(row,res,"ct",len);
					if( dummy ){ chr.create_time = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"llt",len);
					if( dummy ){ chr.last_login_time = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"last_login_ip",len);
					if( dummy ){ strncpy_s(chr.last_login_ip,_countof(chr.last_login_ip),dummy,255);chr.last_login_ip[255]=0;}
					dummy = m_pDBModule->FetchField(row,res,"lot",len);
					if( dummy ){ chr.last_logout_time = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"emap",len);
					if( dummy ){ strncpy_s(chr.exit_map,_countof(chr.exit_map),dummy,255);chr.exit_map[255]=0;}
					dummy = m_pDBModule->FetchField(row,res,"seedname",len);
					if( dummy ){ strncpy_s(chr.mSeedName,_countof(chr.mSeedName),dummy,255);chr.mSeedName[255]=0;}
					else{
						if(strlen(seedname)!=0)
						{
							strncpy_s(chr.mSeedName,_countof(chr.mSeedName),seedname,255);chr.mSeedName[255]=0;
							_snprintf_s(m_Buffer1,1024*1024,1024*1024,"update player set seedname='%s' where chrname= '%s'",seedname,chr.mChrName);
							m_pDBModule->QueryStore(m_Buffer1,true);
						}
					}
					//dummy = m_pDBModule->FetchField(row,res,"couple_id",len);
					//if( dummy ){ chr.mCoupleID = atoi(dummy);}
					//dummy = m_pDBModule->FetchField(row,res,"weddingdate",len);
					//if( dummy ){ chr.mWeddingdate = atoi(dummy);}
					//dummy = m_pDBModule->FetchField(row,res,"marrytimes",len);
					//if( dummy ){ chr.mMarryTimes = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"team_id",len);
					if( dummy ){ chr.mHonor = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"ot_all",len);
					if( dummy ){ chr.online_time_all = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"ot_today",len);
					if( dummy ){ chr.online_time_today = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"ot_today_tag",len);
					if( dummy ){ chr.online_tag_today = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"ot_yeday",len);
					if( dummy ){ chr.online_time_yestoday = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"ot_yeday_tag",len);
					if( dummy ){ chr.online_tag_yestoday = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"offline_time",len);
					if( dummy ){ chr.offline_time = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"offline_tag",len);
					if( dummy ){ chr.offline_tag = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"achievept",len);
					if( dummy ){ chr.mAchievePoint = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"achievegmmax",len);
					if( dummy ){ chr.mAchieveGameMoneyMax = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"onlinetimemax",len);
					if( dummy ){ chr.mOnlineTimeMax = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"logindaycontinue",len);
					if( dummy ){ chr.mLoginDayContinue = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"logindaycontinuemax",len);
					if( dummy ){ chr.mLoginDayContinueMax = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"logindaycount",len);
					if( dummy ){ chr.mLoginDayCount = atoi(dummy);}

					//int china_ok = 0;
					//dummy = m_pDBModule->FetchField(row,res,"chinaok",len);
					//if( dummy ){ china_ok = atoi(dummy); }
					//if( china_ok == 101 || china_ok == 102)
					//{
					//	chr.china_id_ok = china_ok;
					//	dummy = m_pDBModule->FetchField(row,res,"chinaid",len);
					//	if( dummy ){ strncpy_s(chr.china_id,_countof(chr.china_id),dummy,255);chr.china_id[255]=0; }
					//	dummy = m_pDBModule->FetchField(row,res,"chinanm",len);
					//	if( dummy ){ strncpy_s(chr.china_nm,_countof(chr.china_nm),dummy,255);chr.china_nm[255]=0; }
					//}

					dummy = m_pDBModule->FetchField(row,res,"cur_hp",len);
					if( dummy ){ chr.cur_hp = atoi(dummy);}
					dummy = m_pDBModule->FetchField(row,res,"cur_mp",len);
					if( dummy ){ chr.cur_mp = atoi(dummy);}

					chr.exit_x = atoi(m_pDBModule->FetchField(row,res,"emapx",len));
					chr.exit_y = atoi(m_pDBModule->FetchField(row,res,"emapy",len));
					dummy = m_pDBModule->FetchField(row,res,"safemap",len);
					if( dummy ){ strncpy_s(chr.safe_map,_countof(chr.safe_map),dummy,255);chr.safe_map[255]=0;}
					chr.safe_x = atoi(m_pDBModule->FetchField(row,res,"safemapx",len));
					chr.safe_y = atoi(m_pDBModule->FetchField(row,res,"safemapy",len));
					{
						const char* data = m_pDBModule->FetchField(row,res,"task",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && size >0 )
							{
								if( version==102 )
								{
									for(int i = 0;i < size;i++)
									{
										OGSLib::SubTask ti;
										s>>ti.TaskID
											>>ti.TaskState
											>>ti.TaskParam1
											>>ti.TaskParam2
											>>ti.TaskParam3
											>>ti.TaskParam4;
										if( s )
										{
											chr.mTaskMap[ti.TaskID]=ti;
										}
									}
								}
							}
						}
					}
					{
						const char* data = m_pDBModule->FetchField(row,res,"item",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							chr.mItemDataVersion = version;
							if( s && size >0 )
							{
								if (version == 115 || version == 116 || version == 117)
								{
									{
										const char* KEYS[6]={"item_bag","item_depot1","item_depot2","item_lottery","item_chattrade","item_fashion"};
										for(short key=0;key<6;key++)
										{
											const char* data = m_pDBModule->FetchField(row,res,KEYS[key],len);
											if( data && len > 0 )
											{
												LEUD::StreamFix s(data,len,len);
												int size;
												s>>size;
												if( s && size >0 )
												{
													for(int i = 0;i < size;i++)
													{
														OGSLib::SubItem ii;
														if (version == 116)
														{
															s>>ii.mPosition
																>>ii.mTypeID
																>>ii.mDuration
																>>ii.mDuraMax
																>>ii.mItemFlags
																>>ii.mLuck
																>>ii.mLevel
																>>ii.mNumber
																>>ii.mAddAC
																>>ii.mAddMAC
																>>ii.mAddDC
																>>ii.mAddMC
																>>ii.mAddSC
																>>ii.mUpdAC
																>>ii.mUpdMAC
																>>ii.mUpdDC
																>>ii.mUpdMC
																>>ii.mUpdSC
																>>ii.mCreateTime
																>>ii.mIdentId
																>>ii.mProtect
																>>ii.mAddHp
																>>ii.mAddMp
																>>ii.mUpdMaxCount
																>>ii.mUpdFailedCount
																>>ii.mSellPriceType
																>>ii.mSellPrice
																>>ii.mZLevel
																>>ii.mLock;
									
														}
														else if (version == 117)
														{
															s >> ii.mPosition
																>> ii.mTypeID
																>> ii.mDuration
																>> ii.mDuraMax
																>> ii.mItemFlags
																>> ii.mLuck
																>> ii.mLevel
																>> ii.mNumber
																>> ii.mAddAC
																>> ii.mAddMAC
																>> ii.mAddDC
																>> ii.mAddMC
																>> ii.mAddSC
																>> ii.mUpdAC
																>> ii.mUpdMAC
																>> ii.mUpdDC
																>> ii.mUpdMC
																>> ii.mUpdSC
																>> ii.mCreateTime
																>> ii.mIdentId
																>> ii.mProtect
																>> ii.mAddHp
																>> ii.mAddMp
																>> ii.mUpdMaxCount
																>> ii.mUpdFailedCount
																>> ii.mSellPriceType
																>> ii.mSellPrice
																>> ii.mZLevel
																>> ii.mLock
																>> ii.mLasttime;
														}
														else
														{
															int ident = 0;
															s>>ii.mPosition
																>>ii.mTypeID
																>>ii.mDuration
																>>ii.mDuraMax
																>>ii.mItemFlags
																>>ii.mLuck
																>>ii.mLevel
																>>ii.mNumber
																>>ii.mAddAC
																>>ii.mAddMAC
																>>ii.mAddDC
																>>ii.mAddMC
																>>ii.mAddSC
																>>ii.mUpdAC
																>>ii.mUpdMAC
																>>ii.mUpdDC
																>>ii.mUpdMC
																>>ii.mUpdSC
																>>ii.mCreateTime
																>>ident
																>>ii.mProtect
																>>ii.mAddHp
																>>ii.mAddMp
																>>ii.mUpdMaxCount
																>>ii.mUpdFailedCount
																>>ii.mSellPriceType
																>>ii.mSellPrice
																>>ii.mZLevel
																>>ii.mLock;
															ii.mIdentId = ident;
														}
														if( s && key <= 3)
														{
															if(ii.mNumber<=0)ii.mNumber = 1;
															OGSLib::ItemDef* id = m_App->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(ii.mTypeID);
															if (id)
															{
																ii.mSubType = id->SubType;
															}
															chr.mItemList.push_back(ii);
														}
														else if(s && key == 4)
														{
															if(ii.mNumber<=0)ii.mNumber = 1;
															if(ii.mNumber<=0)ii.mNumber = 1;
															OGSLib::ItemDef* id = m_App->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(ii.mTypeID);
															if (id)
															{
																ii.mSubType = id->SubType;
															}
															chr.mChatTradeItemList.push_back(ii);
														}
														else if (s && key == 5)
														{
															if (ii.mNumber <= 0)ii.mNumber = 1;
															if (ii.mNumber <= 0)ii.mNumber = 1;
															OGSLib::ItemDef* id = m_App->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(ii.mTypeID);
															if (id)
															{
																ii.mSubType = id->SubType;
															}
															chr.mItemList.push_back(ii);
														}
													}
												}
											}
										}
									}
								}
								//if( version == 114 )
								//{
								//	{
								//		const char* KEYS[6]={"item_bag","item_depot1","item_depot2","item_lottery","item_chattrade","fashion_have"};
								//		for(short key=0;key<4;key++)
								//		{
								//			const char* data = m_pDBModule->FetchField(row,res,KEYS[key],len);
								//			if( data && len > 0 )
								//			{
								//				LEUD::StreamFix s(data,len,len);
								//				int size;
								//				s>>size;
								//				if( s && size >0 )
								//				{
								//					for(int i = 0;i < size;i++)
								//					{
								//						OGSLib::SubItem ii;
								//						s>>ii.mPosition
								//							>>ii.mTypeID
								//							>>ii.mDuration
								//							>>ii.mDuraMax
								//							>>ii.mItemFlags
								//							>>ii.mLuck
								//							>>ii.mLevel
								//							>>ii.mNumber
								//							>>ii.mAddAC
								//							>>ii.mAddMAC
								//							>>ii.mAddDC
								//							>>ii.mAddMC
								//							>>ii.mAddSC
								//							>>ii.mUpdAC
								//							>>ii.mUpdMAC
								//							>>ii.mUpdDC
								//							>>ii.mUpdMC
								//							>>ii.mUpdSC
								//							>>ii.mCreateTime
								//							>>ii.mIdentID
								//							>>ii.mProtect
								//							>>ii.mAddHp
								//							>>ii.mAddMp
								//							>>ii.mUpdMaxCount
								//							>>ii.mUpdFailedCount
								//							>>ii.mSellPriceType
								//							>>ii.mSellPrice
								//							>>ii.mZLevel;
								//						if( s )
								//						{
								//							if(ii.mNumber<=0)ii.mNumber = 1;
								//							chr.mItemList.push_back(ii);
								//						}
								//					}
								//				}
								//			}
								//		}
								//	}
								//}
								//if( version == 113 )
								//{
								//	{
								//		const char* KEYS[4]={"item_bag","item_depot1","item_depot2","item_lottery"};
								//		for(short key=0;key<4;key++)
								//		{
								//			const char* data = m_pDBModule->FetchField(row,res,KEYS[key],len);
								//			if( data && len > 0 )
								//			{
								//				LEUD::StreamFix s(data,len,len);
								//				int size;
								//				s>>size;
								//				if( s && size >0 )
								//				{
								//					for(int i = 0;i < size;i++)
								//					{
								//						OGSLib::SubItem ii;
								//						s>>ii.mPosition
								//							>>ii.mTypeID
								//							>>ii.mDuration
								//							>>ii.mDuraMax
								//							>>ii.mItemFlags
								//							>>ii.mLuck
								//							>>ii.mLevel
								//							>>ii.mNumber
								//							>>ii.mAddAC
								//							>>ii.mAddMAC
								//							>>ii.mAddDC
								//							>>ii.mAddMC
								//							>>ii.mAddSC
								//							>>ii.mUpdAC
								//							>>ii.mUpdMAC
								//							>>ii.mUpdDC
								//							>>ii.mUpdMC
								//							>>ii.mUpdSC
								//							>>ii.mCreateTime
								//							>>ii.mIdentID
								//							>>ii.mProtect
								//							>>ii.mAddHp
								//							>>ii.mAddMp
								//							>>ii.mUpdMaxCount
								//							>>ii.mUpdFailedCount
								//							>>ii.mSellPriceType
								//							>>ii.mSellPrice;
								//						if( s )
								//						{
								//							if(ii.mNumber<=0)ii.mNumber = 1;
								//							chr.mItemList.push_back(ii);
								//						}
								//					}
								//				}
								//			}
								//		}
								//	}
								//}
								//if( version == 112 )
								//{
								//	{
								//		const char* KEYS[4]={"item_bag","item_depot1","item_depot2","item_lottery"};

								//		for(short key=0;key<4;key++)
								//		{
								//			const char* data = m_pDBModule->FetchField(row,res,KEYS[key],len);
								//			if( data && len > 0 )
								//			{
								//				LEUD::StreamFix s(data,len,len);
								//				int size;
								//				s>>size;
								//				if( s && size >0 )
								//				{
								//					for(int i = 0;i < size;i++)
								//					{
								//						OGSLib::SubItem ii;
								//						s>>ii.mPosition
								//							>>ii.mTypeID
								//							>>ii.mDuration
								//							>>ii.mDuraMax
								//							>>ii.mItemFlags
								//							>>ii.mLuck
								//							>>ii.mLevel
								//							>>ii.mNumber
								//							>>ii.mAddAC
								//							>>ii.mAddMAC
								//							>>ii.mAddDC
								//							>>ii.mAddMC
								//							>>ii.mAddSC
								//							>>ii.mUpdAC
								//							>>ii.mUpdMAC
								//							>>ii.mUpdDC
								//							>>ii.mUpdMC
								//							>>ii.mUpdSC
								//							>>ii.mCreateTime
								//							>>ii.mIdentID
								//							>>ii.mProtect
								//							>>ii.mAddHp
								//							>>ii.mAddMp
								//							>>ii.mUpdFailedCount
								//							>>ii.mSellPriceType
								//							>>ii.mSellPrice;
								//						if( s )
								//						{
								//							if(ii.mNumber<=0)ii.mNumber = 1;
								//							chr.mItemList.push_back(ii);
								//						}
								//					}
								//				}
								//			}
								//		}
								//	}
								//}
							}
						}
					}
					{
						const char* data = m_pDBModule->FetchField(row,res,"skill",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && version==101 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::SubSkill si;
									s>>si.mTypeID>>si.mLevel>>si.mExp;
									if( s )
									{
										chr.mSkillList.push_back(si);
									}
								}
							}
							else if( s && version==102 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::SubSkill si;
									s>>si.mTypeID>>si.mLevel>>si.mExp>>si.mParam1;
									if( s )
									{
										chr.mSkillList.push_back(si);
									}
								}
							}
						}
					}
					//{
					//	const char* data = m_pDBModule->FetchField(row,res,"reincarnation",len);
					//	if( data && len > 0 )
					//	{
					//		LEUD::StreamFix s(data,len,len);
					//		int version;
					//		s>>version;
					//		if( s && version==100 )
					//		{
					//			s>>chr.ReincarnateAttributes()->mDCMax
					//				>>chr.ReincarnateAttributes()->mMCMax
					//				>>chr.ReincarnateAttributes()->mSCMax
					//				>>chr.ReincarnateAttributes()->mACMax
					//				>>chr.ReincarnateAttributes()->mMACMax
					//				>>chr.ReincarnateAttributes()->mAccuracy
					//				>>chr.ReincarnateAttributes()->mDodge
					//				>>chr.ReincarnateAttributes()->mMaxHp
					//				>>chr.ReincarnateAttributes()->mMaxMp
					//				>>chr.ReincarnateAttributes()->mDropProb
					//				>>chr.ReincarnateAttributes()->mDoubleAttProb;
					//		}
					//	}
					//}
					{
						chr.mShortcutMap.clear();
						const char* data = m_pDBModule->FetchField(row,res,"shortcut",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && version==100 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::DomainData::Shortcut sc;
									s>>sc.shortcut_id
										>>sc.type
										>>sc.param;
									if( s )
									{
										chr.mShortcutMap[sc.shortcut_id]=sc;
									}
								}
							}
						}
					}
					{
						const char* data = m_pDBModule->FetchField(row,res,"mail",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							//if( s && version==100 && size >0 )
							//{
							//	for(int i = 0;i < size;i++)
							//	{
							//		OGSLib::SubMail sm;
							//		int size2=0;
							//		s>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mTitle>>sm.mContent>>size2;
							//		sm.mDeleted = 0;
							//		for(int j=0;j<size2;j++)
							//		{
							//			int id;int number;
							//			s>>id>>number;
							//			sm.mItems[id]=number;
							//		}
							//		if( s )
							//		{
							//			chr.mMailList[sm.mMailDate] = sm;
							//		}
							//	}
							//}
							//if( s && version==101 && size >0 )
							//{
							//	for(int i = 0;i < size;i++)
							//	{
							//		OGSLib::SubMail sm;
							//		int size2=0;
							//		s>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>sm.mTitle>>sm.mContent>>size2;
							//		for(int j=0;j<size2;j++)
							//		{
							//			int id;int number;
							//			s>>id>>number;
							//			sm.mItems[id]=number;
							//		}
							//		if( s )
							//		{
							//			chr.mMailList[sm.mMailDate] = sm;
							//		}
							//	}
							//}
							if( s && version==102 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::SubMail sm;
									int size2=0;
									s>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>size2;
									for(int j=0;j<size2;j++)
									{
										int id;int number;
										s>>id>>number;
										sm.mItems[id]=number;
									}
									if( s )
									{
										chr.mMailList[sm.mMailDate] = sm;
									}
								}
							}
							if( s && version==102 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::SubMail sm;
									int size2=0;
									s>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>size2;
									for(int j=0;j<size2;j++)
									{
										int id;int number;
										s>>id>>number;
										sm.mItems[id]=number;
									}
									if( s )
									{
										chr.mMailList[sm.mMailDate] = sm;
									}
								}
							}
							if( s && version==103 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::SubMail sm;
									int size2=0;
									int id_mail = 0;
									s>>id_mail>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>size2;
									sm.mID = id_mail;
									for(int j=0;j<size2;j++)
									{
										int id;int number;
										s>>id>>number;
										sm.mItems[id]=number;
									}
									if( s )
									{
										chr.mMailList[sm.mID] = sm;
									}
								}
							}
							if( s && version==104 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::SubMail sm;
									int size2=0;
									s>>sm.mID>>sm.mMailDate>>sm.mRead>>sm.mReceived>>sm.mDeleted>>size2;
									for(int j=0;j<size2;j++)
									{
										int id;int number;
										s>>id>>number;
										sm.mItems[id]=number;
									}
									if( s )
									{
										chr.mMailList[sm.mID] = sm;
									}
								}
							}
						}
					}
					{
						chr.mParamMap.clear();
						const char* data = m_pDBModule->FetchField(row,res,"var",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && version==101 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									int k;
									std::string v;
									s>>k>>v;
									if( ! v.empty() )
									{
										chr.mParamMap[k]=v;
									}
								}
							}
						}
					}
					{
						chr.mNumParamMap.clear();
						const char* data = m_pDBModule->FetchField(row,res,"intvar",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && version==101 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									int k;
									int v;
									s>>k>>v;
									chr.mNumParamMap[k]=v;
								}
							}
						}
					}
					{
						chr.mStatusMap.clear();
						const char* data = m_pDBModule->FetchField(row,res,"status",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && version==102 && size > 0 )
							{
								for(int i =0;i<size;i++)
								{
									int id;
									int param;
									int duration;
									int gap;
									int flags;
									s>>id
										>>param
										>>duration
										>>gap
										>>flags;
									OGSLib::SubStatus ss;
									ss.StatusID = id;
									ss.Param = param;
									ss.Duration = duration;
									ss.Gap = gap;
									ss.Flags = flags;
									ss.Tick = 0;
									chr.mStatusMap[ss.StatusID] = ss;
								}
							}
						}
					}
					{
						chr.mBuffMap.clear();
						const char* data = m_pDBModule->FetchField(row,res,"buff",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && version==102 && size > 0 )
							{
								for(int i =0;i<size;i++)
								{
									int id;
									int param;
									s>>id>>param;
									OGSLib::SubBuff sb;
									sb.id = id;
									sb.end_val = param;
									chr.mBuffMap[sb.id] = sb;
								}
							}
						}
					}
					{
						chr.mAchieveMap.clear();
						const char* data = m_pDBModule->FetchField(row,res,"achieve",len);
						if( data && len > 0 )
						{
							LEUD::StreamFix s(data,len,len);
							int version;
							int size;
							s>>version>>size;
							if( s && version==100 && size >0 )
							{
								for(int i = 0;i < size;i++)
								{
									OGSLib::SubAchieve sf;
									s>>sf.mAchieveID>>sf.mState>>sf.mParam;
									if( s )
									{
										chr.mAchieveMap[sf.mAchieveID] = sf;
									}
								}
							}
						}
					}
					loadresult = 1;
				}
			}
			m_pDBModule->FreeResult(res);
		}
		{
			_snprintf_s(m_Buffer1,1024*1024,1024*1024,"select * from gift where chrname='%s' and server_id='%d' and used=1",chrname,m_App->m_ServerID);
			MYSQL_RES* res = m_pDBModule->QueryStore(m_Buffer1);
			if( res )
			{
				MYSQL_ROW row;
				while( row = m_pDBModule->FetchRow(res) )
				{
					int len;
					OGSLib::SubGift gift;
					gift.mDBID=atoi(m_pDBModule->FetchField(row,res,"id",len));
					gift.mUsed=atoi(m_pDBModule->FetchField(row,res,"used",len));
					const char* dummy;
					dummy = m_pDBModule->FetchField(row,res,"bind",len);
					if( dummy ){ gift.mBind = atoi(dummy); }
					dummy = m_pDBModule->FetchField(row,res,"num",len);
					if( dummy ){ gift.mNum = atoi(dummy); }
					gift.mTypeID=atoi(m_pDBModule->FetchField(row,res,"typeid",len));
					strncpy_s(gift.mMsg,m_pDBModule->FetchField(row,res,"msg",len),63);
					gift.mMsg[63]=0;
					chr.mGiftArray.push_back(gift);
				}
				m_pDBModule->FreeResult(res);
			}
		}
		{
			_snprintf_s(m_Buffer1,1024*1024,1024*1024,"select chrname,\
													  relationship.rel_seed_name,\
													  relationship.rel_type,\
													  player.lv,\
													  player.job,\
													  player.gender,\
													  player.guildid,\
													  guild2.guild_name\
													  from relationship, player \
													  left join guild2 \
													  on player.guildid = guild2.guild_id \
													  where relationship.server_id = %d \
													  and relationship.seed_name = '%s' \
													  and relationship.rel_seed_name = player.seedname",
													  m_App->m_ServerID, seedname);

			MYSQL_RES* res = m_pDBModule->QueryStore(m_Buffer1);
			if( res )
			{
				MYSQL_ROW row;
				chr.mFriendArray.clear();
				while( row = m_pDBModule->FetchRow(res) )
				{
					OGSLib::SubFriend sf;

					int len;
					const char* dummy;
					dummy = m_pDBModule->FetchField(row,res,"chrname",len);
					if( dummy )
						sf.mName = dummy;
					dummy = m_pDBModule->FetchField(row,res,"rel_seed_name",len);
					if( dummy )
						sf.mSeedName = dummy;

					sf.mTitle = atoi(m_pDBModule->FetchField(row,res,"rel_type",len));
					sf.mLevel = atoi(m_pDBModule->FetchField(row,res,"lv",len));
					sf.mJob = atoi(m_pDBModule->FetchField(row,res,"job",len));
					sf.mGender = atoi(m_pDBModule->FetchField(row,res,"gender",len));

					dummy = m_pDBModule->FetchField(row,res,"guildid",len);
					if( dummy )
						sf.mGuildID = dummy;
					dummy = m_pDBModule->FetchField(row,res,"guild_name",len);
					if( dummy )
						sf.mGuild = dummy;

					if (OGSLib::SubFriend::IsFriend(sf.mTitle))
					{
						++ chr.mFriendArray.nFriend;
					}
					else if (OGSLib::SubFriend::IsMyEnemy(sf.mTitle))
					{
						++ chr.mFriendArray.nEnemy;
					}
					else if (OGSLib::SubFriend::IsMyBlack(sf.mTitle))
					{
						++ chr.mFriendArray.nBlack;
					}

					chr.mFriendArray.reltionship.push_back(sf);
				}
				m_pDBModule->FreeResult(res);
			}
		}
	}
	else
	{
		loadresult=2;
	}
	if( loadresult == 1 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int DBManager::DoSaveCharacter(OGSLib::DomainData* character,bool quit,const char* table)
{
	char temp[513];
	m_pDBModule->EscapeString(temp,character->mChrName,(unsigned long)strlen(character->mChrName));
	std::string chrname = temp;
	m_pDBModule->EscapeString(temp,character->mAccount,(unsigned long)strlen(character->mAccount));
	std::string account = temp;
	m_pDBModule->EscapeString(temp,character->mSeedName,(unsigned long)strlen(character->mSeedName));
	std::string seedname = temp;

	if( 0 == strcmp(table,"player") )
	{
		this->SaveChinaLimit(account.c_str(),character->china_limit_online_time_count,character->china_limit_offline_time_tag,character->china_id_ok,character->china_nm,character->china_id);
	}

	my_ulonglong ef=0;
	std::stringstream ssm;
	ssm<<"update "<<table<<" set lv='"<<character->level
		<<"',exp='"<<character->exp
		<<"',money='"<<character->money
		<<"',money_bind='"<<character->moneybind
		<<"',vcoin='"<<character->vcoin
		<<"',vcoings='"<<character->vcoings
		<<"',vcoin_bind='"<<character->vcoinbind
		<<"',vcoin_accu='"<<character->vcoinaccu
		//<<"',vip_level='"<<character->viplevel
		//<<"',capacity='"<<character->mCapacity
		//<<"',tradelimit='"<<character->mLockTradeLimit
		//<<"',reinlevel='"<<character->mReincarnatedLevel
		<<"',inner_power='"<<character->cur_power
		<<"',lastvcoinday='"<<character->mLastVcoinUsedDay
		<<"',vcoinbyday='"<<character->mVcoinUsedByDay
		<<"',vcoinbymonth='"<<character->mVcoinUsedByMonth
		<<"',lastczhimonth='"<<character->mLastCongzhiMonth
		<<"',czhibymonth='"<<character->mVcoinAccuByMonth
		<<"',guildpt='"<<character->mGuildPt
		<<"',guildspt='"<<character->mGuildSpt
		<<"',pkvalue='"<<character->pkvalue
		<<"',chatoff='"<<character->talk_prohibited
		<<"',depotslotadd='"<<character->depot_slot_add
		//<<"',freedirectfly='"<<character->mFreeDirectFly
		//<<"',couple_id='"<<character->mCoupleID
		//<<"',weddingdate='"<<character->mWeddingdate
		//<<"',marrytimes='"<<character->mMarryTimes
		<<"',team_id='"<<character->mHonor
		<<"',ot_all='"<<character->online_time_all
		<<"',ot_today='"<<character->online_time_today
		<<"',ot_today_tag='"<<character->online_tag_today
		<<"',ot_yeday='"<<character->online_time_yestoday
		<<"',ot_yeday_tag='"<<character->online_tag_yestoday
		//<<"',chinaid='"<<character->china_id
		//<<"',chinanm='"<<character->china_nm
		//<<"',chinaok='"<<character->china_id_ok
		<<"',offline_time='"<<character->offline_time
		<<"',offline_tag='"<<character->offline_tag
		<<"',achievept='"<<character->mAchievePoint
		<<"',achievegmmax='"<<character->mAchieveGameMoneyMax
		<<"',onlinetimemax='"<<character->mOnlineTimeMax
		<<"',logindaycontinue='"<<character->mLoginDayContinue
		<<"',logindaycontinuemax='"<<character->mLoginDayContinueMax
		<<"',logindaycount='"<<character->mLoginDayCount
		<<"',cur_hp='"<<character->cur_hp
		<<"',cur_mp='"<<character->cur_mp
		<<"',bagslotadd='"<<character->bag_slot_add
		<<"',mainTaskState='"<<character->mMainTaskState
		<<"',chrname='"<<character->mChrName
		<<"' where seedname='"<<seedname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
	m_pDBModule->QueryStore(ssm.str().c_str(),true,&ef);

	std::stringstream ssv;
	unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,character->mGuildName,(unsigned long)strlen(character->mGuildName));
	ssv<<"update "<<table<<" set guild='"<<m_Buffer2;
	escape_len = m_pDBModule->EscapeString(m_Buffer2,character->mGuildID,(unsigned long)strlen(character->mGuildID));
	ssv<<"',guildid='"<<m_Buffer2;
	escape_len = m_pDBModule->EscapeString(m_Buffer2,character->exit_map,(unsigned long)strlen(character->exit_map));
	ssv<<"',emap='"<<m_Buffer2;
	ssv<<"',emapx='"<<character->exit_x
		<<"',emapy='"<<character->exit_y;
	escape_len = m_pDBModule->EscapeString(m_Buffer2,character->safe_map,(unsigned long)strlen(character->safe_map));
	ssv<<"',safemap='"<<m_Buffer2;
	ssv<<"',safemapx='"<<character->safe_x
		<<"',safemapy='"<<character->safe_y;
	ssv<<"',kuafu_result='"<<character->mKuafuResult;
// 	{
// 		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
// 		int version=100;
// 		s<<version;
// 		s<<static_cast<int>(character->mFriendArray.reltionship.size());
// 		for( std::vector<OGSLib::SubFriend>::iterator pos = character->mFriendArray.reltionship.begin();
// 			pos != character->mFriendArray.reltionship.end();
// 			++ pos )
// 		{
// 			s<<pos->mName<<pos->mTitle;
// 		}
// 		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
// 		ssv<<"',friend='"<<m_Buffer2;
// 	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=102;
		s<<version;
		s<<static_cast<int>(character->mSkillList.size());
		for( OGSLib::DomainData::SkillMap::iterator pos = character->mSkillList.begin();
			pos != character->mSkillList.end();
			++ pos )
		{
			s<<pos->mTypeID<<pos->mLevel<<pos->mExp<<pos->mParam1;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ssv<<"',skill='"<<m_Buffer2;
	}
	//下线记录玩家身上的一部分装备信息
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=101;
		s<<version;
		s<<character->mCloth<<character->mWeapon<<character->mFashionCloth<<character->mFashionWeapon<<character->mWing;
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ssv<<"',equip_have='"<<m_Buffer2;
	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=100;
		s<<version;
		s<<static_cast<int>(character->mShortcutMap.size());
		for( OGSLib::DomainData::ShortcutMap::iterator pos = character->mShortcutMap.begin();
			pos != character->mShortcutMap.end();
			++ pos )
		{
			s<<pos->second.shortcut_id
				<<pos->second.type
				<<pos->second.param;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ssv<<"',shortcut='"<<m_Buffer2;
	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=102;
		s<<version;
		s<<static_cast<int>(character->mStatusMap.size());
		for( OGSLib::DomainData::StatusMap::iterator pos = character->mStatusMap.begin();
			pos != character->mStatusMap.end();
			++ pos )
		{
			s<<pos->second.StatusID
				<<pos->second.Param
				<<static_cast<int>(pos->second.Duration)
				<<static_cast<int>(pos->second.Gap)
				<<pos->second.Flags;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ssv<<"',status='"<<m_Buffer2;
	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=102;
		s<<version;
		s<<static_cast<int>(character->mBuffMap.size());
		for( OGSLib::DomainData::BuffMap::iterator pos = character->mBuffMap.begin();
			pos != character->mBuffMap.end();
			++ pos )
		{
			s<<pos->second.id
				<<static_cast<int>(pos->second.end_val);
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ssv<<"',buff='"<<m_Buffer2;
	}
	ssv<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
	m_pDBModule->QueryStore(ssv.str().c_str(),true,&ef);

	/*std::stringstream ssmail;
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=101;
		s<<version;
		s<<static_cast<int>(character->mMailList.size());
		for( OGSLib::DomainData::MailMap::iterator pos = character->mMailList.begin();
			pos != character->mMailList.end();
			++ pos )
		{
			s<<pos->second.mMailDate
				<<pos->second.mRead
				<<pos->second.mReceived
				<<pos->second.mDeleted
				<<pos->second.mTitle
				<<pos->second.mContent;
			s<<static_cast<int>(pos->second.mItems.size());
			for( std::map<int,int>::iterator iter = pos->second.mItems.begin();
				iter != pos->second.mItems.end();
				++ iter )
			{
				s<<iter->first;
				s<<iter->second;
			}
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ssmail<<"update "<<table<<" set mail='"<<m_Buffer2;
	}
	ssmail<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
	m_pDBModule->QueryStore(ssmail.str().c_str(),true,&ef);*/

	std::stringstream ssmail;
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=104;
		s<<version;
		s<<static_cast<int>(character->mMailList.size());
		for( OGSLib::DomainData::MailMap::iterator pos = character->mMailList.begin();
			pos != character->mMailList.end();
			++ pos )
		{
			s<<pos->second.mID
				<<pos->second.mMailDate
				<<pos->second.mRead
				<<pos->second.mReceived
				<<pos->second.mDeleted;
			s<<static_cast<int>(pos->second.mItems.size());
			for( std::map<int,int>::iterator iter = pos->second.mItems.begin();
				iter != pos->second.mItems.end();
				++ iter )
			{
				s<<iter->first;
				s<<iter->second;
			}
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ssmail<<"update "<<table<<" set mail='"<<m_Buffer2;
	}
	ssmail<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
	m_pDBModule->QueryStore(ssmail.str().c_str(),true,&ef);

	std::stringstream ss;
	//item 只存version，数据分存于item_bag/item_depot1/item_depot2/item_lottery
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=117;
		s<<version;
		s<<static_cast<int>(character->mItemList.size());
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ss<<"update "<<table<<" set item='"<<m_Buffer2;
	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=101;
		s<<version;
		s<<static_cast<int>(character->mParamMap.size());
		for( OGSLib::DomainData::ParamMap::iterator pos = character->mParamMap.begin();
			pos != character->mParamMap.end();
			++ pos )
		{
			s<<pos->first
				<<pos->second;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ss<<"',var='"<<m_Buffer2;
	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=101;
		s<<version;
		s<<static_cast<int>(character->mParamMap.size());
		for( OGSLib::DomainData::NumParamMap::iterator pos = character->mNumParamMap.begin();
			pos != character->mNumParamMap.end();
			++ pos )
		{
			s<<pos->first
				<<pos->second;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ss<<"',intvar='"<<m_Buffer2;
	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=100;
		s<<version;
		s<<static_cast<int>(character->mAchieveMap.size());
		for( OGSLib::DomainData::AchieveMap::iterator pos = character->mAchieveMap.begin();
			pos != character->mAchieveMap.end();
			++ pos )
		{
			s<<pos->second.mAchieveID<<pos->second.mState<<pos->second.mParam;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ss<<"',achieve='"<<m_Buffer2;
	}
	{
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int version=102;
		s<<version;
		s<<static_cast<int>(character->mTaskMap.size());
		for( OGSLib::DomainData::TaskMap::iterator pos = character->mTaskMap.begin();
			pos != character->mTaskMap.end();
			++ pos )
		{
			OGSLib::SubTask& ti=pos->second;
			s<<ti.TaskID
				<<ti.TaskState
				<<ti.TaskParam1
				<<ti.TaskParam2
				<<ti.TaskParam3
				<<ti.TaskParam4;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		ss<<"',task='"<<m_Buffer2;
	}
	//{
	//	LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
	//	int version=100;
	//	s<<version;
	//	s<<character->ReincarnateAttributes()->mDCMax
	//		<<character->ReincarnateAttributes()->mMCMax
	//		<<character->ReincarnateAttributes()->mSCMax
	//		<<character->ReincarnateAttributes()->mACMax
	//		<<character->ReincarnateAttributes()->mMACMax
	//		<<character->ReincarnateAttributes()->mAccuracy
	//		<<character->ReincarnateAttributes()->mDodge
	//		<<character->ReincarnateAttributes()->mMaxHp
	//		<<character->ReincarnateAttributes()->mMaxMp
	//		<<character->ReincarnateAttributes()->mDropProb
	//		<<character->ReincarnateAttributes()->mDoubleAttProb;
	//	unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
	//	ss<<"',reincarnation='"<<m_Buffer2;
	//}
	if( quit )
	{
		ss<<"',last_logout_time=now(),online='"<<0;
	}
    ss<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
	
	m_pDBModule->QueryStore(ss.str().c_str(),true,&ef);
	{
		for( OGSLib::DomainData::GiftArray::iterator pos = character->mGiftArray.begin();
			pos != character->mGiftArray.end();
			++ pos )
		{
			if( pos->mUsed == 10 )
			{
				_snprintf_s(m_Buffer1,1024*1024,1024*1024,"update gift set used=10 where id='%d'",pos->mDBID);
				m_pDBModule->QueryStore(m_Buffer1,true);
			}
		}
	}
	//item data
	{
		std::stringstream strss;
		strss<<"update "<<table<<" set ";
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int bag_size = 0;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition >= 1000)continue;
			bag_size++;
		}
		s<<bag_size;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition >= 1000)continue;
			s << pos->mPosition
				<< pos->mTypeID
				<< pos->mDuration
				<< pos->mDuraMax
				<< pos->mItemFlags
				<< pos->mLuck
				<< pos->mLevel
				<< pos->mNumber
				<< pos->mAddAC
				<< pos->mAddMAC
				<< pos->mAddDC
				<< pos->mAddMC
				<< pos->mAddSC
				<< pos->mUpdAC
				<< pos->mUpdMAC
				<< pos->mUpdDC
				<< pos->mUpdMC
				<< pos->mUpdSC
				<< pos->mCreateTime
				<< pos->mIdentId
				<< pos->mProtect
				<< pos->mAddHp
				<< pos->mAddMp
				<< pos->mUpdMaxCount
				<< pos->mUpdFailedCount
				<< pos->mSellPriceType
				<< pos->mSellPrice
				<< pos->mZLevel
				<< pos->mLock
				<< pos->mLasttime;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		strss<<" item_bag= '"<<m_Buffer2<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
		m_pDBModule->QueryStore(strss.str().c_str(),true,&ef);
	}
	{
		std::stringstream strss;
		strss<<"update "<<table<<" set ";
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int depot1_size = 0;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition < 1000 || pos->mPosition >= 1200)continue;
			depot1_size++;
		}
		s<<depot1_size;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition < 1000 || pos->mPosition >= 1200)continue;
			s<<pos->mPosition
				<<pos->mTypeID
				<<pos->mDuration
				<<pos->mDuraMax
				<<pos->mItemFlags
				<<pos->mLuck
				<<pos->mLevel
				<<pos->mNumber
				<<pos->mAddAC
				<<pos->mAddMAC
				<<pos->mAddDC
				<<pos->mAddMC
				<<pos->mAddSC
				<<pos->mUpdAC
				<<pos->mUpdMAC
				<<pos->mUpdDC
				<<pos->mUpdMC
				<<pos->mUpdSC
				<<pos->mCreateTime
				<<pos->mIdentId
				<<pos->mProtect
				<<pos->mAddHp
				<<pos->mAddMp
				<<pos->mUpdMaxCount
				<<pos->mUpdFailedCount
				<<pos->mSellPriceType
				<<pos->mSellPrice
				<<pos->mZLevel
				<<pos->mLock
				<<pos->mLasttime;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		strss<<" item_depot1= '"<<m_Buffer2<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
		m_pDBModule->QueryStore(strss.str().c_str(),true,&ef);
	}
	{
		std::stringstream strss;
		strss<<"update "<<table<<" set ";
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int depot2_size = 0;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition < 1200 || pos->mPosition >= 1400)continue;
			depot2_size++;
		}
		s<<depot2_size;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition < 1200 || pos->mPosition >= 1400)continue;
			s<<pos->mPosition
				<<pos->mTypeID
				<<pos->mDuration
				<<pos->mDuraMax
				<<pos->mItemFlags
				<<pos->mLuck
				<<pos->mLevel
				<<pos->mNumber
				<<pos->mAddAC
				<<pos->mAddMAC
				<<pos->mAddDC
				<<pos->mAddMC
				<<pos->mAddSC
				<<pos->mUpdAC
				<<pos->mUpdMAC
				<<pos->mUpdDC
				<<pos->mUpdMC
				<<pos->mUpdSC
				<<pos->mCreateTime
				<<pos->mIdentId
				<<pos->mProtect
				<<pos->mAddHp
				<<pos->mAddMp
				<<pos->mUpdMaxCount
				<<pos->mUpdFailedCount
				<<pos->mSellPriceType
				<<pos->mSellPrice
				<<pos->mZLevel
				<<pos->mLock
				<<pos->mLasttime;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		strss<<" item_depot2= '"<<m_Buffer2<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
		m_pDBModule->QueryStore(strss.str().c_str(),true,&ef);
	}
	{
		std::stringstream strss;
		strss<<"update "<<table<<" set ";
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int lottery_size = 0;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition < 2000 || pos->mPosition >= 4000)continue;
			lottery_size++;
		}
		s<<lottery_size;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++ pos )
		{
			if(pos->mPosition < 2000 || pos->mPosition >= 4000)continue;
			s<<pos->mPosition
				<<pos->mTypeID
				<<pos->mDuration
				<<pos->mDuraMax
				<<pos->mItemFlags
				<<pos->mLuck
				<<pos->mLevel
				<<pos->mNumber
				<<pos->mAddAC
				<<pos->mAddMAC
				<<pos->mAddDC
				<<pos->mAddMC
				<<pos->mAddSC
				<<pos->mUpdAC
				<<pos->mUpdMAC
				<<pos->mUpdDC
				<<pos->mUpdMC
				<<pos->mUpdSC
				<<pos->mCreateTime
				<<pos->mIdentId
				<<pos->mProtect
				<<pos->mAddHp
				<<pos->mAddMp
				<<pos->mUpdMaxCount
				<<pos->mUpdFailedCount
				<<pos->mSellPriceType
				<<pos->mSellPrice
				<<pos->mZLevel
				<<pos->mLock
				<<pos->mLasttime;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		strss<<" item_lottery= '"<<m_Buffer2<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
		m_pDBModule->QueryStore(strss.str().c_str(),true,&ef);
	}
/////////////////////////////////////////
	{
		std::stringstream strss;
		strss << "update " << table << " set ";
		LEUD::StreamFix s(m_Buffer1, m_BufferLen, 0);
		int fashion_size = 0;
		for (OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++pos)
		{
			if (pos->mPosition < 5000 || pos->mPosition >= 5100)continue;
			fashion_size++;
		}
		s << fashion_size;
		for (OGSLib::DomainData::ItemMap::iterator pos = character->mItemList.begin();
			pos != character->mItemList.end();
			++pos)
		{
			if (pos->mPosition < 5000 || pos->mPosition >= 5100)continue;
			s << pos->mPosition
				<< pos->mTypeID
				<< pos->mDuration
				<< pos->mDuraMax
				<< pos->mItemFlags
				<< pos->mLuck
				<< pos->mLevel
				<< pos->mNumber
				<< pos->mAddAC
				<< pos->mAddMAC
				<< pos->mAddDC
				<< pos->mAddMC
				<< pos->mAddSC
				<< pos->mUpdAC
				<< pos->mUpdMAC
				<< pos->mUpdDC
				<< pos->mUpdMC
				<< pos->mUpdSC
				<< pos->mCreateTime
				<< pos->mIdentId
				<< pos->mProtect
				<< pos->mAddHp
				<< pos->mAddMp
				<< pos->mUpdMaxCount
				<< pos->mUpdFailedCount
				<< pos->mSellPriceType
				<< pos->mSellPrice
				<< pos->mZLevel
				<< pos->mLock
				<< pos->mLasttime;
		}
		unsigned long escape_len = m_pDBModule->EscapeString(m_Buffer2, m_Buffer1, (unsigned long)s.size());
		strss << " item_fashion= '" << m_Buffer2 << "' where chrname='" << chrname << "' and account='" << account << "' and server_id='" << m_App->m_ServerID << "' ";
		m_pDBModule->QueryStore(strss.str().c_str(), true, &ef);
	}
	//////////////////////////
	{
		std::stringstream strss;
		strss<<"update "<<table<<" set ";
		LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
		int ChatTrade_size = 0;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mChatTradeItemList.begin();
			pos != character->mChatTradeItemList.end();
			++ pos )
		{
			if(pos->mPosition < 4000 || pos->mPosition >= 4006)continue;
			ChatTrade_size++;
		}
		s<<ChatTrade_size;
		for( OGSLib::DomainData::ItemMap::iterator pos = character->mChatTradeItemList.begin();
			pos != character->mChatTradeItemList.end();
			++ pos )
		{
			if(pos->mPosition < 4000 || pos->mPosition >= 4006)continue;
			s<<pos->mPosition
				<<pos->mTypeID
				<<pos->mDuration
				<<pos->mDuraMax
				<<pos->mItemFlags
				<<pos->mLuck
				<<pos->mLevel
				<<pos->mNumber
				<<pos->mAddAC
				<<pos->mAddMAC
				<<pos->mAddDC
				<<pos->mAddMC
				<<pos->mAddSC
				<<pos->mUpdAC
				<<pos->mUpdMAC
				<<pos->mUpdDC
				<<pos->mUpdMC
				<<pos->mUpdSC
				<<pos->mCreateTime
				<<pos->mIdentId
				<<pos->mProtect
				<<pos->mAddHp
				<<pos->mAddMp
				<<pos->mUpdMaxCount
				<<pos->mUpdFailedCount
				<<pos->mSellPriceType
				<<pos->mSellPrice
				<<pos->mZLevel
				<<pos->mLock
				<<pos->mLasttime;
		}
		unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
		strss<<" item_chattrade= '"<<m_Buffer2<<"' where chrname='"<<chrname<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
		m_pDBModule->QueryStore(strss.str().c_str(),true,&ef);	
	}
	{
		// 删除原先人物关系
		std::stringstream strss;
		strss << "delete from relationship where seed_name = '" << seedname << "' and server_id='" << m_App->m_ServerID << "'";
		m_pDBModule->QueryStore(strss.str().c_str(),true,&ef);	
		
		// 保存人物关系
		for (size_t i = 0; i < character->mFriendArray.reltionship.size(); ++ i)
		{
			std::stringstream strss2;
			OGSLib::SubFriend & sf = character->mFriendArray.reltionship[i];
			strss2 << "insert into relationship(server_id, seed_name, rel_seed_name, rel_type) values(" 
				<< m_App->m_ServerID << ", '" << seedname << "', '" << sf.mSeedName << "', " << sf.mTitle << ")";

			m_pDBModule->QueryStore(strss2.str().c_str(),true,&ef);	
		}
	}
	if( ef == 1 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int DBManager::DoSaveKuafuCharacter(std::string charID,std::string account,int vcoin,int kuafuResult)
{
	std::stringstream ss;
	ss<<"update player set vcoin='"<<vcoin
		<<"',kuafu_result='"<<kuafuResult;
	ss<<"' where chrname='"<<charID<<"' and account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' ";
	my_ulonglong ef=0;
	m_pDBModule->QueryStore(ss.str().c_str(),true,&ef);
	if( ef == 1 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
void DBManager::ClientListCharacter(SOCKET sock,std::string account,GS_Client::ListCharacterRes& resp)
{
	char temp[513];
	m_pDBModule->EscapeString(temp,account.c_str(),(unsigned long)account.length());
	account = temp;

	int online_time_count;
	int offline_time_tag;
	char cn_name[256];
	char cn_id[256];
	int cn_ok;
	this->LoadChinaLimit(account.c_str(),&online_time_count,&offline_time_tag,&cn_ok,cn_name,cn_id);
	if( cn_ok <= 0 || cn_ok == 102)
	{
		if( online_time_count > 60*60*3 )
		{
			time_t tt = time(0);
			if( offline_time_tag + 60*60*5 > tt )
			{
				resp.china_limit_tag = static_cast<int>(offline_time_tag + 60*60*5 - tt);
			}
		}
	}

	std::stringstream ss;
	ss<<"select chrname,seedname,lv,job,gender,svrid,online,equip_have from player where account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' and deleted=0";
	MYSQL_RES* res = m_pDBModule->QueryStore(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			GS_Client::ListCharacterRes::character chr;
			chr.name = m_pDBModule->FetchField(row,res,"chrname",len);
			chr.lv = atoi(m_pDBModule->FetchField(row,res,"lv",len));
			chr.job = atoi(m_pDBModule->FetchField(row,res,"job",len));
			chr.gender = atoi(m_pDBModule->FetchField(row,res,"gender",len));
			chr.online = atoi(m_pDBModule->FetchField(row,res,"online",len));
			{
				const char* dummy;
				chr.svrid = 0;
				dummy = m_pDBModule->FetchField(row,res,"svrid",len);
				if(dummy)
				{
					chr.svrid = atoi(dummy);
				}
				dummy = m_pDBModule->FetchField(row,res,"seedname",len);
				if(dummy)
				{
					chr.seedname = dummy;
				}
				else
				{
					int now = static_cast<int>(time(0));
					int num = rand()%9000 + 1000;
					//std::string strchrname = account;
					std::stringstream seedname;
					seedname<<account<<now<<num;
					chr.seedname = seedname.str();
					_snprintf_s(m_Buffer1,1024*1024,1024*1024,"update player set seedname='%s' where chrname= '%s'",chr.seedname.c_str(),chr.name.c_str());
					m_pDBModule->QueryStore(m_Buffer1,true);
				}
			}
			{
				const char* data = m_pDBModule->FetchField(row,res,"equip_have",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					s>>version;
					if( s && version==101 )
					{
						s>>chr.cloth>>chr.weapon>>chr.fashionCloth>>chr.fashionWeapon>>chr.wing;
					}
				}
				else
				{
					///////////////玩家没进游戏时候初始化一个数据
					chr.cloth = -1;
					chr.weapon = -1;
					chr.fashionCloth = -1;
					chr.fashionWeapon = -1;
					chr.wing = -1;
				}
			}
			resp.charlist.push_back(chr);
		}
		m_pDBModule->FreeResult(res);
	}
}
void DBManager::ClientDeleteCharacter(SOCKET sock,std::string account,std::string chrname,GS_Client::DeleteCharacterRes& res)
{
	char temp[513];
	m_pDBModule->EscapeString(temp,account.c_str(),(unsigned long)account.length());
	account = temp;
	m_pDBModule->EscapeString(temp,chrname.c_str(),(unsigned long)chrname.length());
	chrname = temp;

	std::stringstream ss;
	ss<<"update player set deleted=1 where chrname='"<<chrname<<"' and server_id='"<<m_App->m_ServerID<<"' and account='"<<account<<"' and online=0";
	my_ulonglong ef=0;
	m_pDBModule->QueryStore(ss.str().c_str(),true,&ef);

	if (ef > 0)
	{
		res.result = GS_Client::DeleteCharacterRes::result_type_succes;

		std::stringstream ss;
		ss<<"select seedname, guildid from player where chrname='"<<chrname<<"' and server_id='"<<m_App->m_ServerID<<"' and account='"<<account<<"'";
		MYSQL_RES* r = m_pDBModule->QueryStore(ss.str().c_str());
		if( r )
		{
			MYSQL_ROW row;
			if( row = m_pDBModule->FetchRow(r) )
			{
				int len;
				const char * tp = m_pDBModule->FetchField(row,r,"seedname",len);
				if( tp ) 
					res.chr_seed = tp;
				tp = m_pDBModule->FetchField(row,r,"guildid",len);
				if( tp ) 
					res.guild_seed = tp;
			}
			m_pDBModule->FreeResult(r);
		}
	}
}

void DBManager::ClientAuthModifyName(std::string chrid,int &ret)
{
	char temp[513];
	m_pDBModule->EscapeString(temp,chrid.c_str(),(unsigned long)chrid.length());
	chrid = temp;
	std::stringstream ss;
	ss<<"select * from player where chrname='"<<chrid<<"' and server_id='"<<m_App->m_ServerID<<"'";
	MYSQL_RES* r = m_pDBModule->QueryStore(ss.str().c_str());
	if( r )
	{
		if( MYSQL_ROW row = m_pDBModule->FetchRow(r) )
		{
			ret = 1;
		}
		m_pDBModule->FreeResult(r);
	}
}
void DBManager::ClientCreateCharacter(SOCKET sock,std::string account,std::string chrname,std::string idfa,int pid,int job,int gender,int svrid,std::string youke,GS_Client::CreateCharacterRes& res)
{
	char temp[513];
	m_pDBModule->EscapeString(temp,account.c_str(),(unsigned long)account.length());
	account = temp;
	m_pDBModule->EscapeString(temp,chrname.c_str(),(unsigned long)chrname.length());
	chrname = temp;
	{
		std::stringstream ss;
		ss<<"select count(chrname) as cnt from player where account='"<<account<<"' and server_id='"<<m_App->m_ServerID<<"' and deleted=0";
		MYSQL_RES* r = m_pDBModule->QueryStore(ss.str().c_str());
		if( r )
		{
			if( MYSQL_ROW row = m_pDBModule->FetchRow(r) )
			{
				int len;
				int cnt = atoi(m_pDBModule->FetchField(row,r,"cnt",len));
				if( cnt >= m_App->m_CharacterMaxNumber )
				{
					res.result = res.result_type_too_many;
					m_pDBModule->FreeResult(r);
					return ;
				}
			}
			m_pDBModule->FreeResult(r);
		}
	}
	{
		std::stringstream ss;
		ss<<"select chrname from player where chrname='"<<chrname<<"'";
		MYSQL_RES* r = m_pDBModule->QueryStore(ss.str().c_str());
		if( r )
		{
			if( m_pDBModule->FetchRow(r) )
			{
				res.result = res.result_type_name_existed;
				m_pDBModule->FreeResult(r);
				return ;
			}
			m_pDBModule->FreeResult(r);
		}
	}
	{
		int now = static_cast<int>(time(0));
		int num = rand()%9000 + 1000;
		std::stringstream seedname;
		seedname<<account<<now<<num;
		std::string strname = seedname.str();

		std::stringstream ss;
		ss<<"insert into player set chrname='"<<chrname
			<<"',account='"<<account
			<<"',seedname='"<<strname
			<<"',job='"<<job
			<<"',gender='"<<gender
			<<"',lv='1"
			<<"',server_id='"<<m_App->m_ServerID
			<<"',idfa='"<<idfa
			<<"',pid='"<<pid
			<<"',svrid='"<<svrid
			<<"',create_time=now()";
		my_ulonglong ef=0;
		m_pDBModule->QueryStore(ss.str().c_str(),true,&ef);
		if(ef==1)
		{
			//OGSLib::DomainData chr;
			//if( 0 == DoLoadCharacter(youke,youke,chr,"demo") )
			//{
			//	{
			//		std::stringstream ss;
			//		ss<<"update demo set deleted=1 where chrname='"<<chrname<<"' and account='"<<account<<"'";
			//		m_pDBModule->QueryStore(ss.str().c_str());
			//	}
			//	chr.mChrName = chrname;
			//	chr.mAccount = account;
			//	chr.gender = gender;
			//	chr.job = job;
			//	chr.mGuildName = "";
			//	chr.mShortcutMap.clear();
			//	chr.mSkillList.clear();
			//	chr.mItemList.clear();
			//	chr.mTaskMap.clear();
			//	DoSaveCharacter(&chr,true,"player");
			//}
			res.result = GS_Client::CreateCharacterRes::result_type_succes;
			strncpy_s(res.seedname,_countof(res.seedname),seedname.str().c_str(),511);
			return ;
		}
		else
		{
			return ;
		}
	}
}
int DBManager::LoadGuilds(OGSLib::ManagerGuild& guild_manager)
{
	std::stringstream ss;
	ss<<"select * from guild2 where server_id='"<<m_App->m_ServerID<<"' and del_flag=0";
	int GuildSeed = 1000;
	OGSLib::ManagerGuild::GuildMap gm;
	MYSQL_RES* res = m_pDBModule->QueryUse(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			OGSLib::Guild* guild = new OGSLib::Guild();
			const char* tp;
			GuildSeed ++;
			tp = m_pDBModule->FetchField(row,res,"guild_id",len);//add
			if( tp ) guild->SetSeedID(std::string(tp));
			else
			{
				std::string hf = "merge";
				int num = rand()%9000 + 1000;
				std::stringstream seedid; seedid<<hf<<num<<GuildSeed;
				std::string guildid = seedid.str();
				guild->SetSeedID(guildid);
			}
			tp = m_pDBModule->FetchField(row,res,"guild_name",len);
			if( tp ) guild->SetName(std::string(tp));

			tp = m_pDBModule->FetchField(row,res,"guild_desp",len);//move
			if( tp ) guild->SetDesp(std::string(tp));
			tp = m_pDBModule->FetchField(row,res,"guild_notice",len);//move
			if( tp ) guild->SetNotice(std::string(tp));

			guild->master_level(atoi(m_pDBModule->FetchField(row,res,"mslv",len)));
			tp = m_pDBModule->FetchField(row,res,"guild_lv",len);
			if(tp){guild->guild_level(atoi(tp));}
			tp = m_pDBModule->FetchField(row,res,"guild_pt",len);
			if(tp){guild->guild_pt(atoi(tp));}

			{
				const char* data = m_pDBModule->FetchField(row,res,"members",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					int size;
					s>>version>>size;
					//printf("guild members %d %d\n",version,size);
					//if( s && version==100 && size >0 )
					//{
					//	for(int i = 0;i < size;i++)
					//	{
					//		std::string nick_name;
					//		int title;
					//		s>>nick_name;
					//		s>>title;
					//		if( s )
					//		{
					//			guild->AddMember("",nick_name.c_str(),title,NULL);
					//			guild->SetMemberTitle(nick_name,title);
					//		}
					//	}
					//}
					//if( s && version==101 && size >0 )
					//{
					//	for(int i = 0;i < size;i++)
					//	{
					//		std::string nick_name;
					//		int title;
					//		int fight = 0;
					//		s>>nick_name;
					//		s>>title;
					//		s>>fight;
					//		if( s )
					//		{
					//			guild->AddMember("",nick_name.c_str(),title,NULL);
					//			guild->SetMemberTitle(nick_name,title);
					//		}
					//	}
					//}
					//if( s && version==102 && size >0 )
					//{
					//	for(int i = 0;i < size;i++)
					//	{
					//		std::string nick_name;
					//		int title;
					//		int fight = 0;
					//		int lastout = 0;
					//		s>>nick_name;
					//		s>>title;
					//		s>>fight;
					//		s>>lastout;
 				//			if( s )
 				//			{
					//			guild->AddMember("",nick_name.c_str(),title,NULL);
 				//				guild->SetMemberTitle(nick_name,title);
 				//			}
					//	}
					//}
					//if( s && version==103 && size >0 )
					//{
					//	for(int i = 0;i < size;i++)
					//	{
					//		std::string nick_name;
					//		std::string seed_name;
					//		int title;
					//		int fight = 0;
					//		int lastout = 0;
					//		int gdpt = 0;
					//		s>>seed_name;
					//		s>>nick_name;
					//		s>>title;
					//		s>>fight;
					//		s>>lastout;
					//		s>>gdpt;
					//		//printf("DBManager: 103: %s %s %d %d %d %d\n",seed_name,nick_name,title,fight,lastout,gdpt);
					//		if( s )
					//		{
					//			guild->AddMember(nick_name.c_str(),seed_name.c_str(),title,NULL);
					//			guild->SetMemberTitle(seed_name,title);
					//		}
					//	}
					//}
					//if( s && version==104 && size >0 )
					//{
					//	for(int i = 0;i < size;i++)
					//	{
					//		std::string nick_name;
					//		std::string seed_name;
					//		int title;
					//		int fight = 0;
					//		int lastout = 0;
					//		int gdpt = 0;
					//		int gdspt = 0;
					//		s>>seed_name;
					//		s>>nick_name;
					//		s>>title;
					//		s>>fight;
					//		s>>lastout;
					//		s>>gdspt;
					//		s>>gdpt;
					//		//printf("DBManager: 103: %s %s %d %d %d %d\n",seed_name,nick_name,title,fight,lastout,gdpt);
					//		if( s )
					//		{
					//			guild->AddMember(nick_name.c_str(),seed_name.c_str(),title,NULL);
					//			guild->SetMemberTitle(seed_name,title);
					//			guild->SetMemberGuildPt(seed_name,gdpt,0);
					//		}
					//	}
					//}
					//if( s && version==105 && size >0 )
					//{
					//	for(int i = 0;i < size;i++)
					//	{
					//		std::string nick_name;
					//		std::string seed_name;
					//		int title;
					//		int fight = 0;
					//		int lastout = 0;
					//		int gdpt = 0;
					//		int gdspt = 0;
					//		int entertime = 0;
					//		s>>seed_name;
					//		s>>nick_name;
					//		s>>title;
					//		s>>fight;
					//		s>>lastout;
					//		s>>gdspt;
					//		s>>gdpt;
					//		s>>entertime;
					//		//printf("DBManager: 103: %s %s %d %d %d %d\n",seed_name,nick_name,title,fight,lastout,gdpt);
					//		if( s && title != 101 )
					//		{
					//			guild->AddMember(nick_name.c_str(),seed_name.c_str(),title,NULL);
					//			guild->SetMemberTitle(seed_name,title);
					//			if (title == 200)
					//			{
					//				guild->AddAdvNum();
					//			}
					//			guild->SetMemberGuildPt(seed_name,gdpt,0);
					//			guild->SetMemberEnterTime(seed_name,entertime);
					//		}
					//	}
					//}
					if( s && (version==106|| version == 107)&& size >0 )
					{
						for(int i = 0;i < size;i++)
						{
							std::string nick_name;
							std::string seed_name;
							int title;
							int fight = 0;
							int lastout = 0;
							int gdpt = 0;
							int gdspt = 0;
							int entertime = 0;
							int job = 0;
							int lv = 0;
							int gender = 0;
							int cloth = 0; 
							int weapon = 0;
							int wing = 0;

							s>>seed_name;
							s>>nick_name;
							s>>title;
							s>>job;
							s>>lv;
							s>>fight;
							s>>lastout;
							s>>gdspt;
							s>>gdpt;
							s>>entertime;
							if (version == 107)
							{
								s>>gender;
								s>>cloth;
								s>>weapon;
								s>>wing;
							}
							//printf("DBManager: 103: %s %s %d %d %d %d\n",seed_name,nick_name,title,fight,lastout,gdpt);
							if( s && title != 101 )
							{
								guild->AddMember(nick_name.c_str(),seed_name.c_str(),title,NULL);
								guild->SetMemberTitle(seed_name,title);
								if (title == 200)
									guild->AddAdvNum();
								if (title == 300)
									guild->AddViceAdminNum();
								guild->SetMemberGuildPt(seed_name,gdpt,0);
								guild->SetMemberEnterTime(seed_name,entertime);
								guild->SetMemberFight(seed_name,fight);
								guild->SetMemberJob(seed_name,job);
								guild->SetMemberLv(seed_name,lv);
								guild->SetMemberGender(seed_name,gender);
								guild->SetMemberCloth(seed_name,cloth);
								guild->SetMemberWeapon(seed_name,weapon);
								guild->SetMemberWing(seed_name,wing);
							}
						}
					}
				}
			}

			{
				const char* data = m_pDBModule->FetchField(row,res,"enemys",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					int size;
					s>>version>>size;
					if( s && version==100 && size >0 )
					{
						for(int i = 0;i < size;i++)
						{
							std::string guild_name;
							s>>guild_name;
							if( s )
							{
								guild->AddEnemyGuild(guild_name);
							}
						}
					}
				}
			}

			{
				const char* data = m_pDBModule->FetchField(row,res,"friends",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					int size;
					s>>version>>size;
					if( s && version==100 && size >0 )
					{
						for(int i = 0;i < size;i++)
						{
							std::string guild_name;
							s>>guild_name;
							if( s )
							{
								guild->AddFriendGuild(guild_name);
							}
						}
					}
				}
			}

			{
				//guild->GetParams().clear();
				const char* data = m_pDBModule->FetchField(row,res,"param",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					int size;
					s>>version>>size;
					if( s && version==100 && size >0 )
					{
						for(int i = 0;i < size;i++)
						{
							std::string k;
							std::string v;
							s>>k>>v;
							if( ! v.empty() )
							{
								guild->Var(k,v);
							}
						}
					}
				}
			}
			{
				guild->GetItems().clear();
				const char* data = m_pDBModule->FetchField(row,res,"items",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					int size;
					s>>version>>size;
					if( s && version==100 && size >0 )
					{
						for(int i = 0;i < size;i++)
						{
							int pos;
							int typeID;
							int level;
							s>>pos>>typeID>>level;
							if(pos >= 0 && typeID > 0)
							{
								OGSLib::GuildItem guildItem;
								guildItem.mTypeID = typeID;
								guildItem.mLevel = level;
								guild->GetItems()[pos] = guildItem;
							}
						}
					}
				}
			}
			
			{
				guild->ClearRedPacketInfo();
				OGSLib::GuildRedPacket& red_packet_info = guild->GetRedPacketInfo();
				const char* data = m_pDBModule->FetchField(row,res,"red_packet_info",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					s>>version;
					if( s && version==100)
					{
						int nTime = 0; 
						int size = 0;
						std::string name;

						s >> red_packet_info.mSenderSeedName >> red_packet_info.mSenderName >> nTime >> red_packet_info.mRemainValue >> red_packet_info.mRemainSize;
						red_packet_info.mSendTime = nTime;
						
						s >> size;
						for (int i = 0; i < size; ++ i)
						{
							s >> name;
							red_packet_info.mTakeNames.insert(name);
						}

						s >> size;
						for (int i = 0; i < size; ++ i)
						{
							OGSLib::GuildRedPacketRecord record;
							s >> record.name >> record.value >> record.count >> record.op_code;
							red_packet_info.mRecordList.push_back(record);
						}
					}
				}
			}
			{
				guild->ClearItemRecords();
				OGSLib::Guild::ItemOpRecordList& item_op_list = guild->GetGuildItemOpLogList();
				const char* data = m_pDBModule->FetchField(row,res,"item_op_records",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version, size;
					s>>version>>size;
					int time = 0;
					if( s && version==100 && size > 0)
					{
						for (int i = 0; i < size; ++ i)
						{
							OGSLib::GuildItemOpRecord record;
							s >> record.name >> record.item_type >> record.op_code >> time;
							record.time = time;
							item_op_list.push_back(record);
						}
					}
				}
			}
			{
				// 加载行会战列表
				guild->ClearGuildWar();
				OGSLib::Guild::GuildWarMap& war_map = guild->GetGuildWarMap();
 				const char* data = m_pDBModule->FetchField(row,res,"guildwar",len);
 				if( data && len > 0 )
 				{
 					LEUD::StreamFix s(data,len,len);
 					int version, size;
 					s>>version>>size;
 					int time = 0;
 					if( s && version==100 && size > 0)
 					{
 						for (int i = 0; i < size; ++ i)
 						{
 							OGSLib::GuildWar guild_war;
							s >> guild_war.mGuildSeed >> guild_war.mAct >> time;
							guild_war.mStartTime = time;
							war_map[guild_war.mGuildSeed] = guild_war;
 						}
 					}
 				}
			}

			if( (! guild->SeedID().empty()) && guild->GetMemberNumber() > 0 )
			{
				//printf("guild -map guilds %s %s %d\n",guild->SeedID().c_str(),guild->GetName().c_str(),guild->GetMemberNumber());
				gm[guild->SeedID()] = guild;
			}
			else
			{
				//printf("guild empty guilds %s %s %d\n",guild->SeedID().c_str(),guild->GetName().c_str(),guild->GetMemberNumber());
				delete guild;guild=0;
			}
		}
		m_pDBModule->FreeResult(res);
	}

	for( OGSLib::ManagerGuild::GuildMap::const_iterator guild_pos = gm.begin();
		guild_pos != gm.end();
		++ guild_pos )
	{
		OGSLib::Guild* guild = guild_pos->second;
		std::stringstream ss;
		ss<<"select * from guildinfo where guild_id='"<<guild->SeedID()<<"' and server_id='"<<m_App->m_ServerID<<"' and del_flag=0";
		MYSQL_RES* res = m_pDBModule->QueryUse(ss.str().c_str());
		if( res )
		{
			MYSQL_ROW row;
			while( row = m_pDBModule->FetchRow(res) )
			{
				int len;
				const char* tp;
				tp = m_pDBModule->FetchField(row,res,"guild_desp",len);
				if( tp ) guild->SetDesp(std::string(tp));
				tp = m_pDBModule->FetchField(row,res,"guild_notice",len);
				if( tp ) guild->SetNotice(std::string(tp));
			}
		}
		m_pDBModule->FreeResult(res);
	}
	guild_manager.OnLoadGuilds(gm);
	return 0;
}
int DBManager::SaveGuilds(const OGSLib::ManagerGuild& guild_manager)
{
	char temp[1024];
	char desp[1024];desp[0]=0;
	for( OGSLib::ManagerGuild::GuildMap::const_iterator guild_pos = guild_manager.GetGuilds().begin();
		guild_pos != guild_manager.GetGuilds().end();
		++ guild_pos )
	{
		OGSLib::Guild* guild = guild_pos->second;
		std::stringstream ss;
		ss<<"replace guild2 set ";
		m_pDBModule->EscapeString(temp,guild->SeedID().c_str(),(unsigned long)guild->SeedID().length());
		ss<<"guild_id='"<<temp;
		m_pDBModule->EscapeString(temp,guild->GetName().c_str(),(unsigned long)guild->GetName().length());
		ss<<"',guild_name='"<<temp;
		ss<<"',del_flag="<<guild_pos->second->deleted();
		ss<<",server_id='"<<m_App->m_ServerID;
		ss<<"',mslv='"<<guild_pos->second->master_level();
		ss<<"',guild_lv='"<<guild_pos->second->guild_level();
		ss<<"',guild_pt='"<<guild_pos->second->guild_pt();
		ss<<"',guild_desp='',guild_notice='";
		//m_pDBModule->EscapeString(temp,guild->GetDesp().c_str(),(unsigned long)guild->GetDesp().length());
		//ss<<"',guild_desp='"<<temp;
		//m_pDBModule->EscapeString(temp,guild->GetNotice().c_str(),(unsigned long)guild->GetNotice().length());
		//ss<<"',guild_notice='"<<temp;
		{
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=107;
			s<<version;
			s<<static_cast<int>(guild->GetMembers().size());
			for( OGSLib::Guild::MemberMap::const_iterator pos = guild->GetMembers().begin();
				pos != guild->GetMembers().end();
				++ pos )
			{
				s<<pos->second.seed_name
					<<pos->second.nick_name
					<<pos->second.title
					<<pos->second.job
					<<pos->second.lv
					<<pos->second.fight
					<<pos->second.lastlogin
					<<pos->second.guildspt
					<<pos->second.guildpt
					<<pos->second.entertime
					<<pos->second.gender
					<<pos->second.cloth
					<<pos->second.weapon
					<<pos->second.wing;
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',members='"<<m_Buffer2;
		}
		{
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=100;
			s<<version;
			s<<static_cast<int>(guild->GetEnemyGuilds().size());
			for( OGSLib::Guild::GuildList::const_iterator pos = guild->GetEnemyGuilds().begin();
				pos != guild->GetEnemyGuilds().end();
				++ pos )
			{
				s<<*pos;
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',enemys='"<<m_Buffer2;
		}
		{
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=100;
			s<<version;
			s<<static_cast<int>(guild->GetFriendGuilds().size());
			for( OGSLib::Guild::GuildList::const_iterator pos = guild->GetFriendGuilds().begin();
				pos != guild->GetFriendGuilds().end();
				++ pos )
			{
				s<<*pos;
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',friends='"<<m_Buffer2;
		}
		{
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=100;
			s<<version;
			s<<static_cast<int>(guild->GetParams().size());
			for( OGSLib::Guild::ParamMap::const_iterator pos = guild->GetParams().begin();
				pos != guild->GetParams().end();
				++ pos )
			{
				s<<pos->first<<pos->second;
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',param='"<<m_Buffer2;
		}
		{
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=100;
			s<<version;
			s<<static_cast<int>(guild->GetItems().size());
			for( OGSLib::Guild::ItemMap::const_iterator pos = guild->GetItems().begin();
				pos != guild->GetItems().end();
				++ pos )
			{
				s << pos->first << pos->second.mTypeID << pos->second.mLevel;
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',items='"<<m_Buffer2;
		}
		{
			// 保存红包记录
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=100;
			s<<version;
			OGSLib::GuildRedPacket &red_packet_info = guild->GetRedPacketInfo();
			// 红包发送者 ,发送时间, 剩余值
			s << red_packet_info.mSenderSeedName << red_packet_info.mSenderName << static_cast<int>(red_packet_info.mSendTime) << red_packet_info.mRemainValue << red_packet_info.mRemainSize;
			// 领取者
			s << static_cast<int>(red_packet_info.mTakeNames.size());
			for (std::set<std::string>::iterator it = red_packet_info.mTakeNames.begin(); it != red_packet_info.mTakeNames.end(); ++ it)
			{
				s << *it;
			}
			// 红包记录
			s << static_cast<int>(red_packet_info.mRecordList.size());
			for (std::list<OGSLib::GuildRedPacketRecord>::iterator it = red_packet_info.mRecordList.begin(); it != red_packet_info.mRecordList.end(); ++it)
			{
				s << (*it).name << (*it).value << (*it).count << (*it).op_code;
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
 			ss<<"',red_packet_info='"<<m_Buffer2;
		}
		{
			// 保存物品日志
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=100;
			s<<version;
			OGSLib::Guild::ItemOpRecordList &item_op_list = guild->GetGuildItemOpLogList();
			s << static_cast<int>(item_op_list.size());
			for (OGSLib::Guild::ItemOpRecordList::iterator it = item_op_list.begin(); it != item_op_list.end(); ++it)
			{
				s << (*it).name << (*it).item_type << (*it).op_code << static_cast<int>((*it).time);
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',item_op_records='"<<m_Buffer2;
		}
		{
			// 保存行会战
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=100;
			s<<version;
			OGSLib::Guild::GuildWarMap &war_map = guild->GetGuildWarMap();
			s << static_cast<int>(war_map.size());
			for (OGSLib::Guild::GuildWarMap::iterator it = war_map.begin(); it != war_map.end(); ++it)
			{
				s << it->second.mGuildSeed << it->second.mAct << static_cast<int>(it->second.mStartTime);
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',guildwar='"<<m_Buffer2<<"'";
		}
		//m_pDBModule->EscapeString(temp,guild->GetName().c_str(),(unsigned long)guild->GetName().length());
		//ss<<"guild_name='"<<temp;
		//ss<<"where server_id="<<m_App->m_ServerID<<" and guild_name='"<<temp<<"'";
		//if( guild->mSaveID >= 0 )
		//{
		//	ss<<"where id='"<<guild->mSaveID<<"'";
		//}
		m_pDBModule->QueryStore(ss.str().c_str(),true);

		std::stringstream sstr;
		sstr<<"replace guildinfo set ";
		m_pDBModule->EscapeString(temp,guild->SeedID().c_str(),(unsigned long)guild->SeedID().length());
		sstr<<"guild_id='"<<temp;
		m_pDBModule->EscapeString(temp,guild->GetName().c_str(),(unsigned long)guild->GetName().length());
		sstr<<"',guild_name='"<<temp;
		sstr<<"',server_id='"<<m_App->m_ServerID;
		m_pDBModule->EscapeString(temp,guild->GetDesp().c_str(),(unsigned long)guild->GetDesp().length());
		sstr<<"',guild_desp='"<<temp;
		m_pDBModule->EscapeString(temp,guild->GetNotice().c_str(),(unsigned long)guild->GetNotice().length());
		sstr<<"',guild_notice='"<<temp<<"'";
		m_pDBModule->QueryStore(sstr.str().c_str(),true);
	}
	return 0;
}
int DBManager::LoadCouples(OGSLib::ManagerCouple& couple_manager)
{
	std::stringstream ss;
	ss<<"select * from couple where state = '0'";
	OGSLib::ManagerCouple::CoupleMap gm;
	MYSQL_RES* res = m_pDBModule->QueryUse(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			OGSLib::Couple* couple = new OGSLib::Couple();
			couple->CoupleID(atoi(m_pDBModule->FetchField(row,res,"couple_id",len)));
			couple->SetWeddingDate(atoi(m_pDBModule->FetchField(row,res,"weddingdate",len)));
			const char* tp;
			tp = m_pDBModule->FetchField(row,res,"male_desp",len);
			std::string d=tp;
			if( tp ) couple->SetMaleDesp(d);
			tp = m_pDBModule->FetchField(row,res,"female_desp",len);
			d=tp;
			if( tp ) couple->SetFemaleDesp(d);
			{
				const char* data = m_pDBModule->FetchField(row,res,"members",len);
				if( data && len > 0 )
				{
					LEUD::StreamFix s(data,len,len);
					int version;
					int size;
					s>>version>>size;
					if( s && version==100 && size >0 )
					{
						for(int i = 0;i < size;i++)
						{
							std::string nick_name;
							int gender;
							int job;
							int level;
							int married_times;
							int last;
							s>>nick_name;
							s>>gender;
							s>>job;
							s>>level;
							s>>married_times;
							s>>last;
							if( s )
							{
								couple->AddMember(nick_name,nick_name,gender,job,level,married_times,last);
							}
						}
					}
					if( s && version==101 && size >0 )
					{
						for(int i = 0;i < size;i++)
						{
							std::string nick_name;
							std::string seed_name;
							int gender;
							int job;
							int level;
							int married_times;
							int last;
							s>>nick_name;
							s>>seed_name;
							s>>gender;
							s>>job;
							s>>level;
							s>>married_times;
							s>>last;
							if( s )
							{
								couple->AddMember(seed_name,nick_name,gender,job,level,married_times,last);
							}
						}
					}
				}
			}
			if( ( couple->CoupleID() > 0) && couple->GetMembers().size() == 2 )
			{
				gm[couple->WeddingDate()] = couple;
			}
			else
			{
				delete couple;couple=0;
			}
		}
		m_pDBModule->FreeResult(res);
	}
	couple_manager.OnLoadCouples(gm);
	return 0;
}
int DBManager::SaveCouples(const OGSLib::ManagerCouple& couple_manager)
{
	char temp[1024];
	for( OGSLib::ManagerCouple::CoupleMap::const_iterator couple_pos = couple_manager.GetCouples().begin();
		couple_pos != couple_manager.GetCouples().end();
		++ couple_pos )
	{
		OGSLib::Couple* couple = couple_pos->second;
		std::stringstream ss;
		ss<<"replace couple set ";
		ss<<"couple_id='"<<couple->CoupleID();
		ss<<"',weddingdate='"<<couple->WeddingDate();
		ss<<"',state='"<<couple->State();
		m_pDBModule->EscapeString(temp,couple->GetMaleDesp().c_str(),(unsigned long)couple->GetMaleDesp().length());
		ss<<"',male_desp='"<<temp;
		m_pDBModule->EscapeString(temp,couple->GetFemaleDesp().c_str(),(unsigned long)couple->GetFemaleDesp().length());
		ss<<"',female_desp='"<<temp ;

		{
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			int version=101;
			s<<version;
			s<<static_cast<int>(couple->GetMembers().size());
			for( OGSLib::Couple::MemberMap::const_iterator pos = couple->GetMembers().begin();
				pos != couple->GetMembers().end();
				++ pos )
			{
				s<<pos->second.nick_name
					<<pos->second.seed_name
					<<pos->second.gender
					<<pos->second.job
					<<pos->second.level
					<<pos->second.married_times
					<<pos->second.lastLoginTime;
			}
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			ss<<"',members='"<<m_Buffer2<<"'";
		}
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	return 0;
}
void DBManager::game_exe_sql(const char* sql)
{
	if( ! m_pDBModule ) return;
	m_pDBModule->QueryStore(sql,true);
}
int DBManager::game_store_sql(const char* sql,sexy_db_mysql::mem_db_res& res,long* effect_row_num,long* new_insert_id){
	my_ulonglong ern;
	*effect_row_num=0;
	*new_insert_id=0;
	MYSQL_RES* r = m_pDBModule->QueryStore(sql,false,&ern);
	if( r )
	{
		*effect_row_num=(long)ern;
		res=sexy_db_mysql::mem_db_res::create_from_mysqlres(r);
		m_pDBModule->FreeResult(r);
		return 0;
	}
	return -1;
}
int DBManager::LoadAuto(std::vector<OGSLib::AutoRunDB>* ress)
{
	std::string ss = "select unix_timestamp(begintime) bt,beginscript,unix_timestamp(endtime) et,endscript from auto where begintime > now() or endtime > now()";
	MYSQL_RES* res = m_pDBModule->QueryUse(ss.c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			const char* bt;
			const char* bs;
			const char* et;
			const char* es;
			bt = m_pDBModule->FetchField(row,res,"bt",len);
			bs = m_pDBModule->FetchField(row,res,"beginscript",len);
			et = m_pDBModule->FetchField(row,res,"et",len);
			es = m_pDBModule->FetchField(row,res,"endscript",len);
			if( bt && bs && et && es )
			{
				OGSLib::AutoRunDB db;
				db.mBeginTime = atoi(bt);
				db.mBeginScript = bs;
				db.mEndTime = atoi(et);
				db.mEndScript = es;
				ress->push_back(db);
			}
		}
		m_pDBModule->FreeResult(res);
	}
	return 0;
}
int DBManager::LoadServerVar(std::map<std::string,std::string>& var_map)
{
	std::stringstream ss;
	ss<<"select * from vars where server_id='"<<m_App->m_ServerID<<"' and type='1'";
	MYSQL_RES* res = m_pDBModule->QueryUse(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			const char* key;
			const char* value;
			key = m_pDBModule->FetchField(row,res,"k",len);
			value = m_pDBModule->FetchField(row,res,"v",len);
			if( key && value )
			{
				var_map[key] = value;
			}
		}
		m_pDBModule->FreeResult(res);
	}
	return 0;
}
int DBManager::SaveServerVar(const std::map<std::string,std::string>& var_map)
{
	char temp[1024];
	{
		std::stringstream ss;
		ss<<"update vars set type='2' where server_id='"<<m_App->m_ServerID<<"' and type='1'";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	{
		for( std::map<std::string,std::string>::const_iterator pos = var_map.begin();
			pos != var_map.end();
			++ pos )
		{
			std::stringstream ss;
			ss<<"insert into vars set ";
			ss<<"server_id='"<<m_App->m_ServerID;
			ss<<"',type='1";
			m_pDBModule->EscapeString(temp,pos->first.c_str(),(unsigned long)pos->first.length());
			ss<<"',k='"<<temp;
			m_pDBModule->EscapeString(temp,pos->second.c_str(),(unsigned long)pos->second.length());
			ss<<"',v='"<<temp<<"'";
			m_pDBModule->QueryStore(ss.str().c_str(),true);
		}
	}
	{
		std::stringstream ss;
		ss<<"delete from vars where server_id='"<<m_App->m_ServerID<<"' and type='2'";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	return 0;
}
void DBManager::LoadChinaLimit(const char* account,int* online_time_count,int* offline_time_tag,int* china_limit_ok,char* cn_name,char* cn_id)
{
	*online_time_count = 0;
	*offline_time_tag = 0;
	*china_limit_ok = 0;
	cn_name[0] = 0;
	cn_id[0]=0;
	_snprintf_s(m_Buffer1,m_BufferLen-1,m_BufferLen-1,"select * from fcm where account='%s'",account);m_Buffer1[m_BufferLen-1]=0;
	MYSQL_RES* res = m_pDBModule->QueryStore(m_Buffer1);
	if( res )
	{
		MYSQL_ROW row;
		if( row = m_pDBModule->FetchRow(res) )
		{
			int len;
			*online_time_count = atoi(m_pDBModule->FetchField(row,res,"online_time",len));
			*offline_time_tag = atoi(m_pDBModule->FetchField(row,res,"off_time",len));
			*china_limit_ok = atoi(m_pDBModule->FetchField(row,res,"cn_ok",len));
			strncpy_s(cn_name,127,m_pDBModule->FetchField(row,res,"cn_name",len),127);
			strncpy_s(cn_id,127,m_pDBModule->FetchField(row,res,"cn_id",len),127);
		}
		m_pDBModule->FreeResult(res);
	}

	if (*china_limit_ok == 102)
	{
		{
			_snprintf_s(m_Buffer1,m_BufferLen-1,m_BufferLen-1,"select max(online_time) online_count from fcm where cn_id='%s'",cn_id);m_Buffer1[m_BufferLen-1]=0;
			MYSQL_RES* res = m_pDBModule->QueryStore(m_Buffer1);
			if( res )
			{
				MYSQL_ROW row;
				if( row = m_pDBModule->FetchRow(res) )
				{
					int len;
					*online_time_count = atoi(m_pDBModule->FetchField(row,res,"online_count",len));
				}
				m_pDBModule->FreeResult(res);
			}
		}
		{
			_snprintf_s(m_Buffer1,m_BufferLen-1,m_BufferLen-1,"select max(off_time) max_off_time from fcm where cn_id='%s'",cn_id);m_Buffer1[m_BufferLen-1]=0;
			MYSQL_RES* res = m_pDBModule->QueryStore(m_Buffer1);
			if( res )
			{
				MYSQL_ROW row;
				if( row = m_pDBModule->FetchRow(res) )
				{
					int len;
					*offline_time_tag = atoi(m_pDBModule->FetchField(row,res,"max_off_time",len));
				}
				m_pDBModule->FreeResult(res);
			}
		}
	}
}
void DBManager::SaveChinaLimit(const char* account,int online_time_count,int offline_time_tag,int china_limit_ok,const char* cn_name,const char* cn_id)
{
	_snprintf_s(m_Buffer1,m_BufferLen-1,m_BufferLen-1,"replace fcm values ('%s',%d,%d,'%s','%s',%d)",account,offline_time_tag,online_time_count,cn_name,cn_id,china_limit_ok);
	m_pDBModule->QueryStore(m_Buffer1,true);
}

int DBManager::DoLoadConsignment(std::vector<OGSLib::ConsignableItemInfo*>& info_list, std::map<std::string, int>& user_vcoin_map)
{
	std::stringstream ss;
	ss << "select player.chrname, consignment.chr_seed_name, consignment.item, consignment.price, consignment.end_time "
		<< "from player, consignment "
		<< "where consignment.server_id=" << m_App->m_ServerID << " "
		<< "and player.seedname = consignment.chr_seed_name "
		<< "and player.server_id = consignment.server_id";

	MYSQL_RES* res = m_pDBModule->QueryUse(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			OGSLib::ConsignableItemInfo * item_info = new OGSLib::ConsignableItemInfo;
			int len = 0;
			const char* tp = NULL;
			tp = m_pDBModule->FetchField(row,res,"chrname",len);
			if (tp)
				item_info->seller_name = tp;

			tp = m_pDBModule->FetchField(row,res,"chr_seed_name",len);
			if (tp)
				item_info->seller_seed_name = tp;

			tp = m_pDBModule->FetchField(row,res,"price",len);
			if (tp)
				item_info->price = atoi(tp);

			tp = m_pDBModule->FetchField(row,res,"end_time",len);
			if (tp)
				item_info->end_time = atoi(tp);

			tp = m_pDBModule->FetchField(row,res,"item",len);
			if (tp)
			{
				LEUD::StreamFix s(tp,len,len);
				OGSLib::SubItem& item = item_info->item;
				s>>item.mPosition
					>>item.mTypeID
					>>item.mDuration
					>>item.mDuraMax
					>>item.mItemFlags
					>>item.mLuck
					>>item.mLevel
					>>item.mNumber
					>>item.mAddAC
					>>item.mAddMAC
					>>item.mAddDC
					>>item.mAddMC
					>>item.mAddSC
					>>item.mUpdAC
					>>item.mUpdMAC
					>>item.mUpdDC
					>>item.mUpdMC
					>>item.mUpdSC
					>>item.mCreateTime
					>>item.mIdentId
					>>item.mProtect
					>>item.mAddHp
					>>item.mAddMp
					>>item.mUpdMaxCount
					>>item.mUpdFailedCount
					>>item.mSellPriceType
					>>item.mSellPrice
					>>item.mZLevel
					>>item.mLock;
				OGSLib::ItemDef* id = m_App->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(item.mTypeID);
				if (id)
				{
					item.mSubType = id->SubType;
				}
			}
			info_list.push_back(item_info);
		}
		m_pDBModule->FreeResult(res);
	}

	{
		std::stringstream ss2;
		ss2 << "select chrname, consignment_vcoin.vcoin from player, consignment_vcoin  "
			<< "where seedname = seed_name  "
			<< "and consignment_vcoin.server_id = " << m_App->m_ServerID << " "
			<< "and type = 1";
		res = m_pDBModule->QueryUse(ss2.str().c_str());
		if( res )
		{
			MYSQL_ROW row;
			while( row = m_pDBModule->FetchRow(res) )
			{
				std::string chrname;
				int vcoin = 0;
				const char* tp = NULL;
				int len = 0;
				tp = m_pDBModule->FetchField(row,res,"chrname",len);
				if (!tp)
					continue;

				chrname = tp;

				tp = m_pDBModule->FetchField(row,res,"vcoin",len);
				if (tp)
					vcoin = atoi(tp);
				user_vcoin_map[chrname] = vcoin;
			}
			m_pDBModule->FreeResult(res);
		}
	}
	return 0;
}

int DBManager::DoSaveConsignment(std::vector<OGSLib::ConsignableItemInfo*>& info_list, std::map<std::string, int>& user_vcoin_map)
{
	{
		std::stringstream ss;
		ss<<"update consignment set type=2 where server_id="<<m_App->m_ServerID<<" and type=1";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	{
		for( size_t i = 0; i < info_list.size(); ++ i)
		{
			LEUD::StreamFix s(m_Buffer1,m_BufferLen,0);
			s<<info_list[i]->item.mPosition
				<<info_list[i]->item.mTypeID
				<<info_list[i]->item.mDuration
				<<info_list[i]->item.mDuraMax
				<<info_list[i]->item.mItemFlags
				<<info_list[i]->item.mLuck
				<<info_list[i]->item.mLevel
				<<info_list[i]->item.mNumber
				<<info_list[i]->item.mAddAC
				<<info_list[i]->item.mAddMAC
				<<info_list[i]->item.mAddDC
				<<info_list[i]->item.mAddMC
				<<info_list[i]->item.mAddSC
				<<info_list[i]->item.mUpdAC
				<<info_list[i]->item.mUpdMAC
				<<info_list[i]->item.mUpdDC
				<<info_list[i]->item.mUpdMC
				<<info_list[i]->item.mUpdSC
				<<info_list[i]->item.mCreateTime
				<<info_list[i]->item.mIdentId
				<<info_list[i]->item.mProtect
				<<info_list[i]->item.mAddHp
				<<info_list[i]->item.mAddMp
				<<info_list[i]->item.mUpdMaxCount
				<<info_list[i]->item.mUpdFailedCount
				<<info_list[i]->item.mSellPriceType
				<<info_list[i]->item.mSellPrice
				<<info_list[i]->item.mZLevel
				<<info_list[i]->item.mLock;
			unsigned long escape_len=m_pDBModule->EscapeString(m_Buffer2,m_Buffer1,(unsigned long)s.size());
			std::stringstream ss;
			ss  << "insert into consignment set server_id=" << m_App->m_ServerID<<" "
				<< ",chr_seed_name='" << info_list[i]->seller_seed_name <<"' "
				<< ",item='" << m_Buffer2 << "' "
				<< ",price=" << info_list[i]->price << " "
				<< ",end_time=" << info_list[i]->end_time << " "
				<< ",type=1";
 			m_pDBModule->QueryStore(ss.str().c_str(),true);
		}
 	}
	{
		std::stringstream ssSql;
		ssSql<<"delete from consignment where server_id=" << m_App->m_ServerID << " and type=2";
		m_pDBModule->QueryStore(ssSql.str().c_str(),true);
	}

	{
		std::stringstream ss;
		ss<<"update consignment_vcoin set type=2 where server_id="<<m_App->m_ServerID<<" and type=1";
		m_pDBModule->QueryStore(ss.str().c_str(),true);
	}
	{
		std::map<std::string, int>::iterator it = user_vcoin_map.begin();
		while (it != user_vcoin_map.end())
		{
			std::stringstream ss;
			ss  << "insert into consignment_vcoin set server_id=" << m_App->m_ServerID<<" "
				<< ",seed_name=(select seedname from player where chrname ='"<< it->first <<"')"
				<< ",vcoin=" << it->second << " "
				<< ",type=1";
			m_pDBModule->QueryStore(ss.str().c_str(),true);
			++ it;
		}
	}
	{
		std::stringstream ssSql;
		ssSql<<"delete from consignment_vcoin where server_id=" << m_App->m_ServerID << " and type=2";
		m_pDBModule->QueryStore(ssSql.str().c_str(),true);
	}
	return 0;
}

class PushDBEventLoadRelationCache : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventLoadRelationCache >
{
public:
 	DBManager* manager;
 	std::vector<OGSLib::RelationShipOper*> opers_list;
	int GUID;
	int result;
	PushDBEventLoadRelationCache()
	{
		result = 0;
	}
	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
			result = manager->DoLoadRelationCache(opers_list);
			manager->m_pEventQueue->PushEvent(this);
			return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnLoadRelationCache(GUID,0,opers_list);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnLoadRelationCache(GUID,1,opers_list);
			}
		}
		delete this;
	}
};

int DBManager::push_load_relation_cache(int GUID)
{
	PushDBEventLoadRelationCache* e = new PushDBEventLoadRelationCache;
	e->GUID = GUID;
	e->manager = this;
	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}

class PushDBEventSaveRelationCache : public LEUD::EventBase,public LEUD::PoolMemoryBase< PushDBEventSaveRelationCache >
{
public:
	DBManager* manager;
	std::vector<OGSLib::RelationShipOper *> relation_oper_list;
	int GUID;
	int result;
	PushDBEventSaveRelationCache()
	{
		result = 0;
	}
	void dest_data()
	{
		for (size_t i = 0; i < relation_oper_list.size(); ++ i)
		{
			delete relation_oper_list[i];
		}
		relation_oper_list.clear();
	}

	virtual void OnHanding(unsigned int queue_id,void* param)
	{
		if( queue_id == 100 )
		{
 			result = manager->DoSaveRelationCache(relation_oper_list);
 			manager->m_pEventQueue->PushEvent(this);
			return;
		}
		if( queue_id == 200 )
		{
			if( result == 0 )
			{
				manager->m_App->m_pGameServerLib->OnSaveRelationCache(GUID,0);
			}
			else
			{
				manager->m_App->m_pGameServerLib->OnSaveRelationCache(GUID,1);
			}
		}

		dest_data();
		delete this;
	}
};

int DBManager::push_save_relation_cache(OGSLib::ManagerRelationCache& realtioncache_manager,int GUID)
{
	PushDBEventSaveRelationCache* e = new PushDBEventSaveRelationCache;
	e->GUID = GUID; 
	e->manager = this;
	realtioncache_manager.CopyRelationOperData(e->relation_oper_list);

	m_pDBThread->PushEvent(e);
#ifndef WIN32
	m_pDBThread->Wake();
#endif
	return 0;
}

int DBManager::DoLoadRelationCache(std::vector<OGSLib::RelationShipOper*>& oper_list)
{
	std::stringstream ss;
	ss << "select name, oper_name, title, time "
		<< "from relation_cache "
		<< "where server_id=" << m_App->m_ServerID;

	MYSQL_RES* res = m_pDBModule->QueryUse(ss.str().c_str());
	if( res )
	{
		MYSQL_ROW row;
		while( row = m_pDBModule->FetchRow(res) )
		{
			OGSLib::RelationShipOper * oper = new OGSLib::RelationShipOper;
			int len = 0;
			const char* tp = NULL;
			tp = m_pDBModule->FetchField(row,res,"name",len);
			if (tp)
				oper->name = tp;

			tp = m_pDBModule->FetchField(row,res,"oper_name",len);
			if (tp)
				oper->name_oper = tp;

			tp = m_pDBModule->FetchField(row,res,"title",len);
			if (tp)
				oper->title = atoi(tp);

			tp = m_pDBModule->FetchField(row,res,"time",len);
			if (tp)
				oper->time = atoi(tp);

			oper_list.push_back(oper);
		}
		m_pDBModule->FreeResult(res);
	}
	return 0;
}

int DBManager::DoSaveRelationCache(std::vector<OGSLib::RelationShipOper*>& oper_list)
{
	{
		std::stringstream ssSql;
		ssSql<<"delete from relation_cache where server_id=" << m_App->m_ServerID;
		m_pDBModule->QueryStore(ssSql.str().c_str(),true);
	}
	{
		for( size_t i = 0; i < oper_list.size(); ++ i)
		{
			std::stringstream ss;
			ss  << "insert into relation_cache set server_id=" << m_App->m_ServerID<<" "
				<< ",name='" << oper_list[i]->name <<"' "
				<< ",oper_name='" << oper_list[i]->name_oper << "' "
				<< ",title=" << oper_list[i]->title << " "
				<< ",time="<< oper_list[i]->time;
			m_pDBModule->QueryStore(ss.str().c_str(),true);
		}
	}
	return 0;
}

LEUD::PoolMemory<PushDBEventLoadCharacter>		LEUD::PoolMemoryBase<PushDBEventLoadCharacter>::memPool("PushDBEventLoadCharacter");
LEUD::PoolMemory<PushDBEventSaveCharacter>		LEUD::PoolMemoryBase<PushDBEventSaveCharacter>::memPool("PushDBEventSaveCharacter");

LEUD::PoolMemory<PushDBEventLoadGuild>			LEUD::PoolMemoryBase<PushDBEventLoadGuild>::memPool("PushDBEventLoadGuild");
LEUD::PoolMemory<PushDBEventSaveGuild>			LEUD::PoolMemoryBase<PushDBEventSaveGuild>::memPool("PushDBEventSaveGuild");

LEUD::PoolMemory<PushDBEventLoadCouple>			LEUD::PoolMemoryBase<PushDBEventLoadCouple>::memPool("PushDBEventLoadCouple");
LEUD::PoolMemory<PushDBEventSaveCouple>			LEUD::PoolMemoryBase<PushDBEventSaveCouple>::memPool("PushDBEventSaveCouple");

LEUD::PoolMemory<PushDBEventLoadServerVar>		LEUD::PoolMemoryBase<PushDBEventLoadServerVar>::memPool("PushDBEventLoadServerVar");
LEUD::PoolMemory<PushDBEventSaveServerVar>		LEUD::PoolMemoryBase<PushDBEventSaveServerVar>::memPool("PushDBEventSaveServerVar");

LEUD::PoolMemory<PushDBEventListCharacter>		LEUD::PoolMemoryBase<PushDBEventListCharacter>::memPool("PushDBEventListCharacter");
LEUD::PoolMemory<PushDBEventCreateCharacter>	LEUD::PoolMemoryBase<PushDBEventCreateCharacter>::memPool("PushDBEventCreateCharacter");
LEUD::PoolMemory<PushDBEventDeleteCharacter>	LEUD::PoolMemoryBase<PushDBEventDeleteCharacter>::memPool("PushDBEventDeleteCharacter");

LEUD::PoolMemory<PushDBEventAuthModifyName>		LEUD::PoolMemoryBase<PushDBEventAuthModifyName>::memPool("PushDBEventAuthModifyName");

LEUD::PoolMemory<PushDBEventLoadAuto>			LEUD::PoolMemoryBase<PushDBEventLoadAuto>::memPool("PushDBEventLoadAuto");
LEUD::PoolMemory<PushDBEventStoreSql>			LEUD::PoolMemoryBase<PushDBEventStoreSql>::memPool("PushDBEventStoreSql");
LEUD::PoolMemory<PushDBEventFreshGift>			LEUD::PoolMemoryBase<PushDBEventFreshGift>::memPool("PushDBEventFreshGift");

LEUD::PoolMemory<PushDBEventExeSql>				LEUD::PoolMemoryBase<PushDBEventExeSql>::memPool("PushDBEventExeSql");
LEUD::PoolMemory<PushDBEventLoadMail>			LEUD::PoolMemoryBase<PushDBEventLoadMail>::memPool("PushDBEventLoadMail");
LEUD::PoolMemory<PushDBEventSaveMail>			LEUD::PoolMemoryBase<PushDBEventSaveMail>::memPool("PushDBEventSaveMail");

LEUD::PoolMemory<PushDBEventLoadConsignment>	LEUD::PoolMemoryBase<PushDBEventLoadConsignment>::memPool("PushDBEventLoadConsignment");
LEUD::PoolMemory<PushDBEventSaveConsignment>	LEUD::PoolMemoryBase<PushDBEventSaveConsignment>::memPool("PushDBEventSaveConsignment");

LEUD::PoolMemory<PushDBEventLoadRelationCache>	LEUD::PoolMemoryBase<PushDBEventLoadRelationCache>::memPool("PushDBEventLoadRelationCache");
LEUD::PoolMemory<PushDBEventSaveRelationCache>	LEUD::PoolMemoryBase<PushDBEventSaveRelationCache>::memPool("PushDBEventSaveRelationCache");

template <> LEUD::PoolMemory<PushDBEventSaveKuafuCharacter>	LEUD::PoolMemoryBase<PushDBEventSaveKuafuCharacter>::memPool("PushDBEventSaveKuafuCharacter");