module(..., package.seeall)
util.use_function(_M,task.task1000);
util.use_function(_M,task.task2000);


function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	if flags < 1 then
		local flagB = task.task2000.get_npc_flags(npc,player);flagB=task.util.fix_npc_flags(flagB);
		if flagB > 0 then
			flags = flagB;
		end
	end
	player:show_npc_flags(npc,flags);
end

local task_id=const.TASK_ID_RICHANG;
local newVersion = true;

function onTalk100(npc,player)
	-- print("////////////////////////onTalk100////////////////////////")
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc, player);return;
	end

	if newVersion then
		local s=player:get_task_state(task_id)
		local d=math.fmod(s,const.TASK_STATE_NUM)
		local n = util.ppn(player,const.PP_RICHANG_NUM)
		if n < const.RICHANG_MAX_NUM or d == const.TSCOMP then
			return player:push_lua_table("open", util.encode({name = "panel_chumo"}));
		else
			player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str="你已完成所有降妖除魔！"}))
		end
	end
	-- local s=player:get_task_state(task_id)
	-- local d=math.fmod(s,const.TASK_STATE_NUM)
	-- local n = util.ppn(player,const.PP_RICHANG_NUM)
	-- if n < const.RICHANG_MAX_NUM then
	-- 	if d == const.TSACCE then
	-- 		npc:echo([[
	-- 			可接受<br>
	-- 			当前任务等级：]]..util.ppn(player,const.PP_XYCM_TASKLV)..[[级<br>
	-- 			剩余除魔次数：]]..(const.RICHANG_MAX_NUM - util.ppn(player,const.PP_RICHANG_NUM))..[[次<br>
	-- 			<a href="event:talk_done1">接受任务</a><br>
	-- 			<a href="event:talk_fresh">刷新任务等级(金币)</a><br>
	-- 		]])
	-- 	elseif d == const.TSACED then
	-- 		npc:echo([[
	-- 			已接受<br>
	-- 			<a href="event:talk_sweep">扫荡(需VIP)</a><br>
	-- 			<a href="event:talk_fly">立即前往</a><br>
	-- 		]])
	-- 	elseif d == const.TSCOMP then
	-- 		npc:echo([[
	-- 			已完成<br>
	-- 			<a href="event:talk_done1">领取奖励</a><br>
	-- 			<a href="event:talk_done2">双倍领取</a><br>
	-- 			<a href="event:talk_done3">三倍领取</a><br>
	-- 		]])
	-- 	end
	-- else
	-- 	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str="你已完成所有日常任务！"}))
	-- end
end

function onTalksweep(npc,player)
	-- if gui.PanelVip.getVipValue(player,"subduelichraids") ~= 0 then
	-- 	local s=player:get_task_state(const.TASK_ID_RICHANG)
	-- 	local d=math.fmod(s,const.TASK_STATE_NUM)
	-- 	if d == const.TSACED then
	-- 		player:set_task_state(const.TASK_ID_RICHANG,task.util.ts(d,const.TSCOMP))
	-- 	end
	-- else
	-- 	player:alert(1,1,"您不是VIP，无法一键扫荡！");
	-- end
	onTalk100(npc,player)
end

function onTalkdone1(npc,player) task.task2000.task_done(npc,player) onTalk100(npc,player) end
function onTalkdone2(npc,player)
	if 1==player:sub_gamemoney(50000) then
		player:alert(1,1,"使用[50000金币]成功领取双倍经验奖励!");
		player:set_param(const.PP_RICHANG_LV,2);
		player:set_param(const.PP_RICHANG_LV,2);
		task.task2000.task_done(npc,player)
	else
		player:alert(1,1,"您的金币不足50000,无法领取双倍经验奖励!");
	end
	onTalk100(npc,player)
end
function onTalkdone3(npc,player)
	if 1==player:sub_vcoin(50,"除魔三倍领取") then
		player:alert(1,1,"使用[50元宝]成功领取三倍经验奖励!");
		player:set_param(const.PP_RICHANG_LV,3)
		task.task2000.task_done(npc,player)
	else
		player:alert(1,1,"您的元宝不足50,无法领取三倍经验奖励!");
	end
	onTalk100(npc,player)
