#pragma once

#include <vector>
#include <map>
#include "leudgrid/TCPSessionManager.h"
class CGameServerApp;
class CSessionManager : public LEUD::TCPSessionManager
{
	DWORD mUpdateTick;
	DWORD mUpdateTickGap;
	LEUD::CriticalSection m_SessionVectorCS;
	std::vector<LEUD::TCPSessionManageEQ*> m_SessionVector;
	HANDLE							m_ThreadHandle;
	HANDLE							m_StopEvent;
	HANDLE							m_WakeEvent;
public:
	CSessionManager();
	virtual ~CSessionManager();
	void BreakNetVithLogic();
	virtual size_t Update();
	virtual void OnUpdateFlush();
	virtual void Start(LEUD::Application* app);
	virtual void Stop();
	void LoadConfig(CGameServerApp* app);
	virtual void  AddSession( SOCKET sock,LEUD::TCPSessionManageEQ* session );
	virtual void  RemoveSession( SOCKET sock );
	void Wake();
private:
	int  Run();
	static unsigned int __stdcall MainThread( void* data );
};