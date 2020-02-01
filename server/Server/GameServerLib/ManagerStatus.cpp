#include "StdAfx.h"
#include "ManagerStatus.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
namespace OGSLib
{
	ManagerStatus::ManagerStatus(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerStatus::~ManagerStatus(void)
	{
	}
	bool ManagerStatus::Init()
	{
		//LoadStatusInfo();
		return LoadBuffDef();
	}
	void ManagerStatus::Dest()
	{
	}
	bool ManagerStatus::LoadStatusInfo()
	{
		mStatusInfos.clear();
		char temp[256];
		char name[256];
		size_t size= 0;
		int version = 100;
		const char* data = mGameServerLib->GetListener()->LoadFile("statusdef.csv",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						line[5] = 0;
						if( 0 == strcmp(line,"#v101") )
						{
							version = 101;
						}
						continue;
					}
					int ret = 0;
					if( version == 100 )
					{
						StatusInfo si;
						memset(name,0,256);
						int ret = sscanf_s(line,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
							&si.mStatusID,name,_countof(name),
							&si.mLv,&si.mIconID,&si.mMaxHp,&si.mMaxMp,&si.mAC,&si.mACMax,&si.mMAC,&si.mMACMax,
							&si.mDC,&si.mDCMax,&si.mMC,&si.mMCMax,&si.mSC,&si.mSCMax,&si.mNoDef,&si.mFightPoint);
						if( 4 == ret || 18 == ret )
						{
							if( mStatusInfos.find(si.mStatusID*1000+si.mLv) != mStatusInfos.end() )
							{
								continue;
							}
							UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;si.mName = name;
							mStatusInfos[si.mStatusID * 1000 + si.mLv] = si;
						}
					}
					if( version == 101 )
					{
						StatusInfo si;
						memset(name,0,256);
						ret = sscanf(line,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
							&si.mStatusID,name,
							&si.mLv,&si.mIconID,&si.mMaxHp,&si.mMaxMp,&si.mAC,&si.mACMax,&si.mMAC,&si.mMACMax,
							&si.mDC,&si.mDCMax,&si.mMC,&si.mMCMax,&si.mSC,&si.mSCMax,&si.mNoDef,&si.mBaojiProb,&si.mBaijiPres,&si.mFightPoint,&si.mHolyDam);
						if( 4 == ret || 20 == ret )
						{
							if (mStatusInfos.find(si.mStatusID * 1000 + si.mLv) != mStatusInfos.end())
							{
								continue;
							}
							UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;si.mName = name;
							mStatusInfos[si.mStatusID * 1000 + si.mLv] = si;
						}
					}
				}
			}
		}
		if( mStatusInfos.empty() )
		{
			return false;
		}
		return true;
	}

	bool ManagerStatus::ReloadStatusDef(GameServerLib* lib)
	{
// 		StatusInfoMap i = mStatusInfos;
// 		if( ! LoadStatusInfo() )
// 		{
// 			mStatusInfos = i;
// 			return false;
// 		}
		BuffInfoMap b = mBuffInfos;
		if (!LoadBuffDef())
		{
			mBuffInfos = b;
			return false;
		}
		return true;
	}

	StatusInfo* ManagerStatus::GetStatusInfo(int status_id,int lv)
	{
		int id = status_id*1000+lv;
		StatusInfoMap::iterator pos = mStatusInfos.find(id);
		if( pos != mStatusInfos.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
	StatusInfo* ManagerStatus::GetBaseStatusInfo(int status_id)
	{
		int id = status_id*1000+1;
		StatusInfoMap::iterator pos = mStatusInfos.find(id);
		if( pos != mStatusInfos.end() )
		{
			return &(pos->second);
		}
		return 0;
	}

	bool ManagerStatus::LoadBuffDef()
	{
		mBuffInfos.clear();
		char temp[256] = {0};
		char icon[256] = {0};
		char name[256] = {0};
		char desc[256] = {0};

		size_t size= 0;
		int version = 100;
		const char* data = mGameServerLib->GetListener()->LoadFile("buffdef.csv",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						line[5] = 0;
						if( 0 == strcmp(line,"#v101") )
						{
							version = 101;
						}
						continue;
					}
					int ret = 0;
					if( version == 100 )
					{
						BuffInfo bi;
						memset(name,0,256);
						memset(desc,0,256);
						memset(icon,0,256);
						int ret = sscanf_s(line,"%d,%[^,],%[^,],%d,%d,%d,%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
							&bi.mBuffID,
							name,_countof(name),
							icon,_countof(icon),
							&bi.mUI,
							&bi.mUISort,
							&bi.mBuffType,
							&bi.mLv,
							&bi.mOverlie,
							desc,_countof(desc),
							&bi.mAtkRemove,
							&bi.mAtkedRemove,
							&bi.mDeadRemove,
							&bi.mReliveRemove,
							&bi.mMoveRemove,
							&bi.mSelfVisible,
							&bi.mOfflineSave,
							&bi.mValidType,
							&bi.mValidMaxValue,
							&bi.mGap,
							&bi.mEndBuffID,
							&bi.mValidObjType,
							&bi.mRange,
							&bi.mFuncs[0],
							&bi.mFuncs[1],
							&bi.mFuncs[2],
							&bi.mEffectType,
							&bi.mEffectID,
							&bi.mPropGroup);
						if( 28 == ret )
						{
							if( mBuffInfos.find(bi.mBuffID) != mBuffInfos.end() )
							{
								continue;
							}
							UtilString::act_2_utf8(name,strlen(name),temp,256);
							strncpy_s(name,_countof(name),temp,255);
							name[255]=0;
							bi.mName = name;

							UtilString::act_2_utf8(icon,strlen(icon),temp,256);
							strncpy_s(icon,_countof(icon),temp,255);
							icon[255]=0;
							bi.mIcon = icon;

							UtilString::act_2_utf8(desc,strlen(desc),temp,256);
							strncpy_s(desc,_countof(desc),temp,255);
							desc[255]=0;
							bi.mDesc = desc;

							mBuffInfos[bi.mBuffID] = bi;
						}
					}
				}
			}
		}
		if( mBuffInfos.empty() )
		{
			return false;
		}
		return true;
	}

	BuffInfo * ManagerStatus::GetBuffInfo(int id)
	{
		BuffInfoMap::iterator it = mBuffInfos.find(id);
		if (it != mBuffInfos.end())
		{
			return &(it->second);
		}
		return 0;
	}

}