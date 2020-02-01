#include "StdAfx.h"
#include "PlayActiveObject.h"
#include "PlayMap.h"
#include "PlayMapItem.h"
#include "GameServerLib.h"
#include "SubSkill.h"
#include "SubAction.h"
#include "ManagerStatus.h"
#include "ManagerSkillDef.h"
#include "ManagerItemDef.h"
#include "SubStatus.h"
#include "DomainData.h"
#include "PlayMonster.h"
#include "PlaySlave.h"
#include "DomainPlay.h"
#include "ScriptLua.h"
#include "ManagerStatus.h"
#include "ManagerSwordDef.h"
#include "ManagerMap.h"
#include "PlayMapTrap.h"
#include "TimerFrame.h"
#include "ManagerTrapDef.h"
#include "ManagerObjFuncs.h"

namespace OGSLib
{
	ScriptItemAttibuteParam::ScriptItemAttibuteParam()
	{
		mMaxHpMul=0.0f;
		mMaxHpAdd=0.0f;
		mMaxMpMul=0.0f;
		mMaxMpAdd=0.0f;
		mACMul=0.0f;
		mACMaxMul=0.0f;
		mMACMul=0.0f;
		mMACMaxMul=0.0f;
		mDCMul=0.0f;
		mDCMaxMul=0.0f;
		mMCMul=0.0f;
		mMCMaxMul=0.0f;
		mSCMul=0.0f;
		mSCMaxMul=0.0f;
		mACAdd=0.0f;
		mACMaxAdd=0.0f;
		mMACAdd=0.0f;
		mMACMaxAdd=0.0f;
		mDCAdd=0.0f;
		mDCMaxAdd=0.0f;
		mMCAdd=0.0f;
		mMCMaxAdd=0.0f;
		mSCAdd=0.0f;
		mSCMaxAdd=0.0f;
	}
	ObjectAttribute::ObjectAttribute()
	{
		mMaxMp = 0;
		mMaxHp = 0;
		mMaxPower = 0;
		mPowerPercent = 0;

		mMaxBurden=0;
		mMaxLoad=0;
		mMaxBrawn=0;

		mBurden=0;
		mLoad=0;
		mBrawn=0;

		mAC=0;
		mACMax=0;
		mMAC=0;
		mMACMax=0;
		mDC=0;
		mDCMax=0;
		mMC=0;
		mMCMax=0;
		mSC=0;
		mSCMax=0;

		mAccuracy=AccuracyConstBase;
		mDodge=DodgeConstBase;
		mLuck=0;

		mAntiMagic=1;
		mAntiPoison=0;

		mHpRecover=0;
		mMpRecover=0;
		mPoisonRecover=0;

		mMabi_prob=0;
		mMabi_dura=0;
		mDixiao_pres=0;
		mFuyuan_cd=0;
		mFuyuan_pres=0;
		mDropProb=0;
		mDoubleAttProb=0;

		mXishouProb=0;
		mXishouPres=0;
		mFantanProb=0;
		mFantanPres=0;
		mBaojiProb=0;
		mBaojiPres=0;
		mBaojiliPercent=0;
		mXixueProb=0;
		mXixuePres=0;
		mIgnoreDef=0;
		mHpPres=0;
		mMpPres=0;
		mFight=0;
		mSubBaoji=0;
		mTenacity=0;
		mHolyDam=0;
		mPKIncrement = 0; 
		mPKReduce = 0;
		mExpGain = 0;
		mDamReduce = 0;
		mDamReduceDC = 0;
		mDamReduceMC = 0;
		mDamIncrement = 0;
		mPKReduceDC = 0;
		mPKReduceMC = 0;
		mMonDamIncrement = 0;
		mZSMonDamIncrement = 0;
	}
	PlayActiveObject::PlayActiveObject(void)
	{
		mInteractive=true;
		mActionKeyGen = 10;
		mMp=1;
		mHp=1;
		mPower=0;
		mLevel=0;
		mIncHP=0;
		mHurtSeed = 0;
		mPerHP=5;
		mIncMP=0;
		mPerMP=5;
		mIncMagicHP=0;
		mPerMagicHP=5;
		mUpdateHpMp=0.0;
		mUpdateHpMpDuration=1.0;
		mUpdateHpMpSlow = 0;
		mAutoRecover = 0;
		for(int i = 0;i<STATUS_TYPE_NUMBER;i++)
		{
			mStatusTimer[i] = 0.0;
			mStatusParam[i] = 0;
			mStatusFlags[i] = 0;
			mStatusTimerTick[i] = 0.0;
			mStatusTimerTickGap[i] = 1.0;
			for (int j=0;j<6;j++)
			{
				mStatusLevel[i][j]=0;
			}
		}
		mVisibleInteractiveObjectList.reserve(32);

		mUpdateHPMPTick=0;
		mUpdateHP=0;
		mUpdateMP=0;
		mAttackEffectFlags=0x0;
		mUpdateSpeed=1.0f;
		mZsLevel = 0;
		mBurner = 0;
		mSwordLv=0;
		mMonPointLv=0;

		mBattleStatusTick = 0;
		mBattleStatusGap = 3;
		mLevelUpExp = 0;
		mDefId = 0;
	}
	PlayActiveObject::~PlayActiveObject(void)
	{
	}
	bool PlayActiveObject::IsStatus(int status_id)
	{
		if( status_id>=0 && status_id<STATUS_TYPE_NUMBER) 
		{
			return mStatusTimer[status_id] > 0.0;
		}
		return false;
	}
	bool PlayActiveObject::IsStatusMFD(int status_id)
	{
		if( status_id>=0 && status_id<STATUS_TYPE_NUMBER) 
		{
			return (mStatusTimer[status_id]- mGSL->GetTimerFix()->Elapsed()) > 0.0;
		}
		return false;
	}
	bool PlayActiveObject::IsBattleStatus()
	{
		return (mGSL->GetTimerFix()->Elapsed() - mBattleStatusTick) < mBattleStatusGap;
	}
	int PlayActiveObject::StatusParam(int status_id)
	{
		if( IsStatus(status_id) )
		{
			return mStatusParam[status_id];
		}
		return 0;
	}
	int* PlayActiveObject::StatusLevel(int status_id)
	{
		if( IsStatus(status_id) )
		{
			return mStatusLevel[status_id];
		}
		return NULL;
	}
	StatusInfo*PlayActiveObject::GetStatusData(int status_id,int lv)
	{
		return mGSL->GetStatusManager()->GetStatusInfo(status_id,lv);
	}
	double PlayActiveObject::StatusTickGap(int status_id)
	{
		if( IsStatus(status_id) )
		{
			return mStatusTimerTickGap[status_id];
		}
		return 0;
	}
	double  PlayActiveObject::GetStatusDuration(int status_id)
	{
		if( status_id>=0 && status_id<STATUS_TYPE_NUMBER) 
		{
			if( mStatusTimer[status_id] > 0.0 && mStatusTimer[status_id] > mGSL->GetTimerFix()->Elapsed() )
			{
				return (mStatusTimer[status_id] - mGSL->GetTimerFix()->Elapsed());
			}
		}
		return 0;
	}

	void PlayActiveObject::AttackPosition(int x,int y,int param,int pawer,bool all,int delay)
	{
		SkillDef * sd = mGSL->GetSkillDefineManager()->GetSkillDefine(param);
		AttackPosition(x, y, sd, pawer, all, delay);
	}

	void PlayActiveObject::AttackPosition(int x,int y,SkillDef * sd,int pawer,bool all,int delay)
	{
		PlayActiveObject* objects[20];
		//bool attack_one=false;
		if(!all&&sd->SelectTargetWay > 1)
		{
			all = true;
		}
		//sd.SelectTargetWay > 1 
		int damageNum = 0;
		int obj_num = FindActiveInPosition(x,y,objects,20);
		for(int i = 0; i < obj_num; i++ )
		{
            PlayActiveObject* ob = objects[i];
			if( ob->IsInteractive() )
			{
				if (IsProperAttackDes(ob, sd))
				{
					if( damageNum >= sd->DamageNumMax )
					{
						continue;
					}
					damageNum++;
					//attack_one = true;
					Attack(ob,sd,pawer,0,delay);
					if( ! all ) break;
				}
			}
		}
	}

	int PlayActiveObject::status()
	{
		int st=0;
		for(int i = 0;i<STATUS_TYPE_NUMBER;i++)
		{
			if( mStatusTimer[i] > 0.0 )
			{
				st |= 1<<i;
			}
		}
		return st;
	}
	bool PlayActiveObject::Save(DomainData* data)
	{
		data->mStatusMap.clear();
		double t = mGSL->GetTimerFix()->Elapsed();
		for(int i = 0;i<STATUS_TYPE_NUMBER;i++)
		{
			if( mStatusFlags[i]%10 )
			{
				if( t < mStatusTimer[i] )
				{
					SubStatus ss;
					ss.StatusID = i;
					ss.Duration = mStatusTimer[i]-t;
					if( mStatusFlags[i]/10%10 )
					{
						ss.Duration = time(0) + ss.Duration;
					}
					ss.Gap = mStatusTimerTickGap[i];
					ss.Param = mStatusParam[i];
					ss.Flags = mStatusFlags[i];
					ss.Tick = 0;
					data->mStatusMap[i] = ss;
				}
			}
		}
		for (BuffList::iterator it = m_buffs.begin(); it != m_buffs.end(); ++ it)
		{
			if (it->buff_info.mOfflineSave == 0)
				continue;

			SubBuff sb;
			sb.id = it->buff_info.mBuffID;
			if (it->buff_info.mValidType == BUFF_VALID_TYPE_TIME)
			{
				sb.end_val = it->buff_end_value - it->buff_time_tick;
				// 获取buff结束时间点
				sb.end_val = time(0) + sb.end_val;
			}
			else
				sb.end_val = it->buff_end_value;
			data->mBuffMap[it->buff_info.mBuffID] = sb;
		}
		return false;
	}
	bool PlayActiveObject::Load(DomainData* data)
	{
		for( DomainData::StatusMap::iterator pos = data->mStatusMap.begin();
			pos != data->mStatusMap.end();
			++ pos )
		{
			if( pos->second.StatusID >= 0 && pos->second.StatusID < STATUS_TYPE_NUMBER)
			{
				int status = pos->second.StatusID;
				int flag = pos->second.Flags;
				double curtime = mGSL->GetTimerFix()->Elapsed();
				double t = curtime+pos->second.Duration;
				if( flag/10%10 )
				{
					t = curtime + (pos->second.Duration - time(0));
				}
				if( t > 0 && t < 40*24*60*60 )
				{
					mStatusFlags[status] = flag;
					mStatusTimer[status] = t;
					mStatusParam[status] = pos->second.Param;
					mStatusTimerTick[status] = curtime;
					mStatusTimerTickGap[status] = pos->second.Gap;
				}
			}
		}
		data->mStatusMap.clear();

		for (DomainData::BuffMap::iterator it = data->mBuffMap.begin(); it != data->mBuffMap.end(); ++ it)
		{
// 			if (it->second.end_val >= time(0))
// 			{
// 				double left_time = it->second.end_val- time(0);
				
/*			}*/
			AddBuff(it->second.id, it->second.end_val);
		}
		data->mBuffMap.clear();
		return false;
	}
	void PlayActiveObject::UpdateStatusTimer(double time)
	{
		PERF_NODE_FUNC();
		for(int i = 0;i<STATUS_TYPE_NUMBER;i++)
		{
			if( time > mStatusTimer[i] )
			{
				if( ! (mStatusTimer[i] > -0.1 && mStatusTimer[i]<0.1) )
				{
					ClearStatus(i);
				}
			}
			else
			{
				if( time > mStatusTimerTick[i] + mStatusTimerTickGap[i] )
				{
					mStatusTimerTick[i] = time;
					switch(i)
					{
					case STATUS_TYPE_DEC_HP:
					case STATUS_TYPE_POSION_HP:
						{
							PlayActiveObject* des = 0;
							if( Posioner() > 0 )
							{
								des = FindActiveInView(Posioner());
							}
							DamageHP(des,mStatusParam[i],0,true);
						}
						break;
					case STATUS_TYPE_BURNING:
						{
							PlayActiveObject* des = 0;
							if( Burner() > 0 )
							{
								des = FindActiveInView(Burner());
							}
							DamageHP(des,mStatusParam[i]&0xFFFF,0);
							int x=mStatusParam[i]>>16;
							x--;
							if(x > 0 ){
								mStatusParam[i] = (x<<16) | mStatusParam[i]&0xFFFF;
							}else{
								mStatusTimer[i]=time;
							}
						}
						break;
					case STATUS_TYPE_AUTO_ADD_EXP:
						{
							AddExperience(mStatusParam[i],0);
						}
						break;
					case STATUS_TYPE_USUAL_RECOVER:
					case STATUS_TYPE_BATTLE_RECOVER:
						{
							AddHP(mStatusParam[i]);
						}
						break;
					default:
						//持续性药品药效
						if (i >= STATUS_TYPE_CONTINUATE_DRUG1 && i <= STATUS_TYPE_CONTINUATE_DRUG5)
						{
							DomainPlay* player = static_cast<DomainPlay*>(this);
							if (player) {
								mGSL->GetScriptEngine()->OnlyPlayer(player,i,0,"item.chufa.onAddDrugEffect");
							}
						}
						break;
					}
				}
			}
		}
	}
	
	void PlayActiveObject::UpdateBuffTimer(double time)
	{
		PERF_NODE_FUNC();
		bool calculte = false;
		// 检测过期buff, 删除无效buff
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if ((it->buff_info.mValidType == BUFF_VALID_TYPE_TIME && time > it->buff_end_value)
				||  ((it->buff_info.mValidType == BUFF_VALID_TYPE_HP || it->buff_info.mValidType == BUFF_VALID_TYPE_MP || it->buff_info.mValidType == BUFF_VALID_TYPE_HPMP) && it->buff_end_value <= 0.1)
				)
			{
				int clear_id = it->buff_info.mBuffID;
				it->buff_info.mBuffID = 0;
				// 通知客户端移除buff
				NotifyBuffChange(this, clear_id, 0, 0);
				for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
					p != mVisibleInteractiveObjectList.end();
					++ p )
				{
					if ((*p))
					{
						(*p)->NotifyBuffChange(this, clear_id, 0, 0);
					}
				}
			}

