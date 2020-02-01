#include "StdAfx.h"
#include "PlayMonsterPuppet.h"
#include "DomainPlay.h"
#include "PlaySlave.h"
#include "GameServerLib.h"
namespace OGSLib
{
	PlayMonsterPuppet::PlayMonsterPuppet(void)
	{
		mExpMul = 1000;
		//mRecoveHP = 0;
	}
	void PlayMonsterPuppet::DoOpt(const char* opt)
	{
		int value;
		if( sscanf_s(opt,"expmul(%d)",&value) == 1 ){mExpMul = value;}
		PlayMonsterNomove::DoOpt(opt);
	}
	PlayMonsterPuppet::~PlayMonsterPuppet(void)
	{
	}
	int PlayMonsterPuppet::AiIdle()
	{
		return mAIState;
	}

	int  PlayMonsterPuppet::Attacked(PlayActiveObject* src, SkillDef * sd,int pawer,int flags,int delay)
	{
		DomainPlay* player = 0;
		if( src->TypeIsSlave() )
		{
			PlaySlave* slave = static_cast<PlaySlave*>(src);
			player = slave->Owner();
		}
		if( src->TypeIsPlayer() )
		{
			player = static_cast<DomainPlay*>(src);
		}
		if( player )
		{
			if(player->level()<level()+10)
			{
				player->WinExpByMonster(pawer*mGSL->mPuppetExtMul/1000*mExpMul/1000*player->mPuppetMul/1000);
			}
		}
		return PlayMonster::Attacked(src,sd,pawer,flags,delay);
	}
}