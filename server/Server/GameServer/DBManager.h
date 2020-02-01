#pragma once
#include "Protocol\Client_GS.h"
namespace OGSLib
{
	class DomainData;
	class ManagerGuild;
	class ManagerConsignment;
	class ManagerRelationCache;
	class ManagerCouple;
	struct AutoRunDB;
	class SubGift;
	class ManagerMail;
	class Mail;
	class ConsignableItemInfo;
	class RelationShipOper;
}
class DBEventQueueThread;

class PushDBEventLoadCharacter;
class PushDBEventSaveCharacter;
class PushDBEventLoadGuild;
class PushDBEventSaveGuild;
class PushDBEventLoadConsignment;
class PushDBEventSaveConsignment;
class PushDBEventLoadRelationCache;
class PushDBEventSaveRelationCache;
class PushDBEventLoadCouple;
class PushDBEventSaveCouple;
class PushDBEventLoadServerVar;
class PushDBEventSaveServerVar;
class PushDBEventListCharacter;
class PushDBEventCreateCharacter;
class PushDBEventDeleteCharacter;
class PushDBEventExeSql;
class PushDBEventLoadMail;
class PushDBEventSaveMail;
class PushDBEventAuthModifyName;
class PushDBEventLoadAuto;
class PushDBEventFreshGift;
class DataFetcher{
public:
	virtual const char* fetch_field(const char* fn,int& len)=0;
};

class CGameServerApp;
class DBManager
{
	friend class CGameServerApp;
	friend class DBEventQueueThread;
	friend class PushDBEventLoadCharacter;
	friend class PushDBEventSaveCharacter;
	friend class PushDBEventLoadGuild;
	friend class PushDBEventSaveGuild;
	friend class PushDBEventLoadCouple;
	friend class PushDBEventSaveCouple;
	friend class PushDBEventLoadServerVar;
	friend class PushDBEventSaveServerVar;
	friend class PushDBEventListCharacter;
	friend class PushDBEventCreateCharacter;
	friend class PushDBEventDeleteCharacter;
	friend class PushDBEventLoadAuto;
	friend class PushDBEventFreshGift;
	friend class PushDBEventStoreSql;
	friend class PushDBEventExeSql;
	friend class PushDBEventLoadMail;
	friend class PushDBEventSaveMail;
	friend class PushDBEventAuthModifyName;
	friend class PushDBEventLoadBattle;
	friend class PushDBEventSaveBattle;
	friend class PushDBEventSaveKuafuCharacter;
	friend class PushDBEventLoadConsignment;
	friend class PushDBEventSaveConsignment;
	friend class PushDBEventLoadRelationCache;
	friend class PushDBEventSaveRelationCache;
	CGameServerApp* m_App;
	LEUD::DBModuleMysql* m_pDBModule;
	DBEventQueueThread* m_pDBThread;
	LEUD::EventQueue* m_pEventQueue;
	size_t m_BufferLen;
	char * m_Buffer1;
	char * m_Buffer2;
	time_t m_PingTime;
public:
	DBManager(CGameServerApp* app);
	~DBManager(void);

