#include "StdAfx.h"
#include "SubItem.h"
#include "GameServerLib.h"
#include "PlayActiveObject.h"
#include "ManagerItemDef.h"
#include "DomainPlay.h"
namespace OGSLib
{
	int SubItem::sIdGen=100;
	int add_rand(int num,int randrange)
	{
		int ret = 0;
		//if( rand()%10000 < 7000 )
		//{
		//	ret ++;
		//}
		for(int i = 0;i<num-1;i++)
		{
			if( rand() % 10000 < randrange )
			{
				ret ++;
			}
			else
			{
				break;
			}
		}
		return ret;
	}
	void SubItem::ItemAddType0(ItemDef* id,int flags)
	{
		int add_count = 0;
		mItemFlags |= ITEM_FLAG_JIPIN;

		int r = rand() % 9;
		for(int i = 0;i < 9;i ++ )
		{
			if( rand()%(add_count*3+1)!=0 )
			{
				break;
			}
			r++;
			r = r % 9;
			if( r == 0 || r == 5 || r == 7)
			{
				if( mAddAC == 0 )
				{
					mAddAC = add_rand(id->mRandAC,id->mRandRange*15/10);
					add_count += mAddAC;
				}
			}
			if( r == 1 || r == 6 || r == 8)
			{
				if( mAddMAC == 0 )
				{
					mAddMAC = add_rand(id->mRandMAC,id->mRandRange*15/10);
					add_count += mAddMAC;
				}
			}
			if( r == 2 )
			{
				mAddDC = add_rand(id->mRandDC,id->mRandRange);
				add_count += mAddDC;
			}
			if( r == 3 )
			{
				mAddMC = add_rand(id->mRandMC,id->mRandRange);
				add_count += mAddMC;
			}
			if( r == 4 )
			{
				mAddSC = add_rand(id->mRandSC,id->mRandRange);
				add_count += mAddSC;
			}
		}
		for(int i = 0;i< 100&&add_count<=0;i++)
		{
			r = rand()%5;
			if( r == 0 && ( id->mRandAC > 0) )
			{
				mAddAC = 1;add_count++;
			}else if( r == 1 && ( id->mRandMAC > 0) )
			{
				mAddMAC = 1;add_count++;
			}else if( r == 2 && ( id->mRandDC > 0) )
			{
				mAddDC = 1;add_count++;
			}else if( r == 3 && ( id->mRandMC > 0) )
			{
				mAddMC = 1;add_count++;
			}else if( r == 4 && ( id->mRandSC > 0) )
			{
				mAddSC = 1;add_count++;
			}
		}
		if( mAddAC > 0 )
		{
			mAddAC += id->mAddBaseAC;
		}
		if( mAddMAC > 0 )
		{
			mAddMAC += id->mAddBaseMAC;
		}
		if( mAddDC > 0 )
		{
			mAddDC += id->mAddBaseDC;
		}
		if( mAddMC > 0 )
		{
			mAddMC += id->mAddBaseMC;
		}
		if( mAddSC > 0 )
		{
			mAddSC += id->mAddBaseSC;
		}
	}
	int SubItem::ItemAddType1(ItemDef* id,int flags)
	{
		if( mAddAC+mAddMAC+mAddDC+mAddMC+mAddSC >= id->mAddBaseAC+id->mRandAC+id->mAddBaseMAC+id->mRandMAC+id->mAddBaseDC+id->mRandDC+id->mAddBaseMC+id->mRandMC+id->mAddBaseSC+id->mRandSC)
		{
			return 1;
		}
		else
		{
			mAddAC = mAddMAC = mAddDC = mAddMC = mAddSC = 0;
		}
		mItemFlags |= ITEM_FLAG_JIPIN;
		if( id->mRandAC > 0 )
		{
			mAddAC = id->mAddBaseAC + rand() % id->mRandAC + 1;
		}
		if( id->mRandMAC > 0 )
		{
			mAddMAC = id->mAddBaseMAC + rand() % id->mRandMAC + 1;
		}
		if( id->mRandDC > 0 )
		{
			mAddDC = id->mAddBaseDC + rand() % id->mRandDC + 1;
		}
		if( id->mRandMC > 0 )
		{
			mAddMC = id->mAddBaseMC + rand() % id->mRandMC + 1;
		}
		if( id->mRandSC > 0 )
		{
			mAddSC = id->mAddBaseSC + rand() % id->mRandSC + 1;
		}
		return 0;
	}
	bool SubItem::BuildItemInstance(ItemDef* id,int flags,bool rand_add,int add_type,int number)
	{
		mTypeID = id->mTypeID;
		mSubType = id->SubType;
		mPosition = 0;
		mDuraMax = id->mDurationMax;
		mCreateTime = int(time(0));
		mLasttime = id->mLastTime;
		mIdentId = SubItem::GenSubItemId();
		//mIdentID = sIdGen++;
		mNumber = number;
		//if( id->mDurationMax > 0 )
		//{
		//	int t = id->mDurationMax/3*2;
		//	if( t > 0 )
		//	{
		//		mDuraMax = id->mDurationMax/3*2 + rand() % t;
		//	}
		//}

		if(ItemDef::IsEquipment(mSubType))
		{
			mUpdMaxCount=ITEM_UPGRADE_COUNT;
		}

		mDuration = mDuraMax;
		mItemFlags = flags;
		if (ItemDef::IsGem(id->SubType))
		{
			mItemFlags = 0|ITEM_FLAG_BIND;
		}
		if( rand_add )
		{
			if( add_type == 1 )
			{
				ItemAddType1(id,mItemFlags);
			}
			else
			{
				ItemAddType0(id,mItemFlags);
			}
		}
		return true;
	}

