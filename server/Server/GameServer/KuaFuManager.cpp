#include "stdafx.h"
#include "KuaFuManager.h"
KuaFuManager::KuaFuManager()
{
	mUpdateTick = time(0);
	mTicketSeed = 10;
}
KuaFuManager::~KuaFuManager()
{
}
void KuaFuManager::AddAuthTick(std::string& tick)
{
	mAuthTickMap[tick] = time(0);
}
int KuaFuManager::CheckAuthTick(std::string& tick)
{
	std::map<std::string,time_t>::iterator pos = mAuthTickMap.find(tick);
	if( pos!=mAuthTickMap.end() )
	{
		if( time(0) < pos->second + 60 )
		{
			return 0;
		}
	}
	return 1;
}
void KuaFuManager::Update()
{
	time_t t = time(0);
	if( t > mUpdateTick+10)
	{
		mUpdateTick=t;
		for(std::map<std::string,time_t>::iterator pos = mAuthTickMap.begin();
			pos != mAuthTickMap.end();)
		{
			if( t > pos->second + 60 )
			{
				mAuthTickMap.erase(pos++);
			}
			else
			{
				++pos;
			}
		}
	}
}


