module(...,package.seeall)
local needRMB = 1000
local platforToolQQ = {
	[0] = "正在申请QQ号途中",
	[888] = 3559605391,--测试白包
}
function handlePanelData(player,commond)
	local data = util.decode(commond)
	if not data then return end
	if data.actionid == "fresh" then
		local result = {}
		local totalCharge = util.ppn(player, const.PP_SUM_RECHARGE)
		result.qqinfo = "累计充值"..needRMB.."元显示QQ"
		result.totalCharge = totalCharge
		if totalCharge>=needRMB*100 then
			--if platforToolQQ[data.tag] then
				-- result.qqinfo = "客服QQ："..platforToolQQ["888"]
				result.qqinfo = "客服QQ：".."3559605391";
			-- else
			-- 	result.qqinfo = ""
			-- end
		end
		player:push_lua_table("PanelSuperVip",util.encode(result))
	elseif data.actionid == "get" then
		if util.ppn(player, const.PP_SECUTITY_PACKAGE)<1 then
			player:set_param(const.PP_SECUTITY_PACKAGE,1)
		else
			player:alert(1,1,"礼包已领取！")
		end
	end
end

ckpanel.add_listener("gui.PanelSuperVip.handlePanelData",gui.PanelSuperVip.handlePanelData)

