#include "StdAfx.h"
#include "PlayMonsterHelper.h"
#include "DomainPlay.h"
#include "PlayMap.h"
namespace OGSLib
{
	PlayMonsterHelper::PlayMonsterHelper(void)
	{
	}
	PlayMonsterHelper::~PlayMonsterHelper(void)
	{
	}
	int  PlayMonsterHelper::Attacked(PlayActiveObject* src,int skill_type,int pawer,int flags,int delay)
	{
		if( src->TypeIsPlayer() )
		{
			DomainPlay* play=(DomainPlay*)src;
			char ss[256];
			_snprintf_s(ss,_countof(ss),255,"attack pawer:%d",pawer);
			ss[255]=0;
			play->AlertMessage(10,0,ss);
		}
		return 0;
	}
	int PlayMonsterHelper::AiIdle()
	{
		return mAIState;
	}
	PlayMonsterPlant::PlayMonsterPlant(void)
	{
	}
	PlayMonsterPlant::~PlayMonsterPlant(void)
	{
	}
	int PlayMonsterPlant::AiIdle()
	{
		return mAIState;
	}
	PlayMonsterCollection::PlayMonsterCollection(void)
	{
		mDisapearTime = 0;
		mCollectTime = 0;
	}
	PlayMonsterCollection::~PlayMonsterCollection(void)
	{
	}
	int PlayMonsterCollection::AiIdle()
	{
		return mAIState;
	}
	int PlayMonsterCollection::Attacked(PlayActiveObject* src,int skill_type,int pawer,int flags,int delay)
	{
		return 0;
	}
}