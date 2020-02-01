#pragma once

#include "EventQueue.h"


namespace LEUD
{
	class EventQueueThread : public LEUD::EventQueue
	{
	private:
		int								m_argc;
		char**							m_argv;
		HANDLE							m_ThreadHandle;
		HANDLE							m_StopEvent;
		HANDLE							m_WakeEvent;
	public:
		const char* m_Name;
		EventQueueThread();
		virtual ~EventQueueThread();
	public:
		virtual void Start(int argc,char** argv);
		virtual void Stop();
		void Wake();
	public:
		virtual int OnStart();
		virtual int OnStop();
		virtual void OnUpdate();
	private:
		int  Run();
		static unsigned int __stdcall MainThread( void* data );
	};
}