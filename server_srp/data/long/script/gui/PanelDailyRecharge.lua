module(..., package.seeall)

local awardsTab = {
	[1]={
		[1]={need=700,   ids={23100006,24000001,32000001,32000002},nums={1,1,1,1}, binds={1,1,1,1},value=1100, con=const.PP_MEIRI_LING1},
		[2]={need=5000,  ids={23100006,24000001,32000002,32000003},nums={5,1,2,2}, binds={1,1,1,1},value=3500, con=const.PP_MEIRI_LING2},
		[3]={need=10000, ids={23100007,24000001,32000006,32000003},nums={2,1,3,2}, binds={1,1,1,1},value=4500, con=const.PP_MEIRI_LING3},
		[4]={need=20000, ids={23100007,24000001,32000006,32000003},nums={2,2,3,2}, binds={1,1,1,1},value=5500, con=const.PP_MEIRI_LING4},
		[5]={need=50000, ids={23040001,24030001,23050002,32000003},nums={5,40,5,2},binds={1,1,1,1},value=9000, con=const.PP_MEIRI_LING5},
		[6]={need=100000,ids={23040002,24030001,23050002,32000003},nums={1,50,5,2},binds={1,1,1,1},value=14300,con=const.PP_MEIRI_LING6},
	},
	[2]={
		[1]={need=700,   ids={23040001,24030001,24000001,32000001},nums={1,20,1,1}, binds={1,1,1,1},value=2000, con=const.PP_MEIRI_LING1},
		[2]={need=5000,  ids={23040001,24030001,24000001,32000003},nums={1,50,1,1}, binds={1,1,1,1},value=3000, con=const.PP_MEIRI_LING2},
		[3]={need=10000, ids={23040001,24030001,24000001,32000006},nums={1,80,2,3}, binds={1,1,1,1},value=4600, con=const.PP_MEIRI_LING3},
		[4]={need=20000, ids={23040001,24030001,24000001,32000006},nums={1,90,2,3}, binds={1,1,1,1},value=4900, con=const.PP_MEIRI_LING4},
		[5]={need=50000, ids={23040001,24030001,23050002,23110002},nums={2,100,5,1},binds={1,1,1,1},value=8500, con=const.PP_MEIRI_LING5},
		[6]={need=100000,ids={23040001,24030001,23110002,23050002},nums={2,150,2,5},binds={1,1,1,1},value=11000,con=const.PP_MEIRI_LING6},
	},
	[3]={
		[1]={need=700,   ids={23040001,23020002,23090002,32000001},nums={1,1,1,1},binds={1,1,1,1},value=2400, con=const.PP_MEIRI_LING1},
		[2]={need=5000,  ids={23040001,23020002,23090002,32000003},nums={1,2,1,1},binds={1,1,1,1},value=3500, con=const.PP_MEIRI_LING2},
		[3]={need=10000, ids={23040001,23020002,23090002,32000006},nums={2,3,1,3},binds={1,1,1,1},value=4200, con=const.PP_MEIRI_LING3},
		[4]={need=20000, ids={23040001,23020002,23090002,32000006},nums={2,4,1,3},binds={1,1,1,1},value=5200, con=const.PP_MEIRI_LING4},
		[5]={need=50000, ids={23040001,23020002,23050002,23110002},nums={2,3,5,1},binds={1,1,1,1},value=8500, con=const.PP_MEIRI_LING5},
		[6]={need=100000,ids={23040001,23020002,23110002,23050002},nums={2,3,3,5},binds={1,1,1,1},value=10500,con=const.PP_MEIRI_LING6},
	},
	[4]={
		[1]={need=700,   ids={23040001,23030002,23050002,32000001},nums={1,1,1,1},binds={1,1,1,1},value=2400, con=const.PP_MEIRI_LING1},
		[2]={need=5000,  ids={23040001,23030002,23050002,32000003},nums={1,2,1,1},binds={1,1,1,1},value=3500, con=const.PP_MEIRI_LING2},
		[3]={need=10000, ids={23040001,23030002,23050002,32000006},nums={2,3,1,3},binds={1,1,1,1},value=4200, con=const.PP_MEIRI_LING3},
		[4]={need=20000, ids={23040001,23030002,23050002,32000006},nums={2,4,1,3},binds={1,1,1,1},value=5200, con=const.PP_MEIRI_LING4},
		[5]={need=50000, ids={23040001,23030002,23050002,23110002},nums={2,3,5,1},binds={1,1,1,1},value=8500, con=const.PP_MEIRI_LING5},
		[6]={need=100000,ids={23040001,23030002,23110002,23050002},nums={2,3,3,5},binds={1,1,1,1},value=10500,con=const.PP_MEIRI_LING6},
	},
	[5]={
		[1]={need=600,   ids={23040001,23090002,32000001,32000002},nums={1,1,1,2},binds={1,1,1,1},value=1800, con=const.PP_MEIRI_LING1},
		[2]={need=5000,  ids={23040001,24000001,23090002,32000002},nums={2,1,1,3},binds={1,1,1,1},value=3700, con=const.PP_MEIRI_LING2},
		[3]={need=10000, ids={23040001,24000001,23110002,32000003},nums={2,1,1,4},binds={1,1,1,1},value=4300, con=const.PP_MEIRI_LING3},
		[4]={need=20000, ids={23040001,24000001,23110002,32000003},nums={2,1,2,4},binds={1,1,1,1},value=5400, con=const.PP_MEIRI_LING4},
		[5]={need=50000, ids={23040001,23050002,23020002,32000003},nums={1,5,4,4},binds={1,1,1,1},value=8100, con=const.PP_MEIRI_LING5},
		[6]={need=100000,ids={23040001,23140003,23030002,32000003},nums={3,5,5,5},binds={1,1,1,1},value=11300,con=const.PP_MEIRI_LING6},
	},
}

