#include "StdAfx.h"
#include "ManagerTotalAttr.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
namespace OGSLib
{
	ManagerTotalAttr::ManagerTotalAttr(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerTotalAttr::~ManagerTotalAttr(void)
	{
	}
	bool ManagerTotalAttr::Init()
	{
		return LoadTotalAttrInfo();
	}
	void ManagerTotalAttr::Dest()
	{
	}
	bool ManagerTotalAttr::LoadTotalAttrInfo()
	{
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("totalattr.csv",size);
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
						continue;
					}
					int ret = 0;
					int job = 0;
					int level = 0;
					int ac = 0;
					int acmax = 0;
					int mac = 0;
					int macmax = 0;
					int dc = 0;
					int dcmax = 0;
					int mc = 0;
					int mcmax = 0;
					int sc = 0;
					int scmax = 0;
					int hp = 0;
					int mp = 0;
					int luck = 0;
					int accuracy = 0;
					int dodge = 0;
					int shouhu = 0;
					int baoji = 0;
					{
						ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&job,&level,&ac,&acmax,&mac,&macmax,&dc,&dcmax,&mc,&mcmax,&sc,&scmax,&hp,&mp,&luck,&accuracy,&dodge,&shouhu,&baoji);
						if( ret == 19 )
						{
							TotalAttrInfo tai;
							tai.mLevel = level;tai.mJob = job; tai.mID = job*100+level;
							tai.mAC = ac;tai.mACMax = acmax;tai.mMAC = mac;tai.mMACMax = macmax;tai.mDC = dc;tai.mDCMax = dcmax;
							tai.mMC = mc;tai.mMCMax = mcmax;tai.mSC = sc;tai.mSCMax = scmax;tai.mHP = hp;tai.mMP = mp;
							tai.mAccuracy = accuracy;tai.mDodge = dodge;tai.mLuck = luck;tai.mDropProb = shouhu;tai.mDoubleAttProb = baoji;
							mTotalAttrInfos[tai.mID] = tai;
						}
					}
				}
			}
		}
		return true;
	}
	TotalAttrInfo* ManagerTotalAttr::GetTotalAttrInfo(int id)
	{
		TotalAttrInfoMap::iterator pos = mTotalAttrInfos.find(id);
		if( pos != mTotalAttrInfos.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
	bool ManagerTotalAttr::LoadStatusFightInfo()
	{
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("statusfight.txt",size);
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
						continue;
					}
					int statusid;
					int fightp;
					int ret = sscanf_s(line,"%d%d",&statusid,&fightp);
					if( 2 == ret && statusid > 0 && statusid <= 50 )
					{
						mStatusFights[statusid] = fightp;
					}
				}
			}
		}
		return true;
	}
	bool ManagerTotalAttr::ReloadStatusFight()
	{
		return true;
	}
	int ManagerTotalAttr::GetStatusFight(int id)
	{
		StatusFightMap::iterator pos = this->mStatusFights.find(id);
		if( pos != this->mStatusFights.end() )
		{
			return pos->second;
		}
		return 0;
	}
}