end

function onTalkfresh(npc,player)
	if util.ppn(player,const.PP_XYCM_TASKLV) < 10 then
		local dt = math.random(10,109)
		if player:get_gamemoney() >= 20000 then
			player:sub_gamemoney(20000)
			if util.ppn(player,const.PP_RICHANG_NUM) < 2 then
				player:set_param(const.PP_XYCM_TASKLV,10);
			else
				player:set_param(const.PP_XYCM_TASKLV,math.floor(dt/10))
			end
			player:alert(11,1,"刷新成功,当前任务等级:"..util.ppn(player,const.PP_XYCM_TASKLV))
		else
				player:alert(1,1,"您的金币不足20000,无法刷新!");
		end
	else
		player:alert(11,1,"当前任务已满级，无法再次刷新！");
	end
	onTalk100(npc,player)
end

function onTalkfly(npc,player)
	task.task2000.direct_fly_target(npc,player)
end

-----------------------------新版本----------------------------

---------------获取任务状态---------------
local function getTaskState(player)
	local s = player:get_task_state(task_id);
	return math.fmod(s, const.TASK_STATE_NUM);
end

---------免费次数用完需要花费99元宝接受任务
local function getAcceptTaskCost(player)
	local cost = (util.ppn(player,const.PP_RICHANG_NUM) >= const.RICHANG_FREE_NUM) and 99 or 0;
	return cost;
end

local function getTaskAwardDesp(player)
	return {exp = 10000, reputation = 10000};
end

local function canRefreshTask(player)
	if getTaskState(player) == const.TSACCE and util.ppn(player,const.PP_XYCM_TASKLV) < 10 then
		return true;
	end
	return false;
end

local function updatePanel(player)
	local s = player:get_task_state(task_id);
	local d = math.fmod(s, const.TASK_STATE_NUM);
	local n = util.ppn(player, const.PP_RICHANG_NUM);
	local result = {
		npcName = "除魔使者",
		taskState = d,
		taskLevel = util.ppn(player,const.PP_XYCM_TASKLV),
		timesRemain = const.RICHANG_MAX_NUM - n,
		acceptCost = getAcceptTaskCost(player),
		canRefresh = canRefreshTask(player),
		taskAward = task.task2000.get_award_desp(player),
		taskDesp = "<font color=#dfcdaa>    "..task.task2000.get_task_desp(player).."</font>",
	};

	player:push_lua_table("PanelChuMo",util.encode(result));
end

local awardTable = {
	[1] = {gamemoney_bind = 0,},
	[2] = {gamemoney_bind = 300000,},
	[3] = {gamemoney_bind = 600000,},
}

function isTaskFree(player)
	return util.ppn(player,const.PP_RICHANG_NUM) < const.RICHANG_FREE_NUM
end
-----------------------领取任务奖励-------------------------
local function getSlayerAward(player, times)
	times = times or 1;
	local conf = awardTable[times];
	if conf then
		local canGet = false;
		local alertMsg = nil;
		if conf.gamemoney_bind then
			if conf.gamemoney_bind > 0 then
				if 1==player:sub_gamemoney_bind(conf.gamemoney_bind) then
					alertMsg = "使用["..conf.gamemoney_bind.."绑金]成功领取"..times.."倍经验奖励!";
					canGet = true;
				else
					alertMsg = "您的绑金不足"..conf.gamemoney_bind..",无法领取"..times.."倍经验奖励!";
				end
			else
				canGet = true;
			end
			if canGet then
				player:set_param(const.PP_RICHANG_LV, times);
				task.task2000.task_done("", player);
				-- player:push_lua_table("close", util.encode({name = "panel_chumo"}));
				if util.ppn(player, const.PP_RICHANG_NUM) < const.RICHANG_MAX_NUM and player:get_level() >= 80 then
					player:push_lua_table("open", util.encode({name = "panel_chumo"}));
				else
					player:push_lua_table("close", util.encode({name = "panel_chumo"}));
				end
			end
			if alertMsg then
				player:alert(1, 1, alertMsg);
			end
		end
	end
