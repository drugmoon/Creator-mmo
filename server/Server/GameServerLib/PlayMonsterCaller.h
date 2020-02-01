#pragma once
#include "PlayMonster.h"
namespace OGSLib
{
	class PlayMonsterCaller :
		public PlayMonster
	{
	public:
		PlayMonsterCaller(void);
		virtual ~PlayMonsterCaller(void);

		virtual int AiIdle();
		virtual void OnDead(PlayActiveObject* src,int delay);
	};
}