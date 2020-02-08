module(... ,package.seeall)

local bossTable = {
	[1]={con=const.PP_HALL_BOSS_AWARD1,--"经典BOSS"
		awards={
			[1]={id=0,       num=1,bind=0,effectRes = 0},
			[2]={id=21000004,num=2,bind=1,effectRes = 65079},
			[3]={id=32000001,num=2,bind=1,effectRes = 0},
			[4]={id=32000005,num=2,bind=1,effectRes = 0},

		},
		boss={
			[1]={id=3001001,headid=31016,name="尸王",    con=const.PP_HALL_BOSS1},
			[2]={id=3001002,headid=31021,name="娜迦",    con=const.PP_HALL_BOSS2},
			[3]={id=3001003,headid=31599,name="触龙邪神",con=const.PP_HALL_BOSS3},
			[4]={id=3001004,headid=31153,name="荒漠霸主",con=const.PP_HALL_BOSS4},
		}
	},
	[2]={con=const.PP_HALL_BOSS_AWARD2,--"暗殿BOSS"
		awards={
			[1]={id=0,       num=1,bind=0,effectRes = 0},
			[2]={id=21000004,num=3,bind=1,effectRes = 65079},
			[3]={id=32000001,num=2,bind=1,effectRes = 0},
			[4]={id=32000005,num=5,bind=1,effectRes = 0},
		},
		boss={
			[1]={id=6009019,headid=31508,name="暗之骷髅精灵",con=const.PP_HALL_BOSS5},
			[2]={id=6009022,headid=31153,name="暗之荒漠霸主",con=const.PP_HALL_BOSS6},
			[3]={id=6009028,headid=31084,name="暗之冰封魔王",con=const.PP_HALL_BOSS7},
			[4]={id=6009027,headid=31334,name="暗之双头蛇王",con=const.PP_HALL_BOSS8},
		}
	},
	[3]={con=const.PP_HALL_BOSS_AWARD3,--"禁地BOSS"
		awards={
			[1]={id=0,       num=1,bind=0,effectRes = 0},
			[2]={id=21000004,num=4,bind=1,effectRes = 65079},
			[3]={id=32000003,num=3,bind=1,effectRes = 0},
			[4]={id=32000006,num=2,bind=1,effectRes = 65078},
		},
		boss={
			[1]={id=3001006,headid=31096,name="沃玛教主",con=const.PP_HALL_BOSS9 },
			[2]={id=3001007,headid=31091,name="祖玛教主",con=const.PP_HALL_BOSS10},
			[3]={id=3001008,headid=31066,name="赤月老魔",con=const.PP_HALL_BOSS11},
			[4]={id=3001009,headid=31123,name="牛魔王",  con=const.PP_HALL_BOSS12},
		}
	},
	[4]={con=const.PP_HALL_BOSS_AWARD4,--"终极BOSS"
		awards={
			[1]={id=0,       num=1,bind=0,effectRes = 0},
			[2]={id=21000004,num=6,bind=1,effectRes = 65079},
			[3]={id=32000003,num=4,bind=1,effectRes = 0},
			[4]={id=32000006,num=5,bind=1,effectRes = 65078},
		},
		boss={
			[1]={id=3001012,headid=31083,name="冰宫祭祀",con=const.PP_HALL_BOSS13},
			[2]={id=3001014,headid=31077,name="幽灵教主",    con=const.PP_HALL_BOSS14},
			[3]={id=3001015,headid=31546,name="蛮荒巨兽",con=const.PP_HALL_BOSS15},
			[4]={id=3001016,headid=31421,name="火龙王",  con=const.PP_HALL_BOSS16},
		}
	},
}

--击杀表
local killName = {
	["尸王"]        ={con=const.PP_HALL_BOSS1},
	["娜迦"]        ={con=const.PP_HALL_BOSS2},
	["触龙邪神"]    ={con=const.PP_HALL_BOSS3},
	["荒漠霸主"]    ={con=const.PP_HALL_BOSS4},
	["暗之骷髅精灵"]={con=const.PP_HALL_BOSS5},
	["暗之荒漠霸主"]={con=const.PP_HALL_BOSS6},
	["暗之冰封魔王"]={con=const.PP_HALL_BOSS7},
	["暗之双头蛇王"]={con=const.PP_HALL_BOSS8},
	["沃玛教主"]    ={con=const.PP_HALL_BOSS9},
	["祖玛教主"]    ={con=const.PP_HALL_BOSS10},
	["赤月老魔"]    ={con=const.PP_HALL_BOSS11},
	["牛魔王"]      ={con=const.PP_HALL_BOSS12},
	["冰宫祭祀"]    ={con=const.PP_HALL_BOSS13},
	["幽灵教主"]    ={con=const.PP_HALL_BOSS14},
	["蛮荒巨兽"]    ={con=const.PP_HALL_BOSS15},
	["火龙王"]      ={con=const.PP_HALL_BOSS16},
}

