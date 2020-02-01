#include "StdAfx.h"
#include ".\gameserverapp.h"

#include "leudgrid/ServerMain.h"
#include "leudgrid/Cryptography.h"
#include "leudgrid/ToolsPath.h"

#include "CliAcceptor.h"
#include "SessionManager.h"
#include "DBManager.h"
//#include "AuthManager.h"
#include "OCenterManager.h"
#include "CliNetSession.h"
#include "GMPlatform.h"
#include "ContentAcceptor.h"
#include "KuaFuManager.h"
#include "KuaFuService.h"

#include "GameServerLib/GameServerLib.h"
#include "GameServerLib/DomainData.h"
#include "GameServerLib/ManagerGuild.h"
#include "GameServerLib/ManagerCouple.h"
#include "GameServerLib/ManagerDeny.h"
#include "GameServerLib/UtilString.h"
#include "GameServerLib/GMCommand.h"
#include "GameServerLib/SubItem.h"
#include "GameServerLib/ManagerItemDef.h"

#include <crtdbg.h>
#include <fstream>

CGameServerApp::CGameServerApp(void)
{
	LEUD::SetApplication( this );
	mYouKeID = 100;
	mKuaFuGUID = -100;
	mKuaFuService = 0;
	mAuthType = 0;
	mConnMax=2000;
	m_ServerVarLoaded=false;
	m_GuildsLoaded=false;
	m_CouplesLoaded=false;
	mCheckLimitTime=0;
	LEUD::CryptographySimple::InitKey(mCryptKey);
	LEUD::CryptographySimple::InitDict(mCryptDict1,mCryptDict2);
#ifdef _PERFORMANCE
	mCountMsgTick=0;
	mInMsgCount=0;
	mOutMsgCount=0;
	mInMsgCountHistory.reserve(3600);
	mOutMsgCountHistory.reserve(3600);
#endif
}

