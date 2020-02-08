module(..., package.seeall)

local showItems = {
	{index=1 , id=21010005,num=1,bind=1, prob=1,isshow=true,},
	{index=7 , id=21010017,num=1,bind=1, prob=5,isshow=true,},
	{index=10, id=21010016,num=1,bind=1, prob=10,isshow=true,},
	{index=4 , id=21010010,num=1,bind=1, prob=80,isshow=true,},
	--{index=11, id=21010009,num=1,bind=1, prob=500},
	{index=8 , id=21010003,num=1,bind=1, prob=500,isshow=false,},
	{index=5 , id=21010007,num=1,bind=1, prob=500,isshow=false,},
	{index=2 , id=21010002,num=1,bind=1, prob=500,isshow=false,},
	--{index=12, id=21010015,num=1,bind=1, prob=1000},
	{index=9 , id=21010006,num=1,bind=1, prob=1000,isshow=false,},
	{index=6 , id=21010014,num=1,bind=1, prob=1000,isshow=false,},
	{index=3 , id=21010001,num=1,bind=1, prob=1000,isshow=false,},
}

function handlePanelData(player,commond)
		-- for i=1,30 do

		-- 	server.set_var("lottery_zhuanpan_"..i,"")
		-- end
		-- player:set_param(const.PP_ZHUANPAN_TIMES,1000)
	local data = util.decode(commond);
	if data.actionid == "choujiang" then---抽奖
		local num= data.param and 10 or 1
		checkCondition(player,num)
	elseif data.actionid == "getPanelData" then---打开面板
		updateLotteryRecord(player,nil,{},true)
		updateClientItems(player)
	elseif data.actionid=="rotateStop" then
		giveAwards(player)
	end
end

