#pragma once
#include "PlayMapObject.h"
#include "SubItem.h"
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{

	class PlayMapItem :
		public PlayMapObject,
		public LEUD::PoolMemoryBase< PlayMapItem >
	{
		SubItem mItem;
		std::string mName;
		int mOwnerID;
		int mNumber;
		double mLockTime;
		int mLifeTime;
		int mResetOwnerTime;
		int mPickLimitLv;
		std::vector<int> mOwnerIDList;
	public:
		PlayMapItem(int x,int y,SubItem item);
		virtual ~PlayMapItem(void);
	public:
		void OwnerID(int id){mOwnerID = id;}
		int OwnerID(){return mOwnerID;}
		std::vector<int> & OwnerIDList() {return mOwnerIDList;}
		void PushOwnerID(int id) {mOwnerIDList.push_back(id);}
		void ClothID(int id){mCloth = id;}
		void Number(int num){mNumber = num;}
		int item_type(){return mItem.mTypeID;}
		void life_time(int time){mLifeTime=time;}
		void ResetOwnerTime(int time){ mResetOwnerTime = time; }
		void SetPickLimitLv(int lv){ mPickLimitLv = lv; }
		
		virtual const char* Name();
		virtual int Hp(){return 0;}
		virtual int MaxHp(){return 0;}
		virtual int Power() {return 0;}
		virtual int DefId(){return mItem.mTypeID;}
	public:
		virtual void PickUped(PlayMapObject* src);
		virtual void OnSecondTimer(int param);
		virtual void OnSecondCancelTimer(int param);
		virtual void OnRestItemOwner();
	public:
		virtual void NotifyMapEnter(PlayMap* map);
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void NotifyMapLeave(PlayMap* map);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src){}
		virtual void NotifyAvatarChange(PlayMapObject* src){}
	};
}