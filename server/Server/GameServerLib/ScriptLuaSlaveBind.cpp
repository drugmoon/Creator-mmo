#include "StdAfx.h"
#include "ScriptLuaSlaveBind.h"
#include "ScriptLua.h"
#include "ScriptLuaMapBind.h"
#include "ScriptLuaPlayerBind.h"
#include "PlaySlave.h"
#define GET_SLAVE	PlaySlave* slave = *(PlaySlave**)luaL_checkudata(L,1,"oncient.slave");

namespace OGSLib
{
	ScriptLuaSlaveBind::ScriptLuaSlaveBind(void)
	{
	}

	ScriptLuaSlaveBind::~ScriptLuaSlaveBind(void)
	{
	}

	static int get_map(lua_State* L)
	{
		GET_SLAVE;
		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);	
		PlayMap* map = slave->GetMap();
		if( map )
		{
			This->get_mapbind()->bind(L,map);
			return 1;
		}
		return 0;
	}

	static int get_level(lua_State* L)
	{
		GET_SLAVE;
		lua_pushinteger(L,slave->level());
		return 1;
	}

	static int get_attr(lua_State* L)
	{
		GET_SLAVE;
		int attrid = luaL_checkinteger(L,2);
		switch(attrid)
		{
		case 1:  lua_pushinteger(L,slave->BaseAttributes()->mMaxMp);return 1;break;
		case 2:  lua_pushinteger(L,slave->BaseAttributes()->mMaxHp);return 1;break;		
		case 3:  lua_pushinteger(L,slave->BaseAttributes()->mAC);return 1;break;
		case 4:  lua_pushinteger(L,slave->BaseAttributes()->mACMax);return 1;break;
		case 5:  lua_pushinteger(L,slave->BaseAttributes()->mMAC);return 1;break;
		case 6:  lua_pushinteger(L,slave->BaseAttributes()->mMACMax);return 1;break;
		case 7:  lua_pushinteger(L,slave->BaseAttributes()->mDC);return 1;break;
		case 8:  lua_pushinteger(L,slave->BaseAttributes()->mDCMax);return 1;break;
		case 9:  lua_pushinteger(L,slave->BaseAttributes()->mMC);return 1;break;
		case 10: lua_pushinteger(L,slave->BaseAttributes()->mMCMax);return 1;break;
		case 11: lua_pushinteger(L,slave->BaseAttributes()->mSC);return 1;break;
		case 12: lua_pushinteger(L,slave->BaseAttributes()->mSCMax);return 1;break;
		case 13: lua_pushinteger(L,slave->BaseAttributes()->mAccuracy);return 1;break;
		case 14: lua_pushinteger(L,slave->BaseAttributes()->mDodge);return 1;break;
		case 15: lua_pushinteger(L,slave->BaseAttributes()->mLuck);return 1;break;
		case 16: lua_pushinteger(L,slave->BaseAttributes()->mAntiMagic);return 1;break;
		case 17: lua_pushinteger(L,slave->BaseAttributes()->mAntiPoison);return 1;break;
		case 18: lua_pushinteger(L,slave->BaseAttributes()->mHpRecover);return 1;break;
		case 19: lua_pushinteger(L,slave->BaseAttributes()->mMpRecover);return 1;break;
		case 20: lua_pushinteger(L,slave->BaseAttributes()->mPoisonRecover);return 1;break;
		case 21: lua_pushinteger(L,slave->BaseAttributes()->mMabi_prob);return 1;break;
		case 22: lua_pushinteger(L,slave->BaseAttributes()->mMabi_dura);return 1;break;
		case 23: lua_pushinteger(L,slave->BaseAttributes()->mDixiao_pres);return 1;break;
		case 24: lua_pushinteger(L,slave->BaseAttributes()->mFuyuan_cd);return 1;break;
		case 25: lua_pushinteger(L,slave->BaseAttributes()->mDropProb);return 1;break;
		case 26: lua_pushinteger(L,slave->BaseAttributes()->mDoubleAttProb);return 1;break;
		case 27: lua_pushinteger(L,slave->BaseAttributes()->mXishouProb);return 1;break;
		case 28: lua_pushinteger(L,slave->BaseAttributes()->mXishouPres);return 1;break;
		case 29: lua_pushinteger(L,slave->BaseAttributes()->mFantanProb);return 1;break;
		case 30: lua_pushinteger(L,slave->BaseAttributes()->mFantanPres);return 1;break;
		case 31: lua_pushinteger(L,slave->BaseAttributes()->mBaojiProb);return 1;break;
		case 32: lua_pushinteger(L,slave->BaseAttributes()->mBaojiPres);return 1;break;
		case 33: lua_pushinteger(L,slave->BaseAttributes()->mXixueProb);return 1;break;
		case 34: lua_pushinteger(L,slave->BaseAttributes()->mXixuePres);return 1;break;
		case 35: lua_pushinteger(L,slave->BaseAttributes()->mFuyuan_pres);return 1;break;
		case 36: lua_pushinteger(L,slave->BaseAttributes()->mHpPres);return 1;break;
		case 37: lua_pushinteger(L,slave->BaseAttributes()->mMpPres);return 1;break;
		case 38: lua_pushinteger(L,slave->BaseAttributes()->mFight);return 1;break;
		case 39: lua_pushinteger(L,slave->BaseAttributes()->mPKIncrement);return 1;break;
		case 40: lua_pushinteger(L,slave->BaseAttributes()->mPKReduce);return 1;break;
		case 41: lua_pushinteger(L,slave->BaseAttributes()->mHolyDam);return 1;break;
		case 42: lua_pushinteger(L,slave->BaseAttributes()->mDamIncrement);return 1;break;
		case 43: lua_pushinteger(L,slave->BaseAttributes()->mDamReduce);return 1;break;
		}
		return 0;
	}

	static int set_attr(lua_State* L)
	{
		GET_SLAVE;
		int attrid = luaL_checkinteger(L,2);
		int value = luaL_checkinteger(L,3);
		switch(attrid)
		{
		case 1:  slave->BaseAttributes()->mMaxMp=value;break;
		case 2:  slave->BaseAttributes()->mMaxHp=value;break;		
		case 3:  slave->BaseAttributes()->mAC=value;break;
		case 4:  slave->BaseAttributes()->mACMax=value;break;
		case 5:  slave->BaseAttributes()->mMAC=value;break;
		case 6:  slave->BaseAttributes()->mMACMax=value;break;
		case 7:  slave->BaseAttributes()->mDC=value;break;
		case 8:  slave->BaseAttributes()->mDCMax=value;break;
		case 9:  slave->BaseAttributes()->mMC=value;break;
		case 10: slave->BaseAttributes()->mMCMax=value;break;
		case 11: slave->BaseAttributes()->mSC=value;break;
		case 12: slave->BaseAttributes()->mSCMax=value;break;
		case 13: slave->BaseAttributes()->mAccuracy=value;break;
		case 14: slave->BaseAttributes()->mDodge=value;break;
		case 15: slave->BaseAttributes()->mLuck=value;break;
		case 16: slave->BaseAttributes()->mAntiMagic=value;break;
		case 17: slave->BaseAttributes()->mAntiPoison=value;break;
		case 18: slave->BaseAttributes()->mHpRecover=value;break;
		case 19: slave->BaseAttributes()->mMpRecover=value;break;
		case 20: slave->BaseAttributes()->mPoisonRecover=value;break;
		case 21: slave->BaseAttributes()->mMabi_prob=value;
				 {
					 if(slave->BaseAttributes()->mMabi_prob <= 0)
					 {
						 slave->BaseAttributes()->mMabi_prob =value;
					 }
					 else
					 {
						 slave->BaseAttributes()->mMabi_prob =max(slave->BaseAttributes()->mMabi_prob,value);
					 }
				 }
				 break;
		case 22: slave->BaseAttributes()->mMabi_dura=value;
				 {
					 if(slave->BaseAttributes()->mMabi_dura <= 0)
					 {
						 slave->BaseAttributes()->mMabi_dura =value;
					 }
					 else
					 {
						 slave->BaseAttributes()->mMabi_dura =max(slave->BaseAttributes()->mMabi_dura,value);
					 }
				 }
				 break;
		case 23: slave->BaseAttributes()->mDixiao_pres=value;break;
		case 24: 
			{
				if(slave->BaseAttributes()->mFuyuan_cd <= 0)
				{
					slave->BaseAttributes()->mFuyuan_cd =value;
				}
				else
				{
					slave->BaseAttributes()->mFuyuan_cd =min(slave->BaseAttributes()->mFuyuan_cd,value);
				}
			}
			break;
		case 25: slave->BaseAttributes()->mDropProb=value;break;
		case 26: slave->BaseAttributes()->mDoubleAttProb=value;break;
		case 27: slave->BaseAttributes()->mXishouProb=value;break;
		case 28: slave->BaseAttributes()->mXishouPres=value;break;
		case 29: slave->BaseAttributes()->mFantanProb=value;break;
		case 30: slave->BaseAttributes()->mFantanPres=value;break;
		case 31: slave->BaseAttributes()->mBaojiProb=value;break;
		case 32: slave->BaseAttributes()->mBaojiPres=value;break;
		case 33: slave->BaseAttributes()->mXixueProb=value;break;
		case 34: slave->BaseAttributes()->mXixuePres=value;break;
		case 35: slave->BaseAttributes()->mFuyuan_pres=value;break;
		case 36: slave->BaseAttributes()->mHpPres=value;break;
		case 37: slave->BaseAttributes()->mMpPres=value;break;
		case 38: slave->BaseAttributes()->mFight=value;break;
		case 39: slave->BaseAttributes()->mPKIncrement=value;break;
		case 40: slave->BaseAttributes()->mPKReduce=value;break;
		case 41: slave->BaseAttributes()->mHolyDam=value;break;
		case 42: slave->BaseAttributes()->mDamIncrement=value;break;
		case 43: slave->BaseAttributes()->mDamReduce=value;break;
		}
		return 0;
	}

	static int recal_attr(lua_State* L)
	{
		GET_SLAVE;
		slave->CalculateAttribute();
		slave->NotifyAttributeChange();
		slave->NotifyHPMPChange(slave,0,0);
		return 0;
	}

	static int set_hp(lua_State* L)
	{
		GET_SLAVE;
		int hp = luaL_checkinteger(L,2);
		slave->Hp(hp);
		return 0;
	}

	static int get_hp(lua_State* L)
	{
		GET_SLAVE;
		lua_pushinteger(L,slave->Hp());
		return 1;
	}

	static int add_hp(lua_State* L)
	{
		GET_SLAVE;
		int hp = luaL_checkinteger(L,2);
		slave->AddHP(hp);
		return 0;
	}

	static int set_atk_skill(lua_State* L)
	{
		GET_SLAVE;
		int skill = luaL_checkinteger(L,2);
		slave->setDefSkill(skill);
		return 0;
	}

	static int get_owner(lua_State* L)
	{
		GET_SLAVE;
		DomainPlay * player = slave->Owner();
		if (player == NULL)
		{
			return 0;
		}
		else
		{
			lua_getglobal(L,"server");
			lua_getfield(L,-1,"This");
			OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
			lua_pop(L,2);
			This->get_playerbing()->bind(L,player);
			return 1;
		}
	}

	static int get_id(lua_State* L)
	{
		GET_SLAVE;
		lua_pushinteger(L,slave->ID());
		return 1;
	}

	static const luaL_reg lib_funcs[]={
		{"get_map", get_map},
		{"get_level", get_level},
		{"set_attr", set_attr},
		{"get_attr", get_attr},
		{"set_hp", set_hp},
		{"get_hp", get_hp},
		{"add_hp", add_hp},
		{"recal_attr", recal_attr},
		{"set_atk_skill", set_atk_skill},
		{"get_owner", get_owner},
		{"get_id", get_id},
		{0,0}
	};

	void ScriptLuaSlaveBind::init(lua_State* L)
	{
		luaL_newmetatable(L, "oncient.slave");
		lua_pushvalue(L,-1);
		lua_setfield(L,-2,"__index");
		luaL_register(L,0,lib_funcs);
		lua_pop(L,1);
	}

	void ScriptLuaSlaveBind::bind(lua_State* L,PlaySlave* slave)
	{
		PlaySlave** a = (PlaySlave**)lua_newuserdata(L, sizeof(slave));
		*a = slave;

		luaL_getmetatable(L, "oncient.slave");
		lua_setmetatable(L, -2);
	}
}