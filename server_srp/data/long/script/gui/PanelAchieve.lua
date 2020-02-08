module(..., package.seeall)

local equipPos = {
	const.ITEM_WEAPON_POSITION ,
	const.ITEM_CLOTH_POSITION ,
	const.ITEM_HAT_POSITION 	,
	const.ITEM_NICKLACE_POSITION,
	const.ITEM_GLOVE1_POSITION ,
	const.ITEM_GLOVE2_POSITION ,
	const.ITEM_RING1_POSITION ,
	const.ITEM_RING2_POSITION ,
	const.ITEM_BELT_POSITION 	,
	const.ITEM_BOOT_POSITION,
}
local STATE = {
	FINISH = 50,
	GOT = 100,
}
local countType = {
	[0] = "保留上个数据继续计数",
	[1] = "重新计数",
}
--[[
	max= true, 取最大值
--]]

function getTaskConfig(taskId)
	return tab.Achieve.getTaskConfig(taskId)
end

function checkDressEquip(player,taskId)
	local config = getTaskConfig(taskId)
	local count = 0
	for k,v in pairs(equipPos) do
		local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
			itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(v)
		if ittype_id then
			local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice,reinLv,funcId,funcParams = player:get_item_base_data(ittype_id)
			if itneedparam >=config.compareId then
				count = count + 1
			end
		end
	end
	return count
end

function checkUpgradEquip(player,taskId)
	local config = getTaskConfig(taskId)
	return util.ppn(player, const["PP_UPGRADE_EQUIP_NUM"..config.compareId])
end

function checkBreakUp(player,taskId)
	local config = getTaskConfig(taskId)
	local lv = gui.PanelBreakUp.getMaxSection(player)
	return lv>=config.compareId and 1 or 0
end

