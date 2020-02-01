#pragma once
#include "PlayMonsterRemote.h"
namespace OGSLib
{
	class PlayMonsterBossNomove :
		public PlayMonster
	{
	public:
		PlayMonsterBossNomove(void);
		virtual ~PlayMonsterBossNomove(void);
		virtual int AiIdle();
	};
}