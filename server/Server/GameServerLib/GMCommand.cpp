#include "StdAfx.h"
#include "GmCommand.h"
#include "GameServerLib.h"
#include "ScriptLua.h"
#include "DomainPlay.h"
#include "SubItem.h"
#include "GameSession.h"
#include "ManagerMap.h"
#include "ManagerNotice.h"
#include "ManagerItemDef.h"
#include "ManagerFilter.h"
#include "ManagerDeny.h"
#include "ManagerMail.h"
#include "ManagerAutoRun.h"
#include "ManagerSkillDef.h"
#include "ManagerMonDef.h"
#include "ManagerStatus.h"
#include "ManagerTotalAttr.h"
#include "ManagerGuild.h"
#include "ManagerCouple.h"
#include "ManagerObjFuncs.h"
#include "ManagerLevelInfo.h"
#include "ManagerChart.h"
#include "PlayNpc.h"
#include "PlayMap.h"
#include "time.h"
#include "leudgrid/MemoryPool.h"
#include "leudgrid/TCPSessionMini.h"

namespace OGSLib
{
	GMCommand::GMCommand(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	GMCommand::~GMCommand(void)
	{
	}
	void GMCommand::Start()
	{
		LoadAdminList();
		LoadAdminCommand();
	}
	void GMCommand::AddAdmin(const std::string& name,int lv)
	{
		mAdminList[name] = lv;
	}
	void GMCommand::LoadAdminCommand()
	{
		mCommandList.clear();
		//char temp[256];
		std::string mapinfofile = this->mGameServerLib->GetConfigPath()+"cmdlist.txt";
		std::ifstream ifs(mapinfofile.c_str());
		std::string line;
		while( std::getline(ifs,line) )
		{
			if( line.length()> 0 )
			{
				if( line.substr(0,1) == "#" || line.substr(0,1) == ";" )
				{
					continue;
				}
				char name[256];
				char nn[256];
				memset(name,0,256);
				memset(nn,0,256);
				int ret = sscanf_s(line.c_str(),"%[^=]=%s",name,_countof(name),nn,_countof(nn));
				if( 2 == ret )
				{
					//UtilString::act_2_utf8(name,strlen(name),temp,256);
					//strncpy_s(name,_countof(name),temp,255);
					name[255]=0;
					nn[255]=0;
					mCommandList[name] = nn;
				}
			}
		}
	}
	void GMCommand::LoadAdminList()
	{
		mAdminList.clear();
		char temp[256];
		std::string mapinfofile = this->mGameServerLib->GetConfigPath()+"adminlist.txt";
		std::ifstream ifs(mapinfofile.c_str());
		std::string line;
		while( std::getline(ifs,line) )
		{
			if( line.length()> 0 )
			{
				if( line.substr(0,1) == "#" || line.substr(0,1) == ";" )
				{
					continue;
				}
				char name[256];
				int lv;
				memset(name,0,256);
				int ret = sscanf_s(line.c_str(),"%[^ \t]%*[ \t]%d",name,_countof(name),&lv);
				if( 2 == ret )
				{
					UtilString::act_2_utf8(name,strlen(name),temp,256);
					strncpy_s(name,_countof(name),temp,255);
					name[255]=0;
					mAdminList[name] = lv;
				}
			}
		}
	}
	void GMCommand::SaveAdminList()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"adminlist.txt";
		std::ofstream ofs(mapinfofile);
		char temp[1024];
		if( ofs )
		{
			for(std::map<std::string,int>::iterator pos = mAdminList.begin();
				pos != mAdminList.end();
				++pos )
			{
				UtilString::utf8_2_act(pos->first.c_str(),pos->first.length(),temp,1024);
				ofs<<temp<<" "<<pos->second<<std::endl;
			}
			ofs<<std::endl;
		}
	}

