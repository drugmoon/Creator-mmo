module(..., package.seeall)
--雕像npc
util.use_function(_M,task.task1000);

local ACTIVITY_NAME = '膜拜城主';
function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

function onTalk100(npc,player)
		local task_state = task.task1000.get_npc_flags(npc,player);
		if task_state > 0 then
			task.task1000.show_task(npc,player);return;
		end
	-- if gui.PanelActivityGuild.isActivityOpen(ACTIVITY_NAME) then
		player:push_lua_table("open", util.encode({name = "extend_worship"}))
	-- else
	-- 	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str="不倒翁虽然不会摔倒，但也因此不会前进一步。 "}));
	-- end
end

local worshipTimesAward = {
	[1] = {exp = 300000*1,	times = 1,	percent = 0		,probability = 100,	needCoin = 50000},
	[2] = {exp = 300000*3,	times = 3,	percent = 33	,probability = 50,	needCoin = 50000},
	[3] = {exp = 300000*6,	times = 6,	percent = 66	,probability = 35,	needCoin = 50000},
	[4] = {exp = 300000*10,	times = 10,	percent = 100	,probability = 20,	needCoin = 50000},
}

function autoCheckTimes(player)
	local startTime = server.get_var("worshipStartTime")
	if startTime and startTime~="" then
		local lastTime = player:get_param(const.PP_LAST_WORSHIP_TIME)
		if lastTime~=startTime then
			player:set_param(const.PP_LAST_WORSHIP_TIME,startTime)
			player:set_param(const.PP_KING_CITY_GUILD_WORSHIP_TIMES,const.EVERYDAY_MAX_WORSHIP_TIMES)
		end
	end
end

function freshWorship( player )
	autoCheckTimes(player)
	-- local kingInfo = server.get_var("kingInfo")
	-- local kingtab = util.decode(kingInfo) or {}
	local king_guild = server.get_king_guild()
	local kingGuild
	if king_guild and king_guild ~="" then
		kingGuild = server.find_guild(king_guild)
	end
	local result = {}
	result.cmd = "worshipPanel"
	if kingGuild and server.is_war()~=1 then
		local kingInfo = {kingGuild:get_admin_info()}
		result.guild = king_guild
		result.name = kingInfo[2];
		result.cloth = kingInfo[3];
		result.weapon = kingInfo[4];
		result.wing = kingInfo[5];
		result.gender = kingInfo[6];

		result.ownerGuild = player:get_guild()
	end
	result.citybishi = server.get_var("citybishi")
	result.citymobai = server.get_var("citymobai")
	result.activity_time = gui.PanelActivityGuide.getNextOpenTimeString(ACTIVITY_NAME)
	result.expAward = worshipTimesAward
	result.worshipTimes = util.ppn(player, const.PP_KING_CITY_GUILD_WORSHIP_TIMES) --膜拜次数
	result.expTimes = util.ppn(player, const.PP_WORSHIP_EXP_TIMES) --经验倍率
	if result.expTimes ==0 then
		result.expTimes = 1
		player:set_param(const.PP_WORSHIP_EXP_TIMES,1)
	end
	result.needcoin = worshipTimesAward[result.expTimes].needCoin.."绑定金币"
	result.percent = worshipTimesAward[result.expTimes].percent
	result.awardget = checkint(server.get_var("warAward"))
	result.showEffect = isActivityOpen() and result.worshipTimes>0
	player:push_lua_table("PanelWorship",util.encode(result))
end

function isActivityOpen()
	return gui.PanelActivityGuide.isActivityOpen(ACTIVITY_NAME)
end

function showConfirm(player,cmd)
	local result = {
		book = util.encode({cmd = cmd}),
		str = "经验奖励未达到最大倍数，是否继续？",
		labelConfirm = "确定",labelCancel = "取消",
		callFunc = "npc.statue.onPanelData",
	}
	player:push_lua_table("showConfirm",util.encode(result))
end

function bishi(player)
	local citybishi = server.get_var("citybishi")
	citybishi = checknumber(citybishi)
	server.set_var("citybishi",citybishi+1)
end

function mobai(player)
	local citymobai = server.get_var("citymobai")
	citymobai = checknumber(citymobai)
	server.set_var("citymobai",citymobai+1)
end

