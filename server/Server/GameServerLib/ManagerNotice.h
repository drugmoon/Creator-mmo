#pragma once
namespace OGSLib
{
	class GameServerLib;
	class ManagerNotice
	{
	public:
		typedef std::vector<std::string> NoticeVector;
	private:
		NoticeVector mNotices;
		GameServerLib* mGameServerLib;
		std::string mNoticeDefault;
	public:
		ManagerNotice(GameServerLib* gslib);
		virtual ~ManagerNotice(void);
	public:
		bool Init();
		void Dest();

		std::string& get_notice(int id);
		bool AddNotice(std::string notice);
		bool RemNotice(std::string notice);
		bool RemNotice(int id);
		std::string ShowNotices();
		bool ClearNotice();
		bool LoadNotice();
		void SaveNotice();
	};
}
