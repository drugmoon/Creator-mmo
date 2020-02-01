#pragma once
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
namespace OGSLib
{
	class ScriptLuaNpcBind;
	class ScriptLuaPlayerBind;
	class ScriptLuaMonsterBind;
	class ScriptLuaGuildBind;
	class ScriptLuaMapBind;
	class ScriptLuaItemBind;
	class ScriptLuaSlaveBind;
	class PlayNpc;
	class DomainPlay;
	class Guild;
	class PlayMonster;
	class PlayMap;
	class PlaySlave;
	class SubItem;
	class ScriptLua;
	class GameServerLib;
	class ScriptLua
	{
		lua_State* m_l;
		ScriptLuaNpcBind* m_npcbind;
		ScriptLuaPlayerBind* m_playerbind;
		ScriptLuaMonsterBind* m_monsterbind;
		ScriptLuaGuildBind* m_guildbind;
		ScriptLuaMapBind* m_mapbind;
		ScriptLuaItemBind* m_itembind;
		ScriptLuaSlaveBind * m_slavebind;

		int mNpcPlayerCount;
		int mRetNpcPlayerCount;
		int mItemPlayerCount;
		int mAs2LuaPlayerCount;
		int mAs2LuaPlayer2Count;
		int mPlayerIntCount;
		int mMonsterPlayerCount;
		int mPlayerCongziCount;
		int mPlayerStringCount;
		int mOnlyMapCount;
		int mOnlyPlayerCount;
		int mOnlyPlayerParamCount;
		int mMapPlayerCount;

		char m_script_path[1024];
		GameServerLib* m_game_server_lib;
	public:
		ScriptLua(GameServerLib* gsl);
		virtual ~ScriptLua(void);
		bool init(const char* path);
		void set_state(lua_State* l);
		lua_State* load_script(const char* path);
		void dest();
		void reload();
		size_t mem_size();
		//void doString(std::string src,std::string name);
		GameServerLib* getGSL(){return m_game_server_lib;}
		ScriptLuaNpcBind* get_npcbind(){return m_npcbind;}
		ScriptLuaPlayerBind* get_playerbing(){return m_playerbind;}
		ScriptLuaMonsterBind* get_monsterbind(){return m_monsterbind;}
		ScriptLuaGuildBind* get_guildbind(){return m_guildbind;}
		ScriptLuaMapBind* get_mapbind(){return m_mapbind;}
		ScriptLuaItemBind* get_itembind(){return m_itembind;}
		ScriptLuaSlaveBind * get_slavebind() {return m_slavebind;}
	public:
		void DoCall(const char* function);
		void NpcPlayer(PlayNpc* npc,DomainPlay* player,const char* function);
		int RetNpcPlayer(PlayNpc* npc,DomainPlay* player,const char* function);
		int ItemPlayer(DomainPlay* player,const char* item_name,int item_id,const char* function,int num = 1,int pos = -999,const char* seed_id = NULL);
		//int ExchangeItemPlayer(DomainPlay* player,int exchange_type,int pos,const char* item_name,int item_id,const char* function);
		//int MergeEquipPlayer(DomainPlay* player,int target_id,int param1,int param2,int param3,int param4,int param5,int param6,const char* function);
		int PlayerInt(DomainPlay* player,int param1,int param2,int param3,int param4,const char* function);
		int PlayerInt(DomainPlay* player,std::vector<int>& params,const char* function);
		//int UpEquipItemPlayer(DomainPlay* player,int pos,const char* item_name,int itemid,int money_flag,int hunzhi_flag,int vcoin_flag,int ex_flag,const char* function);
		std::string PlayerGetName(DomainPlay* player,const char* npc_name,const char* function);
		void MonsterPlayer(PlayMonster* monster,DomainPlay* player,const char* function);
		void PlayerMonster(DomainPlay* player,PlayMonster* monster,const char* function);
		void MapPlayer(PlayMap* map,DomainPlay* player,const char* function);
		void PlayerCongzi(DomainPlay* player,int congzi,int congzihou,int congziaccu,const char* function);
		void OnlyPlayer(DomainPlay* player,const char* function);
		void OnlyPlayer(DomainPlay* player,int param1,int param2,const char* function);
		//void PlayerString(DomainPlay* player,const char* function,const char* param);
		int PlayerString(DomainPlay* player, const char* function, const char* param);
		int GetServerStartDay(const char* function);
		void PlayerLuaFunction(DomainPlay* player,const char* function,const char* param,const char* data);
		void PlayerPlayer(DomainPlay* player,DomainPlay* des,const char* function);
		void OnlyMonster(PlayMonster* monster,const char* function);
		void OnlyMap(PlayMap* map,const char* function);
		void OnlyGuild(Guild* guild,const char* function);
		void OnlySlave(PlaySlave * slave, const char* function);
		void SystemScript(const char* function);
	};
}