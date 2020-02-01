#pragma once

#include "ProtocolHead.h"

namespace Client_GS
{
	using namespace ProtocolHead;
}
namespace GS_Client
{
	using namespace ProtocolHead;
}
enum Client_GS_Protocol
{
	Client_GS_Authenticate = 0x7000,
	GS_Client_Authenticate = 0x7001,

	GS_Client_Character_Loaded_Notify = 0x7003,

	Client_GS_Map_Chat = 0x7010,
	GS_Client_Map_Chat = 0x7011,
	GS_Client_Map_Chat_Notify = 0x7012,

	Client_GS_Private_Chat = 0x7020,
	GS_Client_Private_Chat = 0x7021,
	GS_Client_Private_Chat_Notify = 0x7022,

	Client_GS_Walk = 0x7030,
	GS_Client_Walk = 0x7031,
	GS_Client_Walk_Notify = 0x7032,

	Client_GS_Run = 0x7040,
	GS_Client_Run = 0x7041,
	GS_Client_Run_Notify = 0x7042,

	Client_GS_NPCTalk = 0x7050,
	GS_Client_NPCTalk = 0x7051,

	Client_GS_Turn = 0x7060,
	GS_Client_Turn = 0x7061,
	GS_Client_Turn_Notify = 0x7062,

	Client_GS_Attack = 0x7070,
	GS_Client_Attack = 0x7071,
	GS_Client_Attack_Notify = 0x7072,

	Client_GS_PickUp = 0x7080,
	GS_Client_PickUp = 0x7081,

	Client_GS_BagUseItem = 0x7090,
	GS_Client_BagUseItem = 0x7091,

	Client_GS_UndressItem = 0x70A0,
	GS_Client_UndressItem = 0x70A1,

	Client_GS_ItemPositionExchange = 0x70B0,
	GS_Client_ItemPositionExchange = 0x70B1,

	Client_GS_UseSkill = 0x70C0,
	GS_Client_UseSkill = 0x70C1,
	GS_Client_UseSkill_Notify = 0x70C2,

	Client_GS_NPCShop = 0x70D0,
	GS_Client_NPCShop = 0x70D1,

	Client_GS_NPCBuy = 0x70E0,
	GS_Client_NPCBuy = 0x70E1,

	Client_GS_CancelTask = 0x70F0,

	Client_GS_ForceMove = 0x7100,
	Client_GS_ChangeCloth = 0x7110,

	Client_GS_ListCharacter = 0x7120,
	GS_Client_ListCharacter = 0x7121,

	Client_GS_CreateCharacter = 0x7130,
	GS_Client_CreateCharacter = 0x7131,

	Client_GS_EnterGame = 0x7140,
	GS_Client_EnterGame = 0x7141,

	Client_GS_DeleteCharacter = 0x7150,
	GS_Client_DeleteCharacter = 0x7151,

	Client_GS_NPCSell = 0x7160,
	GS_Client_NPCSell = 0x7161,

	Client_GS_DropItem = 0x7170,
	GS_Client_DropItem = 0x7171,

	Client_GS_ListGuild = 0x7180,
	GS_Client_ListGuild = 0x7181,

	Client_GS_GetGuildInfo = 0x7190,
	GS_Client_GetGuildInfo = 0x7191,

	Client_GS_SaveShortcut = 0x71A0,
	GS_Client_LoadShortcut = 0x71A2,

	Client_GS_CreateGuild = 0x71B0,
	GS_Client_CreateGuild = 0x71B1,

	Client_GS_JoinGuild = 0x71C0,
	GS_Client_JoinGuild = 0x71C1,

	Client_GS_SetGuildInfo = 0x71D0,
	GS_Client_SetGuildInfo = 0x71D1,

	Client_GS_ListGuildMember = 0x71E0,
	GS_Client_ListGuildMember = 0x71E1,
	GS_Client_GuildMemberChgNotify = 0x71E2,

	Client_GS_ListGuildEnemy = 0x71F0,
	GS_Client_ListGuildEnemy = 0x71F1,

	Client_GS_ListGuildFriend = 0x7200,
	GS_Client_ListGuildFriend = 0x7201,

	Client_GS_ChangeGuildMemberTitle = 0x7210,

	Client_GS_ChangeEnemyGuild = 0x7220,

	Client_GS_ChangeFriendGuild = 0x7230,

	Client_GS_Guild_Chat = 0x7240,
	GS_Client_Guild_Chat = 0x7241,
	GS_Client_Guild_Chat_Notify = 0x7242,

	Client_GS_ListGuildItem = 0x7243,
	GS_Client_ListGuildItem = 0x7244,

	Client_GS_Guild_Red_Packet_Log_Req = 0x7245,
	GS_Client_Guild_Red_Packet_Log_Res = 0x7246,
	GS_Client_Guild_Red_Packet_Log_Notify = 0x7247,
	Client_GS_Guild_Item_Log_Req = 0x7248,
	GS_Client_Guild_Item_Log_Res = 0x7249,

	GS_Client_Guild_War_Notify = 0x724A,

	Client_GS_ChangeAttackMode = 0x7250,
	GS_Client_ChangeAttackMode = 0x7251,

	Client_GS_VcoinShopList = 0x7260,
	GS_Client_VcoinShopList = 0x7261,

	Client_GS_NPCRepair = 0x7280,
	GS_Client_NPCRepair = 0x7281,

	Client_GS_Relive = 0x7290,
	GS_Client_Relive = 0x7291,

	KuaFu_GS_Get_Char_Data= 0x72A0,
	GS_KuaFu_Get_Char_Data=0x72A1,

	Client_GS_Kuafu_Auth=0x72B0,
	GS_Client_Kuafu_Auth=0x72B1,

	KuaFu_GS_Set_Param= 0x72C0,
	GS_KuaFu_Set_Param= 0x72C1,

	Client_GS_Kuafu_PT_Info=0x72D0,
	GS_Client_Kuafu_PT_Info=0x72D1,

	KuaFu_GS_Player_Data= 0x72E0,

	KuaFu_GS_Comeback_Data= 0x72F0,

	Client_GS_Task_Desp = 0x7300,
	GS_Client_Task_Desp = 0x7301,

	Client_GS_Info_Player = 0x7310,
	GS_Client_Info_Player = 0x7311,

	Client_GS_Create_Group = 0x7320,
	GS_Client_Create_Group = 0x7321,

	Client_GS_Leave_Group = 0x7330,
	GS_Client_Leave_Group = 0x7331,

	Client_GS_Join_Group = 0x7340,
	GS_Client_Join_Group = 0x7341,

	Client_GS_Agree_Join_Group = 0x7350,
	GS_Client_Agree_Join_Group = 0x7351,

	Client_GS_Invite_Group = 0x7360,
	GS_Client_Invite_Group = 0x7361,

	Client_GS_Agree_Invite_Group = 0x7370,
	GS_Client_Agree_Invite_Group = 0x7371,

	Client_GS_Task_Click = 0x7380,

	Client_GS_Group_Chat = 0x7390,
	GS_Client_Group_Chat = 0x7391,

	Client_GS_Normal_Chat = 0x7400,
	GS_Client_Normal_Chat = 0x7401,

	Client_GS_Trade_Invite = 0x7410,

	Client_GS_Agree_Trade_Invite = 0x7420,

	Client_GS_Close_Trade = 0x7430,

	Client_GS_Trade_Add_GameMoney = 0x7440,

	Client_GS_Trade_Add_Vcoin = 0x7450,

	Client_GS_Trade_Submit = 0x7460,

	Client_GS_Group_Set_Leader = 0x7470,

	Client_GS_Trade_Add_Item = 0x7480,

	Client_GS_Chat_Trade_Submit = 0x7481,

	Client_GS_Destory_Item = 0x7490,

	Client_GS_Sort_Item = 0x7500,

	Client_GS_Item_Talk = 0x7510,

	Client_GS_World_Chat = 0x7540,
	GS_Client_World_Chat = 0x7541,

	Client_GS_Fresh_Vcoin = 0x7550,

	Client_GS_Leave_Guild = 0x7560,

	Client_GS_Add_Depot_Slot = 0x7570,

	Client_GS_Group_Pick_Mode = 0x7580,

	Client_GS_Swith_Slave_AI_Mode = 0x7590,

	Client_GS_Ping = 0x7600,
	GS_Client_Ping = 0x7601,

	Client_GS_FreshHPMP = 0x7610,

	Client_GS_Update_Ticket = 0x7620,
	GS_Client_Update_Ticket = 0x7621,

	Client_GS_Player_Talk = 0x7660,

	Client_GS_Get_Chart_Info = 0x7670,
	GS_Client_Get_Chart_Info = 0x7671,

	Client_GS_Get_Item_Desp = 0x7700,

	Client_GS_Horn_Chat = 0x7710,
	GS_Client_Horn_Chat = 0x7711,
	GS_Client_Horn_Chat_Notify = 0x7712,

	Client_GS_Count_Down_Finish = 0x7720,

	Client_GS_Friend_Change = 0x7730,
	GS_Client_Friend_Change = 0x7731,
	
	GS_Client_Friend_Apply = 0x7732,
	Client_GS_Friend_Apply_Agree = 0x7733,

	Client_GS_Friend_Fresh = 0x7740,
	GS_Client_Friend_Fresh = 0x7741,

	Client_GS_Server_Script = 0x7770,

	Client_GS_Open_Rong = 0x7780,

	Client_GS_Protect_Item = 0x7790,
	GS_Client_Protect_Item = 0x7791,

	Client_GS_Group_Kick_Member = 0x7800,

	Client_GS_Fresh_Gift = 0x7810,

	Client_GS_OpenPK = 0x7860,
    Client_GS_Add_Bag_Slot = 0x7861,

	Client_GS_TaskList_Req=0x7873,

	Client_GS_Find_Map_Ghost = 0x7878,
	GS_Client_Find_Map_Ghost = 0x7879,

	Client_GS_Collect_Start_Req=0x7901,
	GS_Client_Collect_End_Notify=0x7902,
	Client_GS_Direct_Fly = 0x7904,

	Client_GS_PUSH_LUA_TABLE=0x7910,
	GS_Client_PUSH_LUA_TABLE=0x7911,

	Client_GS_Mount_Change_Req = 0x7980,

	Client_GS_Split_Item = 0x7A30,

	Client_GS_Get_Mails_Req = 0x7A70,
	GS_Client_Mails_Res = 0x7A71,
	Client_GS_Open_Mail_Req = 0x7A72,
	Client_GS_Receive_Mail_Items_Req = 0x7A73,
	Client_GS_Delete_Mail_Req = 0x7A74,
	GS_Client_Mail_Num_Notify = 0x7A75,
	GS_Client_Mail_Receive_Success_Notify = 0x7A76,

	Client_GS_Consign_Item_Req = 0x7B00,
	GS_Client_Consign_Item_Notify = 0x7B01,
	Client_GS_Get_ConsignableItems = 0x7B02,
	GS_Client_ConsignableItems_Notify = 0x7B03,
	Client_GS_Buy_Consignable_Item = 0x7B04,
	GS_Client_Buy_Consignable_Item_Notify = 0x7B05,
	Client_GS_Take_Back_Consignable_Item = 0x7B06,
	GS_Client_Take_Back_Consignable_Item_Notify = 0x7B07,
	Client_GS_Take_Back_VCoin = 0x7B08,
	GS_Client_Take_Back_VCoin_Notify = 0x7B09,

	GS_Client_Map_Enter_Notify = 0x7F00,
	GS_Client_Map_Meet_Notify = 0x7F01,
	GS_Client_Map_Leave_Notify = 0x7F02,
	GS_Client_Map_Bye_Notify = 0x7F03,
	GS_Client_Injury_Notify = 0x7F04,
	GS_Client_Die_Notify = 0x7F05,
	GS_Client_Item_Change_Notify = 0x7F06,
	GS_Client_Avatar_Change_Notify = 0x7F07,
	GS_Client_Skill_Change_Notify = 0x7F08,
	GS_Client_Attribute_Change_Notify = 0x7F09,
	GS_Client_Kuafu_Info_Notify = 0x7F0A,
	GS_Client_Kuafu_Enter_Main_Server_Notify = 0x7F0B,
	GS_Client_GameMoney_Change_Notify = 0x7F10,
	GS_Client_HP_MP_Change_Notify = 0x7F11,
	GS_Client_Task_Change_Notify = 0x7F12,
	GS_Client_Exp_Change_Notify = 0x7F13,
	GS_Client_Level_Change_Notify = 0x7F14,
	GS_Client_Force_Move_Notify = 0x7F15,
	GS_Client_Item_Desp_Notify = 0x7F17,
	GS_Client_Guild_Info_Notify = 0x7F18,
	GS_Client_Ghost_Guild_Info = 0x7F19,
	GS_Client_GhostMode_Notify = 0x7F20,
	GS_Client_Alert_Notify = 0x7F21,
	GS_Client_Relive_Notify = 0x7F22,
	GS_Client_Group_Info_Change_Notify = 0x7F23,
	GS_Client_Group_State_Notify = 0x7F24,
	GS_Client_Group_Info_Notify = 0x7F25,
	GS_Client_Skill_Desp_Notify = 0x7F26,
	GS_Client_YouKe_SessionID_Notify = 0x7F27,
	GS_Client_Map_Conn_Notify = 0x7F28,
	GS_Client_Map_Safe_Area_Notify = 0x7F29,
	GS_Client_Npc_Show_Flags_Notify = 0x7F30,
	GS_Client_Join_Group_To_Leader_Notify = 0x7F31,
	GS_Client_Invite_Group_To_Member_Notify = 0x7F32,
	GS_Client_Find_Road_Goto_Notify = 0x7F33,
	GS_Client_Group_Chat_Notify = 0x7F34,
	GS_Client_Normal_Chat_Notify = 0x7F35,
	GS_Client_Trade_Invite_Notify = 0x7F36,
	GS_Client_Trade_Info_Notify = 0x7F37,
	GS_Client_Trade_Item_Change_Notify = 0x7F38,
	GS_Client_Map_Item_Owner_Notify = 0x7F39,
	GS_Client_PK_State_Notify = 0x7F40,
	GS_Client_Monster_Add_Info_Notify = 0x7F41,
	GS_Client_Status_Change_Notify = 0x7F42,
	GS_Client_Item_Talk_Notify = 0x7F43,
	GS_Client_Player_Add_Info_Notify = 0x7F44,
	GS_Client_Count_Down_Notify = 0x7F45,
	GS_Client_Mini_Map_Conn_Notify = 0x7F46,
	GS_Client_Play_Effect_Notify = 0x7F47,
	GS_Client_Game_Param_Notify = 0x7F48,
	GS_Client_Info_Item_Change_Notify = 0x7F49,
	GS_Client_World_Chat_Notify = 0x7F50,
	GS_Client_Session_Closed = 0x7F51,
	GS_Client_Session_Delay_Reauth = 0x7F52,
	GS_Client_WarInfo = 0x7F53,
	GS_Client_Map_Option = 0x7F54,
	GS_Client_Guild_Condition = 0x7F55,
	GS_Client_SlotAdd = 0x7F56,
	GS_Client_NameAdd_Notify = 0x7F57,
	GS_Client_URL_Notify = 0x7F58,
	GS_Client_Free_Relive_Level = 0x7F59,

	GS_Client_Set_Model_Notify = 0x7F61,
	GS_Client_China_Limit_Level = 0x7F62,
	GS_Client_Monster_Chat_Notify = 0x7F63,
	GS_Client_Map_Mini_Npc_Notify = 0x7F64,
	GS_Client_Offline_Exp_Info = 0x7F65,
	GS_Client_Team_Info_Notify = 0x7F66,
	GS_Client_Player_Talk_Notify = 0x7F67,
	GS_Client_Prosperity_Change_Notify = 0x7F68,
	GS_Client_GUI_Open_Panel_Notify = 0x7F69,
	GS_Client_Black_Board_Notify = 0x7F70,
	GS_Client_List_Guild_Begin = 0x7F71,
	GS_Client_List_Guild_End = 0x7F72,
	GS_Client_List_Guild_Item = 0x7F73,
    GS_Client_Item_Plus_Desp_Notify = 0x7F74,

	GS_Client_ListTalk_List_Notify = 0x7F77,
	GS_Client_ListTalk_Content_Notify = 0x7F78,
	GS_Client_ListTalk_Title_Notify = 0x7F79,
	GS_Client_Achieve_Done_Notify = 0x7F80,
	GS_Client_PK_Confirm_Notify = 0x7F81,

	GS_Client_Default_Skill_Notify = 0x7F86,

	GS_Client_Free_Direct_Fly_Notify = 0x7F88,
	GS_Client_Goto_End_Notify = 0x7F89,
	GS_Client_Status_HPMP_Change_Notify = 0x7F90,

	GS_Client_SlaveState_Notify = 0x7F92,
	GS_Client_Capacity_Change_Notify = 0x7F93,

	GS_Client_Total_Attr_Param_Notify = 0x7F95,
	GS_Client_Gift_List_Notify = 0x7F96,

	GS_Client_Collect_Break_Notify=0x7F99,

	GS_Client_Attack_Miss_Notify = 0x7FA2,
	GS_Client_Login_Item_List_Notify = 0x7FA3,
	GS_Client_Param_Data_Notify = 0x7FA4,
	GS_Client_speed_notify=0x7FA5,
	GS_Client_Panel_Info_notify=0x7FA6,
	GS_Client_Do_Action_notify=0x7FA7,

	GS_Client_Show_ProgressBar_Notify=0x7FA9,
	GS_Client_ScriptItem_Change_Notify = 0x7FAA,
	GS_Client_ScriptItem_InfoChange_Notify = 0x7FAB,
	GS_Client_Fresh_Panel_Info_Notify = 0x7FAC,
	GS_Client_Tili_Change=0x7FAD,
	GS_Client_Status_Desp_Notify=0x7FAE,
	GS_Client_Buff_Desp_Notify=0x7FB1,
	GS_Client_Buff_Change_Notify=0x7FB2,
	GS_Client_List_Item_Desp_Notify = 0x7FB3,
	GS_Client_List_Item_Change_Notify = 0x7FB4,
	GS_Client_List_Upgrade_Desp_Notify = 0x7FB5,
	GS_Client_Mon_Exp_Hiter_Change_Notify = 0x7FB6,
	GS_Client_List_Buff_Notify = 0x7FB7,

	GS_Client_Map_Mon_Gen_Notify = 0x7FC1,

	GS_Client_Param_Data_List_Notify = 0x7FF1,
	GS_Client_List_Status_Notify = 0x7FF2,
    GS_Client_List_ChargeDart_Notify = 0x7FF3,
    GS_Client_Item_Plus_Desp_Notify_Group = 0x7FF4,

};

#define MESSAGE_CLASS_BEGIN(class_name,message_id) \
	class class_name \
	{\
	public:\
		static const unsigned short MSG_ID = message_id;\


#define MESSAGE_CLASS_INIT(class_name) \
	public:\
		class_name()\
		{\


#define MESSAGE_CLASS_INIT_END\
			}\


#define MESSAGE_CLASS_END \
	};\


#define MESSAGE_CLASS_IMPORT(class_name)\
	template<class T> inline T& operator<<(T& stream,const class_name & data)\
	{\
		stream<<data.MSG_ID;\

#define MESSAGE_CLASS_IMPORT_END\
		return stream;\
	}\


#define MESSAGE_CLASS_EXPORT(class_name)\
	template<class T> inline T& operator>>(T& stream,class_name & data)\
	{\
		unsigned short msg_id=0;\
		stream>>msg_id;\
		if( msg_id != data.MSG_ID )\
		{\
			stream<<LEUD::Stream::FailFlag;\
			return stream;\
		}\


#define MESSAGE_CLASS_EXPORT_END\
		return stream;\
	}\