	void GMCommand::Stop()
	{
		SaveAdminList();
	}
	std::string GMCommand::ConvertDoCommand(std::string command)
	{
		char temp[1024];
		UtilString::act_2_utf8(command.c_str(),command.length(),temp,1024);
		command = temp;
		return DoSystemCommand(command,1000);
	}
	std::string GMCommand::DoSystemCommand(std::string command,int clv)
	{
		if( command.compare("lcmdlist") == 0 )
		{
			LoadAdminCommand();
			return "done";
		}
		if( command == "reloadscript" || command == "lscript" || command == "lua")
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetScriptEngine()->reload();
			return std::string("done");
		}
		if( command == "reloadmondrop" || command == "lmondrop" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetMonDefManager()->ReloadMonDropDef();
			return std::string("done");
		}
		if( command == "reloadadmin" || command == "ladmin" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			LoadAdminList();
			return std::string("done");
		}
		if( command == "reloadnotice" || command == "lnotice" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetNoticeManager()->LoadNotice();
			return std::string("done");
		}
		if( command == "clearnotice" || command == "cnotice" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetNoticeManager()->ClearNotice();
			return std::string("done");
		}
		if( command == "reloadfilter" || command == "lfilter" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetFilterManager()->LoadFilterWord();
			return std::string("done");
		}
		if( command == "reloadconfig" || command == "lconfig" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->LoadConfig();
			return std::string("done");
		}
		if( command == "reloadautorun" || command == "lautorun" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetAutoRunManager()->LoadAutoRun();
			return std::string("done");
		}
		if( command.compare("lautorundb") == 0 )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetListener()->LoadAuto();
			return std::string("done");
		}
		if( command == "reloaditemdef" || command == "litemdef" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if(mGameServerLib->GetItemDefineManager()->ReloadItemDefine(mGameServerLib))
			{
				return std::string("done");
			}
			else
			{
				return std::string("load failed");
			}
		}
		if ( command == "reloadbuffdef" || command == "lbuffdef" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}

			if( ! mGameServerLib->GetStatusManager()->ReloadStatusDef(mGameServerLib) )
			{
				return std::string("load failed");
			}
			return std::string("done");
		}
		if ( command == "reloadfuncdef" || command == "lfuncdef" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}

			if( ! mGameServerLib->GetObjFuncManager()->ReloadFuncsInfo() )
			{
				return std::string("load failed");
			}
			return std::string("done");
		}
		if ( command == "reloadlevelinfo" || command == "levelinfo" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}

			if( ! mGameServerLib->GetLevelInfoManager()->ReloadLevelInfo() )
			{
				return std::string("load failed");
			}
			return std::string("done");
		}
		if( command == "reloadmonai" || command == "lmonai" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if (!mGameServerLib->GetMonDefManager()->ReloadMonAIDef())
			{
				return std::string("load failed");
			}
			return std::string("done");
		}
		if( command == "setmerge" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.setmerge");
			return std::string("done");
		}
		if( command == "setnomerge" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.setnomerge");
			return std::string("done");
		}
		if( command == "reloadplusdef" || command == "lplusdef" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if(mGameServerLib->GetItemDefineManager()->ReloadPlusDefine(mGameServerLib))
			{
				return std::string("done");
			}
			else
			{
				return std::string("load failed");
			}
		}
		if( command == "reloadstatus" || command == "lstatus" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if( ! mGameServerLib->GetStatusManager()->ReloadStatusDef(mGameServerLib) )
			{
				return std::string("statusdef failed");
			}
			if( ! mGameServerLib->GetTotalInfoManager()->ReloadStatusFight())
			{
				return std::string("statusfight failed");
			}
			return std::string("load ok");
		}
		if( command == "reloaditemattr" || command == "litemattr" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if(mGameServerLib->GetItemDefineManager()->ReloadItemAttr(mGameServerLib))
			{
				return std::string("done");
			}
			else
			{
				return std::string("load failed");
			}
		}

		if( command == "litemspeed")
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if(mGameServerLib->GetItemDefineManager()->LoadSpeedDefine(mGameServerLib))
			{
				return std::string("done");
			}
			else
			{
				return std::string("load failed");
			}	
		}
		if( command == "litemstack")
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if(mGameServerLib->GetItemDefineManager()->LoadStackDefine(mGameServerLib))
			{
				return std::string("done");
			}
			else
			{
				return std::string("load failed");
			}	
		}
		if( command == "reloadskilldef" || command == "lskilldef" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if(mGameServerLib->GetSkillDefineManager()->ReLoadSkillDefine(mGameServerLib) )
			{
				return std::string("done");
			}
			else
			{
				return std::string("load failed");
			}
		}
		if( command == "reloadvcoinshop" || command == "lvcoinshop" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if( mGameServerLib->GetShopNpc()->ReloadShopGood() )
			{
				return std::string("done");
			}
			else
			{
				return std::string("load failed");
			}
		}
		if( command == "reloaddeny" || command == "ldeny")
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetDenyManager()->LoadDeny();
			mGameServerLib->GetDenyManager()->LoadDenyIP();
			return std::string("done");
		}
		if( command == "reloadgmallowip" || command == "lgmip" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetListener()->LoadGMAllowIP();
			return std::string("done");
		}
		if( command == "reloadkernel" || command == "lkernel" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->LoadKernelConfig();
			return std::string("done");
		}
		if( 0 == command.compare("lstring") )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->mStringConsts.ReLoad(mGameServerLib);
			return std::string("done");
		}
		if( command == mCommandList["startwar"] )
		{
			if( ! CheckGMLevel(clv,10) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->StartWar();
			return std::string("done");
		}
		if( command == "acceptoff" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetListener()->AcceptOff();
			return "done";
		}
		if( command == "accepton" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetListener()->AcceptOn();
			return "done";
		}
		if( command == "breaknet" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetListener()->BreakNet();
			return "done";
		}
		if( command == mCommandList["stopwar"] )
		{
			if( ! CheckGMLevel(clv,10) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->StopWar();
			return std::string("done");
		}
		if( 0==command.compare("show_mem") )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char ss[256];
			double fm = 0;
			double mp = LEUD::PoolMemoryGetMemorySize()/1024.0/1024.0;
			double sm = mGameServerLib->GetScriptEngine()->mem_size()/1024.0/1024.0;
			double all = fm+mp+sm;
			_snprintf_s(ss,_countof(ss),255,"all:%fMB,fm:%fMB,mp:%fMB,sm:%fMB",all,fm,mp,sm);
			ss[255]=0;
			return ss;
		}
		if( 0==command.compare("dump_mem_pool") )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			LEUD::PoolMemoryDumpInfo();
			return std::string("done");
		}
		if( command == mCommandList["online_num"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char ss[256];
			_snprintf_s(ss,_countof(ss),255,"cur:%d,max:%d",static_cast<int>(mGameServerLib->GetSessionNumber()),static_cast<int>(mGameServerLib->GetMaxSessionNumber()));
			ss[255]=0;
			//std::stringstream ss;
			//ss<<"cur:"<<<<" max:"<<;
			return ss;
		}
		if( command.substr(0,4) == "spsm" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			int num;
			if( 1 == sscanf_s(command.c_str(),"spsm%d",&num) )
			{
				if( num >= 128 && num <= 2048 )
				{
					LEUD::TCPSessionMini::mSendPoolSizeMax = num;
				}
			}
		}
		if( command.substr(0,4) == mCommandList["kick"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1== sscanf_s(command.c_str(),"kick%*[ \t]%s",name,_countof(name)) )
			{
				std::string n(name);
				GameSession* session = mGameServerLib->FindSessionWithCharID(n);
				if( session )
				{
					session->BreakNetAndEndSession(mGameServerLib->mStringConsts.NOTIFY_KICK_BUY_GM);
					return std::string("done");
				}
			}
			return std::string("error,not find");
		}
		if( command.substr(0,4) == mCommandList["king"] )
		{
			if( ! CheckGMLevel(clv,100) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1 == sscanf_s(command.c_str(),"king%*[ \t]%s",name,_countof(name)) )
			{
				std::string n(name);
				mGameServerLib->SetKingGuild(n);
				return std::string("done");
			}
			return std::string("error,not find");
		}
		if( command.substr(0,7) == mCommandList["denychr"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1== sscanf_s(command.c_str(),"denychr%*[ \t]%s",name,_countof(name)) )
			{
				std::string n(name);
				mGameServerLib->GetDenyManager()->AddChrDeny(n);
				GameSession* session = mGameServerLib->FindSessionWithCharID(n);
				if( session )
				{
					session->BreakNetAndEndSession(mGameServerLib->mStringConsts.NOTIFY_KICK_BUY_GM);
					return std::string("done");
				}
			}
			return std::string("error,not find");
		}
		if( command.substr(0,8) == mCommandList["allowchr"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1== sscanf_s(command.c_str(),"allowchr%*[ \t]%s",name,_countof(name)) )
			{
				std::string n(name);
				mGameServerLib->GetDenyManager()->RemChrDeny(n);
				return std::string("done");
			}
			return std::string("error,not find");
		}
		if( command.substr(0,6) == mCommandList["denyip"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char ip[256];
			if( 1== sscanf_s(command.c_str(),"denyip%*[ \t]%s",ip,_countof(ip)) )
			{
				mGameServerLib->GetDenyManager()->AddDenyIP(ip);
				return std::string("done");
			}
			return std::string("error,not find");
		}
		if( command.substr(0,7) == mCommandList["allowip"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char ip[256];
			if( 1== sscanf_s(command.c_str(),"allowip%*[ \t]%s",ip,_countof(ip)) )
			{
				mGameServerLib->GetDenyManager()->RemDenyIP(ip);
				return std::string("done");
			}
			return std::string("error,not find");
		}
		if( command.substr(0,8) == mCommandList["limitchr"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1== sscanf_s(command.c_str(),"limitchr%*[ \t]%s",name,_countof(name)) )
			{
				std::string n(name);
				mGameServerLib->GetDenyManager()->AddChrLimit(n);
				return std::string("done");
			}
			return std::string("error,not find");
		}
		if( command.substr(0,10) == mCommandList["unlimitchr"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1== sscanf_s(command.c_str(),"unlimitchr%*[ \t]%s",name,_countof(name)) )
			{
				std::string n(name);
				mGameServerLib->GetDenyManager()->RemChrLimit(n);
				return std::string("done");
			}
			return std::string("error,not find");
		}
		if( command.substr(0,5) == mCommandList["addgm"] )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char name[256];
			int lv;
			if(2 == sscanf_s(command.c_str(),"addgm%*[ \t]%s%d",name,_countof(name),&lv) )
			{
				std::string nm = name;
				mAdminList[nm] = lv;
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [addgm character_name gm_level]");
			}
		}
		if( command.substr(0,5) == mCommandList["remgm"] )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if(1 == sscanf_s(command.c_str(),"remgm%*[ \t]%s",name,_countof(name)) )
			{
				std::string nm = name;
				mAdminList.erase(nm);
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [remgm character_name]");
			}
		}
		if (command.substr(0,9) == mCommandList["callhuwei"] )
		{
			if( ! CheckGMLevel(clv,100) )
			{
				return std::string("authority failed");
			}

			char name[256];
			int id=0;
			if(2 == sscanf_s(command.c_str(),"callhuwei%*[ \t]%s%d",name,_countof(name),&id) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					gs->GetPlayer()->gen_slave(10, id, 4);
					return std::string("done");
				}
				else
				{
					return std::string("target not find");
				}
			}
			else
			{
				return std::string("param error,try [setparam character_name paramid paramvalue]");
			}
		}
		if( command.substr(0,8) == mCommandList["setparam"] )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char name[256];
			int pid=0;
			char param[256];
			if(3 == sscanf_s(command.c_str(),"setparam%*[ \t]%s%d%*[ \t]%s",name,_countof(name),&pid,param,_countof(param)) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					std::string value=param;
					gs->GetPlayer()->Param(pid,value);
					return std::string("done");
				}
				else
				{
					return std::string("target not find");
				}
			}
			else
			{
				return std::string("param error,try [setparam character_name paramid paramvalue]");
			}
		}
		if( command.substr(0,8) == mCommandList["getparam"] )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char name[256];
			int pid=0;
			if(2 == sscanf_s(command.c_str(),"getparam%*[ \t]%s%d",name,_countof(name),&pid) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					std::string value=gs->GetPlayer()->Param(pid);
					return value;
				}
				else
				{
					return std::string("target not find");
				}
			}
			else
			{
				return std::string("param error,try [getparam character_name paramid]");
			}
		}
		if( command.substr(0,4) == "task" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char name[256];
			int task_type;//任务id
			int task_id;//任务线里的第n个任务
			int task_state=2;//任务状态
			int ret=0;
			ret = sscanf(command.c_str(),"task%*[ \t]%s%d%d%d",name,&task_type,&task_id,&task_state);
			if( ret==3||ret==4 )
			{
				std::string nm(name);
				GameSession* session = mGameServerLib->FindSessionWithCharID(nm);
				if( session && session->GetPlayer() )
				{
					session->GetPlayer()->GMSetTaskState(task_type,task_id,task_state);
				}
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [task character_name task_type task_id task_state]");
			}
		}
		if( command == "savecouple" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetCoupleManager()->SaveCouples();
			return std::string("done");
		}
		if( command == "saveguild" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetGuildManager()->SaveGuilds();
			return std::string("done");
		}
		if( command == "saveservervar")
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetListener()->SaveServerVar(*mGameServerLib,0);
			return std::string("done");
		}
		if( command.substr(0,5) == mCommandList["level"] )
		{
			if( ! CheckGMLevel(clv,100) )
			{
				return std::string("authority failed");
			}
			char name[256];
			int level = 1;
			if( 2 == sscanf_s(command.c_str(),"level%*[ \t]%s%d",name,_countof(name),&level) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					gs->GetPlayer()->SetUpdateLevel(level);
					return std::string("done");
				}
				else
				{
					return std::string("target not find");
				}
			}
			else
			{
				return std::string("param error, try [level character_name new_level]");
			}
		}
		if (command.substr(0, 5) == "usebs")
		{
			if (!CheckGMLevel(clv, 100))
			{
				return std::string("authority failed");
			}
			char name[256];
			int lv = 10;
			if (2 == sscanf_s(command.c_str(), "usebs%*[ \t]%s%d", name, _countof(name), &lv))
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if (gs && gs->GetPlayer())
				{
					gs->GetPlayer()->UseSomeBS(lv);
					return std::string("remove done");
				}
				else
				{
					return std::string("target not find");
				}
			}
			else
			{
				return std::string("param error, try [level character_name new_level]");
			}
		}
		if (command.substr(0, 8) == mCommandList["chongzhi"])
		{
			if (!CheckGMLevel(clv, 1000))
			{
				return std::string("authority failed");
			}
			int num = 1;
			char name[256];
			int ret = sscanf(command.c_str(), "chongzhi%*[ \t]%s%d", name, &num);
			if (ret == 2)
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if (gs && gs->GetPlayer())
				{
					gs->GetPlayer()->OnFreshVcoin(num, 1,true);
					return std::string("chongzhi done");
				}
			}
			else
			{
				return std::string("error");
			}
		}
		if (command.substr(0, 8) == mCommandList["addvcoin"])
		{
			if (!CheckGMLevel(clv, 1000))
			{
				return std::string("authority failed");
			}
			int num = 1;
			int tt = 0;
			char name[256];
			int ret = sscanf(command.c_str(), "addvcoin%*[ \t]%s%d%d", name, &num, &tt);
			if (ret == 3)
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if (gs && gs->GetPlayer())
				{
					if(time && tt + 60 > time(0))
					{
						gs->GetPlayer()->OnFreshVcoin(num, 1);
						return std::string("addvcoin done");
					}
					else
					{
						return std::string("addvcoin time out");
					}
				}
			}
			else if (ret == 2)
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if (gs && gs->GetPlayer())
				{
					gs->GetPlayer()->OnFreshVcoin(num, 1, true);
					return std::string("addvcoin done");
				}
			}
			else
			{
				return std::string("error");
			}
		}
		if( command.substr(0,7) == mCommandList["chatoff"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			int dura = 60;
			if( 2 == sscanf_s(command.c_str(),"chatoff%*[ \t]%s%d",name,_countof(name),&dura) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					gs->GetPlayer()->TalkProhibited(dura);
					return std::string("done");
				}
				else
				{
					return std::string("target not find");
				}
			}
			else
			{
				return std::string("param error, try [chatoff character_name duration]");
			}
		}
		if( command.substr(0,6) == mCommandList["chaton"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1 == sscanf_s(command.c_str(),"chaton%*[ \t]%s",name,_countof(name)) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					gs->GetPlayer()->TalkProhibited(0);
					return std::string("done");
				}
				else
				{
					return std::string("target not find");
				}
			}
			else
			{
				return std::string("param error, try [chaton character_name]");
			}
		}
		if( command.substr(0,4) == mCommandList["info"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char msg[256];
			int lv,flags;
			if( 3 == sscanf_s(command.c_str(),"info%d%d%*[ \t]%[^\n]",&lv,&flags,msg,_countof(msg)) )
			{
				mGameServerLib->ServerAlertMessage(lv,flags,msg);
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [info please_flags flags msg]\nplease_flags:\n 1 flash alert center window\n 10 chat history\n 100 flash notify,like exp +xxx\n 1000 long notify for task history \n 10000 importent info like system reboot");
			}
		}
		if( command.substr(0,8) == "disguild" )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				return std::string("authority failed");
			}
			char name[256];
			if( 1== sscanf(command.c_str(),"disguild%*[ \t]%s",name) )
			{
				std::string n(name);
				if(mGameServerLib->GetGuildManager()->FindGuild(n))
				{
					mGameServerLib->GetGuildManager()->DismissGuild(n);
					return std::string("done");
				}
				return std::string("error,guild not find ");
			}
			return std::string("error,try: disguild guild_name");
		}
		if( command.substr(0,4) == "frvc" )
		{
			char chrname[256];
			if( 1 == sscanf_s(command.c_str(),"frvc%*[ \t]%s",chrname,_countof(chrname) ) )
			{
				std::string nm(chrname);
				GameSession* gs = mGameServerLib->FindSessionWithLoginID(nm);
				if( gs && gs->GetPlayer() )
				{
					gs->GetPlayer()->FreshVcoin();
					return std::string("done");
				}else{return std::string("id not found");}
			}else{return std::string("frvc loginid");}
		}
		if( command.substr(0,6) == "frgift" )
		{
			char chrname[256];
			if( 1 == sscanf_s(command.c_str(),"frgift%*[ \t]%s",chrname,_countof(chrname) ) )
			{
				std::string nm(chrname);
				GameSession* gs = mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					gs->GetPlayer()->FreshGift();
					return std::string("done");
				}else{return std::string("target not found");}
			}else{return std::string("frgift loginid");}
		}
		if (command == "openhq")
		{
			if (!CheckGMLevel(clv, 1000))
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetScriptEngine()->SystemScript("npc.biggm.onTalkmergeSvrResetTime0");
			return std::string("hq on");
		}
		if (command == "closehq")
		{
			if (!CheckGMLevel(clv, 1000))
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetScriptEngine()->SystemScript("npc.biggm.onTalkmergeSvrResetTime8");
			return std::string("hq off");
		}
		if( command.substr(0,9) == mCommandList["addnotice"] )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char msg[256];
			if(1 == sscanf_s(command.c_str(),"addnotice%*[ \t]%s",msg,_countof(msg)) )
			{
				std::string notice = msg;
				if (mGameServerLib->GetNoticeManager()->AddNotice(notice))
				{
					mGameServerLib->GetNoticeManager()->SaveNotice();
					return std::string("done");
				}
				else
				{
					return std::string("param error");
				}
			}
			else
			{
				return std::string("param error,try [addnotice notice]");
			}
		}
		if( command.substr(0,9) == mCommandList["remnotice"] )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			int notice_id;
			if(1 == sscanf_s(command.c_str(),"remnotice%d",&notice_id) )
			{
				if (mGameServerLib->GetNoticeManager()->RemNotice(notice_id))
				{
					mGameServerLib->GetNoticeManager()->SaveNotice();
					return std::string("done");
				}
				else
				{
					return std::string("notices or param error"); 
				}
			}
			char msg[256];
			if(1 == sscanf_s(command.c_str(),"remnotice%*[ \t]%s",msg,_countof(msg)) )
			{
				std::string notice = msg;
				if (mGameServerLib->GetNoticeManager()->RemNotice(notice))
				{
					mGameServerLib->GetNoticeManager()->SaveNotice();
					return std::string("done");
				}
				else
				{
					return std::string("notices or param error"); 
				}
			}
			else
			{
				return std::string("param error,try [remnotice notice/notice_id]");
			}
		}
		if( command.substr(0,9) == mCommandList["getnotice"] )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			int notice_id = 0;
			if(1 == sscanf_s(command.c_str(),"getnotice%d",&notice_id) )
			{
				std::string notice;
				if (notice_id >=1000)
				{
					notice = mGameServerLib->GetNoticeManager()->ShowNotices();
				}
				else
				{
					notice = mGameServerLib->GetNoticeManager()->get_notice(notice_id);
				}
				if (! notice.empty())
				{
					return notice;
				}
				else
				{
					return std::string("notices or param error"); 
				}
			}
			else
			{
				return std::string("param error,try [getnotice notice_id]");
			}
		}
		if( command.substr(0,4) == mCommandList["give"] )
		{
			if( ! CheckGMLevel(clv,100) )
			{
				return std::string("authority failed");
			}
			char chrname[256];
			char itemname[256];
			int num;
			int luck;
			int lv;
			int add_ac;
			int add_mac;
			int add_dc;
			int add_mc;
			int add_sc;
			int upg_ac;
			int upg_mac;
			int upg_dc;
			int upg_mc;
			int upg_sc;
			int protect;
			int flag_bind;
			int ret = sscanf_s(command.c_str(),"give%*[ \t]%s%*[ \t]%s%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",chrname,_countof(chrname),itemname,_countof(itemname),&num,&flag_bind,&luck,&lv,&add_ac,&add_mac,&add_dc,&add_mc,&add_sc,&upg_ac,&upg_mac,&upg_dc,&upg_mc,&upg_sc,&protect);
			if( ret == 2 )
			{
				ret = 3;
				num = 1;
			}
			if( ret >= 3 )
			{
				std::string nm(chrname);
				GameSession* gs = mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer() )
				{
					ItemDef* id = mGameServerLib->GetItemDefineManager()->GetItemDefine(itemname);
					if( ! id )
					{
						if( isdigit(itemname[0]) )
						{
							id = mGameServerLib->GetItemDefineManager()->GetItemDefine(atoi(itemname));
						}
					}
					if( id )
					{
						if( id->mTypeID == ITEM_VCOIN_ID )
						{
							if( ! CheckGMLevel(clv,1000) )
							{
								gs->GetPlayer()->AlertMessage(10,0,"authority failed");
								return "done";
							}

							gs->GetPlayer()->AddVcoinGS(num);
							return "done";
						}
						SubItem ii;
						int flags = 0;
						if( clv <= 100 )
						{
							flags = ITEM_FLAG_USE_BIND;
						}
						if(ret>=4)
						{
							if( flag_bind )
							{
								flags |= ITEM_FLAG_BIND;
							}
						}
						ii.BuildItemInstance(id,flags,false,mGameServerLib->mItemAddType);
						if( ret >= 16 )
						{
							ii.mLuck = (short)luck;
							ii.mLevel = lv;
							ii.mAddAC = (short)add_ac;
							ii.mAddMAC = (short)add_mac;
							ii.mAddDC = (short)add_dc;
							ii.mAddMC = (short)add_mc;
							ii.mAddSC = (short)add_sc;
							ii.mUpdAC = (short)upg_ac;
							ii.mUpdMAC = (short)upg_mac;
							ii.mUpdDC = (short)upg_dc;
							ii.mUpdMC = (short)upg_mc;
							ii.mUpdSC = (short)upg_sc;
							if( ret >= 16 )
							{
								ii.mProtect = (short)protect;
							}
						}
						gs->GetPlayer()->AddItem(&ii,num,102,11,1);
						std::string ss("done");
						return ss;
					}
					else
					{
						std::string ss("error item not found");
						return ss;
					}
				}
				else
				{
					std::string ss("error player not found");
					return ss;
				}
			}
			else
			{
				std::string ss("param error,try [make item_name number=1]");
				return ss;
			}
		}
		if( command.substr(0,8) == "savemail" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			mGameServerLib->GetMailManager()->SaveMails();
			return std::string("done");
		}
		if( command.substr(0,8) == "postmail" )
		{
			if( ! CheckGMLevel(clv,100) )
			{
				return std::string("authority failed");
			}
			char names[256];
			char title[256];
			char content[256];
			char items[256];
			int ret = sscanf(command.c_str(),"postmail%*[ \t]%s%*[ \t]%s%*[ \t]%s%*[ \t]%s",names,title,content,items);
			if( ret == 4 )
			{
				std::string strNames(names);
				const char *d = ",";
				std::map<int,int> itemMap;
				char *p1;char *p2;int id,num = 0;
				p1 = strtok(items,d);
				p2 = strtok(NULL,d);
				while(p1 && p2)
				{
					id = atoi(p1);
					num = atoi(p2);
					if(id > 0 && num > 0)
					{
						itemMap[id] = num;
					}
					p1 = strtok(NULL,d);
					p2 = strtok(NULL,d);
				}
				if(strNames == "all")
				{
					mGameServerLib->GetMailManager()->PostMailToAllPlayers(std::string(title),std::string(content),itemMap,1);
					return std::string("done");
				}
                else if (strNames.substr(0, 5) == "user:") // 这个模式仅用于测试，只能发给在线的指定名字的玩家
                {
                    std::string charId = strNames.substr(5);
                    if (GameSession *gs = mGameServerLib->FindSessionWithCharID(charId)) {
                        if (DomainPlay *player = gs->GetPlayer()) {
							if(mGameServerLib->GetMailManager()->PostMailToOnePlayer(std::string(title), std::string(content), std::string(player->SeedName()), itemMap, 1)==0)
							{
								return std::string("done");
							}
                        }
                    }
                }
				else
				{
					std::vector<std::string> seedNames;
					char *p;
					p = strtok(names,d);
					while(p)
					{
						seedNames.push_back(p);
						p = strtok(NULL,d);
					}
					if(mGameServerLib->GetMailManager()->PostMailToMultiplePlayers(std::string(title),std::string(content),seedNames,itemMap,1)==0)
					{
						return std::string("done");
					}
				}
				return "fail";
			}
			else
			{
				std::string ss("param error,try [postmail all/seedname/user:id title content id,num]]");
				return ss;
			}
		}
		if( command.substr(0,5) == "kfurl" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char url[512],port[128],localUrl[512];
			if(3 == sscanf(command.c_str(),"kfurl%*[ \t]%s%*[ \t]%s%*[ \t]%s",url,port,localUrl))
			{
				mGameServerLib->SetKuafuURL(std::string(url));
				mGameServerLib->SetKuafuPort(std::string(port));
				mGameServerLib->SetLocalDomain(std::string(localUrl));
				mGameServerLib->GetListener()->SaveServerVar(*mGameServerLib,0);
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [kfurl kuafuurl kuafuport localurl]");
			}
		}
		if( command.substr(0,8) == "kuafuurl" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char url[512],port[128],localUrl[512];
			if(3 == sscanf(command.c_str(),"kuafuurl%*[ \t]%s%*[ \t]%s%*[ \t]%s",url,port,localUrl))
			{
				mGameServerLib->SetKuafuURL(std::string(url));
				mGameServerLib->SetKuafuPort(std::string(port));
				mGameServerLib->SetLocalDomain(std::string(localUrl));
				mGameServerLib->GetListener()->SaveServerVar(*mGameServerLib,0);
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [kuafuurl kuafuurl kuafuport localurl]");
			}
		}
		if( command.substr(0,13) == "cleankuafuurl" || command.substr(0,10) == "cleankfurl" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			std::string _str;
			mGameServerLib->SetKuafuURL(_str);
			mGameServerLib->SetKuafuPort(_str);
			mGameServerLib->SetLocalDomain(_str);
			return std::string("done");
		}
		if(command.substr(0,9) == "kuafuopen") //kuafuopen 5
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char kuafusc[256];
			if(1 == sscanf(command.c_str(),"kuafuopen%*[ \t]%s",kuafusc))
			{
				std::string sj = std::string(kuafusc);
				if( sj== "5")
				{
					mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.onTalkkuafusj5");
				}
				else if(sj == "10")
				{
					mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.onTalkkuafusj10");
				}
				else if(sj == "15")
				{
					mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.onTalkkuafusj15");
				}
				else if(sj == "30")
				{
					mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.onTalkkuafusj30");
				}
				else if(sj == "60")
				{
					mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.onTalkkuafusj60");
				}
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [kuafuopen kuafusc]");
			}
		}
		if(command.substr(0,10) == "kuafuclose") //kuafuopen 5
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			if(0 == sscanf(command.c_str(),"kuafuclose"))
			{
				mGameServerLib->GetScriptEngine()->SystemScript("gm.gm.processkuafuclose");
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [kuafuclose]");
			}
		}
		if(command.substr(0,10) == "addconnect")
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char domain[256],port[256];
			if(2 == sscanf(command.c_str(),"addconnect%*[ \t]%s%*[ \t]%s",domain,port))
			{
				this->mGameServerLib->GetListener()->ConnectService(domain,port);
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [addconnect domain/ip port]");
			}
		}
		if (command.substr(0,16) == "cleanplayerchart")
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			char name[256];
			int chart_type = 0;
			int ret = sscanf_s(command.c_str(),"cleanplayerchart%*[ \t]%s%d",name,_countof(name),&chart_type);
			if(ret>0)
			{
				this->mGameServerLib->GetChartManager()->CleanChartByName(name, chart_type);
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [cleanplayerchart name (chat_type)]");
			}
			
		}

		if( command.substr(0,10) == "cleanchart" )
		{
			if( ! CheckGMLevel(clv,1000) )
			{
				return std::string("authority failed");
			}
			int ret=0;
			int chart_type = 0;
			int rank = 0;
			ret = sscanf(command.c_str(),"cleanchart%d%d",&chart_type,&rank);
			if(ret==1)
			{
				this->mGameServerLib->GetChartManager()->CleanChart(chart_type);
				return std::string("done");
			}
			else if (ret==2)
			{
				this->mGameServerLib->GetChartManager()->CleanChartByRank(chart_type, rank);
				return std::string("done");
			}
			else
			{
				return std::string("param error,try [cleanchart chart_type (rank)]");
			}
		}
		return std::string("cmd not found");
	}
	bool GMCommand::CheckGMLevel(int clv,int need)
	{
		if( clv >= need )return true;
		return false;
	}
	int GMCommand::GetGMLevel(const char* name)
	{
		std::map<std::string,int>::iterator pos = mAdminList.find(name);
		if( pos != mAdminList.end() )
		{
			return pos->second;
		}
		return 0;
	}
	void GMCommand::DoCommand(DomainPlay* src,std::string command)
	{
		if( mAdminList.end() == mAdminList.find(src->Name()) )
		{
			return ;
		}
		int clv = mAdminList.at(src->Name());
		if( command.length()> 256 )return;

		std::string ret = DoSystemCommand(command,clv);
		if( ! ret.empty() && ret!=std::string("cmd not found"))
		{
			src->AlertMessage(10,0,ret.c_str());
		}
		if( command.substr(0,4) == mCommandList["move"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				src->AlertMessage(10,0,"authority failed");
				return ;
			}
			int x,y;
			if( 2 == sscanf_s(command.c_str(),"move%d%d",&x,&y) )
			{
				src->ForceMove(x,y,4);
			}
			else
			{
				src->AlertMessage(10,0,"param error,try [move x y]");
			}
		}
		if( command.substr(0,2) == mCommandList["se"] )
		{
			if( ! CheckGMLevel(clv,100) )
			{
				src->AlertMessage(10,0,"authority failed");
				return ;
			}
			std::string sc = command.substr(3);
			src->GMTalk(sc.c_str());
		}
		if( command.substr(0,4) == mCommandList["gmov"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				src->AlertMessage(10,0,"authority failed");
				return ;
			}
			char name[256];
			int x,y;
			if(3 == sscanf_s(command.c_str(),"gmov%*[ \t]%s%d%d",name,_countof(name),&x,&y) )
			{
				std::string nm = name;
				src->EnterMap(nm,x,y);
			}
			else
			{
				src->AlertMessage(10,0,"param error,try [gmov new_map_name x y]");
			}
		}
		if( command.compare(mCommandList["hidetalkon"]) == 0 )
		{
			src->mGMHideTalk = 1;
			src->AlertMessage(10,0,"chat message no public to other player");
		}
		if( command.compare(mCommandList["hidetalkoff"]) == 0 )
		{
			src->mGMHideTalk = 0;
			src->AlertMessage(10,0,"chat message normal");
		}
		if( command.compare(mCommandList["nodamageon"] ) == 0 )
		{
			src->AddStatus(15,0xFFFFFFF,10000);
		}
		if( command.compare(mCommandList["gmyinshenon"] ) == 0 )
		{
			src->AddStatus(16,0xFFFFFFF,10000);
		}
		if( command.compare(mCommandList["nodamageoff"] ) == 0 )
		{
			src->ClearStatus(15);
		}
		if( command.compare(mCommandList["gmyinshenoff"] ) == 0 )
		{
			src->ClearStatus(16);
		}
		if ( command.compare(mCommandList["succeed"] ) == 0 )
		{
			src->KillDart();
		}
		if( command.substr(0,4) == mCommandList["call"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				src->AlertMessage(10,0,"authority failed");
				return ;
			}
			char name[256];
			if(1 == sscanf_s(command.c_str(),"call%*[ \t]%s",name,_countof(name)) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer()  && src->GetMap() )
				{
					gs->GetPlayer()->EnterMap(src->GetMap()->GetID(),src->X(),src->Y()+1);
					src->AlertMessage(10,0,"done");
				}
				else
				{
					src->AlertMessage(10,0,"target not find");
				}
			}
			else
			{
				src->AlertMessage(10,0,"param error,try [call character_name]");
			}
		}
		if( command.substr(0,6) == mCommandList["follow"] )
		{
			if( ! CheckGMLevel(clv,1) )
			{
				src->AlertMessage(10,0,"authority failed");
				return ;
			}
			char name[256];
			if(1 == sscanf_s(command.c_str(),"follow%*[ \t]%s",name,_countof(name)) )
			{
				std::string nm = name;
				GameSession* gs = this->mGameServerLib->FindSessionWithCharID(nm);
				if( gs && gs->GetPlayer()  && gs->GetPlayer()->GetMap() )
				{
					src->EnterMap(gs->GetPlayer()->GetMap()->GetID(),gs->GetPlayer()->X(),gs->GetPlayer()->Y()+1);
					src->AlertMessage(10,0,"done");
				}
				else
				{
					src->AlertMessage(10,0,"target not find");
				}
			}
			else
			{
				src->AlertMessage(10,0,"param error,try [follow character_name]");
			}
		}
		if( command.substr(0,3) == mCommandList["mob"] )
		{
			if( ! CheckGMLevel(clv,10) )
			{
				src->AlertMessage(10,0,"authority failed");
				return ;
			}
			//char name[256];
			int minid;
			int num = 1,range = 1,time = 1;
			int nolive = 1;
			char flags[256];
			memset(flags,0,256);
			int ret = sscanf_s(command.c_str(),"mob%d%d%d%d%d%*[ \t]%s",&minid,&num,&range,&time,&nolive,flags);
			if( ret = 1 || ret == 4 || ret == 5 || ret == 6 )
			{
				mGameServerLib->GetMapManager()->GenMon(src->GetMap(),minid,src->X(),src->Y(),num,range,time,nolive,"update");
				src->AlertMessage(10,0,"monster gened");
			}
			else
			{
				src->AlertMessage(10,0,"param error, try [mob monster_id number range relive_time_in_second]");
			}
		}

		if( command.substr(0,4) == mCommandList["make"] )
		{
			if( ! CheckGMLevel(clv,10) )
			{
				src->AlertMessage(10,0,"authority failed");
				return ;
			}
			char itemname[256];
			int num = 1;
			int bind = 0;
			int ret = sscanf_s(command.c_str(),"make%*[ \t]%s%d%d",itemname,_countof(itemname),&num,&bind);
			if( ret == 1 )
			{
				ret = 2;
				num = 1;
			}
			if( ret == 2 )
			{
				ItemDef* id = mGameServerLib->GetItemDefineManager()->GetItemDefine(itemname);
				if( ! id )
				{
					if( isdigit(itemname[0]) )
					{
						id = mGameServerLib->GetItemDefineManager()->GetItemDefine(atoi(itemname));
					}
				}
				if( id )
				{
					if( id->mTypeID == ITEM_VCOIN_ID )
					{
						if( ! CheckGMLevel(clv,1000) )
						{
							src->AlertMessage(10,0,"authority failed");
							return ;
						}
					}
					SubItem ii;
					int flags = 0;
					if( clv <= 100 )
					{
						flags = ITEM_FLAG_USE_BIND;
					}
					ii.BuildItemInstance(id,flags,false,mGameServerLib->mItemAddType);
					src->AddItem(&ii,num,102,11,1);
					src->AlertMessage(10,0,"item added");
				}
				else
				{
					src->AlertMessage(10,0,"item not found");
				}
			}
			else
			{
				src->AlertMessage(10,0,"param error,try [make item_name number=1]");
			}
		}
	}
}