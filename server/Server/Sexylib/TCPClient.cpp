#include "StdAfx.h"
#include "leudgrid/tcpclient.h"

#define WM_NETWORK (WM_USER + 1234)

namespace LEUD
{
	void TCPClient::RegWindowClass()
	{
		WNDCLASSEX wcex={sizeof(WNDCLASSEX),0,(WNDPROC)WndProc,0,0,::GetModuleHandle(0),0,0,0,0,"TCPClientNetWnd",0};
		RegisterClassEx(&wcex);
	}
	void TCPClient::UnregWindowClass()
	{
		UnregisterClass("NetWnd",::GetModuleHandle(0));
	}
	TCPClient::TCPClient():m_socket(INVALID_SOCKET)
	{
		m_Address = 0;
		m_Port = 0;
		m_socket = INVALID_SOCKET;
		m_wnd = 0;
	}
	TCPClient::~TCPClient()
	{
		Stop();
	}
	void TCPClient::SetAddress(const char* lpszAddr)
	{
		m_Address = inet_addr(lpszAddr);
	}
	void TCPClient::SetPort(unsigned short port)
	{
		m_Port = htons(port);
	}
	bool TCPClient::Start()
	{
		m_wnd = CreateWindowEx(0,"TCPClientNetWnd"," ",0,0,0,0,0, HWND_MESSAGE,0,::GetModuleHandle(0),0);
#ifdef _WIN64
		LONG_PTR nResult = SetWindowLongPtr( m_wnd, GWLP_USERDATA, (LONG_PTR)this );
#else
		LONG_PTR nResult = SetWindowLongPtr( m_wnd, GWLP_USERDATA, (LONG)(LONG_PTR)this );
#endif 
		if( INVALID_SOCKET != (m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) )
		{
			WSAAsyncSelect(m_socket,m_wnd,WM_NETWORK,FD_READ|FD_WRITE|FD_CONNECT|FD_CLOSE);

			sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = m_Address;
			addr.sin_port = m_Port;
			if( 0 != connect(m_socket,(const sockaddr*)&addr,sizeof(SOCKADDR_IN))){
				if( WSAEWOULDBLOCK != WSAGetLastError() ){
					OnError( WSAGetLastError() );
					Stop();
					return false;
				}
			}
			return true;
		}
		OnError(WSAGetLastError());
		Stop();
		return false;
	}
	void TCPClient::Stop()
	{
		if( m_socket != INVALID_SOCKET )
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
		if( m_wnd )
		{
			DestroyWindow(m_wnd);
			m_wnd = NULL;
		}
	}
	void TCPClient::Send(const char* msg,size_t size){
		if( m_socket != INVALID_SOCKET ){
			send(m_socket,msg,(int)size,0);
		}
	}
	void TCPClient::OnSend()
	{
	}
	LRESULT CALLBACK TCPClient::WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
	{
		switch(message)
		{
		case WM_NETWORK:
			{
				TCPClient* This = (TCPClient*)(LONG_PTR)GetWindowLongPtr(hWnd,GWLP_USERDATA);
				if(This){
					if( WSAGETSELECTERROR(lParam) )
					{
						This->OnError( WSAGETSELECTERROR(lParam) );
					}else{
						switch(WSAGETSELECTEVENT(lParam))
						{
						case FD_READ:
							{
								This->OnRecv();
							}
							break;
						case FD_WRITE:
							{
								This->OnSend();
							}
							break;
						case FD_CONNECT:
							{
								This->OnConnect();
							}
							break;
						case FD_CLOSE:
							{
								This->Stop();
							}
							break;
						}
					}
				}
			}
			break;
		}
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
}
