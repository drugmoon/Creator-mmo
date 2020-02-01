#pragma once
namespace OGSLib
{
	class SwordDef
	{
	public:
		int Sid;
		std::string mName;
		std::string mDesp;
		int SkillType;
		int Type;
		int Level;
		int Param1;
		int Param2;
		int Param3;
		int Baojipres;
		int SubBaoji;
		int Baojiprob;
		int Hp;
		int Mp;
		int Rex;
		int Fight;
		int NextNeed;
	};
	class GameServerLib;
	class ManagerSwordDef
	{
		typedef std::map<int,SwordDef> SwordDefineMap;
		SwordDefineMap mSwordDefineMap;
	public:
		bool LoadSwordDefine(GameServerLib* lib);
		bool ReLoadSwordDefine(GameServerLib* lib);
		void Dest();
		SwordDef* GetSwordDefine(int skill_type);
	};

	class MonPointDef
	{
	public:
		int TypeId;
		int Level;
		
		int ac_min;//Îï·À
		int ac_max;
		int mac_min;//Ä§·À
		int mac_max;
		int dc_min;//ÎïÀí¹¥»÷
		int dc_max;
		int mc_min;//Ä§·¨¹¥»÷
		int mc_max;
		int sc_min;//µÀÊõ¹¥»÷
		int sc_max;
		int max_hp;
		int max_mp;
		int jifen;
	};

	class ManagerMonPointDef
	{
		typedef std::map<int,MonPointDef> MonPointDefineMap;
		MonPointDefineMap mMonPointDefineMap;
	public:
		bool LoadMonPointDefine(GameServerLib* lib);
		bool ReLoadMonPointDefine(GameServerLib* lib);
		void Dest();
		MonPointDef* GetMonPointDefine(int skill_type);
	};

	struct EquipUpgradeDef
	{
		int mJob;
		int mEquipType;
		int mLevel;
		int mDC;
		int mDCMax;
		int mMC;
		int mMCMax;
		int mSC;
		int mSCMax;
		int mAC;
		int mACMax;
		int mMAC;
		int mMACMax;
	};
	class ManagerEquipUpgradeDef
	{
	public:
		typedef std::map<int, EquipUpgradeDef> EquipUpgradeDefMap;
		EquipUpgradeDefMap mEquipUpgradeDefMap;
	public:
		bool LoadEquipUpgradeDefine(GameServerLib* lib);
		bool ReLoadEquipUpgradeDefine(GameServerLib* lib);
		void Dest();
		EquipUpgradeDef* GetEquipUpgradeDefine(int skill_type);

		EquipUpgradeDefMap &getUpgradeDefineMap() { return mEquipUpgradeDefMap; }

	};
}