#include "StdAfx.h"
#include "ScriptLuaGuildBind.h"
#include "GuildGuild.h"
#include "ManagerMail.h"
#include "GameServerLib.h"
#define GET_GUILD		Guild* guild = *(Guild**)luaL_checkudata(L,1,"oncient.guild");

namespace OGSLib
{
	static int get_member_title(lua_State* L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		if( name )
		{
			std::string seed_name = guild->GetGuildPreSeed(name);
			if (seed_name == "")
				seed_name = guild->GetGuildSeed(name);
			int title = guild->GetMemberTitle(seed_name);
			lua_pushinteger(L,title);
			return 1;
		}
		return 0;
	}
	static int set_member_title(lua_State* L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		int title = (int)luaL_checkinteger(L,3);
		if( name )
		{
			std::string seed_name = guild->GetGuildPreSeed(name);
			if (seed_name == "")
				seed_name = guild->GetGuildSeed(name);
			title = guild->SetMemberTitle(seed_name,title);
			lua_pushinteger(L,title);
			return 1;
		}
		return 0;
	}

	static int get_member_onlinestate(lua_State* L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		if( name )
		{
			std::string guild_name = name;
			int os = guild->GetMemberOnlineState(guild_name);
			lua_pushinteger(L,os);
			return 1;
		}
		return 0;
	}

	static int add_member(lua_State* L)
	{
		GET_GUILD;
// 		const char* name = luaL_checkstring(L,2);
// 		int title = luaL_checkinteger(L,3);
// 		if( name )
// 		{
// 			std::string guild_name = name;
// 			title = guild->AddMember(guild_name.c_str(),"",title,NULL);
// 			lua_pushinteger(L,title);
// 			return 1;
// 		}
		return 0;
	}

	static int rem_member(lua_State* L)
	{
		GET_GUILD;
		return 0;
	}
	static int add_enemy_guild(lua_State* L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		if( name )
		{
			std::string guild_name = name;
			guild->AddEnemyGuild(guild_name);
		}
		return 0;
	}
	static int rem_enemy_guild(lua_State* L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		if( name )
		{
			std::string guild_name = name;
			guild->RemEnemyGuild(guild_name);
		}
		return 0;
	}
	static int check_enemy_guild(lua_State* L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		if( name )
		{
			std::string guild_name = name;
			if( guild->IsEnemyGuild(guild_name) )
			{
				lua_pushinteger(L,1);
				return 1;
			}
		}
		return 0;
	}
	static int get_leader_name(lua_State *L)
	{
		GET_GUILD;
		std::string leaderName=guild->GetGuildLeaderName();
		lua_pushstring(L,leaderName.c_str());
		return 1;
	}
	static int get_leader_seed(lua_State *L)
	{
		GET_GUILD;
		std::string leaderSeed=guild->GetGuildLeaderSeed();
		lua_pushstring(L,leaderSeed.c_str());
		return 1;
	}
	static int get_admin_info(lua_State *L)
	{
		GET_GUILD;
		const Guild::MemberMap m = guild->GetMembers();

		for ( Guild::MemberMap::const_iterator it = m.begin();
			it != m.end();
			++ it )
		{
			if( it->second.title==GUILD_TITLE_TYPE_ADMIN )
			{
				lua_pushinteger(L, 1);
				lua_pushstring(L,it->second.nick_name.c_str());
				lua_pushinteger(L,it->second.cloth);
				lua_pushinteger(L,it->second.weapon);
				lua_pushinteger(L,it->second.wing);
				lua_pushinteger(L,it->second.gender);
				return 6;
			}
		}
		return 0;
	}

	static int get_vice_admin_info(lua_State * L)
	{
		GET_GUILD;
		if (guild->GetViceAdminNum() == 0)
		{
			return 0;
		}
		int ret = 1;
		lua_pushinteger(L, guild->GetViceAdminNum());
		const Guild::MemberMap m = guild->GetMembers();

		for ( Guild::MemberMap::const_iterator it = m.begin();
			it != m.end();
			++ it )
		{
			if( it->second.title==GUILD_TITLE_TYPE_VICEADMIN )
			{
				lua_pushstring(L,it->second.nick_name.c_str());
				lua_pushinteger(L,it->second.cloth);
				lua_pushinteger(L,it->second.weapon);
				lua_pushinteger(L,it->second.wing);
				lua_pushinteger(L,it->second.gender);
				ret += 5;
			}
		}

		return ret;
	}

	static int get_adv_info(lua_State * L)
	{
		GET_GUILD;
		if (guild->GetAdvNum() == 0)
		{
			return 0;
		}
		int ret = 1;
		lua_pushinteger(L, guild->GetAdvNum());
		const Guild::MemberMap m = guild->GetMembers();

		for ( Guild::MemberMap::const_iterator it = m.begin();
			it != m.end();
			++ it )
		{
			if( it->second.title==GUILD_TITLE_TYPE_ADV )
			{
				lua_pushstring(L,it->second.nick_name.c_str());
				lua_pushinteger(L,it->second.cloth);
				lua_pushinteger(L,it->second.weapon);
				lua_pushinteger(L,it->second.wing);
				lua_pushinteger(L,it->second.gender);
				ret += 5;
			}
		}

		return ret;
	}

