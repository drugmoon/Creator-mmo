#include "StdAfx.h"
#include "ManagerObjFuncs.h"
#include "GameServerLib.h"
#include "leudgrid/ToolsMemFile.h"
#include "leudgrid/Cryptography.h"
#include "PlayActiveObject.h"
#include "DomainPlay.h"
#include "ScriptLua.h"

namespace OGSLib
{
	ManagerObjFuncs::ManagerObjFuncs(GameServerLib* gslib)
	{
		mGameServerLib = gslib;
	}
	ManagerObjFuncs::~ManagerObjFuncs(void)
	{
	}
	bool ManagerObjFuncs::Init()
	{
		return LoadFuncsInfo();
	}

	void ManagerObjFuncs::Dest()
	{
		std::map<int,FuncInfo>::iterator it = mFuncsInfos.begin();
		while (it != mFuncsInfos.end())
		{
			for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
				SAFE_DELETE(it->second.m_propFuncs[i].mFunc);
			++ it;
		}
	}

	bool ManagerObjFuncs::LoadFuncsInfo()
	{
		mFuncsInfos.clear();
		char temp[256];
		char desp[256];
		size_t size= 0;
		int version = 100;
		const char* data = mGameServerLib->GetListener()->LoadFile("funcsdef.csv",size);
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
						FuncInfo fi;
						memset(desp,0,256);
						int ret = sscanf_s(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^,]",
							&fi.m_nDefId,&fi.m_nFuncId,
							&fi.m_propFuncs[0].nProp,&fi.m_propFuncs[0].nVal,
							&fi.m_propFuncs[1].nProp,&fi.m_propFuncs[1].nVal,
							&fi.m_propFuncs[2].nProp,&fi.m_propFuncs[2].nVal,
							&fi.m_propFuncs[3].nProp,&fi.m_propFuncs[3].nVal,
							&fi.m_propFuncs[4].nProp,&fi.m_propFuncs[4].nVal,
							&fi.m_propFuncs[5].nProp,&fi.m_propFuncs[5].nVal,
							&fi.m_nCalAttr,
							desp,_countof(desp));
						if( 16 == ret )
						{
							if( mFuncsInfos.find(fi.m_nDefId) != mFuncsInfos.end() )
							{
								continue;
							}
							UtilString::act_2_utf8(desp,strlen(desp),temp,256);strncpy_s(desp,_countof(desp),temp,255);desp[255]=0;fi.m_strDesp = desp;
							
							mFuncsInfos[fi.m_nDefId] = fi;
							FuncInfo &info = mFuncsInfos[fi.m_nDefId];
							for (size_t i = 0; i < FUNC_PROP_MAX_COUNT; ++ i)
							{
								info.m_propFuncs[i].mFunc = mFuncFactory.CreateFunc(mGameServerLib, &(info.m_propFuncs[i]));
							}
						}
					}
				}
			}
		}
		if( mFuncsInfos.empty() )
		{
			return false;
		}
		return true;
	}

	bool ManagerObjFuncs::ReloadFuncsInfo()
	{
		FuncsInfoMap i = mFuncsInfos;
		if (LoadFuncsInfo())
		{
			// 加载成功,删除老的指针
			FuncsInfoMap::iterator it = i.begin();
			while (it != i.end())
			{
				for (int j = 0; j < FUNC_PROP_MAX_COUNT; ++ j)
				{
					SAFE_DELETE(it->second.m_propFuncs[j].mFunc);
				}
				
				++ it;
			}
			return true;
		}
		
		mFuncsInfos = i;

		return false;
	}

	FuncInfo * ManagerObjFuncs::GetFuncInfo(int id)
	{
		FuncsInfoMap::iterator it = this->mFuncsInfos.find(id);

		if (it == mFuncsInfos.end())
			return 0;

		return &(it->second);
	}

	FuncImpCommon::FuncImpCommon(GameServerLib * pLib)
	{
		mGSL = pLib;
		m_info = NULL;
	}

	FuncImpCommon::FuncImpCommon()
	{
		m_info = NULL;
	}

	int FuncImpCommon::onHandle(PlayActiveObject * src)
	{
		if (src->TypeIsPlayer() && m_info)
		{
			char func[256] = {0};

			if (src->TypeIsPlayer())
			{
				return mGSL->GetScriptEngine()->PlayerInt(static_cast<DomainPlay *>(src), m_info->nProp, m_info->nVal, 0, 0, "player.onFunction");
			}
		}
		return 0;
	}

	int FuncImpCommon::onHandleRemove(PlayActiveObject * src)
	{
		if (src->TypeIsPlayer() && m_info)
		{
			char func[256] = {0};

			if (src->TypeIsPlayer())
			{
				return mGSL->GetScriptEngine()->PlayerInt(static_cast<DomainPlay *>(src), m_info->nProp, m_info->nVal, 0, 0, "player.onFunctionRemove");
			}
		}
		return 0;
	}

	void FuncImpCommon::setPropFuncInfo(PropFunc * p)
	{
		m_info = p;
	}

	IFuncImp * FuncFactory::CreateFunc(GameServerLib * gsl, PropFunc * p)
	{
		if (p == NULL || p->nProp <= 0)
			return NULL;

		IFuncImp * pImp = NULL;
		switch (p->nProp)
		{
		case type_max_hp:
			pImp = new FuncImpAddHPMax;
			break;
		case type_max_mp:
			pImp = new FuncImpAddMPMax;
			break;
		case type_hp : 
			pImp = new FuncImpAddHP;
			break;
		case type_mp : 
			pImp = new FuncImpAddMP;
			break;
		case type_dc:
			pImp = new FuncImpAddDC;
			break;
		case type_dcmax:
			pImp = new FuncImpAddDCMax;
			break;
		case type_mc:
			pImp = new FuncImpAddMC;
			break;
		case type_mcmax:
			pImp = new FuncImpAddMCMax;
			break;
		case type_sc:
			pImp = new FuncImpAddSC;
			break;
		case type_scmax:
			pImp = new FuncImpAddSCMax;
			break;
		case type_ac:
			pImp = new FuncImpAddAC;
			break;
		case type_acmax:
			pImp = new FuncImpAddAC;
			break;
		case type_mac:
			pImp = new FuncImpAddMAC;
			break;
		case type_macmax:
			pImp = new FuncImpAddMACMax;
			break;
		case type_baojiprob:
			pImp = new FuncImpAddBaojiProp;
			break;
		case type_baojipres:
			pImp = new FuncImpAddBaojiPres;
			break;
		case type_accuracy:
			pImp = new FuncImpAddAccuracy;
			break;
		case type_dodge:
			pImp = new FuncImpAddDodge;
			break;
		case type_luck:
			pImp = new FuncImpAddLuck;
			break;
		case type_holydam:
			pImp = new FuncImpAddHolyDam;
			break;
		case type_kill_mon_exp_rate:
			pImp = new FuncImpAddExpGain;
			break;
		case type_max_hp_rate:
			pImp = new FuncImpAddHPRate;
			break;
		case type_dam_to_player:
			pImp = new FuncImpAddPKIncre;
			break;
		case type_reduce_dc_to_player:
			pImp = new FuncImpAddPKReduceDC;
			break;
		case type_reduce_mc_to_player:
			pImp = new FuncImpAddPKReduceMC;
			break;
		case type_dam_mon:
			pImp = new FuncImpAddDamMonIncre;
			break;
		case type_zs_dam_mon:
			pImp = new FuncImpAddZSDamMonIncre;
			break;
		case type_reducedc:
			pImp = new FuncImpAddDamReduceDC;
			break;
		case type_reducemc:
			pImp = new FuncImpAddDamReduceMC;
			break;
///////////////////
		case type_dc_rate:
			pImp = new FuncImpAdddcRate;
			break;
		case type_dc_max_rate:
			pImp = new FuncImpAdddcmaxRate;
			break;
		case type_mc_rate:
			pImp = new FuncImpAddmcRate;
			break;
		case type_mc_max_rate:
			pImp = new FuncImpAddmcmaxRate;
			break;
		case type_sc_rate:
			pImp = new FuncImpAddscRate;
			break;
		case type_sc_max_rate:
			pImp = new FuncImpAddscmaxRate;
			break;

		case type_ac_rate:
			pImp = new FuncImpAddacRate;
			break;
		case type_ac_max_rate:
			pImp = new FuncImpAddacmaxRate;
			break;
		case type_mac_rate:
			pImp = new FuncImpAddmacRate;
			break;
		case type_mac_max_rate:
			pImp = new FuncImpAddmacmaxRate;
			break;

///////////////
		default:
			pImp = new FuncImpCommon(gsl);
			break;
		}
		pImp->setPropFuncInfo(p);
		return pImp;
	}

	int FuncImpAddDC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mDC += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddDCMax::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mDCMax += m_info->nVal;
		return m_info->nVal;
	}


	int FuncImpAddMC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mMC += m_info->nVal;
		return m_info->nVal;
	}


	int FuncImpAddMCMax::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mMCMax += m_info->nVal;
		return m_info->nVal;
	}


	int FuncImpAddSC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mSC += m_info->nVal;
		return m_info->nVal;
	}


	int FuncImpAddSCMax::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mSCMax += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddAC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mAC += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddACMax::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mACMax += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddMAC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mMAC += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddMACMax::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mMACMax += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddHPMax::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mMaxHp += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddMPMax::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mMaxMp += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddBaojiProp::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mBaojiProb += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddBaojiPres::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mBaojiPres += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddAccuracy::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mAccuracy += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddDodge::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mDodge += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddLuck::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mLuck += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddHolyDam::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mHolyDam += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddHP::onHandle(PlayActiveObject * src)
	{
		if ( m_info->nVal >= 0)
		{
			if (src->AddHP(m_info->nVal))
			{
				if (src->TypeIsPlayer())
				{
					DomainPlay * player = static_cast<DomainPlay*>(src);
					player->HpMpChangeBroadcast(m_info->nVal, 0);
				}
				return m_info->nVal;
			}
			else
				return 0;
		}
		else
		{
			if (src->DamageHP(0, -m_info->nVal, 0, true))
				return m_info->nVal;
			else
				return 0;
		}

		return 0;
	}

	int FuncImpAddMP::onHandle(PlayActiveObject * src)
	{
		if ( m_info->nVal >= 0)
		{
			if (src->AddMP(m_info->nVal))
			{
				if (src->TypeIsPlayer())
				{
					DomainPlay * player = static_cast<DomainPlay*>(src);
					player->HpMpChangeBroadcast(0, m_info->nVal);
				}
				
				return m_info->nVal;
			}	
			else
				return 0;
		}
		else
		{
			if (src->DamageMP(-m_info->nVal))
				return m_info->nVal;
			else
				return 0;
		}

		return 0;
	}

	int FuncImpAddExpGain::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mExpGain += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddHPRate::onHandle(PlayActiveObject * src)
	{
		if (src->TypeIsPlayer())
		{
			src->mHpPro = true;
		}
		src->PropPercent()->mMaxHp += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddPKIncre::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mPKIncrement += m_info->nVal;
		return m_info->nVal;
	}


	int FuncImpAddPKReduceDC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mPKReduceDC += m_info->nVal;
		return m_info->nVal;
	}


	int FuncImpAddPKReduceMC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mPKReduceMC += m_info->nVal;
		return m_info->nVal;
	}


	int FuncImpAddDamMonIncre::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mMonDamIncrement += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddZSDamMonIncre::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mZSMonDamIncrement += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddDamReduceDC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mDamReduceDC += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddDamReduceMC::onHandle(PlayActiveObject * src)
	{
		src->Attributes()->mDamReduceMC += m_info->nVal;
		return m_info->nVal;
	}
	////////////////////////
	int FuncImpAdddcRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mDC += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAdddcmaxRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mDCMax += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAddmcRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mMC += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAddmcmaxRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mMCMax += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAddscRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mSC += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAddscmaxRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mSCMax += m_info->nVal;
		return m_info->nVal;
	}

	int FuncImpAddacRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mAC += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAddacmaxRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mACMax += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAddmacRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mMAC += m_info->nVal;
		return m_info->nVal;
	}
	int FuncImpAddmacmaxRate::onHandle(PlayActiveObject * src)
	{
		src->PropPercent()->mMACMax += m_info->nVal;
		return m_info->nVal;
	}
}
