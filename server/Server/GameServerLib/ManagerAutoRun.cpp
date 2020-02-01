#include "StdAfx.h"
#include "ManagerAutoRun.h"
#include "GameServerLib.h"
#include "ScriptLua.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
namespace OGSLib
{
	ManagerAutoRun::ManagerAutoRun(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
		mUpdateTick = 0;
		mUpdateLoadAutoTick = time(0);
		mUpdateDBTick = time(0);
	}
	ManagerAutoRun::~ManagerAutoRun(void)
	{
	}
	bool ManagerAutoRun::Init()
	{
		mGameServerLib->GetListener()->LoadAuto();
		return LoadAutoRun();
	}
	void ManagerAutoRun::Dest()
	{
	}
	void ManagerAutoRun::Update(double tt)
	{
		time_t t = time(0);
		if( mUpdateLoadAutoTick + 10*60 < t )
		{
			mUpdateLoadAutoTick = t;
			mGameServerLib->GetListener()->LoadAuto();
		}
		if( mUpdateDBTick + 60 < t )
		{
			time_t ot = mUpdateDBTick;
			mUpdateDBTick = t;
			for(std::vector<OGSLib::AutoRunDB>::iterator pos = mAutoRunDB.begin();
				pos != mAutoRunDB.end();
				++ pos )
			{
				if( pos->mBeginTime >= ot && pos->mBeginTime < t )
				{
					mGameServerLib->GetScriptEngine()->SystemScript(pos->mBeginScript.c_str());
				}
				if( pos->mEndTime >= ot && pos->mEndTime < t )
				{
					mGameServerLib->GetScriptEngine()->SystemScript(pos->mEndScript.c_str());
				}
			}
		}
		if( mUpdateTick < t )
		{
			mUpdateTick = t;
			for( AutoRunNodeVector::iterator pos = mAutoRuns.begin();
				pos != mAutoRuns.end();
				++ pos )
			{
				switch(pos->mType)
				{
				case AutoRunNode::AUTORUN_TYPE_REPET:
					{
						if( pos->mTick + pos->mTypeParam < t )
						{
							pos->mTick = t;
							mGameServerLib->GetScriptEngine()->SystemScript(pos->mScript.c_str());
						}
					}
					break;
				case AutoRunNode::AUTORUN_TYPE_DAY:
					{
						time_t dt = ( t + mGameServerLib->mTimeZone*60*60 ) % (24*60*60);
						if( pos->mTick!= 0 && dt >= pos->mTypeParam && pos->mTick < pos->mTypeParam )
						{
							mGameServerLib->GetScriptEngine()->SystemScript(pos->mScript.c_str());
						}
						pos->mTick = dt;
					}
					break;
				}
			}
		}
		{
			PERF_NODE("mTimer");
			for (TimerMap::iterator pos = mTimer.begin();
				pos != mTimer.end();
				++pos)
			{
				if (pos->second > 0.0 && tt > pos->second)
				{
					pos->second = 0.0;
					char ss[512]; _snprintf_s(ss, _countof(ss), 511, "war.onTimer%d", (pos->first + 1)); ss[511] = 0;
					mGameServerLib->GetScriptEngine()->SystemScript(ss);
				}
			}
		}
	}
	bool ManagerAutoRun::LoadAutoRun()
	{
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("autorun.txt",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			mAutoRuns.clear();
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					char type[256];
					char param[256];
					char script[256];
					int ret = sscanf_s(line,"%[^ \t]%*[ \t]%[^ \t]%*[ \t]%[^ \t]%*[ \t]",type,_countof(type),param,_countof(param),script,_countof(script));
					if( ret == 3 )
					{
						AutoRunNode arn;
						std::string t(type);
						if( t == "repet" )
						{
							arn.mType = AutoRunNode::AUTORUN_TYPE_REPET;
							arn.mTypeParam = atoi(param);
							arn.mScript = script;
						}
						else if( t == "day" )
						{
							arn.mType = AutoRunNode::AUTORUN_TYPE_DAY;
							int h=0;
							int m=0;
							if( 2 == sscanf_s(param,"%d:%d",&h,&m) )
							{
								arn.mTypeParam = h*60*60 + m*60;
							}
							arn.mScript = script;
						}
						if( arn.mTypeParam > 0 && (!arn.mScript.empty()) )
						{
							if( mGameServerLib->GetListener()->GetKuaFuService() )
							{
								if( arn.mScript != "autorun.checkkuafustart" && arn.mScript != "autorun.checkkuafustop" )
								{
									continue;
								}
							}
							mAutoRuns.push_back(arn);
						}
					}
				}
			}
		}
		return true;
	}
	int  ManagerAutoRun::SetMapTimer(int i, int esp)
	{
		if (i >= 1)
		{
			mTimer[i - 1] = mGameServerLib->GetTimerFix()->Elapsed() + esp / 1000.0;
			if (esp == 0)
			{
				mTimer[i - 1] = 0;
			}
			return i;
		}
		return 0;
	}
}