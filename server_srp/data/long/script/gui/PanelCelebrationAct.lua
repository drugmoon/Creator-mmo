module(... ,package.seeall)

startYMD = 20180520
endYMD   = 20180620
allDay=0--活动开启的天数

local actType = 1  --actType:1(庆典活动)  actType:2(合区活动) --根据actType类型显示页签

--庆典活动页签
local tabTable = {
	{index=4, name="全服双倍", nameStr="tabName4", openDays=4, func=function(player) return true end},
	{index=3, name="登录有礼", nameStr="tabName3", openDays=4, func=function(player) return true end},
	{index=2, name="充值有礼", nameStr="tabName2", openDays=4, func=function(player) return true end},
	{index=1, name="超级瑞兽", nameStr="tabName1", openDays=2, func=function(player) return true end},
	{index=6, name="限时抢购", nameStr="tabName6", openDays=4, func=function(player) return true end},
	{index=7, name="消费豪礼", nameStr="tabName7", openDays=4, func=function(player) return true end},
	{index=5, name="全民探宝", nameStr="tabName5", openDays=4, func=function(player) return true end},
}

--合区活动页签
local tabTable2 = {
	{index=4, name="全服双倍", nameStr="tabName4", openDays=3, func=function(player) return true end},
	{index=2, name="充值有礼", nameStr="tabName2", openDays=3, func=function(player) return true end},
	{index=1, name="超级瑞兽", nameStr="tabName1", openDays=2, func=function(player) return true end},
	{index=6, name="限时抢购", nameStr="tabName6", openDays=2, func=function(player) return true end},
	{index=5, name="全民探宝", nameStr="tabName5", openDays=5, func=function(player) return true end},

	{index=8,  name="狼牙回馈", nameStr="tabName8",  openDays=7, func=function(player) return (server_merge_day()<=6 and server_merge_day()>=0 and actType==2) end},
	{index=9,  name="龙心回馈", nameStr="tabName9",  openDays=7, func=function(player) return (server_merge_day()<=6 and server_merge_day()>=0 and actType==2) end},
	{index=10, name="沙城争霸", nameStr="tabName10", openDays=7, func=function(player) return (server_merge_day()<=6 and server_merge_day()>=0 and actType==2) end},
	{index=11, name="神秘商店", nameStr="tabName11", openDays=3, func=function(player) return (server_merge_day()<=6 and server_merge_day()>=0 and actType==2) end},
}

       
function onPanelData(player, commond)
	-- print(commond)
	local data =util.decode(commond)
	if data.actionid == "reqTabList" then
		senderToClientTab(player)
	elseif data.actionid == "updateRsCount" then--跟新超级瑞兽倒计时
		updateRuiShouCount(player)
	elseif data.actionid=="reqRuiShowGo" then--瑞兽传送
		player:enter_map("biqi", 32, 86)
		player:push_lua_table("close",util.encode({name = "extend_activities"}))
	--充值有礼
	elseif data.actionid=="updateChongZhiData" then
		updateRechargeData(player)
	elseif data.actionid=="lingRechargeAwards" then
		getRechargeData(player)
	--登录有礼
	elseif data.actionid=="updateLoginData" then
		updateLoginData(player)
	elseif data.actionid=="lingLoginAwards" then
		getLoginData(player)
	--全服双倍
	elseif data.actionid=="updateDoubleCount" then
		updateDoubleExpCount(player)
	--全民探宝
	elseif data.actionid=="updateLotteryData" then
		updateLotteryData(player)
	--限时抢购
	elseif data.actionid=="updateQiangGouData" then
		updateQiangGouData(player)
	elseif data.actionid=="reqQiangGouGoods" then
		buyZheKouGoods(player,data.params.index)
	--限时抢购
	elseif data.actionid=="updateXiaoFeiData" then
		updateXiaoFeiData(player)
	--狼牙回馈
	elseif data.actionid=="updateLangYaData" then
		updateLangYaData(player)
	elseif data.actionid=="reqLangYaAwards" then
		getLangYaAwards(player,data.params.index)
	--龙心回馈
	elseif data.actionid=="updateLongXinData" then
		updateLongXinData(player)
	elseif data.actionid=="reqLongXinAwards" then
		getLongXinAwards(player,data.params.index)
	--沙城争霸
	elseif data.actionid=="updateSczbData" then
		updateWangChengData(player)
	elseif data.actionid=="reqSczbAwards" then
		getWangChengAwards(player,data.params.index)

	--刷新红点
	elseif data.actionid=="updateRedpoint" then
		gui.moduleRedPoint.checkRechargeYouLi(player)
		gui.moduleRedPoint.checkLoginYouLi(player)
	elseif data.actionid == "openCharge" then
		if checkActIng() then
			player:push_lua_table("open",util.encode({name = "panel_charge"}));
		else
			player:alert(1,1,"活动已结束！")
		end
	end
