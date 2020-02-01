#include "StdAfx.h"
#include "DomainData.h"
#include "SubItem.h"
#include "SubSkill.h"
#include "SubTask.h"
#include "SubMail.h"
#include "SubStatus.h"
#include "SubGift.h"
#include "SubFriend.h"
#include "SubAchieve.h"

namespace OGSLib
{
	DomainData::DomainData(void)
	{
		level = 1;
		exp = 0;
		money = 0;
		moneybind = 0;
		vcoin = 0;
		vcoings = 0;
		vcoinbind = 0;
		vcoinaccu = 0;

		mVcoinAccuByMonth = 0;
		mLastCongzhiMonth = 0;
		mVcoinUsedByDay = 0;
		mVcoinUsedByMonth = 0;
		mLastVcoinUsedDay = 0;

		job=100;
		gender=200;

		exit_x=0;
		exit_y=0;

		safe_x=0;
		safe_y=0;

		mHonor=0;

		pkvalue = 0;
		talk_prohibited =0;

 		depot_slot_add=0;
		bag_slot_add=0;
		mFreeDirectFly=0;

		mGuildPt=0;
		mGuildSpt=0;

		mOnlineTimeThis = 0;
		online_time_all=0;
		online_time_today=0;
		online_tag_today=0;
		online_time_yestoday=0;
		online_tag_yestoday=0;
		online_time_tick=0;
		china_id_ok=0;
		offline_time=0;
		offline_tag=0;

		china_id[0]=0;
		china_nm[0]=0;
		last_login_ip[0]=0;
		mLoginIP[0]=0;
		mGuildName[0]=0;
		mGuildID[0]=0;
		safe_map[0]=0;
		exit_map[0]=0;
		mAccount[0]=0;
		mChrName[0]=0;
		mSeedName[0]=0;

		cur_hp=0;
		cur_mp=0;
		cur_power=0;

		//mCoupleID = 0;
		//mWeddingdate = 0;
		//mMarryTimes = 0;
		
		china_limit_online_time_count=0;
		china_limit_offline_time_tag=0;

		mAchievePoint=0;
		mAchieveGameMoneyMax=0;
		mOnlineTimeMax=0;
		mLoginDayContinue=0;
		mLoginDayContinueMax=0;
		mLoginDayCount=0;

		mItemDataVersion = 0;

		mKuafuResult = 0;
		mCloth = 0;
		mWeapon = 0;
		mWing = 0;
		mFashionCloth = 0;
		mFashionWeapon = 0;
		mMainTaskState = 0;
	}
	DomainData::~DomainData(void)
	{
	}
	void DomainData::Load()
	{
		online_time_tick = time(0);
		online_time = time(0);
		if( offline_tag > 1000 )
		{
			offline_time +=	(online_time_tick - offline_tag);
			if (offline_time < 0)
			{
				offline_time = 0;
			}
		}
	}

	void DomainData::Save(bool quit)
	{
		freshonlinetime();
	}
	void DomainData::freshonlinetime()
	{
		time_t curTime = time(0);
		time_t curDay = (curTime / (60*60) + 8) / 24;

		time_t playTime = curTime - online_time_tick;	
		online_time_all += playTime;
		mOnlineTimeThis += playTime;
		online_time_tick = curTime;

		if( online_tag_today != curDay )
		{
			if( online_tag_today == curDay-1 )
			{
				mLoginDayContinue ++;
			}
			else
			{
				mLoginDayContinue = 1;
			}
			if( mLoginDayContinue > mLoginDayContinueMax )
			{
				mLoginDayContinueMax = mLoginDayContinue;
			}
			mLoginDayCount ++;
			online_time_yestoday = online_time_today;
			online_tag_yestoday = online_tag_today;
			online_tag_today = curDay;
			online_time_today = playTime;
		}
		else
		{
			online_time_today += playTime;
		}

		china_limit_online_time_count += (int)playTime;
		china_limit_offline_time_tag = (int)curTime;

		offline_tag = curTime;
	}
}
LEUD::PoolMemory<OGSLib::DomainData>	LEUD::PoolMemoryBase<OGSLib::DomainData>::memPool("DomainData");
