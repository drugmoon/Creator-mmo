#pragma once

namespace OGSLib
{
	struct TrapDef
	{
		// id
		int id;
		// 外观
		int cloth;
		// 触发特效id
		int effect_id;
		// 存在时间s
		int life_time;
		// 触发周期ms
		int damage_gap;
		// 触发伤害值
		int damage;
		// 伤害类型(技能)
		int skill_type;
		// 陷阱召唤者离开删除
		bool caller_leave_remove;

		std::string name;
	};

	class GameServerLib;
	class ManagerTrapDef
	{
		typedef std::map<int,TrapDef> TrapMap;

		GameServerLib* mGameServerLib;
	public:
		ManagerTrapDef(GameServerLib* gslib);
		virtual ~ManagerTrapDef(void);
		
		bool init();
		void dest();

	private:
		TrapMap mTraps;
	};
}