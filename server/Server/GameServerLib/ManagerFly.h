#pragma once
namespace OGSLib
{
	class DirectFlyNode
	{
	public:
		int mNodeID;
		std::string mNodeName;

		std::string mStartMapID;
		int mStartX;
		int mStartY;

		int mType;
		int mIsShow;

		std::string mTargetMapID;
		int mTargetX;
		int mTargetY;
		int mTargetSize;
		int mMonLv;
		int mClothId;
	};
	class GameServerLib;
	class ManagerFly
	{
		friend class ManagerMap;
	public:
		typedef std::map<int,DirectFlyNode> DirectFlyNodeMap;
		typedef std::map< std::string,std::vector<int> > MapConnNodeIDMap;
	private:
		DirectFlyNodeMap mDirectFlyNodes;
		MapConnNodeIDMap mMapConnNodeIDs;
		GameServerLib* mGameServerLib;
	public:
		ManagerFly(GameServerLib* gslib);
		virtual ~ManagerFly(void);
	public:
		bool Init();
		void Dest();
		DirectFlyNode* FindNode(int nodeId);
		DirectFlyNodeMap& GetmDirectFlyNodes(){return mDirectFlyNodes;}
		MapConnNodeIDMap& GetMapConnNodes(){return mMapConnNodeIDs;}
		//void SetNode(int nodeId,const char* map,int x,int y,const char* target);
	private:
		bool LoadData();
	};
}
