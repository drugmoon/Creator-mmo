#pragma once

namespace OGSLib
{
	struct MonDef
	{
		int id; //标识id
		int res_id; //模型
		int weapon;
		int wing;
		char name[64];
		char nodigital_name[64];
		char script[64]; //刷新触发脚本
		std::string ai;
		int atk_ai;		 // 攻击ai
		int effect_type; //技能类型
		int effect_res; //技能资源
		int appr; //目标特效
		int lvl;
		int zslevel;
		int cooleye; //视野范围
		int exp;
		int power_exp;

		int hp;
		int mp;
		int ac;
		int acMax;
		int mac;
		int macMax;
		int dc;
		int dcMax;
		int mc;
		int mcMax;
		int dodge; //闪避
		int accuracy; //命中
		int walk_gap; //最小等待时间
		int walk_gap2; //最大等待时间
		int walk_step; //每次walk的步数，对方要求每步所需时间

		int attack_spd; // 攻击间隔
		int head_icon; //头像资源
		int talk_gap; //说话间隔
		int mon_type; //怪物类型
		int drop_group; //掉落组id
		int patrol_range; //巡逻范围
		int fllow_range; //追击范围
		int no_hit; //是否可以野蛮
		// 切换目标类型
		int switch_target_type;
		// 切换目标时间,值为0不切换目标
		int switch_target_inteval;
		// 仇恨伤害系数,默认1
		int hatred_damage_ratio;
		// 距离仇恨系数,默认0
		int hatred_distance_ratio;
		// 追击范围

		// 固定方向
		int dirFixed;

		std::string mapName; //地图
		int mType; //单位类型

		//怪物组别
		int monGroup;

		//传送点id
		int teleportID;
		//巡逻速度
		int patrolTime;
		//尸体消失时间
		int deadTime;
		//刷新时间
		int refreshTime;
		//暴击率
		int CRIRate;
		//抗暴击
		int DUCrate;
		//物理攻击减免
		int ADTR;
		//魔法减免
		int MR;
		//回血速度
		int HPup;
		//是否显示刷新公告
		int isShowRefresh;
		//是否显示掉落公告
		int isShowDrop;
		// boss积分
		int BossPt;
		int title;
		int colltime;
		int no_show_hitter;
		// boss头像
		std::string BossHead;
		//功能ID	
		std::string funcid;
		//功能参数
		std::string funcparams;
		//默认对话	
		std::string dialog;
		//是否会喊话	
		int canSpeak;
		//随机喊话	
		std::string randomDialog;
		//简单掉落ID
		int simpleDropID;	
		//死亡是否显示墓碑
		int isShowTomb;
	};
	struct MonDropDef
	{
		int selNum;
		int maxNum;
		int item_id;
		int count;
		int bind;
		int use_bind;
		int start_drop_day;
	};
	struct NewMonDropDef
	{
		int item_id;
		int count;
		int bind;
		int use_bind;
		NewMonDropDef()
		{
			item_id=0;
			count=0;
			bind=0;
			use_bind=0;
		}
	};
	struct MonDropGroup
	{
		int group_id;
		int select;
		int randmax;
		int randed;
		int passed;
		std::vector<NewMonDropDef> drop;
	};
	struct MonTalkDef
	{
		int prob;
		std::string msg;
	};
	struct NewMonDropDef2
	{
		int group_id;
		int gold_min;
		int gold_max;
		int item_id;
		std::vector<int> drop_plus;
		int drop_prob;
		int bind;
		int owned_duration;
		int start_drop_day;
	};
	struct MonAtkAI
	{
		int id;
		std::string strDesc;
		// 默认技能
		int default_skill;
		// 距离技能
		int dis_skill_min;
		// 距离技能id
		int dis_skill;
		// 概率技能
		int rate_skill;
		// 概率技能概率(万分比)
		int rate_skill_rate;
		// 时间技能id
		int time_skill;
		// 所需战斗时间(ms)
		int time_skill_need;
		// 残血时间技能
		int life_time_skill;
		// 残血时间技能时间
		int life_time_skill_time;
		// 残血时间技能血量比例
		int life_time_skill_life_rate;
		// 残血技能
		int life_skill;
		// 残血技能血量比例
		double life_left_rate;
		int max_life_skill_cout;

		std::string around_kind;
		std::string first_attack_kind;
		// 切换目标类型
		int switch_target_type;
		// 仇恨伤害系数 / 切换目标时间,值为0不切换目标
		int switch_target_param;
		// 距离仇恨系数
		int hatred_distance_ratio;

		MonAtkAI()
		{
			id = 0;
			default_skill = 0;
			dis_skill_min = 0;
			dis_skill = 0;
			rate_skill = 0;
			rate_skill_rate = 0;
			time_skill = 0;
			time_skill_need = 0;
			life_skill = 0;
			life_left_rate = 0;
			max_life_skill_cout = 0;
			switch_target_type = 0;		// 1 仇恨值计算 2 时间计算
			switch_target_param = 0;
			hatred_distance_ratio = 0;
			life_time_skill = 0;
			life_time_skill_life_rate = 0;
			life_time_skill_time = 0;
		}
	};

	class GameServerLib;
	class ManagerMonDef
	{
		typedef std::map<int,MonDef> MondefMap;
		typedef std::map<int,std::vector<MonDropDef> > MonDropDefMap;
		typedef std::map<int,std::vector<MonDropGroup> > MonDropGroupDefMap;
		typedef std::map<int,std::vector<MonTalkDef> > MonTalkDefMap;

		typedef std::map<int,std::vector<NewMonDropDef2>> NewMonDropGroupMap;
		typedef std::map<int,std::vector<NewMonDropDef2>> NewMonDropGroupMap2;

		typedef std::vector<int> DropPlus;
		typedef std::map<int,DropPlus> MonDropPlusMap;

		typedef std::map<int,MonAtkAI> MonAIMap;

		MondefMap mMondefs;
		MonAIMap m_monAIs;
		//MonDropDefMap mMonDropDefs;
		//MonDropGroupDefMap mMonDropGroups;
		//MonTalkDefMap mMonTalkDefs;
		
		MonDropPlusMap mMonDropPlusMap;
		NewMonDropGroupMap mNewMonDropGroups;
		NewMonDropGroupMap2 mNewMonDropGroupItem;

		GameServerLib* mGameServerLib;
	public:
		ManagerMonDef(GameServerLib* gslib);
		virtual ~ManagerMonDef(void);
		
		bool init();
		void dest();
		bool ReloadMonAIDef();
		void ReloadMonDropDef();
		bool ReloadMonDropDef(std::string name);

		bool LoadDropPlusCsv();
		bool LoadMonDropCsv();
		bool LoadMonAICsv();

		MonDef* FindMonDef(int id);
		std::vector<MonDropDef>* FindMonDropDef(int mon_id);
		std::vector<MonDropGroup>* FindMonDropGroupDef(int mon_id);
		std::vector<NewMonDropDef2>* FindNewMonDropGroupDef2(int mon_id);
		std::vector<MonTalkDef>* FindMonTalkDef(int mon_id);

		MonAtkAI * FindAtkAI(int ai_id);
	private:
		void LoadMonDropDef(int mon_id);
		void LoadMonTalkDef(int mon_id);
		void LoadNewMonDropDef(int mon_id);

	};
}