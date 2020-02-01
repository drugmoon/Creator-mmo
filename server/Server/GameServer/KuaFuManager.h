#pragma once
class KuaFuManager
{
private:
	std::map<std::string,time_t> mAuthTickMap;
	time_t mUpdateTick;
	int mTicketSeed;
public:
	KuaFuManager();
	virtual ~KuaFuManager();
	void AddAuthTick(std::string& tick);
	int CheckAuthTick(std::string& tick);
	int GenTicketSeed(){return mTicketSeed++;}
	void Update();
};