#include "StdAfx.h"
#include "DomainGroup.h"
#include "ManagerGroup.h"
#include "GameSession.h"
#include "GameServerlib.h"
#include "DomainPlay.h"
#include "GroupGroup.h"
#include "ManagerFilter.h"
#include "PlayMap.h"
namespace OGSLib
{
	DomainGroup::DomainGroup(GameSession* session)
	{
		mSession = session;
		mGSL = mSession->GetGameServerLib();
		mCharacter = mSession->GetCharacter();
		mGroupID=0;
		mTempStr.reserve(256);
	}
	DomainGroup::~DomainGroup(void)
	{
	}
	void DomainGroup::initGroupHandle()
	{
		mGroupID=0;
		mTempStr=mSession->CharID();
		Group* group = mGSL->GetGroupManager()->FindGroupWithNickName(mTempStr);
		if( group )
		{
			mGroupID = group->ID();
			group->Online(mTempStr,this);
			if(mSession->GetPlayer())
				mSession->GetPlayer()->OnEnterGroup();
		}
	}
	void DomainGroup::destGroupHandle()
	{
		if( mGroupID > 0 )
		{
			mTempStr=mSession->CharID();
			Group* group = mGSL->GetGroupManager()->FindGroupWithNickName(mTempStr);
			if( group )
			{
				group->Offline(mTempStr,this);
			}
			if(mSession->GetPlayer())
				mSession->GetPlayer()->OnLeaveGroup();
			mGroupID = 0;
		}
	}
	void DomainGroup::Save()
	{
	}
	int DomainGroup::EnterGroup(int gid)
	{
		if( mSession->AuthType() != 101 )
		{
			return 0;
		}
		if( mGroupID == gid ) return 0;
		LeaveGroup();
		Group* g = mGSL->GetGroupManager()->FindGroup(gid);
		if( g )
		{
			mGroupID=g->ID();
			mTempStr=mSession->CharID();
			g->AddMember(mTempStr,this);
			if(mSession->GetPlayer())
				mSession->GetPlayer()->OnEnterGroup();
		}
		return 0;
	}
	int DomainGroup::CreateGroup(int flags)
	{
		if( mSession->AuthType() != 101 )
		{
			mSession->GetListener()->notifyAlert(mSession,1,0,mGSL->mStringConsts.NOTIFY_GROUP_CREATE_FAIL_ERROR_AUTH_TYPE);
			return 0;
		}
		if( GroupID() > 0 ) return 0;
		Group* group=mGSL->GetGroupManager()->CreateGroup("");
		if( group )
		{
			EnterGroup(group->ID());
		}
		return 0;
	}
	int DomainGroup::GroupSize()
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				return (int)g->GetMembers().size();
			}
		}
		return 0;
	}
	const std::string& DomainGroup::Var(const std::string& key)
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				return g->Var(key);
			}
		}
		return mParamDefault;
	}

	void DomainGroup::Var(const std::string& key,const std::string& value)
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				return g->Var(key,value);
			}
		}
	}
	void DomainGroup::GroupExe(const char* code)
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				g->ExeCode(code);
			}
		}
	}
	int DomainGroup::GroupMemberOnLineNum()
	{
		int num = 0;
		if (GroupID() > 0)
		{
			Group* g = mGSL->GetGroupManager()->FindGroup(GroupID());
			if (g)
			{
				num = g->ExeGroupNum();
			}
		}
		return num;
	}
	void DomainGroup::ExeCode(const char* code)
	{
		if(mSession->GetPlayer())
			mSession->GetPlayer()->ExeCode(code);
	}
	const std::string& DomainGroup::GetGroupLeader()
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				return g->GetLeader();
			}
		}

		return mParamDefault;
	}

	int DomainGroup::SetGroupLeader(const char* pn)
	{
		std::string player_name=pn;
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g && g->GetLeader()==mSession->CharID() && (!g->mLock) )
			{
				g->SetLeader(player_name);
			}
		}
		return 0;
	}

	const std::string& DomainGroup::GetGroupTaskFlag()
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				return g->GetTaskFlag();
			}
		}

		return mParamDefault;
	}

	const char* DomainGroup::GetMarryGroupMemberName()
	{
		return mParamDefault.c_str();
	}
	int DomainGroup::checkCoupleStatus()
	{
		return 0;
	}
	int DomainGroup::clearCoupleStatus()
	{
		if (mSession && mSession->GetPlayer())
		{
			mSession->GetPlayer()->ClearStatus(28);
		}
		return 0;
	}
	int DomainGroup::PickMode()
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g && g->GetLeader()==mSession->CharID() )
			{
				g->SetPickMode();
			}
		}
		return 0;
	}
	int DomainGroup::RemGroupMember(const char* player_name)
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g && g->GetLeader()==mSession->CharID() && (!g->mLock) )
			{
				mTempStr=player_name;

				if( g->IsMember(mTempStr) )
				{
					GameSession* gs = this->mGSL->FindSessionWithCharID(mTempStr);
					if( gs) 
					{
						gs->GetDomainGroup()->LeaveGroup();
					}
					else
					{
						g->RemMember(mTempStr);
					}
				}
			}
		}
		return 0;
	}
	int DomainGroup::LeaveGroup()
	{
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				if(  (!g->mLock)  )
				{
					if(mSession->GetPlayer())
						mSession->GetPlayer()->OnLeaveGroup();
					mGroupID=0;
					mTempStr=mSession->CharID();
					g->RemMember(mTempStr);
				}
			}
			else
			{
				mGroupID=0;
			}
		}
		return 0;
	}
	int DomainGroup::JoinGroup(int group_id)
	{
		if( mSession->AuthType() != 101 )
		{
			mSession->GetListener()->notifyAlert(mSession,1,0,mGSL->mStringConsts.NOTIFY_GROUP_JOIN_FAIL_ERROR_AUTH_TYPE);
			return 0;
		}
		if( GroupID() > 0 )
		{
			return 1;
		}
		Group* g=mGSL->GetGroupManager()->FindGroup(group_id);
		if( g && g->GetMembers().size() < 6  )
		{
			GameSession* gs=mGSL->FindSessionWithCharID(g->GetLeader());
			if( gs && gs->GetDomainGroup() )
			{
				gs->GetDomainGroup()->NotifyJoinGroupToLeader(g,this);
			}
		}
		return 0;
	}
	int DomainGroup::AgreeJoinGroup(const char* player_name)
	{
		if( mSession->AuthType() != 101 )
		{
			mSession->GetListener()->notifyAlert(mSession,1,0,mGSL->mStringConsts.NOTIFY_GROUP_JOIN_FAIL_ERROR_AUTH_TYPE);
			return 0;
		}
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g && g->GetLeader()==mSession->CharID() && g->GetMembers().size() < MAX_GROUP_MEMBER && (!g->mLock)  )
			{
				GameSession* gs=mGSL->FindSessionWithCharID(player_name);
				if( gs && gs->GetDomainGroup()  )
				{
					if( ! ( gs->GetDomainGroup()->GroupID() > 0 ) )
					{
						gs->GetDomainGroup()->EnterGroup(g->ID());
					}
				}
			}
		}
		return 0;
	}
	int DomainGroup::InviteGroup(const char* pn)
	{
		std::string player_name = pn;
		if( mSession->AuthType() != 101 )
		{
			return 0;
		}
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g && g->GetLeader()==mSession->CharID() && g->GetMembers().size() < MAX_GROUP_MEMBER  )
			{
				GameSession* gs=mGSL->FindSessionWithCharID(player_name);
				if( gs && gs->GetDomainGroup() )
				{
					gs->GetDomainGroup()->NotifyInviteGroupToMember(g,this);
				}
			}
		}
		return 0;
	}
	int DomainGroup::GroupChat(char* msg)
	{
		if( mSession->AuthType() != 101 )
		{
			return 0;
		}
		if( mSession && mSession->GetPlayer() && mSession->GetPlayer()->GetGMHideTalk() )
		{
			return 0;
		}
		if( GroupID() > 0 )
		{
			Group* g=mGSL->GetGroupManager()->FindGroup(GroupID());
			if( g )
			{
				mGSL->GetFilterManager()->Filter(msg);
				g->GroupChat(this,msg);
			}
		}
		return 0;
	}
	int DomainGroup::AgreeInviteGroup(const char* ln,int group_id)
	{
		std::string leader_name = ln;
		if( mSession->AuthType() != 101 )
		{
			return 0;
		}
		if( GroupID() > 0 )
		{
			return 1;
		}
		Group* g=mGSL->GetGroupManager()->FindGroup(group_id);
		if( g && g->GetMembers().size() < MAX_GROUP_MEMBER && (!g->mLock) )
		{
			EnterGroup(g->ID());
		}
		return 0;
	}
	void DomainGroup::NotifyGroupInfoChange(Group* group)
	{
		if( group )
		{
			mPickMode = group->GetPickMode();
		}
		else
		{
			mPickMode = 0;
		}
		mSession->GetListener()->notifyGroupInfoChange(mSession,group);
	}
	void DomainGroup::NotifyGroupState(Group* group)
	{
		mSession->GetListener()->notifyGroupState(mSession,group);
	}
	void DomainGroup::NotifyGroupInfo(Group* group)
	{
		if( mSession->GetPlayer() )
		{
			if( group )
			{
				mSession->GetPlayer()->pushGroupInfo(group->GetName(),group->GetLeader());
			}
			else
			{
				std::string nu="";
				mSession->GetPlayer()->pushGroupInfo(nu,nu);
			}
		}
	}
	void DomainGroup::NotifyInviteGroupToMember(Group* group,DomainGroup* gh)
	{
		mSession->GetListener()->notifyInviteGroupToMember(mSession,gh->mSession->CharID(),group->ID());
	}
	void DomainGroup::NotifyJoinGroupToLeader(Group* group,DomainGroup* gh)
	{
		mSession->GetListener()->notifyJoinGroupToLeader(mSession,gh->mSession->CharID());
	}
	void DomainGroup::NotifyGroupChat(Group* group,DomainGroup* domain,const char* msg)
	{
		mSession->GetListener()->notifyGroupChat(mSession,group,domain,msg);
	}
	void DomainGroup::NotifyAlert(int pos,int flags,const char* msg)
	{
		mSession->GetListener()->notifyAlert(mSession,pos,flags,msg);
	}
	bool DomainGroup::IsDead()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->IsDead();
		else
			return false;
	}
	const char* DomainGroup::Name()
	{
		if(mSession->GetPlayer())
			return mSession->CharID();
		else
			return "";
	}

	int DomainGroup::Hp()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->Hp();
		else
			return 0;
	}
	int DomainGroup::Mp()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->Mp();
		else
			return 0;
	}
	const char* DomainGroup::map_name()
	{
		if(mSession->GetPlayer())
		{
			PlayMap* map = mSession->GetPlayer()->GetMap();
			if( map )
			{
				return map->GetName().c_str();
			}
			return mSession->CharID();
		}
		else
			return "";
	}
	int DomainGroup::x()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->X();
		else
			return 0;
	}
	int DomainGroup::y()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->Y();
		else
			return 0;
	}
	int DomainGroup::level()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->level();
		else
			return 0;
	}
	int DomainGroup::gender()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->gender();
		else
			return 200;
	}
	int DomainGroup::job()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->job();
		else
			return 100;
	}
	int DomainGroup::power()
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->FightPoint();
		else
			return 0;
	}
	LEUD_INT64 DomainGroup::add_exp(int exp)
	{
		if(mSession->GetPlayer())
			return mSession->GetPlayer()->AddExperience(exp,100);
		else
			return 0;
	}

	DomainPlay * DomainGroup::GetPlayer()
	{
		return mSession->GetPlayer();
	}

}
LEUD::PoolMemory<OGSLib::DomainGroup>	LEUD::PoolMemoryBase<OGSLib::DomainGroup>::memPool("DomainGroup");
