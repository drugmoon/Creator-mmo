#include "stdafx.h"
#include "leudgrid/TimerFix.h"

namespace LEUD
{
	TimerFix::TimerFix()
	{
		m_StartTime = std::clock();
	}
	void TimerFix::Restart()
	{
		m_StartTime = std::clock();
	}
	double TimerFix::Elapsed() const
	{
		return double(std::clock() - m_StartTime) / CLOCKS_PER_SEC;
	}
}