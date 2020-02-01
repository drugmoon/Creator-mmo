#include "stdafx.h"
#include "ManagerRelationCache.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include <sstream>
#include "DomainPlay.h"

namespace OGSLib
{
	ManagerRelationCache::ManagerRelationCache(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}

	ManagerRelationCache::~ManagerRelationCache(void)
	{

	}

	void ManagerRelationCache::update()
	{
		if( mSaveTick + 42*60 < time(0) )
		{
			mSaveTick = time(0);
			SaveRelationCache();
		}
	}

	void ManagerRelationCache::init()
	{
		LoadRelationCache();
		mSaveTick = time(0);
	}

	void ManagerRelationCache::dest(bool save/*=true*/)
	{
		if( save )
		{
			SaveRelationCache();
		}

		ChrRelationShipOpersMap::iterator it = mOperateMap.begin();
		while (it != mOperateMap.end())
		{
			RelationShipOperMap::iterator it2 = it->second.begin();
			while (it2 != it->second.end())
			{
				if (it2->second != NULL)
				{
					delete it2->second;
				}
				it2 = it->second.erase(it2);
			}

			++ it;
		}
		mOperateMap.clear();
	}

	void ManagerRelationCache::PushCache(std::string name, std::string name_oper, int title)
	{
		RelationShipOperMap & oper_map = mOperateMap[name];
		RelationShipOper * oper = oper_map[name_oper];
		if (oper == NULL)
		{
			oper = new RelationShipOper;
			oper_map[name_oper] = oper;
		}
		oper->name = name;
		oper->name_oper = name_oper;
		oper->title = title;
		oper->time = time(0);
	}

	bool ManagerRelationCache::PopCache(std::string name, RelationShipOper & oper)
	{
		bool ret = false;
		ChrRelationShipOpersMap::iterator it = mOperateMap.find(name);
		if (it != mOperateMap.end())
		{
			RelationShipOperMap & oper_map = it->second;
			RelationShipOperMap::iterator it2 = oper_map.begin();

			if (it2 != oper_map.end())
			{
				ret = true;
				oper = *(it2->second);
				delete (it2->second);
				oper_map.erase(it2);
			}

			if (oper_map.size() == 0)
				mOperateMap.erase(it);
		}
		return ret;
	}

	RelationShipOper * ManagerRelationCache::FindOperCache(std::string name, std::string name_oper)
	{
		ChrRelationShipOpersMap::iterator it = mOperateMap.find(name);
		if (it == mOperateMap.end())
		{
			return NULL;
		}
		RelationShipOperMap & oper_map = it->second;
		RelationShipOperMap::iterator it2 = oper_map.find(name_oper);
		if (it2 == oper_map.end())
		{
			return NULL;
		}
		
		return it2->second;
	}

	void ManagerRelationCache::LoadRelationCache()
	{
		mGameServerLib->GetListener()->LoadRelationCache(0);
	}

	void ManagerRelationCache::SaveRelationCache()
	{
		mGameServerLib->GetListener()->SaveRelationCache(*this,0);
	}

	void ManagerRelationCache::OnLoadRelationCache(std::vector<RelationShipOper *> vec)
	{
		time_t now = time(0);
		std::vector<RelationShipOper *>::iterator it = vec.begin();
		while (it != vec.end())
		{
			if (now - (*it)->time > 10*24*60*60)
			{
				delete (*it);
				it = vec.erase(it);
				continue;
			}
			else
			{
				RelationShipOperMap & rel_map = mOperateMap[(*it)->name];
				rel_map[(*it)->name_oper] = (*it);
			}
			++ it;
		}
	}

	void ManagerRelationCache::CopyRelationOperData(std::vector<RelationShipOper *> &vec)
	{
		ChrRelationShipOpersMap::iterator it = mOperateMap.begin();
		while (it != mOperateMap.end())
		{
			RelationShipOperMap & map = it->second;
			RelationShipOperMap::iterator it2 = map.begin();
			while (it2 != map.end())
			{
				RelationShipOper * oper = new RelationShipOper;
				*oper = *(it2->second);
				vec.push_back(oper);
				++ it2;
			}
			++ it;
		}
	}

	RelationShipOper::RelationShipOper()
	{
		title = 0;
		time = 0;
	}
}
LEUD::PoolMemory<OGSLib::RelationShipOper>	LEUD::PoolMemoryBase<OGSLib::RelationShipOper>::memPool("RelationShipOper");