module(..., package.seeall)

------------func 既是控件名称，又是资源名称------------nextFunc = {func = "menu_mail",			mType = "menu", 	nextLabel = "邮件即将开启"},},

local func_task = {
	[79] = {func = "extend_lianzhi",	mType = "rtpart",	index = 1,	nextFunc = {func = "menu_wing", 		mType = "menu", 	nextLabel = "翅膀即将开启"},},
	[82] = {func = "extend_activity", mType = "rtpart",	index = 2,	nextFunc = {func = "menu_mount", 		mType = "menu", 	nextLabel = "坐骑即将开启"},},
	[74] = {func = "extend_firstgift",mType = "rtpart",	index = 3,	nextFunc = {func = "extend_lianzhi", 	mType = "rtpart", 	nextLabel = "经验炼制即将开启"},},
}

local func_level = {
	{level = 1,  name="仓库",	  func = "menu_depot",	    mType = "menu"},--仓库
	{level = 1,  name="技能",	  func = "menu_skill",	    mType = "menu"},--技能
	{level = 1,	 name="组队",	  func = "menu_team",    	mType = "menu", },--组队
	{level = 1,	 name="成就",	  func = "main_achieve", 	mType = "menu", },--成就
	{level = 1,	 name="功勋",	  func = "menu_gongxun",    mType = "menu", },--功勋
	{level = 1,	 name="锻造",	  func = "menu_duanzao",    mType = "menu", },--锻造
	{level = 1,	 name="护卫",	  func = "menu_huwei",      mType = "menu", },--护卫
	{level = 1,	 name="邮件",	  func = "menu_mail",    	mType = "menu", },--邮件
	{level = 70, name="排行",     func = "menu_chart",		mType = "menu",},--排行
	{level = 1,  name="神将",	  func = "menu_shenjiang",	mType = "menu", anima=1},--神将
	{level = 1,  name="熔炉",	  func = "menu_furnace",	mType = "menu", },--熔炉
	{level = 25, name="好友",	  func = "menu_friend",  	mType = "menu", },--好友	
	{level = 1,  name="回收",	  func = "menu_recycle",  	mType = "menu", },--回收
	{level = 1,  name="帮会",	  func = "menu_guild",   	mType = "menu", },--帮会
	{level = 65, name="翅膀",     func = "menu_wing",		mType = "menu",   anima=1},--翅膀
	{level = 1,  name="官印",     func = "menu_official",		mType = "menu",  },--官印
}
----依据开服天数开启功能模块
local func_opensvr = {
	[2] = {func = "menu_wing",	mType = "menu",	index = 1},--"翅膀",     
	[3] = {func = "menu_mount",	mType = "menu", index = 2}, --"坐骑",     
}

function checkFuncOpened(player, func)
	for i,v in ipairs(func_level) do
		if v.func == func then
			if util.ppn(player, const.PP_FUNCINDEX_BY_LEVEL) >= i then return true end
		end
	end
end


------------------主线引导------------------

