#pragma once

namespace LEUD
{
	class CriticalSection{
		CRITICAL_SECTION m_cs;
	public:
		CriticalSection(){
			InitializeCriticalSection(&m_cs);
		}
		~CriticalSection(){
			DeleteCriticalSection(&m_cs);
		}
	public:
		class Lock{
			CRITICAL_SECTION *m_cs;
		public:
			Lock(CriticalSection& cs){
				m_cs = &cs.m_cs;
				EnterCriticalSection( m_cs );
			}
			~Lock(){
				LeaveCriticalSection( m_cs );
			}
		};
	};
};