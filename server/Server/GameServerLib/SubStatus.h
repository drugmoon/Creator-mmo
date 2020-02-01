#pragma once

namespace OGSLib
{
	class SubStatus
	{
	public:
		int StatusID;
		int Param;
		int Flags;
		double Gap;
		double Duration;
		double Tick;
		SubStatus(void);
		virtual ~SubStatus(void);
	};

	class SubBuff
	{
	public:
		int id;
		double end_val;
		SubBuff(void);
		virtual ~SubBuff();
	};
}