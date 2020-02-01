#pragma once
#include "PlayMonsterNomove.h"

namespace OGSLib
{
	class PlayMonsterPuppet : public PlayMonsterNomove
	{
		int mExpMul;
	public:
		PlayMonsterPuppet(void);
		virtual ~PlayMonsterPuppet(void);
		virtual int AiIdle();
		virtual int  Attacked(PlayActiveObject* src,SkillDef * sd,int pawer,int flags,int delay);
		virtual void DoOpt(const char* opt);
	};
}