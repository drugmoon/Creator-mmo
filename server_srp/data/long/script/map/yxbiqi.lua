module(..., package.seeall)
local ACTIVITY_NAME = '夜袭比奇';
local changeTime = 5*60;


local killeraward = {
	{id = 23120001,	num = 10,  	bind = 1},--name="低级荣誉令牌",
	{id = 23010001,	num = 3, 	bind = 1}, --name="护盾碎片（小）",
	{id = 23060001,	num = 1,  	bind =1},--name="金砖（小）",
}
local killedaward = {
	{id = 23120001,	num = 2, 	bind =1}, --name="低级荣誉令牌",
	{id = 23060001,	num = 1, 	bind =1},-- name="金砖（小）",
}

local joinAward ={
	{id = 23120001,	num = 8, name="低级荣誉令牌", bind =1},
	{id = 23010001,	num = 3, name="护盾碎片（小）", bind =1},
	{id = 23060001,	num = 1, name="金砖（小）", bind =1},
}
local monConf = { monId = 11005001, num =20 ,range =2, relivetime = tab.getMonReliveTime(11005001)};

--[[
	击杀玩家获得50分，助攻获得25分，击杀怪物获得5分，在线每10秒获得1分，死亡自身拥有的积分减半，活动期间离开地图自身拥有的积分减半
]]
local chartList = {}

function resetChartList(yxbiqimap)
	chartList = {}
	yxbiqimap:set_var("redGroup",0)
	yxbiqimap:set_var("blueGroup",0)
	server.set_var("deadMonster","")
end

function onPlayerEnter(yxbiqimap,player)
	local start = server.get_temp_var("yxbiqiState")
	if start~="start" then
		player:go_home()
		util.alert_msg(4, "活动未开始！", player)
		return
	end
	changeCamp(player)
	npc.yxshacheng.updataUIleftTips(player)
end

function onPlayerLeave(yxbiqimap,player)
	-- if server.get_temp_var("yxbiqiState") =="start" then
	-- 	addScroe(player,"leave")
	-- end
	player:push_lua_table("showyxshacheng",util.encode({close = true}))
	local id = player:get_team_id()
	if id == 1 then
		yxbiqimap:set_var("redGroup",0)
		local redGroupNum = checknumber(yxbiqimap:get_var("redGroup"))
		redGroupNum = redGroupNum - 1
		yxbiqimap:set_var("redGroup",math.max(0,redGroupNum))

	elseif id == 2 then
		yxbiqimap:set_var("blueGroup",0)
		local blueGroupNum = checknumber(yxbiqimap:get_var("blueGroup"))
		blueGroupNum = blueGroupNum + 1
		yxbiqimap:set_var("blueGroupNum",math.max(0,blueGroupNum))
	end
	player:set_team_info(0, "")
	player:set_attack_mode(101)--和平
end

--每5分钟改变一次阵营
function onTimer1(yxbiqimap)
	yxbiqimap:set_timer(1,5*60*1000)
	-- redGroup = {}
	-- blueGroup = {}
	yxbiqimap:set_var("redGroup",0)
	yxbiqimap:set_var("blueGroup",0)
	--前十名预先随机分配
	yxbiqimap:map_exe("map.yxbiqi.changeCamp")
end

--每10秒累计一次
function onTimer2(map)
	map:map_exe("map.yxbiqi.survivalTime")

	map:map_exe("map.yxbiqi.resetChartView")

	map:set_timer(2,10*1000)
end

