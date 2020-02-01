#include "StdAfx.h"
#include "ManagerSkillDef.h"
#include "GameServerlib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
namespace OGSLib
{
	// 基础值
	int SkillDef::DefPawer()
	{
		return DefPower + RandRange(DefMaxPower-DefPower);
	}
	int SkillDef::AdvPawer()
	{
		//return Power + RandRange(MaxPower-Power);
		return SkillAddPower;
	}
	int SkillDef::Pawer(int p,int lv)
	{
		/*double pt = p * (lv+1.0) / (3.0+1);
		return DefPawer()+Round(pt);*/
		return DefPawer() + p;
	}
	int SkillDef::Pawer13(int p,int lv)
	{
		return Pawer(p*2/3,lv) + p/3;
	}

	bool ManagerSkillDef::ReLoadSkillDefine(GameServerLib* lib)
	{
		SkillDefineMap i = mSkillDefineMap;
		if( ! LoadNewSkillDefine(lib) )
		{
			mSkillDefineMap = i;
			return false;
		}
		return true;
	}

	bool ManagerSkillDef::LoadSkillInfo(GameServerLib* lib)
	{
		mSkillLevelInfoMap.clear();
		int skill_type;
		int level_max;
		int need_level[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
		int need_exp[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("skilllevelinfo.csv",size);
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
						if (0 == strcmp(line,"#v101"))
						{
							version = 101;
						}
						continue;
					}
					int ret = 0;
					int parse_ok = 0;
					if (version = 100)
					{
						ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
											&skill_type,&level_max,
											&need_level[0],&need_exp[0],&need_level[1],&need_exp[1],&need_level[2],&need_exp[2],&need_level[3],&need_exp[3],&need_level[4],&need_exp[4],
											&need_level[5],&need_exp[5],&need_level[6],&need_exp[6],&need_level[7],&need_exp[7],&need_level[8],&need_exp[8],&need_level[9],&need_exp[9]);
						if( 22 == ret )
						{
							if( mSkillLevelInfoMap.find(skill_type) != mSkillLevelInfoMap.end() )
							{
								continue;
							}
							parse_ok = 1;
						}
					}
					if (parse_ok == 1)
					{
						SkillInfoVector mSkillInfoVector;
						for (int i = 0; i < 10; i++)
						{
							SkillInfo si;
							si.need_level = need_level[i];
							si.need_exp = need_exp[i];
							mSkillInfoVector.push_back(si);
						}
						mSkillLevelInfoMap[skill_type] = mSkillInfoVector;
					}
				}
			}
		}
		if( mSkillLevelInfoMap.empty() )
		{
			return false;
		}
		return true;

	}
	bool ManagerSkillDef::LoadSkillDefine(GameServerLib* lib)
	{
		mSkillDefineMap.clear();
		if (!LoadSkillInfo(lib))
		{
			return false;
		}

		char temp[256];
		int skill_type;
		int mag_id;
		char name[256];
		char desp[256];
		int iconID=0;
		int shortcut;
		int sound;
		int effect_type;
		int effect_resid;
		int spell;
		int power;
		int maxpower;
		int defspell;
		int defpower;
		int defmaxpower;
		int level_max;
		int range=0;
		int NeedL1;
		int L1Train;
		int NeedL2;
		int L2Train;
		int NeedL3;
		int L3Train;
		int Delay;
		int attribute_percentage = 10000;
		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("magicdef.csv",size);
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
						if (0 == strcmp(line,"#v101"))
						{
							version = 101;
						}
						continue;
					}
					memset(name,0,256);
					int ret = 0;
					int parse_ok = 0;
					if (version = 100)
					{
						ret = sscanf_s(line,"%d,%d,%[^,],%[^,],%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d",
											&skill_type,&mag_id,name,_countof(name),desp,_countof(desp),&iconID,&shortcut,&sound,&effect_type,&effect_resid,
											&spell,&power,&maxpower,&defspell,&defpower,&defmaxpower,&level_max,
											&range,&NeedL1,&L1Train,&NeedL2,&L2Train,&NeedL3,&L3Train,&Delay);
						if( 24 == ret )
						{
							if( mSkillDefineMap.find(skill_type) != mSkillDefineMap.end() )
							{
								continue;
							}
							parse_ok = 1;
						}
					}
					if (version = 101)
					{
						//新增伤害百分比
						ret = sscanf_s(line,"%d,%d,%[^,],%[^,],%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d",
											&skill_type,&mag_id,name,_countof(name),desp,_countof(desp),&iconID,&shortcut,&sound,&effect_type,&effect_resid,
											&spell,&power,&maxpower,&defspell,&defpower,&defmaxpower,&level_max,
											&range,&NeedL1,&L1Train,&NeedL2,&L2Train,&NeedL3,&L3Train,&Delay,&attribute_percentage);
						if( 25 == ret )
						{
							if( mSkillDefineMap.find(skill_type) != mSkillDefineMap.end() )
							{
								continue;
							}
							parse_ok = 1;
						}
					}
					if (parse_ok == 1)
					{
						SkillDef sd;
						sd.TypeID = skill_type;
						UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;sd.mName = name;
						UtilString::act_2_utf8(desp,strlen(desp),temp,256);strncpy_s(desp,_countof(desp),temp,255);desp[255]=0;sd.mDesp = desp;
						sd.IconID=iconID;
						sd.ShortCut = shortcut;
						sd.Sound = sound;
						sd.EffectType = effect_type;
						sd.Effect_ResID = effect_resid;
						sd.Spell = spell;
						sd.Power = power;
						sd.MaxPower = maxpower;
						sd.DefSpell = defspell;
						sd.DefPower = defpower;
						sd.DefMaxPower = defmaxpower;
						sd.LevelMax = level_max;
						sd.Range = range;
						sd.NeedL1 = NeedL1;
						sd.L1Train = L1Train;
						sd.NeedL2 = NeedL2;
						sd.L2Train = L2Train;
						sd.NeedL3 = NeedL3;
						sd.L3Train = L3Train;
						sd.Delay = Delay;
						sd.AttributePercentage = attribute_percentage;
						mSkillDefineMap[sd.TypeID] = sd;
					}
				}
			}
		}
		if( mSkillDefineMap.empty() )
		{
			return false;
		}
		return true;
	}
	
	//北京版
	bool ManagerSkillDef::LoadNewSkillDefine(GameServerLib* lib)
	{
		mSkillDefineMap.clear();
		/*if (!LoadSkillInfo(lib))
		{
			return false;
		}*/

		char temp[256];
		int skill_id;
		char name[256];
		int icon_id=0;
		int isShow=0;
		char desp[256];
		int job;
		int level;
		int need_level;
		int next_skill;
		int use_add_exp;
		int upgrade_need_exp;
		int skill_book;
		int type;
		int consume_mp;
		int consume_hp;
		int skill_cd;
		int public_cd;
		int cast_way;
		int select_target_way;
		int shape;
		int cast_distance_min;
		int cast_distance_max;
		int damage_range;
		int damage_num_max;	

		/*int target_type;
		int target_relation;*/
		char target_type[256];
		char target_relation[256];

		int damage_effect;
		int skill_attack_factor;	
		int skill_add_power;	
		int defense_damage_factor;	
		int trigger_buff1;	
		int trigger_buff2;	
		int trigger_function1_prob;	
		int trigger_function1;	
		int trigger_function2_prob;	
		int trigger_function2;	
		int param;
		int res_id;	
		int aoe_action;
		int effect_type;
		int attack_delay;
		int order_id;

		char damage_desp[256];
		char range_desp[256];
		char cd_desp[256];
		char extra_effect_desp[256];


		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("skilldef.csv",size);
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
						if (0 == strcmp(line,"#v101"))
						{
							version = 101;
						}
						continue;
					}
					memset(name,0,256);
					/*memset(desp,0,256);
					memset(damage_desp,0,256);
					memset(range_desp,0,256);
					memset(cd_desp,0,256);
					memset(extra_effect_desp,0,256);*/
					int ret = 0;
					int parse_ok = 0;
					if (version = 100)
					{
						ret = sscanf_s(line,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^,],%[^,],%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
											%[^,],%[^,],%[^,],%[^,]",
											&skill_id,name,_countof(name),&icon_id,&isShow,desp,_countof(desp),&job,&level,&need_level,&next_skill,&use_add_exp,&upgrade_need_exp,&skill_book,&type,
											&consume_hp, &consume_mp,&skill_cd,&public_cd,&cast_way,&select_target_way,&shape,&cast_distance_min,&cast_distance_max,&damage_range,&damage_num_max,target_type,_countof(target_type),target_relation,_countof(target_relation),&damage_effect,	
											&skill_attack_factor,&skill_add_power,&defense_damage_factor,&trigger_buff1,&trigger_buff2,&trigger_function1_prob,&trigger_function1,&trigger_function2_prob,&trigger_function2, &param, &res_id,&effect_type,&aoe_action,&attack_delay,&order_id,
											damage_desp,_countof(damage_desp),range_desp,_countof(range_desp),cd_desp,_countof(cd_desp),extra_effect_desp,_countof(extra_effect_desp));
						if( 46 == ret )
						{
							if( mSkillDefineMap.find(skill_id) != mSkillDefineMap.end() )
							{
								continue;
							}
							parse_ok = 1;
						}
					}
					if (parse_ok == 1)
					{
						SkillDef sd;

						sd.TypeID = skill_id / 100;
						sd.SkillID = skill_id;
						UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;sd.mName = name;
						UtilString::act_2_utf8(desp,strlen(desp),temp,256);strncpy_s(desp,_countof(desp),temp,255);desp[255]=0;sd.mDesp = desp;
						sd.IconID=icon_id;
						sd.ShortCut = 0;
						sd.Sound = 0;
						sd.EffectType = effect_type;
						sd.Effect_ResID = res_id;
						sd.Spell = 0;
						sd.Power = 0;
						sd.MaxPower = 0;
						sd.DefSpell = 0;
						sd.DefPower = 0;
						sd.DefMaxPower = 0;
						//技能最大等级，当前十级
						sd.LevelMax = 20;
						sd.Range = 0;
						sd.NeedL1 = 0;
						sd.L1Train = 0;
						sd.NeedL2 = 0;
						sd.L2Train = 0;
						sd.NeedL3 = 0;
						sd.L3Train = 0;
						sd.Delay = 0;
						sd.AttributePercentage = skill_attack_factor;

						sd.Job = job;

						sd.IsShow = isShow;
						sd.Level = level;
						sd.NeedLevel = need_level;
						sd.NextSkill = next_skill;

						sd.UseAddExp = use_add_exp;
						sd.UpgradeNeedExp = upgrade_need_exp;
						sd.SkillBook = skill_book;
						sd.DamType = type;
						sd.ConsumeHp = consume_hp;
						sd.ConsumeMp = consume_mp;
						sd.SkillCD = 0;
						if (skill_cd > 0)
						{
							//sd.SkillCD = skill_cd * 1.0 / 1000;
							sd.SkillCD = skill_cd;
						}
						sd.PublicCD = 0;
						if (public_cd > 0)
						{
							//sd.PublicCD = public_cd * 1.0 / 1000;
							sd.PublicCD = public_cd;
						}
						sd.CastWay = cast_way;
						sd.SelectTargetWay = select_target_way;
						sd.Shape = shape;
						sd.CastDistanceMin = cast_distance_min;
						sd.CastDistanceMax = cast_distance_max;
						sd.DamageRange = damage_range;
						sd.DamageNumMax = damage_num_max;

						sd.TargetType;
						sd.TargetRelation;
						sd.DamageEffect = damage_effect;
						sd.SkillAttackFactor = skill_attack_factor;
						sd.SkillAddPower = skill_add_power;
						sd.DefenseDamageFactor = defense_damage_factor;
						sd.TriggerBuff1 = trigger_buff1;
						sd.TriggerBuff2 = trigger_buff2;
						sd.TriggerFunction1Prob = trigger_function1_prob;
						sd.TriggerFunction1 = trigger_function1;
						sd.TriggerFunction2Prob = trigger_function2_prob;
						sd.TriggerFunction2 = trigger_function2;
						sd.Param0 = param;
						sd.ResID = res_id;
						sd.AOEAction = aoe_action;
						sd.Attack_Delay = attack_delay;
						sd.OrderID = order_id;

						UtilString::act_2_utf8(damage_desp,strlen(damage_desp),temp,256);strncpy_s(damage_desp,_countof(damage_desp),temp,255);damage_desp[255]=0;sd.DamageDesp = damage_desp;
						UtilString::act_2_utf8(range_desp,strlen(range_desp),temp,256);strncpy_s(range_desp,_countof(range_desp),temp,255);range_desp[255]=0;sd.RangeDesp = range_desp;
						UtilString::act_2_utf8(cd_desp,strlen(cd_desp),temp,256);strncpy_s(cd_desp,_countof(cd_desp),temp,255);cd_desp[255]=0;sd.CDDesp = cd_desp;
						UtilString::act_2_utf8(extra_effect_desp,strlen(extra_effect_desp),temp,256);strncpy_s(extra_effect_desp,_countof(extra_effect_desp),temp,255);extra_effect_desp[255]=0;sd.ExtraEffectDesp = extra_effect_desp;

						mSkillDefineMap[sd.SkillID] = sd;
					}
				}
			}
		}
		if( mSkillDefineMap.empty() )
		{
			return false;
		}
		return true;
	}
	bool ManagerSkillDef::LoadSkillParam(GameServerLib* lib)
	{
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("skillplus.txt",size);
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
					SkillPlus sp;
					int ret = sscanf_s(line,"%d%d%d%d%d",&sp.skill_id,&sp.base_acc,&sp.lv_acc,&sp.base_dodge,&sp.lv_dodge);
					if( 5 == ret )
					{
						mSkillPlusMap[sp.skill_id] = sp;
					}
				}
			}
		}
		return true;
	}
	void ManagerSkillDef::Dest()
	{
		mSkillDefineMap.clear();
	}
	SkillDef* ManagerSkillDef::GetSkillDefine(int skill_type, int skill_level)
	{
		int skill_id = skill_type * 100 + skill_level;
		SkillDefineMap::iterator pos = mSkillDefineMap.find(skill_id);
		if( pos != mSkillDefineMap.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
	SkillPlus* ManagerSkillDef::GetSkillPlus(int skill_type)
	{
		SkillPlusMap::iterator pos = mSkillPlusMap.find(skill_type);
		if( pos != mSkillPlusMap.end() )
		{
			return &(pos->second);
		}
		return 0;
	}

	bool ManagerSkillDef::IsMagicSkill(int skill_type, int skill_level)
	{
		int skill_id = skill_type * 100 + skill_level;
		SkillDefineMap::iterator pos = mSkillDefineMap.find(skill_id);
		if( pos != mSkillDefineMap.end() )
		{
			if (pos->second.DamType == 3 || pos->second.DamType == 2)
			{
				return true;
			}
		}
		return false;
	}

	SkillDef* ManagerSkillDef::GetSkillDefineById(int skill_id)
	{
		SkillDefineMap::iterator it = mSkillDefineMap.find(skill_id);
		if (it == mSkillDefineMap.end())
		{
			return NULL;
		}
		return &it->second;
	}

}