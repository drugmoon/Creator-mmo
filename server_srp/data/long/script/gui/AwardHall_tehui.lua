module(... ,package.seeall)

--特惠礼包配置
local giftTable = {
	[1]={
		name="神装礼包",vcion=588,con=const.PP_HALL_TEHUI_BUY1,desp="装备任务秒过！",
		award={
			[1]={id=0,num=1,bind=0,effectRes = 0},
			[2]={id=0,num=1,bind=0,effectRes = 0},
			[3]={id=0,num=1,bind=0,effectRes = 0},
			[4]={id=0,num=2,bind=0,effectRes = 0},
			[5]={id=0,num=2,bind=0,effectRes = 0},
			[6]={id=0,num=1,bind=0,effectRes = 0},
			[7]={id=0,num=1,bind=0,effectRes = 0},
		}
	},
	[2]={
		name="神翼礼包",vcion=2888,con=const.PP_HALL_TEHUI_BUY2,desp="翅膀秒升2阶！",
		award={
			[1]={id=24050001,num=1,bind=1,effectRes = 65078},
			[2]={id=23140003,num=1,bind=1,effectRes = 65078},
			[3]={id=23150003,num=5,bind=1,effectRes = 65078},
			[4]={id=32000006,num=4,bind=1,effectRes = 65078},
			[5]={id=23080004,num=1,bind=1,effectRes = 0},
		}
	},
	[3]={
		name="至尊礼包",vcion=8888,con=const.PP_HALL_TEHUI_BUY3,desp="转生等级提升无忧！",
		award={
			[1]={id=23060004,num=1, bind=1,effectRes = 65080},
			[2]={id=24000001,num=20,bind=1,effectRes = 65078},
			[3]={id=23090002,num=10,bind=1,effectRes = 0},
			[4]={id=32000006,num=8, bind=1,effectRes = 65078},
			[5]={id=23080008,num=1, bind=1,effectRes = 0},
		}
	},
	[4]={
		name="主宰礼包",vcion=16888,con=const.PP_HALL_TEHUI_BUY4,desp="极品转生武器祝其傲视群雄！",
		award={
			[1]={id=23020003,num=1, bind=1,effectRes = 65078},
			[2]={id=23030002,num=5, bind=1,effectRes = 0},
			[3]={id=32000006,num=16,bind=1,effectRes = 65078},
			[4]={id=23080012,num=1, bind=1,effectRes = 65078},
			[5]={id=0,       num=1, bind=0,effectRes = 65079},
		}
	},
}

--神装礼拜给装备的部分
local shenEquips = {
	[200]={
		["warrior"]={11107021,11107031,11107041,11107051,11107061,11107071,11107081},
		["wizard"] ={12107021,12107031,12107041,12107051,12107061,12107071,12107081},
		["taoist"] ={13107021,13107031,13107041,13107051,13107061,13107071,13107081},
	},
	[201]={
		["warrior"]={11107091,11107031,11107041,11107051,11107061,11107071,11107081},
		["wizard"] ={12107091,12107031,12107041,12107051,12107061,12107071,12107081},
		["taoist"] ={13107091,13107031,13107041,13107051,13107061,13107071,13107081},
	},
}
local shenEquipsRes = {
	[200]={
		["warrior"]={65078,65078,65078,65078,65078,65078,65078},
		["wizard"] ={65078,65078,65078,65078,65078,65078,65078},
		["taoist"] ={65078,65078,65078,65078,65078,65078,65078},
	},
	[201]={
		["warrior"]={65078,65078,65078,65078,65078,65078,65078},
		["wizard"] ={65078,65078,65078,65078,65078,65078,65078},
		["taoist"] ={65078,65078,65078,65078,65078,65078,65078},
	},
}
--主宰礼拜给装备的部分
local zhuEquips = {
	[200]={
		["warrior"]={11111011,65079},
		["wizard"] ={12111011,65079},
		["taoist"] ={13111011,65079},
	},
	[201]={
		["warrior"]={11111011,65079},
		["wizard"] ={12111011,65079},
		["taoist"] ={13111011,65079},
	},

}

