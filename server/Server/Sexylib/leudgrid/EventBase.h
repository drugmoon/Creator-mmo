#pragma once

namespace LEUD
{
	class EventBase{
	public:
		EventBase* m_Next;
		EventBase():m_Next(0){}
		virtual ~EventBase(){}
        virtual void OnHanding(unsigned int queue_id, void* param)
        {
            delete this;
        }
	};
}