#pragma once
#include "leudgrid/TCPSession.h"
#include "leudgrid/StreamBlockPRD.h"
#include "leudgrid/ToolsThread.h"
#include "leudgrid/ToolsDeque.h"
namespace LEUD
{
	class StreamFix;
	class EventQueue;
}
namespace LEUD
{
	class TCPRecverMini;
	class TCPSenderMini;
	class TCPSessionMini : public LEUD::TCPSessionManageEQ{
	protected:
		friend class TCPRecverMini;
		friend class TCPSenderMini;
		TCPRecverMini* mRecver;
		TCPSenderMini* mSender;
		int      m_HeadSize;
		char     m_InBuffer[1024*12];
		size_t    m_InBufferPos;
		size_t    m_InBufferUsed;
		size_t    m_InBufferSize;
		LEUD::CriticalSection m_OutBufCS;
		char     m_OutBuffer[1024*12];
		volatile long   m_OutBufferSendPos;
		volatile long   m_OutBufferPos;
		volatile long   m_OutBufferSize;
		volatile long   m_OutCount;
		volatile long   m_OutRetCount;
		volatile long   m_OutErrCount;
		volatile long   mSendProcess;
		volatile long   mRecvProcess;
		volatile long	mSendClosed;
		volatile long   mRecvClosed;
		volatile long   mCloseMsgSend;
		volatile long   mClosed;
		LEUD::CriticalSection m_OutStreamCS;
		LeudDeque<Stream512_t> m_OutDeque;
		Stream512_t* m_OutStream;
		short m_EndTag;
	public:
		static size_t mSendPoolSizeMax;
		TCPSessionMini();
		virtual ~TCPSessionMini();
		virtual void flush();
		virtual void Close();
		virtual void Open( SOCKET sock );
		virtual void flushstream();
		virtual void Send( Stream512_t* stream );
		template<class T> size_t Send_Message(T& res)
		{
			size_t bs = m_OutStream->size();
			{
				PERF_NODE("msg pack");
				(*m_OutStream)<<res<<m_EndTag;
			}
			size_t es = m_OutStream->size();
			if( es >= static_cast<size_t>(m_OutBufferSize) )
			{
				m_OutStream->Release();
				m_OutStream = new Stream512_t;
				if( m_HeadSize > 0 )
				{
					m_OutStream->size(m_HeadSize);
				}
				printf("outstream too big on msg %x\n",res.MSG_ID);
			}
			if( m_OutStream->size()>496 )
			{
				flushstream();
			}
			return es-bs;
		}
	public:
		virtual void DoClose( DWORD Error );
		virtual int DoRecv(int num);
		virtual int DoSend(int num);
	public:
		virtual void OnClose( DWORD Error )=0;
		virtual void OnMessage( const char* msg,size_t size )=0;
		virtual size_t BreakMessage( StreamFix& stream )=0;
		virtual void EncodeMessage( char* buf,size_t size ) = 0;
		virtual void DecodeMessage( char* buf,size_t size ) = 0;
		virtual void MakeHead( char* buf,unsigned short size ) = 0;
	};
}
