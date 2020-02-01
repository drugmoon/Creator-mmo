#include "StdAfx.h"
#include "PlayMonsterNomove.h"
#include "ManagerSkillDef.h"

namespace OGSLib
{
	PlayMonsterNomove::PlayMonsterNomove(void)
	{
	}
	PlayMonsterNomove::~PlayMonsterNomove(void)
	{
	}
	void PlayMonsterNomove::AiOther(double time)
	{
	}
	int PlayMonsterNomove::AiIdle()
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
				else if( (std::abs(ao->X()-X())) < 2 && (std::abs(ao->Y()-Y())) < 2 )
				{
					int dir = Diff2Dir((float)ao->X()-X(),(float)ao->Y()-Y());
					Turn(dir);
					int newx = NextX(X(),mDir);
					int newy = NextY(Y(),mDir);
					UseSkill(SkillDef::SKILL_TYPE_YiBanGongJi,newx,newy,ao->ID(),0);
					mAIState = AI_STATE_ATTACK;
				}
			}
			else
			{
				mAIState = AI_STATE_IDLE;;
			}
		}
		else
		{
			mAIState = AI_STATE_NULL;
		}
		return mAIState;
	}
}
