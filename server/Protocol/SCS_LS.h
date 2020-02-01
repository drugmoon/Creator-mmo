#pragma once

#include "ProtocolHead.h"

namespace SCS_LS
{
	using namespace ProtocolHead;
}
namespace LS_SCS
{
	using namespace ProtocolHead;
}

enum SCS_LS_Protocol
{
	SCS_LS_Register_SelCharServer = 0x4000,	
	LS_SCS_Register_SelCharServer = 0x4001,
};

namespace SCS_LS
{
	class Register_SelCharServer
	{
	public:
		static const unsigned short MSG_ID = SCS_LS_Register_SelCharServer;
		std::string mServerName;
	public:
		Register_SelCharServer()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const Register_SelCharServer& data)
	{
		stream<<data.MSG_ID<<data.mServerName;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,Register_SelCharServer& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.mServerName;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
}
namespace LS_SCS
{
	class Register_SelCharServer
	{
	public:
		static const unsigned short MSG_ID = SCS_LS_Register_SelCharServer;
		int result;
	public:
		Register_SelCharServer()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const Register_SelCharServer& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,Register_SelCharServer& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
}