end

ckpanel.add_listener("gui.PanelCelebrationAct.onPanelData",gui.PanelCelebrationAct.onPanelData)

--推送页签数据
function senderToClientTab(player)
	local result = {
		cmd="updateQinDianTabs",
		tabTable=getNeedTabTable(player),
		actType=actType,
	}
	if #result.tabTable then
		player:push_lua_table("PanelCelebrationAct",util.encode(result))
	end
end

--处理页签列表
function getNeedTabTable(player)
	local result = {}
	local tempTable={}
	if actType==2 then
		tempTable=tabTable2
	else
		tempTable=tabTable
	end
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if itemData.func and itemData.func(player) then
			table.insert(result,{index=itemData.index,name=itemData.name,nameStr=itemData.nameStr})
		end
	end
	return result
end

--------------------------------------------------------------超级瑞兽------------------------------------------------------------------
function updateRuiShouCount(player)
	local result = {
		cmd="updateRuiShouCount",
		time=0,
	}
	if checkActIng() and actType==1 then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		-- result.time=(2-startDay)*24*3600+(24-H)*3600-M*60

		-- renum3sult.time=(2-startDay)*24*3600+(24-H)*3600-M*60
		local num1 = util.convertToTime({year = math.floor(startYMD/10000),month = math.floor(startYMD/100)%100,day = startYMD%100})
		local num2 = num1+2*86400
		local num3 = util.convertToTime({year = math.floor(get_day_YMD()/10000),month = math.floor(get_day_YMD()/100)%100,day = get_day_YMD()%100})
		local temp = num2/86400-num3/86400-1
		-- print(temp)
		result.time=(temp)*24*3600+(24-H)*3600-M*60-S
	end
	if (server_merge_day()<=2 and server_merge_day()>=0) and actType==2 then
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(2-server_merge_day()-1)*24*3600+(24-H)*3600-M*60-S
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))

end

--------------------------------------------------------------充值有礼------------------------------------------------------------------
local rechargeAwards = {
	{id = 23040001,	num = 1, bind = 1},
	{id = 24000001,	num = 2, bind = 1},
	{id = 24020003,	num = 5, bind = 1},
	{id = 32000002,	num = 1, bind = 1},
	{id = 23060003,	num = 3, bind = 1},
}

--合区活动充值奖励
local rechargeAwards2 = {
	{id = 11101011,	num = 1, bind = 1},
	{id = 11101011,	num = 2, bind = 1},
	{id = 11101011,	num = 1, bind = 1},
	{id = 11101011,	num = 1, bind = 1},
	{id = 11101011,	num = 1, bind = 1},
}

function updateRechargeData(player)
	local result = {
		cmd="updateRechargeData",
		ling=0,
		time=0,
		awards=util.clone(rechargeAwards)
	}
	if mergeTabOpenState("充值有礼") then
		result.awards=util.clone(rechargeAwards2)
	end
	if util.ppn(player,const.PP_DAILY_RECHARGE)>=5000 then
		result.ling=1--可领取
	end
	if util.ppn(player,const.PP_DAILY_RECHARGE_LING)>0 then
		result.ling=2--已领取
	end

	if checkActIng() or (server_merge_day()<=2 and server_merge_day()>=0) then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		-- result.time=(7-startDay)*24*3600+(24-H)*3600-M*60
		result.time=(24-H)*3600-M*60-S
	else
		-- result.ling=3--已领取
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))

end

