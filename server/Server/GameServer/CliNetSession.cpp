#include "stdafx.h"
#include "ClinetSession.h"
#include "leudgrid/StreamBlock.h"
#include "leudgrid/Cryptography.h"
#include "GameServerApp.h"
#include "KuaFuManager.h"
#include "KuaFuService.h"
#include "DBManager.h"
#include "Protocol/Client_GS.h"
#include "SessionManager.h"
#include "GameServerApp.h"
#include "GameServerLib/GameServerLib.h"
#include "GameServerLib/PlayMapObject.h"
#include "GameServerLib/DomainPlay.h"
#include "GameServerLib/PlayNpc.h"
#include "GameServerLib/PlayMap.h"
#include "GameServerLib/SubItem.h"
#include "GameServerLib/SubSkill.h"
#include "GameServerLib/SubTask.h"
#include "GameServerLib/SubMail.h"
#include "GameServerLib/ManagerLevelInfo.h"
#include "GameServerLib/ManagerTotalAttr.h"
//#include "GameServerLib/ManagerReincarnation.h"
#include "GameServerLib/UtilString.h"
#include "GameServerLib/DomainData.h"
#include "GameServerLib/ManagerGuild.h"
#include "GameServerLib/ManagerConsignment.h"
#include "GameServerLib/GuildGuild.h"
#include "GameServerLib/ManagerGroup.h"
#include "GameServerLib/DomainGroup.h"
#include "GameServerLib/ManagerItemDef.h"
#include "GameServerLib/ManagerSkillDef.h"
//#include "GameServerLib/ManagerRobot.h"
#include "GameServerLib/CoupleCouple.h"
#include "GameServerLib/DomainCouple.h"
#include "GameServerLib/ManagerCouple.h"
#include "GameServerLib/GroupGroup.h"
#include "GameServerLib/DomainGuild.h"
#include "GameServerLib/PlayMapItem.h"
#include "GameServerLib/PlayMonster.h"
//#include "GameServerLib/PlayRobot.h"
#include "GameServerLib/ManagerMap.h"
#include "GameServerLib/ManagerFilter.h"
#include "GameServerLib/ManagerChart.h"
#include "GameServerLib/PlayNeutral.h"
#include "GameServerLib/PlaySlave.h"
//#include "GameServerLib/PlayRobot.h"
#include "GameServerLib/UtilString.h"
#include "GameServerLib/PlayMonsterHelper.h"
#include "GameServerLib/ManagerTask.h"
#include "GameServerLib/ManagerFly.h"
//#include "GameServerLib/ManagerOnSale.h"
#include "GameServerLib/ManagerStatus.h"
#include "GameServerLib/ManagerChargeDart.h"
#include "GameServerLib/ScriptLua.h"
#include "GameServerLib/ManagerMail.h"
#include "GameServerLib/SubFriend.h"
#include "GameServerLib/ManagerSwordDef.h"


CCliNetSession::CCliNetSession(CGameServerApp* app) : m_app(app)
{
	mConnectedTime=time(0);
	mConnectedMsgTime=0;
	mGameSession = 0;
	mGamePlayer = 0;
	mSessionStartTick = 0;
	mAuthType=0;
	mNewYouKe=0;
	mAuthed=false;
	mNetCloseBreak = false;
	mChatTick = 0;
	mSearchTick = 0;
	mPing = 0;
	mPingTick=0;
	mPingUpdateTick=0;
	mFirstPingUpdateTick = 0;
	mPingStopAction = 0;
	mClientTimeTag = 0;
	mServerTimeTag = 0;
	mUseSkillSendTag = 0;
	mUseItemSendTag = 0;
	mCheckItemUse = 0;
	mMoveSendTag = 0;
	mClientTimeErrorCount = 0;
	mUseItemCountPerSecond=0;
	mUseItemCountSecontTick=0;
	mServerTimeResetDuration = 60;

	mItemUseTick = 0;

	mAs2LuaCountPerSecond = 0;
	mAs2LuaSecontTick=0;
	mRunWalkCountPerSecond = 0;
	mRunWalkCountTick = 0;
	mTxSystemFlag = 0;

	mIsKuafuBack = false;
}
CCliNetSession::~CCliNetSession(void)
{
}
void CCliNetSession::NetBreak()
{
	mGameSession = 0;
	mGamePlayer = 0;
	mNetCloseBreak = true;
}
void CCliNetSession::Open( SOCKET sock )
{
	LEUD::TCPSessionMini::Open(sock);
}
void CCliNetSession::OnClose( DWORD Error )
{
	if( mGameSession )
	{
		mGameSession->EndSession();
		mGameSession = 0;
	}
	if( m_app->mKuaFuService && m_app->m_KuaFuService )
	{
		m_app->m_KuaFuService->RemoveCallBackSession(mKuafuSIP.c_str(),mKuafuSPort.c_str(),this);
	}
	Close();
	delete this;
}
void CCliNetSession::OnMessageKuaFu(unsigned short cmdtype,LEUD::StreamFix& instream)
{
	//	return;
	switch( cmdtype )
	{
	case Client_GS::KuaFuGetCharDataReq::MSG_ID:///���player������ҵĻ�������?
		{
			Client_GS::KuaFuGetCharDataReq req;instream>>req;
			if(instream)
			{
				//char temploginid[256];sexy_db_mysql::hex_string(temploginid,req.mLoginID.c_str(),req.mLoginID.length());
				//char tempcharname[256];sexy_db_mysql::hex_string(tempcharname,req.mCharID.c_str(),req.mCharID.length());
				//char tempseedname[256];sexy_db_mysql::hex_string(tempseedname,req.mSeedName.c_str(),req.mSeedName.length());
				std::string _charID = req.mCharID;
				_charID.erase(0,m_app->m_pGameServerLib->mServerTitle.size());
				std::string _loginID = req.mLoginID;
				_loginID.erase(0,m_app->m_pGameServerLib->mServerTitle.size());
				char sql[512];sprintf(sql,"select chrname,seedname,account,exp,vcoin,lv,job,gender,guild,reincarnation,drug,item,item_bag,skill,pet,shortcut,var,intvar,status from player where chrname='%s' and account='%s' and seedname='%s'",_charID.c_str(),_loginID.c_str(),req.mSeedName.c_str());
				DBCallBackContent* callback=new DBCallBackContent;
				callback->calltype=100;
				callback->sock=*this;
				strcpy(callback->loginid,req.mLoginID.c_str());
				strcpy(callback->charname,req.mCharID.c_str());
				m_app->m_pDBManager->push_store_sql(callback,sql);
			}
		}
		break;
	case Client_GS::KuaFuSetParamReq::MSG_ID:
		{
			Client_GS::KuaFuSetParamReq req;instream>>req;
			if(instream)
			{
				if( req.mParamKey.substr(0,1)=="#" )
				{
					m_app->m_pGameServerLib->Var(req.mParamKey,req.mParamValue);
				}
			}
		}
		break;
	case Client_GS::KuaFuPlayerDataReq::MSG_ID:
		{
			Client_GS::KuaFuPlayerDataReq req;instream>>req;
			if(instream)
			{
				m_app->m_pGameServerLib->Var("kuafuseekname",req.seekName);
				m_app->m_pGameServerLib->Var("kuafupktype",req.pkType);
				m_app->m_pGameServerLib->Var("kuafuvalue",req.value);
			}
		}
		break;
	case Client_GS::KuaFuComeBackDataReq::MSG_ID:////�ӿ���������?����������ʱ�����������?
		{
			Client_GS::KuaFuComeBackDataReq req;instream>>req;
			if(instream)
			{
				if(!req.winArea.empty())m_app->m_pGameServerLib->Var("kuafuarea",req.winArea);
				if(!req.winGuild.empty())m_app->m_pGameServerLib->Var("kuafuguild",req.winGuild);
				req.charID.erase(0,m_app->m_pGameServerLib->mServerTitle.size());
				req.account.erase(0,m_app->m_pGameServerLib->mServerTitle.size());
				int result = m_app->m_pGameServerLib->SetKuaFuComeBackData(req.charID,req.vcoin,req.result);///�������DomainData�е�����
				if(!result)
				{
					m_app->m_pGameServerLib->GetListener()->SaveKuafuCharacter(req.charID,req.account,req.vcoin,req.result);///�޸�������ݿ��е�����?
				}
			}
		}
		break;
	default:
		break;
	}
	return;
}
void CCliNetSession::OnMessage( const char* msg,size_t size )
{
#ifdef _PERFORMANCE
	m_app->IncInMsgCount();
#endif
	if( mNetCloseBreak )
	{
		GS_Client::SessionClosedNotify notify;
		strncpy_s(notify.msg ,_countof(notify.msg), "",511);
		notify.msg[511]=0;
		Send_Message(notify);
		return;
	}
	if(msg[0]=='t'&&msg[1]=='g'&&msg[2]=='w'&&msg[3]=='_')
	{
		mTxSystemFlag = 1;
		return;
	}
	LEUD::StreamFix instream(msg,size,size);
	char head[20];
	instream.Out(head,Client_GS::HeadLen());
	unsigned short cmdtype;
	instream>>cmdtype;
	instream.seek(instream.seek()-sizeof(unsigned short));
	if( (!mAuthed) && (cmdtype != Client_GS::AuthenticateREQ::MSG_ID) && (cmdtype != Client_GS::ClientGSKuafuAuth::MSG_ID) )
	{
		return;
	}
	if(mAuthed && mAuthType == 110 )///////����ڿ���������н�����е��?��Э��(�����������г�ȥ֮��)
	{
		OnMessageKuaFu(cmdtype,instream);
		return;
	}
	if( (cmdtype != Client_GS::AuthenticateREQ::MSG_ID)
		&& (cmdtype != Client_GS::ClientGSKuafuAuth::MSG_ID)
		&& (cmdtype != Client_GS::ListCharacterReq::MSG_ID)
		&& (cmdtype != Client_GS::CreateCharacterReq::MSG_ID)
		&& (cmdtype != Client_GS::DeleteCharacterReq::MSG_ID)
		&& (cmdtype != Client_GS::EnterGameReq::MSG_ID) 
		&& (cmdtype != Client_GS_Update_Ticket) 
		)
	{
		if( ! mGamePlayer ) return ;
		if( ! mGameSession ) return ;
		if( mGameSession->GetState() != OGSLib::GameSession::SESSION_STATE_RUN ) return;
		if( cmdtype != Client_GS::PingReq::MSG_ID )
		{
			int updatetick = mGameSession->GetGameServerLib()->UpdateTick();
			if( mSessionStartTick == 0 ){mSessionStartTick = updatetick;}
			if( updatetick - mSessionStartTick > 10 ){
				//if( mPing == 0 ){
				//	//printf("ping zero\n");
				//	return;
				//}
				//time_t t = time(0);
				//if( mPingTick + mPing + 10 < t ){
				//	//printf("ping slower %5d %5d %5d\n",mPingTick,mPing,t);
				//	return;
				//}
			}
		}
	}
	//printf("cmdtype:%x\n",cmdtype);//Э��
	switch( cmdtype )
	{
	case Client_GS::AuthenticateREQ::MSG_ID:
		{
			PERF_NODE("ProcessAuthenticate");
			ProcessAuthenticate(instream);
		}
		break;
	case Client_GS::ClientGSKuafuAuth::MSG_ID:
		{
			ProcessClientGSKuafuAuth(instream);
		}
		break;
	case Client_GS_Update_Ticket:
		{
			PERF_NODE("ProcessUpdateTicket");
			ProcessUpdateTicket(instream);
		}
		break;
	case Client_GS::MapChatReq::MSG_ID:
		{
			PERF_NODE("ProcessMapChat");
			ProcessMapChat(instream);
		}
		break;
	case Client_GS::PrivateChatReq::MSG_ID:
		{
			PERF_NODE("ProcessPrivateChat");
			ProcessPrivateChat(instream);
		}
		break;
	case Client_GS::GuildChatReq::MSG_ID:
		{
			PERF_NODE("ProcessGuildChat");
			ProcessGuildChat(instream);
		}
		break;
	case Client_GS::GroupChatReq::MSG_ID:
		{
			PERF_NODE("ProcessGroupChat");
			ProcessGroupChat(instream);
		}
		break;
	case Client_GS::NormalChatReq::MSG_ID:
		{
			PERF_NODE("ProcessNormalChat");
			ProcessNormalChat(instream);
		}
		break;
	case Client_GS::WorldChatReq::MSG_ID:
		{
			PERF_NODE("ProcessWorldChat");
			ProcessWorldChat(instream);
		}
		break;
	case Client_GS::WalkReq::MSG_ID:
		{
			PERF_NODE("ProcessWalk");
			ProcessWalk(instream);
		}
		break;
	case Client_GS::RunReq::MSG_ID:
		{
			PERF_NODE("ProcessRun");
			ProcessRun(instream);
		}
		break;
	case Client_GS::NPCTalkReq::MSG_ID:
		{
			PERF_NODE("ProcessNPCTalk");
			ProcessNPCTalk(instream);
		}
		break;
	case Client_GS::TurnReq::MSG_ID:
		{
			PERF_NODE("ProcessTurn");
			ProcessTurn(instream);
		}
		break;
	case Client_GS::AttackReq::MSG_ID:
		{
			PERF_NODE("ProcessAttack");
			ProcessAttack(instream);
		}
		break;
	case Client_GS::PickUpReq::MSG_ID:
		{
			PERF_NODE("ProcessPickUp");
			ProcessPickUp(instream);
		}
		break;
	case Client_GS::BagUseItemReq::MSG_ID:
		{
			PERF_NODE("ProcessBagUseItem");
			ProcessBagUseItem(instream);
		}
		break;
	case Client_GS::UndressItemReq::MSG_ID:
		{
			PERF_NODE("ProcessUndressItem");
			ProcessUndressItem(instream);
		}
		break;
	case Client_GS::ItemPostionExchangeReq::MSG_ID:
		{
			PERF_NODE("ProcessItemPositionExchange");
			ProcessItemPositionExchange(instream);
		}
		break;
	case Client_GS::UseSkillReq::MSG_ID:
		{
			PERF_NODE("ProcessUseSkill");
			ProcessUseSkill(instream);
		}
		break;
	case Client_GS::NpcShopReq::MSG_ID:
		{
			PERF_NODE("ProcessNpcShop");
			ProcessNpcShop(instream);
		}
		break;
	case Client_GS::NpcBuyReq::MSG_ID:
		{
			PERF_NODE("ProcessNpcBuy");
			ProcessNpcBuy(instream);
		}
		break;
	case Client_GS::TaskCancelReq::MSG_ID:
		{
			PERF_NODE("ProcessCancelTask");
			ProcessCancelTask(instream);
		}
		break;
	case Client_GS::ForceMoveReq::MSG_ID:
		{
			PERF_NODE("ProcessForceMove");
			ProcessForceMove(instream);
		}
		break;
	case Client_GS::ChangeClothReq::MSG_ID:
		{
			PERF_NODE("ProcessChangeCloth");
			ProcessChangeCloth(instream);
		}
		break;
	case Client_GS::ListCharacterReq::MSG_ID:
		{
			PERF_NODE("ProcessListCharacter");
			ProcessListCharacter(instream);
		}
		break;
	case Client_GS::CreateCharacterReq::MSG_ID:
		{
			PERF_NODE("ProcessCreateCharacter");
			ProcessCreateCharacter(instream);
		}
		break;
	case Client_GS::EnterGameReq::MSG_ID:
		{
			PERF_NODE("ProcessEnterGame");
			ProcessEnterGame(instream);
		}
		break;
	case Client_GS::DeleteCharacterReq::MSG_ID:
		{
			PERF_NODE("ProcessDeleteCharacter");
			ProcessDeleteCharacter(instream);
		}
		break;
	case Client_GS::DropItemReq::MSG_ID:
		{
			PERF_NODE("ProcessDropItem");
			ProcessDropItem(instream);
		}
		break;
	case Client_GS::NPCSellReq::MSG_ID:
		{
			PERF_NODE("ProcessNPCSell");
			ProcessNPCSell(instream);
		}
		break;
	case Client_GS::SaveShortcutReq::MSG_ID:
		{
			PERF_NODE("ProcessSaveShortcut");
			ProcessSaveShortcut(instream);
		}
		break;
	case Client_GS::ListGuildReq::MSG_ID:
		{
			PERF_NODE("ProcessListGuild");
			ProcessListGuild(instream);
		}
		break;
	case Client_GS::GetGuildInfoReq::MSG_ID:
		{
			PERF_NODE("ProcessGetGuildInfo");
			ProcessGetGuildInfo(instream);
		}
		break;
	case Client_GS::CreateGuildReq::MSG_ID:
		{
			PERF_NODE("ProcessCreateGuild");
			ProcessCreateGuild(instream);
		}
		break;
	case Client_GS::JoinGuildReq::MSG_ID:
		{
			PERF_NODE("ProcessJoinGuild");
			ProcessJoinGuild(instream);
		}
		break;
	case Client_GS::SetGuildInfoReq::MSG_ID:
		{
			PERF_NODE("ProcessSetGuildInfo");
			ProcessSetGuildInfo(instream);
		}
		break;
	case Client_GS::ListGuildMemberReq::MSG_ID:
		{
			PERF_NODE("ProcessListGuildMember");
			ProcessListGuildMember(instream);
		}
		break;
	case Client_GS::ListGuildEnemyReq::MSG_ID:
		{
			PERF_NODE("ProcessListGuildEnemy");
			ProcessListGuildEnemy(instream);
		}
		break;
	case Client_GS::ListGuildFriendReq::MSG_ID:
		{
			PERF_NODE("ProcessListGuildFriend");
			ProcessListGuildFriend(instream);
		}
		break;
	case Client_GS::ListGuildItemReq::MSG_ID:
		{
			PERF_NODE("ProcessListGuildItem");
			ProcessListGuildItem(instream);
		}
		break;
	case Client_GS::ChangeGuildMemberTitleReq::MSG_ID:
		{
			PERF_NODE("ProcessChangeGuildMemberTitle");
			ProcessChangeGuildMemberTitle(instream);
		}
		break;
	case Client_GS::ChangeEnemyGuildReq::MSG_ID:
		{
			PERF_NODE("ProcessChangeEnemyGuild");
			ProcessChangeEnemyGuild(instream);
		}
		break;
	case Client_GS::ChangeFriendGuildReq::MSG_ID:
		{
			PERF_NODE("ProcessChangeFriendGuild");
			ProcessChangeFriendGuild(instream);
		}
		break;
	case Client_GS_Guild_Red_Packet_Log_Req:
		{
			PERF_NODE("ProcessRedPacketLogReq");
			ProcessRedPacketLogReq(instream);
		}
		break;
	case Client_GS_Guild_Item_Log_Req:
		{
			PERF_NODE("ProcessGuildItemLogReq");
			ProcessGuildItemLogReq(instream);
		}
		break;
	case Client_GS::ChangeAttackModeReq::MSG_ID:
		{
			PERF_NODE("ProcessChangeAttackMode");
			ProcessChangeAttackMode(instream);
		}
		break;
	//case Client_GS::MarryInviteReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessMarryInvite");
	//		ProcessMarryInvite(instream);
	//	}
	//	break;
	//case Client_GS::AgreeOrNotMarryInviteReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessAgreeOrNotMarryInvite");
	//		ProcessAgreeOrNotMarryInvite(instream);
	//	}
	//	break;
	//case Client_GS::AgreeOrNotDivorceInviteReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessAgreeOrNotDivorceInvite");
	//		ProcessAgreeOrNotDivorceInvite(instream);
	//	}
	//	break;
	case Client_GS::VcoinShopListReq::MSG_ID:
		{
			PERF_NODE("ProcessVcoinShopList");
			ProcessVcoinShopList(instream);
		}
		break;
	case Client_GS::NPCRepairReq::MSG_ID:
		{
			PERF_NODE("ProcessNPCRepair");
			ProcessNPCRepair(instream);
		}
		break;
	case Client_GS::ReliveReq::MSG_ID:
		{
			PERF_NODE("ProcessRelive");
			ProcessRelive(instream);
		}
		break;
	case Client_GS::TaskDespReq::MSG_ID:
		{
			PERF_NODE("ProcessTaskDesp");
			ProcessTaskDesp(instream);
		}
		break;
	case Client_GS::InfoPlayerReq::MSG_ID:
		{
			PERF_NODE("ProcessInfoPlayer");
			ProcessInfoPlayer(instream);
		}
		break;
	case Client_GS::CreateGroupReq::MSG_ID:
		{
			PERF_NODE("ProcessCreateGroup");
			ProcessCreateGroup(instream);
		}
		break;
	case Client_GS::LeaveGroupReq::MSG_ID:
		{
			PERF_NODE("ProcessLeaveGroup");
			ProcessLeaveGroup(instream);
		}
		break;
	case Client_GS::JoinGroupReq::MSG_ID:
		{
			PERF_NODE("ProcessJoinGroup");
			ProcessJoinGroup(instream);
		}
		break;
	case Client_GS::AgreeJoinGroupReq::MSG_ID:
		{
			PERF_NODE("ProcessAgreeJoinGroup");
			ProcessAgreeJoinGroup(instream);
		}
		break;
	case Client_GS::InviteGroupReq::MSG_ID:
		{
			PERF_NODE("ProcessInviteGroup");
			ProcessInviteGroup(instream);
		}
		break;
	case Client_GS::AgreeInviteGroupReq::MSG_ID:
		{
			PERF_NODE("ProcessAgreeInviteGroup");
			ProcessAgreeInviteGroup(instream);
		}
		break;
	case Client_GS::TaskClickReq::MSG_ID:
		{
			PERF_NODE("ProcessTaskClick");
			ProcessTaskClick(instream);
		}
		break;
	case Client_GS::TradeInviteReq::MSG_ID:
		{
			PERF_NODE("ProcessTradeInvite");
			ProcessTradeInvite(instream);
		}
		break;
	case Client_GS::AgreeTradeInviteReq::MSG_ID:
		{
			PERF_NODE("ProcessAgreeTradeInvite");
			ProcessAgreeTradeInvite(instream);
		}
		break;
	case Client_GS::CloseTradeReq::MSG_ID:
		{
			PERF_NODE("ProcessCloseTrade");
			ProcessCloseTrade(instream);
		}
		break;
	case Client_GS::TradeAddGameMoneyReq::MSG_ID:
		{
			PERF_NODE("ProcessTradeAddGameMoney");
			ProcessTradeAddGameMoney(instream);
		}
		break;
	case Client_GS::TradeAddVcoinReq::MSG_ID:
		{
			PERF_NODE("ProcessTradeAddVcoin");
			ProcessTradeAddVcoin(instream);
		}
		break;
	case Client_GS::TradeSubmitReq::MSG_ID:
		{
			PERF_NODE("ProcessTradeSubmit");
			ProcessTradeSubmit(instream);
		}
		break;
	case Client_GS::GroupSetLeaderReq::MSG_ID:
		{
			PERF_NODE("ProcessGroupSetLeader");
			ProcessGroupSetLeader(instream);
		}
		break;
	case Client_GS::TradeAddItemReq::MSG_ID:
		{
			PERF_NODE("ProcessTradeAddItem");
			ProcessTradeAddItem(instream);
		}
		break;
	case Client_GS::ChatTradeSubmitReq::MSG_ID:
		{
			PERF_NODE("ProcessChatTradeSubmit");
			ProcessChatTradeSubmit(instream);
		}
		break;
	case Client_GS::DestoryItemReq::MSG_ID:
		{
			PERF_NODE("ProcessDestoryItem");
			ProcessDestoryItem(instream);
		}
		break;
	case Client_GS::SortItemReq::MSG_ID:
		{
			PERF_NODE("ProcessSortItem");
			ProcessSortItem(instream);
		}
		break;
	case Client_GS::ItemTalkReq::MSG_ID:
		{
			PERF_NODE("ProcessItemTalk");
			ProcessItemTalk(instream);
		}
		break;
	//case Client_GS::MergeSteelReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessMergeSteel");
	//		ProcessMergeSteel(instream);
	//	}
	//	break;
	//case Client_GS::UpgradeEquipReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessUpgradeEquip");
	//		ProcessUpgradeEquip(instream);
	//	}
	//	break;
	case Client_GS::FreshVcoinReq::MSG_ID:
		{
			PERF_NODE("ProcessFreshVcoin");
			ProcessFreshVcoin(instream);
		}
		break;
	case Client_GS::LeaveGuildReq::MSG_ID:
		{
			PERF_NODE("ProcessLeaveGuild");
			ProcessLeaveGuild(instream);
		}
		break;
	case Client_GS::AddDepotSlotReq::MSG_ID:
		{
			PERF_NODE("ProcessAddDepotSlot");
			ProcessAddDepotSlot(instream);
		}
		break;
	case Client_GS::GroupPickModeReq::MSG_ID:
		{
			PERF_NODE("ProcessGroupPickMode");
			ProcessGroupPickMode(instream);
		}
		break;
	case Client_GS_Swith_Slave_AI_Mode:
		{
			PERF_NODE("ProcessSwithSlaveAIMode");
			ProcessSwithSlaveAIMode(instream);
		}
		break;
	case Client_GS_Ping:
		{
			PERF_NODE("ProcessPing");
			ProcessPing(instream);
		}
		break;
	case Client_GS_FreshHPMP:
		{
			PERF_NODE("ProcessFreshHPMP");
			ProcessFreshHPMP(instream);
		}
		break;
	//case Client_GS::UpdateChinaLimitReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessUpdateChinaLimit");
	//		ProcessUpdateChinaLimit(instream);
	//	}
	//	break;
	//case Client_GS::BuyOfflineExpReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessBuyOfflineExp");
	//		ProcessBuyOfflineExp(instream);
	//	}
	//	break;
	//case Client_GS::SteelEquipReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessSteelEquip");
	//		ProcessSteelEquip(instream);
	//	}
	//	break;
	case Client_GS::PlayerTalkReq::MSG_ID:
		{
			PERF_NODE("ProcessPlayerTalk");
			ProcessPlayerTalk(instream);
		}
		break;
	case Client_GS::GetChartInfoReq::MSG_ID:
		{
			PERF_NODE("ProcessGetChartInfo");
			ProcessGetChartInfo(instream);
		}
		break;
	//case Client_GS::InfoItemExchangeReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessInfoItemExchange");
	//		ProcessInfoItemExchange(instream);
	//	}
	//	break;
	//case Client_GS::ItemExchangeReq::MSG_ID:
	//	{
	//		PERF_NODE("ProcessInfoItemExchange");
	//		ProcessItemExchange(instream);
	//	}
	//	break;
	case Client_GS::GetItemDespReq::MSG_ID:
		{
			PERF_NODE("ProcessGetItemDesp");
			ProcessGetItemDesp(instream);
		}
		break;
	case Client_GS::HornChatReq::MSG_ID:
		{
			PERF_NODE("ProcessHornChat");
			ProcessHornChat(instream);
		}
		break;
	case Client_GS_Count_Down_Finish:
		{
			PERF_NODE("ProcessCountDownFinish");
			ProcessCountDownFinish();
		}
		break;
	case Client_GS_Friend_Change:
		{
			PERF_NODE("ProcessFriendChange");
			ProcessFriendChange(instream);
		}
		break;
	case Client_GS_Friend_Fresh:
		{
			PERF_NODE("ProcessFriendFresh");
			ProcessFriendFresh();
		}
		break;
	case Client_GS_Friend_Apply_Agree:
		{
			PERF_NODE("ProcessFriendApplyAgree");
			ProcessFriendApplyAgree(instream);
		}
		break;
	//case Client_GS_Equip_ReRandAdd:
	//	{
	//		PERF_NODE("ProcessEquipReRandAdd");
	//		ProcessEquipReRandAdd(instream);
	//	}
	//	break;
	//case Client_GS_Equip_Exchange_Upgrade:
	//	{
	//		PERF_NODE("ProcessEquipExchangeUpgrade");
	//		ProcessEquipExchangeUpgrade(instream);
	//	}
	//	break;
	case Client_GS::ServerScriptReq::MSG_ID:
		{
			PERF_NODE("ProcessServerScript");
			ProcessServerScript(instream);
		}
		break;
	case Client_GS::ProtectItemReq::MSG_ID:
		{
			PERF_NODE("Client_GS_Protect_Item");
			ProcessProtectItem(instream);
		}
		break;
	case Client_GS::GroupKickMemberReq::MSG_ID:
		{
			PERF_NODE("GroupKickMemberReq");
			ProcessKickMember(instream);
		}
		break;
	case Client_GS_Fresh_Gift:
		{
			PERF_NODE("ProcessFreshGift");
			ProcessFreshGift(instream);
		}
		break;
	//case Client_GS_MergeEquip:
	//	{
	//		PERF_NODE("ProcessMergeEquip");
	//		ProcessMergeEquip(instream);
	//	}
	//	break;
	//case Client_GS_Booth_Add:
	//	{
	//		PERF_NODE("ProcessBoothAdd");
	//		ProcessBoothAdd(instream);
	//	}
	//	break;
	//case Client_GS_OpenPK:
	//	{
	//		PERF_NODE("ProcessOpenPK");
	//		ProcessOpenPK(instream);
	//	}
	//	break;
	case Client_GS_Add_Bag_Slot:
		{
			PERF_NODE("ProcessAddBagSlot");
			ProcessAddBagSlot(instream);
		}
		break;
	case Client_GS::CollectStartReq::MSG_ID:
		{
			PERF_NODE("ProcessCollectStart");
			ProcessCollectStart(instream);
		}
		break;
	case Client_GS::TaskListReq::MSG_ID:
		{
			PERF_NODE("ProcessGetTaskList");
			ProcessGetTaskList(instream);
		}
		break;
	case Client_GS::DirectFlyReq::MSG_ID:
		{
			PERF_NODE("ProcessDirectFly");
			ProcessDirectFly(instream);
		}
		break;
	case Client_GS_Mount_Change_Req:
		{
			PERF_NODE("ProcessMountChange");
			ProcessMountChange(instream);
		}
		break;
	case Client_GS::SplitItemReq::MSG_ID:
		{
			PERF_NODE("ProcessSplitItem");
			ProcessSplitItem(instream);
		}
		break;
	case Client_GS::PushLuaTableReq::MSG_ID:
		{
			PERF_NODE("ProcessPushLuaTable");
			ProcessPushLuaTable(instream);
		}
		break;
	case Client_GS_Get_Mails_Req:
		{
			ProcessGetMails(instream);
		}
		break;
	case Client_GS_Open_Mail_Req:
		{
			ProcessOpenMail(instream);
		}
		break;
	case Client_GS_Receive_Mail_Items_Req:
		{
			ProcessReceiveMailItems(instream);
		}
		break;
	case Client_GS_Delete_Mail_Req:
		{
			ProcessDeleteMail(instream);
		}
		break;
	case Client_GS_Kuafu_PT_Info:///�ݲ������ȡ��Щ������ʲô��?
		{
			PERF_NODE("ProcessGetKuafuPTInfo");
			ProcessGetKuafuPTInfo(instream);
		}
		break;
	case Client_GS_Consign_Item_Req:
		{
			PERF_NODE("ProcessConsignBagItem");
			ProcessConsignBagItem(instream);
		}	
		break;
	case Client_GS_Get_ConsignableItems:
		{
			PERF_NODE("ProcessGetConsignableItems");
			ProcessGetConsignableItems(instream);
		}
		break;
	case Client_GS_Buy_Consignable_Item:
		{
			PERF_NODE("ProcessBuyConsignableItem");
			ProcessBuyConsignableItem(instream);
		}
		break;
	case Client_GS_Take_Back_Consignable_Item:
		{
			PERF_NODE("ProcessTakeBackConsignableItem");
			ProcessTakeBackConsignableItem(instream);			
		}
		break;
	case Client_GS_Take_Back_VCoin:
		{
			PERF_NODE("ProcessTakeBackConsignableItem");
			ProcessTakeBackVCoin(instream);			
		}
		break;
	case Client_GS_Find_Map_Ghost:
		{
			PERF_NODE("ProcessFindMapGhost");
			ProcessFindMapGhost(instream);			
		}
		break;
	}
}

