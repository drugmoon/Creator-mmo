#include "stdafx.h"
#include "leudgrid/DebugCrash.h"
#include "leudgrid/EventQueue.h"
#include "leudgrid/MonitorIOCP.h"
#include "leudgrid/Application.h"
#include "leudgrid/Performance.h"
#include "time.h"

static LEUD::CrashHandler ch;
namespace LEUD
{
	Application::Application(){
		m_StopEvent = CreateEvent(NULL,FALSE,FALSE,"");
		m_WakeEvent = CreateEvent(NULL,FALSE,FALSE,"");
		m_StartEvent = CreateEvent(NULL,FALSE,FALSE,"");
		m_HeartBeat = INFINITE;
		m_CommandEvent = CreateEvent(NULL,FALSE,FALSE,"");
		m_CommandRetEvent = CreateEvent(NULL,FALSE,FALSE,"");
		m_EventNum = 0;
		for(int i = 0;i < 64;i++)
		{
			m_Events[i] = 0;
		}
	}
	Application::~Application(){
		CloseHandle(m_StopEvent);
		CloseHandle(m_WakeEvent);
	}

	bool Application::AddEventHandle(HANDLE handle)
	{
		m_Events[m_EventNum++] = handle;
		return true;
	}
	bool Application::RemoveEventHandle(HANDLE handle)
	{
		for(int i = 0;i < m_EventNum;i ++ )
		{
			if( m_Events[i] == handle )
			{
				m_Events[i] = 0;
				return true;
			}
		}
		return true;
	}

	void Application::Start(int argc,char** argv){
		UINT   ThreadID;
		m_ThreadHandle = (HANDLE)_beginthreadex(NULL,0,MainThread,this,0,&ThreadID);
#ifdef _PERFORMANCE
		printf("%5d Application::Start\n",ThreadID);
#endif
#ifdef _PERFORMANCE
		SetThreadAffinityMask(m_ThreadHandle, 0x00000001); 
#endif
	}
	void Application::Stop(){
		SetEvent(m_StopEvent);
	}
	int Application::QueryStartState()
	{
		if( WAIT_OBJECT_0 == WaitForSingleObject(m_StartEvent,0) )
		{
			return 1;
		}
		return 0;
	}
	int Application::QueryQuitState()
	{
		if( WAIT_OBJECT_0 == WaitForSingleObject(m_ThreadHandle,0) )
		{
			return 0;
		}
		return 1;
	}

	void Application::Wake(){
		SetEvent(m_WakeEvent);
	}
	void Application::HeartBeat(DWORD heartbeat)
	{
		m_HeartBeat = heartbeat;
	}

	void FillEventList(HANDLE *ev,DWORD  &size,HANDLE * events,int num){
		for(int i = 0;i < num;i ++ )
		{
			if( events[i] > 0 )
			{
				if( size < MAXIMUM_WAIT_OBJECTS-1 )
				{
					ev[size++] = events[i];
				}
			}
		}
	}
	int  Application::Run(){
		WSADATA wsaData;WSAStartup( MAKEWORD(2,2), &wsaData );
		int ret=0;

		__try{
			ret = OnStart();
		}__except(UnhandledExceptionFilter(GetExceptionInformation())){
		}
		if( ret != 0 )
		{
			return 1;
		}
		SetEvent(m_StartEvent);

		HANDLE ev[MAXIMUM_WAIT_OBJECTS];
		DWORD  size = 0;
		ev[size++] = m_StopEvent;
		ev[size++] = m_WakeEvent;
		FillEventList(ev,size,m_Events,m_EventNum);
		time_t t = time(0);
		while(WAIT_OBJECT_0 != WaitForMultipleObjects(size,ev,FALSE,m_HeartBeat)){
			__try{
#ifdef _PERFORMANCE
				PerfManager::getTheOne()->IncrementFrameCounter();
#endif
				OnUpdate();
				RunCommand();
			}__except(UnhandledExceptionFilter(GetExceptionInformation())){
			}
		}

		__try{
			OnStop();
		}__except(UnhandledExceptionFilter(GetExceptionInformation())){
		}
		WSACleanup();
		return 0;
	}
	unsigned int __stdcall Application::MainThread( void* data ){
		Application* This = (Application*)data;
		int ret=0;
#ifdef _PERFORMANCE
		PerfManager::createTheOne();
#endif
		{
			PERF_NODE("App");
			ret = This->Run();
		}
#ifdef _PERFORMANCE
		char file[256];
		sprintf(file,"d:\\m%d.psf",time(0));
		PerfManager::getTheOne()->SaveDataFrame(file);
		PerfManager::getTheOne()->Reset();
#endif
		return ret;
	}
	void Application::RunCommand(){
		if( WAIT_OBJECT_0 == WaitForSingleObject(m_CommandEvent,0) )
		{
			std::string ret = OnCommand(m_Command);
			strncpy_s(m_CommandRetMsg,_countof(m_CommandRetMsg),ret.c_str(),1023);
			m_CommandRetMsg[1023]=0;
			SetEvent(m_CommandRetEvent);
		}
	}
	Application::ExeCommandReturn Application::ExeCommand(std::string cmd,std::string& result)
	{
		memset(m_Command,0,256);
		strncpy_s(m_Command,_countof(m_Command),cmd.c_str(),255);
		m_Command[255]=0;
		memset(m_CommandRetMsg,0,1024);
		SetEvent(m_CommandEvent);
		while( WAIT_OBJECT_0 != WaitForSingleObject(m_CommandRetEvent,0) )
		{
			Sleep(1000);
		}
		result = m_CommandRetMsg;
		return EXECOMMAND_RETURN_NEXT;
	}
	std::string Application::Help(int argc, char** argv)
	{
		return "";
	}
}
