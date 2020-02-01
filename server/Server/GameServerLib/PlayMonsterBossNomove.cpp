#include "StdAfx.h"
#include "PlayMonsterBossNomove.h"
#include "ManagerSkillDef.h"
#include "DomainPlay.h"
namespace OGSLib
{
	PlayMonsterBossNomove::PlayMonsterBossNomove(void)
	{
	}
	PlayMonsterBossNomove::~PlayMonsterBossNomove(void)
	{
	}
	int PlayMonsterBossNomove::AiIdle()
	{
		mAIState = AI_STATE_NULL;
		return mAIState;
	}
}