namespace Client_GS
{
	class AuthenticateREQ
	{
	public:
		static const unsigned short MSG_ID  = Client_GS_Authenticate;
		int AuthType;
		char mSessionID[512];
		int AuthSeed;
		int PartformID;
		char mIdfa[512];
	public:
		AuthenticateREQ()
		{
			mSessionID[0]=0;
			AuthType = 0;
			AuthSeed = 0;
			PartformID = 0;
			mIdfa[0]=0;
		}
	};
	template<class T> inline T& operator<<(T& stream,const AuthenticateREQ& data)
	{
		stream<<data.MSG_ID<<data.AuthType<<data.mSessionID<<data.AuthSeed<<data.PartformID<<data.mIdfa;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,AuthenticateREQ& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.AuthType>>data.mSessionID>>data.AuthSeed>>data.PartformID>>data.mIdfa;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuaFuSetParamReq,KuaFu_GS_Set_Param)
		std::string mParamKey;
		std::string mParamValue;
	MESSAGE_CLASS_INIT(KuaFuSetParamReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(KuaFuSetParamReq)
		stream<<data.mParamKey<<data.mParamValue;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuaFuSetParamReq)
		stream>>data.mParamKey>>data.mParamValue;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ClientGSKuafuAuth,Client_GS_Kuafu_Auth)
		std::string ticket;
		std::string loginid;
		std::string charname;
		std::string ip;
		std::string port;
		std::string localip;
		std::string localport;
		std::string param;
		int ticket_seed;
		std::string localPTID;
		std::string localServerID;
		std::string localArea;
	MESSAGE_CLASS_INIT(ClientGSKuafuAuth)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ClientGSKuafuAuth)
		stream<<data.ticket<<data.loginid<<data.charname<<data.ip<<data.port<<data.localip<<data.localport<<data.param<<data.ticket_seed<<data.localPTID<<data.localServerID<<data.localArea;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ClientGSKuafuAuth)
		stream>>data.ticket>>data.loginid>>data.charname>>data.ip>>data.port>>data.localip>>data.localport>>data.param>>data.ticket_seed>>data.localPTID>>data.localServerID>>data.localArea;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuaFuGetCharDataReq,KuaFu_GS_Get_Char_Data)
		std::string mTicket;	
		std::string mLoginID;
		std::string mCharID;
		std::string mSeedName;
	MESSAGE_CLASS_INIT(KuaFuGetCharDataReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(KuaFuGetCharDataReq)
		stream<<data.mTicket<<data.mLoginID<<data.mCharID<<data.mSeedName;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuaFuGetCharDataReq)
		stream>>data.mTicket>>data.mLoginID>>data.mCharID>>data.mSeedName;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuaFuPlayerDataReq,KuaFu_GS_Player_Data)
		std::string charID;
		std::string seekName;
		std::string pkType;
		std::string value;
	MESSAGE_CLASS_INIT(KuaFuPlayerDataReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(KuaFuPlayerDataReq)
		stream<<data.charID<<data.seekName<<data.pkType<<data.value;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuaFuPlayerDataReq)
		stream>>data.charID>>data.seekName>>data.pkType>>data.value;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuaFuComeBackDataReq,KuaFu_GS_Comeback_Data)
		std::string winArea;
		std::string winGuild;
		std::string charID;
		std::string seekName;
		std::string account;
		int vcoin;
		int result;
	MESSAGE_CLASS_INIT(KuaFuComeBackDataReq)
		vcoin = 0;
		result=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(KuaFuComeBackDataReq)
		stream<<data.winArea<<data.winGuild<<data.charID<<data.seekName<<data.account<<data.vcoin<<data.result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuaFuComeBackDataReq)
		stream>>data.winArea>>data.winGuild>>data.charID>>data.seekName>>data.account>>data.vcoin>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapChatReq,Client_GS_Map_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(MapChatReq)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapChatReq)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapChatReq)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(WorldChatReq,Client_GS_World_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(WorldChatReq)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(WorldChatReq)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(WorldChatReq)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GuildChatReq,Client_GS_Guild_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(GuildChatReq)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildChatReq)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GuildChatReq)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PrivateChatReq,Client_GS_Private_Chat)
		char charID[512];
		char msg[512];
	MESSAGE_CLASS_INIT(PrivateChatReq)
		charID[0]=0;
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PrivateChatReq)
		stream<<data.charID<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PrivateChatReq)
		stream>>data.charID>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(WalkReq,Client_GS_Walk)
		int dir;
		int newx;
		int newy;
		int tag;
		int timetag;
	MESSAGE_CLASS_INIT(WalkReq)
		dir = 0;
		newx = 0;
		newy = 0;
		tag=0;
		timetag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(WalkReq)
		stream<<data.newy<<data.newx<<data.timetag<<data.dir<<data.tag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(WalkReq)
		stream>>data.newy>>data.newx>>data.timetag>>data.dir>>data.tag;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(RunReq,Client_GS_Run)
		int dir;
		int newx;
		int newy;
		int tag;
		int timetag;
	MESSAGE_CLASS_INIT(RunReq)
		dir = 0;
		newx = 0;
		newy = 0;
		tag = 0;
		timetag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(RunReq)
		stream<<data.tag<<data.dir<<data.newy<<data.newx<<data.timetag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(RunReq)
		stream>>data.tag>>data.dir>>data.newy>>data.newx>>data.timetag;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NPCTalkReq,Client_GS_NPCTalk)
		unsigned int npcid;
		char param[512];
	MESSAGE_CLASS_INIT(NPCTalkReq)
		npcid = 0;
		param[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NPCTalkReq)
		stream<<data.npcid<<data.param;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NPCTalkReq)
		stream>>data.npcid>>data.param;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TurnReq,Client_GS_Turn)
		int dir;
	MESSAGE_CLASS_INIT(TurnReq)
		dir = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TurnReq)
		stream<<data.dir;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TurnReq)
		stream>>data.dir;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AttackReq,Client_GS_Attack)
		int param;
	MESSAGE_CLASS_INIT(AttackReq)
		param = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AttackReq)
		stream<<data.param;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AttackReq)
		stream>>data.param;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PickUpReq,Client_GS_PickUp)
		unsigned int itemid;
		int x;
		int y;
	MESSAGE_CLASS_INIT(PickUpReq)
		itemid = 0;
		x = 0;
		y = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PickUpReq)
		stream<<data.itemid<<data.x<<data.y;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PickUpReq)
		stream>>data.itemid>>data.x>>data.y;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(BagUseItemReq,Client_GS_BagUseItem)
		int position;
		int type_id;
		int sendtag;
		int num;
	MESSAGE_CLASS_INIT(BagUseItemReq)
		position = 0;
		type_id = 0;
		sendtag = 0;
		num = 1;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(BagUseItemReq)
		stream<<data.position<<data.type_id<<data.num<<data.sendtag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(BagUseItemReq)
		stream>>data.position>>data.type_id>>data.num>>data.sendtag;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(UndressItemReq,Client_GS_UndressItem)
		int position;
	MESSAGE_CLASS_INIT(UndressItemReq)
		position = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(UndressItemReq)
		stream<<data.position;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(UndressItemReq)
		stream>>data.position;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ItemPostionExchangeReq,Client_GS_ItemPositionExchange)
		int from;
		int to;
		int flag;
	MESSAGE_CLASS_INIT(ItemPostionExchangeReq)
		from = 0;
		to = 0;
		flag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ItemPostionExchangeReq)
		stream<<data.from<<data.to<<data.flag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ItemPostionExchangeReq)
		stream>>data.from>>data.to>>data.flag;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(UseSkillReq,Client_GS_UseSkill)
		int skill_type;
		int paramX;
		int paramY;
		unsigned int paramID;
		int sendtag;
		int timetag;
	MESSAGE_CLASS_INIT(UseSkillReq)
		skill_type = 0;
		paramX = 0;
		paramY = 0;
		paramID = 0;
		sendtag = 0;
		timetag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(UseSkillReq)
		stream<<data.skill_type<<data.paramX<<data.paramY<<data.paramID<<data.sendtag<<data.timetag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(UseSkillReq)
		stream>>data.skill_type>>data.paramX>>data.paramY>>data.paramID>>data.sendtag>>data.timetag;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NpcShopReq,Client_GS_NPCShop)
		unsigned int npc_id;
		unsigned int page;
	MESSAGE_CLASS_INIT(NpcShopReq)
		npc_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NpcShopReq)
		stream<<data.npc_id<<data.page;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NpcShopReq)
		stream>>data.npc_id>>data.page;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NpcBuyReq,Client_GS_NPCBuy)
		unsigned int npc_id;
		int page;
		int pos;
		int good_id;
		int type_id;
		int number;
	MESSAGE_CLASS_INIT(NpcBuyReq)
		npc_id = 0;
		page = 0;
		pos = 0;
		good_id = 0;
		type_id = 0;
		number = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NpcBuyReq)
		stream<<data.npc_id<<data.page<<data.pos<<data.good_id<<data.type_id<<data.number;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NpcBuyReq)
		stream>>data.npc_id>>data.page>>data.pos>>data.good_id>>data.type_id>>data.number;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TaskCancelReq,Client_GS_CancelTask)
		int task_id;
	MESSAGE_CLASS_INIT(TaskCancelReq)
		task_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TaskCancelReq)
		stream<<data.task_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TaskCancelReq)
		stream>>data.task_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ForceMoveReq,Client_GS_ForceMove)
		int newx;
		int newy;
		int newdir;
	MESSAGE_CLASS_INIT(ForceMoveReq)
		newx = 0;
		newy = 0;
		newdir = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ForceMoveReq)
		stream<<data.newx<<data.newy<<data.newdir;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ForceMoveReq)
		stream>>data.newx>>data.newy>>data.newdir;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ChangeClothReq,Client_GS_ChangeCloth)
		int cloth;
		int weapon;
		int hair;
	MESSAGE_CLASS_INIT(ChangeClothReq)
		cloth = 0;
		weapon = 0;
		hair = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChangeClothReq)
		stream<<data.cloth<<data.weapon<<data.hair;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChangeClothReq)
		stream>>data.cloth>>data.weapon>>data.hair;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListCharacterReq,Client_GS_ListCharacter)
		int flags;
	MESSAGE_CLASS_INIT(ListCharacterReq)
		flags=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListCharacterReq)
		stream<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListCharacterReq)
		stream>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CreateCharacterReq,Client_GS_CreateCharacter)
		char name[512];
		int job;
		int gender;
		int svrid;
		char youkesession[512];
	MESSAGE_CLASS_INIT(CreateCharacterReq)
		job=0;
		gender=0;
		svrid=0;
		name[0]=0;
		youkesession[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CreateCharacterReq)
		stream<<data.name<<data.job<<data.gender<<data.svrid<<data.youkesession;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CreateCharacterReq)
		stream>>data.name>>data.job>>data.gender>>data.svrid>>data.youkesession;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(EnterGameReq,Client_GS_EnterGame)
		char charname[512];
	    char seedname[512];
	MESSAGE_CLASS_INIT(EnterGameReq)
		charname[0]=0;
	    seedname[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(EnterGameReq)
		stream<<data.charname<<data.seedname;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(EnterGameReq)
		stream>>data.charname>>data.seedname;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DeleteCharacterReq,Client_GS_DeleteCharacter)
		char charname[512];
	MESSAGE_CLASS_INIT(DeleteCharacterReq)
		charname[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DeleteCharacterReq)
		stream<<data.charname;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DeleteCharacterReq)
		stream>>data.charname;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NPCSellReq,Client_GS_NPCSell)
		unsigned int npc_id;
		int pos;
		int type_id;
		int number;
		int flag;
	MESSAGE_CLASS_INIT(NPCSellReq)
		npc_id = 0;
		pos = 0;
		type_id = 0;
		number = 0;
		flag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NPCSellReq)
		stream<<data.npc_id<<data.pos<<data.type_id<<data.number<<data.flag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NPCSellReq)
		stream>>data.npc_id>>data.pos>>data.type_id>>data.number>>data.flag;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DropItemReq,Client_GS_DropItem)
		int pos;
		int type_id;
		int number;
	MESSAGE_CLASS_INIT(DropItemReq)
		pos = 0;
		type_id = 0;
		number = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DropItemReq)
		stream<<data.pos<<data.type_id<<data.number;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DropItemReq)
		stream>>data.pos>>data.type_id>>data.number;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildReq,Client_GS_ListGuild)
		int flags;
	MESSAGE_CLASS_INIT(ListGuildReq)
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildReq)
		stream<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildReq)
		stream>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GetGuildInfoReq,Client_GS_GetGuildInfo)
		char guild_name[512];
		int flags;
	MESSAGE_CLASS_INIT(GetGuildInfoReq)
		flags = 0;
		guild_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GetGuildInfoReq)
		stream<<data.guild_name<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GetGuildInfoReq)
		stream>>data.guild_name>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SaveShortcutReq,Client_GS_SaveShortcut)
		struct shortcut
		{
			shortcut()
			{
				shortcut_id = 0;
				type = 0;
				param = 0;
			}
			int shortcut_id;
			int type;
			int param;
		};
		std::vector<shortcut> shortcuts;
	MESSAGE_CLASS_INIT(SaveShortcutReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SaveShortcutReq)
		stream << static_cast<int>(data.shortcuts.size());
		for( std::vector<SaveShortcutReq::shortcut>::const_iterator pos = data.shortcuts.begin();
			pos != data.shortcuts.end();
			++ pos )
		{
			stream<<pos->shortcut_id<<pos->type<<pos->param;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SaveShortcutReq)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			if(i > 1000){printf("SaveShortcutReq for size beyond stream :%d\n",size);break;}
			SaveShortcutReq::shortcut sc;
			stream>>sc.shortcut_id>>sc.type>>sc.param;
			if(stream)
			{
				data.shortcuts.push_back(sc);
			}
			else
			{
				printf("SaveShortcutReq for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CreateGuildReq,Client_GS_CreateGuild)
		char guild_name[512];
		int flags;
	MESSAGE_CLASS_INIT(CreateGuildReq)
		guild_name[0]=0;
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CreateGuildReq)
		stream<<data.guild_name<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CreateGuildReq)
		stream>>data.guild_name>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(JoinGuildReq,Client_GS_JoinGuild)
		char guild_name[512];
		int flags;
	MESSAGE_CLASS_INIT(JoinGuildReq)
		guild_name[0]=0;
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(JoinGuildReq)
		stream<<data.guild_name<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(JoinGuildReq)
		stream>>data.guild_name>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SetGuildInfoReq,Client_GS_SetGuildInfo)
		char guild_name[512];
		char desp[512];
		char notice[512];
	MESSAGE_CLASS_INIT(SetGuildInfoReq)
		guild_name[0]=0;
		desp[0]=0;
		notice[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SetGuildInfoReq)
		stream<<data.guild_name<<data.desp<<data.notice;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SetGuildInfoReq)
		stream>>data.guild_name>>data.desp>>data.notice;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildMemberReq,Client_GS_ListGuildMember)
		char guild_name[512];
		int list_type;
	MESSAGE_CLASS_INIT(ListGuildMemberReq)
		guild_name[0]=0;
		list_type = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildMemberReq)
		stream<<data.guild_name<<data.list_type;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildMemberReq)
		stream>>data.guild_name>>data.list_type;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildEnemyReq,Client_GS_ListGuildEnemy)
		char guild_name[512];
	MESSAGE_CLASS_INIT(ListGuildEnemyReq)
		guild_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildEnemyReq)
		stream<<data.guild_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildEnemyReq)
		stream>>data.guild_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildFriendReq,Client_GS_ListGuildFriend)
		char guild_name[512];
	MESSAGE_CLASS_INIT(ListGuildFriendReq)
		guild_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildFriendReq)
		stream<<data.guild_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildFriendReq)
		stream>>data.guild_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildItemReq,Client_GS_ListGuildItem)
		char guild_name[512];
	MESSAGE_CLASS_INIT(ListGuildItemReq)
		guild_name[0]=0;
	MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(ListGuildItemReq)
		stream<<data.guild_name;
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(ListGuildItemReq)
		stream>>data.guild_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ChangeGuildMemberTitleReq,Client_GS_ChangeGuildMemberTitle)
		char guild_name[512];
		char nick_name[512];
		int dir;
	MESSAGE_CLASS_INIT(ChangeGuildMemberTitleReq)
		guild_name[0]=0;
		nick_name[0]=0;
		dir = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChangeGuildMemberTitleReq)
		stream<<data.guild_name<<data.nick_name<<data.dir;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChangeGuildMemberTitleReq)
		stream>>data.guild_name>>data.nick_name>>data.dir;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ChangeEnemyGuildReq,Client_GS_ChangeEnemyGuild)
		char guild_name[512];
		char enemy_guild_name[512];
		int action;
	MESSAGE_CLASS_INIT(ChangeEnemyGuildReq)
		guild_name[0]=0;
		enemy_guild_name[0]=0;
		action = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChangeEnemyGuildReq)
		stream<<data.guild_name<<data.enemy_guild_name<<data.action;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChangeEnemyGuildReq)
		stream>>data.guild_name>>data.enemy_guild_name>>data.action;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ChangeFriendGuildReq,Client_GS_ChangeFriendGuild)
		char guild_name[512];
		char friend_guild_name[512];
		int action;
	MESSAGE_CLASS_INIT(ChangeFriendGuildReq)
		guild_name[0]=0;
		friend_guild_name[0]=0;
		action = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChangeFriendGuildReq)
		stream<<data.guild_name<<data.friend_guild_name<<data.action;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChangeFriendGuildReq)
		stream>>data.guild_name>>data.friend_guild_name>>data.action;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ChangeAttackModeReq,Client_GS_ChangeAttackMode)
		int attack_mode;
	MESSAGE_CLASS_INIT(ChangeAttackModeReq)
		attack_mode = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChangeAttackModeReq)
		stream<<data.attack_mode;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChangeAttackModeReq)
		stream>>data.attack_mode;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(VcoinShopListReq,Client_GS_VcoinShopList)
		int shop_id;
		int flags;
	MESSAGE_CLASS_INIT(VcoinShopListReq)
		shop_id = 0;
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(VcoinShopListReq)
		stream<<data.shop_id<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(VcoinShopListReq)
		stream>>data.shop_id>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NPCRepairReq,Client_GS_NPCRepair)
		unsigned int npc_id;
		int pos;
		int type_id;
		int flags;
	MESSAGE_CLASS_INIT(NPCRepairReq)
		pos = 0;
		type_id = 0;
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NPCRepairReq)
		stream<<data.npc_id<<data.pos<<data.type_id<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NPCRepairReq)
		stream>>data.npc_id>>data.pos>>data.type_id>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ReliveReq,Client_GS_Relive)
		int relive_type;
	MESSAGE_CLASS_INIT(ReliveReq)
		relive_type = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ReliveReq)
		stream<<data.relive_type;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ReliveReq)
		stream>>data.relive_type;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TaskDespReq,Client_GS_Task_Desp)
		int task_id;
	MESSAGE_CLASS_INIT(TaskDespReq)
		task_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TaskDespReq)
		stream<<data.task_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TaskDespReq)
		stream>>data.task_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(InfoPlayerReq,Client_GS_Info_Player)
		char player_name[512];
	MESSAGE_CLASS_INIT(InfoPlayerReq)
		player_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(InfoPlayerReq)
		stream<<data.player_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(InfoPlayerReq)
		stream>>data.player_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CreateGroupReq,Client_GS_Create_Group)
		int flags;
	MESSAGE_CLASS_INIT(CreateGroupReq)
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CreateGroupReq)
		stream<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CreateGroupReq)
		stream>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(LeaveGroupReq,Client_GS_Leave_Group)
	MESSAGE_CLASS_INIT(LeaveGroupReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(LeaveGroupReq)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(LeaveGroupReq)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(JoinGroupReq,Client_GS_Join_Group)
		int group_id;
	MESSAGE_CLASS_INIT(JoinGroupReq)
		group_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(JoinGroupReq)
		stream<<data.group_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(JoinGroupReq)
		stream>>data.group_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AgreeJoinGroupReq,Client_GS_Agree_Join_Group)
		char player_name[512];
	MESSAGE_CLASS_INIT(AgreeJoinGroupReq)
		player_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AgreeJoinGroupReq)
		stream<<data.player_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AgreeJoinGroupReq)
		stream>>data.player_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(InviteGroupReq,Client_GS_Invite_Group)
		char player_name[512];
	MESSAGE_CLASS_INIT(InviteGroupReq)
		player_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(InviteGroupReq)
		stream<<data.player_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(InviteGroupReq)
		stream>>data.player_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AgreeInviteGroupReq,Client_GS_Agree_Invite_Group)
		char leader_name[512];
		int group_id;
	MESSAGE_CLASS_INIT(AgreeInviteGroupReq)
		leader_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AgreeInviteGroupReq)
		stream<<data.leader_name<<data.group_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AgreeInviteGroupReq)
		stream>>data.leader_name>>data.group_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TaskClickReq,Client_GS_Task_Click)
		int task_id;
		char param[512];
	MESSAGE_CLASS_INIT(TaskClickReq)
		param[0]=0;
		task_id=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TaskClickReq)
		stream<<data.task_id<<data.param;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TaskClickReq)
		stream>>data.task_id>>data.param;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupChatReq,Client_GS_Group_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(GroupChatReq)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupChatReq)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupChatReq)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NormalChatReq,Client_GS_Normal_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(NormalChatReq)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NormalChatReq)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NormalChatReq)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(HornChatReq,Client_GS_Horn_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(HornChatReq)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(HornChatReq)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(HornChatReq)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeInviteReq,Client_GS_Trade_Invite)
		char invitee[512];
	MESSAGE_CLASS_INIT(TradeInviteReq)
		invitee[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeInviteReq)
		stream<<data.invitee;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeInviteReq)
		stream>>data.invitee;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AgreeTradeInviteReq,Client_GS_Agree_Trade_Invite)
		char inviter[512];
	MESSAGE_CLASS_INIT(AgreeTradeInviteReq)
		inviter[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AgreeTradeInviteReq)
		stream<<data.inviter;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AgreeTradeInviteReq)
		stream>>data.inviter;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CloseTradeReq,Client_GS_Close_Trade)
		char target[512];
	MESSAGE_CLASS_INIT(CloseTradeReq)
		target[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CloseTradeReq)
		stream<<data.target;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CloseTradeReq)
		stream>>data.target;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeAddGameMoneyReq,Client_GS_Trade_Add_GameMoney)
		int value;
	MESSAGE_CLASS_INIT(TradeAddGameMoneyReq)
		value = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeAddGameMoneyReq)
		stream<<data.value;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeAddGameMoneyReq)
		stream>>data.value;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeAddVcoinReq,Client_GS_Trade_Add_Vcoin)
		int value;
	MESSAGE_CLASS_INIT(TradeAddVcoinReq)
		value = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeAddVcoinReq)
		stream<<data.value;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeAddVcoinReq)
		stream>>data.value;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeSubmitReq,Client_GS_Trade_Submit)
	MESSAGE_CLASS_INIT(TradeSubmitReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeSubmitReq)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeSubmitReq)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupSetLeaderReq,Client_GS_Group_Set_Leader)
		char player_name[512];
	MESSAGE_CLASS_INIT(GroupSetLeaderReq)
		player_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupSetLeaderReq)
		stream<<data.player_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupSetLeaderReq)
		stream>>data.player_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeAddItemReq,Client_GS_Trade_Add_Item)
		int pos;
		int item_type;
		int price;
		int flag;
	MESSAGE_CLASS_INIT(TradeAddItemReq)
		pos = 0;
		item_type = 0;
		price = 0;
		flag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeAddItemReq)
		stream<<data.pos<<data.item_type<<data.price<<data.flag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeAddItemReq)
		stream>>data.pos>>data.item_type>>data.price>>data.flag;        
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
MESSAGE_CLASS_BEGIN(ChatTradeSubmitReq,Client_GS_Chat_Trade_Submit)
	std::string chrname;
	int pos;
	int item_type;
	int lock;
	int flag;
	MESSAGE_CLASS_INIT(ChatTradeSubmitReq)
	pos = 0;
	item_type = 0;
	lock = 0;
	flag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChatTradeSubmitReq)
	stream<<data.chrname<<data.pos<<data.item_type<<data.lock<<data.flag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChatTradeSubmitReq)
	stream>>data.chrname>>data.pos>>data.item_type>>data.lock>>data.flag;       
	MESSAGE_CLASS_EXPORT_END
	///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DestoryItemReq,Client_GS_Destory_Item)
		int pos;
		int item_type;
	MESSAGE_CLASS_INIT(DestoryItemReq)
		pos = 0;
		item_type = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DestoryItemReq)
		stream<<data.pos<<data.item_type;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DestoryItemReq)
		stream>>data.pos>>data.item_type;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SortItemReq,Client_GS_Sort_Item)
		int flags;
	MESSAGE_CLASS_INIT(SortItemReq)
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SortItemReq)
		stream<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SortItemReq)
		stream>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ItemTalkReq,Client_GS_Item_Talk)
		int item_id;
		int seed;
		char param[512];
	MESSAGE_CLASS_INIT(ItemTalkReq)
		param[0]=0;
		item_id = 0;
		seed = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ItemTalkReq)
		stream<<data.item_id<<data.seed<<data.param;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ItemTalkReq)
		stream>>data.item_id>>data.seed>>data.param;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PlayerTalkReq,Client_GS_Player_Talk)
		int seed;
		char param[512];
	MESSAGE_CLASS_INIT(PlayerTalkReq)
		seed = 0;
		param[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PlayerTalkReq)
		stream<<data.seed<<data.param;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PlayerTalkReq)
		stream>>data.seed>>data.param;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(MergeSteelReq,Client_GS_Merge_Steel)
//		int item_pos1;
//		int item_pos2;
//		int item_pos3;
//		int item_posadd;
//		int pay_type;
//	MESSAGE_CLASS_INIT(MergeSteelReq)
//		item_pos1 = 0;
//		item_pos2 = 0;
//		item_pos3 = 0;
//		item_posadd = 0;
//		pay_type = 0;
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(MergeSteelReq)
//		stream<<data.item_pos1<<data.item_pos2<<data.item_pos3<<data.item_posadd<<data.pay_type;
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(MergeSteelReq)
//		stream>>data.item_pos1>>data.item_pos2>>data.item_pos3>>data.item_posadd>>data.pay_type;
//	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(UpgradeEquipReq,Client_GS_Upgrade_Equip)
//		int item_posEquip;
//		int item_posSteel;
//		int item_posAdd;
//		int pay_type;
//	MESSAGE_CLASS_INIT(UpgradeEquipReq)
//		item_posEquip = 0;
//		item_posSteel = 0;
//		item_posAdd = 0;
//		pay_type = 0;
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(UpgradeEquipReq)
//		stream<<data.item_posEquip<<data.item_posSteel<<data.item_posAdd<<data.pay_type;
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(UpgradeEquipReq)
//		stream>>data.item_posEquip>>data.item_posSteel>>data.item_posAdd>>data.pay_type;
//	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FreshVcoinReq,Client_GS_Fresh_Vcoin)
	MESSAGE_CLASS_INIT(FreshVcoinReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FreshVcoinReq)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FreshVcoinReq)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(LeaveGuildReq,Client_GS_Leave_Guild)
		char guild_name[512];
	MESSAGE_CLASS_INIT(LeaveGuildReq)
		guild_name[0] = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(LeaveGuildReq)
	stream<<data.guild_name;
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(LeaveGuildReq)
		stream>>data.guild_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AddDepotSlotReq,Client_GS_Add_Depot_Slot)
	MESSAGE_CLASS_INIT(AddDepotSlotReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AddDepotSlotReq)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AddDepotSlotReq)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupPickModeReq,Client_GS_Group_Pick_Mode)
	MESSAGE_CLASS_INIT(GroupPickModeReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupPickModeReq)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupPickModeReq)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(UpdateChinaLimitReq,Client_GS_UpdateChinaLimit)
	//	char china_id[512];
	//	char china_name[512];
	//	int china_id_ok;
	//MESSAGE_CLASS_INIT(UpdateChinaLimitReq)
	//	china_id[0]=0;
	//	china_name[0]=0;
	//	china_id_ok = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(UpdateChinaLimitReq)
	//	stream<<data.china_id<<data.china_name<<data.china_id_ok;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(UpdateChinaLimitReq)
	//	stream>>data.china_id>>data.china_name>>data.china_id_ok;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(BuyOfflineExpReq,Client_GS_BuyOfflineExp)
	//	int mul;
	//MESSAGE_CLASS_INIT(BuyOfflineExpReq)
	//	mul = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(BuyOfflineExpReq)
	//	stream<<data.mul;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(BuyOfflineExpReq)
	//	stream>>data.mul;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(SteelEquipReq,Client_GS_SteelEquip)
	//	int pos;
	//	int type_id;
	//MESSAGE_CLASS_INIT(SteelEquipReq)
	//	pos = 0;
	//	type_id = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(SteelEquipReq)
	//	stream<<data.pos<<data.type_id;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(SteelEquipReq)
	//	stream>>data.pos>>data.type_id;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FreshHPMPReq,Client_GS_FreshHPMP)
	MESSAGE_CLASS_INIT(FreshHPMPReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FreshHPMPReq)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FreshHPMPReq)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PingReq,Client_GS_Ping)
	MESSAGE_CLASS_INIT(PingReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PingReq)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PingReq)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GetChartInfoReq,Client_GS_Get_Chart_Info)
		int chart_type;
		int page;
	MESSAGE_CLASS_INIT(GetChartInfoReq)
		chart_type = 0;
		page = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GetChartInfoReq)
		stream<<data.chart_type<<data.page;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GetChartInfoReq)
		stream>>data.chart_type>>data.page;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(InfoItemExchangeReq,Client_GS_Info_Item_Exchange)
	//	int pos;
	//	int type_id;
	//	int exchange_type;
	//MESSAGE_CLASS_INIT(InfoItemExchangeReq)
	//	pos = 0;
	//	type_id = 0;
	//	exchange_type = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(InfoItemExchangeReq)
	//	stream<<data.pos<<data.type_id<<data.exchange_type;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(InfoItemExchangeReq)
	//	stream>>data.pos>>data.type_id>>data.exchange_type;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(ItemExchangeReq,Client_GS_Item_Exchange)
	//	int pos;
	//	int type_id;
	//	int exchange_type;
	//MESSAGE_CLASS_INIT(ItemExchangeReq)
	//	pos = 0;
	//	type_id = 0;
	//	exchange_type = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(ItemExchangeReq)
	//	stream<<data.pos<<data.type_id<<data.exchange_type;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(ItemExchangeReq)
	//	stream>>data.pos>>data.type_id>>data.exchange_type;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GetItemDespReq,Client_GS_Get_Item_Desp)
		int type_id;
		char type_name[512];
	MESSAGE_CLASS_INIT(GetItemDespReq)
		type_name[0]=0;
		type_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GetItemDespReq)
		stream<<data.type_id<<data.type_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GetItemDespReq)
		stream>>data.type_id>>data.type_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FriendChangeReq,Client_GS_Friend_Change)
		char name[512];
		int title;
	MESSAGE_CLASS_INIT(FriendChangeReq)
		name[0]=0;
		title = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FriendChangeReq)
		stream<<data.name<<data.title;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FriendChangeReq)
		stream>>data.name>>data.title;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FindMapGhostReq,Client_GS_Find_Map_Ghost)
		char map_id[512];
		char mon_name[512];
		short num;
		short type;
	MESSAGE_CLASS_INIT(FindMapGhostReq)
		map_id[0]=0;
		mon_name[0]=0;
		num = 0;
		type = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FindMapGhostReq)
		stream<<data.map_id<<data.num<<data.mon_name<<data.type;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FindMapGhostReq)
		stream>>data.map_id>>data.num>>data.mon_name>>data.type;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FriendApplyAgree,Client_GS_Friend_Apply_Agree)
		char name[512];
		int is_agree;
	MESSAGE_CLASS_INIT(FriendApplyAgree)
		name[0] = 0;
		is_agree = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FriendApplyAgree)
		stream<<data.name<<data.is_agree;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FriendApplyAgree)
	stream>>data.name>>data.is_agree;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(EquipReRandAddReq,Client_GS_Equip_ReRandAdd)
	//	int posEquip;
	//	int posEquipAdd;
	//MESSAGE_CLASS_INIT(EquipReRandAddReq)
	//	posEquip = 0;
	//	posEquipAdd = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(EquipReRandAddReq)
	//	stream<<data.posEquip<<data.posEquipAdd;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(EquipReRandAddReq)
	//	stream>>data.posEquip>>data.posEquipAdd;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(EquipExchangeUpgradeReq,Client_GS_Equip_Exchange_Upgrade)
	//	int posEquipFrom;
	//	int posEquipTo;
	//	int posEquipAdd;
	//	int paytype;
	//MESSAGE_CLASS_INIT(EquipExchangeUpgradeReq)
	//	posEquipFrom = 0;
	//	posEquipTo = 0;
	//	posEquipAdd = 0;
	//	paytype = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(EquipExchangeUpgradeReq)
	//	stream<<data.posEquipFrom<<data.posEquipTo<<data.posEquipAdd<<data.paytype;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(EquipExchangeUpgradeReq)
	//	stream>>data.posEquipFrom>>data.posEquipTo>>data.posEquipAdd>>data.paytype;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ServerScriptReq,Client_GS_Server_Script)
		char param[512];
	MESSAGE_CLASS_INIT(ServerScriptReq)
		param[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ServerScriptReq)
		stream<<data.param;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ServerScriptReq)
		stream>>data.param;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ProtectItemReq,Client_GS_Protect_Item)
		int pos;
		int type_id;
	MESSAGE_CLASS_INIT(ProtectItemReq)
		pos = 0;
		type_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ProtectItemReq)
		stream<<data.pos<<data.type_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ProtectItemReq)
		stream>>data.pos>>data.type_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupKickMemberReq,Client_GS_Group_Kick_Member)
		char member_name[512];
	MESSAGE_CLASS_INIT(GroupKickMemberReq)
		member_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupKickMemberReq)
		stream<<data.member_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupKickMemberReq)
		stream>>data.member_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(BoothAddReq,Client_GS_Booth_Add)
	//	int pos;
	//	int type_id;
	//	int pay_type;
	//	int pay_price;
	//MESSAGE_CLASS_INIT(BoothAddReq)
	//	pos=0;
	//	type_id=0;
	//	pay_type=0;
	//	pay_price=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(BoothAddReq)
	//	stream<<data.pos<<data.type_id<<data.pay_type<<data.pay_price;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(BoothAddReq)
	//	stream>>data.pos>>data.type_id>>data.pay_type>>data.pay_price;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(MergeEquipReq,Client_GS_MergeEquip)
	//	int target_id;
	//	int param_id1;
	//	int param_id2;
	//	int param_id3;
	//	int param_id4;
	//	int param_id5;
	//	int param_id6;
	//MESSAGE_CLASS_INIT(MergeEquipReq)
	//	target_id=-1;
	//	param_id1=-1;
	//	param_id2=-1;
	//	param_id3=-1;
	//	param_id4=-1;
	//	param_id5=-1;
	//	param_id6=-1;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(MergeEquipReq)
	//	stream<<data.target_id<<data.param_id1<<data.param_id2<<data.param_id3<<data.param_id4<<data.param_id5<<data.param_id6;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(MergeEquipReq)
	//	stream>>data.target_id>>data.param_id1>>data.param_id2>>data.param_id3>>data.param_id4>>data.param_id5>>data.param_id6;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(MarryInviteReq,Client_GS_Marry_Invite)
	//	char target_name[512];
	//	char male_desp[512];
	//MESSAGE_CLASS_INIT(MarryInviteReq)
	//	target_name[0]=0;
	//	male_desp[0]=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(MarryInviteReq)
	//	stream<<data.target_name<<data.male_desp;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(MarryInviteReq)
	//	stream>>data.target_name>>data.male_desp;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(AgreeOrNotMarryInviteReq,Client_GS_AgreeOrNot_Marry_Invite)
//		int agree;
//		char male_name[512];
//		char male_desp[512];
//		char female_name[512];
//		char female_desp[512];
//	MESSAGE_CLASS_INIT(AgreeOrNotMarryInviteReq)
//		agree = 0;
//		male_name[0]=0;
//		male_desp[0]=0;
//		female_name[0]=0;
//		female_desp[0]=0;
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(AgreeOrNotMarryInviteReq)
//		stream<<data.agree<<data.male_name<<data.male_desp<<data.female_name<<data.female_desp;
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(AgreeOrNotMarryInviteReq)
//		stream>>data.agree>>data.male_name>>data.male_desp>>data.female_name>>data.female_desp;
//	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(DivorceInviteReq,Client_GS_Divorce_Invite)
//		char target_name[512];
//	MESSAGE_CLASS_INIT(DivorceInviteReq)
//		target_name[0]=0;
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(DivorceInviteReq)
//		stream<<data.target_name;
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(DivorceInviteReq)
//		stream>>data.target_name;
//	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(AgreeOrNotDivorceInviteReq,Client_GS_AgreeOrNot_Divorce_Invite)
//		int agree;
//	MESSAGE_CLASS_INIT(AgreeOrNotDivorceInviteReq)
//		agree = 0;
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(AgreeOrNotDivorceInviteReq)
//		stream<<data.agree;
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(AgreeOrNotDivorceInviteReq)
//		stream>>data.agree;
//	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CollectStartReq,Client_GS_Collect_Start_Req)
		int srcid;
	MESSAGE_CLASS_INIT(CollectStartReq)
		srcid=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CollectStartReq)
		stream<<data.srcid;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CollectStartReq)
		stream>>data.srcid;
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TaskListReq,Client_GS_TaskList_Req)
		int flag;
	MESSAGE_CLASS_INIT(TaskListReq)
		flag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TaskListReq)
		stream<<data.flag;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TaskListReq)
		stream>>data.flag;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DirectFlyReq,Client_GS_Direct_Fly)
		int target_id;
	MESSAGE_CLASS_INIT(DirectFlyReq)
		target_id=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DirectFlyReq)
		stream<<data.target_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DirectFlyReq)
		stream>>data.target_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SplitItemReq,Client_GS_Split_Item)
	    int id;
	    int pos;
	    int number;
	MESSAGE_CLASS_INIT(SplitItemReq)
		id = 0;
	    pos = 0;
	    number = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SplitItemReq)
		stream<<data.id<<data.pos<<data.number;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SplitItemReq)
		stream>>data.id>>data.pos>>data.number;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PushLuaTableReq,Client_GS_PUSH_LUA_TABLE)
		std::string type;
		std::string table;
	MESSAGE_CLASS_INIT(PushLuaTableReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PushLuaTableReq)
		stream<<data.type<<data.table;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PushLuaTableReq)
		stream>>data.type>>data.table;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(OpenMailReq,Client_GS_Open_Mail_Req)
		std::string mailID;
	MESSAGE_CLASS_INIT(OpenMailReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(OpenMailReq)
		stream<<data.mailID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(OpenMailReq)
		stream>>data.mailID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ReceiveMailItemsReq,Client_GS_Receive_Mail_Items_Req)
		std::string mailID;
	MESSAGE_CLASS_INIT(ReceiveMailItemsReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ReceiveMailItemsReq)
		stream<<data.mailID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ReceiveMailItemsReq)
		stream>>data.mailID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DeleteMailReq,Client_GS_Delete_Mail_Req)
		std::vector<std::string> mailIDs;
	MESSAGE_CLASS_INIT(DeleteMailReq)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DeleteMailReq)
		stream << data.mailIDs.size();
		for (std::vector<std::string>::iterator pos = data.mailIDs.begin();
			pos!=data.mailIDs.end();pos++)
		{
			stream<<(*pos);
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DeleteMailReq)
		int size = 0;std::string mailID;
		stream >> size;
		if(size > 20){size = 20;}
		for (int i = 0;i < size;i++)
		{
			stream>>mailID;
			if(stream)
			{
				data.mailIDs.push_back(mailID);
			}
			else
			{
				printf("DeleteMailReq for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ConsignItemReq,Client_GS_Consign_Item_Req)
		int pos;
		int count;
		int price;
		int time_hour;
	MESSAGE_CLASS_INIT(ConsignItemReq)
		pos = 0;
		count = 0;
		price = 0;
		time_hour = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ConsignItemReq)
		stream<<data.pos<<data.count<<data.price<<data.time_hour;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ConsignItemReq)
		stream>>data.pos>>data.count>>data.price>>data.time_hour;
	MESSAGE_CLASS_EXPORT_END

///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GetConsignableItems,Client_GS_Get_ConsignableItems)
	int type; // 0:全部 1:装备 2:药品 3:材料 4:其他 5:自己
	int begin_index; // 开始查找索引
	int job; // 职业
	int condition; // 筛选条件
	std::string filter;
	MESSAGE_CLASS_INIT(GetConsignableItems)
	type = 0;
	begin_index = 0;
	job = 0;
	condition = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GetConsignableItems)
	stream<<data.type<<data.begin_index<<data.job<<data.condition<<data.filter;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GetConsignableItems)
	stream>>data.type>>data.begin_index>>data.job>>data.condition>>data.filter;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(BuyConsignableItem,Client_GS_Buy_Consignable_Item)
	int seed_id;
	MESSAGE_CLASS_INIT(BuyConsignableItem)
	seed_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(BuyConsignableItem)
		stream<<data.seed_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(BuyConsignableItem)
		stream>>data.seed_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
