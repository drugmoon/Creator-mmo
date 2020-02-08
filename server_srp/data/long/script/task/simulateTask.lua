module(..., package.seeall)
-- "<font color=#fff843>找<font color=#30ff00>"..player:get_npc_name(k.accepter).."</font>接任务</font>"
--每日循环任务
local taskInfos = {
	-- [6000] = {
	-- 	ttype = "护卫进阶", 
	-- 	name = "闯天关", 
	-- 	desp = "<font color=#ffffff>完成一次闯天关</font>", 
	-- 	need_level = 62,
	-- 	checkFunc = function (player)
			
	-- 	end
	-- },
	[6001] = {
		ttype = "经验、材料", 
		name = "副本挑战", 
		desp = "<font color=#ffffff>寻找:<font color=#30ff00>副本使者</font>接任务</font>", 
		need_level = 70, 
		accepter = 4002011,
		checkFunc = function (player)
			return npc.party.cailiao.checkTimesComplete(player);
		end
	},
	[4000] = {
		ttype = "金币", 
		name = "挖矿任务", 
		desp = "<font color=#ffffff>寻找:<font color=#30ff00>矿主</font>接任务</font>", 
		need_level = 80, 
		stateDesp = "任意矿石(%d/10)", 
		param = const.PP_WAKUANG_ACTION_COUNT, 
		accepter = 4002005,  
		doner = 4002005,
	},
	[5000] = {
		ttype = "经验", 
		name = "烧猪洞", 
		desp = "<font color=#ffffff>寻找:<font color=#30ff00>烧猪洞</font>接任务</font>", 
		need_level = 75, 
		accepter = 4002021, 
		checkFunc = function (player)
			if util.ppn(player,const.PP_SHAOZHU_FREE_TIME) < const.MAX_TASK_SHAOZHU_COUNT then
				return true;
			elseif util.ppn(player,const.PP_SHAOZHU_FREE_TIME) == const.MAX_TASK_SHAOZHU_COUNT then

			end
		end
	},
	[5001] = {
		ttype = "宝石", 
		name = "守护女神", 
		desp = "<font color=#ffffff>寻找:<font color=#30ff00>守护女神</font>接任务</font>", 
		need_level = 85, 
		accepter = 4002006, 
		checkFunc = function (player)
			if util.ppn(player,const.PP_DEFEND_TIMES) < const.MAX_DFEND_TIMES then
				return true;
			elseif util.ppn(player,const.PP_DEFEND_TIMES) == const.MAX_DFEND_TIMES then

			end
		end
	},
}

for k,v in pairs(taskInfos) do
	v.task_id = k;
end

function getTaskShortDesp(player, info, level, selected)
	local shortDesp = {
		task_type = info.ttype,
		task_name = info.name,
		task_state = "", --可接 可交
		desc = info.desp,
		task_link = info.task_id,
		task_target = posnpc.sp(info.accepter),
	};

	level = level or player:get_level();
	if level < info.need_level then
		shortDesp.task_state = nil;
		return shortDesp;
	end

	if info.task_id == 4000 then
		local state = npc.kuangzhu.getTaskState(player);
		if state then
			shortDesp.task_state = const.TASK_STATE_NAME[state];
		else
			shortDesp.task_state = nil;
			return shortDesp;
		end
		if state == const.TSACED then
			shortDesp.desc = "<font color=#ffffff>获得:<font color=#30ff00>任意矿石</font>("..util.ppn(player, const.PP_WAKUANG_ACTION_COUNT).."/".."10)</font>";
			shortDesp.selected = selected;
		elseif state == const.TSCOMP then
			shortDesp.desc = "<font color=#ffffff>寻找:<font color=#30ff00>矿主</font></font>";
			shortDesp.selected = true;
		elseif state == const.TSACCE then
			shortDesp.desc = "<font color=#ffffff>寻找:<font color=#30ff00>矿主</font>("..util.ppn(player,const.PP_WAKUANG_TASK_TIME).."/"..const.MAX_TASK_KUANG_COUNT..")</font>";
			shortDesp.target_fly=posnpc.fid(info.accepter)
			shortDesp.task_target = posnpc.sp(info.accepter)
		end
	else
		if info.checkFunc and not info.checkFunc(player) then
			shortDesp.task_state = nil;
			return shortDesp;
		end
		shortDesp.target_fly=posnpc.fid(info.accepter)

		if info.task_id == 5000 then
			shortDesp.desc = "<font color=#ffffff>寻找:<font color=#30ff00>烧猪洞</font>接任务("..util.ppn(player,const.PP_SHAOZHU_FREE_TIME).."/"..const.MAX_TASK_SHAOZHU_COUNT..")</font>"
			shortDesp.task_target = posnpc.sp(info.accepter);
		elseif info.task_id == 5001 then
			shortDesp.desc = "<font color=#ffffff>寻找:<font color=#30ff00>守护女神</font>接任务("..util.ppn(player,const.PP_DEFEND_TIMES).."/"..const.MAX_DFEND_TIMES..")</font>"
			shortDesp.task_target = posnpc.sp(info.accepter);
		end
	end
	return shortDesp
end

function showTaskDesp(player, taskid, level, selected)
	local info = taskInfos[taskid];
	if info then
		local result = {
			mTaskID = taskid,
			mName = info.name,
			mParam_1=0,
			mShortDesp = getTaskShortDesp(player, info, level, selected)
		}
		-- result.task_state=const.TASK_STATE_NAME[d]
		-- if taskid == 4000 then
		-- 	print("showTaskDesp", taskid, util.encode(result));
		-- end

		player:push_lua_table("taskChange",util.encode(result))
	end
end

function checkTaskLevel(player, level)
	-- print("/////////////////////checkTaskLevel/////////////////////", level)
	for k,v in pairs(taskInfos) do
		showTaskDesp(player, k, level)
	end
end

function onPushTaskItem(player, command)
	command = tonumber(command);
	if not command then return; end
	local info = taskInfos[command];
	if not info then return; end
	-- print("onPushTaskItem", command)
	if command == 6000 then
		player:push_lua_table("open",util.encode({name = "extend_breakup"}))
	elseif command == 5000 then
		local map = player:get_map();
		if map and map:get_def_id() ~= "shaozhu" then
			-- posnpc.go(player, info.accepter);
		end
	elseif command == 6001 then
		local map = player:get_map();
		-- posnpc.go(player, info.accepter);
	elseif command == 4000 then
		local state = npc.kuangzhu.getTaskState(player);
		if state == const.TSACED then
			local map = player:get_map();
			if map then 
				if map:get_def_id() ~= "wakuang" then
					npc.kuangzhu.btnContinue(player);
				else
					player:push_lua_table("startAutoFight", "");
				end
			end

		elseif state == const.TSCOMP then
			posnpc.fly(player, info.doner);
		elseif state == const.TSACCE then
			-- posnpc.go(player, info.accepter);
		end
	end
end

function onLevelUp(player, level)
	checkTaskLevel(player, level);
end
cklevel.add_listener(onLevelUp);

ckpanel.add_listener("task.simulateTask.onPushTaskItem", task.simulateTask.onPushTaskItem);
-- task.simulateTask.showTaskDesp(player, taskid)