CGameServerApp::~CGameServerApp(void)
{
}
#ifdef _PERFORMANCE
void CGameServerApp::IncInMsgCount()
{
	mInMsgCount++;
}
void CGameServerApp::IncOutMsgCount()
{
	mOutMsgCount++;
}
void CGameServerApp::LogOutMsg(int type,size_t size)
{
	mOutMsgTypeCount[type] = mOutMsgTypeCount[type]+1;
	mOutMsgTypeSize[type] = mOutMsgTypeSize[type] + size;
}
#endif
static std::string FindConfigFile()
{
	char path[256];
	::GetCurrentDirectory(256,path);
	char file[256];
	_snprintf_s(file,_countof(file),255,"%s\\GameServer.ini",path);
	if( PathFileExists(file) )
	{
		return file;
	}
	_snprintf_s(file,_countof(file),255,"%s\\GameServer.ini",LEUD::BinaryPath::path().c_str());
	if( PathFileExists(file) )
	{
		return file;
	}
	return "c:\\GameServer.ini";
}
bool CGameServerApp::CheckLimit(std::string ini)
{
	LEUD::RETVALUE lResult = LEUD::SUCCESS;
	char*	lSoftwareKeyStringFromIniFile=NULL;
	lResult = LEUD::CSoftwareKey::GetSoftwareKeyStringFromIniFile(
		ini.c_str(), 
		&lSoftwareKeyStringFromIniFile);
	bool lIsValid = false;
	lResult = LEUD::CSoftwareKey::VerifySoftwareKey(
		lSoftwareKeyStringFromIniFile, 
		&lIsValid);
	delete[] lSoftwareKeyStringFromIniFile;lSoftwareKeyStringFromIniFile=0;
	if (!lIsValid)
	{
		return false;
	}
	return true;
}
int CGameServerApp::OnStart()
{
	int ret = 0;

	::CoInitialize(0);
	srand(unsigned int(time(0)));

	m_ConfigFile = FindConfigFile();
	printf("\nconfig file : %s\n",m_ConfigFile.c_str());
	char temp[256];
	char temp2[256];
	GetPrivateProfileString("server","server_name","GS",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	m_ServerName=temp2;
	m_ServerID=GetPrivateProfileInt("server","server_id",1000,m_ConfigFile.c_str());
	m_CharacterMaxNumber=GetPrivateProfileInt("server","character_max_number",6,m_ConfigFile.c_str());
	_snprintf_s(temp2,_countof(temp2),255,"%d%d",rand()%10000,rand()%10000);
	temp2[255]=0;
	mYouKeMD5Pad=temp2;
	GetPrivateProfileString("demo","prefix","YouKe",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mYouKePrefix=temp2;
	mAuthType = GetPrivateProfileInt("server","auth_type",0,m_ConfigFile.c_str());
	mCongziType = GetPrivateProfileInt("server","congzi_type",0,m_ConfigFile.c_str());

	GetPrivateProfileString("net", "authkey", "", temp, 256, m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp, strlen(temp), temp2, 256);
	mAuthKey = temp2;
	
	mKuaFuService = GetPrivateProfileInt("kuafu","kuafu_service",0,m_ConfigFile.c_str());
	GetPrivateProfileString("kuafu","md5_pad","kasumika",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mKuaFuMD5Pad=temp2;
	GetPrivateProfileString("kuafu","local_ip","127.0.0.1",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mKuaFuLocalIP=temp2;
	char ip[128];
	if(mKuaFuLocalIP.empty() || mKuaFuLocalIP == "127.0.0.1")
	{
		if(GetLocalIP(ip))
		{
			mKuaFuLocalIP = ip;
		}
	}
	GetPrivateProfileString("kuafu","local_port","",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mKuaFuLocalPort=temp2;
	if(mKuaFuLocalPort.empty())
	{
		GetPrivateProfileString("net","port","9994",temp,256,m_ConfigFile.c_str());
		OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
		mKuaFuLocalPort=temp2;
	}
	GetPrivateProfileString("kuafu","kuafu_ip","127.0.0.1",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mKuaFuServerIP=temp2;
	GetPrivateProfileString("kuafu","kuafu_port","7863",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mKuaFuServerPort=temp2;
	GetPrivateProfileString("kuafu","kuafu_pt_url","",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mKuaFuPTURL=temp2;
	GetPrivateProfileString("kuafu","local_pt_id","",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mLocalPTID=temp2;
	GetPrivateProfileString("kuafu","local_server_id","",temp,256,m_ConfigFile.c_str());
	OGSLib::UtilString::act_2_utf8(temp,strlen(temp),temp2,256);
	mLocalServerID=temp2;
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_Logger.Start(0,0);
#endif
//#endif
	mRsaBufferSize=1024*1024*10;
	mRsaBuffer=new char[mRsaBufferSize];

	HeartBeat(20);
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pContentAcceptor = new ContentAcceptor;
	m_pContentAcceptor->Start(this);
#endif
//#endif
#ifdef NDEBUG
#ifndef LIMIT_RELEASE
	while (!m_pContentAcceptor->isLoaded())
	{
		m_pContentAcceptor->Update();
		Sleep(100);
	}
#endif
#endif

	m_pCliAcceptor = new CCliAcceptor;
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pGMAcceptor = new GMAcceptor;
#endif
//#endif
	m_pSessionManager = new CSessionManager;
	m_pGameServerLib = new OGSLib::GameServerLib;
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager = new DBManager(this);
	m_pOCenterManager = new OCenterManager(this);

	m_KuaFuManager = new KuaFuManager();
	if(mKuaFuService)
	{
		m_KuaFuService = new KuaFuService(this);
	}
	else
	{
		m_KuaFuService = 0;
	}
#endif
//#endif

//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	if( ! m_pDBManager->Start() )
	{
		return 1;
	}
	if( mAuthType == 1 || mAuthType == 2 || mCongziType == 1 || mCongziType == 2)
	{
		m_pOCenterManager->Start();
	}

#endif
//#endif
	CHECK_DEBUG;
	m_pGameServerLib->SetListener(this);
	m_pGameServerLib->Start(m_ConfigFile.c_str());
	m_pSessionManager->Start(this);
#ifdef NDEBUG
#ifndef LIMIT_RELEASE
	if( ! CheckLimit(m_ConfigFile) ) return ret;
#endif
#endif
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	while( ! (m_ServerVarLoaded && m_GuildsLoaded && m_CouplesLoaded ) )
	{
		m_pDBManager->Update();
	}
#endif
//#endif
	m_pGameServerLib->Start2();
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	if( ! m_pGMAcceptor->Start(this) )
	{
		printf("failed listen on master port!");
	}
	if(mKuaFuService && m_KuaFuService)
	{
		m_KuaFuService->Start();
	}
#endif
//#endif
	if ( ! m_pCliAcceptor->Start(this) )
	{
		printf("failed listen on net port!");
	}
	LoadConfig();
	LoadKernelConfig();
	return ret;
}
int CGameServerApp::OnStop()
{
	int ret = 0;

	printf("[Turn off accept new connection.");
	m_pCliAcceptor->mbOnService = false;
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pGMAcceptor->mbOnService = false;
	if( m_KuaFuService)m_KuaFuService->StopInport();
#endif
//#endif
	printf("done]\n");
	printf("[Break net and logic module.");
	m_pSessionManager->BreakNetVithLogic();
	printf("done]\n");
	if(mAuthType == 2 || mCongziType == 2 )
	{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
		printf("[disconnecting with ocenter database.");
		m_pOCenterManager->Stop();
		printf("done]\n");
#endif
//#endif
	}
//	if( mAuthType == 3 )
//	{
////#ifdef NDEBUG
//#ifndef LIMIT_RELEASE_2
//		printf("[disconnecting with auth database.");
//		m_pAuthManager->Stop();
//		printf("done]\n");
//#endif
////#endif
//	}
	printf("[stoping game logic.");
	m_pGameServerLib->StopAllSession();
	while( m_pGameServerLib->GetSessionNumber() )
	{
		OnUpdate();
	}
	printf(" all session closed.please wait ");
	m_pGameServerLib->Stop();
	printf("done]\n");
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	printf("[disconnecting with database.");
	m_pDBManager->Stop();
	printf("done]\n");

	printf("done]\n");
#endif
//#endif

	printf("All data is saved.kill or wait it close self\n");
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	printf("[Stop GM Acceptor.");
	m_pGMAcceptor->Stop();
	printf("done]\n");
#endif
//#endif
	printf("[Stop listening net port.");
	m_pCliAcceptor->Stop();
	printf("done]\n");
	printf("[Stop net connections.if too long kill process is safe.");
	m_pSessionManager->Stop();
	printf("done]\n");
#ifndef LIMIT_RELEASE_2
	if( m_KuaFuService)
	{
		printf("[Stop net kuafu service.");
		m_KuaFuService->Stop();
		printf("done]\n");
	}
#endif

	delete m_pCliAcceptor;m_pCliAcceptor=0;
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	delete m_pGMAcceptor;m_pGMAcceptor=0;
#endif
//#endif
	delete m_pSessionManager;m_pSessionManager=0;
	delete m_pGameServerLib;m_pGameServerLib=0;
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	delete m_pDBManager;m_pDBManager=0;
	//delete m_pAuthManager;m_pAuthManager=0;
	delete m_pOCenterManager;m_pOCenterManager=0;
	delete m_KuaFuManager;m_KuaFuManager=0;
	if(m_KuaFuService)
	{
		delete m_KuaFuService;m_KuaFuService=0;
	}
	m_Logger.Stop();
#endif
//#endif

	delete[] mRsaBuffer;mRsaBuffer=0;

//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pContentAcceptor->Stop();
	delete m_pContentAcceptor;m_pContentAcceptor=0;
#endif
//#endif
#ifdef _PERFORMANCE
	{
		char file_name[256];
		sprintf(file_name,"d:\\msgcount%d.log",time(0));
		FILE* file=fopen(file_name,"w");
		int incount = 0;
		int inti = 0;
		int outcount = 0;
		int outti = 0;
		int in = 0;
		int out = 0;
		for(size_t i = 0;i < mInMsgCountHistory.size();i++)
		{
			in = mInMsgCountHistory[i];
			out = mOutMsgCountHistory[i];
			incount += in;
			outcount += out;
			if( in > 0 )
			{
				inti ++;
			}
			if( out > 0 )
			{
				outti ++;
			}
			fprintf(file,"i:%5d,o:%5d\n",in,out);
		}
		fprintf(file,"isun:%d,iavg:%d,osun:%d,oavg:%d\n",incount,incount/inti,outcount,outcount/outti);
		fprintf(file,"time:%d,imsgtime:%d,omsgtime:%d\n",mInMsgCountHistory.size(),inti,outti);
		fclose(file);
	}
	{
		char file_name[256];
		sprintf(file_name,"d:\\msgtype%d.log",time(0));
		FILE* file = fopen(file_name,"w");
		for(std::map<int,int>::iterator pos = mOutMsgTypeCount.begin();
			pos != this->mOutMsgTypeCount.end();
			++ pos)
		{
			fprintf(file,"t:%4x,c:%20d,s:%5d\n",pos->first,pos->second,mOutMsgTypeSize[pos->first]/pos->second);
		}
		fclose(file);
	}
#endif
	::CoUninitialize();
	return ret;
}

void CGameServerApp::OnUpdate()
{
	PERF_NODE_FUNC();
	{
		PERF_NODE("CheckLimit");
#ifdef NDEBUG
#ifndef LIMIT_RELEASE
		if( mCheckLimitTime + 600 < time(0) )
		{
			mCheckLimitTime = time(0);
			if( ! CheckLimit(m_ConfigFile) ) return;
		}
#endif
#endif
	}
	{
		PERF_NODE("msg_count");
#ifdef _PERFORMANCE
		time_t t = time(0);
		if( mCountMsgTick == 0 )
		{
			mCountMsgTick = t;
		}
		for(time_t s=mCountMsgTick;s<t;s++)
		{
			mCountMsgTick=t;
			mInMsgCountHistory.push_back(mInMsgCount);
			mOutMsgCountHistory.push_back(mOutMsgCount);
			mInMsgCount = 0;
			mOutMsgCount = 0;
		}
		mCountMsgTick = t;
#endif
	}
	{
		PERF_NODE("m_pCliAcceptor");
		m_pCliAcceptor->Update();
	}
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	{
		PERF_NODE("m_pGMAcceptor");
		m_pGMAcceptor->Update();
	}
	{
		PERF_NODE("m_pContentAcceptor");
		m_pContentAcceptor->Update();
	}
#endif
//#endif
	{
		PERF_NODE("m_pSessionManager");
		m_pSessionManager->Update();
	}
	{
		PERF_NODE("m_pGameServerLib");
		m_pGameServerLib->Update();
	}
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	//{
	//	PERF_NODE("m_pAuthManager");
	//	m_pAuthManager->Update();
	//}
	//m_KuaFuManager->UpdateAuthTick();
	{
		PERF_NODE("m_pDBManager");
		m_pDBManager->Update();
	}
	{
		PERF_NODE("m_pOCenterManager");
		m_pOCenterManager->Update();
	}

	m_KuaFuManager->Update();
	if(m_KuaFuService)
	{
		m_KuaFuService->Update();
	}
#endif
//#endif
}
//void CGameServerApp::BatDegradeItem(bool checkonly)
//{
////#ifdef NDEBUG
//#ifndef LIMIT_RELEASE_2
//	return ;
//#endif
////#endif
//	for(int i = 1;i<=5;i++)
//	{
//		if( ! this->m_pGameServerLib->GetItemDefineManager()->mSteelArray[i] )
//		{
//			return;
//		}
//	}
//	std::string logfile = LEUD::BinaryPath::path() +"\\batdegradeitem.log";
//	FILE* file = 0;
//	fopen_s(&file,logfile.c_str(),"w");
//	OGSLib::ItemDef** steel = this->m_pGameServerLib->GetItemDefineManager()->mSteelArray;
//	std::string mapinfofile = LEUD::BinaryPath::path() +"\\batdegradeitem.txt";
//	std::ifstream ifs(mapinfofile.c_str());
//	std::string line;
//	if( ifs )
//	{
//		while( std::getline(ifs,line) )
//		{
//			if( line.length()> 0 )
//			{
//				if( line.substr(0,1) == "#" || line.substr(0,1) == ";" )
//				{
//					continue;
//				}
//				GS_Client::ListCharacterRes res;
////#ifdef NDEBUG
//#ifndef LIMIT_RELEASE_2
//				m_pDBManager->ClientListCharacter(0,line,res);
//				for(std::vector<GS_Client::ListCharacterRes::character>::iterator pos = res.charlist.begin();
//					pos != res.charlist.end();
//					++pos )
//				{
//					OGSLib::DomainData chr;
//					if( 0 == m_pDBManager->DoLoadCharacter(line.c_str(),pos->name.c_str(),pos->seedname.c_str(),chr,"player") )
//					{
//						if( chr.last_login_time + 60*60*26 < time(0) )
//						{
//							continue;
//						}
//						bool moded = false;
//						for(OGSLib::DomainData::ItemMap::iterator p = chr.mItemList.begin();
//							p != chr.mItemList.end();
//							++ p)
//						{
//							OGSLib::ItemDef* id = this->m_pGameServerLib->GetItemDefineManager()->GetItemDefine(p->mTypeID);
//							if( id )
//							{
//								if( OGSLib::ItemDef::IsEquipment(id->SubType) )
//								{
//									if( checkonly && (p->mLevel == 8||p->mLevel == 6||p->mLevel == 4))
//									{
//										printf("%s %s %d %d\n",line.c_str(),pos->name.c_str(),p->mTypeID,p->mLevel);
//									}
//									if(p->mLevel == 8||p->mLevel == 6||p->mLevel == 4)
//									{
//										fprintf(file,"%s %s %d %d %d %d %d\n",line.c_str(),pos->name.c_str(),p->mTypeID,p->mPosition,p->mLevel,p->mCreateTime,p->mIdentId);
//									}
//									if( p->mLevel == 4 )
//									{
//										p->mLevel = 0;
//										p->mUpdAC = 0;
//										p->mUpdMAC = 0;
//										p->mUpdDC = 0;
//										p->mUpdMC = 0;
//										p->mUpdSC = 0;
//										p->Upgrade(id,steel[1]);
//										p->Upgrade(id,steel[2]);
//										p->Upgrade(id,steel[3]);
//										moded = true;
//									}
//									if( p->mLevel == 6 )
//									{
//										p->mLevel = 0;
//										p->mUpdAC = 0;
//										p->mUpdMAC = 0;
//										p->mUpdDC = 0;
//										p->mUpdMC = 0;
//										p->mUpdSC = 0;
//										p->Upgrade(id,steel[1]);
//										p->Upgrade(id,steel[2]);
//										p->Upgrade(id,steel[3]);
//										p->Upgrade(id,steel[4]);
//										moded = true;
//									}
//									if( p->mLevel == 8 )
//									{
//										p->mLevel = 0;
//										p->mUpdAC = 0;
//										p->mUpdMAC = 0;
//										p->mUpdDC = 0;
//										p->mUpdMC = 0;
//										p->mUpdSC = 0;
//										p->Upgrade(id,steel[1]);
//										p->Upgrade(id,steel[2]);
//										p->Upgrade(id,steel[3]);
//										p->Upgrade(id,steel[4]);
//										p->Upgrade(id,steel[5]);
//										moded = true;
//									}
//								}
//							}
//						}
//						if( moded && (! checkonly) )
//						{
//							m_pDBManager->DoSaveCharacter(&chr,true,"player");
//						}
//					}
//				}
//#endif
////#endif
//			}
//		}
//	}
//	fclose(file);
//}
std::string CGameServerApp::OnCommand(const char* command)
{
	//if( strcmp(command,"batdegradeitem") == 0 )
	//{
	//	BatDegradeItem(false);
	//	return "done";
	//}
	//if( strcmp(command,"checkbatdegradeitem") == 0 )
	//{
	//	BatDegradeItem(true);
	//	return "done";
	//}
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	if( strcmp(command,"dbinfo") == 0 )
	{
		char temp[512];
		sprintf_s(temp,"db info query : %d , query_res : %d , query_res_free : %d",this->m_pDBManager->m_pDBModule->m_QueryCount,this->m_pDBManager->m_pDBModule->m_QueryResCount,this->m_pDBManager->m_pDBModule->m_QueryResFreeCount);
		temp[511] = 0;
		return temp;
	}
#endif
//#endif
	std::string ret = m_pGameServerLib->GetGMCommand()->ConvertDoCommand(command);
	return ret;
}
LEUD::Application::ExeCommandReturn CGameServerApp::ExeCommand(std::string cmd,std::string& result)
{
	if (strcmp(cmd.c_str(), "exit") == 0 || strcmp(cmd.c_str(), "quit") == 0)
	{
		return EXECOMMAND_RETURN_FINISH;
	}
	return LEUD::Application::ExeCommand( cmd,result );
}
void CGameServerApp::LoadCharacter(const char* account,const char* chrname,const char* seedname,const char* client_ip,int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_character(account,chrname,seedname,client_ip,GUID,"player");
#endif
//#endif
}
void CGameServerApp::LoadDemoCharacter(const char* account,const char* chrname,const char* client_ip,int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_character(account,chrname,"",client_ip,GUID,"demo");
#endif
//#endif
}
void CGameServerApp::SaveCharacter(OGSLib::DomainData* character,bool quit,int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_character(character,quit,GUID,"player");
#endif
//#endif
}
void CGameServerApp::SaveDemoCharacter(OGSLib::DomainData* character,bool quit,int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_character(character,quit,GUID,"demo");
#endif
//#endif
}
void CGameServerApp::SaveKuafuCharacter(std::string& charID,std::string& account,int vcoin,int kuafuResult)
{
	//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_kuafu_character(charID,account,vcoin,kuafuResult);
#endif
	//#endif
}
void CGameServerApp::LoadGuilds(int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_guild(GUID);
#endif
//#endif
}

void CGameServerApp::LoadConsignment(int GUID)
{
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_consignment(GUID);
#endif
}

void CGameServerApp::SaveConsignment(OGSLib::ManagerConsignment& consignment_manager,int GUID)
{
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_consignment(consignment_manager,GUID);
#endif
}

void CGameServerApp::SaveGuilds(const OGSLib::ManagerGuild& guild_manager,int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_guild(guild_manager,GUID);
#endif
//#endif
}
void CGameServerApp::LoadCouples(int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_couple(GUID);
#endif
//#endif
}
void CGameServerApp::SaveCouples(const OGSLib::ManagerCouple& couple_manager,int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_couple(couple_manager,GUID);
#endif
//#endif
}
void CGameServerApp::LoadServerVar(int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_server_var(GUID);
#endif
//#endif
}
void CGameServerApp::LoadAuto()
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_auto(0);
#endif
//#endif
}
void CGameServerApp::OnStoreSql(void* arg,const char* sql,int result,sexy_db_mysql::mem_db_res& r,long effect_row_num,long new_insert_id){
	DBCallBackContent* cbc=(DBCallBackContent*)arg;
	if(cbc->calltype==100){
		void* se=m_pSessionManager->FindSession(cbc->sock);
		if(se){
			CCliNetSession* session = static_cast<CCliNetSession*>(se);
			if( session)
			{
				GS_Client::KuaFuGetCharDataRes res;
				res.loginid=cbc->loginid;
				res.charname=cbc->charname;
				res.result=result;
				if(result==0&&r.num_rows()==1){
					res.step=0;
					unsigned int num_fields;
					long num_rows;
					size_t fields_data_size;
					size_t rows_data_size;
					size_t rows_array_pos;
					const char* field_data;
					const char* row_data;
					r.get_data(&field_data,&row_data,&num_fields,&num_rows,&fields_data_size,&rows_data_size,&rows_array_pos);
					res.num_fields=num_fields;
					res.num_rows=num_rows;
					res.fields_size=fields_data_size;
					res.rows_size=rows_data_size;
					res.row_pos=rows_array_pos;
					session->Send_Message(res);
					session->flushstream();
					size_t data_size_per=1024*6;
					res.step=1000;
					for(size_t p=0;p<fields_data_size;p+=data_size_per){
						res.pos=p;
						res.len=data_size_per;
						if(fields_data_size-p<data_size_per){
							res.len=fields_data_size-p;
						}
						res.data=field_data+p;
						session->Send_Message(res);
						res.step++;
					}
					res.step=2000;
					for(size_t p=0;p<rows_data_size;p+=data_size_per){
						res.pos=p;
						res.len=data_size_per;
						if(rows_data_size-p<data_size_per){
							res.len=rows_data_size-p;
						}
						res.data=row_data+p;
						session->Send_Message(res);
						res.step++;
					}
					res.step=-1;
					session->Send_Message(res);
					session->flushstream();
				}else{
					res.result=-1;
					res.step=-1;
					session->Send_Message(res);
					session->flushstream();
				}
			}
		}
	}
	delete cbc;
}
void CGameServerApp::SaveServerVar(const OGSLib::GameServerLib& gsl,int GUID)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_server_var(gsl.Vars(),GUID);
#endif
//#endif
}
void CGameServerApp::game_exe_sql(const char* sql)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_exe_sql(sql);
#endif
//#endif
}
void CGameServerApp::LoadMails(int GUID)
{
	//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_mail(GUID,m_pGameServerLib->mMailValidDays);
#endif
	//#endif
}
void CGameServerApp::SaveMails(OGSLib::ManagerMail& mail_manager,int GUID)
{
	//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_mail(mail_manager,GUID);
#endif
	//#endif
}
void CGameServerApp::Log(const char* msg)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_Logger.Log(msg);
#endif
//#endif
}
void CGameServerApp::AcceptOff()
{
	m_pCliAcceptor->mbOnService = false;
}
void CGameServerApp::AcceptOn()
{
	m_pCliAcceptor->mbOnService = true;
}
void CGameServerApp::BreakNet()
{
	m_pSessionManager->BreakNetVithLogic();
}
void CGameServerApp::ClientAuth(SOCKET sock,std::string& sessionID,int authseed,int pid,const std::string& idfa)
{
	std::string account;
	int ret = 1;
	bool testlogin = false;
#ifdef _DEBUG
	if(authseed==0)
	{
		testlogin = true;
	}
#endif
#ifdef LIMIT_RELEASE
	if(authseed==0)
	{
		testlogin = true;
	}
#endif
	if( ( mAuthType == 4 || authseed==0 ) && !testlogin )
	{
		char ip[128];
		LEUD::TCPSession::GetPeerIP(sock,ip);
		bool denyip = m_pGameServerLib->GetDenyManager()->CheckDenyIP(ip);

		int result=0;
		if(!sessionID.empty() && !denyip)
		{
			std::vector<std::string> str;
			int index=sessionID.size();
			while(1)
			{
				index=sessionID.find_first_of('|');
				str.push_back(sessionID.substr(0,index));
				sessionID=sessionID.substr(index+1);
				if(index<=0)
				{
					break;
				}
			}
			if(str.size()==3)
			{
				//int timeflag=atoi(str[1].c_str());

				unsigned char md5[32];
				unsigned char src[256];
				memset(md5,0,32);
				memset(src,0,256);
				const char* md5Key = mAuthKey.c_str();
				std::string md5str=str[0]+str[1]+md5Key;
				memcpy(src,md5str.c_str(),md5str.length());
				LEUD::CryptographyMD5::Do(md5,src,md5str.length());
				char temp[256];
				_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
				if(strcmp(temp,str[2].c_str())!=0)
				{
					//printf("temp=%s,str2=%s,====e1\r\n",temp,str[2].c_str());
					result=GS_Client::AuthenticateRES::result_error_session_id_error;
				}
				else
				{
					account=str[0];
				}
			}
			else
			{
				result=GS_Client::AuthenticateRES::result_error_session_id_error;
			}
		}
		else
		{
			result=GS_Client::AuthenticateRES::result_error_session_id_error;
		}
		CCliNetSession* se = (CCliNetSession*)m_pSessionManager->FindSession(sock);
		if( se )
		{
			se->OnAuthResult(result,sessionID,account,1);
		}
	}
	else if( mAuthType == 2 )
	{
		char ip[128];
		LEUD::TCPSession::GetPeerIP(sock,ip);
#ifndef LIMIT_RELEASE_2
		m_pOCenterManager->push_client_auth(sock,sessionID,ip/*,authseed,0*/);
#endif
	}
}
int CGameServerApp::UpdateTicket(SOCKET sock,std::string& sessionID,int auth_id)
{
	if( mAuthType == 2 )
	{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
		m_pOCenterManager->push_update_ticket(sock,sessionID,auth_id);
#endif
//#endif
		return 1;
	}
	return 0;
}

void CGameServerApp::RefreshGift(const std::string& chrname)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_fresh_gift(chrname.c_str());
#endif
//#endif
}
void CGameServerApp::RefreshVcoinCongzi(const std::string& account,const std::string& chrname)
{
	if( mCongziType == 2 )
	{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
		m_pOCenterManager->push_fresh_vcoin(account,chrname);
#endif
//#endif
	}
}

void CGameServerApp::LoadGMAllowIP()
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pGMAcceptor->LoadAllowIP();
#endif
//#endif
}
void CGameServerApp::SetAuthCheckIP(bool checkip)
{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_pOCenterManager->checkIP(checkip);
#endif
//#endif
}
void CGameServerApp::LoadConfig()
{
}
void CGameServerApp::LoadKernelConfig()
{
	mConnMax = GetPrivateProfileInt("net","conn_max",5000,m_ConfigFile.c_str());
	m_pSessionManager->LoadConfig(this);
}
const char* CGameServerApp::LoadFile(const char* fn,size_t& size)
{
	size_t buflen=mRsaBufferSize;
	char* data =mRsaBuffer;
	size = 0;
	char file_name[256];
#ifdef NDEBUG
#ifndef LIMIT_RELEASE
	_snprintf_s(file_name,255,"%s%s","gs_data/",fn);
	if (m_pContentAcceptor->Extr(file_name, data, buflen))
	{
		size = buflen;
		LEUD::CryptographySimple::Decode((unsigned char*)data,size,mCryptKey,mCryptDict2);
		return data;
	}
	else
	{
		char fp[256];strncpy_s(fp,fn,255);
		bool pt=true;
		if( strlen(fp) > 7 )
		{
			fp[7]=0;
			if( strcmp(fp,"mondrop")==0 )
			{
				pt = false;
			}
			if( strcmp(fp,"montalk")==0 )
			{
				pt = false;
			}
			fp[6] = 0;
			if( strcmp(fp,"script") == 0 )
			{
				pt=false;
			}
		}
		if( pt )
		{
			printf("zip load error on file %s\n",file_name);
		}
	}
	return 0;
#endif
#endif
	_snprintf_s(file_name,_countof(file_name),255,"%s%s",m_pGameServerLib->GetDataPath().c_str(),fn);
	file_name[255]=0;
	FILE* file;
	fopen_s(&file,file_name,"rb");
	if( file )
	{
		size = fread(data,1,buflen,file);
		fclose(file);
	}
	if( size > 0 )
	{
#ifdef NDEBUG
#ifndef LIMIT_RELEASE
		LEUD::CryptographySimple::Decode((unsigned char*)data,size,mCryptKey,mCryptDict2);
#endif
#endif
		return data;
	}
	else
	{
		char fp[256];strncpy_s(fp,_countof(fp),fn,255);fp[255]=0;
		bool pt=true;
		if( strlen(fp) > 7 )
		{
			fp[7]=0;
			if( strcmp(fp,"mondrop")==0 )
			{
				pt = false;
			}
			if( strcmp(fp,"montalk")==0 )
			{
				pt = false;
			}
			fp[6] = 0;
			if( strcmp(fp,"script") == 0 )
			{
				pt=false;
			}
		}
		if( pt )
		{
			printf("load error on file %s\n",file_name);
		}
	}
	return 0;
}
bool CGameServerApp::GetLocalIP(char* ip)  
{  
	//1.初始化wsa  
	WSADATA wsaData;  
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);  
	if (ret!=0)  
	{  
		return false;  
	}  
	//2.获取主机名  
	char hostname[256];  
	ret=gethostname(hostname,sizeof(hostname));  
	if (ret==SOCKET_ERROR)  
	{  
		return false;  
	}  
	//3.获取主机ip  
	HOSTENT* host=gethostbyname(hostname);  
	if (host==NULL)  
	{  
		return false;  
	}  
	//4.转化为char*并拷贝返回  
	strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));  
	return true;  
}  
void CGameServerApp::ConnectService(const char* ip,const char* port)
{
	if( mKuaFuService && m_KuaFuService )
	{
		m_KuaFuService->AddServer(ip,port);
	}
	else
	{
		printf("kuafuservice not start!");
	}
}

void CGameServerApp::LoadRelationCache(int GUID)
{
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_load_relation_cache(GUID);
#endif
}

void CGameServerApp::SaveRelationCache(OGSLib::ManagerRelationCache& relationcache_manager,int GUID)
{
#ifndef LIMIT_RELEASE_2
	m_pDBManager->push_save_relation_cache(relationcache_manager,GUID);
#endif
}
