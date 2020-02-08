module(..., package.seeall)

local mapid = "taskad";

function onPlayerEnter(map,player)
	map:mon_gen(40,136,12004002,1,1,1,1,"show_name")--12004002 暗之骷髅精灵
	map:mon_gen(38,137,12004001,1,5,1,1,"show_name")--12004001 暗殿钳虫
	player:notify_mini_npc()
	-- 20分钟后离开地图
	map:set_timer(2, 20 * 60 * 1000)
end

function onPlayerLeave(map,player)
	map:clear_mon();
	map:clear_item();
	map:set_timer(2, 0);
	map:set_timer(1, 0);
end


function onMonKill(player, mon)
	local map = player:get_map();
	if map and map:get_def_id() == mapid then
		if map:num_monster() == 0 then
			-- map:enable_conn(mapconn.nodeid, 1);
			map:set_timer(1, 2 * 1000)
		end
	end
end

function onTimer1(map)
	-- print("onTimer1")
	if  map:num_item() <= 0 then
		map:map_exe("map.taskandian.checkTaskDone")
	else
		map:set_timer(1, 2 * 1000)
	end
end

function onTimer2(map)
	map:map_exe("go.home");
end

function checkTaskDone(player)
	--特殊处理 检测下任务是否完成
	task.task1000.check_task_done(player);
end

ckmon.add_listener(12004002, onMonKill);
ckmon.add_listener(12004001, onMonKill);
