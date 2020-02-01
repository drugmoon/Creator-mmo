#pragma once
#include "PlayMonster.h"
namespace OGSLib
{
	class PlayMonsterRemote : public PlayMonster
	{
	protected:
		int mValor;
		bool mNomove;
	public:
		PlayMonsterRemote(void);
		virtual ~PlayMonsterRemote(void);
		virtual int AiIdle();
	};
}