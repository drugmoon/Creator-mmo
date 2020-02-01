#include "StdAfx.h"
#include "ContentAcceptor.h"
#include "GameServerApp.h"
#include "CliNetSession.h"
#include "SessionManager.h"
#include "MSTCPiP.h"
#include "leudgrid\Cryptography.h"
#include "miniz.h"
#include "GameServerLib/ScriptLua.h"
#include "GameServerLib/UtilString.h"

#define     KEEPALIVEINTERVAL                       1000
#define     KEEPALIVETIME                           18000

class ContentData{
public:
	mz_zip_archive* m_zip;
	char* m_zipbuf;
};

ContentAcceptor::ContentAcceptor()
{
	m_Listen_port = "9994";
	m_Listen_ip = "0.0.0.0";
	m_Backlog = "128";
	m_ThreadHandle = INVALID_HANDLE_VALUE;
	m_Socket = INVALID_SOCKET;
	m_zipname[0] = 0;
	mData = new ContentData;
	mData->m_zip = 0;
	mData->m_zipbuf = 0;
	mContentPlatformCheck = 0;
}
ContentAcceptor::~ContentAcceptor(){
	delete mData; mData = 0;
}
void ContentAcceptor::OnAccepted(SOCKET sock)
{
	CGameServerApp* app = static_cast<CGameServerApp*>(m_app);
	if (INVALID_HANDLE_VALUE != m_ThreadHandle)
	{
		closesocket(sock);
	}
	UINT   ThreadID;
	m_Socket = sock;
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, WorkMainThread, this, 0, &ThreadID);
}
bool ContentAcceptor::Extr(const char* file, char* dest, size_t& dest_size){
	if (mData->m_zip){
		size_t fl = strlen(file);
		char fi[256];
		OGSLib::UtilString::act_2_utf8(file, fl, fi, 256);
		//strcpy(fi,file);
		for (size_t i = 0; i<fl; i++){
			if (fi[i] == '/' || fi[i] == '\\'){
				fi[i] = '_';
			}
		}
		size_t ucom_size = 0;
		void* p = mz_zip_reader_extract_file_to_heap(mData->m_zip, fi, &ucom_size, 0);
		if (p){
			memcpy(dest, p, ucom_size);
			dest_size = ucom_size;
			free(p);
			return true;
		}
	}
	return false;
}
BOOL ContentAcceptor::Start(LEUD::Application* app)
{
	CGameServerApp* ap = static_cast<CGameServerApp*>(app);
	char temp[256];
	::GetPrivateProfileString("content", "listen_ip", "0.0.0.0", temp, 256, ap->m_ConfigFile.c_str());
	m_Listen_ip = temp;
	::GetPrivateProfileString("content", "port", "9994", temp, 256, ap->m_ConfigFile.c_str());
	m_Listen_port = temp;
	::GetPrivateProfileString("content", "backlog", "128", temp, 256, ap->m_ConfigFile.c_str());
	m_Backlog = temp;
	mContentPlatformCheck = GetPrivateProfileInt("content", "platform_mode", 0, ap->m_ConfigFile.c_str());
	return LEUD::TCPAcceptEventQueue::Start(app);
}
BOOL ContentAcceptor::Stop()
{
	closesocket(m_Socket);
	m_Socket = INVALID_SOCKET;
	if (m_ThreadHandle != INVALID_HANDLE_VALUE)
	{
		while (WAIT_OBJECT_0 != WaitForSingleObject(m_ThreadHandle, 0))
		{
			Sleep(100);
		}
		m_ThreadHandle = INVALID_HANDLE_VALUE;
	}
	BOOL ret = LEUD::TCPAcceptEventQueue::Stop();
	if (mData->m_zip){
		mz_zip_reader_end(mData->m_zip); delete mData->m_zip; mData->m_zip = 0;
		delete[] mData->m_zipbuf; mData->m_zipbuf = 0;
	}
	return ret;
}
bool ContentAcceptor::isLoaded(){
	if (mData->m_zip)return true;
	return false;
}
size_t ContentAcceptor::Update()
{
	if (m_ThreadHandle != INVALID_HANDLE_VALUE)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_ThreadHandle, 0))
		{
			m_ThreadHandle = INVALID_HANDLE_VALUE;
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
	}
	return LEUD::TCPAcceptEventQueue::Update();
}
class ContentDataEvent : public LEUD::EventBase, public LEUD::PoolMemoryBase< ContentDataEvent >
{
public:
	mz_zip_archive* m_zip;
	char* m_zipbuf;
	char m_name[256];
	ContentDataEvent(){
		m_zip = 0;
	}
	virtual void OnHanding(unsigned int queue_id, void* param){
		ContentAcceptor* aeq = (ContentAcceptor*)param;
		if (aeq->mData->m_zip){
			mz_zip_reader_end(aeq->mData->m_zip); delete aeq->mData->m_zip; aeq->mData->m_zip = 0;
			delete[] aeq->mData->m_zipbuf; aeq->mData->m_zipbuf = 0;
		}
		if (m_zip){
			aeq->mData->m_zip = m_zip;
			aeq->mData->m_zipbuf = m_zipbuf;
		}
		memcpy(aeq->m_zipname, m_name, 256);
		delete this;
	}
};
int  ContentAcceptor::WorkRun()
{
	if (mContentPlatformCheck == 1)
	{
		//open to tencent
		char tgw[256];
		memset(tgw, 1, 256);
		int tgwrecved = 0;
		while (true)
		{
			int rec = recv(m_Socket, tgw + tgwrecved, 1, 0);
			if (rec > 0)
			{
				tgwrecved += rec;
				if (tgwrecved>0 && tgw[tgwrecved - 1] == 0)
				{
					break;
				}
				else
				{
					printf("%c", tgw[tgwrecved - 1]);
				}
			}
			else
			{
				printf("tgw error");
				return 0;
			}
		}
	}
	char* data = 0;
	char head[256];
	int recved = 0;
	while (recved < sizeof(int)* 4)
	{
		int rec = recv(m_Socket, head + recved, sizeof(int)* 4 - recved, 0);
		if (rec > 0)
		{
			recved += rec;
		}
		else
		{
			printf("head error");
			return 0;
		}
	}
	int* h = (int*)head;
	int name_len = *(h);
	int name_len_tag = *(h + 1);
	int data_len = *(h + 2);
	int data_len_tag = *(h + 3);
	if (((~name_len) ^ 0x12345678) != name_len_tag)
	{
		char* ret = "name_len check error";
		send(m_Socket, ret, strlen(ret) + 1, 0);
		Sleep(10000);
		return 0;
	}
	if (((~data_len) ^ 0x87654321) != data_len_tag)
	{
		char* ret = "data_len check error";
		send(m_Socket, ret, strlen(ret) + 1, 0);
		Sleep(10000);
		return 0;
	}
	if (name_len > 256)
	{
		char* ret = "name_len too long";
		send(m_Socket, ret, strlen(ret) + 1, 0);
		Sleep(10000);
		return 0;
	}
	recved = 0;
	memset(head, 0, 256);
	while (recved < name_len)
	{
		int rec = recv(m_Socket, head + recved, name_len - recved, 0);
		if (rec > 0)
		{
			recved += rec;
		}
		else
		{
			return 0;
		}
	}
	for (int i = 0; i < name_len; i++)
	{
		char e = head[i];
		e = ~(e ^ 0x7A);
		head[i] = e;
	}
	recved = 0;
	data = new char[data_len];
	while (recved < data_len)
	{
		int rec = recv(m_Socket, data + recved, data_len - recved, 0);
		if (rec > 0)
		{
			recved += rec;
		}
		else
		{
			delete[] data;
			return 0;
		}
	}
	for (int i = 0; i < data_len; i++)
	{
		char e = data[i];
		e = ((~e) ^ 0x96);
		data[i] = e;
	}
	unsigned char key[16];
	LEUD::CryptographySimple::InitKey(key);
	unsigned char dict[256];
	unsigned char dict2[256];
	LEUD::CryptographySimple::InitDict(dict, dict2);
	LEUD::CryptographySimple::Decode((unsigned char*)data, data_len, key, dict2);
	{
		mz_zip_archive* zip = new mz_zip_archive;
		memset(zip, 0, sizeof(mz_zip_archive));
		if (mz_zip_reader_init_mem(zip, data, data_len, 0)){
			ContentDataEvent* cde = new ContentDataEvent;
			cde->m_zip = zip;
			cde->m_zipbuf = data;
			memcpy(cde->m_name, head, name_len);
			m_EQ->PushEvent(cde);
			char* ret = "ok";
			send(m_Socket, ret, 3, 0);
			Sleep(10000);
		}
		else{
			delete zip;
			delete[] data; data = 0;
			char* ret = "zip error";
			send(m_Socket, ret, strlen(ret) + 1, 0);
			Sleep(10000);
		}
	}
	return 0;
}
unsigned int __stdcall ContentAcceptor::WorkMainThread(void* data)
{
	int ret;
	__try{
		ContentAcceptor* This = (ContentAcceptor*)data;
		ret = This->WorkRun();
	}
	__except (UnhandledExceptionFilter(GetExceptionInformation())){
	}
	return ret;
}

LEUD::PoolMemory<ContentDataEvent>	LEUD::PoolMemoryBase<ContentDataEvent>::memPool("ContentDataEvent");
