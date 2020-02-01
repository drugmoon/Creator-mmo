#pragma once
#include "SubItem.h"
#include "leudgrid/MemoryPool.h"
#include <set>

namespace OGSLib
{
	class GameServerLib;
	class DomainPlay;
	// 寄售物品信息
	class ConsignableItemInfo : public LEUD::PoolMemoryBase< ConsignableItemInfo >
	{
	public:
		int seed_id;
		SubItem item;
		std::string seller_seed_name;
		std::string seller_name;
		// 寄售结束时间
		time_t end_time;
		// 物品标价
		int price;

		// 穿戴等级权值,排序使用
		int dress_weight;
		// 穿戴位置权值
		int equiptype_weight;

		//宽字符名字,搜索使用
		std::wstring ws_item_name;
	public:
		ConsignableItemInfo();

		bool operator >=(const ConsignableItemInfo val) const;
	};

	class ManagerConsignment
	{
	public:
		typedef std::vector<ConsignableItemInfo *> ConsignableItemList;
		typedef std::map<std::string, int> UserVCoinMap;
	public:
		ManagerConsignment(GameServerLib* gslib);
		virtual ~ManagerConsignment(void);

		int ConsignItem(DomainPlay * src, SubItem * item, int count, int price, int duration_hour);
		int BuyConsignableItem(DomainPlay * src, int seed_id);
		int TakeBackConsignableItem(DomainPlay * src, int seed_id);
		int TakeBackVCoin(DomainPlay * src);
		int GetVCoin(DomainPlay * src);
		void GetConsignableItems(DomainPlay * src, int type, int begin_index, int job, int condition, std::string filter, std::vector<ConsignableItemInfo *>& vec, std::vector<int>& index_list);
		
		void init();
		void update();
		void dest(bool save=true);
		void LoadConsignment();
		bool OnLoadConsignment(const std::vector<ConsignableItemInfo *>& consignmentVector, UserVCoinMap& user_vcoin_map);
		void SaveConsignment();
		ConsignableItemList & GetConsignableItemInfoList() {return mItemAllInfos;};
		ConsignableItemList & GetOverTimeItemInfoList() {return mItemOverTime;};
		UserVCoinMap& GetVCoinMap() {return mVcoinMap;};
	private:
		void AddConsignableItem(ConsignableItemInfo * pItem);
		// 查找物品,只在寄售列表里查找
		ConsignableItemInfo * FindConsignableItem(int seed_id);
		// 只删除,不释放
		void DelConsignableItem(ConsignableItemInfo * pItem);

		size_t GetInsertPos(ConsignableItemList & v, ConsignableItemInfo * key);

		void UpdateItemsOverTime();
	private:
		GameServerLib* mGameServerLib;
		// 寄售中所有物品
		ConsignableItemList mItemAllInfos;
		// 装备
		ConsignableItemList mItemsEquip;
		// 药品
		ConsignableItemList mItemsEatable;
		// 材料
		ConsignableItemList mItemsMaterial;
		// 其他
		ConsignableItemList mItemsOther;
		// 玩家物品
		std::map<std::string, ConsignableItemList> mUserItemMap;
		// 过期物品
		ConsignableItemList mItemOverTime;
		
		// 出售物品收益
		UserVCoinMap mVcoinMap;

		time_t mSaveTick;
		time_t mUpdateTick;
	/*寄售日志操作码:*/
	/*
		0:放入寄售商店
		1:购买
		2:取回
	*/
		static int m_seed_id;
	};
}