module(... ,package.seeall)

local serverDayShowGroup = {
	{startDay = 1,endDay = 2,     group = {1,2,3,4,5,6,7,8,201}},
	{startDay = 3,endDay = 7,     group = {1,2,3,4,5,6,7,8,9,201}},
	{startDay = 8,endDay = 99999, group = {1,2,3,4,5,6,7,8,9,10,11,201}},
}

local showItems = {
	[1]={11113011,11113021,11113031,11113041,12113011,12113021,12113031,12113041,13113011,13113021,13113031,13113041},--开区第1天
	[2]={11114011,11114021,11114031,11114041,12114011,12114021,12114031,12114041,13114011,13114021,13114031,13114041},--开区第3天
	[3]={11116011,11116021,11116031,11116041,12116011,12116021,12116031,12116041,13116011,13116021,13116031,13116041},--开区第7天
}

local specialEquips = {
	[1]={
		["warrior"]={[200]={26010052,26010042},[201]={26010052,26010042}},--战士 男 女
		["wizard"] ={[200]={26020052,26020012},[201]={26020052,26020012}},--法士 男 女
		["taoist"] ={[200]={26030012,26030032},[201]={26030012,26030032}},--道士 男 女
	},
	[2]={
		["warrior"]={[200]={26010052,26010042},[201]={26010052,26010042}},
		["wizard"] ={[200]={26020052,26020012},[201]={26020052,26020012}},
		["taoist"] ={[200]={26030012,26030032},[201]={26030012,26030032}},
	},
	[3]={
		["warrior"]={[200]={26010052,26010042},[201]={26010052,26010042}},
		["wizard"] ={[200]={26020052,26020012},[201]={26020052,26020012}},
		["taoist"] ={[200]={26030012,26030032},[201]={26030012,26030032}},
	},

}

--每日寻宝次数奖励配置  宝藏钥匙 100W经验丹
local boxsTable = {
	[1]={bgnum=1,needNum=10,con=const.PP_LOTTERY_BOX_LING1,
			awards={
				{id=24060001,num=1,bind=1},
			},
		},
	[2]={bgnum=2,needNum=20,con=const.PP_LOTTERY_BOX_LING2,
			awards={
				{id=24060001,num=1,bind=1},
				{id=23100002,num=1,bind=1},
			},
		},
	[3]={bgnum=2,needNum=30,con=const.PP_LOTTERY_BOX_LING3,
			awards={
				{id=24060001,num=1,bind=1},
				{id=23100002,num=2,bind=1},
			},
		},
	[4]={bgnum=2,needNum=40,con=const.PP_LOTTERY_BOX_LING4,
			awards={
				{id=24060001,num=1,bind=1},
				{id=23100002,num=3,bind=1},
			},
		},
	[5]={bgnum=2,needNum=50,con=const.PP_LOTTERY_BOX_LING5,
			awards={
				{id=24060001,num=1,bind=1},
				{id=23100002,num=5,bind=1},
			},
		},
}

--宝箱tips说明
local boxAwardsDesp = {
	[1]="<font color=#E7BA52 size=18>宝箱奖励：</font><br><font color=#f1e8d0>1、宝藏钥匙*1</font>",
	[2]="<font color=#E7BA52 size=18>宝箱奖励：</font><br><font color=#f1e8d0>1、宝藏钥匙*1<br>2、100W经验丹*1</font>",
	[3]="<font color=#E7BA52 size=18>宝箱奖励：</font><br><font color=#f1e8d0>1、宝藏钥匙*1<br>2、100W经验丹*2</font>",
	[4]="<font color=#E7BA52 size=18>宝箱奖励：</font><br><font color=#f1e8d0>1、宝藏钥匙*1<br>2、100W经验丹*3</font>",
	[5]="<font color=#E7BA52 size=18>宝箱奖励：</font><br><font color=#f1e8d0>1、宝藏钥匙*1<br>2、100W经验丹*5</font>",
}

--庆典活动时显示的4个装备
local actEquips = {
	["warrior"]={
		{id = 11240011,	index = 6 },
		{id = 11240012,	index = 7 },
		{id = 11240013,	index = 13},
		{id = 11240014,	index = 14},
	},
	["wizard"]={
		{id = 11240011,	index = 6 },
		{id = 11240012,	index = 7 },
		{id = 11240013,	index = 13},
		{id = 11240014,	index = 14},
	},
	["taoist"]={
		{id = 11240011,	index = 6 },
		{id = 11240012,	index = 7 },
		{id = 11240013,	index = 13},
		{id = 11240014,	index = 14},
	},
}

function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "startLottery" then
		startLottery(player,data.params.times)
	elseif data.actionid == "reqShowItems" then 
		updateShowItems(player)
		updateLotteryRecord(player,nil,{},true)
		updateBoxsShow(player)
	elseif data.actionid == "reqBoxAward" then
		getLotteryNumAward(player,data.params.index)

	end
	
