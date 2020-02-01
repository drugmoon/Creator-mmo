#include "StdAfx.h"
#include "ManagerItemDef.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
namespace OGSLib
{
	ItemDef::ItemDef()
	{
		mTypeID=0;
		mIconID=0;
		mPrice=0;
		mWeight=0;
		mDurationMax=0;
		mNeedType=0;
		mNeedParam=0;
		mNeedReinLv=0;
		mLastTime=0;
		mColor=0;

		mResMale=0;
		mResFemale=0;
		mAC=0;
		mACMax=0;
		mMAC=0;
		mMACMax=0;
		mDC=0;
		mDCMax=0;
		mMC=0;
		mMCMax=0;
		mSC=0;
		mSCMax=0;
		mLuck=0;
		mCurse=0;
		mAccuracy=0;
		mDodge=0;
		mAntiMagic=0;
		mAntiPoison=0;
		mHpRecover=0;
		mMpRecover=0;
		mPoisonRecover=0;
		mBaoji = 0;

		mRandRange=0;
		mRandAC=0;
		mRandMAC=0;
		mRandDC=0;
		mRandMC=0;
		mRandSC=0;
		mAddBaseAC=0;
		mAddBaseMAC=0;
		mAddBaseDC=0;
		mAddBaseMC=0;
		mAddBaseSC=0;

		mProtect = 0;
		mNotips = 1;

		mMabi_prob=0;
		mMabi_dura=0;
		mDixiao_pres=0;
		mFuyuan_cd=0;
		mFuyuan_pres=0;
		mMax_hp=0;
		mMax_mp=0;
		mMax_hp_pres=0;
		mMax_mp_pres=0;
		mNeedZsLevel=0;
		mEquipLevel=0;
		mJob=0;
		mGender=0;
		mEquipGroup=0;
		mEquipComp=0;
		mEquipType=0;
		mXishou_prob=0;
		mXishou_pres=0;
		mFantan_prob=0;
		mFantan_pres=0;
		mBaoji_prob=0;
		mBaoji_pres=0;
		mXixue_prob=0;
		mXixue_pres=0;

		SubType=0;
		HPChange=0;
		MPChange=0;
		ZipType=0;
		ZipNumber=0;
		Script[0]=0;
		mFuncId = 0;
		//mFuncParams = "";
	}
	bool ItemDef::IsDissipative(int sub_type)
	{
		return sub_type != ITEM_TYPE_EQUIP;
	}
	bool ItemDef::IsEquipment(int sub_type)
	{
		return sub_type == ITEM_TYPE_EQUIP;
	}
	bool ItemDef::IsEatable(int sub_type)
	{
		// 需求未确认,先占坑
		/*if (sub_type == ITEM_TYPE_MATERIAL)
		{*/
			return true;
		//}
	}
	bool ItemDef::IsMaterial(int sub_type)
	{
		return sub_type == ITEM_TYPE_MATERIAL;
	}

