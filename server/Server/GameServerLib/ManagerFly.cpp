#include "StdAfx.h"
#include "ManagerFly.h"
#include "GameServerLib.h"
#include "leudgrid\ToolsMemFile.h"
#include "leudgrid\Cryptography.h"
namespace OGSLib
{
	ManagerFly::ManagerFly(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerFly::~ManagerFly(void)
	{
	}
	bool ManagerFly::Init()
	{
		return LoadData();
	}
	void ManagerFly::Dest()
	{
	}
	bool ManagerFly::LoadData()
	{
		size_t len= 0;
		int version = 100;

		int nodeid;
		char name[256];
		char showname[256];
		char srcid[64];
		int srcx;
		int srcy;
		char destid[64];
		int destx;
		int desty;
		int type;
		int destsize;
		int monlv;
		int isshow;
		int clothid;

		char temp[512];

		const char* data = mGameServerLib->GetListener()->LoadFile("directfly.csv",len);
		if( len && len > 0 )
		{
			LEUD::ToolsMemFile f(data,len);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						line[5] = 0;
						if( 0 == strcmp(line,"#v100") )
						{
							version = 100;
						}
						continue;
					}
					clothid = 0;
					int ret = sscanf_s(line,"%d,%[^,],%[^,],%[^,],%d,%d,%d,%d,%d,%[^,],%d,%d,%d,%d,%d",
						&nodeid,name,_countof(name),showname,_countof(showname),
						destid,_countof(destid),&destx,&desty,&destsize,&type,&isshow,
						srcid,_countof(srcid),&srcx,&srcy,&monlv,&clothid
						);
					if( 14 == ret ||13 == ret)
					{
						DirectFlyNode dfn;

						UtilString::act_2_utf8(showname,strlen(showname),temp,256);
						strncpy_s(showname,_countof(showname),temp,256);
						showname[255]=0;
						
						dfn.mNodeID=nodeid;
						if(strcmp(showname,"null") != 0)
						{
							dfn.mNodeName=showname;
						}
						dfn.mTargetMapID=destid;
						dfn.mTargetX=destx;
						dfn.mTargetY=desty;
						dfn.mTargetSize=destsize?destsize:1;
						dfn.mType=type;
						dfn.mStartMapID=srcid;
						dfn.mStartX=srcx;
						dfn.mStartY=srcy;
						dfn.mMonLv=monlv;
						dfn.mIsShow=isshow?1:0;
						dfn.mClothId = clothid;
						mDirectFlyNodes[nodeid]=dfn;

						if(type==1 && strcmp(destid,"null") != 0)
						{
							mMapConnNodeIDs[dfn.mStartMapID].push_back(nodeid);
						}
					}
				}
			}
		}

		return true;
	}

	DirectFlyNode* ManagerFly::FindNode(int nodeId)
	{
		DirectFlyNodeMap::iterator pos = mDirectFlyNodes.find(nodeId);
		if( pos != mDirectFlyNodes.end() )
		{
			return &pos->second;
		}
		return 0;
	}
}