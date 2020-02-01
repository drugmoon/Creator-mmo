#pragma once

namespace LEUD
{
	class UDPSesson
	{
	private:
		SOCKET m_Socket;
		unsigned long m_Address;
		unsigned short m_Port;
	public:
		UDPSesson(unsigned long address,unsigned short port ):m_Socket(INVALID_SOCKET),m_Address(address),m_Port(port){
		}
		UDPSesson(unsigned short port ):m_Socket(INVALID_SOCKET),m_Address(0),m_Port(port){
		}
		UDPSesson( ):m_Socket(INVALID_SOCKET),m_Address(0),m_Port(0){
		}
		virtual ~UDPSesson(){
		}
	public:
		operator SOCKET( ){
			return m_Socket;
		}
	public:
		virtual BOOL	Start( unsigned long address, unsigned short port );
		virtual BOOL	Start( unsigned short port );
		virtual BOOL	Start( );
		virtual BOOL	Stop( );
	};
}