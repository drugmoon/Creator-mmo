#pragma once
#include "ConstsString.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) do{if(p) {delete (p); (p) = 0;} } while(0)
#endif
#ifndef SAFE_DELETE_AR
#define SAFE_DELETE_AR(p) do{if(p) {delete[] (p); (p) = 0;} } while(0)
#endif

namespace LEUD
{
	class CryptographyRSA;
}
namespace OGSLib
{
	class ManagerMap;
	class GameSession;
	class TimerFrame;
	class TimerSecond;
	class ScriptLua;
	class ManagerItemDef;
	class ManagerSkillDef;
	class ManagerRobotDef;
	class GMCommand;
	class ManagerLevelInfo;
	class ManagerTotalAttr;
	class ManagerMonDef;
	class DomainData;
	class TimerFix;
	class GameSessionListener;
	class ManagerMonster;
	class ManagerGuild;
	class ManagerGroup;
	//class ManagerOnSale;
	class ManagerCouple;
	class ManagerRobot;
	class ManagerNotice;
	class ManagerFilter;
	class ManagerDenySell;
	class ManagerChart;
	class ManagerDeny;
	class ManagerTask;
	class ManagerAutoRun;
	//class ManagerReincarnation;
	class ManagerStatus;
	class PlayNpc;
	class DomainPlay;
	class GameServerLib;
	struct AutoRunDB;
	class SubGift;
	class ManagerFly;
	class Mail;
	class SubMail;
	class ManagerMail;
	class ManagerChargeDart;
	class ManagerSwordDef;
	class ManagerMonPointDef;
	class ManagerEquipUpgradeDef;
	class ManagerConsignment;
	class ManagerRelationCache;
	class RelationShipOper;
	class ConsignableItemInfo;
	class ManagerObjFuncs;

	enum AUTH_TYPE
	{
		AUTH_TYPE_DEMO=100,
		AUTH_TYPE_REAL=101,
		AUTH_TYPE_CHAT=102,
		AUTH_TYPE_SKUAFU=110,
		AUTH_TYPE_CKUAFU=120,
	};

	class GameServerLibListener
	{
	public:
		virtual int GetKuaFuService()=0;
		virtual void ConnectService(const char* ip,const char* port)=0;
	public:
		virtual void LoadCharacter(const char* account,const char* chrname,const char* seedname,const char* client_ip,int GUID)=0;
		virtual void LoadDemoCharacter(const char* account,const char* chrname,const char* client_ip,int GUID)=0;

		virtual void SaveCharacter(OGSLib::DomainData* character,bool quit,int GUID)=0;
		virtual void SaveDemoCharacter(OGSLib::DomainData* character,bool quit,int GUID)=0;
		virtual void SaveKuafuCharacter(std::string& charID,std::string& account,int vcoin,int kuafuResult)=0;

		virtual void LoadCouples(int GUID)=0;
		virtual void SaveCouples(const OGSLib::ManagerCouple& couple_manager,int GUID)=0;

		virtual void LoadGuilds(int GUID)=0;
		virtual void SaveGuilds(const OGSLib::ManagerGuild& guild_manager,int GUID)=0;

		virtual void LoadConsignment(int GUID) = 0;
		virtual void SaveConsignment(OGSLib::ManagerConsignment& consignment_manager,int GUID) = 0;

		virtual void LoadRelationCache(int GUID) = 0;
		virtual void SaveRelationCache(OGSLib::ManagerRelationCache& relationcache_manager,int GUID) = 0;

		virtual void LoadServerVar(int GUID)=0;
		virtual void SaveServerVar(const OGSLib::GameServerLib& gsl,int GUID)=0;

		virtual void LoadMails(int GUID)=0;
		virtual void SaveMails(OGSLib::ManagerMail& mail_manager,int GUID)=0;

		virtual void LoadAuto()=0;
		virtual void game_exe_sql(const char* sql)=0;

		virtual void RefreshVcoinCongzi(const std::string& account,const std::string& chrname)=0;
		virtual void RefreshGift(const std::string& chrname)=0;

		virtual void Log(const char* msg)=0;

