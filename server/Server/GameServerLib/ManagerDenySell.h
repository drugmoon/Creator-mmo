#pragma once
namespace OGSLib
{
	class GameServerLib;
	class ManagerDenySell
	{
	public:
		typedef std::vector<int> DenyVector;
		
	private:
		DenyVector mDenyChrs;
		GameServerLib* mGameServerLib;
	public:
		ManagerDenySell(GameServerLib* gslib);
		virtual ~ManagerDenySell(void);
	public:
		bool Init();
		void Dest();
		bool IsDeny(int type_id);
		bool LoadDeny();
		bool SaveDeny();
	};
}