function getRechargeData(player)
	local tempTable = util.clone(rechargeAwards)
	if mergeTabOpenState("充值有礼") then
		tempTable=util.clone(rechargeAwards2)
	end
	if util.ppn(player,const.PP_DAILY_RECHARGE)<5000 then
		player:alert(1,1,"今日充值不足5000元宝！")
		return
	end
	if util.ppn(player,const.PP_DAILY_RECHARGE_LING)>0 then
		player:alert(1,1,"已领取，不能重复领取！")
		return
	end
	if player:num_bag_black() < #tempTable then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#tempTable do
		local awardData = tempTable[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(const.PP_DAILY_RECHARGE_LING,1)
	player:alert(1,1,"成功领取充值有礼！")
	updateRechargeData(player)
	gui.moduleRedPoint.checkRechargeYouLi(player)
end

--------------------------------------------------------------登录有礼------------------------------------------------------------------
local loginAwards = {
	{id = 32000005,	num = 2, bind = 1},
	{id = 23020001,	num = 5, bind = 1},
	{id = 23060001,	num = 1, bind = 1},
	{id = 21000003,	num = 3, bind = 1},
	{id = 32000001,	num = 2, bind = 1},
}

function updateLoginData(player)
	local result = {
		cmd="updateLoginData",
		ling=util.ppn(player,const.PP_DAILY_LOGIN_LING),
		time=0,
		awards=util.clone(loginAwards)
	}
	if checkActIng()then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(formattime(get_day_YMD(),endYMD)-1)*24*3600+(24-H)*3600-M*60-S
	end

	player:push_lua_table("PanelCelebrationAct",util.encode(result))
end

function getLoginData(player)
	if util.ppn(player,const.PP_DAILY_LOGIN_LING)>0 then
		player:alert(1,1,"已领取，不能重复领取！")
		return
	end
	if player:num_bag_black() < #loginAwards then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#loginAwards do
		local awardData = loginAwards[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(const.PP_DAILY_LOGIN_LING,1)
	player:alert(1,1,"成功领取登录有礼！")
	updateLoginData(player)
	gui.moduleRedPoint.checkLoginYouLi(player)
end

--------------------------------------------------------------全服双倍------------------------------------------------------------------
function updateDoubleExpCount(player)
	local result = {
		cmd="updateDoubleExpCount",
		time=0,
	}
	if checkActIng()then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		-- result.time=(7-startDay)*24*3600+(24-H)*3600-M*60
		result.time=(formattime(get_day_YMD(),endYMD)-1)*24*3600+(24-H)*3600-M*60-S
		if actType==2 then
			result.time=(3-server_merge_day()-1)*24*3600+(24-H)*3600-M*60-S
		end
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))

end

--------------------------------------------------------------全民探宝------------------------------------------------------------------
local showEquips = {
	["warrior"]={
		{id = 11240011,	num = 1, bind = 1},
		{id = 11240012,	num = 1, bind = 1},
		{id = 11240013,	num = 1, bind = 1},
		{id = 11240014,	num = 1, bind = 1},
	},
	["wizard"]={
		{id = 11240011,	num = 1, bind = 1},
		{id = 11240012,	num = 1, bind = 1},
		{id = 11240013,	num = 1, bind = 1},
		{id = 11240014,	num = 1, bind = 1},
	},
	["taoist"]={
		{id = 11240011,	num = 1, bind = 1},
		{id = 11240012,	num = 1, bind = 1},
		{id = 11240013,	num = 1, bind = 1},
		{id = 11240014,	num = 1, bind = 1},
	},
}
function updateLotteryData(player)
	local job = player:get_job_name()
	local result = {
		cmd="updateLotteryData",
		time=0,
		equips=util.clone(showEquips[job])
	}
	if checkActIng()then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		-- result.time=(7-startDay)*24*3600+(24-H)*3600-M*60
		result.time=(formattime(get_day_YMD(),endYMD)-1)*24*3600+(24-H)*3600-M*60-S
		if actType==2 then
			result.time=(5-server_merge_day()-1)*24*3600+(24-H)*3600-M*60-S
		end
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))
end