		virtual void LoadGMAllowIP()=0;
		virtual void LoadConfig()=0;
		virtual void LoadKernelConfig()=0;
		virtual const char* LoadFile(const char* fn,size_t& size)=0;

		virtual void AcceptOff()=0;
		virtual void AcceptOn()=0;
		virtual void BreakNet()=0;
	};
	class GameServerLib
	{
		friend GameSession;
		enum GAME_SERVER_LIB_STATE
		{
			GAME_SERVER_LIB_STATE_NULL=100,
			GAME_SERVER_LIB_STATE_STARTING=101,
			GAME_SERVER_LIB_STATE_RUN=102,
			GAME_SERVER_LIB_STATE_ENDING=103,
			GAME_SERVER_LIB_STATE_END=104,
		};
		GAME_SERVER_LIB_STATE mState;
	private:
		typedef std::map<std::string,GameSession*> LoginIDSessionMap;
		typedef std::map<std::string,GameSession*> CharIDSessionMap;
		typedef std::map<std::string,GameSession*> SeedIDSessionMap;
		typedef std::map<std::string,GameSession*> ChatListenSessionMap;

		int mUpdateTick;
		double mUpdateTime;
		double mUpdateDuration;

		std::string mConfigFile;

		std::string mDataPath;
		std::string mConfigPath;
		std::string mMapPath;

		GameServerLibListener* mListener;

		ManagerMap* mMapManager;
		TimerFrame* mFrameTimer;
		TimerSecond* mSecondTimer;
		ScriptLua*  mScriptLua;
		ManagerItemDef* mItemDefineManager;
		ManagerSkillDef* mSkillDefineManager;
		ManagerSwordDef* mSwordDefManager;
		ManagerMonPointDef* mMonPointDefManager;
		ManagerEquipUpgradeDef* mEquipUpgradeDefManager;
		GMCommand* mGMCommand;
		ManagerLevelInfo* mLevelInfoManager;
		ManagerTotalAttr* mTotalAttrManager;
		ManagerMonDef* mMonDefManager;
		TimerFix* mTimerFix;
		ManagerMonster* mMonsterManager;
		ManagerCouple* mCoupleManager;
		ManagerGuild* mGuildManager;
		ManagerConsignment * mConsignmentManager;
		ManagerRelationCache * mRelationCacheManager;
		ManagerGroup* mGroupManager;
		ManagerNotice* mNoticeManager;
		ManagerFilter* mFilterManager;
		ManagerDenySell* mDenySellManager;
		ManagerChart* mChartManager;
		ManagerDeny* mDenyManager;
		ManagerTask* mTaskManager;
		ManagerAutoRun* mAutoRunManager;
		ManagerStatus* mStatusManager;
		ManagerObjFuncs *mObjFuncManager;
		ManagerFly* mFlyManager;
		PlayNpc* mShopNpc;
		ManagerMail* mMailManager;
		ManagerChargeDart* mChargeDartManager;

		LoginIDSessionMap mLoginIDSessionMap;
		SeedIDSessionMap mSeedIDSessionMap;
		size_t m_MaxSessionNum;
		CharIDSessionMap mCharIDSessionMap;
		ChatListenSessionMap mChatListenSessionMap;
		std::vector<GameSession*> mSessions;

		std::map<int,std::map<std::string,std::string> > mCallBackContent;
		int mGenGUID;
		time_t mCallBackContentClearTick;
		int mCallBackContentClearTag;

		std::map<std::string,std::string> mVars;
		std::map<std::string,std::string> mTempVars;
		std::string mVarDefault;

		int mWar;
		int mKuafu;

		int ServerStartDay;
		int MergeStartDay;

		int ActivityBegin;
		int ActivityEnd;
		int TodayData;

		bool show_server_start;
		bool show_merge_start;

		int mProsperity;
		int mProsperityNext;
		time_t mProsperityTick;
		time_t mProsperityGap;

		std::map<std::string,int> mKfStartPosition;
	public:
		ConstsString mStringConsts;
		std::string mServerName;
		std::string mServerTitle;
		std::string mServerHome;
		std::string mServerPlatform;
		int mServerID;

