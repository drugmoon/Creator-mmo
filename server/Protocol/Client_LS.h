#pragma once

#include "ProtocolHead.h"

namespace Client_LS
{
	using namespace ProtocolHead;
}
namespace LS_Client
{
	using namespace ProtocolHead;
}

enum Client_LS_Protocol
{
	Client_LS_Authenticate = 0x5000,
	LS_Client_Authenticate = 0x5001,

	Client_LS_ListServerGroup = 0x5002,
	LS_Client_ListServerGroup = 0x5003,

	Client_LS_SelectServerGroup = 0x5004,
	LS_Client_SelectServerGroup = 0x5005,
};

namespace Client_LS
{
	class AuthenticateREQ
	{
	public:
		static const unsigned short MSG_ID  = Client_LS_Authenticate;
		std::string  mID;
		std::string  mPwd;
	public:
		AuthenticateREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const AuthenticateREQ& data)
	{
		stream<<data.MSG_ID<<data.mID<<data.mPwd;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,AuthenticateREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.mID>>data.mPwd;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class ListServerGroupREQ
	{
	public:
		static const unsigned short MSG_ID  = Client_LS_ListServerGroup;
	public:
		ListServerGroupREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const ListServerGroupREQ& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,ListServerGroupREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class SelectServerGroupREQ
	{
	public:
		static const unsigned short MSG_ID  = Client_LS_SelectServerGroup;
		std::string mServerGroupName;
	public:
		SelectServerGroupREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const SelectServerGroupREQ& data)
	{
		stream<<data.MSG_ID<<data.mServerGroupName;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,SelectServerGroupREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.mServerGroupName;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
}
namespace LS_Client
{
	class AuthenticateRES
	{
	public:
		static const unsigned short MSG_ID = LS_Client_Authenticate;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
			result_wrong_pwd = 103,
			result_no_user = 104,
		};
	public:
		AuthenticateRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const AuthenticateRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,AuthenticateRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	struct ServerGroupInfo
	{
		std::string Name;
		std::string IP;
		std::string Port;
	};
	typedef std::vector<ServerGroupInfo> ServerGroupInfoVector;
	class ListServerGroupRES
	{
	public:
		static const unsigned short MSG_ID = LS_Client_ListServerGroup;
		int result;
		int ServerGroupNumber;
		ServerGroupInfoVector ServerGroupInfos;
		enum result_type
		{
			result_succ=200,
			result_error_system = 201,
			result_updating = 203,
			result_full = 204,
		};
	public:
		ListServerGroupRES()
		{
			result = result_error_system;
			ServerGroupNumber = 0;
		}
	};
	template<class T> inline T& operator<<(T& stream,const ListServerGroupRES& data)
	{
		stream<<data.MSG_ID<<data.result<<data.ServerGroupNumber;
		for( ServerGroupInfoVector::const_iterator pos = data.ServerGroupInfos.begin();
			pos != data.ServerGroupInfos.end();
			++ pos )
		{
			stream<<pos->Name<<pos->IP<<pos->Port;
		}
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,ListServerGroupRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result>>data.ServerGroupNumber;
		for( int i = 0;i < data.ServerGroupNumber;i++ )
		{
			ServerGroupInfo info;
			stream>>info.Name>>info.IP>>info.Port;
			data.ServerGroupInfos.push_back(info);
		}
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class SelectServerGroupRES
	{
	public:
		static const unsigned short MSG_ID = LS_Client_SelectServerGroup;
		int result;
		enum result_type
		{
			result_succ=300,
			result_error_system = 301,
		};
	public:
		SelectServerGroupRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const SelectServerGroupRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,SelectServerGroupRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
}