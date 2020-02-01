#include "StdAfx.h"
#include "PlayNpc.h"
#include "DomainPlay.h"
#include "ScriptLua.h"
#include "GameServerLib.h"
#include "ManagerMap.h"
#include "ManagerItemDef.h"
#include "PlayMap.h"
#include "SubItem.h"
#include "GameSession.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
namespace OGSLib
{
	PlayNpc::PlayNpc(GameServerLib* gsl)
	{
		mType = MAP_OBJECT_TYPE_NPC;
		mGSL = gsl;
		mCloth = 300101;
		mWeapon = -1;
		mHair = -1;

		mDir = 4;

		mScript = 0;

		mCheckRange=true;
		mSell=true;
		mRepair=true;
		mMessage.reserve(1024);
		mShowNpcFlag = 0;
		mDirectFlyID = 0;
		mChangeName = 0;
		mDialog = "";
	}

	PlayNpc::~PlayNpc(void)
	{
	}
	void PlayNpc::NotifyMapLeave(PlayMap* map)
	{
		mGSL->GetFrameTimer()->RemoveObject(this);
		PlayMapObject::NotifyMapLeave(map);
	}
	void PlayNpc::NotifyMeet(PlayMap* map,PlayMapObject* src)
	{
		if( src->TypeIsPlayer() )
		{
			DomainPlay* player = static_cast<DomainPlay*>(src);
			//std::stringstream ss;ss<<mScriptPackage<<".onMeet";
			//mScript->NpcPlayer(this,player,ss.str());
			//CheckAndSendMessage(player);
			RefreshShowFlags(player);
		}
	}
	void PlayNpc::RefreshShowFlags(DomainPlay* src)
	{
		if( src )
		{
			//std::stringstream ss;ss<<mScriptPackage<<".onRefreshShowFlags";
			mScript->NpcPlayer(this,src,m_ScriptonRefreshShowFlags);
		}
	}
	void PlayNpc::CheckAndSendMessage(DomainPlay* src)
	{
		//if( mMessage.length() > 0 )
		{
			int flags = 0;
			int param = 0;
			if( m_iAcceptMsg )
			{
				flags = 1;
				param = m_iAcceptMsg;
			}
			if( m_iContinueMsg )
			{
				flags = 2;
				param = m_iContinueMsg;
			}
			if( m_iDoneMsg )
			{
				flags = 4;
				param = m_iDoneMsg;
			}
			src->NotifyNPCTalk(this,flags,param,mMessage);
		}
		mMessage.clear();
		m_iAcceptMsg=0;
		m_iContinueMsg=0;
		m_iDoneMsg=0;
	}
	bool PlayNpc::CheckCanTalk(DomainPlay* src)
	{
		if( mCheckRange )
		{
			if( std::abs(X()-src->X()) + std::abs(Y()-src->Y()) > 100 )
			{
				return false;
			}
		}
		return true;
	}
	void PlayNpc::Talk(DomainPlay* src,const char* param)
	{
		if( ! CheckCanTalk(src) ) return ;
		char ss[256];
		_snprintf_s(ss,255,255,"%s.onTalk%s",mScriptPackage,param);
		ss[255]=0;
		int ret = mScript->RetNpcPlayer(this,src,ss);
		//ret = 100;
		if ( ret > 1 )
		{
			_snprintf_s(ss,255,255,"%s.onTalk%d",mScriptPackage,ret);
			ss[255]=0;
			mScript->RetNpcPlayer(this,src,ss);
		}
		CheckAndSendMessage(src);
	}
	void PlayNpc::Shop(DomainPlay* src,int page)
	{
		if( ! CheckCanTalk(src) ) return ;
		mMessage.clear();
		mScript->NpcPlayer(this,src,m_ScriptonShop);
		if( mMessage.length() > 0 )
		{
			src->NotifyNPCShop(this,mMessage,page);
		}
		mMessage.clear();
		m_iAcceptMsg=0;
		m_iContinueMsg=0;
		m_iDoneMsg=0;
	}
	void PlayNpc::Buy(DomainPlay* src,int page,int pos,int good_id,int type_id,int number)
	{
		if( ! CheckCanTalk(src) ) return ;
		int flags = 0;
		ShopGoodMap::iterator p = mShopGoods.find(page*100+pos);
		if( p != mShopGoods.end() )
		{
			if( page == p->second.page && pos == p->second.pos && good_id == p->second.good_id && type_id == p->second.type_id )
			{
				if( p->second.price_type == 0 )
				{
					if( src->RemGameMoney(p->second.price*number,111) )
					{
						if( mGSL->GetShopNpc()->ID() == ID() )mGSL->GetScriptEngine()->PlayerCongzi(src,p->second.type_id,p->second.price_type,p->second.price,"player.onVcoinShopBuy");
						ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
						if( id )
						{
							if( mGSL->mNpcBuyGameMoneyBind )
							{
								flags |= ITEM_FLAG_BIND;
							}
							else if( mGSL->mNpcBuyGameMoneyUseBind )
							{
								flags |= ITEM_FLAG_USE_BIND;
							}
							SubItem ii;
							ii.BuildItemInstance(id,flags,false,mGSL->mItemAddType,p->second.number);
							src->AddItem(&ii,number,101,11,1);
						}
					}
					else
					{
						src->AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BUY_ITEM_NOT_ENOPHY_GAMEMONEY);
					}
				}
				if( p->second.price_type == 1 )
				{
					if( src->Session()->AuthType() == 100 )
					{
						src->AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BUY_ITEM_NOT_YOUKE);
						return;
					}
					if( src->RemVcoin(p->second.price*number,111) )
					{
						if( mGSL->GetShopNpc()->ID() == ID() )mGSL->GetScriptEngine()->PlayerCongzi(src,p->second.type_id,p->second.price_type,p->second.price,"player.onVcoinShopBuy");
						ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
						if( id )
						{
							if( mGSL->mNpcBuyVcoinBind )
							{
								flags |= ITEM_FLAG_BIND;
							}
							else if( mGSL->mNpcBuyVcoinUseBind )
							{
								flags |= ITEM_FLAG_USE_BIND;
							}
							SubItem ii;
							ii.BuildItemInstance(id,flags,false,mGSL->mItemAddType,p->second.number);
							src->AddItem(&ii,number,101,11,1);
						}
					}
					else
					{
						src->AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BUY_ITEM_NOT_ENOPHY_VCOIN);
					}
				}
				if( p->second.price_type == 2 )
				{
					if( src->RemVcoinBind(p->second.price*number,111) )
					{
						if( mGSL->GetShopNpc()->ID() == ID() )mGSL->GetScriptEngine()->PlayerCongzi(src,p->second.type_id,p->second.price_type,p->second.price,"player.onVcoinShopBuy");
						ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
						if( id )
						{
							SubItem ii;
							ii.BuildItemInstance(id,flags|ITEM_FLAG_BIND,false,mGSL->mItemAddType,p->second.number);
							src->AddItem(&ii,number,101,11,1);
						}
					}
					else
					{
						src->AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BUY_ITEM_NOT_ENOPHY_VCOINBIND);
					}
				}
				if( p->second.price_type == 3 )
				{
					if( src->RemGameMoneyBind(p->second.price*number,111) )
					{
						if( mGSL->GetShopNpc()->ID() == ID() )mGSL->GetScriptEngine()->PlayerCongzi(src,p->second.type_id,p->second.price_type,p->second.price,"player.onVcoinShopBuy");
						ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
						if( id )
						{
							SubItem ii;
							ii.BuildItemInstance(id,flags|ITEM_FLAG_BIND,false,mGSL->mItemAddType,p->second.number);
							src->AddItem(&ii,number,101,11,1);
						}
					}
					else
					{
						src->AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BUY_ITEM_NOT_ENOPHY_GAMEMONEYBIND);
					}
				}
				if( p->second.price_type == 4 )
				{
					if( src->UseGameMoney(p->second.price*number,111) )
					{
						if( mGSL->GetShopNpc()->ID() == ID() )mGSL->GetScriptEngine()->PlayerCongzi(src,p->second.type_id,p->second.price_type,p->second.price,"player.onVcoinShopBuy");
						ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(type_id);
						if( id )
						{
							SubItem ii;
							ii.BuildItemInstance(id,flags|ITEM_FLAG_BIND,false,mGSL->mItemAddType,p->second.number);
							src->AddItem(&ii,number,101,11,1);
						}
					}
					else
					{
						src->AlertMessage(1,0,mGSL->mStringConsts.NOTIFY_BUY_ITEM_NOT_ENOPHY_GAMEMONEY);
					}
				}
			}
		}
	}
	void PlayNpc::Sell(DomainPlay* src,SubItem* it,int number)
	{
		if( ! CheckCanTalk(src) ) return;
		if( ! mSell ) return;
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(it->mTypeID);
		if( id )
		{
			int gm = id->mPrice*number/3;
			if( ItemDef::IsEquipment(it->mSubType) )
			{
				gm = int(id->mPrice/3.0 * it->mDuration / it->mDuraMax);
			}
			if( gm <= 0 ) gm = 1;
			int flags = it->mItemFlags;
			if( 0 == src->RemItemPosition(it->mTypeID,it->mPosition,number,109) )
			{
				/*if( flags & ITEM_FLAG_BIND )
				{
					src->AddGameMoneyBind(gm,112);
				}
				else
				{*/
					src->AddGameMoney(gm,112);	
				//}
			}
		}
	}
	int PlayNpc::Repair(DomainPlay* src,SubItem* item)
	{
		if( ! CheckCanTalk(src) ) return 4;
		if( ! mRepair ) return 5;
		ItemDef* id = mGSL->GetItemDefineManager()->GetItemDefine(item->mTypeID);
		if( id )
		{
			int rp = item->RepairPrice(id);
			if( rp > 0 )
			{
				if( src->RemGameMoney( rp,113 ) )
				{
					item->mDuration = item->mDuraMax;
					return 0;
				}
				else
				{
					return 3;
				}
			}
			else
			{
				return 2;
			}
		}
		return 1;
	}
	void PlayNpc::LoadScript(ScriptLua* script,const char* package)
	{
		mScript = script;
		strncpy_s(mScriptPackage,255,package,255);
		mScriptPackage[255]=0;
		_snprintf_s(m_ScriptonRefreshShowFlags,255,255,"%s.onRefreshShowFlags",mScriptPackage);
		m_ScriptonRefreshShowFlags[255]=0;
		_snprintf_s(m_ScriptonShop,255,255,"%s.onShop",mScriptPackage);
		m_ScriptonShop[255]=0;
	}
	bool PlayNpc::ReloadShopGood()
	{
		ShopGoodMap l = mShopGoods;
		if( ! LoadShopGood(mGSL) )
		{
			mShopGoods = l;
			return false;
		}
		return true;
	}
	bool PlayNpc::LoadShopGood(GameServerLib* lib)
	{
		mShopGoods.clear();

		if(mShopFile.empty())
		{
			return false;
		}

		char temp[256];

		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile(mShopFile.c_str(),size);
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
					int pos;
					char nameorid[256];
					int good_id=0;
					int page=0;
					int num=0;
					int price=0;
					int pricetype=0;
					int oldprice=0;
					int good_tag = 0;
					int sscanfret = sscanf_s(line,"%d,%d,%[^,],%d,%d,%d,%d,%d",&page,&pos,nameorid,_countof(nameorid),&num,&price,&pricetype,&oldprice,&good_tag);
					if( 3 == sscanfret || sscanfret == 7 || sscanfret == 6 || sscanfret == 8 )
					{
						if( sscanfret == 3 )
						{
							num = 1;
							price = 0;
							pricetype = 0;
							oldprice = 0;
						}
						if( sscanfret == 6 )
						{
							oldprice = price;
						}
						if( nameorid[0] > 0 && isdigit(nameorid[0]) )
						{
							good_id=atoi(nameorid);
							ItemDef* id = lib->GetItemDefineManager()->GetItemDefine(good_id);
							if( id )
							{
								num = 1;
								if( sscanfret == 3 )
								{
									price = id->mPrice;
									oldprice = id->mPrice;
									pricetype = 0;
								}
								if( price == 0 )
								{
									price = id->mPrice;
									oldprice = id->mPrice;
								}
							}
							else
							{
								good_id=0;
							}
						}
						else
						{
							UtilString::act_2_utf8(nameorid,strlen(nameorid),temp,256);
							strncpy_s(nameorid,255,temp,255);
							nameorid[255]=0;
							ItemDef* id = lib->GetItemDefineManager()->GetItemDefine(nameorid);
							if( id )
							{
								num = 1;
								if( sscanfret == 3 )
								{
									price = id->mPrice;
									oldprice = id->mPrice;
									pricetype = 0;
								}
								if( price == 0 )
								{
									price = id->mPrice;
									oldprice = id->mPrice;
								}
								good_id = id->mTypeID;
							}
						}
						if( good_id > 0 && num > 0 && (pricetype==0||pricetype==1||pricetype==2||pricetype==3||pricetype==4) && price >= 0 && oldprice >=0 && oldprice>=price )
						{
							ShopGoodClass sg;
							sg.pos = pos;
							sg.page = page;
							sg.good_id = good_id;
							sg.type_id = good_id;
							sg.price_type = pricetype;
							sg.price = price;
							sg.oldprice = oldprice;
							sg.number = 1;//num;
							sg.good_tag = good_tag;
							mShopGoods[page*100+pos] = sg;
						}
					}
				}
			}
			//fclose(file);
		}
		if( mShopGoods.empty() )
		{
			return false;
		}
		return true;
	}
	void PlayNpc::ParseOptions(std::string options)
	{

	}
	void PlayNpc::SetOption(const char* option)
	{
		char word[256];
		strncpy_s(word,_countof(word),option,255);
		word[255]=0;
		_strlwr_s(word,_countof(word));
		size_t len = strlen(word);
		char shrname[256];
		memset(shrname,0,256);
		if( 0 == strcmp(word,"shownpc") )
		{
			mShowNpcFlag=1;
		}
		if( 1 == sscanf_s(word,"shortname(%[^)])",shrname,_countof(shrname)) )
		{
			mShortName = shrname;
		}
		if ( 0 == strcmp(word,"changename"))
		{
			mChangeName = 1;
		}
	}
}