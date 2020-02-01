#include "StdAfx.h"
#include "ManagerGroup.h"
#include "GameServerLib.h"
#include "DomainGroup.h"
#include "GroupGroup.h"
namespace OGSLib
{

	ManagerGroup::ManagerGroup(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
		m_id_seed = 100;
		mUpdateGM=0;
		mUpdateGMDuration=3.0;
	}
	ManagerGroup::~ManagerGroup(void)
	{
	}
	int ManagerGroup::init()
	{
		return 0;
	}
	void ManagerGroup::dest()
	{
		for(GroupMap::iterator pos=m_Groups.begin();
			pos != m_Groups.end();
			++ pos )
		{
			delete pos->second;
		}
		m_Groups.clear();
	}
	Group* ManagerGroup::CreateGroup(std::string map_name)
	{
		Group* g=new Group(++m_id_seed);
		g->mGameServerLib = mGameServerLib;
		g->SetTaskFlag(map_name);
		m_Groups[g->ID()] = g;
		return g;
	}
	Group* ManagerGroup::FindGroup(int id)
	{
		GroupMap::iterator pos = m_Groups.find(id);
		if( m_Groups.end()!= pos )
		{
			return pos->second;
		}
		return 0;
	}
	Group* ManagerGroup::FindGroupWithNickName(std::string& nick_name)
	{
		for( GroupMap::iterator pos = m_Groups.begin();
			pos != m_Groups.end();
			++ pos )
		{
			if( pos->second && pos->second->IsMember(nick_name) )
			{
				return pos->second;
			}
		}
		return 0;
	}
	Group* ManagerGroup::FindTaskGroup(std::string map_name)
	{
		std::multimap<int, Group*> m;
		for( GroupMap::iterator pos = m_Groups.begin();
			pos != m_Groups.end();
			++ pos )
		{
			if( pos->second && pos->second->GetTaskFlag() == map_name && pos->second->GetMembers().size() < MAX_GROUP_MEMBER )
			{
				m.insert(std::make_pair(MAX_GROUP_MEMBER - pos->second->GetMembers().size(), pos->second));
			}
		}

		if (m.size() > 0)
		{
			std::multimap<int, Group*>::iterator it = m.begin();
			return it->second;
		}

		return 0;
	}
	void ManagerGroup::Update(double time)
	{
		if( mUpdateGM + mUpdateGMDuration < time )
		{
			mUpdateGM = time;
			for( GroupMap::iterator pos = m_Groups.begin();
				pos != m_Groups.end();
				)
			{
				if(pos->second)
				{
					pos->second->Update();
				}
				if( pos->second && pos->second->mDeleted )
				{
					delete pos->second;
					m_Groups.erase(pos++);
				}
				else
				{
					++pos;
				}
			}
		}
	}
}