function handlePanelData(player,commond)
	-- player:set_param(const.PP_HALL_TEHUI_BUY1,0)
	-- player:set_param(const.PP_HALL_TEHUI_BUY2,0)
	-- player:set_param(const.PP_HALL_TEHUI_BUY3,0)
	-- player:set_param(const.PP_HALL_TEHUI_BUY4,0)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "reqTeHuiData" then
		senderToClient(player,data.params.index)
	elseif data.actionid == "buyGiftBag" then
		buyGiftBag(player,data.params.index)
	elseif data.actionid == "" then

	end
end

--推送在线奖励数据
function senderToClient(player,index)
	local needTable = getNeedTable(player)
	local result = {
		cmd="updateTeHuiData",
		dataTable=needTable[index],
	}
	result.dataTable.buy=util.ppn(player,result.dataTable.con)--1:已购买 0:未购买
	player:push_lua_table("PanelRewardHall",util.encode(result))

	if util.ppn(player, const.PP_TEHUI_OPEN_NUM)<=0 then
		player:set_param(const.PP_TEHUI_OPEN_NUM,1)
		gui.moduleRedPoint.checkOpenSvrSaleUsable(player)
	end
end

--购买礼包
function buyGiftBag(player,index)
	local result = getNeedTable(player)
	local itemData = result[index]
	if not itemData then return end
	if util.ppn(player,itemData.con)==1 then
		player:alert(1,1,"该礼包已购买,不能重复购买！")
		return
	end
	if player:num_bag_black() < #itemData.award then
		player:alert(1,1,"背包空间不足！")
		return
	end
	if player:get_vcoin()<itemData.vcion then
		showChongzhi(player)
		return
	end
	for i=1,#itemData.award do
		local awardData = itemData.award[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:sub_vcoin(itemData.vcion,itemData.name)

	player:set_param(itemData.con,1)
	player:alert(1,1,"成功购买 "..itemData.name.."！")
	senderToClient(player,index)
	server.info(10000,0,"恭喜<font color='#00ccff'>"..player:get_name().."</font>成功购买了<font color='#ff0000'>"..itemData.name.."</font>"..itemData.desp)
	server.info(10, 1, "<font color='#fff843'>玩家<font color='#00ccff'>"..player:get_name().."</font>成功领取<font color='#ff0000'>"..itemData.name.."</font>"..itemData.desp.."<a color='#00ff00' href='event:local_SeverOpen_"..index.."' underline='#00ff00'>我要领取</a></font>")
	-- if not canBuy(player) then
	-- 	gui.PanelActivities.senderToClientTab(player)
	-- end
end

--处理表
function getNeedTable(player)
	local result = util.clone(giftTable)
	local job = player:get_job_name()
	local gender = player:get_gender()
	for i=1,#result[1].award do
		local itemData = result[1].award[i]
		itemData.id=shenEquips[gender][job][i]
		itemData.effectRes=shenEquipsRes[gender][job][i]
	end
	result[4].award[5].id=zhuEquips[gender][job][1]
	result[4].award[5].effectRes=zhuEquips[gender][job][2]
	return result
end


ckpanel.add_listener("gui.AwardHall_tehui.handlePanelData",gui.AwardHall_tehui.handlePanelData)

--检测特惠礼包红点
function checkRedPoint(player)
	local buyNum=0
	for i=1,#giftTable do
		local itemData = giftTable[i]
		if util.ppn(player,itemData.con)==0 then
			buyNum=1
			break
		end
	end
	if buyNum>0 and util.ppn(player, const.PP_TEHUI_OPEN_NUM)<=0 then
		return true
	end
	return false
end


--判断特惠礼包有无可买的
function canBuy(player)
	for i=1,#giftTable do
		local itemData = giftTable[i]
		if util.ppn(player,itemData.con)==0 then
			return true
		end
	end
	return false
end

--返回神装礼包有无领取
function checkShenZhuang(player)
	if util.ppn(player,const.PP_HALL_TEHUI_BUY1)==0 and gui.moduleGuiButton.checkFuncOpened(player, 20031) then
		return true
	end
	return false
end





