module(... ,package.seeall)

local tabTable = {
	{index=1, name="特惠礼包",nameStr="tehui",    func=function(player) return gui.AwardHall_tehui.canBuy(player) and server_start_day()<7 end},
	{index=2, name="全民BOSS",nameStr="qmboss",   func=function(player) return server_start_day()<7 and gui.AwardHall_boss.haveBossAward(player)==true end},
	{index=9, name="连续充值",nameStr="lxcz",     func=function(player) return server_start_day()<7 end},--开服第一天
	{index=8, name="累计充值",nameStr="leichong", func=function(player) return server_start_day()<7 end},--开服第一天
	{index=3, name="等级竞技",nameStr="dengji",   func=function(player) return server_start_day()<7 end},--开服第一天
	{index=4, name="神翼竞技",nameStr="shenyi",   func=function(player) return server_start_day()>=1 and server_start_day()<7 end},--开服第二天
	{index=5, name="龙心竞技",nameStr="longxin",  func=function(player) return server_start_day()>=2 and server_start_day()<7 end},--开服第四天
	{index=6, name="狼牙竞技",nameStr="langya",   func=function(player) return server_start_day()>=3 and server_start_day()<7 end},--开服第三天
	{index=7, name="决战沙城",nameStr="shabake",  func=function(player) return server_start_day()<7 end},--开服前七天显示
	{index= 10 , name = "七天狂欢",nameStr = "sevenDay" ,func=function(player) return server_start_day()<7 end},
	--{index= 11 , name = "嗜魔神",nameStr = "bossShi" ,func=function(player) return server_start_day()<7 end},
	--{index= 12 , name = "兽统领",nameStr = "bossShou" ,func=function(player) return server_start_day()<7 end},
}
       
function onPanelData(player, commond)
	-- print(server_start_day()<=7,"--22222")
	local data =util.decode(commond)
	if data.actionid == "reqTabList" then
		senderToClientTab(player)
	elseif data.actionid == "receive" then

	end
end

ckpanel.add_listener("gui.PanelActivities.onPanelData",gui.PanelActivities.onPanelData)

--推送页签数据
function senderToClientTab(player)
	local result = {
		cmd="updateActTabs",
		tabTable=getNeedTabTable(player),
	}
	if #result.tabTable then
		player:push_lua_table("PanelRewardHall",util.encode(result))
	end
end

--处理页签列表
function getNeedTabTable(player)
	local result = {}
	for i=1,#tabTable do
		local itemData = tabTable[i]
		if itemData.func and itemData.func(player) then
			table.insert(result,{index=itemData.index,name=itemData.name,nameStr=itemData.nameStr})
		end
	end
	return result
end

--刷新页签红点
function updateTabRedShow(player,index,isshow)
	local result = {
		cmd="updateTabRedShow",
		index=index,
		isshow=isshow,
	}
	player:push_lua_table("PanelRewardHall",util.encode(result))
end