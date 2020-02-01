#include "stdafx.h"
#include "leudgrid/TCPSessionMini.h"
#include "leudgrid/MemoryBlock.h"
#include "leudgrid/StreamBlock.inl"
#include "leudgrid/EventBase.h"
#include "leudgrid/EventQueue.h"
#include "leudgrid/ActionBase.h"
#include "leudgrid/TCPSessionManager.h"
#include "leudgrid/StreamFix.h"
#include "leudgrid/MonitorIOCP.h"
#include "leudgrid/Performance.h"
namespace LEUD
{
	class TCPSessionMiniMsgEvent : public LEUD::EventBase,public PoolMemoryBase< TCPSessionMiniMsgEvent >
	{
		Block_t* m_block;
		const char* m_msg;
		size_t m_length;
		TCPSessionMini* m_session;
	public:
		TCPSessionMiniMsgEvent( Block_t* block,const char* msg,size_t length,TCPSessionMini* session ){
			m_block = block;
			m_msg = msg;
			m_length = length;
			m_session = session;
		}
		virtual void OnHanding(unsigned int queue_id,void* param){
			if( m_block )
			{
				m_session->OnMessage(m_block->data(),m_block->size());
				delete m_block;
			}else
			{
				m_session->OnMessage(m_msg,m_length);
				delete[] m_msg;
			}
			delete this;
		}
	};
	class TCPSessionMiniCloseEvent : public LEUD::EventBase,public PoolMemoryBase< TCPSessionMiniCloseEvent >
	{
		DWORD m_Error;
		TCPSessionMini* m_session;
	public:
		TCPSessionMiniCloseEvent(DWORD Error,TCPSessionMini* session){
			m_Error = Error;
			m_session = session;
		}
		virtual void OnHanding(unsigned int queue_id,void* param){
			m_session->OnClose(m_Error);
			delete this;
		}
	};
	class TCPRecverMini : public ActionBase,public PoolMemoryBase< TCPRecverMini >{
	public:
		SOCKET   m_Socket;
		TCPSessionMini* mSession;
	public:
		TCPRecverMini()
		{
			m_Socket = INVALID_SOCKET;
			mSession = 0;
		}
		void Initiate( SOCKET sock,TCPSessionMini* session,char* d,int len ){
			m_Socket = sock;
			mSession = session;
			DWORD dw=0;DWORD fg=0;
			if( len > 1024 ) len = 1024;
			WSABUF	 m_Buf;	m_Buf.buf = d;m_Buf.len = len;
			if( 0 != WSARecv(m_Socket,&m_Buf,1,&dw,&fg,this,NULL) ){
				int error = GetLastError( );
				if( error != ERROR_IO_PENDING )
					OnFaile( error );
			}
		}
		virtual int OnComplete( DWORD Number, DWORD Key )
		{
			mSession->DoRecv(Number);
			return 0;
		}
		virtual int OnFaile( DWORD Error )
		{
			InterlockedDecrement(&mSession->mRecvProcess);
			mSession->DoClose(Error);
			return 0;
		}
	};
	class TCPSenderMini : public ActionBase,public PoolMemoryBase< TCPSenderMini >{
	public:
		SOCKET   m_Socket;
		TCPSessionMini* mSession;
	public:
		TCPSenderMini()
		{
			m_Socket = INVALID_SOCKET;
			mSession = 0;
		}
		void Initiate( SOCKET sock,TCPSessionMini* session,char* d,int len ){
			m_Socket = sock;
			mSession = session;
			DWORD dw=0;DWORD fg=0;
			WSABUF	 m_Buf;	m_Buf.buf = d;m_Buf.len = len;
			InterlockedIncrement(&session->m_OutCount);
			if( 0 != WSASend(m_Socket,&m_Buf,1,&dw,0,this,NULL) ){
				int error = GetLastError( );
				if( error != ERROR_IO_PENDING )
				{
					OnFaile( error );
				}
				//else
				//{
				//	int i = 0;
				//}
			}
			//else
			//{
			//	int i = 0;
			//}
		}
		virtual int OnComplete( DWORD Number, DWORD Key )
		{
			InterlockedIncrement(&mSession->m_OutRetCount);
			mSession->DoSend(Number);
			return 0;
		}
		virtual int OnFaile( DWORD Error )
		{
			InterlockedIncrement(&mSession->m_OutErrCount);
			InterlockedCompareExchange(&mSession->mSendProcess,0,1);
			mSession->DoClose(Error);
			return 0;
		}
	};
	size_t TCPSessionMini::mSendPoolSizeMax=1024;
	void TCPSessionMini::Close()
	{
		m_EQ = 0;
		m_SessionManager->RemoveSession(*this);
		m_SessionManager = 0;
		delete mRecver;
		delete mSender;
		m_OutStream->Release();
		m_OutStream = 0;
		mRecver=0;
		mSender=0;
		while( ! m_OutDeque.empty() )
		{
			Stream512_t* t = m_OutDeque.front();
			m_OutDeque.pop_front();
			t->Release();
		}
		TCPSessionManageEQ::Close();
	}
	void TCPSessionMini::Open( SOCKET sock )
	{
		LEUD::TCPSession::Open( sock );
		m_OutCount=0;
		m_OutRetCount=0;
		m_OutErrCount=0;
		mSender = new TCPSenderMini;
		mSender->m_Socket=sock;
		mSender->mSession=this;
		mRecver = new TCPRecverMini;
		m_OutStream = new Stream512_t;
		if( m_HeadSize > 0 )
		{
			m_OutStream->size(m_HeadSize);
		}
		InterlockedIncrement(&mRecvProcess);
		mRecver->Initiate(sock,this,m_InBuffer,m_InBufferSize);
	}
	TCPSessionMini::TCPSessionMini()
	{
		m_InBufferPos=0;
		m_InBufferUsed=0;
		m_InBufferSize=1024*12;
		m_OutBufferPos=0;
		m_OutBufferSendPos=0;
		m_OutBufferSize = 1024*12;
		mRecver=0;
		mSender=0;
		mSendProcess=0;
		mRecvProcess=0;
		mSendClosed=0;
		mRecvClosed=0;
		mCloseMsgSend=0;
		mClosed=0;
		m_OutStream=0;
		m_EndTag=0x0303;
		m_HeadSize = 4;
	}
	TCPSessionMini::~TCPSessionMini()
	{
	}
	void TCPSessionMini::flushstream()
	{
		PERF_NODE_FUNC();
		size_t stream_size = m_OutStream->size();
		if( stream_size > 4 )
		{
			MakeHead( m_OutStream->begin()->data(),(unsigned short)(stream_size) );
			Send(m_OutStream);
			m_OutStream->Release();
			m_OutStream = new Stream512_t;
			if( m_HeadSize > 0 )
			{
				m_OutStream->size(m_HeadSize);
			}
		}
	}
	void TCPSessionMini::flush()
	{
		if( mSendClosed > 0 )
		{
			return;
		}
		if( mSendProcess > 0 )
		{
			return;
		}
		bool empty = true;
		{
			LEUD::CriticalSection::Lock lock(m_OutStreamCS);
			empty = m_OutDeque.empty();
		}
		if( ! empty )
		{
			if( 0 == InterlockedCompareExchange(&mSendProcess,1,0) )
			{
				DoSend(0);
				//mSender->InitiateEmpty(m_Socket,this,m_OutBuffer+m_OutBufferSendPos);
			}
		}
	}
	void TCPSessionMini::Send( Stream512_t* stream )
	{
		PERF_NODE_FUNC();
		if( mSendClosed > 0 )
		{
			return;
		}
		{
			LEUD::CriticalSection::Lock lock(m_OutStreamCS);
			if( m_OutDeque.size() <= mSendPoolSizeMax )
			{
				stream->AddRef();
				m_OutDeque.push_back(stream);
			}
			else
			{
				//printf("out buffer %d %d %d %d %d %d %d\n",mSendClosed,mSendProcess,m_OutBufferSendPos,m_OutBufferPos,m_OutCount,m_OutRetCount,m_OutErrCount);
			}
		}
	}
	int TCPSessionMini::DoSend(int num)
	{
		LEUD::CriticalSection::Lock lock(m_OutBufCS);
		m_OutBufferSendPos += num;
		if( mSendClosed > 0 )
		{
			InterlockedCompareExchange(&mSendProcess,0,1);
			DoClose(0);
			return 0;
		}
		if( m_OutBufferSendPos >= m_OutBufferPos )
		{
			m_OutBufferSendPos=0;
			m_OutBufferPos = 0;
			for(;;)
			{
				Stream512_t* stream=0;
				{
					LEUD::CriticalSection::Lock lock(m_OutStreamCS);
					if( ! m_OutDeque.empty() )
					{
						stream = m_OutDeque.front();
						if( stream )
						{
							if( m_OutBufferSize - m_OutBufferPos >= static_cast<long>(stream->size()) )
							{
								m_OutDeque.pop_front();
							}
							else
							{
								stream = 0;
							}
						}
					}
				}
				if( stream )
				{
					size_t out_size = stream->size();
					if( m_OutBufferSize - m_OutBufferPos >= static_cast<long>(out_size) )
					{
						stream->Out(m_OutBuffer+m_OutBufferPos,out_size);
						if( * stream )
						{
							EncodeMessage(m_OutBuffer+m_OutBufferPos+4,out_size-4);
							m_OutBufferPos += out_size;
						}
						else
						{
							int i = 0;
						}
					}
					else
					{
						int i = 0;
					}
					stream->Release();
				}
				else
				{
					break;
				}
			}
		}
		if( m_OutBufferSendPos >= m_OutBufferPos )
		{
			InterlockedCompareExchange(&mSendProcess,0,1);
			return 0;
		}
		mSender->Initiate(m_Socket,this,m_OutBuffer+m_OutBufferSendPos,m_OutBufferPos-m_OutBufferSendPos);
		return 1;
	}
	void TCPSessionMini::DoClose( DWORD Error )
	{
		InterlockedIncrement(&mSendClosed);
		InterlockedIncrement(&mRecvClosed);
		if( mRecvProcess <= 0 && mSendProcess <= 0 )
		{
			if( InterlockedIncrement(&mCloseMsgSend) == 1 )
			{
				m_EQ->PushEvent( new TCPSessionMiniCloseEvent(Error,this) );
			}
		}
	}
	int TCPSessionMini::DoRecv(int num)
	{
		if( mRecvClosed > 0 )
		{
			InterlockedDecrement(&mRecvProcess);
			DoClose(0);
			return 0;
		}
		if( num == 0 )
		{
			InterlockedDecrement(&mRecvProcess);
			DoClose(0);
			return 0;
		}
		m_InBufferPos+=num;
		if( m_InBufferPos >= m_InBufferSize )
		{
			m_InBufferPos=0;
			m_InBufferUsed =0;
		}
		StreamFix stm(m_InBuffer+m_InBufferUsed,m_InBufferPos-m_InBufferUsed,m_InBufferPos-m_InBufferUsed);
		size_t msglength;
		size_t pos = stm.seek();
		while( (msglength = this->BreakMessage(stm)) > 0 )
		{
			stm.seek(pos);
			if( stm && stm.size() >= msglength )
			{
				if( msglength <= Block_t::capacity() )
				{
					Block_t* block = new Block_t;
					stm.Out(block->data(),msglength);
					pos=stm.seek();
					block->size( msglength );
					if( stm )
					{
						this->DecodeMessage(block->data()+4,msglength-4);
						TCPSessionMiniMsgEvent* msg = new TCPSessionMiniMsgEvent(block,0,0,this);
						m_EQ->PushEvent( msg );
					}
					else
					{
						delete block;
					}
				}else
				{
					char* buffer = new char[msglength];
					stm.Out(buffer,msglength);
					pos=stm.seek();
					if( stm )
					{
						this->DecodeMessage(buffer+4,msglength-4);
						TCPSessionMiniMsgEvent* msg = new TCPSessionMiniMsgEvent(0,buffer,msglength,this);
						m_EQ->PushEvent( msg );
					}
					else
					{
						delete[] buffer;
					}
				}
			}
			else
			{
				break;
			}
		}
		m_InBufferUsed+=pos;
		if( m_InBufferUsed>=m_InBufferPos )
		{
			m_InBufferUsed=0;
			m_InBufferPos=0;
		}
		else
		{
			if( m_InBufferUsed > m_InBufferSize/3 )
			{
				memcpy(m_InBuffer,m_InBuffer+m_InBufferUsed,m_InBufferPos-m_InBufferUsed);
				m_InBufferPos=m_InBufferPos-m_InBufferUsed;
				m_InBufferUsed=0;
			}
		}
		mRecver->Initiate(m_Socket,this,m_InBuffer+m_InBufferPos,m_InBufferSize-m_InBufferPos);
		return 1;
	}
}
LEUD::PoolMemory<LEUD::TCPSessionMiniCloseEvent>	LEUD::PoolMemoryBase<LEUD::TCPSessionMiniCloseEvent>::memPool("TCPSessionMiniCloseEvent");
LEUD::PoolMemory<LEUD::TCPSessionMiniMsgEvent>		LEUD::PoolMemoryBase<LEUD::TCPSessionMiniMsgEvent>::memPool("TCPSessionMiniMsgEvent");
LEUD::PoolMemory<LEUD::TCPSenderMini>				LEUD::PoolMemoryBase<LEUD::TCPSenderMini>::memPool("TCPSenderMini");
LEUD::PoolMemory<LEUD::TCPRecverMini>				LEUD::PoolMemoryBase<LEUD::TCPRecverMini>::memPool("TCPRecverMini");
