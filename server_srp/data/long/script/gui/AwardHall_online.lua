module(... ,package.seeall)

--每日在线奖励配置
local awardsTable = {
	[1]={needTime=300,needGe=1,con=const.PP_HALL_ONLINE_LING1,
		awards={
			[1]={id=23060001,num=1,bind=1},
		}
	},
	[2]={needTime=600,needGe=2,con=const.PP_HALL_ONLINE_LING2,
		awards={
			[1]={id=23060002,num=1,bind=1},
			[2]={id=32000002,num=1,bind=1},
		}
	},
	[3]={needTime=1800,needGe=3,con=const.PP_HALL_ONLINE_LING3,
		awards={
			[1]={id=23060002,num=1,bind=1},
			[2]={id=32000003,num=1,bind=1},
		}
	},
	[4]={needTime=3600,needGe=4,con=const.PP_HALL_ONLINE_LING4,
		awards={
			[1]={id=23060003,num=1,bind=1},
			[2]={id=32000003,num=1,bind=1},
		}
	},
}

function handlePanelData(player,commond)
	local data = util.decode(commond)
	local todayWeek = tonumber(os.date("%w", os.time())) --0-6-周日-周六
	if data.actionid == "reqOnlineData" then
		senderToClient(player)
	elseif data.actionid == "reqOnlineAward" then
		getDailyOnlineAward(player,data.params.index)
	elseif data.actionid == "reqOldWeekAward" then
		getOldWeekAward(player)
	end
end

--推送在线奖励数据
function senderToClient(player)
	local result = {
		cmd="updateOnlineData",
		dataTable=util.clone(awardsTable),
		oldWeekAward=util.ppn(player,const.PP_ONLINE_BVCION_OLD),
		newWeekAward=util.ppn(player,const.PP_ONLINE_BVCION_NEW),
		onlineTime=player:get_online_time_today(),
		weekLing=util.ppn(player,const.PP_ONLINE_OLDWEEK_LING),
	}
	for i=1,#result.dataTable do
		local itemData = result.dataTable[i]
		itemData.ling=util.ppn(player,itemData.con)
	end
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--领取每日在线奖励
function getDailyOnlineAward(player,index)
	local itemData=awardsTable[index]
	if not itemData then return end
	local onlineTime = math.ceil(player:get_online_time_today())
	if util.ppn(player,itemData.con)==1 then
		player:alert(1,1,"该奖励已领取，不能重复领取！")
		return
	end
	if onlineTime<itemData.needTime then
		player:alert(1,1,"在线时长不满足条件！")
		return
	end
	player:set_param(itemData.con,1)
	for j=1,#itemData.awards do
		local awardData = itemData.awards[j]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:alert(1,1,"在线奖励领取成功！")
	senderToClient(player)
	gui.moduleRedPoint.checkAwardOnlineUsable(player);
end

--领取上周累计奖励
function getOldWeekAward(player)
	local todayWeek = tonumber(os.date("%w", os.time())) --0-6-周日-周六
	-- if todayWeek~=1 then
	-- 	player:alert(1,1,"上周累计奖励只能在周一领取！")
	-- 	return
	-- end
	if util.ppn(player,const.PP_ONLINE_OLDWEEK_LING)==1 then 
		player:alert(1,1,"上周累计奖励已领取，不能重复领取！")
		return
	end
	local allAward = util.ppn(player,const.PP_ONLINE_BVCION_OLD)
	if allAward==0 then 
		player:alert(1,1,"上周累计奖励为空！")
		return
	end
	player:add_vcoin_bind(allAward)--绑定元宝
	player:set_param(const.PP_ONLINE_OLDWEEK_LING,1)
	player:alert(1,1,"成功领取 绑定元宝*"..allAward)
	player:set_param(const.PP_ONLINE_BVCION_OLD,0)--上周奖励领取成功后置0
	senderToClient(player)
	gui.moduleRedPoint.checkAwardOnlineUsable(player);
end

--每周在线时间累计（没到周一清0一次）
function addWeekOnline(player)
	-- local onlineTime = math.ceil(player:get_online_time_today()/60)
	-- player:set_param(const.PP_ONLINE_WEEK,util.ppn(player,const.PP_ONLINE_WEEK)+onlineTime)--周累计时间
	-- player:set_param(const.PP_ONLINE_BVCION_NEW,util.ppn(player,const.PP_ONLINE_BVCION_NEW)+onlineTime*10)--周累计奖励
end

--每小时记录一次周在线累计
function updateWeekOnline(player)
	local weekHour = util.ppn(player,const.PP_ONLINE_WEEK)--累计时间不超过70小时
	if weekHour<70 then
		local startDay = tonumber(os.date("%w", server_start_time()))--开区第一天是周几
		if startDay==0 then startDay=7 end
		local add = 30
		local serverday = server_start_day() or 0
		if (startDay+serverday)>7 and (startDay+serverday)<=14 then
			add = 40
		elseif (startDay+serverday)>14 then
			add = 50
		end
		player:set_param(const.PP_ONLINE_WEEK,util.ppn(player,const.PP_ONLINE_WEEK)+1)--周累计时间(单位小时)
		player:set_param(const.PP_ONLINE_BVCION_NEW,util.ppn(player,const.PP_ONLINE_BVCION_NEW)+add)--周累计奖励
		senderToClient(player)
		gui.moduleRedPoint.checkAwardOnlineUsable(player);
	end
end

--周一的时候把累计时间转化为奖励 清0周累计时间变量
function setOldWeekAward(player)
	local todayWeek = tonumber(os.date("%w", os.time())) --0-6-周日-周六
	if todayWeek==1 then
		local weekOnline = util.ppn(player,const.PP_ONLINE_BVCION_NEW)
		player:set_param(const.PP_ONLINE_BVCION_OLD,0)--清0周累计奖励
		player:set_param(const.PP_ONLINE_BVCION_OLD,util.ppn(player,const.PP_ONLINE_BVCION_OLD)+weekOnline)
		player:set_param(const.PP_ONLINE_BVCION_NEW,0)--清0周累计奖励
		player:set_param(const.PP_ONLINE_WEEK,0)--清0周累计时间没到周一重新开始计时
		player:set_param(const.PP_ONLINE_AWARD_DAY,server_start_day())--记录每次累计时间转换时是开区第几天
		player:set_param(const.PP_ONLINE_OLDWEEK_LING,0)
		senderToClient(player)
	end
end


ckpanel.add_listener("gui.AwardHall_online.handlePanelData",gui.AwardHall_online.handlePanelData)

--检测在线奖励有无可奖励可领取
function checkRedPoint(player)
	local onlineTime = math.ceil(player:get_online_time_today())
	for i=1,#awardsTable do
		local itemData = awardsTable[i]
		if util.ppn(player,itemData.con)==0 and  onlineTime>=itemData.needTime then
			return true
		end
	end
	local todayWeek = tonumber(os.date("%w", os.time())) --0-6-周日-周六
	if todayWeek~=1 then
		-- player:alert(1,1,"上周累计奖励只能在周一领取！")
		return false
	end
	if util.ppn(player,const.PP_ONLINE_OLDWEEK_LING)==1 then 
		-- player:alert(1,1,"上周累计奖励已领取，不能重复领取！")
		return false
	end
	local allAward = util.ppn(player,const.PP_ONLINE_BVCION_OLD)
	if allAward==0 then 
		-- player:alert(1,1,"上周累计奖励为空！")
		return false
	end
	return true
end





