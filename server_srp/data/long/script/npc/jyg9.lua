module(..., package.seeall)

local npcInfo =	{
	strs1={
		"<font color=#b2a58b>少年，老夫已经在这里恭候多时了</font>",
		"<font color=#b2a58b>恭喜你达到精英阁第九层</font>",
	},
	strs2={
		"<font color=#b2a58b>领取奖励后自动进入BOSS房间</font>",
		"<font color=#b2a58b>击杀魔王有丰厚奖励等着你</font>",
	},
	resData={talkTitle="精英阁",btnName = "领取奖励",},
	func = "npc.jyg9.btnget",
}

local npcInfo2 =	{
	strs1={
		"<font color=#b2a58b>在精英阁第十层住着一个乱世“魔王”</font>",
		"<font color=#b2a58b>击杀“魔王”可以获得魔王的全部宝藏</font>",
	},
	resData={talkTitle="精英阁",btnName = "进入下一层",},
	func = "npc.jyg9.btnEnter",
}

local pos = {{x = 20,	y = 25,	}, {x = 15,y = 33,	}, {x = 21,	y = 39,	}, {x = 26,y = 33,	}, }

local ACTIVITY_NAME = '精英阁';
local awards = {
	{ chart = "1-1",award ={
			{name="护盾碎片（小）", 	id = 23010001, num = 600, bind = 1},
			-- {name="玉佩碎片（小）", id = 23000001, num = 10},
		},
		exp = 0, 	coin = 2000000,
	},
	{ chart = "2-2",award ={
			{name="护盾碎片（小）", 	id = 23010001, num =500, bind = 1},
			-- {name="玉佩碎片（小）", id = 23000001, num = 8},
		},
		exp = 0, 	coin = 1500000,
	},
	{ chart = "3-5",award ={
			{name="护盾碎片（小）", 	id = 23010001, num =400, bind = 1},
			-- {name="玉佩碎片（小）", id = 23000001, num = 6},
		},
		exp = 0, 	coin = 1000000,
	},
	{ chart = "6-10",award ={
			{name="护盾碎片（小）", 	id = 23010001, num =300, bind = 1},
			-- {name="玉佩碎片（小）", id = 23000001, num = 4},
		},
		exp = 0, 	coin = 800000,
	},
	{ chart = "11-99999",award ={
			{name="护盾碎片（小）", 	id = 23010001, num =200, bind = 1},
			-- {name="玉佩碎片（小）", id = 23000001, num = 2},
		},
		exp = 0, 	coin = 500000,
	},
}

function onTalk100(npc,player)
	local lastJoinTime = player:get_param(const.PP_JYG_JOIN_TIME)
	if lastJoinTime ~= server.get_var("jygStartTime") then
		player:set_param(const.PP_JYG_JOIN_TIME,server.get_var("jygStartTime"))
		player:set_param(const.PP_JYG_AWARD,0)
	end
	if util.ppn(player, const.PP_JYG_AWARD) >0 then
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = npcInfo2}));
	else
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = npcInfo}));
	end
end

function getAwardIndex(player)
	local chart,index = 99,5
	for i=1,10 do
		local var = server.get_var("jyg"..i)
		if var and var==player:get_name() then
			chart = i
			break;
		end
	end
	for i,v in ipairs(awards) do
		local t = util.splitString(v.chart, "-")
		if #t==2 then
			if tonumber(t[1])<=chart and chart<=tonumber(t[2]) then
				index = i;break;
			end
		end
	end
	return index
end

function btnget(player)
	if server.get_temp_var("jygopen") ~="open" then
	-- if not gui.PanelActivityGuide.isActivityOpen(ACTIVITY_NAME) then
		return util.alert_msg(4, "活动未开始！", player)
	end

	if player:get_level()>=gui.PanelActivityGuide.getActivityLevelLimit(ACTIVITY_NAME) then
		local index = getAwardIndex(player)
		if index then
			player:set_param(const.PP_JYG_AWARD,1)

			util.getAward(player,awards[index].award)
			if checknumber(awards[index].exp)>0 then
				player:add_exp(awards[index].exp)
			end
			if checknumber(awards[index].coin)>0 then
				player:add_gamemoney(awards[index].coin)
			end
			util.alert_msg(4, "成功领取精英阁奖励！", player)
		end
		local rand = math.random(1,4)
		player:enter_map("jingying10",pos[rand].x,pos[rand].y)
	else
		player:alert(11,1,"等级不足");
	end
end
ckpanel.add_listener("npc.jyg9.btnget", npc.jyg9.btnget);

function btnEnter(player)
	if server.get_temp_var("jygopen") ~="open" then
	-- if not gui.PanelActivityGuide.isActivityOpen(ACTIVITY_NAME) then
		return util.alert_msg(4, "活动未开始！", player)
	end

	if player:get_level()>=gui.PanelActivityGuide.getActivityLevelLimit(ACTIVITY_NAME) then
		local rand = math.random(1,4)
		player:enter_map("jingying10",pos[rand].x,pos[rand].y)
	else
		player:alert(1,1,"等级不足!");
	end
end
ckpanel.add_listener("npc.jyg9.btnEnter", npc.jyg9.btnEnter);