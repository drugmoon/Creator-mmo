module(... ,package.seeall)

local awardTab = {
	[1]={
		cmd="yr",needvcoion=3000,max_num=100,remainnum="INVESTYR_NUM",investtag=const.PP_TZJH_HAS_INVEST_TAG1,awardlist={
			{needlevel=80,isZs=0,typeId=21010004,itemname="500元宝",bind=0,num=4,gettag=const.PP_TZJH_YR_LQJL1},
			{needlevel=2,isZs=1,typeId=21010004,itemname="500元宝",bind=0,num=4,gettag=const.PP_TZJH_YR_LQJL2},
			{needlevel=3,isZs=1,typeId=21010004,itemname="500元宝",bind=0,num=4,gettag=const.PP_TZJH_YR_LQJL3},
			{needlevel=4,isZs=1,typeId=21010004,itemname="500元宝",bind=0,num=5,gettag=const.PP_TZJH_YR_LQJL4},
			{needlevel=5,isZs=1,typeId=21010004,itemname="500元宝",bind=0,num=5,gettag=const.PP_TZJH_YR_LQJL5},
			{needlevel=7,isZs=1,typeId=21010004,itemname="500元宝",bind=0,num=5,gettag=const.PP_TZJH_YR_LQJL6},
		},
	},
	[2]={
		cmd="my",needvcoion=2888,investtag=const.PP_TZJH_HAS_INVEST_TAG2,awardlist={
			{needdays=1,typeId=23140004,itemname="超级成就令牌",bind=1,num=1,gettag=const.PP_TZJH_MY_LQJL1},
			{needdays=2,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL2},
			{needdays=3,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL3},
			{needdays=4,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL4},
			{needdays=5,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL5},
			{needdays=6,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL6},
			{needdays=7,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL7},
			{needdays=8,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL8},
			{needdays=9,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL9},
			{needdays=10,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL10},
			{needdays=11,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL11},
			{needdays=12,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL12},
			{needdays=13,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL13},
			{needdays=14,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL14},
			{needdays=15,typeId=23140004,itemname="超级成就令牌",bind=1,num=2,gettag=const.PP_TZJH_MY_LQJL15},
		},
	},
	[3]={
		cmd="xh",needvcoion=888,investtag=const.PP_TZJH_HAS_INVEST_TAG3,awardlist={
			{needdays=1,typeId=21000008,itemname="1388绑元",bind=1,num=1,gettag=const.PP_TZJH_XH_LQJL1},
			{needdays=2,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL2},
			{needdays=3,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL3},
			{needdays=4,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL4},
			{needdays=5,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL5},
			{needdays=6,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL6},
			{needdays=7,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL7},
			{needdays=8,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL8},
			{needdays=9,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL9},
			{needdays=10,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL10},
			{needdays=11,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL11},
			{needdays=12,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL12},
			{needdays=13,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL13},
			{needdays=14,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL14},
			{needdays=15,typeId=21000002,itemname="50绑元",bind=1,num=5,gettag=const.PP_TZJH_XH_LQJL15},
		},
	},
}

function handlePanelData(player,commond)
	if util.ppn(player, const.PP_REDDOT_INVEST) == 0 then
		player:set_param(const.PP_REDDOT_INVEST, 1);
		gui.moduleRedPoint.checkInvestUsable(player);
	end

	local data = util.decode(commond)
	if data.actionid == "fresh" then
		freshPanel(player)
	elseif data.actionid == "initYr" then
		initPanel(player,1);
	elseif data.actionid == "initMy" then
		initPanel(player,2);
	elseif data.actionid == "initXh" then
		initPanel(player,3);
	elseif data.actionid == "investyr" then
		invest(player,1);initPanel(player,1);
	elseif data.actionid == "investmy" then
		invest(player,2);initPanel(player,2);
	elseif data.actionid == "investxh" then
		invest(player,3);initPanel(player,3);
	elseif data.actionid == "getawardyr" then
		getAward(player,1,data.tag);initPanel(player,1);
	elseif data.actionid == "getawardmy" then
		getAward(player,2,data.tag);initPanel(player,2);
	elseif data.actionid == "getawardxh" then
		getAward(player,3,data.tag);initPanel(player,3);
	end
end
ckpanel.add_listener("gui.PanelInvest.handlePanelData",gui.PanelInvest.handlePanelData)

