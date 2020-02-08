module(..., package.seeall)
local mapid = "kingjia";
-- 1040002	null	土城	tucheng	65	54	1	1	0	kingjia	21	48	0	70001
local mapconn = {
	nodeid = 1040002, showname="土城", destid="tucheng", destx=65, desty=54, srcid="kingjia", srcx=21, srcy=48, cloth=70001, range = 1
};

function onPlayerEnter(map,player)
	-- map:remove_dy_conn(mapconn.nodeid);
	map:enable_conn(mapconn.nodeid, 0);
	--出外滑引导
	player:push_lua_table("showGestureGuide", util.encode({slideIn = false}));
end


function onPlayerLeave(map,player)
	map:clear_mon();
	map:clear_item();

	-- 特殊处理，显示正式任务状态
	task.task1000.check_leave_copy(player ,map);
	map:set_timer(1,0);
end

local mon_data={
	[1]	={name="帮会小弟",	num=1,id=12003001, x=25, y=34,},
	[2]	={name="帮会小弟",	num=1,id=12003002, x=21, y=38,},
	[3]	={name="帮会小弟",	num=1,id=12003003, x=18, y=35,},
	[4]	={name="帮会小弟",	num=1,id=12003004, x=21, y=32,},
	[5]	={name="帮会小弟",	num=1,id=12003005, x=18, y=28,},
	[6]	={name="帮会小弟",	num=1,id=12003006, x=14, y=31,},
	[7]	={name="大老王",	num=1,id=12003007, x=13, y=26,},
	[8]	={name="云鹏",		num=1,id=12003008, x=9,  y=29,},
	[9]	={name="张小凡",	num=1,id=12003009, x=16, y=21,},
	[10]={name="戴你飞",	num=1,id=12003010, x=10, y=23,},
}

function enter_map(player)
	local map = server.find_map(mapid)
	if not map then return end

	map:clear_mon();
	map:clear_item();
	map:clear_mininpc();
	--map:set_timer(1, award_table[index].time);--------设置本地图中出去的时间
	-- local x, y;
	player:enter_map(mapid,35,53);
	for i=1,#mon_data do
		-- x,y = map:gen_rand_position();
		map:mon_gen(mon_data[i].x, mon_data[i].y, mon_data[i].id,mon_data[i].num,1,1,1,"show_name");
	end
	player:notify_mini_npc()
	--player:find_road_goto(mapid, 23, 38, "autofightstart");
end

function onMonKill(player, mon)
	local map = player:get_map();
	if map and map:get_def_id() == mapid then
		if map:num_monster() == 0 then
			map:enable_conn(mapconn.nodeid, 1);
			-- map:add_dy_conn(mapconn.nodeid, mapconn.cloth, mapconn.srcx, mapconn.srcy, mapconn.destid, mapconn.destx, mapconn.desty, mapconn.range);
			-- task.task1000.check_leave_copy(player ,map);
			map:set_timer(1, 2 * 1000);
		end
	end
end

function onTimer1(map)
	--- 10秒后引导点击退出副本
	-- print("onTimer1")
	if  map:num_item() <= 0 then
		--出内滑引导
		map:map_exe("map.kingjia.showSlideInGuide")
	else
		map:set_timer(1, 2 * 1000)
	end
end

-- function onTimer2(map)

-- end

-- function onTimer3(map)
-- 	-- body
-- end

function showSlideInGuide(player)
	player:push_lua_table("stopAutoFight", "");
	player:push_lua_table("showGestureGuide", util.encode({slideIn = true}));
end

function checkTaskDone(player)
	task.task1000.check_leave_copy(player ,player:get_map());
end

ckmon.add_all_listener(onMonKill);

function onDoneGuide(player, commond)
	local data =util.decode(commond);
	if data.guideType == "slideIn" then
		checkTaskDone(player)
	elseif data.guideType == "slideOut" then
		player:find_road_goto(mapid, 23, 38, "autofightstart");
	end
end

ckpanel.add_listener("map.kingjia.onDoneGuide", map.kingjia.onDoneGuide)
