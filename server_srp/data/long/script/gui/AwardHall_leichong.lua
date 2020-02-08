module(... ,package.seeall)


local awardsTable = {
	[1]={
		{index=1, needNum=1000,  con=const.PP_LEICHONG_LING1, ids={25010003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1}, effectRes = {0,0,65078,0}},
		{index=2, needNum=10000, con=const.PP_LEICHONG_LING2, ids={25010004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=3, needNum=30000, con=const.PP_LEICHONG_LING3, ids={25010005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=4, needNum=50000, con=const.PP_LEICHONG_LING4, ids={25010006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=5, needNum=100000,con=const.PP_LEICHONG_LING5, ids={25010007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1}, effectRes = {65080,65078,65078,65078}},
		{index=6, needNum=300000,con=const.PP_LEICHONG_LING6, ids={25010008,24020003,23060003,15200039}, nums={2,150,5,1}, binds={1,1,0}, effectRes = {65080,65078,65078,65080}},
	},
	[2]={
		{index=1, needNum=1000,  con=const.PP_LEICHONG_LING1, ids={25040003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1}, effectRes = {0,0,65078,0}},
		{index=2, needNum=10000, con=const.PP_LEICHONG_LING2, ids={25040004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=3, needNum=30000, con=const.PP_LEICHONG_LING3, ids={25040005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=4, needNum=50000, con=const.PP_LEICHONG_LING4, ids={25040006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=5, needNum=100000,con=const.PP_LEICHONG_LING5, ids={25040007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1}, effectRes = {65080,65078,65078,65078}},
		{index=6, needNum=300000,con=const.PP_LEICHONG_LING6, ids={25040008,24020003,23060003,15200039}, nums={2,150,5,1}, binds={1,1,0}, effectRes = {65080,65078,65078,65080}},
	},
	[3]={
		{index=1, needNum=1000,  con=const.PP_LEICHONG_LING1, ids={25030003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1}, effectRes = {0,0,65078,0}},
		{index=2, needNum=10000, con=const.PP_LEICHONG_LING2, ids={25030004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=3, needNum=30000, con=const.PP_LEICHONG_LING3, ids={25030005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=4, needNum=50000, con=const.PP_LEICHONG_LING4, ids={25030006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=5, needNum=100000,con=const.PP_LEICHONG_LING5, ids={25030007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1}, effectRes = {65080,65078,65078,65078}},
		{index=6, needNum=300000,con=const.PP_LEICHONG_LING6, ids={25030008,24020003,23060003,15200039}, nums={2,150,5,1}, binds={1,1,0}, effectRes = {65080,65078,65078,65080}},
	},
	[4]={
		{index=1, needNum=1000,  con=const.PP_LEICHONG_LING1, ids={25080003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1}, effectRes = {0,0,65078,0}},
		{index=2, needNum=10000, con=const.PP_LEICHONG_LING2, ids={25080004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=3, needNum=30000, con=const.PP_LEICHONG_LING3, ids={25080005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=4, needNum=50000, con=const.PP_LEICHONG_LING4, ids={25080006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=5, needNum=100000,con=const.PP_LEICHONG_LING5, ids={25080007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1}, effectRes = {65080,65078,65078,65078}},
		{index=6, needNum=300000,con=const.PP_LEICHONG_LING6, ids={25080008,24020003,23060003,15200039}, nums={2,150,5,1}, binds={1,1,0}, effectRes = {65080,65078,65078,65080}},
	},
	[5]={
		{index=1, needNum=1000,  con=const.PP_LEICHONG_LING1, ids={25020003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1}, effectRes = {0,0,65078,0}},
		{index=2, needNum=10000, con=const.PP_LEICHONG_LING2, ids={25020004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=3, needNum=30000, con=const.PP_LEICHONG_LING3, ids={25020005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=4, needNum=50000, con=const.PP_LEICHONG_LING4, ids={25020006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=5, needNum=100000,con=const.PP_LEICHONG_LING5, ids={25020007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1}, effectRes = {65080,65078,65078,65078}},
		{index=6, needNum=300000,con=const.PP_LEICHONG_LING6, ids={25020008,24020003,23060003,15200039}, nums={2,150,5,1}, binds={1,1,0}, effectRes = {65080,65078,65078,65080}},
	},
	[6]={
		{index=1, needNum=1000,  con=const.PP_LEICHONG_LING1, ids={25070003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1}, effectRes = {0,0,65078,0}},
		{index=2, needNum=10000, con=const.PP_LEICHONG_LING2, ids={25070004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=3, needNum=30000, con=const.PP_LEICHONG_LING3, ids={25070005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=4, needNum=50000, con=const.PP_LEICHONG_LING4, ids={25070006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=5, needNum=100000,con=const.PP_LEICHONG_LING5, ids={25070007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1}, effectRes = {65080,65078,65078,65078}},
		{index=6, needNum=300000,con=const.PP_LEICHONG_LING6, ids={25070008,24020003,23060003,15200039}, nums={2,150,5,1}, binds={1,1,0}, effectRes = {65080,65078,65078,65080}},
	},
	[7]={
		{index=1, needNum=1000,  con=const.PP_LEICHONG_LING1, ids={25060003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1}, effectRes = {0,0,65078,0}},
		{index=2, needNum=10000, con=const.PP_LEICHONG_LING2, ids={25060004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=3, needNum=30000, con=const.PP_LEICHONG_LING3, ids={25060005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=4, needNum=50000, con=const.PP_LEICHONG_LING4, ids={25060006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1}, effectRes = {65078,65078,65078,65078}},
		{index=5, needNum=100000,con=const.PP_LEICHONG_LING5, ids={25060007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1}, effectRes = {65080,65078,65078,65078}},
		{index=6, needNum=300000,con=const.PP_LEICHONG_LING6, ids={25060008,24020003,23060003,15200039}, nums={2,150,5,1}, binds={1,1,0}, effectRes = {65080,65078,65078,65080}},
	},

}

function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "reqLeiChongData" then
		senderToClient(player)
	elseif data.actionid == "lingLeiChongData" then
		getAwards(player,data.params.index)
	elseif data.actionid == "" then

	end
end

--根据开区天数取对应的表数据
function getNeedTable(player)
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local serverDay = server_start_day()+1--server_start_day()开区天数实在0点跟新的
	if H==0 and M==0 and S<60 then
		serverDay=serverDay-1
	end
	if serverDay>7 then return end--超过活动期限
	if awardsTable[serverDay] then
		return awardsTable[serverDay]
	end
	return awardsTable[1]
end

--推送在线奖励数据
function senderToClient(player)
	local result = {
		cmd="updateLeiChongData",
		dataTable=getNeedTable(player),
	}
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))

	if H==0 and M==0 and S<60 then
		result.time=60-S
	else
		result.time=(24-H)*3600-M*60-S+60
	end

	local dailyRecharge = util.ppn(player,const.PP_DAILY_RECHARGE)
	for i=1,#result.dataTable do
		local itemData = result.dataTable[i]
		if dailyRecharge>=itemData.needNum then
			if util.ppn(player,itemData.con)>0 then
				itemData.flag=3--已领取
			else
				itemData.flag=2--可领取
			end
		else
			itemData.flag=1--未达成
		end
	end
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--领取累计充值奖励
function getAwards(player,index)
	local dataTable=getNeedTable(player)
	if not dataTable or not dataTable[index] then
		return player:alert(1,1,"数据异常！")
	end
	local itemData = dataTable[index]
	if util.ppn(player,itemData.con)==1 then
		player:alert(1,1,"该档奖励已领取,不可重复领取！")
		return
	end
	if player:num_bag_black() < #itemData.ids then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#itemData.ids do
		player:add_item(itemData.ids[i],itemData.nums[i],itemData.binds[i])
	end
	player:set_param(itemData.con,1)
	senderToClient(player)
	gui.moduleRedPoint.checkKfLeiChong(player)
end

function isActing(player)
	local serverDay = server_start_day()+1
	if serverDay<=7 then
		gui.moduleRedPoint.checkKfLeiChong(player)
		return true
	end
	return false
end

--红点检测
function checkRedPoint(player)
	local dataTable=getNeedTable(player)
	if not dataTable then return false end
	local dailyRecharge = util.ppn(player,const.PP_DAILY_RECHARGE)
	for i=1,#dataTable do
		local itemData = dataTable[i]
		if dailyRecharge>=itemData.needNum and util.ppn(player,itemData.con)<=0 then
			return true
		end
	end
	return false
end





ckpanel.add_listener("gui.AwardHall_leichong.handlePanelData",gui.AwardHall_leichong.handlePanelData)









