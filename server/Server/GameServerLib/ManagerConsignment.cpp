#include "stdafx.h"
#include "ManagerConsignment.h"
#include "GameServerLib.h"
#include "ManagerItemDef.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include <sstream>
#include "DomainPlay.h"
#include "GameSession.h"
#include "ScriptLua.h"
#include <codecvt>

#define CONSIGNMEMT_PAGE_SIZE 20 
namespace OGSLib
{
	int ManagerConsignment::m_seed_id = 0;

	ManagerConsignment::ManagerConsignment(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}

	ManagerConsignment::~ManagerConsignment(void)
	{

	}

	int ManagerConsignment::ConsignItem(DomainPlay * src, SubItem * item, int count, int price, int duration_hour)
	{
		if (duration_hour != 12 && duration_hour != 24 && duration_hour != 48)
			duration_hour = 48;

		ItemDef * def = mGameServerLib->GetItemDefineManager()->GetItemDefine(item->mTypeID);
		if (!def || price < 0)
		{
			return -1;
		}

		int nFee = 0;
		if (duration_hour == 12)
			nFee = mGameServerLib->mConsignFee12;
		else if (duration_hour == 24)
			nFee = mGameServerLib->mConsignFee24; 
		else if (duration_hour == 48)
			nFee = mGameServerLib->mConsignFee48; 

		// 扣钱
		if (nFee > 0)
		{
			if (!src->RemGameMoney(nFee,200))
				return 1;
		}

		std::map<std::string, ConsignableItemList>::iterator it = mUserItemMap.find(src->Name());

		if (it != mUserItemMap.end())
		{
			// 数量已达上限
			if (it->second.size() >= CONSIGNMEMT_PAGE_SIZE)
			{
				return 3;
			}
		}

		time_t t_now = time(0);
		SubItem item_sell;
		if (item->mNumber > count)
		{
			// 拆分物品
			item->mNumber -= count;
			item_sell.BuildItemInstance(def,item->mItemFlags,false,mGameServerLib->mItemAddType,count);
			src->NotifyItemChange(*item, 0);
		}
		else
		{		
			item_sell = *item;
			item->mTypeID = 0;
			src->NotifyItemChange(*item, 0);
			src->SetNeedClearItemList(true);
		}
		// opcode 0:放入寄售商店,1:购买,2:退回
		// 寄售商店记录日志
		ConsignableItemInfo *item_info = new ConsignableItemInfo;
		item_info->seed_id = ++ m_seed_id;
		item_info->item = item_sell;
		item_info->item.mPosition = 0;
		item_info->price = price;
		item_info->end_time = t_now + (duration_hour * 60 * 60);
		item_info->seller_name = src->Name();
		item_info->seller_seed_name = src->SeedName();

		this->AddConsignableItem(item_info);

		return 0;
	}

	void ManagerConsignment::AddConsignableItem(ConsignableItemInfo * pItem)
	{
		// 将不同种物品加入不同列表中
		ItemDef* item_def = mGameServerLib->GetItemDefineManager()->GetItemDefine(pItem->item.mTypeID);
		if (!item_def)
		{
			delete pItem;
			return;
		}

		time_t now = time(0);
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		pItem->ws_item_name = conv.from_bytes(item_def->mName);
		// 先判断物品是否过期
		if (pItem->end_time <= now)
		{
			mItemOverTime.push_back(pItem);

			int pos = 0;
			ConsignableItemList& list = mUserItemMap[pItem->seller_name];
			pos = GetInsertPos(list, pItem);
			list.insert(list.begin() + pos, pItem);
			return;
		}

		pItem->dress_weight = item_def->mNeedZsLevel * 1000 + item_def->mNeedParam;
		pItem->equiptype_weight = 1000-item_def->mEquipType;
		// 所有物品列表增加
		size_t pos = GetInsertPos(mItemAllInfos, pItem);
		mItemAllInfos.insert(mItemAllInfos.begin() + pos, pItem);

		if (ItemDef::IsEquipment(pItem->item.mSubType))
		{
			pos = GetInsertPos(mItemsEquip, pItem);
			mItemsEquip.insert(mItemsEquip.begin() + pos, pItem);
		}
		else if(ItemDef::IsEatable(pItem->item.mSubType))
		{
			pos = GetInsertPos(mItemsEatable, pItem);
			mItemsEatable.insert(mItemsEatable.begin() + pos, pItem);
		}
		else if (ItemDef::IsMaterial(pItem->item.mSubType))
		{
			pos = GetInsertPos(mItemsMaterial, pItem);
			mItemsMaterial.insert(mItemsMaterial.begin() + pos, pItem);
		}
		else
		{
			pos = GetInsertPos(mItemsOther, pItem);
			mItemsOther.insert(mItemsOther.begin() + pos, pItem);
		}

		// 按人物划分
		ConsignableItemList& list = mUserItemMap[pItem->seller_name];
		pos = GetInsertPos(list, pItem);
		list.insert(list.begin() + pos, pItem);
	}

