#include "StdAfx.h"
#include "ManagerTask.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
namespace OGSLib
{

	ManagerTask::ManagerTask(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerTask::~ManagerTask(void)
	{
	}
	int ManagerTask::init()
	{
		size_t size= 0;
		const char* data1 = mGameServerLib->GetListener()->LoadFile("dailytask.csv",size);
		if( data1 && size > 0 )
		{
			LEUD::ToolsMemFile f(data1,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int id;
					char temp[256];
					char temp_detail[1024];
					char name[256];
					int needlevel;
					int times;
					char detail[1024];
					char mapid[256];
					int map_x;
					int map_y;
					int fly = 0;
					int ret = 0;
					ret = sscanf_s(line,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%[^,]",&id,name,_countof(name),&needlevel,&times,mapid,_countof(mapid),&map_x,&map_y,&fly,detail,_countof(detail));
					if (8==ret || ret==9)
					{
						dailyTask.ID = id;
						UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;
						dailyTask.TaskName = name;
						dailyTask.NeedLevel = needlevel;
						dailyTask.Times = times;
						UtilString::act_2_utf8(detail,strlen(detail),temp_detail,1024);strncpy_s(detail,_countof(detail),temp_detail,1023);detail[1023]=0;
						dailyTask.Detail = detail;
						UtilString::act_2_utf8(mapid,strlen(mapid),temp,256);strncpy_s(mapid,_countof(mapid),temp,255);mapid[255]=0;
						dailyTask.MapID = mapid;
						dailyTask.MapX = map_x;
						dailyTask.MapY = map_y;
						dailyTask.FlyID = fly;
						DailyTasks.push_back(dailyTask);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		const char* data2 = mGameServerLib->GetListener()->LoadFile("instancetask.csv",size);
		if( data2 && size > 0 )
		{
			LEUD::ToolsMemFile f(data2,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int id;
					char temp[256];
					char temp_detail[1024];
					char name[256];
					int times;
					int needlevel;
					char suggestion[1024];
					char mapid[256];
					int map_x;
					int map_y;
					int fly = 0;
					int ret =0;
					ret = sscanf_s(line,"%d,%[^,],%d,%d,%[^,],%d,%d,%d,%[^,]",&id,name,_countof(name),&times,&needlevel,mapid,_countof(mapid),&map_x,&map_y,&fly,suggestion,_countof(suggestion));
					if (8==ret || 9==ret)
					{
						instanceTask.ID = id;
						UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;
						instanceTask.TaskName = name;
						instanceTask.NeedLevel = needlevel;
						instanceTask.Times = times;
						UtilString::act_2_utf8(suggestion,strlen(suggestion),temp_detail,1024);strncpy_s(suggestion,_countof(suggestion),temp_detail,1023);suggestion[1023]=0;
						instanceTask.Suggestion = suggestion;
						UtilString::act_2_utf8(mapid,strlen(mapid),temp,256);strncpy_s(mapid,_countof(mapid),temp,255);mapid[255]=0;
						instanceTask.MapID = mapid;
						instanceTask.MapX = map_x;
						instanceTask.MapY = map_y;
						instanceTask.FlyID = fly;
						InstanceTasks.push_back(instanceTask);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		const char* data3 = mGameServerLib->GetListener()->LoadFile("everydaytask.csv",size);
		if( data3 && size > 0 )
		{
			LEUD::ToolsMemFile f(data3,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int id;
					char temp[256];
					char temp_detail[1024];
					char name[256];
					int needlevel;
					char tasktime[256];
					char detail[1024];
					char mapid[256];
					int map_x;
					int map_y;
					int fly = 0;
					int ret = 0;
					ret = sscanf_s(line,"%d,%[^,],%d,%[^,],%[^,],%d,%d,%d,%[^,]",&id,name,_countof(name),&needlevel,tasktime,_countof(tasktime),mapid,_countof(mapid),&map_x,&map_y,&fly,detail,_countof(detail));
					if (8==ret || 9==ret)
					{
						everydayTask.ID = id;
						UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;
						everydayTask.TaskName = name;
						everydayTask.NeedLevel = needlevel;
						UtilString::act_2_utf8(tasktime,strlen(tasktime),temp,256);strncpy_s(tasktime,_countof(tasktime),temp,255);tasktime[255]=0;
						everydayTask.TaskTime = tasktime;
						UtilString::act_2_utf8(detail,strlen(detail),temp_detail,1024);strncpy_s(detail,_countof(detail),temp_detail,1023);detail[1023]=0;
						everydayTask.Detail = detail;
						UtilString::act_2_utf8(mapid,strlen(mapid),temp,256);strncpy_s(mapid,_countof(mapid),temp,255);mapid[255]=0;
						everydayTask.MapID = mapid;
						everydayTask.MapX = map_x;
						everydayTask.MapY = map_y;
						everydayTask.FlyID = fly;
						EverydayTasks.push_back(everydayTask);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		const char* data4 = mGameServerLib->GetListener()->LoadFile("bossfresh.csv",size);
		if( data4 && size > 0 )
		{
			LEUD::ToolsMemFile f(data4,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						continue;
					}
					int id;
					char temp[256];
					char temp_detail[1024];
					char name[256];
					int needlevel;
					char freshtime[256];
					char detail[1024];
					char mapname[256];
					char mapid[256];
					int map_x;
					int map_y;
					int flag;
					int fly = 0;
					int ret = 0;
					ret = sscanf_s(line,"%d,%[^,],%d,%[^,],%[^,],%[^,],%d,%d,%d,%d,%[^,]",&id,name,_countof(name),&needlevel,freshtime,_countof(freshtime),mapname,_countof(mapname),mapid,_countof(mapid),&map_x,&map_y,&flag,&fly,detail,_countof(detail));
					if (ret==10 || ret==11)
					{
						bossFresh.ID = id;
						UtilString::act_2_utf8(name,strlen(name),temp,256);strncpy_s(name,_countof(name),temp,255);name[255]=0;
						bossFresh.BossName = name;
						bossFresh.NeedLevel = needlevel;
						UtilString::act_2_utf8(freshtime,strlen(freshtime),temp,256);strncpy_s(freshtime,_countof(freshtime),temp,255);freshtime[255]=0;
						bossFresh.FreshTime = freshtime;
						UtilString::act_2_utf8(detail,strlen(detail),temp_detail,1024);strncpy_s(detail,_countof(detail),temp_detail,1023);detail[1023]=0;
						bossFresh.Detail = detail;
						UtilString::act_2_utf8(mapname,strlen(mapname),temp,256);strncpy_s(mapname,_countof(mapname),temp,255);mapname[255]=0;
						bossFresh.MapName = mapname;
						UtilString::act_2_utf8(mapid,strlen(mapid),temp,256);strncpy_s(mapid,_countof(mapid),temp,255);mapid[255]=0;
						bossFresh.MapID = mapid;
						bossFresh.MapX = map_x;
						bossFresh.MapY = map_y;
						bossFresh.Flag = flag;
						bossFresh.FlyID = fly;
						BossFreshs.push_back(bossFresh);
					}
				}
			}
		}
		return 0;
	}
	void ManagerTask::Dest()
	{
	}
}
