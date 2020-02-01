#include "stdafx.h"
#include "leudgrid/ToolsPath.h"
#include "leudgrid/ToolsConsole.h"
#include <shlobj.h>

namespace LEUD
{
	std::string BinaryPath::path()
	{
		char path[MAX_PATH];
		GetModuleFileName(NULL,path,MAX_PATH);
        char *p = strrchr(path, '\\');
        *p = '\0';
		return path;
	}
	std::string BinaryPath::name()
	{
		char path[MAX_PATH];
		GetModuleFileName(NULL,path,MAX_PATH);
        char *p = strrchr(path, '\\');
		return ++p;
	}
	std::string BinaryPath::full()
	{
		char path[MAX_PATH];
		GetModuleFileName(NULL,path,MAX_PATH);
		return path;
	}
	BinaryPathAsCurrentPath::BinaryPathAsCurrentPath(bool reset)
	{
		m_reset = reset;
		char path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,path);
		m_OldPath = path;
		SetCurrentDirectory( BinaryPath::path().c_str() );
	}
	BinaryPathAsCurrentPath::~BinaryPathAsCurrentPath()
	{
		if( m_reset )
		{
			SetCurrentDirectory(m_OldPath.c_str());
		}
	}
	std::string SpecialFolderPath::LocalAppData()
	{
		char path[MAX_PATH];path[0]=0;
		SHGetSpecialFolderPath(0,path,CSIDL_LOCAL_APPDATA,0);
		return path;
	}
	std::string SpecialFolderPath::CommonLocalAppData()
	{
		char path[MAX_PATH];path[0]=0;
		SHGetSpecialFolderPath(0,path,CSIDL_COMMON_APPDATA,0);
		return path;
	}	
	void load_string(const char* section,const char* key,const char* def,char* out,size_t out_len,std::string& config_file)
	{
		GetPrivateProfileString(section,key,def,out,out_len,config_file.c_str());
		printf("@%s %s : ",section,key);
		ConsoleColorIntensityBackGreen();
		printf("%s\n",out);
		ConsoleColorIntensity();
	}
	int load_int(const char* section,const char* key,int def,std::string& config_file)
	{
		int ret = GetPrivateProfileInt(section,key,def,config_file.c_str());
		printf("@%s %s :",section,key);
		ConsoleColorIntensityBackGreen();
		printf("%d\n",ret);
		ConsoleColorIntensity();
		return ret;
	}
}