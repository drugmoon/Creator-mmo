#pragma once

#define ITEM_GOLD_ID 40000003
#define ITEM_EXP_ID  40000001
#define ITEM_RELIVE_COIN_ID 19002
#define ITEM_ADD_SLOT 19003

//#define ITEM_OFFLINE_EXP_MUL_1 19004
//#define ITEM_OFFLINE_EXP_MUL_2 19005
//#define ITEM_OFFLINE_EXP_MUL_4 19006
#define ITEM_GOLD_BIND_ID 40000004
#define ITEM_VCOIN_ID 40000005
#define ITEM_VCOIN_BIND_ID 40000006

#define ITEM_BAOSHI_BEGIN 17020
#define ITEM_BAOSHI_END 17067
//#define ITEM_CAPACITY_ID 19010

#define ITEM_BAG_BEGIN 0
#define ITEM_BAG_SIZE 60
//#define ITEM_BAG_MAX 120
#define ITEM_BAG_END (ITEM_BAG_BEGIN+ITEM_BAG_SIZE)

#define ITEM_STALL_BEGIN 900
#define ITEM_STALL_SIZE 20
#define ITEM_STALL_END (ITEM_STALL_BEGIN+ITEM_STALL_SIZE)

#define ITEM_DEPOT_BEGIN 1000
#define ITEM_DEPOT_SIZE 40
#define ITEM_DEPOT_END (ITEM_DEPOT_BEGIN+ITEM_DEPOT_SIZE)

#define ITEM_LOTTERYDEPOT_BEGIN 3000
#define ITEM_LOTTERYSIZE 300
#define ITEM_LOTTERYDEPOT_END (ITEM_LOTTERYDEPOT_BEGIN+ITEM_LOTTERYSIZE)
#define ITEM_XUANJING_BEGIN 3500
#define ITEM_XUANJING_SIZE 140
#define ITEM_XUANJING_END (ITEM_XUANJING_BEGIN+ITEM_XUANJING_SIZE)

#define ITEM_CHATTRADE_BEGIN 4000
#define ITEM_CHATTRADE_SIZE 6
#define ITEM_CHATTRADE_END (ITEM_CHATTRADE_BEGIN+ITEM_CHATTRADE_SIZE)
//#define ITEM_MYSALE_BEGIN 2000
//#define ITEM_MYSALE_SIZE 10
//#define ITEM_MYSALE_END (ITEM_MYSALE_BEGIN+ITEM_MYSALE_SIZE)

//#define DEFAULT_CLOTH_MAN 20000
//#define DEFAULT_CLOTH_FAMALE 10000
#define DEFAULT_HAIR_MAN (-1)
#define DEFAULT_HAIR_FAMALE (-1)

#define ITEM_EQUIP_BEGIN 10000000
#define ITEM_EQUIP_END 19999999

#define ITEM_EQUIP_WARRIOR_BEGIN 11000000
#define ITEM_EQUIP_WARRIOR_END 11999999

#define ITEM_EQUIP_WIZARD_BEGIN 12000000
#define ITEM_EQUIP_WIZARD_END 12999999

#define ITEM_EQUIP_TAOIST_BEGIN 13000000
#define ITEM_EQUIP_TAOIST_END 13999999

#define ITEM_EQUIP_OTHER_BEGIN 14000000
#define ITEM_EQUIP_OTHER_END 19999999

#define ITEM_DIS_BEGIN 20000000
#define ITEM_DIS_END 39999999

//**********************************宝石**********************************
#define ITEM_GEM_BEGIN 25010000
#define ITEM_GEM_END 25999999

#define ITEM_GEM_ATTACK_BEGIN 25010000
#define ITEM_GEM_ATTACK_END 25019999

#define ITEM_GEM_AC_BEGIN 25020000
#define ITEM_GEM_AC_END 25029999

#define ITEM_GEM_MAC_BEGIN 25030000
#define ITEM_GEM_MAC_END 25039999

#define ITEM_GEM_HP_BEGIN 25040000
#define ITEM_GEM_HP_END 25049999