local completeType = {
	[1] = {name = "签到次数", 				event = "sign",					con = const.PP_ACHIEVE_1},
	[2] = {name = "击杀怪物", 				event = "monster",				con = const.PP_ACHIEVE_2},--
	[3] = {name = "击杀BOSS", 				event = "boss",					con = const.PP_ACHIEVE_3},--
	[4] = {name = "进入副本", 				event = "fuben",				con = const.PP_ACHIEVE_4},
	[5] = {name = "使用道具", 				event = "useProperty",			con = const.PP_ACHIEVE_5},
	[6] = {name = "帮会贡献", 				event = "guildDonate",			con = const.PP_ACHIEVE_6},
	[7] = {name = "日常任务", 				event = "dailyTasks",			con = const.PP_ACHIEVE_7},
	[8] = {name = "装备强化次数", 			event = "upgradeTimes",			con = const.PP_ACHIEVE_8, max =true,},--
	[9] = {name = "击杀几名玩家", 			event = "killPlayer",			con = const.PP_ACHIEVE_9},
	[10] = {name = "升到指定等级", 			event = "level",				con = const.PP_ACHIEVE_10},--
	[11] = {name = "npc对话", 				event = "npcTalk",				con = const.PP_ACHIEVE_11},
	[12] = {name = "指定地图击杀怪物", 		event = "mapKill",				con = const.PP_ACHIEVE_12},
	[13] = {name = "上缴指定物品", 			event = "donateItem",			con = const.PP_ACHIEVE_13},
	[14] = {name = "穿戴指定等级装备", 		event = "dressEquip",			con = const.PP_ACHIEVE_14,	max = true, check = function(player,taskId) return checkDressEquip(player,taskId) end},--穿戴60级
	[15] = {name = "购买指定道具", 			event = "buyProperty",			con = const.PP_ACHIEVE_15},
	[16] = {name = "击杀指定怪物收集道具", 	event = "collect",				con = const.PP_ACHIEVE_16},
	[17] = {name = "激活宝石", 				event = "startJewel",			con = const.PP_ACHIEVE_17},
	[18] = {name = "加入或创建公会", 		event = "joinOrCreateGuild",	con = const.PP_ACHIEVE_18},--
	[19] = {name = "人物转生", 				event = "zhuansheng",			con = const.PP_ACHIEVE_19},--
	[20] = {name = "加入或创建队伍", 		event = "joinOrCreateGroup",	con = const.PP_ACHIEVE_20},--
	[21] = {name = "添加好友", 				event = "addFriend",			con = const.PP_ACHIEVE_21},--
	[22] = {name = "玉佩升到指定等级", 		event = "yupei",				con = const.PP_ACHIEVE_22, check = function(player) return gui.PanelFurnace.getFurnaceLevelByType(player,1) end},--
	[23] = {name = "护盾升到指定等级", 		event = "hudun",				con = const.PP_ACHIEVE_23, check = function(player) return gui.PanelFurnace.getFurnaceLevelByType(player,2) end},--
	[24] = {name = "龙心升到指定等级", 		event = "longxin",				con = const.PP_ACHIEVE_24, check = function(player) return gui.PanelFurnace.getFurnaceLevelByType(player,3) end},--
	[25] = {name = "狼牙升到指定等级", 		event = "langya",				con = const.PP_ACHIEVE_25, check = function(player) return gui.PanelFurnace.getFurnaceLevelByType(player,4) end},--
	[26] = {name = "翅膀升到指定等级", 		event = "wing",					con = const.PP_ACHIEVE_26, check = function(player) return util.ppn(player, const.PP_WING_LEVEL) end},--
	[27] = {name = "角色死亡", 				event = "death",				con = const.PP_ACHIEVE_27},
	[28] = {name = "拥有资源", 				event = "resource",				con = const.PP_ACHIEVE_28},
	[29] = {name = "强化指定星级数量", 		event = "upgradeStars",			con = const.PP_ACHIEVE_29},
	[30] = {name = "进入指定地图", 			event = "mapEnter",				con = const.PP_ACHIEVE_30},
	[31] = {name = "使用聚灵珠", 			event = "useJulingzhu",			con = const.PP_ACHIEVE_31},
	[32] = {name = "门派挑战次数", 			event = "jobChallengeTimes",	con = const.PP_ACHIEVE_32},
	[33] = {name = "门派排名", 				event = "jobChart",				con = const.PP_ACHIEVE_33},
	[34] = {name = "门派排名次数", 			event = "jobChartTimes",		con = const.PP_ACHIEVE_34},
	[35] = {name = "闯天关关卡数", 			event = "Breakup",				con = const.PP_ACHIEVE_35,check = function(player,taskId) return checkBreakUp(player,taskId) end},--
	[36] = {name = "登录天数", 				event = "login",				con = const.PP_ACHIEVE_36, btn= false},--
	[37] = {name = "连续登录天数", 			event = "loginContinue",		con = const.PP_ACHIEVE_37, btn= false},--
	[38] = {name = "击杀指定组别怪物",		event = "killgroupmonster" , 	con = const.PP_ACHIEVE_38,},--
	[39] = {name = "帮会捐献金币次数",		event = "guildDonateCoin" , 	con = const.PP_ACHIEVE_39},
	[40] = {name = "登录游戏",				event = "logingame" , 			con = const.PP_ACHIEVE_40, btn= false},
	[41] = {name = "完成日常声望任务次数",	event = "dailySWTaskTimes" , 	con = const.PP_ACHIEVE_41},
	[42] = {name = "每日挖宝次数",			event = "wabaoTimes" , 			con = const.PP_ACHIEVE_42},
	[43] = {name = "累计挖宝次数",			event = "wabaoTotalTimes" , 	con = const.PP_ACHIEVE_43},
	[44] = {name = "完成挖矿任务次数",		event = "wabaoTaskTimes" , 		con = const.PP_ACHIEVE_44},
	[45] = {name = "商城购买任意商品次数",	event = "storeBuy" , 			con = const.PP_ACHIEVE_45},
	[46] = {name = "进入材料副本次数",		event = "cailiaoEnterTimes" , 	con = const.PP_ACHIEVE_46},
	[47] = {name = "进入闯天关次数",		event = "breakupEnterTimes" , 	con = const.PP_ACHIEVE_47},
	[48] = {name = "押镖次数",				event = "dartTimes" , 			con = const.PP_ACHIEVE_48},
	[49] = {name = "装备强化达到等级",		event = "upEquipToLevel" , 		con = const.PP_ACHIEVE_49, check = function(player,taskId) return checkUpgradEquip(player,taskId) end},
}

