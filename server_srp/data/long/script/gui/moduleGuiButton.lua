module(..., package.seeall)

-- 10011	角色
-- 10012	内功
-- 10013	技能
-- 10014	转生
-- 10015	宝石
-- 10021	锻造
-- 10031	神炉
-- 10041	官位
-- 10051	兑换
-- 10061	翅膀
-- 10071	成就
-- 10081	好友
-- 10091	组队
-- 10101	帮会
-- 10111	邮件
-- 10121	寄售
-- 10131	排行榜
-- 20011	首充
-- 20021	每日签到
-- 20022	在线奖励
-- 20023	15日登录
-- 20031	特惠礼包
-- 20032	全民boss
-- 20041	寻宝
-- 20051	商城
-- 20061	激战boss
-- 20071	离线挂机
-- 20081	皇城战
-- 20091	世界活动
-- 20101	闯天关
-- 20111	功勋
-- 20121	护卫
-- 20131	经验炼制


local enmu2ui = {
	[1001] = "btn_main_avatar",
	[1002] = "btn_main_forge",
	[1003] = "btn_main_furnace",
	[1004] = "btn_main_official",
	[1005] = "btn_main_convert",
	[1006] = "btn_main_wing",
	[1007] = "btn_main_achieve",
	[1008] = "btn_main_friend",
	[1009] = "btn_main_group",
	[1010] = "btn_main_guild",
	[1011] = "btn_main_mail",
	[1012] = "btn_main_consign",
	[1013] = "btn_main_rank",
	[1014] = "btn_main_puzzle",

	[2001] = "extend_firstPay",
	[2002] = "extend_awardHall",
	[2003] = "extend_openServer",
	[2004] = "extend_lottory",
	[2005] = "extend_store",
	[2006] = "btn_main_boss",
	[2007] = "extend_offline",
	[2008] = "extend_kingWar",
	[2009] = "extend_world",
	[2010] = "extend_breakup",
	[2011] = "extend_exploit",
	[2012] = "extend_mars",
	[2013] = "extend_makeExp",
	[2014] = "extend_activities",
	[2015] = "extend_heFu",
	[2016] = "extend_zhuanPan",
	[2017] = "extend_dailyPay",
	[2018] = "extend_dice",
	[2019] = "extend_invest",
	[2020] = "extend_events",
	[2021] = "extend_download",
	[2022] = "extend_task",
	[2023] = "extend_superVip",
}


local extendButtons = {
	["extend_firstPay"] = {
		checkFunc = function (player)
			return gui.PanelFirstRecharge.checkRedPointRecharge(player);
		end
	},
	["extend_openServer"] = {
		checkFunc = function (player) -- 验证开服活动中，正在开启的活动数量大于0
			local result = gui.PanelActivities.getNeedTabTable(player)
			if #result>0 then return true end
			return false
		end
	},
	["extend_activities"] = {
		checkFunc = function (player) -- 验证精彩活动中，正在开启的活动
			if gui.PanelCelebrationAct.checkActIng() then
				return true
			end
			return false
		end
	},
	["extend_makeExp"] = {
		checkFunc = function (player) -- 开服第二天24点隐藏
			if util.ppn(player, const.PP_EXP_DAILY_TIMES)  <4 then
				return true
			else
				return false
			end
		end
	},
	["extend_heFu"] = {
		checkFunc = function (player)
			if server_merge_day()>=0 and server_merge_day()<=4 then
				return true
			end
			return false
		end
	},
	["extend_zhuanPan"] = {
		checkFunc = function (player) -- 开服第二天24点隐藏
			if (server_merge_day()>=0 and server_merge_day()<=4) or gui.PanelCelebrationAct.checkActIng() then
				return true
			end
			return false
		end
	},
	["extend_dailyPay"] = {
		checkFunc = function (player)
			if gui.PanelFirstRecharge.checkRedPointRecharge(player)==false and server_start_day()>=0 and not gui.PanelDailyRecharge.getAwardsFinish(player) then
				return true
			end
			return false
		end
	},
	["extend_events"] = {
		checkFunc = function (player)
			return gui.PanelJingCaiAct.checkJingCaiActOpen(player);
		end
	},
	["extend_invest"] = {
		checkFunc = function (player)
			return gui.PanelInvest.isIconShow(player);
		end
	},
	["extend_download"] = {
		checkFunc = function (player)
			local day = role_create_day(player);
			return day>=1 and util.ppn(player,const.PP_DOWNLOAD_AWARD)<2
		end
	},
	["extend_task"] = {
		checkFunc = function (player)
			return true;
		end
	},
	["extend_superVip"] = {
		checkFunc = function (player)
			return true;
		end
	},
	-- ["extend_dice"] = {
	-- 	checkFunc = function (player)
	-- 		return gui.PanelInvest.isIconShow(player);
	-- 	end
	-- }
	-- ["extend_exploit"] = {
	-- 	checkFunc = function ()
	-- 		return false;
	-- 	end
	-- },
	-- ["btn_main_achieve"] = {
	-- 	checkFunc = function ()
	-- 		return false;
	-- 	end
	-- }
}

