#include "stdafx.h"
#include "leudgrid/EventQueueThread.h"
#include "leudgrid/Performance.h"
namespace LEUD
{
	EventQueueThread::EventQueueThread()
	{
		m_Name = 0;
		m_StopEvent = CreateEvent(NULL,FALSE,FALSE,"");
		m_WakeEvent = CreateEvent(NULL,FALSE,FALSE,"");
	}
	EventQueueThread::~EventQueueThread()
	{
		CloseHandle(m_StopEvent);
		CloseHandle(m_WakeEvent);
	}
	void EventQueueThread::Start(int argc,char** argv)
	{
		UINT   ThreadID;
		m_ThreadHandle = (HANDLE)_beginthreadex(NULL,0,MainThread,this,0,&ThreadID);
#ifdef _PERFORMANCE
		printf("%5d EventQueueThread::Start\n",ThreadID);
#endif
	}
	void EventQueueThread::Stop()
	{
		SetEvent(m_StopEvent);
		WaitForSingleObject(m_ThreadHandle,INFINITE);
		OnUpdate();
		OnUpdate();
		OnUpdate();
	}
	void EventQueueThread::Wake(){
		SetEvent(m_WakeEvent);
	}
	int EventQueueThread::OnStart()
	{
		return 0;
	}
	int EventQueueThread::OnStop()
	{
		return 0;
	}
	void EventQueueThread::OnUpdate()
	{
		HandEvent(this);
	}
	int  EventQueueThread::Run(){
#ifdef _PERFORMANCE
	PerfSigleNode pn;
	if( m_Name )
	{
		pn.start(m_Name);
	}
	else
	{
		pn.start("EventQueueThread");
	}
#endif
		__try{
			OnStart();
		}__except(UnhandledExceptionFilter(GetExceptionInformation())){
		}

		DWORD WaitTime = INFINITE;
		HANDLE event[MAXIMUM_WAIT_OBJECTS];
		DWORD  size = 0;
		event[size++] = m_StopEvent;
		event[size++] = m_WakeEvent;
		event[size++] = m_Event;
		while(WAIT_OBJECT_0 != WaitForMultipleObjects(size,event,FALSE,WaitTime)){
			__try{
#ifdef _PERFORMANCE
			pn.Call();
#endif
				OnUpdate();
#ifdef _PERFORMANCE
			pn.Return();
#endif
			}__except(UnhandledExceptionFilter(GetExceptionInformation())){
			}
		}

		__try{
			OnStop();
		}__except(UnhandledExceptionFilter(GetExceptionInformation())){
		}
#ifdef _PERFORMANCE
	pn.stop();
#endif
		return 0;
	}
	unsigned int __stdcall EventQueueThread::MainThread( void* data ){
		int ret;
		__try{
			EventQueueThread* This = (EventQueueThread*)data;
			ret = This->Run();
		}__except(UnhandledExceptionFilter(GetExceptionInformation())){
		}
		return ret;
	}
}
