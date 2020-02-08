-- PanelDefend.lua(守护女神)
module(..., package.seeall)


local posIconRes = {
	-- [-2] = {res = "img_icon_weapon"}
	[-4] = {res = "img_icon_cloth"},
	[-6] = {res = "img_icon_hat"},
	[-8] = {res = "img_icon_necklace"},
	[-10] = {res = "img_icon_glove"},
	[-11] = {res = "img_icon_glove"},
	[-12] = {res = "img_icon_ring"},
	[-13] = {res = "img_icon_ring"},
	[-14] = {res = "img_icon_belt"},
	[-16] = {res = "img_icon_boot"},
}

local needLevel = 85;


function onPanelData(player, commond)
	-- print("/////////onPanelData///////////", commond)
	local data =util.decode(commond);
	if not data then return; end

	if data.actionid == "updatePanel" then
		updatePanel(player, data.level);
	elseif data.actionid == "challenge" then
		challengeDefend(player, data.level);
	elseif data.actionid == "sweep" then
		sweepDefend(player, data.level);
	elseif data.actionid == "getAward" then
		if data.awardType == "sweep" then
			getSweepAward(player, data.level);
		else
			getDefendAward(player);
		end
	elseif data.actionid == "firstAward" then
		--首次通关奖励
		getFirstAward(player, data.level);
	end
end

-- return: 0(null), 1(uncollected),2(collected)
function getFirstAwardState(player, level)
	local gotAwards = player:get_param(const.PP_DEFEND_FIRST_AWARD);
	if gotAwards == "" then 
		gotAwards = {};
	else
		gotAwards = util.decode(gotAwards);
	end
	local maxLevel = util.ppn(player, const.PP_MAX_DEFEND_LEVEL);
	if maxLevel < level then 
		return 0; 
	end
	if table.indexof(gotAwards, level) then 
		return 2; 
	end
	return 1, gotAwards;
end

-- return: 0(已通关), 1(可挑战), 2(不可挑战)
function getChallengeState (player, level)
	local maxLevel = util.ppn(player, const.PP_MAX_DEFEND_LEVEL);
	if level <= maxLevel then
		return 0;
	elseif level == maxLevel + 1 then
		return 1;
	else
		return 2;
	end
end

-- return: 0(不可扫荡), 1(可扫荡)
function getSweepState(player, level)
	local maxLevel = util.ppn(player, const.PP_MAX_DEFEND_LEVEL);
	if level <= maxLevel then
		return 1;
	else
		return 0;
	end
end

function getOpenGemTips(player, level)
	local info = tab.openGem[level];
	if not info then return; end
	return "通关后开启宝石系统的"..info.posName.."部位";
end

function getOpenGemIcon(player, level)
	local info = tab.openGem[level];
	if not info then return; end
	local pos = info.posEnum;
	if not posIconRes[pos] then return; end
	return posIconRes[pos].res;
end

function updatePanel(player, level)
	--level是关卡的意思
	local result = {};
	if not level then 
		level = util.ppn(player, const.PP_MAX_DEFEND_LEVEL) + 1;
		if level > 20 then level = 20; end
		result.level = level;
		--打开面板时候刷新
		updateTabRedDot(player);
	end
	local levelInfo = tab.defendFirstAward[level];
	if not levelInfo then return; end

	result.cmd = "updatePanel";
	result.bossName = "";
	result.remainDesp = "剩余次数："
	result.recommendFight = levelInfo.Power;
	result.remainTimes = const.MAX_DFEND_TIMES - util.ppn(player, const.PP_DEFEND_TIMES);
	result.firstAward = {id = levelInfo.ItemID, count = levelInfo.ItemCount, bind = levelInfo.ItemBindType};
	result.totalAward = levelInfo.noramlTotalAwards;
	result.monId = levelInfo.UnitID;
	result.firstAwardState = getFirstAwardState(player, level);
	result.challengeState = getChallengeState(player, level);
	-- 
	if result.challengeState > 1 then
		result.remainDesp = "需通过："
		result.remainTimes = tab.defendFirstAward[level-1].Name
	end
	result.sweepState = getSweepState(player, level);
	result.openGemTips = getOpenGemTips(player, level);
	result.openGemIcon = getOpenGemIcon(player, level);
	-- result.level = level;
	player:push_lua_table("PanelDefend", util.encode(result));
end

function updateTabRedDot(player)
	local maxLevel = util.ppn(player, const.PP_MAX_DEFEND_LEVEL);

	local gotAwards = player:get_param(const.PP_DEFEND_FIRST_AWARD);
	if gotAwards == "" then 
		gotAwards = {};
	else
		gotAwards = util.decode(gotAwards);
	end
	local redDots = {};
	for i = 1, maxLevel do
		if not table.indexof(gotAwards, i) then
			table.insert(redDots, i);
		end
	end
	player:push_lua_table("PanelDefend", util.encode({cmd = "updateTabRedDot", redDots = redDots}));
end

function challengeDefend(player, level)
	if player:get_level() < needLevel then return player:alert(1, 1, "等级不足"..needLevel.."级,不可挑战！"); end
	if util.ppn(player, const.PP_DEFEND_TIMES) >= const.MAX_DFEND_TIMES then return player:alert(1, 1, "今日挑战次数已用完"); end
	local ret = map.defend.enterMap(player, level);
