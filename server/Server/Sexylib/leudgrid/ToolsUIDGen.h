#pragma once

namespace LEUD
{
	class UIDGen
	{
		unsigned int mCur;
	public:
		UIDGen(unsigned int base):mCur(base){}
		~UIDGen(){}

		unsigned int Generate(){return mCur++;}
	};
}