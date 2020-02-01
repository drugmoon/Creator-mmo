#pragma once
#include "PlayMapObjectNode.h"
#include <deque>
namespace OGSLib
{
	class PlayMapObject;
	class PlayNpc;
	class PlayMonster;
	class PlayMapItem;
	class PlayNeutral;
	class ManagerMap;
	class PlayActiveObject;
	class MapZone;
	class DomainPlay;
	class DirectFlyNode;
	class MapData
	{
	public:
		unsigned short mWidth;
		unsigned short mHeight;
		char* mMapData;
	};
	class MapCell
	{
	public:
		int MapItem_Num;
		int MapMon_Num;
		int MapNpc_Num;
		int MapTrap_Num;
		int MapPlayer_Num;
// 		int HuoQian_Id;
// 		int HuoQian_LockID;
// 		double HuoQian_OutTime;
		PlayMapObjectNodeList mObjList;
		int SafeArea;
		int HoldArea;
		int EventArea;
		std::string EventIdArea;
		int Conn;
		void Clear()
		{
			MapTrap_Num = 0;
			MapItem_Num = 0;
			MapMon_Num = 0;
			MapNpc_Num = 0;
			MapPlayer_Num = 0;
// 			HuoQian_Id = 0;
// 			HuoQian_LockID = 0;
// 			HuoQian_OutTime = 0;
			SafeArea = -1;
			HoldArea = -1;
			EventArea = -1;
			Conn = 0;
		}
	};
	class MapConn
	{
	public:
		std::string from_id;
		std::string from_name;
		int from_x;
		int from_y;
		std::string to_id;
		std::string to_name;
		int to_x;
		int to_y;
		int size;
	};
	class MapMonGen
	{
	public:
		int mon_id;
		int map_x;
		int map_y;
		int num;
		int range;
		int time;
		std::string flags;
	};
	class MapNpcGen
	{
	public:
		int npc_id;
		int map_x;
		int map_y;
		std::string name;
		std::string script;
		std::string shopdef;
		int cloth;
		int dir;
		int directflyID;
		int changename;
		int showflag;
		std::string dialog;
		std::string shortname;
	};
	class MapSafeArea
	{
	public:
		int id;
		std::string map_id;
		int x;
		int y;
		int size;
	};
	class MapHoldArea
	{
	public:
		int id;
		std::string map_id;
		int top_x;
		int top_y;
		int under_x;
		int under_y;
		int left_x;
		int left_y;
		int right_x;
		int right_y;
	};
	class MapEventArea
	{
	public:
		int id;
		std::string map_id;
		int x;
		int y;
		int size;
		std::string event_id;
	};
	class MapMiniNpc
	{
	public:
		int x;
		int y;
		std::string npc_name;
		std::string short_name;
		int npc_directfly_ID;
		int showNpcFlag;
	};
	class MapMiniMonGen
	{
	public:
		std::string name;
		int x;
		int y;
		int time;
	};
	class PlayMapTrap;
	// 动态传送点
	struct DyMayConn
	{
		int x;
		int y;
		int target_x;
		int target_y;
		int cloth;
		int target_range;
		std::string target_map_id;
		PlayMapTrap * trap;

		DyMayConn()
		{
			x = -1;
			y = -1;
			target_x = -1;
			target_y = -1;
			target_range = 0;
			cloth = 0;
			trap = NULL;
		}
	};
	enum MAP_TYPE
	{
		MAP_STAND=0,
		MAP_PUBLIC=1,
		MAP_PERSONAL=2,
	};
	class PlayMap
	{
	public:
		friend class ManagerMap;
		typedef std::deque<PlayMapObject*> MapObjectDeque;
		typedef std::vector<DirectFlyNode*> MapConnVector;
		typedef std::vector<MapSafeArea> MapSafeAreaVector;
		typedef std::vector<MapEventArea> MapEventAreaVector;
		typedef std::vector<MapMiniMonGen>  MapMiniMonGenVector;
		typedef std::vector<MapMiniNpc>  MapMiniNpcVector;
		typedef std::map<std::string, std::string> MapVars;
		typedef std::map<int, DyMayConn> DyMapConnMap;
	private:
		DyMapConnMap mDyMapConnMap;
		MapConnVector mConnList;
		MapSafeAreaVector mSafeAreaList;
		MapMiniNpcVector mMiniNpcList;
		MapMiniMonGenVector mMapMiniMonGen;
		//PlayMapObjectNodeList mObjectList;
		MapHoldArea* mHoldArea;
		MapEventAreaVector mEventAreaList;

		MapObjectDeque mObjectList;

		MapVars mVars;

		double mTimer[10];
		bool mTimerUpdateTag;
		bool mOwnerWait;////地图等待状态
		int mOwnerWaitTime;////地图等待时间

		std::deque<PlayMapObject*>* mZones;
		int mZoneWidth;
		int mZoneHeight;

