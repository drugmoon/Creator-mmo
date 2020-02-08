module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local dartInfo =	{
	strs1={
		"<font color=#b2a58b>参与押镖获得大量绑定元宝和经验奖励</font>",
	},
	strs2={
		"<font color=#b2a58b>1. 参与条件：<font color=#30ff00>50级</font>，每日<font color=#30ff00>2次</font></font>",
		"<font color=#b2a58b>2. 活动时间:<font color=#30ff00> 16:00-16:30</font></font>",
		"<font color=#b2a58b>3. 镖车<font color=#30ff00>等级</font>越高奖励越高，</font>",
		"<font color=#b2a58b>   刷新镖车需要消耗<font color=#30ff00>礼券</font>",
		"<font color=#b2a58b>4. 镖车被劫只获得<font color=#30ff00>部分奖励</font>",
		"<font color=#b2a58b>5. 劫杀他人镖车获得<font color=#30ff00>额外奖励</font>",
	},
	resData={talkTitle="日常押镖",btnName = "选择镖车"},
	func = "npc.dart.btnEnter",
}

local dartawardInfo =	{
	strs1="<font color=#b2a58b>押镖任务奖励</font>",
	resData={talkTitle="日常押镖",btnName = "领取奖励",},
	func = "npc.dart.getAward",
	awards = {},
}


function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	if PLATFORM_BANSHU then
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str=npc:get_dialog(),npcName=npc:get_name()}));
	else
		local dart_lv = util.ppn(player,const.PP_DART_SUCCEED_LEVEL)
		if dart_lv > 0 then
			dartawardInfo.awards = gui.PanelDart.getAwardsTable(dart_lv)
			player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="dartaward",talk_str = dartawardInfo}))
			return
		end


		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = dartInfo}));
	end
end

function btnEnter(player,data)
	if get_hour_minute() < 1600 or get_hour_minute() > 1630 then
		player:alert(1,1,"活动时间未到,无法押送镖车！")
		return
	end

	if player:get_level() < 50 then
		player:alert(1,1,"等级不足50级,无法押送镖车！")
		return
	end

	player:push_lua_table("open",util.encode({name = "panel_dart"}))
end

ckpanel.add_listener("npc.dart.btnEnter",npc.dart.btnEnter)

function btnGetAward(player,data)
	gui.PanelDart.getDartAwards(player, util.ppn(player,const.PP_DART_SUCCEED_LEVEL), 1)
	player:set_param(const.PP_DART_SUCCEED_LEVEL,0);

	player:push_lua_table("npc_echo",util.encode({id=nil, panelType="common",talk_str = dartInfo}));
end

ckpanel.add_listener("npc.dart.getAward",npc.dart.btnGetAward)
