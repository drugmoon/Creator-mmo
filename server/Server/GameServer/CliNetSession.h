#pragma once

#include "GameServerLib/GameSession.h"

namespace GS_Client
{
	class ListCharacterRes;
	class CreateCharacterRes;
	class DeleteCharacterRes;
}
class CGameServerApp;

class CCliNetSession :
	public LEUD::TCPSessionMini,
	public OGSLib::GameSessionListener,
	public LEUD::PoolMemoryBase< CCliNetSession >
{
private:
	CGameServerApp*	m_app;
	OGSLib::GameSession* mGameSession;
	OGSLib::DomainPlay* mGamePlayer;
	std::string mLoginID;
	std::string mCharID;
	std::string mSessionID;
	std::string mIdfa;
	int mAuthSeed;
	int mPlatformID;
	int mAuthType;
	int mNewYouKe;
	bool mAuthed;
	bool mNetCloseBreak;
	int mTxSystemFlag;

	time_t mSearchTick;
	time_t mChatTick;
	time_t mPing;
	time_t mPingTick;
	time_t mPingStopAction;
	DWORD mClientTimeTag;
	int mClientTimeErrorCount;
	int mClientTimeTagFrame;
	DWORD mServerTimeTag;
	DWORD mServerTimeResetDuration;
	int mPingUpdateTick;
	int mFirstPingUpdateTick;
	int mSessionStartTick;
	int mUseSkillSendTag;
	int mUseItemSendTag;
	int mCheckItemUse;
	int mMoveSendTag;
	int mUseItemCountPerSecond;
	time_t mUseItemCountSecontTick;

	double mItemUseTick;

	int mAs2LuaCountPerSecond;
	time_t mAs2LuaSecontTick;

	int mRunWalkCountPerSecond;
	time_t mRunWalkCountTick;
	time_t mConnectedTime;
	time_t mConnectedMsgTime;

	std::string mKuafuParam;
	std::string mKuafuSIP;
	std::string mKuafuSPort;
	std::string mKuafuTicket;
	std::string mKuafuArea;
	bool mIsKuafuBack;
public:
	CCliNetSession(CGameServerApp* app);
	~CCliNetSession(void);

	virtual void Open( SOCKET sock );
	virtual void OnClose( DWORD Error );
	virtual void OnMessage( const char* msg,size_t size );
	//virtual size_t BreakMessage( Stream_t* stream );
	virtual size_t BreakMessage( LEUD::StreamFix& stream );
	virtual void EncodeMessage( char* buf,size_t size );
	virtual void DecodeMessage( char* buf,size_t size );
	virtual void MakeHead( char* buf,unsigned short size );
	virtual void NetBreak();
	int check_client_time(DWORD timetag);
	int check_client_tag(int tag);
	bool check_as2lua_tag();
	bool check_runwalk_tag();
	void KuaFuSendCharData(int result,OGSLib::DomainData* character);
public:
	virtual std::string GetKuafuArea(){return mKuafuArea;}
public:
	void OnMessageKuaFu(unsigned short cmdtype,LEUD::StreamFix& instream);
	void ProcessAuthenticate(LEUD::StreamFix& instream);
	void ProcessClientGSKuafuAuth(LEUD::StreamFix& instream);
	void ProcessUpdateTicket(LEUD::StreamFix& instream);
	void ProcessMapChat(LEUD::StreamFix& instream);
	void ProcessPrivateChat(LEUD::StreamFix& instream);
	void ProcessWalk(LEUD::StreamFix& instream);
	void ProcessRun(LEUD::StreamFix& instream);
	void ProcessNPCTalk(LEUD::StreamFix& instream);
	void ProcessTurn(LEUD::StreamFix& instream);
	void ProcessAttack(LEUD::StreamFix& instream);
	void ProcessPickUp(LEUD::StreamFix& instream);
	void ProcessBagUseItem(LEUD::StreamFix& instream);
	void ProcessConsignBagItem(LEUD::StreamFix& instream);
	void ProcessGetConsignableItems(LEUD::StreamFix& instream);
	void ProcessBuyConsignableItem(LEUD::StreamFix& instream);
	void ProcessTakeBackConsignableItem(LEUD::StreamFix& instream);
	void ProcessTakeBackVCoin(LEUD::StreamFix& instream);
	void ProcessUndressItem(LEUD::StreamFix& instream);
	void ProcessItemPositionExchange(LEUD::StreamFix& instream);
	void ProcessUseSkill(LEUD::StreamFix& instream);
	void ProcessNpcShop(LEUD::StreamFix& instream);
	void ProcessNpcBuy(LEUD::StreamFix& instream);
	void ProcessCancelTask(LEUD::StreamFix& instream);
	void ProcessForceMove(LEUD::StreamFix& instream);
	void ProcessChangeCloth(LEUD::StreamFix& instream);
	void ProcessListCharacter(LEUD::StreamFix& instream);
	void ProcessCreateCharacter(LEUD::StreamFix& instream);
	void ProcessEnterGame(LEUD::StreamFix& instream);
	void ProcessDeleteCharacter(LEUD::StreamFix& instream);
	void ProcessDropItem(LEUD::StreamFix& instream);
	void ProcessNPCSell(LEUD::StreamFix& instream);
	void ProcessSaveShortcut(LEUD::StreamFix& instream);
	void ProcessListGuild(LEUD::StreamFix& instream);
	void ProcessGetGuildInfo(LEUD::StreamFix& instream);
	void ProcessCreateGuild(LEUD::StreamFix& instream);
	void ProcessJoinGuild(LEUD::StreamFix& instream);
	void ProcessSetGuildInfo(LEUD::StreamFix& instream);
	void ProcessListGuildMember(LEUD::StreamFix& instream);
	void ProcessListGuildEnemy(LEUD::StreamFix& instream);
	void ProcessListGuildFriend(LEUD::StreamFix& instream);
	void ProcessListGuildItem(LEUD::StreamFix& instream);
	void ProcessChangeGuildMemberTitle(LEUD::StreamFix& instream);
	void ProcessChangeEnemyGuild(LEUD::StreamFix& instream);
	void ProcessChangeFriendGuild(LEUD::StreamFix& instream);
	void ProcessRedPacketLogReq(LEUD::StreamFix& instream);
	void ProcessGuildItemLogReq(LEUD::StreamFix& instream);
	void ProcessGuildChat(LEUD::StreamFix& instream);
	void ProcessChangeAttackMode(LEUD::StreamFix& instream);
	void ProcessVcoinShopList(LEUD::StreamFix& instream);
	void ProcessNPCRepair(LEUD::StreamFix& instream);
	void ProcessRelive(LEUD::StreamFix& instream);
	void ProcessTaskDesp(LEUD::StreamFix& instream);
	void ProcessInfoPlayer(LEUD::StreamFix& instream);
	void ProcessCreateGroup(LEUD::StreamFix& instream);
	void ProcessLeaveGroup(LEUD::StreamFix& instream);
	void ProcessJoinGroup(LEUD::StreamFix& instream);
	void ProcessAgreeJoinGroup(LEUD::StreamFix& instream);
	void ProcessInviteGroup(LEUD::StreamFix& instream);
	void ProcessAgreeInviteGroup(LEUD::StreamFix& instream);
	void ProcessTaskClick(LEUD::StreamFix& instream);
	void ProcessGroupChat(LEUD::StreamFix& instream);
	void ProcessNormalChat(LEUD::StreamFix& instream);
	void ProcessTradeInvite(LEUD::StreamFix& instream);
	void ProcessAgreeTradeInvite(LEUD::StreamFix& instream);
	void ProcessCloseTrade(LEUD::StreamFix& instream);
	void ProcessTradeAddGameMoney(LEUD::StreamFix& instream);
	void ProcessTradeAddVcoin(LEUD::StreamFix& instream);
	void ProcessTradeSubmit(LEUD::StreamFix& instream);
	void ProcessGroupSetLeader(LEUD::StreamFix& instream);
	void ProcessTradeAddItem(LEUD::StreamFix& instream);
	void ProcessChatTradeSubmit(LEUD::StreamFix& instream);
	void ProcessDestoryItem(LEUD::StreamFix& instream);
	void ProcessSortItem(LEUD::StreamFix& instream);
	void ProcessItemTalk(LEUD::StreamFix& instream);
	void ProcessMergeSteel(LEUD::StreamFix& instream);
	//void ProcessUpgradeEquip(LEUD::StreamFix& instream);
	void ProcessWorldChat(LEUD::StreamFix& instream);
	void ProcessFreshVcoin(LEUD::StreamFix& instream);
	void ProcessLeaveGuild(LEUD::StreamFix& instream);
	void ProcessAddDepotSlot(LEUD::StreamFix& instream);
	void ProcessGroupPickMode(LEUD::StreamFix& instream);
	void ProcessSwithSlaveAIMode(LEUD::StreamFix& instream);
	void ProcessPing(LEUD::StreamFix& instream);
	void ProcessFreshHPMP(LEUD::StreamFix& instream);
	//void ProcessUpdateChinaLimit(LEUD::StreamFix& instream);
	//void ProcessBuyOfflineExp(LEUD::StreamFix& instream);
	//void ProcessSteelEquip(LEUD::StreamFix& instream);
	void ProcessPlayerTalk(LEUD::StreamFix& instream);
	void ProcessGetChartInfo(LEUD::StreamFix& instream);
	void ProcessInfoItemExchange(LEUD::StreamFix& instream);
	void ProcessItemExchange(LEUD::StreamFix& instream);
	void ProcessGetItemDesp(LEUD::StreamFix& instream);
	void ProcessHornChat(LEUD::StreamFix& instream);
	void ProcessCountDownFinish();
	void ProcessFriendChange(LEUD::StreamFix& instream);
	void ProcessFriendFresh();
	void ProcessFriendApplyAgree(LEUD::StreamFix& instream);
	//void ProcessEquipReRandAdd(LEUD::StreamFix& instream);
	//void ProcessEquipExchangeUpgrade(LEUD::StreamFix& instream);
	void ProcessServerScript(LEUD::StreamFix& instream);
	void ProcessProtectItem(LEUD::StreamFix& instream);
	void ProcessKickMember(LEUD::StreamFix& instream);
	void ProcessFreshGift(LEUD::StreamFix& instream);
	//void ProcessMergeEquip(LEUD::StreamFix& instream);
	//void ProcessBoothAdd(LEUD::StreamFix& instream);
	void ProcessAddBagSlot(LEUD::StreamFix& instream);
	//void ProcessMarryInvite(LEUD::StreamFix& instream);
	//void ProcessAgreeOrNotMarryInvite(LEUD::StreamFix& instream);
	//void ProcessAgreeOrNotDivorceInvite(LEUD::StreamFix& instream);
	void ProcessCollectStart(LEUD::StreamFix& instream);
	void ProcessGetTaskList(LEUD::StreamFix& instream);
	void ProcessDirectFly(LEUD::StreamFix& instream);
	void ProcessMountChange(LEUD::StreamFix& instream);
	void ProcessSplitItem(LEUD::StreamFix& instream);
	void ProcessPushLuaTable(LEUD::StreamFix& instream);
	void ProcessGetMails(LEUD::StreamFix& instream);
	void ProcessOpenMail(LEUD::StreamFix& instream);
	void ProcessReceiveMailItems(LEUD::StreamFix& instream);
	void ProcessDeleteMail(LEUD::StreamFix& instream);
	void ProcessGetKuafuPTInfo(LEUD::StreamFix& instream);
	void ProcessFindMapGhost(LEUD::StreamFix& instream);
public:
	virtual void notifyAuthResult(int result);
	virtual void LoadKuafuCharacter(OGSLib::GameSession* session,const char* loginid,const char* charid,const char* seedname);
	virtual void notifySessionCreateResult(OGSLib::GameSession* session,int result);
	virtual void notifySessionDataLoaded(OGSLib::GameSession* session,int result);
	virtual void notifyCharacterLoad(OGSLib::GameSession* session);
	virtual void notifyKuafuSetGsParam(OGSLib::GameSession* session,const char* k,const char* v);
	virtual void notifyTiliChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src);
	//新增listItemChange，解决消息条数过长的问题(登入游戏时候用)
	virtual void notifyListItemChange(OGSLib::GameSession* session,OGSLib::DomainPlay* src);
	virtual void notifyItemChange(OGSLib::GameSession* session,OGSLib::SubItem item,int show_flags);
	virtual void notifyItemPanelFresh(OGSLib::GameSession* session,int panel);
	virtual void notifySkillChange(OGSLib::GameSession* session,OGSLib::SubSkill skill);
	virtual void notifyAttributeChange(OGSLib::GameSession* session);
	//virtual void notifyUpGradeInfoChange(OGSLib::GameSession* session,int arry[14]); 
	//virtual void notifyUpGradeWeaponInfo(OGSLib::GameSession* session, int arry[14]);
	virtual void notifyGameMoneyChange(OGSLib::GameSession* session,LEUD_INT64 gm_change,int vc_change,LEUD_INT64 gmb_change,int vcb_change);
	virtual void notifyHPMPChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int hp_change,int mp_change);
	virtual void notifyTaskChange(OGSLib::GameSession* session,int task_id,int flags);
	virtual void notifyItemDesp(OGSLib::GameSession* session,int item_id);
	virtual void notifyListItemDesp(OGSLib::GameSession* session);
	virtual void notifyListUpgradeDesp(OGSLib::GameSession* session);
	virtual void notifyListItemPlusDesp(OGSLib::GameSession* session);
	virtual void notifySkillDefine(OGSLib::GameSession* session,int skill_type, int skill_level);
	virtual void notifyExpChange(OGSLib::GameSession* session,int change_value);
	virtual void notifyLevelChange(OGSLib::GameSession* session);
	virtual void notifyLoadShortcut(OGSLib::GameSession* session);
	virtual void notifyGuildInfoChange(OGSLib::GameSession* session);
	virtual void notifyGuildtitle(OGSLib::GameSession* session,const char* guild_name,int title);
	virtual void notifyGuildMemberChange(OGSLib::GameSession* session);
	virtual void notifyListGuildItem(OGSLib::GameSession* session);
	virtual void notifyGuildItem(OGSLib::GameSession* session,const char* guild_name,int mnum,const char* desp,int mlv,int entering,std::string leader_name,int guild_level);
	virtual void notifyGuildWar(OGSLib::GameSession* session);
	virtual void resTaskDesp(OGSLib::GameSession* session,int task_id,short task_state,const char* desp);
	virtual void notifyNPCShowFlags(OGSLib::GameSession* session,unsigned int npc_id,int flags);
	virtual void resInfoPlayer(OGSLib::GameSession* session,OGSLib::DomainPlay* player,OGSLib::MiniPlayerInfo* info);
	virtual void notifySessionEnd(OGSLib::GameSession* session);
	virtual void notifyMapEnter(OGSLib::GameSession* session,OGSLib::PlayMap* map);
	virtual void notifyMiniNpc(OGSLib::GameSession* session,OGSLib::PlayMap* map);
	virtual void notifyMapMonGen(OGSLib::GameSession* session,OGSLib::PlayMap* map);
	virtual void notifyMeetItem(OGSLib::GameSession* session, OGSLib::PlayMapItem* map);
	virtual void notifyMapMeet(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src,int flags);
	virtual void notifyMapLevae(OGSLib::GameSession* session,OGSLib::PlayMap* map);
	virtual void notifyMapBye(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src);
	virtual void notifyGhostGuildInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* src);
	virtual void notifyDoAction(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int action);
	virtual void notifySpeed(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,float speed);
	virtual void notifyWalk(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir,int x,int y);
	virtual void notifyRun(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir,int x,int y);
	virtual void resWalk(OGSLib::GameSession* session,int dir,int x,int y,int tag);
	virtual void resSkill(OGSLib::GameSession* session,int result,int skill_type);
	virtual void resRun(OGSLib::GameSession* session,int dir,int x,int y,int tag);
	virtual void notifyTurn(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir);
	virtual void notifyAttack(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int param);
	virtual void notifyInjury(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int attacker,int change,int delay,int effect,int change_power);
	virtual void notifyAttackMiss(OGSLib::GameSession* session,OGSLib::PlayMapObject* des);
	virtual void notifyDie(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int delay);
	virtual void notifyAvatarChange(OGSLib::GameSession* session,OGSLib::PlayMapObject* src);
	virtual void notifyUseSkill(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int skill_type,int paramX,int paramY,unsigned int paramID,int resID,int dir, int skill_level);
	virtual void notifyForceMove(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int x,int y,int dir);
	virtual void notifyMapChat(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src,const char* msg);
	virtual void notifyPrivateChat(OGSLib::GameSession* session,OGSLib::DomainPlay* src,const char* msg);
	virtual void notifyMonsterChat(OGSLib::GameSession* session,OGSLib::PlayMonster* src,const char* msg);
	virtual void notifyNormalChat(OGSLib::GameSession* session,OGSLib::DomainPlay* src,const char* msg);
	virtual void notifyGuildChat(OGSLib::GameSession* session,OGSLib::Guild* gld,OGSLib::DomainGuild* src,const char* msg);
	virtual void notifyGuildRedPacketLogRes(OGSLib::GameSession* session,std::list<OGSLib::GuildRedPacketRecord>& log_list);
	virtual void notifyGuildItemLogRes(OGSLib::GameSession* session,std::list<OGSLib::GuildItemOpRecord>& log_list);
	virtual void notifyGuildRedPacketLog(OGSLib::GameSession* session,OGSLib::GuildRedPacketRecord * log);
	virtual void notifyGroupChat(OGSLib::GameSession* session,OGSLib::Group* g,OGSLib::DomainGroup* domain,const char* msg);
	virtual void notifyWorldChat(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const char* msg);
	virtual void notifyHornChat(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const char* msg);
	virtual void resHornChat(OGSLib::GameSession* session,const char* msg);
	virtual void notifyNpcTalk(OGSLib::GameSession* session,int src_id,int flags,int param,const char* title,const std::string& msg);
	virtual void notifyBlackBoard(OGSLib::GameSession* session,int flags,const char* title,const std::string& msg);
	virtual void notifyItemTalk(OGSLib::GameSession* session,OGSLib::DomainPlay* player,int item_id,const char* title,const std::string& msg,int seed);
	virtual void notifyPlayerTalk(OGSLib::GameSession* session,OGSLib::DomainPlay* player,const std::string& msg,int seed);
	virtual void notifyNpcShop(OGSLib::GameSession* session,OGSLib::PlayNpc* npc,int page,std::string msg);
	virtual void notifyGhostMode(OGSLib::GameSession* session,int mode);
	virtual void notifyAlert(OGSLib::GameSession* session,int lv,int flags,const char* msg);
	virtual void notifyRelive(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int type);
	virtual void resVcoinShopList(OGSLib::GameSession* session,unsigned int shop_npc_id);
	virtual void notifyGroupInfoChange(OGSLib::GameSession* session,OGSLib::Group* g);
	virtual void notifyGroupState(OGSLib::GameSession* session,OGSLib::Group* g);
	virtual void resChangeAttackMode(OGSLib::GameSession* session,int attack_mode);
	virtual void notifyGroupInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* player,const std::string& group_name,const std::string& group_leader);
	virtual void notifyJoinGroupToLeader(OGSLib::GameSession* session,const std::string& player_name);
	virtual void notifyInviteGroupToMember(OGSLib::GameSession* session,const std::string& leader_name,int group_id);
	virtual void notifyFindRoadGoto(OGSLib::GameSession* session,const char* map,int x,int y,const char* target,int flags);
	virtual void notifyTradeInvite(OGSLib::GameSession* session,OGSLib::DomainPlay* player);
	virtual void notifyTradeInfo(OGSLib::GameSession* session,int gamemoney,int vcoin,bool submit,const std::string& des_name,int des_level,int des_gamemoney,int des_vcoin,bool des_submit,int trade_result = 0);
	//virtual void notifyShowChatTradeItem(OGSLib::GameSession* session,int money,);
	virtual void notifyTradeItemChange(OGSLib::GameSession* session,int side,OGSLib::SubItem item,int show_flags);
	virtual void notifyPKStateChange(OGSLib::GameSession* session,OGSLib::DomainPlay* player);
	virtual void notifyStatusChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int status_id);
	virtual void notifyStatusDesp(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int status_id,int begin=0,int end=0);
	virtual void notifyCountDown(OGSLib::GameSession* session,int delay,std::string& msg);
	virtual void notifyPlayerAddInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* play);
	virtual void notifyPlayEffect(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int skill_type,int x,int y,unsigned int targetID,int ownerID,int resID);
	virtual void notifySessionClosed(OGSLib::GameSession* session,const std::string& msg);
	virtual void notifyWarInfo(OGSLib::GameSession* session,int var_state,const std::string& king_guild);
	virtual void notifyMapOption(OGSLib::GameSession* session,OGSLib::PlayMap* map);
	virtual void notifySlotAdd(OGSLib::GameSession* session);
	virtual void notifyBuffDesp(OGSLib::GameSession* session,int id);
	virtual void notifyBuffChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src, int id, int op_code, double time_left);
	//virtual void resMergeSteel(OGSLib::GameSession* session,int result);
	//virtual void resUpgradeEquip(OGSLib::GameSession* session,int result);
	virtual void resProtectItem(OGSLib::GameSession* session,int result);
	//virtual void resEquipExchangeUpgrade(OGSLib::GameSession* session,int result);
	virtual void notifyNameAdd(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const std::string& pre,const std::string& pro);
	virtual void notifySetModel(OGSLib::GameSession* session,unsigned int src_id,int id,int vl);
	virtual void notifyChinaLimit(OGSLib::GameSession* session,int china_limit_lv,int online_time_today,int china_id);
	virtual void notifyOfflineExpInfo(OGSLib::GameSession* session,int offline_time,int offline_time_valide,int offline_time_valide_max,int offline_exp,int price1,int price2,int price3);
	virtual void notifyTeamInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* src);
	virtual void notifyUpdateTick(OGSLib::GameSession* session);
	//virtual void resInfoItemExchange(OGSLib::GameSession* session,int pos,int type_id,int res,const std::string* msg);
	//virtual void resItemExchange(OGSLib::GameSession* session,int pos,int type_id,int res);
	virtual void notifyProsperityChange(OGSLib::GameSession* session,int prosperity,int next);
	virtual void notifyFriendChange(OGSLib::GameSession* session,OGSLib::SubFriend &sf);
	virtual void notifyFriendApply(OGSLib::GameSession* session,const char* name);
	virtual void notifyFriendFresh(OGSLib::GameSession* session);
	//virtual void resBoothAdd(OGSLib::GameSession* session,int position,int type_id,int boothtype,int boothprice);
	//virtual void resBoothClose(OGSLib::GameSession* session);
	virtual void notifyNameLoad(OGSLib::GameSession* session);
	//virtual void notifyEnterMarryInvite(OGSLib::GameSession* session,const char* target);
	//virtual void notifyMarryInvite(OGSLib::GameSession* session,const char* male_name,const char* male_desp);
	//virtual void notifyMarrySuc(OGSLib::GameSession* session,int couple_id,int weddingdate,const char* male_name,const char* male_desp,const char* female_name,const char* female_desp);
	//virtual void notifyDivorceInvite(OGSLib::GameSession* session,const char* male_name);
	//virtual void notifyFreeDirectFly(OGSLib::GameSession* session);
	virtual void notifyGotoEnd(OGSLib::GameSession* session,const char* target);
	virtual void notifySlaveState(OGSLib::GameSession* session,int alive);
	virtual void notifyCapacityChange(OGSLib::GameSession* session,int change_capacity);
	virtual void notifyTotalAttrInfo(OGSLib::GameSession* session);
	//virtual void notifyLoginItemList(OGSLib::GameSession* session);
	virtual void notifyGiftList(OGSLib::GameSession* session,std::string msg);
	virtual void notifyCollectBreak(OGSLib::GameSession* session);
	virtual void notifyCollectEnd(OGSLib::GameSession* session, int src_id);
	virtual void notifyParamData(OGSLib::GameSession* session,int srcid,int id,std::string desp);
	virtual void notifyParamDataList(OGSLib::GameSession* session,OGSLib::DomainPlay* player);
	virtual void notifyShowProgressBar(OGSLib::GameSession* session,int duration,const char* msg);
	virtual void notifyPushLuaTable(OGSLib::GameSession* session,std::string type,int flag,std::string table);
	virtual void notifyMails(OGSLib::GameSession* session,OGSLib::DomainPlay* player);
	virtual void notifyNewMailNum(OGSLib::GameSession* session,int num);
	virtual void notifyMailReceiveSuccess(OGSLib::GameSession* session,LEUD_INT64 mailID);
	virtual void notifyInfoPlayItem(int src_id,OGSLib::SubItem* item,int pos);
	virtual void notifyChargeDartList(OGSLib::GameSession* session);
	virtual void notifyConsignItem(OGSLib::GameSession* session, int ret);
	virtual void notifyBuyConsignableItem(OGSLib::GameSession* session, int ret, int consign_seed_id);
	virtual void notifyTakeBackConsignableItem(OGSLib::GameSession* session, int ret, int consign_seed_id);
	virtual void notifyConsignableItems(OGSLib::GameSession* session, int type, int job, int condition, std::vector<OGSLib::ConsignableItemInfo*>& vec, std::vector<int>& list_index);
	virtual void notifyTakeBackVCoin(OGSLib::GameSession* session, int ret);
	virtual void notifyKuafuInfo(OGSLib::GameSession* session,const char* ip,const char* port,const char* param);
	virtual void notifyKuafuEnterMainServer(OGSLib::GameSession* session,const char* result);
	virtual void notifyKuafuComeBackData(OGSLib::GameSession* session,const std::string& winArea,const std::string& winGuild,const char* loginid,const char* charid,const char* seekName,int vcoin,int result);
	virtual void notifyMonExpHiterChange(OGSLib::GameSession* session,int src_id, int hiter_id, const char * name);

	void OnListCharacter(GS_Client::ListCharacterRes& res);
	void OnCreateCharacter(GS_Client::CreateCharacterRes& res);
	void OnDeleteCharacter(GS_Client::DeleteCharacterRes& res);

	void OnAuthResult(int result,const std::string& sessionID,std::string& account,int tim);
	void OnUpdateTicket(int result);
private:
	template<class T> void SendMsg(T& res)
	{
		PERF_NODE_FUNC();
		if( m_Socket == INVALID_SOCKET )
		{
			return;
		}
		if( ! mNetCloseBreak )
		{
#ifdef _PERFORMANCE
			m_app->IncOutMsgCount();
			m_app->LogOutMsg(res.MSG_ID,Send_Message(res));
#else
			Send_Message(res);
#endif
		}
	}
};
