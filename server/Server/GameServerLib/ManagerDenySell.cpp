#include "StdAfx.h"
#include "ManagerDenySell.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"

namespace OGSLib
{
	ManagerDenySell::ManagerDenySell(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerDenySell::~ManagerDenySell(void)
	{
	}
	bool ManagerDenySell::Init()
	{
		return LoadDeny();
	}
	void ManagerDenySell::Dest()
	{
		SaveDeny();
	}
	bool ManagerDenySell::IsDeny(int type_id)
	{
		if( mDenyChrs.end() != std::find(mDenyChrs.begin(),mDenyChrs.end(),type_id) )
		{
			return true;
		}
		return false;
	}
	bool ManagerDenySell::LoadDeny()
	{
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("denysell.txt",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int s = atoi(line);
					if( s > 1000 )
					{
						mDenyChrs.push_back(s);
					}
				}
			}
		}
		return true;
	}
	bool ManagerDenySell::SaveDeny()
	{
		return true;
	}
}