	bool ItemDef::IsSkillItem(int sub_type)
	{
		return sub_type == ITEM_TYPE_SKILLITEM;
	}
	bool ItemDef::IsEquipWarrior(int type_id)
	{
		if( type_id > ITEM_EQUIP_WARRIOR_BEGIN && type_id < ITEM_EQUIP_WARRIOR_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsEquipWizard(int type_id)
	{
		if( type_id > ITEM_EQUIP_WIZARD_BEGIN && type_id < ITEM_EQUIP_WIZARD_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsEquipTaoist(int type_id)
	{
		if( type_id > ITEM_EQUIP_TAOIST_BEGIN && type_id < ITEM_EQUIP_TAOIST_END )
		{
			return true;
		}
		return false;
	}


	bool ItemDef::IsMoxueshi(int type_id)
	{
		if( type_id > ITEM_MOXUESHI_BEGIN && type_id < ITEM_MOXUESHI_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsHunqi(int type_id)
	{
		if( type_id > ITEM_HUNQI_BEGIN && type_id < ITEM_HUNQI_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsLinqi(int type_id)
	{
		if( type_id > ITEM_LINQI_BEGIN && type_id < ITEM_LINQI_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsWeapon(int equip_type)
	{
		return equip_type == EQUIP_TYPE_WEAPON;
	}
	bool ItemDef::IsCloth(int equip_type)
	{
		return equip_type == EQUIP_TYPE_CLOTH;
	}

	bool ItemDef::IsGlove(int equip_type)
	{
		return equip_type == EQUIP_TYPE_GLOVE;
	}

	bool ItemDef::IsRing(int equip_type)
	{
		return equip_type == EQUIP_TYPE_RING;
	}

	bool ItemDef::IsLoadEquipment(int type_id)
	{
		if( type_id > ITEM_CLOTH_BEGIN && type_id < ITEM_EQUIP_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsFashion(int type_id)
	{
		if( type_id > ITEM_FASHION_WEAPON_BEGIN && type_id < ITEM_FASHION_CLOTH_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsFashionWeapon(int type_id)
	{
		if( type_id > ITEM_FASHION_WEAPON_BEGIN && type_id < ITEM_FASHION_WEAPON_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsFashionCloth(int type_id)
	{
		if( type_id > ITEM_FASHION_CLOTH_BEGIN && type_id < ITEM_FASHION_CLOTH_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsXYFashionWeapon(int type_id)
	{
		if( type_id > ITEM_XYFASHION_WEAPON_BEGIN && type_id < ITEM_XYFASHION_WEAPON_END )
		{
			return true;
		}
		return false;
	}
	bool ItemDef::IsXYFashionCloth(int type_id)
	{
		if( type_id > ITEM_XYFASHION_CLOTH_BEGIN && type_id < ITEM_XYFASHION_CLOTH_END )
		{
			return true;
		}
		return false;
	}
	//*****************************宝石相关*****************************
	bool ItemDef::IsGem(int sub_type)
	{
		return sub_type == ITEM_TYPE_GEM;
	}
	
	bool ItemDef::IsAttackGem(int type_id)
	{
		if( type_id > ITEM_GEM_ATTACK_BEGIN && type_id < ITEM_GEM_ATTACK_END )
		{
			return true;
		}
		return false;
	}

	bool ItemDef::IsACGem(int type_id)
	{
		if( type_id > ITEM_GEM_AC_BEGIN && type_id < ITEM_GEM_AC_END )
		{
			return true;
		}
		return false;
	}

	bool ItemDef::IsMACGem(int type_id)
	{
		if( type_id > ITEM_GEM_MAC_BEGIN && type_id < ITEM_GEM_MAC_END )
		{
			return true;
		}
		return false;
	}

	bool ItemDef::IsHpGem(int type_id)
	{
		if( type_id > ITEM_GEM_HP_BEGIN && type_id < ITEM_GEM_HP_END )
		{
			return true;
		}
		return false;
	}

	bool ItemDef::IsMpGem(int type_id)
	{
		if( type_id > ITEM_GEM_MP_BEGIN && type_id < ITEM_GEM_MP_END )
		{
			return true;
		}
		return false;
	}

	bool ItemDef::IsSpecialGem(int type_id)
	{
		if( type_id > ITEM_GEM_SPECIAL_BEGIN && type_id < ITEM_GEM_SPECIAL_END )
		{
			return true;
		}
		return false;
	}

	int ItemDef::getGemOffSet(int type_id)
	{
		if (IsAttackGem(type_id))
		{
			return GEM_ATTACK_OFFSET_POSITION;
		}
		if (IsACGem(type_id))
		{
			return GEM_AC_OFFSET_POSITION;
		}
		if (IsMACGem(type_id))
		{
			return GEM_MAC_OFFSET_POSITION;
		}
		if (IsHpGem(type_id))
		{
			return GEM_HP_OFFSET_POSITION;
		}
		if (IsMpGem(type_id))
		{
			return GEM_MP_OFFSET_POSITION;
		}
		if (IsSpecialGem(type_id))
		{
			return GEM_SPECIAL_OFFSET_POSITION;
		}
		return 0;
	}

	int ItemDef::getGemEquipPosition (int equip_pos, int gem_id)
	{
		int offSet = ItemDef::getGemOffSet(gem_id);
		if (offSet)
		{
			return equip_pos - offSet;
		}
		return 0;
	}

	bool ManagerItemDef::ReloadItemDefine(GameServerLib* lib)
	{
		ItemProgramMap j = mItemProgramMap;
		ItemDefineMap i = mItemDefineMap;
		if( ! LoadItemDefine(lib) )
		{
			mItemProgramMap = j;
			mItemDefineMap = i;
			return false;
		}
		ReloadPlusDefine(lib);
		ReloadItemAttr(lib);
		return true;
	}
	bool ManagerItemDef::ReloadPlusDefine(GameServerLib* lib)
	{
		ItemPlusMap i = mItemPlusMap;
		if( ! LoadItemPlusDefine2(lib) )
		{
			mItemPlusMap = i;
			return false;
		}
		LoadItemPlusTxt(lib);
		return true;
	}
	bool ManagerItemDef::ReloadItemAttr(GameServerLib* lib)
	{
		if( LoadItemPlusAttr(lib) )
		{
			return true;
		}
		return false;
	}
	void ManagerItemDef::AddItemTriger(int type_id)
	{
		mItemTriger[type_id] = type_id;
	}
	int  ManagerItemDef::GetItemTriger(int type_id)
	{
		ItemTrigerMap::iterator pos = mItemTriger.find(type_id);
		if( pos != mItemTriger.end() ){return pos->second;}
		return 0;
	}
	void ManagerItemDef::AddItemAlert(int type_id)
	{
		mItemAlert[type_id] = type_id;
	}
	int  ManagerItemDef::GetItemAlert(int type_id)
	{
		ItemDropAlertMap::iterator pos = mItemAlert.find(type_id);
		if( pos != mItemAlert.end() ){return pos->second;}
		return 0;
	}
	bool ManagerItemDef::LoadSpeedDefine(GameServerLib* lib)
	{
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("itemmount.txt",size);
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
					int itemid;
					float itemspeed;
					int mountdelay;
					int ret = sscanf_s(line,"%d%f%d",&itemid,&itemspeed,&mountdelay);
					if( 3 == ret )
					{
						mItemSpeedMap[itemid] = itemspeed;
						mItemMountDelayMap[itemid] = mountdelay;
					}
				}
			}
		}
		return true;
	}
	bool ManagerItemDef::LoadStackDefine(GameServerLib* lib)
	{
		mItemStackMap.clear();
		int itemid;
		char name[256];
		int stackmax=0;
		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("itemStack.csv",size);
		if( data && size > 0 )
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					int ret = 0;
					if( version == 100 )
					{
						ret = sscanf_s(line,"%d,%[^,],%d",&itemid,name,_countof(name),&stackmax);
						if( ret == 3)
						{
							mItemStackMap[itemid] = stackmax;
						}
					}
					
				}
			}
		}
		return true;
	}
	bool ManagerItemDef::LoadTradeLimit(GameServerLib* lib)
	{
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("tradelimit.txt",size);
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
					int itemid;
					int limitlv;
					int ret = sscanf_s(line,"%d%d",&itemid,&limitlv);
					if( 2 == ret )
					{
						mTradeLimitMap[itemid] = limitlv;
					}
				}
			}
		}
		return true;
	}
	bool ManagerItemDef::LoadItemPlusAttr(GameServerLib* lib)
	{
		char name[256];
		int type;
		int xishou_prob=0;
		int xishou_pres=0;
		int fantan_prob=0;
		int fantan_pres=0;
		int baoji_prob=0;
		int baoji_pres=0;
		int xixue_prob=0;
		int xixue_pres=0;
		int mabi_prob=0;
		int mabi_dura=0;
		int dixiao_pres=0;
		int fuyuan_cd=0;
		int fuyuan_pres=0;
		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("itemattr.csv",size);
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
						line[5] = 0;
						if( 0 == strcmp(line,"#v101") )
						{
							version = 101;
						}
						continue;
					}
					int ret = 0;
					int parse_ok = 0;
					if( version == 100 )
					{
						ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^,]",&type,&xishou_prob,&xishou_pres,&fantan_prob,&fantan_pres,&baoji_prob,&baoji_pres,&xixue_prob,&xixue_pres,&mabi_prob,&mabi_dura,&dixiao_pres,&fuyuan_cd,&fuyuan_pres,name,_countof(name));
						if( ret == 14 || ret == 15)
						{
							parse_ok=1;
						}
					}
					if( parse_ok )
					{
						ItemDef* id = GetItemDefine(type);
						if( id )
						{
							id->mXishou_prob = xishou_prob;
							id->mXishou_pres = xishou_pres;
							id->mFantan_prob = fantan_prob;
							id->mFantan_pres = fantan_pres;
							id->mBaoji_prob = baoji_prob;
							id->mBaoji_pres = baoji_pres;
							id->mXixue_prob = xixue_prob;
							id->mXixue_pres = xixue_pres;
							id->mMabi_prob=mabi_prob;
							id->mMabi_dura=mabi_dura;
							id->mDixiao_pres=dixiao_pres;
							id->mFuyuan_cd=fuyuan_cd;
							id->mFuyuan_pres=fuyuan_pres;
						}
					}
				}
			}
		}
		return true;
	}
	bool ManagerItemDef::LoadItemPlusDefine(GameServerLib* lib)
	{
		int itemplusid;
		int itemtypeid;
		int need_type;
		int need_param1;
		int need_param2;
		int need_param3;
		int need_param4;
		int need_param5;
		int ac;
		int ac_max;
		int mac;
		int mac_max;
		int dc;
		int dc_max;
		int mc;
		int mc_max;
		int sc;
		int sc_max;
		int luck;
		int curse;
		int accuracy;
		int dodge;
		int antimagic;
		int antiposion;
		int hprecover;
		int mprecover;
		int posionrecover;
		int param100;
		int param101;
		int param102;
		int param103;
		int max_hp;
		int max_mp;
		int max_hp_pres;
		int max_mp_pres;
		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("itemplusdef.csv",size);
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
						line[5] = 0;
						if( 0 == strcmp(line,"#v101") )
						{
							version = 101;
						}
						if( 0 == strcmp(line,"#v102") )
						{
							version = 102;
						}
						continue;
					}
					int ret = 0;
					int parse_ok = 0;
					if( version == 100 )
					{
						ret = sscanf_s(line,"%d,%d\
											,%d,%d,%d,%d,%d,%d\
											,%d,%d,%d,%d\
											,%d,%d,%d,%d,%d,%d\
											,%d,%d,%d,%d,%d,%d\
											,%d,%d,%d\
											,%d,%d,%d,%d\
											,%d,%d,%d,%d",
											&itemplusid,&itemtypeid,
											&need_type,&need_param1,&need_param2,&need_param3,&need_param4,&need_param5,
											&ac,&ac_max,&mac,&mac_max,
											&dc,&dc_max,&mc,&mc_max,&sc,&sc_max,
											&luck,&curse,&accuracy,&dodge,&antimagic,&antiposion,
											&hprecover,&mprecover,&posionrecover,
											&param100,&param101,&param102,&param103,
											&max_hp,&max_mp,&max_hp_pres,&max_mp_pres);
						if( ret == 35 )
						{
							parse_ok=1;
						}
					}
					if( parse_ok )
					{
						ItemDef* id = GetItemDefine(itemtypeid);
						if( id )
						{
							ItemPlusDef ipd;
							ipd.mItemPlusID = itemplusid;
							ipd.mTypeID = itemtypeid;
							ipd.mNeedType = need_type;
							ipd.mNeedParam1 = need_param1;
							ipd.mNeedParam2 = need_param2;
							ipd.mNeedParam3 = need_param3;
							ipd.mNeedParam4 = need_param4;
							ipd.mNeedParam5 = need_param5;
							ipd.mAC = ac;
							ipd.mACMax = ac_max;
							ipd.mMAC = mac;
							ipd.mMACMax = mac_max;
							ipd.mDC = dc;
							ipd.mDCMax = dc_max;
							ipd.mMC = mc;
							ipd.mMCMax = mc_max;
							ipd.mSC = sc;
							ipd.mSCMax = sc_max;
							ipd.mLuck = luck;
							ipd.mCurse = curse;
							ipd.mAccuracy = accuracy;
							ipd.mDodge = dodge;
							ipd.mAntiMagic = antimagic;
							ipd.mAntiPoison = antiposion;
							ipd.mHpRecover = hprecover;
							ipd.mMpRecover = mprecover;
							ipd.mPoisonRecover = posionrecover;
							ipd.mFuyuan_cd = 0;
							ipd.mFuyuan_pres = 0;
							ipd.mMabi_dura = 0;
							ipd.mMabi_prob = 0;
							ipd.mDixiao_pres = 0;
							ipd.mHalfBaoji = param100;
							ipd.mMax_hp = max_hp;
							ipd.mMax_mp = max_mp;
							ipd.mMax_hp_pres = max_hp_pres;
							ipd.mMax_mp_pres = max_mp_pres;
							id->mPlus.push_back(ipd);
						}
					}
				}
			}
		}
		return true;
	}
	bool ManagerItemDef::LoadItemPlusDefine2(GameServerLib* lib)
	{
		mItemPlusMap.clear();
		int itemplusid;
		int need_type;
		int need_param1;
		int need_param2;
		int need_param3;
		int need_param4;
		int need_param5;
		int ac;
		int ac_max;
		int mac;
		int mac_max;
		int dc;
		int dc_max;
		int mc;
		int mc_max;
		int sc;
		int sc_max;
		int luck;
		int curse;
		int accuracy;
		int dodge;
		int antimagic;
		int antiposion;
		int hprecover;
		int mprecover;
		int posionrecover;
		int param100;
		int param101;
		int param102;
		int param103;
		int max_hp;
		int max_mp;
		int max_hp_pres;
		int max_mp_pres;
		int version = 100;
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("itemplusdef2.csv",size);
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
						line[5] = 0;
						if( 0 == strcmp(line,"#v101") )
						{
							version = 101;
						}
						if( 0 == strcmp(line,"#v102") )
						{
							version = 102;
						}
						continue;
					}
					int ret = 0;
					int parse_ok = 0;
					int mathb = 100;
					if( version == 100 )
					{
						ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d\
											,%d,%d,%d,%d\
											,%d,%d,%d,%d,%d,%d\
											,%d,%d,%d,%d,%d,%d\
											,%d,%d,%d\
											,%d,%d,%d,%d\
											,%d,%d,%d,%d",
											&itemplusid,&need_type,&need_param1,&need_param2,&need_param3,&need_param4,&need_param5,
											&ac,&ac_max,&mac,&mac_max,
											&dc,&dc_max,&mc,&mc_max,&sc,&sc_max,
											&luck,&curse,&accuracy,&dodge,&antimagic,&antiposion,
											&hprecover,&mprecover,&posionrecover,
											&param100,&param101,&param102,&param103,
											&max_hp,&max_mp,&max_hp_pres,&max_mp_pres);
						if( ret == 34 )
						{
							parse_ok=1;
						}
					}
					if( parse_ok )
					{
						if( itemplusid > 0 )
						{
							ItemPlusDef ipd;
							ipd.mItemPlusID = itemplusid;
							ipd.mNeedType = need_type;
							ipd.mNeedParam1 = need_param1;
							ipd.mNeedParam2 = need_param2;
							ipd.mNeedParam3 = need_param3;
							ipd.mNeedParam4 = need_param4;
							ipd.mNeedParam5 = need_param5;
							ipd.mAC = ac;
							ipd.mACMax = ac_max;
							ipd.mMAC = mac;
							ipd.mMACMax = mac_max;
							ipd.mDC = dc;
							ipd.mDCMax = dc_max;
							ipd.mMC = mc;
							ipd.mMCMax = mc_max;
							ipd.mSC = sc;
							ipd.mSCMax = sc_max;
							ipd.mLuck = luck;
							ipd.mCurse = curse;
							ipd.mAccuracy = accuracy;
							ipd.mDodge = dodge;
							ipd.mAntiMagic = antimagic;
							ipd.mAntiPoison = antiposion;
							ipd.mHpRecover = hprecover;
							ipd.mMpRecover = mprecover;
							ipd.mPoisonRecover = posionrecover;
							ipd.mFuyuan_cd = 0;
							ipd.mFuyuan_pres = 0;
							ipd.mMabi_dura = 0;
							ipd.mMabi_prob = 0;
							ipd.mDixiao_pres = 0;
							ipd.mHalfBaoji = param100;
							ipd.mMax_hp = max_hp;
							ipd.mMax_mp = max_mp;
							ipd.mMax_hp_pres = max_hp_pres;
							ipd.mMax_mp_pres = max_mp_pres;
							
                            int tempid = itemplusid / mathb;
                            mItemPlusMap[tempid].push_back(ipd);
						}
					}
				}
			}
		}
		return true;
	}
	bool ManagerItemDef::LoadItemPlusTxt(GameServerLib* lib)
	{
		size_t size= 0;
		const char* data = lib->GetListener()->LoadFile("itemplus.txt",size);
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
					int itemid;
					int plusid = 0;
					int zlid = 0;
					int ret = sscanf_s(line,"%d%d%d",&itemid,&plusid,&zlid);
					if( 3 == ret || 2 == ret)
					{
						ItemDef* id = GetItemDefine(itemid);
						if( id )
                        {
                            {
                                auto pos = mItemPlusMap.find(plusid);
                                if (pos != mItemPlusMap.end()) {
                                    id->mPlus = pos->second;
                                    for (auto it = id->mPlus.begin(); it != id->mPlus.end(); ++it) {
                                        it->mTypeID = id->mTypeID;
                                    }
                                }
                            }

                            {
                                auto pos = mItemPlusMap.find(zlid);
                                if (pos != mItemPlusMap.end()) {
                                    id->mPlusZl = pos->second;
                                    for (auto it = id->mPlusZl.begin(); it != id->mPlusZl.end(); ++it) {
                                        it->mTypeID = id->mTypeID;
                                    }
                                }
                            }
						}
					}
				}
			}
		}
		return true;
	}
	bool ManagerItemDef::LoadItemDefine(GameServerLib* lib)
	{
		//for(int i = 0;i < ITEM_STEEL_MAX;i++)
		//{
		//	mSteelArray[i] = 0;
		//}
		mItemProgramMap.clear();
		mItemDefineMap.clear();
		//mOfflineExpPrice1=0;
		//mOfflineExpPrice2=0;
		//mOfflineExpPrice4=0;
		mRelivePrice = 0;
		mAddDepotPrice = 0;
		int type_id;
		int sub_type;
		int res_male;
		int res_female;
		int icon_id;
		//int old_id;//no use
		char script[64];
		char name[256];
		//int stdmod;//no use 
		int color;
		int weight;
		int last_time=0;
		//int anicount;//no use
		//int source;//no use
		//int reserved;//no use
		//int looks;//no use
		int duramax=1;
		int ac;
		int ac2;
		int mac;
		int mac2;
		int dc;
		int dc2;
		int mc;
		int mc2;
		int sc;
		int sc2;
		int luck;
		int curse;
		int accuracy;
		int dodge;
		int anti_magic;
		int anti_poison;
		int hp_recover = 0; // 改增加hp上限
		int mp_recover = 0; // 改增加mp上限
		int poison_recover = 0; //改神圣伤害
		int sacred_damage = 0; // 改神圣伤害
		int need;
		int needlevel;
		int price;
		//int stock;//no use
		int rand_range=0;
		int rand_ac=0;
		int rand_mac=0;
		int rand_dc=0;
		int rand_mc=0;
		int rand_sc=0;
		int add_base_ac=0;
		int add_base_mac=0;
		int add_base_dc=0;
		int add_base_mc=0;
		int add_base_sc=0;
		//int param100;
		//int param101;
		//int param102;
		//int param103;
		int max_hp=0;
		int max_mp=0;
		int max_hp_pres=0;
		int max_mp_pres=0;
		int job;
		int gender;
		char desp[1024];
		char source[1024];
		int protect=0;
		int notips=0;
		int onsale_ok=1;
		//int baoji=0;
		int baoji_prob=0;
		int baoji_pres=0;
		int drop_luck=0;
		int equip_type=0;
		int zsneed_level=0;
		int equip_level=0;
		int equip_group=0;
		int equip_comp=0;
		int equip_contribute=0;
		int destory_show=0;
		int add_power=0;
		int item_bg=0;
		int recycle_money=0;
		int recycle_exp=0;
		int recycle_xuefu=0;
		int can_destroy = 0;
		int	can_depot = 0;
		int can_push = 0;
		int can_use = 0;
		int bag_show = 0;
		int times_limit = 0;
		int func_id = 0;
		char func_params[64];

		char temp[2024];

		{
			ItemDef id;
			id.mTypeID = ITEM_GOLD_ID;
			id.mName = "金币";
			UtilString::act_2_utf8(id.mName.c_str(),id.mName.length(),temp,256);
			id.mName = temp;
			id.mIconID = ITEM_GOLD_ID;
			mItemDefineMap[id.mTypeID] = id;
		}
		{
			ItemDef id;
			id.mTypeID = ITEM_GOLD_BIND_ID;
			id.mName = "银币";
			UtilString::act_2_utf8(id.mName.c_str(),id.mName.length(),temp,256);
			id.mName = temp;
			id.mIconID = ITEM_GOLD_BIND_ID;
			mItemDefineMap[id.mTypeID] = id;
		}
		{
			ItemDef id;
			id.mTypeID = ITEM_VCOIN_ID;
			id.mName = "hahale";
			UtilString::act_2_utf8(id.mName.c_str(),id.mName.length(),temp,256);
			id.mName = temp;
			id.mIconID = ITEM_VCOIN_ID;
			mItemDefineMap[id.mTypeID] = id;
		}
		{
			ItemDef id;
			id.mTypeID = ITEM_VCOIN_BIND_ID;
			id.mName = "绑定元宝";
			UtilString::act_2_utf8(id.mName.c_str(),id.mName.length(),temp,256);
			id.mName = temp;
			id.mIconID = ITEM_VCOIN_BIND_ID;
			mItemDefineMap[id.mTypeID] = id;
		}
		{
			ItemDef id;
			id.mTypeID = ITEM_EXP_ID;
			id.mName = "经验";
			UtilString::act_2_utf8(id.mName.c_str(),id.mName.length(),temp,256);
			id.mName = temp;
			id.mIconID = ITEM_EXP_ID;
			mItemDefineMap[id.mTypeID] = id;
		}
		{
			ItemDef id;
			id.mTypeID = ITEM_RELIVE_COIN_ID;
			id.mName = "原地复活";
			UtilString::act_2_utf8(id.mName.c_str(),id.mName.length(),temp,256);
			id.mName = temp;
			id.mIconID = ITEM_RELIVE_COIN_ID;
			id.mDesp = "原地复活";
			UtilString::act_2_utf8(id.mDesp.c_str(),id.mDesp.length(),temp,256);
			id.mDesp = temp;
			id.mPrice = 200;
			id.mWeight = 1;
			mItemDefineMap[id.mTypeID] = id;
		}
		{
			ItemDef id;
			id.mTypeID = ITEM_ADD_SLOT;
			id.mName = "开启物品栅格";
			UtilString::act_2_utf8(id.mName.c_str(),id.mName.length(),temp,256);
			id.mName = temp;
			id.mIconID = ITEM_ADD_SLOT;
			id.mDesp = "开启物品栅格";
			UtilString::act_2_utf8(id.mDesp.c_str(),id.mDesp.length(),temp,256);
			id.mDesp = temp;
			id.mPrice = 100;
			id.mWeight = 1;
			mItemDefineMap[id.mTypeID] = id;
		}

		int version = 0;
		size_t size= 0;

		const char* data = lib->GetListener()->LoadFile("itemprogram.csv",size);
		if (data && size > 0)
		{
			LEUD::ToolsMemFile f(data,size);
			char line[1204];
			while( f.read_line(line,1024) )
			{
				if( line[0]!=0 )
				{
					if( line[0] == '#' || line[0] == ';' )
					{
						line[5] = 0;
						if( 0 == strcmp(line,"#v101") )
						{
							version = 101;
						}
						if(0==strcmp(line,"#v110"))//增加转生和装备等级
						{
							version=110;
						}
						continue;
					}
					if( version == 0 )
					{
						printf("ERROR: your itemprogram is NO VERSION !!!!!!!");
						return false;
					}
					int ret=0;
					int parse_ok = 0;
					if( version == 110 )
					{
						ret = sscanf_s(line,"%d,%d,%d,%[^,]",
											&type_id,&res_male,&res_female,script,_countof(script));
						if(ret==4)
						{
							parse_ok = 1;
						}
					}
					if( parse_ok > 0 )
					{
						ItemProgram itp;
						itp.mTypeID = type_id;
						itp.mResMale = res_male;
						itp.mResFemale = res_female;

						memset(itp.Script,0,64);
						if( strcmp(script,"null") != 0 )
						{
							strncpy_s(itp.Script,_countof(itp.Script),script,63);
							itp.Script[63]=0;
						}
						mItemProgramMap[type_id] = itp;
						//mItemProgramMap
					}
				}
			}
		}

		data = lib->GetListener()->LoadFile("itemdef.csv",size);
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
						line[5] = 0;
						if( 0 == strcmp(line,"#v101") )
						{
							version = 101;
						}
						if(0==strcmp(line,"#v110"))//增加转生和装备等级
						{
							version=110;
						}
						continue;
					}
					if( version == 0 )
					{
						printf("ERROR: your itemdef is NO VERSION !!!!!!!");
						return false;
					}
					memset(name,0,256);
					memset(desp,0,256);
					memset(source,0,256);
					int ret=0;
					int parse_ok = 0;
					if( version == 110 )
					{
						ret = sscanf_s(line,"%d,%d,%d,%[^,],%x,\
											%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,\
											%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
											%d,%d,%d,%[^,],%[^,],%[^,]",
											&type_id,&sub_type,&icon_id,name,_countof(name),&color,
											&weight,&equip_type,
											&ac,&ac2,&mac,&mac2,&dc,&dc2,&mc,&mc2,&sc,&sc2,
											&luck,&curse,&accuracy,&dodge,&anti_magic,&anti_poison,&max_hp,&max_mp,&max_hp_pres,&max_mp_pres,&sacred_damage,&baoji_prob,&baoji_pres, &drop_luck,
											&need,&needlevel,&price,
											&zsneed_level,&equip_level,&job,&gender,&equip_group,&equip_comp,&equip_contribute,&destory_show,&add_power,&item_bg,&recycle_money,&recycle_exp,&recycle_xuefu,&can_use, &can_destroy,&can_depot,&can_push,
											&bag_show,&times_limit,&func_id,&func_params,_countof(func_params),desp,_countof(desp),source,_countof(source));
						if(ret==57)
						{
							parse_ok = 1;
						}
					}
					if( parse_ok > 0 )
					{
						ItemDef id;
						UtilString::act_2_utf8(name,strlen(name),temp,256);
						strncpy_s(name,_countof(name),temp,255);
						name[255]=0;
						id.mName = name;
						id.mTypeID = type_id;
						id.mIconID = icon_id;
						id.mPrice = price;
						id.mWeight = weight;
						id.mLastTime=last_time*60;
						id.mOnSaleFlag = onsale_ok;
						id.mDurationMax = duramax;
						id.mNeedType = need;
						id.mNeedParam = needlevel;
						if(id.mNeedType == 0 && id.mNeedParam >= 1000)
						{
							id.mNeedReinLv =  id.mNeedParam/1000;
							id.mNeedParam = id.mNeedParam-1000*id.mNeedReinLv;
						}
						id.mColor = color;
						
						id.SubType = sub_type;
						id.HPChange=ac;
						id.MPChange=mac;
						id.ZipType=ac;
						id.ZipNumber=duramax;
						memset(id.Script,0,64);
						if ( mItemProgramMap.find(type_id) != mItemProgramMap.end() )
						{
							strncpy_s(id.Script,_countof(id.Script),mItemProgramMap[type_id].Script,63);
							id.Script[63]=0;
							id.mResMale = mItemProgramMap[type_id].mResMale;
							id.mResFemale = mItemProgramMap[type_id].mResFemale;
						}
						id.mRandRange = rand_range;
						id.mAC = ac;
						id.mACMax = ac2;
						id.mRandAC = rand_ac;
						id.mMAC = mac;
						id.mMACMax = mac2;
						id.mRandMAC = rand_mac;
						id.mDC = dc;
						id.mDCMax = dc2;
						id.mRandDC = rand_dc;
						id.mMC = mc;
						id.mMCMax = mc2;
						id.mRandMC = rand_mc;
						id.mSC = sc;
						id.mSCMax = sc2;
						id.mRandSC = rand_sc;
						id.mAddBaseAC = add_base_ac;
						id.mAddBaseMAC = add_base_mac;
						id.mAddBaseDC = add_base_dc;
						id.mAddBaseMC = add_base_mc;
						id.mAddBaseSC = add_base_sc;
						id.mLuck = luck;
						id.mCurse = curse;
						id.mAccuracy = accuracy;
						id.mDodge = dodge;
						//id.mBaoji = 0;
						id.mBaoji_prob = baoji_prob;
						id.mBaoji_pres = baoji_pres;
						id.mDrop_luck = drop_luck;
						id.mAntiMagic = anti_magic;
						id.mAntiPoison = anti_poison;
						id.mHpRecover = hp_recover;
						id.mMpRecover = mp_recover;
						id.mPoisonRecover = poison_recover;
						//id.mMabi_prob=mabi_prob;
						//id.mMabi_dura=mabi_dura;
						//id.mDixiao_pres=dixiao_pres;
						//id.mFuyuan_cd=fuyuan_cd;
						id.mMax_hp=max_hp;
						id.mMax_mp=max_mp;
						id.mHolyDam = sacred_damage;
						id.mMax_hp_pres=max_hp_pres;
						id.mMax_mp_pres=max_mp_pres;
						if (zsneed_level > 0)
						{
							id.mNeedZsLevel = zsneed_level;
						}
						else
						{
							id.mNeedZsLevel = 0;
						}

						id.mProtect = protect;
						id.mNotips = notips;

						id.mEquipType = equip_type;
						id.mEquipLevel = equip_level;
						if (job > 0)
						{
							job = job + 99;
						}
						id.mJob = job;
						if (gender > 0)
						{
							gender = gender + 199;
						}
						id.mGender = gender;
						id.mEquipGroup = equip_group;
						id.mEquipComp = equip_comp;
						id.mEquipContribute = equip_contribute;
						id.mShowDest = destory_show;
						id.mAddPower = add_power;
						id.mItemBg = item_bg;
						id.mRecycleMoney = recycle_money;
						id.mRecycleExp = recycle_exp;
						id.mRecycleXuefu = recycle_xuefu;
						id.mCanDestroy = can_destroy;
						id.mCanDepot = can_depot;
						id.mCanPush = can_push;
						id.mBagShow = bag_show;
						id.mTimesLimit = times_limit;
						id.mCanUse = can_use;
						id.mFuncId =func_id;
						UtilString::act_2_utf8(func_params,strlen(func_params),temp,64);
						id.mFuncParams = temp;
						if (func_id == 10)
						{
							id.mLastTime = atoi(temp);
						}
						UtilString::act_2_utf8(desp,strlen(desp),temp,2024);
						if( strcmp(temp,"null") != 0 )
						{
							id.mDesp = temp;
						}
						else
						{
							id.mDesp = "";
						}
						
						UtilString::act_2_utf8(source,strlen(source),temp,2024);
						if( strcmp(temp,"null") != 0 )
						{
							id.mSource = temp;
						}
						else
						{
							id.mSource = "";
						}

						if( mItemDefineMap.find(id.mTypeID) != mItemDefineMap.end() )
						{
							switch(id.mTypeID)
							{
							case ITEM_GOLD_ID:
							case ITEM_EXP_ID:
							case ITEM_RELIVE_COIN_ID:
							case ITEM_ADD_SLOT:
							//case ITEM_OFFLINE_EXP_MUL_1:
							//case ITEM_OFFLINE_EXP_MUL_2:
							//case ITEM_OFFLINE_EXP_MUL_4:
							case ITEM_GOLD_BIND_ID:
							case ITEM_VCOIN_ID:
							case ITEM_VCOIN_BIND_ID:
							//case ITEM_CAPACITY_ID:
								break;
							default:
								printf("item id double check failed,id %d\n",id.mTypeID);
								return false;
								break;
							}
						}
						mItemDefineMap[type_id] = id;
					}
				}
			}
		}
		{
			ItemDef* id = GetItemDefine(ITEM_RELIVE_COIN_ID);
			if( id )
			{
				mRelivePrice = id->mPrice;
			}
			id = GetItemDefine(ITEM_ADD_SLOT);
			if( id )
			{
				mAddDepotPrice = id->mPrice;
			}
		}

		if( mItemDefineMap.empty() )
		{
			return false;
		}
		return true;
	}
	void ManagerItemDef::dest()
	{
		mItemDefineMap.clear();
	}
	int ManagerItemDef::CheckItemTradeLimit(int type_id)
	{
		TradeLimitMap::iterator pos = mTradeLimitMap.find(type_id);
		if( pos != mTradeLimitMap.end() )
		{
			return pos->second;
		}
		return 0;
	}
	ItemDef* ManagerItemDef::GetItemDefine(int type_id)
	{
		ItemDefineMap::iterator pos = mItemDefineMap.find(type_id);
		if( pos != this->mItemDefineMap.end() )
		{
			return &pos->second;
		}
		return 0;
	}
	int ManagerItemDef::GetItemStackMax(int type_id)
	{
		ItemStackMap::iterator pos = mItemStackMap.find(type_id);
		if( pos != this->mItemStackMap.end() )
		{
			return pos->second;
		}
		return 1;
	}
	float ManagerItemDef::GetItemSpeed(int type_id)
	{
		ItemSpeedMap::iterator pos = mItemSpeedMap.find(type_id);
		if( pos != this->mItemSpeedMap.end() )
		{
			return pos->second;
		}
		return 1.0f;
	}
	int ManagerItemDef::GetItemMountDelay(int type_id)
	{
		ItemMountDelayMap::iterator pos = mItemMountDelayMap.find(type_id);
		if( pos != this->mItemMountDelayMap.end() )
		{
			return pos->second;
		}
		return 3000;
	}
	ItemDef* ManagerItemDef::GetItemDefine(const char* name)
	{
		for( ItemDefineMap::iterator pos = mItemDefineMap.begin();
			pos != mItemDefineMap.end();
			++ pos )
		{
			if( 0==strcmp(pos->second.mName.c_str(),name) )
			{
				return &pos->second;
			}
		}
		return 0;
	}
	ItemDef* ManagerItemDef::FindItemDefine(int sub_type,int duramax)
	{
		for( ItemDefineMap::iterator pos = mItemDefineMap.begin();
			pos != mItemDefineMap.end();
			++ pos )
		{
			if( ItemDef::IsDissipative(pos->second.SubType) && pos->second.SubType == sub_type && pos->second.mDurationMax == duramax )
			{
				return &pos->second;
			}
		}
		return 0;
	}
}