--------------------------------------------------------------限时抢购------------------------------------------------------------------
local zheKouTable = {
	[1]={
		{id=23040002, name="超级转生丹", moneyType=102, pricey=9888, price=7999, num=1,   bind=1, allnum=3, svr="svr_curbuynum1", con=const.PP_QIANGGOU_BUYNUM1},
		{id=23040001, name="高级转生丹", moneyType=102, pricey=1000, price=799,  num=1,   bind=1, allnum=3, svr="svr_curbuynum2", con=const.PP_QIANGGOU_BUYNUM2},
		{id=24030001, name="羽毛",       moneyType=102, pricey=6000, price=4800, num=200, bind=1, allnum=3, svr="svr_curbuynum3", con=const.PP_QIANGGOU_BUYNUM3},
	},
	[2]={
		{id=23020003, name="龙心碎片（大）", moneyType=102, pricey=9888, price=7999, num=1,  bind=1, allnum=3, svr="svr_curbuynum1", con=const.PP_QIANGGOU_BUYNUM1},
		{id=23020002, name="龙心碎片（中）", moneyType=102, pricey=1000, price=799,  num=1,  bind=1, allnum=3, svr="svr_curbuynum2", con=const.PP_QIANGGOU_BUYNUM2},
		{id=23050003, name="声望卷（大）",   moneyType=102, pricey=4880, price=3900, num=10, bind=1, allnum=3, svr="svr_curbuynum3", con=const.PP_QIANGGOU_BUYNUM3},
	},
	[3]={
		{id=23030003, name="狼牙碎片（大）", moneyType=102, pricey=9888, price=7999, num=1,   bind=1, allnum=3, svr="svr_curbuynum1", con=const.PP_QIANGGOU_BUYNUM1},
		{id=23030002, name="狼牙碎片（中）", moneyType=102, pricey=1000, price=799, num=1,   bind=1, allnum=3, svr="svr_curbuynum2", con=const.PP_QIANGGOU_BUYNUM2},
		{id=24030001, name="羽毛",           moneyType=102, pricey=6000, price=4800, num=200, bind=1, allnum=3, svr="svr_curbuynum3", con=const.PP_QIANGGOU_BUYNUM3},
	},
	[4]={
		{id=23140003, name="高级成就令牌", moneyType=102, pricey=10000, price=7999, num=20, bind=1, allnum=3, svr="svr_curbuynum1", con=const.PP_QIANGGOU_BUYNUM1},
		{id=23140002, name="中级成就令牌", moneyType=102, pricey=1125,  price=799,  num=6,  bind=1, allnum=3, svr="svr_curbuynum2", con=const.PP_QIANGGOU_BUYNUM2},
		{id=23050003, name="声望卷（大）", moneyType=102, pricey=4880,  price=3900, num=10, bind=1, allnum=3, svr="svr_curbuynum3", con=const.PP_QIANGGOU_BUYNUM3},
	},
}

--合区活动-限时抢购
local zheKouTable2 = {
	[1]={
		{id=24030001, name="羽毛",           moneyType=103, pricey=11400, price=8999, num=380, bind=1, allnum=3, svr="svr_curbuynum1", con=const.PP_QIANGGOU_BUYNUM1},
		{id=23020002, name="龙心碎片（中）", moneyType=103, pricey=11000, price=8888, num=11,  bind=1, allnum=3, svr="svr_curbuynum2", con=const.PP_QIANGGOU_BUYNUM2},
		{id=23030002, name="狼牙碎片（中）", moneyType=103, pricey=13000, price=9999, num=13,  bind=1, allnum=3, svr="svr_curbuynum3", con=const.PP_QIANGGOU_BUYNUM3},
	}
}

function updateQiangGouData(player)
	local result = {
		cmd="updateQiangGouData",
		time=0,
		goods=util.clone(zheKouTable[formattime(startYMD,get_day_YMD())])
	}
	if checkActIng() or (server_merge_day()<=1 and server_merge_day()>=0) then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		-- result.time=(7-startDay)*24*3600+(24-H)*3600-M*60
		result.time=(24-H)*3600-M*60-S
	end
	if not result.goods or #result.goods<=0 then
		result.goods=util.clone(zheKouTable[1])
	end
	if actType==2 then
		result.goods=util.clone(zheKouTable2[1])
	end
	for i=1,#result.goods do
		local itemData = result.goods[i]
		-- itemData.yuNum=itemData.allnum-util.svn(itemData.svr)
		itemData.yuNum=itemData.allnum-util.ppn(player, itemData.con)
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))
end

function buyZheKouGoods(player,index)
	if not checkActIng() then return player:alert(1,1,"活动已结束！") end

	local tempTable = util.clone(zheKouTable[formattime(startYMD,get_day_YMD())])
	if not tempTable or #tempTable<=0 then
		tempTable=util.clone(zheKouTable[1])
	end

	if actType==2 then
		tempTable=util.clone(zheKouTable2[1])
	end

	local itemData = tempTable[index]
	if not itemData then return end
	if (itemData.allnum-util.ppn(player, itemData.con))<=0 then
		player:alert(1,1,"该物品已售罄！")
		return
	end
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	if itemData.moneyType==102 and player:get_vcoin()<itemData.price then
		showChongzhi(player)
		return
	else
		player:sub_vcoin(itemData.price,"限时抢购")
	end
	if itemData.moneyType==103 and player:get_vcoin_bind()<itemData.price then
		player:alert(1,1,"绑定元宝不足！")
		return
	else
		player:sub_vcoin_bind(itemData.price,"限时抢购")
	end

	player:add_item(itemData.id,itemData.num,itemData.bind)
	-- local num = util.svn(itemData.svr)+itemData.num
	-- server.set_var(itemData.svr,num)
	player:set_param(itemData.con,util.ppn(player, itemData.con)+1)
	player:alert(1,1,"成功购买"..itemData.name)
	updateQiangGouData(player)
