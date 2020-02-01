#include "stdafx.h"
#include "leudgrid/ToolsHTTP.h"

namespace LEUD
{
	GetHttpUrl::GetHttpUrl()
	{
		m_DataLen = 0;
	}
	const char* GetHttpUrl::Data()
	{
		return m_Data;
	}
	bool GetHttpUrl::Get(const char* url)
	{
		bool ret = false;
		memset(m_Data,0,M_DATA_LEN);
		m_DataLen = 0;
		HINTERNET hSession = InternetOpen("OncientWeb", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if(hSession != NULL)
		{
			HINTERNET hHttp = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
			if (hHttp != NULL)
			{
				DWORD readed =0;
				while( (m_DataLen < M_DATA_LEN) && InternetReadFile(hHttp,m_Data+m_DataLen,static_cast<DWORD>(M_DATA_LEN-m_DataLen),&readed) )
				{
					if( readed == 0 )
					{
						ret = true;
						break;
					}
					m_DataLen += readed;
				}
				m_Data[M_DATA_LEN-1]=0;
				InternetCloseHandle(hHttp);
				hHttp = NULL;
			}
			InternetCloseHandle(hSession);
			hSession = NULL;
		} 
		return ret;
	}
}