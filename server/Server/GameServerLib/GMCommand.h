#pragma once

namespace OGSLib
{
	class GameServerLib;
	class DomainPlay;
	class GMCommand
	{
		GameServerLib* mGameServerLib;
		std::map<std::string,int> mAdminList;
		std::map<std::string,std::string> mCommandList;
	public:
		GMCommand(GameServerLib* gslib);
		virtual ~GMCommand(void);
	public:
		void Start();
		void Stop();
		std::string DoSystemCommand(std::string command,int clv);
		std::string ConvertDoCommand(std::string command);
		void DoCommand(DomainPlay* src,std::string command);
		void LoadAdminList();
		void SaveAdminList();
		void LoadAdminCommand();
		bool CheckGMLevel(int clv,int need);
		int GetGMLevel(const char* name);
		void AddAdmin(const std::string& name,int lv);
	};
}