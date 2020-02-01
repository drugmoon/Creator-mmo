#pragma once

namespace OGSLib
{
	class TotalAttrInfo
	{
	public:
		int mLevel;
		int mJob;
		int mID;
		int mDC;
		int mDCMax;
		int mMC;
		int mMCMax;
		int mSC;
		int mSCMax;
		int mAC;
		int mACMax;
		int mMAC;
		int mMACMax;
		int mHP;
		int mMP;

		int mAccuracy;
		int mDodge;
		int mLuck;
		int mDropProb;
		int mDoubleAttProb;
	};
	class GameServerLib;
	class ManagerTotalAttr
	{
	public:
		typedef std::map<int,TotalAttrInfo> TotalAttrInfoMap;
		typedef std::map<int,int> StatusFightMap;
	private:
		TotalAttrInfoMap mTotalAttrInfos;
		StatusFightMap mStatusFights;
		GameServerLib* mGameServerLib;
	public:
		ManagerTotalAttr(GameServerLib* gslib);
		virtual ~ManagerTotalAttr(void);
	public:
		bool Init();
		void Dest();

		TotalAttrInfoMap& GetTotalAttrMap(){return mTotalAttrInfos;}
		TotalAttrInfo* GetTotalAttrInfo(int level);
		int GetStatusFight(int id);
		bool ReloadStatusFight();
	private:
		bool LoadTotalAttrInfo();
		bool LoadStatusFightInfo();
	};
}
