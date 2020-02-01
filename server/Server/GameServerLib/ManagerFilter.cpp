#include "StdAfx.h"
#include "ManagerFilter.h"
#include "GameServerLib.h"
#include "UtilString.h"
namespace OGSLib
{
	ManagerFilter::ManagerFilter(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}

	ManagerFilter::~ManagerFilter(void)
	{
	}
	bool ManagerFilter::Init()
	{
		return LoadFilterWord();
	}
	void ManagerFilter::Dest()
	{
	}
	bool ManagerFilter::LoadFilterWord()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"filterword.txt";
		std::ifstream ifs(mapinfofile.c_str());
		std::string line;
		if( ifs )
		{
			mFilterWords.clear();
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
					mFilterWords.push_back(line);
				}
			}
		}
		return true;
	}
	bool ManagerFilter::checkFilter(std::string& msg)
	{
		for( FilterWordVector::iterator pos = mFilterWords.begin();
			pos != mFilterWords.end();
			++ pos )
		{
			if( msg.find((*pos)) != std::string::npos )
			{
				return false;
			}
		}
		return true;
	}
	bool ManagerFilter::FilterName(char* name)
	{
		Filter(name);
		UtilString::strrep(name,"`",'*');
		UtilString::strrep(name,"~",'*');
		UtilString::strrep(name,"!",'*');
		UtilString::strrep(name,"@",'*');
		UtilString::strrep(name,"#",'*');
		UtilString::strrep(name,"$",'*');
		UtilString::strrep(name,"%",'*');
		UtilString::strrep(name,"^",'*');
		UtilString::strrep(name,"&",'*');
		UtilString::strrep(name,"*",'_');
		UtilString::strrep(name,"(",'*');
		UtilString::strrep(name,")",'*');
		UtilString::strrep(name,"[",'*');
		UtilString::strrep(name,"]",'*');
		UtilString::strrep(name,"{",'*');
		UtilString::strrep(name,"}",'*');
		UtilString::strrep(name,";",'*');
		UtilString::strrep(name,":",'*');
		UtilString::strrep(name,"'",'*');
		UtilString::strrep(name,"\"",'*');
		UtilString::strrep(name,"/",'*');
		UtilString::strrep(name,"?",'*');
		UtilString::strrep(name," ",'*');
		UtilString::strrep(name,",",'*');
		UtilString::strrep(name,".",'*');
		UtilString::strrep(name,">",'*');
		UtilString::strrep(name,"<",'*');
		char temp[256];
		std::string key;

		key = "¡¡";
		UtilString::act_2_utf8(key.c_str(),key.length(),temp,256);
		key = temp;
		UtilString::strrep(name,key.c_str(),'*');
		key = "¡õ";
		UtilString::act_2_utf8(key.c_str(),key.length(),temp,256);
		key = temp;
		UtilString::strrep(name,key.c_str(),'*');
		key = "àí";
		UtilString::act_2_utf8(key.c_str(),key.length(),temp,256);
		key = temp;
		UtilString::strrep(name,key.c_str(),'*');
		key = "\n";
		UtilString::act_2_utf8(key.c_str(),key.length(),temp,256);
		key = temp;
		UtilString::strrep(name,key.c_str(),'*');
		key = "¡£";
		UtilString::act_2_utf8(key.c_str(),key.length(),temp,256);
		key = temp;
		UtilString::strrep(name,key.c_str(),'*');
		key = "£¬";
		UtilString::act_2_utf8(key.c_str(),key.length(),temp,256);
		key = temp;
		UtilString::strrep(name,key.c_str(),'*');
		return true;
	}
	bool ManagerFilter::Filter(char* msg)
	{
		std::string dststr;
		char cc;
		bool voice = false;
		bool nonum = false;
		if(strstr(msg,"<voice>"))
		{
			voice = true;
		}
		for( FilterWordVector::iterator pos = mFilterWords.begin();
			pos != mFilterWords.end();
			++ pos )
		{
			nonum = false;
			if(voice)
			{
				for(std::string::iterator ic = pos->begin();ic != pos->end(); ic++)
				{
					cc = (*ic);
					if(isalnum(cc)==0)
					{
						nonum = true;
						break;
					}
				}
				if(nonum)
				{
					UtilString::strrep(msg,pos->c_str(),'*');\
				}
			}
			else
			{
				UtilString::strrep(msg,pos->c_str(),'*');
			}
		}
		return true;
	}
}