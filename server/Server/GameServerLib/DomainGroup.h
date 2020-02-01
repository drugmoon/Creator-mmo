#pragma once
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{
	class GameSession;
	class DomainData;
	class GameServerLib;
	class Group;
	class DomainPlay;
	class DomainGroup : public LEUD::PoolMemoryBase< DomainGroup >
	{
		GameServerLib* mGSL;
		GameSession* mSession;
		DomainData* mCharacter;
		int mGroupID;
		int mPickMode;
		std::string mTempStr;
		std::string mParamDefault;
	public:
		DomainGroup(GameSession* session);
		virtual ~DomainGroup(void);
		int GroupID(){return mGroupID;};
		void initGroupHandle();
		void destGroupHandle();
		void Save();

		int EnterGroup(int gid);

		int CreateGroup(int flags);
		int SetGroupLeader(const char* player_name);
		const std::string& GetGroupLeader();
		int RemGroupMember(const char* player_name);
		int LeaveGroup();
		int JoinGroup(int group_id);
		int AgreeJoinGroup(const char* player_name);
		int InviteGroup(const char* player_name);
		int AgreeInviteGroup(const char* leader_name,int group_id);
		int GroupChat(char* msg);
		int PickMode();
		int GroupSize();
		void GroupExe(const char* code);
		int GroupMemberOnLineNum();
		int GetPickMode(){return mPickMode;}
		const char* GetMarryGroupMemberName();
		int checkCoupleStatus();
		int clearCoupleStatus();
		DomainPlay * GetPlayer();
	public:
		const char* Name();
		bool IsDead();
		int Hp();
		int Mp();
		const char* map_name();
		int x();
		int y();
		int level();
		int job();
		int power();
		int gender();
		LEUD_INT64 add_exp(int exp);
		const std::string& Var(const std::string& key);
		const std::string& GetGroupTaskFlag();
		void Var(const std::string& key,const std::string& value);
		void ExeCode(const char* code);
		int ExeGroupNum();
	public:
		virtual void NotifyGroupInfoChange(Group* group);
		virtual void NotifyGroupState(Group* group);
		virtual void NotifyGroupInfo(Group* group);

		virtual void NotifyInviteGroupToMember(Group* group,DomainGroup* gh);
		virtual void NotifyJoinGroupToLeader(Group* group,DomainGroup* gh);

		void NotifyGroupChat(Group* group,DomainGroup* domain,const char* msg);
		void NotifyAlert(int pos,int flags,const char* msg);
	};
}