	void ManagerConsignment::update()
	{
		// 一分钟更新一次
		if (mUpdateTick + 60 < time(0))
		{
			mUpdateTick = time(0);
			UpdateItemsOverTime();
		}

		// 五分钟保存一次
		if( mSaveTick + 60 * 5 < time(0) )
		{
			mSaveTick = time(0);
			SaveConsignment();
		}
	}

	void ManagerConsignment::init()
	{
		LoadConsignment();
		mSaveTick = time(0);
		mUpdateTick = 0;
	}

	void ManagerConsignment::LoadConsignment()
	{
		mGameServerLib->GetListener()->LoadConsignment(0);
	}

	void ManagerConsignment::SaveConsignment()
	{
		mGameServerLib->GetListener()->SaveConsignment(*this,0);
	}

	void ManagerConsignment::dest(bool save/*=true*/)
	{
		if( save )
		{
			SaveConsignment();
		}
		ConsignableItemList::iterator it = mItemAllInfos.begin();
		while (it != mItemAllInfos.end())
		{
			SAFE_DELETE(*it);
			++ it;
		}
		mItemAllInfos.clear();

		it = mItemOverTime.begin();
		while (it != mItemOverTime.end())
		{
			SAFE_DELETE(*it);
			++ it;
		}
		mItemOverTime.clear();
	}

	bool ManagerConsignment::OnLoadConsignment(const std::vector<ConsignableItemInfo *>& consignmentVector, UserVCoinMap& user_vcoin_map)
	{
		for (size_t i = 0; i < consignmentVector.size(); ++ i)
		{
			consignmentVector[i]->seed_id = ++ m_seed_id;
			this->AddConsignableItem(consignmentVector[i]);
		}
		mVcoinMap = user_vcoin_map;
		return true;
	}

	void ManagerConsignment::GetConsignableItems(DomainPlay * src, int type, int begin_index, int job,int condition, std::string filter, std::vector<ConsignableItemInfo *>& vec, std::vector<int>& index_list)
	{
		ConsignableItemList * list = NULL;
		switch (type)
		{
		case 0:
			list = &mItemAllInfos;
			break;
		case 1:
			list = &mItemsEquip;
			break;
		case 2:
			list = &mItemsEatable;
			break;
		case 3:
			list = &mItemsMaterial;
			break;
		case 4:
			list = &mItemsOther;
			break;
		default:
			break;
		}
		if (type == 5)
		{
			// 列出自己寄售的物品
			std::map<std::string, ConsignableItemList>::iterator it = mUserItemMap.find(src->Name());
			if (it != mUserItemMap.end())
			{
				ConsignableItemList & items = it->second;
				vec.insert(vec.begin(), items.begin(), items.end());
			}
			return;
		}
		
		if (begin_index >= list->size())
		{
			return;
		}

		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

		do 
		{
			// 判断职业
			if (job == JOB_WARRIOR)
			{
				// 判断物品职业
				if (!ItemDef::IsEquipWarrior(list->at(begin_index)->item.mTypeID))
				{
					++ begin_index;
					continue;
				}
			}
			if (job == JOB_WIZARD)
			{
				if (!ItemDef::IsEquipWizard(list->at(begin_index)->item.mTypeID))
				{
					++ begin_index;
					continue;
				}
			}
			if (job == JOB_TAOIST)
			{
				if (!ItemDef::IsEquipTaoist(list->at(begin_index)->item.mTypeID))
				{
					++ begin_index;
					continue;
				}
			}

			// 判断条件 等级
			ItemDef* item_def = mGameServerLib->GetItemDefineManager()->GetItemDefine(list->at(begin_index)->item.mTypeID);
			if (condition != 0)
			{
				// 0 全部显示
				if (condition == 1)
				{
					//0-80
					if (item_def->mNeedParam > 80 || item_def->mNeedZsLevel >=2)
					{
						++ begin_index;
						continue;
					}
				}

				// 80-2转
				if (condition == 2)
				{
					if (item_def->mNeedZsLevel > 2 || item_def->mNeedParam <= 80)
					{
						++ begin_index;
						continue;
					}
				}

				// 2转以上
				if (condition == 3)
				{
					if (item_def->mNeedZsLevel <= 2)
					{
						++ begin_index;
						continue;
					}
				}
			}

			if (filter != "")
			{
				std::wstring w_filter = conv.from_bytes(filter.c_str());
				if (list->at(begin_index)->ws_item_name.find_first_of(w_filter) == std::wstring::npos)
				{
					++ begin_index;
					continue;
				}
			}

			vec.push_back(list->at(begin_index));
			index_list.push_back(begin_index);
			++ begin_index;

			if (vec.size() == CONSIGNMEMT_PAGE_SIZE)
			{
				break;
			}

		} while (begin_index < list->size());
	}

