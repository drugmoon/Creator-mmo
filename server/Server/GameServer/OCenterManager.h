#pragma once
class OCenterEventQueueThread;
class PushOCenterEventClientAuth;
class PushOCenterEventFreshVcoin;
class PushOCenterEventUpdateTicket;
class PushOCenterEventLogCongzi;
class CGameServerApp;
class OCenterManager
{
	friend class OCenterEventQueueThread;
	friend class PushOCenterEventClientAuth;
	friend class PushOCenterEventFreshVcoin;
	friend class PushOCenterEventUpdateTicket;
	friend class PushOCenterEventLogCongzi;
	CGameServerApp* m_App;
	LEUD::DBModuleMysql* m_pDBModule;
	OCenterEventQueueThread* m_pDBThread;
	LEUD::EventQueue* m_pEventQueue;
	size_t m_BufferLen;
	char * m_Buffer1;
	char * m_Buffer2;
	time_t m_PingTime;
	bool  m_check_ip;
public:
	OCenterManager(CGameServerApp* app);
	virtual ~OCenterManager(void);

	void Start();
	void Stop();
	void Update();
	void checkIP(bool checkip);
	void DBUpVersion();
public:
	void push_client_auth(SOCKET sock,const std::string& sessionID,const std::string& loginIP/*,int authseed,int authid*/);
	void push_fresh_vcoin(const std::string& account,const std::string& chrname);
	void push_update_ticket(SOCKET sock,const std::string& sessionID,int authid);
	void push_log_congzi(const std::string& account,int number);
private:
	void PingDB();
	virtual int ClientAuth(const std::string& sessionID,const std::string& loginIP,std::string& account,int &tim/*,int authseed,int authid*/);
	virtual int update_ticket(const std::string& sessionID,int authid);
	virtual int FreshVcoin(const std::string& account,const std::string& chrname,int &gs);
	virtual int FreshVcoinAccu(const std::string& account,int &gs);
	virtual int LogCongzi(const std::string& account,int number);
};

