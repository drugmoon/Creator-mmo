module(..., package.seeall)

util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local kuangzhuInfo = {
	strs1={
		"<font color=#b2a58b>好好挖，用心挖，万一挖到个金矿，不就发财了吗？</font>",
	},
	strs2="今日剩余次数:",
	resData={talkTitle="矿主",btnName="接受任务",},
	func = "npc.kuangzhu.btnEnter",
	count = 3,
	awards = {{id = 40000001, num = 500000},{id = 23060001, num = 2},{id=24020001,num=20},},
}

function onTalk100(npc,player)
	--player:set_param(const.PP_WAKUANG_TASK_TIME, 0)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	if PLATFORM_BANSHU then
		return player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str=npc:get_dialog(),npcName=npc:get_name()}));
	end
	
	kuangzhuInfo.count = const.MAX_TASK_KUANG_COUNT - util.ppn(player,const.PP_WAKUANG_TASK_TIME)
	local flag = util.ppn(player,const.PP_WAKUANG_TASK_FLAG)
	if flag == 0 then 
		-- 没有接任务,弹出接任务界面
		if kuangzhuInfo.count <= 0 then 
			kuangzhuInfo.resData.btnName = "进入矿洞"
			kuangzhuInfo.func = "npc.kuangzhu.btnEnter"
		else
			kuangzhuInfo.resData.btnName="接受任务"
			kuangzhuInfo.func = "npc.kuangzhu.btnEnter"
		end

		return player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="kuangzhu",talk_str = kuangzhuInfo}))
	end

	if flag == 1 then
		-- 已经接任务,弹出继续任务,或者完成任务界面
		local wakuang_action_count = util.ppn(player,const.PP_WAKUANG_ACTION_COUNT)
		if wakuang_action_count < 10 then 
			kuangzhuInfo.resData.btnName = "继续任务"
			kuangzhuInfo.func = "npc.kuangzhu.btnContinue"
		else 
			kuangzhuInfo.resData.btnName = "完成"
			kuangzhuInfo.func = "npc.kuangzhu.btnComplete"	
		end

		return player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="kuangzhu",talk_str = kuangzhuInfo}))
	end
end

-- 接受任务
function btnEnter(player,data)
	local flag = util.ppn(player,const.PP_WAKUANG_TASK_FLAG)
	if flag > 0 then
		return
	end

	if player:get_level() < 80 then
		return player:alert(1,1,"等级不足80级,无法进入挖矿地图");
	end

	local count = util.ppn(player,const.PP_WAKUANG_TASK_TIME)
	if count < const.MAX_TASK_KUANG_COUNT then
		count = count + 1
		player:set_param(const.PP_WAKUANG_TASK_TIME, count)
		player:set_param(const.PP_WAKUANG_TASK_FLAG, 1)
	end

	local map = server.find_map("wakuang")
	local x,y
	x,y=map:gen_rand_position()
	player:enter_map("wakuang",x,y)
	player:find_road_goto(map:get_id(),x,y,"autofightstart")
	gui.PanelGongXun.checkTaskComplete(player,10)
	task.simulateTask.showTaskDesp(player, 4000);
end

-- 继续任务
function btnContinue(player,data)
	local flag = util.ppn(player,const.PP_WAKUANG_TASK_FLAG)
	if flag <= 0 then
		return
	end

	local map = server.find_map("wakuang")
	local x,y
	x,y=map:gen_rand_position()
	player:enter_map("wakuang",x,y)
	player:find_road_goto(map:get_id(),x,y,"autofightstart")
	gui.PanelGongXun.checkTaskComplete(player,10)
end

-- 完成任务
function btnComplete(player, data)
	local flag = util.ppn(player,const.PP_WAKUANG_TASK_FLAG)
	if flag <= 0 then
		return
	end

	local wakuang_action_count = util.ppn(player,const.PP_WAKUANG_ACTION_COUNT)
	if wakuang_action_count < 10 then
		return
	end

	player:set_param(const.PP_WAKUANG_ACTION_COUNT, 0)
	player:set_param(const.PP_WAKUANG_TASK_FLAG, 0)
	player:add_exp(50 * 10000)
	player:add_item(23060001, 2, 1)
	player:add_item(24020001, 20, 1)
	player:alert(1,1,"获得经验*500000、绑定金砖（小）*2、黑铁矿石*20")
	task.simulateTask.showTaskDesp(player, 4000);
end

function getTaskState(player)
	local count = util.ppn(player,const.PP_WAKUANG_TASK_TIME);
	if count <= const.MAX_TASK_KUANG_COUNT then
		local flag = util.ppn(player,const.PP_WAKUANG_TASK_FLAG);
		if flag == 0 then
			if count < const.MAX_TASK_KUANG_COUNT then
				return const.TSACCE;
			end
		elseif flag == 1 then
			local count = util.ppn(player,const.PP_WAKUANG_ACTION_COUNT);
			if count < 10 then
				return const.TSACED;
			else
				return const.TSCOMP;
			end
		end
	else
		return const.TASK_STATE_NULL;
	end
end

ckpanel.add_listener("npc.kuangzhu.btnEnter",npc.kuangzhu.btnEnter)
ckpanel.add_listener("npc.kuangzhu.btnContinue",npc.kuangzhu.btnContinue)
ckpanel.add_listener("npc.kuangzhu.btnComplete",npc.kuangzhu.btnComplete)