end

function getFirstAward(player, level)
	local awardState, gotAwards = getFirstAwardState(player, level);
	if awardState == 0 then return player:alert(1, 1, "未通关该重天,无可领取奖励！"); end
	if awardState == 2 then return player:alert(1, 1, "奖励已领取！"); end
	if awardState == 1 then
		if player:num_bag_black() < 1 then return player:alert(1, 1, "背包已满,请清理后再来领取"); end
		local levelInfo = tab.defendFirstAward[level];
		if not levelInfo then return; end
		table.insert(gotAwards, level);
		player:set_param(const.PP_DEFEND_FIRST_AWARD, util.encode(gotAwards))

		player:add_item(levelInfo.ItemID, levelInfo.ItemCount, levelInfo.ItemBindType);
		player:alert(1, 1, "成功领取首次通关奖励！");

		updatePanel(player, level);
		updateTabRedDot(player);
	end
end

function sweepDefend(player, level)
	if player:get_level() < needLevel then return player:alert(1, 1, "等级不足"..needLevel.."级,不可扫荡！"); end

	if util.ppn(player, const.PP_DEFEND_TIMES) >= const.MAX_DFEND_TIMES then return player:alert(1, 1, "今日扫荡次数已用完"); end
	local maxLevel = util.ppn(player, const.PP_MAX_DEFEND_LEVEL);
	if maxLevel < level then return player:alert(1, 1, "尚未通关该关卡,不可扫荡！"); end

	-- local levelInfo = tab.defendFirstAward[level];
	-- if not levelInfo then return; end

	local param = {
		name = "defendResult",
		success = true,
		successType = "sweep",
		defendLevel = level,
		awards = tab.getDefendNormalAward(level, 1, 10),
	}
	player:push_lua_table("openTips", util.encode(param));
end

function getSweepAward(player, level)
	if not level then return; end
	local awards = tab.getDefendNormalAward(level, 1, 10);
	
	if player:num_bag_black() < #awards then
		return player:alert(1, 1, "背包空间不足,请清理后再来扫荡!");
	end

	player:set_param(const.PP_DEFEND_TIMES, util.ppn(player, const.PP_DEFEND_TIMES) + 1);
	for i,v in ipairs(awards) do
		player:add_item(v.id, v.count, v.bind);
	end
	player:alert(1, 1, "成功领取扫荡守护女神奖励！");

	updatePanel(player, level);

	task.simulateTask.showTaskDesp(player, 5001);
end

function getAwardsFromIndex(index)
	local awards = {}
	local level = math.floor(index / 10);
	local wave = index - level * 10;
	if wave == 0 and level > 1 then
		level = level -1;
		wave = 10;
	end
	awards = tab.getDefendNormalAward(level, 1, wave);
	return awards;
end

function getDefendAward(player)
	player:push_lua_table("closeTips", util.encode({name = "defendResult"}));
	posnpc.fly(player, 4002006);

	local awardIndex = util.ppn(player, const.PP_DEFEND_AWARD_INDEX);
	if awardIndex <= 0 then return player:alert(1, 1, "无可领取奖励!"); end
	
	local awards = getAwardsFromIndex(awardIndex);
	
	if player:num_bag_black() < #awards then
		checkDefendAward(player);
		return player:alert(1, 1, "背包空间不足,通过邮件形式发放!");
	end

	-- player:set_param(const.PP_DEFEND_TIMES, util.ppn(player, const.PP_DEFEND_TIMES) + 1);
	-- task.simulateTask.showTaskDesp(player, 5001);
	player:set_param(const.PP_DEFEND_AWARD_INDEX, 0);
	-- print(util.encode(awards));
	for i,v in ipairs(awards) do
		player:add_item(v.id, v.count, v.bind);
	end
	player:alert(1, 1, "成功领取守护女神奖励！");
end

ckpanel.add_listener("gui.PanelDefend.onPanelData",gui.PanelDefend.onPanelData)

function checkDefendAward(player)
	player:set_param(const.PP_SHOW_DEFEND_RESULT, 0);

	local awardIndex = util.ppn(player, const.PP_DEFEND_AWARD_INDEX)
	if awardIndex <= 0 then return; end
	-- print("checkDefendAward", awardIndex)
	-- player:set_param(const.PP_DEFEND_TIMES, util.ppn(player, const.PP_DEFEND_TIMES) + 1);
	-- task.simulateTask.showTaskDesp(player, 5001);

	local awards = getAwardsFromIndex(awardIndex);
	if #awards == 1 then
		player:post_mail("守护女神", "奖励结算", awards[1].id, awards[1].count);
	elseif #awards == 2 then
		player:post_mail("守护女神", "奖励结算", awards[1].id, awards[1].count, awards[2].id, awards[2].count);
	elseif #awards == 3 then
		player:post_mail("守护女神", "奖励结算", awards[1].id, awards[1].count, awards[2].id, awards[2].count, awards[3].id, awards[3].count);
	end
	player:set_param(const.PP_DEFEND_AWARD_INDEX, 0);
end

login.add_login_listener(checkDefendAward)