end

--------------------------------------------------------------消费有礼------------------------------------------------------------------
local rankAwards = {
	[1]={
		svr="qingdian_rank_name_1",
		awards={
			{id = 27050001,	num = 1,   bind = 0},
			{id = 24030001,	num = 500, bind = 1},
			{id = 23140003,	num = 20,  bind = 1},
		}
	},
	[2]={
		svr="qingdian_rank_name_2",
		awards={
			{id = 27020006,	num = 1,   bind = 0},
			{id = 24030001,	num = 300, bind = 1},
			{id = 23140003,	num = 10,  bind = 1},
		}
	},
	[3]={
		svr="qingdian_rank_name_3",
		awards={
			{id = 27020005,	num = 1,   bind = 0},
			{id = 24030001,	num = 200, bind = 1},
			{id = 23140003,	num = 5,   bind = 1},
		}
	},
}

function updateXiaoFeiData(player)
	local result = {
		cmd="updateXiaoFeiData",
		time=0,
		myRank=0,
		curXiaoFei=util.ppn(player, const.PP_XIAOFEI_RANK),
		awards=util.clone(rankAwards)
	}
	if checkActIng()then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		-- result.time=(7-startDay)*24*3600+(24-H)*3600-M*60
		result.time=(formattime(get_day_YMD(),endYMD)-1)*24*3600+(24-H)*3600-M*60-S
	end
	for i=1,#result.awards do
		local itemData = result.awards[i]
		itemData.name=server.get_var(itemData.svr)
		
		if itemData.name==player:get_name() then
			result.myRank=i
		end
		-- print(itemData.name,"==============",result.myRank)
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))
end

--计算狼牙排名前五的
function setXiaoFeiRank(player)
	local curXiaoFei = util.ppn(player, const.PP_XIAOFEI_RANK)
	if curXiaoFei<300000 then return end--最低30万上榜
	local rankData={
		{name=server.get_var("qingdian_rank_name_1"),xfNum=util.svn("qingdian_rank_xf_1")},--1
		{name=server.get_var("qingdian_rank_name_2"),xfNum=util.svn("qingdian_rank_xf_2")},--2
		{name=server.get_var("qingdian_rank_name_3"),xfNum=util.svn("qingdian_rank_xf_3")},--3
		{name=server.get_var("qingdian_rank_name_4"),xfNum=util.svn("qingdian_rank_xf_4")},--4
		{name=server.get_var("qingdian_rank_name_5"),xfNum=util.svn("qingdian_rank_xf_5")},--5
	}
	local tempTable = util.clone(rankData)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if itemData.name==player:get_name() then
			table.remove(tempTable,i)
			break
		end
	end
	for j=1,5 do
		if not tempTable[j] or curXiaoFei>tempTable[j].xfNum then
			table.insert(tempTable,j,{name=player:get_name(),xfNum=curXiaoFei})
			break
		end	 
	end
	for n=1,5 do
		if tempTable[n] then
			server.set_var("qingdian_rank_name_"..n,tempTable[n].name)
			server.set_var("qingdian_rank_xf_"..n,tempTable[n].xfNum)
		else
			server.set_var("qingdian_rank_name_"..n,"")
			server.set_var("qingdian_rank_xf_"..n,0)
		end
	end
end

function sendRankAwards(player)
	-- local serverDay = server_start_day()+1
	local myName = player:get_name()
	--消费有礼
	if formattime(startYMD,get_day_YMD())==(allDay+1) then
		for i=1,#rankAwards do
			local itemData = rankAwards[i]
			local name =server.get_var(itemData.svr)
			if name==myName then
				util.sendMail(player,"消费有礼", "您在消费有礼中名列前茅\n获得排名第"..i.."名奖励", itemData.awards)
			end
		end
		-- player:set_param(const.PP_RANK_MAIL_AWARD1,1)
	end
