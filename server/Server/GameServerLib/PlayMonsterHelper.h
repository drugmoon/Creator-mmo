#pragma once
#include "PlayMonster.h"

namespace OGSLib
{
	class PlayMonsterHelper : public PlayMonster
	{
	public:
		PlayMonsterHelper(void);
		virtual ~PlayMonsterHelper(void);
		virtual int  Attacked(PlayActiveObject* src,int skill_type,int pawer,int flags,int delay);
		virtual int AiIdle();
	};
	class PlayMonsterPlant : public PlayMonster
	{
	public:
		PlayMonsterPlant(void);
		virtual ~PlayMonsterPlant(void);
		virtual int AiIdle();
	};
	class PlayMonsterCollection:public PlayMonster
	{
	public:
		int mCollectTime;
		PlayMonsterCollection(void);
		virtual ~PlayMonsterCollection(void);
		virtual int AiIdle();
		virtual int CollectTime(){return mCollectTime;}
		virtual int Attacked(PlayActiveObject* src,int skill_type,int pawer,int flags,int delay);
	};
}