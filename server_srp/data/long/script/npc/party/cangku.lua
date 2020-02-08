module(... ,package.seeall)
-- util.use_function(_M,task.task1000);

function onTalk100(npc,player)
	player:push_lua_table("open",util.encode({name = "panel_depot"}))
end

