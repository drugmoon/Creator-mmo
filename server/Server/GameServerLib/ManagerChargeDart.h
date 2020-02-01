#pragma once
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{

	class GameServerLib;
	class DomainPlay;

	class ChargeDart : public LEUD::PoolMemoryBase< ChargeDart >
	{
	public:
		std::string mCharName;
		int mDartCar;
		int mIcon;
		double mStartTime;
		int mDuration;
		int mFightingForce;
		int	mStolenTimes;
		int mTotalAwards;
		int mSurplusAwards;
		int mState;
		std::string mRobDartCharNames;
		int mDartLevel;
	public:
		ChargeDart();
	};

	class ManagerChargeDart
	{
	public:
		typedef std::map<std::string,ChargeDart*> ChargeDartMap;
	private:
		ChargeDartMap mChargeDarts;
		GameServerLib* mGameServerLib;
	public:
		ChargeDartMap& GetChargeDarts(){return mChargeDarts;}
	public:
		ManagerChargeDart(GameServerLib* gslib);
		virtual ~ManagerChargeDart(void);
	
		int init();
		void dest(bool save=true);
		void Update(double time);

		void StartChargeDart(std::string charName, int dartCar ,int icon,int duration,int fightingForce,int totalAwards,int dartLevel);
		ChargeDart* GetChargeDart(std::string charName);
		void SetChargeDart(std::string charName, int mDartCar,int mIcon,double mStartTime,int mDuration,int mFightingForce,int mStolenTimes,int mTotalAwards,int mSurplusAwards,int mState,std::string robDartCharNames);
	};
}
