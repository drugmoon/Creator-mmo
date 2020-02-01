#pragma once

namespace OGSLib
{
	class UtilString
	{
	public:
		UtilString(void);
		virtual ~UtilString(void);
		static void utf8_2_act(const char* src,size_t src_len,char* des,size_t des_len);
		static void act_2_utf8(const char* src,size_t src_len,char* des,size_t des_len);
		//static void string_replace(std::string &strBig,const std::string &strsrc,const std::string &strdst);
		static void strrep(char* src,const char* patten,char des);
		static std::vector<std::string> split(const std::string& str, const std::string& patten);
	};
}