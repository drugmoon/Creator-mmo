#pragma once
namespace OGSLib
{
	class GameServerLib;
	class ManagerFilter
	{
	public:
		typedef std::vector<std::string> FilterWordVector;
	private:
		FilterWordVector mFilterWords;
		GameServerLib* mGameServerLib;
	public:
		ManagerFilter(GameServerLib* gslib);
		virtual ~ManagerFilter(void);
	public:
		bool Init();
		void Dest();

		bool checkFilter(std::string& msg);
		bool Filter(char* msg);
		bool FilterName(char* name);
		bool LoadFilterWord();
	};
}