	int SubItem::ItemUpgrade(DomainPlay* play,int updac,int updmac,int upddc,int updmc,int updsc)
	{
		if(mUpdMaxCount-mUpdFailedCount-mLevel>=0)
		{
			mLevel++;
			mUpdAC+=updac;
			mUpdMAC+=updmac;
			mUpdDC+=upddc;
			mUpdMC+=updmc;
			mUpdSC+=updsc;

			return mLevel;
		}
		return 0;
	}

	int SubItem::ItemUpgrade(DomainPlay* play, int level)
	{
		if(mUpdMaxCount-mUpdFailedCount-mLevel>=0)
		{
			mLevel=level;
			return mLevel;
		}
		return 0;
	}

	int SubItem::ItemZhuLing(DomainPlay* play)
	{
		if(mZLevel<ITEM_UPGRADE_COUNT)
		{
			mZLevel++;
			return mZLevel;
		}
		return 0;
	}
	int SubItem::RemoveZhuLing(DomainPlay* play)
	{
		if(mZLevel > 0)
		{
			mZLevel = 0;
			return 1;
		}
		return 0;
	}
	void SubItem::ApplyAttribute(DomainPlay* play,ItemDef* id,ObjectAttribute* attr,ObjectAttribute* baseattr,ScriptItemAttibuteParam* scriptattr)
	{
		if( mDuration <= 0 ) return;
		if( scriptattr )
		{
			attr->mAC += static_cast<int>(id->mAC*(scriptattr->mACMul+1.0f)+scriptattr->mACAdd);
			attr->mACMax += static_cast<int>(id->mACMax*(scriptattr->mACMaxMul+1.0f)+scriptattr->mACMaxAdd + mAddAC + mUpdAC);
			attr->mMAC += static_cast<int>(id->mMAC*(scriptattr->mMACMul+1.0f)+scriptattr->mMACAdd);
			attr->mMACMax += static_cast<int>(id->mMACMax*(scriptattr->mMACMaxMul+1.0f)+scriptattr->mMACMaxAdd + mAddMAC + mUpdMAC);
			attr->mDC += static_cast<int>(id->mDC*(scriptattr->mDCMul+1.0f)+scriptattr->mDCAdd);
			attr->mDCMax += static_cast<int>(id->mDCMax*(scriptattr->mDCMaxMul+1.0f)+scriptattr->mDCMaxAdd + mAddDC + mUpdDC);
			attr->mMC += static_cast<int>(id->mMC*(scriptattr->mMCMul+1.0f)+scriptattr->mMCAdd);
			attr->mMCMax += static_cast<int>(id->mMCMax*(scriptattr->mMCMaxMul+1.0f)+scriptattr->mMCMaxAdd + mAddMC + mUpdMC);
			attr->mSC += static_cast<int>(id->mSC*(scriptattr->mSCMul+1.0f)+scriptattr->mSCAdd);
			attr->mSCMax += static_cast<int>(id->mSCMax*(scriptattr->mSCMaxMul+1.0f)+scriptattr->mSCMaxAdd + mAddSC + mUpdSC);
		}
		else
		{
			attr->mAC += id->mAC;
			attr->mACMax += id->mACMax + mAddAC + mUpdAC;
			attr->mMAC += id->mMAC;
			attr->mMACMax += id->mMACMax + mAddMAC + mUpdMAC;
			attr->mDC += id->mDC;
			attr->mDCMax += id->mDCMax + mAddDC + mUpdDC;
			attr->mMC += id->mMC;
			attr->mMCMax += id->mMCMax + mAddMC + mUpdMC;
			attr->mSC += id->mSC;
			attr->mSCMax += id->mSCMax + mAddSC + mUpdSC;
		}
		attr->mAccuracy += id->mAccuracy;
		attr->mDodge += id->mDodge;
		attr->mLuck += id->mLuck;
		attr->mLuck -= id->mCurse;
		attr->mLuck += mLuck;
		attr->mAntiMagic += id->mAntiMagic;
		attr->mAntiPoison += id->mAntiPoison;
		attr->mHpRecover += id->mHpRecover;
		attr->mMpRecover += id->mMpRecover;
		attr->mPoisonRecover += id->mPoisonRecover;
		attr->mDoubleAttProb += id->mBaoji;
		attr->mDropProb += id->mDrop_luck;
		attr->mMaxPower += id->mAddPower;
		attr->mHolyDam += id->mHolyDam;

		if(attr->mXishouProb < id->mXishou_prob)
		{
			attr->mXishouProb = id->mXishou_prob;
		}
		attr->mXishouPres += id->mXishou_pres;
		if(attr->mFantanProb < id->mFantan_prob)
		{
			attr->mFantanProb = id->mFantan_prob;
		}
		attr->mFantanPres += id->mFantan_pres;
		if(attr->mXixueProb < id->mXixue_pres)
		{
			attr->mXixueProb = id->mXixue_pres;
		}
		attr->mXixuePres += id->mXixue_pres;
		if(attr->mBaojiProb < id->mBaoji_prob)
		{
			attr->mBaojiProb = id->mBaoji_prob;
		}
		attr->mBaojiPres += id->mBaoji_pres;

		attr->mMabi_prob += id->mMabi_prob;
		if(attr->mMabi_dura<id->mMabi_dura)
		{
			attr->mMabi_dura = id->mMabi_dura;
		}
		attr->mDixiao_pres += id->mDixiao_pres;
		if(attr->mFuyuan_cd <= 0 )
		{
			attr->mFuyuan_cd = id->mFuyuan_cd;
		}
		if(attr->mFuyuan_cd>id->mFuyuan_cd&&id->mFuyuan_cd>0)
		{
			attr->mFuyuan_cd = id->mFuyuan_cd;
		}
		attr->mFuyuan_pres+=id->mFuyuan_pres;

		if(scriptattr)
		{
			attr->mMaxHp += static_cast<int>(baseattr->mMaxHp*(scriptattr->mMaxHpMul/10000.0f)+scriptattr->mMaxHpAdd + id->mMax_hp + mAddHp);
			attr->mMaxMp += static_cast<int>(baseattr->mMaxMp*(scriptattr->mMaxMpMul/10000.0f)+scriptattr->mMaxMpAdd + id->mMax_mp + mAddMp);
		}
		else
		{
			attr->mMaxHp += id->mMax_hp + mAddHp;
			attr->mMaxMp += id->mMax_mp + mAddMp;
		}
		if( id->mMax_hp_pres > 0 )
		{
			attr->mMaxHp += int(baseattr->mMaxHp * 1.0 / 10000.0 * id->mMax_hp_pres);
		}
		if( id->mMax_mp_pres > 0 )
		{
			attr->mMaxMp += int(baseattr->mMaxMp * 1.0 / 10000.0 * id->mMax_mp_pres);
		}
		for( std::vector<ItemPlusDef>::iterator pos = id->mPlus.begin();
			pos != id->mPlus.end();
			++ pos )
		{
			ApplyAttributePlus(play,id,&(*pos),attr,baseattr);
		}
		for( std::vector<ItemPlusDef>::iterator pos = id->mPlusZl.begin();
			pos != id->mPlusZl.end();
			++ pos )
		{
			ApplyAttributePlus(play,id,&(*pos),attr,baseattr);
		}
	}
	void SubItem::ApplyAttributePlus(DomainPlay* play,ItemDef* idd,ItemPlusDef* ipd,ObjectAttribute* attr,ObjectAttribute* baseattr)
	{
		bool need_ok = false;
		switch( ipd->mNeedType )
		{
		case 200:
			{
				if( mLevel >= ipd->mNeedParam1 )
				{
					need_ok = true;
				}
			}
			break;
		case 201:
			{
				if( mItemFlags & ITEM_FLAG_JIANDING )
				{
					need_ok = true;
				}
			}
			break;
		case 202:
			{
				if( mAddAC+mAddMAC+mAddDC+ mAddMC+mAddSC >= ipd->mNeedParam1 )
				{
					need_ok = true;
				}
			}
			break;
		case 203:
			{
				if( play->level() >= ipd->mNeedParam1 )
				{
					need_ok = true;
				}
			}
			break;
		case 204:
			{
				if( mZLevel >= ipd->mNeedParam1 )
				{
					need_ok = true;
				}
			}
			break;
		case 205:
			{
				if (ItemDef::IsEquipment(idd->SubType))
				{
					int on_position = ipd->mTypeID / 10000 * (-1) * 2;
					int on_position_2 = on_position - 1;
					bool one_full = true;
					int cycle_pos = 0;
					if (on_position == -5 * 2 || on_position == -6 * 2)
					{
				
					}
					std::map<int, int>equipmap;
					int data1[10] = { 7606,7607,7608,7609,7610,7611,7612,7613,7614,7615};
					int data2[10] = {-4,-8,-12,-10,-18,-6,-14,-13,-11,-20};
					for (int i = 0; i < 10; i++)
					{
						equipmap[data2[i]] = data1[i];
					}
					int lv = 0;
					if (play->Param(equipmap[on_position]).length())
					{
						lv = atoi(play->Param(equipmap[on_position]).c_str());
					}
					if (lv >= ipd->mNeedParam1)
					{
						need_ok = true;
					}
				}
			}
			break;
		}
		if( need_ok )
		{
			attr->mAC += ipd->mAC;
			attr->mACMax += ipd->mACMax;
			attr->mMAC += ipd->mMAC;
			attr->mMACMax += ipd->mMACMax;
			attr->mDC += ipd->mDC;
			attr->mDCMax += ipd->mDCMax;
			attr->mMC += ipd->mMC;
			attr->mMCMax += ipd->mMCMax;
			attr->mSC += ipd->mSC;
			attr->mSCMax += ipd->mSCMax;
			attr->mAccuracy += ipd->mAccuracy;
			attr->mDodge += ipd->mDodge;
			attr->mLuck += ipd->mLuck;
			attr->mLuck -= ipd->mCurse;
			attr->mAntiMagic += ipd->mAntiMagic;
			attr->mAntiPoison += ipd->mAntiPoison;
			attr->mHpRecover += ipd->mHpRecover;
			attr->mMpRecover += ipd->mMpRecover;
			attr->mPoisonRecover += ipd->mPoisonRecover;

			attr->mMaxHp += ipd->mMax_hp;
			attr->mMaxMp += ipd->mMax_mp;
			if( ipd->mHalfBaoji > 0 )
			{
				attr->mDoubleAttProb += ipd->mHalfBaoji;
			}
			if( ipd->mMax_hp_pres > 0 )
			{
				attr->mMaxHp += int(baseattr->mMaxHp * 1.0 / 10000.0 * ipd->mMax_hp_pres);
			}
			if( ipd->mMax_mp_pres > 0 )
			{
				attr->mMaxMp += int(baseattr->mMaxMp * 1.0 / 10000.0 * ipd->mMax_mp_pres);
			}
		}
	}
	int SubItem::RepairPrice(ItemDef* id)
	{
		if( ItemDef::IsEquipment(mSubType) )
		{
			int d = mDuraMax - min( mDuraMax,max( 0,mDuration ) );
			return int(id->mPrice / 3.0 / mDuraMax * d);
		}
		return id->mPrice;
	}
	int SubItem::RemainTime(ItemDef* id)
	{
		if(id)
		{
			int remain = max(0,static_cast<int>(id->mLastTime + mCreateTime - time(0)));
			return remain;
		}
		return 0;
	}

	LEUD_INT64 SubItem::GenSubItemId()
	{
		LEUD_INT64 time_stamp = time(0);
		time_stamp = time_stamp << 32 & 0xFFFFFFFF00000000;
		time_stamp += SubItem::sIdGen;
		++ SubItem::sIdGen;
		return time_stamp;
	}

	/////////////////////////////////////////////////////////////////////////////////
}
