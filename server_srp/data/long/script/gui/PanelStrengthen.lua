module(..., package.seeall)
--查看--打开面板
--前往--寻路到npc或地图内
local data = {
	[1]={
		{name = "烧猪洞",	star = 5,	operate = "前往",	dest = 2020021},
		{name = "寻宝系统",	star = 5,	operate = "查看",	dest = "extend_lottory"},
		{name = "降妖除魔",	star = 4,	operate = "前往",	dest = 2020015},
		{name = "装备回收",	star = 4,	operate = "前往",	dest = 2020014},
		{name = "膜拜城主",	star = 4,	operate = "前往",	dest = 2020022},
		{name = "安全挂机",	star = 3,	operate = "前往",	dest = 2020013},
		{name = "闯天关",	star = 3,	operate = "查看",	dest = "extend_breakup"},
	},
	[2]={
		{name = "激战BOSS",	star = 5,	operate = "查看",	dest = "btn_main_boss"},
		{name = "寻宝系统",	star = 5,	operate = "查看",	dest = "extend_lottory"},
		{name = "BOSS之家",	star = 4,	operate = "前往",	dest = 2020012},
		{name = "玛雅神殿",	star = 4,	operate = "前往",	dest = 2020024},
		{name = "转生神殿",	star = 4,	operate = "前往",	dest = 2020020},
		{name = "未知暗殿",	star = 3,	operate = "前往",	dest = 2020019},
		{name = "神威魔域",	star = 3,	operate = "前往",	dest = 2020006},
		{name = "装备合成",	star = 3,	operate = "查看",	dest = 2020017},
	},
	[3]={
		{name = "锻造系统",	star = 5,	operate = "查看",	dest = "main_forge"},
		{name = "神炉系统",	star = 5,	operate = "查看",	dest = "main_furnace"},
		{name = "转生系统",	star = 5,	operate = "查看",	dest = "main_avatar",	tab = 4},
		{name = "官位系统",	star = 4,	operate = "查看",	dest = "main_official"},
		{name = "技能系统",	star = 4,	operate = "查看",	dest = "main_skill"},
		{name = "翅膀培养",	star = 4,	operate = "查看",	dest = "btn_main_wing"},
		{name = "成就勋章",	star = 4,	operate = "查看",	dest = "main_achieve"},
	},
	[4]={
		{name = "副本挑战",	star = 5,	operate = "前往",	dest = 2020011},
		{name = "矿洞挖矿",	star = 5,	operate = "前往",	dest = 2020005},
		{name = "激战BOSS",	star = 4,	operate = "查看",	dest = "btn_main_boss"},
		{name = "功勋系统",	star = 4,	operate = "查看",	dest = "extend_exploit"},
		{name = "闯天关",	star = 4,	operate = "查看",	dest = "extend_breakup"},
		{name = "离线挂机",	star = 3,	operate = "查看",	dest = "extend_offline"},
		{name = "尸王殿",	star = 3,	operate = "前往",	dest = 2030003},
		{name = "夜袭比奇",	star = 3,	operate = "前往",	dest = 2030002},
	}
}

function onPanelData(player, commondData)
	local data = util.decode(commondData);
	if data.actionid == "fresh" then
		freshPanel(player,data.tab)
	elseif data.actionid == "operate" then
		onPressOperate(player,data.tab,data.index)
	end
end
ckpanel.add_listener("gui.PanelStrengthen.onPanelData",gui.PanelStrengthen.onPanelData)

function freshPanel(player,tabIdx)
   	if data[tabIdx] then
	   	local result = {}
	   	result.cmd = "fresh"
	   	result.panelData = data[tabIdx]
	   	if tabIdx ==2 then
	   		result.item = {11116011,11116021,11116031,11116041}
	   	end
	   	player:push_lua_table("PanelStrengthen",util.encode(result));
	end
end

function onPressOperate(player,tabIdx,index)
	if data[tabIdx] and data[tabIdx][index] then
		local t = data[tabIdx][index]
		if type(t.dest) =="string" then
   			player:push_lua_table("open",util.encode({
   				name = t.dest,tab = t.tab
   			}));
		elseif type(t.dest) =="number" then
			posmap.go(player,t.dest)
			player:push_lua_table("showFly",util.encode({flyInfo = t.dest}));
		end
	end
end