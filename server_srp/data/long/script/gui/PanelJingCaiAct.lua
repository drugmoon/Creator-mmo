module(... ,package.seeall)
---精彩活动图标显示
--bargainActivity.csv文件配置开启活动相关配置。
--bargain_rechargeRank.csv：配置充值排名的物品道具数量和id，是否绑定
--bargain_recharge.csv: 配置累计充值的排行榜奖励道具和id
--bargain_limitBuy.csv: 限时抢购配置
--bargain_wolfang.csv: 狼牙碎片配置表
--bargain_dragonHeart.csv:  龙心碎片配置表

function checkJingCaiActOpen()
	--if get_day_YMD()>=jingcaiact_start and get_day_YMD()<=jingcaiact_end then   
	-- else
	-- if string.find(server.get_server_title(),"中手游") then return false end 
	return  get_day_YMD()>=util.svn("JingCaiOpen")
	-- end
end
----活动开启时间
if util.svn("JingCaiOpen") ==0  then
	-- if not string.find(server.get_server_title(),"中手游") then  
		server.set_var("JingCaiOpen",get_day_YMD())
	-- end 
end

function checActivityOpen(id)
	--print(os.date("%H%M", os.time()))
	--if os.date("%H%M", os.time())=="0001" then  return 

	local week=math.floor((today()+3)/7)
	for k,v in pairs(tab.eventsData) do
		if k==id then 
			if v.open==1 then return false end
			if  v.openweek>0 then 
				if week%2==0 and v.openweek==2 then --双周开
					return true
				elseif week%2==1 and v.openweek==1 then --单周开
					return true
				end
			else
				return true
			end
			
		end 
	end
end
-------------------------------------------------------检测是否在活动时间内-------------------------------------------------
function formattime(startTime,endTime)
	local num1 = util.convertToTime({year = math.floor(startTime/10000),month = math.floor(startTime/100)%100,day = startTime%100})
	local num2 = util.convertToTime({year = math.floor(endTime/10000),month = math.floor(endTime/100)%100,day = endTime%100})
	local dayNum = 1+(num2-num1)/86400
	return dayNum
end
--20170527,7,20170603
function getAddTime(time,addday)
	local year = math.floor(time/10000)
	local month = math.floor(time%10000/100)
	local day = time%100
	local t = util.convertToTime({year=year,month=month,day = day})
	if not t then t=0 end
	t = t+ addday*86400
	return tonumber(os.date("%Y%m%d",t))
end
function getWeekTime(tnum)
	local year = math.floor(tnum/10000)
	local month = math.floor(tnum%10000/100)
	local day = tnum%100
	local t = util.convertToTime({year=year,month=month,day=day})
	local weekDay = os.date("%w",t)
	return weekDay
end

function getActivityTime(id)
	local  endtime= 0
	--if tab.eventsData[id].openweek==0 then 
	--	return util.svn("JingCaiOpen"),getAddTime(util.svn("JingCaiOpen"),6)
	--else  --单双周
		if util.svn("JingCaiOpen") ==0 then --首次
			endtime= 7-getWeekTime(util.svn("JingCaiOpen"))
			if endtime==7 then endtime=0 end
			return util.svn("JingCaiOpen"), getAddTime(util.svn("JingCaiOpen"),endtime)
			--return tonumber(server.get_var("JingCaiOpen").."0001"),tonumber(getAddTime(util.svn("JingCaiOpen"),endtime).."0001")
		else
			endtime= 7-getWeekTime(util.svn("JingCaiOpen"))
			if endtime==7 then endtime=0 end
			--return tonumber(server.get_var("JingCaiOpen").."0001"),tonumber(getAddTime(util.svn("JingCaiOpen"),endtime).."0001")
			return util.svn("JingCaiOpen"), getAddTime(util.svn("JingCaiOpen"),endtime)
		end
		---print("===",endtime,util.svn("JingCaiOpen"),util.svn("JingCaiOpen"))
	--end
end

function getActivityOpenEndTime(id)
	local  endtime= 0
	if tab.eventsData[id].openweek==0 then 
		return tonumber(server.get_var("JingCaiOpen").."0001"),tonumber(getAddTime(util.svn("JingCaiOpen"),7).."0001")
	else  --单双周
		if util.svn("JingCaiOpen") ==0 then --首次
			endtime= 7-getWeekTime(util.svn("JingCaiOpen"))
			if endtime==7 then endtime=0 end
			--return server.get_var("JingCaiOpen"), getAddTime(util.svn("JingCaiOpen"),endtime)
			return tonumber(server.get_var("JingCaiOpen").."0001"),tonumber(getAddTime(util.svn("JingCaiOpen"),endtime+1).."0001")
		else
			endtime= 7-getWeekTime(util.svn("JingCaiOpen"))
			if endtime==7 then endtime=0 end
			return tonumber(server.get_var("JingCaiOpen").."0001"),tonumber(getAddTime(util.svn("JingCaiOpen"),endtime+1).."0001")
		end
	end
end
----每天0点 执行 
function set_JingCaiVar()
	-- --if get_day_YMD()-util.svn("JingCaiOpen")>=7 then 
	-- local  time=formattime(util.svn("JingCaiOpen"),get_day_YMD())
	-- print(get_day_YMD(),util.svn("JingCaiOpen"),time)
	-- if  time >7 then 
	-- if not string.find(server.get_server_title(),"中手游") then  

		if  weekDay()==1 then --星期一零点重置数据,重置精彩活动。
			if get_day_YMD() ~= util.svn("JingCaiOpen") then 
				server.set_var("JingCaiOpen",get_day_YMD())
				sendRankAwards()  --发送排行榜奖励
				for n=1,5 do
					server.set_var("jingcai_rank_name_"..n,"")
					server.set_var("jingcai_rank_seedname_"..n,"")
					server.set_var("jingcai_rank_cz_"..n,0)
				end
			end
		end
	-- end