#define ITEM_GEM_MP_BEGIN 25050000
#define ITEM_GEM_MP_END 25059999

#define ITEM_GEM_SPECIAL_BEGIN 25060000
#define ITEM_GEM_SPECIAL_END 25089999
//********************************************************************


#define ITEM_SKILL_BEGIN 30000000
#define ITEM_SKILL_END 31999999

#define ITEM_WEAPON_BEGIN 20000
#define ITEM_WEAPON_END 29999

#define ITEM_CLOTH_BEGIN 30000
#define ITEM_CLOTH_END 39999

#define ITEM_HUNQI_BEGIN 110000
#define ITEM_HUNQI_END 119999

#define ITEM_LINQI_BEGIN 80000
#define ITEM_LINQI_END 89999

#define ITEM_MOXUESHI_BEGIN 120000
#define ITEM_MOXUESHI_END 129999

#define ITEM_ZUOJI_BEGIN 130000
#define ITEM_ZUOJI_END 139999

#define ITEM_FASHION_WEAPON_BEGIN 140000
#define ITEM_FASHION_WEAPON_END 149999

#define ITEM_FASHION_CLOTH_BEGIN 150000
#define ITEM_FASHION_CLOTH_END 159999

#define ITEM_XYFASHION_WEAPON_BEGIN 149000
#define ITEM_XYFASHION_WEAPON_END 149999

#define ITEM_XYFASHION_CLOTH_BEGIN 159000
#define ITEM_XYFASHION_CLOTH_END 159999
//主装
#define EQUIP_TYPE_WEAPON 1
#define EQUIP_TYPE_CLOTH 2
#define EQUIP_TYPE_HAT 3
#define EQUIP_TYPE_NICKLACE 4
#define EQUIP_TYPE_GLOVE 5
#define EQUIP_TYPE_RING 6
#define EQUIP_TYPE_BELT 7
#define EQUIP_TYPE_BOOT 8

//副装
//玉佩
#define EQUIP_TYPE_JADE_PENDANT 11
//护盾
#define EQUIP_TYPE_SHIELD 12
//护心镜
#define EQUIP_TYPE_MIRROR_ARMOUR 13
//面巾
#define EQUIP_TYPE_FACE_CLOTH 14
//龙心
#define EQUIP_TYPE_DRAGON_HEART 15
//狼牙
#define EQUIP_TYPE_WOLFANG 16
//龙骨
#define EQUIP_TYPE_DRAGON_BONE 17
//虎符
#define EQUIP_TYPE_CATILLA 18
//勋章
#define EQUIP_TYPE_ACHIEVE_MEDAL 19



#define ITEM_WEAPON_POSITION (-2 * EQUIP_TYPE_WEAPON)
#define ITEM_CLOTH_POSITION (-2 * EQUIP_TYPE_CLOTH)
#define ITEM_HAT_POSITION (-2 * EQUIP_TYPE_HAT)
#define ITEM_NICKLACE_POSITION (-2 * EQUIP_TYPE_NICKLACE)
#define ITEM_GLOVE1_POSITION (-2 * EQUIP_TYPE_GLOVE)
#define ITEM_GLOVE2_POSITION (-2 * EQUIP_TYPE_GLOVE-1)
#define ITEM_RING1_POSITION (-2 * EQUIP_TYPE_RING)
#define ITEM_RING2_POSITION (-2 * EQUIP_TYPE_RING-1)
#define ITEM_BELT_POSITION (-2 * EQUIP_TYPE_BELT)
#define ITEM_BOOT_POSITION (-2 * EQUIP_TYPE_BOOT)

#define ITEM_JADE_PENDANT_POSITION (-2 * EQUIP_TYPE_JADE_PENDANT)
#define ITEM_SHIELD_POSITION (-2 * EQUIP_TYPE_SHIELD)
#define ITEM_MIRROR_ARMOUR_POSITION (-2 * EQUIP_TYPE_MIRROR_ARMOUR)
#define ITEM_FACE_CLOTH_POSITION (-2 * EQUIP_TYPE_FACE_CLOTH)
#define ITEM_DRAGON_HEART_POSITION (-2 * EQUIP_TYPE_DRAGON_HEART)
#define ITEM_WOLFANG_POSITION (-2 * EQUIP_TYPE_WOLFANG)
#define ITEM_DRAGON_BONE_POSITION (-2 * EQUIP_TYPE_DRAGON_BONE)
#define ITEM_CATILLA_POSITION (-2 * EQUIP_TYPE_CATILLA)
#define ITEM_ACHIEVE_MEDAL_POSITION (-2*EQUIP_TYPE_ACHIEVE_MEDAL)

