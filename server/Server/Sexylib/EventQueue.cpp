#include "stdafx.h"
#include "leudgrid/EventQueue.h"
#include "leudgrid/EventBase.h"


namespace LEUD
{
	EventQueue::EventQueue(){
		m_ID = 0;
		m_Event = CreateEvent(NULL,FALSE,FALSE,NULL);
		{
			CriticalSection::Lock lock(m_QueueCS);
			m_TheQueue = &m_Queue1;
		}
	}
	EventQueue::~EventQueue(){
		CloseHandle(m_Event);
	}
	void EventQueue::PushEvent(EventBase* event){
		if( ! event ) return;
		{
			CriticalSection::Lock lock(m_QueueCS);
			m_TheQueue->push_back(event);
		}
		SetEvent(m_Event);
	}
	void EventQueue::PushEventFront(EventBase* event){
		if( ! event ) return;
		{
			CriticalSection::Lock lock(m_QueueCS);
			m_TheQueue->push_front(event);
		}
		SetEvent(m_Event);
	}
	size_t EventQueue::EventSize( ){
		CriticalSection::Lock lock(m_QueueCS);
		return m_TheQueue->size();
	}
	size_t EventQueue::HandEvent(void* param){
		size_t ret=0;
		Contain_t *Queue;
		{
			CriticalSection::Lock lock(m_QueueCS);
			if( m_TheQueue == &m_Queue1 ){
				m_TheQueue = &m_Queue2;
				Queue = &m_Queue1;
			}else{
				m_TheQueue = &m_Queue1;
				Queue = &m_Queue2;
			}
		}
		ret = Queue->size();
		while( ! Queue->empty() ){
			EventBase* event = Queue->front();
			Queue->pop_front();
			event->OnHanding(m_ID,param);
		}
		return ret;
	}
}