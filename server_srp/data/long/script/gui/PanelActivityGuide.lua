module(..., package.seeall)
--openState:(1-未开启 2-进行中 3-等级不足 4-即将开启，5-已结束)
local activityConfigs = {
	{index=1, name="神威魔狱",	repeatDay=0, startTime=1000, endTime=1100, levelLimit=80, open=true, openDate=0,openState=0,},

	{index=2, name="超级瑞兽",	repeatDay=0, startTime=1100, endTime=1130, levelLimit=50, open=true, openDate=0,openState=0,},

	{index=3, name="膜拜城主",	repeatDay=0, startTime=1130, endTime=1230, levelLimit=50, open=true, openDate=0,openState=0,},

	{index=4, name="天下第一",	repeatDay=0, startTime=1240, endTime=1320, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=5, name="双倍经验",	repeatDay=0, startTime=1330, endTime=1630, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=6, name="神威魔狱",	repeatDay=0, startTime=1400, endTime=1500, levelLimit=80, open=true, openDate=0,openState=0,},

	{index=7, name="尸王殿",	repeatDay=0, startTime=1500, endTime=1530, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=8, name="全民押镖",	repeatDay=0, startTime=1600, endTime=1630, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=9, name="精英阁",	repeatDay=0, startTime=1700, endTime=1730, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=10,name="膜拜城主",	repeatDay=0, startTime=1730, endTime=1830, levelLimit=50, open=true, openDate=0,openState=0,},

	{index=11,name="天降赐福",	repeatDay=0, startTime=1900, endTime=1920, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=12,name="双倍经验",	repeatDay=0, startTime=1900, endTime=2100, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=13,name="夜袭比奇",	repeatDay=0, startTime=2120, endTime=2140, levelLimit=60, open=true, openDate=0,openState=0,},

	{index=14,name="神威魔狱",	repeatDay=0, startTime=2200, endTime=2300, levelLimit=80, open=true, openDate=0,openState=0,},
}

