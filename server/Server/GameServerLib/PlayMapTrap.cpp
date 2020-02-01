#include "StdAfx.h"
#include "PlayMapTrap.h"
#include "DomainPlay.h"
#include "PlayMap.h"
#include "ManagerMap.h"
#include "GameServerLib.h"
#include "PlayActiveObject.h"
#include "ManagerSkillDef.h"
#include "ManagerTrapDef.h"

namespace OGSLib
{

	PlayMapTrap::PlayMapTrap(int x,int y,TrapDef * def, PlayActiveObject * caller)
	{
		mType=MAP_OBJECT_TYPE_TRAP;
		SetPosition(x,y);
		mCloth = 0;
		mCaller = caller;
		mPickLimitLv = 0;
		mDir = 4;
		mTriggerTimes = -1;
		mLifeTimeRemove = false;
		if (def)
		{
			mLifeTime = def->life_time;
			mDamageGap = def->damage_gap;
			mDamage = def->damage;
			mSkillType = def->skill_type;
			mEffect = def->effect_id;
			mCloth = def->cloth;
			mTrapTypeID = def->id;
			mCallerLeaveRemove = def->caller_leave_remove;
			if(!def->name.empty())
			{
				mName  = def->name;
			}
		}
	}

	PlayMapTrap::~PlayMapTrap(void)
	{
	}

	void PlayMapTrap::NotifyMapEnter(PlayMap* map)
	{
		PlayMapObject::NotifyMapEnter(map);

// 		if (mLifeTime > 0)
// 		{
// 			mGSL->GetSecondTimer()->AddTimer(mLifeTime,this,ID());
// 		}

		if (mDamageGap > 0)
		{
			int gap = mDamageGap / 1000;
			gap = gap < 1 ? 1:gap;

			// 计算触发次数
			if (mLifeTime > 0)
				mTriggerTimes = mLifeTime / gap;
			
			mGSL->GetSecondTimer()->AddTimer(gap,this,ID());
		}
		else
		{
			if (mLifeTime > 0)
			{
				mLifeTimeRemove = true;
				mGSL->GetSecondTimer()->AddTimer(mLifeTime,this,ID());
			}
		}
		
		// 获得该位置上的ActiveObj,加上buf
		MapCell* cell = const_cast<MapCell*>(map->FindMapCell(X(), Y()));
		bool bFind = false;
		if( cell)
		{
			PlayActiveObject* objects[20];
			int finded_num = 0;
			for( PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();
				pos = pos->Next)
			{
				if( pos->This->IsInteractive() )
				{
					objects[finded_num] = (PlayActiveObject*)pos->This;
					finded_num ++;
					if(finded_num >= 20)
					{
						break;
					}
				}
			}

			for(int i = 0; i < finded_num; ++ i)
			{
				if(objects[i])
				{
					NotifyWalkIn(objects[i]);
				}
			}
		}
	}

	void PlayMapTrap::NotifyMapLeave(PlayMap* map)
	{
		// 获得该位置上的ActiveObj
		MapCell* cell = const_cast<MapCell*>(map->FindMapCell(X(), Y()));
		bool bFind = false;
		if( cell)
		{
			for( PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();
				pos = pos->Next)
			{
				if(pos->This->IsInteractive())
				{
					NotifyWalkOut((PlayActiveObject *)(pos->This));
				}
			}
		}
		
		PlayMapObject::NotifyMapLeave(map);
	}

	void PlayMapTrap::RemoveFromMap()
	{
		if (this->mCaller)
			mCaller->DecCallTrap(this);
		mGSL->GetSecondTimer()->RemoveObject(this);
		if (mParentMap)
		{
			mParentMap->RemoveObject(this);
		}
	}

	void PlayMapTrap::OnSecondTimer(int param)
	{
		if (mLifeTimeRemove)
		{
			RemoveFromMap();
			delete this;
			return;
		}
		else
		{
			if (mTriggerTimes > 0)
			{
				-- mTriggerTimes;
			}

			if (mTriggerTimes == 0)
			{
				mGSL->GetSecondTimer()->RemoveObject(this);
				RemoveFromMap();
				delete this;
				return;
			}

			OnTrapGap();
			int gap = mDamageGap / 1000;
			gap = gap < 1 ? 1:gap;
			mGSL->GetSecondTimer()->AddTimer(gap,this,ID());
		}
	}

	const char* PlayMapTrap::Name()
	{
		return mName.c_str();
	}

	void PlayMapTrap::NotifyWalkOut(PlayActiveObject* src)
	{
		DomainPlay* player = 0;
		if (src && src->TypeIsPlayer())
		{
			player = (DomainPlay*)src;
			player->LeaveTrap(src, mTrapTypeID);
		
		}
	}

	void PlayMapTrap::NotifyWalkIn(PlayActiveObject* src)
	{
		DomainPlay* player = 0;
		if (src && src->TypeIsPlayer())
		{
			player = (DomainPlay*)src;
			player->EnterTrap(src, mTrapTypeID);
		}
	}

	int PlayMapTrap::GetTrapTypeId()
	{
		return mTrapTypeID;
	}

	void PlayMapTrap::ClearCaller()
	{
		mCaller = NULL;
	}

	bool PlayMapTrap::GetCallerLeaveRemove()
	{
		return mCallerLeaveRemove;
	}

	void PlayMapTrap::OnTrapGap()
	{
		PlayMap* map = this->GetMap();
		if (!map)
		{
			return;
		}

		MapCell* cell = const_cast<MapCell*>(map->FindMapCell(X(), Y()));
		bool bFind = false;
		if( cell)
		{
			std::vector<PlayActiveObject *> vec;
			for( PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();
				pos = pos->Next)
			{
				if(pos->This->IsInteractive())
				{
					PlayActiveObject * src = (PlayActiveObject *)(pos->This);
					vec.push_back(src);
				}
			}
			
			for (std::vector<PlayActiveObject *>::iterator it = vec.begin(); it != vec.end(); ++ it)
			{
				PlayActiveObject * src = (PlayActiveObject *)(*it);
				if (!src->IsDead()&&src->Hp()>0)
				{
					int damage = mDamage;
					SkillDef * sd = mGSL->GetSkillDefineManager()->GetSkillDefine(mSkillType);
					if (mCaller)
					{
						if (!mCaller->IsProperAttackDes(src, sd))
							continue;

						if (sd)
							damage = src->Attacked(mCaller, sd, mDamage, 0, sd->Attack_Delay);
					}
					else
					{
						int defence = 0;
						if (sd)
						{
							defence = src->CalculateDefencePower(sd);

							damage = max(0,damage-defence);

							// 计算技能抵消伤害,(魔法盾....)
							if (damage > 0)
								damage = src->CalculateSkillDefence(damage);
						}
						src->DamageHP(0, damage, 0);
					}
				}
			}
 		}
	}

}
LEUD::PoolMemory<OGSLib::PlayMapTrap>	LEUD::PoolMemoryBase<OGSLib::PlayMapTrap>::memPool("PlayMapTrap");
