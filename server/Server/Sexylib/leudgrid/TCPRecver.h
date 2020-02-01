#pragma once 
#include "ActionBase.h"
namespace LEUD
{
	template<class BLOCK>
	class TCPRecver : public ActionBase{
	public:
		typedef typename BLOCK Block_t;
		Block_t* m_Block;
		SOCKET   m_Socket;
	public:
		void Initiate( SOCKET sock,Block_t* Block ){
			m_Socket = sock;
			m_Block = Block;
			DWORD dw=0;DWORD fg=0;
			WSABUF	 m_Buf;	m_Buf.buf = m_Block->data();m_Buf.len = static_cast<u_long>(m_Block->capacity()-m_Block->size());
			if( 0 != WSARecv(m_Socket,&m_Buf,1,&dw,&fg,this,NULL) ){
				int error = GetLastError( );
				if( error != ERROR_IO_PENDING )
					OnFaile( error );
			}
		}
	};
}