#include "stdafx.h"
#include "leudgrid/toolsstring.h"   
namespace LEUD
{
	int Buffer2String(const char* pBuffer, const unsigned int pBufferSize, std::string& str)
	{
		char out;
		str.clear();
		unsigned char from;
		for( unsigned int i = 0;i < pBufferSize; i++ )
		{
			from = pBuffer[i];
			out = 'C' + (from & 0xF);
			str += out;
			out = 'd' + ((from & 0xF0) >> 4);
			str += out;
		}
		return 0;
	}
	int String2Buffer(const std::string& str, char* pBuffer, unsigned int* pBufferSize)
	{
		unsigned int bs=*pBufferSize;
		if( str.length() % 2 ) return 1;
		size_t buflen = str.length()/2;
		if( bs < buflen ) return 2;
		memset(pBuffer,0,bs);
		char out;
		for(size_t i = 0;i < buflen ;i ++)
		{
			char f = str.at(i*2);
			char e = str.at(i*2+1);
			out = ((e-'d')<<4) + (f-'C');
			pBuffer[i] = out;
		}
		*pBufferSize = static_cast<unsigned int>(buflen);
		return 0;
	}
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
	//void UtilString::string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst )
	//{
	//	std::string::size_type pos = 0;
	//	std::string::size_type srclen = strsrc.size();
	//	std::string::size_type dstlen = strdst.size();
	//	while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
	//	{
	//		strBig.replace( pos, srclen, strdst );
	//		pos += dstlen;
	//	}
	//}
	void UtilString::strrep(char* src,const char* patten,char des)
	{
		size_t pl = strlen(patten);
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
			}
		}
	}
}