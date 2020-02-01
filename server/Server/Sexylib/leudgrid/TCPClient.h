#pragma once

namespace LEUD
{

class TCPClient
{
protected:
	unsigned long					m_Address;
	unsigned short					m_Port;
	SOCKET							m_socket;
	HWND							m_wnd;
protected:
	TCPClient();
	virtual ~TCPClient();
	virtual void OnSend();
public:
	void SetAddress(const char* lpszAddr);
	void SetPort( unsigned short port );

	bool Start();
	void Stop();

	void Send(const char* msg,size_t size);

	static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
public:
	virtual void OnRecv()=0;
	virtual void OnConnect()=0;
	virtual void OnClose()=0;
	virtual void OnError(int err)=0;
public:
	static void RegWindowClass();
	static void UnregWindowClass();
};

}
	//说明

	//start成功后才有必要调用stop，而且必须要调用来关闭连接。
	//在两个地方最好马上去调用stop，
	//	1.Onerror
	//	2.Onclose
	//我们的例子里面是发送退出消息结束消息循环，因为在消息循环结束后会调用stop。
	//	多次调用stop是没有关系的
	//最好在协议里面服务器发一个消息过来，然后客户端开始关闭连接。

	//失败的start内部会调用OnError，然后自动调用stop，如果你在Onerror里面调用了Stop也没关系，因为这些函数里面stop是可以不断重复调用而不会出问题的。
	//一次成功的start后，会有
	//	1，1次OnConnect
	//	2，1次OnError，没连接到会有一个错误，比如服务器没开是10061，连接过多也是这个错误
	//这之后可能会有多次OnError，一般是这种情况：比如在没连接的，或者连接断掉后再发送消息都会是onerror来通知到，

	//OnClose只在对方关闭连接后通知，自己主动关闭socket是不会有这个通知的，这个一般不会用到，
	//在设计中优先考虑的是客户端先发起关闭连接，就是自己主动调用Stop来关闭连接，只有在极端情况下才需要服务器来关闭连接

	//TCPClient 类还很不成熟，需要不断修改
	//
	//发送缓冲区满了就会断线，因为是异步的，所以有这个问题，要特别处理send和onsend的函数，目前够用了，所以懒得写。
