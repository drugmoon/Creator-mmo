module(..., package.seeall)

local monConf =
{
	{section=1	,	monid = 11004001, award = {id = 40000001, num= 0}},
	{section=2	,	monid = 11004002, award = {id = 40000001, num= 0}},
	{section=3	,	monid = 11004003, award = {id = 40000001, num= 0}},
	{section=4	,	monid = 11004004, award = {id = 40000001, num= 0}},
	{section=5	,	monid = 11004005, award = {id = 40000001, num= 0}},
	{section=6	,	monid = 11004006, award = {id = 40000001, num= 0}},
	{section=7	,	monid = 11004007, award = {id = 40000001, num= 0}},
	{section=8	,	monid = 11004008, award = {id = 40000001, num= 0}},
	{section=9	,	monid = 11004009, award = {id = 40000001, num= 0}},
	{section=10	,	monid = 11004010, award = {id = 40000001, num= 0}},
}

local monExp =
{
	{id = 11004001, exp = 10000},
	{id = 11004002, exp = 15000},
	{id = 11004003, exp = 20000},
	{id = 11004004, exp = 30000},
	{id = 11004005, exp = 40000},
	{id = 11004006, exp = 50000},
	{id = 11004007, exp = 65000},
	{id = 11004008, exp = 80000},
	{id = 11004009, exp = 90000},
	{id = 11004010, exp = 100000},
}

local kill_award =
{
	id = 40000001,
	num = 0,
}

local fresh_mon_pos =
{
	[1] = {x = 15, y = 34},
	[2] = {x = 16, y = 39},
	[3] = {x = 21, y = 41},
}

function getMon(section)
	for i,v in ipairs(monConf) do
		if v.section == section then
			return v
		end
	end
end

function getMonExp(id)
	for i,v in ipairs(monExp) do
		if v.id == id then
			return v.exp
		end
	end
end

function refreshMons(player,map, section)
	local mapid=map:get_id()
	local monConf = getMon(section)
	local posx,posy = player:get_pos()
	local x,y
	for i = 1, 3 do
		x = fresh_mon_pos[i].x
		y = fresh_mon_pos[i].y
		map:mon_gen(x, y, monConf.monid, 20, 8, 1, 1);
	end
	player:find_road_goto(mapid, posx, posy, "autofightstart");
end

function onPlayerEnter(map,player)
	-- 进入副本,初始化通过关卡数
	player:set_param(const.PP_SHAOZHU_PASS_SECTION, 0)
	player:set_param(const.PP_SHAOZHU_KILL_MON_EXP, 0)

	map:clear_mon()

	-- 当前关卡
	map:set_var("section", "1")

	-- 状态,等待刷怪
	map:set_var("waiting_gen_mon", "1")

	-- 刷怪timer
	player:set_timer(17, 30*1000)

	-- 离开时间timer
	player:set_timer(18, 8*60*1000)

	-- 初始化召唤法师次数
	map:set_var("call_leiting_free_count", 5)
	map:set_var("call_haoyue_free_count", 0)
	map:set_var("call_fashi_count", 0)

	local result = {}
  	result.time_left = 8*60+os.time()
  	result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
  	result.gen_mon_time = 30
	result.award = kill_award
	result.section_info = {}
	result.section_info.cur = 0
	result.section_info.max = 10
	result.mon_info = {}
	result.mon_info.cur = 60
	result.mon_info.max = 60
	result.award = kill_award
	result.award.num = 0
	result.call_count = {}
	result.call_count.leiting = util.mvn(map, "call_leiting_free_count") + player:num_item(32000005)
	result.call_count.haoyue = util.mvn(map, "call_haoyue_free_count") + player:num_item(32000006)

  	player:push_lua_table("shaozhuInfo",util.encode(result))


	if util.ppn(player, const.PP_GUIDE_ONCE1) == 0 then
		player:push_lua_table("guide",util.encode({lv = 13}));
		player:set_param(const.PP_GUIDE_ONCE1, 1)
	end
