module(..., package.seeall)

local awardTab = {
	con=const.PP_FIRST_RECHARGE_AWARD,
	awards = {
		["generic"] = {
			["male"]={
				["warrior"] ={
					{typeId = 11107011,	num = 1,mLevel = 8,bind = 1,effect= 65081},
				},
				["wizard"] = {
					{typeId = 12107011,	num = 1,mLevel = 8,bind = 1,effect = 65082},
				},
				["taoist"] = {
					{typeId = 13107011,	num = 1,mLevel = 8,bind = 1,effect = 65083},
				},
			},
			["female"]={
				["warrior"] ={
					{typeId = 11107011,	num = 1,mLevel = 8,bind = 1,effect= 65081},
				},
				["wizard"] = {
					{typeId = 12107011,	num = 1,mLevel = 8,bind = 1,effect = 65082},
				},
				["taoist"] = {
					{typeId = 13107011,	num = 1,mLevel = 8,bind = 1,effect = 65083},
				},
			},
		},
		["db"] = {
			{typeId = 27000002,	num = 1,bind = 1},
			{typeId = 23100008,	num = 1,bind = 1},
			{typeId = 23040001,	num = 1,bind = 1},
		},
	},
}

function getAwardsTab(player)
	-- local pos = player:add_item2(12107011,1,1);
	-- print(12107011,pos)
	local needAward = util.clone(awardTab);
	local awards = needAward.awards;
	if (not awards["db"]) and (not awards["generic"]) then
		return awards
	end
	local gender = player:get_gender_name();
	local job = player:get_job_name();
	local awardDB = awards["db"];
	local awardGeneric=nil;
	if awards["generic"] and awards["generic"][gender] and awards["generic"][gender][job] then
		awardGeneric= awards["generic"][gender][job];
	end
	local nowAward = awardDB;
	if awardGeneric then
		-- print(util.encode(awardGeneric[1]))
		-- for k,v in pairs(awardGeneric) do
		-- 	nowAward[#nowAward+1] = v;
		-- end
		table.insert(nowAward, 1, awardGeneric[1])
	end
	return nowAward
	
end

function onPanelData(player, commond)
	local data =util.decode(commond);
	if data.actionid == "init" then
		-- player:set_param(const.PP_SUM_RECHARGE, util.ppn(player, const.PP_SUM_RECHARGE)+10000);
		update(player);

	elseif data.actionid == "receive" then
		receive(player)
	end
end

ckpanel.add_listener("gui.PanelFirstRecharge.onPanelData",gui.PanelFirstRecharge.onPanelData)

function update(player)
	local result = {cmd = "init"}
	local rechargeTab = util.clone(awardTab);
	local rechargeNum = util.ppn(player, const.PP_SUM_RECHARGE);
	local isReceive = util.ppn(player, rechargeTab.con);
	local receiveState;
	if rechargeNum>0 then
		receiveState = isReceive;
	else
		receiveState = -1
	end
	result.table = {};
	result.table.state = receiveState;
	result.table.awards = getAwardsTab(player)
	player:push_lua_table("PanelFirstRecharge",util.encode(result))
end

function receive(player)
	local awards = util.clone(awardTab);
	if util.ppn(player, awards.con) <=0 then
		local rechargeNum = util.ppn(player, const.PP_SUM_RECHARGE);
		-- print("SXXXSWDSSHSJKH",rechargeNum)
		if rechargeNum>0 then
			local awardsTab = getAwardsTab(player)
			local bagnum = player:num_bag_black();
			local needbags = 0;
			for i,v in ipairs(awardsTab) do
				needbags = needbags +util.getNeedBags(player,v.typeId,v.num);
			end
			if needbags<=bagnum then
				player:set_param(awards.con, util.ppn(player, awards.con)+1);
				for i,v in ipairs(awardsTab) do
					if v.mLevel then
						local pos = player:add_item2(v.typeId,v.num,v.bind);
						player:set_item_uplv(v.typeId,pos,v.mLevel)
					else
						player:add_item(v.typeId,v.num,v.bind);
					end
					-- player:alert(1, 1, "已成功领取"..v.num.."个"..v.name);
				end
				update(player);
				gui.moduleRedPoint.checkFirstRechargeUsable(player);
				gui.moduleGuiButton.checkOpenFunc(player);
				player:push_lua_table("close",util.encode({name = "extend_firstPay"}))
				server.info(10000,0,"玩家<font color='#00ccff'>"..player:get_name().."</font>成功领取价值8888元宝<font color='#ff0000'>【首充大礼】</font>,实力猛增！")
				server.info(10, 1, "<font color='#fff843'>玩家<font color='#00ccff'>"..player:get_name().."</font>成功领取价值8888元宝<font color='#ff0000'>【首充大礼】</font>,实力猛增！<a color='#00ff00' href='event:local_SeverOpen_5' underline='#00ff00'>我要领取</a></font>")
			else
				player:alert(1, 1, "背包空间不足，请清理背包后重试！");
			end
		else
			player:alert(1, 1, "您还未进行过充值，请充值！");
		end
	else
		player:alert(1, 1, "您已领取过该奖励，不可重复领取！");
	end
end

--检测首充领取状态
function checkRedPointRecharge(player)
	if util.ppn(player, awardTab.con)<=0  then
		return true
	end
	return false
end

--已经充值，且未领取奖励
function checkRedPointAward(player)
	if util.ppn(player, const.PP_SUM_RECHARGE) > 0 and util.ppn(player, awardTab.con)<=0  then
		return true
	end
	return false
end