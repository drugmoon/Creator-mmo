#pragma once 
#include <ctime>

namespace LEUD
{
	class TimerFix
	{
	public:
		TimerFix();
		void Restart();
		double Elapsed() const;
	private:
		std::clock_t m_StartTime;
	};
	class TimerGapChecker
	{
		double mTick;
		double mGap;
	public:
		TimerGapChecker(double tick,double gap):mTick(tick),mGap(gap){}
		bool check(double new_tick){if( mTick + mGap < new_tick ) { mTick=new_tick;return true; } return false;}
	};
}