MESSAGE_CLASS_BEGIN(TakeBackConsignableItem,Client_GS_Take_Back_Consignable_Item)
	int seed_id;
	MESSAGE_CLASS_INIT(TakeBackConsignableItem)
	seed_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TakeBackConsignableItem)
		stream<<data.seed_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TakeBackConsignableItem)
		stream>>data.seed_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
}

namespace GS_Client
{
	class AuthenticateRES
	{
	public:
		static const unsigned short MSG_ID = GS_Client_Authenticate;
		int result;
		enum result_type
		{
			result_succ=100,
			result_error_system = 101,
			result_error_session_id_error = 102,
			result_error_req_error = 103,
			result_error_req_auth_type_error = 104,
			result_session_time_out = 105,
		};
	public:
		AuthenticateRES()
		{
			result = result_error_system;
		}
	};
	template<class T> inline T& operator<<(T& stream,const AuthenticateRES& data)
	{
		stream<<data.MSG_ID<<data.result;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,AuthenticateRES& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.result;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
	class CharacterLoadedNotify
	{
	public:
		static const unsigned short MSG_ID = GS_Client_Character_Loaded_Notify;
		unsigned int id;
		char charName[512];
		char loverName[512];
		char seedName[512];
		int cloth;
		int weapon;
		int hair;
		int mount;
		int x;
		int y;
		int dir;
		int hp;
		int maxhp;
		int mp;
		int maxmp;

		double exp;
		int level;

		int burden;

		int job;
		int gender;
		int fbresid;
		int fashion;
		int createtime;

		int teamid;
	public:
		CharacterLoadedNotify()
		{
		 charName[0]=0;
		 loverName[0]=0;
		 seedName[0]=0;
			id = 0;
			cloth = 0;
			weapon = 0;
			hair = 0;
			mount = 0;
			x = 0;
			y = 0;
			dir = 0;
			hp = 0;
			maxhp = 0;
			mp = 0;
			maxmp = 0;

			exp = 0;
			level = 0;

			burden = 0;
			job = 0;
			gender = 0;
			fbresid = 0;
			fashion = 0;
			createtime = 0;
			teamid = 0;
		}
	};
	template<class T> inline T& operator<<(T& stream,const CharacterLoadedNotify& data)
	{
		stream<<data.MSG_ID<<data.charName<<data.loverName<<data.id<<
			data.x<<data.y<<data.dir<<
			data.cloth<<data.weapon<<data.hair<<data.mount<<
			data.exp<<data.level<<
			data.hp<<data.maxhp<<data.mp<<data.maxmp<<
			data.burden<<data.job<<data.gender<<data.seedName<<data.fbresid<<data.fashion<<data.createtime<<data.teamid;
			return stream;
	}
	template<class T> inline T& operator>>(T& stream,CharacterLoadedNotify& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.charName>>data.loverName>>data.id>>
			data.x>>data.y>>data.dir>>
			data.cloth>>data.weapon>>data.hair>>data.mount>>
			data.exp>>data.level>>
			data.hp>>data.maxhp>>data.mp>>data.maxmp>>
			data.burden>>data.job>>data.gender>>data.seedName>>data.fbresid>>data.fashion>>data.createtime>>data.teamid;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
	class MapEnterNotify
	{
	public:
		static const unsigned short MSG_ID = GS_Client_Map_Enter_Notify;
		char map_id[512];
		char map_file[512];
		char map_name[512];
		int map_flags;
		int minimap_id;
		int init_x;
		int init_y;
		int init_dir;
		int nointeract;
		int weather;
	public:
		MapEnterNotify()
		{
			map_id[0]=0;
			map_file[0]=0;
			map_name[0]=0;
			minimap_id = -1;
			map_flags=0;
			init_x = 0;
			init_y = 0;
			init_dir = 0;
			nointeract=0;
			weather=0;
		}
	};
	template<class T> inline T& operator<<(T& stream,const MapEnterNotify& data)
	{
		stream<<data.MSG_ID<<data.map_id<<data.init_x<<data.init_y<<data.init_dir<<data.minimap_id<<data.map_file<<data.map_name<<data.map_flags<<data.nointeract<<data.weather;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,MapEnterNotify& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id>>data.map_id>>data.init_x>>data.init_y>>data.init_dir>>data.minimap_id>>data.map_file>>data.map_name>>data.map_flags>>data.nointeract>>data.weather;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
	class MapLeaveNotify
	{
	public:
		static const unsigned short MSG_ID = GS_Client_Map_Leave_Notify;
	public:
		MapLeaveNotify()
		{
		}
	};
	template<class T> inline T& operator<<(T& stream,const MapLeaveNotify& data)
	{
		stream<<data.MSG_ID;
		return stream;
	}
	template<class T> inline T& operator>>(T& stream,MapLeaveNotify& data)
	{
		unsigned short msg_id=0;
		stream>>msg_id;
		if( msg_id != data.MSG_ID )
			stream<<LEUD::Stream::FailFlag;
		return stream;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapMeetNotify,GS_Client_Map_Meet_Notify)
		unsigned int id;
		unsigned short type;
		char name[512];
		char loverName[512];
		int cloth;
		int weapon;
		int hair;
		int mount;
		unsigned short x;
		unsigned short y;
		unsigned char dir;
		int hp;
		int maxhp;
		unsigned char viplevel;
		unsigned char flags;
		int fbresid;
		int fashion;
		unsigned char zsLevel;
		int maxpower;
		int teamid;
		int ismon;
		//std::string cur_map;
	MESSAGE_CLASS_INIT(MapMeetNotify)
		name[0]=0;
		loverName[0]=0;
		id = 0;
		type = 0;
		cloth = 0;
		weapon = 0;
		hair = 0;
		mount = 0;
		x = 0;
		y = 0;
		dir = 0;
		hp = 0;
		maxhp = 0;
		viplevel=0;
		flags = 0;
		fbresid = 0;
		fashion = 0;
		zsLevel = 0;
		maxpower = 0;
		teamid = 0;
		ismon =0;
		//cur_map = "";
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapMeetNotify)
	stream << data.id << data.type << data.name << data.loverName << data.cloth << data.weapon << data.hair << data.mount << data.x << data.y << data.dir << data.hp << data.maxhp << data.viplevel << data.flags << data.fbresid << data.fashion << data.zsLevel << data.maxpower << data.teamid << data.ismon/*<<data.cur_map*/;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapMeetNotify)
	stream >> data.id >> data.type >> data.name >> data.loverName >> data.cloth >> data.weapon >> data.hair >> data.mount >> data.x >> data.y >> data.dir >> data.hp >> data.maxhp >> data.viplevel >> data.flags >> data.fbresid >> data.fashion >> data.zsLevel >> data.maxpower >> data.teamid >> data.ismon/*>>data.cur_map*/;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapByeNotify,GS_Client_Map_Bye_Notify)
		int id;
	MESSAGE_CLASS_INIT(MapByeNotify)
		id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapByeNotify)
		stream<<data.id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapByeNotify)
		stream>>data.id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CollectEndNotify,GS_Client_Collect_End_Notify)
	int srcid;
	MESSAGE_CLASS_INIT(CollectEndNotify)
		srcid=0;
	MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(CollectEndNotify)
		stream<<data.srcid;
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(CollectEndNotify)
		stream>>data.srcid;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
MESSAGE_CLASS_BEGIN(KuaFuSetParamRes,GS_KuaFu_Set_Param)
int result;
	MESSAGE_CLASS_INIT(KuaFuSetParamRes)
		result = 0;
	MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(KuaFuSetParamRes)
		stream<<data.result;
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(KuaFuSetParamRes)
		stream>>data.result;
	MESSAGE_CLASS_EXPORT_END
		///////////////////////////////////////////////////////////////////////////////////////////////////
		MESSAGE_CLASS_BEGIN(GSClientKuafuAuthRes,GS_Client_Kuafu_Auth)
		int result;
	MESSAGE_CLASS_INIT(GSClientKuafuAuthRes)
		result = 0;
	MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(GSClientKuafuAuthRes)
		stream<<data.result;
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(GSClientKuafuAuthRes)
		stream>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuaFuGetCharDataRes,GS_KuaFu_Get_Char_Data)
		int result;
		std::string loginid;
		std::string charname;
		int step;
		int len;
		const char* data;
		char* d;
		int pos;
		int num_fields;
		int num_rows;
		int fields_size;
		int rows_size;
		int row_pos;
	MESSAGE_CLASS_INIT(KuaFuGetCharDataRes)
		result = 0;
		step =0;
		len=0;
		data=0;
		d=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(KuaFuGetCharDataRes)
		stream<<data.result<<data.loginid<<data.charname<<data.step;
		if(data.step>=0){
			stream<<data.num_fields<<data.num_rows<<data.fields_size<<data.rows_size<<data.row_pos;
			stream<<data.pos<<data.len;
			if(data.len>0){
				stream.In(data.data,data.len);
			}
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuaFuGetCharDataRes)
		stream>>data.result>>data.loginid>>data.charname>>data.step;
		if(data.step>=0){
			stream>>data.num_fields>>data.num_rows>>data.fields_size>>data.rows_size>>data.row_pos;
			stream>>data.pos>>data.len;
			if(data.len>0){
				stream.Out(data.d,data.len);
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapChatNotify,GS_Client_Map_Chat_Notify)
		int srcid;
		int lv;
		int gender;
		int job;
		int vip;
		char guild[512];
		char name[512];
		char msg[512];
	MESSAGE_CLASS_INIT(MapChatNotify)
		name[0]=0;
		msg[0]=0;
		guild[0]=0;
		srcid = 0;
		lv = 0;
		gender = 0;
		job = 0;
		vip = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapChatNotify)
		stream<<data.srcid<<data.name<<data.lv<<data.gender<<data.job<<data.vip<<data.guild<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapChatNotify)
		stream>>data.srcid>>data.name>>data.lv>>data.gender>>data.job>>data.vip>>data.guild>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(WorldChatNotify,GS_Client_World_Chat_Notify)
		int srcid;
		int lv;
		int gender;
		int job;
		int vip;
		char guild[512];
		char name[512];
		char msg[512];
	MESSAGE_CLASS_INIT(WorldChatNotify)
		name[0]=0;
		msg[0]=0;
		guild[0]=0;
		srcid = 0;
		lv = 0;
		gender = 0;
		job = 0;
		vip = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(WorldChatNotify)
		stream<<data.srcid<<data.name<<data.lv<<data.gender<<data.job<<data.vip<<data.guild<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(WorldChatNotify)
		stream>>data.srcid>>data.name>>data.lv>>data.gender>>data.job>>data.vip>>data.guild>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GuildChatNotify,GS_Client_Guild_Chat_Notify)
		int srcid;
		int lv;
		int gender;
		int job;
		int vip;
		char guild[512];
		char name[512];
		char msg[512];
	MESSAGE_CLASS_INIT(GuildChatNotify)
		name[0]=0;
		msg[0]=0;
		guild[0]=0;
		srcid = 0;
		lv = 0;
		gender = 0;
		job = 0;
		vip = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildChatNotify)
		stream<<data.srcid<<data.name<<data.lv<<data.gender<<data.job<<data.vip<<data.guild<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GuildChatNotify)
		stream>>data.srcid>>data.name>>data.lv>>data.gender>>data.job>>data.vip>>data.guild>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PrivateChatNotify,GS_Client_Private_Chat_Notify)
	int srcid;
	int lv;
	int gender;
	int job;
	int vip;
	char guild[512];
	char name[512];
	char msg[512];
	MESSAGE_CLASS_INIT(PrivateChatNotify)
	name[0]=0;
	msg[0]=0;
	guild[0]=0;
	srcid = 0;
	lv = 0;
	gender = 0;
	job = 0;
	vip = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PrivateChatNotify)
		stream<<data.srcid<<data.name<<data.lv<<data.gender<<data.job<<data.vip<<data.guild<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PrivateChatNotify)
		stream>>data.srcid>>data.name>>data.lv>>data.gender>>data.job>>data.vip>>data.guild>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(HornChatNotify,GS_Client_Horn_Chat_Notify)
	int srcid;
	int lv;
	int gender;
	int job;
	int vip;
	char guild[512];
	char name[512];
	char msg[512];
	MESSAGE_CLASS_INIT(HornChatNotify)
	name[0]=0;
	msg[0]=0;
	guild[0]=0;
	srcid = 0;
	lv = 0;
	gender = 0;
	job = 0;
	vip = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(HornChatNotify)
		stream<<data.srcid<<data.name<<data.lv<<data.gender<<data.job<<data.vip<<data.guild<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(HornChatNotify)
		stream>>data.srcid>>data.name>>data.lv>>data.gender>>data.job>>data.vip>>data.guild>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(WalkNotify,GS_Client_Walk_Notify)
		unsigned int srcid;
		unsigned char dir;
		unsigned short newx;
		unsigned short newy;
	MESSAGE_CLASS_INIT(WalkNotify)
		srcid = -1;
		dir = 0;
		newx = 0;
		newy = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(WalkNotify)
		stream<<data.newx<<data.dir<<data.srcid<<data.newy;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(WalkNotify)
		stream>>data.newx>>data.dir>>data.srcid>>data.newy;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(WalkRes,	GS_Client_Walk)
		unsigned char dir;
		unsigned short newx;
		unsigned short newy;
		int tag;
	MESSAGE_CLASS_INIT(WalkRes)
		dir = 0;
		newx = 0;
		newy = 0;
		tag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(WalkRes)
		stream<<data.tag<<data.dir<<data.newy<<data.newx;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(WalkRes)
		stream>>data.tag>>data.dir>>data.newy>>data.newx;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(RunNotify,GS_Client_Run_Notify)
		unsigned int srcid;
		unsigned char dir;
		unsigned short newx;
		unsigned short newy;
	MESSAGE_CLASS_INIT(RunNotify)
		srcid = -1;
		dir = 0;
		newx = 0;
		newy = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(RunNotify)
		stream<<data.newx<<data.srcid<<data.newy<<data.dir;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(RunNotify)
		stream>>data.newx>>data.srcid>>data.newy>>data.dir;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(RunRes,GS_Client_Run)
		unsigned char dir;
		unsigned short newx;
		unsigned short newy;
		int tag;
	MESSAGE_CLASS_INIT(RunRes)
		dir = 0;
		newx = 0;
		newy = 0;
		tag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(RunRes)
		stream<<data.newx<<data.tag<<data.dir<<data.newy;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(RunRes)
		stream>>data.newx>>data.tag>>data.dir>>data.newy;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NPCTalkres,GS_Client_NPCTalk)
		int srcid;
		int flags;
		int param;
		const char* title;
		const std::string* msg;
	MESSAGE_CLASS_INIT(NPCTalkres)
		srcid = 0;
		flags = 0;
		param = 0;
		title=0;
		msg=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NPCTalkres)
		stream<<data.srcid<<data.flags<<data.param<<data.title<<(*data.msg);
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NPCTalkres)
		std::string title;
		std::string msg;
		stream>>data.srcid>>data.flags>>data.param>>title>>msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(BlackBoardNotify,GS_Client_Black_Board_Notify)
		int flags;
		const char* title;
		const std::string* msg;
	MESSAGE_CLASS_INIT(BlackBoardNotify)
		flags = 0;
		title=0;
		msg=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(BlackBoardNotify)
		stream<<data.flags<<data.title<<(*data.msg);
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(BlackBoardNotify)
		std::string title;
		std::string msg;
		stream>>data.flags>>title>>msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TurnNotify,GS_Client_Turn_Notify)
		unsigned int srcid;
		unsigned char dir;
	MESSAGE_CLASS_INIT(TurnNotify)
		srcid = 0;
		dir = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TurnNotify)
		stream<<data.srcid<<data.dir;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TurnNotify)
		stream>>data.srcid>>data.dir;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AttackNotify,GS_Client_Attack_Notify)
		int srcid;
		int param;
	MESSAGE_CLASS_INIT(AttackNotify)
		srcid = 0;
		param = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AttackNotify)
		stream<<data.srcid<<data.param;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AttackNotify)
		stream>>data.srcid>>data.param;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(InjuryNotify,GS_Client_Injury_Notify)
		int srcid;
		int attacker;
		int newhp;
		int newpower;
		int change;
		int delay;
		int show_flag;
		int change_power;
	MESSAGE_CLASS_INIT(InjuryNotify)
		srcid = 0;
		attacker = 0;
		newhp = 0;
		newpower = 0;
		change = 0;
		delay = 0;
		show_flag = 0;
		change_power = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(InjuryNotify)
		stream<<data.srcid<<data.newhp<<data.newpower<<data.change<<data.delay<<data.attacker<<data.show_flag<<data.change_power;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(InjuryNotify)
		stream>>data.srcid>>data.newhp>>data.newpower>>data.change>>data.delay>>data.attacker>>data.show_flag>>data.change_power;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DieNotify,GS_Client_Die_Notify)
		int srcid;
		int delay;
	MESSAGE_CLASS_INIT(DieNotify)
		srcid = 0;
		delay = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DieNotify)
		stream<<data.srcid<<data.delay;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DieNotify)
		stream>>data.srcid>>data.delay;
	MESSAGE_CLASS_EXPORT_END

