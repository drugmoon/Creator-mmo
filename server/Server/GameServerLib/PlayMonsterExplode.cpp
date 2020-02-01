#include "StdAfx.h"
#include "PlayMonsterExplode.h"
#include "ManagerMap.h"
#include "PlayMap.h"
#include "ManagerSkillDef.h"

namespace OGSLib
{
	PlayMonsterExplode::PlayMonsterExplode(void)
	{

	}

	PlayMonsterExplode::~PlayMonsterExplode(void)
	{
	}

	int PlayMonsterExplode::AiIdle()
	{
		if( mPlayerList.size() > 0 )
		{
			if (mCaller && mCaller->Target())
			{
				// 将自爆怪的目标设置成"召唤者"的目标
				SwitchTarget(mCaller->Target());
			}

			if( Target() )
			{
				PlayActiveObject* ao=Target();
				if( ao->IsDead() || IsDesYinShen(ao) || (!IsProperAttackDes(ao, NULL)) )
				{
					ClearTarget();
					mAIState = AI_STATE_IDLE;
				}
				else
				{
					 if( (std::abs(ao->X()-X())) < 2 && (std::abs(ao->Y()-Y())) < 2)
					 {
						// 靠近目标自爆
						 UseSkill(SkillDef::Skill_Type_Monster_Explode,ao->X(),ao->Y(),ao->ID(),0);
						 DamageHP(ao, Hp(), 0);
						 mAIState = AI_STATE_ATTACK;
					 }
					 else
					 {
						 Walk(ao->X(),ao->Y(),2);
						 mAIState = AI_STATE_WALK;
					 }
				}
			}
			else
			{
				mAIState = AI_STATE_IDLE;
			}
		}
		else
		{
			mAIState = AI_STATE_NULL;
		}
		return mAIState;
	}

	void PlayMonsterExplode::OnDead(PlayActiveObject* src,int delay)
	{
		// 自爆死后处理
		PlayMonster::OnDead(src, delay);
	}
}