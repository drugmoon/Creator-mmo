#include "StdAfx.h"
#include "TimerFix.h"
namespace OGSLib
{
	TimerFix::TimerFix()
	{
		m_StartTime = std::clock();
	}
	void TimerFix::Restart()
	{
		m_StartTime = std::clock();
	}
	double TimerFix::Elapsed(bool restart)
	{
		std::clock_t c = std::clock();
		double re = double(c - m_StartTime) / CLOCKS_PER_SEC;
		if( restart ) 
			m_StartTime = c;
		return re;
	}
}