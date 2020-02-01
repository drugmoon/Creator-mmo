#include "StdAfx.h"
#include "ScriptLuaMonsterBind.h"
#include "ScriptLua.h"
#include "ScriptLuaMapBind.h"
#include "PlayMonster.h"
#define GET_MONSTER		PlayMonster* monster = *(PlayMonster**)luaL_checkudata(L,1,"oncient.monster");

namespace OGSLib
{
	ScriptLuaMonsterBind::ScriptLuaMonsterBind(void)
	{
	}

	ScriptLuaMonsterBind::~ScriptLuaMonsterBind(void)
	{
	}
	static int get_name(lua_State* L)
	{
		GET_MONSTER;
		lua_pushstring(L,monster->Name());
		return 1;
	}
	static int get_first_name(lua_State* L)
	{
		GET_MONSTER;
		lua_pushstring(L,monster->FirstName());
		return 1;
	}
	static int get_map(lua_State* L)
	{
		GET_MONSTER;
		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);	
		PlayMap* map = monster->GetMap();
		if( map )
		{
			This->get_mapbind()->bind(L,map);
			return 1;
		}
		return 0;
	}
	static int get_level(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->level());
		return 1;
	}
	static int get_pos(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->X());
		lua_pushinteger(L,monster->Y());
		return 2;
	}
	static int get_caiji_duration(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->CollectTime());
		return 1;
	}
	static int get_exp_award(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->GetDeathExp());
		return 1;
	}
	static int get_power_exp(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->GetPowerExp());
		return 1;
	}
	static int isboss(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L, monster->IsBoss());
		return 1;
	}
	static int iselite(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L, monster->IsElite());
		return 1;
	}
	static int get_def_id(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->DefId());
		return 1;
	}
	static int get_id(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->ID());
		return 1;
	}
	static int lock_target(lua_State* L)
	{
		GET_MONSTER;
		int targetID = luaL_checkinteger(L,2);
		monster->LockTarget(targetID);
		//lua_pushinteger(L,monster->DefId());
		return 0;
	}
	static int get_mon_group(lua_State* L)
	{
		GET_MONSTER;
		lua_pushinteger(L,monster->GetMonGroup());
		return 1;
	}
	/*static int set_destination(lua_State* L)
	{
	GET_MONSTER;
	int x = luaL_checkinteger(L,2);
	int y = luaL_checkinteger(L,3);
	monster->SetDestination(x, y);
	return 0;
	}*/
	static const luaL_reg lib_funcs[]={
		{"get_name",get_name},
		{"get_first_name",get_first_name},
		{"get_map",get_map},
		{"get_level",get_level},
		{"get_pos",get_pos},
		{"get_caiji_duration",get_caiji_duration},
		{"get_exp_award",get_exp_award},
		{"get_power_exp",get_power_exp},
		{"isboss",isboss},
		{"iselite",iselite},
		{"get_def_id",get_def_id},
		{"get_id", get_id},
		{"lock_target",lock_target},
		{"get_mon_group",get_mon_group},
		{0,0}
	};
	void ScriptLuaMonsterBind::init(lua_State* L)
	{
		luaL_newmetatable(L, "oncient.monster");
		lua_pushvalue(L,-1);
		lua_setfield(L,-2,"__index");
		luaL_register(L,0,lib_funcs);
		lua_pop(L,1);
	}
	void ScriptLuaMonsterBind::bind(lua_State* L,PlayMonster* monster)
	{
		PlayMonster** a = (PlayMonster**)lua_newuserdata(L, sizeof(monster));
		*a = monster;

		luaL_getmetatable(L, "oncient.monster");
		lua_setmetatable(L, -2);
	}
}