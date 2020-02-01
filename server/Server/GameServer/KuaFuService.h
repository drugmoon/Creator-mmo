#pragma once

class CGameServerApp;
class KuaFuConnecter;
class KuaFuService;
struct KuaFuDBRes
{
	sexy_db_mysql::mem_db_res mDBRes;
	int mDBResLoadFieldsMax;
	int mDBResLoadRowsMax;
	int mDBResLoadFields;
	int mDBResLoadRows;
	std::string mDBResLoginID;
	std::string mDBResCharName;
};
class KuaFuNetSession:
	public LEUD::TCPSessionMini,
	public LEUD::PoolMemoryBase< KuaFuNetSession >
{
	bool mNetCloseBreak;
	friend class KuaFuService;
	std::string mServerName;
	std::string mIP;
	std::string mPort;
	bool mAuthed;
	char mDBResD[1024*12];
	std::map<std::string,KuaFuDBRes*> mKuaFuDBResMap;
public:
	CGameServerApp* m_app;
	KuaFuNetSession();
	virtual ~KuaFuNetSession();
	virtual void NetBreak(){}
	virtual void OnClose( DWORD Error );
	virtual void OnMessage( const char* msg,size_t size );
	size_t OnMessageOne(const char* msg,size_t size);
	virtual size_t BreakMessage( LEUD::StreamFix& stream );
	virtual void EncodeMessage( char* buf,size_t size );
	virtual void DecodeMessage( char* buf,size_t size );
	virtual void MakeHead( char* buf,unsigned short size );
	void LoadChar(const char* ticket,const char* loginid,const char* charid,const char* seedname);
	void SavePlayerData(const char* charid,const char* seekName,int pkType,int value);
	void SetGsParam(const char* k,const char* v);
	void DBResLoadError(const char* loginid,const char* charname);
	void ComeBackData(const std::string& winArea,const std::string& winGuild,const char* loginid,const char* charid,const char* seekName,int vcoin,int result);
private:
	template<class T> void SendMsg(T& res)
	{
		if( m_Socket == INVALID_SOCKET )return;
		if( ! mNetCloseBreak ){Send_Message(res);}
	}
};
struct KuaFuServerData
{
	friend class CCliNetSession;
	std::string mServerName;
	std::string mIP;
	std::string mPort;
	SOCKET	    mSock;
	std::vector<CCliNetSession*> mCallBackSessions;
	time_t      mConnTick;
	time_t		mFailedNum;
	KuaFuNetSession* mSession;
};
class KuaFuService
{
	friend class KuaFuConnecter;
	friend class CCliNetSession;
private:
	CGameServerApp* m_app;
	KuaFuConnecter* mConnecter;
	std::vector<KuaFuServerData> mServers;
	size_t mConnectServerKey;
public:
	KuaFuService(CGameServerApp* app);
	virtual ~KuaFuService();
	void Start();
	void Stop();
	void StopInport();
	void Update();
	void OnConnect(SOCKET sock);
	void OnConfail(DWORD Error);
	void CheckAndConnect();
	int AddServer(const char* servername,const char* ip,const char* port,CCliNetSession* callbacksession);
	int AddServer(const char* ip,const char* port);
	KuaFuServerData* FindServer(const char* ip,const char* port);
	KuaFuServerData* FindServer(const char* servername);
	void RemoveCallBackSession(const char* ip,const char* port,CCliNetSession* callbacksession);
};