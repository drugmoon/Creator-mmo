#include "StdAfx.h"
#include "SubGift.h"
namespace OGSLib
{
	SubGift::SubGift(void)
	{
		mBind |= 0x1;
		mDBID=0;
		mUsed=0;
		mTypeID=0;
		mNum=1;
		mMsg[0]=0;
	}
	SubGift::~SubGift(void)
	{
	}
}