function onPanelData(player, commond)
	local data =util.decode(commond);
	if data.actionid == "reqUpdateData" then
		updateClient(player)
	elseif data.actionid == "receive" then
		receiveAwards(player)
	end
end

ckpanel.add_listener("gui.PanelDailyRecharge.onPanelData",gui.PanelDailyRecharge.onPanelData)

--每日充值1-4天的特殊检测
function checkRechargeRecord(player)
	local startDay = server_start_day()+1
	local desNum=0
	if startDay<5 then
		if util.ppn(player,const.PP_RCHARGE_FLAG)>0 and util.ppn(player,const.PP_RCHARGE_FLAG)<startDay then
			desNum=100
		end
	end
	return desNum
end

--刷新客户端操作
function updateClient(player)
	local startDay = server_start_day()+1
	if startDay>5 then startDay=5 end
	if not awardsTab[startDay] then return end
	local tempTable = awardsTab[startDay]
	local rechargeNum = util.ppn(player,const.PP_DAILY_RECHARGE)
	local desNum=checkRechargeRecord(player)
	local result = {
		cmd="updateRechargeAwards",
		dataTable=util.clone(tempTable[5]),
		ling=1,
		againc=0,
		hideBtn=true,
	}
	local find = false
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if rechargeNum>=(itemData.need-desNum) then
			if util.ppn(player,itemData.con)<=0 then
				result.dataTable=util.clone(itemData)
				result.ling=0
				result.againc=0
				find=true
				result.hideBtn=false
				break
			end
		end
	end
	if not find then
		for i=1,#tempTable do
			local itemData = tempTable[i]
			if rechargeNum<(itemData.need-desNum) then
				result.dataTable=util.clone(itemData)
				result.ling=0
				result.againc=(itemData.need-desNum)-rechargeNum
				find=true
				result.hideBtn=false
				break
			end
		end
	end
	player:push_lua_table("PanelDailyRecharge",util.encode(result))
end

--领取奖励操作
function receiveAwards(player)
	local startDay = server_start_day()+1
	if startDay>5 then startDay=5 end
	if not awardsTab[startDay] then return end
	local tempTable = awardsTab[startDay]
	local rechargeNum = util.ppn(player,const.PP_DAILY_RECHARGE)
	local desNum=checkRechargeRecord(player)
	local result = {}
	local find = false
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if rechargeNum>=(itemData.need-desNum) then
			if util.ppn(player,itemData.con)<=0 then
				result=util.clone(itemData)
				find=true
				break
			end
		end
	end
	if not find then
		player:alert(1,1,"数据异常！")
		return
	end
	if player:num_bag_black() < 4 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#result.ids do
		player:add_item(result.ids[i],result.nums[i],result.binds[i])
	end
	player:set_param(result.con,1)
	if result.need==700 or result.need==600 then
		server.info(10000,0,"玩家<font color='#00ccff'>"..player:get_name().."</font>成功领取价值"..result.value.."元宝<font color='#ff0000'>【每日充值】</font>,实力猛增！")
		server.info(10, 1, "<font color='#fff843'>玩家<font color='#00ccff'>"..player:get_name().."</font>成功领取价值"..result.value.."元宝<font color='#ff0000'>【每日充值】</font>,实力猛增！</font>")
	end
	updateClient(player)
	if result.need==100000 then
		gui.moduleRedPoint.checkDailyRecharge(player)
		gui.moduleGuiButton.checkOpenFunc(player)
		player:push_lua_table("close",util.encode({name = "extend_dailyPay"}))
	end
end

--每日奖励是否已领取完
function getAwardsFinish(player)
	local result = true
	for i=0,5 do
		if util.ppn(player,const.PP_MEIRI_LING1+i)<=0 then
			result=false
			break
		end
	end
	return result
end

