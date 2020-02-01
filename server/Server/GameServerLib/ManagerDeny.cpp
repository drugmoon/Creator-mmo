#include "StdAfx.h"
#include "ManagerDeny.h"
#include "GameServerLib.h"
namespace OGSLib
{
	ManagerDeny::ManagerDeny(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}

	ManagerDeny::~ManagerDeny(void)
	{
	}
	bool ManagerDeny::Init()
	{
		LoadLimit();
		LoadDenyIP();
		return LoadDeny();
	}
	void ManagerDeny::Dest()
	{
		SaveLimit();
		SaveDeny();
		SaveDenyIP();
	}
	bool ManagerDeny::CheckChrDeny(const std::string& chrname)
	{
		if( std::find(mDenyChrs.begin(),mDenyChrs.end(),chrname) != mDenyChrs.end() )
		{
			return true;
		}
		return false;
	}
	bool ManagerDeny::AddChrDeny(const std::string& chrname)
	{
		if( std::find(mDenyChrs.begin(),mDenyChrs.end(),chrname) != mDenyChrs.end() )
		{
			return true;
		}
		mDenyChrs.push_back(chrname);
		return true;
	}
	bool ManagerDeny::RemChrDeny(const std::string& chrname)
	{
		mDenyChrs.erase(std::remove(mDenyChrs.begin(),mDenyChrs.end(),chrname),mDenyChrs.end());
		return true;
	}
	bool ManagerDeny::LoadDeny()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"denychr.txt";
		std::ifstream ifs(mapinfofile.c_str());
		std::string line;
		if( ifs )
		{
			mDenyChrs.clear();
			while( std::getline(ifs,line) )
			{
				if( line.length()> 0 )
				{
					if( line.substr(0,1) == "#" || line.substr(0,1) == ";" )
					{
						continue;
					}
					char temp[1024];
					UtilString::act_2_utf8(line.c_str(),line.length(),temp,1024);
					line = temp;
					mDenyChrs.push_back(line);
				}
			}
		}
		return true;
	}
	bool ManagerDeny::SaveDeny()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"denychr.txt";
		std::ofstream ofs(mapinfofile);
		char temp[1024];
		if( ofs )
		{
			for(std::vector<std::string>::iterator pos = mDenyChrs.begin();
				pos != mDenyChrs.end();
				++pos )
			{
				UtilString::utf8_2_act(pos->c_str(),pos->length(),temp,1024);
				ofs<<temp<<std::endl;
			}
			ofs<<std::endl;
		}
		return true;
	}

	bool ManagerDeny::CheckDenyIP(const char* ip)
	{
		if( std::find(mDenyIPs.begin(),mDenyIPs.end(),ip) != mDenyIPs.end() )
		{
			return true;
		}
		return false;
	}
	bool ManagerDeny::AddDenyIP(const char* ip)
	{
		if( std::find(mDenyIPs.begin(),mDenyIPs.end(),ip) != mDenyIPs.end() )
		{
			return true;
		}
		mDenyIPs.push_back(ip);
		return true;
	}
	bool ManagerDeny::RemDenyIP(const char* ip)
	{
		mDenyIPs.erase(std::remove(mDenyIPs.begin(),mDenyIPs.end(),ip),mDenyIPs.end());
		return true;
	}
	bool ManagerDeny::LoadDenyIP()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"denyip.txt";
		std::ifstream ifs(mapinfofile.c_str());
		std::string line;
		if( ifs )
		{
			mDenyIPs.clear();
			while( std::getline(ifs,line) )
			{
				if( line.length()> 0 )
				{
					if( line.substr(0,1) == "#" || line.substr(0,1) == ";" )
					{
						continue;
					}
					//char temp[1024];
					//UtilString::act_2_utf8(line.c_str(),line.length(),temp,1024);
					//line = temp;
					mDenyIPs.push_back(line);
				}
			}
		}
		return true;
	}
	bool ManagerDeny::SaveDenyIP()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"denyip.txt";
		std::ofstream ofs(mapinfofile);
		char temp[1024];
		if( ofs )
		{
			for(std::vector<std::string>::iterator pos = mDenyIPs.begin();
				pos != mDenyIPs.end();
				++pos )
			{
				ofs<<pos->c_str()<<std::endl;
			}
			ofs<<std::endl;
		}
		return true;
	}

	bool ManagerDeny::CheckChrLimit(const std::string& chrname)
	{
		if( std::find(mLimitChrs.begin(),mLimitChrs.end(),chrname) != mLimitChrs.end() )
		{
			return true;
		}
		return false;
	}
	bool ManagerDeny::AddChrLimit(const std::string& chrname)
	{
		if( std::find(mLimitChrs.begin(),mLimitChrs.end(),chrname) != mLimitChrs.end() )
		{
			return true;
		}
		mLimitChrs.push_back(chrname);
		return true;
	}
	bool ManagerDeny::RemChrLimit(const std::string& chrname)
	{
		mLimitChrs.erase(std::remove(mLimitChrs.begin(),mLimitChrs.end(),chrname),mLimitChrs.end());
		return true;
	}
	bool ManagerDeny::LoadLimit()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"limitchr.txt";
		std::ifstream ifs(mapinfofile.c_str());
		std::string line;
		if( ifs )
		{
			mLimitChrs.clear();
			while( std::getline(ifs,line) )
			{
				if( line.length()> 0 )
				{
					if( line.substr(0,1) == "#" || line.substr(0,1) == ";" )
					{
						continue;
					}
					char temp[1024];
					UtilString::act_2_utf8(line.c_str(),line.length(),temp,1024);
					line = temp;
					mLimitChrs.push_back(line);
				}
			}
		}
		return true;
	}
	bool ManagerDeny::SaveLimit()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"limitchr.txt";
		std::ofstream ofs(mapinfofile);
		char temp[1024];
		if( ofs )
		{
			for(std::vector<std::string>::iterator pos = mLimitChrs.begin();
				pos != mLimitChrs.end();
				++pos )
			{
				UtilString::utf8_2_act(pos->c_str(),pos->length(),temp,1024);
				ofs<<temp<<std::endl;
			}
			ofs<<std::endl;
		}
		return true;
	}
}