end
function clear_param( player ) --活动结束需要清理下数据
	if util.svn("JingCaiOpen")>util.ppn(player,const.PP_TODAY_JINGCAIACT)   then
		player:set_param(const.PP_LANGYA_GOT_NUM,0)---活动期间活动狼牙的数目
		player:set_param(const.PP_LONGXIN_GOT_NUM,0)--活动期间活动龙心的数目 
		for n=0,9 do
			player:set_param(const.PP_LANGYA_JINGCAI_LING1+n,0)
		end
		for n=0,9 do
			player:set_param(const.PP_LONGXIN_JINGCAI_LING1+n,0)
		end
		for n=0,5 do
			player:set_param(const.PP_JINGCAI_CZ1+n,0)
		end
		player:set_param(const.PP_RECHARGE_JINGCAI,0)
		--cz排行
		player:set_param(const.PP_CZRANK_JINGCAI_NUM,0)
		
		gui.PanelZhuanPan.clearValue(player)
	end
	---每天清理限时抢购
	player:set_param(const.PP_QIANGGOU_JINGCAI1,0)
	player:set_param(const.PP_QIANGGOU_JINGCAI2,0)
	player:set_param(const.PP_QIANGGOU_JINGCAI3,0)
end

local tabTable = {
	{index=4, name="全服双倍", nameStr="tabName4",  openDays=4, func=function(player) return checActivityOpen(1001) end},
	{index=9, name="龙心回馈", nameStr="tabName9",  openDays=5, func=function(player) return checActivityOpen(4002) end},
	{index=8, name="狼牙回馈", nameStr="tabName8",  openDays=5, func=function(player) return checActivityOpen(4003) end},
	{index=2, name="累计充值", nameStr="tabName2",  openDays=4, func=function(player) return checActivityOpen(4001) end},
	{index=7, name="充值排行", nameStr="tabName7", 	openDays=4, func=function(player) return checActivityOpen(4005) end}, 
	{index=6, name="限时抢购", nameStr="tabName6",  openDays=5, func=function(player) return checActivityOpen(4004) end},
	{index=5, name="全民探宝", nameStr="tabName5",  openDays=5, func=function(player) return checActivityOpen(1006) end},
	{index=10,name="幸运转盘", nameStr="tabName10", openDays=0, func=function(player) return checActivityOpen(2011) end}, 
	--{index=1, name="超级瑞兽", nameStr="tabName1", openDays=2, func=function(player) return false end},
	--{index=3, name="累计消费", nameStr="tabName3", openDays=4, func=function(player) return false end},
	--{index=6, name="极速充值", nameStr="tabName6", openDays=4, func=function(player) return false end},
}


----------------checkActOpen-------------------------
function checkljxf( player,num ) --累计消费
	return false
end

function checkljcz( player,num )  --累计充值
	local t1,t2 =getActivityOpenEndTime(4001)
	if get_day_YMDHM()>=t1 and get_day_YMDHM()<=t2 and checActivityOpen(4001) then 
		player:set_param(const.PP_RECHARGE_JINGCAI,util.ppn(player, const.PP_RECHARGE_JINGCAI)+num)
		checkRedPointRecharge(player)
		gui.moduleRedPoint.checkEventsUsable(player)
	end
end

function checkczph( player,num ) --充值排行
	local t1,t2 =getActivityOpenEndTime(4005)
	if get_day_YMDHM()>=t1 and get_day_YMDHM()<=t2 and checActivityOpen(4005) then 
		player:set_param(const.PP_CZRANK_JINGCAI_NUM,util.ppn(player, const.PP_CZRANK_JINGCAI_NUM)+num)
		setRechargeRank(player)
	end
end

function checkjscz( player,num )
	return false
end

function RuiShouOpenState()
	return false
end
function Exp2OpenState()  --检测双倍经验是否开启。

	local t1,t2 =getActivityTime(1001)
	if get_day_YMD()>=t1 and get_day_YMD()<=t2 and checActivityOpen(1001)  then 
		return true
	else
		return false
	end
end
function LottoryOpenState()
	local t1,t2 =getActivityTime(1006)
	if get_day_YMD()>=t1 and get_day_YMD()<=t2 and checActivityOpen(1006)  then 
		return true
	else
		return false
	end
end
-----------------------------------------------------------------------------------------
function onPanelData(player, commond)
	-- print(commond)
	local data =util.decode(commond)
	if data.actionid == "reqTabList" then--叶签列表
		senderToClientTab(player)
		--PP_JINGCAI_ICON_BRIGHT
		--player:set_param(const.PP_JINGCAI_ICON_BRIGHT,1)
		gui.moduleRedPoint.checkEventsUsable(player)
	elseif data.actionid == "updateRsCount" then--跟新超级瑞兽倒计时
		updateRuiShouCount(player)
	elseif data.actionid=="reqRuiShowGo" then--瑞兽传送
		player:enter_map("biqi", 32, 86)
		player:push_lua_table("close",util.encode({name = "extend_events"}))
	elseif data.actionid=="updateChongZhiData" then	--累计充值
		updateczData(player)
	elseif data.actionid=="getljczAward" then--累计充值领奖
		local index = data.params.index
		getljczAward(player,index)
	elseif data.actionid=="updateljxfData" then--累计消费
		updateljxfData(player)
	elseif data.actionid=="getljxfAward" then--累计消费领奖
		local index = data.params.index
		getljxfAward(player,index)
	elseif data.actionid=="updateLotteryData" then--全民探宝
		updateLotteryData(player)	
	elseif data.actionid=="updateQiangGouData" then--限时抢购
		updateQiangGouData(player)
	elseif data.actionid=="reqQiangGouGoods" then
		buyZheKouGoods(player,data.params.index)
	elseif data.actionid=="updateRechargeData" then
		updateRechargeData(player)
	elseif data.actionid=="updateRedpoint" then--刷新红点
		checkRedPointRecharge_js(player)
		checkRedPointxiaofei(player)
		checkRedPointRecharge(player)
		checkRedPointLangYa(player)
		checkRedPointLongXin(player)
		gui.PanelZhuanPan.checkZhuanPanRed(player)
	elseif data.actionid=="updatejsczData" then--急速充值
		updatejsczData(player)
	elseif data.actionid=="getjsczAward" then--急速
		getjsczAward(player)
	elseif data.actionid=="updateDoubleCount" then	--双倍exp
		updateDoubleExpCount(player)
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
	end