local openFuncs = tab.openFuncCsv

-- funcid和序号的转换表
local id2Index = {}

-- 给表增加funcs字段
local funcenmu
for i,v in ipairs(openFuncs) do
	v.funcenmu = math.floor(v.funcid / 10) -- 增加功能枚举，对应ui按钮
	if enmu2ui[v.funcenmu] then
		v.funcs = enmu2ui[v.funcenmu]
		id2Index[v.funcid] = i
	end
end

function checkCondition(player, conf, mLevel, mZslevel, tid, serverStartDay)
	if conf then
		local show = true;
		if conf.level and mLevel < conf.level  then
			show = false;
		end
		if show and conf.zslevel and mZslevel < conf.zslevel then
			show = false;
		end
		if show and conf.taskid and conf.taskid > 0 and tid < conf.taskid then
			show = false;
		end
		-- print("checkCondition", conf.funcid, show, conf.openSvr, serverStartDay)
		if show and conf.openSvr > 0 and serverStartDay + 1 > conf.openSvr  then
			show = false;
		end

		--特殊验证功能开启
		if show and extendButtons[conf.funcs] and extendButtons[conf.funcs].checkFunc then
			if extendButtons[conf.funcs].checkFunc(player) then
				show = true;
			else
				show = false;
			end
		end
		return show;
	end
	return false;
end

-- 功能是否开启
function checkFuncOpened(player, funcid)
	if not funcid then return false; end
	local index = id2Index[funcid];
	if index and openFuncs[index] then
		local mLevel = player:get_level();
		local mZslevel = util.ppn(player,const.PP_ZHSH_LEVEL);
		local tid = task.util.get_task_state(player, const.TASK_ID_MAIN);
		local serverStartDay = server_start_day();
		return checkCondition(player, openFuncs[index], mLevel, mZslevel, tid, serverStartDay);
	end
end




-- 开启功能
function checkOpenFunc(player, mLevel, mZslevel, tid)
	local result = {
		allFuncs = {},
		extend = {},
		-- basic = {},
	};
	local show;
	mLevel = mLevel or player:get_level();
	mZslevel = mLevel or util.ppn(player,const.PP_ZHSH_LEVEL);
	tid = tid or task.util.get_task_state(player, const.TASK_ID_MAIN);

	local serverStartDay = server_start_day();
	-- local tid, ts = task.util.get_task_state(player, const.TASK_ID_MAIN);
	if tid == 0 then tid = 10094 end
	for k,v in pairs(openFuncs) do
		show = checkCondition(player, v, mLevel, mZslevel, tid, serverStartDay);
		if show then
			-- if not table.indexof(result.allFuncs, v.funcs) then
			-- 	table.insert(result.allFuncs, v.funcs);
			-- end
			if v.funcenmu > 2000 then
				if not table.indexof(result.extend, v.funcs) then
					table.insert(result.extend, v.funcs);
				end
			else

			end
		end
		table.insert(result.allFuncs, {funcid = v.funcid, opened = show, level = v.level, funcname = v.funcname, funcKey = v.funcs})
		-- if not result.basic[v.funcs] then
		-- 	result.basic[v.funcs] = {opened = show, level = v.level, funcname = v.funcname}
		-- elseif result.basic[v.funcs].level > v.level then
		-- 	result.basic[v.funcs] = {opened = show, level = v.level, funcname = v.funcname}
		-- end
	end
	player:set_param(const.PP_OPEN_FUN_RECORD,util.encode(openRecord))
	-- print("checkOpenFunc", util.encode(result.extend))
	-- print("checkOpenFunc", util.encode(result.allFuncs))
	player:push_lua_table("allGuiButtons", util.encode(result));
	gui.moduleRedPoint.reqRedPointInfo(player);
	-- player:push_lua_table("allFuncOpened", util.encode(result));
	gui.moduleRedPoint.checkDailyRecharge(player)
end

