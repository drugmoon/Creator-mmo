#pragma once
#include "PlayMapObject.h"
#include "ManagerStatus.h"

namespace OGSLib
{
	class PlayNpc;
	class SubItem;
	class SubAction;
	class DomainData;
	class SubSkill;
	class SkillDef;
	class StatusInfo;
	class PlayMapTrap;
	enum AccuracyConst
	{
		AccuracyConstBase=15,
	};
	enum DodgeConst
	{
		DodgeConstBase=5,
	};
	class ScriptItemAttibuteParam
	{
	public:
		ScriptItemAttibuteParam();
	public:
		float mMaxHpMul;
		float mMaxHpAdd;
		float mMaxMpMul;
		float mMaxMpAdd;
		float mACMul;
		float mACMaxMul;
		float mMACMul;
		float mMACMaxMul;
		float mDCMul;
		float mDCMaxMul;
		float mMCMul;
		float mMCMaxMul;
		float mSCMul;
		float mSCMaxMul;
		float mACAdd;
		float mACMaxAdd;
		float mMACAdd;
		float mMACMaxAdd;
		float mDCAdd;
		float mDCMaxAdd;
		float mMCAdd;
		float mMCMaxAdd;
		float mSCAdd;
		float mSCMaxAdd;
		float mFight;
	};
	class ObjectAttribute
	{
	public:
		ObjectAttribute();
	public:
		int mMaxMp;
		int mMaxHp;		
		int mMaxPower;
		int mPowerPercent;

		int mMaxBurden;
		int mMaxLoad;
		int mMaxBrawn;

		int mBurden;
		int mLoad;
		int mBrawn;

		int mAC;
		int mACMax;
		int mMAC;
		int mMACMax;
		int mDC;
		int mDCMax;
		int mMC;
		int mMCMax;
		int mSC;
		int mSCMax;

		int mAccuracy;
		int mDodge;

		int mLuck;

		int mAntiMagic;
		int mAntiPoison;
		
		int mHpRecover;
		int mMpRecover;
		int mPoisonRecover;

