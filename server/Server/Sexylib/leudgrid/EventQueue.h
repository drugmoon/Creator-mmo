#pragma once
#include "leudgrid/ToolsThread.h"
#include "leudgrid/ToolsDeque.h"
namespace LEUD
{
	class EventBase;
	class EventQueue{
		typedef LeudDeque<EventBase> Contain_t;
		Contain_t		m_Queue1;
		Contain_t		m_Queue2;
		Contain_t		*m_TheQueue;
		CriticalSection	m_QueueCS;
	public:
		HANDLE m_Event;
		unsigned int m_ID;
	public:
		EventQueue();
		virtual ~EventQueue();
	public:
		void PushEvent(EventBase* event);
		void PushEventFront(EventBase* event);
		size_t EventSize( );
		size_t HandEvent(void* param);
	};
}