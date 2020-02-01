#include "StdAfx.h"
#include "PlayMonsterRemote.h"
#include "ManagerSkillDef.h"
namespace OGSLib
{
	PlayMonsterRemote::PlayMonsterRemote(void)
	{
		mValor = 3;
		mNomove = false;
	}
	PlayMonsterRemote::~PlayMonsterRemote(void)
	{
	}
	int PlayMonsterRemote::AiIdle()
	{
		if( mPlayerList.size() > 0 )
		{
			if( Target() )
			{
				PlayActiveObject* ao=Target();
				if( ao->IsDead() || IsDesYinShen(ao) || (!IsProperAttackDes(ao,NULL)) )
				{
					ClearTarget();
					mAIState = AI_STATE_IDLE;
				}
				else
				{
					bool attack = true;
					if( mValor < 100 )
					{
						if( (std::abs(ao->X()-X())) < 2 && (std::abs(ao->Y()-Y())) < 2 )
						{
							if( rand() % mValor == 0 )
							{
								int dir = Diff2Dir((float)X()-ao->X(),(float)Y()-ao->Y());
								IntPointClass ipc = NextPosition(dir,3);
								dir = this->FindNextDirection(X(),Y(),ipc.x,ipc.y);
								Walk(dir);
								mAIState = AI_STATE_WALK;
								attack = false;
							}
						}
					}
					if( attack )
					{
						int dir = Diff2Dir((float)ao->X()-X(),(float)ao->Y()-Y());
						Turn(dir);
						UseSkill(SkillDef::SKILL_TYPE_MonArrow,ao->X(),ao->Y(),ao->ID(),0);
						mAIState = AI_STATE_ATTACK;
						//onAiAttack(ao);
					}
				}
			}
			else
			{
				if( ! mNomove )
				{
					RandGo();
				}
				mAIState = AI_STATE_IDLE;
			}
		}
		else
		{
			if( ! mNomove )
			{
				RandGo();
			}
			mAIState = AI_STATE_NULL;
		}
		return mAIState;
	}
}
