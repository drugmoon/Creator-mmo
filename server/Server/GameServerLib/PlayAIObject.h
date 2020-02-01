#pragma once
#include "PlayActiveObject.h"
#include "ManagerMonDef.h"
namespace OGSLib
{
	class DomainPlay;
	class ManagerMonster;
	class ScriptLua;
	class EmptyMonsterRemover;

	struct AiAtkParam
	{
		// 残血技能释放次数
		int life_skill_use_count;
		// 脱离战斗时间点
		double out_fight_tick;
		// 进入战斗时间点
		double in_fight_tick;
		// 残血时间技能使用时间
		double life_time_skill_tick;
		// 时间技能使用时间
		double time_skill_tick;

		AiAtkParam()
		{
			life_skill_use_count = 0;

			out_fight_tick = 1.0;
			in_fight_tick = 0;
			life_time_skill_tick = 0;
			time_skill_tick = 0;
		}
	};

	class PlayAIObject : public PlayActiveObject
	{
		friend class ManagerMap;
		friend class ManagerMonster;
		friend class EmptyMonsterRemover;
		ManagerMonster* mManager;
		PlayActiveObject* mTarget;
		PlayMapObjectNode mUpdateNode;
	protected:
		std::string m_Name;
		std::string mScriptPackage;
		ScriptLua* mScript;
		typedef std::vector<DomainPlay*> DomainPlayVector;
		DomainPlayVector mPlayerList;
		int mBlockCheckType;

		int mTargetEffectResID;
		int mOwnerEffectType;
		int mOwnerEffectResID;
		int mWalkGap;
		int mWalkStep;
		int mCoolEye;
		int mNoHit;
		TimerFix   m_WalkTimer;
		int mAttackGap;
		TimerFix   m_AttackTimer;

		int mAntiCisha;
		enum AI_STATE
		{
			AI_STATE_IDLE=100,
			AI_STATE_WALK=101,
			AI_STATE_ATTACK=102,
			AI_STATE_RELIVE=103,
			AI_STATE_WALK2=104,
			AI_STATE_ATTACK2=105,
			AI_STATE_NULL=106,
		};
		int mAIState;
		int mDieClear;
		int mRecoveHP;

		double mRecoverTick;
		int mRecoverGap;
		int mUsualRecoveHP;
		int mBattleRecoverHp;

		MonAtkAI m_AI;
		AiAtkParam m_AIAtkParam;
		
	public:
		PlayAIObject(void);
		virtual ~PlayAIObject(void);
		void ResetUpdate(int next);
		void SetManager(ManagerMonster* manager,int rightnow);
		void LoadScript(ScriptLua* script,const char* package);
		virtual const char* Name(){return m_Name.c_str();}
		virtual void Name(std::string name){m_Name=name;}
		PlayActiveObject* Target(){return mTarget;}
		void Target(PlayActiveObject* target);
		void ClearTarget(){mTarget = 0;}
		int dieclear(){return mDieClear;}

		void setDefSkill(int skill){m_AI.default_skill = skill;}

		void ActionOfAOE0(SkillDef* sd,int paramX,int paramY,unsigned int* paramID);

		int FindNextDirection(int fromx,int fromy,int toX,int toY);
		virtual int evaluateWalkDirection(int fromX,int fromY,int dir,int toX,int toY);
		
		virtual int  DefenseAttackPawer(PlayActiveObject* src,int pawer,int skill_type,int flags);

		virtual void Walk(int dir);
		virtual bool Walk(int x,int y,int endrange);
		virtual int Turn(int dir);
		void UseSkill(int skill_type,int x,int y,int id,int resid,bool show = true);
		void ResetAttribute();
		virtual void ForceMove(int x,int y,int dir);
		virtual int Update(double time);
		virtual bool IsNoHit();
	protected:
		virtual void OnHurt(PlayActiveObject* src,int change,int delay,int change_power);
		virtual void OnDead(PlayActiveObject* src,int delay);

		virtual void ParseOpt(const char* opt);
		virtual void DoOpt(const char* opt){};
		virtual int AiIdle();
		virtual void AiOther(double time);
		virtual void AiAttack();
		virtual void AiWalk();
	public:
		virtual void NotifyMapLeave(PlayMap* map);
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src);
		virtual void NotifyWalk(PlayMapObject* src,int dir,int newx,int newy){};
		virtual void NotifyRun(PlayMapObject* src,int dir,int newx,int newy){};
		virtual void NotifyTurn(PlayMapObject* src,int dir){}
		virtual void NotifyAttack(PlayMapObject* src,int param){}
		virtual void NotifyAvatarChange(PlayMapObject* src){}
		virtual void NotifyInjury(PlayMapObject* src,int attacker,int change,int delay,int effect,int change_power){};
		virtual void NotifyDie(PlayMapObject* src,int delay){};
		virtual void NotifyRelive(PlayMapObject* src,int type){};
		virtual void NotifyHPMPChange(PlayActiveObject* src,int hp_change,int mp_change){};
		virtual void NotifyUseSkill(PlayMapObject* src,int skill_type,int paramX,int paramY,unsigned int paramID,int resID, int dir, int skill_level = 1){};
		virtual void NotifyForceMove(PlayMapObject* src,int x,int y,int dir){};
		virtual void NotifySatusChange(PlayActiveObject* src,int status_id){};
		virtual void NotifyTiliChange(PlayActiveObject* src){};
		virtual void NotifyBuffChange(PlayActiveObject* src,int id, int op_code, double time_left){};	
	};

	
	class PlayAIBase : public PlayAIObject
	{
	protected:
		int mNoDefaultAttack;

		struct SkillNode
		{
			int tick;
			int skill_type;
			int skill_level;
		};
		struct Skill
		{
			int hppres;
			int prob;
			int maxtick;
			int escape;
			std::vector<SkillNode> skills;
		};
		std::vector<Skill> mSkills;
		int mChooseAction;
		int mActionTick;

		int mAttackRange;
		int mEscapeHP;
		int mEscapeRange;
		int mReliveX;
		int mReliveY;
		int mPatrolRange;
	public:
		PlayAIBase(void);
		virtual ~PlayAIBase(void);
		virtual void DoOpt(const char* opt);
		virtual int onAiAttack(PlayActiveObject* target, SkillDef* sd);
		virtual int OnSkillAttack(PlayActiveObject* target,SkillDef* sd);
		virtual int ChooseSkill(PlayActiveObject* target);
		virtual void RandGo();
	};
}