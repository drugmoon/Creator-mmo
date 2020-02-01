#pragma once
#include "ActionBase.h"
#include "MonitorIOCP.h"
namespace LEUD
{
	template<class STREAM>
	class TCPSenderStream : public ActionBase{
	public:
		STREAM* m_Stream;
		SOCKET   m_Socket;
	public:
		void Initiate( SOCKET sock,STREAM* Stream ){
			m_Socket = sock;
			m_Stream = Stream;
			m_Stream->AddRef();
			DWORD dw;
			WSABUF	 m_Buf[100];
			DWORD num = 0;
			for( Stream_t::iterator pos = m_Stream->begin();
				! pos.Is_Done();
				++ pos )
			{
				m_Buf[num].buf = pos->data();
				m_Buf[num].len = static_cast<u_long>(pos->size());
				num ++;
				if( num >= 100 ) break;
			}
			if( 0 != WSASend(m_Socket,m_Buf,num,&dw,0,this,NULL) ){
				int error = GetLastError( );
				if( error != ERROR_IO_PENDING )
					OnFaile( error );
			}
		}
	};
}