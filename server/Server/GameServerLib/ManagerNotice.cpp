#include "StdAfx.h"
#include "ManagerNotice.h"
#include "GameServerLib.h"
namespace OGSLib
{
	ManagerNotice::ManagerNotice(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerNotice::~ManagerNotice(void)
	{
	}
	bool ManagerNotice::Init()
	{
		return LoadNotice();
	}
	void ManagerNotice::Dest()
	{
	}
	bool ManagerNotice::LoadNotice()
	{
		std::string mapinfofile = mGameServerLib->GetConfigPath()+"notice.txt";
		std::ifstream ifs(mapinfofile.c_str());
		std::string line;
		if( ifs )
		{
			mNotices.clear();
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
					mNotices.push_back(line);
				}
			}
		}
		return true;
	}
	bool ManagerNotice::ClearNotice()
	{
		mNotices.clear();
		return true;
	}
	void ManagerNotice::SaveNotice()
	{
		if( ! mNotices.empty() )
		{
			std::string mapinfofile = mGameServerLib->GetConfigPath()+"notice.txt";
			std::ofstream ofs;
			ofs.open(mapinfofile,ofs.trunc);
			char temp[1024];
			if( ofs )
			{
				for(std::vector<std::string>::iterator pos = mNotices.begin();
					pos != mNotices.end();
					++pos )
				{
					UtilString::utf8_2_act(pos->c_str(),pos->length(),temp,1024);
					ofs<<temp<<std::endl;
				}
				ofs<<std::endl;
			}
		}
	}
	std::string& ManagerNotice::get_notice(int id)
	{
		if( ! mNotices.empty() )
		{
			size_t i = id % mNotices.size();
			if( i >= 0 && mNotices.size() > i )
			{
				return mNotices[i];
			}
		}
		return mNoticeDefault;
	}
	std::string ManagerNotice::ShowNotices()
	{
		std::string str;
		for(std::vector<std::string>::iterator pos = mNotices.begin();
			pos != mNotices.end();
			++pos )
		{
			str.append("\n");
			str.append(pos->c_str());
		}
		return str;
	}
	bool ManagerNotice::AddNotice(std::string notice)
	{
		if (!notice.empty())
		{
			for(std::vector<std::string>::iterator pos = mNotices.begin();
				pos != mNotices.end();
				++pos )
			{
				if (0 == pos->compare(notice))
				{
					mNotices.erase(pos);
					pos = mNotices.begin();
				}
			}
			mNotices.push_back(notice);
			return true;
		}
		return false;
	}
	bool ManagerNotice::RemNotice(std::string notice)
	{
		if (!mNotices.empty() && !notice.empty())
		{
			for(std::vector<std::string>::iterator pos = mNotices.begin();
				pos != mNotices.end();
				++pos )
			{
				if (0 == pos->compare(notice))
				{
					mNotices.erase(pos);
					pos = mNotices.begin();
					return true;
				}
			}
		}
		return false;
	}
	bool ManagerNotice::RemNotice(int id)
	{
		if (!mNotices.empty())
		{
			size_t t = id;
			if( t >= 0 && mNotices.size() > t )
			{
				for(std::vector<std::string>::iterator pos = mNotices.begin();
					pos != mNotices.end();
					++pos )
				{
					if (0 == pos->compare(mNotices[id]))
					{
						mNotices.erase(pos);
						pos = mNotices.begin();
						return true;
					}
				}
			}
		}
		return false;
	}
}