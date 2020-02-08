module(...,package.seeall)

function gx(player)
	player:set_param(const.PP_TEHUI_OPEN_NUM,0)
	player:set_param(const.PP_ACT_OPEN_NUM,0)
	
	if util.ppn(player,const.PP_TODAY) ~= today()  then
		----------------------材料收集--------------
		player:set_param(const.PP_CAILIAO_TIMES,0);

		gui.AwardHall_jingji.sendRankAwards(player)
		----------------------------------------------
		for i=1,12 do
			player:set_param(const["PP_VIP_BUFF"..i],0)
		end
		player:set_param(const.PP_RECYCLE_TOTAL_EXP,0)
		player:set_param(const.PP_YXBIQI_KILLER,0)
		player:set_param(const.PP_YXBIQI_KILLED,0)
		--vip免费复活次数
		player:set_param(const.PP_FREERELIVE_TIMES,tab.Vip.getFreeReliveTimes(player));

		player:set_param(const.PP_BREAKUP_FIRST_CLICK,0)

		player:set_param(const.PP_SHENGWEI_ARRIVR,0);------神威层数
		player:set_param(const.PP_SHENGWEI_ARRIVR_HOUR,0);------神威层数
		-----------夜袭比奇城-------------------------
		player:set_param(const.PP_YXBIQI_SCORE,0)
		player:set_param(const.PP_TXDY_AWARD1,0)
		-----------帮会副本-------------------------
		player:set_param(const.PP_GUILD_FUBEN1,0);----进入标记
		player:set_param(const.PP_GUILD_FUBEN2,0);
		player:set_param(const.PP_GUILD_FUBEN3,0);
		player:set_param(const.PP_GUILD_FUBEN1_AWARD,0);----领奖标记
		player:set_param(const.PP_GUILD_FUBEN2_AWARD,0);
		player:set_param(const.PP_GUILD_FUBEN3_AWARD,0);
		-------------------------------------------------
		player:set_param(const.PP_ENTER_GROUP_MAP1_NUM,0);
		player:set_param(const.PP_ENTER_GROUP_MAP2_NUM,0);
		player:set_param(const.PP_ENTER_GROUP_MAP3_NUM,0);
		player:set_param(const.PP_GETAWARD_GROUP_MAP1,0);
		player:set_param(const.PP_GETAWARD_GROUP_MAP2,0);
		player:set_param(const.PP_GETAWARD_GROUP_MAP3,0);
		-- if player:get_level()>=60 then
		player:set_param(const.PP_ONLINE_DAYS, util.ppn(player, const.PP_ONLINE_DAYS)+1)
		-- end
		player:set_param(const.PP_DAY_CLEAR_BOX,0)	

		player:set_param(const.PP_TODAY,today());

		player:set_param(const.PP_FREE_MOUNT_UP,3)	
		---翅膀---
		-- player:set_param(const.PP_WING_BLESS_VALUE,0)
		player:set_param(const.PP_EXPLORE_FREE,2)
		player:set_param(const.PP_EXPLORE_POS,0)
		player:set_param(const.PP_EXPLORE_MULTI,1)
		player:set_param(const.PP_EXPLORE_FUBEN,0)
		player:set_param(const.PP_EXPLORE_TIMES,0)
		player:set_param(const.PP_EXPLORE_AWARD,0)
		player:set_param(const.PP_LASTZB_FUBEN,0)
		player:set_param(const.PP_LASTJY_FUBEN,0)
		player:set_param(const.PP_LASTZQ_FUBEN,0)
		player:set_param(const.PP_LASTYM_FUBEN,0)
		player:set_param(const.PP_NO_EXP_TIMES,0)
		
		player:set_param(const.PP_DAILY_RECHARGE,0)--每天充值清空
		player:set_param(const.PP_HEQU_DAILY_CZ,0)--合区每天充值清空

		player:set_param(const.PP_DAILY_FIRST_RECHARGE,0)
		player:set_param(const.PP_DAILY_RECHARGE_LING,0)

		player:set_param(const.PP_DAILY_LOGIN_STATE,0)
		-- player:set_param(const.PP_DAILY_LOGIN_LING,0)
		
		--摇摇乐变量每日清理
		player:set_param(const.PP_DAILY_YAO_NUM,0)
		player:set_param(const.PP_DAILY_GY_NUM,0)
		player:set_param(const.PP_JINRI_WUHUN,0)
		player:set_param(const.PP_YYL_SHAI_INFO,util.encode({0,0,0,0,0,0}))--重置点数
		player:set_param(const.PP_YYL_RECORD_WUHUN,0)
		-- player:set_param(const.PP_XINMOTA_CUR_LEVEL,0);
		-- player:set_param(const.PP_XINMOTA_DAILYAWARD,"");

		-- player:set_param(const.PP_ZHENMOTA_TIMES,0);
		-- player:set_param(const.PP_ZHENMOTA_EXP_POOL,0);
		-- player:set_param(const.PP_LIXIAN_TOTAL_TIME,0);
		player:set_param(const.PP_ZHSH_NUM,0);
		player:set_param(const.PP_DUOBAO_NUM,0);
		player:set_param(const.PP_DAILY_DONATION_TIMES,0);

		player:set_param(const.PP_EXP_DAILY_TIMES,0)--每日经验炼制领取次数重置

		for i=0,5 do--开区活动-每日累计充值
			player:set_param(const.PP_LEICHONG_LING1+i,0)
		end
		player:set_param(const.PP_LEICHONG_NUM,0)

		-- player:set_param(const.PP_PERSON_COYE_TIMES1,0)
		-- player:set_param(const.PP_PERSON_COYE_TIMES2,0)
		-- player:set_param(const.PP_PERSON_COYE_TIMES3,0)
		-- player:set_param(const.PP_PERSON_COYE_TIMES4,0)
		-- player:set_param(const.PP_PERSON_COYE_TIMES5,0)
		-- player:set_param(const.PP_PERSON_COYE_TIMES6,0)
		-- player:set_param(const.PP_PERSON_COYE_TIMES7,0)
		for i=0,12 do 
			player:set_param(const.PP_TIMER_INDEX100+i,0)---尸王殿定时器
		end 
		-------------------------------------材料副本次数
		for i=0,5 do
			player:set_param(const.PP_CLFU_TIME1+i,0)
		end
		-- for i=1,5 do
		-- 	player:set_param(const["PP_INVEST_IS_GOT"..i],0)			
		-- end
		-- player:set_param(const.PP_TIMER_INDEX106,0)

		-- player:set_param(const.PP_EXPDAN_USENUM,0)
		
		-- player:set_param(const.PP_YUANSHEN_PASSED_TOWER,"")

		-- for i=0,5 do
		-- 	player:set_param(const.PP_COPY_TIMES_EXP+i,"")
		-- end
		---------------------副本---------------------
		-- player:set_param(const.PP_COPY_TIMES, 0)
		for i = 0, 3 do--在线礼包清空
			player:set_param(const.PP_ONLINE_AWARD_5M+i,0)
		end

		for i = 0, 9 do--每日充值清空
			player:set_param(const.PP_DAILY_RECHARGE+i,0)
		end

		for i = 0,2 do--庆典活动 限时折扣清空
			player:set_param(const.PP_QIANGGOU_BUYNUM1+i,0)
		end

		---------------------VIP副本---------------------
		-- for i=0,2 do
		-- 	player:set_param(const.PP_ACTIVE_VIPROOM1+i,0)
		-- end

		player:set_param(const.PP_CHARGE6_LIMIT,0)

		if tonumber(os.date("%d", os.time())) == 1 then--月初清空签到表
			-- player:set_param(const.PP_SIGN_DATES, "")
			
			player:set_param(const.PP_DAILYSIGNIN_RECEIVE ,0)
			for i=1,31 do
				player:set_param(const["PP_DAILYSIGNIN_CUMULATIVE_"..i] ,0)
			end
		end

		-- player:set_param(const.PP_SIGN_TODAY, 0)

		-- player:set_free_direct_fly(20);

		-----------------烧猪副本相关变量-------------------
		player:set_param(const.PP_SHAOZHU_FREE_TIME, 0)

		-----------------挖矿人物相关变量-------------------
		player:set_param(const.PP_WAKUANG_TASK_TIME, 0)
		player:set_param(const.PP_WAKUANG_TASK_FLAG, 0)
		player:set_param(const.PP_WAKUANG_ACTION_COUNT, 0)
		
		--------------------红点相关变量--------------------
		player:set_param(const.PP_POINT_DAILY_VIP, 0)
		player:set_param(const.PP_POINT_DALIY_RECHARGE, 0)

		------------王城战参与情况清空--------------
		player:set_param(const.PP_CITYWAR_JOIN_STATE,0)

		------------镖车奖励等级刷新--------------
		-- player:set_param(const.PP_DART_LEVEL,0)

		------------每日抽奖状态重置----------------

		player:set_param(const.PP_LOTTERY_FREEDRAWTIME_INGAME,0)
		player:set_param(const.PP_LOTTERY_FREEDRAWTIME_DEGREE,3)

		--------------通知客户端清理交易记录--------------
		player:push_lua_table("clearTradeRecord", player:get_seed_name())
		player:push_lua_table("clearExploitAward", "")

		player:set_param(const.PP_RICHANG_NUM, 0) --降妖除魔
		player:set_param(const.PP_ACTIVE_XUNBAO, 0) --每日寻宝次数
		-- player:set_param(const.PP_COPY_TIMES_EXP, 0)
		-- player:set_param(const.PP_COPY_TIMES_MOUNT, 0)
		-- player:set_param(const.PP_COPY_TIMES_WING, 0)
		-- player:set_param(const.PP_KILL_BOSS_NUM, 0)	--击杀boss次数
		player:set_param(const.PP_RECYCLE_NUM, 0)	--回收装备次数
		-- player:set_param(const.PP_WULIN_JOIN_NUM, 0)--参与武林争霸次数
		-- player:set_param(const.PP_BOSSZJ_TIMES, 0)--BOSS之家进入次数
		-- player:set_param(const.PP_ZI_DUIHUAN_TIMES, 0)--兑换字帖副本进入次数
		-- for i=1,4 do
		-- 	player:set_param(const.PP_ACTIVE_AWARD1+i-1, 0)--每日活跃领奖
		-- end
		-- for i=1,9 do
		-- 	player:set_param(const.PP_LIVENESS_CHUMO+i-1, 0)--每日活跃领奖状态
		-- end

		-- for i=1,11 do
			-- player:set_param(const.PP_ANDIAN_KILL_NUM_STATE+i-1, 0)--每日活跃领奖
			-- player:set_param(const.PP_ANDIAN_KILL_NUM+i-1, 0)--每日活跃领奖状态
		-- end
		-- player:set_param(const.PP_PERSON_BOSS_NUM, 0)--参与个人BOSS次数
		----------记录累计登陆天数----------
		-- gui.PanelAchieve.checkAchieveLogin(player)

		-- local temp = util.ppn(player, const.PP_MAINTASK_BOSS)
		-- if temp > 0 and temp < 2 then
		-- 	map.raid_main_task1.removeEquip(player, "体验武器")
		-- end
		-- if util.ppn(player, const.PP_FUNCINDEX_BY_OPENSVR) < 2 then 
		-- end
		---开服前几天开放指定功能
		
		-- player:set_param(const.PP_ESCORT_REMAIN_TIMES, 0)
		-- player:set_param(const.PP_ROBDART_TIMES, 0)
		-- player:set_param(const.PP_FIRST_WEEK_SIGN_GET, 0)--七日礼包每日只能领取一个礼包

		------------------王城帮会膜拜次数恢复-----------------------
		player:set_param(const.PP_KING_CITY_GUILD_WORSHIP_TIMES, const.EVERYDAY_MAX_WORSHIP_TIMES)
		player:set_param(const.PP_KING_CITY_GUILD_LEADER_AWARD, 0)
		player:set_param(const.PP_KING_CITY_GUILD_MEMBER_AWARD, 0)

		-- for i=0,6 do -- 数据统计相关变量重置
		-- 	player:set_param(const.PP_AUTO_ADD_EXP1 + i, 0)
		-- end

		-- for i=0,3 do --重置打折宝石购买次数
		-- 	player:set_param(const.PP_BUY_GEM1 + i, 0)
		-- end
		-- player:set_param(const.PP_DAY_KILL_NUM, 0)
		-- player:set_param(const.PP_HUANYAO_TIMES, 0)
		
		-- player:set_param(const.PP_REBORN_DAN_NUM, 0)

		-- player:set_param(const.PP_BAOSHI_LIMIT_TIMES, 5)
		-- player:set_param(const.PP_BAOJI_LIMIT_TIMES, 5)
		-- player:set_param(const.PP_BOSSJF_LIMTI_TIMES, 10)
		-- player:set_param(const.PP_ANDIAN_ALL_TIME, 0)
		-- player:set_param(const.PP_ANDIAN_START_TIME,tonumber(os.time()))
		-- player:set_param(const.PP_GUAJI_ALL_TIME, 0)
		-- player:set_param(const.PP_GUAJI_START_TIME,tonumber(os.time()))
		
		-- player:set_param(const.PP_YS_LAST_CHART_AWARD, 0)
		-- player:set_param(const.PP_ONLINE_AWARD_LIMIT,"")

		-- -- player:set_param(const.PP_SUITE_CHARACTER_CONVERT, 0)

		-- for i=0,30 do -- Boss挑战相关变量重置
		-- 	player:set_param(const.PP_BOSS_FIGHT_NUM + i, 0)
		-- end
		-- for i=0, 29 do
		-- 	player:set_param(const.PP_BOSS_SWEEP_SIMPLE1 + i, 0)
		-- end
		-- player:set_param(const.PP_BOSS_SWEEP_SCORE, "")
		
		-- player:set_param(const.PP_XINMOTA_SAODANGAWARD, "")
		-- player:set_param(const.PP_CANGBAOTU_TASK_NUM, 0)
		-- player:set_param(const.PP_CANGBAOTU_USE_NUM, 0)
		-- for i=0,13 do
		-- 	player:set_param(const.PP_VIP_DAILYAWARD0+i, 0);
		-- end
		-- for i=0,5 do
		-- 	player:set_param(const.PP_YUANSHENXIANG_TIMES1+i, 0);
		-- end
		-- player:set_param(const.PP_ZMCBOSS_NUM, 0)
		
		-- player:set_param(const.PP_GAOJI_DAN, 0)
		-- player:set_param(const.PP_CHAOJI_DAN, 0)
		--清理关卡挑战记录
		for i=0,64*6 do
			local contable = player:get_param(const.CUSTOM_PASS_1_1+i)
			contable = util.decode(contable)
			if contable and type(contable)=="table" then
				contable.passtimes = 0
				player:set_param(const.CUSTOM_PASS_1_1+i,util.encode(contable))
			end
		end
		--清理关卡宝箱领取记录
		-- for i=0,15 do
		-- 	player:set_param(const.AWARD_BOX_CON1+i, "000")
		-- end
		-- for i=0,15 do
		-- 	player:set_param(const.PP_BREAKUP_FIRST1+i, "000000")
		-- end

		player:set_param(const.PP_HUWEI_BUYZHU, 0)
		player:set_param(const.PP_HUWEI_BUYJIE, 0)
		player:set_param(const.PP_HUWEI_USENUM, 0)
		player:set_param(const.PP_HUWEI_RUNTIME, 0)	

		player:set_param(const.PP_DAY_GONGXUN_NUM,0)--清理功勋值
		player:set_param(const.PP_GONGXUN_RANK,0)--清理功勋排名
		for i=0,4 do
			player:set_param(const.PP_GONGXUN_GETBOX1+i,0)
		end
		for i=0,17 do
			player:set_param(const.PP_GONGXUN_TASK1+i,0)
			player:set_param(const.PP_GX_TSAK_RECORD1+i,0)
		end

		for i=0,2 do--官职的荣誉和声望购买清0
			player:set_param(const.PP_MAXBUYNUM_SW1+i,0)
			player:set_param(const.PP_MAXBUYNUM_RY1+i,0)
		end
		--累计登录天数
		gui.PanelAchieve.triggerTaskEvent(player, "login")
		player:set_param(const.PP_LOGIN_DAY,util.ppn(player,const.PP_LOGIN_DAY)+1)--累计登录天数
		gui.moduleRedPoint.checkAwardFifteenUsable(player)
		--连续登录天数
		if player:get_last_login_time()-player:get_last_logout_time()<24*60*60 then
			gui.PanelAchieve.triggerTaskEvent(player, "loginContinue")
		else
			gui.PanelAchieve.triggerTaskEvent(player, "loginContinue",1)
		end
		--个人BOSS副本次数每日清0
		for i=0,5 do
			player:set_param(const.PP_BOSS_FUBEN1+i,0)
		end
		for i=0,4 do--清理寻宝每日宝箱获得记录
			player:set_param(const.PP_LOTTERY_BOX1+i,0)
			player:set_param(const.PP_LOTTERY_BOX_LING1+i,0)
		end
		for i=0,5 do--清理每日充值
			player:set_param(const.PP_MEIRI_LING1+i,0)
		end
		for i=0,3 do--每日的在线奖励领取
			player:set_param(const.PP_HALL_ONLINE_LING1+i,0)
		end
		player:set_param(const.PP_LOTTERY_NUM,0)--每日寻宝次数清0
		-- player:set_param(const.PP_DART_FREE_TIME,0)
		player:set_param(const.PP_DART_USE_TIME,0)

		player:set_param(const.PP_JYG_AWARD,0)
		player:set_param(const.PP_TJCF_HONOR_VALUE,0)
		player:set_param(const.PP_DART_HURT_ALERT_TICK,0)

		tab.Store.resetDailyMaxCount(player)
		-- player:set_param(const.PP_STORE_DAILY_BUY_LIMIT1,0)
		-- player:set_param(const.PP_STORE_DAILY_BUY_LIMIT2,0)
		-- player:set_param(const.PP_STORE_DAILY_BUY_LIMIT3,0)
		player:set_param(const.PP_DAILYSIGNIN_RECEIVE_TODAY,0);
		gui.AwardHall_online.setOldWeekAward(player)
		--物品使用次数清零
		for i=1,5 do
			player:set_param(const["PP_DAILY_ITEM_LIMIT"..i],0);
		end
		gui.PanelGongXun.checkTaskComplete(player,1)

		player:set_param(const.PP_REDDOT_EXPLOIT_PANEL, 0);

		player:set_param(const.PP_DAILY_LOGIN_STATE,1)--上线或者0点时更新登录状态
		gui.PanelCelebrationAct.sendRankAwards(player)--发放消费有礼邮件
		----------------------------------------------合区变量清理------------------------------------------------
		if util.ppn(player,const.PP_HEQU_TIME) ~= util.svn("server_merge_time") then
			player:set_param(const.PP_HEQU_TIME,util.svn("server_merge_time"))
			-----合区后设置
			gui.PanelMysteryStore.refreshStore(player)
			for i=0,11 do
				player:set_param(const.PP_LANGYA_USEB_NUM+i,0)
			end
		end

		----------------------------------------------活动变量清理------------------------------------------------
		if util.ppn(player,const.PP_FESTIVAL_INDEX) ~= sever_index() then
			player:set_param(const.PP_DAILY_LOGIN_LING,0)--登录有礼清0
			player:set_param(const.PP_XIAOFEI_RANK,0)--个人活动期间累计消费清0

			player:set_param(const.PP_FESTIVAL_INDEX,sever_index())

		end
		if util.svn("hdindex") ~= sever_index() then
			for n=1,5 do
				server.set_var("qingdian_rank_name_"..n,"")
				server.set_var("qingdian_rank_xf_"..n,0)
			end
			server.set_var("hdindex",sever_index())
		end
		-----------------------------------------------精彩活动------------------------------------------------------------
		gui.PanelJingCaiAct.set_JingCaiVar()
		gui.PanelJingCaiAct.clear_param( player )
		gui.PanelJingCaiAct.updatejsczData( player )--急速充值面板刷
		gui.PanelJingCaiAct.senderToClientTab(player)--叶签刷新
		player:set_param(const.PP_RECHARGEJS_JINGCAI,0)
		player:set_param(const.PP_RECHARGEJS_JINGCAI_LING,0)

		if util.ppn(player,const.PP_JINGCAI_INDEX) ~= sever_index_jingcai() then
			for n=0,5 do
				player:set_param(const.PP_JINGCAI_XF1+n,0)
			end
			for n=0,5 do
				player:set_param(const.PP_JINGCAI_CZ1+n,0)
			end
			player:set_param(const.PP_XIAOFEI_JINGCAI,0)
			player:set_param(const.PP_JINGCAI_INDEX,sever_index_jingcai())

		end
		tab.pushItemLimitToClient(player)
		player:set_param(const.PP_TODAY_JINGCAIACT, get_day_YMD());
		player:set_param(const.PP_DEFEND_TIMES, 0);
	end
	updateCocosGuiState(player)
	updateClientShow(player)
	-------------重置连续登录天数---------
	--gui.AwardExercise.checkContinueLoginDay(player)
	-- ----------主线给的强12武器任务自动完成----------
	if util.ppn(player, const.PP_MAINTASK_BOSS) > 0 and util.ppn(player, const.PP_MAINTASK_BOSS) < 3 then
		task.task1000.check_task_done(player, "个人Boss")
	end

	---------个人技能自动释放变量推送---------
	-- gui.PanelSkill.pushAutoCastSkills(player)

	task.simulateTask.checkTaskLevel(player);