		int mMabi_prob;
		int mMabi_dura;
		int mDixiao_pres;
		int mFuyuan_cd;
		int mFuyuan_pres;
		int mDropProb;
		int mDoubleAttProb;
		int mXishouProb;
		int mXishouPres;
		int mFantanProb;
		int mFantanPres;
		int mBaojiProb;
		int mBaojiPres;
		int mBaojiliPercent;
		int mXixueProb;
		int mXixuePres;
		int mIgnoreDef;
		int mHpPres;
		int mMpPres;
		int mFight;
		int mSubBaoji;
		int mTenacity;		// 抗暴击
		int mHolyDam;		// 神圣伤害
		int mPKIncrement;	// PK增益(万分比)
		int mPKReduce;		// PK减免(万分比)
		int mExpGain;		// 经验增益
		int mDamIncrement;	// 增伤
		int mDamReduce;		// 免伤(整体)
		int mDamReduceDC;	// 物理免伤
		int mDamReduceMC;	// 魔法免伤
		int mPKReduceDC;	// PK物理减免(万分比)
		int mPKReduceMC;	// PK魔法减免(万分比)
		int mMonDamIncrement; // 对怪物伤害增益
		int mZSMonDamIncrement; // 战神队怪物伤害增益 
	};
	class PlayActiveObject :
		public PlayMapObject
	{
	public:
		enum STATUS_TYPE
		{
			STATUS_TYPE_MOFADUN = 0,
			STATUS_TYPE_YINGSHEN = 1,
			STATUS_TYPE_YOULINGDUN = 2,//mMACMax
			STATUS_TYPE_SHENSHENGZHANJIASHU = 3,//mACMax
			STATUS_TYPE_POSION_HP = 4, //持续性扣血
			STATUS_TYPE_POSION_ARMOR = 5, //不能自动回血同时减防御
			STATUS_TYPE_HP_RECOVER = 6,
			STATUS_TYPE_ADD_EXP = 7,
			STATUS_TYPE_ADD_ACMAX = 8,//mACMax
			STATUS_TYPE_ADD_MACMAX = 9,//mMACMax
			STATUS_TYPE_ADD_DCMAX = 10,//mDCMax
			STATUS_TYPE_ADD_MCMAX = 11,//mMCMax
			STATUS_TYPE_ADD_SCMAX = 12,//mSCMax
			STATUS_TYPE_ADD_DROP_ITEMADD_PROB = 13,
			STATUS_TYPE_AUTO_ADD_EXP = 14,
			STATUS_TYPE_NO_DAMAGE = 15,
			STATUS_TYPE_ALL_YINGSHEN = 16,
			STATUS_TYPE_NO_DROP = 17,
			STATUS_TYPE_SHUT_PK_VALUE = 18,
			//STATUS_TYPE_SEVEN_COLOR_DAN = 19,//mAC mACMax mMAC mMACMax mDC mDCMax mMC mMCMax mSC mSCMax mMaxHp mMaxMp
			STATUS_TYPE_MABI = 20,
			//STATUS_TYPE_YUANSHENHUTI = 21,//mAC mACMax mMAC mMACMax mDC mDCMax mMC mMCMax mSC mSCMax
			//STATUS_TYPE_BAQIHUTI = 22,//mAC mACMax mMAC mMACMax mDC mDCMax mMC mMCMax mSC mSCMax
			STATUS_TYPE_ADD_HP = 23,//mMaxHp
			STATUS_TYPE_ADD_MP = 24,//mMaxMp
			//STATUS_TYPE_TIANSHENHUTI = 25,//mMaxHp mDC mDCMax mMC mMCMax mSC mSCMax mDixiao_pres mFuyuan_cd
			//STATUS_TYPE_SHENWEI = 26,//mMaxHp mDC mDCMax mMC mMCMax mSC mSCMax mDixiao_pres mFuyuan_cd
			//STATUS_TYPE_ZHIZUN = 27,//mMaxHp mDC mDCMax mMC mMCMax mSC mSCMax mDixiao_pres mFuyuan_cd
			//STATUS_TYPE_FUQITONGXIN = 28,//mAC mACMax mMAC mMACMax 
			//STATUS_TYPE_XUANTIANZHENQI = 29,
			STATUS_TYPE_MOXUESHI = 30,
			STATUS_TYPE_DEC_HP = 31,	// 持续性扣血buf
			//STATUS_TYPE_XINFA1 = 30,
			//STATUS_TYPE_XINFA2 = 31,
			STATUS_TYPE_WLMZ = 32,
			//帮会祈祷buff
			STATUS_TYPE_AC = 33,
			STATUS_TYPE_MAC = 34,
			STATUS_TYPE_DC = 35,
			STATUS_TYPE_MC = 36,
			STATUS_TYPE_SC = 37,
			STATUS_TYPE_BOJIPROP = 38,
			STATUS_TYPE_BOJIPRES = 39,
			STATUS_TYPE_TENACITY = 40,
			STATUS_TYPE_SACREDDAM = 41,
// 			STATUS_TYPE_GUILD_HP = 39,
// 			STATUS_TYPE_GUILD_MP = 40,
			//STATUS_TYPE_ZHUANSHEN = 33,
			//STATUS_TYPE_VIP = 34,
			//STATUS_TYPE_ZUOQI = 35,
			//STATUS_TYPE_GUANZHI = 36,
			//STATUS_TYPE_ZUAN = 37,
			//STATUS_TYPE_XINFA_XIN = 38,
			//帮会祈祷buff
			STATUS_TYPE_GUILD_ATK = 50,
			STATUS_TYPE_GUILD_ATKMAX = 51,
			STATUS_TYPE_GUILD_AC = 52,
			STATUS_TYPE_GUILD_ACMAX = 53,
			STATUS_TYPE_GUILD_MAC = 54,
			STATUS_TYPE_GUILD_MACMAX = 55,
			STATUS_TYPE_GUILD_HPMAX = 56,
			STATUS_TYPE_GUILD_MPMAX = 57,
			STATUS_TYPE_GUILD_SACREDDAM = 58,
			STATUS_TYPE_GUILD_BOJIPROP = 59,
			STATUS_TYPE_GUILD_BOJIPRES = 60,
			STATUS_TYPE_GUILD_TENACITY = 61,
			
			//STATUS_TYPE_ZUNQI_XIN = 55,
			STATUS_TYPE_SHOULING=78,
			STATUS_TYPE_GUILD=79,
			//STATUS_TYPE_YUANSHEN=80,
			STATUS_TYPE_BURNING = 82,
			//STATUS_TYPE_ADD_EXP_VIP = 83,
			//STATUS_TYPE_YUANSHEN2 = 84,//元神突破
			STATUS_TYPE_MULPRO = 85,//爆率提升
			//STATUS_TYPE_FCZS = 86,//复仇转生

			//药瓶buff
			STATUS_TYPE_DRUG=89,
			STATUS_TYPE_INSTANT_DRUG1=90,
			STATUS_TYPE_INSTANT_DRUG2=91,
			STATUS_TYPE_INSTANT_DRUG3=92,
			STATUS_TYPE_INSTANT_DRUG4=93,
			STATUS_TYPE_INSTANT_DRUG5=94,
			STATUS_TYPE_CONTINUATE_DRUG1=95,
			STATUS_TYPE_CONTINUATE_DRUG2=96,
			STATUS_TYPE_CONTINUATE_DRUG3=97,
			STATUS_TYPE_CONTINUATE_DRUG4=98,
			STATUS_TYPE_CONTINUATE_DRUG5=99,
			//回血buff(怪物，宝宝，神将)
			//平时
			STATUS_TYPE_USUAL_RECOVER = 100,
			//战时
			STATUS_TYPE_BATTLE_RECOVER = 101,

			STATUS_TYPE_NUMBER = 110,
		};
		enum SHOULING_TYPE
		{
			SHOULING_TYPE_JIANREN=0,
			SHOULING_TYPE_WUWEI=1,
			SHOULING_TYPE_SHASHANG=2,
			SHOULING_TYPE_ZHENFEN=3,
			SHOULING_TYPE_SHENGYUAN=4,
			SHOULING_TYPE_MANLI=5,
		};
	protected:
		ObjectAttribute mActiveAttribute;
		ObjectAttribute mBaseAttribute;
		ObjectAttribute mPropPercent;
		std::string mNullStr;

		//ObjectAttribute mReincarnateAttribute;
		int mMp;
		int mHp;
		int mPower;
		int mLevel;
		int mHurtSeed;

		int mIncHP;
		int mPerHP;
		int mIncMP;
		int mPerMP;
		int mIncMagicHP;
		int mPerMagicHP;
		double mUpdateHPMPInc;

		double mUpdateHpMp;
		double mUpdateHpMpDuration;
		int mUpdateHpMpSlow;
		int mAutoRecover;
		float mUpdateSpeed;
		int mZsLevel;
		int mBurner;
		int mPosioner;
		int mSwordLv;
		int mMonPointLv;

		int mBattleStatusTick;
		int mBattleStatusGap;
		int mDefId;

		std::string mPosionerName;
		LEUD_INT64 mLevelUpExp;
		std::vector<PlayMapTrap *> mTrapsCalled; // 召唤的陷阱
	public:
		typedef std::vector<PlayActiveObject*> InteractiveObjectVector;
		InteractiveObjectVector mVisibleInteractiveObjectList;

		typedef std::vector<SubSkill> SkillMap;
		SkillMap mSkillList;
		SkillMap& GetSkillList(){return mSkillList;}
		virtual int AddSkill(int type_id, int level);
		void IncCallTrap(PlayMapTrap * trapCalled);
		void DecCallTrap(PlayMapTrap * trapCalled);

	private:
		typedef std::map<int,SubAction> ActionMap;
		ActionMap mActionMap;
		int mActionKeyGen;
		int find_action_key(){return mActionKeyGen++;}

		double mStatusTimer[STATUS_TYPE_NUMBER];
		int mStatusParam[STATUS_TYPE_NUMBER];
		int mStatusFlags[STATUS_TYPE_NUMBER];
		int mStatusLevel[STATUS_TYPE_NUMBER][6];
		double mStatusTimerTick[STATUS_TYPE_NUMBER];
		double mStatusTimerTickGap[STATUS_TYPE_NUMBER];
		double mUpdateHPMPTick;

		BuffList m_buffs;
	public:
		bool mHpPro;
		int mUpdateHP;
		int mUpdateMP;
		virtual void UpdateHPMPInc(double time);
		virtual void UpdateHPMPRecast(double time);
	public:
		PlayActiveObject(void);
		virtual ~PlayActiveObject(void);
		int mAttackEffectFlags;
	public:
		bool IsDead(){return IsState(MAP_OBJECT_STATE_DEAD);}
		// bool IsMoFaDun(){return mStatusTimer[STATUS_TYPE_MOFADUN] > 0.0;}
		bool IsYingShen(){return mStatusTimer[STATUS_TYPE_YINGSHEN] > 0.0;}
		bool IsYouLingDun(){return mStatusTimer[STATUS_TYPE_YOULINGDUN] > 0.0;}
		bool IsShenShengZhanJiaShu(){return mStatusTimer[STATUS_TYPE_SHENSHENGZHANJIASHU] > 0.0;}
		bool IsPosionHP(){return mStatusTimer[STATUS_TYPE_POSION_HP] > 0.0;}
		bool IsPosionArmor(){return mStatusTimer[STATUS_TYPE_POSION_ARMOR] > 0.0;}
		bool IsHpRecover(){return mStatusTimer[STATUS_TYPE_HP_RECOVER] > 0.0;}
		bool IsAllYingShen(){return mStatusTimer[STATUS_TYPE_ALL_YINGSHEN] > 0.0;}
		bool IsNoDamage(){return mStatusTimer[STATUS_TYPE_NO_DAMAGE] > 0.0;}
		bool IsNoDrop(){return mStatusTimer[STATUS_TYPE_NO_DROP] > 0.0;}
		bool IsMabi(){return mStatusTimer[STATUS_TYPE_MABI] > 0.0;}
		bool IsShutPKValue(){return mStatusTimer[STATUS_TYPE_SHUT_PK_VALUE] > 0.0;}
		bool IsStatus(int status_id);
		bool IsStatusMFD(int status_id);///是否拥有魔法盾
		int StatusFlags(int status_id);
		bool IsBattleStatus ();//是否战斗状态
		int StatusParam(int status_id);
		int* StatusLevel(int status_id);
		StatusInfo* GetStatusData(int status_id,int lv);
		double StatusTickGap(int status_id);
		double  GetStatusDuration(int status_id);
		int GetStatusFight(int status_id);
		int HpRecoverParam(){return mStatusParam[STATUS_TYPE_HP_RECOVER];}
		int PosionArmorParam(){return mStatusParam[STATUS_TYPE_POSION_ARMOR];}
		int Burner(){return mBurner;}
		void SetBurner(int er){mBurner = er;}
		int Posioner(){return mPosioner;}
		void SetPosioner(int er){mPosioner = er;}
		virtual void SetPosioner(PlayActiveObject * src);
		std::string PosionerName(){return mPosionerName;}
		void SetPosionerName(std::string name) {mPosionerName = name;}
		virtual int DefId(){return 0;}
		virtual int Hp(){return mHp;}
		virtual void Hp(int hp){mHp = hp;}
		virtual int MaxHp(){return mActiveAttribute.mMaxHp;}
		virtual int Mp(){return mMp;}
		virtual int MaxMp(){return mActiveAttribute.mMaxMp;}
		virtual int Power(){return mPower;}
		virtual int MaxPower(){return mActiveAttribute.mMaxPower;}
		virtual int level(){return mLevel;}
		virtual int level(int level){return mLevel = level;}
		virtual int hurtSeed(){ return mHurtSeed; }
		virtual void hurtSeed(int seed){ mHurtSeed = seed; }
		virtual int ZsLevel(){return mZsLevel;}
		virtual int swordlv(){return mSwordLv;}
		virtual int Accuracy(){return mActiveAttribute.mAccuracy;}
		virtual int Dodge(){return mActiveAttribute.mDodge;}
		virtual ObjectAttribute* Attributes(){return &mActiveAttribute;}
		virtual ObjectAttribute* BaseAttributes(){return &mBaseAttribute;}
		virtual	ObjectAttribute* PropPercent(){ return &mPropPercent; }
		virtual void CalculateStatusAttribute();
		virtual void CalculateBuffAttribute();
	public:
		virtual void CalculateAttribute();
		virtual void CalculatePropPercent();

		void AttackPosition(int x,int y,int param,int pawer,bool all,int delay);
		void AttackPosition(int x,int y,SkillDef * sd,int pawer,bool all,int delay);

		virtual void Attack(PlayActiveObject* des,SkillDef * def,int pawer,int flags,int delay);
		virtual void OnAttack(PlayActiveObject* des,SkillDef * def,int pawer,int flags,int delay);
		virtual bool IsProperAttackDes(PlayActiveObject* des,SkillDef * sd);
		virtual bool IsProperFriendDes(PlayActiveObject* des,int skill_type);
		virtual bool CheckHit(PlayActiveObject* des,SkillDef * sd);
		virtual int GetAttackPawer(int base,int add);
		int GetAttackPawerRange(int base,int maxp);
		virtual int  DefenseAttackPawer(PlayActiveObject* src,int pawer,int skill_type,int flags);
		virtual int  Attacked(PlayActiveObject* src,SkillDef * sd,int pawer,int flags,int delay);
		virtual bool DamageHP(PlayActiveObject* src,int damage,int delay, bool safeAreaEnable = false);
		virtual bool AddHP(int add);
		virtual bool DamageMP(int damage);
		virtual bool AddMP(int add);
		virtual bool IsNoHit();
		virtual void ForceMove(int x,int y,int dir);

		int status();
		virtual void UpdateStatusTimer(double time);
		virtual void UpdateBuffTimer(double time);
		virtual bool AddBuff(int id, double param = -1);
		virtual void EnableBuff(int id, bool enable);
		int HaveBuff(int id);
		bool HaveBuffByType(int type);
		bool ClearBuffOnDead();
		bool ClearBuff(int id);
		bool ClearBuffByType(int type);
		bool ClearAllBuff();
		double GetBuffValidValue(int id);
		BuffList & GetBuffList();

		virtual bool AddStatus(int status,int duration,int param,double gap=1.0,int flags=0,int level[]=NULL);
		bool ClearStatus(int status);
		bool ClearAllStatus();
		bool ClearStatusOnDead();
		bool MoFaDun(int paw, SkillDef * sd);
		bool IsMoFaDun();
		bool ClearMoFaDun();
		bool YingShen(int paw,int param);
		bool ClearYingShen();
		bool YouLingDun(int paw,int param);
		bool ClearYouLingDun();
		bool ShenShengZhanJiaShu(int paw,int param);
		bool ClearShenShengZhanJiaShu();
		bool HpRecover(int paw,int param,int flags);
		bool ClearHpRecover();
		bool AllYingShen(int paw,int param,int flags);
		bool ClearAllYingShen();
		bool NoDamage(int paw,int param,int flags);
		bool ClearNoDamage();
		bool NoDrop(int paw,int param,int flags);
		bool ClearNoDrop();
		bool Mabi(SkillDef * sd, int time);
		bool ClearMabi();
		int checkStatusMonDropItemAddProb();
		int checkStatusmulpro();
		bool AddIncMagicHP(int paw);

		void DropTheItem(SubItem* ii, int num = 1, int ownerid = 0, int pick_limit_lv=0, int owned_duration = 30);
		void DropTheItem(SubItem* ii, int num, std::vector<int>& owners, int pick_limit_lv=0, int owned_duration = 30);

		int AddAction(SubAction& action);
		SubAction* FindAction(int action_id);
		void DelAction(int action_id);

		PlayActiveObject* FindActiveInView(int id);
		int FindActiveInPosition(int x,int y,PlayActiveObject** objects,int max_slot);
		int FindMonInPosition(int x,int y);
		int FindEmptyPosition(int *x,int *y);
		bool IsPositionEmpty(int x, int y);
		virtual void UpdateHPMP(double time);

		// 计算使用技能伤害
		int GetUseSkillPower(SkillDef * pDef);
		// 计算防御抵消伤害,技能无视防御也在此函数计算
		int CalculateDefencePower(SkillDef * pDef);

		// 计算技能抵消伤害,(魔法盾这些)
		int CalculateSkillDefence(int power);

		bool Save(DomainData* data);
		bool Load(DomainData* data);
		//////////////////////////////////////////////////////////////////////////
		virtual int Turn(int dir){return 0;}
		SubSkill* FindSkill(int type_id);
		SubSkill* RemoveSkill(int type_id);
		// 人物攻击不需要转身,怪物攻击需要
		void ActionCiShaJianShuBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionBanYueWanDaoBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionYuanYueWanDaoBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionYeManChongZhuangBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionYiBanGongJiBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionGongShaJianShuBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionLieHuoJianFaBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionHuoQiuShuBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn = true);
		void ActionKangJuHuoHuanBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionDiYuHuoBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionLeiDianShuBegin(SkillDef* sd,int* targetx,int* targety,unsigned int paramID, bool need_turn = true);
		void ActionShunJianYiDongBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionDaHuoQiuBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn = true);
		void ActionBaoLieHuoYanBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn = true);
		void ActionHuoQiangBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn = true);
		void ActionJiGuangDianYingBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn = true);
		void ActionDiYuLeiGuangBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionMoFaDunBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID);
		void ActionBingPaoXiaoBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID, bool need_turn = true);
		void ActionZhiYuShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn = true);
		void ActionShiDuShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn = true);
		void ActionLingHunHuoFuBegin(SkillDef* sd,int* targetx,int* targety,unsigned int paramID, bool need_turn = true);
		void ActionYinShenShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID);
		void ActionJiTiYinShenShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn = true);
		void ActionYouLingDunShenShengZhanJiaShuHelper(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn = true);
		void ActionYouLingDunBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn = true);
		void ActionShenShengZhanJiaShuBegin(SkillDef* sd,int targetx,int targety,unsigned int* paramID, bool need_turn = true);
		void ActionQunTiZhiLiaoBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID,int need_turn = true);
		void ActionCollectBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionPickBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionPushAwayBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionPushBegin(SkillDef* sd,int targetx,int targety,unsigned int paramID);
		void ActionWizardNormalBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionTaoistNormalBegin(SkillDef* sd,int paramX,int paramY,unsigned int paramID);
		void ActionLiuXingHuoYuBegin(SkillDef* sd,int paramX,int paramY,unsigned int* paramID);
		// void ActionOfAOE0(SkillDef* sd,int paramX,int paramY,unsigned int* paramID);
		void ActionOfAOE1(SkillDef* sd,int paramX,int paramY,unsigned int* paramID);
	public:
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src);

		virtual void NotifyHPMPChange(PlayActiveObject* src,int hp_change,int mp_change)=0;

		virtual void NotifyWalk(PlayMapObject* src,int dir,int newx,int newy)=0;
		virtual void NotifyRun(PlayMapObject* src,int dir,int newx,int newy)=0;
		virtual void NotifyTurn(PlayMapObject* src,int dir)=0;
		virtual void ResRun(int dir,int newx,int newy){};
		virtual void notifySpeed(PlayMapObject* src,float speed){};
		virtual void notifyDoAction(PlayMapObject* src,int action){};
		virtual void NotifyMapLeave(PlayMap* map);
		virtual void NotifyInjury(PlayMapObject* src,int attacker,int change,int delay,int effect,int change_power)=0;
		virtual void NotifyDie(PlayMapObject* src,int delay)=0;
		virtual void NotifyRelive(PlayMapObject* src,int type)=0;

		virtual void NotifyUseSkill(PlayMapObject* src,int skill_type,int paramX,int paramY,unsigned int paramID,int resID, int dir, int skill_level = 1)=0;
		virtual void NotifyForceMove(PlayMapObject* src,int x,int y,int dir)=0;
		virtual void NotifySatusChange(PlayActiveObject* src,int status_id)=0;
		virtual void NotifyBuffChange(PlayActiveObject* src,int id, int op_code, double time_left)=0;
		virtual void NotifyTiliChange(PlayActiveObject* src)=0;
		virtual void NotifyPlayEffect(PlayMapObject* src,int skill_type,int x,int y,unsigned int targetID,unsigned int ownerID,int resID){};
		virtual void NotifyAttributeChange(){}
		virtual LEUD_INT64 AddExperience(int exp,int src){return 0;}
		virtual void publicPlayEffect(int effect_type,int x,int y,unsigned int targetID,unsigned int ownerID,int resID);
	protected:
		virtual void OnHurt(PlayActiveObject* src,int change,int delay,int change_power)=0;
		virtual void OnDead(PlayActiveObject* src,int delay);
	};
}