end

ckpanel.add_listener("gui.PanelJingCaiAct.onPanelData",gui.PanelJingCaiAct.onPanelData)

--推送页签数据
function senderToClientTab(player)
	local result = {
		cmd="updateJingCaiTabs",
		tabTable=getNeedTabTable(player),
		-- actType=actType,
	}
	if #result.tabTable then
		player:push_lua_table("PanelJingCaiAct",util.encode(result))
	end
end

--处理页签列表
function getNeedTabTable(player)
	local result = {}
	local tempTable={}
	tempTable=tabTable
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if itemData.func and itemData.func(player) then  --检测活动是否开启， 开启的话，需要推送给客户端。
			table.insert(result,{index=itemData.index,name=itemData.name,nameStr=itemData.nameStr})
		end
	end
	return result
end
-------------------------------------------------------------全民探宝------------------------------------------------------------------
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
	local t1,t2 =getActivityTime(1006)
	local result = {
		cmd="updateLotteryData",
		time=0,
		equips=util.clone(showEquips[job])
	}
	if  true then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		-- result.time=(7-startDay)*24*3600+(24-H)*3600-M*60
		result.time=(formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S
	end
	player:push_lua_table("PanelJingCaiAct",util.encode(result))
end

--------------------------------------------------------------限时抢购------------------------------------------------------------------
local zheKouTable = {
	[1]={
		{id=0, name="0", moneyType=0, pricey=0, price=99999, num=0, bind=1, allnum=3, svr="svr_curbuynum1", con=const.PP_QIANGGOU_JINGCAI1},
		{id=0, name="0", moneyType=0, pricey=0, price=99999, num=0, bind=1, allnum=3, svr="svr_curbuynum2", con=const.PP_QIANGGOU_JINGCAI2},
		{id=0, name="0", moneyType=0, pricey=0, price=99999, num=0, bind=1, allnum=3, svr="svr_curbuynum3", con=const.PP_QIANGGOU_JINGCAI3},
	},

}

function updateQiangGouData(player)
	--print("00000")
	local t1,t2 =getActivityTime(4004)
	local day = weekDay()
	local result = {
		cmd="updateQiangGouData",
		time=0,
		goods=util.clone(zheKouTable[1])
	}
	if true then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S
	end
	if not result.goods or #result.goods<=0 then
		result.goods=util.clone(zheKouTable[1])
	end
	
	for i=1,#result.goods do
		local itemData = result.goods[i]
		for j=1,#tab.bargain_limitBuy do 
			if tab.bargain_limitBuy[j].openday==day and tab.bargain_limitBuy[j].pos==i then 
				itemData.id=tab.bargain_limitBuy[j].itemid
				itemData.moneyType=tab.bargain_limitBuy[j].moneyType
				itemData.pricey=tab.bargain_limitBuy[j].oldMoneyNum
				itemData.price=tab.bargain_limitBuy[j].nowMoneyNum
				itemData.allnum=tab.bargain_limitBuy[j].limittimes
				itemData.num=tab.bargain_limitBuy[j].itemnum
			end
		end 
		itemData.yuNum=itemData.allnum-util.ppn(player, itemData.con)
	end
	player:push_lua_table("PanelJingCaiAct",util.encode(result))
end

function buyZheKouGoods(player,index)
	local t1,t2 =getActivityTime(2011)
	local tabindex=1
	local tempTable = util.clone(zheKouTable[1])
	if not tempTable or #tempTable<=0 then
		tempTable=util.clone(zheKouTable[1])
	end

	local itemData = tempTable[index]

	for j=1,#tab.bargain_limitBuy do 
		if tab.bargain_limitBuy[j].openday==weekDay() and tab.bargain_limitBuy[j].pos==index then 
			tabindex=tab.bargain_limitBuy[j].id
			break
		end
	end 
	if not itemData then return end
	if get_hour_minute()<1 then 
		player:alert(1,1,"该物品暂未开售,请一分钟后再来！")
		return
	end

	if (tab.bargain_limitBuy[tabindex].limittimes-util.ppn(player, itemData.con))<=0 then
		player:alert(1,1,"该物品已售罄！")
		return
	end
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end

	if tab.bargain_limitBuy[tabindex].moneyType==102 then
		if player:get_vcoin()<tab.bargain_limitBuy[tabindex].nowMoneyNum then
			return showChongzhi(player)
		else
			player:sub_vcoin(tab.bargain_limitBuy[tabindex].nowMoneyNum,"限时抢购")
		end
	end
	if tab.bargain_limitBuy[tabindex].moneyType==103 then
		if player:get_vcoin_bind()<tab.bargain_limitBuy[tabindex].nowMoneyNum then
			return player:alert(1,1,"绑定元宝不足！")
		else
			player:sub_vcoin_bind(tab.bargain_limitBuy[tabindex].nowMoneyNum,"限时抢购")
		end
	end
	player:add_item(tab.bargain_limitBuy[tabindex].itemid,tab.bargain_limitBuy[tabindex].itemnum,1)
	player:set_param(itemData.con,util.ppn(player, itemData.con)+1)
	player:alert(1,1,"成功购买"..tab.itemdef[tab.bargain_limitBuy[tabindex].itemid].name)
	updateQiangGouData(player)
end
--------------------------------------------------------------充值有礼------------------------------------------------------------------
local rankAwards = {
	[1]={
		svr="jingcai_rank_name_1",
		seedsvr="jingcai_rank_seedname_1",
		awards={
		{id = 0,	num = 1, bind = 1},
		{id = 0,	num = 1, bind = 1},
		{id = 0,	num = 1, bind = 1},
			--{id = 0,	num = 0,  bind = 0},
		}
	},
	[2]={
		svr="jingcai_rank_name_2",
		seedsvr="jingcai_rank_seedname_2",
		awards={
			{id = 0,	num = 1, bind = 1},
		{id = 0,	num = 1, bind = 1},
		{id = 0,	num = 1, bind = 1},
			--{id = 0,	num = 0,  bind = 0},
		}
	},
	[3]={
		svr="jingcai_rank_name_3",
		seedsvr="jingcai_rank_seedname_3",
		awards={
		{id = 0,	num = 1, bind = 1},
		{id = 0,	num = 1, bind = 1},
		{id = 0,	num = 1, bind = 1},
			--{id = 0,	num = 0,  bind = 0},
		}
	},
}
--tab.bargain_rechargerank[index].need_num
function updateRechargeData(player)
	local t1,t2 =getActivityTime(4005)
	local result = {
		cmd="updateRechargeRankData",
		time=0,
		myRank=0,
		curXiaoFei=util.ppn(player, const.PP_CZRANK_JINGCAI_NUM),
		awards=util.clone(rankAwards)
	}
	if  true then
		local startDay = server_start_day()+1
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S
	end
	for i=1,#result.awards do
		local itemData = result.awards[i]
		itemData.name=server.get_var(itemData.svr)
		
		if itemData.name==player:get_name() then
			result.myRank=i
		end
		for j=1,#tab.bargain_rechargerank[i].itemid do 
			itemData.awards[j].id=tab.bargain_rechargerank[i].itemid[j]
			itemData.awards[j].num=tab.bargain_rechargerank[i].itemnum[j]
			itemData.awards[j].bind=2-tab.bargain_rechargerank[i].itembind[j]
			--print(itemData.awards[j].id,itemData.awards[j].num,itemData.awards[j].bind)
		end
	end
	player:push_lua_table("PanelJingCaiAct",util.encode(result))
end

--计算充值排名前五的, 每有一个玩家充值就要调用一下这个接口刷新下数据。
function setRechargeRank(player)
	local curcz = util.ppn(player, const.PP_CZRANK_JINGCAI_NUM)
	if curcz<200000 then return end--最低20万上榜
	local rankData={
		{name=server.get_var("jingcai_rank_name_1"),seedname=server.get_var("jingcai_rank_seedname_1"),czNum=util.svn("jingcai_rank_cz_1")},--1
		{name=server.get_var("jingcai_rank_name_2"),seedname=server.get_var("jingcai_rank_seedname_2"),czNum=util.svn("jingcai_rank_cz_2")},--2
		{name=server.get_var("jingcai_rank_name_3"),seedname=server.get_var("jingcai_rank_seedname_3"),czNum=util.svn("jingcai_rank_cz_3")},--3
		{name=server.get_var("jingcai_rank_name_4"),seedname=server.get_var("jingcai_rank_seedname_4"),czNum=util.svn("jingcai_rank_cz_4")},--4
		{name=server.get_var("jingcai_rank_name_5"),seedname=server.get_var("jingcai_rank_seedname_5"),czNum=util.svn("jingcai_rank_cz_5")},--5
	}
	local tempTable = util.clone(rankData)
	local myIndex=0
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if itemData.seedname==player:get_seed_name() then
			table.remove(tempTable,i)
			myIndex=i
			break
		end
	end
	for j=1,5 do
		if not tempTable[j] or curcz>tempTable[j].czNum then
			table.insert(tempTable,j,{name=player:get_name(),seedname=player:get_seed_name(),czNum=curcz})
			if myIndex~=j then 
				server.info(10010,0,"恭喜<font color='#00ccff'>"..player:get_name().."</font>成为充值排行<font color='#ff0000'>第"..j.."名</font>")
			end 
			break
		end	 
	end
	for n=1,5 do
		if tempTable[n] then
			server.set_var("jingcai_rank_name_"..n,tempTable[n].name)
			server.set_var("jingcai_rank_seedname_"..n,tempTable[n].seedname)
			server.set_var("jingcai_rank_cz_"..n,tempTable[n].czNum)
		else
			server.set_var("jingcai_rank_name_"..n,"")
			server.set_var("jingcai_rank_seedname_"..n,"")
			server.set_var("jingcai_rank_cz_"..n,0)
		end
	end
end
login.add_login_listener(setRechargeRank)
function sendRankAwards()
	-- local serverDay = server_start_day()+1
	--local myName = player:get_seed_name()
	--消费有礼
	if weekDay()==1 then
		--print("00000000")
		for i=1,#rankAwards do
			local itemData = rankAwards[i]
			local seedname =server.get_var(itemData.seedsvr)
			--print("1111",seedname)
			local player = server.find_player_by_seed(seedname)
			local t = {}
			for j=1,#tab.bargain_rechargerank[i].itemid do
				table.insert(t,checknumber(tab.bargain_rechargerank[i].itemid[j]))
				table.insert(t,checknumber(tab.bargain_rechargerank[i].itemnum[j]))
			end
			if player then 
				--print("222222",player:get_name(),i)
				player:post_mail("充值排行", "您在超值回馈-充值排行中名列前茅\n获得排名第"..i.."名奖励",unpack(t))
			elseif seedname then 
				--print("333333",seedname)
			  ---local id = itemData.awards[j].id=tab.bargain_rechargerank[i].itemid[j]  itemData.awards[j].num=tab.bargain_rechargerank[i].itemnum[j]
				server.post_mail_to_one(seedname,"充值排行","您在超值回馈-充值排行中名列前茅\n获得排名第"..i.."名奖励",unpack(t))
				--util.sendMail(player,"充值豪礼", "您在充值豪礼中名列前茅\n获得排名第"..i.."名奖励", itemData.awards)
			end
			--util.sendMail(seedname,"消费有礼", "您在消费有礼中名列前茅\n获得排名第"..i.."名奖励", itemData.awards)
		end
		-- player:set_param(const.PP_RANK_MAIL_AWARD1,1)
	end
end

----------------------------------------------------累计充值----------------------------------------
--转生 龙新 狼牙


local ljcz_data1={
	[1]={con=const.PP_JINGCAI_CZ1,needyb=0,index=1,
		award={
			{name="",		     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
		},
	},
	[2]={con=const.PP_JINGCAI_CZ2,needyb=0, index=2,
		award={
			{name="",		     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
		},
	},
	[3]={con=const.PP_JINGCAI_CZ3,needyb=0, index=3,
		award={
		   {name="",		     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
		},
	},
	[4]={con=const.PP_JINGCAI_CZ4,needyb=0,index=4,
		award={
			{name="",		     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
		},
	},
	[5]={con=const.PP_JINGCAI_CZ5,needyb=0,index=5,
		award={ 
			{name="",		     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
		},
	},
	[6]={con=const.PP_JINGCAI_CZ6,needyb=0,index=6,
		award={
			{name="",		     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
			{name="",	 	     id=0, num=0},
		},
	},

}

function getcz_data( player )
		return ljcz_data1
end

function getljczAward(player,index)
	local  hdcz=util.ppn(player, const.PP_RECHARGE_JINGCAI)
	local ljcz_data=getcz_data( player )
	local data=ljcz_data[index]
		if data then
			if hdcz>=tab.bargain_recharge[index].need_num then
				if util.ppn(player, data.con) ==0  then
					if player:num_bag_black()>= #data.award then
						for i=1,#tab.bargain_recharge[index].itemid do
							player:add_item(tab.bargain_recharge[index].itemid[i],tab.bargain_recharge[index].itemnum[i],2-tab.bargain_recharge[index].itembind[i])
						end
						player:set_param(data.con,1)
						checkRedPointRecharge(player)
						gui.moduleRedPoint.checkEventsUsable(player)
					else
						player:alert(1, 1, "背包空间不足！")
						return
					end
				else
					player:alert(11,1,"您已经领取过，无法重复领取");
				end
			else
				player:alert(11,1,"累计充值元宝不足");
			end
		else
			util.alert_msg(4, "领取失败！", player);
		end
	updateczData(player)
end



function updateczData(player)
	local t1,t2 =getActivityOpenEndTime(4001)
	local d1,d2 =getActivityTime(4001)
	--print("ljcz_time",t1,t2)
	if  get_day_YMDHM()>=t1 and get_day_YMDHM()<=t2 then 
		local 	ljcz_data=getcz_data( player )
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		
		local result = {};
		result.cmd = "updateczData";
		result.datas = {};
		result.datas2 = {};
		result.cznum = util.ppn(player, const.PP_RECHARGE_JINGCAI)
		result.time = (formattime(get_day_YMD(),d2)-1)*24*3600+(24-H)*3600-M*60-S
		for i=1,#ljcz_data do 
			local info = {}
			info.con=util.ppn(player, ljcz_data[i].con)
			info.need = tab.bargain_recharge[i].need_num
			info.award = ljcz_data[i].award;
			--print(info.award[1].id)
			for j=1,#tab.bargain_recharge[i].itemid  do
				info.award[j].id=tab.bargain_recharge[i].itemid[j]
				info.award[j].num=tab.bargain_recharge[i].itemnum[j]
			end
			info.index=ljcz_data[i].index
			if util.ppn(player,ljcz_data[i].con) == 0 then
				table.insert(result.datas,info)
			else
				table.insert(result.datas2,info)
			end 
		end
		for j=1,#result.datas2 do
			result.datas[#result.datas+1]=result.datas2[j]
		end
		player:push_lua_table("PanelJingCaiAct",util.encode(result));
	end
end

--------------------------------------------------------------物品数量记录---------------------------------------------
function GetFurnaceScore(player,types,exp)
	--记录狼牙碎片大获取个数
	local langyat1,langyat2 =getActivityOpenEndTime(4003)
	local longxint1,longxint2 =getActivityOpenEndTime(4002)
	--print("000000000",langyat1,langyat2)
	if types==4  and (get_day_YMDHM() >=langyat1  and  get_day_YMDHM() <=langyat2)  and checActivityOpen(4003)  then 
		player:set_param(const.PP_LANGYA_GOT_NUM,util.ppn(player,const.PP_LANGYA_GOT_NUM)+exp)
		checkRedPointLangYa(player)
		gui.moduleRedPoint.checkEventsUsable(player)
	end
	--记录龙心碎片大获取个数
	-- print("111111111",longxint1,longxint2,get_day_YMDHM() ,checActivityOpen(4002) )
	if types==3  and (get_day_YMDHM() >=longxint1  and  get_day_YMDHM() <=longxint2) and checActivityOpen(4002) then
		player:set_param(const.PP_LONGXIN_GOT_NUM,util.ppn(player,const.PP_LONGXIN_GOT_NUM)+exp)
		-- print("00000000",longxint1,longxint2,get_day_YMDHM() ,checActivityOpen(4002), util.ppn(player,const.PP_LONGXIN_GOT_NUM))
		checkRedPointLongXin(player)
		gui.moduleRedPoint.checkEventsUsable(player)
	end
end

--------------------------------------------------------------狼牙回馈-----------------------------------------------------
local lyTable={
	[1]={id=23030003, name="狼牙碎片", useNum=0,  	con=const.PP_LANGYA_JINGCAI_LING1, awards={{id=0, num=1,  bind=1}} },
	[2]={id=23030003, name="狼牙碎片", useNum=0,  	con=const.PP_LANGYA_JINGCAI_LING2, awards={{id=0, num=0,  bind=1}} },
	[3]={id=23030003, name="狼牙碎片", useNum=0,  	con=const.PP_LANGYA_JINGCAI_LING3, awards={{id=0, num=0,  bind=1}} },
	[4]={id=23030003, name="狼牙碎片", useNum=0,	con=const.PP_LANGYA_JINGCAI_LING4, awards={{id=0, num=0,  bind=1}} },
	[5]={id=23030003, name="狼牙碎片", useNum=0, 	con=const.PP_LANGYA_JINGCAI_LING5, awards={{id=0, num=0,  bind=1}} },
	-- [6]={id=23030003, name="狼牙碎片", useNum=0, 	con=const.PP_LANGYA_JINGCAI_LING6, awards={{id=0, num=0,  bind=1}} },
	-- [7]={id=23030003, name="狼牙碎片", useNum=0, 	con=const.PP_LANGYA_JINGCAI_LING7, awards={{id=0, num=0,  bind=1}} },
	-- [8]={id=23030003, name="狼牙碎片", useNum=0, 	con=const.PP_LANGYA_JINGCAI_LING8, awards={{id=0, num=0,  bind=1}} },
	-- [9]={id=23030003, name="狼牙碎片", useNum=0, 	con=const.PP_LANGYA_JINGCAI_LING9, awards={{id=0, num=0,  bind=1}} },
	-- [10]={id=23030003,name="狼牙碎片", useNum=0, 	con=const.PP_LANGYA_JINGCAI_LING10, awards={{id=0,num=0,  bind=1}} },
}


function updateLangYaData(player)
	local t1,t2 =getActivityOpenEndTime(4003)
	local d1,d2 =getActivityTime(4003)
	--print("langya_time",t1,t2)
	if  get_day_YMDHM()>=t1 and get_day_YMDHM()<=t2 then 
		local result = {
			cmd="updateLangYaData",
			time=0,
			allUse=util.ppn(player,const.PP_LANGYA_GOT_NUM),
			dataTable=util.clone(lyTable)
		}
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(formattime(get_day_YMD(),d2)-1)*24*3600+(24-H)*3600-M*60-S
		-- print(result.time,allDay,mergeDay,server_merge_day())
		--keys={"id","need_num","itemid","itemnum","itembind"}
		for i=1,#result.dataTable do
			local itemData = result.dataTable[i]
			itemData.ling = util.ppn(player,itemData.con)
			itemData.useNum =tab.bargain_wolfang[i].need_num
			itemData.awards[1].id =  tab.bargain_wolfang[i].itemid
			itemData.awards[1].num =  tab.bargain_wolfang[i].itemnum
			itemData.awards[1].bind =  tab.bargain_wolfang[i].itembind
			--itemData.awards.num =   util.ppn(player,itemData.con)
		end
		player:push_lua_table("PanelJingCaiAct",util.encode(result))
	end
end

function getLangYaAwards(player,index)
	local itemData = lyTable[index]
	local allnum = util.ppn(player,const.PP_LANGYA_GOT_NUM)

	if not itemData then return end
	if util.ppn(player,itemData.con)>0 then
		player:alert(1,1,"已领取，不能重复领取！")
		return
	end
	if allnum<tab.bargain_wolfang[index].need_num then
		player:alert(1,1,"狼牙碎片（大）未达指定数量！")
		return
	end
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	-- for i=1,#itemData.awards do
	-- 	local awardData = itemData.awards[i]
	-- 	player:add_item(awardData.id,awardData.num,awardData.bind)
	-- end
	player:add_item(tab.bargain_wolfang[index].itemid,tab.bargain_wolfang[index].itemnum,2-tab.bargain_wolfang[index].itembind)
	player:set_param(itemData.con,1)
	player:alert(1,1,"成功领取狼牙回馈！")
	updateLangYaData(player)
	checkRedPointLangYa(player)
	gui.moduleRedPoint.checkEventsUsable(player)
	--gui.moduleRedPoint.checkHeQuAct(player)
end
--------------------------------------------------------------龙心回馈------------------------------------------------------------------
local lxTable={
	[1]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING1, awards={{id=0, num=0,  bind=0}} },
	[2]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING2, awards={{id=0, num=0,  bind=0}} },
	[3]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING3, awards={{id=0, num=0,  bind=0}} },
	[4]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING4, awards={{id=0, num=0,  bind=0}} },
	[5]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING5, awards={{id=0, num=0,  bind=0}} },
	-- [6]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING6, awards={{id=0, num=0,  bind=0}} },
	-- [7]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING7, awards={{id=0, num=0,  bind=0}} },
	-- [8]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING8, awards={{id=0, num=0,  bind=0}} },
	-- [9]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING9, awards={{id=0, num=0,  bind=0}} },
	-- [10]={id=23020003, name="龙心碎片", useNum=0,  con=const.PP_LONGXIN_JINGCAI_LING10,awards={{id=0, num=0,  bind=0}} },
}

