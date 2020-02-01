#pragma once
namespace OGSLib
{
	class StatusInfo
	{
	public:
		int mStatusID;
		int mLv;
		int mIconID;
		std::string mName;

		int mMaxMp;
		int mMaxHp;
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

		int mBaojiProb;
		int mBaijiPres;
		int mNoDef;
		int mFightPoint;
		int mHolyDam;

		StatusInfo::StatusInfo()
		{
			mMaxMp=0;
			mMaxHp=0;
			mAC=0;
			mACMax=0;
			mMAC=0;
			mMACMax=0;
			mDC=0;
			mDCMax=0;
			mMC=0;
			mMCMax=0;
			mSC=0;
			mSCMax=0;
			mNoDef=0;
			mBaojiProb=0;
			mBaijiPres=0;
			mFightPoint=0;
			mHolyDam=0;
		}
	};
	enum
	{
		BUFF_VALID_TYPE_TIME = 1,
		BUFF_VALID_TYPE_HP = 2,
		BUFF_VALID_TYPE_MP = 3,
		BUFF_VALID_TYPE_HPMP = 4,
		BUFF_VALID_TYPE_FOREVER = 5,
	};

	class BuffInfo
	{
	public:
		int mBuffID;
		std::string mName;
		std::string mIcon;
		int mUI;
		int mUISort;
		int mBuffType;
		int mLv;
		// 叠加类型 1
		int mOverlie;
		std::string mDesc;
		int mAtkRemove;
		int mAtkedRemove;
		int mDeadRemove;
		int mReliveRemove;
		int mMoveRemove;
		int mSelfVisible;
		int mOfflineSave;
		// 有效类型
		int mValidType;
		// 有效上限值
		time_t mValidMaxValue;
		int mGap;
		int mEndBuffID;
		int mValidObjType;
		int mRange;
		int mFuncs[3];
		int	mEffectType;
		int mEffectID;
		int mPropGroup;
	public:
		BuffInfo()
		{
			mBuffID = 0;
			mUI = 0;
			mUISort = 0;
			mBuffType = 0;
			mLv = 0;
			mOverlie = 0;
			mAtkRemove = 0;
			mAtkedRemove = 0;
			mDeadRemove = 0;
			mReliveRemove = 0;
			mMoveRemove = 0;
			mSelfVisible = 0;
			mOfflineSave = 0;
			mValidType = 0;
			mValidMaxValue = 0;
			mGap = 0;
			mEndBuffID = 0;
			mValidObjType = 0;
			mRange = 0;
			mFuncs[0] = 0;
			mFuncs[1] = 0;
			mFuncs[2] = 0;
			mEffectType = 0;
			mEffectID = 0;
			mPropGroup = 0;
		}
	};

	struct ObjBuff
	{
		BuffInfo buff_info;
		// buff效果触发时间点
		double buff_time_tick;
		// 到期值,可表示时间(值)
		double buff_end_value;
		
		bool enable;
	};

	typedef std::vector<ObjBuff> BuffList;

	class GameServerLib;
	class ManagerStatus
	{
	public:
		typedef std::map<int,StatusInfo> StatusInfoMap;
		typedef std::map<int,BuffInfo> BuffInfoMap;
	private:
		StatusInfoMap mStatusInfos;
		BuffInfoMap mBuffInfos;

		GameServerLib* mGameServerLib;
	public:
		ManagerStatus(GameServerLib* gslib);
		virtual ~ManagerStatus(void);
	public:
		bool Init();
		void Dest();
	public:
		bool LoadBuffDef();
		bool LoadStatusInfo();
		bool ReloadStatusDef(GameServerLib* lib);
		StatusInfo* GetStatusInfo(int status_id,int lv);
		StatusInfo* GetBaseStatusInfo(int status_id);
		StatusInfoMap& GetStatusMap() {return mStatusInfos;}

		BuffInfoMap & GetBuffMap(){return mBuffInfos;}

		BuffInfo * GetBuffInfo(int id);
	};
}