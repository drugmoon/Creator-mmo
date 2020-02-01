#include "StdAfx.h"
#include "PlaySlave.h"
#include "ScriptLua.h"
#include "PlayMap.h"
#include "SubSkill.h"
#include "DomainPlay.h"
#include "ManagerSkillDef.h"
#include "GameServerLib.h"
#include "TimerFrame.h"
#include "ManagerMap.h"
#include "ManagerItemDef.h"
#include "SubItem.h"

namespace OGSLib
{
	PlayDart2::PlayDart2(void)
	{
		mType=MAP_OBJECT_TYPE_DART;
		mAlertOut = 1;
		mDartTaskStartTick = 0;
		mCurPathIndex = 0;
		mReached = false;
		m_nStopX = -1;
		m_nStopY = -1;
		m_nLifeTime = 15 * 60;
	}
	PlayDart2::~PlayDart2(void)
	{
	}
	bool PlayDart2::IsOwnerCloseMe()
	{
		if( ! mLiving ) return false;
		if (! mOwner) return false;
		if( mOwner->GetMap() != mParentMap ) return false;
		int disX = std::abs(mOwner->X()-X());
		int disY = std::abs(mOwner->Y()-Y());
		if( ( disX * disX + disY * disY )<= 16 )
		{
			return true;
		}
		return false;
	}
	int PlayDart2::AiIdle()
	{
		if( ! mParentMap ) return AI_STATE_NULL;
		if (mDartTaskStartTick + m_nLifeTime < time(0))
		{
			if (mOwner)
			{
				mGSL->GetScriptEngine()->OnlyPlayer(mOwner, "player.onDartTimeOut");
			}
			DamageHP(this,999999999,0);
			return AI_STATE_NULL;
		}

		if( IsOwnerCloseMe() )
		{
			if (mAlertOut == 0)
			{
				if (mOwner)
				{
					mGSL->GetScriptEngine()->OnlyPlayer(mOwner, "player.onDartInRange");
				}
			}

			const std::vector<MapPoint> & dart_path = mGSL->GetMapManager()->GetDartPath();
			if (mCurPathIndex == dart_path.size())
			{
				if (!mReached)
				{
					if (mOwner)
					{
						mGSL->GetScriptEngine()->OnlyPlayer(mOwner, "player.onDartReached");
					}
					mReached = true;
				}
				// 到达目的地
				return AI_STATE_NULL;
			}

			if (X() == m_nStopX && Y() == m_nStopY)
			{
				return AI_STATE_IDLE;
			}

			// 判断是否到达当前路径点
			if (X() == dart_path[mCurPathIndex].x && Y() == dart_path[mCurPathIndex].y)
			{
				if (dart_path[mCurPathIndex].stop == 1)
				{
					m_nStopX = dart_path[mCurPathIndex].x;
					m_nStopY = dart_path[mCurPathIndex].y;
					++ mCurPathIndex;
					return AI_STATE_IDLE;
				}
				++ mCurPathIndex;
			}

			if (mCurPathIndex < dart_path.size())
			{
				// 获取路径点
				Walk(dart_path[mCurPathIndex].x,dart_path[mCurPathIndex].y,1);
			}
			mAlertOut = 1;
			return AI_STATE_WALK;
		}

		if( mAlertOut == 1 )
		{
			mAlertOut = 0;
			if (mOwner)
			{
				mGSL->GetScriptEngine()->OnlyPlayer(mOwner, "player.onDartOutRange");
			}

			return AI_STATE_NULL;
		}
		return mAIState;
	}

	bool PlayDart2::DamageHP(PlayActiveObject* src,int damage,int delay ,bool safeAreaEnable)
	{
		if( ! (damage>0) ) return false;
		if( IsDead() ) return false;
		if( Type() == MAP_OBJECT_TYPE_PLAYER )
		{
			if( SafeArea() > 0 ) return false;
		}
		if( damage == 999999999 )
		{
			OnDead(src,delay);
			mDieClear = 1;
			if (mOwner)
				mOwner->mDart2 = NULL;
			return true;
		}
		if( mHp <= 1 )
		{
			return false;
		}

		mHp -= damage;
		if( mHp > 1 )
		{
			if (src && src->TypeIsPlayer() && mOwner)
			{
				mGSL->GetScriptEngine()->PlayerPlayer(mOwner,(DomainPlay*)src,"player.onDartAttacked");
			}

			OnHurt(src,-damage,delay,0);
		}
		else
		{
			if (src && src->TypeIsPlayer() && mOwner)
			{
				mGSL->GetScriptEngine()->PlayerPlayer(mOwner,(DomainPlay*)src,"player.onDartDestoryed");
			}

			DropItem();
			OnDead(src,delay);
			mDieClear = 1;
			if (mOwner)
				mOwner->mDart2 = NULL;
		}
		return true;
	}