function updateLongXinData(player)
	local d1,d2 =getActivityTime(4002)
	local t1,t2 =getActivityOpenEndTime(4002)
	if  get_day_YMDHM()>=t1 and get_day_YMDHM()<=t2 then 
		local result = {
			cmd="updateLongXinData",
			time=0,
			allUse=util.ppn(player,const.PP_LONGXIN_GOT_NUM),
			dataTable=util.clone(lxTable)
		}
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(formattime(get_day_YMD(),d2)-1)*24*3600+(24-H)*3600-M*60-S
		for i=1,#result.dataTable do
			local itemData = result.dataTable[i]
			itemData.ling = util.ppn(player,itemData.con)
			itemData.useNum =tab.bargain_dragonHeart[i].need_num
			itemData.awards[1].id =  tab.bargain_dragonHeart[i].itemid
			itemData.awards[1].num =  tab.bargain_dragonHeart[i].itemnum
			itemData.awards[1].bind =  tab.bargain_dragonHeart[i].itembind
		end
		player:push_lua_table("PanelJingCaiAct",util.encode(result))
	end
end

function getLongXinAwards(player,index)
	local itemData = lxTable[index]
	local allnum = util.ppn(player,const.PP_LONGXIN_GOT_NUM)
	if not itemData then return end
	if util.ppn(player,itemData.con)>0 then
		player:alert(1,1,"已领取，不能重复领取！")
		return
	end
	if allnum<tab.bargain_dragonHeart[index].need_num then
		player:alert(1,1,"龙心碎片（大）未达指定数量！")
		return
	end
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	-- for i=1,#itemData.awards do
	-- 	local awardData = itemData.awards[i]
	-- 	player:add_item(awardData.id,awardData.num,awardData.bind)
	-- end
	player:add_item(tab.bargain_dragonHeart[index].itemid,tab.bargain_dragonHeart[index].itemnum,2-tab.bargain_dragonHeart[index].itembind)
	player:set_param(itemData.con,1)
	player:alert(1,1,"成功领取龙心回馈！")
	updateLongXinData(player)
	checkRedPointLongXin(player)
	gui.moduleRedPoint.checkEventsUsable(player)
