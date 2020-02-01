#include "stdafx.h"
#include "leudgrid/MemoryPool.h"

namespace LEUD
{
	struct PoolMemoryCount
	{
		const char* name;
		size_t* max;
		size_t* free;
		size_t* used;
		size_t size;
	};
	PoolMemoryCount G_MPC[1000];
	int G_MPCP=0;
	void AddMemoryPoolItem(const char* name,size_t* max,size_t* free,size_t* used,size_t size)
	{
		static bool Inited=false;
		if( ! Inited )
		{
			Inited = true;
			G_MPCP = 0;
		}
		PoolMemoryCount* mpc = &G_MPC[G_MPCP];
		mpc->name = name;
		mpc->max = max;
		mpc->free = free;
		mpc->used = used;
		mpc->size = size;
		G_MPCP++;
	}
	size_t PoolMemoryGetMemorySize()
	{
		size_t s=0;
		for(int i = 0;i < G_MPCP;i ++)
		{
			PoolMemoryCount* pmc=&G_MPC[i];
			s += (*pmc->max)*pmc->size;
		}
		return s;
	}
	void PoolMemoryDumpInfo()
	{
		for(int i = 0;i < G_MPCP;i ++)
		{
			PoolMemoryCount* pmc=&G_MPC[i];
			if( pmc->name )
			{
				printf( "sizeof<%5d> max<%5d> free<%5d> used<%5d> name:%s\n",pmc->size,*pmc->max,*pmc->free,*pmc->used,pmc->name );
			}
			else
			{
				printf( "sizeof<%5d> max<%5d> free<%5d> used<%5d>",pmc->size,*pmc->max,*pmc->free,*pmc->used );
			}
		}
	}
}