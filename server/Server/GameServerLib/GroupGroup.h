#pragma once
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{
	enum 
	{
		MAX_GROUP_MEMBER = 5,
	};

	class DomainGroup;
	class GameServerLib;
	class GroupNode
	{
	public:
		enum GN_STATE
		{
			GN_STATE_OFFLINE=100,
			GN_STATE_IDLE=101,
			GN_STATE_DEAD=102,
		};
		DomainGroup* session;
		int state;
		int hp;
		int mp;
		int job;
		int level;
		int power;
		int gender;
		std::string cur_map;
	};
	class Group : public LEUD::PoolMemoryBase< Group >
	{
	public:
		typedef std::map<std::string,GroupNode> MemberMap;
		typedef std::map<std::string,std::string> ParamMap;
	private:
		int mID;
		int mPickMode;
		std::string mTaskFlag;
		std::string mLeader;
		std::string mName;
		MemberMap mMembers;
		ParamMap mParams;
		std::string mParamDefault;
	public:
		GameServerLib* mGameServerLib;
		bool mDeleted;
		bool mLock;
		Group(int id)
		{
			mID=id;
			mPickMode = 0;
			mLock=false;
			mDeleted=false;
			mTaskFlag="";
		}
		int ID(){return mID;}
		int Update();

		int SetLeader(std::string& nick_name);
		const std::string& GetLeader();

		void SetPickMode();
		int GetPickMode();

		int SetName(std::string& name);
		const std::string& GetName();

		const std::string& Var(const std::string& key);
		void Var(const std::string& key,const std::string& value);

		int AddMember(std::string& nick_name,DomainGroup* session);
		int RemMember(std::string& nick_name);
		bool IsMember(std::string& nick_name);
		const MemberMap& GetMembers(){return mMembers;}

		int Online(std::string& nick_name,DomainGroup* session);
		int Offline(std::string& nick_name,DomainGroup* session);

		const char* GetMarryMemberName(std::string& nick_name);
		int clearCoupleStatus();
		int checkCoupleStatus();

		int GroupChat(DomainGroup* domain,char* msg);
		void ExeCode(const char* code);
		int ExeGroupNum();
		void SetTaskFlag(std::string flags){mTaskFlag = flags;}
		const std::string& GetTaskFlag(){return mTaskFlag;}
	private:
		void publicGroupInfoChange();
		void publicGroupState();
		void publicGroupInfo();
		void publicGroupAlert(int lv,int flags,const char* msg);
	};
}