end

--------------------------------------------------------------超级瑞兽------------------------------------------------------------------
function updateRuiShouCount(player)

	local result = {
		cmd="updateRuiShouCount",
		time=0,
	}

		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(formattime(get_day_YMD(),jingcaiactrs_end)-1)*24*3600+(24-H)*3600-M*60-S

	
	--player:push_lua_table("PanelJingCaiAct",util.encode(result))

end


--------------------------------------------------------------累计消费------------------------------------------------------------------

local ljxf_data={
	[1]={con=const.PP_JINGCAI_XF1,needyb=20000,index=1,
		award={
			{name="",		     id=24060001, num=9},
			{name="",	 	     id=23060003, num=2},
		},
	},
	[2]={con=const.PP_JINGCAI_XF2,needyb=50000, index=2,
		award={
			{name="",		     id=24060001, num=18},
			{name="",	 	     id=23090002, num=3},
		},
	},
	[3]={con=const.PP_JINGCAI_XF3,needyb=100000, index=3,
		award={
		    {name="",		     id=23000002, num=80},
			{name="",	 	     id=23010002, num=70},
			{name="",	 	     id=23120002, num=50},
		},
	},
	[4]={con=const.PP_JINGCAI_XF4,needyb=200000,index=4,
		award={
			{name="",		     id=24000001, num=5},
			{name="",	 	     id=23050003, num=10},
			{name="",	 	     id=23120003, num=5},
		},
	},
	[5]={con=const.PP_JINGCAI_XF5,needyb=300000,index=5,
		award={ 
			{name="",		     id=23030002, num=5},
			{name="",	 	     id=23020002, num=5},
			{name="",	 	     id=24020004, num=10},
		},
	},
	[6]={con=const.PP_JINGCAI_XF6,needyb=500000,index=6,
		award={
			{name="",		     id=27020006, num=1},
		},
	},


}