local achieveAwards = {
	[1] ={name="经验加成" ,	check = function(id) id = math.floor(id/1000) return id==20 end},
	[2] ={name="攻击加成" ,	check = function(id) id = math.floor(id/1000) return id==21 or id==30 or id == 31 end},
	[3] ={name="功勋加成" ,	check = function(id) id = math.floor(id/1000) return false end},
	[4] ={name="在线奖励" ,	check = function(id) id = math.floor(id/1000) return false end},
	[5] ={name="其他" ,		check = function(id) id = math.floor(id/1000) return id==23 or id ==24 end},
}
local function getTaskNum(player,typeId,taskId)
	local num = 0
	if completeType[typeId] then
		if completeType[typeId].check then
			num = completeType[typeId].check(player,taskId)
			player:set_param(completeType[typeId].con,num)
		else
			num = util.ppn(player, completeType[typeId].con)
		end
	end
	return num;
end

function loadTasks(player)
	local taskstateStr = player:get_param(const.PP_TASKS_STATE);
	local taskfinishStr = player:get_param(const.PP_ACHIEVE_FINISH);
	local taskActivateStr = player:get_param(const.PP_ACHIEVE_ACTIVATION);----激活过的成就
	local taskstates = util.splitString(taskstateStr,";");
	local tasks,config = {}
	-- print("cur---",taskstateStr)
	-- print("finish---",taskfinishStr)
	-- print("activate---",taskActivateStr)
	if #taskstates>0 then
		for i,v in ipairs(taskstates) do
			config = getTaskConfig(v)
			if config then
				config.curNum = getTaskNum(player,config.completeType,config.taskId)
				if config.curNum>=config.num then
					config.activate = string.find(taskActivateStr,config.taskId)
				end
				table.insert(tasks,config)
			end
		end
	else
		for k,v in pairs(tab.Achieve.Data) do
			if v.preId == 0 then
				v.curNum = getTaskNum(player,v.completeType,v.taskId)
				if v.curNum>=v.num then
					v.activate = string.find(taskActivateStr,v.taskId)
				end
				taskstateStr = taskstateStr..string.format("%s;",v.taskId)
				table.insert(tasks,v)
			end
		end
		player:set_param(const.PP_TASKS_STATE,taskstateStr)
	end
	return tasks
end

function checkTaskFinish(player)
	local tasks = loadTasks(player)
	local taskfinishStr = player:get_param(const.PP_ACHIEVE_FINISH);
	for i,v in ipairs(tasks) do
		if getTaskNum(player,v.completeType,v.taskId)>=v.num then
			if not string.find(taskfinishStr,v.taskId) then
				taskfinishStr = taskfinishStr..";"..v.taskId
				checkAchieveAlert(player, v)
			end
		end
	end
	player:set_param(const.PP_ACHIEVE_FINISH,taskfinishStr);
end

function triggerTaskEvent(player,event,value)
	for i,v in ipairs(completeType) do
		if v.event == event then
			local pvalue = util.ppn(player, v.con)
			value = checknumber(value)
			if value>0 then
				if v.max then
					player:set_param(v.con,math.max(value,pvalue))
				else
					player:set_param(v.con,value)
				end
			else
				player:set_param(v.con,pvalue+1)
			end
			break
		end
	end
	checkTaskFinish(player)
end
----激活成就
function checkAchieveMoneyEnough(player,taskId)
	local config = getTaskConfig(taskId)
	if config and getTaskNum(player,config.completeType,config.taskId)>=config.num then
		local enough = false
		local gamemoneybind = player:get_gamemoney_bind()
		local gamemoney = player:get_gamemoney()
		if gamemoney+gamemoneybind >= config.coin then
			enough = true
			player:push_lua_table("PanelAchieve",util.encode({
				cmd = "moneyenough",
				taskId = taskId,
			}))
		else
			util.alert_msg(4, "绑定金币不足，无法激活！", player)
		end
		return enough
	end
