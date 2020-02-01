#include "stdafx.h"
#include "DomainGuild.h"
#include "GameSession.h"
#include "DomainData.h"
#include "GameServerLib.h"
#include "ManagerGuild.h"
#include "GuildGuild.h"
#include "DomainPlay.h"
#include "ManagerFilter.h"
#include "ScriptLua.h"
#include "PlayActiveObject.h"

namespace OGSLib
{
	DomainGuild::DomainGuild(GameSession* session)
	{
		mSession = session;
		mGSL = mSession->GetGameServerLib();
		mCharacter = mSession->GetCharacter();
		mGuildTitle = 0;
		mIsInit = false;
		mGetItemLogTick = 0;
		// 拉取红包日志时间
		mGetRedPacketLogTick = 0;
	}
	DomainGuild::~DomainGuild(void)
	{
	}
	const char* DomainGuild::Name()
	{
		return mSession->CharID();
	}
	const char* DomainGuild::SeedName()
	{
		return mSession->SeedID();
	}
	int DomainGuild::level()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->level();
		}
		return 0;
	}
	int DomainGuild::gmlevel()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->GetGMLevel();
		}
		return 0;
	}
	int DomainGuild::fight()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->FightPoint();
		}
		return 0;
	}
	int DomainGuild::guildpt()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->GuildPt();
		}
		return 0;
	}
	int DomainGuild::guildspt()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->GuildSpt();
		}
		return 0;
	} 
	int DomainGuild::groupid()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->GroupID();
		}
		return 0;
	} 
	int DomainGuild::lastloginout()
	{
		if( mSession && mSession->GetPlayer() )
		{
			return static_cast<int>(mSession->GetPlayer()->LastLogoutTime());
		}
		return 0;
	}
	int DomainGuild::gender()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->gender();
		}
		return 0;
	}
	int DomainGuild::job()
	{
		if( mSession->GetPlayer() )
		{
			return mSession->GetPlayer()->job();
		}
		return 0;
	}
	void DomainGuild::initGuildHandle()
	{
		mGuildID = mCharacter->mGuildID;
		mGuildName = mSession->GetCharacter()->mGuildName;
		Guild* gld = 0;
		if (mGuildID.empty())
		{
			if (!mGuildName.empty())
			{
				gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGuildName);
			}
		}
		else
		{
			gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(mGuildID);
		}
		if (gld)
		{
			GuildNode* gn = gld->GetMember(SeedName());
			if (gn == 0 || gld->deleted())
			{
				mGuildName.clear();
				mGuildTitle = 0;
				mGuildID.clear();
			}
			else
			{
				mGuildID = gld->SeedID();
				mGuildName = gld->GetName();
				mGuildTitle=gn->title;
				gld->SetMemberInfo(SeedName(),this);
			}
		}
		else
		{
			mGuildName.clear();
			mGuildTitle = 0;
			mGuildID.clear();
		}
		//没有行会扫描是否有人确认我的申请了
		if(mGuildID.empty())
		{
			const ManagerGuild::GuildMap& gm=mGSL->GetGuildManager()->GetGuilds();
			for(ManagerGuild::GuildMap::const_iterator p=gm.begin();p!=gm.end();++p)
			{
				if (p->second->deleted())
				{
					continue;
				}
				GuildNode* gn=p->second->GetMember(SeedName());
				if(gn)
				{
					mGuildName=p->second->GetName();
					mGuildTitle=gn->title;
					gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGuildName);
					mGuildID = gld->SeedID();
					p->second->SetMemberInfo(SeedName(),this);
					//mSession->GetPlayer()->AddStatus(40,86400,1,1,1);
					break;
				}
			}
		}
		//没有行会，也没有确认申请，通知所有行会更新我的申请人信息
		if(mGuildID.empty())
		{
			const ManagerGuild::GuildMap& gm=mGSL->GetGuildManager()->GetGuilds();
			for(ManagerGuild::GuildMap::const_iterator p=gm.begin();p!=gm.end();++p)
			{
				if (!p->second->deleted())	
					p->second->SetMemberInfo(SeedName(),this);
			}
		}
		SetGuildPt();
		SetGuildFight();
		SetGuildLastLoginOut();
		notifyGuildTitle();
		mIsInit = true;
	}
	void DomainGuild::destGuildHandle()
	{
		if( ! mGuildID.empty() ){
			Guild* gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(GuildID());
			if( gld ){
				gld->SetMemberInfo(SeedName(),this);
			}
		}else{
			const ManagerGuild::GuildMap& gm=mGSL->GetGuildManager()->GetGuilds();
			for(ManagerGuild::GuildMap::const_iterator p=gm.begin();p!=gm.end();++p){
				p->second->SetMemberInfo(SeedName(),0);
			}
		}
		SetGuildPt();
		SetGuildFight();
		SetGuildLastLoginOut();
		Save();
		SetGuildOnlineState(false);
	}

	void DomainGuild::Save()
	{
		strncpy_s(mCharacter->mGuildID,_countof(mCharacter->mGuildID),mGuildID.c_str(),255);
		mCharacter->mGuildID[255]=0;
		strncpy_s(mCharacter->mGuildName,_countof(mCharacter->mGuildName),mGuildName.c_str(),255);
		mCharacter->mGuildName[255]=0;
	}
	const std::string& DomainGuild::GetGuildName()
	{
		if(mIsInit)
		{
			return mGuildName;
		}
		else
		{
			mGuildID = mCharacter->mGuildID;
			mGuildName = mCharacter->mGuildName;
			Guild* gld = 0;
			if( !mGuildID.empty() )
			{
				gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(mGuildID);
				if( gld )
				{
					mGuildTitle = gld->GetMemberTitle(SeedName());
					mGuildName = gld->GetName();
					if( mGuildTitle == GUILD_TITLE_TYPE_OUT )
					{
						mGuildID.clear();
						mGuildName.clear();
					}
				}
				else
				{
					mGuildID.clear();
					mGuildName.clear();
				}
			}
			else
			{
				mGuildID.clear();
				mGuildName.clear();
			}
			return mGuildName;
		}
	}
	int DomainGuild::GuildChat(char* msg)
	{
		Guild* gld = 0;
		if( GuildID().empty() )
		{
			if (GuildName().empty())
			{
				return 0;
			}
			else
			{
				gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(GuildName());
			}
		}
		else
		{
			gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(GuildID());
		}
		if( mSession && mSession->GetPlayer() && mSession->GetPlayer()->GetGMHideTalk() )
		{
			return 0;
		}
		if( gld )
		{
			mGSL->GetFilterManager()->Filter(msg);
			gld->GuildChat(this,msg);
		}
		return 0;
	}
	int DomainGuild::GuildSize()
	{
		if( GuildName().empty() )
		{
			return 0;
		}
		Guild* gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(GuildName());
		if( gld )
		{
			return gld->GetMemberNumber();
		}
		return 0;
	}
	const std::string& DomainGuild::Var(const std::string& key)
	{
		if( ! GuildID().empty() )
		{
			Guild* gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(GuildID());
			if( gld )
			{
				return gld->Var(key);
			}
		}
		return mParamDefault;
	}
	void DomainGuild::Var(const std::string& key,const std::string& value)
	{
		if( ! GuildID().empty() )
		{
			Guild* gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(GuildID());
			if( gld )
			{
				return gld->Var(key,value);
			}
		}
	}
	const std::string& DomainGuild::LeaderName()
	{
		if(! mGuildID.empty() )
		{
			Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(mGuildID);
			if (guild)
			{
				return guild->GetGuildLeaderName();
			}
		}
		return mParamDefault;
	}
	void DomainGuild::GuildExe(const char* code)
	{
		Guild* gld = 0;
		if( !GuildID().empty() )
		{
			gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(GuildID());
		}
		else if ( !GuildName().empty() )
		{
			gld = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(GuildName());
		}
		if( gld )
		{
			gld->ExeCode(code);
		}
		return;
	}
	void DomainGuild::ExeCode(const char* code)
	{
		if( mSession->GetPlayer() )
		{
			mSession->GetPlayer()->ExeCode(code);
		}
	}
	int DomainGuild::CreateGuild(char* guild_name,int lv)
	{
		if( mSession->GetPlayer()->level() <= mGSL->mCreateGuildLimitLevel )
		{
			char temp[512];_snprintf_s(temp,_countof(temp),511,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_LEVEL_LIMIT,mGSL->mCreateGuildLimitLevel);temp[511]=0;
			mSession->GetListener()->notifyAlert(mSession,11,0,temp);
			return 0;
		}
		size_t gnl = strlen(guild_name);
		if( gnl <= 0 )
		{
			mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_EMPTY_NAME);
			return 0;
		}
		if( gnl > 16 )
		{
			mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_TOO_LONG_NAME);
			return 0;
		}
		UtilString::strrep(guild_name,"<",' ');
		UtilString::strrep(guild_name,">",' ');
		mGSL->GetFilterManager()->Filter(guild_name);
		if( mSession->AuthType() != 101 )
		{
			mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_AUTH_TYPE_ERROR);
			return 0;
		}
		if( ! GuildName().empty() ) 
		{
			mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_HAVE_GUILD);
			return 0;
		}
		std::string gn=guild_name;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(gn);
		if( guild )
		{
			mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_EXIST_NAME);
			return 0;
		}
		if( mSession->GetPlayer()->checkCreateGuild() )
		{
			guild = mSession->GetGameServerLib()->GetGuildManager()->CreateGuild(gn);
			if( guild )
			{
				const ManagerGuild::GuildMap& gm=mGSL->GetGuildManager()->GetGuilds();
				for(ManagerGuild::GuildMap::const_iterator p=gm.begin();p!=gm.end();++p)
				{
					p->second->RemPreMember(SeedName(),this);
					p->second->RemMember(SeedName(), 4);
				}
				guild->guild_level(1);
				mGuildTitle = guild->AddMember(Name(),SeedName(),GUILD_TITLE_TYPE_ADMIN,this);
				guild->SetMemberOnlineState(SeedName(),1,this);
				guild->SetMemberEnterTime(SeedName(),static_cast<int>(time(0)));
				mGuildName = gn;
				mGuildID = guild->SeedID();
				mSession->GetPlayer()->OnEnterGuild();
				// mSession->GetPlayer()->AddStatus(40, 86400, 1, 1, 1);////添加建行会后立即拥有行会buff
				mSession->GetPlayer()->ReallyCreateGuild();
				mSession->GetGameServerLib()->GetGuildManager()->SaveGuilds();
			}
			else
			{
				//mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_EXIST_NAME);
			}
		}
		else
		{
			//mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_CREATE_CONDITION_FAIL);
		}
		return 0;
	}

	void DomainGuild::notifyGuildInfoChange()
	{
		mSession->GetListener()->notifyGuildInfoChange(mSession);
	}

	void DomainGuild::NotifyGuildExpChange(int lv,int exp,int tarexp)
	{
		//mark
		//mSession->GetListener()->notifyGuildExpChange(mSession,mGuildName.c_str(),lv,exp,tarexp);
	}
	void DomainGuild::NotifyAlert(int pos,int flags,const char* msg)
	{
		mSession->GetListener()->notifyAlert(mSession,pos,flags,msg);
	}

	void DomainGuild::NotifyWar()
	{
		mSession->GetListener()->notifyGuildWar(mSession);
	}

	int DomainGuild::JoinGuild(char* guild_name,int flags)
	{
		if (mSession->GetPlayer()->checkJoinGuild())
		{
			if (mSession->AuthType() != 101)
			{
				mSession->GetListener()->notifyAlert(mSession, 11, 0, mGSL->mStringConsts.NOTIFY_GUILD_JOIN_AUTH_TYPE_ERROR);
				return 0;
			}

			if (mSession->GetPlayer()->level() < mGSL->mJoinGuildLimitLevel)
			{
				char temp[512]; _snprintf_s(temp, _countof(temp), 511, mGSL->mStringConsts.NOTIFY_GUILD_JOIN_LEVEL_LIMIT, mGSL->mJoinGuildLimitLevel); temp[511] = 0;
				mSession->GetListener()->notifyAlert(mSession, 11, 0, temp);
				return 0;
			}

			Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(guild_name);
			if (guild)
			{
				if (guild->deleted() == 1){ NotifyAlert(11, 0, mGSL->mStringConsts.NOTIFY_GUILD_JOIN_NOT_EXIST); return 0; }
				int maxnum = mGSL->mGuildMemberMax;
				if (mGSL->mGuildCheckPoint)
				{
					GuildLvInfo* gli = mGSL->GetGuildManager()->GetGuildLvInfo(guild->guild_level());
					if (gli)
					{
						maxnum = gli->mMaxNum;
					}
				}
				if (guild->GetMemberNumber() >= size_t(maxnum) && gmlevel() <= 0)
				{
					NotifyAlert(11, 0, mGSL->mStringConsts.NOTIFY_GUILD_FULL);
					return 0;
				}
				if (guild->GetPremem(SeedName()))
				{
					guild->RemPreMember(SeedName(), this);
					return 0;
				}
				if (guild->GetMember(SeedName()))
				{
					guild->RemMember(SeedName(), 4);
					return 0;
				}
				if (mSession->GetPlayer()->GetGMLevel() > 0)
				{
					mGuildTitle = guild->AddMember(Name(), SeedName(), GUILD_TITLE_TYPE_NORMAL, this);//gm直接进入行会
				}
				else
				{
					guild->AddPreMember(SeedName(), this);
				}
				mSession->GetPlayer()->OnEnterGuild();
			}
			else
			{
				mSession->GetListener()->notifyAlert(mSession, 11, 0, mGSL->mStringConsts.NOTIFY_GUILD_JOIN_NOT_EXIST);
			}
		}
		return 0;
	}
	int DomainGuild::LeaveGuild(const char* guild_name)
	{
		if( strlen(guild_name)==0 || (! mGuildName.empty()&&mGuildName.compare(guild_name)==0) )
		{
			Guild* guild = mGSL->GetGuildManager()->FindGuildByName(mGuildName);
			if( guild )
			{
				guild->RemMember( SeedName(), 3);
			}
		}
		
		{
			Guild* guild = mGSL->GetGuildManager()->FindGuildByName(guild_name);
			if(guild){
				guild->RemPreMember( SeedName(),this );
			}
		}
		return 0;
	}
	int DomainGuild::SetGuildOnlineState(bool online)
	{
		if( mGuildID.empty() )return 0;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(mGuildID);
		if( guild )
		{
			if( online )
			{
				guild->SetMemberOnlineState(SeedName(),1,this);
			}
			else
			{
				guild->SetMemberOnlineState(SeedName(),0,0);
			}
		}
		return 0;
	}
	int DomainGuild::SetGuildFight()
	{
		if( mGuildName.empty() )return 0;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGuildName);
		if( guild )
		{
			guild->SetMemberFight(SeedName(),fight());
		}
		return 0;
	}
	int DomainGuild::SetGuildPt()
	{
		if( mGuildID.empty() )return 0;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(mGuildID);
		if( guild )
		{
			guild->SetMemberGuildPt(SeedName(),guildpt(),guildspt());
		}
		return 0;
	}
	void DomainGuild::ClearGuildPt()
	{
		if( mGuildName.empty() )return;
		if (mSession && mSession->GetPlayer())
		{
			mSession->GetPlayer()->SetGuildPt(0);
		}
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGuildName);
		if( guild )
		{
			guild->SetMemberGuildPt(SeedName(),0,0);
		}
		return;
	}
	int DomainGuild::GetEnterTime()
	{
		if( mGuildName.empty() )return 0;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGuildName);
		if( guild )
		{
			return guild->GetMemberEnterTime(SeedName());
		}
		return 1;
	}
	int DomainGuild::SetGuildLastLoginOut()
	{
		if( mGuildID.empty() )return 0;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuild(mGuildID);
		if( guild )
		{
			guild->SetMemberLastLoginOut(SeedName(),this->lastloginout());
		}
		return 0;
	}
	int DomainGuild::SetGuildInfo(const std::string& guild_name,char* desp,char* notice)
	{
		if( mGuildID.empty() ) return 0;
		if( mGuildName != guild_name ) return 0;
		UtilString::strrep(desp,"<",' ');
		UtilString::strrep(desp,">",' ');
		mGSL->GetFilterManager()->Filter(desp);

		UtilString::strrep(notice,"<img",' ');
		mGSL->GetFilterManager()->Filter(notice);

		std::string tdesp = desp;
		std::string tnotice = notice;
		if( tdesp.length() > 256 ) {tdesp = tdesp.substr(0,256);}
		if( tnotice.length() > 256 ) {tnotice = tnotice.substr(0,256);}
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(guild_name);
		if( guild )
		{
			if( guild->GetMemberTitle(SeedName()) == GUILD_TITLE_TYPE_ADMIN )
			{
				guild->SetDesp(tdesp);
				guild->SetNotice(tnotice);
				notifyGuildTitle();
			}
		}
		return 0;
	}
	DomainPlay* DomainGuild::GetPlayer()
	{
		if(mSession && mSession->GetState()==GameSession::SessionState::SESSION_STATE_RUN){
			return mSession->GetPlayer();
		}
		return 0;
	}

	/*
	int DomainGuild::ChangeGuildMemberTitle(DomainPlay* play,const char* guild_name,const char* nick_name,int dir)
	{
		if( mGuildName != guild_name ) return 0;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(guild_name);

		if (!guild)
		{
			return 0;
		}

		std::string seed_name = guild->GetGuildPreSeed(nick_name);
		if (seed_name == "")
			seed_name = guild->GetGuildSeed(nick_name);

		GuildNode* mygn=guild->GetMember(SeedName());
		GuildNode* tagn=guild->GetMember(seed_name.c_str());
		if(0==mygn)
		{
			return 0;
		}

		int mytitle = mygn->title;
		if(tagn)
		{
			int thtitle = tagn->title;
			if( dir == 1000 )
			{
				// 禅让
				if( mytitle == GUILD_TITLE_TYPE_ADMIN )
				{
					OGSLib::GameSession* session = mGSL->FindSessionWithSeedID(seed_name);
					if (session && session->GetPlayer())
					{
						if (thtitle > GUILD_TITLE_TYPE_ENTERING)
						{
							// 职位互换
							guild->SetMemberTitle(SeedName(),thtitle);
							guild->SetMemberTitle(seed_name.c_str(),GUILD_TITLE_TYPE_ADMIN);
							mGSL->GetScriptEngine()->OnlyPlayer(session->GetPlayer(), "map.kinghome.SetGuildOwnerData");
							guild->NotifyGuildPresident(nick_name, 3);
						}
					}
					else
					{
						const std::string &name = nick_name;
						char temp[512];
						_snprintf_s(temp, _countof(temp), 511, mGSL->mStringConsts.NOTIFY_GROUP_MEMBER_OFFLINE, name.c_str());
						temp[511] = 0;
						play->AlertMessage(11, 1, temp);
					}
				}
				return 0;
			}

			if(mytitle > thtitle)
			{
				if( dir == 1 )
				{
					if( thtitle == GUILD_TITLE_TYPE_NORMAL )
					{
						if (guild->GetAdvNum() >= 3)
						{
							mSession->GetListener()->notifyAlert(mSession,11,0,mGSL->mStringConsts.NOTIFY_GUILD_ADV_FULL);
						}
						else
						{
							guild->SetMemberTitle(seed_name.c_str(), GUILD_TITLE_TYPE_ADV);
							guild->AddAdvNum();
							guild->NotifyGuildPresident(nick_name, 1);
						}
					}
				}

				if( dir == 0 )
				{
					if( thtitle == GUILD_TITLE_TYPE_ADV )
					{
						guild->SetMemberTitle(seed_name.c_str(),GUILD_TITLE_TYPE_NORMAL);
						guild->SubAdvNum();
						guild->NotifyGuildPresident(nick_name, 2);
					}
					if( thtitle == GUILD_TITLE_TYPE_NORMAL ){
						guild->RemMember(seed_name.c_str());
						GameSession* ts = mGSL->FindSessionWithCharID(nick_name);/////移出行会消buff
						if (ts && ts->GetPlayer())
						{
							ts->GetPlayer()->ClearStatus(40);
						}
					}
				}
			}
		}
		else
		{
			DomainGuild* dg=0;
			GameSession* gs=mGSL->FindSessionWithCharID(nick_name);
			if(gs)
			{
				dg=gs->GetDomainGuild();
			}
			if(dir==1)
			{
				tagn=guild->GetPremem(seed_name.c_str());
				if(tagn&&mytitle>=GUILD_TITLE_TYPE_ADV)
				{
					const ManagerGuild::GuildMap& gm=mGSL->GetGuildManager()->GetGuilds();
					for(ManagerGuild::GuildMap::const_iterator p=gm.begin();p!=gm.end();++p){
						p->second->RemPreMember(seed_name.c_str(),dg);
						p->second->RemMember(seed_name.c_str());
					}
					if (dg)
					{
						dg->mGuildID = guild->SeedID();
					}
					guild->AddMember(nick_name,seed_name.c_str(),GUILD_TITLE_TYPE_NORMAL,dg);
					guild->SetMemberEnterTime(seed_name,static_cast<int>(time(0)));
					if (gs && gs->GetPlayer())
					{
						gs->GetPlayer()->AddStatus(40,86400,1,1,1);
					}
				}
			}
			if(dir == 0)
			{
				if(mytitle>=GUILD_TITLE_TYPE_ADV)
				{
					mGuildID[0] = 0;
					guild->RemPreMember(seed_name.c_str(),dg);
				}
			}
		}
		return 0;
	}
	*/
	void DomainGuild::NotifyGuildChat(Guild* gld,DomainGuild* src,const char* msg)
	{
		mSession->GetListener()->notifyGuildChat(mSession,gld,src,msg);
	}
	void DomainGuild::onSetGuildEntering(Guild* guild,int entering)
	{
		mSession->GetListener()->notifyGuildItem(mSession,guild->GetName().c_str(),guild->GetMembers().size(),guild->GetDesp().c_str(),guild->master_level(),entering,guild->GetGuildLeaderName(),guild->guild_level());
	}

	void DomainGuild::onSetGuildAndTitle(const char* guild_name,const char* guild_seed, int title)
	{
		mGuildName=guild_name;
		mGuildID = guild_seed;
		mGuildTitle=title;
		notifyGuildTitle();
		DomainPlay * player = mSession->GetPlayer();
		if (player)
			player->GuildChangeBroadcast();
	}

	void DomainGuild::onLeaveGuildRemoveBuff()
	{
		if (mSession && mSession->GetPlayer())
		{
			if (mSession->GetPlayer()->IsStatus(PlayActiveObject::STATUS_TYPE_GUILD))
			{
				mSession->GetPlayer()->ClearStatus(PlayActiveObject::STATUS_TYPE_GUILD);
			}
		}
	}

	void DomainGuild::notifyGuildTitle()
	{
		mSession->GetListener()->notifyGuildtitle(mSession, mGuildName.c_str(),mGuildTitle);
	}

	void DomainGuild::notifyGuildMemberChange()
	{
		mSession->GetListener()->notifyGuildMemberChange(mSession);
	}

	void DomainGuild::onGetGuildRedPacketLog()
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		if (now - mGetRedPacketLogTick < 0.3)
		{
			return;
		}
		mGetRedPacketLogTick = now;


		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(GuildName());
		if (guild)
		{
			std::list<GuildRedPacketRecord>& list = guild->GetRedPacketLogList();
			mSession->GetListener()->notifyGuildRedPacketLogRes(mSession, list);
		}
	}

	void DomainGuild::onGetGuildItemLog()
	{
		double now = mGSL->GetTimerFix()->Elapsed();
		if (now - mGetItemLogTick < 0.3)
		{
			return;
		}
		mGetItemLogTick = now;

		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(GuildName());
		if (guild)
		{
			std::list<GuildItemOpRecord>& list = guild->GetGuildItemOpLogList();
			mSession->GetListener()->notifyGuildItemLogRes(mSession, list);
		}
	}

	void DomainGuild::notifyGuildRedPacketLog(GuildRedPacketRecord * pRecord)
	{
		mSession->GetListener()->notifyGuildRedPacketLog(mSession, pRecord);
	}

	void DomainGuild::GetGuildWarState(std::string& seed_id, int &war_state, int &start_time)
	{
		if( mGuildName.empty() )return;
		Guild* guild = mSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGuildName);
		if( guild )
		{
			return guild->GuildWarState(seed_id, war_state, start_time);
		}
	}

	void DomainGuild::onLeaveGuild(int reason)
	{
		if (this->GetPlayer())
		{
			mGSL->GetScriptEngine()->PlayerInt(this->GetPlayer(), reason, 0, 0, 0, "player.onLeaveGuild");
			onLeaveGuildRemoveBuff();
			onSetGuildAndTitle("","",0);
		}
	}

	void DomainGuild::onEnterGuild()
	{
		if (this->GetPlayer())
		{
			mGSL->GetScriptEngine()->OnlyPlayer(this->GetPlayer(), "player.onEnterGuild");
		}
	}
}

LEUD::PoolMemory<OGSLib::DomainGuild>	LEUD::PoolMemoryBase<OGSLib::DomainGuild>::memPool("DomainGuild");
