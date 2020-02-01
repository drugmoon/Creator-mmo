#include "StdAfx.h"
#include "SubSkill.h"
#include "GameServerLib.h"
#include "ScriptLua.h"
#include "ManagerSkillDef.h"
namespace OGSLib
{
	SubSkill::SubSkill(){
		mTypeID=SkillDef::SKILL_TYPE_YiBanGongJi;
		mLevel=0;
		mExp=0;
		mTick=0.0;
		mParam1=0;
		mParam2=0;
	}
	bool SubSkill::add_exp(SkillDef* define,int exp,int player_lv)
	{
		if( ! define ) return false;
		if( exp > 100000000 || exp < -100000000 )
		{
			return false;
		}
		//防止溢出
		if (mExp > INT_MAX - exp)
		{
			return false;
		}
		mExp += exp;
		if( mExp < 0 )
		{
			mExp = 0;
		}
		//改成需要手动升级
		//check_level_up(define,player_lv);
		return true;
	}
	/*bool SubSkill::check_level_up(SkillDef* define,int player_lv)
	{
		if( mLevel < 0 || mLevel >= 4 )
		{
			mLevel = 0;
		}
		int level_up_exp = 0;
		int level_up_need= 0;
		if( mLevel == 0 )
		{
			level_up_exp = define->L1Train;
			level_up_need= define->NeedL1;
		}
		if( mLevel == 1 )
		{
			level_up_exp = define->L2Train;
			level_up_need= define->NeedL2;
		}
		if( mLevel == 2 )
		{
			level_up_exp = define->L3Train;
			level_up_need= define->NeedL3;
		}
		if( level_up_exp == 0 && level_up_need == 0 )
		{
			return false;
		}
		if( mExp >= level_up_exp && (mLevel+1)<=4 )
		{
			if( player_lv >= level_up_need )
			{
				mExp -= level_up_exp;
				mLevel += 1;
				return true;
			}
		}
		return false;
	}*/
	int SubSkill::check_level_up(SkillDef* define,int player_lv)
	{
		if( mLevel < 0 || mLevel > define->LevelMax )
		{
			mLevel = 0;
		}
		if (define->UpgradeNeedExp == 0 || define->NeedLevel == 0)
		{
			return -1;
		}
		if( mExp >= define->UpgradeNeedExp)
		{
			if ((mLevel+1)<=define->LevelMax)
			{
				if( player_lv >= define->NeedLevel )
				{
					mExp -= define->UpgradeNeedExp;
					mLevel += 1;
					return 1;
				}
			}
			return -2;
		}
		return 0;
	}
}