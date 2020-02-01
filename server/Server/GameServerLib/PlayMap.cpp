#include "StdAfx.h"
#include "PlayMap.h"
#include "DomainPlay.h"
#include "PlayNpc.h"
#include "PlayMonster.h"
#include "PlayMapItem.h"
#include "PlayNeutral.h"
#include "ManagerMap.h"
#include "GameServerLib.h"
#include "ScriptLua.h"
#include "ManagerMonster.h"
#include "ManagerFly.h"
#include "GameSession.h"
#include "PlayMapTrap.h"
#include "ManagerTrapDef.h"

namespace OGSLib
{
	static const int MapZoneWidth = 8;
	static const int MapZoneHeight = 8;
	PlayMap::PlayMap(ManagerMap* manager)
	{
		mManager = manager;
		mMapData.mMapData=0;
		mMiniMapID = -1;
		mOptionRunMonster=0;
		mOptionRunPlayer=0;
		mOptionAutoEnter=0;
		mOptionCanDropItem=1;
		mOptionCanTrade =1;
		mOptionCanRandGo = 1;
		mOptionPKProhibit = 0;
		mOptionPKAllow = 0;
		mOptionAutoAlive = 0;
		mOptionCanReliveStand = 1;
		mTrigerEnterLeave = 0;
		mOptionNoProtectNewer = 0;
		mNoDieDropItem = 0;
		mEnterMapProtected = 1;
		mOptionCoupleForceMove = 0;
		mAutoSecondDamage = 0;
		mWeather=-1;
		mTimerUpdateTag = false;
		mOwnerWait = false;
		mOwnerWaitTime = 0;
		for( int i = 0;i<10; i++ )
		{
			mTimer[i]=0.0;
		}
		mConnList.reserve(5);
		mSafeAreaList.reserve(3);
		mMiniNpcList.reserve(20);
		mMapMiniMonGen.reserve(20);
		mOptionExpMul = 0;
		mAutoSecondTick = 0;
		mOptionNoDirectFly = 0;
		mOptionNoInteract = 0;
		mOptionNoMount = 0;
		mOptionLockAction = 0;
		mOptionWanderHangup = 0;
		mOptionFightAnonym = 0;
		mOptionNoGod = 0;
		mOptionHomePos = 0;
		mOptionRelivePos = 0;

		mOptionMinLv=0;
		mOptionMinLvR=0;
		mOptionTaskShare=0;
		mOptionFunbenType=0;
		mOptionNoCtgBoss = 0;
	}
	PlayMap::~PlayMap(void)
	{
	}
	void PlayMap::Update(double time1)
	{
		if( mTimerUpdateTag )
		{
			bool tag = false;
			for( int i = 0;i<10; i++ )
			{
				if( mTimer[i]>0.0 && time1 > mTimer[i])
				{
					mTimer[i] = 0.0;
					if( ! mScript.empty() )
					{
						char ss[256];_snprintf_s(ss,_countof(ss),255,"%s.onTimer%d",mScript.c_str(),i+1);ss[255]=0;
						mManager->GetGSLib()->GetScriptEngine()->OnlyMap(this,ss);
					}
				}
				if( mTimer[i] > 0.0 )
				{
					tag = true;
				}
			}
			mTimerUpdateTag = tag;
		}
		if( mAutoSecondDamage > 0 )
		{
			if( mAutoSecondTick + this->GetManager()->GetGSLib()->mMapAutoSecondGap < time1 )
			{
				mAutoSecondTick = time1;
				for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
					pos != mObjectList.end();
					pos ++ )
				{
					if( (*pos)&&(*pos)->TypeIsPlayer() )
					{
						DomainPlay* play = static_cast<DomainPlay*>(*pos);
						if( mAutoSecondDamage > 0 )
						{
							play->DamageHP(0,mAutoSecondDamage,-1);
						}
					}
				}
			} 
		}
		if (mOwnerWait)//////针对意外掉线的玩家
		{
			int now = static_cast<int>(time(0));
			if (mOwnerWaitTime + 600 < now)
			{
				mOwnerWait = false;
				ClearOwner();
			}
		}
	}
	void PlayMap::SetWaitTag(bool result)
	{
		if (result)
		{
			mOwnerWait = true;
			mOwnerWaitTime = static_cast<int>(time(0));////记录离线的时间
		}
		else
		{
			mOwnerWait = false;
			mOwnerWaitTime = 0;////玩家上线之后将之前离线的数据清理
		}
		
	}
	int  PlayMap::SetMapTimer(int i,int esp)
	{
		if( i >=1 && i<=10 )
		{
			if (esp == 0)/////0表示关闭定时器
			{
				mTimer[i - 1] = 0;
				mTimerUpdateTag = true;
				return i;
			}
			mTimer[i-1] = mManager->GetGSLib()->GetTimerFix()->Elapsed() + esp/1000.0;
			mTimerUpdateTag = true;
			return i;
		}
		return 0;
	}
	double  PlayMap::GetMapTimer(int i)
	{
		double time = 0;
		if (i >= 1 && i <= 10)
		{
			if (mTimer[i - 1] >= mManager->GetGSLib()->GetTimerFix()->Elapsed())
			{
				time = mTimer[i - 1] - mManager->GetGSLib()->GetTimerFix()->Elapsed();
			}
			else
			{
				time = 0;
			}
			return time;
		}
		return 0;
	}
	MapCell * PlayMap::GetMapCell(int x,int y)
	{
		if( &mMapData )
		{
			if( x >= 0 && x<mMapData.mWidth && y >= 0 && y < mMapData.mHeight )
			{
				return mCells[y*mMapData.mWidth + x];
			}
		}
		return 0;
	}
	PlayMapObjectNodeList* PlayMap::GetMapZone(int x,int y)
	{
		//if( x >= 0 && x<mMapData.mWidth && y >= 0 && y < mMapData.mHeight )
		//{
		//	int xz = x / MapZoneWidth;
		//	int yz = y / MapZoneHeight;
		//	return &mZones[yz*mZoneWidth+xz];
		//}
		return 0;
	}
	const MapCell * PlayMap::FindMapCell(int x,int y)
	{
		return GetMapCell(x,y);
	}
	int PlayMap::AddMapLogicBlock(PlayMapObject* obj,int x,int y)
	{
		MapCell* cell = GetMapCell(x,y);
		if( cell )
		{
			cell->mObjList.push_back(&obj->mCellNode);
			obj->SetSafeArea(cell->SafeArea);
			obj->SetEventArea(cell->EventArea, cell->EventIdArea);
			//for( PlayMapObjectNode* pos = cell->mObjList.begin();
			//	pos != cell->mObjList.end();
			//	pos = pos->Next )
			//{
			//	pos->This->IsInteractive();
			//}
			if( obj->TypeIsMonster() || obj->TypeIsSlave() )
			{
				cell->MapMon_Num++;
				return cell->MapMon_Num;
			}
			if( obj->TypeIsNPC() )
			{
				cell->MapNpc_Num++;
				return cell->MapNpc_Num;
			}
			if( obj->TypeIsPlayer() )
			{
				cell->MapPlayer_Num++;
				return cell->MapPlayer_Num;
			}
			if ( obj->TypeIsTrap() )
			{
				cell->MapTrap_Num ++;
				return cell->MapTrap_Num;
			}
		}
		return 0;
	}
	int PlayMap::RemMapLogicBlock(PlayMapObject* obj,int x,int y)
	{
		MapCell* cell = GetMapCell(x,y);
		if( cell )
		{
			obj->mCellNode.leave();
			obj->SetSafeArea(cell->SafeArea);
			obj->SetEventArea(cell->EventArea, cell->EventIdArea);
			if( obj->TypeIsMonster() || obj->TypeIsSlave() )
			{
				cell->MapMon_Num--;
				return cell->MapMon_Num;
			}
			if( obj->TypeIsNPC() )
			{
				cell->MapNpc_Num--;
				return cell->MapNpc_Num;
			}
			if( obj->TypeIsPlayer() )
			{
				cell->MapPlayer_Num--;
				return cell->MapPlayer_Num;
			}
			if (obj->TypeIsTrap())
			{
				cell->MapTrap_Num --;
				return cell->MapTrap_Num;
			}
		}
		return 0;
	}

	void PlayMap::AddMapItem(PlayMapObject* obj)
	{
		MapCell* mc = GetMapCell(obj->X(),obj->Y());
		if( mc )
		{
			if( mc->MapItem_Num <= 0 )
			{
				mc->MapItem_Num = 1;
			}
			else
			{
				mc->MapItem_Num ++;
			}
		}
	}
	void PlayMap::RemoveMapItem(PlayMapObject* obj)
	{
		MapCell* mc = GetMapCell(obj->X(),obj->Y());
		if( mc )
		{
			if( mc->MapItem_Num > 0 )
			{
				mc->MapItem_Num--;
			}
		}
	}
	void PlayMap::AutoAlive(int v)
	{
		mOptionAutoAlive = v;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				DomainPlay* play = (DomainPlay*)(*pos);
				play->NotifyMapOption(this);
			}
		}
	}
	int PlayMap::MapFlags()
	{
		int flags=0;
		if( RunMonster() )
		{
			flags |= 0x1;
		}
		if( RunPlayer() )
		{
			flags |= 0x2;
		}
		return flags;
	}
	bool PlayMap::GenValidateMapItemPosition(int cx,int cy,int &x,int &y)
	{
		for( int check = 0;check < 100;check ++ )
		{
			for(int step=1;step<10;step++)
			{
				for(int tx = cx-step;tx<=cx+step;tx++)
				{
					int ty=cy-step;
					MapCell* mc = GetMapCell(tx,ty);
					if( mc && mc->Conn== 0 && mc->MapNpc_Num ==0 && mc->MapItem_Num <= check ){x=tx;y=ty;return true;}
				}
				for(int tx = cx-step;tx<=cx+step;tx++)
				{
					int ty=cy+step;
					MapCell* mc = GetMapCell(tx,ty);
					if( mc && mc->Conn== 0 && mc->MapNpc_Num ==0 && mc->MapItem_Num <= check ){x=tx;y=ty;return true;}
				}
				for(int ty = cy-step+1;ty<=cy+step-1;ty++)
				{
					int tx=cx-step;
					MapCell* mc = GetMapCell(tx,ty);
					if( mc && mc->Conn== 0 && mc->MapNpc_Num ==0 && mc->MapItem_Num <= check ){x=tx;y=ty;return true;}
				}
				for(int ty = cy-step+1;ty<=cy+step-1;ty++)
				{
					int tx=cx+step;
					MapCell* mc = GetMapCell(tx,ty);
					if( mc && mc->Conn== 0 && mc->MapNpc_Num ==0 && mc->MapItem_Num <= check ){x=tx;y=ty;return true;}
				}
			}
		}
		x = cx;
		y = cy;
		return true;
	}
	bool PlayMap::GenValidateRandomPosition(int* x,int* y)
	{
		if( x && y && (mMapData.mWidth>0 && mMapData.mHeight>0))
		{
			*x=0;
			*y=0;
			for(int i =0;i<100;i++)
			{
				int cx = rand() % mMapData.mWidth;
				int cy = rand() % mMapData.mHeight;
				if( GenValidatePosition(cx,cy,x,y) )
				{
					return true;
				}
			}
		}
		return false;
	}
	bool PlayMap::GenValidateRandomPosition(int cx,int cy,int range,int* x,int *y, int check_pos)
	{
		int i = 0;
		do 
		{
			i++;
			int xx = rand()%range;
			int yy = rand()%(range-xx);
			if( rand()%2== 0 ) xx = -xx;
			if( rand()%2== 0 ) yy = -yy;
			*x = cx+xx;
			*y = cy+yy;
			if(i>100)
			{
				break;
			}
			if (mMapData.mHeight==0 || mMapData.mWidth==0)
			{
				return false;
			}
			if( GetBlockData(*x,*y,check_pos) > 0)
			{
				continue;
			}
			else if (0!=*x||0!=*y)
			{
				return true;
			}
		} while ( true );
		*x = cx;
		*y = cy;
		return true;
	}
	void PlayMap::AddObject( PlayMapObject* obj )
	{
		obj->NotifyMapEnter(this);
		if( obj->IsState(PlayMapObject::MAP_OBJECT_STATE_IDLE) )
		{
			AddMapLogicBlock(obj,obj->X(),obj->Y());
		}
		if (isCopy)
		{
			if( obj->TypeIsPlayer() )
			{
				mManager->GetGSLib()->GetScriptEngine()->MapPlayer(this,static_cast<DomainPlay*>(obj),"player.onEnterCopy");
			}
		}
		if( mTrigerEnterLeave )
		{
			if( obj->TypeIsPlayer() )
			{
				if( ! mScript.empty() )
				{
					char ss[256];_snprintf_s(ss,_countof(ss),255,"%s.onPlayerEnter",mScript.c_str());ss[255]=0;
					mManager->GetGSLib()->GetScriptEngine()->MapPlayer(this,static_cast<DomainPlay*>(obj),ss);
				}
			}
		}
		try
		{
			NotifyMove(obj,obj->X(),obj->Y(),-10000000,-10000000);
		}
		catch(...)
		{
			printf("exception on add object\n");
		}
		std::deque<PlayMapObject*>::iterator pos = mObjectList.end();
		for(pos = mObjectList.begin();
			pos != mObjectList.end();
			pos++ )
		{
			if((*pos) && obj == (*pos))
			{
				break;
			}
		}
		if(pos != mObjectList.end())
		{
			mObjectList.erase(pos);
		}

		mObjectList.push_back(obj);
		//mObjectList.push_back(&obj->mMapNode);
		if( obj->TypeIsMapItem() )
		{
			AddMapItem(obj);
		}
		if( obj->TypeIsNPC() )
		{
			SetBlockData(obj->X(),obj->Y());
			PlayNpc* npc = static_cast<PlayNpc*>(obj);
			AddMiniNpc(obj->X(),obj->Y(),obj->Name(),npc->ShortName(),npc->mDirectFlyID,npc->mShowNpcFlag);
		}
	}
	void PlayMap::AddMiniNpc(int x,int y,const char* name,const char* strname,int directflyID,int showNpcFlag)
	{
		for(auto iter = mMiniNpcList.begin();iter != mMiniNpcList.end();iter++)
		{
			if((*iter).x==x && (*iter).y==y)
			{
				return;
			}
		}
		MapMiniNpc mn;
		mn.x = x;
		mn.y = y;
		mn.npc_name = name;
		mn.short_name = strname;
		mn.npc_directfly_ID = directflyID;
		mn.showNpcFlag = showNpcFlag;
		mMiniNpcList.push_back(mn);
	}
	void PlayMap::AddMiniMonGen(const std::string& name,int x,int y,int time)
	{
		MapMiniMonGen mg;
		mg.name = name;
		mg.x = x;
		mg.y = y;
		mg.time = time;
		mMapMiniMonGen.push_back(mg);
	}
	bool PlayMap::FindMiniNpc(const char* name)
	{
		for( OGSLib::PlayMap::MapMiniNpcVector::const_iterator p = mMiniNpcList.begin();
			p != mMiniNpcList.end();
			++ p )
		{
			if( 0==strcmp(p->npc_name.c_str(),name) )
			{
				return true;
			}
		}
		return false;
	}
	void PlayMap::ResetOwner(PlayMapObject* obj)
	{
		if (obj->TypeIsMapItem())
		{
			PlayMapItem* item = (PlayMapItem*)(obj);
			item->OwnerID(0);
			for (std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
				pos != mObjectList.end();
				pos++)
			{
				if ((*pos) && (*pos)->TypeIsPlayer())
				{
					DomainPlay* play = (DomainPlay*)(*pos);
					if (play)
					{
						play->NotifyMeetItem(play,item);
					}
				}
			}
		}
	}
	void PlayMap::RemoveObject( PlayMapObject* obj)
	{
		if( obj->TypeIsMapItem() )
		{
			RemoveMapItem(obj);
		}
		//obj->mMapNode.leave();
		std::deque<PlayMapObject*>::iterator pos = mObjectList.end();
		for(pos = mObjectList.begin();
			pos != mObjectList.end();
			pos++ )
		{
			if((*pos) && obj == (*pos))
			{
				break;
			}
		}
		if(pos != mObjectList.end())
		{
			mObjectList.erase(pos);
		}

		try
		{
			NotifyMove(obj,-10000000,-10000000,obj->X(),obj->Y());
		}
		catch(...)
		{
			printf("exception on remove object\n");
		}
		if( obj->mMapZone )
		{
			printf("error leave have mapzone (id=%s , name=%s , file=%s)\n",mID.c_str(),mName.c_str(),mMapFile.c_str());
		}
		if( mTrigerEnterLeave )
		{
			if( obj->TypeIsPlayer() )
			{
				if( ! mScript.empty() )
				{
					char ss[256];_snprintf_s(ss,_countof(ss),255,"%s.onPlayerLeave",mScript.c_str());ss[255]=0;
					mManager->GetGSLib()->GetScriptEngine()->MapPlayer(this,static_cast<DomainPlay*>(obj),ss);
				}
			}
		}
		if( obj->IsState(PlayMapObject::MAP_OBJECT_STATE_IDLE) )
		{
			RemMapLogicBlock(obj,obj->X(),obj->Y());
		}
		obj->NotifyMapLeave(this);
	}
	bool PlayMap::CanSeeEachOther(PlayMapObject* obj1,PlayMapObject* obj2)
	{
		if( obj1->TypeIsPlayer() || obj2->TypeIsPlayer() )
		{
			return true;
		}
		if( obj1->TypeIsSlave() || obj2->TypeIsSlave() )
		{
			return true;
		}
		if( obj1->TypeIsDart() || obj2->TypeIsDart() )
		{
			return true;
		}
		//if( obj1->TypeIsCollection() || obj2->TypeIsCollection() )
		//{
		//	return true;
		//}
		return false;
	}
	int PlayMap::GetZoneID(int newx,int newy)
	{
		return (newy / MapZoneHeight)*mZoneWidth+(newx / MapZoneWidth);
	}
	void PlayMap::NotifyMove( PlayMapObject* obj,int newx,int newy,int oldx,int oldy)
	{
		int oids[9];
		int nids[9];
		int enter_ids[9];
		int enter_number=0;
		int leave_ids[9];
		int leave_number=0;

		int nxid = newx / MapZoneWidth;
		int nyid = newy / MapZoneHeight;
		int nid = nyid * mZoneWidth + nxid;
		int oxid = oldx / MapZoneWidth;
		int oyid = oldy / MapZoneHeight;
		int oid = oyid * mZoneWidth + oxid;
		if( nid == oid )
		{
			return ;
		}
		for(int i = -1;i<=1;i++)
		{
			for(int j = -1;j<=1;j++)
			{
				nids[(i+1)*3+(j+1)] = (nyid+i)*mZoneWidth + (nxid+j);
			}
		}

		for(int i = -1;i<=1;i++)
		{
			for(int j = -1;j<=1;j++)
			{
				oids[(i+1)*3+(j+1)] = (oyid+i)*mZoneWidth + (oxid+j);
			}
		}

		for(int i = 0;i < 9 ;i ++ )
		{
			int id = nids[i];
			bool find = false;
			for(int j = 0;j < 9 ;j ++ )
			{
				if( id == oids[j] )
				{
					find = true;
				}
			}
			if( ! find )
			{
				enter_ids[enter_number++] = id;
			}
		}
		for(int i = 0;i < 9 ;i ++ )
		{
			int id = oids[i];
			bool find = false;
			for(int j = 0;j < 9 ;j ++ )
			{
				if( id == nids[j] )
				{
					find = true;
				}
			}
			if( ! find )
			{
				leave_ids[leave_number++] = id;
			}
		}
		int id_count = mZoneWidth*mZoneHeight;
		
		if( oid >= 0 && oid <id_count )
		{
			//std::deque<PlayMapObject*> ozone = mZones[oid];
			//obj->mZoneNode.leave();
			if(obj->mMapZoneID>=0)
			{
				std::deque<PlayMapObject*>& nzone=mZones[obj->mMapZoneID];
				nzone.erase(std::remove(nzone.begin(),nzone.end(),obj),nzone.end());
				obj->mMapZoneID=-1000;
			}
			//if( ozone != obj->mMapZone )
			//{
			//	printf("zone ptr error (id=%s , name=%s , file=%s)\n",mID.c_str(),mName.c_str(),mMapFile.c_str());
			//}
			obj->mMapZone = 0;
		}
		int id = 0;
		//PlayMapObjectNodeList* z=0;
		for( int i = 0;i < leave_number;i ++)
		{
			id = leave_ids[i];
			if( id >= 0 && id<id_count )
			{
				std::deque<PlayMapObject*>& z = mZones[id];
				for(std::deque<PlayMapObject*>::iterator pos = z.begin();
					pos != z.end();)
				{
					if ((*pos))
					{
						if ((*pos) == obj)
						{
							pos++;
							continue;
						}
						if (CanSeeEachOther(obj, *pos))
						{
							(*pos)->NotifyBye(this, obj);
							obj->NotifyBye(this, *pos);
						}
						pos++;
					}
					else
					{
						z.erase(pos++);
						printf("mapmove bye *pos=0\n");
					}
				}
			}
		}
		for( int i = 0;i < enter_number;i ++)
		{
			id = enter_ids[i];
			if( id >= 0 && id<id_count )
			{
				std::deque<PlayMapObject*>& z = mZones[id];
				for(std::deque<PlayMapObject*>::iterator  pos = z.begin();
					pos != z.end();)
				{
					if ((*pos))
					{
						if ((*pos) == obj)
						{
							pos++;
							continue;
						}
						if (CanSeeEachOther(obj, *pos))
						{
							obj->NotifyMeet(this, *pos);
							(*pos)->NotifyMeet(this, obj);
						}
						pos++;
					}
					else
					{
						z.erase(pos++);
						printf("mapmove meet *pos=0\n");
					}
				}
			}
		}
		if( nid >= 0 && nid <id_count )
		{
			std::deque<PlayMapObject*>& z = mZones[nid];
			//obj->mMapZone = z;
			obj->mMapZoneID = nid;
			z.push_back(obj);
		}
	}

	int PlayMap::CheckMapSafeArea(int newx,int newy)
	{
		MapCell* cell = GetMapCell(newx,newy);
		if( cell )
		{
			return cell->SafeArea;
		}
		return -1;
	}
	int PlayMap::CheckMapHoldArea(int newx,int newy)
	{
		MapCell* cell = GetMapCell(newx,newy);
		if( cell )
		{
			return cell->HoldArea;
		}
		return -1;
	}
	bool PlayMap::CheckMapConn(PlayMapObject* obj,int newx,int newy)
	{
		MapCell* cell = GetMapCell(newx,newy);

		if( cell && cell->Conn )
		{
			// 判断传送点是否有效
			if (cell->MapTrap_Num > 0)
			{
				bool found = false;
				for( PlayMapObjectNode* pos = cell->mObjList.begin();
					pos != cell->mObjList.end();
					pos = pos->Next)
				{
					if( pos->This->TypeIsTrap())
					{
						PlayMapTrap * trap = static_cast<PlayMapTrap *>(pos->This);
						if (trap->DefId() == eTrapIdMapConn)
						{
							found = true;
							break;
						}
					}
				}

				// 没有传送点类型的陷阱
				if (found == false)
				{
					return false;
				}
			}
			else
			{
				return false;
			}

			for(MapConnVector::iterator pos = mConnList.begin();
				pos != mConnList.end();
				++ pos )
			{
				if( (*pos) && obj->X() == (*pos)->mStartX && obj->Y() == (*pos)->mStartY)
				{
					DomainPlay* player =0;
					player = (DomainPlay*)obj;
					if ((*pos)->mType == 4)
					{
						mManager->GetGSLib()->GetScriptEngine()->PlayerInt(player, (*pos)->mNodeID, 0, 0, 0, "onPlayerEnterConn");
						return true;
					}

					if( 1==player->GoMapConn((*pos)->mTargetMapID,(*pos)->mTargetX,(*pos)->mTargetY,(*pos)->mTargetSize ) )
					{
						return true;
					}
				}
			}

			for (DyMapConnMap::iterator it = mDyMapConnMap.begin();
				it != mDyMapConnMap.end(); ++ it)
			{
				if(obj->X() == it->second.x && obj->Y() == it->second.y)
				{
					DomainPlay* player =0;
					player = (DomainPlay*)obj;
					if( 1==player->GoMapConn(it->second.target_map_id,it->second.target_x,it->second.target_y,it->second.target_range) )
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	int PlayMap::Alert(int lv,int flags,const char* msg)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				DomainPlay* p = (DomainPlay*)(*pos);
				p->AlertMessage(lv,flags,msg);
			}
		}
		return 0;
	}
	int PlayMap::PlayEffect(int skill_type,int res_id)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				DomainPlay* p = (DomainPlay*)(*pos);
				p->NotifyPlayEffect(p,skill_type,p->X(),p->Y(),p->ID(),p->ID(),res_id);
			}
		}
		return 0;
	}
	int PlayMap::NumberMonster(const char* monster_name)
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsMonster() && (*pos)->IsState(PlayMapObject::MAP_OBJECT_STATE_IDLE) )
			{
				if( monster_name )
				{
					if( 0 == strcmp((*pos)->Name(),monster_name) )
					{
						num ++;
					}
				}
				else
				{
					num++;
				}
			}
		}
		return num;
	}
	int PlayMap::NumberMonster(int mon_id)
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsMonster() && (*pos)->IsState(PlayMapObject::MAP_OBJECT_STATE_IDLE) )
			{
				if (mon_id)
				{
					if (mon_id == (*pos)->DefId())
					{
						num++;
					}
				}
				else
				{
					num++;
				}
			}
		}
		return num;
	}
	bool PlayMap::IsNpc(const char* npc_name)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsNPC() )
			{
				if( npc_name )
				{
					if( 0 == strcmp((*pos)->Name(),npc_name) )
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	int PlayMap::NumberNeutral(const char* neutral_name)
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsNeutral() && (*pos)->IsState(PlayMapObject::MAP_OBJECT_STATE_IDLE) )
			{
				if( neutral_name )
				{
					if( 0 == strcmp((*pos)->Name(),neutral_name) )
					{
						num ++;
					}
				}
				else
				{
					num++;
				}
			}
		}
		return num;
	}
	void PlayMap::ClearItem()
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();)
		{
			if ((*pos))
			{
				if ((*pos) && (*pos)->TypeIsMapItem())
				{
					PlayMapItem* mon = (PlayMapItem*)(*pos);
					mManager->GetGSLib()->GetSecondTimer()->AddTimer(0, mon, mon->item_type());
				}
				pos++;
			}
			else
			{
				mObjectList.erase(pos++);
				printf("Clear_Item_Object Null\n");
			}
		}
	}
	void PlayMap::ClearMiniNpc()
	{
		mMiniNpcList.clear();
	}
	void PlayMap::ClearNeutral()
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsNeutral() )
			{
				PlayNeutral* mon = (PlayNeutral*)(*pos);
				if( ! mon->IsDead() )
				{
					mon->ClearDie();
				}
			}
		}
	}
	void PlayMap::ClearMon()
	{
		for (std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();)
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsMonster())
				{
					PlayMonster* mon = (PlayMonster*)(*pos);
					if (!mon->IsDead())
					{
						mon->ClearDie();
					}
				}
				pos++;
			}
			else
			{
				mObjectList.erase(pos++);
				printf("Clear_AllMon_Object Null\n");
			}
		}
	}
	void PlayMap::ClearMonById(int id)
	{
		for (std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();)
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsMonster())
				{
					PlayMonster* mon = (PlayMonster*)(*pos);
					if (mon->DefId()==id)
					{
						if (!mon->IsDead())
						{
							mon->ClearDie();
						}
					}
				}
				pos++;
			}
			else
			{
				mObjectList.erase(pos++);
				printf("Clear_OneMon_Object Null\n");
			}
		}
	}
	void PlayMap::ClearMonByName(const char* monster_name)
	{
		for (std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();)
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsMonster())
				{
					PlayMonster* mon = (PlayMonster*)(*pos);
					if (strcmp(mon->Name(), monster_name) == 0)
					{
						if (!mon->IsDead())
						{
							mon->ClearDie();
						}
					}
				}
				pos++;
			}
			else
			{
				mObjectList.erase(pos++);
				printf("Clear_OneMon_Object Null\n");
			}
		}
	}

	void PlayMap::ClearNeutralBySeedID(int seed_id)
	{
		for (std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();)
		{
			if ((*pos))
			{
				if ((*pos)->TypeIsNeutral())
				{
					if ((*pos)->ID() == seed_id)
					{
						PlayNeutral* mon = (PlayNeutral*)(*pos);
						if( ! mon->IsDead() )
						{
							mon->ClearDie();
						}
					}
				}
				pos++;
			}
			/*else
			{
				mObjectList.erase(pos++);
				printf("Clear_OneMon_Object Null\n");
			}*/
		}
	}
	PlayNeutral* PlayMap::FindNeutralBySeedID(int seed_id)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsNeutral() &&(*pos)->ID() == seed_id )
			{
				return static_cast<PlayNeutral*>(*pos);
			}
		}
		return 0;
	}

	PlayNpc* PlayMap::FindNPC(std::string& npc_name)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsNPC() && (*pos)->Name() == npc_name )
			{
				return static_cast<PlayNpc*>(*pos);
			}
		}
		return 0;
	}
	PlayNpc* PlayMap::FindNPC(int id)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsNPC() && (*pos)->ID() == id )
			{
				return static_cast<PlayNpc*>((*pos));
			}
		}
		return 0;
	}
	DomainPlay* PlayMap::RandPlayer(int id)
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				num ++;
				if( num == id )
				{
					DomainPlay* player = static_cast<DomainPlay*>(*pos);
					return player;
				}
			}
		}
		return 0;
	}

	PlayActiveObject * PlayMap::FindPlayerOrSlave(int id)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&& ((*pos)->TypeIsPlayer() || (*pos)->TypeIsSlave()))
			{
				if( (*pos)->ID() == id )
				{
					PlayActiveObject* player = static_cast<PlayActiveObject*>(*pos);
					return player;
				}
			}
		}
		return 0;
	}
	PlayMonster * PlayMap::FindMonster(int id)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&& ((*pos)->TypeIsMonster()))
			{
				if( (*pos)->ID() == id )
				{
					PlayMonster* monster = static_cast<PlayMonster*>(*pos);
					return monster;
				}
			}
		}
		return 0;
	}

	PlayActiveObject* PlayMap::FindMonsterOrNeutral(int id)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&& ((*pos)->TypeIsMonster() || (*pos)->TypeIsNeutral()))
			{
				if( (*pos)->ID() == id )
				{
					PlayActiveObject* activeObj = static_cast<PlayActiveObject*>(*pos);
					return activeObj;
				}
			}
		}
		return 0;
	}

	int PlayMap::NumberPlayer()
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				num ++;
			}
		}
		return num;
	}
	int PlayMap::NumberItem()
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsMapItem() )
			{
				num ++;
			}
		}
		return num;
	}
	int PlayMap::NumberTeamMember(int team_id)
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				DomainPlay* play = (DomainPlay*)(*pos);
				if( play && play->TeamID() == team_id )
				{
					num ++;
				}
			}
		}
		return num;
	}
	void PlayMap::ExeTeamMember(int team_id,const char* exe)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				DomainPlay* play = (DomainPlay*)(*pos);
				if( play && play->TeamID() == team_id )
				{
					play->ExeCode(exe);
				}
			}
		}
	}
	void PlayMap::MapExe(const char* code)
	{
		for (std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();)
		{
			if ((*pos))
			{
				PlayMapObject* object = (*(pos++));
				if (object && object->TypeIsPlayer())
				{
					DomainPlay* play = (DomainPlay*)object;
					if (play)
					{
						play->ExeCode(code);
					}
				}
			}
			else
			{
				mObjectList.erase(pos++);
				printf("MapExe_Object Null\n");
			}
		}
			
	}
	void PlayMap::ExeNotifyMonDie()
	{
		//for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
		//	pos != mObjectList.end();)
		//{
		//	PlayMapObject* object=(*(pos ++));
		//	if( object && object->TypeIsPlayer() )
		//	{
		//		DomainPlay* play = (DomainPlay*)object;
		//		if( play )
		//		{
		//			play->NotifyMiniNpc(play,this);
		//		}
		//	}
		//}
	}
	PlayMapItem* PlayMap::FindMapItemNear(int x,int y,int id)
	{
		MapCell* cell = GetMapCell(x, y);
		if (cell)
		{
			for (PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();)
			{
				if (pos->This)
				{
					if (pos->This->TypeIsMapItem() && pos->This->ID() == id)
					{
						return static_cast<PlayMapItem*>(pos->This);
					}
					pos = pos->Next;
				}
				else
				{
					PlayMapObjectNode* invalid_pos = pos;
					pos = pos->Next;
					invalid_pos->leave();
				}
			}
		}
		return 0;
	}
	void PlayMap::Chat(PlayMapObject* src,char* msg,bool self)
	{
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos)&&(*pos)->TypeIsPlayer() )
			{
				if( (! self) && src == (*pos) )continue;
				DomainPlay* play = (DomainPlay*)(*pos);
				play->NotifyMapChat(this,src,msg);
			}
		}
	}
	void PlayMap::FindHoldKuafuGuild(std::string&a,std::string& g)
	{
		std::string area,guild;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos) && (*pos)->TypeIsPlayer() && !(*pos)->IsState(PlayMapObject::MAP_OBJECT_STATE_DEAD) )
			{
				DomainPlay* play = (DomainPlay*)(*pos);
				if(CheckMapHoldArea(play->X(),play->Y()) >= 0)
				{
					if( guild.empty() )
					{
						area = play->Session()->GetListener()->GetKuafuArea();
						guild = play->GuildName();
					}
					if( area != play->Session()->GetListener()->GetKuafuArea() || guild != play->GuildName() )
					{
						return;
					}
				}
			}
		}
		a = area;
		g = guild;
	}
	void PlayMap::FindHoldGuild(std::string& g)
	{
		std::string guild;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if( (*pos) && (*pos)->TypeIsPlayer() && !(*pos)->IsState(PlayMapObject::MAP_OBJECT_STATE_DEAD) )
			{
				DomainPlay* play = (DomainPlay*)(*pos);
				if( guild.empty() )
				{
					guild = play->GuildName();
				}
				if( guild != play->GuildName() )
				{
					return;
				}
			}
		}
		g = guild;
	}
	int PlayMap::SetBlockData(int x,int y)
	{

		return 0;
	}
	int PlayMap::GetBlockData(int x,int y,int check_obj)
	{
		//3 不检测怪,别的都检测
		//2 不检测
		//0 根据地图配置检测
		//1 强制检测
		if( mMapData.mMapData )
		{
			if( x >= 0 && x < mMapData.mWidth && y >= 0 && y < mMapData.mHeight )
			{
				MapCell* mc = GetMapCell(x,y);
				if( mMapData.mMapData[(x+y*mMapData.mWidth)] & 0x01 || (mc && mc->MapNpc_Num > 0))
				{
					return 999;
				}
				else if ( check_obj == 2 )
				{
					return 0;
				}
				else if ( check_obj == 3)
				{
					int num = 0;
					if (mc)
					{
						if( mc->MapNpc_Num > 0 )
							num = mc->MapNpc_Num * 10;
						if( num > 0 ) 
							return num;

						if(CheckMapSafeArea(x,y) < 0 )
						{
							if( num > 0 ) 
								return num;
							if( mOptionRunPlayer )
							{
								num += mc->MapPlayer_Num;
							}
						}
						return num;
					}
					else
						return 999;
				}
				else if( check_obj==0 || check_obj==1 )
				{
					int num = 0;
					if (mc)
					{
						if( mc->MapNpc_Num > 0 )
						{
							num = mc->MapNpc_Num * 10;
						}
						if( num > 0 ) return num;
						if( num <= 0 && CheckMapSafeArea(x,y) < 0 )
						{
							if( (! mOptionRunMonster) || check_obj==1 )
							{
								num += mc->MapMon_Num;
							}
							if( num > 0 ) return num;
							if( (! mOptionRunPlayer) || check_obj==1 )
							{
								num += mc->MapPlayer_Num;
							}
						}
					}
					return num;
				}
			}
		}
		return 1000;
	}
	bool PlayMap::PreLoad(const std::string& id,const char* file,const char* name,const char* script,const char* options)
	{
		mID = id;
		mMapFile=file;
		mName = name;
		if( strcmp(script,"null") != 0 )
		{
			mScript = script;
		}
		mOptions = options;
		return true;
	}
	bool PlayMap::Load()
	{
		//ParseOptions(mOptions);
		mMiniMapID = mManager->GetMiniMapID(mMapFile);
		mMapData = mManager->GetMapData(mMapFile);
		mZoneWidth = mMapData.mWidth/MapZoneWidth+1;
		mZoneHeight = mMapData.mHeight/MapZoneHeight+1;
		mZones = new std::deque<PlayMapObject*>[mZoneWidth*mZoneHeight];
		mCells = new MapCell*[mMapData.mWidth*mMapData.mHeight];
		memset(mCells,0,mMapData.mWidth*mMapData.mHeight*sizeof(MapCell*));
		for(int x = 0;x<mMapData.mWidth;x++)
		{
			for(int y = 0;y<mMapData.mHeight;y++)
			{
				if( GetBlockData(x,y,2) == 0 )
				{
					mCells[y*mMapData.mWidth + x] = new MapCell;
					mCells[y*mMapData.mWidth + x]->Clear();
				}
			}
		}
		if( ! mScript.empty() )
		{
			char ss[256];_snprintf_s(ss,_countof(ss),255,"%s.onLoad",mScript.c_str());ss[255]=0;
			mManager->GetGSLib()->GetScriptEngine()->OnlyMap(this,ss);
		}
		return true;
	}
	void PlayMap::Unload()
	{
		while( ! mObjectList.empty() )
		{
			PlayMapObject* mo = (*(mObjectList.begin()++));
			if( mo )
			{
				RemoveObject(mo);
				if( mo->TypeIsMonster() || mo->TypeIsNeutral() )
				{
					GetManager()->GetGSLib()->GetMonsterManager()->RemMonster((PlayAIObject*)mo);
				}
				delete mo;
			}
		}
		for(int i =0;i<mMapData.mHeight;i++)
		{
			for(int j=0;j<mMapData.mWidth;j++)
			{
				if( mCells[i*mMapData.mWidth+j] )
				{
					delete mCells[i*mMapData.mWidth+j];
					mCells[i*mMapData.mWidth+j] =0;
				}
			}
		}
		delete[] mCells;
		delete[] mZones;
	}
	void PlayMap::ParseOptions(std::string options)
	{

	}
	void PlayMap::SetOption(const char* option)
	{
		char word[256];
		strncpy_s(word,_countof(word),option,255);
		word[255]=0;
		_strlwr_s(word,_countof(word));
		size_t len = strlen(word);
		if( 0 == strcmp(word,"runmon") )
		{
			mOptionRunMonster=1;
		}
		if( 0 == strcmp(word,"runplayer") )
		{
			mOptionRunPlayer=1;
		}
		if( 0 == strcmp(word,"noautoenter") )
		{
			mOptionAutoEnter=0;
		}
		if( 0 == strcmp(word,"nodropitem") )
		{
			mOptionCanDropItem = 0;
		}
		if( 0 == strcmp(word,"nodiedropitem") )
		{
			mNoDieDropItem = 1;
		}
		//if( 0 == strcmp(word,"norandgo") )
		//{
		//	mOptionCanRandGo = 0;
		//}
		if( 0 == strcmp(word,"pkprohibit") )
		{
			mOptionPKProhibit = 1;
		}
		if( 0 == strcmp(word,"pkallow") )
		{
			mOptionPKAllow = 1;
		}
		//int id = 0;
		//if( 1 == sscanf_s(word,"weather(%d)",&id) )
		//{
		//	mWeather = id;
		//}
		//if( 1 == sscanf_s(word,"di(%d)",&id) )
		//{
		//	mOptionDisableItems[id] = id;
		//}
		//if( 1 == sscanf_s(word,"ds(%d)",&id) )
		//{
		//	mOptionDisableSkills[id] = id;
		//}
		//if( 1 == sscanf_s(word,"em(%d)",&id) )
		//{
		//	mOptionExpMul = id;
		//}
		//if( 1 == sscanf_s(word,"delhp(%d)",&id) )
		//{
		//	mAutoSecondDamage = id;
		//}
		if( 0 == strcmp(word,"autoalive") )
		{
			mOptionAutoAlive = 1;
		}
		if( 0 == strcmp(word,"autoalivescript") )
		{
			mOptionAutoAlive = 2;
		}
		if( 0 == strcmp(word,"norelivestand") )
		{
			mOptionCanReliveStand=0;
		}
		if( 0 == strcmp(word,"trigerenterleave") )
		{
			mTrigerEnterLeave = 1;
		}
		if( 0 == strcmp(word,"noprotectnewer") )
		{
			mOptionNoProtectNewer = 1;
		}
		if( 0 == strcmp(word,"noprotectentermap") )
		{
			mEnterMapProtected = 0;
		}
		if (0 == strcmp(word,"coupleforcemove"))
		{
			mOptionCoupleForceMove = 1;
		}
		if( 0 == strcmp(word,"nodirectfly") )
		{
			mOptionNoDirectFly = 1;
		}
		if( 0 == strcmp(word,"nointeract"))
		{
			mOptionNoInteract = 1;
		}
		if( 0 == strcmp(word,"nomount") )
		{
			mOptionNoMount = 1;
		}
		if( 0 == strcmp(word,"lockaction") )
		{
			mOptionLockAction = 1;
		}
		if( 0 == strcmp(word,"wanderhangup") )
		{
			mOptionWanderHangup = 1;
		}
		if( 0 == strcmp(word,"anonym") )
		{
			mOptionFightAnonym = 1;
		}
		if (0 == strcmp(word,"nogod"))
		{
			mOptionNoGod = 1;
		}
	}
	bool PlayMap::IsItemDisabled(int item_id)
	{
		if( mOptionDisableItems.find(item_id)!=mOptionDisableItems.end() )
		{
			return true;
		}
		return false;
	}
	bool PlayMap::IsSkillDisabled(int skill_id)
	{
		if( mOptionDisableSkills.find(skill_id)!=mOptionDisableSkills.end() )
		{
			return true;
		}
		return false;
	}
	const char* PlayMap::GetVar(const std::string& var)
	{
		MapVars::iterator pos = mVars.find(var);
		if( pos != mVars.end() )
		{
			return pos->second.c_str();
		}
		return 0;
	}
	void PlayMap::SetVar(const std::string& var,const std::string& value)
	{
		mVars[var] = value;
	}
	bool PlayMap::GenValidatePosition(int ccx,int ccy,int* x,int* y)
	{
		*x=0;
		*y=0;
		int y_step = 1;
		if( mMapData.mHeight > 30 )
		{
			y_step = 4;
		}
		if( mMapData.mHeight > 60 )
		{
			y_step = 8;
		}
		if( mMapData.mHeight > 90 )
		{
			y_step = 16;
		}
		int x_step = 1;
		if( mMapData.mWidth > 30 )
		{
			x_step = 4;
		}
		if( mMapData.mWidth > 60 )
		{
			x_step = 8;
		}
		if( mMapData.mWidth > 90 )
		{
			x_step = 16;
		}
		if( x && y && (mMapData.mWidth>0 && mMapData.mHeight>0))
		{
			int cx = ccx;
			int cy = ccy;
			for(int step=1;step<mMapData.mHeight/2;step+=y_step)
			{
				for(int tx = cx-step;tx<=cx+step;tx+=x_step)
				{
					int ty=cy-step;
					if( 0 == GetBlockData(tx,ty,2) && rand()%2 ){
						*x=tx;*y=ty;return true;
					}
				}
				for(int tx = cx-step;tx<=cx+step;tx+=x_step)
				{
					int ty=cy+step;
					if( 0 == GetBlockData(tx,ty,2) && rand()%2 ){
						*x=tx;*y=ty;return true;
					}
				}
			}
		}
		return false;
	}
	void PlayMap::AddSafeArea(MapSafeArea& sa)
	{
		mSafeAreaList.push_back(sa);
		for( int x = -sa.size+1;x < sa.size;x ++ )
		{
			int sy = sa.size-abs(x);
			for( int y = -sy+1;y < sy;y ++ )
			{
				MapCell* cell = GetMapCell(sa.x+x,sa.y+y);
				if( cell )
				{
					cell->SafeArea = sa.id;
				}
			}
		}
	}
	void PlayMap::AddHoldArea(MapHoldArea* ha)
	{
		int top,under = 0;
		mHoldArea = ha;
		if(mHoldArea)
		{
			for (int x = mHoldArea->left_x + 1;x < mHoldArea->right_x;x++)
			{
				if(x <= mHoldArea->top_x)
				{
					top = (mHoldArea->top_y-mHoldArea->left_y)*(x-mHoldArea->left_x)/(mHoldArea->top_x-mHoldArea->left_x)+mHoldArea->left_y;
				}
				else
				{
					top = (mHoldArea->top_y-mHoldArea->right_y)*(x-mHoldArea->right_x)/(mHoldArea->top_x-mHoldArea->right_x)+mHoldArea->right_y;
				}
				if(x <= mHoldArea->under_x)
				{
					under = (mHoldArea->under_y-mHoldArea->left_y)*(x-mHoldArea->left_x)/(mHoldArea->under_x-mHoldArea->left_x)+mHoldArea->left_y;
				}
				else
				{
					under = (mHoldArea->under_y-mHoldArea->right_y)*(x-mHoldArea->right_x)/(mHoldArea->under_x-mHoldArea->right_x)+mHoldArea->right_y;
				}
				for(int y = top;y <= under;y++)
				{
					MapCell* cell = GetMapCell(x,y);
					if( cell )
					{
						cell->HoldArea = ha->id;
					}
				}
			}
		}
	}
	void PlayMap::AddEventArea(MapEventArea& sa)
	{
		mEventAreaList.push_back(sa);
		for (int x = -sa.size + 1; x < sa.size; x++)
		{
			int sy = sa.size - abs(x);
			for (int y = -sy + 1; y < sy; y++)
			{
				MapCell* cell = GetMapCell(sa.x + x, sa.y + y);
				if (cell)
				{
					cell->EventArea = sa.id;
					cell->EventIdArea = sa.event_id;
				}
			}
		}
	}
	void PlayMap::AddMapConn(DirectFlyNode* mc)
	{
		mConnList.push_back(mc);

		MapCell* cell = GetMapCell(mc->mStartX,mc->mStartY);
		if( cell )
		{
			cell->Conn = 1;
		}
		else
			return;

		TrapDef def;
		def.id = eTrapIdMapConn;
		// 外观id

		def.cloth = mc->mClothId;
		def.damage = 0;
		def.skill_type = 0;
		def.effect_id = 0;
		def.damage_gap = 0;
		def.caller_leave_remove = false;
		def.life_time = 0;
		def.name = mc->mNodeName;

		mManager->GenTrap(this, mc->mStartX, mc->mStartY, &def, NULL);
	}

	int PlayMap::AddDyMapConn(int nodeid,int cloth, int x, int y, std::string target_map_id, int target_x, int target_y, int range)
	{
		MapCell* cell = GetMapCell(x, y);

		if (!cell || cell->Conn == 1)
		{
			return -1;
		}

		cell->Conn = 1;
		DyMayConn conn;
		conn.cloth = cloth;
		conn.x = x;
		conn.y = y;
		conn.target_map_id = target_map_id;
		conn.target_x = target_x;
		conn.target_y = target_y;
		conn.target_range = range;

		TrapDef def;
		def.id = eTrapIdMapConn;
		// 外观id
		def.cloth = cloth;
		def.damage = 0;
		def.skill_type = 0;
		def.effect_id = 0;
		def.damage_gap = 0;
		def.caller_leave_remove = false;
		def.life_time = 0;

		conn.trap = new PlayMapTrap(x, y, &def, NULL);
		this->AddObject(conn.trap);
		mDyMapConnMap[nodeid] = conn;
		return nodeid;
	}

	int PlayMap::RemoveDyMapConn(int id)
	{
		DyMapConnMap::iterator it = mDyMapConnMap.find(id);
		if (it == mDyMapConnMap.end())
		{
			return -1;
		}

		MapCell* cell = GetMapCell(it->second.x, it->second.y);
		if(!cell)return -1;

		cell->Conn = 0;
		this->RemoveObject(it->second.trap);
		SAFE_DELETE(it->second.trap);
		mDyMapConnMap.erase(it);

		return 0;
	}

	int PlayMap::PublicTarget(PlayActiveObject* obj, bool lock)
	{
		int num=0;
		for( std::deque<PlayMapObject*>::iterator pos = mObjectList.begin();
			pos != mObjectList.end();
			pos ++ )
		{
			if((*pos)&& (*pos)->TypeIsMonster() && (*pos)->IsState(PlayMapObject::MAP_OBJECT_STATE_IDLE) )
			{
				PlayMonster* mon = static_cast<PlayMonster*>(*pos);
				if( mon )
				{
					mon->Target(obj);
					mon->SetLockTarget(lock);
				}
			}
		}
		return num;
	}

	// 说明：游戏中 宝宝 怪物不能互穿 不能穿人，这个跟mapinfo中的允许穿人穿怪配置无关，配置只对玩家生效
	int PlayMap::GetBlockDataMon(int x,int y,int check_obj)
	{
		//2 不检测
		//0 根据地图配置检测
		//1 强制检测
		if( mMapData.mMapData )
		{
			if( x >= 0 && x < mMapData.mWidth && y >= 0 && y < mMapData.mHeight )
			{
				MapCell* mc = GetMapCell(x,y);
				if( mMapData.mMapData[(x+y*mMapData.mWidth)] & 0x01 || (mc && mc->MapNpc_Num > 0))
				{
					return 999;
				}

				if (check_obj == 2)
					return 0;
				
				int num = 0;
				if (!mc)
				{
					return 999;
				}
				if (check_obj != 3)
				{
					num += mc->MapMon_Num;
				}
				num += mc->MapPlayer_Num;
				return num;
			}
		}
		return 1000;
	}

	void PlayMap::NotifyTrapInOut(PlayMapObject* obj,int newx,int newy,int oldx,int oldy)
	{
		if (!obj->IsInteractive())
			return;

		OGSLib::PlayActiveObject* ac = (OGSLib::PlayActiveObject*)obj;
		// 通知陷阱离开
		MapCell* cell = GetMapCell(oldx, oldy);
		if (cell && cell->MapTrap_Num > 0)
		{
			for (PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();)
			{
				if (pos->This)
				{
					if (pos->This->TypeIsTrap())
					{
						static_cast<PlayMapTrap*>(pos->This)->NotifyWalkOut(ac);
					}
					pos = pos->Next;
				}
				else
				{
					PlayMapObjectNode* invalid_pos = pos;
					pos = pos->Next;
					invalid_pos->leave();
				}
			}
		}

		// 通知陷阱进入
		cell =  GetMapCell(newx, newy);
		if (cell && cell->MapTrap_Num > 0)
		{
			std::vector<int> playMapTraps;
			for (PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();)
			{
				if (pos->This)
				{
					if (pos->This->TypeIsTrap())
					{
						//static_cast<PlayMapTrap*>(pos->This)->NotifyWalkIn(ac);
						playMapTraps.push_back(static_cast<PlayMapTrap*>(pos->This)->GetTrapTypeId());
					}
					pos = pos->Next;
				}
				else
				{
					PlayMapObjectNode* invalid_pos = pos;
					pos = pos->Next;
					invalid_pos->leave();
				}
			}
			for (auto a : playMapTraps)
			{
				DomainPlay* player = 0;
				if (ac && ac->TypeIsPlayer())
				{
					player = (DomainPlay*)ac;
					mManager->GetGSLib()->GetScriptEngine()->PlayerInt(player, a, 0, 0, 0, "player.onEnterTrap");
				}
			}
		}
	}

	void PlayMap::EnableMapConn(DirectFlyNode* mc, bool enable)
	{
		if (!mc)
			return;

		MapCell * cell = GetMapCell(mc->mStartX, mc->mStartY);
		
		if (!cell || cell->Conn == 0)
			return;

		PlayMapTrap * trapConn = NULL;
		for( PlayMapObjectNode* pos = cell->mObjList.begin();
			pos != cell->mObjList.end();
			pos = pos->Next)
		{
			if( pos->This->TypeIsTrap())
			{
				PlayMapTrap * trap = static_cast<PlayMapTrap *>(pos->This);
				if (trap->DefId() == eTrapIdMapConn)
				{
					trapConn = trap;
					break;
				}
			}
		}

		if (trapConn == NULL && enable == true)
		{
			// 当前没有传送点陷阱,搞一个
			TrapDef def;
			def.id = eTrapIdMapConn;
			// 外观id

			def.cloth = mc->mClothId;
			def.damage = 0;
			def.skill_type = 0;
			def.effect_id = 0;
			def.damage_gap = 0;
			def.caller_leave_remove = false;
			def.life_time = 0;

			mManager->GenTrap(this, mc->mStartX, mc->mStartY, &def, NULL);
			return;
		}

		if (trapConn != NULL && enable == false)
		{
			RemoveObject(trapConn);
			delete trapConn;
		}
	}


}
