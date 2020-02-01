#pragma once
namespace LEUD
{
	class GetHttpUrl
	{
		static const int M_DATA_LEN=1024*1024;
		char m_Data[M_DATA_LEN];
		size_t m_DataLen;
	public:
		GetHttpUrl();
		bool Get(const char* url);
		const char* Data();
	};
}