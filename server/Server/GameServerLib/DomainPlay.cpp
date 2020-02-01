#include "StdAfx.h"
#include "DomainPlay.h"
#include "GameSession.h"
#include "PlayMap.h"
#include "PlayNpc.h"
#include "GameServerlib.h"
#include "ManagerMap.h"
#include "SubItem.h"
#include "SubSkill.h"
#include "SubAction.h"
#include "ScriptLua.h"
#include "SubTask.h"
#include "SubGift.h"
#include "SubFriend.h"
#include "SubMail.h"
#include "GmCommand.h"
#include "ManagerLevelInfo.h"
#include "ManagerTotalAttr.h"
#include "ManagerItemDef.h"
#include "ManagerSkillDef.h"
#include "PlayMapItem.h"
#include "DomainData.h"
#include "GuildGuild.h"
#include "ManagerGuild.h"
#include "PlaySlave.h"
#include "DomainGroup.h"
#include "GroupGroup.h"
#include "ManagerGroup.h"
#include "DomainGuild.h"
#include "ManagerMonDef.h"
#include "ManagerNotice.h"
#include "ManagerFilter.h"
#include "ManagerDenySell.h"
#include "ManagerChart.h"
#include "PlayMonsterDummy.h"
#include "SubAchieve.h"
#include "CoupleCouple.h"
#include "DomainCouple.h"
#include "ManagerCouple.h"
#include "ManagerMail.h"
#include "ManagerStatus.h"
#include "ManagerDeny.h"
#include "ManagerFly.h"
#include "ManagerSwordDef.h"
#include "ManagerRelationCache.h"
#include "ManagerConsignment.h"
#include "ManagerMonster.h"
#include <vector>
#include <iostream>
#include <sstream>
#include "ManagerObjFuncs.h"

namespace OGSLib
{
	class EmptyItemRemover
	{
	public:
		bool operator() (SubItem & ii)
		{
			if( ii.mTypeID <= 0 )
			{
				return true;
			}
			return false;
		}
	};
	class EmptyFriendRemover
	{
	public:
		bool operator() (SubFriend & sf)
		{
			if( sf.mTitle == 0 )
			{
				return true;
			}
			return false;
		}
	};

	DomainPlay::DomainPlay(GameSession* session)
	{
		mType = MAP_OBJECT_TYPE_PLAYER;
		mSession = session;
		mGSL = mSession->GetGameServerLib();
		mCharacter = mSession->GetCharacter();

		mLockAction=0;
		mLockShow=0;
		mExp=0;
		mGameMoney = 0;
		mVcoin = 0;
		mGameMoneyBind = 0;
		mVcoinBind = 0;
		mVcoinGS = 0;

		mHonor = 0;
		mUpGradeLv = 0;
		mUpGradeMinLv = 0;

		m_nFightPoint = 0;

		mPKconfirm = 0;
		mCountDownTick=0;
		mCountDownTag=0;

		mTili = 100;
		mTiliRecoverTick = 0;
		mPowerRecoverTick = 0;
		mPowerRecoverGap = 1;
		mPowerRecoverPercent = 2000;
		mPowerRecoverSpeed = 0;
		mPowerRecoverVal = 0;

		mCountClientSell=200;

		mJob = JOB_WARRIOR;
		mGender = GENDER_MALE;
		mMount = 0;
		mMountTypeID = 0;
		mMountLock = 0;
		mMountLockUseSkill=0;
		mShowMount = 0;
		mChangeMountTick=0;
		mCaijiTick=0;

		mSAP.mAccuracy=0;
		mSAP.mDodge=0;
		mSAP.mAntiMagic=0;

		mActionCheckCount=0;
		mActionCheckTick=0;
		mSkillCheckCount=0;
		mSkillCheckTick=0;

		mGuildPt = 0;
		mGuildSpt = 0;
		mFirstTarget = 0;

		mTotalUpdLevel = 0;

		mNeedClearItemList = true;
		mNeedClearChatTradeItemList=true;
		mNeedFreshItemList = true;
		mFreshItemTime = time(0);

		mAttackMode = ATTACK_MODE_TYPE_PEACE;
		mLastSafeAreaID = -1;

		mLockAttackMode = false;

		mPuppetMul = 1000;

		mSlaveKuLou = 0;
		mSlaveShenShou = 0;
		for(int i=0;i<5;i++)
		{
			mSlaves[i] = 0;
		}

		mSlaveHuwei = 0;

		mUpdateHPMPInc = 0.0;
		mUpdateHPMPStatusInc =0.0;

		mTradeGameMoney=0;
		mTradeVcoin=0;
		mTradeSubmited = false;


		mStallStart=false;

		mPKValue = 0;
		mPKValueTick = mGSL->GetTimerFix()->Elapsed();
		mPKState = 0;
		mPKStateTick = mGSL->GetTimerFix()->Elapsed();

		mDepotSlotAdd = 0;
		mBagSlotAdd = 0;

		//mCoupleID = 0;
		//mWeddingdate = 0;
		//mMarryTimes = 0;

		mDressRing=0;
		mDressGlove=0;

		mVcoinBind = 0;
		mVcoinAccuByMonth = 0;
		mLastCongzhiMonth = 0;
		mVcoinUsedByDay = 0;
		mVcoinUsedByMonth = 0;
		mLastVcoinUsedDay = 0;
		mNeedRecordDayVcoin = 1;

		mEquipMoXueShi=false;

		mItemUseTick = 0;
		mItemUseAddHPTick=0;
		mItemuseAddHPCount=0;

		mNoticeTick = 0;

		mFriendFreshTick = 0;

		mSaveTick = 0;
		mFreshVcoinTick = 0;
		mTalkProhibited = 0;

		mSecondTick=0;
		mMiniTick=0;
		mSecondTickGap=1.0;
		mMiniTickGap=0.3;

		mDartTick=0;

		mMapEnterTick = 0;
		china_limit_lv = 0;

		mNpcList.reserve(50);
		mScriptEcho.reserve(1024);
		mItemList.reserve(50);
		mChatTradeItemList.reserve(50);
		mSkillDespSendList.reserve(50);

		mTeamID=0;
		mTeamName[0]=0;
		mSeedName[0]=0;

		mGMLevel = 0;
		mGMHideTalk = 0;

		mUpdateChartGamemoneyTimeTick = time(0);
		mUpdateChartGamemoneyTick=0;

		mSlaveKuLouLevel=0;
		mSlaveShenShouLevel=0;
		
		china_limit_lv_tick = time(0);
		mEnteringMap = 0;
		mBoothOpen = false;

		mAchievePoint=0;
		mAchieveGameMoneyMax=0;
		mOnlineTimeMax=0;

		mAchieveFirstKillMonster=0;
		mAchieveFirstAddFriend=0;
		mAchieveFirstGroup=0;
		mAchieveFirstGuild=0;
		mAchieveFirstPKKill=0;
		mAchieveFirstUpdEquip=0;
		mAchieveFirstMergeSteel=0;

		mDart2 = 0;
		mAcceptDartTaskNumberUsed = 0;
		//mRobDartNumberUsed = 0;
		mCollectTarget=0;

		mShowFashion = true;
		mFabaoResID = 0;

		mYuanqiBuffTick = 0;
		mApplyFriendTick = 0;
		mGetConsignableItemsTick = 0;
		mBuyConsignableItemsTick = 0;
		m_nTaskMonId = 0;
		mMainTaskState = 0;
	}
	DomainPlay::~DomainPlay(void)
	{
		//delete mDart2;mDart2=0;
		delete mSlaveKuLou;mSlaveKuLou=0;
		delete mSlaveShenShou;mSlaveShenShou=0;
		for(int i =0;i<5;i++)
		{
			delete mSlaves[i];mSlaves[i]=0;
		}

		for (size_t i = 0; i < mSlaveList.size(); ++ i)
		{
			SAFE_DELETE(mSlaveList[i]);
		}
	}
	bool DomainPlay::check_skill(double dura)
	{
		if( 0 == mGSL->mCheckSecondSkill ) return true;
		time_t t = time(0);
		mSkillCheckCount = max(mSkillCheckCount - (t-mSkillCheckTick),0);
		mSkillCheckTick=t;
		if( mSkillCheckCount >= mGSL->mCheckSecondSkillParam )
		{
			//printf("check skill error, %f ",mSkillCheckCount);
			return false;
		}
		mSkillCheckCount+=dura;
		return true;
	}
	bool DomainPlay::check_action(double dura)
	{
		if( 0 == mGSL->mCheckSecondAction ) return true;
		time_t t = time(0);
		mActionCheckCount = max(mActionCheckCount - (t-mActionCheckTick),0);
		mActionCheckTick=t;
		if( mActionCheckCount >= mGSL->mCheckSecondActionParam )
		{
			//printf("check action error, %f ",mActionCheckCount);
			return false;
		}
		mActionCheckCount+=dura;
		return true;
	}
	
	int DomainPlay::GroupSize()
	{
		if(mSession && mSession->GetDomainGroup())
			return mSession->GetDomainGroup()->GroupSize();
		return 0;
	}
	void DomainPlay::GroupExe(const char* code)
	{
		if(mSession && mSession->GetDomainGroup())
			mSession->GetDomainGroup()->GroupExe(code);
	}
	int DomainPlay::GroupMemberOnLineNum()
	{
		if(mSession && mSession->GetDomainGroup())
			return mSession->GetDomainGroup()->GroupMemberOnLineNum();
		return 0;
	}
	int DomainPlay::GuildSize()
	{
		if(mSession && mSession->GetDomainGuild())
			return mSession->GetDomainGuild()->GuildSize();
		return 0;
	}
	int DomainPlay::FriendNum()
	{
		return mFriendArray.nFriend;
	}
	void DomainPlay::GuildExe(const char* code)
	{
		if(mSession && mSession->GetDomainGuild())
			mSession->GetDomainGuild()->GuildExe(code);
	}
	void DomainPlay::FriendExe(const char* code)
	{
		if (mFriendArray.reltionship.size() >= 1)
		{
			for( std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();
				pos != mFriendArray.reltionship.end();
				++ pos )
			{
				GameSession* ts = mGSL->FindSessionWithCharID(pos->mName);
				if (ts && ts->GetPlayer())
				{
					DomainPlay* target = ts->GetPlayer();
					if (target)
					{
						target->ExeCode(code);
					}
				}
			}
		}
		else
		{
			AlertMessage(11, 0, mGSL->mStringConsts.NOTIFY_NO_FRIEND_ONLINE);
		}
	}
	const std::string& DomainPlay::GroupLeader()
	{
		if(mSession && mSession->GetDomainGroup())
			return mSession->GetDomainGroup()->GetGroupLeader();
		return "";
	}
	int DomainPlay::GroupID()
	{
		if(mSession && mSession->GetDomainGroup())
			return mSession->GetDomainGroup()->GroupID();
		return 0;
	}
	void DomainPlay::ExeCode(const char* code)
	{
		mScriptEcho = "";
		mGSL->GetScriptEngine()->OnlyPlayer(this,code);
		if( ! mScriptEcho.empty() )
		{
			mPlayerTalkSeed = rand();
			mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
		}
		mScriptEcho = "";
	}
	void DomainPlay::ExeCodeClient(const char* param)
	{
		mScriptEcho = "";
		mGSL->GetScriptEngine()->PlayerString(this,"player.onClientTalk",param);
		if( ! mScriptEcho.empty() )
		{
			mPlayerTalkSeed = rand();
			mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
		}
		mScriptEcho = "";
	}
	
	bool DomainPlay::IsGroupMember(int id)
	{
		PlayActiveObject* obj = FindActiveInView(id);
		if( obj && obj->TypeIsPlayer() )
		{
			DomainPlay* des = static_cast<DomainPlay*>(obj);
			if( des && des->GroupID() > 0 && des->GroupID() == GroupID() )
			{
				if( mSession && mSession->GetDomainGroup())
				{
					if( mSession->GetDomainGroup()->GetPickMode() == 0 )
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	const char* DomainPlay::LoverName()
	{
		return "";
		//std::string loversdn = mSession->GetDomainCouple()->GetLoverSeedName();
		//GameSession* gs = mGSL->FindSessionWithSeedID(loversdn);
		//if( gs ){return gs->CharID();}
		//return mSession->GetDomainCouple()->GetLoverName();
	}
	int DomainPlay::FightPoint()
	{
		return m_nFightPoint;
	}

	void DomainPlay::FightPoint(int nFight)
	{
		m_nFightPoint = nFight;
	}

	const std::string DomainPlay::GuildID()
	{
		if (mSession && mSession->GetDomainGuild())
		{
			return mSession->GetDomainGuild()->GuildID();
		}
		return "";
	}

	const std::string& DomainPlay::GuildName()
	{
		if(gsl()->GetListener()->GetKuaFuService())
		{
			return mGuildName;
		}
		else
		{
			if (mSession && mSession->GetDomainGuild())
			{
				return mSession->GetDomainGuild()->GetGuildName();
			}
			return mGuildName;
		}
	}

	void DomainPlay::OnUpdEquip()
	{
		if( mAchieveFirstUpdEquip <= 0 )
		{
			mAchieveFirstUpdEquip = GetAchieveState(AchieveTypeFirstUpdEquip);
			if( mAchieveFirstUpdEquip <= 0 )
			{
				SetAchieveParam(AchieveTypeFirstUpdEquip,1);
				AddOneAchieveState(AchieveTypeFirstUpdEquip);
				mAchieveFirstUpdEquip = 1;
			}
		}
	}
	void DomainPlay::OnMergeSteel()
	{
		if( mAchieveFirstMergeSteel <= 0 )
		{
			mAchieveFirstMergeSteel = GetAchieveState(AchieveTypeFirstMergeSteel);
			if( mAchieveFirstMergeSteel <= 0 )
			{
				SetAchieveParam(AchieveTypeFirstMergeSteel,1);
				AddOneAchieveState(AchieveTypeFirstMergeSteel);
				mAchieveFirstMergeSteel = 1;
			}
		}
	}
	void DomainPlay::OnPKKill()
	{
		if( mAchieveFirstPKKill <= 0 )
		{
			mAchieveFirstPKKill = GetAchieveState(AchieveTypeFirstPKKill);
			if( mAchieveFirstPKKill <= 0 )
			{
				SetAchieveParam(AchieveTypeFirstPKKill,1);
				AddOneAchieveState(AchieveTypeFirstPKKill);
				mAchieveFirstPKKill = 1;
			}
		}
	}
	void DomainPlay::OnEnterGuild()
	{
		if( mAchieveFirstGuild <= 0 )
		{
			mAchieveFirstGuild = GetAchieveState(AchieveTypeFirstGuild);
			if( mAchieveFirstGuild <= 0 )
			{
				SetAchieveParam(AchieveTypeFirstGuild,1);
				AddOneAchieveState(AchieveTypeFirstGuild);
				mAchieveFirstGuild = 1;
			}
		}
		ExeCode("player.onEnterGuild");
	}
	void DomainPlay::OnLeaveGroup()
	{
		ExeCode("player.onLeaveGroup");
	}
	int DomainPlay::CheckCreateLvGuild(int lv)
	{
		int ret = mSession->GetGameServerLib()->GetScriptEngine()->PlayerInt(this,lv,0,0,0,"guild.checkCreateGuild");
		return ret;
	}
	void DomainPlay::OnEnterGroup()
	{
		if( mAchieveFirstGroup <= 0 )
		{
			mAchieveFirstGroup = GetAchieveState(AchieveTypeFirstGroup);
			if( mAchieveFirstGroup <= 0 )
			{
				SetAchieveParam(AchieveTypeFirstGroup,1);
				AddOneAchieveState(AchieveTypeFirstGroup);
				mAchieveFirstGroup = 1;
			}
		}
		ExeCode("player.onEnterGroup");
	}
	int DomainPlay::GuildTitle()
	{
		if(gsl()->GetListener()->GetKuaFuService())
		{
			return GUILD_TITLE_TYPE_NORMAL;
		}
		else
		{
			if (mSession && mSession->GetDomainGuild())
			{
				return mSession->GetDomainGuild()->GuildTitle();
			}
			return 0;
		}
	}
	int DomainPlay::SubGuildPt(int pt)
	{
		if( pt > 0 && (!GuildName().empty()) && GuildTitle() > GUILD_TITLE_TYPE_ENTERING )
		{
			if( mGuildPt >= pt )
			{
				mGuildPt -= pt;
			}
			if (mSession && mSession->GetDomainGuild())
			{
				mSession->GetDomainGuild()->SetGuildPt();
				return mGuildPt;
			}
		}
		return -1;
	}
	void DomainPlay::AddGuildPt(int add)
	{
		if( add > 0 && (!GuildName().empty()) && GuildTitle() > GUILD_TITLE_TYPE_ENTERING )
		{
			mGuildPt += add;
			mGuildSpt += add;
			if (mSession && mSession->GetDomainGuild())
			{
				mSession->GetDomainGuild()->SetGuildPt();
			}
		}
	}
	int DomainPlay::GuildEnterTime()
	{
		if ( (!GuildName().empty()) && GuildTitle() > GUILD_TITLE_TYPE_ENTERING )
		{
			if (mSession && mSession->GetDomainGuild())
			{
				return mSession->GetDomainGuild()->GetEnterTime();
			}
		}
		return 0;
	}
	const char* DomainPlay::LoginID()
	{
		return mSession->LoginID();
	}
	const char* DomainPlay::Name()
	{
		return mSession->CharID();
	}
	const std::string& DomainPlay::GetNamePre()
	{
		if(mParentMap)
		{
			if(mParentMap->FightAnonym())
			{
				return mNullStr;
			}
		}
		return mNamePre;
	}
	const std::string& DomainPlay::GetNamePro()
	{
		if(mParentMap)
		{
			if(mParentMap->FightAnonym())
			{
				return mNullStr;
			}
		}
		return mNamePro;
	}
	int DomainPlay::setName(const char* name)
	{
		DomainData* d = mSession->GetCharacter();
		strncpy_s(d->mChrName,_countof(d->mChrName),name,255);
		d->mChrName[255]=0;
		return 1;
	}
	const char* DomainPlay::SeedName()
	{
		return mSeedName;
	}
	void DomainPlay::NotifyMapEnter(PlayMap* map)
	{
		PlayMapObject::NotifyMapEnter(map);
		if( mSlaveKuLou && mSlaveKuLou->mLiving )
		{
			mSlaveKuLou->ForceMove(X(),Y(),Dir());
			mParentMap->AddObject(mSlaveKuLou);
		}
		if( mSlaveShenShou && mSlaveShenShou->mLiving )
		{
			mSlaveShenShou->ForceMove(X(),Y(),Dir());
			mParentMap->AddObject(mSlaveShenShou);
		}
		if( mSlaveHuwei&& mSlaveHuwei->mLiving )
		{
// 			if (map->NoGod())
// 			{
// 				kill_slave(10);
// 			}
// 			else
// 			{
				mSlaveHuwei->ForceMove(X(),Y(),Dir());
				mParentMap->AddObject(mSlaveHuwei);
//			}
		}
		for( int i=0;i<5;i++)
		{
			if( mSlaves[i] && mSlaves[i]->mLiving )
			{
				mSlaves[i]->ForceMove(X(),Y(),Dir());
				mParentMap->AddObject(mSlaves[i]);
			}
		}

		for (size_t i = 0; i < mSlaveList.size(); ++ i)
		{
			if( mSlaveList[i] && mSlaveList[i]->mLiving )
			{
				mSlaveList[i]->ForceMove(X(),Y(),Dir());
				mParentMap->AddObject(mSlaveList[i]);
			}
		}

		if(map->FightAnonym())
		{
			this->ChangeAttackMode(ATTACK_MODE_TYPE_ALL);
			this->LockAttackMode(1);
		}
		else
		{
			this->LockAttackMode(0);
		}
		mSession->GetListener()->notifyMapEnter(mSession,map);
	}
	bool DomainPlay::UseGameMoney(LEUD_INT64 num,int src)
	{
		if( num > 0 && gamemoney()+gamemoneybind() >= num )
		{
			int gmc = 0;
			int gmbc = 0;
			if( gamemoneybind() >= num )
			{
				mGameMoneyBind -= (int)num;
				gmbc = (int)num;
			}
			else
			{
				num -= mGameMoneyBind;
				gmbc = (int)mGameMoneyBind;
				mGameMoneyBind = 0;
				mGameMoney-=num;
				gmc = (int)num;
			}
			if( gamemoneybind() < 0 ) mGameMoneyBind = 0;
			if( gamemoney() < 0 ) mGameMoney = 0;
			mSession->GetListener()->notifyGameMoneyChange(mSession,-gmc,0,-gmbc,0);
			return true;
		}
		return false;
	}
	bool DomainPlay::AddGameMoney(LEUD_INT64 number,int src)
	{
		if( number > 0 )
		{
			gamemoney(gamemoney() + number);
			if( gamemoney() > 3000000000 )
			{
				AlertMessage(110, 0, mGSL->mStringConsts.NOTIFY_GAMEMONEY_ENOUGH);
				gamemoney(3000000000);
			}
			mSession->GetListener()->notifyGameMoneyChange(mSession,number,0,0,0);
			return true;
		}
		return false;
	}
	bool DomainPlay::RemGameMoney(LEUD_INT64 number,int src)
	{
		if( number == 0 )return true;
		if( number > 0 && gamemoney() >= number )
		{
			gamemoney(gamemoney() - number);
			if( gamemoney() < 0 )
			{
				gamemoney(0);
			}
			mSession->GetListener()->notifyGameMoneyChange(mSession,-number,0,0,0);
			return true;
		}
		return false;
	}
	bool DomainPlay::AddGameMoneyBind(LEUD_INT64 number,int src)
	{
		if( number > 0 )
		{
			//if( china_limit_lv == 3 )
			//{
			//	char temp[512];_snprintf_s(temp,_countof(temp),511,mGSL->mStringConsts.NOTIFY_CHINA_LIMIT_GAMEMONEYBIND_NOTIFY,number,0);temp[511]=0;
			//	AlertMessage(1100,0,temp);
			//	number = 0;
			//}
			//else if( china_limit_lv == 2 )
			//{
			//	char temp[512];_snprintf_s(temp,_countof(temp),511,mGSL->mStringConsts.NOTIFY_CHINA_LIMIT_GAMEMONEYBIND_NOTIFY,number,number/2);temp[511]=0;
			//	AlertMessage(1100,0,temp);
			//	number /= 2;
			//}
			mGameMoneyBind += number;
			if (mGameMoneyBind > 3000000000)
			{
				AlertMessage(110, 0, mGSL->mStringConsts.NOTIFY_GAMEMONEYBIND_ENOUGH);
				mGameMoneyBind = 3000000000;
			}
			mSession->GetListener()->notifyGameMoneyChange(mSession,0,0,number,0);
			return true;
		}
		return false;
	}
	bool DomainPlay::RemGameMoneyBind(LEUD_INT64 number,int src)
	{
		if( number > 0 && mGameMoneyBind >= number )
		{
			mGameMoneyBind -= number;
			if( mGameMoneyBind < 0 )
			{
				mGameMoneyBind = 0;
			}
			mSession->GetListener()->notifyGameMoneyChange(mSession,0,0,-number,0);
			return true;
		}
		return false;
	}
	bool DomainPlay::AddVcoin(int number, int src, const std::string& desp)
	{
		if( number > 0 )
		{
			if (number > 2000000000 - mVcoin)
			{
				number = 2000000000 - mVcoin;
				AlertMessage(110, 0, mGSL->mStringConsts.NOTIFY_VCOIN_ENOUGH);
			}
			if(number <= 0) return false;
			mVcoin += number;
			mSession->GetListener()->notifyGameMoneyChange(mSession,0,number,0,0);
			return true;
		}
		return false;
	}
	bool DomainPlay::AddVcoinGS(int number, int src, const std::string& desp)
	{
		if( number > 0 )
		{
			if (number > 2000000000 - mVcoin - mVcoinGS)
			{
				number = 2000000000 - mVcoin - mVcoinGS;
				AlertMessage(110, 0, mGSL->mStringConsts.NOTIFY_VCOIN_ENOUGH);
			}
			if(number <= 0) return false;
			mVcoinGS += number;
			mVcoin += number;

			mSession->GetListener()->notifyGameMoneyChange(mSession,0,number,0,0);
			return true;
		}
		return false;
	}
	bool DomainPlay::RemVcoin(int number, int src, const std::string& desp)
	{
		if(number>(mVcoin-mVcoinGS) && (src==108||src==200))
		{
			return false;
		}
		if( number > 0 && mVcoin >= number )
		{
			mVcoin -= number;

			if(mVcoinGS>0)mVcoinGS-=number;
			if(mVcoinGS<0)mVcoinGS=0;

			mSession->GetListener()->notifyGameMoneyChange(mSession,0,-number,0,0);

			if (1 == mNeedRecordDayVcoin)
			{
				time_t t = time(0);
				char tmp[64]; int day = 0;
				tm ttmm;localtime_s(&ttmm,&t);
				strftime(tmp,sizeof(tmp),"%Y%m%d",&ttmm);
				day = atoi(tmp);

				if (mLastVcoinUsedDay == day)
				{
					mVcoinUsedByDay += number;
				}
				else
				{
					mLastVcoinUsedDay = day;
					mVcoinUsedByDay = number;
				}
				int cur_month = 0;int rec_month = 0;
				cur_month = day/100;rec_month = mLastVcoinUsedDay / 100;
				if (rec_month == cur_month)
				{
					mVcoinUsedByMonth += number;
				}
				else
				{
					mLastVcoinUsedDay = day;
					mVcoinUsedByMonth = number;
				}
			}
			else
			{
				mNeedRecordDayVcoin = 1;
			}
			mGSL->GetScriptEngine()->PlayerCongzi(this,number,mVcoinUsedByDay,src,"player.onRemVcoin");
			return true;
		}
		return false;
	}
	bool DomainPlay::AddVcoinBind(int number,int src)
	{
		if( number > 0 )
		{
			if (number > 2000000000 - mVcoinBind)
			{
				number = 2000000000 - mVcoinBind;
				AlertMessage(110, 0, mGSL->mStringConsts.NOTIFY_VCOINBIND_ENOUGH);
			}
			if(number <= 0) return false;
			mVcoinBind += number;
			mSession->GetListener()->notifyGameMoneyChange(mSession,0,0,0,number);
			return true;
		}
		return false;
	}
	bool DomainPlay::RemVcoinBind(int number,int src)
	{
		if( number > 0 && mVcoinBind >= number )
		{
			mVcoinBind -= number;
			mSession->GetListener()->notifyGameMoneyChange(mSession,0,0,0,-number);
			mGSL->GetScriptEngine()->PlayerCongzi(this,number,0,src,"player.onRemVcoinBind");
			return true;
		}
		return false;
	}
	std::string& DomainPlay::Param(int id)
	{
		ParamMap::iterator pos = mParams.find(id);
		if( pos != mParams.end() )
		{
			return pos->second;
		}
		return mParamDefault;
	}
	void DomainPlay::Param(int id,std::string& value)
	{
		ParamMap::iterator pos = mParams.find(id);
		if( pos != mParams.end() )
		{
			if( value.empty() )
			{
				mParams.erase(pos);
				return;
			}
		}
		mParams[id] = value;
		if (id >= 1000 && id < 2000)
		{
			mSession->GetListener()->notifyParamData(mSession,ID(),id,value);
		}
	}
	void DomainPlay::Param(int id,int value)
	{
		mNumParams[id] = value;
		//脚本设置转生等级
		if (id == 130)
		{
			mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onZsLevelUp");
		}
	}
	void DomainPlay::PushParamData()
	{
		mSession->GetListener()->notifyParamDataList(mSession,this);
		//for (ParamMap::iterator pos = mParams.begin();
		//	pos != mParams.end();
		//	++ pos)
		//{
		//	mSession->GetListener()->notifyParamData(mSession,ID(),pos->first,pos->second);
		//}
	}
	std::string& DomainPlay::TempParam(int id)
	{
		ParamMap::iterator pos = mTempParams.find(id);
		if( pos != mTempParams.end() )
		{
			return pos->second;
		}
		return mParamDefault;
	}
	void DomainPlay::TempParam(int id,std::string& value)
	{
		ParamMap::iterator pos = mTempParams.find(id);
		if( pos != mTempParams.end() )
		{
			if( value.empty() )
			{
				mTempParams.erase(pos);
				return;
			}
		}
		mTempParams[id] = value;
	}
	int DomainPlay::SetPKState(int state)
	{
		if( state == 0 || state == 1 )
		{
			mPKStateTick = mGSL->GetTimerFix()->Elapsed();
			if( mPKState != state )
			{
				mPKState = state;
				NotifyPKStateChange(this);
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();
					++ pos )
				{
					if ((*pos))
					{
						if ((*pos)->TypeIsPlayer())
						{
							DomainPlay* play = (DomainPlay*)(*pos);
							play->NotifyPKStateChange(this);
						}
					}
				}
			}
		}
		return mPKState;
	}
	int DomainPlay::AddPKValue(int value)
	{
		//暂时注掉PK值
		if( value > 0 )
		{
			mPKValue += value;
			if( mPKValue > 1000 )
			{
				mPKValue = 1000;
			}
			mPKValueTick = mGSL->GetTimerFix()->Elapsed();
			//this->AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_PK_VALUE_DESP);
			NotifyPKStateChange(this);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsPlayer())
					{
						DomainPlay* play = (DomainPlay*)(*pos);
						play->NotifyPKStateChange(this);
					}
				}
			}
		}
		return mPKValue;
	}
	int DomainPlay::SubPKValue(int value)
	{
		//暂时注掉PK值
		if( value > 0 )
		{
			mPKValue -= value;
			if( mPKValue < 0 )
			{
				mPKValue = 0;
			}
			mPKValueTick = mGSL->GetTimerFix()->Elapsed();
			NotifyPKStateChange(this);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsPlayer())
					{
						DomainPlay* play = (DomainPlay*)(*pos);
						play->NotifyPKStateChange(this);
					}
				}
			}
		}
		return mPKValue;
	}
	int  DomainPlay::SetPKValue(int value)
	{
		//暂时注掉PK值
		mPKValue = value;
		if( mPKValue < 0 )
		{
			mPKValue = 0;
		}
		mPKValueTick = mGSL->GetTimerFix()->Elapsed();
		NotifyPKStateChange(this);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* play = (DomainPlay*)(*pos);
					play->NotifyPKStateChange(this);
				}
			}
		}
		return mPKValue;
	}
	int DomainPlay::CheckLevelUp()
	{
		int i = 0;
		int old_lv = level();
		int max_lv = mSession->GetGameServerLib()->GetLevelInfoManager()->GetMaxLevel();
		if (old_lv >= max_lv)
			return 0;

		while( exp() >= exp_next() && i < mGSL->mLevelUpMax )
		{
			int lv = level();
			LEUD_INT64 nexp = exp_next();
			if( lv+1 == SetLevel(lv+1,true) )
			{
				exp(exp()-nexp);
			}
			i++;
		}
		if( old_lv != level() )
		{
			mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this,"player.onLevelUp");
			mSession->GetListener()->notifyLevelChange(mSession);
			CalculateAttribute();
			UpdateAfterCalculateAttribute();
			mSession->GetListener()->notifyAttributeChange(mSession);
			SkillDef* lvsd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_LevelUp);
			if( lvsd )
			{
				NotifyPlayEffect(this,lvsd->EffectType,0,0,0,ID(),lvsd->Effect_ResID);
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();
					++ pos )
				{
					if ((*pos))
					{
						if ((*pos)->TypeIsPlayer())
						{
							DomainPlay* play = static_cast<DomainPlay*>(*pos);
							play->NotifyPlayerAddinfo(this);
							play->NotifyPlayEffect(this, lvsd->EffectType, 0, 0, 0, ID(), lvsd->Effect_ResID);
						}
					}
				}
			}
			mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
		}
		return 0;
	}
	void DomainPlay::publicPlayEffect(int effect_type,int x,int y,unsigned int targetID,unsigned int ownerID,int resID)
	{
		NotifyPlayEffect(this,effect_type,x,y,targetID,ownerID,resID);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if( (*pos)->TypeIsPlayer() )
			{
				DomainPlay* play = static_cast<DomainPlay*>(*pos);
				play->NotifyPlayEffect(this,effect_type,x,y,targetID,ownerID,resID);
			}
		}
	}
	void DomainPlay::TalkProhibited(int value)
	{
		if( value > 0 )
		{
			mTalkProhibited = int(time(0)) + value;
		}
		else
		{
			mTalkProhibited = 0;
		}
		if( value>0 )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_TALK_PROHIBITED);
		}
		else
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_TALK_ALLOWED);
		}
	}
	int  DomainPlay::TalkProhibited()
	{
		if( mTalkProhibited > 0 )
		{
			if( time(0) < mTalkProhibited )
			{
				return 1;
			}
		}
		return 0;
	}
	int DomainPlay::SetModel(int id,int vl)
	{
		mModelMap[id] = vl;
		mSession->GetListener()->notifySetModel(mSession,ID(),id,vl);
		if (id = 10)
		{
			NotifyAvatarChange(this);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyAvatarChange(this);
				}
			}
		}
		return 0;
	}
	int DomainPlay::GetModel(int id)
	{
		if( mModelMap.find(id) != mModelMap.end() )
		{
			return mModelMap[id];
		}
		return 0;
	}

	int DomainPlay::SetUpdateLevel(int level)
	{
		SetLevel(level,true);
		mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this,"player.onLevelUp");
		mSession->GetListener()->notifyLevelChange(mSession);
		CalculateAttribute();
		UpdateAfterCalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
		SkillDef* lvsd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_LevelUp);
		if( lvsd )
		{
			NotifyPlayEffect(this,lvsd->EffectType,0,0,0,ID(),lvsd->Effect_ResID);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsPlayer())
					{
						DomainPlay* play = static_cast<DomainPlay*>(*pos);
						play->NotifyPlayerAddinfo(this);
						play->NotifyPlayEffect(this, lvsd->EffectType, 0, 0, 0, ID(), lvsd->Effect_ResID);
					}
				}
			}
		}
		mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);

		NotifyRelationInfoChange();
		return 0;
	}
	int DomainPlay::OnKillMonster(PlayMonster* pm)
	{
		if( GroupID() > 0 )
		{
			DomainPlay* arr[100];
			int i = 0;
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsPlayer())
					{
						DomainPlay* des = (DomainPlay*)(*pos);
						if (des && des->GroupID() > 0 && des->GroupID() == GroupID() && des->ID() != ID() && i < 100)
						{
							arr[i] = des;
							i++;
						}
					}
				}
			}
			for( int j = 0;j < i; j ++ )
			{
				DomainPlay* des = arr[j];
				mGSL->GetScriptEngine()->MonsterPlayer(pm,des,"player.onKillGroup");
			}
		}
		mGSL->GetScriptEngine()->MonsterPlayer(pm,this,"player.onKill");

		if( mAchieveFirstKillMonster <= 0 )
		{
			mAchieveFirstKillMonster = GetAchieveState(AchieveTypeFirstKillMonster);
			if( mAchieveFirstKillMonster <= 0 )
			{
				SetAchieveParam(AchieveTypeFirstKillMonster,1);
				AddOneAchieveState(AchieveTypeFirstKillMonster);
				mAchieveFirstKillMonster = 1;
			}
		}
		return 0;
	}
	LEUD_INT64 DomainPlay::WinExpByMonster(int exp)
	{
		if( exp <= 0 ) return 0;
		if( GroupID() > 0 )
		{
			int expt=exp;
			int lv_sum=level();
			DomainPlay* buf[30];
			buf[0] = this;
			int slot=1;
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsPlayer())
					{
						DomainPlay* des = (DomainPlay*)(*pos);
						if (slot < 30 && des && des->GroupID() > 0 && des->GroupID() == GroupID() && abs(level() - des->level()) < 15)
						{
							lv_sum += des->level();
							buf[slot++] = des;
						}
					}
				}
			}
			if( slot > 1 )
			{
				expt = exp * ( 9 + (slot<=11?slot:11) ) / 10;
			}
			for(int i = 0;i<slot;i++)
			{
				if( lv_sum > 0 )
				{
					buf[i]->AddExperience(expt * buf[i]->level() / lv_sum + 1,100);
				}
				else
				{
					buf[i]->AddExperience(expt / slot + 1,100);
				}
			}
		}
		else
		{
			return AddExperience(exp,100);
		}
		return 0;
	}
	int DomainPlay::checkItemWithExp(int exp)
	{
		int add_exp=0;
		bool doubleExp=false;
		bool threeExp=false;
		bool fourExp=false;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && ItemDef::IsDissipative(pos->mSubType) )
			{
				ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
				if( id )
				{
					if( !doubleExp && id->SubType == 20 )
					{
						doubleExp=true;
						int ad = int(exp*0.5f);
						if( pos->mDuration > ad )
						{
							pos->mDuration -= ad;
							add_exp += ad;
						}
						else
						{
							add_exp += pos->mDuration;
							pos->mDuration = 0;
							pos->mTypeID = 0;
							mNeedClearItemList = true;
						}
						mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					}
					if( !threeExp && id->SubType == 21 )
					{
						threeExp=true;
						if( pos->mDuration > exp )
						{
							pos->mDuration -= exp;
							add_exp += exp;
						}
						else
						{
							add_exp += pos->mDuration;
							pos->mDuration = 0;
							pos->mTypeID = 0;
							mNeedClearItemList = true;
						}
						mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					}
					if( !fourExp && id->SubType == 22 )
					{
						fourExp=true;
						int ad = int(exp*1.5f);
						if( pos->mDuration > ad )
						{
							pos->mDuration -= ad;
							add_exp += ad;
						}
						else
						{
							add_exp += pos->mDuration;
							pos->mDuration = 0;
							pos->mTypeID = 0;
							mNeedClearItemList = true;
						}
						mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					}
				}
			}
		}
		return add_exp;
	}
	LEUD_INT64 DomainPlay::RemExperience(int ex,int src)
	{
		if( ex < 0 ) return exp();
		exp(exp() - ex);
		if( exp()<0 )
		{
			exp(0);
		}
		mSession->GetListener()->notifyExpChange(mSession,-ex);
		return exp();
	}
	LEUD_INT64 DomainPlay::AddExperience(int ex,int src)
	{
		if( ex > 2000000000 || ex < 0 )
		{
			return exp();
		}
		int add_exp_by_item = 0;
		int add_exp_by_status = 0;
		int add_exp_by_sys_mul = 0;
		int add_exp_by_map_mul = 0;
		int add_exp_prop = 0;
		if( src == 100 )
		{
			add_exp_by_item = checkItemWithExp(ex);
			add_exp_by_sys_mul = ex*(mGSL->mExpMul-1);
			if( mParentMap && mParentMap->ExpMul() > 0 )
			{
				add_exp_by_map_mul = int(ex * mParentMap->ExpMul() / 100.0f - ex);
				if( add_exp_by_map_mul < 0 ){ add_exp_by_map_mul = 0;}
			}
			add_exp_prop = Attributes()->mExpGain * 1.0 / 10000 * ex;
		}
		int e = ex + add_exp_by_item + add_exp_by_status + add_exp_by_sys_mul + add_exp_by_map_mul + add_exp_prop;
		//if( china_limit_lv == 3 )
		//{
		//	char temp[512];_snprintf_s(temp,_countof(temp),511,mGSL->mStringConsts.NOTIFY_CHINA_LIMIT_EXP_NOTIFY,e,0);temp[511]=0;
		//	AlertMessage(1100,0,temp);
		//	e = 0;
		//}
		//else if( china_limit_lv == 2 )
		//{
		//	char temp[512];_snprintf_s(temp,_countof(temp),511,mGSL->mStringConsts.NOTIFY_CHINA_LIMIT_EXP_NOTIFY,e,e/2);temp[511]=0;
		//	AlertMessage(1100,0,temp);
		//	e /= 2;
		//}
		if( level() >= mGSL->mMaxLevelLimit ){e = 1;}
		exp(exp() + e);
		if( exp()<0 ){exp(0);}
		CheckLevelUp();
		mSession->GetListener()->notifyExpChange(mSession,e);
		
		// 给狗加exp
		if (mSlaveShenShou && mSlaveShenShou->mLiving)
		{
			mSlaveShenShou->AddExperience(e, src);
		}

		return exp();
	}

	int DomainPlay::SetLevel(int lv,bool log,const std::string& desp)
	{
		int old = level();
		level(lv);

		int param =  10000 + level();
		if (level() >= mGSL->mLevelChartLimit)
		{
			mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_ALL, Name(), param, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender());
			if( job()==JOB_WARRIOR )
			{
				mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_WIR, Name(), param, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender());
			}
			if( job()==JOB_WIZARD )
			{
				mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_WIZ, Name(), param, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender());
			}
			if( job()==JOB_TAOIST )
			{
				mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_TAO, Name(), param, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender());
			}
		}	
		ManagerLevelInfo* lim = mSession->GetGameServerLib()->GetLevelInfoManager();
		mExpNext = lim->GetNextExp(level());
		return level();
	}
	void DomainPlay::NotifyMeet(PlayMap* map,PlayMapObject* src)
	{
		if( map != mParentMap )
		{
			printf("meet not self map %s %s\n", map->GetID().c_str(), mParentMap->GetID().c_str());
			return;
		}
		if( src->TypeIsMapItem() )
		{
			PushItemInfo(static_cast<PlayMapItem*>(src)->item_type());
		}
		if( src->TypeIsNPC())
		{
			mNpcList.push_back(static_cast<PlayNpc*>(src));
		}

		PlayActiveObject::NotifyMeet(map,src);

		mSession->GetListener()->notifyMapMeet(mSession,map,src,0);
	}
	void DomainPlay::NotifyMapLeave(PlayMap* map)
	{
		if( map != mParentMap )
		{
			printf("leave not self map %s\n",map->GetID().c_str());
		}
		if( mSlaveKuLou && mSlaveKuLou->mLiving )
		{
			mParentMap->RemoveObject(mSlaveKuLou);
		}
		if( mSlaveShenShou && mSlaveShenShou->mLiving )
		{
			mParentMap->RemoveObject(mSlaveShenShou);
		}
		if (mSlaveHuwei && mSlaveHuwei->mLiving)
		{
			mParentMap->RemoveObject(mSlaveHuwei);
		}
		for(int i= 0;i<5;i++)
		{
			if( mSlaves[i] && mSlaves[i]->mLiving )
			{
				mParentMap->RemoveObject(mSlaves[i]);
			}
		}

		for (size_t i = 0; i < mSlaveList.size(); ++ i)
		{
			if( mSlaveList[i] && mSlaveList[i]->mLiving )
			{
				mParentMap->RemoveObject(mSlaveList[i]);
			}
		}

		mSession->GetListener()->notifyMapLevae(mSession,map);
		PlayActiveObject::NotifyMapLeave(map);
	}
	void DomainPlay::NotifyBye(PlayMap* map,PlayMapObject* src)
	{
		if( map != mParentMap )
		{
			printf("bye not self map %s %s\n", map->GetID().c_str(), mParentMap->GetID().c_str());
		}
		if( src->TypeIsNPC())
		{
			mNpcList.erase(std::remove(mNpcList.begin(),mNpcList.end(),static_cast<PlayNpc*>(src)),mNpcList.end());
		}
// 		if (src->TypeIsMonster())
// 		{
// 			if ((PlayActiveObject*)src == GetFirstTarget())
// 			{
// 				SetFirstTarget(0);////怪物出自己的视野之后清空第一攻击目标(针对道士宝宝)
// 			}
// 		}
		PlayActiveObject::NotifyBye(map,src);
		mSession->GetListener()->notifyMapBye(mSession,map,src);
	}
	void DomainPlay::NotifyMapChat(PlayMap* map,PlayMapObject* src,char* msg)
	{
		mSession->GetListener()->notifyMapChat(mSession,map,src,msg);
	}
	void DomainPlay::NotifyWorldChat(DomainPlay* src,char* msg)
	{
		mSession->GetListener()->notifyWorldChat(mSession,src,msg);
	}
	int DomainPlay::NormalChat(char* msg)
	{
		if( mSession && mSession->GetPlayer() && mSession->GetPlayer()->GetGMHideTalk() )
		{
			return 0;
		}
		mGSL->GetFilterManager()->Filter(msg);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* pl = (DomainPlay*)(*pos);
					pl->NotifyNormalChat(this, msg);
				}
			}
		}
		return 0;
	}
	int DomainPlay::MapChat(char* msg)
	{
		size_t msglen = strlen(msg);

		if( msglen>1 && msg[0] == '@' )
		{
			GMCommand* gmc = mSession->GetGameServerLib()->GetGMCommand();
			gmc->DoCommand(this,msg+1);
			return 0;
		}
		if( mParentMap )
		{
			if( mSession && mSession->GetPlayer() && mSession->GetPlayer()->GetGMHideTalk() )
			{
				return 0;
			}
			mGSL->GetFilterManager()->Filter(msg);
			mParentMap->Chat(this,msg,false);
			return 0;
		}
		return 0;
	}
	int DomainPlay::HornChat(char* msg)
	{
		if( 1==RemItemDuration(30,1,1) )
		{
			mSession->GetListener()->resHornChat(mSession,msg);
			mGSL->PublicHornChat(this,msg);
		}
		else
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_HORN_HAVE_NO);
		}
		return 0;
	}
	
	int DomainPlay::Walk(int dir,int nx,int ny,int tag)
	{
		PERF_NODE_FUNC();
		mMapEnterTick=0;
		lock_action_end();
		CollectBreak();
		if( IsState(MAP_OBJECT_STATE_DEAD) || IsMabi() )
		{
			mSession->GetListener()->resWalk(mSession,mDir,X(),Y(),tag);
			return 0;
		}
		if( ! check_action(0.25/mUpdateSpeed) )
		{
			mSession->GetListener()->resRun(mSession,mDir,X(),Y(),tag);
			return 0;
		}
		if( IsYingShen() )	{		ClearYingShen();		}
		IntPointClass pt(0,0);
		{
			PERF_NODE("setposition");
			pt = NextPosition(dir,1);
			SetPosition(pt.x,pt.y);
			mDir = dir;
		}
		{
			PERF_NODE("notify");
			mSession->GetListener()->resWalk(mSession,mDir,X(),Y(),tag);
			if( pt.x == X() || pt.y == Y() )
			{
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();)
				{
					if ((*pos))
					{
						(*pos)->NotifyWalk(this, mDir, X(), Y());
						++pos;
					}
					else
					{
						printf("walk *pos=0\n");
						pos = mVisibleInteractiveObjectList.erase(pos);
						continue;
					}
				}
			}
		}
		if( mParentMap )
		{
			mParentMap->CheckMapConn(this,X(),Y());
		}
		return 0;
	}
	int DomainPlay::Run(int dir,int nx,int ny,int tag)
	{
		PERF_NODE_FUNC();
		mMapEnterTick=0;
		lock_action_end();
		CollectBreak();
		if( IsState(MAP_OBJECT_STATE_DEAD) || IsMabi() )
		{
			mSession->GetListener()->resRun(mSession,mDir,X(),Y(),tag);
			return 0;
		}
		if( ! check_action(0.48/mUpdateSpeed) )
		{
			mSession->GetListener()->resRun(mSession,mDir,X(),Y(),tag);
			return 0;
		}
		if( IsYingShen() ){			ClearYingShen();		}
		IntPointClass pt(0,0);
		{
			PERF_NODE("setposition");
			pt = NextPosition(dir,2);
			SetPosition(pt.x,pt.y);
			mDir = dir;
		}
		{
			PERF_NODE("notify");
			mSession->GetListener()->resRun(mSession,mDir,X(),Y(),tag);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();)
			{
				if((*pos))
				{
					(*pos)->NotifyRun(this,mDir,X(),Y());
					++pos;
				}
				else
				{
					printf("run *pos=0\n");
					pos = mVisibleInteractiveObjectList.erase(pos);
					continue;
				}
			}
		}
		if( mParentMap )
		{
			mParentMap->CheckMapConn(this,X(),Y());
		}
		return 0;
	}
	int DomainPlay::Turn(int dir)
	{
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		mDir = dir;
		mSession->GetListener()->notifyTurn(mSession,this,dir);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyTurn(this, dir);
			}
		}
		return 0;
	}
	int DomainPlay::NPCTalk(unsigned int npcid,const char* param)
	{
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		PlayNpc* npc = FindNPCInview(npcid);
		if( npc )
		{
			npc->Talk(this,param);
		}
		return 0;
	}
	int DomainPlay::NPCShop(unsigned int npcid,int page)
	{
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		PlayNpc* npc = FindNPCInview(npcid);
		if( npc )
		{
			npc->Shop(this,page);
		}
		return 0;
	}
	int DomainPlay::VcoinShopList(int shop_id,int flags)
	{
		if( mGSL->GetShopNpc() )
		{
			mSession->GetListener()->resVcoinShopList(mSession,mGSL->GetShopNpc()->ID());
			mGSL->GetShopNpc()->Shop(this,shop_id);
		}
		return 0;
	}
	int DomainPlay::NPCBuy(unsigned int npcid,int page,int pos,int good_id,int type_id,int number)
	{
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		if( number <= 0 || number > 1000 )return 0;
		if( pos >= 1000 )
		{
			pos -= 1000;
			page = 9;
		}
		if( find_empty_item_position() < 0 )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BUY_ITEM_BAG_FULL);
			return 0;
		}
		/*if(npcid==mGSL->GetShopNpc()->ID()&&page==2)
		{
			return 0;
		}*/
		PlayNpc* npc = FindNPCInview(npcid);
		if( npc )
		{
			npc->Buy(this,page,pos,good_id,type_id,number);
		}
		else
		{
			if( mGSL->GetShopNpc()->ID() == npcid )
			{
				mGSL->GetShopNpc()->Buy(this,page,pos,good_id,type_id,number);
			}
		}
		return 0;
	}
	int DomainPlay::NPCRepairItem(unsigned int npc_id,int pos,int type_id,int flags)
	{
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
		if (!id) return 0;
		if( ! ItemDef::IsEquipment(id->SubType) ) return 0;
		if( ItemDef::IsMoxueshi(type_id))return 0;
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		PlayNpc* npc = FindNPCInview(npc_id);
		if( npc )
		{
			for( ItemMap::iterator p = GetItemList().begin();
				p != GetItemList().end();
				++ p )
			{
				if( p->mTypeID > 0 && p->mTypeID == type_id && pos == p->mPosition )
				{
					int ret = npc->Repair( this,&(*p) );
					if( ret == 0 )
					{
						mSession->GetListener()->notifyItemChange(mSession,*p,0);
					}
					if( ret == 2 )
					{
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_REPAIR_ITEM_NONEEDTOREPAIR);
					}
					if( ret == 3 )
					{
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_REPAIR_ITEM_NOT_ENOPHY_GAMEMONEY);
					}
					break;
				}
			}
		}
		return 0;
	}
	int DomainPlay::ReqairAllItem(int flags)
	{
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		int re_num = 0;
		for( ItemMap::iterator p = GetItemList().begin();
			p != GetItemList().end();
			++ p )
		{
			if( p->mPosition >= ITEM_BAG_END ) continue;
			if( p->mTypeID > 0 )
			{
				if( ItemDef::IsMoxueshi(p->mTypeID))continue;
				ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(p->mTypeID);
				if( id )
				{
					if( ! ItemDef::IsEquipment(id->SubType) ) continue;
					int rp = p->RepairPrice(id);
					if( rp > 0 )
					{
						re_num += 100;
						if( RemGameMoney( rp,113 ) )
						{
							p->mDuration = p->mDuraMax;
							re_num ++;
							mSession->GetListener()->notifyItemChange(mSession,*p,0);
						}
					}
				}
			}
		}
		return re_num;
	}
	int DomainPlay::NPCSell(unsigned int npc_id,int pos,int type_id,int number,int flag)
	{
		if (number <= 0)
		{
			return 0;
		}
		if(flag >= 200)
		{
			mCountClientSell ++;
			//if( mCountClientSell - flag >= -2 && mCountClientSell - flag <= 2 )
			{
				ItemSell(pos,type_id,number);
			}
			return 0;
		}
		if( mGSL->GetDenySellManager()->IsDeny(type_id) )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_NPC_SELL_DENY);
			return 0;
		}
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		PlayNpc* npc = FindNPCInview(npc_id);
		if( npc )
		{
			for( ItemMap::iterator p = GetItemList().begin();
				p != GetItemList().end();
				++ p )
			{
				if( p->mTypeID > 0 && p->mTypeID == type_id && pos == p->mPosition )
				{
					if(p->mNumber < number || number <= 0 )return 0;
					npc->Sell(this,&(*p),number);
					break;
				}
			}
		}
		return 0;
	}
	int DomainPlay::ItemSell(int pos,int type_id,int number)
	{
		if(number>1000){return 0;}
		if( mGSL->GetDenySellManager()->IsDeny(type_id) )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_NPC_SELL_DENY);
			return 0;
		}
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		SubItem* si = get_item_by_pos(pos);if(!si){return 0;}
		if(type_id > 0){si = get_item_by_pos_type(pos,type_id);if(!si)return 0;}
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
		if( id && si )
		{
			/*int gm = id->mPrice*number/3;
			if( ItemDef::IsEquipment(id->SubType) )
			{
				gm = int(id->mPrice/3.0 * si->mDuration / si->mDuraMax);
			}*/
			int gm = id->mPrice*number;
			if( gm <= 0 ) return 0;
			int flags = si->mItemFlags;
			if( 0 == RemItemPosition(si->mTypeID,si->mPosition,number,109) )
			{
				/*if( flags & ITEM_FLAG_BIND )
				{
					AddGameMoneyBind(gm,112);
				}
				else
				{*/
					AddGameMoneyBind(gm,112);	
				//}
			}
		}
		return 0;
	}
	int DomainPlay::PickUp(unsigned int item_obj_id,int x,int y)
	{
		if( mTradeItem.size() > size_t(get_empty_item_position_number()) )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_CANCEL_BY_TRADING);
			return 0;
		}
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		PlayMapItem* item;
		if (x != 0 && y != 0)
		{
			item = mParentMap->FindMapItemNear(x,y,item_obj_id);
		}
		else
		{
			item = mParentMap->FindMapItemNear(X(),Y(),item_obj_id);
		}
		if( item )
		{
			item->PickUped(this);
		}
		return 0;
	}

	//void DomainPlay::subfreefly(int t)
	//{
	//	mFreeDirectFly -= t;
	//	if(mFreeDirectFly < 0){mFreeDirectFly = 0;}
	//}
	//void DomainPlay::addfreefly(int t)
	//{
	//	mFreeDirectFly += t;
	//}
	void DomainPlay::ChangeHpMp(int hp, int mp, int flag)
	{
		if (flag == 1)///按照百分比扣血和魔
		{
			hp = (hp >= 100)?100:hp;
			mp = (mp >= 100)?100:mp;
			mHp = static_cast<int>(mActiveAttribute.mMaxHp*hp / 100);
			mMp = static_cast<int>(mActiveAttribute.mMaxMp*mp / 100);
		}
		else if (flag == 0)
		{
			mHp = (hp >= mHp) ? mHp : hp;
			mMp = (mp >= mMp) ? mMp : mp;
		}
		
		mSession->GetListener()->notifyHPMPChange(mSession, this, 0, 0);
	}

	int DomainPlay::GoMapConn(const std::string& map_id,int x,int y,int size)
	{
		bool trandart = false;
		if( mDart2 && mDart2->IsOwnerCloseMe() && mDart2->StopPosReached())
		{
			trandart = true;
		}
		
		int ret = EnterMap(map_id, x, y, size);
		
		if( trandart == true && ret == 1 )
		{
			if( mDart2 && mDart2->mLiving )
			{
				if( mDart2->mParentMap )
				{
					mDart2->mParentMap->RemoveObject(mDart2);
				}
				mDart2->SetPosition(x,y);
				if( mParentMap )
				{
					mParentMap->AddObject(mDart2);
				}
			}
		}
		return ret;
	}
	int DomainPlay::EnterMap(const std::string& map_id,int x,int y,int size)
	{
		if( mEnteringMap ) return 0;
		StopStall();
		mEnteringMap = 1;
		PlayMap* map = mSession->GetGameServerLib()->GetMapManager()->FindMap(map_id);
		if( map )
		{
			int ret = mGSL->GetScriptEngine()->PlayerString(this, "player.onMapEnterCheck", map_id.c_str());
			if (ret!=1)
			{
				mEnteringMap = 0;
				return 0;
			}

			int rx=x;int ry=y;
			if(size>1)
			{
				if(!map->GenValidateRandomPosition(x,y,size,&rx,&ry) )
				{
					mEnteringMap = 0;
					return 0;
				}
			}
			else
			{
				if( map->GetBlockData(x,y,2) != 0 )
				{
					mEnteringMap = 0;
					return 0;
				}
			}

			if( mParentMap == map )
			{
				// 
				std::string strBeforeMapId;
				if (mParentMap)
					strBeforeMapId = mParentMap->GetID();
				mGSL->GetScriptEngine()->PlayerString(this, "player.onMapEnterd", strBeforeMapId.c_str());

				ForceMove(rx,ry,Dir());
				mEnteringMap = 0;
				CollectBreak();
				ClearBelongMonster();
				return 1;
			}
			std::string strBeforeMapId;
			if( mParentMap )
			{
				strBeforeMapId = mParentMap->GetID();
				MapRemoveOwner();
				LeaveMap();
			}
			if( map->NoMount() == 1 )
			{
				//mGSL->GetScriptEngine()->OnlyPlayer(this,"gui.PanelMount.check_map_nomount");
			}
			/*if (map->NoGod() == 1)
			{
				kill_slave(0);
				AlertMessage(11, 0, mGSL->mStringConsts.NOTIFY_MAP_NOGOD);
			}*/
			CollectBreak();
			SetPosition(rx,ry);
			map->AddObject(this);
			mMapEnterTick = mGSL->GetTimerFix()->Elapsed();
			mEnteringMap = 0;
			mGSL->GetScriptEngine()->PlayerString(this, "player.onMapEnterd", strBeforeMapId.c_str());
			ClearBelongMonster();
			return 1;
		}
		mEnteringMap = 0;
		return 0;
	}
	int DomainPlay::EnterKuafuMap()
	{
		int x,y;
		std::string map_id;
		PlayMap* map = mGSL->GetMapManager()->FindMap(mGSL->mKf_Start_map);
		if( map )
		{
			map_id = mGSL->mKf_Start_map;
			std::map<std::string,int>& kfStartPosition = mGSL->GetKfStartPosition();
			std::map<std::string,int>::iterator pos = kfStartPosition.find(mSession->GetListener()->GetKuafuArea() + GuildName());
			int _position = 1;
			if(pos != kfStartPosition.end())
			{
				_position = pos->second;
			}
			else
			{
				for(int i = 1;i <= 5;i++)
				{
					bool _isExist = false;
					for (std::map<std::string,int>::iterator pos1 = kfStartPosition.begin();
						pos1 != kfStartPosition.end();pos1++)
					{
						if(i == pos1->second)
						{
							_isExist = true;
							break;
						}
					}
					if(!_isExist)
					{
						_position = i;
						break;
					}
				}
			}
			kfStartPosition[mSession->GetListener()->GetKuafuArea() + GuildName()] = _position;
			int _start_x,_start_y;
			switch(_position)
			{
			case 1:
				_start_x = mGSL->mKf_Start_x1;
				_start_y = mGSL->mKf_Start_y1;
				break;
			case 2:
				_start_x = mGSL->mKf_Start_x2;
				_start_y = mGSL->mKf_Start_y2;
				break;
			case 3:
				_start_x = mGSL->mKf_Start_x3;
				_start_y = mGSL->mKf_Start_y3;
				break;
			case 4:
				_start_x = mGSL->mKf_Start_x4;
				_start_y = mGSL->mKf_Start_y4;
				break;
			case 5:
				_start_x = mGSL->mKf_Start_x5;
				_start_y = mGSL->mKf_Start_y5;
				break;
			}
			if( map->GenValidateRandomPosition(_start_x,_start_y,mGSL->mKf_Start_size,&x,&y) )
			{
				EnterMap(map_id,x,y);
				return 1;
			}
		}
		return 0;
	}
	int DomainPlay::setKuafujlstate(int state)
	{
		Session()->GetCharacter()->mKuafuResult = state;
		return state;
	}
	int DomainPlay::LeaveMap()
	{
		if( mParentMap )
		{
			mParentMap->RemoveObject(this);
		}
		return 0;
	}
	int DomainPlay::MapRemoveOwner()
	{
		if (mParentMap && mParentMap->FubenType() == OGSLib::MAP_TYPE::MAP_PERSONAL)//mParentMap->AutoEnterLeave())
		{
			mParentMap->ClearOwner();
		}
		return 0;
	}
	int DomainPlay::HandleMapOwner(bool result)
	{
		if (mParentMap && mParentMap->FubenType()== OGSLib::MAP_TYPE::MAP_PERSONAL && mParentMap->AutoEnter() && !mParentMap->GetOwnerName().empty())////若此地图属于副本类地图
		{
			mParentMap->SetWaitTag(result);
		}
		return 0;
	}
	int DomainPlay::ChangeCloth(int lock,int cloth,int weapon)
	{
		mLockShow = 0;
		UpdateAvatar();
		mLockShow=lock;
		if(cloth>0)
		{
			mCloth = cloth;
		}
		if(weapon>0)
		{
			mWeapon = weapon;
		}
		NotifyAvatarChange(this);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyAvatarChange(this);
			}
		}
		return 0;
	}
	int DomainPlay::ChangeMount()
	{
		if( mMountLockUseSkill )
		{
			return 0;
		}
		if( mParentMap->NoMount() == 1 )
		{
			return 0;
		}
		if( mMount )
		{
			if(mShowMount == 0)
			{
				mChangeMountTick = mGSL->GetTimerFix()->Elapsed()+max(0.6,mGSL->GetItemDefineManager()->GetItemMountDelay(mMountTypeID)/1000.0f);
			}
			else
			{
				set_showmount(0);
			}
		}
		return 0;
	}
	int DomainPlay::ChangeFaBaoRes(int res)
	{
		if( mFabaoResID != res )
		{
			mFabaoResID = res;
			NotifyAvatarChange(this);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyAvatarChange(this);
				}
			}
		}
		return 0;
	}
	
	int DomainPlay::ConsignItem(int pos, int count, int price, int time_out)
	{
		int nRes = -1;
		for( ItemMap::iterator it = GetItemList().begin();
			it != GetItemList().end();
			++ it )
		{
			if (it->mTypeID > 0 
				&& it->mPosition == pos 
				&& pos >= ITEM_BAG_BEGIN 
				&& pos <ITEM_BAG_END + mBagSlotAdd
				)
			{
				if (it->mItemFlags & ITEM_FLAG_BIND)
				{
					// 绑定物品不可寄售
					nRes = 2;
					break;
				}
				else if (it->mNumber < count)
				{
					nRes = -1;
					break;
				}
				else
				{
					nRes = mGSL->GetConsignmentManager()->ConsignItem(this, &(*it), count, price, time_out);
					break;
				}
			}
		}
		// -1:无此物品 0:成功 1:手续费不够 2:绑定物品不可寄售
		mSession->GetListener()->notifyConsignItem(mSession,nRes);
		return 0;
	}

	int DomainPlay::GetConsignableItems(int type, int begin_index, int job, int condition, std::string filter)
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		if (now - mGetConsignableItemsTick < 0.2)
		{
			return -1;
		}
		mGetConsignableItemsTick = now;

		std::vector<ConsignableItemInfo *> vec;
		std::vector<int> list_index;
		mGSL->GetConsignmentManager()->GetConsignableItems(this, type, begin_index, job, condition, filter, vec, list_index);
		mSession->GetListener()->notifyConsignableItems(mSession, type, job, condition, vec, list_index);
		return 0;
	}

	int DomainPlay::BuyConsignableItem(int seed_id)
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		if (now - mBuyConsignableItemsTick < 0.5)
		{
			return -1;
		}
		mBuyConsignableItemsTick = now;

		int nRes = mGSL->GetConsignmentManager()->BuyConsignableItem(this, seed_id);
		// -1 : 无此物品 0:成功 1:背包放不下 2:钱不够
		mSession->GetListener()->notifyBuyConsignableItem(mSession,nRes, seed_id);
		return 0;
	}

	int DomainPlay::TakeBackConsignableItem(int seed_id)
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		if (now - mBuyConsignableItemsTick < 0.5)
		{
			return -1;
		}
		mBuyConsignableItemsTick = now;

		int nRes = mGSL->GetConsignmentManager()->TakeBackConsignableItem(this, seed_id);
		mSession->GetListener()->notifyTakeBackConsignableItem(mSession, nRes, seed_id);
		return 0;
	}
	
	int DomainPlay::TakeBackVCoin()
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		if (now - mBuyConsignableItemsTick < 0.5)
		{
			return -1;
		}
		mBuyConsignableItemsTick = now;

		int nRes = mGSL->GetConsignmentManager()->TakeBackVCoin(this);
		mSession->GetListener()->notifyTakeBackVCoin(mSession,nRes);
		return 0;
	}

	int DomainPlay::ItemProtect(int posEquip,int type_id)
	{
		for( ItemMap::iterator p = GetItemList().begin();
			p != GetItemList().end();
			++ p )
		{
			if( p->mTypeID > 0 && p->mTypeID == type_id && posEquip == p->mPosition )
			{
				if( p->mProtect > 50000 ) break;
				if( RemVcoin(mGSL->mItemProtectPrice,187) )
				{
					if( (rand() % mGSL->mItemProtectProbMax) < mGSL->mItemProtectProb )
					{
						int protect_add = rand() % mGSL->mItemProtectAdd + 1;
						p->mProtect += protect_add;
						mSession->GetListener()->notifyItemChange(mSession,*p,0);
						mSession->GetListener()->resProtectItem(mSession,0);
						return 0;
					}
					else
					{
						mSession->GetListener()->resProtectItem(mSession,100);
						return 0;
					}
				}
				else
				{
					AlertMessage(110,0,mGSL->mStringConsts.NOTIFY_PROTECT_ITEM_NOT_ENOUPH_VCOIN);
				}
				break;
			}
		}
		mSession->GetListener()->resProtectItem(mSession,101);
		return 0;
	}
	
	int DomainPlay::GMTalk(const char* param)
	{
		mScriptEcho = "";
		mGSL->GetScriptEngine()->OnlyPlayer(this,param);
		if( ! mScriptEcho.empty() )
		{
			mPlayerTalkSeed = rand();
			mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
		}
		mScriptEcho = "";
		return 0;
	}
	int DomainPlay::ItemTalk(int item_id,int seed,const char* param)
	{
		if( seed == mItemTalkSeed )
		{
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(item_id);
			if( id )
			{
				char ss[512];_snprintf_s(ss,_countof(ss),511,"item.item%d.onTalk%s",id->mTypeID,param);ss[511]=0;
				mScriptEcho = "";
				mGSL->GetScriptEngine()->ItemPlayer(this,id->mName.c_str(),id->mTypeID,ss);
				if( ! mScriptEcho.empty() )
				{
					mItemTalkSeed = rand();
					mSession->GetListener()->notifyItemTalk(mSession,this,id->mTypeID,id->mName.c_str(),mScriptEcho,mItemTalkSeed);
				}
				mScriptEcho = "";
			}
		}
		return 0;
	}
	int DomainPlay::PlayerTalk(int seed,const char* param)
	{
		if( seed == mPlayerTalkSeed )
		{
			char ss[512];_snprintf_s(ss,_countof(ss),511,"player.onTalk%s",param);ss[511]=0;
			ExeCode(ss);
		}
		return 0;
	}
	int DomainPlay::PushLuaTable(const char* function,const char* table)
	{
		mGSL->GetScriptEngine()->PlayerLuaFunction(this,"ckpanel.onLuaFunction",function,table);
		return 0;
	}

	void DomainPlay::SetActionNode(std::string panel,std::string id,std::string attr,std::string value)
	{
		ActionSet* ast = &(mFKActionSet[panel]);
		if( ast )
		{
			bool have = false;
			for (OGSLib::DomainPlay::ActionSet::iterator pos = ast->begin();
			pos != ast->end();
			++ pos)
			{ 
				if(pos->id != id || pos->attr != attr){continue;}
				if(pos->id == id && pos->attr == attr)
				{
					pos->value = value;
					have = true;
				}
			}
			if(!have)
			{
				actionset st;
				st.id = id;st.attr = attr;st.value = value;
				ast->push_back(st);
			}
		}
		else
		{
			actionset st;
			st.id = id;st.attr = attr;st.value = value;
			ActionSet aset;aset.push_back(st);
			mFKActionSet[panel] = aset;
		}
	}
	int DomainPlay::CountDownFinish()
	{
		if( mCountDownTag )
		{
			if( mCountDownTick > 0 && mCountDownTick <= time(0)+1 )
			{
				mCountDownTag = 0;
				mCountDownTick=0;
				ExeCode("player.onCountDownFinit");
			}
		}
		return 0;
	}
	int DomainPlay::SortItem(int flags)
	{
		int begin = 0;
		int end = ITEM_BAG_END+mBagSlotAdd;
		if( flags == 1 ){begin = ITEM_DEPOT_BEGIN;end = (ITEM_DEPOT_END + mDepotSlotAdd);}
		if( flags == 3 ){begin = ITEM_LOTTERYDEPOT_BEGIN;end = ITEM_LOTTERYDEPOT_END;}
		if( flags == 4 ){begin = ITEM_XUANJING_BEGIN;end = ITEM_XUANJING_END;}
		for(int i = begin;i<end;i++)
		{
			SubItem* si = this->get_item_by_pos(i);if( ! si){continue;}
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(si->mTypeID);if( ! id ) {continue;}
			int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(si->mTypeID);
			if(stackmax <= 1 || si->mNumber == stackmax) continue;

			for (int j = i+1;j<end;j++)
			{
				SubItem* sj = this->get_item_by_pos(j);if( ! sj){continue;}
				if(sj->mTypeID>0 && sj->mTypeID == si->mTypeID && sj->mItemFlags == si->mItemFlags)
				{
					int sub = max(0,min(sj->mNumber,stackmax - si->mNumber));
					si->mNumber += sub;
					// 背包整理
					mSession->GetListener()->notifyItemChange(mSession,*si,0);
					sj->mNumber -= sub;

					if(sj->mNumber <= 0)
					{
						sj->mTypeID = 0;
						mNeedClearItemList = true;
					}
					mSession->GetListener()->notifyItemChange(mSession,*sj,0);
					if(si->mNumber == mGSL->GetItemDefineManager()->GetItemStackMax(si->mTypeID))
					{
						break;
					}
				}
			}
		}
		for(int i = begin;i<end;i++)
		{
			int find_pos=-1;
			int find_type_id=99999999;
			SubItem* find_item=0;
			for( ItemMap::iterator p = GetItemList().begin();
				p != GetItemList().end();
				++ p )
			{
				if( p->mTypeID>0 && p->mPosition >= i && p->mPosition<end )
				{
					 //老版本排序优先级，mTypeID小的排在前面
					/*if( p->mTypeID < find_type_id )
					{
						find_pos=p->mPosition;
						find_type_id = p->mTypeID;
						find_item=&(*p);
					}*/
					//新版本排序，武器>装备>药品>其他
					ItemDef* findDef = mGSL->GetItemDefineManager()->GetItemDefine(find_type_id);
					if (findDef && find_item)
					{
						ItemDef* pDef = mGSL->GetItemDefineManager()->GetItemDefine(p->mTypeID);
						if (pDef)
						{
							if (findDef->SubType > pDef->SubType)
							{
								find_pos=p->mPosition;
								find_type_id = p->mTypeID;
								find_item=&(*p);
							}
							else if (findDef->SubType == pDef->SubType)
							{
								if (find_item->mZLevel < p->mZLevel)
								{
									find_pos=p->mPosition;
									find_type_id = p->mTypeID;
									find_item=&(*p);
								}
								else if (find_item->mZLevel == p->mZLevel)
								{
									if (findDef->mNeedZsLevel < pDef->mNeedZsLevel)
									{
										find_pos=p->mPosition;
										find_type_id = p->mTypeID;
										find_item=&(*p);
									}
									else if (findDef->mNeedZsLevel == pDef->mNeedZsLevel)
									{
										if (findDef->mNeedParam < pDef->mNeedParam)
										{
											find_pos=p->mPosition;
											find_type_id = p->mTypeID;
											find_item=&(*p);
										}
										else if (findDef->mNeedParam == pDef->mNeedParam)
										{
											if (find_item->mLevel < p->mLevel)
											{
												find_pos=p->mPosition;
												find_type_id = p->mTypeID;
												find_item=&(*p);
											}
											else if (find_item->mLevel == p->mLevel)
											{
												if (find_item->mZLevel < p->mZLevel)
												{
													find_pos=p->mPosition;
													find_type_id = p->mTypeID;
													find_item=&(*p);
												}
												else if (find_item->mZLevel == p->mZLevel)
												{
													if (findDef->mJob > pDef->mJob)
													{
														find_pos=p->mPosition;
														find_type_id = p->mTypeID;
														find_item=&(*p);
													}
													else if (findDef->mJob == pDef->mJob)
													{
														if (findDef->mTypeID < pDef->mTypeID)
														{
															find_pos=p->mPosition;
															find_type_id = p->mTypeID;
															find_item=&(*p);
														}
													}
												}
											}
										}
									}
								}
							}
						}
						else
						{
							find_pos=p->mPosition;
							find_type_id = p->mTypeID;
							find_item=&(*p);
						}
					}
					else
					{
						find_pos=p->mPosition;
						find_type_id = p->mTypeID;
						find_item=&(*p);
					}
				}
			}
			if( find_pos >= 0 )
			{
				if( i != find_pos )
				{
					ItemPositionExchange(find_pos,i,0,100);
				}
			}
			else
			{
				break;
			}
		}
		mSession->GetListener()->notifyItemPanelFresh(mSession,flags);
		return 0;
	}
	int DomainPlay::DestoryItem(int pos,int type_id)
	{
		if( pos < ITEM_BAG_BEGIN || pos >= ITEM_BAG_END+mBagSlotAdd ) return 1; 
		for( ItemMap::iterator p = GetItemList().begin();
			p != GetItemList().end();
			++ p )
		{
			if( p->mTypeID > 0 && p->mTypeID == type_id && pos == p->mPosition )
			{
				//检测物品是否可摧毁
				ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(p->mTypeID);if(!id){return 0;}
				if (!id->mCanDestroy)
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_CAN_NOT_DESTORY);
					return 0;
				}
				p->mTypeID = 0;
				mNeedClearItemList = true;
				mSession->GetListener()->notifyItemChange(mSession,*p,0);
			}
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
			if (id)
			{
				mGSL->GetScriptEngine()->ItemPlayer(this, id->mName.c_str(), id->mTypeID, "player.onRemItem");
			}
		}

		return 0;
	}

	int DomainPlay::SplitItem(int pos,int type_id,int number)
	{
		if (number <= 0)
		{
			return 0;
		}
		int add_pos = find_empty_item_position();
		if(add_pos < 0){AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SPLIT_BAG_FULL_FAIL);return 1;}
		for( ItemMap::iterator p = GetItemList().begin();
			p != GetItemList().end();
			++ p )
		{
			if( p->mTypeID > 0 && p->mTypeID == type_id && pos == p->mPosition )
			{
				if(number <= 0 || number >= p->mNumber )return 2;
				SubItem ii=*p;
				ii.mPosition = add_pos;
				ii.mNumber = number;
				p->mNumber -= number;
				mSession->GetListener()->notifyItemChange(mSession,*p,0);
				GetItemList().push_back(ii);
				mSession->GetListener()->notifyItemChange(mSession,ii,0);		
				break;
			}
		}
		return 0;
	}
	int DomainPlay::DropItem(int pos,int type_id,int number)
	{
		if (number <= 0)
		{
			return 0;
		}
		for( ItemMap::iterator p = GetItemList().begin();
			p != GetItemList().end();
			++ p )
		{
			if( p->mTypeID > 0 && p->mTypeID == type_id && pos == p->mPosition )
			{
				if( mSession->AuthType()== 100 )
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_DROP_ITEM_CAN_NOT_AUTH_TYPE);
				}
				else if( mParentMap && (!mParentMap->CanDropItem()) )
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_DROP_ITEM_CAN_NOT_ON_MAP);
				}
				else if( p->mItemFlags & ITEM_FLAG_BIND )
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_DROP_ITEM_CAN_NOT_BIND);
				}
				else if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) && mGSL->GetItemDefineManager()->CheckItemTradeLimit(type_id) > 0 )
				{
				}
				else if(mSafeArea >= 0)  //安全区禁止丢弃物品
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SAFEAREA_NO_DROP);
				}
				else
				{
					
					SubItem ii=*p;
					ii.mPosition = 0;
					ii.mNumber = min(number,p->mNumber);
					DropTheItem(&ii);
					
					p->mNumber -= number;
					if(p->mNumber <= 0)
					{
						p->mTypeID = 0;
						mNeedClearItemList = true;
						//GetItemList().erase(p);
					}
					mSession->GetListener()->notifyItemChange(mSession,*p,0);
					if(pos<0)
					{
						UpdateAvatar();
						for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
							pos != mVisibleInteractiveObjectList.end();
							++ pos )
						{
							if ((*pos))
							{
								(*pos)->NotifyAvatarChange(this);
							}
						}
						NotifyAvatarChange(this);
						CalculateAttribute();
						mSession->GetListener()->notifyAttributeChange(mSession);
						mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
					}
					ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
					if (id)
					{
						char szId[255] = {0};
						sprintf_s(szId, "%I64d", ii.mIdentId);
						mGSL->GetScriptEngine()->ItemPlayer(this, id->mName.c_str(), id->mTypeID, "player.onDropItem", 1,-999, szId);
					}
				}
				break;
			}
		}
		return 0;
	}

	int DomainPlay::PrivateChat(DomainPlay* player,char* msg)
	{
		mGSL->GetFilterManager()->Filter(msg);
		player->NotifyPrivateChat(this,msg);
		return 0;
	}

	void DomainPlay::OnHurt(PlayActiveObject* src,int change,int delay,int change_power)
	{
		//受伤即脱离隐身状态
		if( IsYingShen() )
		{	
			ClearYingShen();
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();
					++ pos )
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsMonster())
					{
						PlayMonster* mon = (PlayMonster*)(*pos);
						mon->NotifyClearYingShen(this);
					}
				}
			}
		}
		int attacker=0;
		if( src )
		{
			attacker=src->ID();
			if (src->TypeIsPlayer())
				m_mapDeadAssist[src->ID()] = mGSL->GetTimerFix()->Elapsed();
			if (src->TypeIsSlave())
			{
				PlaySlave * slave = static_cast<PlaySlave *>(src);
				if (slave->Owner())
					m_mapDeadAssist[slave->Owner()->ID()] = mGSL->GetTimerFix()->Elapsed();
			}
		}
		if(delay>=0)
		{
			NotifyInjury(this,attacker,change,delay,mAttackEffectFlags,change_power);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyInjury(this, attacker, change, delay, mAttackEffectFlags, change_power);
				}
			}
		}
		else
		{
			NotifyHPMPChange(this,change,0);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyHPMPChange(this, change, 0);
				}
			}
		}
		if (change_power > 0)
		{
			NotifyTiliChange(this);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				(*pos)->NotifyTiliChange(this);
			}
		}
		mAttackEffectFlags = 0x0;
	}
	void DomainPlay::EnterTrap(PlayActiveObject* src, int id)
	{
		mGSL->GetScriptEngine()->PlayerInt(this, id, 0, 0, 0, "player.onEnterTrap");
	}

	void DomainPlay::LeaveTrap(PlayActiveObject* src, int id)
	{
		mGSL->GetScriptEngine()->PlayerInt(this, id, 0, 0, 0, "player.onLeaveTrap");
	}
	void DomainPlay::OnDead(PlayActiveObject* src,int delay)
	{
		PlayActiveObject::OnDead(src, delay);
		set_showmount(0);
		CollectBreak();
		mGSL->GetScriptEngine()->OnlyPlayer(this,"gui.PanelMount.check_mount_cd");
		lock_action_end();

		if(Attributes()->mFuyuan_cd>0)
		{
			if(mSession->GetGameServerLib()->GetScriptEngine()->PlayerInt(this,Attributes()->mFuyuan_cd,static_cast<int>(time(0)),Attributes()->mFuyuan_pres,0,"player.fuyuanCheck")==1)
			{
				int fuyuanHp=MaxHp()*Attributes()->mFuyuan_pres/100;
				AddHP(fuyuanHp);
				NotifyHPMPChange(this,fuyuanHp,0);
				return;
			}
		}
		if( mSlaveKuLou && mSlaveKuLou->mLiving )
		{
			mSlaveKuLou->DamageHP(this,999999,0);
			changeSlaveState(0);
		}
		if( mSlaveShenShou && mSlaveShenShou->mLiving )
		{
			mSlaveShenShou->DamageHP(this,999999,0);
			changeSlaveState(0);
		}
		if (mSlaveHuwei && mSlaveHuwei->mLiving)
		{
			mSlaveHuwei->DamageHP(this,999999,0);
			changeSlaveState(0);
		}
		for(int i=0;i<5;i++)
		{
			if( mSlaves[i] && mSlaves[i]->mLiving )
			{
				mSlaves[i]->DamageHP(this, 10000000, 0);
			}
		}

		for(int i=0;i<mSlaveList.size();++i)
		{
			if( mSlaveList[i] && mSlaveList[i]->mLiving )
			{
				mSlaveList[i]->DamageHP(this, 10000000, 0);
			}
		}

		SetState(MAP_OBJECT_STATE_DEAD);
		mIncHP=0;
		mIncMP=0;
		mIncMagicHP=0;
		NotifyDie(this,delay);
		
		if( mParentMap->AutoAlive() <= 0 /* && mParentMap->CanReliveStand() > 0*/)
		{
			if (src)
			{
				std::string name = src->Name();
				if (src->TypeIsSlave())
				{
					PlaySlave * slave = static_cast<PlaySlave *>(src);
					if (slave->Owner())
						name = slave->Owner()->Name();
				}

				if(GetMap() && GetMap()->FightAnonym())
					name = mGSL->mStringConsts.NOTIFY_ANONYM_NAME;

				mGSL->GetScriptEngine()->PlayerString(this,"gui.PanelRelive.openPanel",name.c_str());
			}
			else
			{
				mGSL->GetScriptEngine()->PlayerString(this,"gui.PanelRelive.openPanel",PosionerName().c_str());
			}
			SetPosionerName("");
		}
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyDie(this, delay);
			}
		}
		if (mParentMap && mParentMap->NoDieDropItem())
		{
		}
		else
		{
			DieDropItem();
		}
		ClearStatusOnDead();
		ClearBuffOnDead();

		if( PKValue() < 400 ) //PKState() == 0 &&
		{
			DomainPlay* pkCheckTarget = 0;
			if( src && src->TypeIsPlayer() )
			{
				pkCheckTarget = (DomainPlay*)src;
			}
			if( src && src->TypeIsSlave() )
			{
				PlaySlave* slave = static_cast<PlaySlave*>(src);
				if( slave )
				{
					pkCheckTarget = slave->Owner();
				}
			}
			if( pkCheckTarget )
			{
				//char msg[256];
				//_snprintf_s(msg,_countof(msg),255,mGSL->mStringConsts.NOTIFY_PK_KILLED,pkCheckTarget->Name());msg[255]=0;
				//AlertMessage(10,0,msg);
				//_snprintf_s(msg,_countof(msg),255,mGSL->mStringConsts.NOTIFY_PK_KILL,Name());msg[255]=0;
				//pkCheckTarget->AlertMessage(10,0,msg);
				if( mParentMap && mParentMap->PKAllow() )
				{
					if(mGSL->GetWarState() <= 0 )
					{
						//pkCheckTarget->AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_PK_MAP_NOT_MERDER);
					}
				}
				else
				{
 					if (!IsWarGuildMember(pkCheckTarget))
					{
						pkCheckTarget->AlertMessage(11,0,mGSL->mStringConsts.NOTIFY_MERDER);
						pkCheckTarget->AddPKValue(100);
					}

					if( mGSL->mPKKillSubLuckOpen && rand()%mGSL->mPKKillSubLuckProb == 0 )
					{
						pkCheckTarget->ChangeWeaponLuck(-1);
						pkCheckTarget->AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_PK_KILL_SUB_LUCK);
					}
				}
			}
		}

		if( mGSL->mPlayerTrigerPKEvent )
		{
			double valid_time = mGSL->GetTimerFix()->Elapsed() - mGSL->mDeadAssistValidTime;
			// 助攻触发
			for (std::map<int, double>::iterator it = m_mapDeadAssist.begin(); it!= m_mapDeadAssist.end(); ++ it)
			{
				// 助攻有效时间
				if (it->second > valid_time)
				{
					PlayActiveObject * assist = FindActiveInView(it->first);
					if (assist && assist->TypeIsPlayer())
					{
						mSession->GetGameServerLib()->GetScriptEngine()->PlayerPlayer(static_cast<DomainPlay*>(assist),this,"player.onKillAssist");
					}
				}
			}
		}
		if( mGSL->mPlayerTrigerPKEvent )
		{
			if( src && src->TypeIsMonster() )
			{
				PlayMonster* monster = (PlayMonster*)src;
				mSession->GetGameServerLib()->GetScriptEngine()->PlayerMonster(this,monster,"player.onDead");
			}
			else
			{
				ExeCode("player.onDead");
			}
		}
		if( mGSL->mPlayerTrigerPKEvent > 0)
		{
			DomainPlay* killer = 0;
			if( src && src->TypeIsPlayer() )
			{
				killer = (DomainPlay*)src;
			}
			if( src && src->TypeIsSlave() )
			{
				PlaySlave* slave = static_cast<PlaySlave*>(src);
				if( slave )
				{
					killer = slave->Owner();
				}
			}
			if( killer )
			{
				mSession->GetGameServerLib()->GetScriptEngine()->PlayerPlayer(this,killer,"player.onPKDead");
			}
		}
		
		if( src && src->TypeIsPlayer() )
		{
			DomainPlay* killer = static_cast<DomainPlay*>(src);
			if( killer )
			{
				killer->OnPKKill();
			}
		}
		m_mapDeadAssist.clear();

		ClearBelongMonster();
	}
	void DomainPlay::OnAttack(PlayActiveObject* des,SkillDef * def,int pawer,int flags,int delay)
	{
		if( mSlaveShenShou && mSlaveShenShou->mLiving )
		{
			mSlaveShenShou->SetHateObj(des, true, pawer);
		}

		if( mSlaveKuLou && mSlaveKuLou->mLiving )
		{
			//if( ! mSlaveKuLou->Target() )
			{
				mSlaveKuLou->FindNextTarget(des);
			}
		}

		if( mSlaveHuwei && mSlaveHuwei->mLiving && !des->TypeIsPlayer() && !des->TypeIsSlave())
		{
			mSlaveHuwei->SetHateObj(des, true, pawer);
		}

		for(int i=0;i<5;i++)
		{
			if( mSlaves[i] && mSlaves[i]->mLiving )
			{
				//if( ! mSlaveShenShou->Target() )
				{
					mSlaves[i]->FindNextTarget(des);
				}
			}
		}
		/*int dura_damage = 1;
		DamageLoadEquipment(dura_damage);*/
	}
	void DomainPlay::ForceMove(int x,int y,int dir)
	{
		mMapEnterTick=0;
		PlayMap* m = GetMap();
		if( m && m->GetBlockData(x,y,2)==0 )
		{
			PlayActiveObject::ForceMove(x,y,dir);
		}
		if( x == X() && y == Y() )
		{
			NotifyForceMove(this,x,y,dir);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyForceMove(this, x, y, dir);
				}
			}
		}
	}
	int  DomainPlay::Attacked(PlayActiveObject* src,SkillDef * sd ,int pawer,int flags,int delay)
	{
		if(mCollectTarget && sd->TypeID != SkillDef::SKILL_TYPE_ShiDuShu)
		{
			CollectBreak();
		}

		ChangeMountBreak();
		if( src->TypeIsPlayer() )
		{
			DomainPlay* playsrc = (DomainPlay*)src;
			if (mShowMount == 1)
			{
				ChangeMount();
			}
			if( PKValue() < 400 && PKState() == 0 && playsrc->PKState()!=1 )
			{
				if( mParentMap && mParentMap->PKAllow()<=0 )
				{
					playsrc->SetPKState(1);
				}
			}
			/*if (PKValue() <= 0 && PKState()!=1 && playsrc->PKState()!=1)
			{
				if( mParentMap && mParentMap->PKAllow()<=0 )
				{
					playsrc->SetPKState(1);
				}
			}*/
		}
		if( mSlaveKuLou && mSlaveKuLou->mLiving && ! src->TypeIsPlayer() )
		{
			//if( ! mSlaveKuLou->Target() )
			{
				mSlaveKuLou->FindNextTarget(src);
			}
		}
		if( mSlaveHuwei && mSlaveHuwei->mLiving && ! src->TypeIsPlayer() )
		{
			//if( ! mSlaveKuLou->Target() )
			{
				mSlaveHuwei->FindNextTarget(src);
			}
		}
		for(int i = 0;i< 5;i++)
		{
			if( mSlaves[i] && mSlaves[i]->mLiving && ! src->TypeIsPlayer() )
			{
				//if( ! mSlaves[i]->Target() )
				{
					mSlaves[i]->FindNextTarget(src);
				}
			}
		}
		if( src->TypeIsPlayer() && mMapEnterTick + mGSL->mMapEnterSafeTime > mGSL->GetTimerFix()->Elapsed() )
		{
			if( mParentMap && mParentMap->EnterMapProtected() )
			{
				pawer = 0;
			}
		}
		if( IsNoDamage()  && rand() %10000 < StatusParam(STATUS_TYPE_NO_DAMAGE) )
		{
			//if( src->TypeIsPlayer() )
			//{
			//	DomainPlay* play = static_cast<DomainPlay*>(src);
				//play->AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_STATUS_NO_DAMAGE);
			//	return 0;
			//}
			return 0;
		}

		int damage = PlayActiveObject::Attacked(src,sd,pawer,flags,delay);

		if( mSlaveShenShou && mSlaveShenShou->mLiving && mSlaveShenShou->IsProperAttackDes(src, NULL))
		{
			mSlaveShenShou->SetHateObj(src, false, damage);
		}

		if( mSlaveHuwei && mSlaveHuwei->mLiving && mSlaveHuwei->IsProperAttackDes(src, NULL))
		{
			mSlaveHuwei->SetHateObj(src, false, pawer);
		}

		return damage;
	}
	void DomainPlay::Attack(PlayActiveObject* des,SkillDef * sd,int pawer,int flags,int delay)
	{
		if( mMapEnterTick + mGSL->mMapEnterSafeTime > mGSL->GetTimerFix()->Elapsed() )
		{
			if( des->TypeIsPlayer() )
			{
				if( mParentMap && mParentMap->EnterMapProtected() )
				{
					pawer = 0;
				}
			}
		}
		if( job() == JOB_WIZARD )
		{
			pawer = static_cast<int>(pawer * (mGSL->mWizardAttackMul/100.0f));
		}
		if( job() == JOB_TAOIST )
		{
			pawer = static_cast<int>(pawer * (mGSL->mTaoistAttackMul / 100.0f));
		}
		if (des && !des->TypeIsPlayer())//////记录不是玩家的对象
		{
			SetFirstTarget(des);
		}	
		return PlayActiveObject::Attack(des,sd,pawer,flags,delay);
	}

	bool DomainPlay::CheckHit(PlayActiveObject* des, SkillDef * sd)
	{
		bool ret = PlayActiveObject::CheckHit(des,sd);
		if( ! ret )
		{
			mSession->GetListener()->notifyAttackMiss(mSession,des);
		}
		return ret;
	}
	bool DomainPlay::IsProperFriendDes(PlayActiveObject* des,int skill_type)
	{
		if( PlayActiveObject::IsProperFriendDes(des,skill_type) )
		{
			return true;
		}
		if( des->TypeIsSlave() )
		{
			PlaySlave* slave = (PlaySlave*)des;
			if( slave->mOwner == this )
			{
				return true;
			}
		}
		if( des->TypeIsPlayer() )
		{
			DomainPlay* play = (DomainPlay*)des;
			switch(mAttackMode)
			{
			case ATTACK_MODE_TYPE_ALL:
				return true;
				break;
			case ATTACK_MODE_TYPE_PEACE:
				return true;
				break;
			case ATTACK_MODE_TYPE_PK:
				{
					DomainPlay* desplayer = (DomainPlay*)des;
					if( desplayer->PKValue() >= 400 || desplayer->PKState() > 0 )
					{
						return true;
					}
				}
				break;
			case ATTACK_MODE_TYPE_GUILD:
				{
					if( (!GuildName().empty()) && GuildTitle() > GUILD_TITLE_TYPE_ENTERING )
					{
						DomainPlay* desplayer = (DomainPlay*)des;
						if( desplayer->GuildName() == GuildName() )
						{
							if( desplayer->GuildTitle() > GUILD_TITLE_TYPE_ENTERING )
							{
								return true;
							}
						}
						if( ! desplayer->GuildName().empty() )
						{
							Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(GuildName());
							if( guild )
							{
								if( guild->IsFriendGuild(desplayer->GuildName() ) )
								{
									if( desplayer->GuildTitle() > GUILD_TITLE_TYPE_ENTERING )
									{
										return true;
									}
								}
							}
						}
					}
				}
				break;
			case ATTACK_MODE_TYPE_GROUP:
				{
					DomainPlay* desplayer = (DomainPlay*)des;
					if( GroupID()>0 && GroupID() == desplayer->GroupID() )
					{
						return true;
					}
				}
				break;
			case ATTACK_MODE_TYPE_TEAM:
				{
					DomainPlay* desplayer = (DomainPlay*)des;
					if( TeamID()>0 && TeamID() == desplayer->TeamID() )
					{
						return true;
					}
				}
				break;
			}
		}
		return false;
	}
	bool DomainPlay::IsProperAttackDes(PlayActiveObject* des,SkillDef * sd)
	{
		if( ! PlayActiveObject::IsProperAttackDes(des,sd) )
		{
			return false;
		}

		if (des->TypeIsDart())
		{
			if(sd->TypeID== SkillDef::SKILL_TYPE_ShiDuShu)
			{
				AlertMessage(11,0,mGSL->mStringConsts.NOTIFY_SKILL_FORBID_ROB_DART);
				return false;
			}
// 			if(mGSL->mRobDartLimit && mRobDartNumberUsed >= mGSL->mRobDartNumber)
// 			{
// 				AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_ROB_DART_NUMBER_FULL);
// 				return false;
// 			}
			PlayDart2* dart = (PlayDart2*)des;
			if( dart->mOwner && static_cast<PlayActiveObject*>(dart->mOwner) == this )
			{
				return false;
			}

			if( mAttackMode==ATTACK_MODE_TYPE_PEACE )
				return false;
		
			return true;
		}
		if( des->TypeIsNeutral() || des->TypeIsMonster() )
		{
			PlayMonster* neu = (PlayMonster*)des;
			if( neu->TeamID() > 0 && neu->TeamID() == TeamID() )
			{
				return false;
			}
		}
		if( des->TypeIsPlayer() )
		{
			if(mParentMap&&mParentMap->PKProhibit())
			{
				if( mAttackMode==ATTACK_MODE_TYPE_ALL )
				{
					if (mParentMap&&mParentMap->FightAnonym())
						AlertMessage(10, 0, mGSL->mStringConsts.NOTIFY_MAPTIME_PK_PROHIBIT);
					else
						AlertMessage(10, 0, mGSL->mStringConsts.NOTIFY_MAP_PK_PROHIBIT);
				}
				return false;
			}
			if( mParentMap && mParentMap->NoProtectNewer()==0 )
			{
				if( level() < mGSL->mPKLimitLevel || des->level() < mGSL->mPKLimitLevel )
				{
					if( mAttackMode!=ATTACK_MODE_TYPE_PEACE )
					{
						char temp[256];
						_snprintf_s(temp,_countof(temp),255,mGSL->mStringConsts.NOTIFY_PK_LEVEL_LIMIT,mGSL->mPKLimitLevel);temp[255]=0;
						//std::string msg(temp);
						AlertMessage(1,0,temp);
						DomainPlay* play = (DomainPlay*)des;
						play->AlertMessage(10,0,temp);
					}
					return false;
				}
			}
		}
		PlayActiveObject* checkdes = des;
		if( des->TypeIsSlave() )
		{
			PlaySlave* slave = (PlaySlave*)des;

			if (slave->GetSlaveType() == Slave_type_huwei)
			{
				return false;
			}
			if( slave->mOwner == static_cast<PlayActiveObject*>(this) )
			{
				return false;
			}

			if(mParentMap&&mParentMap->PKProhibit())
			{
				if( mAttackMode==ATTACK_MODE_TYPE_ALL )
				{
					if (mParentMap&&mParentMap->FightAnonym())
						AlertMessage(10, 0, mGSL->mStringConsts.NOTIFY_MAPTIME_PK_PROHIBIT);
					else
						AlertMessage(10, 0, mGSL->mStringConsts.NOTIFY_MAP_PK_PROHIBIT);
				}
				return false;
			}

			if( slave->mOwner )
			{
				checkdes = slave->mOwner;
			}
		}

		if( checkdes->TypeIsPlayer() )
		{
			//if( mGSL->mPKConfirm )
			//{
			//	DomainPlay* desplayer = (DomainPlay*)checkdes;
			//	if( desplayer->mAttackMode == ATTACK_MODE_TYPE_PEACE && desplayer->mPKconfirm == 0 )
			//	{
			//		return false;
			//	}
			//}
			switch(mAttackMode)
			{
			case ATTACK_MODE_TYPE_ALL:
				break;
			case ATTACK_MODE_TYPE_PEACE:
				return false;
				break;
			case ATTACK_MODE_TYPE_GUILD:
				{
					DomainPlay* desplayer = (DomainPlay*)checkdes;
					if( GuildTitle() > 101 && desplayer->GuildTitle() > 101 && desplayer->GuildName() == GuildName() )
					{
						return false;
					}
				}
				break;
			case ATTACK_MODE_TYPE_GROUP:
				{
					DomainPlay* desplayer = (DomainPlay*)checkdes;
					if( GroupID() > 0 && GroupID() == desplayer->GroupID() )
					{
						return false;
					}
				}
				break;
			case ATTACK_MODE_TYPE_TEAM:
				{
					DomainPlay* desplayer = (DomainPlay*)checkdes;
					if( TeamID() > 0 && TeamID() == desplayer->TeamID() )
					{
						return false;
					}
				}
				break;
			}
		}
		return true;
	}

	void DomainPlay::NotifyInjury(PlayMapObject* src,int attacker,int change,int delay,int effect,int change_power)
	{
		mSession->GetListener()->notifyInjury(mSession,src,attacker,change,delay,effect,change_power);
	}
	void DomainPlay::NotifyDie(PlayMapObject* src,int delay)
	{
		mSession->GetListener()->notifyDie(mSession,src,delay);
	}
	void DomainPlay::NotifyRelive(PlayMapObject* src,int type)
	{
		mSession->GetListener()->notifyRelive(mSession,src,type);
	}
	void DomainPlay::NotifyHPMPChange(PlayActiveObject* src,int hp_change,int mp_change)
	{
		mSession->GetListener()->notifyHPMPChange(mSession,src,hp_change,mp_change);
	}
	void DomainPlay::NotifySatusChange(PlayActiveObject* src,int status_id)
	{
		mSession->GetListener()->notifyStatusChange(mSession,src,status_id);
	}

	void DomainPlay::NotifyBuffChange(PlayActiveObject* src,int id, int op_code, double time_left)
	{
		mSession->GetListener()->notifyBuffChange(mSession,src,id,op_code, time_left);
	}

	void DomainPlay::NotifyPrivateChat(DomainPlay* src,char* msg)
	{
		mSession->GetListener()->notifyPrivateChat(mSession,src,msg);
	}
	void DomainPlay::NotifyMonsterChat(PlayMonster* src,const char* msg)
	{
		mSession->GetListener()->notifyMonsterChat(mSession,src,msg);
	}
	void DomainPlay::NotifyNormalChat(DomainPlay* src,char* msg)
	{
		mSession->GetListener()->notifyNormalChat(mSession,src,msg);
	}
	//void DomainPlay::NotifyMiniNpc(DomainPlay* src,PlayMap* map)
	//{
	//	mSession->GetListener()->notifyMiniNpc(mSession,map);
	//}
	void DomainPlay::NotifyMeetItem(DomainPlay* src, PlayMapItem* item)
	{
		mSession->GetListener()->notifyMeetItem(mSession,item);
	}
	void DomainPlay::NotifyNPCTalk(PlayNpc* src,int flags,int param,const std::string& msg)
	{
		mSession->GetListener()->notifyNpcTalk(mSession,src->ID(),flags,param,src->Name(),msg);
	}
	void DomainPlay::NotifyNPCShop(PlayNpc* src,std::string msg,int page)
	{
		for( PlayNpc::ShopGoodMap::iterator pos = src->GetGoods()->begin();
			pos != src->GetGoods()->end();
			++ pos )
		{
			PushItemInfo(pos->second.type_id);
		}
		mSession->GetListener()->notifyNpcShop(mSession,src,page,msg);
	}
	void DomainPlay::notifyDoAction(PlayMapObject* src,int action)
	{
		mSession->GetListener()->notifyDoAction(mSession,src,action);
	}
	void DomainPlay::notifySpeed(PlayMapObject* src,float speed)
	{
		mSession->GetListener()->notifySpeed(mSession,src,speed);
	}
	void DomainPlay::NotifyWalk(PlayMapObject* src,int dir,int newx,int newy)
	{
		mSession->GetListener()->notifyWalk(mSession,src,dir,newx,newy);
	}
	void DomainPlay::NotifyRun(PlayMapObject* src,int dir,int newx,int newy)
	{
		mSession->GetListener()->notifyRun(mSession,src,dir,newx,newy);
	}
	void DomainPlay::ResRun(int dir,int newx,int newy)
	{
		mSession->GetListener()->resRun(mSession,dir,newx,newy,-1);
	}
	void DomainPlay::NotifyTurn(PlayMapObject* src,int dir)
	{
		mSession->GetListener()->notifyTurn(mSession,src,dir);
	}
	void DomainPlay::NotifyAttack(PlayMapObject* src,int param)
	{
		mSession->GetListener()->notifyAttack(mSession,src,param);
	}
	void DomainPlay::NotifyAvatarChange(PlayMapObject* src)
	{
		mSession->GetListener()->notifyAvatarChange(mSession,src);
	}
	void DomainPlay::NotifyUseSkill(PlayMapObject* src,int skill_type,int paramX,int paramY,unsigned int paramID,int resID, int dir, int skill_level)
	{
		mSession->GetListener()->notifyUseSkill(mSession,src,skill_type,paramX,paramY,paramID,resID,dir,skill_level);
	}
	void DomainPlay::NotifyForceMove(PlayMapObject* src,int x,int y,int dir)
	{
		mSession->GetListener()->notifyForceMove(mSession,src,x,y,dir);
	}

	void DomainPlay::NotifyShowProgressBar(int duration,const char* msg)
	{
		mSession->GetListener()->notifyShowProgressBar(mSession,duration,msg);
	}
	void DomainPlay::UpdateAllRelation()
	{
		// 缓存操作
		bool ret = false;
		do 
		{
			RelationShipOper oper;
			ret = mGSL->GetRelationCacheManager()->PopCache(mCharacter->mChrName, oper);
			if (ret)
			{
				if (oper.title == 0)
					DelRelationShip(oper.name_oper.c_str(), false);
				else
					ChangeRelationShip(oper.name_oper.c_str(), oper.title, false);
			}

		} while (ret);
		
		for (size_t i = 0; i < mFriendArray.reltionship.size(); ++ i)
		{
			SubFriend & sf = mFriendArray.reltionship[i];
			OGSLib::GameSession* se = mGSL->FindSessionWithCharID(sf.mName);
			if( se && se->GetPlayer())
			{
				sf.mOnlineState = 1;
				sf.mGender = se->GetPlayer()->gender();
				sf.mGuildID = se->GetPlayer()->GuildID();
				sf.mGuild = se->GetPlayer()->GuildName();
				sf.mLevel = se->GetPlayer()->level();
				sf.mJob = se->GetPlayer()->job();
			}
			else
				sf.mOnlineState = 0;
		}
	}

	void DomainPlay::NotifyRelationInfoChange(int off_line /*= 0*/)
	{
		for (size_t i = 0; i < mFriendArray.reltionship.size(); ++ i)
		{
			SubFriend & sf = mFriendArray.reltionship[i];
			if (SubFriend::IsFriend(sf.mTitle) || SubFriend::IsOtherSideBlack(sf.mTitle) || SubFriend::IsOtherSideEnemy(sf.mTitle))
			{
				OGSLib::GameSession* se = mGSL->FindSessionWithCharID(sf.mName);
				if( se && se->GetPlayer())
				{
					se->GetPlayer()->RelationInfoChange(this, off_line);
				}
			}
		}
	}

	void DomainPlay::RelationInfoChange(DomainPlay * src, int off_line)
	{
		std::string strName = src->mCharacter->mChrName;
		for (size_t i = 0; i < mFriendArray.reltionship.size(); ++ i)
		{
			SubFriend & sf = mFriendArray.reltionship[i];
			
			if (strName == sf.mName)
			{
				if (off_line)
				{
					sf.mOnlineState = 0;
				}
				else
				{
					// 刷新下信息
					sf.mOnlineState = 1;
					sf.mGender = src->gender();
					sf.mGuildID = src->GuildID();
					sf.mGuild = src->GuildName();
					sf.mLevel = src->level();
					sf.mJob = src->job();
				}
				mSession->GetListener()->notifyFriendChange(mSession, sf);
			}
		}
	}

	void DomainPlay::PushFriendData()
	{
		for( std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();
			pos != mFriendArray.reltionship.end();
			++ pos )
		{
			mSession->GetListener()->notifyFriendChange(mSession, *pos);
		}
	}
	bool DomainPlay::IsFriend(const char* name)
	{
		for( std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();
			pos != mFriendArray.reltionship.end();
			++ pos )
		{
			if (pos->mName == name)
			{
				if (SubFriend::IsFriend(pos->mTitle))
				{
					return true;
				}
				return false;
			}
		}
		return false;
	}
	bool DomainPlay::IsEnemy(const char* name)
	{
		for( std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();
			pos != mFriendArray.reltionship.end();
			++ pos )
		{
			if (pos->mName == name)
			{
				if (SubFriend::IsMyEnemy(pos->mTitle))
				{
					return true;
				}
				return false;
			}
		}
		return false;
	}
	int DomainPlay::GetRelationShip(const char* name)
	{
		for (std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();pos!=mFriendArray.reltionship.end();++pos)
		{
			if (pos->mName == name)
			{
				return pos->mTitle & 0x0000ffff;
			}
		}
		return 0;
	}

	void DomainPlay::PushDataToAllForRank(int param, int num)
	{
		mSession->GetGameServerLib()->GetChartManager()->UpdateData(param, Name(), num, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender());
	}

	bool DomainPlay::PlayerApplyFriendRequest(const char* name)
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		mFriendApplyMap[name] = now;

		// 发送好友申请消息
		mSession->GetListener()->notifyFriendApply(mSession,name);

		return true;
	}

	// 删除关系
	bool DomainPlay::DelRelationShip(const char* name, bool bAct)
	{
		bool found = false;
		int nTitleDelType = 0;
		for( std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();
			pos != mFriendArray.reltionship.end();
			++ pos )
		{
			if( pos->mName.compare(name) == 0)
			{
				found = true;
				if (SubFriend::IsFriend(pos->mTitle))
				{
					// 好友关系一方删,两方都没了
					--mFriendArray.nFriend;
					pos->mTitle = 0;

					SubFriend sf = *pos;
					sf.mTitle = 0;
					mSession->GetListener()->notifyFriendChange(mSession, *pos);
				}
				if (bAct)
				{
					if (SubFriend::IsMyEnemy(pos->mTitle))
					{
						--mFriendArray.nEnemy;
					}
					else if (SubFriend::IsMyBlack(pos->mTitle))
					{
						--mFriendArray.nBlack;
					}
					pos->mTitle = pos->mTitle & 0xFFFF0000;

					SubFriend sf = *pos;
					sf.mTitle = 0;
					mSession->GetListener()->notifyFriendChange(mSession, sf);
				}
				else
					pos->mTitle = pos->mTitle & 0x0000FFFF;

				break;
			}
		}

		if (found)
		{
			EmptyFriendRemover efr;
			mFriendArray.reltionship.erase(std::remove_if(mFriendArray.reltionship.begin(),mFriendArray.reltionship.end(),efr),mFriendArray.reltionship.end());		
		}

		if (found && bAct)
		{
			// 对方关系和自己的关系也需要删除
			GameSession* gs = mGSL->FindSessionWithCharID(name);
			if (gs && gs->GetPlayer())
			{
				gs->GetPlayer()->DelRelationShip(mCharacter->mChrName, false);
			}
			else
			{
				// 加到删除缓存表中,等待该玩家上线,删除好友关系
				mGSL->GetRelationCacheManager()->PushCache(name, mCharacter->mChrName, 0);
			}
		}

		return found;
	}

	void DomainPlay::ChangeRelationShip(const char* name, int title, bool bAct)
	{
		SubFriend * p = NULL;
		bool found = false;
		bool bDelFriend = false;
		for( std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();
			pos != mFriendArray.reltionship.end();
			++ pos )
		{
			if( pos->mName.compare(name) == 0)
			{
				found = true;
				p = &(*pos);

				// 添加好友
				if (title == RELTYPE_FRIEND)
				{
					if (SubFriend::IsMyEnemy(pos->mTitle))
					{
						-- mFriendArray.nEnemy;
					}
					else if (SubFriend::IsMyBlack(pos->mTitle))
					{
						-- mFriendArray.nBlack;
					}
					else if (SubFriend::IsFriend(pos->mTitle))
					{
						break;
					}
					// 如果是添加好友操作,需将双方设置成好友
					pos->mTitle = RELTYPE_FRIEND;
					++ mFriendArray.nFriend;
					char szParam[255] = {0};
					sprintf_s(szParam, "%d %d", bAct?1:0, mFriendArray.nFriend);
					mGSL->GetScriptEngine()->PlayerLuaFunction(this, "player.onFriendAdd", name, szParam);
				}
				else if (title == RELTYPE_ENEMY)
				{
					// 添加仇人
					if (SubFriend::IsFriend(pos->mTitle))
					{
						-- mFriendArray.nFriend;
					}
					else if (SubFriend::IsMyBlack(pos->mTitle))
					{
						-- mFriendArray.nBlack;
					}
					else if (SubFriend::IsMyEnemy(pos->mTitle))
					{
						break;
					}
					if (bAct)
					{
						pos->mTitle = pos->mTitle & 0xFFFF0000;
						pos->mTitle = pos->mTitle | RELTYPE_ENEMY;
					}
					else
					{
						// 被动添加仇人
						if (pos->mTitle == RELTYPE_FRIEND)
						{
							// 原先是好友关系
							bDelFriend = true;
							pos->mTitle = 0;
						}
						int temp = RELTYPE_ENEMY;
						temp = temp << 16 & 0xFFFF0000;
						pos->mTitle = pos->mTitle & 0x0000FFFF;
						pos->mTitle = pos->mTitle | temp;
					}
					++ mFriendArray.nEnemy;
				}
				else if (title == RELTYPE_BLACK)
				{
					// 添加仇人
					if (SubFriend::IsFriend(pos->mTitle))
					{
						-- mFriendArray.nFriend;
					}
					else if (SubFriend::IsMyEnemy(pos->mTitle))
					{
						-- mFriendArray.nEnemy;
					}
					else if (SubFriend::IsMyBlack(pos->mTitle))
					{
						break;
					}
					if (bAct)
					{
						pos->mTitle = pos->mTitle & 0xFFFF0000;
						pos->mTitle = pos->mTitle | RELTYPE_BLACK;
					}
					else
					{
						// 被动添加仇人
						if (pos->mTitle == RELTYPE_FRIEND)
						{
							// 原先是好友关系
							bDelFriend = true;
							pos->mTitle = 0;
						}
						int temp = RELTYPE_BLACK;
						temp = temp << 16 & 0xFFFF0000;
						pos->mTitle = pos->mTitle & 0x0000FFFF;
						pos->mTitle = pos->mTitle | temp;
					}
					++ mFriendArray.nBlack;
				}
			}
		}

		GameSession* gs = mGSL->FindSessionWithCharID(name);
		if (gs && gs->GetPlayer())
		{
			if (!found)
			{
				// 添加
				SubFriend sub;
				sub.mName = name;
				sub.mSeedName = gs->GetPlayer()->mCharacter->mSeedName;
				sub.mLevel = gs->GetPlayer()->level();
				sub.mJob = gs->GetPlayer()->job();
				sub.mGender = gs->GetPlayer()->gender();
				sub.mGuildID = gs->GetPlayer()->GuildID();
				sub.mGuild = gs->GetPlayer()->GuildName();
				sub.mOnlineState = 1;
				if (title == RELTYPE_FRIEND)
				{
					sub.mTitle = RELTYPE_FRIEND;
					++ mFriendArray.nFriend;
				}
				else if (title == RELTYPE_ENEMY)
				{
					if (bAct)
					{
						sub.mTitle = sub.mTitle | RELTYPE_ENEMY;
						++ mFriendArray.nEnemy;
					}
					else
					{
						int temp = RELTYPE_ENEMY;
						temp = temp << 16;
						sub.mTitle = sub.mTitle & 0x0000FFFF | temp;
					}
				}
				else if (title == RELTYPE_BLACK)
				{
					if (bAct)
					{
						sub.mTitle = sub.mTitle | RELTYPE_BLACK;
						++ mFriendArray.nBlack;
					}
					else
					{
						int temp = RELTYPE_BLACK;
						temp = temp << 16;
						sub.mTitle = sub.mTitle & 0x0000FFFF | temp;
					}
				}
				mFriendArray.reltionship.push_back(sub);
				p = &mFriendArray.reltionship[mFriendArray.reltionship.size() - 1];
			}

			if (bAct)
			{
				gs->GetPlayer()->ChangeRelationShip(mCharacter->mChrName, title, false);
			}
		}
		else
		{
			// 不在线
			mGSL->GetRelationCacheManager()->PushCache(name, mCharacter->mChrName, title);
		}

		if ((bAct || title == RELTYPE_FRIEND || bDelFriend) && p)
		{
			mSession->GetListener()->notifyFriendChange(mSession, *p);
		}

		OGSLib::EmptyFriendRemover efr;
		mFriendArray.reltionship.erase(std::remove_if(mFriendArray.reltionship.begin(),mFriendArray.reltionship.end(),efr),mFriendArray.reltionship.end());	
	}

	void DomainPlay::FriendChange(const char* name,int title)
	{
		
		/*
		title < 0 删除
		title = 100 好友
		title = 101 仇人
		title = 102 黑名单
		*/
		double now = mGSL->GetTimerFix()->Elapsed();
		if (now - mApplyFriendTick < mGSL->mCheckChangeFriendRelationGap)
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_FRIEND_OPERATER_ERROR);
			return;
		}

		mApplyFriendTick = now;

		if( mSession->AuthType() != 101 )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_FRIEND_AUTHTYPE_ERROR);
			return ;
		}

		if( title <= 0 )
		{
			if (this->DelRelationShip(name, true))
			{
				char temp[512];
				_snprintf_s(temp,512,mGSL->mStringConsts.NOTIFY_FRIEND_DELETE,name);
				AlertMessage(10,0,temp);
			}
		}
		else
		{
			// 添加关系
			size_t nCount = 0;
			switch (title)
			{
			case RELTYPE_FRIEND:
				if (mFriendArray.nFriend > 99)
				{
					AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_FRIEND_TOO_MUCH);
					return;
				}
				break;
			case RELTYPE_ENEMY:
				if (mFriendArray.nEnemy > 99)
				{
					AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_ENEMY_TOO_MUCH);
					return;
				}
				break;
			case RELTYPE_BLACK:
				if (mFriendArray.nBlack > 99)
				{
					AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_BLACK_TOO_MUCH);
					return;
				}
				break;
			default:
				return;
			}

			if (title == RELTYPE_FRIEND)
			{
				// 添加好友
				ExeCode("player.onApplyFriend");
				for( std::vector<SubFriend>::iterator pos = mFriendArray.reltionship.begin();
					pos != mFriendArray.reltionship.end();
					++ pos )
				{
					if( pos->mName.compare(name) == 0)
					{
						if (SubFriend::IsFriend(pos->mTitle))
						{
							// 对方已经是好友, 检测是否重复添加
							return;
						}
					}
				}

				GameSession* gs = mGSL->FindSessionWithCharID(name);
				if (gs && gs->GetPlayer())
				{
					// 对方在线 发送请求
					gs->GetPlayer()->PlayerApplyFriendRequest(mCharacter->mChrName);
					return;
				}
				else
				{
					// 不在线,无法添加
					return;
				}
			}

			if (title == RELTYPE_BLACK || title == RELTYPE_ENEMY)
			{
				// 添加黑名单或仇人
				ChangeRelationShip(name, title, true);
			}
		}
// 		mSession->GetListener()->notifyFriendChange(mSession,name,title);
// 		mSession->GetListener()->notifyFriendFresh(mSession);
	}

	void DomainPlay::FriendFresh()
	{
// 		if( mFriendFreshTick + 60 < time(0) )
// 		{
// 			mFriendFreshTick = time(0);
// 			PushFriendData();
// 		}
	}
	void DomainPlay::PushAllItemData()
	{
		mSession->GetListener()->notifyListItemDesp(mSession);
		mSession->GetListener()->notifyListUpgradeDesp(mSession);
	}
	void DomainPlay::PushItemData()
	{
		mSession->GetListener()->notifyListItemChange(mSession, this);
		//for( ItemMap::iterator pos = GetItemList().begin();
		//	pos != GetItemList().end();
		//	++ pos )
		//{
		//	if( pos->mTypeID > 0 )
		//	{
		//		//PushItemInfo(pos->mTypeID);
		//		mSession->GetListener()->notifyItemChange(mSession,*pos,0);
		//	}
		//}
	}
	void DomainPlay::PushTradeItemData()
	{
		for( ItemMap::iterator pos = mTradeItem.begin();
			pos != mTradeItem.end();
			++ pos )
		{
			PushItemInfo(pos->mTypeID);
			mSession->GetListener()->notifyTradeItemChange(mSession,100,*pos,0);
		}
		if( ! mTradeTarget.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(mTradeTarget);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des && des->mTradeTarget==Name() )
				{
					for( ItemMap::iterator pos = des->mTradeItem.begin();
						pos != des->mTradeItem.end();
						++ pos )
					{
						PushItemInfo(pos->mTypeID);
						mSession->GetListener()->notifyTradeItemChange(mSession,101,*pos,0);
					}
				}
			}
		}
	}
	void DomainPlay::PushChatTradeItemData()
	{
		for( ItemMap::iterator pos = GetChatTradeItemList().begin();
			pos != GetChatTradeItemList().end();
			++ pos )
		{
			if(pos->mTypeID > 0 && pos->mPosition >= ITEM_CHATTRADE_BEGIN && pos->mPosition <ITEM_CHATTRADE_END)
			{
				PushItemInfo(pos->mTypeID);
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				mSession->GetListener()->notifyTradeItemChange(mSession,100,*pos,0);////此处的协议需要修改一下
			}		
		}
	}
	void DomainPlay::PushChatTradeTargetItemData(std::string mTradeTarget,int item_pos)
	{
		if( ! mTradeTarget.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(mTradeTarget);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des )
				{
					for( ItemMap::iterator pos = des->GetChatTradeItemList().begin();
						pos != des->GetChatTradeItemList().end();
						++ pos )
					{
						if(pos->mPosition >= ITEM_CHATTRADE_BEGIN && pos->mPosition < ITEM_CHATTRADE_END && pos->mPosition == item_pos && pos->mTypeID > 0)
						{
							PushItemInfo(pos->mTypeID);
							mSession->GetListener()->notifyTradeItemChange(mSession,101,*pos,0);
							return;
						}
					}
				}
			}
		}
	}
	void DomainPlay::PushItemInfo(const char* item_name)
	{
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(item_name);
		if( id )
		{
			PushItemInfo(id->mTypeID);
		}
	}
	void DomainPlay::PushItemInfo(int type_id)
	{

	}
	void DomainPlay::PushSkillDefine(int type_id,int level)
	{
		int id = type_id * 100 + level;
		if( std::find(mSkillDespSendList.begin(),mSkillDespSendList.end(),id) == mSkillDespSendList.end() )
		{
			mSession->GetListener()->notifySkillDefine(mSession,type_id, level);
			mSkillDespSendList.push_back(id);
		}
	}
	void DomainPlay::PushSkillData()
	{
		for( SkillMap::iterator pos = GetSkillList().begin();
			pos != GetSkillList().end();
			++ pos )
		{
			PushSkillDefine(pos->mTypeID,pos->mLevel);
			mSession->GetListener()->notifySkillChange(mSession,*pos);
		}
	}
	void DomainPlay::PushStatusDef()
	{
		for(int i = 0;i<STATUS_TYPE_NUMBER;i++)
		{
			if (i == 55 || i == 68 || i == 80 || i == 84)///翅膀和元神1,2的暂时不发,主动请求(数据量太大)
			{
				continue;
			}
			mSession->GetListener()->notifyStatusDesp(mSession,this,i);
		}
	}

	void DomainPlay::PushBuffDef()
	{
		ManagerStatus::BuffInfoMap& map =  mGSL->GetStatusManager()->GetBuffMap();
		ManagerStatus::BuffInfoMap::iterator it = map.begin();
		while (it != map.end())
		{
			mSession->GetListener()->notifyBuffDesp(mSession, it->first);
			++ it;
		}
	}
	void DomainPlay::PushShortcutData()
	{
		mSession->GetListener()->notifyLoadShortcut(mSession);
	}

	void DomainPlay::PushTaskData(int task_id,int flags)
	{
		for( TaskMap::iterator pos = mTaskMap.begin();
			pos != mTaskMap.end();
			++ pos )
		{
			if( task_id < 0 || task_id == pos->second.TaskID )
			{
				if( pos->second.TaskID >= 100 )
				{
					mSession->GetListener()->notifyTaskChange(mSession,pos->second.TaskID,flags);
				}
			}
		}
	}

	int DomainPlay::AddAndDressEquip(SubItem* ii, int src, int onPos, int show_flags)
	{
		ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);;if( ! id ) {return 0;}
		if( ! ItemDef::IsEquipment(ii->mSubType) ) return 0;
		
		int check = bag_use_item_equipment_check_limit(id);
		if( 0 != check )
		{
			if( 5 == check )
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_ZHUANSHENG_LEVEL_LIMIT);
				return 0;
			}
			if( 6 == check )
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_ITEM_USE_GENDER_ERROR);
				return 0;
			}
			if( 7 == check )
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_ITEM_USE_JOB_ERROR);
				return 0;
			}
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_USE_TIEM_LIMIT_ERROR);
			return 0;
		}

		PushItemInfo(id->mTypeID);
		//目标穿戴位必须为空
		SubItem* si = get_item_by_pos(onPos);if(si){return 1;}
		bool canDress = false;
		if (onPos == -2 * id->mEquipType)
		{
			canDress = true;
		}
		else
		{
			if (id->mEquipType == EQUIP_TYPE_GLOVE&& onPos == -2 * id->mEquipType - 1)
			{
				canDress = true;
			}
			else if (id->mEquipType == EQUIP_TYPE_RING&& onPos == -2 * id->mEquipType - 1)
			{
				canDress = true;
			}
		}
		if (!canDress){return 0;}
		SubItem it = * ii;
		it.mPosition = onPos;
		ii->mNumber-=0;
		it.mNumber = 1;
		GetItemList().push_back(it);
		if(id->mLastTime > 0){mNeedFreshItemList = true;}

		mSession->GetListener()->notifyItemChange(mSession,it,show_flags);

		UpdateAvatar();
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyAvatarChange(this);
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* play = static_cast<DomainPlay*>(*pos);
					play->NotifyPlayerAddinfo(this);
				}
			}
		}
		NotifyAvatarChange(this);
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
		mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);

		return onPos;
	}

	int DomainPlay::AddItem(SubItem* ii,int number,int src,int bag_full_action,int show_flags,int *topos)
	{
		ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);;if( ! id ) {return 0;}
		if (id->SubType == 17)
		{
			AddXuanJing(ii,number,src,bag_full_action,show_flags);return 0;
			return 0;
		}
		if (id->mEquipType == 31 || id->mEquipType == 32 || id->mEquipType == 33)
		{
			AddFashion(ii, number, src, bag_full_action, show_flags); return 0;
		}
		PushItemInfo(id->mTypeID);
		//if( pos ){*pos = -1;}
		if(topos)*topos=-1;
		if( ii->mTypeID == ITEM_GOLD_ID ){AddGameMoney(number,103);return 0;}
		if( ii->mTypeID == ITEM_GOLD_BIND_ID ){AddGameMoneyBind(number,103);return 0;}
		if( ii->mTypeID == ITEM_VCOIN_ID ){AddVcoin(number,103);return 0;}
		if( ii->mTypeID == ITEM_VCOIN_BIND_ID ){AddVcoinBind(number,103);return 0;}
		if( ii->mTypeID == ITEM_EXP_ID ){AddExperience(number,0);return 0;}
		//if(ii->mTypeID==ITEM_CAPACITY_ID){AddCapacity(number,103);return 0;}
		if(number>1000){return 0;}
		bool add_to_depot_on_bag_full = bag_full_action%10==1;
		bool drop_on_groud_on_slot_full = bag_full_action/10%10 == 1;
		int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(id->mTypeID);if(stackmax <=0){stackmax=1;}
		int repeatnum = 0;
		while(number >0)
		{
			repeatnum++;
			if( repeatnum > 1000 )
			{
				printf("additem dying id:%d num:%d stack:%d",ii->mTypeID,number,stackmax);
				break;
			}
			if(number <= stackmax){ii->mNumber = number;number = 0;}
			else{ii->mNumber = stackmax;number -= stackmax;}
			//包裹:一致的物品数量叠加
			if( stackmax > 1 )
			{
				for( ItemMap::iterator pos = GetItemList().begin();
					pos != GetItemList().end();
					++ pos )
				{
					if( pos->mTypeID > 0 && pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < (ITEM_BAG_SIZE+mBagSlotAdd) && pos->mTypeID == ii->mTypeID && pos->mNumber<stackmax && pos->mItemFlags == ii->mItemFlags)
					{
						int sub = max(0,min(ii->mNumber,stackmax-pos->mNumber));
						ii->mNumber-=sub;
						pos->mNumber += sub;
						if(topos)*topos=pos->mPosition;
						mSession->GetListener()->notifyItemChange(mSession,*pos,show_flags);
						if( ii->mNumber <= 0 ) break;
					}
				}
			}
			//叠加后放入空位置
			int fkhere1 = 0;
			while( ii->mNumber > 0 )
			{
				fkhere1++;
				if(fkhere1 > 1000){printf("additem while1 dying id:%d num:%d stack:%d",ii->mTypeID,ii->mNumber,stackmax);break;}
				int position = find_empty_item_position();
				if( position >= 0 )
				{
					SubItem it = * ii;
					it.mPosition = position;
					if(topos)*topos=position;
					int sub = max(0,min(ii->mNumber,stackmax));
					ii->mNumber-=sub;
					it.mNumber = sub;
					GetItemList().push_back(it);
					if(id->mLastTime > 0){mNeedFreshItemList = true;}
					mSession->GetListener()->notifyItemChange(mSession,it,show_flags);
				}else{break;}
			}
			//包裹满行为、存仓库
			if( ii->mNumber > 0 && add_to_depot_on_bag_full )
			{
				int fkhere2 = 0;
				while( ii->mNumber > 0 )
				{
					fkhere2++;
					if(fkhere2 > 1000){printf("additem while2 dying id:%d num:%d stack:%d",ii->mTypeID,ii->mNumber,stackmax);break;}
					int position = find_empty_item_position_in_depot();
					if( position >= 0 )
					{
						SubItem it = * ii;
						it.mPosition = position;
						if(topos)*topos=position;
						int sub = max(0,min(ii->mNumber,stackmax));
						ii->mNumber-=sub;
						it.mNumber = sub;
						GetItemList().push_back(it);
						if(id->mLastTime > 0){mNeedFreshItemList = true;}
						mSession->GetListener()->notifyItemChange(mSession,it,show_flags);
					}else{break;}
				}
			}
			//掉落物品
			if( ii->mNumber > 0 && drop_on_groud_on_slot_full )
			{
				DropTheItem(ii);
				ii->mNumber = 0;
			}
			//其他
			if( ii->mNumber > 0 )
			{
				DropTheItem(ii);
				ii->mNumber = 0;
			}
		}
		if( mGSL->mTrigerOnGetItem || mGSL->GetItemDefineManager()->GetItemTriger(ii->mTypeID) )
		{
			ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);
			if( id )
			{
				char szId[255] = {0};
				sprintf_s(szId, "%I64d", ii->mIdentId);
				mGSL->GetScriptEngine()->ItemPlayer(this, id->mName.c_str(), id->mTypeID, "player.onGetItem", 1,-999, szId);
			}
		}
		return 0;
	}
	int DomainPlay::AddXuanJing(SubItem* ii,int number,int src,int bag_full_action,int show_flags,int *pos)
	{
		ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);;if( ! id ) {return 0;}
		PushItemInfo(id->mTypeID);
		if( id->SubType != 17 ){return 0;}
		if(number>1000){return 0;}
		int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(id->mTypeID);if(stackmax<=0){stackmax=1;}
		int position = find_empty_item_position_in_xuanjing();
		int repeatnum = 0;
		int addnum=0;
		while(number >0)
		{
			repeatnum++;if( repeatnum > 1000 ){printf("additem xuanjing dying id:%d num:%d stack:%d",ii->mTypeID,number,stackmax);break;}
			if(ii->mNumber<=0){ii->mNumber = stackmax;}
			if(number <= stackmax){ii->mNumber = number;addnum+=ii->mNumber;number -= ii->mNumber;number=max(0,number);}
			else{addnum+=stackmax;number -= stackmax;}
			//包裹:一致的物品数量叠加
			if( stackmax > 1 )
			{
				for( ItemMap::iterator pos = GetItemList().begin();
					pos != GetItemList().end();
					++ pos )
				{
					if( pos->mTypeID > 0 && pos->mPosition >= ITEM_XUANJING_BEGIN && pos->mPosition < ITEM_XUANJING_END && pos->mTypeID == ii->mTypeID && pos->mNumber<stackmax && pos->mItemFlags == ii->mItemFlags)
					{
						int sub = max(0,min(ii->mNumber,stackmax-pos->mNumber));
						ii->mNumber-=sub;
						pos->mNumber += sub;
						mSession->GetListener()->notifyItemChange(mSession,*pos,show_flags);
						if( ii->mNumber <= 0 ) break;
					}
				}
			}
			int fkhere1 = 0;
			while( ii->mNumber > 0 )
			{
				fkhere1++;
				if(fkhere1 > 1000){printf("additem xuanjing while1 dying id:%d num:%d stack:%d",ii->mTypeID,ii->mNumber,stackmax);break;}
				int position = find_empty_item_position_in_xuanjing();
				if( position >= 0 )
				{
					SubItem it = * ii;
					it.mPosition = position;
					int sub = max(0,min(ii->mNumber,stackmax));
					ii->mNumber-=sub;
					it.mNumber = sub;
					GetItemList().push_back(it);
					mSession->GetListener()->notifyItemChange(mSession,it,show_flags);
					PushItemInfo(it.mTypeID);
					if(id->mLastTime > 0){mNeedFreshItemList = true;}
				}else{break;}
			}
			if( ii->mNumber > 0  )
			{
				//todo...
			}
		}
		//mGSL->GetCountManager()->count_item(ii->mTypeID,addnum);
		return 0;
	}
	int DomainPlay::AddFashion(SubItem* ii, int number, int src, int bag_full_action, int show_flags, int *pos)
	{
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);; if (!id) { return 0; }
		PushItemInfo(id->mTypeID);
		//if (id->SubType != 17){ return 0; }
		if (number>1000){ return 0; }
		int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(id->mTypeID); if (stackmax <= 0){ stackmax = 1; }
		int position = find_empty_item_position_fashion();
		int repeatnum = 0;
		int addnum = 0;
		while (number >0)
		{
			repeatnum++; if (repeatnum > 1000){ printf("additem xuanjing dying id:%d num:%d stack:%d", ii->mTypeID, number, stackmax); break; }
			if (ii->mNumber <= 0){ ii->mNumber = stackmax; }
			if (number <= stackmax){ ii->mNumber = number; addnum += ii->mNumber; number -= ii->mNumber; number = max(0, number); }
			else{ addnum += stackmax; number -= stackmax; }
			//id一样则 lasttime +
			if (stackmax > 1)
			{
				for (ItemMap::iterator pos = GetItemList().begin();
					pos != GetItemList().end();
					++pos)
				{
					if (pos->mTypeID > 0 && pos->mPosition >= EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN && pos->mPosition < EQUIP_TYPE_FASHION_CLOTH_POS_END && pos->mTypeID == ii->mTypeID && pos->mNumber<stackmax && pos->mItemFlags == ii->mItemFlags)
					{
						int sub = max(0, min(ii->mNumber, stackmax - pos->mNumber));
						ii->mNumber -= sub;
						pos->mNumber += sub;
						mSession->GetListener()->notifyItemChange(mSession, *pos, show_flags);
						if (ii->mNumber <= 0) break;
					}
				}
			}
			int fkhere1 = 0;
			while (ii->mNumber > 0)
			{
				fkhere1++;
				if (fkhere1 > 1000){ printf("additem xuanjing while1 dying id:%d num:%d stack:%d", ii->mTypeID, ii->mNumber, stackmax); break; }
				int position = find_empty_item_position_fashion();
				if (position >= 0)
				{
					SubItem it = *ii;
					it.mPosition = position;
					int sub = max(0, min(ii->mNumber, stackmax));
					ii->mNumber -= sub;
					it.mNumber = sub;
					GetItemList().push_back(it);
					mSession->GetListener()->notifyItemChange(mSession, it, show_flags);
					PushItemInfo(it.mTypeID);
					if (id->mLastTime > 0){ mNeedFreshItemList = true; }
				}
				else{ break; }
			}
			if (ii->mNumber > 0)
			{
				//todo...
			}
		}
		//mGSL->GetCountManager()->count_item(ii->mTypeID,addnum);
		return 0;
	}
	int DomainPlay::AddItem_Lottery(SubItem* ii,int number,int src,int bag_full_action,int show_flags,int *pos)
	{
		ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);;if( ! id ) {return 0;}
		if( id->SubType == 17 )
		{
			AddXuanJing(ii,number,src,bag_full_action,show_flags);return 0;
		}
		PushItemInfo(id->mTypeID);
		if( ii->mTypeID == ITEM_GOLD_ID ){AddGameMoney(number,103);return 0;}
		if( ii->mTypeID == ITEM_GOLD_BIND_ID ){AddGameMoneyBind(number,103);return 0;}
		if( ii->mTypeID == ITEM_VCOIN_ID ){AddVcoin(number,103);return 0;}
		if( ii->mTypeID == ITEM_VCOIN_BIND_ID ){AddVcoinBind(number,103);return 0;}
		if( ii->mTypeID == ITEM_EXP_ID ){AddExperience(number,0);return 0;}
		//if(ii->mTypeID==ITEM_CAPACITY_ID){AddCapacity(number,103);return 0;}
		if(number>1000){return 0;}
		int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(id->mTypeID);if(stackmax<=0){stackmax=1;}
		int position = find_empty_item_position_in_lotterydepot();
		int repeatnum = 0;
		while(number >0)
		{
			repeatnum++;if( repeatnum > 1000 ){printf("additem lottery dying id:%d num:%d stack:%d",ii->mTypeID,number,stackmax);break;}
			if(ii->mNumber<=0){ii->mNumber = stackmax;}
			if(number <= stackmax){ii->mNumber = number;number -= ii->mNumber;number=max(0,number);}
			else{number -= stackmax;}
			//包裹:一致的物品数量叠加
			if( stackmax > 1 )
			{
				for( ItemMap::iterator pos = GetItemList().begin();
					pos != GetItemList().end();
					++ pos )
				{
					if( pos->mTypeID > 0 && pos->mPosition >= ITEM_LOTTERYDEPOT_BEGIN && pos->mPosition < ITEM_LOTTERYDEPOT_END && pos->mTypeID == ii->mTypeID && pos->mNumber<stackmax && pos->mItemFlags == ii->mItemFlags)
					{
						int sub = max(0,min(ii->mNumber,stackmax-pos->mNumber));
						ii->mNumber-=sub;
						pos->mNumber += sub;
						mSession->GetListener()->notifyItemChange(mSession,*pos,show_flags);
						if( ii->mNumber <= 0 ) break;
					}
				}
			}
			int fkhere1 = 0;
			while( ii->mNumber > 0 )
			{
				fkhere1++;
				if(fkhere1 > 1000){printf("additem lottery while1 dying id:%d num:%d stack:%d",ii->mTypeID,ii->mNumber,stackmax);break;}
				int position = find_empty_item_position_in_lotterydepot();
				if( position >= 0 )
				{
					SubItem it = * ii;
					it.mPosition = position;
					int sub = max(0,min(ii->mNumber,stackmax));
					ii->mNumber-=sub;
					it.mNumber = sub;
					GetItemList().push_back(it);
					PushItemInfo(it.mTypeID);
					if(id->mLastTime > 0){mNeedFreshItemList = true;}
					mSession->GetListener()->notifyItemChange(mSession,it,show_flags);
				}else{break;}
			}
			if( ii->mNumber > 0  )
			{
				int fkhere2 = 0;
				while( ii->mNumber > 0 )
				{
					fkhere2++;
					if(fkhere2 > 1000){printf("additem lottery while2 dying id:%d num:%d stack:%d",ii->mTypeID,ii->mNumber,stackmax);break;}
					int position = find_empty_item_position_in_depot();
					if( position >= 0 )
					{
						SubItem it = * ii;
						it.mPosition = position;
						int sub = max(0,min(ii->mNumber,stackmax));
						ii->mNumber-=sub;
						it.mNumber = sub;
						GetItemList().push_back(it);
						PushItemInfo(it.mTypeID);
						if(id->mLastTime > 0){mNeedFreshItemList = true;}
						mSession->GetListener()->notifyItemChange(mSession,it,show_flags);
					}else{break;}
				}
			}
			if( ii->mNumber > 0 )
			{
				DropTheItem(ii);
				ii->mNumber = 0;
			}
			//其他
			if( ii->mNumber > 0 )
			{
				DropTheItem(ii);
				ii->mNumber = 0;
			}
		}
		return 0;
	}
	int DomainPlay::AddSkill(int type_id,int level)
	{
		for( SkillMap::iterator pos = GetSkillList().begin();
			pos != GetSkillList().end();
			++ pos )
		{
			if( pos->mTypeID == type_id )
			{
				return 1;
			}
		}
		SubSkill skill;
		skill.mTypeID = type_id;
		skill.mLevel = level;
		skill.mExp = 0;
		GetSkillList().push_back(skill);
		PushSkillDefine(skill.mTypeID,skill.mLevel);
		mSession->GetListener()->notifySkillChange(mSession,skill);
		if( type_id == SkillDef::SKILL_TYPE_JinShenLiZhanFa
			|| type_id == SkillDef::SKILL_TYPE_JiChuJianShu )
		{
			CalculateAttribute();
		}
		if( type_id == SkillDef::SKILL_TYPE_CiShaJianShu )
		{
			OpenCiShaJianShu();
		}
		else if (type_id == SkillDef::SKILL_TYPE_BanYueWanDao)
		{
			OpenBanYueWanDao();
		}
		return 0;
	}
	int DomainPlay::AddSkillExp(int type_id,int exp)
	{
		if( exp <= 0 ) return 0;
		for( SkillMap::iterator pos = GetSkillList().begin();
			pos != GetSkillList().end();
			++ pos )
		{
			if( pos->mTypeID == type_id )
			{
				pos->mExp += exp;
				mSession->GetListener()->notifySkillChange(mSession,*pos);
				return 1;
			}
		}
		return 0;
	}
	bool DomainPlay::find_validate_map_position(int* x,int* y)
	{
		if( mParentMap )
		{
			return mParentMap->GenValidateRandomPosition(x,y);
		}
		return false;
	}
	bool DomainPlay::get_home_map_position(std::string& map_id,int* x,int* y)
	{
		if( !x || !y ) return false;
		if( mLastSafeAreaID > 0 )
		{
			MapSafeArea msa;
			if( mSession->GetGameServerLib()->GetMapManager()->FindSafeArea(mLastSafeAreaID,&msa) )
			{
				PlayMap* map = mGSL->GetMapManager()->FindMap(msa.map_id);
				if( map )
				{
					map_id = msa.map_id;
					if( map->GenValidateRandomPosition(msa.x,msa.y,msa.size,x,y) )
					{
						return true;
					}
				}
			}
		}
		else
		{
			PlayMap* map = mGSL->GetMapManager()->FindMap(mGSL->mStart_map);
			if( map )
			{
				map_id = mGSL->mStart_map;
				if( map->GenValidateRandomPosition(mGSL->mStart_x,mGSL->mStart_y,mGSL->mStart_size,x,y) )
				{
					return true;
				}
			}
		}
		return false;
	}
	bool DomainPlay::go_home()
	{
		int x,y;
		if( mParentMap )
		{
			if(mParentMap->HomePos())
			{
				DirectFlyNode* node = mGSL->GetFlyManager()->FindNode(mParentMap->HomePos());
				if(node)
				{
					if(EnterMap(node->mTargetMapID,node->mTargetX,node->mTargetY,node->mTargetSize))
					{
						return true;
					}
				}
			}
		}

		std::string map_id;
		if( get_home_map_position(map_id,&x,&y) )
		{
			EnterMap(map_id,x,y);
			return true;
		}
		return false;
	}
	int DomainPlay::NumBagBlack()
	{
		int i = 0;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && pos->mPosition < 256 && pos->mTypeID>0 )
			{
				i++;
			}
		}
		return max(0,(ITEM_BAG_END+mBagSlotAdd-ITEM_BAG_BEGIN) - i);
	}
	int DomainPlay::NumBag()
	{
		return ITEM_BAG_END+mBagSlotAdd-ITEM_BAG_BEGIN;
	}
	int DomainPlay::NumXjBlack()
	{
		int i = 0;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if (pos->mTypeID > 0 && (pos->mPosition >= ITEM_XUANJING_BEGIN && pos->mPosition <= ITEM_XUANJING_END))
			{
				i++;
			}
		}
		return max(0,ITEM_XUANJING_SIZE - i);
	}
	int DomainPlay::NumItem(int type_id,int flags,int bind)
	{
		int num = 0;
		int count_bag = flags % 10;
		int count_use = flags / 10 % 10;
		int count_depot = flags / 100 % 10;
		int count_xuanjingbag = flags / 1000 % 10;///flags=1000
		int count_lottery = flags/10000%10;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mTypeID == type_id && ((bind>=0 && pos->mItemFlags == bind) || bind < 0 ) )
			{
				if( pos->mPosition < 0 && count_use )
				{
					num += pos->mNumber;
				}
				if( pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && count_bag )
				{
					num += pos->mNumber;
				}
				if( pos->mPosition >= ITEM_DEPOT_BEGIN && pos->mPosition<=ITEM_DEPOT_END+mDepotSlotAdd && count_depot )
				{
					num += pos->mNumber;
				}
				if (pos->mPosition >= ITEM_XUANJING_BEGIN && pos->mPosition<=ITEM_XUANJING_END && count_xuanjingbag)
				{
					num += pos->mNumber;
				}
				if(pos->mPosition>=ITEM_LOTTERYDEPOT_BEGIN && pos->mPosition<=ITEM_LOTTERYDEPOT_END && count_lottery)
				{
					num += pos->mNumber;
				}
			}
		}
		return num;
	}
	int DomainPlay::CheckItemNumber(int type_id,int number)
	{
		if( number <= 0 )
		{
			return 2;
		}
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mTypeID == type_id )
			{
				number -= pos->mNumber;
				if( number <= 0 )
				{
					return 0;
				}
			}
		}
		return 1;
	}
	int DomainPlay::CheckItemPosition(int type_id,int position)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == position && pos->mTypeID == type_id )
			{
				return 0;
			}
		}
		return 1;
	}
	int DomainPlay::DropItemByTypeID(int type_id)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID <= 0 )continue;
			if( type_id == pos->mTypeID )
			{
				SubItem ii = *pos;
				if( mParentMap && mParentMap->CanDropItem() && !(ii.mItemFlags&ITEM_FLAG_BIND))
				{
					ii.mPosition = 0;
					DropTheItem(&ii);
					
					pos->mTypeID = 0;
					mNeedClearItemList = true;
					mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					if(pos->mPosition <0 )
					{
						UpdateAvatar();
						for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
							pos != mVisibleInteractiveObjectList.end();
							++ pos )
						{
							if ((*pos))
							{
								(*pos)->NotifyAvatarChange(this);
							}
						}
						NotifyAvatarChange(this);
						CalculateAttribute();
						mSession->GetListener()->notifyAttributeChange(mSession);
						mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
					}
					ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
					if (id)
					{
						char szId[255] = {0};
						sprintf_s(szId, "%I64d", ii.mIdentId);
						mGSL->GetScriptEngine()->ItemPlayer(this, id->mName.c_str(), id->mTypeID, "player.onDropItem", 1, -999, szId);
					}
				}
			}
		}
		return 0;
	}
	void DomainPlay::DieDropItem()
	{
		bool updateavatar=false;
		if( mParentMap && mParentMap->NoProtectNewer()==0 )
		{
			if( mGSL->mDieDropLimitLevel > level() )
			{
				return;
			}
		}
		if ( IsNoDrop() && rand() %10000 < StatusParam(STATUS_TYPE_NO_DROP) )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_STATUS_NO_DROP_ON_DIE);
			return;
		}
		if( mGSL->mDieDropExp > 0 )
		{
			if( rand()%10000 < mGSL->mDieDropExpProb )
			{
				RemExperience(int((mExpNext&0x7FFFFFFF) / 10000.0 * mGSL->mDieDropExpPresent),100);
			}
		}
		if( mGSL->mDieDropGold > 0 )
		{
			if( rand()%1000 < mGSL->mDieDropGoldProb )
			{
				int dropgoldnum=int(gamemoney()/10000.0 * mGSL->mDieDropExpPresent);
				if( dropgoldnum > 0 && RemGameMoney(dropgoldnum,155) )
				{
					SubItem ii;
					ii.mTypeID = ITEM_GOLD_ID;
					DropTheItem(&ii,dropgoldnum);
				}
			}
		}
		SubItem* equip[20];
		int equip_num=0;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID <= 0 )continue;
			if( pos->mItemFlags & ITEM_FLAG_BIND )continue;
			if( pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd )
			{
				//包裹物品
				if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) && mGSL->GetItemDefineManager()->CheckItemTradeLimit(pos->mTypeID) >= 10 ){continue;}
				if( RandRange(mGSL->mDieDropBagProb+min(50000,max(0,pos->mProtect)))*(1000-Attributes()->mDropProb)/1000 ==0 || mPKValue >= 400 )
				{
					if( mParentMap && mParentMap->CanDropItem() )
					{
						SubItem ii=*pos;
						ii.mPosition = 0;
						if(pos->mNumber > 2)
						{
							int nn = rand()%(1+pos->mNumber/3);
							ii.mNumber = min(5,max(nn,1));
						}
						else
						{
							ii.mNumber = 1;
						}
						DropTheItem(&ii);
						pos->mNumber -= ii.mNumber;
						if(pos->mNumber <= 0)
						{
							pos->mTypeID = 0;
							mNeedClearItemList = true;
						}
						mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					}
				}
				else
				{
					if( RandRange(mGSL->mDieDropLoadProb) == 0 )
					{
						if( pos->mProtect > 0 )
						{
							pos->mProtect -= 1;
							mSession->GetListener()->notifyItemChange(mSession,*pos,0);
						}
					}
				}
			}
			if( pos->mPosition < ITEM_BAG_BEGIN )
			{
				equip[equip_num] = &(*pos);
				equip_num ++;
			}
		}
		//身上物品
		int load_item_num=0;
		if( equip_num > 0 )
		{
			int start = rand() % equip_num;
			for(int i = 0;i < equip_num; i ++ )
			{
				if( load_item_num >= 2 ) continue;
				int id = (start+i) % equip_num;
				if( equip[id] )
				{
					ItemDef* iid = mGSL->GetItemDefineManager()->GetItemDefine(equip[id]->mTypeID);
					if( ! iid ) continue;
					if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) && mGSL->GetItemDefineManager()->CheckItemTradeLimit(equip[id]->mTypeID) >= 10 ){continue;}
					if( RandRange(mGSL->mDieDropLoadProb*(load_item_num+1)+iid->mProtect +equip[id]->mLevel+min(50000,max(0,equip[id]->mProtect)) )*(1000-Attributes()->mDropProb)/1000==0 )
					{
						load_item_num++;
						if( mParentMap && mParentMap->CanDropItem() )
						{
							SubItem ii=*equip[id];
							ii.mPosition = 0;
							DropTheItem(&ii);
						}
						equip[id]->mNumber --;
						if(equip[id]->mNumber <= 0)
						{
							equip[id]->mTypeID = 0;
							mNeedClearItemList = true;
						}
						if(equip[id]->mPosition < 0 ) updateavatar=true;
						mSession->GetListener()->notifyItemChange(mSession,*equip[id],0);
					}
					else
					{
						if( RandRange(mGSL->mDieDropLoadProb) == 0 )
						{
							if( equip[id]->mProtect > 0 )
							{
								equip[id]->mProtect -= 1;
								mSession->GetListener()->notifyItemChange(mSession,*equip[id],0);
							}
						}
					}
				}
			}
		}
		if(updateavatar)
		{
			UpdateAvatar();
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyAvatarChange(this);
					if ((*pos)->TypeIsPlayer())
					{
						DomainPlay* play = static_cast<DomainPlay*>(*pos);
						play->NotifyPlayerAddinfo(this);
					}
				}
			}
			NotifyAvatarChange(this);
			CalculateAttribute();
			mSession->GetListener()->notifyAttributeChange(mSession);
			mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
		}
	}
	int DomainPlay::Clear_Item()
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 )
			{
				pos->mTypeID = 0;
				mNeedClearItemList = true;
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
			}
		}
		UpdateAvatar();
		CalculateAttribute();
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyAvatarChange(this);
			}
		}
		NotifyAvatarChange(this);
		mSession->GetListener()->notifyAttributeChange(mSession);
		mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
		return 0;
	}
	int DomainPlay::get_item_type_by_pos(int position)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && position==pos->mPosition )
			{
				return pos->mTypeID;
			}
		}
		return 0;
	}
	int DomainPlay::push_item_change_by_pos_type(int position,int type_id)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mTypeID == type_id && position==pos->mPosition )
			{
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				break;
			}
		}
		return 0;
	}
	SubItem* DomainPlay::get_item_by_pos(int position)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && position==pos->mPosition )
			{
				return &(*pos);
			}
		}
		return 0;
	}
	SubItem* DomainPlay::get_saleitem_by_pos(int position)
	{
		for( ItemMap::iterator pos = MySaleList().begin();
			pos != MySaleList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && position==pos->mPosition )
			{
				return &(*pos);
			}
		}
		return 0;
	}
	SubItem* DomainPlay::get_item_by_pos_type(int position,int type_id)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mTypeID == type_id && position==pos->mPosition )
			{
				return &(*pos);
			}
		}
		return 0;
	}
	int DomainPlay::RemItemPosition(int type_id,int position,int number,int src)
	{
		int ret=number;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mTypeID == type_id && position==pos->mPosition )
			{
				if(pos->mNumber >= number){ret = 0;}else{ret=number-pos->mNumber;}
				pos->mNumber -= number;
				if(pos->mNumber <= 0)
				{
					pos->mTypeID = 0;
					mNeedClearItemList = true;
				}
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				break;
			}
		}
		if( position < 0 )
		{
			UpdateAvatar();
			CalculateAttribute();
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyAvatarChange(this);
				}
			}
			NotifyAvatarChange(this);
			mSession->GetListener()->notifyAttributeChange(mSession);
			mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
		}
		return ret;
	}
	int DomainPlay::RemItemDuration(int sub_type,int number,int flags)
	{
		int done_number=number;
		int count_bag = flags % 10;
		int count_use = flags / 10 % 10;
		int count_depot = flags / 100 % 10;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && ItemDef::IsDissipative(pos->mSubType) )
			{
				if( pos->mPosition < 0 && (!count_use) )
				{
					continue;
				}
				if( pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && (!count_bag) )
				{
					continue;
				}
				if( pos->mPosition >= ITEM_DEPOT_BEGIN && (!count_depot) )
				{
					continue;
				}
				if( pos->mDuration > 0 )
				{
					ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id && id->SubType == sub_type )
					{
						for(int i = done_number;i > 0&&pos->mDuration>0;i--)
						{
							done_number --;
							pos->mDuration --;
						}
						if( pos->mDuration <= 0 )
						{
							pos->mTypeID = 0;
							mNeedClearItemList = true;
						}
						mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					}
				}
				if( done_number <= 0 )
				{
					break;
				}
			}
		}
		return number - done_number;
	}
	int DomainPlay::RemItemNumber(int type_id,int number,int flags,int src_log,int bind)
	{
		if( number <= 0 )
		{
			return 0;
		}
		if( NumItem(type_id,flags,bind) < number )
		{
			return 0;
		}
		int done_number=number;
		int count_bag = flags % 10;
		int count_use = flags / 10 % 10;
		int count_depot = flags / 100 % 10;
		int count_xuanjingbag = flags / 1000 % 10;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mTypeID == type_id &&( (bind>=0 && pos->mItemFlags == bind) || bind < 0 ) )
			{
				if( pos->mPosition < 0 && (!count_use) )
				{
					continue;
				}
				if( pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && (!count_bag) )
				{
					continue;
				}
				if (pos->mPosition >= ITEM_DEPOT_BEGIN && pos->mPosition < ITEM_DEPOT_END + mDepotSlotAdd && (!count_depot))
				{
					continue;
				}
				if (pos->mPosition >= ITEM_XUANJING_BEGIN && (!count_xuanjingbag))
				{
					continue;
				}
				if(pos->mNumber > done_number)
				{
					pos->mNumber -= done_number;
					done_number = 0;
				}
				else
				{
					done_number -= pos->mNumber;
					pos->mTypeID = 0;
					mNeedClearItemList = true;
				}
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				if( done_number <= 0 )
				{
					break;
				}
			}
		}
		if (mGSL->mTrigerOnGetItem || mGSL->GetItemDefineManager()->GetItemTriger(type_id))
		{
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
			if (id)
			{
				mGSL->GetScriptEngine()->ItemPlayer(this, id->mName.c_str(), id->mTypeID, "player.onRemItem");
			}
		}
		return number - done_number;
	}
	int DomainPlay::CheckEquiped(int pp)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == pp )
			{
				return 1;
			}
		}
		return 0;
	}
	int DomainPlay::GetWeaponLuck()
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == ITEM_WEAPON_POSITION )
			{
				return pos->mLuck;
			}
		}
		return 0;
	}
	int DomainPlay::SetWeaponLuck(int luck)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == ITEM_WEAPON_POSITION )
			{
				pos->mLuck = luck;
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				CalculateAttribute();
				mSession->GetListener()->notifyAttributeChange(mSession);
				return luck;
			}
		}
		if( luck == 0 )
		{
			return -1;
		}
		return 0;
	}
	int DomainPlay::ChangeWeaponLuck(int change)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == ITEM_WEAPON_POSITION )
			{
				pos->mLuck += change;
				if( pos->mLuck < -10 )
				{
					pos->mLuck = -10;
				}
				if( pos->mLuck > 10 )
				{
					pos->mLuck = 10;
				}
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				CalculateAttribute();
				mSession->GetListener()->notifyAttributeChange(mSession);
				return change;
			}
		}
		if( change == 0 )
		{
			return -1;
		}
		return 0;
	}
	int DomainPlay::use_item_by_type(int type_id)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && pos->mTypeID == type_id )
			{
				BagUseItem(pos->mPosition,type_id);
			}
		}
		return 0;
	}
	int DomainPlay::UseSomeBS(int bslv)
	{
		if (bslv >= 1 && bslv <= 12)
		{
			mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this, bslv, 0, "gui.PanelQiangHua.onUseBaoshi");
		}
		return 0;
	}
	void DomainPlay::DamageLoadEquipment(int dura_damage)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition < -4 )
			{
				if( pos->mPosition == -22 ){continue;}
				if( pos->mPosition != -6 )
				{
					if( rand()%8 )
					{
						continue;
					}
				}
				int old=pos->mDuration/1000;
				pos->mDuration-=dura_damage;
				if( pos->mDuration < 0 )
				{
					pos->mDuration = 0;
				}
				if( old != pos->mDuration/1000 )
				{
					mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				}
			}
		}
	}
	void DomainPlay::DamageWeaponDuration(int dura_damage)
	{
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == -4 )
			{
				int old=pos->mDuration/1000;
				pos->mDuration-=dura_damage;
				if( pos->mDuration < 0 )
				{
					pos->mDuration = 0;
				}
				if( old != pos->mDuration/1000 )
				{
					mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				}
				return;
			}
		}
	}
	//void DomainPlay::BoothAdd(int position,int type_id,int pay_type,int pay_price)
	//{
	//	mBoothOpen = true;
	//	if( pay_type!= 100 && pay_type!= 102 ) return ;
	//	if( position < ITEM_BAG_BEGIN || position >= ITEM_BAG_END+mBagSlotAdd ) return;
	//	for( ItemMap::iterator pos = GetItemList().begin();
	//		pos != GetItemList().end();
	//		++ pos )
	//	{
	//		if( pos->mTypeID > 0 && pos->mPosition == position && pos->mTypeID == type_id )
	//		{
	//			pos->mBoothType = pay_type;
	//			pos->mBoothPrice = pay_price;
	//			mSession->GetListener()->resBoothAdd(mSession,position,type_id,pay_type,pay_price);
	//		}
	//	}
	//}
	//void DomainPlay::BoothClose()
	//{
	//	mBoothOpen = false;
	//	for( ItemMap::iterator pos = GetItemList().begin();
	//		pos != GetItemList().end();
	//		++ pos )
	//	{
	//		pos->mBoothType = 0;
	//		pos->mBoothPrice = 0;
	//	}
	//	mSession->GetListener()->resBoothClose(mSession);
	//}
	int DomainPlay::BagUseItem(int position,int type_id,int num,int to)
	{
		//在背包或者在衣橱
		if (position >= ITEM_BAG_BEGIN && position <= ITEM_BAG_END + mBagSlotAdd || position >= EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN && position <= EQUIP_TYPE_FASHION_CLOTH_POS_END)
		{
			for (ItemMap::iterator pos = GetItemList().begin();
				pos != GetItemList().end();
				++pos)
			{
				if (pos->mTypeID > 0 && pos->mPosition == position && pos->mTypeID == type_id)
				{
					int check = bag_use_item_equipment_check_limit(pos);
					if (0 != check)
					{
						if (5 == check)
						{
							AlertMessage(1, 0, mGSL->mStringConsts.NOTIFY_ZHUANSHENG_LEVEL_LIMIT);
							return 0;
						}
						if (6 == check)
						{
							AlertMessage(1, 0, mGSL->mStringConsts.NOTIFY_ITEM_USE_GENDER_ERROR);
							return 0;
						}
						if (7 == check)
						{
							AlertMessage(1, 0, mGSL->mStringConsts.NOTIFY_ITEM_USE_JOB_ERROR);
							return 0;
						}
						AlertMessage(1, 0, mGSL->mStringConsts.NOTIFY_USE_TIEM_LIMIT_ERROR);
						return 0;
					}
					if (ItemDef::IsDissipative(pos->mSubType))
					{
						bag_use_item_dissipative(pos, num);
						break;
					}
					if (ItemDef::IsEquipment(pos->mSubType))
					{
						if (to == 0)
						{
							bag_use_item_equipment(pos);
						}
						else
						{
							//仅支持手镯和戒指
							ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
							if (to == ITEM_GLOVE1_POSITION || to == ITEM_GLOVE2_POSITION)
							{
								if (!ItemDef::IsGlove(id->mEquipType))
								{
									to = 0;
								}
							}
							else if (to == ITEM_RING1_POSITION || to == ITEM_RING2_POSITION)
							{
								if (!ItemDef::IsRing(id->mEquipType))
								{
									to = 0;
								}
							}
							else
							{
								to = 0;
							}
							bag_use_item_equipment(pos, to);
						}
						break;
					}
				}
			}
		}
		else
		{
			return 1;
		}
		return 0;
	}
	int DomainPlay::UndressItem(int position)
	{
		if( position >= ITEM_BAG_BEGIN )
		{
			if(position<ITEM_DEPOT_BEGIN||position>ITEM_DEPOT_END+mDepotSlotAdd)
			{
				if(position<ITEM_LOTTERYDEPOT_BEGIN||position>ITEM_LOTTERYDEPOT_END)
				{
					return 1;
				}
			}
		}
		if(position == ITEM_XUESHI_POSITION){mEquipMoXueShi = false;}
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == position )
			{
				int new_pos = find_empty_item_position();
				int new_pos_fashion = find_empty_item_position_fashion();
				if (pos->mPosition == ITEM_FASHION_WEAPON_POSITION || pos->mPosition == ITEM_FASHION_CLOTH_POSITION || pos->mPosition == ITEM_FASHION_WING_POSITION)
				{
					new_pos = new_pos_fashion;
				}
				if( new_pos >= 0 )
				{
					SubItem it;
					it.mPosition = pos->mPosition;
					ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mGSL->GetScriptEngine()->ItemPlayer(this,id->mName.c_str(),id->mTypeID,"player.undressEquipItem");
					}
					it.mTypeID = 0;
					mNeedClearItemList = true;
					mSession->GetListener()->notifyItemChange(mSession,it,0);
					pos->mPosition = new_pos;
					mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					UpdateAvatar();
					for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
						pos != mVisibleInteractiveObjectList.end();
						++ pos )
					{
						if ((*pos))
						{
							(*pos)->NotifyAvatarChange(this);
							if ((*pos)->TypeIsPlayer())
							{
								DomainPlay* play = static_cast<DomainPlay*>(*pos);
								play->NotifyPlayerAddinfo(this);
							}
						}
					}
					NotifyAvatarChange(this);
					CalculateAttribute();
					mSession->GetListener()->notifyAttributeChange(mSession);
					mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
				}
				break;
			}
		}
		return 0;
	}
	int DomainPlay::BagUseGem(int equip_pos, int gem_pos)
	{
		if( gem_pos < ITEM_BAG_BEGIN || gem_pos >= ITEM_BAG_END+mBagSlotAdd ) return 1;  
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == gem_pos)
			{
				int check = bag_use_item_equipment_check_limit(pos);
				if( 0 != check )
				{
					if( 5 == check )
					{
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_ZHUANSHENG_LEVEL_LIMIT);
						return 0;
					}
					if( 6 == check )
					{
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_ITEM_USE_GENDER_ERROR);
						return 0;
					}
					if( 7 == check )
					{
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_ITEM_USE_JOB_ERROR);
						return 0;
					}
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_USE_TIEM_LIMIT_ERROR);
					return 0;
				}
				if( ItemDef::IsGem(pos->mSubType) )
				{
					return bag_use_item_gem(pos, equip_pos);
					break;
				}
			}
		}
		return 0;
	}
	int DomainPlay::UndressGem(int position)
	{
		if( position >= ITEM_BAG_BEGIN )
		{
			return 0;
		}
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == position )
			{
				int new_pos = find_empty_item_position();
				if( new_pos >= 0 )
				{
					SubItem it;
					it.mPosition = pos->mPosition;
					ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mGSL->GetScriptEngine()->ItemPlayer(this,id->mName.c_str(),id->mTypeID,"player.undressGemItem");
					}
					it.mTypeID = 0;
					mNeedClearItemList = true;
					//卸下镶嵌宝石
					mSession->GetListener()->notifyItemChange(mSession,it,0);
					//放入宝石背包
					pos->mPosition = new_pos;
					mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					//UpdateAvatar();
					for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
						pos != mVisibleInteractiveObjectList.end();
						++ pos )
					{
						if ((*pos))
						{
							//(*pos)->NotifyAvatarChange(this);
							if ((*pos)->TypeIsPlayer())
							{
								DomainPlay* play = static_cast<DomainPlay*>(*pos);
								play->NotifyPlayerAddinfo(this);
							}
						}
					}
					//NotifyAvatarChange(this);
					CalculateAttribute();
					mSession->GetListener()->notifyAttributeChange(mSession);
					mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
					return 1;
				}
				return -1;
				break;
			}
		}
		return 0;
	}
	void DomainPlay::CalculateBaseAttribute()
	{
		CalculateBaseAttribute3();

		mBaseAttribute.mAccuracy = 15;
		mBaseAttribute.mDodge = 10;
		m_nFightPoint = 0;
	}
	void DomainPlay::WarriorCalculateBaseAttribute()
	{

		mBaseAttribute.mMaxHp = 14 + Round((level()*(level() / 4.0 + 4.5 + level()/20.0)));//*0.72
		mBaseAttribute.mMaxMp = 11 + Round(level()*3.5);
		mBaseAttribute.mMaxBurden = 50 + Round(level() * (level() / 3.0));
		mBaseAttribute.mMaxLoad = 15 + Round(level() * (level() / 20.0));
		mBaseAttribute.mMaxBrawn = 12 + Round(level() * (level() / 13.0));

		
		int n = level() / 7;
		int n1 = level() / 5;
		mBaseAttribute.mDC = max(n1-1,1);
		mBaseAttribute.mDCMax = max(1,n1);
		mBaseAttribute.mMC = 0;
		mBaseAttribute.mMCMax = 0;
		mBaseAttribute.mSC = 0;
		mBaseAttribute.mSCMax = 0;

		mBaseAttribute.mAC = 0;
		mBaseAttribute.mACMax = n;
		mBaseAttribute.mMAC = 0;
		mBaseAttribute.mMACMax = 0;
		mBaseAttribute.mHolyDam = 0;
	}
	void DomainPlay::WizardCalculateBaseAttribute()
	{
		mBaseAttribute.mMaxHp = 14 + Round(level()*(level() / 15.0 + 1.8));
		mBaseAttribute.mMaxMp = 13 + Round(level()*(level() / 5.0 +2.0)*2.2);

		mBaseAttribute.mMaxBurden = 50 + Round(level() * (level() / 5.0));
		mBaseAttribute.mMaxLoad = 15 + Round(level() * (level() / 100.0));
		mBaseAttribute.mMaxBrawn = 12 + Round(level() * (level() / 90.0));

		int n = level() / 7;
		mBaseAttribute.mDC = max(n-1,0);
		mBaseAttribute.mDCMax = max(1,n);
		mBaseAttribute.mMC = max(n-1,0);
		mBaseAttribute.mMCMax = max(1,n);
		mBaseAttribute.mSC = 0;
		mBaseAttribute.mSCMax = 0;

		mBaseAttribute.mAC = 0;
		mBaseAttribute.mACMax = 0;
		mBaseAttribute.mMAC = 0;
		mBaseAttribute.mMACMax = 0;
		mBaseAttribute.mHolyDam = 0;
	}
	void DomainPlay::TaoistCalculateBaseAttribute()
	{
		mBaseAttribute.mMaxHp = 14 + Round((level()*(level() / 6.0 + 2.5)));//*0.89
		mBaseAttribute.mMaxMp = 13 + Round(level()*(level() / 8.0)*2.2);

		mBaseAttribute.mMaxBurden = 50 + Round(level() * (level() / 4.0));
		mBaseAttribute.mMaxLoad = 15 + Round(level() * (level() / 50.0));
		mBaseAttribute.mMaxBrawn = 12 + Round(level() * (level() / 42.0));

		int n = level() / 7;
		mBaseAttribute.mDC = max(n-1,0);
		mBaseAttribute.mDCMax = max(1,n);
		mBaseAttribute.mMC = 0;
		mBaseAttribute.mMCMax = 0;
		mBaseAttribute.mSC = max(n-1,0);
		mBaseAttribute.mSCMax = max(1,n);

		n = Round(level()/6.0);
		mBaseAttribute.mAC = 0;
		mBaseAttribute.mACMax = 0;
		mBaseAttribute.mMAC = n/2;
		mBaseAttribute.mMACMax = n+1;
		mBaseAttribute.mHolyDam = 0;
	}

	void DomainPlay::CalculateBaseAttribute3()
	{
		int id = job()*1000 + level();
		lv_attr* la = mGSL->GetLevelInfoManager()->GetLevelAttr(id);if(! la){;return;}

		mBaseAttribute.mMaxHp = la->mMaxHp;
		mBaseAttribute.mMaxMp = la->mMaxMp;

		mBaseAttribute.mMaxBurden = 50 + Round(level() * (level() / 3.0));
		mBaseAttribute.mMaxLoad = 15 + Round(level() * (level() / 20.0));;
		mBaseAttribute.mMaxBrawn = 12 + Round(level() * (level() / 13.0));

		mBaseAttribute.mDC = la->mDC;
		mBaseAttribute.mDCMax = la->mDCMax;
		mBaseAttribute.mMC = la->mMC;
		mBaseAttribute.mMCMax = la->mMCMax;
		mBaseAttribute.mSC = la->mSC;
		mBaseAttribute.mSCMax = la->mSCMax;

		mBaseAttribute.mAC = la->mAC;
		mBaseAttribute.mACMax = la->mACMax;
		mBaseAttribute.mMAC = la->mMAC;
		mBaseAttribute.mMACMax = la->mMACMax;
		mBaseAttribute.mHolyDam = 0;
		mBaseAttribute.mAccuracy = la->mAccuracy;
		mBaseAttribute.mDodge = la->mDodge;
	}

	void DomainPlay::CalculateAttribute()
	{
		//清理buffpropercent
		ClearBuffPropPercent();
		CalculateBaseAttribute();
		if( mGSL->mBaseMul > 0 )
		{
			mBaseAttribute.mMaxHp = Round(mBaseAttribute.mMaxHp*mGSL->mBaseHPMul);
			mBaseAttribute.mMaxMp = Round(mBaseAttribute.mMaxMp*mGSL->mBaseMPMul);
			mBaseAttribute.mAC = Round(mBaseAttribute.mAC*mGSL->mBaseACMul);
			mBaseAttribute.mACMax = Round(mBaseAttribute.mACMax*mGSL->mBaseACMul);
			mBaseAttribute.mMAC = Round(mBaseAttribute.mMAC*mGSL->mBaseMACMul);
			mBaseAttribute.mMACMax = Round(mBaseAttribute.mMACMax*mGSL->mBaseMACMul);
			mBaseAttribute.mDC = Round(mBaseAttribute.mDC*mGSL->mBaseDCMul);
			mBaseAttribute.mDCMax = Round(mBaseAttribute.mDCMax*mGSL->mBaseDCMul);
			mBaseAttribute.mMC = Round(mBaseAttribute.mMC*mGSL->mBaseMCMul);
			mBaseAttribute.mMCMax = Round(mBaseAttribute.mMCMax*mGSL->mBaseMCMul);
			mBaseAttribute.mSC = Round(mBaseAttribute.mSC*mGSL->mBaseSCMul);
			mBaseAttribute.mSCMax = Round(mBaseAttribute.mSCMax*mGSL->mBaseSCMul); 
		}
		int hp = Hp();
		int mp = Mp();
		float hpPercent = 1.0f * Hp() / MaxHp();
		PlayActiveObject::CalculateAttribute();
		CalculateItemAttribute();
		CalculateSkillAttribute();
		CalculateSwordAttribute();
		CalculateMonPointAttribute();
		//CalculateTitleAttribute();
		//CalculateReinAttribute();
		CalculateScriptAttribute();
		//CalculateTotalUpdAttribute();
		CalculateUpGradeAttribute();//强化系统
		//CalculateGemAttribute();//宝石系统
		//CalculateMonsterAttribute();
		CalculateStatusAttribute();
		CalculateBuffAttribute();
		CalculateExtraObjFuncAttribute();
		CalculatePropPercent();
		mHp = hp;
		mMp = mp;
		
		if ( mPower > mActiveAttribute.mMaxPower) mPower = mActiveAttribute.mMaxPower;

		if (mActiveAttribute.mPKIncrement > 10000)
			mActiveAttribute.mPKIncrement  = 10000;
		if (mActiveAttribute.mPKReduce > 10000)
			mActiveAttribute.mPKReduce = 10000;
		if (mActiveAttribute.mDamIncrement > 10000)
			mActiveAttribute.mDamIncrement = 10000;
		if (mActiveAttribute.mDamReduce > 10000)
			mActiveAttribute.mDamReduce = 10000;
		if (mActiveAttribute.mDamReduceDC > 10000)
			mActiveAttribute.mDamReduceDC = 10000;
		if (mActiveAttribute.mDamReduceMC > 10000)
			mActiveAttribute.mDamReduceMC = 10000;
		if (mActiveAttribute.mPKReduceDC > 10000)
			mActiveAttribute.mPKReduceDC = 10000;
		if (mActiveAttribute.mPKReduceMC > 10000)
			mActiveAttribute.mPKReduceMC = 10000;
		if (mActiveAttribute.mMonDamIncrement > 10000)
			mActiveAttribute.mMonDamIncrement = 10000;
		if (mActiveAttribute.mZSMonDamIncrement > 10000)
			mActiveAttribute.mZSMonDamIncrement = 10000;
		//if (mHpPro)
		{
			Hp(MaxHp() * hpPercent);
		//	mHpPro = false;
		}
		if (mHp > mActiveAttribute.mMaxHp) mHp = mActiveAttribute.mMaxHp;
		if (mMp > mActiveAttribute.mMaxMp) mMp = mActiveAttribute.mMaxMp;
		CalculateFightPoint();
	}
	void DomainPlay::ClearBuffPropPercent()
	{
		mPropPercent.mMaxMp = 0;
		mPropPercent.mMaxHp = 0;
		mPropPercent.mAC = 0;
		mPropPercent.mACMax = 0;

		mPropPercent.mMAC = 0;
		mPropPercent.mMACMax = 0;

		mPropPercent.mDC = 0;
		mPropPercent.mDCMax = 0;

		mPropPercent.mMC = 0;
		mPropPercent.mMCMax = 0;

		mPropPercent.mSC = 0;
		mPropPercent.mSCMax = 0;
		mPropPercent.mAccuracy = 0;
		mPropPercent.mDodge = 0;
		mPropPercent.mLuck = 0;
	}
	void DomainPlay::CalculateExtraObjFuncAttribute()
	{
		for (size_t i = 0; i < m_extraObjFuncs.size(); ++ i)
		{
			FuncInfo * pFunc = mGSL->GetObjFuncManager()->GetFuncInfo(m_extraObjFuncs[i]);
			if (!pFunc)
				continue;

			if (pFunc->m_nCalAttr == 1)
			{
				for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
				{
					if (pFunc->m_propFuncs[i].mFunc)
						pFunc->m_propFuncs[i].mFunc->onHandle(this);
				}
			}
		}
	}

	void DomainPlay::CalculateFightPoint()
	{
		// 计算战斗力
		m_nFightPoint = 0;
		double temp = 0;
		if (job() == JOB_WARRIOR)
		{
			temp +=mActiveAttribute.mMaxPower * 1.0 * 900 / 10000;
			temp += mActiveAttribute.mMaxHp * 1.0 * 1800 / 10000;
			temp += mActiveAttribute.mDC * 1.0 * 32000 / 10000;
			temp += mActiveAttribute.mDCMax * 1.0 * 16000 / 10000;
			temp += mActiveAttribute.mAC * 1.0 * 7000 / 10000;
			temp += mActiveAttribute.mACMax * 1.0 * 5000 / 10000;
			temp += mActiveAttribute.mMAC * 1.0 * 7000 / 10000;
			temp += mActiveAttribute.mMACMax * 1.0 * 5000 / 10000;
			temp += mActiveAttribute.mBaojiProb * 1.0 * 60000 / 10000;
			temp += mActiveAttribute.mBaojiPres* 1.0 * 10000 / 10000;
			/*temp += mActiveAttribute.mAccuracy * 1.0 * 40000 / 10000;
			temp += mActiveAttribute.mDodge * 1.0 * 40000 / 10000;*/
			temp += mActiveAttribute.mLuck* 1.0 * 4000000 / 10000;
			temp += mActiveAttribute.mTenacity* 1.0 * 30000 / 10000;
			temp += mActiveAttribute.mHolyDam * 1.0 * 30000 / 10000;
			//神圣防御
			//temp += mActiveAttribute.mHolyDam * 1.0 * 30000 / 10000; 
			temp += mActiveAttribute.mDamReduceDC * 1.0 * 20000 / 10000;
			temp += mActiveAttribute.mDamReduceMC * 1.0 * 20000 / 10000;
		}
		else if (job() == JOB_WIZARD)
		{
			temp +=mActiveAttribute.mMaxPower * 1.0 * 2400 / 10000;
			temp += mActiveAttribute.mMaxHp * 1.0 * 4800 / 10000;
			temp += mActiveAttribute.mMC * 1.0 * 32000 / 10000;
			temp += mActiveAttribute.mMCMax * 1.0 * 16000 / 10000;
			temp += mActiveAttribute.mAC * 1.0 * 7000 / 10000;
			temp += mActiveAttribute.mACMax * 1.0 * 5000 / 10000;
			temp += mActiveAttribute.mMAC * 1.0 * 7000 / 10000;
			temp += mActiveAttribute.mMACMax * 1.0 * 5000 / 10000;
			temp += mActiveAttribute.mBaojiProb * 1.0 * 60000 / 10000;
			temp += mActiveAttribute.mBaojiPres* 1.0 * 10000 / 10000;
			/*temp += mActiveAttribute.mAccuracy * 1.0 * 40000 / 10000;
			temp += mActiveAttribute.mDodge * 1.0 * 40000 / 10000;*/
			temp += mActiveAttribute.mLuck* 1.0 * 4000000 / 10000;
			temp += mActiveAttribute.mTenacity* 1.0 * 30000 / 10000;
			temp += mActiveAttribute.mHolyDam * 1.0 * 30000 / 10000;
			//神圣防御
			//temp += mActiveAttribute.mHolyDam * 1.0 * 30000 / 10000; 
			temp += mActiveAttribute.mDamReduceDC * 1.0 * 20000 / 10000;
			temp += mActiveAttribute.mDamReduceMC * 1.0 * 20000 / 10000;
		}
		else if (job() == JOB_TAOIST)
		{
			temp +=mActiveAttribute.mMaxPower * 1.0 * 1500 / 10000;
			temp += mActiveAttribute.mMaxHp * 1.0 * 3000 / 10000;
			temp += mActiveAttribute.mSC * 1.0 * 32000 / 10000;
			temp += mActiveAttribute.mSCMax * 1.0 * 16000 / 10000;
			temp += mActiveAttribute.mAC * 1.0 * 7000 / 10000;
			temp += mActiveAttribute.mACMax * 1.0 * 5000 / 10000;
			temp += mActiveAttribute.mMAC * 1.0 * 7000 / 10000;
			temp += mActiveAttribute.mMACMax * 1.0 * 5000 / 10000;
			temp += mActiveAttribute.mBaojiProb * 1.0 * 60000 / 10000;
			temp += mActiveAttribute.mBaojiPres* 1.0 * 10000 / 10000;
			/*temp += mActiveAttribute.mAccuracy * 1.0 * 40000 / 10000;
			temp += mActiveAttribute.mDodge * 1.0 * 40000 / 10000;*/
			temp += mActiveAttribute.mLuck* 1.0 * 4000000 / 10000;
			temp += mActiveAttribute.mTenacity* 1.0 * 30000 / 10000;
			temp += mActiveAttribute.mHolyDam * 1.0 * 30000 / 10000;
			//神圣防御
			//temp += mActiveAttribute.mHolyDam * 1.0 * 30000 / 10000; 
			temp += mActiveAttribute.mDamReduceDC * 1.0 * 20000 / 10000;
			temp += mActiveAttribute.mDamReduceMC * 1.0 * 20000 / 10000;
		}
		m_nFightPoint = ceil(temp);

		mGSL->GetScriptEngine()->PlayerInt(this, m_nFightPoint, 0, 0, 0, "player.onCalculateFight");

		if(mSession && mSession->GetDomainGuild())
			mSession->GetDomainGuild()->SetGuildFight();
		mGSL->GetChartManager()->UpdateData(mJob + 6, Name(), FightPoint(), GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender()/*,mSession->TXnetInfo()*/);
		mGSL->GetChartManager()->UpdateData(CHART_TYPE_FIGHTPOINT_ALL, Name(), FightPoint(), mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender()/*,mSession->TXnetInfo()*/);
		
	}

	void DomainPlay::CalculateTotalUpdAttribute()
	{
		if(mTotalUpdLevel < mGSL->mTotalAttrLevelLimit){return;}
		int id = mJob*100 + mTotalUpdLevel;
		TotalAttrInfo* tai = mGSL->GetTotalInfoManager()->GetTotalAttrInfo(id);if(! tai){return;}
		Attributes()->mDC += tai->mDC;
		Attributes()->mDCMax += tai->mDCMax;
		Attributes()->mMC += tai->mMC;
		Attributes()->mMCMax += tai->mMCMax;
		Attributes()->mSC += tai->mSC;
		Attributes()->mSCMax += tai->mSCMax;
		Attributes()->mAC += tai->mAC;
		Attributes()->mACMax += tai->mACMax;
		Attributes()->mMAC += tai->mMAC;
		Attributes()->mMACMax += tai->mMACMax;
		Attributes()->mMaxHp += tai->mHP;
		Attributes()->mMaxMp += tai->mMP;
		Attributes()->mAccuracy += tai->mAccuracy;
		Attributes()->mDodge += tai->mDodge;
		Attributes()->mLuck  += tai->mLuck;
		Attributes()->mDropProb += tai->mDropProb;
		Attributes()->mDoubleAttProb += tai->mDoubleAttProb;
	}
	void DomainPlay::CalculateScriptAttribute()
	{
		Attributes()->mMaxMp+=			mSAP.mMaxMp;
		Attributes()->mMaxHp+=			mSAP.mMaxHp;	
		Attributes()->mMaxMp+=			int(mBaseAttribute.mMaxMp*(1.0f*mSAP.mMpPres/10000));
		Attributes()->mMaxHp+=			int(mBaseAttribute.mMaxHp*(1.0f*mSAP.mHpPres/10000));	
		Attributes()->mAC+=				mSAP.mAC;
		Attributes()->mACMax+=			mSAP.mACMax;
		Attributes()->mMAC+=			mSAP.mMAC;
		Attributes()->mMACMax+=			mSAP.mMACMax;
		Attributes()->mDC+=				mSAP.mDC;
		Attributes()->mDCMax+=			mSAP.mDCMax;
		Attributes()->mMC+=				mSAP.mMC;
		Attributes()->mMCMax+=			mSAP.mMCMax;
		Attributes()->mSC+=				mSAP.mSC;
		Attributes()->mSCMax+=			mSAP.mSCMax;
		Attributes()->mAccuracy+=		mSAP.mAccuracy;
		Attributes()->mDodge+=			mSAP.mDodge;
		Attributes()->mLuck+=			mSAP.mLuck;
		Attributes()->mAntiMagic+=		mSAP.mAntiMagic;
		Attributes()->mAntiPoison+=		mSAP.mAntiPoison;
		Attributes()->mHpRecover+=		mSAP.mHpRecover;
		Attributes()->mMpRecover+=		mSAP.mMpRecover;
		Attributes()->mPoisonRecover+=	mSAP.mPoisonRecover;
		Attributes()->mMabi_prob+=		mSAP.mMabi_prob;
		Attributes()->mMabi_dura+=		mSAP.mMabi_dura;
		Attributes()->mDixiao_pres+=	mSAP.mDixiao_pres;
		if(Attributes()->mFuyuan_cd <= 0 )
		{
			Attributes()->mFuyuan_cd = mSAP.mFuyuan_cd;
		}
		else
		{
			if(mSAP.mFuyuan_cd>0)
			{
				Attributes()->mFuyuan_cd =	min(Attributes()->mFuyuan_cd,mSAP.mFuyuan_cd);
			}
		}
		Attributes()->mDropProb+=		mSAP.mDropProb;
		Attributes()->mDoubleAttProb+=	mSAP.mDoubleAttProb;
		Attributes()->mXishouProb+=		mSAP.mXishouProb;
		Attributes()->mXishouPres+=		mSAP.mXishouPres;
		Attributes()->mFantanProb+=		mSAP.mFantanProb;
		Attributes()->mFantanPres+=		mSAP.mFantanPres;
		Attributes()->mBaojiProb+=		mSAP.mBaojiProb;
		Attributes()->mBaojiPres+=		mSAP.mBaojiPres;
		Attributes()->mXixueProb+=		mSAP.mXixueProb;
		Attributes()->mXixuePres+=		mSAP.mXixuePres;
		Attributes()->mPKIncrement+=	mSAP.mPKIncrement;
		Attributes()->mPKReduce+=		mSAP.mPKReduce;
		Attributes()->mHolyDam+=		mSAP.mHolyDam;
		Attributes()->mDamIncrement+=	mSAP.mDamIncrement;
		Attributes()->mDamReduce+=		mSAP.mDamReduce;
		Attributes()->mPKReduceDC+=		mSAP.mPKReduceDC;
		Attributes()->mPKReduceMC+=		mSAP.mPKReduceMC;
		Attributes()->mTenacity+=		mSAP.mTenacity;
	}

	void DomainPlay::CalculateSkillAttribute()
	{
		for( SkillMap::iterator pos = GetSkillList().begin();
			pos != GetSkillList().end();
			++ pos )
		{
			if( pos->mTypeID == SkillDef::SKILL_TYPE_JiChuJianShu )
			{
				Attributes()->mAccuracy += Round(3.0*pos->mLevel);
			}
			if( pos->mTypeID == SkillDef::SKILL_TYPE_JinShenLiZhanFa )
			{
				Attributes()->mAccuracy += Round(8.0/3.0*pos->mLevel);
			}
		}
	}
	void DomainPlay::CalculateSwordAttribute()
	{
		
	}
	void DomainPlay::CalculateMonPointAttribute()
	{
		if(mMonPointLv <= 0){return;}
		if(mMonPointLv>=96) mMonPointLv=96;
		int lv =1;
		while(lv <= mMonPointLv)
		{
			MonPointDef* sd = mGSL->GetMonPointDefManager()->GetMonPointDefine(100000+lv);
			if( sd ){
				Attributes()->mDC += sd->dc_min;
				Attributes()->mDCMax += sd->dc_max;
				Attributes()->mMC += sd->mc_min;
				Attributes()->mMCMax += sd->mc_max;
				Attributes()->mSC += sd->sc_min;
				Attributes()->mSCMax += sd->sc_max;
				Attributes()->mAC += sd->ac_min;
				Attributes()->mACMax += sd->ac_max;
				Attributes()->mMAC += sd->mac_min;
				Attributes()->mMACMax += sd->mac_max;
				Attributes()->mMaxHp += sd->max_hp;
				Attributes()->mMaxMp += sd->max_mp;
			}
			lv++;
		}
	}
	void DomainPlay::CalculateTitleAttribute()
	{
		int arry[10][2] = { { 8301, 230 }, { 8302, 240 }, { 8303, 250 }, { 8304, 260 }, { 8305, 270 }, { 8306, 280 }, { 8307, 290 }, { 8308, 300 }, { 8309, 310 }, { 8310, 320 } };
		int arry2[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0 };
		for (int i = 0; i < 10; i++)
		{
			int lv = 0;
			int lv_basic = 1;
			if (Param(arry[i][0]).length())
			{
				lv = atoi(Param(arry[i][0]).c_str());
			}
			while (lv_basic <= lv)
			{
				MonPointDef* sd = mGSL->GetMonPointDefManager()->GetMonPointDefine(arry[i][1] * 1000 + lv_basic);
				if (sd)
				{
					arry2[0] += sd->dc_min;
					arry2[1] += sd->dc_max;
					arry2[2] += sd->mc_min;
					arry2[3] += sd->mc_max;
					arry2[4] += sd->sc_min;
					arry2[5] += sd->sc_max;
					arry2[6] += sd->ac_min;
					arry2[7] += sd->ac_max;
					arry2[8] += sd->mac_min;
					arry2[9] += sd->mac_max;
					arry2[10] += sd->max_hp;
					arry2[11] += sd->max_mp;
				}
				lv_basic++;
			}
		}
		{
			Attributes()->mDC += arry2[0];
			Attributes()->mDCMax += arry2[1];
			Attributes()->mMC += arry2[2];
			Attributes()->mMCMax += arry2[3];
			Attributes()->mSC += arry2[4];
			Attributes()->mSCMax += arry2[5];
			Attributes()->mAC += arry2[6];
			Attributes()->mACMax += arry2[7];
			Attributes()->mMAC += arry2[8];
			Attributes()->mMACMax += arry2[9];
			Attributes()->mMaxHp += arry2[10];
			Attributes()->mMaxMp += arry2[11];
		}
	}
	void DomainPlay::CalculateUpGradeAttribute()
	{

		std::map<int, int>equipmap;
		int data1[18] = { 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270 };
		int data2[18] = { -2, -4, -6, -8, -10, -11, -12, -13, -14, -16 ,-22, -24, -26,-28, -30, -32, -34,-36 };
		for (int i = 0; i < 18; i++)
		{
			equipmap[data2[i]] = data1[i];
		}
		for (int j = 0; j<18; j++)
		{
			for (ItemMap::iterator pos = GetItemList().begin();
				pos != GetItemList().end();
				++pos)
			{
				if (pos->mTypeID > 0 && pos->mPosition == data2[j])
				{
					if (pos->mLevel >= 1 && pos->mLevel <= 20)
					{
						MonPointDef* sd = mGSL->GetMonPointDefManager()->GetMonPointDefine(equipmap[pos->mPosition] * 1000 + pos->mLevel);
						if (sd)
						{
							Attributes()->mDC += sd->dc_min;
							Attributes()->mDCMax += sd->dc_max;
							Attributes()->mMC += sd->mc_min;
							Attributes()->mMCMax += sd->mc_max;
							Attributes()->mSC += sd->sc_min;
							Attributes()->mSCMax += sd->sc_max;
							Attributes()->mAC += sd->ac_min;
							Attributes()->mACMax += sd->ac_max;
							Attributes()->mMAC += sd->mac_min;
							Attributes()->mMACMax += sd->mac_max;
							Attributes()->mMaxHp += sd->max_hp;
							Attributes()->mMaxMp += sd->max_mp;
						}
					}
				}
			}
		}
	}
	std::vector<int> getNumbers(const std::string &s)
	{
		std::istringstream iss(s);
		std::vector<int> r;
		int i;
		char c;
		while (iss >> i) {
			r.push_back(i);
			iss >> c;
		}
		return r;
	}
	void DomainPlay::CalculateGemAttribute()
	{
		int arry[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int arry_const[10] = { 7660, 7661, 7662, 7663, 7664, 7665, 7666, 7667, 7668, 7669};
		int gem_num = 0;
		bool added = false;///是否需要最终增加属性
		int min_gem_lv = 0;///默认最小等级为0
		for (int i = 0; i < 10; i++)
		{
			std::string gem_str = "";
			if (Param(arry_const[i]).length())
			{
				gem_str = Param(arry_const[i]);
			}
			else
			{
				continue;
			}
			auto r = getNumbers(gem_str);
			
			if (r.size() <= 0 || r.size() > 4) continue;

			for (int j = 0; j < r.size(); j++)
			{
				if (r[j] == 0) continue;
				if (r[j] >= 17020 && r[j] <= 17031)///攻击宝石id
				{
					if (min_gem_lv == 0 || (r[j] - 17019 < min_gem_lv))
					{
						min_gem_lv = r[j] - 17019;
					}
					r[j] = r[j] - 17019 + 190000;	
				}
				else if (r[j] >= 17032 && r[j] <= 17043)///物理防御宝石id
				{
					if (min_gem_lv == 0 || (r[j] - 17031 < min_gem_lv))
					{
						min_gem_lv = r[j] - 17031;
					}
					r[j] = r[j] - 17031 + 200000;
				}
				else if (r[j] >= 17044 && r[j] <= 17055)///魔法防御宝石id
				{
					if (min_gem_lv == 0 || (r[j] - 17043 < min_gem_lv))
					{
						min_gem_lv = r[j] - 17043;
					}
					r[j] = r[j] - 17043 + 210000;
				}
				else if (r[j] >= 17056 && r[j] <= 17067)///坚韧宝石id
				{
					if (min_gem_lv == 0 || (r[j] - 17055 < min_gem_lv))
					{
						min_gem_lv = r[j] - 17055;
					}
					r[j] = r[j] - 17055 + 220000;
				}
				MonPointDef* sd = mGSL->GetMonPointDefManager()->GetMonPointDefine(r[j]);
				if (sd)
				{
					arry[0] += sd->dc_min;
					arry[1] += sd->dc_max;
					arry[2] += sd->mc_min;
					arry[3] += sd->mc_max;
					arry[4] += sd->sc_min;
					arry[5] += sd->sc_max;
					arry[6] += sd->ac_min;
					arry[7] += sd->ac_max;
					arry[8] += sd->mac_min;
					arry[9] += sd->mac_max;
					arry[10] += sd->max_hp;
					added = true;
					gem_num++;
				}
			}
		}
		if (gem_num == 40)////如果已经镶嵌了40个宝石之后将会触发额外属性加成
		{
			int chufa_min_lv = 0;////默认触发最低等级的宝石额外属性
			int gem_lv[6] = { 1, 4, 7, 9, 11, 12 };///最低等级宝石达到多少级触发
			int baoji_prob[6] = { 200, 300, 500, 1000, 1500, 2000 };///额外增加的暴击概率(万分之一)
			int baojili[6] = { 1000, 3000, 5000, 8000, 10000, 10000 };///暴击附加伤害(暴击力)
			for (int m = 5; m >= 0; m--)
			{
				if (min_gem_lv >= gem_lv[m])
				{
					chufa_min_lv = m;
					break;
				}
			}
			Attributes()->mBaojiProb += baoji_prob[chufa_min_lv];
			Attributes()->mBaojiliPercent += baojili[chufa_min_lv];
		}
		if (added)
		{
			Attributes()->mDC += arry[0];
			Attributes()->mDCMax += arry[1];
			Attributes()->mMC += arry[2];
			Attributes()->mMCMax += arry[3];
			Attributes()->mSC += arry[4];
			Attributes()->mSCMax += arry[5];
			Attributes()->mAC += arry[6];
			Attributes()->mACMax += arry[7];
			Attributes()->mMAC += arry[8];
			Attributes()->mMACMax += arry[9];
			Attributes()->mMaxHp += arry[10];
		}

	}
	void DomainPlay::CalculateMonsterAttribute()
	{
	}
	void DomainPlay::CalculateItemAttribute()
	{
		GameServerLib* gsl = mSession->GetGameServerLib();
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 )
			{
				ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
				if( id )
				{
					if( pos->mPosition == ITEM_XUESHI_POSITION ){continue;}
					if( pos->mPosition < 0 )
					{
						ScriptItemAttrParamMap::iterator siap = mSIAP.find(pos->mPosition);
						if( siap != mSIAP.end() )
						{
							pos->ApplyAttribute(this,id,&mActiveAttribute,&mBaseAttribute,&siap->second);
						}
						else
						{
							pos->ApplyAttribute(this,id,&mActiveAttribute,&mBaseAttribute,0);
						}
					}
					if( pos->mPosition >=0 && pos->mPosition < ITEM_BAG_SIZE + mBagSlotAdd )
					{
						Attributes()->mBurden += id->mWeight;
					}
					if( pos->mPosition == -4 )
					{
						Attributes()->mBrawn = id->mWeight;
					}
					if( pos->mPosition < -4 )
					{
						Attributes()->mLoad += id->mWeight;
					}
				}
			}
		}
	}
	void DomainPlay::UpdateAfterCalculateAttribute()
	{
		mHp = mActiveAttribute.mMaxHp;
		mMp = mActiveAttribute.mMaxMp;
	}
	
	void DomainPlay::AddDropProb(int prob)
	{
		if( prob < 0 ){return;}
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::SubDropProb(int prob)
	{
		if( prob < 0 /*|| prob >= ReincarnateAttributes()->mDropProb*/){return;}
		//ReincarnateAttributes()->mDropProb -= prob;
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::AddDoubleAttProb(int prob)
	{
		if( prob < 0 ){return;}
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::SubDoubleAttProb(int prob)
	{
		if( prob < 0 /*|| prob >= ReincarnateAttributes()->mDropProb*/){return;}
		//ReincarnateAttributes()->mDoubleAttProb -= prob;
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	int DomainPlay::GetDropProb()
	{
		return mActiveAttribute.mDropProb;
	}
	int DomainPlay::GetDoubleAttProb()
	{
		return mActiveAttribute.mDoubleAttProb;
	}
	void DomainPlay::ChangeJob(int new_job,int undress_item)
	{
		if( mJob == new_job )return;
		mJob = new_job;
		if( undress_item )
		{
			bool end_tag = false;
			int loop_tag = 0;
			while ( end_tag == false && loop_tag < 20 )
			{
				loop_tag ++;
				end_tag = true;
				for( ItemMap::iterator pos = GetItemList().begin();
					pos != GetItemList().end();
					++pos )
				{
					if( pos->mTypeID > 0 && pos->mPosition<0 )
					{
						UndressItem(pos->mPosition);
						end_tag = false;
						break;
					}
				}
			}
		}
		CalculateAttribute();
		mSession->GetListener()->notifyPlayerAddInfo(mSession,this);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if( (*pos)->TypeIsPlayer() )
			{
				DomainPlay* player = (DomainPlay*)(*pos);
				if( player && player->Session() )
				{
					player->Session()->GetListener()->notifyPlayerAddInfo(mSession,this);
				}
			}
		}
		UpdateAvatar();
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyAvatarChange(this);
			}
		}
		NotifyAvatarChange(this);
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::ChangeGender(int new_gender,int undress_item)
	{
		if( mGender == new_gender ) return;
		mGender = new_gender;
		if( undress_item )
		{
			bool end_tag = false;
			int loop_tag = 0;
			while ( end_tag == false && loop_tag < 20 )
			{
				loop_tag ++;
				end_tag = true;
				for( ItemMap::iterator pos = GetItemList().begin();
					pos != GetItemList().end();
					++pos )
				{
					if( pos->mTypeID > 0 && pos->mPosition<0 )
					{
						UndressItem(pos->mPosition);
						end_tag = false;
						break;
					}
				}
			}
		}
		CalculateAttribute();
		mSession->GetListener()->notifyPlayerAddInfo(mSession,this);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* player = (DomainPlay*)(*pos);
					if (player && player->Session())
					{
						player->Session()->GetListener()->notifyPlayerAddInfo(mSession, this);
					}
				}
			}
		}
		UpdateAvatar();
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyAvatarChange(this);
			}
		}
		NotifyAvatarChange(this);
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::Load()
	{
		mLevel = mCharacter->level;
		mExp = mCharacter->exp;
		mGameMoney = mCharacter->money;
		mGameMoneyBind = mCharacter->moneybind;
		mVcoin = mCharacter->vcoin;
		mVcoinGS = mCharacter->vcoings;
		mVcoinBind = mCharacter->vcoinbind;
		mVcoinAccu = mCharacter->vcoinaccu;
		mLastVcoinUsedDay = mCharacter->mLastVcoinUsedDay;
		mVcoinUsedByDay = mCharacter->mVcoinUsedByDay;
		mVcoinUsedByMonth = mCharacter->mVcoinUsedByMonth;
		mLastCongzhiMonth = mCharacter->mLastCongzhiMonth;
		mVcoinAccuByMonth = mCharacter->mVcoinAccuByMonth;
		mGuildName = mCharacter->mGuildName;
		mGuildPt = mCharacter->mGuildPt;
		mGuildSpt = mCharacter->mGuildSpt;
		mJob = mCharacter->job;
		mPKValue = mCharacter->pkvalue;
		mCreateTime = mCharacter->create_time;
		mLastLoginTime = mCharacter->last_login_time;
		mLastLoginIP = mCharacter->last_login_ip;
		mLastLogoutTime = mCharacter->last_logout_time;
		mTalkProhibited = mCharacter->talk_prohibited;
		mDepotSlotAdd = mCharacter->depot_slot_add;
		mBagSlotAdd = mCharacter->bag_slot_add;
		//mCoupleID = mCharacter->mCoupleID;
		//mWeddingdate = mCharacter->mWeddingdate;
		mHonor = mCharacter->mHonor;
		//mMarryTimes = mCharacter->mMarryTimes;
		strncpy_s(mSeedName,_countof(mSeedName),mCharacter->mSeedName,255);mSeedName[255]=0;

		mGender = mCharacter->gender;
		mLastSafeMap = mCharacter->safe_map;
		mLastSafeX = mCharacter->safe_x;
		mLastSafeY = mCharacter->safe_y;
		SetLevel(mLevel,false);
		PlayMap* map = mGSL->GetMapManager()->FindMap(mLastSafeMap);
		if( map )
		{
			mLastSafeAreaID = map->CheckMapSafeArea(mLastSafeX,mLastSafeY);
		}

		mTaskMap=mCharacter->mTaskMap;
		mFriendArray = mCharacter->mFriendArray;
		mAchieveMap = mCharacter->mAchieveMap;
		mAchievePoint = mCharacter->mAchievePoint;
		mAchieveGameMoneyMax = mCharacter->mAchieveGameMoneyMax;
		mOnlineTimeMax = mCharacter->mOnlineTimeMax;
		mNumParams = mCharacter->mNumParamMap;
		mParams = mCharacter->mParamMap;
		{
			for(TaskMap::iterator pos = mTaskMap.begin();
				pos != mTaskMap.end();
				++pos)
			{
				{
					mScriptEcho.clear();
					char ss[512];_snprintf_s(ss,_countof(ss),511,"task.task%d.onGetTaskName",pos->second.TaskID);ss[511]=0;
					mGSL->GetScriptEngine()->OnlyPlayer(this,ss);
					pos->second.TaskName=mScriptEcho;
					mScriptEcho="";
				}
				{
					mScriptEcho.clear();
					char ss[512];_snprintf_s(ss,_countof(ss),511,"task.task%d.onGetTaskShortDesp",pos->second.TaskID);ss[511]=0;
					mGSL->GetScriptEngine()->OnlyPlayer(this,ss);
					pos->second.TaskShortDesp=mScriptEcho;
					mScriptEcho="";
				}
			}
		}
		mItemList=mCharacter->mItemList;
		mChatTradeItemList=mCharacter->mChatTradeItemList;
		mSkillList=mCharacter->mSkillList;
		mMailMap=mCharacter->mMailList;

		PlayActiveObject::Load(mCharacter);
		{
			time_t t = time(0);
			char tmp[64]; int day = 0;int month = 0;int rec_month = 0;
			tm ttmm;localtime_s(&ttmm,&t);
			strftime(tmp,sizeof(tmp),"%Y%m%d",&ttmm); 
			day = atoi(tmp);
			strftime(tmp,sizeof(tmp),"%Y%m",&ttmm); 
			month = atoi(tmp);
			if (mLastVcoinUsedDay != day)
			{
				mVcoinUsedByDay = 0;
			}
			if (mLastCongzhiMonth != month)
			{
				mVcoinAccuByMonth = 0;
			}

			rec_month = mLastVcoinUsedDay / 100;
			if (rec_month != month)
			{
				mVcoinUsedByMonth = 0;
			}
		}
	}
	void DomainPlay::Save()
	{
		mCharacter->level = mLevel;
		mCharacter->exp = mExp;
		mCharacter->money = mGameMoney;
		mCharacter->moneybind = mGameMoneyBind;
		mCharacter->vcoin = mVcoin;
		mCharacter->vcoings = mVcoinGS;
		mCharacter->vcoinbind = mVcoinBind;
		mCharacter->vcoinaccu = mVcoinAccu;
		mCharacter->mLastVcoinUsedDay = mLastVcoinUsedDay;
		mCharacter->mVcoinUsedByDay = mVcoinUsedByDay;
		mCharacter->mVcoinUsedByMonth = mVcoinUsedByMonth;
		mCharacter->mLastCongzhiMonth = mLastCongzhiMonth;
		mCharacter->mVcoinAccuByMonth = mVcoinAccuByMonth;

		EmptyItemRemover eir;
		GetItemList().erase(std::remove_if(GetItemList().begin(),GetItemList().end(),eir),GetItemList().end());
		mCharacter->mItemList = mItemList;
		GetChatTradeItemList().erase(std::remove_if(GetChatTradeItemList().begin(),GetChatTradeItemList().end(),eir),GetChatTradeItemList().end());
		mCharacter->mChatTradeItemList = mChatTradeItemList;//////记录聊天交易上面的物品数据
		mCharacter->mTaskMap = mTaskMap;
		mCharacter->mFriendArray = mFriendArray;
		mCharacter->mAchieveMap = mAchieveMap;
		mCharacter->mSkillList = mSkillList;
		mCharacter->mParamMap = mParams;
		mCharacter->mNumParamMap = mNumParams;
		mCharacter->mMailList = mMailMap;

		mCharacter->mAchievePoint=mAchievePoint;
		mCharacter->mAchieveGameMoneyMax=mAchieveGameMoneyMax;
		mCharacter->mOnlineTimeMax=mOnlineTimeMax;
		
		mCharacter->pkvalue = mPKValue;

		mCharacter->mGuildPt=mGuildPt;
		mCharacter->mGuildSpt=mGuildSpt;

		strncpy_s(mCharacter->safe_map,_countof(mCharacter->safe_map),mLastSafeMap.c_str(),255);
		mCharacter->safe_map[255]=0;
		mCharacter->safe_x = mLastSafeX;
		mCharacter->safe_y = mLastSafeY;

		mCharacter->talk_prohibited = mTalkProhibited;

		mCharacter->bag_slot_add = mBagSlotAdd;
		mCharacter->depot_slot_add = mDepotSlotAdd;
		//mCharacter->mCoupleID = mCoupleID;
		//mCharacter->mWeddingdate = mWeddingdate;
		//mCharacter->mMarryTimes = mMarryTimes;
		mCharacter->mHonor = mHonor;
		mCharacter->mCloth = mClothIdOn;
		mCharacter->mWeapon = mWeaponIdOn;
		mCharacter->mFashionCloth = mFashionClothIdOn;
		mCharacter->mFashionWeapon = mFashionWeaponIdOn;
		mCharacter->mWing = mHair;

		PlayActiveObject::Save(mCharacter);
		//FreshChinaLimit();

		mCharacter->cur_hp = mHp;
		mCharacter->cur_mp = mMp;
		mCharacter->cur_power = mPower;
		mCharacter->mMainTaskState = mMainTaskState;
	}
	void DomainPlay::SetHonor(int honor)
	{
		mHonor = honor;
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::AddHonor(int add)
	{
		mHonor += add;
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::SubHonor(int sub)
	{
		mHonor -= sub;if(mHonor < 0){mHonor = 0;}
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::SaveExitMapPosition()
	{
		if( mParentMap )
		{
			if(mParentMap->FubenType() == OGSLib::MAP_TYPE::MAP_PERSONAL && mParentMap->AutoEnter() )
			{
				if(mParentMap->GetOwnerName().empty())
				{
					mParentMap->SetOwnerNmae(Name());
				}
				strncpy_s(mCharacter->exit_map,_countof(mCharacter->exit_map),mParentMap->GetRealID().c_str(),255);
			}
			else
			{
				strncpy_s(mCharacter->exit_map,_countof(mCharacter->exit_map),mParentMap->GetID().c_str(),255);
			}
			mCharacter->exit_map[255]=0;
			mCharacter->exit_x = X();
			mCharacter->exit_y = Y();
		}
		else
		{
			mCharacter->exit_map[0] = 0;
		}
	}

	void DomainPlay::SetMountInfo(int lock,int mount,int itemtypeid)
	{
		mMountLock=lock;
		mMount = mount;
		mMountTypeID = itemtypeid;
		if(mMount==0)
		{
			UpdateAvatar();
		}
	}
	void DomainPlay::UpdateAvatar()
	{
		mTotalUpdLevel = 12;
		int check_number = 0;
		if( ! mMountLock )
		{
			mMount = 0;
			mMountTypeID = 0;
		}
		if( ! mLockAction )
		{
			mCloth = mSession->GetGameServerLib()->mDefaultClothMan;
			mWeapon = -1;
			//mHair = DEFAULT_HAIR_MAN;
			if( gender() == GENDER_FEMALE )
			{
				mCloth = mSession->GetGameServerLib()->mDefaultClothWoman;
				//mHair = DEFAULT_HAIR_FAMALE;
			}
			mFashion = -1;
		}
		GameServerLib* gsl = mSession->GetGameServerLib();
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID <= 0 )continue;
			if( pos->mPosition == ITEM_XUESHI_POSITION && pos->mDuration > 0 )
			{
				mEquipMoXueShi = true;
			}
			if( ! mLockAction )
			{
				if( pos->mPosition == ITEM_WEAPON_POSITION )
				{
					ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mWeapon = id->mResMale;
						mWeaponIdOn = id->mTypeID;
						if( gender() == GENDER_FEMALE )
						{
							mWeapon = id->mResFemale;
						}
					}
				}
				if( pos->mPosition == ITEM_CLOTH_POSITION )
				{
					ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mCloth = id->mResMale;
						if (gender() == GENDER_FEMALE)
						{
							mCloth = id->mResFemale;
						}
						mClothIdOn = id->mTypeID;
					}
				}
				if (pos->mPosition == ITEM_WING_POSITION)
				{
					ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mHair = id->mResMale;
					}
				}
				if (pos->mPosition == ITEM_FASHION_WING_POSITION)
				{
					ItemDef* id = gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if (id)
					{
						mHair = id->mResMale;
					}
				}
				//if( mShowFashion && pos->mPosition == ITEM_FASHION_WEAPON_POSITION )
				if (pos->mPosition == ITEM_FASHION_WEAPON_POSITION)
				{
					ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mWeapon = id->mResMale;
						mFashionWeaponIdOn = id->mTypeID;
						if( gender() == GENDER_FEMALE )
						{
							mWeapon = id->mResFemale;
						}
					}
				}
				//if (mShowFashion==pos->mPosition && pos->mPosition >= EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN  && pos->mPosition <= EQUIP_TYPE_FASHION_CLOTH_POS_END)
				if (pos->mPosition == ITEM_FASHION_CLOTH_POSITION)
				//if (mShowFashion && pos->mPosition == ITEM_FASHION_CLOTH_POSITION)
				{
					ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mFashionClothIdOn = id->mTypeID;
						mFashion = id->mResMale;
						if( gender() == GENDER_FEMALE )
						{
							mFashion = id->mResFemale;
						}
					}
				}
			}
			if( ! mMountLock )
			{
				if( pos->mPosition == ITEM_MOUNT_POSITION )
				{
					ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						mMount = id->mResMale;
						mMountTypeID = id->mTypeID;
						if(mShowMount)
						{
							mUpdateSpeed=1.0f;
							if(mShowMount) mUpdateSpeed=mGSL->GetItemDefineManager()->GetItemSpeed(mMountTypeID);
							notifySpeed(this,mUpdateSpeed);
						}
					}
				}
			}
			if( pos->mPosition < 0 && pos->mPosition != ITEM_XUESHI_POSITION && pos->mPosition != ITEM_HUNQI_POSITION 
				&& pos->mPosition != ITEM_MEDAL_POSITION && pos->mPosition != ITEM_MOUNT_POSITION 
				&& pos->mPosition != ITEM_FASHION_WEAPON_POSITION && pos->mPosition != ITEM_FASHION_CLOTH_POSITION
				&& pos->mPosition != ITEM_XUEFU_POSITION && pos->mPosition != ITEM_HUDUN_POSITION
				&& pos->mPosition != ITEM_BAOSHI_POSITION && pos->mPosition != ITEM_HUNZHU_POSITION
				)
			{
				check_number ++;
				if( mTotalUpdLevel > pos->mLevel )
				{
					mTotalUpdLevel = pos->mLevel;
				}
			}
		}
		if( check_number < 10 ){mTotalUpdLevel = 0;}
		if (mFashion > 0){mWeapon = mCloth = -1;}
	}
	void DomainPlay::UpdateChartData(int chart_type,int param)
	{
		mGSL->GetChartManager()->UpdateData(chart_type, Name(), param, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(),gender());
	}
	bool DomainPlay::changeShowFashion(int show)
	{
		if(show == 0 || show == 1)
		{
			if((mShowFashion && 1==show) || (!mShowFashion && 0==show)) return mShowFashion;
			mShowFashion = !mShowFashion;
			UpdateAvatar();
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyAvatarChange(this);
				}
			}
			NotifyAvatarChange(this);
			CalculateAttribute();
			mSession->GetListener()->notifyAttributeChange(mSession);
			//mSession->GetListener()->notifyShowFashion(mSession,show);
		}
		return 0;
	}
	void DomainPlay::bag_use_item_dissipative(ItemMap::iterator& pos,int num)
	{
		if( IsDead() )
		{
			return;
		}
		if( mParentMap && mParentMap->IsItemDisabled(pos->mTypeID) )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BAG_USE_ITEM_MAP_DISABLED);
			return ;
		}
		if( ItemDef::IsDissipative(pos->mSubType) )
		{
			if( mItemUseTick + 0.25 > mGSL->GetTimerFix()->Elapsed() )
			{
				//AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_ITEM_USE_TIME_TOO_SHORT);
				return;
			}
			mItemUseTick = mGSL->GetTimerFix()->Elapsed();
			GameServerLib* gsl = mParentMap->GetManager()->GetGSLib();
			ItemDef* id=gsl->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
			if( id && id->mCanUse > 0)
			{
				if (strlen(id->Script) != 0)
				{
					int position=pos->mPosition;
					int type_id=pos->mTypeID;
					mScriptEcho="";
					int usenum = num;
					if (usenum > pos->mNumber)
					{
						usenum = 1;
					}
					int ret = gsl->GetScriptEngine()->ItemPlayer(this,id->mName.c_str(),pos->mTypeID,id->Script,usenum,pos->mPosition);
					if( mScriptEcho.length() > 0 )
					{
						mItemTalkSeed = rand();
						mSession->GetListener()->notifyItemTalk(mSession,this,id->mTypeID,id->mName.c_str(),mScriptEcho,mItemTalkSeed);
					}
					mScriptEcho="";
					if (ret == 0){ return; }
					for( ItemMap::iterator pos = GetItemList().begin();
						pos != GetItemList().end();
						++ pos )
					{
						if( pos->mTypeID == type_id && pos->mPosition == position )
						{
							pos->mDuration-= ret;
							if(pos->mDuration <= 0)
							{
								pos->mNumber -= ret;
							}
							if( pos->mNumber <= 0 )
							{
								pos->mTypeID=0;
								mNeedClearItemList = true;
							}
							mSession->GetListener()->notifyItemChange(mSession,*pos,0);
							return;
						}
					}
				}
			}
		}
		mSession->GetListener()->notifyItemChange(mSession,*pos,0);
	}
	int DomainPlay::bag_use_item_equipment_check_limit(ItemMap::iterator& pos)
	{
		ItemDef* id = mSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
		if( id )
		{
			switch(id->mNeedType)
			{
			case 0:
				{
					/*if (id->mNeedParam == 0 && id->mNeedZsLevel >0)
					{
						if ( id -> mNeedZsLevel <= ZsLevel())
						{
							break;
						}
						else
						{
							return 5;
						}
					}*/
					if ( id -> mNeedZsLevel > 0)
					{
						if( id -> mNeedZsLevel <= ZsLevel() && id->mNeedParam <= level())
						{
							break;
						}
						else
						{
							return 5;
						}
					}
					else
					{
						if(id->mNeedParam <= level())
						{
							break;
						}
						else
						{
							return 1;
						}
					}
				}
				break;
			case 1:
				if( id->mNeedParam > Attributes()->mDCMax )
				{	
					return 2;
				}
				break;
			case 2:
				if( id->mNeedParam > Attributes()->mMCMax )
				{	
					return 3;
				}
				break;
			case 3:
				if( id->mNeedParam > Attributes()->mSCMax )
				{
					return 4;
				}
				break;
			}
			if( ( ItemDef::IsFashionCloth(pos->mTypeID ) || ItemDef::IsCloth(id->mEquipType) ) && id->mResMale > 0 )
			{
				/*if( gender() == GENDER_FEMALE && !( id->mResFemale > 0 && id->mResFemale<2000 ) )
				{
					return 6;
				}
				if( gender() == GENDER_MALE && !( id->mResMale >= 2000 && id->mResMale<4000 ) )
				{
					return 6;
				}*/
				if (id->mGender != 0 && id->mGender != gender())
				{
					return 6;
				}
			}
			if (id->mJob != job() && id->mJob != 0)
			{
				return 7;
			}
		}
		return 0;
	}
	int DomainPlay::bag_use_item_equipment_check_limit(ItemDef* id)
	{
		switch(id->mNeedType)
		{
		case 0:
			{
				if ( id -> mNeedZsLevel > 0)
				{
					if( id -> mNeedZsLevel <= ZsLevel() && id->mNeedParam <= level())
					{
						break;
					}
					else
					{
						return 5;
					}
				}
				else
				{
					if(id->mNeedParam <= level())
					{
						break;
					}
					else
					{
						return 1;
					}
				}
			}
			break;
		case 1:
			if( id->mNeedParam > Attributes()->mDCMax )
			{	
				return 2;
			}
			break;
		case 2:
			if( id->mNeedParam > Attributes()->mMCMax )
			{	
				return 3;
			}
			break;
		case 3:
			if( id->mNeedParam > Attributes()->mSCMax )
			{
				return 4;
			}
			break;
		}
		if( ( ItemDef::IsFashionCloth(id->mTypeID ) || ItemDef::IsCloth(id->mEquipType) ) && id->mResMale > 0 )
		{
			/*if( gender() == GENDER_FEMALE && !( id->mResFemale > 0 && id->mResFemale<2000 ) )
			{
				return 6;
			}
			if( gender() == GENDER_MALE && !( id->mResMale >= 2000 && id->mResMale<4000 ) )
			{
				return 6;
			}*/
			if (id->mGender != 0 && id->mGender != gender())
			{
				return 6;
			}
		}
		if (id->mJob != job() && id->mJob != 0)
		{
			return 7;
		}
		return 0;
	}

	void DomainPlay::bag_use_item_equipment(ItemMap::iterator& pos, int to)
	{
		if( pos->mItemFlags & ITEM_FLAG_USE_BIND )
		{
			pos->mItemFlags |= ITEM_FLAG_BIND;
			pos->mItemFlags &= (~ITEM_FLAG_USE_BIND);
		}
		if( pos->mDuration <= 0 )
		{
			AlertMessage(11,0,mGSL->mStringConsts.NOTIFY_ITEM_USE_BROKE);
			return;
		}
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
		if( ! id )
		{
			return;
		}
		{
			//ItemDef* id=mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
			if( id )
			{
				mScriptEcho = "";
				mGSL->GetScriptEngine()->ItemPlayer(this,id->mName.c_str(),id->mTypeID,"player.onEquipItem");
				if( ! mScriptEcho.empty() )
				{
					AlertMessage(1,0,mScriptEcho.c_str());
					return ;
				}
				mScriptEcho = "";
			}
		}
		//int on_position = pos->mTypeID / 10000 * (-1) * 2;
		int on_position = -2 * id->mEquipType;
		int on_position_2 = on_position - 1;
		bool one_full = true;
		int cycle_pos = 0;
		if( on_position == -5*2 
			|| on_position == -6*2 )
		{
			one_full = false;
			if( on_position == -5*2)
			{ //mDressGlove
				mDressGlove ++;
				if( mDressGlove > 1 )
				{
					mDressGlove = 0;
				}
				if( mDressGlove == 1 )
				{
					cycle_pos = 1;
				}

				if (to == on_position)
				{
					mDressGlove = 0;
					cycle_pos = 0;
				}
				else if(to == on_position_2)
				{
					mDressGlove = 1;
					cycle_pos = 1;
				}
			}
			if( on_position == -6*2)
			{//mDressRing
				mDressRing ++;
				if( mDressRing > 1 )
				{
					mDressRing = 0;
				}
				if( mDressRing == 1 )
				{
					cycle_pos = 1;
				}

				if (to == on_position)
				{
					mDressRing = 0;
					cycle_pos = 0;
				}
				else if(to == on_position_2)
				{
					mDressRing = 1;
					cycle_pos = 1;
				}
			}
		}
		ItemMap::iterator pos_1 = GetItemList().end();
		ItemMap::iterator pos_2 = GetItemList().end();
		ItemMap::iterator on_pos = GetItemList().begin();
		for(;on_pos != GetItemList().end();
			++ on_pos)
		{
			if( on_pos->mTypeID > 0 )
			{
				if( on_pos->mPosition == on_position )
				{
					pos_1 = on_pos;
				}
				if( on_pos->mPosition == on_position_2 )
				{
					pos_2 = on_pos;
				}
			}
		}
		int res_pos = on_position;
		on_pos = pos_1;
		if( cycle_pos )
		{
			res_pos = on_position_2;
			on_pos = pos_2;
			if( pos_2 != GetItemList().end() )
			{
				if( (!one_full) && pos_1 == GetItemList().end() )
				{
					res_pos = on_position;
					on_pos = pos_1;
				}
			}
		}
		else
		{
			if( pos_1 != GetItemList().end() )
			{
				if( (!one_full) && pos_2 == GetItemList().end() )
				{
					res_pos = on_position_2;
					on_pos = pos_2;
				}
			}
		}
		if( on_position == ITEM_XUESHI_POSITION && pos->mDuration > 0 )
		{
			mEquipMoXueShi = true;
		}
		
		int position = pos->mPosition;

		pos->mPosition = res_pos;
		mSession->GetListener()->notifyItemChange(mSession,*pos,0);

		if( on_pos != GetItemList().end() )
		{
			on_pos->mPosition = position;
			mSession->GetListener()->notifyItemChange(mSession,*on_pos,0);
		}
		else
		{
			SubItem it;
			it.mPosition = position;
			it.mTypeID = -1;
			mSession->GetListener()->notifyItemChange(mSession,it,0);
		}

		UpdateAvatar();
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyAvatarChange(this);
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* play = static_cast<DomainPlay*>(*pos);
					play->NotifyPlayerAddinfo(this);
				}
			}
		}
		NotifyAvatarChange(this);
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
		mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
	}
	int DomainPlay::find_empty_item_position_fashion()
	{
		char slot[EQUIP_TYPE_FASHION_CLOTH_SIZE];
		memset(slot, 0, EQUIP_TYPE_FASHION_CLOTH_SIZE);
		for (ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++pos)
		{
			if (pos->mTypeID > 0 && pos->mPosition >= EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN && pos->mPosition <= EQUIP_TYPE_FASHION_CLOTH_POS_END)
			{
				
				slot[(pos->mPosition - EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN)] ++;
				
			}
		}
		for (int i = 0; i < EQUIP_TYPE_FASHION_CLOTH_SIZE; i++)
		{
			if (slot[i] == 0)
			{
				return EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN+i;
				
			}
		}
		return -1;
	}
	int DomainPlay::CheckFashionPosNum()
	{
		return 0;
	}
	int DomainPlay::CheckItemFashion(int TypeID)
	{
		ItemDef* id2 = mGSL->GetItemDefineManager()->GetItemDefine(TypeID);//背包物品
		if(!id2)return 0;
		for (ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++pos)
		{
			if (pos->mTypeID > 0 && pos->mPosition >= EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN && pos->mPosition <= EQUIP_TYPE_FASHION_CLOTH_POS_END || pos->mPosition <0)
			{
				ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);//衣橱物品
				if(!id)continue;
				//衣橱中的物品icon和获得的物品相同则是一类
				if (id->mIconID == id2->mIconID)//同类
				{
					if (id->mLastTime > 0 && id2->mLastTime > 0)//同一限时物品
					{
						return 1;
					}
					else if (id->mLastTime > 0 && id2->mLastTime == 0)//衣橱限时  背包不限时
					{
						return 2;
					}
					else if (id->mLastTime == 0 && id2->mLastTime > 0)//衣橱不限时  背包限时
					{
						return 3;
					}
					else if (id->mLastTime == 0 && id2->mLastTime == 0)//都不限时
					{
						return 4;
					}
				}
			}
		}
		return 5;
	}

	int DomainPlay::CheckItemFashion2(int TypeID)
	{
		ItemDef* id2 = mGSL->GetItemDefineManager()->GetItemDefine(TypeID);//背包物品
		if(!id2)return 0;
		for (ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++pos)
		{
			if (pos->mTypeID > 0 && pos->mPosition >= EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN && pos->mPosition <= EQUIP_TYPE_FASHION_CLOTH_POS_END || pos->mPosition <0 )
			{
				ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);//衣橱物品
				if(!id)continue;
				//衣橱中的物品icon和获得的物品相同则是一类
				if (id->mIconID == id2->mIconID)//同类
				{
					if (id->mLastTime > 0 && id2->mLastTime > 0)//同一限时物品
					{
						return pos->mPosition;
					}
					else if (id->mLastTime > 0 && id2->mLastTime == 0)//衣橱限时  背包不限时
					{
						return pos->mPosition;
					}
					else if (id->mLastTime == 0 && id2->mLastTime > 0)//衣橱不限时  背包限时
					{
						return pos->mPosition;
					}
					else if (id->mLastTime == 0 && id2->mLastTime == 0)//都不限时
					{
						return pos->mPosition;
					}
				}
				
			}
		}
		return 0;
	}
	int DomainPlay::bag_use_item_fashion(int position, int id)
	{	
		int position2 = find_empty_item_position_fashion();
		for (ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++pos)
		{
			if (pos->mTypeID > 0 && position == pos->mPosition)
			{
				pos->mPosition = position2;
				SubItem it;
				it.mPosition = position;
				it.mTypeID = 0;
				mNeedClearItemList = true;
				mSession->GetListener()->notifyItemChange(mSession, it, 0);


				if (&pos)
				{
					mSession->GetListener()->notifyItemChange(mSession, *pos, 0);
				}
			}
		}
		return 0;
	}


	int DomainPlay::bag_use_item_gem(ItemMap::iterator& pos, int equip_pos)
	{
		if( pos->mItemFlags & ITEM_FLAG_USE_BIND )
		{
			pos->mItemFlags |= ITEM_FLAG_BIND;
			pos->mItemFlags &= (~ITEM_FLAG_USE_BIND);
		}
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
		if( ! id )
		{
			return 0;
		}
		mScriptEcho = "";
		mGSL->GetScriptEngine()->ItemPlayer(this,id->mName.c_str(),id->mTypeID,"player.onEquipGem");
		if( ! mScriptEcho.empty() )
		{
			AlertMessage(1,0,mScriptEcho.c_str());
			return 0;
		}
		mScriptEcho = "";
		//宝石镶嵌目标pos位
		int on_position = ItemDef::getGemEquipPosition(equip_pos,pos->mTypeID);
		
		//记录下待镶嵌宝石的宝石背包pos位
		int position = pos->mPosition;

		ItemMap::iterator on_pos = GetItemList().begin();
		for(;on_pos != GetItemList().end();
			++ on_pos)
		{
			if( on_pos->mTypeID > 0 )
			{
				if( on_pos->mPosition == on_position )
				{
					break;
				}
			}
		}

		//目标位置有宝石则卸下宝石
		if (on_pos != GetItemList().end())
		{
			int empty_pos = -999;
			int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(on_pos->mTypeID);

			char slot[256];
			memset(slot,0,256);

			//背包内未满堆叠宝石
			ItemMap::iterator usable_pos = GetItemList().begin();
			for(;usable_pos != GetItemList().end();
				++ usable_pos )
			{
				if( usable_pos->mPosition >= ITEM_BAG_BEGIN && usable_pos->mPosition < ITEM_BAG_END+mBagSlotAdd && usable_pos->mPosition < 256 )
				{
					if (usable_pos->mTypeID>0 )
					{
						slot[usable_pos->mPosition] ++;
						if (usable_pos->mTypeID == on_pos->mTypeID && usable_pos->mNumber < stackmax)
						{
							break;
						}
					}
				}
			}
			if(usable_pos != GetItemList().end())
			{
				//清理身上宝石
				on_pos->mTypeID = 0;
				//mSession->GetListener()->notifyItemChange(mSession,*on_pos,0);

				//增加背包宝石数量
				usable_pos->mNumber++;
				mSession->GetListener()->notifyItemChange(mSession,*usable_pos,0);
			}
			else
			{
				for(int i = 0;i < ITEM_BAG_SIZE + mBagSlotAdd ;i ++ )
				{
					if( slot[i] == 0 )
					{
						empty_pos = i;
						break;
					}
				}
				if (empty_pos >= 0)
				{
					//身上宝石转背包
					on_pos->mPosition = empty_pos;
					mSession->GetListener()->notifyItemChange(mSession,*on_pos,0);
				}
				else
				{
					//失败，背包满
					return -1;
				}
			}
		}


		//镶嵌宝石部分代码
		if (pos->mNumber > 1)
		{
			//扣除宝石背包
			pos->mNumber--;
			//镶嵌
			SubItem ii=*pos;
			ii.mPosition = on_position;
			ii.mNumber = 1;
			GetItemList().push_back(ii);
			mSession->GetListener()->notifyItemChange(mSession,ii,0);
		}
		else
		{
			//镶嵌
			pos->mPosition = on_position;
			//清理背包宝石
			SubItem it;
			it.mPosition = position;
			it.mTypeID = 0;
			mNeedClearItemList = true;
			mSession->GetListener()->notifyItemChange(mSession,it,0);
		}
		if (&pos)
		{
			mSession->GetListener()->notifyItemChange(mSession,*pos,0);
		}

		

		//UpdateAvatar();
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				//(*pos)->NotifyAvatarChange(this);
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* play = static_cast<DomainPlay*>(*pos);
					play->NotifyPlayerAddinfo(this);
				}
			}
		}
		//NotifyAvatarChange(this);
		CalculateAttribute();
		mSession->GetListener()->notifyAttributeChange(mSession);
		mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
		return 1;
	}
	int DomainPlay::ItemBagDepotExchange(int from,int number,int flag)
	{
		if(from < 0 || number <= 0){return 1;}
		int position = find_empty_item_position_in_depot();
		if(flag == 1){position = find_empty_item_position();}
		if(position <0){return 2;}
		SubItem* si = this->get_item_by_pos(from); if(! si){return 3;}if(number > si->mNumber){return 4;}
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(si->mTypeID); if(! id){return 5;}
		int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(si->mTypeID);
		SubItem sm; sm = *si; sm.mPosition = position; sm.mNumber = min(number,stackmax);
		si->mNumber -= min(number,stackmax);
		if(si->mNumber <= 0){
			si->mTypeID=0;mNeedClearItemList = true;
		}else{
			mSession->GetListener()->notifyItemChange(mSession,*si,0);
		}
		GetItemList().push_back(sm);mSession->GetListener()->notifyItemChange(mSession,sm,0);
		if(id->mLastTime > 0){mNeedFreshItemList = true;}
		return 0;
	}
	int DomainPlay::ItemPositionExchange(int from,int to,int flag,int issort)//是否直接重叠绑定物品
	{
		ItemMap::iterator pos_from = GetItemList().begin();
		if( mTradeItem.size() > size_t(get_empty_item_position_number()) )
		{
			if( from < ITEM_BAG_BEGIN )
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_CANCEL_BY_TRADING);
				return 0;
			}
		}
		if(to==1999&&from>=ITEM_BAG_BEGIN&&from<=ITEM_BAG_END+mBagSlotAdd)
		{
			to = find_empty_item_position_in_depot();
			if(to<0){mSession->GetListener()->notifyItemChange(mSession,*pos_from,0);return 0;}
		}
		if (from<ITEM_LOTTERYDEPOT_BEGIN&&to>=ITEM_LOTTERYDEPOT_BEGIN)
		{
			AlertMessage(11,0,mGSL->mStringConsts.NOTIFY_CANNOT_USE_LOTTERYDEPOT);
			mSession->GetListener()->notifyItemChange(mSession,*pos_from,0);
			return 0;
		}
		if ((from>=ITEM_LOTTERYDEPOT_BEGIN&&to<ITEM_BAG_BEGIN) )
		{
			mSession->GetListener()->notifyItemChange(mSession,*pos_from,0);
			return 0;
		}
		if (from>=ITEM_LOTTERYDEPOT_BEGIN&&to<ITEM_LOTTERYDEPOT_BEGIN)
		{
			if (to<ITEM_DEPOT_BEGIN)
			{
				to = find_empty_item_position();
				if (to<0)
				{
					to = find_empty_item_position_in_depot();
					if(to<0)
					{
						AlertMessage(11,0,mGSL->mStringConsts.NOTIFY_BAG_AND_DEPOT_FULL);
						mSession->GetListener()->notifyItemChange(mSession,*pos_from,0);
						return 10;
					}
				}
			}
			else if (to>=ITEM_DEPOT_BEGIN)
			{
				to = find_empty_item_position_in_depot();
				if(to<0){mSession->GetListener()->notifyItemChange(mSession,*pos_from,0);return 0;}
			}
		}
		if(  from < ITEM_BAG_BEGIN && to >= ITEM_BAG_BEGIN )
		{
			return UndressItem(from);
		}
		if( to < ITEM_BAG_BEGIN && from >= ITEM_BAG_BEGIN )
		{
			for( ItemMap::iterator pos = GetItemList().begin();
				pos != GetItemList().end();
				++ pos )
			{
				if( pos->mTypeID > 0 && pos->mPosition == from )
				{
					if( ItemDef::IsEquipment(pos->mSubType) )
					{
						return BagUseItem(pos->mPosition,pos->mTypeID,1,to);
					}
				}
			}
			return 3;
		}
		if( from < ITEM_BAG_BEGIN && to < ITEM_BAG_BEGIN )
		{
			if( from/2!=to/2 )
			{
				return 2;
			}
		}
		else
		{
			if( ! (
				( from >= ITEM_BAG_BEGIN && from < ITEM_BAG_END+mBagSlotAdd ) ||
				( from >= ITEM_DEPOT_BEGIN && from < (ITEM_DEPOT_END + mDepotSlotAdd) ) ||
				( from >= ITEM_LOTTERYDEPOT_BEGIN && from < ITEM_LOTTERYDEPOT_END )
				)
				)
			{
				return 2;
			}
			if( ! (
				( to >= ITEM_BAG_BEGIN && to < ITEM_BAG_END+mBagSlotAdd ) ||
				( to >= ITEM_DEPOT_BEGIN && to < (ITEM_DEPOT_END + mDepotSlotAdd) )||
				( to >= ITEM_LOTTERYDEPOT_BEGIN && from < ITEM_LOTTERYDEPOT_END )
				)
				)
			{
				return 2;
			}
		}
		ItemMap::iterator from_pos = GetItemList().begin();
		for(;from_pos != GetItemList().end();
			++ from_pos)
		{
			if( from_pos->mTypeID > 0 && from_pos->mPosition == from )
			{
				break;
			}
		}
		if( from < ITEM_DEPOT_BEGIN && to == ITEM_POSITION_EXCHANGE_BAG2DEPOT )
		{
			to = find_empty_item_position_in_depot();return 4;
		}
		if( from >= ITEM_DEPOT_BEGIN && to == ITEM_POSITION_EXCHANGE_DEPOT2BAG )
		{
			to = find_empty_item_position();return 4;
		}
		ItemMap::iterator to_pos = GetItemList().begin();
		for(;to_pos != GetItemList().end();
			++ to_pos)
		{
			if( to_pos->mTypeID > 0 && to_pos->mPosition == to )
			{
				break;
			}
		}
		if( from_pos != GetItemList().end() )
		{
			//放入仓库检测
			if (from >= ITEM_BAG_BEGIN && from < ITEM_BAG_END+mBagSlotAdd && to >= ITEM_DEPOT_BEGIN && to < ITEM_DEPOT_END+mDepotSlotAdd)
			{
				ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(from_pos->mTypeID);if(!id){return 7;}
				if (!id->mCanDepot)
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_CAN_NOT_DEPOT);
					return 8;
				}
			}
			if( to_pos != GetItemList().end() )
			{
				ItemDef* idf = mGSL->GetItemDefineManager()->GetItemDefine(to_pos->mTypeID);if(!idf){return 6;}
				int stackmax = mGSL->GetItemDefineManager()->GetItemStackMax(to_pos->mTypeID);
				if(from_pos->mTypeID == to_pos->mTypeID && to_pos->mNumber < stackmax && from_pos->mItemFlags == to_pos->mItemFlags)
				{
					int sub = max(0,min(stackmax-to_pos->mNumber,from_pos->mNumber));
					from_pos->mNumber-=sub;
					to_pos->mNumber+=sub;
					mSession->GetListener()->notifyItemChange(mSession,*to_pos,100);
					if( from_pos->mNumber<=0 ){from_pos->mTypeID=0;mNeedClearItemList = true;}
					mSession->GetListener()->notifyItemChange(mSession,*from_pos,100);
				}
				else
				{
					if(from_pos->mTypeID == to_pos->mTypeID && to_pos->mNumber < stackmax && 1 == flag)//允许将非绑定物品并为绑定物品
					{
						int sub = max(0,min(stackmax-to_pos->mNumber,from_pos->mNumber));
						from_pos->mNumber-=sub;
						to_pos->mNumber+=sub;
						to_pos->mItemFlags |= from_pos->mItemFlags;
						mSession->GetListener()->notifyItemChange(mSession,*to_pos,100);
						if( from_pos->mNumber<=0 ){from_pos->mTypeID=0;mNeedClearItemList = true;}
						mSession->GetListener()->notifyItemChange(mSession,*from_pos,100);
					}
					else
					{
						from_pos->mPosition =to;
						mSession->GetListener()->notifyItemChange(mSession,*from_pos,100);
						to_pos->mPosition = from;
						mSession->GetListener()->notifyItemChange(mSession,*to_pos,100);
					}
				}
			}
			else
			{
				from_pos->mPosition =to;
				mSession->GetListener()->notifyItemChange(mSession,*from_pos,100);
				SubItem it;it.mPosition = from;it.mTypeID = -1;
				mSession->GetListener()->notifyItemChange(mSession,it,100);
			}
			if( issort != 100 )
			{
				mSession->GetListener()->notifyItemPanelFresh(mSession,0);
			}
		}
		return 0;
	}
	int DomainPlay::find_empty_item_position()
	{
		char slot[256];
		memset(slot,0,256);
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && pos->mPosition < 256 && pos->mTypeID>0 )
			{
				slot[pos->mPosition] ++;
			}
		}
		for(int i = 0;i < ITEM_BAG_SIZE + mBagSlotAdd ;i ++ )
		{
			if( slot[i] == 0 )
			{
				return i;
			}
		}
		return -1;
	}
	int DomainPlay::find_empty_item_position_in_stall()
	{
		char slot[128];
		memset(slot,0,128);
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_STALL_BEGIN && pos->mPosition < ITEM_STALL_END && (pos->mPosition-ITEM_STALL_BEGIN) < 128 && pos->mTypeID>0 )
			{
				slot[pos->mPosition-ITEM_STALL_BEGIN] ++;
			}
		}
		for(int i = 0;i < ITEM_STALL_END-ITEM_STALL_BEGIN;i ++ )
		{
			if( slot[i] == 0 )
			{
				return ITEM_STALL_BEGIN+i;
			}
		}
		return -1;
	}
	int DomainPlay::find_empty_item_position_in_lotterydepot()
	{
		char slot[500];
		memset(slot,0,500);
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_LOTTERYDEPOT_BEGIN && pos->mPosition < ITEM_LOTTERYDEPOT_END && (pos->mPosition-ITEM_LOTTERYDEPOT_BEGIN) < 500 && pos->mTypeID>0 )
			{
				slot[pos->mPosition-ITEM_LOTTERYDEPOT_BEGIN] ++;
			}
		}
		for(int i = 0;i < ITEM_LOTTERYDEPOT_END-ITEM_LOTTERYDEPOT_BEGIN;i ++ )
		{
			if( slot[i] == 0 )
			{
				return ITEM_LOTTERYDEPOT_BEGIN+i;
			}
		}
		return -1;
	}
	int DomainPlay::get_empty_item_position_number_in_lotterydepot()
	{
		char slot[500];
		memset(slot,0,500);
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_LOTTERYDEPOT_BEGIN && pos->mPosition < ITEM_LOTTERYDEPOT_END )
			{
				slot[pos->mPosition-3000] ++;
			}
		}
		int ret = 0;
		for(int i = 0;i < ITEM_LOTTERYSIZE;i ++ )
		{
			if( slot[i] == 0 )
			{
				ret ++;
			}
		}
		return ret;
	}
	int DomainPlay::find_empty_item_position_in_depot()
	{
		char slot[256];
		memset(slot,0,256);
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_DEPOT_BEGIN && pos->mPosition < (ITEM_DEPOT_END + mDepotSlotAdd) && (pos->mPosition-ITEM_DEPOT_BEGIN) < 80 )
			{
				slot[pos->mPosition-ITEM_DEPOT_BEGIN] ++;
			}
		}
		for(int i = 0;i < (ITEM_DEPOT_END + mDepotSlotAdd)-ITEM_DEPOT_BEGIN;i ++ )
		{
			if( slot[i] == 0 )
			{
				return ITEM_DEPOT_BEGIN+i;
			}
		}
		return -1;
	}
	int DomainPlay::find_empty_item_position_in_xuanjing()
	{
		char slot[1024];
		memset(slot,0,1024);
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_XUANJING_BEGIN && pos->mPosition < ITEM_XUANJING_END && (pos->mPosition-ITEM_XUANJING_BEGIN) < ITEM_XUANJING_SIZE )
			{
				slot[pos->mPosition-ITEM_XUANJING_BEGIN] ++;
			}
		}
		for(int i = 0;i < ITEM_XUANJING_SIZE;i ++ )
		{
			if( slot[i] == 0 )
			{
				return ITEM_XUANJING_BEGIN+i;
			}
		}
		return -1;
	}
	int DomainPlay::get_empty_item_position_number()
	{
		char slot[256];
		memset(slot,0,256);
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_BAG_BEGIN && pos->mPosition < ITEM_BAG_END+mBagSlotAdd && pos->mPosition < 256 )
			{
				slot[pos->mPosition] ++;
			}
		}
		int ret = 0;
		for(int i = 0;i < ITEM_BAG_SIZE + mBagSlotAdd ;i ++ )
		{
			if( slot[i] == 0 )
			{
				ret ++;
			}
		}
		return ret;
	}
	int DomainPlay::get_empty_item_position_number_in_ChatTrade()
	{
		char slot[10];
		memset(slot,0,10);
		for( ItemMap::iterator pos = GetChatTradeItemList().begin();
			pos != GetChatTradeItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_CHATTRADE_BEGIN && pos->mPosition < ITEM_CHATTRADE_END )
			{
				slot[pos->mPosition-ITEM_CHATTRADE_BEGIN] ++;
			}
		}
		int ret = 0;
		for(int i = 0;i < ITEM_CHATTRADE_SIZE;i ++ )
		{
			if( slot[i] == 0 )
			{
				ret++;
			}
		}
		return ret;
	}
	int DomainPlay::find_empty_item_position_in_ChatTrade()
	{
		char slot[10];
		memset(slot,0,10);
		for( ItemMap::iterator pos = GetChatTradeItemList().begin();
			pos != GetChatTradeItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition >= ITEM_CHATTRADE_BEGIN && pos->mPosition < ITEM_CHATTRADE_END && (pos->mPosition-ITEM_CHATTRADE_BEGIN) < ITEM_CHATTRADE_SIZE )
			{
				slot[pos->mPosition-ITEM_CHATTRADE_BEGIN] ++;
			}
		}
		for(int i = 0;i < ITEM_CHATTRADE_SIZE;i ++ )
		{
			if( slot[i] == 0 )
			{
				return ITEM_CHATTRADE_BEGIN+i;
			}
		}
		return -1;
	}
	
	bool DomainPlay::DamageSkillMP(SkillDef* sd,SubSkill* pos)
	{
		//int mp = sd->DefSpell+sd->Spell*(pos->mLevel+1)/(3+1);
		int mp = sd->ConsumeMp;
		if( mp > 0 )
		{
			if( DamageMP(mp) )
			{
				NotifyHPMPChange(this,0,mp);
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();
					++ pos )
				{
					if ((*pos))
					{
						(*pos)->NotifyHPMPChange(this, 0, mp);
					}
				}
				return true;
			}
			else
			{
				AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_NOT_ENOUPH_MP);//not enouph mp
				mSession->GetListener()->resSkill(mSession,3,sd->TypeID);
				return false;
			}
		}
		return false;
	}
	int DomainPlay::UseSkill(int skill_type,int paramX,int paramY,unsigned int paramID)
	{
		PERF_NODE_FUNC();
		mMapEnterTick=0;
		if( mShowMount )
		{
			if( mMountLockUseSkill )
			{
				mSession->GetListener()->resSkill(mSession,1,skill_type);
				return 0;
			}
			else
			{
				if( skill_type != SkillDef::SKILL_TYPE_BanYueWanDao
					&& skill_type != SkillDef::SKILL_TYPE_LieHuoJianFa
					&& skill_type != SkillDef::SKILL_TYPE_PoTianZhan
					&& skill_type != SkillDef::SKILL_TYPE_ZhuRiJianFa
					&& skill_type != SkillDef::SKILL_TYPE_MoFaDun
					&& skill_type != SkillDef::SKILL_TYPE_ZhaoHuanKuLou
					&& skill_type != SkillDef::SKILL_TYPE_ZhaoHuanShenShou
					&& skill_type != SkillDef::SKILL_TYPE_CiShaJianShu
					&& skill_type != SkillDef::SKILL_TYPE_ZhaoHuanYueLing)
				{
					set_showmount(0);
					mGSL->GetScriptEngine()->OnlyPlayer(this,"gui.PanelMount.check_mount_cd");
				}
			}
		}
		lock_action_end();
		//SlowHPMPRecover(true);
		if( IsState(MAP_OBJECT_STATE_DEAD) || IsMabi() )
		{
			mSession->GetListener()->resSkill(mSession,1,skill_type);
			return 0;
		}
		if( mParentMap && mParentMap->IsSkillDisabled(skill_type) )
		{
			mSession->GetListener()->resSkill(mSession,1,skill_type);
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SKILL_MAP_DISABLED);
			return 0;
		}
		if( mParentMap && mParentMap->LockAction() )
		{
			mSession->GetListener()->resSkill(mSession,1,skill_type);
			return 0;
		}
		SubSkill* ss = FindSkill(skill_type);if(!ss){mSession->GetListener()->resSkill(mSession,1,skill_type);return 0;}
		SkillDef* sd = mSession->GetGameServerLib()->GetSkillDefineManager()->GetSkillDefine(skill_type, ss->mLevel);
		if(!sd)
		{
			mSession->GetListener()->resSkill(mSession,1,skill_type);
			return 0;
		}

		if( skill_type != SkillDef::SKILL_TYPE_BanYueWanDao
			&& skill_type != SkillDef::SKILL_TYPE_LieHuoJianFa
			&& skill_type != SkillDef::SKILL_TYPE_CiShaJianShu
			&& skill_type!=SkillDef::SKILL_TYPE_PoTianZhan
			&& skill_type != SkillDef::SKILL_TYPE_ZhuRiJianFa)
		{
			if( ss->mTick + sd->SkillCD * 1.0 / 1250  > mGSL->GetTimerFix()->Elapsed())
			{
				mSession->GetListener()->resSkill(mSession,1,skill_type);
				return 0;
			}

			if( SkillDef::IsNearSkill(skill_type) )
			{
				if( ! check_action(0.48) )
				{
					mSession->GetListener()->resSkill(mSession,1,skill_type);
					return 0;
				}
				if( ! check_skill(0.52) )
				{
					mSession->GetListener()->resSkill(mSession,1,skill_type);
					return 0;
				}
			}
			else
			{
				if( ! check_action(0.56) )
				{
					mSession->GetListener()->resSkill(mSession,1,skill_type);
					return 0;
				}
				if( ! check_skill(0.75) )
				{
					mSession->GetListener()->resSkill(mSession,1,skill_type);
					return 0;
				}
			}
		}
		bool check_ok = false;
		
		if (ss)
		{
			if( skill_type == SkillDef::SKILL_TYPE_YiBanGongJi )
			{
				check_ok = true;
			}
			else
			{
				if( skill_type == SkillDef::SKILL_TYPE_CiShaJianShu )
				{
					if( ss->mParam1 == 0 )
					{
						ss->mParam1 = 1;
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SKILL_CISHAJIANSHU_OPEN);
					}
					else
					{
						ss->mParam1 = 0;
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SKILL_CISHAJIANSHU_CLOSE);
					}
					mSession->GetListener()->notifySkillChange(mSession,*ss);
					mSession->GetListener()->resSkill(mSession,0,skill_type);
					return 0;
				}
				if( skill_type == SkillDef::SKILL_TYPE_BanYueWanDao )
				{
					if( ss->mParam1 == 0 )
					{
						ss->mParam1 = 1;
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SKILL_BANYUEWANDAO_OPEN);
					}
					else
					{
						ss->mParam1 = 0;
						AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SKILL_BANYUEWANDAO_CLOSE);
					}
					mSession->GetListener()->notifySkillChange(mSession,*ss);
					mSession->GetListener()->resSkill(mSession,0,skill_type);
					return 0;
				}
				if( skill_type == SkillDef::SKILL_TYPE_JiChuJianShu )
				{
					mSession->GetListener()->resSkill(mSession,0,skill_type);
					return 0;
				}
				if( skill_type == SkillDef::SKILL_TYPE_GongShaJianShu )
				{
					mSession->GetListener()->resSkill(mSession,0,skill_type);
					return 0;
				}
				if( skill_type == SkillDef::SKILL_TYPE_JinShenLiZhanFa )
				{
					mSession->GetListener()->resSkill(mSession,0,skill_type);
					return 0;
				}

				if( skill_type == SkillDef::SKILL_TYPE_YeManChongZhuang )
				{
					if( ! StartYeManChongZhuang(ss,true) )
					{
						mSession->GetListener()->resSkill(mSession,7,skill_type);
						return 0;
					}
				}
				if( sd )
				{
					if( skill_type == SkillDef::Skill_Type_Jump )
					{
						
					}
					else
					{
						if( skill_type == SkillDef::SKILL_TYPE_LieHuoJianFa||skill_type == SkillDef::SKILL_TYPE_PoTianZhan || skill_type == SkillDef::SKILL_TYPE_ZhuRiJianFa)
						{
							if (StartLieHuoJianFa( ss,false ))
								mSession->GetListener()->resSkill(mSession,9,skill_type);
							else
								mSession->GetListener()->resSkill(mSession,0,skill_type);
							return 0;
						}
						if( !DamageSkillMP(sd,ss) )
						{
							mSession->GetListener()->resSkill(mSession,8,skill_type);
							return 0;
						}
						check_ok = true;
					}
				}
			}
		}
		if (skill_type == SkillDef::SKILL_TYPE_LeiDianShu
			|| skill_type == SkillDef::SKILL_TYPE_BingPaoXiao
			|| skill_type == SkillDef::SKILL_TYPE_LingHunHuoFu)
		{
			int disX = std::abs(paramX - X());
			int disY = std::abs(paramY - Y());
			int dis = disX > disY ? disX : disY;
			if (dis > sd->CastDistanceMax)
			{
				check_ok = false;
			}
		}
		if( check_ok )
		{
			//释放技能的时候需要显隐，并且让怪物攻击玩家
			if( IsYingShen() ){	
				ClearYingShen();
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();
					++ pos )
				{
					if ((*pos))
					{
						if ((*pos)->TypeIsMonster())
						{
							PlayMonster* mon = (PlayMonster*)(*pos);
							mon->NotifyClearYingShen(this);
						}
					}
				}
			}
			//(sd->TypeID / 100) != skill_type
			if (sd->TypeID != skill_type){mSession->GetListener()->resSkill(mSession,1,skill_type);return 0;}
			PERF_NODE("swith_skill");
			switch(sd->TypeID)
			{
			case SkillDef::SKILL_TYPE_YiBanGongJi:
				{
					PERF_NODE("SKILL_TYPE_YiBanGongJi");
					if( mJob == JOB_WARRIOR )
					{
						skill_type = WarriorAttack(sd,paramX,paramY,paramID);
					}
					else
					{
						ActionYiBanGongJiBegin(sd,paramX,paramY,paramID);
					}
				}
				break;
				//warrior
			case SkillDef::SKILL_TYPE_JiChuJianShu:
			case SkillDef::SKILL_TYPE_GongShaJianShu:
			case SkillDef::SKILL_TYPE_CiShaJianShu:
			case SkillDef::SKILL_TYPE_BanYueWanDao:
			case SkillDef::SKILL_TYPE_LieHuoJianFa:
			case SkillDef::SKILL_TYPE_PoTianZhan:
			case SkillDef::SKILL_TYPE_ZhuRiJianFa:
				break;
			case SkillDef::SKILL_TYPE_YeManChongZhuang:
				{
					PERF_NODE("SKILL_TYPE_YeManChongZhuang");
					ActionYeManChongZhuangBegin(sd,paramX,paramY,paramID);
					StopYeManChongZhuang(ss);
				}
				break;
				//wizzad
			case SkillDef::SKILL_TYPE_HuoQiuShu:
				{
					PERF_NODE("SKILL_TYPE_HuoQiuShu");
					ActionHuoQiuShuBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_KangJuHuoHuan:
				{
					PERF_NODE("SKILL_TYPE_KangJuHuoHuan");
					ActionKangJuHuoHuanBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_DiYuHuo:
				{
					PERF_NODE("SKILL_TYPE_DiYuHuo");
					ActionDiYuHuoBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_LeiDianShu:
				{
					PERF_NODE("SKILL_TYPE_LeiDianShu");
					ActionLeiDianShuBegin(sd,&paramX,&paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_MieTianHuo:
				{
					PERF_NODE("SKILL_TYPE_MieTianHuo");
					ActionLeiDianShuBegin(sd,&paramX,&paramY,paramID);
				}
			case SkillDef::SKILL_TYPE_ShunJianYiDong:
				{
					PERF_NODE("SKILL_TYPE_ShunJianYiDong");
					ActionShunJianYiDongBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_DaHuoQiu:
				{
					PERF_NODE("SKILL_TYPE_DaHuoQiu");
					ActionDaHuoQiuBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_BaoLieHuoYan:
				{
					PERF_NODE("SKILL_TYPE_BaoLieHuoYan");
					ActionBaoLieHuoYanBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_HuoQiang:
				{
					PERF_NODE("SKILL_TYPE_HuoQiang");
					ActionHuoQiangBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_JiGuangDianYing:
				{
					PERF_NODE("SKILL_TYPE_JiGuangDianYing");
					ActionJiGuangDianYingBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_DiYuLeiGuang:
				{
					PERF_NODE("SKILL_TYPE_DiYuLeiGuang");
					ActionDiYuLeiGuangBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_MoFaDun:
				{
					PERF_NODE("SKILL_TYPE_MoFaDun");
					ActionMoFaDunBegin(sd, paramX, paramY, &paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_BingPaoXiao:
				{
					PERF_NODE("SKILL_TYPE_BingPaoXiao");
					ActionBingPaoXiaoBegin(sd,paramX,paramY,paramID);
				}
				break;
				//toarist
			case SkillDef::SKILL_TYPE_QiGongBo:
				{
					PERF_NODE("SKILL_TYPE_QiGongBo");
					ActionKangJuHuoHuanBegin(sd,paramX,paramY,paramID);
				}
			case SkillDef::SKILL_TYPE_ZhiYuShu:
				{
					PERF_NODE("SKILL_TYPE_ZhiYuShu");
					ActionZhiYuShuBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_JinShenLiZhanFa:
				break;
			case SkillDef::SKILL_TYPE_ShiDuShu:
				{
					PERF_NODE("SKILL_TYPE_ShiDuShu");
					ActionShiDuShuBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_LingHunHuoFu:
				{
					PERF_NODE("SKILL_TYPE_LingHunHuoFu");
					ActionLingHunHuoFuBegin(sd,&paramX,&paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_ZhaoHuanKuLou:
				{
					PERF_NODE("SKILL_TYPE_ZhaoHuanKuLou");
					ActionZhaoHuanKuLouBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_ZhaoHuanYueLing:
				{
					PERF_NODE("SKILL_TYPE_ZhaoHuanYueLing");
					ActionZhaoHuanYueLingBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_YinShenShu:
				{
					PERF_NODE("SKILL_TYPE_YinShenShu");
					ActionYinShenShuBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_JiTiYinShenShu:
				{
					PERF_NODE("SKILL_TYPE_JiTiYinShenShu");
					ActionJiTiYinShenShuBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_YouLingDun:
				{
					PERF_NODE("SKILL_TYPE_YouLingDun");
					ActionYouLingDunBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_ShenShengZhanJiaShu:
				{
					PERF_NODE("SKILL_TYPE_ShenShengZhanJiaShu");
					ActionShenShengZhanJiaShuBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_QunTiZhiLiao:
				{
					PERF_NODE("SKILL_TYPE_QunTiZhiLiao");
					ActionQunTiZhiLiaoBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_ZhaoHuanShenShou:
				{
					PERF_NODE("SKILL_TYPE_ZhaoHuanShenShou");
					ActionZhaoHuanShenShouBegin(sd,paramX,paramY,paramID);
				}
				break;
			case SkillDef::Skill_Type_Jump:
				{
					PERF_NODE("SKILL_TYPE_LingHunHuoFu");
					ActionJumpBegin(sd,&paramX,&paramY,paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_LiuXingHuoYu:
				{
					PERF_NODE("SKILL_TYPE_LiuXingHuoYu");
					ActionLiuXingHuoYuBegin(sd,paramX,paramY,&paramID);
				}
				break;
			case SkillDef::SKILL_TYPE_HuoLongQiYan:
				{
					PERF_NODE("SKILL_TYPE_HuoLongQiYan");
					ActionLiuXingHuoYuBegin(sd,paramX,paramY,&paramID);
				}
				break;
			}
			if(skill_type != SkillDef::SKILL_TYPE_MoFaDun && skill_type != SkillDef::SKILL_TYPE_LieHuoJianFa 
				&& skill_type != SkillDef::SKILL_TYPE_PoTianZhan && skill_type != SkillDef::SKILL_TYPE_ZhuRiJianFa
				&& skill_type != SkillDef::SKILL_TYPE_ZhaoHuanKuLou && skill_type != SkillDef::SKILL_TYPE_ZhaoHuanShenShou
				&& skill_type != SkillDef::SKILL_TYPE_ZhaoHuanYueLing )
			{
				CollectBreak();
				ChangeMountBreak();
			}
			SkillDef* sdd = mSession->GetGameServerLib()->GetSkillDefineManager()->GetSkillDefine(skill_type,ss->mLevel);
			if(!sdd)
			{
				mSession->GetListener()->resSkill(mSession,1,skill_type);
				return 0;
			}

			{
				PERF_NODE("public skill msg");
				mSession->GetListener()->resSkill(mSession,0,skill_type);
				NotifyUseSkill(this,sdd->EffectType,paramX,paramY,paramID,sdd->Effect_ResID, this->Dir(), ss->mLevel);
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();
					++ pos )
				{
					if ((*pos))
					{
						(*pos)->NotifyUseSkill(this, sdd->EffectType, paramX, paramY, paramID, sdd->Effect_ResID, this->Dir(), ss->mLevel);
					}
				}
				/*int weapon_dura_damage = RandRange(3) + 1;
				DamageWeaponDuration(weapon_dura_damage);*/
			}
			/*if (sd->TypeID ==SkillDef::SKILL_TYPE_YiBanGongJi && mJob==JOB_TAOIST)
			{
				TrainSkill(SkillDef::SKILL_TYPE_JinShenLiZhanFa,RandRange(3)+1);
			}
			else if (sd->TypeID != SkillDef::SKILL_TYPE_YiBanGongJi &&
				sd->TypeID != SkillDef::SKILL_TYPE_DiYuHuo &&
				sd->TypeID != SkillDef::SKILL_TYPE_ShunJianYiDong &&
				sd->TypeID != SkillDef::SKILL_TYPE_HuoQiang &&
				sd->TypeID != SkillDef::SKILL_TYPE_YouHuoZhiGuang &&
				sd->TypeID != SkillDef::SKILL_TYPE_ShengYanShu &&
				sd->TypeID != SkillDef::SKILL_TYPE_ZhaoHuanKuLou &&
				sd->TypeID != SkillDef::SKILL_TYPE_ZhaoHuanShenShou &&
				sd->TypeID != SkillDef::SKILL_TYPE_ZhaoHuanYueLing &&
				sd->TypeID != SkillDef::SKILL_TYPE_QiGongBo &&
				sd->TypeID != SkillDef::SKILL_TYPE_HuoLongQiYan &&
				sd->TypeID != SkillDef::SKILL_TYPE_LiuXingHuoYu &&
				sd->TypeID != SkillDef::SKILL_TYPE_PoTianZhan &&
				sd->TypeID != SkillDef::SKILL_TYPE_ZhuRiJianFa )
			{
				TrainSkill(sd->TypeID,RandRange(3)+1);
			}*/
			TrainSkill(sd->TypeID, sdd->UseAddExp);
			return 0;
		}
		ss->mTick = mGSL->GetTimerFix()->Elapsed();
		mSession->GetListener()->resSkill(mSession,4,skill_type);
		return 0;
	}
	void DomainPlay::OnFrameTimer(int param)
	{
		int deletepos = 0;
		SubAction* act = FindAction(param);
		if( act )
		{
			switch(act->mType)
			{
			case SkillDef::SKILL_TYPE_ZhuRiJianFa:
				break;
			case SkillDef::SKILL_TYPE_YiBanGongJi:
				break;
			case SkillDef::SKILL_TYPE_JiChuJianShu:
				break;
			case SkillDef::SKILL_TYPE_GongShaJianShu:
				break;
			case SkillDef::SKILL_TYPE_CiShaJianShu:
				break;
			case SkillDef::SKILL_TYPE_BanYueWanDao:
				break;
			case SkillDef::SKILL_TYPE_YeManChongZhuang:
				break;
			case SkillDef::SKILL_TYPE_LieHuoJianFa:
				break;
			case SkillDef::SKILL_TYPE_PoTianZhan:
				break;
			case SkillDef::SKILL_TYPE_HuoQiuShu:
				break;
			case SkillDef::SKILL_TYPE_KangJuHuoHuan:
				break;
			case SkillDef::SKILL_TYPE_QiGongBo:
				break;
			case SkillDef::SKILL_TYPE_MieTianHuo:
				break;
			case SkillDef::SKILL_TYPE_DiYuHuo:
				deletepos = ActionDiYuHuoAttack(param,act);
				break;
			case SkillDef::SKILL_TYPE_LeiDianShu:
				break;
			case SkillDef::SKILL_TYPE_ShunJianYiDong:
				deletepos = ActionShunJianYiDongAttack(act);
				break;
			case SkillDef::SKILL_TYPE_DaHuoQiu:
				break;
			case SkillDef::SKILL_TYPE_BaoLieHuoYan:
				break;
			case SkillDef::SKILL_TYPE_HuoQiang:
				//deletepos = ActionHuoQiangAttack(param,act);
				break;
			case SkillDef::SKILL_TYPE_JiGuangDianYing:
				break;
			case SkillDef::SKILL_TYPE_DiYuLeiGuang:
				break;
			case SkillDef::SKILL_TYPE_BingPaoXiao:
				break;
			case SkillDef::SKILL_TYPE_LingHunHuoFu:
				break;
			}
			if( deletepos==0 )
			{
				DelAction(param);
			}
		}
	}

	void DomainPlay::TrainSkill(int skill_type,int exp)
	{
		SubSkill* si = FindSkill(skill_type);
		if (si)
		{
			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(skill_type,si->mLevel);
			if(sd)
			{
				if( si->add_exp(sd,exp,level()) )
				{
					PushSkillDefine(si->mTypeID,si->mLevel);
					mSession->GetListener()->notifySkillChange(mSession,*si);
					if( skill_type == SkillDef::SKILL_TYPE_JinShenLiZhanFa
						|| skill_type == SkillDef::SKILL_TYPE_JiChuJianShu )
					{
						CalculateAttribute();
					}
				}
			}
		}
	}
	void DomainPlay::SetSkillLevel(int skill,int level)
	{
		SubSkill* si = FindSkill(skill);
		if (si)
		{
			//SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(skill,si->mLevel);
			if (si/* && sd*/)
			{
				if (level >= 0 && level <= 9)
				{
					si->mLevel = level;
				}
				PushSkillDefine(si->mTypeID,si->mLevel);
				mSession->GetListener()->notifySkillChange(mSession,*si);
				if (si->mLevel == 0)
				{
					RemoveSkill(skill);
				}
			}
		}
	}
	int DomainPlay::CheckSkillLevelUp(int skill_type)
	{
		SubSkill* si = FindSkill(skill_type);
		if (si)
		{
			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(skill_type, si->mLevel);
			if (sd)
			{
				int ret = si->check_level_up(sd, level());
				if (ret)
				{
					PushSkillDefine(si->mTypeID,si->mLevel);
					mSession->GetListener()->notifySkillChange(mSession,*si);
					if( skill_type == SkillDef::SKILL_TYPE_JinShenLiZhanFa
						|| skill_type == SkillDef::SKILL_TYPE_JiChuJianShu )
					{
						CalculateAttribute();
					}
					return true;
				}
			}
		}
		return false;
	}
	int DomainPlay::WarriorAttack(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int ret=SkillDef::SKILL_TYPE_YiBanGongJi;
		SubSkill* jichujianfa=0;
		//修改没武器也能使用释放技能
		/*if( mWeapon == -1 )
		{
			ActionYiBanGongJiBegin(sd,ss,paramX,paramY,paramID);
			ret = SkillDef::SKILL_TYPE_YiBanGongJi;
		}
		else*/
		{
			SubSkill* liehuojianfa=0;
			SubSkill* cishajianshu=0;
			SubSkill* gongshajianshu=0;
			SubSkill* banyuewandao=0;
			SubSkill* potianzhan=0;
			SubSkill* zhuri=0;
			for( SkillMap::iterator pos = GetSkillList().begin();
				pos != GetSkillList().end();
				++ pos )
			{
				if(pos->mLevel <= 0){continue;}
				if(pos->mTypeID==SkillDef::SKILL_TYPE_ZhuRiJianFa)
				{
					zhuri=&(*pos);
				}
				if(pos->mTypeID==SkillDef::SKILL_TYPE_PoTianZhan)
				{
					potianzhan=&(*pos);
				}
				if( pos->mTypeID == SkillDef::SKILL_TYPE_LieHuoJianFa )
				{
					liehuojianfa = &(*pos);
				}
				if( pos->mTypeID == SkillDef::SKILL_TYPE_CiShaJianShu )
				{
					cishajianshu = &(*pos);
				}
				if( pos->mTypeID == SkillDef::SKILL_TYPE_GongShaJianShu )
				{
					gongshajianshu = &(*pos);
				}
				if( pos->mTypeID == SkillDef::SKILL_TYPE_BanYueWanDao )
				{
					banyuewandao = &(*pos);	
				}
				if (pos->mTypeID == SkillDef::SKILL_TYPE_JiChuJianShu)
				{
					jichujianfa = &(*pos);
				}
			}
			SkillDef* zrd=0;
			SkillDef* ptd=0;
			SkillDef* lsd = 0;
			SkillDef* csd = 0;
			SkillDef* gsd = 0;
			SkillDef* bsd = 0;
			if (zhuri)
			{
				zrd=mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_ZhuRiJianFa,zhuri->mLevel);
			}
			if (potianzhan)
			{
				ptd=mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_PoTianZhan,potianzhan->mLevel);
			}
			if (liehuojianfa)
			{
				lsd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_LieHuoJianFa,liehuojianfa->mLevel);
			}
			if (cishajianshu)
			{
				csd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_CiShaJianShu,cishajianshu->mLevel);
			}
			if (gongshajianshu)
			{
				gsd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_GongShaJianShu,gongshajianshu->mLevel);
			}
			if (banyuewandao)
			{
				bsd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_BanYueWanDao,banyuewandao->mLevel);
			}
			if( gongshajianshu )
			{
				StartGongShaJianShu(gongshajianshu);
			}
			if ( zrd && zhuri && IsLieHuoJianFaOnway(zhuri) && IsLieHuoNear(zhuri))
			{
				ActionLieHuoJianFaBegin(zrd,paramX,paramY,paramID);
				ret=SkillDef::SKILL_TYPE_ZhuRiJianFa;
				TrainSkill(SkillDef::SKILL_TYPE_ZhuRiJianFa,zrd->UseAddExp);
				StopLieHuoJianFa(zhuri);
			}
			else if(ptd&&potianzhan&&IsLieHuoJianFaOnway(potianzhan) && IsLieHuoNear(potianzhan))
			{
				ActionLieHuoJianFaBegin(ptd,paramX,paramY,paramID);
				ret=SkillDef::SKILL_TYPE_PoTianZhan;
				TrainSkill(SkillDef::SKILL_TYPE_PoTianZhan,ptd->UseAddExp);
				StopLieHuoJianFa(potianzhan);
			}
			else if( lsd && liehuojianfa && IsLieHuoJianFaOnway(liehuojianfa) && IsLieHuoNear(liehuojianfa) )
			{
				ActionLieHuoJianFaBegin(lsd,paramX,paramY,paramID);
				ret = SkillDef::SKILL_TYPE_LieHuoJianFa;
				TrainSkill(SkillDef::SKILL_TYPE_LieHuoJianFa,lsd->UseAddExp);
				StopLieHuoJianFa(liehuojianfa);
			}
			else if( gsd && gongshajianshu && IsGongShaJianShuOnway(gongshajianshu) )
			{
				ActionGongShaJianShuBegin(gsd,paramX,paramY,paramID);
				ret = SkillDef::SKILL_TYPE_GongShaJianShu;
				TrainSkill(SkillDef::SKILL_TYPE_GongShaJianShu,gsd->UseAddExp);
				StopGongShaJianShu(gongshajianshu);
			}
			else if( banyuewandao && IsBanYueWanDaoOpen(banyuewandao) && bsd && DamageSkillMP(bsd,banyuewandao) )
			{
				ActionBanYueWanDaoBegin(bsd,paramX,paramY,paramID);
				ret = SkillDef::SKILL_TYPE_BanYueWanDao;
				TrainSkill(SkillDef::SKILL_TYPE_BanYueWanDao,bsd->UseAddExp);
			}
			else if( csd && cishajianshu && IsCiShaJianShuOpen(cishajianshu) )
			{
				ActionCiShaJianShuBegin(csd,paramX,paramY,paramID);
				ret = SkillDef::SKILL_TYPE_CiShaJianShu;
				TrainSkill(SkillDef::SKILL_TYPE_CiShaJianShu,csd->UseAddExp);
			}
			else
			{
				ActionYiBanGongJiBegin(sd,paramX,paramY,paramID);
				ret = SkillDef::SKILL_TYPE_YiBanGongJi;
			}
		}
		if (jichujianfa)
		{
			SkillDef* jcd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_CiShaJianShu,jichujianfa->mLevel);
			if (jcd)
			{
				TrainSkill(SkillDef::SKILL_TYPE_JiChuJianShu,jcd->UseAddExp);
			}
		}
		
		return ret;
	}

	void DomainPlay::StartGongShaJianShu(SubSkill* subskill)
	{
		subskill->mParam1 --;
	}
	void DomainPlay::StopGongShaJianShu(SubSkill* subskill)
	{
		subskill->mParam1 = 7 - subskill->mLevel;
		subskill->mParam2 = rand()%subskill->mParam1;
	}
	bool DomainPlay::IsGongShaJianShuOnway(SubSkill* subskill)
	{
		if ( subskill->mParam1 < subskill->mParam2 )
		{
			int x = NextX(X(),mDir);
			int y = NextY(Y(),mDir);
			PlayActiveObject* objects[20];
			int obj_num = FindActiveInPosition(x,y,objects,20);
			for(int i = 0; i < obj_num; i++ )
			{
				PlayActiveObject* ob = objects[i];
				if( ob->IsInteractive() )
				{
					if( IsProperAttackDes(ob, NULL) )
					{
						return true;
					}
					break;
				}
			}
		}
		return false;
	}

	void DomainPlay::OpenCiShaJianShu()
	{
		SubSkill* sk = FindSkill(SkillDef::SKILL_TYPE_CiShaJianShu);
		if( sk )
		{
			if( sk->mParam1==0 )
			{
				UseSkill(SkillDef::SKILL_TYPE_CiShaJianShu,0,0,0);
			}
		}
	}
	bool DomainPlay::IsCiShaJianShuOpen(SubSkill* subskill)
	{
		if( subskill->mParam1>0)
		{
			if( mGSL->mCiShaJianShuCheckObj )
			{
				int x = NextX(NextX(X(),mDir),mDir);
				int y = NextY(NextY(Y(),mDir),mDir);
				PlayActiveObject* objects[20];
				int obj_num = FindActiveInPosition(x,y,objects,20);
				for(int i = 0; i < obj_num; i++ )
				{
					PlayActiveObject* ob = objects[i];
					if( ob->IsInteractive() )
					{
						if( IsProperAttackDes(ob, NULL) )
						{
							return true;
						}
						break;
					}
				}
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	void DomainPlay::OpenBanYueWanDao()
	{
		SubSkill* sk = FindSkill(SkillDef::SKILL_TYPE_BanYueWanDao);
		if( sk )
		{
			if( sk->mParam1==0 )
			{
				UseSkill(SkillDef::SKILL_TYPE_BanYueWanDao,0,0,0);
			}
		}
	}
	bool DomainPlay::IsBanYueWanDaoOpen(SubSkill* subskill)
	{
		if( subskill->mParam1 > 0 )
		{
			int monsters = 0;
			PlayActiveObject* objects[2];
			for(int i=-2;i<3;i++)
			{
				int x1 = NextX(X(),(mDir+i+8)%8);int y1 = NextY(Y(),(mDir+i+8)%8);
				int obj_num = FindActiveInPosition(x1,y1,objects,2);
				monsters += obj_num;
				if(monsters>=2)
				{
					return true;
				}
			}
		}
		return false;
	}

	void DomainPlay::CheckLieHuoValid()
	{
		SubSkill* liehuo = FindSkill(SkillDef::SKILL_TYPE_LieHuoJianFa);
		SubSkill* potian = FindSkill(SkillDef::SKILL_TYPE_PoTianZhan);
		SubSkill* zhuri = FindSkill(SkillDef::SKILL_TYPE_ZhuRiJianFa);
		if(liehuo)
		{
			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(liehuo->mTypeID,liehuo->mLevel);
			if( sd && liehuo->mParam1 && liehuo->mTick > mGSL->GetTimerFix()->Elapsed() + sd->SkillCD * 1.0 / 1000)
			{
				liehuo->mParam1=0;
				mSession->GetListener()->resSkill(mSession,7,sd->TypeID);
			}
		}
		if(potian)
		{
			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(potian->mTypeID,potian->mLevel);
			if( sd && potian->mParam1 && potian->mTick > mGSL->GetTimerFix()->Elapsed() + sd->SkillCD * 1.0 / 1000)
			{
				potian->mParam1=0;
				mSession->GetListener()->resSkill(mSession,7,sd->TypeID);
			}
		}
		if(zhuri)
		{
			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(zhuri->mTypeID,zhuri->mLevel);
			if( sd && zhuri->mParam1 && zhuri->mTick > mGSL->GetTimerFix()->Elapsed() + sd->SkillCD * 1.0 / 1000)
			{
				zhuri->mParam1=0;
				mSession->GetListener()->resSkill(mSession,7,sd->TypeID);
			}
		}

	}

	bool DomainPlay::StartLieHuoJianFa(SubSkill* subskill,bool check)
	{
		SkillDef* sd = mSession->GetGameServerLib()->GetSkillDefineManager()->GetSkillDefine(subskill->mTypeID, subskill->mLevel);

		if (!sd)
			return false;
	
		if( subskill->mTick + sd->SkillCD * 1.0 / 1250  < mGSL->GetTimerFix()->Elapsed() || subskill->mTick < 0.1f)
		{
			SubSkill* liehuo = FindSkill(SkillDef::SKILL_TYPE_LieHuoJianFa);
			SubSkill* potian = FindSkill(SkillDef::SKILL_TYPE_PoTianZhan);
			SubSkill* zhuri = FindSkill(SkillDef::SKILL_TYPE_ZhuRiJianFa);
			if(liehuo)
				liehuo->mParam1=0;
			if(potian)
				potian->mParam1=0;
			if(zhuri)
				zhuri->mParam1=0;

			subskill->mParam1 = 1;
			subskill->mTick = mGSL->GetTimerFix()->Elapsed();
			//mSession->GetListener()->notifySkillChange(mSession,*subskill);

			return true;
		}
		
		return false;
	}
	void DomainPlay::StopLieHuoJianFa(SubSkill* subskill)
	{
		subskill->mParam1 =0;
		subskill->mTick = mGSL->GetTimerFix()->Elapsed();
		mSession->GetListener()->notifySkillChange(mSession,*subskill);
	}
	bool DomainPlay::IsLieHuoJianFaOnway(SubSkill* subskill)
	{
		if( subskill->mParam1 > 0 )
		{
			
			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(subskill->mTypeID,subskill->mLevel);
			if(sd)
			{
				if( subskill->mTick + sd->SkillCD * 1.0 / 1000 > mGSL->GetTimerFix()->Elapsed() )
				{
					return true;
				}
			}
		}
		return false;
	}
	bool DomainPlay::IsLieHuoNear(SubSkill* subskill)
	{
		if( subskill->mParam1>0)
		{
			int x = NextX(X(),mDir);
			int y = NextY(Y(),mDir);
			PlayActiveObject* objects[20];
			int obj_num = FindActiveInPosition(x,y,objects,20);
			for(int i = 0; i < obj_num; i++ )
			{
				PlayActiveObject* ob = objects[i];
				if( ob->IsInteractive() )
				{
// 					if( IsProperAttackDes(ob,SkillDef::SKILL_TYPE_LieHuoJianFa) )
// 					{
						return true;
//					}
					break;
				}
			}
		}
		return false;
	}
	bool DomainPlay::StartYeManChongZhuang(SubSkill* subskill,bool check)
	{
		SkillDef* sd = mSession->GetGameServerLib()->GetSkillDefineManager()->GetSkillDefine(subskill->mTypeID, subskill->mLevel);

		if (!sd)
		{
			return false;
		}

		if( subskill->mParam1==0 || (subskill->mParam1 >0 && (subskill->mTick + sd->SkillCD * 1.0 / 1250 < mGSL->GetTimerFix()->Elapsed() || subskill->mTick < 0.1f)))
		{

			if( subskill->mTick + sd->SkillCD * 1.0 / 1250 < mGSL->GetTimerFix()->Elapsed() || subskill->mTick < 0.1f)
			{
				if( ! check )
				{
					subskill->mParam1 = 1;
					subskill->mTick = mGSL->GetTimerFix()->Elapsed();
					mSession->GetListener()->notifySkillChange(mSession,*subskill);
				}
				return true;
			}
		}
		return false;
	}
	void DomainPlay::StopYeManChongZhuang(SubSkill* subskill)
	{
		subskill->mParam1 =0;
		// subskill->mTick = mGSL->GetTimerFix()->Elapsed();
		mSession->GetListener()->notifySkillChange(mSession,*subskill);
	}
	bool DomainPlay::IsYeManChongZhuangOnway(SubSkill* subskill)
	{
		if( subskill->mParam1 > 0 )
		{
			if( subskill->mTick + 5 > mGSL->GetTimerFix()->Elapsed() )
			{
				return true;
			}
		}
		return false;
	}
	int DomainPlay::ActionDiYuHuoAttack(int ActionKey,SubAction* ai)
	{
		int x = ai->mData.attackCount.targetx;
		int y = ai->mData.attackCount.targety;
		for(int i = 0;i<5-ai->mData.attackCount.count;i++)
		{
			x = NextX(x,ai->mData.attackCount.target_id);
			y = NextY(y,ai->mData.attackCount.target_id);
		}
		if( mParentMap->GetBlockData(x,y,2) )
		{
			return 0;
		}
		SubSkill* si = FindSkill(ai->mType);
		if (si)
		{
			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(ai->mType,si->mLevel);
			if (sd)
			{
				int paw = GetAttackPawer(sd->Pawer(sd->AdvPawer(),si->mLevel) + Attributes()->mMC,Attributes()->mMCMax-Attributes()->mMC+1);
				// AttackPosition(x,y,ai->mType,paw,false,0);
			}
		}
		ai->mData.attackCount.count--;
		if( ai->mData.attackCount.count > 0 )
		{
			mSession->GetGameServerLib()->GetFrameTimer()->AddTimer(5,this,ActionKey);
			//TODO
			return 1;
		}
		//TrainSkill(ai->mType,RandRange(3)+1);
		return 0;
	}

	int DomainPlay::ActionShunJianYiDongAttack(SubAction* ai)
	{
		int x,y;
		std::string map_id;
		PlayMap* map = 0;
		if( get_home_map_position(map_id,&x,&y) )
		{
			map = mGSL->GetMapManager()->FindMap(map_id);
		}
		if( ! map ){
			map = mParentMap;
		}
		if( map )
		{
			if( map->GenValidateRandomPosition(&x,&y) )
			{
				EnterMap(map->GetID(),x,y);
			}
		}
		TrainSkill(ai->mType,RandRange(3)+1);
		return 0;
	}

	void DomainPlay::ActionJumpBegin(SkillDef* sd,int* targetx,int* targety,unsigned int paramID)
	{
		int dir = Diff2Dir((float)*targetx-X(),(float)*targety-Y());
		Turn(dir);
		int rx = X();
		int ry = Y();
		int step = 5;
		for( int i = step;i>1;i-- )
		{
			int ox = X();
			int oy = Y();
			for(int j=0;j<i;j++)
			{
				ox = NextX(ox,dir);
				oy = NextY(oy,dir);
			}
			if( mParentMap )
			{
				int b = mParentMap->GetBlockData(ox,oy,0);
				if( b<=0 )
				{
					rx = ox;
					ry = oy;
					break;
				}
			}
		}
		*targetx = rx;
		*targety = ry;
		SetPosition(rx,ry);
	}
	void DomainPlay::ActionZhaoHuanShenShouBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		if( mSlaveKuLou )
		{
			mSlaveKuLou->DamageHP(this,999999,0);
		}
		if (!sd)
		{
			return;
		}

		MonDef* md = 0;
		//从技能配置里读取神兽的id
		int monid = sd->Param0;
		md = mGSL->GetMonDefManager()->FindMonDef(monid);
		if( mSlaveShenShou == 0 )
		{
			if( md )
			{
				mSlaveShenShou = new PlaySlaveDummy;
				mSlaveShenShou->mOwner = this;
				mSlaveShenShou->Name(md->name);
				char aii[256];aii[0]=0;
				char opt[1024];opt[0]=0;
				//sscanf_s(md->ai.c_str(),"%[^|]|%s",aii,_countof(aii),opt,_countof(opt));
				mGSL->GetMapManager()->LoadAIObject(md,mSlaveShenShou,opt);

				// 调整属性
				mSlaveShenShou->ChangeAttrByOwnerAttr(md);
			}
		}
		if( mSlaveShenShou )
		{
			if( ! mSlaveShenShou->mLiving )
			{
				if (sd && md)
				{
					mGSL->GetMapManager()->LoadAIObject(md, mSlaveShenShou, "");
					mSlaveShenShou->Name(md->name);
					// 调整狗属性
					mSlaveShenShou->ChangeAttrByOwnerAttr(md);

				}
				mSlaveShenShou->Relive(mParentMap,X(),Y(),Dir());
			}
			else
			{
				mSlaveShenShou->ForceMove(X(),Y(),Dir());
			}
		}
		changeSlaveState(1);
	}
	void DomainPlay::ActionZhaoHuanKuLouBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		
	}
	void DomainPlay::ActionZhaoHuanYueLingBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		
	}
	void DomainPlay::checkZhaoHuan()
	{
		if( job() == JOB_TAOIST )
		{
			changeSlaveState(0);
		}
	}

	void DomainPlay::gen_slave(int id, int mon_id,int ai_type)
	{
		PlaySlave ** pSlave = NULL;

		if( id >= 0 && id< 5 )
			pSlave = &mSlaves[id];
		else if (id == 10)
			pSlave = &mSlaveHuwei;
		else
			return;

		if( *pSlave )
		{
			if( (*pSlave)->mParentMap )
			{
				(*pSlave)->mParentMap->RemoveObject((*pSlave));
			}
			delete (*pSlave);(*pSlave) = 0;
		}
		if( (*pSlave) == 0 )
		{
			MonDef* md = mGSL->GetMonDefManager()->FindMonDef(mon_id);
			if( md )
			{
				(*pSlave) = new PlaySlaveDummy;
				(*pSlave)->mOwner = this;
				(*pSlave)->Name(md->name);

				(*pSlave)->mAiType = ai_type;
				char aii[256];aii[0]=0;
				char opt[1024];opt[0]=0;
				sscanf_s(md->ai.c_str(),"%[^|]|%s",aii,_countof(aii),opt,_countof(opt));
				mGSL->GetMapManager()->LoadAIObject(md,(*pSlave),opt);
			}
		}

		if( (*pSlave) )
		{
			if( ! (*pSlave)->mLiving )
			{
				(*pSlave)->Relive(mParentMap,X(),Y(),Dir());
			}
			else
			{
				(*pSlave)->ForceMove(X(),Y(),Dir());
			}
		}
	}

	int DomainPlay::GenSlaveStatue(int mon_id, int x, int y, const char* flags)
	{
		MonDef* md = mGSL->GetMonDefManager()->FindMonDef(mon_id);
		if (md == NULL)
		{
			return 0;
		}
		// 清理一遍
		std::vector<PlaySlave *>::iterator it = mSlaveList.begin();
		while (it != mSlaveList.end())
		{
			if( *it )
			{
				if( (*it)->mParentMap == NULL )
				{
					delete (*it);(*it) = 0;
					it = mSlaveList.erase(it);
					continue;
				}
			}
			else
			{
				it = mSlaveList.erase(it);
				continue;
			}
			++ it;
		}

		PlaySlaveStatueDummy * pSlave = new PlaySlaveStatueDummy;
		pSlave->mOwner = this;
		pSlave->Name(md->name);
		//雕像名字不显示归属
		pSlave->mNameShowOwner = false;
		pSlave->mAiType = 5;
		char aii[256];aii[0]=0;
		char opt[1024];opt[0]=0;
		sscanf_s(md->ai.c_str(),"%[^|]|%s",aii,_countof(aii),opt,_countof(opt));
		mGSL->GetMapManager()->LoadAIObject(md,pSlave,opt);

		char flag[256];
		int temp_int =0;
		int flag_len=0;
		int reliveDir=0;

		memset(flag,0,256);
		for(size_t i=0;i<=strlen(flags);i++)
		{
			if( flags[i] == ',' || flags[i] == 0 )
			{
				
				if( 1 == sscanf_s(flag,"dirfixed(%d)",&temp_int) )
				{
					reliveDir=temp_int;
				}
				memset(flag,0,256);
				flag_len=0;
				continue;
			}
			else
			{
				flag[flag_len++] = flags[i];
			}
		}

		mSlaveList.push_back(pSlave);
		if( pSlave )
		{
			pSlave->Dir(reliveDir);
			pSlave->mPatrolRange = md->patrol_range;
			pSlave->mReliveX = x;
			pSlave->mReliveY = y;
			if( ! (pSlave->mLiving) )
			{
				pSlave->Relive(mParentMap, pSlave->mReliveX,pSlave->mReliveY,reliveDir);
			}
			else
			{
				pSlave->ForceMove(pSlave->mReliveX,pSlave->mReliveY,reliveDir);
			}
		}
		return pSlave->ID();
	}

	PlaySlave* DomainPlay::GetSlaveByID(int id)
	{
		std::vector<PlaySlave *>::iterator it = mSlaveList.begin();
		while (it != mSlaveList.end())
		{
			if( *it )
			{
				if( (*it)->mParentMap == NULL )
				{
					delete (*it);(*it) = 0;
					it = mSlaveList.erase(it);
					continue;
				}
			}
			else
			{
				it = mSlaveList.erase(it);
				continue;
			}
			if ((*it)->ID() == id)
			{
				return (*it);
			}
			++ it;
		}
		return NULL;
	}

	int DomainPlay::gen_slave_fashi(int mon_id, int x, int y, const char* flags)
	{
		MonDef* md = mGSL->GetMonDefManager()->FindMonDef(mon_id);
		if (md == NULL)
		{
			return 0;
		}

		// 清理一遍
		std::vector<PlaySlave *>::iterator it = mSlaveList.begin();
		while (it != mSlaveList.end())
		{
			if( *it )
			{
				if( (*it)->mParentMap == NULL )
				{
					delete (*it);(*it) = 0;
					it = mSlaveList.erase(it);
					continue;
				}
			}
			else
			{
				it = mSlaveList.erase(it);
				continue;
			}
			++ it;
		}

		PlaySlaveFashiDummy * pSlave = new PlaySlaveFashiDummy;
		pSlave->mOwner = this;
		pSlave->Name(md->name);

		pSlave->mAiType = 5;
		char aii[256];aii[0]=0;
		char opt[1024];opt[0]=0;
		sscanf_s(md->ai.c_str(),"%[^|]|%s",aii,_countof(aii),opt,_countof(opt));
		mGSL->GetMapManager()->LoadAIObject(md,pSlave,opt);
		mSlaveList.push_back(pSlave);

		char flag[256];
		int flag_len=0;
		memset(flag,0,256);
		for(size_t i=0;i<=strlen(flags);i++)
		{
			if( flags[i] == ',' || flags[i] == 0 )
			{

				if( 0 == strcmp(flag,"nomove") )
				{
					pSlave->SetNoMove(true);
				}
				if ( 0 == strcmp(flag, "nofire"))
				{
					pSlave->SetNoFire(true);
				}
				memset(flag,0,256);
				flag_len=0;
				continue;
			}
			else
			{
				flag[flag_len++] = flags[i];
			}
		}

		if (x == 0 && y == 0)
		{
			x = X();
			y = Y();
		}
		if( pSlave )
		{
			pSlave->mPatrolRange = md->patrol_range;
			pSlave->mReliveX = x;
			pSlave->mReliveY = y;
			if( ! (pSlave->mLiving) )
			{
				pSlave->Relive(mParentMap, pSlave->mReliveX,pSlave->mReliveY,Dir());
			}
			else
			{
				pSlave->ForceMove(pSlave->mReliveX,pSlave->mReliveY,Dir());
			}
		}
		return pSlave->ID();
	}
	/*void DomainPlay::gen_slave(int id,std::string mon_name,int ai_type)
	{
		PlaySlave ** pSlave = NULL;

		if( id >= 0 && id< 5 )
			pSlave = &mSlaves[id];
		else if (id == 10)
			pSlave = &mSlaveHuwei;
		else
			return;
	
		if( *pSlave )
		{
			if( (*pSlave)->mParentMap )
			{
				(*pSlave)->mParentMap->RemoveObject((*pSlave));
			}
			delete (*pSlave);(*pSlave) = 0;
		}
		if( (*pSlave) == 0 )
		{
			MonDef* md = mGSL->GetMonDefManager()->FindMonDef(mon_name);
			if( md )
			{
				(*pSlave) = new PlaySlaveDummy;
				(*pSlave)->mOwner = this;
				(*pSlave)->Name(std::string(md->nodigital_name)+"("+Name()+")");
				
				(*pSlave)->mAiType = ai_type;
				char aii[256];aii[0]=0;
				char opt[1024];opt[0]=0;
				sscanf_s(md->ai.c_str(),"%[^|]|%s",aii,_countof(aii),opt,_countof(opt));
				mGSL->GetMapManager()->LoadAIObject(md,(*pSlave),opt);
			}
		}

		if( (*pSlave) )
		{
			if( ! (*pSlave)->mLiving )
			{
				(*pSlave)->Relive(mParentMap,X(),Y(),Dir());
			}
			else
			{
				(*pSlave)->ForceMove(X(),Y(),Dir());
			}
		}
	}*/

	void DomainPlay::kill_slave(int id)
	{
		if( id >= 0 && id< 5 )
		{
			if( mSlaves[id] )
			{
				mSlaves[id]->DamageHP(this,10000000,0);
			}
		}
		if (id == 10)
		{
			if (mSlaveHuwei)
			{
				mSlaveHuwei->DamageHP(this,10000000,0);
			}
		}
	}

	int DomainPlay::kill_slave_fashi(int id)
	{
		std::vector<PlaySlave *>::iterator it = mSlaveList.begin();
		while (it != mSlaveList.end())
		{
			if (id == (*it)->ID())
			{
				(*it)->DamageHP(this,10000000,0);
				break;
			}
			++ it;
		}
		return 0;
	}

	void DomainPlay::clear_slave_fashi()
	{
		std::vector<PlaySlave *>::iterator it = mSlaveList.begin();
		while (it != mSlaveList.end())
		{
			(*it)->DamageHP(this,10000000,0);
			++ it;
		}
	}

	void DomainPlay::clear_slave()
	{
		if( mSlaveKuLou && mSlaveKuLou->mLiving )
		{
			mSlaveKuLou->DamageHP(this,999999,0);
			changeSlaveState(0);
		}
		if( mSlaveShenShou && mSlaveShenShou->mLiving )
		{
			mSlaveShenShou->DamageHP(this,999999,0);
			changeSlaveState(0);
		}
		if( mSlaveHuwei && mSlaveHuwei->mLiving )
		{
			mSlaveHuwei->DamageHP(this,999999,0);
			changeSlaveState(0);
		}
		for(int i=0;i<5;i++)
		{
			if( mSlaves[i] && mSlaves[i]->mLiving )
			{
				mSlaves[i]->DamageHP(this,999999,0);
			}
		}
	}
	int DomainPlay::have_slave(int id)
	{
		if( id >= 0 && id< 5 )
		{
			if( mSlaves[id] )
			{
				if( mSlaves[id]->mLiving )
				{
					return 1;
				}
			}
		}
		if (id == 10)
		{
			if (mSlaveHuwei && mSlaveHuwei->mLiving)
			{
				return 1;
			}
		}
		return 0;
	}

	void DomainPlay::changeSlaveState(int state)
	{
		mSession->GetListener()->notifySlaveState(mSession,state);
	}
	int DomainPlay::InfoPlayer(const char* pn)
	{
		if( mParentMap->NoInteract() )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_MAP_NO_INFO_PLAYER);
			return 0;
		}
		std::string player_name=pn;
		GameSession* session=mGSL->FindSessionWithCharID(player_name);
		if( session && session->GetPlayer() )
		{
			MiniPlayerInfo mpi;
			memset(&mpi,0,sizeof(mpi));
			session->GetPlayer()->GetMiniPlayerInfo(&mpi);

			if( mpi.item_weapon_id ){PushItemInfo(mpi.item_weapon_id->mTypeID);}
			if( mpi.item_fashion_weapon_id){PushItemInfo(mpi.item_fashion_weapon_id->mTypeID);}
			if( mpi.item_cloth_id ){PushItemInfo(mpi.item_cloth_id->mTypeID);}
			if( mpi.item_fashion_cloth_id ){PushItemInfo(mpi.item_fashion_cloth_id->mTypeID);}
			if( mpi.item_hat_id ){PushItemInfo(mpi.item_hat_id->mTypeID);}
			if( mpi.item_nicklace_id ){PushItemInfo(mpi.item_nicklace_id->mTypeID);}
			if( mpi.item_glove1_id ){PushItemInfo(mpi.item_glove1_id->mTypeID);}
			if( mpi.item_glove2_id ){PushItemInfo(mpi.item_glove2_id->mTypeID);}
			if( mpi.item_ring1_id ){PushItemInfo(mpi.item_ring1_id->mTypeID);}
			if( mpi.item_ring2_id ){PushItemInfo(mpi.item_ring2_id->mTypeID);}
			if( mpi.item_boot_id ){PushItemInfo(mpi.item_boot_id->mTypeID);}
			if( mpi.item_belt_id ){PushItemInfo(mpi.item_belt_id->mTypeID);}
			if( mpi.item_huizhang ){PushItemInfo(mpi.item_huizhang->mTypeID);}
			if(mpi.item_hunqi){PushItemInfo(mpi.item_hunqi->mTypeID);}
			if(mpi.item_wing){PushItemInfo(mpi.item_wing->mTypeID);}
			if(mpi.item_xueshi){PushItemInfo(mpi.item_xueshi->mTypeID);}
			if(mpi.item_zuoji){PushItemInfo(mpi.item_zuoji->mTypeID);}
			if(mpi.item_tejie){PushItemInfo(mpi.item_tejie->mTypeID);}
			mSession->GetListener()->resInfoPlayer(mSession,session->GetPlayer(),&mpi);
		}
		return 0;
	}
	int DomainPlay::GetMiniPlayerInfo(MiniPlayerInfo* info)
	{
		info->model = &mModelMap;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID <= 0 )continue;
			if( pos->mPosition>=0)continue;
			if( pos->mPosition==ITEM_WEAPON_POSITION )
			{
				info->item_weapon_id=&(*pos);
			}
			if( pos->mPosition==ITEM_CLOTH_POSITION )
			{
				info->item_cloth_id=&(*pos);
			}
			if( pos->mPosition==ITEM_HAT_POSITION)
			{
				info->item_hat_id=&(*pos);
			}
			if( pos->mPosition==ITEM_NICKLACE_POSITION )
			{
				info->item_nicklace_id=&(*pos);
			}
			if( pos->mPosition==ITEM_RING1_POSITION )
			{
				info->item_ring1_id=&(*pos);
			}
			if( pos->mPosition==ITEM_RING2_POSITION)
			{
				info->item_ring2_id=&(*pos);
			}
			if( pos->mPosition==ITEM_GLOVE1_POSITION)
			{
				info->item_glove1_id=&(*pos);
			}
			if( pos->mPosition==ITEM_GLOVE2_POSITION)
			{
				info->item_glove2_id=&(*pos);
			}
			if( pos->mPosition==ITEM_BELT_POSITION )
			{
				info->item_belt_id=&(*pos);
			}
			if( pos->mPosition==ITEM_BOOT_POSITION)
			{
				info->item_boot_id=&(*pos);
			}
			if( pos->mPosition == ITEM_MEDAL_POSITION )
			{
				info->item_huizhang=&(*pos);
			}
			if(pos->mPosition==ITEM_HUNQI_POSITION)
			{
				info->item_hunqi=&(*pos);
			}
			if(pos->mPosition==ITEM_XUESHI_POSITION)
			{
				info->item_xueshi=&(*pos);
			}
			if(pos->mPosition==ITEM_WING_POSITION)
			{
				info->item_wing=&(*pos);
			}
			if(pos->mPosition==ITEM_MOUNT_POSITION)
			{
				info->item_zuoji=&(*pos);
			}
			if(pos->mPosition == ITEM_FASHION_WEAPON_POSITION )
			{
				info->item_fashion_weapon_id=&(*pos);
			}
			if(pos->mPosition == ITEM_FASHION_CLOTH_POSITION )
			{
				info->item_fashion_cloth_id =&(*pos);
			}
			if(pos->mPosition == ITEM_SPECIAL_RING_POSITION )
			{
				info->item_tejie =&(*pos);
			}
			if(pos->mPosition == ITEM_XUEFU_POSITION )
			{
				info->item_xuefu =&(*pos);
			}
			if(pos->mPosition == ITEM_HUDUN_POSITION )
			{
				info->item_hudun =&(*pos);
			}
			if(pos->mPosition == ITEM_BAOSHI_POSITION )
			{
				info->item_baoshi =&(*pos);
			}
			if(pos->mPosition == ITEM_HUNZHU_POSITION )
			{
				info->item_hunzhu =&(*pos);
			}
			if(pos->mPosition == ITEM_JADE_PENDANT_POSITION )
			{
				info->item_jade_pendant =&(*pos);
			}
			if(pos->mPosition == ITEM_SHIELD_POSITION )
			{
				info->item_shield =&(*pos);
			}
			if(pos->mPosition == ITEM_MIRROR_ARMOUR_POSITION )
			{
				info->item_mirror_armour =&(*pos);
			}
			if(pos->mPosition == ITEM_FACE_CLOTH_POSITION )
			{
				info->item_face_cloth =&(*pos);
			}
			if(pos->mPosition == ITEM_DRAGON_HEART_POSITION )
			{
				info->item_dragon_heart =&(*pos);
			}
			if(pos->mPosition == ITEM_WOLFANG_POSITION )
			{
				info->item_wolfang =&(*pos);
			}
			if(pos->mPosition == ITEM_DRAGON_BONE_POSITION )
			{
				info->item_dragon_bone =&(*pos);
			}
			if(pos->mPosition == ITEM_CATILLA_POSITION )
			{
				info->item_catilla =&(*pos);
			}
		}
		return 0;
	}
	SubTask* DomainPlay::GetTask(int task_id)
	{
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
	int DomainPlay::CancelTask(int task_id)
	{
		if( IsState(MAP_OBJECT_STATE_DEAD) ) return 0;
		SubTask* ti = GetTask(task_id);
		if( ti && (ti->TaskState >= TASK_STATE_ACCEPTED||ti->TaskState < TASK_STATE_COMPLETED) )
		{
			ti->TaskState = 0;
			PushTaskData(task_id);
		}
		return 0;
	}
	int DomainPlay::QueryTaskDesp(int task_id)
	{
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			{
				mScriptEcho="";
				char ss[512];_snprintf_s(ss,_countof(ss),511,"task.task%d.onGetTaskDesp",task_id);ss[511]=0;
				mGSL->GetScriptEngine()->OnlyPlayer(this,ss);
				pos->second.TaskDesp=mScriptEcho;
				//strncpy(pos->second.TaskDesp,mScriptEcho.c_str(),511);
				//pos->second.TaskDesp[511]=0;
				mScriptEcho="";
			}
			mSession->GetListener()->resTaskDesp(mSession,task_id,pos->second.TaskState,pos->second.TaskDesp.c_str());
		}
		return 0;
	}
	int DomainPlay::TaskClick(int task_id,const char* param)
	{
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			char ss[512];_snprintf_s(ss,_countof(ss),511,"task.task%d.onClick",task_id);ss[511]=0;
			mGSL->GetScriptEngine()->OnlyPlayer(this,ss);
		}
		return 0;
	}
	int DomainPlay::NPCShowFlags(unsigned int npc_id,int flags)
	{
		mSession->GetListener()->notifyNPCShowFlags(mSession,npc_id,flags);
		return 1;
	}
	bool DomainPlay::checkCreateGuild()
	{
		mScriptEcho="";
		mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this,"player.checkCreateGuild");
		if( mScriptEcho == "true" )
		{
			return true;
		}
		return false;
	}
	bool DomainPlay::checkJoinGuild()
	{
		mScriptEcho = "";
		mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this, "player.checkJoinGuild");
		if (mScriptEcho == "true")
		{
			return true;
		}
		return false;
	}
	bool DomainPlay::checkUseFashion(int type_id)
	{
		mScriptEcho = "";
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
		if (id)
		{
			mGSL->GetScriptEngine()->ItemPlayer(this, id->mName.c_str(), id->mTypeID, "player.onuse_fashion");
		}
		if (mScriptEcho == "true")
		{
			return true;
		}
		return false;
	}
	void DomainPlay::ReallyCreateGuild()
	{
		mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this, "player.ReallyCreateGuild");
	}
	void DomainPlay::conditionCreateGuild(std::string& conditional)
	{
		mScriptEcho="";
		mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this,"player.conditionCreateGuild");
		conditional = mScriptEcho;
	}
	//bool DomainPlay::checkDirectFly()
	//{
	//	mScriptEcho="";
	//	mSession->GetGameServerLib()->GetScriptEngine()->OnlyPlayer(this,"player.checkDirectFly");
	//	if( mScriptEcho == "true" )
	//	{
	//		return true;
	//	}
	//	return false;
	//}
	void DomainPlay::findRoadGoto(const char* map,int x,int y,const char* target,int flags)
	{
		mSession->GetListener()->notifyFindRoadGoto(mSession,map,x,y,target,flags);
	}
	void DomainPlay::pushGroupInfo(const std::string& group_name,const std::string& group_leader)
	{
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* player = (DomainPlay*)(*pos);
					player->NotifyGroupInfo(this, group_name, group_leader);
				}
			}
		}
	}
	void DomainPlay::NotifyGroupInfo(DomainPlay* src,const std::string& group_name,const std::string& group_leader)
	{
		mSession->GetListener()->notifyGroupInfo(mSession,src,group_name,group_leader);
	}
	void DomainPlay::NotifyTradeInvite(DomainPlay* src)
	{
		mSession->GetListener()->notifyTradeInvite(mSession,src);
	}
	void DomainPlay::NotifyPKStateChange(DomainPlay* src)
	{
		mSession->GetListener()->notifyPKStateChange(mSession,src);
	}
	void DomainPlay::NotifyPlayerAddinfo(DomainPlay* src)
	{
		mSession->GetListener()->notifyPlayerAddInfo(mSession,src);
	}
	void DomainPlay::NotifyAttributeChange()
	{
		mSession->GetListener()->notifyAttributeChange(mSession);
	}
	void DomainPlay::NotifyPlayEffect(PlayMapObject* src,int skill_type,int x,int y,unsigned int targetID,unsigned int ownerID,int resID)
	{
		mSession->GetListener()->notifyPlayEffect(mSession,src,skill_type,x,y,targetID,ownerID,resID);
	}
	void DomainPlay::NotifyMapOption(PlayMap* map)
	{
		mSession->GetListener()->notifyMapOption(mSession,map);
	}
	int DomainPlay::RefreshNpcShowFlagsInview()
	{
		for( NpcList::iterator pos = mNpcList.begin();
			pos != mNpcList.end();
			++ pos )
		{
			(*pos)->RefreshShowFlags(this);
		}
		return 1;
	}
	PlayNpc* DomainPlay::FindNPCInview(int id)
	{
		for( NpcList::iterator pos = mNpcList.begin();
			pos != mNpcList.end();
			++ pos )
		{
			if((*pos)->ID() == id )
			{
				return *pos;
			}
		}
		return 0;
	}
	void DomainPlay::UpdateTaskName(int task_id)
	{
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			mScriptEcho="";
			char ss[512];_snprintf_s(ss,_countof(ss),511,"task.task%d.onGetTaskName",task_id);ss[511]=0;
			mGSL->GetScriptEngine()->OnlyPlayer(this,ss);
			pos->second.TaskName=mScriptEcho;
			//strncpy(pos->second.TaskName,mScriptEcho.c_str(),127);
			//pos->second.TaskName[127]=0;
			mScriptEcho="";
		}
		{
			mScriptEcho="";
			char ss[512];_snprintf_s(ss,_countof(ss),511,"task.task%d.onGetTaskShortDesp",task_id);ss[511]=0;
			mGSL->GetScriptEngine()->OnlyPlayer(this,ss);
			pos->second.TaskShortDesp=mScriptEcho;
			//strncpy(pos->second.TaskShortDesp,mScriptEcho.c_str(),255);
			//pos->second.TaskShortDesp[255]=0;
			mScriptEcho="";
		}
	}
	//void DomainPlay::AwardLoginForm()
	//{
	//	mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onAwardLoginForm");
	//}
	int DomainPlay::GetAchieveState(int achieve_id)
	{
		AchieveMap::iterator pos = mAchieveMap.find(achieve_id);
		if( pos != mAchieveMap.end() )
		{
			return pos->second.mState;
		}
		return 0;
	}
	int DomainPlay::SetAchieveState(int achieve_id,int state)
	{
		int ret = 0;
		int param = 0;
		AchieveMap::iterator pos = mAchieveMap.find(achieve_id);
		if( pos != mAchieveMap.end() )
		{
			pos->second.mState = state;
			param = pos->second.mParam;
			ret = pos->second.mState;
		}
		else
		{
			SubAchieve ti;
			ti.mAchieveID = achieve_id;
			ti.mState = state;
			ti.mParam = 0;
			mAchieveMap[achieve_id] = ti;
			param = ti.mParam;
			ret = ti.mState;
		}
		mScriptEcho = "";
		mGSL->GetScriptEngine()->PlayerCongzi(this,achieve_id,ret,param,"achieve.onAchieveStateChange");
		if( ! mScriptEcho.empty() )
		{
			mPlayerTalkSeed = rand();
			mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
		}
		mScriptEcho = "";
		return ret;
	}
	int DomainPlay::AddOneAchieveState(int achieve_id)
	{
		int ret = 0;
		int param = 0;
		AchieveMap::iterator pos = mAchieveMap.find(achieve_id);
		if( pos != mAchieveMap.end() )
		{
			pos->second.mState ++;
			param = pos->second.mParam;
			ret = pos->second.mState;
		}
		else
		{
			SubAchieve ti;
			ti.mAchieveID = achieve_id;
			ti.mState = 1;
			ti.mParam = 0;
			mAchieveMap[achieve_id] = ti;
			param = ti.mParam;
			ret = ti.mState;
		}
		mScriptEcho = "";
		mGSL->GetScriptEngine()->PlayerCongzi(this,achieve_id,ret,param,"achieve.onAchieveStateChange");
		if( ! mScriptEcho.empty() )
		{
			mPlayerTalkSeed = rand();
			mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
		}
		mScriptEcho = "";
		return ret;
	}
	int DomainPlay::GetAchieveParam(int achieve_id)
	{
		AchieveMap::iterator pos = mAchieveMap.find(achieve_id);
		if( pos != mAchieveMap.end() )
		{
			return pos->second.mParam;
		}
		return 0;
	}
	int DomainPlay::GetAchievePoint()
	{
		return mAchievePoint;
	}
	int DomainPlay::AddAchievePoint(int ap)
	{
		mAchievePoint += ap;

		if(this->GetAchievePoint() > 0 )
		{
			mGSL->GetChartManager()->UpdateData(CHART_TYPE_ACHIEVE, Name(), GetAchievePoint(), mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
		}
		return mAchievePoint;
	}
	int DomainPlay::AddOneAchieveParam(int achieve_id)
	{
		AchieveMap::iterator pos = mAchieveMap.find(achieve_id);
		if( pos != mAchieveMap.end() )
		{
			pos->second.mParam ++;
			return pos->second.mParam;
		}
		else
		{
			SubAchieve ti;
			ti.mAchieveID = achieve_id;
			ti.mState = 0;
			ti.mParam = 1;
			mAchieveMap[achieve_id] = ti;
			return ti.mParam;
		}
		return 0;
	}
	int DomainPlay::SetAchieveParam(int achieve_id,int param)
	{
		AchieveMap::iterator pos = mAchieveMap.find(achieve_id);
		if( pos != mAchieveMap.end() )
		{
			pos->second.mParam = param;
			return pos->second.mParam;
		}
		else
		{
			SubAchieve ti;
			ti.mAchieveID = achieve_id;
			ti.mState = 0;
			ti.mParam = param;
			mAchieveMap[achieve_id] = ti;
			return ti.mParam;
		}
		return 0;
	}
	void DomainPlay::GMSetTaskState(int taskid,int id,int state)
	{
		SetTaskState(taskid,id*10+state);
		PushTaskData(taskid,0);
	}
	int DomainPlay::SetTaskState(int task_id,int task_state)
	{
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			pos->second.TaskState = task_state;
			UpdateTaskName(task_id);
			return pos->second.TaskState;
		}
		else
		{
			SubTask ti;
			ti.TaskID = task_id;
			ti.TaskState = task_state;
			ti.TaskParam1 = 0;
			ti.TaskParam2 = 0;
			ti.TaskParam3 = 0;
			ti.TaskParam4 = 0;
			mTaskMap[task_id] = ti;
			UpdateTaskName(task_id);
			return ti.TaskState;
		}
		return 0;
	}
	int DomainPlay::SetTaskParam(int task_id,int param_id,int param)
	{
		if( param_id > 4 || param_id < 1 )
		{
			return 0;
		}
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			if( param_id == 1 )
			{
				pos->second.TaskParam1 = param;
				return pos->second.TaskParam1;
			}
			if( param_id == 2 )
			{
				pos->second.TaskParam2 = param;
				return pos->second.TaskParam2;
			}
			if( param_id == 3 )
			{
				pos->second.TaskParam3 = param;
				return pos->second.TaskParam3;
			}
			if( param_id == 4 )
			{
				pos->second.TaskParam4 = param;
				return pos->second.TaskParam4;
			}
		}
		return 0;
	}
	int DomainPlay::GetTaskState(int task_id)
	{
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			return pos->second.TaskState;
		}
		return 0;
	}
	int DomainPlay::GetTaskParam(int task_id,int param_id)
	{
		if( param_id > 4 || param_id < 1 )
		{
			return 0;
		}
		TaskMap::iterator pos = mTaskMap.find(task_id);
		if( pos != mTaskMap.end() )
		{
			if( param_id == 1 )
			{
				return pos->second.TaskParam1;
			}
			if( param_id == 2 )
			{
				return pos->second.TaskParam2;
			}
			if( param_id == 3 )
			{
				return pos->second.TaskParam3;
			}
			if( param_id == 4 )
			{
				return pos->second.TaskParam4;
			}
		}
		return 0;
	}
	void DomainPlay::Update(double ti)
	{
		//printf("DomainPlay Update, %f\n",ti);
		PERF_NODE_FUNC();
		if( mSecondTick + mSecondTickGap < ti )
		{
			PERF_NODE("mSecondTick");
			mSecondTick = ti;
			if( mNeedClearItemList )
			{
				EmptyItemRemover eir;
				GetItemList().erase(std::remove_if(GetItemList().begin(),GetItemList().end(),eir),GetItemList().end());
				mNeedClearItemList = false;
			}
			if(mNeedClearChatTradeItemList)
			{
				EmptyItemRemover eir;
				GetChatTradeItemList().erase(std::remove_if(GetChatTradeItemList().begin(),GetChatTradeItemList().end(),eir),GetChatTradeItemList().end());
				mNeedClearChatTradeItemList = false;
			}
			if( IsDead() )
			{
				if( mParentMap && mParentMap->AutoAlive()==1 )
				{
					Relive(100,0,0,0);
					return;
				}
			}
			if( ! IsDead() )
			{
				PERF_NODE("UpdateHPMP");
				UpdateHPMP(ti);
			}
			{
				PERF_NODE("mTimer");
				for( TimerMap::iterator pos = mTimer.begin();
					pos != mTimer.end();
					++ pos )
				{
					if( pos->second > 0.0 && ti > pos->second )
					{
						pos->second = 0.0;
						char ss[512];_snprintf_s(ss,_countof(ss),511,"player.onTimer%d",(pos->first+1));ss[511]=0;
						mGSL->GetScriptEngine()->OnlyPlayer(this,ss);
					}
				}
			}
			if( mNoticeTick + mGSL->mNoticeGap < ti )
			{
				PERF_NODE("mNoticeTick");
				mNoticeTick = ti;
				std::string& str = mGSL->GetNoticeManager()->get_notice(mNoticeID);
				if(! str.empty() )
				{
					AlertMessage(10,0,str.c_str());
				}
				mNoticeID ++;
			}
			if( mTiliRecoverTick + mGSL->mTiliRecoverGap < ti )
			{
				mTiliRecoverTick=ti;
				if(mTili < 100)
				{
					mTili = min(100,mTili+1);
					//Session()->GetListener()->notifyTiliChange(Session(),mTili);
				}
			}

			// 检测好友申请是否过期
			FriendApplyMap::iterator it = mFriendApplyMap.begin();
			while (it != mFriendApplyMap.end())
			{
				if (it->second + mGSL->mFriendApplyTimeOut < mGSL->GetTimerFix()->Elapsed())
				{
					it = mFriendApplyMap.erase(it);
					continue;
				}
				++ it;
			}

			//检测预存技能是否过期
			CheckLieHuoValid();

			//内功值恢复相关代码
			if( ! IsDead() )
			{
				if (mPowerRecoverTick + mPowerRecoverGap < ti)
				{
					mPowerRecoverTick = ti;
					if (mPower < Attributes()->mMaxPower)
					{
						mPower += mPowerRecoverVal;
						if (mPower > Attributes()->mMaxPower)
							mPower = Attributes()->mMaxPower;

						NotifyTiliChange(this);
						for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
							pos != mVisibleInteractiveObjectList.end();
							++ pos )
						{
							(*pos)->NotifyTiliChange(this);
						}
					}
				}
			}
			
			if( mPKState > 0 )
			{
				PERF_NODE("SetPKState");
				if( mPKStateTick + 30.0 < ti )
				{
					SetPKState(0);
				}
			}
			if( mPKValue > 0 )
			{
				PERF_NODE("SubPKValue");
				if( mPKValueTick + 60.0 < ti )
				{
					int i = 1;
					/*if( IsShutPKValue() )
					{
						i = i * StatusParam(STATUS_TYPE_SHUT_PK_VALUE);
					}*/
					if (SafeArea() < 0)
					{
						i = 3;
					}
					SubPKValue(i);
				}
			}
			if( mSaveTick + 60.0*mGSL->mSessionSaveGap < ti )
			{
				PERF_NODE("mSaveTick");
				mSaveTick = ti;
				mSession->SaveSession();
			}
			if( mUpdateChartGamemoneyTimeTick + 120 < time(0) )
			{
				mUpdateChartGamemoneyTimeTick = time(0);
				if( mUpdateChartGamemoneyTick != gamemoney() )
				{
					mUpdateChartGamemoneyTick = gamemoney();
					mGSL->GetChartManager()->UpdateData(CHART_TYPE_GAMEMONEY, Name(), (int)gamemoney(), mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
				}
			}
			if( china_limit_lv_tick + 5 < time(0) )
			{
				china_limit_lv_tick = time(0);
				FreshOnlineTime();
			}
			if( mFreshItemTime + mGSL->mFreshTimeLimitItemGap < time(0) )
			{
				if( mNeedFreshItemList ){ FreshTimeItem(time(0));}
				mFreshItemTime = time(0);
			}
		}
		if( mMiniTick + mMiniTickGap < ti )
		{
			PERF_NODE("mMiniTick");
			mMiniTick = ti;
			if( mSlaveKuLou && mSlaveKuLou->mLiving )
			{
				mSlaveKuLou->Update(ti);
			}

			if (mSlaveHuwei && mSlaveHuwei->mLiving)
			{
				mSlaveHuwei->Update(ti);
			}

			if( mSlaveShenShou && mSlaveShenShou->mLiving )
			{
				mSlaveShenShou->Update(ti);
			}
			for( int i =0;i<5;i++)
			{
				if( mSlaves[i] && mSlaves[i]->mLiving )
				{
					mSlaves[i]->Update(ti);
				}
			}
			for (size_t i = 0; i < mSlaveList.size(); ++ i)
			{
				if( mSlaveList[i] && mSlaveList[i]->mLiving )
				{
					mSlaveList[i]->Update(ti);
				}
			}
			{
				PERF_NODE("UpdateStatusTimer");
				UpdateStatusTimer(ti);
				UpdateBuffTimer(ti);
			}
			if( mChangeMountTick > 0 && mChangeMountTick < ti )
			{
				mChangeMountTick=0;
				set_showmount(1);
			}
			if( mCaijiTick > 0 && mCaijiTick < ti )
			{
				mCaijiTick=0;
				EndCollect();
			}
		}
	}

	void DomainPlay::lock_action_end()
	{
		if( mLockAction > 0 )
		{
			mLockAction = 0;
			if( ! mLockActionCallback.empty() )
			{
				mGSL->GetScriptEngine()->OnlyPlayer(this,mLockActionCallback.c_str());
				mLockActionCallback="";
			}
		}
	}
	void DomainPlay::lock_action(int lockaction,const char* script)
	{
		lock_action_end();
		mLockAction = lockaction;
		mLockActionCallback = script;
		if(mLockAction>=0 )
		{
			notifyDoAction(this,mLockAction);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->notifyDoAction(this, mLockAction);
				}
			}
		}
	}
	void DomainPlay::set_showmount(int i)
	{
		if(mShowMount==i)return ;
		mShowMount=i;
		mUpdateSpeed=1.0f;
		if(mShowMount) mUpdateSpeed=mGSL->GetItemDefineManager()->GetItemSpeed(mMountTypeID);
		notifySpeed(this,mUpdateSpeed);
		NotifyAvatarChange(this);
		//mGSL->GetScriptEngine()->OnlyPlayer(this,"newgui.self.fresh_self_mountbtn");
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyAvatarChange(this);
				(*pos)->notifySpeed(this, mUpdateSpeed);
			}
		}
	}
	int DomainPlay::LoadPostMail()
	{
		//for(int i=0;i<mGSL->GetMailManager()->GetMails().size();i++)
		std::map<LEUD_INT64,int> vaildmail;
		for(auto pos = mGSL->GetMailManager()->GetMails().begin(); pos != mGSL->GetMailManager()->GetMails().end(); ++pos)
		{
			//Mail* mail = mGSL->GetMailManager()->GetMails()[i];
			Mail* mail = (*pos);
			if( mail->mDate > mCreateTime )
			{
				if (mail->mOwners == "verify")
				{
					vaildmail[mail->mID]=1;
					int ret = mGSL->GetScriptEngine()->PlayerString(this, "player.onCheckPostMail", mail->mTitle.c_str());
					if (ret == 1)
					{
						MailMap::iterator iter = mMailMap.find(mail->mID);
						if( iter == mMailMap.end() )
						{
							SubMail sb;
							sb.BuildMail(mail,mail->mID);
							mMailMap[mail->mID] = sb;
						}
					}
				}
                else if (mail->mOwners == "all" || mail->mOwners == mSession->SeedID()) 
				{

					vaildmail[mail->mID]=1;

					MailMap::iterator iter = mMailMap.find(mail->mID);
					if( iter == mMailMap.end() )
					{
						SubMail sb;
						sb.BuildMail(mail,mail->mID);
						mMailMap[mail->mID] = sb;
					}
				}
			}
		}
		for(auto submail = mMailMap.begin();submail!=mMailMap.end();)
		{
			std::map<LEUD_INT64,int>::iterator iter = vaildmail.find(submail->second.mID);
			if(iter==vaildmail.end())
			{
				submail = mMailMap.erase(submail);
			}
			else
			{
				submail++;
			}
		}

		int noread = 0;
		for( MailMap::iterator it = mMailMap.begin();
			it!= mMailMap.end();
			++it)
		{
            if (it->second.mDeleted == 0 && it->second.mRead == 0) {
				noread ++;
			}
		}
		NotifyNewMailNum(noread);
		return 0;
	}
	void DomainPlay::ReadMail(LEUD_INT64 id)
	{
		MailMap::iterator iter = mMailMap.find(id);
		if( iter != mMailMap.end() )
		{
			iter->second.mRead = 1;
		}
	}
	int DomainPlay::RecieveMailItem(LEUD_INT64 id)
	{
		MailMap::iterator pos = mMailMap.find(id);
		if( pos != mMailMap.end() )
		{
			int num = 0;
			if( pos->second.mDeleted == 0 ){
				if( pos->second.mReceived == 0 ){
					for (std::map<int,int>::iterator iter = pos->second.mItems.begin();
						iter != pos->second.mItems.end();
						++ iter)
					{
						if(iter->first != ITEM_GOLD_ID && iter->first != ITEM_EXP_ID
							&& iter->first != ITEM_GOLD_BIND_ID && iter->first != ITEM_VCOIN_ID && iter->first != ITEM_VCOIN_BIND_ID)
						{
							//ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(iter->first);if(!id)continue;
							int stackmax = mSession->GetGameServerLib()->GetItemDefineManager()->GetItemStackMax(iter->first);
							int needNum = iter->second / stackmax;
							if(iter->second % stackmax > 0)
							{
								num += needNum + 1;
							}
							else
							{
								num += needNum;
							}
						}
					}
					if ( NumBagBlack() < num )
					{
						char temp[512];_snprintf_s(temp,_countof(temp),511,mGSL->mStringConsts.NOTIFY_BAG_BLANK_NOT_ENOUGH,num);temp[511]=0;
						AlertMessage(1,0,temp);
						return 0;
					}
					for (std::map<int,int>::iterator pos2 = pos->second.mItems.begin();
						pos2 != pos->second.mItems.end();
						++pos2)
					{
						if( pos2->first == ITEM_GOLD_ID ){
							AddGameMoney(pos2->second,600);continue;
						}
						if( pos2->first == ITEM_GOLD_BIND_ID ){
							AddGameMoneyBind(pos2->second,600);continue;
						}
						if( pos2->first == ITEM_VCOIN_ID ){
							if(pos->second.mGS){
								AddVcoinGS(pos2->second,600);
							}else{
								AddVcoin(pos2->second,600);
							}
							continue;
						}
						if( pos2->first == ITEM_VCOIN_BIND_ID ){
							AddVcoinBind(pos2->second,600);continue;
						}
						if( pos2->first == ITEM_EXP_ID ){
							AddExperience(pos2->second,600);continue;
						}

						ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos2->first);
						if( id )
						{
							SubItem ii;
							ii.BuildItemInstance(id,1,false,mGSL->mItemAddType);
							AddItem(&ii,pos2->second,600,11,0);
						}
					}
					pos->second.mReceived = 1;
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_MAIL_RECEIVE_SUC);
					mGSL->GetScriptEngine()->PlayerString(this, "player.onGetMailAward", pos->second.mTitle.c_str());
					NotifyMailReceiveSuccess(pos->second.mID);
					return 1;
				}
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_MAIL_HAVE_RECEIVED);
				return 0;
			}
			return 0;
		}
		return 0;
	}
	void DomainPlay::DeleteMail(LEUD_INT64 id)
	{
		//printf("delete %d\n",id);
		MailMap::iterator pos = mMailMap.find(id);
		if( pos != mMailMap.end() )
		{
			//if(pos->second.mReceived == 0){
			//	AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_MAIL_DELETE_UNRECEIVED);return;
			//}
			pos->second.mDeleted = 1;
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_MAIL_DELETE_SUC);
		}
	}
	void DomainPlay::UpdateStatusTimer(double time)
	{
		PlayActiveObject::UpdateStatusTimer(time);
		if(IsStatus(STATUS_TYPE_MOXUESHI))
		{
			if( mYuanqiBuffTick + StatusTickGap(STATUS_TYPE_MOXUESHI) < time)
			{
				mYuanqiBuffTick = time;
				if(Hp()!=MaxHp()||Mp()!=MaxMp())
				{
					int power = atoi(Param(1019).c_str());//魔血石剩余能量
					
					int _hp = 100;//每秒最多回100点血
					int _mp = 300;//每秒最多回300点魔
					if (MaxHp() - Hp() < 100)
					{
						_hp = MaxHp() - Hp();
					}
					if (MaxMp() - Mp() < 300)
					{
						_mp = MaxMp() - Mp();
					}
					int hp_change = 0;
					int mp_change = 0;
					if (power > 0)
					{
						if (power > _hp)
						{
							AddHP(_hp);
							power = power - _hp;
							hp_change = _hp;
						}
						else
						{
							AddHP(power);
							hp_change = power;
							power = 0;
						}
					}

					if (power > 0)
					{
						if (power > _mp)
						{
							AddMP(_mp);
							power = power - _mp;
							mp_change = _mp;
						}

						else
						{
							AddMP(power);
							mp_change = power;
							power = 0;
						}
					}
					char ss[256];
					itoa(power, ss, 10);
					std::string value(ss);
					Param(1019, value);
					NotifyHPMPChange(this,hp_change,mp_change);
					if (power <= 0)
					{
						ClearStatus(STATUS_TYPE_MOXUESHI);///魔血石能量不足清除该BUFF
					}
				}
			}
		}
	}

	void DomainPlay::UpdateHPMPInc(double time)
	{
		PlayActiveObject::UpdateHPMPInc(time);
		if( mEquipMoXueShi )
		{
			SubItem* sb = get_item_by_pos(ITEM_XUESHI_POSITION);if(!sb){return;}
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(sb->mTypeID);if(!id){return;}
			int inc_gap = id->mDC;
			int up_limit = id->mDCMax;
			int add_hp = id->mAC * level()/100 + id->mACMax;
			int add_mp = id->mMAC * level()/100 + id->mMACMax;

			if(mUpdateHPMPStatusInc + inc_gap <= time)
			{
				mUpdateHPMPStatusInc = time;
				int ok = 0;
				int hp = min(min(sb->mDuration,add_hp),MaxHp()-Hp());
				if( Hp() < (MaxHp() * up_limit/100) && AddHP(hp))
				{
					ok = 1;
					mUpdateHP += hp;
					sb->mDuration -= hp;
					if( sb->mDuration <= 0 ){sb->mDuration = 0;}
				}
				int mp = min(min(sb->mDuration,add_mp),MaxMp()-Mp());
				if(sb->mDuration > 0 && Mp() < (MaxMp()*up_limit/100) && AddMP(mp))
				{
					ok = 1;
					mUpdateMP += mp;
					sb->mDuration -= mp;
					if( sb->mDuration <= 0 ){sb->mDuration = 0;}
				}
				if(ok == 1 )
				{
					if(sb->mDuration <= 0)
					{
						sb->mTypeID = 0;
						mNeedClearItemList = true;
						AlertMessage(11,0,mGSL->mStringConsts.NOTIFY_MOXUESHI_EMPTY);
					}
					mSession->GetListener()->notifyItemChange(mSession,*sb,0);
				}
			}
		}
	}
	void DomainPlay::FreshOnlineTime()
	{
		if( mCharacter )
		{
			int logincontinuemax = mCharacter->mLoginDayContinueMax;
			int logincount = mCharacter->mLoginDayCount;
			mCharacter->freshonlinetime();
			if( mCharacter->mOnlineTimeThis >= mOnlineTimeMax + 60 )
			{
				mOnlineTimeMax = mCharacter->mOnlineTimeThis - mCharacter->mOnlineTimeThis % 60;
				mScriptEcho = "";
				mGSL->GetScriptEngine()->PlayerCongzi(this,int(mOnlineTimeMax/60),0,0,"achieve.onAchieveNewOnlineTimeMax");
				if( ! mScriptEcho.empty() )
				{
					mPlayerTalkSeed = rand();
					mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
				}
				mScriptEcho = "";
			}
			if( mCharacter->mLoginDayContinueMax > logincontinuemax )
			{
				mScriptEcho = "";
				mGSL->GetScriptEngine()->PlayerCongzi(this,mCharacter->mLoginDayContinueMax,0,0,"achieve.onAchieveNewContinueLoginDay");
				if( ! mScriptEcho.empty() )
				{
					mPlayerTalkSeed = rand();
					mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
				}
				mScriptEcho = "";
			}
			if( mCharacter->mLoginDayCount > logincount )
			{
				mScriptEcho = "";
				mGSL->GetScriptEngine()->PlayerCongzi(this,mCharacter->mLoginDayCount,0,0,"achieve.onAchieveNewLoginDay");
				if( ! mScriptEcho.empty() )
				{
					mPlayerTalkSeed = rand();
					mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
				}
				mScriptEcho = "";
			}
			//FreshChinaLimit();
		}
	}
	void DomainPlay::NotifyCountDown(int delay,std::string& msg)
	{
		mCountDownTick = static_cast<int>(time(0)+delay);
		mCountDownTag = 1;
		mSession->GetListener()->notifyCountDown(mSession,delay,msg);
	}
	int  DomainPlay::SetMapTimer(int i,int esp)
	{
		if( i >=1 )
		{
			mTimer[i-1] = mGSL->GetTimerFix()->Elapsed() + esp/1000.0;
			if( esp == 0 )
			{
				mTimer[i-1] = 0;
			}
			return i;
		}
		return 0;
	}
	double DomainPlay::GetMapTimer(int i)
	{
		double time = 0;
		if (i >= 1)
		{
			if (mTimer[i - 1] >= mGSL->GetTimerFix()->Elapsed())
			{
				time = mTimer[i - 1] - mGSL->GetTimerFix()->Elapsed();
			}
			else
			{
				time = 0;
			}
			return time;
		}
		return 0;
	}
	void DomainPlay::OnLeaveSafeArea()
	{
		mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onLeaveSafeArea");//添加脚本调用
		//AlertMessage(100,0,mGSL->mStringConsts.NOTIFY_SAFEAREA_LEAVE);
		mPKValueTick = mGSL->GetTimerFix()->Elapsed(); //重置pk值减少的倒计时
	}
	void DomainPlay::OnEnterSafeArea(int sa)
	{
		mGSL->GetScriptEngine()->OnlyPlayer(this, "player.onEnterSafeArea");//添加脚本调用
		//AlertMessage(100,0,mGSL->mStringConsts.NOTIFY_SAFEAREA_ENTER);
		mLastSafeAreaID = sa;
		mLastSafeMap = mParentMap->GetID();
		mLastSafeX = X();
		mLastSafeY = Y();

		mPKValueTick = mGSL->GetTimerFix()->Elapsed(); //重置pk值减少的倒计时
	}
	void DomainPlay::OnEnterEventArea(std::string id)
	{
		mGSL->GetScriptEngine()->PlayerString(this, "player.onEnterEventArea", id.c_str());
	}
	void DomainPlay::OnLeaveEventArea(std::string id)
	{
		mGSL->GetScriptEngine()->PlayerString(this, "player.onLeaveEventArea", id.c_str());
	}
	void DomainPlay::FreshVcoin()
	{
		mGSL->GetListener()->RefreshVcoinCongzi( mSession->LoginID(),mSession->CharID());
	}
	void DomainPlay::FreshGift()
	{
		mGSL->GetListener()->RefreshGift( mSession->CharID() );
	}
	void DomainPlay::UpdateChinaLimit(const char* id,const char* name,int china_id_ok)
	{
		
	}
	void DomainPlay::OnFreshGift(std::vector<OGSLib::SubGift>* gift)
	{
		bool added = false;
		for(std::vector<OGSLib::SubGift>::iterator gp = gift->begin();
			gp != gift->end();
			++ gp)
		{
			bool find = false;
			for( DomainData::GiftArray::iterator pos = mCharacter->mGiftArray.begin();
				pos != mCharacter->mGiftArray.end();
				++ pos )
			{
				if( pos->mDBID == gp->mDBID )
				{
					find = true;
				}
			}
			if( find == false )
			{
				mCharacter->mGiftArray.push_back(*gp);
				added = true;
			}
		}
		if( added )
		{
			ProcessGift();
		}
	}
	void DomainPlay::OnFreshVcoin(int vc,int accu,bool gs)
	{
		if( vc > 0 )
		{
			int add = vc*mGSL->mVcoinMul;
			if( accu > vc ){accu = vc;}

			bool ret = false;
			if(gs)
			{
				ret = AddVcoinGS(add);
			}
			else
			{
				ret = AddVcoin(add,105);
			}
			if( ret )
			{
				if(accu > 0)
				{
					mVcoinAccu += add;
					mGSL->GetScriptEngine()->PlayerCongzi(this,add,vcoin(),mVcoinAccu,"player.onCongzi");
					//mSession->GetListener()->notifyVipChange(mSession);
				}
			}
			if( mGSL->mInfoOnCongzi > 0 )
			{
				char infomsg[512];
				_snprintf_s(infomsg,_countof(infomsg),511,mGSL->mStringConsts.NOTIFY_CONGZI_INFO,Name(),add);infomsg[511]=0;
				//std::string s(infomsg);
				mGSL->ServerAlertMessage(10,0,infomsg);
			}
			mSession->SaveSession();
		}
	}

	int DomainPlay::ScriptEcho(const char* msg)
	{
		mScriptEcho.append(msg);
		return 0;
	}
	int DomainPlay::TradeInvite(const char* inv)
	{
		if( IsDead() ) return 0;
		std::string invitee=inv;
#ifndef _DEBUG
		if( mSession->AuthType()== 100 )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_CAN_NOT_AUTH_TYPE);
			return 0;
		}
#endif
		if(this->level()<=mGSL->mTradeAllowLevel)
		{
			char ss[512];_snprintf_s(ss,_countof(ss),511,"%d%s",mGSL->mTradeAllowLevel,mGSL->mStringConsts.NOTIFY_TRADE_CAN_NOT_LEVEL);ss[511]=0;
			AlertMessage(1,0,ss);
			return 0;
		}
		if( ! mTradeTarget.empty() )
		{
			CloseTrade(mTradeTarget.c_str());
		}
		GameSession* ts = mGSL->FindSessionWithCharID(invitee);
		if( ts && ts->GetPlayer() )
		{
#ifndef _DEBUG
			if( ts->AuthType()== 100 )
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_CAN_NOT_AUTH_TYPE);
				return 0;
			}
#endif
			if(ts->GetPlayer()->level()<=mGSL->mTradeAllowLevel)
			{
				char ss[512];_snprintf_s(ss,_countof(ss),511,"%d%s",mGSL->mTradeAllowLevel,mGSL->mStringConsts.NOTIFY_TRADE_CAN_NOT_LEVEL);ss[511]=0;
				AlertMessage(1,0,ss);
				return 0;
			}
			DomainPlay* player = ts->GetPlayer();
			if( GetMap() && player->GetMap() && player->GetMap()->GetName() == GetMap()->GetName() && abs(player->X()-X())+abs(player->Y()-Y()) < 10 )
			{
				mTradeTarget = invitee;
				player->NotifyTradeInvite(this);
			}
			else
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_TOO_FAR);
			}
		}
		return 0;
	}
	int DomainPlay::AgreeTradeInvite(const char* inv)
	{
		if( IsDead() ) return 0;
		std::string inviter = inv;
#ifndef _DEBUG
		if( mSession->AuthType()== 100 )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_CAN_NOT_AUTH_TYPE);
			return 0;
		}
#endif
		if( ! mTradeTarget.empty() )
		{
			CloseTrade(mTradeTarget.c_str());
		}
		GameSession* ts = mGSL->FindSessionWithCharID(inviter);
		if( ts && ts->GetPlayer() )
		{
#ifndef _DEBUG
			if( ts->AuthType()== 100 )
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_CAN_NOT_AUTH_TYPE);
				return 0;
			}
#endif
			DomainPlay* des = ts->GetPlayer();
			if( des && des->mTradeTarget==Name() )
			{
				if( GetMap() && des->GetMap() && des->GetMap()->GetName() == GetMap()->GetName() && abs(des->X()-X())+abs(des->Y()-Y()) < 10 )
				{
					mTradeTarget = des->Name();
					mSession->GetListener()->notifyTradeInfo(mSession,mTradeGameMoney,mTradeVcoin,mTradeSubmited,mTradeTarget,des->level(),des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited);
					des->mSession->GetListener()->notifyTradeInfo(des->mSession,des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited,des->mTradeTarget,level(),mTradeGameMoney,mTradeVcoin,mTradeSubmited);
				}
				else
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_TOO_FAR);
				}
			}
		}
		return 0;
	}


	int DomainPlay::TradeSubmit()
	{
		if( IsDead() ) return 0;
		bool ok = false;
		int trade_result = 0;
		mTradeSubmited = true;
		if( ! mTradeTarget.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(mTradeTarget);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des && des->mTradeTarget==Name() )
				{
					if( mTradeItem.size() <= static_cast<unsigned int>(des->get_empty_item_position_number()) 
						&& des->mTradeItem.size() <= static_cast<unsigned int>(get_empty_item_position_number()) )
					{
						if( des->mTradeSubmited )
						{
							for( ItemMap::iterator pos = mTradeItem.begin();
								pos != mTradeItem.end();
								++ pos )
							{
								des->AddItem(&(*pos),pos->mNumber,105,11,1);
							}
							mTradeItem.clear();
							for( ItemMap::iterator pos = des->mTradeItem.begin();
								pos != des->mTradeItem.end();
								++ pos )
							{
								AddItem(&(*pos),pos->mNumber,105,11,1);
							}
							des->mTradeItem.clear();
							if( mTradeGameMoney > 0 )
							{
								des->AddGameMoney(mTradeGameMoney,106);
							}
							mTradeGameMoney = 0;
							if( des->mTradeGameMoney > 0 )
							{
								AddGameMoney(des->mTradeGameMoney,106);
							}
							des->mTradeGameMoney = 0;
							if( mTradeVcoin > 0 )
							{
								des->AddVcoin(mTradeVcoin,106);
							}
							mTradeVcoin = 0;
							if( des->mTradeVcoin > 0 )
							{
								AddVcoin(des->mTradeVcoin,106);
							}
							des->mTradeVcoin = 0;
							mTradeTarget.clear();
							des->mTradeTarget.clear();
							mTradeSubmited = false;
							des->mTradeSubmited = false;
							trade_result = 1;
							AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_TRADE_SUC);
							des->AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_TRADE_SUC);
							mSession->SaveSession();
							des->mSession->SaveSession();
						}
						mSession->GetListener()->notifyTradeInfo(mSession,mTradeGameMoney,mTradeVcoin,mTradeSubmited,mTradeTarget,des->level(),des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited,trade_result);
						des->mSession->GetListener()->notifyTradeInfo(des->mSession,des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited,des->mTradeTarget,level(),mTradeGameMoney,mTradeVcoin,mTradeSubmited,trade_result);
						ok = true;
					}
				}
			}
		}
		if( ! ok )
		{
			CloseTrade(mTradeTarget.c_str());
		}
		return 0;
	}
	int DomainPlay::CloseTrade(const char* target)
	{
		AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_TRADE_FAIL);
		std::string tradetarget=mTradeTarget;
		if (tradetarget.empty())
		{
			if (target != "")
			{
				tradetarget = target;
			}
		}
		mTradeTarget.clear();
		mTradeSubmited = false;
		if( ! tradetarget.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(tradetarget);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des && des->mTradeTarget==Name() )
				{
					des->CloseTrade(des->mTradeTarget.c_str());
				}
			}
		}
		for( ItemMap::iterator pos = mTradeItem.begin();
			pos != mTradeItem.end();
			++ pos )
		{
			AddItem(&(*pos),pos->mNumber,106,11,1);
		}
		mTradeItem.clear();
		if( mTradeGameMoney > 0 )
		{
			AddGameMoney(mTradeGameMoney,107);
			mTradeGameMoney = 0;
		}
		if( mTradeVcoin > 0 )
		{
			AddVcoin(mTradeVcoin,107);
			mTradeVcoin = 0;
		}
		mSession->GetListener()->notifyTradeInfo(mSession,mTradeGameMoney,mTradeVcoin,mTradeSubmited,mTradeTarget,0,0,0,false);
		return 0;
	}
	int DomainPlay::SwithSlaveAIMode()
	{
		bool open = false;
		if( mSlaveKuLou && mSlaveKuLou->mAutoFindNext )
		{
			open = true;
		}
		if( mSlaveShenShou && mSlaveShenShou->mAutoFindNext )
		{
			open = true;
		}
		for(int i=0;i<5;i++)
		{
			if( mSlaves[i] && mSlaves[i]->mAutoFindNext )
			{
				open = true;
			}
		}
		open = !open;
		if( mSlaveKuLou )
		{
			mSlaveKuLou->mAutoFindNext = open;
		}
		if( mSlaveShenShou )
		{
			mSlaveShenShou->mAutoFindNext = open;
		}
		for(int i=0;i<5;i++)
		{
			if( mSlaves[i] )
			{
				mSlaves[i]->mAutoFindNext = open;
			}
		}
		if( open )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_SLAVE_AI_OPEN);
		}
		else
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_SLAVE_AI_CLOSE);
		}
		return 0;
	}
	int DomainPlay::AddDepotSlot()
	{
		bool ok = false;
		if( 1 == RemItemNumber(ITEM_ADD_SLOT,1,111,168) )
		{
			ok = true;
			mDepotSlotAdd ++;
		}
		else
		{
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(ITEM_ADD_SLOT);
			if( id )
			{
				if( RemVcoin(id->mPrice*mGSL->mAddDepotOnceNum,121) )
				{
					ok = true;
					mDepotSlotAdd += mGSL->mAddDepotOnceNum;
				}
				else
				{
					char temp[256];_snprintf_s(temp,256,mGSL->mStringConsts.NOTIFY_ADD_SLOT_NOT_ENOUPH_VCOIN,mGSL->GetItemDefineManager()->mAddDepotPrice*mGSL->mAddDepotOnceNum);
					AlertMessage(11,0,temp);
				}
			}
			else
			{
				AlertMessage(10,0,"no unlock item slot item");
			}
		}
		if( ok )
		{
			mSession->GetListener()->notifySlotAdd(mSession);
		}
		return 0;
	}
	void DomainPlay::AddBagSlot()
	{
		if(mBagSlotAdd < mGSL->mAddBagMaxSlot)
		{
			int cost = mGSL->mAddBagSlotPrice;
			if( RemVcoin(cost,121) )
			{
				mBagSlotAdd +=mGSL->mAddBagOnceNum;
				mSession->GetListener()->notifySlotAdd(mSession);
			}
			else
			{
				char temp[256];_snprintf_s(temp,256,mGSL->mStringConsts.NOTIFY_ADD_SLOT_NOT_ENOUPH_VCOIN,cost);
				AlertMessage(11,0,temp);
			}
		}
		else
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_ADD_SLOT_MAX_LIMIT); 
		}
	}
	void DomainPlay::AddBagFromLua(int num)
	{
		if(mBagSlotAdd+num <= mGSL->mAddBagMaxSlot)
		{
			mBagSlotAdd +=num;
			mSession->GetListener()->notifySlotAdd(mSession);
		}
	}
	void DomainPlay::addDepotFromLua(int num)
	{
		mDepotSlotAdd +=num;
		mSession->GetListener()->notifySlotAdd(mSession);
	}
	int DomainPlay::ClientFreshVcoin()
	{
		if( mFreshVcoinTick + mGSL->mVcoinFreshTime < mGSL->GetTimerFix()->Elapsed() )
		{
			mFreshVcoinTick = mGSL->GetTimerFix()->Elapsed();
			FreshVcoin();
			mSaveTick = mGSL->GetTimerFix()->Elapsed();
		}
		else
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_FRESH_VCOIN_WAIT);
		}
		return 0;
	}
	int DomainPlay::TradeAddItem(int item_pos,int item_type)
	{
		if( item_pos < ITEM_BAG_BEGIN || item_pos >= ITEM_BAG_SIZE+mBagSlotAdd )
		{
			return 4;
		}
		if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) && mGSL->GetItemDefineManager()->CheckItemTradeLimit(item_type) > 0 )
		{
			return 5;
		}
		if( mTradeSubmited )
		{
			return 2;
		}
		if( mTradeItem.size() >= 12 )
		{
			return 3;
		}
		if( ! mTradeTarget.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(mTradeTarget);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des && des->mTradeTarget==Name() )
				{
					for( ItemMap::iterator pos = GetItemList().begin();
						pos != GetItemList().end();
						++ pos )
					{
						if( pos->mTypeID > 0 && pos->mPosition == item_pos && pos->mTypeID == item_type )
						{
							if(pos->mItemFlags & ITEM_FLAG_BIND)
							{
								AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_ITEM_CAN_NOT_BIND);
								return 4;
							}
							SubItem si = *pos;
							si.mPosition = mTradeItem.size();
							mTradeItem.push_back(si);
							pos->mTypeID = 0;
							mNeedClearItemList = true;
							mSession->GetListener()->notifyItemChange(mSession,*pos,0);
							PushTradeItemData();
							des->PushTradeItemData();
							mSession->GetListener()->notifyTradeInfo(mSession,mTradeGameMoney,mTradeVcoin,mTradeSubmited,mTradeTarget,des->level(),des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited);
							des->mSession->GetListener()->notifyTradeInfo(des->mSession,des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited,des->mTradeTarget,level(),mTradeGameMoney,mTradeVcoin,mTradeSubmited);
						}
					}
				}
			}
		}
		return 0;
	}

	LEUD_INT64 DomainPlay::gamemoney(LEUD_INT64 gm)
	{
		mGameMoney=gm;
		if( mGameMoney >= mAchieveGameMoneyMax + 10000 )
		{
			mAchieveGameMoneyMax = (int)(mGameMoney - mGameMoney% 10000);
			mScriptEcho = "";
			mGSL->GetScriptEngine()->PlayerCongzi(this,(int)mGameMoney,mAchieveGameMoneyMax,0,"achieve.onAchieveNewGameMoneyMax");
			if( ! mScriptEcho.empty() )
			{
				mPlayerTalkSeed = rand();
				mSession->GetListener()->notifyPlayerTalk(mSession,this,mScriptEcho,mPlayerTalkSeed);
			}
			mScriptEcho = "";
		}
		return mGameMoney;
	}
	int DomainPlay::TradeAddGameMoney(int value)
	{
		if( mTradeSubmited )
		{
			return 2;
		}
		if( value <= 0 || value > gamemoney() )
		{
			return  1;
		}
		if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) )//iner lock
		{
			return 3;
		}
		if( ! mTradeTarget.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(mTradeTarget);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des && des->mTradeTarget==Name() )
				{
					if( RemGameMoney(value,108) )
					{
						mTradeGameMoney += value;
						mSession->GetListener()->notifyTradeInfo(mSession,mTradeGameMoney,mTradeVcoin,mTradeSubmited,mTradeTarget,des->level(),des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited);
						des->mSession->GetListener()->notifyTradeInfo(des->mSession,des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited,des->mTradeTarget,level(),mTradeGameMoney,mTradeVcoin,mTradeSubmited);
					}
				}
			}
		}
		return 0;
	}
	int DomainPlay::TradeAddVcoin(int value)
	{
		if( mTradeSubmited )
		{
			return 2;
		}
		if( value <=0 || value > (mVcoin-mVcoinGS) )//GS元宝不可以交易
		{
			return 1;
		}
		if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) )//iner lock
		{
			return 3;
		}
		if( ! mTradeTarget.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(mTradeTarget);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des && des->mTradeTarget==Name() )
				{
					if( RemVcoin(value,108) )
					{
						mTradeVcoin += value;
						mSession->GetListener()->notifyTradeInfo(mSession,mTradeGameMoney,mTradeVcoin,mTradeSubmited,mTradeTarget,des->level(),des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited);
						des->mSession->GetListener()->notifyTradeInfo(des->mSession,des->mTradeGameMoney,des->mTradeVcoin,des->mTradeSubmited,des->mTradeTarget,level(),mTradeGameMoney,mTradeVcoin,mTradeSubmited);
					}
				}
			}
		}
		return 0;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////喊话模式下的交易系统
	int DomainPlay::ChatTradeAddItem(int item_pos,int item_type,int value)
	{
		if( item_pos < ITEM_BAG_BEGIN || item_pos >= ITEM_BAG_SIZE+mBagSlotAdd || value <= 0 || item_type <= 0)
		{
			return 4;
		}
		if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) && mGSL->GetItemDefineManager()->CheckItemTradeLimit(item_type) > 0 )
		{
			return 5;
		}
		int num_ChatTrade_black = get_empty_item_position_number_in_ChatTrade();
		if(num_ChatTrade_black <= 0 )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_CHATTRADE_ADDITEM_FAIL);
			return 3;
		}
		int position_ChatTrade = find_empty_item_position_in_ChatTrade();
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == item_pos && pos->mTypeID == item_type )
			{
				if(pos->mItemFlags & ITEM_FLAG_BIND)
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_ITEM_CAN_NOT_BIND);
					return 4;
				}
				SubItem si = *pos;
				si.mPosition = position_ChatTrade;
				si.mSellPrice = value;
				si.mLock++;
				GetChatTradeItemList().push_back(si);////向我的物品列表中添加一个新物品。同时去除我原先背包中的物品
				pos->mTypeID = 0;
				mNeedClearItemList = true;
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				mSession->GetListener()->notifyItemChange(mSession,si,0);
				PushChatTradeItemData();
				AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_CHATTRADE_ADDITEM_SUC);
				return 0;
			}
		}
		
		return 0;
	}
	int DomainPlay::ChatTradeSubItem(int item_pos,int item_type)
	{
		bool find_item_result = false;
		if( item_pos < ITEM_CHATTRADE_BEGIN || item_pos >= ITEM_CHATTRADE_END )
		{
			return 4;
		}
		if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) && mGSL->GetItemDefineManager()->CheckItemTradeLimit(item_type) > 0 )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_CHATTRADE_SUBITEM_FAIL);
			return 5;
		}
		int position_bag = find_empty_item_position();
		for( ItemMap::iterator pos = GetChatTradeItemList().begin();
			pos != GetChatTradeItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 && pos->mPosition == item_pos && pos->mTypeID == item_type )
			{
				pos->mLock++;
				AddItem(&(*pos),pos->mNumber,105,11,1);///移除物品时发现此物品符合要求则放入背包中
				pos->mTypeID = 0;
				mNeedClearChatTradeItemList = true;
				mSession->GetListener()->notifyItemChange(mSession,*pos,0);
				PushChatTradeItemData();
				AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_CHATTRADE_SUBITEM_SUC);
				find_item_result = true;
				return 0;
			}
		}
		if(!find_item_result)
		{
			char infomsg[512];
			_snprintf_s(infomsg,_countof(infomsg),511,mGSL->mStringConsts.NOTIFY_CHATTRADE_SUBITEM_NOITEM,Name());infomsg[511]=0;
			AlertMessage(10,0,infomsg);
		}
		return 0;
	}
	int DomainPlay::CheckOnGroup()
	{
		std::string mTempStr=mSession->CharID();
		Group* group = mGSL->GetGroupManager()->FindGroupWithNickName(mTempStr);
		if( group )
		{
			return 1;
		}
		return 0;
	}
	int DomainPlay::ShowTradeItem(const std::string& inviter,int item_pos,int type,int lock)
	{
		if( IsDead() ) return 0;
		bool result_itemsearch = false;
		if(item_pos<ITEM_CHATTRADE_BEGIN || item_pos >= ITEM_CHATTRADE_END || item_pos <= 0) return 0;
		std::string target_inviter = inviter;
		if( ! target_inviter.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(target_inviter);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				for( ItemMap::iterator pos = des->GetChatTradeItemList().begin();
					pos != des->GetChatTradeItemList().end();
					++ pos )
				{
					if(pos->mTypeID == type && pos->mPosition == item_pos && pos->mLock == lock)
					{	
						PushChatTradeTargetItemData(target_inviter,item_pos);
						result_itemsearch = true;
						return 0;
					}
				}
				if(!result_itemsearch)
				{
					AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_PLAYER_ITEM_DATA_ERROR);
				}
			}
			else
			{
				AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_PLAYER_OFFLINE);
				return 1;
			}
		}
		return 0;
	}
	int DomainPlay::ChatTradeSubmit(const std::string& inviter, int item_pos, int item_type, int lock)
	{
		if( IsDead() ) return 0;
		
		if(item_pos<ITEM_CHATTRADE_BEGIN || item_pos >= ITEM_CHATTRADE_END || item_type <= 0) return 0;
		std::string target_inviter = inviter;
		if(Name() == target_inviter)////玩家禁止自我交易
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_CHATTRADE_MYSELF);
			return 0;
		}
		bool result_SearchItem = false;////找到物品的结果
		if( ! target_inviter.empty() )
		{
			GameSession* ts = mGSL->FindSessionWithCharID(target_inviter);
			if( ts && ts->GetPlayer() )
			{
				DomainPlay* des = ts->GetPlayer();
				if( des )
				{
					if(static_cast<unsigned int>(get_empty_item_position_number()) >= 1 )
					{
							for( ItemMap::iterator pos = des->GetChatTradeItemList().begin();
								pos != des->GetChatTradeItemList().end();
								++ pos )
							{
								if(pos->mTypeID == item_type && pos->mPosition == item_pos && pos->mLock == lock)
								{
									if((mVcoin-mVcoinGS) >= pos->mSellPrice && pos->mSellPrice > 0)//GS元宝不可以交易
									{
										RemVcoin(pos->mSellPrice,108);
										des->AddVcoin(pos->mSellPrice,106);

										pos->mSellPrice=0;
										pos->mLock=0;
										AddItem(&(*pos),pos->mNumber,105,11,1);
										ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
										if(id > 0)
										{
											char msg[512];
											_snprintf_s(msg,_countof(msg),511,mGSL->mStringConsts.NOTIFY_CHATTRADE_SUC,des->Name(),id->mName.c_str(),Name());
											msg[511]=0;
											AlertMessage(11,0,msg);
											des->AlertMessage(11,0,msg);
										}
										pos->mTypeID = 0;
										des->mNeedClearChatTradeItemList = true;
										des->mSession->GetListener()->notifyItemChange(des->mSession,*pos,0);
										des->PushChatTradeItemData();
										mSession->SaveSession();
										des->mSession->SaveSession();
										return 0;
									}
									else/////钱不够的时候的提示
									{
										char temp[512];_snprintf_s(temp,_countof(temp),511,mGSL->mStringConsts.NOTIFY_CHATTRADE_MONEY_NOTENOUGH,pos->mSellPrice);temp[511]=0;
										mSession->GetListener()->notifyAlert(mSession,11,0,temp);
										result_SearchItem = true;////只要找到了这个物品就设为true
										return 0;
									}
									result_SearchItem = true;////只要找到了这个物品就设为true
								}
								
							}							
						}
					else
					{
						AlertMessage(1, 0, mGSL->mStringConsts.NOTIFY_CHATTRADE_BAGFULL);
						return 0;
					}
				}
			}
		}
		if( ! result_SearchItem )
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_CHATTRADE_FAIL);
		}
		return 0;
	}
	int DomainPlay::ChangeAttackMode(int attack_mode)
	{
		if( mLockAttackMode ) 
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_CHANGE_ATTACK_MODE_FAIL);
			return 0;
		}
		if( ATTACK_MODE_TYPE_ALL <= attack_mode && ATTACK_MODE_TYPE_END > attack_mode )
		{
			mAttackMode=attack_mode;
			mSession->GetListener()->resChangeAttackMode(mSession,mAttackMode);
			if( mAttackMode != ATTACK_MODE_TYPE_PEACE )
			{
				mPKconfirm = 1;
			}
		}
		return 0;
	}
	int DomainPlay::AttackMode(int attack_mode)
	{
		if( ATTACK_MODE_TYPE_ALL <= attack_mode && ATTACK_MODE_TYPE_END > attack_mode )
		{
			mAttackMode=attack_mode;
		}
		return mAttackMode;
	}
	void DomainPlay::AlertMessage(int lv,int flags,const char* msg)
	{
		if( mSession && mSession->GetListener() )
		{
			mSession->GetListener()->notifyAlert(mSession,lv,flags,msg);
		}
	}
	void DomainPlay::Relive(int relive_type,const char* map_name,int x,int y)
	{
		if( IsDead() )
		{
			bool ok = false;
			bool stand = false;
			if( (!ok) && relive_type == 120 && map_name )
			{
				PlayMap* map = mGSL->GetMapManager()->FindMap(map_name);
				if(map)
				{
					DirectFlyNode* node = mGSL->GetFlyManager()->FindNode(map->RelivePos());
					if(node)
					{
						if(EnterMap(node->mTargetMapID,node->mTargetX,node->mTargetY,node->mTargetSize))
						{
							ok=true;
						}
					}
				}
				if(!ok)
				{
					EnterMap(map_name,x,y);
					ok=true;
				}
			}

			if( (!ok) && mParentMap && (!mParentMap->CanReliveStand()) )
			{
				relive_type = 100;
				AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_RELIVE_STAND_NOT_ALLOW);
			}
			if( (!ok) && (relive_type == 100 || relive_type == 101 ) )
			{
				if( relive_type == 101 )
				{
					if( mParentMap && mParentMap->NoProtectNewer() == 0 && mGSL->mFreeReliveLimitLevel > level() )
					{
						ok = true;
					}
					else
					{
						if(mSession->GetGameServerLib()->GetScriptEngine()->PlayerInt(this,0,0,0,0,"gui.PanelRelive.rem_freerelive_times"))
						{
							stand = true;
							ok = true;
						}
						else if( 1 == RemItemNumber(ITEM_RELIVE_COIN_ID,1,111,169) )
						{
							stand = true;
							ok = true;
						}
						else
						{
							ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(ITEM_RELIVE_COIN_ID);
							if( id )
							{
								if( RemVcoin(id->mPrice,109) )
								{
									stand = true;
									ok = true;
								}
								else
								{
									AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_RELIVE_STAND_NOT_ENOUPH_VCOIN);
								}
							}
							else
							{
								AlertMessage(10,0,"Relive in place prohibited");
							}
						}
					}
				}
				if( relive_type == 100 )
				{
					if(gsl()->GetListener()->GetKuaFuService())
					{
						EnterKuafuMap();
					}
					else
					{

						if(mParentMap)
						{
							DirectFlyNode* node = mGSL->GetFlyManager()->FindNode(mParentMap->RelivePos());
							if(node)
							{
								if(EnterMap(node->mTargetMapID,node->mTargetX,node->mTargetY,node->mTargetSize))
								{
									ok=true;
								}
							}
						}
						if(!ok)
						{
							if(!go_home())
							{
								EnterMap(mGSL->mStart_map,mGSL->mStart_x,mGSL->mStart_y);
							}
						}
					}
					ok = true;
				}
			}
			if( ok )
			{
				SetState(PlayMapObject::MAP_OBJECT_STATE_IDLE);
				UpdateAfterCalculateAttribute();
				mSession->GetGameServerLib()->GetScriptEngine()->PlayerInt(this,stand,0,0,0,"player.onReliveAfter");
				mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
				NotifyRelive(this,relive_type);
				hurtSeed(hurtSeed() + 1);
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();
					++ pos )
				{
					if ((*pos))
					{
						(*pos)->NotifyHPMPChange(this, 0, 0);
						(*pos)->NotifyRelive(this, relive_type);
					}
				}
			}
		}
	}
	void DomainPlay::ProcessGift()
	{
		for( DomainData::GiftArray::iterator pos = mCharacter->mGiftArray.begin();
			pos != mCharacter->mGiftArray.end();
			++ pos )
		{
			int num=pos->mNum>0?pos->mNum:1;
			if( pos->mUsed == 1)
			{
				if(NumBagBlack() >= num)
				{
					SubItem ii;
					ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);
					if( id )
					{
						ii.BuildItemInstance(id,pos->mBind,false,mGSL->mItemAddType);
						AddItem(&ii,num,124,11,1);
					}
				}else{
					continue;
				}
			}
			pos->mUsed = 10;
		}
	}
	int DomainPlay::initPlayer(void)
	{
		PushStatusDef();
		PushBuffDef();
		PushAllItemData();

		Load();
		mGMLevel = mGSL->GetGMCommand()->GetGMLevel(Name());
		CalculateAttribute();

		mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onLoad");
		CalculateAttribute();
		UpdateAfterCalculateAttribute();
		UpdateAvatar();
		mHp = mCharacter->cur_hp;
		if( mHp <= 0 || mHp > Attributes()->mMaxHp )
		{
			if( mHp <= 0 )
			{
				mCharacter->exit_map[0] = 0;
			}
			mHp = Attributes()->mMaxHp;
		}
		mMp = mCharacter->cur_mp;
		if( mMp <= 0 || mMp >= Attributes()->mMaxMp )
		{
			mMp = Attributes()->mMaxMp;
		}
		mPower = mCharacter->cur_power;
		if (mPower <= 0 || mPower >= Attributes()->mMaxPower)
		{
			mPower = Attributes()->mMaxPower;
		}
		//if( CoupleID() > 0 ) FreshCoupleInfo();
		mSession->GetListener()->notifyCharacterLoad(mSession);
		mSession->GetListener()->notifyAttributeChange(mSession);
		mSession->GetListener()->notifyExpChange(mSession,0);
		mSession->GetListener()->notifyLevelChange(mSession);
		mSession->GetListener()->notifyCapacityChange(mSession,0);
		mSession->GetListener()->notifyGameMoneyChange(mSession,0,0,0,0);
		mSession->GetListener()->notifyTotalAttrInfo(mSession);
		mSession->GetListener()->notifyTiliChange(mSession, this);
		NotifyAvatarChange(this);
		PushItemData();
		PushChatTradeItemData();
		PushSkillData();
		PushTaskData();
		// 更新好友信息
		UpdateAllRelation();
		// 推送好友列表
		PushFriendData();
		PushShortcutData();
		// 
		GameServerLib* gsl=mGSL;
		if(GetGSL()->GetListener()->GetKuaFuService())
		{
			EnterKuafuMap();
		}
		else
		{
			PlayMap* exit_map=0;
			if( mCharacter->exit_map[0] != 0 )
			{
				exit_map = gsl->GetMapManager()->FindMap(mCharacter->exit_map,Name());
			}
			if( exit_map && exit_map->FubenType() != OGSLib::MAP_TYPE::MAP_PERSONAL && exit_map->AutoEnter() && exit_map->GetBlockData(mCharacter->exit_x,mCharacter->exit_y,2) == 0 )
			{
				EnterMap(mCharacter->exit_map,mCharacter->exit_x,mCharacter->exit_y,1);
			}
			else if (exit_map && exit_map->GetOwnerName() == Name() && exit_map->AutoEnter() && exit_map->FubenType() == OGSLib::MAP_TYPE::MAP_PERSONAL && exit_map->GetBlockData(mCharacter->exit_x, mCharacter->exit_y, 2) == 0)////掉线进入个人副本
			{
				EnterMap(mCharacter->exit_map, mCharacter->exit_x, mCharacter->exit_y);
				HandleMapOwner(false);
				mGSL->GetScriptEngine()->MapPlayer(exit_map, this, "player.onFubenMapSetTimer");
			}
			else
			{
				go_home();
			}
		}
		mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onEnterGame");
		mSession->GetListener()->resChangeAttackMode(mSession,AttackMode());
		NotifyPKStateChange(this);
		PushParamData();
		FreshVcoin();

		mSaveTick = mGSL->GetTimerFix()->Elapsed();

		FreshOnlineTime();
		mGSL->GetChartManager()->UpdateData(CHART_TYPE_GAMEMONEY, Name(), (int)gamemoney(), mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
		ProcessGift();
		if(mSession->mAuthType == AUTH_TYPE_CHAT)
		{
			mGSL->GetGMCommand()->AddAdmin(Name(),1000);
		}
		if( mGMLevel > 0  )
		{
			mGMHideTalk = 1;
			AlertMessage(10,0,"chat message no public to other player");
			AddStatus(15,0xFFFFFFF,10000);
			AddStatus(16,0xFFFFFFF,10000);
		}

		if(this->GetAchievePoint() > 0 )
		{
			mGSL->GetChartManager()->UpdateData(CHART_TYPE_ACHIEVE, Name(), GetAchievePoint(), mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
		}
		mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onLoadRobDartNumUsed");
		mGSL->GetScriptEngine()->OnlyPlayer(this,"lottery.onLoadLotteryList");

		this->LoadPostMail();

		// 通知好友上线
		NotifyRelationInfoChange();
		return 0;
	}
	void DomainPlay::BuyOfflineExp(int mul)
	{
		
	}
	void DomainPlay::FreshPlayerInfo()
	{
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* pl = (DomainPlay*)(*pos);
					pl->Session()->GetListener()->notifyMapBye(pl->Session(), GetMap(), this);
					pl->Session()->GetListener()->notifyMapMeet(pl->Session(), GetMap(), this, 0);
				}
			}
		}
	}

	int DomainPlay::FreshTimeItem(time_t t)
	{
		mNeedFreshItemList = false;
		for( ItemMap::iterator pos = GetItemList().begin();
			pos != GetItemList().end();
			++ pos )
		{
			if( pos->mTypeID > 0 )
			{
				ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(pos->mTypeID);if(! id){continue;}
				if( id->mLastTime <= 0 ){continue;}
				mNeedFreshItemList = true;
				//printf("=??===%d,%d,%d,%d\n", pos->mTypeID, pos->mPosition, pos->mCreateTime, pos->mLasttime);
				if (pos->mCreateTime + pos->mLasttime <= t)
				{
					//printf("=>>>>===%d,%d,%d,%d\n", pos->mTypeID, pos->mPosition, pos->mCreateTime, pos->mLasttime);
					char msg[256];
					_snprintf_s(msg,_countof(msg),255,mGSL->mStringConsts.NOTIFY_ITEM_TIME_LIMIT_FULL,id->mName.c_str());msg[255]=0;//,id->mLastTime/60
					AlertMessage(11,0,msg);
					mGSL->GetScriptEngine()->PlayerString(this, "player.onItemLastTime", id->mName.c_str());
					int position = pos->mPosition;
					pos->mTypeID = 0;
					mNeedClearItemList = true;
					mSession->GetListener()->notifyItemChange(mSession,*pos,0);
					if( position < 0 )
					{
						UpdateAvatar();
						CalculateAttribute();
						for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
							pos != mVisibleInteractiveObjectList.end();
							++ pos )
						{
							if ((*pos))
							{
								(*pos)->NotifyAvatarChange(this);
							}
						}
						NotifyAvatarChange(this);
						mSession->GetListener()->notifyAttributeChange(mSession);
						mSession->GetListener()->notifyHPMPChange(mSession,this,0,0);
					}
				}
			}
		}
		return 0;
	}

	int DomainPlay::destPlayer(void)
	{
		CloseTrade(mTradeTarget.c_str());
		StopStall();

		mSession->GetListener()->notifySessionEnd(mSession);
		SaveExitMapPosition();
		mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onLeaveGame");
		
		if( mDart2 && mDart2->mParentMap )
			mDart2->mOwner = NULL;

		HandleMapOwner(true);/////对于从副本地图中出去的玩家(意外掉线)
		LeaveMap();
		mGSL->GetFrameTimer()->RemoveObject(this);
		mGSL->GetScriptEngine()->OnlyPlayer(this,"player.onUnload");
		if(gsl()->GetListener()->GetKuaFuService())
		{
			notifyKuafuComeBackData();
		}
		else
		{
			Save();
		}
		NotifyRelationInfoChange(1);
		return 0;
	}
	void DomainPlay::LogItemChange(int src,int act,SubItem* item)
	{
		
	}
	
	void DomainPlay::SetNamePre(const std::string& namepre)
	{
		mNamePre = namepre;
		NotifyNameAdd(this,mNamePre,mNamePro);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* play = static_cast<DomainPlay*>(*pos);
					play->NotifyNameAdd(this, mNamePre, mNamePro);
				}
			}
		}
	}
	void DomainPlay::SetNamePro(const std::string& namepro)
	{
		mNamePro = namepro;
		NotifyNameAdd(this,mNamePre,mNamePro);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* play = static_cast<DomainPlay*>(*pos);
					play->NotifyNameAdd(this, mNamePre, mNamePro);
				}
			}
		}
	}
	void DomainPlay::NotifyNameAdd(DomainPlay* src,const std::string& namepre,const std::string& namepro)
	{
		mSession->GetListener()->notifyNameAdd(mSession,src,namepre,namepro);
	}
	void DomainPlay::NotifyTeamInfo(DomainPlay* src)
	{
		mSession->GetListener()->notifyTeamInfo(mSession,src);
	}
	time_t DomainPlay::OnlineTime()
	{
		return time(0)-mCharacter->online_time;
	}
	time_t DomainPlay::AllOnlineTime()
	{
		return mCharacter->online_time_all;
	}
	time_t DomainPlay::TodayOnlineTime()
	{
		return mCharacter->online_time_today;
	}
	time_t DomainPlay::YestodayOnlineTime()
	{
		return mCharacter->online_time_yestoday;
	}
	void DomainPlay::SetTeamInfo(int team,const char* teamname)
	{
		mTeamID = team;
		strncpy_s(mTeamName,_countof(mTeamName),teamname,127);
		mTeamName[127] = 0;

		NotifyTeamInfo(this);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsPlayer())
				{
					DomainPlay* play = static_cast<DomainPlay*>(*pos);
					play->NotifyTeamInfo(this);
				}
			}
		}
	}

	void DomainPlay::GenDart(int def_id)
	{
		if( 0 == mDart2 )
		{
			MonDef* md = 0;
			md = mGSL->GetMonDefManager()->FindMonDef(def_id);
			if( md )
			{
				mDart2 = new PlayDart2Dummy;
				char monname[256];
				sprintf_s(monname,"%s(%s)",md->name,Name());
				mDart2->Name(monname);
				char aii[256];aii[0]=0;
				char opt[1024];opt[0]=0;
				sscanf_s(md->ai.c_str(),"%[^|]|%s",aii,_countof(aii),opt,_countof(opt));
				mGSL->GetMapManager()->LoadAIObject(md,mDart2,opt);
				mDart2->mWalkGap = md->walk_gap2;
				mDart2->mOwner = this;
				mDart2->SetManager(mGSL->GetMonsterManager(), 0);
			}
		}

		if (mDart2)
		{
			if (!mDart2->mLiving)
			{
				const std::vector<MapPoint>& path = mGSL->GetMapManager()->GetDartPath();
				if (path.size() > 0)
				mDart2->Relive(mParentMap, path[0].x, path[0].y,Dir());
				mDart2->mDartTaskStartTick = (int)time(0);
			}
		}
	}

	int DomainPlay::HaveDart()
	{
		if (mDart2 && mDart2->mLiving)
		{
			return 1;
		}
		return 0;
	}

	int DomainPlay::GetDartID()
	{
		if (mDart2 && mDart2->mLiving)
		{
			return mDart2->ID();
		}
		return 0;
	}

	bool DomainPlay::BindDart(int id)
	{
		PlayDart2 * dart = static_cast<PlayDart2 *>(mGSL->GetMonsterManager()->FindDartById(id));
		if (dart)
		{
			dart->mOwner = this;
			this->mDart2 = dart;
			return true;
		}

		return false;
	}

	void DomainPlay::KillDart()
	{
		if (mDart2)
		{
			mDart2->DamageHP(this,999999999,0);
		}
	}
	int DomainPlay::StartCollect(int srcid)
	{
		mCollectTarget=srcid;
		PlayActiveObject* src=FindActiveInView(srcid);
		if( src && src->TypeIsMonster() )
		{
			PlayMonster* mon=(PlayMonster*)src;
			if( mon->CollectTime() <= 0 )return 0;
			mGSL->GetScriptEngine()->MonsterPlayer(mon,this,"player.onStartCaiji");
			mCaijiTick = mGSL->GetTimerFix()->Elapsed()+mon->CollectTime();

		}
		return 0;
	}
	int DomainPlay::EndCollect()
	{
		if(!mCollectTarget)
		{
			return 1;
		}
		PlayActiveObject* src=FindActiveInView(mCollectTarget);
		if( src && src->TypeIsMonster() )
		{
			PlayMonster* mon=(PlayMonster*)src;
			if( mon->CollectTime() > 0 )
			{
				PlayMonsterCollection* mcol=(PlayMonsterCollection*)src;
				mSession->GetListener()->notifyCollectEnd(mSession, src->ID());
				//mcol->DamageHP(this,1,0);
				mGSL->GetScriptEngine()->MonsterPlayer(mon,this,"player.onCaiji"); 
				mCaijiTick=0;
				mCollectTarget=0;
			}
		}
		return 0;
	}
	int DomainPlay::CollectBreak()
	{
		mCaijiTick = 0;
		if(!mCollectTarget)
		{
			return 1;
		}
		else
		{
			mCollectTarget=0;
			mSession->GetListener()->notifyCollectBreak(mSession);
		}
		return 0;
	}
	int DomainPlay::ChangeMountBreak()
	{
		if( mChangeMountTick > 0 )
		{
			mChangeMountTick = 0;
			mSession->GetListener()->notifyCollectBreak(mSession);
		}
		return 0;
	}

	void DomainPlay::NotifyPushLuaTable(std::string type,int flag,std::string table)
	{
		mSession->GetListener()->notifyPushLuaTable(mSession,type,flag,table);
	}
	int DomainPlay::SetZsLevel(int zslevel)
	{
		if (zslevel > mZsLevel)
		{
			mZsLevel = zslevel;
			NotifyAvatarChange(this);
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyAvatarChange(this);
				}
			}
			if (level() >= mGSL->mLevelChartLimit)
			{
				////////////////////转生成功之后触发排行版
				//int param = mZsLevel * 10000 + mLevel;
				int param =  10000 + mLevel;
				mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_ALL, Name(), param, mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
				if (job() == JOB_WARRIOR)
				{
					mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_WIR, Name(), param, mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
				}
				if (job() == JOB_WIZARD)
				{
					mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_WIZ, Name(), param, mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
				}
				if (job() == JOB_TAOIST)
				{
					mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_LEVEL_TAO, Name(), param, mCharacter->mGuildName, mTitleName.c_str(), job(), level(), ZsLevel(),gender());
				}
			}
		}
		return mZsLevel;
	}
	void DomainPlay::setHpMpPercent(int percent)
	{
		if (percent > 0 && percent < 100)
		{
			mHp = mActiveAttribute.mMaxHp*percent/100;
			mMp = mActiveAttribute.mMaxMp*percent/100;
			mPower = MaxPower()*percent/100;
		}
	}

	int DomainPlay::ItemUpgrade(int pos,int updac,int updmac,int upddc,int updmc,int updsc)
	{
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		SubItem* si = get_item_by_pos(pos);if(!si){return 0;}
		if(!ItemDef::IsEquipment(si->mSubType) ){return 0;}

		int oldlevel=si->mLevel;
		int oldmaxcount=si->mUpdMaxCount;
		int oldfailedcount=si->mUpdFailedCount;

		int ret=si->ItemUpgrade(this,updac,updmac,upddc,updmc,updsc);
		if(!ret)
		{
			return -1;
		}
		else
		{
			CalculateAttribute();
			mSession->GetListener()->notifyAttributeChange(mSession);
		}
		
		UpdateAvatar();
		mSession->GetListener()->notifyItemChange(mSession,*si,0);

		return si->mLevel;
	}

	int DomainPlay::ItemUpgrade(int pos,int type_id, int level)
	{
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		SubItem* si = get_item_by_pos_type(pos, type_id);if(!si){return 0;}
		if(!ItemDef::IsEquipment(si->mSubType) ){return 0;}

		int oldlevel=si->mLevel;
		int oldmaxcount=si->mUpdMaxCount;
		int oldfailedcount=si->mUpdFailedCount;

		int ret=si->ItemUpgrade(this,level);
		if(!ret)
		{
			return -1;
		}

		UpdateAvatar();
		mSession->GetListener()->notifyItemChange(mSession,*si,0);
		return si->mLevel;
	}

	int DomainPlay::ItemZhuLing(int pos,int flag)
	{
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		SubItem* si = get_item_by_pos(pos);if(!si){return 0;}
		if(!ItemDef::IsEquipment(si->mSubType) ){return 0;}

		int oldlevel=si->mZLevel;

		int ret=0;
		if (flag == 1)
		{
			ret = si->ItemZhuLing(this);
		}
		else
		{
			ret = si->RemoveZhuLing(this);
		}
		if(!ret)
		{
			return -1;
		}
		else
		{
			CalculateAttribute();
			mSession->GetListener()->notifyAttributeChange(mSession);
		}

		mSession->GetListener()->notifyItemChange(mSession,*si,0);

		return si->mZLevel;
	}
	int DomainPlay::ItemAddUpdFailedCount(int pos,int count)
	{
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		SubItem* si = get_item_by_pos(pos);if(!si){return 0;}
		if(!ItemDef::IsEquipment(si->mSubType) ){return 0;}

		//if(si->mUpdFailedCount+count<=si->mUpdMaxCount-si->mLevel)
		//{
			si->mUpdFailedCount+=count;

			mSession->GetListener()->notifyItemChange(mSession,*si,0);
			return 1;
		//}
		///return -1;
	}
	int DomainPlay::ItemSubUpdFailedCount(int pos,int count)
	{
		if( pos >= ITEM_BAG_SIZE + mBagSlotAdd ) return 0;
		SubItem* si = get_item_by_pos(pos);if(!si){return 0;}
		if(!ItemDef::IsEquipment(si->mSubType) ){return 0;}

		if(si->mUpdFailedCount-count>=0)
		{
			si->mUpdFailedCount-=count;

			mSession->GetListener()->notifyItemChange(mSession,*si,0);
			return 1;
		}
		return -1;
	}

	int DomainPlay::AddStallItem(int pos,int price_type,int price)
	{
		if( pos < ITEM_BAG_BEGIN || pos >= ITEM_BAG_SIZE+mBagSlotAdd )
		{
			return 4;
		}
		if( mStallStart )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_STALL_NO_ACTION);
			return 2;
		}
		int emp_pos=find_empty_item_position_in_stall();
		if( emp_pos<0 )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_STALL_ITEM_FULL);
			return 3;
		}

		SubItem* si = get_item_by_pos(pos);if(!si){return 1;}
		
		if( mGSL->GetDenyManager()->CheckChrLimit(mCharacter->mAccount) && mGSL->GetItemDefineManager()->CheckItemTradeLimit(si->mTypeID) > 0 )
		{
			return 5;
		}

		if(si->mItemFlags & ITEM_FLAG_BIND)
		{
			AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_TRADE_ITEM_CAN_NOT_BIND);
			return 4;
		}

		if(price_type>0&&price>0)
		{
			si->mPosition=emp_pos;
			si->mSellPriceType=price_type;
			si->mSellPrice=price;

			mSession->GetListener()->notifyItemChange(mSession,*si,0);
		}

		return 0;
	}
	int DomainPlay::RemStallItem(int pos)
	{
		if( pos >= ITEM_STALL_END || pos < ITEM_STALL_BEGIN )
		{
			return 3;
		}
		if( mStallStart )
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_STALL_NO_ACTION);
			return 2;
		}
		SubItem* si = get_item_by_pos(pos);if(!si){return 1;}

		int emp_pos=find_empty_item_position();

		if(emp_pos>=0)
		{
			si->mPosition=emp_pos;
			si->mSellPriceType=0;
			si->mSellPrice=0;

			mSession->GetListener()->notifyItemChange(mSession,*si,0);
		}

		return 0;
	}
	int DomainPlay::StartStall(const char* stallname)
	{
		size_t gnl = strlen(stallname);
		if( gnl <= 0||gnl > 64 )
		{
			mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_STALL_NAME_ERROR);
			return 0;
		}


		return 0;
	}
	void DomainPlay::StopStall()
	{
		if(mStallStart)
		{
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_STALL_STOP);
			mStallStart=false;
			mStallName.clear();
		}
	}
	void DomainPlay::ViewStall(DomainPlay* des)
	{

	}
	int DomainPlay::BuyStallItem(DomainPlay* des,int pos)
	{

		return 0;
	}

	void DomainPlay::NotifyStallChange(DomainPlay* src,int x,int y,bool stall,const std::string& stallname)
	{

	}
	void DomainPlay::NotifyStallItemInfo()
	{

	}
	
	void DomainPlay::NotifyNewMailNum(int num)
	{
		mSession->GetListener()->notifyNewMailNum(this->mSession,num);
	}
	void DomainPlay::PostMail(std::string& title,std::string& content,const std::map<int,int>& items)
	{
		mGSL->GetMailManager()->PostMailToOnePlayer(title,content,std::string(this->SeedName()),items);
	}
	void DomainPlay::PostMailToAll(std::string& title,std::string& content,const std::map<int,int>& items)
	{
		mGSL->GetMailManager()->PostMailToAllPlayers(title,content,items);
	}
	void DomainPlay::NotifyMailReceiveSuccess(LEUD_INT64 mailID)
	{
		mSession->GetListener()->notifyMailReceiveSuccess(mSession,mailID);
	}
	void DomainPlay::notifyKuafuComeBackData()
	{
		int result = 1;
		std::string kuafuArea,kuafuGuild;
		if(!GetGSL()->GetKuafuState())
		{
			if(GetGSL()->Var("kuafuarea") == mSession->GetListener()->GetKuafuArea() &&
				GetGSL()->Var("kuafuguild") == GuildName())
			{
				result = 3;
			}
			else
			{
				result = 2;
			}
			kuafuArea = GetGSL()->Var("kuafuarea");
			kuafuGuild = GetGSL()->Var("kuafuguild");
		}
		mSession->GetListener()->notifyKuafuComeBackData(mSession,kuafuArea,kuafuGuild,LoginID(),Name(),SeedName(),vcoin(),result);
	}
	void DomainPlay::NotifyChargeDartList()
	{
		mSession->GetListener()->notifyChargeDartList(mSession);
	}

	int DomainPlay::SetSwordLv(int lv)
	{
		if(lv >= mSwordLv)
		{
			mSwordLv = lv;
		}
		CalculateAttribute();
		NotifyAttributeChange();
		NotifyHPMPChange(this,0,0);
		return mSwordLv;
	}
	int DomainPlay::SetMonPointLv(int lv)
	{
		if(lv >= mMonPointLv)
		{
			mMonPointLv = lv;
		}
		CalculateAttribute();
		NotifyAttributeChange();
		NotifyHPMPChange(this,0,0);
		return mMonPointLv;
	}
	void DomainPlay::BecomeMonster(int mon_show,std::string monname)
	{
		mMonShow = mon_show;
		mMonshowName = monname;
		CalculateAttribute();
	}
	int DomainPlay::SetWingLv(int lv, bool updateChart)
	{
		if (lv >= 0)
		{
			mHair = lv;
			UpdateAvatar();
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					(*pos)->NotifyAvatarChange(this);
				}
			}
			NotifyAvatarChange(this);
			//更新翅膀排行
			if (updateChart)
			{
				int param = lv - 70000;
				mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_WING_ALL, Name(), param, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender());
			}
		}
		return mHair;
	}

	void DomainPlay::UpdataChart(int winglv,int tianguanlv)
	{
		mSession->GetGameServerLib()->GetChartManager()->setChartGuild(CHART_TYPE_WING_ALL, Name(), GuildName().c_str());
		mSession->GetGameServerLib()->GetChartManager()->setChartGuild(CHART_TYPE_TIANGUAN_ALL, Name(), GuildName().c_str());
		mSession->GetGameServerLib()->GetChartManager()->setChartGuild(CHART_TYPE_LEVEL_ALL, Name(), GuildName().c_str());
		mSession->GetGameServerLib()->GetChartManager()->setChartGuild(mJob + 6, Name(), GuildName().c_str());
	}

	void DomainPlay::SetTianGuanLv(int lv)
	{
		mSession->GetGameServerLib()->GetChartManager()->UpdateData(CHART_TYPE_TIANGUAN_ALL, Name(), lv, GuildName().c_str(), mTitleName.c_str(), job(), level(), ZsLevel(), gender());
	}

	void DomainPlay::NotifyGuildItemList()
	{
		mSession->GetListener()->notifyListGuildItem(mSession);
	}

	void DomainPlay::SetMaxPower(int power,int needfresh,int powper)
	{		
		if (!BaseAttributes()->mMaxPower)
		{
			mPower = power;
		}
		else if(BaseAttributes()->mMaxPower<power && mPower == BaseAttributes()->mMaxPower){
			mPower = power;
		}
		BaseAttributes()->mMaxPower = power;
		BaseAttributes()->mPowerPercent = powper;
		if (needfresh > 0)
		{
			CalculateAttribute();
			NotifyTiliChange(this);
		}
	}

	void DomainPlay::NotifyTiliChange(PlayActiveObject* src)
	{
		Session()->GetListener()->notifyTiliChange(Session(),src);
	}

	void DomainPlay::AgreeFriendApply(const char* name, int agree)
	{
		if (agree != 1 && agree != 0)
			return;
		FriendApplyMap::iterator it = mFriendApplyMap.find(name);
		if (it == mFriendApplyMap.end())
		{
			// 请求过期
			AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_FRIEND_AGREE_TIMEOUT_ERROR);
			return;
		}

		mFriendApplyMap.erase(it);
		if (agree == 0)
		{
			// 拒绝
			return;
		}

		// 同意,对方如果在线互相为好友(this为被动方)
		GameSession* gs = mGSL->FindSessionWithCharID(name);
		if (gs && gs->GetPlayer())
		{
			// 主动添加者添加好友(name:主动申请者名字)
			gs->GetPlayer()->ChangeRelationShip(this->mCharacter->mChrName, RELTYPE_FRIEND, true);
			char temp[512];
			_snprintf_s(temp,512,mGSL->mStringConsts.NOTIFY_FRIEND_ADD,this->mCharacter->mChrName);
			gs->GetPlayer()->AlertMessage(10,0,temp);
			_snprintf_s(temp,512,mGSL->mStringConsts.NOTIFY_FRIEND_ADD_NOTIFY,name);
			AlertMessage(10,0,temp);
		}
		else
		{
			// 不在线,无法添加
			return;
		}
	}

	void DomainPlay::NotifyItemChange(OGSLib::SubItem item,int show_flags)
	{
		mSession->GetListener()->notifyItemChange(mSession, item, show_flags);
	}

	void DomainPlay::NotifyMiniNpc()
	{
		if(mParentMap)
		{
			mSession->GetListener()->notifyMiniNpc(mSession, mParentMap);
		}
	}

	PlaySlave * DomainPlay::GetSlave(int type)
	{
		if (type >=0 && type < 5)
			return mSlaves[type];
		if (type == 10)
			return mSlaveHuwei;
		return NULL;
	}

	void DomainPlay::SetTaskMonID(int id)
	{
		m_nTaskMonId = id;
	}

	int DomainPlay::GetTaskMonID()
	{
		return m_nTaskMonId;
	}

	void DomainPlay::NotifyMonExpHiterChange(PlayMapObject* src, DomainPlay * play)
	{
		mSession->GetListener()->notifyMonExpHiterChange(mSession, src->ID(), play->ID(), play->Name());
	}

	void DomainPlay::NotifyMonExpHiterChange(PlayMapObject* src, int hiter_id, const char * name)
	{
		mSession->GetListener()->notifyMonExpHiterChange(mSession, src->ID(), hiter_id, name);
	}

	bool DomainPlay::IsWarGuildMember(DomainPlay * src)
	{
		std::string id = src->GuildID();
		if (id.size() == 0)
			return false;

		if (mSession && mSession->GetDomainGuild())
		{
			int war_state = 0;
			int time_left = 0;
			mSession->GetDomainGuild()->GetGuildWarState(id, war_state, time_left);
			if (war_state == 1)
				return true;
		}

		return false;
	}

	void DomainPlay::NotifyGhostGuildInfo(DomainPlay* src)
	{
		mSession->GetListener()->notifyGhostGuildInfo(mSession, src);
	}

	void DomainPlay::HpMpChangeBroadcast(int hpChange, int mpChange)
	{
		NotifyHPMPChange(this,hpChange,mpChange);
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos) && (*pos)->TypeIsPlayer())
			{
				(*pos)->NotifyHPMPChange(this, hpChange, mpChange);
			}
		}
	}

	void DomainPlay::GuildChangeBroadcast()
	{
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos) && (*pos)->TypeIsPlayer())
			{
				DomainPlay * player = static_cast<DomainPlay *>(*pos);
				player->NotifyGhostGuildInfo(this);
			}
		}
	}

	void DomainPlay::AddExtraObjFuncs(int id)
	{
		m_extraObjFuncs.push_back(id);

		FuncInfo * pFunc = mGSL->GetObjFuncManager()->GetFuncInfo(id);
		if (!pFunc)
			return;

		if (pFunc->m_nCalAttr == 0)
		{
			for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
			{
				if (pFunc->m_propFuncs[i].mFunc)
					pFunc->m_propFuncs[i].mFunc->onHandle(this);
			}
		}
		else
		{
			// 重新计算属性
			CalculateAttribute();
			NotifyAttributeChange();
			NotifyHPMPChange(this,0,0);
		}
	}

	void DomainPlay::RemExtraObjFuncs(int id)
	{
		std::vector<int>::iterator it = m_extraObjFuncs.begin();
		bool found = false;
		while (it != m_extraObjFuncs.end())
		{
			if (*it == id)
			{
				m_extraObjFuncs.erase(it);
				found = true;
				break;
			}
			++ it;
		}

		if (found)
		{
			FuncInfo * pFunc = mGSL->GetObjFuncManager()->GetFuncInfo(id);
			if (!pFunc)
				return;

			if (pFunc->m_nCalAttr == 0)
			{
				for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
				{
					if (pFunc->m_propFuncs[i].mFunc)
						pFunc->m_propFuncs[i].mFunc->onHandleRemove(this);
				}
			}
			else
			{
				// 重新计算属性
				CalculateAttribute();
				NotifyAttributeChange();
				NotifyHPMPChange(this,0,0);
			}
		}
	}

	void DomainPlay::AddBelongMonster(int monsterId)
	{
		std::vector<int>::iterator it = std::find(mBelongMons.begin(), mBelongMons.end(), monsterId);
		if (it != mBelongMons.end())
		{
			return;
		}
		mBelongMons.push_back(monsterId);
	}

	void DomainPlay::ClearBelongMonster()
	{
		for(std::vector<int>::iterator pos = mBelongMons.begin();
			pos!=mBelongMons.end();
			++pos)
		{
			if( (*pos) )
			{
				//取怪物，通知取消归属
				PlayMonster* mon = static_cast<PlayMonster *>(mGSL->GetMonsterManager()->FindMonsterById((*pos)));
				if (mon && mon->GetExpHitterId() == ID())
				{
					mon->clearExpHiter();
				}
			}
		}

		mBelongMons.clear();
	}
}
LEUD::PoolMemory<OGSLib::DomainPlay>	LEUD::PoolMemoryBase<OGSLib::DomainPlay>::memPool("DomainPlay");
