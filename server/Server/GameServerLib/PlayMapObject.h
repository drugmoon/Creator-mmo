#pragma once
#include "TimerFrame.h"
#include "TimerSecond.h"
#include "PlayMapObjectNode.h"
#define ENETERED_MAPCELL_NUM 50
namespace OGSLib
{
	struct IntPointClass
	{
		IntPointClass(int vx,int vy)
		{
			x = vx;
			y = vy;
		}
		int x;
		int y;
	};
	class PlayMap;
	class PlayActiveObject;
	class GameServerLib;
	class PlayMapObject : public FrameTimerObject,public SecondTimerObject
	{
		friend PlayMap;
		static int ID_SEED;
		int mID;
		int mX;
		int mY;
		int mEnteredMapCell[ENETERED_MAPCELL_NUM];
		int mEnteredMapCellNum;
		int mState;
		int mSettingPosition;
	protected:
		enum MapObjectType
		{
			MAP_OBJECT_TYPE_NPC = 500,
			MAP_OBJECT_TYPE_PLAYER = 501,
			MAP_OBJECT_TYPE_MONSTER = 502,
			MAP_OBJECT_TYPE_MAP_ITEM = 503,
			MAP_OBJECT_TYPE_SLAVE = 504,
			MAP_OBJECT_TYPE_NEUTRAL = 505,
			MAP_OBJECT_TYPE_NULL = 506,
			MAP_OBJECT_TYPE_ROBOT=507,
			MAP_OBJECT_TYPE_DART = 508,
			MAP_OBJECT_TYPE_TRAP = 509,
		};
		MapObjectType mType;
		bool mInteractive;
		int mDir;
		int mCloth;
		int mWeapon;
		int mHair;
		int mFashion;
		int mClothIdOn;////选角面板需要用到的一些数据
		int mWeaponIdOn;
		int mFashionClothIdOn;
		int mFashionWeaponIdOn;

		
		int mMapZoneID;
		int mSafeArea;
		int mEventArea;
		PlayMap* mParentMap;
		GameServerLib* mGSL;
	public:
		PlayMapObjectNodeList* mMapZone;
		PlayMapObjectNode mMapNode;
		PlayMapObjectNode mZoneNode;
		PlayMapObjectNode mCellNode;
		PlayMapObject(void);
		virtual ~PlayMapObject(void);
	public:
		enum MapObjectState
		{
			MAP_OBJECT_STATE_IDLE = 200,
			MAP_OBJECT_STATE_DEAD = 201,
		};
		enum MapObjectDir
		{
			DIR_UP=0,
			DIR_UP_RIGHT=1,
			DIR_RIGHT=2,
			DIR_DOWN_RIGHT=3,
			DIR_DOWN=4,
			DIR_DOWN_LEFT=5,
			DIR_LEFT=6,
			DIR_UP_LEFT=7,
			DIR_COUNT=8,
		};
		static const int cDirWalkX[];
		static const int cDirWalkY[];
	public:
		int ID(){return mID;}
		int Type(){return mType;}
		int NextX(int x,int dir){return x+cDirWalkX[dir];}
		int NextY(int y,int dir){return y+cDirWalkY[dir];}
		IntPointClass NextPosition(int dir,int step,int blockcheckflag=0);
		int Diff2Dir(float x,float y);
		int NextDir(int dir,int shift);
		int SetPosition(int x,int y);
		int SetSafeArea(int safearea);
		int SetEventArea(int eventarea, std::string eventid);
		void SetState(int state);
		PlayMap* GetMap(){return mParentMap;}
		bool TypeIsPlayer(){return MAP_OBJECT_TYPE_PLAYER==mType;}
		bool TypeIsMonster(){return MAP_OBJECT_TYPE_MONSTER==mType;}
		bool TypeIsNPC(){return MAP_OBJECT_TYPE_NPC==mType;}
		bool TypeIsMapItem(){return MAP_OBJECT_TYPE_MAP_ITEM==mType;}
		bool TypeIsSlave(){return MAP_OBJECT_TYPE_SLAVE==mType;}
		bool TypeIsNeutral(){return MAP_OBJECT_TYPE_NEUTRAL==mType;}
		bool TypeIsRobot(){return MAP_OBJECT_TYPE_ROBOT==mType;}
		bool TypeIsDart(){return MAP_OBJECT_TYPE_DART==mType;}
		bool TypeIsTrap(){return MAP_OBJECT_TYPE_TRAP==mType;}
		bool IsInteractive(){return mInteractive;}

	public:
		virtual const char* Name()=0;
		virtual	int DefId() = 0;
		int X(){return mX;}
		int Y(){return mY;}
		int Dir(){return mDir;}
		void Dir(int d){mDir = d;}
		int Cloth(){return mCloth;}
		int Weapon(){return mWeapon;}
		int Hair(){return mHair;}
		int SafeArea(){return mSafeArea;}
		int State(){return mState;}
		bool IsState(int state){return mState == state;}
		virtual int Hp()=0;
		virtual int MaxHp()=0;
		virtual int Power()=0;
		void FreshEnteredMapCell();
		void ClearEnteredMapCell(int id);
		bool IsEnteredMapCell(int id);
		void AddEnterMapCell(int id);

	public:
		virtual void NotifyMapEnter(PlayMap* map);
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src)=0;
		virtual void NotifyMapLeave(PlayMap* map);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src)=0;
		virtual void NotifyAvatarChange(PlayMapObject* src)=0;
		virtual void OnLeaveSafeArea(){}
		virtual void OnEnterSafeArea(int sa){}
		virtual void OnEnterEventArea(std::string id){}
		virtual void OnLeaveEventArea(std::string id){}
	};
}