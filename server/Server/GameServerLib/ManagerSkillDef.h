#pragma once
namespace OGSLib
{
	struct SkillPlus
	{
		int skill_id;
		int base_acc;
		int lv_acc;
		int base_dodge;
		int lv_dodge;
		SkillPlus()
		{
			skill_id=0;
			base_acc=0;
			lv_acc=0;
			base_dodge=0;
			lv_dodge=0;
		}
	};
	struct SkillInfo
	{
		int need_level;
		int need_exp;
	};
	class SkillDef
	{
	public:
		enum Skill_Type
		{
			SKILL_TYPE_YiBanGongJi = 100,

			SKILL_TYPE_JiChuJianShu = 101,
			SKILL_TYPE_GongShaJianShu = 102,
			SKILL_TYPE_CiShaJianShu = 103,
			SKILL_TYPE_BanYueWanDao = 104,
			SKILL_TYPE_YeManChongZhuang = 105,
			SKILL_TYPE_LieHuoJianFa = 106,
			SKILL_TYPE_PoTianZhan=107,
			SKILL_TYPE_ZhuRiJianFa=109,

			SKILL_TYPE_HuoQiuShu = 401,
			SKILL_TYPE_KangJuHuoHuan = 402,
			SKILL_TYPE_YouHuoZhiGuang = 403,
			SKILL_TYPE_DiYuHuo = 404,
			SKILL_TYPE_LeiDianShu = 405,
			SKILL_TYPE_ShunJianYiDong = 406,
			SKILL_TYPE_DaHuoQiu = 407,
			SKILL_TYPE_BaoLieHuoYan = 408,
			SKILL_TYPE_HuoQiang = 409,
			SKILL_TYPE_JiGuangDianYing = 410,
			SKILL_TYPE_DiYuLeiGuang = 411,
			SKILL_TYPE_MoFaDun = 412,
			SKILL_TYPE_ShengYanShu = 413,
			SKILL_TYPE_BingPaoXiao = 414,
			SKILL_TYPE_MieTianHuo=415,
			SKILL_TYPE_HuoLongQiYan=416,
			SKILL_TYPE_LiuXingHuoYu=417,

			SKILL_TYPE_ZhiYuShu = 501,
			SKILL_TYPE_JinShenLiZhanFa = 502,
			SKILL_TYPE_ShiDuShu = 503,
			SKILL_TYPE_LingHunHuoFu = 504,
			SKILL_TYPE_ZhaoHuanKuLou = 505,
			SKILL_TYPE_YinShenShu = 506,
			SKILL_TYPE_JiTiYinShenShu = 507,
			SKILL_TYPE_YouLingDun = 508,
			SKILL_TYPE_ShenShengZhanJiaShu = 509,
			SKILL_TYPE_XinLingQiShi = 510,
			SKILL_TYPE_KunMoZhou = 511,
			SKILL_TYPE_QunTiZhiLiao = 512,
			SKILL_TYPE_ZhaoHuanShenShou = 513,
			SKILL_TYPE_QiGongBo=514,
			SKILL_TYPE_XuanTianZhenQi=515,
			SKILL_TYPE_ShiXueShu=516,
			SKILL_TYPE_WuJiZhenQi=517,
			SKILL_TYPE_ZhaoHuanYueLing = 518,

// 			SKILL_TYPE_Mon_Attack = 601,
// 			SKILL_TYPE_Mon_BaoFengXue = 601,
			SKILL_TYPE_MonArrow = 601,
			Skill_Type_Monster_Zhaohuan=603,
			Skill_Type_Monster_Mabi=604,
			Skill_Type_Monster_Collect=605,
			Skill_Type_Monster_Pick=606,
			Skill_Type_Monster_PushAway=607,
			SKill_Type_Monster_Push=608,
			Skill_Type_Monster_ClearHurt=609,
			Skill_Type_Monster_NextTarget=610,
			Skill_Type_Monster_Empty = 611,
			Skill_Type_Monster_PubTarget = 612,
			Skill_Type_Slave_Attack = 613,
			Skill_Type_Jump=614,
			Skill_Type_Slave_Stop=615,
			Skill_Type_Slave_Locktarget=616,
			Skill_Type_Monster_AttackAll=617,
			Skill_Type_Slave_Super=618,
			Skill_Type_Monster_Explode=619,
			Skill_Type_Slave_YuanYueZhan=620,
			SKILL_TYPE_Percent_Atk = 800,
			SKILL_TYPE_LevelUp = 900,
		};
		//技能释放方式
		enum Skill_CastWay {
			SKILL_CASTWAW_Null = 0, //无
			SKILL_CASTWAY_Manul = 1, //玩家选择后释放
			SKILL_CASTWAY_Auto = 2, //玩家选择开启后自动释放
			SKILL_CASTWAY_Pasv = 3, //玩家学习后释放
			SKILL_CASTWAY_Delay = 4, //玩家选择后在下次攻击时释放
		};
		//目标选择方式
		enum Skill_Targeting {
			SKILL_TARGETING_Null = 0, //无
			SKILL_TARGETING_Object = 1, //单位
			SKILL_TARGETING_Grid = 2, //格子
			SKILL_TARGETING_Dir = 3, //方向
		};
		static bool IsNearSkill(int skill_type)
		{
			if( skill_type < SKILL_TYPE_HuoQiuShu )
			{
				return true;
			}
			return false;
		}

