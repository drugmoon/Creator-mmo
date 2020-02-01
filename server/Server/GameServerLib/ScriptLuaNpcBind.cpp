#include "StdAfx.h"
#include "ScriptLuaNpcBind.h"
#include "ScriptLua.h"
#include "ScriptLuaMapBind.H"
#include "PlayNpc.h"

#define GET_NPC		PlayNpc* npc = *(PlayNpc**)luaL_checkudata(L,1,"oncient.npc");

namespace OGSLib
{
	static int get_name(lua_State* L)
	{
		GET_NPC;
		lua_pushstring(L,npc->Name());
		return 1;
	}
	static int get_fly_id(lua_State* L)
	{
		GET_NPC;
		lua_pushinteger(L,npc->flyId());
		return 1;
	}
	static int echo(lua_State* L)
	{
		GET_NPC;
		for(int i = 2;i<100;i++)
		{
			const char* msg = lua_tostring(L,i);
			if( msg )
			{
				npc->Echo(msg);
			}
			else
			{
				break;
			}
		}
		return 0;
	}
	static int show_accept(lua_State* L)
	{
		GET_NPC;
		int msg = luaL_checkint(L,2);
		npc->ShowAccept(msg);
		return 0;
	}
	static int show_continue(lua_State* L)
	{
		GET_NPC;
		int msg = luaL_checkint(L,2);
		npc->ShowContinue(msg);
		return 0;
	}
	static int show_done(lua_State* L)
	{
		GET_NPC;
		int msg = luaL_checkint(L,2);
		npc->ShowDone(msg);
		return 0;
	}
	static int get_map(lua_State* L)
	{
		GET_NPC;
		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);	
		PlayMap* map = npc->GetMap();
		if( map )
		{
			This->get_mapbind()->bind(L,map);
			return 1;
		}
		return 0;
	}
	static int get_id(lua_State* L)
	{
		GET_NPC;
		lua_pushinteger(L,npc->ID());
		return 1;
	}
	static int refresh_show_flags(lua_State* L)
	{
		GET_NPC;
		DomainPlay* player = *(DomainPlay**)luaL_checkudata(L,2,"oncient.player");
		npc->RefreshShowFlags(player);
		return 0;
	}
	static int get_def_id(lua_State* L)
	{
		GET_NPC;
		lua_pushinteger(L,npc->DefId());
		return 1;
	}
	static int get_dialog(lua_State* L)
	{
		GET_NPC;
		lua_pushstring(L,npc->GetDialog());
		return 1;
	}
	static const luaL_reg lib_funcs[]={
		{"get_name",get_name},
		{"echo",echo},
		{"get_id",get_id},
		{"get_map",get_map},
		{"get_fly_id",get_fly_id},
		{"get_def_id",get_def_id},
		{"get_dialog",get_dialog},
		{0,0}
	};
	ScriptLuaNpcBind::ScriptLuaNpcBind(void)
	{
	}

	ScriptLuaNpcBind::~ScriptLuaNpcBind(void)
	{
	}
	void ScriptLuaNpcBind::init(lua_State* L)
	{
		luaL_newmetatable(L, "oncient.npc");
		lua_pushvalue(L,-1);
		lua_setfield(L,-2,"__index");
		luaL_register(L,0,lib_funcs);
		lua_pop(L,1);
	}
	void ScriptLuaNpcBind::bind(lua_State* L,PlayNpc* npc)
	{
		PlayNpc** a = (PlayNpc**)lua_newuserdata(L, sizeof(npc));
		*a = npc;

		luaL_getmetatable(L, "oncient.npc");
		lua_setmetatable(L, -2);
	}
}