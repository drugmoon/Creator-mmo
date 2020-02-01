#pragma once
#define ITEM_FLAG_BIND 0x1
#define ITEM_FLAG_USE_BIND 0x2
#define ITEM_FLAG_JIPIN 0x4
#define ITEM_FLAG_JIANDING 0x8

#define ITEM_UPGRADE_COUNT 20

namespace OGSLib
{
	class ObjectAttribute;
	class ScriptItemAttibuteParam;
	class ItemDef;
	class ItemPlusDef;
	class DomainPlay;
	class SubItem
	{
	public:
		friend class ScriptLuaItemBind;
		static int sIdGen;
		int mPosition;
		int mTypeID;
		int mDuraMax;
		int mDuration;
		int mItemFlags;
		short mLuck;
		int mLevel;
		int mNumber;
		short mAddAC;
		short mAddMAC;
		short mAddDC;
		short mAddMC;
		short mAddSC;
		short mAddHp;
		short mAddMp;
		short mUpdAC;
		short mUpdMAC;
		short mUpdDC;
		short mUpdMC;
		short mUpdSC;
		int mCreateTime;
		LEUD_INT64 mIdentId;
		short mProtect;//GM指令 脚本指令 日志 数据库保存读取 交易下发 物品变化下发 查询装备下发

		int mUpdMaxCount;
		int mUpdFailedCount;
		int mSellPriceType;
		int mSellPrice;
		int mZLevel;
		int mLasttime;
		int mLock;////用于物品交易时候玩家改变交易信息时候比对

		int mSubType;//用于判断物品类型，不计入数据库
	public:
		SubItem():mPosition(0),mTypeID(0),mDuration(0),mItemFlags(0),mLuck(0),mLevel(0),mAddAC(0),mAddMAC(0),mAddDC(0),mAddMC(0),mAddSC(0),mAddHp(0),mAddMp(0),mUpdAC(0),mUpdMAC(0),mUpdDC(0),mUpdMC(0),mUpdSC(0),mCreateTime(0),mIdentId(0),mProtect(0)
		{
			mUpdMaxCount=0;
			mUpdFailedCount=0;
			mSellPriceType=0;
			mSellPrice=0;
			mZLevel=0;
			mLock=0;
			mSubType=-1;
			mLasttime = 0;
			//mBoothType = 0;
			//mBoothPrice = 0;
			//mSaleStatus = 0;
			//mSaleMoney = 0;
			//mSaleVcoin = 0;
			//mSalePage = -1;
			//mSalePos = -1;
		}
	public:
		bool BuildItemInstance(ItemDef* id,int flags,bool rand_add,int add_type,int number=1);
		void ItemAddType0(ItemDef* id,int flags);
		int ItemAddType1(ItemDef* id,int flags);
		void BuildItemWeapon(ItemDef* id);
		void BuildItemEquip(ItemDef* id);
		void ApplyAttribute(DomainPlay* play,ItemDef* id,ObjectAttribute* attr,ObjectAttribute* baseattr,ScriptItemAttibuteParam* scriptattr);
		void ApplyAttributePlus(DomainPlay* play,ItemDef* id,ItemPlusDef* ipd,ObjectAttribute* attr,ObjectAttribute* baseattr);
		int RepairPrice(ItemDef* id);
		//bool Upgrade(ItemDef* thisID,ItemDef* steelID);
		//bool Degrade(ItemDef* thisID,ItemDef* steelID);
		int RemainTime(ItemDef* id);

		int ItemUpgrade(DomainPlay* play,int updac,int updmac,int upddc,int updmc,int updsc);
		//新增接口用于直接设置强化等级并做记录
		int ItemUpgrade(DomainPlay* play, int level);

		int ItemZhuLing(DomainPlay* play);
		int RemoveZhuLing(DomainPlay* play);

		static LEUD_INT64 GenSubItemId();
	};
}