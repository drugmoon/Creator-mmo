#pragma once
#include "leudgrid/MemoryPool.h"

#include <list>

namespace OGSLib
{
	class GameServerLib;
	class GameSession;
	class DomainData;
	struct MiniPlayerInfo;
	class DomainPlay;
	class PlayMap;
	class PlayMapItem;
	class PlayMapObject;
	class PlayNpc;
	class PlayRobot;
	class SubItem;
	class SubSkill;
	class PlayActiveObject;
	class Guild;
	class Group;
	//class DomainCouple;
	class DomainGroup;
	class DomainGuild;
	class PlayMonster;
	class SaleNode;
	class SubFriend;
	class ConsignableItemInfo;
	struct GuildRedPacketRecord;
	struct GuildItemOpRecord;

	class GameSessionListener
	{
	public:
		virtual std::string GetKuafuArea()=0;
	public:
		virtual void LoadKuafuCharacter(OGSLib::GameSession* session,const char* loginid,const char* charid,const char* seedname)=0;
		// error: session == 0
		// error result: 1 µÇÂ¼·½Ê½´íÎó 2 ÖØ¸´µÇÂ¼ 3 ÔØÈë´íÎó
		virtual void notifySessionCreateResult(OGSLib::GameSession* session,int result)=0;
		virtual void notifySessionDataLoaded(OGSLib::GameSession* session,int result)=0;
		virtual void notifyCharacterLoad(OGSLib::GameSession* session)=0;
		virtual void notifyKuafuSetGsParam(OGSLib::GameSession* session,const char* k,const char* v)=0;
		virtual void notifyTiliChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src)=0;
		virtual void notifyListItemChange(OGSLib::GameSession* session,OGSLib::DomainPlay* src)=0;
		virtual void notifyItemChange(OGSLib::GameSession* session,OGSLib::SubItem item,int show_flags)=0;
		virtual void notifyItemPanelFresh(OGSLib::GameSession* session,int panel)=0;
		virtual void notifySkillChange(OGSLib::GameSession* session,OGSLib::SubSkill skill)=0;
		virtual void notifyAttributeChange(OGSLib::GameSession* session) = 0;
		//virtual void notifyUpGradeInfoChange(OGSLib::GameSession* session,int arry[14]) = 0;
		//virtual void notifyUpGradeWeaponInfo(OGSLib::GameSession* session, int arry[14]) = 0;
		virtual void notifyGameMoneyChange(OGSLib::GameSession* session,LEUD_INT64 gm_change,int vc_change,LEUD_INT64 gmb_change,int vcb_change)=0;
		virtual void notifyHPMPChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int hp_change,int mp_change)=0;
		virtual void notifyTaskChange(OGSLib::GameSession* session,int task_id,int flags)=0;
		virtual void notifyListItemPlusDesp(OGSLib::GameSession* session)=0;
		virtual void notifyListUpgradeDesp(OGSLib::GameSession* session)=0;
		virtual void notifyListItemDesp(OGSLib::GameSession* session)=0;
		virtual void notifyItemDesp(OGSLib::GameSession* session,int item_id)=0;
		virtual void notifySkillDefine(OGSLib::GameSession* session,int skill_type, int skill_level)=0;
		virtual void notifyExpChange(OGSLib::GameSession* session,int change_value)=0;
		virtual void notifyLevelChange(OGSLib::GameSession* session)=0;
		virtual void resTaskDesp(OGSLib::GameSession* session,int task_id,short task_state,const char* desp)=0;
		virtual void notifyNPCShowFlags(OGSLib::GameSession* session,unsigned int npc_id,int flags)=0;
		virtual void resInfoPlayer(OGSLib::GameSession* session,OGSLib::DomainPlay* player,MiniPlayerInfo* info)=0;
		virtual void notifyNameLoad(OGSLib::GameSession* session)=0;
		virtual void notifySessionEnd(OGSLib::GameSession* session)=0;
		virtual void notifyMapEnter(OGSLib::GameSession* session,OGSLib::PlayMap* map)=0;
		virtual void notifyMiniNpc(OGSLib::GameSession* session,OGSLib::PlayMap* map)=0;
		virtual void notifyMapMonGen(OGSLib::GameSession* session,OGSLib::PlayMap* map)=0;
		virtual void notifyMeetItem(OGSLib::GameSession* session, OGSLib::PlayMapItem* map) = 0;
		virtual void notifyMapMeet(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src,int flags)=0;
		virtual void notifyMapLevae(OGSLib::GameSession* session,OGSLib::PlayMap* map)=0;
		virtual void notifyMapBye(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src)=0;
		virtual void notifyGhostGuildInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* src) = 0;
		virtual void notifyDoAction(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int action)=0;
		virtual void notifySpeed(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,float speed)=0;
		virtual void notifyWalk(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir,int x,int y)=0;
		virtual void notifyRun(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir,int x,int y)=0;
		virtual void resWalk(OGSLib::GameSession* session,int dir,int x,int y,int tag)=0;
		virtual void resRun(OGSLib::GameSession* session,int dir,int x,int y,int tag)=0;
		virtual void resSkill(OGSLib::GameSession* session,int result,int skill_type)=0;
		virtual void notifyTurn(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int dir)=0;
		virtual void notifyAttack(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int param)=0;
		virtual void notifyInjury(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int attacker,int change,int delay,int effect,int change_power)=0;
		virtual void notifyAttackMiss(OGSLib::GameSession* session,OGSLib::PlayMapObject* des)=0;
		virtual void notifyDie(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int delay)=0;
		virtual void notifyAvatarChange(OGSLib::GameSession* session,OGSLib::PlayMapObject* src)=0;
		virtual void notifyUseSkill(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int skill_type,int paramX,int paramY,unsigned int paramID,int resID,int dir, int skill_level)=0;
		virtual void notifyForceMove(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int x,int y,int dir)=0;
		virtual void notifyMapChat(OGSLib::GameSession* session,OGSLib::PlayMap* map,OGSLib::PlayMapObject* src,const char* msg)=0;
		virtual void notifyWorldChat(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const char* msg)=0;
		virtual void notifyHornChat(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const char* msg)=0;
		virtual void notifyPrivateChat(OGSLib::GameSession* session,OGSLib::DomainPlay* src,const char* msg)=0;
		virtual void notifyNormalChat(OGSLib::GameSession* session,OGSLib::DomainPlay* src,const char* msg)=0;
		virtual void notifyNpcTalk(OGSLib::GameSession* session,int src_id,int flags,int param,const char* title,const std::string& msg)=0;
		virtual void notifyBlackBoard(OGSLib::GameSession* session,int flags,const char* title,const std::string& msg)=0;
		virtual void notifyItemTalk(OGSLib::GameSession* session,OGSLib::DomainPlay* player,int item_id,const char* title,const std::string& msg,int seed)=0;
		virtual void notifyPlayerTalk(OGSLib::GameSession* session,OGSLib::DomainPlay* player,const std::string& msg,int seed)=0;
		virtual void notifyNpcShop(OGSLib::GameSession* session,OGSLib::PlayNpc* npc,int page,std::string msg)=0;
		virtual void notifyLoadShortcut(OGSLib::GameSession* session)=0;
		virtual void notifyGuildInfoChange(OGSLib::GameSession* session)=0;
		virtual void notifyGuildChat(OGSLib::GameSession* session,OGSLib::Guild* gld,OGSLib::DomainGuild* src,const char* msg)=0;
		virtual void notifyGuildtitle(OGSLib::GameSession* session,const char* guild_name,int title)=0;
		virtual void notifyGuildMemberChange(OGSLib::GameSession* session)=0;
		virtual void notifyGuildRedPacketLogRes(OGSLib::GameSession* session,std::list<GuildRedPacketRecord>& log_list)=0;
		virtual void notifyGuildItemLogRes(OGSLib::GameSession* session,std::list<GuildItemOpRecord>& log_list)=0;
		virtual void notifyGuildRedPacketLog(OGSLib::GameSession* session,GuildRedPacketRecord * log) = 0;
		virtual void notifyListGuildItem(OGSLib::GameSession* session)=0;
		virtual void notifyGuildItem(OGSLib::GameSession* session,const char* guild_name,int mnum,const char* desp,int mlv,int entering,std::string leader_name,int guild_level)=0;
		virtual void notifyGuildWar(OGSLib::GameSession* session)=0;
		virtual void notifyGhostMode(OGSLib::GameSession* session,int mode)=0;
		virtual void notifyAlert(OGSLib::GameSession* session,int lv,int flags,const char* msg)=0;
		virtual void notifyRelive(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int type)=0;
		virtual void resVcoinShopList(OGSLib::GameSession* session,unsigned int shop_npc_id)=0;
		virtual void notifyGroupInfoChange(OGSLib::GameSession* session,OGSLib::Group* g)=0;
		virtual void notifyGroupState(OGSLib::GameSession* session,OGSLib::Group* g)=0;
		virtual void notifyGroupChat(OGSLib::GameSession* session,OGSLib::Group* g,OGSLib::DomainGroup* domain,const char* msg)=0;
		virtual void resChangeAttackMode(OGSLib::GameSession* session,int attack_mode)=0;
		virtual void notifyGroupInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* player,const std::string& group_name,const std::string& group_leader)=0;
		virtual void notifyJoinGroupToLeader(OGSLib::GameSession* session,const std::string& player_name)=0;
		virtual void notifyInviteGroupToMember(OGSLib::GameSession* session,const std::string& leader_name,int group_id)=0;
		virtual void notifyFindRoadGoto(OGSLib::GameSession* session,const char* map,int x,int y,const char* target,int flags)=0;
		virtual void notifyTradeInvite(OGSLib::GameSession* session,OGSLib::DomainPlay* player)=0;
		virtual void notifyTradeInfo(OGSLib::GameSession* session,int gamemoney,int vcoin,bool submit,const std::string& des_name,int des_level,int des_gamemoney,int des_vcoin,bool des_submit,int tarde_result = 0)=0;
		virtual void notifyTradeItemChange(OGSLib::GameSession* session,int side,OGSLib::SubItem item,int show_flags)=0;
		virtual void notifyPKStateChange(OGSLib::GameSession* session,OGSLib::DomainPlay* player)=0;
		virtual void notifyStatusChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int status_id)=0;
		virtual void notifyStatusDesp(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src,int status_id,int begin=0,int end=0)=0;
		virtual void notifyCountDown(OGSLib::GameSession* session,int delay,std::string& msg)=0;
		virtual void notifyPlayerAddInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* play)=0;
		virtual void notifyPlayEffect(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,int skill_type,int x,int y,unsigned int targetID,int ownerID,int resID)=0;
		virtual void notifySessionClosed(OGSLib::GameSession* session,const std::string& msg)=0;
		virtual void notifyWarInfo(OGSLib::GameSession* session,int var_state,const std::string& king_guild)=0;
		virtual void notifyMapOption(OGSLib::GameSession* session,OGSLib::PlayMap* map)=0;
		virtual void notifySlotAdd(OGSLib::GameSession* session)=0;
		virtual void notifyBuffDesp(OGSLib::GameSession* session,int id)=0;
		virtual void notifyBuffChange(OGSLib::GameSession* session,OGSLib::PlayActiveObject* src, int id, int op_code, double time_left)=0;
		//virtual void resMergeSteel(OGSLib::GameSession* session,int result)=0;
		//virtual void resUpgradeEquip(OGSLib::GameSession* session,int result)=0;
		//virtual void resEquipExchangeUpgrade(OGSLib::GameSession* session,int result)=0;
		virtual void resProtectItem(OGSLib::GameSession* session,int result)=0;
		virtual void notifyNameAdd(OGSLib::GameSession* session,OGSLib::PlayMapObject* src,const std::string& pre,const std::string& pro)=0;
		virtual void notifySetModel(OGSLib::GameSession* session,unsigned int src_id,int id,int vl)=0;
		virtual void notifyChinaLimit(OGSLib::GameSession* session,int china_limit_lv,int online_time_today,int china_id)=0;
		virtual void notifyMonsterChat(OGSLib::GameSession* session,OGSLib::PlayMonster* src,const char* msg)=0;
		virtual void notifyOfflineExpInfo(OGSLib::GameSession* session,int offline_time,int offline_time_valide,int offline_time_valide_max,int offline_exp,int price1,int price2,int price3)=0;
		virtual void notifyTeamInfo(OGSLib::GameSession* session,OGSLib::DomainPlay* src)=0;
		virtual void notifyUpdateTick(OGSLib::GameSession* session)=0;
		//virtual void resInfoItemExchange(OGSLib::GameSession* session,int pos,int type_id,int res,const std::string* msg)=0;
		//virtual void resItemExchange(OGSLib::GameSession* session,int pos,int type_id,int res)=0;
		virtual void notifyProsperityChange(OGSLib::GameSession* session,int prosperity,int next)=0;
		virtual void resHornChat(OGSLib::GameSession* session,const char* msg)=0;
		virtual void notifyFriendChange(OGSLib::GameSession* session,OGSLib::SubFriend &sf)=0;
		virtual void notifyFriendApply(OGSLib::GameSession* session,const char* name) = 0;
		virtual void notifyFriendFresh(OGSLib::GameSession* session)=0;
		virtual void notifyPushLuaTable(OGSLib::GameSession* session,std::string type,int flag,std::string table)=0;

		virtual void notifyGotoEnd(OGSLib::GameSession* session,const char* target)=0;
		virtual void notifySlaveState(OGSLib::GameSession* session,int alive)=0;
		//virtual void notifyLoginItemList(OGSLib::GameSession* session)=0;
		virtual void notifyCapacityChange(OGSLib::GameSession* session,int change_capacity)=0;
		virtual void notifyTotalAttrInfo(OGSLib::GameSession* session)=0;
		virtual void notifyGiftList(OGSLib::GameSession* session,std::string msg)=0;
		virtual void notifyCollectBreak(OGSLib::GameSession* session)=0;
		virtual void notifyCollectEnd(OGSLib::GameSession* session, int src_id)=0;
		virtual void notifyParamData(OGSLib::GameSession* session,int srcid,int id,std::string desp)=0;
		virtual void notifyParamDataList(OGSLib::GameSession* session,OGSLib::DomainPlay* player)=0;
		virtual void notifyShowProgressBar(OGSLib::GameSession* session,int duration,const char* msg)=0;
		virtual void notifyMails(OGSLib::GameSession* session,OGSLib::DomainPlay* player)=0;
		virtual void notifyNewMailNum(OGSLib::GameSession* session,int num)=0;
		virtual void notifyMailReceiveSuccess(OGSLib::GameSession* session,LEUD_INT64 mailID)=0;
		virtual void notifyChargeDartList(OGSLib::GameSession* session)=0;
		virtual void notifyConsignItem(OGSLib::GameSession* session, int ret)=0;
		virtual void notifyBuyConsignableItem(OGSLib::GameSession* session, int ret, int consign_seed_id)=0;
		virtual void notifyTakeBackConsignableItem(OGSLib::GameSession* session, int ret, int consign_seed_id)=0;
		virtual void notifyTakeBackVCoin(OGSLib::GameSession* session, int ret)=0;
		virtual void notifyConsignableItems(OGSLib::GameSession* session, int type, int job, int condition, std::vector<OGSLib::ConsignableItemInfo*>& vec, std::vector<int>& list_index)=0;
		virtual void notifyKuafuInfo(OGSLib::GameSession* session,const char* ip,const char* port,const char* param)=0;
		virtual void notifyKuafuEnterMainServer(OGSLib::GameSession* session,const char* result)=0;
		virtual void notifyKuafuComeBackData(OGSLib::GameSession* session,const std::string& winArea,const std::string& winGuild,const char* loginid,const char* charid,const char* seekName,int vcoin,int result)=0;
		virtual void notifyMonExpHiterChange(OGSLib::GameSession* session,int src_id, int hiter_id, const char * name) = 0;
	};
	class GameSession : public LEUD::PoolMemoryBase< GameSession >
	{
	public:
		enum SessionState
		{
			SESSION_STATE_NULL=100,
			SESSION_STATE_LOADING=101,
			SESSION_STATE_RUN=102,
			SESSION_STATE_SAVEING=103,
			SESSION_STATE_END=104,
		};
	private:
		friend DomainPlay;
		GameServerLib* mGameServerLib;
		char mLoginID[256];
		char mCharID[256];
		char mSeedID[256];
		int mAuthType;
		int mNewYouKe;
		char mClientIP[256];

		GameSessionListener* mListener;

		DomainData* mCharacter;
		DomainPlay* mPlayer;
		DomainGroup* mGroup;
		DomainGuild* mGuild;
		//DomainCouple* mCouple;
		SessionState mState;
	public:
		GameSession(GameServerLib* gameserverlib,const char* loginID,const char* charID,const char* seedID,int AutyType,int newYouKe,const char* client_ip);
		virtual ~GameSession();
	public:
		void SetCharacter(DomainData* chr);
		void CreatePlayer();
		void SetListener(GameSessionListener* listener){mListener=listener;}
		GameSessionListener* GetListener(){return mListener;}
		DomainData*	GetCharacter(){return mCharacter;}
		DomainPlay* GetPlayer(){return mPlayer;}
		DomainGroup* GetDomainGroup(){return mGroup;}
		DomainGuild* GetDomainGuild(){return mGuild;}
		//DomainCouple* GetDomainCouple(){return mCouple;}
		SessionState GetState(){return mState;}
		GameServerLib* GetGameServerLib(){return mGameServerLib;}
		const char* LoginID(){return mLoginID;}
		const char* CharID(){return mCharID;}
		const char* SeedID(){return mSeedID;}
		int AuthType(){return mAuthType;}
	public:
		void Update(double time);
		void EnterGame();
		void EndSession();
		void SaveSession();
		void Destroy();
		void BreakNet(const std::string& msg);
		void BreakNetAndEndSession(const std::string& msg);
	};
}