#include "StdAfx.h"
#include "PlayNeutralRam.h"
#include "DomainPlay.h"
#include "GuildGuild.h"
#include "GameServerLib.h"
namespace OGSLib
{
	PlayNeutralRam::PlayNeutralRam(void)
	{
	}
	PlayNeutralRam::~PlayNeutralRam(void)
	{
	}
	int PlayNeutralRam::Update(double time)
	{
		if( mGSL->IsWar() <= 0 )
		{
			DamageHP(0,999999,-1);
		}
		return PlayNeutral::Update(time);
	}
	void PlayNeutralRam::NotifyMeet(PlayMap* map,PlayMapObject* src)
	{
		PlayNeutral::NotifyMeet(map,src);
		if( mGuildName[0] != 0 )
		{
			PlayActiveObject* tar = Target();
			if( tar && tar->TypeIsPlayer() )
			{
				DomainPlay* tarplay = static_cast<DomainPlay*>(tar);
				char gn[256];
				gn[0]=0;
				mGSL->GetKingGuild(gn,256);
				if( 0 != strcmp(gn,tarplay->GuildName().c_str()) )
				{
					if( src->TypeIsPlayer() )
					{
						DomainPlay* play = static_cast<DomainPlay*>(src);
						if( play && play->GuildTitle() > GUILD_TITLE_TYPE_ENTERING && 0 == strcmp(gn,play->GuildName().c_str()) )
						{
							CheckTarget(src,src->X(),src->Y());
						}
					}
				}
			}
		}
	}
	void PlayNeutralRam::CheckTarget(PlayMapObject* src,int newx,int newy)
	{
		if( src->TypeIsPlayer() )
		{
			DomainPlay* play = static_cast<DomainPlay*>(src);
			if( play )
			{
				if( mGuildName[0] != 0 )
				{
					if( play->GuildName().length() > 0 && play->GuildTitle() > GUILD_TITLE_TYPE_ENTERING && 0 != strcmp(mGuildName,play->GuildName().c_str()) )
					{
						Target((PlayActiveObject*)src);
					}
				}
			}
		}
	}
	bool PlayNeutralRam::IsProperAttackDes(PlayActiveObject* des, SkillDef * sd)
	{
		bool ret=PlayNeutral::IsProperAttackDes(des,sd);
		if( ret )
		{
			if( des->TypeIsPlayer() )
			{
				DomainPlay* play = static_cast<DomainPlay*>(des);
				if( play )
				{
					if( play->GuildName().length() > 0 && play->GuildTitle() > GUILD_TITLE_TYPE_ENTERING && 0 == strcmp(mGuildName,play->GuildName().c_str()) )
					{
						return false;
					}
				}
			}
		}
		return ret;
	}
	void PlayNeutralRam::OnDead(PlayActiveObject* src,int delay)
	{
		if( mGSL->IsWar() && mGSL->mPlayerTrigerWarEvent > 0 && src && src->TypeIsPlayer() )
		{
			DomainPlay* ply = (DomainPlay*)src;
			if( mGuildName[0] != 0 && (!ply->GuildName().empty()) && ply->GuildTitle()>GUILD_TITLE_TYPE_ENTERING )
			{
				char msg[512];
				_snprintf_s(msg,_countof(msg),511,mGSL->mStringConsts.NOTIFY_WAR_KILL_RAM,ply->GuildName().c_str(),ply->Name(),mGuildName);
				msg[511]=0;
				mGSL->ServerAlertMessage(10,0,msg);
			}
		}
		PlayNeutral::OnDead(src,delay);
	}
}