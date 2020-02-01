#pragma once
namespace OGSLib
{
	class SubGift
	{
	public:
		int mBind;
		int mDBID;
		int mUsed;
		int mTypeID;
		int mNum;
		char mMsg[64];
	public:
		SubGift(void);
		virtual ~SubGift(void);
	};
}