local guide_task = {
--	[15] = {lv = 10}, --第一次装备引导
--	[4] = {lv = {7, 8}, checkfunc = function (player) return player:num_item("双倍宝典", 1) > 0 end}, --双倍宝典
--	[18] = {lv = 12}, --第二次装备引导
--	[18] = {lv = 9}, --设置
--	[20] = {lv = 11}, --寻宝
--	[29] = {lv = 13}, --锻造
	-- [14] = {lv = 30}, -- 熔炉（玉佩）
	-- [44] = {lv = 17}, --降妖除魔
	-- [27] = {lv = 31}, --强化+3
	-- [35] = {lv = 13}, --强化+5	

	[37] = {lv = 37, checkfunc = function (player) return util.ppn(player,const.PP_NEW_SIGN_DAY) ~= today() end}, --引导签到

	[40] = {lv = 25, checkfunc = function (player) return util.ppn(player, const.PP_BOSS_FIGHT_SIMPLE2) == 0 end}, --猪洞

	[42] = {lv = 33, checkfunc = function (player) return player:get_level() < 47 end}, --日常宝典（降妖除魔）

	-- [43] = {lv = 15, checkfunc = function (player) return util.ppn(player, const.PP_MEDAL_LEVEL) == 0 and player:get_honor() >= 1000 end}, --勋章引导

	[56] = {lv = 26, checkfunc = function (player) return util.ppn(player, const.PP_BOSS_FIGHT_SIMPLE3) == 0 end}, -- 猛犸

	[57] = {lv = 33, checkfunc = function (player) return player:get_level() < 53 end}, --日常宝典（降妖除魔）

	-- [65] = {lv = 40, checkfunc = function (player) return player:num_item("镖车令", 1) >= 4 and util.ppn(player,const.PP_ESCORT_REMAIN_TIMES)==0 end}, --押镖引导

	-- [71] = {lv = 33, checkfunc = function (player) return player:get_level() < 60 end}, --日常宝典（降妖除魔）

	-- [72] = {lv = 41, checkfunc = function (player) return util.ppn(player, const.PP_MAINTASK_LOTTERY1) == 0 end}, --寻宝

	[75] = {lv = 38, checkfunc = function (player) return player:get_level() < 65 end}, --经验副本

	-- [75] = {lv = 39, checkfunc = function (player) return player:get_level() < 65 end}, --镇魔塔
	-- [60] = {lv = 20}, --个人副本
	-- [61] = {lv = 32}, --强化转移（跳过）
	-- [81] = {lv = 8, checkfunc = function (player) return player:num_item("双倍宝典", 1) > 0 end},
	-- [60] = {lv = 20}, --个人副本

	-- [66] = {lv = 17} --镇魔塔

	[89] = {lv = 46, checkfunc = function (player) return player:get_level() < 75 end}, --主线断开后引导
}

function showGuideByTask(player, tid)
	-- print("showGuideByTask", tid)
	if guide_task[tid] then 
		local guide = true
		if guide_task[tid].checkfunc and not guide_task[tid].checkfunc(player) then
			guide = false
		end
		if guide then
			player:push_lua_table("guide",util.encode({lv = guide_task[tid].lv}))
		end
	end
end

--------------------等级挂钩的引导-----------------------------
local guild_level = {
	[17]={lv = 44 ,desp="17级强化引导"},
	[30]={lv = 44 ,desp="30级强化引导"},

}

function showGuideByLevel(player,level)
	if guild_level[level] then
		local guide = true
		if guild_level[level].checkfunc and not guild_level[level].checkfunc(player) then
			guide = false
		end
		if guide then
			player:push_lua_table("guide",util.encode({lv = guild_level[level].lv}));
		end
	end
end


--------------引导相关--------------

--除魔引导特殊处理（依据除魔的任务状态开启指定应到）

local panelAndGuide = {
	[1] = {pName = "menu_recycle", lv = 3},--回收
	[2] = {pName = "panel_chumo", lv = 4},--降妖除魔
	[3] = {pName = "panel_chumo", lv = 5},--降妖除魔
	[4] = {pName = "extend_mars", lv = 6},--护卫
	[5] = {pName = "menu_bag", lv = 7},--背包买药
	[6] = {pName = "panel_quickset", lv = 8},--快捷设置
	[7] = {pName = "main_forge", lv = 9},--强化
	[8] = {pName = "btn_main_boss", lv = 10},--个人boss
	[9] = {pName = "btn_main_boss", lv = 11},--个人boss
	[10]= {pName = "btn_main_wing", lv = 12},--翅膀

	[11] = {pName = "main_avatar", lv = 15},--内功

	[12] = {pName = "main_furnace", lv = 16},--护盾
	[13] = {pName = "main_furnace", lv = 19},--玉佩
	[14] = {pName = "main_furnace", lv = 21},--龙心
	[15] = {pName = "main_furnace", lv = 22},--狼牙

	[16] = {pName = "main_official", lv = 23},--官位
	[17] = {pName = "panel_cailiao" , lv = 24},--引导羽毛副本
};

local guideConfig = {}; -- 引导配置表，以面板key做key
local taskOpenPanel = {}; -- 指定任务打开面板表

