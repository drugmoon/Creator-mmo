#include "StdAfx.h"
#include "PlayAIObject.h"
#include "ManagerMonster.h"
#include "PlayMap.h"
#include "DomainPlay.h"
#include "GameServerlib.h"
#include "ManagerSkillDef.h"
#include "SubSkill.h"
#include "PlayMonsterCaller.h"
#include "ManagerMap.h"
#include "PlayNeutral.h"

namespace OGSLib
{
	PlayAIObject::PlayAIObject(void)
	{
		mManager = 0;
		mTarget = 0;
		mAttackGap = 0;
		mWalkGap = 0;
		mWalkStep = 1;
		mDieClear = 0;
		mRecoveHP = 1;
		mBlockCheckType=1;
		mAIState = AI_STATE_NULL;
		mPlayerList.reserve(64);
		mAntiCisha = 0;
		mUpdateNode.This = this;

		mRecoverTick = 0;
		mRecoverGap = 1;
		mUsualRecoveHP = 0;
		mBattleRecoverHp = 0;
		mCoolEye = 0;
		mNoHit = 0;
	}

	PlayAIObject::~PlayAIObject(void)
	{
	}
	void PlayAIObject::ResetUpdate(int next)
	{
		/*if (TypeIsNeutral())
		{
			PlayNeutral* pNeutral = (PlayNeutral *)(this);
			if (pNeutral->GetMonsterSeedId() == 0)
			{
				mManager->AddMonsterNode(next,&mUpdateNode);
			}
		}
		else*/
			mManager->AddMonsterNode( next,&mUpdateNode );
	}
	int  PlayAIObject::DefenseAttackPawer(PlayActiveObject* src,int pawer,int skill_type,int flags)
	{
		if( rand() % 1000 < mAntiCisha )
		{
			if( skill_type == SkillDef::SKILL_TYPE_CiShaJianShu || skill_type == SkillDef::SKILL_TYPE_BanYueWanDao )
			{
				skill_type = SkillDef::SKILL_TYPE_YiBanGongJi;
			}
		}
		return max(mGSL->mMonMiniDamage,PlayActiveObject::DefenseAttackPawer(src,pawer,skill_type,flags));
	}
	void PlayAIObject::SetManager(ManagerMonster* manager,int rightnow)
	{
		mManager = manager;
		if (TypeIsDart())
		{
			mManager->AddDart(this);
		}
		else if (TypeIsNeutral())
		{
			PlayNeutral* pNeutral = (PlayNeutral *)(this);
			if (pNeutral->GetMonsterSeedId() == 0)
			{
				mManager->AddMonster(this,rightnow);
			}
		}
		else
			mManager->AddMonster(this,rightnow);
	}
	void PlayAIObject::NotifyMeet(PlayMap* map,PlayMapObject* src)
	{
		mAIState = AI_STATE_IDLE;
		PlayActiveObject::NotifyMeet(map,src);
		if( src->TypeIsPlayer() )
		{
			mPlayerList.push_back((DomainPlay*)src);
		}
	}
	void PlayAIObject::NotifyBye(PlayMap* map,PlayMapObject* src)
	{
		PlayActiveObject::NotifyBye(map,src);
		if( src->TypeIsPlayer() )
		{
			mPlayerList.erase(std::remove(mPlayerList.begin(),mPlayerList.end(),(DomainPlay*)src),mPlayerList.end());
		}
		if( src->IsInteractive() && mTarget == (PlayActiveObject*)src )
		{
			mTarget = 0;
		}
		if( mTarget )
		{
			if( mVisibleInteractiveObjectList.end() == std::find(mVisibleInteractiveObjectList.begin(),mVisibleInteractiveObjectList.end(),mTarget) )
			{
				mTarget = 0;
			}
		}
	}
	void PlayAIObject::Target(PlayActiveObject* target)
	{
		if (TypeIsMonster())
		{
			PlayMonster* pMonster = dynamic_cast<PlayMonster*>(this);
			if (pMonster && pMonster->IsLockTarget())
			{
				return;
			}
		}
		
		if( target )
		{
			if( mVisibleInteractiveObjectList.end() != std::find(mVisibleInteractiveObjectList.begin(),mVisibleInteractiveObjectList.end(),target) )
			{
				mTarget = target; 
				if(mManager && (mAIState == AI_STATE_IDLE || mAIState == AI_STATE_NULL)) 
				{
					if (TypeIsNeutral())
					{
						PlayNeutral* pNeutral = (PlayNeutral *)(this);
						if (pNeutral->GetMonsterSeedId() == 0)
						{
							mManager->AddMonsterNode(1,&mUpdateNode);
						}
					}
					else
						mManager->AddMonsterNode(1,&mUpdateNode);
				}
			}
		}
		else
		{
			mTarget = 0;
		}
	}
	void PlayAIObject::NotifyMapLeave(PlayMap* map)
	{
		mTarget = 0;
		PlayActiveObject::NotifyMapLeave(map);
	}
	int PlayAIObject::FindNextDirection(int fromx,int fromy,int toX,int toY)
	{
		int evaluate=0;
		int evaluateLast = 50000;
		int dir = 0;
		for (int i = 0; i < DIR_COUNT; i++) 
		{
			evaluate = evaluateWalkDirection(fromx,fromy,i,toX,toY);
			if ( evaluate < evaluateLast )
			{
				evaluateLast = evaluate;
				dir = i;
			}
		}
		return dir;
	}
	int PlayAIObject::evaluateWalkDirection(int fromX,int fromY,int dir,int toX,int toY)
	{
		int nx = NextX(fromX,dir);
		int ny = NextY(fromY,dir);
		if( mParentMap && mParentMap->GetBlockData(nx,ny,mBlockCheckType) > 0 )
		{
			return 5000000;
		}
		return abs(nx-toX) + abs(ny-toY);
	}
	void PlayAIObject::LoadScript(ScriptLua* script,const char* package)
	{
		mScript = script;
		mScriptPackage = package;
	}
	void PlayAIObject::OnHurt(PlayActiveObject* src,int change,int delay,int change_power)
	{
		int attacker=0;
		if( src )
		{
			attacker=src->ID();
		}
		if (this->TypeIsMonster())
		{
			PlayMonster * mon = dynamic_cast<PlayMonster *>(this);
			if (mon->IsElite() && strcmp(mon->mParentMap->GetID().c_str(), "nvshen") == 0)
			{
				delay = 0;
			}
		}
		for( DomainPlayVector::iterator pos = mPlayerList.begin();
			pos != mPlayerList.end();)
		{
			if ((*pos))
			{
				(*pos)->NotifyInjury(this, attacker, change, delay, mAttackEffectFlags, change_power);
				++pos;
			}
			else
			{
				printf("PlayAIObject OnHurt *pos=0\n");
				pos = mPlayerList.erase(pos);
				continue;
			}
		}
		mAttackEffectFlags = 0x0;
	}
	void PlayAIObject::OnDead(PlayActiveObject* src,int delay)
	{
		PlayActiveObject::OnDead(src, delay);
		for( DomainPlayVector::iterator pos = mPlayerList.begin();
			pos != mPlayerList.end();)
		{
			if ((*pos))
			{
				(*pos)->NotifyDie(this, delay);
				++pos;
			}
			else
			{
				printf("PlayAIObject OnDead *pos=0\n");
				pos = mPlayerList.erase(pos);
				continue;
			}
		}
	}
	bool PlayAIObject::Walk(int x,int y,int endrange)
	{
		if( IsMabi() )
		{
			return false;
		}
		int newx = X();
		int newy = Y();
		for(int i = 0 ;i < mWalkStep;i ++ )
		{
			if( (std::abs(x-newx)) < endrange && (std::abs(y-newy)) < endrange ) break;
			int dir = FindNextDirection(newx,newy,x,y);
			if( dir < 0 )break;
			newx = NextX(newx,dir);
			newy = NextY(newy,dir);
		}

		int run_flag = 1;
		if(TypeIsDart())
			run_flag=2;
		if (TypeIsSlave())
			run_flag=3;

		if( mParentMap && mParentMap->GetBlockData(newx,newy,run_flag) <= 0 )
		{
			if( newx != X() || newy != Y() )
			{
				int dir = Diff2Dir((float)newx-X(),(float)newy-Y());
				mDir = dir;
				SetPosition(newx,newy);

				if (TypeIsSlave())
				{
					for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
						pos != mVisibleInteractiveObjectList.end();)
					{
						if ((*pos))
						{
							(*pos)->NotifyWalk(this, mDir, X(), Y());
							++pos;
						}
						else
						{
							printf("walk *pos=0\n");
							pos = mVisibleInteractiveObjectList.erase(pos);
							continue;
						}
					}
				}
				else
				{
					for( DomainPlayVector::iterator pos = mPlayerList.begin();
						pos != mPlayerList.end();)
					{
						if ((*pos))
						{
							(*pos)->NotifyWalk(this, mDir, X(), Y());
							++pos;
						}
						else
						{
							printf("PlayAIObject Walk *pos=0\n");
							pos = mPlayerList.erase(pos);
							continue;
						}
					}
				}

				mAIState = AI_STATE_WALK;
				return true;
			}
		}
		return false;
	}
	void PlayAIObject::Walk(int dir)
	{
		if( IsMabi() )
		{
			return;
		}

		int newx = X();
		int newy = Y();

		for(int i = 0 ;i < mWalkStep;i ++ )
		{
			newx = NextX(newx,dir);
			newy = NextY(newy,dir);
		}

		mDir = dir;
		int run_flag = 1;
		if(TypeIsSlave())
		{
			run_flag=3;
		}
		if (TypeIsDart())
		{
			run_flag = 2;
		}
		if( mParentMap && mParentMap->GetBlockDataMon(newx,newy,run_flag) <= 0 )
		{
			SetPosition(newx,newy);
		}
		if( newx == X() && newy == Y() )
		{
			for( DomainPlayVector::iterator pos = mPlayerList.begin();
				pos != mPlayerList.end();)
			{
				if ((*pos))
				{
					(*pos)->NotifyWalk(this, mDir, X(), Y());
					++pos;
				}
				else
				{
					printf("PlayAIObject Walk2 *pos=0\n");
					pos = mPlayerList.erase(pos);
					continue;
				}
			}

			if (TypeIsSlave())
			{
				for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
					pos != mVisibleInteractiveObjectList.end();)
				{
					if ((*pos))
					{
						(*pos)->NotifyWalk(this, mDir, X(), Y());
						++pos;
					}
					else
					{
						printf("walk *pos=0\n");
						pos = mVisibleInteractiveObjectList.erase(pos);
						continue;
					}
				}
			}
			else
			{
				for( DomainPlayVector::iterator pos = mPlayerList.begin();
					pos != mPlayerList.end();)
				{
					if ((*pos))
					{
						(*pos)->NotifyWalk(this, mDir, X(), Y());
						++pos;
					}
					else
					{
						printf("PlayAIObject Walk *pos=0\n");
						pos = mPlayerList.erase(pos);
						continue;
					}
				}
			}
		}
		mAIState = AI_STATE_WALK;
	}
	int PlayAIObject::Turn(int dir)
	{
		mDir = dir;
		for( DomainPlayVector::iterator pos = mPlayerList.begin();
			pos != mPlayerList.end();)
		{
			if ((*pos))
			{
				(*pos)->NotifyTurn(this, mDir);
				++pos;
			}
			else
			{
				printf("PlayAIObject Turn *pos=0\n");
				pos = mPlayerList.erase(pos);
				continue;
			}
		}
		return mDir;
	}
	void PlayAIObject::ForceMove(int x,int y,int dir)
	{
		PlayActiveObject::ForceMove(x,y,dir);
		if( x == X() && y == Y() )
		{
			for( DomainPlayVector::iterator pos = mPlayerList.begin();
				pos != mPlayerList.end();)
			{
				if ((*pos))
				{
					(*pos)->NotifyForceMove(this, X(), Y(), mDir);
					++pos;
				}
				else
				{
					printf("PlayAIObject ForceMove *pos=0\n");
					pos = mPlayerList.erase(pos);
					continue;
				}
			}
		}
	}
	void PlayAIObject::UseSkill(int skill_type,int x,int y,int id,int resid,bool show)
	{
 		mAIState = AI_STATE_ATTACK;
	}
	void PlayAIObject::ResetAttribute()
	{
		CalculateAttribute();
		mHp = mActiveAttribute.mMaxHp;
		mMp = mActiveAttribute.mMaxMp;
	}
	void PlayAIObject::ParseOpt(const char* flags)
	{
		char flag[1024];
		int flag_len=0;
		memset(flag,0,1024);
		flag_len=0;
		for(size_t i=0;i<=strlen(flags);i++)
		{
			if( flags[i] == ';' || flags[i] == 0 )
			{
				if( strlen(flag) > 0 )
				{
					DoOpt(flag);
				}
				memset(flag,0,1024);
				flag_len=0;
				continue;
			}
			else
			{
				flag[flag_len++] = flags[i];
			}
		}
	}
	int PlayAIObject::AiIdle()
	{
		return mAIState;
	}
	void PlayAIObject::AiAttack()
	{
		if( m_AttackTimer.Elapsed() > mAttackGap )
		{
			mAIState = AI_STATE_IDLE;
		}
	}
	void PlayAIObject::AiWalk()
	{
		if( m_WalkTimer.Elapsed() > mWalkGap )
		{
			mAIState = AI_STATE_IDLE;
		}
	}
	void PlayAIObject::AiOther(double time)
	{
	}
	int PlayAIObject::Update(double time)
	{
		PERF_NODE_FUNC();
		//printf("PlayAIObject Update :%d, %f\n, %d",ID(),time, mAIState);
		//if( ! mParentMap ){return;}
		if( ! IsDead() )
		{
			PERF_NODE("UpdateBase");
			if( mRecoveHP )
			{
				UpdateHPMP(time);
			}
			UpdateStatusTimer(time);
			UpdateBuffTimer(time);
		}
		mAIState = AiIdle();
		//回血buff 怪物,宝宝，神将才有
		if ((TypeIsMonster() || TypeIsSlave()))
		{
			if (Hp() < MaxHp())
			{
				if (mTarget)
				{
					//战斗回血buff
					if (IsStatus(STATUS_TYPE_USUAL_RECOVER))
					{
						ClearStatus(STATUS_TYPE_USUAL_RECOVER);
					}
					if (mBattleRecoverHp && !IsStatus(STATUS_TYPE_BATTLE_RECOVER))
					{
						AddStatus(STATUS_TYPE_BATTLE_RECOVER, 60 * 1000, mBattleRecoverHp, 1, 1);
					}
				}
				else 
				{
					//平时回血buff
					if (IsStatus(STATUS_TYPE_BATTLE_RECOVER))
					{
						ClearStatus(STATUS_TYPE_BATTLE_RECOVER);
					}
					if (mUsualRecoveHP && !IsStatus(STATUS_TYPE_USUAL_RECOVER))
					{
						AddStatus(STATUS_TYPE_USUAL_RECOVER, 60 * 1000, mUsualRecoveHP, 1, 1);
					}
				}
			}
			else
			{
				if (IsStatus(STATUS_TYPE_BATTLE_RECOVER))
				{
					ClearStatus(STATUS_TYPE_BATTLE_RECOVER);
				}
				if (IsStatus(STATUS_TYPE_USUAL_RECOVER))
				{
					ClearStatus(STATUS_TYPE_USUAL_RECOVER);
				}
			}
			
		}

		switch(mAIState)
		{
		case AI_STATE_IDLE:return 0;
		case AI_STATE_ATTACK:return 18 + min(100,max(0,static_cast<int>(mAttackGap)/70));
		case AI_STATE_ATTACK2:return 20;
		case AI_STATE_WALK:return 12 + min(100,max(0,static_cast<int>(mWalkGap)/40));
		case AI_STATE_RELIVE:return 25;
		case AI_STATE_NULL:return -1;
		default:
			{
				PERF_NODE("AiOther");
				AiOther(time);
				return 1;
			}
			break;
			//case AI_STATE_IDLE2:return 0;
			//case AI_STATE_WALK2:return 12 + min(3,(rand()%mWalkGap)/40);
			//case AI_STATE_ATTACK2:return 18 + min(3,(rand()%mAttackGap)/40);
			//case AI_STATE_SLEEP2:return -1;
			//case AI_STATE_RELIVE:return 25;
			//case AI_STATE_NEXT:return 1;
		}
		return 0;
	}
	void PlayAIObject::ActionOfAOE0(SkillDef* sd,int paramX,int paramY,unsigned int* paramID)
	{
		if (sd->AOEAction)
		{
			for( DomainPlayVector::iterator pos2 = mPlayerList.begin();
				pos2 != mPlayerList.end();)
			{
				if ((*pos2))
				{
					(*pos2)->NotifyUseSkill(this, 10, X(), Y(), ID(), 0, this->Dir());
					++pos2;
				}
				else
				{
					printf("PlayAIBase onAiAttack *pos=0\n");
					pos2 = mPlayerList.erase(pos2);
					continue;
				}
			}
		}

		InteractiveObjectVector v = mVisibleInteractiveObjectList;
		for( InteractiveObjectVector::iterator pos = v.begin();
			pos != v.end();
			++ pos )
		{
			if((*pos))
			{
				int paw = GetUseSkillPower(sd);

				Attack(*pos, sd, paw, 0, sd->Attack_Delay);

				for( DomainPlayVector::iterator pos2 = mPlayerList.begin();
					pos2 != mPlayerList.end();)
				{
					if ((*pos2))
					{
						(*pos2)->NotifyPlayEffect(*pos, sd->EffectType, (*pos)->X(), (*pos)->Y(), 0,  (*pos)->ID(), sd->Effect_ResID);
						//(*pos2)->NotifyUseSkill(this, sd->EffectType, (*pos)->X(), (*pos)->Y(), (*pos)->ID(), (*pos)->ID(), this->Dir());
						++pos2;
					}
					else
					{
						printf("PlayAIBase onAiAttack *pos=0\n");
						pos2 = mPlayerList.erase(pos2);
						continue;
					}
				}
			}
		}
	}

	bool PlayAIObject::IsNoHit()
	{
		return mNoHit;
	}

	//////////////////////////////////////////////////////////////////////////
	PlayAIBase::PlayAIBase(void)
	{
		mAttackRange=2;
		mChooseAction=-1;
		mActionTick=0;
		//mActionTickMax=0;
		mEscapeHP=0;
		mEscapeRange=0;
		//mNomove=0;
		mNoDefaultAttack = 0;
		mPatrolRange = 0;
		mReliveX = 0;
		mReliveY = 0;
	}

	PlayAIBase::~PlayAIBase(void)
	{
	}
	void PlayAIBase::DoOpt(const char* opt)
	{
		if( strcmp(opt,"norecove") == 0 ) { mRecoveHP=1;}
		//if( strcmp(opt,"nomove") == 0 ){mNomove = 1;}
		if( strcmp(opt,"nodefaultattack") == 0 ){mNoDefaultAttack=1;}
		int value;int value2;
		if( sscanf(opt,"blocktype(%d)",&value) ==1 ){
			if(value==0||value==1||value==2){
				mBlockCheckType=value;
			}
		}
		Skill s;SkillNode sn;
		s.maxtick=0;s.escape = 0;sn.tick = 0;
		if( sscanf_s(opt,"sl(%d:%d:%d)",&s.prob,&sn.skill_type,&sn.skill_level) == 3 ){s.hppres = 100;s.skills.push_back(sn);mSkills.push_back(s);}
		if( sscanf_s(opt,"slh(%d:%d:%d:%d)",&s.hppres,&s.prob,&sn.skill_type,&sn.skill_level) == 4 ){s.skills.push_back(sn);mSkills.push_back(s);}
		if( sscanf_s(opt,"ar(%d)",&value) == 1 ){mAttackRange = value;}
		char temp[1024];
		if( sscanf_s(opt,"ls(%d:%d:%d:%[^)])",&s.prob,&s.hppres,&s.maxtick,temp,_countof(temp)) == 4 )
		{
			char t[1024];
			for(int d = 0 ;d < 20;d++ )
			{
				if( sscanf_s(temp,"%d:%d:%d>%s",&sn.skill_type,&sn.skill_level,&sn.tick,t,_countof(t)) == 4 )
				{
					s.skills.push_back(sn);
					strncpy_s(temp,t,1024);
				}
				else
				{
					break;
				}
			}
			if( ! s.skills.empty() )
			{
				mSkills.push_back(s);
			}
		}
	}

	int PlayAIBase::onAiAttack(PlayActiveObject* target, SkillDef* sd)
	{
		if (sd == NULL)
		{
			return AI_STATE_ATTACK;
		}

		if (IsMabi())
		{
			return AI_STATE_ATTACK;
		}

 		int paramX = target->X();
		int paramY = target->Y();
		unsigned int paramID = target->ID();
		bool bNotify = true;

		bool bCanTurn = true;

		if (TypeIsMonster())
		{
			PlayMonster * mon = static_cast<PlayMonster *>(this);
			if (mon->IsDirFixed())
			{
				bCanTurn = false;
			}
		}

		if (bCanTurn)
		{
			int dir = Diff2Dir((float)paramX - X(), (float)paramY - Y());
			Turn(dir);
		}

		if( sd )
		{
			switch (sd->Shape)
			{
			case 0: //普通攻击
				ActionYiBanGongJiBegin(sd,paramX,paramY,paramID);
				break;
			case 1: //射箭(单体远程技能)
				ActionHuoQiuShuBegin(sd,paramX,paramY,paramID, false);
				break;
			case 2: //刺杀(由于刺杀第二格有特殊效果,单独列出来)
				ActionCiShaJianShuBegin(sd, paramX, paramY, paramID);
				break;
			case 3: //直线型攻击(逐日)
				ActionLieHuoJianFaBegin(sd, paramX, paramY, paramID);
				break;
			case 4: //范围型攻击,以自身为中心
				ActionYuanYueWanDaoBegin(sd, paramX, paramY, paramID);
				break;
			case 5: //范围型攻击,以目标为中心
				ActionBingPaoXiaoBegin(sd, paramX, paramY, paramID, false);
				break;
			case 6: //地面效果,(火墙)
				ActionHuoQiangBegin(sd, paramX, paramY, &paramID, false);
				break;
			case 7:
				ActionOfAOE0(sd, paramX, paramY, &paramID);
				bNotify = false;
				break;
			case 8:
				ActionOfAOE1(sd, paramX, paramY, &paramID);
				break;
			case 9: 
				ActionLeiDianShuBegin(sd, &paramX, &paramY, paramID, false);
				break;
			case 10:
				ActionBanYueWanDaoBegin(sd, paramX, paramY, paramID);
			default:
				OnSkillAttack(target,sd);
				break;
			}

			if (bNotify)
			{
				for( DomainPlayVector::iterator pos = mPlayerList.begin();
					pos != mPlayerList.end();)
				{
					if ((*pos))
					{
						(*pos)->NotifyUseSkill(this, sd->EffectType, paramX, paramY, paramID, sd->Effect_ResID, this->Dir());
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

			m_AttackTimer.Restart();
			mAIState = AI_STATE_ATTACK;
			return AI_STATE_ATTACK;
		}

		m_AttackTimer.Restart();
		mAIState = AI_STATE_ATTACK;
		return mAIState;
	}

	int PlayAIBase::OnSkillAttack(PlayActiveObject* target,SkillDef* sd)
	{
		return 0;
	}

	int PlayAIBase::ChooseSkill(PlayActiveObject* target)
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		// 1.残血技能优先级别最高,如果达到残血技能释放条件,重置其他条件
		if (m_AI.life_skill > 0 
			&& this->Hp() * 10000 <  this->MaxHp() * m_AI.life_left_rate 
			&& m_AIAtkParam.life_skill_use_count < m_AI.max_life_skill_cout)
		{
			++ m_AIAtkParam.life_skill_use_count;
			
			// 重置优先级低的技能释放时间
			m_AIAtkParam.life_time_skill_tick = now;
			m_AIAtkParam.time_skill_tick = now;
			
			return m_AI.life_skill;
		}

		if (m_AI.life_time_skill > 0 
			&& this->Hp() * 10000 <  this->MaxHp() * m_AI.life_time_skill_life_rate
			&& m_AIAtkParam.in_fight_tick/* - double(m_AI.life_time_skill_time) / 1000*/ > m_AIAtkParam.out_fight_tick
			&& m_AIAtkParam.life_time_skill_tick + double(m_AI.life_time_skill_time) / 1000 < now)
		{
			m_AIAtkParam.life_time_skill_tick = now;
			m_AIAtkParam.time_skill_tick = now;
			return m_AI.life_time_skill;
		}

		if (m_AI.time_skill > 0 
			&& m_AIAtkParam.in_fight_tick /*- double(m_AI.time_skill_need) / 1000*/ > m_AIAtkParam.out_fight_tick
			&& m_AIAtkParam.time_skill_tick + double(m_AI.time_skill_need) / 1000 < now)
		{
			m_AIAtkParam.time_skill_tick = now;
			return m_AI.time_skill;
		}

		// 概率技能
		if (m_AI.rate_skill > 0)
		{
			int n = rand() % 10000;
			if (n < m_AI.rate_skill_rate )
			{
				return m_AI.rate_skill;
			}
		}
		
		// 距离技能
		if (m_AI.dis_skill > 0)
		{
			if (std::abs(target->X()-X()) <= m_AI.dis_skill_min
			&& std::abs(target->Y()-Y()) <= m_AI.dis_skill_min)
			{
				return m_AI.dis_skill;
			}
		}

		return m_AI.default_skill;
	}

	void PlayAIBase::RandGo()
	{
		if (this)
		{
			if (mPatrolRange > 0)
			{
				if ((abs(X() - mReliveX) + abs(Y() - mReliveY)) > mPatrolRange)
				{
					Walk(FindNextDirection(X(), Y(), mReliveX, mReliveY));
				}
				else
				{
					Walk(rand() % DIR_COUNT);
				}
			}

			mAIState = AI_STATE_WALK;
		}
	}

}