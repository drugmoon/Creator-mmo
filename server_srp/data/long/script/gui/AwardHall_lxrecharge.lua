module(... ,package.seeall)

local awardsTable = {
	{index=1 , needDays=1, needNum=1000 , con=const.PP_LXCZ_LING1 , ids={25010003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1,1}, effectRes = {0,0,65078,0}},
	{index=2 , needDays=1, needNum=5000 , con=const.PP_LXCZ_LING2 , ids={25010004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=3 , needDays=1, needNum=10000, con=const.PP_LXCZ_LING3 , ids={25010005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=4 , needDays=2, needNum=1000 , con=const.PP_LXCZ_LING4 , ids={25010006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=5 , needDays=2, needNum=5000 , con=const.PP_LXCZ_LING5 , ids={25010007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1,1}, effectRes = {65080,65078,65078,65078}},
	{index=6 , needDays=2, needNum=10000, con=const.PP_LXCZ_LING6 , ids={25010008,24020003,23060003,15200021}, nums={2,150,5,1}, binds={1,1,1,0}, effectRes = {65080,65078,65078,65080}},
	{index=7 , needDays=3, needNum=1000 , con=const.PP_LXCZ_LING7 , ids={25040003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1,1}, effectRes = {0,0,65078,0}},
	{index=8 , needDays=3, needNum=5000 , con=const.PP_LXCZ_LING8 , ids={25040004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=9 , needDays=3, needNum=10000, con=const.PP_LXCZ_LING9 , ids={25040005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=10, needDays=4, needNum=1000 , con=const.PP_LXCZ_LING10, ids={25040006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=11, needDays=4, needNum=5000 , con=const.PP_LXCZ_LING11, ids={25040007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1,1}, effectRes = {65080,65078,65078,65078}},
	{index=12, needDays=4, needNum=10000, con=const.PP_LXCZ_LING12, ids={25040008,24020003,23060003,15200021}, nums={2,150,5,1}, binds={1,1,1,0}, effectRes = {65080,65078,65078,65080}},
	{index=13, needDays=5, needNum=1000 , con=const.PP_LXCZ_LING13, ids={25030003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1,1}, effectRes = {0,0,65078,0}},
	{index=14, needDays=5, needNum=5000 , con=const.PP_LXCZ_LING14, ids={25030004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=15, needDays=5, needNum=10000, con=const.PP_LXCZ_LING15, ids={25030005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=16, needDays=6, needNum=1000 , con=const.PP_LXCZ_LING16, ids={25030006,24020003,23060003,32000006}, nums={2,85,2,5 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=17, needDays=6, needNum=5000 , con=const.PP_LXCZ_LING17, ids={25030007,24020003,23060003,32000006}, nums={2,100,5,6}, binds={1,1,1,1}, effectRes = {65080,65078,65078,65078}},
	{index=18, needDays=6, needNum=10000, con=const.PP_LXCZ_LING18, ids={25030008,24020003,23060003,15200021}, nums={2,150,5,1}, binds={1,1,1,0}, effectRes = {65080,65078,65078,65080}},
	{index=19, needDays=7, needNum=1000 , con=const.PP_LXCZ_LING19, ids={25080003,24020002,32000006,32000002}, nums={2,25,1,2 }, binds={1,1,1,1}, effectRes = {0,0,65078,0}},
	{index=20, needDays=7, needNum=5000 , con=const.PP_LXCZ_LING20, ids={25080004,24020003,23060003,32000006}, nums={2,60,1,3 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
	{index=21, needDays=7, needNum=10000, con=const.PP_LXCZ_LING21, ids={25080005,24020003,23060003,32000006}, nums={2,70,1,4 }, binds={1,1,1,1}, effectRes = {65078,65078,65078,65078}},
}

function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "reqLxczData" then
		senderToClient(player)
	elseif data.actionid == "reqLxczLing" then
		getAwards(player,data.params.index)
	elseif data.actionid == "" then

	end
end

--推送连续奖励数据
function senderToClient(player,typedang)
	local result = {
		cmd="updateLxczData",
		dataTable=getNeedTable(player),
		typedang=typedang or 1,
	}
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	result.time=(7-server_start_day()-1)*24*3600+(24-H)*3600-M*60-S

	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--处理数据给客户端
function getNeedTable(player)
	local dayNumOne   = util.ppn(player,const.PP_LXCZ_DANG1)
	local dayNumTwo   = util.ppn(player,const.PP_LXCZ_DANG2)
	local dayNumThree = util.ppn(player,const.PP_LXCZ_DANG3)
	local tempTable=util.clone(awardsTable)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		local tempDayNum = 0
		if itemData.needNum==1000 then
			tempDayNum=dayNumOne
		elseif itemData.needNum==5000 then
			tempDayNum=dayNumTwo
		elseif itemData.needNum==10000 then
			tempDayNum=dayNumThree
		end
		if util.ppn(player,itemData.con)>=1 then
			itemData.flag=3--已领取
		else
			if tempDayNum>=itemData.needDays then
				itemData.flag=2--可领取
			else
				itemData.flag=1--未达成
			end
		end
	end
	return tempTable
end

--领取连续充值奖励
function getAwards(player,index)
	local dayNumOne   = util.ppn(player,const.PP_LXCZ_DANG1)
	local dayNumTwo   = util.ppn(player,const.PP_LXCZ_DANG2)
	local dayNumThree = util.ppn(player,const.PP_LXCZ_DANG3)
	if not awardsTable[index] then
		return player:alert(1,1,"数据异常！")
	end
	local itemData=awardsTable[index]

	local tempDayNum = 0
	local typeDang = nil
	if itemData.needNum==1000 then
		tempDayNum=dayNumOne
		typeDang=1
	elseif itemData.needNum==5000 then
		tempDayNum=dayNumTwo
		typeDang=2
	elseif itemData.needNum==10000 then
		tempDayNum=dayNumThree
		typeDang=3
	end
	if util.ppn(player,itemData.con)>=1 then
		return player:alert(1,1,"已领取，不能重复领取！")
	end
	if tempDayNum<itemData.needDays then
		return player:alert(1,1,"未达成！")
	end
	if player:num_bag_black()<#itemData.ids then
		return player:alert(1,1,"背包空间不足！")
	end
	for i=1,#itemData.ids do
		player:add_item(itemData.ids[i],itemData.nums[i],itemData.binds[i])
	end
	player:set_param(itemData.con,1)
	senderToClient(player,typeDang)
	gui.moduleRedPoint.checkKfLxRecharge(player)
end

--计算每档连续的天数
function setLxczDayNum(player)
	local serverDay = server_start_day()+1--server_start_day()开区天数实在0点跟新的
	if serverDay>7 then return end
	-- print("serverDay",serverDay)
	local dailyRecharge = util.ppn(player,const.PP_DAILY_RECHARGE)
	if dailyRecharge>=1000 then
		local result = player:get_param(const.PP_LXCZ_DAYINFO1)
		if result==nil or result=="" then
			result={0,0,0,0,0,0,0}
		else
			result=util.decode(result)
		end
		result[serverDay]=1--标记今日充值
		-- if serverDay>1 and result[serverDay-1]<=0 then--前一天没充值重新计算累计
		-- 	for i=1,(serverDay-1) do
		-- 		result[i]=0
		-- 	end
		-- end
		player:set_param(const.PP_LXCZ_DAYINFO1,util.encode(result))
		player:set_param(const.PP_LXCZ_DANG1,(result[1]+result[2]+result[3]+result[4]+result[5]+result[6]+result[7]))
		-- print("1000===",util.ppn(player,const.PP_LXCZ_DANG1))
	end
	if dailyRecharge>=5000 then
		local result = player:get_param(const.PP_LXCZ_DAYINFO2)
		if result==nil or result=="" then
			result={0,0,0,0,0,0,0}
		else
			result=util.decode(result)
		end
		result[serverDay]=1--标记今日充值
		-- if serverDay>1 and result[serverDay-1]<=0 then--前一天没充值重新计算累计
		-- 	for i=1,(serverDay-1) do
		-- 		result[i]=0
		-- 	end
		-- end
		player:set_param(const.PP_LXCZ_DAYINFO2,util.encode(result))
		player:set_param(const.PP_LXCZ_DANG2,(result[1]+result[2]+result[3]+result[4]+result[5]+result[6]+result[7]))
		-- print("5000===",util.ppn(player,const.PP_LXCZ_DANG2))
	end
	if dailyRecharge>=10000 then
		local result = player:get_param(const.PP_LXCZ_DAYINFO3)
		if result==nil or result=="" then
			result={0,0,0,0,0,0,0}
		else
			result=util.decode(result)
		end
		result[serverDay]=1--标记今日充值
		-- if serverDay>1 and result[serverDay-1]<=0 then--前一天没充值重新计算累计
		-- 	for i=1,(serverDay-1) do
		-- 		result[i]=0
		-- 	end
		-- end
		player:set_param(const.PP_LXCZ_DAYINFO3,util.encode(result))
		player:set_param(const.PP_LXCZ_DANG3,(result[1]+result[2]+result[3]+result[4]+result[5]+result[6]+result[7]))
		-- print("10000===",util.ppn(player,const.PP_LXCZ_DANG3))
	end
 	gui.moduleRedPoint.checkKfLxRecharge(player)
end


--红点检测
function checkRedPoint(player)
	local dayNumOne   = util.ppn(player,const.PP_LXCZ_DANG1)
	local dayNumTwo   = util.ppn(player,const.PP_LXCZ_DANG2)
	local dayNumThree = util.ppn(player,const.PP_LXCZ_DANG3)
	for i=1,#awardsTable do
		local itemData=awardsTable[i]
		local typeDang = nil
		if itemData.needNum==1000 then
			tempDayNum=dayNumOne
		elseif itemData.needNum==5000 then
			tempDayNum=dayNumTwo
		elseif itemData.needNum==10000 then
			tempDayNum=dayNumThree
		end
		if util.ppn(player,itemData.con)<=0 and tempDayNum>=itemData.needDays then--为领取且可领取
			return true
		end
	end
	return false
end

ckpanel.add_listener("gui.AwardHall_lxrecharge.handlePanelData",gui.AwardHall_lxrecharge.handlePanelData)









