#include "StdAfx.h"
#include "ManagerMap.h"
#include "PlayMap.h"
#include "GameServerLib.h"
#include "PlayNpc.h"
#include "PlayMonster.h"
#include "ManagerMonDef.h"
#include "PlayMonsterNomove.h"
#include "PlayMonsterRemote.h"
#include "PlayMonsterHelper.h"
#include "PlayMonsterRemoteNomove.h"
#include "PlayMonsterPuppet.h"
#include "PlayMonsterBossNomove.h"
#include "PlayMonsterCaller.h"
#include "PlayNeutral.h"
#include "PlayNeutralRam.h"
#include "PlaySlave.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include "PlayMonsterDummy.h"
#include "ManagerFly.h"
#include "PlayMapTrap.h"
#include "ManagerTrapDef.h"

namespace OGSLib
{
	ManagerMap::ManagerMap(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
		mUpdateTime=0;
		mUpdateDuration=TimerFrame::FRAME_TICK;
	}
	ManagerMap::~ManagerMap(void)
	{
	}
	bool ManagerMap::Init()
	{
		LoadMapInfo();
		LoadNpcGen();
		LoadMonGen();
		LoadDartPath();
		return true;
	}
	void ManagerMap::Dest()
	{
		for( MapMap::iterator pos = mMapMap.begin();
			pos != mMapMap.end();
			++ pos )
		{
			UnloadMap(pos->second);
		}
		mMapMap.clear();
		for( MapDataMap::iterator pos = mMapDataMap.begin();
			pos != mMapDataMap.end();
			++ pos )
		{
			delete[] pos->second.mMapData;
		}
		mMapDataMap.clear();
		mMiniMapMap.clear();
		mMonGenVectorMap.clear();
		mNpcGenVectorMap.clear();
		mMapNpcIDName.clear();
	}
	void ManagerMap::Update(double time)
	{
		if( mUpdateTime + mUpdateDuration < time )
		{
			mUpdateTime = time;
			for( MapVector::iterator pos = mMapVector.begin();
				pos != mMapVector.end();
				++ pos )
			{
				(*pos)->Update(time);
			}
		}
	}
	PlayMap* ManagerMap::FindRealMap(const std::string& real_id)
	{
		MapMap::iterator pos = mMapMap.find(real_id);
		if( pos != mMapMap.end() )
		{
			return pos->second;
		}
		return 0;
	}
	PlayMap* ManagerMap::FindMap(const std::string& map_id,const std::string& onwer)
	{
		MapMap::iterator pos = mMapMap.find(map_id);
		if( pos != mMapMap.end() )
		{
			PlayMap* map = pos->second;
			if(map && map->FubenType()== OGSLib::MAP_TYPE::MAP_PUBLIC && map->NumberPlayer()>=map->MaxPlayer())
			{
				char tmp[64];
				int i=0;
				for(i=1;i<200;++i)
				{
					_snprintf_s(tmp,_countof(tmp),255,"%s-%03d",map_id.c_str(),i);
					tmp[63]=0;

					pos = mMapMap.find(tmp);
					if( pos != mMapMap.end() ) {
						if(pos->second && pos->second->NumberPlayer()<pos->second->MaxPlayer())
						{
							return pos->second;
						}
					}else {
						break;
					}
				}

				_snprintf_s(tmp,_countof(tmp),255,"%s-%03d",map_id.c_str(),i);
				tmp[63]=0;

				PlayMap* new_map = CopyMap(map,tmp);
				if(new_map)
				{
					return new_map;
				}
			}
			return pos->second;
		}
		else
		{
			//直接找id没找到，从001开始找
			char tmp[64];
			for(int i=1;i<200;++i)
			{
				_snprintf_s(tmp,_countof(tmp),255,"%s%03d",map_id.c_str(),i);
				tmp[63]=0;

				pos = mMapMap.find(tmp);
				if( pos != mMapMap.end() )
				{
					if( pos->second->MaxPlayer()>0 && pos->second->NumberPlayer() >= pos->second->MaxPlayer() )
					{
						continue;
					}
					if((!onwer.empty() && pos->second->GetOwnerName()!=onwer) ||(!pos->second->GetOwnerName().empty()))
					{
						continue;
					}
					return pos->second;
				}
				else
				{
					return 0;
				}
			}
		}
		return 0;
	}
	PlayMap* ManagerMap::CopyMap(PlayMap* map,const std::string& map_id)
	{
		PlayMap* new_map = 0;
		if(map && map->FubenType() == OGSLib::MAP_TYPE::MAP_PUBLIC )
		{
			std::string defid = map->mID;
			new_map = new PlayMap(this);
			new_map->isCopy = false;
			if(new_map->PreLoad(defid,map->mMapFile.c_str(),map->mName.c_str(),map->mScript.c_str(),""))
			{
				new_map->mTrigerEnterLeave = map->mTrigerEnterLeave;
				new_map->mOptionRunMonster = map->mOptionRunMonster;
				new_map->mOptionRunPlayer = map->mOptionRunPlayer;
				new_map->mOptionPKProhibit = map->mOptionPKProhibit;
				new_map->mOptionPKAllow = map->mOptionPKAllow;
				new_map->mOptionAutoEnter = map->mOptionAutoEnter;
				new_map->mNoDieDropItem = map->mNoDieDropItem;
				new_map->mEnterMapProtected = map->mEnterMapProtected;
				new_map->mOptionNoDirectFly = map->mOptionNoDirectFly;
				new_map->mOptionCanDropItem = map->mOptionCanDropItem;
				new_map->mOptionNoInteract = map->mOptionNoInteract;
				new_map->mOptionNoMount = map->mOptionNoMount;
				new_map->mOptionLockAction = map->mOptionLockAction;
				new_map->mOptionWanderHangup = map->mOptionWanderHangup;
				new_map->mOptionNoGod = map->mOptionNoGod;
				new_map->mOptionAutoAlive = map->mOptionAutoAlive;
				new_map->mOptionCanReliveStand = map->mOptionCanReliveStand;
				new_map->mOptionFightAnonym = map->mOptionFightAnonym;
				new_map->mOptionMaxPlayer = map->mOptionMaxPlayer;
				new_map->mOptionTaskShare = map->mOptionTaskShare;
				new_map->mOptionFunbenType = map->mOptionFunbenType;
				new_map->mOptionMinLv = map->mOptionMinLv;
				new_map->mOptionMinLvR = map->mOptionMinLvR;
				new_map->mOptionDisableItems = map->mOptionDisableItems;
				new_map->mOptionDisableSkills = map->mOptionDisableSkills;

				new_map->mRealID = map_id;
			}

			mMapMap[map_id] = new_map;
			mMapVector.push_back(new_map);

			new_map->Load();
			SetMapParams(new_map);

			if(mMonGenVectorMap.find(defid)!=mMonGenVectorMap.end())
			{
				MonGenVector& vector = mMonGenVectorMap[defid];
				for(MonGenVector::iterator pos = vector.begin();
					pos!=vector.end();pos++)
				{
					MapMonGen& gen = (*pos);
					std::string monname;
					MonDef* md = GetGSLib()->GetMonDefManager()->FindMonDef(gen.mon_id);
					if(md)monname=md->name;
					map->AddMiniMonGen(monname,gen.map_x,gen.map_y,gen.time);
					GenMon(new_map,gen.mon_id,gen.map_x,gen.map_y,gen.num,gen.range,gen.time,0,gen.flags.c_str());
				}
			}
			if(mNpcGenVectorMap.find(defid)!=mNpcGenVectorMap.end())
			{
				NpcGenVector& vector = mNpcGenVectorMap[defid];
				for(NpcGenVector::iterator pos = vector.begin();
					pos!=vector.end();pos++)
				{
					MapNpcGen& gen = (*pos);
					GenNpc(new_map,gen);
				}
			}

			return new_map;
		}
		return 0;
	}
	int ManagerMap::GetMiniMapID(std::string map_file)
	{
		MiniMapMap::iterator pos = mMiniMapMap.find(map_file);
		if( pos != mMiniMapMap.end() )
		{
			return pos->second;
		}
		return -1;
	}
	bool ManagerMap::FindSafeArea(int id,MapSafeArea* msa)
	{
		if( ! msa ) return false;
		for( MapSafeAreaVector::iterator pos = this->mMapSafeAreaList.begin();
			pos != this->mMapSafeAreaList.end();
			++ pos )
		{
			if( pos->id == id )
			{
				*msa = *pos;
				return true;
			}
		}
		return false;
	}

