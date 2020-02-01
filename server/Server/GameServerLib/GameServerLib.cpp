#include "StdAfx.h"
#include "GameServerLib.h"
#include "GameSession.h"
#include "ManagerMap.h"
#include "TimerFrame.h"
#include "TimerSecond.h"
#include "ScriptLua.h"
#include "SubItem.h"
#include "GmCommand.h"
#include "ManagerLevelInfo.h"
#include "ManagerTotalAttr.h"
//#include "ManagerReincarnation.h"
#include "ManagerMonDef.h"
#include "ManagerItemDef.h"
#include "ManagerSkillDef.h"
//#include "ManagerOnSale.h"
#include "ManagerStatus.h"
#include "SubSkill.h"
#include "TimerFix.h"
#include "DomainData.h"
#include "SubTask.h"
#include "SubStatus.h"
#include "SubGift.h"
#include "SubFriend.h"
#include "SubMail.h"
#include "ManagerMonster.h"
#include "ManagerCouple.h"
//#include "ManagerRobot.h"
#include "ManagerGuild.h"
#include "ManagerMail.h"
#include "ManagerDenySell.h"
#include "ManagerGroup.h"
#include "DomainPlay.h"
#include "ManagerFilter.h"
#include "ManagerNotice.h"
#include "ManagerDeny.h"
#include "ManagerChart.h"
#include "ManagerAutoRun.h"
#include "ManagerTask.h"
#include "PlayNpc.h"
#include "SubAchieve.h"
#include "ManagerMail.h"
#include "ManagerChargeDart.h"
#include "ManagerFly.h"
#include "ManagerSwordDef.h"
#include <set>
#include "ManagerRelationCache.h"
#include "ManagerConsignment.h"
#include "ManagerObjFuncs.h"

