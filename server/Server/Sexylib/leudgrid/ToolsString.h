#pragma once
namespace LEUD
{
	int Buffer2String(const char* pBuffer, const unsigned int pBufferSize, std::string& str);
	int String2Buffer(const std::string& str, char* pBuffer, unsigned int* pBufferSize);
	class UtilString
	{
	public:
		UtilString(void);
		virtual ~UtilString(void);
		static void utf8_2_act(const char* src,size_t src_len,char* des,size_t des_len);
		static void act_2_utf8(const char* src,size_t src_len,char* des,size_t des_len);
		//static void string_replace(std::string &strBig,const std::string &strsrc,const std::string &strdst);
		static void strrep(char* src,const char* patten,char des);
	};
}