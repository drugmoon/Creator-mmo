module(..., package.seeall)
local ACTIVITY_NAME = '精英阁';

local allChart = {}

function onPlayerEnter(map,player)
	local idx = string.gsub(map:get_id(),"[^%d+]","")
	idx = checknumber(idx)
	local preIdx = util.ptpn(player, 999)
	-- print(idx,preIdx)
	if preIdx==0 then
		player:alert(1,0,"恭喜你已进入第"..idx.."层！")
	elseif idx>preIdx then
		player:alert(1,0,"进层成功，恭喜你已进入第"..idx.."层！")
	else
		player:alert(1,0,"进层失败，你已返回第"..idx.."层！")
	end
	player:set_temp_param(999,idx)
	if string.find(map:get_id(),9) then
		local name = player:get_name()
		local c = getChartIndex(player)
		if checkint(c)==0 then
			table.insert(allChart,tostring(name))
		end
		for k=1,10 do
			local cname = server.get_var("jyg"..k) or ""
			if cname == name then break; end
			if cname == "" then
				server.set_var("jyg"..k,name)
				map:map_exe("map.jyg.updataUIleftTips")
				break;
			end
		end
	end
	updataUIleftTips(player)
	-- print("map:enter",map:get_id())
end

function onPlayerLeave(map,player)
	-- print("map:leave",map:get_id())
	if map then
		local result={}
		result.close = true
		player:push_lua_table("showjyg",util.encode(result))
	end
end
function resetChart()
	allChart = {}
end

function getChartIndex(player)
	local c = table.indexof(allChart, player:get_name())
	return c and c or ""
end

local awards = {
	{name="护盾", 	id = 23010001, num = 1,bind = 1},
	{name="jinbi", id = 40000003, num = 1, bind = 1},
}

function updataUIleftTips(player)
	local result={}
	result.title = ACTIVITY_NAME;
	result.btnAction = "npc.jyg.btnExit"
	local openTime = tonumber(server.get_var("jygStartTime")) or os.time()
	result.second = gui.PanelActivityGuide.getActivityTotalSecond( ACTIVITY_NAME ) - os.time() + openTime
	result.serverTime = os.time()
	if string.find(player:get_map():get_id(),10) then
		result.lblinfo = "战斗吧！少年！！！"
	elseif string.find(player:get_map():get_id(),9) then
		result.chartTable = {}
		for h=1,10 do
			table.insert(result.chartTable,server.get_var("jyg"..h))
		end
		result.chart = getChartIndex(player)
	else
		result.lblinfo = "只有一个传送阵可以通往下一层"
		result.awards=awards
	end
	player:push_lua_table("showjyg",util.encode(result))
end