	static int get_member_num(lua_State *L)
	{
		GET_GUILD;
		int memberNum=guild->GetMemberNumber();
		lua_pushinteger(L,memberNum);
		return 1;
	}
	static int get_guild_level(lua_State *L)
	{
		GET_GUILD;
		lua_pushinteger(L,guild->guild_level());
		return 1;
	}
	static int set_guild_level(lua_State *L)
	{
		GET_GUILD;
		int lv = luaL_checkinteger(L,2);
		guild->guild_level(lv);
		return 0;
	}
	static int get_guild_point(lua_State *L)
	{
		GET_GUILD;
		lua_pushinteger(L,guild->guild_pt());
		return 1;
	}
	static int set_guild_point(lua_State *L)
	{
		GET_GUILD;
		int pt = luaL_checkinteger(L,2);
		if(pt > 0)
		{
			guild->guild_pt(pt);
			lua_pushinteger(L,guild->guild_pt());
			return 1;
		}
		return 0;
	}
	static int add_guild_exp(lua_State *L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		int pt = luaL_checkinteger(L,3);
		guild->add_guildpt(name,pt);
		return 0;
	}
	static int set_var(lua_State *L)
	{
		GET_GUILD;
		const char* key = luaL_checkstring(L,2);
		const char* value = luaL_checkstring(L,3);
		std::string k = key;
		std::string v = value;
		guild->Var(k,v);

		return 0;
	}
	static int get_var(lua_State *L)
	{
		GET_GUILD;
		const char* key = luaL_checkstring(L,2);
		std::string k = key;
		lua_pushstring(L,guild->Var(k).c_str());
		return 1;
	}
	static int add_item(lua_State *L)
	{
		GET_GUILD;
		int typeID =  luaL_checkinteger(L,2);
		int level = luaL_checkinteger(L,3);
		int zlevel = luaL_checkinteger(L,4);
		lua_pushnumber(L,guild->AddItem(typeID,level,zlevel));
		return 1;
	}
	static int get_item(lua_State *L)
	{
		GET_GUILD;
		int pos =  luaL_checkinteger(L,2);
		Guild::ItemMap::iterator item = guild->GetItems().find(pos);
		if(item != guild->GetItems().end())
		{
			lua_pushinteger(L,item->first);
			lua_pushinteger(L,item->second.mTypeID);
			lua_pushinteger(L,item->second.mLevel);
			lua_pushinteger(L,item->second.mZlevel);
			return 4;
		}
		return 0;
	}
	static int rem_item(lua_State *L)
	{
		GET_GUILD;
		int pos =  luaL_checkinteger(L,2);
		guild->RemItem(pos);
		return 0;
	}

	static int get_red_packet_info(lua_State *L)
	{
		GET_GUILD;
		if( guild->GetRedPacketSender())
		{
			lua_pushstring(L,guild->GetRedPacketSender());
			lua_pushinteger(L,guild->GetRedPacketRemainingValue());
			lua_pushinteger(L,guild->GetRedPacketRemainingSize());
			lua_pushinteger(L,guild->GetRedPacketRemainingTime());
			return 4;
		}
		return 0;
	}

	static int set_red_packet_info(lua_State * L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		int value = luaL_checkinteger(L,3);
		int count = luaL_checkinteger(L,4);
		guild->SetRedPacket(name, value, count);
		return 0;
	}
	
	static int take_red_packet(lua_State * L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		int nTake = guild->TakeRedPacket(name);
		lua_pushinteger(L,nTake);
		return 1;
	}

	static int check_red_packet(lua_State * L)
	{
		GET_GUILD;
		const char* name = luaL_checkstring(L,2);
		int ret = guild->CheckRedPacket(name);
		lua_pushinteger(L,ret);
		return 1;
	}

	static int add_item_op_record(lua_State * L)
	{
		GET_GUILD;
		const char * name = luaL_checkstring(L,2);
		int type_id = luaL_checkint(L,3);
		int op_code = luaL_checkint(L,4);
		guild->AddItemOpRecord(name, type_id, op_code);
		return 0;
	}

	static int dissmiss_guild(lua_State * L)
	{
		GET_GUILD;
		guild->DismissGuild();
		return 0;
	}

	static int get_viceadmin_count(lua_State * L)
	{
		GET_GUILD;
		lua_pushinteger(L,guild->GetViceAdminNum());
		return 1;
	}

	static int get_adv_count(lua_State * L)
	{
		GET_GUILD;
		lua_pushinteger(L,guild->GetAdvNum());
		return 1;
	}

	static int start_war(lua_State * L)
	{
		GET_GUILD;
		const char * name = luaL_checkstring(L,2);
		lua_pushinteger(L,guild->StartGuildWar(name));
		return 1;
	}