end

--推送14个展示的物品
function updateShowItems(player)
	local job = player:get_job_name()
	local gender = player:get_gender()
	local serverday = server_start_day()+1
	local index = 1
	if serverday>=7 then
		index=3
	elseif serverday>=3 then
		index=2
	end
	-- print(job,gender)
	local result = {
		cmd="updateShowItems",
		dataTable={},
		serverday=serverday,
	}
	table.insertto(result.dataTable,showItems[index])
	table.insertto(result.dataTable,specialEquips[index][tostring(job)][gender])
	if gui.PanelCelebrationAct.checkActIng() or gui.PanelHeQuAct.mergeTabOpenState("全民探宝") then
		for i=1,#actEquips[job] do
			local itemData = actEquips[job][i]
			result.dataTable[itemData.index]=itemData.id
		end
	end
	player:push_lua_table("PanelLottery",util.encode(result))
end

--点击宝箱领取奖励逻辑
function getLotteryNumAward(player,index)
	local itemData = boxsTable[index]
	if itemData then
		local lotteryNum=util.ppn(player,const.PP_LOTTERY_NUM)--今日寻宝次数
		if util.ppn(player,itemData.con)==1 then
			-- player:alert(1, 1, "该奖励已领取！")
			-- print(util.encode(tab.Lottery.lotteryGroup))
			player:push_lua_table("PanelLottery",util.encode({cmd="showBoxTips",desp=boxAwardsDesp[index]}))
			return
		end
		if lotteryNum<itemData.needNum then
			-- player:alert(1, 1, "寻宝次数未满足次数！")
			player:push_lua_table("PanelLottery",util.encode({cmd="showBoxTips",desp=boxAwardsDesp[index]}))
			return
		end
		if player:num_bag_black() < itemData.bgnum then
			player:alert(1,1,"背包空间不足！")
			return
		end
		player:set_param(itemData.con,1)
		for i=1,#itemData.awards do
			local awardData = itemData.awards[i]
			player:add_item(awardData.id,awardData.num,awardData.bind)
		end
		player:alert(1,1,"宝箱奖励领取成功！")
		updateBoxsShow(player)
	end
end

--刷新宝箱显示
function updateBoxsShow(player)
	local lotteryNum=util.ppn(player,const.PP_LOTTERY_NUM)--今日寻宝次数
	local tempTable=util.clone(boxsTable)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		itemData.ling=util.ppn(player,itemData.con)
	end
	local result = {
		cmd="updateBoxsState",
		dataTable=tempTable,
		lotteryNum=lotteryNum,
	}
	player:push_lua_table("PanelLottery",util.encode(result))
end

--首次开面板取10条全服记录
function getWorldRecord()
	local result = {}
	for i=1,30 do
		local str = server.get_var("lottery_world_"..i)
		if str~="" then
			table.insert(result,str)
		end
	end
	return result
end

--推送积分和寻宝日志
function updateLotteryRecord(player,myRecord,worldRecord,firstOpen)
	local result = {
		cmd="updateLotteryRecord",
		curJiFen=util.ppn(player,const.PP_XUNBAO_JIFEN),--寻宝积分
		curLotteryNum=util.ppn(player,const.PP_LOTTERY_NUM),--今日寻宝次数
		curRecord=myRecord,
		curWorldRecord=worldRecord,--表
		keyNum=player:num_item("宝藏钥匙"),
	}
	if firstOpen then
		result.curWorldRecord=getWorldRecord()
	end
	player:push_lua_table("PanelLottery",util.encode(result))
end

--存全服抽奖记录
function saveWorldRecord(player,record)
	-- local record = "<font color='#1debde'>"..player:get_name().."</font><font color='#9b9079'>获得:</font><font color='#a77602'><p>##"..itemName.."##</p></font>"
	for i=1,30 do
		if server.get_var("lottery_world_"..i)~="" then
			if i==30 then 
				for q=1,30 do
					if q==30 then
						server.set_var("lottery_world_"..q,record)
					else
						server.set_var("lottery_world_"..q,server.get_var("lottery_world_"..q+1))
					end
				end
			end
		else
			server.set_var("lottery_world_"..i,record)
			return
		end
	end
	-- updateLotteryRecord(player,nil,record)
end

function startLottery(player,times)
	onceLottery(player,times)
end

