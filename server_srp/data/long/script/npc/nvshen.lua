module(..., package.seeall)
util.use_function(_M,task.task1000);

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
	
	local ret = util.ppn(player, const.PP_SHOW_DEFEND_RESULT);
	-- if ret == 1 then
	-- 	player:set_param(const.PP_SHOW_DEFEND_RESULT, 0);
	-- 	map.defend.showFailure(player);
	-- else
		player:push_lua_table("open", util.encode({name = "panel_defend"}));
	-- end	
end