local activityInfos = {
	["皇城战"] = {
		awards ={
			[1]={name="元宝",	  id=40000005,num=200},

		},
		name="皇城战",
		flyInfos = {flyId=1001014,},
		explain = "参与王城攻城战，持续占领皇宫的帮会，将在活动结束后，所有帮会成员均可获得奖励。PS：本活动开服第三天开启。",
	},
	["膜拜城主"] = {
		awards ={
			[1]={name="人物经验",	  id=40000001,num=0},

		},
		name="膜拜城主",
		flyInfos = {flyId=2020022,},
		explain = "活动开启后,在土城安全区内可获得<font color=#00ff00>大量经验</font>奖励,等级越高获得经验越多,玩家可通过对城主<font color=#00ff00>使用鲜花和鸡蛋</font>获取更高奖励，<font color=#00ff00>倍数越高</font>奖励越多",
	},
	["精英阁"] = {
		awards ={
			[1]={name="人物经验", id=40000001,num=0},
			[2]={name="战士7转",  id=11114011,num=1},
			[3]={name="法师7转",  id=12114011,num=1},
			[4]={name="道士7转",  id=13114011,num=1},

		},
		name="精英阁",
		flyInfos = {npcId=4005006,},
		explain = "活动期间内，最快到达精英阁九层的玩家有<font color=#00ff00>丰厚经验奖励</font>，精英阁十层更有超级BOSS，击杀可获得<font color=#00ff00>高级装备奖励</font>",
	},
	["天降赐福"] = {
		awards ={
			[1]={name="元宝",	  id=40000005,num=0},
			[2]={name="金币",	  id=40000003,num=0},
			[3]={name="声望",	  id=40000007,num=0},
			[4]={name="人物经验", id=40000001,num=0},
		},
		name="天降赐福",
		flyInfos = {npcId=4005004},
		explain = "活动开启后，只要在活动地图内，即可享受女神普照，获得大量奖励，地图内共有<font color=#00ff00>八个双倍光圈</font>，<font color=#00ff00>每三分钟</font>随机更换位置，击杀其他玩家可获得荣誉值<font color=#00ff00>（击杀低于自身等级10级以上的玩家不获得荣誉值）</font>",
	},
	["夜袭比奇"] = {
		awards ={
			{name="低级荣誉令牌",  id=23120001,num=1},
			{name="龙心碎片（小）",id=23020001,num=1},
			{name="狼牙碎片（小）",id=23030001,num=1},
		},
		name="夜袭比奇",
		flyInfos = {npcId=4005002,},
		explain = "活动时间内，玩家可以<font color=#00ff00>放肆的战斗</font>，击杀其他玩家可以获得<font color=#00ff00>积分</font>，并且获得<font color=#00ff00>大量荣誉</font>，最终根据<font color=#00ff00>积分多少</font>进行<font color=#00ff00>排名</font>发放奖励",
	},
	["全民押镖"] = {
		awards ={
			[1]={name="人物经验",	  id=40000001,num=0},
			[2]={name="绑定元宝",	  id=40000006,num=0},

		},
		name="全民押镖",
		flyInfos = {npcId=4005009,},
		explain = "活动期间内，完成押镖任务获得<font color=#00ff00>经验和绑定元宝</font>，镖车等级越高，奖励越丰富",
	},
	["神威魔狱"] = {
		awards ={
			[1]={name="金币",	  id=40000003,num=0},
			[2]={name="强化石",	  id=24020003,num=0},
			[3]={name="战士9转",  id=11115011,num=1},
			[4]={name="法师9转",  id=12115011,num=1},
			[5]={name="道士9转",  id=13115011,num=1},

		},
		name="神威魔狱",
		flyInfos = {npcId=4005010,},---2020006
		explain = "神威魔狱有<font color=#00ff00>10层</font>，每层均有<font color=#00ff00>BOSS</font>镇守，进入下一层需要击败<font color=#ff0000>【神威魔狱统领】</font>获得<font color=#ff0000>【神威魔狱通行证】</font>,每闯入下一层可以获得<font color=#00ff00>大量金币</font>和<font color=#00ff00>强化石</font>奖励",
	},
	["超级瑞兽"] = {
		awards ={
			[1]={name="绑定元宝",id=40000006,num=0},
			[2]={name="元宝",    id=40000005,num=0},
		},
		name="超级瑞兽",
		flyInfos = {mapId="biqi", x=32, y=86,autofight=false,},
		explain = "BOSS【超级瑞兽】携带大量元宝于每天<font color=#00ff00>11:00-11:30、21:00-21:30</font>出现在比奇城捣乱，击败它可获得<font color=#00ff00>大量元宝、绑定元宝</font>奖励，掉落的所有物品为<font color=#00ff00>自由拾取</font>",
	},
	["天下第一"] = {
		awards ={
			[1]={name="金砖（小）",	 id=23060001,num=0},
			[2]={name="元宝",        id=40000005,num=0},
		},
		name="天下第一",
		flyInfos = {npcId=4005005,},
		explain = "天下第一地图内<font color=#00ff00>自由PK</font>，直至剩下一个玩家，该玩家获得胜利，可获得<font color=#00ff00>大量元宝</font>奖励，其他玩家可获得<font color=#00ff00>大量金币</font>奖励",
	},
	["双倍经验"] = {
		awards ={
			[1]={name="人物经验",	  id=40000001,num=0},

		},
		name="双倍经验",
		-- flyInfos = {flyId=2020013},
		explain = "活动期间击败<font color=#00ff00>任意怪物</font>均可享受<font color=#00ff00>双倍经验</font>奖励，使用多倍经验道具可<font color=#00ff00>叠加</font>经验奖励",
	},
	["尸王殿"] = {
		awards ={
			{name="怒之半月弯刀",	id=26010022, num=0},
			{name="怒之地狱雷光",	id=26020022, num=0},
			{name="怒之圣甲术",	    id=26030022, num=0},
			{name="怒之野蛮冲撞",	id=26010032, num=0},
			{name="怒之火墙",	    id=26020032, num=0},
			{name="怒之群体治疗术",	id=26030042, num=0},

		},
		name="尸王殿",
		flyInfos = {npcId=4005003,},
		explain = "活动地图内可以<font color=#00ff00>自由PK无惩罚、无掉落</font>，总共刷新<font color=#00ff00>6波尸王</font>，消灭尸王可掉落<font color=#00ff00>封印的技能书</font>，持有封印的技能书<font color=#00ff00>3分钟</font>才可真正获得<font color=#00ff00>本职业随机技能书</font>",
	},
}
function onPanelData(player,commond)
	local data = util.decode(commond);
	if not data  then return end
	if data.actionid == "reqActsData" then
		updateClientShow(player)
		-- player:enter_map("v001",110,173)
		if util.ppn(player, const.PP_ACT_OPEN_NUM)<=0 then
			player:set_param(const.PP_ACT_OPEN_NUM,1)
			gui.moduleRedPoint.checkWordAcitivityUsable(player)
		end
	elseif data.actionid == "reqActsAllInfo" then
		senderActsInfo(player)
	elseif data.actionid == "btnGo" then
		startFlyTarget(player,data.nameIndex)
	end
