#include "StdAfx.h"
#include "ManagerChart.h"
#include "GameServerLib.h"
namespace OGSLib
{
	ManagerChart::ManagerChart(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerChart::~ManagerChart(void)
	{
	}
	bool ManagerChart::Init()
	{
		mLogTick = time(0)/60/60;
		for(int c = 0;c < CHART_TYPE_NUM;c++)
		{
			ChartNode* chart = this->mData[c];
			for(int i = 0;i < ChartRankMax;i++)
			{
				chart[i].name[0]=0;
				chart[i].guild[0]=0;
				chart[i].title[0]=0;
				chart[i].fightpoint=0;
				chart[i].param = 0;
				chart[i].job = 0;
				chart[i].lv = 0;
				chart[i].zslv = 0;
				chart[i].gender=0;
			}
		}
		return true;
	}
	void ManagerChart::Dest()
	{
		LogData();
	}
	void ManagerChart::LogData()
	{
		
	}
	void ManagerChart::Update()
	{
		if( mLogTick < time(0)/60/60 )
		{
			mLogTick = time(0)/60/60;
			LogData();
		}
	}
	ChartNode* ManagerChart::GetChart(int chart_type)
	{
		if( chart_type >= CHART_TYPE_LEVEL_ALL && chart_type < CHART_TYPE_END )
		{
			return mData[chart_type-CHART_TYPE_LEVEL_ALL];
		}
		return 0;
	}
	void printChart(ChartNode* chart)
	{
		printf("===========================\n");
		for(int i = 0;i < ChartRankMax;i++)
		{
			printf("%d == %s %d\n",i,chart[i].name,chart[i].param);
		}
	}
	void ManagerChart::ClearEmptyData(int chart_type)
	{
		ChartNode* chart = GetChart(chart_type);
		if( ! chart )return ;
		int pos=0;
		for(int i = 0;i < ChartRankMax;i++)
		{
			if( chart[i].name[0] != 0 )
			{
				if( pos != i )
				{
					strncpy_s(chart[pos].name,_countof(chart[pos].name),chart[i].name,255);
					chart[pos].name[255]=0;
					strncpy_s(chart[pos].guild,_countof(chart[pos].guild),chart[i].guild,255);
					chart[pos].guild[255]=0;
					strncpy_s(chart[pos].title,_countof(chart[pos].title),chart[i].title,255);
					chart[pos].title[255]=0;
					chart[pos].param = chart[i].param;
					chart[pos].job = chart[i].job;
					chart[pos].gender = chart[i].gender;
					chart[pos].lv = chart[i].lv;
					chart[pos].zslv = chart[i].zslv;
					chart[i].name[0]=0;
					chart[i].guild[0]=0;
					chart[i].title[0]=0;
				}
				pos++;
			}
		}
		//if( chart_type==100 )printChart(chart);
	}
	void ManagerChart::setChartGuild(int chart_type, const char* name, const char* guildName)
	{
		ChartNode* chart = GetChart(chart_type);
		if (!chart)return;
		for (int i = 0; i < ChartRankMax; i++)
		{
			if (chart[i].name[0] != 0 && 0 == strcmp(chart[i].name, name))
			{
				strncpy_s(chart[i].guild, _countof(chart[i].guild), guildName, 255);
				chart[i].guild[255] = 0;
				break;
			}
		}
	}

	void ManagerChart::UpdateData(int chart_type,const char* name,int param,const char* guild,const char* title,int job,int lv,int zslv,int gender)
	{
		if (lv < mGameServerLib->mLevelChartLimit)
		{
			return;
		}
		ChartNode* chart = GetChart(chart_type);
		if( ! chart )return ;
		for(int i = 0;i < ChartRankMax;i++)
		{
			if( chart[i].name[0] != 0 && 0 == strcmp(chart[i].name,name) )
			{
				if( chart[i].param == param )
				{
					return;
				}
				chart[i].name[0]=0;
			}
		}
		ClearEmptyData(chart_type);
		int pos = 0;
		for(pos = 0;pos < ChartRankMax;pos++)
		{
			if( chart[pos].name[0] == 0 || chart[pos].param < param )
			{
				break;
			}
		}
		if( pos >= 0 && pos < ChartRankMax )
		{
			MakePosition(chart_type,pos);
			strncpy_s(chart[pos].name,_countof(chart[pos].name),name,255);
			chart[pos].name[255] = 0;
			strncpy_s(chart[pos].guild,_countof(chart[pos].guild),guild,255);
			chart[pos].guild[255] = 0;
			strncpy_s(chart[pos].title,_countof(chart[pos].title),title,255);
			chart[pos].title[255] = 0;
			chart[pos].param = param;
			chart[pos].job = job;
			chart[pos].lv = lv;
			chart[pos].zslv = zslv;
			chart[pos].gender = gender;
		//if( chart_type==100 )printChart(chart);
		}
	}
	int ManagerChart::GetChartLevel(int chart_type,const char* name)
	{
		ChartNode* chart = GetChart(chart_type);
		if( ! chart )return -1;
		for(int i = 0;i < ChartRankMax;i++)
		{
			if( chart[i].name[0] != 0 && 0 == strcmp(chart[i].name,name) )
			{
				return chart[i].param;
			}
		}
		return -1;
	}
	void ManagerChart::MakePosition(int chart_type,int pos)
	{
		ChartNode* chart = GetChart(chart_type);
		if( ! chart )return ;
		if( pos >= 0 && pos < ChartRankMax )
		{
			for(int i = ChartRankMax-1;i > pos;i--)
			{
				if( chart[i-1].name[0] != 0 )
				{
					strncpy_s(chart[i].name,_countof(chart[i].name),chart[i-1].name,255);
					chart[i].name[255]=0;
					strncpy_s(chart[i].guild,_countof(chart[i].guild),chart[i-1].guild,255);
					chart[i].guild[255]=0;
					strncpy_s(chart[i].title,_countof(chart[i].title),chart[i-1].title,255);
					chart[i].title[255]=0;
					chart[i].param = chart[i-1].param;
					chart[i].job = chart[i-1].job;
					chart[i].lv = chart[i-1].lv;
					chart[i].zslv = chart[i - 1].zslv;
					chart[i].gender = chart[i - 1].gender;
				}
			}
		}
		//if( chart_type==100 )printChart(chart);
	}
	ChartNode* ManagerChart::GetData(int chart_type,int page,int& num)
	{
		ChartNode* chart = GetChart(chart_type);
		if( ! chart )return 0;
		num = 0;
		//int pos = page*10;
		int pos = 0;
		//if( pos >= 0 && pos < ChartRankMax )
		//{
			for(int i = 0;i < ChartRankMax ;i ++ )
			{
				if( pos+i>=0&& (pos+i)<ChartRankMax )
				{
					if( chart[pos+i].name[0] != 0 )
					{
						if(mGameServerLib->FindSessionWithCharID(chart[pos+i].name))
						{
							chart[pos+i].state=1;
						}
						else
						{
							chart[pos+i].state=0;
						}
						num ++;
					}
					else
					{
						break;
					}
				}
			}
			return &chart[pos];
		//}
		//return 0;
	}

	void ManagerChart::CleanChart(int chart_type)
	{
		ChartNode* chart = GetChart(chart_type);
		for(int i = 0;i < ChartRankMax;i++)
		{
			chart[i].name[0]=0;
			chart[i].guild[0]=0;
			chart[i].title[0]=0;
			chart[i].fightpoint=0;
			chart[i].param = 0;
			chart[i].job = 0;
			chart[i].lv = 0;
			chart[i].zslv = 0;
			chart[i].gender=0;
		}
	}

	void ManagerChart::CleanChartByRank(int chart_type, int rank)
	{
		ChartNode* chart = GetChart(chart_type);
		if (rank < ChartRankMax)
		{
			chart[rank].name[0]=0;
			chart[rank].guild[0]=0;
			chart[rank].title[0]=0;
			chart[rank].fightpoint=0;
			chart[rank].param = 0;
			chart[rank].job = 0;
			chart[rank].lv = 0;
			chart[rank].zslv = 0;
			chart[rank].gender=0;

			ClearEmptyData(chart_type);
		}
	}

	void ManagerChart::CleanChartByName(const std::string& name, int chart_type)
	{
		if (chart_type == 0)
		{
			for(int c = 0;c < CHART_TYPE_NUM;c++)
			{

				CleanPlayerChart(name, CHART_TYPE_LEVEL_ALL + c);
			}
		}
		else
		{
			CleanPlayerChart(name, chart_type);
		}
	}

	void ManagerChart::CleanPlayerChart(const std::string& name, int chart_type)
	{
		ChartNode* chart = GetChart(chart_type);
		if( ! chart )return;
		for(int i = 0;i < ChartRankMax;i++)
		{
			if( chart[i].name[0] != 0 && 0 == strcmp(chart[i].name, name.c_str()) )
			{
				chart[i].name[0]=0;
				chart[i].guild[0]=0;
				chart[i].title[0]=0;
				chart[i].fightpoint=0;
				chart[i].param = 0;
				chart[i].job = 0;
				chart[i].lv = 0;
				chart[i].zslv = 0;
				chart[i].gender=0;

				ClearEmptyData(chart_type);
			}
		}
	}
}