module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local shaozhuInfo =	{
	strs1={
		"<font color=#b2a58b>    为躲避战乱而奔逃至此的猪妖们找到了属于自己的避难所，也因此引来无数追杀它们的人...</font>",
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b></font>",
	},
	strs2={
		"<font color=#b2a58b>1.进入条件：<font color=#30ff00>75级</font>，限时8分钟</font>",
		"<font color=#b2a58b>2.杀怪获得<font color=#30ff00>大量经验</font></font>",
		"<font color=#b2a58b>3.雇佣法师获得<font color=#30ff00>更多经验</font></font>",
		"<font color=#b2a58b>4.每波<font color=#30ff00>全部击杀</font>后刷怪</font>",
		"<font color=#b2a58b>5.合理分布法师<font color=#30ff00>高效刷怪</font></font>",
	},
	countstr="今日剩余次数:",
	count=0,
	resData={talkTitle="猪洞",btnName = "进入猪洞",},
	func = "npc.task.n018.btnEnter",
}

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);
		return;
	end

	-- 显示烧猪奖励
	local exp = util.ppn(player,const.PP_SHAOZHU_KILL_MON_EXP)
	local pass_section = util.ppn(player,const.PP_SHAOZHU_PASS_SECTION)
	if pass_section > 0 or exp > 0 then
		local panel_data = {
			name = "panel_shaozhuaward",
			extend = {},
		}
		--local exp = map.shaozhu.getPassSectionExp(pass_section)
		panel_data.extend.pass_title_str = "通过最大波数:"..pass_section.."/10"
		panel_data.extend.exp_str = "本次累计获得经验:"..exp
		panel_data.extend.exp = exp
		player:push_lua_table("open", util.encode(panel_data))
		return
	end

	-- -- 显示烧猪任务
	local times = util.ppn(player,const.PP_SHAOZHU_FREE_TIME)
	shaozhuInfo.count = const.MAX_TASK_SHAOZHU_COUNT - times
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="shaozhu",talk_str = shaozhuInfo}));
end

function btnEnter(player,data)

--	if 1 then
--		player:set_buff(27004)
--		return
--	end

	local times = util.ppn(player,const.PP_SHAOZHU_FREE_TIME)
	if times >= const.MAX_TASK_SHAOZHU_COUNT then
		util.alert_msg(4, "您今天的副本次数已经用完，请明天再来", player)
		return
	end

	if player:get_level() < 75 then
		util.alert_msg(4, "等级不足75级，无法进入副本", player)
		return
	end

	local curMapId = player:get_map():get_id()

	if string.find(curMapId,"shaozhu")  then
		util.alert_msg(4, "您已在猪洞地图中，无需传送！", player)
		return
	end

	local map = server.find_map("shaozhu")
	if map and map:num_player() < 1 then
		local x,y
--		x,y=map:gen_rand_position()
		player:set_param(const.PP_SHAOZHU_FREE_TIME, times + 1)

		player:enter_map(map:get_id(),21,36)

		task.simulateTask.showTaskDesp(player, 5000);
	else
		util.alert_msg(4, "烧猪人数太多，请稍后再来", player)
	end
	gui.PanelGongXun.checkTaskComplete(player,7)
end

local shaozhuAwardCost =
{
	[1] = 0,
	[2] = 500,
	[3] = 1100,
	[4] = 1800,
	[5] = 2600,
	[6] = 5000,
}

local rate =
{
	[1] = 1,
	[2] = 2,
	[3] = 3,
	[4] = 4,
	[5] = 5,
	[6] = 8,
}

local vipneed =
{
	[1] = 0,
	[2] = 0,
	[3] = 1,
	[4] = 2,
	[5] = 3,
	[6] = 4,
}

function btnGetAward(player, commond)
	local pass_section = util.ppn(player,const.PP_SHAOZHU_PASS_SECTION)
	local exp = util.ppn(player,const.PP_SHAOZHU_KILL_MON_EXP)

	if pass_section < 0 or pass_section > 10 or exp <= 0 then
		return
	end

	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
	local data = util.decode(commond)
	if data.actionid == "getAward" then
		local id = data.params.id
		if id >=1 and id <= 6 then
			if player:get_vcoin() >= shaozhuAwardCost[id] then
				if vipLv >= vipneed[id] then
					player:set_param(const.PP_SHAOZHU_PASS_SECTION, 0)
					player:set_param(const.PP_SHAOZHU_KILL_MON_EXP, 0)

					if shaozhuAwardCost[id] > 0 then
						player:sub_vcoin(shaozhuAwardCost[id],"烧猪多倍经验")
					end

					exp = exp * rate[id]
					player:add_exp(exp)
					player:alert(1,1,"获得经验*"..exp)
					player:push_lua_table("close", util.encode({name = "panel_shaozhuaward"}))
				else
					player:alert(1,1,"vip等级不足,无法获取该奖励")
					return
				end
			else
				--弹出充值
				showChongzhi(player)
				return
			end
		end
	end
end

ckpanel.add_listener("npc.task.n018.btnEnter",npc.task.n018.btnEnter)
ckpanel.add_listener("npc.task.n018.btnGetAward",npc.task.n018.btnGetAward)