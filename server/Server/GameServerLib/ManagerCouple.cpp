#include "StdAfx.h"
#include "ManagerCouple.h"
#include "GameServerLib.h"
#include "CoupleCouple.h"
namespace OGSLib
{
	ManagerCouple::ManagerCouple(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerCouple::~ManagerCouple(void)
	{
	}
	Couple* ManagerCouple::CreateCouple()
	{
		Couple* c=new Couple;
		c->CoupleID(GetCoupleServerID());
		c->SetWeddingDate(static_cast<int>(time(0)));
		while( FindCouple(c->WeddingDate()) )
		{
			c->SetWeddingDate(static_cast<int>(time(0))+1);
		}
		c->mGameServerLib = mGameServerLib;
		mCouples[c->WeddingDate()]=c;
		//SaveGuilds();
		return c;
	}
	int ManagerCouple::GetCoupleServerID()
	{
		int couple_serID = 0;
		int countCur = 0;
		for( CoupleMap::iterator pos = mCouples.begin();
			pos != mCouples.end();
			++ pos )
		{
			if (couple_serID < pos->second->CoupleID())
			{
				couple_serID = pos->second->CoupleID();
				countCur++;
			}
		}
		couple_serID++;
		if (countCur > couple_serID){couple_serID = countCur;}
		return couple_serID;
	}
	void ManagerCouple::DeleteCouple(int seed_id)
	{
		Couple* g = FindCouple(seed_id);
		if( g )
		{
			mCouples.erase(seed_id);
			delete g;g=0;
		}
	}
	Couple* ManagerCouple::FindCouple(int seed)
	{
		CoupleMap::iterator pos = mCouples.find(seed);
		if( pos != mCouples.end() )
		{
			return pos->second;
		}
		return 0;
	}
	bool ManagerCouple::LoadCouples()
	{
		mGameServerLib->GetListener()->LoadCouples(0);
		return true;
	}
	bool ManagerCouple::OnLoadCouples(const OGSLib::ManagerCouple::CoupleMap& guild_map)
	{
		mCouples = guild_map;
		for( CoupleMap::iterator pos = mCouples.begin();
			pos != mCouples.end();
			++ pos )
		{
			pos->second->mGameServerLib = mGameServerLib;
		}
		return true;
	}
	void ManagerCouple::update()
	{
		if( mSaveTick + 30*60 < time(0) )
		{
			mSaveTick = time(0);
			SaveCouples();
		}
	}
	bool ManagerCouple::SaveCouples()
	{
		mGameServerLib->GetListener()->SaveCouples(*this,0);
		return true;
	}
	int ManagerCouple::init()
	{
		LoadCouples();
		mSaveTick = time(0);
		return 0;
	}
	void ManagerCouple::dest(bool save)
	{
		if( save )
		{
			SaveCouples();
		}
		for( CoupleMap::iterator pos = mCouples.begin();
			pos != mCouples.end();
			++ pos )
		{
			Couple* g = pos->second;
			delete g;g=0;
		}
	}
}