	static int start_all_war(lua_State * L)
	{
		GET_GUILD;
		guild->StartAllGuildWar();
		return 0;
	}

	static int notify_member_change(lua_State * L)
	{
		GET_GUILD;
		int min_title = 0;
		if (lua_isnumber(L,2))
		{
			min_title = luaL_checkinteger(L,2);
		}
		
		guild->NotifyGuildMemberChange(min_title);
		return 0;
	}

	static int agree_all(lua_State * L)
	{
		GET_GUILD;
		lua_pushinteger(L, guild->AgreeAll());
		return 1;
	}

	static int refuse_all(lua_State * L)
	{
		GET_GUILD;
		lua_pushinteger(L, guild->RefuseAll());
		return 1;
	}

	static int alert(lua_State * L)
	{
		GET_GUILD;
		int lv = luaL_checkinteger(L,2);
		int flags = luaL_checkinteger(L,3);
		const char* msg = luaL_checkstring(L,4);
		guild->NotifyAlert(lv,flags,msg);
		return 0;
	}

	static int set_max_member_count(lua_State * L)
	{
		GET_GUILD;
		int count = luaL_checkinteger(L,2);
		guild->SetMaxMemberCount(count);
		return 0;
	}

	static int post_mail_members(lua_State * L)
	{
		GET_GUILD;

		std::string title = luaL_checkstring(L,2);
		std::string content = luaL_checkstring(L,3);
		std::map<int,int>items;
		for(int i=2;i<=20;i++)
		{
			if(lua_isnumber(L,i*2)&&lua_isnumber(L,i*2+1))
			{
				int itemID = luaL_checkinteger(L,i*2);
				int num = luaL_checkinteger(L,i*2+1);
				items[itemID] += num;
			}
			else
			{
				break;
			}
		}
		
		std::vector<std::string> seedNames;
		Guild::MemberMap& members = guild->GetMembers();
		for(Guild::MemberMap::iterator iter = members.begin();iter != members.end(); iter++)
		{
			seedNames.push_back(iter->second.seed_name);
		}

		guild->mGameServerLib->GetMailManager()->PostMailToMultiplePlayers(title,content,seedNames,items);
		return 0;
	}

	static int fresh_member_info(lua_State * L)
	{
		GET_GUILD;
		guild->FreshMemberInfo();
		return 0;
	}

	static int is_guild_depot_full(lua_State * L)
	{
		GET_GUILD;
		int pos = guild->find_empty_item_position();
		lua_pushboolean(L, pos < 0);
		return 1;
	}

	static const luaL_reg lib_funcs[]={
		{"get_member_title",get_member_title},
		{"set_member_title",set_member_title},
		{"get_member_onlinestate",get_member_onlinestate},
		{"add_member",add_member},
		{"rem_member",rem_member},
		{"get_leader_name",get_leader_name},
		{"get_leader_seed",get_leader_seed},
		{"get_member_num",get_member_num},
		{"get_guild_level",get_guild_level},
		{"set_guild_level",set_guild_level},
		{"get_guild_point",get_guild_point},
		{"set_guild_point",set_guild_point},
		{"add_guild_exp",add_guild_exp},
		{"get_var",get_var},
		{"set_var",set_var},
		{"add_item",add_item},
		{"get_item",get_item},
		{"rem_item",rem_item},
		{"get_red_packet_info",get_red_packet_info},
		{"set_red_packet_info",set_red_packet_info},
		{"take_red_packet",take_red_packet},
		{"check_red_packet",check_red_packet},
		{"add_item_op_record",add_item_op_record},
		{"dissmiss_guild", dissmiss_guild},
		{"get_viceadmin_count", get_viceadmin_count},
		{"get_adv_count", get_adv_count},
		{"get_admin_info", get_admin_info},
		{"get_vice_admin_info", get_vice_admin_info},
		{"get_adv_info", get_adv_info},
		{"start_war", start_war},
		{"start_all_war", start_all_war},
		{"notify_member_change", notify_member_change},
		{"alert", alert},
		{"post_mail_members", post_mail_members},
		{"agree_all", agree_all},
		{"refuse_all", refuse_all},
		{"set_max_member_count", set_max_member_count},
		{"fresh_member_info", fresh_member_info},
		{"is_guild_depot_full", is_guild_depot_full},
		{0,0}
	};


	ScriptLuaGuildBind::ScriptLuaGuildBind(void)
	{
	}

	ScriptLuaGuildBind::~ScriptLuaGuildBind(void)
	{
	}

	void ScriptLuaGuildBind::init(lua_State* L)
	{
		luaL_newmetatable(L, "oncient.guild");
		lua_pushvalue(L,-1);
		lua_setfield(L,-2,"__index");
		luaL_register(L,0,lib_funcs);
		lua_pop(L,1);
	}

	void ScriptLuaGuildBind::bind(lua_State* L,Guild* guild)
	{
		Guild** a = (Guild**)lua_newuserdata(L, sizeof(guild));
		*a = guild;

		luaL_getmetatable(L, "oncient.guild");
		lua_setmetatable(L, -2);
	}
}