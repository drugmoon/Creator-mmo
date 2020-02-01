#pragma once

namespace OGSLib
{
	class Couple;
	class GameServerLib;
	class ManagerCouple
	{
	public:
		typedef std::map<int,Couple*> CoupleMap;
	private:
		CoupleMap mCouples;
		GameServerLib* mGameServerLib;
		time_t mSaveTick;
	public:
		ManagerCouple(GameServerLib* gslib);
		virtual ~ManagerCouple(void);

		int init();
		void dest(bool save=true);
		void update();

		Couple* CreateCouple();
		Couple* FindCouple(int coupleID);
		void DeleteCouple(int couple_id);
		const CoupleMap& GetCouples()const{return mCouples;}
		int GetCoupleServerID();

		bool LoadCouples();
		bool OnLoadCouples(const OGSLib::ManagerCouple::CoupleMap& couple_map);
		bool SaveCouples();
	};
}