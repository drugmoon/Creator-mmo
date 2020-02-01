#pragma once
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{
	class GameSession;
	class DomainData;
	class GameServerLib;
	class Couple;
	class DomainCouple : public LEUD::PoolMemoryBase< DomainCouple >
	{
		GameServerLib* mGSL;
		GameSession* mSession;
		DomainData* mCharacter;

		std::string mCoupleName;
		int mCoupleTitle;
		std::string mParamDefault;
		int mLeaveCoupletime;
	public:
		DomainCouple(GameSession* session);
		virtual ~DomainCouple(void);
		void initCoupleHandle();
		void destCoupleHandle();
		const std::string& CoupleName(){return mCoupleName;}
		void clearCoupleName(){mCoupleName = "";}
		int CoupleTitle(){return mCoupleTitle;}
		void CoupleTitle(int title){mCoupleTitle=title;}
	public:
		GameSession* Session(){return mSession;}
		const char* Name();
		const char* SeedName();
		int coupleID();
		void coupleID(int id);
		int weddingDate();
		void weddingDate(int date);
		int level();
		int gender();
		int job();
		int marry_times();
		void add_marry_times();
		int lastLogin();
	public:
		int MarryInvite(const char* invitee,const char* male_desp);
		int AgreeOrNotMarryInvite(int agree,const char* male_name,const char* male_desp,const char* female_name,const char* female_desp);
		int AgreeOrNotDivorceInvite(int agree);
		int Divorce();
		const char* GetLoverName();
		const char* GetLoverSeedName();
		int SetCoupleOnlineState(bool online);
		int ForceMoveToLoverPos();
		int ForceFollowMoveHere();
	public:
		virtual void NotifyCoupleNameLoad();
		virtual void NotifyMarrySuc(int couple_id,int weddingdate,const char* male_name,const char* male_desp,const char* female_name,const char* female_desp);
		void notifyCoupleInfoChange();
		void NotifyAlert(int pos,int flags,const char* msg);
		void NotifyMarryInvite(DomainCouple* dc,const char* male_desp);
	};
}