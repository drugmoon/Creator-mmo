#include "StdAfx.h"
#include "ManagerTrapDef.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include <sstream>

namespace OGSLib
{
	ManagerTrapDef::ManagerTrapDef(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerTrapDef::~ManagerTrapDef(void)
	{
	}

	bool ManagerTrapDef::init()
	{
		size_t size= 0;
		const char* data = mGameServerLib->GetListener()->LoadFile("trapdef.csv",size);
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
					TrapDef td;

					int caller_leave_remove = 0;

					int ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d",
						&td.id,&td.cloth,&td.life_time,&td.damage_gap, &td.damage, &td.skill_type, caller_leave_remove
						);
					td.caller_leave_remove = caller_leave_remove > 0 ? true : false;

					if( 7 == ret )
					{
						mTraps[td.id] = td;
					}
				}
			}
		}
		return mTraps.empty();
	}
}