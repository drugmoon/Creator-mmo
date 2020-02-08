module(... ,package.seeall)

local awardTable = {
	{ id = 24030001, num = 10, bind = 1},
	{ id = 21000003, num = 5,  bind = 1},
	{ id = 23060002, num = 5,  bind = 1},
	{ id = 23080003, num = 5,  bind = 1},
}
local con = const.PP_DOWNLOAD_AWARD

function handlePanelData(player,actionid)
	if actionid == "fresh" then
		local result ={}
		result.award = awardTable
		result.con = util.ppn(player, con)
		player:push_lua_table("PanelDownLoad",util.encode(result))
	elseif actionid == "get" then
		if util.ppn(player, con) < 2 then
			local need_bag_num = util.getAwardNeedBags(player,awardTable)
			if player:num_bag_black()>=need_bag_num then
				for k,v in pairs(awardTable) do
					player:add_item(v.id,v.num,1,v.bind)
				end
				player:set_param(con,2)
				player:push_lua_table("PanelDownLoad",util.encode({cmd = "close",con = 2}))
				gui.moduleGuiButton.checkOpenFunc(player);
			else
				util.alert_msg(4, "背包不足！", player)
			end
		else
			util.alert_msg(4, "下载奖励已领取！", player)
		end
	elseif actionid == "downall" then
		-- player:push_lua_table("LoadAward",util.ppn(player, con))
		if util.ppn(player, con) < 2 then
			player:set_param(con,1)
			gui.moduleRedPoint.checkDownLoadUsable(player);
		end
	end
end
ckpanel.add_listener("gui.PanelDownLoad.handlePanelData",gui.PanelDownLoad.handlePanelData)

--检测首充领取状态
function checkRedPointDownLoad(player)
	if util.ppn(player, con) == 1  then
		return true
	end
	return false
end