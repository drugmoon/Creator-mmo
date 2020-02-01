#pragma once
namespace OGSLib
{
	class ChartNode
	{
	public:
		char name[256];
		char guild[256];
		char title[256];
		int param;
		int job;
		int fightpoint;
		int lv;
		char state;
		int zslv;
		int gender;
	};
	enum ChartType
	{
		CHART_TYPE_LEVEL_ALL=100,
		CHART_TYPE_LEVEL_WIR=101,
		CHART_TYPE_LEVEL_WIZ=102,
		CHART_TYPE_LEVEL_TAO=103,
		CHART_TYPE_GAMEMONEY=104,
		CHART_TYPE_ACHIEVE=105,
		CHART_TYPE_FIGHTPOINT_WIR=106,
		CHART_TYPE_FIGHTPOINT_WIZ=107,
		CHART_TYPE_FIGHTPOINT_TAO=108,
		CHART_TYPE_FIGHTPOINT_ALL=109,
		CHART_TYPE_BONUS_ALL=110,
		CHART_TYPE_WING_ALL=111,
		CHART_TYPE_TIANGUAN_ALL=112,
		CHART_TYPE_END=115,
		CHART_TYPE_NUM=16,
	};
	static const int ChartRankMax = 100;
	class GameServerLib;
	class ManagerChart
	{
		GameServerLib* mGameServerLib;
		ChartNode mData[CHART_TYPE_NUM][ChartRankMax];
		time_t mLogTick;
	public:
		ManagerChart(GameServerLib* gslib);
		virtual ~ManagerChart(void);

		bool Init();
		void Dest();
		void Update();
		
		void LogData();
		ChartNode* GetChart(int chart_type);
		void ClearEmptyData(int char_tchart_typeype);
		void UpdateData(int chart_type,const char* name,int param,const char* guild,const char* title,int job,int lv,int zslv,int gender);
		void MakePosition(int chart_type,int pos);
		int GetChartLevel(int chart_type,const char* name);
		ChartNode* GetData(int chart_type,int page,int& num);

		void setChartGuild(int chart_type, const char* name, const char* guildName);
		//清理指定榜单
		void CleanChart(int chart_type);
		//清理指定榜单的指定排名数据
		void CleanChartByRank(int chart_type, int rank);
		//依据玩家名字清理排行榜
		void CleanChartByName(const std::string& name, int chart_type = 0);

		void CleanPlayerChart(const std::string& name, int chart_type);
	};
}