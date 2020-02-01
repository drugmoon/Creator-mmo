#include "StdAfx.h"
#include "DomainCouple.h"
#include "GameSession.h"
#include "DomainData.h"
#include "GameServerLib.h"
#include "ManagerCouple.h"
#include "CoupleCouple.h"
#include "DomainPlay.h"
#include "ManagerFilter.h"
namespace OGSLib
{
	DomainCouple::DomainCouple(GameSession* session)
	{
		mSession = session;
		mGSL = mSession->GetGameServerLib();
		mCharacter = mSession->GetCharacter();
	}
	DomainCouple::~DomainCouple(void)
	{
	}
	const char* DomainCouple::Name()
	{
		return mSession->CharID();
	}
	const char* DomainCouple::SeedName()
	{
		return mSession->GetPlayer()->SeedName();
	}
	const char* DomainCouple::GetLoverName()
	{
		if(weddingDate() > 0)
		{
			Couple* cp = mGSL->GetCoupleManager()->FindCouple(weddingDate());
			if (cp)
			{
				return cp->GetLoverName(this);
			}
		}
		return mParamDefault.c_str();
	}
	const char* DomainCouple::GetLoverSeedName()
	{
		if(weddingDate() > 0)
		{
			Couple* cp = mGSL->GetCoupleManager()->FindCouple(weddingDate());
			if (cp)
			{
				return cp->GetLoverSeedName(this);
			}
		}
		return mParamDefault.c_str();
	}
	int DomainCouple::level()
	{
		return mSession->GetPlayer()->level();
	}
	int DomainCouple::gender()
	{
		return mSession->GetPlayer()->gender();
	}
	int DomainCouple::job()
	{
		return mSession->GetPlayer()->job();
	}
	int DomainCouple::marry_times()
	{
		return 0;
		//return mSession->GetPlayer()->MarryTimes();
	}
	void DomainCouple::add_marry_times()
	{
		//mSession->GetPlayer()->addMarryTimes();
	}
	int DomainCouple::lastLogin()
	{
		return 0;//last;
	}
	int DomainCouple::coupleID()
	{
		return 0;
		//return mSession->GetPlayer()->CoupleID();
	}
	void DomainCouple::coupleID(int id)
	{
		//mSession->GetPlayer()->CoupleID(id);
	}
	int DomainCouple::weddingDate()
	{
		return 0;
		//return mSession->GetPlayer()->Weddingdate();
	}
	void DomainCouple::weddingDate(int date)
	{
		//mSession->GetPlayer()->Weddingdate(date);
	}
	void DomainCouple::initCoupleHandle()
	{
		SetCoupleOnlineState(true);
		if( weddingDate() > 0 )
		{
			Couple* cp = mSession->GetGameServerLib()->GetCoupleManager()->FindCouple(weddingDate());
			if( cp )
			{
				if (cp->GetMembers().size() != 2)
				{
					coupleID(0);
					weddingDate(0);
					cp->ClearMembers();
				}
				if(cp->NeedSeedName())
				{
					cp->ReSetSeedName(this);
				}
				std::string sdn = SeedName();
				std::string name = Name();
				CoupleNode* cn = cp->GetMember(sdn);
				if( 0 != cn->nick_name.compare(name) )
				{
					cn->nick_name = name;
				}
			}
			else
			{
				coupleID(0);
				weddingDate(0);
			}
		}
	}
	void DomainCouple::destCoupleHandle()
	{
		SetCoupleOnlineState(false);
	}
	int DomainCouple::MarryInvite(const char* inv,const char* male_desp)
	{
		
		return 0;
	}
	int DomainCouple::AgreeOrNotMarryInvite(int agree,const char* male_name,const char* male_desp,const char* female_name,const char* female_desp)
	{
		
		return 0;
	}
	int DomainCouple::AgreeOrNotDivorceInvite(int agree)
	{
		
		return 0;
	}
	int DomainCouple::Divorce()
	{
		
		return 0;
	}
	int DomainCouple::SetCoupleOnlineState(bool online)
	{
		if( weddingDate()<=0 )return 0;
		Couple* couple = mGSL->GetCoupleManager()->FindCouple(weddingDate());
		if( couple )
		{
			if( online )
			{
				couple->SetMemberOnlineState(SeedName(),1,this);
			}
			else
			{
				couple->SetMemberOnlineState(SeedName(),0,0);
			}
		}
		return 0;
	}
	int DomainCouple::ForceMoveToLoverPos()
	{
		
		return 0;
	}
	int DomainCouple::ForceFollowMoveHere()
	{
		
		return 0;
	}
	void DomainCouple::NotifyMarryInvite(DomainCouple* dc,const char* male_desp)
	{
		//mSession->GetListener()->notifyMarryInvite(mSession,dc->mSession->CharID(),male_desp);
	}
	void DomainCouple::NotifyCoupleNameLoad()
	{
		mSession->GetListener()->notifyNameLoad(mSession);
	}
	void DomainCouple::NotifyMarrySuc(int couple_id,int weddingdate,const char* male_name,const char* male_desp,const char* female_name,const char* female_desp)
	{
		//mSession->GetListener()->notifyMarrySuc(mSession,couple_id,weddingdate,male_name,male_desp,female_name,female_desp);
	}
	void DomainCouple::NotifyAlert(int pos,int flags,const char* msg)
	{
		mSession->GetListener()->notifyAlert(mSession,pos,flags,msg);
	}
}

LEUD::PoolMemory<OGSLib::DomainCouple>	LEUD::PoolMemoryBase<OGSLib::DomainCouple>::memPool("DomainCouple");