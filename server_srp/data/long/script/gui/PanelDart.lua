module(... ,package.seeall)
--local FREE_TIMES = 3--每次能免费刷新的次数
local ENABLE_TIMES = 2--每日能押镖的次数

--镖车等级和奖励表
local dartTable = {
	[1]={pro=0.5,dartLev=1,limitTime=15,limitLev=50,awardDesp="绑元和经验", idexp=40000001, expNum=500000, idmoney=40000006, moneyNum=1000},
	[2]={pro=0.4,dartLev=2,limitTime=15,limitLev=50,awardDesp="绑元和经验", idexp=40000001, expNum=600000, idmoney=40000006, moneyNum=1200},
	[3]={pro=0.3,dartLev=3,limitTime=15,limitLev=50,awardDesp="绑元和经验", idexp=40000001, expNum=800000, idmoney=40000006, moneyNum=1500},
	[4]={pro=0.2,dartLev=4,limitTime=15,limitLev=50,awardDesp="绑元和经验", idexp=40000001, expNum=1000000, idmoney=40000006, moneyNum=2000},
}

local freshDartWeight =
{
	[1]= {
			weight =
			{
			[1] =0,
			[2] =30,
			[3] =40,
			[4] = 0,
			},
			all=40,
		},
	[2]= {
			weight =
			{
			[1] =0,
			[2] =0,
			[3] =18,
			[4] =20,
			},
			all=20,
		},
	[3]= {
			weight =
			{
			[1] =0,
			[2] =0,
			[3] =20,
			[4] =30,
			},
			all=30,
		},
}

local DART_PRICE = {
	REFRESH = 500000,
	TRANS = 10,
}

local dartName ={
	[1] = "贫民镖车",
	[2] = "白银镖车",
	[3] = "五彩镖车",
	[4] = "紫金镖车",
}

function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "reqDartData" then
		sendToClient(player)
	elseif data.actionid == "reqRefreshDart" then
		refreshDartLevel(player)
	elseif data.actionid == "reqStartDart" then
		startDart(player)
	elseif data.actionid == "reqVCoinRefresh" then
		refreshVCoinDartLevel(player)
	elseif data.actionid == "reqTrans" then
		local result = {
			book = util.encode({actionid = "reqVCoinTrans"}),
			str = "传送至镖车处需要消耗10绑定元宝,是否确定?",
			labelConfirm = "确定",labelCancel = "取消",
			callFunc = "gui.PanelDart.handlePanelData",
		}
		player:push_lua_table("showConfirm",util.encode(result))
	elseif data.actionid == "reqGiveup" then
		local result = {
			book = util.encode({actionid = "reqGiveupConfirm"}),
			str = "取消押送镖车将无法获得奖励,是否确定?",
			labelConfirm = "确定",labelCancel = "取消",
			callFunc = "gui.PanelDart.handlePanelData",
		}
		player:push_lua_table("showConfirm",util.encode(result))
	elseif data.actionid == "reqAuto" then
		--自动押镖
		autoDart(player)
	elseif data.actionid == "reqStopAuto" then
		player:set_timer(21, 0)
	elseif data.actionid == "reqVCoinTrans" then
		transToDart(player)
	elseif data.actionid == "reqGiveupConfirm" then
		giveUpDart(player)
	elseif data.actionid == "initOK" then
		showDartTip(player)
	end
end

function autoDart(player)
	local hp = player:get_dart_hp()
	if hp ~= nil and hp > 0 then
		local map = player:get_dart_map()
		local stop = player:get_dart_reach_stop()
		local x,y
		if stop == 1 then
			x,y = player:get_dart_pos()
		else
			x,y = player:get_dart_follow_pos()
		end
		if map ~= nil then
			player:find_road_goto(map:get_id(),x,y, "")
			player:set_timer(21, 1500)
		end
	end
end

function sendToClient(player)
	local curDartLev = util.ppn(player,const.PP_DART_UP_LEVEL)
	if curDartLev==0 then
		curDartLev=1
		player:set_param(const.PP_DART_UP_LEVEL,1)
	end
	local result = {
		cmd="updateDart",
		curData=dartTable[curDartLev],
		-- times=(FREE_TIMES-util.ppn(player,const.PP_DART_FREE_TIME)).."/"..FREE_TIMES,
		yuTimes=(ENABLE_TIMES-util.ppn(player,const.PP_DART_USE_TIME)).."/"..ENABLE_TIMES,
	}
	player:push_lua_table("PanelDart",util.encode(result))
