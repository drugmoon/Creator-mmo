#pragma once
#include "PlayMapObjectNode.h"

namespace OGSLib
{
	class PlayAIObject;
	static const int G_MANAGER_MONSTER_LIST_NUM=250;
	class ManagerMonster
	{
	public:
		ManagerMonster(void);
		virtual ~ManagerMonster(void);

		std::vector<PlayAIObject*> mMonsterList;
		double mUpdateTime;
		double mUpdateDuration;
		double mDieClearTime;
		double mDieClearDuration;
		PlayMapObjectNodeList mUpdateList[G_MANAGER_MONSTER_LIST_NUM];
		std::vector<PlayAIObject*> m_DartList;
		int mUpdateSeed;

		void AddMonster(PlayAIObject* mon,int rightnow);
		void RemMonster(PlayAIObject* mon);
		void AddMonsterNode(int next,PlayMapObjectNode* node);
		void AddDart(PlayAIObject * dart);
		PlayAIObject * FindDartById(int id);
		PlayAIObject * FindMonsterById(int id);

		void Update(double time);
		void TheDest();
	};
}