#pragma once
#include "PlayMonster.h"

namespace OGSLib
{
	class PlayMonsterNomove :
		public PlayMonster
	{
	public:
		PlayMonsterNomove(void);
		virtual ~PlayMonsterNomove(void);
		virtual int AiIdle();
		virtual void AiOther(double time);
	};
}