		static bool IsWarriorSkill(int skill_type)
		{
			if( skill_type >= SKILL_TYPE_JiChuJianShu && skill_type <= SKILL_TYPE_ZhuRiJianFa  )
			{
				return true;
			}
			return false;
		}
		static bool IsWizzardSkill(int skill_type)
		{
			if( skill_type >= SKILL_TYPE_HuoQiuShu && skill_type <= SKILL_TYPE_LiuXingHuoYu  )
			{
				return true;
			}
			return false;
		}
		static bool IsTaoistSkill(int skill_type )
		{
			if( skill_type >= SKILL_TYPE_ZhiYuShu && skill_type <= SKILL_TYPE_ZhaoHuanYueLing  )
			{
				return true;
			}
			return false;
		}
	public:
		int TypeID;
		int SkillID;
		std::string mName;
		std::string mDesp;
		int IconID;
		int ShortCut;
		int Sound;
		int EffectType;
		int Effect_ResID;
		int Attack_Delay;
		int Spell;
		int Power;
		int MaxPower;
		int DefSpell;
		int DefPower;
		int DefMaxPower;
		int Job;
		int LevelMax;
		int Range;
		int NeedL1;
		int L1Train;
		int NeedL2;
		int L2Train;
		int NeedL3;
		int L3Train;
		int Delay;
		int AttributePercentage;

		//北京版新加
		int IsShow;
		int Level;
		int NeedLevel;
		int NextSkill;
		int UseAddExp;
		int UpgradeNeedExp;
		int SkillBook;
		int DamType;
		int ConsumeHp;
		int ConsumeMp;
		double SkillCD;
		double PublicCD;
		int CastWay;
		int SelectTargetWay;
		int Shape;
		int CastDistanceMin;
		int CastDistanceMax;
		int DamageRange;
		int DamageNumMax;

		std::vector<int> TargetType;
		std::vector<int> TargetRelation;
		int DamageEffect;
		int SkillAttackFactor;
		int SkillAddPower;
		int DefenseDamageFactor;
		int TriggerBuff1;
		int TriggerBuff2;
		int TriggerFunction1Prob;
		int TriggerFunction1;
		int TriggerFunction2Prob;
		int TriggerFunction2;
		int Param0;
		int ResID;
		int AOEAction;
		int OrderID;
		std::string DamageDesp;
		std::string RangeDesp;
		std::string CDDesp;
		std::string ExtraEffectDesp;

	public:
		int DefPawer();
		int AdvPawer();
		int Pawer(int p,int lv);
		int Pawer13(int p,int lv);
	};
	class GameServerLib;
	class ManagerSkillDef
	{
		typedef std::map<int,SkillDef> SkillDefineMap;
		SkillDefineMap mSkillDefineMap;

		typedef std::map<int,SkillPlus> SkillPlusMap;
		SkillPlusMap mSkillPlusMap;
		
		typedef std::vector<SkillInfo> SkillInfoVector;
		typedef std::map<int,SkillInfoVector> SkillLevelInfoMap;
		SkillLevelInfoMap mSkillLevelInfoMap;

	public:
		bool LoadSkillInfo(GameServerLib* lib);
		bool LoadSkillDefine(GameServerLib* lib);
		bool LoadNewSkillDefine(GameServerLib* lib);
		bool LoadSkillParam(GameServerLib* lib);
		bool ReLoadSkillDefine(GameServerLib* lib);
		void Dest();
		SkillDef* GetSkillDefine(int skill_type, int skill_level = 1);
		SkillDef* GetSkillDefineById(int skill_id);

		bool IsMagicSkill(int skill_type, int skill_level = 1);

		SkillPlus* GetSkillPlus(int skill_type);
	};
}