#include "stdafx.h"
#include "SubMail.h"
#include "ManagerMail.h"
namespace OGSLib
{
	SubMail::SubMail(void)
	{
		mID=0;
		mRead=0;
		mReceived=0;
		mDeleted=0;
		mNeedLv=0;
	}
	SubMail::~SubMail(void)
	{
	}
	void SubMail::BuildMail(Mail* ml,LEUD_INT64 id)
	{
		mMailDate = ml->mDate;
		mID = id;
		mRead = 0;
		mReceived = 0;
		mDeleted = 0;
		mTitle = ml->mTitle;
		mContent= ml->mContent;
		mItems = ml->mItems;
		mGS= ml->mGS;
	}
}