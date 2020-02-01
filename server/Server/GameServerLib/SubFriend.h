#pragma once
namespace OGSLib
{
class SubFriend
{
public:
	std::string mSeedName;
	std::string mName;
	// 前4个字节表示对方与自己的关系,后4个字节表示自己与对方的关系
	int mTitle;
	int mLevel;
	int mJob;
	int mGender;
	int mOnlineState;
	std::string mGuild;
	std::string mGuildID;
	static bool IsFriend(int nTitle);
	static bool IsMyEnemy(int nTitle);
	static bool IsOtherSideEnemy(int nTitle);
	static bool IsMyBlack(int nTitle);
	static bool IsOtherSideBlack(int nTitle);
public:
	SubFriend(void);
	virtual ~SubFriend(void);
};

class FrindArray
{
public:
	std::vector<SubFriend> reltionship;
	int nFriend;
	int nEnemy;
	int nBlack;
	FrindArray()
	{
		nFriend = 0;
		nEnemy = 0;
		nBlack = 0;
	}
	FrindArray& operator=(FrindArray& value)
	{
		nFriend = value.nFriend;
		nEnemy = value.nEnemy;
		nBlack = value.nBlack;
		reltionship = value.reltionship;
		return *this;
	}
	void clear() 
	{
		nFriend = 0;
		nEnemy = 0;
		nBlack = 0;
		reltionship.clear();
	};
};

}

