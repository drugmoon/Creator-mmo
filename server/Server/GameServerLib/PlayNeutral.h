#pragma once
#include "PlayMonster.h"
namespace OGSLib
{
	class ManagerMap;
	class PlayNeutral : public PlayMonster
	{
		friend class ManagerMap;
	protected:
		int mValor;
		bool mNomove;
		char mGuildName[256];

		//ƒπ±ÆπÈ Ùπ÷id
		int m_nMonsterSeedId;

		TimerFix   m_DisappearTime;
		int m_nDisappearTime;

	public:
		const char* GuildName(){return mGuildName;}
		PlayNeutral(void);
		virtual ~PlayNeutral(void);
		virtual int  Attacked(PlayActiveObject* src, SkillDef * sd,int pawer,int flags,int delay);
		virtual int AiIdle();
		virtual void AiOther(double time);
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void CheckTarget(PlayMapObject* src,int newx,int newy);
		virtual bool IsProperAttackDes(PlayActiveObject* des, SkillDef *sd);
		//virtual int CollectTime();
		
		//”√”⁄œ‘ æƒπ±Æµπº∆ ±
		void RestartDisappear(int time);
		int GetDisappearTime();

		void SetMonsterSeedId(int seedId){m_nMonsterSeedId = seedId;}
		int GetMonsterSeedId() {return m_nMonsterSeedId;}

	};
}