end

--------------------------------------------------------------狼牙回馈------------------------------------------------------------------
local lyTable={
	[1]={id=23030003, name="狼牙碎片（大）", useNum=1,  con=const.PP_LANGYA_LING1, awards={{id=23030002, num=1,  bind=1}} },
	[2]={id=23030003, name="狼牙碎片（大）", useNum=3,  con=const.PP_LANGYA_LING2, awards={{id=23030002, num=4,  bind=1}} },
	[3]={id=23030003, name="狼牙碎片（大）", useNum=5,  con=const.PP_LANGYA_LING3, awards={{id=23030002, num=8,  bind=1}} },
	[4]={id=23030003, name="狼牙碎片（大）", useNum=10, con=const.PP_LANGYA_LING4, awards={{id=23030002, num=18, bind=1}} },
	[5]={id=23030003, name="狼牙碎片（大）", useNum=20, con=const.PP_LANGYA_LING5, awards={{id=23030003, num=5,  bind=1}} },
}

function updateLangYaData(player)
	local result = {
		cmd="updateLangYaData",
		time=0,
		allUse=util.ppn(player,const.PP_LANGYA_USEB_NUM),
		dataTable=util.clone(lyTable)
	}
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local mergeDay = server_merge_day()+1
	result.time=(allDay-mergeDay)*24*3600+(24-H)*3600-M*60-S
	-- print(result.time,allDay,mergeDay,server_merge_day())
	for i=1,#result.dataTable do
		local itemData = result.dataTable[i]
		itemData.ling = util.ppn(player,itemData.con)
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))
end

function getLangYaAwards(player,index)
	local itemData = lyTable[index]
	local allnum = util.ppn(player,const.PP_LANGYA_USEB_NUM)
	if not itemData then return end
	if util.ppn(player,itemData.con)>0 then
		player:alert(1,1,"已领取，不能重复领取！")
		return
	end
	if allnum<itemData.useNum then
		player:alert(1,1,"狼牙碎片（大）使用次数未达指定数量！")
		return
	end
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#itemData.awards do
		local awardData = itemData.awards[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(itemData.con,1)
	player:alert(1,1,"成功领取狼牙回馈！")
	updateLangYaData(player)
	-- gui.moduleRedPoint.checkLoginYouLi(player)
end

--------------------------------------------------------------龙心回馈------------------------------------------------------------------
local lxTable={
	[1]={id=23020003, name="龙心碎片（大）", useNum=1,  con=const.PP_LONGXIN_LING1, awards={{id=23020002, num=1,  bind=1}} },
	[2]={id=23020003, name="龙心碎片（大）", useNum=3,  con=const.PP_LONGXIN_LING2, awards={{id=23020002, num=4,  bind=1}} },
	[3]={id=23020003, name="龙心碎片（大）", useNum=5,  con=const.PP_LONGXIN_LING3, awards={{id=23020002, num=8,  bind=1}} },
	[4]={id=23020003, name="龙心碎片（大）", useNum=10, con=const.PP_LONGXIN_LING4, awards={{id=23020002, num=18, bind=1}} },
	[5]={id=23020003, name="龙心碎片（大）", useNum=20, con=const.PP_LONGXIN_LING5, awards={{id=23020003, num=5,  bind=1}} },
}

function updateLongXinData(player)
	local result = {
		cmd="updateLongXinData",
		time=0,
		allUse=util.ppn(player,const.PP_LONGXIN_USEB_NUM),
		dataTable=util.clone(lxTable)
	}
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local mergeDay = server_merge_day()+1
	result.time=(allDay-mergeDay)*24*3600+(24-H)*3600-M*60-S
	for i=1,#result.dataTable do
		local itemData = result.dataTable[i]
		itemData.ling = util.ppn(player,itemData.con)
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))
end