		std::string mStart_map;
		int mStart_x;
		int mStart_y;
		int mStart_size;
		std::string mWar_Start_map;
		int mWar_Start_x;
		int mWar_Start_y;
		int mWar_Start_size;
		std::string mRed_Start_map;
		int mRed_Start_x;
		int mRed_Start_y;
		int mRed_Start_size;
		int mPawMul;
		int mExpMul;
		int mPuppetExtMul;
		int mMonDropBind;
		int mMonDropUseBind;
		int mNpcBuyGameMoneyBind;
		int mNpcBuyGameMoneyUseBind;
		int mNpcBuyVcoinBind;
		int mNpcBuyVcoinUseBind;

		int mNoticeGap;
		int mTiliRecoverGap;
		int mListenChat;
		int mDefaultClothMan;
		int mDefaultClothWoman;

		//int mSteelEquipCostBase;
		//int mSteelEquipCostMul;
		int mVcoinFreshTime;
		int mVcoinMul;
		int mBaseMul;
		double mBaseHPMul;
		double mBaseMPMul;
		double mBaseACMul;
		double mBaseMACMul;
		double mBaseDCMul;
		double mBaseMCMul;
		double mBaseSCMul;
		int mCiShaJianShuCheckObj;
		int mInfoOnCongzi;
		int mLevelChartLimit;
		int mWorldChatLevelLimit;
		int mGroupChatLevelLimit;
		int mGuildChatLevelLimit;
		int mMapChatLevelLimit;
		int mNormalChatLevelLimit;
		int mPrivateChatLevelLimit;
		int mHornChatLevelLimit;
		int mGroupChatGap;
		int mWorldChatGap;
		int mGuildChatGap;
		int mMapChatGap;
		int mNormalChatGap;
		int mPrivateChatGap;
		int mHornChatGap;
		int mDieDropBagProb;
		int mDieDropLoadProb;
		int mMapEnterSafeTime;

		time_t mOnlineLogTick;
		time_t mMemoryLogTick;
		time_t mScriptMemoryCheckTick;
		time_t mSaveServerVarTick;
		int mPKLimitLevel;
		int mDieDropLimitLevel;
		int mFreeReliveLimitLevel;
		time_t mPingGap;
		time_t mPingGapMini;
		time_t mPingStopTime;

		int mMofadunParam;
		int mMofadunCishaParam;
		int mLieHuoBigHitProb;
		int mLieHuoBigHitMul;
		int mLieHuoParamBase;
		int mLieHuoParamGap;
		int mCreateGuildLimitLevel;
		int mJoinGuildLimitLevel;
		int mItemAddType;
		int mMaxLevelLimit;
		int mPlayerTrigerPKEvent;
		int mMaxMagicAnti;
		int mChartOpenLimitLevel;
		int mSessionSaveGap;
		double mMapAutoSecondGap;
		double mDeadAssistValidTime;
		int mTrigerOnGetItem;
		int mPlayerTrigerWarEvent;
		int mDieDropExp;
		int mDieDropExpProb;
		int mDieDropExpPresent;
		int mDieDropGold;
		int mDieDropGoldProb;
		int mDieDropGoldPresent;
		int mUpgradeApplyFlags;
		int mItemDataUpgrade106;
		int mSkillTickActionCountMax;
		int mTickActionCountMax;
		int mYouLinShenshenMul;
		int mMofadunMul;

		int mAccuracyConstBase;
		int mPosionArmorMul;

		int mHpIncParam1;
		int mHpIncParam2;
		int mMpIncParam1;
		int mMpIncParam2;
		int mHpMagicIncParam1;
		int mHpMagicIncParam2;

		//int mDeleteExchangeUpdFromEquip;

		int mPKKillSubLuckOpen;
		int mPKKillSubLuckProb;

		int mPosionStopRecast;

		int mFightLevelPressed;
		int mFightLevelPressedParam;

		int mUpgradeEquipPulishInfoLevelSucMin;
		int mUpgradeEquipPulishInfoLevelFalMin;