end

function refreshDartLevelOp(player)
	local curDartLev = util.ppn(player,const.PP_DART_UP_LEVEL)
	if curDartLev == 0 then
		curDartLev = 1
	end
	local rate = freshDartWeight[curDartLev]
	local randNum = math.random(1,rate.all)
	local lv = 0
	for i = 1, 4 do
		if randNum <= rate.weight[i] then
			lv = i
			break
		end
	end
	if lv == curDartLev then
		player:alert(1,1,"很遗憾！本次镖车等级刷新失败！")
	elseif lv == 4 then
		-- server.info(110,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>刷新到紫金镖车, <a color='#00ff00' href='event:local_FlyTo_4005009' underline='#00ff00'>我也要押镖</a>");
	end
	player:set_param(const.PP_DART_UP_LEVEL,lv)
	sendToClient(player)
	-- if randNum<=itemData.pro*100 then
	-- 	player:set_param(const.PP_DART_UP_LEVEL,util.ppn(player,const.PP_DART_UP_LEVEL)+1)
	-- 	player:alert(1,1,"成功提升镖车等级！")

	-- else
	--
	-- end
end

--刷新镖车
function refreshDartLevel(player)
	local enable_times = util.ppn(player,const.PP_DART_USE_TIME)
	if enable_times >= ENABLE_TIMES then
		player:alert(1,1,"押镖次数已用完无法刷新镖车")
		return
	end

--	local times = util.ppn(player,const.PP_DART_FREE_TIME)
	local curDartLev = util.ppn(player,const.PP_DART_UP_LEVEL)
	if curDartLev>=#dartTable then
		player:alert(1,1,"镖车已达最大等级！")
		return
	end
	-- if times<FREE_TIMES then
	-- 	refreshDartLevelOp(player)
	-- 	player:set_param(const.PP_DART_FREE_TIME,times+1)
	-- else
	local result = {
		book = util.encode({actionid = "reqVCoinRefresh"}),
		str = "是否消耗500000绑定金币(绑定金币不足用金币代替)刷新?",
		labelConfirm = "确定",labelCancel = "取消",checkBox = "下次不显示",
		callFunc = "gui.PanelDart.handlePanelData",
	}

	player:push_lua_table("showConfirm",util.encode(result))
	--end
end

function refreshVCoinDartLevel(player)
	local enable_times = util.ppn(player,const.PP_DART_USE_TIME)
	if enable_times >= ENABLE_TIMES then
		player:alert(1,1,"押镖次数已用完无法刷新镖车")
		return
	end

	local curDartLev = util.ppn(player,const.PP_DART_UP_LEVEL)
	if curDartLev>=#dartTable then
		player:alert(1,1,"镖车已达最大等级！")
		return
	end

	if player:get_gamemoney_bind() + player:get_gamemoney() < DART_PRICE.REFRESH then
		player:alert(1,1,"金币不足！")
		return
	end

	if player:get_gamemoney_bind() > DART_PRICE.REFRESH then
		player:sub_gamemoney_bind(DART_PRICE.REFRESH)
	else
		local left = DART_PRICE.REFRESH-player:get_gamemoney_bind()
		player:sub_gamemoney_bind(player:get_gamemoney_bind())
		player:sub_gamemoney(left)
	end

	refreshDartLevelOp(player)
end

--开始押镖
function startDart(player)
	if get_hour_minute() < 1600 or get_hour_minute() > 1630 then
		player:alert(1,1,"活动时间未到,无法押送镖车！")
		return
	end

	if player:have_dart() ~= 0 then
		player:alert(1,1,"请先完成押镖,再领取镖车！")
		return
	end

	if util.ppn(player,const.PP_DART_SUCCEED_LEVEL) ~= 0 then
		player:alert(1,1,"请先领取押镖奖励,再领取镖车！")
		return
	end

	local times = util.ppn(player,const.PP_DART_USE_TIME)
	if times<ENABLE_TIMES then
		--开始押镖
		local curDartLev = util.ppn(player,const.PP_DART_UP_LEVEL)
		player:set_param(const.PP_DART_CURLEVEL, curDartLev)
		player:set_param(const.PP_DART_UP_LEVEL, 1)

		player:gen_dart(5007001 + curDartLev - 1)
		-- 押镖提示
		if curDartLev > 2 and dartName[curDartLev] then
			server.info(10,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>开始押送<font color='#ff0000'>"..dartName[curDartLev].."</font>, 各位与此人有仇的快去劫镖啊！<a color='#00ff00' href='event:local_FlyTo_4005009' underline='#00ff00'>我要押镖</a>");
			server.info(100,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>开始押送<font color='#ff0000'>"..dartName[curDartLev].."</font>, 各位与此人有仇的快去劫镖啊！");
		end

		-- server.info(110,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>刷新到紫金镖车, <a color='#00ff00' href='event:local_FlyTo_4005009' underline='#00ff00'>我也要押镖</a>");
		-- 自动押镖
		autoDart(player)

		player:set_param(const.PP_DART_USE_TIME,times+1)
		player:push_lua_table("close", util.encode({name = "panel_dart"}));

		local life_time = 15 * 60
		player:set_dart_life_time(life_time)

		--显示押镖信息面板
		local result = {}
		result.time_left = life_time+os.time()
		result.award = getAwardsTable(curDartLev)
		result.show = 1
		player:push_lua_table("dartInfo",util.encode(result))

		gui.PanelGongXun.checkTaskComplete(player,11)
	else
		player:alert(1,1,"今日押镖次数已用完！")
	end
end

function showDartTip(player)
	if player:have_dart() == 0 then
		return
	end

	local time_left = player:get_dart_life_time()
	if time_left <= 0 then
		return
	end

	local curDartLev = util.ppn(player,const.PP_DART_CURLEVEL)
	local result = {}
	result.time_left = time_left+os.time()
	result.award = getAwardsTable(curDartLev)
	result.show = 1
	local hp,maxhp = player:get_dart_hp()
	result.dart_hp = hp
	result.dart_maxhp = maxhp

	player:push_lua_table("dartInfo",util.encode(result))
end

function getAwardsTable(index)
	if index > 0 and index <=4 then
		local awards = {
			{id = 0, num = 0},
			{id = 0, num = 0},
			}
		awards[1].id = dartTable[index].idmoney
		awards[1].num = dartTable[index].moneyNum
		awards[2].id = dartTable[index].idexp
		awards[2].num = dartTable[index].expNum
		return awards
	end
end

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>友情提醒：</font>",
	},
	strs2={

		"<font color=#b2a58b>您还有一次押送镖车的机会哦！</font>",
	},
	resData={talkTitle="日常押镖",btnName="再来一次",},
	func = "gui.PanelDart.gotoDartNpc",
}


function getDartAwards(player, curDartLev, rate)
	if rate <= 0 then
		rate = 1
	end
	if curDartLev > 0 and curDartLev <= 4 then
		local itemData = dartTable[curDartLev]
		if itemData then
			-- if server.get_var("doubleDart")=="open" then ----双倍押镖
			-- 	player:add_exp(itemData.expNum*2)
			-- 	player:add_gamemoney_bind(itemData.moneyNum*2)
			-- 	player:alert(1,1,"获得经验*"..2*itemData.expNum.."、金币*"..2*itemData.moneyNum)
			-- else

			player:add_vcoin_bind(math.floor(itemData.moneyNum * rate))
			player:add_exp(math.floor(itemData.expNum * rate))
			player:alert(1,1,"获得绑定元宝*"..math.floor(itemData.moneyNum * rate).."、经验*"..math.floor(itemData.expNum * rate))
			--end
			if  util.ppn(player,const.PP_DART_USE_TIME)<ENABLE_TIMES then
				player:push_lua_table("npc_echo",util.encode({id=4002002,panelType="common",talk_str = bosssInfo}));
			end
		end
	end
end


function gotoDartNpc(player,data)
	posnpc.fly(player,4005009)
end
ckpanel.add_listener("gui.PanelDart.gotoDartNpc",gui.PanelDart.gotoDartNpc)

function giveUpDart(player)
	if player:have_dart() ~= 0 then
		player:kill_dart()

		local result = {}
		result.hide = 1
		player:push_lua_table("dartInfo",util.encode(result))
	end
end

function transToDart(player)
	if player:get_vcoin_bind() < DART_PRICE.TRANS then
		player:alert(1,1,"绑定元宝不足！")
		return
	end

	player:sub_vcoin_bind(DART_PRICE.TRANS)
	player:directfly_to_dart()
end

ckpanel.add_listener("gui.PanelDart.handlePanelData",gui.PanelDart.handlePanelData)



