#pragma once
#include "leudgrid/MemoryPool.h"
#include <list>
#include <set>

namespace OGSLib
{
	#define RED_PACKET_RECORD_MAX_COUNT 50
	#define GUILD_ITEM_RECORD_MAX_COUNT 50
	#define GUILD_DEPOT_BEGIN 0
	#define GUILD_DEPOT_SIZE 375
	#define GUILD_DEPOT_END (GUILD_DEPOT_BEGIN + GUILD_DEPOT_SIZE)
	enum GUILD_TITLE_TYPE
	{
		GUILD_TITLE_TYPE_OUT=100,
		GUILD_TITLE_TYPE_ENTERING=101,
		GUILD_TITLE_TYPE_NORMAL=102,
		GUILD_TITLE_TYPE_ADV=200,
		GUILD_TITLE_TYPE_VICEADMIN=300,
		GUILD_TITLE_TYPE_ADMIN=1000,
	};

	class DomainGuild;
	class GameServerLib;
	struct GuildNode
	{
	public:
		GuildNode()
		{
			title=GUILD_TITLE_TYPE_OUT;
			online_state = 0;
			player = 0;
			fight = 0;
			lastlogin = 0;

			job=0;
			gender=0;
			lv=0;
			cloth=0;
			weapon=0;
			wing=0;
			pifeng=0;
			guildpt=0;
			guildspt=0;
			entertime = 0;
		}
		std::string nick_name;
		std::string seed_name;
		int title;
		int online_state;
		int fight;
		int lastlogin;
		int guildpt;
		int guildspt;
		int entertime;
		DomainGuild* player;
		int job;
		int gender;
		int lv;
		int cloth;
		int weapon;
		int wing;
		int pifeng;
	};
	struct GuildItem
	{
	public:
		GuildItem()
		{
			mTypeID = 0;
			mLevel = 0;
			mZlevel = 0;
		}
		int mTypeID;
		int mLevel;
		int mZlevel;
	};

	enum 
	{
		OP_CODE_SEND = 0, 
		OP_CODE_TAKE,
	};

	// 红包记录
	struct GuildRedPacketRecord
	{
		std::string name;
		// 0 发送 1领取
		int op_code;
		int value;
		int count;
		GuildRedPacketRecord()
		{
			op_code = 0;
			value = 0;
			count = 0;
		};
	};

	// 行会仓库操作记录
	struct GuildItemOpRecord
	{
		// 操作者名字
		std::string name;
		// 物品id
		int item_type;
		// 时间
		time_t time;
		// 操作类型
		int op_code;
		GuildItemOpRecord()
		{
			item_type = 0;
			time = 0;
			op_code = 0;
		}
	};

	struct GuildRedPacket
	{
		std::string mSenderSeedName;
		std::string mSenderName;
		// 剩余数量
		int mRemainSize;
		// 剩余红包
		int mRemainValue;
		// 红包发送时间
		time_t mSendTime;
		// 红包记录
 		std::list<GuildRedPacketRecord> mRecordList;
		// 领取红包记录
		std::set<std::string> mTakeNames; 
		GuildRedPacket()
		{
			mRemainSize = 0;
			mRemainValue = 0;
			mSendTime = 0;
		}
	};

	struct GuildWar
	{
		// 对方行会
		std::string mGuildSeed;
		// 是否主动发起
		int mAct;
		// 帮会战发起时间
		time_t mStartTime;
	};

	class Guild : public LEUD::PoolMemoryBase< Guild >
	{
	public:
		typedef std::map<std::string,std::string> ParamMap;
		typedef std::map<std::string,GuildNode> MemberMap;
		typedef std::vector<std::string> GuildList;
		typedef std::map<int,GuildItem> ItemMap;
		typedef std::list<GuildItemOpRecord> ItemOpRecordList;
		typedef std::list<GuildRedPacketRecord> RedPacketRecordList;
		typedef std::map<std::string, GuildWar> GuildWarMap;
	private:
		std::string mName;
		std::string mDesp;
		std::string mNotice;
		MemberMap mMembers;
		MemberMap mSeedMap;
		MemberMap mPremems;

		GuildList mEnemyGuilds;
		GuildList mFriendGuilds;
		ParamMap mParams;
		std::string mParamDefault;
		std::string mSeedID;
		int mGuildMasterLevel;
		int mGuildLevel;
		int mGuildMemberMaxNum;
		int mGuildExp;
		int mDeleted;
		int mGuildAdvNum;
		int mGuildViceAdminNum;
		int mMaxMemberCount;

		ItemMap mItems;
		GuildRedPacket mRedPacketInfo;

		std::list<GuildItemOpRecord> mItemOpRecordList;

		GuildWarMap mGuildWarMap;
	public:
		GameServerLib* mGameServerLib;
		Guild();
		virtual ~Guild(void);
		void update();
		ItemMap& GetItems(){return mItems;}
		int AddItem(int typeID,int level,int zlevel);
		int find_empty_item_position();
		void RemItem(int pos);

		int master_level(){return mGuildMasterLevel;}
		void master_level(int mslv){mGuildMasterLevel=mslv;}

		int guild_level(){return mGuildLevel;}
		void guild_level(int lv){mGuildLevel = lv;}

		int guild_pt(){return mGuildExp;}
		void guild_pt(int pt){mGuildExp = pt;}
		void addguild_pt(int pt){mGuildExp += pt;}
		int add_guildpt(const std::string& name,int add);
		int GetAdvNum(){return mGuildAdvNum;}
		void AddAdvNum(){mGuildAdvNum ++;}
		void SubAdvNum(){ mGuildAdvNum--; }
		void AddViceAdminNum(){ mGuildViceAdminNum ++;}
		void SubViceAdminNum(){ mGuildViceAdminNum --; }
		int GetViceAdminNum(){return mGuildViceAdminNum;}