		int mWarriorAttrBaseHp;
		int mWarriorAttrLvHp;
		int mWizardAttrBaseHp;
		int mWizardAttrLvHp;
		int mTaoistAttrBaseHp;
		int mTaoistAttrLvHp;
		int mWarriorAttrBaseMp;
		int mWarriorAttrLvMp;
		int mWizardAttrBaseMp;
		int mWizardAttrLvMp;
		int mTaoistAttrBaseMp;
		int mTaoistAttrLvMp;

		int mTotalAttrLevelLimit;

		DWORD mTimeCheckPad;
		int mTimeCheckAlert;

		int mItemProtectPrice;
		int mItemProtectProbMax;
		int mItemProtectProb;
		int mItemProtectAdd;

		int mGuildMemberMax;
		int mGuildCheckPoint;

		int mRunToSkillDuraPresent;

		double mItemUseAddHPOntimeGap;


		int mKuLouLevelParam;
		int mKuLouLevelParamAdd;
		int mShenShouLevelParam;
		int mShenShouLevelParamAdd;

		int mCheckWalkRunClientTime;
		int mCheckWalkRunClientTimeResetGap;
		int mCheckWalkRunClientTimeKick;

		int mYeManChongZhuangGap;
		int mUseDisItemPerSecondMax;
		float mUseSkillPerSecondMax;
		float mActionPreSecontMax;

		int mAs2LuaPerSecondMax;
		int mCheckAs2LuaCount;
		int mRunWalkPerSecondMax;
		int mCheckRunWalkCount;

		float mSkillHuoQiangScale;
		float mSkillDiyuleiguangScale;
		float mSkillBaoliehuoyanScale;
		float mSkillBingpaoxiaoScale;

		int mItemUseAddHPMaxCount;
		int mPKConfirm;

		int mMarryLimitLevel;


		int mAutoReloadScriptGap;
		int mFreshTimeLimitItemGap;
		int mAddBagSlotPrice;
		int mAddBagMaxSlot;
		int mWizardAttackMul;
		int mTaoistAttackMul;

		int mAddBagOnceNum;
		int mAddDepotOnceNum;

		int mYeManMaBiProb;
		int mYeManMaBiDura;

		int mDartTaskRange;
		int mDartTaskNumber;
		int mRobDartNumber;
		int mRobDartLimit;
		int mWarriorDefaultSkill;
		int mWizardDefaultSkill;
		int mTaoistDefaultSkill;
		int mDirectFlyPrice;
		int mCheckSecondAction;
		double mCheckSecondActionParam;
		int mCheckSecondSkill;
		double mCheckSecondSkillParam;
		int mBrawnLimitOpen;
		int mLoadLimitOpen;
		int mCheckSteelOpen;
		int mMonMiniDamage;
		int mTradeAllowLevel;
		double mCheckChangeFriendRelationGap;
		double mFriendApplyTimeOut;

		int mShiDuShuMaxTimer;
		int mShiDuShuParam;
		int mShenLongSight;
		int mCishaCheckOther;

		int mMailValidDays;
		int mIsOpenMail;

		std::string mKf_Start_map;
		int mKf_Start_size;
		int mKf_Start_x1;
		int mKf_Start_y1;
		int mKf_Start_x2;
		int mKf_Start_y2;
		int mKf_Start_x3;
		int mKf_Start_y3;
		int mKf_Start_x4;
		int mKf_Start_y4;
		int mKf_Start_x5;
		int mKf_Start_y5;

		int mMonClearOwnerTime;
		int mMonClearRecordTime;

		int mTimeZone;
		int mClientWalkFrame;
		int mClientAttackFrame;
		int mClientMagicFrame;

		int mConsignFee12;
		int mConsignFee24;
		int mConsignFee48;
		int mConsignTex;
		int mGuildRedPacketTime;
		int mGuildWarTime;

