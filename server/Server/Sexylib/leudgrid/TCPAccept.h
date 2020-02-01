#pragma once
#include <queue>
namespace LEUD
{
	class TCPAccept{
	protected:
		SOCKET m_Socket;
		unsigned long m_Address;
		unsigned short m_Port;
	public:
		TCPAccept(unsigned long address,unsigned short port ):m_Socket(INVALID_SOCKET),m_Address(address),m_Port(port){
		}
		TCPAccept( ):m_Socket(INVALID_SOCKET),m_Address(0),m_Port(0){
		}
		virtual ~TCPAccept(){
		}
	private:
		TCPAccept(const TCPAccept&);
		TCPAccept& operator=(const TCPAccept&);
	public:
		operator SOCKET( ){
			return m_Socket;
		}
	public:
		virtual void DoAccept( SOCKET sock ) = 0;
	public:
		virtual BOOL	Start( int backlog )=0;
		virtual BOOL	Start( int backlog,unsigned long address, unsigned short port )=0;
		virtual BOOL	Stop( )=0;
	};
	class TCPAcceptThread : public TCPAccept{
	private:
		HANDLE m_Thread;
	public:
		TCPAcceptThread( unsigned long address, unsigned short port );
		TCPAcceptThread( );
		virtual ~TCPAcceptThread( );
	public:
		virtual BOOL	Start( int backlog );
		virtual BOOL	Start( int backlog,unsigned long address, unsigned short port );
		virtual BOOL	Stop( );
	private:
		static 	unsigned int __stdcall ThreadFun( void* data );
	};
}