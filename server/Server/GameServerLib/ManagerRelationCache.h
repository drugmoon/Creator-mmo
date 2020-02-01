#pragma once
#include "leudgrid/MemoryPool.h"
#include <string>
#include <map>

namespace OGSLib
{
	class GameServerLib;
	class DomainPlay;
	// 寄售物品信息
	class RelationShipOper : public LEUD::PoolMemoryBase< RelationShipOper >
	{
	public:
		// 操作者名字
		std::string name_oper;
		// 离线玩家seed_name
		std::string name;
		// 操作类型
		int title;
		// 时间
		time_t time;
	public:
		RelationShipOper();
	};

	class ManagerRelationCache
	{
	public:
		// first 操作者
		typedef std::map<std::string, RelationShipOper *> RelationShipOperMap;
		// first 被操作者
		typedef std::map<std::string, RelationShipOperMap> ChrRelationShipOpersMap;
	public:
		ManagerRelationCache(GameServerLib* gslib);
		virtual ~ManagerRelationCache(void);

		void PushCache(std::string name, std::string name_oper, int title);
		bool PopCache(std::string ame, RelationShipOper & oper);
		RelationShipOper * FindOperCache(std::string name, std::string name_oper);
		void init();
		void update();
		void dest(bool save=true);
		void LoadRelationCache();
		void SaveRelationCache();
		void OnLoadRelationCache(std::vector<RelationShipOper *> vec);
		void CopyRelationOperData(std::vector<RelationShipOper *> &vec);
	private:
		GameServerLib* mGameServerLib;
		ChrRelationShipOpersMap mOperateMap;
		time_t mSaveTick;
	};
}