		int mLevelUpMax;
	public:
		GameServerLib(void);
		virtual ~GameServerLib(void);
		ManagerMap* GetMapManager(){return mMapManager;}
		TimerFrame* GetFrameTimer(){return mFrameTimer;}
		TimerSecond* GetSecondTimer(){return mSecondTimer;}
		std::string& GetDataPath(){return mDataPath;}
		std::string& GetConfigPath(){return mConfigPath;}
		std::string& GetMapPath(){return mMapPath;}
		ScriptLua*  GetScriptEngine(){return mScriptLua;}
		ManagerItemDef* GetItemDefineManager(){return mItemDefineManager;}
		ManagerSkillDef* GetSkillDefineManager(){return mSkillDefineManager;}
		ManagerSwordDef* GetSwordDefManager(){return mSwordDefManager;}
		ManagerMonPointDef* GetMonPointDefManager(){return mMonPointDefManager;};
		ManagerEquipUpgradeDef* GetEquipUpgradeDefManager(){return mEquipUpgradeDefManager;};
		std::string  GetConfigFile(){return mConfigFile;}
		GMCommand* GetGMCommand(){return mGMCommand;}
		ManagerLevelInfo* GetLevelInfoManager(){return mLevelInfoManager;}
		ManagerTotalAttr* GetTotalInfoManager(){return mTotalAttrManager;}
		ManagerMonDef* GetMonDefManager(){return mMonDefManager;}
		void SetListener(GameServerLibListener* listener){mListener=listener;}
		TimerFix* GetTimerFix(){return mTimerFix;}
		ManagerMonster* GetMonsterManager(){return mMonsterManager;}
		ManagerGuild* GetGuildManager(){return mGuildManager;}
		ManagerConsignment * GetConsignmentManager() {return mConsignmentManager;}
		ManagerRelationCache * GetRelationCacheManager() {return mRelationCacheManager;};
		ManagerGroup* GetGroupManager(){return mGroupManager;}
		ManagerCouple* GetCoupleManager(){return mCoupleManager;}
		ManagerNotice* GetNoticeManager(){return mNoticeManager;}
		ManagerFilter* GetFilterManager(){return mFilterManager;}
		ManagerChart* GetChartManager(){return mChartManager;}
		ManagerDeny* GetDenyManager(){return mDenyManager;}
		ManagerTask* GetTaskManager(){return mTaskManager;}
		ManagerDenySell* GetDenySellManager(){return mDenySellManager;}
		ManagerAutoRun* GetAutoRunManager(){return mAutoRunManager;}
		ManagerFly* GetFlyManager(){return mFlyManager;}
		ManagerStatus* GetStatusManager(){return mStatusManager;}
		PlayNpc* GetShopNpc(){return mShopNpc;}
		ManagerMail* GetMailManager(){return mMailManager;}
		ManagerChargeDart* GetChargeDartManager(){return mChargeDartManager;}
		ManagerObjFuncs* GetObjFuncManager(){return mObjFuncManager;}
		int UpdateTick(){return mUpdateTick;}
		const std::string& Var(const std::string& key);
		void Var(const std::string& key,const std::string& value);
		const std::string& TempVar(const std::string& key);
		void TempVar(const std::string& key,const std::string& value);
		const std::map<std::string,std::string>& Vars()const{return mVars;}
		int GetKuafuState(){return mKuafu;}
		void StartKuafu();
		void StopKuafu();
		void GetStrongerKuafuGuild(std::string& guild_name);
		void SetStrongerKuafuGuild(std::string& guild_name);
		void GetStrongerKuafuArea(std::string& guild);
		void SetStrongerKuafuArea(std::string& guild);
		void GetKuafuURL(std::string& url);
		void SetKuafuURL(std::string& url);
		void GetKuafuPort(std::string& port);
		void SetKuafuPort(std::string& port);
		void SetLocalDomain(std::string& domain);
		void GetLocalDomain(std::string& domain);
		int GetWarState(){return mWar;}
		void StartWar();
		void StopWar();
		void SetServerDays();
		void SetMergeDays();
		void SetAcitivityDays(int begin,int end,int today);//设置活动日期
		int GetServerStartDays(){ return ServerStartDay; }
		int GetMergeStartDays(){ return MergeStartDay; }
		int GetActivityBegin(){ return ActivityBegin; }
		int GetToday() { return TodayData; }
		int GetActivityEnd(){ return ActivityEnd; }
		int IsWar(){return mWar;}
		void GetKingGuild(std::string& guild_name);
		void GetKingGuild(char* guild_name,size_t s);
		void SetKingGuild(std::string& guild_name);
		void PublicWarInfo();
		void LoadConfig();
		void LoadKernelConfig();
		int SetKuaFuComeBackData(std::string& charID,int vcoin,int kuafuResult);