function getljxfAward(player,index)

	local  hdxf=util.ppn(player, const.PP_XIAOFEI_JINGCAI)
	local data=ljxf_data[index]
		if data then
			if hdxf>=data.needyb then
				if util.ppn(player, data.con) ==0  then
					if player:num_bag_black()>= #data.award then
						for i=1,#data.award do
							player:add_item(data.award[i].id,data.award[i].num,1)
							--player:alert(1, 1, "获得"..data.award[i].name.."*"..data.award[i].num)
						end
						player:set_param(data.con,1)
						checkRedPointxiaofei(player)
						gui.moduleRedPoint.checkEventsUsable(player)
					else
						player:alert(1, 1, "背包空间不足！")
						return
					end
				else
					player:alert(11,1,"您已经领取过，无法重复领取");
				end
			else
				player:alert(11,1,"累计消费元宝不足");
			end
		else
			util.alert_msg(4, "领取失败！", player);
		end
	updateljxfData(player)
end

function updateljxfData(player)
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local result = {};
	result.cmd = "updatexfData";
	result.datas = {};
	result.datas2 = {};
	result.xfnum =util.ppn(player, const.PP_XIAOFEI_JINGCAI)
	result.time = (formattime(get_day_YMD(),jingcaiact_end)-1)*24*3600+(24-H)*3600-M*60-S
	for i=1,#ljxf_data do 
			local info = {}
			info.con=util.ppn(player, ljxf_data[i].con)
			info.need = ljxf_data[i].needyb;
			info.award = ljxf_data[i].award;
			info.index=ljxf_data[i].index
			if util.ppn(player,ljxf_data[i].con) == 0 then
				table.insert(result.datas,info)
				--print(i)
				--print("=1=",util.encode(info))
				--print("=2=",result.datas[i].need)
			else
				table.insert(result.datas2,info)
			end 
		end
		for j=1,#result.datas2 do
			result.datas[#result.datas+1]=result.datas2[j]
		end
	--player:push_lua_table("PanelJingCaiAct",util.encode(result));
end





--201704180001
---------------------------------------------jscz-------------------------------
local jscz_data={
	[1]={con=const.PP_JINGCAI_CZ1,needyb=5000,times=20,ymd1=201704160000,ymd2=201704170001,
		award={
			{name="",		     id=24030001, num=10},
			
		},
	},
	[2]={con=const.PP_JINGCAI_CZ1,needyb=5000,times=20,ymd1=201704170001,ymd2=201704180001,
		award={
			{name="",		     id=23090001, num=10},
			
		},
	},
	[3]={con=const.PP_JINGCAI_CZ3,needyb=5000, times=20,ymd1=201704180001,ymd2=201704190001,
		award={
		   {name="",		     id=24000001, num=1},
			
		},
	},
	[4]={con=const.PP_JINGCAI_CZ4,needyb=5000,times=20,ymd1=201704190001,ymd2=201704200001,
		award={
			{name="",		     id=24060001, num=1},
			
		},
	},
	[5]={con=const.PP_JINGCAI_CZ5,needyb=5000,times=20,ymd1=201704200001,ymd2=201704210001,
		award={ 
			{name="",		     id=23120002, num=3},
			
		},
	},
	[6]={con=const.PP_JINGCAI_CZ6,needyb=5000,times=20,ymd1=201704210001,ymd2=201704220001,
		award={
			{name="",		     id=23050002, num=3},
			
		},
	},
	[7]={con=const.PP_JINGCAI_CZ,needyb=5000,times=20,ymd1=201704220001,ymd2=201704230000,
		award={
			{name="",		     id=24030001, num=10},
			
		},
	},


}

function getjsczAward(player)
	local ymdhm = tonumber(os.date("%Y%m%d%H%M", os.time()))

	local  num=20-util.ppn(player,const.PP_RECHARGEJS_JINGCAI_LING)
	if num<=0 then 
		return
	end


	for k,v in pairs(jscz_data) do
		if  ymdhm >=v.ymd1 and ymdhm <v.ymd2  then 
			if player:num_bag_black()>= #v.award then
				for i=1,#v.award do
					player:add_item(v.award[i].id,v.award[i].num,1)
					player:set_param(const.PP_RECHARGEJS_JINGCAI_LING,util.ppn(player,const.PP_RECHARGEJS_JINGCAI_LING)+1)
				end
				checkRedPointRecharge_js(player)
				gui.moduleRedPoint.checkEventsUsable(player)
			else
				player:alert(1, 1, "背包空间不足！")
				return
			end 
		end
	end

	updatejsczData(player)
end

function updatejsczData(player)
	-- if not tabTable[5].func(player) then 
	-- 	return
	-- end
	local ymdhm = tonumber(os.date("%Y%m%d%H%M", os.time()))
	local shengyu=20-util.ppn(player,const.PP_RECHARGEJS_JINGCAI_LING)			
	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local result = {};
	result.cmd = "updatejsczData";
	result.datas = {};
	if (24-H)*3600-M*60-S+60 >86400 then 
		result.time = (24-H)*3600-M*60-S+60-86400
	else
		result.time = (24-H)*3600-M*60-S+60
	end
	result.jscznum = math.floor(util.ppn(player,const.PP_RECHARGEJS_JINGCAI)/5000)-util.ppn(player,const.PP_RECHARGEJS_JINGCAI_LING)
	
	--result.actiDate = util.formatDate(actiStartTime).."-"..util.formatDate(actiEndTime);
	-- for k,v in pairs(jscz_data) do
	-- 	--if v.ymd== get_day_YMD() then 
	
	-- 		result.datas[k] = {};
	-- 		result.datas[k].con = util.ppn(player, v.con)
	-- 		result.datas[k].need = v.needyb;
	-- 		result.datas[k].award = v.award;
	-- 		print(k)
	-- 	--end
	-- end
	for k,v in pairs(jscz_data) do
		if  ymdhm >=v.ymd1 and ymdhm <v.ymd2  then 
			result.datas[1] = {};
			--result.datas[1].con = util.ppn(player, v.con)
			result.datas[1].need = v.needyb;
			result.datas[1].award = v.award;
			result.datas[1].shengyu = shengyu
			result.datas[1].times = v.times;
		end
	end
	--result.cellIndex = cellIndex
	--player:push_lua_table("PanelJingCaiAct",util.encode(result));
end



--------------------------------------------------------------全服双倍------------------------------------------------------------------

function updateDoubleExpCount(player)
	local t1,t2 =getActivityTime(1001)
	local result = {
		cmd="updateDoubleExpCount",
		time=0,
	}
		local H = tonumber(os.date("%H",os.time()))
		local M = tonumber(os.date("%M",os.time()))
		local S = tonumber(os.date("%S",os.time()))
		result.time=(formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S

	player:push_lua_table("PanelJingCaiAct",util.encode(result))
end


function getYuTime(player)
	local t1,t2 =getActivityTime(1001)

	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	local time=(formattime(get_day_YMD(),t2)-1)*24*3600+(24-H)*3600-M*60-S
	
	return time
end






-------------------------------------------------------红点检测--------------------------------------------------------------
--狼牙回馈红点检测
function checkRedPointLangYa(player)
	local result = false
	local allUse=util.ppn(player,const.PP_LANGYA_GOT_NUM)--已使用次数
	local dataTable=util.clone(lyTable)
	for i=1,#dataTable do
		local itemData = dataTable[i]
		itemData.ling = util.ppn(player,itemData.con)
		if allUse>=tab.bargain_wolfang[i].need_num and itemData.ling<=0 then
			result = true
			break
		end
	end
	player:push_lua_table("PanelJingCaiAct",util.encode({cmd="updateTabRed",name="tabName8",show=result}))
	return result
end

--龙心回馈红点检测
function checkRedPointLongXin(player)
	local result = false
	local allUse=util.ppn(player,const.PP_LONGXIN_GOT_NUM)
	local dataTable=util.clone(lxTable)
	for i=1,#dataTable do
		local itemData = dataTable[i]
		itemData.ling = util.ppn(player,itemData.con)
		if allUse>=tab.bargain_dragonHeart[i].need_num and itemData.ling<=0 then
			result = true
			break
		end
	end
	player:push_lua_table("PanelJingCaiAct",util.encode({cmd="updateTabRed",name="tabName9",show=result}))
	return result
end

--累计消费红点检测
function checkRedPointxiaofei(player)
	
	local result=false
	local  hdxf=util.ppn(player, const.PP_XIAOFEI_JINGCAI)
	for i=1,#ljxf_data do
		if util.ppn(player,ljxf_data[i].con)==0 and hdxf >=ljxf_data[i].needyb then
			result=true
		end
	end
	--player:push_lua_table("PanelJingCaiAct",util.encode({cmd="updateTabRed",name="tabName3",show=result}))
	return result
end

--累计充值红点检测
function checkRedPointRecharge(player)
	local result=false
	local 	ljcz_data=getcz_data(player)
	local  hdcz=util.ppn(player, const.PP_RECHARGE_JINGCAI)
	for i=1,#ljcz_data do
		if util.ppn(player,ljcz_data[i].con)==0 and hdcz >=tab.bargain_recharge[i].need_num then
			result= true
		end
	end
	player:push_lua_table("PanelJingCaiAct",util.encode({cmd="updateTabRed",name="tabName2",show=result}))
	return result
end


--急速充值红点检测
function checkRedPointRecharge_js(player)
	local result=false
	if 20-util.ppn(player,const.PP_RECHARGEJS_JINGCAI_LING)>0  and  math.floor(util.ppn(player,const.PP_RECHARGEJS_JINGCAI)/5000)-util.ppn(player,const.PP_RECHARGEJS_JINGCAI_LING)>0 then
		result= true
	end
	--player:push_lua_table("PanelJingCaiAct",util.encode({cmd="updateTabRed",name="tabName6",show=result}))
	return result
end

--精彩活动图标红点  gui.moduleRedPoint.checkEventsUseable(player)
function checkJingCaiRedPoint(player)
	--if  util.ppn(player,const.PP_JINGCAI_ICON_BRIGHT)== 0 then return true end 
	if checkRedPointLangYa(player) or checkRedPointLongXin(player)  or checkRedPointRecharge(player) or gui.PanelZhuanPan.checkZhuanPanRed(player) then 
	--if checkRedPointxiaofei(player) or checkRedPointRecharge(player) or checkRedPointRecharge_js(player) then 
		return true
	else
		return false	
	end 
end
