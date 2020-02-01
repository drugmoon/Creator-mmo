#pragma once

namespace OGSLib
{
	class PlayMap;
	class GameServerLib;
	class ScriptLua;
	class MapData;
	class MapConn;
	class MapNpcGen;
	class MapMonGen;
	class MapSafeArea;
	class MapEventArea;
	class MapHoldArea;
	class PlayMonster;
	class PlayNpc;
	class PlayAIObject;
	class PlayActiveObject;
	struct MonDef;
	struct TrapDef;
	
	struct MapPoint
	{
		int x;
		int y;
		int stop;
	};

	class ManagerMap
	{
		friend class GameServerLib;
		friend class GMCommand;
	public:
		typedef std::map<std::string,PlayMap*> MapMap;
		typedef std::map<std::string,int> MiniMapMap;
		typedef std::map<std::string,MapData> MapDataMap;
		typedef std::vector<MapMonGen> MonGenVector;
		typedef std::map<std::string,MonGenVector> MonGenVectorMap;
		typedef std::vector<MapNpcGen> NpcGenVector;
		typedef std::map<std::string,NpcGenVector> NpcGenVectorMap;
		typedef std::vector<MapConn> MapConnVector;
		typedef std::vector<MapSafeArea> MapSafeAreaVector;
		typedef std::vector<MapHoldArea> MapHoldAreaVector;
		typedef std::vector<MapEventArea> MapEventAreaVector;

		typedef std::map<int,std::string> MapNpcIDName;
	private:
		MiniMapMap mMiniMapMap;
		MapDataMap mMapDataMap;
		//MapConnVector mMapConnList;
		MapMap mMapMap;
		MapNpcIDName mMapNpcIDName;
		MonGenVectorMap mMonGenVectorMap;
		NpcGenVectorMap mNpcGenVectorMap;

		typedef std::vector<PlayMap*> MapVector;
		MapVector mMapVector;
		MapSafeAreaVector mMapSafeAreaList;
		MapHoldAreaVector mMapHoldAreaList;
		MapEventAreaVector mMapEventAreaList;
		GameServerLib* mGameServerLib;
		std::vector<PlayNpc*> mNpcList;
		double mUpdateTime;
		double mUpdateDuration;
		std::vector<MapPoint> mDartPath;
	public:
		ManagerMap(GameServerLib* gslib);
		virtual ~ManagerMap(void);
	public:
		bool Init();
		void Dest();
		//MapConnVector& GetMapConn(){return mMapConnList;}
		int GetMiniMapID(std::string map_file);
		MapData GetMapData(std::string map_file);
		PlayMap* FindRealMap(const std::string& real_id);
		PlayMap* FindMap(const std::string& map_id,const std::string& onwer="");
		bool FindSafeArea(int id,MapSafeArea* msa);
		GameServerLib* GetGSLib(){return mGameServerLib;}
		bool AddMap(std::string& map_id,const char* file,const char* name,const char* script,const char* options);
		bool RemoveMap(std::string& map_id);
		void Update(double time);
		const std::vector<MapPoint> & GetDartPath();
	private:
		PlayMap* CopyMap(PlayMap* map,const std::string& map_id);
		void SetMapParams(PlayMap* map);
		void GenNpc(PlayMap* map,MapNpcGen& gen);

		void LoadDartPath();
		void LoadMapInfo();
		void LoadMiniMapInfo();
		void LoadMapConnInfo();
		void LoadMapSafeAreaInfo();
		void LoadMapEventAreaInfo();
		void LoadMapHoldAreaInfo();
		//PlayMap* LoadMap(std::string& map_id,const char* file,const char* name,const char* script,const char* options);
		void UnloadMap(PlayMap* map);
	public:
		void LoadNpcGen();
		bool ReLoadNpcGen();
		PlayMonster* LoadMonster(MonDef* md,PlayMonster* mon=0);
		PlayAIObject* LoadAIObject(MonDef* md,PlayAIObject* aio,char* Opt);
		void InitMonster(PlayMonster* mon,int x,int y,int dir,const char* name);
		void LoadMonGen();
		int GenMon(PlayMap* map,int id,int map_x,int map_y,int num,int range,int time,int noalive,const char* flags,int tip=1, int limitlv=0, int cloth = 0,int weapon= 0, int wing = 0, PlayMonster * caller = 0, const std::string& name="",int tomb_owner = 0);
		int GenMon(PlayMap* map,MonDef* md,int map_x,int map_y,int num,int range,int time,int noalive,const char* flags,int tip=1,int limitlv=0, int cloth = 0,int weapon= 0, int wing = 0, PlayMonster * caller = 0, const std::string& name="",int tomb_owner = 0);
		bool GenTrap(PlayMap* map, int map_x,int map_y,TrapDef * def,PlayActiveObject* caller = 0);
		bool RemoveTrap(PlayMap* map, int map_x,int map_y, int id);

		const char* getNpcName(int npc_id);

		MapMonGen* GetMapMonGen(std::string map_id, int mon_id);
	private:
		void GenMon(PlayMap* map,const char* name,int map_x,int map_y,int num,int range,int time,int noalive,const char* flags,int tip=1,int limitlv=0, PlayMonster * caller = 0);
	};
}