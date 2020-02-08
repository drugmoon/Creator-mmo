module(..., package.seeall)

local ACTIVITY_NAME = '天降赐福';
local changeTime = 3*60
--天降赐福 v405
local doubleAward = {
	{ name = "人物经验",	num =100000,	prob = 125},
	{ name = "护卫经验",	num =100000,	prob = 125},
	{ name = "内功经验",	num =10000,		prob = 125},
	{ name = "绑定金币",	num =100000,	prob = 125},
	{ name = "绑定元宝",	num =40,		prob = 125},
	{ name = "声望值",		num =1000,		prob = 125},
	{ name = "荣誉值",		num =150,		prob = 250},
}
local award = {
	{ name = "人物经验",	num =50000,	prob = 125},
	{ name = "护卫经验",	num =50000,	prob = 125},
	{ name = "内功经验",	num =5000,	prob = 125},
	{ name = "绑定金币",	num =50000,	prob = 125},
	{ name = "绑定元宝",	num =20,	prob = 125},
	{ name = "声望值",		num =500,	prob = 125},
	{ name = "荣誉值",		num =75,	prob = 250},
}
local honorValue = {default = 60, percentMax = 2, max = 3000}

function onPlayerEnter(map,player)
	if server.get_temp_var("tjcfopen")~="open" then
		player:go_home();return;
	end
	updataUIleftTips(player)
end

function onPlayerLeave(map,player)
	player:push_lua_table("showtjcf",util.encode({close = true}))
end

function onTimer1(map)
	map:map_exe("map.cifu.addExp");
	map:set_timer(1,5*1000)
end
--修改双倍经验圈
function onTimer2(map)
	changeDoubleArea(map)
	map:set_timer(2,changeTime*1000)
end

function addExp(player)
	local rand,t
	if isDouble(player) then
		rand = util.produceProb(doubleAward)
		t = doubleAward[rand]
	else
		rand = util.produceProb(award)
		t = award[rand]
	end
	if t then
		if t.name =="人物经验" then
			player:add_exp(t.num)
		elseif t.name =="护卫经验" then
			gui.PanelHuWei.addHuweiExp( player,t.num )
		elseif t.name =="内功经验" then
			gui.PanelInnerPower.addInnerPower( player,t.num )
		elseif t.name =="绑定金币" then
			player:add_gamemoney_bind(t.num)
		elseif t.name =="绑定元宝" then
			player:add_vcoin_bind(t.num)
		elseif t.name =="声望值" then
			gui.PanelOfficial.addsw(player,t.num)
		elseif t.name =="荣誉值" then
			gui.PanelOfficial.addry(player,t.num)
		end
		player:alert(1,1,t.name.."增加"..t.num)

		-- updataUIleftTips(player)
	end
end

function updataUIleftTips(player)
	local result={}
	result.btnAction = "npc.tjcf.btnExit"
	local openTime = tonumber(server.get_var("tjcfStartTime")) or os.time()
	result.second = gui.PanelActivityGuide.getActivityTotalSecond( ACTIVITY_NAME ) - os.time() + openTime
	result.serverTime = os.time()
	result.awardTable = {
		"地图内每5秒随机获得：",
		"人物经验",
		"护卫经验",
		"内功经验",
		"绑定元宝",
		"绑定金币",
		"荣誉值",
		"声望值",
		"站在双倍光圈更可获得",
		"高额收益！！！",
	}
	result.title = ACTIVITY_NAME
	player:push_lua_table("showtjcf",util.encode(result))
end
--8个点不重叠
local mapPos ={
--选3个
{id = 1001,	x= 	20,	y=31},
{id = 1002,	x = 22,	y=33},
{id = 1003,	x = 17,	y=32},
{id = 1004,	x = 15,	y=34},
{id = 1005,	x = 17,	y=36},
{id = 1006,	x = 24,	y=36},
--选5个
{id = 1010,	x = 21,	y=26},
{id = 1011,	x = 16,	y=28},
{id = 1012,	x = 12,	y=32},
{id = 1013,	x = 10,	y=36},
{id = 1014,	x = 14,	y=40},
{id = 1015,	x = 17,	y=43},
{id = 1016,	x = 21,	y=43},
{id = 1017,	x = 24,	y=41},
{id = 1018,	x = 26,	y=38},
{id = 1019,	x = 29,	y=35},
{id = 1020,	x = 29,	y=31},
{id = 1021,	x = 25,	y=26},
{id = 1022,	x = 22,	y=28},
{id = 1023,	x = 25,	y=36},
{id = 1024,	x = 27,	y=30},
{id = 1025,	x = 19,	y=33},
{id = 1026,	x = 22,	y=38},
{id = 1027,	x = 14,	y=28},
{id = 1028,	x = 17,	y=25},
{id = 1029,	x = 9,	y=32},
{id = 1030,	x = 18,	y=44},
}
function changeDoubleArea(mapcifu)
	local x,y,id = 0,0,0
	removeTrap(mapcifu)
	local p1 = util.chooseSomeFromTable(mapPos,3,1,6)
	local p2 = util.chooseSomeFromTable(mapPos,5,7,#mapPos)
	local t={}
	for i,v in ipairs(p1) do
		x = v.x;
		y = v.y;
		id = v.id;
		mapcifu:trap_gen(x,y,id,50030,0,0,0)
		table.insert(t,v.id)
	end
	for i,v in ipairs(p2) do
		x = v.x;
		y = v.y;
		id = v.id;
		mapcifu:trap_gen(x,y,id,50030,0,0,0)
		table.insert(t,v.id)
	end
	server.set_var("trap_t",table.concat(t,";"))
end

function removeTrap(mapcifu)
	local traps = server.get_var("trap_t")
	for k,v in pairs(mapPos) do
		if string.find(traps,v.id) then
			mapcifu:remove_trap(v.x,v.y,v.id)
		end
	end
	server.set_var("trap_t","")
end
function isDouble(player)
	local x,y = player:get_pos()
	local traps = server.get_var("trap_t")
	for i,v in ipairs(mapPos) do
		if x==v.x and y==v.y and string.find(traps,v.id) then
			return true
		end
	end
end
--双倍外的区域
function getRelivePos(player)
	local map = server.find_map("cifu")
	local x,y =16,50
	if map then
		x,y = map:gen_rand_position()
		for k,v in pairs(mapPos) do
			if v.x==x and v.y==y then
				return getRelivePos(player)
			end
		end
		return x,y
	end
	return x,y
end

function killman(killer,killed)
	if server.get_temp_var("tjcfopen")=="open" then
		local mylevel = killer:get_level()
		local otherlevel = killed:get_level()
		local percent = 1
		local value = util.ppn(killer, const.PP_TJCF_HONOR_VALUE)
		if mylevel<otherlevel then
			percent = percent + 0.1*(otherlevel-mylevel)
		elseif mylevel>otherlevel then
			percent = percent - 0.05*(mylevel-otherlevel)
		end
		percent = util.bound(0, percent, honorValue.percentMax)
		local honor = percent* honorValue.default
		value = value + honor
		if value > honorValue.max then honor = honorValue.max - value; value = honorValue.max; end
		honor = util.bound(0, honor, honorValue.max)
		honor = checkint(honor)
		killer:set_param(const.PP_TJCF_HONOR_VALUE,value)
		if honor >0 then
			gui.PanelOfficial.addry(killer,honor)
		end
		killer:alert(1,1,"荣誉值增加"..honor)
	end
end