	void PlayDart2::Destroyed()
	{
		mCloth++;
		mHp=1;
		ClearAllStatus();
		if (mOwner)
		{
			mOwner->NotifyHPMPChange(this,0,0);
			mOwner->NotifyAvatarChange(this);
			mOwner->mDart2 = NULL;
		}

		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyHPMPChange(this, 0, 0);
				(*pos)->NotifyAvatarChange(this);
			}
		}
	}

	bool PlayDart2::StopPosReached()
	{
		if (X() == m_nStopX && Y() == m_nStopY)
		{
			return true;
		}
		return false;
	}

	void PlayDart2::DropItem()
	{
		std::vector<NewMonDropDef2>* pmddv = mGSL->GetMonDefManager()->FindNewMonDropGroupDef2(mDefId);
		std::string item_str = "";
		int itemId;
		if( pmddv )
		{
			int mul = 10;
			int day_start = mGSL->GetServerStartDays();///获取开区天数

			for (std::vector<NewMonDropDef2>::iterator pos = pmddv->begin();
				pos != pmddv->end();
				++pos)
			{
				itemId = 0;
				if (pos->gold_min && pos->gold_max)
				{
					itemId = ITEM_GOLD_ID;
				}
				else if (pos->item_id)
				{
					itemId = pos->item_id;
				}
				else if (pos->drop_plus.size() > 0)
				{
					int index = (rand() % pos->drop_plus.size());
					itemId = pos->drop_plus[index];
				}
				if (itemId)
				{
					ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(itemId);

					if ((rand() % (10000 * 10)) < pos->drop_prob * mul /*&& (day_start >= pos->start_drop_day)*/)
					{
						ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(itemId);
						if (id)
						{
							SubItem ii;
							bool rand_add = false;
							int flags = 0;
							if (mGSL->mMonDropBind == 1 || pos->bind == 1)
							{
								flags = ITEM_FLAG_BIND;
							}
							else if (mGSL->mMonDropUseBind == 1 || pos->bind == 2)
							{
								flags = ITEM_FLAG_USE_BIND;
							}
							ii.BuildItemInstance(id, flags, rand_add, mGSL->mItemAddType);
							int count = 1;
							if (itemId == ITEM_GOLD_ID)
							{
								count = (rand() % (pos->gold_max - pos->gold_min + 1)) + pos->gold_min;
							}
							int owner_id = 0;
							int pick_limit_lv = 0;

							ItemDef* drop_id = mGSL->GetItemDefineManager()->GetItemDefine(ii.mTypeID);
							DropTheItem(&ii, count, owner_id, pick_limit_lv);
						}
					}
				}					
			}
			
		}
	}

	PlaySlave::PlaySlave(void)
	{
		mType=MAP_OBJECT_TYPE_SLAVE;
		mOwner = 0;
		mLiving = false;
		mSkill = 0;
		mFkAttPlus = 0;
		mAutoFindNext = true;
		mDelayTick=0.0;
		mDelayTime=0.0;
		mGongshaTick=0.0;
		mLiehuoTick=0.0;
		mBlockCheckType=2;
		mWalkTick = 0.0;
		mLocktarget=0;
		mAiType = 0;
		mStartPosX = -1;
		mStartPosY = -1;
		mMoveRange = 0;
		mCoolEye = 0;
		m_dLockTargetTime = 0;
		mExp = 0;
		mNewTarget = true;
		mNameShowOwner = true;
		memset(&m_nHateIDList, 0, sizeof(int) * HATE_LIST_MAX_PRIORITY);
	}

	PlaySlave::~PlaySlave(void)
	{
	}

	void PlaySlave::FindNextTarget(PlayActiveObject* t)
	{
		if (t && !t->IsProperAttackDes(t , NULL))
		{
			return;
		}
		if( t && t->TypeIsMonster() )
		{
			PlayMonster* mon = static_cast<PlayMonster*>(t);
			if( mon->CollectTime() > 0 )
			{
				return;
			}
		}
		PlayActiveObject* ta = t;
		int dis = 100000;
		if (mCoolEye > 0)
			dis = mCoolEye;
		if( ta )
		{
			 int dis2 = (abs(t->X() - X()) + abs(t->Y()-Y()))-1;
			 if (dis2 < dis)
				 dis = dis2;
		}

		if( Target() )
		{
			PlayActiveObject* taget = Target();
			if( taget )
			{
				int d = abs((taget)->X()-X()) + abs((taget)->Y()-Y());
				// 当前目标的距离 如果小于 新设置目标的距离
				// 当前目标不变
				if( dis >= d )
				{
					dis = d;
					ta = taget;
				}
			}
		}
		if( t == 0 )
		{
			for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
				pos != mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsMonster() && dis >= 0)
					{
						PlayMonster* mon = static_cast<PlayMonster*>(*pos);
						if (mon->CollectTime() > 0 || mon->IsDead())
						{
							continue;
						}
						int d = abs((*pos)->X() - X()) + abs((*pos)->Y() - Y());
						if (dis >= d)
						{
							// 选择最近目标
							dis = d;
							ta = (*pos);
						}
					}
				}
			}
		}
		if( ta )
		{
			double now = mGSL->GetTimerFix()->Elapsed();

			if (Target() == 0 || now - m_dLockTargetTime > LOCK_TARGET_TIME)
			{
				Target(ta);
				m_dLockTargetTime = now;
				mNewTarget = true;
			}
		}
	}
	void PlaySlave::Relive(PlayMap* map,int x,int y,int dir)
	{
		SetState(PlayMapObject::MAP_OBJECT_STATE_IDLE);
		mLiving = true;
		ResetAttribute();
		ForceMove(x,y,dir);
		map->AddObject(this);
	}
	void PlaySlave::OnDead(PlayActiveObject* src,int delay)
	{
		if (GetSlaveType() == Slave_type_huwei)
		{
			mGSL->GetScriptEngine()->OnlySlave(this, "slave.onHuweiDead");
		}
		mGSL->GetScriptEngine()->OnlySlave(this, "slave.onSlaveDead");
		
		ClearStatusOnDead();
		ClearBuffOnDead();
		SetState(PlayMapObject::MAP_OBJECT_STATE_DEAD);
		PlayMap* map = mParentMap;
		if( map )
			map->RemoveObject(this);
		mLiving = false;
		if (mOwner)
		{
			mOwner->changeSlaveState(0);
		}
		PlayAIObject::OnDead(src,delay);
	}
	int PlaySlave::Update(double time)
	{
		//printf("slave update %.2f\n", time);
		UpdateStatusTimer(time);
		UpdateBuffTimer(time);
		if( mDelayTick + mDelayTime < time )
		{
			switch(mAiType)
			{
			case 0:
				mAIState = AiIdle();
				break;
			case 1:
				mAIState = AiIdleZS(time);
				break;
			case 2:	
				mAIState = AiIdleFS(time);
				break;
			case 3:
				mAIState = AiIdleDS(time);
				break;
			case 4:
				mAIState = AiIdle();
				break;
			case 5:
				mAIState = AiIdle();
				break;
			}
			mDelayTick = time;
			mDelayTime = TimerFrame::FRAME_TICK;
			switch(mAIState)
			{
			case AI_STATE_IDLE:
				mDelayTime = TimerFrame::FRAME_TICK;
				break;
			case AI_STATE_WALK:
				mDelayTime = mWalkGap * 1.0 / 1000;
				break;
			case AI_STATE_ATTACK:
				mDelayTime = mAttackGap * 1.0 / 1000;
				break;
			case AI_STATE_ATTACK2:
				mDelayTime = mAttackGap * 1.0 / 1000;
				break;
			case AI_STATE_NULL:
				mDelayTime = TimerFrame::FRAME_TICK;
				break;
			}
		}
		return 0;
	}

	int PlaySlave::AiIdle()
	{
		if( ! mParentMap ) return AI_STATE_NULL;
		int nOwnerDisX = std::abs(mOwner->X()-X());
		int nOwnerDisY = std::abs(mOwner->Y()-Y());
		int nOwnerDis = nOwnerDisX > nOwnerDisY ? nOwnerDisX : nOwnerDisY;
		if( nOwnerDis > 9 )
		{
			ForceMove(mOwner->X(),mOwner->Y(),mOwner->Dir());
			return AI_STATE_IDLE;
		}

		FindHateListTarget();
		
		if( Target() )
		{
			PlayActiveObject* ao = Target();

			if( ao->IsDead() || ao->IsYingShen())
			{
				ClearTarget();
				mAIState = AI_STATE_IDLE;
			}
			else
			{
				// 选择技能
				int skill = ChooseSkill(ao);
				SkillDef * skill_def = mGSL->GetSkillDefineManager()->GetSkillDefineById(skill);
				int disX = std::abs(ao->X()-X());
				int disY = std::abs(ao->Y()-Y());

				int dis = disX > disY ? disX : disY;

				int moveDisEnd = 2;

				if (skill_def != NULL)
					moveDisEnd = skill_def->CastDistanceMax + 1;

				bool bCanUseSkill = false;
				if (skill_def != NULL 
					&& dis >= skill_def->CastDistanceMin && dis <= skill_def->CastDistanceMax)
				{
					if (skill_def->Shape == 2 || skill_def->Shape == 3)
					{
						// 直线型技能
						if (disX == 0 || disY == 0 || disX == disY)
							bCanUseSkill = true;
					}
					else
					{
						bCanUseSkill = true;
					}
				}

				if (bCanUseSkill)
				{
					// 使用技能
					onAiAttack(ao, skill_def);
				}
				else
				{
					// 朝着目标移动
					if (!Walk(ao->X(),ao->Y(),moveDisEnd))
					{
						if (!Walk(ao->X(),ao->Y(),moveDisEnd - 1))
						{
							int dir = NextDir(mDir, -1);
							if (!Walk(NextX(X(), dir), NextY(Y(), dir), 0))
							{
								dir = NextDir(mDir, 1);
								Walk(NextX(X(), dir), NextY(Y(), dir), 0);
							}
						}
					}
					mAIState = AI_STATE_WALK;
				}
			}
		}
		else
		{
			if( mAutoFindNext )
			{
				FindNextTarget(0);
				if (Target())
				{
					return AI_STATE_WALK;
				}
			}
			
			if( ! ( (std::abs(mOwner->X()-X())) < 2 && (std::abs(mOwner->Y()-Y())) < 2 ) )
			{
				if (!Walk(mOwner->X(),mOwner->Y(),2))
				{
					int dir = NextDir(mDir, -1);
					if (!Walk(NextX(X(), dir), NextY(Y(), dir), 0))
					{
						dir = NextDir(mDir, 1);
						Walk(NextX(X(), dir), NextY(Y(), dir), 0);
					}
				}
				
				// printf("time = %.2f\n", mDelayTick);
				return AI_STATE_WALK;
			}
			else
			{
				IntPointClass np = mOwner->NextPosition(mOwner->Dir(),1,2);
				if( np.x == X() && np.y == Y() )
				{
					np = mOwner->NextPosition(mOwner->NextDir(mOwner->Dir(),4),1);
					Walk( FindNextDirection( X(),Y(),np.x,np.y ) );
					return AI_STATE_WALK;
				}
			}
			mAIState = AI_STATE_IDLE;
		}
		return mAIState;
	}

	int PlaySlave::AiIdleZS(double time)
	{
		if( ! mParentMap ) return AI_STATE_NULL;
		if( (std::abs(mOwner->X()-X()) + std::abs(mOwner->Y()-Y())) > 15 )
		{
			int x= mOwner->X();
			int y= mOwner->Y();		
			FindEmptyPosition(&x,&y);
			ForceMove(x,y,mOwner->Dir());
			return AI_STATE_IDLE;
		}
		if( Target())
		{
			if(Target()->TypeIsPlayer())
			{
				Walk(mOwner->X(),mOwner->Y(),mOwner->Dir());
				return  AI_STATE_IDLE;
			}
			PlayActiveObject* ao=Target();	
			int obj_num = FindMonInPosition(ao->X(),ao->Y());
			if (obj_num >= 2)
			{
				mSkill= SkillDef::SKILL_TYPE_BanYueWanDao;
			}
			if (obj_num <= 1)
			{
				mSkill= SkillDef::SKILL_TYPE_CiShaJianShu;
			}
			if (mLiehuoTick + 8<time)
			{
				mSkill= SkillDef::SKILL_TYPE_LieHuoJianFa;
				mLiehuoTick = time;
			}
			if ((mGongshaTick + 5<time) && mSkill != SkillDef::SKILL_TYPE_LieHuoJianFa)
			{
				mSkill= SkillDef::SKILL_TYPE_GongShaJianShu;
				mGongshaTick = time;
			}
			if( ao->IsDead() || ao->IsYingShen()  || !mAutoFindNext)
			{
				if(ao->ID()==mLocktarget){
					mLocktarget=0;
				}
				ClearTarget();
				mAIState = AI_STATE_IDLE;
				mWalkTick = time;
			}
			else if( (std::abs(ao->X()-X())) < 2 && (std::abs(ao->Y()-Y())) < 2 && (!(ao->X()==X()&&ao->Y()==Y())))
			{
				int dir = Diff2Dir((float)ao->X()-X(),(float)ao->Y()-Y());
				Turn(dir);
				int newx = NextX(X(),mDir);
				int newy = NextY(Y(),mDir);
				if( mSkill== SkillDef::SKILL_TYPE_BanYueWanDao )//需要配置
				{
					UseSkill(10, newx, newy, ao->ID(),11041);//onAiAttack(ao);
					int pawer = GetAttackPawerRange(Attributes()->mDC, Attributes()->mDCMax);
					int curDir = -4;
					for (int i = 0; i < 8; i++)
					{
						if (i != 4)
						{
							int dir = NextDir(mDir, curDir + i);
							int x = NextX(X(), dir);
							int y = NextY(Y(), dir);
							//AttackPosition(x, y, SkillDef::SKILL_TYPE_BanYueWanDao, pawer, false, -1);
						}
					}
				}
				else if( mSkill == SkillDef::SKILL_TYPE_CiShaJianShu )
				{
					UseSkill(10, newx, newy, ao->ID(), 11031, false);//onAiAttack(ao);
					int pawer = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);
					int x = NextX(NextX(X(),mDir),mDir);
					int y = NextY(NextY(Y(),mDir),mDir);
					//tackPosition(ao->X(),ao->Y(),SkillDef::SKILL_TYPE_YiBanGongJi,pawer,false,-1);
					//AttackPosition(x,y,SkillDef::SKILL_TYPE_YiBanGongJi,pawer,false,-1);
				}
				else if( mSkill == SkillDef::SKILL_TYPE_GongShaJianShu )
				{
					UseSkill(10, newx, newy, ao->ID(), 11021, false);//onAiAttack(ao);
					int pawer = GetAttackPawerRange(Attributes()->mDC*2,Attributes()->mDCMax*2);
					//AttackPosition(ao->X(),ao->Y(),SkillDef::SKILL_TYPE_GongShaJianShu,pawer,false,-1);
				}
				else if( mSkill == SkillDef::SKILL_TYPE_LieHuoJianFa )
				{
					UseSkill(10, newx, newy, ao->ID(), 11061,false);//onAiAttack(ao);
					int pawer = GetAttackPawerRange(Attributes()->mDC*5,Attributes()->mDCMax*5);
					//AttackPosition(ao->X(),ao->Y(),SkillDef::SKILL_TYPE_LieHuoJianFa,pawer,false,-1);
				}	
				mAIState = AI_STATE_ATTACK;
				mWalkTick = time;
			}
			else
			{
				
				if( ! Walk(ao->X(),ao->Y(),2) )
				{
					Walk(FindNextDirection(X(),Y(),ao->X(),ao->Y()));			
					mAIState = AI_STATE_WALK;
				}
				if (mWalkTick + 3<time)
				{
					FindNextTarget(0);
					return  AI_STATE_WALK;
				}
			}
		}
		else
		{
			if( mAutoFindNext )
			{
				FindNextTarget(0);
			}
			if( ! ( (std::abs(mOwner->X()-X())) < 2 && (std::abs(mOwner->Y()-Y())) < 2 ) )
			{
				Walk(mOwner->X(),mOwner->Y(),2);
				return AI_STATE_WALK;
			}
			else
			{
				IntPointClass np = mOwner->NextPosition(mOwner->Dir(),1,2);
				if( np.x == X() && np.y == Y() )
				{
					np = mOwner->NextPosition(mOwner->NextDir(mOwner->Dir(),4),1);
					Walk( FindNextDirection( X(),Y(),np.x,np.y ) );
					return AI_STATE_WALK;
				}
			}
			mAIState = AI_STATE_IDLE;
		}
		return mAIState;
	}
	int PlaySlave::AiIdleFS(double time)
	{
		if( ! mParentMap ) return AI_STATE_NULL;
		if( (std::abs(mOwner->X()-X()) + std::abs(mOwner->Y()-Y())) > 15 )
		{
			int x= mOwner->X();
			int y= mOwner->Y();		
			FindEmptyPosition(&x,&y);
			ForceMove(x,y,mOwner->Dir());
			return AI_STATE_IDLE;
		}

		if (!IsStatusMFD(STATUS_TYPE_MOFADUN))
		{
			UseSkill(17,X(),Y(),ID(),24121);
			//MoFaDun(15,3);
		}

		if( Target())
		{
			if(Target()->TypeIsPlayer())
			{
				Walk(mOwner->X(),mOwner->Y(),mOwner->Dir());
				return  AI_STATE_IDLE;
			}
			PlayActiveObject* ao=Target();
			int obj_num = FindMonInPosition(ao->X(),ao->Y());
			if (obj_num >= 2)
			{
				mSkill= SkillDef::SKILL_TYPE_BingPaoXiao;
			}
			else
			{
				mSkill= SkillDef::SKILL_TYPE_LeiDianShu;
			}
			if (mLiehuoTick + 3<time)
			{
				mSkill= SkillDef::SKILL_TYPE_LiuXingHuoYu;
				mLiehuoTick = time;
			}
			int mValor = 2;
			if( (std::abs(ao->X()-X())) < 3 && (std::abs(ao->Y()-Y())) < 3 )
			{
				if( rand() % mValor == 0 )
				{
					int dir = Diff2Dir((float)X()-ao->X(),(float)Y()-ao->Y());
					IntPointClass ipc = NextPosition(dir,3);
					dir = this->FindNextDirection(X(),Y(),ipc.x,ipc.y);
					Walk(dir);
					return AI_STATE_WALK;

				}
			}
			if( ao->IsDead() || ao->IsYingShen()  || !mAutoFindNext)
			{
				if(ao->ID()==mLocktarget){
					mLocktarget=0;
				}
				ClearTarget();
				mAIState = AI_STATE_IDLE;
				mWalkTick = time;
			}
			else if( mSkill== SkillDef::SKILL_TYPE_LeiDianShu && (std::abs(ao->X()-X())) <= 20 && (std::abs(ao->Y()-Y())) <= 20 )
			{
				UseSkill(17,ao->X(),ao->Y(),ao->ID(),24051,false);
				int pow = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);

				//AttackPosition(ao->X(),ao->Y(),SkillDef::SKILL_TYPE_LeiDianShu,pow,false,-1);
				mAIState = AI_STATE_ATTACK;
				mWalkTick = time;
			}
			else if( mSkill== SkillDef::SKILL_TYPE_LiuXingHuoYu && (std::abs(ao->X()-X())) <= 20 && (std::abs(ao->Y()-Y())) <= 20 )
			{
				UseSkill(17, ao->X(), ao->Y(), ao->ID(), 24161, false);
				int pow = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);
				for(int i = -4;i<5;i++)
				{
					for(int j = -4;j<5;j++)
					{
						int paw = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);
						//AttackPosition(ao->X()+i,ao->Y()+j,SkillDef::SKILL_TYPE_BingPaoXiao,paw,false,-1);
					}
				}
				mAIState = AI_STATE_ATTACK;
				mWalkTick = time;
			}
			else if( mSkill == SkillDef::SKILL_TYPE_BingPaoXiao && (std::abs(ao->X()-X())) <= 20 && (std::abs(ao->Y()-Y())) <= 20 )
			{
				int dir = Diff2Dir((float)ao->X()-X(),(float)ao->Y()-Y());
				UseSkill(17, ao->X(), ao->Y(), ao->ID(), 24141, false);

				Turn(dir);
				for(int i = -1;i<2;i++)
				{
					for(int j = -1;j<2;j++)
					{
						int paw = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);
						//AttackPosition(ao->X()+i,ao->Y()+j,SkillDef::SKILL_TYPE_BingPaoXiao,paw,false,-1);
					}
				}
				mAIState = AI_STATE_ATTACK;
				mWalkTick = time;
			}
			else
			{
				if (mWalkTick + 3<time)
				{
					FindNextTarget(0);
					return  AI_STATE_WALK;
				}
			}
		}
		else
		{
			if( mAutoFindNext )
			{
				FindNextTarget(0);
			}
			if( ! ( (std::abs(mOwner->X()-X())) < 2 && (std::abs(mOwner->Y()-Y())) < 2 ) )
			{
				Walk(mOwner->X(),mOwner->Y(),2);
				return AI_STATE_WALK;
			}
			else
			{
				IntPointClass np = mOwner->NextPosition(mOwner->Dir(),1,2);
				if( np.x == X() && np.y == Y() )
				{
					np = mOwner->NextPosition(mOwner->NextDir(mOwner->Dir(),4),1);
					Walk( FindNextDirection( X(),Y(),np.x,np.y ) );
					return AI_STATE_WALK;
				}
			}
			mAIState = AI_STATE_IDLE;
		}
		return mAIState;
	}
	int PlaySlave::AiIdleDS(double time)
	{
		if( ! mParentMap ) return AI_STATE_NULL;
		if( (std::abs(mOwner->X()-X()) + std::abs(mOwner->Y()-Y())) > 15 )
		{
			int x= mOwner->X();
			int y= mOwner->Y();		
			FindEmptyPosition(&x,&y);
			ForceMove(x,y,mOwner->Dir());
			return AI_STATE_IDLE;
		}
		if( Target())
		{
			if(Target()->TypeIsPlayer())
			{
				Walk(mOwner->X(),mOwner->Y(),mOwner->Dir());
				return  AI_STATE_IDLE;
			}
			PlayActiveObject* ao=Target();
			if (ao->IsStatus(STATUS_TYPE_POSION_HP)&&ao->IsStatus(STATUS_TYPE_POSION_ARMOR))
			{
				mSkill= SkillDef::SKILL_TYPE_LingHunHuoFu;
			}
			else
			{
				mSkill= SkillDef::SKILL_TYPE_ShiDuShu;
			}
			int mValor = 2;
			if( (std::abs(ao->X()-X())) < 3 && (std::abs(ao->Y()-Y())) < 3 )
			{
				if( rand() % mValor == 0 )
				{
					int dir = Diff2Dir((float)X()-ao->X(),(float)Y()-ao->Y());
					IntPointClass ipc = NextPosition(dir,3);
					dir = this->FindNextDirection(X(),Y(),ipc.x,ipc.y);
					Walk(dir);
					return AI_STATE_WALK;

				}
			}
			if( ao->IsDead() || ao->IsYingShen()  || !mAutoFindNext)
			{
				if(ao->ID()==mLocktarget){
					mLocktarget=0;
				}
				ClearTarget();
				mAIState = AI_STATE_IDLE;
				mWalkTick = time;
			}
			else if( mSkill == SkillDef::SKILL_TYPE_LingHunHuoFu && (std::abs(ao->X()-X())) <= 20 && (std::abs(ao->Y()-Y())) <= 20 )
			{
				UseSkill(15,ao->X(),ao->Y(),ao->ID(),35041,false);
				int pow = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);
				//AttackPosition(ao->X(),ao->Y(),SkillDef::SKILL_TYPE_LingHunHuoFu,pow,false,-1);
				mAIState = AI_STATE_ATTACK;
				mWalkTick = time;
			}
			else if( mSkill == SkillDef::SKILL_TYPE_ShiDuShu && (std::abs(ao->X()-X())) <= 20 && (std::abs(ao->Y()-Y())) <= 20 )
			{		
//				Attack(ao,SkillDef::SKILL_TYPE_ShiDuShu,0,0,0);

				UseSkill(17, ao->X(), ao->Y(), ao->ID(), 35031, false);

				ao->AddStatus(STATUS_TYPE_POSION_HP,60 * 1000,500,2.5,1);
				ao->AddStatus(STATUS_TYPE_POSION_ARMOR,60 * 1000,500,2.5,1);
				ao->SetPosioner(ID());
				ao->SetPosionerName(Name());

				mAIState = AI_STATE_ATTACK;
				mWalkTick = time;
			}
			else
			{
				if (mWalkTick + 3<time)
				{
					FindNextTarget(0);
					return  AI_STATE_WALK;
				}
			}
		}
		else
		{
			if( mAutoFindNext )
			{
				FindNextTarget(0);
			}
			if( ! ( (std::abs(mOwner->X()-X())) < 2 && (std::abs(mOwner->Y()-Y())) < 2 ) )
			{
				Walk(mOwner->X(),mOwner->Y(),2);
				return AI_STATE_WALK;
			}
			else
			{
				IntPointClass np = mOwner->NextPosition(mOwner->Dir(),1,2);
				if( np.x == X() && np.y == Y() )
				{
					np = mOwner->NextPosition(mOwner->NextDir(mOwner->Dir(),4),1);
					Walk( FindNextDirection( X(),Y(),np.x,np.y ) );
					return AI_STATE_WALK;
				}
			}
			mAIState = AI_STATE_IDLE;
		}
		return mAIState;
	}

	void PlaySlave::AiOther(double time)
	{
		mAIState = AI_STATE_IDLE;
	}

	int PlaySlave::AiIdleSZFS(double time)
	{
		if( Target())
		{
			PlayActiveObject* ao=Target();
			mSkill= SkillDef::SKILL_TYPE_HuoQiang;
		
			if( ao->IsDead() || ao->IsYingShen()  || !mAutoFindNext)
			{
				if(ao->ID()==mLocktarget)
				{
					mLocktarget=0;
				}
				ClearTarget();
				mAIState = AI_STATE_IDLE;
			}
			else if( mSkill== SkillDef::SKILL_TYPE_HuoQiang && (std::abs(ao->X()-X())) <= 10 && (std::abs(ao->Y()-Y())) <= 10 )
			{
				UseSkill(SkillDef::SKILL_TYPE_HuoQiang,ao->X(),ao->Y(),ao->ID(), 24161 ,false);
				int pow = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);

				mAIState = AI_STATE_ATTACK;
				mWalkTick = time;
			}
			else
			{
				if (mWalkTick + 3 < time)
				{
					FindNextTarget(0);
					return AI_STATE_WALK;
				}
			}
		}
		else
		{
			if( mAutoFindNext )
			{
				FindNextTarget(0);
			}

			if( ! ( (std::abs(this->mStartPosX-X())) < mMoveRange && (std::abs(this->mStartPosY-Y())) < mMoveRange ) )
			{
				Walk(mStartPosX, mStartPosY,2);
				return AI_STATE_WALK;
			}
			mAIState = AI_STATE_IDLE;
		}
		return mAIState;
	}

	bool PlaySlave::IsProperAttackDes(PlayActiveObject* des, SkillDef *sd)
	{
		if( ! PlayActiveObject::IsProperAttackDes(des,sd) )
		{
			return false;
		}
		if( mOwner && static_cast<PlayActiveObject*>(mOwner) == des )
		{
			return false;
		}
		if( des->TypeIsMonster() )
		{
			PlayMonster* mon = static_cast<PlayMonster*>(des);
			if( mon->CollectTime() > 0 )
			{
				return false;
			}
		}

		if (GetSlaveType() == Slave_type_huwei)
		{
			if ( des->TypeIsPlayer() || des->TypeIsSlave())
			{
				return false;
			}
		}

		if (des->TypeIsSlave())
		{
			PlaySlave * slave = static_cast<PlaySlave *>(des);
			if (slave->GetSlaveType() == Slave_type_huwei)
			{
				return false;
			}
		}

		if( mOwner )
		{
			if( (std::abs(mOwner->X()-des->X()) + std::abs(mOwner->Y()-des->Y())) > 15 )
			{
				return false;
			}
			if( ! mOwner->IsProperAttackDes(des, sd) )
			{
				return false;
			}
		}
		return true;
	}

	int PlaySlave::evaluateWalkDirection(int fromX,int fromY,int dir,int toX,int toY)
	{
		int nx = NextX(fromX,dir);
		int ny = NextY(fromY,dir);
		if( mParentMap && mParentMap->GetBlockData(nx,ny,mBlockCheckType) > 0 )
		{
			return 5000000;
		}
		return abs(nx-toX) + abs(ny-toY);
	}