size_t CCliNetSession::BreakMessage( LEUD::StreamFix& stream )
{
	if( stream.size() > Client_GS::HeadLen() )
	{
		char buf[20];
		stream.Out(buf,Client_GS::HeadLen());
		if( buf[0]=='t' && buf[1]=='g' && buf[2]=='w' && buf[3]=='_' )
		{
			if(stream.size()>256)
			{
				return stream.size();
			}
			size_t l=stream.findchar(0);
			if(l>0)
			{
				return l+Client_GS::HeadLen();
			}
			return 0;
		}
		if( stream )
		{
			unsigned short len = Client_GS::FindHead(buf);
			return len;
		}		
	}
	return 0;
}
void CCliNetSession::EncodeMessage( char* buf,size_t size )
{
	int * bb = (int*) buf;
	size_t s = size/4;
	size_t end = s/2;
	int t = 0;
	for( size_t i = 0;i < end;i ++ )
	{
		t = bb[i];
		bb[i] = bb[s-1-i]^0xA6E839CD;
		bb[s-1-i] = t^0xA6E839CD;
	}
	if( s % 2 == 1 )
	{
		bb[end] = bb[end]^0xA6E839CD;
	}
}
void CCliNetSession::DecodeMessage( char* buf,size_t size )
{
	int * bb = (int*) buf;
	size_t s = size/4;
	size_t end = s/2;
	int t = 0;
	for( size_t i = 0;i < end;i ++ )
	{
		t = bb[i];
		bb[i] = bb[s-1-i]^0xA6E839CD;
		bb[s-1-i] = t^0xA6E839CD;
	}
	if( s % 2 == 1 )
	{
		bb[end] = bb[end]^0xA6E839CD;
	}
}
void CCliNetSession::MakeHead( char* buf,unsigned short size )
{
	GS_Client::MakeHead(buf,size);
}










