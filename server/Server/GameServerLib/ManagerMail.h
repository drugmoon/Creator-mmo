#pragma once
#include "leudgrid/MemoryPool.h"
namespace OGSLib
{

	class GameServerLib;
	class DomainPlay;

	class Mail : public LEUD::PoolMemoryBase< Mail >
	{
	public:
		std::string mSeedName;
		std::string mTitle;
		std::string mContent;
		LEUD_INT64 mID;
		int mDate;
		int mDuration;
		int mIsOpen;
		int mIsReceive;
		std::map<int,int> mItems;
		int mType;
		int mNeedLv;
		int mGS;
		std::string mOwners;
		int mDataStatus;//0Õý³£1´ýÉ¾
	public:
		Mail();
	};

	class ManagerMail
	{
	public:
		typedef std::vector<Mail *> MailList;
	private:
		MailList mMails;
		GameServerLib* mGameServerLib;
		time_t mSaveTick;
		int mMailGen;
	public:
		ManagerMail(GameServerLib* gslib);
		virtual ~ManagerMail(void);
	
		int init();
		void dest(bool save=true);
		void update();
		bool OnLoadMails(int gen,const std::vector<OGSLib::Mail*>& mailVector);
		LEUD_INT64 GenMailID();
		bool SaveMails();
		MailList& GetMails(){return mMails;}
		void ClearUselessMails();
		void FilterCharacter(std::string& str);
		int PostMailToOnePlayer(std::string& title,std::string& content,std::string& seedName,const std::map<int,int>& items,int gs=0);
		int PostMailToMultiplePlayers(std::string& title,std::string& content,const std::vector<std::string>& seedNames,const std::map<int,int>& items,int gs=0);
		int PostMailToAllPlayers(std::string& title,std::string& content,const std::map<int,int>& items,int gs=0, int verifyMail = 0);
	};
}
