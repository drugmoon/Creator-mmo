#pragma once
namespace OGSLib
{
	class GameServerLib;
	class ManagerDeny
	{
	public:
		typedef std::vector<std::string> DenyVector;
	private:
		DenyVector mDenyChrs;
		DenyVector mDenyIPs;
		DenyVector mLimitChrs;
		GameServerLib* mGameServerLib;
	public:
		ManagerDeny(GameServerLib* gslib);
		virtual ~ManagerDeny(void);
	public:
		bool Init();
		void Dest();

		bool CheckChrDeny(const std::string& chrname);
		bool AddChrDeny(const std::string& chrname);
		bool RemChrDeny(const std::string& chrname);

		bool LoadDeny();
		bool SaveDeny();

		bool CheckDenyIP(const char* ip);
		bool AddDenyIP(const char* ip);
		bool RemDenyIP(const char* ip);
		bool LoadDenyIP();
		bool SaveDenyIP();

		bool CheckChrLimit(const std::string& account);
		bool AddChrLimit(const std::string& chrname);
		bool RemChrLimit(const std::string& chrname);
		bool LoadLimit();
		bool SaveLimit();
	};
}
