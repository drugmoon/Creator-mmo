#pragma once
#include "PlayActiveObject.h"
#include "leudgrid/MemoryPool.h"
#include "TimerFix.h"
#include <set>
#include "SubFriend.h"
namespace OGSLib
{
	enum JobType{
		JOB_WARRIOR=100,
		JOB_WIZARD=101,
		JOB_TAOIST=102,
		JOB_NUMBER=3,
	};
	enum MoneyType{
		MONEY_TYPE_GAMEMONEY = 11,
		MONEY_TYPE_GAMEMONEY_BIND = 12,
		MONEY_TYPE_VCOIN = 13,
		MONEY_TYPE_VCOIN_BIND = 14,
	};
	enum GenderType{
		GENDER_MALE=200,
		GENDER_FEMALE=201,
		GENDER_NUMBER=2,
	};
	enum AttackModeType{
		ATTACK_MODE_TYPE_ALL=100,
		ATTACK_MODE_TYPE_PEACE=101,
		ATTACK_MODE_TYPE_GROUP=102,
		ATTACK_MODE_TYPE_GUILD=103,
		ATTACK_MODE_TYPE_PK=104,
		ATTACK_MODE_TYPE_TEAM=105,
		ATTACK_MODE_TYPE_END=106,
	};
	class SubItem;
	class SubGift;

	enum ReltionType
	{
		RELTYPE_FRIEND = 100,
		RELTYPE_ENEMY,
		RELTYPE_BLACK,
	};


	struct MiniPlayerInfo
	{
		std::map<int,int>* model;
		SubItem* item_weapon_id;
		SubItem* item_fashion_weapon_id;
		SubItem* item_cloth_id;
		SubItem* item_fashion_cloth_id;
		SubItem* item_hat_id;
		SubItem* item_nicklace_id;
		SubItem* item_glove1_id;
		SubItem* item_glove2_id;
		SubItem* item_ring1_id;
		SubItem* item_ring2_id;
		SubItem* item_boot_id;
		SubItem* item_belt_id;
		SubItem* item_huizhang;
		SubItem* item_hunqi;
		SubItem* item_xueshi;
		SubItem* item_wing;
		SubItem* item_zuoji;
		SubItem* item_tejie;
		SubItem* item_xuefu;
		SubItem* item_hudun;
		SubItem* item_baoshi;
		SubItem* item_hunzhu;

		SubItem* item_jade_pendant;
		SubItem* item_shield;
		SubItem* item_mirror_armour;
		SubItem* item_face_cloth;
		SubItem* item_dragon_heart;
		SubItem* item_wolfang;
		SubItem* item_dragon_bone;
		SubItem* item_catilla;

		MiniPlayerInfo()
		{
			item_weapon_id=0;
			item_fashion_weapon_id=0;
			item_cloth_id=0;
			item_fashion_cloth_id=0;
			item_hat_id=0;
			item_nicklace_id=0;
			item_glove1_id=0;
			item_glove2_id=0;
			item_ring1_id=0;
			item_ring2_id=0;
			item_boot_id=0;
			item_belt_id=0;
			item_huizhang=0;
			item_hunqi=0;
			item_xueshi=0;
			item_wing=0;
			item_zuoji=0;
			item_tejie=0;
			item_xuefu=0;
			item_hudun=0;
			item_baoshi=0;
			item_hunzhu=0;
			item_jade_pendant=0;
			item_shield=0;
			item_mirror_armour=0;
			item_face_cloth=0;
			item_dragon_heart=0;
			item_wolfang=0;
			item_dragon_bone=0;
			item_catilla=0;
		}
	};

