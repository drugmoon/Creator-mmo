#pragma once

namespace OGSLib
{
	class LevelInfo
	{
	public:
		int mLevel;
		LEUD_INT64 mNextExp;
	};
	class lv_attr
	{
	public:
		int mID;
		int mMaxMp;
		int mMaxHp;		

		int mMaxBurden;
		int mMaxLoad;
		int mMaxBrawn;

		int mAC;
		int mACMax;
		int mMAC;
		int mMACMax;
		int mDC;
		int mDCMax;
		int mMC;
		int mMCMax;
		int mSC;
		int mSCMax;
 		int mAccuracy;
 		int mDodge;
	};
	class GameServerLib;
	class ManagerLevelInfo
	{
	public:
		typedef std::map<int,LevelInfo> LevelInfoMap;
		typedef std::map<int,lv_attr> LevelAttrMap;
	private:
		LevelInfoMap mLevelInfos;
		LevelAttrMap mLvAttrs;

		/*LevelAttrMap mWarriorLvAttrs;
		LevelAttrMap mWizardLvAttrs;
		LevelAttrMap mTaoistLvAttrs;*/

		GameServerLib* mGameServerLib;
	public:
		ManagerLevelInfo(GameServerLib* gslib);
		virtual ~ManagerLevelInfo(void);
	public:
		bool Init();
		void Dest();
		bool ReloadLevelInfo();

		int GetMaxLevel();

		LEUD_INT64 GetNextExp(int level);
		lv_attr* GetLevelAttr(int job);
		lv_attr* GetLevelAttrByJob(int level, int job);
	private:
		bool LoadLevelInfo();
		bool LoadLevelAttr();

		bool LoadLevelInfoAndAttr();
		int m_nMaxLv;
	};
}
