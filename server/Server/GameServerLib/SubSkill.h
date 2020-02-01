#pragma once
namespace OGSLib
{
	class SkillDef;
	class SubSkill
	{
	public:
		int mTypeID;
		int mLevel;
		int mExp;
		double mTick;
		int mParam1;
		int mParam2;
	public:
		SubSkill();
		bool add_exp(SkillDef* define,int exp,int player_lv);
		//bool check_level_up(SkillDef* define,int player_lv);
		int check_level_up(SkillDef* define,int player_lv);
	};
}