	class GameSession;
	class PlayMap;
	class PlayMapItem;
	class DomainData;
	class PlayNpc;
	class SubItem;
	class SubSkill;
	class SubAction;
	class SubTask;
	class Guild;
	class GameServerLib;
	class PlaySlave;
	class ItemDef;
	class SubItem;
	class SubMail;
	class SkillDef;
	class PlayMonster;
	class GMCommand;
	class SubAchieve;
	class PlayDart2;
	class ScriptLuaPlayerBind;
	class DomainPlay :
		public PlayActiveObject,
		public LEUD::PoolMemoryBase< DomainPlay >
	{
		friend GameSession;
		friend GMCommand;
		friend ScriptLuaPlayerBind;
		GameSession* mSession;
		DomainData* mCharacter;

		typedef std::vector<PlayNpc*> NpcList;
		NpcList mNpcList;
	public:
		typedef std::vector<SubItem> ItemMap;
	private:
		ItemMap mItemList;
		bool mNeedClearItemList;
		ItemMap mChatTradeItemList;
		bool mNeedClearChatTradeItemList;
		
		ItemMap& GetChatTradeItemList(){ return mChatTradeItemList; }
		bool mNeedFreshItemList;
		time_t mFreshItemTime;

		typedef std::map<int, SubTask> TaskMap;
		TaskMap mTaskMap;

		FrindArray mFriendArray;
		time_t mFriendFreshTick;

		typedef std::map<int, SubAchieve> AchieveMap;
		AchieveMap mAchieveMap;
		int mAchievePoint;
		int mAchieveGameMoneyMax;
		time_t mOnlineTimeMax;

		typedef std::map<int, std::string> ParamMap;
		ParamMap mParams;
		typedef std::map<int, int> NumParamMap;
		NumParamMap mNumParams;
		ParamMap mTempParams;
		std::string mParamDefault;
		typedef std::map<int, double> TimerMap;
		TimerMap mTimer;

		typedef std::map<LEUD_INT64, SubMail> MailMap;
		MailMap mMailMap;

		typedef std::map<int, ScriptItemAttibuteParam> ScriptItemAttrParamMap;
		ScriptItemAttrParamMap mSIAP;
		ObjectAttribute mSAP;

		std::set<int> mItemDespSendList;
		std::vector<int> mSkillDespSendList;

		LEUD_INT64 mExp;
		LEUD_INT64 mExpNext;
		LEUD_INT64 mGameMoney;
		LEUD_INT64 mGameMoneyBind;
		int mVcoin;
		int mVcoinBind;
		int mVcoinGS;
		int mVcoinAccu;
		int mVipLevel;
		int mCapacity;
		int mVcoinAccuByMonth;
		int mLastCongzhiMonth;
		int mVcoinUsedByDay;
		int mVcoinUsedByMonth;
		int mLastVcoinUsedDay;
		int mNeedRecordDayVcoin;
		int mGender;
		int mJob;
		int mMount;
		int mMountTypeID;
		int mMountLock;
		int mMountLockUseSkill;
		int mShowMount;
		double mChangeMountTick;
		double mCaijiTick;
		int mAttackMode;
		int mLockAttackMode;
		int mLastSafeAreaID;
		std::string mLastSafeMap;
		int mLastSafeX;
		int mLastSafeY;

		int mCountDownTick;
		int mCountDownTag;

		int mCountClientSell;

		int mSlaveKuLouLevel;
		PlaySlave* mSlaveKuLou;
		int mSlaveShenShouLevel;
		PlaySlave* mSlaveShenShou;
		PlaySlave* mSlaves[5];
		PlaySlave* mSlaveHuwei;

		std::vector<PlaySlave*> mSlaveList;

		std::string mScriptEcho;

		std::string mTradeTarget;
		bool mTradeSubmited;
		int mTradeGameMoney;
		int mTradeVcoin;
		ItemMap mTradeItem;


		bool mStallStart;
		std::string mStallName;

		bool mBoothOpen;

		int mHonor;
		int mUpGradeLv;////强化等级
		int mUpGradeMinLv;////强化最低等级
		int mPKValue;
		double mPKValueTick;
		int mPKState;
		double mPKStateTick;

		int m_nFightPoint;

		int mDressRing;
		int mDressGlove;
		int mItemTalkSeed;
		int mPlayerTalkSeed;

		time_t mCreateTime;
		time_t mLastLoginTime;
		std::string mLastLoginIP;
		time_t mLastLogoutTime;

		double mActionCheckCount;
		time_t mActionCheckTick;
		double mSkillCheckCount;
		time_t mSkillCheckTick;

		double mItemUseTick;
		time_t mItemUseAddHPTick;
		int    mItemuseAddHPCount;
		double mNoticeTick;
		int mNoticeID;

		int mDepotSlotAdd;
		int mBagSlotAdd;

		//int mCoupleID;
		//int mWeddingdate;
		//int mMarryTimes;

		double mSaveTick;
		double mFreshVcoinTick;

		int mTalkProhibited;

		double mSecondTick;
		double mSecondTickGap;
		double mMiniTick;
		double mMiniTickGap;
		double mYuanqiBuffTick;

		double mDartTick;

		std::string mNamePre;
		std::string mNamePro;

		double mMapEnterTick;

		std::map<int, int> mModelMap;

		int china_limit_lv;
		time_t china_limit_lv_tick;

		int mTeamID;
		char mTeamName[128];
		char mSeedName[256];

		time_t mUpdateChartGamemoneyTimeTick;
		LEUD_INT64 mUpdateChartGamemoneyTick;
		int mGMLevel;
		int mGMHideTalk;

		int mEnteringMap;

		int mAchieveFirstKillMonster;
		int mAchieveFirstAddFriend;
		int mAchieveFirstGroup;
		int mAchieveFirstGuild;
		int mAchieveFirstPKKill;
		int mAchieveFirstUpdEquip;
		int mAchieveFirstMergeSteel;

		bool mEquipMoXueShi;
		double mUpdateHPMPStatusInc;

		int mPKconfirm;
		int mCollectTarget;

		int mTotalUpdLevel;
		bool mShowFashion;
		int mLockShow;
		int mLockAction;
		std::string mLockActionCallback;

		int mMonShow;
		std::string mMonshowName;
		int mMonshowId;
		int mMonShowResid;

		int mTili;
		double mTiliRecoverTick;

		double mPowerRecoverTick;
		int mPowerRecoverGap;
		//0~10000,万分比
		int mPowerRecoverPercent;
		int mPowerRecoverVal;

		int mPowerRecoverSpeed;
		int mFabaoResID;
		int mGuildPt;
		int mGuildSpt;

		std::string mGuildName;
		PlayActiveObject* mFirstTarget;

		// 好友申请表
		typedef std::map<std::string, double> FriendApplyMap;
		FriendApplyMap mFriendApplyMap;
		// 申请别人为好友操作时间
		double mApplyFriendTick;

		// 拉取寄售列表
		double mGetConsignableItemsTick;
		// 购买寄售物品时间
		double mBuyConsignableItemsTick;
		// 当前任务怪id
		int m_nTaskMonId;
		// 被击杀助攻表
		std::map<int, double> m_mapDeadAssist;

		//主线任务id
		int mMainTaskState;

		// 额外的功能id
		std::vector<int> m_extraObjFuncs;

		//玩家的归属怪物
		std::vector<int> mBelongMons;
	public:
		PlayDart2* mDart2;
		int mAcceptDartTaskNumberUsed;
		// int mRobDartNumberUsed;
		//int mFreeDirectFly;
		int mPuppetMul;

		struct actionset
		{
			std::string id;
			std::string attr;
			std::string value;
		};
		typedef std::vector<actionset> ActionSet;
		typedef std::map<std::string, ActionSet> FKActionSet;
		FKActionSet mFKActionSet;
		void SetActionNode(std::string panel, std::string id, std::string attr, std::string value);
		ActionSet* GetActionSet(std::string panel)
		{
			FKActionSet::iterator p = mFKActionSet.find(panel);
			if (p != mFKActionSet.end())
			{
				return &p->second;
			}
			return 0;
		}

		typedef std::vector<SubItem> ItemList;
		ItemMap mGiftList;
		ItemMap& GetGiftList(){ return mGiftList; }
		ItemList mMySaleList;
		ItemList& MySaleList(){ return mMySaleList; }

		MailMap& GetMailMap(){ return mMailMap; }

		std::string mTitleName;
	public://properties
		DomainPlay(GameSession* session);
		virtual ~DomainPlay(void);

		GameSession* Session(){ return mSession; }

		bool check_action(double dura);
		bool check_skill(double dura);

		virtual const char* Name();
		virtual const char* SeedName();
		virtual const char* LoverName();
		int setName(const char* name);
		virtual int FightPoint();
		void FightPoint(int nFight);

		ParamMap& GetParamVar(){ return mParams; }
		ScriptItemAttibuteParam* get_siap(int pos)
		{
			ScriptItemAttrParamMap::iterator p = mSIAP.find(pos);
			if (p != mSIAP.end())
			{
				return &p->second;
			}
			return 0;
		}
		void set_siap(int pos, ScriptItemAttibuteParam& siap)
		{
			mSIAP[pos] = siap;//TODO 通知到客户端显示出来
		}
		ObjectAttribute* SAP(){ return &mSAP; }

		LEUD_INT64 exp(){ return mExp; }
		LEUD_INT64 exp(LEUD_INT64 exp){ return mExp = exp; }
		LEUD_INT64 exp_next(){ return mExpNext; }
		LEUD_INT64 gamemoney(){ return mGameMoney; }
		LEUD_INT64 gamemoney(LEUD_INT64 gm);
		LEUD_INT64 gamemoneybind(){ return mGameMoneyBind; }
		int vcoin(){ return mVcoin; }
		int vcoings(){ return mVcoinGS; }
		int vcoinbind(){ return mVcoinBind; }
		int vcoinaccu(){ return mVcoinAccu; }
		int viplevel(){ return mVipLevel; }
		int capacity(){ return mCapacity; }
		int VcoinAccuByMonth(){ return mVcoinAccuByMonth; }
		int LastCongzhiMonth(){ return mLastCongzhiMonth; }
		int VcoinUsedByDay(){ return mVcoinUsedByDay; }
		int VcoinUsedByMonth(){ return mVcoinUsedByMonth; }
		int LastVcoinUsedDay(){ return mLastVcoinUsedDay; }
		int job(){ return mJob; }
		int gender(){ return mGender; }
		int mount(){ return mMount; }
		int show_mount(){ return mShowMount; }
		int mount_lock(){ return mMountLock; }
		int mount_type_id(){ return mMountTypeID; }
		int mount_lock_skill(){ return mMountLockUseSkill; }
		void mount_lock_skill(int v){ mMountLockUseSkill = v; }
		int mon_show() { return mMonShow; }
		std::string mon_showName() { return mMonshowName; }
		int mon_showResid() { return mMonShowResid; }
		int AttackMode(){ return mAttackMode; }
		int AttackMode(int attack_mode);
		int LockAttackMode(){ return this->mLockAttackMode; }
		int LockAttackMode(int attack_mode){ return mLockAttackMode = attack_mode; }
		GameServerLib* gsl(){ return mGSL; }
		int PKState(){ return mPKState; }
		int PKValue(){ return mPKValue; }
		time_t CreateTime(){ return mCreateTime; }
		time_t LastLoginTime(){ return mLastLoginTime; }
		std::string LastLoginIP(){ return mLastLoginIP; }
		time_t LastLogoutTime(){ return mLastLogoutTime; }
		time_t OnlineTime();
		time_t AllOnlineTime();
		time_t TodayOnlineTime();
		time_t YestodayOnlineTime();
		int depot_slot_add(){ return mDepotSlotAdd; }
		int bag_slot_add(){ return mBagSlotAdd; }
		int lock_action(){ return mLockAction; }
		std::string& lock_action_callback(){ return mLockActionCallback; }
		
		PlaySlave * GetSlave(int type);

		ParamMap& GetParamMap(){ return mParams; }

		int Honor(){ return mHonor; }
		void SetHonor(int honor);
		void AddHonor(int add);
		void SubHonor(int sub);

		int PowerRecoverVal() {return mPowerRecoverVal;}
		void PowerRecoverVal(int val) {mPowerRecoverVal = val;}

		//int CoupleID(){ return mCoupleID; }
		//void CoupleID(int id){ mCoupleID = id; }
		//int Weddingdate(){ return mWeddingdate; }
		//void Weddingdate(int date){ mWeddingdate = date; }
		//int MarryTimes(){ return mMarryTimes; }
		//void addMarryTimes(){ mMarryTimes++; }
		//void clearMarryTimes(){ mMarryTimes = 0; }

		int TotalUpdLevel(){ return mTotalUpdLevel; }
		void TotalUpdLevel(int upd){ mTotalUpdLevel = upd; }
		void SetNeedClearItemList(bool b) { mNeedClearItemList = b; } 
		bool showfashion(){ return mShowFashion; }
		bool changeShowFashion(int show);

		int GroupID();
		const std::string& GroupLeader();
		bool IsGroupMember(int id);
		int GroupSize();
		void GroupExe(const char* code);
		int GroupMemberOnLineNum();
		const std::string GuildID();
		const std::string& GuildName();
		bool IsWarGuildMember(DomainPlay * src);
		std::string& GuildName1(){ return mGuildName; }
		int GuildSize();
		void GuildExe(const char* code);
		void FriendExe(const char* code);
		int FriendNum();
		int GuildTitle();
		int SubGuildPt(int pt);
		void AddGuildPt(int pt);
		int GuildPt(){ return mGuildPt; }
		int GuildSpt(){ return mGuildSpt; }
		void SetGuildPt(int pt){ mGuildPt = pt; }
		void SetGuildSpt(int pt){ mGuildSpt = pt; }
		int GetUpGradeLv() { return mUpGradeLv; }
		int GetUpGradeMinLv(){ return mUpGradeMinLv; }
		int GuildEnterTime();
		void OnEnterGroup();
		void OnLeaveGroup();
		void OnEnterGuild();
		int CheckCreateLvGuild(int lv);
		void OnPKKill();
		void OnUpdEquip();
		void OnMergeSteel();
		const char* LoginID();
		int StartCollect(int srcid);
		int EndCollect();
		int CollectBreak();
		int ChangeMountBreak();
		//int DirectFly(int target_id);
		void ChangeHpMp(int hp,int mp,int flag);
		//int free_direct_fly(){ return mFreeDirectFly; }
		//int free_direct_fly(int freedirect_fly);
		//void subfreefly(int t);
		//void addfreefly(int t);
		//void LotteryTimes(int times);
		int get_empty_item_position_number_in_lotterydepot();
		int get_empty_item_position_number();
		void SetMaxPower(int power, int needfresh, int powper);
		void SetTaskMonID(int id);
		int GetTaskMonID();
	public://call by internal systems
		int initPlayer(void);
		int destPlayer(void);

		ItemMap& GetItemList(){ return mItemList; }

		bool AddGameMoney(LEUD_INT64 number, int src);
		bool RemGameMoney(LEUD_INT64 number, int src);
		bool AddGameMoneyBind(LEUD_INT64 number, int src);
		bool RemGameMoneyBind(LEUD_INT64 number, int src);
		bool UseGameMoney(LEUD_INT64 num, int src);
		bool AddVcoin(int number, int src, const std::string& desp="");
		bool AddVcoinGS(int number, int src = 666, const std::string& desp="");
		bool RemVcoin(int number, int src, const std::string& desp="");
		bool AddVcoinBind(int number, int src);
		bool RemVcoinBind(int number, int src);
		bool AddCapacity(int num, int src);
		bool UseCapacity(int num, int src);
		std::string& Param(int id);
		void Param(int id, std::string& value);
		void Param(int id, int value);
		std::string& TempParam(int id);
		void TempParam(int id, std::string& value);
		void PushParamData();

		virtual LEUD_INT64 AddExperience(int exp, int src);
		virtual LEUD_INT64 RemExperience(int exp, int src);
		int CheckLevelUp();
		int SetLevel(int lv,bool log,const std::string& desp="");
		int SetUpdateLevel(int level);
		LEUD_INT64 WinExpByMonster(int exp);
		int OnKillMonster(PlayMonster* pm);


		void GMSetTaskState(int task_id, int id, int state);

		int SetPKState(int state);
		int AddPKValue(int value);
		int SubPKValue(int value);
		int SetPKValue(int value);

		int GetMiniPlayerInfo(MiniPlayerInfo* info);

		//添加并穿戴装备（去除先入背包这一步）
		int AddAndDressEquip(SubItem* ii, int src, int onPos, int show_flags);
		int AddItem(SubItem* ii, int number, int src, int bag_full_action, int show_flags, int *pos = 0);
		int AddItem_Lottery(SubItem* ii, int number, int src, int bag_full_action, int show_flags, int *pos = 0);
		int AddXuanJing(SubItem* ii, int number, int src, int bag_full_action, int show_flags, int *pos = 0);
		int AddFashion(SubItem* ii, int number, int src, int bag_full_action, int show_flags, int *pos = 0);
		int RemItemDuration(int sub_type, int number, int flags);
		int RemItemNumber(int type_id, int number, int flags, int src, int bind = -1);
		int RemItemPosition(int type_id, int position, int number, int src);
		int CheckItemNumber(int type_id, int number);
		int CheckItemPosition(int type_id, int position);
		int NumItem(int type_id, int flags, int bind = -1);
		int NumBagBlack();
		int NumBag();
		int NumXjBlack();
		int use_item_by_type(int type_id);
		int UseSomeBS(int bslv);
		SubItem* get_item_by_pos_type(int position, int type_id);
		SubItem* get_item_by_pos(int position);
		SubItem* get_saleitem_by_pos(int position);
		int push_item_change_by_pos_type(int position, int type_id);
		int get_item_type_by_pos(int position);
		int find_empty_item_position();
		int find_empty_item_position_fashion();
		int CheckItemFashion(int id);
		int CheckItemFashion2(int id);
		int CheckFashionPosNum();
		int Clear_Item();
		int DropItemByTypeID(int type_id);

		virtual int AddSkill(int type_id, int level);
		int AddSkillExp(int type_id, int exp);

		bool find_validate_map_position(int* x, int* y);
		bool get_home_map_position(std::string& map_id, int* x, int* y);
		bool go_home();

		SubTask* GetTask(int task_id);
		int SetTaskState(int task_id, int task_state);
		int SetTaskParam(int task_id, int param_id, int param);
		int GetTaskState(int task_id);
		int GetTaskParam(int task_id, int param_id);
		int CancelTask(int task_id);
		void UpdateTaskName(int task_id);

		int GetAchieveState(int achieve_id);
		int SetAchieveState(int achieve_id, int state);
		int GetAchieveParam(int achieve_id);
		int SetAchieveParam(int achieve_id, int param);
		int AddOneAchieveState(int achieve_id);
		int AddOneAchieveParam(int achieve_id);
		int GetAchievePoint();
		int AddAchievePoint(int ap);

		void UpdateAllRelation();
		void PushAllItemData();
		void PushItemData();
		void PushFriendData();
		bool IsFriend(const char* name);
		bool IsEnemy(const char* name);
		int GetRelationShip(const char* name);
		void PushDataToAllForRank(int param, int num);
		void PushTradeItemData();
		void PushChatTradeItemData();
		void PushChatTradeTargetItemData(std::string target_name, int item_pos);///获取目标玩家的物品数据
		void PushItemInfo(int type_id);
		void PushItemInfo(const char* item_name);
		void PushStatusDef();
		void PushBuffDef();
		void PushSkillData();
		void PushSkillDefine(int type_id, int level);
		void PushTaskData(int task_id = -1, int flags = 0);
		void PushShortcutData();
		void UpdateAvatar();
		void SetMountInfo(int lock, int mount, int itemtypeid);

		void SwitchGhost(int ghost, int level, int exp, int gamemoney);

		void AlertMessage(int lv, int flags, const char* msg);

		void DieDropItem();
		void Relive(int relive_type, const char* map_name, int x, int y);

		virtual void CalculateAttribute();

		virtual void CalculateExtraObjFuncAttribute();
		virtual void ClearBuffPropPercent();
		virtual void OnFrameTimer(int param);

		virtual void ForceMove(int x, int y, int dir);
		virtual void Attack(PlayActiveObject* des, SkillDef * sd, int pawer, int flags, int delay);
		virtual int  Attacked(PlayActiveObject* src, SkillDef * sd, int pawer, int flags, int delay);
		virtual bool IsProperAttackDes(PlayActiveObject* des, SkillDef * sd);
		virtual bool IsProperFriendDes(PlayActiveObject* des, int skill_type);
		virtual bool CheckHit(PlayActiveObject* des, SkillDef * sd);

		void Update(double time);
		void UpdateStatusTimer(double time);
		void UpdateHPMPInc(double time);

		void FreshVcoin();
		void FreshGift();
		int ScriptEcho(const char* msg);
		int NPCShowFlags(unsigned int npc_id, int flags);
		int RefreshNpcShowFlagsInview();
		PlayNpc* FindNPCInview(int id);

		void pushGroupInfo(const std::string& group_name, const std::string& group_leader);
		bool checkCreateGuild();
		bool checkJoinGuild();
		bool checkUseFashion(int id);
		void ReallyCreateGuild();
		void conditionCreateGuild(std::string& conditional);
		void findRoadGoto(const char* map, int x, int y, const char* target, int flags);
		void HpMpChangeBroadcast(int hpChange, int mpChange);
		void GuildChangeBroadcast();
		//bool checkDirectFly();

		int checkItemWithExp(int exp);
		int SetMapTimer(int i, int esp);
		double GetMapTimer(int i);

		int GetWeaponLuck();
		int SetWeaponLuck(int luck);
		int ChangeWeaponLuck(int change);

		//int notifyLoginItemList();

		int CheckEquiped(int pos);
		void NotifyItemChange(OGSLib::SubItem item,int show_flags);
		void NotifyMiniNpc();
		void NotifyCountDown(int delay, std::string& msg);
		virtual void publicPlayEffect(int effect_type, int x, int y, unsigned int targetID, unsigned int ownerID, int resID);

		void TalkProhibited(int value);
		int  TalkProhibited();

		int SetModel(int id, int vl);
		int GetModel(int id);

		void LogItemChange(int src, int act, SubItem* item);

		void SetNamePre(const std::string& namepre);
		void SetNamePro(const std::string& namepro);
		const std::string& GetNamePre();
		const std::string& GetNamePro();

		GameServerLib* GetGSL(){ return mGSL; }

		void FreshPlayerInfo();

		int FreshTimeItem(time_t t);

		int TeamID() {return mTeamID;}
		const char* TeamName(){ return mTeamName; }
		void SetTeamInfo(int team, const char* teamname);

		void ExeCode(const char* code);
		void ExeCodeClient(const char* param);
		void ShowVipInfo(int lv);

		int LoadPostMail();
		void ReadMail(LEUD_INT64 id);
		int RecieveMailItem(LEUD_INT64 id);
		void DeleteMail(LEUD_INT64 id);

		void ProcessGift();
		// 删除关系,bAct == true 主动
		bool DelRelationShip(const char* name, bool bAct);
		// 改变好友关系,没有就添加
		void ChangeRelationShip(const char* name, int title, bool bAct);
		// 其他玩家发送请求
		bool PlayerApplyFriendRequest(const char* name);
		void AgreeFriendApply(const char* name, int agree);
		void NotifyRelationInfoChange(int off_line = 0);
		void RelationInfoChange(DomainPlay * src, int off_line = 0);

		void FriendChange(const char* name, int title);
		void FriendFresh();

		void ChangeJob(int new_job, int undress_item);
		void ChangeGender(int new_gender, int undress_item);
		
		int GetGMHideTalk(){ return mGMHideTalk; }
		int GetGMLevel(){ return mGMLevel; }

		void checkZhaoHuan();
		//void gen_slave(int id, std::string mon_name, int ai_type);
		void gen_slave(int id, int mon_id,int ai_type);
		void kill_slave(int id);
		int have_slave(int id);
		void clear_slave();

		PlaySlave* GetSlaveByID(int id);

		int gen_slave_fashi(int mon_id, int x = 0, int y = 0, const char* flags = "");
		int kill_slave_fashi(int id);
		void clear_slave_fashi();
		//刷新雕像类slave，（避免被自己的slave攻击，又能被怪物锁定）
		int GenSlaveStatue(int mon_id, int x, int y, const char* flags);

		void SetFirstTarget(PlayActiveObject* target){ mFirstTarget = target; };
		// PlayActiveObject* GetFirstTarget(){return mFirstTarget; };

		void FreshOnlineTime();
		void GenDart(int def_id);
		int HaveDart();
		void KillDart();
		int GetDartID();
		bool BindDart(int id);

		int GoMapConn();
		void set_showmount(int i);
		void lock_action(int lockaction,const char* script);
		void lock_action_end();
		int SetZsLevel(int zslevel);
		void setHpMpPercent(int percent);
		void EnterTrap(PlayActiveObject* src, int id);
		void LeaveTrap(PlayActiveObject* src, int id);
		int ItemUpgrade(int pos,int updac,int updmac,int upddc,int updmc,int updsc);
		//新增直接设置物品强化等级并记录
		int ItemUpgrade(int pos,int type_id, int level);

		int ItemAddUpdFailedCount(int pos,int count=1);
		int ItemSubUpdFailedCount(int pos,int count=1);
		int SetSwordLv(int lv);
		int SetMonPointLv(int lv);
		void BecomeMonster(int mon_show,std::string mon);
		int SetWingLv(int lv, bool updateChart = true);
		void SetTianGuanLv(int lv);
		void UpdataChart(int winglv, int tianguanlv);
		int GetWingLv(){ if (mHair)return mHair-1000; return -10000;}
		int ItemZhuLing(int pos,int flag);

		void setMainTaskState(int state){ mMainTaskState = state; };

		void AddExtraObjFuncs(int id);
		void RemExtraObjFuncs(int id);

		void AddBelongMonster(int monsterId);
		void ClearBelongMonster();
	protected://call by self
		virtual void OnHurt(PlayActiveObject* src,int change,int delay,int change_power);
		virtual void OnDead(PlayActiveObject* src,int delay);
		
		virtual void OnAttack(PlayActiveObject* des,SkillDef * def,int pawer,int flags,int delay);
		virtual void OnLeaveSafeArea();
		virtual void OnEnterSafeArea(int sa);
		virtual void OnEnterEventArea(std::string id);
		virtual void OnLeaveEventArea(std::string id);
	private://help functions
		void CalculateBaseAttribute();
		void WarriorCalculateBaseAttribute();
		void WizardCalculateBaseAttribute();
		void TaoistCalculateBaseAttribute();

		void CalculateBaseAttribute3();
		//新版配置表读取玩家基础属性levelinfo.csv
		void CalculateItemAttribute();
		void CalculateSwordAttribute();
		void CalculateMonPointAttribute();
		void CalculateUpGradeAttribute();
		void CalculateGemAttribute();
		void CalculateMonsterAttribute();
		void CalculateSkillAttribute();
		void CalculateReinAttribute();
		void CalculateScriptAttribute();
		void CalculateTotalUpdAttribute();
		void CalculateTitleAttribute();
		void CalculateFightPoint();
		void Load();
		void UpdateAfterCalculateAttribute();
		void SaveExitMapPosition();
		void Save();
		void TrainSkill(int skill_type,int exp);

		void bag_use_item_dissipative(ItemMap::iterator& pos,int num = 1);
		//to装备目标位(仅限戒指和手镯)
		void bag_use_item_equipment(ItemMap::iterator& pos, int to=0);
		int bag_use_item_equipment_check_limit(ItemMap::iterator& pos);
		int bag_use_item_equipment_check_limit(ItemDef* id);
		int bag_use_item_gem(ItemMap::iterator& pos,int equip_pos);
		

		int find_empty_item_position_in_stall();
		int find_empty_item_position_in_depot();
		int find_empty_item_position_in_lotterydepot();
		int find_empty_item_position_in_xuanjing();
		int get_empty_item_position_number_in_ChatTrade();///获得聊天交易中剩余的格子数目
		int find_empty_item_position_in_ChatTrade();////获得聊天交易中空余的位置position
		//int get_empty_pos_in_salelist();	

		void DamageWeaponDuration(int dura_damage);
		void DamageLoadEquipment(int dura_damage);
		bool DamageSkillMP(SkillDef* sd,SubSkill* pos);

		int  WarriorAttack(SkillDef* sd,int paramX,int paramY,unsigned int paramID);

		void StartGongShaJianShu(SubSkill* subskill);
		void StopGongShaJianShu(SubSkill* subskill);
		bool IsGongShaJianShuOnway(SubSkill* subskill);

		void OpenCiShaJianShu();
		void OpenBanYueWanDao();
		bool IsCiShaJianShuOpen(SubSkill* subskill);
		bool IsBanYueWanDaoOpen(SubSkill* subskill);

		void CheckLieHuoValid();
		bool StartLieHuoJianFa(SubSkill* subskill,bool check);
		void StopLieHuoJianFa(SubSkill* subskill);
		bool IsLieHuoJianFaOnway(SubSkill* subskill);
		bool IsLieHuoNear(SubSkill* subskill);

		bool StartYeManChongZhuang(SubSkill* subskill,bool check);
		void StopYeManChongZhuang(SubSkill* subskill);
		bool IsYeManChongZhuangOnway(SubSkill* subskill);

		int ActionDiYuHuoAttack(int ActionKey,SubAction* ai);
		int ActionShunJianYiDongAttack(SubAction* ai);
		//int ActionHuoQiangAttack(int ActionKey,SubAction* ai);
		void ActionZhaoHuanKuLouBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionZhaoHuanYueLingBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionZhaoHuanShenShouBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionJumpBegin(SkillDef* sd,int* targetx,int* targety,unsigned int paramID);

	public://call from client
		int MapChat(char* msg);
		int HornChat(char* msg);
		int PrivateChat(DomainPlay* player,char* msg);
		int NormalChat(char* msg);

		int EnterMap(const std::string& map_id,int x,int y,int size=1);
		int LeaveMap();
		int MapRemoveOwner();
		int HandleMapOwner(bool result);
		int GoMapConn(const std::string& map_id,int x,int y,int size=1);
		int EnterKuafuMap();
		int setKuafujlstate(int state);

		virtual int Turn(int dir);
		int Walk(int dir,int nx,int ny,int tag);
		int Run(int dir,int nx,int ny,int tag);

		int NPCTalk(unsigned int npcid,const char* param);
		int NPCShop(unsigned int npcid,int page);
		int NPCBuy(unsigned int npcid,int page,int pos,int good_id,int type_id,int number);
		int NPCSell(unsigned int npc_id,int pos,int type_id,int number,int flag);
		int VcoinShopList(int shop_id,int flags);
		int NPCRepairItem(unsigned int npcid,int pos,int type_id,int flags);
		int ReqairAllItem(int flags);
		int TaskClick(int task_id,const char* param);
		int ItemSell(int pos,int type_id,int number=1);

		int QueryTaskDesp(int task_id);
		int InfoPlayer(const char* player_name);
		int InfoRobot(const char* robot_name);
		int CountDownFinish();

		int PickUp(unsigned int itemid,int x=0,int y=0);

		//void BoothAdd(int pos,int type_id,int pay_type,int pay_price);
		//void BoothClose();
		//to 目标pos位置（仅限戒指和手镯）
		int BagUseItem(int position,int type_id,int num=1, int to = 0);
		int bag_use_item_fashion(int pos, int id);
		int UndressItem(int position);
		//宝石相关
		int BagUseGem(int equip_pos, int gem_pos);
		int UndressGem(int position);

		int ItemPositionExchange(int from,int to,int flags=0,int issort=0);
		int ItemBagDepotExchange(int from,int number,int flag);
		int DropItem(int pos,int type_id,int number=1);
		int SplitItem(int pos,int type_id,int number);
		int DestoryItem(int pos,int type_id);
		int SortItem(int flags);
		int ItemTalk(int item_id,int seed,const char* param);
		//int MergeSteel(int pos1,int pos2,int pos3,int posadd,int paytype);
		//int UpgradeEquip(int posEquip,int posSteel,int posAdd,int paytype);
		//int EquipReRandAdd(int posEquip,int posAdd);
		//int EquipExchangeUpgrade(int posEquipFrom,int posEquipTo,int posAdd,int paytype);
		int ItemProtect(int posEquip,int type_id);
		//int SteelEquip(int posEquip,int type_id);
		//int InfoItemExchange(int pos,int type_id,int exchange_type);
		//int ItemExchange(int pos,int type_id,int exchange_type);
		//int ClientMergeEquip(int target_id,int param1,int param2,int param3,int param4,int param5,int param6);
		//int InfoUpEquip(int pos,int id);
		//int UpEquipItem(int pos,int id,int money,int hunzhi,int vcoin,int ex_flag);
		//int MergeFashionEquip(int id,int pos,int pos1,int pos2);
		// 寄售物品
		int ConsignItem(int pos, int count, int price, int time_out);
		// 获取寄售物品列表
		int GetConsignableItems(int type, int begin_index, int job, int condition, std::string filter);
		// 购买寄售物品
		int BuyConsignableItem(int seed_id);
		// 取回寄售物品
		int TakeBackConsignableItem(int seed_id);
		// 取回寄售行元宝
		int TakeBackVCoin();

		void AddDropProb(int prob);
		void SubDropProb(int prob);
		void AddDoubleAttProb(int prob);
		void SubDoubleAttProb(int prob);
		int GetDropProb();
		int GetDoubleAttProb();

		int GMTalk(const char* param);
		void SetSkillLevel(int skill,int level);
		int CheckSkillLevelUp(int  skill);

		int PlayerTalk(int seed,const char* param);

		int UseSkill(int skill_type,int paramX,int paramY,unsigned int paramID);

		int lockshow(){return mLockAction;}
		int ChangeCloth(int lock,int cloth,int weapon);
		int ChangeMount();
		int FaBaoRes(){return mFabaoResID;}
		int Fashion(){return mFashion;}

		void SetFaBaoRes(int jie){mFabaoResID = jie;}
		int ChangeFaBaoRes(int jie);
		int ChangeAttackMode(int attack_mode);

		int TradeInvite(const char* invitee);
		int AgreeTradeInvite(const char* inviter);
		int CloseTrade(const char* target);
		int TradeAddGameMoney(int value);
		int TradeAddVcoin(int value);
		int TradeSubmit();

		int TradeAddItem(int pos,int item_type);

		int ChatTradeAddItem(int pos,int item_type,int vcoin_num);////交易发起者添加交易的物品
		int ChatTradeSubItem(int pos,int item_type);////交易发起者取消交易的物品
		int ChatTradeSubmit(const std::string& inviter, int pos, int type, int lock);////交易接收完成交易
		int ShowTradeItem(const std::string& inviter,int pos,int type,int lock);

		int CheckOnGroup();

		int PushLuaTable(const char* function,const char* table);
		
		int ClientFreshVcoin();

		int AddDepotSlot();

		int SwithSlaveAIMode();
		
		void OnFreshVcoin(int id,int accu,bool gs=false);
		void OnFreshGift(std::vector<OGSLib::SubGift>* gift);

		void UpdateChinaLimit(const char* id,const char* name,int china_id_ok);
		void BuyOfflineExp(int mul);

		void UpdateChartData(int chart_type,int param);

		void AddBagFromLua(int num);
		void addDepotFromLua(int num);

		void AddBagSlot();
		void MapEnterTick(int entertick){mMapEnterTick = entertick;}

		void PostMail(std::string& title,std::string& content,const std::map<int,int>& items);
		void PostMailToAll(std::string& title,std::string& content,const std::map<int,int>& items);

		int AddStallItem(int pos,int price_type,int price);
		int RemStallItem(int pos);
		int StartStall(const char* stallname);
		void StopStall();
		void ViewStall(DomainPlay* des);
		int BuyStallItem(DomainPlay* des,int pos);
		void changeSlaveState(int state);

	public://call for send msg to client
		virtual void NotifyMapEnter(PlayMap* map);
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void NotifyMapLeave(PlayMap* map);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src);