#define ITEM_XUESHI_POSITION (-20*2)

//#define ITEM_JADE_PENDANT_POSITION (-21*2)
#define ITEM_MEDAL_POSITION (-22*2)
#define ITEM_WING_POSITION (-23*2)
//#define ITEM_WOLFANG_POSITION (-24*2)
//#define ITEM_SHIELD_POSITION (-25*2)
#define ITEM_TALISMAN_POSITION (-26*2)
#define ITEM_OFFICIAL_SEALS_POSITION (-27*2)
#define ITEM_MOUNT_POSITION (-28*2)
#define ITEM_SPECIAL_RING_POSITION (-29*2)

#define ITEM_FASHION_WEAPON_POSITION (-31*2)
#define ITEM_FASHION_CLOTH_POSITION (-32*2)
#define ITEM_FASHION_WING_POSITION (-33*2)

#define ITEM_HUNQI_POSITION (-40*2)
#define ITEM_XUEFU_POSITION (-41*2)
#define ITEM_HUDUN_POSITION (-42*2)
#define ITEM_BAOSHI_POSITION (-43*2)
#define ITEM_HUNZHU_POSITION (-44*2)



//时装
#define EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN 5000
#define EQUIP_TYPE_FASHION_CLOTH_SIZE 100
#define EQUIP_TYPE_FASHION_CLOTH_POS_END   (EQUIP_TYPE_FASHION_CLOTH_POS_BEGIN+EQUIP_TYPE_FASHION_CLOTH_SIZE)

//**********************************宝石position**********************************
#define GEM_ATTACK_OFFSET_POSITION 500

#define GEM_ATTACK_WEAPON_POSITION (ITEM_WEAPON_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_CLOTH_POSITION (ITEM_CLOTH_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_HAT_POSITION (ITEM_HAT_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_NICKLACE_POSITION (ITEM_NICKLACE_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_GLOVE1_POSITION (ITEM_GLOVE1_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_GLOVE2_POSITION (ITEM_GLOVE2_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_RING1_POSITION (ITEM_RING1_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_RING2_POSITION (ITEM_RING2_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_BELT_POSITION (ITEM_BELT_POSITION - GEM_ATTACK_OFFSET_POSITION)
#define GEM_ATTACK_BOOT_POSITION (ITEM_BOOT_POSITION - GEM_ATTACK_OFFSET_POSITION)

#define GEM_AC_OFFSET_POSITION 530

#define GEM_AC_WEAPON_POSITION (ITEM_WEAPON_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_CLOTH_POSITION (ITEM_CLOTH_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_HAT_POSITION (ITEM_HAT_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_NICKLACE_POSITION (ITEM_NICKLACE_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_GLOVE1_POSITION (ITEM_GLOVE1_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_GLOVE2_POSITION (ITEM_GLOVE2_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_RING1_POSITION (ITEM_RING1_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_RING2_POSITION (ITEM_RING2_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_BELT_POSITION (ITEM_BELT_POSITION - GEM_AC_OFFSET_POSITION)
#define GEM_AC_BOOT_POSITION (ITEM_BOOT_POSITION - GEM_AC_OFFSET_POSITION)

#define GEM_MAC_OFFSET_POSITION 560

#define GEM_MAC_WEAPON_POSITION (ITEM_WEAPON_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_CLOTH_POSITION (ITEM_CLOTH_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_HAT_POSITION (ITEM_HAT_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_NICKLACE_POSITION (ITEM_NICKLACE_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_GLOVE1_POSITION (ITEM_GLOVE1_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_GLOVE2_POSITION (ITEM_GLOVE2_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_RING1_POSITION (ITEM_RING1_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_RING2_POSITION (ITEM_RING2_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_BELT_POSITION (ITEM_BELT_POSITION - GEM_MAC_OFFSET_POSITION)
#define GEM_MAC_BOOT_POSITION (ITEM_BOOT_POSITION - GEM_MAC_OFFSET_POSITION)