function checkFuncPreview(player, level)
	-- print("checkFuncPreview", level)
	local result = {
		visible = false
	};
	for k,v in pairs(openFuncs) do
		if v.push then
			if level >= v.pushlevel and level < v.level then
				result = {
					visible = true,
					level = v.level - level,
					pushicon = v.pushicon,
				};
				break
			end
		end
	end
	-- print("1111111111111", util.encode(result))
	player:push_lua_table("funcPreview", util.encode(result))
end

function onLevelUp(player, level)
	-- pushAllGuiButtons(player);
	checkOpenFunc(player, level);
	checkFuncPreview(player, level);
end
cklevel.add_listener(onLevelUp);

function onZslevelUp(player, zslevel)
	checkOpenFunc(player, nil, zslevel);
end
ckzslevel.add_listener(onZslevelUp);

function onTaskChange(player, tid)
	checkOpenFunc(player, nil, nil, tid);
end
cktask.add_listener(const.TASK_ID_MAIN, onTaskChange)

function handlePanelData(player,commond)
	local data = util.decode(commond)
	if data.fly and data.fly == 1 then
		posnpc.fly(player,tonumber(data.actionid))
		return
	end

	if data.actionid then
		openClientPanel(player,data.actionid)
	end
end

ckpanel.add_listener("gui.moduleGuiButton.handlePanelData",gui.moduleGuiButton.handlePanelData)

-----聊天链接打开面板
local openTable = {
	["1"]={name="特惠礼包",openName="extend_openServer",desp="您已购买所有特惠礼包!",tab=1,index=1,needOpen=function(player) return gui.AwardHall_tehui.canBuy(player,1) end},
	["2"]={name="特惠礼包",openName="extend_openServer",desp="您已购买所有特惠礼包!",tab=1,index=2,needOpen=function(player) return gui.AwardHall_tehui.canBuy(player,2) end},
	["3"]={name="特惠礼包",openName="extend_openServer",desp="您已购买所有特惠礼包!",tab=1,index=3,needOpen=function(player) return gui.AwardHall_tehui.canBuy(player,3) end},
	["4"]={name="特惠礼包",openName="extend_openServer",desp="您已购买所有特惠礼包!",tab=1,index=4,needOpen=function(player) return gui.AwardHall_tehui.canBuy(player,4) end},
	["5"]={name="首充",    openName="extend_firstPay",  desp="您已领取过首充奖励!",  tab=1,index=0,needOpen=function(player) return gui.PanelFirstRecharge.checkRedPointRecharge(player) end},
}


local gotonpc = {
	["a"]={name="boss之家",npcId=4002012},
	["b"]={name="玛雅神殿",npcId=4002024},
	["c"]={name="转生神殿",npcId=4002020},
}


function openClientPanel(player,index)
	if openTable[index] then
		local itemData = openTable[index]
		local result = itemData.needOpen(player)
		if result then
			player:push_lua_table("open",util.encode({name = itemData.openName,extend={tab=itemData.tab,index=itemData.index}}))
		else
			player:alert(1,1,itemData.desp)
		end
	end
	if gotonpc[index] then
		posnpc.fly(player,gotonpc[index].npcId)
	end
end

----------------卡装备任务提示栏------------------------

local equipGetButtons = {
	[1] = {name = "btnAnDian", 		title = "未 知 暗 殿"},
	[2] = {name = "btnBoss", 		title = "激 战  BOSS"},
	[3] = {name = "btnShouChong", 	title = "首充礼包[推荐]", checkFunc = function (player)
		return gui.PanelFirstRecharge.checkRedPointRecharge(player)
	end},
	[4] = {name = "btnShenZhuang", 	title = "神装礼包[推荐]", checkFunc = function (player)
		return gui.AwardHall_tehui.checkShenZhuang(player)
	end},
	[5] = {name = "btnHeCheng", 	title = "装 备 合 成"},
}

function reqEquipGetButtons(player,commond)
	-- print("reqEquipGetButtons")
	local buttons = {}
	for i,v in ipairs(equipGetButtons) do
		if not v.checkFunc then
			table.insert(buttons, {name = v.name, title = v.title})
		elseif v.checkFunc and v.checkFunc(player) then
			table.insert(buttons, {name = v.name, title = v.title})
		end
	end
	player:push_lua_table("equipGetButtons", util.encode({buttons = buttons}))
end

ckpanel.add_listener("gui.moduleGuiButton.reqEquipGetButtons",gui.moduleGuiButton.reqEquipGetButtons)