	void ManagerConsignment::DelConsignableItem(ConsignableItemInfo * pItem)
	{
		ConsignableItemList::iterator it = std::find(mItemAllInfos.begin(), mItemAllInfos.end(), pItem);
		if (it != mItemAllInfos.end())
			mItemAllInfos.erase(it);

		it = std::find(mItemsEquip.begin(), mItemsEquip.end(), pItem);
		if (it != mItemsEquip.end())
			mItemsEquip.erase(it);

		it = std::find(mItemsEatable.begin(), mItemsEatable.end(), pItem);
		if (it != mItemsEatable.end())
			mItemsEatable.erase(it);

		it = std::find(mItemsMaterial.begin(), mItemsMaterial.end(), pItem);
		if (it != mItemsMaterial.end())
			mItemsMaterial.erase(it);

		it = std::find(mItemsOther.begin(), mItemsOther.end(), pItem);
		if (it != mItemsOther.end())
			mItemsOther.erase(it);

		it = std::find(mItemOverTime.begin(), mItemOverTime.end(), pItem);
		if (it != mItemOverTime.end())
			mItemOverTime.erase(it);

		std::map<std::string, ConsignableItemList>::iterator it2 = mUserItemMap.find(pItem->seller_name);
		if (it2 != mUserItemMap.end())
		{
			ConsignableItemList & list = it2->second;

			it = std::find(list.begin(), list.end(), pItem);
			if (it != list.end())
				list.erase(it);

			if (list.size() == 0)
			{
				mUserItemMap.erase(it2);
			}
		}
	}

	ConsignableItemInfo * ManagerConsignment::FindConsignableItem(int seed_id)
	{
		for (size_t i = 0; i < mItemAllInfos.size(); ++ i)
		{
			if (mItemAllInfos[i]->seed_id == seed_id)
			{
				return mItemAllInfos[i];
			}
		}
		return NULL;
	}

	size_t ManagerConsignment::GetInsertPos(ConsignableItemList & v, ConsignableItemInfo * key)
	{
		size_t len = v.size();
		if (len == 0)
			return 0;

		size_t high = len - 1;
		size_t low = 0;
		size_t mid = 0;

		if ((*key) >= *(v[low]))
		{
			return low;
		}

		if (*(v[high]) >= (*key))
		{
			return high + 1;
		}

		while (low <= high)
		{
			mid = (low + high) / 2;
			if ((*key) >= *(v[mid]))
			{
				high = mid - 1;
			}
			else
			{
				low = mid + 1;
			}
		}

		return low;
	}

	int ManagerConsignment::BuyConsignableItem(DomainPlay * src, int seed_id)
	{
		ConsignableItemInfo * pItem = FindConsignableItem(seed_id);
		// 不存在该物品
		if (pItem == NULL)
		{
			return -1;
		}
		int empty_num = src->get_empty_item_position_number();
		if (empty_num < 1)
		{
			// 背包放不下
			return 1;
		}
		if (!src->RemVcoin(pItem->price, 200))
		{
			// 钱不够
			return 2;
		}

		int nBenefit = pItem->price;
		if (pItem->price >= 10)
		{
			int nTax = ceil((double)pItem->price * mGameServerLib->mConsignTex / 100);
			nBenefit = pItem->price - nTax;
		}

		mVcoinMap[pItem->seller_name] += nBenefit;

		std::string seller = pItem->seller_name;

		// 将物品放入背包
		src->AddItem(&(pItem->item),pItem->item.mNumber,105,11,1);

		
		ItemDef* item_def = mGameServerLib->GetItemDefineManager()->GetItemDefine(pItem->item.mTypeID);
		if (item_def)
		{
			// 购买物品触发
			mGameServerLib->GetScriptEngine()->ItemPlayer(src, item_def->mName.c_str(), item_def->mTypeID, "player.onBuyConsignItem", pItem->item.mNumber, nBenefit, pItem->seller_seed_name.c_str());
		}

		// 删除物品
		DelConsignableItem(pItem);
		SAFE_DELETE(pItem);

		// 卖出物品触发
		OGSLib::GameSession* gs = mGameServerLib->FindSessionWithCharID(seller.c_str());

		if (gs && gs->GetPlayer())
		{
			mGameServerLib->GetScriptEngine()->OnlyPlayer(gs->GetPlayer(), "player.onConsignItemSold");
		}

		return 0;
	}