void CCliNetSession::ProcessClientGSKuafuAuth(LEUD::StreamFix& instream){
	Client_GS::ClientGSKuafuAuth req;instream>>req;
	if(instream){
		mAuthType=OGSLib::AUTH_TYPE_CKUAFU;
		mSessionID=req.ticket;
		mAuthSeed=0;
		mPlatformID=0;
		unsigned char md5[16];
		unsigned char src[1024];memset(src,0,1024);
		sprintf((char*)src,"%s_%s_%s_%s_%s_%d_%s_%s_%s",req.loginid.c_str(),req.charname.c_str(),req.ip.c_str(),req.port.c_str(),req.param.c_str(),req.ticket_seed,req.localip.c_str(),req.localport.c_str(),m_app->mKuaFuMD5Pad.c_str());
		LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
		char temp[256];sprintf(temp,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
		char tick[256];sprintf(tick,"%s_%d",temp,req.ticket_seed);
		if(0==strcmp(tick,req.ticket.c_str())){
			mAuthed=true;
			mLoginID=req.loginid;
			mKuafuParam=req.param;
			mKuafuSIP=req.localip;
			mKuafuSPort=req.localport;
			mKuafuTicket=req.ticket;
			mKuafuArea = req.localArea;
			char sn[512];sprintf(sn,"%s%s_%d",req.localip.c_str(),req.localport.c_str(),rand()%9000+1000);
			if(m_app->mKuaFuService && m_app->m_KuaFuService)
			{
				int result = m_app->m_KuaFuService->AddServer(sn,req.localip.c_str(),req.localport.c_str(),this);
				if(result)
				{
					GS_Client::GSClientKuafuAuthRes res;
					res.result = GS_Client::AuthenticateRES::result_succ;
					SendMsg(res);
				}
			}
			else
			{
				GS_Client::GSClientKuafuAuthRes res;
				res.result = GS_Client::AuthenticateRES::result_error_req_error;
				SendMsg(res);
			}
		}else{
			GS_Client::GSClientKuafuAuthRes res;
			res.result = GS_Client::AuthenticateRES::result_error_req_error;
			SendMsg(res);
		}
	}else{
		GS_Client::GSClientKuafuAuthRes res;
		res.result = GS_Client::AuthenticateRES::result_error_req_error;
		SendMsg(res);
	}
}
void CCliNetSession::ProcessAuthenticate(LEUD::StreamFix& instream)
{
	if( m_app->m_pSessionManager->GetSessionNum() > m_app->mConnMax )
	{
		GS_Client::SessionClosedNotify notify;
		strncpy_s(notify.msg ,_countof(notify.msg), m_app->m_pGameServerLib->mStringConsts.NOTIFY_SERVER_FULL,511);
		notify.msg[511]=0;
		Send_Message(notify);
		NetBreak();
		return;
	}
	Client_GS::AuthenticateREQ req;
	instream>>req;
	if( instream )
	{
		if( ( req.AuthType == 101 )	|| ( req.AuthType == 100 ) || ( req.AuthType == 102 ) || (req.AuthType == 103) )
		{
			mAuthType=req.AuthType;
			mSessionID=req.mSessionID;
			mAuthSeed=req.AuthSeed;
			mPlatformID=req.PartformID;
			mIdfa=req.mIdfa;
			if( ( req.AuthType == 100 ) || ( req.AuthType == 102 ) )
			{
				mAuthed = true;

				bool gen_new=true;

				if( mSessionID.length() > 32 )
				{
					mLoginID = mSessionID.substr(32);
					unsigned char md5[16];
					unsigned char src[256];
					memset(src,0,256);
					_snprintf_s((char*)src,_countof(src),255,"%s%s",mLoginID.c_str(),m_app->mYouKeMD5Pad.c_str());
					LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
					char temp[256];
					_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
					std::string sessionMD5 = mSessionID.substr(0,32);
					if( sessionMD5 == temp )
					{
						gen_new = false;
					}
				}
				if( gen_new )
				{
					char temp[256];
					_snprintf_s(temp,_countof(temp),255,"%s%d",m_app->mYouKePrefix.c_str(),m_app->mYouKeID++);temp[255]=0;
					mLoginID=temp;
					unsigned char md5[16];
					unsigned char src[256];
					memset(src,0,256);
					_snprintf_s((char*)src,_countof(src),255,"%s%s",mLoginID.c_str(),m_app->mYouKeMD5Pad.c_str());src[255]=0;
					LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
					_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
					temp[255]=0;
					mSessionID = temp;
					mSessionID += mLoginID;
					GS_Client::YouKeSessionIDNotify notify;
					strncpy_s(notify.SessionID,_countof(notify.SessionID),mSessionID.c_str(),511);notify.SessionID[511]=0;
					notify.SessionID[511]=0;
					SendMsg(notify);
					mNewYouKe = 1;
				}

				GS_Client::AuthenticateRES res;
				res.result = GS_Client::AuthenticateRES::result_succ;
				SendMsg(res);
			}
			else if( req.AuthType == 103 )//////�ӿ������������ʱ����?
			{
				mIsKuafuBack = true;
				mAuthType = 101;
				m_app->ClientAuth(m_Socket,mSessionID,req.AuthSeed,req.PartformID,req.mIdfa);
			}
			else
			{
				m_app->ClientAuth(m_Socket,mSessionID,req.AuthSeed,req.PartformID,req.mIdfa);
			}
		}
		else
		{
			if( req.AuthType == 110 )
			{
				mAuthType=req.AuthType;
				mSessionID=req.mSessionID;
				mAuthSeed=req.AuthSeed;
				mPlatformID=req.PartformID;
				mIdfa=req.mIdfa;
				if( mSessionID.length() > 32 )
				{
					mLoginID = mSessionID.substr(32); // ȡ�� MD5 ֵ���渽���ķ���������
					unsigned char md5[16];
					unsigned char src[256];
					memset(src,0,256);
					_snprintf_s((char*)src,_countof(src),255,"%s%s",mLoginID.c_str(),m_app->mKuaFuMD5Pad.c_str());
					LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
					char temp[256];
					_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
					std::string sessionMD5 = mSessionID.substr(0,32);
					if( sessionMD5 == temp )
					{
						mAuthed = true;
#ifndef LIMIT_RELEASE_2
						//m_app->m_KuaFuManager->AddSock(m_app->mKuaFuGuid--,*this);
						GS_Client::AuthenticateRES res;
						res.result = GS_Client::AuthenticateRES::result_succ;
						SendMsg(res);
#endif
					}
					else
					{
						GS_Client::AuthenticateRES res;
						res.result = GS_Client::AuthenticateRES::result_error_req_error;
						SendMsg(res);
					}
				}
				else
				{
					GS_Client::AuthenticateRES res;
					res.result = GS_Client::AuthenticateRES::result_error_req_error;
					SendMsg(res);
				}
			}
			else
			{
				GS_Client::AuthenticateRES res;
				res.result = GS_Client::AuthenticateRES::result_error_req_auth_type_error;
				SendMsg(res);
			}
		}
	}
	else
	{
		GS_Client::AuthenticateRES res;
		res.result = GS_Client::AuthenticateRES::result_error_req_error;
		SendMsg(res);
	}
}

void CCliNetSession::ProcessUpdateTicket(LEUD::StreamFix& instream)
{
	//bool send = true;
	//if( mAuthType == 101 && 1 == m_app->UpdateTicket(m_Socket,mSessionID,0) )
	//{
	//	send = false;
	//}
	//if( send )
	//{
	//	GS_Client::UpdateTicketRes res;
	//	SendMsg(res);
	//}
}
void CCliNetSession::ProcessListCharacter(LEUD::StreamFix& instream)
{
	Client_GS::ListCharacterReq req;
	instream>>req;
	if( ! instream ) return;
	//m_app->m_pDBManager->ClientListCharacter(m_Socket,mLoginID);
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_app->m_pDBManager->push_list_character(m_Socket,mLoginID);
#endif
//#endif
}
void CCliNetSession::ProcessCreateCharacter(LEUD::StreamFix& instream)
{
	Client_GS::CreateCharacterReq req;
	instream>>req;
	if( ! instream ) return;
	char youke[128];
	if( strlen(req.youkesession) > 32 )
	{
		//youke = req.youkesession.substr(32);
		//memcpy(youke,req.youkesession,32);
		//youke[32]=0;
		strncpy_s(youke,_countof(youke),req.youkesession,127);
		youke[127] = 0;
		unsigned char md5[16];
		unsigned char src[256];
		memset(src,0,256);
		_snprintf_s((char*)src,_countof(src),255,"%s%s",youke,m_app->mYouKeMD5Pad.c_str());
		src[255]=0;
		LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
		char temp[256];
		_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
		temp[255]=0;
		//std::string sessionMD5 = req.youkesession.substr(0,32);
		char sessionMD5[33];
		memcpy(sessionMD5,req.youkesession,32);
		sessionMD5[32]=0;
		if( 0 != strcmp(sessionMD5,temp) )
		{
			////youke = "";
			youke[0] = 0;
		}
	}
	//if( req.name.length() > 60 )
	//{
	//	req.name = req.name.substr(0,60);
	//}
	req.name[60] = 0;
	char nn[512];strncpy_s(nn,_countof(nn),req.name,511);nn[511]=0;
	m_app->m_pGameServerLib->GetFilterManager()->FilterName(req.name);
	OGSLib::UtilString::strrep(req.name,">",'_');
	OGSLib::UtilString::strrep(req.name,"<",'_');
	OGSLib::UtilString::strrep(req.name," ",'_');
	//m_app->m_pDBManager->ClientCreateCharacter(m_Socket,mLoginID,req.name,req.job,req.gender,youke);
	if( strcmp(nn,req.name)== 0 )
	{
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
		m_app->m_pDBManager->push_create_character(m_Socket,mLoginID,req.name,mIdfa,mPlatformID,req.job,req.gender,req.svrid,youke);
#endif
//#endif
	}
	else
	{
		GS_Client::CreateCharacterRes res;
		res.result = GS_Client::CreateCharacterRes::result_type_name_filter;
		SendMsg(res);
	}
}
void CCliNetSession::ProcessDeleteCharacter(LEUD::StreamFix& instream)
{
	Client_GS::DeleteCharacterReq req;
	instream>>req;
	if( ! instream ) return ;
	//m_app->m_pDBManager->ClientDeleteCharacter(m_Socket,mLoginID,req.charname);
//#ifdef NDEBUG
#ifndef LIMIT_RELEASE_2
	m_app->m_pDBManager->push_delete_character(m_Socket,mLoginID,req.charname);
#endif
//#endif
}
void CCliNetSession::ProcessEnterGame(LEUD::StreamFix& instream)
{
	Client_GS::EnterGameReq req;
	instream>>req;
	if( ! instream ) return;
    if ((mAuthType == 101) || (mAuthType == 100) || (mAuthType == 102) || (mAuthType == 120))
	{
		std::string mSeedID;
		mCharID=req.charname;
		mSeedID=req.seedname;
		if(0 == mSeedID.compare(""))
		{
			int now = static_cast<int>(time(0));
			int num = rand()%9000 + 1000;
			std::stringstream seedname;
			seedname<<mLoginID<<now<<num;
			mSeedID = seedname.str();
		}
		if( ( mAuthType == 100 ) || ( mAuthType == 102 ) )
		{
			mCharID=mLoginID;
		}
		char ip[128];GetPeerIP(m_Socket,ip);
		m_app->m_pGameServerLib->CreateSession(mLoginID.c_str(),mCharID.c_str(),mSeedID.c_str(),mAuthType,mNewYouKe,ip,this);
	}
	else
	{
		GS_Client::EnterGameRes res;
		res.result = GS_Client::EnterGameRes::result_type_fail_system;
		SendMsg(res);
	}
}
void CCliNetSession::ProcessFreshVcoin(LEUD::StreamFix& instream)
{
	mGameSession->GetPlayer()->ClientFreshVcoin();
}
void CCliNetSession::ProcessGroupChat(LEUD::StreamFix& instream)
{
	//if( mGameSession->GetPlayer()->GetMap()->NoInteract() )
	//{
	//	mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_NO_INFO_CHAT);
	//	return;
	//}
	if( mChatTick + mGameSession->GetGameServerLib()->mGroupChatGap > time(0) )
	{
		mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHAT_TOO_QUICK);
		return;
	}
	if( mGameSession->GetPlayer()->level() < mGameSession->GetGameServerLib()->mGroupChatLevelLimit )
	{
		char temp[512];
		_snprintf_s(temp,_countof(temp),511,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_GROUP_CHAT_LEVEL_LIMIT,mGameSession->GetGameServerLib()->mGroupChatLevelLimit);
		temp[511]=0;
		mGameSession->GetPlayer()->AlertMessage(10,0,temp);
		return;
	}
	mChatTick = time(0);
	Client_GS::GroupChatReq req;
	instream>>req;
	if( !instream ) return;
	mGameSession->GetGameServerLib()->GetFilterManager()->Filter(req.msg);
	//if( req.msg.length() > 512 )
	//{
	//	req.msg = req.msg.substr(0,512);
	//}
	{
		GS_Client::GroupChatRes res;
		strncpy_s(res.msg,_countof(res.msg),req.msg,511);
		res.msg[511]=0;
		SendMsg(res);
	}
	if( mGameSession->GetPlayer()->TalkProhibited() )
	{
		//mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_TALK_PROHIBITED);
		return;
	}
	mGameSession->GetDomainGroup()->GroupChat(req.msg);

	if(mGameSession->GetGameServerLib()->mListenChat)
	{
		if(mGameSession->GetPlayer())
		{
			std::string tomsg = "[group] " + std::string(req.msg);
			std::map<std::string,OGSLib::GameSession*> chatsessions=mGameSession->GetGameServerLib()->GetChatListenSessionMap();
			for(std::map<std::string,OGSLib::GameSession*>::iterator pos = chatsessions.begin();
				pos != chatsessions.end();
				++ pos )
			{
				OGSLib::GameSession* chatsession = pos->second;
				if( chatsession )
				{
					chatsession->GetListener()->notifyWorldChat(chatsession,mGameSession->GetPlayer(),tomsg.c_str());
				}
			}
		}
	}
}
void CCliNetSession::ProcessNormalChat(LEUD::StreamFix& instream)
{
	//if( mGameSession->GetPlayer()->GetMap()->NoInteract() )
	//{
	//	mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_NO_INFO_CHAT);
	//	return;
	//}
	if( mChatTick + mGameSession->GetGameServerLib()->mNormalChatGap > time(0) )
	{
		mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHAT_TOO_QUICK);
		return;
	}
	if( mGameSession->GetPlayer()->level() < mGameSession->GetGameServerLib()->mNormalChatLevelLimit )
	{
		char temp[512];
		_snprintf_s(temp,_countof(temp),511,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_NORMAL_CHAT_LEVEL_LIMIT,mGameSession->GetGameServerLib()->mNormalChatLevelLimit);
		temp[511]=0;
		mGameSession->GetPlayer()->AlertMessage(10,0,temp);
		return;
	}
	mChatTick = time(0);
	Client_GS::NormalChatReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetGameServerLib()->GetFilterManager()->Filter(req.msg);
	//if( req.msg.length() > 512 )
	//{
	//	req.msg = req.msg.substr(0,512);
	//}
	{
		GS_Client::NormalChatRes res;
		strncpy_s(res.msg,_countof(res.msg),req.msg,511);
		res.msg[511]=0;
		SendMsg(res);
	}
	if( mGameSession->GetPlayer()->TalkProhibited() )
	{
		//mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_TALK_PROHIBITED);
		return;
	}
	mGameSession->GetPlayer()->NormalChat(req.msg);

	if(mGameSession->GetGameServerLib()->mListenChat)
	{
		if(mGameSession->GetPlayer())
		{
			std::string tomsg = "[normal] " + std::string(req.msg);
			std::map<std::string,OGSLib::GameSession*> chatsessions=mGameSession->GetGameServerLib()->GetChatListenSessionMap();
			for(std::map<std::string,OGSLib::GameSession*>::iterator pos = chatsessions.begin();
				pos != chatsessions.end();
				++ pos )
			{
				OGSLib::GameSession* chatsession = pos->second;
				if( chatsession )
				{
					chatsession->GetListener()->notifyWorldChat(chatsession,mGameSession->GetPlayer(),tomsg.c_str());
				}
			}
		}
	}
}
void CCliNetSession::ProcessMapChat(LEUD::StreamFix& instream)
{
	//if( mGameSession->GetPlayer()->GetMap()->NoInteract() )
	//{
	//	mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_NO_INFO_CHAT);
	//	return;
	//}
	if( mChatTick + mGameSession->GetGameServerLib()->mMapChatGap > time(0) )
	{
		mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHAT_TOO_QUICK);
		return;
	}
	if( mGameSession->GetPlayer()->level() < mGameSession->GetGameServerLib()->mMapChatLevelLimit )
	{
		char temp[512];
		_snprintf_s(temp,_countof(temp),511,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_CHAT_LEVEL_LIMIT,mGameSession->GetGameServerLib()->mMapChatLevelLimit);
		temp[511]=0;
		mGameSession->GetPlayer()->AlertMessage(10,0,temp);
		return;
	}
	mChatTick = time(0);
	Client_GS::MapChatReq req;
	instream>>req;
	if( ! instream ) return;
	if( req.msg[0] != '@' )
	{
		mGameSession->GetGameServerLib()->GetFilterManager()->Filter(req.msg);
	}
	//if( req.msg.length() > 512 )
	//{
	//	req.msg = req.msg.substr(0,512);
	//}
	{
		GS_Client::MapChatRes res;
		strncpy_s(res.msg,_countof(res.msg),req.msg,511);
		res.msg[511]=0;
		SendMsg(res);
	}
	if( mGameSession->GetPlayer()->TalkProhibited() )
	{
		//mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_TALK_PROHIBITED);
		return;
	}
	mGameSession->GetPlayer()->MapChat(req.msg);

	if(mGameSession->GetGameServerLib()->mListenChat)
	{
		if(mGameSession->GetPlayer())
		{
			std::string tomsg = "[map] " + std::string(req.msg);
			std::map<std::string,OGSLib::GameSession*> chatsessions=mGameSession->GetGameServerLib()->GetChatListenSessionMap();
			for(std::map<std::string,OGSLib::GameSession*>::iterator pos = chatsessions.begin();
				pos != chatsessions.end();
				++ pos )
			{
				OGSLib::GameSession* chatsession = pos->second;
				if( chatsession )
				{
					chatsession->GetListener()->notifyWorldChat(chatsession,mGameSession->GetPlayer(),tomsg.c_str());
				}
			}
		}
	}
}
void CCliNetSession::ProcessHornChat(LEUD::StreamFix& instream)
{
	//if( mGameSession->GetPlayer()->GetMap()->NoInteract() )
	//{
	//	mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_NO_INFO_CHAT);
	//	return;
	//}
	if( mChatTick + mGameSession->GetGameServerLib()->mHornChatGap > time(0) )
	{
		mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHAT_TOO_QUICK);
		return;
	}
	if( mGameSession->GetPlayer()->level() < mGameSession->GetGameServerLib()->mHornChatLevelLimit )
	{
		char temp[512];
		_snprintf_s(temp,_countof(temp),511,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_HORN_CHAT_LEVEL_LIMIT,mGameSession->GetGameServerLib()->mHornChatLevelLimit);
		temp[511]=0;
		mGameSession->GetPlayer()->AlertMessage(11,0,temp);
		return;
	}
	mChatTick = time(0);
	Client_GS::HornChatReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetGameServerLib()->GetFilterManager()->Filter(req.msg);
	if( mGameSession->GetPlayer()->TalkProhibited() )
	{
		//mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_TALK_PROHIBITED);
		return;
	}
	mGameSession->GetPlayer()->HornChat(req.msg);

	if(mGameSession->GetGameServerLib()->mListenChat)
	{
		if(mGameSession->GetPlayer())
		{
			std::string tomsg = "[horn] " + std::string(req.msg);
			std::map<std::string,OGSLib::GameSession*> chatsessions=mGameSession->GetGameServerLib()->GetChatListenSessionMap();
			for(std::map<std::string,OGSLib::GameSession*>::iterator pos = chatsessions.begin();
				pos != chatsessions.end();
				++ pos )
			{
				OGSLib::GameSession* chatsession = pos->second;
				if( chatsession )
				{
					chatsession->GetListener()->notifyWorldChat(chatsession,mGameSession->GetPlayer(),tomsg.c_str());
				}
			}
		}
	}
}
void CCliNetSession::ProcessWorldChat(LEUD::StreamFix& instream)
{
	//if( mGameSession->GetPlayer()->GetMap()->NoInteract() )
	//{
	//	mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_NO_INFO_CHAT);
	//	return;
	//}
	if( mChatTick + mGameSession->GetGameServerLib()->mWorldChatGap > time(0) )
	{
		mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHAT_TOO_QUICK);
		return;
	}
	if( mGameSession->GetPlayer()->level() < mGameSession->GetGameServerLib()->mWorldChatLevelLimit )
	{
		char temp[512];
		_snprintf_s(temp,_countof(temp),511,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_WORLD_CHAT_LEVEL_LIMIT,mGameSession->GetGameServerLib()->mWorldChatLevelLimit);
		temp[511]=0;
		mGameSession->GetPlayer()->AlertMessage(11,0,temp);
		return;
	}
	mChatTick = time(0);
	Client_GS::WorldChatReq req;
	instream>>req;
	if( ! instream ) return;
	if( req.msg[0] != '@' )
	{
		mGameSession->GetGameServerLib()->GetFilterManager()->Filter(req.msg);
	}
	//if( req.msg.length() > 512 )
	//{
	//	req.msg = req.msg.substr(0,512);
	//}
	{
		GS_Client::WorldChatRes res;
		strncpy_s(res.msg,_countof(res.msg),req.msg,511);
		res.msg[511]=0;
		SendMsg(res);
	}
	if( mGameSession->GetPlayer()->TalkProhibited() )
	{
		//mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_TALK_PROHIBITED);
		return;
	}
	mGameSession->GetGameServerLib()->WorldChat(mGameSession->GetPlayer(),req.msg);
}
void CCliNetSession::ProcessPrivateChat(LEUD::StreamFix& instream)
{
	//if( mGameSession->GetPlayer()->GetMap()->NoInteract() )
	//{
	//	mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_NO_INFO_CHAT);
	//	return;
	//}
	if( mChatTick + mGameSession->GetGameServerLib()->mPrivateChatGap > time(0) )
	{
		mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHAT_TOO_QUICK);
		return;
	}
	if( mGameSession->GetPlayer()->level() < mGameSession->GetGameServerLib()->mPrivateChatLevelLimit )
	{
		char temp[512];
		_snprintf_s(temp,_countof(temp),511,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_PRIVATE_CHAT_LEVEL_LIMIT,mGameSession->GetGameServerLib()->mPrivateChatLevelLimit);
		temp[511]=0;
		mGameSession->GetPlayer()->AlertMessage(10,0,temp);
		return;
	}
	mChatTick = time(0);
	Client_GS::PrivateChatReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetGameServerLib()->GetFilterManager()->Filter(req.msg);
	OGSLib::GameSession* session = this->m_app->m_pGameServerLib->FindSessionWithCharID(req.charID);
	if( session )
	{
		OGSLib::DomainPlay* player = session->GetPlayer();
		if( player )
		{
			//if( req.msg.length() > 512 )
			//{
			//	req.msg = req.msg.substr(0,512);
			//}
			{
				GS_Client::PrivateChatRes res;
				strncpy_s(res.tname,_countof(res.tname),req.charID,511);
				res.tname[511]=0;
				strncpy_s(res.msg,_countof(res.msg),req.msg,511);
				res.msg[511]=0;
				SendMsg(res);
			}
			if( mGameSession->GetPlayer()->TalkProhibited() )
			{
				//mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_TALK_PROHIBITED);
				return;
			}
			mGameSession->GetPlayer()->PrivateChat(player,req.msg);

			if(mGameSession->GetGameServerLib()->mListenChat)
			{
				std::string tomsg = "[private to "+std::string(player->Name())+"] "+std::string(req.msg);
				std::map<std::string,OGSLib::GameSession*> chatsessions=mGameSession->GetGameServerLib()->GetChatListenSessionMap();
				for(std::map<std::string,OGSLib::GameSession*>::iterator pos = chatsessions.begin();
					pos != chatsessions.end();
					++ pos )
				{
					OGSLib::GameSession* chatsession = pos->second;
					if( chatsession )
					{
						chatsession->GetListener()->notifyWorldChat(chatsession,mGameSession->GetPlayer(),tomsg.c_str());
					}
				}
			}
		}
	}
	else
	{
		OGSLib::SubFriend sf;
		sf.mOnlineState = 2;
		sf.mName = req.charID;
		sf.mTitle = mGameSession->GetPlayer()->GetRelationShip(req.charID);
		mGameSession->GetListener()->notifyFriendChange(mGameSession, sf);
		mGameSession->GetPlayer()->AlertMessage(1,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_PLAYER_NOT_ONLINE_CHAT);
	}
}
void CCliNetSession::ProcessGuildChat(LEUD::StreamFix& instream)
{
	//if( mGameSession->GetPlayer()->GetMap()->NoInteract() )
	//{
	//	mGameSession->GetPlayer()->AlertMessage(100,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAP_NO_INFO_CHAT);
	//	return;
	//}
	if( mChatTick + mGameSession->GetGameServerLib()->mGuildChatGap > time(0) )
	{
		mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHAT_TOO_QUICK);
		return;
	}
	if( mGameSession->GetPlayer()->level() < mGameSession->GetGameServerLib()->mGuildChatLevelLimit )
	{
		char temp[512];
		_snprintf_s(temp,_countof(temp),511,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_GUILD_CHAT_LEVEL_LIMIT,mGameSession->GetGameServerLib()->mGuildChatLevelLimit);
		temp[511]=0;
		mGameSession->GetPlayer()->AlertMessage(10,0,temp);
		return;
	}
	mChatTick = time(0);
	Client_GS::GuildChatReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetGameServerLib()->GetFilterManager()->Filter(req.msg);
	//if( req.msg.length() > 512 )
	//{
	//	req.msg = req.msg.substr(0,512);
	//}
	{
		GS_Client::GuildChatRes res;
		strncpy_s(res.msg,_countof(res.msg),req.msg,511);
		res.msg[511]=0;
		SendMsg(res);
	}
	if( mGameSession->GetPlayer()->TalkProhibited() )
	{
		//mGameSession->GetPlayer()->AlertMessage(10,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_TALK_PROHIBITED);
		return;
	}
	if (mGameSession->GetDomainGuild())
	{
		mGameSession->GetDomainGuild()->GuildChat(req.msg);
	}

	if(mGameSession->GetGameServerLib()->mListenChat)
	{
		if(mGameSession->GetPlayer())
		{
			std::string tomsg = "[guild] " + std::string(req.msg);
			std::map<std::string,OGSLib::GameSession*> chatsessions=mGameSession->GetGameServerLib()->GetChatListenSessionMap();
			for(std::map<std::string,OGSLib::GameSession*>::iterator pos = chatsessions.begin();
				pos != chatsessions.end();
				++ pos )
			{
				OGSLib::GameSession* chatsession = pos->second;
				if( chatsession )
				{
					chatsession->GetListener()->notifyWorldChat(chatsession,mGameSession->GetPlayer(),tomsg.c_str());
				}
			}
		}
	}
}
bool CCliNetSession::check_as2lua_tag()
{
	if( mGameSession->GetGameServerLib()->mCheckAs2LuaCount )
	{
		time_t t = time(0);
		if( mAs2LuaSecontTick != t )
		{
			mAs2LuaSecontTick = t;
			mAs2LuaCountPerSecond = 1;
		}
		else
		{
			mAs2LuaCountPerSecond ++;
			if( mAs2LuaCountPerSecond <= mGameSession->GetGameServerLib()->mAs2LuaPerSecondMax )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return true;
}
bool CCliNetSession::check_runwalk_tag()
{
	if( mGameSession->GetGameServerLib()->mCheckRunWalkCount )
	{
		time_t t = time(0);
		if( mRunWalkCountTick != t )
		{
			mRunWalkCountTick = t;
			mRunWalkCountPerSecond = 1;
		}
		else
		{
			mRunWalkCountPerSecond ++;
			if( mRunWalkCountPerSecond <= mGameSession->GetGameServerLib()->mRunWalkPerSecondMax )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return true;
}
int CCliNetSession::check_client_tag(int tag)
{
	if( mMoveSendTag == 0 )
	{
		mMoveSendTag = tag-1;
	}
	if( tag != (mMoveSendTag +1) )
	{
		mMoveSendTag = tag;
		return 1;
	}
	mMoveSendTag = tag;
	return 0;
}
int CCliNetSession::check_client_time(DWORD timetag)
{
	if( mGameSession->GetGameServerLib()->mCheckWalkRunClientTime )
	{
		DWORD t = timeGetTime();
		if( mClientTimeTag == 0 || mServerTimeTag + 1000*mServerTimeResetDuration < t) 
		{ 
			mClientTimeTag = timetag;
			mServerTimeTag = t;
			mServerTimeResetDuration = mGameSession->GetGameServerLib()->mCheckWalkRunClientTimeResetGap;
			mClientTimeTagFrame = mGameSession->GetGameServerLib()->UpdateTick();
		}
		else {
			if( timetag < mClientTimeTag )
			{
				notifyAlert(mGameSession,11,0,"negative server tag");
				return 2;
			}
			if( t < mServerTimeTag )
			{
				notifyAlert(mGameSession,11,0,"negative server tag");
				return 2;
			}
			if( mClientTimeTagFrame != mGameSession->GetGameServerLib()->UpdateTick() )
			{
				if( timetag - mClientTimeTag > (t - mServerTimeTag + mGameSession->GetGameServerLib()->mTimeCheckPad ) )
				{
					mClientTimeErrorCount ++;if( mClientTimeErrorCount < 3 ) return 0;
					if( mGameSession->GetGameServerLib()->mTimeCheckAlert )
					{
						char temp[255];_snprintf_s(temp,255,"time error %d %d",timetag - mClientTimeTag,t - mServerTimeTag + mGameSession->GetGameServerLib()->mTimeCheckPad );
						notifyAlert(mGameSession,11,0,temp);
					}
					if( mServerTimeResetDuration > 10 ){ mServerTimeResetDuration -= 10; }
					return 1;
				}
				if( (t - mServerTimeTag) > (mGameSession->GetGameServerLib()->mTimeCheckPad) && (timetag - mClientTimeTag) < (t - mServerTimeTag - mGameSession->GetGameServerLib()->mTimeCheckPad ) )
				{
					mClientTimeErrorCount ++;if( mClientTimeErrorCount < 3 ) return 0;
					if( mGameSession->GetGameServerLib()->mTimeCheckAlert )
					{
						char temp[255];_snprintf_s(temp,255,"time error %d %d",(timetag - mClientTimeTag),(t - mServerTimeTag - mGameSession->GetGameServerLib()->mTimeCheckPad ) );
						notifyAlert(mGameSession,11,0,temp);
					}
					if( mServerTimeResetDuration > 10 ){ mServerTimeResetDuration -= 10; }
					return 1;
				}
				mClientTimeErrorCount = 0;
			}
		}
	}
	return 0;
}
void CCliNetSession::ProcessWalk(LEUD::StreamFix& instream)
{
	Client_GS::WalkReq req;
	instream>>req;
	if( ! instream ) return;
	if( !check_runwalk_tag() )return;
	int ret = 0;
	if( 0 != (ret = check_client_tag(req.tag) ) ){resWalk(mGameSession,mGameSession->GetPlayer()->Dir(),mGameSession->GetPlayer()->X(),mGameSession->GetPlayer()->Y(),req.tag);return;}
	//if( mPingStopAction + mGameSession->GetGameServerLib()->mPingStopTime >= time(0) ){resWalk(mGameSession,mGameSession->GetPlayer()->Dir(),mGameSession->GetPlayer()->X(),mGameSession->GetPlayer()->Y(),req.tag);return;}
	if( 0 != (ret = check_client_time(req.timetag) ) ){if( mGameSession ){resWalk(mGameSession,mGameSession->GetPlayer()->Dir(),mGameSession->GetPlayer()->X(),mGameSession->GetPlayer()->Y(),req.tag);}return;}
	mGameSession->GetPlayer()->Walk(req.dir,req.newx,req.newy,req.tag);
}
void CCliNetSession::ProcessRun(LEUD::StreamFix& instream)
{
	//Client_GS::RunReq req;
	//instream>>req;
	//if( ! instream ) return;
	//if( !check_runwalk_tag() )return;
	//int ret = 0;
	//if( 0 != (ret = check_client_tag(req.tag) ) ){resRun(mGameSession,mGameSession->GetPlayer()->Dir(),mGameSession->GetPlayer()->X(),mGameSession->GetPlayer()->Y(),req.tag);return;}
	////if( mPingStopAction + mGameSession->GetGameServerLib()->mPingStopTime >= time(0) ){resRun(mGameSession,mGameSession->GetPlayer()->Dir(),mGameSession->GetPlayer()->X(),mGameSession->GetPlayer()->Y(),req.tag);return;}
	//if( 0 != (ret = check_client_time(req.timetag) ) ){if( mGameSession ){resRun(mGameSession,mGameSession->GetPlayer()->Dir(),mGameSession->GetPlayer()->X(),mGameSession->GetPlayer()->Y(),req.tag);}return;}
	//mGameSession->GetPlayer()->Run(req.dir,req.newx,req.newy,req.tag);
}
void CCliNetSession::ProcessUseSkill(LEUD::StreamFix& instream)
{
	Client_GS::UseSkillReq req;
	instream>>req;
	if( !instream ) return;
	int ret = 0;
	if( mUseSkillSendTag == 0 ){mUseSkillSendTag = req.sendtag-1;}
	if( req.sendtag != (mUseSkillSendTag +1) ){resSkill(mGameSession,1,req.skill_type);mUseSkillSendTag=req.sendtag;return;}
	mUseSkillSendTag=req.sendtag;
	//if( mPingStopAction + mGameSession->GetGameServerLib()->mPingStopTime >= time(0) ){return;}
	if( 0 != (ret = check_client_time(req.timetag) ) ){if( mGameSession ){resSkill(mGameSession,1,req.skill_type);}return;}
	mGameSession->GetPlayer()->UseSkill(req.skill_type,req.paramX,req.paramY,req.paramID);
}
void CCliNetSession::ProcessNPCTalk(LEUD::StreamFix& instream)
{
	Client_GS::NPCTalkReq req;
	instream>>req;
	if( ! instream ) return ;
	mGameSession->GetPlayer()->NPCTalk(req.npcid,req.param);
}
void CCliNetSession::ProcessTurn(LEUD::StreamFix& instream)
{
	Client_GS::TurnReq req;
	instream>>req;
	if( ! instream ) return ;
	mGameSession->GetPlayer()->Turn(req.dir);
}
void CCliNetSession::ProcessAttack(LEUD::StreamFix& instream)
{
	//Client_GS::AttackReq req;
	//instream>>req;
	//if( ! instream ) return;
	//mGameSession->GetPlayer()->Attack(req.param);
}
void CCliNetSession::ProcessCountDownFinish()
{
	mGameSession->GetPlayer()->CountDownFinish();
}
void CCliNetSession::ProcessFriendChange(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::FriendChangeReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->FriendChange(req.name,req.title);
}

void CCliNetSession::ProcessFriendApplyAgree(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::FriendApplyAgree msg;
	instream>>msg;
	if (! instream) return;
	mGameSession->GetPlayer()->AgreeFriendApply(msg.name, msg.is_agree);
}

void CCliNetSession::ProcessFriendFresh()
{
	mGameSession->GetPlayer()->FriendFresh();
}
//void CCliNetSession::ProcessEquipReRandAdd(LEUD::StreamFix& instream)
//{
//	Client_GS::EquipReRandAddReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->EquipReRandAdd(req.posEquip,req.posEquipAdd);
//}
//void CCliNetSession::ProcessEquipExchangeUpgrade(LEUD::StreamFix& instream)
//{
//	Client_GS::EquipExchangeUpgradeReq req;
//	instream>>req;
//	if( ! instream ) return;
//	//if( SYSTEM_ENGINE_WUZUN == mGameSession->GetGameServerLib()->mSystemEngineType || mGameSession->GetGameServerLib()->mSysEquipExchangeUpgradeType == 1 )
//	//{
//		mGameSession->GetPlayer()->EquipExchangeUpgrade(req.posEquipFrom,req.posEquipTo,req.posEquipAdd,req.paytype);
//	//}
//	//if( SYSTEM_ENGINE_WENDING == mGameSession->GetGameServerLib()->mSystemEngineType || mGameSession->GetGameServerLib()->mSysEquipExchangeUpgradeType == 2 )
//	//{
//	//	mGameSession->GetPlayer()->EquipExchangeUpgradeFromTuLong(req.posEquipFrom,req.posEquipTo,req.paytype);
//	//}
//}
void CCliNetSession::ProcessServerScript(LEUD::StreamFix& instream)
{
	Client_GS::ServerScriptReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->ExeCodeClient(req.param);
}
void CCliNetSession::ProcessProtectItem(LEUD::StreamFix& instream)
{
	Client_GS::ProtectItemReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->ItemProtect(req.pos,req.type_id);
}
void CCliNetSession::ProcessKickMember(LEUD::StreamFix& instream)
{
	Client_GS::GroupKickMemberReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetDomainGroup()->RemGroupMember(req.member_name);
}
//void CCliNetSession::ProcessOpenAchieve(LEUD::StreamFix& instream)
//{
//	mGameSession->GetPlayer()->OpenAchieve();
//}
//void CCliNetSession::ProcessOpenPK(LEUD::StreamFix& instream)
//{
//	mGameSession->GetPlayer()->OpenPK(0);
//}
void CCliNetSession::ProcessAddBagSlot(LEUD::StreamFix& instream)
{
	mGameSession->GetPlayer()->AddBagSlot();
}
//void CCliNetSession::ProcessMergeEquip(LEUD::StreamFix& instream)
//{
//	if(m_app->mKuaFuService)return;
//	Client_GS::MergeEquipReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->ClientMergeEquip(req.target_id,req.param_id1,req.param_id2,req.param_id3,req.param_id4,req.param_id5,req.param_id6);
//}
//void CCliNetSession::ProcessBoothAdd(LEUD::StreamFix& instream)
//{
//	Client_GS::BoothAddReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->BoothAdd(req.pos,req.type_id,req.pay_type,req.pay_price);
//}
void CCliNetSession::ProcessFreshGift(LEUD::StreamFix& instream)
{
	mGameSession->GetPlayer()->FreshGift();
}

void CCliNetSession::ProcessPickUp(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::PickUpReq req;
	instream>>req;
	if( ! instream ) return ;
	mGameSession->GetPlayer()->PickUp(req.itemid,req.x,req.y);
}

void CCliNetSession::ProcessBagUseItem(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::BagUseItemReq req;
	instream>>req;
	if( ! instream )
	{
		if( mCheckItemUse == 0 )
		{
			mCheckItemUse = 1;
		}
		return;
	}
	if( mUseItemSendTag == 0 ){mUseItemSendTag = req.sendtag-1;}
	if( req.sendtag != (mUseItemSendTag +1) )
	{
		if( mCheckItemUse == 0 )
		{
			mUseItemSendTag=req.sendtag;
			mCheckItemUse = 1;
		}
		return;
	}
	mUseItemSendTag=req.sendtag;
	OGSLib::ItemDef* id = mGameSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(req.type_id);
	if (!id) return;
	if( OGSLib::ItemDef::IsDissipative(id->SubType) )
	{
		if( mItemUseTick + 0.25 > mGameSession->GetGameServerLib()->GetTimerFix()->Elapsed() )
		{
			return;
		}
		else
		{
			mItemUseTick = mGameSession->GetGameServerLib()->GetTimerFix()->Elapsed();
			time_t t = time(0);
			if( mUseItemCountSecontTick != t )
			{
				mUseItemCountSecontTick = t;
				mUseItemCountPerSecond = 1;
			}
			else
			{
				mUseItemCountPerSecond ++;
				//printf("CNS item count %d\n",mUseItemCountPerSecond);
				if( mUseItemCountPerSecond > mGameSession->GetGameServerLib()->mUseDisItemPerSecondMax )
				{
					return;
				}
			}
		}
	}
	mGameSession->GetPlayer()->BagUseItem(req.position,req.type_id,req.num);
}

void CCliNetSession::ProcessConsignBagItem(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::ConsignItemReq req;
	instream >> req;
	if(!instream) return;
	mGameSession->GetPlayer()->ConsignItem(req.pos, req.count, req.price, req.time_hour);
}

void CCliNetSession::ProcessGetConsignableItems(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::GetConsignableItems req;
	instream >> req;
	if(!instream) return;
	mGameSession->GetPlayer()->GetConsignableItems(req.type, req.begin_index, req.job, req.condition, req.filter);
}

void CCliNetSession::ProcessBuyConsignableItem(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::BuyConsignableItem req;
	instream >> req;
	if(!instream) return;
	mGameSession->GetPlayer()->BuyConsignableItem(req.seed_id);
}

void CCliNetSession::ProcessTakeBackConsignableItem(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::TakeBackConsignableItem req;
	instream >> req;
	if(!instream) return;
	mGameSession->GetPlayer()->TakeBackConsignableItem(req.seed_id);
}

void CCliNetSession::ProcessTakeBackVCoin(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	mGameSession->GetPlayer()->TakeBackVCoin();
}

void CCliNetSession::ProcessUndressItem(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::UndressItemReq req;
	instream>>req;
	if( ! instream ) return ;
	mGameSession->GetPlayer()->UndressItem(req.position);
}
void CCliNetSession::ProcessItemPositionExchange(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::ItemPostionExchangeReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->ItemPositionExchange(req.from,req.to,req.flag);
}
void CCliNetSession::ProcessNpcShop(LEUD::StreamFix& instream)
{
	Client_GS::NpcShopReq req;
	instream>>req;
	if( !instream ) return;
	mGameSession->GetPlayer()->NPCShop(req.npc_id,req.page);
}
void CCliNetSession::ProcessNpcBuy(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::NpcBuyReq req;
	instream>>req;
	if( !instream ) return;
	mGameSession->GetPlayer()->NPCBuy(req.npc_id,req.page,req.pos,req.good_id,req.type_id,req.number);
}
void CCliNetSession::ProcessCancelTask(LEUD::StreamFix& instream)
{
	Client_GS::TaskCancelReq req;
	instream>>req;
	if( !instream ) return;
	mGameSession->GetPlayer()->CancelTask(req.task_id);
}
void CCliNetSession::ProcessForceMove(LEUD::StreamFix& instream)
{
	Client_GS::ForceMoveReq req;
	instream>>req;
	if(!instream) return;
	mGameSession->GetPlayer()->ForceMove(req.newx,req.newy,req.newdir);
}

void CCliNetSession::ProcessChangeCloth(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::ChangeClothReq req;
	instream>>req;
	if(!instream)return;
	mGameSession->GetPlayer()->ChangeCloth(req.cloth,req.weapon,req.hair);
}

void CCliNetSession::ProcessDropItem(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::DropItemReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->DropItem(req.pos,req.type_id,req.number);
}
void CCliNetSession::ProcessNPCSell(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::NPCSellReq req;
	instream>>req;
	if( ! instream )return;
	mGameSession->GetPlayer()->NPCSell(req.npc_id,req.pos,req.type_id,req.number,req.flag);
}
void CCliNetSession::ProcessSaveShortcut(LEUD::StreamFix& instream)
{
	Client_GS::SaveShortcutReq req;
	instream>>req;
	if( ! instream )return;
	mGameSession->GetCharacter()->mShortcutMap.clear();
	for(size_t i = 0;i < req.shortcuts.size();i++)
	{
		OGSLib::DomainData::Shortcut sc;
		sc.shortcut_id = req.shortcuts[i].shortcut_id;
		sc.type = req.shortcuts[i].type;
		sc.param = req.shortcuts[i].param;
		mGameSession->GetCharacter()->mShortcutMap[sc.shortcut_id] = sc;
	}
}
void CCliNetSession::ProcessListGuild(LEUD::StreamFix& instream)
{
	Client_GS::ListGuildReq req;
	instream>>req;
	if( ! instream ) return;
	GS_Client::ListGuildBeginNotify bn;
	SendMsg(bn);
	const OGSLib::ManagerGuild::GuildMap& gm = mGameSession->GetGameServerLib()->GetGuildManager()->GetGuilds();
	for( OGSLib::ManagerGuild::GuildMap::const_iterator pos = gm.begin();
		pos != gm.end();
		++ pos )
	{
		if( pos->second->GetMemberNumber() > 0 && !pos->second->deleted())
		{
			GS_Client::ListGuildItemNotify notify;
			notify.name = pos->second->GetName();
			notify.seed_id = pos->second->SeedID();
			notify.member_number = static_cast<int>(pos->second->GetMemberNumber());
			notify.desp = pos->second->GetDesp();
			notify.master_level = pos->second->master_level();
			notify.leader_name = pos->second->GetGuildLeaderName();
			notify.guild_level = pos->second->guild_level();
			mGameSession->GetDomainGuild()->GetGuildWarState(notify.seed_id, notify.war_state, notify.war_start_time);
			if(pos->second->GetPremem(mGameSession->GetDomainGuild()->SeedName())){
				notify.entering=1;
			}
			SendMsg(notify);
		}
	}
	GS_Client::ListGuildEndNotify en;
	SendMsg(en);

	//GS_Client::ListGuildRes res;
	//const OGSLib::ManagerGuild::GuildMap& gm = mGameSession->GetGameServerLib()->GetGuildManager()->GetGuilds();
	//int i = 0;
	//for( OGSLib::ManagerGuild::GuildMap::const_iterator pos = gm.begin();
	//	i < 50 && pos != gm.end();
	//	++ pos )
	//{
	//	if( pos->second->GetMemberNumber() > 0 )
	//	{
	//		i++;
	//		GS_Client::ListGuildRes::guild_info gi;
	//		gi.name = pos->second->GetName();
	//		gi.member_number = static_cast<int>(pos->second->GetMemberNumber());
	//		gi.desp = pos->second->GetDesp();
	//		gi.master_level = pos->second->master_level();
	//		res.guildinfos.push_back(gi);
	//	}
	//}
	//SendMsg(res);
}
void CCliNetSession::ProcessGetGuildInfo(LEUD::StreamFix& instream)
{
	Client_GS::GetGuildInfoReq req;
	instream>>req;
	if( ! instream ) return;
	GS_Client::GetGuildInfoRes res;
	strncpy_s(res.name,_countof(res.name),mGameSession->GetDomainGuild()->GuildName().c_str(),511);
	res.name[511]=0;
	if( res.name[0] != 0 )
	{
		OGSLib::Guild* guild = mGameSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(res.name);
		if( guild )
		{
			res.member_number = static_cast<int>(guild->GetMemberNumber());
			strncpy_s(res.desp,_countof(res.desp),guild->GetDesp().c_str(),511);
			res.desp[511]=0;
			strncpy_s(res.notice,_countof(res.notice),guild->GetNotice().c_str(),511);
			res.notice[511]=0;
			strncpy_s(res.leader_name,_countof(res.leader_name),guild->GetGuildLeaderName().c_str(),511);
			res.leader_name[511]=0;
			res.guild_level = static_cast<int>(guild->guild_level());
			res.guild_exp = static_cast<int>(guild->guild_pt());
			strncpy_s(res.seed_id,_countof(res.seed_id),guild->SeedID().c_str(),511);
			res.seed_id[511]=0;
		}
	}
	SendMsg(res);
}
void CCliNetSession::ProcessCreateGuild(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	if( mAuthType != 101 )
	{
		mGameSession->GetListener()->notifyAlert(mGameSession,1,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_YOUKE_CAN_NOT_CREATE_GUILD);
		return;
	}
	Client_GS::CreateGuildReq req;
	instream>>req;
	if( ! instream ) return;
	char n[512];
	memcpy(n,req.guild_name,512);
	mGameSession->GetGameServerLib()->GetFilterManager()->FilterName(req.guild_name);
	if( 0!=strcmp(n,req.guild_name) )
	{
		notifyAlert(mGameSession,1,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_GUILD_CREATE_NAME_PROHIBITED);
		return;
	}
	mGameSession->GetDomainGuild()->CreateGuild(req.guild_name,req.flags);
}
void CCliNetSession::ProcessLeaveGuild(LEUD::StreamFix& instream)
{
	Client_GS::LeaveGuildReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetDomainGuild()->LeaveGuild(req.guild_name);
}
void CCliNetSession::ProcessAddDepotSlot(LEUD::StreamFix& instream)
{
	mGameSession->GetPlayer()->AddDepotSlot();
}
void CCliNetSession::ProcessGroupPickMode(LEUD::StreamFix& instream)
{
	mGameSession->GetDomainGroup()->PickMode();
}
void CCliNetSession::ProcessSwithSlaveAIMode(LEUD::StreamFix& instream)
{
	mGameSession->GetPlayer()->SwithSlaveAIMode();
}
void CCliNetSession::ProcessFreshHPMP(LEUD::StreamFix& instream)
{
	notifyHPMPChange(mGameSession,mGameSession->GetPlayer(),0,0);
}
//void CCliNetSession::ProcessUpdateChinaLimit(LEUD::StreamFix& instream)
//{
//	Client_GS::UpdateChinaLimitReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->UpdateChinaLimit(req.china_id,req.china_name,req.china_id_ok);
//}
//void CCliNetSession::ProcessBuyOfflineExp(LEUD::StreamFix& instream)
//{
//	Client_GS::BuyOfflineExpReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->BuyOfflineExp(req.mul);
//}
//void CCliNetSession::ProcessSteelEquip(LEUD::StreamFix& instream)
//{
//	Client_GS::SteelEquipReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->SteelEquip(req.pos,req.type_id);
//}
void CCliNetSession::ProcessPlayerTalk(LEUD::StreamFix& instream)
{
	Client_GS::PlayerTalkReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->PlayerTalk(req.seed,req.param);
}
void CCliNetSession::ProcessGetChartInfo(LEUD::StreamFix& instream)
{
	Client_GS::GetChartInfoReq req;
	instream>>req;
	if( ! instream ) return;
	int num=0;
	OGSLib::ChartNode* nodes =this->mGameSession->GetGameServerLib()->GetChartManager()->GetData(req.chart_type,req.page,num);
	if( nodes && (num > 0||req.page==0) )
	{
		GS_Client::GetChartInfoRes res;
		res.chart_type=req.chart_type;
		res.page = req.page;
		res.num = num;
		for(int i = 0;i < num/*&&i<10*/;i ++)
		{
			strncpy_s(res.nodes[i].name,_countof(res.nodes[i].name),nodes[i].name,255);
			nodes[i].name[255]=0;
			strncpy_s(res.nodes[i].guild,_countof(res.nodes[i].guild),nodes[i].guild,255);
			nodes[i].guild[255]=0;
			strncpy_s(res.nodes[i].title,_countof(res.nodes[i].title),nodes[i].title,255);
			nodes[i].title[255]=0;
			res.nodes[i].param=nodes[i].param;
			res.nodes[i].job=nodes[i].job;
			res.nodes[i].lv=nodes[i].lv;
			res.nodes[i].state=nodes[i].state;
			res.nodes[i].zslv = nodes[i].zslv;
			res.nodes[i].gender = nodes[i].gender;
		}
		SendMsg(res);
	}
}
//void CCliNetSession::ProcessInfoItemExchange(LEUD::StreamFix& instream)
//{
//	if(m_app->mKuaFuService)return;
//	Client_GS::InfoItemExchangeReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->InfoItemExchange(req.pos,req.type_id,req.exchange_type);
//}
//void CCliNetSession::ProcessItemExchange(LEUD::StreamFix& instream)
//{
//	if(m_app->mKuaFuService)return;
//	Client_GS::ItemExchangeReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetPlayer()->ItemExchange(req.pos,req.type_id,req.exchange_type);
//}
void CCliNetSession::ProcessGetItemDesp(LEUD::StreamFix& instream)
{
	Client_GS::GetItemDespReq req;
	instream>>req;
	if( ! instream ) return;
	if( req.type_id > 0 )
	{
		mGameSession->GetPlayer()->PushItemInfo(req.type_id);
	}
	else
	{
		mGameSession->GetPlayer()->PushItemInfo(req.type_name);
	}
}

void CCliNetSession::ProcessPing(LEUD::StreamFix& instream)
{
	mPing++;
	if( mPing == 1 )
	{
		mFirstPingUpdateTick = mGameSession->GetGameServerLib()->UpdateTick();
		mPingTick = time(0);
	}
	if( mFirstPingUpdateTick == mGameSession->GetGameServerLib()->UpdateTick() )
	{
		mPingTick = time(0);
		mPing = 1;
	}
	//printf("%d,%d\n",mPing,time(0));
	GS_Client::PingRes res;
	SendMsg(res);
	//time_t t = time(0);
	//if( mPingTick + mPing - mGameSession->GetGameServerLib()->mPingGapMini > t )
	//{
	//	if( mPing < 10 )
	//	{
	//		mPing = t - mPingTick;
	//	}
	//	else
	//	{
	//		mPingStopAction = t;
			//if( mPingTick + mPing - mGameSession->GetGameServerLib()->mPingGap > t )// ||  mPingTick + mPing + 100 < t )
			//{
			//	if( mPing < 100 )
			//	{
			//		mPing = t - mPingTick;
			//	}
			//	else
			//	{
			//		if( mPingUpdateTick != mGameSession->GetGameServerLib()->UpdateTick() )
			//		{
			//			mPingUpdateTick = mGameSession->GetGameServerLib()->UpdateTick();
			//			//mGameSession->BreakNetAndEndSession("time error");
			//		}
			//	}
			//}
	//	}
	//}
}

void CCliNetSession::ProcessJoinGuild(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	if( mAuthType != 101 )
	{
		mGameSession->GetListener()->notifyAlert(mGameSession,1,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_YOUKE_CAN_NOT_JOIN_GUILD);
		return;
	}
	Client_GS::JoinGuildReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetDomainGuild()->JoinGuild(req.guild_name,req.flags);
}
void CCliNetSession::ProcessSetGuildInfo(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::SetGuildInfoReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetDomainGuild()->SetGuildInfo(req.guild_name,req.desp,req.notice);
}
void CCliNetSession::ProcessListGuildMember(LEUD::StreamFix& instream)
{
	Client_GS::ListGuildMemberReq req;
	instream>>req;
	if( ! instream ) return;
	GS_Client::ListGuildMemberRes res;
	strncpy_s(res.guild_name,_countof(res.guild_name),mGameSession->GetDomainGuild()->GuildName().c_str(),511);
	res.guild_name[511]=0;
	res.list_type = req.list_type;
	if( res.guild_name[0] != 0 )
	{
		OGSLib::Guild* guild = mGameSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGameSession->GetDomainGuild()->GuildName());
		if( guild )
		{
			OGSLib::Guild::MemberMap* mm = &guild->GetMembers();
			if( req.list_type == 100 ){
				mm=&guild->GetPremems();
			}
			if( req.list_type == 101 )
			{
			}
			for(OGSLib::Guild::MemberMap::const_iterator pos = mm->begin(); pos != mm->end(); ++ pos)
			{
				if( res.guild_members.size() > 200 )
				{
					if(mGameSession->GetPlayer()->Name() != pos->second.nick_name)
					{
						continue;
					}
				}
				GS_Client::ListGuildMemberRes::guild_member_info gmi;
				gmi.name = pos->second.nick_name;
				gmi.title = pos->second.title;
				gmi.online = pos->second.online_state;
				gmi.job = pos->second.job;
				gmi.level = pos->second.lv;
				gmi.fight = pos->second.fight;
				gmi.guildpt = pos->second.guildpt;
				gmi.entertime = static_cast<int>(time(0)) - pos->second.entertime;
				if( pos->second.player )
				{
					gmi.gender = pos->second.player->gender();
					gmi.job = pos->second.player->job();
					gmi.level = pos->second.player->level();
				}
				res.guild_members.push_back(gmi);
			}
		}
	}
	SendMsg(res);
}
void CCliNetSession::ProcessListGuildEnemy(LEUD::StreamFix& instream)
{
	//Client_GS::ListGuildEnemyReq req;
	//instream>>req;
	//if( ! instream ) return;
	//GS_Client::ListGuildEnemyRes res;
	//res.guild_name = mGameSession->GetDomainGuild()->GuildName();
	//if( ! res.guild_name.empty() )
	//{
	//	OGSLib::Guild* guild = mGameSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(res.guild_name);
	//	if( guild )
	//	{
	//		const OGSLib::Guild::GuildList& gle = guild->GetEnemyGuilds();
	//		for(OGSLib::Guild::GuildList::const_iterator pos = gle.begin();
	//			pos != gle.end();
	//			++ pos)
	//		{
	//			res.guild_enemys.push_back(*pos);
	//		}
	//	}
	//}
	//SendMsg(res);
}
void CCliNetSession::ProcessListGuildFriend(LEUD::StreamFix& instream)
{
	//Client_GS::ListGuildFriendReq req;
	//instream>>req;
	//if( ! instream ) return;
	//GS_Client::ListGuildFriendRes res;
	//res.guild_name = mGameSession->GetDomainGuild()->GuildName();
	//if( ! res.guild_name.empty() )
	//{
	//	OGSLib::Guild* guild = mGameSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(res.guild_name);
	//	if( guild )
	//	{
	//		const OGSLib::Guild::GuildList& gle = guild->GetFriendGuilds();
	//		for(OGSLib::Guild::GuildList::const_iterator pos = gle.begin();
	//			pos != gle.end();
	//			++ pos)
	//		{
	//			res.guild_friends.push_back(*pos);
	//		}
	//	}
	//}
	//SendMsg(res);
}
void CCliNetSession::ProcessListGuildItem(LEUD::StreamFix& instream)
{
	Client_GS::ListGuildItemReq req;
	instream>>req;
	if( ! instream ) return;
	notifyListGuildItem(mGameSession);
}
void CCliNetSession::ProcessChangeGuildMemberTitle(LEUD::StreamFix& instream)
{
// 	Client_GS::ChangeGuildMemberTitleReq req;
// 	instream>>req;
// 	if( ! instream ) return;
// 	mGameSession->GetDomainGuild()->ChangeGuildMemberTitle(mGameSession->GetPlayer(),req.guild_name, req.nick_name, req.dir);
}

void CCliNetSession::ProcessChangeEnemyGuild(LEUD::StreamFix& instream)
{
	//Client_GS::ChangeEnemyGuildReq req;
	//instream>>req;
	//if( ! instream ) return;
	//mGameSession->GetDomainGuild()->ChangeEnemyGuild(req.guild_name,req.enemy_guild_name,req.action);
}
void CCliNetSession::ProcessChangeFriendGuild(LEUD::StreamFix& instream)
{
	//Client_GS::ChangeFriendGuildReq req;
	//instream>>req;
	//if( ! instream ) return;
	//mGameSession->GetDomainGuild()->ChangeFriendGuild(req.guild_name,req.friend_guild_name,req.action);
}

void CCliNetSession::ProcessRedPacketLogReq(LEUD::StreamFix& instream)
{
	mGameSession->GetDomainGuild()->onGetGuildRedPacketLog();
}

void CCliNetSession::ProcessGuildItemLogReq(LEUD::StreamFix& instream)
{
	mGameSession->GetDomainGuild()->onGetGuildItemLog();
}

void CCliNetSession::ProcessChangeAttackMode(LEUD::StreamFix& instream)
{
	Client_GS::ChangeAttackModeReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->ChangeAttackMode(req.attack_mode);
}
//void CCliNetSession::ProcessMarryInvite(LEUD::StreamFix& instream)
//{
//	Client_GS::MarryInviteReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetDomainCouple()->MarryInvite(req.target_name,req.male_desp);
//}
//void CCliNetSession::ProcessAgreeOrNotMarryInvite(LEUD::StreamFix& instream)
//{
//	Client_GS::AgreeOrNotMarryInviteReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetDomainCouple()->AgreeOrNotMarryInvite(req.agree,req.male_name,req.male_desp,req.female_name,req.female_desp);
//}
//void CCliNetSession::ProcessAgreeOrNotDivorceInvite(LEUD::StreamFix& instream)
//{
//	Client_GS::AgreeOrNotDivorceInviteReq req;
//	instream>>req;
//	if( ! instream ) return;
//	mGameSession->GetDomainCouple()->AgreeOrNotDivorceInvite(req.agree);
//}
void CCliNetSession::ProcessVcoinShopList(LEUD::StreamFix& instream)
{
	Client_GS::VcoinShopListReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->VcoinShopList(req.shop_id,req.flags);
}
void CCliNetSession::ProcessNPCRepair(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::NPCRepairReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->NPCRepairItem(req.npc_id,req.pos,req.type_id,req.flags);
}
void CCliNetSession::ProcessRelive(LEUD::StreamFix& instream)
{
	Client_GS::ReliveReq req;
	instream>>req;
	if(!instream) return;
	if( !(req.relive_type == 100 || req.relive_type == 101 ) )
	{
		req.relive_type = 100;
	}
	mGameSession->GetPlayer()->Relive(req.relive_type,0,0,0);
}
void CCliNetSession::ProcessTaskDesp(LEUD::StreamFix& instream)
{
	Client_GS::TaskDespReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->QueryTaskDesp(req.task_id);
}
void CCliNetSession::ProcessInfoPlayer(LEUD::StreamFix& instream)
{
	Client_GS::InfoPlayerReq req;
	instream>>req;
	if(!instream)return;
	if(mGameSession->GetGameServerLib()->FindSessionWithCharID(req.player_name))
	{
		mGameSession->GetPlayer()->InfoPlayer(req.player_name);
	}
	else
	{
		mGameSession->GetPlayer()->AlertMessage(1,0,mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_CHECK_EQUIP_NO_EXIST);
	}
	//else if(mGameSession->GetGameServerLib()->GetRobotManager()->FindRobot(req.player_name))
	//{
	//	//mGameSession->GetGameServerLib()->GetRobotManager()->FindRobot(req.player_name)
	//	mGameSession->GetPlayer()->InfoRobot(req.player_name);
	//}
}
void CCliNetSession::ProcessCreateGroup(LEUD::StreamFix& instream)
{
	Client_GS::CreateGroupReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetDomainGroup()->CreateGroup(req.flags);
}
void CCliNetSession::ProcessLeaveGroup(LEUD::StreamFix& instream)
{
	//Client_GS::LeaveGroupReq req;
	//instream>>req;
	//if( !instream ) return;
	mGameSession->GetDomainGroup()->LeaveGroup();
}
void CCliNetSession::ProcessJoinGroup(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::JoinGroupReq req;
	instream>>req;
	if(!instream) return;
	mGameSession->GetDomainGroup()->JoinGroup(req.group_id);
}
void CCliNetSession::ProcessAgreeJoinGroup(LEUD::StreamFix& instream)
{
	Client_GS::AgreeJoinGroupReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetDomainGroup()->AgreeJoinGroup(req.player_name);
}
void CCliNetSession::ProcessInviteGroup(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::InviteGroupReq req;
	instream>>req;
	if( ! instream) return;
	mGameSession->GetDomainGroup()->InviteGroup(req.player_name);
}
void CCliNetSession::ProcessAgreeInviteGroup(LEUD::StreamFix& instream)
{
	Client_GS::AgreeInviteGroupReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetDomainGroup()->AgreeInviteGroup(req.leader_name,req.group_id);
}
void CCliNetSession::ProcessTaskClick(LEUD::StreamFix& instream)
{
	Client_GS::TaskClickReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->TaskClick(req.task_id,req.param);
}
void CCliNetSession::ProcessTradeInvite(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	Client_GS::TradeInviteReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->TradeInvite(req.invitee);
}
void CCliNetSession::ProcessAgreeTradeInvite(LEUD::StreamFix& instream)
{
	Client_GS::AgreeTradeInviteReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->AgreeTradeInvite(req.inviter);
}
void CCliNetSession::ProcessCloseTrade(LEUD::StreamFix& instream)
{
	Client_GS::CloseTradeReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->CloseTrade(req.target);
}
void CCliNetSession::ProcessTradeAddGameMoney(LEUD::StreamFix& instream)
{
	Client_GS::TradeAddGameMoneyReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->TradeAddGameMoney(req.value);
}
void CCliNetSession::ProcessTradeAddVcoin(LEUD::StreamFix& instream)
{
	Client_GS::TradeAddVcoinReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->TradeAddVcoin(req.value);
}
void CCliNetSession::ProcessTradeSubmit(LEUD::StreamFix& instream)
{
	//Client_GS::TradeSubmitReq req;
	//instream>>req;
	//if( ! instream) return;
	mGameSession->GetPlayer()->TradeSubmit();
}
void CCliNetSession::ProcessGroupSetLeader(LEUD::StreamFix& instream)
{
	Client_GS::GroupSetLeaderReq req;
	instream>>req;
	if ( ! instream ) return;
	mGameSession->GetDomainGroup()->SetGroupLeader(req.player_name);
}
void CCliNetSession::ProcessTradeAddItem(LEUD::StreamFix& instream)
{
	Client_GS::TradeAddItemReq req;
	instream>>req;
	if( ! instream ) return;
	if(req.flag == 1)///��ʾ�����������Ľ���(����һ����Ʒ)
	{
		mGameSession->GetPlayer()->ChatTradeAddItem(req.pos,req.item_type,req.price);
		return;
	}
	else if(req.flag == 2)///��ʾ�����������Ľ���(�Ƴ�һ����Ʒ)
	{
		mGameSession->GetPlayer()->ChatTradeSubItem(req.pos,req.item_type);
		return;
	}
	else if(req.flag == 0)
	{
		mGameSession->GetPlayer()->TradeAddItem(req.pos,req.item_type);////һ��һ�Ľ���
		return;
	}
	
}
void CCliNetSession::ProcessChatTradeSubmit(LEUD::StreamFix& instream)
{
	Client_GS::ChatTradeSubmitReq req;
	instream>>req;
	if( ! instream ) return;
	if(req.flag == 1)////���콻������Ľ������
	{
		if(req.pos >= ITEM_CHATTRADE_BEGIN && req.pos < ITEM_CHATTRADE_END && req.item_type > 0)
		{
			mGameSession->GetPlayer()->ChatTradeSubmit(req.chrname,req.pos,req.item_type,req.lock);
		}
	}
	if(req.flag == 2)////�������鿴�ۻ�Ա����Ʒ��Ϣ
	{
		if(req.pos >= ITEM_CHATTRADE_BEGIN && req.pos < ITEM_CHATTRADE_END && req.item_type > 0)
		{
			mGameSession->GetPlayer()->ShowTradeItem(req.chrname,req.pos,req.item_type,req.lock);
		}
	}
}
void CCliNetSession::ProcessDestoryItem(LEUD::StreamFix& instream)
{
	Client_GS::DestoryItemReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->DestoryItem(req.pos,req.item_type);
}
void CCliNetSession::ProcessSortItem(LEUD::StreamFix& instream)
{
	Client_GS::SortItemReq req;
	instream>>req;
	if( !instream) return;
	mGameSession->GetPlayer()->SortItem(req.flags);
}
void CCliNetSession::ProcessItemTalk(LEUD::StreamFix& instream)
{
	Client_GS::ItemTalkReq req;
	instream>>req;
	if( ! instream ) return;
	mGameSession->GetPlayer()->ItemTalk(req.item_id,req.seed,req.param);
}
//void CCliNetSession::ProcessMergeSteel(LEUD::StreamFix& instream)
//{
//	if(m_app->mKuaFuService)return;
//	Client_GS::MergeSteelReq req;
//	instream>>req;
//	if( ! instream) return;
//	mGameSession->GetPlayer()->MergeSteel(req.item_pos1,req.item_pos2,req.item_pos3,req.item_posadd,req.pay_type);
//}
//void CCliNetSession::ProcessUpgradeEquip(LEUD::StreamFix& instream)
//{
//	if(m_app->mKuaFuService)return;
//	Client_GS::UpgradeEquipReq req;
//	instream>>req;
//	if(! instream) return;
//	mGameSession->GetPlayer()->UpgradeEquip(req.item_posEquip,req.item_posSteel,req.item_posAdd,req.pay_type);
//}
void CCliNetSession::ProcessCollectStart(LEUD::StreamFix& instream)
{
	Client_GS::CollectStartReq req;
	instream>>req;
	if( ! instream) return;
	mGameSession->GetPlayer()->StartCollect(req.srcid);
}
void CCliNetSession::ProcessGetTaskList(LEUD::StreamFix& instream)
{
	Client_GS::TaskListReq req;
	instream>>req;
	//resDailyTaskList();
	//resEverydayTaskList();
	//resInstanceTaskList();

}
void CCliNetSession::ProcessDirectFly(LEUD::StreamFix& instream)
{
	Client_GS::DirectFlyReq req;
	instream>>req;
	if( ! instream ) return;
	//mGameSession->GetPlayer()->DirectFly(req.target_id);
}
void CCliNetSession::ProcessMountChange(LEUD::StreamFix& instream)
{
	if(m_app->mKuaFuService)return;
	mGameSession->GetPlayer()->ChangeMount();
}
void CCliNetSession::ProcessSplitItem(LEUD::StreamFix& instream)
{
	Client_GS::SplitItemReq req;
	instream>>req;
	if( ! instream) return;
	mGameSession->GetPlayer()->SplitItem(req.id,req.pos,req.number);
}
void CCliNetSession::ProcessPushLuaTable(LEUD::StreamFix& instream)
{
	Client_GS::PushLuaTableReq req;
	instream>>req;
	if( ! instream) return;
	if(check_as2lua_tag())
	{
		mGameSession->GetPlayer()->PushLuaTable(req.type.c_str(),req.table.c_str());
	}
}
void CCliNetSession::ProcessGetMails(LEUD::StreamFix& instream)
{
	if( mGameSession && mGameSession->GetPlayer() ){
		notifyMails(mGameSession,mGameSession->GetPlayer());
	}
}
void CCliNetSession::ProcessOpenMail(LEUD::StreamFix& instream)
{
	Client_GS::OpenMailReq req;
	instream>>req;
	if(!instream) return;
	INT64 id = 0;
	sscanf(req.mailID.c_str(),"%I64d",&id);
	mGameSession->GetPlayer()->ReadMail(id);
}
void CCliNetSession::ProcessReceiveMailItems(LEUD::StreamFix& instream)
{
	Client_GS::ReceiveMailItemsReq req;
	instream>>req;
	if(!instream) return;
	INT64 id = 0;
	sscanf(req.mailID.c_str(),"%I64d",&id);
	mGameSession->GetPlayer()->RecieveMailItem(id);
}
void CCliNetSession::ProcessDeleteMail(LEUD::StreamFix& instream)
{
	Client_GS::DeleteMailReq req;
	instream>>req;
	if(!instream) return;
	for (std::vector<std::string>::iterator pos = req.mailIDs.begin();
		pos != req.mailIDs.end();++pos)
	{
		//printf("de.. %d\n",(*pos));
		INT64 id = 0;
		sscanf((*pos).c_str(),"%I64d",&id);
		mGameSession->GetPlayer()->DeleteMail(id);
	}
	notifyMails(mGameSession,mGameSession->GetPlayer());
}
void CCliNetSession::ProcessGetKuafuPTInfo(LEUD::StreamFix& instream)
{
	GS_Client::KuafuPTInfoNotify notify;
	notify.kuafuPTURL=m_app->mKuaFuPTURL;
	notify.localPTID=m_app->mLocalServerID;
	notify.localServerID=m_app->mLocalPTID;
	unsigned char md5[16];
	unsigned char src[1024];memset(src,0,256);
	sprintf((char*)src,"%s_%s_%s_%s",notify.kuafuPTURL.c_str(),notify.localServerID.c_str(),notify.localPTID.c_str(),m_app->mKuaFuMD5Pad.c_str());
	LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
	char temp[256];
	_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
	notify.ticket=temp;
	SendMsg(notify);
}
void CCliNetSession::ProcessFindMapGhost(LEUD::StreamFix& instream)
{
	Client_GS::FindMapGhostReq req;
	instream>>req;
	if(!instream) return;

	GS_Client::FindMapGhostRes res;

	if( mSearchTick + 1 > time(0) )
	{
		SendMsg(res);
		return;
	}

	unsigned short num = req.num;
	OGSLib::PlayMap* map = mGameSession->GetPlayer()->GetMap();
	if(map)
	{
		if( map->FubenType()!= OGSLib::MAP_TYPE::MAP_PERSONAL )
		{
			return;
		}

		bool monster_name = strlen(req.mon_name)>0;

		const OGSLib::PlayMap::MapObjectDeque* objectdeq = map->GetMapObjectDeque();
		if(objectdeq && objectdeq->size()>1)//���Լ�����
		{
			strncpy_s(res.map_id ,_countof(res.map_id), map->GetID().c_str(),511);
			res.map_id[511]=0;
			strncpy_s(res.mon_name ,_countof(res.mon_name), req.mon_name,511);
			res.mon_name[511]=0;

			for( OGSLib::PlayMap::MapObjectDeque::const_iterator pos = objectdeq->begin();
				pos != objectdeq->end();
				pos ++ )
			{
				if( (*pos)&&(*pos)->TypeIsMonster() && (*pos)->IsState(OGSLib::PlayMapObject::MAP_OBJECT_STATE_IDLE) )
				{
					if( monster_name )
					{
						if( 0 == strcmp((*pos)->Name(),req.mon_name) )
						{
							GS_Client::FindMapGhostRes::GhostNode gn;
							gn.id = (*pos)->ID();
							gn.x = (*pos)->X();
							gn.y = (*pos)->Y();
							gn.type = 0;
							res.mGhostList.push_back(gn);
						}
					}
					else
					{
						GS_Client::FindMapGhostRes::GhostNode gn;
						gn.id = (*pos)->ID();
						gn.x = (*pos)->X();
						gn.y = (*pos)->Y();
						gn.type = 0;
						res.mGhostList.push_back(gn);
					}

					if( res.mGhostList.size() >= num )
					{
						break;
					}
				}
			}
		}
	}

	mSearchTick = time(0);
	SendMsg(res);
}

void CCliNetSession::LoadKuafuCharacter(OGSLib::GameSession* session,const char* loginid,const char* charid,const char* seedname){
	if( session != mGameSession ) return;
	if(mAuthed){
		if(m_app->mKuaFuService && m_app->m_KuaFuService)
		{
			KuaFuServerData* kfsd=m_app->m_KuaFuService->FindServer(mKuafuSIP.c_str(),mKuafuSPort.c_str());
			if(kfsd&&kfsd->mSession){
				kfsd->mSession->LoadChar(mKuafuTicket.c_str(),loginid,charid,seedname);
			}else{
				notifySessionDataLoaded(0,3);
			}
		}
	}
}


void CCliNetSession::notifyAuthResult(int result){
	GS_Client::GSClientKuafuAuthRes res;
	res.result = result;
	SendMsg(res);
}
void CCliNetSession::notifyKuafuSetGsParam(OGSLib::GameSession* session,const char* k,const char* v){
	if( session != mGameSession ) return;
	if(!m_app->mKuaFuService)return;
	if(mAuthed){
		if(m_app->mKuaFuService && m_app->m_KuaFuService)
		{
			KuaFuServerData* kfsd=m_app->m_KuaFuService->FindServer(mKuafuSIP.c_str(),mKuafuSPort.c_str());
			if(kfsd&&kfsd->mSession){
				kfsd->mSession->SetGsParam(k,v);
			}
		}
	}
}
void CCliNetSession::notifyKuafuComeBackData(OGSLib::GameSession* session,const std::string& winArea,const std::string& winGuild,const char* loginid,const char* charid,const char* seekName,int vcoin,int result){
	if( session != mGameSession ) return;
	if(mAuthed){
		if(m_app->mKuaFuService && m_app->m_KuaFuService)
		{
			KuaFuServerData* kfsd=m_app->m_KuaFuService->FindServer(mKuafuSIP.c_str(),mKuafuSPort.c_str());
			if(kfsd&&kfsd->mSession){
				kfsd->mSession->ComeBackData(winArea,winGuild,loginid,charid,seekName,vcoin,result);
			}
		}
	}
}
//void CCliNetSession::notifySystemError(OGSLib::GameSession* session,int error)
//{
//}
void CCliNetSession::notifySessionCreateResult(OGSLib::GameSession* session,int result)
{
	mGameSession = session;
}
void CCliNetSession::notifySessionDataLoaded(OGSLib::GameSession* session,int result)
{
	bool ok = false;
	if( session && session == mGameSession )
	{
		mGamePlayer = mGameSession->GetPlayer();
		mGameSession->GetCharacter()->mKuafuParam=mKuafuParam;
		GS_Client::EnterGameRes res;
		res.result = GS_Client::EnterGameRes::result_type_succes;
		SendMsg(res);
		mGameSession->EnterGame();
		ok = true;
	}
	if( ! ok )
	{
		GS_Client::EnterGameRes res;
		switch(result)
		{
		case 1:
			res.result = GS_Client::EnterGameRes::result_type_fail_auth_type;
			break;
		case 2:
			res.result = GS_Client::EnterGameRes::result_type_fail_double_enter;
			break;
		case 3:
			res.result = GS_Client::EnterGameRes::result_type_fail_load_error;
			break;
		case 4:
			res.result = GS_Client::EnterGameRes::result_type_fail_deny;
			break;
		}
		SendMsg(res);
	}
}
void CCliNetSession::notifyNameLoad(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::CharacterLoadedNotify notify;
	OGSLib::DomainPlay* player = session->GetPlayer();
	notify.id = player->ID();
	strncpy_s(notify.charName,_countof(notify.charName),player->Name(),511);
	notify.charName[511] = 0;
	notify.loverName[0] = 0;
	//if (player->CoupleID() > 0 &&  player->LoverName())
	//{
	//	strncpy_s(notify.loverName,_countof(notify.loverName),player->LoverName(),511);
	//	notify.loverName[511] = 0;
	//}
	notify.cloth = player->Cloth();
	notify.weapon = player->Weapon();
	notify.hair = player->Hair();
	if(player->show_mount() == 1)
	{
		notify.mount = player->mount();
	}
	notify.x = player->X();
	notify.y = player->Y();
	notify.dir = player->Dir();
	notify.hp = player->Hp();
	notify.maxhp = player->MaxHp();
	notify.mp = player->Mp();
	notify.maxmp = player->MaxMp();
	notify.job = player->job();
	notify.gender = player->gender();
	notify.exp = player->exp();
	notify.level = player->level();
	OGSLib::ObjectAttribute* attr = player->Attributes();
	notify.burden = attr->mBurden;
	strncpy_s(notify.seedName,_countof(notify.seedName),player->SeedName(),511);
	notify.seedName[511] = 0;
	notify.fbresid = player->FaBaoRes();
	notify.fashion = player->Fashion();
	notify.createtime = player->CreateTime();
	if (player->GetModel(10) == 1)
	{
		notify.fashion = 0;
	}

	notify.teamid = player->TeamID();
	SendMsg(notify);
}
void CCliNetSession::notifyCharacterLoad(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	{
		notifyNameLoad(session);
	}
	OGSLib::DomainPlay* player = session->GetPlayer();
	{
		notifyProsperityChange(session,mGameSession->GetGameServerLib()->Getprosperity(),mGameSession->GetGameServerLib()->GetprosperityNext());
	}
	{
		GS_Client::MiniMapConnNotify notify;
		OGSLib::ManagerFly::DirectFlyNodeMap& nodemap = mGameSession->GetGameServerLib()->GetFlyManager()->GetmDirectFlyNodes();
		OGSLib::ManagerFly::DirectFlyNodeMap::iterator mappos = nodemap.begin();
		for(;mappos!=nodemap.end();mappos++)
		{
			OGSLib::DirectFlyNode& node = mappos->second;

			if(node.mType == 1 )
			{
				GS_Client::MiniMapConnNotify::conn _conn;
				strncpy_s(_conn.from_id,_countof(_conn.from_id),node.mStartMapID.c_str(),511);
				_conn.from_id[511]=0;
				strncpy_s(_conn.to_id ,_countof(_conn.to_id), node.mTargetMapID.c_str(),511);
				_conn.to_id[511]=0;
				notify.conns.push_back(_conn);
			}
		}
		SendMsg(notify);
	}
	{
		for( int i = 0;i < OGSLib::PlayActiveObject::STATUS_TYPE_NUMBER;i++)
		{
			if( player->IsStatus(i) )
			{
				GS_Client::StatusChangeNotify notify;
				notify.src_id = player->ID();
				notify.status_id = i;
				notify.status_time = player->GetStatusDuration(i);
				notify.status_param = player->StatusParam(i);
				notify.gap = static_cast<int>(player->StatusTickGap(i));
				SendMsg(notify);
			}
		}
	}
	{
		GS_Client::GameParamNotify notify;
		//notify.steel_equip_cost_base = mGameSession->GetGameServerLib()->mSteelEquipCostBase;
		//notify.steel_equip_cost_mul = mGameSession->GetGameServerLib()->mSteelEquipCostMul;
		notify.max_magic_anti = mGameSession->GetGameServerLib()->mMaxMagicAnti;
		notify.walk_speed_warrior_client_param = 0;
		notify.stand_relive_price = mGameSession->GetGameServerLib()->GetItemDefineManager()->mRelivePrice;
		notify.chart_open_limit_level = mGameSession->GetGameServerLib()->mChartOpenLimitLevel;
		notify.add_depot_price = mGameSession->GetGameServerLib()->GetItemDefineManager()->mAddDepotPrice;
		//notify.exchange_upd_prob_base = mGameSession->GetGameServerLib()->mExchangeUpgradeProbBase;
		//notify.exchange_upd_prob_gap = mGameSession->GetGameServerLib()->mExchangeUpgradeProbGap;
		//notify.exchange_upd_drop_max = mGameSession->GetGameServerLib()->mExchangeUpgradeDropLevelMax;
		//notify.exchange_upd_cost_gm = mGameSession->GetGameServerLib()->mExchangeUpgradeCostGM;
		//notify.exchange_upd_cost_bv = mGameSession->GetGameServerLib()->mExchangeUpgradeCostBV;
		//notify.mStatusQiseshendanAC = mGameSession->GetGameServerLib()->mStatusQiseshendanAC;
		//notify.mStatusQiseshendanACMax = mGameSession->GetGameServerLib()->mStatusQiseshendanACMax;
		//notify.mStatusQiseshendanMAC = mGameSession->GetGameServerLib()->mStatusQiseshendanMAC;
		//notify.mStatusQiseshendanMACMax = mGameSession->GetGameServerLib()->mStatusQiseshendanMACMax;
		//notify.mStatusQiseshendanDC = mGameSession->GetGameServerLib()->mStatusQiseshendanDC;
		//notify.mStatusQiseshendanDCMax = mGameSession->GetGameServerLib()->mStatusQiseshendanDCMax;
		//notify.mStatusQiseshendanMC = mGameSession->GetGameServerLib()->mStatusQiseshendanMC;
		//notify.mStatusQiseshendanMCMax = mGameSession->GetGameServerLib()->mStatusQiseshendanMCMax;
		//notify.mStatusQiseshendanSC = mGameSession->GetGameServerLib()->mStatusQiseshendanSC;
		//notify.mStatusQiseshendanSCMax = mGameSession->GetGameServerLib()->mStatusQiseshendanSCMax;
		//notify.mStatusQiseshendanHpmaxBase = mGameSession->GetGameServerLib()->mStatusQiseshendanHpmaxBase;
		//notify.mStatusQiseshendanHpmaxGap = mGameSession->GetGameServerLib()->mStatusQiseshendanHpmaxGap;
		//notify.mStatusQiseshendanMpmaxBase = mGameSession->GetGameServerLib()->mStatusQiseshendanMpmaxBase;
		//notify.mStatusQiseshendanMpmaxGap = mGameSession->GetGameServerLib()->mStatusQiseshendanMpmaxGap;

		//notify.mStatusXinFaHPMax = mGameSession->GetGameServerLib()->mStatusXinFaHPMax;
		//notify.mStatusXinFaXishou = mGameSession->GetGameServerLib()->mStatusXinFaXishou;
		//notify.mStatusXinFaAC = mGameSession->GetGameServerLib()->mStatusXinFaAC;
		//notify.mStatusXinFaACMax = mGameSession->GetGameServerLib()->mStatusXinFaACMax;
		//notify.mStatusXinFaMAC = mGameSession->GetGameServerLib()->mStatusXinFaMAC;
		//notify.mStatusXinFaMACMax = mGameSession->GetGameServerLib()->mStatusXinFaMACMax;
		//notify.mStatusXinFaDC = mGameSession->GetGameServerLib()->mStatusXinFaDC;
		//notify.mStatusXinFaDCMax = mGameSession->GetGameServerLib()->mStatusXinFaDCMax;
		//notify.mStatusXinFaMC = mGameSession->GetGameServerLib()->mStatusXinFaMC;
		//notify.mStatusXinFaMCMax = mGameSession->GetGameServerLib()->mStatusXinFaMCMax;
		//notify.mStatusXinFaSC = mGameSession->GetGameServerLib()->mStatusXinFaSC;
		//notify.mStatusXinFaSCMax = mGameSession->GetGameServerLib()->mStatusXinFaSCMax;

		//notify.mStatusYuanshenhutiAC = mGameSession->GetGameServerLib()->mStatusYuanshenhutiAC;
		//notify.mStatusYuanshenhutiACMax = mGameSession->GetGameServerLib()->mStatusYuanshenhutiACMax;
		//notify.mStatusYuanshenhutiMAC = mGameSession->GetGameServerLib()->mStatusYuanshenhutiMAC;
		//notify.mStatusYuanshenhutiMACMax = mGameSession->GetGameServerLib()->mStatusYuanshenhutiMACMax;
		//notify.mStatusYuanshenhutiDC = mGameSession->GetGameServerLib()->mStatusYuanshenhutiDC;
		//notify.mStatusYuanshenhutiDCMax = mGameSession->GetGameServerLib()->mStatusYuanshenhutiDCMax;
		//notify.mStatusYuanshenhutiMC = mGameSession->GetGameServerLib()->mStatusYuanshenhutiMC;
		//notify.mStatusYuanshenhutiMCMax = mGameSession->GetGameServerLib()->mStatusYuanshenhutiMCMax;
		//notify.mStatusYuanshenhutiSC = mGameSession->GetGameServerLib()->mStatusYuanshenhutiSC;
		//notify.mStatusYuanshenhutiSCMax = mGameSession->GetGameServerLib()->mStatusYuanshenhutiSCMax;

		//notify.mStatusTianshenhutiMAXHP = mGameSession->GetGameServerLib()->mStatusTianshenhutiMAXHP;
		//notify.mStatusTianshenhutiDC = mGameSession->GetGameServerLib()->mStatusTianshenhutiDC;
		//notify.mStatusTianshenhutiDCMax = mGameSession->GetGameServerLib()->mStatusTianshenhutiDCMax;
		//notify.mStatusTianshenhutiMC = mGameSession->GetGameServerLib()->mStatusTianshenhutiMC;
		//notify.mStatusTianshenhutiMCMax = mGameSession->GetGameServerLib()->mStatusTianshenhutiMCMax;
		//notify.mStatusTianshenhutiSC = mGameSession->GetGameServerLib()->mStatusTianshenhutiSC;
		//notify.mStatusTianshenhutiSCMax = mGameSession->GetGameServerLib()->mStatusTianshenhutiSCMax;
		//notify.mStatusTianshenhutiSubDamageProb = mGameSession->GetGameServerLib()->mStatusTianshenhutiSubDamageProb;
		//notify.mStatusTianshenhutiSubDamagePres = mGameSession->GetGameServerLib()->mStatusTianshenhutiSubDamagePres;

		//notify.mStatusBaqihutiAC = mGameSession->GetGameServerLib()->mStatusBaqihutiAC;
		//notify.mStatusBaqihutiACMax = mGameSession->GetGameServerLib()->mStatusBaqihutiACMax;
		//notify.mStatusBaqihutiMAC = mGameSession->GetGameServerLib()->mStatusBaqihutiMAC;
		//notify.mStatusBaqihutiMACMax = mGameSession->GetGameServerLib()->mStatusBaqihutiMACMax;
		//notify.mStatusBaqihutiDC = mGameSession->GetGameServerLib()->mStatusBaqihutiDC;
		//notify.mStatusBaqihutiDCMax = mGameSession->GetGameServerLib()->mStatusBaqihutiDCMax;
		//notify.mStatusBaqihutiMC = mGameSession->GetGameServerLib()->mStatusBaqihutiMC;
		//notify.mStatusBaqihutiMCMax = mGameSession->GetGameServerLib()->mStatusBaqihutiMCMax;
		//notify.mStatusBaqihutiSC = mGameSession->GetGameServerLib()->mStatusBaqihutiSC;
		//notify.mStatusBaqihutiSCMax = mGameSession->GetGameServerLib()->mStatusBaqihutiSCMax;

		//notify.mDeleteExchangeUpdFromEquip = mGameSession->GetGameServerLib()->mDeleteExchangeUpdFromEquip;
		notify.mDieDropBagProb = mGameSession->GetGameServerLib()->mDieDropBagProb;
		notify.mDieDropLoadProb = mGameSession->GetGameServerLib()->mDieDropLoadProb;

		notify.mProtectItemPrice = mGameSession->GetGameServerLib()->mItemProtectPrice;
		notify.mProtectItemProbMax = mGameSession->GetGameServerLib()->mItemProtectProbMax;
		notify.mProtectItemProb = mGameSession->GetGameServerLib()->mItemProtectProb;
		notify.mProtectItemAdd = mGameSession->GetGameServerLib()->mItemProtectAdd;

		//notify.mStatusFuQiTongXinAC = mGameSession->GetGameServerLib()->mStatusFuQiTongXinAC;
		//notify.mStatusFuQiTongXinMAC = mGameSession->GetGameServerLib()->mStatusFuQiTongXinMAC;
		//notify.mStatusFuQiTongXinACMax = mGameSession->GetGameServerLib()->mStatusFuQiTongXinACMax;
		//notify.mStatusFuQiTongXinMACMax = mGameSession->GetGameServerLib()->mStatusFuQiTongXinMACMax;

		//notify.mStatusZhuanShenMaxHp = mGameSession->GetGameServerLib()->mStatusZhuanShenMaxHp;
		//notify.mStatusZhuanShenMaxMp = mGameSession->GetGameServerLib()->mStatusZhuanShenMaxMp;
		//notify.mStatusZhuanShenXishou = mGameSession->GetGameServerLib()->mStatusZhuanShenXishou;
		//notify.mStatusZhuanShenDC = mGameSession->GetGameServerLib()->mStatusZhuanShenDC;
		//notify.mStatusZhuanShenDCMax = mGameSession->GetGameServerLib()->mStatusZhuanShenDCMax;
		//notify.mStatusZhuanShenMC = mGameSession->GetGameServerLib()->mStatusZhuanShenMC;
		//notify.mStatusZhuanShenMCMax = mGameSession->GetGameServerLib()->mStatusZhuanShenMCMax;
		//notify.mStatusZhuanShenSC = mGameSession->GetGameServerLib()->mStatusZhuanShenSC;
		//notify.mStatusZhuanShenSCMax = mGameSession->GetGameServerLib()->mStatusZhuanShenSCMax;

		//notify.mStatusVipDC = mGameSession->GetGameServerLib()->mStatusVipDC;
		//notify.mStatusVipDCMax = mGameSession->GetGameServerLib()->mStatusVipDCMax;
		//notify.mStatusVipMC = mGameSession->GetGameServerLib()->mStatusVipMC;
		//notify.mStatusVipMCMax = mGameSession->GetGameServerLib()->mStatusVipMCMax;
		//notify.mStatusVipSC = mGameSession->GetGameServerLib()->mStatusVipSC;
		//notify.mStatusVipSCMax = mGameSession->GetGameServerLib()->mStatusVipSCMax;
		//notify.mStatusVipAC = mGameSession->GetGameServerLib()->mStatusVipAC;
		//notify.mStatusVipACMax = mGameSession->GetGameServerLib()->mStatusVipACMax;
		//notify.mStatusVipMAC = mGameSession->GetGameServerLib()->mStatusVipMAC;
		//notify.mStatusVipMACMax = mGameSession->GetGameServerLib()->mStatusVipMACMax;

		notify.mPKConfirm = mGameSession->GetGameServerLib()->mPKConfirm;
		notify.mGuildMaxMember = mGameSession->GetGameServerLib()->mGuildMemberMax;

		//notify.mReinResetAttrVcoin = mGameSession->GetGameServerLib()->mReinResetAttrVcoin;
		//notify.mReinBuyTimesVcoin = mGameSession->GetGameServerLib()->mReinBuyTimesVcoin;
		//notify.mReinDCMaxPoint = mGameSession->GetGameServerLib()->mReinDCMaxPoint;
		//notify.mReinMCMaxPoint = mGameSession->GetGameServerLib()->mReinMCMaxPoint;
		//notify.mReinSCMaxPoint = mGameSession->GetGameServerLib()->mReinSCMaxPoint;
		//notify.mReinACMaxPoint = mGameSession->GetGameServerLib()->mReinACMaxPoint;
		//notify.mReinMACMaxPoint = mGameSession->GetGameServerLib()->mReinMACMaxPoint;
		//notify.mReinHPMaxPoint = mGameSession->GetGameServerLib()->mReinHPMaxPoint;
		//notify.mReinMPMaxPoint = mGameSession->GetGameServerLib()->mReinMPMaxPoint;
		//notify.mReinAccuaryPoint = mGameSession->GetGameServerLib()->mReinAccuaryPoint;
		//notify.mReinDodgePoint = mGameSession->GetGameServerLib()->mReinDodgePoint;
		//notify.mReinFreeTimesADay = mGameSession->GetGameServerLib()->mReinFreeTimesADay;

		notify.mTotalAttrLevelLimit = mGameSession->GetGameServerLib()->mTotalAttrLevelLimit;
		//notify.mShowLoginForm = mGameSession->GetGameServerLib()->mShowLoginForm;
		SendMsg(notify);
	}
	{
		std::string kg;
		mGameSession->GetGameServerLib()->GetKingGuild(kg);
		notifyWarInfo(mGameSession,mGameSession->GetGameServerLib()->GetWarState(),kg);
	}
	{
		GS_Client::GuildConditionNotify notify;
		mGameSession->GetPlayer()->conditionCreateGuild(notify.condition);
		SendMsg(notify);
	}
	{
		notifySlotAdd(session);
	}
	{
		notifyNameAdd(session,player,player->GetNamePre(),player->GetNamePro());
	}
	//{
	//	notifyFreeDirectFly(session);
	//}
	//{
	//	GS_Client::FreeReliveNotify notify;
	//	notify.level = session->GetGameServerLib()->mFreeReliveLimitLevel;
	//	SendMsg(notify);
	//}
	{
		//GS_Client::URLNotify notify;
		//strncpy_s(notify.reg ,_countof(notify.reg), session->GetGameServerLib()->mRegURL.c_str(),511);
		//notify.reg[511]=0;
		//strncpy_s(notify.login ,_countof(notify.login), session->GetGameServerLib()->mLoginURL.c_str(),511);
		//notify.login[511]=0;
		//char temp[512];
		//_snprintf_s(temp,_countof(temp),511,session->GetGameServerLib()->mPayURL.c_str(),session->LoginID());
		//temp[511]=0;
		//strncpy_s(notify.pay ,_countof(notify.pay), temp,511);
		//notify.pay[511]=0;
		//char tempWH[512];
		//_snprintf_s(tempWH,_countof(tempWH),511,session->GetGameServerLib()->mWebHomeURL.c_str(),session->LoginID());
		//tempWH[511]=0;
		//strncpy_s(notify.webhome ,_countof(notify.webhome), tempWH,511);
		//notify.webhome[511]=0;
		//char tempBBS[512];
		//_snprintf_s(tempBBS,_countof(tempBBS),511,session->GetGameServerLib()->mBBSURL.c_str(),session->LoginID());
		//tempBBS[511]=0;
		//strncpy_s(notify.bbs ,_countof(notify.bbs), tempBBS,511);
		//notify.bbs[511]=0;
		//char tempDL[512];
		//_snprintf_s(tempDL,_countof(tempDL),511,session->GetGameServerLib()->mDownloadURL.c_str(),session->LoginID());
		//tempDL[511]=0;
		//strncpy_s(notify.download ,_countof(notify.download),tempDL,511);
		//notify.download[511]=0;
		//char tempKF[512];
		//_snprintf_s(tempKF,_countof(tempKF),511,session->GetGameServerLib()->mKefuURL.c_str(),session->LoginID());
		//tempKF[511]=0;
		//strncpy_s(notify.kefu ,_countof(notify.kefu),tempKF,511);
		//notify.kefu[511]=0;
		////SendMsg(notify);

		//strncpy_s(notify.paramurl1,_countof(notify.paramurl1), session->GetGameServerLib()->mClientParamURL1.c_str(),511);
		//notify.paramurl1[511]=0;
		//_snprintf_s(temp,_countof(temp),511,session->GetGameServerLib()->mClientParamURL2.c_str(),session->LoginID(),session->SeedID());
		//temp[511]=0;
		//strncpy_s(notify.paramurl2 ,_countof(notify.paramurl2), temp,511);
		//notify.paramurl2[511]=0;
		//strncpy_s(notify.paramurl3,_countof(notify.paramurl3), session->GetGameServerLib()->mClientParamURL3.c_str(),511);
		//notify.paramurl3[511]=0;
		//strncpy_s(notify.paramurl4,_countof(notify.paramurl4), session->GetGameServerLib()->mClientParamURL4.c_str(),511);
		//notify.paramurl4[511]=0;
		//strncpy_s(notify.paramurl5,_countof(notify.paramurl5), session->GetGameServerLib()->mClientParamURL5.c_str(),511);
		//notify.paramurl5[511]=0;
		//SendMsg(notify);
	}
	//notifyVipChange(session);
}
void CCliNetSession::notifyLoadShortcut(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::LoadShortcutNotify not;
	for(OGSLib::DomainData::ShortcutMap::iterator pos = session->GetCharacter()->mShortcutMap.begin();
		pos != session->GetCharacter()->mShortcutMap.end();
		++ pos)
	{
		GS_Client::LoadShortcutNotify::shortcut sc;
		sc.shortcut_id = pos->second.shortcut_id;
		sc.type = pos->second.type;
		sc.param = pos->second.param;
		not.shortcuts.push_back(sc);
	}
	SendMsg(not);
}
//void CCliNetSession::notifyPushLua2AsData(OGSLib::GameSession* session,const char* actionsetid,const char* panelid)
//{
//	if( session != mGameSession ) return;
//	GS_Client::Lua2AsNotify not;
//	strncpy_s(not.actionsetid,_countof(not.actionsetid),actionsetid,511);
//	not.actionsetid[511]=0;
//	strncpy_s(not.panelid,_countof(not.panelid),panelid,511);
//	not.panelid[511]=0;
//	OGSLib::DomainPlay::ActionSet* setlist = session->GetPlayer()->GetActionSet(std::string(panelid));
//	if( setlist )
//	{
//		for(OGSLib::DomainPlay::ActionSet::iterator p = setlist->begin();
//			p != setlist->end();
//			++ p)
//		{
//			GS_Client::Lua2AsNotify::actionset as;
//			as.id=p->id;
//			as.attr=p->attr;
//			as.value=p->value;
//			not.actionsets.push_back(as);
//		}
//	}
//	SendMsg(not);
//	//session->GetPlayer()->mActionSet.clear();
//}
void CCliNetSession::notifyGuildInfoChange(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::GuildInfoNotify notify;
	strncpy_s(notify.name,_countof(notify.name),mGameSession->GetPlayer()->GuildName().c_str(),511);
	notify.name[511]=0;
	if( notify.name[0] != 0 )
	{
		OGSLib::Guild* guild = mGameSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(mGameSession->GetPlayer()->GuildName());
		if( guild )
		{
			notify.title = guild->GetMemberTitle(mGameSession->GetPlayer()->SeedName());
		}
	}
	SendMsg(notify);
}

void CCliNetSession::notifyGhostGuildInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* src)
{
	GS_Client::GhostGuildInfo ggi;
	ggi.srcid = src->ID();
	ggi.title = OGSLib::GUILD_TITLE_TYPE_OUT;
	if (!(mGameSession->GetPlayer()->GetMap()&&mGameSession->GetPlayer()->GetMap()->FightAnonym()))
	{
		if (src->GuildName().size() > 0)
		{
			strncpy_s(ggi.guild_name,_countof(ggi.guild_name),src->GuildName().c_str(),511);
			ggi.guild_name[511]=0;
			ggi.title = src->GuildTitle();
		}
	}

	SendMsg(ggi);
}

void CCliNetSession::notifyGuildMemberChange(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::GuildMemberChangeNotify notify;
	SendMsg(notify);
}

void CCliNetSession::notifyGuildItem(OGSLib::GameSession* session,const char* guild_name,int mnum,const char* desp,int mlv,int entering,std::string leader_name,int guild_level){
	if( session != mGameSession ) return;
	GS_Client::ListGuildItemNotify notify;
	notify.name = guild_name;
	notify.member_number = mnum;
	notify.desp = desp;
	notify.master_level = mlv;
	notify.entering=entering;
	notify.leader_name = leader_name;
	notify.guild_level = guild_level;
	SendMsg(notify);
	GS_Client::ListGuildEndNotify en;SendMsg(en);
}
void CCliNetSession::notifyGuildtitle(OGSLib::GameSession* session,const char* guild_name,int title)
{
	if( session != mGameSession ) return;
	GS_Client::GuildInfoNotify notify;
	strncpy_s(notify.name,_countof(notify.name),guild_name,511);notify.name[511]=0;
	notify.title=title;
	SendMsg(notify);
}
void CCliNetSession::notifyListGuildItem(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::ListGuildItemRes res;
	std::string guild_name = mGameSession->GetDomainGuild()->GuildName();
	if( !guild_name.empty() )
	{
		OGSLib::Guild* guild = mGameSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(guild_name);
		if( guild )
		{
			for(OGSLib::Guild::ItemMap::const_iterator pos = guild->GetItems().begin();
				pos != guild->GetItems().end();++ pos)
			{
				GS_Client::ListGuildItemRes::guild_item guildItem;
				guildItem.pos = pos->first;
				guildItem.typeID = pos->second.mTypeID;
				guildItem.level = pos->second.mLevel;
				guildItem.zlevel = pos->second.mZlevel;
				OGSLib::ItemDef* id = mGameSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(pos->second.mTypeID);
				if (id)
				{
					guildItem.price = id->mEquipContribute;
					guildItem.job = id->mJob;
				}
				res.guild_items.push_back(guildItem);
			}
		}
	}
	SendMsg(res);
}
void CCliNetSession::notifyTiliChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src)
{
	if( session != mGameSession ) return;
	GS_Client::TiliChangeNotify not;
	not.id = src->ID();
	not.power = src->Power();
	not.power_max = src->MaxPower();
	SendMsg(not);
}
//void CCliNetSession::notifyUpGradeInfoChange(OGSLib::GameSession* session, int arry[14]) 
//{
//	if (session != mGameSession) return;
//	GS_Client::UpGradeInfoNotify not;
//	not.dc_min_count = arry[0];
//	not.dc_max_count = arry[1];
//	not.mc_min_count = arry[2];
//	not.mc_max_count = arry[3];
//	not.sc_min_count = arry[4];
//	not.sc_max_count = arry[5];
//	not.ac_min_count = arry[6];
//	not.ac_max_count = arry[7];
//	not.mac_min_count = arry[8];
//	not.mac_max_count = arry[9];
//	not.hp_count = arry[10];
//	not.mp_count = arry[11];
//	not.monster_hurt_count = arry[13];
//	not.up_lv_count = arry[12];
//	SendMsg(not);
//}
//void CCliNetSession::notifyUpGradeWeaponInfo(OGSLib::GameSession* session, int arry[14])
//{
//	if (session != mGameSession) return;
//	GS_Client::UpGradeWeaponInfoNotify not;
//	not.pos = arry[0];
//	not.dc_min = arry[1];
//	not.dc_max = arry[2];
//	not.mc_min = arry[3];
//	not.mc_max = arry[4];
//	not.sc_min = arry[5];
//	not.sc_max = arry[6];
//	not.ac_min = arry[7];
//	not.ac_max = arry[8];
//	not.mac_min = arry[9];
//	not.mac_max = arry[10];
//	not.hp = arry[11];
//	not.mp = arry[12];
//	not.percent = arry[13];
//	SendMsg(not);
//}
void CCliNetSession::notifyAttributeChange(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::AttributeChangeNotify notify;
	OGSLib::DomainPlay* player = session->GetPlayer();

	OGSLib::ObjectAttribute* attr = player->Attributes();

	notify.max_hp = attr->mMaxHp;
	notify.max_mp = attr->mMaxMp;

	notify.max_burden = attr->mMaxBurden;
	notify.burden = attr->mBurden;
	notify.max_load = attr->mMaxLoad;
	notify.load = attr->mLoad;
	notify.max_brawn = attr->mMaxBrawn;
	notify.brawn = attr->mBrawn;

	notify.maxac = attr->mACMax;
	notify.ac = attr->mAC;
	notify.maxmac = attr->mMACMax;
	notify.mac = attr->mMAC;
	notify.maxdc = attr->mDCMax;
	notify.dc = attr->mDC;
	notify.maxmc = attr->mMCMax;
	notify.mc = attr->mMC;
	notify.maxsc = attr->mSCMax;
	notify.sc = attr->mSC;

	notify.accuracy = attr->mAccuracy;
	notify.dodge = attr->mDodge;

	notify.dropprob = attr->mDropProb;
	notify.doubleattk = attr->mDoubleAttProb;
	notify.attrlevel = player->GetUpGradeMinLv();
	notify.maxpower = player->FightPoint();
	notify.luck = attr->mLuck;
	notify.honor = 0;//player->Honor();
	notify.xishou_pres = attr->mXishouPres;

	notify.critProb = attr->mBaojiProb;//��������
	notify.critPoint = attr->mBaojiPres;//�����˺�
	notify.tenacity = attr->mTenacity;//����
	notify.holyDam = attr->mHolyDam;//��ʥ
	SendMsg(notify);
}
void CCliNetSession::notifyListItemChange(OGSLib::GameSession* session,OGSLib::DomainPlay* src)
{
	if( session != mGameSession ) return;
	GS_Client::ListItemChangeNotify not;
	int count=0;
	for( OGSLib::DomainPlay::ItemMap::iterator pos = src->GetItemList().begin();
			pos != src->GetItemList().end();
			++ pos )
	{
		if( pos->mTypeID > 0 )
		{
			
			GS_Client::ListItemChangeNotify::NetItem id;
			id.position = pos->mPosition;
			id.type_id = pos->mTypeID;
			id.duration_max = pos->mDuraMax;
			id.duration = pos->mDuration;
			id.item_flags = pos->mItemFlags;
			id.show_flags = 0;
			id.level = pos->mLevel;
			id.number=pos->mNumber;
			id.add_ac = pos->mAddAC;
			id.add_mac = pos->mAddMAC;
			id.add_dc = pos->mAddDC;
			id.add_mc = pos->mAddMC;
			id.add_sc = pos->mAddSC;
			id.upd_ac = pos->mUpdAC;
			id.upd_mac = pos->mUpdMAC;
			id.upd_dc = pos->mUpdDC;
			id.upd_mc = pos->mUpdMC;
			id.upd_sc = pos->mUpdSC;
			id.upd_maxcount = pos->mUpdMaxCount;
			id.upd_failedcount = pos->mUpdFailedCount;
			id.luck = pos->mLuck;
			id.protect = pos->mProtect;
			id.sell_pricetype = pos->mSellPriceType;
			id.sell_price = pos->mSellPrice;
			id.add_hp = pos->mAddHp;
			id.add_mp = pos->mAddMp;
			id.create_time=pos->mCreateTime;
			id.last_time = pos->mLasttime;
			id.zlevel = pos->mZLevel;
			id.lock = pos->mLock;
			not.m_NetItemList.push_back(id);
			count++;
			if (count>=10)
			{
				SendMsg(not);
				not.m_NetItemList.clear();
				count=0;
			}
		}
	}
	if (!not.m_NetItemList.empty())
	{
		SendMsg(not);
	}
}

void CCliNetSession::notifyItemChange(OGSLib::GameSession* session,OGSLib::SubItem item,int show_flags)
{
	if( session != mGameSession ) return;
	GS_Client::ItemChangeNotify notify;
	notify.position = item.mPosition;
	notify.type_id = item.mTypeID;
	notify.duration_max = item.mDuraMax;
	notify.duration = item.mDuration;
	notify.item_flags = item.mItemFlags;
	notify.show_flags = show_flags;
	notify.level = item.mLevel;
	notify.number=item.mNumber;
	notify.add_ac = item.mAddAC;
	notify.add_mac = item.mAddMAC;
	notify.add_dc = item.mAddDC;
	notify.add_mc = item.mAddMC;
	notify.add_sc = item.mAddSC;
	notify.upd_ac = item.mUpdAC;
	notify.upd_mac = item.mUpdMAC;
	notify.upd_dc = item.mUpdDC;
	notify.upd_mc = item.mUpdMC;
	notify.upd_sc = item.mUpdSC;
	notify.upd_maxcount = item.mUpdMaxCount;
	notify.upd_failedcount = item.mUpdFailedCount;
	notify.luck = item.mLuck;
	notify.protect = item.mProtect;
	notify.sell_pricetype = item.mSellPriceType;
	notify.sell_price = item.mSellPrice;
	notify.add_hp = item.mAddHp;
	notify.add_mp = item.mAddMp;
	notify.create_time=item.mCreateTime;
	notify.last_time = item.mLasttime;
	notify.zlevel = item.mZLevel;
	notify.lock = item.mLock;
	SendMsg(notify);
}
void CCliNetSession::notifyItemPanelFresh(OGSLib::GameSession* session,int panel)
{
	if( session != mGameSession ) return;
	GS_Client::FreshPanelInfoNotify notify;
	notify.panel_id = panel;
	SendMsg(notify);
}
//void CCliNetSession::notifyScriptItemChange(OGSLib::GameSession* session,int pos)
//{
//	if( session != mGameSession ) return;
//	OGSLib::DomainPlay* player = session->GetPlayer();
//	OGSLib::ScriptItemAttibuteParam* siap=player->get_siap(pos);
//	GS_Client::ScriptItemChangeNotify notify;
//	if(siap)
//	{
//		notify.position = pos;
//		notify.mMaxHpMul = siap->mMaxHpMul;
//		notify.mMaxHpAdd = siap->mMaxHpAdd;
//		notify.mMaxMpMul = siap->mMaxMpMul;
//		notify.mMaxMpAdd = siap->mMaxMpAdd;
//
//		notify.mACAdd = siap->mACAdd;
//		notify.mACMaxAdd = siap->mACMaxAdd;
//		notify.mACMul = siap->mACMul;
//		notify.mACMaxMul = siap->mACMaxMul;
//
//		notify.mMACAdd = siap->mMACAdd;
//		notify.mMACMaxAdd = siap->mMACMaxAdd;
//		notify.mMACMul = siap->mMACMul;
//		notify.mMACMaxMul = siap->mMACMaxMul;
//
//		notify.mDCAdd = siap->mDCAdd;
//		notify.mDCMaxAdd = siap->mDCMaxAdd;
//		notify.mDCMul = siap->mDCMul;
//		notify.mDCMaxMul = siap->mDCMaxMul;
//
//		notify.mMCAdd = siap->mMCAdd;
//		notify.mMCMaxAdd = siap->mMCMaxAdd;
//		notify.mMCMul = siap->mMCMul;
//		notify.mMCMaxMul = siap->mMCMaxMul;
//
//		notify.mSCAdd = siap->mSCAdd;
//		notify.mSCMaxAdd = siap->mSCMaxAdd;
//		notify.mSCMul = siap->mSCMul;
//		notify.mSCMaxMul = siap->mSCMaxMul;
//
//		SendMsg(notify);
//	}
//}
//void CCliNetSession::notifyScriptItemInfoChange(OGSLib::GameSession* session,int srcid,int pos)
//{
//	if( session != mGameSession ) return;
//	OGSLib::DomainPlay* player = session->GetPlayer();
//	OGSLib::ScriptItemAttibuteParam* siap=player->get_siap(pos);
//	GS_Client::ScriptItemInfoChangeNotify notify;
//	if(siap)
//	{
//		notify.position = pos;
//		notify.src_id = srcid;
//		notify.mMaxHpMul = siap->mMaxHpMul;
//		notify.mMaxHpAdd = siap->mMaxHpAdd;
//		notify.mMaxMpMul = siap->mMaxMpMul;
//		notify.mMaxMpAdd = siap->mMaxMpAdd;
//
//		notify.mACAdd = siap->mACAdd;
//		notify.mACMaxAdd = siap->mACMaxAdd;
//		notify.mACMul = siap->mACMul;
//		notify.mACMaxMul = siap->mACMaxMul;
//
//		notify.mMACAdd = siap->mMACAdd;
//		notify.mMACMaxAdd = siap->mMACMaxAdd;
//		notify.mMACMul = siap->mMACMul;
//		notify.mMACMaxMul = siap->mMACMaxMul;
//
//		notify.mDCAdd = siap->mDCAdd;
//		notify.mDCMaxAdd = siap->mDCMaxAdd;
//		notify.mDCMul = siap->mDCMul;
//		notify.mDCMaxMul = siap->mDCMaxMul;
//
//		notify.mMCAdd = siap->mMCAdd;
//		notify.mMCMaxAdd = siap->mMCMaxAdd;
//		notify.mMCMul = siap->mMCMul;
//		notify.mMCMaxMul = siap->mMCMaxMul;
//
//		notify.mSCAdd = siap->mSCAdd;
//		notify.mSCMaxAdd = siap->mSCMaxAdd;
//		notify.mSCMul = siap->mSCMul;
//		notify.mSCMaxMul = siap->mSCMaxMul;
//
//		SendMsg(notify);
//	}
//}
void CCliNetSession::notifyKuafuInfo(OGSLib::GameSession* session,const char* ip,const char* port,const char* param){
	if( session != mGameSession ) return;
	GS_Client::KuafuInfoNotify notify;
	notify.charname = mGameSession->GetGameServerLib()->mServerTitle + mCharID;
	notify.loginid = mGameSession->GetGameServerLib()->mServerTitle + mLoginID;
	std::string localUrl;
	m_app->m_pGameServerLib->GetLocalDomain(localUrl);
	if(!localUrl.empty()){
		notify.localip = localUrl;
	}
	else{
		notify.localip = m_app->mKuaFuLocalIP;
	}
	notify.localport=m_app->mKuaFuLocalPort;
	std::string url;
	m_app->m_pGameServerLib->GetKuafuURL(url);
	if(ip&&!std::string(ip).empty()){
		notify.kuafuip=ip;
	}
	else if(!url.empty()){
		notify.kuafuip = url;
	}
	else{
		notify.kuafuip=m_app->mKuaFuServerIP;
	}
	std::string setPort;
	m_app->m_pGameServerLib->GetKuafuPort(setPort);
	if(port&&!std::string(port).empty()){
		notify.kuafuport=port;
	}
	else if(!setPort.empty()){
		notify.kuafuport = setPort;
	}
	else{
		notify.kuafuport=m_app->mKuaFuServerPort;
	}
	notify.kuafuparam=param;
	notify.localPTID=m_app->mLocalPTID;
	notify.localServerID=m_app->mLocalServerID;
	notify.localArea=mGameSession->GetGameServerLib()->mServerTitle;
	int ticket_seed=m_app->m_KuaFuManager->GenTicketSeed();
	unsigned char md5[16];
	unsigned char src[1024];memset(src,0,256);
	sprintf((char*)src,"%s_%s_%s_%s_%s_%d_%s_%s_%s",notify.loginid.c_str(),notify.charname.c_str(),notify.kuafuip.c_str(),notify.kuafuport.c_str(),param,ticket_seed,notify.localip.c_str(),notify.localport.c_str(),m_app->mKuaFuMD5Pad.c_str());
	LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
	char temp[256];
	_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
	char tick[256];
	sprintf(tick,"%s_%d",temp,ticket_seed);
	notify.ticket=tick;
	notify.ticketseed=ticket_seed;
	m_app->m_KuaFuManager->AddAuthTick(notify.ticket);
	SendMsg(notify);
}
void CCliNetSession::notifyKuafuEnterMainServer(OGSLib::GameSession* session,const char* result){
	if( session != mGameSession ) return;
	if( !m_app->mKuaFuService ) return;
	//m_app->m_KuaFuService->RemoveCallBackSession(mKuafuSIP.c_str(),mKuafuSPort.c_str(),this);
	GS_Client::KuafuEnterMainServerNotify notify;
	notify.result="1";
	unsigned char md5[16];
	unsigned char src[1024];memset(src,0,256);
	sprintf((char*)src,"%s_%s",notify.result.c_str(),m_app->mKuaFuMD5Pad.c_str());
	LEUD::CryptographyMD5::Do(md5,src,static_cast<int>(strlen((char*)src)));
	char temp[256];
	_snprintf_s(temp,_countof(temp),255,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],md5[14],md5[15]);
	notify.ticket=temp;
	SendMsg(notify);
}
void CCliNetSession::notifySkillChange(OGSLib::GameSession* session,OGSLib::SubSkill skill)
{
	if( session != mGameSession ) return;
	OGSLib::SkillDef* sd = session->GetGameServerLib()->GetSkillDefineManager()->GetSkillDefine(skill.mTypeID,skill.mLevel);
	if (sd)
	{
		GS_Client::SkillChangeNotify notify;
		notify.skill_type_id = skill.mTypeID;
		notify.lv = skill.mLevel;
		notify.exp = skill.mExp;
		notify.param1 = skill.mParam1;
		SendMsg(notify);
	}
}
void CCliNetSession::notifyGameMoneyChange(OGSLib::GameSession* session,LEUD_INT64 gm_change,int vc_change,LEUD_INT64 gmb_change,int vcb_change)
{
	if( session != mGameSession ) return;
	GS_Client::GameMoneyChangeNotify notify;
	notify.new_game_money = session->GetPlayer()->gamemoney();
	notify.new_game_money_bind = session->GetPlayer()->gamemoneybind();
	notify.new_vcion = session->GetPlayer()->vcoin();
	notify.new_vcion_bind = session->GetPlayer()->vcoinbind();
	notify.gm_change = gm_change;
	notify.vc_change = vc_change;
	notify.gmb_change = gmb_change;
	notify.vcb_change = vcb_change;
	SendMsg(notify);
}
void CCliNetSession::notifyTaskChange(OGSLib::GameSession* session,int task_id,int flags)
{
	if( session != mGameSession ) return;
	OGSLib::SubTask* ti = session->GetPlayer()->GetTask(task_id);
	if( ti )
	{
		GS_Client::TaskChangeNotify notify;
		notify.TaskID = task_id;
		notify.Flags = flags;
		notify.TaskState = ti->TaskState;
		notify.TaskParam1 = ti->TaskParam1;
		notify.TaskParam2 = ti->TaskParam2;
		notify.TaskParam3 = ti->TaskParam3;
		notify.TaskParam4 = ti->TaskParam4;
		strncpy_s(notify.TaskName,_countof(notify.TaskName),ti->TaskName.c_str(),511);
		notify.TaskName[511]=0;
		//strncpy_s(notify.TaskShortDesp,_countof(notify.TaskShortDesp),ti->TaskShortDesp.c_str(),511);
		notify.TaskShortDesp2=ti->TaskShortDesp.c_str();
		SendMsg(notify);
	}
}
void CCliNetSession::resTaskDesp(OGSLib::GameSession* session,int task_id,short task_state,const char* desp)
{
	if( session != mGameSession ) return;
	GS_Client::TaskDespRes res;
	res.task_id=task_id;
	res.task_state=task_state;
	res.desp=desp;
	SendMsg(res);
}
void CCliNetSession::notifyNPCShowFlags(OGSLib::GameSession* session,unsigned int npc_id,int flags)
{
	if( session != mGameSession ) return;
	GS_Client::NpcShowFlagsNotify notify;
	notify.npc_id=npc_id;
	notify.flags=flags;
	SendMsg(notify);
}
void CCliNetSession::notifyInfoPlayItem(int src_id,OGSLib::SubItem* item,int pos)
{
	if( item )
	{
		GS_Client::InfoItemChangeNotify notify;
		notify.src_id = src_id;
		notify.position=item->mPosition;
		notify.type_id=item->mTypeID;
		notify.duration_max=item->mDuraMax;
		notify.duration=item->mDuration;
		notify.item_flags=item->mItemFlags;
		notify.level=item->mLevel;
		notify.zlevel=item->mZLevel;
		notify.upd_count=item->mNumber;
		notify.add_ac=item->mAddAC;
		notify.add_mac=item->mAddMAC;
		notify.add_dc=item->mAddDC;
		notify.add_mc=item->mAddMC;
		notify.add_sc=item->mAddSC;
		notify.upd_ac=item->mUpdAC;
		notify.upd_mac=item->mUpdMAC;
		notify.upd_dc=item->mUpdDC;
		notify.upd_mc=item->mUpdMC;
		notify.upd_sc=item->mUpdSC;
		notify.luck = item->mLuck;
		notify.protect = item->mProtect;
		notify.add_hp = item->mAddHp;
		notify.add_mp = item->mAddMp;
		notify.create_time=item->mCreateTime;
		SendMsg(notify);
	}
	else
	{
		GS_Client::InfoItemChangeNotify notify;
		notify.src_id = 0;
		notify.position=pos;
		notify.type_id= 0;
		notify.duration_max= 0;
		notify.duration= 0;
		notify.item_flags= 0;
		notify.level= 0;
		notify.zlevel= 0;
		notify.upd_count= 0;
		notify.add_ac= 0;
		notify.add_mac= 0;
		notify.add_dc= 0;
		notify.add_mc= 0;
		notify.add_sc= 0;
		notify.upd_ac= 0;
		notify.upd_mac= 0;
		notify.upd_dc= 0;
		notify.upd_mc= 0;
		notify.upd_sc= 0;
		notify.luck = 0;
		notify.protect = 0;
		notify.add_hp = 0;
		notify.add_mp = 0;
		notify.create_time=0;
		SendMsg(notify);
	}
}
//void CCliNetSession::notifyOpenPK(OGSLib::GameSession* session)
// {
//	 if( session != mGameSession ) return;
//	 GS_Client::PKConfirmNotify notify;
//	 SendMsg(notify);
// }
void CCliNetSession::resInfoPlayer(OGSLib::GameSession* session,OGSLib::DomainPlay* player,OGSLib::MiniPlayerInfo* info)
{
	if( session != mGameSession ) return;
	GS_Client::InfoPlayerRes res;
	res.player_id=player->ID();

	std::string name = player->Name();
	if( player->TypeIsPlayer() )
	{
		if(mGameSession->GetPlayer()->GetMap())
		{
			if(mGameSession->GetPlayer()->GetMap()->FightAnonym())
			{
				name = mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_ANONYM_NAME;
			}
		}
	}

	strncpy_s(res.name,_countof(res.name),player->Name(),511);
	res.name[511]=0;

	res.loverName[0] = 0;
	//if (player->CoupleID() > 0 &&  player->LoverName())
	//{
	//	strncpy_s(res.loverName,_countof(res.loverName),player->LoverName(),511);
	//	res.loverName[511] = 0;
	//}
	strncpy_s(res.guild,_countof(res.guild),player->GuildName().c_str(),511);
	res.guild[511]=0;
	res.gender=player->gender();
	res.maxpower = player->FightPoint();
	res.job = player->job();
	res.level = player->level();
	if (player->GetModel(5)>0)
	{
		res.viplevel = player->GetModel(5);
	}
	else
	{
		res.viplevel = 0;
	}

	if (player->Param(5005).length() && player->Param(5006).length())
	{
		res.mountlv = atoi(player->Param(5005).c_str()) * 100 + atoi(player->Param(5006).c_str());
	}
	else
	{
		res.mountlv = 0;
	}
	res.winglv = player->GetWingLv();
	notifyInfoPlayItem(player->ID(),info->item_belt_id,ITEM_BELT_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_boot_id,ITEM_BOOT_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_cloth_id,ITEM_CLOTH_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_glove1_id,ITEM_GLOVE1_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_glove2_id,ITEM_GLOVE2_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_hat_id,ITEM_HAT_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_nicklace_id,ITEM_NICKLACE_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_ring1_id,ITEM_RING1_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_ring2_id,ITEM_RING2_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_weapon_id,ITEM_WEAPON_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_huizhang,ITEM_MEDAL_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_hunqi,ITEM_HUNQI_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_xueshi,ITEM_XUESHI_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_wing,ITEM_WING_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_zuoji,ITEM_MOUNT_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_tejie,ITEM_SPECIAL_RING_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_fashion_weapon_id,ITEM_FASHION_WEAPON_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_fashion_cloth_id,ITEM_FASHION_CLOTH_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_xuefu,ITEM_XUEFU_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_hudun,ITEM_HUDUN_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_baoshi,ITEM_BAOSHI_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_hunzhu,ITEM_HUNZHU_POSITION);

	notifyInfoPlayItem(player->ID(),info->item_jade_pendant,ITEM_JADE_PENDANT_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_shield,ITEM_SHIELD_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_mirror_armour,ITEM_MIRROR_ARMOUR_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_face_cloth,ITEM_FACE_CLOTH_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_dragon_heart,ITEM_DRAGON_HEART_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_wolfang,ITEM_WOLFANG_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_dragon_bone,ITEM_DRAGON_BONE_POSITION);
	notifyInfoPlayItem(player->ID(),info->item_catilla,ITEM_CATILLA_POSITION);

	for( std::map<int,int>::iterator pos = info->model->begin();
		pos != info->model->end();
		++ pos )
	{
		notifySetModel(session,player->ID(),pos->first,pos->second);
	}
	for(int i= 400;i<=450;i++)
	{
		std::map<int,std::string>::iterator pos = player->GetParamVar().find(i);
		if( pos != player->GetParamVar().end() )
		{
			notifyParamData(session,player->ID(),pos->first,pos->second);
		}
		else
		{
			notifyParamData(session,player->ID(),i,"");
		}
	}
	SendMsg(res);
}
//void CCliNetSession::resInfoRobot(OGSLib::GameSession* session,OGSLib::PlayRobot* robot,OGSLib::MiniPlayerInfo* info)
//{
//	if( session != mGameSession ) return;
//	GS_Client::InfoPlayerRes res;
//	res.player_id=robot->ID();
//	strncpy_s(res.name,_countof(res.name),robot->Name(),511);
//	res.name[511]=0;
//	res.loverName[0] = 0;
//	res.guild[0]=0;
//	res.coutry[0]=0;
//	res.gender=robot->Gender();
//	notifyInfoPlayItem(robot->ID(),info->item_belt_id,ITEM_BELT_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_boot_id,ITEM_BOOT_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_cloth_id,ITEM_CLOTH_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_glove1_id,ITEM_GLOVE1_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_glove2_id,ITEM_GLOVE2_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_hat_id,ITEM_HAT_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_nicklace_id,ITEM_NICKLACE_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_ring1_id,ITEM_RING1_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_ring2_id,ITEM_RING2_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_weapon_id,ITEM_WEAPON_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_huizhang,ITEM_MDEAL_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_hunqi,ITEM_HUNQI_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_xueshi,ITEM_XUESHI_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_zuoji,ITEM_MOUNT_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_fashion_weapon_id,ITEM_FASHION_WEAPON_POSITION);
//	notifyInfoPlayItem(robot->ID(),info->item_fashion_cloth_id,ITEM_FASHION_CLOTH_POSITION);
//	SendMsg(res);
//}
void CCliNetSession::notifyListItemPlusDesp(OGSLib::GameSession* session)
{
	/*if( session != mGameSession ) return;
	GS_Client::ListItemDespNotify not;
	int count=0;*/
	/*for(OGSLib::ManagerItemDef::ItemDefineMap::iterator pos = session->GetGameServerLib()->GetItemDefineManager()->getItemDefineMap().begin();
		pos != mGameSession->GetGameServerLib()->GetItemDefineManager()->getItemDefineMap().end();
		++ pos)
	{	
		GS_Client::ListItemDespNotify::ItemDesp id;
		int stackmax = session->GetGameServerLib()->GetItemDefineManager()->GetItemStackMax(pos->second.mTypeID);
		strncpy_s(id.mName,_countof(id.mName),pos->second.mName.c_str(),511);
		id.mName[511]=0;
		id.mDesp2 = pos->second.mDesp.c_str();
		id.mTypeID = pos->second.mTypeID;
		id.mIconID = pos->second.mIconID;
		id.mPrice = pos->second.mPrice;
		id.mWeight = pos->second.mWeight;
		id.mLastTime = pos->second.mLastTime;
		id.mDurationMax = pos->second.mDurationMax;
		id.mNeedType = pos->second.mNeedType;
		id.mNeedParam = pos->second.mNeedParam;
		id.mReinLv = pos->second.mNeedReinLv;
		id.mColor = pos->second.mColor;
		id.mNotips = pos->second.mNotips;
		id.mResMale = pos->second.mResMale;
		id.mResFemale = pos->second.mResFemale;
		id.mAC = pos->second.mAC;
		id.mACMax = pos->second.mACMax;
		id.mMAC = pos->second.mMAC;
		id.mMACMax = pos->second.mMACMax;
		id.mDC = pos->second.mDC;
		id.mDCMax = pos->second.mDCMax;
		id.mMC = pos->second.mMC;
		id.mMCMax = pos->second.mMCMax;
		id.mSC = pos->second.mSC;
		id.mSCMax = pos->second.mSCMax;
		id.mLuck = pos->second.mLuck;
		id.mCurse = pos->second.mCurse;
		id.mAccuracy = pos->second.mAccuracy;
		id.mDodge = pos->second.mDodge;
		id.mAntiMagic = pos->second.mAntiMagic;
		id.mAntiPoison = pos->second.mAntiPoison;
		id.mHpRecover = pos->second.mHpRecover;
		id.mMpRecover = pos->second.mMpRecover;
		id.mPoisonRecover = pos->second.mPoisonRecover;
		id.mMabiProb = pos->second.mMabi_prob;
		id.mMabiDura = pos->second.mMabi_dura;
		id.mDixiaoPres = pos->second.mDixiao_pres;
		id.mFuyuanCd = pos->second.mFuyuan_cd;
		id.mFuyuanPres=pos->second.mFuyuan_pres;
		id.mMaxHp = pos->second.mMax_hp;
		id.mMaxMp = pos->second.mMax_mp;
		id.mMaxHpPres = pos->second.mMax_hp_pres;
		id.mMaxMpPres = pos->second.mMax_mp_pres;
		id.mNeedZsLevel=pos->second.mNeedZsLevel;
		id.mEquipLevel = pos->second.mEquipLevel;
		id.mEquipComp = pos->second.mEquipComp;
		id.mEquipContribute = pos->second.mEquipContribute;
		id.mShowDest = pos->second.mShowDest;
		id.mAddPower = pos->second.mAddPower;
		id.mItemBg = pos->second.mItemBg;
		id.mJob = pos->second.mJob;
		id.mGender = pos->second.mGender;
		id.SubType = pos->second.SubType;
		id.HPChange = pos->second.HPChange;
		id.MPChange = pos->second.MPChange;
		id.ZipType = pos->second.ZipType;
		id.ZipNumber = pos->second.ZipNumber;
		id.mBaoji = pos->second.mBaoji;
		id.mDrop_luck = pos->second.mDrop_luck;
		id.mStackMax = stackmax;
		id.mEquipType = pos->second.mEquipType;
		id.mXishou_prob = pos->second.mXishou_prob;
		id.mXishou_pres = pos->second.mXishou_pres;
		id.mFantan_prob = pos->second.mFantan_prob;
		id.mFantan_pres = pos->second.mFantan_pres;
		id.mBaoji_prob = pos->second.mBaoji_prob;
		id.mBaoji_pres = pos->second.mBaoji_pres;
		id.mXixue_prob = pos->second.mXixue_prob;
		id.mXixue_pres = pos->second.mXixue_pres;
		id.randac = pos->second.mRandAC;
		id.randmac = pos->second.mRandMAC;
		id.randdc = pos->second.mRandDC;
		id.randmc = pos->second.mRandMC;
		id.randsc = pos->second.mRandSC;
		id.mRecycleExp = pos->second.mRecycleExp;
		id.mRecycleXuefu = pos->second.mRecycleXuefu;
		id.mCanDestroy = pos->second.mCanDestroy;
		id.mCanDepot = pos->second.mCanDepot;
		id.mSource = pos->second.mSource.c_str();
		not.m_ItemDefList.push_back(id);
		count++;
		if (count>=10)
		{
			SendMsg(not);
			not.m_ItemDefList.clear();
		}
	}*/
	/*if (!not.m_ItemDefList.empty())
	{
		SendMsg(not);
	}*/
}

