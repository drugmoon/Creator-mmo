#pragma once
#include "PlayMapObject.h"

namespace OGSLib
{
	class DomainPlay;
	class ScriptLua;
	class GameServerLib;
	class SubItem;
	class PlayNpc :
		public PlayMapObject
	{
	public:
		struct ShopGoodClass
		{
			int page;
			int pos;
			int good_id;
			int type_id;
			int price_type;
			int price;
			int oldprice;
			int number;
			int good_tag;
		};
		typedef std::map<int,ShopGoodClass> ShopGoodMap;
	private:
		friend class ManagerMap;
		std::string mName;
		std::string mShortName;
		std::string mDialog;
		ScriptLua* mScript;
		std::string mMessage;
		ShopGoodMap mShopGoods;
		int mDefId;
		int m_iAcceptMsg;
		int m_iContinueMsg;
		int m_iDoneMsg;

		char mScriptPackage[256];
		char m_ScriptonRefreshShowFlags[256];
		char m_ScriptonShop[256];
	public:
		int mDirectFlyID;
		int mShowNpcFlag;
		std::string mShopFile;
		bool mCheckRange;
		bool mSell;
		bool mRepair;
		int mChangeName;
		PlayNpc(GameServerLib* gsl);
		virtual ~PlayNpc(void);
	public:
		virtual const char* Name(){return mName.c_str();}
		virtual const char* ShortName(){return mShortName.c_str();}
		virtual void Name(std::string name){mName = name;}
		virtual int Hp(){return 0;}
		virtual int flyId(){return mDirectFlyID;}
		virtual int MaxHp(){return 0;}
		virtual int Power(){return 0;}
		virtual int DefId(){return mDefId;}
		ShopGoodMap* GetGoods(){return &mShopGoods;}
	public:
		void Echo(const char* msg){mMessage.append(msg);}
		void ShowAccept(int msg){m_iAcceptMsg = msg;}
		void ShowContinue(int msg){m_iContinueMsg = msg;}
		void ShowDone(int msg){m_iDoneMsg = msg;}
		void CheckAndSendMessage(DomainPlay* src);
		void RefreshShowFlags(DomainPlay* src);
		void ParseOptions(std::string options);
		void SetOption(const char* option);

		void Talk(DomainPlay* src,const char* param);		
		void Sell(DomainPlay* src,SubItem* it,int number);
		void Shop(DomainPlay* src,int page);
		void Buy(DomainPlay* src,int page,int pos,int good_id,int type_id,int number);
		int Repair(DomainPlay* src,SubItem* item);
		void LoadScript(ScriptLua* script,const char* package);
		bool LoadShopGood(GameServerLib* lib);
		bool ReloadShopGood();
		bool CheckCanTalk(DomainPlay* src);
		const char* GetDialog(){return mDialog.c_str();};
	public:
		virtual void NotifyMeet(PlayMap* map,PlayMapObject* src);
		virtual void NotifyMapLeave(PlayMap* map);
		virtual void NotifyBye(PlayMap* map,PlayMapObject* src){}
		virtual void NotifyAvatarChange(PlayMapObject* src){}
	};
}