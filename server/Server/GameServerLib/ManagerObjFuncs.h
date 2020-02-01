#pragma once
namespace OGSLib
{
	enum 
	{
		FUNC_PROP_MAX_COUNT = 6,
	};

	enum AttrTypeFuncID 
	{
		type_gold = 100,
		type_bindgold = 101,
		type_vcoin = 102,
		type_bindvcoin = 103,
		type_lv = 104,
		type_zs_lv = 105,
		type_exp = 106,
		type_zhanshen_exp = 107,
		type_neigong_lv = 108,
		type_neigong_exp = 109,
		type_neigong_exp1 = 110,
		type_neigong = 111,
		type_neigong_max = 112,
		type_vip = 113,
		type_zhuanshen = 114,
		type_tenacity = 115,
		type_dam_zs_to_fs,
		type_dam_fs_to_ds, 
		type_dam_ds_to_fs,
		type_cz_record = 119,
		type_vip_relive,
		type_bz_point,
		type_item_take_point,
		type_gongxun,
		type_gongxuntitle,
		type_kill_mon_exp_rate,
		type_dam_mon,
		type_zs_dam_mon,
		type_item_take_exp_max,
		type_depot_max,
		type_miemo,
		type_miemo_max,
		type_dam_to_player,
		type_reduce_dc_to_player,
		type_reduce_mc_to_player,
		type_yupei = 551,
		type_hudun,
		type_langya,
		type_hunzhu,
		type_max_hp = 1000,
		type_hp,
		type_max_mp,
		type_mp,
		type_dc,
		type_dcmax,
		type_mc,
		type_mcmax,
		type_sc,
		type_scmax,
		type_ac,
		type_acmax,
		type_mac,
		type_macmax,
		type_baojiprob,
		type_baojipres,
		type_accuracy,
		type_dodge,
		type_luck,
		type_holydam,
		type_holyac,
		type_reducedc,
		type_reducemc,
		type_max_hp_rate,
		type_max_mp_rate,
		type_ac_rate,
		type_ac_max_rate,
		type_mac_rate,
		type_mac_max_rate,
		type_dc_rate,
		type_dc_max_rate,
		type_mc_rate,
		type_mc_max_rate,
		type_sc_rate,
		type_sc_max_rate,
	};


	class IFuncImp;

	struct PropFunc
	{
		int nProp;
		int nVal;
		IFuncImp * mFunc;
	};

	class FuncInfo
	{
	public:
		int m_nDefId;
		int m_nFuncId;
		PropFunc m_propFuncs[FUNC_PROP_MAX_COUNT];
		// 是否重新计算属性
		int m_nCalAttr;
		std::string m_strDesp;
		FuncInfo()
		{
			m_nDefId=0;
			m_nFuncId=0;
			memset(m_propFuncs, 0, sizeof(PropFunc) * FUNC_PROP_MAX_COUNT);
			m_strDesp = "";
			m_nCalAttr = 0;
		}
	};

	class GameServerLib;
	class PlayActiveObject;
//////////////////////////////////////////////////////////////////////////////////////////////

	class IFuncImp
	{
	public:
		virtual int onHandle(PlayActiveObject * src) = 0;
		virtual int onHandleRemove(PlayActiveObject * src) = 0;
		virtual void setPropFuncInfo(PropFunc * p) = 0;
	};

	class FuncImpCommon : public IFuncImp
	{
	public:
		FuncImpCommon();
		FuncImpCommon(GameServerLib * pLib);
		virtual int onHandle(PlayActiveObject * src);
		virtual int onHandleRemove(PlayActiveObject * src);
		virtual void setPropFuncInfo(PropFunc * p);
	protected:
		PropFunc * m_info;
		GameServerLib * mGSL;
	};

	class FuncImpAddDC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddDCMax : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddMC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddMCMax : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddSC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddSCMax : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddAC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddACMax : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddMAC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddMACMax : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddHPMax : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddMPMax : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddBaojiProp : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddBaojiPres : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddAccuracy : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddDodge : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddLuck : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAddHolyDam : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddHP : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddMP : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	// 125
	class FuncImpAddExpGain : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 1023
	class FuncImpAddHPRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 132
	class FuncImpAddPKIncre : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 133
	class FuncImpAddPKReduceDC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 134
	class FuncImpAddPKReduceMC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 126
	class FuncImpAddDamMonIncre : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 127
	class FuncImpAddZSDamMonIncre : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 1021
	class FuncImpAddDamReduceDC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	// 1022
	class FuncImpAddDamReduceMC : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

////////////

	class FuncImpAdddcRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAdddcmaxRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAddmcRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAddmcmaxRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAddscRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAddscmaxRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddacRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};

	class FuncImpAddacmaxRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAddmacRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	class FuncImpAddmacmaxRate : public FuncImpCommon
	{
	public:
		virtual int onHandle(PlayActiveObject * src);
	};
	
//////////////////////////////////////////////////////////////////////////////////////
	class FuncFactory
	{
	public:
		IFuncImp * CreateFunc(GameServerLib * gsl, PropFunc * p);	
	};

	class ManagerObjFuncs
	{
	public:
		typedef std::map<int,FuncInfo> FuncsInfoMap;
	private:
		FuncFactory mFuncFactory;
		FuncsInfoMap mFuncsInfos;
		GameServerLib* mGameServerLib;
	public:
		ManagerObjFuncs(GameServerLib* gslib);
		virtual ~ManagerObjFuncs(void);
	public:
		bool Init();
		void Dest();
		FuncInfo * GetFuncInfo(int id);
	public:
		bool LoadFuncsInfo();
		bool ReloadFuncsInfo();
	};
}