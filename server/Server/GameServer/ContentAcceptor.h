#pragma once

#include "leudgrid/TCPAcceptEventQueue.h"
class ContentData;

class ContentAcceptor :
	public LEUD::TCPAcceptEventQueue
{
	HANDLE							m_ThreadHandle;
	SOCKET							m_Socket;
public:
	char				m_zipname[256];
	ContentData*		mData;
	ContentAcceptor(void);
	~ContentAcceptor();
	int					mContentPlatformCheck;
	virtual void OnAccepted(SOCKET sock);
public:
	virtual BOOL Start(LEUD::Application* app);
	virtual BOOL Stop();
	virtual size_t Update();
	bool isLoaded();
	bool Extr(const char* file, char* dest, size_t& dest_size);
private:
	int  WorkRun();
	static unsigned int __stdcall WorkMainThread(void* data);
};