#define GEM_HP_OFFSET_POSITION 590

#define GEM_HP_WEAPON_POSITION (ITEM_WEAPON_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_CLOTH_POSITION (ITEM_CLOTH_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_HAT_POSITION (ITEM_HAT_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_NICKLACE_POSITION (ITEM_NICKLACE_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_GLOVE1_POSITION (ITEM_GLOVE1_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_GLOVE2_POSITION (ITEM_GLOVE2_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_RING1_POSITION (ITEM_RING1_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_RING2_POSITION (ITEM_RING2_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_BELT_POSITION (ITEM_BELT_POSITION - GEM_HP_OFFSET_POSITION)
#define GEM_HP_BOOT_POSITION (ITEM_BOOT_POSITION - GEM_HP_OFFSET_POSITION)

#define GEM_MP_OFFSET_POSITION 620

#define GEM_MP_WEAPON_POSITION (ITEM_WEAPON_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_CLOTH_POSITION (ITEM_CLOTH_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_HAT_POSITION (ITEM_HAT_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_NICKLACE_POSITION (ITEM_NICKLACE_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_GLOVE1_POSITION (ITEM_GLOVE1_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_GLOVE2_POSITION (ITEM_GLOVE2_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_RING1_POSITION (ITEM_RING1_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_RING2_POSITION (ITEM_RING2_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_BELT_POSITION (ITEM_BELT_POSITION - GEM_MP_OFFSET_POSITION)
#define GEM_MP_BOOT_POSITION (ITEM_BOOT_POSITION - GEM_MP_OFFSET_POSITION)

#define GEM_SPECIAL_OFFSET_POSITION 650

#define GEM_SPECIAL_WEAPON_POSITION (ITEM_WEAPON_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_CLOTH_POSITION (ITEM_CLOTH_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_HAT_POSITION (ITEM_HAT_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_NICKLACE_POSITION (ITEM_NICKLACE_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_GLOVE1_POSITION (ITEM_GLOVE1_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_GLOVE2_POSITION (ITEM_GLOVE2_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_RING1_POSITION (ITEM_RING1_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_RING2_POSITION (ITEM_RING2_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_BELT_POSITION (ITEM_BELT_POSITION - GEM_SPECIAL_OFFSET_POSITION)
#define GEM_SPECIAL_BOOT_POSITION (ITEM_BOOT_POSITION - GEM_SPECIAL_OFFSET_POSITION)
//********************************************************************

#define ITEM_STEEL_MAX 50

#define ITEM_DISP_SUBTYPE_HP_INC 1
#define ITEM_DISP_SUBTYPE_MP_INC 2
#define ITEM_DISP_SUBTYPE_SCRIPT 3
#define ITEM_DISP_SUBTYPE_HP_ADD 4
#define ITEM_DISP_SUBTYPE_MP_ADD 5
#define ITEM_DISP_SUBTYPE_HP_MP_INC 7
#define ITEM_DISP_SUBTYPE_HP_MP_ADD 8
#define ITEM_DISP_SUBTYPE_ZIP 9
#define ITEM_DISP_SUBTYPE_SUB_PK 10
#define ITEM_DISP_SUBTYPE_HP_RECOVER 11
#define ITEM_DISP_SUBTYPE_STEEL 12

#define ITEM_POSITION_EXCHANGE_BAG2DEPOT -5001
#define ITEM_POSITION_EXCHANGE_DEPOT2BAG -5002

#define ITEM_TYPE_EQUIP 1
#define ITEM_TYPE_MONEY 2
#define ITEM_TYPE_DRUG 3
#define ITEM_TYPE_MATERIAL 4
#define ITEM_TYPE_SKILLITEM 5
#define ITEM_TYPE_GEM 6
#define ITEM_TYPE_GIFT 7
#define ITEM_TYPE_CHEST 8
#define ITEM_TYPE_BUFF 9
#define ITEM_TYPE_FASHION 10
#define ITEM_TYPE_SCROLL 11
#define ITEM_TYPE_OTHER 12