--给装备的部分
local specialEquips = {
	[1]={
		["warrior"]={[200]={11106011,65080},[201]={11106011,65080}},--战士 男 女
		["wizard"] ={[200]={12106011,65080},[201]={12106011,65080}},--法士 男 女
		["taoist"] ={[200]={13106011,65080},[201]={13106011,65080}},--道士 男 女
	},
	[2]={
		["warrior"]={[200]={11107061,65080},[201]={11107061,65080}},
		["wizard"] ={[200]={12107061,65080},[201]={12107061,65080}},
		["taoist"] ={[200]={13107061,65080},[201]={13107061,65080}},
	},
	[3]={
		["warrior"]={[200]={11108061,65080},[201]={11108061,65080}},
		["wizard"] ={[200]={12108061,65080},[201]={12108061,65080}},
		["taoist"] ={[200]={13108061,65080},[201]={13108061,65080}},
	},
	[4]={
		["warrior"]={[200]={11110061,65080},[201]={11110061,65079}},
		["wizard"] ={[200]={12110061,65080},[201]={12110061,65079}},
		["taoist"] ={[200]={13110061,65080},[201]={13110061,65079}},
	},
}

function handlePanelData(player,commond)
	-- for i=0,15 do
	-- 	player:set_param(const.PP_HALL_BOSS1+i,1)
	-- end
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "reqBossData" then
		senderToClient(player)
	elseif data.actionid == "reqGetAward" then
		getBossAward(player,data.params.index)
	elseif data.actionid == "reqGo" then
		goToKill(player,data.params.index)
	end
end

--推送全名BOSS数据
function senderToClient(player)
	local result = {
		cmd="updateBossData",
		dataTable=setNeedAward(player),
	}
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--处理成客户端需要的信息表
function setNeedAward(player)
	local result = util.clone(bossTable)
	local job = player:get_job_name()
	local gender = player:get_gender()
	for i=1,#result do
		local itemData = result[i]
		local equipData = specialEquips[i]
		itemData.awards[1].id=equipData[job][gender][1]
		itemData.awards[1].effectRes = equipData[job][gender][2]
		for j=1,#itemData.boss do
			local bossData = itemData.boss[j]
			bossData.kill=util.ppn(player,bossData.con)--1:已击杀 0:未击杀
		end
		itemData.ling=util.ppn(player,itemData.con)--1:已领取 0:未领取
	end
	return result
end

--组的奖励领取逻辑
function getBossAward(player,index)
	local result = setNeedAward(player)
	local targetTable = result[index]
	local serverday = server_start_day()+1
	if not targetTable then
		player:alert(1,1, "数据出现异常错误！")
		return
	end
	if serverday>7 then
		player:alert(1,1, "活动时间已过！")
		return
	end
	if targetTable.ling==1 then
		player:alert(1,1, "该奖励已领取")
		return
	end
	local isLing = 1--全都击杀方可领取
	for i=1,#targetTable.boss do
		local itemData = targetTable.boss[i]
		if util.ppn(player,itemData.con)==0 then
			isLing=0
			break
		end
	end
	if isLing==0 then
		player:alert(1,1,"未完成击杀上组BOSS,无法领取！")
		return
	end
	if player:num_bag_black() < 4 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for j=1,#targetTable.awards do
		local awardData = targetTable.awards[j]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(targetTable.con,1)
	player:alert(1,1,"奖励领取成功！")
	senderToClient(player)
	gui.moduleRedPoint.checkOpenSvrBossUsable(player)
	-- if not haveBossAward(player) then
	-- 	gui.PanelActivities.senderToClientTab(player)
	-- end
end

--前往击杀
function goToKill(player,index)
	if index==2 then
		posnpc.fly(player,4002019)--"未知暗殿"
		player:push_lua_table("close",util.encode({name = "extend_awardHall"}))
	else
		player:push_lua_table("open",util.encode({name = "btn_main_boss"}))
	end
end

--击杀怪物时判断
function killMon(player,monName)
	local serverday = server_start_day()+1
	if serverday>7 then return end
	if killName[tostring(monName)] then
		local itemData = killName[monName]
		player:set_param(itemData.con,1)
		gui.moduleRedPoint.checkOpenSvrBossUsable(player)
	end

end


ckpanel.add_listener("gui.AwardHall_boss.handlePanelData",gui.AwardHall_boss.handlePanelData)


--全名BOSS红点
function checkRedPoint(player)
	for i=1,#bossTable do
		local itemData = bossTable[i]
		local ling = true
		if util.ppn(player,itemData.con)<=0 then
			for j=1,#itemData.boss do
				local bossData = itemData.boss[j]
				if util.ppn(player,bossData.con)<=0 then
					ling = false
					break
				end
			end
			if ling then
				return true
			end
		end
	end
	return false
end

--全名boss奖励领取完
function haveBossAward(player)
	for i=1,#bossTable do
		local itemData = bossTable[i]
		if util.ppn(player,itemData.con)==0 then
			return true
		end
	end
	return false
end




