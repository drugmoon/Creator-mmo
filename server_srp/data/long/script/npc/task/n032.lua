module(..., package.seeall)
util.use_function(_M,task.task1000);

local dartawardInfo =	{
	strs1="<font color=#b2a58b>押镖任务奖励</font>",
	resData={talkTitle="日常押镖",btnName = "领取奖励",},
	func = "npc.task.n032.getAward",
	awards = {},
}

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

function onTalk100(npc,player)
	local dart_lv = util.ppn(player,const.PP_DART_SUCCEED_LEVEL)
	if dart_lv > 0 then
		dartawardInfo.awards = gui.PanelDart.getAwardsTable(dart_lv)
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="dartaward",talk_str = dartawardInfo}))	
		return
	end


	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str=npc:get_dialog(),npcName=npc:get_name()}));
end


function btnGetAward(player,data)
	gui.PanelDart.getDartAwards(player, util.ppn(player,const.PP_DART_SUCCEED_LEVEL), 1)
	player:set_param(const.PP_DART_SUCCEED_LEVEL,0);
end

-- function btnGetAwardDouble(player,data)
-- 	if player:get_vcoin_bind() < 10 then
-- 		player:alert(1,1,"绑定元宝不足！")
-- 		return
-- 	end

-- 	player:sub_vcoin_bind(10)

-- 	gui.PanelDart.getDartAwards(player, util.ppn(player,const.PP_DART_SUCCEED_LEVEL), 2)
-- 	player:set_param(const.PP_DART_SUCCEED_LEVEL,0);
-- end

-- function btnGetAwardTripple(player,data)
-- 	if player:get_vcoin_bind() < 20 then
-- 		player:alert(1,1,"绑定元宝不足！")
-- 		return
-- 	end

-- 	player:sub_vcoin_bind(20)
-- 	gui.PanelDart.getDartAwards(player, util.ppn(player,const.PP_DART_SUCCEED_LEVEL), 3)
-- 	player:set_param(const.PP_DART_SUCCEED_LEVEL,0);
-- end


ckpanel.add_listener("npc.task.n032.getAward",npc.task.n032.btnGetAward)
-- ckpanel.add_listener("npc.task.n032.getAwardDouble",npc.task.n032.btnGetAwardDouble)
-- ckpanel.add_listener("npc.task.n032.getAwardTripple",npc.task.n032.btnGetAwardTripple)