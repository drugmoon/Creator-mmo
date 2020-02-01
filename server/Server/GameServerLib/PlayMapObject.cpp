#include "StdAfx.h"
#include "PlayMapObject.h"
#include "PlayMap.h"
#include "ManagerMap.h"
#include "PlayMapTrap.h"

namespace OGSLib
{
	const int PlayMapObject::cDirWalkX[]={0,1,1,1,0,-1,-1,-1};
	const int PlayMapObject::cDirWalkY[]={-1,-1,0,1,1,1,0,-1};
	int PlayMapObject::ID_SEED = 1000;
	PlayMapObject::PlayMapObject(void)
	{
		mState = MAP_OBJECT_STATE_IDLE;
		mID = ID_SEED++;
	
		mCloth=-1;
		mWeapon=-1;
		mHair=-1;
		mClothIdOn=-1;
		mWeaponIdOn=-1;
		mFashionClothIdOn=-1;
		mFashionWeaponIdOn=-1;
		mX=0;
		mY=0;
		mDir=0;
		mSafeArea=-1;
		mEventArea = -1;
		mParentMap = 0;
		mGSL = 0;
		mMapZone = 0;
		mMapZoneID = -1000;

		mType=MAP_OBJECT_TYPE_NULL;
		mInteractive=false;

		for( int i = 0;i < ENETERED_MAPCELL_NUM;i ++ )
		{
			mEnteredMapCell[i] = -1;
		}
		mEnteredMapCellNum=0;
		mMapNode.This = this;
		mZoneNode.This = this;
		mCellNode.This = this;
		mSettingPosition = 0;
	}

	PlayMapObject::~PlayMapObject(void)
	{
	}
	int PlayMapObject::SetSafeArea(int safearea)
	{
		if( safearea != mSafeArea )
		{
			if( mSafeArea >= 0 && safearea < 0 )
			{
				OnLeaveSafeArea();
			}
			if( /*mSafeArea < 0 &&*/ safearea > 0 )
			{
				OnEnterSafeArea(safearea);
			}
			mSafeArea = safearea;
		}
		return mSafeArea;
	}
	int PlayMapObject::SetEventArea(int eventarea, std::string eventid)
	{
		if (eventarea != mEventArea)
		{
			if (mEventArea < 0 && eventarea > 0)
			{
				if (eventid != "")
				{
					OnEnterEventArea(eventid);
				}
			}
			if (mEventArea >= 0 && eventarea < 0)
			{
				char string[256];
				itoa(mEventArea, string,10);
				OnLeaveEventArea(string);
			}
			mEventArea = eventarea;
		}
		return mEventArea;
	}

