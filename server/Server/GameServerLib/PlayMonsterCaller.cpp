#include "StdAfx.h"
#include "PlayMonsterCaller.h"
#include "ManagerMap.h"
#include "PlayMap.h"
#include "ManagerSkillDef.h"

namespace OGSLib
{
	PlayMonsterCaller::PlayMonsterCaller(void)
	{

	}

	PlayMonsterCaller::~PlayMonsterCaller(void)
	{
	}

	int PlayMonsterCaller::AiIdle()
	{
		if( mPlayerList.size() > 0 )
		{
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
 					//bool attack = true;
					int dir = Diff2Dir((float)ao->X()-X(),(float)ao->Y()-Y());

					if (mDir != dir)
						Turn(dir);

					if (mMonstersCalled.size() >= mCallMonMaxCount)
					{
						mAIState = AI_STATE_IDLE;
					}
					else
					{
						if (mCallMonName && mCallMonCountOnce > 0)
						{
							UseSkill(SkillDef::Skill_Type_Monster_Zhaohuan,ao->X(),ao->Y(),ao->ID(),0);
							mAIState = AI_STATE_ATTACK;
						}
						else
							mAIState = AI_STATE_IDLE;
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

	void PlayMonsterCaller::OnDead(PlayActiveObject* src,int delay)
	{
		// 如果召唤师死了,需要干掉他召唤出来的怪物,此处可以处理
		PlayMonster::OnDead(src, delay);
	}
}