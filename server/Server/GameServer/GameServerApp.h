#pragma once

#include <vector>
#include "leudgrid/Application.h"
#include "GameServerLib/GameServerLib.h"

class CSessionManager;
class CCliAcceptor;
class CSecurityAcceptor;
class DBManager;
class AuthManager;
class OCenterManager;
class GMAcceptor;
class ContentAcceptor;
class KuaFuManager;
class KuaFuService;
class OGSLib::GameServerLib;

class DBCallBackContent
{
public:
	int calltype;
	SOCKET sock;
	char loginid[256];
	char charname[256];
};

class CGameServerApp :
	public LEUD::Application,
	public OGSLib::GameServerLibListener
{
public:
	CGameServerApp(void);
	~CGameServerApp(void);
	size_t mConnMax;
public:
	void OnStoreSql(void* arg,const char* sql,int result,sexy_db_mysql::mem_db_res& res,long effect_row_num,long new_insert_id);
protected:
	virtual int OnStart();
	virtual int OnStop();
	virtual void OnUpdate();
	virtual std::string OnCommand(const char* command);

	bool CheckLimit(std::string ini);

	virtual ExeCommandReturn ExeCommand(std::string cmd,std::string& result);
public:
	virtual int GetKuaFuService(){return mKuaFuService;}
	virtual void ConnectService(const char* ip,const char* port);
public:
	virtual void LoadCharacter(const char* account,const char* chrname,const char* seedname,const char* client_ip,int GUID);
protected:
	virtual void LoadDemoCharacter(const char* account,const char* chrname,const char* client_ip,int GUID);

	virtual void SaveCharacter(OGSLib::DomainData* character,bool quit,int GUID);
	virtual void SaveDemoCharacter(OGSLib::DomainData* character,bool quit,int GUID);
	virtual void SaveKuafuCharacter(std::string& charID,std::string& account,int vcoin,int kuafuResult);

	virtual void LoadCouples(int GUID);
	virtual void SaveCouples(const OGSLib::ManagerCouple& couple_map,int GUID);

	virtual void LoadGuilds(int GUID);
	virtual void SaveGuilds(const OGSLib::ManagerGuild& guild_map,int GUID);

	virtual void LoadConsignment(int GUID);
	virtual void SaveConsignment(OGSLib::ManagerConsignment& consignment_manager,int GUID);

	virtual void LoadRelationCache(int GUID);
	virtual void SaveRelationCache(OGSLib::ManagerRelationCache& relationcache_manager,int GUID);

	virtual void LoadServerVar(int GUID);
	virtual void SaveServerVar(const OGSLib::GameServerLib& gsl,int GUID);

	void LoadMails(int GUID);
	void SaveMails(OGSLib::ManagerMail& mail_manager,int GUID);

	virtual void LoadAuto();
	virtual void game_exe_sql(const char* sql);

	virtual void RefreshVcoinCongzi(const std::string& account,const std::string& chrname);
	virtual void RefreshGift(const std::string& chrname);

	virtual void Log(const char* msg);

	virtual void LoadGMAllowIP();
	virtual void LoadConfig();
	virtual void LoadKernelConfig();
	virtual const char* LoadFile(const char* fn,size_t& size);

	virtual void AcceptOff();
	virtual void AcceptOn();
	virtual void BreakNet();

	virtual void SetAuthCheckIP(bool checkip);

	//void BatDegradeItem(bool checkonly);

	bool GetLocalIP(char* ip);
public:
	void ClientAuth(SOCKET sock,std::string& sessionID,int authseed,int pid,const std::string& idfa);
	int  UpdateTicket(SOCKET sock,std::string& sessionID,int authid);

#ifdef _PERFORMANCE
	void IncInMsgCount();
	void IncOutMsgCount();
	void LogOutMsg(int type,size_t size);
#endif
public:
	CSessionManager*	m_pSessionManager;
	OGSLib::GameServerLib* m_pGameServerLib;

	CCliAcceptor*		m_pCliAcceptor;
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	GMAcceptor*			m_pGMAcceptor;
	ContentAcceptor*	m_pContentAcceptor;
	DBManager *			m_pDBManager;
	//AuthManager *		m_pAuthManager;
	OCenterManager *    m_pOCenterManager;
	LEUD::Logger		m_Logger;
	KuaFuManager*       m_KuaFuManager;
	KuaFuService*		m_KuaFuService;
#endif
//#endif

	bool				m_ServerVarLoaded;
	bool				m_GuildsLoaded;
	bool                m_CouplesLoaded;

	std::string			m_ConfigFile;

	std::string			m_ServerName;
	int					m_ServerID;

	int					m_CharacterMaxNumber;

	int					mYouKeID;
	std::string         mYouKeMD5Pad;
	std::string			mYouKePrefix;
	std::string         mKuaFuMD5Pad;
	int					mKuaFuGUID;
	int					mKuaFuService;
	std::string			mKuaFuLocalIP;
	std::string			mKuaFuLocalPort;
	std::string			mKuaFuServerIP;
	std::string			mKuaFuServerPort;
	std::string			mKuaFuPTURL;
	std::string			mLocalPTID;
	std::string			mLocalServerID;

	int					mAuthType;
	int					mCongziType;

	char*				mRsaBuffer;
	size_t				mRsaBufferSize;

	time_t				mCheckLimitTime;

	unsigned char	mCryptKey[16];
	unsigned char	mCryptDict1[256];
	unsigned char	mCryptDict2[256];

	std::string		mAuthKey;

#ifdef _PERFORMANCE
	time_t				mCountMsgTick;
	int					mInMsgCount;
	int					mOutMsgCount;
	std::vector<int>    mInMsgCountHistory;
	std::vector<int>	mOutMsgCountHistory;
	std::map<int,int>   mOutMsgTypeCount;
	std::map<int,size_t>   mOutMsgTypeSize;
#endif
};
