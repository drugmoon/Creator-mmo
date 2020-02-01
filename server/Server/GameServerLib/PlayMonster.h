#pragma once
#include "PlayAIObject.h"
#include "TimerFix.h"
#include <set>

namespace OGSLib
{
	enum TALKTYPE{
		TALK_NORMAL=0,
		TALK_ATTACK,
		TALK_ATTACKED,
		TALK_HALF_HP,
		TALK_DIE,
	};
	class ScriptLua;
	class SubItem;
	class ManagerMonster;
	class DomainPlay;
	class PlayMonsterCaller;

	class GameServerLib;

	class PlayMonster : public PlayAIBase
	{
		friend class ManagerMap;
	public:
		//typedef std::map<int,int>	HurtRecordMap;
		struct ExpHitter
		{
			int id;
			time_t tick;
			std::string name;
		};
	protected:
		std::string mReliveMap;
		int mReliveDir;
		int mReliveRange;
		double m_ReliveGap;
		double m_ReliveGap2;////其他刷新时间
		double mTalkTick;
		int mTalkGap;
		int mMonType;
		///////////////////////////////新的怪物ai  Begin
		int mMabiGap;///麻痹相关
		int mMabiProb;
		int mPoisonProb;//施毒相关
		int mPoisonTime;
		int mPoisonDefense;
		int mPoisonDamage;
		int mAoeRange;//范围攻击相关
		int mFenshenNum;//分身相关
		int mFenshenHp;
		int mFenshenMonName;
		bool mFenshenFlag;
		int mRageHp;//狂暴相关
		int mRagePowerMul;
		int mRageAttackSpeedMul;
		int mRageRunSpeedMul;
		bool mRageFlag;
		bool mLandification;//石化相关
		int mDirFixed;
		bool mDirectDie;
		bool mNoShowHitter;
		
		// 怪物召唤相关		
		int mCallMonCountOnce;			// 单次召唤数量
		int mCallMonMaxCount;			// 最大召唤数量
		int mCallMonName;				// 召唤怪物名字
		PlayMonster * mCaller;			// 召唤this的怪物
		std::vector<PlayMonster *> mMonstersCalled; // this召唤的怪物

		int mSwitchTargetType;			// 切换目标类型(2:时间,1:仇恨值)
		int mSwitchTargetInteval;		// 切换目标时间(针对于非仇恨选择目标的怪),0不切换目标
		int mHatredDamageRatio;			// 伤害系数
		int mHatredDistanceRatio;		// 距离仇仇恨系数
		double mLastSwitchTargetTick;	// 最后切换目标时间
		int mFollowRange;				// 追击范围
		int mNoTargetTick;				// 记录没有目标的时间
		///////////////////////////////新的怪物ai  End
		TimerFix   m_ReliveTimer;
		int mNoRilive;

		int mDeathExp;
		int mDropNoOwner;
		int mTeamID;
		int mlimitlv;
		//怪物组别
		int m_nMonGroup;

		int mHurtRecord[10];
		int mHurtRecordID[10];
		int mHurtRecordSeed[10];
		time_t mHurtRecordTick[10];
		ExpHitter m_expHiter;
		int mHurtRecordTime;
		bool mHalfHp;
		double mLastSlowTimer;
		std::vector<MonDropDef>* mDropList;
		std::vector<MonTalkDef>* mTalkList;
		std::vector<NewMonDropDef2>* mDropList2;
		std::map<int,int> mDropGroupRecord;
		int mDeadNum;
		int mDisapearTime;
		int mPowerExp;

		int m_nTombId;

		bool m_bLockTarget;
		/*int m_nDestX;
		int m_nDestY;*/

