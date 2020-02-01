#include "stdafx.h"
#include "leudgrid/Logger.h"
#include "leudgrid/ToolsPath.h"
#include <sstream>
#include <ctime>
#include "leudgrid/EventBase.h"
#include "leudgrid/MemoryPool.h"
namespace LEUD
{
	class LogEvent : public EventBase,public LEUD::PoolMemoryBase< LogEvent >
	{
		Logger* mLogger;
		char mMsg[128];
		time_t mTick;
	public:
		LogEvent(Logger* logger,const char* msg)
		{
			mLogger = logger;
			strncpy_s(mMsg,_countof(mMsg),msg,127);
			mMsg[127]=0;
			mTick = time(0);//+rand()%(60*60*4) - 60*60*2;
		}
		virtual void OnHanding(unsigned int queue_id,void* param)
		{
			if( mLogger )
			{
				mLogger->writeLog(mMsg,mTick);
			}
			delete this;
		}
	};
	int Logger::OnStart()
	{
		for(int i = 0; i < 3;i++ )
		{
			mTick[i]=0;
			mFile[i]=0;
		}
		mUpdateTick = time(0);
		mDummy = OpenLogFile(0);
		return 0;
	}
	int Logger::OnStop()
	{
		OnUpdate();
		OnUpdate();
		OnUpdate();
		for(int i = 0;i<3;i++)
		{
			if( mFile[i] )
			{
				fclose(mFile[i]);
				mFile[i] = 0;
			}
		}
		if( mDummy )
		{
			fclose(mDummy);
			mDummy = 0;
		}
		return 0;
	}
	void Logger::OnUpdate()
	{
		if( mUpdateTick + 60*3 < time(0) )
		{
			mUpdateTick = time(0);
			UpdateFile();
			EventQueueThread::OnUpdate();
			for(int i = 0;i<3;i++)
			{
				if( mFile[i] )
				{
					fclose(mFile[i]);
					mFile[i] = 0;
					mTick[i] = 0;
				}
			}
		}
	}
	void Logger::Log(const char* msg)
	{
		LogEvent* le = new LogEvent(this,msg);
		PushEvent(le);
	}
	void Logger::writeLog(const char* msg,time_t tick)
	{
		time_t h = (tick) / (60*60)+8;
		FILE* f=0;
		if( mTick[1] == h )
		{
			f = mFile[1];
		}
		else if( mTick[0] == h )
		{
			f = mFile[0];
		}
		else if( mTick[2] == h )
		{
			f = mFile[2];
		}
		if( ! f )
		{
			f = mDummy;
		}
		if( f )
		{
			char t[256];
			tm ttm;
			localtime_s(&ttm,&tick);
			strftime(t,256,"%Y-%m-%d %H:%M:%S",&ttm);
			t[255]=0;
			fprintf(f,"[%s]%s\n",t,msg);
		}
	}
	void Logger::UpdateFile()
	{
		time_t ch = (time(0)+8*60*60) / (60*60);
		if( ch != mTick[1] )
		{
			time_t ot[3];
			FILE* of[3];
			for(int i = 0; i < 3;i++ )
			{
				ot[i]=mTick[i];
				of[i]=mFile[i];
			}
			mTick[0] = ch-1;
			mTick[1] = ch;
			mTick[2] = ch+1;
			mFile[0] = mFile[1] = mFile[2] = 0;
			for(int i =0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{
					if( ot[j] > 0 && of[j] && ot[j] == mTick[i] )
					{
						mFile[i] = of[j];
						ot[j] = 0;
						of[j] = 0;
					}
				}
			}
			for(int i = 0;i<3;i++)
			{
				if( ! mFile[i] )
				{
					mFile[i] = OpenLogFile(mTick[i]);
				}
			}
			for(int i = 0;i<3;i++)
			{
				if( of[i] )
				{
					fclose(of[i]);
				}
			}
		}
	}
	FILE* Logger::OpenLogFile(time_t h)
	{
		std::string path = BinaryPath::path();
		time_t ch = h % 24;
		time_t tt=(max(0,h-8))*60*60;
		tm tmt;
		localtime_s(&tmt,&tt);
		char t[256];
		{
			strftime(t,256,"%Y",&tmt);
			std::stringstream ss;
			ss<<path<<"/"<<t;
			CreateDirectory(ss.str().c_str(),0);
		}
		{
			strftime(t,256,"%Y/%m",&tmt);
			std::stringstream ss;
			ss<<path<<"/"<<t;
			CreateDirectory(ss.str().c_str(),0);
		}
		{
			strftime(t,256,"%Y/%m/%d",&tmt);
			std::stringstream ss;
			ss<<path<<"/"<<t;
			CreateDirectory(ss.str().c_str(),0);
		}
		strftime(t,256,"%Y/%m/%d/%H.log",&tmt);
		std::stringstream ss;
		ss<<path<<"/"<<t;
		FILE* file;
		fopen_s(&file,ss.str().c_str(),"a");
		return file;
	}
}
LEUD::PoolMemory<LEUD::LogEvent>	LEUD::PoolMemoryBase<LEUD::LogEvent>::memPool("LogEvent");
