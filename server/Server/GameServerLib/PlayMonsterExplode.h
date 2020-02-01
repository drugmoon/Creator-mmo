#pragma once
#include "PlayMonster.h"
namespace OGSLib
{
	class PlayMonsterExplode :
		public PlayMonster
	{
	public:
		PlayMonsterExplode(void);
		virtual ~PlayMonsterExplode(void);

		virtual int AiIdle();
		virtual void OnDead(PlayActiveObject* src,int delay);
	};
}