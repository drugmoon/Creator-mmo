#pragma once
#include "PlayNeutral.h"
namespace OGSLib
{
	class PlayNeutralRam : public PlayNeutral
	{
	public:
		PlayNeutralRam(void);
		virtual ~PlayNeutralRam(void);
		virtual int Update(double time);
		virtual bool IsProperAttackDes(PlayActiveObject* des, SkillDef * sd);
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void CheckTarget(PlayMapObject* src,int newx,int newy);
		virtual void OnDead(PlayActiveObject* src,int delay);
	};
}