end
ckpanel.add_listener("gui.PanelActivityGuide.onPanelData",gui.PanelActivityGuide.onPanelData)

--活动开始显示活动提示按钮并刷新活动列表界面
function startNotice(player)
	updateClientShow(player)
	local curName = server.get_temp_var("activityName")
	local level = player:get_level()
	if activityInfos[curName] then
		for i=1,#activityConfigs do
			local itemData = activityConfigs[i]
			if itemData.name==curName and level>=itemData.levelLimit then
				player:push_lua_table("activityOpen",util.encode(activityInfos[curName]))
				break
			end
		end

	end
end

 --更新客户端显示
function updateClientShow(player)
	local result = {
		cmd="updateClientShow",
		actsTable={},
	}
	local serverTime = server.get_var("server_start_time")
	local server_day = tonumber(os.date("%Y%m%d",tonumber(serverTime)))
	local now = tonumber(os.date("%Y%m%d",os.time()))
	local time = tonumber(os.date("%H%M",os.time()))
	local level = player:get_level()
	local table1 = {}--开启的活动表
	local table2 = {}--未开启的活动表
	local table3 = {}--已结束的活动表
	local table4 = {}--即将开启的活动表

	local tempTable = util.clone(activityConfigs)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if itemData.open then
			if time>=itemData.endTime then
				itemData.openState=5--已结束
				table.insert(table3,itemData)
			else
				if time>=itemData.startTime then
					itemData.openState=2--进行中
					table.insert(table1,1,itemData)
				else
					if (getMinute(itemData.startTime)-getMinute(time))<=10 then
						itemData.openState=4--即将开始
						table.insert(table4,itemData)
					else
						itemData.openState=1--未开始
						table.insert(table2,itemData)
					end
				end
				if level<itemData.levelLimit then
					itemData.openState=3--等级不足
				end
			end
		end
	end
	local sort = function(a,b)
		if a.startTime == b.startTime then
			if a.endTime==b.endTime then
				return a.levelLimit < b.levelLimit
			else
				return a.endTime <b.endTime
			end
		else
			return a.startTime<b.startTime
		end
	end
	table.sort(table1,sort)
	table.sort(table2,sort)
	table.sort(table3,sort)
	table.sort(table4,sort)
	table.insertto(result.actsTable,table1)
	table.insertto(result.actsTable,table4)
	table.insertto(result.actsTable,table2)
	table.insertto(result.actsTable,table3)
	-- print(util.encode(table3))
	-- print(util.encode(result.actsTable))
	player:push_lua_table("PanelActivityGuide",util.encode(result))
	-- server.info(10010,0,"<font color='#ff0000'>大厅所有人获得荣誉和经验奖励</font>");
end

--推送活动基础信息
function senderActsInfo(player)
	local result = {
		cmd="senderActsInfo",
		actsInfo=util.clone(activityInfos),
	}
	player:push_lua_table("PanelActivityGuide",util.encode(result))
end

--执行传送功能
function startFlyTarget(player,nameIndex)
	local flyInfos = activityInfos[nameIndex].flyInfos
	if not flyInfos then return end
	if flyInfos.flyId then
		-- print(flyInfos.flyId)
		--posnpc.fly(player,flyInfos.flyId)
		posmap.fly(player, flyInfos.flyId)
	elseif flyInfos.npcId then
		posnpc.fly(player,flyInfos.npcId)
	elseif flyInfos.mapId then
		local map=server.find_map(flyInfos.mapId)
		local posX,posY
		if flyInfos.x and flyInfos.y then
			posX,posY = flyInfos.x, flyInfos.y
		elseif map then
			posX,posY=map:gen_rand_position()
		end
		player:enter_map(flyInfos.mapId, posX, posY)
		if flyInfos.autofight then
			player:find_road_goto(flyInfos.mapId, posX, posY, "autofightstart")
		end
	end
	player:push_lua_table("close",util.encode({name = "extend_world"}))
	player:push_lua_table("close",util.encode({name = "panel_acttip"}))