--活动时间内出副装的组
function addActEquipGroup(result)
	if gui.PanelHeQuAct.mergeTabOpenState("全民探宝") or gui.PanelCelebrationAct.checkActIng() then
		table.insert(result,#result+1,101)
	end
	return result
end

--开服天数能出现的抽奖组
function getEnableGroup(player)
	local result = {}
	local serverDay = server_start_day()+1
	local showGroup = util.clone(serverDayShowGroup)
	for i=1,#showGroup do
		local itemData = showGroup[i]
		if serverDay>=itemData.startDay and serverDay<=itemData.endDay then
			result=itemData.group
			break
		end
	end
	result = addActEquipGroup(result)
	return result
end

--根据本次全服抽奖次数来判定取抽奖表里的那个组来抽奖
function getNeedGroup(player,times)
	local enableGroup = getEnableGroup(player)
	local showGroup = util.clone(tab.Lottery.lotteryGroup)
	local groupId = 1
	for i=#enableGroup,1,-1 do	
		local itemData = showGroup[tostring(enableGroup[i])]
		if times%tonumber(itemData["getnum"])==0 then
			groupId=tonumber(enableGroup[i])
			break
		end
	end
	return groupId
end

--抽奖表中筛选出某一组的所有物品
function getOneGroupAllItems(player,times)
	local result = {}
	local groupId = getNeedGroup(player,times)
	for k,v in pairs(tab.Lottery.lotteryItem) do
		if groupId==v.group then
			table.insert(result,v)
			-- print(util.encode(v),v.group,times)
		end
	end
	return result
end

--单次寻宝逻辑
-- local num = 0
function onceLottery(player,times)
	-- num=num+1
	local record = "<font color='#1debde'>%s</font><font color='#9b9079'>获得:</font><p>##%s##</p>"
	-- updateLotteryRecord(player,record..num,record..num)
	if player:get_empty_number_in_lotterydepot()<times then
		return util.alert_msg(4, "寻宝背包格子不足！", player)
	end
	-- local serverDay = server_start_day()
	-- for k,v in ipairs(serverDayShowGroup) do

	-- 	if v.startDay <=serverDay and serverDay<=v.endDay then
	-- 		group = v.group;break;
	-- 	end
	-- end
	-- for k,v in pairs(tab.Lottery.lotteryItem) do
	-- 	if table.indexof(group,v.group) then
	-- 		table.insert(items,v)
	-- 	end
	-- end
	local severNum = util.svn("sever_lottery_num")--全服寻宝次数
	server.set_var("sever_lottery_num",severNum+times)--累计全服寻宝次数

	-- player:sub_vcoin(200*times,"寻宝")
	if not lotteryXiaoHao(player,times) then return end

	player:set_param(const.PP_XUNBAO_JIFEN,util.ppn(player,const.PP_XUNBAO_JIFEN)+times);
	player:set_param(const.PP_LOTTERY_NUM,util.ppn(player,const.PP_LOTTERY_NUM)+times);
	updateBoxsShow(player)
	for i=1,times do
		local items = getOneGroupAllItems(player,severNum+i)
		local index = util.produceProb(items)
		if items[index] then
			if items[index].bind==2 then
				player:add_item_lottery(items[index].typeId,1)
			else
				player:add_item_lottery(items[index].typeId,1,1)
			end
			-- local str = string.format(record,player:get_name(),tab.getItemName(items[index].typeId))
			local str = string.format(record,player:get_name(), items[index].typeId)

			if items[index].isshow>0 then
				updateLotteryRecord(player,str,{str})
				saveWorldRecord(player,str)

				local resultStr2 = string.format("<font color='#01ebff'>%s</font>探寻宝藏,获得:<p>##%s##</p>",player:get_name(),tab.getItemName(items[index].typeId))
				server.info(10,0," "..resultStr2)
				resultStr2 = string.format("<font color='#01ebff'>%s</font>探寻宝藏,获得:%s",player:get_name(),tab.getItemColorName(items[index].typeId))
				server.info(100,0," "..resultStr2)
			else
				updateLotteryRecord(player,str,nil)
			end
			gui.PanelGongXun.checkTaskComplete(player,6)
		end
	end
end

--寻宝扣元宝或者钥匙逻辑
function lotteryXiaoHao(player,times)
	local num = player:num_item("宝藏钥匙")
	local needVcion = 200*(times-num)
	if needVcion<0 then needVcion=0 end
	if player:get_vcoin()<needVcion then
		showChongzhi(player)
		return false
	end
	if num>=times then
		player:remove_item("宝藏钥匙",times)
	else
		player:remove_item("宝藏钥匙",num)
		player:sub_vcoin(200*(times-num),"寻宝")
	end
	return true
end

ckpanel.add_listener("gui.PanelLottery.handlePanelData",gui.PanelLottery.handlePanelData)

function addLotteryScore(player,exp)
	if type(exp) == "number" then
		player:set_param(const.PP_XUNBAO_JIFEN,util.ppn(player,const.PP_XUNBAO_JIFEN)+exp);
		player:alert(1,0,"增加宝藏积分"..exp)
		return true
	end
end