local index, conf
for i,v in ipairs(tab.guideCsv) do
	index = v.guideIndex;
	conf = panelAndGuide[index];
	if conf then
		guideConfig[conf.pName] = guideConfig[conf.pName] or {};
		table.insert(guideConfig[conf.pName], {
			pName = conf.pName,
			tid = v.tid,
			ts = v.ts,
			once = v.once,
			guidelv = conf.lv,
			followLv = v.followLv,
			showicon = v.showicon,
			funName = v.funName
		});
		if conf.pName ~= "panel_chumo" then
			v.pName = conf.pName
			taskOpenPanel[v.tid] = v;
			-- taskShowIcon[v.tid] = conf.showicon;
		end
	end
end

-- for k,v in pairs(taskOpenPanel) do
-- 	print(k,v)
-- end

-- print(util.encode(taskOpenPanel));

-- print(util.encode(guideConfig));

local function checkMainTask(player)
	local tid, ts = task.util.get_task_state(player, const.TASK_ID_MAIN);
	return tid < 10022;
end

local function checkMainTaskOpen(player)
	if checkMainTask(player) then
		player:push_lua_table("guide",util.encode({lv = 2}));
	end
end

local function checkMainTaskClose(player)
	if checkMainTask(player) then
		player:push_lua_table("guide",util.encode({lv = 1}));
	end
end

local function getGuideConf(player, pName)
	if not pName then return end
	local conf = guideConfig[pName];
	-- print("////", util.encode(conf));
	if not conf then return end
	local tid, ts = task.util.get_task_state(player, const.TASK_ID_MAIN);
	-- print("////", tid, ts);
	local str = player:get_param(const.PP_GUIDE_ONCE);
	-- print("//////////////canGuide//////////////", str);
	local guides = {};
	if str ~= "" then
		guides = util.decode(str);
	end
	for i,v in ipairs(conf) do
		if (v.tid == tid and v.ts == ts) or (v.followLv and not table.indexof(guides,v.tid)) then
			return v;
		end
	end
end

local function canGuide(player, conf)
	local str = player:get_param(const.PP_GUIDE_ONCE);
	-- print("//////////////canGuide//////////////", str);
	local guides = {};
	if str ~= "" then
		guides = util.decode(str);
	end
	-- print("//////////////conf.once//////////////", conf.once, util.encode(guides));
	if conf.once == 1 and not table.indexof(guides, conf.tid) then
		table.insert(guides, conf.tid)
		player:set_param(const.PP_GUIDE_ONCE, util.encode(guides));
		return true
	elseif conf.once == 0 then
		return true
	end
	return false
end

local function onPanelOpen(player, name)
	
	local conf = getGuideConf(player, name);
	if conf then
		-- print("1111111", conf.guidelv);
		if conf.pName == "panel_chumo" then
			local tid, ts = task.util.get_task_state(player, const.TASK_ID_RICHANG);
			if ts == const.TSACCE then
				player:push_lua_table("guide",util.encode({lv = 4}));
			elseif ts == const.TSCOMP then
				player:push_lua_table("guide",util.encode({lv = 5}));
			end
		else
			if canGuide(player, conf) then
				--护卫引导特殊处理
				if conf.pName == "extend_mars" and player:have_slave(10) > 0 then
					return;
				end
				player:push_lua_table("guide",util.encode({lv = conf.guidelv}));
			end
		end
	end
end

local function onPanelClose(player, name)
	local conf = getGuideConf(player, name);
	if conf then
		local guideLevel = conf.guidelv;
		-- print("1111111", guideLevel);
		if conf.pName == "panel_chumo" then
			local tid, ts = task.util.get_task_state(player, const.TASK_ID_RICHANG);
			if ts == const.TSACCE or ts == const.TSACED then
				player:push_lua_table("endGuide",util.encode({lv = 4}));
				player:push_lua_table("endGuide",util.encode({lv = 5}));
				-- player:push_lua_table("continueTask", "");
			elseif ts == const.TSCOMP then
				player:push_lua_table("endGuide",util.encode({lv = 5}));
				-- player:push_lua_table("continueTask", "");
			end
		-- elseif conf.pName == "panel_mainTask" then
		-- 	player:push_lua_table("endGuide",util.encode({lv = 1}));
		else
			player:push_lua_table("endGuide",util.encode({lv = guideLevel}));
			if name == "menu_recycle" then
				player:push_lua_table("endGuide",util.encode({lv = 31}));
			end
			if conf.tid == 10052 then return end
			if conf.tid == 10080 or conf.tid == 10083 then return end
			if conf.tid == 10055 then return end
			player:push_lua_table("continueTask", "");
		end
	end