// 	const char * PlaySlave::Name()
// 	{
// 		return NULL;
// 	}

	int PlaySlave::Attacked(PlayActiveObject* src,SkillDef * sd,int pawer,int flags,int delay)
	{
		if (GetSlaveType() == Slave_type_huwei)
		{
			if (!src->TypeIsMonster())
				return 0;
		}
	
		return PlayAIBase::Attacked(src, sd, pawer, flags, delay);
	}

	void PlaySlave::SetHateObjID(int priority, int id)
	{
		if (priority < 0 || priority >= HATE_LIST_MAX_PRIORITY)
			return;
		m_nHateIDList[priority] = id;
	}

	int PlaySlave::GetSlaveType()
	{
		if (mAiType == 0)
		{
			return Slave_type_dog;
		}

		if (mAiType == 4)
		{
			return Slave_type_huwei;
		}

		return Slave_type_other;
	}

	void PlaySlave::SetHateObj(PlayActiveObject * src, bool act, int damage)
	{
		int priority = HATE_LIST_MAX_PRIORITY;
		// 主动攻击
		if (act == true)
		{
			if (src->TypeIsPlayer())
			{
				priority = 0;
			}
			else if (src->TypeIsSlave())
			{
				priority = 2;
			}
			else if (src->TypeIsMonster())
			{
				PlayMonster * pMon = static_cast<PlayMonster *>(src);
				if (pMon->IsBoss())
					priority = 3;
				else 
					priority = 6;
			}
		}
		else
		{
			// 被攻击
			if (damage > 0)
			{
				if (src->TypeIsPlayer())
					priority = 1;
				else if (src->TypeIsSlave())
					priority = 4;
				else if (src->TypeIsMonster())
				{
					PlayMonster * pMon = static_cast<PlayMonster *>(src);
					if (pMon->IsBoss())
						priority = 5;
					else
						priority = 7;
				}
			}
			else
			{
				if (src->TypeIsPlayer())
					priority = 8;
				else if (src->TypeIsSlave())
					priority = 9;
				else if (src->TypeIsMonster())
				{
					PlayMonster * pMon = static_cast<PlayMonster *>(src);
					if (pMon->IsBoss())
						priority = 10;
					else
						priority = 11;
				}
			}
		}

		if (priority >= HATE_LIST_MAX_PRIORITY)
			return;
		this->SetHateObjID(priority, src->ID());
	}

	void PlaySlave::FindHateListTarget()
	{
		for (size_t i = 0; i < HATE_LIST_MAX_PRIORITY; ++ i)
		{
			if (m_nHateIDList[i] != 0)
			{
				PlayActiveObject * obj = FindActiveInView(m_nHateIDList[i]);
				if (!obj || obj->IsDead() || obj->IsYingShen() || !IsProperAttackDes(obj, NULL))
				{
					continue;
				}
				double now = mGSL->GetTimerFix()->Elapsed();

				if (Target() == 0 || now - m_dLockTargetTime > LOCK_TARGET_TIME)
				{
					Target(obj);
					m_dLockTargetTime = now;
					break;
				}
			}
		}
	}

	LEUD_INT64 PlaySlave::AddExperience(int ex,int src)
	{
		if (src != 100)
			return mExp;

		if (mLevelUpExp <= 0)
			return 0;

		mExp += ex;
		CheckLevelUp();
		return mExp;
	}

	int PlaySlave::CheckLevelUp()
	{
		if (mExp > mLevelUpExp)
		{
			mExp -= mLevelUpExp;
			mLevel ++;
			if (GetSlaveType() == Slave_type_dog)
			{
				int def = mDefId + 1;
				MonDef * md = mGSL->GetMonDefManager()->FindMonDef(def);
				if (md)
				{
					mGSL->GetMapManager()->LoadAIObject(md, this, "");
					this->Name(md->name);

 					// 调整狗属性
					ChangeAttrByOwnerAttr(md);

					for( DomainPlayVector::iterator pos = mPlayerList.begin();
						pos != mPlayerList.end();)
					{
						if ((*pos))
						{
							(*pos)->NotifyAvatarChange(this);
							++pos;
						}
						else
						{
							printf("PlayAIBase onAiAttack *pos=0\n");
							pos = mPlayerList.erase(pos);
							continue;
						}
					}
				}
			}
		}
		
		return 0;
	}

	void PlaySlave::ChangeAttrByOwnerAttr(MonDef * md)
	{
		if (GetSlaveType() != Slave_type_dog)
			return;

		if (!md || !mOwner)
			return;
		
		mBaseAttribute.mAC = md->ac * 1.0f / 10000 * mOwner->Attributes()->mAC + md->acMax;
		mBaseAttribute.mACMax = md->ac * 1.0f / 10000 * mOwner->Attributes()->mACMax + md->acMax;
		mBaseAttribute.mMAC = md->mac * 1.0f / 10000 * mOwner->Attributes()->mMAC + md->macMax;
		mBaseAttribute.mMACMax = md->mac * 1.0f / 10000 * mOwner->Attributes()->mMAC + md->macMax;
		mBaseAttribute.mMC = md->mc * 1.0f / 10000 * mOwner->Attributes()->mSC + md->mcMax;
		mBaseAttribute.mMCMax = md->mc * 1.0f / 10000 * mOwner->Attributes()->mSCMax + md->mcMax;
	}

	void PlaySlave::NotifyBye(PlayMap* map,PlayMapObject* src)
	{
		for (size_t i = 0; i < HATE_LIST_MAX_PRIORITY; ++ i)
		{
			if (src->ID() == m_nHateIDList[i])
			{
				m_nHateIDList[i] = 0;
				break;
			}
		}
		PlayAIObject::NotifyBye(map, src);
	}

	void PlaySlave::NotifyHPMPChange(PlayActiveObject* src,int hp_change,int mp_change)
	{
		for( DomainPlayVector::iterator pos = mPlayerList.begin();
			pos != mPlayerList.end();)
		{
			if ((*pos))
			{
				(*pos)->NotifyHPMPChange(this,0,0);
				++pos;
			}
			else
			{
				printf("PlayAIBase onAiAttack *pos=0\n");
				pos = mPlayerList.erase(pos);
				continue;
			}
		}
	}

	PlaySalveFashi::PlaySalveFashi(void)
	{
		m_bNoMove = false;
		m_bNoFire = false;
	}

	PlaySalveFashi::~PlaySalveFashi(void)
	{

	}

	bool PlaySalveFashi::IsProperAttackDes(PlayActiveObject* des,  SkillDef* sd)
	{
		if (des->TypeIsMonster())
		{
			return true;
		}
		return false;
	}

	int PlaySalveFashi::AiIdle()
	{
		if( ! mParentMap ) return AI_STATE_NULL;

		if( Target() )
		{
			PlayActiveObject* ao = Target();

			if( ao->IsDead() || ao->IsYingShen())
			{
				ClearTarget();
				mAIState = AI_STATE_IDLE;
			}
			else
			{
				bool atk = false;
				if (mNewTarget)
				{
					// 新目标,释放火墙
					SkillDef * sd = mGSL->GetSkillDefineManager()->GetSkillDefineById(70002);
					int disX = std::abs(ao->X()-X());
					int disY = std::abs(ao->Y()-Y());

					int dis = disX > disY ? disX : disY;
					if (sd != NULL 
						&& dis >= sd->CastDistanceMin && dis <= sd->CastDistanceMax)
					{
						if (!m_bNoFire)
						{
							onAiAttack(ao, sd);
						}
						mNewTarget = false;
						atk = true;
					}
				}

				int skill = ChooseSkill(ao);
				SkillDef * skill_def = mGSL->GetSkillDefineManager()->GetSkillDefineById(skill);
				int disX = std::abs(ao->X()-X());
				int disY = std::abs(ao->Y()-Y());
				int dis = disX > disY ? disX : disY;

				if (skill_def != NULL 
					&& dis >= skill_def->CastDistanceMin && dis <= skill_def->CastDistanceMax)
				{
					// 使用技能
					onAiAttack(ao, skill_def);
					atk = true;
				}

				if (!atk)
					ClearTarget();
			}
		}
		else
		{
			if( mAutoFindNext )
			{
				FindNextTarget(0);
			}
			if (!m_bNoMove)
			{
				RandGo();
			}
		}
		return mAIState;
	}

	//雕像slave

	PlaySlaveStatue::PlaySlaveStatue(void)
	{

	}

	PlaySlaveStatue::~PlaySlaveStatue(void)
	{

	}

	bool PlaySlaveStatue::IsProperAttackDes(PlayActiveObject* des,  SkillDef* sd)
	{
		return false;
	}

	int PlaySlaveStatue::AiIdle()
	{
		if( ! mParentMap ) return AI_STATE_NULL;

		return mAIState;
	}
}
