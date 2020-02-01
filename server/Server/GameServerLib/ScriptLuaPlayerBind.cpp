#include "StdAfx.h"
#include "ScriptLuaPlayerBind.h"
#include "DomainPlay.h"
#include "ScriptLua.h"
#include "ScriptLuaMapBind.h"
#include "ScriptLuaItemBind.h"
#include "ScriptLuaSlaveBind.h"
#include "DomainGroup.h"
#include "DomainGuild.h"
#include "SubItem.h"
#include "ManagerStatus.h"
#include "GameServerLib.h"
#include "GameSession.h"
#include "ManagerItemDef.h"
#include "DomainData.h"
#include "PlayMap.h"
#include "ManagerMap.h"
#include "PlayNpc.h"
#include "SubSkill.h"
#include "PlaySlave.h"
#include "ManagerSwordDef.h"
#include "ManagerLevelInfo.h"
#include "ManagerMonDef.h"
#include "ManagerConsignment.h"
#include "ManagerFly.h"
#include "GuildGuild.h"
#include "zlib/zlib.h"

#define GET_PLAYER		DomainPlay* player = *(DomainPlay**)luaL_checkudata(L,1,"oncient.player");
namespace OGSLib
{
	static int get_name(lua_State* L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->Name());
		return 1;
	}
	static int get_seed_name(lua_State* L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->SeedName());
		return 1;
	}
	static int enter_map(lua_State* L)
	{
		GET_PLAYER;
		const char* map_id = luaL_checkstring(L,2);
		int x = static_cast<int>(luaL_checkinteger(L,3));
		int y = static_cast<int>(luaL_checkinteger(L,4));
		std::string mapid = map_id;
		PlayMap* m = player->GetGSL()->GetMapManager()->FindMap(mapid);
		if( m && m->GetBlockData(x,y,2)==0 )
		{
			player->EnterMap(mapid,x,y);
		}
		return 0;
	}
	static int move(lua_State* L)
	{
		GET_PLAYER;
		int x = static_cast<int>(luaL_checkinteger(L,2));
		int y = static_cast<int>(luaL_checkinteger(L,3));
		player->MapEnterTick(0);
		player->ForceMove(x,y,player->Dir());
		return 0;
	}
	static int use_item(lua_State* L)
	{
		GET_PLAYER;
		const char* item_name = 0;
		int item=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			ItemDef* id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);;
			if( id )
			{
				item = id->mTypeID;
			}
		}
		if( item > 0 )
		{
			player->use_item_by_type(item);
		}
		return 0;
	}
	static int use_item_type_pos(lua_State* L)
	{
		GET_PLAYER;
		int type = luaL_checkinteger(L,2);
		int pos = luaL_checkinteger(L,3);
		int number = 1;
		if( lua_isnumber(L,4) )
		{
			number = luaL_checkinteger(L,4);
		}
		if( type > 0 )
		{
			player->BagUseItem(pos,type,number);
		}
		return 0;
	}
	static int set_fashion_cloth(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L, 2);
		if (pos <= -1000)
		{
			player->changeShowFashion(pos);
		}
		return 0;
	}
	static int use_fashion_cloth(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L, 2);
		int id = luaL_checkinteger(L, 3);
		//if (pos <= -1000)
		//{
			player->bag_use_item_fashion(pos,id);
		//}
		return 0;
	}
	void SetOption(const char* option,SubItem* si)
	{
		char word[256];
		strncpy_s(word,_countof(word),option,255);
		word[255]=0;
		_strlwr_s(word,_countof(word));
		size_t len = strlen(word);

		if( 0 == strcmp(word,"bind") )
		{
			si->mItemFlags |= ITEM_FLAG_BIND;
		}
		if( 0 == strcmp(word,"usebind") )
		{
			si->mItemFlags |= ITEM_FLAG_USE_BIND;
		}
		int id = 0;
		if( 1 == sscanf_s(word,"adddc(%d)",&id) )
		{
			si->mAddDC = id;
		}
		if( 1 == sscanf_s(word,"addmc(%d)",&id) )
		{
			si->mAddMC = id;
		}
		if( 1 == sscanf_s(word,"addsc(%d)",&id) )
		{
			si->mAddSC = id;
		}
		if( 1 == sscanf_s(word,"addac(%d)",&id) )
		{
			si->mAddAC = id;
		}
		if( 1 == sscanf_s(word,"addmac(%d)",&id) )
		{
			si->mAddMAC = id;
		}
		if( 1 == sscanf_s(word,"upddc(%d)",&id) )
		{
			si->mUpdDC = id;
		}
		if( 1 == sscanf_s(word,"updmc(%d)",&id) )
		{
			si->mUpdMC = id;
		}
		if( 1 == sscanf_s(word,"updsc(%d)",&id) )
		{
			si->mUpdSC = id;
		}
		if( 1 == sscanf_s(word,"updac(%d)",&id) )
		{
			si->mUpdAC = id;
		}
		if( 1 == sscanf_s(word,"updmac(%d)",&id) )
		{
			si->mUpdMAC = id;
		}
		if( 1 == sscanf_s(word,"addhp(%d)",&id) )
		{
			si->mAddHp = id;
		}
		if( 1 == sscanf_s(word,"addmp(%d)",&id) )
		{
			si->mAddMp = id;
		}
		if( 1 == sscanf_s(word,"luck(%d)",&id) )
		{
			si->mLuck = id;
		}
		if( 1 == sscanf_s(word,"level(%d)",&id) )
		{
			si->mLevel = id;
		}
		if( 1 == sscanf_s(word,"duration(%d)",&id) )
		{
			si->mDuration = id;
		}
		if( 1 == sscanf_s(word,"zlevel(%d)",&id) )
		{
			si->mZLevel = id;
		}
	}
	void ParseOptions(std::string options,SubItem* si)
	{
		std::string word;
		for( std::string::iterator pos = options.begin();
			pos != options.end();
			++ pos )
		{
			if( *pos == ',' )
			{
				SetOption(word.c_str(),si);
				word.clear();
				continue;
			}
			word.push_back(*pos);
		}
		if( ! word.empty() )
		{
			SetOption(word.c_str(),si);
		}
	}
	static int add_item2(lua_State* L)
	{
		GET_PLAYER;
		int pos = -1;
		const char* item_name = 0;
		int item=0;
		int add_src=100;
		int show_flag=1;
		int rand_add=0;
		ItemDef* id=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item);
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);;
			if( id )
			{
				item = id->mTypeID;
			}
		}
		if( item > 0 && id )
		{
			std::string word;
			int num = static_cast<int>(luaL_checkinteger(L,3));
			if( lua_isnumber(L,4) )
			{
				add_src = luaL_checkinteger(L,4);
			}
			if( lua_isstring(L,5))
			{
				word = luaL_checkstring(L,5);
			}
			if( lua_isnumber(L,6) )
			{
				show_flag = luaL_checkinteger(L,6);
			}
			SubItem ii;
			ii.BuildItemInstance(id,0,false,player->GetGSL()->mItemAddType);
			ParseOptions(word,&ii);
			player->AddItem(&ii,num,add_src,11,show_flag,&pos);
		}
		lua_pushnumber(L,pos);
		return 1;
	}
	static int add_item(lua_State* L)
	{
		GET_PLAYER;
		int pos = -1;
		const char* item_name = 0;
		int item=0;
		int add_src=100;
		int show_flag=1;
		int flag_bind=0;
		int flag_use_bind=0;
		int rand_add=0;
		ItemDef* id=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item);
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);
			if( id )
			{
				item = id->mTypeID;
			}
		}
		if( item > 0 && id )
		{
			int num = static_cast<int>(luaL_checkinteger(L,3));
			if( lua_isnumber(L,4) )
			{
				flag_bind = luaL_checkinteger(L,4);
			}
			if( lua_isnumber(L,5) )
			{
				flag_use_bind = luaL_checkinteger(L,5);
			}
			int flags = 0;
			if( flag_bind )
			{
				flags |= ITEM_FLAG_BIND;
			}
			if( flag_use_bind )
			{
				flags |= ITEM_FLAG_USE_BIND;
			}
			
			if( lua_isnumber(L,6) )
			{
				add_src = luaL_checkinteger(L,6);
				add_src=add_src>0?add_src:100;
			}
			if( lua_isnumber(L,7) )
			{
				rand_add = luaL_checkinteger(L,7);
			}
			bool ra = false;
			if( rand_add>0 )
			{
				ra = true;
			}
			SubItem ii;
			ii.BuildItemInstance(id,flags,ra,player->GetGSL()->mItemAddType);
			if( rand_add == 2 )
			{
				if( lua_isnumber(L,8) )
				{
					ii.mAddAC=luaL_checkinteger(L,8);
				}
				if( lua_isnumber(L,9) )
				{
					ii.mAddMAC=luaL_checkinteger(L,9);
				}
				if( lua_isnumber(L,10) )
				{
					ii.mAddDC=luaL_checkinteger(L,10);
				}
				if( lua_isnumber(L,11) )
				{
					ii.mAddMC=luaL_checkinteger(L,11);
				}
				if( lua_isnumber(L,12) )
				{
					ii.mAddSC=luaL_checkinteger(L,12);
				}
				if( lua_isnumber(L,13) )
				{
					ii.mAddHp = luaL_checkinteger(L,13);
				}
				if( lua_isnumber(L,14) )
				{
					ii.mAddMp = luaL_checkinteger(L,14);
				}
			}
			if( lua_isnumber(L,15) )
			{
				show_flag = luaL_checkinteger(L,15);
			}
			player->AddItem(&ii,num,add_src,11,show_flag,&pos);
		}
		lua_pushnumber(L,pos);
		return 1;
	}
	static int add_item_log(lua_State* L)
	{
		GET_PLAYER;
		const char* item_name = 0;
		int item=0;
		ItemDef* id=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item);
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);;
			if( id )
			{
				item = id->mTypeID;
			}
		}
		int log_src = luaL_checkinteger(L,3);
		int flag_bind=0;
		int flag_use_bind=0;
		int rand_add=0;
		if( item > 0 && id )
		{
			int num = static_cast<int>(luaL_checkinteger(L,4));
			if( lua_isnumber(L,5) )
			{
				flag_bind = luaL_checkinteger(L,5);
			}
			if( lua_isnumber(L,6) )
			{
				flag_use_bind = luaL_checkinteger(L,6);
			}
			int flags = 0;
			if( flag_bind )
			{
				flags |= ITEM_FLAG_BIND;
			}
			if( flag_use_bind )
			{
				flags |= ITEM_FLAG_USE_BIND;
			}
			if( lua_isnumber(L,7) )
			{
				rand_add = luaL_checkinteger(L,7);
			}
			bool ra = false;
			if( rand_add>0 )
			{
				ra = true;
			}
			SubItem ii;
			ii.BuildItemInstance(id,flags,ra,player->GetGSL()->mItemAddType);
			if( rand_add == 2 )
			{
				if( lua_isnumber(L,8) )
				{
					ii.mAddAC=luaL_checkinteger(L,8);
				}
				if( lua_isnumber(L,9) )
				{
					ii.mAddMAC=luaL_checkinteger(L,9);
				}
				if( lua_isnumber(L,10) )
				{
					ii.mAddDC=luaL_checkinteger(L,10);
				}
				if( lua_isnumber(L,11) )
				{
					ii.mAddMC=luaL_checkinteger(L,11);
				}
				if( lua_isnumber(L,12) )
				{
					ii.mAddSC=luaL_checkinteger(L,12);
				}
				if( lua_isnumber(L,13) )
				{
					ii.mAddHp = luaL_checkinteger(L,13);
				}
				if( lua_isnumber(L,14) )
				{
					ii.mAddMp = luaL_checkinteger(L,14);
				}
			}
			player->AddItem(&ii,num,log_src,11,1);
		}
		return 0;
	}

	static int add_item_lottery(lua_State* L)
	{
		GET_PLAYER;
		int pos = -1;
		const char* item_name = 0;
		int item=0;
		int flag = 1;
		int flag_bind=0;
		int flag_use_bind=0;
		int rand_add=0;
		ItemDef* id=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item);
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);;
			if( id )
			{
				item = id->mTypeID;
			}
		}
		if( item > 0 && id )
		{
			int num = static_cast<int>(luaL_checkinteger(L,3));
			if( lua_isnumber(L,4) )
			{
				flag_bind = luaL_checkinteger(L,4);
			}
			if( lua_isnumber(L,5) )
			{
				flag_use_bind = luaL_checkinteger(L,5);
			}
			int flags = 0;
			if( flag_bind )
			{
				flags |= ITEM_FLAG_BIND;
			}
			if( flag_use_bind )
			{
				flags |= ITEM_FLAG_USE_BIND;
			}
			if( lua_isnumber(L,6) )
			{
				rand_add = luaL_checkinteger(L,6);
			}
			bool ra = false;
			if( rand_add>0 )
			{
				ra = true;
			}
			SubItem ii;
			ii.BuildItemInstance(id,flags,ra,player->GetGSL()->mItemAddType);
			if( rand_add == 2 )
			{
				if( lua_isnumber(L,7) )
				{
					ii.mAddAC=luaL_checkinteger(L,7);
				}
				if( lua_isnumber(L,8) )
				{
					ii.mAddMAC=luaL_checkinteger(L,8);
				}
				if( lua_isnumber(L,9) )
				{
					ii.mAddDC=luaL_checkinteger(L,9);
				}
				if( lua_isnumber(L,10) )
				{
					ii.mAddMC=luaL_checkinteger(L,10);
				}
				if( lua_isnumber(L,11) )
				{
					ii.mAddSC=luaL_checkinteger(L,11);
				}
				if( lua_isnumber(L,12) )
				{
					ii.mAddHp = luaL_checkinteger(L,12);
				}
				if( lua_isnumber(L,13) )
				{
					ii.mAddMp = luaL_checkinteger(L,13);
				}
			}
			if( lua_isnumber(L,14) )
			{
				flag = luaL_checkinteger(L,14);
			}
			player->AddItem_Lottery(&ii,num,100,11,flag,&pos);
		}
		lua_pushnumber(L,pos);
		return 1;
	}
	static int add_and_dress_equip(lua_State* L)
	{
		// item_name(item_id),onPos,bind,add_src,show_flag
		GET_PLAYER;
		int pos = 0;
		const char* item_name = 0;
		int item=0;
		int add_src=100;
		int show_flag=1;
		int flag_bind=0;
		int onPos = 0;
		ItemDef* id=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item);
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);
			if( id )
			{
				item = id->mTypeID;
			}
		}
		if( item > 0 && id )
		{
			if ( lua_isnumber(L,3))
			{
				onPos = luaL_checkinteger(L,3);
			}
			if( lua_isnumber(L,4) )
			{
				flag_bind = luaL_checkinteger(L,4);
			}
			int flags = 0;
			if( flag_bind )
			{
				flags |= ITEM_FLAG_BIND;
			}
			if( lua_isnumber(L,5) )
			{
				add_src = luaL_checkinteger(L,5);
				add_src=add_src>0?add_src:100;
			}
			SubItem ii;
			ii.BuildItemInstance(id,flags,false,player->GetGSL()->mItemAddType);
			if( lua_isnumber(L,6) )
			{
				show_flag = luaL_checkinteger(L,6);
			}
			if (onPos < 0)
			{
				onPos = player->AddAndDressEquip(&ii,add_src,onPos,show_flag);
			}
		}
		lua_pushnumber(L,onPos);
		return 1;
	}
	static int log_item_change(lua_State* L)
	{
		GET_PLAYER;
		int log_src=luaL_checkinteger(L,2);
		int log_act=luaL_checkinteger(L,3);
		int item_pos=luaL_checkinteger(L,4);
		player->LogItemChange(log_src,log_act,player->get_item_by_pos(item_pos));
		return 0;
	}
	static int use_gem(lua_State* L)
	{
		GET_PLAYER;
		int equip_pos=luaL_checkinteger(L,2);
		int gem_pos=luaL_checkinteger(L,3);
		int ret = player->BagUseGem(equip_pos, gem_pos);
		lua_pushinteger(L,ret);
		return 1;
	}
	static int undress_gem(lua_State* L)
	{
		GET_PLAYER;
		int gem_pos=luaL_checkinteger(L,2);
		int ret = player->UndressGem(gem_pos);
		lua_pushinteger(L,ret);
		return 1;
	}
	static int remove_item_by_type_pos(lua_State* L)
	{
		GET_PLAYER;
		int type = luaL_checkinteger(L,2);
		int pos = luaL_checkinteger(L,3);
		int number = 1;
		if( lua_isnumber(L,4) )
		{
			number = luaL_checkinteger(L,4);
		}
		int log_src = 167;
		if( lua_isnumber(L,5) )
		{
			log_src = luaL_checkinteger(L,5);
		}
		int ret = player->RemItemPosition(type,pos,number,log_src);
		lua_pushinteger(L,ret);
		return 1;
	}
	static int remove_item(lua_State* L)
	{
		GET_PLAYER;
		const char* item_name = 0;
		int item=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			ItemDef* id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);;
			if( id )
			{
				item = id->mTypeID;
			}
		}
		if( item > 0 )
		{
			int num = static_cast<int>(luaL_checkinteger(L,3));
			int flags = 1;
			int bind = -1;
			if( lua_isnumber(L,4) )
			{
				bind = static_cast<int>(luaL_checkinteger(L,4));
			}
			if( lua_isnumber(L,5) )
			{
				flags = static_cast<int>(luaL_checkinteger(L,5));
			}
			int src_log = 110;
			if( lua_isnumber(L,6) )
			{
				src_log = static_cast<int>(luaL_checkinteger(L,6));
			}
			int ret = player->RemItemNumber(item,num,flags,src_log,bind);
			lua_pushinteger(L,ret);
			return 1;
		}
		lua_pushinteger(L,0);
		return 1;
	}
	static int num_item(lua_State* L)
	{
		GET_PLAYER;
		const char* item_name = 0;
		int item=0;
		if( lua_isnumber(L,2) )
		{
			item = static_cast<int>(luaL_checkinteger(L,2));
		}
		else
		{
			item_name = luaL_checkstring(L,2);
		}
		if( item <= 0 )
		{
			ItemDef* id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);;
			if( id )
			{
				item = id->mTypeID;
			}
		}
		if( item > 0 )
		{
			int flags = 1;
			if( lua_isnumber(L,3) )
			{
				flags = static_cast<int>(luaL_checkinteger(L,3));
			}
			int bind = -1;
			if( lua_isnumber(L,4) )
			{
				bind = static_cast<int>(luaL_checkinteger(L,4));
			}
			int num = player->NumItem(item,flags,bind);
			lua_pushinteger(L,num);
			return 1;
		}
		lua_pushinteger(L,0);
		return 1;
	}

	static int add_skill(lua_State* L)
	{
		GET_PLAYER;
		int skill = static_cast<int>(luaL_checkinteger(L,2));
		int lv = 1;
		if( lua_isnumber(L,3) )
		{
			lv = luaL_checkinteger(L,3);
		}
		player->AddSkill(skill,lv);
		return 0;
	}
	static int use_skill(lua_State* L)
	{
		GET_PLAYER;
		int skill = static_cast<int>(luaL_checkinteger(L,2));
		player->UseSkill(skill,0,0,0);
		return 0;
	}
	static int skill_level(lua_State* L)
	{
		GET_PLAYER;
		int skill = static_cast<int>(luaL_checkinteger(L,2));
		SubSkill* sk = player->FindSkill(skill);
		if( sk )
		{
			lua_pushinteger(L,sk->mLevel);
		}
		else
		{
			lua_pushinteger(L,0);
		}
		return 1;
	}
	static int set_skill_level(lua_State* L)
	{
		GET_PLAYER;
		int skill = static_cast<int>(luaL_checkinteger(L,2));
		int level = static_cast<int>(luaL_checkinteger(L,3));
		player->SetSkillLevel(skill,level);
		return 0;
	}
	static int add_skill_exp(lua_State* L)
	{
		GET_PLAYER;
		int skill = static_cast<int>(luaL_checkinteger(L,2));
		int exp = static_cast<int>(luaL_checkinteger(L,3));
		player->AddSkillExp(skill,exp);
		return 0;
	}
	static int check_skill_level_up(lua_State* L)
	{
		GET_PLAYER;
		int skill = static_cast<int>(luaL_checkinteger(L,2));
		int ret = player->CheckSkillLevelUp(skill);
		if (ret)
		{
			SubSkill* sk = player->FindSkill(skill);
			lua_pushinteger(L,sk->mLevel);
		}
		else
		{
			lua_pushinteger(L,0);
		}
		return 1;
	}
	static int set_task_state(lua_State* L)
	{
		GET_PLAYER;
		int task_id = luaL_checkint(L,2);
		int state = luaL_checkint(L,3);
		player->SetTaskState(task_id,state);
		//记录玩家主线通过率
		if (task_id == 1000)
		{
			//player->Param(160,state);
			player->setMainTaskState(state);
		}
		return 0;
	}
	static int set_task_param(lua_State* L)
	{
		GET_PLAYER;
		int task_id = luaL_checkint(L,2);
		int param_id = luaL_checkint(L,3);
		int param = luaL_checkint(L,4);
		player->SetTaskParam(task_id,param_id,param);
		return 0;
	}
	static int get_task_state(lua_State* L)
	{
		GET_PLAYER;
		int task_id = luaL_checkint(L,2);
		int state = player->GetTaskState(task_id);
		lua_pushnumber(L,state);
		return 1;
	}
	static int get_task_param(lua_State* L)
	{
		GET_PLAYER;
		int task_id = luaL_checkint(L,2);
		int param_id = luaL_checkint(L,3);
		int param = player->GetTaskParam(task_id,param_id);
		lua_pushnumber(L,param);
		return 1;
	}
	static int push_task_data(lua_State* L)
	{
		GET_PLAYER;
		int task_id = luaL_checkint(L,2);
		int task_flags = luaL_checkint(L,3);
		player->PushTaskData(task_id,task_flags);
		return 0;
	}
	static int fresh_vcoin_input(lua_State* L)
	{
		GET_PLAYER;
		int mol = luaL_checkint(L,2);
		player->FreshVcoin();
		return 0;
	}
	static int get_level(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->level());
		return 1;
	}
	//static int direct_fly_to_npc(lua_State* L)
	//{
	//	GET_PLAYER;
	//	int fly_id = luaL_checkint(L,2);
	//	lua_pushnumber(L,player->DirectFly(fly_id));
	//	return 1;
	//}
	static int set_level(lua_State* L)
	{
		GET_PLAYER;
		int level = luaL_checkint(L,2);
		if( level >= 0 && level < 1000 )
		{
			lua_pushnumber(L,player->SetUpdateLevel(level));
			return 1;
		}
		return 0;
	}
	static int get_exp(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,(lua_Number)player->exp());
		return 1;
	}
	static int get_exp_next(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,(lua_Number)player->exp_next());
		return 1;
	}
	static int add_exp(lua_State* L)
	{
		GET_PLAYER;
		int exp = luaL_checkint(L,2);
		int src = 150;
		if( lua_isnumber(L,3) )
		{
			src = luaL_checkinteger(L,3);
		}
		if( exp > 0 )
		{
			lua_pushnumber(L,(lua_Number)player->AddExperience(exp,src));
			return 1;
		}
		return 0;
	}
	static int sub_exp(lua_State* L)
	{
		GET_PLAYER;
		int exp = luaL_checkint(L,2);
		int src = 180;
		if( lua_isnumber(L,3) )
		{
			src = luaL_checkinteger(L,3);
		}
		if( exp > 0 )
		{
			lua_pushnumber(L,(lua_Number)player->AddExperience(-exp,src));
			return 1;
		}
		return 0;
	}
	static int get_gamemoney(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,(lua_Number)player->gamemoney());
		return 1;
	}
	static int use_gamemoney(lua_State* L)
	{
		GET_PLAYER;
		int number = luaL_checkint(L,2);
		int src = luaL_checkint(L,3);
		if( number > 0 )
		{
			if( player->UseGameMoney(number,src) )
			{
				lua_pushinteger(L,1);
				return 1;
			}
		}
		return 0;
	}
	static int add_gamemoney(lua_State* L)
	{
		GET_PLAYER;
		int gamemoney = luaL_checkint(L,2);
		if( gamemoney > 0 )
		{
			if(player->AddGameMoney(gamemoney,110))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int sub_gamemoney(lua_State* L)
	{
		GET_PLAYER;
		int gamemoney = luaL_checkint(L,2);
		if( gamemoney > 0 )
		{
			if(player->RemGameMoney(gamemoney,110))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int get_gamemoney_bind(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,(lua_Number)player->gamemoneybind());
		return 1;
	}
	static int add_gamemoney_bind(lua_State* L)
	{
		GET_PLAYER;
		int gamemoney = luaL_checkint(L,2);
		if( gamemoney > 0 )
		{
			if(player->AddGameMoneyBind(gamemoney,110))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int sub_gamemoney_bind(lua_State* L)
	{
		GET_PLAYER;
		int gamemoney = luaL_checkint(L,2);
		if( gamemoney > 0 )
		{
			if(player->RemGameMoneyBind(gamemoney,110))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int get_vcoin_gs(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->vcoings());
		return 1;
	}
	static int get_vcoin(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->vcoin());
		return 1;
	}
	static int get_vcoin_accu(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->vcoinaccu());
		return 1;
	}
	static int add_vcoin(lua_State* L)
	{
		GET_PLAYER;
		int number = luaL_checkint(L,2);
		if( number > 0 )
		{
			int flag = 110;
			const char* e = lua_tostring(L,3);
			if (lua_isnumber(L, 4))
			{
				flag = luaL_checkint(L, 4);
			}
			if (player->AddVcoin(number, flag, e))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int add_congzi(lua_State* L)
	{
		GET_PLAYER;
		int number = luaL_checkint(L,2);
		if( number > 0 )
		{
			player->OnFreshVcoin(number,number,true);
			return 1;
		}
		return 0;
	}
	static int sub_vcoin(lua_State* L)
	{
		GET_PLAYER;
		int number = luaL_checkint(L,2);
		const char* e="script";
		//if( lua_isstring(L,3) )
		//{
		e = lua_tostring(L,3);
		//}
		if( number > 0 )
		{
			if(player->RemVcoin(number,110,e))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int get_vcoin_bind(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->vcoinbind());
		return 1;
	}
	static int add_vcoin_bind(lua_State* L)
	{
		GET_PLAYER;
		int number = luaL_checkint(L,2);
		if( number > 0 )
		{
			if(player->AddVcoinBind(number,110))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int sub_vcoin_bind(lua_State* L)
	{
		GET_PLAYER;
		int number = luaL_checkint(L,2);
		if( number > 0 )
		{
			if(player->RemVcoinBind(number,110))
			{
				lua_pushinteger(L,1);
			}
			else
			{
				lua_pushinteger(L,0);
			}
			return 1;
		}
		return 0;
	}
	static int gen_rand_position(lua_State* L)
	{
		GET_PLAYER;
		int x,y;
		x=y=0;
		if( player->find_validate_map_position(&x,&y) )
		{
			lua_pushinteger(L,x);
			lua_pushinteger(L,y);
			return 2;
		}
		return 0;
	}
	static int go_home(lua_State* L)
	{
		GET_PLAYER;
		player->go_home();
		return 0;
	}
	static int create_guild(lua_State* L)
	{
		GET_PLAYER;
		int lv = luaL_checkint(L,2);
		const char* guild_name = luaL_checkstring(L,3);
		//player->Session()->GetDomainGuild()->CreateGuild(guild_name,lv);
		return 1;
	}
	static int get_guild(lua_State* L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->GuildName().c_str());
		return 1;
	}
	static int get_guild_title(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GuildTitle());
		return 1;
	}
	static int get_guild_pt(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GuildPt());
		return 1;
	}
	static int add_guild_pt(lua_State* L)
	{
		GET_PLAYER;
		int pt = luaL_checkint(L,2);
		player->AddGuildPt(pt);
		return 0;
	}
	static int sub_guild_pt(lua_State* L)
	{
		GET_PLAYER;
		int pt = luaL_checkint(L,2);
		player->SubGuildPt(pt);
		return 0;
	}
	static int get_enter_guild_time(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GuildEnterTime());
		return 1;
	}
	static int change_name(lua_State* L)
	{
		GET_PLAYER;
		std::string name =  luaL_checkstring(L,2);
		lua_pushinteger(L,player->setName(name.c_str()));
		return 1;
	}
	static int get_gender(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->gender());
		return 1;
	}
	static int alert(lua_State* L)
	{
		GET_PLAYER;
		int lv = luaL_checkint(L,2);
		int flags = luaL_checkint(L,3);
		const char* msg = luaL_checkstring(L,4);
		if( msg )
		{
			player->AlertMessage(lv,flags,msg);
		}
		return 0;
	}
	static int get_map(lua_State* L)
	{
		GET_PLAYER;
		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);	
		PlayMap* map = player->GetMap();
		if( map )
		{
			This->get_mapbind()->bind(L,map);
			return 1;
		}
		return 0;
	}
	static int relive(lua_State* L)
	{
		GET_PLAYER;
		int type = luaL_checkint(L,2);
		if( type == 120 )
		{
			const char* map_name = luaL_checkstring(L,3);
			int x = luaL_checkint(L,4);
			int y = luaL_checkint(L,5);
			player->Relive(type,map_name,x,y);
		}
		else
		{
			player->Relive(type,0,0,0);
		}
		return 0;
	}
	static int set_attack_mode(lua_State* L)
	{
		GET_PLAYER;
		int at = luaL_checkint(L,2);
		lua_pushinteger(L,player->ChangeAttackMode(at));
        return 1;
	}

	static int set_tianguan_lv(lua_State* L)
	{
		GET_PLAYER;
		int lv = luaL_checkint(L, 2);
		player->SetTianGuanLv(lv); 
		return 1;
	}

	static int updataChart(lua_State* L)
	{
		GET_PLAYER;
		int winglv = luaL_checkint(L, 2);
		int tianguanlv = luaL_checkint(L, 3);
		player->UpdataChart(winglv, tianguanlv);
		return 1;
	}

	static int get_attack_mode(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->AttackMode());
        return 1;
	}
	static int set_lock_attack_mode(lua_State* L)
	{
		GET_PLAYER;
		int at = luaL_checkint(L,2);
		lua_pushinteger(L,player->LockAttackMode(at));
        return 1;
	}
	static int get_lock_attack_mode(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->LockAttackMode());
        return 1;
	}
	static int get_job_name(lua_State* L)
	{
		GET_PLAYER;
		if( player->job()==JOB_WARRIOR )
		{
			lua_pushstring(L,"warrior");
			return 1;
		}
		if( player->job()==JOB_WIZARD )
		{
			lua_pushstring(L,"wizard");
			return 1;
		}
		if( player->job()==JOB_TAOIST )
		{
			lua_pushstring(L,"taoist");
			return 1;
		}
		return 0;
	}
	static int get_gender_name(lua_State* L)
	{
		GET_PLAYER;
		if( player->gender() == GENDER_MALE )
		{
			lua_pushstring(L,"male");
			return 1;
		}
		if( player->gender() == GENDER_FEMALE )
		{
			lua_pushstring(L,"female");
			return 1;
		}
		return 0;
	}
	static int get_auth_type(lua_State* L)
	{
		GET_PLAYER;
		if( player->Session() )
		{
			lua_pushinteger(L,player->Session()->AuthType());
			return 1;
		}
		return 0;
	}
	static int echo(lua_State* L)
	{
		GET_PLAYER;
		for(int i = 2;i<100;i++)
		{
			const char* msg = lua_tostring(L,i);
			if( msg )
			{
				player->ScriptEcho(msg);
			}
			else
			{
				break;
			}
		}
		return 0;
	}
	static int script_exe_code(lua_State* L)
	{
		GET_PLAYER;
		const char* code = lua_tostring(L,2);
		if( code )
		{
			player->ExeCode(code);
		}
		return 0;
	}
	static int show_npc_flags(lua_State* L)
	{
		GET_PLAYER;
		PlayNpc* npc = *(PlayNpc**)luaL_checkudata(L,2,"oncient.npc");
		int flags = luaL_checkint(L,3);
		player->NPCShowFlags(npc->ID(),flags);
		return 0;
	}
	static int refresh_task_name(lua_State* L)
	{
		GET_PLAYER;
		int task_id = luaL_checkinteger(L,2);
		player->UpdateTaskName(task_id);
		return 0;
	}
	static int refresh_task_desp(lua_State* L)
	{
		GET_PLAYER;
		int task_id = luaL_checkinteger(L,2);
		player->QueryTaskDesp(task_id);
		return 0;
	}
	static int refresh_npc_show_flags_inview(lua_State* L)
	{
		GET_PLAYER;
		player->RefreshNpcShowFlagsInview();
		return 0;
	}
	static int find_raod_goto(lua_State* L)
	{
		GET_PLAYER;
		const char* map = luaL_checkstring(L,2);
		int	x = luaL_checkinteger(L,3);
		int	y = luaL_checkinteger(L,4);
		const char* target = luaL_checkstring(L,5);
		int flags = 0;
		if( lua_isnumber(L,6) )
		{
			flags = luaL_checkinteger(L,6);
		}
		player->findRoadGoto(map,x,y,target,flags);
		return 0;
	}
	static int set_param(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		if(lua_isnumber(L,3))
		{
			int value = luaL_checkint(L,3);
			player->Param(id,value);
		}
		const char* value = luaL_checkstring(L,3);
		std::string ss(value);
		player->Param(id,ss);
        return 0;
	}
	static int get_param(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		lua_pushstring(L,player->Param(id).c_str() );
        return 1;
	}
	static int set_group_param(lua_State* L)
	{
		GET_PLAYER;
		const char* key = luaL_checkstring(L,2);
		const char* value = luaL_checkstring(L,3);
		player->Session()->GetDomainGroup()->Var(key,value);
        return 0;
	}
	static int get_group_param(lua_State* L)
	{
		GET_PLAYER;
		const char* key = luaL_checkstring(L,2);
		lua_pushstring(L,player->Session()->GetDomainGroup()->Var(key).c_str() );

        return 1;
	}

	static int get_group_task_flag(lua_State* L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->Session()->GetDomainGroup()->GetGroupTaskFlag().c_str());
		return 1;
	}

	static int set_guild_param(lua_State* L)
	{
		GET_PLAYER;
		const char* key = luaL_checkstring(L,2);
		const char* value = luaL_checkstring(L,3);
		player->Session()->GetDomainGuild()->Var(key,value);
        return 0;
	}
	static int get_guild_param(lua_State* L)
	{
		GET_PLAYER;
		const char* key = luaL_checkstring(L,2);
		lua_pushstring(L,player->Session()->GetDomainGuild()->Var(key).c_str() );
        return 1;
	}
	static int set_temp_param(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		const char* value = luaL_checkstring(L,3);
		std::string ss(value);
		player->TempParam(id,ss);
        return 0;
	}
	static int get_temp_param(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		lua_pushstring(L,player->TempParam(id).c_str() );
        return 1;
	}
	static int set_timer(lua_State* L)
	{
		GET_PLAYER;
		int i = luaL_checkint(L,2);
		int esp = luaL_checkint(L,3);
		lua_pushinteger(L,player->SetMapTimer(i,esp));
		return 1;
	}
	static int get_timer(lua_State* L)
	{
		GET_PLAYER;
		int i = luaL_checkint(L, 2);
		lua_pushnumber(L, player->GetMapTimer(i));
		return 1;
	}
	static int get_last_login_time(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,static_cast<int>(player->LastLoginTime()));
		return 1;
	}
	static int get_last_logout_time(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,static_cast<int>(player->LastLogoutTime()));
		return 1;
	}
	static int get_create_time(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,static_cast<int>(player->CreateTime()));
		return 1;
	}
	static int get_last_login_ip(lua_State* L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->LastLoginIP().c_str());
		return 1;
	}
	static int countdown_notify(lua_State* L)
	{
		GET_PLAYER;
		int delay = luaL_checkint(L,2);
		const char* msg = luaL_checkstring(L,3);
		std::string ss(msg);
		player->NotifyCountDown(delay,ss);
        return 0;
	}
	static int set_status(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		int dura = luaL_checkint(L,3);
		int param = luaL_checkint(L,4);
		double gap = 1.0;
		if( lua_isnumber(L,5) )
		{
			gap = luaL_checknumber(L,5);
		}
		int flags=0;
		if( lua_isnumber(L,6) )
		{
			flags = luaL_checkinteger(L,6);
		}
		if( id >0 && id<PlayActiveObject::STATUS_TYPE_NUMBER )
		{
			player->AddStatus(id,dura * 1000,param,gap,flags);
		}
		return 0;
	}

	static int set_buff(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		double param = 0;
		if( lua_isnumber(L,3) )
		{
			param = luaL_checknumber(L,3);
		}
		player->AddBuff(id,param);
		return 0;
	}

	static int enable_buff(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		int enable = luaL_checkint(L,3);
		player->EnableBuff(id, enable > 0 ? true:false);
		return 0;
	}

	static int clear_buff(lua_State* L)
	{		
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		player->ClearBuff(id);
		return 0;
	}

	static int have_buff(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		lua_pushinteger(L,player->HaveBuff(id));
		return 1;
	}

	// 获取buff有效最大值
	static int get_buff_valid_max_value(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		BuffInfo * p = player->Session()->GetGameServerLib()->GetStatusManager()->GetBuffInfo(id);
		if (p)
		{
			lua_pushinteger(L, p->mValidMaxValue);
			return 1;
		}
		return 0;
	}

	// 获取buff剩余值
	static int get_buff_valid_value(lua_State * L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		lua_pushinteger(L, player->GetBuffValidValue(id));
		return 1;
	}

	static int set_slave_status(lua_State* L)
	{
		GET_PLAYER;
		int type = luaL_checkint(L,2);

		PlaySlave * pSalve = player->GetSlave(type);
		if (pSalve == NULL)
			return 0;

		int id = luaL_checkint(L,3);
		int dura = luaL_checkint(L,4);
		int param = luaL_checkint(L,5);

		double gap = 1.0;
		if( lua_isnumber(L,6) )
		{
			gap = luaL_checknumber(L,6);
		}
		int flags=0;
		if( lua_isnumber(L,7) )
		{
			flags = luaL_checkinteger(L,7);
		}
		if( id >0 && id<PlayActiveObject::STATUS_TYPE_NUMBER )
		{
			pSalve->AddStatus(id,dura * 1000,param,gap,flags);
		}
		return 0;
	}
	static int set_status_plus(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		int dura = luaL_checkint(L,3);
		int param = luaL_checkint(L,4);
		double gap = 1.0;
		if( lua_isnumber(L,5) )
		{
			gap = luaL_checknumber(L,5);
		}
		int flags=0;
		if( lua_isnumber(L,6) )
		{
			flags = luaL_checkinteger(L,6);
		}
		int level[6]={0,0,0,0,0,0};
		for (int i = 0;i<6;i++)
		{
			if (lua_isnumber(L,7+i))
			{
				level[i]=luaL_checkinteger(L,7+i);
			}
		}
		if( id >0 && id<PlayActiveObject::STATUS_TYPE_NUMBER )
		{
			player->AddStatus(id,dura * 1000,param,gap,flags,level);
		}
		return 0;
	}
	static int get_status_param(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		lua_pushinteger(L,player->StatusParam(id));
		return 1;
	}
	static int get_status_duration(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		int leftTime = player->GetStatusDuration(id);
		lua_pushinteger(L,leftTime);
		return 1;
	}
	static int get_status_data(lua_State*L)
	{
		GET_PLAYER;
		int status_id = luaL_checkint(L,2);
		int lv = luaL_checkint(L,3);
		StatusInfo*status_data = player->GetStatusData(status_id,lv);
		if (status_data)
		{
			lua_pushinteger(L,status_data->mMaxHp);
			lua_pushinteger(L,status_data->mMaxMp);
			lua_pushinteger(L,status_data->mAC);
			lua_pushinteger(L,status_data->mACMax);
			lua_pushinteger(L,status_data->mMAC);
			lua_pushinteger(L,status_data->mMACMax);
			lua_pushinteger(L,status_data->mDC);
			lua_pushinteger(L,status_data->mDCMax);
			lua_pushinteger(L,status_data->mMC);
			lua_pushinteger(L,status_data->mMCMax);
			lua_pushinteger(L,status_data->mSC);
			lua_pushinteger(L,status_data->mSCMax);
			lua_pushinteger(L,status_data->mBaojiProb);
			lua_pushinteger(L,status_data->mBaijiPres);
			lua_pushinteger(L,status_data->mNoDef);
			lua_pushinteger(L, status_data->mFightPoint);
			return 16;
		}
		return 1;
	}
	static int is_status(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		lua_pushboolean(L,player->IsStatus(id));
		return 1;
	}
	static int clear_status(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		if( id >0 && id<PlayActiveObject::STATUS_TYPE_NUMBER )
		{
			player->ClearStatus(id);
		}
		return 0;
	}
	static int get_weapon_luck(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->GetWeaponLuck());
		return 1;
	}
	static int check_euqip(lua_State* L)
	{
		GET_PLAYER;
		int pp = luaL_checkint(L,2);
		lua_pushnumber(L,player->CheckEquiped(pp));
		return 1;
	}
	static int set_weapon_luck(lua_State* L)
	{
		GET_PLAYER;
		int luck = luaL_checkint(L,2);
		lua_pushnumber(L,player->SetWeaponLuck(luck));
		return 1;
	}
	static int get_pk_value(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->PKValue());
		return 1;
	}
	static int set_pk_value(lua_State* L)
	{
		GET_PLAYER;
		int pk_value = luaL_checkint(L,2);
		lua_pushnumber(L,player->SetPKValue(pk_value));
		return 1;
	}
	static int sub_pk_value(lua_State* L)
	{
		GET_PLAYER;
		int pk_value = luaL_checkint(L,2);
		lua_pushnumber(L,player->SubPKValue(pk_value));
		return 1;
	}
	static int get_honor(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->Honor());
		return 1;
	}
	static int set_honor(lua_State* L)
	{
		GET_PLAYER;
		int value = luaL_checkint(L,2);
		player->SetHonor(value);
		return 0;
	}
	static int sub_honor(lua_State* L)
	{
		GET_PLAYER;
		int value = luaL_checkint(L,2);
		player->SubHonor(value);
		return 0;
	}
	static int add_honor(lua_State* L)
	{
		GET_PLAYER;
		int value = luaL_checkint(L,2);
		player->AddHonor(value);
		return 0;
	}
	static int get_pos(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->X());
		lua_pushnumber(L,player->Y());
		return 2;
	}
	static int chat_on(lua_State* L)
	{
		GET_PLAYER;
		player->TalkProhibited(0);
		return 0;
	}
	static int chat_off(lua_State* L)
	{
		GET_PLAYER;
		int t = luaL_checkint(L,2);
		player->TalkProhibited(t);
		return 0;
	}
	static int set_model(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		int vl = luaL_checkint(L,3);
		player->SetModel(id,vl);
		return 0;
	}
	static int get_model(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);
		lua_pushinteger(L,player->GetModel(id));
		return 1;
	}
	static int num_bag_black(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->NumBagBlack());
		return 1;
	}
	static int num_bag(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->NumBag());
		return 1;
	}
	static int get_offline_time(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,(int)player->Session()->GetCharacter()->offline_time);
		return 1;
	}
	static int set_offline_time(lua_State* L)
	{
		GET_PLAYER;
		int v = luaL_checkinteger(L,2);
		if( v > 0 )
		{
			player->Session()->GetCharacter()->offline_time = 0;
		}
		return 0;
	}
	static int get_online_time(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,(int)player->OnlineTime());
		return 1;
	}
	static int get_online_time_all(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,(int)player->AllOnlineTime());
		return 1;
	}
	static int get_online_time_today(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,(int)player->TodayOnlineTime());
		return 1;
	}
	static int get_online_time_yestoday(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,(int)player->YestodayOnlineTime());
		return 1;
	}
	static int set_name_pre(lua_State* L)
	{
		GET_PLAYER;
		const char* pre = luaL_checkstring(L,2);
		std::string spre(pre);
		player->SetNamePre(spre);
		return 0;
	}
	static int set_name_pro(lua_State* L)
	{
		GET_PLAYER;
		const char* pro = luaL_checkstring(L,2);
		std::string spro(pro);
		player->SetNamePro(spro);
		return 0;
	}
	static int get_team_id(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->TeamID());
		return 1;
	}
	static int get_team_name(lua_State* L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->TeamName());
		return 1;
	}
	static int set_team_info(lua_State* L)
	{
		GET_PLAYER;
		int team_id = luaL_checkint(L,2);
		const char* team_name = luaL_checkstring(L,3);
		player->SetTeamInfo(team_id,team_name);
		return 0;
	}
	static int get_group_size(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GroupSize());
		return 1;
	}
	static int group_exe(lua_State* L)
	{
		GET_PLAYER;
		const char* code = luaL_checkstring(L,2);
		player->GroupExe(code);
		return 0;
	}
	static int group_size_online(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L, player->GroupMemberOnLineNum());
		return 1;
	}
	static int get_guild_size(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GuildSize());
		return 1;
	}
	static int guild_exe(lua_State* L)
	{
		GET_PLAYER;
		const char* code = luaL_checkstring(L,2);
		player->GuildExe(code);
		return 0;
	}
	static int num_friend(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L, player->FriendNum());
		return 1;
	}
	static int get_item_stack(lua_State* L)
	{
		GET_PLAYER;
		int type_id = luaL_checkinteger(L, 2);
		ItemDef* id = player->GetGSL()->GetItemDefineManager()->GetItemDefine(type_id);
		if (id)
		{
			int stackmax = player->GetGSL()->GetItemDefineManager()->GetItemStackMax(id->mTypeID); 
			if (stackmax <= 0){ stackmax = 1; }
			lua_pushinteger(L, stackmax);
			return 1;
		}
		return 0;
	}
	static int change_hp_mp(lua_State* L)
	{
			GET_PLAYER;
			int hp = luaL_checkinteger(L, 2);
			int mp = luaL_checkinteger(L, 3);
			int flag = 0;
			if (lua_isnumber(L, 4))
			{
				flag = luaL_checkinteger(L, 4);
			}
			player->ChangeHpMp(hp,mp,flag);
			return 1;
	}
	static int notify_status_desp(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkinteger(L, 2);
		int begin = 0;
		int end = 0;
		if (lua_isnumber(L, 3))
		{
			begin = luaL_checkinteger(L, 3);
		}
		if (lua_isnumber(L, 4))
		{
			end = luaL_checkinteger(L, 4);
		} 
		player->Session()->GetListener()->notifyStatusDesp(player->Session(), player, id,begin,end);
		return 0;
	}
	static int friend_exe(lua_State* L)
	{
		GET_PLAYER;
		const char* code = luaL_checkstring(L, 2);
		player->FriendExe(code);
		return 0;
	}
	static int is_dead(lua_State* L)
	{
		GET_PLAYER;
		lua_pushboolean(L,player->IsDead());
		return 1;
	}
	static int black_board(lua_State* L)
	{
		GET_PLAYER;
		int flags = luaL_checkinteger(L,2);
		const char* title = luaL_checkstring(L,3);
		const char* msg = luaL_checkstring(L,4);
		player->Session()->GetListener()->notifyBlackBoard(player->Session(),flags,title,msg);
		return 0;
	}
	static int play_effect(lua_State* L)
	{
		GET_PLAYER;
		int effect_type=luaL_checkinteger(L,2);
		int res_id = luaL_checkinteger(L,3);
		player->publicPlayEffect(effect_type,player->X(),player->Y(),player->ID(),player->ID(),res_id);
		return 0;
	}
	static int get_item(lua_State* L)
	{
		GET_PLAYER;
		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);	
		int type_id=luaL_checkinteger(L,2);
		int position = luaL_checkinteger(L,3);
		SubItem* item=player->get_item_by_pos_type(position,type_id);
		if( item )
		{
			This->get_itembind()->bind(L,item);
			return 1;
		}
		return 0;
	}
	static int get_item_by_pos(lua_State* L)
	{
		GET_PLAYER;
		lua_getglobal(L,"server");
		lua_getfield(L,-1,"This");
		OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
		lua_pop(L,2);	
		int position = luaL_checkinteger(L,2);
		SubItem* item=player->get_item_by_pos(position);
		if( item )
		{
			This->get_itembind()->bind(L,item);
			return 1;
		}
		return 0;
	}
	static int get_item_data(lua_State* L)
	{
		GET_PLAYER;
		int type_id=luaL_checkinteger(L,2);
		int position = luaL_checkinteger(L,3);
		SubItem* item=player->get_item_by_pos_type(position,type_id);
		if( item && lua_checkstack(L,40) )
		{
			lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_BIND?1:0);
			lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_USE_BIND?1:0);
			lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_JIPIN?1:0);
			lua_pushinteger(L,item->mDuration);
			lua_pushinteger(L,item->mDuraMax);
			lua_pushinteger(L,item->mAddAC);
			lua_pushinteger(L,item->mAddMAC);
			lua_pushinteger(L,item->mAddDC);
			lua_pushinteger(L,item->mAddMC);
			lua_pushinteger(L,item->mAddSC);
			lua_pushinteger(L,item->mLuck);
			lua_pushinteger(L,item->mLevel);
			lua_pushinteger(L,item->mNumber);
			lua_pushinteger(L,item->mUpdAC);
			lua_pushinteger(L,item->mUpdMAC);
			lua_pushinteger(L,item->mUpdDC);
			lua_pushinteger(L,item->mUpdMC);
			lua_pushinteger(L,item->mUpdSC);
			lua_pushinteger(L,item->mProtect);
			lua_pushinteger(L,item->mAddHp);
			lua_pushinteger(L,item->mAddMp);
			lua_pushinteger(L,item->mUpdMaxCount);
			lua_pushinteger(L,item->mUpdFailedCount);
			lua_pushinteger(L,item->mZLevel);
			ItemDef* idf = player->GetGSL()->GetItemDefineManager()->GetItemDefine(item->mTypeID);
			int funcid =0;std::string funcparams = "";
			if (idf)
			{
				funcid = idf->mFuncId;
				funcparams = idf->mFuncParams;
			}
			lua_pushinteger(L,funcid);
			lua_pushstring(L,funcparams.c_str());
			return 26;
		}
		return 0;
	}
	static int get_item_crtaetime_by_pos(lua_State* L)
	{
		GET_PLAYER;
		int position = luaL_checkinteger(L, 2);
		SubItem* item = player->get_item_by_pos(position);
		if (item )
		{
			lua_pushinteger(L, item->mTypeID);
			lua_pushinteger(L, item->mCreateTime);
			lua_pushinteger(L, item->mLasttime);
			return 3;
		}
		return 0;
	}
	static int get_item_data_by_pos(lua_State* L)
	{
		GET_PLAYER;
		int position = luaL_checkinteger(L,2);
		SubItem* item=player->get_item_by_pos(position);
		if( item && lua_checkstack(L,40) )
		{
			lua_pushinteger(L,item->mTypeID);
			lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_BIND?1:0);
			lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_USE_BIND?1:0);
			lua_pushinteger(L,item->mItemFlags&ITEM_FLAG_JIPIN?1:0);
			lua_pushinteger(L,item->mDuration);
			lua_pushinteger(L,item->mDuraMax);
			lua_pushinteger(L,item->mAddAC);
			lua_pushinteger(L,item->mAddMAC);
			lua_pushinteger(L,item->mAddDC);
			lua_pushinteger(L,item->mAddMC);
			lua_pushinteger(L,item->mAddSC);
			lua_pushinteger(L,item->mLuck);
			lua_pushinteger(L,item->mLevel);
			lua_pushinteger(L,item->mNumber);
			lua_pushinteger(L,item->mUpdAC);
			lua_pushinteger(L,item->mUpdMAC);
			lua_pushinteger(L,item->mUpdDC);
			lua_pushinteger(L,item->mUpdMC);
			lua_pushinteger(L,item->mUpdSC);
			lua_pushinteger(L,item->mProtect);
			lua_pushinteger(L,item->mAddHp);
			lua_pushinteger(L,item->mAddMp);
			ItemDef* id = player->GetGSL()->GetItemDefineManager()->GetItemDefine(item->mTypeID);
			int remain = 0;int funcid = 0; std::string funcparams = "";
			if(id){
				remain = item->RemainTime(id);
				funcid= id->mFuncId;
				funcparams = id->mFuncParams;
			}
			lua_pushinteger(L,remain);
			lua_pushinteger(L,item->mUpdMaxCount);
			lua_pushinteger(L,item->mUpdFailedCount);
			lua_pushinteger(L,item->mZLevel);
			lua_pushinteger(L,funcid);
			lua_pushstring(L,funcparams.c_str());
			return 28;
		}
		return 0;
	}
	static int check_item_fashion(lua_State* L)
	{
		GET_PLAYER;
		int type_id = luaL_checkinteger(L, 2);
		
		if (type_id>0)
		{
			lua_pushinteger(L, player->CheckItemFashion(type_id));
			lua_pushinteger(L, player->CheckItemFashion2(type_id));
			return 2;
		}
		return 0;
	}

	static int get_item_uplv(lua_State* L)
	{
		GET_PLAYER;
		int type_id = luaL_checkinteger(L, 2);
		int position = luaL_checkinteger(L, 3);
		SubItem* item = player->get_item_by_pos_type(position, type_id);
		if (item)
		{
			lua_pushinteger(L, item->mLevel);
			return 1;
		}
		return 0;
	}

	static int set_item_uplv(lua_State* L)
	{
		GET_PLAYER;
		int type_id = luaL_checkinteger(L, 2);
		int position = luaL_checkinteger(L, 3);
		int uplv = luaL_checkinteger(L, 4);
		int ret = player->ItemUpgrade(position, type_id, uplv);
		lua_pushinteger(L,ret);
		return 1;
	}

	static int get_item_base_data(lua_State* L)
	{
		GET_PLAYER;
		int type_id=luaL_checkinteger(L,2);
		ItemDef* id = player->GetGSL()->GetItemDefineManager()->GetItemDefine(type_id);
		if( id && lua_checkstack(L,40) )
		{
			lua_pushstring(L,id->mName.c_str());
			lua_pushinteger(L,id->mAC);
			lua_pushinteger(L,id->mACMax);
			lua_pushinteger(L,id->mMAC);
			lua_pushinteger(L,id->mMACMax);
			lua_pushinteger(L,id->mDC);
			lua_pushinteger(L,id->mDCMax);
			lua_pushinteger(L,id->mMC);
			lua_pushinteger(L,id->mMCMax);
			lua_pushinteger(L,id->mSC);
			lua_pushinteger(L,id->mSCMax);
			lua_pushinteger(L,id->mAccuracy);
			lua_pushinteger(L,id->mDodge);
			lua_pushinteger(L,id->mNeedType);
			lua_pushinteger(L,id->mNeedParam);
			lua_pushinteger(L,id->mPrice);

			lua_pushinteger(L,id->mNeedReinLv);
			lua_pushinteger(L,id->mFuncId);
			lua_pushstring(L,id->mFuncParams.c_str());
			lua_pushinteger(L,id->mEquipType);
			lua_pushinteger(L,id->mJob);

			lua_pushinteger(L,id->mMax_hp);
			lua_pushinteger(L,id->mMax_mp);
			lua_pushinteger(L,id->mHolyDam);
			lua_pushinteger(L,id->mBaoji_prob);
			lua_pushinteger(L,id->mBaoji_pres);
			return 26;
		}
		return 0;
	}
	static int get_item_rand_data(lua_State* L)
	{
		GET_PLAYER;
		int type_id=luaL_checkinteger(L,2);
		ItemDef* id = player->GetGSL()->GetItemDefineManager()->GetItemDefine(type_id);
		if( id && lua_checkstack(L,40) )
		{
			lua_pushinteger(L,id->mRandAC);
			lua_pushinteger(L,id->mRandMAC);
			lua_pushinteger(L,id->mRandDC);
			lua_pushinteger(L,id->mRandMC);
			lua_pushinteger(L,id->mRandSC);
			return 5;
		}
		return 0;
	}
	static int set_item_data(lua_State* L)
	{
		GET_PLAYER;
		int type_id=luaL_checkinteger(L,2);
		int position = luaL_checkinteger(L,3);
		int bind = luaL_checkinteger(L,4);
		int use_bind = luaL_checkinteger(L,5);
		int jiping = luaL_checkinteger(L,6);
		int duration = luaL_checkinteger(L,7);
		int dura_max = luaL_checkinteger(L,8);
		int add_ac = luaL_checkinteger(L,9);
		int add_mac = luaL_checkinteger(L,10);
		int add_dc = luaL_checkinteger(L,11);
		int add_mc = luaL_checkinteger(L,12);
		int add_sc = luaL_checkinteger(L,13);
		int luck = luaL_checkinteger(L,14);
		int upd_level = luaL_checkinteger(L,15);
		int number=luaL_checkinteger(L,16);
		int upd_ac = luaL_checkinteger(L,17);
		int upd_mac = luaL_checkinteger(L,18);
		int upd_dc = luaL_checkinteger(L,19);
		int upd_mc = luaL_checkinteger(L,20);
		int upd_sc = luaL_checkinteger(L,21);
		int protect = 0;
		bool protect_d = false;
		if( lua_isnumber(L,22)  )
		{
			protect=luaL_checkinteger(L,22);
			protect_d = true;
		}
		int add_hp = 0;
		bool add_hp_d = false;
		if( lua_isnumber(L,23) )
		{
			add_hp = luaL_checkinteger(L,23);
			add_hp_d = true;
		}
		int add_mp = 0;
		bool add_mp_d = false;
		if( lua_isnumber(L,24) )
		{
			add_mp = luaL_checkinteger(L,24);
			add_mp_d = true;
		}
		int updmaxcount = 0;
		bool maxcount = false;
		if( lua_isnumber(L,25) )
		{
			updmaxcount = luaL_checkinteger(L,25);
			maxcount = true;
		}
		int updfailedcount = 0;
		bool failedcount = false;
		if( lua_isnumber(L,26) )
		{
			updfailedcount = luaL_checkinteger(L,26);
			failedcount = true;
		}

		SubItem* item=player->get_item_by_pos_type(position,type_id);
		if( item )
		{
			protect = item->mProtect;
			if( lua_isnumber(L,22) )
			{
				protect = int(lua_tonumber(L,22));
			}
			int flag=0;
			if( bind ) flag|=ITEM_FLAG_BIND;
			if( use_bind ) flag|=ITEM_FLAG_USE_BIND;
			if( jiping ) flag|=ITEM_FLAG_JIPIN;
			item->mItemFlags = flag;
			item->mDuration = duration;
			item->mDuraMax = dura_max;
			item->mAddAC = add_ac;
			item->mAddMAC = add_mac;
			item->mAddDC = add_dc;
			item->mAddMC = add_mc;
			item->mAddSC = add_sc;
			item->mLuck = luck;
			item->mLevel = upd_level;
			item->mUpdAC = upd_ac;
			item->mUpdMAC = upd_mac;
			item->mUpdDC = upd_dc;
			item->mUpdMC = upd_mc;
			item->mUpdSC = upd_sc;
			if( protect_d )	item->mProtect = protect;
			if( add_hp_d )item->mAddHp = add_hp;
			if( add_mp_d )item->mAddMp = add_mp;
			if( maxcount )item->mUpdMaxCount = updmaxcount;
			if( failedcount )item->mUpdFailedCount = updfailedcount;
			player->push_item_change_by_pos_type(position,type_id);
		}
		return 0;
	}
	static int fresh_item_info(lua_State* L)
	{
		GET_PLAYER;
		int pos=luaL_checkinteger(L,2);
		int id=luaL_checkinteger(L,3);
		player->push_item_change_by_pos_type(pos,id);
		return 0;
	}
	static int set_item_lasttime(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L, 2);
		int id = luaL_checkinteger(L, 3);
		int last = luaL_checkinteger(L, 4);
		SubItem* item = player->get_item_by_pos_type(pos, id);
		if (item)
		{
			item->mLasttime = last;
			player->Session()->GetListener()->notifyItemChange(player->Session(), *item, 0);
		}
		return 0;
	}
	static int get_item_lasttime(lua_State* L)
	{
		GET_PLAYER;
		int type_id = luaL_checkinteger(L, 2);

		ItemDef* id = player->GetGSL()->GetItemDefineManager()->GetItemDefine(type_id);
		if (id && lua_checkstack(L, 40))
		{
			lua_pushinteger(L, id->mLastTime);
			return 1;
		}
		return 0;
	}
	static int get_item_type_by_pos(lua_State* L)
	{
		GET_PLAYER;
		int pos=luaL_checkinteger(L,2);
		lua_pushinteger(L,player->get_item_type_by_pos(pos));
		return 1;
	}
	static int push_item_info(lua_State* L)
	{
		GET_PLAYER;
		if( lua_isnumber(L,2) )
		{
			int item = static_cast<int>(luaL_checkinteger(L,2));
			player->PushItemInfo(item);
		}
		else
		{
			std::string item_name = luaL_checkstring(L,2);
			player->PushItemInfo(item_name.c_str());
		}
		return 0;
	}
	static int danger_clear_item(lua_State* L)
	{
		GET_PLAYER;
		player->Clear_Item();
		return 0;
	}
	static int danger_drop_item_by_typeid(lua_State* L)
	{
		GET_PLAYER;
		int type_id = luaL_checkinteger(L,2);
		player->DropItemByTypeID(type_id);
		return 0;
	}
	static int drop_item(lua_State* L)
	{
		GET_PLAYER;
		const char* item_name = 0;
		int item = 0;
		ItemDef* id = 0;
		if (lua_isnumber(L, 2))
		{
			item = static_cast<int>(luaL_checkinteger(L, 2));
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item);
		}
		else
		{
			item_name = luaL_checkstring(L, 2);
		}
		if (item <= 0)
		{
			id = player->gsl()->GetItemDefineManager()->GetItemDefine(item_name);
			if (id)
			{
				item = id->mTypeID;
			}
		}
		lua_pushboolean(L, player->DropItemByTypeID(item));
		return 1;
	}
	static int get_ac(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mAC);
		lua_pushinteger(L,player->Attributes()->mACMax);
		return 2;
	}
	static int get_mac(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mMAC);
		lua_pushinteger(L,player->Attributes()->mMACMax);
		return 2;
	}
	static int get_dc(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mDC);
		lua_pushinteger(L,player->Attributes()->mDCMax);
		return 2;
	}
	static int get_mc(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mMC);
		lua_pushinteger(L,player->Attributes()->mMCMax);
		return 2;
	}
	static int get_sc(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mSC);
		lua_pushinteger(L,player->Attributes()->mSCMax);
		return 2;
	}
	static int get_dodge(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mDodge);
		return 1;
	}
	static int get_accuracy(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mAccuracy);
		return 1;
	}
	static int get_hp_max(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mMaxHp);
		return 1;
	}
	static int get_mp_max(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mMaxMp);
		return 1;
	}
	static int get_load_max(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mMaxLoad);
		return 1;
	}
	static int get_brawn_max(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mMaxBrawn);
		return 1;
	}
	static int get_burden_max(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Attributes()->mMaxBurden);
		return 1;
	}
	static int change_job(lua_State* L)
	{
		GET_PLAYER;
		int job=luaL_checkinteger(L,2);
		if( job == 100 || job == 101 || job == 102 )
		{
			int undress_item = 0;
			if( lua_isnumber(L,3) )
			{
				undress_item = luaL_checkinteger(L,3);
			}
			player->ChangeJob(job,undress_item);
		}
		return 0;
	}
	static int change_gender(lua_State* L)
	{
		GET_PLAYER;
		int gender=luaL_checkinteger(L,2);
		if( gender == 200 || gender == 201 )
		{
			int undress_item = 0;
			if( lua_isnumber(L,3) )
			{
				undress_item = luaL_checkinteger(L,3);
			}
			player->ChangeGender(gender,undress_item);
		}
		return 0;
	}
	static int get_gm_level(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GetGMLevel());
		return 1;
	}
	static int get_group_id(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GroupID());
		return 1;
	}
	static int get_group_leader(lua_State* L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->GroupLeader().c_str());
		return 1;
	}
	static int leave_group(lua_State* L)
	{
		GET_PLAYER;
		player->Session()->GetDomainGroup()->LeaveGroup();
		return 0;
	}
	static int gen_slave(lua_State* L)
	{
		GET_PLAYER;
		int id= luaL_checkinteger(L,2);
		int mon_id = luaL_checkinteger(L,3);
		int AiType = luaL_checkinteger(L,4);
		player->gen_slave(id,mon_id,AiType);
		return 0;
	}
	static int kill_slave(lua_State* L)
	{
		GET_PLAYER;
		int id= luaL_checkinteger(L,2);
		player->kill_slave(id);
		return 0;
	}
	static int clear_slave(lua_State* L)
	{
		GET_PLAYER;
		player->clear_slave();
		return 0;
	}
	static int have_slave(lua_State* L)
	{
		GET_PLAYER;
		int id=luaL_checkinteger(L,2);
		lua_pushinteger(L,player->have_slave(id));
		return 1;
	}

	static int get_slave(lua_State * L)
	{
		GET_PLAYER;
		int id=luaL_checkinteger(L,2);
		PlaySlave * pSlave = player->GetSlave(id);
		if (pSlave == NULL)
		{
			return 0;
		}
		else
		{
			lua_getglobal(L,"server");
			lua_getfield(L,-1,"This");
			OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
			lua_pop(L,2);
			This->get_slavebind()->bind(L,pSlave);
			return 1;
		}
	}
	static int get_slave_by_id(lua_State * L)
	{
		GET_PLAYER;
		int id=luaL_checkinteger(L,2);
		PlaySlave * pSlave = player->GetSlaveByID(id);
		if (pSlave == NULL)
		{
			return 0;
		}
		else
		{
			lua_getglobal(L,"server");
			lua_getfield(L,-1,"This");
			OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
			lua_pop(L,2);
			This->get_slavebind()->bind(L,pSlave);
			return 1;
		}
	}

	static int gen_slave_fashi(lua_State* L)
	{
		GET_PLAYER;
		int mon_id = luaL_checkinteger(L,2);
		int x = 0;
		if( lua_isnumber(L,3) )
		{
			x = luaL_checkinteger(L,3);
		}
		int y = 0;
		if( lua_isnumber(L,4) )
		{
			y = luaL_checkinteger(L,4);
		}
		const char* flags = "";
		if( lua_isstring(L,5) )
		{
			flags = luaL_checkstring(L,5);
		}
		lua_pushinteger(L,player->gen_slave_fashi(mon_id, x, y, flags));
		return 1;
	}

	static int gen_slave_statue(lua_State* L)
	{
		GET_PLAYER;
		int mon_id = luaL_checkinteger(L, 2);
		int x = luaL_checkinteger(L, 3);
		int y = luaL_checkinteger(L, 4);
		const char* flags = "";
		if( lua_isstring(L,5) )
		{
			flags = luaL_checkstring(L,5);
		}

		lua_pushinteger(L, player->GenSlaveStatue(mon_id, x, y, flags));
		return 1;
	}
	static int kill_slave_fashi(lua_State * L)
	{
		GET_PLAYER;
		int id = luaL_checkinteger(L,2);
		player->kill_slave_fashi(id);
		return 0;
	}

	static int clear_slave_fashi(lua_State* L)
	{
		GET_PLAYER;
		player->clear_slave_fashi();
		return 0;
	}

	static int add_slave_skill(lua_State* L)
	{
		GET_PLAYER;
		int type=luaL_checkinteger(L,2);

		PlaySlave * pSalve = player->GetSlave(type);
		if (pSalve == NULL)
			return 0;

		int skill = static_cast<int>(luaL_checkinteger(L,3));
		int lv = 1;
		if( lua_isnumber(L,4) )
		{
			lv = luaL_checkinteger(L,4);
		}
		pSalve->AddSkill(skill,lv);
		return 0;
	}
	static int clear_mofadun(lua_State* L)
	{
		GET_PLAYER;
		player->ClearMoFaDun();
		return 0;
	}
	static int achieve_get_state(lua_State* L)
	{
		GET_PLAYER;
		int aid = luaL_checkinteger(L,2);
		lua_pushinteger(L,player->GetAchieveState(aid) );
		return 1;
	}
	static int achieve_set_state(lua_State* L)
	{
		GET_PLAYER;
		int aid = luaL_checkinteger(L,2);
		int st  = luaL_checkinteger(L,3);
		lua_pushinteger(L,player->SetAchieveState(aid,st) );
		return 1;
	}
	static int achieve_addone_state(lua_State* L)
	{
		GET_PLAYER;
		int aid = luaL_checkinteger(L,2);
		lua_pushinteger(L,player->AddOneAchieveState(aid) );
		return 1;
	}
	static int achieve_get_param(lua_State* L)
	{
		GET_PLAYER;
		int aid = luaL_checkinteger(L,2);
		lua_pushinteger(L,player->GetAchieveParam(aid) );
		return 1;
	}
	static int achieve_set_param(lua_State* L)
	{
		GET_PLAYER;
		int aid = luaL_checkinteger(L,2);
		int st  = luaL_checkinteger(L,3);
		lua_pushinteger(L,player->SetAchieveParam(aid,st) );
		return 1;
	}
	static int achieve_addone_param(lua_State* L)
	{
		GET_PLAYER;
		int aid = luaL_checkinteger(L,2);
		lua_pushinteger(L,player->AddOneAchieveParam(aid) );
		return 1;
	}
	static int achieve_get_point(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GetAchievePoint());
		return 1;
	}
	static int achieve_add_point(lua_State* L)
	{
		GET_PLAYER;
		int ap = luaL_checkinteger(L,2);
		lua_pushinteger(L,player->AddAchievePoint(ap));
		return 1;
	}
	static int get_safearea_id(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->SafeArea());
		return 1;
	}

	static int vcoin_used_by_day(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->VcoinUsedByDay());
		return 1;
	}
	static int vcoin_used_by_month(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->VcoinUsedByMonth());
		return 1;
	}
	static int vcoin_accu_by_month(lua_State* L)
	{
		GET_PLAYER;
		lua_pushnumber(L,player->VcoinAccuByMonth());
		return 1;
	}

	static int gen_dart(lua_State* L)
	{
		GET_PLAYER;
		int dart = luaL_checkinteger(L,2);
		player->GenDart(dart);
		return 0;
	}

	static int kill_dart(lua_State* L)
	{
		GET_PLAYER;
		player->KillDart();
		return 0;
	}

	static int have_dart(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->HaveDart());
		return 1;
	}

	static int bind_dart(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkinteger(L,2);
		lua_pushboolean(L,player->BindDart(id));
		return 1;
	}

	static int get_dart_id(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GetDartID());
		return 1;
	}

	static int directfly_to_dart(lua_State* L)
	{
		GET_PLAYER;
		if(player->mDart2 && player->mDart2->mLiving)
		{
			player->EnterMap(player->mDart2->GetMap()->GetID(),player->mDart2->X(),player->mDart2->Y());
		}
		return 0;
	}

	static int set_dart_life_time(lua_State* L)
	{
		GET_PLAYER;
		if(player->mDart2)
		{
			player->mDart2->m_nLifeTime = luaL_checkinteger(L,2);
		}
		return 0;
	}
	static int get_dart_life_time(lua_State* L)
	{
		GET_PLAYER;
		if(player->mDart2 && player->mDart2->mLiving)
		{
			int nTime = time(0) - player->mDart2->mDartTaskStartTick;
			int left = player->mDart2->m_nLifeTime - nTime; 
			if (left > 0)
				lua_pushnumber(L,left);
		}
		else
		{
			lua_pushnumber(L,0);
		}
		return 1;
	}
	static int get_dart_hp(lua_State* L)
	{
		GET_PLAYER;
		if (player->mDart2 )
		{
			lua_pushnumber(L,player->mDart2->Hp());
			lua_pushnumber(L,player->mDart2->MaxHp());

			return 2;
		}
		return 0;
	}

	static int get_dart_map(lua_State* L)
	{
		GET_PLAYER;
		if (player->mDart2 )
		{
			lua_getglobal(L,"server");
			lua_getfield(L,-1,"This");
			OGSLib::ScriptLua* This = *(OGSLib::ScriptLua**)lua_topointer(L,-1);
			lua_pop(L,2);	
			PlayMap* map = player->mDart2->GetMap();
			if( map )
			{
				This->get_mapbind()->bind(L,map);
				return 1;
			}
		}
		return 0;
	}
	static int get_dart_pos(lua_State* L)
	{
		GET_PLAYER;
		if (player->mDart2 )
		{
			lua_pushnumber(L,player->mDart2->X());
			lua_pushnumber(L,player->mDart2->Y());

			return 2;
		}
		return 0;
	}

	static int get_dart_follow_pos(lua_State* L)
	{
		GET_PLAYER;
		if (player->mDart2 )
		{
			int x = player->mDart2->X();
			int y = player->mDart2->Y();
			int dir = player->mDart2->Dir();
			dir = player->NextDir(dir, 4);
			x = player->NextX(x, dir);
			y = player->NextY(y, dir);
			lua_pushnumber(L,x);
			lua_pushnumber(L,y);

			return 2;
		}
		return 0;
	}
	
	static int get_dart_reach_stop(lua_State* L)
	{
		GET_PLAYER;
		if (player->mDart2 )
		{
			lua_pushnumber(L,player->mDart2->StopPosReached() ? 1 : 0);
			return 1;
		}
		return 0;
	}
	static int push_skill_def(lua_State* L)
	{
		GET_PLAYER;
		int skill_id=luaL_checkinteger(L,2);
		int level=luaL_checkinteger(L,3);
		if (skill_id && level)
		{
			player->PushSkillDefine(skill_id, level);
		}
		return 0;
	}

	static int set_fight_point(lua_State* L)
	{
		GET_PLAYER;
		int fight = luaL_checkinteger(L,2);
		player->FightPoint(fight);
 		return 0;
	}

	static int get_empty_number_in_lotterydepot(lua_State* L)
	{
		GET_PLAYER;
		int num = 0;
		num = player->get_empty_item_position_number_in_lotterydepot();
		lua_pushnumber(L,num);
		return 1;
	}
	static int add_drop_prob(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->AddDropProb(num);
		return 0;
	}
	static int sub_drop_prob(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->SubDropProb(num);
		return 0;
	}
	static int add_double_att_prob(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->AddDoubleAttProb(num);
		return 0;
	}
	static int sub_double_att_prob(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->SubDoubleAttProb(num);
		return 0;
	}
	static int get_drop_prob(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GetDropProb());
		return 1;
	}
	static int get_double_att_prob(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GetDoubleAttProb());
		return 1;
	}
	static int set_total_updlevel(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->TotalUpdLevel(num);
		return 0;
	}
	static int get_total_updlevel(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->TotalUpdLevel());
		return 1;
	}
	static int get_puppet_mul(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->mPuppetMul);
		return 1;
	}
	static int set_puppet_mul(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->mPuppetMul = num;
		return 0;
	}

	static int update_chart_list(lua_State* L)
	{
		GET_PLAYER;
		int chart_type = luaL_checkinteger(L,2);
		int chart_param = luaL_checkinteger(L,3);
		player->UpdateChartData(chart_type,chart_param);
		return 0;
	}

	static int set_mount_show(lua_State* L)
	{
		GET_PLAYER;
		int m = luaL_checkinteger(L,2);
		if( m==0||m==1)
		{
			player->set_showmount(m);
		}
		return 0;
	}
	static int get_mount_show(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->show_mount());
		return 1;
	}
	static int set_mount_show_lock(lua_State* L)
	{
		GET_PLAYER;
		int m = luaL_checkinteger(L,2);
		if( m==0||m==1)
		{
			player->mount_lock_skill(m);
		}
		return 0;
	}
	static int get_mount_show_lock(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->mount_lock_skill());
		return 1;
	}
	static int set_avatar(lua_State* L)
	{
		GET_PLAYER;
		int lock = luaL_checkinteger(L,2);
		int cloth = luaL_checkinteger(L,3);
		int weapon = luaL_checkinteger(L,4);
		player->ChangeCloth(lock,cloth,weapon);
		return 0;
	}
	static int get_avatar(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->lockshow());
		lua_pushinteger(L,player->Cloth());
		lua_pushinteger(L,player->Weapon());
		return 3;
	}
	static int doaction(lua_State* L)
	{
		GET_PLAYER;
		int action = luaL_checkinteger(L,2);
		const char* callback = luaL_checkstring(L,3);
		player->lock_action(action,callback);
		return 0;
	}
	static int item_sell(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L,2);
		int type_id = 0;
		if( lua_isnumber(L,3) )
		{
			type_id = luaL_checkinteger(L,3);
		}
		player->ItemSell(pos,type_id);
		return 0;
	}
	static int sort_depot(lua_State* L)
	{
		GET_PLAYER;
		int flags = luaL_checkinteger(L,2);
		player->SortItem(flags);
		return 0;
	}
	static int add_depot_slot(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->addDepotFromLua(num);
		return 0;
	}
	static int add_bag_slot(lua_State* L)
	{
		GET_PLAYER;
		int num = luaL_checkinteger(L,2);
		player->AddBagFromLua(num);
		return 0;
	}
	static int get_id(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->ID());
		return 1;
	}
	static int get_fightpoint(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->FightPoint());
		return 1;
	}
	static int start_progress_bar(lua_State* L)
	{
		GET_PLAYER;
		int duration = luaL_checkinteger(L,2);
		std::string msg = luaL_checkstring(L,3);
		player->NotifyShowProgressBar(duration,msg.c_str());
		return 0;
	}
	static int repair_all(lua_State* L)
	{
		GET_PLAYER;
		int flag = luaL_checkinteger(L,2);
		lua_pushinteger(L,player->ReqairAllItem(flag));
		return 1;
	}
	static int load_fabao_res(lua_State* L)
	{
		GET_PLAYER;
		int jie = luaL_checkinteger(L,2);
		player->SetFaBaoRes(jie);
		return 0;
	}
	static int change_fabao_res(lua_State* L)
	{
		GET_PLAYER;
		int jie = luaL_checkinteger(L,2);
		player->ChangeFaBaoRes(jie);
		return 0;
	}
	static int set_zslevel(lua_State* L)
	{
		GET_PLAYER;
		int zslevel = luaL_checkinteger(L,2);
		lua_pushnumber(L,player->SetZsLevel(zslevel));
		return 0;
	}
	static int push_lua_table(lua_State* L)
	{
		GET_PLAYER;
		std::string type = luaL_checkstring(L,2);
		std::string table = luaL_checkstring(L,3);

		unsigned long len = table.length();
		static char temp[1024*12];
		unsigned long osize;
		if( len>128 )
		{
			compress((unsigned char*)(temp),&osize,(unsigned char*)table.c_str(),len);
		}
		else
		{
			osize = 0;
		}
		if( osize>0 && osize<1024*12 && osize<len )
		{
			temp[osize]=0;
			std::string data = std::string(temp,osize);
			player->NotifyPushLuaTable(type,osize,data);
			return 0;
		}
		player->NotifyPushLuaTable(type,0,table);
		return 0;
	}
	static int set_attr_mult(lua_State* L)
	{
		GET_PLAYER;
		int attrid = luaL_checkinteger(L,2);
		int value = luaL_checkinteger(L,3);
		switch(attrid)
		{
		case 1:  player->PropPercent()->mMaxMp=value;break;
		case 2:  player->PropPercent()->mMaxHp=value;break;		
		case 3:  player->PropPercent()->mAC=value;break;
		case 4:  player->PropPercent()->mACMax=value;break;
		case 5:  player->PropPercent()->mMAC=value;break;
		case 6:  player->PropPercent()->mMACMax=value;break;
		case 7:  player->PropPercent()->mDC=value;break;
		case 8:  player->PropPercent()->mDCMax=value;break;
		case 9:  player->PropPercent()->mMC=value;break;
		case 10: player->PropPercent()->mMCMax=value;break;
		case 11: player->PropPercent()->mSC=value;break;
		case 12: player->PropPercent()->mSCMax=value;break;
		case 13: player->PropPercent()->mAccuracy=value;break;
		case 14: player->PropPercent()->mDodge=value;break;
		case 15: player->PropPercent()->mLuck=value;break;
		}
		return 0;
	}
	static int get_attr_mult(lua_State* L)
	{
		GET_PLAYER;
		int attrid = luaL_checkinteger(L,2);
		switch(attrid)
		{
		case 1:  lua_pushinteger(L,player->PropPercent()->mMaxMp);return 1;break;
		case 2:  lua_pushinteger(L,player->PropPercent()->mMaxHp);return 1;break;		
		case 3:  lua_pushinteger(L,player->PropPercent()->mAC);return 1;break;
		case 4:  lua_pushinteger(L,player->PropPercent()->mACMax);return 1;break;
		case 5:  lua_pushinteger(L,player->PropPercent()->mMAC);return 1;break;
		case 6:  lua_pushinteger(L,player->PropPercent()->mMACMax);return 1;break;
		case 7:  lua_pushinteger(L,player->PropPercent()->mDC);return 1;break;
		case 8:  lua_pushinteger(L,player->PropPercent()->mDCMax);return 1;break;
		case 9:  lua_pushinteger(L,player->PropPercent()->mMC);return 1;break;
		case 10: lua_pushinteger(L,player->PropPercent()->mMCMax);return 1;break;
		case 11: lua_pushinteger(L,player->PropPercent()->mSC);return 1;break;
		case 12: lua_pushinteger(L,player->PropPercent()->mSCMax);return 1;break;
		case 13: lua_pushinteger(L,player->PropPercent()->mAccuracy);return 1;break;
		case 14: lua_pushinteger(L,player->PropPercent()->mDodge);return 1;break;
		case 15: lua_pushinteger(L,player->PropPercent()->mLuck);return 1;break;
		}
		return 0;
	}

	static int set_script_attr(lua_State* L)
	{
		GET_PLAYER;
		int attrid = luaL_checkinteger(L,2);
		int value = luaL_checkinteger(L,3);
		switch(attrid)
		{
		case 1:  player->SAP()->mMaxMp=value;break;
		case 2:  player->SAP()->mMaxHp=value;break;		
		case 3:  player->SAP()->mAC=value;break;
		case 4:  player->SAP()->mACMax=value;break;
		case 5:  player->SAP()->mMAC=value;break;
		case 6:  player->SAP()->mMACMax=value;break;
		case 7:  player->SAP()->mDC=value;break;
		case 8:  player->SAP()->mDCMax=value;break;
		case 9:  player->SAP()->mMC=value;break;
		case 10: player->SAP()->mMCMax=value;break;
		case 11: player->SAP()->mSC=value;break;
		case 12: player->SAP()->mSCMax=value;break;
		case 13: player->SAP()->mAccuracy=value;break;
		case 14: player->SAP()->mDodge=value;break;
		case 15: player->SAP()->mLuck=value;break;
		case 16: player->SAP()->mAntiMagic=value;break;
		case 17: player->SAP()->mAntiPoison=value;break;
		case 18: player->SAP()->mHpRecover=value;break;
		case 19: player->SAP()->mMpRecover=value;break;
		case 20: player->SAP()->mPoisonRecover=value;break;
		case 21: player->SAP()->mMabi_prob=value;
				 {
					 if(player->SAP()->mMabi_prob <= 0)
					 {
						 player->SAP()->mMabi_prob =value;
					 }
					 else
					 {
						 player->SAP()->mMabi_prob =max(player->SAP()->mMabi_prob,value);
					 }
				 }
				 break;
		case 22: player->SAP()->mMabi_dura=value;
				 {
					 if(player->SAP()->mMabi_dura <= 0)
					 {
						 player->SAP()->mMabi_dura =value;
					 }
					 else
					 {
						 player->SAP()->mMabi_dura =max(player->SAP()->mMabi_dura,value);
					 }
				 }
				 break;
		case 23: player->SAP()->mDixiao_pres=value;break;
		case 24: 
			{
				if(player->SAP()->mFuyuan_cd <= 0)
				{
					player->SAP()->mFuyuan_cd =value;
				}
				else
				{
					player->SAP()->mFuyuan_cd =min(player->SAP()->mFuyuan_cd,value);
				}
			}
			break;
		case 25: player->SAP()->mDropProb=value;break;
		case 26: player->SAP()->mDoubleAttProb=value;break;
		case 27: player->SAP()->mXishouProb=value;break;
		case 28: player->SAP()->mXishouPres=value;break;
		case 29: player->SAP()->mFantanProb=value;break;
		case 30: player->SAP()->mFantanPres=value;break;
		case 31: player->SAP()->mBaojiProb=value;break;
		case 32: player->SAP()->mBaojiPres=value;break;
		case 33: player->SAP()->mXixueProb=value;break;
		case 34: player->SAP()->mXixuePres=value;break;
		case 35: player->SAP()->mFuyuan_pres=value;break;
		case 36: player->SAP()->mHpPres=value;break;
		case 37: player->SAP()->mMpPres=value;break;
		case 38: player->SAP()->mFight=value;break;
		case 39: player->SAP()->mPKIncrement=value;break;
		case 40: player->SAP()->mPKReduce=value;break;
		case 41: player->SAP()->mHolyDam=value;break;
		case 42: player->SAP()->mDamIncrement=value;break;
		case 43: player->SAP()->mDamReduce=value;break;
		case 44: player->SAP()->mTenacity=value;break;
		}
		return 0;
	}
	static int get_script_attr(lua_State* L)
	{
		GET_PLAYER;
		int attrid = luaL_checkinteger(L,2);
		switch(attrid)
		{
		case 1:  lua_pushinteger(L,player->SAP()->mMaxMp);return 1;break;
		case 2:  lua_pushinteger(L,player->SAP()->mMaxHp);return 1;break;		
		case 3:  lua_pushinteger(L,player->SAP()->mAC);return 1;break;
		case 4:  lua_pushinteger(L,player->SAP()->mACMax);return 1;break;
		case 5:  lua_pushinteger(L,player->SAP()->mMAC);return 1;break;
		case 6:  lua_pushinteger(L,player->SAP()->mMACMax);return 1;break;
		case 7:  lua_pushinteger(L,player->SAP()->mDC);return 1;break;
		case 8:  lua_pushinteger(L,player->SAP()->mDCMax);return 1;break;
		case 9:  lua_pushinteger(L,player->SAP()->mMC);return 1;break;
		case 10: lua_pushinteger(L,player->SAP()->mMCMax);return 1;break;
		case 11: lua_pushinteger(L,player->SAP()->mSC);return 1;break;
		case 12: lua_pushinteger(L,player->SAP()->mSCMax);return 1;break;
		case 13: lua_pushinteger(L,player->SAP()->mAccuracy);return 1;break;
		case 14: lua_pushinteger(L,player->SAP()->mDodge);return 1;break;
		case 15: lua_pushinteger(L,player->SAP()->mLuck);return 1;break;
		case 16: lua_pushinteger(L,player->SAP()->mAntiMagic);return 1;break;
		case 17: lua_pushinteger(L,player->SAP()->mAntiPoison);return 1;break;
		case 18: lua_pushinteger(L,player->SAP()->mHpRecover);return 1;break;
		case 19: lua_pushinteger(L,player->SAP()->mMpRecover);return 1;break;
		case 20: lua_pushinteger(L,player->SAP()->mPoisonRecover);return 1;break;
		case 21: lua_pushinteger(L,player->SAP()->mMabi_prob);return 1;break;
		case 22: lua_pushinteger(L,player->SAP()->mMabi_dura);return 1;break;
		case 23: lua_pushinteger(L,player->SAP()->mDixiao_pres);return 1;break;
		case 24: lua_pushinteger(L,player->SAP()->mFuyuan_cd);return 1;break;
		case 25: lua_pushinteger(L,player->SAP()->mDropProb);return 1;break;
		case 26: lua_pushinteger(L,player->SAP()->mDoubleAttProb);return 1;break;
		case 27: lua_pushinteger(L,player->SAP()->mXishouProb);return 1;break;
		case 28: lua_pushinteger(L,player->SAP()->mXishouPres);return 1;break;
		case 29: lua_pushinteger(L,player->SAP()->mFantanProb);return 1;break;
		case 30: lua_pushinteger(L,player->SAP()->mFantanPres);return 1;break;
		case 31: lua_pushinteger(L,player->SAP()->mBaojiProb);return 1;break;
		case 32: lua_pushinteger(L,player->SAP()->mBaojiPres);return 1;break;
		case 33: lua_pushinteger(L,player->SAP()->mXixueProb);return 1;break;
		case 34: lua_pushinteger(L,player->SAP()->mXixuePres);return 1;break;
		case 35:lua_pushinteger(L,player->SAP()->mFuyuan_pres);return 1;break;
		case 36: lua_pushinteger(L,player->SAP()->mHpPres);return 1;break;
		case 37: lua_pushinteger(L,player->SAP()->mMpPres);return 1;break;
		case 38: lua_pushinteger(L,player->SAP()->mFight);return 1;break;
		case 39: lua_pushinteger(L,player->SAP()->mPKIncrement);return 1;break;
		case 40: lua_pushinteger(L,player->SAP()->mPKReduce);return 1;break;
		case 41: lua_pushinteger(L,player->SAP()->mHolyDam);return 1;break;
		case 42: lua_pushinteger(L,player->SAP()->mDamIncrement);return 1;break;
		case 43: lua_pushinteger(L,player->SAP()->mDamReduce);return 1;break;
		case 44: lua_pushinteger(L,player->SAP()->mTenacity);return 1;break;
		}
		return 0;
	}
	static int recal_attr(lua_State* L)
	{
		GET_PLAYER;
		player->CalculateAttribute();
		player->NotifyAttributeChange();
		player->NotifyHPMPChange(player,0,0);
		return 0;
	}

	static int set_script_item_attr(lua_State* L)
	{
		GET_PLAYER;
		int posid = luaL_checkinteger(L,2);
		ScriptItemAttibuteParam siap;
		siap.mMaxHpMul=static_cast<float>(luaL_checknumber(L,3));
		siap.mMaxHpAdd=static_cast<float>(luaL_checknumber(L,4));

		siap.mMaxMpMul=static_cast<float>(luaL_checknumber(L,5));
		siap.mMaxMpAdd=static_cast<float>(luaL_checknumber(L,6));

		siap.mACMul=static_cast<float>(luaL_checknumber(L,7));
		siap.mACAdd=static_cast<float>(luaL_checknumber(L,8));

		siap.mACMaxMul=static_cast<float>(luaL_checknumber(L,9));
		siap.mACMaxAdd=static_cast<float>(luaL_checknumber(L,10));

		siap.mMACMul=static_cast<float>(luaL_checknumber(L,11));
		siap.mMACAdd=static_cast<float>(luaL_checknumber(L,12));

		siap.mMACMaxMul=static_cast<float>(luaL_checknumber(L,13));
		siap.mMACMaxAdd=static_cast<float>(luaL_checknumber(L,14));

		siap.mDCMul=static_cast<float>(luaL_checknumber(L,15));
		siap.mDCAdd=static_cast<float>(luaL_checknumber(L,16));

		siap.mDCMaxMul=static_cast<float>(luaL_checknumber(L,17));
		siap.mDCMaxAdd=static_cast<float>(luaL_checknumber(L,18));

		siap.mMCMul=static_cast<float>(luaL_checknumber(L,19));
		siap.mMCAdd=static_cast<float>(luaL_checknumber(L,20));

		siap.mMCMaxMul=static_cast<float>(luaL_checknumber(L,21));
		siap.mMCMaxAdd=static_cast<float>(luaL_checknumber(L,22));

		siap.mSCMul=static_cast<float>(luaL_checknumber(L,23));
		siap.mSCAdd=static_cast<float>(luaL_checknumber(L,24));

		siap.mSCMaxMul=static_cast<float>(luaL_checknumber(L,25));
		siap.mSCMaxAdd=static_cast<float>(luaL_checknumber(L,26));

		siap.mFight=static_cast<float>(luaL_checknumber(L,27));
		player->set_siap(posid,siap);
		return 0;
	}
	static int num_xjbag_black(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->NumXjBlack());
		return 1;
	}
	static int setHpMpPercent(lua_State* L)
	{
		GET_PLAYER;
		int percent = luaL_checkinteger(L,2);
		player->setHpMpPercent(percent);
		return 0;
	}

	static int post_mail(lua_State* L)
	{
		GET_PLAYER;
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
		player->PostMail(title,content,items);
		return 0;
	}

	static int item_upgrade(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L,2);
		int updac = luaL_checkinteger(L,3);
		int updmac = luaL_checkinteger(L,4);
		int upddc = luaL_checkinteger(L,5); 
		int updmc = luaL_checkinteger(L,6); 
		int updsc = luaL_checkinteger(L,7); 

		int ret=player->ItemUpgrade(pos,updac,updmac,upddc,updmc,updsc);
		lua_pushinteger(L,ret);

		return 0;
	}
	static int item_zhuling(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L,2);
		int flag = luaL_checkinteger(L,3);

		int ret=player->ItemZhuLing(pos,flag);
		lua_pushinteger(L,ret);

		return 0;
	}
	static int item_add_upd_failed(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L,2);
		int count = luaL_checkinteger(L,3);

		int ret=player->ItemAddUpdFailedCount(pos,count);
		lua_pushinteger(L,ret);

		return 0;
	}
	static int item_sub_upd_failed(lua_State* L)
	{
		GET_PLAYER;
		int pos = luaL_checkinteger(L,2);
		int count = luaL_checkinteger(L,3);

		int ret=player->ItemSubUpdFailedCount(pos,count);
		lua_pushinteger(L,ret);

		return 0;
	}
	static int get_viplevel(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->viplevel());
		return 1;
	}
	static int list_chargedart(lua_State* L)
	{
		GET_PLAYER;
		player->NotifyChargeDartList();
		return 0;
	}
	static int add_hp(lua_State* L)
	{
		GET_PLAYER;
		int _hp = luaL_checkinteger(L,2);
		player->AddHP(_hp);
		return 0;
	}
	static int add_mp(lua_State* L)
	{
		GET_PLAYER;
		int _mp = luaL_checkinteger(L,2);
		player->AddMP(_mp);
		return 0;
	}
	static int notify_hpmpchange(lua_State* L)
	{
		GET_PLAYER;
		int hp_change = 0;
		int mp_change =0;
		if( lua_isnumber(L,2) )
		{
			hp_change = luaL_checkinteger(L,2);
		}
		if( lua_isnumber(L,3) )
		{
			mp_change = luaL_checkinteger(L,3);
		}
		player->HpMpChangeBroadcast(hp_change, mp_change);
		return 0;
	}
	static int notify_mini_npc(lua_State* L)
	{
		GET_PLAYER;
		player->NotifyMiniNpc();
		return 0;
	}
	static int set_sword_level(lua_State*L)
	{
		GET_PLAYER;
		int zslevel = luaL_checkint(L,2);
		lua_pushnumber(L,player->SetSwordLv(zslevel));
		return 1;
	}
	static int set_monpoint_level(lua_State*L)
	{
		GET_PLAYER;
		int lv = luaL_checkint(L,2);
		lua_pushnumber(L,player->SetMonPointLv(lv));
		return 1;
	}
	static int Become_Monster(lua_State* L)
	{
		GET_PLAYER;
		int mon_show = luaL_checkinteger(L,2);////确认显示怪物的外观
		std::string mon_name = luaL_checkstring(L,3);///显示那种怪物的外观
		player->BecomeMonster(mon_show,mon_name);
		return 0;
	}

	static int get_huweidef_data(lua_State * L)
	{
		GET_PLAYER;
		int mon_id = luaL_checkint(L,2);
		//int lv = luaL_checkint(L, 3);
		/*char monname[256];
		if (lv > 0)
		{
			sprintf_s(monname,"%s%d",mon_name.c_str(), lv);
			mon_name = monname;
		}*/

		MonDef* md = player->GetGSL()->GetMonDefManager()->FindMonDef(mon_id);
		if (md)
		{
			lua_pushinteger(L, md->exp);
			lua_pushinteger(L, md->hp);
			lua_pushinteger(L, md->dc);
			lua_pushinteger(L, md->dcMax);
			lua_pushinteger(L, md->ac);
			lua_pushinteger(L, md->acMax);
			// mc恢复
			lua_pushinteger(L, md->mc);
			// Max增伤
			lua_pushinteger(L, md->mcMax);

			return 8;
		}

		return 0;
	}

	static int get_monpoint_data(lua_State* L)
	{
		GET_PLAYER;
		int type = luaL_checkint(L, 2);
		int lv = luaL_checkint(L, 3);
		bool result = false;
		int index = 1;
		int dc_min = 0;
		int dc_max = 0;
		int mc_min = 0;
		int mc_max = 0;
		int sc_min = 0;
		int sc_max = 0;
		int ac_min = 0;
		int ac_max = 0;
		int mac_min = 0;
		int mac_max = 0;
		int hp_max = 0;
		int mp_max = 0;
		int jifen = 0;
		if (type == 10)
		{
			if (lv >= 1 && lv <= 96)
			{
				while (index <= lv)
				{
					MonPointDef* sd = player->GetGSL()->GetMonPointDefManager()->GetMonPointDefine(100000+index);
					if (sd){
						dc_min += sd->dc_min;
						dc_max += sd->dc_max;
						mc_min += sd->mc_min;
						mc_max += sd->mc_max;
						sc_min += sd->sc_min;
						sc_max += sd->sc_max;
						ac_min += sd->ac_min;
						ac_max += sd->ac_max;
						mac_min += sd->mac_min;
						mac_max += sd->mac_max;
						hp_max += sd->max_hp;
						mp_max += sd->max_mp;
						if (index == lv)
						{
							jifen = sd->jifen;
						}
					}
					index++;
				}
				result = true;
			}
		}
		else if (type >= 230 && type <= 320)
		{
			if (lv >= 1 && lv <= 100)
			{
				while (index <= lv)
				{
					MonPointDef* sd = player->GetGSL()->GetMonPointDefManager()->GetMonPointDefine(type*1000 + index);
					if (sd){
						dc_min += sd->dc_min;
						dc_max += sd->dc_max;
						mc_min += sd->mc_min;
						mc_max += sd->mc_max;
						sc_min += sd->sc_min;
						sc_max += sd->sc_max;
						ac_min += sd->ac_min;
						ac_max += sd->ac_max;
						mac_min += sd->mac_min;
						mac_max += sd->mac_max;
						hp_max += sd->max_hp;
					}
					index++;
				}
				MonPointDef* sd = player->GetGSL()->GetMonPointDefManager()->GetMonPointDefine(type * 1000 + lv);
				if (sd){
					///获取需求1
					mp_max = sd->max_mp;
					///获取需求2
					jifen = sd->jifen;
				}
				result = true;
			}
		}
		else
		{
			MonPointDef* sd = player->GetGSL()->GetMonPointDefManager()->GetMonPointDefine(type*1000 + lv);
			if (sd){
				dc_min = sd->dc_min;
				dc_max = sd->dc_max;
				mc_min = sd->mc_min;
				mc_max = sd->mc_max;
				sc_min = sd->sc_min;
				sc_max = sd->sc_max;
				ac_min = sd->ac_min;
				ac_max = sd->ac_max;
				mac_min = sd->mac_min;
				mac_max = sd->mac_max;
				hp_max = sd->max_hp;
				mp_max = sd->max_mp;
				jifen = sd->jifen;
				result = true;
			}
		}
		if (result)
		{
			lua_pushinteger(L, dc_min);
			lua_pushinteger(L, dc_max);
			lua_pushinteger(L, mc_min);
			lua_pushinteger(L, mc_max);
			lua_pushinteger(L, sc_min);
			lua_pushinteger(L, sc_max);
			lua_pushinteger(L, ac_min);
			lua_pushinteger(L, ac_max);
			lua_pushinteger(L, mac_min);
			lua_pushinteger(L, mac_max);
			lua_pushinteger(L, hp_max);
			lua_pushinteger(L, mp_max);
			lua_pushinteger(L, jifen);
			return 13;
		}
		return 0;
	}
	static int get_sword_data(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkint(L,2);

		return 0;
	}
	static int get_level_exp(lua_State* L)
	{
		GET_PLAYER;
		int level = luaL_checkinteger(L,2);
		LEUD_INT64 exp = player->GetGSL()->GetLevelInfoManager()->GetNextExp(level);
		lua_pushnumber(L,exp);
		return 1;
	}
	
	static int set_wing_lv(lua_State*L)
	{
		GET_PLAYER;
		bool updateChart = true;
		int winglevel = luaL_checkint(L,2);
		int noUpdateChart = 0;
		if( lua_isnumber(L,3))
		{
			noUpdateChart = luaL_checkint(L,3);
		}
		if (noUpdateChart && noUpdateChart == 1)
		{
			updateChart = false;
		}
		lua_pushnumber(L,player->SetWingLv(winglevel,updateChart));
		return 1;
	}
	static int update_guild_depot(lua_State*L)
	{
		GET_PLAYER;
		player->NotifyGuildItemList();
		return 0;
	}
	static int get_item_contribute(lua_State* L)
	{
		GET_PLAYER;
		int type_id=luaL_checkinteger(L,2);
		ItemDef* id = player->GetGSL()->GetItemDefineManager()->GetItemDefine(type_id);
		if( id && lua_checkstack(L,40) )
		{
			lua_pushinteger(L,id->mEquipContribute);
			return 1;
		}
		return 0;
	}
	static int get_item_recycle_data(lua_State* L)
	{
		GET_PLAYER;
		int type_id=luaL_checkinteger(L,2);
		ItemDef* id = player->GetGSL()->GetItemDefineManager()->GetItemDefine(type_id);
		if( id && lua_checkstack(L,40) )
		{
			lua_pushinteger(L,id->mRecycleExp);
			lua_pushinteger(L,id->mRecycleXuefu);
			return 2;
		}
		return 0;
	}
	static int gs_kuafu(lua_State *L)//////通知客户端进入跨服操作(告知跨服的相关信息):第一步
	{
		GET_PLAYER;
		const char* ip=luaL_checkstring(L,2);
		const char* port=luaL_checkstring(L,3);
		const char* param=luaL_checkstring(L,4);
		player->Session()->GetListener()->notifyKuafuInfo(player->Session(),ip,port,param);
		return 0;
	}
	static int kuafu_back_gs(lua_State *L)
	{
		GET_PLAYER;
		const char* param=luaL_checkstring(L,2);
		player->Session()->GetListener()->notifyKuafuEnterMainServer(player->Session(),param);
		return 0;
	}
	static int kuafu_get_param(lua_State *L)
	{
		GET_PLAYER;
		lua_pushstring(L,player->Session()->GetCharacter()->mKuafuParam.c_str());
		return 1;
	}
	static int kuafu_set_gs_param(lua_State* L)
	{
		GET_PLAYER;
		const char* k=luaL_checkstring(L,2);
		const char* v=luaL_checkstring(L,3);
		player->Session()->GetListener()->notifyKuafuSetGsParam(player->Session(),k,v);
		return 0;
	}
	static int kuafu_get_jl_state(lua_State *L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->Session()->GetCharacter()->mKuafuResult);
		return 1;
	}
	static int kuafu_set_jl_state(lua_State *L)
	{
		GET_PLAYER;
		int state = luaL_checkint(L,2);
		player->setKuafujlstate(state);
		return 0;
	}
	static int set_max_power(lua_State *L)
	{
		GET_PLAYER;
		int maxpower = luaL_checkint(L,2);
		int needfresh = luaL_checkint(L,3);
		int powper = luaL_checkint(L,4);
		player->SetMaxPower(maxpower,needfresh,powper);
		return 0;
	}
	static int is_in_group(lua_State *L)
	{
		GET_PLAYER;
		int result = player->CheckOnGroup();
		if(lua_checkstack(L,40))
		{
			lua_pushinteger(L,result);
			return 1;
		}
		return 0;
	}
	static int is_friend(lua_State* L)
	{
		GET_PLAYER;
		const char* name = luaL_checkstring(L, 2);
		int result = 0;
		if(player->IsFriend(name))
		{
			result = 1;
		}
		if (lua_checkstack(L, 40))
		{
			lua_pushinteger(L, result);
			return 1;
		}
		return 0;
	}
	static int is_enemy(lua_State* L)
	{
		GET_PLAYER;
		const char* name = luaL_checkstring(L, 2);
		int result = 0;
		if(player->IsEnemy(name))
		{
			result = 1;
		}
		if (lua_checkstack(L, 40))
		{
			lua_pushinteger(L, result);
			return 1;
		}
		return 0;
	}

	static int friend_change(lua_State * L)
	{
		GET_PLAYER;
		const char* name = luaL_checkstring(L, 2);
		int title = luaL_checkint(L, 3);
		player->FriendChange(name, title);
		return 0;
	}

	static int rank_in_all(lua_State* L)///用于全服的排名
	{ 
		GET_PLAYER;
		int param = luaL_checkint(L, 2);///对应索引(16个)
		int bonus_num = luaL_checkint(L,3);//红包元宝数目
		if (param >= 100 && param <= 115)
		{
			player->PushDataToAllForRank(param, bonus_num);
		}
		return 0;
	}
	static int get_mon_name(lua_State* L)
	{
		GET_PLAYER;
		int mon_id = luaL_checkint(L, 2);
		MonDef* md = player->GetGSL()->GetMonDefManager()->FindMonDef(mon_id);
		if(md)
		{
			lua_pushstring(L, md->name);
			return 1;
		}
		return 0;
	}
	static int get_npc_name(lua_State* L)
	{
		GET_PLAYER;
		int npc_id = luaL_checkint(L, 2);
		const char* name = player->GetGSL()->GetMapManager()->getNpcName(npc_id);
		if(name)
		{
			lua_pushstring(L, name);
			return 1;
		}
		lua_pushstring(L, "");
		return 1;
	}
	static int get_directfly_info(lua_State* L)
	{
		GET_PLAYER;
		int fly_id = luaL_checkint(L, 2);

		DirectFlyNode* fn = player->GetGSL()->GetFlyManager()->FindNode(fly_id);
		if(fn)
		{
			lua_pushstring(L, fn->mTargetMapID.c_str()); //
			lua_pushinteger(L, fn->mTargetX);
			lua_pushinteger(L, fn->mTargetY);
			lua_pushstring(L, fn->mNodeName.c_str());
			return 4;
		}
		return 0;
	}

	static int set_task_mon_id(lua_State* L)
	{
		GET_PLAYER;
		int mon_id = luaL_checkint(L, 2);
		player->SetTaskMonID(mon_id);
		return 0;
	}

	static int get_task_mon_id(lua_State* L)
	{
		GET_PLAYER;
		lua_pushinteger(L,player->GetTaskMonID());
		return 1;
	}

	static int notify_guild_member_change(lua_State* L)
	{
		GET_PLAYER;
		if ( (!player->GuildName().empty()) && player->GuildTitle() > GUILD_TITLE_TYPE_ENTERING )
		{
			if (player->Session() && player->Session()->GetDomainGuild())
			{
				player->Session()->GetDomainGuild()->notifyGuildMemberChange();
			}
		}
		return 0;
	}

	static int set_power_recover(lua_State* L)
	{
		GET_PLAYER;
		int val = luaL_checkinteger(L, 2);
		player->PowerRecoverVal(val);
		return 0;
	}

	static int set_short_cut(lua_State* L)
	{
		GET_PLAYER;
		OGSLib::DomainData::Shortcut sc;
		sc.shortcut_id = static_cast<int>(luaL_checkinteger(L,2));
		sc.type = static_cast<int>(luaL_checkinteger(L,3));
		sc.param = static_cast<int>(luaL_checkinteger(L,4));
		player->Session()->GetCharacter()->mShortcutMap[sc.shortcut_id] = sc;
		return 0;
	}

	static int add_obj_func(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkinteger(L,2);
		player->AddExtraObjFuncs(id);
		return 0;
	}

	static int rem_obj_func(lua_State* L)
	{
		GET_PLAYER;
		int id = luaL_checkinteger(L,2);
		player->RemExtraObjFuncs(id);
		return 0;
	}

	static int get_consignment_vcoin(lua_State* L)
	{
		GET_PLAYER;
		int n = player->GetGSL()->GetConsignmentManager()->GetVCoin(player);
		lua_pushinteger(L,n);
		return 1;
	}

	static int push_item_panel_fresh(lua_State* L)
	{
		GET_PLAYER;
		int flag = luaL_checkinteger(L,2);
		player->Session()->GetListener()->notifyItemPanelFresh(player->Session(), flag);
		return 0;
	}

	static int check_skill_can_level_up(lua_State* L)
	{
		GET_PLAYER;
		int flag = luaL_checkinteger(L,2);
		lua_pushnumber(L, 0);  //check_skill_can_level_up
		return 1;
	}

	static const luaL_reg lib_funcs[]={
		{"get_id",get_id},
		{"get_name",get_name},
		{"enter_map",enter_map},
		{"use_item",use_item},
		{"use_item_type_pos",use_item_type_pos},
		{"set_fashion_cloth",set_fashion_cloth},
		
		{"use_fashion_cloth", use_fashion_cloth },
		{"add_item",add_item},
		{"add_item2",add_item2},
		{"add_item_lottery",add_item_lottery},
		{"add_and_dress_equip",add_and_dress_equip},
		{"repair_all",repair_all},
		{"num_item",num_item},
		{"remove_item",remove_item},
		{"remove_item_by_type_pos",remove_item_by_type_pos},
		{"sort_depot",sort_depot},
		{"add_bag_slot",add_bag_slot},
		{"add_depot_slot",add_depot_slot},
		{"start_progress_bar",start_progress_bar},
		{"add_skill",add_skill},
		{"skill_level",skill_level},
		{"set_skill_level",set_skill_level},
		{"use_skill",use_skill},
		{"add_skill_exp",add_skill_exp},
		{"check_skill_level_up",check_skill_level_up},
		{"move",move},
		//{"equip_rerand",equip_rerand},
		//{"equip_upgrade",equip_upgrade},
		{"set_task_state",set_task_state},
		{"set_task_param",set_task_param},
		{"get_task_state",get_task_state},
		{"get_task_param",get_task_param},
		{"push_task_data",push_task_data},
		{"fresh_vcoin_input",fresh_vcoin_input},
		{"get_level",get_level},
		//{"direct_fly_to_npc",direct_fly_to_npc},
		{"set_level",set_level},
		{"get_exp",get_exp},
		{"get_exp_next",get_exp_next},
		{"add_exp",add_exp},
		{"sub_exp",sub_exp},
		{"get_gamemoney",get_gamemoney},
		{"use_gamemoney",use_gamemoney},
		{"add_gamemoney",add_gamemoney},
		{"sub_gamemoney",sub_gamemoney},
		{"get_gamemoney_bind",get_gamemoney_bind},
		{"add_gamemoney_bind",add_gamemoney_bind},
		{"sub_gamemoney_bind",sub_gamemoney_bind},
		{"get_vcoin",get_vcoin},
		{"get_vcoin_gs",get_vcoin_gs},
		{"get_vcoin_accu",get_vcoin_accu},
		{"add_vcoin",add_vcoin_bind},
		{"add_vcoin_best_enable",add_vcoin},
		{"add_congzi",add_congzi},
		{"sub_vcoin",sub_vcoin},
		{"get_vcoin_bind",get_vcoin_bind},
		{"add_vcoin_bind",add_vcoin_bind},
		{"sub_vcoin_bind",sub_vcoin_bind},
		{"gen_rand_position",gen_rand_position},
		{"go_home",go_home},
		{"get_guild",get_guild},
		{"get_gender",get_gender},
		{"alert",alert},
		{"get_map",get_map},
		{"relive",relive},
		{"set_attack_mode",set_attack_mode},
		{"get_attack_mode",get_attack_mode},
		{"set_lock_attack_mode",set_lock_attack_mode},
		{"get_lock_attack_mode",get_lock_attack_mode},
		{"get_job_name",get_job_name},
		{"get_gender_name",get_gender_name},
		{"get_auth_type",get_auth_type},
		{"echo",echo},
		{"show_npc_flags",show_npc_flags},
		{"refresh_task_name",refresh_task_name},
		{"refresh_task_desp",refresh_task_desp},
		{"refresh_npc_show_flags_inview",refresh_npc_show_flags_inview},
		{"find_road_goto",find_raod_goto},
		{"set_param",set_param},
		{"get_param",get_param},
		{"set_guild_param",set_guild_param},
		{"get_guild_param",get_guild_param},
		{"set_group_param",set_group_param},
		{"get_group_param",get_group_param},
		{ "set_tianguan_lv", set_tianguan_lv },
		{ "updataChart", updataChart },
		{"get_group_task",get_group_task_flag},
		{"set_temp_param",set_temp_param},
		{"get_temp_param",get_temp_param},
		{"set_timer",set_timer},
		{"get_timer",get_timer},
		{"get_last_login_time",get_last_login_time},
		{"get_last_logout_time",get_last_logout_time},
		{"get_create_time",get_create_time},
		{"get_last_login_ip",get_last_login_ip},
		{"countdown_notify",countdown_notify},
		{"set_status",set_status},
		{"set_status_plus",set_status_plus},
		{"set_slave_status",set_slave_status},
		{"clear_status",clear_status},
		{"set_buff",set_buff},
		{"enable_buff",enable_buff},
		{"have_buff", have_buff},
		{"clear_buff", clear_buff},
		{"is_status",is_status},
		{"get_status_param",get_status_param},
		{"get_status_data",get_status_data},
		{"get_status_duration",get_status_duration},
		{"get_weapon_luck",get_weapon_luck},
		{"set_weapon_luck",set_weapon_luck},
		{"get_pk_value",get_pk_value},
		{"set_pk_value",set_pk_value},
		{"sub_pk_value",sub_pk_value},
		{"get_honor",get_honor},
		{"set_honor",set_honor},
		{"sub_honor",sub_honor},
		{"add_honor",add_honor},
		{"get_pos",get_pos},
		{"chat_on",chat_on},
		{"chat_off",chat_off},
		{"num_bag_black",num_bag_black},
		{"num_bag",num_bag},
		{"get_guild_title",get_guild_title},
		{"get_guild_pt",get_guild_pt},
		{"add_guild_pt",add_guild_pt},
		{"sub_guild_pt",sub_guild_pt},
		{"set_name_pre",set_name_pre},
		{"set_name_pro",set_name_pro},
		{"check_euqip",check_euqip},
		{"set_model",set_model},
		{"get_model",get_model},
		{"get_online_time",get_online_time},
		{"get_online_time_all",get_online_time_all},
		{"get_online_time_today",get_online_time_today},
		{"get_online_time_yestoday",get_online_time_yestoday},
		{"get_offline_time",get_offline_time},
		{"set_offline_time",set_offline_time},
		{"get_team_id",get_team_id},
		{"get_team_name",get_team_name},
		{"set_team_info",set_team_info},
		{"get_group_size",get_group_size},
		{"group_exe",group_exe},
		{"group_size_online",group_size_online},
		{"get_group_id",get_group_id},
		{"get_group_leader",get_group_leader},
		{"get_guild_size",get_guild_size},
		{"guild_exe",guild_exe},
		{"friend_exe", friend_exe},
		{"is_dead",is_dead},
		{"black_board",black_board},
		{"play_effect",play_effect},
		{"get_item",get_item},
		{"get_item_by_pos",get_item_by_pos},
		{"get_item_data",get_item_data},
		{"get_item_data_by_pos",get_item_data_by_pos},
		{"get_item_crtaetime_by_pos", get_item_crtaetime_by_pos },
		
		{"get_item_base_data",get_item_base_data},
		{"get_item_rand_data",get_item_rand_data},
		{"set_item_data",set_item_data},
		{"push_item_info",push_item_info},
		{"fresh_item_info",fresh_item_info},
		{"get_item_type_by_pos",get_item_type_by_pos},
		{ "set_item_lasttime", set_item_lasttime },
		
		{"get_item_lasttime", get_item_lasttime },
		{"danger_clear_item",danger_clear_item},
		{"danger_drop_item_by_typeid",danger_drop_item_by_typeid},
		{"drop_item", drop_item },
		{"get_ac",get_ac},
		{"get_mac",get_mac},
		{"get_dc",get_dc},
		{"get_mc",get_mc},
		{"get_sc",get_sc},
		{"get_dodge",get_dodge},
		{"get_accuracy",get_accuracy},
		{"get_hp_max",get_hp_max},
		{"get_mp_max",get_mp_max},
		{"get_load_max",get_load_max},
		{"get_brawn_max",get_brawn_max},
		{"get_burden_max",get_burden_max},
		{"change_job",change_job},
		{"change_gender",change_gender},
		{"get_gm_level",get_gm_level},
		{"add_item_log",add_item_log},
		{"log_item_change",log_item_change},
		{"use_gem", use_gem},
		{"undress_gem", undress_gem},
		{"gen_slave",gen_slave},
		{"kill_slave",kill_slave},
		{"have_slave",have_slave},
		{"add_slave_skill",add_slave_skill},
		{"clear_slave",clear_slave},
		{"gen_slave_fashi",gen_slave_fashi},
		{"kill_slave_fashi",kill_slave_fashi},
		{"clear_slave_fashi",clear_slave_fashi},
		{"gen_slave_statue", gen_slave_statue},
		{"clear_mofadun",clear_mofadun},
		{"achieve_get_param",achieve_get_param},
		{"achieve_set_param",achieve_set_param},
		{"achieve_addone_param",achieve_addone_param},
		{"achieve_get_state",achieve_get_state},
		{"achieve_set_state",achieve_set_state},
		{"achieve_addone_state",achieve_addone_state},
		{"achieve_get_point",achieve_get_point},
		{"achieve_add_point",achieve_add_point},
		{"leave_group",leave_group},
		{"get_safearea_id",get_safearea_id},
		//{"get_pay_url",get_pay_url},
		{"vcoin_used_by_day",vcoin_used_by_day},
		{ "get_item_uplv", get_item_uplv },
		{ "check_item_fashion", check_item_fashion },
		{ "set_item_uplv", set_item_uplv },
		{"vcoin_used_by_month",vcoin_used_by_month},
		{"vcoin_accu_by_month",vcoin_accu_by_month},
		//{"invite_marry",invite_marry},
		//{"invite_divorce",invite_divorce},
		//{"get_couple_id",get_couple_id},
		//{"check_female_to_marry",check_female_to_marry},
		//{"check_female_to_divorce",check_female_to_divorce},
		//{"group_follow_move",group_follow_move},
		//{"clear_marrytimes",clear_marrytimes},
		//{"get_double_name",get_double_name},
		{"gen_dart",gen_dart},
		{"kill_dart",kill_dart},
		{"have_dart",have_dart},
		{"bind_dart", bind_dart},
		{"get_dart_id", get_dart_id},
		{"directfly_to_dart",directfly_to_dart},
		{"set_dart_life_time",set_dart_life_time},
		{"get_dart_life_time",get_dart_life_time},
		{"get_dart_map", get_dart_map},
		{"get_dart_pos", get_dart_pos},
		{"get_dart_follow_pos", get_dart_follow_pos},
		{"get_dart_reach_stop", get_dart_reach_stop},

		{"get_empty_number_in_lotterydepot",get_empty_number_in_lotterydepot},
		{"add_drop_prob",add_drop_prob},
		{"sub_drop_prob",sub_drop_prob},
		{"add_double_att_prob",add_double_att_prob},
		{"sub_double_att_prob",sub_double_att_prob},
		{"get_drop_prob",get_drop_prob},
		{"get_double_att_prob",get_double_att_prob},
		{"get_total_updlevel",get_total_updlevel},
		{"set_total_updlevel",set_total_updlevel},
		//{"set_gift_list",set_gift_list},
		{"set_puppet_mul",set_puppet_mul},
		{"get_puppet_mul",get_puppet_mul},

		{"update_chart_list",update_chart_list},
		//{"get_mount_info",get_mount_info},
		//{"set_mount_info",set_mount_info},
		{"get_mount_show",get_mount_show},
		{"set_mount_show",set_mount_show},
		{"get_mount_show_lock",get_mount_show_lock},
		{"set_mount_show_lock",set_mount_show_lock},
		{"set_avatar",set_avatar},
		{"get_avatar",get_avatar},
		{"doaction",doaction},
		{"item_sell",item_sell},
		{"set_script_attr",set_script_attr},
		{"get_script_attr",get_script_attr},
		{"recal_attr",recal_attr},
		{"set_script_item_attr",set_script_item_attr},
		{"script_exe_code",script_exe_code},
		{"get_fightpoint",get_fightpoint},
		{"push_lua_table",push_lua_table},
		{"load_fabao_res",load_fabao_res},
		{"change_fabao_res",change_fabao_res},
		{"set_zslevel",set_zslevel},
		{"get_enter_guild_time",get_enter_guild_time},
		{"change_name",change_name},
		{"num_xjbag_black",num_xjbag_black},
		{"get_seed_name",get_seed_name},
		{"setHpMpPercent",setHpMpPercent},
		{"post_mail",post_mail},
		//{"post_mail_to_all",post_mail_to_all},
		{"item_upgrade",item_upgrade},
		{"item_add_upd_failed",item_add_upd_failed},
		{"item_sub_upd_failed",item_sub_upd_failed},
		{"get_viplevel",get_viplevel},
		{"list_chargedart",list_chargedart},
		{"add_hp",add_hp},
		{"add_mp",add_mp},
		{"notify_hpmpchange",notify_hpmpchange},
		{"notify_mini_npc",notify_mini_npc},
		{"set_sword_level",set_sword_level},
		{"get_sword_data",get_sword_data},
		{"get_level_exp",get_level_exp},
		{"set_wing_lv",set_wing_lv},
		{"update_guild_depot",update_guild_depot},
		{"get_item_contribute",get_item_contribute},
		{"get_item_recycle_data",get_item_recycle_data},
		{"gs_kuafu",gs_kuafu},
		{"kuafu_back_gs",kuafu_back_gs},
		{"kuafu_get_param",kuafu_get_param},
		{"kuafu_set_gs_param",kuafu_set_gs_param},
		{"kuafu_get_jl_state",kuafu_get_jl_state},
		{"kuafu_set_jl_state",kuafu_set_jl_state},
		{"set_max_power",set_max_power},
		{"item_zhuling",item_zhuling},
	    {"is_in_group",is_in_group},
		{"set_monpoint_level",set_monpoint_level},
		{"Become_Monster",Become_Monster},
		{"get_monpoint_data",get_monpoint_data},
		{"is_friend",is_friend},
		{"is_enemy",is_enemy},
		{"friend_change", friend_change},
		{"rank_in_all",rank_in_all},
		{"num_friend",num_friend},
		{"get_item_stack",get_item_stack},
		{"change_hp_mp",change_hp_mp},
		{"notify_status_desp",notify_status_desp},
		{"get_huweidef_data",get_huweidef_data},
		{"get_mon_name",get_mon_name},
		{"get_npc_name",get_npc_name},
		{"get_directfly_info",get_directfly_info},
		{"set_attr_mult",set_attr_mult},
		{"get_attr_mult",get_attr_mult},
		{"set_task_mon_id",set_task_mon_id},
		{"get_task_mon_id",get_task_mon_id},
		{"get_dart_hp",get_dart_hp},
		{"push_skill_def", push_skill_def},
		{"set_fight_point", set_fight_point},
		{"get_slave", get_slave},
		{"get_slave_by_id", get_slave_by_id},
		{"get_buff_valid_max_value", get_buff_valid_max_value},
		{"get_buff_valid_value", get_buff_valid_value},
		{"notify_guild_member_change", notify_guild_member_change},
		{"set_power_recover", set_power_recover},
		{"get_consignment_vcoin", get_consignment_vcoin},
		{"set_short_cut", set_short_cut},
		{"add_obj_func", add_obj_func},
		{"rem_obj_func", rem_obj_func},
		{"push_item_panel_fresh", push_item_panel_fresh},
		{"check_skill_can_level_up", check_skill_can_level_up },
		{0,0},
	};
	ScriptLuaPlayerBind::ScriptLuaPlayerBind(void)
	{
	}
	ScriptLuaPlayerBind::~ScriptLuaPlayerBind(void)
	{
	}
	void ScriptLuaPlayerBind::init(lua_State* L)
	{
		luaL_newmetatable(L, "oncient.player");
		lua_pushvalue(L,-1);
		lua_setfield(L,-2,"__index");
		luaL_register(L,0,lib_funcs);
		lua_pop(L,1);
	}

	void ScriptLuaPlayerBind::bind(lua_State* L,DomainPlay* player)
	{
		DomainPlay** a = (DomainPlay**)lua_newuserdata(L, sizeof(player));
		*a = player;

		luaL_getmetatable(L, "oncient.player");
		lua_setmetatable(L, -2);
	}
}