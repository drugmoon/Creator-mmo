module(..., package.seeall)
local task_id=const.TASK_ID_BRANCH;

local task_data = tab.TaskData1100;
task_data.ttype="支线";
task_data.focus=true;
task_data.autogo=true;

local task_state_data={};

function onGetTaskShortDesp(player)		task.util.show_task_short_desp(player,player,task_id,task_state_data,task_data);end

function show_task(npc,player)			task.util.show_main_task_talk(npc,player,task_id,task_state_data,task_data);end

function get_npc_flags(npc,player)		return task.util.get_npc_flags(npc,player,task_id,task_state_data,task_data);end

function check_enter_map(player,map) 	task.util.check_enter_map(player,map,task_id,task_state_data,task_data);end
function check_join_guild(player) 		task.util.check_join_guild(player,task_id,task_state_data,task_data);end
function check_join_group(player) 		task.util.check_join_group(player,task_id,task_state_data,task_data);end
function check_add_friend(player) 		task.util.check_add_friend(player,task_id,task_state_data,task_data);end
function check_get_exploit(player,exploit)	task.util.check_get_exploit(player,exploit,task_id,task_state_data,task_data);end

_M['onTalkT'..task_id..'Show']=function (npc,player) task.util.show_main_task_talk(npc,player,task_id,task_state_data,task_data); end
_M['onTalkT'..task_id..'Do']=function (npc,player) return task.util.task_done(npc,player,task_id,task_state_data,task_data); end

local MIN_TID = 0;
local MAX_TID = 0;

for i,v in pairs(task_data) do
	if util.isTable(task_data[i]) then

		if MIN_TID == 0 then
			MIN_TID = i;
		elseif MIN_TID > i then
			MIN_TID = i;
		end

		if MAX_TID == 0 then
			MAX_TID = i;
		elseif MAX_TID < i then
			MAX_TID = i;
		end


		task.util.build_task_state(task_id,task_data[i],task_data,task_state_data);
		
		if task_data[i].conditionType == const.TASK_CONDITION.ENTERMAP then
			ckmap.add_listener(task_data[i].checkID, check_enter_map);
		end

		if task_data[i].conditionType == const.TASK_CONDITION.JOINGUILD then
			ckguild.add_all_listener(check_join_guild);
		end

		if task_data[i].conditionType == const.TASK_CONDITION.JOINGROUP then
			ckgroup.add_all_listener(check_join_group);
		end
	end
end;

for i=1,100 do if _M['init_task_'..i] then _M['init_task_'..i](); end; end;

function isFirstTask(tid)
	return tid == MIN_TID;
end


function onTaskChange(player, tid)
	local tid, ts = task.util.get_task_state(player, const.TASK_ID_MAIN);
	-- 主线完成
	if tid >= 10101 and ts == const.TSCOMP then
		player:set_task_state(task_id,task.util.ts(MIN_TID, const.TSCOMP));
		player:push_task_data(task_id,1);

		if task.task1000.isLastTask(tid) then
			--自动寻路
			local td = task_data[MIN_TID];
			if td and td.doner then
				posnpc.go(player, td.doner);
				player:push_lua_table("showFly",util.encode({flyInfo = posnpc.getFlyId(td.doner)}));
			end
		end
	end
end

cktask.add_listener(const.TASK_ID_MAIN, onTaskChange)