void CCliNetSession::notifyListUpgradeDesp(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::ListUpgradeDespNotify not;
	int count=0;
	for(OGSLib::ManagerEquipUpgradeDef::EquipUpgradeDefMap::iterator pos = mGameSession->GetGameServerLib()->GetEquipUpgradeDefManager()->getUpgradeDefineMap().begin();
		pos != mGameSession->GetGameServerLib()->GetEquipUpgradeDefManager()->getUpgradeDefineMap().end();
		++ pos)
	{	
		GS_Client::ListUpgradeDespNotify::UpgradeDesp ud;
		ud.mJob = pos->second.mJob;
		ud.mEquipType = pos->second.mEquipType;
		ud.mLevel = pos->second.mLevel;
		ud.mDC = pos->second.mDC;
		ud.mDCMax =	pos->second.mDCMax;
		ud.mMC = pos->second.mMC;
		ud.mMCMax= pos->second.mMCMax;
		ud.mSC = pos->second.mSC;
		ud.mSCMax = pos->second.mSCMax;
		ud.mAC = pos->second.mAC;
		ud.mACMax = pos->second.mACMax;
		ud.mMAC = pos->second.mMAC;
		ud.mMACMax = pos->second.mMACMax;
		not.m_UpgradeDefList.push_back(ud);
		count++;
		if (count>=30)
		{
			SendMsg(not);
			not.m_UpgradeDefList.clear();
			count=0;
		}
	}
	if (!not.m_UpgradeDefList.empty())
	{
		SendMsg(not);
	}
}