///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MonExpHiterChangeNotify,GS_Client_Mon_Exp_Hiter_Change_Notify)
	int src_id;
	int hiter_id;
	std::string hiter_name; 
	MESSAGE_CLASS_INIT(MonExpHiterChangeNotify)
	src_id = 0;
	hiter_id = 0;
	MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(MonExpHiterChangeNotify)
		stream<<data.src_id<<data.hiter_id<<data.hiter_name;
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(MonExpHiterChangeNotify)
		stream>>data.src_id>>data.hiter_id>>data.hiter_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ItemChangeNotify,GS_Client_Item_Change_Notify)
		int position;
		int type_id;
		int duration_max;
		int duration;
		int item_flags;
		int show_flags;
		int level;
		int number;
		short add_ac;
		short add_mac;
		short add_dc;
		short add_mc;
		short add_sc;
		short add_hp;
		short add_mp;
		short upd_ac;
		short upd_mac;
		short upd_dc;
		short upd_mc;
		short upd_sc;
		short upd_maxcount;
		short upd_failedcount;
		short luck;
		short protect;
		short sell_pricetype;
		int sell_price;
		int create_time;
		int last_time;
		int zlevel;
		int lock;
	MESSAGE_CLASS_INIT(ItemChangeNotify)
		position = 0;
		type_id = 0;
		duration = 0;
		duration_max = 0;
		item_flags = 0;
		show_flags = 0;
		level = 0;
		number=0;
		luck = 0;
		add_ac = 0;
		add_mac = 0;
		add_dc = 0;
		add_mc = 0;
		add_sc = 0;
		add_hp = 0;
		add_mp = 0;
		upd_ac = 0;
		upd_mac = 0;
		upd_dc = 0;
		upd_mc = 0;
		upd_sc = 0;
		upd_maxcount = 0;
		upd_failedcount = 0;
		protect = 0;
		sell_pricetype = 0;
		sell_price = 0;
		create_time=0;
		last_time = 0;
		zlevel = 0;
		lock = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ItemChangeNotify)
		stream<<data.position<<data.type_id<<data.duration_max<<data.duration<<data.item_flags<<data.level<<data.number
		<<data.add_ac<<data.add_mac<<data.add_dc<<data.add_mc<<data.add_sc
		<<data.upd_ac<<data.upd_mac<<data.upd_dc<<data.upd_mc<<data.upd_sc<<data.upd_maxcount<<data.upd_failedcount
		<<data.luck<<data.show_flags<<data.protect<<data.sell_pricetype<<data.sell_price
		<< data.add_hp << data.add_mp << data.create_time << data.last_time << data.zlevel << data.lock;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ItemChangeNotify)
	stream>>data.position>>data.type_id>>data.duration_max>>data.duration>>data.item_flags>>data.level>>data.number
		>>data.add_ac>>data.add_mac>>data.add_dc>>data.add_mc>>data.add_sc
		>>data.upd_ac>>data.upd_mac>>data.upd_dc>>data.upd_mc>>data.upd_sc>>data.upd_maxcount>>data.upd_failedcount
		>>data.luck>>data.show_flags>>data.protect>>data.sell_pricetype>>data.sell_price
		>> data.add_hp >> data.add_mp >> data.create_time >> data.last_time >> data.zlevel >> data.lock;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListUpgradeDespNotify,GS_Client_List_Upgrade_Desp_Notify)
		struct UpgradeDesp
		{
			int mJob;
			int mEquipType;
			int mLevel;
			int mDC;
			int mDCMax;
			int mMC;
			int mMCMax;
			int mSC;
			int mSCMax;
			int mAC;
			int mACMax;
			int mMAC;
			int mMACMax;
		};
		std::vector<ListUpgradeDespNotify::UpgradeDesp> m_UpgradeDefList;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListUpgradeDespNotify)
	stream<<static_cast<int>(data.m_UpgradeDefList.size());
	for (std::vector<ListUpgradeDespNotify::UpgradeDesp> ::const_iterator it = data.m_UpgradeDefList.begin();it!=data.m_UpgradeDefList.end();it++)
	{
		stream<<it->mJob
			<<it->mEquipType
			<<it->mLevel
			<<it->mDC
			<<it->mDCMax
			<<it->mMC
			<<it->mMCMax
			<<it->mSC
			<<it->mSCMax
			<<it->mAC
			<<it->mACMax
			<<it->mMAC
			<<it->mMACMax;
	}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListUpgradeDespNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			ListUpgradeDespNotify::UpgradeDesp it;
			stream>>it->mJob
				>>it->mEquipType
				>>it->mLevel
				>>it->mDC
				>>it->mDCMax
				>>it->mMC
				>>it->mMCMax
				>>it->mSC
				>>it->mSCMax
				>>it->mAC
				>>it->mACMax
				>>it->mMAC
				>>it->mMACMax;
			if( stream )
			{
				data.m_UpgradeDefList.push_back(it);
			}
			else
			{
				printf("ListGuildMemberRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END

///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListItemChangeNotify,GS_Client_List_Item_Change_Notify)
		struct NetItem
		{
			int position;
			int type_id;
			int duration_max;
			int duration;
			int item_flags;
			int show_flags;
			int level;
			int number;
			short add_ac;
			short add_mac;
			short add_dc;
			short add_mc;
			short add_sc;
			short add_hp;
			short add_mp;
			short upd_ac;
			short upd_mac;
			short upd_dc;
			short upd_mc;
			short upd_sc;
			short upd_maxcount;
			short upd_failedcount;
			short luck;
			short protect;
			short sell_pricetype;
			int sell_price;
			int create_time;
			int last_time;
			int zlevel;
			int lock;
		};
		std::vector<ListItemChangeNotify::NetItem> m_NetItemList;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListItemChangeNotify)
	stream<<static_cast<int>(data.m_NetItemList.size());
	for (std::vector<ListItemChangeNotify::NetItem> ::const_iterator it = data.m_NetItemList.begin();it!=data.m_NetItemList.end();it++)
	{
		stream<<it->position
			<<it->type_id
			<<it->duration_max
			<<it->duration
			<<it->item_flags
			<<it->level
			<<it->number
		
			<<it->add_ac
			<<it->add_mac
			<<it->add_dc
			<<it->add_mc
			<<it->add_sc

			<<it->upd_ac
			<<it->upd_mac
			<<it->upd_dc
			<<it->upd_mc
			<<it->upd_sc
			<<it->upd_maxcount
			<<it->upd_failedcount

			<<it->luck
			<<it->show_flags
			<<it->protect
			<<it->sell_pricetype
			<<it->sell_price

			<<it->add_hp
			<<it->add_mp
			<<it->create_time
			<<it->last_time
			<<it->zlevel
			<<it->lock;
	}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListItemChangeNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			ListItemChangeNotify::NetItem it;
			stream>>it->position
				>>it->type_id
				>>it->duration_max
				>>it->duration
				>>it->item_flags
				>>it->level
				>>it->number
		
				>>it->add_ac
				>>it->add_mac
				>>it->add_dc
				>>it->add_mc
				>>it->add_sc

				>>it->upd_ac
				>>it->upd_mac
				>>it->upd_dc
				>>it->upd_mc
				>>it->upd_sc
				>>it->upd_maxcount
				>>it->upd_failedcount

				>>it->luck
				>>it->show_flags
				>>it->protect
				>>it->sell_pricetype
				>>it->sell_price

				>>it->add_hp
				>>it->add_mp
				>>it->create_time
				>>it->last_time
				>>it->zlevel
				>>it->lock;
			if( stream )
			{
				data.m_NetItemList.push_back(it);
			}
			else
			{
				printf("ListGuildMemberRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ScriptItemChangeNotify,GS_Client_ScriptItem_Change_Notify)
		int position;
		float mMaxHpMul;
		float mMaxHpAdd;
		float mMaxMpMul;
		float mMaxMpAdd;
		float mACMul;
		float mACMaxMul;
		float mMACMul;
		float mMACMaxMul;
		float mDCMul;
		float mDCMaxMul;
		float mMCMul;
		float mMCMaxMul;
		float mSCMul;
		float mSCMaxMul;
		float mACAdd;
		float mACMaxAdd;
		float mMACAdd;
		float mMACMaxAdd;
		float mDCAdd;
		float mDCMaxAdd;
		float mMCAdd;
		float mMCMaxAdd;
		float mSCAdd;
		float mSCMaxAdd;
	MESSAGE_CLASS_INIT(ScriptItemChangeNotify)
		position = 0;
		mMaxHpMul=0.0f;
		mMaxHpAdd=0.0f;
		mMaxMpMul=0.0f;
		mMaxMpAdd=0.0f;
		mACMul=0.0f;
		mACMaxMul=0.0f;
		mMACMul=0.0f;
		mMACMaxMul=0.0f;
		mDCMul=0.0f;
		mDCMaxMul=0.0f;
		mMCMul=0.0f;
		mMCMaxMul=0.0f;
		mSCMul=0.0f;
		mSCMaxMul=0.0f;
		mACAdd=0.0f;
		mACMaxAdd=0.0f;
		mMACAdd=0.0f;
		mMACMaxAdd=0.0f;
		mDCAdd=0.0f;
		mDCMaxAdd=0.0f;
		mMCAdd=0.0f;
		mMCMaxAdd=0.0f;
		mSCAdd=0.0f;
		mSCMaxAdd=0.0f;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ScriptItemChangeNotify)
		stream<<data.position<<data.mMaxHpMul<<data.mMaxHpAdd<<data.mMaxMpMul<<data.mMaxMpAdd<<data.mACMul<<data.mACMaxMul<<data.mMACMul<<data.mMACMaxMul
			<<data.mDCMul<<data.mDCMaxMul<<data.mMCMul<<data.mMCMaxMul<<data.mSCMul<<data.mSCMaxMul<<data.mACAdd<<data.mACMaxAdd
			<<data.mMACAdd<<data.mMACMaxAdd<<data.mDCAdd<<data.mDCMaxAdd<<data.mMCAdd<<data.mMCMaxAdd<<data.mSCAdd<<data.mSCMaxAdd;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ScriptItemChangeNotify)
		stream>>data.position>>data.mMaxHpMul>>data.mMaxHpAdd>>data.mMaxMpMul>>data.mMaxMpAdd>>data.mACMul>>data.mACMaxMul>>data.mMACMul>>data.mMACMaxMul
			>>data.mDCMul>>data.mDCMaxMul>>data.mMCMul>>data.mMCMaxMul>>data.mSCMul>>data.mSCMaxMul>>data.mACAdd>>data.mACMaxAdd
			>>data.mMACAdd>>data.mMACMaxAdd>>data.mDCAdd>>data.mDCMaxAdd>>data.mMCAdd>>data.mMCMaxAdd>>data.mSCAdd>>data.mSCMaxAdd;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeItemChangeNotify,GS_Client_Trade_Item_Change_Notify)
		int side;
		int position;
		int type_id;
		int duration_max;
		int duration;
		int item_flags;
		int show_flags;
		int level;
		int upd_count;
		short add_ac;
		short add_mac;
		short add_dc;
		short add_mc;
		short add_sc;
		short add_hp;
		short add_mp;
		short upd_ac;
		short upd_mac;
		short upd_dc;
		short upd_mc;
		short upd_sc;
		short luck;
		short protect;
		int create_time;
		int sellprice;
		int zlevel;
		int lock;
	MESSAGE_CLASS_INIT(TradeItemChangeNotify)
		side = 0;
		position = 0;
		type_id = 0;
		duration = 0;
		item_flags = 0;
		show_flags = 0;
		level = 0;
		upd_count=0;
		luck = 0;
		add_ac = 0;
		add_mac = 0;
		add_dc = 0;
		add_mc = 0;
		add_sc = 0;
		add_hp=0;
		add_mp=0;
		upd_ac = 0;
		upd_mac = 0;
		upd_dc = 0;
		upd_mc = 0;
		upd_sc = 0;
		protect = 0;
		create_time=0;
		sellprice=0;
		zlevel = 0;
		lock = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeItemChangeNotify)
		stream<<data.side<<data.position<<data.type_id<<data.duration_max<<data.duration<<data.item_flags<<data.level<<data.upd_count
		<<data.add_ac<<data.add_mac<<data.add_dc<<data.add_mc<<data.add_sc
		<<data.upd_ac<<data.upd_mac<<data.upd_dc<<data.upd_mc<<data.upd_sc
		<<data.luck<<data.show_flags<<data.protect
		<<data.add_hp<<data.add_mp<<data.create_time<<data.sellprice<<data.zlevel<<data.lock;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeItemChangeNotify)
		stream>>data.side>>data.position>>data.type_id>>data.duration_max>>data.duration>>data.item_flags>>data.level>>data.upd_count
		>>data.add_ac>>data.add_mac>>data.add_dc>>data.add_mc>>data.add_sc
		>>data.upd_ac>>data.upd_mac>>data.upd_dc>>data.upd_mc>>data.upd_sc
		>>data.luck>>data.show_flags>>data.protect
		>>data.add_hp>>data.add_mp>>data.create_time>>data.sellprice>>data.zlevel>>data.lock;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(InfoItemChangeNotify,GS_Client_Info_Item_Change_Notify)
		unsigned int src_id;
		int position;
		int type_id;
		int duration_max;
		int duration;
		int item_flags;
		int level;
		int zlevel;
		int upd_count;
		short add_ac;
		short add_mac;
		short add_dc;
		short add_mc;
		short add_sc;
		short add_hp;
		short add_mp;
		short upd_ac;
		short upd_mac;
		short upd_dc;
		short upd_mc;
		short upd_sc;
		short luck;
		short protect;
		int create_time;
	MESSAGE_CLASS_INIT(InfoItemChangeNotify)
		src_id = 0;
		position = 0;
		type_id = 0;
		duration = 0;
		item_flags = 0;
		luck = 0;
		level = 0;
		zlevel =0;
		upd_count=0;
		add_ac = 0;
		add_mac = 0;
		add_dc = 0;
		add_mc = 0;
		add_sc = 0;
		upd_ac = 0;
		upd_mac = 0;
		upd_dc = 0;
		upd_mc = 0;
		upd_sc = 0;
		protect = 0;
		add_hp=0;
		add_mp=0;
		create_time=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(InfoItemChangeNotify)
	stream<<data.src_id<<data.position<<data.type_id<<data.duration_max<<data.duration<<data.item_flags<<data.level<<data.zlevel<<data.upd_count<<data.add_ac<<data.add_mac<<data.add_dc<<data.add_mc<<data.add_sc<<data.upd_ac<<data.upd_mac<<data.upd_dc<<data.upd_mc<<data.upd_sc<<data.luck<<data.protect<<data.add_hp<<data.add_mp<<data.create_time;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(InfoItemChangeNotify)
	stream>>data.src_id>>data.position>>data.type_id>>data.duration_max>>data.duration>>data.item_flags>>data.level>>data.zlevel>>data.upd_count>>data.add_ac>>data.add_mac>>data.add_dc>>data.add_mc>>data.add_sc>>data.upd_ac>>data.upd_mac>>data.upd_dc>>data.upd_mc>>data.upd_sc>>data.luck>>data.protect>>data.add_hp>>data.add_mp>>data.create_time;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ScriptItemInfoChangeNotify,GS_Client_ScriptItem_InfoChange_Notify)
		unsigned int src_id;
		int position;
		float mMaxHpMul;
		float mMaxHpAdd;
		float mMaxMpMul;
		float mMaxMpAdd;
		float mACMul;
		float mACMaxMul;
		float mMACMul;
		float mMACMaxMul;
		float mDCMul;
		float mDCMaxMul;
		float mMCMul;
		float mMCMaxMul;
		float mSCMul;
		float mSCMaxMul;
		float mACAdd;
		float mACMaxAdd;
		float mMACAdd;
		float mMACMaxAdd;
		float mDCAdd;
		float mDCMaxAdd;
		float mMCAdd;
		float mMCMaxAdd;
		float mSCAdd;
		float mSCMaxAdd;
	MESSAGE_CLASS_INIT(ScriptItemInfoChangeNotify)
		src_id = 0;
		position = 0;
		mMaxHpMul=0.0f;
		mMaxHpAdd=0.0f;
		mMaxMpMul=0.0f;
		mMaxMpAdd=0.0f;
		mACMul=0.0f;
		mACMaxMul=0.0f;
		mMACMul=0.0f;
		mMACMaxMul=0.0f;
		mDCMul=0.0f;
		mDCMaxMul=0.0f;
		mMCMul=0.0f;
		mMCMaxMul=0.0f;
		mSCMul=0.0f;
		mSCMaxMul=0.0f;
		mACAdd=0.0f;
		mACMaxAdd=0.0f;
		mMACAdd=0.0f;
		mMACMaxAdd=0.0f;
		mDCAdd=0.0f;
		mDCMaxAdd=0.0f;
		mMCAdd=0.0f;
		mMCMaxAdd=0.0f;
		mSCAdd=0.0f;
		mSCMaxAdd=0.0f;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ScriptItemInfoChangeNotify)
		stream<<data.src_id<<data.position<<data.mMaxHpMul<<data.mMaxHpAdd<<data.mMaxMpMul<<data.mMaxMpAdd<<data.mACMul<<data.mACMaxMul<<data.mMACMul<<data.mMACMaxMul
			<<data.mDCMul<<data.mDCMaxMul<<data.mMCMul<<data.mMCMaxMul<<data.mSCMul<<data.mSCMaxMul<<data.mACAdd<<data.mACMaxAdd
			<<data.mMACAdd<<data.mMACMaxAdd<<data.mDCAdd<<data.mDCMaxAdd<<data.mMCAdd<<data.mMCMaxAdd<<data.mSCAdd<<data.mSCMaxAdd;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ScriptItemInfoChangeNotify)
		stream>>data.src_id>>data.position>>data.mMaxHpMul>>data.mMaxHpAdd>>data.mMaxMpMul>>data.mMaxMpAdd>>data.mACMul>>data.mACMaxMul>>data.mMACMul>>data.mMACMaxMul
			>>data.mDCMul>>data.mDCMaxMul>>data.mMCMul>>data.mMCMaxMul>>data.mSCMul>>data.mSCMaxMul>>data.mACAdd>>data.mACMaxAdd
			>>data.mMACAdd>>data.mMACMaxAdd>>data.mDCAdd>>data.mDCMaxAdd>>data.mMCAdd>>data.mMCMaxAdd>>data.mSCAdd>>data.mSCMaxAdd;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AvatarChangeNotify,GS_Client_Avatar_Change_Notify)
		unsigned int srcid;
		int cloth;
		int weapon;
		int mount;
		char name[512];
		char loverName[512];
		int wing;
		int fbresid;
		int fashion;
		int zsLevel;
		int ismon;
		char low;
	MESSAGE_CLASS_INIT(AvatarChangeNotify)
		srcid = 0;
		cloth = 0;
		weapon = 0;
		mount = 0;
		name[0] = 0;
		loverName[0]=0;
		wing=0;
		fbresid = 0;
		fashion = 0;
		zsLevel = 0;
		ismon = 0;
		low=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AvatarChangeNotify)
	stream<<data.srcid<<data.cloth<<data.weapon<<data.mount<< data.name << data.loverName<<data.wing<<data.fbresid<<data.fashion<<data.zsLevel<<data.ismon<<data.low;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AvatarChangeNotify)
	stream>>data.srcid>>data.cloth>>data.weapon>>data.mount>>data.name >> data.loverName>>data.wing>>data.fbresid>>data.fashion>>data.zsLevel>>data.ismon>>data.low ;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SkillChangeNotify,GS_Client_Skill_Change_Notify)
		int skill_type_id;
		int lv;
		int exp;
		int param1;
	MESSAGE_CLASS_INIT(SkillChangeNotify)
		skill_type_id = 0;
		lv = 0;
		exp = 0;
		param1 = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SkillChangeNotify)
		stream<<data.skill_type_id<<data.lv<<data.exp<<data.param1;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SkillChangeNotify)
		stream>>data.skill_type_id>>data.lv>>data.exp>>data.param1;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(UseSkillRes,GS_Client_UseSkill)
		int skill_type_id;
		int result;
	MESSAGE_CLASS_INIT(UseSkillRes)
		skill_type_id = 0;
		result = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(UseSkillRes)
		stream<<data.result<<data.skill_type_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(UseSkillRes)
		stream>>data.result>>data.skill_type_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(UseSkillNotify,GS_Client_UseSkill_Notify)
		unsigned int srcid;
		unsigned short skill_type_id;
		unsigned short paramX;
		unsigned short paramY;
		unsigned int paramID;
		int resID;
		unsigned char dir;
		unsigned char skill_level;
		
	MESSAGE_CLASS_INIT(UseSkillNotify)
		srcid = 0;
		skill_type_id = 0;
		paramX = 0;
		paramY = 0;
		paramID = 0;
		resID = 0;
		skill_level = 0;
		dir = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(UseSkillNotify)
		stream<<data.srcid<<data.skill_type_id<<data.paramX<<data.paramY<<data.paramID<<data.resID<<data.skill_level<<data.dir;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(UseSkillNotify)
		stream>>data.srcid>>data.skill_type_id>>data.paramX>>data.paramY>>data.paramID>>data.resID>>data.skill_level>>data.dir;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AttributeChangeNotify,GS_Client_Attribute_Change_Notify)
		int max_hp;
		int max_mp;

		int max_burden;
		int burden;
		int max_load;
		int load;
		int max_brawn;
		int brawn;

		int ac;
		int maxac;
		int mac;
		int maxmac;
		int dc;
		int maxdc;
		int mc;
		int maxmc;
		int sc;
		int maxsc;

		int accuracy;
		int dodge;

		int dropprob;
		int doubleattk;
		int attrlevel;
		int maxpower;

		int luck;
		int honor;
		int xishou_pres;

		int tenacity;
		int critProb;
		int critPoint;
		int holyDam;
	MESSAGE_CLASS_INIT(AttributeChangeNotify)
		max_hp=0;
		max_mp=0;

		max_burden = 0;
		burden = 0;
		max_load = 0;
		load = 0;
		max_brawn=0;
		brawn = 0;

		ac = 0;
		maxac = 0;
		mac = 0;
		maxmac = 0;
		dc = 0;
		maxdc = 0;
		mc = 0;
		maxmc = 0;
		sc = 0;
		maxsc = 0;

		accuracy = 0;
		dodge = 0;

		dropprob = 0;
		doubleattk = 0;
		attrlevel = 0;
		maxpower = 0;

		luck = 0;
		honor = 0;
		xishou_pres = 0;

		tenacity = 0;
		critProb = 0;
		critPoint = 0;
		holyDam = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AttributeChangeNotify)
		stream<<data.max_hp<<data.max_mp<<data.max_burden<<data.burden<<
			data.max_load<<data.load<<
			data.max_brawn<<data.brawn<<
			data.maxac<<data.ac<<
			data.maxmac<<data.mac<<
			data.maxdc<<data.dc<<
			data.maxmc<<data.mc<<
			data.maxsc<<data.sc<<
			data.accuracy<<data.dodge<<
			data.dropprob<<data.doubleattk<<
			data.attrlevel <<data.maxpower<<
			data.luck<<data.honor<<
			data.xishou_pres<<data.critProb<<
			data.critPoint<<data.tenacity<<data.holyDam;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AttributeChangeNotify)
		stream>>data.max_hp>>data.max_mp>>data.max_burden>>data.burden>>
			data.max_load>>data.load>>
			data.max_brawn>>data.brawn>>
			data.maxac>>data.ac>>
			data.maxmac>>data.mac>>
			data.maxdc>>data.dc>>
			data.maxmc>>data.mc>>
			data.maxsc>>data.sc>>
			data.accuracy>>data.dodge>>
			data.dropprob>>data.doubleattk>>
			data.attrlevel>>data.maxpower>>
			data.luck>>data.honor>>
			data.xishou_pres>>data.critProb>>
			data.critPoint>>data.tenacity>>data.holyDam;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NpcShopRes,GS_Client_NPCShop)
		unsigned int srcid;
		std::string msg;
		int page;
		struct good
		{
			int pos;
			int good_id;
			int type_id;
			int number;
			int price_type;
			int price;
			int oldprice;
			int good_tag;
		};
		std::vector<good> goods;
	MESSAGE_CLASS_INIT(NpcShopRes)
		srcid = 0;
		page = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NpcShopRes)
		stream<<data.srcid<<data.msg<<data.page<<static_cast<int>(data.goods.size());
		for( std::vector<NpcShopRes::good>::const_iterator pos = data.goods.begin();
			pos != data.goods.end();
			++ pos )
		{
			stream<<pos->pos<<pos->good_id<<pos->type_id<<pos->number<<pos->price_type<<pos->price<<pos->oldprice<<pos->good_tag;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NpcShopRes)
		int size=0;
		stream>>data.srcid>>data.msg>>data.page>>size;
		for( int i = 0;i<size;i++ )
		{
			NpcShopRes::good g;
			stream>>g.pos>>g.good_id>>g.type_id>>g.number>>g.price_type>>g.price>>g.oldprice>>g.good_tag;
			if( stream )
			{
				data.goods.push_back(g);
			}
			else
			{
				printf("NpcShopRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GameMoneyChangeNotify,GS_Client_GameMoney_Change_Notify)
		double new_game_money;
		double new_game_money_bind;
		int new_vcion;
		int new_vcion_bind;
		double gm_change;
		int vc_change;
		double gmb_change;
		int vcb_change;
	MESSAGE_CLASS_INIT(GameMoneyChangeNotify)
		new_game_money = 0;
		new_game_money_bind = 0;
		new_vcion = 0;
		new_vcion_bind = 0;
		gm_change= 0;
		vc_change=0;
		gmb_change = 0;
		vcb_change = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GameMoneyChangeNotify)
		stream<<data.new_game_money<<data.new_game_money_bind<<data.new_vcion<<data.new_vcion_bind<<data.gm_change<<data.vc_change<<data.gmb_change<<data.vcb_change;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GameMoneyChangeNotify)
		stream>>data.new_game_money>>data.new_game_money_bind>>data.new_vcion>>data.new_vcion_bind>>data.gm_change>>data.vc_change>>data.gmb_change>>data.vcb_change;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuafuInfoNotify,GS_Client_Kuafu_Info_Notify)
		std::string ticket;
		std::string loginid;
		std::string charname;
		std::string kuafuip;
		std::string kuafuport;
		std::string kuafuparam;
		int ticketseed;
		std::string localip;
		std::string localport;
		std::string localPTID;
		std::string	localServerID;
		std::string localArea;
	MESSAGE_CLASS_INIT(KuafuInfoNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(KuafuInfoNotify)
		stream<<data.ticket<<data.loginid<<data.charname<<data.kuafuip<<data.kuafuport<<data.kuafuparam<<data.ticketseed<<data.localip<<data.localport<<data.localPTID<<data.localServerID<<data.localArea;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuafuInfoNotify)
		stream>>data.ticket>>data.loginid>>data.charname>>data.kuafuip>>data.kuafuport>>data.kuafuparam>>data.ticketseed>>data.localip>>data.localport>>data.localPTID>>data.localServerID>>data.localArea;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuafuEnterMainServerNotify,GS_Client_Kuafu_Enter_Main_Server_Notify)
		std::string ticket;
		std::string result;
	MESSAGE_CLASS_INIT(KuafuEnterMainServerNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(KuafuEnterMainServerNotify)
		stream<<data.ticket<<data.result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuafuEnterMainServerNotify)
		stream>>data.ticket>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(KuafuPTInfoNotify,GS_Client_Kuafu_PT_Info)
		std::string ticket;
		std::string kuafuPTURL;
		std::string localPTID;
		std::string localServerID;
	MESSAGE_CLASS_INIT(KuafuPTInfoNotify)
		MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(KuafuPTInfoNotify)
		stream<<data.ticket<<data.kuafuPTURL<<data.localPTID<<data.localServerID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(KuafuPTInfoNotify)
		stream>>data.ticket>>data.kuafuPTURL>>data.localPTID>>data.localServerID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(HPMPChangeNotify,GS_Client_HP_MP_Change_Notify)
		unsigned int id;
		int hp;
		int mp;
		int hpmax;
		int mpmax;
		int hpchange;
		int mpchange;
	MESSAGE_CLASS_INIT(HPMPChangeNotify)
		id = 0;
		hp = 0;
		mp = 0;
		hpmax = 0;
		mpmax = 0;
		hpchange = 0;
		mpchange = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(HPMPChangeNotify)
		stream<<data.id<<data.hp<<data.mp<<data.hpmax<<data.mpmax<<data.hpchange<<data.mpchange;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(HPMPChangeNotify)
		stream>>data.id>>data.hp>>data.mp>>data.hpmax>>data.mpmax>>data.hpchange>>data.mpchange;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TaskChangeNotify,GS_Client_Task_Change_Notify)
		int TaskID;
		int Flags;
		int TaskState;
		short TaskParam1;
		short TaskParam2;
		short TaskParam3;
		int TaskParam4;
		char TaskName[512];
		const char* TaskShortDesp2;
	MESSAGE_CLASS_INIT(TaskChangeNotify)
		TaskID=0;		
		Flags=0;
		TaskState=0;		
		TaskParam1=0;		
		TaskParam2=0;		
		TaskParam3=0;		
		TaskParam4=0;
		TaskName[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TaskChangeNotify)
		stream<<data.TaskID<<data.Flags<<data.TaskState<<data.TaskParam1<<data.TaskParam2<<data.TaskParam3<<data.TaskParam4<<data.TaskName<<data.TaskShortDesp2;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TaskChangeNotify)
		stream>>data.TaskID>>data.Flags>>data.TaskState>>data.TaskParam1>>data.TaskParam2>>data.TaskParam3>>data.TaskParam4>>data.TaskName>>data.TaskShortDesp2;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ExpChangeNotify,GS_Client_Exp_Change_Notify)
		double exp;
		double next_exp;
		int exp_change;
	MESSAGE_CLASS_INIT(ExpChangeNotify)
		exp = 0;
		next_exp = 0;
		exp_change = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ExpChangeNotify)
		stream<<data.exp<<data.next_exp<<data.exp_change;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ExpChangeNotify)
		stream>>data.exp>>data.next_exp>>data.exp_change;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(LevelChangeNotify,GS_Client_Level_Change_Notify)
		int level;
	MESSAGE_CLASS_INIT(LevelChangeNotify)
		level = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(LevelChangeNotify)
		stream<<data.level;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(LevelChangeNotify)
		stream>>data.level;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ForceMoveNotify,GS_Client_Force_Move_Notify)
		unsigned int id;
		unsigned short x;
		unsigned short y;
		unsigned char dir;
	MESSAGE_CLASS_INIT(ForceMoveNotify)
		id = 0;
		x = 0;
		y = 0;
		dir = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ForceMoveNotify)
		stream<<data.id<<data.x<<data.y<<data.dir;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ForceMoveNotify)
		stream>>data.id>>data.x>>data.y>>data.dir;
    MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ItemPlusDespNotify,GS_Client_Item_Plus_Desp_Notify)
		int mItemPlusID;
		int mTypeID;

		int mNeedType;
		int mNeedParam1;
		int mNeedParam2;
		int mNeedParam3;
		int mNeedParam4;
		int mNeedParam5;
		
		int mAC;
		int mACMax;
		int mMAC;
		int mMACMax;
		int mDC;
		int mDCMax;
		int mMC;
		int mMCMax;
		int mSC;
		int mSCMax;

		int mLuck;
		int mCurse;
		int mAccuracy;
		int mDodge;
		int mAntiMagic;
		int mAntiPoison;

		int mHpRecover;
		int mMpRecover;
		int mPoisonRecover;

		int mMabiProb;
		int mMabiDura;
		int mDixiaoPres;
		int mFuyuanCd;
		int mFuyuanPres;

		int mMaxHp;
		int mMaxMp;
		int mMaxHpPres;
		int mMaxMpPres;
		int mHalfBaoji;
	MESSAGE_CLASS_INIT(ItemPlusDespNotify)
		mItemPlusID=0;
		mTypeID=0;

		mNeedType=0;
		mNeedParam1=0;
		mNeedParam2=0;
		mNeedParam3=0;
		mNeedParam4=0;		
		mNeedParam5=0;

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

		mMabiProb=0;
		mMabiDura=0;
		mDixiaoPres=0;
		mFuyuanCd=0;
		mFuyuanPres=0;

		mMaxHp=0;
		mMaxMp=0;
		mMaxHpPres=0;
		mMaxMpPres=0;
		mHalfBaoji=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ItemPlusDespNotify)
		stream<<data.mItemPlusID<<data.mTypeID
				<<data.mNeedType<<data.mNeedParam1<<data.mNeedParam2<<data.mNeedParam3<<data.mNeedParam4<<data.mNeedParam5
				<<data.mAC<<data.mACMax<<data.mMAC<<data.mMACMax<<data.mDC<<data.mDCMax<<data.mMC<<data.mMCMax<<data.mSC<<data.mSCMax
				<<data.mLuck<<data.mCurse<<data.mAccuracy<<data.mDodge<<data.mAntiMagic<<data.mAntiPoison<<data.mHpRecover<<data.mMpRecover<<data.mPoisonRecover
				<<data.mMabiProb<<data.mMabiDura<<data.mDixiaoPres<<data.mFuyuanCd<<data.mFuyuanPres<<data.mMaxHp<<data.mMaxMp<<data.mMaxHpPres<<data.mMaxMpPres<<data.mHalfBaoji;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ItemPlusDespNotify) 
		stream>>data.mItemPlusID>>data.mTypeID
				>>data.mNeedType>>data.mNeedParam1>>data.mNeedParam2>>data.mNeedParam3>>data.mNeedParam4>>data.mNeedParam5
				>>data.mAC>>data.mACMax>>data.mMAC>>data.mMACMax>>data.mDC>>data.mDCMax>>data.mMC>>data.mMCMax>>data.mSC>>data.mSCMax
				>>data.mLuck>>data.mCurse>>data.mAccuracy>>data.mDodge>>data.mAntiMagic>>data.mAntiPoison>>data.mHpRecover>>data.mMpRecover>>data.mPoisonRecover
				>>data.mMabiProb>>data.mMabiDura>>data.mDixiaoPres>>data.mFuyuanCd>>data.mFuyuanPres>>data.mMaxHp>>data.mMaxMp>>data.mMaxHpPres>>data.mMaxMpPres>>data.mHalfBaoji;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
    MESSAGE_CLASS_BEGIN(ItemPlusDespNotifyGroup, GS_Client_Item_Plus_Desp_Notify_Group)
        std::vector<ItemPlusDespNotify> items;
    MESSAGE_CLASS_INIT(ItemPlusDespNotifyGroup)
    MESSAGE_CLASS_INIT_END
    MESSAGE_CLASS_END
    MESSAGE_CLASS_IMPORT(ItemPlusDespNotifyGroup)
    stream << int(data.items.size());
    for (auto &it = data.items.begin(); it != data.items.end(); ++it) {
        stream << (*it).mItemPlusID << (*it).mTypeID
            << (*it).mNeedType << (*it).mNeedParam1 << (*it).mNeedParam2 << (*it).mNeedParam3 << (*it).mNeedParam4 << (*it).mNeedParam5
            << (*it).mAC << (*it).mACMax << (*it).mMAC << (*it).mMACMax << (*it).mDC << (*it).mDCMax << (*it).mMC << (*it).mMCMax << (*it).mSC << (*it).mSCMax
            << (*it).mLuck << (*it).mCurse << (*it).mAccuracy << (*it).mDodge << (*it).mAntiMagic << (*it).mAntiPoison << (*it).mHpRecover << (*it).mMpRecover << (*it).mPoisonRecover
            << (*it).mMabiProb << (*it).mMabiDura << (*it).mDixiaoPres << (*it).mFuyuanCd << (*it).mFuyuanPres << (*it).mMaxHp << (*it).mMaxMp << (*it).mMaxHpPres << (*it).mMaxMpPres << (*it).mHalfBaoji;
    }
    MESSAGE_CLASS_IMPORT_END
    MESSAGE_CLASS_EXPORT(ItemPlusDespNotifyGroup)
        int count = 0;
        stream >> count;
        if (count > 0) {
            data.items.resize(count);
            for (auto &it = data.items.begin(); it != data.items.end(); ++it) {
                stream >> (*it).mItemPlusID >> (*it).mTypeID
                    >> (*it).mNeedType >> (*it).mNeedParam1 >> (*it).mNeedParam2 >> (*it).mNeedParam3 >> (*it).mNeedParam4 >> (*it).mNeedParam5
                    >> (*it).mAC >> (*it).mACMax >> (*it).mMAC >> (*it).mMACMax >> (*it).mDC >> (*it).mDCMax >> (*it).mMC >> (*it).mMCMax >> (*it).mSC >> (*it).mSCMax
                    >> (*it).mLuck >> (*it).mCurse >> (*it).mAccuracy >> (*it).mDodge >> (*it).mAntiMagic >> (*it).mAntiPoison >> (*it).mHpRecover >> (*it).mMpRecover >> (*it).mPoisonRecover
                    >> (*it).mMabiProb >> (*it).mMabiDura >> (*it).mDixiaoPres >> (*it).mFuyuanCd >> (*it).mFuyuanPres >> (*it).mMaxHp >> (*it).mMaxMp >> (*it).mMaxHpPres >> (*it).mMaxMpPres >> (*it).mHalfBaoji;
            }
        }
    MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ItemDespNotify,GS_Client_Item_Desp_Notify)
		char mName[512];
		const char* mDesp2;
		int mTypeID;
		int mIconID;
		int mPrice;
		int mWeight;
		int mLastTime;
		int mDurationMax;
		int mNeedType;
		int mNeedParam;
		int mReinLv;
		int mColor;
		int mNotips;

		int mResMale;
		int mResFemale;

		int mAC;
		int mACMax;
		int mMAC;
		int mMACMax;
		int mDC;
		int mDCMax;
		int mMC;
		int mMCMax;
		int mSC;
		int mSCMax;
		int mLuck;
		int mCurse;
		int mAccuracy;
		int mDodge;
		int mAntiMagic;
		int mAntiPoison;
		int mHpRecover;
		int mMpRecover;
		int mPoisonRecover;

		int mMabiProb;
		int mMabiDura;
		int mDixiaoPres;
		int mFuyuanCd;
		int mFuyuanPres;
		int mMaxHp;
		int mMaxMp;
		int mMaxHpPres;
		int mMaxMpPres;
		int mNeedZsLevel;
		int mEquipLevel;
		int mEquipComp;
		int mEquipGroup;
		int mEquipContribute;
		int mShowDest;
		int mAddPower;
		int mItemBg;
		int mJob;
		int mGender;

		int SubType;
		int HPChange;
		int MPChange;
		int ZipType;
		int ZipNumber;

		int mBaoji;
		int mDrop_luck;
		int mStackMax;
		
		int mEquipType;
		int mXishou_prob;
		int mXishou_pres;
		int mFantan_prob;
		int mFantan_pres;
		int mBaoji_prob;
		int mBaoji_pres;
		int mXixue_prob;
		int mXixue_pres;

		int randac;
		int randmac;
		int randdc;
		int randmc;
		int randsc;
		int mRecycleExp;
		int mRecycleXuefu;
		int mCanUse;
		int mCanDestroy;
		int mCanDepot;
		int mCanPush;
		int mBagShow;
		int mTimesLimit;
		const char* mSource;
	MESSAGE_CLASS_INIT(ItemDespNotify)
		mTypeID=0;
		mDesp2="";
		mIconID=0;
		mPrice=0;
		mWeight=0;
		mLastTime=0;
		mDurationMax=0;
		mNeedType=0;
		mNeedParam=0;
		mReinLv=0;
		mColor=0;
		mNotips=1;

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
		mEquipContribute=0;
		mShowDest=0;
		mAddPower=0;
		mItemBg=0;

		mMabiProb=0;
		mMabiDura=0;
		mDixiaoPres=0;
		mFuyuanCd=0;
		mFuyuanPres=0;
		mMaxHp=0;
		mMaxMp=0;
		mMaxHpPres=0;
		mMaxMpPres=0;
		mNeedZsLevel=0;
		mEquipLevel=0;
		mEquipComp=0;
		mEquipGroup = 0;
		mJob=0;
		mGender=0;

		SubType=0;
		HPChange=0;
		MPChange=0;
		ZipType=0;
		ZipNumber=0;

		mBaoji=0;
		mDrop_luck=0;
		mName[0]=0;
		//mDesp[0]=0;
		mStackMax=0;

		mEquipType=0;
		mXishou_prob=0;
		mXishou_pres=0;
		mFantan_prob=0;
		mFantan_pres=0;
		mBaoji_prob=0;
		mBaoji_pres=0;
		mXixue_prob=0;
		mXixue_pres=0;

		randac=0;
		randmac=0;
		randdc=0;
		randmc=0;
		randsc=0;
		mRecycleExp=0;
		mRecycleXuefu=0;
		mCanUse = 0;
		mCanDestroy=0;
		mCanDepot=0;
		mCanPush = 0;
		mBagShow = 0;
		mTimesLimit = 0;
		mSource="";
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ItemDespNotify)
		stream<<data.mName<<data.mDesp2
				<<data.mTypeID<<data.mIconID<<data.mPrice<<data.mWeight<<data.mLastTime<<data.mDurationMax<<data.mNeedType<<data.mNeedParam<<data.mReinLv<<data.mColor<<data.mNotips
				<<data.mResMale<<data.mResFemale<<data.mAC<<data.mACMax<<data.mMAC<<data.mMACMax<<data.mDC<<data.mDCMax<<data.mMC<<data.mMCMax<<data.mSC<<data.mSCMax
				<<data.mLuck<<data.mCurse<<data.mAccuracy<<data.mDodge<<data.mAntiMagic<<data.mAntiPoison<<data.mHpRecover<<data.mMpRecover<<data.mPoisonRecover
				<<data.SubType<<data.HPChange<<data.MPChange<<data.ZipType<<data.ZipNumber
				<<data.mMabiProb<<data.mMabiDura<<data.mDixiaoPres<<data.mFuyuanCd<<data.mFuyuanPres<<data.mMaxHp<<data.mMaxMp<<data.mMaxHpPres<<data.mMaxMpPres<<data.mNeedZsLevel<<data.mEquipLevel<<data.mEquipComp<<data.mEquipGroup<<data.mEquipContribute<<data.mShowDest<<data.mAddPower<<data.mJob<<data.mGender<<data.mBaoji<<data.mDrop_luck<<data.mStackMax
				<<data.mEquipType<<data.mXishou_prob<<data.mXishou_pres<<data.mFantan_prob<<data.mFantan_pres<<data.mBaoji_prob<<data.mBaoji_pres<<data.mXixue_prob<<data.mXixue_pres
				<<data.randac<<data.randmac<<data.randdc<<data.randmc<<data.randsc<<data.mItemBg<<data.mRecycleExp<<data.mRecycleXuefu<<data.mCanUse<<data.mCanDestroy<<data.mCanDepot<<data.mCanPush<<data.mBagShow<<data.mTimesLimit<<data.mSource;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ItemDespNotify)
		stream>>data.mName>>data.mDesp2
				>>data.mTypeID>>data.mIconID>>data.mPrice>>data.mWeight>>data.mLastTime>>data.mDurationMax>>data.mNeedType>>data.mNeedParam>>data.mReinLv>>data.mColor>>data.mNotips
				>>data.mResMale>>data.mResFemale>>data.mAC>>data.mACMax>>data.mMAC>>data.mMACMax>>data.mDC>>data.mDCMax>>data.mMC>>data.mMCMax>>data.mSC>>data.mSCMax
				>>data.mLuck>>data.mCurse>>data.mAccuracy>>data.mDodge>>data.mAntiMagic>>data.mAntiPoison>>data.mHpRecover>>data.mMpRecover>>data.mPoisonRecover
				>>data.SubType>>data.HPChange>>data.MPChange>>data.ZipType>>data.ZipNumber
				>>data.mMabiProb>>data.mMabiDura>>data.mDixiaoPres>>data.mFuyuanCd>>data.mFuyuanPres>>data.mMaxHp>>data.mMaxMp>>data.mMaxHpPres>>data.mMaxMpPres>>data.mNeedZsLevel>>data.mEquipLevel>>data.mEquipComp>>data.mEquipGroup>>data.mEquipContribute>>data.mShowDest>>data.mAddPower>>data.mJob>>data.mGender>>data.mBaoji>>data.mDrop_luck>>data.mStackMax
				>>data.mEquipType>>data.mXishou_prob>>data.mXishou_pres>>data.mFantan_prob>>data.mFantan_pres>>data.mBaoji_prob>>data.mBaoji_pres>>data.mXixue_prob>>data.mXixue_pres
				>>data.randac>>data.randmac>>data.randdc>>data.randmc>>data.randsc>>data.mItemBg>>data.mRecycleExp>>data.mRecycleXuefu>>data.mCanUse>>data.mCanDestroy>>data.mCanDepot>>data.mCanPush>>data.mBagShow>>data.mTimesLimit>>data.mSource;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListItemDespNotify,GS_Client_List_Item_Desp_Notify)
		struct ItemDesp
		{
			char mName[512];
			const char* mDesp2;
			int mTypeID;
			int mIconID;
			int mPrice;
			int mWeight;
			int mLastTime;
			int mDurationMax;
			int mNeedType;
			int mNeedParam;
			int mReinLv;
			int mColor;
			int mNotips;

			int mResMale;
			int mResFemale;

			int mAC;
			int mACMax;
			int mMAC;
			int mMACMax;
			int mDC;
			int mDCMax;
			int mMC;
			int mMCMax;
			int mSC;
			int mSCMax;
			int mLuck;
			int mCurse;
			int mAccuracy;
			int mDodge;
			int mAntiMagic;
			int mAntiPoison;
			int mHpRecover;
			int mMpRecover;
			int mPoisonRecover;

			int mMabiProb;
			int mMabiDura;
			int mDixiaoPres;
			int mFuyuanCd;
			int mFuyuanPres;
			int mMaxHp;
			int mMaxMp;
			int mMaxHpPres;
			int mMaxMpPres;
			int mNeedZsLevel;
			int mEquipLevel;
			int mEquipComp;
			int mEquipGroup;
			int mEquipContribute;
			int mShowDest;
			int mAddPower;
			int mItemBg;
			int mJob;
			int mGender;

			int SubType;
			int HPChange;
			int MPChange;
			int ZipType;
			int ZipNumber;

			int mBaoji;
			int mDrop_luck;
			int mStackMax;
		
			int mEquipType;
			int mXishou_prob;
			int mXishou_pres;
			int mFantan_prob;
			int mFantan_pres;
			int mBaoji_prob;
			int mBaoji_pres;
			int mXixue_prob;
			int mXixue_pres;

			int randac;
			int randmac;
			int randdc;
			int randmc;
			int randsc;
			int mRecycleExp;
			int mRecycleXuefu;
			int mCanUse;
			int mCanDestroy;
			int mCanDepot;
			int mCanPush;
			int mBagShow;
			int mTimesLimit;
			const char* mSource;
		};
		std::vector<ListItemDespNotify::ItemDesp> m_ItemDefList;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListItemDespNotify)
	stream<<static_cast<int>(data.m_ItemDefList.size());
	for (std::vector<ListItemDespNotify::ItemDesp> ::const_iterator it = data.m_ItemDefList.begin();it!=data.m_ItemDefList.end();it++)
	{
		stream<<it->mName
			<<it->mDesp2
			<<it->mTypeID
			<<it->mIconID
			<<it->mPrice
			<<it->mWeight
			<<it->mLastTime
			<<it->mDurationMax
			<<it->mNeedType
			<<it->mNeedParam
			<<it->mReinLv
			<<it->mColor
			<<it->mNotips
			<<it->mResMale
			<<it->mResFemale
			<<it->mAC
			<<it->mACMax
			<<it->mMAC
			<<it->mMACMax
			<<it->mDC
			<<it->mDCMax
			<<it->mMC
			<<it->mMCMax
			<<it->mSC
			<<it->mSCMax
			<<it->mLuck
			<<it->mCurse
			<<it->mAccuracy
			<<it->mDodge
			<<it->mAntiMagic
			<<it->mAntiPoison
			<<it->mHpRecover
			<<it->mMpRecover
			<<it->mPoisonRecover
			<<it->SubType
			<<it->HPChange
			<<it->MPChange
			<<it->ZipType
			<<it->ZipNumber
			<<it->mMabiProb
			<<it->mMabiDura
			<<it->mDixiaoPres
			<<it->mFuyuanCd
			<<it->mFuyuanPres
			<<it->mMaxHp
			<<it->mMaxMp
			<<it->mMaxHpPres
			<<it->mMaxMpPres
			<<it->mNeedZsLevel
			<<it->mEquipLevel
			<<it->mEquipComp
			<<it->mEquipGroup
			<<it->mEquipContribute
			<<it->mShowDest
			<<it->mAddPower
			<<it->mJob
			<<it->mGender
			<<it->mBaoji
			<<it->mDrop_luck
			<<it->mStackMax
			<<it->mEquipType
			<<it->mXishou_prob
			<<it->mXishou_pres
			<<it->mFantan_prob
			<<it->mFantan_pres
			<<it->mBaoji_prob
			<<it->mBaoji_pres
			<<it->mXixue_prob
			<<it->mXixue_pres
			<<it->randac
			<<it->randmac
			<<it->randdc
			<<it->randmc
			<<it->randsc
			<<it->mItemBg
			<<it->mRecycleExp
			<<it->mRecycleXuefu
			<<it->mCanUse
			<<it->mCanDestroy
			<<it->mCanDepot
			<<it->mCanPush
			<<it->mBagShow
			<<it->mTimesLimit
			<<it->mSource;
	}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListItemDespNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			ListItemDespNotify::ItemDesp it;
			stream>>it->mName
				>>it->mDesp2
				>>it->mTypeID
				>>it->mIconID
				>>it->mPrice
				>>it->mWeight
				>>it->mLastTime
				>>it->mDurationMax
				>>it->mNeedType
				>>it->mNeedParam
				>>it->mReinLv
				>>it->mColor
				>>it->mNotips
				
				>>it->mResMale
				>>it->mResFemale
				>>it->mAC
				>>it->mACMax
				>>it->mMAC
				>>it->mMACMax
				>>it->mDC
				>>it->mDCMax
				>>it->mMC
				>>it->mMCMax
				>>it->mSC
				>>it->mSCMax

				>>it->mLuck
				>>it->mCurse
				>>it->mAccuracy
				>>it->mDodge
				>>it->mAntiMagic
				>>it->mAntiPoison
				>>it->mHpRecover
				>>it->mMpRecover
				>>it->mPoisonRecover

				>>it->SubType
				>>it->HPChange
				>>it->MPChange
				>>it->ZipType
				>>it->ZipNumber

				>>it->mMabiProb
				>>it->mMabiDura
				>>it->mDixiaoPres
				>>it->mFuyuanCd
				>>it->mFuyuanPres
				>>it->mMaxHp
				>>it->mMaxMp
				>>it->mMaxHpPres
				>>it->mMaxMpPres
				>>it->mNeedZsLevel
				>>it->mEquipLevel
				>>it->mEquipComp
				>>it->mEquipGroup
				>>it->mEquipContribute
				>>it->mShowDest
				>>it->mAddPower
				>>it->mJob
				>>it->mGender
				>>it->mBaoji
				>>it->mDrop_luck
				>>it->mStackMax

				>>it->mEquipType
				>>it->mXishou_prob
				>>it->mXishou_pres
				>>it->mFantan_prob
				>>it->mFantan_pres
				>>it->mBaoji_prob
				>>it->mBaoji_pres
				>>it->mXixue_prob
				>>it->mXixue_pres

				>>it->randac
				>>it->randmac
				>>it->randdc
				>>it->randmc
				>>it->randsc
				>>it->mItemBg
				>>it->mRecycleExp
				>>it->mRecycleXuefu
				>>it->mCanUse
				>>it->mCanDestroy
				>>it->mCanDepot
				>>it->mCanPush
				>>it->mBagShow
				>>it->mTimesLimit
				>>it->mSource;
			if( stream )
			{
				data.m_ItemDefList.push_back(it);
			}
			else
			{
				printf("ListGuildMemberRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListCharacterRes,GS_Client_ListCharacter)
		struct character
		{
			std::string name;
			std::string seedname;
			int lv;
			int job;
			int gender;
			int svrid;
			int online;
			int cloth;
			int weapon;
			int fashionCloth;
			int fashionWeapon;
			int wing;
		};
		int china_limit_tag;
		std::vector<character> charlist;
	MESSAGE_CLASS_INIT(ListCharacterRes)
		china_limit_tag = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListCharacterRes)
		stream<<data.china_limit_tag<<static_cast<int>(data.charlist.size());
		for( std::vector<ListCharacterRes::character>::const_iterator pos = data.charlist.begin();
			pos != data.charlist.end();
			++ pos )
		{
			stream<<pos->lv<<pos->job<<pos->gender<<pos->svrid<<pos->online<<pos->name<<pos->seedname<<pos->cloth<<pos->weapon<<pos->fashionCloth<<pos->fashionWeapon<<pos->wing;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListCharacterRes)
		int size=0;
		stream>>data.china_limit_tag>>size;
		for(int i = 0;i<size;i++)
		{
			ListCharacterRes::character c;
			stream>>c.lv>>c.job>>c.gender>>c.svrid>>c.online>>c.name>>c.seedname>>c->cloth>>c->weapon>>c->fashionCloth>>c->fashionWeapon>>c->wing;
			if( stream )
			{
				data.charlist.push_back(c);
			}
			else
			{
				printf("ListCharacterRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CreateCharacterRes,GS_Client_CreateCharacter)
		int result;
	    char seedname[512];
		enum result_type
		{
			result_type_succes=100,
			result_type_fail_system=101,
			result_type_too_many=102,
			result_type_name_existed=103,
			result_type_name_filter=104,
		};
	MESSAGE_CLASS_INIT(CreateCharacterRes)
	    seedname[0]=0;
		result=result_type_fail_system;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CreateCharacterRes)
		stream<<data.result<<data.seedname;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CreateCharacterRes)
		stream>>data.result>>data.seedname;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(EnterGameRes,GS_Client_EnterGame)
		int result;
		enum result_type
		{
			result_type_succes=100,
			result_type_fail_system=101,
			result_type_fail_auth_type=102,
			result_type_fail_double_enter=103,
			result_type_fail_load_error=104,
			result_type_fail_deny=105,
		};
	MESSAGE_CLASS_INIT(EnterGameRes)
		result=result_type_fail_system;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(EnterGameRes)
		stream<<data.result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(EnterGameRes)
		stream>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DeleteCharacterRes,GS_Client_DeleteCharacter)
		int result;
		std::string chr_seed;
		std::string guild_seed;
		enum result_type
		{
			result_type_succes=100,
			result_type_fail_system=101,
		};
	MESSAGE_CLASS_INIT(DeleteCharacterRes)
		result=result_type_fail_system;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DeleteCharacterRes)
		stream<<data.result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DeleteCharacterRes)
		stream>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NpcSellRes,GS_Client_NPCSell)
		unsigned int npc_id;
		int result;
	MESSAGE_CLASS_INIT(NpcSellRes)
		npc_id = 0;
		result = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NpcSellRes)
		stream<<data.npc_id<<data.result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NpcSellRes)
		stream>>data.npc_id>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DropItemRes,GS_Client_DropItem)
		int type_id;
		int result;
	MESSAGE_CLASS_INIT(DropItemRes)
		type_id = 0;
		result = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DropItemRes)
		stream<<data.type_id<<data.result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DropItemRes)
		stream>>data.type_id>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(LoadShortcutNotify,GS_Client_LoadShortcut)
		struct shortcut
		{
			shortcut()
			{
				shortcut_id = 0;
				type = 0;
				param = 0;
			}
			int shortcut_id;
			int type;
			int param;
		};
		std::vector<shortcut> shortcuts;
	MESSAGE_CLASS_INIT(LoadShortcutNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(LoadShortcutNotify)
		stream << static_cast<int>(data.shortcuts.size());
		for( std::vector<LoadShortcutNotify::shortcut>::const_iterator pos = data.shortcuts.begin();
			pos != data.shortcuts.end();
			++ pos )
		{
			stream<<pos->shortcut_id<<pos->type<<pos->param;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(LoadShortcutNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			LoadShortcutNotify::shortcut sc;
			stream>>sc.shortcut_id>>sc.type>>sc.param;
			if( stream )
			{
				data.shortcuts.push_back(sc);
			}
			else
			{
				printf("LoadShortcutNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildRes,GS_Client_ListGuild)
		struct guild_info
		{
			guild_info()
			{
				member_number = 0;
				master_level = 0;
			}
			std::string name;
			int member_number;
			std::string desp;
			int master_level;
		};
		std::vector<guild_info> guildinfos;
	MESSAGE_CLASS_INIT(ListGuildRes)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildRes)
		stream << static_cast<int>(data.guildinfos.size());
		for( std::vector<ListGuildRes::guild_info>::const_iterator pos = data.guildinfos.begin();
			pos != data.guildinfos.end();
			++ pos )
		{
			stream<<pos->name<<pos->member_number<<pos->desp<<pos->master_level;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildRes)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			ListGuildRes::guild_info gi;
			stream>>gi.name>>gi.member_number>>gi.desp>>gi.master_level;
			if( stream )
			{
				data.guildinfos.push_back(gi);
			}
			else
			{
				printf("ListGuildRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildBeginNotify,GS_Client_List_Guild_Begin)
	MESSAGE_CLASS_INIT(ListGuildBeginNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildBeginNotify)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildBeginNotify)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildEndNotify,GS_Client_List_Guild_End)
	MESSAGE_CLASS_INIT(ListGuildEndNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildEndNotify)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildEndNotify)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildItemNotify,GS_Client_List_Guild_Item)
		std::string name;
		std::string seed_id;
		std::string leader_name;
		int member_number;
		std::string desp;
		int master_level;
		int entering;
		int guild_level;
		int guild_exp;
		int war_state;
		int war_start_time;
	MESSAGE_CLASS_INIT(ListGuildItemNotify)
		member_number = 0;
		master_level = 0;
		entering = 0;
		guild_level=1;
		guild_exp=0;
		war_state=0;
		war_start_time=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildItemNotify)
		stream<<data.name<<data.seed_id<<data.member_number<<data.desp<<data.master_level<<data.entering<<data.leader_name<<data.guild_level<<data.guild_exp<<data.war_state<<data.war_start_time;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildItemNotify)
		stream>>data.name>>data.seed_id>>data.member_number>>data.desp>>data.master_level>>data.entering>>data.leader_name>>data.guild_level>>data.guild_exp>>data.war_state>>data.war_start_time;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GetGuildInfoRes,GS_Client_GetGuildInfo)
		char name[512];
		int member_number;
		char desp[512];
		char notice[512];
		char leader_name[512];
		int guild_level;
		int guild_exp;
		char seed_id[512];
	MESSAGE_CLASS_INIT(GetGuildInfoRes)
		member_number = 0;
		 name[0]=0;
		 desp[0]=0;
		 notice[0]=0;
		 leader_name[0]=0;
		 guild_level = 1;
		 guild_exp = 0;
		 seed_id[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GetGuildInfoRes)
		stream<<data.name<<data.member_number<<data.desp<<data.notice<<data.leader_name<<data.guild_level<<data.guild_exp<<data.seed_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GetGuildInfoRes)
		stream>>data.name>>data.member_number>>data.desp>>data.notice>>data.leader_name>>data.guild_level>>data.guild_exp>>data.seed_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GuildInfoNotify,GS_Client_Guild_Info_Notify)
		char name[512];
		char guild_id[512];
		int title;
	MESSAGE_CLASS_INIT(GuildInfoNotify)
		title = 0;
		name[0]=0;
		guild_id[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildInfoNotify)
		stream<<data.name<<data.title<<data.guild_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GuildInfoNotify)
		stream>>data.name>>data.title>>data.guild_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GhostGuildInfo,GS_Client_Ghost_Guild_Info)
		unsigned int srcid;
		char guild_name[512];
		char guild_id[512];
		int title;
	MESSAGE_CLASS_INIT(GhostGuildInfo)
		guild_name[0]=0;
		guild_id[0]=0;
		srcid = 0;
		title = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GhostGuildInfo)
		stream<<data.srcid<<data.guild_name<<data.title<<data.guild_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GhostGuildInfo)
		stream>>data.srcid>>data.guild_name>>data.title>>data.guild_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildMemberRes,GS_Client_ListGuildMember)
		char guild_name[512];
		int list_type;
		struct guild_member_info
		{
			guild_member_info()
			{
				title = 0;
				online = 0;
				gender = 0;
				job = 0;
				level = 0;
				fight = 0;
				guildpt = 0;
				entertime = 0;
			}
			std::string name;
			short title;
			short online;
			short gender;
			short job;
			short level;
			int fight;
			int guildpt;
			int entertime;
		};
		std::vector<guild_member_info> guild_members;
	MESSAGE_CLASS_INIT(ListGuildMemberRes)
		guild_name[0]=0;
		list_type = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildMemberRes)
		stream << data.guild_name << data.list_type << static_cast<int>(data.guild_members.size());
		for( std::vector<ListGuildMemberRes::guild_member_info>::const_iterator pos = data.guild_members.begin();
			pos != data.guild_members.end();
			++ pos )
		{
			stream<<pos->name<<pos->title<<pos->online<<pos->gender<<pos->job<<pos->level<<pos->fight<<pos->guildpt<<pos->entertime;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildMemberRes)
		int size=0;
		stream>>data.guild_name>>data.list_type>>size;
		for( int i = 0;i<size;i++ )
		{
			ListGuildMemberRes::guild_member_info gi;
			stream>>gi.name>>gi.title>>gi.online>>gi.gender>>gi.job>>gi.level>>gi.fight>>gi.guildpt>>gi.entertime;
			if( stream )
			{
				data.guild_members.push_back(gi);
			}
			else
			{
				printf("ListGuildMemberRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListGuildItemRes,GS_Client_ListGuildItem)
		struct guild_item
		{
			guild_item()
			{
				pos = 0;
				typeID = 0;
				level = 0;
				zlevel = 0;
				price= 0;
				job = 0;
			}
			int pos;
			int typeID;
			int level;
			int zlevel;
			int price;
			int job;
		};
		std::vector<guild_item> guild_items;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListGuildItemRes)
		stream << static_cast<int>(data.guild_items.size());
		for( std::vector<ListGuildItemRes::guild_item>::const_iterator pos = data.guild_items.begin();
			pos != data.guild_items.end();
			++ pos )
		{
			stream<<pos->pos<<pos->typeID<<pos->level<<pos->zlevel<<pos->price<<pos->job;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListGuildItemRes)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			ListGuildItemRes::guild_item gi;
			stream>>gi.pos>>gi.typeID>>gi.level>>gi.zlevel>>pos->price>>pos->job;
			if( stream )
			{
				data.guild_items.push_back(gi);
			}
			else
			{
				printf("ListGuildItemRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(ListGuildEnemyRes,GS_Client_ListGuildEnemy)
//		std::string guild_name;
//		std::vector<std::string> guild_enemys;
//	MESSAGE_CLASS_INIT(ListGuildEnemyRes)
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(ListGuildEnemyRes)
//		stream << data.guild_name << static_cast<int>(data.guild_enemys.size());
//		for( std::vector<std::string>::const_iterator pos = data.guild_enemys.begin();
//			pos != data.guild_enemys.end();
//			++ pos )
//		{
//			stream<<*pos;
//		}
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(ListGuildEnemyRes)
//		int size=0;
//		stream>>data.guild_name>>size;
//		for( int i = 0;i<size;i++ )
//		{
//			std::string name;
//			stream>>name;
//			data.guild_enemys.push_back(name);
//		}
//	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(ListGuildFriendRes,GS_Client_ListGuildFriend)
//		std::string guild_name;
//		std::vector<std::string> guild_friends;
//	MESSAGE_CLASS_INIT(ListGuildFriendRes)
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(ListGuildFriendRes)
//		stream << data.guild_name << static_cast<int>(data.guild_friends.size());
//		for( std::vector<std::string>::const_iterator pos = data.guild_friends.begin();
//			pos != data.guild_friends.end();
//			++ pos )
//		{
//			stream<<*pos;
//		}
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(ListGuildFriendRes)
//		int size=0;
//		stream>>data.guild_name>>size;
//		for( int i = 0;i<size;i++ )
//		{
//			std::string name;
//			stream>>name;
//			data.guild_friends.push_back(name);
//		}
//	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GhostModeNotify,GS_Client_GhostMode_Notify)
		int ghost_mode;
	MESSAGE_CLASS_INIT(GhostModeNotify)
		ghost_mode = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GhostModeNotify)
		stream<<data.ghost_mode;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GhostModeNotify)
		stream>>data.ghost_mode;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AlertNotify,GS_Client_Alert_Notify)
		int lv;
		int flags;
		char msg[1024];
	MESSAGE_CLASS_INIT(AlertNotify)
		lv = 0;
		flags = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AlertNotify)
		stream<<data.lv<<data.flags<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AlertNotify)
		stream>>data.lv>>data.flags>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ReliveNotify,GS_Client_Relive_Notify)
		unsigned int src;
		int type;
	MESSAGE_CLASS_INIT(ReliveNotify)
		src = 0;
		type = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ReliveNotify)
		stream<<data.src<<data.type;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ReliveNotify)
		stream>>data.src>>data.type;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupInfoChangeNotify,GS_Client_Group_Info_Change_Notify)
		int group_id;
		int pick_mode;
		std::string name;
		std::string leader;
		struct member
		{
			int src;
			std::string nick_name;
		};
		typedef std::vector<member> membervector;
		membervector members;
	MESSAGE_CLASS_INIT(GroupInfoChangeNotify)
		group_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupInfoChangeNotify)
		stream<<data.group_id<<data.pick_mode<<data.name<<data.leader;
		stream<<static_cast<int>(data.members.size());
		for( GroupInfoChangeNotify::membervector::const_iterator pos = data.members.begin();
			pos != data.members.end();
			++ pos )
		{
			stream<<pos->src;
			stream<<pos->nick_name;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupInfoChangeNotify)
		stream>>data.group_id>>data.pick_mode>>data.name>>data.leader;
		int num = 0;
		stream>>num;
		for(int i =0;i <num;i++)
		{
			GroupInfoChangeNotify::member mb;
			stream>>mb.src;
			stream>>mb.nick_name;
			if( stream )
			{
				data.members.push_back(mb);
			}
			else
			{
				printf("GroupInfoChangeNotify for size out stream :%d\n",num);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupStateNotify,GS_Client_Group_State_Notify)
		int group_id;
		enum state
		{
			MS_OFFLINE=100,
			MS_IDLE=101,
			MS_DEAD=102,
		};
		struct member
		{
			member()
			{
				state = MS_OFFLINE;
				hp = 0;
				mp = 0;
				job = 0;
				level = 0;
				power = 0;
				cur_map = "";
				gender = 0;
			}
			int state;
			int hp;
			int mp;
			int job;
			int level;
			int power;
			int gender;
			std::string cur_map;
		};
		typedef std::vector<member> membervector;
		membervector members;
	MESSAGE_CLASS_INIT(GroupStateNotify)
		group_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupStateNotify)
		stream<<data.group_id;
		stream<<static_cast<int>(data.members.size());
		for( GroupStateNotify::membervector::const_iterator pos = data.members.begin();
			pos != data.members.end();
			++ pos )
		{
			stream<<pos->state<<pos->hp<<pos->mp<<pos->job<<pos->level<<pos->power<<pos->cur_map<<pos->gender;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupStateNotify)
		stream>>data.group_id;
		int num = 0;
		stream>>num;
		for(int i =0;i <num;i++)
		{
			GroupStateNotify::member mb;
			stream>>mb.state>>mb.hp>>mb.mp>>mb.job>>mb.level>>mb.power>>mb.cur_map>>mb.gender;
			if( stream )
			{
				data.members.push_back(mb);
			}
			else
			{
				printf("GroupStateNotify for size out stream :%d\n",num);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ChangeAttackModeRes,GS_Client_ChangeAttackMode)
		int attack_mode;
	MESSAGE_CLASS_INIT(ChangeAttackModeRes)
		attack_mode = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChangeAttackModeRes)
		stream<<data.attack_mode;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChangeAttackModeRes)
		stream>>data.attack_mode;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupInfoNotify,GS_Client_Group_Info_Notify)
		unsigned int src_id;
		int group_id;
		char group_leader[512];
		char group_name[512];
	MESSAGE_CLASS_INIT(GroupInfoNotify)
		group_leader[0]=0;
		group_name[0]=0;
		src_id = 0;
		group_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupInfoNotify)
		stream<<data.src_id<<data.group_id<<data.group_name<<data.group_leader;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupInfoNotify)
		stream>>data.src_id>>data.group_id>>data.group_name>>data.group_leader;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SkillDespNotify,GS_Client_Skill_Desp_Notify)
		int skill_id;
		char mName[512];
		char mDesp[512];
		int icon_id;
		int skill_level;
		int short_cut;
		int sound;
		int effect_type;
		int effect_resID;
		int levelmax;
		int range;
		int min_dis;
		int max_dis;
		/*int lv1_train;
		int lv1_need;
		int lv2_train;
		int lv2_need;
		int lv3_train;
		int lv3_need;*/
		int spell_def;
		int spell;
		int consume_mp;

		int isShow;
		int cast_way;
		int select_target_way;
		int skill_cd;
		int public_cd;
		int order_id;
		int need_level;
		int need_exp;

		char damage_desp[512];
		char range_desp[512];
		char cd_desp[512];
		char exteffect_desp[512];
		
		//下级技能信息
		char damage_desp_next[512];
		char range_desp_next[512];
		char cd_desp_next[512];
		char exteffect_desp_next[512];
		int damageEffect;
	MESSAGE_CLASS_INIT(SkillDespNotify)
		skill_id=0;
		mName[0]=0;
		mDesp[0]=0;
	    icon_id=0;
		skill_level=0;
		short_cut=0;
		sound=0;
		effect_type=0;
		effect_resID=0;
		levelmax=0;
		range=0;
		min_dis=0;
		max_dis=0;
		/*lv1_train=0;
		lv1_need=0;
		lv2_train=0;
		lv2_need=0;
		lv3_train=0;
		lv3_need=0;*/
		spell_def=0;
		spell = 0;
		consume_mp=0;

		isShow = 1;
		cast_way = 0;
		select_target_way = 0;
		skill_cd = 0;
		public_cd = 0;
		order_id = 0;
		need_level = 0;
		need_exp = 0;

		damage_desp[0]=0;
		range_desp[0]=0;
		cd_desp[0]=0;
		exteffect_desp[0]=0;

		damage_desp_next[0]=0;
		range_desp_next[0]=0;
		cd_desp_next[0]=0;
		exteffect_desp_next[0]=0;
		damageEffect = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SkillDespNotify)
		stream<<data.skill_id<<data.mName<<data.mDesp<<data.icon_id<<data.skill_level<<data.levelmax<<data.short_cut<<data.effect_type<<data.effect_resID
		<<data.spell_def<<data.spell<<data.consume_mp<<data.range<<data.min_dis<<data.max_dis<<data.sound<<data.isShow<<data.cast_way<<data.select_target_way<<data.skill_cd<<data.public_cd<<data.order_id<<data.need_level<<data.need_exp
		<<data.damage_desp<<data.range_desp<<data.cd_desp<<data.exteffect_desp<<data.damage_desp_next<<data.range_desp_next<<data.cd_desp_next<<data.exteffect_desp_next<<data.damageEffect;
		//<<data.lv1_need<<data.lv1_train<<data.lv2_need<<data.lv2_train<<data.lv3_need<<data.lv3_train;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SkillDespNotify)
		stream>>data.skill_id>>data.mName>>data.mDesp>>data.icon_id>>data.skill_level>>data.levelmax>>data.short_cut>>data.effect_type>>data.effect_resID
		>>data.spell_def>>data.spell>>data.consume_mp>>data.range>>data.min_dis>>data.max_dis>>data.sound>>data.isShow>>data.cast_way>>data.select_target_way>>data.skill_cd>>data.public_cd>>data.order_id>>data.need_level>>data.need_exp
		>>data.damage_desp>>data.range_desp>>data.cd_desp>>data.exteffect_desp>>data.damage_desp_next>>data.range_desp_next>>data.cd_desp_next>>data.exteffect_desp_next>>data.damageEffect;
		//>>data.lv1_need>>data.lv1_train>>data.lv2_need>>data.lv2_train>>data.lv3_need>>data.lv3_train;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(YouKeSessionIDNotify,GS_Client_YouKe_SessionID_Notify)
		char SessionID[512];
	MESSAGE_CLASS_INIT(YouKeSessionIDNotify)
		SessionID[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(YouKeSessionIDNotify)
		stream<<data.SessionID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(YouKeSessionIDNotify)
		stream>>data.SessionID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(VcoinShopListRes,GS_Client_VcoinShopList)
		unsigned int shop_npc_id;
	MESSAGE_CLASS_INIT(VcoinShopListRes)
		shop_npc_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(VcoinShopListRes)
		stream<<data.shop_npc_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(VcoinShopListRes)
		stream>>data.shop_npc_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapConnNotify,GS_Client_Map_Conn_Notify)
		char map_id[512];
		char des_map_id[512];
		char des_map_name[512];
		int from_x;
		int from_y;
		int to_x;
		int to_y;
		int size;
	MESSAGE_CLASS_INIT(MapConnNotify)
		map_id[0]=0;
		des_map_id[0]=0;
		des_map_name[0]=0;
		from_x=0;
		from_y=0;
		to_x=0;
		to_y=0;
		size=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapConnNotify)
		stream<<data.map_id<<data.des_map_id<<data.des_map_name<<data.from_x<<data.from_y<<data.to_x<<data.to_y<<data.size;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapConnNotify)
		stream>>data.map_id>>data.des_map_id>>data.des_map_name>>data.from_x>>data.from_y>>data.to_x>>data.to_y>>data.size;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapSafeAreaNotify,GS_Client_Map_Safe_Area_Notify)
		char map_id[512];
		int x;
		int y;
		int size;
	MESSAGE_CLASS_INIT(MapSafeAreaNotify)
		map_id[0]=0;
		x=0;
		y=0;
		size=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapSafeAreaNotify)
		stream<<data.map_id<<data.x<<data.y<<data.size;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapSafeAreaNotify)
		stream>>data.map_id>>data.x>>data.y>>data.size;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TaskDespRes,GS_Client_Task_Desp)
		int task_id;
		short task_state;
		const char* desp;
	MESSAGE_CLASS_INIT(TaskDespRes)
		task_id=0;
		task_state=0;
		desp = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TaskDespRes)
		stream<<data.task_id<<data.task_state<<data.desp;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TaskDespRes)
		stream>>data.task_id>>data.task_state>>data.desp;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NpcShowFlagsNotify,GS_Client_Npc_Show_Flags_Notify)
		unsigned int npc_id;
		int flags;
	MESSAGE_CLASS_INIT(NpcShowFlagsNotify)
		npc_id=0;
		flags=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NpcShowFlagsNotify)
		stream<<data.npc_id<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NpcShowFlagsNotify)
		stream>>data.npc_id>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(InfoPlayerRes,GS_Client_Info_Player)
		unsigned int player_id;
		char name[512];
		char loverName[512];
		char guild[512];
		int job;
		int gender;
		int maxpower;
		int level;
		int viplevel;
		int mountlv;
		int winglv;
	MESSAGE_CLASS_INIT(InfoPlayerRes)
		player_id=0;
		gender=0;
		name[0]=0;
		loverName[0]=0;
		guild[0]=0;
		job = 0;
		maxpower = 0;
		level = 0;
		viplevel = 0;
		mountlv = 0;
		winglv = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(InfoPlayerRes)
	stream << data.player_id << data.name << data.loverName << data.guild << data.job << data.gender << data.maxpower << data.level << data.viplevel << data.mountlv << data.winglv;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(InfoPlayerRes)
	stream >> data.player_id >> data.name >> data.loverName >> data.guild >> data.job >> data.gender >> data.maxpower >> data.level >> data.viplevel >> data.mountlv >> data.winglv;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(JoinGroupToLeaderNotify,GS_Client_Join_Group_To_Leader_Notify)
		char player_nick_name[512];
	MESSAGE_CLASS_INIT(JoinGroupToLeaderNotify)
		player_nick_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(JoinGroupToLeaderNotify)
		stream<<data.player_nick_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(JoinGroupToLeaderNotify)
		stream>>data.player_nick_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(InviteGroupToMemberNotify,GS_Client_Invite_Group_To_Member_Notify)
		char leader_name[511];
		int group_id;
	MESSAGE_CLASS_INIT(InviteGroupToMemberNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(InviteGroupToMemberNotify)
		stream<<data.leader_name<<data.group_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(InviteGroupToMemberNotify)
		stream>>data.leader_name>>data.group_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FindRoadGotoNotify,GS_Client_Find_Road_Goto_Notify)
		char map[512];
		int x;
		int y;
		char target[512];
		int flags;
	MESSAGE_CLASS_INIT(FindRoadGotoNotify)
		x=0;
		y=0;
		flags=0;
		map[0]=0;
		target[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FindRoadGotoNotify)
		stream<<data.map<<data.x<<data.y<<data.target<<data.flags;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FindRoadGotoNotify)
		stream>>data.map>>data.x>>data.y>>data.target>>data.flags;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupChatNotify,GS_Client_Group_Chat_Notify)
	int srcid;
	int lv;
	int gender;
	int job;
	int vip;
	char guild[512];
	char name[512];
	char msg[512];
	MESSAGE_CLASS_INIT(GroupChatNotify)
	name[0]=0;
	msg[0]=0;
	guild[0]=0;
	srcid = 0;
	lv = 0;
	gender = 0;
	job = 0;
	vip = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupChatNotify)
		stream<<data.srcid<<data.name<<data.lv<<data.gender<<data.job<<data.vip<<data.guild<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupChatNotify)
		stream>>data.srcid>>data.name>>data.lv>>data.gender>>data.job>>data.vip>>data.guild>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NormalChatNotify,GS_Client_Normal_Chat_Notify)
	int srcid;
	int lv;
	int gender;
	int job;
	int vip;
	char guild[512];
	char name[512];
	char msg[512];
	MESSAGE_CLASS_INIT(NormalChatNotify)
	name[0]=0;
	msg[0]=0;
	guild[0]=0;
	srcid = 0;
	lv = 0;
	gender = 0;
	job = 0;
	vip = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NormalChatNotify)
		stream<<data.srcid<<data.name<<data.lv<<data.gender<<data.job<<data.vip<<data.guild<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NormalChatNotify)
		stream>>data.srcid>>data.name>>data.lv>>data.gender>>data.job>>data.vip>>data.guild>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeInviteNotify,GS_Client_Trade_Invite_Notify)
		char inviter[512];
	MESSAGE_CLASS_INIT(TradeInviteNotify)
		inviter[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeInviteNotify)
		stream<<data.inviter;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeInviteNotify)
		stream>>data.inviter;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TradeInfoNotify,GS_Client_Trade_Info_Notify)
		int gamemoney;
		int vcoin;
		int submit;
		char des_name[512];
		int des_gamemoney;
		int des_vcoin;
		int des_submit;
		int des_level;
		int trade_result;
	MESSAGE_CLASS_INIT(TradeInfoNotify)
		gamemoney=0;
		vcoin=0;
		submit=0;
		des_gamemoney=0;
		des_vcoin=0;
		des_submit=0;
		des_level = 0;
		des_name[0]=0;
		trade_result =0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TradeInfoNotify)
		stream<<data.gamemoney<<data.vcoin<<data.submit<<data.des_name<<data.des_gamemoney<<data.des_vcoin<<data.des_submit<<data.des_level<<data.trade_result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TradeInfoNotify)
		stream>>data.gamemoney>>data.vcoin>>data.submit>>data.des_name>>data.des_gamemoney>>data.des_vcoin>>data.des_submit>>data.des_level>>data.trade_result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapItemOwnerNotify,GS_Client_Map_Item_Owner_Notify)
		int id;
		int owner;
		int item_type;
	MESSAGE_CLASS_INIT(MapItemOwnerNotify)
		id=0;
		owner=0;
		item_type=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapItemOwnerNotify)
		stream<<data.id<<data.owner<<data.item_type;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapItemOwnerNotify)
		stream>>data.id>>data.owner>>data.item_type;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PKStateNotify,GS_Client_PK_State_Notify)
		unsigned int src_id;
		int pk_value;
		int pk_state;
	MESSAGE_CLASS_INIT(PKStateNotify)
		src_id=0;
		pk_value=0;
		pk_state=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PKStateNotify)
		stream<<data.src_id<<data.pk_value<<data.pk_state;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PKStateNotify)
		stream>>data.src_id>>data.pk_value>>data.pk_state;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MonsterAddInfo,GS_Client_Monster_Add_Info_Notify)
		unsigned int src_id;
		unsigned short lv;
		int collect_time;
		int state;
		int disapear_time;
		int isboss;
		int owner;
	MESSAGE_CLASS_INIT(MonsterAddInfo)
		src_id=0;
		lv=0;
		collect_time=0;
		state = 0;
		disapear_time = 8;
		isboss = 0;
		owner = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MonsterAddInfo)
		stream<<data.src_id<<data.lv<<data.collect_time<<data.state<<data.disapear_time<<data.isboss<<data.owner;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MonsterAddInfo)
	stream >> data.src_id >> data.lv >> data.collect_time >> data.state >> data.disapear_time >> data.isboss >> data.owner;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(StatusChangeNotify,GS_Client_Status_Change_Notify)
		unsigned int src_id;
		unsigned short status_id;
		int status_param;
		double status_time;
		int gap;
	MESSAGE_CLASS_INIT(StatusChangeNotify)
		src_id=0;
		status_id=0;
		status_time = 0;
		status_param = 0;
		gap = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(StatusChangeNotify)
		stream<<data.src_id<<data.status_id<<data.status_time<<data.status_param<<data.gap;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(StatusChangeNotify)
		stream>>data.src_id>>data.status_id>>data.status_time>>data.status_param>>data.gap;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ItemTalkNotify,GS_Client_Item_Talk_Notify)
		int item_id;
		int seed;
		const char* title;
		const std::string* msg;
	MESSAGE_CLASS_INIT(ItemTalkNotify)
		item_id=0;
		seed=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ItemTalkNotify)
		stream<<data.item_id<<data.seed<<data.title<<(*data.msg);
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ItemTalkNotify)
		std::string title;
		std::string msg;
		stream>>data.item_id>>data.seed>>title>>msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PlayerAddInfoNotify,GS_Client_Player_Add_Info_Notify)
		unsigned int id;
		unsigned short lv;
		unsigned char job;
		unsigned char gender;
		int maxhp;
		int hp;
		int maxmp;
		int mp;
		unsigned char low;
	MESSAGE_CLASS_INIT(PlayerAddInfoNotify)
		id =0;
		lv = 0;
		job = 0;
		gender = 0;
		maxhp = 0;
		hp = 0;
		low= 0;
		maxmp = 0;
		mp = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PlayerAddInfoNotify)
		stream<<data.id<<data.lv<<data.job<<data.gender<<data.maxhp<<data.hp<<data.maxmp<<data.mp<<data.low;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PlayerAddInfoNotify)
		stream>>data.id>>data.lv>>data.job>>data.gender>>data.maxhp>>data.hp>>data.maxmp>>data.mp>>data.low;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CountDownNotify,GS_Client_Count_Down_Notify)
		int delay;
		std::string msg;
	MESSAGE_CLASS_INIT(CountDownNotify)
		delay = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CountDownNotify)
		stream<<data.delay<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CountDownNotify)
		stream>>data.delay>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MiniMapConnNotify,GS_Client_Mini_Map_Conn_Notify)
		struct conn
		{
			char from_id[512];
			char to_id[512];
			conn()
			{
				from_id[0]=0;
				to_id[0]=0;
			}
		};
		std::vector<conn> conns;
	MESSAGE_CLASS_INIT(MiniMapConnNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MiniMapConnNotify)
		stream << static_cast<int>(data.conns.size());
		for( std::vector<MiniMapConnNotify::conn>::const_iterator pos = data.conns.begin();
			pos != data.conns.end();
			++ pos )
		{
			stream<<pos->from_id<<pos->to_id;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MiniMapConnNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			MiniMapConnNotify::conn _conn;
			stream>>_conn.from_id<<_conn.to_id;

			data.conns.push_back(_conn);
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PlayEffectNotify,GS_Client_Play_Effect_Notify)
		unsigned int src_id;
		int effect_id;
		unsigned short x;
		unsigned short y;
		unsigned int target_id;
		unsigned int owner_id;
		int resID;
	MESSAGE_CLASS_INIT(PlayEffectNotify)
		src_id = 0;
		effect_id = 0;
		x = 0;
		y = 0;
		target_id = 0;
		owner_id = 0;
		resID = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PlayEffectNotify)
		stream<<data.src_id<<data.effect_id<<data.x<<data.y<<data.target_id<<data.owner_id<<data.resID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PlayEffectNotify)
		stream>>data.src_id>>data.effect_id>>data.x>>data.y>>data.target_id>>data.owner_id>>data.resID;
	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GameParamNotify,GS_Client_Game_Param_Notify)
		//int steel_equip_cost_base;
		//int steel_equip_cost_mul;
		int max_magic_anti;
		int walk_speed_warrior_client_param;
		int stand_relive_price;
		int chart_open_limit_level;
		int add_depot_price;
		//int exchange_upd_prob_base;
		//int exchange_upd_prob_gap;
		//int exchange_upd_drop_max;
		//int exchange_upd_cost_gm;
		//int exchange_upd_cost_bv;

		//int mStatusQiseshendanAC;
		//int mStatusQiseshendanACMax;
		//int mStatusQiseshendanMAC;
		//int mStatusQiseshendanMACMax;
		//int mStatusQiseshendanDC;
		//int mStatusQiseshendanDCMax;
		//int mStatusQiseshendanMC;
		//int mStatusQiseshendanMCMax;
		//int mStatusQiseshendanSC;
		//int mStatusQiseshendanSCMax;
		//int mStatusQiseshendanHpmaxBase;
		//int mStatusQiseshendanHpmaxGap;
		//int mStatusQiseshendanMpmaxBase;
		//int mStatusQiseshendanMpmaxGap;

		//int mStatusXinFaHPMax;
		//int mStatusXinFaXishou;
		//int mStatusXinFaAC;
		//int mStatusXinFaACMax;
		//int mStatusXinFaMAC;
		//int mStatusXinFaMACMax;
		//int mStatusXinFaDC;
		//int mStatusXinFaDCMax;
		//int mStatusXinFaMC;
		//int mStatusXinFaMCMax;
		//int mStatusXinFaSC;
		//int mStatusXinFaSCMax;

		//int mStatusYuanshenhutiAC;
		//int mStatusYuanshenhutiACMax;
		//int mStatusYuanshenhutiMAC;
		//int mStatusYuanshenhutiMACMax;
		//int mStatusYuanshenhutiDC;
		//int mStatusYuanshenhutiDCMax;
		//int mStatusYuanshenhutiMC;
		//int mStatusYuanshenhutiMCMax;
		//int mStatusYuanshenhutiSC;
		//int mStatusYuanshenhutiSCMax;

		//int mStatusFuQiTongXinAC;
		//int mStatusFuQiTongXinACMax;
		//int mStatusFuQiTongXinMAC;
		//int mStatusFuQiTongXinMACMax;

		//int mStatusTianshenhutiMAXHP;
		//int mStatusTianshenhutiDC;
		//int mStatusTianshenhutiDCMax;
		//int mStatusTianshenhutiMC;
		//int mStatusTianshenhutiMCMax;
		//int mStatusTianshenhutiSC;
		//int mStatusTianshenhutiSCMax;
		//int mStatusTianshenhutiSubDamageProb;
		//int mStatusTianshenhutiSubDamagePres;

		//int mStatusBaqihutiAC;
		//int mStatusBaqihutiACMax;
		//int mStatusBaqihutiMAC;
		//int mStatusBaqihutiMACMax;
		//int mStatusBaqihutiDC;
		//int mStatusBaqihutiDCMax;
		//int mStatusBaqihutiMC;
		//int mStatusBaqihutiMCMax;
		//int mStatusBaqihutiSC;
		//int mStatusBaqihutiSCMax;

		//int mStatusZhuanShenMaxHp;
		//int mStatusZhuanShenMaxMp;
		//int mStatusZhuanShenXishou;
		//int mStatusZhuanShenDC;
		//int mStatusZhuanShenDCMax;
		//int mStatusZhuanShenMC;
		//int mStatusZhuanShenMCMax;
		//int mStatusZhuanShenSC;
		//int mStatusZhuanShenSCMax;

		//int mStatusVipAC;
		//int mStatusVipACMax;
		//int mStatusVipMAC;
		//int mStatusVipMACMax;
		//int mStatusVipDC;
		//int mStatusVipDCMax;
		//int mStatusVipMC;
		//int mStatusVipMCMax;
		//int mStatusVipSC;
		//int mStatusVipSCMax;

		//int mDeleteExchangeUpdFromEquip;
		int mDieDropBagProb;
		int mDieDropLoadProb;

		int mProtectItemPrice;
		int mProtectItemProbMax;
		int mProtectItemProb;
		int mProtectItemAdd;
		int mPKConfirm;
		int mGuildMaxMember;

		//int mReinResetAttrVcoin;
		//int mReinBuyTimesVcoin;
		//int mReinDCMaxPoint;
		//int mReinMCMaxPoint;
		//int mReinSCMaxPoint;
		//int mReinACMaxPoint;
		//int mReinMACMaxPoint;
		//int mReinHPMaxPoint;
		//int mReinMPMaxPoint;
		//int mReinAccuaryPoint;
		//int mReinDodgePoint;
		//int mReinFreeTimesADay;

		int mTotalAttrLevelLimit;
		//int mShowLoginForm;
	MESSAGE_CLASS_INIT(GameParamNotify)
		//steel_equip_cost_base = 0;
		//steel_equip_cost_mul = 0;
		max_magic_anti = 0;
		walk_speed_warrior_client_param = 0;
		stand_relive_price = 0;
		chart_open_limit_level = 0;
		add_depot_price = 0;
		//exchange_upd_prob_base=0;
		//exchange_upd_prob_gap=0;
		//exchange_upd_drop_max=0;
		//exchange_upd_cost_gm = 0;
		//exchange_upd_cost_bv = 0;

		//mStatusQiseshendanAC = 0;
		//mStatusQiseshendanACMax = 0;
		//mStatusQiseshendanMAC = 0;
		//mStatusQiseshendanMACMax = 0;
		//mStatusQiseshendanDC = 0;
		//mStatusQiseshendanDCMax = 0;
		//mStatusQiseshendanMC = 0;
		//mStatusQiseshendanMCMax = 0;
		//mStatusQiseshendanSC = 0;
		//mStatusQiseshendanSCMax = 0;
		//mStatusQiseshendanHpmaxBase = 0;
		//mStatusQiseshendanHpmaxGap = 0;
		//mStatusQiseshendanMpmaxBase = 0;
		//mStatusQiseshendanMpmaxGap = 0;

		//mStatusXinFaHPMax = 0;
		//mStatusXinFaXishou = 0;
		//mStatusXinFaAC = 0;
		//mStatusXinFaACMax = 0;
		//mStatusXinFaMAC = 0;
		//mStatusXinFaMACMax = 0;
		//mStatusXinFaDC = 0;
		//mStatusXinFaDCMax = 0;
		//mStatusXinFaMC = 0;
		//mStatusXinFaMCMax = 0;
		//mStatusXinFaSC = 0;
		//mStatusXinFaSCMax = 0;

		//mStatusYuanshenhutiAC = 0;
		//mStatusYuanshenhutiACMax = 0;
		//mStatusYuanshenhutiMAC = 0;
		//mStatusYuanshenhutiMACMax = 0;
		//mStatusYuanshenhutiDC = 0;
		//mStatusYuanshenhutiDCMax = 0;
		//mStatusYuanshenhutiMC = 0;
		//mStatusYuanshenhutiMCMax = 0;
		//mStatusYuanshenhutiSC = 0;
		//mStatusYuanshenhutiSCMax = 0;

		//mStatusFuQiTongXinAC=0;
		//mStatusFuQiTongXinACMax=0;
		//mStatusFuQiTongXinMAC=0;
		//mStatusFuQiTongXinMACMax=0;

		//mStatusTianshenhutiMAXHP = 0;
		//mStatusTianshenhutiDC = 0;
		//mStatusTianshenhutiDCMax = 0;
		//mStatusTianshenhutiMC = 0;
		//mStatusTianshenhutiMCMax = 0;
		//mStatusTianshenhutiSC = 0;
		//mStatusTianshenhutiSCMax = 0;
		//mStatusTianshenhutiSubDamageProb = 0;
		//mStatusTianshenhutiSubDamagePres = 0;

		//mStatusBaqihutiAC=0;
		//mStatusBaqihutiACMax=0;
		//mStatusBaqihutiMAC=0;
		//mStatusBaqihutiMACMax=0;
		//mStatusBaqihutiDC=0;
		//mStatusBaqihutiDCMax=0;
		//mStatusBaqihutiMC=0;
		//mStatusBaqihutiMCMax=0;
		//mStatusBaqihutiSC=0;
		//mStatusBaqihutiSCMax=0;

		//mStatusZhuanShenMaxHp=0;
		//mStatusZhuanShenMaxMp=0;
		//mStatusZhuanShenXishou=0;
		//mStatusZhuanShenDC=0;
		//mStatusZhuanShenDCMax=0;
		//mStatusZhuanShenMC=0;
		//mStatusZhuanShenMCMax=0;
		//mStatusZhuanShenSC=0;
		//mStatusZhuanShenSCMax=0;

		//mStatusVipAC=0;
		//mStatusVipACMax=0;
		//mStatusVipMAC=0;
		//mStatusVipMACMax=0;
		//mStatusVipDC=0;
		//mStatusVipDCMax=0;
		//mStatusVipMC=0;
		//mStatusVipMCMax=0;
		//mStatusVipSC=0;
		//mStatusVipSCMax=0;

		//mDeleteExchangeUpdFromEquip = 0;
		mDieDropBagProb = 0;
		mDieDropLoadProb = 0;
		
		mProtectItemPrice=0;
		mProtectItemProbMax=0;
		mProtectItemProb=0;
		mProtectItemAdd=0;
		mPKConfirm = 0;
		mGuildMaxMember=0;

		//mReinResetAttrVcoin=0;
		//mReinBuyTimesVcoin=0;
		//mReinDCMaxPoint=0;
		//mReinMCMaxPoint=0;
		//mReinSCMaxPoint=0;
		//mReinACMaxPoint=0;
		//mReinMACMaxPoint=0;
		//mReinHPMaxPoint=0;
		//mReinMPMaxPoint=0;
		//mReinAccuaryPoint=0;
		//mReinDodgePoint=0;
		//mReinFreeTimesADay=0;

		mTotalAttrLevelLimit=0;
		//mShowLoginForm=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GameParamNotify)
		stream
		//<<data.steel_equip_cost_base<<data.steel_equip_cost_mul
		<<data.max_magic_anti<<data.walk_speed_warrior_client_param<<data.stand_relive_price<<data.chart_open_limit_level<<data.add_depot_price
		//<<data.exchange_upd_prob_base<<data.exchange_upd_prob_gap<<data.exchange_upd_drop_max<<data.exchange_upd_cost_gm<<data.exchange_upd_cost_bv
		//<<data.mStatusQiseshendanAC<<data.mStatusQiseshendanACMax<<data.mStatusQiseshendanMAC<<data.mStatusQiseshendanMACMax<<data.mStatusQiseshendanDC<<data.mStatusQiseshendanDCMax
		//<<data.mStatusQiseshendanMC<<data.mStatusQiseshendanMCMax<<data.mStatusQiseshendanSC<<data.mStatusQiseshendanSCMax<<data.mStatusQiseshendanHpmaxBase<<data.mStatusQiseshendanHpmaxGap
		//<<data.mStatusQiseshendanMpmaxBase<<data.mStatusQiseshendanMpmaxGap
		//<<data.mStatusYuanshenhutiAC<<data.mStatusYuanshenhutiACMax<<data.mStatusYuanshenhutiMAC<<data.mStatusYuanshenhutiMACMax
		//<<data.mStatusYuanshenhutiDC<<data.mStatusYuanshenhutiDCMax<<data.mStatusYuanshenhutiMC<<data.mStatusYuanshenhutiMCMax<<data.mStatusYuanshenhutiSC<<data.mStatusYuanshenhutiSCMax
		//<<data.mStatusTianshenhutiMAXHP<<data.mStatusTianshenhutiDC<<data.mStatusTianshenhutiDCMax<<data.mStatusTianshenhutiMC<<data.mStatusTianshenhutiMCMax
		//<<data.mStatusTianshenhutiSC<<data.mStatusTianshenhutiSCMax<<data.mStatusTianshenhutiSubDamageProb<<data.mStatusTianshenhutiSubDamagePres
		//<<data.mStatusBaqihutiAC<<data.mStatusBaqihutiACMax<<data.mStatusBaqihutiMAC<<data.mStatusBaqihutiMACMax
		//<<data.mStatusBaqihutiDC<<data.mStatusBaqihutiDCMax<<data.mStatusBaqihutiMC<<data.mStatusBaqihutiMCMax<<data.mStatusBaqihutiSC<<data.mStatusBaqihutiSCMax
		//<<data.mDeleteExchangeUpdFromEquip
		<<data.mDieDropBagProb<<data.mDieDropLoadProb
		<<data.mProtectItemPrice<<data.mProtectItemProbMax<<data.mProtectItemProb<<data.mProtectItemAdd<<data.mPKConfirm
		//<<data.mStatusFuQiTongXinAC<<data.mStatusFuQiTongXinACMax<<data.mStatusFuQiTongXinMAC<<data.mStatusFuQiTongXinMACMax
		<<data.mGuildMaxMember
		//<<data.mReinResetAttrVcoin<<data.mReinBuyTimesVcoin<<data.mReinDCMaxPoint<<data.mReinMCMaxPoint<<data.mReinSCMaxPoint
		//<<data.mReinACMaxPoint<<data.mReinMACMaxPoint<<data.mReinHPMaxPoint<<data.mReinMPMaxPoint<<data.mReinAccuaryPoint<<data.mReinDodgePoint<<data.mReinFreeTimesADay
		<<data.mTotalAttrLevelLimit
		//<<data.mStatusXinFaHPMax<<data.mStatusXinFaXishou<<data.mStatusXinFaAC<<data.mStatusXinFaACMax<<data.mStatusXinFaMAC<<data.mStatusXinFaMACMax
		//<<data.mStatusXinFaDC<<data.mStatusXinFaDCMax<<data.mStatusXinFaMC<<data.mStatusXinFaMCMax<<data.mStatusXinFaSC<<data.mStatusXinFaSCMax<<data.mShowLoginForm
		//<<data.mStatusZhuanShenMaxHp<<data.mStatusZhuanShenMaxMp<<data.mStatusZhuanShenXishou<<data.mStatusZhuanShenDC<<data.mStatusZhuanShenDCMax
		//<<data.mStatusZhuanShenMC<<data.mStatusZhuanShenMCMax<<data.mStatusZhuanShenSC<<data.mStatusZhuanShenSCMax
		//<<data.mStatusVipDC<<data.mStatusVipDCMax<<data.mStatusVipMC<<data.mStatusVipMCMax<<data.mStatusVipSC<<data.mStatusVipSCMax<<data.mStatusVipAC<<data.mStatusVipACMax<<data.mStatusVipMAC<<data.mStatusVipMACMax
		;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GameParamNotify)
		stream
		//>>data.steel_equip_cost_base>>data.steel_equip_cost_mul
		>>data.max_magic_anti>>data.walk_speed_warrior_client_param>>data.stand_relive_price>>data.chart_open_limit_level>>data.add_depot_price
		//>>data.exchange_upd_prob_base
		//>>data.exchange_upd_prob_gap>>data.exchange_upd_drop_max>>data.exchange_upd_cost_gm>>data.exchange_upd_cost_bv
		//>>data.mStatusQiseshendanAC>>data.mStatusQiseshendanACMax>>data.mStatusQiseshendanMAC>>data.mStatusQiseshendanMACMax>>data.mStatusQiseshendanDC>>data.mStatusQiseshendanDCMax
		//>>data.mStatusQiseshendanMC>>data.mStatusQiseshendanMCMax>>data.mStatusQiseshendanSC>>data.mStatusQiseshendanSCMax>>data.mStatusQiseshendanHpmaxBase>>data.mStatusQiseshendanHpmaxGap
		//>>data.mStatusQiseshendanMpmaxBase>>data.mStatusQiseshendanMpmaxGap
		//>>data.mStatusYuanshenhutiAC>>data.mStatusYuanshenhutiACMax>>data.mStatusYuanshenhutiMAC>>data.mStatusYuanshenhutiMACMax
		//>>data.mStatusYuanshenhutiDC>>data.mStatusYuanshenhutiDCMax>>data.mStatusYuanshenhutiMC>>data.mStatusYuanshenhutiMCMax>>data.mStatusYuanshenhutiSC>>data.mStatusYuanshenhutiSCMax
		//>>data.mStatusTianshenhutiMAXHP>>data.mStatusTianshenhutiDC>>data.mStatusTianshenhutiDCMax>>data.mStatusTianshenhutiMC>>data.mStatusTianshenhutiMCMax
		//>>data.mStatusTianshenhutiSC>>data.mStatusTianshenhutiSCMax>>data.mStatusTianshenhutiSubDamageProb>>data.mStatusTianshenhutiSubDamagePres
		//>>data.mStatusBaqihutiAC>>data.mStatusBaqihutiACMax>>data.mStatusBaqihutiMAC>>data.mStatusBaqihutiMACMax
		//>>data.mStatusBaqihutiDC>>data.mStatusBaqihutiDCMax>>data.mStatusBaqihutiMC>>data.mStatusBaqihutiMCMax>>data.mStatusBaqihutiSC>>data.mStatusBaqihutiSCMax
		//>>data.mDeleteExchangeUpdFromEquip
		>>data.mDieDropBagProb>>data.mDieDropLoadProb
		>>data.mProtectItemPrice>>data.mProtectItemProbMax>>data.mProtectItemProb>>data.mProtectItemAdd>>data.mPKConfirm
		//>>data.mStatusFuQiTongXinAC>>data.mStatusFuQiTongXinACMax>>data.mStatusFuQiTongXinMAC>>data.mStatusFuQiTongXinMACMax
		>>data.mGuildMaxMember
		//>>data.mReinResetAttrVcoin>>data.mReinBuyTimesVcoin>>data.mReinDCMaxPoint>>data.mReinMCMaxPoint>>data.mReinSCMaxPoint
		//>>data.mReinACMaxPoint>>data.mReinMACMaxPoint>>data.mReinHPMaxPoint>>data.mReinMPMaxPoint>>data.mReinAccuaryPoint>>data.mReinDodgePoint>>data.mReinFreeTimesADay
		>>data.mTotalAttrLevelLimit
		//>>data.mStatusXinFaHPMax>>data.mStatusXinFaXishou>>data.mStatusXinFaAC>>data.mStatusXinFaACMax>>data.mStatusXinFaMAC>>data.mStatusXinFaMACMax
		//>>data.mStatusXinFaDC>>data.mStatusXinFaDCMax>>data.mStatusXinFaMC>>data.mStatusXinFaMCMax>>data.mStatusXinFaSC>>data.mStatusXinFaSCMax>>data.mShowLoginForm
		//>>data.mStatusZhuanShenMaxHp>>data.mStatusZhuanShenMaxMp>>data.mStatusZhuanShenXishou>>data.mStatusZhuanShenDC>>data.mStatusZhuanShenDCMax
		//>>data.mStatusZhuanShenMC>>data.mStatusZhuanShenMCMax>>data.mStatusZhuanShenSC>>data.mStatusZhuanShenSCMax
		//>>data.mStatusVipDC>>data.mStatusVipDCMax>>data.mStatusVipMC>>data.mStatusVipMCMax>>data.mStatusVipSC>>data.mStatusVipSCMax>>data.mStatusVipAC>>data.mStatusVipACMax>>data.mStatusVipMAC>>data.mStatusVipMACMax
		;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SessionClosedNotify,GS_Client_Session_Closed)
		char msg[512];
	MESSAGE_CLASS_INIT(SessionClosedNotify)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SessionClosedNotify)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SessionClosedNotify)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SessionDelayNotify,GS_Client_Session_Delay_Reauth)
		int delay;
	MESSAGE_CLASS_INIT(SessionDelayNotify)
		delay = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SessionDelayNotify)
		stream<<data.delay;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SessionDelayNotify)
		stream>>data.delay;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(WarInfoNotify,GS_Client_WarInfo)
		int war_state;
		char king_guild[512];
		char king_leader[512];
	MESSAGE_CLASS_INIT(WarInfoNotify)
		war_state = 0;
		king_guild[0]=0;
		king_leader[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(WarInfoNotify)
		stream<<data.war_state<<data.king_guild<<data.king_leader;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(WarInfoNotify)
		stream>>data.war_state>>data.king_guild>>data.king_leader;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapOptionNotify,GS_Client_Map_Option)
		char map_id[512];
		char pkprohibit;
		char pkallow;
		char auto_alive;
		char nointeract;
		char lockaction;
		char wanderhangup;
		char fightstate;
	MESSAGE_CLASS_INIT(MapOptionNotify)
		map_id[0]=0;
		pkprohibit = 0;
		pkallow = 0;
		auto_alive = 0;
		nointeract = 0;
		lockaction = 0;
		wanderhangup = 0;
		fightstate = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapOptionNotify)
		stream<<data.map_id<<data.pkprohibit<<data.pkallow<<data.auto_alive<<data.nointeract<<data.lockaction<<data.wanderhangup<<data.fightstate;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapOptionNotify)
		stream>>data.map_id>>data.pkprohibit>>data.pkallow>>data.auto_alive>>data.nointeract>>data.lockaction>>data.wanderhangup<<data.fightstate;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GuildConditionNotify,GS_Client_Guild_Condition)
		std::string condition;
	MESSAGE_CLASS_INIT(GuildConditionNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildConditionNotify)
		stream<<data.condition;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GuildConditionNotify)
		stream>>data.condition;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(StotAddNotify,GS_Client_SlotAdd)
		int depot_add;
		int bag_add;
		int bag_max_num;
	MESSAGE_CLASS_INIT(StotAddNotify)
		depot_add = 0;
		bag_add = 0;
		bag_max_num = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(StotAddNotify)
		stream<<data.depot_add<<data.bag_add<<data.bag_max_num;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(StotAddNotify)
		stream>>data.depot_add>>data.bag_add>>data.bag_max_num;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(MergeSteelRes,GS_Client_Merge_Steel)
	//	int result;
	//MESSAGE_CLASS_INIT(MergeSteelRes)
	//	result = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(MergeSteelRes)
	//	stream<<data.result;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(MergeSteelRes)
	//	stream>>data.result;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(UpgradeEquipRes,GS_Client_Upgrade_Equip)
	//	int result;
	//MESSAGE_CLASS_INIT(UpgradeEquipRes)
	//	result = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(UpgradeEquipRes)
	//	stream<<data.result;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(UpgradeEquipRes)
	//	stream>>data.result;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(EquipExchangeUpgradeRes,GS_Client_Equip_Exchange_Upgrade)
	//	int result;
	//MESSAGE_CLASS_INIT(EquipExchangeUpgradeRes)
	//	result = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(EquipExchangeUpgradeRes)
	//	stream<<data.result;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(EquipExchangeUpgradeRes)
	//	stream>>data.result;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(EquipReRandAddRes,GS_Client_Equip_ReRandAdd)
	//	int result;
	//MESSAGE_CLASS_INIT(EquipReRandAddRes)
	//	result = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(EquipReRandAddRes)
	//	stream<<data.result;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(EquipReRandAddRes)
	//	stream>>data.result;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NameAddNotify,GS_Client_NameAdd_Notify)
		unsigned int src_id;
		char pre[512];
		char pro[512];
	MESSAGE_CLASS_INIT(NameAddNotify)
		pre[0]=0;
		pro[0]=0;
		src_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NameAddNotify)
		stream<<data.src_id<<data.pre<<data.pro;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NameAddNotify)
		stream>>data.src_id>>data.pre>>data.pro;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(URLNotify,GS_Client_URL_Notify)
		char reg[512];
		char login[512];
		char pay[512];
		char webhome[512];
		char bbs[512];
		char download[512];
		char kefu[512];
		char paramurl1[512];
		char paramurl2[512];
		char paramurl3[512];
		char paramurl4[512];
		char paramurl5[512];
	MESSAGE_CLASS_INIT(URLNotify)
		reg[0]=0;
		login[0]=0;
		pay[0]=0;
		webhome[0]=0;
		bbs[0]=0;
		download[0]=0;
		kefu[0]=0;
		paramurl1[0]=0;
		paramurl2[0]=0;
		paramurl3[0]=0;
		paramurl4[0]=0;
		paramurl5[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(URLNotify)
		stream<<data.reg<<data.login<<data.pay<<data.webhome<<data.bbs<<data.download<<data.kefu
			<<data.paramurl1<<data.paramurl2<<data.paramurl3<<data.paramurl4<<data.paramurl5;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(URLNotify)
		stream>>data.reg>>data.login>>data.pay>>data.webhome>>data.bbs>>data.download>>data.kefu
			>>data.paramurl1>>data.paramurl2>>data.paramurl3>>data.paramurl4>>data.paramurl5;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FreeReliveNotify,GS_Client_Free_Relive_Level)
		int level;
	MESSAGE_CLASS_INIT(FreeReliveNotify)
		level = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FreeReliveNotify)
		stream<<data.level;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FreeReliveNotify)
		stream>>data.level;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SetModelNotify,GS_Client_Set_Model_Notify)
		unsigned int src_id;
		int id;
		int vl;
	MESSAGE_CLASS_INIT(SetModelNotify)
		src_id = 0;
		id = 0;
		vl = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SetModelNotify)
		stream<<data.src_id<<data.id<<data.vl;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SetModelNotify)
		stream>>data.src_id>>data.id>>data.vl;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ChinaLimitNotify,GS_Client_China_Limit_Level)
		unsigned int china_limit_lv;
		int online_time_today;
		int china_id;
	MESSAGE_CLASS_INIT(ChinaLimitNotify)
		china_limit_lv = 0;
		online_time_today = 0;
		china_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ChinaLimitNotify)
		stream<<data.china_limit_lv<<data.online_time_today<<data.china_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ChinaLimitNotify)
		stream>>data.china_limit_lv>>data.online_time_today>>data.china_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MonsterChatNotify,GS_Client_Monster_Chat_Notify)
		unsigned int src_id;
		char msg[512];
	MESSAGE_CLASS_INIT(MonsterChatNotify)
		src_id = 0;
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MonsterChatNotify)
		stream<<data.src_id<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MonsterChatNotify)
		stream>>data.src_id>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(UpdateTicketRes,GS_Client_Update_Ticket)
	MESSAGE_CLASS_INIT(UpdateTicketRes)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(UpdateTicketRes)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(UpdateTicketRes)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FindMapGhostRes,GS_Client_Find_Map_Ghost)
		char map_id[512];
		char mon_name[512];
		struct GhostNode
		{
			int id;
			short x;
			short y;
			short type;
		};
		std::vector<GhostNode>mGhostList;
	MESSAGE_CLASS_INIT(FindMapGhostRes)
		map_id[0]=0;
		mon_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FindMapGhostRes)
		stream<<data.map_id<<data.mon_name<<static_cast<short>(data.mGhostList.size());
		for( std::vector<FindMapGhostRes::GhostNode>::const_iterator pos = data.mGhostList.begin();
			pos != data.mGhostList.end();
			++ pos )
		{
			stream<<pos->id<<pos->x<<pos->y<<pos->type;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FindMapGhostRes)
		short size=0;
		stream>>data.map_id>>data.mon_name>>size;
		for( short i = 0;i<size;i++ )
		{
			FindMapGhostRes::GhostNode ta;
			stream>>ta.id>>ta.x>>ta.y>>ta.type;
			if( stream )
			{
				data.mGhostList.push_back(ta);
			}
			else
			{
				printf("FindMapGhostRes for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapMonGenNotify,GS_Client_Map_Mon_Gen_Notify)
		char map_id[512];
		struct GenNode
		{
			char name[64];
			short x;
			short y;
			int time;
		};
		std::vector<GenNode>mGenList;
	MESSAGE_CLASS_INIT(MapMonGenNotify)
		map_id[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapMonGenNotify)
		stream<<data.map_id<<static_cast<short>(data.mGenList.size());
		for( std::vector<MapMonGenNotify::GenNode>::const_iterator pos = data.mGenList.begin();
			pos != data.mGenList.end();
			++ pos )
		{
			stream<<pos->name<<pos->x<<pos->y<<pos->time;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapMonGenNotify)
		short size=0;
		stream>>data.map_id>>size;
		for( short i = 0;i<size;i++ )
		{
			MapMonGenNotify::GenNode ta;
			stream>>ta.name>>ta.x>>ta.y>>ta.time;
			if( stream )
			{
				data.mGenList.push_back(ta);
			}
			else
			{
				printf("MapMonGenNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapMiniNpcNotify,GS_Client_Map_Mini_Npc_Notify)
		char map_id[512];
		char npc_id[512];
		char short_id[512];
		int x;
		int y;
		int directflyID;
		int showNpcFlag;
		int num;
	MESSAGE_CLASS_INIT(MapMiniNpcNotify)
		x = 0;
		y = 0;
		directflyID = 0;
		showNpcFlag = 0;
		map_id[0]=0;
		npc_id[0]=0;
		short_id[0]=0;
		num=1;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapMiniNpcNotify)
		stream<<data.map_id<<data.npc_id<<data.short_id<<data.x<<data.y<<data.directflyID<<data.showNpcFlag<<data.num;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapMiniNpcNotify)
		stream>>data.map_id>>data.npc_id>>data.short_id>>data.x>>data.y>>data.directflyID>>data.showNpcFlag>>data.num;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(OfflineExpInfoNotify,GS_Client_Offline_Exp_Info)
		int offline_time;
		int offline_time_valide;
		int offline_time_valide_max;
		int offline_exp;
		int price1;
		int price2;
		int price3;
	MESSAGE_CLASS_INIT(OfflineExpInfoNotify)
		offline_time=0;
		offline_time_valide=0;
		offline_time_valide_max=0;
		offline_exp=0;
		price1=0;
		price2=0;
		price3=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(OfflineExpInfoNotify)
		stream<<data.offline_time<<data.offline_time_valide<<data.offline_time_valide_max<<data.offline_exp<<data.price1<<data.price2<<data.price3;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(OfflineExpInfoNotify)
		stream>>data.offline_time>>data.offline_time_valide>>data.offline_time_valide_max>>data.offline_exp>>data.price1>>data.price2>>data.price3;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TeamInfoNotify,GS_Client_Team_Info_Notify)
		unsigned int src_id;
		int team_id;
		char team_name[512];
	MESSAGE_CLASS_INIT(TeamInfoNotify)
		src_id=0;
		team_id=0;
		team_name[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TeamInfoNotify)
		stream<<data.src_id<<data.team_id<<data.team_name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TeamInfoNotify)
		stream>>data.src_id>>data.team_id>>data.team_name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PlayerTalkNotify,GS_Client_Player_Talk_Notify)
		int seed;
		const std::string* msg;
	MESSAGE_CLASS_INIT(PlayerTalkNotify)
		seed=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PlayerTalkNotify)
		stream<<data.seed<<(*data.msg);
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PlayerTalkNotify)
		std::string msg;
		stream>>data.seed>>msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GetChartInfoRes,GS_Client_Get_Chart_Info)
		int chart_type;
		int page;
		int num;
		struct node{
			char name[256];
			char guild[256];
			char title[256];
			int param;
			int job;
			int lv;
			char state;
			int zslv;
			int gender;
			node()
			{
				name[0] = 0;
				guild[0] = 0;
				title[0] = 0;
				param = 0;
				job = 0;
				lv = 0;
				state = 0;
				zslv = 0;
				gender=0;
			}
		};
		node nodes[100];
	MESSAGE_CLASS_INIT(GetChartInfoRes)
		chart_type=0;
		page=0;
		num=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GetChartInfoRes)
		stream<<data.chart_type<<data.page<<data.num;
		for(int i = 0;i < data.num;i++)
		{
			stream<<data.nodes[i].name<<data.nodes[i].param<<data.nodes[i].guild<<data.nodes[i].title<<data.nodes[i].job<<data.nodes[i].lv<<data.nodes[i].state<<data.nodes[i].zslv<<data.nodes[i].gender;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GetChartInfoRes)
		stream>>data.chart_type>>data.page>>data.num;
		for(int i = 0;i < data.num;i++)
		{
			stream >> data.nodes[i].name >> data.nodes[i].param >> data.nodes[i].guild >> data.nodes[i].title >> data.nodes[i].job >> data.nodes[i].lv >> data.nodes[i].state >> data.nodes[i].zslv>>data.nodes[i].gender;
			if( stream ){}
			else
			{
				printf("GetChartInfoRes for size out stream :%d\n",data.num);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(InfoItemExchangeRes,GS_Client_Info_Item_Exchange)
	//	int pos;
	//	int type_id;
	//	int ret;
	//	const std::string* msg;
	//MESSAGE_CLASS_INIT(InfoItemExchangeRes)
	//	pos=0;
	//	type_id=0;
	//	ret = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(InfoItemExchangeRes)
	//	stream<<data.pos<<data.type_id<<data.ret<<(*data.msg);
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(InfoItemExchangeRes)
	//	std::string msg;
	//	stream>>data.pos>>data.type_id>>data.ret>>msg;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(ItemExchangeRes,GS_Client_Item_Exchange)
	//	int pos;
	//	int type_id;
	//	int ret;
	//MESSAGE_CLASS_INIT(ItemExchangeRes)
	//	pos=0;
	//	type_id=0;
	//	ret = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(ItemExchangeRes)
	//	stream<<data.pos<<data.type_id<<data.ret;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(ItemExchangeRes)
	//	stream>>data.pos>>data.type_id>>data.ret;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ProsperityChangeNotify,GS_Client_Prosperity_Change_Notify)
		int prosperity;
		int next;
	MESSAGE_CLASS_INIT(ProsperityChangeNotify)
		prosperity=0;
		next = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ProsperityChangeNotify)
		stream<<data.prosperity<<data.next;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ProsperityChangeNotify)
		stream>>data.prosperity>>data.next;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MapChatRes,GS_Client_Map_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(MapChatRes)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MapChatRes)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MapChatRes)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(HornChatRes,GS_Client_Horn_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(HornChatRes)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(HornChatRes)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(HornChatRes)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PrivateChatRes,GS_Client_Private_Chat)
		char tname[512];
		char msg[512];
	MESSAGE_CLASS_INIT(PrivateChatRes)
		tname[0]=0;
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PrivateChatRes)
		stream<<data.tname<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PrivateChatRes)
		stream>>data.tname>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GroupChatRes,GS_Client_Group_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(GroupChatRes)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GroupChatRes)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GroupChatRes)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GuildChatRes,GS_Client_Guild_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(GuildChatRes)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildChatRes)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GuildChatRes)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(NormalChatRes,GS_Client_Normal_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(NormalChatRes)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(NormalChatRes)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(NormalChatRes)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(WorldChatRes,GS_Client_World_Chat)
		char msg[512];
	MESSAGE_CLASS_INIT(WorldChatRes)
		msg[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(WorldChatRes)
		stream<<data.msg;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(WorldChatRes)
		stream>>data.msg;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GUIOpenPanelNotify,GS_Client_GUI_Open_Panel_Notify)
		int panel_id;
	MESSAGE_CLASS_INIT(GUIOpenPanelNotify)
		panel_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GUIOpenPanelNotify)
		stream<<data.panel_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GUIOpenPanelNotify)
		stream>>data.panel_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FriendChangeRes,GS_Client_Friend_Change)
		char name[512];
		int title;
		int online_state;
		int job;
		int gender;
		int level;
		char guild[512];
	MESSAGE_CLASS_INIT(FriendChangeRes)
		title = 0;
		online_state = 0;
		name[0]=0;
		job = 0;
		gender = 0;
		level = 0;
		guild[0] = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FriendChangeRes)
		stream<<data.name<<data.title<<data.online_state<<data.job<<data.gender<<data.level<<data.guild;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FriendChangeRes)
		stream>>data.name>>data.title>>data.online_state>>data.job>>data.gender>>data.level>>data.guild;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FriendFreshRes,GS_Client_Friend_Fresh)
	MESSAGE_CLASS_INIT(FriendFreshRes)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FriendFreshRes)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FriendFreshRes)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FriendApply,GS_Client_Friend_Apply)
	char name[512];
	MESSAGE_CLASS_INIT(FriendApply)
	name[0] = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FriendApply)
	stream<<data.name;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FriendApply)
	stream>>data.name;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PKConfirmNotify,GS_Client_PK_Confirm_Notify)
	MESSAGE_CLASS_INIT(PKConfirmNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PKConfirmNotify)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PKConfirmNotify)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PingRes,GS_Client_Ping)
	MESSAGE_CLASS_INIT(PingRes)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PingRes)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PingRes)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ProtectItemRes,GS_Client_Protect_Item)
		int result;
	MESSAGE_CLASS_INIT(ProtectItemRes)
		result = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ProtectItemRes)
		stream<<data.result;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ProtectItemRes)
		stream>>data.result;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListTalkListNotify,GS_Client_ListTalk_List_Notify)
		const std::string* msg;
	    int uiID;
	MESSAGE_CLASS_INIT(ListTalkListNotify)
		msg=0;
	    uiID=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListTalkListNotify)
		stream<<(*data.msg)<<data.uiID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListTalkListNotify)
		std::string msg;
		stream>>msg>>data.uiID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListTalkContentNotify,GS_Client_ListTalk_Content_Notify)
		const std::string* msg;
	    int uiID;
	MESSAGE_CLASS_INIT(ListTalkContentNotify)
		msg=0;
	    uiID=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListTalkContentNotify)
		stream<<(*data.msg)<<data.uiID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListTalkContentNotify)
		std::string msg;
		stream>>msg>>data.uiID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListTalkTitleNotify,GS_Client_ListTalk_Title_Notify)
		char msg[512];
		int seed;
		int uiID;
	MESSAGE_CLASS_INIT(ListTalkTitleNotify)
		msg[0]=0;
		seed = 0;
		uiID=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListTalkTitleNotify)
		stream<<data.msg<<data.seed<<data.uiID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListTalkTitleNotify)
		stream>>data.msg>>data.seed>>data.uiID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AchieveDoneNotify,GS_Client_Achieve_Done_Notify)
		int aid;
		int ap;
	MESSAGE_CLASS_INIT(AchieveDoneNotify)
		aid = 0;
		ap = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AchieveDoneNotify)
		stream<<data.aid<<data.ap;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AchieveDoneNotify)
		stream>>data.aid>>data.ap;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(EnterMarryInviteNotify,GS_Client_Enter_Marry_Invite_Notify)
	//	char target[512];
	//MESSAGE_CLASS_INIT(EnterMarryInviteNotify)
	//	target[0]=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(EnterMarryInviteNotify)
	//	stream<<data.target;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(EnterMarryInviteNotify)
	//	stream>>data.target;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(MarryInviteNotify,GS_Client_Marry_Invite_Notify)
	//	char male_name[512];
	//	char male_desp[512];
	//MESSAGE_CLASS_INIT(MarryInviteNotify)
	//	male_name[0]=0;
	//	male_desp[0]=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(MarryInviteNotify)
	//	stream<<data.male_name<<data.male_desp;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(MarryInviteNotify)
	//	stream>>data.male_name>>data.male_desp;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(DivorceInviteNotify,GS_Client_Divorce_Invite_Notify)
	//	char male_name[512];
	//MESSAGE_CLASS_INIT(DivorceInviteNotify)
	//	male_name[0]=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(DivorceInviteNotify)
	//	stream<<data.male_name;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(DivorceInviteNotify)
	//	stream>>data.male_name;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(MarrySucNotify,GS_Client_Marry_Suc_Notify)
	//	int couple_id;
	//	int weddingdate;
	//	char male_name[512];
	//	char male_desp[512];
	//	char female_name[512];
	//	char female_desp[512];
	//MESSAGE_CLASS_INIT(MarrySucNotify)
	//	couple_id=0;
	//	weddingdate=0;
	//	male_name[0]=0;
	//	male_desp[0]=0;
	//	female_name[0]=0;
	//	female_desp[0]=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(MarrySucNotify)
	//	stream<<data.couple_id<<data.weddingdate<<data.male_name<<data.male_desp<<data.female_name<<data.female_desp;
	//MESSAGE_CLASS_IMPORT_END
	//	MESSAGE_CLASS_EXPORT(MarrySucNotify)
	//	stream>>data.couple_id>>data.weddingdate>>data.male_name>>data.male_desp>>data.female_name>>data.female_desp;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DefaultSkillNotify,GS_Client_Default_Skill_Notify)
		int defaultskill;
	MESSAGE_CLASS_INIT(DefaultSkillNotify)
		defaultskill = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DefaultSkillNotify)
		stream<<data.defaultskill;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DefaultSkillNotify)
		stream>>data.defaultskill;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(DailyTaskListRes,GS_Client_DailyTaskList_Res)
