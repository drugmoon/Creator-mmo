#include "StdAfx.h"
#include "PlayNeutral.h"
#include "ManagerSkillDef.h"
#include "DomainPlay.h"
#include "GuildGuild.h"
#include "GameServerLib.h"
#include "PlaySlave.h"
#include "PlayMap.h"
namespace OGSLib
{
	PlayNeutral::PlayNeutral(void)
	{
		mType=MAP_OBJECT_TYPE_NEUTRAL;
		mValor = 200;
		mNomove = true;
		memset(mGuildName,0,256);
		m_nMonsterSeedId = 0;
		m_nDisappearTime = 0;
	}
	PlayNeutral::~PlayNeutral(void)
	{
	}
	void PlayNeutral::NotifyMeet(PlayMap* map,PlayMapObject* src)
	{
		PlayMonster::NotifyMeet(map,src);
		if( ! Target() )
		{
			CheckTarget(src,src->X(),src->Y());
		}
	}
	int PlayNeutral::Attacked(PlayActiveObject* src,SkillDef * sd,int pawer,int flags,int delay)
	{
		if (m_nMonsterSeedId == 0)
		{
			if(!Target())
			{
				Target(src);
			}

			if(Target() && Target()!=src)
			{
				Target(src);
			}
		}
	
		return PlayMonster::Attacked(src,sd,pawer,flags,delay);
	}
	void PlayNeutral::CheckTarget(PlayMapObject* src,int newx,int newy)
	{
		if (Target() || !src)
			return;
		//Ä¹±®²»¹¥»÷
		if (m_nMonsterSeedId > 0)
		{
			return;
		}

		if( src->TypeIsPlayer() )
		{
			DomainPlay* play = static_cast<DomainPlay*>(src);
			if(play->PKValue() >= 400)
			{
				Target((PlayActiveObject*)src);
				return;
			}
		}

		if (src->TypeIsSlave())
		{
			PlaySlave* slave = static_cast<PlaySlave*>(src);
			if (slave->GetSlaveType() == Slave_type_dog)
			{
				Target((PlayActiveObject*)src);
			}
		}
	}
	int PlayNeutral::AiIdle()
	{
		//printf("%f\n", mGSL->GetTimerFix()->Elapsed());
		if (X() != mReliveX || Y()!= mReliveY)
		{
			ForceMove(mReliveX, mReliveY, mDirFixed);
			mAIState = AI_STATE_ATTACK;
			return mAIState;
		}

		if( mPlayerList.size() > 0 )
		{
			if( Target() )
			{
				PlayActiveObject* ao=Target();
				if( ao->IsDead() )
				{
					ClearTarget();
					mAIState = AI_STATE_IDLE;
				}
				else
				{
					IntPointClass np(ao->X(), ao->Y());
					
					bool can_fly = false;
					int dir = ao->Dir();
					for (size_t i = 0; i < 8; ++ i)
					{
						np = ao->NextPosition(dir + i,1,2);
						if (np.x != ao->X() || np.y != ao->Y())
						{
							can_fly = true;
							break;
						}
					}

					if (can_fly)
					{
						int dir = Diff2Dir(ao->X()-np.x,ao->Y()-np.y);
						ForceMove(np.x, np.y, dir);
						int skill = ChooseSkill(ao);
						SkillDef *sd = mGSL->GetSkillDefineManager()->GetSkillDefineById(skill);
						onAiAttack(ao, sd);
						mAIState = AI_STATE_ATTACK2;
					}
					else
					{
						mAIState = AI_STATE_ATTACK;
					}
				}
			}
		}
		else
		{
			mAIState = AI_STATE_NULL;
		}
		return mAIState;
	}
	void PlayNeutral::AiOther(double time)
	{
	}

	bool PlayNeutral::IsProperAttackDes(PlayActiveObject* des, SkillDef *sd)
	{
		if (des == Target())
			return true;
		return false;
	}

	/*int PlayNeutral::CollectTime()
	{
		if (m_nMonsterSeedId > 0)
		{
			PlayMonster* pMon = mParentMap->FindMonster(m_nMonsterSeedId);
			if (pMon)
			{
				return pMon->GetRefreshRemain();
			}
		}
		return 0;
	}*/

	void PlayNeutral::RestartDisappear(int time)
	{
		m_DisappearTime.Restart();
		m_nDisappearTime = time;
		int a = m_DisappearTime.Elapsed();
	}

	int PlayNeutral::GetDisappearTime()
	{
		int b = m_DisappearTime.Elapsed();
		if (m_nDisappearTime > 0 && m_nDisappearTime > m_DisappearTime.Elapsed())
		{
			return m_nDisappearTime -  m_DisappearTime.Elapsed();
		}
		return 0;
	}

}