function worship(player,state)
	local worshipTimes = util.ppn(player, const.PP_KING_CITY_GUILD_WORSHIP_TIMES)
	if worshipTimes >0 then
		local curIndex = util.ppn(player, const.PP_WORSHIP_EXP_TIMES) --倍率
		-- local kingGuild = server.get_king_guild()
		if isActivityOpen() then
			worshipTimes = worshipTimes - 1
			player:set_param(const.PP_KING_CITY_GUILD_WORSHIP_TIMES, worshipTimes)

			if state == 1 then
				mobai(player)
			elseif state == -1 then
				bishi(player)
			end

			player:add_exp(worshipTimesAward[curIndex].exp)

			util.alert_msg(4, "剩余鄙视/膜拜次数:"..worshipTimes.."次", player)
			player:set_param(const.PP_WORSHIP_EXP_TIMES,1)

			freshWorship( player )
		else
			return util.alert_msg(4, "城主膜拜暂未开启，无法膜拜！", player)
		end
	else
		util.alert_msg(4, "今日膜拜次数已用完", player)
	end
end

function onPanelData(player, data)
	local d = util.decode(data)
	if not d then return end
	if d.cmd == "worshipPanel" then
		freshWorship( player )
	elseif d.cmd == "freshexp" then
		local curIndex = util.ppn(player, const.PP_WORSHIP_EXP_TIMES) --倍率
		if curIndex >= #worshipTimesAward then
			return util.alert_msg(4, "已刷到最高倍率", player)
		end
		local success = false
		if player:sub_gamemoney_bind(worshipTimesAward[curIndex].needCoin) ==1 then
			success = true
		else
			local coin = player:get_gamemoney()
			local coinbind = player:get_gamemoney_bind()
			if coin + coinbind >= worshipTimesAward[curIndex].needCoin then
				player:sub_gamemoney_bind(coinbind)
				player:sub_gamemoney(worshipTimesAward[curIndex].needCoin-coinbind)
				success = true
			end
		end
		if success then
			local rand = math.random(1,100)
			if rand <= worshipTimesAward[curIndex+1].probability then
				curIndex = curIndex + 1
				player:set_param(const.PP_WORSHIP_EXP_TIMES,curIndex)
				freshWorship( player )
			end
		else
			util.alert_msg(4, "您绑定金币不足"..worshipTimesAward[curIndex].needCoin.."，无法刷新！", player)
		end
	elseif d.cmd == "ten" then
		local curIndex = util.ppn(player, const.PP_WORSHIP_EXP_TIMES) --倍率
		if curIndex == #worshipTimesAward then
			return util.alert_msg(4, "已刷到最高倍率", player)
		end
		if player:sub_vcoin(100,"刷新膜拜经验")==1 then
			player:set_param(const.PP_WORSHIP_EXP_TIMES,#worshipTimesAward)
			freshWorship(player)
		else
			showChongzhi(player)
		end
	elseif d.cmd == "bishisure" then
		worship(player,-1)
	elseif d.cmd == "bishi" then
		if isActivityOpen() then
			local curIndex = util.ppn(player, const.PP_WORSHIP_EXP_TIMES) --倍率
			if curIndex<#worshipTimesAward then
				showConfirm(player,"bishisure")
				return 
			end
			worship(player,-1)
		else
			util.alert_msg(4, "活动尚未开放，请在活动开始后再来！", player)
		end
	elseif d.cmd == "mobaisure" then
		worship(player,1)
	elseif d.cmd == "mobai" then
		if isActivityOpen() then
			local curIndex = util.ppn(player, const.PP_WORSHIP_EXP_TIMES) --倍率
			if curIndex<#worshipTimesAward then
				showConfirm(player,"mobaisure")
				return 
			end
			worship(player,1)
		else
			util.alert_msg(4, "活动尚未开放，请在活动开始后再来！", player)
		end
	elseif d.cmd == "getdailyaward" then
		local kingInfo = server.get_var("kingInfo")
		kingInfo = util.decode(kingInfo)
		if kingInfo and kingInfo.guild then
			local guild = server.find_guild(kingInfo.guild)
			if guild and guild:get_leader_name() == player:get_name()  then
				if server.get_var("warAward") ~= "1" then
					if os.date("%H",os.time()) == "21" then
						server.set_var("warAward",1)

						player:add_vcoin_best_enable(2000,"皇城帮主每日元宝")
					else
						util.alert_msg(4, "每日21:00-22:00可领取城主元宝奖励！", player)
					end
				else
					util.alert_msg(4, "每日元宝已领取！", player)
				end
			else
				util.alert_msg(4, "您不是皇城城主，无法领取！", player)
			end
		end
	end
end

ckpanel.add_listener("npc.statue.onPanelData",npc.statue.onPanelData)
