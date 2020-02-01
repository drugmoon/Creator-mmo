#include "StdAfx.h"
#include "UtilString.h"
namespace OGSLib
{
	UtilString::UtilString(void)
	{
	}

	UtilString::~UtilString(void)
	{
	}
	void UtilString::utf8_2_act(const char* src,size_t src_len,char* des,size_t des_len)
	{
		*des = 0;
		wchar_t temp[1024];
		int wl = ::MultiByteToWideChar(CP_UTF8,0,src,(int)src_len,temp,1024);
		if( wl > 0 )
		{
			int ml = ::WideCharToMultiByte(CP_ACP,0,temp,wl,des,(int)des_len,0,false);
			des[ml]=0;
		}
		
	}
	void UtilString::act_2_utf8(const char* src,size_t src_len,char* des,size_t des_len)
	{
		*des = 0;
		wchar_t temp[1024];
		int wl = ::MultiByteToWideChar(CP_ACP,0,src,(int)src_len,temp,1024);
		if( wl > 0 )
		{
			int ml = ::WideCharToMultiByte(CP_UTF8,0,temp,wl,des,(int)des_len,0,false);
			des[ml]=0;
		}
	}

	void UtilString::strrep(char* src,const char* patten,char des)
	{
		size_t pl = strlen(patten);
		int limit=0;
		if( pl > 0 )
		{
			char* p=src;
			while( (p = strstr(p,patten)) > 0 )
			{
				for(size_t i = 0;i< pl;i ++)
				{
					*p = des;
					++p;
				}
				limit++;
				if(limit>100)
				{
					printf("strrep too long");
					break;
				}
			}
		}
	}

	std::vector<std::string> UtilString::split(const std::string& text, const std::string& key)
	{
		std::vector<std::string> strvec;
		std::string strtemp;

		std::string::size_type pos1, pos2;
		pos2 = text.find(key);
		pos1 = 0;
		while (std::string::npos != pos2)
		{
			strvec.push_back(text.substr(pos1, pos2 - pos1));

			pos1 = pos2 + 1;
			pos2 = text.find(key, pos1);
		}
		strvec.push_back(text.substr(pos1));

		return strvec;
	}
}