end

function onPlayerLeave(map,player)
	player:set_timer(17, 0)
	player:set_timer(18, 0)

	map:clear_mon()
	player:clear_slave_fashi()
	util.alert_msg(4, "离开烧猪地图", player)

	player:push_lua_table("endGuide",util.encode({lv = 13}));
end

function onMonKill(player, mon1)
	local map = player:get_map()
	local monTotal = map:num_monster()

	local exp = util.ppn(player,const.PP_SHAOZHU_KILL_MON_EXP)
	exp = exp + getMonExp(mon1:get_def_id())
	player:set_param(const.PP_SHAOZHU_KILL_MON_EXP, exp)


	-- 刷新显示怪物数量,获得经验
	local result = {}
	result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
	result.mon_info = {}
	result.mon_info.cur = 60 - monTotal
	result.mon_info.max = 60
	result.award = kill_award
	result.award.num = exp
	-- 刷怪倒计时
	if monTotal == 0 then
		result.gen_mon_time = 3
	end

	player:push_lua_table("shaozhuInfo",util.encode(result))

	if monTotal == 0 then
		-- 设置地图状态(等待刷怪)
		local map = player:get_map()
		-- 当前关卡
		local section = util.mvn(map, "section")
		player:set_param(const.PP_SHAOZHU_PASS_SECTION, section)

		if section == 10 then
			posnpc.fly(player, 4002021)
			return
		end
		-- 设置为下一关
		map:set_var("section", tostring(section + 1))

		map:set_var("waiting_gen_mon", "1")

		player:set_timer(17, 3*1000)
	end
end

function genMonNextWave(player)
	local map = player:get_map()
	local state = util.mvn(map, "waiting_gen_mon")
	if state == 0 then
		return
	end

	map:set_var("waiting_gen_mon", "0")
	local section = util.mvn(map, "section")
	refreshMons(player,map,section)

	-- 刷新界面
	local result = {}
	result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
	result.section_info = {}
	result.section_info.cur = section
	result.section_info.max = 10
	result.mon_info = {}
	result.mon_info.cur = 0
	result.mon_info.max = 60
	player:push_lua_table("shaozhuInfo",util.encode(result))
end

function exitConfirm(player,commond)
	posnpc.fly(player, 4002021)
end

function freshCallFashiCount(player)
	local map = player:get_map()
	local curMapId = map:get_id()
	if string.find(curMapId,"shaozhu") == nil then
		return
	end

	local result = {}
	result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
	result.call_count = {}
	result.call_count.leiting = util.mvn(map, "call_leiting_free_count") + player:num_item(32000005)
	result.call_count.haoyue = util.mvn(map, "call_haoyue_free_count") + player:num_item(32000006)

  	player:push_lua_table("shaozhuInfo",util.encode(result))
end