namespace OGSLib
{
	GameServerLib::GameServerLib(void)
	{
		mDeadAssistValidTime = 10.0f;
		mPKConfirm = 0;
		mMapManager = 0;
		mFrameTimer = 0;
		mSecondTimer = 0;
		mScriptLua = 0;
		mItemDefineManager = 0;
		mSkillDefineManager = 0;
		mGMCommand = 0;
		mLevelInfoManager = 0;
		mTotalAttrManager = 0;
		//mReinManager = 0;
		mStatusManager = 0;
		mMonDefManager = 0;
		mTimerFix = 0;
		mGuildManager = 0;
		mConsignmentManager = 0;
		mRelationCacheManager = 0;
		mCoupleManager = 0;
		//mRobotManager=0;
		mDenyManager = 0;
		mDenySellManager = 0;
		mAutoRunManager = 0;
		mShopNpc = 0;
		//mOnSaleManager = 0;
		mListenChat = 0;
		mDefaultClothMan = 0;
		mDefaultClothWoman = 0;

		mYouLinShenshenMul =0;
		mMofadunMul = 0;
		mTimeCheckAlert = 0;

		mListener = 0;
		mGenGUID = 100;
		m_MaxSessionNum=0;
		mUpdateTick = 10;

		mUpdateTime=0.0;
		mUpdateDuration=TimerFrame::FRAME_TICK;
		mLieHuoBigHitMul = 300;

		mUseDisItemPerSecondMax=4;
		mUseSkillPerSecondMax=2.2f;
		mActionPreSecontMax=2.2f;

		mAs2LuaPerSecondMax = 4;
		mCheckAs2LuaCount = 0;
		mRunWalkPerSecondMax = 5;
		mCheckRunWalkCount=0;

		mWar = 0;
		mKuafu = 0;

		ServerStartDay = 0;
		MergeStartDay = -1;

		ActivityBegin = 0;
		ActivityEnd = 0;
		TodayData = -1;

		show_server_start = false;
		show_merge_start = false;

		mDieDropBagProb=6;
		mDieDropLoadProb=20;

		mInfoOnCongzi = 0;

		mOnlineLogTick = 0;
		mMemoryLogTick = time(0);
		mScriptMemoryCheckTick = time(0);
		//mPlayerAttributeType = 0;

		mItemAddType = 0;

		mMaxLevelLimit = 0;

		mPlayerTrigerPKEvent = 0;
		mChartOpenLimitLevel = 20;

		mTiliRecoverGap = 6;
		mSessionSaveGap = 5;

		mTrigerOnGetItem = 0;
		mPlayerTrigerWarEvent = 0;

		mDieDropExp=0;
		mDieDropExpProb=0;
		mDieDropExpPresent=0;
		mDieDropGold=0;
		mDieDropGoldProb=0;
		mDieDropGoldPresent=0;
		mUpgradeApplyFlags = 0;
		mItemDataUpgrade106 = 1;

		mAccuracyConstBase = 35;

		mTotalAttrLevelLimit = 7;

		mWarriorAttrBaseHp = 0;
		mWarriorAttrLvHp = 0;
		mWizardAttrBaseHp = 0;
		mWizardAttrLvHp = 0;
		mTaoistAttrBaseHp = 0;
		mTaoistAttrLvHp = 0;
		mWarriorAttrBaseMp = 0;
		mWarriorAttrLvMp = 0;
		mWizardAttrBaseMp = 0;
		mWizardAttrLvMp = 0;
		mTaoistAttrBaseMp = 0;
		mTaoistAttrLvMp = 0;

		mSkillTickActionCountMax=2;
		mTickActionCountMax=3;

		mPosionArmorMul = 120;
		
		mHpIncParam1=0;
		mHpIncParam2=0;
		mMpIncParam1=0;
		mMpIncParam2=0;

		mPKKillSubLuckOpen = 0;
		mPKKillSubLuckProb = 10000;

		mFightLevelPressed = 0;
		mFightLevelPressedParam = 95;

		mCallBackContentClearTick=time(0);
		mCallBackContentClearTag=mGenGUID;

		mAutoReloadScriptGap = 12;
		mPosionStopRecast = 0;
		mFreshTimeLimitItemGap = 60;

		mItemProtectPrice = 100;
		mItemProtectProbMax = 100;
		mItemProtectProb = 30;
		mItemProtectAdd = 3;

		mGuildMemberMax = 200;
		mGuildCheckPoint = 0;

		mRunToSkillDuraPresent = 50;

		mItemUseAddHPOntimeGap = 1.0;

		//mCisha1GuaDaoFanwei=1;
		//mCisha2GuaDaoFanwei=1;
		//mLiehuoGuaDaoFanwei=2;

		mKuLouLevelParam = 10;
		mKuLouLevelParamAdd = 5;
		mShenShouLevelParam = 10;
		mShenShouLevelParamAdd = 5;

		mHpMagicIncParam1 = 5;
		mHpMagicIncParam2 = 5;

		mCheckWalkRunClientTime = 1;

		mYeManChongZhuangGap =3;

		mSkillHuoQiangScale=0;
		mSkillDiyuleiguangScale=0;
		mSkillBaoliehuoyanScale=0;
		mSkillBingpaoxiaoScale=0;
		mAddBagSlotPrice=0;
		mAddBagMaxSlot=0;

		mDartTaskRange = 15;
		mDartTaskNumber = 2;
		mRobDartNumber = 2;
		mRobDartLimit=1;

		mWizardAttackMul=100;
		mTaoistAttackMul=100;
		mWarriorDefaultSkill=100;
		mWizardDefaultSkill=400;
		mTaoistDefaultSkill=500;
		mDirectFlyPrice = 0;

		mCheckSecondAction=1;
		mCheckSecondActionParam=1.0;
		mCheckSecondSkill=1;
		mCheckSecondSkillParam=1.0;
		mBrawnLimitOpen=1;
		mLoadLimitOpen=1;
		mCheckSteelOpen=0;
		mMonMiniDamage=0;
		mTradeAllowLevel=0;
		mCheckChangeFriendRelationGap = 1.0;
		mFriendApplyTimeOut = 120.0;

		mShiDuShuMaxTimer=180;
		mShiDuShuParam=1000;

		mAddBagOnceNum=0;
		mAddDepotOnceNum=0;

		mYeManMaBiProb=0;
		mYeManMaBiDura=0;

		mShenLongSight = 5;
		mCishaCheckOther = 0;

		mMailValidDays=15;
		mIsOpenMail=1;

		mKf_Start_size = 15;
		mKf_Start_x1 = 1;
		mKf_Start_y1 = 1;
		mKf_Start_x2 = 1;
		mKf_Start_y2 = 1;
		mKf_Start_x3 = 1;
		mKf_Start_y3 = 1;
		mKf_Start_x4 = 1;
		mKf_Start_y4 = 1;
		mKf_Start_x5 = 1;
		mKf_Start_y5 = 1;

		mLevelChartLimit = 0;

		mMonClearOwnerTime = 30;
		mMonClearRecordTime = 60;

		mTimeZone = 8;
		mClientWalkFrame = 8;
		mClientAttackFrame = 16;
		mClientMagicFrame = 23;

		mState = GAME_SERVER_LIB_STATE_NULL;
	}
	GameServerLib::~GameServerLib(void)
	{
	}
	void GameServerLib::LoadKernelConfig()
	{
		int itemp;
		itemp = GetPrivateProfileInt("kernel","player_update_gap",100,mConfigFile.c_str());
		mUpdateDuration = 1.0*itemp/1000.0;
		itemp = GetPrivateProfileInt("kernel","monster_update_gap",40,mConfigFile.c_str());
		mMonsterManager->mUpdateDuration = 1.0*itemp/1000.0;
		//itemp = GetPrivateProfileInt("kernel","monster_update_div_param",3,mConfigFile.c_str());
		//mMonsterManager->mUpdateDivParam = itemp;
		itemp = GetPrivateProfileInt("kernel","map_update_gap",100,mConfigFile.c_str());
		mMapManager->mUpdateDuration = 1.0*itemp/1000.0;
		itemp = GetPrivateProfileInt("kernel","group_update_gap",3000,mConfigFile.c_str());
		mGroupManager->mUpdateGMDuration = 1.0*itemp/1000.0;
		mPingGap = GetPrivateProfileInt("kernel","ping_gap",4,mConfigFile.c_str());
		mPingGapMini = GetPrivateProfileInt("kernel","ping_gap_mini",1,mConfigFile.c_str());
		mPingStopTime = GetPrivateProfileInt("kernel","ping_stop_time",3,mConfigFile.c_str());
		GetListener()->LoadKernelConfig();
	}
	void GameServerLib::LoadConfig()
	{
		char temp[256];

		GetPrivateProfileString("server","name","aoshiqianxiong",temp,256,mConfigFile.c_str());
		mServerName=temp;
		UtilString::act_2_utf8(mServerName.c_str(),mServerName.length(),temp,256);
		mServerName = temp;

		GetPrivateProfileString("server","title","aoshiqianxiong_s1",temp,256,mConfigFile.c_str());
		mServerTitle=temp;
		UtilString::act_2_utf8(mServerTitle.c_str(),mServerTitle.length(),temp,256);
		mServerTitle = temp;

		GetPrivateProfileString("server","home","http://web.4399.com/asqx",temp,256,mConfigFile.c_str());
		mServerHome=temp;
		UtilString::act_2_utf8(mServerHome.c_str(),mServerHome.length(),temp,256);
		mServerHome = temp;

		GetPrivateProfileString("server","platform","my4399",temp,256,mConfigFile.c_str());
		mServerPlatform=temp;
		UtilString::act_2_utf8(mServerPlatform.c_str(),mServerPlatform.length(),temp,256);
		mServerPlatform = temp;

		mServerID=GetPrivateProfileInt("server","server_id",1000,mConfigFile.c_str());

		GetPrivateProfileString("game","start_map","0",temp,256,mConfigFile.c_str());
		mStart_map=temp;
		UtilString::act_2_utf8(mStart_map.c_str(),mStart_map.length(),temp,256);
		mStart_map = temp;
		mStart_x=GetPrivateProfileInt("game","start_x",4,mConfigFile.c_str());
		mStart_y=GetPrivateProfileInt("game","start_y",4,mConfigFile.c_str());
		mStart_size=GetPrivateProfileInt("game","start_size",4,mConfigFile.c_str());

		GetPrivateProfileString("game","war_start_map","0",temp,256,mConfigFile.c_str());
		mWar_Start_map=temp;
		UtilString::act_2_utf8(mWar_Start_map.c_str(),mWar_Start_map.length(),temp,256);
		mWar_Start_map = temp;
		mWar_Start_x=GetPrivateProfileInt("game","war_start_x",4,mConfigFile.c_str());
		mWar_Start_y=GetPrivateProfileInt("game","war_start_y",4,mConfigFile.c_str());
		mWar_Start_size=GetPrivateProfileInt("game","war_start_size",4,mConfigFile.c_str());

		GetPrivateProfileString("game","red_start_map","0",temp,256,mConfigFile.c_str());
		mRed_Start_map=temp;
		UtilString::act_2_utf8(mRed_Start_map.c_str(),mRed_Start_map.length(),temp,256);
		mRed_Start_map = temp;
		mRed_Start_x=GetPrivateProfileInt("game","red_start_x",4,mConfigFile.c_str());
		mRed_Start_y=GetPrivateProfileInt("game","red_start_y",4,mConfigFile.c_str());
		mRed_Start_size=GetPrivateProfileInt("game","red_start_size",4,mConfigFile.c_str());

		if( mStart_size < 1 ) mStart_size = 1;
		mPawMul = GetPrivateProfileInt("game","paw_mul",1,mConfigFile.c_str());
		if( mPawMul < 0 && mPawMul > 100000000 ) mPawMul = 1;
		mExpMul = GetPrivateProfileInt("game","exp_mul",1,mConfigFile.c_str());
		if( mExpMul < 0 && mExpMul > 100000000 ) mExpMul = 1;
		mPuppetExtMul = GetPrivateProfileInt("game","puppet_exp_mul",1,mConfigFile.c_str());
		if( mPuppetExtMul < 0 && mPuppetExtMul > 100000000 ) mPuppetExtMul = 1;
		mMonDropBind = GetPrivateProfileInt("game","mon_drop_bind",0,mConfigFile.c_str());
		mMonDropUseBind = GetPrivateProfileInt("game","mon_drop_use_bind",0,mConfigFile.c_str());
		mNoticeGap = GetPrivateProfileInt("game","notice_gap",300,mConfigFile.c_str());
		mNpcBuyGameMoneyBind = GetPrivateProfileInt("game","npc_buy_gamemoney_bind",0,mConfigFile.c_str());
		mNpcBuyGameMoneyUseBind = GetPrivateProfileInt("game","npc_buy_gamemoney_use_bind",0,mConfigFile.c_str());
		mNpcBuyVcoinBind = GetPrivateProfileInt("game","npc_buy_vcoin_bind",0,mConfigFile.c_str());
		mNpcBuyVcoinUseBind = GetPrivateProfileInt("game","npc_buy_vcoin_use_bind",0,mConfigFile.c_str());

		mVcoinFreshTime = GetPrivateProfileInt("game","vcoin_fresh_time",60,mConfigFile.c_str());
		mVcoinMul = GetPrivateProfileInt("game","vcoin_mul",1,mConfigFile.c_str());
		mBaseMul = GetPrivateProfileInt("game","base_mul",0,mConfigFile.c_str());
		mBaseHPMul = GetPrivateProfileInt("game","base_hp_mul",10000,mConfigFile.c_str())/10000.0;
		mBaseMPMul = GetPrivateProfileInt("game","base_mp_mul",10000,mConfigFile.c_str())/10000.0;
		mBaseACMul = GetPrivateProfileInt("game","base_ac_mul",10000,mConfigFile.c_str())/10000.0;
		mBaseMACMul = GetPrivateProfileInt("game","base_mac_mul",10000,mConfigFile.c_str())/10000.0;
		mBaseDCMul = GetPrivateProfileInt("game","base_dc_mul",10000,mConfigFile.c_str())/10000.0;
		mBaseMCMul = GetPrivateProfileInt("game","base_mc_mul",10000,mConfigFile.c_str())/10000.0;
		mBaseSCMul = GetPrivateProfileInt("game","base_sc_mul",10000,mConfigFile.c_str())/10000.0;
		mCiShaJianShuCheckObj = GetPrivateProfileInt("game","cishajianshu_check_obj",0,mConfigFile.c_str());
		mDieDropBagProb=GetPrivateProfileInt("game","die_drop_bag_prob",6,mConfigFile.c_str());
		mDieDropLoadProb=GetPrivateProfileInt("game","die_drop_load_prob",30,mConfigFile.c_str());

		mInfoOnCongzi = GetPrivateProfileInt("game","info_on_congzi",1,mConfigFile.c_str());

		mListenChat = GetPrivateProfileInt("game","listen_chat",0,mConfigFile.c_str());
		mDefaultClothMan = GetPrivateProfileInt("game","default_cloth_man",20000,mConfigFile.c_str());
		mDefaultClothWoman = GetPrivateProfileInt("game","default_cloth_woman",10000,mConfigFile.c_str());

		mWorldChatLevelLimit = GetPrivateProfileInt("game","world_chat_level_limit",65,mConfigFile.c_str());
		mGroupChatLevelLimit = GetPrivateProfileInt("game","group_chat_level_limit",60,mConfigFile.c_str());
		mGuildChatLevelLimit = GetPrivateProfileInt("game","guild_chat_level_limit",60,mConfigFile.c_str());
		mMapChatLevelLimit = GetPrivateProfileInt("game","map_chat_level_limit",60,mConfigFile.c_str());
		mNormalChatLevelLimit = GetPrivateProfileInt("game","normal_chat_level_limit",60,mConfigFile.c_str());
		mPrivateChatLevelLimit = GetPrivateProfileInt("game","private_chat_level_limit",60,mConfigFile.c_str());
		mHornChatLevelLimit = GetPrivateProfileInt("game","horn_chat_level_limit",60,mConfigFile.c_str());
		//mOnsaleLimitLevel = GetPrivateProfileInt("game","onsale_level_limit",40,mConfigFile.c_str());

		mLevelChartLimit = GetPrivateProfileInt("game","level_chart_limit",70,mConfigFile.c_str());

		mPrivateChatGap = GetPrivateProfileInt("game","chat_private_gap",1,mConfigFile.c_str());
		mGroupChatGap = GetPrivateProfileInt("game","chat_group_gap",1,mConfigFile.c_str());
		mWorldChatGap = GetPrivateProfileInt("game","chat_world_gap",5,mConfigFile.c_str());
		mGuildChatGap = GetPrivateProfileInt("game","chat_guild_gap",2,mConfigFile.c_str());
		mMapChatGap = GetPrivateProfileInt("game","chat_map_gap",2,mConfigFile.c_str());
		mNormalChatGap = GetPrivateProfileInt("game","chat_normal_gap",1,mConfigFile.c_str());
		mHornChatGap = GetPrivateProfileInt("game","chat_horn_gap",3,mConfigFile.c_str());
		mMapEnterSafeTime = GetPrivateProfileInt("game","map_enter_safe_time",5,mConfigFile.c_str());
		mPKLimitLevel = GetPrivateProfileInt("game","pk_limit_level",50,mConfigFile.c_str());
		mDieDropLimitLevel = GetPrivateProfileInt("game","die_drop_limit_level",10,mConfigFile.c_str());
		mFreeReliveLimitLevel = GetPrivateProfileInt("game","free_relive_limit_level",0,mConfigFile.c_str());

		mMofadunParam = GetPrivateProfileInt("game","mofadun_param",7,mConfigFile.c_str());
		mMofadunCishaParam = GetPrivateProfileInt("game","mofadun_cisha_param",0,mConfigFile.c_str());
		mLieHuoBigHitProb = GetPrivateProfileInt("game","liehuo_bighit_prob",20,mConfigFile.c_str());
		mLieHuoBigHitMul = GetPrivateProfileInt("game","liehuo_bighit_mul",300,mConfigFile.c_str());
		mLieHuoParamBase = GetPrivateProfileInt("game","liehuo_param_base",4,mConfigFile.c_str());
		mLieHuoParamGap = GetPrivateProfileInt("game","liehuo_param_gap",4,mConfigFile.c_str());

		mFreshTimeLimitItemGap = GetPrivateProfileInt("game","fresh_time_limit_item_gap",60,mConfigFile.c_str());

		mCreateGuildLimitLevel = GetPrivateProfileInt("game","create_guild_limit_level",10,mConfigFile.c_str());
		mJoinGuildLimitLevel = GetPrivateProfileInt("game","join_guild_limit_level",60,mConfigFile.c_str());

		mItemAddType = GetPrivateProfileInt("game","item_add_type",0,mConfigFile.c_str());

		mMaxLevelLimit = GetPrivateProfileInt("game","max_level_limit",1000,mConfigFile.c_str());

		mShenLongSight = GetPrivateProfileInt("game","shen_long_sight",5,mConfigFile.c_str());
		mCishaCheckOther = GetPrivateProfileInt("game","mofadun_check_param",0,mConfigFile.c_str());

		mPlayerTrigerPKEvent = GetPrivateProfileInt("game","player_triger_pk_event",1000,mConfigFile.c_str());

		mMaxMagicAnti = GetPrivateProfileInt("game","max_magic_anti",10,mConfigFile.c_str());

		mChartOpenLimitLevel = GetPrivateProfileInt("game","chart_open_limit_level",20,mConfigFile.c_str());

		mMapAutoSecondGap = GetPrivateProfileInt("game","map_auto_second_gap",1000,mConfigFile.c_str())/1000.0f;
		mDeadAssistValidTime = GetPrivateProfileInt("game","dead_assist_valid_time",10000,mConfigFile.c_str())/1000.0f;

		mSessionSaveGap = GetPrivateProfileInt("game","session_save_gap",5,mConfigFile.c_str());

		mTrigerOnGetItem = GetPrivateProfileInt("game","triger_on_get_item",0,mConfigFile.c_str());

		mPlayerTrigerWarEvent = GetPrivateProfileInt("game","player_triger_war_event",1,mConfigFile.c_str());

		mDieDropExp=GetPrivateProfileInt("game","die_drop_exp",0,mConfigFile.c_str());
		mDieDropExpProb=GetPrivateProfileInt("game","die_drop_exp_prob",10000,mConfigFile.c_str());
		mDieDropExpPresent=GetPrivateProfileInt("game","die_drop_exp_presend",100,mConfigFile.c_str());
		mDieDropGold=GetPrivateProfileInt("game","die_drop_gold",0,mConfigFile.c_str());
		mDieDropGoldProb=GetPrivateProfileInt("game","die_drop_gold_prob",10000,mConfigFile.c_str());
		mDieDropGoldPresent=GetPrivateProfileInt("game","die_drop_gold_present",100,mConfigFile.c_str());

		mUpgradeApplyFlags = GetPrivateProfileInt("game","upgrade_apply_flags",0,mConfigFile.c_str());
		mItemDataUpgrade106 = GetPrivateProfileInt("game","item_data_upgrade_106",0,mConfigFile.c_str());
		mSkillTickActionCountMax=GetPrivateProfileInt("game","skill_tick_action_count_max",0,mConfigFile.c_str());
		mTickActionCountMax=GetPrivateProfileInt("game","tick_action_count_max",2,mConfigFile.c_str());

		mTotalAttrLevelLimit=GetPrivateProfileInt("game","total_attr_level_limit",7,mConfigFile.c_str());

		mWarriorAttrBaseHp = GetPrivateProfileInt("game","warrior_attr_base_hp",100,mConfigFile.c_str());
		mWarriorAttrLvHp = GetPrivateProfileInt("game","warrior_attr_lv_hp",20,mConfigFile.c_str());
		mWizardAttrBaseHp = GetPrivateProfileInt("game","wizard_attr_base_hp",100,mConfigFile.c_str());
		mWizardAttrLvHp = GetPrivateProfileInt("game","wizard_attr_lv_hp",10,mConfigFile.c_str());
		mTaoistAttrBaseHp = GetPrivateProfileInt("game","taoist_attr_base_hp",100,mConfigFile.c_str());
		mTaoistAttrLvHp = GetPrivateProfileInt("game","taoist_attr_lv_hp",15,mConfigFile.c_str());
		mWarriorAttrBaseMp = GetPrivateProfileInt("game","warrior_attr_base_mp",100,mConfigFile.c_str());
		mWarriorAttrLvMp = GetPrivateProfileInt("game","warrior_attr_lv_mp",10,mConfigFile.c_str());
		mWizardAttrBaseMp = GetPrivateProfileInt("game","wizard_attr_base_mp",100,mConfigFile.c_str());
		mWizardAttrLvMp = GetPrivateProfileInt("game","wizard_attr_lv_mp",20,mConfigFile.c_str());
		mTaoistAttrBaseMp = GetPrivateProfileInt("game","taoist_attr_base_mp",100,mConfigFile.c_str());
		mTaoistAttrLvMp = GetPrivateProfileInt("game","taoist_attr_lv_mp",15,mConfigFile.c_str());

		mMonClearOwnerTime = GetPrivateProfileInt("game","mon_clear_owner_time",30,mConfigFile.c_str());
		mAccuracyConstBase=GetPrivateProfileInt("game","accuracy_const_base",35,mConfigFile.c_str());
		mPosionArmorMul=GetPrivateProfileInt("game","posion_armor_mul",120,mConfigFile.c_str());

		mHpIncParam1=GetPrivateProfileInt("game","hp_inc_param1",5,mConfigFile.c_str());
		mHpIncParam2=GetPrivateProfileInt("game","hp_inc_param2",5,mConfigFile.c_str());
		mMpIncParam1=GetPrivateProfileInt("game","mp_inc_param1",5,mConfigFile.c_str());
		mMpIncParam2=GetPrivateProfileInt("game","mp_inc_param2",5,mConfigFile.c_str());
		mHpMagicIncParam1=GetPrivateProfileInt("game","hp_magic_inc_param1",2,mConfigFile.c_str());
		mHpMagicIncParam2=GetPrivateProfileInt("game","hp_magic_inc_param2",20,mConfigFile.c_str());

		//mDeleteExchangeUpdFromEquip = GetPrivateProfileInt("game","delete_exchange_upd_from_equip",0,mConfigFile.c_str());

		mPKKillSubLuckOpen = GetPrivateProfileInt("game","pk_kill_sub_luck_open",0,mConfigFile.c_str());
		mPKKillSubLuckProb = GetPrivateProfileInt("game","pk_kill_sub_luck_prob",10000,mConfigFile.c_str());

		mPosionStopRecast = GetPrivateProfileInt("game","posion_stop_recast",0,mConfigFile.c_str());

		mUpgradeEquipPulishInfoLevelSucMin = GetPrivateProfileInt("game","upgrade_equip_publish_info_level_suc_min",200,mConfigFile.c_str());
		mUpgradeEquipPulishInfoLevelFalMin = GetPrivateProfileInt("game","upgrade_equip_publish_info_level_fal_min",200,mConfigFile.c_str());

		mTimeCheckPad = GetPrivateProfileInt("game","time_check_pad",1000,mConfigFile.c_str());
		mTimeCheckAlert = GetPrivateProfileInt("game","time_check_alert",0,mConfigFile.c_str());
		mItemProtectPrice = GetPrivateProfileInt("game","item_protect_price",100,mConfigFile.c_str());
		mItemProtectProbMax = GetPrivateProfileInt("game","item_protect_prob_max",100,mConfigFile.c_str());
		mItemProtectProb = GetPrivateProfileInt("game","item_protect_prob",30,mConfigFile.c_str());
		mItemProtectAdd = GetPrivateProfileInt("game","item_protect_add",3,mConfigFile.c_str());

		mGuildMemberMax = GetPrivateProfileInt("game","guild_member_max",200,mConfigFile.c_str());
		mGuildCheckPoint = GetPrivateProfileInt("game","guild_check_point",0,mConfigFile.c_str());

		mRunToSkillDuraPresent = GetPrivateProfileInt("game","run_to_skill_dura_present",0,mConfigFile.c_str());

		mItemUseAddHPOntimeGap = GetPrivateProfileInt("game","item_use_addhp_ontime_gap",25,mConfigFile.c_str())/100.0;

		//mCisha1GuaDaoFanwei = GetPrivateProfileInt("game","cisha_1_gua_dao_fan_wei",1,mConfigFile.c_str());
		//mCisha2GuaDaoFanwei = GetPrivateProfileInt("game","cisha_2_gua_dao_fan_wei",0,mConfigFile.c_str());
		//mLiehuoGuaDaoFanwei = GetPrivateProfileInt("game","liehuo_gua_dao_fan_wei",2,mConfigFile.c_str());

		mKuLouLevelParam = GetPrivateProfileInt("game","kulou_level_param",10,mConfigFile.c_str());
		mKuLouLevelParamAdd = GetPrivateProfileInt("game","kulou_level_param_add",5,mConfigFile.c_str());
		mShenShouLevelParam = GetPrivateProfileInt("game","shenshou_level_param",10,mConfigFile.c_str());
		mShenShouLevelParamAdd = GetPrivateProfileInt("game","shenshou_level_param_add",5,mConfigFile.c_str());

		mCheckWalkRunClientTime = GetPrivateProfileInt("game","check_walk_run_client_time",1,mConfigFile.c_str());
		mCheckWalkRunClientTimeResetGap = GetPrivateProfileInt("game","check_walk_run_client_time_reset_gap",60,mConfigFile.c_str());
		mCheckWalkRunClientTimeKick = GetPrivateProfileInt("game","check_walk_run_client_time_kick",0,mConfigFile.c_str());

		mAs2LuaPerSecondMax = GetPrivateProfileInt("game","as2lua_count_max",4,mConfigFile.c_str());
		mCheckAs2LuaCount = GetPrivateProfileInt("game","check_as2lua_count",0,mConfigFile.c_str());
		mRunWalkPerSecondMax = GetPrivateProfileInt("game","runwalk_count_max",5,mConfigFile.c_str());
		mCheckRunWalkCount = GetPrivateProfileInt("game","check_runwalk_count",0,mConfigFile.c_str());

		mYeManChongZhuangGap = GetPrivateProfileInt("game","yemanchongzhuang_gap",3,mConfigFile.c_str());
		mUseDisItemPerSecondMax = GetPrivateProfileInt("game","use_disitem_per_second_max",5,mConfigFile.c_str());
		mUseSkillPerSecondMax = GetPrivateProfileInt("game","use_skill_per_second_max",2200,mConfigFile.c_str())/1000.0f;
		mActionPreSecontMax = GetPrivateProfileInt("game","action_per_second_max",2100,mConfigFile.c_str())/1000.0f;
		mSkillHuoQiangScale = GetPrivateProfileInt("game","skill_huoqiang_scale",1000,mConfigFile.c_str())/1000.0f;;
		mSkillDiyuleiguangScale = GetPrivateProfileInt("game","skill_diyuleiguang_scale",1000,mConfigFile.c_str())/1000.0f;;
		mSkillBaoliehuoyanScale = GetPrivateProfileInt("game","skill_baoliehuoyan_scale",1000,mConfigFile.c_str())/1000.0f;;
		mSkillBingpaoxiaoScale = GetPrivateProfileInt("game","skill_bingpaoxiao_scale",1000,mConfigFile.c_str())/1000.0f;
		mItemUseAddHPMaxCount = GetPrivateProfileInt("game","mItemUseAddHPMaxCount",4,mConfigFile.c_str());
		mPKConfirm = GetPrivateProfileInt("game","pk_confirm",0,mConfigFile.c_str());
		mAutoReloadScriptGap = GetPrivateProfileInt("game","auto_reload_script_gap",12,mConfigFile.c_str());
		mAddBagSlotPrice = GetPrivateProfileInt("game","add_bag_slot_price",1000,mConfigFile.c_str());
		mAddBagMaxSlot = GetPrivateProfileInt("game","add_bag_max_slot",80,mConfigFile.c_str());
		mWizardAttackMul = GetPrivateProfileInt("game","wizard_attack_mul",100,mConfigFile.c_str());
		mTaoistAttackMul = GetPrivateProfileInt("game","taoist_attack_mul",100,mConfigFile.c_str());

		mAddBagOnceNum = GetPrivateProfileInt("game","add_bag_slot_once",5,mConfigFile.c_str());
		mAddDepotOnceNum = GetPrivateProfileInt("game","add_depot_slot_once",5,mConfigFile.c_str());

		mDartTaskRange=GetPrivateProfileInt("game","dart_task_range",15,mConfigFile.c_str());
		mDartTaskNumber=GetPrivateProfileInt("game","dart_task_number",2,mConfigFile.c_str());
		mRobDartNumber=GetPrivateProfileInt("game","rob_dart_number",2,mConfigFile.c_str());
		mRobDartLimit=GetPrivateProfileInt("game","rob_dart_limit",1,mConfigFile.c_str());
		mDirectFlyPrice = GetPrivateProfileInt("game","direct_fly_price",1,mConfigFile.c_str());
		mYouLinShenshenMul = GetPrivateProfileInt("game","youlinshenshen_mul",1000,mConfigFile.c_str());
		mMofadunMul = GetPrivateProfileInt("game","mofadun_mul",1000,mConfigFile.c_str());
		mCheckSecondAction = GetPrivateProfileInt("game","check_second_action",1,mConfigFile.c_str());
		mCheckSecondActionParam = GetPrivateProfileInt("game","check_second_action_param",1000,mConfigFile.c_str())/1000.0f;
		mCheckSecondSkill = GetPrivateProfileInt("game","check_second_skill",1,mConfigFile.c_str());
		mCheckSecondSkillParam = GetPrivateProfileInt("game","check_second_action_skill",1000,mConfigFile.c_str())/1000.0f;
		mCheckChangeFriendRelationGap = GetPrivateProfileInt("game","check_change_friend_relation_gap",1000,mConfigFile.c_str())/1000.0f;
		mFriendApplyTimeOut = GetPrivateProfileInt("game","friend_apply_timeout",120000,mConfigFile.c_str())/1000.0f;
		mBrawnLimitOpen = GetPrivateProfileInt("game","brawn_limit_open",1,mConfigFile.c_str());
		mLoadLimitOpen = GetPrivateProfileInt("game","load_limit_open",1,mConfigFile.c_str());
		mCheckSteelOpen = GetPrivateProfileInt("game","check_steel_open",0,mConfigFile.c_str());
		mMonMiniDamage = GetPrivateProfileInt("game","mon_mini_damage",0,mConfigFile.c_str());
		mTradeAllowLevel = GetPrivateProfileInt("game","trade_allow_level",45,mConfigFile.c_str());

		mFightLevelPressed = GetPrivateProfileInt("game","fight_level_pressed",0,mConfigFile.c_str());
		mFightLevelPressedParam = GetPrivateProfileInt("game","fight_level_pressed_param",95,mConfigFile.c_str());

		//mShowLoginForm = GetPrivateProfileInt("game","show_login_form",0,mConfigFile.c_str());
		mTiliRecoverGap = GetPrivateProfileInt("game","tili_recover_gap",6,mConfigFile.c_str());

		mShiDuShuMaxTimer=GetPrivateProfileInt("game","shidushu_maxtimer",180,mConfigFile.c_str());
		mShiDuShuParam=GetPrivateProfileInt("game","shidushu_param",1000,mConfigFile.c_str());

		mYeManMaBiProb=GetPrivateProfileInt("game","yeman_mabi_prob",500,mConfigFile.c_str());
		mYeManMaBiDura=GetPrivateProfileInt("game","yeman_mabi_dura",1,mConfigFile.c_str());

		mMailValidDays=GetPrivateProfileInt("game","mail_valid_days",15,mConfigFile.c_str());
		mIsOpenMail=GetPrivateProfileInt("game","is_open_mail",1,mConfigFile.c_str());

		mConsignFee12 = GetPrivateProfileInt("game","consign_fee12",0,mConfigFile.c_str());
		mConsignFee24 = GetPrivateProfileInt("game","consign_fee24",0,mConfigFile.c_str());
		mConsignFee48 = GetPrivateProfileInt("game","consign_fee48",0,mConfigFile.c_str());
		mConsignTex = GetPrivateProfileInt("game","consign_tex",5,mConfigFile.c_str());
		mGuildRedPacketTime = GetPrivateProfileInt("game","GuildRedPacketTime",1440,mConfigFile.c_str());
		mGuildWarTime = GetPrivateProfileInt("game","GuildWarTime",1440,mConfigFile.c_str());
		mLevelUpMax = GetPrivateProfileInt("game","levelup_max_num",10,mConfigFile.c_str());

		GetPrivateProfileString("kuafu","kf_start_map","",temp,256,mConfigFile.c_str());
		mKf_Start_map = temp;
        UtilString::act_2_utf8(mKf_Start_map.c_str(), mKf_Start_map.length(), temp, 256);
		mKf_Start_map = temp;
		mKf_Start_size = GetPrivateProfileInt("kuafu","kf_start_size",4,mConfigFile.c_str());
		mKf_Start_x1 = GetPrivateProfileInt("kuafu","kf_start_x1",4,mConfigFile.c_str());
		mKf_Start_y1 = GetPrivateProfileInt("kuafu","kf_start_y1",4,mConfigFile.c_str());
		mKf_Start_x2 = GetPrivateProfileInt("kuafu","kf_start_x2",4,mConfigFile.c_str());
		mKf_Start_y2 = GetPrivateProfileInt("kuafu","kf_start_y2",4,mConfigFile.c_str());
		mKf_Start_x3 = GetPrivateProfileInt("kuafu","kf_start_x3",4,mConfigFile.c_str());
		mKf_Start_y3 = GetPrivateProfileInt("kuafu","kf_start_y3",4,mConfigFile.c_str());
		mKf_Start_x4 = GetPrivateProfileInt("kuafu","kf_start_x4",4,mConfigFile.c_str());
		mKf_Start_y4 = GetPrivateProfileInt("kuafu","kf_start_y4",4,mConfigFile.c_str());
		mKf_Start_x5 = GetPrivateProfileInt("kuafu","kf_start_x5",4,mConfigFile.c_str());
		mKf_Start_y5 = GetPrivateProfileInt("kuafu","kf_start_y5",4,mConfigFile.c_str());

		GetListener()->LoadConfig();
	}
	void GameServerLib::Start(const char* config_file)
	{
		mState = GAME_SERVER_LIB_STATE_STARTING;

		char temp[256];
		GetPrivateProfileString("path","data_path","d:\\",temp,256,config_file);
		mDataPath = temp;
		if( mDataPath.substr(mDataPath.length()-1) != "\\" )
		{
			mDataPath += "\\";
		}
		GetPrivateProfileString("path","config_path","d:\\",temp,256,config_file);
		mConfigPath = temp;
		if( mConfigPath.substr(mConfigPath.length()-1) != "\\" )
		{
			mConfigPath += "\\";
		}
		GetPrivateProfileString("path","map_path","d:\\map",temp,256,config_file);
		mMapPath=temp;
		if( mMapPath.substr(mMapPath.length()-1) != "\\" )
		{
			mMapPath += "\\";
		}
		mConfigFile = config_file;//mDataPath+"config.ini";

		mStringConsts.Load(this);

		LoadConfig();

		mListener->LoadServerVar(0);
		mMonsterManager = new ManagerMonster;
		mTimerFix = new TimerFix;
		mItemDefineManager = new ManagerItemDef;
		mItemDefineManager->LoadItemDefine(this);
		mItemDefineManager->LoadSpeedDefine(this);
		mItemDefineManager->LoadStackDefine(this);
		mItemDefineManager->LoadItemPlusAttr(this);
		//mItemDefineManager->LoadTradeLimit(this);
		mItemDefineManager->LoadItemPlusDefine2(this);
		mItemDefineManager->LoadItemPlusTxt(this);

		mSkillDefineManager = new ManagerSkillDef;
		//mSkillDefineManager->LoadSkillDefine(this);
		mSkillDefineManager->LoadNewSkillDefine(this);

		mSwordDefManager = new ManagerSwordDef;
		//mSwordDefManager->LoadSwordDefine(this);
		mMonPointDefManager = new ManagerMonPointDef;
		mMonPointDefManager->LoadMonPointDefine(this);
		mEquipUpgradeDefManager = new ManagerEquipUpgradeDef;
		mEquipUpgradeDefManager->LoadEquipUpgradeDefine(this);
		
		//mSkillDefineManager->LoadSkillParam(this);
		mLevelInfoManager = new ManagerLevelInfo(this);
		mLevelInfoManager->Init();
		mTotalAttrManager = new ManagerTotalAttr(this);
		mTotalAttrManager->Init();
		//mReinManager = new ManagerReincarnation(this);
		//mReinManager->Init();
		mStatusManager = new ManagerStatus(this);
		mStatusManager->Init();
		mObjFuncManager = new ManagerObjFuncs(this);
		mObjFuncManager->Init();
		mGMCommand = new GMCommand(this);
		mGMCommand->Start();
		mFrameTimer = new TimerFrame(512,this);
		mSecondTimer = new TimerSecond;
		mGuildManager = new ManagerGuild(this);
		mGuildManager->init();
		mConsignmentManager = new ManagerConsignment(this);
		mConsignmentManager->init();
		mRelationCacheManager = new ManagerRelationCache(this);
		mRelationCacheManager->init();
		mCoupleManager = new ManagerCouple(this);
		mCoupleManager->init();
		//mRobotManager=new ManagerRobot(this);
		mGroupManager = new ManagerGroup(this);
		mGroupManager->init();
		//mOnSaleManager = new ManagerOnSale(this);
		//mOnSaleManager->Init();
		mNoticeManager = new ManagerNotice(this);
		mNoticeManager->Init();
		mFilterManager = new ManagerFilter(this);
		mFilterManager->Init();
		mChartManager = new ManagerChart(this);
		mChartManager->Init();
		mDenyManager = new ManagerDeny(this);
		mDenyManager->Init();
		//mTaskManager = new ManagerTask(this);
		//mTaskManager->init();
		mDenySellManager = new ManagerDenySell(this);
		mDenySellManager->Init();
		mFlyManager = new ManagerFly(this);
		mFlyManager->Init();
        mMailManager = new ManagerMail(this);
        mMailManager->init();
		mChargeDartManager = new ManagerChargeDart(this);
		mChargeDartManager->init();
	}
	void GameServerLib::Start2()
	{
		mAutoRunManager = new ManagerAutoRun(this);
		mAutoRunManager->Init();
		char temp[256];
		mScriptLua = new ScriptLua(this);
		_snprintf_s(temp,_countof(temp),255,"%sscript\\",mDataPath.c_str());
		temp[255]=0;
		mScriptLua->init(temp);
		mMonDefManager = new ManagerMonDef(this);
		mMonDefManager->init();
		mMapManager = new ManagerMap(this);
		mMapManager->Init();
		mTimerFix->Restart();
		mShopNpc = new PlayNpc(this);
		mShopNpc->mCheckRange = false;
		mShopNpc->mSell = false;
		mShopNpc->mRepair = false;
		//mShopNpc->LoadScript(this->GetScriptEngine(),"vcoinshop");
		//mShopNpc->mShopFile = "vcoinshop.csv";
		//mShopNpc->LoadShopGood(this);
		LoadKernelConfig();
		mSaveServerVarTick = time(0);
		StartProsperity();
		mState = GAME_SERVER_LIB_STATE_RUN;
	}
	void GameServerLib::Stop()
	{
		mState = GAME_SERVER_LIB_STATE_ENDING;
		mListener->SaveServerVar(*this,0);
		Sleep(2000);
		mGuildManager->SaveGuilds();
		Sleep(2000);
		mAutoRunManager->Dest();
		delete mAutoRunManager;mAutoRunManager = 0;
		delete mShopNpc;mShopNpc=0;
		if( mMapManager )mMapManager->Dest();
		delete mMapManager;mMapManager=0;
		if( mMonDefManager )mMonDefManager->dest();
		delete mMonDefManager;mMonDefManager=0;
		if( mScriptLua )mScriptLua->dest();		
		delete mScriptLua;mScriptLua=0;
		if( mGroupManager ) mGroupManager->dest();
		delete mGroupManager;mGroupManager=0;
		//if(mRobotManager)mRobotManager->dest();
		//delete mRobotManager;mRobotManager=0;
		if( mNoticeManager ) mNoticeManager->Dest();
		delete mNoticeManager;mNoticeManager=0;
		if( mFilterManager ) mFilterManager->Dest();
		delete mFilterManager;mFilterManager=0;
		if( mChartManager ) mChartManager->Dest();
		delete mChartManager;mChartManager=0;
		if( mDenyManager ) mDenyManager->Dest();
		delete mDenyManager;mDenyManager=0;
		if( mDenySellManager ) mDenySellManager->Dest();
		delete mDenySellManager;mDenySellManager=0;
		if( mGuildManager ) mGuildManager->dest(false);
		delete mGuildManager;mGuildManager=0;
		if (mConsignmentManager) mConsignmentManager->dest();
		delete mConsignmentManager;mConsignmentManager = 0;
		if (mRelationCacheManager)mRelationCacheManager->dest();
		delete mRelationCacheManager;mRelationCacheManager = 0;
		if( mCoupleManager ) mCoupleManager->dest();
		delete mCoupleManager;mCoupleManager=0;
		//if( mOnSaleManager ) mOnSaleManager->Dest();
		//delete mOnSaleManager;mOnSaleManager=0;
		if( mLevelInfoManager )mLevelInfoManager->Dest();
		delete mLevelInfoManager;mLevelInfoManager=0;
		if( mTotalAttrManager )mTotalAttrManager->Dest();
		delete mTotalAttrManager;mTotalAttrManager=0;
		//if( mReinManager )mReinManager->Dest();
		//delete mReinManager;mReinManager=0;
		if( mStatusManager )mStatusManager->Dest();
		delete mStatusManager;mStatusManager=0;
		if (mObjFuncManager) mObjFuncManager->Dest();
		SAFE_DELETE(mObjFuncManager);
		//if( mTaskManager )mTaskManager->Dest();
		//delete mTaskManager;mTaskManager=0;
		if( mFlyManager )mFlyManager->Dest();
		delete mFlyManager;mFlyManager=0;
		if( mGMCommand )mGMCommand->Stop();
		delete mGMCommand;mGMCommand=0;
		delete mSecondTimer;mSecondTimer=0;
		delete mFrameTimer;mFrameTimer = 0;
		mScriptLua = 0;
		if( mItemDefineManager ) mItemDefineManager->dest();
		delete mItemDefineManager;mItemDefineManager=0;
		if( mSkillDefineManager ) mSkillDefineManager->Dest();
		delete mSkillDefineManager;mSkillDefineManager=0;
        if (mSwordDefManager) mSwordDefManager->Dest();
        delete mSwordDefManager; mSwordDefManager = 0;
        if (mMonPointDefManager) mMonPointDefManager->Dest();
        delete mMonPointDefManager; mMonPointDefManager = 0;
		if (mEquipUpgradeDefManager) mEquipUpgradeDefManager->Dest();
		delete mEquipUpgradeDefManager; mEquipUpgradeDefManager = 0;
		delete mTimerFix;mTimerFix=0;
		mMonsterManager->TheDest();
		delete mMonsterManager;mMonsterManager=0;
		mMonsterManager=0;
        if (mMailManager) mMailManager->dest();
        delete mMailManager; mMailManager = 0;
		if( mChargeDartManager )mChargeDartManager->dest();
		delete mChargeDartManager;mChargeDartManager=0;
		mListener->SaveServerVar(*this,0);
		StopProsperity();
		mState = GAME_SERVER_LIB_STATE_END;
	}
	class SessionUpdateFuntor
	{
	public:
		GameServerLib* This;
		double time;
		bool operator() (GameSession* & session)
		{
			if( session->GetState()==GameSession::SESSION_STATE_END )
			{
				This->DestroySession(session);
				return true;
			}
			session->Update(time);
			return false;
		}
	};
	class CallBackContentRemover
	{
	public:
		GameServerLib* This;
		double time;
		bool operator() (GameSession* & session)
		{
			if( session->GetState()==GameSession::SESSION_STATE_END )
			{
				This->DestroySession(session);
				return true;
			}
			session->Update(time);
			return false;
		}
	};
	void GameServerLib::Update()
	{
		mUpdateTick++;
		if( mState != GAME_SERVER_LIB_STATE_RUN ) return;
		double tim = GetTimerFix()->Elapsed();

		{
			PERF_NODE("mFrameTimer");
			mFrameTimer->Update(tim);
		}
		{
			PERF_NODE("mSecondTimer");
			mSecondTimer->Update();
		}
		{
			PERF_NODE("mSessionsUpdate");
			if( mUpdateTime + mUpdateDuration < tim )
			{
				mUpdateTime = tim;
				SessionUpdateFuntor funtor;funtor.This = this;funtor.time = tim;
				mSessions.erase(std::remove_if(mSessions.begin(),mSessions.end(),funtor),mSessions.end());
			}
		}
		{
			PERF_NODE("mMonsterManager");
			mMonsterManager->Update(tim);
		}
		{
			PERF_NODE("mMapManager");
			mMapManager->Update(tim);
		}
		{
			PERF_NODE("mGroupManager");
			mGroupManager->Update(tim);
		}
		{
			PERF_NODE("mAutoRunManager");
			mAutoRunManager->Update(tim);
		}
		{
			mGuildManager->update();
		}
		{
			mConsignmentManager->update();
		}
		{
			mRelationCacheManager->update();
		}
		{
			mCoupleManager->update();
		}
		{
			UpdateProsperity();
		}
		{
			mChartManager->Update();
		}
        {
            mMailManager->update();
        }
		{
			mChargeDartManager->Update(tim);
		}
		{
			time_t t = time(0);
			if( mCallBackContentClearTick + 60*10 < t )
			{
				mCallBackContentClearTick = t;
				for(std::map<int,std::map<std::string,std::string> >::iterator pos = mCallBackContent.begin();
					pos != mCallBackContent.end();)
				{
					if( pos->first < mCallBackContentClearTag )
					{
						mCallBackContent.erase(pos++);
					}
					else
					{
						++pos;
					}
				}
				mCallBackContentClearTag = mGenGUID;
			}
			t = t / 60 * 60;
			if( t > mOnlineLogTick )
			{
				mOnlineLogTick = t;
			}
			
			if( mScriptMemoryCheckTick + mAutoReloadScriptGap*60*60 < time(0) )
			{
				GetScriptEngine()->reload();
				mScriptMemoryCheckTick = time(0);
			}

			if( mSaveServerVarTick + 30 * 60 < time(0) )
			{
				mSaveServerVarTick = time(0);
				mListener->SaveServerVar(*this,0);
			}
		}
	}
	void GameServerLib::InitDemoCharacter(OGSLib::DomainData* chr,const char* loginID,const char* charID)
	{
		strncpy_s(chr->mAccount,_countof(chr->mAccount),loginID,255);
		chr->mAccount[255]=0;
		strncpy_s(chr->mChrName,_countof(chr->mChrName),charID,255);
		chr->mChrName[255]=0;
		chr->job = JOB_WARRIOR + rand()%JOB_NUMBER;//JOB_WIZARD;//JOB_TAOIST;//JOB_WARRIOR;//JOB_TAOIST;//;
		chr->gender = GENDER_MALE + rand()%GENDER_NUMBER;
	}
	void GameServerLib::CreateSession(const char* loginID,const char* charID,const char* seedID,int authtype,int newyouke,const char* client_ip,GameSessionListener* listener)
	{
		if( FindSessionWithLoginID(loginID) || FindSessionWithCharID(charID) )
		{
			listener->notifySessionCreateResult(0,2);
			listener->notifySessionDataLoaded(0,2);
			GameSession* session = FindSessionWithLoginID(loginID);
			if( session )
			{
				session->BreakNetAndEndSession(mStringConsts.NOTIFY_LOGIN_ANOTHER_PLACE);
			}
			return;
		}
		if( GetDenyManager()->CheckChrDeny(charID) )
		{
			listener->notifySessionDataLoaded(0,4);
			return;
		}
		GameSession* session = new GameSession(this,loginID,charID,seedID,authtype,newyouke,client_ip);
		mLoginIDSessionMap[loginID]=session;
		mCharIDSessionMap[charID]=session;
		mSeedIDSessionMap[seedID]=session;
		if(authtype==AUTH_TYPE_CHAT && mListenChat)
		{
			mChatListenSessionMap[loginID]=session;
		}
		mSessions.push_back(session);
		if( mSessions.size() > m_MaxSessionNum )
		{
			m_MaxSessionNum = mSessions.size();
		}
		session->SetListener(listener);
		session->GetListener()->notifySessionCreateResult(session,0);
		if( authtype == AUTH_TYPE_REAL )
		{
			std::map<std::string,std::string> cm;
			cm["loginID"] = loginID;
			cm["charID"] = charID;
			mCallBackContent[mGenGUID]=cm;
			mListener->LoadCharacter(loginID,charID,seedID,client_ip,mGenGUID++);
			return ;
		}
		if(authtype == AUTH_TYPE_CKUAFU){
			//std::map<std::string,std::string> cm;
			//cm["loginID"] = loginID;
			//cm["charID"] = charID;
			//mCallBackContent[mGenGUID]=cm;
			//mListener->LoadKuafuCharacter(loginID,charID,seedID,client_ip,mGenGUID++);
			session->GetListener()->LoadKuafuCharacter(session,loginID,charID,seedID);
			return;
		}
		if( (authtype == AUTH_TYPE_DEMO)||(authtype == AUTH_TYPE_CHAT) )
		{
			if( newyouke )
			{
				OGSLib::DomainData* chr = new OGSLib::DomainData;
				InitDemoCharacter(chr,loginID,charID);
				session->SetCharacter(chr);
				session->CreatePlayer();
				session->GetListener()->notifySessionDataLoaded(session,0);
			}
			else
			{
				std::map<std::string,std::string> cm;
				cm["loginID"] = loginID;
				cm["charID"] = charID;
				mCallBackContent[mGenGUID]=cm;
				mListener->LoadDemoCharacter(loginID,charID,client_ip,mGenGUID++);
			}
			return ;
		}
		listener->notifySessionDataLoaded(0,1);
		session->EndSession();
	}
	void GameServerLib::SaveSession(GameSession* session,bool quit)
	{
		std::map<std::string,std::string> cm;
		cm["loginID"] = session->LoginID();
		cm["charID"] = session->CharID();
		cm["quit"] = "false";
		if( quit )
		{
			cm["quit"] = "true";
		}
		mCallBackContent[mGenGUID]=cm;
		if( session->AuthType() == AUTH_TYPE_REAL )
		{
			mListener->SaveCharacter(session->GetCharacter(),quit,mGenGUID++);
			if( quit )
			{
				session->GetListener()->notifyUpdateTick(session);
			}
		}
		else if( session->AuthType() == AUTH_TYPE_DEMO || session->AuthType() == AUTH_TYPE_CHAT)
		{
			mListener->SaveDemoCharacter(session->GetCharacter(),quit,mGenGUID++);
			//session->Destroy();
		}
		else if(session->AuthType()==AUTH_TYPE_CKUAFU)
		{
			this->OnSaveCharacter(mGenGUID++,0);
		}
	}
	void GameServerLib::DestroySession(GameSession* session)
	{
		mLoginIDSessionMap.erase(session->LoginID());
		mChatListenSessionMap.erase(session->LoginID());
		mCharIDSessionMap.erase(session->CharID());
		mSeedIDSessionMap.erase(session->SeedID());
		delete session;session=0;
	}
	size_t	GameServerLib::GetSessionNumber()
	{
		return mSessions.size();
	}
	size_t	GameServerLib::GetRegSessionNumber()
	{
		int ret=0;
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			if( session->AuthType() == AUTH_TYPE_REAL )
			{
				ret ++;
			}
		}
		return ret;
	}

	void GameServerLib::ServerAlertMessage(int lv,int flags,const char* msg)
	{
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			DomainPlay* pl = session->GetPlayer();
			if( pl )
			{
				pl->AlertMessage(lv,flags,msg);
			}
		}
	}
	void GameServerLib::ServerPlayEffect(int skill_type,int res_id)
	{
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			DomainPlay* pl = session->GetPlayer();
			if( pl )
			{
				pl->NotifyPlayEffect(pl,skill_type,pl->X(),pl->Y(),pl->ID(),pl->ID(),res_id);
			}
		}
	}
	void GameServerLib::ServerExe(const char* code)
	{
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			DomainPlay* pl = session->GetPlayer();
			if( pl )
			{
				pl->ExeCode(code);
			}
		}
	}
	void GameServerLib::ServerGuild(const char* code)
	{
		const OGSLib::ManagerGuild::GuildMap& gm = GetGuildManager()->GetGuilds();
		for( OGSLib::ManagerGuild::GuildMap::const_iterator pos = gm.begin();
			pos != gm.end();
			++ pos )
		{
			if (pos->second)
			{
				GetScriptEngine()->OnlyGuild(pos->second,code);
			}
		}
	}
	void GameServerLib::WorldChat(DomainPlay* play,char* msg)
	{
		size_t msglen = strlen(msg);
		if( msglen>1 && msg[0] == '@' )
		{
			GMCommand* gmc = GetGMCommand();
			gmc->DoCommand(play,msg+1);
			return;
		}
		if( play && play->GetGMHideTalk() )
		{
			return ;
		}
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			DomainPlay* pl = session->GetPlayer();
			if( pl )
			{
				if( pl->ID() != play->ID() )
				{
					pl->NotifyWorldChat(play,msg);
				}
			}
		}
	}
	void GameServerLib::PublicHornChat(DomainPlay* play,const char* msg)
	{
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			DomainPlay* pl = session->GetPlayer();
			if( pl )
			{
				if( pl->ID() != play->ID() )
				{
					//pl->NotifyWorldChat(play,msg);
					session->GetListener()->notifyHornChat(session,play,msg);
				}
			}
		}
	}
	void GameServerLib::PublicProsperityChange()
	{
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			session->GetListener()->notifyProsperityChange(session,Getprosperity(),GetprosperityNext());
		}
	}
	class SessionStopFuntor
	{
	public:
		bool operator() (GameSession* & session)
		{
			session->EndSession();
			return false;
		}
	};
	void GameServerLib::StopAllSession()
	{
		SessionStopFuntor funtor;
		std::for_each(mSessions.begin(),mSessions.end(),funtor);
	}
	GameSession* GameServerLib::FindSessionWithLoginID(const std::string& loginID)
	{
		LoginIDSessionMap::iterator pos = this->mLoginIDSessionMap.find(loginID);
		if( pos != this->mLoginIDSessionMap.end() )
		{
			return pos->second;
		}
		return 0;
	}
	GameSession* GameServerLib::FindSessionWithCharID(const std::string& charID)
	{
		CharIDSessionMap::iterator pos = this->mCharIDSessionMap.find(charID);
		if( pos != this->mCharIDSessionMap.end() )
		{
			return pos->second;
		}
		return 0;
	}
	GameSession* GameServerLib::FindSessionWithSeedID(const std::string& seedID)
	{
		SeedIDSessionMap::iterator pos = this->mSeedIDSessionMap.find(seedID);
		if( pos != this->mSeedIDSessionMap.end() )
		{
			return pos->second;
		}
		return 0;
	}
	bool GameServerLib::OnFreshGift(const char* chrname,int result,std::vector<OGSLib::SubGift>* gift)
	{
		if( result == 0 )
		{
			GameSession* session=FindSessionWithCharID(chrname);
			if( session )
			{
				OGSLib::DomainPlay* player = session->GetPlayer();
				if( player )
				{
					player->OnFreshGift(gift);
				}
			}
		}
		return true;
	}
	bool GameServerLib::OnFreshVcoin(const std::string& account,int vcoin,int czhaccu,bool gs)
	{
		GameSession* session=FindSessionWithLoginID(account);
		if( session )
		{
			OGSLib::DomainPlay* player = session->GetPlayer();
			if( player )
			{
				player->OnFreshVcoin(vcoin,czhaccu,gs);
			}
		}
		return true;
	}
	bool GameServerLib::OnLoadCharacter(int result,OGSLib::DomainData* character,const char* loginid,const char* charid){
		GameSession* session=FindSessionWithLoginID(loginid);
		if( session )
		{
			if( result != 0 )
			{
				if((session->AuthType() == AUTH_TYPE_DEMO) || (session->AuthType() == AUTH_TYPE_CHAT))
				{
					OGSLib::DomainData* chr = new OGSLib::DomainData;
					InitDemoCharacter(chr,session->LoginID(),session->CharID());
					session->SetCharacter(chr);
					session->CreatePlayer();
					session->GetListener()->notifySessionDataLoaded(session,0);
				}
				else
				{
					session->Destroy();
					session->GetListener()->notifySessionDataLoaded(0,3);
				}
			}
			else
			{
				OGSLib::DomainData* chr = new OGSLib::DomainData;
				*chr = *character;
				//FixItemData106(chr);
				session->SetCharacter(chr);
				session->CreatePlayer();
				session->GetListener()->notifySessionDataLoaded(session,0);
			}
		}
		return true;
	}
	bool GameServerLib::OnLoadCharacter(int GUID,int result,OGSLib::DomainData* character)
	{
		std::map<int,std::map<std::string,std::string> >::iterator pos = mCallBackContent.find(GUID);
		if( pos != mCallBackContent.end() )
		{
			OnLoadCharacter(result,character,pos->second["loginID"].c_str(),pos->second["charID"].c_str());
			mCallBackContent.erase(pos);
		}
		return true;
	}

	bool GameServerLib::OnSaveCharacter(int GUID,int result)
	{
		std::map<int,std::map<std::string,std::string> >::iterator pos = mCallBackContent.find(GUID);
		if( pos != mCallBackContent.end() )
		{
			if( pos->second["quit"] == "true" )
			{
				GameSession* session=FindSessionWithLoginID(pos->second["loginID"]);
				if( session )
				{
					session->Destroy();
				}
			}
			mCallBackContent.erase(pos);
		}
		return true;
	}
	bool GameServerLib::OnLoadGuilds(int GUID,int result,const OGSLib::ManagerGuild& guild_manager)
	{
		if( result == 0 )
		{
			GetGuildManager()->OnLoadGuilds(guild_manager.GetGuilds());
		}
		return true;
	}
	bool GameServerLib::OnSaveGuilds(int GUID,int result)
	{
		return true;
	}
	bool GameServerLib::OnLoadCouples(int GUID,int result,const OGSLib::ManagerCouple& couple_manager)
	{
		if( result == 0 )
		{
			GetCoupleManager()->OnLoadCouples(couple_manager.GetCouples());
		}
		return true;
	}
	bool GameServerLib::OnSaveCouples(int GUID,int result)
	{
		return true;
	}
	bool GameServerLib::OnModifyName(const char* seedname,const char* chrname,int result)
	{
		GameSession* session = FindSessionWithSeedID(seedname);
		if( session )
		{
			OGSLib::DomainPlay* player = session->GetPlayer();
			if( player )
			{
				//player->OnNameModify(chrname,result);
			}
		}
		return true;
	}
	bool GameServerLib::OnFreshItem(const char* seedname,const char* chrname,int result,std::vector<OGSLib::SubGift>* gift)
	{
		if( result == 0 )
		{
			GameSession* session=FindSessionWithSeedID(seedname);
			if( session )
			{
				OGSLib::DomainPlay* player = session->GetPlayer();
				if( player )
				{
					//player->OnFreshItem(gift);
				}
			}
		}
		return true;
	}
	bool GameServerLib::OnLoadMails(int GUID,int result,const std::vector<OGSLib::Mail*>& mailVector)
	{
		if( result )
		{
			GetMailManager()->OnLoadMails(GUID,mailVector);
		}
		return true;
	}
	bool GameServerLib::OnSaveMails(int GUID,int result)
	{
		return true;
	}
	void GameServerLib::StartKuafu()
	{
		GetScriptEngine()->SystemScript("war.onStartKuafu");
		mKuafu = 1;
	}
	void GameServerLib::StopKuafu()
	{
		GetScriptEngine()->SystemScript("war.onStopKuafu");
		mKuafu = 0;
		ClearKfStartPosition();
	}
	void GameServerLib::GetStrongerKuafuGuild(std::string& guild_name)
	{
		const std::string& v=Var("kuafuguild");
		guild_name = v;
	}
	void GameServerLib::SetStrongerKuafuGuild(std::string& guild_name)
	{
		Var("kuafuguild",guild_name);
	}
	void GameServerLib::GetStrongerKuafuArea(std::string& guild)
	{
		const std::string& v=Var("kuafuarea");
		guild = v;
	}
	void GameServerLib::SetStrongerKuafuArea(std::string& guild)
	{
		Var("kuafuarea",guild);
	}
	void GameServerLib::GetKuafuURL(std::string& url)
	{
		url = Var("kuafuurl");
	}
	void GameServerLib::SetKuafuURL(std::string& url)
	{
		Var("kuafuurl",url);
	}
	void GameServerLib::GetKuafuPort(std::string& port)
	{
		port = Var("kuafuport");
	}
	void GameServerLib::SetLocalDomain(std::string& domain)
	{
		Var("localdomain",domain);
	}
	void GameServerLib::GetLocalDomain(std::string& domain)
	{
		domain = Var("localdomain");
	}
	void GameServerLib::SetKuafuPort(std::string& port)
	{
		Var("kuafuport",port);
	}
	const std::string& GameServerLib::Var(const std::string& key)
	{
		std::map<std::string,std::string>::iterator pos = mVars.find(key);
		if( pos != mVars.end() )
		{
			return pos->second;
		}
		return mVarDefault;
	}
	void GameServerLib::Var(const std::string& key,const std::string& value)
	{
		if( key.length() )
		{
			mVars[key] = value;
		}
	}
	const std::string& GameServerLib::TempVar(const std::string& key)
	{
		std::map<std::string,std::string>::iterator pos = mTempVars.find(key);
		if( pos != mTempVars.end() )
		{
			return pos->second;
		}
		return mVarDefault;
	}
	void GameServerLib::TempVar(const std::string& key,const std::string& value)
	{
		if( key.length() )
		{
			mTempVars[key] = value;
		}
	}
	bool GameServerLib::OnLoadAuto(int GUID,int result,std::vector<OGSLib::AutoRunDB>* res)
	{
		mAutoRunManager->mAutoRunDB = *res;
		return true;
	}
	bool	GameServerLib::OnLoadServerVar(int GUID,int result,const std::map<std::string,std::string>& vars)
	{
		if( result == 0 )
		{
			mVars = vars;
		}
		return true;
	}
	bool	GameServerLib::OnSaveServerVar(int GUID,int result)
	{
		return true;
	}
	void GameServerLib::StartWar()
	{
		mWar = 1;
		GetScriptEngine()->SystemScript("war.onStart");
		PublicWarInfo();
	}
	void GameServerLib::StopWar()
	{
		mWar = 0;
		GetScriptEngine()->SystemScript("war.onStop");
		PublicWarInfo();
	}
	void  GameServerLib::SetServerDays()
	{
		ServerStartDay = GetScriptEngine()->GetServerStartDay("player.onGetServerStartDay");
		if (!show_server_start)
		{
			show_server_start = true;
			printf("server_start_day:%d\n", ServerStartDay+1);
		}
	}
	void  GameServerLib::SetMergeDays()
	{
		MergeStartDay = GetScriptEngine()->GetServerStartDay("player.onGetMergeStartDay");
		if (!show_merge_start)
		{
			show_merge_start = true;
			printf("merge_start_day:%d\n", MergeStartDay+1);
		}
	}
	void  GameServerLib::SetAcitivityDays(int begin,int end,int today)
	{
		ActivityBegin = begin;
		ActivityEnd = end;
		TodayData = today;
	}
	void GameServerLib::GetKingGuild(std::string& guild)
	{
		const std::string& v=Var("_king_guild");
		guild = v;
	}
	void GameServerLib::GetKingGuild(char* guild_name,size_t s)
	{
		const std::string& v=Var("_king_guild");
		strncpy_s(guild_name,s,v.c_str(),s);
		guild_name[s-1] = 0;
	}
	void GameServerLib::SetKingGuild(std::string& guild_name)
	{
		Var("_king_guild",guild_name);
		PublicWarInfo();
	}
	int GameServerLib::SetKuaFuComeBackData(std::string& charID,int vcoin,int kuafuResult)
	{
		GameSession* session = FindSessionWithCharID(charID);
		if(session)
		{
			DomainData* data = session->GetCharacter();
			if(data)
			{
				data->vcoin = vcoin;
				data->mKuafuResult = kuafuResult;
				return 1;
			}
		}
		return 0;
	}
	void GameServerLib::Log(const char* msg)
	{
		GetListener()->Log(msg);
	}

	void GameServerLib::PublicWarInfo()
	{
		std::string kg;
		GetKingGuild(kg);
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			session->GetListener()->notifyWarInfo(session,GetWarState(),kg);
		}
	}
	void GameServerLib::StartProsperity()
	{
		mProsperity = rand() % 50 + 50;
		mProsperityNext = rand() % 60 - 30;
		while( mProsperityNext == 0 )
		{
			mProsperityNext = rand() % 30 - 60;
		}
		mProsperityTick=time(0);
		mProsperityGap=60;
	}
	void GameServerLib::UpdateProsperity()
	{
		time_t t = time(0);
		if( mProsperityTick + mProsperityGap < t )
		{
			mProsperityTick = t;
			if( mProsperityNext > 0 )
			{
				mProsperityNext --;
				mProsperity ++;
			}
			else if( mProsperityNext < 0 )
			{
				mProsperityNext ++;
				mProsperity --;
			}
			if( mProsperity > 100 )
			{
				mProsperity = 100;
				mProsperityNext = rand() % 15 - 30;
			}
			if( mProsperity < 5 )
			{
				mProsperity = 5;
				mProsperityNext = rand() % 45 + 45;
			}
			while( mProsperityNext == 0 )
			{
				mProsperityNext = rand() % 30 - 60;
			}
			//PublicProsperityChange();
			//printf("mProsperity : %d  mProsperityNext : %d\n",mProsperity,mProsperityNext);
		}
	}
	void GameServerLib::StopProsperity()
	{
	}
	const std::string GameServerLib::ServerExeRand(const char* name,int num,int level)
	{
		if (num < 0 || num >= 1000)
			return "";
		std::map<int,std::set<std::string>> tempMap;
		int index = 0;
		for(std::vector<GameSession*>::iterator pos = mSessions.begin();
			pos != mSessions.end();
			++ pos )
		{
			GameSession* session = *pos;
			GameSession* player_session = FindSessionWithCharID(name);
			DomainPlay* mpl = player_session->GetPlayer();
			DomainPlay* pl = session->GetPlayer();
			if( pl && mpl )
			{
				if ((pl->level() <= (level+10) || pl->level() >= (level-10)) && !mpl->IsFriend(pl->Name()) && pl != mpl)
				{
					tempMap[pl->level()].insert(pl->Name());
				}
			}
		}
		/*for (int i = 0;i < 8;i++)
		{
			int level = RandRange2(10,60);
			char str[16];
			sprintf(str,"%d",RandRange2(100,200));
			std::string name = str ;
			tempMap[level].insert(name);
		}*/
		std::string name_str  = "";
		for (int i = 0;i<20;i++)
        {
            int cur_level = level + (i & 1 ? -1 : 1)*static_cast<int>((i + 1) / 2);
			if (tempMap[cur_level].size() > 0)
			{
				for(std::set<std::string>::iterator va = tempMap[cur_level].begin();
					va != tempMap[cur_level].end();
					++ va )
				{
					if (index < num)
					{
						if (*va != "")
						{
							name_str += *va + "_";
							index ++;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		return name_str;
	}

	bool GameServerLib::OnLoadConsignment(int GUID,int result,const std::vector<ConsignableItemInfo *> info_list, std::map<std::string, int> user_vcoin_map)
	{
		if( result == 0 )
		{
			GetConsignmentManager()->OnLoadConsignment(info_list, user_vcoin_map);
		}
		return true;
	}

	bool GameServerLib::OnSaveConsignment(int GUID,int result)
	{
		return true;
	}

	bool GameServerLib::OnLoadRelationCache(int GUID,int result,const std::vector<RelationShipOper *> oper_list)
	{
		if( result == 0 )
		{
			GetRelationCacheManager()->OnLoadRelationCache(oper_list);
		}
		return true;
	}

	bool GameServerLib::OnSaveRelationCache(int GUID,int result)
	{
		return true;
	}
}