	int ManagerConsignment::TakeBackVCoin(DomainPlay * src)
	{
		UserVCoinMap::iterator it = mVcoinMap.find(src->Name());

		if (it == mVcoinMap.end())
		{
			return 0;
		}

		int nRet = 0;
		if (it->second > 0)
		{
			nRet = it->second;
			src->AddVcoin(it->second, 200);
			mVcoinMap.erase(it);

			mGameServerLib->GetScriptEngine()->OnlyPlayer(src, "player.onTakeConsignVCoin");
		}

		return nRet;
	}

	int ManagerConsignment::GetVCoin(DomainPlay * src)
	{
		UserVCoinMap::iterator it = mVcoinMap.find(src->Name());

		if (it == mVcoinMap.end())
			return 0;

		return it->second;
	}

	int ManagerConsignment::TakeBackConsignableItem(DomainPlay * src, int seed_id)
	{
		std::map<std::string, ConsignableItemList>::iterator it = mUserItemMap.find(src->Name());

		if (it == mUserItemMap.end())
		{
			return -1;
		}

		ConsignableItemList & list = it->second;

		ConsignableItemInfo * pItem = NULL;
		for (size_t i = 0; i < list.size(); ++ i)
		{
			if (list[i]->seed_id == seed_id)
			{
				pItem = list[i];
				break;
			}
		}

		if (pItem == NULL)
			return -1;

		// 取回物品
		src->AddItem(&(pItem->item),pItem->item.mNumber,105,11,1);

		// 删除物品
		DelConsignableItem(pItem);
		SAFE_DELETE(pItem);

		return 0;
	}

	void ManagerConsignment::UpdateItemsOverTime()
	{
		ConsignableItemList::iterator it = mItemAllInfos.begin();
		while (it != mItemAllInfos.end())
		{
			time_t now = time(0);
			if (now >= (*it)->end_time )
			{
				ConsignableItemInfo * pItem = (*it);
				// 过期
				ConsignableItemList::iterator it2 = std::find(mItemsEquip.begin(), mItemsEquip.end(), pItem);
				if (it2 != mItemsEquip.end())
					mItemsEquip.erase(it2);

				it2 = std::find(mItemsEatable.begin(), mItemsEatable.end(), pItem);
				if (it2 != mItemsEatable.end())
					mItemsEatable.erase(it2);

				it2 = std::find(mItemsMaterial.begin(), mItemsMaterial.end(), pItem);
				if (it2 != mItemsMaterial.end())
					mItemsMaterial.erase(it2);

				it2 = std::find(mItemsOther.begin(), mItemsOther.end(), pItem);
				if (it2 != mItemsOther.end())
					mItemsOther.erase(it2);

				mItemOverTime.push_back(pItem);

				it = mItemAllInfos.erase(it);
				continue;
			}
			++ it;
		}
	}

	ConsignableItemInfo::ConsignableItemInfo()
	{
		seed_id = 0;
		price = 0;
		end_time = 0;
		dress_weight = 0;
		equiptype_weight = 0;
	}

	bool ConsignableItemInfo::operator>=(const ConsignableItemInfo val) const
	{
		//1.分类 装备>药品>材料>其他
		int type0 = 0;
		int type1 = 0;

		if (ItemDef::IsEquipment(this->item.mSubType))
			type0 = 4;
		else if (ItemDef::IsEatable(this->item.mSubType))
			type0 = 3;
		else if (ItemDef::IsMaterial(this->item.mSubType))
			type0 = 2;
		else
			type0 = 1;

		if (ItemDef::IsEquipment(val.item.mSubType))
			type1 = 4;
		else if (ItemDef::IsEatable(val.item.mSubType))
			type1 = 3;
		else if (ItemDef::IsMaterial(val.item.mSubType))
			type1 = 2;
		else
			type1 = 1;

		if (type0 > type1)
		{
			return true;
		}
		else if (type0 < type1)
		{
			return false;
		}
		else
		{
			// 类型相同
			// 对比穿戴等级
			if (this->dress_weight > val.dress_weight)
				return true;
			else if (this->dress_weight < val.dress_weight)
				return false;
			else
			{
				// 穿戴等级形同,对比类型
				if (this->equiptype_weight > val.equiptype_weight)
					return true;
				else if(this->equiptype_weight < val.equiptype_weight)
					return false;
				else
				{
					if (this->end_time < val.end_time)
						return true;
					else if (this->end_time > val.end_time)
						return false;
					else
						return true;
				}
			}
		}
		return false;
	}
}
LEUD::PoolMemory<OGSLib::ConsignableItemInfo>	LEUD::PoolMemoryBase<OGSLib::ConsignableItemInfo>::memPool("ConsignableItemInfo");