--怪物复活
function onTimer3(map)
	local deadMonster = util.decode(server.get_var("deadMonster"))
	if not util.isTable(deadMonster) then deadMonster = {} end
	if #deadMonster>0 then
		for i=#deadMonster,1,-1 do
			if deadMonster[i] + monConf.relivetime <= os.time() then
				genMonster(1)
				table.remove(deadMonster,i)
			end
		end
		server.set_var("deadMonster",util.encode(deadMonster))
	end
	local interval = 3
	-- if #deadMonster>0 then
	-- 	interval = deadMonster[#deadMonster] + monConf.relivetime - os.time()
	-- end
	-- interval = util.bound(1, interval, 60)
	-- print("set_timer3----",interval,#deadMonster)
	map:set_timer(3,interval*1000)
end

--buff
local buff = {
	{id = 39001, name="攻击神力", resId = 70002},--红
	{id = 39002, name="防御神盾", resId = 70003},--蓝
	{id = 39003, name="生命洪流", resId = 70004},--绿
	{id = 39004, name="暴击神力", resId = 70005},--紫
	{id = 39005, name="神圣之力", resId = 70006},--黄
}
local mapPos ={
	{id = 1001,	x= 	41,	y=43,	},
	{id = 1002,	x = 41,	y=36,	},
	{id = 1003,	x = 33,	y=44,	},
	{id = 1004,	x = 41,	y=52,	},
	{id = 1005,	x = 50,	y=44,	},
}
function shuffle( t )
	local n = table.nums(t)
	local new,r = util.clone(t),1
	if n>1 then
		for k,v in pairs(new) do
			r = math.random(1,n)
			if k ~= r then
				new[k],new[r] = new[r],new[k]
			end
		end
	else
		new = t
	end
	return new
end

function onTimer4(yxmap)
	removeTrap(yxmap)
	local x,y,id
	local t = {}
	local newBuff = shuffle(buff)
	for i,v in ipairs(mapPos) do
		x = v.x;
		y = v.y;
		id = v.id;
		yxmap:trap_gen(x,y,newBuff[i].id,newBuff[i].resId,0,0,0)
		table.insert(t,newBuff[i].id.."_"..x.."_"..y)
	end
	server.set_var("trap_yx",table.concat(t,";"))

	if server.get_temp_var("yxbiqiState") == "start" then
		yxmap:set_timer(4,120*1000)
	end
end

function removeTrap(yxmap)
	local traps = server.get_var("trap_yx")
	local ts = string.split(traps,";")
	for k,v in pairs(ts) do
		local ss = string.split(v,"_")
		if #ss==3 then
			-- print(ss[1],ss[2],ss[3])
			yxmap:remove_trap(tonumber(ss[2]),tonumber(ss[3]),tonumber(ss[1]))
		end
	end
	server.set_var("trap_yx","")
end

function onEnterTrap( player,trapId )
	for k,v in pairs(buff) do
		if v.id == trapId - 10000 then
			player:set_buff(v.id)
			local pmap = player:get_map()
			if pmap then
				local x,y = player:get_pos()
				-- print("x,y = ",x,y)
				pmap:remove_trap(x,y,trapId-10000)
			end
		end
	end
end

function onLeaveTrap( player,trapId )
	--print(player:get_id(),trapId)
end

function survivalTime(player)
	addScroe(player,"online","notExe")

	local joinTime = util.ppn(player, const.PP_YXBIQI_JOIN_TIME)
	joinTime = joinTime + 10
	player:set_param(const.PP_YXBIQI_JOIN_TIME,joinTime)
	if joinTime>=5*60 then
		player:set_param(const.PP_YXBIQI_JOIN,1)
	end
end

function changeCamp(player)
	local pName = player:get_name()
	local yxbiqimap = player:get_map()
	local redGroupNum = checknumber(yxbiqimap:get_var("redGroup"))
	local blueGroupNum = checknumber(yxbiqimap:get_var("blueGroup"))
	if redGroupNum==blueGroupNum then
		local rand = math.random(1,100)
		if rand >50 then
			blueGroupNum = blueGroupNum + 1
			player:set_team_info(2,"blueGroup")
		else
			redGroupNum = redGroupNum + 1
			player:set_team_info(1,"redGroup")
		end
	elseif redGroupNum>blueGroupNum then
		blueGroupNum = blueGroupNum + 1
		player:set_team_info(2,"blueGroup")
	elseif redGroupNum<blueGroupNum then
		redGroupNum = redGroupNum + 1
		player:set_team_info(1,"redGroup")
	end
	yxbiqimap:set_var("redGroup",redGroupNum)
	yxbiqimap:set_var("blueGroup",blueGroupNum)
	player:set_attack_mode(105)--阵营
end

function addScroe(player,types,noticeAll)
	-- if server.get_temp_var("yxbiqiState") ~="open" then return end
	local map = player:get_map()

	local score = util.ppn(player, const.PP_YXBIQI_SCORE)
	if types =="online" then
		score = score + 10
	elseif types =="killPlayer" then
		score = score + 50
	elseif types =="killMon" then
		local deadMonster = util.decode(server.get_var("deadMonster"))
		if not util.isTable(deadMonster) then deadMonster = {} end
		table.insert(deadMonster,1,os.time())
		server.set_var("deadMonster",util.encode(deadMonster))
		score = score + 5
	elseif types =="leave" then
		score = score/2
	elseif types =="dead" then
		score = score/2
	elseif types =="assist" then
		score = score+ 25
	else
		return
	end
	score = math.floor(score)
	if score <1 then return end
	local pName = player:get_name()
	player:set_param(const.PP_YXBIQI_SCORE,score)

	local chart = 99
	local index
	for i=#chartList,1,-1 do
		if chartList[i].name == pName then
			index = i
			table.remove(chartList,i)
		end
	end
	local guild = player:get_guild()
	table.insert(chartList,{
		name = pName,
		score = score,
		time = #chartList,
		guild = player:get_guild(),
		job = util.getJobId(player),
		gender = player:get_gender(),
		-- fight = player:get_fightpoint(),
		level = player:get_level(),
		seedName = player:get_seed_name(),
		-- weapon = player:get_item_type_by_pos(const.ITEM_WEAPON_POSITION),
		-- cloth = player:get_item_type_by_pos(const.ITEM_CLOTH_POSITION),
		-- wing = util.ppn(player,const.PP_WING_LEVEL),
	})
	table.sort(chartList,function( a,b )
		if a.score==b.score then
			return a.time<b.time
		else
			return a.score>b.score
		end
	end)

	for i,v in ipairs(chartList) do
		if v.name== pName then
			chart = i
			break;
		end
	end
	local result = {}
	result.score = score
	result.chart = chart
	if not noticeAll and (chart<=10 or index<=10 and chart >10) then
		map:map_exe("map.yxbiqi.resetChartView")
	else
		result.scoreChart = table.slice(chartList,1,10)
	end
	for i=1,10 do
		server.set_var("scoreChart"..i,util.encode(chartList[i]))
	end
	local pmap = player:get_map()
	if pmap and pmap:get_id() == "yxbiqi" then
		player:push_lua_table("showyxshacheng",util.encode(result))
	end
end

--全部排名更新
function resetChartView( player )
	local pmap = player:get_map()
	if pmap and pmap:get_id() == "yxbiqi" then
		local result = {}
		result.scoreChart = table.slice(chartList,1,10)
		player:push_lua_table("showyxshacheng",util.encode(result))
	end
end

function killer(player)
	player:set_param(const.PP_YXBIQI_KILLER,util.ppn(player, const.PP_YXBIQI_KILLER)+1)
end

function killed(player)
	player:set_param(const.PP_YXBIQI_KILLED,util.ppn(player, const.PP_YXBIQI_KILLED)+1)
end
-- 1	23120001,23030001,23020001	20,4,5	1,1,1
-- 2	23120001,23030001,23020001	20,3,3	1,1,1
-- 3	23120001,23030001,23020001	18,3,2	1,1,1
-- 4	23120001,23030001,23020001	18,3,1	1,1,1
-- 5	23120001,23030001,23020001	16,3,1	1,1,1
-- 6	23120001,23030001	16,2	1,1
-- 7	23120001,23030001	15,2	1,1
-- 8	23120001,23030001	14,2	1,1
-- 9	23120001,23030001	13,2	1,1
-- 10	23120001,23030001	12,2	1,1
-- 1000	23120001,23010001,23060001	8,3,1	1,1,1--参与奖
-- 2000	23120001,23010001,23060001	10,3,1	1,1,1--击杀奖
-- 3000	23120001,23060001	2,1	1,1 --被杀奖

local chartAward = {
	{ chart = "1-1",award ={
			{id = 23120001, num = 20,	name="低级荣誉令牌",	bind = 1},
			{id = 23030001,	num = 4, 	name="狼牙碎片（小）",	bind = 1},
			{id = 23020001,	num = 5, 	name="龙心碎片（小）",	bind = 1},
		},
	},
	{ chart = "2-2",award ={
			{id = 23120001, num = 20,	name="低级荣誉令牌",	bind = 1	},
			{id = 23030001,	num = 3, 	name="狼牙碎片（小）",	bind = 1},
			{id = 23020001,	num = 3, 	name="龙心碎片（小）",	bind = 1},
		},
	},
	{ chart = "3-3",award ={
			{id = 23120001, num = 18,	name="低级荣誉令牌",	bind = 1	},
			{id = 23030001,	num = 3, 	name="狼牙碎片（小）",  bind = 1},
			{id = 23020001,	num = 2, 	name="龙心碎片（小）",  bind = 1},
		},
	},
	{ chart = "4-4",award ={
			{id = 23120001, num = 18,	name="低级荣誉令牌",	bind = 1},
			{id = 23030001,	num = 3, 	name="狼牙碎片（小）",bind = 1},
			{id = 23020001,	num = 1, 	name="龙心碎片（小）",bind = 1},
		},
	},
	{ chart = "5-5",award ={
			{id = 23120001, num = 16,	name="低级荣誉令牌",	bind = 1	},
			{id = 23030001,	num = 3, 	name="狼牙碎片（小）",bind = 1},
			{id = 23020001,	num = 1, 	name="龙心碎片（小）",bind = 1},
		},
	},
	{ chart = "6-6",award ={
			{id = 23120001, num = 16,	name="低级荣誉令牌",bind = 1		},
			{id = 23030001,	num = 2, 	name="狼牙碎片（小）",bind = 1},
		},
	},
	{ chart = "7-7",award ={
			{id = 23120001, num = 15,	name="低级荣誉令牌",	bind = 1	},
			{id = 23030001,	num = 2, 	name="狼牙碎片（小）",bind = 1},
		},
	},
	{ chart = "8-8",award ={
			{id = 23120001, num = 14,	name="低级荣誉令牌",	bind = 1	},
			{id = 23030001,	num = 2, 	name="狼牙碎片（小）",bind = 1},
		},
	},
	{ chart = "9-9",award ={
			{id = 23120001, num = 13,	name="低级荣誉令牌",	bind = 1	},
			{id = 23030001,	num = 2, 	name="狼牙碎片（小）",bind = 1},
		},
	},
	{ chart = "10-10",award ={
			{id = 23120001, num = 12,	name="低级荣誉令牌",	bind = 1	},
			{id = 23030001,	num = 2, 	name="狼牙碎片（小）",bind = 1},
		},
	},
}

local function genPos(map)
	local x,y = map:gen_rand_position()
	if math.abs(x-45)<6 and math.abs(y-45)<6 then
		return genPos(map)
	else
		return x,y
	end
end

function genMonster(monNum)
	monNum = monNum or monConf.num
	local mapyx = server.find_map("yxbiqi")
	if mapyx then
		for i = 1,monNum do
			local x,y = genPos(mapyx)
			mapyx:mon_gen(x,y,monConf.monId,1,monConf.range,1,1);--x,y,name,num,range,time,nolive,flags,tip,limitlv
			mapyx:alert(1,0,"1只怪物已复活，当前地图剩余"..mapyx:num_monster().."只怪！")
		end
	end
end

function sendAwardEmail(player)
	local score = util.ppn(player, const.PP_YXBIQI_SCORE)
	local joinTime = util.ppn(player, const.PP_YXBIQI_JOIN_TIME)
	local killernum = util.ppn(player, const.PP_YXBIQI_KILLER)
	local killednum = util.ppn(player, const.PP_YXBIQI_KILLED)

	-- print("sendAwardEmail---------",score,joinTime)
	player:set_param(const.PP_YXBIQI_SCORE,0)--已领奖
	player:set_param(const.PP_YXBIQI_JOIN,0)--已领奖
	player:set_param(const.PP_YXBIQI_JOIN_TIME,0)
	player:set_param(const.PP_YXBIQI_KILLER,0)
	player:set_param(const.PP_YXBIQI_KILLED,0)

	if joinTime >= 5*60 then
		util.sendMail(player, "夜袭比奇参与奖", "您参与夜袭比奇城活动，获得丰厚奖励，如下", joinAward)
	end
	if killernum>=10 then
		util.sendMail(player, "夜袭比奇击杀奖", "您在夜袭比奇城活动中骁勇善战，击杀"..killernum.."人，奖励如下", killeraward)
	end
	if killednum>=1 then
		util.sendMail(player, "夜袭比奇历练奖", "您在夜袭比奇城活动中，曾不幸牺牲，奖励如下", killedaward)
	end
	for i=1,10 do
		local scoreChart = server.get_var("scoreChart"..i)
		scoreChart = util.decode(scoreChart)
		if util.isTable(scoreChart) then
			if scoreChart.name == player:get_name() then
				if chartAward[i] then
					util.sendMail(player, "夜袭比奇排名奖", "您在夜袭比奇城活动中表现优异，获得第"..i.."名，奖励如下", chartAward[i].award)
					server.set_var("scoreChart"..i,"")
				end
				break
			end
		end
	end
end

function loginCheckAward(player)
	if util.ppn(player, const.PP_YXBIQI_JOIN) >0 then
		sendAwardEmail(player)
	end
end
login.add_login_listener(loginCheckAward)

function showChartPanel(player)
	local result={}
	result.name = "panel_chart_yx"
	result.extend = {}
	result.extend.score = util.ppn(player, const.PP_YXBIQI_SCORE)
	result.extend.scoreChart = {}
	for i=1,10 do
		local scoreCharti = server.get_var("scoreChart"..i)
		scoreCharti = util.decode(scoreCharti)
		if type(scoreCharti) =="table" then
			table.insert(result.extend.scoreChart,scoreCharti)
		end
	end
	result.extend.mychart = 0
	for i,v in ipairs(chartList) do
		if v.name== player:get_name() then
			result.extend.mychart = i
			break;
		end
	end
	result.extend.callFunc = "npc.yxshacheng.btnExitSure"
	result.extend.book = ""
	player:push_lua_table("open",util.encode(result))
	player:set_timer(6,60*1000)--一分钟后自动回城
end
--[[
function checkParams(player)

	player:set_param(const.PP_YXBIQI_SCORE,1000)
	player:set_param(const.PP_YXBIQI_JOIN_TIME,10*60)
	player:set_param(const.PP_YXBIQI_JOIN,1)

	addScroe(player,"killPlayer")
	for i=1,10 do
		killer(player)
	end
	killed(player)
	for i=1,10 do
		local scoreCharti = server.get_var("scoreChart"..i)
		print("scoreCharti=",scoreCharti)
	end
	sendAwardEmail(player)
end
server.server_exe("map.yxbiqi.checkParams")
--]]