void CCliNetSession::notifyListItemDesp(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::ListItemDespNotify not;
	int count=0;
	for(OGSLib::ManagerItemDef::ItemDefineMap::iterator pos = session->GetGameServerLib()->GetItemDefineManager()->getItemDefineMap().begin();
		pos != mGameSession->GetGameServerLib()->GetItemDefineManager()->getItemDefineMap().end();
		++ pos)
	{	
		GS_Client::ListItemDespNotify::ItemDesp id;
		int stackmax = session->GetGameServerLib()->GetItemDefineManager()->GetItemStackMax(pos->second.mTypeID);
		strncpy_s(id.mName,_countof(id.mName),pos->second.mName.c_str(),511);
		id.mName[511]=0;
		id.mDesp2 = pos->second.mDesp.c_str();
		id.mTypeID = pos->second.mTypeID;
		id.mIconID = pos->second.mIconID;
		id.mPrice = pos->second.mPrice;
		id.mWeight = pos->second.mWeight;
		id.mLastTime = pos->second.mLastTime;
		id.mDurationMax = pos->second.mDurationMax;
		id.mNeedType = pos->second.mNeedType;
		id.mNeedParam = pos->second.mNeedParam;
		id.mReinLv = pos->second.mNeedReinLv;
		id.mColor = pos->second.mColor;
		id.mNotips = pos->second.mNotips;
		id.mResMale = pos->second.mResMale;
		id.mResFemale = pos->second.mResFemale;
		id.mAC = pos->second.mAC;
		id.mACMax = pos->second.mACMax;
		id.mMAC = pos->second.mMAC;
		id.mMACMax = pos->second.mMACMax;
		id.mDC = pos->second.mDC;
		id.mDCMax = pos->second.mDCMax;
		id.mMC = pos->second.mMC;
		id.mMCMax = pos->second.mMCMax;
		id.mSC = pos->second.mSC;
		id.mSCMax = pos->second.mSCMax;
		id.mLuck = pos->second.mLuck;
		id.mCurse = pos->second.mCurse;
		id.mAccuracy = pos->second.mAccuracy;
		id.mDodge = pos->second.mDodge;
		id.mAntiMagic = pos->second.mAntiMagic;
		id.mAntiPoison = pos->second.mAntiPoison;
		id.mHpRecover = pos->second.mHpRecover;
		id.mMpRecover = pos->second.mMpRecover;
		id.mPoisonRecover = pos->second.mPoisonRecover;
		id.mMabiProb = pos->second.mMabi_prob;
		id.mMabiDura = pos->second.mMabi_dura;
		id.mDixiaoPres = pos->second.mDixiao_pres;
		id.mFuyuanCd = pos->second.mFuyuan_cd;
		id.mFuyuanPres=pos->second.mFuyuan_pres;
		id.mMaxHp = pos->second.mMax_hp;
		id.mMaxMp = pos->second.mMax_mp;
		id.mMaxHpPres = pos->second.mMax_hp_pres;
		id.mMaxMpPres = pos->second.mMax_mp_pres;
		id.mNeedZsLevel=pos->second.mNeedZsLevel;
		id.mEquipLevel = pos->second.mEquipLevel;
		id.mEquipComp = pos->second.mEquipComp;
		id.mEquipGroup = pos->second.mEquipGroup;
		id.mEquipContribute = pos->second.mEquipContribute;
		id.mShowDest = pos->second.mShowDest;
		id.mAddPower = pos->second.mAddPower;
		id.mItemBg = pos->second.mItemBg;
		id.mJob = pos->second.mJob;
		id.mGender = pos->second.mGender;
		id.SubType = pos->second.SubType;
		id.HPChange = pos->second.HPChange;
		id.MPChange = pos->second.MPChange;
		id.ZipType = pos->second.ZipType;
		id.ZipNumber = pos->second.ZipNumber;
		id.mBaoji = pos->second.mBaoji;
		id.mDrop_luck = pos->second.mDrop_luck;
		id.mStackMax = stackmax;
		id.mEquipType = pos->second.mEquipType;
		id.mXishou_prob = pos->second.mXishou_prob;
		id.mXishou_pres = pos->second.mXishou_pres;
		id.mFantan_prob = pos->second.mFantan_prob;
		id.mFantan_pres = pos->second.mFantan_pres;
		id.mBaoji_prob = pos->second.mBaoji_prob;
		id.mBaoji_pres = pos->second.mBaoji_pres;
		id.mXixue_prob = pos->second.mXixue_prob;
		id.mXixue_pres = pos->second.mXixue_pres;
		id.randac = pos->second.mRandAC;
		id.randmac = pos->second.mRandMAC;
		id.randdc = pos->second.mRandDC;
		id.randmc = pos->second.mRandMC;
		id.randsc = pos->second.mRandSC;
		id.mRecycleExp = pos->second.mRecycleExp;
		id.mRecycleXuefu = pos->second.mRecycleXuefu;
		id.mCanUse = pos->second.mCanUse;
		id.mCanDestroy = pos->second.mCanDestroy;
		id.mCanDepot = pos->second.mCanDepot;
		id.mCanPush = pos->second.mCanPush;
		id.mBagShow = pos->second.mBagShow;
		id.mTimesLimit = pos->second.mTimesLimit;
		id.mSource = pos->second.mSource.c_str();
		not.m_ItemDefList.push_back(id);
		count++;
		if (count>=10)
		{
			SendMsg(not);
			not.m_ItemDefList.clear();
			count=0;
		}
	}
	if (!not.m_ItemDefList.empty())
	{
		SendMsg(not);
	}
}
void CCliNetSession::notifyItemDesp(OGSLib::GameSession* session,int item_id)
{
	if( session != mGameSession ) return;
	GS_Client::ItemDespNotify notify;
	OGSLib::ItemDef* id = session->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(item_id);
	int stackmax = session->GetGameServerLib()->GetItemDefineManager()->GetItemStackMax(item_id);
	if( id )
	{
		strncpy_s(notify.mName,_countof(notify.mName),id->mName.c_str(),511);
		notify.mName[511]=0;
		//strncpy_s(notify.mDesp,_countof(notify.mDesp),id->mDesp.c_str(),511);
		//notify.mDesp[511]=0;
		notify.mDesp2 = id->mDesp.c_str();
		notify.mTypeID=id->mTypeID;
		notify.mIconID=id->mIconID;
		notify.mPrice=id->mPrice;
		notify.mWeight=id->mWeight;
		notify.mLastTime=id->mLastTime;
		notify.mDurationMax=id->mDurationMax;
		notify.mNeedType=id->mNeedType;
		notify.mNeedParam=id->mNeedParam;
		notify.mReinLv=id->mNeedReinLv;
		notify.mColor = id->mColor;
		notify.mNotips = id->mNotips;

		notify.mResMale=id->mResMale;
		notify.mResFemale=id->mResFemale;
		notify.mAC=id->mAC;
		notify.mACMax=id->mACMax;
		notify.mMAC=id->mMAC;
		notify.mMACMax=id->mMACMax;
		notify.mDC=id->mDC;
		notify.mDCMax=id->mDCMax;
		notify.mMC=id->mMC;
		notify.mMCMax=id->mMCMax;
		notify.mSC=id->mSC;
		notify.mSCMax=id->mSCMax;
		notify.mLuck=id->mLuck;
		notify.mCurse=id->mCurse;
		notify.mAccuracy=id->mAccuracy;
		notify.mDodge=id->mDodge;
		notify.mAntiMagic=id->mAntiMagic;
		notify.mAntiPoison=id->mAntiPoison;
		notify.mHpRecover=id->mHpRecover;
		notify.mMpRecover=id->mMpRecover;
		notify.mPoisonRecover=id->mPoisonRecover;

		notify.SubType=id->SubType;
		notify.HPChange=id->HPChange;
		notify.MPChange=id->MPChange;
		notify.ZipType=id->ZipType;
		notify.ZipNumber=id->ZipNumber;

		notify.mMabiProb = id->mMabi_prob;
		notify.mMabiDura = id->mMabi_dura;
		notify.mDixiaoPres = id->mDixiao_pres;
		notify.mFuyuanCd = id->mFuyuan_cd;
		notify.mFuyuanPres=id->mFuyuan_pres;
		notify.mMaxHp = id->mMax_hp;
		notify.mMaxMp = id->mMax_mp;
		notify.mMaxHpPres = id->mMax_hp_pres;
		notify.mMaxMpPres = id->mMax_mp_pres;
		notify.mNeedZsLevel=id->mNeedZsLevel;
		notify.mEquipLevel=id->mEquipLevel;
		notify.mJob = id->mJob;
		notify.mGender = id->mGender;
		notify.mEquipComp = id->mEquipComp;
		notify.mEquipGroup = id->mEquipGroup;
		notify.mEquipContribute = id->mEquipContribute;
		notify.mShowDest = id->mShowDest;
		notify.mAddPower = id->mAddPower;

		notify.mBaoji = id->mBaoji;
		notify.mDrop_luck = id->mDrop_luck;
		notify.mStackMax = stackmax;

		notify.mEquipType = id->mEquipType;
		notify.mXishou_prob = id->mXishou_prob;
		notify.mXishou_pres = id->mXishou_pres;
		notify.mFantan_prob = id->mFantan_prob;
		notify.mFantan_pres = id->mFantan_pres;
		notify.mBaoji_prob = id->mBaoji_prob;
		notify.mBaoji_pres = id->mBaoji_pres;
		notify.mXixue_prob = id->mXixue_prob;
		notify.mXixue_pres = id->mXixue_pres;

		notify.randac = id->mRandAC;
		notify.randmac = id->mRandMAC;
		notify.randdc = id->mRandDC;
		notify.randmc = id->mRandMC;
		notify.randsc = id->mRandSC;
		notify.mItemBg = id->mItemBg;
		notify.mRecycleExp = id->mRecycleExp;
		notify.mRecycleXuefu = id->mRecycleXuefu;
		notify.mCanUse = id->mCanUse;
		notify.mCanDestroy = id->mCanDestroy;
		notify.mCanDepot = id->mCanDepot;
		notify.mCanPush = id->mCanPush;
		notify.mBagShow = id->mBagShow;
		notify.mTimesLimit = id->mTimesLimit;
		notify.mSource = id->mSource.c_str();
		SendMsg(notify);

        if (!id->mPlus.empty()) {
            GS_Client::ItemPlusDespNotifyGroup not;
            not.items.resize(id->mPlus.size());
            for (int i = 0; i < id->mPlus.size(); ++i) {
                not.items[i].mItemPlusID = id->mPlus[i].mItemPlusID;
                not.items[i].mTypeID = id->mPlus[i].mTypeID;

                not.items[i].mNeedType = id->mPlus[i].mNeedType;
                not.items[i].mNeedParam1 = id->mPlus[i].mNeedParam1;
                not.items[i].mNeedParam2 = id->mPlus[i].mNeedParam2;
                not.items[i].mNeedParam3 = id->mPlus[i].mNeedParam3;
                not.items[i].mNeedParam4 = id->mPlus[i].mNeedParam4;
                not.items[i].mNeedParam5 = id->mPlus[i].mNeedParam5;

                not.items[i].mAC = id->mPlus[i].mAC;
                not.items[i].mACMax = id->mPlus[i].mACMax;
                not.items[i].mMAC = id->mPlus[i].mMAC;
                not.items[i].mMACMax = id->mPlus[i].mMACMax;
                not.items[i].mDC = id->mPlus[i].mDC;
                not.items[i].mDCMax = id->mPlus[i].mDCMax;
                not.items[i].mMC = id->mPlus[i].mMC;
                not.items[i].mMCMax = id->mPlus[i].mMCMax;
                not.items[i].mSC = id->mPlus[i].mSC;
                not.items[i].mSCMax = id->mPlus[i].mSCMax;

                not.items[i].mLuck = id->mPlus[i].mLuck;
                not.items[i].mCurse = id->mPlus[i].mCurse;
                not.items[i].mAccuracy = id->mPlus[i].mAccuracy;
                not.items[i].mDodge = id->mPlus[i].mDodge;
                not.items[i].mAntiMagic = id->mPlus[i].mAntiMagic;
                not.items[i].mAntiPoison = id->mPlus[i].mAntiPoison;

                not.items[i].mHpRecover = id->mPlus[i].mHpRecover;
                not.items[i].mMpRecover = id->mPlus[i].mMpRecover;
                not.items[i].mPoisonRecover = id->mPlus[i].mPoisonRecover;

                not.items[i].mMabiProb = id->mPlus[i].mMabi_prob;
                not.items[i].mMabiDura = id->mPlus[i].mMabi_dura;
                not.items[i].mDixiaoPres = id->mPlus[i].mDixiao_pres;
                not.items[i].mFuyuanCd = id->mPlus[i].mFuyuan_cd;
                not.items[i].mFuyuanPres = id->mPlus[i].mFuyuan_pres;

                not.items[i].mMaxHp = id->mPlus[i].mMax_hp;
                not.items[i].mMaxMp = id->mPlus[i].mMax_mp;
                not.items[i].mMaxHpPres = id->mPlus[i].mMax_hp_pres;
                not.items[i].mMaxMpPres = id->mPlus[i].mMax_mp_pres;

                not.items[i].mHalfBaoji = id->mPlus[i].mHalfBaoji;
            }
            SendMsg(not);
        }

        if (!id->mPlusZl.empty()) {
            GS_Client::ItemPlusDespNotifyGroup not;
            not.items.resize(id->mPlusZl.size());
            for (int i = 0; i < id->mPlusZl.size(); ++i) {
                not.items[i].mItemPlusID = id->mPlusZl[i].mItemPlusID;
                not.items[i].mTypeID = id->mPlusZl[i].mTypeID;

                not.items[i].mNeedType = id->mPlusZl[i].mNeedType;
                not.items[i].mNeedParam1 = id->mPlusZl[i].mNeedParam1;
                not.items[i].mNeedParam2 = id->mPlusZl[i].mNeedParam2;
                not.items[i].mNeedParam3 = id->mPlusZl[i].mNeedParam3;
                not.items[i].mNeedParam4 = id->mPlusZl[i].mNeedParam4;
                not.items[i].mNeedParam5 = id->mPlusZl[i].mNeedParam5;

                not.items[i].mAC = id->mPlusZl[i].mAC;
                not.items[i].mACMax = id->mPlusZl[i].mACMax;
                not.items[i].mMAC = id->mPlusZl[i].mMAC;
                not.items[i].mMACMax = id->mPlusZl[i].mMACMax;
                not.items[i].mDC = id->mPlusZl[i].mDC;
                not.items[i].mDCMax = id->mPlusZl[i].mDCMax;
                not.items[i].mMC = id->mPlusZl[i].mMC;
                not.items[i].mMCMax = id->mPlusZl[i].mMCMax;
                not.items[i].mSC = id->mPlusZl[i].mSC;
                not.items[i].mSCMax = id->mPlusZl[i].mSCMax;

                not.items[i].mLuck = id->mPlusZl[i].mLuck;
                not.items[i].mCurse = id->mPlusZl[i].mCurse;
                not.items[i].mAccuracy = id->mPlusZl[i].mAccuracy;
                not.items[i].mDodge = id->mPlusZl[i].mDodge;
                not.items[i].mAntiMagic = id->mPlusZl[i].mAntiMagic;
                not.items[i].mAntiPoison = id->mPlusZl[i].mAntiPoison;

                not.items[i].mHpRecover = id->mPlusZl[i].mHpRecover;
                not.items[i].mMpRecover = id->mPlusZl[i].mMpRecover;
                not.items[i].mPoisonRecover = id->mPlusZl[i].mPoisonRecover;

                not.items[i].mMabiProb = id->mPlusZl[i].mMabi_prob;
                not.items[i].mMabiDura = id->mPlusZl[i].mMabi_dura;
                not.items[i].mDixiaoPres = id->mPlusZl[i].mDixiao_pres;
                not.items[i].mFuyuanCd = id->mPlusZl[i].mFuyuan_cd;
                not.items[i].mFuyuanPres = id->mPlusZl[i].mFuyuan_pres;

                not.items[i].mMaxHp = id->mPlusZl[i].mMax_hp;
                not.items[i].mMaxMp = id->mPlusZl[i].mMax_mp;
                not.items[i].mMaxHpPres = id->mPlusZl[i].mMax_hp_pres;
                not.items[i].mMaxMpPres = id->mPlusZl[i].mMax_mp_pres;

                not.items[i].mHalfBaoji = id->mPlusZl[i].mHalfBaoji;
            }
            SendMsg(not);
        }
	}
}
void CCliNetSession::notifySkillDefine(OGSLib::GameSession* session,int skill_type, int skill_level)
{
	if( session != mGameSession ) return;
	GS_Client::SkillDespNotify notify;
	OGSLib::SkillDef* sd = session->GetGameServerLib()->GetSkillDefineManager()->GetSkillDefine(skill_type, skill_level);
	if( sd )
	{
		notify.skill_id = sd->TypeID;
		strncpy_s(notify.mName,sd->mName.c_str(),511);notify.mName[511]=0;
		strncpy_s(notify.mDesp,sd->mDesp.c_str(),511);notify.mDesp[511]=0;
		notify.icon_id = sd->IconID;
		notify.skill_level = sd->Level;
		notify.short_cut = sd->ShortCut;
		notify.sound = sd->Sound;
		notify.effect_type = sd->EffectType;
		notify.effect_resID = sd->Effect_ResID;
		notify.levelmax = sd->LevelMax;
		notify.range = sd->Range;
		notify.min_dis = sd->CastDistanceMin;
		notify.max_dis = sd->CastDistanceMax;
		/*notify.lv1_train = sd->L1Train;
		notify.lv1_need = sd->NeedL1;
		notify.lv2_train = sd->L2Train;
		notify.lv2_need = sd->NeedL2;
		notify.lv3_train = sd->L3Train;
		notify.lv3_need = sd->NeedL3;*/
		notify.spell_def = sd->DefSpell;
		notify.spell = sd->Spell;
		notify.consume_mp = sd->ConsumeMp;

		notify.isShow = sd->IsShow;
		notify.cast_way = sd->CastWay;
		notify.select_target_way = sd->SelectTargetWay;
		notify.skill_cd = sd->SkillCD;
		notify.public_cd = sd->PublicCD;
		notify.order_id = sd->OrderID;
		notify.need_level = sd->NeedLevel;
		notify.need_exp = sd->UpgradeNeedExp;
		notify.damageEffect = sd->DamageEffect;

		strncpy_s(notify.damage_desp,sd->DamageDesp.c_str(),511);notify.damage_desp[511]=0;
		strncpy_s(notify.range_desp,sd->RangeDesp.c_str(),511);notify.range_desp[511]=0;
		strncpy_s(notify.cd_desp,sd->CDDesp.c_str(),511);notify.cd_desp[511]=0;
		strncpy_s(notify.exteffect_desp,sd->ExtraEffectDesp.c_str(),511);notify.exteffect_desp[511]=0;

		if (sd->LevelMax > skill_level)
		{
			OGSLib::SkillDef* sd_next = session->GetGameServerLib()->GetSkillDefineManager()->GetSkillDefine(skill_type, skill_level + 1);
			if( sd_next )
			{
				strncpy_s(notify.damage_desp_next,sd_next->DamageDesp.c_str(),511);notify.damage_desp_next[511]=0;
				strncpy_s(notify.range_desp_next,sd_next->RangeDesp.c_str(),511);notify.range_desp_next[511]=0;
				strncpy_s(notify.cd_desp_next,sd_next->CDDesp.c_str(),511);notify.cd_desp_next[511]=0;
				strncpy_s(notify.exteffect_desp_next,sd_next->ExtraEffectDesp.c_str(),511);notify.exteffect_desp_next[511]=0;
			}
		}

		SendMsg(notify);
	}
}