end
function activateTask( player,taskId )
	local taskActivateStr = player:get_param(const.PP_ACHIEVE_ACTIVATION);
	if string.find(taskActivateStr,taskId) then
		return util.alert_msg(4, "成就已激活", player)
	end
	local config = getTaskConfig(taskId)
	if config and getTaskNum(player,config.completeType,config.taskId)>=config.num then
		local enough = false
		if player:sub_gamemoney_bind(config.coin) ==1 then
			enough = true
		else
			local gamemoneybind = player:get_gamemoney_bind()
			local gamemoney = player:get_gamemoney()
			if gamemoney+gamemoneybind >= config.coin then
				enough = true
				if config.coin>gamemoneybind then
					player:sub_gamemoney_bind(gamemoneybind)
					player:sub_gamemoney(config.coin-gamemoneybind)
				else
					player:sub_gamemoney_bind(config.coin)
				end
			end
		end
		if enough then
			taskActivateStr = taskActivateStr..";"..taskId
			player:set_param(const.PP_ACHIEVE_ACTIVATION,taskActivateStr)

			--取下一个成就,下次打开成就面板显示
			local nextTaskConfig = getTaskConfig(config.nextId)
			local taskstateStr = player:get_param(const.PP_TASKS_STATE);--载入当前成就任务
			if nextTaskConfig then
				if checknumber(nextTaskConfig.countType)>0 then
					--重新计数
					player:set_param(completeType[nextTaskConfig.completeType].con,0)
				end
				taskstateStr = string.gsub(taskstateStr,taskId,config.nextId)
			else
				taskstateStr = string.gsub(taskstateStr,taskId,"")
			end

			changeFuncStatus(player,taskId)
			taskstateStr = string.gsub(taskstateStr,";;",";")
			player:set_param(const.PP_TASKS_STATE,taskstateStr)
			util.alert_msg(4,"成就["..config.name.."]已激活！", player)
			local result = {
				cmd = "activate",
				taskId = taskId,
			}
			player:push_lua_table("PanelAchieve",util.encode(result))

			addAchieveScore(player,config.achieveValue)--增加成就值
			gui.moduleRedPoint.checkAchieveUsable(player)

			local statuesStr,n = player:get_param(const.PP_ACHIEVE_STATUE)
			if config.preId>0 then
				statuesStr,n = string.gsub(statuesStr,config.preId,"")
			end
			statuesStr = statuesStr..";"..taskId
			statuesStr,n = string.gsub(statuesStr,";;",";")
			player:set_param(const.PP_ACHIEVE_STATUE,statuesStr)
		else
			util.alert_msg(4, "金币不足，无法激活成就", player)
		end
	else
		util.alert_msg(4, "成就未完成，无法激活", player)
	end
end

function changeFuncStatus(player,taskId)
	local config = getTaskConfig(taskId)
	if config then
		local preConfig = getTaskConfig(config.preId)
		if preConfig then
			if preConfig.param1>0 then
				player:rem_obj_func(preConfig.param1)
			end
			if preConfig.param2>0 then
				player:rem_obj_func(preConfig.param2)
			end
		end
		if config.param1>0 then
			player:add_obj_func(config.param1)
		end
		if config.param2>0 then
			player:add_obj_func(config.param2)
		end
	end
end

function loginResetStatues(player)
	local statuesStr = player:get_param(const.PP_ACHIEVE_STATUE)
	local t = string.split(statuesStr,";")
	for k,v in pairs(t) do
		if checkint(v)>0 then
			local config = getTaskConfig(v)
			if config then
				local funcs = tab.funcsdef[config.param1]
				if funcs and funcs.reCal ==1 then
					changeFuncStatus(player,v)
				end
			end
		end
	end
end
login.add_login_listener(loginResetStatues)

function onPanelData(player, commond)
	local data =util.decode(commond);
	if data.actionid == "updateList" then
		updateList(player);
	elseif data.actionid == "updateXunzhang" then
		updateMedalList(player);
	elseif data.actionid == "checkmoney" then
		checkAchieveMoneyEnough(player,data.taskId)
	elseif data.actionid == "activation" then
		if data.taskId then
			activateTask(player,data.taskId)
		end
	elseif data.actionid == "achieveAward" then
		achieveAward(player,data.tag)
	elseif data.actionid == "btn_go" and data.taskId then
		taskGoCallback(player,data.taskId)
	elseif data.actionid == "upgradeXZ" then
		upgradeXunZhang(player);
	elseif data.actionid == "buy" then
		onShopBuy(player,data.param[1])
	elseif data.actionid =="getTabList" then
		getTabList(player)
	elseif data.actionid =="updateAchieveWithId" then
		updateAchieveWithId(player,data.taskId)
	end
