#include "StdAfx.h"
#include "ManagerMonDef.h"
#include "ManagerItemDef.h"
#include "GameServerLib.h"
#include "SubItem.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include <sstream>

namespace OGSLib
{
	ManagerMonDef::ManagerMonDef(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerMonDef::~ManagerMonDef(void)
	{
	}
	bool ManagerMonDef::init()
	{
		LoadMonAICsv();
		LoadDropPlusCsv(); 
		LoadMonDropCsv();
		//if ()
		{
			char temp[256];
			//char notemp[64];
			char ai[1024];
			char editdir[1024];
			//char bosshead[1024];
			
			int digital;
			int type;
			int tel_id;
			//int group_id;
			int param_id;
			int param;
			char default_talk[1024];
			int istalk;
			char random_talk[1024];
			int easy_drop;

			size_t size= 0;
			const char* data = mGameServerLib->GetListener()->LoadFile("mondef.csv",size);
			if( data && size > 0 )
			{
				LEUD::ToolsMemFile f(data,size);
				char line[1024];
				while( f.read_line(line,1024) )
				{
					if( line[0]!=0 )
					{
						if( line[0] == '#' || line[0] == ';' )
						{
							continue;
						}
						MonDef md;
						//std::string word;
						int ret = sscanf_s(line,"%d,%d,%[^,],%d,%[^,],%d,%d,%d,%d,%d, \
												 %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,		  \
												 %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,		  \
												 %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,		  \
												 %[^,],%d,%d,%d,%[^,],%d,%d,%d,%d,%d, \
												 %d,%d,%d,%d,%d,%d,%d,%d,%d,%[^,],    \
												 %d,%[^,],%d,%d,%d,%d,%d,%d,%d",
												 &md.id,&md.res_id,md.script,_countof(md.script),&md.atk_ai,md.name,_countof(md.name),&md.effect_type,&md.effect_res,&md.appr,&md.lvl,&md.zslevel,
												 &md.cooleye,&md.exp,&md.power_exp,&md.hp,&md.mp,&md.ac,&md.acMax,&md.mac,&md.macMax,&md.dc,
												 &md.dcMax,&md.mc,&md.mcMax,&md.dodge,&md.accuracy,&md.walk_gap, &md.walk_gap2, &md.walk_step, &md.attack_spd, &md.head_icon, 
												 &md.talk_gap, &md.mon_type, &md.drop_group, &md.patrol_range, &md.fllow_range, &md.no_hit, &md.switch_target_type, &md.switch_target_inteval, &md.hatred_damage_ratio, &md.hatred_distance_ratio,
												 editdir, _countof(editdir), &type, &md.monGroup, &tel_id, ai,_countof(ai),&md.patrolTime,&md.deadTime,&md.refreshTime, &md.dirFixed, &md.CRIRate, 
												 &md.DUCrate, &md.ADTR, &md.MR, &md.HPup, &md.isShowRefresh, &md.isShowDrop, &md.BossPt,&param_id,&param,default_talk,_countof(default_talk),
												 &istalk,random_talk,_countof(random_talk),&easy_drop, &md.weapon, &md.wing, &md.title, &md.colltime, &md.no_show_hitter,&md.isShowTomb);
						if( 69 == ret )
						{
							//std::string tname = md.name;
							//word.clear();
							//for( std::string::iterator pos = tname.begin();
							//	pos != tname.end();
							//	++ pos )
							//{ 
							//	if( *pos == '0'|| *pos == '1'|| *pos == '2'|| *pos == '3'|| *pos == '4'|| *pos == '5'|| *pos == '6'|| *pos == '7'|| *pos == '8'|| *pos == '9')
							//	{
							//		continue;
							//	}
							//	word.push_back(*pos);
							//}
							UtilString::act_2_utf8(ai,strlen(ai),temp,1024);md.ai = temp;
							UtilString::act_2_utf8(md.name,strlen(md.name),temp,256);
							strncpy_s(md.name,_countof(md.name),temp,63);
							md.name[63]=0;
							//UtilString::act_2_utf8(word.c_str(),strlen(word.c_str()),notemp,256);
							strncpy_s(md.nodigital_name,_countof(md.nodigital_name),temp,63);
							md.nodigital_name[63]=0;

							md.colltime = md.colltime/1000;
							md.refreshTime = md.refreshTime / 1000;
							md.dirFixed -= 1;
							mMondefs[md.id] = md;
						}
					}
				}
			}
			for( MondefMap::iterator pos = mMondefs.begin();
			pos != mMondefs.end();
			++ pos )
			{
				//LoadMonDropDef(pos->second.name);
				LoadNewMonDropDef(pos->second.id);
				//LoadMonTalkDef(pos->second.name);
			}
		}
		return mMondefs.empty();
	}
	bool ManagerMonDef::LoadDropPlusCsv()
	{
		mMonDropPlusMap.clear();
		size_t size= 0;
		int index;
		int group_id;
		int equip_id;
		const char* data = mGameServerLib->GetListener()->LoadFile("dropplus.csv",size);
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
					int ret = sscanf_s(line,"%d,%d,%d",
						&index,&group_id,&equip_id);
					if( 3 == ret )
					{
						mMonDropPlusMap[group_id].push_back(equip_id);
					}
				}
			}
		}
		return !mMonDropPlusMap.empty();
	}
	bool ManagerMonDef::LoadMonDropCsv()
	{
		mNewMonDropGroups.clear();
		size_t size= 0;
		int index;
		int group_id;
		int gold_min;
		int gold_max;
		int item_id;
		int plus_id;
		int drop_prob;
		int bind;
		int owned_duration;
		const char* data = mGameServerLib->GetListener()->LoadFile("mondrop.csv",size);
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

					NewMonDropDef2 nmd;
					int ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d",
						&index,&group_id,&gold_min,&gold_max,&item_id,&plus_id,&drop_prob,&bind,&owned_duration);
					if( 9 == ret )
					{
						nmd.group_id = group_id;
						nmd.gold_min = gold_min;
						nmd.gold_max = gold_max;
						nmd.item_id = item_id;
						if (plus_id && (mMonDropPlusMap.find(plus_id) != mMonDropPlusMap.end()))
						{
							nmd.drop_plus = mMonDropPlusMap[plus_id];
						}
						nmd.drop_prob = drop_prob;
						nmd.bind = bind;
						nmd.owned_duration = owned_duration;
						nmd.start_drop_day = 0;
						mNewMonDropGroups[group_id].push_back(nmd);
					}
				}
			}
		}
		return !mNewMonDropGroups.empty();
	}
	void ManagerMonDef::ReloadMonDropDef()
	{
		LoadMonDropCsv();

		for( MondefMap::iterator pos = mMondefs.begin();
		pos != mMondefs.end();
		++ pos )
		{
			LoadNewMonDropDef(pos->second.id);
		}
	}
	bool ManagerMonDef::ReloadMonDropDef(std::string name)
	{
		return false;
	}
	void ManagerMonDef::LoadMonTalkDef(int mon_id)
	{
	}
	void ManagerMonDef::LoadMonDropDef(int mon_id)
	{
	}
	void ManagerMonDef::LoadNewMonDropDef(int mon_id)
	{
		MonDef* md = FindMonDef(mon_id);
		if (md)
		{
			int group_id = md->drop_group;
			if(mNewMonDropGroups.find(group_id) != mNewMonDropGroups.end() )
			{
				mNewMonDropGroupItem[mon_id] = mNewMonDropGroups[group_id];
			}
		}
	}
	void ParseMonDropOpt(const char* opt,NewMonDropDef* mdd)
	{
		char o[256];
		int lp = 0;
		size_t l = strlen(opt);
		for(size_t i = 0;i < l;i++)
		{
			if( opt[i] == ',' || i == l-1 )
			{
				memset(o,0,256);
				if( i == l-1 )
				{
					strncpy_s(o,_countof(o),&opt[lp],i-lp+1);
					o[255]=0;
				}
				else
				{
					strncpy_s(o,_countof(o),&opt[lp],i-lp);
					o[255]=0;
				}
				lp = i+1;
				if( isdigit(o[0]) )
				{
					mdd->count = atoi(o);
				}
				if( strcmp(o,"bind") == 0 )
				{
					mdd->bind = 1;
				}
				if( strcmp(o,"usebind") == 0 )
				{
					mdd->use_bind = 1;
				}
			}
		}
	}
	void ManagerMonDef::dest()
	{
	}

	
	MonDef* ManagerMonDef::FindMonDef(int mon_id)
	{
		MondefMap::iterator pos = mMondefs.find(mon_id);
		if( pos == mMondefs.end() )
		{
			return 0;
		}
		return &(pos->second);
	}
	std::vector<MonDropDef>* ManagerMonDef::FindMonDropDef(int mon_id)
	{
		//MonDropDefMap::iterator pos = mMonDropDefs.find(name);
		//if( pos == mMonDropDefs.end() )
		//{
			return 0;
		//}
		//return &(pos->second);
	}
	std::vector<NewMonDropDef2>* ManagerMonDef::FindNewMonDropGroupDef2(int mon_id)
	{
		NewMonDropGroupMap2::iterator pos = mNewMonDropGroupItem.find(mon_id);
		if( pos == mNewMonDropGroupItem.end() )
		{
			return 0;
		}
		return &(pos->second);
	}
	std::vector<MonDropGroup>* ManagerMonDef::FindMonDropGroupDef(int mon_id)
	{
		//MonDropGroupDefMap::iterator pos = mMonDropGroups.find(name);
		//if( pos == mMonDropGroups.end() )
		//{
			return 0;
		//}
		//return &(pos->second);
	}
	std::vector<MonTalkDef>* ManagerMonDef::FindMonTalkDef(int mon_id)
	{
		//MonTalkDefMap::iterator pos = mMonTalkDefs.find(name);
		//if( pos == mMonTalkDefs.end() )
		//{
			return 0;
		//}
		//return &(pos->second);
	}

	bool ManagerMonDef::LoadMonAICsv()
	{
		m_monAIs.clear();
		char temp[256];
		char ai[1024];
		char editdir[1024];
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("monai.csv",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1024];
			char desc[256];
			char aroundkind[256];
			char firstAtkKind[256];

			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					MonAtkAI mon_ai;
					int digital = 0;
					char notemp[64];
					std::string word;
					int ret = sscanf_s(line,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^,],%[^,],%d,%d,%d",
						&mon_ai.id, 
						desc,_countof(desc),
						&mon_ai.default_skill,
						&mon_ai.dis_skill_min,
						&mon_ai.dis_skill,
						&mon_ai.rate_skill,
						&mon_ai.rate_skill_rate, 
						&mon_ai.time_skill,
						&mon_ai.time_skill_need, 
						&mon_ai.life_time_skill,
						&mon_ai.life_time_skill_time,
						&mon_ai.life_time_skill_life_rate,
						&mon_ai.life_skill,
						&mon_ai.life_left_rate,
						&mon_ai.max_life_skill_cout,
						aroundkind,_countof(aroundkind),
						firstAtkKind,_countof(firstAtkKind),
						&mon_ai.switch_target_type,
						&mon_ai.switch_target_param,
						&mon_ai.hatred_distance_ratio
						);
					if( 20 == ret )
					{
						UtilString::act_2_utf8(desc,strlen(desc),temp,1024);
						mon_ai.strDesc = temp;
						mon_ai.around_kind = aroundkind;
						mon_ai.first_attack_kind = firstAtkKind;
						m_monAIs[mon_ai.id] = mon_ai;
					}
				}
			}
		}
		return true;
	}

	MonAtkAI * ManagerMonDef::FindAtkAI(int ai_id)
	{
		MonAIMap::iterator it = m_monAIs.find(ai_id);

		if (it == m_monAIs.end())
		{
			return NULL;
		}
		return &it->second;
	}

	bool ManagerMonDef::ReloadMonAIDef()
	{
		MonAIMap ai = m_monAIs;
		if (!LoadMonAICsv())
		{
			m_monAIs = ai;
			return false;
		}
		return true;
	}

}