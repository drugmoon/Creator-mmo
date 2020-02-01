#pragma once
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{
	class DomainCouple;
	class GameServerLib;
	struct CoupleNode
	{
	public:
		CoupleNode()
		{
			online_state = 0;
			player = 0;
			gender = 0;
			job = 0;
			level = 0;
			married_times = 0;
			lastLoginTime = 0;
		}
		std::string nick_name;
		std::string seed_name;
		int online_state;
		DomainCouple* player;
		int gender;
		int job;
		int level;
		int married_times;
		int lastLoginTime;
	};
	class Couple : public LEUD::PoolMemoryBase< Couple >
	{
	public:
		typedef std::map<std::string,CoupleNode> MemberMap;
		typedef std::map<std::string,std::string> CoupleMap;
		typedef std::vector<std::string> CoupleList;
	private:
		int mCoupleID;
		int mWeddingDate;
		int mState;
		bool mNeedSeedName;
		std::string mMaleDesp;
		std::string mFemaleDesp;
		MemberMap mMembers;
		CoupleMap mCouple;
		std::string mParamDefault;
	public:
		GameServerLib* mGameServerLib;
		Couple(void);
		virtual ~Couple(void);

		int CoupleID(){return mCoupleID;}
		void CoupleID(int id){mCoupleID = id;}

		int WeddingDate(){return mWeddingDate;}
		void SetWeddingDate(int date){mWeddingDate = date;}
		int State(){return mState;}
		void State(int st){mState = st;}
		bool NeedSeedName(){return mNeedSeedName;}
		void ReSetNeedSeedName(){ mNeedSeedName = false;}
		void ReSetSeedName(DomainCouple* dc);
		int Update();

		int SetMaleDesp(std::string& name);
		const std::string& GetMaleDesp();
		int SetFemaleDesp(std::string& name);
		const std::string& GetFemaleDesp();

		const char* GetLoverName(DomainCouple* dc);
		const char* GetLoverSeedName(DomainCouple* dc);
		DomainCouple* GetLoverMember(DomainCouple* dc);
		CoupleNode* GetMember(std::string& name);
		int AddMember(const std::string& seed_name,const std::string& nick_name,int gender,int job,int level,int marrytimes,int last);
		int ClearMembers();
		int RemMember(const std::string& nick_name);
		const MemberMap& GetMembers(){return mMembers;}

		int SetMemberOnlineState(const std::string& nick_name,int online_state,DomainCouple* player);

		int Marry(DomainCouple* dc1,DomainCouple* dc2);
		int CoupleChat(DomainCouple* domain,char* msg);
	private:
		void publicGroupInfoChange();
		void publicGroupState();
		void publicGroupInfo();
		void publicCoupleAlert(int lv,int flags,const char* msg);
	};
}