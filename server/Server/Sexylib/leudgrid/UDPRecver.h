#pragma once 
#include "ActionBase.h"
namespace LEUD
{
	template<class BLOCK>
	class UDPRecver : public ActionBase{
	public:
		typedef typename BLOCK Block_t;
		Block_t* m_Block;
		SOCKET   m_Socket;
		sockaddr_in m_Addr;
	public:
		void Initiate( SOCKET sock,Block_t* Block ){
			m_Socket = sock;
			m_Block = Block;
			m_Addr.sin_family = AF_INET;
			m_Addr.sin_addr.s_addr = 0;
			m_Addr.sin_port = 0;
			int	m_Recved = sizeof(m_Addr);
			DWORD dw=0;DWORD fg=0;
			WSABUF	 m_Buf;	m_Buf.buf = m_Block->data();m_Buf.len = static_cast<u_long>(m_Block->capacity()-m_Block->size());
			if( 0 != WSARecvFrom(m_Socket,&m_Buf,1,&dw,&fg,(sockaddr*)&m_Addr,&m_Recved,this,NULL) )
			{
				int error = GetLastError( );
				if( error != WSA_IO_PENDING )
					OnFaile( error );
			}
		}
	};
}