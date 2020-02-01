#pragma once
namespace LEUD
{
	class ActionBase : public OVERLAPPED{
	public:
		ActionBase() : OVERLAPPED(){
			Internal = 0;
			InternalHigh = 0;
			Offset = 0;
			OffsetHigh = 0;
			hEvent = 0;
		}
		virtual ~ActionBase(){
		}
	public:
		virtual int OnComplete( DWORD Number, DWORD Key ) = 0;
		virtual int OnFaile( DWORD Error ) = 0;
	};
}