end
ckpanel.add_listener("gui.PanelAchieve.onPanelData",gui.PanelAchieve.onPanelData)

function updateList(player)
	local result = {cmd = "updateList"}
	local taskinfos = loadTasks(player)
	result.data = {}
	for k,v in pairs(taskinfos) do
		table.insert(result.data,{
			taskId = v.taskId,
			name = v.name,
			activate = 0,
			huizhang = v.huizhang,
			curNum = getTaskNum(player,v.completeType,v.taskId),
			num = v.num
		})
		local nextTaskConfig = getTaskConfig(v.nextId)
		if nextTaskConfig then
			table.insert(result.data,{
				taskId = nextTaskConfig.taskId,
				name = nextTaskConfig.name,
				activate = 0,
				huizhang = nextTaskConfig.huizhang,
				curNum = 0,
				num = v.num,
				preShow = 1,
			})
		end
	end
	local taskActivateStr = player:get_param(const.PP_ACHIEVE_ACTIVATION);----激活过的成就
	local activatetasks = string.split(taskActivateStr,";")
	for k,v in pairs(activatetasks) do
		local config = getTaskConfig(tonumber(v))
		if config then
			table.insert(result.data,{
				taskId = config.taskId,
				name = config.name,
				activate = 1,
				huizhang = config.huizhang,
				curNum = config.num,
				num = config.num,
			})
		end
	end
	result.score = util.ppn(player, const.PP_ACHIEVE_INTEGRAL)
	player:push_lua_table("PanelAchieve",util.encode(result))
end

function updateAchieveWithId(player,taskId)
	local result = {cmd = "updateAchieveWithId"}
	local config = getTaskConfig(taskId)
	if config then
		result.data = config
		result.data.btn = true
		if completeType[config.completeType].btn == false then
			result.data.btn = false
		end
		result.data.curNum = getTaskNum(player,config.completeType,config.taskId)
		local taskstateStr = player:get_param(const.PP_TASKS_STATE);
		result.data.preShow = not string.find(taskstateStr,config.taskId)
		local taskActivateStr = player:get_param(const.PP_ACHIEVE_ACTIVATION);----激活过的成就
		result.data.activate = string.find(taskActivateStr,config.taskId)
		if result.data.preShow then
			result.data.curNum = 0
		elseif result.data.activate then
			result.data.curNum = config.num
		end
		player:push_lua_table("PanelAchieve",util.encode(result))
	end
end

function updateMedalList(player,up)
	local result = {cmd = "updateXunzhang"}
	result.data = tab.Achieve.getMedalConfig()
	result.score = util.ppn(player, const.PP_ACHIEVE_INTEGRAL)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
	itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount,mZLevel,mFuncId,mFuncParams = player:get_item_data_by_pos(const.ITEM_ACHIEVE_MEDAL_POSITION)

	result.medalLv = 0
	if ittype_id then
		result.medalLv = ittype_id%100-- util.ppn(player, const.PP_MEDAL_LEVEL)
	end	
	result.scoreMax = tab.Achieve.getMedalValue(util.bound(1,result.medalLv+1,12))
	result.breath = gui.PanelFurnace.checkXunZhangUsable(player)
	
	result.medalShop = tab.Store.getItemsByType(player,const.storeType.medal)
	result.up = up
	player:push_lua_table("PanelAchieve",util.encode(result))
end

function getTabList(player)
	local result = {cmd ="getTabList"}
	result.data = {}
	local taskActivateStr = player:get_param(const.PP_ACHIEVE_ACTIVATION);
	local taskIds = util.splitString(taskActivateStr, ";")
	local tasks = {}
	for k,v in pairs(taskIds) do
		if v and #v>0 then
			local config = getTaskConfig(v)
			if config then
				table.insert(tasks,config)
			end
		end
	end
	for k,v in pairs(achieveAwards) do
		result.data[k] = false
		for _,conf in pairs(tasks) do
			if conf and conf.param1 and v.check(conf.param1) then
				result.data[k] = true
			end
		end
	end

	player:push_lua_table("PanelAchieveAward",util.encode(result))
