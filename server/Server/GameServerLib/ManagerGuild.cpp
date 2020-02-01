#include "stdafx.h"
#include "ManagerGuild.h"
#include "GameServerLib.h"
#include "GuildGuild.h"
#include "ManagerItemDef.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include <sstream>

namespace OGSLib
{
	ManagerGuild::ManagerGuild(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
		mUpdateTick = 0;
	}
	ManagerGuild::~ManagerGuild(void)
	{
	}
	bool ManagerGuild::LoadGuildLvInfo()
	{
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("guildinfo.txt",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int level;
					int maxnumber;
					int exp;
					int ret = sscanf(line,"%d%d%d",&level,&maxnumber,&exp);
					if( 3 == ret )
					{
						GuildLvInfo gli;
						gli.mLevel = level;
						gli.mMaxNum = maxnumber;
						gli.mPoint = exp;
						mGuildLvInfos[gli.mLevel] = gli;
					}
				}
			}
		}
		return true;
	}
	GuildLvInfo* ManagerGuild::GetGuildLvInfo(int level)
	{
		GuildLvMap::iterator pos = mGuildLvInfos.find(level);
		if( pos != mGuildLvInfos.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
	bool ManagerGuild::LoadShopGood()
	{
		mShopGoods.clear();
		char temp[256];
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("guildshop.csv",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int pos;
					char nameorid[256];
					int good_id=0;
					int num=0;
					int price=0;
					int pricetype=0;
					int oldprice=0;
					int guildlv=0;
					int limitnum=0;
					int sscanfret = sscanf(line,"%d,%[^,],%d,%d,%d,%d,%d,%d",&pos,nameorid,&num,&price,&pricetype,&oldprice,&guildlv,&limitnum);
					if( 8 == sscanfret )
					{
						if( nameorid[0] > 0 && isdigit(nameorid[0]) )
						{
							good_id=atoi(nameorid);
							ItemDef* id = mGameServerLib->GetItemDefineManager()->GetItemDefine(good_id);
							if( id )
							{
								num = 1;	
							}else
							{
								good_id=0;
							}
						}
						else
						{
							UtilString::act_2_utf8(nameorid,strlen(nameorid),temp,256);
							strncpy_s(nameorid,255,temp,255);
							nameorid[255]=0;
							ItemDef* id = mGameServerLib->GetItemDefineManager()->GetItemDefine(nameorid);
							if( id )
							{
								num = 1;
								good_id = id->mTypeID;
							}
						}
						if( good_id > 0 && num > 0 && price >= 0 && oldprice >=0 )
						{
							ShopGoodClass sg;
							sg.pos = pos;
							sg.good_id = good_id;
							sg.type_id = good_id;
							sg.price_type = pricetype;
							sg.price = price;
							sg.oldprice = oldprice;
							sg.number = 1;//num;
							sg.guildlv = guildlv;
							sg.limitnum = limitnum;
							mShopGoods[pos] = sg;
						}
					}
				}
			}
		}
		if( mShopGoods.empty() )
		{
			return false;
		}
		return true;
	}
	Guild* ManagerGuild::CreateGuild(const std::string& guild_name)
	{
		Guild* gg = this->FindGuildByName(guild_name);
		if( gg ){ return 0;}

		std::string cg ="new";
		int now = static_cast<int>(time(0));
		int num = rand()%9000 + 1000;
		std::stringstream seedid; seedid<<cg<<now<<num;
		std::string guildid = seedid.str();

		Guild* g=new Guild;
		g->SetName(guild_name);
		g->SetSeedID(guildid);
		g->mGameServerLib = mGameServerLib;
		mGuilds[guildid]=g;
		//SaveGuilds();
		return g;
	}
	void ManagerGuild::DeleteGuild(const std::string& guild_name)
	{
		Guild* g = FindGuild(guild_name);
		if( g )
		{
			mGuilds.erase(guild_name);
			delete g;
		}
	}
	Guild* ManagerGuild::FindGuild(const std::string& guild_name)
	{
		GuildMap::iterator pos = mGuilds.find(guild_name);
		if( pos != mGuilds.end() )
		{
			return pos->second;
		}
		return 0;
	}
	Guild* ManagerGuild::FindGuildByName(const std::string& guild_name)
	{
		for( GuildMap::iterator pos = mGuilds.begin();
			pos != mGuilds.end();
			++ pos )
		{
			if( guild_name == pos->second->GetName())
			{
				return pos->second;
			}
		}
		return 0;
	}
	void ManagerGuild::DismissGuild(const std::string& guild_name)
	{
		Guild* g = FindGuildByName(guild_name);
		if( g ){
			g->DismissGuild();
		}
		SaveGuilds();
	}
	bool ManagerGuild::LoadGuilds()
	{
		mGameServerLib->GetListener()->LoadGuilds(0);
		return true;
	}
	bool ManagerGuild::OnLoadGuilds(const OGSLib::ManagerGuild::GuildMap& guild_map)
	{
		mGuilds = guild_map;
		for( GuildMap::iterator pos = mGuilds.begin();
			pos != mGuilds.end();
			++ pos )
		{
			pos->second->mGameServerLib = mGameServerLib;
		}
		return true;
	}
	void ManagerGuild::update()
	{
		if (mUpdateTick + 60 < time(0))
		{
			mUpdateTick = time(0);
			for( GuildMap::iterator pos = mGuilds.begin();
				pos != mGuilds.end();
				++ pos )
			{
				pos->second->update();;
			}
		}

		if( mSaveTick + 30*60 < time(0) )
		{
			mSaveTick = time(0);
			SaveGuilds();
		}
	}
	bool ManagerGuild::SaveGuilds()
	{
		mGameServerLib->GetListener()->SaveGuilds(*this,0);
		return true;
	}
	int ManagerGuild::init()
	{
		LoadGuilds();
		LoadGuildLvInfo();
		mSaveTick = time(0);
		mUpdateTick = time(0);
		return 0;
	}
	void ManagerGuild::dest(bool save)
	{
		if( save )
		{
			SaveGuilds();
		}
		for( GuildMap::iterator pos = mGuilds.begin();
			pos != mGuilds.end();
			++ pos )
		{
			Guild* g = pos->second;
			delete g;
		}
	}
}
