module(..., package.seeall)

local awardTab = {
	["today"] = {
		con= const.PP_DAILYSIGNIN_RECEIVE_TODAY,
		conAll = const.PP_DAILYSIGNIN_RECEIVE,
		awards = {},
	},
	["cumulative"] = {
		[1] = {
			day=2,
			con=const.PP_DAILYSIGNIN_CUMULATIVE_1,
			awards = {
				-- ["generic"] = {			--区分职业性别的物品
				-- 	["male"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- 	["female"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- },
				["db"] = {	--通用物品
					{typeId = 23060001,	num = 5,bind = 1},
					{typeId = 23150001,	num = 2,bind = 1},
					{typeId = 32000001,	num = 2,bind = 1},
					{typeId = 23100004,	num = 1,bind = 1},
					{typeId = 23080003,	num = 1,bind = 1},
				}
			},
		},
		[2] = {
			day=5,
			con=const.PP_DAILYSIGNIN_CUMULATIVE_2,
			awards = {
				-- ["generic"] = {
				-- 	["male"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- 	["female"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- },
				["db"] = {
					{typeId = 23060001,	num = 5,bind = 1},
					{typeId = 23150001,	num = 8,bind = 1},
					{typeId = 32000002,	num = 2,bind = 1},
					{typeId = 23100004,	num = 1,bind = 1},
					{typeId = 23080003,	num = 1,bind = 1},
				}
			},
		},
		[3] = {
			day=10,
			con=const.PP_DAILYSIGNIN_CUMULATIVE_3,
			awards = {
				-- ["generic"] = {
				-- 	["male"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- 	["female"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- },
				["db"] = {
					{typeId = 23060001,	num = 5,bind = 1},
					{typeId = 23150001,	num =10,bind = 1},
					{typeId = 32000003,	num = 2,bind = 1},
					{typeId = 23100005,	num = 1,bind = 1},
					{typeId = 23080003,	num = 2,bind = 1},
				}
			},
		},
		[4] = {
			day=17,
			con=const.PP_DAILYSIGNIN_CUMULATIVE_4,
			awards = {
				-- ["generic"] = {
				-- 	["male"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- 	["female"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- },
				["db"] = {
					{typeId = 23060001,	num = 8,bind = 1},
					{typeId = 23150001,	num =15,bind = 1},
					{typeId = 32000002,	num = 3,bind = 1},
					{typeId = 23100005,	num = 1,bind = 1},
					{typeId = 23080003,	num = 2,bind = 1},
				}
			},
		},
		[5] = {
			day=25,
			con=const.PP_DAILYSIGNIN_CUMULATIVE_5,
			awards = {
				-- ["generic"] = {
				-- 	["male"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- 	["female"]={
				-- 		["warrior"] ={
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["wizard"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 		["taoist"] = {
				-- 			{typeId = 11109011,	name = "雷霸",		num = 1},
				-- 			{typeId = 11109021,	name = "坚毅战甲",	num = 1},
				-- 		},
				-- 	},
				-- },
				["db"] = {
					{typeId = 23060001,	num = 10,bind = 1},
					{typeId = 23150001,	num = 25,bind = 1},
					{typeId = 32000002,	num = 5, bind = 1},
					{typeId = 23100006,	num = 10,bind = 1},
					{typeId = 21000004,	num = 5, bind = 1},
				}
			},
		}
	}
}

function onPanelData(player, commond)
	local data =util.decode(commond);
	if data.actionid == "init" then
		-- player:set_param(const.PP_DAILYSIGNIN_RECEIVE_TODAY, 0);
		-- player:set_param(const.PP_DAILYSIGNIN_RECEIVE ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_1 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_2 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_3 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_4 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_5 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_6 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_7 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_8 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_9 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_10 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_11 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_12 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_13 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_14 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_15 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_16 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_17 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_18 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_19 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_20 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_21 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_22 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_23 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_24 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_25 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_26 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_27 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_28 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_29 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_30 ,0)
		-- player:set_param(const.PP_DAILYSIGNIN_CUMULATIVE_31 ,0)
		updateList(player);
		getReceiveDay(player);
		updateReceiveButton(player);
	elseif data.actionid == "receiveCumulative" then
		receiveCumulativeAward(player,data.tag)
	elseif data.actionid == "receiveToday" then
		receiveTodayAward(player)

	end
end

ckpanel.add_listener("gui.PanelDailySignIn.onPanelData",gui.AwardHall_daily.onPanelData)

function getAwardsTab(player,aType,tag)
	local awardTab = util.clone(awardTab[aType]);
	local needAward = awardTab[tag];
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
		for k,v in pairs(awardGeneric) do
			nowAward[#nowAward+1] = v;
		end
	end
	return nowAward
	
end

function updateList(player,noReplace)
	local result = {cmd = "daily",childCmd = "updateList"}
	local awardTab = util.clone(awardTab["cumulative"]);
	local receiveNum = util.ppn(player, const.PP_DAILYSIGNIN_RECEIVE);
	for k,v in pairs(awardTab) do
		local thisReceiveNum = util.ppn(player, v.con);
		local receiveState;
		if thisReceiveNum>0 then
			receiveState = thisReceiveNum;
		else
			receiveState = (receiveNum>=v.day) and -1 or 0
		end
		local nowAward = getAwardsTab(player,"cumulative",k)
		v.awards = nowAward;
		v.state = receiveState;
	end
	result.table = awardTab;
	result.noReplace = noReplace;
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

function getReceiveDay(player)
	local result = {cmd = "daily",childCmd = "updateReceive"}
	result.receiveNum = util.ppn(player, const.PP_DAILYSIGNIN_RECEIVE)
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

function updateReceiveButton(player)
	local result = {cmd = "daily",childCmd = "updatereceiveBtn"}
	result.receiveBtn = util.ppn(player, const.PP_DAILYSIGNIN_RECEIVE_TODAY)
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

function receiveCumulativeAward(player,tag)
	if tag and tag>0 then
		local awards = util.clone(awardTab["cumulative"]);
		local nowAward = awards[tag];
		if nowAward then
			if util.ppn(player, nowAward.con) <=0 then
				local receiveNum = util.ppn(player, const.PP_DAILYSIGNIN_RECEIVE);
				if receiveNum>=nowAward.day then
					local bagnum = player:num_bag_black();
					local needbags = 0;
					local nowAwardTab = getAwardsTab(player,"cumulative",tag)
					for i,v in ipairs(nowAwardTab) do
						needbags = needbags +util.getNeedBags(player,v.typeId,v.num);
					end
					if needbags<=bagnum then
						player:set_param(nowAward.con, util.ppn(player, nowAward.con)+1);
						for i,v in ipairs(nowAwardTab) do
							player:add_item(v.typeId,v.num,v.bind);
							-- player:alert(1, 1, "已成功领取"..v.num.."个"..v.name);
						end
						local result = {cmd = "daily",childCmd = "updateListBtn",tag = tag}
						result.state = util.ppn(player, nowAward.con);
						player:push_lua_table("PanelRewardHall",util.encode(result))
						gui.moduleRedPoint.checkAwardSignUsable(player);
					else
						player:alert(1, 1, "背包空间不足，请清理背包后重试！");
					end
				else
					player:alert(1, 1, "未达到累计签到次数，不可领取！");
				end
			else
				player:alert(1, 1, "您已领取过该奖励，不可重复领取！");
			end
		end
	end
end

function receiveTodayAward(player)
	local award = util.clone(awardTab["today"]);
	if award then
		if util.ppn(player, award.con) <=0 then
			player:set_param(award.con, util.ppn(player, award.con)+1);
			player:set_param(award.conAll, util.ppn(player, award.conAll)+1);
			player:alert(1, 1, "您今天已成功签到！");
			updateList(player,true);
			getReceiveDay(player);
			updateReceiveButton(player);
			gui.moduleRedPoint.checkAwardSignUsable(player);
		else
			player:alert(1, 1, "您今天已签过到，不可重复签到！");
		end
	end
end

--签到红点检测
function checkRedPoint(player)
	local award = util.clone(awardTab["today"])
	if award and util.ppn(player, award.con)<=0 then--可签到
		return true
	end
	local receiveNum = util.ppn(player, const.PP_DAILYSIGNIN_RECEIVE)--累计签到次数
	local awardTable = util.clone(awardTab["cumulative"])
	for i=1,#awardTable do
		local itemData = awardTable[i]
		if util.ppn(player,itemData.con)<=0 and receiveNum>=itemData.day then--有累计奖励可以领取
			return true
		end
	end
	return false
end