#include "StdAfx.h"
#include "ScriptLuaMapBind.h"
#include "PlayMap.h"
#include "ScriptLua.h"
#include "ScriptLuaNpcBind.h"
#include "ScriptLuaMonsterBind.h"
#include "ScriptLuaPlayerBind.h"
#include "ManagerMap.h"
#include "PlayMapItem.h"
#include "GameServerLib.h"
#include "ManagerItemDef.h"
#include "ManagerFly.h"
#include "ManagerTrapDef.h"
#include "PlayMapTrap.h"
#include "PlayMonster.h"

#define GET_MAP		PlayMap* map = *(PlayMap**)luaL_checkudata(L,1,"oncient.map");

namespace OGSLib
{
	static int get_name(lua_State* L)
	{
		GET_MAP;
		lua_pushstring(L,map->GetName().c_str());
		return 1;
	}
	static int set_name(lua_State* L)
	{
		GET_MAP;
		std::string name = luaL_checkstring(L, 2);
		map->SetName(name);
		return 0;
	}
	static int get_id(lua_State* L)
	{
		GET_MAP;
		lua_pushstring(L,map->GetID().c_str());
		return 1;
	}
	static int get_var(lua_State* L)
	{
		GET_MAP;
		const char* key = luaL_checkstring(L,2);
		if( key )
		{
			std::string var_key = key;
			const char* value = map->GetVar(key);
			if( value && strlen(value)>0 )
			{
				lua_pushstring(L,value);
				return 1;
			}
		}
		return 0;
	}
	static int set_var(lua_State* L)
	{
		GET_MAP;
		const char* key = luaL_checkstring(L,2);
		if( key )
		{
			std::string var_key = key;
			if( LUA_TNIL == lua_type(L,3) )
			{
				map->SetVar(var_key,"");
				return 0;
			}
			const char* val = luaL_checkstring(L,3);
			if( val )
			{
				std::string var_val = val;
				map->SetVar(var_key,var_val);
				return 0;
			}
		}
		return 0;
	}
	static int set_timer(lua_State* L)
	{
		GET_MAP;
		int i = luaL_checkint(L,2);
		int esp = luaL_checkint(L,3);
		lua_pushinteger(L,map->SetMapTimer(i,esp));
		return 1;
	}
	static int get_timer(lua_State* L)
	{
		GET_MAP;
		int i = luaL_checkint(L, 2);
		lua_pushnumber(L, map->GetMapTimer(i));
		return 1;
	}
	static int get_npc(lua_State* L)
	{
		GET_MAP;
		if( lua_gettop(L)<2 || lua_isnil(L,2) ) return 0;
		const char* name = luaL_checkstring(L,2);

		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);

