module(..., package.seeall)

local vipData = tab.Vip.getVipConf()

function onPanelData(player, commondData)
	local data =util.decode(commondData);
	if data.actionid == "fresh" then
		freshVip(player)
	elseif data.actionid == "vipinfo" then
		freshVipInfo(player,data.vip)
	elseif data.actionid == "getAward" then
		getVipAward(player,data.vip)
	elseif data.actionid == "getBuff" then
		getVipBuff(player)
	end
end
ckpanel.add_listener("gui.PanelVip.onPanelData",gui.PanelVip.onPanelData)

function freshVip(player)
   	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
   	local result = {}
   	result.cmd = "fresh"
   	result.vipLv = vipLv
   	result.vipData = util.clone(vipData)
   	for k,v in pairs(vipData) do
   		result.vipData[k].redPoint = v.lv<=vipLv and (v.lv==vipLv and util.ppn(player, const["PP_VIP_BUFF"..v.lv])==0 or util.ppn(player, const["PP_VIP_PACKAGE"..v.lv]) ==0 )
   	end
   	player:push_lua_table("PanelVip",util.encode(result));
end

function freshVipInfo(player,vip)
   	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
   	local result = {}
   	result.cmd = "vipinfo"
   	result.vipLv = vipLv
   	result.vipInfoData = tab.Vip.getVipInfo(vip)
   	result.vipAward = tab.Vip.getVipAward(vip)
   	result.vipExp = util.ppn(player, const.PP_VIP_EXP)
    result.vipMax = getNextMaxCharge(player)
   	result.isGot = util.ppn(player, const["PP_VIP_PACKAGE"..vip])
   	result.showBuff = false
   	if vip == vipLv and util.ppn(player, const["PP_VIP_BUFF"..vipLv])==0 then
   		result.showBuff = true
   	end
   	local redPoints = {}
   	for k,v in pairs(vipData) do
   		redPoints[k] = v.lv<=vipLv and (v.lv==vipLv and util.ppn(player, const["PP_VIP_BUFF"..v.lv])==0 or util.ppn(player, const["PP_VIP_PACKAGE"..v.lv]) ==0 )
   	end
   	result.redPoints = redPoints
   	result.btnGetShow = vip==vipLv and  util.ppn(player, const["PP_VIP_PACKAGE"..vip]) ==0

   	player:push_lua_table("PanelVip",util.encode(result));
end

function getVipAward(player,vip)
	local vipAward = tab.Vip.getVipAward(vip)
	if vipAward then
		if util.ppn(player, const["PP_VIP_PACKAGE"..vip]) ==0 then
			if util.ppn(player, const.PP_VIP_LEVEL)>=vip then
				if util.getAwardNeedBags(player,vipAward)<=player:num_bag_black() then
					if util.getAward(player,vipAward) then
						player:set_param(const["PP_VIP_PACKAGE"..vip],1)
						util.alert_msg(4, string.format("成功领取VIP%d礼包！",vip),player)
						freshVipInfo(player,vip)
						gui.moduleRedPoint.checkVipUsable(player)
					end
				else
					util.alert_msg(4, "背包格子不足！", player)
				end
			else
				util.alert_msg(4, "VIP等级不足！", player)
			end
		else
			util.alert_msg(4, "VIP礼包已领取！", player)
		end
	else
		util.alert_msg(4, "VIP礼包不存在！", player)
	end
end

function getVipBuff(player)
	local vip = util.ppn(player, const.PP_VIP_LEVEL)
	local vipConf = tab.Vip.getVipConf(vip)
	if vipConf then
		local expBuffId = vipConf.expBuffId
		if expBuffId>0 and util.ppn(player, const["PP_VIP_BUFF"..vip])==0 then
			util.alert_msg(4, string.format("成功领取VIP%d多倍经验BUFF！",vip),player)
			player:set_param(const["PP_VIP_BUFF"..vip],1)
			player:set_buff(expBuffId)
			freshVipInfo(player,vip)
			gui.moduleRedPoint.checkVipUsable(player)
		else
			util.alert_msg(4, "多倍经验BUFF已领取!", player)
		end
	else
		util.alert_msg(4, "VIP经验BUFF不存在！", player)
	end
end

function getNextMaxCharge(player)
	local vip = util.ppn(player, const.PP_VIP_LEVEL)
	if vip<#vipData then
		return vipData[vip+1].needVcoin
	else
		return vipData[#vipData].needVcoin
	end
end

function getMaxLevel(player)
	return #vipData
end

function upgradeVipLevel(player,total)
	local curLev,vipLv = util.ppn(player, const.PP_VIP_LEVEL),0
	for i,v in ipairs(vipData) do
		if v.needVcoin<=total and v.lv >vipLv then
			vipLv = v.lv
		end
	end
	local vipconf = tab.Vip.getVipConf(vipLv)
	player:set_param(const.PP_VIP_EXP,total)
	if curLev~=vipLv then
		player:set_param(const.PP_VIP_LEVEL,vipLv)
		player:set_model(5,vipLv)

		player:set_buff(vipconf.attackBuffId)

		freshVip(player)
		gui.moduleRedPoint.checkVipUsable(player)
		gui.PanelOffline.checkUpLevel(player,player:get_level())
	else
		freshVipInfo(player,curLev)
	end
	if curLev == 0 and vipLv>0 then
		server.info(10000, 1, "<font color='#fff843'>恭喜<font color='#01ebff'>"..player:get_name().."</font>成为VIP！享受尊贵VIP特权！</font>")
		server.info(10, 1, "[system]<font color='#fff843'>恭喜<font color='#01ebff'>"..player:get_name().."</font>成为VIP用户！享受尊贵VIP特权！<a color='#00ff00' href='event:local_OpenPanel_act30' underline='#00ff00'>我也要成为VIP</a></font>")
	end
end

function checkRedPoint(player)
	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
	if vipLv>0 then
		if util.ppn(player, const["PP_VIP_BUFF"..vipLv])==0 then
			return true
		end
		for i=1,#vipData do
			if i<=vipLv and util.ppn(player, const["PP_VIP_PACKAGE"..i]) ==0 then
				return true
			end
		end
	end
end