		void Log(const char* msg);

		void StartProsperity();
		void UpdateProsperity();
		void StopProsperity();
		int Getprosperity(){return this->mProsperity;}
		int GetprosperityNext(){return mProsperityNext;}
		void SaveCoupleServerID();
		std::map<std::string,int>& GetKfStartPosition(){return mKfStartPosition;}
		void ClearKfStartPosition(){mKfStartPosition.clear();}
	public:
		void Start(const char* config_file);
		void Start2();
		void Stop();
		void Update();
	public:
		GameServerLibListener* GetListener(){return mListener;}
		std::vector<GameSession*>& GetAllSessions(){return mSessions;}
		void		 CreateSession(const char* loginID,const char* charID,const char* seedID,int authtype,int newyouke,const char* client_ip,GameSessionListener* listener);
		void		 InitDemoCharacter(OGSLib::DomainData* chr,const char* loginID,const char* charID);
		void		 SaveSession(GameSession* session,bool quit);
		void		 DestroySession(GameSession* session);
		size_t		 GetSessionNumber();
		size_t		 GetRegSessionNumber();
		size_t		 GetMaxSessionNumber(){return m_MaxSessionNum;}
		void		 GetNameList(std::stringstream& ss);
		void		 ServerAlertMessage(int lv,int flags,const char* msg);
		void		 ServerPlayEffect(int skill_type,int res_id);
		void         ServerExe(const char* code);
		void		 ServerGuild(const char* code);
		void		 WorldChat(DomainPlay* play,char* msg);
		void		 PublicProsperityChange();
		void		 PublicHornChat(DomainPlay* play,const char* msg);
		void		 StopAllSession();
		GameSession* FindSessionWithID(int id);
		GameSession* FindSessionWithLoginID(const std::string& loginID);
		GameSession* FindSessionWithCharID(const std::string& charID);
		GameSession* FindSessionWithSeedID(const std::string& seedID);
		std::map<std::string,GameSession*>& GetChatListenSessionMap(){return mChatListenSessionMap;}

		bool		 OnLoadCharacter(int GUID,int result,OGSLib::DomainData* character);
		bool		 OnLoadCharacter(int result,OGSLib::DomainData* character,const char* loginid,const char* charid);
		//void		 FixItemData106(OGSLib::DomainData* chr);
		bool		 OnSaveCharacter(int GUID,int result);
		bool		 OnLoadGuilds(int GUID,int result,const OGSLib::ManagerGuild& guild_map);
		bool		 OnSaveGuilds(int GUID,int result);
		bool		 OnLoadConsignment(int GUID,int result,const std::vector<ConsignableItemInfo *> info_list, std::map<std::string, int> user_vcoin_map);
		bool		 OnSaveConsignment(int GUID,int result);
		bool		 OnLoadRelationCache(int GUID,int result,const std::vector<RelationShipOper *> oper_list);
		bool		 OnSaveRelationCache(int GUID,int result);
		bool		 OnLoadCouples(int GUID,int result,const OGSLib::ManagerCouple& couple_map);
		bool		 OnSaveCouples(int GUID,int result);
		bool		 OnLoadServerVar(int GUID,int result,const std::map<std::string,std::string>& vars);
		bool		 OnSaveServerVar(int GUID,int result);
		bool		 OnLoadMails(int GUID,int result,const std::vector<OGSLib::Mail*>& mailVector);
		bool		 OnSaveMails(int GUID,int result);
		bool		 OnLoadAuto(int GUID,int result,std::vector<OGSLib::AutoRunDB>* res);
		bool		 OnFreshVcoin(const std::string& account,int vcoin,int czhaccu,bool gs=false);
		bool		 OnFreshGift(const char* chrname,int result,std::vector<OGSLib::SubGift>* gift);
		bool		 OnFreshItem(const char* seedname,const char* chrname,int result,std::vector<OGSLib::SubGift>* gift);
		bool		 OnModifyName(const char* seedname,const char* chrname,int result);
		const std::string ServerExeRand(const char* name,int num,int level);
	};
}