			if (it->buff_info.mBuffID == 0)
			{
				if (!calculte)
				{
					for (size_t i = 0; i < 3; ++ i)
					{
						if (it->buff_info.mFuncs[i] == 0)
							continue;

						FuncInfo * pFunc = mGSL->GetObjFuncManager()->GetFuncInfo(it->buff_info.mFuncs[i]);
						if (!pFunc)
							continue;

						if (pFunc->m_nCalAttr == 1)
						{
							calculte = true;
							break;
						}
					}
				}

				it = m_buffs.erase(it);
				continue;
			}
			++ it;
		}

		it = m_buffs.begin();

		while (it != m_buffs.end())
		{
			if (it->buff_info.mGap <= 0)
			{
				++it;
				continue;
			}

			if ((time > it->buff_time_tick + it->buff_info.mGap * 1.0f / 1000))
			{
				it->buff_time_tick = time;
				if (!it->enable)
				{
					++it;
					continue;
				}
				bool bChange = false;
				for (size_t i = 0; i < 3; ++ i)
				{
					if (it->buff_info.mFuncs[i] == 0)
						continue;

					FuncInfo * pFunc = mGSL->GetObjFuncManager()->GetFuncInfo(it->buff_info.mFuncs[i]);
					if (!pFunc)
						continue;

					if (pFunc->m_nCalAttr == 0)
					{
						for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
						{
							if (pFunc->m_propFuncs[i].mFunc)
							{
								int val = pFunc->m_propFuncs[i].mFunc->onHandle(this);
								if (it->buff_info.mValidType >= BUFF_VALID_TYPE_HP && it->buff_info.mValidType <= BUFF_VALID_TYPE_HPMP && val > 0)
								{
									bChange = true;
									it->buff_end_value -= val;
									if (it->buff_end_value <= 0.1)
										break;
								}
							}
						}

						if (it->buff_info.mValidType >= BUFF_VALID_TYPE_HP && it->buff_info.mValidType <= BUFF_VALID_TYPE_HPMP)
						{
							if (it->buff_end_value <= 0.1)
								break;
						}

						continue;
					}

					if (pFunc->m_nCalAttr == 1)
					{
						calculte = true;
						continue;
					}
				}

				if (bChange && it->buff_info.mUI && it->buff_info.mValidType >= BUFF_VALID_TYPE_HP && it->buff_info.mValidType <= BUFF_VALID_TYPE_HPMP)
				{
					// buff信息更新
					NotifyBuffChange(this, it->buff_info.mBuffID, 2, it->buff_end_value);
					for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
						p != mVisibleInteractiveObjectList.end();
						++ p )
					{
						if ((*p))
						{
							(*p)->NotifyBuffChange(this, it->buff_info.mBuffID, 2, it->buff_end_value);
						}
					}
				}
			}
			++ it;
		}

		if (calculte == true)
		{
			// 重新计算属性
			CalculateAttribute();
			NotifyAttributeChange();
			NotifyHPMPChange(this,0,0);
		}
	}

	bool PlayActiveObject::AddIncMagicHP(int paw)
	{
		mIncMagicHP += paw;
		if( mIncMagicHP > Attributes()->mMaxHp ) mIncMagicHP = Attributes()->mMaxHp;
		mPerMagicHP = 20;
		return true;
	}

	int PlayActiveObject::checkStatusMonDropItemAddProb()
	{
		if( IsStatus(STATUS_TYPE_ADD_DROP_ITEMADD_PROB) )
		{
			return mStatusParam[STATUS_TYPE_ADD_DROP_ITEMADD_PROB];
		}
		return 0;
	}

	int PlayActiveObject::checkStatusmulpro()
	{
		if (IsStatus(STATUS_TYPE_MULPRO))
		{
			return mStatusParam[STATUS_TYPE_MULPRO];
		}
		return 10;
	}

	bool PlayActiveObject::AddStatus(int status,int duration,int param,double gap,int flags,int level[])
	{
		double curtime = mGSL->GetTimerFix()->Elapsed();

		double time = curtime + duration * 1.0 /1000;
		if (duration == 0xFFFFFFFF)
			time = time + 0xFFFFFFFF;

		mStatusTimer[status] = time;
		mStatusParam[status] = param;
		mStatusFlags[status] = flags;
		mStatusTimerTick[status] = curtime;
		mStatusTimerTickGap[status] = gap;
		if (level)
		{
			for (int i=0;i<6;i++)
			{
				mStatusLevel[status][i] = level[i];
			}
		}

		NotifySatusChange(this,status);
		for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
			p != mVisibleInteractiveObjectList.end();
			++ p )
		{
			if ((*p))
			{
				(*p)->NotifySatusChange(this, status);
			}
		}

		if( status == STATUS_TYPE_ADD_ACMAX
			|| status == STATUS_TYPE_ADD_MACMAX
			|| status == STATUS_TYPE_ADD_DCMAX
			|| status == STATUS_TYPE_ADD_MCMAX
			|| status == STATUS_TYPE_ADD_SCMAX
			|| status == STATUS_TYPE_ADD_HP
			|| status == STATUS_TYPE_ADD_MP
			|| status == STATUS_TYPE_AC
			|| status == STATUS_TYPE_MAC
			|| status == STATUS_TYPE_DC
			|| status == STATUS_TYPE_MC
			|| status == STATUS_TYPE_SC
			|| status == STATUS_TYPE_BOJIPROP
			|| status == STATUS_TYPE_BOJIPRES
			|| status == STATUS_TYPE_TENACITY
			|| status == STATUS_TYPE_SACREDDAM
			|| status == STATUS_TYPE_YOULINGDUN
			|| status == STATUS_TYPE_SHENSHENGZHANJIASHU
			|| status == STATUS_TYPE_WLMZ
			/*|| status >STATUS_TYPE_GUANZHI*/ )
		{
			CalculateAttribute();
			NotifyAttributeChange();
			NotifyHPMPChange(this,0,0);
		}
		return true;
	}

	void PlayActiveObject::CalculateStatusAttribute()
	{
 		if (IsStatus(STATUS_TYPE_YOULINGDUN) )
		{
			Attributes()->mMAC += StatusParam(STATUS_TYPE_YOULINGDUN);
 			Attributes()->mMACMax += StatusParam(STATUS_TYPE_YOULINGDUN);
 		}
 		if (IsStatus(STATUS_TYPE_SHENSHENGZHANJIASHU))
 		{
			Attributes()->mAC += StatusParam(STATUS_TYPE_SHENSHENGZHANJIASHU);
 			Attributes()->mACMax += StatusParam(STATUS_TYPE_SHENSHENGZHANJIASHU);
 		}

		for( ManagerStatus::StatusInfoMap::iterator pos = mGSL->GetStatusManager()->GetStatusMap().begin();
			pos != mGSL->GetStatusManager()->GetStatusMap().end();
			++ pos )
		{
			int status_id = pos->first / 1000;
			//if( status_id <= STATUS_TYPE_GUANZHI ){continue;}
			if( IsStatus(status_id) )
			{
				if (status_id==STATUS_TYPE_SHOULING)
				{
					int* level = StatusLevel(status_id);
					if (level)
					{
						int minilevel = 50;
						for (int i = 0 ; i < 6 ; i++)
						{
							if (level[i] <= minilevel)
							{
								minilevel = level[i];
							}
						}
						StatusInfo* mini_si = nullptr;
						if (minilevel > 0)
						{
							mini_si = mGSL->GetStatusManager()->GetStatusInfo(status_id,minilevel);
						}
						if (level[SHOULING_TYPE_JIANREN] && level[SHOULING_TYPE_JIANREN] > 0 && level[SHOULING_TYPE_JIANREN] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[SHOULING_TYPE_JIANREN]);
							if (si)
							{
								Attributes()->mAC += si->mAC+static_cast<int>(mini_si ? (mini_si->mAC/3) : 0 );
								Attributes()->mACMax += si->mACMax+static_cast<int>(mini_si ? (mini_si->mACMax/3) : 0 );
							}
						}
						if (level[SHOULING_TYPE_WUWEI] && level[SHOULING_TYPE_WUWEI] > 0 && level[SHOULING_TYPE_WUWEI] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[SHOULING_TYPE_WUWEI]);
							if (si)
							{
								Attributes()->mMAC += si->mMAC+static_cast<int>(mini_si ? (mini_si->mMAC/3) : 0 );
								Attributes()->mMACMax += si->mMACMax+static_cast<int>(mini_si ? (mini_si->mMACMax/3) : 0 );
							}
						}
						if (level[SHOULING_TYPE_SHASHANG] && level[SHOULING_TYPE_SHASHANG] > 0 && level[SHOULING_TYPE_SHASHANG] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[SHOULING_TYPE_SHASHANG]);
							if (si)
							{
								Attributes()->mBaojiPres += si->mBaijiPres+static_cast<int>(mini_si ? (mini_si->mBaijiPres/3) : 0 );
							}
						}
						if (level[SHOULING_TYPE_ZHENFEN] && level[SHOULING_TYPE_ZHENFEN] > 0 && level[SHOULING_TYPE_ZHENFEN] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[SHOULING_TYPE_ZHENFEN]);
							if (si)
							{
								Attributes()->mBaojiProb += si->mBaojiProb+static_cast<int>(mini_si ? (mini_si->mBaojiProb/3) : 0 );
							}
						}
						if (level[SHOULING_TYPE_SHENGYUAN] && level[SHOULING_TYPE_SHENGYUAN] > 0 && level[SHOULING_TYPE_SHENGYUAN] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[SHOULING_TYPE_SHENGYUAN]);
							if (si)
							{
								Attributes()->mMaxHp += si->mMaxHp+static_cast<int>(mini_si ? (mini_si->mMaxHp/3) : 0 );
							}
						}
						if (level[SHOULING_TYPE_MANLI] && level[SHOULING_TYPE_MANLI] > 0 && level[SHOULING_TYPE_MANLI] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[SHOULING_TYPE_MANLI]);
							if (si)
							{
								Attributes()->mDC += si->mDC+static_cast<int>(mini_si ? (mini_si->mDC/3) : 0 );
								Attributes()->mDCMax += si->mDCMax+static_cast<int>(mini_si ? (mini_si->mDCMax/3) : 0 );
								Attributes()->mMC += si->mMC+static_cast<int>(mini_si ? (mini_si->mMC/3) : 0 );
								Attributes()->mMCMax += si->mMCMax+static_cast<int>(mini_si ? (mini_si->mMCMax/3) : 0 );
								Attributes()->mSC += si->mSC+static_cast<int>(mini_si ? (mini_si->mSC/3) : 0 );
								Attributes()->mSCMax += si->mSCMax+static_cast<int>(mini_si ? (mini_si->mSCMax/3) : 0 );
							}
						}
					}
				}
				else if (status_id==STATUS_TYPE_GUILD)
				{
					int* level = StatusLevel(status_id);
					if (level)
					{
						if (level[0] &&  level[0] > 0 && level[0] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[0]);
							if (si)
							{
								Attributes()->mMaxHp += si->mMaxHp;
							}
						}
						if (level[1] &&  level[1] > 0 && level[1] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[1]);
							if (si)
							{
								Attributes()->mAC += si->mAC;
								Attributes()->mACMax += si->mACMax;
							}
						}
						if (level[2] &&  level[2] > 0 && level[2] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[2]);
							if (si)
							{
								Attributes()->mMAC += si->mMAC;
								Attributes()->mMACMax += si->mMACMax;
							}
						}
						if (level[3] &&  level[3] > 0 && level[3] == pos->second.mLv)
						{
							StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,level[3]);
							if (si)
							{
								Attributes()->mDC += si->mDC;
								Attributes()->mDCMax += si->mDCMax;
								Attributes()->mMC += si->mMC;
								Attributes()->mMCMax += si->mMCMax;
								Attributes()->mSC += si->mSC;
								Attributes()->mSCMax += si->mSCMax;
							}
						}
					}
				}
				else
				{
					int status_lv = StatusParam(status_id);
					StatusInfo* si = mGSL->GetStatusManager()->GetStatusInfo(status_id,status_lv);
					if( si )
					{
						if(pos->second.mLv != status_lv){continue;}
						float hpt = 1.0f*si->mMaxHp / 10000;
						float mpt = 1.0f*si->mMaxHp / 10000;
						Attributes()->mMaxHp += static_cast<int>(Attributes()->mMaxHp * hpt);
						Attributes()->mMaxMp += static_cast<int>(Attributes()->mMaxMp * mpt);
						Attributes()->mAC += si->mAC;
						Attributes()->mACMax += si->mACMax;
						Attributes()->mMAC += si->mMAC;
						Attributes()->mMACMax += si->mMACMax;
						Attributes()->mDC += si->mDC;
						Attributes()->mDCMax += si->mDCMax;
						Attributes()->mMC += si->mMC;
						Attributes()->mMCMax += si->mMCMax;
						Attributes()->mSC += si->mSC;
						Attributes()->mSCMax += si->mSCMax;
						Attributes()->mIgnoreDef += si->mNoDef;
						Attributes()->mBaojiProb += si->mBaojiProb;
						Attributes()->mBaojiPres += si->mBaijiPres;
						Attributes()->mHolyDam += si->mHolyDam;
					}
					else
					{
						StatusInfo* sif = mGSL->GetStatusManager()->GetStatusInfo(status_id,1);
						if( sif )
						{
							float hpt = 1.0f*sif->mMaxHp * status_lv / 10000;
							float mpt = 1.0f*sif->mMaxMp * status_lv / 10000;
							Attributes()->mMaxHp += static_cast<int>(Attributes()->mMaxHp * hpt);
							Attributes()->mMaxMp += static_cast<int>(Attributes()->mMaxMp * mpt);
							Attributes()->mAC += sif->mAC * status_lv;
							Attributes()->mACMax += sif->mACMax * status_lv;
							Attributes()->mMAC += sif->mMAC * status_lv;
							Attributes()->mMACMax += sif->mMACMax * status_lv;
							Attributes()->mDC += sif->mDC * status_lv;
							Attributes()->mDCMax += sif->mDCMax * status_lv;
							Attributes()->mMC += sif->mMC * status_lv;
							Attributes()->mMCMax += sif->mMCMax * status_lv;
							Attributes()->mSC += sif->mSC * status_lv;
							Attributes()->mSCMax += sif->mSCMax * status_lv;
							Attributes()->mIgnoreDef += sif->mNoDef * status_lv;
							Attributes()->mBaojiProb += sif->mBaojiProb * status_lv;
							Attributes()->mBaojiPres += sif->mBaijiPres * status_lv;
							Attributes()->mHolyDam += sif->mHolyDam * status_lv;
						}
					}
				}
			}
		}
	}

	bool PlayActiveObject::AddBuff(int id, double param)
	{
		BuffInfo * pBuff = mGSL->GetStatusManager()->GetBuffInfo(id);
		if (!pBuff)
		{
			return false;
		}
		double curTick = mGSL->GetTimerFix()->Elapsed();
		ObjBuff buf;
		buf.enable = true;
		buf.buff_info = * pBuff;
		buf.buff_time_tick = curTick;
		if (buf.buff_info.mValidType == BUFF_VALID_TYPE_TIME)
		{
			// 记录时间
			if (param > 0)
			{
				// 剩余值
				int nLeft = param - time(0);
				if (nLeft > 0)
				{
					buf.buff_end_value = curTick + nLeft;
				}
				else
					return false;
				
			}
			else
				buf.buff_end_value = curTick + pBuff->mValidMaxValue / 1000;
		}
		else 
		{
			if (param > 0)
				buf.buff_end_value = param;
			else
				buf.buff_end_value = pBuff->mValidMaxValue;
		}

		if (pBuff->mOverlie == 0)
		{
			// 替换同id
			BuffList::iterator it = m_buffs.begin();
			while (it != m_buffs.end())
			{
				if (it->buff_info.mBuffID == buf.buff_info.mBuffID)
				{
					int clear_id = it->buff_info.mBuffID;

					NotifyBuffChange(this, clear_id, 0, 0);
					for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
						p != mVisibleInteractiveObjectList.end();
						++ p )
					{
						if ((*p))
						{
							(*p)->NotifyBuffChange(this, clear_id, 0, 0);
						}
					}

					it = m_buffs.erase(it);
					continue;
				}
				++ it;
			}
		}
		else if (pBuff->mOverlie == 1)
		{
			// 替换同类型
			BuffList::iterator it = m_buffs.begin();
			while (it != m_buffs.end())
			{
				if (it->buff_info.mBuffType == buf.buff_info.mBuffType)
				{
					int clear_id = it->buff_info.mBuffID;

					NotifyBuffChange(this, clear_id, 0, 0);
					for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
						p != mVisibleInteractiveObjectList.end();
						++ p )
					{
						if ((*p))
						{
							(*p)->NotifyBuffChange(this, clear_id, 0, 0);
						}
					}

					it = m_buffs.erase(it);
					continue;
				}
				++ it;
			}
		}
		else if (pBuff->mOverlie == 2)
		{
			// 替换低等级
			BuffList::iterator it = m_buffs.begin();
			while (it != m_buffs.end())
			{
				if (it->buff_info.mBuffType == buf.buff_info.mBuffType && it->buff_info.mLv <= buf.buff_info.mLv)
				{
					int clear_id = it->buff_info.mBuffID;

					NotifyBuffChange(this, clear_id, 0, 0);
					for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
						p != mVisibleInteractiveObjectList.end();
						++ p )
					{
						if ((*p))
						{
							(*p)->NotifyBuffChange(this, clear_id, 0, 0);
						}
					}

					it = m_buffs.erase(it);
					continue;
				}
				++ it;
			}
		}
		else if (pBuff->mOverlie == 4)
		{
			// 附加同类型
			BuffList::iterator it = m_buffs.begin();
			while (it != m_buffs.end())
			{
				if (it->buff_info.mBuffType == buf.buff_info.mBuffType)
				{
					// 老的记录剩余值,移除
					buf.buff_end_value += it->buff_end_value;
					int clear_id = it->buff_info.mBuffID;

					NotifyBuffChange(this, clear_id, 0, 0);
					for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
						p != mVisibleInteractiveObjectList.end();
						++ p )
					{
						if ((*p))
						{
							(*p)->NotifyBuffChange(this, clear_id, 0, 0);
						}
					}
					it = m_buffs.erase(it);
					continue;
				}
				++ it;
			}
		}
		else 
		{
			// 3 替换低伤害 
			// 需求不明确,暂时不实现
			return false;
		}

		m_buffs.push_back(buf);

		bool calculte = false;

		if (buf.buff_info.mGap == 0)
		{
			// 添加的时候生效
			for (int i = 0; i < 3; ++ i)
			{
				if (buf.buff_info.mFuncs[i] == 0)
					continue;

				FuncInfo * pFunc = mGSL->GetObjFuncManager()->GetFuncInfo(buf.buff_info.mFuncs[i]);
				if (!pFunc)
					continue;

				if (pFunc->m_nCalAttr == 0)
				{
					for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
					{
						if (pFunc->m_propFuncs[i].mFunc)
							pFunc->m_propFuncs[i].mFunc->onHandle(this);
					}
					continue;
				}

				if (pFunc->m_nCalAttr == 1)
				{
					calculte = true;
					continue;
				}
			}
		}

		// 通知客户端
		if (buf.buff_info.mValidType == BUFF_VALID_TYPE_TIME)
			NotifyBuffChange(this,id, 1, buf.buff_end_value - curTick);
		else
			NotifyBuffChange(this, id, 1, buf.buff_end_value);

		for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
			p != mVisibleInteractiveObjectList.end();
			++ p )
		{
			if ((*p))
			{
				if (buf.buff_info.mValidType == BUFF_VALID_TYPE_TIME)
					(*p)->NotifyBuffChange(this, id, 1, buf.buff_end_value - curTick);
				else
					(*p)->NotifyBuffChange(this, id, 1, buf.buff_end_value);
			}
		}

		if (calculte == true)
		{
			// 重新计算属性
			CalculateAttribute();
			NotifyAttributeChange();
			NotifyHPMPChange(this,0,0);
		}

		return true;
	}

	int PlayActiveObject::HaveBuff(int id)
	{
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if (it->buff_info.mBuffID == id)
			{
				return 1;
			}
			++ it;
		}
		return 0;
	}

	double PlayActiveObject::GetBuffValidValue(int id)
	{
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if (it->buff_info.mBuffID == id)
			{
				if (it->buff_info.mValidType == BUFF_VALID_TYPE_TIME)
					return it->buff_end_value - it->buff_time_tick;
				return it->buff_end_value;
			}
			++ it;
		}
		return 0;
	}

	bool PlayActiveObject::ClearBuffOnDead()
	{
		std::vector<int> id_list;
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if (it->buff_info.mDeadRemove)
			{
				id_list.push_back(it->buff_info.mBuffID);
			}
			++ it;
		}

		for (size_t i = 0; i < id_list.size(); ++ i)
		{
			ClearBuff(id_list[i]);
		}
		return true;
	}

	bool PlayActiveObject::ClearBuff(int id)
	{
		bool found = false;
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if (it->buff_info.mBuffID == id)
			{
				found = true;
				it->buff_info.mBuffID = 0;
			}
			++ it;
		}

		// 重新计算属性
		if (found)
		{
			NotifyBuffChange(this, id, 0, 0);
			for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
				p != mVisibleInteractiveObjectList.end();
				++ p )
			{
				if ((*p))
				{
					(*p)->NotifyBuffChange(this, id, 0, 0);
				}
			}

			CalculateAttribute();
			NotifyAttributeChange();
			NotifyHPMPChange(this,0,0);
		}

		return found;
	}

	bool PlayActiveObject::ClearBuffByType(int type)
	{
		std::vector<int> ids;
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if (it->buff_info.mBuffType == type)
			{
				int id = it->buff_info.mBuffID;
				ids.push_back(id);
				it->buff_info.mBuffID = 0;
			}
			++ it;
		}

		// 重新计算属性
		for (size_t i = 0; i < ids.size(); ++ i)
		{
			int id = ids[i];
			NotifyBuffChange(this, id, 0, 0);
			for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
				p != mVisibleInteractiveObjectList.end();
				++ p )
			{
				if ((*p))
				{
					(*p)->NotifyBuffChange(this, id, 0, 0);
				}
			}
		}

		if (ids.size() > 0)
		{
			CalculateAttribute();
			NotifyAttributeChange();
			NotifyHPMPChange(this,0,0);
		}

		return ids.size() != 0;
	}

	bool PlayActiveObject::ClearAllBuff()
	{
		m_buffs.clear();
		return true;
	}

	void PlayActiveObject::CalculateBuffAttribute()
	{
		for (size_t i = 0; i < m_buffs.size(); ++ i)
		{
			if (m_buffs[i].buff_info.mBuffID == 0)
			{
				continue;
			}

			if (m_buffs[i].enable == false)
			{
				continue;
			}

			for (size_t j = 0; j < 3; ++ j)
			{
				int funcId = m_buffs[i].buff_info.mFuncs[j];
				if (funcId == 0)
					continue;

				FuncInfo * pFunc = mGSL->GetObjFuncManager()->GetFuncInfo(funcId);
				if (!pFunc)
					continue;

				if (pFunc->m_nCalAttr == 1)
				{
					for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
					{
						if (pFunc->m_propFuncs[i].mFunc)
							pFunc->m_propFuncs[i].mFunc->onHandle(this);
					}
				}
			}
		}
	}

	bool PlayActiveObject::ClearAllStatus()
	{
		for(int i = 0;i<STATUS_TYPE_NUMBER;i++)
		{
			ClearStatus(i);
		}
		return true;
	}
	bool PlayActiveObject::ClearStatusOnDead()
	{
		for(int i = 0;i<STATUS_TYPE_NUMBER;i++)
		{
			if( (!( mStatusFlags[i]%10 ))  || i == STATUS_TYPE_POSION_HP || i == STATUS_TYPE_POSION_ARMOR || i == STATUS_TYPE_BURNING)
			{
				ClearStatus(i);
			}
		}
		return true;
	}
	bool PlayActiveObject::ClearStatus(int status)
	{
		mStatusTimer[status] = 0.0;
		mStatusParam[status] = 0;
		mStatusFlags[status] = 0;
		mStatusTimerTick[status] = 0.0;
		mStatusTimerTickGap[status] = 1.0;
		for (int i=0;i<6;i++)
		{
			mStatusLevel[status][i]=0;
		}

		if (status == STATUS_TYPE_POSION_HP || status == STATUS_TYPE_POSION_ARMOR || status == STATUS_TYPE_BURNING)
		{
			SetBurner(0);
		}
		if( status == STATUS_TYPE_ADD_ACMAX
			|| status == STATUS_TYPE_ADD_MACMAX
			|| status == STATUS_TYPE_ADD_DCMAX
			|| status == STATUS_TYPE_ADD_MCMAX
			|| status == STATUS_TYPE_ADD_SCMAX
			//|| status == STATUS_TYPE_FCZS
			|| status == STATUS_TYPE_ADD_HP
			|| status == STATUS_TYPE_ADD_MP
			//|| status == STATUS_TYPE_SEVEN_COLOR_DAN
			//|| status == STATUS_TYPE_YUANSHENHUTI 
			//|| status == STATUS_TYPE_TIANSHENHUTI
			//|| status == STATUS_TYPE_FUQITONGXIN 
			|| status == STATUS_TYPE_YOULINGDUN
			|| status == STATUS_TYPE_SHENSHENGZHANJIASHU
			//|| status == STATUS_TYPE_SHENWEI
			//|| status == STATUS_TYPE_ZHIZUN
			//|| status == STATUS_TYPE_XINFA1
			//|| status == STATUS_TYPE_XINFA2
			|| status == STATUS_TYPE_WLMZ
			//|| status == STATUS_TYPE_ZHUANSHEN
			//|| status == STATUS_TYPE_VIP
			//|| status == STATUS_TYPE_ZUOQI
			//|| status == STATUS_TYPE_GUANZHI 
			|| status == 40)
		{
			CalculateAttribute();
			NotifyAttributeChange();
			NotifyHPMPChange(this,0,0);
		}
		return true;
	}
	bool PlayActiveObject::ClearMoFaDun()
	{
		SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_MoFaDun);
		if (!sd)
			return false;
		if (sd->TriggerBuff1)
		{
			BuffInfo * pBuff = mGSL->GetStatusManager()->GetBuffInfo(sd->TriggerBuff1);
			if (!pBuff)
				return false;
			return ClearBuffByType(pBuff->mBuffType);
		}
		return true;
	}
	bool PlayActiveObject::ClearYingShen()
	{
		// 获取隐身术buf type
		SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_JiTiYinShenShu);

		if (sd && sd->TriggerBuff1)
		{
			BuffInfo * pBuff = mGSL->GetStatusManager()->GetBuffInfo(sd->TriggerBuff1);
			if (pBuff)
				ClearBuffByType(pBuff->mBuffType);
		}

		return ClearStatus(STATUS_TYPE_YINGSHEN);
	}
	bool PlayActiveObject::ClearYouLingDun()
	{
		return ClearStatus(STATUS_TYPE_YOULINGDUN);
	}

	bool PlayActiveObject::ClearShenShengZhanJiaShu()
	{
		return ClearStatus(STATUS_TYPE_SHENSHENGZHANJIASHU);
	}

	bool PlayActiveObject::ClearHpRecover()
	{
		return ClearStatus(STATUS_TYPE_HP_RECOVER);
	}

	bool PlayActiveObject::AllYingShen(int paw,int param,int flags)
	{
		return AddStatus(STATUS_TYPE_ALL_YINGSHEN,paw * 1000,param);
	}
	bool PlayActiveObject::ClearAllYingShen()
	{
		return ClearStatus(STATUS_TYPE_ALL_YINGSHEN);
	}
	bool PlayActiveObject::NoDamage(int paw,int param,int flags)
	{
		return AddStatus(STATUS_TYPE_NO_DAMAGE,paw * 1000,param);
	}
	bool PlayActiveObject::ClearNoDamage()
	{
		return ClearStatus(STATUS_TYPE_NO_DAMAGE);
	}
	bool PlayActiveObject::NoDrop(int paw,int param,int flags)
	{
		return AddStatus(STATUS_TYPE_NO_DROP,paw * 1000,param);
	}
	bool PlayActiveObject::ClearNoDrop()
	{
		return ClearStatus(STATUS_TYPE_NO_DROP);
	}
	bool PlayActiveObject::Mabi(SkillDef * sd, int time)
	{
		if (sd->TriggerBuff1)
			AddBuff(sd->TriggerBuff1);
		if (sd->TriggerBuff2)
			AddBuff(sd->TriggerBuff2);
		return AddStatus(STATUS_TYPE_MABI,time, 0);
	}

	bool PlayActiveObject::ClearMabi()
	{
		return ClearStatus(STATUS_TYPE_MABI);
	}

	bool PlayActiveObject::MoFaDun(int paw, SkillDef * sd)
	{
		if (sd->TriggerBuff1)
			AddBuff(sd->TriggerBuff1);

		if (sd->TriggerBuff2)
			AddBuff(sd->TriggerBuff2);

		return true;
	}

	bool PlayActiveObject::IsMoFaDun()
	{
		SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_MoFaDun);
		if (!sd)
			return false;
		if (sd->TriggerBuff1)
		{
			BuffInfo * pBuff = mGSL->GetStatusManager()->GetBuffInfo(sd->TriggerBuff1);
			if (!pBuff)
				return false;
			return HaveBuffByType(pBuff->mBuffType);
		}

		return false;
	}

	bool PlayActiveObject::YingShen(int paw,int param)
	{
		for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
			p != mVisibleInteractiveObjectList.end();
			++ p )
		{
			if ((*p))
			{
				if ((*p)->TypeIsMonster())
				{
					PlayMonster* pm = (PlayMonster*)(*p);
					if (pm)
					{
						if (pm->Target() == this)
						{
							pm->ClearTarget();
						}
					}
				}
			}
		}
		return AddStatus(STATUS_TYPE_YINGSHEN,paw * 1000,param);
	}
	bool PlayActiveObject::YouLingDun(int paw,int param)
	{
		return AddStatus(STATUS_TYPE_YOULINGDUN,paw * 1000,param);
	}
	bool PlayActiveObject::ShenShengZhanJiaShu(int paw,int param)
	{
		return AddStatus(STATUS_TYPE_SHENSHENGZHANJIASHU,paw * 1000,param);
	}
	bool PlayActiveObject::HpRecover(int paw,int param,int flags)
	{
		return AddStatus(STATUS_TYPE_HP_RECOVER,paw * 1000,param,1.0,flags);
	}
	void PlayActiveObject::UpdateHPMP(double time)
	{
		PERF_NODE_FUNC();
		UpdateHPMPInc(time);
		UpdateHPMPRecast(time);
		if( mUpdateHPMPTick + 1 < time )
		{
			mUpdateHPMPTick = time;
			if( mUpdateHP > 0 || mUpdateMP > 0 )
			{
				NotifyHPMPChange(this,0,0);
				for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
					p != mVisibleInteractiveObjectList.end();
					++ p )
				{
					if ((*p))
					{
						(*p)->NotifyHPMPChange(this, 0, 0);
					}
				}
				mUpdateHP = 0;
				mUpdateMP = 0;
			}
		}
	}
	void PlayActiveObject::UpdateHPMPInc(double time)
	{
		if( mIncHP > 0 || mIncMP > 0 || mIncMagicHP > 0 )
		{
			double IncGap = max( 0,(600 - min(400,(level()*10))) ) / 1000.0;
			int nHP = 0;
			int nMP = 0;
			if( mUpdateHPMPInc + IncGap <= time )
			{
				mUpdateHPMPInc = time;
				if( mPerHP <= 0 ) mPerHP = 1;
				if( mPerMP <= 0 ) mPerMP = 1;
				if( mPerMagicHP <= 0 ) mPerMagicHP = 1;
				int hpper = mPerHP;
				if( mPerMagicHP > hpper )
				{
					hpper = mPerMagicHP;
				}

				if( mPerHP > mPerMagicHP )
				{
					if( mIncHP < mPerHP ){
						nHP += mIncHP;
						mIncHP = 0;
						hpper -= mIncHP;
					}else{
						nHP += mPerHP;
						mIncHP -= mPerHP;
						hpper -= mPerHP;
					}
					if( hpper > 0 )
					{
						if( mIncMagicHP < mPerMagicHP ){
							nHP += mIncMagicHP;
							mIncMagicHP = 0;
						}else{
							nHP += mPerMagicHP;
							mIncMagicHP -= mPerMagicHP;
						}
					}
				}
				else
				{
					if( mIncMagicHP < mPerMagicHP ){
						nHP += mIncMagicHP;
						mIncMagicHP = 0;
						hpper -= mIncMagicHP;
					}else{
						nHP += mPerMagicHP;
						mIncMagicHP -= mPerMagicHP;
						hpper -= mPerMagicHP;
					}
					if( hpper > 0 )
					{
						if( mIncHP < mPerHP ){
							nHP += mIncHP;
							mIncHP = 0;
						}else{
							nHP += mPerHP;
							mIncHP -= mPerHP;
						}
					}
				}

				if( mIncMP < mPerMP ){
					nMP += mIncMP;
					mIncMP = 0;
				}else{
					nMP += mPerMP;
					mIncMP -= mPerMP;
				}
				mPerHP = level()/max(mGSL->mHpIncParam1,5) + mGSL->mHpIncParam2;
				mPerMP = level()/max(mGSL->mMpIncParam1,5) + mGSL->mMpIncParam2;
				mPerMagicHP = level()/max(mGSL->mHpMagicIncParam1,5) + mGSL->mHpMagicIncParam2;
				//printf("per %d, remain: %d\n",mPerMagicHP,mIncMagicHP);
				bool h = AddHP(nHP);
				bool m = AddMP(nMP);
				if( Hp()>=MaxHp() ){
					mIncHP = 0;
					mIncMagicHP = 0;
				}
				if( Mp()>=MaxMp() ){
					mIncMP = 0;
				}
				if( h )
				{
					mUpdateHP += nHP;
				}
				if( m )
				{
					mUpdateMP += nMP;
				}
				//if( h || m )
				//{
				//	NotifyHPMPChange(this,0,0);
				//	for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
				//		p != mVisibleInteractiveObjectList.end();
				//		++ p )
				//	{
				//		(*p)->NotifyHPMPChange(this,0,0);
				//	}
				//}
			}
		}else{
			mUpdateHPMPInc = time;
		}
	}
	void PlayActiveObject::UpdateHPMPRecast(double time)
	{
		if( mUpdateHpMp + mUpdateHpMpDuration < time )
		{
			mUpdateHpMp = time;
			mUpdateHpMpSlow --;
			//屏蔽自动回血
			//mAutoRecover --;
			if( mUpdateHpMpSlow < 0 )
			{
				if( mGSL->mPosionStopRecast && (IsPosionArmor() || IsPosionHP()) )
				{
					return;
				}
				mUpdateHpMpSlow = 0;
				int hp_add = 0;
				int mp_add = 0;
				/*if( mAutoRecover < 0 )
				{
					hp_add += (MaxHp() / 75 + 1);
					mp_add += (MaxMp() / 75 + 1);
					mAutoRecover = 10;
				}*/
				if( IsHpRecover() )
				{
					int param = HpRecoverParam();
					int h = (param & 0xFF00) >> 8;
					int m = (param & 0xFF);
					hp_add += h;
					mp_add += m;
				}
				if( hp_add >0 && mp_add>0 )
				{
					bool h = AddHP(hp_add);
					bool m = AddMP(mp_add);
					if( h )
					{
						mUpdateHP += hp_add;
					}
					if( m )
					{
						mUpdateMP += mp_add;
					}
					//if( h || m )
					//{
					//	NotifyHPMPChange(this,hp_add,mp_add);
					//	for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
					//		p != mVisibleInteractiveObjectList.end();
					//		++ p )
					//	{
					//		(*p)->NotifyHPMPChange(this,0,0);
					//	}
					//}
				}
			}
		}
	}
	void PlayActiveObject::NotifyMeet(PlayMap* map,PlayMapObject* src)
	{
		if (src && src->IsInteractive())
		{
			InteractiveObjectVector::iterator pos = std::find(mVisibleInteractiveObjectList.begin(), mVisibleInteractiveObjectList.end(), src);
			if (pos == mVisibleInteractiveObjectList.end())///判断此对象是否在此列表当中
			{
				PlayActiveObject * obj = static_cast<PlayActiveObject*>(src);
				if(obj)
				{
					mVisibleInteractiveObjectList.push_back(obj);
				}
			}
		}
	}
	void PlayActiveObject::publicPlayEffect(int effect_type,int x,int y,unsigned int targetID,unsigned int ownerID,int resID)
	{
		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if ((*pos))
			{
				(*pos)->NotifyPlayEffect(this, effect_type, x, y, targetID, ownerID, resID);
			}
		}
	}
	void PlayActiveObject::NotifyBye(PlayMap* map,PlayMapObject* src)
	{
		//if( src->IsInteractive() )
		{
			mVisibleInteractiveObjectList.erase(std::remove(mVisibleInteractiveObjectList.begin(),mVisibleInteractiveObjectList.end(),src),mVisibleInteractiveObjectList.end());
		}
	}

	void PlayActiveObject::CalculateAttribute()
	{
		mActiveAttribute = mBaseAttribute;

		if( mHp > mActiveAttribute.mMaxHp ) mHp = mActiveAttribute.mMaxHp;
		if( mMp > mActiveAttribute.mMaxMp ) mMp = mActiveAttribute.mMaxMp;
		CalculatePropPercent();
	}

	void PlayActiveObject::CalculatePropPercent()
	{
		Attributes()->mMaxMp += static_cast<int>(1.0f * Attributes()->mMaxMp * mPropPercent.mMaxMp / 10000);
		Attributes()->mMaxHp += static_cast<int>(1.0f * Attributes()->mMaxHp * mPropPercent.mMaxHp / 10000);

		Attributes()->mAC += static_cast<int>(1.0f * Attributes()->mAC * mPropPercent.mAC / 10000);
		Attributes()->mACMax += static_cast<int>(1.0f * Attributes()->mACMax * mPropPercent.mACMax / 10000);

		Attributes()->mMAC += static_cast<int>(1.0f * Attributes()->mMAC * mPropPercent.mMAC / 10000);
		Attributes()->mMACMax += static_cast<int>(1.0f * Attributes()->mMACMax * mPropPercent.mMACMax / 10000);

		Attributes()->mDC += static_cast<int>(1.0f * Attributes()->mDC * mPropPercent.mDC / 10000);
		Attributes()->mDCMax += static_cast<int>(1.0f * Attributes()->mDCMax * mPropPercent.mDCMax / 10000);

		Attributes()->mMC += static_cast<int>(1.0f * Attributes()->mMC * mPropPercent.mMC / 10000);
		Attributes()->mMCMax += static_cast<int>(1.0f * Attributes()->mMCMax * mPropPercent.mMCMax / 10000);

		Attributes()->mSC += static_cast<int>(1.0f * Attributes()->mSC * mPropPercent.mSC / 10000);
		Attributes()->mSCMax += static_cast<int>(1.0f * Attributes()->mSCMax * mPropPercent.mSCMax / 10000);

		Attributes()->mAccuracy += static_cast<int>(1.0f * Attributes()->mAccuracy * mPropPercent.mAccuracy / 10000);
		Attributes()->mDodge += static_cast<int>(1.0f * Attributes()->mDodge * mPropPercent.mDodge / 10000);
		Attributes()->mLuck += static_cast<int>(1.0f * Attributes()->mLuck * mPropPercent.mLuck / 10000);
	}

	void PlayActiveObject::ForceMove(int x,int y,int dir)
	{
		SetPosition(x,y);
		mDir = dir;
	}

	void PlayActiveObject::OnAttack(PlayActiveObject* des,SkillDef * def,int pawer,int flags,int delay)
	{
	}

	void PlayActiveObject::Attack(PlayActiveObject* des, SkillDef * sd,int pawer,int flags,int delay)
	{
		if( IsProperAttackDes(des, sd) )
		{
			// 刺杀剑术第二格必中
			if( ! CheckHit(des, sd) )
			{
				pawer = 0;
			}
			OnAttack(des, sd, pawer,flags,delay);
			//if (this->TypeIsPlayer() && des->TypeIsMonster())////玩家攻击怪物时
			//{
			//	DomainPlay* play = static_cast<DomainPlay*>(this);
			//	if (play)
			//	{
			//		if (play->GetUpGradeLv())////若有强化等级(每一级增加额外的25的怪物伤害)
			//		{
			//			pawer += play->GetUpGradeLv() * 25;
			//		}
			//	}
			//}
			des->Attacked(this,sd,pawer,flags,delay);
		}
	}
	bool PlayActiveObject::CheckHit(PlayActiveObject* des,SkillDef * sd)
	{
		// 刺杀必然命中
		if (sd->TypeID == SkillDef::SKILL_TYPE_CiShaJianShu)
		{
			return true;
		}

		// 命中
		int accu = Accuracy();
		int dodge = des->Dodge();
		if (dodge - accu <= 0)
		{
			return true;
		}

		double rate = (double)(dodge - accu) / dodge;
		int nRate = rate * 10000;
		if (rand() % 10000 < nRate)
		{
			return false;
		}

		return true;
	}
	bool PlayActiveObject::IsProperFriendDes(PlayActiveObject* des,int skill_type)
	{
		if( des == this )
		{
			return true;
		}
		return false;
	}
	bool PlayActiveObject::IsProperAttackDes(PlayActiveObject* des, SkillDef * sd)
	{
		if( des == this )
		{
			return false;
		}
		if( ! ( TypeIsNeutral() || TypeIsMonster() ) )
		{
			if( des->TypeIsPlayer() || des->TypeIsSlave() || des->TypeIsNeutral() || des->TypeIsDart())
			{
				if( des->SafeArea() > 0 )
				{
					return false;
				}
				if( SafeArea() > 0 )
				{
					return false;
				}
			}
		}

		if (des->TypeIsMonster())
		{
			PlayMonster * mon = static_cast<PlayMonster *>(des);
			if (mon->CollectTime() > 0)
				return false;
		}
		
		if (sd)
		{
			// 地狱雷光不打人和宝宝
			if( (des->TypeIsPlayer() || des->TypeIsSlave()) && sd->TypeID == SkillDef::SKILL_TYPE_DiYuLeiGuang )
				return false;

			// 野蛮不可撞
			if ((sd->TypeID == SkillDef::SKILL_TYPE_YeManChongZhuang || sd->TypeID == SkillDef::SKILL_TYPE_KangJuHuoHuan) && des->IsNoHit())
				return false;
		}
		return true;
	}

	int PlayActiveObject::GetAttackPawer(int base,int add)
	{
		int ret = 0;
		if( add < 0 ) add = 0;
		ret = base + RandRange(add+1);
		int luck = Attributes()->mLuck;
		if( luck > 0 )
		{
			/*if( RandRange(10-min(9,luck)) == 0 )
			{
				ret = base + add;
			}*/
			if (luck == 1 && rand() % 10000 < 1)
			{
				ret = base + add / 2;
			}
			else if(luck == 2 && rand() % 10000 < 25)
			{
				ret = base + add / 2;
			}
			else if(luck == 3 && rand() % 10000 < 50)
			{
				ret = base + add / 2;
			}
			else if(luck == 4 && rand() % 10000 < 100)
			{
				ret = base + add / 2;
			}
			else if(luck == 5 && rand() % 10000 < 500)
			{
				ret = base + add / 2;
			}
			else if(luck == 6 && rand() % 10000 < 1000)
			{
				ret = base + add / 2;
			}
			else if(luck == 7 && rand() % 10000 < 500)
			{
				ret = base + add;
			}
			else if(luck == 8 && rand() % 10000 < 1000)
			{
				ret = base + add;
			}
			else if(luck == 9 && rand() % 10000 < 10000)
			{
				ret = base + add;
			}
		}
		else
		{
			if( RandRange(10-min(9,-luck)) == 0 )
			{
				ret = base;
			}
		}
		return ret;
	}
	int PlayActiveObject::GetAttackPawerRange(int base,int maxp)
	{
		if( maxp <= base )
		{
			return GetAttackPawer(base,1);
		}
		return GetAttackPawer(base,maxp-base);
	}

	int PlayActiveObject::DefenseAttackPawer(PlayActiveObject* src,int pawer,int skill_type,int flags)
	{
		int defense = 0;

		if( skill_type == SkillDef::SKILL_TYPE_CiShaJianShu || skill_type == SkillDef::SKILL_TYPE_BanYueWanDao )
		{
			if(!TypeIsDart())
			{
				defense = 0;
			}
		}
		if (src->Attributes()->mIgnoreDef > 0)
		{
			defense = defense - defense*src->Attributes()->mIgnoreDef/10000;
		}

		int ret = max(0,pawer-defense);
		if( ret > 0 && IsPosionArmor() )
		{
			if (PosionArmorParam() > 0)
			{
				ret = max(0,pawer-(defense-PosionArmorParam()*defense/100));
			}
			/*float param = mGSL->mPosionArmorMul / 100.0f;
			ret = static_cast<int>(ret * param);*/
		}
		if( ret > 0 && IsMoFaDun() )
		{
			if( skill_type == SkillDef::SKILL_TYPE_CiShaJianShu )
			{
				int po = 0;
				ret = Round( static_cast<int>( ret * mGSL->mMofadunCishaParam / 100.0f*((10000+po)/10000.0f)));
			}
			else
			{
				ret -= Round(static_cast<int>((ret * (mStatusParam[STATUS_TYPE_MOFADUN] + 2) * mGSL->mMofadunParam) / 100.0f));///越来越小，受到的伤害越来越高
			}
			
			mStatusTimer[STATUS_TYPE_MOFADUN] -= 2.0;
		}

		return ret;
	}
	int  PlayActiveObject::Attacked(PlayActiveObject* src,SkillDef * sd,int pawer,int flags,int delay)
	{
		if (src->TypeIsMonster())///怪物打出的技能
		{
			OGSLib::PlayMonster* mon = (OGSLib::PlayMonster*)src;
			if (mon)
			{
				if (sd && sd->SkillID == SkillDef::Skill_Type_Monster_Mabi)
				{
					AddStatus(STATUS_TYPE_MABI, mon->GetMabiGap() * 1000, 1, 1, 1);
				}
				else if (sd && sd->SkillID == SkillDef::SKILL_TYPE_ShiDuShu)
				{
					AddStatus(STATUS_TYPE_POSION_HP, mon->GetPoisonTime(), mon->GetPoisonDamage(), 1, 1);///////持续性扣血
					AddStatus(STATUS_TYPE_POSION_ARMOR, mon->GetPoisonTime(), mon->GetPoisonDefense(), 1, 1);////玩家这段时间内将不能自动回血同时减防御(参数:时间,减多少防御百分比)
					SetPosioner(ID());
					SetPosionerName(Name());
				}
			}
		}
		if( !TypeIsDart() &&  pawer > 0 && src->TypeIsPlayer() && src->Attributes()->mMabi_prob > 0)
		{
			int randmb = (rand()%100) + 1;
			if (randmb < (src->Attributes()->mMabi_prob + 50)/100)
			{
				if(sd && (sd->SkillID == SkillDef::SKILL_TYPE_YiBanGongJi || sd->SkillID == SkillDef::SKILL_TYPE_JiChuJianShu || sd->SkillID == SkillDef::SKILL_TYPE_GongShaJianShu||sd->SkillID==SkillDef::SKILL_TYPE_CiShaJianShu))
				{
					if(abs(X()-src->X())<=2&&abs(Y()-src->Y())<=2 && Hp()>1 )
					{
						AddStatus(STATUS_TYPE_MABI,src->Attributes()->mMabi_dura * 1000,1,1,1);
					}
				}
			}
		}

		// 计算防御抵消伤害(基础伤害)
		int defence = CalculateDefencePower(sd);

		int damage = max(0,pawer-defence);
		
		// 计算技能抵消伤害,(魔法盾....)
		if (damage > 0)
			damage = CalculateSkillDefence(damage);

		// 计算PK增伤和减免
		if (this->TypeIsPlayer() && src->TypeIsPlayer())
		{
			damage = damage * (1.0f + 1.0 * (src->Attributes()->mPKIncrement - this->Attributes()->mPKReduce) / 10000);
			if (sd && (sd->DamType == 1 || sd->DamType == 0))
				damage = damage * (1.0f - 1.0 * (this->Attributes()->mPKReduceDC) / 10000);
			else
				damage = damage * (1.0f - 1.0 * (this->Attributes()->mPKReduceMC) / 10000);
		}

		// 增伤 免伤
		damage = damage * (1.0f + 1.0 * (src->Attributes()->mDamIncrement - this->Attributes()->mDamReduce) / 10000);
		damage = max(0,damage);

		if (sd && (sd->DamType == 1 || sd->DamType == 0))
			damage = damage * (1.0f - 1.0 * (this->Attributes()->mDamReduceDC) / 10000);
		else
			damage = damage * (1.0f - 1.0 * (this->Attributes()->mDamReduceMC) / 10000);

		if (sd->TypeID != SkillDef::SKILL_TYPE_CiShaJianShu 
			&& sd->TypeID != SkillDef::SKILL_TYPE_HuoQiang
			&& sd->TypeID != SkillDef::SKILL_TYPE_ShiDuShu
			&& sd->Shape != 6
			)
		{
/*
test
 			src->Attributes()->mBaojiProb = 1100;
 			src->Attributes()->mBaojiPres = 1000;
*/
			
			if (src->Attributes()->mBaojiProb - Attributes()->mTenacity > 0)
			{
				if((rand() % 10000) < (src->Attributes()->mBaojiProb - Attributes()->mTenacity) )
				{
					// 暴击
					damage += damage/2 + src->Attributes()->mBaojiPres;
					mAttackEffectFlags |= 0x01;
				}
			}
		}

		damage += src->Attributes()->mHolyDam;

		if(src->TypeIsSlave())
		{
			PlaySlave* ps = static_cast<PlaySlave*>(src);
			if(ps)
			{
				damage += ps->mFkAttPlus;
			}
		}

		if(this->TypeIsMonster())
		{
			//雕像特殊处理
			if(mActiveAttribute.mDamReduce>=10000)
			{
				damage = 0;
			}
		}

		if (sd->TypeID == SkillDef::SKILL_TYPE_Percent_Atk)
		{
			damage = Attributes()->mMaxHp * 1.0 * sd->Param0 / 10000;
		}

		if (this->TypeIsMonster())
		{
			damage = damage * (1.0f + 1.0 * src->Attributes()->mMonDamIncrement / 10000);

			if (src->TypeIsSlave() && static_cast<PlaySlave*>(src)->GetSlaveType() == Slave_type_huwei)
			{
				PlaySlave * slave = static_cast<PlaySlave*>(src);
				if (slave->GetSlaveType() == Slave_type_huwei)
				{
					int inc = 0;
					if (slave->Owner())
						inc = slave->Owner()->Attributes()->mZSMonDamIncrement;

					if (inc > 0)
						damage = damage * (1.0f + 1.0 * inc / 10000);
				}
			}
		}

		if( damage > 0 )
		{
			DamageHP(src,damage,delay);
		}
		return damage;
	}

	PlayActiveObject* PlayActiveObject::FindActiveInView(int id)
	{
		for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
			p != mVisibleInteractiveObjectList.end();
			++ p )
		{
			if ((*p))
			{
				if ((*p)->IsInteractive()
					&& (*p)->ID() == id
					&& (!(*p)->IsDead()))
				{
					return (PlayActiveObject*)*p;
				}
			}
		}
		return 0;
	}
	int PlayActiveObject::FindActiveInPosition(int x,int y,PlayActiveObject** objects,int max_slot)
	{
		if( max_slot < 1 ) return 0;
		if( !mParentMap ) return 0;
		int finded_num = 0;
		MapCell* cell = const_cast<MapCell*>(mParentMap->FindMapCell(x,y));
		if( cell )
		{
			for( PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();
				pos = pos->Next)
			{
				if( pos->This->IsInteractive() )
				{
					if (pos->This->TypeIsSlave())
					{
						PlaySlave* pSlave = dynamic_cast<PlaySlave*>(pos->This);
						if (pSlave && pSlave->Owner()->ID() == ID())
						{
							continue;
						}
					}
					objects[finded_num] = (PlayActiveObject*)pos->This;
					finded_num ++;
					if( ! ( finded_num < max_slot ) )
					{
						break;
					}
				}
			}
		}
		return finded_num;
	}
	int PlayActiveObject::FindMonInPosition(int x,int y)
	{
		if( !mParentMap ) return 0;
		int finded_num = 0;

		int posx = 0;
		int posy = 0;

		for(posx =x-1;posx <= x+1;posx ++)
		{
			for(posy =y-1;posy <= y+1;posy ++)
			{
				MapCell* cell = const_cast<MapCell*>(mParentMap->FindMapCell(posx,posy));
				if( cell )
				{
					for( PlayMapObjectNode* pos = cell->mObjList.begin();
						pos != cell->mObjList.end();
						pos = pos->Next)
					{

						if( pos->This->TypeIsMonster() )
						{
							finded_num ++;
						}
					}
				}
			}
		}
		return finded_num;
	}
	int PlayActiveObject::FindEmptyPosition(int *x,int *y)
	{
		if( !mParentMap ) return 0;
		int posx = 0;
		int posy = 0;

		for(posx =*x-1;posx <= *x+1;posx ++)

		{
			for(posy =*y-1;posy <= *y+1;posy ++)
			{
				MapCell* cell = const_cast<MapCell*>(mParentMap->FindMapCell(posx,posy));
				if( cell )
				{

					PlayMapObjectNode* pos = cell->mObjList.begin();

					if(pos == cell->mObjList.end())
					{
						*x=posx;
						*y=posy;
						return 1;
					}

				}
			}
		}

		return 0;
	}
	bool PlayActiveObject::IsPositionEmpty(int x, int y)
	{
		if( !mParentMap ) return false;
		MapCell* cell = const_cast<MapCell*>(mParentMap->FindMapCell(x,y));
		if( cell )
		{
			bool empty = cell->mObjList.empty();
			return cell->mObjList.empty();
		}
		return false;
	}
	bool PlayActiveObject::DamageHP(PlayActiveObject* src,int damage,int delay,bool safeAraeaEnable)
	{
		if( ! (damage>0) ) return false;
		if( IsDead() ) return false;
		int change_power = 0;
		if( Type() == MAP_OBJECT_TYPE_PLAYER )
		{
			if(!safeAraeaEnable && SafeArea() > 0 ) return false;
			if (mPower > 0)
			{
				/*int damageP = Attributes()->mMaxPower*Attributes()->mPowerPercent/10000;
				if (damage < damageP)
				{
					if (mPower > damage)
					{
						mPower -= damage;
						change_power = damage;
						damage = 0;
					}
					else
					{
						damage -= mPower;
						change_power = mPower;
						mPower = 0;
					}
				}
				else
				{
					if (mPower > damageP)
					{
						mPower -= damageP;
						change_power = damageP;
						damage -= damageP;
					}
					else
					{
						damage -= mPower;
						change_power = mPower;
						mPower = 0;
					}
				}*/
				//新的内功减伤按伤害比例算
				int damageP = damage * 1.0f * Attributes()->mPowerPercent / 10000;
				if (mPower > damageP)
				{
					mPower -= damageP;
					damage -= damageP;
					change_power = damageP;
				}
				else
				{
					damage -= mPower;
					change_power = mPower;
					mPower = 0;
				}

			}
			if(Attributes()->mDixiao_pres>0)
			{
				int damageM=damage*Attributes()->mDixiao_pres/10000;
				if( Mp()>damageM )
				{
					DamageMP(damageM);
					damage-=damageM;
				}
				else
				{
					if( Mp()>0 )
					{
						damageM=Mp();
						DamageMP(damageM);
						damage-=damageM;
					}
				}
			}
		}
		mHp -= damage;
		if( mHp > 0 )
		{
			OnHurt(src,-damage,delay,change_power);
		}
		else
		{
			mHp = 0;
			if (TypeIsMonster())
			{
				PlayMonster*mon = (PlayMonster*)this;
				if (mon->CollectTime()>0)
				{
					OnDead(src,delay);
					return true;
				}
			}
			OnHurt(src,-damage,delay,change_power);
			OnDead(src,delay);
		}
		return true;
	}
	int PlayActiveObject::AddAction(SubAction& action)
	{
		int ActionKey = find_action_key();
		action.mTime = time(0);
		mActionMap[ActionKey] = action;
		return ActionKey;
	}
	SubAction* PlayActiveObject::FindAction(int action_id)
	{
		ActionMap::iterator pos = mActionMap.find(action_id);
		if( pos != mActionMap.end() )
		{
			return &(pos->second);
		}
		return 0;
	}
	void PlayActiveObject::DelAction(int action_id)
	{
		mActionMap.erase(action_id);
		time_t tt = time(0);
		ActionMap::iterator it=mActionMap.begin();   
		while(it!=mActionMap.end())   
		{
			if( it->second.mTime + 100 < tt )
			{
				mActionMap.erase(it++);
			}
			else
			{
				++it;
			}
		}  
	}
	void PlayActiveObject::DropTheItem(SubItem* ii, int num, int ownerid, int pick_limit_lv, int owned_duration)
	{
		if( ! ii ) return;
		if( ! mParentMap ) return;
		int x,y;
		if( mParentMap->GenValidateMapItemPosition(X(),Y(),x,y) )
		{
			PlayMapItem* mi = new PlayMapItem(x,y,*ii);
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);
			mi->OwnerID(ownerid);
			if (id)
			{
				mi->ClothID(id->mIconID);
			}
			if (owned_duration == 0)
			{
				mi->OwnerID(0);
			}
			mi->Number(max(num,ii->mNumber));
			mi->SetPickLimitLv(pick_limit_lv);
			mi->ResetOwnerTime(owned_duration);
			mParentMap->AddObject(mi);
		}
	}

	void PlayActiveObject::DropTheItem(SubItem* ii, int num, std::vector<int>& owners, int pick_limit_lv, int owned_duration)
	{
		if( ! ii ) return;
		if( ! mParentMap ) return;
		int x,y;
		if( mParentMap->GenValidateMapItemPosition(X(),Y(),x,y) )
		{
			PlayMapItem* mi = new PlayMapItem(x,y,*ii);
			ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(ii->mTypeID);
			if (id)
			{
				mi->ClothID(id->mIconID);
			}

			if (owned_duration)
			{
				for (size_t i = 0; i < owners.size(); ++ i)
				{
					mi->PushOwnerID(owners[i]);
				}
			}
			mi->Number(max(num,ii->mNumber));
			mi->SetPickLimitLv(pick_limit_lv);
			mi->ResetOwnerTime(owned_duration);
			mParentMap->AddObject(mi);
		}
	}

	bool PlayActiveObject::AddHP(int add)
	{
		if( ! (add>0) ) return false;
		if( IsDead() ) return false;
		if( mHp >= MaxHp() )return false;
		mHp += add;
		if( mHp > MaxHp() )
		{
			mHp = MaxHp();
		}
		return true;
	}
	bool PlayActiveObject::DamageMP(int damage)
	{
		if( ! (damage>0) ) return false;
		if( IsDead() ) return false;
		if( mMp - damage < 0 )
		{
			return false;
		}
		mMp -= damage;
		return true;
	}
	bool PlayActiveObject::AddMP(int add)
	{
		if( ! (add>0) ) return false;
		if( IsDead() ) return false;
		if( mMp >= MaxMp() ) return false;
		mMp += add;
		if( mMp > MaxMp())
		{
			mMp = MaxMp();
		}
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void PlayActiveObject::ActionBanYueWanDaoBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		// 正前方算普通攻击
		int power = GetUseSkillPower(sd);
		AttackPosition(NextX(X(),mDir),NextY(Y(),mDir), SkillDef::SKILL_TYPE_YiBanGongJi, power,true,sd->Attack_Delay);

		int shift[3] = {-1, 1, 2};
		for (size_t i = 0; i < 3; ++ i)
		{
			int dir = NextDir(mDir, shift[i]);
			int x = NextX(X(), dir);
			int y = NextY(Y(), dir);
			AttackPosition(x,y,sd,ceil(power * 0.4f),true,sd->Attack_Delay);
		}
	}

	void PlayActiveObject::ActionYuanYueWanDaoBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int power = GetUseSkillPower(sd);
		// 获取影响范围半径
		int radius = sd->DamageRange;
		int x = X();
		int y = Y();
		for(int i = -radius; i <= radius; i++)
		{
			for(int j = -radius;j <= radius; j++)
			{
				if (i == 0 && j == 0)
					continue;

				AttackPosition(x+i,y+j,sd, power, false,sd->Attack_Delay);
			}
		}
	}
	void PlayActiveObject::ActionLiuXingHuoYuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID)
	{
		int paw = GetAttackPawer(sd->Pawer(sd->AdvPawer(),3) + Attributes()->mMC,Attributes()->mMCMax-Attributes()->mMC+1);
		paw = static_cast<int>(paw* mGSL->mSkillBingpaoxiaoScale);
		int pow = 0;
		int time = 0;
// 		if(sd->TypeID==SkillDef::SKILL_TYPE_HuoLongQiYan)
// 		{
// 			paw = paw + ss->mLevel*50;
// 			time = 3;
// 			pow = 10*ss->mLevel;
// 		}
// 		if(sd->TypeID==SkillDef::SKILL_TYPE_LiuXingHuoYu)
// 		{
// 			paw = paw + (ss->mLevel+3)*50;
// 			time = 3;
// 			pow = 30+10*ss->mLevel;
// 		}
		//paw = int(paw*sd->AttributePercentage/10000);
		if( sd)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
			for(int i = -2;i<3;i++)
			{
				for(int j = -2;j<3;j++)
				{
					PlayActiveObject* objects[5];
					bool attack_mon_one=false;
					int obj_num = FindActiveInPosition(targetx+i,targety+j,objects,5);
					for(int i = 0; i < obj_num; i++ )
					{
						PlayActiveObject* ob = objects[i];
						if(  IsProperAttackDes(ob,sd) && ob->IsInteractive() )
						{
							Attack(ob,sd,paw,0,sd->Attack_Delay);
							if(time>0 )
							{
								if (ob->IsDead()) {break;}
								ob->AddStatus(STATUS_TYPE_BURNING,30 * 1000,(time<<16)|pow,1,1);
								ob->SetBurner(ID());
							}
						}
					}
				}
			}
		}
	}

	void PlayActiveObject::ActionOfAOE1(SkillDef* sd,int paramX,int paramY,unsigned int* paramID)
	{
		InteractiveObjectVector v = mVisibleInteractiveObjectList;

		for( InteractiveObjectVector::iterator pos = v.begin();
			pos != v.end();
			++ pos )
		{
			if((*pos))
			{
				int paw = GetUseSkillPower(sd);

				Attack(*pos, sd, paw, 0, sd->Attack_Delay);
			}
		}
	}

	void PlayActiveObject::ActionYeManChongZhuangBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		//if( ss->mTick + mGSL->mYeManChongZhuangGap > mGSL->GetTimerFix()->Elapsed())return;
		int count = sd->DamageRange; //推出格子数量
		int pawer = GetUseSkillPower(sd);
		int target_x = X();
		int target_y = Y();
		PlayActiveObject* mo[20];
		int mo_num = 0;
		PlayActiveObject* objects[20];
		int tx = NextX(X(),mDir);
		int ty = NextY(Y(),Dir());
		bool bCanHit = true;
		int max_num = 1;
		int obj_num = 0;

		do 
		{
			// 先判断前一格能不能撞
			if(mParentMap->GetBlockData(tx,ty,2)!=0)
			{
				bCanHit = false;
				break;
			}
			obj_num = FindActiveInPosition(tx,ty,objects,20);
			if (obj_num > max_num)
			{
				bCanHit = false;
				break;
			}
				
			for(int o = 0;o < obj_num; o ++ )
			{
				PlayActiveObject* ob = objects[o];
				if( !IsProperAttackDes(ob, sd) && ob->level() < level())
				{
					bCanHit = false;
					break;
				}
			}
		} while (0);

		if (!bCanHit)
		{
			if (this->TypeIsPlayer())
			{
				DomainPlay * player = static_cast<DomainPlay *>(this);
				player->AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_SKILL_YEMAN_FALIED);
			}
			return;
		}

		int temp_x = 0;
		int temp_y = 0;
		for(int i = 0;i<count;i ++ )
		{
			if(mParentMap->GetBlockData(tx,ty,2)!=0) break;
			obj_num = FindActiveInPosition(tx,ty,objects,20);
			bool stop=false;
			if (obj_num > max_num)
			{
				stop = true;
			}
			if( ! stop )
			{
				for(int o = 0;o < obj_num;o ++ )
				{
					PlayActiveObject* ob = objects[o];
					if( IsProperAttackDes(ob, sd) && ob->level() < level())
					{
						if( mo_num < max_num )
						{
							mo[mo_num++] = ob;
						}
					}
					else 
					{
						stop = true;
						break;
					}
					if( mo_num >= max_num )
					{
						break;
					}
				}
			}
			if( stop ) 
				break;

			temp_x = NextX(tx,Dir());
			temp_y = NextY(ty,Dir());
			if (IsPositionEmpty(temp_x, temp_y) || mo_num < max_num)
			{
				target_x = tx;
				target_y = ty;
				tx = temp_x;
				ty = temp_y;
			}
			else 
				break;
		}

		//被撞单位目标点
		int mx = NextX(target_x,Dir());
		int my = NextY(target_y,Dir());
		if(mParentMap->GetBlockData(mx,my,2)!=0)
		{
			mx = target_x;
			my = target_y;
		}
		for(int m=0;m<mo_num;m++)
		{
			PlayActiveObject* ob = mo[m];
			ob->Dir((Dir()+4)%8);
			ob->ForceMove(mx,my,ob->Dir());
			ob->ResRun(ob->Dir(),ob->X(),ob->Y());
			for( InteractiveObjectVector::iterator pos = ob->mVisibleInteractiveObjectList.begin();
				pos != ob->mVisibleInteractiveObjectList.end();
				++ pos )
			{
				if((*pos))
				{
					(*pos)->NotifyRun(ob,ob->Dir(),ob->X(),ob->Y());
				}
			}
			Attack(ob,sd,pawer,0,sd->Attack_Delay);
			ob->Mabi(sd, sd->Param0);
		}

		//(边界)自己和目标坐标重叠，修正自己坐标后退一个
		if (mx == target_x && my == target_y)
		{
			int dir2 = (Dir() + 8 + 4) % 8; //反向
			target_x = NextX(target_x, dir2);
			target_y = NextY(target_y, dir2);
		}

		PlayActiveObject::ForceMove(target_x,target_y,Dir());
		ResRun(Dir(),target_x,target_y);

		for( InteractiveObjectVector::iterator pos = mVisibleInteractiveObjectList.begin();
			pos != mVisibleInteractiveObjectList.end();
			++ pos )
		{
			if((*pos))
			{
				(*pos)->NotifyRun(this,Dir(),target_x,target_y);
			}
		}
		// publicPlayEffect(sd->EffectType,X(),Y(),ID(),ID(),sd->Effect_ResID);
	}
	void PlayActiveObject::ActionYiBanGongJiBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int paw = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);
		bool attacked=false;
		if( ! attacked )
		{
			AttackPosition(NextX(X(),mDir),NextY(Y(),mDir),sd,paw,true,sd->Attack_Delay);
		}
	}
	void PlayActiveObject::ActionWizardNormalBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int paw = GetAttackPawerRange(Attributes()->mMC,Attributes()->mMCMax);
		AttackPosition(NextX(X(),mDir),NextY(Y(),mDir),sd,paw,true,sd->Attack_Delay);
	}
	void PlayActiveObject::ActionTaoistNormalBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int paw = GetAttackPawerRange(Attributes()->mSCMax,Attributes()->mSCMax);
		//paw = static_cast<int>(1.5f*paw);
		AttackPosition(NextX(X(),mDir),NextY(Y(),mDir),sd,paw,true,sd->Attack_Delay);
	}
	void PlayActiveObject::ActionGongShaJianShuBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int pawer = GetAttackPawerRange(Attributes()->mDC,Attributes()->mDCMax);
		AttackPosition(NextX(X(),mDir),NextY(Y(),mDir),sd,pawer,true,sd->Attack_Delay);

	}

	void PlayActiveObject::ActionCiShaJianShuBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int power = GetUseSkillPower(sd);
		int x = NextX(X(),mDir);
		int y = NextY(Y(),mDir);
		//邻位
		AttackPosition(x,y, SkillDef::SKILL_TYPE_YiBanGongJi, power,true,sd->Attack_Delay);
		//隔位(修改为16方向)
		if (!(x == paramX && y == paramY))
		{
			x = NextX(x,mDir);
			y = NextY(y,mDir);
			if (std::abs(paramX - x) + std::abs(paramY - y) <= 1)
			{
				AttackPosition(paramX,paramY,sd,power,true,sd->Attack_Delay);
			}
			else
			{
				AttackPosition(x,y,sd,power,true,sd->Attack_Delay);
			}
		}
		else
		{
			AttackPosition(NextX(x,mDir),NextY(y,mDir),sd,power,true,sd->Attack_Delay);
		}
	}

	void PlayActiveObject::ActionLieHuoJianFaBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID)
	{
		int power = GetUseSkillPower(sd);

		// 根据技能配置,方向上N个格子进行攻击,适应怪物技能
		int x = NextX(X(), mDir);
		int y = NextY(Y(), mDir);
		for (int i = 0; i < sd->DamageRange; ++ i)
		{
			AttackPosition(x,y, sd, power, true, sd->Attack_Delay);
			x = NextX(x, mDir);
			y = NextY(y, mDir);
		}
	}

	void PlayActiveObject::ActionHuoQiuShuBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn/* = true*/)
	{
		if( paramID == 0 ) return ;
		if (need_turn)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}
		PlayActiveObject* des = FindActiveInView(paramID);
		if( des )
		{
			float time = ((abs(des->X()-X()) + abs(des->Y()-Y()))/50.0f)/0.05f;
			if( abs(des->X()-targetx) + abs(des->Y()-targety) <= 2 )
			{
				if( sd )
				{
					int paw = GetUseSkillPower(sd);
					Attack(des, sd, paw, 0, (23+(int)time)*40);
				}
			}
			//TrainSkill(SkillDef::SKILL_TYPE_HuoQiuShu,RandRange(3)+1);
		}
	}

	void PlayActiveObject::ActionKangJuHuoHuanBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		std::map<int, PlayActiveObject *> map_des;

		for(int dir = 0;dir < 8 ;dir ++ )
		{
			int x = NextX(X(),dir);
			int y = NextY(Y(),dir);
			PlayActiveObject* objects[20];
			int obj_num = FindActiveInPosition(x,y,objects,20);
			if( obj_num == 1 )
			{
				if (objects[0] && IsProperAttackDes(objects[0],sd) && objects[0]->level() < level())
				{
					map_des[dir] = objects[0];
				}
			}
		}

		for (std::map<int, PlayActiveObject *>::iterator it = map_des.begin(); it != map_des.end(); ++ it)
		{
			PlayActiveObject * ob = it->second;
			int x = ob->X();
			int y = ob->Y();
			for (int i = 0; i < sd->Param0; ++ i)
			{
				int nx = NextX(x, it->first);
				int ny = NextY(y, it->first);
				if (0 == mParentMap->GetBlockData(nx,ny,1))
				{
					x = nx;
					y = ny;
				}
				else
					break;
			}
			if (x != ob->X() || y != ob->Y())
			{
				ob->ForceMove(x,y,ob->Dir());
			}
		}
	}

	void PlayActiveObject::ActionDiYuHuoBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		SubAction ai;
		ai.mType = sd->TypeID;
		ai.mData.attackCount.targetx = X();
		ai.mData.attackCount.targety = Y();
		ai.mData.attackCount.target_id = Diff2Dir(static_cast<float>(targetx-X()),static_cast<float>(targety-Y()));
		ai.mData.attackCount.count = 5;
		int ActionKey = AddAction(ai);
		mGSL->GetFrameTimer()->AddTimer(20,this,ActionKey);
		//TODO
		Turn(ai.mData.attackCount.target_id);
	}

	void PlayActiveObject::ActionLeiDianShuBegin(SkillDef* sd,int* ptargetx,int* ptargety,unsigned int paramID, bool need_turn/* = true*/)
	{
		int targetx = *ptargetx;
		int targety = *ptargety;
		if( paramID == 0 ) return ;
		if (need_turn)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}

		PlayActiveObject* des = FindActiveInView(paramID);
		if( des )
		{
			*ptargetx = des->X();
			*ptargety = des->Y();
			//float time = ((abs(des->X()-X()) + abs(des->Y()-Y()))/50.0f)/0.05f;
			if( abs(des->X()-targetx) + abs(des->Y()-targety) <= 2 )
			{
				int power = GetUseSkillPower(sd);
				Attack(des, sd, power,0, sd->Attack_Delay);
			}
		}
	}

	void PlayActiveObject::ActionShunJianYiDongBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		if( sd )
		{
			if( RandRange(11) < (sd->Level*2 + 4) )
			{
				SubAction ai;
				ai.mType = SkillDef::SKILL_TYPE_ShunJianYiDong;
				ActionInstanceData* aid = &ai.mData;
				aid->attack.targetx = targetx;
				aid->attack.targety = targety;
				aid->attack.target_id = paramID;
				int ActionKey = AddAction(ai);
				mGSL->GetFrameTimer()->AddTimer(18,this,ActionKey);
				//TODO
			}
			else
			{
				if (TypeIsPlayer())
				{
					((DomainPlay*)this)->AlertMessage(10,0,mGSL->mStringConsts.NOTIFY_SKILL_SHUNJIANYIDONG_FAILED);
				}
			}
		}
	}
	void PlayActiveObject::ActionDaHuoQiuBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn/* = true*/)
	{
		if( paramID == 0 ) return ;
		if (need_turn)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}

		PlayActiveObject* des = FindActiveInView(paramID);
		if( des )
		{
			float time = ((abs(des->X()-X()) + abs(des->Y()-Y()))/50.0f)/0.05f;
			if( abs(des->X()-targetx) + abs(des->Y()-targety) <= 2 )
			{
				if( sd )
				{
					int paw = GetUseSkillPower(sd);
					Attack(des,sd,paw,0,(23+(int)time)*40);
				}
			}
			//TrainSkill(SkillDef::SKILL_TYPE_DaHuoQiu,RandRange(3)+1);
		}
	}
	void PlayActiveObject::ActionBaoLieHuoYanBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn/* = true*/)
	{
		if (need_turn)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}

		for(int i = -1;i<2;i++)
		{
			for(int j = -1;j<2;j++)
			{
				int paw = GetUseSkillPower(sd);
				AttackPosition(targetx+i,targety+j,sd,paw,false,16*40);
			}
		}
	}

	void PlayActiveObject::ActionHuoQiangBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn /*= true*/)
	{
		static const int x_diff[] = {
			0,-1,0,1,0,
			-1,-1,0,1,1,
			0,-1,0,1,0,
			-1,-1,0,1,1,
			0,-1,0,1,0,
			-1,-1,0,1,1,
			0,-1,0,1,0,
			-1,-1,0,1,1,
		};
		static const int y_diff[] = {
			-1,0,0,0,1,
			1,-1,0,1,-1,
			-1,0,0,0,1,
			1,-1,0,1,-1,
			-1,0,0,0,1,
			1,-1,0,1,-1,
			-1,0,0,0,1,
			1,-1,0,1,-1,
		};

		if (need_turn)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}

		int paw = GetUseSkillPower(sd);

		TrapDef def;
		def.id = eTrapIdHuoQiang;
		// 外观id
		def.cloth = sd->Effect_ResID+1;
		def.damage = paw;
		def.skill_type = sd->SKILL_TYPE_HuoQiang;
		def.effect_id = 0;
		def.damage_gap = 1800;
		def.caller_leave_remove = true;
		def.life_time = sd->Param0 / 1000;

		// DamageRange==5特殊处理
		if (sd->DamageRange == 5)
		{
			for(int i = 0;i<5;i++)
			{
				int x = targetx + x_diff[i];
				int y = targety + y_diff[i];

				mGSL->GetMapManager()->GenTrap(GetMap(), x, y, &def, this);
			}
			return;
		}
		
		int nTrapSet = 0;
		// 目标位置生成一坨
		const MapCell * cell = mParentMap->FindMapCell(targetx, targety);
		if (cell && cell->MapTrap_Num == 0)
		{
			mGSL->GetMapManager()->GenTrap(GetMap(), targetx, targety, &def, this);

			++ nTrapSet;
		}
		
		if (nTrapSet >= sd->DamageRange)
		{
			return;
		}

		for (int i = -1; i < 2; ++ i)
		{
			// 九个格子顺时针放置
			for (int j = -1; j < 2; ++ j)
			{
				if (i == 0 && j == 0)
				{
					continue;
				}

				const MapCell * cell = mParentMap->FindMapCell(targetx + i, targety + j);
				if (cell && cell->MapTrap_Num == 0)
				{
					mGSL->GetMapManager()->GenTrap(GetMap(), targetx + i, targety + j, &def, this);
					++ nTrapSet;
					if (nTrapSet >= sd->DamageRange)
					{
						return;
					}
				}
			}
		}
	}

	void PlayActiveObject::ActionJiGuangDianYingBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID,  bool need_turn/* = true*/)
	{
		if (need_turn)
		{
			int dir = Diff2Dir(static_cast<float>(targetx-X()),static_cast<float>(targety-Y()));
			Turn(dir);
		}

		if( ! (sd) )
		{
			return;
		}
		int x = X();
		int y = Y();
		for(int i = 0;i<5;i++)
		{
			x = NextX(x,mDir);
			y = NextY(y,mDir);
			int paw = GetUseSkillPower(sd);
			AttackPosition(x,y,sd,paw,true,sd->Attack_Delay);
		}
		//TrainSkill(SkillDef::SKILL_TYPE_JiGuangDianYing,RandRange(3)+1);
	}
	void PlayActiveObject::ActionDiYuLeiGuangBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		int power = GetUseSkillPower(sd);
		// 获取影响范围半径
		int radius = sd->DamageRange;
		int x = X();
		int y = Y();
		for(int i = -radius; i <= radius; i++)
		{
			for(int j = -radius;j <= radius; j++)
			{
				if (i == 0 && j == 0)
					continue;

				AttackPosition(x+i,y+j,sd, power, false,sd->Attack_Delay);
			}
		}

		//TrainSkill(SkillDef::SKILL_TYPE_DiYuLeiGuang,RandRange(3)+1);
	}
	void PlayActiveObject::ActionMoFaDunBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID)
	{
		*paramID = 0;
		if( sd)
		{
			int paw = GetUseSkillPower(sd);
			MoFaDun(paw, sd);
		}
	}
	void PlayActiveObject::ActionBingPaoXiaoBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn/* = true*/)
	{
		if( sd)
		{
			if (need_turn)
			{
				int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
				Turn(dir);
			}

			// 获取影响范围半径
			int radius = sd->DamageRange;
			for(int i = -radius; i <= radius; i++)
			{
				for(int j = -radius;j <= radius; j++)
				{
// 					int paw = GetAttackPawer(sd->Pawer(sd->AdvPawer(),ss->mLevel) + Attributes()->mMC,Attributes()->mMCMax-Attributes()->mMC+1);
// 					paw = static_cast<int>(paw* mGSL->mSkillBingpaoxiaoScale);
					int power = GetUseSkillPower(sd);
					AttackPosition(targetx+i,targety+j,sd,power,true,sd->Attack_Delay);
				}
			}
		}
		//TrainSkill(SkillDef::SKILL_TYPE_BingPaoXiao,RandRange(3)+1);
	}
	void PlayActiveObject::ActionZhiYuShuBegin(SkillDef* sd, int targetx,int targety,unsigned int* paramID, bool need_turn /*= true*/)
	{
		if (need_turn)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}

		if( sd)
		{
			PlayActiveObject* des = 0;
			if( paramID ) des = FindActiveInView(*paramID);
			if( ! des ) des = this;
			if( ! IsProperFriendDes(des,sd->TypeID) )
			{
				des = this;
			}
			*paramID=0;
			if( des && IsProperFriendDes(des,sd->TypeID) )
			{
				*paramID = des->ID();if(*paramID==0){*paramID=ID();}
				int paw = GetUseSkillPower(sd);
				if(paw >4000){paw = 4000;}
				des->AddIncMagicHP(paw);
			}
		}
		//TrainSkill(SkillDef::SKILL_TYPE_ZhiYuShu,RandRange(3)+1);
	}
	void PlayActiveObject::ActionShiDuShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn/* = true*/)
	{
		if (need_turn)
		{	
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}

		PlayActiveObject* des = 0;

		int radius = sd->DamageRange;
		for(int i = -radius; i<=radius; i++)
		{
			for(int j = -radius; j<=radius; j++)
			{
				PlayActiveObject* objects[20];
				int obj_num = FindActiveInPosition(targetx+i,targety+j,objects,20);
				for(int k = 0; k < obj_num; k++ )
				{
					des = objects[k];
					if( des && IsProperAttackDes(des, sd))
					{
						int power = GetUseSkillPower(sd);
						int time = 40 * 1000;

						if (des->TypeIsPlayer() && this->TypeIsPlayer())
						{
							// 计算pk 增伤免伤
							power = power * (1.0f + 1.0f * (this->Attributes()->mPKIncrement - des->Attributes()->mPKReduce) / 10000);
							if (sd && (sd->DamType == 1 || sd->DamType == 0))
								power = power * (1.0f - 1.0 * (this->Attributes()->mPKReduceDC) / 10000);
							else
								power = power * (1.0f - 1.0 * (this->Attributes()->mPKReduceMC) / 10000);
						}
						des->AddBuff(sd->TriggerBuff1);
						des->AddStatus(STATUS_TYPE_POSION_HP,time,power,2.0,1);
						des->AddStatus(STATUS_TYPE_POSION_ARMOR,time,power,2.0,1);
						des->SetPosioner(this);
						des->SetPosionerName(Name());
						Attack(des, sd, 0, 0, sd->Delay);
					}
				}
			}
		}
	}

	void PlayActiveObject::ActionLingHunHuoFuBegin(SkillDef* sd,int* ptargetx,int* ptargety,unsigned int paramID, bool need_turn /*= true*/)
	{
		int targetx = * ptargetx;
		int targety = * ptargety;
		if( paramID == 0 ) return ;
		PlayActiveObject* des = FindActiveInView(paramID);
		if( des )
		{
			*ptargetx = des->X();
			*ptargety = des->Y();
			targetx = * ptargetx;
			targety = * ptargety;
			float time = ((abs(des->X()-X()) + abs(des->Y()-Y()))/50.0f)/0.05f;
			if( abs(des->X()-targetx) + abs(des->Y()-targety) <= 2 )
			{
				if( sd)
				{
					int power = GetUseSkillPower(sd);
					Attack(des, sd, power, 0, (23+(int)time)*40);
				}
			}
		}
		if (need_turn)
		{
			int dir = Diff2Dir((float)(*ptargetx)-X(),(float)(*ptargety)-Y());
			Turn(dir);
		}
	}
	void PlayActiveObject::ActionYinShenShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID)
	{
		*paramID = 0;
		if( sd)
		{
			int paw = sd->Pawer13(30,sd->Level) + RandRange2(Attributes()->mSC,Attributes()->mSCMax)*3;
			if( YingShen(paw,sd->Level) ) 
			{
				*paramID = static_cast<unsigned int>(paw);
			}
		}
		//TrainSkill(SkillDef::SKILL_TYPE_YinShenShu,RandRange(3)+1);
	}
	void PlayActiveObject::ActionJiTiYinShenShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn/* = true*/)
	{
		PlayActiveObject* des = 0;

		if (need_turn)
		{	
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}
		int radius = sd->DamageRange;
		for(int i = -radius; i<=radius; i++)
		{
			for(int j = -radius; j<=radius; j++)
			{
				PlayActiveObject* objects[20];
				int obj_num = FindActiveInPosition(targetx+i,targety+j,objects,20);
				for(int k = 0; k < obj_num; k++ )
				{
					des = objects[k];
					if( des && IsProperFriendDes(des,sd->TypeID) && des->TypeIsPlayer() )
					{
						des->AddBuff(sd->TriggerBuff1);
						des->YingShen(sd->Param0 / 1000,sd->Level);
					}
				}
			}
		}
		//TrainSkill(SkillDef::SKILL_TYPE_JiTiYinShenShu,RandRange(3)+1);
	}
	void PlayActiveObject::ActionYouLingDunShenShengZhanJiaShuHelper(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn/* = true*/)
	{
		if( sd )
		{
			if (need_turn)
			{
				int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
				Turn(dir);
			}

			PlayActiveObject* des = 0;
// 			int paw = sd->Pawer13(60,sd->Level) + Attributes()->mSC/5 + RandRange2(Attributes()->mSC,Attributes()->mSCMax);
// 			paw = mGSL->mYouLinShenshenMul * paw / 1000;
// 			if( paw > 20*60 ){paw = 20*60;}
// 			*paramID = static_cast<unsigned int>(paw);
// 			int pow = 20 * sd->Level +10;
			//if( mGSL->mSystemEngineType == SYSTEM_ENGINE_WUZUN )
			//{
			//	pow = 5 * ss->mLevel;
			//}
			int radius = sd->DamageRange;
			for(int i = -radius; i<=radius;i++)
			{
				for(int j = -radius;j<radius;j++)
				{
					if( targetx+i == X() && targety+j == Y() )
					{
						int add = 2 + level()/7;
						AddBuff(sd->TriggerBuff1);
						YouLingDun(sd->Param0 / 1000, sd->SkillAddPower);
						ShenShengZhanJiaShu(sd->Param0 / 1000, sd->SkillAddPower);
					}
					PlayActiveObject* objects[20];
					int obj_num = FindActiveInPosition(targetx+i,targety+j,objects,20);
					for(int k = 0; k < obj_num; k++ )
					{
						des = objects[k];
						if( des && IsProperFriendDes(des,sd->TypeID) )
						{
							if( des->TypeIsPlayer() )
							{
								DomainPlay* desp = static_cast<DomainPlay*>(des);
								int add = 2 + level()/7;
								des->AddBuff(sd->TriggerBuff1);
								des->YouLingDun(sd->Param0 / 1000, sd->SkillAddPower);
								des->ShenShengZhanJiaShu(sd->Param0 / 1000, sd->SkillAddPower);
							}
						}
					}
				}
			}
		}
	}
	void PlayActiveObject::ActionYouLingDunBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn/* = true*/)
	{
		ActionYouLingDunShenShengZhanJiaShuHelper(sd,targetx,targety,paramID, need_turn);
		//TrainSkill(SkillDef::SKILL_TYPE_YouLingDun,RandRange(3)+1);
	}
	void PlayActiveObject::ActionShenShengZhanJiaShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn/* = true*/)
	{
		ActionYouLingDunShenShengZhanJiaShuHelper(sd,targetx,targety,paramID, need_turn);
		//TrainSkill(SkillDef::SKILL_TYPE_ShenShengZhanJiaShu,RandRange(3)+1);
	}
	void PlayActiveObject::ActionQunTiZhiLiaoBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID,int need_turn/* = true*/)
	{
		if (need_turn)
		{
			int dir = Diff2Dir((float)targetx-X(),(float)targety-Y());
			Turn(dir);
		}

		PlayActiveObject* des = 0;
		int paw = GetAttackPawer(sd->Pawer(sd->AdvPawer(),sd->Level) + Attributes()->mSC*2,(Attributes()->mSCMax-Attributes()->mSC)*2+1)+level();
		if (paw >4000){paw = 4000;}
			
		int radius = sd->DamageRange;
		for(int i = -radius; i <= radius; i++)
		{
			for(int j = -radius ;j <= radius; j++)
			{
				//if( targetx+i == X() && targety+j == Y() )
				//{
				//	AddIncMagicHP(paw);
				//}
				PlayActiveObject* objects[20];
				int obj_num = FindActiveInPosition(targetx+i,targety+j,objects,20);
				for(int k = 0; k < obj_num; k++ )
				{
					des = objects[k];
					if( des && IsProperFriendDes(des,sd->TypeID) )
					{
						des->AddIncMagicHP(paw);
					}
				}
			}
		}
		//TrainSkill(SkillDef::SKILL_TYPE_QunTiZhiLiao,RandRange(3)+1);
	}
	void PlayActiveObject::ActionPickBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		PlayActiveObject* des = FindActiveInView(paramID);
		if( des && IsProperAttackDes(des,sd)  )
		{
			int dir = Diff2Dir((float)des->X()-X(),(float)des->Y()-Y());
			int x = NextX(X(),dir);int y = NextY(Y(),dir);
			if( mParentMap && mParentMap->GetBlockData(x,y,2) > 0 )
			{
				x = NextX(X(),(dir+4)%8);y = NextY(Y(),(dir+4)%8);
				if( mParentMap && mParentMap->GetBlockData(x,y,2) > 0 )
				{
					x = X();
					y = Y();
				}
			}
			des->ForceMove(x,y,(dir+4)%8);
		}
	}
	void PlayActiveObject::ActionCollectBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		if( this->TypeIsPlayer() )
		{
		}
		if( this->TypeIsMonster() )
		{
			for(InteractiveObjectVector::iterator pos = this->mVisibleInteractiveObjectList.begin();
				pos != this->mVisibleInteractiveObjectList.end();
				++ pos)
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsPlayer() && IsProperAttackDes(*pos, sd))
					{
						int dir = Diff2Dir((float)(*pos)->X() - X(), (float)(*pos)->Y() - Y());
						int x = NextX(X(), dir); int y = NextY(Y(), dir);
						if (mParentMap && mParentMap->GetBlockData(x, y, 2) > 0)
						{
							x = NextX(X(), (dir + 4) % 8); y = NextY(Y(), (dir + 4) % 8);
							if (mParentMap && mParentMap->GetBlockData(x, y, 2) > 0)
							{
								x = X();
								y = Y();
							}
						}
						(*pos)->ForceMove(x, y, (dir + 4) % 8);
					}
				}
			}
		}
	}
	void PlayActiveObject::ActionPushBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		PlayActiveObject* des = FindActiveInView(paramID);
		if( des && IsProperAttackDes(des, sd)  )
		{
			for(int i = 0;i<sd->Level;i++)
			{
				int dir = Diff2Dir((float)des->X()-X(),(float)des->Y()-Y());
				int x = NextX(des->X(),dir);int y = NextY(des->Y(),dir);
				if( mParentMap && mParentMap->GetBlockData(x,y,2) <= 0 )
				{
					des->ForceMove(x,y,(dir+4)%8);
				}
			}
		}
	}
	void PlayActiveObject::ActionPushAwayBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID)
	{
		if( this->TypeIsMonster() )
		{
			for(InteractiveObjectVector::iterator pos = this->mVisibleInteractiveObjectList.begin();
				pos != this->mVisibleInteractiveObjectList.end();
				++ pos)
			{
				if ((*pos))
				{
					if ((*pos)->TypeIsPlayer() && IsProperAttackDes(*pos, sd))
					{
						for (int i = 0; i < sd->Level; i++)
						{
							int dir = Diff2Dir((float)(*pos)->X() - X(), (float)(*pos)->Y() - Y());
							int x = NextX((*pos)->X(), dir); int y = NextY((*pos)->Y(), dir);
							if (mParentMap && mParentMap->GetBlockData(x, y, 2) <= 0)
							{
								(*pos)->ForceMove(x, y, (dir + 4) % 8);
							}
						}
					}
				}
			}
		}
	}
	SubSkill* PlayActiveObject::FindSkill(int type_id)
	{
		for( SkillMap::iterator pos = GetSkillList().begin();
			pos != GetSkillList().end();
			++ pos )
		{
			if( pos->mTypeID == type_id )
			{
				return &(*pos);
			}
		}
		return 0;
	}
	SubSkill* PlayActiveObject::RemoveSkill(int type_id)
	{
		for( SkillMap::iterator pos = GetSkillList().begin();
			pos != GetSkillList().end();
			++ pos )
		{
			if( pos->mTypeID == type_id )
			{
				GetSkillList().erase(pos);break;
			}
		}
		return 0;
	}

	int PlayActiveObject::StatusFlags(int status_id)
	{
		if( IsStatus(status_id) )
		{
			return mStatusFlags[status_id];
		}
		return 0;
	}

	void PlayActiveObject::IncCallTrap(PlayMapTrap * trapCalled)
	{
		mTrapsCalled.push_back(trapCalled);
	}

	void PlayActiveObject::DecCallTrap(PlayMapTrap * trapCalled)
	{
		std::vector<PlayMapTrap *>::iterator it = std::find(mTrapsCalled.begin(), mTrapsCalled.end(), trapCalled);
		if (it != mTrapsCalled.end())
		{
			mTrapsCalled.erase(it);
		}
	}

	void PlayActiveObject::OnDead(PlayActiveObject* src,int delay)
	{
		// 移除所有陷阱依赖关系
		if (mTrapsCalled.size() > 0)
		{
			for (size_t i = 0; i < mTrapsCalled.size(); ++ i)
			{
				mTrapsCalled[i]->ClearCaller();
				if (mTrapsCalled[i]->GetCallerLeaveRemove())
					mTrapsCalled[i]->RemoveFromMap();	
			}
			mTrapsCalled.clear();
		}
	}

	void PlayActiveObject::NotifyMapLeave(PlayMap* map)
	{
		if (mTrapsCalled.size() > 0)
		{
			for (size_t i = 0; i < mTrapsCalled.size(); ++ i)
			{
				mTrapsCalled[i]->ClearCaller();
				if (mTrapsCalled[i]->GetCallerLeaveRemove())
					mTrapsCalled[i]->RemoveFromMap();	
			}
			mTrapsCalled.clear();
		}

		PlayMapObject::NotifyMapLeave(map);
	}

	int PlayActiveObject::AddSkill(int type_id, int level)
	{
		for( SkillMap::iterator pos = GetSkillList().begin();
			pos != GetSkillList().end();
			++ pos )
		{
			if( pos->mTypeID == type_id )
			{
				return 1;
			}
		}
		SubSkill skill;
		skill.mTypeID = type_id;
		skill.mLevel = level;
		skill.mExp = 0;
		GetSkillList().push_back(skill);
		return 0;
	}

	int PlayActiveObject::GetUseSkillPower(SkillDef * pDef)
	{
		int jobDC = 0;
		int jobDCMax = 0;
		
		if (pDef->DamType == 1 || pDef->DamType == 0)
		{
			// 物理伤害
			jobDC = Attributes()->mDC;
			jobDCMax = Attributes()->mDCMax;
		}
		else if (pDef->DamType == 2)
		{
			// 魔法伤害
			jobDC = Attributes()->mMC;
			jobDCMax = Attributes()->mMCMax;
		}
		else if (pDef->DamType == 3)
		{
			jobDC = Attributes()->mSC;
			jobDCMax = Attributes()->mSCMax;
		}
		int power = GetAttackPawerRange(jobDC, jobDCMax);
		power = static_cast<int>(power * (1.0f * pDef->SkillAttackFactor/10000) + pDef->SkillAddPower);

		return power;
	}
	
	int PlayActiveObject::CalculateDefencePower(SkillDef * pDef)
	{
		int jobAC = 0;
		int jobACMax = 0;

		if (pDef && (pDef->DamType == 1 || pDef->DamType == 0))
		{
			// 物理伤害
			jobAC = Attributes()->mAC;
			jobACMax = Attributes()->mACMax;
		}
		else if (pDef && (pDef->DamType == 2 || pDef->DamType == 3))
		{
			// 魔法伤害
			jobAC = Attributes()->mMAC;
			jobACMax = Attributes()->mMACMax;
		}

		int defence = jobAC + RandRange(jobACMax - jobAC);
		defence = static_cast<int>(defence * (1.0f * pDef->DefenseDamageFactor/10000));
		
		return defence;
	}

	int PlayActiveObject::CalculateSkillDefence(int power)
	{
		// 魔法盾
		if (IsMoFaDun())
		{
			SubSkill* ss = FindSkill(SkillDef::SKILL_TYPE_MoFaDun);
			if(!ss)
				return power;

			SkillDef* sd = mGSL->GetSkillDefineManager()->GetSkillDefine(SkillDef::SKILL_TYPE_MoFaDun, ss->mLevel);

			if (!sd)
			{
				return power;
			}

			int def = power * 1.0 * sd->SkillAttackFactor/10000;
			power = power - def;
		}

		return power;
	}

	bool PlayActiveObject::HaveBuffByType(int type)
	{
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if (it->buff_info.mBuffType == type)
			{
				return true;
			}
			++ it;
		}
		return false;
	}

	BuffList & PlayActiveObject::GetBuffList()
	{
		return m_buffs;
	}

	bool PlayActiveObject::IsNoHit()
	{
		return false;
	}

	void PlayActiveObject::SetPosioner(PlayActiveObject * src)
	{
		if (src)
			mPosioner = src->ID();
	}

	void PlayActiveObject::EnableBuff(int id, bool enable)
	{
		BuffList::iterator it = m_buffs.begin();
		while (it != m_buffs.end())
		{
			if (it->buff_info.mBuffID == id)
			{
				it->enable = enable;

				int op = 3;
				if (enable == true)
					op = 4;
				NotifyBuffChange(this, id, op, 0);
				for( InteractiveObjectVector::iterator p = mVisibleInteractiveObjectList.begin();
					p != mVisibleInteractiveObjectList.end();
					++ p )
				{
					if ((*p))
					{
						(*p)->NotifyBuffChange(this, id, op, 0);
					}
				}
				return;
			}
			++ it;
		}
	}

}