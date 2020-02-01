#pragma once
#include "ActionBase.h"
#include "MonitorIOCP.h"
namespace LEUD
{
	template<class BLOCK>
	class UDPSender : public ActionBase{
	public:
		typedef typename BLOCK Block_t;
		Block_t* m_Block;
		SOCKET   m_Socket;
		sockaddr_in m_Addr;
	public:
		void Initiate( SOCKET sock,Block_t* Block,unsigned long address,unsigned short port ){
			m_Socket = sock;
			m_Block = Block;
			m_Addr.sin_family = AF_INET;
			m_Addr.sin_addr.s_addr = address;
			m_Addr.sin_port = port;
			DWORD dw;
			WSABUF	 m_Buf;m_Buf.buf = Block->data();m_Buf.len = static_cast<u_long>(Block->size());
			if( 0 != WSASendTo(m_Socket,&m_Buf,1,&dw,0,(sockaddr*)&m_Addr,sizeof(m_Addr),this,NULL) )
			{
				int error = GetLastError();
				if( error != WSA_IO_PENDING )
					OnFaile(error);
			}
		}
	};
}