#include "stdafx.h"
#include "GuildGuild.h"
#include "DomainPlay.h"
#include "DomainGuild.h"
#include "GameServerLib.h"
#include "GameSession.h"
#include "ManagerMail.h"
#include "ManagerItemDef.h"
#include "ConstsString.h"
#include "ManagerGuild.h"
#include "ScriptLua.h"
#include "SubItem.h"

namespace OGSLib
{
	Guild::Guild(void)
	{
		mGameServerLib=0;
		mGuildMasterLevel = 0;
		mGuildLevel = 1;
		mGuildMemberMaxNum = 0;
		mGuildExp = 0;
		mDeleted = 0;
		mGuildAdvNum = 0;
		mGuildViceAdminNum = 0;
		mMaxMemberCount = 50;
	}
	Guild::~Guild(void)
	{
	}

	void Guild::update()
	{
		if (mRedPacketInfo.mRemainValue > 0 && GetRedPacketRemainingTime() < 0)
		{
			// 红包过期
			// 退回
			SendBackRedPacket();
		}

		UpdateGuildWar();
	}

	int Guild::GetNextTitle(int title)
	{
		if( title == GUILD_TITLE_TYPE_OUT )
		{
			return GUILD_TITLE_TYPE_OUT;
		}
		if( title == GUILD_TITLE_TYPE_ENTERING )
		{
			return GUILD_TITLE_TYPE_NORMAL;
		}
		if( title == GUILD_TITLE_TYPE_NORMAL )
		{
			return GUILD_TITLE_TYPE_ADV;
		}
		if( title == GUILD_TITLE_TYPE_ADV )
		{
			return GUILD_TITLE_TYPE_VICEADMIN;
		}
		if( title == GUILD_TITLE_TYPE_VICEADMIN )
		{
			return GUILD_TITLE_TYPE_ADMIN;
		}
		if( title == GUILD_TITLE_TYPE_ADMIN )
		{
			return GUILD_TITLE_TYPE_ADMIN;
		}
		return GUILD_TITLE_TYPE_OUT;
	}
	int Guild::GetPrevTitle(int title)
	{
		if( title == GUILD_TITLE_TYPE_OUT )
		{
			return GUILD_TITLE_TYPE_OUT;
		}
		if( title == GUILD_TITLE_TYPE_ENTERING )
		{
			return GUILD_TITLE_TYPE_OUT;
		}
		if( title == GUILD_TITLE_TYPE_NORMAL )
		{
			return GUILD_TITLE_TYPE_ENTERING;
		}
		if( title == GUILD_TITLE_TYPE_ADV )
		{
			return GUILD_TITLE_TYPE_NORMAL;
		}
		if( title == GUILD_TITLE_TYPE_VICEADMIN )
		{
			return GUILD_TITLE_TYPE_ADV;
		}
		if( title == GUILD_TITLE_TYPE_VICEADMIN )
		{
			return GUILD_TITLE_TYPE_ADMIN;
		}
		return GUILD_TITLE_TYPE_OUT;
	}
	int Guild::AddMember(const char* nick_name,const char* seed_name,int title,DomainGuild* player)
	{
		//printf("GuildGuild: AddMember guilds %s %s %d  %d  %d  %d \n",seed_name.c_str(),nick_name.c_str(),title, lastout, guildpt, spt);
		if( 0 == GetMember(seed_name) )
		{
			GuildNode gn;
			gn.nick_name = nick_name;
			gn.seed_name = seed_name;
			gn.title = title;
			if (title == GUILD_TITLE_TYPE_ADMIN && (HaveAdmin() != "" && HaveAdmin() != gn.seed_name ))
			{
				gn.title = GUILD_TITLE_TYPE_NORMAL;
			}
			mMembers[seed_name] = gn;
			if(player)
			{
				SetMemberInfo(seed_name,player);
				player->onSetGuildAndTitle(mName.c_str(), mSeedID.c_str(), gn.title);
				MakeAdmin();
			}
			return gn.title;
		}
		return GUILD_TITLE_TYPE_OUT;
	}

