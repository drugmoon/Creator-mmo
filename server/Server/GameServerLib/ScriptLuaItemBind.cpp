#include "StdAfx.h"
#include "ScriptLuaItemBind.h"
#include "ScriptLua.h"
#include "SubItem.h"
#include "ManagerItemDef.h"
#include "GameServerLib.h"
#define GET_ITEM		SubItem* item = *(SubItem**)luaL_checkudata(L,1,"oncient.item");
namespace OGSLib
{
	static int get_addac(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mAddAC);
		return 1;
	}
	static int set_addac(lua_State* L)
	{
		GET_ITEM;
		int ac=luaL_checkinteger(L,2);
		item->mAddAC = ac;
		return 0;
	}
	static int get_addmac(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mAddMAC);
		return 1;
	}
	static int set_addmac(lua_State* L)
	{
		GET_ITEM;
		int mac=luaL_checkinteger(L,2);
		item->mAddMAC = mac;
		return 0;
	}
	static int get_adddc(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mAddDC);
		return 1;
	}
	static int set_adddc(lua_State* L)
	{
		GET_ITEM;
		int dc=luaL_checkinteger(L,2);
		item->mAddDC = dc;
		return 0;
	}
	static int get_addmc(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mAddMC);
		return 1;
	}
	static int set_addmc(lua_State* L)
	{
		GET_ITEM;
		int mc=luaL_checkinteger(L,2);
		item->mAddMC = mc;
		return 0;
	}
	static int get_addsc(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mAddSC);
		return 1;
	}
	static int set_addsc(lua_State* L)
	{
		GET_ITEM;
		int sc=luaL_checkinteger(L,2);
		item->mAddSC = sc;
		return 0;
	}
	static int get_addhp(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mAddHp);
		return 1;
	}
	static int set_addhp(lua_State* L)
	{
		GET_ITEM;
		int hp=luaL_checkinteger(L,2);
		item->mAddHp = hp;
		return 0;
	}
	static int get_addmp(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mAddMp);
		return 1;
	}
	static int set_addmp(lua_State* L)
	{
		GET_ITEM;
		int mp=luaL_checkinteger(L,2);
		item->mAddMp = mp;
		return 0;
	}
	static int get_updlevel(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mLevel);
		return 1;
	}
	static int set_updlevel(lua_State* L)
	{
		GET_ITEM;
		int lv=luaL_checkinteger(L,2);
		item->mLevel = lv;
		return 0;
	}
	static int get_upzlevel(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mZLevel);
		return 1;
	}
	static int get_luck(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mLuck);
		return 1;
	}
	static int set_luck(lua_State* L)
	{
		GET_ITEM;
		int lc=luaL_checkinteger(L,2);
		item->mLuck = lc;
		return 0;
	}
	static int get_item_flags(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mItemFlags);
		return 1;
	}
	static int get_duration(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mDuration);
		return 1;
	}
	static int set_duration(lua_State* L)
	{
		GET_ITEM;
		int du=luaL_checkinteger(L,2);
		item->mDuration = du;
		return 0;
	}
	static int get_number(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mNumber);
		return 1;
	}
	static int get_updac(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mUpdAC);
		return 1;
	}
	static int set_updac(lua_State* L)
	{
		GET_ITEM;
		int updac=luaL_checkinteger(L,2);
		item->mUpdAC = updac;
		return 0;
	}
	static int get_updmac(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mUpdMAC);
		return 1;
	}
	static int set_updmac(lua_State* L)
	{
		GET_ITEM;
		int updmac=luaL_checkinteger(L,2);
		item->mUpdMAC = updmac;
		return 0;
	}
	static int get_upddc(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mUpdDC);
		return 1;
	}
	static int set_upddc(lua_State* L)
	{
		GET_ITEM;
		int upddc=luaL_checkinteger(L,2);
		item->mUpdDC = upddc;
		return 0;
	}
	static int get_updmc(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mUpdMC);
		return 1;
	}
	static int set_updmc(lua_State* L)
	{
		GET_ITEM;
		int updmc=luaL_checkinteger(L,2);
		item->mUpdMC = updmc;
		return 0;
	}
	static int get_updsc(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mUpdSC);
		return 1;
	}
	static int set_updsc(lua_State* L)
	{
		GET_ITEM;
		int updsc=luaL_checkinteger(L,2);
		item->mUpdSC = updsc;
		return 0;
	}
	static int get_createtime(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mCreateTime);
		return 1;
	}
	static int check_bind(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_BIND?1:0);
		return 1;
	}
	static int check_use_bind(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_USE_BIND?1:0);
		return 1;
	}
	static int check_jipin(lua_State* L)
	{
		GET_ITEM;
		lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_JIPIN?1:0);
		return 1;
	}
	static int set_bind(lua_State* L)
	{
		GET_ITEM;
		int flag=luaL_checkinteger(L,2);
		if( flag == 1 )
		{
			item->mItemFlags |= ITEM_FLAG_BIND;
		}
		else
		{
			item->mItemFlags &= 0xE;
		}
		return 0;
	}
	static int set_use_bind(lua_State* L)
	{
		GET_ITEM;
		int flag=luaL_checkinteger(L,2);
		if( flag == 1 )
		{
			item->mItemFlags |= ITEM_FLAG_USE_BIND;
		}
		else
		{
			item->mItemFlags &= 0xD;
		}
		return 0;
	}
	static int set_jipin(lua_State* L)
	{
		GET_ITEM;
		int flag=luaL_checkinteger(L,2);
		if( flag == 1 )
		{
			item->mItemFlags |= ITEM_FLAG_JIPIN;
		}
		else
		{
			item->mItemFlags &= 0xB;
		}
		return 0;
	}
	static const luaL_reg lib_funcs[]={
		{"get_addac",get_addac},
		{"set_addac",set_addac},
		{"get_addmac",get_addmac},
		{"set_addmac",set_addmac},
		{"get_adddc",get_adddc},
		{"set_adddc",set_adddc},
		{"get_addmc",get_addmc},
		{"set_addmc",set_addmc},
		{"get_addsc",get_addsc},
		{"set_addsc",set_addsc},
		{"get_addhp",get_addhp},
		{"set_addhp",set_addhp},
		{"get_addmp",get_addmp},
		{"set_addmp",set_addmp},
		{"get_updlevel",get_updlevel},
		{"set_updlevel",set_updlevel},
		{"get_upzlevel",get_upzlevel},//获得注灵等级
		{"get_luck",get_luck},
		{"set_luck",set_luck},
		{"get_item_flags",get_item_flags},
		{"check_bind",check_bind},
		{"check_use_bind",check_use_bind},
		{"check_jipin",check_jipin},
		{"set_bind",set_bind},
		{"set_use_bind",set_use_bind},
		{"set_jipin",set_jipin},
		{"get_duration",get_duration},
		{"set_duration",set_duration},
		{"get_number",get_number},
		{"get_updac",get_updac},
		{"set_updac",set_updac},
		{"get_updmac",get_updmac},
		{"set_updmac",set_updmac},
		{"get_upddc",get_upddc},
		{"set_upddc",set_upddc},
		{"get_updmc",get_updmc},
		{"set_updmc",set_updmc},
		{"get_updsc",get_updsc},
		{"set_updsc",set_updsc},
		{"get_createtime",get_createtime},
		{0,0},
	};
	ScriptLuaItemBind::ScriptLuaItemBind(void)
	{
	}
	ScriptLuaItemBind::~ScriptLuaItemBind(void)
	{
	}
	void ScriptLuaItemBind::init(lua_State* L)
	{
		luaL_newmetatable(L, "oncient.item");
		lua_pushvalue(L,-1);
		lua_setfield(L,-2,"__index");
		luaL_register(L,0,lib_funcs);
		lua_pop(L,1);
	}
	void ScriptLuaItemBind::bind(lua_State* L,SubItem* item)
	{
		SubItem** si = (SubItem**)lua_newuserdata(L, sizeof(item));
		*si = item;

		luaL_getmetatable(L, "oncient.item");
		lua_setmetatable(L, -2);
	}
}