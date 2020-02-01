#include "StdAfx.h"
#include "ManagerChargeDart.h"
#include "GameServerLib.h"
#include "DomainPlay.h"
#include "GameSession.h"

namespace OGSLib
{
	ChargeDart::ChargeDart()
	{
		mDartCar = 0;
		mIcon = 0;
		mStartTime = 0;
		mDuration = 0;
		mFightingForce = 0;
		mStolenTimes = 0;
		mTotalAwards = 0;
		mSurplusAwards = 0;
		mState = 0;
	}

	ManagerChargeDart::ManagerChargeDart(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerChargeDart::~ManagerChargeDart(void)
	{
	}
	int ManagerChargeDart::init()
	{
		return 0;
	}
	void ManagerChargeDart::dest(bool save)
	{
		for( ChargeDartMap::iterator pos = mChargeDarts.begin();
			pos != mChargeDarts.end();
			++ pos )
		{
			ChargeDart* g = pos->second;
			delete g; g = 0;
		}
	}
	void ManagerChargeDart::Update(double time)
	{
		for( ChargeDartMap::iterator pos = mChargeDarts.begin();
			pos != mChargeDarts.end();
			++ pos )
		{
			ChargeDart* g = pos->second;
			if(g)
			{
				if( g->mState == 0 && time - g->mStartTime >= g->mDuration )
				{
					g->mState = 1;
					OGSLib::GameSession* session = mGameServerLib->FindSessionWithCharID(g->mCharName);
					if (session && session->GetPlayer())
					{
						session->GetPlayer()->NotifyChargeDartList();
					}
				}
			}
		}

		for( ChargeDartMap::iterator pos = mChargeDarts.begin();
			pos != mChargeDarts.end();)
		{
			ChargeDart* g = pos->second;
			if(g)
			{
				if( g->mState == 2)
				{
					mChargeDarts.erase(pos++);
				}
				else
				{
					pos++;
				}
			}
			else
			{
				pos++;
			}
		}

	}
	void ManagerChargeDart::StartChargeDart(std::string charName, int dartCar ,int icon,int duration,int fightingForce,int totalAwards,int dartLevel)
	{
		ChargeDart* cd;
		ChargeDartMap::iterator pos = mChargeDarts.find(charName);
		if(pos != mChargeDarts.end())
		{
			cd = pos->second;
		}
		else
		{
			cd = new ChargeDart;
			mChargeDarts[charName] = cd;
		}
		cd->mCharName = charName;
		cd->mDartCar = dartCar;
		cd->mIcon = icon;
		cd->mStartTime =mGameServerLib->GetTimerFix()->Elapsed();
		cd->mDuration = duration;
		cd->mFightingForce = fightingForce;
		cd->mStolenTimes = 0;
		cd->mTotalAwards = totalAwards;
		cd->mSurplusAwards = totalAwards;
		cd->mState = 0;
		cd->mDartLevel = dartLevel;
	}
	ChargeDart* ManagerChargeDart::GetChargeDart(std::string charName)
	{
		ChargeDartMap::iterator pos = mChargeDarts.find(charName);
		if(pos != mChargeDarts.end())
		{
			return pos->second;
		}
		return 0;
	}
	void ManagerChargeDart::SetChargeDart(std::string charName, int dartCar,int icon,double startTime,int duration,int fightingForce,int stolenTimes,int totalAwards,int surplusAwards,int state,std::string robDartCharNames)
	{
		ChargeDartMap::iterator pos = mChargeDarts.find(charName);
		if(pos != mChargeDarts.end())
		{
			ChargeDart* cd = pos->second;
			cd->mCharName = charName;
			cd->mDartCar = dartCar;
			cd->mIcon = icon;
			cd->mStartTime = startTime;
			cd->mDuration = duration;
			cd->mFightingForce = fightingForce;
			cd->mStolenTimes = stolenTimes;
			cd->mTotalAwards = totalAwards;
			cd->mSurplusAwards = surplusAwards;
			cd->mState = state;
			cd->mRobDartCharNames = robDartCharNames;
			OGSLib::GameSession* session = mGameServerLib->FindSessionWithCharID(charName);
			if (session && session->GetPlayer())
			{
				session->GetPlayer()->NotifyChargeDartList();
			}
		}
	}
}
LEUD::PoolMemory<OGSLib::ChargeDart>	LEUD::PoolMemoryBase<OGSLib::ChargeDart>::memPool("ChargeDart");