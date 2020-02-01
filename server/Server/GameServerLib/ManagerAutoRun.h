#pragma once
#include "TimerFix.h"
namespace OGSLib
{
	struct AutoRunNode
	{
		int mType;
		time_t mTypeParam;
		time_t mTick;
		std::string mScript;
		enum AUTORUN_TYPE
		{
			AUTORUN_TYPE_REPET,
			AUTORUN_TYPE_DAY,
			AUTORUN_TYPE_FIX,
		};
		AutoRunNode()
		{
			mType=0;
			mTypeParam=0;
			mTick=0;
		}
	};
	struct AutoRunDB
	{
		time_t mBeginTime;
		std::string mBeginScript;
		time_t mEndTime;
		std::string mEndScript;
	};
	class GameServerLib;
	class ManagerAutoRun
	{
	public:
		typedef std::vector<AutoRunNode> AutoRunNodeVector;
	private:
		GameServerLib* mGameServerLib;
		AutoRunNodeVector mAutoRuns;
		time_t mUpdateTick;
		time_t mUpdateLoadAutoTick;
		time_t mUpdateDBTick;
		typedef std::map<int, double> TimerMap;
		TimerMap mTimer;
	public:
		std::vector<OGSLib::AutoRunDB> mAutoRunDB;
		ManagerAutoRun(GameServerLib* gslib);
		virtual ~ManagerAutoRun(void);
	public:
		bool Init();
		void Dest();
		void Update(double time);

		bool LoadAutoRun();
		int SetMapTimer(int i, int esp);
	};
}