end
-----------------------继续任务-------------------------
local function continueTask(player)
	-- print("///////////////////continueTask///////////////////");
	task.task2000.direct_fly_target("",player)
	player:push_lua_table("close", util.encode({name = "panel_chumo"}));
end
-----------------------接受除魔任务-------------------------
local function acceptTask(player)
	local d = getTaskState(player);
	if d == const.TSCOMP then
		player:alert(11,1,"请先领取上次降妖的奖励!");
	elseif d == const.TSACED then
		continueTask(player);
	elseif d == const.TSACCE then
		if util.ppn(player, const.PP_RICHANG_NUM) < const.RICHANG_MAX_NUM then
			local canAccepted = false;
			if isTaskFree(player) then
				canAccepted = true;
			else
				if player:sub_vcoin(99,"降妖除魔") == 1 then
					canAccepted = true;
				else
					if not checkFirstCharge(player) then
						player:alert(1, 1, "免费次数用完需要花费99元宝接受任务");
					end
				end
			end
			if canAccepted then
				task.task2000.task_done("", player);
				task.task2000.direct_fly_target("", player);
			end
		else
			player:alert(1, 1, "你已完成所有降妖除魔！");
		end
	else
		player:alert(1, 1, "当前任务不可接!");
	end
	player:push_lua_table("close", util.encode({name = "panel_chumo"}));
end

-----------------------刷新除魔任务-------------------------
local function freshTask(player)
	if getTaskState(player) ~= const.TSACCE then
		return player:alert(1, 1, "当前状态不可刷新任务等级!");
	end
	if util.ppn(player,const.PP_XYCM_TASKLV) < 10 then
		local dt = math.random(10,109);
		local num = 10000
		local money = player:get_gamemoney()
		local bindMoney = player:get_gamemoney_bind()
		local success = true
		if bindMoney >= num then
			player:sub_gamemoney_bind(num)
		elseif bindMoney + money >= num then
			player:sub_gamemoney_bind(bindMoney)
			player:sub_gamemoney(num-bindMoney)
		else
			success = false
		end

		if success then
			task.task2000.fresh_task_level(player);
			updatePanel(player);
		else
			player:alert(1,1,"您的绑金和金币不足10000,无法刷新!");
		end
		-- if 1==player:sub_gamemoney_bind(10000) then
		-- 	-- if util.ppn(player,const.PP_RICHANG_NUM) < 2 then
		-- 	-- 	player:set_param(const.PP_XYCM_TASKLV,10);
		-- 	-- else
		-- 		-- player:set_param(const.PP_XYCM_TASKLV,math.floor(dt/10));
		-- 	-- end
		-- 	task.task2000.fresh_task_level(player);
		-- 	-- player:alert(11,1,"刷新成功,当前任务等级:"..util.ppn(player,const.PP_XYCM_TASKLV));
		-- 	updatePanel(player);
		-- else

		-- 	player:alert(1,1,"您的绑金不足10000,无法刷新!");
		-- end
	else
		player:alert(11,1,"当前任务已满级，无法再次刷新！");
	end
end

function onPanelData(player, commond)
	-- print("onPanelData", commond);
	local param = util.decode(commond);
	if param.cmd == "panel" then
		updatePanel(player);
	elseif param.cmd == "acceptTask" then
		acceptTask(player);
	elseif param.cmd == "continueTask" then
		continueTask(player)
	elseif param.cmd == "getAward1" then
		getSlayerAward(player, 1);
	elseif param.cmd == "getAward2" then
		getSlayerAward(player, 2);
	elseif param.cmd == "getAward3" then
		getSlayerAward(player, 3);
	elseif param.cmd == "freshTask" then
		freshTask(player);
	end
end
ckpanel.add_listener("npc.chumo.onPanelData", npc.chumo.onPanelData);