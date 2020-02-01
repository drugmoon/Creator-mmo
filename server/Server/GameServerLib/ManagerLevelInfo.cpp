#include "StdAfx.h"
#include "ManagerLevelInfo.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include "DomainPlay.h"
namespace OGSLib
{
	ManagerLevelInfo::ManagerLevelInfo(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
		m_nMaxLv = 0;
	}
	ManagerLevelInfo::~ManagerLevelInfo(void)
	{
	}
	bool ManagerLevelInfo::Init()
	{
		//return LoadLevelInfo();
		return LoadLevelInfoAndAttr();
	}
	void ManagerLevelInfo::Dest()
	{
	}
	bool ManagerLevelInfo::LoadLevelInfo()
	{
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("levelinfo.txt",size);
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
					int level;
					LEUD_INT64 next_exp;
					int ret = sscanf_s(line,"%d%I64d",&level,&next_exp);
					if( 2 == ret )
					{
						LevelInfo li;
						li.mLevel = level;
						li.mNextExp = next_exp;
						mLevelInfos[li.mLevel] = li;
					}
				}
			}
		}
		return true;
	}
	LEUD_INT64 ManagerLevelInfo::GetNextExp(int level)
	{
		LevelInfoMap::iterator pos = this->mLevelInfos.find(level);
		if( pos != this->mLevelInfos.end() )
		{
			return pos->second.mNextExp;
		}
		return 10000000;
	}
	bool ManagerLevelInfo::LoadLevelAttr()
	{
		mLvAttrs.clear();

		int version = 0;
		size_t size= 0;

		int id;
		int maxhp;
		int maxmp;

		int maxBurden;
		int maxLoad;
		int maxBrawn;

		int ac;
		int acmax;
		int mac;
		int macmax;
		int dc;
		int dcmax;
		int mc;
		int mcmax;
		int sc;
		int scmax;

		const char* data = mGameServerLib->GetListener()->LoadFile("lv_attr.csv",size);
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
						if( 0 == strcmp(line,"#v100") )
						{
							version = 100;
						}
						continue;
					}
					if(version==0)
					{
						printf("\n    error:your lv_attr table has no defined version    \n");
						return false;
					}
					int ret=0;
					int parse_ok = 0;
					if( version == 100 )
					{
						ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&id,&maxhp,&maxmp,&ac,&acmax,&mac,&macmax,&dc,&dcmax,&mc,&mcmax,&sc,&scmax,&maxBurden,&maxLoad,&maxBrawn);
						if( ret == 16 )
						{
							parse_ok = 1;
						}
					}
					if( parse_ok > 0 )
					{
						lv_attr la;
						la.mID = id;
						la.mMaxHp = maxhp;
						la.mMaxMp = maxmp;
						la.mMaxBrawn = maxBrawn;
						la.mMaxBurden = maxBurden;
						la.mMaxLoad = maxLoad;
						la.mAC = ac;
						la.mACMax = acmax;
						la.mMAC = mac;
						la.mMACMax = macmax;
						la.mDC = dc;
						la.mDCMax = dcmax;
						la.mMC = mc;
						la.mMCMax = mcmax;
						la.mSC = sc;
						la.mSCMax = scmax;
						mLvAttrs[la.mID] = la;
					}
				}
			}
		}
		return true;
	}
	bool ManagerLevelInfo::LoadLevelInfoAndAttr()
	{
		/*mWarriorLvAttrs.clear();
		mWizardLvAttrs.clear();
		mTaoistLvAttrs.clear();*/
		m_nMaxLv = 0;
		mLvAttrs.clear();

		int version = 100;
		size_t size= 0;
		int mId = 0;

		int id;
		int level;
		LEUD_INT64 next_exp;
		int zs;
		int sw;

		int warrior_maxhp;
		int warrior_maxmp;
		int warrior_dc;
		int warrior_dcmax;
		int warrior_mc;
		int warrior_mcmax;
		int warrior_sc;
		int warrior_scmax;
		int warrior_ac;
		int warrior_acmax;
		int warrior_mac;
		int warrior_macmax;
		int warrior_accuracy;
		int warrior_dodge;

		int wizard_maxhp;
		int wizard_maxmp;
		int wizard_dc;
		int wizard_dcmax;
		int wizard_mc;
		int wizard_mcmax;
		int wizard_sc;
		int wizard_scmax;
		int wizard_ac;
		int wizard_acmax;
		int wizard_mac;
		int wizard_macmax;
		int wizard_accuracy;
		int wizard_dodge;

		int taoist_maxhp;
		int taoist_maxmp;
		int taoist_dc;
		int taoist_dcmax;
		int taoist_mc;
		int taoist_mcmax;
		int taoist_sc;
		int taoist_scmax;
		int taoist_ac;
		int taoist_acmax;
		int taoist_mac;
		int taoist_macmax;
		int taoist_accuracy;
		int taoist_dodge;

		const char* data = mGameServerLib->GetListener()->LoadFile("levelinfo.csv",size);
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
						if( 0 == strcmp(line,"#v100") )
						{
							version = 100;
						}
						continue;
					}
					if(version==0)
					{
						printf("\n    error:your levelinfo table has no defined version    \n");
						return false;
					}
					int ret=0;
					int parse_ok = 0;
					if( version == 100 )
					{
						ret = sscanf_s(line,"%d,%d,%I64d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
											&id,&level,&next_exp,&zs,&sw,
											&warrior_maxhp,&warrior_maxmp,&warrior_dc,&warrior_dcmax,&warrior_mc,&warrior_mcmax,&warrior_sc,&warrior_scmax,&warrior_ac,&warrior_acmax,&warrior_mac,&warrior_macmax,&warrior_accuracy,&warrior_dodge,
											&wizard_maxhp,&wizard_maxmp,&wizard_dc,&wizard_dcmax,&wizard_mc,&wizard_mcmax,&wizard_sc,&wizard_scmax,&wizard_ac,&wizard_acmax,&wizard_mac,&wizard_macmax,&wizard_accuracy,&wizard_dodge,
											&taoist_maxhp,&taoist_maxmp,&taoist_dc,&taoist_dcmax,&taoist_mc,&taoist_mcmax,&taoist_sc,&taoist_scmax,&taoist_ac,&taoist_acmax,&taoist_mac,&taoist_macmax,&taoist_accuracy,&taoist_dodge);
						if( ret == 47 )
						{
							parse_ok = 1;
						}
					}
					if( parse_ok > 0 )
					{
						//等级和经验

						LevelInfo li;
						li.mLevel = level;
						li.mNextExp = next_exp;
						mLevelInfos[li.mLevel] = li;

						if (level > m_nMaxLv)
							m_nMaxLv = level;
						//int id = job()*1000 + level();

						//各职业属性
						lv_attr la;
						la.mID = level;
						la.mMaxHp = warrior_maxhp;
						la.mMaxMp = warrior_maxmp;
						la.mDC = warrior_dc;
						la.mDCMax = warrior_dcmax;
						la.mMC = warrior_mc;
						la.mMCMax = warrior_mcmax;
						la.mSC = warrior_sc;
						la.mSCMax = warrior_scmax;
						la.mAC = warrior_ac;
						la.mACMax = warrior_acmax;
						la.mMAC = warrior_mac;
						la.mMACMax = warrior_macmax;
						la.mMaxBrawn = 0;
						la.mMaxBurden = 0;
						la.mMaxLoad = 0;
						la.mAccuracy = warrior_accuracy;
						la.mDodge = warrior_dodge;

						//mWarriorLvAttrs[la.mID] = la;
						
						mId = JOB_WARRIOR * 1000 + level;
						mLvAttrs[mId] = la;

						lv_attr la2;
						la2.mID = level;
						la2.mMaxHp = wizard_maxhp;
						la2.mMaxMp = wizard_maxmp;
						la2.mDC = wizard_dc;
						la2.mDCMax = wizard_dcmax;
						la2.mMC = wizard_mc;
						la2.mMCMax = wizard_mcmax;
						la2.mSC = wizard_sc;
						la2.mSCMax = wizard_scmax;
						la2.mAC = wizard_ac;
						la2.mACMax = wizard_acmax;
						la2.mMAC = wizard_mac;
						la2.mMACMax = wizard_macmax;
						la2.mMaxBrawn = 0;
						la2.mMaxBurden = 0;
						la2.mMaxLoad = 0;
						la2.mAccuracy = wizard_accuracy;
						la2.mDodge = wizard_dodge;

						//mWizardLvAttrs[la2.mID] = la2;

						mId = JOB_WIZARD * 1000 + level;
						mLvAttrs[mId] = la2;

						lv_attr la3;
						la3.mID = level;
						la3.mMaxHp = taoist_maxhp;
						la3.mMaxMp = taoist_maxmp;
						la3.mDC = taoist_dc;
						la3.mDCMax = taoist_dcmax;
						la3.mMC = taoist_mc;
						la3.mMCMax = taoist_mcmax;
						la3.mSC = taoist_sc;
						la3.mSCMax = taoist_scmax;
						la3.mAC = taoist_ac;
						la3.mACMax = taoist_acmax;
						la3.mMAC = taoist_mac;
						la3.mMACMax = taoist_macmax;
						la3.mMaxBrawn = 0;
						la3.mMaxBurden = 0;
						la3.mMaxLoad = 0;
						la3.mAccuracy = wizard_accuracy;
						la3.mDodge = wizard_dodge;
						//mTaoistLvAttrs[la3.mID] = la3;

						mId = JOB_TAOIST * 1000 + level;
						mLvAttrs[mId] = la3;
					}
				}
			}
		}
		return true;
	}
	lv_attr* ManagerLevelInfo::GetLevelAttr(int id)
	{
		LevelAttrMap::iterator pos = mLvAttrs.find(id);
		if( pos != this->mLvAttrs.end() )
		{
			return &pos->second;
		}
		return 0;
	}

	bool ManagerLevelInfo::ReloadLevelInfo()
	{
		LevelAttrMap LvAttrs;
		if (!LoadLevelInfoAndAttr())
		{
			mLvAttrs = LvAttrs;
			return false;
		}
		return true;
	}

	int ManagerLevelInfo::GetMaxLevel()
	{
		return m_nMaxLv;
	}

}