namespace OGSLib
{
	class ItemPlusDef
	{
	public:
		int mItemPlusID;
		int mTypeID;
		int mNeedType;
		int mNeedParam1;
		int mNeedParam2;
		int mNeedParam3;
		int mNeedParam4;
		int mNeedParam5;

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
		int mLuck;
		int mCurse;
		int mAccuracy;
		int mDodge;
		int mAntiMagic;
		int mAntiPoison;
		int mHpRecover;
		int mMpRecover;
		int mPoisonRecover;
		int mDixiao_pres;
		int mMabi_prob;
		int mMabi_dura;
		int mHalfBaoji;
		int mFuyuan_cd;
		int mFuyuan_pres;
		int mMax_hp;
		int mMax_mp;
		int mMax_hp_pres;
		int mMax_mp_pres;
		int mJob;
		int mGender;
	};
	class ItemPlusNode
	{
	public:
		int mItemPlusID;
		std::vector<ItemPlusDef> mPlus; 
		std::vector<ItemPlusDef> mPlusZl; 
	};
	class GameServerLib;
	struct ItemProgram
	{
		ItemProgram()
		{
			mTypeID = 0;
			mResMale = 0;
			mResFemale = 0;
			Script[63]=0;
		}
		int mTypeID;
		int mResMale;
		int mResFemale;
		char Script[64];
		
	};
	class ItemDef
	{
	public:
		std::string mName;
		std::string mDesp;
		int mTypeID;
		int mIconID;
		int mPrice;
		int mWeight;
		int mDurationMax;
		int mNeedType;
		int mNeedParam;
		int mNeedReinLv;
		int mColor;
		int mLastTime;
		int mOnSaleFlag;

		int mResMale;
		int mResFemale;
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
		int mLuck;
		int mCurse;
		int mAccuracy;
		int mDodge;
		int mAntiMagic;
		int mAntiPoison;

		int mHpRecover;
		int mMpRecover;

		int mPoisonRecover;
		int mHolyDam; // 神圣伤害

		int mBaoji;//双倍暴击
		int mDrop_luck;

		int mRandRange;
		short mRandAC;
		short mRandMAC;
		short mRandDC;
		short mRandMC;
		short mRandSC;
		short mAddBaseAC;
		short mAddBaseMAC;
		short mAddBaseDC;
		short mAddBaseMC;
		short mAddBaseSC;

		int mMabi_prob;
		int mMabi_dura;
		int mDixiao_pres;
		int mFuyuan_cd;
		int mFuyuan_pres;
		int mMax_hp;
		int mMax_mp;
		int mMax_hp_pres;
		int mMax_mp_pres;
		int mNeedZsLevel;
		int mEquipLevel;
		int mJob;
		int mGender;
		int mEquipGroup;
		int mEquipComp;
		int mEquipContribute;
		int mShowDest;
		int mAddPower;
		int mItemBg;
		int mCanUse;

		int mEquipType;
		int mXishou_prob;
		int mXishou_pres;
		int mFantan_prob;
		int mFantan_pres;
		int mBaoji_prob;
		int mBaoji_pres;
		int mXixue_prob;
		int mXixue_pres;
		int mProtect;
		int mNotips;

		int SubType;
		int HPChange;
		int MPChange;
		int ZipType;
		int ZipNumber;
		char Script[64];
		int mRecycleMoney;
		int mRecycleExp;
		int mRecycleXuefu;
		int mCanDestroy;
		int mCanDepot;
		int mCanPush;
		//背包显示红点
		int mBagShow;
		//使用次数限制红点
		int mTimesLimit;
		int mFuncId;
		std::string mFuncParams;
		std::string mSource;
		std::vector<ItemPlusDef> mPlus;
		std::vector<ItemPlusDef> mPlusZl; 

