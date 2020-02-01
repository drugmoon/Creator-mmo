#pragma once
#include <ctime>

namespace OGSLib
{
	class TimerFix
	{
	public:
		TimerFix();
		void Restart();
		double Elapsed(bool restart=false);
	private:
		std::clock_t m_StartTime;
	};
	class TimerDuration
	{
		double mDuration;
		double mBase;
	public:
		TimerDuration(double base,double duration)
		{
			mDuration = duration;
			mBase = base;
		}
		void ResetBase(double base)
		{
			mBase = base;
		}
		void ResetDuration(double duration)
		{
			mDuration = duration;
		}
		bool TimeOut(double cur,bool resetbase=false)
		{
			if( mBase + mDuration > cur )
			{
				if( resetbase )
				{
					mBase = cur;
				}
				return true;
			}
			return false;
		}
	};
}