		MapCell** mCells;

		ManagerMap* mManager;
		std::string mID;
		std::string mRealID;
		std::string mMapFile;
		int mMiniMapID;
		int mWeather;
		std::string mName;
		std::string mScript;
		std::string mOptions;
		std::string mOwnerName;
		MapData mMapData;

		int mOptionMinLv;
		int mOptionMinLvR;
		int mOptionTaskShare;
		int mOptionFunbenType;

		int mOptionRunMonster;
		int mOptionRunPlayer;
		int mOptionAutoEnter;
		int mOptionMaxPlayer;
		int mOptionCanDropItem;
		int mOptionCanTrade;
		int mOptionCanRandGo;
		int mOptionPKProhibit;
		int mOptionPKAllow;
		int mOptionAutoAlive;
		int mOptionCanReliveStand;
		int mTrigerEnterLeave;
		int mOptionNoProtectNewer;
		int mOptionExpMul;
		int mEnterMapProtected;
		int mAutoSecondDamage;
		int mNoDieDropItem;
		int mOptionCoupleForceMove;
		int mOptionNoDirectFly;
		int mOptionNoInteract;
		int mOptionNoMount;
		int mOptionLockAction;
		int mOptionWanderHangup;
		int mOptionFightAnonym;
		int mOptionNoGod;
		int mOptionHomePos;
		int mOptionRelivePos;
		int mOptionNoCtgBoss;

		double mAutoSecondTick;
		std::map<int, int> mOptionDisableItems;
		std::map<int, int> mOptionDisableSkills;
		//副本标记
		int isCopy;
	public:
		PlayMap(ManagerMap* manager);
		virtual ~PlayMap(void);
	public:
		const std::string& GetName(){ return mName; }
		void SetName(const std::string& name){mName = name;}
		ManagerMap* GetManager(){ return mManager; }
		int GetBlockData(int x, int y, int check_obj);
		int SetBlockData(int x, int y);
		int GetBlockDataMon(int x, int y, int check_obj);

		const std::string& GetID(){ return mID; }
		const std::string& GetRealID(){ return mRealID; }
		int GetMiniMapID(){ return mMiniMapID; }
		const std::string& GetMapFile(){ return mMapFile; }

		int Width(){ return mMapData.mWidth; }
		int Height(){ return mMapData.mHeight; }
		int AutoEnter(){ return mOptionAutoEnter; }
		void AutoEnter(int a) {mOptionAutoEnter = a;}
		int CanDropItem(){ return mOptionCanDropItem; }
		int CanTrade(){ return mOptionCanTrade; }
		int CanRandGo(){ return mOptionCanRandGo; }
		int RunMonster(){ return mOptionRunMonster; }
		int RunPlayer(){ return mOptionRunPlayer; }
		int PKProhibit(){ return mOptionPKProhibit; }
		void PKProhibit(int v){ mOptionPKProhibit = v ? 1 : 0; }
		int PKAllow(){ return mOptionPKAllow; }
		int NoDieDropItem(){ return mNoDieDropItem; }
		int NoProtectNewer(){ return mOptionNoProtectNewer; }
		void PKAllow(int v){ mOptionPKAllow = v ? 1 : 0; }
		int AutoAlive(){ return mOptionAutoAlive; }
		int CanReliveStand(){ return mOptionCanReliveStand; }
		int AutoEnterLeave(){return mTrigerEnterLeave;}
		int EnterMapProtected(){return mEnterMapProtected;}
		int ExpMul(){return mOptionExpMul;}
		void SetExpMul(int mul){mOptionExpMul = mul;}
		void SetOwnerNmae(const std::string& name){ mOwnerName = name;}
		const std::string& GetOwnerName(){ return mOwnerName; }
		void ClearOwner(){ mOwnerName = ""; }
		void SetWaitTag(bool result);
		int NoInteract(){return mOptionNoInteract;}
		int NoMount(){return mOptionNoMount;}
		int SetNoMount(int flag){mOptionNoMount = flag;}
		int LockAction(){return mOptionLockAction;}
		int WanderHangup(){return mOptionWanderHangup;}
		int FightAnonym(){return mOptionFightAnonym;}
		int NoGod(){return mOptionNoGod;}
		int HomePos(){return mOptionHomePos;}
		int RelivePos(){return mOptionRelivePos;}
		int NoCtgBoss(){return mOptionNoCtgBoss;}
		void AutoAlive(int v);
		bool IsItemDisabled(int item_id);
		bool IsSkillDisabled(int skill_id);
		int MapFlags();
		int CoupleForceMove(){return mOptionCoupleForceMove;}
		int weather(){return mWeather;}

		int EnterMinLv(){return mOptionMinLv;}
		int EnterMinLvR(){return mOptionMinLvR;}
		int FubenType(){return mOptionFunbenType;}
		int TaskShare(){return mOptionTaskShare;}
		int MaxPlayer(){return mOptionMaxPlayer;}

