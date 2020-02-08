module(..., package.seeall)
local mapid = "kuang3jia";
-- 1050005	null	土城	tucheng	65	54	1	1	0	kuang3jia	18	12	0	70001
local mapconn = {
	nodeid = 1050005, showname="土城", destid="tucheng", destx=65, desty=54, srcid="kuang3jia", srcx=18, srcy=12, cloth=70001, range = 1
};

local mon_data={
	[1]={name="尸卫",num=2,id=12001001,x=21,y=32,},
	[2]={name="尸王",num=1,id=12001002,x=23,y=30,},
}

function onPlayerEnter(map,player)
	-- map:remove_dy_conn(mapconn.nodeid);
	map:enable_conn(mapconn.nodeid, 0);
end

function onPlayerLeave(map,player)
	map:clear_mon();
	map:clear_item();
	task.task1000.check_leave_copy(player, map);
	map:set_timer(1,0);
end

-- 外部调用
function enter_map(player)
	local map = server.find_map(mapid);
	if not map then return end

	map:clear_mon();
	map:clear_item();
	map:clear_mininpc();
	--map:set_timer(1, award_table[index].time);--------设置本地图中出去的时间

	player:enter_map(mapid,20,39);

	local x, y;
	for i=1,#mon_data do
		x,y = map:gen_rand_position();
		map:mon_gen(mon_data[i].x, mon_data[i].y, mon_data[i].id,mon_data[i].num,1,1,1,"show_name");
	end
	player:notify_mini_npc()

	player:find_road_goto(mapid, 21, 32, "autofightstart");
end

function onMonKill(player, mon)
	local map = player:get_map();
	if map and map:get_def_id() == mapid then
		-- print("onMonKill", map:num_monster())
		if map:num_monster() == 0 then
			map:enable_conn(mapconn.nodeid, 1);
			-- map:add_dy_conn(mapconn.nodeid, mapconn.cloth, mapconn.srcx, mapconn.srcy, mapconn.destid, mapconn.destx, mapconn.desty, mapconn.range);
			map:set_timer(1, 2 * 1000);
		end
	end
end


function onTimer1(map)
	--- 10秒后引导点击退出副本
	-- print("onTimer1")
	if  map:num_item() <= 0 then
		map:map_exe("map.kuang3jia.checkTaskDone")
	else
		map:set_timer(1, 2 * 1000)
	end
end

function checkTaskDone(player)
	task.task1000.check_leave_copy(player ,player:get_map());
end

ckmon.add_listener(mon_data[1].id, onMonKill);
ckmon.add_listener(mon_data[2].id, onMonKill);