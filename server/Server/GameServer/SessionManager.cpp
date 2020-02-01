#include "StdAfx.h"
#include ".\sessionmanager.h"
#include "GameServerApp.h"

CSessionManager::CSessionManager()
{
	mUpdateTick = 0;
	mUpdateTickGap = 40;
	m_SessionVector.reserve(2000);
	m_StopEvent = CreateEvent(NULL,FALSE,FALSE,"");
	m_WakeEvent = CreateEvent(NULL,FALSE,FALSE,"");
}
CSessionManager::~CSessionManager()
{
	CloseHandle(m_StopEvent);
	CloseHandle(m_WakeEvent);
}
void CSessionManager::BreakNetVithLogic()
{
	for( SessionMap::iterator pos = m_Sessions.begin();
		pos != m_Sessions.end();
		++ pos )
	{
		//LEUD::TCPSessionMini* session = (LEUD::TCPSessionMini*)pos->second;
		pos->second->NetBreak();
	}
}
void  CSessionManager::AddSession( SOCKET sock,LEUD::TCPSessionManageEQ* session )
{
	LEUD::TCPSessionManager::AddSession(sock,session);
	{
		LEUD::CriticalSection::Lock lock( m_SessionVectorCS );
		m_SessionVector.push_back(session);
	}
}
void  CSessionManager::RemoveSession( SOCKET sock )
{
	LEUD::TCPSessionManageEQ* session = (LEUD::TCPSessionManageEQ*)FindSession(sock);
	if( session )
	{
		LEUD::CriticalSection::Lock lock( m_SessionVectorCS );
		m_SessionVector.erase(std::remove(m_SessionVector.begin(),m_SessionVector.end(),session),m_SessionVector.end());
	}
	LEUD::TCPSessionManager::RemoveSession(sock);
}

size_t CSessionManager::Update()
{
	size_t event_num;
	{
		PERF_NODE("TCPSessionManager");
		event_num = LEUD::TCPSessionManager::Update();
	}
	{
		PERF_NODE("TCPSessionManagerFlush");
		DWORD t = timeGetTime();
		if( mUpdateTick + mUpdateTickGap < t || mUpdateTick > t + 10000 )
		{
			mUpdateTick = t;
			for( SessionMap::iterator pos = m_Sessions.begin();
				pos != m_Sessions.end();
				++ pos )
			{
				//LEUD::TCPSessionMini* session = (LEUD::TCPSessionMini*)pos->second;
				pos->second->flushstream();
				//session->flush();
			}
			//Wake();
		}
	}
	return event_num;
}
void CSessionManager::OnUpdateFlush()
{
	LEUD::CriticalSection::Lock lock( m_SessionVectorCS );
	for( std::vector<LEUD::TCPSessionManageEQ*>::iterator pos = m_SessionVector.begin();
		pos != m_SessionVector.end();
		++ pos )
	{
		//LEUD::TCPSessionMini* session = (LEUD::TCPSessionMini*)(*pos);
		(*pos)->flush();
	}
}

void CSessionManager::Start(LEUD::Application* app)
{
	CGameServerApp* ap = static_cast<CGameServerApp*>(app) ;
	m_WorkThreadNum = GetPrivateProfileInt("net","work_thread_num",3,ap->m_ConfigFile.c_str());
	LoadConfig(ap);
	UINT   ThreadID;
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL,0,MainThread,this,0,&ThreadID);
#ifdef _PERFORMANCE
	printf("%5d CSessionManager\n",ThreadID);
#endif
	LEUD::TCPSessionManager::Start(app);
}
void CSessionManager::LoadConfig(CGameServerApp* ap)
{
	mUpdateTickGap = GetPrivateProfileInt("net","update_tick_gap",80,ap->m_ConfigFile.c_str());
}
int  CSessionManager::Run(){
#ifdef _PERFORMANCE
	PerfSigleNode pn;
	pn.start("CSessionManager");
#endif
	DWORD WaitTime = 80;
	HANDLE event[MAXIMUM_WAIT_OBJECTS];
	DWORD  size = 0;
	event[size++] = m_StopEvent;
	event[size++] = m_WakeEvent;
	while(WAIT_OBJECT_0 != WaitForMultipleObjects(size,event,FALSE,WaitTime)){
		__try{
#ifdef _PERFORMANCE
			pn.Call();
#endif
			OnUpdateFlush();
#ifdef _PERFORMANCE
			pn.Return();
#endif
		}__except(UnhandledExceptionFilter(GetExceptionInformation())){
		}
	}
#ifdef _PERFORMANCE
	pn.stop();
#endif
	return 0;
}
unsigned int __stdcall CSessionManager::MainThread( void* data ){
	int ret;
	__try{
		CSessionManager* This = (CSessionManager*)data;
		ret = This->Run();
	}__except(UnhandledExceptionFilter(GetExceptionInformation())){
	}
	return ret;
}
void CSessionManager::Stop()
{
	SetEvent(m_StopEvent);
	WaitForSingleObject(m_ThreadHandle,INFINITE);
	LEUD::TCPSessionManager::Stop();
}
void CSessionManager::Wake(){
	SetEvent(m_WakeEvent);
}