		virtual void ResRun(int dir,int newx,int newy);

		virtual void NotifyWalk(PlayMapObject* src,int dir,int newx,int newy);
		virtual void NotifyRun(PlayMapObject* src,int dir,int newx,int newy);
		virtual void NotifyTurn(PlayMapObject* src,int dir);
		virtual void NotifyAttack(PlayMapObject* src,int param);
		virtual void NotifyAvatarChange(PlayMapObject* src);
		virtual void notifySpeed(PlayMapObject* src,float speed);
		virtual void notifyDoAction(PlayMapObject* src,int action);

		virtual void NotifyInjury(PlayMapObject* src,int attacker,int change,int delay,int effect,int change_power);
		virtual void NotifyDie(PlayMapObject* src,int delay);
		virtual void NotifyRelive(PlayMapObject* src,int type);
		virtual void NotifyHPMPChange(PlayActiveObject* src,int hp_change,int mp_change);
		virtual void NotifySatusChange(PlayActiveObject* src,int status_id);
		virtual void NotifyBuffChange(PlayActiveObject* src,int id, int op_code, double time_left);

		virtual void NotifyMapChat(PlayMap* map,PlayMapObject* src,char* msg);
		virtual void NotifyWorldChat(DomainPlay* src,char* msg);

		virtual void NotifyUseSkill(PlayMapObject* src,int skill_type,int paramX,int paramY,unsigned int paramID,int resID,int dir, int skill_level = 1);
		virtual void NotifyForceMove(PlayMapObject* src,int x,int y,int dir);
		virtual void NotifyMonExpHiterChange(PlayMapObject* src, DomainPlay * play);
		virtual void NotifyMonExpHiterChange(PlayMapObject* src, int hiter_id, const char * name);
		virtual void NotifyShowProgressBar(int duration,const char* msg);

