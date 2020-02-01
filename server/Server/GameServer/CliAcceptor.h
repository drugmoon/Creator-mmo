#pragma once

#include "leudgrid/TCPAcceptEventQueue.h"

class CCliAcceptor :
	public LEUD::TCPAcceptEventQueue
{
	int mAcceptor_num;
public:
	bool mbOnService;
	CCliAcceptor(void);
	virtual void OnAccepted( SOCKET sock );
public:
	virtual BOOL Start(LEUD::Application* app);
};