end
function set_bless(player)
	--print("---",util.ppn(player,const.PP_BlESS_FLAG),today_bless())
	if util.ppn(player,const.PP_BlESS_FLAG) ~= today_bless()  then
		local wingLv = util.ppn(player,const.PP_WING_LEVEL) 
		if wingLv>3 then 
			player:set_param(const.PP_WING_BLESS_VALUE, 0);
			player:set_param(const.PP_WING_UP_TIMES, 0);	
		end
		player:set_param(const.PP_BlESS_FLAG,today_bless());
	end
end 
-- server.set_king_guild("77777")

function updateCocosGuiState(player)
	--------------功能开启状态---------------
	local funcState = {
		["menu"] = {},
		["rcpart"] = {},
		["rtpart"] = {},
		["rbpart"] = {},
	}

	local function setFuncIndex(index, funcs)
		if index > 0 then
			for i = 1, index do
				if funcs[i] and funcs[i].mType then
					if funcs[i].func=="extend_lianzhi" then
						if tonumber(server_start_day())<2 then
							funcState[funcs[i].mType][funcs[i].func] = 1
						end
					else
						funcState[funcs[i].mType][funcs[i].func] = 1
					end
				end
			end
		end
	end
	player:push_lua_table("allfunc", util.encode({allfunc = funcState}));

	gui.moduleGuiButton.checkOpenFunc(player);
end

--处理所有跨0点时更新客户端的显示
function updateClientShow(player)
	map.andian.updateTimeShow(player)
	gui.PanelGem.getServerDay(player)
	gui.PanelDailyRecharge.updateClient(player)
end

--重置各种礼包只能使用一次限制
function clearOnceTimesVar(player)
	for i=const.PP_PTHQ1_LIBAO,const.PP_PTHQ1_LIBAO+22 do
	 	player:set_param(i, 0)
	end

end