end

function isActivityOpen(name)
	local activities = {}
	for i,v in ipairs(activityConfigs) do
		if v.open and name == v.name then
			table.insert(activities,v)
		end
	end
	local now = tonumber(os.date("%H%M",os.time()))
	-- local date = tonumber(os.date("%Y%m%d",os.time()))
	local isOpen = false
	for k,v in pairs(activities) do
		if v.startTime<=now and now<v.endTime then
			isOpen = true
		end
	end
	return isOpen
end

function getActivityLevelLimit( name )
	local lv = 0
	for i,v in ipairs(activityConfigs) do
		if v.open and name == v.name then
			lv = v.levelLimit
			break
		end
	end
	return lv
end

function getSecond(timenumber)
	return (math.modf(timenumber/100)*60 + timenumber%100)*60
end
function getMinute(timenumber)
	return math.modf(timenumber/100)*60 + timenumber%100
end
function getActivityTotalSecond( name )
	local second = 0
	for i,v in ipairs(activityConfigs) do
		if v.open and name == v.name then
			second = getSecond(v.endTime)-getSecond(v.startTime)
			break
		end
	end
	return second
end

--自动检测10分钟后开启的活动，并计算下一次检测时间间隔秒
function checkPreStartActivity()
	local time = activityConfigs[#activityConfigs].startTime
	local now = tonumber(os.date("%H%M",os.time()))
	local nowSecond = getSecond(now)+tonumber(os.date("%S",os.time()))
	local isNextDay = true
	local tenMinuteOpen = {}
	for k,v in pairs(activityConfigs) do
		if getMinute(now)+10 == getMinute(v.startTime) then
			table.insert(tenMinuteOpen,v)
		end
		if getMinute(now)+10 < getMinute(v.startTime) and time > v.startTime then
			time = v.startTime
			isNextDay = false
		end
	end
	for k,v in pairs(tenMinuteOpen) do
		server.info(10010,0,"<font color='#00ccff'>"..v.name.."</font>将在10分钟后开启!请各位玩家踊跃参加！");
		server.server_exe("gui.PanelActivityGuide.updateClientShow")
	end
	if isNextDay then
		nowSecond = nowSecond - 24*60*60
		time = activityConfigs[1].startTime
	end
	return getSecond(time)-nowSecond-10*60
end

-- print(string.format("%02d:%02d-%02d:%02d",1500/100,1500%100,2100/100,2100%100))
function getNextOpenTimeString(name)
	local str
	local now = tonumber(os.date("%H%M",os.time()))
	local openT = {}
	for i,v in ipairs(activityConfigs) do
		if v.open and name == v.name then
			table.insert(openT,{
				startTime = v.startTime,
				endTime = v.endTime,
			})
		end
	end
	if #openT>1 then
		for i=1,#openT do
			if now<openT[i].endTime then
				str = string.format("%02d:%02d-%02d:%02d",openT[i].startTime/100,openT[i].startTime%100,openT[i].endTime/100,openT[i].endTime%100)
				break
			end
		end
		if not str then
			str = string.format("%02d:%02d-%02d:%02d",openT[1].startTime/100,openT[1].startTime%100,openT[1].endTime/100,openT[1].endTime%100)
		end
	elseif #openT==1 then
		str = string.format("%02d:%02d-%02d:%02d",openT[1].startTime/100,openT[1].startTime%100,openT[1].endTime/100,openT[1].endTime%100)
	end
	return str or ""
end


--活动红点检测
function checkRedPoint(player)
	if util.ppn(player, const.PP_ACT_OPEN_NUM)<=0 then
		local time = tonumber(os.date("%H%M",os.time()))
		local tempTable = util.clone(activityConfigs)
		for i=1,#tempTable do
			local itemData = tempTable[i]
			if itemData.open then
				if time<itemData.endTime and time>=itemData.startTime then
					return true
				end
			end
		end
	end
	return false
end