function btnOp(player,data)
	local req = util.decode(data)
	local map = player:get_map()

	local curMapId = map:get_id()

	if string.find(curMapId,"shaozhu") == 0 then
		return
	end

	if req.actionid == "gohome" then
		local result = {
			str = "退出副本后将失去即将获得的奖励，是否继续?",
			labelConfirm = "确定",labelCancel = "取消",
			callFunc = "map.shaozhu.exitConfirm",
		}
		player:push_lua_table("showConfirm",util.encode(result))
		return
	end

	if req.actionid == "start_gen_mon" then
		genMonNextWave(player);
		player:push_lua_table("endGuide",util.encode({lv = 13}));
		return
	end

	if req.actionid == "buy_leiting" then
		if player:get_vcoin_bind() < 500 then
			player:alert(1,1,"绑元不足,绑元可通过任务获取")
			return
		end
		player:sub_vcoin_bind(500)
		player:add_item(32000005, 1, 1)

		-- 刷新界面
		local result = {}
		result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
		result.call_count = {}
		result.call_count.leiting = util.mvn(map, "call_leiting_free_count") + player:num_item(32000005)
		result.call_count.haoyue = util.mvn(map, "call_haoyue_free_count") + player:num_item(32000006)

	  	player:push_lua_table("shaozhuInfo",util.encode(result))
		return
	end

	if req.actionid == "buy_haoyue" then
		-- 扣元宝,加次数
		if player:get_vcoin() < 99 then
			showChongzhi(player)
			return
		end
		player:sub_vcoin(99,"召唤皓月法师")
		player:add_item(32000006, 1, 1)

		-- 刷新界面
		local result = {}
		result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
		result.call_count = {}
		result.call_count.leiting = util.mvn(map, "call_leiting_free_count") + player:num_item(32000005)
		result.call_count.haoyue = util.mvn(map, "call_haoyue_free_count") + player:num_item(32000006)

	  	player:push_lua_table("shaozhuInfo",util.encode(result))
		return
	end

	-- 召唤雷霆
	if req.actionid == "call_leiting" then
		-- 刷新界面
		local call_count = util.mvn(map,"call_fashi_count");
		-- local call_count = util.mvn(map, "call_fashi_count")
		if call_count >= 12 then
			player:alert(1,1,"召唤次数已达上限")
			return
		end

		local free_count1 = util.mvn(map, "call_leiting_free_count")

		if free_count1 > 0 or player:num_item(32000005) > 0 then
			if free_count1 > 0 then
				free_count1 = free_count1 - 1
				map:set_var("call_leiting_free_count", tostring(free_count1))
				player:gen_slave_fashi(301001)
			else
				player:remove_item(32000005, 1)
				player:gen_slave_fashi(301001)
			end
			call_count = call_count + 1
			map:set_var("call_fashi_count", tostring(call_count))
			player:alert(1,1,"最多只能召唤12次,剩余召唤"..tostring(12-call_count).."次")
		else
			player:alert(1,1,"召唤令牌不足,请购买后使用")
			return
		end

		local result = {}
		result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
		result.call_count = {}
		result.call_count.leiting = util.mvn(map, "call_leiting_free_count") + player:num_item(32000005)
		result.call_count.haoyue = util.mvn(map, "call_haoyue_free_count") + player:num_item(32000006)

	  	player:push_lua_table("shaozhuInfo",util.encode(result))
	  	return
	end

	-- 召唤皓月
	if req.actionid == "call_haoyue" then
		-- 刷新界面
		local call_count = util.mvn(map,"call_fashi_count");
		if call_count >= 12 then
			player:alert(1,1,"召唤次数已达上限")
			return
		end

		local free_count2 = util.mvn(map, "call_haoyue_free_count")

		if free_count2 > 0 or player:num_item(32000006) > 0 then
			if free_count2 > 0 then
				free_count2 = free_count2 - 1
				map:set_var("call_haoyue_free_count", tostring(free_count2))
				player:gen_slave_fashi(301002)
			else
				player:remove_item(32000006, 1)
				player:gen_slave_fashi(301002)
			end
			call_count = call_count + 1
			map:set_var("call_fashi_count", tostring(call_count))
			player:alert(1,1,"最多只能召唤12次,剩余召唤"..tostring(12-call_count).."次")
		else
			player:alert(1,1,"召唤令牌不足,请购买后使用")
			return
		end

		local result = {}
		result.gen_mon_tick_show = util.mvn(map, "waiting_gen_mon")
		result.call_count = {}
		result.call_count.leiting = util.mvn(map, "call_leiting_free_count") + player:num_item(32000005)
		result.call_count.haoyue = util.mvn(map, "call_haoyue_free_count") + player:num_item(32000006)

	  	player:push_lua_table("shaozhuInfo",util.encode(result))
	  	return
	end
end

ckpanel.add_listener("map.shaozhu.btnOp",map.shaozhu.btnOp)
ckpanel.add_listener("map.shaozhu.exitConfirm",map.shaozhu.exitConfirm)