end

function achieveAward(player,tag)
	local result = {cmd = "updateAwardList"}
	result.data = {}
	local taskActivateStr = player:get_param(const.PP_ACHIEVE_ACTIVATION);
	local taskIds = util.splitString(taskActivateStr, ";")
	local tasks = {}
	for k,v in pairs(taskIds) do
		if v and #v>0 then
			local config = getTaskConfig(v)
			if config then
				table.insert(tasks,config)
			end
		end
	end
	local acv = achieveAwards[tag]
	if acv then
		for _,conf in pairs(tasks) do
			if conf and conf.param1 and acv.check(conf.param1) then
				table.insert(result.data,conf)
			end
		end
	end

	player:push_lua_table("PanelAchieveAward",util.encode(result))
end

function taskGoCallback(player,taskId)
	local config = getTaskConfig(taskId)
	if config and completeType[config.completeType] then
		local event = completeType[config.completeType].event
		if event == "sign" then
			player:push_lua_table("open",util.encode({name = "extend_awardHall"}))

		elseif event == "monster" then
			posnpc.fly(player,4002013)
			--[[
			local maps = {}
			local lv = player:get_level()
			local zsLv = util.ppn(player, const.PP_ZHSH_LEVEL)
			for k,v in pairs(tab.MapInfo) do
				if v.minLv<=lv and v.minLvR<=zsLv and v.mapType==2 then
					table.insert(maps,{mapid = v.mapid,lv = v.minLv, zsLv = v.minLvR})
				end
			end
			table.sort(maps,function(a,b)
				if a.zsLv == b.zsLv then
					return a.zsLv<b.zsLv
				else
					return a.lv <b.lv
				end
			end)
			local map = server.find_map(maps[1].mapid)
			if map then
				local x,y = map:gen_rand_position()
				player:enter_map(map:get_id(),x,y)
				player:find_road_goto(map:get_id(),x,y, "autofightstart")				
			end
			]]
		elseif event == "boss" then
			player:push_lua_table("open",util.encode({name = "btn_main_boss",extend = {tab = 3,bossId = config.compareId}}))
		elseif event == "fuben" then
			if config.compareId>0 then
				posmap.fly(player,config.compareId)
			end
		elseif event == "useProperty" then
		elseif event == "guildDonate"		 then
		elseif event == "dailyTasks"		 then
		elseif event == "upgradeTimes"		 then
			player:push_lua_table("open",util.encode({name = "main_forge",extend = {tab = 1}}))			
		elseif event == "killPlayer"		 then
		elseif event == "level"			 then
		elseif event == "npcTalk"			 then
		elseif event == "mapKill"			 then
		elseif event == "donateItem"		 then
			player:push_lua_table("open",util.encode({name = "menu_guild"}))
		elseif event == "dressEquip"		 then
			-- player:push_lua_table("open",util.encode({name = "menu_bag"}))
			posnpc.fly(player,4002019)
		elseif event == "buyProperty"		 then
		elseif event == "collect"			 then
		elseif event == "startJewel"		 then
		elseif event == "joinOrCreateGuild" then
			player:push_lua_table("open",util.encode({name = "menu_guild"}))
		elseif event == "zhuansheng"		 then
			player:push_lua_table("open",util.encode({name = "menu_equip",tab = 2}))
		elseif event == "joinOrCreateGroup" then
			player:push_lua_table("open",util.encode({name = "menu_team"}))
		elseif event == "addFriend"		 then
			player:push_lua_table("open",util.encode({name = "menu_friend"}))
		elseif event == "yupei"			 then
			player:push_lua_table("open",util.encode({name = "main_furnace",tab = 1}))
		elseif event == "hudun"			 then
			player:push_lua_table("open",util.encode({name = "main_furnace",tab = 2}))
		elseif event == "longxin"			 then
			player:push_lua_table("open",util.encode({name = "main_furnace",tab = 3}))
		elseif event == "langya"			 then
			player:push_lua_table("open",util.encode({name = "main_furnace",tab = 4}))
		elseif event == "wing"				 then
			player:push_lua_table("open",util.encode({name = "menu_wing"}))
		elseif event == "death"			 then
		elseif event == "resource"			 then
		elseif event == "upgradeStars"		 then
		elseif event == "mapEnter"			 then
		elseif event == "useJulingzhu"		 then
		elseif event == "jobChallengeTimes" then
		elseif event == "jobChart"			 then
		elseif event == "jobChartTimes"	 then
		elseif event == "Breakup"			 then
			player:push_lua_table("open",util.encode({name = "extend_breakup"}))
		elseif event == "login"			 then
		elseif event == "loginContinue"	 then
		elseif event == "killgroupmonster"	 then
			for k,v in pairs(tab.Boss.bossPersonal) do
				if string.find(config.completeDesp,v.monName) then
					player:push_lua_table("open",util.encode({name = "btn_main_boss",extend = {tab = 3,needIndex = k}}))
					return
				end
			end
			for k,v in pairs(tab.Boss.WildBoss) do
				if string.find(config.completeDesp,v.monName) then
					player:push_lua_table("open",util.encode({name = "btn_main_boss",extend = {tab = 2,needIndex = k}}))
					return
				end
			end			
		elseif event == "upEquipToLevel"	 then
			player:push_lua_table("open",util.encode({name = "main_forge"}))
		end
	end