		ItemDef();
		static bool IsDissipative(int sub_type);
		static bool IsEquipment(int sub_type);
		static bool IsEatable(int sub_type);
		static bool IsMaterial(int sub_type);
		static bool IsSkillItem(int sub_type);
		static bool IsWeapon(int equip_type);
		static bool IsCloth(int equip_type);
		static bool IsGlove(int equip_type);
		static bool IsRing(int equip_type);

		static bool IsLoadEquipment(int type_id);
		static bool IsMoxueshi(int type_id);
		static bool IsHunqi(int type_id);
		static bool IsLinqi(int type_id);
		static bool IsFashion(int type_id);
		static bool IsFashionWeapon(int type_id);
		static bool IsFashionCloth(int type_id);
		static bool IsXYFashionWeapon(int type_id);
		static bool IsXYFashionCloth(int type_id);
		static bool IsEquipWarrior(int type_id);
		static bool IsEquipWizard(int type_id);
		static bool IsEquipTaoist(int type_id);
		
		//宝石相关
		static bool IsGem(int sub_type);
		static bool IsAttackGem(int type_id);
		static bool IsACGem(int type_id);
		static bool IsMACGem(int type_id);
		static bool IsHpGem(int type_id);
		static bool IsMpGem(int type_id);
		static bool IsSpecialGem(int type_id);

		static int getGemOffSet(int type_id);
		static int getGemEquipPosition (int equip_pos, int gem_id);

	};

	class GameServerLib;
	class ManagerItemDef
	{
	public:
		typedef std::map<int, ItemProgram> ItemProgramMap;
		typedef std::map<int,ItemDef> ItemDefineMap;
	private:
		ItemProgramMap mItemProgramMap;
		ItemDefineMap mItemDefineMap;
		typedef std::map<int,int> ItemTrigerMap;
		ItemTrigerMap mItemTriger;
		typedef std::map<int,int> ItemDropAlertMap;
		ItemDropAlertMap mItemAlert;
		typedef std::map<int,float> ItemSpeedMap;
		ItemSpeedMap mItemSpeedMap;
		typedef std::map<int,int> ItemMountDelayMap;
		ItemMountDelayMap mItemMountDelayMap;
		typedef std::map<int,int> ItemStackMap;
		ItemStackMap mItemStackMap;
		typedef std::map<int,int> TradeLimitMap;
		TradeLimitMap mTradeLimitMap;

		typedef std::map<int,std::vector<ItemPlusDef>> ItemPlusMap;
		ItemPlusMap mItemPlusMap;

	public:
		//ItemDef*  mSteelArray[ITEM_STEEL_MAX];
		//int mOfflineExpPrice1;
		//int mOfflineExpPrice2;
		//int mOfflineExpPrice4;
		int mRelivePrice;
		int mAddDepotPrice;
	public:
        ItemDefineMap &getItemDefineMap() { return mItemDefineMap; }
		bool LoadItemDefine(GameServerLib* lib);
		bool ReloadItemDefine(GameServerLib* lib);
		bool ReloadPlusDefine(GameServerLib* lib);
		bool ReloadItemAttr(GameServerLib* lib);
		bool LoadItemPlusDefine(GameServerLib* lib);
		bool LoadItemPlusDefine2(GameServerLib* lib);
		bool LoadItemPlusTxt(GameServerLib* lib);
		bool LoadItemPlusAttr(GameServerLib* lib);
		bool LoadSpeedDefine(GameServerLib* lib);
		bool LoadStackDefine(GameServerLib* lib);
		bool LoadTradeLimit(GameServerLib* lib);
		void AddItemTriger(int type_id);
		int  GetItemTriger(int type_id);
		void AddItemAlert(int type_id);
		int  GetItemAlert(int type_id);
		void dest();
		ItemDef* GetItemDefine(int type_id);
		ItemDef* GetItemDefine(const char* name);
		ItemDef* FindItemDefine(int sub_type,int duramax);
		float GetItemSpeed(int type_id);
		int GetItemMountDelay(int type_id);
		int GetItemStackMax(int type_id);
		int CheckItemTradeLimit(int type_id);
	};
}