#include "stdafx.h"
#include "ManagerSwordDef.h"
#include "GameServerlib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"

namespace OGSLib
{
	bool ManagerSwordDef::ReLoadSwordDefine(GameServerLib* lib)
	{
		SwordDefineMap i = mSwordDefineMap;
		if( ! LoadSwordDefine(lib) )
		{
			mSwordDefineMap = i;
			return false;
		}
		return true;
	}
	
	bool ManagerSwordDef::LoadSwordDefine(GameServerLib* lib)
	{
		mSwordDefineMap.clear();
		char temp[256];
		char name[256];
		char desp[256];
		int sid;
		int skilltype;
		int type;
		int level;
		int param1;
		int param2;
		int param3;
		int baojipres;
		int subBaoji;
		int baojiprob;
		int hp;
		int mp;
		int rex;
		int fight;
		int nextneed;

		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("magicdef2.csv",size);
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
						line[5]=0;
						//if( 0 == strcmp(line,"#v101") )
						//{
						//	version = 101;
						//}
						continue;
					}
					memset(name,0,256);
					int ret = 0;
					int parse_ok = 0;
					if(version == 100)
					{
						ret = sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%[^,]",
							&sid,&skilltype,&type,&level,&param1,&param2,&param3,&baojipres,
							&subBaoji,&baojiprob,&hp,&mp,&rex,&fight,&nextneed,desp);
						if( 15 == ret || 16 == ret )
						{
							if( mSwordDefineMap.find(sid) != mSwordDefineMap.end() )
							{
								continue;
							}
							parse_ok = 1;
						}
					}
					if( parse_ok = 1 )
					{
						SwordDef sd;
						sd.Sid = sid;
						sd.SkillType = skilltype;
						sd.Type = type;
						sd.Level = level;
						sd.Param1 = param1;
						sd.Param2 = param2;
						sd.Param3 = param3;
						sd.Baojipres = baojipres;
						sd.SubBaoji = subBaoji;
						sd.Baojiprob = baojiprob;
						sd.Hp = hp;
						sd.Mp = mp;
						sd.Rex = rex;
						sd.Fight = fight;
						sd.NextNeed = nextneed;
						UtilString::act_2_utf8(desp,strlen(desp),temp,256);strncpy_s(desp,_countof(desp),temp,255);desp[255]=0;sd.mDesp = desp;
						mSwordDefineMap[sd.Sid] = sd;
					}
				}
			}
		}
		if( mSwordDefineMap.empty() )
		{
			return false;
		}
		return true;
	}
	void ManagerSwordDef::Dest()
	{
		mSwordDefineMap.clear();
	}
	SwordDef* ManagerSwordDef::GetSwordDefine(int skill_type)
	{
		SwordDefineMap::iterator pos = mSwordDefineMap.find(skill_type);
		if( pos != mSwordDefineMap.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
	bool ManagerMonPointDef::ReLoadMonPointDefine(GameServerLib* lib)
	{
		MonPointDefineMap i = mMonPointDefineMap;
		if( ! LoadMonPointDefine(lib) )
		{
			mMonPointDefineMap = i;
			return false;
		}
		return true;
	}
	bool ManagerMonPointDef::LoadMonPointDefine(GameServerLib* lib)
	{
		mMonPointDefineMap.clear();

		int TypeId;//用于区分不同功能
		int Level;
		int ac_min;//物防
		int ac_max;
		int mac_min;//魔防
		int mac_max;
		int dc_min;//物理攻击
		int dc_max;
		int mc_min;//魔法攻击
		int mc_max;
		int sc_min;//道术攻击
		int sc_max;
		int max_hp;
		int max_mp;
		int jifen;

		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("monpointaddatt.csv",size);
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
						line[5]=0;
						continue;
					}
					int ret = 0;
					int parse_ok = 0;
					if(version == 100)
					{
						ret = sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,\
										  %d,%d,%d,%d,%d,%d",
										  &TypeId,&Level, &dc_min, &dc_max, &mc_min, &mc_max, &sc_min, &sc_max, &ac_min,
										  &ac_max,&mac_min,&mac_max,&max_hp,&max_mp,&jifen);
						if( 15 == ret )
						{
							if (mMonPointDefineMap.find(TypeId*1000+Level) != mMonPointDefineMap.end())
							{
								continue;
							}
							parse_ok = 1;
						}
					}
					if( parse_ok = 1 )
					{
						MonPointDef sd;
						sd.TypeId = TypeId;
						sd.Level = Level;
						sd.dc_min = dc_min;
						sd.dc_max = dc_max;
						sd.mc_min = mc_min;
						sd.mc_max = mc_max;
						sd.sc_min = sc_min;
						sd.sc_max = sc_max;
						sd.ac_min = ac_min;
						sd.ac_max = ac_max;
						sd.mac_min = mac_min;
						sd.mac_max = mac_max;
						sd.max_hp = max_hp;
						sd.max_mp = max_mp;
						sd.jifen = jifen;
						mMonPointDefineMap[sd.TypeId*1000+sd.Level] = sd;
					}
				}
			}
		}
		if( mMonPointDefineMap.empty() )
		{
			return false;
		}
		return true;
	}
	void ManagerMonPointDef::Dest()
	{
		mMonPointDefineMap.clear();
	}
	MonPointDef* ManagerMonPointDef::GetMonPointDefine(int skill_type)
	{
		MonPointDefineMap::iterator pos = mMonPointDefineMap.find(skill_type);
		if( pos != mMonPointDefineMap.end() )
		{
			return &(pos->second);
		}
		return 0;
	}

	bool ManagerEquipUpgradeDef::ReLoadEquipUpgradeDefine(GameServerLib* lib)
	{
		EquipUpgradeDefMap i = mEquipUpgradeDefMap;
		if( ! LoadEquipUpgradeDefine(lib) )
		{
			mEquipUpgradeDefMap = i;
			return false;
		}
		return true;
	}
	bool ManagerEquipUpgradeDef::LoadEquipUpgradeDefine(GameServerLib* lib)
	{
		mEquipUpgradeDefMap.clear();

		int id;
		int job;
		int equip_type;
		int level;

		//脚本使用数据
		int success_rate;
		int cost_coin;
		int cost_item;
		int cost_num;
		int upgrade_cost_num;
		int buy_cost_type;
		int buy_cost_num;
		int shift_cost_type;
		int shift_cost_num;
		int store_item;
		int success_cost_vcoin;

		int dc;
		int dc_max;
		int mc;
		int mc_max;
		int sc;
		int sc_max;
		int ac;
		int ac_max;
		int mac;
		int mac_max;

		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("equipUpgrade.csv",size);
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
						line[5]=0;
						continue;
					}
					int ret = 0;
					int parse_ok = 0;
					if(version == 100)
					{
						ret = sscanf(line,"%d,%d,%d,%d,\
										  %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
										  %d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
										  &id,&job,&equip_type,&level,
										  &success_rate,&cost_coin,&cost_item,&cost_num,&upgrade_cost_num,&buy_cost_type,&buy_cost_num,&shift_cost_type,&shift_cost_num,&store_item,&success_cost_vcoin,
										  &dc,&dc_max,&mc,&mc_max,&sc,&sc_max,&ac,&ac_max,&mac,&mac_max);
						if( 25 == ret )
						{
							if (mEquipUpgradeDefMap.find(job*10000+equip_type*100+level) != mEquipUpgradeDefMap.end())
							{
								continue;
							}
							parse_ok = 1;
						}
					}
					if( parse_ok = 1 )
					{
						EquipUpgradeDef ed;
						if (job > 0)
						{
							job = job + 99;
						}
						ed.mJob = job;
						ed.mEquipType = equip_type;
						ed.mLevel = level;
						ed.mDC = dc;
						ed.mDCMax = dc_max;
						ed.mMC = mc;
						ed.mMCMax = mc_max;
						ed.mSC = sc;
						ed.mSCMax = sc_max;
						ed.mAC = ac;
						ed.mACMax = ac_max;
						ed.mMAC = mac_max;
						ed.mMACMax = mac_max;

						mEquipUpgradeDefMap[job*10000+equip_type*100+level] = ed;
					}
				}
			}
		}
		if( mEquipUpgradeDefMap.empty() )
		{
			return false;
		}
		return true;
	}
	void ManagerEquipUpgradeDef::Dest()
	{
		mEquipUpgradeDefMap.clear();
	}
	EquipUpgradeDef* ManagerEquipUpgradeDef::GetEquipUpgradeDefine(int id)
	{
		EquipUpgradeDefMap::iterator pos = mEquipUpgradeDefMap.find(id);
		if( pos != mEquipUpgradeDefMap.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
}