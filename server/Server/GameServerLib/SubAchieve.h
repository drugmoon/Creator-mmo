#pragma once
namespace OGSLib
{
	enum AchieveType
	{
		AchieveTypeFirstKillMonster=1001,
		AchieveTypeFirstAddFriend=1002,
		AchieveTypeFirstGroup=1003,
		AchieveTypeFirstGuild=1004,
		AchieveTypeFirstPKKill=1005,
		AchieveTypeFirstUpdEquip=1006,
		AchieveTypeFirstMergeSteel=1007,
	};
	class SubAchieve
	{
	public:
		int mAchieveID;
		int mState;
		int mParam;
	public:
		SubAchieve(void);
		virtual ~SubAchieve(void);
	};
}