	void Guild::SetGuildName(const std::string& seed_name)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			if( pos->second.player )
			{
				DomainGuild* player = pos->second.player;
				if( player )
				{
					pos->second.nick_name = player->Name();
				}
			}
		}
	}
	int Guild::RemMember(const std::string& seed_name, int reason)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			if (mRedPacketInfo.mSenderSeedName == seed_name)
			{
				SendBackRedPacket();
			}

			if( pos->second.player )
			{
				DomainGuild* player = pos->second.player;
				player->ClearGuildPt();
				if (pos->second.title == GUILD_TITLE_TYPE_ADV)///如果是副会长退出行会
				{
					SubAdvNum();
				}
				if (pos->second.title == GUILD_TITLE_TYPE_VICEADMIN)
				{
					SubViceAdminNum();
				}
				MakeAdmin();
				player->onLeaveGuild(reason);
			}

			mMembers.erase(pos);
		}
		
		return GUILD_TITLE_TYPE_OUT;
	}
	int Guild::add_guildpt(const std::string& name,int add)
	{
		if( add > 0 )
		{
			mGuildExp += add;
		}
		std::string seed_name = GetGuildSeed(name);
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			if( pos->second.player )
			{
				pos->second.guildpt = pos->second.player->guildpt();
			}
		}
		return mGuildExp;
	}
	const std::string& Guild::Var(const std::string& key)
	{
		std::map<std::string,std::string>::iterator pos = mParams.find(key);
		if( pos != mParams.end() )
		{
			return pos->second;
		}
		return mParamDefault;
	}
	void Guild::Var(const std::string& key,const std::string& value)
	{
		if( key.length() )
		{
			mParams[key] = value;
		}
	}
	const std::string Guild::HaveAdmin()
	{
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.title == GUILD_TITLE_TYPE_ADMIN )
			{
				return pos->second.seed_name;
			}
		}
		return "";
	}
	int Guild::MakeAdmin()
	{
		bool ha=false;
		for( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.title == GUILD_TITLE_TYPE_ADMIN )
			{
				ha = true;
			}
		}
		if( ! ha )
		{
			for( MemberMap::iterator pos = mMembers.begin();
				pos != mMembers.end();
				++ pos )
			{
				if( pos->second.title == GUILD_TITLE_TYPE_VICEADMIN )
				{
					SetMemberTitle(pos->first,GUILD_TITLE_TYPE_ADMIN);
					ha = true;
					break;
				}
			}

			for( MemberMap::iterator pos = mMembers.begin();
				pos != mMembers.end();
				++ pos )
			{
				if( pos->second.title == GUILD_TITLE_TYPE_ADV )
				{
					SetMemberTitle(pos->first,GUILD_TITLE_TYPE_ADMIN);
					ha = true;
					break;
				}
			}
			if( ! ha )
			{
				for( MemberMap::iterator pos = mMembers.begin();
					pos != mMembers.end();
					++ pos )
				{
					if( pos->second.title == GUILD_TITLE_TYPE_NORMAL )
					{
						SetMemberTitle(pos->first,GUILD_TITLE_TYPE_ADMIN);
						ha = true;
						break;
					}
				}
			}
			if( ! ha )
			{
				for( MemberMap::iterator pos = mMembers.begin();
					pos != mMembers.end();
					++ pos )
				{
					SetMemberTitle(pos->first,GUILD_TITLE_TYPE_ADMIN);
					ha = true;
					break;
				}
			}
		}
		return 0;
	}
	void Guild::DismissGuild()
	{
		SendBackRedPacket();
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.player )
			{
				pos->second.player->onLeaveGuild(1);
			}
		}
		mDeleted = 1;
		mMembers.clear();
		ClearRelGuildWar();
		ClearGuildWar();
	}
	const std::string& Guild::GetGuildLeaderName()
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.title==GUILD_TITLE_TYPE_ADMIN )
			{
				return pos->second.nick_name;
			}
		}
		return mParamDefault;
	}
	const std::string& Guild::GetGuildLeaderSeed()
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.title==GUILD_TITLE_TYPE_ADMIN )
			{
				return pos->second.seed_name;
			}
		}
		return mParamDefault;
	}
	const std::string& Guild::GetGuildSeed(const std::string& nick_name)
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( pos->second.nick_name == nick_name )
			{
				return pos->second.seed_name;
			}
		}
		return mParamDefault;
	}
	const std::string& Guild::GetGuildPreSeed(const std::string& nick_name)
	{
		for ( MemberMap::iterator pos = mPremems.begin();
			pos != mPremems.end();
			++ pos )
		{
			if( pos->second.nick_name == nick_name )
			{
				return pos->second.seed_name;
			}
		}
		return mParamDefault;
	}
	int Guild::GetMemberTitle(const std::string& seed_name)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			return pos->second.title;
		}
		pos = mPremems.find(seed_name);
		if (pos != mPremems.end())
		{
			return pos->second.title;
		}

		return GUILD_TITLE_TYPE_OUT;
	}
	void Guild::NotifyGuildPresident(const std::string& nick_name, int flag)
	{
		char msg[512];
		if (flag==1)///提升为副会长
		{
			_snprintf_s(msg, _countof(msg), 511, mGameServerLib->mStringConsts.NOTIFY_GUILD_ADD_PRESIDENT, nick_name.c_str());
			msg[511] = 0;
		}
		else if (flag==2)///降级
		{
			_snprintf_s(msg, _countof(msg), 511, mGameServerLib->mStringConsts.NOTIFY_GUILD_SUB_PRESIDENT, nick_name.c_str());
			msg[511] = 0;
		}
		else if (flag == 3)///提升为行会会长
		{
			_snprintf_s(msg, _countof(msg), 511, mGameServerLib->mStringConsts.NOTIFY_GUILD_BECOME_ADMIN, nick_name.c_str());
			msg[511] = 0;
		}
		for (MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++pos)
		{
			DomainGuild* player = pos->second.player;
			if (player)
			{
				DomainPlay* play = player->GetPlayer();
				if (play)
				{
					play->AlertMessage(11, 0, msg);
				}
			}
		}
	}

	// 一键同意
	int Guild::AgreeAll()
	{
		int nLeft = 0;
		int maxNum = 50;
		GuildLvInfo* gli = mGameServerLib->GetGuildManager()->GetGuildLvInfo(guild_level());
		if( gli )
		{
			maxNum = gli->mMaxNum;
		}

		if (mMembers.size() < maxNum)
			nLeft = maxNum - mMembers.size();

		if (nLeft <= 0)
			return 0;

		std::vector<GuildNode> vecAgree;
		MemberMap::iterator pos = mPremems.begin();
		
		while (pos != mPremems.end())
		{
			if (vecAgree.size() < nLeft)
			{
				vecAgree.push_back(pos->second);
			}
			else
			{
				for (size_t i = 0; i < vecAgree.size(); ++ i)
				{
					if (pos->second.entertime < vecAgree[i].entertime)
					{
						vecAgree[i] = pos->second;
						break;
					}
				}
			}
			++ pos;
		}

		for (size_t i = 0; i < vecAgree.size(); ++ i)
		{
			mPremems.erase(vecAgree[i].seed_name);

			vecAgree[i].title = GUILD_TITLE_TYPE_NORMAL;
			vecAgree[i].entertime = time(0);
			
			if( vecAgree[i].player )
			{
				vecAgree[i].player->onSetGuildAndTitle(mName.c_str(), mSeedID.c_str(), GUILD_TITLE_TYPE_NORMAL);
				vecAgree[i].player->onEnterGuild();
			}
			mMembers[vecAgree[i].seed_name] = vecAgree[i];
		}

		if (vecAgree.size() > 0)
		{
			//清理其他帮会的申请信息
			DomainGuild* dg=0;
			GameSession* gs=0;
			const ManagerGuild::GuildMap& gm= mGameServerLib->GetGuildManager()->GetGuilds();
			for(ManagerGuild::GuildMap::const_iterator p=gm.begin();p!=gm.end();++p){
				for (size_t i = 0; i < vecAgree.size(); ++ i)
				{
					if (p->second != this)
					{
						gs=mGameServerLib->FindSessionWithSeedID(vecAgree[i].seed_name.c_str());
						dg=0;
						if(gs)
						{
							dg=gs->GetDomainGuild();
						}

						p->second->RemPreMember(vecAgree[i].seed_name.c_str(),dg);
						p->second->RemMember(vecAgree[i].seed_name.c_str(),5);
					}
				}
			}
		}

		return vecAgree.size();
	}

	// 一键拒绝
	int Guild::RefuseAll()
	{
		int count = 0;
		MemberMap::iterator pos = mPremems.begin();
		while (pos != mPremems.end())
		{
			count ++ ;
			pos = mPremems.erase(pos);
		}

		return count;
	}

	int Guild::SetMemberTitle(const std::string& seed_name,int title)
	{
		MemberMap::iterator pos = mPremems.find(seed_name);
		if (pos != mPremems.end())
		{
			if (title < GUILD_TITLE_TYPE_ENTERING)
			{
				mPremems.erase(pos);
				return GUILD_TITLE_TYPE_OUT;
			}
			else if (title > GUILD_TITLE_TYPE_ENTERING)
			{
				pos->second.entertime = time(0);
				mMembers[seed_name] = pos->second;
				mPremems.erase(pos);

				//清理其他帮会的申请信息
				DomainGuild* dg=0;
				GameSession* gs=mGameServerLib->FindSessionWithSeedID(seed_name);
				if(gs)
				{
					dg=gs->GetDomainGuild();
				}

				const ManagerGuild::GuildMap& gm= mGameServerLib->GetGuildManager()->GetGuilds();
				for(ManagerGuild::GuildMap::const_iterator p=gm.begin();p!=gm.end();++p){
					if (p->second != this)
					{
						p->second->RemPreMember(seed_name.c_str(),dg);
						p->second->RemMember(seed_name.c_str(),5);
					}
				}
			}
			else
				return GUILD_TITLE_TYPE_OUT;
		}

		pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			int title_old = pos->second.title;
			if (title == GUILD_TITLE_TYPE_ADV)
				AddAdvNum();

			if (title == GUILD_TITLE_TYPE_VICEADMIN)
				AddViceAdminNum();

			if (pos->second.title == GUILD_TITLE_TYPE_ADV)
				SubAdvNum();

			if (pos->second.title == GUILD_TITLE_TYPE_VICEADMIN)
				SubViceAdminNum();

			pos->second.title = title;

			if( pos->second.player )
			{
				pos->second.player->onSetGuildAndTitle(mName.c_str(), mSeedID.c_str(), title);
			}

			if (title_old == GUILD_TITLE_TYPE_ENTERING && pos->second.title > GUILD_TITLE_TYPE_ENTERING)
			{
				if( pos->second.player )
					pos->second.player->onEnterGuild();
			}

			if (pos->second.title == GUILD_TITLE_TYPE_OUT)
			{
				RemMember(seed_name, 2);
			}

			return title;
		}
		return GUILD_TITLE_TYPE_OUT;
	}

	int Guild::GetMemberOnlineState(const std::string& seed_name)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			return pos->second.online_state;
		}
		return 0;
	}

	int Guild::SetMemberOnlineState(const std::string& seed_name,int online_state,DomainGuild* player)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.online_state = online_state;
			pos->second.player = player;
			if( pos->second.title == GUILD_TITLE_TYPE_ADMIN && player )
			{
				mGuildMasterLevel=player->level();
			}
			return pos->second.online_state;
		}
		return 0;
	}
	int Guild::SetMemberFight(const std::string& seed_name,int fight)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.fight = fight;
		}
		return 0;
	}
	int Guild::SetMemberGuildPt(const std::string& seed_name,int guildpt,int spt)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.guildpt = guildpt;
			if( pos->second.player )
			{
				pos->second.guildpt = pos->second.player->guildpt();
			}
			pos->second.guildspt = spt;
		}
		return 0;
	}
	int Guild::SetMemberEnterTime(const std::string& seed_name,int entertime)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.entertime = entertime;
		}
		return 0;
	}
	int Guild::GetMemberEnterTime(const std::string& seed_name)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			return pos->second.entertime;
		}
		return 1;
	}
	int Guild::SetMemberLastLoginOut(const std::string& seed_name,int out)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.lastlogin = out;
		}
		return 0;
	}
	void Guild::GuildExpChange(DomainGuild* src)
	{
		if( GetMemberTitle(src->SeedName()) < GUILD_TITLE_TYPE_NORMAL )
		{
			return ;
		}
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( ( pos->second.title==GUILD_TITLE_TYPE_NORMAL
				|| pos->second.title==GUILD_TITLE_TYPE_ADV
				|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
				|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
				&& pos->second.player
				&& pos->second.player != src )
			{
				pos->second.player->NotifyGuildExpChange(guild_level(),guild_pt(),guild_pt());
			}
		}
	}
	void Guild::GuildChat(DomainGuild* src,char* msg)
	{
		if( GetMemberTitle(src->SeedName()) < GUILD_TITLE_TYPE_NORMAL )
		{
			return ;
		}
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( ( pos->second.title==GUILD_TITLE_TYPE_NORMAL
				|| pos->second.title==GUILD_TITLE_TYPE_ADV
				|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
				|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
				&& pos->second.player
				&& pos->second.player != src )
			{
				if ( pos->second.player->GuildName() == src->GuildName() &&  pos->second.player->GuildID() == src->GuildID() )
				{
					pos->second.player->NotifyGuildChat(this,src,msg);
				}
				/*else
				{
					RemMember(pos->second.player->SeedName());
				}*/
			}
		}
	}
	void Guild::ExeCode(const char* code)
	{
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( ( pos->second.title==GUILD_TITLE_TYPE_NORMAL
				|| pos->second.title==GUILD_TITLE_TYPE_ADV
				|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
				|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
				&& pos->second.player )
			{
				pos->second.player->ExeCode(code);
			}
		}
	}
	int Guild::AddEnemyGuild(const std::string& guild_name)
	{
		if( std::find(this->mEnemyGuilds.begin(),mEnemyGuilds.end(),guild_name) == mEnemyGuilds.end() )
		{
			mEnemyGuilds.push_back(guild_name);
		}
		return 0;
	}
	int Guild::RemEnemyGuild(const std::string& guild_name)
	{
		mEnemyGuilds.erase(std::remove(mEnemyGuilds.begin(),mEnemyGuilds.end(),guild_name),mEnemyGuilds.end());
		return 0;
	}
	bool Guild::IsEnemyGuild(const std::string& guild_name)
	{
		if( std::find(this->mEnemyGuilds.begin(),mEnemyGuilds.end(),guild_name) == mEnemyGuilds.end() )
		{
			return false;
		}
		return true;
	}
	int Guild::AddFriendGuild(const std::string& guild_name)
	{
		if( std::find(this->mFriendGuilds.begin(),mFriendGuilds.end(),guild_name) == mFriendGuilds.end() )
		{
			mFriendGuilds.push_back(guild_name);
		}
		return 0;
	}
	int Guild::RemFriendGuild(const std::string& guild_name)
	{
		mFriendGuilds.erase(std::remove(mFriendGuilds.begin(),mFriendGuilds.end(),guild_name),mFriendGuilds.end());
		return 0;
	}
	bool Guild::IsFriendGuild(const std::string& guild_name)
	{
		if( std::find(this->mFriendGuilds.begin(),mFriendGuilds.end(),guild_name) == mFriendGuilds.end() )
		{
			return false;
		}
		return true;
	}
	GuildNode* Guild::GetPremem(const char* seed_name){
		MemberMap::iterator pos = mPremems.find(seed_name);
		if( pos != mPremems.end() ){
			return &pos->second;
		}
		return 0;
	}

	int Guild::RemPreMember(const char* seed_name,DomainGuild* player)
	{
		mPremems.erase(seed_name);
		if(player)
		{
			player->onSetGuildEntering(this,0);

			for ( MemberMap::iterator pos = mMembers.begin();
				pos != mMembers.end();
				++ pos )
			{
				if( ( pos->second.title==GUILD_TITLE_TYPE_ADV
					|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
					|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
					&& pos->second.player )
				{
					if ( pos->second.player->GetPlayer() && pos->second.player->GuildName() == mName && player->GetPlayer())
					{
						mGameServerLib->GetScriptEngine()->PlayerPlayer(pos->second.player->GetPlayer(), player->GetPlayer(), "player.onCancelJoinGuild");
					}
				}
			}
		}

		return GUILD_TITLE_TYPE_OUT;
	}

	GuildNode* Guild::GetMember(const char* seed_name)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			return &pos->second;
		}
		return 0;
	}

	int Guild::AddPreMember(const char* seed_name,DomainGuild* player)
	{
		if(0==GetPremem(seed_name))
		{
			GuildNode gn;
			gn.seed_name=seed_name;
			gn.title=GUILD_TITLE_TYPE_ENTERING;
			gn.entertime = time(0);
			mPremems[seed_name]=gn;
			if(player)
			{
				SetMemberInfo(seed_name,player);
				player->onSetGuildEntering(this, 1);
			}
			
			// 有通过权的人触发
			for ( MemberMap::iterator pos = mMembers.begin();
				pos != mMembers.end();
				++ pos )
			{
				if( ( pos->second.title==GUILD_TITLE_TYPE_ADV
					|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
					|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
					&& pos->second.player )
				{
					if ( pos->second.player->GetPlayer() && pos->second.player->GuildName() == mName && player->GetPlayer())
					{
						mGameServerLib->GetScriptEngine()->PlayerPlayer(pos->second.player->GetPlayer(), player->GetPlayer(), "player.onApplyJoinGuild");
					}
				}
			}
		}
		return GUILD_TITLE_TYPE_OUT;
	}

	int Guild::SetMemberInfo(const char* seed_name,DomainGuild* player)
	{
		GuildNode* gn=0;
		{
			MemberMap::iterator pos = mMembers.find(seed_name);
			if( pos != mMembers.end() )
			{
				gn=&pos->second;
				pos->second.player = player;
				pos->second.online_state=player?1:0;
				if(player)
				{
					DomainPlay* p = player->GetPlayer();
					if (p)
					{
						MiniPlayerInfo mpi;
						memset(&mpi,0,sizeof(mpi));
						p->GetMiniPlayerInfo(&mpi);
						pos->second.nick_name = p->Name();
						pos->second.fight=p->FightPoint();
						pos->second.lastlogin=int(p->LastLogoutTime());
						pos->second.job = p->job();
						pos->second.lv = p->level();
						pos->second.gender = p->gender();
						pos->second.wing = p->GetWingLv();
						pos->second.weapon = mpi.item_weapon_id?mpi.item_weapon_id->mTypeID:0;
						pos->second.cloth = mpi.item_cloth_id?mpi.item_cloth_id->mTypeID : 0;
						// pos->second.wing = p->GetWingLv();
// 						pos->second.weapon = p->Weapon();
// 						pos->second.cloth = p->Cloth();
// 						

						if( pos->second.title == GUILD_TITLE_TYPE_ADMIN )
						{
							master_level(p->level());
							/*mMasterCloth=player->Cloth();
							mMasterWeapon=player->Weapon();
							mMasterWing=player->wing();
							mMasterPifeng=player->Pifeng();*/
						}
					}
				}
			}
		}
		{
			MemberMap::iterator pos = mPremems.find(seed_name);
			if( pos!=mPremems.end())
			{
				gn=&pos->second;
				pos->second.player = player;
				pos->second.online_state=player?1:0;
				if(player)
				{
					DomainPlay* p = player->GetPlayer();
					if (p)
					{
						MiniPlayerInfo mpi;
						memset(&mpi,0,sizeof(mpi));
						p->GetMiniPlayerInfo(&mpi);

						pos->second.nick_name = p->Name();
						pos->second.fight=p->FightPoint();
						pos->second.lastlogin=int(p->LastLogoutTime());
						pos->second.job = p->job();
						pos->second.lv = p->level();
						pos->second.gender = p->gender();

						pos->second.wing = p->GetWingLv();
						pos->second.weapon = mpi.item_weapon_id?mpi.item_weapon_id->mTypeID:0;
						pos->second.cloth = mpi.item_cloth_id?mpi.item_cloth_id->mTypeID : 0;
						//printf("seed %s   weapon= %d", pos->second.seed_name.c_str(), pos->second.weapon);
					}
				}
			}
		}
		if(gn&&player)
		{

		}
		return 0;
	}

	int Guild::AddItem(int typeID,int level,int zlevel)
	{
		int pos = find_empty_item_position();
		if(pos >=0 )
		{
			GuildItem guildItem;
			guildItem.mTypeID = typeID;
			guildItem.mLevel = level;
			guildItem.mZlevel = zlevel;
			mItems[pos] = guildItem;
			return 1;
		}
		return 0;
	}

	int Guild::find_empty_item_position()
	{
		char slot[GUILD_DEPOT_SIZE];
		memset(slot,0,GUILD_DEPOT_SIZE);
		for( ItemMap::const_iterator pos = GetItems().begin();
			pos != GetItems().end();
			++ pos )
		{
			if( pos->second.mTypeID > 0 && pos->first >= GUILD_DEPOT_BEGIN && pos->first < GUILD_DEPOT_END )
			{
				slot[pos->first - GUILD_DEPOT_BEGIN] ++;
			}
		}
		for(int i = 0;i < GUILD_DEPOT_SIZE;i++)
		{
			if( slot[i] == 0 )
			{
				return i;
			}
		}
		return -1;
	}

	void Guild::RemItem(int pos)
	{
		ItemMap::iterator item = mItems.find(pos);
		if(item != mItems.end())
		{
			mItems.erase(item);
		}
	}

	void Guild::AddRedPacketRecord(const char * nick_name, int value, int count, int op_code)
	{
		if (mRedPacketInfo.mRecordList.size() == RED_PACKET_RECORD_MAX_COUNT)
		{
			mRedPacketInfo.mRecordList.pop_front();
		}

		GuildRedPacketRecord record;
		record.name = nick_name;
		record.value = value;
		record.count = count;
		record.op_code = op_code;

		mRedPacketInfo.mRecordList.push_back(record);

		// 消息推送
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( ( pos->second.title==GUILD_TITLE_TYPE_NORMAL
				|| pos->second.title==GUILD_TITLE_TYPE_ADV
				|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
				|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
				&& pos->second.player )
			{
				if ( pos->second.player->GuildName() == mName)
				{
					pos->second.player->notifyGuildRedPacketLog(&record);
				}
			}
		}
	}

	void Guild::SetRedPacket(const char * nick_name, int value, int count)
	{ 
		if (value < 0 || count < 0)
			return;
		GameSession* gs = mGameServerLib->FindSessionWithCharID(nick_name);
		if (gs && gs->GetPlayer())
		{
			mRedPacketInfo.mSenderSeedName = gs->GetPlayer()->SeedName();
			mRedPacketInfo.mSenderName = nick_name;
			mRedPacketInfo.mRemainSize = count;
			mRedPacketInfo.mRemainValue = value;
			mRedPacketInfo.mSendTime = time(0);
			mRedPacketInfo.mTakeNames.clear();
			AddRedPacketRecord(nick_name, value, count, OP_CODE_SEND);
		}
		else
			return;
	}

	const char * Guild::GetRedPacketSender()
	{
		if (mRedPacketInfo.mSenderName.length() > 0)
		{
			return mRedPacketInfo.mSenderName.c_str();
		}
		return NULL;
	}

	int Guild::GetRedPacketRemainingValue()
	{
		return mRedPacketInfo.mRemainValue;
	}

	int Guild::GetRedPacketRemainingSize()
	{
		return mRedPacketInfo.mRemainSize;
	}

	int Guild::GetRedPacketRemainingTime()
	{
		return mGameServerLib->mGuildRedPacketTime * 60 - (time(0) - mRedPacketInfo.mSendTime);
	}

	int Guild::TakeRedPacketProc()
	{
		if (GetRedPacketRemainingTime() <= 0)
		{
			// 已过期
			return 0;
		}

		if (mRedPacketInfo.mRemainSize == 0)
		{
			// 没有剩余
			return 0;
		}

		int nTake = 0;
		if (mRedPacketInfo.mRemainSize == 1)
		{
			// 最后一个领取,取走剩余的
			nTake = mRedPacketInfo.mRemainValue;
		}
		else
		{
			// 总共可领取数量
			int nTotal = mRedPacketInfo.mRemainValue - mRedPacketInfo.mRemainSize + 1;

			int nMax = nTotal / mRedPacketInfo.mRemainSize * 3;
			nTake = rand() % nMax;

			if (nTake < 1)
				nTake = 1;

			int nLeft = mRedPacketInfo.mRemainValue - nTake;
			if (nLeft < mRedPacketInfo.mRemainSize)
			{
				int temp = mRedPacketInfo.mRemainSize - nLeft;
				nTake -= temp;
			}
		}

		mRedPacketInfo.mRemainValue -= nTake;
		mRedPacketInfo.mRemainSize --;

		return nTake;
	}

	int Guild::TakeRedPacket(const char * nick_name)
	{
		if (mRedPacketInfo.mTakeNames.find(nick_name) != mRedPacketInfo.mTakeNames.end())
		{
			// 已经领取
			return -1;
		}

		int nTake = TakeRedPacketProc();

		if (nTake > 0)
		{
			AddRedPacketRecord(nick_name, nTake, 1, OP_CODE_TAKE);
			mRedPacketInfo.mTakeNames.insert(nick_name);
		}
		return nTake;
	}

	int Guild::CheckRedPacket(const char * nick_name)
	{
		if (mRedPacketInfo.mTakeNames.find(nick_name) != mRedPacketInfo.mTakeNames.end())
		{
			// 已经领取
			return -1;
		}
		if (GetRedPacketRemainingTime() <= 0)
		{
			// 已过期
			return -2;
		}
		//返回剩余红包数量
		return mRedPacketInfo.mRemainSize;
	}

	Guild::RedPacketRecordList & Guild::GetRedPacketLogList()
	{
		return mRedPacketInfo.mRecordList;
	}

	Guild::ItemOpRecordList & Guild::GetGuildItemOpLogList()
	{
		return mItemOpRecordList;
	}

	void Guild::SendBackRedPacket()
	{
		if (mRedPacketInfo.mRemainValue <= 0)
			return;
		std::string title = mGameServerLib->mStringConsts.GUILD_RED_PACKET_MAIL_TITLE;
		std::string content = mGameServerLib->mStringConsts.GUILD_RED_PACKET_MAIL_CONTENT;
		std::vector<std::string> vNames;
		vNames.push_back(mRedPacketInfo.mSenderSeedName);
		std::map<int,int> items;
		items[ITEM_VCOIN_ID] = mRedPacketInfo.mRemainValue;
		mGameServerLib->GetMailManager()->PostMailToMultiplePlayers(title, title, vNames, items);
		ClearRedPacketInfo();
		
	}

	void Guild::AddItemOpRecord(std::string name, int item_type, int op_code)
	{
		if (mItemOpRecordList.size() == GUILD_ITEM_RECORD_MAX_COUNT)
		{
			mItemOpRecordList.pop_front();
		}

		GuildItemOpRecord record;

		record.name = name;
		record.item_type = item_type;
		record.time = time(0);
		record.op_code = op_code;

		mItemOpRecordList.push_back(record);
	}

	void Guild::ClearRedPacketInfo()
	{
		mRedPacketInfo.mSenderSeedName = "";
		mRedPacketInfo.mSenderName = "";
		mRedPacketInfo.mTakeNames.clear();
		mRedPacketInfo.mRemainSize = 0;
		mRedPacketInfo.mRemainValue = 0;
		mRedPacketInfo.mSendTime = 0;

		mRedPacketInfo.mRecordList.clear();
	}

	void Guild::ClearItemRecords()
	{
		mItemOpRecordList.clear();
	}

	int Guild::SetMemberLv(const std::string& seed_name,int lv)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.lv = lv;
		}
		return 0;
	}

	int Guild::SetMemberJob(const std::string& seed_name,int job)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.job = job;
		}
		return 0;
	}

	int Guild::StartGuildWar(std::string seed_id, bool boAct)
	{
		if (seed_id == SeedID())
		{
			return 1;
		}

		GuildWarMap::iterator it = mGuildWarMap.find(seed_id);

		if (it != mGuildWarMap.end())
		{
			// 已发起
			return 1;
		}

		Guild * pWarGuild = mGameServerLib->GetGuildManager()->FindGuild(seed_id);
		if (pWarGuild == NULL || pWarGuild->deleted())
		{
			return 1;
		}

		GuildWar war;
		war.mGuildSeed = seed_id;
		war.mAct = boAct?1:0;
		war.mStartTime = time(0);
		mGuildWarMap[seed_id] = war;

		if (boAct)
		{
			// 主动发起
			pWarGuild->StartGuildWar(this->SeedID(), false);
			char temp[512];_snprintf_s(temp,_countof(temp),511,mGameServerLib->mStringConsts.NOTIFY_GUILD_WAR_START,pWarGuild->mName.c_str());temp[511]=0;
			NotifyAlert(1, 1, temp);
			NotifyWar();
			_snprintf_s(temp,_countof(temp),511,mGameServerLib->mStringConsts.NOTIFY_GUILD_WAR_START,mName.c_str());temp[511]=0;
			pWarGuild->NotifyAlert(1, 1, temp);
			pWarGuild->NotifyWar();
		}
		return 0;
	}

	bool Guild::IsWarGuild(std::string& seed_id)
	{
		GuildWarMap::iterator it = mGuildWarMap.find(seed_id);

		if (it != mGuildWarMap.end())
			return true;
		return false;
	}

	void Guild::RemoveGuildWar(std::string seed_id)
	{
		GuildWarMap::iterator it = mGuildWarMap.find(seed_id);
		if (it != mGuildWarMap.end())
		{
			mGuildWarMap.erase(it);
		}
	}

	void Guild::ClearGuildWar()
	{
		mGuildWarMap.clear();
	}

	void Guild::ClearRelGuildWar()
	{
		GuildWarMap::iterator it = mGuildWarMap.begin();
		while (it != mGuildWarMap.end())
		{
			if (it->second.mAct)
			{
				// 将对方移除
				Guild * guild = mGameServerLib->GetGuildManager()->FindGuild(it->second.mGuildSeed);
				if (guild)
				{
					guild->RemoveGuildWar(this->SeedID());
				}
			}

			++ it;
		}
	
	}

	void Guild::UpdateGuildWar()
	{
		GuildWarMap::iterator it = mGuildWarMap.begin();
		
		while (it != mGuildWarMap.end())
		{
			// 判断行会战是否到期
			if (it->second.mAct && time(0) - it->second.mStartTime > mGameServerLib->mGuildWarTime * 60)
			{
				// 将对方移除
				Guild * guild = mGameServerLib->GetGuildManager()->FindGuild(it->second.mGuildSeed);
				if (guild)
				{
					guild->RemoveGuildWar(this->SeedID());
				}

				// 到期
				it = mGuildWarMap.erase(it);
				continue;
			}
		
			++ it;
		}
	}

	Guild::GuildWarMap & Guild::GetGuildWarMap()
	{
		return mGuildWarMap;
	}

	void Guild::StartAllGuildWar()
	{
		const ManagerGuild::GuildMap& guild_map = mGameServerLib->GetGuildManager()->GetGuilds();
		for (ManagerGuild::GuildMap::const_iterator it = guild_map.begin(); it != guild_map.end() ; ++ it)
		{
			if (it->second)
			{
				this->StartGuildWar(it->second->SeedID());
			}
		}
	}

	void Guild::GuildWarState(std::string& seed_id, int &war_state, int &start_time)
	{
		GuildWarMap::iterator it = mGuildWarMap.find(seed_id);
		if (it == mGuildWarMap.end())
		{
			war_state = 0;
			start_time = 0;
			return;
		}
		war_state = 1;
		//time_left = mGameServerLib->mGuildWarTime - (time(0) - it->second.mStartTime);
		start_time = it->second.mStartTime;
	}

	void Guild::NotifyGuildMemberChange(int min_title)
	{
		// 消息推送
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if (pos->second.player)
			{
				if (
					((min_title == 0) && ( pos->second.title==GUILD_TITLE_TYPE_NORMAL
					|| pos->second.title==GUILD_TITLE_TYPE_ADV
					|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
					|| pos->second.title==GUILD_TITLE_TYPE_ADMIN )) || 
					((min_title > 0) && (pos->second.title >= pos->second.title))
					)
				{
					if ( pos->second.player->GuildName() == mName)
					{
						pos->second.player->notifyGuildMemberChange();
					}
				}
			}
		}
	}

	void Guild::NotifyAlert(int po,int flags,const char* msg)
	{
		// 消息推送
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( ( pos->second.title>=GUILD_TITLE_TYPE_NORMAL
				|| pos->second.title==GUILD_TITLE_TYPE_ADV
				|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
				|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
				&& pos->second.player )
			{
				if ( pos->second.player->GuildName() == mName)
				{
					pos->second.player->NotifyAlert(po,flags,msg);
				}
			}
		}
	}

	void Guild::NotifyWar()
	{
		// 消息推送
		for ( MemberMap::iterator pos = mMembers.begin();
			pos != mMembers.end();
			++ pos )
		{
			if( ( pos->second.title>=GUILD_TITLE_TYPE_NORMAL
				|| pos->second.title==GUILD_TITLE_TYPE_ADV
				|| pos->second.title==GUILD_TITLE_TYPE_VICEADMIN
				|| pos->second.title==GUILD_TITLE_TYPE_ADMIN ) 
				&& pos->second.player )
			{
				if ( pos->second.player->GuildName() == mName)
				{
					pos->second.player->NotifyWar();
				}
			}
		}
	}

	void Guild::SetMaxMemberCount(int count)
	{
		mMaxMemberCount = count;
	}

	int Guild::SetMemberGender(const std::string& seed_name,int gender)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.gender = gender;
		}
		return 0;
	}

	int Guild::SetMemberCloth(const std::string& seed_name,int v)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.cloth = v;
		}
		return 0;
	}

	int Guild::SetMemberWeapon(const std::string& seed_name,int v)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.weapon = v;
		}
		return 0;
	}

	int Guild::SetMemberWing(const std::string& seed_name,int v)
	{
		MemberMap::iterator pos = mMembers.find(seed_name);
		if( pos != mMembers.end() )
		{
			pos->second.wing = v;
		}
		return 0;
	}

	void Guild::FreshMemberInfo()
	{
		for (MemberMap::iterator it = mMembers.begin(); it != mMembers.end(); ++ it)
		{
			if (it->second.player)
			{
				//it->second.player->SetGuildInfo()
				DomainPlay* p = it->second.player->GetPlayer();
				if (p)
				{
					MiniPlayerInfo mpi;
					memset(&mpi,0,sizeof(mpi));
					p->GetMiniPlayerInfo(&mpi);
					it->second.nick_name = p->Name();
					it->second.fight=p->FightPoint();
					it->second.lastlogin=int(p->LastLogoutTime());
					it->second.job = p->job();
					it->second.lv = p->level();
					it->second.gender = p->gender();
					it->second.wing = p->GetWingLv();
					it->second.weapon = mpi.item_weapon_id?mpi.item_weapon_id->mTypeID:0;
					it->second.cloth = mpi.item_cloth_id?mpi.item_cloth_id->mTypeID : 0;
				}
			}
		}
	}

}

LEUD::PoolMemory<OGSLib::Guild>	LEUD::PoolMemoryBase<OGSLib::Guild>::memPool("Guild");