	int PlayMapObject::SetPosition(int x,int y)
	{
		if( mSettingPosition ) return 0;
		mSettingPosition = 1;
		int ox = mX;
		int oy = mY;
		mX = x;
		mY = y;
		if( mParentMap )
		{
			if( IsState(MAP_OBJECT_STATE_IDLE) )
			{
				mParentMap->RemMapLogicBlock(this,ox,oy);
				mParentMap->AddMapLogicBlock(this,x,y);
			}
			int zi = mParentMap->GetZoneID(x,y);
			if( zi != mMapZoneID )
			{
				mParentMap->NotifyMove(this,x,y,ox,oy);
			}

			mParentMap->NotifyTrapInOut(this, x, y, ox, oy);
		}

		mSettingPosition = 0;
		return 0;
	}
	void PlayMapObject::SetState(int state)
	{
		if( state == MAP_OBJECT_STATE_IDLE && mState == MAP_OBJECT_STATE_DEAD )
		{
			if( mParentMap )
			{
				mParentMap->AddMapLogicBlock(this,X(),Y());
			}
		}
		if( state == MAP_OBJECT_STATE_DEAD && mState == MAP_OBJECT_STATE_IDLE )
		{
			if( mParentMap )
			{
				mParentMap->RemMapLogicBlock(this,X(),Y());
			}
		}
		mState = state;
	}
	void PlayMapObject::AddEnterMapCell(int id)
	{
		if( mEnteredMapCellNum < ENETERED_MAPCELL_NUM )
		{
			mEnteredMapCell[mEnteredMapCellNum++] = id;
		}
		return;
	}
	void PlayMapObject::ClearEnteredMapCell(int id)
	{
		for(int i = 0;i < ENETERED_MAPCELL_NUM;i++)
		{
			if( mEnteredMapCell[i] == id )
			{
				mEnteredMapCell[i] = -1;
			}
		}
		return;
	}
	bool PlayMapObject::IsEnteredMapCell(int id)
	{
		for(int i = 0;i < ENETERED_MAPCELL_NUM;i++)
		{
			if( mEnteredMapCell[i] == id )
			{
				return true;
			}
		}
		return false;
	}
	void PlayMapObject::FreshEnteredMapCell()
	{
		int num = 0;
		for(int i = 0;i < ENETERED_MAPCELL_NUM;i++)
		{
			if(mEnteredMapCell[i] >= 0 )
			{
				mEnteredMapCell[num++] = mEnteredMapCell[i];
			}
		}
		mEnteredMapCellNum = num;
	}
	IntPointClass PlayMapObject::NextPosition(int dir,int step,int blockcheckflag)
	{
		int rx = mX;
		int ry = mY;
		int ox = mX;
		int oy = mY;
		for( int i = 0;i< step;i++ )
		{
			rx = NextX(ox,dir);
			ry = NextY(oy,dir);
			if( mParentMap )
			{
				int b = mParentMap->GetBlockData(rx,ry,blockcheckflag);
				if( b > 100 || (b > 0 && SafeArea()<0 ) )
				{
					rx = ox;
					ry = oy;
				}
			}
			ox = rx;
			oy = ry;
		}
		return IntPointClass(rx,ry);
	}
	int PlayMapObject::Diff2Dir(float x,float y)
	{
		float rot = atan2(y,x);
		rot = rot * 180.0f / 3.141592653589793f;
		while ( rot < 0 )
		{
			rot += 360.0f;
		}
		if ( rot >= 337.5 || rot < 22.5 )
		{
			return DIR_RIGHT;
		}
		if ( rot >= 22.5 && rot < 67.5 )
		{
			return DIR_DOWN_RIGHT;
		}
		if ( rot >= 67.5 && rot < 112.5 )
		{
			return DIR_DOWN;
		}
		if ( rot >= 112.5 && rot < 157.5 )
		{
			return DIR_DOWN_LEFT;
		}
		if ( rot >= 157.5 && rot < 202.5 )
		{
			return DIR_LEFT;
		}
		if ( rot >= 202.5 && rot < 247.5 )
		{
			return DIR_UP_LEFT;
		}
		if ( rot >= 247.5 && rot < 292.5 )
		{
			return DIR_UP;
		}
		if ( rot >= 292.5 && rot < 337.5 )
		{
			return DIR_UP_RIGHT;
		}
		return DIR_UP;
	}
	int PlayMapObject::NextDir(int dir,int shift)
	{
		static const int NEXT_DIR[8][9]={
			{4,5,6,7,0,1,2,3,4},
			{5,6,7,0,1,2,3,4,5},
			{6,7,0,1,2,3,4,5,6},
			{7,0,1,2,3,4,5,6,7},
			{0,1,2,3,4,5,6,7,0},
			{1,2,3,4,5,6,7,0,1},
			{2,3,4,5,6,7,0,1,2},
			{3,4,5,6,7,0,1,2,3},
		};
		if( dir >=0 && dir <=7 )
		{
			int sh = shift+4;
			if( sh >=0 && sh<=8 )
			{
				return NEXT_DIR[dir][sh];
			}
		}
		return 0;
	}
	void PlayMapObject::NotifyMapEnter(PlayMap* map)
	{
		mParentMap = map;
		if( mParentMap )
		{
			mGSL = mParentMap->GetManager()->GetGSLib();
		}
	}
	void PlayMapObject::NotifyMapLeave(PlayMap* map)
	{
		mParentMap = 0;
	}
}