function getLongXinAwards(player,index)
	local itemData = lxTable[index]
	local allnum = util.ppn(player,const.PP_LONGXIN_USEB_NUM)
	if not itemData then return end
	if util.ppn(player,itemData.con)>0 then
		player:alert(1,1,"已领取，不能重复领取！")
		return
	end
	if allnum<itemData.useNum then
		player:alert(1,1,"龙心碎片（大）使用次数未达指定数量！")
		return
	end
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#itemData.awards do
		local awardData = itemData.awards[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(itemData.con,1)
	player:alert(1,1,"成功领取龙心回馈！")
	updateLongXinData(player)
	-- gui.moduleRedPoint.checkLoginYouLi(player)
end

--------------------------------------------------------------沙城争霸------------------------------------------------------------------
local cityTable={
	[1]={svr="svr_sczb_ling1", mergeDay=3, awards={{id=27040004, num=1, bind=1},{id=40000005, num=10000, bind=0}} },
	[2]={svr="svr_sczb_ling2", mergeDay=5, awards={{id=27040005, num=1, bind=1},{id=40000005, num=10000, bind=0}} },
	[3]={svr="svr_sczb_ling3", mergeDay=7, awards={{id=27040005, num=1, bind=1},{id=40000005, num=10000, bind=0}} },
}

function updateWangChengData(player)
	local result = {
		cmd="updateWangChengData",
		time=0,
		dataTable=util.clone(cityTable)
	}
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local mergeDay = server_merge_day()+1
	result.time=(allDay-mergeDay)*24*3600+(24-H)*3600-M*60-S
	for i=1,#result.dataTable do
		local itemData = result.dataTable[i]
		itemData.ling = util.svn(itemData.svr)
	end
	player:push_lua_table("PanelCelebrationAct",util.encode(result))
end

function getWangChengAwards(player,index)
	local itemData = cityTable[index]
	if not itemData then return end
	local mergeDay = server_merge_day()+1
	local myName = player:get_name()
	if mergeDay~=itemData.mergeDay then
		player:alert(1,1,"合区第"..itemData.mergeDay.."天沙城争霸结束方可领取！")
		return
	end
	if npc.huangcheng.isWarClose() then
		player:alert(1,1,"今日沙城争霸结束才能领取！")
		return
	end
	if util.svn(itemData.svr)>0 then
		player:alert(1,1,"已领取，不能重复领取！")
		return
	end
	local candidate = server.get_king_guild()
	local leaderOfKing = ""
	if candidate and candidate ~= ""  then
		local kingGuild = server.find_guild(candidate)
		if kingGuild then
			leaderOfKing = kingGuild:get_leader_name()
		end
	end
	if myName~=leaderOfKing then
		player:alert(1,1,"您不是王城会长无法领取！")
		return
	end
	for i=1,#itemData.awards do
		local awardData = itemData.awards[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	server.set_var(itemData.svr,1)
	player:alert(1,1,"成功沙城争霸奖励！")
	updateWangChengData(player)
end

-------------------------------------------------------检测是否在活动时间内-------------------------------------------------
function formattime(startTime,endTime)
	local num1 = util.convertToTime({year = math.floor(startTime/10000),month = math.floor(startTime/100)%100,day = startTime%100})
	local num2 = util.convertToTime({year = math.floor(endTime/10000),month = math.floor(endTime/100)%100,day = endTime%100})
	local dayNum = 1+(num2-num1)/86400
	return dayNum
end

allDay=formattime(startYMD,endYMD)
if actType==2 then allDay=7 end

--庆典活动期间
function checkActIng()
	if actType==1 and get_day_YMD()>=startYMD and get_day_YMD()<=endYMD then 
		return true
	end
	return false
end
--合服活动期间

--检测合服活动--页签是否开启
function mergeTabOpenState(tabName)
	if server_merge_day()<0 or actType~=2 then return false end
	local mergeDay = server_merge_day()+1
	if tabName=="全服双倍" then
		if mergeDay<=3 then return true end
	end
	if tabName=="充值有礼" then
		if mergeDay<=3 then return true end
	end
	if tabName=="超级瑞兽" then
		if mergeDay<=2 then return true end
	end
	if tabName=="限时抢购" then
		if mergeDay<=2 then return true end
	end
	if tabName=="全民探宝" then
		if mergeDay<=5 then return true end
	end
	if tabName=="神秘商店" then
		if mergeDay<=3 then return true end
	end
	return false
end


-------------------------------------------------------红点检测--------------------------------------------------------------
--充值有礼红点检测
function checkRedPointRecharge(player)
	if util.ppn(player,const.PP_DAILY_RECHARGE)>=5000 and util.ppn(player,const.PP_DAILY_RECHARGE_LING)<=0 then
		return true
	end
	return false
end

--登录有礼红点检测
function checkRedPointLogin(player)
	if util.ppn(player,const.PP_DAILY_LOGIN_LING)<=0 then
		return true
	end
	return false
end





