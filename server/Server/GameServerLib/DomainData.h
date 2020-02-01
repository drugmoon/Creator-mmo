#pragma once
#include "leudgrid/MemoryPool.h"
#include "SubFriend.h"

namespace OGSLib
{
	class SubItem;
	class SubSkill;
	class SubTask;
	class SubStatus;
	class SubGift;
	class SubMail;
	class SubFriend;
	class SubAchieve;
	class SubBuff;

	class DomainData : public LEUD::PoolMemoryBase< DomainData >
	{
	public:
		int mGuildPt;
		int mGuildSpt;
		char mAccount[256];
		char mChrName[256];
		char mSeedName[256];

		int level;
		LEUD_INT64 exp;
		LEUD_INT64 money;
		LEUD_INT64 moneybind;
		int vcoin;
		int vcoings;
		int vcoinbind;
		int vcoinaccu;

		int mVcoinAccuByMonth;
		int mLastCongzhiMonth;
		int mVcoinUsedByDay;
		int mVcoinUsedByMonth;
		int mLastVcoinUsedDay;

		int mLockTradeLimit;

		int mHonor;
		int mCloth;///登录游戏时候显示的衣服
		int mWeapon;
		int mFashionCloth;
		int mFashionWeapon;
		int mWing;

		int job;
		int gender;

		int cur_hp;
		int cur_mp;
		int cur_power;

		char exit_map[256];
		int         exit_x;
		int			exit_y;
		char safe_map[256];
		int			safe_x;
		int			safe_y;

		int pkvalue;
		int talk_prohibited;

		char mGuildName[256];
		char mGuildID[256];

		char mLoginIP[256];

		//int mCoupleID;
		//int mWeddingdate;
		//int mMarryTimes;

		int depot_slot_add;
		int bag_slot_add;
		int mFreeDirectFly;

		typedef std::vector<SubItem> ItemMap;
		int mItemDataVersion;
		ItemMap mItemList;
		ItemMap mChatTradeItemList;

		typedef std::vector<SubSkill> SkillMap;
		SkillMap mSkillList;

		typedef std::map<int,SubTask> TaskMap;
		TaskMap mTaskMap;

		typedef std::map<LEUD_INT64,SubMail> MailMap;
		MailMap mMailList;

		typedef std::map<int,SubStatus> StatusMap;
		StatusMap mStatusMap;

		typedef std::map<int,SubBuff> BuffMap;
		BuffMap mBuffMap;

		typedef std::vector<SubGift> GiftArray;
		GiftArray mGiftArray;

		OGSLib::FrindArray mFriendArray;

		typedef std::map<int,SubAchieve> AchieveMap;
		AchieveMap mAchieveMap;
		int mAchievePoint;
		int mAchieveGameMoneyMax;
		time_t mOnlineTimeMax;
		int mLoginDayContinue;
		int mLoginDayContinueMax;
		int mLoginDayCount;

		time_t create_time;
		time_t last_login_time;
		char last_login_ip[256];
		time_t last_logout_time;

		time_t mOnlineTimeThis;
		time_t online_time_all;
		time_t online_time_today;
		time_t online_tag_today;
		time_t online_time_yestoday;
		time_t online_tag_yestoday;
		time_t online_time_tick;

		time_t offline_time;
		time_t offline_tag;

		time_t online_time;

		struct Shortcut
		{
			int shortcut_id;
			int type;
			int param;
		};
		typedef std::map<int,Shortcut> ShortcutMap;
		ShortcutMap mShortcutMap;

		typedef std::map<int,std::string> ParamMap;
		ParamMap mParamMap;

		typedef std::map<int,int> NumParamMap;
		NumParamMap mNumParamMap;

		char china_id[256];
		char china_nm[256];
		int china_id_ok;
		int china_limit_online_time_count;
		int china_limit_offline_time_tag;
		std::string mKuafuParam;
		int mKuafuResult;
		//主线任务id
		int mMainTaskState;
	public:
		DomainData(void);
		virtual ~DomainData(void);
		void Load();
		void Save(bool quit);
		void freshonlinetime();
	};
}