#include "StdAfx.h"
#include "SubStatus.h"
namespace OGSLib
{
	SubStatus::SubStatus(void)
	{
		StatusID=0;
		Param=0;
		Gap=0;
		Duration=0;
		Tick=0;
	}
	SubStatus::~SubStatus(void)
	{
	}

	SubBuff::SubBuff(void)
	{
		id = 0;
		end_val = 0;
	}

	SubBuff::~SubBuff()
	{

	}
}