		std::string npc_name = name;
		OGSLib::PlayNpc* npc= map->FindNPC(npc_name);
		if( npc )
		{
			This->get_npcbind()->bind(L,npc);
			return 1;
		}
		return 0;
	}
	static int get_monster(lua_State* L)
	{
		GET_MAP;
		if( lua_gettop(L)<2 || lua_isnil(L,2) ) return 0;
		int id = luaL_checkinteger(L,2);

		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);

		OGSLib::PlayMonster* mon= map->FindMonster(id);
		if( mon )
		{
			This->get_monsterbind()->bind(L,mon);
			return 1;
		}
		return 0;
	}
	static int get_minlv(lua_State* L)
	{
		GET_MAP;
		lua_pushnumber(L,map->EnterMinLv());
		return 1;
	}
	static int get_minlvR(lua_State* L)
	{
		GET_MAP;
		lua_pushnumber(L,map->EnterMinLvR());
		return 1;
	}
	static int get_flag_randgo(lua_State* L)
	{
		GET_MAP;
		if( map->CanRandGo() > 0 )
		{
			lua_pushinteger(L,1);
			return 1;
		}
		return 0;
	}
	static int get_flag_pkprohibit(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->PKProhibit());
		return 1;
	}
	static int set_flag_pkprohibit(lua_State* L)
	{
		GET_MAP;
		int v = luaL_checkint(L,2);
		map->PKProhibit(v);
		return 0;
	}
	static int get_flag_pkallow(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->PKAllow());
		return 1;
	}
	static int set_flag_pkallow(lua_State* L)
	{
		GET_MAP;
		int v = luaL_checkint(L,2);
		map->PKAllow(v);
		return 0;
	}
	static int get_flag_autoalive(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->AutoAlive());
		return 1;
	}
	static int set_flag_autoalive(lua_State* L)
	{
		GET_MAP;
		int v = luaL_checkint(L,2);
		map->AutoAlive(v);
		return 0;
	}
	static int get_flag_mount(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->NoMount());
		return 1;
	}
	static int set_flag_mount(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->NoMount());
		return 1;
	}
	static int find_hold_kuafu_guild(lua_State* L)
	{
		GET_MAP;
		std::string a,g;
		map->FindHoldKuafuGuild(a,g);
		lua_pushstring(L,a.c_str());
		lua_pushstring(L,g.c_str());
		return 2;
	}
	static int find_hold_guild(lua_State* L)
	{
		GET_MAP;
		std::string g;
		map->FindHoldGuild(g);
		lua_pushstring(L,g.c_str());
		return 1;
	}
	static int num_monster(lua_State* L)
	{
		GET_MAP;
		if (lua_isnumber(L, 2))
		{
			int na = luaL_checkinteger(L, 2);
			lua_pushinteger(L, map->NumberMonster(na));
			return 1;
		}
		const char* na=0;
		if(lua_isstring(L, 2))
		{
			na = luaL_checkstring(L,2);
			
		}
		lua_pushinteger(L,map->NumberMonster(na));
		return 1;
		
	}
	static int num_neutral(lua_State* L)
	{
		GET_MAP;
		const char* na=0;
		if( lua_isstring(L,2) )
		{
			na = luaL_checkstring(L,2);
		}
		lua_pushinteger(L,map->NumberNeutral(na));
		return 1;
	}
	static int num_player(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->NumberPlayer());
		return 1;
	}
	static int num_item(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->NumberItem());
		return 1;
	}
	static int mon_gen(lua_State* L)
	{
		GET_MAP;
		int x = luaL_checkinteger(L,2);
		int y = luaL_checkinteger(L,3);
		int id = 0;
		if (lua_isnumber(L, 4))
		{
			id = luaL_checkinteger(L,4);
		}
		//name = luaL_checkstring(L,4);
		int num = luaL_checkinteger(L,5);
		int range = luaL_checkinteger(L,6);
		int time = luaL_checkinteger(L,7);
		int noalive = luaL_checkinteger(L,8);
		const char* flags = "";
		if( lua_isstring(L,9) )
		{
			flags = luaL_checkstring(L,9);
		}
		int tip = 0;
		if (lua_isnumber(L, 10))
		{
			tip = luaL_checkinteger(L, 10);
		}
		int limitlv = 0;
		if (lua_isnumber(L, 11))
		{
			limitlv = luaL_checkinteger(L, 11);
		}
		int cloth = 0;
		if (lua_isnumber(L, 12))
		{
			cloth = luaL_checkinteger(L, 12);
		}
		int weapon = 0;
		if (lua_isnumber(L, 13))
		{
			weapon = luaL_checkinteger(L, 13);
		}
		int wing = 0;
		if (lua_isnumber(L, 14))
		{
			wing = luaL_checkinteger(L, 14);
		}

		if ( id )
		{
			map->GetManager()->GenMon(map,id,x,y,num,range,time,noalive,flags,tip,limitlv, cloth, weapon, wing);
		}
		return 0;
	}

	static int trap_gen(lua_State* L)
	{
		GET_MAP;
		int x = luaL_checkinteger(L,2);
		int y = luaL_checkinteger(L,3);
		TrapDef td;
		td.id = luaL_checkinteger(L,4) + eTrapIdUserEefBegin;
		td.cloth = luaL_checkinteger(L,5);
		td.damage = luaL_checkinteger(L,6);
		td.damage_gap = luaL_checkinteger(L,7); // ºÁÃë
		td.life_time = luaL_checkinteger(L,8) / 1000;
		td.effect_id = 0;
		td.skill_type = 0;
		td.caller_leave_remove = 0;

		const char* name = "";
		if( lua_isstring(L,9) )
		{
			td.name = luaL_checkstring(L,9);
		}
		map->GetManager()->GenTrap(map,x, y, &td, NULL);
		return 0;
	}

	static int remove_trap(lua_State* L)
	{
		GET_MAP;
		int x = luaL_checkinteger(L,2);
		int y = luaL_checkinteger(L,3);
		int id = luaL_checkinteger(L,4) + eTrapIdUserEefBegin;

		map->GetManager()->RemoveTrap(map, x, y, id);
		return 0;
	}

	static int clear_mon(lua_State* L)
	{
		GET_MAP;
		map->ClearMon();
		return 0;
	}

	static int conn_gen(lua_State* L)
	{
		GET_MAP;
		
		return 0;
	}
	static int clear_mon_by_id(lua_State* L)
	{
		GET_MAP;
		int id = luaL_checkinteger(L,2);
		map->ClearMonById(id);
		return 0;
	}
	static int clear_mon_by_name(lua_State* L)
	{
		GET_MAP;
		const char* name = luaL_checkstring(L,2);
		map->ClearMonByName(name);
		return 0;
	}
	static int clear_neutral(lua_State* L)
	{
		GET_MAP;
		map->ClearNeutral();
		return 0;
	}
	static int clear_item(lua_State* L)
	{
		GET_MAP;
		map->ClearItem();
		return 0;
	}
	static int clear_mininpc(lua_State* L)
	{
		GET_MAP;
		map->ClearMiniNpc();
		return 0;
	}
	static int num_team_member(lua_State* L)
	{
		GET_MAP;
		int team_id = luaL_checkinteger(L,2);
		lua_pushinteger(L,map->NumberTeamMember(team_id));
		return 1;
	}
	static int exe_team_member(lua_State* L)
	{
		GET_MAP;
		int team_id = luaL_checkinteger(L,2);
		const char* exe = luaL_checkstring(L,3);
		map->ExeTeamMember(team_id,exe);
		return 0;
	}
	static int map_exe(lua_State* L)
	{
		GET_MAP;
		const char* code = luaL_checkstring(L,2);
		map->MapExe(code);
		return 0;
	}
	static int alert(lua_State* L)
	{
		GET_MAP;
		int lv = luaL_checkinteger(L,2);
		int flags = luaL_checkinteger(L,3);
		const char* msg = luaL_checkstring(L,4);
		map->Alert(lv,flags,msg);
		return 0;
	}
	static int play_effect(lua_State* L)
	{
		GET_MAP;
		int skill_type = luaL_checkinteger(L,2);
		int res_id = luaL_checkinteger(L,3);
		map->PlayEffect(skill_type,res_id);
		return 0;
	}
	static int gen_rand_position(lua_State* L)
	{
		GET_MAP;
		int x,y;
		x=y=0;
		if( map->GenValidateRandomPosition(&x,&y) )
		{
			lua_pushinteger(L,x);
			lua_pushinteger(L,y);
			return 2;
		}
		return 0;
	}
	static int get_rand_player(lua_State* L)
	{
		GET_MAP;
		if( lua_gettop(L)<2 || lua_isnil(L,2) ) return 0;
		int rand = luaL_checkinteger(L,2);

		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);

		OGSLib::DomainPlay* player= map->RandPlayer(rand);
		if( player )
		{
			This->get_playerbing()->bind(L,player);
			return 1;
		}
		return 0;
	}
	static int drop_item(lua_State* L)
	{
		GET_MAP;
		const char* item_name = 0;
		int item=0;
		ItemDef* id=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
			id = map->GetManager()->GetGSLib()->GetItemDefineManager()->GetItemDefine(item);
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			id = map->GetManager()->GetGSLib()->GetItemDefineManager()->GetItemDefine(item_name);;
			if( id )
			{
				item = id->mTypeID;
			}
		}
		int flags = luaL_checkinteger(L,3);
		int x = luaL_checkinteger(L,4);
		int y = luaL_checkinteger(L,5);
		int ownerid = luaL_checkinteger(L,6);
		int num = luaL_checkinteger(L, 7);
		int life_time = luaL_checkinteger(L, 8);
		int resetowner_time = luaL_checkinteger(L, 9);
		int pick_limit_lv = 0;
		if (lua_isnumber(L, 10))
		{
			pick_limit_lv = luaL_checkinteger(L, 10);
		}
		int created_time = static_cast<int>(time(0));
		SubItem ii;
		if (id)
		{
			ii.BuildItemInstance(id, flags, false, map->GetManager()->GetGSLib()->mItemAddType);
			PlayMapItem* mi = new PlayMapItem(x, y, ii);
			mi->ClothID(id->mIconID);
			mi->OwnerID(ownerid);
			mi->Number(num);
			mi->life_time(life_time);
			mi->ResetOwnerTime(resetowner_time);
			mi->SetPickLimitLv(pick_limit_lv);
			map->AddObject(mi);
		}
		
		return 0;
	}
	static int get_exp_mul(lua_State* L)
	{
		GET_MAP;
		lua_pushinteger(L,map->ExpMul());
		return 1;
	}
	static int set_exp_mul(lua_State* L)
	{
		GET_MAP;
		int mul = luaL_checkinteger(L,2);
		map->SetExpMul(mul);
		return 0;
	}
	static int set_owner(lua_State* L)
	{
		GET_MAP;
		std::string name = luaL_checkstring(L, 2);
		map->SetOwnerNmae(name);
		return 0;
	}
	static int get_owner(lua_State* L)
	{
		GET_MAP;
		lua_pushstring(L, map->GetOwnerName().c_str());
		return 1;
	}
	static int get_def_id(lua_State* L)
	{
		GET_MAP;
		lua_pushstring(L,map->GetID().c_str());
		return 1;
	}
	static int add_dy_conn(lua_State* L)
	{
		GET_MAP;
		int nodeid = luaL_checkinteger(L,2);
		int cloth = luaL_checkinteger(L,3);
		int x = luaL_checkinteger(L,4);
		int y = luaL_checkinteger(L,5);
		std::string map_id  = luaL_checkstring(L,6);
		int tx = luaL_checkinteger(L,7);
		int ty = luaL_checkinteger(L,8);
		int range = luaL_checkinteger(L,9);
		lua_pushinteger(L, map->AddDyMapConn(nodeid, cloth, x, y, map_id, tx, ty, range));
		return 1;
	}

	static int remove_dy_conn(lua_State* L)
	{
		GET_MAP;
		int id = luaL_checkinteger(L,2);
		lua_pushinteger(L, map->RemoveDyMapConn(id));
		return 1;
	}

	static int enable_conn(lua_State * L)
	{
		GET_MAP;
		int id = luaL_checkinteger(L,2);
		DirectFlyNode * p = map->GetManager()->GetGSLib()->GetFlyManager()->FindNode(id);
		if (p == NULL)
			return 0;
		int enable = luaL_checkinteger(L,3);
		bool b = enable == 1? true:false;
		map->EnableMapConn(p, b);

		return 0;
	}

	static int disable_huwei(lua_State * L)
	{
		GET_MAP;
		lua_pushinteger(L, map->NoGod());
		return 1;
	}

	static int set_auto_enter(lua_State * L)
	{
		GET_MAP;
		int auto_enter = luaL_checkinteger(L,2);
		map->AutoEnter(auto_enter);
		return 0;
	}

	static int is_no_ctg_boss(lua_State * L)
	{
		GET_MAP;
		lua_pushinteger(L, map->NoCtgBoss());
		return 1;
	}

	static int mon_public_target(lua_State* L)
	{
		GET_MAP;
		int targetId = luaL_checkinteger(L, 2);
		PlayActiveObject* target = map->FindPlayerOrSlave(targetId);
		if (target)
		{
			map->PublicTarget(target, true);
		}
		return 0;
	}

	static const luaL_reg lib_funcs[]={
		{"get_exp_mul",get_exp_mul},
		{"set_exp_mul",set_exp_mul},
		{"get_name",get_name},
		{"set_name",set_name},
		{"get_id",get_id},
		{"get_var",get_var},
		{"set_var",set_var},
		{"set_timer",set_timer},
		{"get_timer",get_timer },
		{"get_minlv",get_minlv},
		{"get_minlvR",get_minlvR},
		{"get_npc",get_npc},
		{"get_monster",get_monster},
		{"get_flag_randgo",get_flag_randgo},
		{"get_flag_pkprohibit",get_flag_pkprohibit},
		{"set_flag_pkprohibit",set_flag_pkprohibit},
		{"get_flag_pkallow",get_flag_pkallow},
		{"set_flag_pkallow",set_flag_pkallow},
		{"get_flag_autoalive",get_flag_autoalive},
		{"set_flag_autoalive",set_flag_autoalive},
		{"get_flag_mount",get_flag_mount},
		{"set_flag_mount",set_flag_mount},
		{"find_hold_guild",find_hold_guild},
		{"mon_gen",mon_gen},
		{"num_neutral",num_neutral},
		{"num_monster",num_monster},
		{"num_player",num_player},
		{"num_item",num_item},
		{"num_team_member",num_team_member},
		{"exe_team_member",exe_team_member},
		{"clear_mon",clear_mon},
		{"clear_mon_by_name",clear_mon_by_name},
		{"clear_mon_by_id",clear_mon_by_id},
		{"clear_neutral",clear_neutral},
		{"clear_item",clear_item},
		{"clear_mininpc",clear_mininpc},
		{"map_exe",map_exe},
		{"alert",alert},
		{"play_effect",play_effect},
		{"gen_rand_position",gen_rand_position},
		{"get_rand_player",get_rand_player},
		{"drop_item",drop_item},
		{"find_hold_kuafu_guild",find_hold_kuafu_guild},
		{"set_owner", set_owner},
		{"get_owner", get_owner },
		{"get_def_id", get_def_id},
		{"add_dy_conn", add_dy_conn },
		{"remove_dy_conn", remove_dy_conn},
		{"enable_conn", enable_conn},
		{"trap_gen", trap_gen},
		{"remove_trap", remove_trap},
		{"disable_huwei",disable_huwei},
		{"set_auto_enter",set_auto_enter},
		{"is_no_ctg_boss", is_no_ctg_boss},
		{"mon_public_target", mon_public_target},
		{0,0}
	};

	ScriptLuaMapBind::ScriptLuaMapBind(void)
	{
	}
	ScriptLuaMapBind::~ScriptLuaMapBind(void)
	{
	}
	void ScriptLuaMapBind::init(lua_State* L)
	{
		luaL_newmetatable(L, "oncient.map");
		lua_pushvalue(L,-1);
		lua_setfield(L,-2,"__index");
		luaL_register(L,0,lib_funcs);
		lua_pop(L,1);
	}
	void ScriptLuaMapBind::bind(lua_State* L,PlayMap* map)
	{
		PlayMap** a = (PlayMap**)lua_newuserdata(L, sizeof(map));
		*a = map;

		luaL_getmetatable(L, "oncient.map");
		lua_setmetatable(L, -2);
	}
}