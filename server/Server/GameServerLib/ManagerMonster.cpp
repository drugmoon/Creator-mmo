#include "StdAfx.h"
#include "ManagerMonster.h"
#include "PlayAIObject.h"
namespace OGSLib
{
	ManagerMonster::ManagerMonster(void)
	{
		mUpdateTime=0.0;
		mUpdateDuration=TimerFrame::FRAME_TICK;
		mDieClearTime=0;
		mDieClearDuration=60;
		mUpdateSeed = 0;
	}
	ManagerMonster::~ManagerMonster(void)
	{
	}

	void ManagerMonster::AddMonster(PlayAIObject* mon,int rightnow)
	{
		mMonsterList.push_back(mon);
		AddMonsterNode(rightnow,&mon->mUpdateNode);
	}

	void ManagerMonster::AddDart(PlayAIObject * dart)
	{
		this->m_DartList.push_back(dart);
	}

	void ManagerMonster::RemMonster(PlayAIObject* mon)
	{
		mMonsterList.erase(std::remove(mMonsterList.begin(),mMonsterList.end(),mon),mMonsterList.end());
	}
	class EmptyMonsterRemover
	{
	public:
		bool operator() (PlayAIObject* & mon){if( mon->dieclear() ){mon->mUpdateNode.leave();delete mon;return true;}return false;}
	};
	class EmptyDartRemover
	{
	public:
		bool operator() (PlayAIObject* & dart){if( dart->dieclear() ){delete dart;return true;}return false;}
	};
	class MonsterRemover
	{
	public:
		bool operator() (PlayAIObject* & mon)
		{
			delete mon;
			return true;
		}
	};
	void ManagerMonster::TheDest()
	{
		MonsterRemover eir;
		mMonsterList.erase(std::remove_if(mMonsterList.begin(),mMonsterList.end(),eir),mMonsterList.end());
	}
	void ManagerMonster::Update(double time)
	{
		PERF_NODE_FUNC();
		if( mDieClearTime + mDieClearDuration < time )
		{
			PERF_NODE("clear_die");
			mDieClearTime = time;
			mMonsterList.erase(std::remove_if(mMonsterList.begin(),mMonsterList.end(),EmptyMonsterRemover()),mMonsterList.end());

			m_DartList.erase(std::remove_if(m_DartList.begin(),m_DartList.end(),EmptyDartRemover()),m_DartList.end());
		}
		if( mUpdateTime + mUpdateDuration < time )
		{
			PERF_NODE("update mon");
			mUpdateTime = time;
			mUpdateSeed ++;
			mUpdateSeed = mUpdateSeed % G_MANAGER_MONSTER_LIST_NUM;
		}

		size_t num = 0;
		for( PlayMapObjectNode* node = mUpdateList[mUpdateSeed].begin();
			node != mUpdateList[mUpdateSeed].end(); )
		{
			num ++;if( num > 500 ) break;
			PlayAIObject* mon = (PlayAIObject*)node->This;
			node = node->Next;
			if( mon && mon->dieclear() == 0 )
			{
				AddMonsterNode(mon->Update(time),&mon->mUpdateNode);
			}
		}

		for (std::vector<PlayAIObject *>::iterator it = m_DartList.begin(); it != m_DartList.end(); ++ it)
		{
			PlayAIObject * dart = *it;
			if (dart && dart->dieclear() == 0)
				dart->Update(time);
		}
	}
	void ManagerMonster::AddMonsterNode(int next,PlayMapObjectNode* node)
	{
		if( next == -1 ) next = G_MANAGER_MONSTER_LIST_NUM-1;
		if( next == 0 ) next = 50 - 1 + rand() % (G_MANAGER_MONSTER_LIST_NUM-50);
		next = next % G_MANAGER_MONSTER_LIST_NUM;
		int pos = (next + mUpdateSeed)%G_MANAGER_MONSTER_LIST_NUM;
		PlayMapObjectNodeList* nl = &mUpdateList[pos];
		nl->push_back(node);
	}

	PlayAIObject * ManagerMonster::FindDartById(int id)
	{
		for (size_t i = 0; i < m_DartList.size(); ++ i)
		{
			if (m_DartList[i]->ID() == id)
				return m_DartList[i];
		}
		return NULL;
	}

	PlayAIObject * ManagerMonster::FindMonsterById(int id)
	{
		for (size_t i = 0; i < mMonsterList.size(); ++ i)
		{
			if (mMonsterList[i]->ID() == id)
				return mMonsterList[i];
		}
		return NULL;
	}
}