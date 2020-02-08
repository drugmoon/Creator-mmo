module(..., package.seeall)

local countTable = {
	["每日泡点"] = 	{var = "mrpd",	time = {{0800, 1100},	{1500, 1800}},},
	["神威狱"]   = 	{var = "swy", 	time = {{1000, 1015},	{1930, 1945}},},
	["玛雅神殿"] = 	{var = "mysd",	time = {{1030, 1100},	{1410, 1440},	{1730, 1800}}},
	["夺宝奇兵"] = 	{var = "dbqb",	time = {{1130, 1150},}},
	["武林争霸"] = 	{var = "wlzb",	time = {{1230, 1245},}},
	["擂台挑战"] = 	{var = "lttz",	time = {{1300, 1400},	{2200, 2300}}},
	-- ["元宝嘉年华"] ={var = "ybjnh",	time = {{1300, 1400},}},
	-- ["双倍经验"] = 	{var = "sbjy",	time = {{1600, 1700},	{2300, 2400}}},
	["皇城战"]   = 	{var = "wcz",	time = {{2000, 2100},}},
	["宝物争夺"] = 	{var = "bwzd",	time = {{2050, 2105},}},
	["蚩尤战场"] = 	{var = "cyzc",	time = {{2120, 2135},}}, --1
	["傲视沙城兑换"] = {var = "assc",},
	["未知暗殿进入"] = {var = "wzad"},
	["降妖除魔"] = {var = "xycm"},
	["经验副本"] = {var = "jyfb"},
	["翅膀副本"] = {var = "cbfb"},
	["坐骑副本"] = {var = "zqfb"},
	["组队副本"] = {var = "zdfb"},
	["帮会副本"] = {var = "fbgd"},
	["镇魔塔"]	 = {var = "zmt"},
	["vip副本"]	 = {var = "vipfb"},
	["个人boss副本"] ={var = "grboss"},

	["回收"]= {var = "recycle",	},		--"回收"
	["锻造"]= {var = "forge",	},		--"锻造"
	["称号"]= {var = "title",	},		--"称号"
	["熔炉"]= {var = "furnace",	},		--"熔炉"
	["押镖"]= {var = "escort",	},		--"押镖"
	["元神"]= {var = "spirit",	},		--"元神"
	["坐骑"]= {var = "mount",	},		--"坐骑"
	["翅膀"]= {var = "wing",	},		--"翅膀"
	["帮会"]= {var = "guild",	},		--"帮会"
	["注灵"]= {var = "zhuling",	},		--"注灵"
	["转生"]= {var = "zhuansheng",},	--"转生"
	["神将"]= {var = "shenjiang",},		--"神将"
	["排行"]= {var = "chart",	},		--"排行"
	["组队"]= {var = "team",	},		--"组队"
	["好友"]= {var = "friend",	},		--"好友"
	["邮件"]= {var = "mail",	},		--"邮件"
	["商城"]= {var = "store",	},		--"商城"
	["设置"]= {var = "set",		},		--"设置"
	["角色"]= {var = "avatar",	},		--"角色"
	["装备"]= {var = "equip",	},		--"装备"
	["背包"]= {var = "bag",		},		--"背包"

	["免费坐骑"]= {var = "freemount",},	--"免费坐骑"
	["点击回城"]= {var = "home",},		--"点击回城"
	["点击聊天"]= {var = "chat",},		--"点击聊天"
	["点击交易"]= {var = "trade",},		--"点击交易"
}

function resetCount()
	if util.svn("countDay") ~= today() then
		exportCount()
		clearCount()
		server.set_var("countDay", today())
	end
end

function countTimes(key)
	local countData = countTable[key]
	if countData then
		local serverVar
		if countData.time then
			local clock = tonumber(os.date("%H%M",os.time()))
			for i,v in ipairs(countData.time) do
				if clock >= v[1] and clock <= v[2] then
					serverVar = countData.var..v[1]
					break
				end
			end
		else
			serverVar = countData.var
		end
		if serverVar then
			-- print("count"..serverVar)
			server.set_var("count"..serverVar, util.svn("count"..serverVar) + 1)
		end
	end
end

function exportCount()
	for k,v in pairs(countTable) do
		if v.time then
			for i,vv in ipairs(v.time) do
				server.log_act(k..vv[1], util.svn("count"..v.var..vv[1]))
			end
		else
			server.log_act(k, util.svn("count"..v.var))
		end
	end
end

----------------变量清理----------------
function clearCount()
	for k,v in pairs(countTable) do
		if v.time then
			for i,vv in ipairs(v.time) do
				server.set_var(v.var..vv[1], 0)
			end
		else
			server.set_var(v.var, 0)
		end
	end
end

function onClientData(player, commond)
	-- print("onClientData", commond)
	local data = util.decode(commond)
	if data.cmd and countTable[data.cmd] then
		countTimes(data.cmd)
	end
end

ckpanel.add_listener("count.onClientData",count.onClientData)