	public:
		PlayMonster();
		virtual ~PlayMonster(void);
	public:
		int TeamID(){return mTeamID;}
		int GetMonGroup(){return m_nMonGroup;}
		const char* FirstName();
		virtual int DefId(){return mDefId;}
		virtual int  Attacked(PlayActiveObject* src,SkillDef * sd,int pawer,int flags,int delay);
		// virtual bool IsProperAttackDes(PlayActiveObject* des,int param);
		virtual bool IsProperAttackDes(PlayActiveObject* des, SkillDef * sd);
		virtual int Update(double time);
		virtual int State();
		virtual int CollectTime(){return 0;}
		virtual int IsBoss(){ return mMonType == 4002; }
		virtual int IsElite(){ return mMonType == 4003; }
		virtual int getlimitlv(){ return mlimitlv; }
		virtual int DisaperTime();
		virtual int getShowState();
		virtual int GetMabiGap() { return mMabiGap; }
		virtual int GetPoisonTime() { return mPoisonTime; }
		virtual int GetPoisonDefense() { return mPoisonDefense; }
		virtual int GetPoisonDamage() { return mPoisonDamage; }
		virtual void SetPosioner(PlayActiveObject * src);
		virtual bool AddBuff(int id, double param = -1);
		virtual bool AddStatus(int status,int duration,int param,double gap=1.0,int flags=0,int level[]=NULL);
		GameServerLib* gsl(){ return mGSL; }

		bool IsDirFixed();
		bool IsNoShowHitter(){return mNoShowHitter;}
		//清理归属
		void clearExpHiter();
	protected:
		virtual int AiIdle();
		virtual void AiOther(double time);
		virtual void AiRelive();
		virtual void AiWalk();
		virtual void OnDead(PlayActiveObject* src,int delay);
		virtual int OnSkillAttack(PlayActiveObject* target,SkillDef* sd,SubSkill* ss);
	public:
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src);
		virtual void NotifyWalk(PlayMapObject* src,int dir,int newx,int newy);
		virtual void NotifyRun(PlayMapObject* src,int dir,int newx,int newy);
		void ClearDie();
		int PlayMonster::GetDeathExp();
		int PlayMonster::GetPowerExp(){return mPowerExp;}
		void IncCallMon(PlayMonster * monCalled);
		void DecCallMon(PlayMonster * monCalled);
		int GetCallMonCountOnce() {return mCallMonCountOnce;};
		int GetCallMonMaxCount() {return mCallMonMaxCount;}
		int GetCallMonCurCount() {return mMonstersCalled.size();}
		int GetCallMonName() {return mCallMonName;}
		//通知怪物src显隐
		void NotifyClearYingShen(PlayMapObject* src);
		// 获取经验获得者
		void GetExpHitter(int& id, std::string& name);

		int GetExpHitterId();

		//生成墓碑
		void GenTombStone();
		//清除墓碑
		void CleanTombStone();

		//获取剩余刷新时间
		//double GetRefreshRemain(){return m_ReliveGap - m_ReliveTimer.Elapsed();}

		//锁定目标相关
		void SetLockTarget(bool lock);
		bool IsLockTarget();
		void LockTarget(int targetId);

		//void SetDestination(int x, int y) {m_nDestX = x; m_nDestY = y;};
	protected:
		void Relive();
		void ClearHurtRecord();
		void RecordExpHitter(PlayActiveObject* src);
		void RecordHurtRecord(int src_id, int pawer, int seed);
		bool IsDesYinShen(PlayActiveObject* src);
		virtual void CheckTarget(PlayMapObject* src,int newx,int newy);
		void DropItem(DomainPlay* play);
		//void DropItem2(DomainPlay* play);
		//void DropItem2(DomainPlay* play,NewMonDropDef* pos);
		void DropItem3(DomainPlay* play);
		void DropItem3(std::set<DomainPlay *>& players);
		bool check_drop_group(int group_id,int select,int randmax);
		void RandTalk(int flags);
		void GiveExperience(DomainPlay* player);
		DomainPlay* FindDeadOwner();
		unsigned int FindMaxHurt();
		void SwitchTarget();
		void SwitchTarget(PlayActiveObject * target);

		std::set<DomainPlay *> GetHurtPlayers();

		// 进入战斗状态
		void EnterFightState();
		// 离开战斗状态
		void LeaveFightState();
	};
}