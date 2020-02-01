#include "StdAfx.h"
#include "SubFriend.h"

namespace OGSLib
{
SubFriend::SubFriend(void)
{
	mOnlineState = 0;
	mTitle = 0;
	mLevel = 0;
	mJob = 0;
	mGender = 0;
}

SubFriend::~SubFriend(void)
{
}

bool SubFriend::IsFriend(int nTitle)
{
	if (nTitle == 100)
		return true;
	return false;
}

bool SubFriend::IsMyEnemy(int nTitle)
{
	nTitle = nTitle & 0x0000FFFF;
	if (nTitle == 101)
	{
		return true;
	}
	return false;
}

bool SubFriend::IsOtherSideEnemy(int nTitle)
{
	nTitle = (nTitle & 0xFFFF0000) >> 16;
	if (nTitle == 101)
	{
		return true;
	}
	return false;
}

bool SubFriend::IsMyBlack(int nTitle)
{
	nTitle = nTitle & 0x0000FFFF;
	if (nTitle == 102)
	{
		return true;
	}
	return false;
}

bool SubFriend::IsOtherSideBlack(int nTitle)
{
	nTitle = (nTitle & 0xFFFF0000) >> 16;
	if (nTitle == 102)
	{
		return true;
	}
	return false;
}

}