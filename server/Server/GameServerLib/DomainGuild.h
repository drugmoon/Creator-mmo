#pragma once
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{
	class GameSession;
	class DomainData;
	class GameServerLib;
	class DomainPlay;
	class Guild;
	struct GuildRedPacketRecord;
	class DomainGuild : public LEUD::PoolMemoryBase< DomainGuild >
	{
		GameServerLib* mGSL;
		GameSession* mSession;
		DomainData* mCharacter;

		std::string mGuildName;
		std::string mGuildID;
		int mGuildTitle;
		std::string mParamDefault;
		bool mIsInit;

		// 拉取物品日志时间
		double mGetItemLogTick;
		// 拉取红包日志时间
		double mGetRedPacketLogTick;
	public:
		DomainGuild(GameSession* session);
		virtual ~DomainGuild(void);
		void initGuildHandle();
		void destGuildHandle();
		void Save();
		const std::string& GuildName(){return mGuildName;}
		const std::string& GetGuildName();
		void clearGuildName(){mGuildName = "";}
		const std::string& GuildID(){return mGuildID;}
		void clearGuildID(){mGuildID = "";}
		int GuildTitle(){return mGuildTitle;}
		void GuildTitle(int title){mGuildTitle=title;}
		int GuildSize();
		void GuildExe(const char* code);
		DomainPlay*GetPlayer();
		void GetGuildWarState(std::string& seed_id, int &war_state, int &time_left);
	public:
		const std::string& Var(const std::string& key);
		void Var(const std::string& key,const std::string& value);
		const std::string& LeaderName();
		const char* Name();
		const char* SeedName();
		int level();
		int gmlevel();
		int gender();
		int job();
		int fight();
		int guildpt();
		int guildspt();
		int lastloginout();
		int groupid();
		void ExeCode(const char* code);
	public:
		int GuildChat(char* msg);
		int CreateGuild(char* guild_name,int flags);
		int JoinGuild(char* guild_name,int flags);
		int LeaveGuild(const char* guild_name);
		int SetGuildFight();
		int SetGuildPt();
		void ClearGuildPt();
		int GetEnterTime();
		int SetGuildLastLoginOut();
		int SetGuildOnlineState(bool online);
		int SetGuildInfo(const std::string& guild_name,char* desp,char* notice);
		// int ChangeGuildMemberTitle(DomainPlay* play, const char* guild_name, const char* nick_name, int dir);

	public:
		void onEnterGuild();
		void onLeaveGuild(int reason);
		void onSetGuildEntering(Guild* guild,int entering);
		void onSetGuildAndTitle(const char* guild_name, const char* guild_seed, int title);
		void onLeaveGuildRemoveBuff();
		void onGetGuildRedPacketLog();
		void onGetGuildItemLog();
		void notifyGuildRedPacketLog(GuildRedPacketRecord * pRecord);
		virtual void NotifyGuildChat(Guild* gld,DomainGuild* src,const char* msg);
		void notifyGuildInfoChange();
		void notifyGuildMemberChange();
		void NotifyGuildExpChange(int lv,int exp,int tarexp);
		void NotifyAlert(int pos,int flags,const char* msg);
		void NotifyWar();
	private:
		void notifyGuildTitle();
	};
}