		virtual void NotifyPrivateChat(DomainPlay* src,char* msg);
		virtual void NotifyNormalChat(DomainPlay* src,char* msg);
		//virtual void NotifyMiniNpc(DomainPlay* src,PlayMap* map);
		virtual void NotifyMeetItem(DomainPlay* src, PlayMapItem* item);
		virtual void NotifyNPCTalk(PlayNpc* src,int flags,int param,const std::string& msg);
		virtual void NotifyNPCShop(PlayNpc* src,std::string msg,int page);
		virtual void NotifyGroupInfo(DomainPlay* src,const std::string& group_name,const std::string& group_leader);
		virtual void NotifyTradeInvite(DomainPlay* src);
		virtual void NotifyPKStateChange(DomainPlay* src);
		virtual void NotifyPlayerAddinfo(DomainPlay* src);
		virtual void NotifyAttributeChange();
		virtual void NotifyPlayEffect(PlayMapObject* src,int skill_type,int x,int y,unsigned int targetID,unsigned int ownerID,int resID);
		virtual void NotifyMapOption(PlayMap* map);
		virtual void NotifyNameAdd(DomainPlay* src,const std::string& namepre,const std::string& namepro);
		virtual void NotifyTeamInfo(DomainPlay* src);
		virtual void NotifyMonsterChat(PlayMonster* src,const char* msg);
		//virtual void NotifyGiftList(std::string msg);
		virtual void NotifyPushLuaTable(std::string typee,int flag,std::string table);
		virtual void NotifyNewMailNum(int num);
		virtual void NotifyMailReceiveSuccess(LEUD_INT64 mailID);
		virtual void notifyKuafuComeBackData();

		virtual void NotifyStallChange(DomainPlay* src,int x,int y,bool stall,const std::string& stallname);
		virtual void NotifyStallItemInfo();
		virtual	void NotifyChargeDartList();
		virtual	void NotifyGuildItemList();
		virtual	void NotifyTiliChange(PlayActiveObject* src);
		virtual void NotifyGhostGuildInfo(DomainPlay* src);
	};
}