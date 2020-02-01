#pragma once
namespace OGSLib
{
	class GameServerLib;
	class ManagerTask
	{
		friend class GameServerLib;

		GameServerLib* mGameServerLib;

	public:
		ManagerTask(GameServerLib* gslib);
		virtual ~ManagerTask(void);
		struct DailyTask
		{
			int ID;
			std::string TaskName;
			int NeedLevel;
			int Times;
			std::string Detail;
			std::string MapID;
			int MapX;
			int MapY;
			int FlyID;
		};
		DailyTask dailyTask;
		typedef std::vector<DailyTask> DailyTaskVector;
		DailyTaskVector DailyTasks;

		struct InstanceTask
		{
			int ID;
			std::string TaskName;
			int Times;
			int NeedLevel;
			std::string Suggestion;
			std::string MapID;
			int MapX;
			int MapY;
			int FlyID;
		};
		InstanceTask instanceTask;
		typedef std::vector<InstanceTask> InstanceTaskVector;
		InstanceTaskVector InstanceTasks;

		struct EverydayTask
		{
			int ID;
			std::string TaskName;
			int NeedLevel;
			std::string TaskTime;
			std::string Detail;
			std::string MapID;
			int MapX;
			int MapY;
			int FlyID;
		};
		EverydayTask everydayTask;
		typedef std::vector<EverydayTask> EverydayTaskVector;
		EverydayTaskVector EverydayTasks;

		struct BossFresh
		{
			int ID;
			std::string BossName;
			int NeedLevel;
			std::string FreshTime;
			std::string Detail;
			std::string MapName;
			std::string MapID;
			int MapX;
			int MapY;
			int Flag;
			int FlyID;
		};
		BossFresh bossFresh;
		typedef std::vector<BossFresh> BossFreshVector;
		BossFreshVector BossFreshs;
		int init();
		void Dest();
	private:
	};
}