//		struct dailytask
//		{
//			dailytask()
//			{
//				id = 0;
//				needlevel = 0;
//				times= 0;
//				mapX = 0;
//				mapY = 0;
//				flyid = 0;
//			}
//			int id;
//			std::string taskname;
//			int needlevel;
//			int times;
//			std::string detail;
//			std::string mapID;
//			int mapX;
//			int mapY;
//			int flyid;
//			struct dailytaskaward
//			{
//				char item_name[512];
//				int item_num;
//				int type_id;
//			};
//			std::vector<dailytaskaward> mdailytaskaward;
//		};
//		std::vector<dailytask> dailytasks;
//	MESSAGE_CLASS_INIT(DailyTaskListRes)
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(DailyTaskListRes)
//		stream << static_cast<int>(data.dailytasks.size());
//		for( std::vector<DailyTaskListRes::dailytask>::const_iterator pos = data.dailytasks.begin();
//			pos != data.dailytasks.end();
//			++ pos )
//		{
//			stream<<pos->id<<pos->taskname<<pos->needlevel<<pos->times<<pos->detail<<pos->mapID<<pos->mapX<<pos->mapY<<pos->flyid;
//			stream << static_cast<int>(pos->mdailytaskaward.size());
//			for( std::vector<DailyTaskListRes::dailytask::dailytaskaward>::const_iterator pos1 = pos->mdailytaskaward.begin();
//				pos1 != pos->mdailytaskaward.end();
//				++ pos1 )
//			{
//				stream<<pos1->type_id<<pos1->item_num;
//			}
//		}
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(DailyTaskListRes)
//		int size=0;
//		stream>>size;
//		for( int i = 0;i<size;i++ )
//		{
//			DailyTaskListRes::dailytask dt;
//			stream>>dt.id>>dt.taskname>>dt.needlevel>>dt.times>>dt.detail>>dt.mapID>>dt.mapX>>dt.mapY>>dt.flyid;
//			int size1=0;
//			stream>>size1;
//			for( int i = 0;i<size1;i++ )
//			{
//				DailyTaskListRes::dailytask::dailytaskaward dtw;
//				stream>>dtw.type_id>>dtw.item_num;
//				dt.mdailytaskaward.push_back(dtw);
//			}
//			data.dailytasks.push_back(dt);
//		}
//	MESSAGE_CLASS_EXPORT_END
////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(InstanceTaskListRes,GS_Client_InstanceTaskList_Res)
//		struct instancetask
//		{
//			instancetask()
//			{
//				id = 0;
//				needlevel = 0;
//				times= 0;
//				mapX = 0;
//				mapY = 0;
//				flyid = 0;
//			}
//			int id;
//			std::string taskname;
//			int needlevel;
//			int times;
//			std::string suggestion;
//			std::string mapID;
//			int mapX;
//			int mapY;
//			int flyid;
//			struct instancetaskaward
//			{
//				char item_name[512];
//				int item_num;
//				int type_id;
//			};
//			std::vector<instancetaskaward>minstancetaskaward;
//		};
//		std::vector<instancetask> instancetasks;
//	MESSAGE_CLASS_INIT(InstanceTaskListRes)
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(InstanceTaskListRes)
//		stream << static_cast<int>(data.instancetasks.size());
//		for( std::vector<InstanceTaskListRes::instancetask>::const_iterator pos = data.instancetasks.begin();
//			pos != data.instancetasks.end();
//			++ pos )
//		{
//			stream<<pos->id<<pos->taskname<<pos->times<<pos->needlevel<<pos->suggestion<<pos->mapID<<pos->mapX<<pos->mapY<<pos->flyid;
//			stream << static_cast<int>(pos->minstancetaskaward.size());
//			for( std::vector<InstanceTaskListRes::instancetask::instancetaskaward>::const_iterator pos1 = pos->minstancetaskaward.begin();
//				pos1 != pos->minstancetaskaward.end();
//				++ pos1 )
//			{
//				stream<<pos1->type_id<<pos1->item_num;
//			}
//		}
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(InstanceTaskListRes)
//		int size=0;
//		stream>>size;
//		for( int i = 0;i<size;i++ )
//		{
//			InstanceTaskListRes::instancetask it;
//			stream>>it.id>>it.taskname>>it.times>>it.needlevel>>it.suggestion>>it.mapID>>it.mapX>>it.mapY>>it.flyid;
//			int size1=0;
//			stream>>size1;
//			for( int i = 0;i<size1;i++ )
//			{
//				InstanceTaskListRes::instancetask::instancetaskaward itw;
//				stream>>itw.type_id>>itw.item_num;
//				it.minstancetaskaward.push_back(itw);
//			}
//			data.instancetasks.push_back(it);
//		}
//	MESSAGE_CLASS_EXPORT_END
////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(BossFreshListRes,GS_Client_BossFreshList_Res)
//		struct bossfresh
//		{
//			bossfresh()
//			{
//				id = 0;
//				needlevel = 0;
//				mapX = 0;
//				mapY = 0;
//				flag = 0;
//				flyid = 0;
//			}
//			int id;
//			std::string bossname;
//			int needlevel;
//			std::string freshtime;
//			std::string detail;
//			std::string mapname;
//			std::string mapID;
//			int mapX;
//			int mapY;
//			int flag;
//			int flyid;
//			struct bossfreshaward
//			{
//				char item_name[512];
//				int item_num;
//				int type_id;
//			};
//			std::vector<bossfreshaward>mbossfreshaward;
//		};
//		std::vector<bossfresh> bossfreshs;
//	MESSAGE_CLASS_INIT(BossFreshListRes)
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(BossFreshListRes)
//		stream << static_cast<int>(data.bossfreshs.size());
//		for( std::vector<BossFreshListRes::bossfresh>::const_iterator pos = data.bossfreshs.begin();
//			pos != data.bossfreshs.end();
//			++ pos )
//		{
//			stream<<pos->id<<pos->bossname<<pos->needlevel<<pos->freshtime<<pos->detail<<pos->mapname<<pos->mapID<<pos->mapX<<pos->mapY<<pos->flag<<pos->flyid;
//			stream << static_cast<int>(pos->mbossfreshaward.size());
//			for( std::vector<BossFreshListRes::bossfresh::bossfreshaward>::const_iterator pos1 = pos->mbossfreshaward.begin();
//				pos1 != pos->mbossfreshaward.end();
//				++ pos1 )
//			{
//				stream<<pos1->type_id<<pos1->item_num;
//			}
//		}
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(BossFreshListRes)
//		int size=0;
//		stream>>size;
//		for( int i = 0;i<size;i++ )
//		{
//			BossFreshListRes::bossfresh bf;
//			stream>>bf.id>>bf.bossname>>bf.needlevel>>bf.freshtime>>bf.detail>>bf.mapname>>bf.mapID>>bf.mapX>>bf.mapY>>bf.flag>>bf.flyid;
//			int size1=0;
//			stream>>size1;
//			for( int i = 0;i<size1;i++ )
//			{
//				BossFreshListRes::bossfresh::bossfreshaward itw;
//				stream>>itw.type_id>>itw.item_num;
//				bf.mbossfreshaward.push_back(itw);
//			}
//			data.bossfreshs.push_back(bf);
//		}
//	MESSAGE_CLASS_EXPORT_END
////////////////////////////////////////////////////////////////////////////
//	MESSAGE_CLASS_BEGIN(EverydayTaskListRes,GS_Client_EverydayTaskList_Res)
//		struct everydaytask
//		{
//			everydaytask()
//			{
//				id = 0;
//				needlevel = 0;
//				mapX = 0;
//				mapY = 0;
//				flyid =0;
//			}
//			int id;
//			std::string taskname;
//			int needlevel;
//			std::string tasktime;
//			std::string detail;
//			std::string mapID;
//			int mapX;
//			int mapY;
//			int flyid;
//			struct everydaytaskaward
//			{
//				char item_name[512];
//				int item_num;
//				int type_id;
//			};
//			std::vector<everydaytaskaward>meverydaytaskaward;
//		};
//		std::vector<everydaytask> everydaytasks;
//	MESSAGE_CLASS_INIT(EverydayTaskListRes)
//	MESSAGE_CLASS_INIT_END
//	MESSAGE_CLASS_END
//	MESSAGE_CLASS_IMPORT(EverydayTaskListRes)
//		stream << static_cast<int>(data.everydaytasks.size());
//		for( std::vector<EverydayTaskListRes::everydaytask>::const_iterator pos = data.everydaytasks.begin();
//			pos != data.everydaytasks.end();
//			++ pos )
//		{
//			stream<<pos->id<<pos->taskname<<pos->needlevel<<pos->tasktime<<pos->detail<<pos->mapID<<pos->mapX<<pos->mapY<<pos->flyid;
//			stream << static_cast<int>(pos->meverydaytaskaward.size());
//			for( std::vector<EverydayTaskListRes::everydaytask::everydaytaskaward>::const_iterator pos1 = pos->meverydaytaskaward.begin();
//				pos1 != pos->meverydaytaskaward.end();
//				++ pos1 )
//			{
//				stream<<pos1->type_id<<pos1->item_num;
//			}
//		}
//	MESSAGE_CLASS_IMPORT_END
//	MESSAGE_CLASS_EXPORT(EverydayTaskListRes)
//		int size=0;
//		stream>>size;
//		for( int i = 0;i<size;i++ )
//		{
//			EverydayTaskListRes::everydaytask et;
//			stream>>et.id>>et.taskname>>et.needlevel>>et.tasktime>>et.detail>>et.mapID>>et.mapX>>et.mapY>>et.flyid;
//			int size1=0;
//			stream>>size1;
//			for( int i = 0;i<size1;i++ )
//			{
//				EverydayTaskListRes::everydaytask::everydaytaskaward itw;
//				stream>>itw.type_id>>itw.item_num;
//				et.meverydaytaskaward.push_back(itw);
//			}
//			data.everydaytasks.push_back(et);
//		}
//	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(DailyTaskTimesNotify,GS_Client_Daily_Task_Times_Notify)
	//	int times;
	//	int tasktype;
	//	int id;
	//MESSAGE_CLASS_INIT(DailyTaskTimesNotify)
	//	times = 0;
	//	tasktype=0;
	//	id=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(DailyTaskTimesNotify)
	//	stream<<data.times<<data.tasktype<<data.id;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(DailyTaskTimesNotify)
	//	stream>>data.times>>data.tasktype>>data.id;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(FreeDirectFlyNotify,GS_Client_Free_Direct_Fly_Notify)
	//	int free_direct_fly;
	//MESSAGE_CLASS_INIT(FreeDirectFlyNotify)
	//	free_direct_fly = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(FreeDirectFlyNotify)
	//	stream<<data.free_direct_fly;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(FreeDirectFlyNotify)
	//	stream>>data.free_direct_fly;
	//MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GotoEndNotify,GS_Client_Goto_End_Notify)
		char target[512];
	MESSAGE_CLASS_INIT(GotoEndNotify)
		target[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GotoEndNotify)
		stream<<data.target;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GotoEndNotify)
		stream>>data.target;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(StatusHPMPChangeNotify,GS_Client_Status_HPMP_Change_Notify)
		int hpInc;
		int mpInc;
	MESSAGE_CLASS_INIT(StatusHPMPChangeNotify)
		hpInc=0;
		mpInc=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(StatusHPMPChangeNotify)
		stream<<data.hpInc<<data.mpInc;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(StatusHPMPChangeNotify)
		stream>>data.hpInc>>data.mpInc;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(LoginItemListNotify,GS_Client_Login_Item_List_Notify)
		struct login
		{
			int item_num;
			int type_id;
		};
		std::vector<login>mlogin;
	MESSAGE_CLASS_INIT(LoginItemListNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(LoginItemListNotify)
		stream<<static_cast<int>(data.mlogin.size());
		for( std::vector<LoginItemListNotify::login>::const_iterator pos = data.mlogin.begin();
			pos != data.mlogin.end();
			++ pos )
		{
			stream<<pos->type_id<<pos->item_num;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(LoginItemListNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			LoginItemListNotify::login ly;
			stream>>ly.type_id>>ly.item_num;
			if( stream )
			{
				data.mlogin.push_back(ly);
			}
			else
			{
				printf("LoginItemListNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SlaveStateNotify,GS_Client_SlaveState_Notify)
	    int alive;
	MESSAGE_CLASS_INIT(SlaveStateNotify)
		alive=0;
	MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(SlaveStateNotify)
		stream<<data.alive;
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(SlaveStateNotify)
		stream>>data.alive;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CapacityChangeNotify,GS_Client_Capacity_Change_Notify)
		int capacity;
		int change_capacity;
	MESSAGE_CLASS_INIT(CapacityChangeNotify)
		capacity = 0;
		change_capacity = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CapacityChangeNotify)
		stream<<data.capacity<<data.change_capacity;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CapacityChangeNotify)
		stream>>data.capacity>>data.change_capacity;
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TotalAttrParamNotify,GS_Client_Total_Attr_Param_Notify)
		struct TotalAttr
		{
			int mID;
			int mJob;
			int mLevel;
			int mDC;
			int mDCMax;
			int mMC;
			int mMCMax;
			int mSC;
			int mSCMax;
			int mAC;
			int mACMax;
			int mMAC;
			int mMACMax;
			int mHP;
			int mMP;
			int mAccuracy;
			int mDodge;
			int mLuck;
			int mDropProb;
			int mDoubleAttProb;
		};
		std::vector<TotalAttr>mTotalAttr;
	MESSAGE_CLASS_INIT(TotalAttrParamNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TotalAttrParamNotify)
		stream<<static_cast<int>(data.mTotalAttr.size());
		for( std::vector<TotalAttrParamNotify::TotalAttr>::const_iterator pos = data.mTotalAttr.begin();
			pos != data.mTotalAttr.end();
			++ pos )
		{
			stream<<pos->mID<<pos->mJob<<pos->mLevel<<pos->mDC<<pos->mDCMax<<pos->mMC<<pos->mMCMax
				  <<pos->mSC<<pos->mSCMax<<pos->mAC<<pos->mACMax<<pos->mMAC<<pos->mMACMax<<pos->mHP
				  <<pos->mMP<<pos->mAccuracy<<pos->mDodge<<pos->mLuck<<pos->mDropProb<<pos->mDoubleAttProb;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TotalAttrParamNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			LotteryListRes::TotalAttr ta;
			stream>>ta.mID>>ta.mJob>>ta.mLevel>>ta.mDC>>ta.mDCMax>>ta.mMC>>ta.mMCMax>>ta.mSC>>ta.mSCMax>>ta.mAC>>ta.mACMax
				  >>ta.mMAC>>ta.mMACMax>>ta.mHP>>ta.mMP>>ta.mAccuracy>>ta.mDodge>>ta.mLuck>>ta.mDropProb>>ta.mDoubleAttProb;
			if( stream )
			{
				data.mTotalAttr.push_back(ta);
			}
			else
			{
				printf("TotalAttrParamNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(GiftListNotify,GS_Client_Gift_List_Notify)
		char msg[1024];
		struct GiftNode
		{
			int mID;
			int mNum;
		};
		std::vector<GiftNode>mGiftList;
	MESSAGE_CLASS_INIT(GiftListNotify)
		msg[0] = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GiftListNotify)
		stream<<data.msg<<static_cast<int>(data.mGiftList.size());
		for( std::vector<GiftListNotify::GiftNode>::const_iterator pos = data.mGiftList.begin();
			pos != data.mGiftList.end();
			++ pos )
		{
			stream<<pos->mID<<pos->mNum;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GiftListNotify)
		int size=0;
		stream>>data.msg>>size;
		for( int i = 0;i<size;i++ )
		{
			GiftListNotify::GiftNode ta;
			stream>>ta.mID>>ta.mNum;
			if( stream )
			{
				data.mGiftList.push_back(ta);
			}
			else
			{
				printf("GiftListNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(CollectBreakNotify,GS_Client_Collect_Break_Notify)
	MESSAGE_CLASS_INIT(CollectBreakNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(CollectBreakNotify)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(CollectBreakNotify)
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(SpeedNotify,GS_Client_speed_notify)
		int src_id;
		float speed;
	MESSAGE_CLASS_INIT(SpeedNotify)
		speed = 1.0f;
		src_id=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(SpeedNotify)
		stream<<data.src_id<<data.speed;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(SpeedNotify)
		stream>>data.src_id>>data.speed;
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(DoActionNotify,GS_Client_Do_Action_notify)
		int src_id;
		int action;
	MESSAGE_CLASS_INIT(DoActionNotify)
		action = 0;
		src_id=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(DoActionNotify)
		stream<<data.src_id<<data.action;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(DoActionNotify)
		stream>>data.src_id>>data.action;
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(AttackMissNotify,GS_Client_Attack_Miss_Notify)
		int src_id;
	MESSAGE_CLASS_INIT(AttackMissNotify)
		src_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(AttackMissNotify)
		stream<<data.src_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(AttackMissNotify)
		stream>>data.src_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ParamDataNotify,GS_Client_Param_Data_Notify)
		int srcid;
		int id;
		char desp[512];
	MESSAGE_CLASS_INIT(ParamDataNotify)
		srcid = 0;
		id = 0;
		desp[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ParamDataNotify)
		stream<<data.srcid<<data.id<<data.desp;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ParamDataNotify)
		stream>>data.srcid>>data.id>>data.desp;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ParamDataListNotify,GS_Client_Param_Data_List_Notify)
		int srcid;
		int num;
		struct ParamNode
		{
			int id;
			std::string desp;
		};
		std::vector<ParamNode>mParamList;
	MESSAGE_CLASS_INIT(ParamDataListNotify)
		srcid=0;
		num=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ParamDataListNotify)
		stream<<data.srcid<<data.num;
		for( std::vector<ParamDataListNotify::ParamNode>::const_iterator pos = data.mParamList.begin();
			pos != data.mParamList.end();
			++ pos )
		{
			stream<<pos->id<<pos->desp;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ParamDataListNotify)
		stream>>data.srcid>>data.num;
		for( int i = 0;i<size;i++ )
		{
			ParamDataListNotify::ParamNode ta;
			stream>>ta.id>>ta.desp;
			if( stream )
			{
				data.mParamList.push_back(ta);
			}
			else
			{
				printf("XinFaListNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PanelInfoNotify,GS_Client_Panel_Info_notify)
		int panelid;
		char desp[512];
	MESSAGE_CLASS_INIT(PanelInfoNotify)
		panelid = 0;
		desp[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PanelInfoNotify)
		stream<<data.panelid<<data.desp;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PanelInfoNotify)
		stream>>data.panelid>>data.desp;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ShowProgressBarNotify,GS_Client_Show_ProgressBar_Notify)
		int duration;
		char desp[512];
	MESSAGE_CLASS_INIT(ShowProgressBarNotify)
		duration=0;
		desp[0]=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ShowProgressBarNotify)
		stream<<data.duration<<data.desp;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ShowProgressBarNotify)
		stream>>data.duration>>data.desp;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(PushLuaTableNotify,GS_Client_PUSH_LUA_TABLE)
		std::string type;
		int flag;
		std::string table;
	MESSAGE_CLASS_INIT(PushLuaTableNotify)
		flag=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(PushLuaTableNotify)
		stream<<data.type<<data.flag<<data.table;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(PushLuaTableNotify)
		stream>>data.type>>data.flag>>data.table;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(FreshPanelInfoNotify,GS_Client_Fresh_Panel_Info_Notify)
		int panel_id;
	MESSAGE_CLASS_INIT(FreshPanelInfoNotify)
		panel_id=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(FreshPanelInfoNotify)
		stream<<data.panel_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(FreshPanelInfoNotify)
		stream>>data.panel_id;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(TiliChangeNotify,GS_Client_Tili_Change)
		int id;
		int power;
		int power_max;
	MESSAGE_CLASS_INIT(TiliChangeNotify)
		id = 0;
		power=0;
		power_max = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TiliChangeNotify)
		stream<<data.id<<data.power<<data.power_max;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TiliChangeNotify)
		stream>>data.id>>data.power>>data.power_max;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
		MESSAGE_CLASS_BEGIN(StatusDespNotify,GS_Client_Status_Desp_Notify)
		struct StatusNode
		{
			std::string desp;
			int lv;
			int icon;
			int ac;
			int acmax;
			int mac;
			int macmax;
			int dc;
			int dcmax;
			int mc;
			int mcmax;
			int sc;
			int scmax;
			int hpmax;
			int mpmax;
			int nodef;
			int baoji;
			int baoprob;
			int fightpoint;
			StatusNode()
			{
				lv = 0;
				icon = 0;
				ac = 0;
				acmax = 0;
				mac = 0;
				macmax = 0;
				dc = 0;
				dcmax = 0;
				mc = 0;
				mcmax = 0;
				sc = 0;
				scmax = 0;
				hpmax = 0;
				mpmax = 0;
				nodef = 0;
				baoji = 0;
				baoprob =0;
				fightpoint = 0;
			}
		};
		int id;
		std::vector<StatusNode>mStatusList;
	MESSAGE_CLASS_INIT(StatusDespNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(StatusDespNotify)
		stream<<data.id<<static_cast<int>(data.mStatusList.size());
		for( std::vector<StatusDespNotify::StatusNode>::const_iterator pos = data.mStatusList.begin();
			pos != data.mStatusList.end();
			++ pos )
		{
			stream<<pos->lv<<pos->icon<<pos->ac<<pos->acmax<<pos->mac<<pos->macmax<<pos->dc<<pos->dcmax<<pos->mc<<pos->mcmax<<pos->sc<<pos->scmax<<pos->hpmax<<pos->mpmax<<pos->nodef<<pos->fightpoint<<pos->baoji<<pos->baoprob<<pos->desp;
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(StatusDespNotify)
		int size=0;
		stream>>data.id>>size;
		for( int i = 0;i<size;i++ )
		{
			StatusDespNotify::StatusNode ta;
			stream>>ta.lv>>ta.icon>>ta.ac>>ta.acmax>>ta.mac>>ta.macmax>>ta.dc>>ta.dcmax>>ta.mc>>ta.mcmax>>ta.sc>>ta.scmax>>ta.hpmax>>ta.mpmax>>ta.nodef>>ta.fightpoint>>ta.baoji>>ta.baoprob>>ta.desp;
			if( stream )
			{
				data.mStatusList.push_back(ta);
			}
			else
			{
				printf("StatusDespNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListStatusNotify,GS_Client_List_Status_Notify)
	unsigned int src_id;
	int num;
	typedef struct STUNetStatus
	{
		int id;
		int dura;
		int param;
		float starttime;
		STUNetStatus()
		{
			id = 0;
			dura = 0;
			param = 0;
			starttime = 0;
		}

	} NetStatus;
	NetStatus sta;
	std::map<int,ListStatusNotify::NetStatus> m_StatusTemp;
	MESSAGE_CLASS_INIT(ListStatusNotify)
		src_id=0;
	num=0;
	//m_StatusTemp.clear();
	MESSAGE_CLASS_INIT_END
		MESSAGE_CLASS_END
		MESSAGE_CLASS_IMPORT(ListStatusNotify)
		stream<<data.src_id<<data.num;
	for (std::map<int,ListStatusNotify::NetStatus> ::const_iterator it = data.m_StatusTemp.begin();it!=data.m_StatusTemp.end();it++)
	{
		stream<<it->second.id<<it->second.dura<<it->second.param<<it->second.starttime;
	}
	MESSAGE_CLASS_IMPORT_END
		MESSAGE_CLASS_EXPORT(ListStatusNotify)
		stream>>data.src_id>>data.num;
	for (int i=0;i<data.num;i++)
	{
		ListStatusNotify::NetStatus ta;
		stream>>ta.id>>ta.dura>>ta.param>>ta.starttime;
		if (stream)
		{
			data.m_StatusTem[ta.id]=ta;
		}
	}
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MailsNotify,GS_Client_Mails_Res)
	struct Mail
	{
		std::string title;
		std::string content;
		std::string id;
		int date;
		int isOpen;
		int isReceive;
		std::map<int,int> items;
		Mail()
		{
			isOpen=0;
			isReceive=0;
		}
	};
	std::vector<Mail> mMails;
	MESSAGE_CLASS_INIT(MailsNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MailsNotify)
		stream<<static_cast<int>(data.mMails.size());
		for( std::vector<MailsNotify::Mail>::const_iterator pos = data.mMails.begin();
			pos != data.mMails.end();
			++ pos )
		{
			stream<<pos->id<<pos->title<<pos->content<<pos->date<<pos->isOpen<<pos->isReceive;
			stream<<static_cast<int>(pos->items.size());
			for(std::map<int,int>::const_iterator pos_item = pos->items.begin();
				pos_item!=pos->items.end();
				++pos_item)
			{
				stream<<pos_item->first<<pos_item->second;
			}
		}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MailsNotify)
		int size=0;
		stream>>size;
		for( int i = 0;i<size;i++ )
		{
			MailsNotify::Mail mail;
			stream>>pos->id>>mail.title>>mail.content>>mail.date>>mail.isOpen>>mail.isReceive;
			int item_size=0;
			stream>>item_size;
			for( int i = 0;i<item_size;i++ )
			{
				int key,value;
				stream>>key>>value;
				mail.items[key]=value;
			}
			if( stream )
			{
				data.mMails.push_back(mail);
			}
			else
			{
				printf("MailsNotify for size out stream :%d\n",size);
				break;
			}
		}
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(MailNumNotify,GS_Client_Mail_Num_Notify)
		int num;
	MESSAGE_CLASS_INIT(MailNumNotify)
		num=0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MailNumNotify)
		stream<<data.num;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MailNumNotify)
		stream>>data.num;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	//MESSAGE_CLASS_BEGIN(UpGradeInfoNotify, GS_Client_UpGrade_Info_Change)
	//	int dc_min_count;
	//	int dc_max_count;
	//	int mc_min_count;
	//	int mc_max_count;
	//	int sc_min_count;
	//	int sc_max_count;
	//	int ac_min_count;
	//	int ac_max_count;
	//	int mac_min_count;
	//	int mac_max_count;
	//	int hp_count;
	//	int mp_count;
	//	int monster_hurt_count;
	//	int up_lv_count;
	//MESSAGE_CLASS_INIT(UpGradeInfoNotify)
	//	dc_min_count=0;
	//	dc_max_count=0;
	//	mc_min_count=0;
	//	mc_max_count=0;
	//	sc_min_count=0;
	//	sc_max_count=0;
	//	ac_min_count=0;
	//	ac_max_count=0;
	//	mac_min_count=0;
	//	mac_max_count=0;
	//	hp_count=0;
	//	mp_count=0;
	//	monster_hurt_count=0;
	//	up_lv_count=0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(UpGradeInfoNotify)
	//stream << data.dc_min_count << data.dc_max_count << data.mc_min_count << data.mc_max_count << data.sc_min_count << data.sc_max_count << data.ac_min_count << data.ac_max_count << data.mac_min_count << data.mac_max_count << data.hp_count << data.mp_count << data.monster_hurt_count << data.up_lv_count;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(UpGradeInfoNotify)
	//stream >> data.dc_min_count >> data.dc_max_count >> data.mc_min_count >> data.mc_max_count >> data.sc_min_count >> data.sc_max_count >> data.ac_min_count >> data.ac_max_count >> data.mac_min_count >> data.mac_max_count >> data.hp_count >> data.mp_count >> data.monster_hurt_count >> data.up_lv_count;
	//MESSAGE_CLASS_EXPORT_END
	/////////////////////////////////////////////////////////////////////////////////////////////////// 
	//MESSAGE_CLASS_BEGIN(UpGradeWeaponInfoNotify, GS_Client_UpGrade_Weapon_Info)
	//	int pos;
	//	int dc_min;
	//	int dc_max;
	//	int mc_min;
	//	int mc_max;
	//	int sc_min;
	//	int sc_max;
	//	int ac_min;
	//	int ac_max;
	//	int mac_min;
	//	int mac_max;
	//	int hp;
	//	int mp;
	//	int percent;
	//MESSAGE_CLASS_INIT(UpGradeWeaponInfoNotify)
	//	pos = 0;
	//    dc_min = 0;
	//	dc_max = 0;
	//	mc_min = 0;
	//	mc_max = 0;
	//	sc_min = 0;
	//	sc_max = 0;
	//	ac_min = 0;
	//	ac_max = 0;
	//	mac_min = 0;
	//	mac_max = 0;
	//	hp = 0;
	//	mp = 0;
	//	percent = 0;
	//MESSAGE_CLASS_INIT_END
	//MESSAGE_CLASS_END
	//MESSAGE_CLASS_IMPORT(UpGradeWeaponInfoNotify)
	//stream << data.pos << data.dc_min << data.dc_max << data.mc_min << data.mc_max << data.sc_min << data.sc_max << data.ac_min << data.ac_max << data.mac_min << data.mac_max << data.hp << data.mp << data.percent;
	//MESSAGE_CLASS_IMPORT_END
	//MESSAGE_CLASS_EXPORT(UpGradeWeaponInfoNotify)
	//stream >> data.pos >> data.dc_min >> data.dc_max >> data.mc_min >> data.mc_max >> data.sc_min >> data.sc_max >> data.ac_min >> data.ac_max >> data.mac_min >> data.mac_max >> data.hp >> data.mp >> data.percent;
	//MESSAGE_CLASS_EXPORT_END
	/////////////////////////////////////////////////////////////////////////////////////////////////// 

	MESSAGE_CLASS_BEGIN(MailReceiveSuccessNotify,GS_Client_Mail_Receive_Success_Notify)
		std::string mailID;
	MESSAGE_CLASS_INIT(MailReceiveSuccessNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(MailReceiveSuccessNotify)
		stream<<data.mailID;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(MailReceiveSuccessNotify)
		stream>>data.mailID;
	MESSAGE_CLASS_EXPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListChargeDartNotify,GS_Client_List_ChargeDart_Notify)
		struct ChargeDart
		{
			std::string mCharName;
			int mIcon;
			int mSurplusTime;
			int mDuration;
			int mFightingForce;
			int mStolenTimes;
			int mTotalAwards;
			int mSurplusAwards;
			int mState;
			std::string mRobDartCharNames;
			ChargeDart()
			{
				mIcon = 0;
				mSurplusTime = 0;
				mDuration = 0;
				mFightingForce = 0;
				mStolenTimes = 0;
				mTotalAwards = 0;
				mSurplusAwards = 0;
				mState = 0;
			}
		};
		std::vector<ListChargeDartNotify::ChargeDart> m_ChargeDartList;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListChargeDartNotify)
		stream<<static_cast<int>(data.m_ChargeDartList.size());
		for (std::vector<ListChargeDartNotify::ChargeDart> ::const_iterator it = data.m_ChargeDartList.begin();it!=data.m_ChargeDartList.end();it++)
		{
			stream<<it->mCharName<<it->mIcon<<it->mSurplusTime<<it->mDuration<<it->mFightingForce<<it->mStolenTimes<<it->mTotalAwards<<it->mSurplusAwards<<it->mState<<it->mRobDartCharNames;
		}
	MESSAGE_CLASS_IMPORT_END
