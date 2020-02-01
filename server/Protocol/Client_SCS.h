#pragma once

#include "ProtocolHead.h"

namespace Client_SCS
{
	using namespace ProtocolHead;
}
namespace SCS_Client
{
	using namespace ProtocolHead;
}

enum Client_SCS_Protocol
{
	Client_SCS_Authenticate = 0x6000,
	SCS_Client_Authenticate = 0x6001,

	Client_SCS_ReqCharList = 0x6002,
	SCS_Client_ReqCharList = 0x6003,

	Client_SCS_SelChar = 0x6004,
	SCS_Client_SelChar = 0x6005,

	Client_SCS_CreateChar = 0x6006,
	SCS_Client_CreateChar = 0x6007,

	Client_SCS_DelChar = 0x6008,
	SCS_Client_DelChar = 0x6009,

	Client_SCS_ReqServerList = 0x6010,
	SCS_Client_ReqServerList = 0x6011,

	Client_SCS_SelectServer = 0x6012,
	SCS_Client_SelectServer = 0x6013,
};

namespace Client_SCS
{
	class AuthenticateREQ
	{
	public:
		static const unsigned short MSG_ID  = Client_SCS_Authenticate;
		std::string  mSessionID;
	public:
		AuthenticateREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const AuthenticateREQ& data)
	{
		stream<<data.MSG_ID<<data.mSessionID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,AuthenticateREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.mSessionID;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class ReqCharListREQ
	{
	public:
		static const unsigned short MSG_ID = Client_SCS_ReqCharList;
	public:
		ReqCharListREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const ReqCharListREQ& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,ReqCharListREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class SelCharREQ
	{
	public:
		static const unsigned short MSG_ID = Client_SCS_SelChar;
	public:
		SelCharREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const SelCharREQ& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,SelCharREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class CreateCharREQ
	{
	public:
		static const unsigned short MSG_ID = Client_SCS_CreateChar;
	public:
		CreateCharREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const CreateCharREQ& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,CreateCharREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class DelCharREQ
	{
	public:
		static const unsigned short MSG_ID = Client_SCS_DelChar;
	public:
		DelCharREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const DelCharREQ& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,DelCharREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class ReqServerListREQ
	{
	public:
		static const unsigned short MSG_ID = Client_SCS_ReqServerList;
	public:
		ReqServerListREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const ReqServerListREQ& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,ReqServerListREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class SelectServerREQ
	{
	public:
		static const unsigned short MSG_ID = Client_SCS_SelectServer;
	public:
		SelectServerREQ()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const SelectServerREQ& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,SelectServerREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
}
namespace SCS_Client
{
	class AuthenticateRES
	{
	public:
		static const unsigned short MSG_ID = SCS_Client_Authenticate;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
			result_error_session_id_error = 102,
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
	struct CharacterInfo
	{
		std::string Name;
	};
	typedef std::vector<CharacterInfo> CharacterInfoVector;
	class ReqCharListRES
	{
	public:
		static const unsigned short MSG_ID = SCS_Client_ReqCharList;
		int result;
		int mCharacterNumber;
		CharacterInfoVector mCharacterInfos;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
		};
	public:
		ReqCharListRES()
		{
			result = result_error_system;
			mCharacterNumber=0;
		}
	};
	template<class T> inline T& operator<<(T& stream,const ReqCharListRES& data)
	{
		stream<<data.MSG_ID<<data.result<<data.mCharacterNumber;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,ReqCharListRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result>>data.mCharacterNumber;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class SelCharRES
	{
	public:
		static const unsigned short MSG_ID = SCS_Client_SelChar;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
		};
	public:
		SelCharRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const SelCharRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,SelCharRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class CreateCharRES
	{
	public:
		static const unsigned short MSG_ID = SCS_Client_CreateChar;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
		};
	public:
		CreateCharRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const CreateCharRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,CreateCharRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class DelCharRES
	{
	public:
		static const unsigned short MSG_ID = SCS_Client_DelChar;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
		};
	public:
		DelCharRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const DelCharRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,DelCharRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class ReqServerListRES
	{
	public:
		static const unsigned short MSG_ID = SCS_Client_ReqServerList;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
		};
	public:
		ReqServerListRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const ReqServerListRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,ReqServerListRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
	class SelectServerRES
	{
	public:
		static const unsigned short MSG_ID = SCS_Client_SelectServer;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
		};
	public:
		SelectServerRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const SelectServerRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,SelectServerRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
}