function getAward(player,num,index)
	local awardData = awardTab[num];
	local isinvest = util.ppn(player,awardData.investtag);
	if isinvest == 0 then
		util.alert_msg(4, "未投资，怎能不劳而获！", player)
		return;
	end
	if index < 0 or index > #awardData.awardlist then return end
	local data = awardData.awardlist[index];
	local tishimsg = "";
	if num ==  1 then
		local mLevel = player:get_level();
  		local mZslevel = util.ppn(player,const.PP_ZHSH_LEVEL);
		if data.isZs == 0 then
			tishimsg = "等级";
			if mLevel < data.needlevel then
				util.alert_msg(4, "无法领取奖励，等级不足"..data.needlevel.."级", player)
				return;
			end
		elseif data.needlevel > 0 then
			tishimsg = "转生";
			if mZslevel < data.needlevel then
				util.alert_msg(4, "无法领取奖励，转生不足"..data.needlevel.."级", player)
				return;
			end
		end
	else
		local buyday = getBuyDay(util.ppn(player,awardData.investtag));
		if buyday <= 0 then return	end
		local daynum = today() - buyday + 1;
		if daynum < data.needdays then
			util.alert_msg(4, "无法领取奖励，投资不足"..data.needdays.."天", player)
			return;
		end
	end
	local isGot = util.ppn(player,data.gettag);
	if isGot ~= 1 then
		local needbagnum = util.getNeedBags(player,data.typeId,data.num);
		if player:num_bag_black()>=needbagnum then
			player:add_item(data.typeId,data.num,data.bind);
			player:set_param(data.gettag,1);
			gui.moduleRedPoint.checkInvestUsable(player)
			if num == 1 then
				if data.needdays then
					util.alert_msg(4, "您已领取[曜日礼包]"..tishimsg..data.needdays.."级奖励！", player)
				end
			else
				local libaoname = "";
				if num == 2 then
					libaoname = "明月";
				elseif num == 3 then
					libaoname = "星辉";
				end
				util.alert_msg(4, "您已领取["..libaoname.."礼包]第"..data.needdays.."天奖励！", player)
			end
			if isIconShow(player) then
				freshPanel(player)
			else
				player:push_lua_table("close",util.encode({name = "extend_invest"}));
				gui.moduleGuiButton.checkOpenFunc(player);
			end
		else
			util.alert_msg(4, "您的背包格子数不足，请先清理背包！", player)
		end
	else
		util.alert_msg(4, "该奖励只能领取一次！", player)
	end
end

function invest(player,num)
	local data = awardTab[num];
	if util.ppn(player,data.investtag) > 0 then
		player:alert(1, 1, "您已投资,系统未开放重复投资功能!")
		return;
	end
	if player:get_vcoin() >= data.needvcoion then
		if num == 1 then
			if util.svn(data.remainnum) < data.max_num then
				if util.ppn(player, const.PP_VIP_LEVEL) >= 1 then
					server.set_var(data.remainnum,util.svn(data.remainnum)+1);
				else
					player:alert(1, 1, "投资曜日礼包需要VIP1级！")
					return;
				end
			else
				player:alert(1, 1, "曜日礼包投资已结束！")
				return;
			end
		end
		player:set_param(data.investtag,os.time());
		player:sub_vcoin(data.needvcoion, "投资计划"..data.cmd)

		player:alert(1, 1, "投资计划,消耗"..data.needvcoion.."元宝")

	else
		player:alert(1, 1, "您的元宝不足,无法投资")
	end
end
function initPanel(player,num,noReplace)
	local awardTab = util.clone(awardTab[num]);
	local result = {cmd = awardTab.cmd,childCmd = "updateList"}
	for i=1,#awardTab.awardlist do
		local data = awardTab.awardlist[i];
		data.getState = util.ppn(player,data.gettag);
	end
	result.table = awardTab.awardlist;
	result.noReplace = noReplace;
	result.investState = util.ppn(player,awardTab.investtag) > 0 and 1 or 0;
	if num == 1 then
		result.remainnum = awardTab.max_num - util.svn(awardTab.remainnum);
	elseif num == 2 or num == 3 then
		local buyday = getBuyDay(util.ppn(player,awardTab.investtag));
		if buyday <= 0 then 
			result.buydates = 0;	
		else
			result.buydates = today() - buyday + 1;
		end
	end
	player:push_lua_table("PanelInvest",util.encode(result))
end



function getBuyDay(st)
	if st ~= "" then
		return get_day(tonumber(st));
	end
	return 0;
end

function freshPanel(player)
	initPanel(player,1);
	-- local result = {}
	-- result.cmd = "fresh";

	-- result.data = tab.invest.getTypeData();
	-- for i,v in ipairs(result.data) do
	-- 	local buyTime = util.ppn(player, const["PP_INVEST_BUY"..v.id])
	-- 	v.buy = buyTime>0;
	-- 	v.hasGotTimes = util.ppn(player, const["PP_INVEST_GOT"..i])

	-- 	v.red = v.buy and util.ppn(player, const["PP_INVEST_IS_GOT"..i])==0
	-- end
	-- player:push_lua_table("PanelInvest",util.encode(result));
