#include "StdAfx.h"
#include "GameSession.h"
#include "DomainData.h"
#include "DomainPlay.h"
#include "PlayMap.h"
#include "GameServerLib.h"
#include "ManagerMap.h"
#include "ScriptLua.h"
#include "DomainGroup.h"
#include "DomainGuild.h"
#include "DomainCouple.h"
namespace OGSLib
{
	GameSession::GameSession(GameServerLib* gameserverlib,const char* loginID,const char* charID,const char* seedID,int AutyType,int newYouKe,const char* client_ip)
	{
		mState = SESSION_STATE_NULL;
		mGameServerLib = gameserverlib;
		strncpy_s(mLoginID,_countof(mLoginID),loginID,255);
		mLoginID[255]=0;
		mLoginID[255]=0;
		strncpy_s(mCharID,_countof(mCharID),charID,255);
		mCharID[255]=0;
		mCharID[255]=0;
		mAuthType = AutyType;
		mNewYouKe = newYouKe;
		strncpy_s(mClientIP,_countof(mClientIP),client_ip,255);
		mClientIP[255]=0;
		mClientIP[255]=0;
		strncpy_s(mSeedID,_countof(mSeedID),seedID,255);
		mSeedID[255]=0;

		mCharacter = 0;
		mPlayer = 0;
		mGroup=0;
		mGuild=0;
		//mCouple=0;
		mState = SESSION_STATE_LOADING;
	}
	void GameSession::SetCharacter(DomainData* chr)
	{
		if( mState != SESSION_STATE_LOADING ) return;
		mCharacter = chr;
	}
	void GameSession::CreatePlayer()
	{
		if( mState != SESSION_STATE_LOADING ) return;
		mPlayer = new DomainPlay(this);
		mGroup = new DomainGroup(this);
		mGuild = new DomainGuild(this);
		//mCouple = new DomainCouple(this);
	}
	GameSession::~GameSession(void)
	{
		delete mCharacter;mCharacter = 0;
		delete mPlayer;mPlayer = 0;
		delete mGroup;mGroup = 0;
		delete mGuild;mGuild = 0;
		//delete mCouple;mCouple = 0;
	}
	void GameSession::EnterGame()
	{
		if( mState != SESSION_STATE_LOADING ) return;
		mCharacter->Load();
		mPlayer->initPlayer();
		mGroup->initGroupHandle();
		mGuild->initGuildHandle();
		//mCouple->initCoupleHandle();
		mState = SESSION_STATE_RUN;
	}
	void GameSession::Update(double time)
	{
		if( mState != SESSION_STATE_RUN ) return ;
		mPlayer->Update(time);
	}
	void GameSession::SaveSession()
	{
		if( mState == SESSION_STATE_RUN )
		{
			mCharacter->Save(false);
			mPlayer->Save();
			mGroup->Save();
			mGuild->Save();
			GetGameServerLib()->SaveSession(this,false);
		}
	}
	void GameSession::BreakNet(const std::string& msg)
	{
		GetListener()->notifySessionClosed(this,msg);
	}
	void GameSession::BreakNetAndEndSession(const std::string& msg)
	{
		BreakNet(msg);
		EndSession();
	}
	void GameSession::EndSession()
	{
		if( mState != SESSION_STATE_RUN ) 
		{
			if( mState != SESSION_STATE_SAVEING )
			{
				mState = SESSION_STATE_END;
			}
			return;
		}
		mCharacter->Save(true);
		mPlayer->destPlayer();
		mGroup->destGroupHandle();
		mGuild->destGuildHandle();
		//mCouple->destCoupleHandle();
		mState = SESSION_STATE_SAVEING;
		GetGameServerLib()->SaveSession(this,true);
	}
	void GameSession::Destroy()
	{
		mState = SESSION_STATE_END;
	}

}
LEUD::PoolMemory<OGSLib::GameSession>	LEUD::PoolMemoryBase<OGSLib::GameSession>::memPool("GameSession");
