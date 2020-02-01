#pragma once
#include "PlayMapObject.h"
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{
	enum EnumTrapTypeId
	{
		eTrapIdMapConn =0,
		eTrapIdHuoQiang,
		eTrapIdEnd,
		eTrapIdUserEefBegin = 10000,
	};

	struct TrapDef;

	// 陷阱类(火墙...释放在地面上造成伤害)
	class PlayMapTrap :
		public PlayMapObject,
		public LEUD::PoolMemoryBase< PlayMapTrap >
	{
		std::string mName;

		PlayActiveObject * mCaller;
		int mPickLimitLv;
		// 存在时间s
		int mLifeTime;
		// 触发周期ms
		int mDamageGap;
		// 触发伤害值
		int mDamage;
		// 伤害类型
		int mSkillType;
		// 触发特效id
		int mEffect;
		// 陷阱类型id
		int mTrapTypeID;

		bool mCallerLeaveRemove;
	private:
		// 0 永久
		int mTriggerTimes;
		bool mLifeTimeRemove;
	public:
		PlayMapTrap(int x,int y, TrapDef * def, PlayActiveObject * caller);
		virtual ~PlayMapTrap(void);
	public:
		void ClearCaller();
		int GetTrapTypeId();
		void ClothID(int id){mCloth = id;}
		bool GetCallerLeaveRemove();
		void RemoveFromMap();
		void OnTrapGap();
		virtual const char* Name();
		virtual int Hp(){return 0;}
		virtual int MaxHp(){return 0;}
		virtual int Power(){return 0;}
		virtual int DefId(){return mTrapTypeID;}
	public:
		virtual void OnSecondTimer(int param);
	public:
		virtual void NotifyMapEnter(PlayMap* map);
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src){};
		virtual void NotifyMapLeave(PlayMap* map);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src){}
		virtual void NotifyAvatarChange(PlayMapObject* src){}

		virtual void NotifyWalkOut(PlayActiveObject* src);
		virtual void NotifyWalkIn(PlayActiveObject* src);
	};
}