///////////////////////////////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ConsignItemNotify,GS_Client_Consign_Item_Notify)
	int ret;
	MESSAGE_CLASS_INIT(ConsignItemNotify)
	ret = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ConsignItemNotify)
	stream<<data.ret;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ConsignItemNotify)
	stream>>data.ret;
	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(ConsignableItemsNotify,GS_Client_ConsignableItems_Notify)
		struct ConsignableItem
		{
			int id;
			int index;
			int price;
			int time_left;

			int mTypeID;
			int mDuraMax;
			int mDuration;
			int mItemFlags;
			short mLuck;
			int mLevel;
			int mNumber;
			short mAddAC;
			short mAddMAC;
			short mAddDC;
			short mAddMC;
			short mAddSC;
			short mAddHp;
			short mAddMp;
			short mUpdAC;
			short mUpdMAC;
			short mUpdDC;
			short mUpdMC;
			short mUpdSC;

			short mProtect;
			int mUpdMaxCount;
			int mUpdFailedCount;
			int mZLevel;
			int mLock;
		};
		int type;
		int job;
		int condition;
		std::vector<ConsignableItemsNotify::ConsignableItem> m_ItemsList;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ConsignableItemsNotify)
	stream<<data.type<<data.job<<data.condition<<static_cast<int>(data.m_ItemsList.size());
	for (std::vector<ConsignableItemsNotify::ConsignableItem> ::const_iterator it = data.m_ItemsList.begin();it!=data.m_ItemsList.end();it++)
	{
		stream<<it->id
			<<it->index
			<<it->price
			<<it->time_left
			<<it->mTypeID
			<<it->mDuraMax
			<<it->mDuration
			<<it->mItemFlags
			<<it->mLuck
			<<it->mLevel
			<<it->mNumber
			<<it->mAddAC
			<<it->mAddMAC
			<<it->mAddDC
			<<it->mAddMC
			<<it->mAddSC
			<<it->mAddHp
			<<it->mAddMp
			<<it->mUpdAC
			<<it->mUpdMAC
			<<it->mUpdDC
			<<it->mUpdMC
			<<it->mUpdSC
			<<it->mProtect
			<<it->mUpdMaxCount
			<<it->mUpdFailedCount
			<<it->mZLevel
			<<it->mLock;
	}
	MESSAGE_CLASS_IMPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(BuyConsignableItemNotify,GS_Client_Buy_Consignable_Item_Notify)
	int ret;
	int seed_id;
	MESSAGE_CLASS_INIT(BuyConsignableItemNotify)
	ret = 0;
	int seed_id;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(BuyConsignableItemNotify)
	stream<<data.ret<<data.seed_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(BuyConsignableItemNotify)
	stream>>data.ret<<data.seed_id;
	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(TakeBackConsignableItemNotify,GS_Client_Take_Back_Consignable_Item_Notify)
	int ret;
	int seed_id;
	MESSAGE_CLASS_INIT(TakeBackConsignableItemNotify)
	ret = 0;
	seed_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TakeBackConsignableItemNotify)
		stream<<data.ret<<data.seed_id;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TakeBackConsignableItemNotify)
		stream>>data.ret>>data.seed_id;
	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 

	MESSAGE_CLASS_BEGIN(TakeBackVCoinNotify,GS_Client_Take_Back_VCoin_Notify)
	int ret;
	MESSAGE_CLASS_INIT(TakeBackVCoinNotify)
	ret = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(TakeBackVCoinNotify)
		stream<<data.ret;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(TakeBackVCoinNotify)
		stream>>data.ret;
	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(GuildRedPacketLogRes,GS_Client_Guild_Red_Packet_Log_Res)
	struct GuildRedPacketLog
	{
		std::string sender;
		int op_code;
		int value;
		int count;
	};
	std::vector<GuildRedPacketLogRes::GuildRedPacketLog> m_LogList;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildRedPacketLogRes)
	stream<<static_cast<int>(data.m_LogList.size());
	for (std::vector<GuildRedPacketLogRes::GuildRedPacketLog> ::const_iterator it = data.m_LogList.begin();it!=data.m_LogList.end();it++)
	{
		stream<<it->sender<<it->op_code<<it->value<<it->count;
	}
	MESSAGE_CLASS_IMPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
    MESSAGE_CLASS_BEGIN(GuildRedPacketLogNotify,GS_Client_Guild_Red_Packet_Log_Notify)
	std::string sender;
	int op_code;
	int value;
	int count;
	MESSAGE_CLASS_INIT(GuildRedPacketLogNotify)
	op_code = 0;
	value = 0;
	count = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildRedPacketLogNotify)
	stream<<data.sender << data.op_code << data.value << data.count;
	MESSAGE_CLASS_IMPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(GuildItemLogRes,GS_Client_Guild_Item_Log_Res)
	struct GuildItemLog
	{
		std::string name;
		// 物品id
		std::string item_name;
		// 操作类型
		int op_code;
		// 时间
		int time;
	};
	std::vector<GuildItemLogRes::GuildItemLog> m_LogList;
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildItemLogRes)
	stream<<static_cast<int>(data.m_LogList.size());
	for (std::vector<GuildItemLogRes::GuildItemLog> ::const_iterator it = data.m_LogList.begin();it!=data.m_LogList.end();it++)
	{
		stream<<it->name<<it->item_name<<it->op_code<<it->time;
	}
	MESSAGE_CLASS_IMPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(GuildMemberChangeNotify,GS_Client_GuildMemberChgNotify)
	MESSAGE_CLASS_INIT(GuildMemberChangeNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildMemberChangeNotify)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GuildMemberChangeNotify)
	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(GuildWarNotify,GS_Client_Guild_War_Notify)
	MESSAGE_CLASS_INIT(GuildWarNotify)
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(GuildWarNotify)
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(GuildWarNotify)
	MESSAGE_CLASS_EXPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////// 
	MESSAGE_CLASS_BEGIN(BuffDespNotify,GS_Client_Buff_Desp_Notify)
	int id;
	std::string name;
	std::string icon;
	int ui;
	int uisort;
	int type;
	int lv;
	std::string desp;
	int eff_type;
	int eff_id;
	int validTimeMax;
	MESSAGE_CLASS_INIT(BuffDespNotify)
	id = 0;
	ui = 0;
	uisort = 0;
	type = 0;
	lv = 0;
	eff_type = 0;
	eff_id = 0;
	validTimeMax = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(BuffDespNotify)
	stream<<data.id<<data.name<<data.icon<<data.ui<<data.uisort<<data.type<<data.lv<<data.desp<<data.eff_type<<data.eff_id<<data.validTimeMax;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(BuffDespNotify)
	stream>>data.id>>data.name>>data.icon>>data.ui>>data.uisort>>data.type>>data.lv>>data.desp>>data.eff_type>>data.eff_id>>data.validTimeMax;
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(BuffChangeNotify,GS_Client_Buff_Change_Notify)
	int src_id;
	int id;
	int op_code;
	double time_left;
	MESSAGE_CLASS_INIT(BuffChangeNotify)
	src_id = 0;
	id = 0;
	op_code = 0;
	time_left = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(BuffChangeNotify)
	stream<<data.src_id<<data.id<<data.op_code<<data.time_left;
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(BuffChangeNotify)
	stream>>data.src_id>>data.id>>data.op_code>>data.time_left;
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////
	MESSAGE_CLASS_BEGIN(ListBuffNotify,GS_Client_List_Buff_Notify)
	int src_id;
	struct NetBuff
	{
		int id;
		double time_left;
		NetBuff()
		{
			id = 0;
			time_left = 0;
		}
	};
	std::vector<NetBuff> buff_list;
	MESSAGE_CLASS_INIT(ListBuffNotify)
	src_id = 0;
	MESSAGE_CLASS_INIT_END
	MESSAGE_CLASS_END
	MESSAGE_CLASS_IMPORT(ListBuffNotify)
	stream<<data.src_id<<static_cast<int>(data.buff_list.size());
	for (std::vector<ListBuffNotify::NetBuff>::const_iterator it = data.buff_list.begin();it!=data.buff_list.end();it++)
	{
		stream<<it->id<<it->time_left;
	}
	MESSAGE_CLASS_IMPORT_END
	MESSAGE_CLASS_EXPORT(ListBuffNotify)
	int size=0;
	stream>>data.src_id>>size;
	for (int i=0;i<size;i++)
	{
		ListBuffNotify::NetBuff nb;
		stream>>nb.id>>nb.time_left;
		if (stream)
		{
			data.buff_list.push_back(nb);
		}
	}
	MESSAGE_CLASS_EXPORT_END
//////////////////////////////////////////////////////////////////////////

}