void CCliNetSession::notifyExpChange(OGSLib::GameSession* session,int change_value)
{
	if( session != mGameSession ) return;
	GS_Client::ExpChangeNotify notify;
	notify.exp = session->GetPlayer()->exp();
	notify.next_exp = session->GetPlayer()->exp_next();
	notify.exp_change = change_value;
	SendMsg(notify);
}
void CCliNetSession::notifyLevelChange(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::LevelChangeNotify notify;
	notify.level = session->GetPlayer()->level();
	SendMsg(notify);
}
//void CCliNetSession::notifyDefaultSkill(OGSLib::GameSession* session,int default_skill)
//{
//	GS_Client::DefaultSkillNotify notify;
//	notify.defaultskill = default_skill;
//	SendMsg(notify);
//}
void CCliNetSession::notifySessionEnd(OGSLib::GameSession* session)
{
}
void CCliNetSession::notifyMapEnter(OGSLib::GameSession* session,OGSLib::PlayMap* map)
{
	if( session != mGameSession ) return;
	GS_Client::MapEnterNotify notify;
	strncpy_s(notify.map_id,_countof(notify.map_id),map->GetID().c_str(),511);
	notify.map_id[511] = 0;
	notify.init_x = session->GetPlayer()->X();
	notify.init_y = session->GetPlayer()->Y();
	notify.init_dir = session->GetPlayer()->Dir();
	notify.minimap_id = map->GetMiniMapID();
	notify.weather = map->WanderHangup();
	strncpy_s(notify.map_file,_countof(notify.map_file),map->GetMapFile().c_str(),511);
	notify.map_file[511]=0;
	strncpy_s(notify.map_name,_countof(notify.map_name),map->GetName().c_str(),511);
	notify.map_name[511]=0;
	notify.map_flags = map->MapFlags();
	notify.nointeract=map->NoInteract();
	SendMsg(notify);
	{
		const OGSLib::PlayMap::MapSafeAreaVector* sa = map->GetSafeArea();
		if( sa )
		{
			for( OGSLib::PlayMap::MapSafeAreaVector::const_iterator p = sa->begin();
				p != sa->end();
				++ p )
			{
				GS_Client::MapSafeAreaNotify notify;
				strncpy_s(notify.map_id ,_countof(notify.map_id), p->map_id.c_str(),511);
				notify.map_id[511]=0;
				notify.x = p->x;
				notify.y = p->y;
				notify.size = p->size;
				SendMsg(notify);
			}
		}
	}
	{
		const OGSLib::PlayMap::MapConnVector* con = map->GetConn();
		if( con )
		{
			for( OGSLib::PlayMap::MapConnVector::const_iterator p = con->begin();
				p != con->end();
				++ p )
			{
				OGSLib::DirectFlyNode* node = (*p);
				if(node)
				{
					GS_Client::MapConnNotify notify;
					strncpy_s(notify.map_id ,_countof(notify.map_id), node->mStartMapID.c_str(),511);
					notify.map_id[511]=0;
					notify.from_x = node->mStartX;
					notify.from_y = node->mStartY;
					strncpy_s(notify.des_map_id,_countof(notify.des_map_id),node->mTargetMapID.c_str(),511);
					notify.des_map_id[511]=0;
					notify.to_x = node->mTargetX;
					notify.to_y = node->mTargetY;
					notify.size = 1;
					strncpy_s(notify.des_map_name ,_countof(notify.des_map_id), node->mNodeName.c_str(),511);
					notify.des_map_name[511]=0;
					SendMsg(notify);
				}
			}
		}
	}
	{
		if(map->FubenType() != OGSLib::MAP_TYPE::MAP_PERSONAL)
		{
			notifyMiniNpc(mGameSession,map);
			notifyMapMonGen(mGameSession,map);
		}
	}
	{
		notifyMapOption(mGameSession,map);
	}
}
void CCliNetSession::notifyMiniNpc(OGSLib::GameSession* session,OGSLib::PlayMap* map)
{
	const OGSLib::PlayMap::MapMiniNpcVector* mn = map->GetMiniNpc();
	if( mn )
	{
		for( OGSLib::PlayMap::MapMiniNpcVector::const_iterator p = mn->begin();
			p != mn->end();
			++ p )
		{
			GS_Client::MapMiniNpcNotify notify;
			strncpy_s(notify.map_id ,_countof(notify.map_id), map->GetID().c_str(),511);
			notify.map_id[511]=0;
			strncpy_s(notify.npc_id ,_countof(notify.npc_id), p->npc_name.c_str(),511);
			notify.npc_id[511]=0;
			strncpy_s(notify.short_id ,_countof(notify.short_id), p->short_name.c_str(),511);
			notify.short_id[511]=0;
			notify.x = p->x;
			notify.y = p->y;
			notify.num = map->IsNpc(notify.npc_id)?1:map->NumberMonster(notify.short_id);
			notify.directflyID = p->npc_directfly_ID;
			notify.showNpcFlag = p->showNpcFlag;
			SendMsg(notify);
		}
	}
}
void CCliNetSession::notifyMapMonGen(OGSLib::GameSession* session,OGSLib::PlayMap* map)
{
	const OGSLib::PlayMap::MapMiniMonGenVector* mongenvec = map->GetMiniMonGen();
	if( mongenvec )
	{
		GS_Client::MapMonGenNotify notify;
		strncpy_s(notify.map_id ,_countof(notify.map_id), map->GetID().c_str(),511);
		notify.map_id[511]=0;
		for( OGSLib::PlayMap::MapMiniMonGenVector::const_iterator p = mongenvec->begin();
			p != mongenvec->end();
			++ p )
		{
			GS_Client::MapMonGenNotify::GenNode gn;
			strncpy_s(gn.name ,_countof(gn.name), p->name.c_str(),63);
			gn.name[63]=0;
			gn.x = p->x;
			gn.y = p->y;
			gn.time = p->time;
			notify.mGenList.push_back(gn);
		}
		SendMsg(notify);
	}
}
void CCliNetSession::notifyMeetItem(OGSLib::GameSession* session, OGSLib::PlayMapItem* item)
{
	GS_Client::MapItemOwnerNotify notify;
	notify.id = item->ID();
	notify.owner = item->OwnerID();
	notify.item_type = item->item_type();
	SendMsg(notify);
}
void CCliNetSession::notifyMapMeet(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src,int flags)
{
	if( session != mGameSession ) return;
	GS_Client::MapMeetNotify notify;
	notify.id = src->ID();
	notify.type = src->Type();
	//notify.name = src->Name();

	std::string name = src->Name();
	if( src->TypeIsPlayer() )
	{
		if(mGameSession->GetPlayer()->GetMap())
		{
			if(mGameSession->GetPlayer()->GetMap()->FightAnonym())
			{
				name = mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_ANONYM_NAME;
			}
		}
	}

	if (src->TypeIsSlave())
	{
		if(mGameSession->GetPlayer()->GetMap())
		{
			OGSLib::PlaySlave * slave = static_cast<OGSLib::PlaySlave *>(src);

			if (slave->mNameShowOwner)
			{
				if(mGameSession->GetPlayer()->GetMap()->FightAnonym() && mGameSession->GetPlayer() != slave->Owner())
				{
					name += "(";
					name += mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_ANONYM_NAME;
					name += ")";
				}
				else
				{
					if (slave->Owner())
					{
						name += "(";
						name += slave->Owner()->Name();
						name += ")";
					}
				}
			}
		}
	}

	strncpy_s(notify.name,_countof(notify.name),name.c_str(),512);
	notify.name[511] = 0;

	if (src->TypeIsNPC())
	{
		OGSLib::PlayNpc* npc = (OGSLib::PlayNpc*)src;
		if (npc->mChangeName > 0 )
		{
			if (session->GetPlayer())
			{
				std::string new_name = session->GetGameServerLib()->GetScriptEngine()->PlayerGetName(session->GetPlayer(),notify.name,"player.getNpcName");
				if (new_name != "")
				{
					strncpy_s(notify.name,_countof(notify.name),new_name.c_str(),512);
					notify.name[511] = 0;
				}
			}
		}
	}
	notify.loverName[0] = 0;
	notify.cloth = src->Cloth();
	notify.weapon = src->Weapon();
	notify.hair = src->Hair();

	if( src->TypeIsPlayer() )
	{
		OGSLib::DomainPlay* play = (OGSLib::DomainPlay*)src;
		//if (play->CoupleID() > 0 &&  play->LoverName())
		//{
		//	strncpy_s(notify.loverName,_countof(notify.loverName),play->LoverName(),511);
		//	notify.loverName[511] = 0;
		//}
		notify.zsLevel = play->ZsLevel();
		notify.maxpower = play->FightPoint();
		notify.teamid = play->TeamID();
		if(play->mon_show() >= 1 && play->mon_showName() != "")
		{
			notify.mount = 0;
			notify.fbresid = 0;
			notify.fashion = 0;
			notify.ismon = 1;
			if (play->GetModel(10) == 1)
			{
				notify.fashion = 0;
			}	
			notify.cloth = play->mon_showResid();
			notify.weapon = 0;
			notify.hair = 0;

		}
		else
		{
			if(play->show_mount() == 1)
			{
				notify.mount = play->mount();
			}
			notify.fbresid = play->FaBaoRes();
			notify.fashion = play->Fashion();
			if (play->GetModel(10) == 1)
			{
				notify.fashion = 0;
			}
			if (notify.fashion > 0)
			{
				notify.cloth=notify.fashion;
			}
			// notify.cloth = src->Cloth();
		}
	}

	notify.x = src->X();
	notify.y = src->Y();
	notify.dir = src->Dir();
	notify.hp = src->Hp();
	notify.maxhp = src->MaxHp();
	notify.flags = flags;
	if( src->TypeIsMonster() )
	{
		OGSLib::PlayMonster* mon = (OGSLib::PlayMonster*)src;
		if( mon )
		{
			notify.flags = mon->getShowState();
		}
		int hitter_id = 0;
		std::string hitter_name;
		mon->GetExpHitter(hitter_id, hitter_name);
		if (hitter_id > 0 && !mon->IsNoShowHitter())
		{
			notifyMonExpHiterChange(session, mon->ID(), hitter_id, hitter_name.c_str());
		}
	}
	//if(src->TypeIsRobot())
	//{
	//	OGSLib::PlayRobot* robot = (OGSLib::PlayRobot*)src;
	//	notify.type=robot->GetPlayerType();
	//	notify.robotstate=1;
	//	if(this->mGameSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(robot->Cloth()))
	//	{
	//		notify.cloth=this->mGameSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(robot->Cloth())->mResourceID;
	//	}
	//	else
	//	{
	//		if(robot->Gender()==200){notify.cloth=DEFAULT_CLOTH_MAN;}
	//		if(robot->Gender()==201){notify.cloth=DEFAULT_CLOTH_FAMALE;}
	//	}
	//	if(this->mGameSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(robot->Weapon()))
	//	{
	//		notify.weapon=this->mGameSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine(robot->Weapon())->mResourceID;
	//	}
	//	SendMsg(notify);
	//	if(!robot->GuildName().empty())
	//	{
	//		GS_Client::GhostGuildInfo ggi;
	//		ggi.srcid = robot->ID();
	//		strncpy_s(ggi.guild_name,_countof(ggi.guild_name),robot->GuildName().c_str(),511);
	//		ggi.guild_name[511]=0;
	//		ggi.title = robot->GuildTitle();
	//		SendMsg(ggi);
	//	}
	//	GS_Client::PlayerAddInfoNotify notify;
	//	notify.id=robot->ID();
	//	notify.gender=robot->Gender();
	//	notify.job=robot->Job();
	//	notify.lv=robot->level();
	//	notify.hp=robot->Hp();
	//	notify.maxhp=robot->MaxHp();
	//	SendMsg(notify);
	//	return;
	//}
	/*else*/ if( src->TypeIsPlayer() )
	{
		OGSLib::DomainPlay* player = (OGSLib::DomainPlay*)src;
		if(player)
		{
			if(player->GetModel(5)>0)
			{
				if (!(mGameSession->GetPlayer()->GetMap()&&mGameSession->GetPlayer()->GetMap()->FightAnonym()))
				{
					notify.viplevel=player->GetModel(5);
				}
			}
		}
	}
	SendMsg(notify);
	if( src->TypeIsPlayer() )
	{
		OGSLib::DomainPlay* player = (OGSLib::DomainPlay*)src;
		if( player && (!player->GuildName().empty()))
		{
			GS_Client::GhostGuildInfo ggi;
			ggi.srcid = player->ID();
			if (!(mGameSession->GetPlayer()->GetMap()&&mGameSession->GetPlayer()->GetMap()->FightAnonym()))
			{
				strncpy_s(ggi.guild_name,_countof(ggi.guild_name),player->GuildName().c_str(),511);
				ggi.guild_name[511]=0;
				ggi.title = player->GuildTitle();
			}
			SendMsg(ggi);
		}
		if( player && player->GroupID() > 0 )
		{
			GS_Client::GroupInfoNotify gi;
			gi.src_id = player->ID();
			gi.group_id = player->GroupID();
			OGSLib::Group* g = this->mGameSession->GetGameServerLib()->GetGroupManager()->FindGroup(gi.group_id);
			if( g )
			{
				strncpy_s(gi.group_leader,_countof(gi.group_leader),g->GetLeader().c_str(),511);
				gi.group_leader[511]=0;
				strncpy_s(gi.group_name,_countof(gi.group_name),g->GetName().c_str(),511);
				gi.group_name[511]=0;
				SendMsg(gi);
			}
		}
		{
			notifyPKStateChange(session,player);
		}
		{
			notifyPlayerAddInfo(session,player);
		}
		{
			notifyTeamInfo(session,player);
		}
		{
			notifyNameAdd(session,player,player->GetNamePre(),player->GetNamePro());
		}
	}
	if( src->TypeIsMapItem() )
	{
		OGSLib::PlayMapItem* mapitem = (OGSLib::PlayMapItem*)src;
		if( mapitem )
		{
			GS_Client::MapItemOwnerNotify notify;
			notify.id = src->ID();
			notify.owner = mapitem->OwnerID();
			if(session->GetPlayer()->IsGroupMember(mapitem->OwnerID()))
			{
				notify.owner=session->GetPlayer()->ID();
			}
			notify.item_type = mapitem->item_type();
			SendMsg(notify);
		}
	}
	if( src->TypeIsMonster() )
	{
		
		OGSLib::PlayMonster* mon = (OGSLib::PlayMonster*)src;
		if( mon )
		{
			GS_Client::MonsterAddInfo notify;
			notify.src_id = mon->ID();
			notify.lv = mon->level();
			notify.collect_time = mon->CollectTime();
			notify.state = mon->State();
			notify.disapear_time = mon->DisaperTime();
			notify.isboss = mon->IsBoss();
			if(mon->IsElite())
			{
				notify.isboss = 2;
			}
			//if (mon->Target() && mon->Target()->TypeIsPlayer() )
			//{
			//	OGSLib::DomainPlay* play = (OGSLib::DomainPlay*)mon->Target();
			//	strncpy_s(notify.owner ,_countof(notify.owner), play->Name(),511);
			//	notify.owner[511]=0;
			//}
			SendMsg(notify);
		}
	}
	if( src->TypeIsSlave() || src->TypeIsDart())
	{
		OGSLib::PlaySlave* slave = (OGSLib::PlaySlave*)src;
		if( slave )
		{
			if(slave->mOwner)
			{
				GS_Client::MonsterAddInfo notify;
				notify.src_id = slave->ID();
				notify.lv = slave->level();
				notify.state = slave->State();
				notify.owner = slave->mOwner->ID();
				SendMsg(notify);
			}
		}
	}
	if( src->TypeIsNeutral() )
	{
		OGSLib::PlayNeutral* neutral  = (OGSLib::PlayNeutral*)src;
		if( neutral )
		{
			if( neutral->GuildName()[0] != 0 )
			{
				notifyNameAdd(session,src,neutral->GuildName(),"");
			}

			GS_Client::MonsterAddInfo notify;
			notify.src_id = neutral->ID();
			notify.collect_time = neutral->GetDisappearTime();
			SendMsg(notify);


		}
	}
	if( src->IsInteractive() )
	{
		OGSLib::PlayActiveObject* ac = (OGSLib::PlayActiveObject*)src;
		if( ac )
		{
			GS_Client::ListBuffNotify notify;
			OGSLib::BuffList& list = ac->GetBuffList(); 
			for (OGSLib::BuffList::iterator it = list.begin(); it != list.end(); ++ it)
			{
				GS_Client::ListBuffNotify::NetBuff net_buff;
				net_buff.id = it->buff_info.mBuffID;
				net_buff.time_left = it->buff_end_value;
 				notify.buff_list.push_back(net_buff);
			}
			notify.src_id = ac->ID();

			if (notify.buff_list.size() > 0)
			{
				SendMsg(notify);
			}
// 			GS_Client::ListStatusNotify lnotify;
// 			for( int i = 0;i < OGSLib::PlayActiveObject::STATUS_TYPE_NUMBER;i++)
// 			{
// 				if( ac->IsStatus(i) )
// 				{
// 					lnotify.num++;
// 				}
// 			}
// 			for( int i = 0;i < OGSLib::PlayActiveObject::STATUS_TYPE_NUMBER;i++)
// 			{
// 				if( ac->IsStatus(i) )
// 				{
// 					//GS_Client::StatusChangeNotify notify;
// 					GS_Client::ListStatusNotify::NetStatus status;
// 					lnotify.src_id = ac->ID();
// 					status.id = i;
// 					status.dura = static_cast<int>(ac->GetStatusDuration(i));
// 					status.param = ac->StatusParam(i);
// 					status.starttime = static_cast<float>(ac->StatusTickGap(i));
// 					if (lnotify.m_StatusTemp.find(i)==lnotify.m_StatusTemp.end())
// 					{
// 						lnotify.m_StatusTemp[i] = status;
// 					}
// 					//SendMsg(notify);
// 					//lnotify.NetStatus = notify;
// 				}
// 			}
// 			if (lnotify.num > 0)
// 			{
// 				SendMsg(lnotify);
// 			}
 		}
	}
}
void CCliNetSession::notifyPlayerAddInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* player)
{
	if( session != mGameSession ) return;
	GS_Client::PlayerAddInfoNotify notify;
	notify.id = player->ID();
	notify.lv = player->level();
	notify.job = player->job();
	notify.gender = player->gender();
	notify.maxhp = player->MaxHp();
	notify.hp = player->Hp();
	notify.maxmp = player->MaxMp();
	notify.mp = player->Mp();
	notify.low = player->GetUpGradeMinLv();
	SendMsg(notify);
}
void CCliNetSession::notifyPlayEffect(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int skill_type,int x,int y,unsigned int targetID,int ownerID,int resID)
{
	if( session != mGameSession ) return;
	GS_Client::PlayEffectNotify notify;
	notify.src_id = src->ID();
	
	notify.effect_id = skill_type;

	if (src->TypeIsPlayer())
	{
		OGSLib::DomainPlay* play = (OGSLib::DomainPlay*)src;
		//���Խ�ɫ����Чid
		if(play && play->gender() == 201)
		{
			if (notify.effect_id == 10100)
			{
				notify.effect_id = 10110;
			}
			else if (notify.effect_id == 10120)
			{
				notify.effect_id = 10130;
			}
			else if (notify.effect_id == 10200)
			{
				notify.effect_id = 10210;
			}
			else if (notify.effect_id == 10220)
			{
				notify.effect_id = 10230;
			}
			else if (notify.effect_id == 10310)
			{
				notify.effect_id = 10320;
			}
		}
	}

	notify.x = x;
	notify.y = y;
	notify.target_id = targetID;
	notify.owner_id = ownerID;
	notify.resID = resID;
	SendMsg(notify);
}
void CCliNetSession::notifySessionClosed(OGSLib::GameSession* session,const std::string& msg)
{
	if( session != mGameSession ) return;
	GS_Client::SessionClosedNotify notify;
	strncpy_s(notify.msg ,_countof(notify.msg), msg.c_str(),511);
	notify.msg[511]=0;
	SendMsg(notify);
	NetBreak();
}