	void ManagerMap::UnloadMap(PlayMap* map)
	{
		map->Unload();
		delete map;
	}
	void ManagerMap::LoadMiniMapInfo()
	{

	}
	MapData ManagerMap::GetMapData(std::string map_file)
	{
		MapDataMap::iterator pos = mMapDataMap.find(map_file);
		if( pos != mMapDataMap.end() )
		{
			return pos->second;
		}

		MapData md;
		char mapfile[256];
		_snprintf_s(mapfile,_countof(mapfile),255,"%s%s.mapo",GetGSLib()->GetMapPath().c_str(),map_file.c_str());
		mapfile[255]=0;
		unsigned short version;
		FILE* file;
		fopen_s(&file,mapfile,"rb");
		if( file )
		{
			unsigned short width,height;
			fread(&version,2,1,file);//version
			unsigned short gwidth,gheight;
			fread(&gwidth,2,1,file);
			fread(&gheight,2,1,file);

			fread(&width,2,1,file);
			fread(&height,2,1,file);
			md.mWidth = width;
			md.mHeight = height;
			unsigned char tile_dir;
			fread(&tile_dir,1,1,file);

			int size;
			if( version == 3 )
			{
				fseek(file,gwidth*gheight*2+4,SEEK_CUR);

				fread(&size,4,1,file);
				fseek(file,size*7,SEEK_CUR);
			}
			fread(&size,4,1,file);
			md.mMapData = new char[width*height*2];
			int p=0;
			char* out=md.mMapData;
			char b;unsigned char l;
			int si=0;
			while(p<size)
			{
                fread(&b,1,1,file);
				fread(&l,1,1,file);
				for(int i = 0;i < l;i++)
				{
					*out++ = b;
					si ++;
				}
				p+=2;
			}
			mMapDataMap[map_file] = md;
			//fread(mMapData,1,width*height*4,file);
			fclose(file);
		}
		else
		{
			md.mWidth=0;
			md.mHeight=0;
			md.mMapData=0;
		}
		return md;
	}
	void ManagerMap::LoadMapInfo()
	{
		//LoadMiniMapInfo();
		LoadMapConnInfo();
		LoadMapSafeAreaInfo();
		LoadMapEventAreaInfo();
		LoadMapHoldAreaInfo();

		char temp[2024];
		
		char id_str[64];
		char file[64];
		char name[256];
		char script[64];
		int minimap;
		int type;
		int fubentype;
		int homepos;
		int relivepos;
		int maxplayer;
		int copynum;
		int minlv;
		int minlvR;
		int trigerenterleave;
		int taskshare;
		int runmon;
		int runplayer;
		int pkprohibit;
		int pkallow;
		int autoenter;
		int nodiedropitem;
		int noprotectnewer;
		int nodirectfly;
		int nodropitem;
		int nointeract;
		int nomount;
		int lockaction;
		int wanderhangup;
		int nogod;
		int autoalive;
		int norelivestand;
		int anonym;
		int nocall;
		int noctgboss;
		char diitem[512];diitem[0]=0;
		char diskill[512];diskill[0]=0;

		std::string tempstr = diitem;
		std::vector<std::string> tempvec;

		size_t size= 0;
		int version = 100;
		const char* data = mGameServerLib->GetListener()->LoadFile("mapinfo.csv",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						line[5] = 0;
						if( 0 == strcmp(line,"#v100") )
						{
							version = 100;
						}
						continue;
					}
					memset(name,0,256);
					int ret = 0;
					int parse_ok = 0;
					diitem[0] = 0;
					diskill[0] = 0;

					if( version == 100 )
					{
						ret = sscanf_s(line,"%[^,],%[^,],%[^,]\
											,%d,%[^,],%d,%d,%d,%d\
											,%d,%d,%d,%d,%d,%d,%d,%d\
											,%d,%d,%d,%d,%d\
											,%d,%d,%d,%d,%d\
											,%d,%d,%d,%d,%d\
											,%d,%d,%[^,],%[^,]",
											id_str,_countof(id_str),file,_countof(file),name,_countof(name),
											&minimap,script,_countof(script),&type,&fubentype,&maxplayer,&copynum,
											&homepos,&relivepos,&minlv,&minlvR,&trigerenterleave,&taskshare,&runmon,&runplayer,
											&pkprohibit,&pkallow,&autoenter,&nodiedropitem,&noprotectnewer,
											&nodirectfly,&nodropitem,&nointeract,&nomount,&lockaction,
											&wanderhangup,&nogod,&autoalive,&norelivestand,&anonym,
											&nocall,&noctgboss,diitem,_countof(diitem),diskill,_countof(diskill));
						if( ret == 34 || ret == 35 || ret == 36 )
						{
							parse_ok=1;
						}
					}
					if( parse_ok > 0 )
					{
						copynum = (fubentype == 2)?copynum:1;
						copynum = (copynum>200)?200:copynum;
						copynum = (copynum<=0)?1:copynum;

						char tmp[64];
						UtilString::act_2_utf8(name,strlen(name),temp,256);
						strncpy_s(name,_countof(name),temp,255);
						name[255]=0;

						for(int i=0;i<copynum;i++)
						{
							std::string id = id_str;
							std::string map_id = id_str;

							if(copynum>1)
							{
								_snprintf_s(tmp,_countof(tmp),255,"%s%03d",id.c_str(),i+1);
								tmp[63]=0;
								id = tmp;
							}

							if( mMapMap.find(id) != mMapMap.end() )
							{
								continue;
							}

							mMiniMapMap[file] = minimap;

							

							PlayMap* map = new PlayMap(this);

							map->isCopy = (fubentype == 2)?1:0;

							if( map && map->PreLoad(map_id,file,name,script,""))
							{
								map->mTrigerEnterLeave = trigerenterleave?1:0;
								map->mOptionRunMonster = runmon?1:0;
								map->mOptionRunPlayer = runplayer?1:0;
								map->mOptionPKProhibit = pkprohibit?1:0;
								map->mOptionPKAllow = pkallow?1:0;
								map->mOptionAutoEnter = autoenter?1:0;
								map->mNoDieDropItem = nodiedropitem?1:0;
								map->mEnterMapProtected = noprotectnewer?0:1;
								map->mOptionNoDirectFly = nodirectfly?1:0;
								map->mOptionCanDropItem = nodropitem?0:1;
								map->mOptionNoInteract = nointeract?1:0;
								map->mOptionNoMount = nomount?1:0;
								map->mOptionLockAction = lockaction?1:0;
								map->mOptionWanderHangup = wanderhangup?1:0;
								map->mOptionNoGod = nogod?1:0;
								map->mOptionAutoAlive = (autoalive==1)?1:0;
								map->mOptionAutoAlive = (autoalive==2)?2:0;
								map->mOptionCanReliveStand = norelivestand?0:1;
								map->mOptionFightAnonym = anonym?1:0;
								map->mOptionMaxPlayer = maxplayer;
								map->mOptionTaskShare = taskshare?1:0;
								map->mOptionFunbenType = fubentype;
								map->mOptionMinLv = minlv;
								map->mOptionMinLvR = minlvR;
								map->mOptionHomePos = homepos;
								map->mOptionRelivePos = relivepos;
								map->mOptionNoCtgBoss = noctgboss;
								tempvec.clear();
								tempstr = diitem;
								if(!tempstr.empty())
								{
									tempvec = UtilString::split(tempstr,"|");
									if(tempvec.size()>0)
									{
										for(short i=0;i<tempvec.size();i++)
										{
											int itemid = atoi(tempvec[i].c_str());
											if(itemid)
												map->mOptionDisableItems[itemid] = itemid;
										}
									}
								}
								tempstr = diskill;
								if(!tempstr.empty())
								{
									tempvec = UtilString::split(tempstr,"|");
									if(tempvec.size()>0)
									{
										for(short i=0;i<tempvec.size();i++)
										{
											int skilid = atoi(tempvec[i].c_str());
											if(skilid)
												map->mOptionDisableSkills[skilid] = skilid;
										}
									}
								}

								if(map->mOptionFunbenType == 2)
								{
									map->mRealID = id;
								}
								else
								{
									map->mRealID = map_id;
								}

								mMapMap[id] = map;
								mMapVector.push_back(map);
							}
						}
					}
				}
			}
		}
		for( MapVector::iterator pos = mMapVector.begin();
			pos != mMapVector.end();
			++ pos )
		{
			(*pos)->Load();
		}

		for(MapMap::iterator mp = mMapMap.begin();
			mp != mMapMap.end();
			++ mp)
		{
			SetMapParams(mp->second);
		}
	}

	void ManagerMap::SetMapParams(PlayMap* map)
	{
		for( MapSafeAreaVector::iterator pos = mMapSafeAreaList.begin();
			pos != mMapSafeAreaList.end();
			++ pos )
		{
			if( pos->map_id == map->GetID())
			{
				map->AddSafeArea(*pos);
			}
		}
		for( MapHoldAreaVector::iterator pos = mMapHoldAreaList.begin();
			pos != mMapHoldAreaList.end();
			++ pos )
		{
			if( pos->map_id == map->GetID())
			{
				map->AddHoldArea(&*pos);
			}
		}
		for (MapEventAreaVector::iterator pos = mMapEventAreaList.begin();
			pos != mMapEventAreaList.end();
			++pos)
		{
			if (pos->map_id == map->GetID())
			{
				map->AddEventArea(*pos);
			}
		}

		std::vector<int>& connmap = mGameServerLib->GetFlyManager()->GetMapConnNodes()[map->GetID()];
		for(std::vector<int>::iterator pos = connmap.begin();
			pos!=connmap.end();
			++pos)
		{
			if( (*pos) )
			{
				DirectFlyNode* node = mGameServerLib->GetFlyManager()->FindNode((*pos));
				if(node)
				{
					map->AddMapConn(node);
				}
			}
		}

	}

	void ManagerMap::LoadDartPath()
	{
		mDartPath.clear();
		size_t len= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("dartpath.txt",len);
		if (data && len > 0)
		{
			LEUD::ToolsMemFile f(data,len);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				MapPoint pt;
				int ret = sscanf_s(line,"%d,%d,%d",&pt.x, &pt.y, &pt.stop);
				mDartPath.push_back(pt);
			}
		}
	}

	void ManagerMap::LoadMapSafeAreaInfo()
	{
		int id;
		char map_id[256];
		int x;
		int y;
		int size;
		size_t len= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("safearea.txt",len);
		if( data && len > 0 )
		{
			LEUD::ToolsMemFile f(data,len);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int ret = sscanf_s(line,"%d%*[ \t]%[^ \t]%d%d%d",&id,map_id,_countof(map_id),&x,&y,&size);
					if( 5 == ret )
					{
						MapSafeArea sa;
						sa.id = id;
						sa.map_id = map_id;
						sa.x = x;
						sa.y = y;
						sa.size = size;
						this->mMapSafeAreaList.push_back(sa);
					}
				}
			}
		}
	}
	void ManagerMap::LoadMapHoldAreaInfo()
	{
		int id;
		char map_id[256];
		int top_x;
		int top_y;
		int under_x;
		int under_y;
		int left_x;
		int left_y;
		int right_x;
		int right_y;
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("holdarea.txt",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int ret = sscanf_s(line,"%d%*[ \t]%[^ \t]%d%d%d%d%d%d%d%d",&id,map_id,_countof(map_id),&top_x,&top_y,&under_x,&under_y,&left_x,&left_y,&right_x,&right_y);
					if( 10 == ret )
					{
						MapHoldArea sa;
						sa.id = id;
						sa.map_id = map_id;
						sa.top_x = top_x;
						sa.top_y = top_y;
						sa.under_x = under_x;
						sa.under_y = under_y;
						sa.left_x = left_x;
						sa.left_y = left_y;
						sa.right_x = right_x;
						sa.right_y = right_y;
						this->mMapHoldAreaList.push_back(sa);
					}
				}
			}
		}
	}
	void ManagerMap::LoadMapEventAreaInfo()
	{
		int id;
		char map_id[256];
		int x;
		int y;
		int size;
		size_t len = 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("eventarea.txt", len);
		if (data && len > 0)
		{
			LEUD::ToolsMemFile f(data, len);
			char line[1204];
			while (f.read_line(line, 1024))
			{
				if (line[0] != 0)
				{
					if (line[0] == '#' || line[0] == ';')
					{
						continue;
					}
					char event_id[256];
					int ret = sscanf_s(line, "%d%*[ \t]%[^ \t]%d%d%d%*[ \t]%[^ \t]", &id, map_id, _countof(map_id), &x, &y, &size, event_id, _countof(event_id));
					if (6 == ret)
					{
						MapEventArea sa;
						sa.id = id;
						sa.map_id = map_id;
						sa.x = x;
						sa.y = y;
						sa.size = size;
						sa.event_id = event_id;
						this->mMapEventAreaList.push_back(sa);
					}
				}
			}
		}
	}
	void ManagerMap::LoadMapConnInfo()
	{

	}
	bool ManagerMap::AddMap(std::string& map_id,const char* file,const char* name,const char* script,const char* options)
	{
		return true;
	}
	bool ManagerMap::RemoveMap(std::string& map_id)
	{
		return false;
	}
	void ManagerMap::LoadNpcGen()
	{
		size_t size= 0;
		int version = 100;

		int npc_id;
		char name[256];
		std::string map_id;
		char mapid[64];
		int map_x;
		int map_y;
		int cloth;
		int dir;
		int directflyID;
		char shortname[256];
		int changename;
		int showflag;
		char script[256];
		script[0]=0;
		char shopdef[256];
		shopdef[0]=0;
		char dialog[1024];
		dialog[0]=0;
		char temp[2048];

		const char* data = GetGSLib()->GetListener()->LoadFile("npcgen.csv",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						line[5] = 0;
						if( 0 == strcmp(line,"#v100") )
						{
							version = 100;
						}
						continue;
					}
					//char options[256];
					//memset(options,0,256);
					memset(name,0,256);
					memset(dialog,0,1024);
					memset(shortname,0,256);
					int ret = sscanf_s(line,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%d,%d,%[^,],%d,%d,%[^,]",&npc_id,name,_countof(name),script,_countof(script),shopdef,_countof(shopdef),mapid,_countof(mapid),&map_x,&map_y,&cloth,&dir,&directflyID,shortname,_countof(shortname),&changename,&showflag,dialog,_countof(dialog));
					if( 13 == ret || 14 == ret )
					{
						map_id = mapid;

						UtilString::act_2_utf8(name,strlen(name),temp,256);
						strncpy_s(name,_countof(name),temp,256);
						name[255]=0;

						UtilString::act_2_utf8(dialog,strlen(dialog),temp,1024);
						strncpy_s(dialog,_countof(dialog),temp,1024);
						dialog[1023]=0;

						MapNpcGen npcgen;
						npcgen.npc_id = npc_id;
						npcgen.name = name;
						npcgen.script = script;
						npcgen.map_x = map_x;
						npcgen.map_y = map_y;
						npcgen.cloth = cloth;
						npcgen.dir = dir;
						npcgen.directflyID = directflyID;
						npcgen.changename = changename?1:0;
						npcgen.showflag = showflag;
						npcgen.dialog = dialog;

						if( strcmp(shopdef,"null") != 0 && strcmp(shopdef,"0") != 0 )
						{
							std::string shopfile = "npcshop/";
							shopfile += shopdef;
							npcgen.shopdef = shopfile;
						}
						if( strcmp(shortname,"null") != 0 && strcmp(shortname,"0") != 0 )
						{
							UtilString::act_2_utf8(shortname,strlen(shortname),temp,256);
							strncpy_s(shortname,_countof(shortname),temp,256);
							shortname[255]=0;

							npcgen.shortname = shortname;
						}

						mNpcGenVectorMap[map_id].push_back(npcgen);

						mMapNpcIDName[npc_id] = name;
						

						PlayMap* map = 0;
						MapMap::iterator pos = mMapMap.find(map_id);
						if( pos != mMapMap.end() )
						{
							map = pos->second;
						}
						if( map )
						{
							GenNpc(map,npcgen);
						}
					}
				}
			}
		}
		for( std::vector<PlayNpc*>::iterator pos = mNpcList.begin();
			pos != mNpcList.end();
			++ pos )
		{
			(*pos)->LoadShopGood(GetGSLib());
		}
	}

	const char* ManagerMap::getNpcName(int npc_id)
	{
		MapNpcIDName::iterator pos = mMapNpcIDName.find(npc_id);
		if( pos != mMapNpcIDName.end() )
		{
			return pos->second.c_str();;
		}
		return "";
	}

	MapMonGen* ManagerMap::GetMapMonGen(std::string map_id, int mon_id)
	{
		if(mMonGenVectorMap.find(map_id)!=mMonGenVectorMap.end())
		{
			MonGenVector& vector = mMonGenVectorMap[map_id];
			for(MonGenVector::iterator pos = vector.begin();
				pos!=vector.end();pos++)
			{
				if (pos->mon_id == mon_id)
				{
					return &(*pos);
				}
			}
		}
		return 0;

	}

	void ManagerMap::GenNpc(PlayMap* map,MapNpcGen& gen)
	{
		PlayNpc* npc = new PlayNpcDummy(GetGSLib());
		//npc->ParseOptions(options);

		npc->Name(gen.name);
		npc->SetPosition(gen.map_x,gen.map_y);
		npc->mDir = gen.dir;
		npc->mCloth = gen.cloth;
		npc->LoadScript(GetGSLib()->GetScriptEngine(),gen.script.c_str());
		npc->mShopFile = gen.shopdef;
		if(!gen.shopdef.empty())
		{
			npc->LoadShopGood(GetGSLib());
		}
		npc->mShortName = gen.shortname;
		npc->mShowNpcFlag = gen.showflag;
		npc->mChangeName = gen.changename;
		npc->mDirectFlyID = gen.directflyID;
		npc->mDefId = gen.npc_id;
		npc->mDialog = gen.dialog;
		mNpcList.push_back(npc);

		map->AddObject(npc);
	}

	bool ManagerMap::ReLoadNpcGen()
	{
		return true;
	}
	void ManagerMap::LoadMonGen()
	{
		//char temp[256];
		size_t size= 0;

		int mon_id = 0;
		char mapid[256];
		std::string map_id;
		int map_x;
		int map_y;
		int range;
		int num;
		int time;
		char flags[256];
		flags[0]=0;

		const char* data = GetGSLib()->GetListener()->LoadFile("mongen.txt",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}

					int rn = sscanf_s(line,"%[^ \t]%d%d%d%d%d%d%s",mapid,_countof(mapid),&map_x,&map_y,&mon_id,&range,&num,&time,flags,_countof(flags));
					if( 7 == rn || 8 == rn )
					{
						map_id = mapid;
// 						UtilString::act_2_utf8(name,strlen(name),temp,256);
// 						strncpy_s(name,_countof(name),temp,255);
// 						name[255]=0;

						MapMonGen gen;
						gen.mon_id=mon_id;
						gen.map_x=map_x;
						gen.map_y=map_y;
						gen.num=num;
						gen.range=range;
						gen.time=time;
						gen.flags=flags;flags[0]=0;
						mMonGenVectorMap[map_id].push_back(gen);
						
						PlayMap* map = 0;
						MapMap::iterator pos = mMapMap.find(map_id);
						if( pos != mMapMap.end() )
						{
							map = pos->second;
						}
						if( map )
						{
							std::string monname;
							MonDef* md = GetGSLib()->GetMonDefManager()->FindMonDef(gen.mon_id);
							if(md)monname=md->name;
							map->AddMiniMonGen(monname,map_x,map_y,time);
							GenMon(map,mon_id,map_x,map_y,num,range,time,0,gen.flags.c_str());
						}
					}
				}
			}
		}
	}

	int ManagerMap::GenMon(PlayMap* map,int id,int map_x,int map_y,int num,int range,int time,int noalive,const char* flags,int tip,int limitlv, int cloth,int weapon, int wing, PlayMonster * caller, const std::string& name, int tomb_owner)
	{
		int SeedId = 0;
		MonDef* md = GetGSLib()->GetMonDefManager()->FindMonDef(id);
		if(md)
		{
			SeedId = GenMon(map,md,map_x,map_y,num,range,md->refreshTime,noalive,flags,tip,limitlv,cloth, weapon, wing, caller, name, tomb_owner);

			if( (md->mon_type%1000==2 || md->mon_type%1000==3) && map )
			{
				map->AddMiniNpc(map_x,map_y,md->name,"",0,2);
			}
		}
		return SeedId;
	}

	void ManagerMap::GenMon(PlayMap* map,const char* name,int map_x,int map_y,int num,int range,int time,int noalive,const char* flags,int tip,int limitlv, PlayMonster * caller)
	{
		/*MonDef* md = GetGSLib()->GetMonDefManager()->FindMonDef(name);
		if(md)
		{
			GenMon(map,md,map_x,map_y,num,range,time,noalive,flags,tip,limitlv,caller);
		}*/
	}

	int ManagerMap::GenMon(PlayMap* map,MonDef* md,int map_x,int map_y,int num,int range,int time,int noalive,const char* flags,int tip,int limitlv, int cloth, int weapon, int wing, PlayMonster * caller, const std::string& name, int tomb_owner)
	{
		int SeedId = 0;

		int range_flag = 0;
		if( range <= 0 ) {range = 1;range_flag=1;}
		char temp[256];
		char flag[256];
		int flag_len=0;
		int antiposion = 0;
		int anticisha = 0;
		int pos_rand_type=0;
		memset(flag,0,256);
		int team_id=-1;
		char guild_name[256];
		memset(guild_name,0,256);
		int no_owner = 0;
		int temp_int =0;
		int rightnow = 0;
		int disapear_time=5;
		int ReliveGap2 = 0;
		int MabiTimes = 0;
		int MabiProb = 0;
		int PoisonProb = 0;
		int PoisonTime = 0;
		int PoisonDefense = 0;
		int PoisonDamage = 0;
		int AoeRange = 0;
		int FenshenNum = 0;
		int FenshenHp = 0;
		int RageHp=0;
		int RagePowerMul = 0;
		int RageAttackSpeedMul = 0;
		int RageRunSpeedMul = 0;
		int FenshenMonName;
		bool landification = false;
		int UsualRecoverHp = 0;
		int BattleRecoverHp = 0;
		int CallMonMax = 0;
		int CallMonOnce = 0;
		int CallMonName;
		int DirFixed = -1;
		
		for(size_t i=0;i<=strlen(flags);i++)
		{
			if( flags[i] == ',' || flags[i] == 0 )
			{
				if( 1 == sscanf_s(flag,"antiposion(%d)",&temp_int) )
				{
					antiposion = temp_int;
				}
				if( 1 == sscanf_s(flag,"anticisha(%d)",&temp_int) )
				{
					anticisha = temp_int;
				}
				if( 1 == sscanf_s(flag,"usualRecoverHp(%d)",&temp_int) )
				{
					UsualRecoverHp = temp_int;
				}
				if( 1 == sscanf_s(flag,"battleRecoverHp(%d)",&temp_int) )
				{
					BattleRecoverHp = temp_int;
				}
				if( 1 == sscanf_s(flag,"disapeartime(%d)",&temp_int) )
				{
					disapear_time = temp_int;
				}
				if( 1 == sscanf_s(flag,"team(%d)",&team_id) )
				{
					int nothing=1;
					int just_do_nothing=1;
				}
				if( 1 == sscanf_s(flag,"guild(%[^)])",guild_name,_countof(guild_name)) )
				{
					int nothing=1;
					int just_do_nothing=1;
				}
				if( 1 == sscanf_s(flag,"dirfixed(%d)",&temp_int) )
				{
					DirFixed = temp_int;
				}
				if( 0 == strcmp(flag,"no_owner") )
				{
					no_owner = 1;
				}
				if( 0 == strcmp(flag,"pos_rand_type_1") )
				{
					pos_rand_type = 1;
				}
				if( 0 == strcmp(flag,"update"))
				{
					rightnow = 1;
				}
				int times = 0;
				if (1 == sscanf_s(flag, "time2(%d)", &times))
				{
					ReliveGap2 = times;
				}
				if (0 == strcmp(flag, "landification"))
				{
					landification = true;
				}
				{
					int times_mabi = 0;////麻痹多久
					int prob_mabi = 0;////麻痹的概玿
					if (2 == sscanf_s(flag, "mabi(%d:%d)", &prob_mabi, &times_mabi))
					{
						MabiProb = prob_mabi;
						MabiTimes = times_mabi;
					}
				}
				{
					int prob_shidu = 0;////施毒几率
					int times_shidu = 0;////施毒时间
					int defense_sub_shidu = 0;///施毒减少多少防御
					int damage_shidu = 0;///施毒每秒的基础伤害
					if (4 == sscanf_s(flag, "shidu(%d:%d:%d:%d)", &prob_shidu, &times_shidu, &defense_sub_shidu, &damage_shidu))
					{
						PoisonProb = prob_shidu;
						PoisonTime = times_shidu;
						PoisonDefense = defense_sub_shidu;
						PoisonDamage = damage_shidu;
					}
				}
				{
					int range_aoe = 0;////AOE的范囿
					if (1 == sscanf_s(flag, "aoe(%d)", &range_aoe))
					{
						AoeRange = range_aoe;
					}
				}
				{
					int fenshen_hp = 0;////到达多少血量开始分躿
					int fenshen_num = 0;////分出几个小怿
					int fenshen_monname = 0;
					char temp[64];
					if (3 == sscanf_s(flag, "fenshen(%d:%d:%d)", &fenshen_hp, &fenshen_num, fenshen_monname))
					{
						FenshenHp = fenshen_hp;
						FenshenMonName = fenshen_monname;
						//fenshen_monname[63] = 0;
						FenshenNum = fenshen_num;
					}
				}
				{
					int rage_hp = 0;//到达多少血量之后会开始狂暿
					int rage_power = 0;///狂暴力量加成
					int rage_attack_speed = 0;////狂暴攻击速度加成
					int rage_run_speed = 0;///狂暴移动速度加成
					if (4 == sscanf_s(flag, "rage(%d%*[ \t]%d%*[ \t]%d%*[ \t]%d)", &rage_hp, &rage_power, &rage_attack_speed, &rage_run_speed))
					{
						RageHp = rage_hp;
						RagePowerMul = rage_power;
						RageAttackSpeedMul = rage_attack_speed;
						RageRunSpeedMul = rage_run_speed;
					}
				}
				{
					// 召唤类型属怿
					int call_once_count = 0;
					int call_max_count = 0;
					int call_mon_name =0;
					char temp[64];
					if (3 == sscanf_s(flag, "callmon(%d:%d:%d)", &call_once_count, &call_max_count, call_mon_name))
					{
						CallMonOnce = call_once_count;
						CallMonMax = call_max_count;
						CallMonName = call_mon_name;
						CallMonOnce = call_once_count;
						CallMonMax = call_max_count;
					}
				}
				memset(flag,0,256);
				flag_len=0;
				continue;
			}
			else
			{
				flag[flag_len++] = flags[i];
			}
		}
		for(int i = 1; i<num+1;i++)
		{
			int rd = 0;
			if (range > 0)
				rd = i*range/num + 1;

			PlayMonster* monster = LoadMonster(md);
			if( monster )
			{
				monster->m_ReliveGap = time;
				//monster->mDisapearTime = disapear_time;
				monster->m_ReliveGap2 = ReliveGap2;
				monster->mMabiProb = MabiProb;
				monster->mMabiGap = MabiTimes;
				monster->mPoisonProb = PoisonProb;
				monster->mPoisonTime = PoisonTime;
				monster->mPoisonDefense = PoisonDefense;
				monster->mPoisonDamage = PoisonDamage;
				monster->mAoeRange = AoeRange;
				monster->mFenshenHp = FenshenHp;
				monster->mFenshenMonName = FenshenMonName;
				monster->mFenshenNum = FenshenNum;
				monster->mRageHp = RageHp;
				monster->mRagePowerMul = RagePowerMul;
				monster->mRageAttackSpeedMul = RageAttackSpeedMul;
				monster->mRageRunSpeedMul = RageRunSpeedMul;
				monster->mLandification = landification;
				monster->mReliveMap = map->GetRealID();
				monster->mCallMonCountOnce = CallMonOnce;
				monster->mCallMonMaxCount = CallMonMax;
				monster->mCallMonName = CallMonName;
				monster->mCaller = caller;

				if (cloth > 0)
					monster->mCloth = cloth;
				if (weapon > 0)
					monster->mWeapon = weapon;
				if (wing > 0)
					monster->mHair = wing;

				if (caller)
					caller->IncCallMon(monster);
				
				int ox=map_x,oy=map_y;
				if( rd > 0 && !range_flag)
				{
					if( pos_rand_type == 0 )
					{
						map->GenValidateRandomPosition(map_x,map_y,range,&ox,&oy, 1);
					}
					else
					{
						int time = 0;
						while( time < 10 )
						{
							if( map->GenValidatePosition(map_x + (rand() % (rd*2)) - rd,map_y + (rand() % (rd*2)) - rd,&ox,&oy) )
							{
								break;
							}
							time ++;
						}
					}
				}
				else
				{
					ox = map_x;
					oy = map_y;
					rd = 0;
				}

				int dir = 0;
				dir = rand()%8;

				if (monster->mDirFixed >= 0)
				{
					dir = monster->mDirFixed;
				}

				// 脚本设置固定方向优先
				if (DirFixed >= 0)
				{
					dir = DirFixed;
					monster->mDirFixed = DirFixed;
				}
				if (name == "")
				{
					InitMonster(monster,ox,oy,dir,md->name);
				}
				else
					InitMonster(monster,ox,oy,dir,name.c_str());
				if( team_id > 0 )
				{
					monster->mTeamID = team_id;
				}
				if( monster->TypeIsNeutral() )
				{
					PlayNeutral* neutral = static_cast<PlayNeutral*>(monster);
					if( guild_name[0] != 0 )
					{
						
						strncpy_s(neutral->mGuildName,_countof(neutral->mGuildName),guild_name,255);
						neutral->mGuildName[255]=0;
					}
					if (tomb_owner > 0)
					{
						neutral->SetMonsterSeedId(tomb_owner);
						PlayMonster* pMon = map->FindMonster(tomb_owner);
						if (pMon)
						{
							neutral->RestartDisappear(pMon->m_ReliveGap + pMon->mDisapearTime);
						}
					}
				}
				//MonDef* md = GetGSLib()->GetMonDefManager()->FindMonDef(name);
				if( strlen(flags)>0 )monster->DoOpt(flags);
				monster->BaseAttributes()->mAntiPoison = antiposion;
				monster->ResetAttribute();
				monster->mAntiCisha = anticisha;
				monster->mDropNoOwner = no_owner ;
				if (range_flag){monster->mReliveRange = 0;}else{monster->mReliveRange = rd;}
				monster->mNoRilive = noalive;
				monster->mlimitlv = limitlv;
				//monster->mDropList = map_manager->GetGSLib()->GetMonDefManager()->FindMonDropDef(monster->Name());
				//monster->mTalkList = map_manager->GetGSLib()->GetMonDefManager()->FindMonTalkDef(monster->Name());
				//monster->mDropGroup = map_manager->GetGSLib()->GetMonDefManager()->FindMonDropGroupDef(monster->Name());
				monster->mDropList2 = GetGSLib()->GetMonDefManager()->FindNewMonDropGroupDef2(monster->mDefId);
				monster->SetManager(GetGSLib()->GetMonsterManager(),rightnow);
				monster->mUsualRecoveHP = UsualRecoverHp;
				monster->mBattleRecoverHp = BattleRecoverHp;
				map->AddObject(monster);
				if(!SeedId)
				{
					SeedId = monster->ID();
				}
			}
		}
		return SeedId;
	}

	bool ManagerMap::GenTrap(PlayMap* map, int map_x,int map_y, TrapDef * def,PlayActiveObject *caller)
	{
		if (!def)
			return false;

		if (map->GetBlockData(map_x, map_y, 1) == 999)
			return false;

		MapCell* cell = const_cast<MapCell*>(map->FindMapCell(map_x,map_y));
		PlayMapTrap* trap_pre = NULL;
		if( cell && cell->MapTrap_Num > 0)
		{
			for( PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();
				pos = pos->Next)
			{
				if( pos->This->TypeIsTrap() && ((PlayMapTrap*)(pos->This))->GetTrapTypeId() == def->id)
				{
					trap_pre = (PlayMapTrap*)(pos->This);
					break;
				}
			}
		}

		if (trap_pre)
			trap_pre->RemoveFromMap();
			//map->RemoveObject(trap_pre);

		// 同一个位置只能放一个同种类型的陷阱
		PlayMapTrap* trap = new PlayMapTrap(map_x, map_y, def, caller);
		if( trap )
		{
			map->AddObject(trap);
			if (caller)
				caller->IncCallTrap(trap);
			return true;
		}
		return false;
	}

	bool ManagerMap::RemoveTrap(PlayMap* map, int map_x,int map_y, int id)
	{
		MapCell* cell = const_cast<MapCell*>(map->FindMapCell(map_x,map_y));
		PlayMapTrap * pTrap = NULL;
		if( cell && cell->MapTrap_Num > 0)
		{
			for( PlayMapObjectNode* pos = cell->mObjList.begin();
				pos != cell->mObjList.end();
				pos = pos->Next)
			{
				if( pos->This->TypeIsTrap() && ((PlayMapTrap*)(pos->This))->GetTrapTypeId() == id)
				{
					pTrap = (PlayMapTrap*)(pos->This);
					break;
				}
			}

			if (pTrap)
			{
				map->RemoveObject(pTrap);
				delete pTrap;
				return true;
			}
			
		}
		return false;
	}

	void ManagerMap::InitMonster(PlayMonster* monster,int x,int y,int dir,const char* name)
	{
		monster->Name(name);
		monster->mReliveX = x;
		monster->mReliveY = y;
		monster->mReliveDir = dir;
		monster->ResetAttribute();
		monster->ForceMove(x,y,dir);
	}

	PlayAIObject* ManagerMap::LoadAIObject(MonDef* md,PlayAIObject* aio,char* opt)
	{
		MonAtkAI * pAI = mGameServerLib->GetMonDefManager()->FindAtkAI(md->atk_ai);
		if (pAI)
		{
			aio->m_AI = *pAI;
		}
		aio->mBaseAttribute.mMaxHp = md->hp;
		aio->mBaseAttribute.mMaxMp = md->mp;

		aio->mBaseAttribute.mAC = md->ac;
		aio->mBaseAttribute.mACMax = md->acMax;
		aio->mBaseAttribute.mMAC = md->mac;
		aio->mBaseAttribute.mMACMax = md->macMax;
		aio->mBaseAttribute.mDC = md->dc;
		aio->mBaseAttribute.mDCMax = md->dcMax;
		aio->mBaseAttribute.mMC = md->mc;
		aio->mBaseAttribute.mMCMax = md->mcMax;
		aio->mBaseAttribute.mSC = 0;
		aio->mBaseAttribute.mSCMax = 0;
		aio->mBaseAttribute.mAccuracy = md->accuracy;
		aio->mBaseAttribute.mDodge = md->dodge;
		aio->mBaseAttribute.mBaojiProb = md->CRIRate;
		aio->mBaseAttribute.mTenacity = md->DUCrate;
		aio->mAttackGap = md->attack_spd;
		aio->mWalkGap = md->walk_gap2;
		aio->mWalkStep = md->walk_step;
		aio->mLevel = md->lvl;
		aio->mDefId = md->id;
		aio->mCloth = md->res_id;
		aio->mLevelUpExp = md->exp;
		aio->mCoolEye = md->cooleye;
		aio->mNoHit = md->no_hit;
		if (md->weapon > 0)
			aio->mWeapon = md->weapon;
		if (md->wing > 0)
			aio->mHair = md->wing;

		if( aio->mScriptPackage.empty() && strlen(md->script) > 0 && strcmp(md->script,"null") != 0 )
		{
			aio->LoadScript(GetGSLib()->GetScriptEngine(),md->script);
		}
		aio->ParseOpt(opt);
		return aio;
	}

	PlayMonster* ManagerMap::LoadMonster(MonDef* md,PlayMonster* monn)
	{
		//MonDef* md = GetGSLib()->GetMonDefManager()->FindMonDef(mondefname);
		if( md )
		{
			PlayMonster* mon=monn;
			char aii[256];aii[0]=0;
			char opt[1024];opt[0]=0;
			sscanf_s(md->ai.c_str(),"%[^|]|%s",aii,_countof(aii),opt,_countof(opt));
			if( ! mon )
			{
				if( strcmp(aii,"default")==0 )
				{
					mon = new PlayMonsterDummy;
				}
				if( strcmp(aii,"nomove")==0 )
				{
					mon = new PlayMonsterNomoveDummy;
				}
				if( strcmp(aii,"remote")==0 )
				{
					mon = new PlayMonsterRemoteDummy;
				}
				if( strcmp(aii,"helper")==0 )
				{
					mon = new PlayMonsterHelperDummy;
				}
				if( strcmp(aii,"remotenomove" )==0)
				{
					mon = new PlayMonsterRemoteNomoveDummy;
				}
				if( strcmp(aii,"puppet") == 0 )
				{
					mon = new PlayMonsterPuppetDummy;
				}
				if( strcmp(aii,"bossnomove") == 0 )
				{
					mon = new PlayMonsterBossNomoveDummy;
				}
				if( strcmp(aii,"neutral") == 0 )
				{
					mon = new PlayNeutralDummy;
				}
				if( strcmp(aii,"neutralram") == 0 )
				{
					mon = new PlayNeutralRamDummy;
				}
				if( strcmp(aii,"plant" ) == 0 )
				{
					mon = new PlayMonsterPlantDummy;
				}
				if( strcmp(aii,"collection" ) == 0 )
				{
					PlayMonsterCollectionDummy* m = new PlayMonsterCollectionDummy;
					mon = m;
					m->mCollectTime = md->colltime;
				}
				if ( strcmp(aii, "caller") == 0)
				{
					mon = new PlayMonsterCallerDummy;
				}
				if ( strcmp(aii, "explode") == 0)
				{
					mon = new PlayMonsterExplodeDummy;
				}
				//测试代码
				if(!mon)
				{
					mon = new PlayMonsterDummy;
				}
			}
			if( ! mon ) return 0;

			LoadAIObject(md,mon,opt);
			mon->m_nTombId = 0;
			mon->mTargetEffectResID = md->appr;
			mon->mOwnerEffectType = md->effect_type;
			mon->mOwnerEffectResID = md->effect_res;
			mon->mDeathExp = md->exp;
			mon->mPowerExp = md->power_exp;
			mon->mTalkGap = md->talk_gap;
			mon->mMonType = md->mon_type;
			mon->mFollowRange = md->fllow_range;
			mon->mDisapearTime = md->deadTime/1000;
			mon->mPatrolRange = md->patrol_range;
			mon->mDirFixed = md->dirFixed;
			mon->mNoShowHitter = md->no_show_hitter;

			//免伤
			mon->mBaseAttribute.mDamReduce = md->ADTR;

			mon->m_nMonGroup = md->monGroup;
			MonAtkAI * pAI = mGameServerLib->GetMonDefManager()->FindAtkAI(md->atk_ai);
			if (pAI)
			{
				//mon->m_AI = *pAI;
				mon->mSwitchTargetType = pAI->switch_target_type;
				if (mon->mSwitchTargetType == 1)
				{
					mon->mSwitchTargetInteval = 0;
					mon->mHatredDamageRatio = pAI->switch_target_param;
				}
				else
				{
					mon->mSwitchTargetInteval = pAI->switch_target_param;
					mon->mHatredDamageRatio = 1;
				}
				mon->mHatredDistanceRatio = pAI->hatred_distance_ratio;
			}

			return mon;
		}
		return 0;
	}

	const std::vector<MapPoint> & ManagerMap::GetDartPath()
	{
		return mDartPath;
	}

}
