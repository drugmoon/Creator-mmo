#include "StdAfx.h"
#include "ManagerMail.h"
#include "GameServerLib.h"
#include "DomainPlay.h"
#include "ManagerItemDef.h"
#include "SubItem.h"
#include "GameSession.h"

namespace OGSLib
{
	Mail::Mail()
	{
		mDate = 0;
		mType = 0;
		mDataStatus = 0;
		mDuration=60*60*24*15;
	}

	ManagerMail::ManagerMail(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
		mMailGen = 0;
	}

	ManagerMail::~ManagerMail(void)
	{
	}
	int ManagerMail::init()
	{
		mSaveTick = time(0);
		mGameServerLib->GetListener()->LoadMails(0);
		return 0;
	}
	void ManagerMail::dest(bool save)
	{
		if( save )
		{
			SaveMails();
		}
		for(auto pos = mMails.begin(); pos != mMails.end(); ++pos)
		{
            delete *pos;
		}
	}
	void ManagerMail::update()
	{
		if( mSaveTick + 30*60 < time(0) )
		{
			mSaveTick = time(0);
			SaveMails();
		}
	}
	bool ManagerMail::OnLoadMails(int gen,const std::vector<OGSLib::Mail*>& mailVector)
	{
		for( std::vector<OGSLib::Mail*>::const_iterator pos = mailVector.begin();
			pos != mailVector.end();
			++pos )
		{
			mMails.push_back(*pos);
			//mGameServerLib->LogMail(mail->mSeedName.c_str(),mail->mID,mail->mTitle.c_str(),mail->mDate);
		}
		mMailGen = 0;
		return true;
	}
	bool ManagerMail::SaveMails()
	{
		mGameServerLib->GetListener()->SaveMails(*this,0);
		return true;
	}
	void ManagerMail::ClearUselessMails()
    {
        for (auto pos = mMails.begin(); pos != mMails.end(); )
		{
			if((*pos)->mDataStatus==1)
			{
                delete *pos;
                pos = mMails.erase(pos);
            } else {
                ++pos;
            }
		}
	}
	void ManagerMail::FilterCharacter(std::string& str)
	{
		std::string searchString("&nbsp;");
		std::string searchString1("'");
		std::string searchString2("\\");
		std::string replaceString(" ");
		std::string replaceString1("");
		std::string::size_type pos = 0;
		while((pos = str.find(searchString,pos)) != std::string::npos)
		{
			str.replace(pos,searchString.size(),replaceString);
			pos++;
		}
		while((pos = str.find(searchString1,pos)) != std::string::npos)
		{
			str.replace(pos,searchString1.size(),replaceString1);
			pos++;
		}
		while((pos = str.find(searchString2,pos)) != std::string::npos)
		{
			str.replace(pos,searchString2.size(),replaceString1);
			pos++;
		}
	}

	int ManagerMail::PostMailToOnePlayer(std::string& title,std::string& content,std::string& seedName,const std::map<int,int>& items,int gs)
	{
		DomainPlay *player = NULL;
		if (GameSession *gs = mGameServerLib->FindSessionWithSeedID(seedName)) {
			player = gs->GetPlayer();
		}

		//if(player)
		//{
			FilterCharacter(title);
			FilterCharacter(content);
			if (title.length() > 255 || content.length() > 1024)
			{
				if(player)
				{
					player->AlertMessage(1,0,mGameServerLib->mStringConsts.NOTIFY_MAIL_EXCEED_MAX_LENGTH);
				}
				return -1;
			}
			Mail* mail = new Mail;
			mail->mID = GenMailID(); 
			mail->mTitle = title;
			mail->mContent = content;
			mail->mDate = int(time(0));
			mail->mItems = items;
			mail->mOwners = seedName;
			mail->mGS = gs;
			mMails.push_back(mail);
			if(player)
			{
				player->LoadPostMail();
			}
			return 0;
		//}
		//return -1;
	}
	int ManagerMail::PostMailToMultiplePlayers(std::string& title,std::string& content,const std::vector<std::string>& seedNames,const std::map<int,int>& items,int gs)
	{
		FilterCharacter(title);
		FilterCharacter(content);
		if (title.length() > 255 || content.length() > 1024)
		{
			//AlertMessage(1,0,mGameServerLib->mStringConsts.NOTIFY_MAIL_EXCEED_MAX_LENGTH);
			return -1;
		}
		for (std::vector<std::string>::const_iterator pos = seedNames.begin();
			pos != seedNames.end();
			++pos)
		{
			Mail* mail = new Mail;
			mail->mID = GenMailID(); 
			mail->mTitle = title;
			mail->mContent = content;
			mail->mDate = int(time(0));
			mail->mOwners = *pos;
            mail->mItems = items;
			mail->mGS = gs;
            mMails.push_back(mail);
			//mGameServerLib->LogMail(mail->mSeedName.c_str(),mail->mDate,mail->mTitle.c_str(),mail->mDate);
		    GameSession* gameSession = mGameServerLib->FindSessionWithSeedID(*pos);
			if(gameSession)
			{
				gameSession->GetPlayer()->LoadPostMail();
			}
		}
		return 0;
	}
	int ManagerMail::PostMailToAllPlayers(std::string& title,std::string& content,const std::map<int,int>& items,int gs, int verifyMail)
	{
		FilterCharacter(title);
		FilterCharacter(content);
		if (title.length() > 255 || content.length() > 1024)
		{
			//AlertMessage(1,0,mGameServerLib->mStringConsts.NOTIFY_MAIL_EXCEED_MAX_LENGTH);
			return -1;
		}
		Mail* mail = new Mail;
		mail->mID = GenMailID(); 
		mail->mTitle = title;
		mail->mContent = content;
		mail->mDate = int(time(0));
		mail->mDuration=60*60*24*7;
		mail->mItems = items;
        mail->mOwners = "all";
		if(verifyMail == 1)
		{
			mail->mOwners = "verify";
		}
		mail->mGS = gs;
        mMails.push_back(mail);
		//mGameServerLib->LogMail("",mail->mID,mail->mTitle.c_str(),mail->mDate);
		for (std::vector<GameSession*>::iterator pos = mGameServerLib->GetAllSessions().begin();
			pos != mGameServerLib->GetAllSessions().end();
			++pos)
		{
			(*pos)->GetPlayer()->LoadPostMail();
		}
		return 0;
	}

	LEUD_INT64 ManagerMail::GenMailID()
	{
		++ mMailGen;
		LEUD_INT64 id = time(0) * 1000000;
		id += (mMailGen % 1000000);
		return id;
		//return time(0) * 1000000 + mMailGen % 1000000;
	}
}
LEUD::PoolMemory<OGSLib::Mail>	LEUD::PoolMemoryBase<OGSLib::Mail>::memPool("Mail");