#pragma once

namespace OGSLib
{
	class GuildLvInfo
		{
	public:
		int mLevel;
		int mMaxNum;
		int mPoint;
	};
	class Guild;
	class GameServerLib;
	class ManagerGuild
	{
	public:
		struct ShopGoodClass
		{
			int pos;
			int good_id;
			int type_id;
			int price_type;
			int price;
			int oldprice;
			int number;
			int guildlv;
			int limitnum;
		};
		typedef std::map<int,ShopGoodClass> ShopGoodMap;

		typedef std::map<std::string,Guild*> GuildMap;
		typedef std::map<int,GuildLvInfo> GuildLvMap;
	private:
		GuildMap mGuilds;
		GuildLvMap mGuildLvInfos;
		ShopGoodMap mShopGoods;
		GameServerLib* mGameServerLib;
		time_t mSaveTick;
		time_t mUpdateTick;
	public:
		ManagerGuild(GameServerLib* gslib);
		virtual ~ManagerGuild(void);

		int init();
		void dest(bool save=true);
		void update();
		
		bool LoadGuildLvInfo();

		Guild* CreateGuild(const std::string& guild_name);
		Guild* FindGuild(const std::string& guild_id);
		Guild* FindGuildByName(const std::string& guild_name);
		void DismissGuild(const std::string& guild_id);
		void DeleteGuild(const std::string& guild_id);
		const GuildMap& GetGuilds()const{return mGuilds;}
		GuildLvMap& GetGuildLvMap(){return mGuildLvInfos;}

		GuildLvInfo* GetGuildLvInfo(int level);

		bool LoadShopGood();
		bool ReloadShopGood(bool record = false);
		ShopGoodMap* GetGoods(){return &mShopGoods;}

		bool LoadGuilds();
		bool OnLoadGuilds(const OGSLib::ManagerGuild::GuildMap& guild_map);
		bool SaveGuilds();
	};
}