end

function buyInvestment( player, idx)
	local typedata = tab.invest.getTypeData(idx)
	if typedata then
		local buyTime = util.ppn(player, const["PP_INVEST_BUY"..idx])
		if buyTime==0 then
			if typedata.MoneyKind == 102 then
				if player:sub_vcoin(typedata.MoneyValue,"投资计划") == 1 then
					player:set_param(const["PP_INVEST_BUY"..idx],os.time())
					freshPage(player, idx)
					gui.moduleRedPoint.checkInvestUsable(player)
				else
					showChongzhi(player)
				end
			end
		else
			local isGot = util.ppn(player, const["PP_INVEST_IS_GOT"..idx])
			local hasGotTimes = util.ppn(player, const["PP_INVEST_GOT"..idx])
			if isGot==0 then
				local cdata = tab.invest.getAwardDataByIndex(idx,hasGotTimes)
				if #cdata>0 then
					local bagnum = 0
					for k,v in pairs(cdata) do
						for i=1,#v.ItemBaseID do
							bagnum = bagnum + util.getNeedBags(player,v.ItemBaseID[i],v.ItemNum[i])
						end
					end
					if player:num_bag_black()>=bagnum then
						for k,v in pairs(cdata) do
							for i=1,#v.ItemBaseID do
								player:add_item(v.ItemBaseID[i],v.ItemNum[i],2-v.ItemBind[i])
							end
						end
						player:set_param(const["PP_INVEST_GOT"..idx],hasGotTimes+1)
						player:set_param(const["PP_INVEST_IS_GOT"..idx],1)
						gui.moduleRedPoint.checkInvestUsable(player)

						util.alert_msg(4, "您已领取["..typedata.Name.."]第"..(hasGotTimes+1).."天奖励！", player)
						if hasGotTimes+1 == 15 then
							if isIconShow(player) then
								freshPanel(player)
							else
								player:push_lua_table("close",util.encode({name = "extend_invest"}));
								gui.moduleGuiButton.checkOpenFunc(player);
							end
						else
							freshPage(player,idx)
						end
					else
						util.alert_msg(4, "您的背包已满，请先清理背包！", player)
					end
				else
					util.alert_msg(4, "当前无奖励可领取！", player)
				end
			else
				util.alert_msg(4, "今日已领取，请明天再来！", player)
			end
		end
	end
end

function freshPage(player,pageIdx)
	-- local typedata = tab.invest.getTypeData(pageIdx)
	-- if typedata then
	-- 	local buyTime = util.ppn(player, const["PP_INVEST_BUY"..pageIdx])
	-- 	local hasGotTimes = util.ppn(player, const["PP_INVEST_GOT"..pageIdx])
	-- 	local buy = getBuyDay(buyTime)>0

	-- 	local cdata = tab.invest.getAwardDataByIndex(pageIdx,hasGotTimes)
	-- 	if #cdata>0 then
	-- 		local result = {}
	-- 		result.cmd = "freshPage";
	-- 		result.buy = buy
	-- 		result.hasGotTimes = hasGotTimes;
	-- 		result.data = cdata[1]
	-- 		result.pageIdx = pageIdx
	-- 		result.isGot = util.ppn(player, const["PP_INVEST_IS_GOT"..pageIdx])
	-- 		player:push_lua_table("PanelInvest",util.encode(result));
	-- 	end

	-- end
end

function isIconShow(player)
	-- local typedata = tab.invest.getTypeData()
	-- for i=1,#typedata do
	-- 	if util.ppn(player, const["PP_INVEST_BUY"..i])==0 or util.ppn(player, const["PP_INVEST_GOT"..i])<15 then
	-- 		return true
	-- 	end
	-- end
	-- return false
	return true;
end

function isRedPointShow(player)
	return true;
	-- if not gui.moduleGuiButton.checkFuncOpened(player, 20191) then return false; end
	-- if util.ppn(player, const.PP_REDDOT_INVEST) == 0 then return true; end
	-- local typedata = tab.invest.getTypeData()
	-- for i=1,#typedata do
	-- 	local buyTime = util.ppn(player, const["PP_INVEST_BUY"..i])
	-- 	if buyTime>0 then
	-- 		local hasGotTimes = util.ppn(player, const["PP_INVEST_GOT"..i])
	-- 		local isGot = util.ppn(player, const["PP_INVEST_IS_GOT"..i])
	-- 		if isGot==0 and hasGotTimes<15 then
	-- 			return true
	-- 		end
	-- 	end
	-- end
	-- return false
end