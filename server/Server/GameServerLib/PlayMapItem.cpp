#include "StdAfx.h"
#include "PlayMapItem.h"
#include "DomainPlay.h"
#include "PlayMap.h"
#include "ManagerMap.h"
#include "ManagerItemDef.h"
#include "GameServerLib.h"
namespace OGSLib
{

	PlayMapItem::PlayMapItem(int x,int y,SubItem item)
	{
		mType=MAP_OBJECT_TYPE_MAP_ITEM;
		SetPosition(x,y);
		mItem = item;
		mCloth = 0;
		mOwnerID = 0;
		mLifeTime = 120;
		mResetOwnerTime = 30;
		mPickLimitLv = 0;
	}

	PlayMapItem::~PlayMapItem(void)
	{
	}
	const char* PlayMapItem::Name()
	{
		if( ! mName.empty() )
		{
			return mName.c_str();
		}
		ItemDef* id = mParentMap->GetManager()->GetGSLib()->GetItemDefineManager()->GetItemDefine(mItem.mTypeID);
		if( id )
		{
			mName = id->mName;
		}
		return mName.c_str();
	}
	void PlayMapItem::NotifyMeet(PlayMap* map,PlayMapObject* src)
	{
		if( src->TypeIsPlayer() )
		{
			DomainPlay* player = (DomainPlay*)src;
			player->PushItemInfo(mItem.mTypeID);
		}
	}
	void PlayMapItem::NotifyMapEnter(PlayMap* map)
	{
		PlayMapObject::NotifyMapEnter(map);
		mGSL->GetSecondTimer()->AddTimer(mLifeTime,this,mItem.mTypeID);
		mGSL->GetSecondTimer()->AddTimer2(mResetOwnerTime, this, mItem.mTypeID);////为掉落在地面上的物品添加一个计时器(60s后将物品的ownerid置0，从而发给客户端,让玩家可以捡取物品)
		mLockTime = mGSL->GetTimerFix()->Elapsed();
	}
	void PlayMapItem::NotifyMapLeave(PlayMap* map)
	{
		mParentMap->GetManager()->GetGSLib()->GetFrameTimer()->RemoveObject(this);
		mParentMap->GetManager()->GetGSLib()->GetSecondTimer()->RemoveObject(this);
		PlayMapObject::NotifyMapLeave(map);
	}
	void PlayMapItem::PickUped(PlayMapObject* src)
	{
		if( src->TypeIsPlayer() )
		{
			DomainPlay* player = (DomainPlay*)src;
			if( mOwnerID != 0 && src->ID() != mOwnerID )
			{
				if( ! player->IsGroupMember(mOwnerID) )
				{
					if( mParentMap )
					{
						if (this->mLockTime + mResetOwnerTime > this->mParentMap->GetManager()->GetGSLib()->GetTimerFix()->Elapsed())
						{
							player->AlertMessage(10,0,mParentMap->GetManager()->GetGSLib()->mStringConsts.NOTIFY_ITEM_LOCK_CAN_NOT_PICK_UP);
							return;
						}
					}
				}
			}

			if (mOwnerIDList.size() > 0)
			{
				bool canPick = false;
				for (size_t i = 0; i < mOwnerIDList.size(); ++ i)
				{
					if (src->ID() == mOwnerIDList[i])
					{
						canPick = true;
						break;
					}

					if (player->IsGroupMember(mOwnerIDList[i]))
					{
						canPick = true;
						break;
					}
				}

				if (!canPick && this->mLockTime + mResetOwnerTime > this->mParentMap->GetManager()->GetGSLib()->GetTimerFix()->Elapsed())
				{
					player->AlertMessage(10,0,mParentMap->GetManager()->GetGSLib()->mStringConsts.NOTIFY_ITEM_LOCK_CAN_NOT_PICK_UP);
					return;
				}
			}

			if (player->level() < this->mPickLimitLv)
			{
				player->AlertMessage(11, 1, mParentMap->GetManager()->GetGSLib()->mStringConsts.NOTIFY_ITEM_LEVEL_LIMINT_CAN_NOT_PICK_UP);
				return;
			}
			if (item_type() >= ITEM_BAOSHI_BEGIN && item_type() <= ITEM_BAOSHI_END)
			{
				if (player->NumXjBlack() > 0)
				{
					player->AddItem(&mItem, mNumber, 155, 0, 1);
					mParentMap->RemoveObject(this);
					delete this;
				}
				else
				{
					player->AlertMessage(10, 0, mParentMap->GetManager()->GetGSLib()->mStringConsts.NOTIFY_ITEM_SLOB_FULL_CAN_NOT_PICK_UP);
				}
			}
			else if( player->NumBagBlack() > 0 || item_type()==ITEM_GOLD_ID || item_type()==ITEM_GOLD_BIND_ID || item_type()==ITEM_VCOIN_ID || item_type()==ITEM_VCOIN_BIND_ID)
			{
				player->AddItem(&mItem,mNumber,155,0,1);
				mParentMap->RemoveObject(this);
				delete this;
			}
			else
			{
				player->AlertMessage(10,0,mParentMap->GetManager()->GetGSLib()->mStringConsts.NOTIFY_ITEM_SLOB_FULL_CAN_NOT_PICK_UP);
			}
		}
	}
	void PlayMapItem::OnSecondCancelTimer(int param)
	{
		//printf("c %d\n",mItem.mTypeID);
	}
	void PlayMapItem::OnSecondTimer(int param)
	{
		mParentMap->RemoveObject(this);
		//printf("d %d\n",mItem.mTypeID);
		delete this;
	}
    void PlayMapItem::OnRestItemOwner()
	{
		if (mParentMap)
			mParentMap->ResetOwner(this);
	}
}
LEUD::PoolMemory<OGSLib::PlayMapItem>	LEUD::PoolMemoryBase<OGSLib::PlayMapItem>::memPool("PlayMapItem");