--传送点配置,传送到下一个地图的中心点
local dyCof = {
	["jingying1"] = {
		{id = 1001,cloth = 70001,	x = 14,	y = 28,	targetMap = "jingying1",tx = 21, ty = 33,range = 3},
		{id = 1002,cloth = 70001,	x = 27,	y = 28,	targetMap = "jingying1",tx = 21, ty = 33,range = 3},
		{id = 1003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying1",tx = 21, ty = 33,range = 3},
		{id = 1004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying2",tx = 21, ty = 33,range = 3},
	},
	["jingying2"] = {
		{id = 2001,cloth = 70001,	x = 14,	y = 28, targetMap = "jingying1",tx = 21, ty = 33,range = 3},
		{id = 2002,cloth = 70001,	x = 27,	y = 28, targetMap = "jingying2",tx = 21, ty = 33,range = 3},
		{id = 2003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying2",tx = 21, ty = 33,range = 3},
		{id = 2004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying3",tx = 21, ty = 33,range = 3},
	},
	["jingying3"] = {
		{id = 3001,cloth = 70001,	x = 14,	y = 28, targetMap = "jingying4",tx = 21, ty = 33,range = 3},
		{id = 3002,cloth = 70001,	x = 27,	y = 28, targetMap = "jingying2",tx = 21, ty = 33,range = 3},
		{id = 3003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying2",tx = 21, ty = 33,range = 3},
		{id = 3004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying1",tx = 21, ty = 33,range = 3},
	},
	["jingying4"] = {
		{id = 4001,cloth = 70001,	x = 14,	y = 28, targetMap = "jingying5",tx = 21, ty = 33,range = 3},
		{id = 4002,cloth = 70001,	x = 27,	y = 28, targetMap = "jingying3",tx = 21, ty = 33,range = 3},
		{id = 4003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying3",tx = 21, ty = 33,range = 3},
		{id = 4004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying2",tx = 21, ty = 33,range = 3},
	},
	["jingying5"] = {
		{id = 5001,cloth = 70001,	x = 14,	y = 28, targetMap = "jingying6",tx = 21, ty = 33,range = 3},
		{id = 5002,cloth = 70001,	x = 27,	y = 28, targetMap = "jingying4",tx = 21, ty = 33,range = 3},
		{id = 5003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying4",tx = 21, ty = 33,range = 3},
		{id = 5004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying3",tx = 21, ty = 33,range = 3},
	},
	["jingying6"] = {
		{id = 6001,cloth = 70001,	x = 14,	y = 28, targetMap = "jingying7",tx = 21, ty = 33,range = 3},
		{id = 6002,cloth = 70001,	x = 27,	y = 28, targetMap = "jingying5",tx = 21, ty = 33,range = 3},
		{id = 6003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying5",tx = 21, ty = 33,range = 3},
		{id = 6004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying4",tx = 21, ty = 33,range = 3},
	},
	["jingying7"] = {
		{id = 7001,cloth = 70001,	x = 14,	y = 28, targetMap = "jingying8",tx = 21, ty = 33,range = 3},
		{id = 7002,cloth = 70001,	x = 27,	y = 28, targetMap = "jingying6",tx = 21, ty = 33,range = 3},
		{id = 7003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying6",tx = 21, ty = 33,range = 3},
		{id = 7004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying5",tx = 21, ty = 33,range = 3},
	},
	["jingying8"] = {
		{id = 8001,cloth = 70001,	x = 14,	y = 28, targetMap = "jingying9",tx = 21, ty = 33,range = 3},
		{id = 8002,cloth = 70001,	x = 27,	y = 28, targetMap = "jingying7",tx = 21, ty = 33,range = 3},
		{id = 8003,cloth = 70001,	x = 28,	y = 41,	targetMap = "jingying7",tx = 21, ty = 33,range = 3},
		{id = 8004,cloth = 70001,	x = 14,	y = 41,	targetMap = "jingying6",tx = 21, ty = 33,range = 3},
	},
}
local mapConf = dyCof;

function exchangeTargetMap()
	local rand,newT = 0,{}
	for mapId,sendPointT in pairs(mapConf) do
		newT[mapId] = {}
		for i,v in ipairs(sendPointT) do
			rand = math.random(1,#sendPointT)
			if rand ~= i then
				v.targetMap,sendPointT[rand].targetMap = sendPointT[rand].targetMap,v.targetMap
			end
			table.insert(newT[mapId],v)
		end
	end
	mapConf = newT
end
--绘制传送点
function drawSendPoints()
	removeSendPoints()
	exchangeTargetMap()
	exchangeTargetMap()
	local mapjy
	for mapId,sendPointT in pairs(mapConf) do
		mapjy = server.find_map(mapId)
		if mapjy then
			for i,v in ipairs(sendPointT) do
				mapjy:add_dy_conn(v.id,v.cloth,v.x,v.y,v.targetMap,v.tx,v.ty,v.range)
				-- if checknumber(string.find(v.targetMap,"%d")) == checknumber(string.find(mapId,"%d"))+1 then
				-- 	print(v.id,v.x,v.y,v.targetMap)
				-- end
			end
		end
	end
end
--移除传送点
function removeSendPoints()
	local mapjy
	for mapId,sendPointT in pairs(mapConf) do
		mapjy = server.find_map(mapId)
		if mapjy then
			for i,v in ipairs(sendPointT) do
				mapjy:remove_dy_conn(v.id)
			end
		end
	end
end