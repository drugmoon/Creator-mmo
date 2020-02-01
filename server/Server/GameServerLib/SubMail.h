#pragma once
namespace OGSLib
{
	class Mail;
	class SubMail
	{
	public:
		LEUD_INT64 mID;
		int mMailDate;
		int mRead;
		int mReceived;
		int mDeleted;
		int mNeedLv;
		int mGS;
		std::string mTitle;
		std::string mContent;
		std::map<int,int> mItems;
	public:
		SubMail(void);
		virtual ~SubMail(void);
		void BuildMail(Mail* ml,LEUD_INT64 id);
	};
}