void CCliNetSession::notifyGroupInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* player,const std::string& group_name,const std::string& group_leader)
{
	if( session != mGameSession ) return;
	GS_Client::GroupInfoNotify gi;
	gi.src_id = player->ID();
	gi.group_id = player->GroupID();
	strncpy_s(gi.group_name,_countof(gi.group_name),group_name.c_str(),511);
	gi.group_name[511] = 0;
	strncpy_s(gi.group_leader,_countof(gi.group_leader),group_leader.c_str(),511);
	gi.group_leader[511] = 0;
	SendMsg(gi);
}
void CCliNetSession::notifyJoinGroupToLeader(OGSLib::GameSession* session,const std::string& player_name)
{
	if( session != mGameSession ) return;
	GS_Client::JoinGroupToLeaderNotify notify;
	strncpy_s(notify.player_nick_name ,_countof(notify.player_nick_name), player_name.c_str(),511);
	notify.player_nick_name[511]=0;
	SendMsg(notify);
}
void CCliNetSession::notifyInviteGroupToMember(OGSLib::GameSession* session,const std::string& leader_name,int group_id)
{
	if( session != mGameSession ) return;
	GS_Client::InviteGroupToMemberNotify notify;
	strncpy_s(notify.leader_name ,_countof(notify.leader_name), leader_name.c_str(),511);
	notify.leader_name[511]=0;
	notify.group_id= group_id;
	SendMsg(notify);
}
void CCliNetSession::notifyFindRoadGoto(OGSLib::GameSession* session,const char* map,int x,int y,const char* target,int flags)
{
	if( session != mGameSession ) return;
	GS_Client::FindRoadGotoNotify notify;
	strncpy_s(notify.map,_countof(notify.map),map,511);
	notify.map[511]=0;
	notify.x= x;
	notify.y = y;
	strncpy_s(notify.target,_countof(notify.target),target,511);
	notify.target[511]=0;
	notify.flags = flags;
	SendMsg(notify);
}
void CCliNetSession::notifyTradeInvite(OGSLib::GameSession* session,OGSLib::DomainPlay* player)
{
	if( session != mGameSession ) return;
	GS_Client::TradeInviteNotify notify;
	strncpy_s(notify.inviter ,_countof(notify.inviter), player->Name(),511);
	notify.inviter[511]=0;
	SendMsg(notify);
}
void CCliNetSession::notifyTradeInfo(OGSLib::GameSession* session,int gamemoney,int vcoin,bool submit,const std::string& des_name,int des_level,int des_gamemoney,int des_vcoin,bool des_submit,int trade_result)
{
	GS_Client::TradeInfoNotify notify;
	notify.gamemoney = gamemoney;
	notify.vcoin = vcoin;
	notify.submit = submit?1:0;
	strncpy_s(notify.des_name ,_countof(notify.des_name), des_name.c_str(),511);
	//_snprintf_s(notify.des_name,511,"%s(%d)",des_name.c_str(),des_level);
	notify.des_name[511]=0;
	notify.des_gamemoney = des_gamemoney;
	notify.des_vcoin = des_vcoin;
	notify.des_submit = des_submit?1:0;
	notify.des_level = des_level;
	notify.trade_result = trade_result;
	SendMsg(notify);
}
void CCliNetSession::notifyTradeItemChange(OGSLib::GameSession* session,int side,OGSLib::SubItem item,int show_flags)
{
	if( session != mGameSession ) return;
	GS_Client::TradeItemChangeNotify notify;
	notify.side = side;
	notify.position = item.mPosition;
	notify.type_id = item.mTypeID;
	notify.duration_max = item.mDuraMax;
	notify.duration = item.mDuration;
	notify.item_flags = item.mItemFlags;
	notify.show_flags = show_flags;
	notify.level = item.mLevel;
	notify.upd_count=item.mNumber;
	notify.add_ac = item.mAddAC;
	notify.add_mac = item.mAddMAC;
	notify.add_dc = item.mAddDC;
	notify.add_mc = item.mAddMC;
	notify.add_sc = item.mAddSC;
	notify.upd_ac = item.mUpdAC;
	notify.upd_mac = item.mUpdMAC;
	notify.upd_dc = item.mUpdDC;
	notify.upd_mc = item.mUpdMC;
	notify.upd_sc = item.mUpdSC;
	notify.luck = item.mLuck;
	notify.protect = item.mProtect;
	notify.add_hp = item.mAddHp;
	notify.add_mp = item.mAddMp;
	notify.create_time=item.mCreateTime;
	notify.sellprice = item.mSellPrice;
	notify.zlevel = item.mZLevel;
	notify.lock = item.mLock;
	SendMsg(notify);
}
void CCliNetSession::notifyPKStateChange(OGSLib::GameSession* session,OGSLib::DomainPlay* player)
{
	if( session != mGameSession ) return;
	GS_Client::PKStateNotify notify;
	notify.src_id = player->ID();
	notify.pk_state = player->PKState();
	notify.pk_value = player->PKValue();
	SendMsg(notify);
}

void CCliNetSession::notifyBuffDesp(OGSLib::GameSession* session,int id)
{
	if( session != mGameSession ) return;
	OGSLib::BuffInfo * buff = mGameSession->GetGameServerLib()->GetStatusManager()->GetBuffInfo(id);
	if (!buff)
	{
		return;
	}
	GS_Client::BuffDespNotify notify;
	notify.id = id;
	notify.name = buff->mName;
	notify.icon = buff->mIcon;
	notify.ui = buff->mUI + buff->mValidType * 10;
	notify.uisort = buff->mUISort;
	notify.type = buff->mBuffType;
	notify.lv = buff->mLv;
	notify.desp = buff->mDesc;
	notify.eff_type = buff->mEffectType;
	notify.eff_id = buff->mEffectID;
	notify.validTimeMax = buff->mValidMaxValue;
	SendMsg(notify);
}

void CCliNetSession::notifyBuffChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src, int id, int op_code, double time_left)
{
	if( session != mGameSession ) return;
	GS_Client::BuffChangeNotify notify;
	notify.src_id = src->ID();
	notify.id = id;
	notify.op_code = op_code;
	notify.time_left = time_left;
	SendMsg(notify);
}

void CCliNetSession::notifyStatusChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int status_id)
{
	if( session != mGameSession ) return;
	if( status_id>=0 && status_id<OGSLib::PlayActiveObject::STATUS_TYPE_NUMBER )
	{
		GS_Client::StatusChangeNotify notify;
		notify.src_id = src->ID();
		notify.status_id = status_id;
		notify.status_param = src->StatusParam(status_id);
		notify.status_time = src->GetStatusDuration(status_id);
		notify.gap = static_cast<int>(src->StatusTickGap(status_id));
		SendMsg(notify);
	}
}
void CCliNetSession::notifyStatusDesp(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int status_id,int begin,int end)
{
	if( session != mGameSession ) return;
	OGSLib::StatusInfo* sif = mGameSession->GetGameServerLib()->GetStatusManager()->GetBaseStatusInfo(status_id);
	if( sif )
	{
		GS_Client::StatusDespNotify notify;
		notify.id = status_id;

		for( OGSLib::ManagerStatus::StatusInfoMap::iterator pos = mGameSession->GetGameServerLib()->GetStatusManager()->GetStatusMap().begin();
			pos != mGameSession->GetGameServerLib()->GetStatusManager()->GetStatusMap().end();
			++ pos )
		{
			int id = pos->first / 1000;
			if( id == status_id )///��������
			{
				if (begin > 0)
				{
					if (pos->second.mLv >= begin && pos->second.mLv <= end)
					{
						GS_Client::StatusDespNotify::StatusNode sn;
						sn.desp = pos->second.mName;
						sn.lv = pos->second.mLv;
						sn.icon = pos->second.mIconID;
						sn.ac = pos->second.mAC;
						sn.acmax = pos->second.mACMax;
						sn.mac = pos->second.mMAC;
						sn.macmax = pos->second.mMACMax;
						sn.dc = pos->second.mDC;
						sn.dcmax = pos->second.mDCMax;
						sn.mc = pos->second.mMC;
						sn.mcmax = pos->second.mMCMax;
						sn.sc = pos->second.mSC;
						sn.scmax = pos->second.mSCMax;
						sn.hpmax = pos->second.mMaxHp;
						sn.mpmax = pos->second.mMaxMp;
						sn.nodef = pos->second.mNoDef;
						sn.fightpoint = pos->second.mFightPoint;
						sn.baoji = pos->second.mBaojiProb;
						sn.baoprob = pos->second.mBaijiPres;
						notify.mStatusList.push_back(sn);
					}
				}
				else///ȫ������
				{
					GS_Client::StatusDespNotify::StatusNode sn;
					sn.desp = pos->second.mName;
					sn.lv = pos->second.mLv;
					sn.icon = pos->second.mIconID;
					sn.ac = pos->second.mAC;
					sn.acmax = pos->second.mACMax;
					sn.mac = pos->second.mMAC;
					sn.macmax = pos->second.mMACMax;
					sn.dc = pos->second.mDC;
					sn.dcmax = pos->second.mDCMax;
					sn.mc = pos->second.mMC;
					sn.mcmax = pos->second.mMCMax;
					sn.sc = pos->second.mSC;
					sn.scmax = pos->second.mSCMax;
					sn.hpmax = pos->second.mMaxHp;
					sn.mpmax = pos->second.mMaxMp;
					sn.nodef = pos->second.mNoDef;
					sn.fightpoint = pos->second.mFightPoint;
					sn.baoji = pos->second.mBaojiProb;
					sn.baoprob = pos->second.mBaijiPres;
					notify.mStatusList.push_back(sn);
				}
			}
		}
		SendMsg(notify);
	}
}
void CCliNetSession::notifyCountDown(OGSLib::GameSession* session,int delay,std::string& msg)
{
	if( session != mGameSession ) return;
	GS_Client::CountDownNotify notify;
	notify.delay = delay;
	notify.msg = msg;
	SendMsg(notify);
}
void CCliNetSession::notifyMapLevae(OGSLib::GameSession* session,OGSLib::PlayMap* map)
{
	if( session != mGameSession ) return;
	GS_Client::MapLeaveNotify notify;
	SendMsg(notify);
}
void CCliNetSession::notifyMapBye(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src)
{
	if( session != mGameSession ) return;
	GS_Client::MapByeNotify notify;
	notify.id = src->ID();
	SendMsg(notify);
}

void CCliNetSession::notifyWalk(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir,int x,int y)
{
	if( session != mGameSession ) return;
	GS_Client::WalkNotify notify;
	notify.srcid = src->ID();
	notify.dir = dir;
	notify.newx = x;
	notify.newy = y;
	SendMsg(notify);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::WalkNotify::MSG_ID<<src->ID()<<dir<<x<<y;
	//SendStream(s);
}
void CCliNetSession::notifyDoAction(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int action)
{
	if( session != mGameSession ) return;
	GS_Client::DoActionNotify not;
	not.src_id = src->ID();
	not.action = action;
	SendMsg(not);
}
void CCliNetSession::notifySpeed(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,float speed)
{
	if( session != mGameSession ) return;
	GS_Client::SpeedNotify not;
	not.src_id = src->ID();
	not.speed = speed;
	SendMsg(not);
}
void CCliNetSession::notifyRun(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir,int x,int y)
{
	if( session != mGameSession ) return;
	GS_Client::RunNotify notify;
	notify.srcid = src->ID();
	notify.dir = dir;
	notify.newx = x;
	notify.newy = y;
	SendMsg(notify);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::RunNotify::MSG_ID<<src->ID()<<dir<<x<<y;
	//SendStream(s);
}
void CCliNetSession::resWalk(OGSLib::GameSession* session,int dir,int x,int y,int tag)
{
	if( session != mGameSession ) return;
	GS_Client::WalkRes res;
	res.dir = dir;
	res.newx = x;
	res.newy = y;
	res.tag = tag;
	SendMsg(res);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::WalkRes::MSG_ID<<dir<<x<<y<<tag;
	//SendStream(s);
}
void CCliNetSession::resSkill(OGSLib::GameSession* session,int result,int skill_type)
{
	if( session != mGameSession ) return;
	GS_Client::UseSkillRes res;
	res.result = result;
	res.skill_type_id = skill_type;
	SendMsg(res);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::UseSkillRes::MSG_ID<<result<<skill_type;
	//SendStream(s);
}
void CCliNetSession::resRun(OGSLib::GameSession* session,int dir,int x,int y,int tag)
{
	if( session != mGameSession ) return;
	GS_Client::RunRes res;
	res.dir = dir;
	res.newx = x;
	res.newy = y;
	res.tag = tag;
	SendMsg(res);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::RunRes::MSG_ID<<dir<<x<<y<<tag;
	//SendStream(s);
}

void CCliNetSession::notifyTurn(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir)
{
	if( session != mGameSession ) return;
	GS_Client::TurnNotify notify;
	notify.srcid = src->ID();
	notify.dir = dir;
	SendMsg(notify);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::TurnNotify::MSG_ID<<src->ID()<<dir;
	//SendStream(s);
}
void CCliNetSession::notifyAttack(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int param)
{
	if( session != mGameSession ) return;
	GS_Client::AttackNotify notify;
	notify.srcid = src->ID();
	notify.param = param;
	SendMsg(notify);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::AttackNotify::MSG_ID<<src->ID()<<param;
	//SendStream(s);
}
void CCliNetSession::notifyAvatarChange(OGSLib::GameSession* session,OGSLib::PlayMapObject* src)
{
	if( session != mGameSession ) return;
	GS_Client::AvatarChangeNotify notify;
	notify.srcid = src->ID();

	std::string name = src->Name();
	if( src->TypeIsPlayer() )
	{
		if(mGameSession->GetPlayer()->GetMap())
		{
			if(mGameSession->GetPlayer()->GetMap()->FightAnonym() && mGamePlayer!=src)
			{
				name = mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_ANONYM_NAME;
			}
		}
	}

	if (src->TypeIsSlave())
	{
		if(mGameSession->GetPlayer()->GetMap())
		{
			OGSLib::PlaySlave * slave = static_cast<OGSLib::PlaySlave *>(src);

			if(mGameSession->GetPlayer()->GetMap()->FightAnonym() && mGameSession->GetPlayer() != slave->Owner())
			{
				name += "(";
				name += mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_ANONYM_NAME;
				name += ")";
			}
			else
			{
				if (slave->Owner())
				{
					name += "(";
					name += slave->Owner()->Name();
					name += ")";
				}
			}
		}
	}

	strncpy_s(notify.name,_countof(notify.name),name.c_str(),512);
	notify.name[511] = 0;

	if (src->TypeIsPlayer())////�����ǰ�?����״̬�Ļ���
	{
		OGSLib::DomainPlay* play = (OGSLib::DomainPlay*)src;
		if(play->mon_show() >= 1 && play->mon_showName() != "")
		{
			notify.cloth = play->mon_showResid();
			notify.weapon = -1;
			notify.loverName[0] = 0;
			notify.wing = -1;
			//if (play->CoupleID() > 0 &&  play->LoverName())
			//{
			//	strncpy_s(notify.loverName,_countof(notify.loverName),play->LoverName(),511);
			//	notify.loverName[511] = 0;
			//}
			if(play->show_mount() == 1)
			{
				notify.mount = -1;
			}
			notify.fbresid = -1;
			notify.fashion = -1;
			notify.zsLevel = 0;
			notify.ismon = 1;
			if (play->GetModel(10) == 1)
			{
				notify.fashion = 0;
			}
			if (play->Fashion()>0)
			{
				notify.cloth = play->Fashion();
			}
			SendMsg(notify);
		}
		else
		{
			notify.cloth = src->Cloth();
			notify.weapon = src->Weapon();
			notify.loverName[0] = 0;
			notify.wing = src->Hair();
			if( src->TypeIsPlayer() )
			{
				OGSLib::DomainPlay* play = (OGSLib::DomainPlay*)src;
				//if (play->CoupleID() > 0 &&  play->LoverName())
				//{
				//	strncpy_s(notify.loverName,_countof(notify.loverName),play->LoverName(),511);
				//	notify.loverName[511] = 0;
				//}
				if(play->show_mount() == 1)
				{
					notify.mount = play->mount();
				}
				notify.fbresid = play->FaBaoRes();
				notify.fashion = play->Fashion();
				notify.zsLevel = play->ZsLevel();
				if (play->GetModel(10) == 1)
				{
					notify.fashion = 0;
				}
				if (play->Fashion()>0)
				{
					notify.cloth = play->Fashion();
				}
				notify.low = play->GetUpGradeMinLv();
			}
			SendMsg(notify);
		}
	}
	else
	{
		notify.cloth = src->Cloth();
		notify.weapon = src->Weapon();
		notify.loverName[0] = 0;
		notify.wing = src->Hair();
		/*if( src->TypeIsPlayer() )
		{
			OGSLib::DomainPlay* play = (OGSLib::DomainPlay*)src;
			if (play->CoupleID() > 0 &&  play->LoverName())
			{
				strncpy_s(notify.loverName,_countof(notify.loverName),play->LoverName(),511);
				notify.loverName[511] = 0;
			}
			if(play->show_mount() == 1)
			{
				notify.mount = play->mount();
			}
			notify.fbresid = play->FaBaoRes();
			notify.fashion = play->Fashion();
			notify.zsLevel = play->ZsLevel();
			if (play->GetModel(10) == 1)
			{
				notify.fashion = 0;
			}
		}*/
		SendMsg(notify);
	}
	
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::AvatarChangeNotify::MSG_ID<<src->ID()<<src->Cloth()<<src->Weapon();
	//SendStream(s);
}
void CCliNetSession::notifyInjury(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int attacker,int change,int delay,int effect,int change_power)
{
	if( session != mGameSession ) return;
	GS_Client::InjuryNotify notify;
	notify.srcid = src->ID();
	notify.newhp = src->Hp();
	notify.newpower = src->Power();
	notify.attacker = attacker;
	notify.change = change;
	notify.delay = delay;
	notify.show_flag = effect;
	notify.change_power = 0;
	SendMsg(notify);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::InjuryNotify::MSG_ID<<src->ID()<<src->Hp()<<change<<delay;
	//SendStream(s);
}
void CCliNetSession::notifyAttackMiss(OGSLib::GameSession* session,OGSLib::PlayMapObject* des)
{
	if( session != mGameSession) return;
	GS_Client::AttackMissNotify notify;
	notify.src_id = des->ID();
	SendMsg(notify);
}
void CCliNetSession::notifyDie(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int delay)
{
	if( session != mGameSession ) return;
	GS_Client::DieNotify notify;
	notify.srcid = src->ID();
	notify.delay = delay;
	SendMsg(notify);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::DieNotify::MSG_ID<<src->ID()<<delay;
	//SendStream(s);
}
void CCliNetSession::notifyHPMPChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int hp_change,int mp_change)
{
	if( session != mGameSession ) return;
	GS_Client::HPMPChangeNotify notify;
	notify.id = src->ID();
	notify.hp = src->Hp();
	notify.mp = src->Mp();
	notify.hpmax = src->MaxHp();
	notify.mpmax = src->MaxMp();
	notify.hpchange=hp_change;
	notify.mpchange=mp_change;
	SendMsg(notify);
	notifyTiliChange(session,src);
	//Stream_t* s = CreateStream();
	//(*s)<<GS_Client::HPMPChangeNotify::MSG_ID<<src->ID()<<src->Hp()<<src->Mp()<<hp_change<<mp_change;
	//SendStream(s);
}

void CCliNetSession::notifyWorldChat(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::WorldChatNotify notify;
	notify.srcid = src->ID();
	strncpy_s(notify.name,_countof(notify.name),src->Name(),511);
	notify.name[511] = 0;
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;

	if (src->TypeIsPlayer())
	{
		OGSLib::DomainPlay * player = static_cast<OGSLib::DomainPlay*>(src);
		const std::string& guild = player->GuildName();
		strncpy_s(notify.guild,_countof(notify.guild), guild.c_str(),511);
		notify.guild[511] = 0;
		notify.gender = player->gender();
		notify.lv = player->level();
		notify.job = player->job();
		notify.vip = player->GetModel(5);
	}

	SendMsg(notify);
}
void CCliNetSession::notifyHornChat(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::HornChatNotify notify;
	notify.srcid = src->ID();
	strncpy_s(notify.name,_countof(notify.name),src->Name(),511);
	notify.name[511] = 0;
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;

	if (src->TypeIsPlayer())
	{
		OGSLib::DomainPlay * player = static_cast<OGSLib::DomainPlay*>(src);
		const std::string& guild = player->GuildName();
		strncpy_s(notify.guild,_countof(notify.guild), guild.c_str(),511);
		notify.guild[511] = 0;
		notify.gender = player->gender();
		notify.lv = player->level();
		notify.job = player->job();
		notify.vip = player->GetModel(5);
	}
	SendMsg(notify);
}
void CCliNetSession::resHornChat(OGSLib::GameSession* session,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::HornChatRes res;
	strncpy_s(res.msg,_countof(res.msg),msg,511);
	res.msg[511] = 0;
	SendMsg(res);
}

