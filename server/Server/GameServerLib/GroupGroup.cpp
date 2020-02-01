#include "StdAfx.h"
#include "GroupGroup.h"
#include "GameServerLib.h"
#include "DomainGroup.h"
namespace OGSLib
{
	int Group::Update()
	{
		bool changed=false;
		for( Group::MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			GroupNode gn;
			DomainGroup* session = pos->second.session;
			if( session )
			{
				if( session->IsDead() )
				{
					gn.state = GroupNode::GN_STATE_DEAD;
				}
				else
				{
					gn.state = GroupNode::GN_STATE_IDLE;
				}
				gn.hp = session->Hp();
				gn.mp = session->Mp();
				gn.job = session->job();
				gn.level = session->level();
				gn.power = session->power();
				gn.cur_map = session->map_name();
				gn.gender = session->gender();
			}
			else
			{
				gn.state = GroupNode::GN_STATE_OFFLINE;
				gn.hp = pos->second.hp;
				gn.mp = pos->second.mp;
				gn.job = pos->second.job;
				gn.level = pos->second.level;
				gn.power = pos->second.power;
				gn.cur_map = pos->second.cur_map;
				gn.gender = pos->second.gender;
			}
			if( pos->second.state != gn.state
				|| pos->second.hp != gn.hp
				|| pos->second.mp != gn.mp
				|| pos->second.job != gn.job
				|| pos->second.level != gn.level
				|| pos->second.power != gn.power
				|| pos->second.cur_map != gn.cur_map
				|| pos->second.gender != gn.gender )
			{
				pos->second.state = gn.state;
				pos->second.hp = gn.hp;
				pos->second.mp = gn.mp;
				pos->second.job = gn.job;
				pos->second.level = gn.level;
				pos->second.power = gn.power;
				pos->second.cur_map = gn.cur_map;
				pos->second.gender = gn.gender;
				changed=true;
			}
		}
		if( changed )
		{
			publicGroupState();
		}
		return 0;
	}
	int Group::SetLeader(std::string& nick_name)
	{
		MemberMap::iterator pos = mMembers.find(nick_name);
		if( pos != mMembers.end() )
		{
			mLeader=nick_name;
			publicGroupInfoChange();
			for( MemberMap::iterator pp = mMembers.begin();
				pp != mMembers.end();
				++ pp )
			{
				if( pp->second.session )
				{
					pp->second.session->NotifyGroupInfo(this);
				}
			}
			return 0;
		}
		return 1;
	}
	const std::string& Group::GetLeader()
	{
		return mLeader;
	}
	const char* Group::GetMarryMemberName(std::string& nick_name)
	{
		for( MemberMap::iterator pp = mMembers.begin();
			pp != mMembers.end();
			++ pp )
		{
			if( pp->second.session && pp->second.session->Name() != nick_name)
			{
				return pp->second.session->Name();
			}
		}
		return mParamDefault.c_str();
	}
	int Group::clearCoupleStatus()
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session)
			{
				pos->second.session->clearCoupleStatus();
			}
		}
		return 0;
	}
	int Group::checkCoupleStatus()
	{
		if (mMembers.size() != 2){return 0;}
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session)
			{
				pos->second.session->checkCoupleStatus();
				return 1;
			}
		}
		return 0;
	}
	void Group::SetPickMode()
	{
		if( mPickMode == 0 )
		{
			mPickMode = 1;
		}
		else
		{
			mPickMode = 0;
		}
		publicGroupInfoChange();
	}
	int Group::GetPickMode()
	{
		return mPickMode;
	}
	int Group::SetName(std::string& name)
	{
		mName=name;
		publicGroupInfoChange();
		publicGroupInfo();
		return 0;
	}
	const std::string& Group::Var(const std::string& key)
	{
		std::map<std::string,std::string>::iterator pos = mParams.find(key);
		if( pos != mParams.end() )
		{
			return pos->second;
		}
		return mParamDefault;
	}
	void Group::Var(const std::string& key,const std::string& value)
	{
		if( key.length() )
		{
			mParams[key] = value;
		}
	}
	const std::string& Group::GetName()
	{
		return mName;
	}
	int Group::AddMember(std::string& nick_name,DomainGroup* session)
	{
		GroupNode gn;
		gn.session = 0;
		gn.state = GroupNode::GN_STATE_OFFLINE;
		{
			char temp[256];
			_snprintf_s(temp,_countof(temp),255,mGameServerLib->mStringConsts.NOTIFY_GROUP_MEMBER_ADD,nick_name.c_str());
			temp[255]=0;
			//std::string msg(temp);
			publicGroupAlert(10,0,temp);
		}
		if( session )
		{
			gn.session=session;
			gn.state=GroupNode::GN_STATE_IDLE;
		}
		gn.hp = 0;
		gn.mp = 0;
		gn.job = 0;
		gn.level = 0;
		gn.power = 0;
		gn.cur_map = "";
		gn.gender = 0;
		mMembers[nick_name]=gn;
		if( mLeader.empty() )
		{
			mLeader = nick_name;
		}
		publicGroupInfoChange();
		if( gn.session )
		{
			gn.session->NotifyGroupInfo(this);
			char temp[256];
			_snprintf_s(temp,_countof(temp),255,mGameServerLib->mStringConsts.NOTIFY_JOIN_GROUP,mLeader.c_str());
			temp[255]=0;
			//std::string msg(temp);
			if( mLeader != gn.session->Name() )
			{
				gn.session->NotifyAlert(10,0,temp);
			}
		}
		return 0;
	}
	int Group::RemMember(std::string& nick_name)
	{
		MemberMap::iterator pos = mMembers.find(nick_name);
		if( pos != mMembers.end() )
		{
			if( pos->second.session )
			{
				pos->second.session->NotifyGroupInfoChange(0);
				pos->second.session->NotifyGroupInfo(0);
				pos->second.session->NotifyAlert(10,0,mGameServerLib->mStringConsts.NOTIFY_LEAVE_GROUP);
			}
			mMembers.erase(pos);
			if( mMembers.empty() )
			{
				mDeleted = true;
				return 0;
			}
			else
			{
				if( mLeader == nick_name )
				{
					mLeader = mMembers.begin()->first;
					publicGroupInfo();
				}
			}
			publicGroupInfoChange();
			{
				char temp[256];
				_snprintf_s(temp,_countof(temp),255,mGameServerLib->mStringConsts.NOTIFY_GROUP_MEMBER_REM,nick_name.c_str());
				temp[255]=0;
				//std::string msg(temp);
				publicGroupAlert(10,0,temp);
			}
		}
		return 0;
	}
	bool Group::IsMember(std::string& nick_name)
	{
		MemberMap::iterator pos = mMembers.find(nick_name);
		if( pos != mMembers.end() )
		{
			return true;
		}
		return false;
	}

	int Group::Online(std::string& nick_name,DomainGroup* session)
	{
		MemberMap::iterator pos = mMembers.find(nick_name);
		if( pos != mMembers.end() )
		{
			{
				char temp[256];
				_snprintf_s(temp,_countof(temp),255,mGameServerLib->mStringConsts.NOTIFY_GROUP_MEMBER_ONLINE,nick_name.c_str());
				temp[255]=0;
				//std::string msg(temp);
				publicGroupAlert(10,0,temp);
			}
			//pos->second.state=GroupNode::GN_STATE_IDLE;
			pos->second.session=session;
			publicGroupInfoChange();
			if( pos->second.session )
			{
				pos->second.session->NotifyGroupInfo(this);
			}
		}
		return 0;
	}
	int Group::Offline(std::string& nick_name,DomainGroup* session)
	{
		MemberMap::iterator pos = mMembers.find(nick_name);
		if( pos != mMembers.end() )
		{
			//pos->second.state=GroupNode::GN_STATE_OFFLINE;
			pos->second.session=0;
			{
				char temp[512];
				_snprintf_s(temp,_countof(temp),511,mGameServerLib->mStringConsts.NOTIFY_GROUP_MEMBER_OFFLINE,nick_name.c_str());
				temp[511]=0;
				//std::string msg(temp);
				publicGroupAlert(10,0,temp);
			}
		}
		return 0;
	}
	int Group::GroupChat(DomainGroup* domain,char* msg)
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session && domain != pos->second.session )
			{
				pos->second.session->NotifyGroupChat(this,domain,msg);
				//pos->second.session->NotifyGroupInfoChange(this);
			}
		}
		return 0;
	}
	void Group::ExeCode(const char* code)
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session )
			{
				pos->second.session->ExeCode(code);
			}
		}
	}
	int Group::ExeGroupNum()
	{
		int num = 0;
		for (MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++pos)
		{
			if (pos->second.session)
			{
				num++;
			}
		}
		return num;
	}
	void Group::publicGroupInfoChange()
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session )
			{
				pos->second.session->NotifyGroupInfoChange(this);
			}
		}
	}
	void Group::publicGroupState()
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session )
			{
				pos->second.session->NotifyGroupState(this);
			}
		}
	}
	void Group::publicGroupInfo()
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session )
			{
				pos->second.session->NotifyGroupInfo(this);
			}
		}
	}
	void Group::publicGroupAlert(int lv,int flags,const char* msg)
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.session )
			{
				pos->second.session->NotifyAlert(lv,flags,msg);
			}
		}
	}
}
LEUD::PoolMemory<OGSLib::Group>	LEUD::PoolMemoryBase<OGSLib::Group>::memPool("Group");