		int deleted(){return mDeleted;}

		const std::string& SeedID(){return mSeedID;}
		void SetSeedID(const std::string& id){mSeedID=id;}

		const std::string& GetGuildLeaderName();
		const std::string& GetGuildLeaderSeed();
		DomainGuild* GetGuildLeader();

		const std::string& GetDesp(){return mDesp;}
		void SetDesp(const std::string& desp){mDesp=desp;}

		const std::string& GetNotice(){return mNotice;}
		void SetNotice(const std::string& notice){mNotice=notice;}

		const std::string& GetName(){return mName;}
		void SetName(const std::string& name){mName=name;}

		int AddMember(const char* nick_name,const char* seed_name,int title,DomainGuild* player);
		int RemMember(const std::string& seed_name, int reason);

		size_t GetMemberNumber(){return mMembers.size();}
		MemberMap& GetMembers(){return mMembers;}
		const MemberMap& GetSeedMap(){return mSeedMap;}

		void SetGuildName(const std::string& seed_name);
		const std::string& GetGuildSeed(const std::string& nick_name);
		const std::string& GetGuildPreSeed(const std::string& nick_name);

		const std::string& Var(const std::string& key);
		void Var(const std::string& key,const std::string& value);
		const ParamMap& GetParams(){return mParams;}

		int GetMemberTitle(const std::string& seed_name);
		int SetMemberTitle(const std::string& seed_name,int title);
		int GetNextTitle(int title);
		int GetPrevTitle(int title);

		// 一键同意
		int AgreeAll();
		// 一键拒绝
		int RefuseAll();

		void NotifyGuildPresident(const std::string& seed_name,int flag);

		int GetMemberOnlineState(const std::string& seed_name);
		int SetMemberOnlineState(const std::string& seed_name,int online_state,DomainGuild* player);
		int SetMemberFight(const std::string& seed_name,int fight);
		int SetMemberLv(const std::string& seed_name,int lv);
		int SetMemberGender(const std::string& seed_name,int gender);
		int SetMemberCloth(const std::string& seed_name,int v);
		int SetMemberWeapon(const std::string& seed_name,int v);
		int SetMemberWing(const std::string& seed_name,int v);

		int SetMemberJob(const std::string& seed_name,int job);
		int SetMemberGuildPt(const std::string& seed_name,int guildpt,int spt);
		int SetMemberEnterTime(const std::string& seed_name,int entertime);
		int GetMemberEnterTime(const std::string& seed_name);
		int SetMemberLastLoginOut(const std::string& seed_name,int out);
		void GuildChat(DomainGuild* src,char* msg);
		void GuildExpChange(DomainGuild* src);
		void DismissGuild();

		int AddEnemyGuild(const std::string& guild_name);
		int RemEnemyGuild(const std::string& guild_name);
		bool IsEnemyGuild(const std::string& guild_name);
		const GuildList& GetEnemyGuilds(){return mEnemyGuilds;};

		int AddFriendGuild(const std::string& guild_name);
		int RemFriendGuild(const std::string& guild_name);
		bool IsFriendGuild(const std::string& guild_name);
		const GuildList& GetFriendGuilds(){return mFriendGuilds;};

		int MakeAdmin();
		const std::string HaveAdmin();

		GuildRedPacket& GetRedPacketInfo() {return mRedPacketInfo;};

		MemberMap& GetPremems(){return mPremems;}
		GuildNode* GetMember(const char* nick_name);
		GuildNode* GetPremem(const char* nick_name);

		int AddPreMember(const char* seed_name,DomainGuild* player);
		int RemPreMember(const char* seed_name,DomainGuild* player);
		int SetMemberInfo(const char* seed_name,DomainGuild* player);

		void AddRedPacketRecord(const char * nick_name, int value, int count, int op_code);
		// 设置行会红包,会替换老的红包
		void SetRedPacket(const char * nick_name, int value, int count);
		const char * GetRedPacketSender();
		int GetRedPacketRemainingValue();
		int GetRedPacketRemainingSize();
		int GetRedPacketRemainingTime();

		RedPacketRecordList & GetRedPacketLogList();
		ItemOpRecordList & GetGuildItemOpLogList();
		// 领取红包
		int TakeRedPacket(const char * nick_name);
		int TakeRedPacketProc();

		//检测是否还能领取红包
		int CheckRedPacket(const char * nick_name);

		// 退回红包
		void SendBackRedPacket();

		// 行会仓库操作日志
		void AddItemOpRecord(std::string name, int item_type, int op_code);

		void ClearRedPacketInfo();
		void ClearItemRecords();

		// 物品捐献日志
		void ExeCode(const char* code);

		// 主动发起帮会战
		int StartGuildWar(std::string seed_id, bool boAct = true);
		// 所有行会行会战
		void StartAllGuildWar();
		// 获取行会战状态
		void GuildWarState(std::string& seed_id, int &war_state, int &time_left);
		// 更新行会战,时间到了清除
		void UpdateGuildWar();
		void RemoveGuildWar(std::string seed_id);
		bool IsWarGuild(std::string& seed_id);
		void ClearGuildWar();
		void ClearRelGuildWar();
		GuildWarMap & GetGuildWarMap();
		void NotifyGuildMemberChange(int min_title = 0);
		void NotifyAlert(int po,int flags,const char* msg);
		void NotifyWar();
		void SetMaxMemberCount(int count);
		// 刷新成员信息
		void FreshMemberInfo();
	};
}