void CCliNetSession::notifyMapChat(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::MapChatNotify notify;
	notify.srcid = src->ID();
	strncpy_s(notify.name,_countof(notify.name),src->Name(),511);
	notify.name[511] = 0;
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;

	if (src->TypeIsPlayer())
	{
		OGSLib::DomainPlay * player = static_cast<OGSLib::DomainPlay*>(src);
		const std::string& guild = player->GuildName();
 		strncpy_s(notify.guild,_countof(notify.guild), guild.c_str(),511);
 		notify.guild[511] = 0;
		notify.gender = player->gender();
		notify.lv = player->level();
		notify.job = player->job();
		notify.vip = player->GetModel(5);
 	}
	
	SendMsg(notify);
}

void CCliNetSession::notifyPrivateChat(OGSLib::GameSession* session,OGSLib::DomainPlay* src,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::PrivateChatNotify notify;
	notify.srcid = src->ID();
	strncpy_s(notify.name,_countof(notify.name),src->Name(),511);
	notify.name[511] = 0;
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;

	if (src->TypeIsPlayer())
	{
		OGSLib::DomainPlay * player = static_cast<OGSLib::DomainPlay*>(src);
		const std::string& guild = player->GuildName();
		strncpy_s(notify.guild,_countof(notify.guild), guild.c_str(),511);
		notify.guild[511] = 0;
		notify.gender = player->gender();
		notify.lv = player->level();
		notify.job = player->job();
		notify.vip = player->GetModel(5);
	}

	SendMsg(notify);
}
void CCliNetSession::notifyMonsterChat(OGSLib::GameSession* session,OGSLib::PlayMonster* src,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::MonsterChatNotify notify;
	notify.src_id = src->ID();
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;
	SendMsg(notify);
}
void CCliNetSession::notifyNormalChat(OGSLib::GameSession* session,OGSLib::DomainPlay* src,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::NormalChatNotify notify;
	strncpy_s(notify.name,_countof(notify.name),src->Name(),511);
	notify.name[511] = 0;
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;

	if (src->TypeIsPlayer())
	{
		OGSLib::DomainPlay * player = static_cast<OGSLib::DomainPlay*>(src);
		const std::string& guild = player->GuildName();
		strncpy_s(notify.guild,_countof(notify.guild), guild.c_str(),511);
		notify.guild[511] = 0;
		notify.gender = player->gender();
		notify.lv = player->level();
		notify.job = player->job();
		notify.vip = player->GetModel(5);
	}

	SendMsg(notify);
}

void CCliNetSession::notifyGuildChat(OGSLib::GameSession* session,OGSLib::Guild* gld,OGSLib::DomainGuild* src,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::GuildChatNotify notify;
	strncpy_s(notify.name,_countof(notify.name),src->Name(),511);
	notify.name[511] = 0;
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;

	OGSLib::DomainPlay * player = src->GetPlayer();
	if (player)
	{
		const std::string& guild = player->GuildName();
		strncpy_s(notify.guild,_countof(notify.guild), guild.c_str(),511);
		notify.guild[511] = 0;
		notify.gender = player->gender();
		notify.lv = player->level();
		notify.job = player->job();
		notify.vip = player->GetModel(5);
	}

	SendMsg(notify);
}

void CCliNetSession::notifyGuildRedPacketLogRes(OGSLib::GameSession* session,std::list<OGSLib::GuildRedPacketRecord>& log_list)
{
	if( session != mGameSession ) return;
	GS_Client::GuildRedPacketLogRes res;
	std::list<OGSLib::GuildRedPacketRecord>::iterator it = log_list.begin();
	while (it != log_list.end())
	{
		GS_Client::GuildRedPacketLogRes::GuildRedPacketLog log;
		log.sender = (*it).name;
		log.op_code = (*it).op_code;
		log.value = (*it).value;
		log.count = (*it).count;
		res.m_LogList.push_back(log);
		++ it;
	}
	SendMsg(res);
}

void CCliNetSession::notifyGuildWar(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::GuildWarNotify notify;
	SendMsg(notify);
}

void CCliNetSession::notifyGuildItemLogRes(OGSLib::GameSession* session,std::list<OGSLib::GuildItemOpRecord>& log_list)
{
	if( session != mGameSession ) return;
	GS_Client::GuildItemLogRes res;
	std::list<OGSLib::GuildItemOpRecord>::iterator it = log_list.begin();
	while (it != log_list.end())
	{
		OGSLib::ItemDef* def = mGameSession->GetGameServerLib()->GetItemDefineManager()->GetItemDefine((*it).item_type);
		if (def)
		{
			GS_Client::GuildItemLogRes::GuildItemLog log;
			log.name = (*it).name;
			log.item_name = def->mName;
			log.op_code = (*it).op_code;
			log.time = (*it).time;
			res.m_LogList.push_back(log);
		}

		++ it;
	}
	SendMsg(res);
}

void CCliNetSession::notifyGuildRedPacketLog(OGSLib::GameSession* session,OGSLib::GuildRedPacketRecord * log)
{
	if( session != mGameSession ) return;
	GS_Client::GuildRedPacketLogNotify res;
	res.sender = log->name;
	res.op_code = log->op_code;
	res.value = log->value;
	res.count = log->count;
	SendMsg(res);
}

void CCliNetSession::notifyGroupChat(OGSLib::GameSession* session,OGSLib::Group* g,OGSLib::DomainGroup* domain,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::GroupChatNotify notify;
	strncpy_s(notify.name,_countof(notify.name),domain->Name(),511);
	notify.name[511] = 0;
	strncpy_s(notify.msg,_countof(notify.msg),msg,511);
	notify.msg[511] = 0;

	OGSLib::DomainPlay * player = domain->GetPlayer();
	if (player)
	{
		const std::string& guild = player->GuildName();
		strncpy_s(notify.guild,_countof(notify.guild), guild.c_str(),511);
		notify.guild[511] = 0;
		notify.gender = player->gender();
		notify.lv = player->level();
		notify.job = player->job();
		notify.vip = player->GetModel(5);
	}
	SendMsg(notify);
}
void CCliNetSession::notifyGhostMode(OGSLib::GameSession* session,int mode)
{
	if( session != mGameSession ) return;
	GS_Client::GhostModeNotify notify;
	notify.ghost_mode = mode;
	SendMsg(notify);
}
void CCliNetSession::notifyAlert(OGSLib::GameSession* session,int lv,int flags,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::AlertNotify notify;
	notify.lv = lv;
	notify.flags = flags;
	strncpy_s(notify.msg,_countof(notify.msg),msg,1023);
	notify.msg[1023]=0;
	SendMsg(notify);
}
void CCliNetSession::notifyRelive(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int type)
{
	if( session != mGameSession ) return;
	GS_Client::ReliveNotify notify;
	notify.src = src->ID();
	notify.type = type;
	SendMsg(notify);
}

void CCliNetSession::notifyGroupInfoChange(OGSLib::GameSession* session,OGSLib::Group* g)
{
	if( session != mGameSession ) return;
	GS_Client::GroupInfoChangeNotify notify;
	if ( g )
	{
		notify.group_id = g->ID();
		notify.pick_mode = g->GetPickMode();
		notify.leader = g->GetLeader();
		notify.name = g->GetName();
		for( OGSLib::Group::MemberMap::const_iterator pos = g->GetMembers().begin();
			pos != g->GetMembers().end();
			++ pos )
		{
			GS_Client::GroupInfoChangeNotify::member mb;
			
			if (pos->second.session && pos->second.session->GetPlayer())
				mb.src = pos->second.session->GetPlayer()->ID();
			else
				mb.src = 0;

			mb.nick_name = pos->first;
			notify.members.push_back(mb);
		}
	}
	SendMsg(notify);
}
void CCliNetSession::notifyGroupState(OGSLib::GameSession* session,OGSLib::Group* g)
{
	if( session != mGameSession ) return;
	GS_Client::GroupStateNotify notify;
	if( g )
	{
		notify.group_id = g->ID();
		for( OGSLib::Group::MemberMap::const_iterator pos = g->GetMembers().begin();
			pos != g->GetMembers().end();
			++ pos )
		{
			GS_Client::GroupStateNotify::member mb;
			const OGSLib::GroupNode* gn = &(pos->second);
			mb.state = gn->state;
			mb.hp = gn->hp;
			mb.mp = gn->mp;
			mb.job = gn->job;
			mb.level = gn->level;
			mb.power = gn->power;
			mb.cur_map = gn->cur_map;
			mb.gender = gn->gender;
			notify.members.push_back(mb);
		}
	}
	SendMsg(notify);
}
void CCliNetSession::resChangeAttackMode(OGSLib::GameSession* session,int attack_mode)
{
	if( session != mGameSession ) return;
	GS_Client::ChangeAttackModeRes res;
	res.attack_mode = attack_mode;
	SendMsg(res);
}
void CCliNetSession::notifyNpcTalk(OGSLib::GameSession* session,int src_id,int flags,int param,const char* title,const std::string& msg)
{
	GS_Client::NPCTalkres res;
	res.srcid = src_id;
	res.flags = flags;
	res.param = param;
	res.title = title;
	res.msg = &msg;
	SendMsg(res);
}
void CCliNetSession::notifyBlackBoard(OGSLib::GameSession* session,int flags,const char* title,const std::string& msg)
{
	GS_Client::BlackBoardNotify notify;
	notify.flags = flags;
	notify.title = title;
	notify.msg = &msg;
	SendMsg(notify);
}
void CCliNetSession::notifyItemTalk(OGSLib::GameSession* session,OGSLib::DomainPlay* player,int item_id,const char* title,const std::string& msg,int seed)
{
	if( session != mGameSession ) return;
	GS_Client::ItemTalkNotify notify;
	notify.item_id = item_id;
	notify.seed = seed;
	notify.title = title;
	notify.msg = &msg;
	SendMsg(notify);
}
void CCliNetSession::notifyPlayerTalk(OGSLib::GameSession* session,OGSLib::DomainPlay* player,const std::string& msg,int seed)
{
	if( session != mGameSession ) return;
	GS_Client::PlayerTalkNotify notify;
	notify.seed = seed;
	notify.msg = &msg;
	SendMsg(notify);
}
void CCliNetSession::notifyNpcShop(OGSLib::GameSession* session,OGSLib::PlayNpc* npc,int page,std::string msg)
{
	if( session != mGameSession ) return;
	GS_Client::NpcShopRes res;
	res.srcid = npc->ID();
	res.msg = msg;
	res.page = page;
	GS_Client::NpcShopRes::good g;
	for( OGSLib::PlayNpc::ShopGoodMap::iterator pos = npc->GetGoods()->begin();
		pos != npc->GetGoods()->end();
		++ pos )
	{
		if( pos->second.page == page || pos->second.page == 9 )
		{
			g.pos = pos->second.pos;
			if( pos->second.page == 9 )
			{
				g.pos += 1000;
			}
			g.good_id = pos->second.good_id;
			g.type_id = pos->second.type_id;
			g.number = pos->second.number;
			g.price_type = pos->second.price_type;
			g.price = pos->second.price;
			g.oldprice = pos->second.oldprice;
			g.good_tag = pos->second.good_tag;
			res.goods.push_back(g);
		}
	}
	SendMsg(res);
}
void CCliNetSession::resVcoinShopList(OGSLib::GameSession* session,unsigned int shop_npc_id)
{
	if( session != mGameSession ) return;
	GS_Client::VcoinShopListRes res;
	res.shop_npc_id = shop_npc_id;
	SendMsg(res);
}
void CCliNetSession::notifyUseSkill(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int skill_type,int paramX,int paramY,unsigned int paramID,int resID,int dir, int skill_level)
{
	if( session != mGameSession ) return;
	GS_Client::UseSkillNotify notify;
	notify.srcid = src->ID();
	notify.skill_type_id = skill_type;
	notify.paramX = paramX;
	notify.paramY = paramY;
	notify.paramID = paramID;
	notify.resID = resID;
	notify.skill_level = skill_level;
	notify.dir = dir;
	SendMsg(notify);
}
void CCliNetSession::notifyForceMove(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int x,int y,int dir)
{
	if( session != mGameSession ) return;
	GS_Client::ForceMoveNotify notify;
	notify.id = src->ID();
	notify.x = x;
	notify.y = y;
	notify.dir = dir;
	SendMsg(notify);
}
void CCliNetSession::notifyWarInfo(OGSLib::GameSession* session,int var_state,const std::string& king_guild)
{
	if( session != mGameSession ) return;
	GS_Client::WarInfoNotify notify;
	notify.war_state = var_state;
	strncpy_s(notify.king_guild ,_countof(notify.king_guild), king_guild.c_str(),511);
	notify.king_guild[511]=0;
	OGSLib::Guild* guild = mGameSession->GetGameServerLib()->GetGuildManager()->FindGuildByName(king_guild);
	if (guild)
	{
		strncpy_s(notify.king_leader,_countof(notify.king_leader),guild->GetGuildLeaderName().c_str(),511);
		notify.king_leader[511]=0;
	}
	else{notify.king_leader[0]=0;}
	SendMsg(notify);
}
void CCliNetSession::notifyMapOption(OGSLib::GameSession* session,OGSLib::PlayMap* map)
{
	if( session != mGameSession ) return;
	GS_Client::MapOptionNotify notify;
	strncpy_s(notify.map_id,_countof(notify.map_id),map->GetID().c_str(),511);
	notify.map_id[511]=0;
	notify.pkprohibit = map->PKProhibit();
	notify.pkallow = map->PKAllow();
	notify.auto_alive = map->AutoAlive();
	notify.nointeract = map->NoInteract();
	notify.lockaction = map->LockAction();
	notify.wanderhangup = map->WanderHangup();
	notify.fightstate = map->FightAnonym();
	SendMsg(notify);
}
void CCliNetSession::notifyFriendChange(OGSLib::GameSession* session,OGSLib::SubFriend &sf)
{
	if( session != mGameSession ) return;
	GS_Client::FriendChangeRes res;
	strncpy_s(res.name, sf.mName.c_str(), sf.mName.length());
	res.title = sf.mTitle & 0x0000ffff;
	res.online_state=sf.mOnlineState;
	res.gender = sf.mGender;
	res.job = sf.mJob;
	res.level = sf.mLevel;
	strncpy_s(res.guild, sf.mGuild.c_str(), sf.mGuild.length());

	SendMsg(res);
}

void CCliNetSession::notifyFriendApply(OGSLib::GameSession* session,const char* name)
{
	if( session != mGameSession ) return;
	GS_Client::FriendApply msg_apply;
	strncpy_s(msg_apply.name,name,strlen(name));
	SendMsg(msg_apply);
}

void CCliNetSession::notifyFriendFresh(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::FriendFreshRes res;
	SendMsg(res);
}

void CCliNetSession::notifyParamData(OGSLib::GameSession* session,int srcid,int id,std::string desp)
{
	if( session != mGameSession ) return;
	GS_Client::ParamDataNotify notify;
	notify.srcid = srcid;
	notify.id = id;
	strncpy_s(notify.desp,_countof(notify.desp),desp.c_str(),511);
	notify.desp[511]=0;
	SendMsg(notify);
}
void CCliNetSession::notifyParamDataList(OGSLib::GameSession* session,OGSLib::DomainPlay* player)
{
	if( session != mGameSession ) return;
	GS_Client::ParamDataListNotify notify;
	notify.srcid = player->ID();
	notify.num=player->GetParamMap().size();
	for( std::map<int,std::string>::const_iterator pos = player->GetParamMap().begin();
			pos != player->GetParamMap().end();
			++ pos )
	{
		GS_Client::ParamDataListNotify::ParamNode node;
		node.id=pos->first;
		node.desp=pos->second;
		notify.mParamList.push_back(node);
	}
	SendMsg(notify);
}

void CCliNetSession::notifyShowProgressBar(OGSLib::GameSession* session,int duration,const char* msg)
{
	if( session != mGameSession ) return;
	GS_Client::ShowProgressBarNotify notify;
	notify.duration = duration;
	strncpy_s(notify.desp,_countof(notify.desp),msg,511);
	notify.desp[511]=0;
	SendMsg(notify);
}

void CCliNetSession::notifyProsperityChange(OGSLib::GameSession* session,int prosperity,int next)
{
	
}

void CCliNetSession::notifySlotAdd(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::StotAddNotify notify;
	notify.bag_add = session->GetPlayer()->bag_slot_add();
	notify.depot_add = session->GetPlayer()->depot_slot_add();
	notify.bag_max_num = mGameSession->GetGameServerLib()->mAddBagMaxSlot;
	SendMsg(notify);
}

void CCliNetSession::resProtectItem(OGSLib::GameSession* session,int result)
{
	if( session != mGameSession ) return;
	GS_Client::ProtectItemRes res;
	res.result = result;
	SendMsg(res);
}

void CCliNetSession::notifySetModel(OGSLib::GameSession* session,unsigned int src_id,int id,int vl)
{
	if( session != mGameSession ) return;
	GS_Client::SetModelNotify notify;
	notify.src_id = src_id;
	notify.id = id;
	notify.vl = vl;
	SendMsg(notify);
}
void CCliNetSession::notifyChinaLimit(OGSLib::GameSession* session,int china_limit_lv,int online_time_today,int china_id)
{
	if( session != mGameSession ) return;
	GS_Client::ChinaLimitNotify notify;
	notify.china_limit_lv = china_limit_lv;
	notify.online_time_today = online_time_today;
	notify.china_id = china_id;
	SendMsg(notify);
}
void CCliNetSession::notifyOfflineExpInfo(OGSLib::GameSession* session,int offline_time,int offline_time_valide,int offline_time_valide_max,int offline_exp,int price1,int price2,int price3)
{
	if( session != mGameSession ) return;
	GS_Client::OfflineExpInfoNotify notify;
	notify.offline_time = offline_time;
	notify.offline_time_valide = offline_time_valide;
	notify.offline_time_valide_max = offline_time_valide_max;
	notify.offline_exp = offline_exp;
	notify.price1 = price1;
	notify.price2 = price2;
	notify.price3 = price3;
	SendMsg(notify);
}
void CCliNetSession::notifyTeamInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* src)
{
	if( session != mGameSession ) return;
	GS_Client::TeamInfoNotify notify;
	notify.src_id = src->ID();
	notify.team_id = src->TeamID();
	strncpy_s(notify.team_name,_countof(notify.team_name),src->TeamName(),511);
	notify.team_name[511] = 0;
	SendMsg(notify);
}
void CCliNetSession::notifyNameAdd(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const std::string& pre,const std::string& pro)
{
	if( session != mGameSession ) return;

	if( src->TypeIsPlayer() )
	{
		if(mGameSession->GetPlayer()->GetMap())
		{
			if(mGameSession->GetPlayer()->GetMap()->FightAnonym())
			{
				return;
			}
		}
	}

	GS_Client::NameAddNotify notify;
	notify.src_id = src->ID();
	strncpy_s(notify.pre ,_countof(notify.pre), pre.c_str(),511);
	notify.pre[511]=0;
	strncpy_s(notify.pro ,_countof(notify.pro), pro.c_str(),511);
	notify.pro[511]=0;
	SendMsg(notify);
}
void CCliNetSession::notifyUpdateTick(OGSLib::GameSession* session)
{
	//m_app->UpdateTicket(m_Socket,mSessionID,0);
}
//void CCliNetSession::notifyVipChange(OGSLib::GameSession* session)
//{
//	if( session != mGameSession ) return;
//	GS_Client::VipChangeNotify notify;
//	notify.vcion_accu = session->GetPlayer()->vcoinaccu();
//	notify.vip_level = session->GetPlayer()->viplevel();
//	SendMsg(notify);
//}
void CCliNetSession::notifySlaveState(OGSLib::GameSession* session,int alive)
{
	if( session != mGameSession ) return;
	GS_Client::SlaveStateNotify notify;
	notify.alive = alive;
	SendMsg(notify);
}
void CCliNetSession::notifyCapacityChange(OGSLib::GameSession* session,int change_capacity)
{
	if( session != mGameSession ) return;
	GS_Client::CapacityChangeNotify notify;
	notify.capacity = session->GetPlayer()->capacity();
	notify.change_capacity = change_capacity;
	SendMsg(notify);
}

void CCliNetSession::notifyTotalAttrInfo(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::TotalAttrParamNotify notify;
	for (OGSLib::ManagerTotalAttr::TotalAttrInfoMap::iterator pos = mGameSession->GetGameServerLib()->GetTotalInfoManager()->GetTotalAttrMap().begin();
		pos != mGameSession->GetGameServerLib()->GetTotalInfoManager()->GetTotalAttrMap().end();
		++ pos)
	{
		OGSLib::TotalAttrInfo* tai = mGameSession->GetGameServerLib()->GetTotalInfoManager()->GetTotalAttrInfo(pos->second.mID);
		if( tai )
		{
			GS_Client::TotalAttrParamNotify::TotalAttr ta;
			ta.mID = pos->second.mID;
			ta.mLevel = pos->second.mLevel;
			ta.mJob = pos->second.mJob;
			ta.mAC = pos->second.mAC;
			ta.mACMax = pos->second.mACMax;
			ta.mMAC = pos->second.mMAC;
			ta.mMACMax = pos->second.mMACMax;
			ta.mDC = pos->second.mDC;
			ta.mDCMax = pos->second.mDCMax;
			ta.mMC = pos->second.mMC;
			ta.mMCMax = pos->second.mMCMax;
			ta.mSC = pos->second.mSC;
			ta.mSCMax = pos->second.mSCMax;
			ta.mHP = pos->second.mHP;
			ta.mMP = pos->second.mMP;
			ta.mAccuracy = pos->second.mAccuracy;
			ta.mDodge = pos->second.mDodge;
			ta.mLuck = pos->second.mLuck;
			ta.mDropProb = pos->second.mDropProb;
			ta.mDoubleAttProb = pos->second.mDoubleAttProb;
			notify.mTotalAttr.push_back(ta);
		}
	}
	SendMsg(notify);
}

void CCliNetSession::notifyGiftList(OGSLib::GameSession* session,std::string msgs)
{
	if( session != mGameSession ) return;
	GS_Client::GiftListNotify notify;
	strncpy_s(notify.msg,msgs.c_str(),1023);
	notify.msg[1023]=0;
	for (std::vector<OGSLib::SubItem>::iterator pos = session->GetPlayer()->GetGiftList().begin();
		pos != session->GetPlayer()->GetGiftList().end();
		++ pos)
	{
		GS_Client::GiftListNotify::GiftNode gn;
		gn.mID = pos->mTypeID;
		gn.mNum = pos->mDuraMax;
		mGameSession->GetPlayer()->PushItemInfo(gn.mID);
		notify.mGiftList.push_back(gn);
	}
	SendMsg(notify);
}
void CCliNetSession::notifyCollectBreak(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::CollectBreakNotify notify;
	SendMsg(notify);
}

void CCliNetSession::notifyCollectEnd(OGSLib::GameSession* session, int src_id)
{
	if( session != mGameSession ) return;
	GS_Client::CollectEndNotify notify;
	notify.srcid = src_id;
	SendMsg(notify);
}

void CCliNetSession::notifyPushLuaTable(OGSLib::GameSession* session,std::string type,int flag,std::string table)
{
	if( session != mGameSession ) return;
	GS_Client::PushLuaTableNotify notify;
	notify.type=type;
	notify.flag=flag;
	notify.table=table;
	SendMsg(notify);
}
void CCliNetSession::notifyMails(OGSLib::GameSession* session,OGSLib::DomainPlay* player)
{
	GS_Client::MailsNotify notify;
	int mail_num = 0;
	std::string seedid = mGameSession->SeedID();
	for (std::map<LEUD_INT64,OGSLib::SubMail>::iterator pos = player->GetMailMap().begin();
		pos!= player->GetMailMap().end();
		++pos)
	{
		if (mail_num < 30)
		{
			if (pos->second.mDeleted == 0){
				GS_Client::MailsNotify::Mail mail;
				char buffer[512] = {0};
				sprintf(buffer, "%I64d", pos->second.mID);
				mail.id = buffer;
				mail.date = pos->second.mMailDate;
				mail.isOpen = pos->second.mRead;
				mail.isReceive = pos->second.mReceived;
				
				mail.title = mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAIL_TITLE; //��δ�ҵ���Ӧ��title��content���һ���?��ֵ
				mail.content = mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAIL_CONTENT;

				for (auto pp = mGameSession->GetGameServerLib()->GetMailManager()->GetMails().begin(); 
					pp != mGameSession->GetGameServerLib()->GetMailManager()->GetMails().end();
					++pp)
				{
					if (*pp)
					{
						OGSLib::Mail* new_mail = *pp;
						if (pos->second.mID == new_mail->mID && (new_mail->mOwners == seedid || new_mail->mOwners == "all" || new_mail->mOwners == "verify") )
						{
							mail.title = new_mail->mTitle;
							mail.content = new_mail->mContent;
							break;
						}
					}
					
				}
				mail.items = pos->second.mItems;
				notify.mMails.push_back(mail);
				mail_num++;
			}
		}
		else
		{
			player->AlertMessage(11, 1, mGameSession->GetGameServerLib()->mStringConsts.NOTIFY_MAIL_FULL);
			break;
		}
	}
	SendMsg(notify);
}
void CCliNetSession::notifyNewMailNum(OGSLib::GameSession* session,int num)
{
	GS_Client::MailNumNotify notify;
	notify.num = num;
	SendMsg(notify);
}
void CCliNetSession::notifyMailReceiveSuccess(OGSLib::GameSession* session,LEUD_INT64 mailID)
{
	if( session != mGameSession ) return;
	GS_Client::MailReceiveSuccessNotify notify;
	char buffer[512] = {0};
	sprintf(buffer, "%I64d", mailID);
	//notify.mailID=mailID;
	notify.mailID = buffer;
	SendMsg(notify);
}

void CCliNetSession::notifyGotoEnd(OGSLib::GameSession* session,const char* target)
{
	GS_Client::GotoEndNotify notify;
	strncpy_s(notify.target,target,511);
	notify.target[511]=0;
	SendMsg(notify);
}
void CCliNetSession::notifyChargeDartList(OGSLib::GameSession* session)
{
	if( session != mGameSession ) return;
	GS_Client::ListChargeDartNotify notify;
	OGSLib::ManagerChargeDart::ChargeDartMap list =  mGameSession->GetGameServerLib()->GetChargeDartManager()->GetChargeDarts();
	int selfDartLevel = mGameSession->GetPlayer()->level() + mGameSession->GetPlayer()->ZsLevel()*10000;
	if (selfDartLevel <= 80)
	{
		selfDartLevel = 3;
	}
	else
	{
		if (selfDartLevel >= 80 && selfDartLevel <= 50000)
			selfDartLevel = 2;
		else
			selfDartLevel = 1;
	}
	
	for(OGSLib::ManagerChargeDart::ChargeDartMap::iterator pos = list.begin();pos != list.end();pos++)
	{
		if((pos->second->mState == 0 && pos->second->mDartLevel == selfDartLevel) || pos->second->mCharName == mGameSession->CharID())
		{
			GS_Client::ListChargeDartNotify::ChargeDart data;
			data.mCharName = pos->second->mCharName;
			data.mIcon = pos->second->mIcon;
			data.mSurplusTime = pos->second->mDuration - (mGameSession->GetGameServerLib()->GetTimerFix()->Elapsed() - pos->second->mStartTime);
			if(data.mSurplusTime < 0) data.mSurplusTime = 0;
			data.mDuration = pos->second->mDuration;
			data.mFightingForce = pos->second->mFightingForce;
			data.mStolenTimes = pos->second->mStolenTimes;
			data.mTotalAwards = pos->second->mTotalAwards;
			data.mSurplusAwards = pos->second->mSurplusAwards;
			data.mState = pos->second->mState;
			data.mRobDartCharNames = pos->second->mRobDartCharNames;
			notify.m_ChargeDartList.push_back(data);
		}
	}
	SendMsg(notify);
}
void CCliNetSession::OnListCharacter(GS_Client::ListCharacterRes& res)
{
	SendMsg(res);
}

void CCliNetSession::OnCreateCharacter(GS_Client::CreateCharacterRes& res)
{
	SendMsg(res);
}

void CCliNetSession::OnDeleteCharacter(GS_Client::DeleteCharacterRes& res)
{
	if (res.result == GS_Client::DeleteCharacterRes::result_type_succes)
	{
		OGSLib::Guild * p = m_app->m_pGameServerLib->GetGuildManager()->FindGuild(res.guild_seed);
		if (p)
		{
			p->SetMemberTitle(res.chr_seed, OGSLib::GUILD_TITLE_TYPE_OUT);
			p->NotifyGuildMemberChange();
		}
	}
	SendMsg(res);
}

void CCliNetSession::OnAuthResult(int result,const std::string& sessionID,std::string& account,int tim)
{
	if( 0 == result )
	{
		if( tim == 0 && !mIsKuafuBack )
		{
			if( ! m_app->m_pGameServerLib->FindSessionWithLoginID(account) )
			{
				GS_Client::AuthenticateRES res;
				res.result = GS_Client::AuthenticateRES::result_session_time_out;
				SendMsg(res);
				return ;
			}
		}
		mAuthed = true;
		mLoginID = account;

		GS_Client::AuthenticateRES res;
		res.result = GS_Client::AuthenticateRES::result_succ;
		SendMsg(res);
	}
	else
	{
		GS_Client::AuthenticateRES res;
		res.result = result;
		SendMsg(res);
		return;
	}
}
void CCliNetSession::OnUpdateTicket(int result)
{
	GS_Client::UpdateTicketRes res;
	SendMsg(res);
}

void CCliNetSession::notifyConsignItem(OGSLib::GameSession* session, int ret)
{
	GS_Client::ConsignItemNotify res;
	res.ret = ret;
	SendMsg(res);
}

void CCliNetSession::notifyBuyConsignableItem(OGSLib::GameSession* session, int ret, int consign_seed_id)
{
	GS_Client::BuyConsignableItemNotify res;
	res.ret = ret;
	res.seed_id = consign_seed_id;
	SendMsg(res);
}

void CCliNetSession::notifyTakeBackConsignableItem(OGSLib::GameSession* session, int ret, int consign_seed_id)
{
	GS_Client::TakeBackConsignableItemNotify res;
	res.ret = ret;
	res.seed_id = consign_seed_id;
	SendMsg(res);
}

void CCliNetSession::notifyTakeBackVCoin(OGSLib::GameSession* session, int ret)
{
	GS_Client::TakeBackVCoinNotify res;
	res.ret = ret;
	SendMsg(res);
}

void CCliNetSession::notifyConsignableItems(OGSLib::GameSession* session, int type, int job, int condition, std::vector<OGSLib::ConsignableItemInfo*>& vec, std::vector<int>& list_index)
{
	time_t now = time(0);
	GS_Client::ConsignableItemsNotify msg;
	msg.type = type;
	msg.job = job;
	msg.condition = condition;

	for (size_t i = 0; i < vec.size(); ++ i)
	{

		GS_Client::ConsignableItemsNotify::ConsignableItem item;
		item.id = vec[i]->seed_id;
		item.price = vec[i]->price;
		item.time_left = vec[i]->end_time - now;
		if (vec.size() != list_index.size())
			item.index = 0;
		else
			item.index = list_index[i];

		item.mTypeID = vec[i]->item.mTypeID;
		item.mDuraMax= vec[i]->item.mDuraMax;
		item.mDuration= vec[i]->item.mDuration;
		item.mItemFlags= vec[i]->item.mItemFlags;
		item.mLuck= vec[i]->item.mLuck;
		item.mLevel= vec[i]->item.mLevel;
		item.mNumber= vec[i]->item.mNumber;
		item.mAddAC= vec[i]->item.mAddAC;
		item.mAddMAC= vec[i]->item.mAddMAC;
		item.mAddDC= vec[i]->item.mAddDC;
		item.mAddMC= vec[i]->item.mAddMC;
		item.mAddSC= vec[i]->item.mAddSC;
		item.mAddHp= vec[i]->item.mAddHp;
		item.mAddMp= vec[i]->item.mAddMp;
		item.mUpdAC= vec[i]->item.mUpdAC;
		item.mUpdMAC= vec[i]->item.mUpdMAC;
		item.mUpdDC= vec[i]->item.mUpdDC;
		item.mUpdMC= vec[i]->item.mUpdMC;
		item.mUpdSC= vec[i]->item.mUpdSC;
		item.mProtect= vec[i]->item.mProtect;
		item.mUpdMaxCount= vec[i]->item.mUpdMaxCount;
		item.mUpdFailedCount= vec[i]->item.mUpdFailedCount;
		item.mZLevel= vec[i]->item.mZLevel;
		item.mLock= vec[i]->item.mLock;

		msg.m_ItemsList.push_back(item);
	}
	SendMsg(msg);
	
}

void CCliNetSession::notifyMonExpHiterChange(OGSLib::GameSession* session,int src_id, int hiter_id, const char * name)
{
	GS_Client::MonExpHiterChangeNotify msg;
	msg.src_id = src_id;
	msg.hiter_id = hiter_id;
	msg.hiter_name = name;
	SendMsg(msg);
}

LEUD::PoolMemory<CCliNetSession>	LEUD::PoolMemoryBase<CCliNetSession>::memPool("CCliNetSession");
