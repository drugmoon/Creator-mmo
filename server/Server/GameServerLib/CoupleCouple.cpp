#include "StdAfx.h"
#include "CoupleCouple.h"
#include "DomainCouple.h"
#include "GameServerLib.h"
namespace OGSLib
{
	Couple::Couple(void)
	{
		mGameServerLib = 0;
		mCoupleID = 0;
		mWeddingDate = 0;
		mState = 0;
		mNeedSeedName = false;
	}
	Couple::~Couple(void)
	{
	}
	const std::string& Couple::GetMaleDesp()
	{
		return mMaleDesp;
	}
	int Couple::SetMaleDesp(std::string& name)
	{
		mMaleDesp=name;
		return 0;
	}
	const std::string& Couple::GetFemaleDesp()
	{
		return mFemaleDesp;
	}
	int Couple::SetFemaleDesp(std::string& name)
	{
		mFemaleDesp=name;
		return 0;
	}
	int Couple::AddMember(const std::string& seed_name,const std::string& nick_name,int gender,int job,int level,int marry_times,int last)
	{
		CoupleNode cn;
		cn.seed_name = seed_name;
		cn.nick_name = nick_name;
		cn.online_state = 1;
		cn.gender = gender;
		cn.job = job;
		cn.level = level;
		cn.married_times = marry_times;
		cn.lastLoginTime = last;
		mMembers[seed_name] = cn;
		if(0 == seed_name.compare(nick_name))
		{
			mNeedSeedName = true;
		}
		return 0;
	}
	void Couple::ReSetSeedName(DomainCouple* dc)
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if (pos->second.player == dc)
			{
				pos->second.seed_name = dc->SeedName();
			}
		}
	}
	int Couple::ClearMembers()
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if (pos->second.player)
			{
				pos->second.player->coupleID(0);
				pos->second.player->weddingDate(0);
			}
		}
		return 0;
	}
	int Couple::RemMember(const std::string& nick_name)
	{
		MemberMap::iterator pos = mMembers.find(nick_name);
		if( pos != mMembers.end() )
		{
			if( pos->second.player )
			{
				pos->second.player->coupleID(0);
				pos->second.player->weddingDate(0);
			}
		}
		return 0;
	}
	int Couple::Marry(DomainCouple* dc1,DomainCouple* dc2)
	{
		CoupleNode cn1;
		cn1.player = dc1;
		cn1.seed_name = dc1->SeedName();
		cn1.nick_name = dc1->Name();
		cn1.gender = dc1->gender();
		cn1.job = dc1->job();
		cn1.level = dc1->level();
		cn1.married_times = dc1->marry_times();
		cn1.lastLoginTime = dc1->lastLogin();
		mMembers[dc1->SeedName()] = cn1;
		dc1->coupleID(CoupleID());
		dc1->weddingDate(WeddingDate());

		CoupleNode cn2;
		cn2.player = dc2;
		cn2.seed_name = dc2->SeedName();
		cn2.nick_name = dc2->Name();
		cn2.gender = dc2->gender();
		cn2.job = dc2->job();
		cn2.level = dc2->level();
		cn2.married_times = dc2->marry_times();
		cn2.lastLoginTime = dc2->lastLogin();
		mMembers[dc2->SeedName()] = cn2;
		dc2->coupleID(CoupleID());
		dc2->weddingDate(WeddingDate());
		return 0;
	}
	const char* Couple::GetLoverName(DomainCouple* dc)
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if (pos->second.gender != dc->gender())
			{
				return pos->second.nick_name.c_str();
			}
		}
		return mParamDefault.c_str();
	}
	const char* Couple::GetLoverSeedName(DomainCouple* dc)
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if (pos->second.gender != dc->gender())
			{
				return pos->second.seed_name.c_str();
			}
		}
		return mParamDefault.c_str();
	}
	DomainCouple* Couple::GetLoverMember(DomainCouple* dc)
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if (pos->second.player && pos->second.gender != dc->gender())
			{
				return pos->second.player;
			}
		}
		return 0;
	}
	CoupleNode* Couple::GetMember(std::string& name)
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			return &(pos->second);
		}
		return 0;
	}
	int Couple::SetMemberOnlineState(const std::string& nick_name,int online_state,DomainCouple* player)
	{
		MemberMap::iterator pos = mMembers.find(nick_name);
		if( pos != mMembers.end() )
		{
			pos->second.online_state = online_state;
			pos->second.player = player;
			return pos->second.online_state;
		}
		return 0;
	}
}
LEUD::PoolMemory<OGSLib::Couple>	LEUD::PoolMemoryBase<OGSLib::Couple>::memPool("Couple");