end

-- 特殊引导
function checkGuideSpecial(player, name)
	local conf = getGuideConf(player, name);
	if conf then
		local guideLevel = conf.guidelv;
		-- print("1111111", guideLevel);
		if name == "menu_recycle" then
			player:push_lua_table("guide",util.encode({lv = 31}));
		end
	end
end

function checkGuide(player, commond)
	local data = util.decode(commond)
	if data == nil then return end
	if data.actionid == "mainTaskOpen" then
		checkMainTaskOpen(player);
	elseif data.actionid == "mainTaskClose" then
		if data.param then
			checkMainTaskClose(player);
		else
			player:push_lua_table("endGuide",util.encode({lv = 2}));
		end
	elseif data.actionid == "onPanelOpen" then
		onPanelOpen(player, data.pName);
	elseif data.actionid == "onPanelClose" then
		onPanelClose(player, data.pName);
	end
end

-- function checkOpenPanel(player, tid)
-- 	-- print("checkOpenPanel", tid);
-- 	local open = false;
-- 	if tid and (tid == 10035 or tid == 10047 or tid == 10056) then
-- 		player:push_lua_table("open",util.encode({name = "menu_recycle"}));open=true;
-- 	elseif tid == 10052 then
-- 		player:push_lua_table("open",util.encode({name = "menu_bag"}));open=true;
-- 	elseif tid == 10053 then
-- 		player:push_lua_table("open",util.encode({name = "panel_quickset", extend={type=2}}));open=true;
-- 	elseif tid == 10061 or tid == 10073 or tid == 10076 then
-- 		player:push_lua_table("open",util.encode({name = "main_forge", extend={type=2}}));open=true;
-- 	end
-- 	return open;
-- end

function checkOpenPanel(player, tid)
	local open = false;
	local conf = taskOpenPanel[tid]
	if not conf then return open end
	local pName = conf.pName
	if not pName then return open end
	local extend;
	if pName == "panel_quickset" then
		extend = {type=2};		
	end
	-- print("checkOpenPanel", util.encode(taskOpenPanel), tid, pName);
	if pName == "extend_mars" and player:have_slave(10) > 0 then
		return open;
	end
	if conf.showicon == "null" or conf.showicon == nil then
		player:push_lua_table("open",util.encode({name = pName, extend = extend}));open=true;
	else
		player:push_lua_table("showFun",util.encode({name = pName, showicon = conf.showicon,funName = conf.funName}));open=true;	
	end
	
	return open;
end

ckpanel.add_listener("gui.moduleGuide.checkGuide",gui.moduleGuide.checkGuide);

-- if player:get_param(const.PP_OPEN_FUN_RECORD) == nil or player:get_param(const.PP_OPEN_FUN_RECORD) == "" or player:get_param(const.PP_OPEN_FUN_RECORD) == "0" then
-- 	openRecord = {}
-- else
-- 	openRecord = util.decode(player:get_param(const.PP_OPEN_FUN_RECORD))
-- end

function getTidByIndex( index )
	if tab.guideCsv[index] then
		return tab.guideCsv[index].tid
	end
	return 0
end
-- function onContinueTask(player, commond)
-- 	local data = util.decode(commond)
-- 	print("onContinueTask", commond)
-- 	if data.taskType == "getEquip" then
-- 		local tid, ts = task.util.get_task_state(player, const.TASK_ID_MAIN);
-- 		print(tid)
-- 		if tid >= 10083 then posnpc.go(player, 4002019) end
-- 	end
-- end

-- ckpanel.add_listener("gui.moduleGuide.onContinueTask",gui.moduleGuide.onContinueTask);

