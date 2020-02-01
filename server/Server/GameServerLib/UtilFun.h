#pragma once
namespace OGSLib
{
	inline int RandRange(int max)
	{
		if( max <= 0 ) return 0;
		return rand()%max;
	}
	inline int RandRange2(int min,int max)
	{
		if( max <= min ) return min;
		return rand()%(max-min+1) + min;
	}
	inline int Round(double v)
	{
		if( v >= 0.0 )
		{
			return (int)(v+0.5);
		}
		else
		{
			return (int)(v-0.5);
		}
		return (int)v;
	}
}