		void AddObject( PlayMapObject* obj );
		void RemoveObject( PlayMapObject* obj);
		void ResetOwner(PlayMapObject* obj);
		int GetZoneID(int newx,int newy);

		void NotifyMove( PlayMapObject* obj,int newx,int newy,int oldx,int oldy);
		void NotifyTrapInOut( PlayMapObject* obj,int newx,int newy,int oldx,int oldy);

// 		bool LockHuoQianCell( int x,int y,int from_id,int lock_id,double time,double out_time );
// 		bool CheckHuoQianCell( int x,int y,int from_id,int lock_id,double time );
// 		int FindHuoQianOwner( int x,int y,double time );

		const MapCell * FindMapCell(int x,int y);
		int AddMapLogicBlock(PlayMapObject* obj,int x,int y);
		int RemMapLogicBlock(PlayMapObject* obj,int x,int y);

		bool GenValidateMapItemPosition(int cx,int cy,int &x,int &y);
		bool GenValidateRandomPosition(int* x,int* y);
		bool GenValidateRandomPosition(int cx,int cy,int range,int* x,int *y, int check_pos = 2);
		bool GenValidatePosition(int ccx,int ccy,int* x,int* y);

		PlayNpc* FindNPC(int id);
		PlayNpc* FindNPC(std::string& npc_name);
		PlayMapItem* FindMapItemNear(int x,int y,int id);
		DomainPlay* RandPlayer(int id);
		PlayActiveObject * FindPlayerOrSlave(int id);
		PlayMonster * FindMonster(int id);
		PlayActiveObject * FindMonsterOrNeutral(int id);

		const MapConnVector* GetConn(){return &mConnList;}
		const MapSafeAreaVector* GetSafeArea(){return &mSafeAreaList;}
		const MapMiniNpcVector* GetMiniNpc(){return &mMiniNpcList;}
		const MapMiniMonGenVector* GetMiniMonGen(){return &mMapMiniMonGen;}
		const MapObjectDeque* GetMapObjectDeque(){return &mObjectList;}

		int CheckMapSafeArea(int newx,int newy);
		int CheckMapHoldArea(int newx,int newy);
        bool CheckMapConn(PlayMapObject* obj,int newx,int newy);

		void Chat(PlayMapObject* src,char* msg,bool self);

		const char* GetVar(const std::string& var);
		void SetVar(const std::string& var,const std::string& value);

		void Update(double time);
		int SetMapTimer(int i,int esp);
		double GetMapTimer(int i);

		void FindHoldKuafuGuild(std::string&a,std::string& g);
		void FindHoldGuild(std::string& guild);

		int NumberMonster(const char* monster_name);
		int NumberMonster(int mon_id);
		bool IsNpc(const char* npc_name);
		int NumberNeutral(const char* neutral_name);
		int NumberPlayer();
		int NumberItem();
		int NumberTeamMember(int team_id);
		void ExeTeamMember(int team_id,const char* exe);
		int Alert(int lv,int flags,const char* msg);
		int PlayEffect(int skill_type,int res_id);
		void ClearMon();
		//依据def_id清理怪物
		void ClearMonById(int id);
		void ClearMonByName(const char* monster_name);

		//
		void ClearNeutralBySeedID(int seed_id);

		PlayNeutral* FindNeutralBySeedID(int seed_id);

		void ClearNeutral();
		void ClearItem();
		void ClearMiniNpc();
		int PublicTarget(PlayActiveObject* obj, bool lock = false);
		void MapExe(const char* code);
		int NoDirectFly(){return mOptionNoDirectFly;}
		bool FindMiniNpc(const char* name);
		void ExeNotifyMonDie();

		int AddDyMapConn(int nodeid, int cloth, int x, int y, std::string target_map_id, int target_x, int target_y, int range);
		int RemoveDyMapConn(int id);
		void EnableMapConn(DirectFlyNode* mc, bool enable);
	public:
		bool PreLoad(const std::string& id,const char* file,const char* name,const char* script,const char* options);
		bool Load();
		void Unload();
	private:
		MapCell * GetMapCell(int x,int y);
		PlayMapObjectNodeList* GetMapZone(int x,int y);
		void AddMapItem(PlayMapObject* obj);
		void RemoveMapItem(PlayMapObject* obj);
		void AddMiniNpc(int x,int y,const char* name,const char* strname,int directflyID,int showNpc);
		void AddMiniMonGen(const std::string& name,int x,int y,int time);
		void ParseOptions(std::string options);
		void SetOption(const char* option);
		bool CanSeeEachOther(PlayMapObject* obj1,PlayMapObject* obj2);
		void AddSafeArea(MapSafeArea& sa);
		void AddHoldArea(MapHoldArea* ha);
		void AddEventArea(MapEventArea& sa);
		void AddMapConn(DirectFlyNode* mc);

	};
}