	bool Start();
	void Stop();
	void Update();
	void DBUpVersion();
public:
	int push_load_character(const char* account,const char* chrname,const char* seedname,const char* client_ip,int GUID,const char* table);
	int push_save_character(OGSLib::DomainData* character,bool quit,int GUID,const char* table);
	int push_load_guild(int GUID);
	int push_save_guild(const OGSLib::ManagerGuild& guild_manager,int GUID);
	int push_load_consignment(int GUID);
	int push_save_consignment(OGSLib::ManagerConsignment& consignment_manager,int GUID);
	int push_load_relation_cache(int GUID);
	int push_save_relation_cache(OGSLib::ManagerRelationCache& realtioncache_manager,int GUID);
	int push_load_couple(int GUID);
	int push_save_couple(const OGSLib::ManagerCouple& couple_manager,int GUID);
	int push_load_server_var(int GUID);
	int push_save_server_var(const std::map<std::string,std::string>& vars,int GUID);
	int push_load_auto(int GUID);
	int push_store_sql(void* arg,const char* sql);
	void push_list_character(SOCKET sock,std::string account);
	void push_create_character(SOCKET sock,std::string account,std::string chrname,std::string idfa,int pid,int job,int gender,int svrid,std::string youke);
	void push_delete_character(SOCKET sock,std::string& account,const char* chrname);
	void push_auth_modifyname(const char* seed,const char* chrname);
	void push_fresh_gift(const char* seedname);
	int push_exe_sql(const char* sql);
	void push_load_mail(int GUID,int mailValidDays);
	int push_save_mail(OGSLib::ManagerMail& mail_manager,int GUID);
	void UndumpKuafuCharacter(OGSLib::DomainData& chr,DataFetcher* fetcher);
	int push_save_kuafu_character(std::string& charID,std::string& account,int vcoin,int kuafuResult);
private:
	void PingDB();
	int DoFreshGift(const char* chrname,std::vector<OGSLib::SubGift>& giftarray);
	int DoFreshItem(const char* seedname,const char* chrname,std::vector<OGSLib::SubGift>& giftarray);
	int DoSaveCharacter(OGSLib::DomainData* character,bool quit,const char* table);
	int DoLoadCharacter(const char* account,const char* chrname,const char* seedname,OGSLib::DomainData& chr,const char* table);
	int DoSaveKuafuCharacter(std::string charID,std::string account,int vcoin,int kuafuResult);
	virtual int LoadGuilds(OGSLib::ManagerGuild& guild_map);
	virtual int SaveGuilds(const OGSLib::ManagerGuild& guild_map);
	virtual int LoadCouples(OGSLib::ManagerCouple& couple_map);
	virtual int SaveCouples(const OGSLib::ManagerCouple& couple_map);
	virtual int LoadServerVar(std::map<std::string,std::string>& var_map);
	virtual int SaveServerVar(const std::map<std::string,std::string>& var_map);
	int LoadAuto(std::vector<OGSLib::AutoRunDB>* ress);
	int game_store_sql(const char* sql,sexy_db_mysql::mem_db_res& res,long* effect_row_num,long* new_insert_id);
	void ClientListCharacter(SOCKET sock,std::string account,GS_Client::ListCharacterRes& resp);
	void ClientCreateCharacter(SOCKET sock,std::string account,std::string chrname,std::string idfa,int pid,int job,int gender,int svrid,std::string youke,GS_Client::CreateCharacterRes& res);
	void ClientDeleteCharacter(SOCKET sock,std::string account,std::string chrname,GS_Client::DeleteCharacterRes& res);
    int OnSaveMails(std::vector<OGSLib::Mail*>& mailList);
	int DoLoadMails(std::vector<OGSLib::Mail*>& mailVector,int mailValidDays);
	void game_exe_sql(const char* sql);
	void ClientAuthModifyName(std::string chrid,int &ret);
	void LoadChinaLimit(const char* account,int* online_time_count,int* offline_time_tag,int* china_limit_ok,char* cn_name,char* cn_id);
	void SaveChinaLimit(const char* account,int online_time_count,int offline_time_tag,int china_limit_ok,const char* cn_name,const char* cn_id);
	int DoLoadConsignment(std::vector<OGSLib::ConsignableItemInfo*>& info_list, std::map<std::string, int>& user_vcoin_map);
	int DoSaveConsignment(std::vector<OGSLib::ConsignableItemInfo*>& info_list, std::map<std::string, int>& user_vcoin_map);

	int DoLoadRelationCache(std::vector<OGSLib::RelationShipOper*>& oper_list);
	int DoSaveRelationCache(std::vector<OGSLib::RelationShipOper*>& oper_list);
};
