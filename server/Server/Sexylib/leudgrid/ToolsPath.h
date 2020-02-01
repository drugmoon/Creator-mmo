#pragma once

namespace LEUD
{
	class BinaryPath
	{
	public:
		static std::string path();
		static std::string name();
		static std::string full();
	};
	class BinaryPathAsCurrentPath
	{
	public:
		explicit BinaryPathAsCurrentPath(bool reset = false);
		virtual ~BinaryPathAsCurrentPath();
	private:
		bool m_reset;
		std::string m_OldPath;
	};
	class SpecialFolderPath
	{
	public:
		static std::string LocalAppData();
		static std::string CommonLocalAppData();
	};
	void load_string(const char* section,const char* key,const char* def,char* out,size_t out_len,std::string& config_file);
	int load_int(const char* section,const char* key,int def,std::string& config_file);
}