end
----------------------------杀怪杀BOSS计数---------------------------------
function checkAchieveMonster(player, mon)
	local tasks = loadTasks(player)
	for _,v in ipairs(tasks) do
		-- local monName = tab.getMonName(v.compareId)
		local mongroup = tab.getMonGroup(mon:get_def_id())
		if v.completeType == 38 and v.compareId == mongroup then
			triggerTaskEvent(player, "killgroupmonster");
		end
		if v.compareId==0 or mon:get_def_id() == v.compareId then --v.compareId>0 and string.find(mon:get_name(),monName)
			if v.completeType == 3 and mon:isboss()==1 then
				triggerTaskEvent(player, "boss");
			end
			if v.completeType == 2 and mon:isboss()~=1 then
				triggerTaskEvent(player, "monster")
			end
		end
	end
end
ckmon.add_all_listener(checkAchieveMonster);

--------------------------------升级（转生）--------------------------------
function checkAchieveLevel(player, lv)
	triggerTaskEvent(player, "level", lv)
end
cklevel.add_listener(checkAchieveLevel)

function addAchieveScore(player,value)
	value = checkint(value)
	local score = util.ppn(player, const.PP_ACHIEVE_INTEGRAL);
	player:set_param(const.PP_ACHIEVE_INTEGRAL,score+value);
	player:alert(100000,1,"获得勋章积分:"..value)

	updateMedalList(player)
end

function upgradeXunZhang(player)
	if gui.PanelFurnace.onUpgrade(player,5) then
		player:set_param(const.PP_MEDAL_LEVEL,util.ppn(player, const.PP_MEDAL_LEVEL)+1)
		--print(util.ppn(player, const.PP_MEDAL_LEVEL))
		updateMedalList(player,true)
	end
end

function onShopBuy(player ,storeId)
	local itemData = tab.Store.buyShopItem(player,storeId,"勋章购买",1,"notinbag")
	if itemData then
		gui.moduleRedPoint.checkXunZhangUsable(player)
	end
end
-------------------------------成就达成和红点提示---------------------------

function checkAchieveAlert(player,config)
	player:alert(1, 0, "恭喜您，已达成 ["..config.name.."] 成就")
	gui.moduleRedPoint.checkAchieveUsable(player)
	--显示成就达成tips
	config.time = os.date("(%Y年%m月%d日)",os.time())
	player:push_lua_table("openTips",util.encode({name = "achieveComplete", config = config}))
end

-------------------------------红点规则判断-----------------------------------
function checkAchieveUsable(player)
	local achieveUsable = false
	local tasks = loadTasks(player)
	for k,v in pairs(tasks) do
		if not v.activate and v.curNum>=v.num then
			achieveUsable = true
			break
		end
	end
	return achieveUsable
end

function reset(player)
	for i=1,49 do
		player:set_param(const.PP_ACHIEVE_1+i-1,0)
	end
	player:set_param(const.PP_TASKS_STATE,"")
	player:set_param(const.PP_ACHIEVE_ACTIVATION,"")
	player:set_param(const.PP_ACHIEVE_FINISH,"")
end