--推送积分和寻宝日志
function updateLotteryRecord(player,myRecord,worldRecord,firstOpen)
	local result = {
		cmd="updateRecord",
		curRecord=myRecord,
		curWorldRecord=worldRecord,--表
		yuTimes=util.ppn(player,const.PP_ZHUANPAN_TIMES),
	}
	if firstOpen then
		result.curWorldRecord=getWorldRecord()
	end
	local t1,t2 =gui.PanelJingCaiAct.getActivityTime(2011)
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	result.time=(gui.PanelJingCaiAct.formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S
	player:push_lua_table("PanelZhuanPan",util.encode(result))
end

--显示的转盘道具
function updateClientItems(player)
	local result = {
		cmd="updateShowItems",
		dataTable=util.clone(showItems),
	}

	player:push_lua_table("PanelZhuanPan",util.encode(result))
end


--首次开面板取10条全服记录
function getWorldRecord()
	local result = {}
	for i=1,30 do
		local str = server.get_var("lottery_zhuanpan_"..i)
		if str~="" then
			table.insert(result,str)
		end
	end
	return result
end



--判断条件 给物品
function checkCondition(player,times)
	-- print(">>>>>>>",times)

	local t1,t2 =gui.PanelJingCaiAct.getActivityTime(2011)
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local time=(gui.PanelJingCaiAct.formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S
	if time<= 0 then
		player:push_lua_table("PanelZhuanPan",util.encode({cmd="openStartBtn"}))
		return util.alert_msg(4, "活动已结束", player)
		
	end
	if player:num_bag_black()<times then
		player:push_lua_table("PanelZhuanPan",util.encode({cmd="openStartBtn"}))
		return util.alert_msg(4, "背包格子不足！", player)
		
	end
	if times>util.ppn(player,const.PP_ZHUANPAN_TIMES) then
		player:push_lua_table("PanelZhuanPan",util.encode({cmd="openStartBtn"}))
		return util.alert_msg(4, "剩余抽奖次数不足"..times.."次", player)
		
	end

	local record = "<font color='#FDDFAE'>%s</font><font color='#FFB400'>获得： </font><p>##%s##</p>"
	local result = {}
	for i=1,times do
		local rand=util.produceProb(showItems)
		if showItems[rand] then
			-- player:add_item(showItems[rand].id,1,1)
			-- local str = string.format(record,player:get_name(),showItems[rand].id)
			-- updateLotteryRecord(player,str,{str})
			-- saveWorldRecord(player,str)
			table.insert(result,showItems[rand])
			if i==times then
				player:push_lua_table("PanelZhuanPan",util.encode({cmd="startRotate",index=showItems[rand].index}))
				-- print(rand)
			end
		end
	end
	player:set_param(const.PP_ZHUANPAN_AWARDS,util.encode(result))
	-- print(player:get_param(id))
end

--客户端动画结束开始给物品
function giveAwards(player)
	local record = "<font color='#FDDFAE'>%s</font><font color='#FFB400'>获得： </font><p>##%s##</p>"
	local result = util.decode(player:get_param(const.PP_ZHUANPAN_AWARDS))
	if type(result)=="table" and #result>0 then
		for i=1,#result do
			local itemData = result[i]
			-- print(itemData.id,itemData.num,itemData.bind)
			player:add_item(itemData.id,itemData.num,itemData.bind)
			local str = string.format(record,player:get_name(),itemData.id)
			updateLotteryRecord(player,str,{str})
			saveWorldRecord(player,str)
			if itemData.isshow then
				resultStr2 = string.format("恭喜玩家[<font color='#01ebff'>%s</font>]在幸运转盘活动,获得:%s",player:get_name(),tab.getItemColorName(itemData.id))
				server.info(100,0," "..resultStr2)
			end
		end
		player:set_param(const.PP_ZHUANPAN_TIMES,util.ppn(player,const.PP_ZHUANPAN_TIMES)-#result)
		-- updateLotteryRecord(player,nil,{},true)
		player:push_lua_table("PanelZhuanPan",util.encode({cmd="updateYuTimes",yuTimes=util.ppn(player,const.PP_ZHUANPAN_TIMES)}))--刷新剩余次数
		gui.moduleRedPoint.checkEventsUsable(player)
	end
end


--存全服抽奖记录
function saveWorldRecord(player,record)
	-- local record = "<font color='#1debde'>"..player:get_name().."</font><font color='#9b9079'>获得:</font><font color='#a77602'><p>##"..itemName.."##</p></font>"
	for i=1,30 do
		if server.get_var("lottery_zhuanpan_"..i)~="" then
			if i==30 then
				for q=1,30 do
					if q==30 then
						server.set_var("lottery_zhuanpan_"..q,record)
					else
						server.set_var("lottery_zhuanpan_"..q,server.get_var("lottery_zhuanpan_"..q+1))
					end
				end
			end
		else
			server.set_var("lottery_zhuanpan_"..i,record)
			return
		end
	end
end

--充值送抽奖次数
function addLotteryTimes(player,add)
	local t1,t2 =gui.PanelJingCaiAct.getActivityOpenEndTime(2011)
	if get_day_YMDHM()>=t1 and get_day_YMDHM()<=t2 and gui.PanelJingCaiAct.checActivityOpen(2011) then
		local all = util.ppn(player,const.PP_ZHUANPAN_YVCION)+add
		local times = (all-all%2000)/2000
		if times>=1 then
			player:set_param(const.PP_ZHUANPAN_TIMES,util.ppn(player,const.PP_ZHUANPAN_TIMES)+times)
		end
		player:set_param(const.PP_ZHUANPAN_YVCION,all%2000)
		gui.moduleRedPoint.checkEventsUsable(player)
	end
end

--活动结束后清理转盘相关变量
function clearValue(player)
	player:set_param(const.PP_ZHUANPAN_TIMES,0)
	player:set_param(const.PP_ZHUANPAN_YVCION,0)
	if server.get_var("lottery_zhuanpan_1")~="" then
		for i=1,30 do
			server.set_var("lottery_zhuanpan_"..i,"")
		end
	end
end

--转盘活动时间开区前5天
function checkOpenState(player)
	if server_merge_day()>=0 and server_merge_day()<=4 then
		return true
	end
	return false
end

function checkZhuanPanRed(player)
	local result =false
	local t1,t2 =gui.PanelJingCaiAct.getActivityTime(2011)
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local time=(gui.PanelJingCaiAct.formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S
	if time>0 and gui.PanelJingCaiAct.checActivityOpen(2011) then
		local times = util.ppn(player,const.PP_ZHUANPAN_TIMES)
		if times>=1 then
			result = true
		end
	end
	player:push_lua_table("PanelJingCaiAct",util.encode({cmd="updateTabRed",name="tabName10",show=result}))
	return result
end


ckpanel.add_listener("gui.PanelZhuanPan.handlePanelData",gui.PanelZhuanPan.handlePanelData)

