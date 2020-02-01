#pragma once
#include "EventQueueThread.h"
namespace LEUD
{
	class Logger : public EventQueueThread
	{
		time_t mTick[3];
		FILE*  mFile[3];
		FILE*  mDummy;
		time_t mUpdateTick;
	public:
		virtual int OnStart();
		virtual int OnStop();
		virtual void OnUpdate();

		void Log(const char* msg);
		void writeLog(const char* msg,time_t tick);
	private:
		void UpdateFile();
		FILE* OpenLogFile(time_t h);
	};
}