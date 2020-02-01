#pragma once
namespace OGSLib
{
	class GameServerLib;
	class Group;
	class ManagerGroup
	{
		friend class GameServerLib;
		int m_id_seed;
		GameServerLib* mGameServerLib;

		typedef std::map<int,Group*> GroupMap;
		GroupMap m_Groups;

		double mUpdateGM;
		double mUpdateGMDuration;
	public:
		ManagerGroup(GameServerLib* gslib);
		virtual ~ManagerGroup(void);

		int init();
		void dest();

		Group* CreateGroup(std::string map_name);
		Group* FindGroup(int id);
		Group* FindGroupWithNickName(std::string& nick_name);
		Group* FindTaskGroup(std::string map_name);

		void Update(double time);
	};
}