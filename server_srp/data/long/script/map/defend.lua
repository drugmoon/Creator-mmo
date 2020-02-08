--守护女神
module(..., package.seeall)
--关卡数和怪物刷新波数和底图挂钩

local mapID = "nvshen";
local statueDefId = 304001;

local statueX = 22;
local statueY = 32;

local genPos = {
	{x = 15, y = 26},
	{x = 29, y = 26},
	{x = 29, y = 39},
	{x = 15, y = 39},
};

-- local genX = 39;
-- local genY = 44;
--限时20分钟
local limitTime = 20;
local npcId = 4002006;

local slaveGen = {
	{x = 22, y = 29},
	{x = 21, y = 30},
	{x = 18, y = 33},
	{x = 20, y = 35},
	{x = 22, y = 37},
	{x = 24, y = 35},
	{x = 26, y = 33},
	{x = 24, y = 31},
	{x = 21, y = 34},
	{x = 20, y = 32},
}

function onPlayerEnter(map, player)
	local statusID = player:gen_slave_statue(statueDefId, statueX, statueY, "dirfixed(4),");
	map:set_var("statue_id", statusID);

	genMon(map);
	pushDefendInfo(player, true);

	-- map:set_timer(1, limitTime * 60 * 1000);
	player:set_timer(24, limitTime * 60 * 1000);
end

function closeDefendResult(player)
	player:push_lua_table("closeTips", util.encode({name = "defendResult"}));
	posnpc.fly(player, npcId);
end

--定时out
function onTimerOut(player)
	local map = player:get_map();
	if map and map:get_def_id() == mapID then
		if map:num_monster() > 0 then
			map:clear_mon();
			gui.PanelDefend.getDefendAward(player);
		else
			closeDefendResult(player);
		end
	end
end

function onTimer1(map)

end

function initMapVar(map)
	map:set_var("call_fashi_count", "");
	map:set_var("call_fashi_free_count", "");
	map:set_var("last_call_fashi_time", "");

	map:set_var("inspire_count", "");
	map:set_var("inspire_free_count", "");
	map:set_var("last_inspire_time", "");
end

--此处为玩家手动点击进入副本（后期要加玩家下线保留副本）
function enterMap(player, level)
	local maxLevel = util.ppn(player, const.PP_MAX_DEFEND_LEVEL);
	if maxLevel + 1 < level then return player:alert(1, 1, "当前关卡不可挑战!"); end

	local map = server.find_map("nvshen");
	if map then
		initMapVar(map);

		map:clear_item();
		map:clear_mon();
		player:clear_slave_fashi();

		map:set_var("monster_level", level);
		--print("1111111")
		-- local x, y = map:gen_rand_position();
		player:enter_map("nvshen", 24, 33);

		player:push_lua_table("close", util.encode({name = "panel_defend"}));

		player:set_param(const.PP_DEFEND_TIMES, util.ppn(player, const.PP_DEFEND_TIMES) + 1);
		task.simulateTask.showTaskDesp(player, 5001);
	else
		player:alert(1, 1, "挑战守护女神副本人数太多，请稍后重试");
	end
end

function onPlayerLeave(map, player)

	-- map:set_timer(1,0)
	player:set_timer(24, 0);

	map:set_var("statue_id", "");
	map:set_var("monster_level", "");
	map:set_var("monster_wave", "");
	map:clear_mon();
	player:clear_slave_fashi();

	--buff清理
	player:clear_buff(38001);
	player:clear_buff(38002);

	player:alert(1, 1, "离开守护女神");
end

function isInDefenMap(player)

end

--挑战成功
function onChallengeSucceed(player, level, wave)
	local param = {
		name = "defendResult",
		success = true,
		successType = "challenge",
		awards = tab.getDefendNormalAward(level, 1, wave - 1),
	}
	player:push_lua_table("openTips", util.encode(param));

	--记录奖励变量

	local maxLevel = util.ppn(player, const.PP_MAX_DEFEND_LEVEL);
	if level > maxLevel then
		player:set_param(const.PP_MAX_DEFEND_LEVEL, level);
	end
end

function onMonKill(player, mon)
	local map = player:get_map();
	if not map then return; end
	-- local monConf = getMonsterConfig(map, level, wave);
	-- if not monConf then return; end

	if map:get_def_id() == mapID then
		if map:num_monster() == 0 then
			local level = util.mvn(map, "monster_level");
			local wave = util.mvn(map, "monster_wave");
			--每波打完都记录奖励
			if wave > 0 then player:set_param(const.PP_DEFEND_AWARD_INDEX, level * 10 + wave); end

			if wave  < 10 then
				genMon(map);
				pushDefendInfo(player);
			else
				local level = util.mvn(map, "monster_level");
				onChallengeSucceed(player, level, wave);
			end
		end
	end
end

function showFailure(player, map)
	map = map or player:get_map();
	if not map then return; end
	map:clear_mon();
	local level = util.mvn(map, "monster_level");
	local wave = util.mvn(map, "monster_wave");
	local param = {
		name = "defendResult",
		awards = tab.getDefendNormalAward(level, 1, wave - 1),
	}
	player:push_lua_table("openTips", util.encode(param));
end

function onSlaveDead(slave)
	local map = slave:get_map();
	if not map then return; end
	if slave:get_id() == util.mvn(map, "statue_id") then
		--失败
		local player = slave:get_owner();
		if player then
			showFailure(player, map)
		end
	end
end

function getMonsterConfig(map, level, wave)
	if not map then return; end
	level = level or util.mvn(map, "monster_level") - 1;
	wave = wave or util.mvn(map, "monster_wave");
	local index = level * 10 + wave;
	-- print("///////getMonsterConfig///////", index);
	if not tab.defendMonster[index] then return; end
	return tab.defendMonster[index];
end

--刷新怪物
function genMon(map)
	local wave = util.mvn(map, "monster_wave");
	local level = util.mvn(map, "monster_level");
	if wave < 10 then
		wave = wave + 1;
		map:set_var("monster_wave", wave);
		local monConf = getMonsterConfig(map, level - 1, wave);
		if monConf then
			-- print("///////genMon/////")
			-- map:mon_gen();
			local statueID = util.mvn(map, "statue_id");
			-- local monster;
			local index = 1;
			for monId,v in pairs(monConf) do
				for i=1,v do
					map:mon_gen(genPos[index].x, genPos[index].y, monId, 1, 1, 1, 1);
					index = index + 1;
					if index > 3 then
						index = index - 3;
					end
				end
				-- monster:lock_target(statueID);
			end
			map:mon_public_target(statueID);
		end
	else
		--成功
	end
end

ckmon.add_all_listener(onMonKill);
--主界面左侧关卡信息

function pushDefendInfo(player, showTime)
	local map = player:get_map();
	if not map then return; end
	local mapid = map:get_def_id();
	if not map == "" then return; end
	local result = {};
	local level = util.mvn(map, "monster_level");
	local wave = util.mvn(map, "monster_wave");
	result.statueId = util.mvn(map, "statue_id");
	result.statueName = tab.getMonName(statueDefId)
	result.levelName = "第"..level.."关,第"..wave.."波";
	result.awards = tab.getDefendNormalAward(level, 1, wave - 1);
	if showTime then
		result.remainTime = limitTime * 60;
		result = getInspireState(player, result, map, true);
		result = getSlaveState(player, result, map, true);
	end
	player:push_lua_table("defendInfo", util.encode(result))
end

function onRefreshMon(player, map)
	local result = {};
	local level = util.mvn(map, "monster_level");
	local wave = util.mvn(map, "monster_wave");
	result.levelName = "第"..level.."关,第"..wave.."波";
	result.awards = tab.getDefendNormalAward(level, 1, wave - 1);
	player:push_lua_table("defendInfo", util.encode(result));
end

function onClientData(player, commond)
	-- print("/////////onClientData///////////", commond)
	local data =util.decode(commond);
	if not data then return; end
	if data.actionid == "exit" then
		exitDefend(player);
	elseif data.actionid == "addPower" then
		addPower(player);
	elseif data.actionid == "callSlave" then
		callSlave(player);
	elseif data.actionid == "confirmExit" then
		-- print("11111,", npcId)
		-- player:set_param(const.PP_SHOW_DEFEND_RESULT , 1);
		-- posnpc.fly(player, npcId);
		showFailure(player);
	end
end


local price = 100;
local freeNum = 1;
local maxInspireNum = 2;
local maxFashiNum = 10;
local inspireCD = 3;
local callFashiCD = 6;

-- inspireCost
-- inspireCD
-- slaveCost
-- slaveCD

function getInspireState(player, result, map, first)
	map = map or player:get_map();
	if (not map) or map:get_def_id() ~= mapID then
		return;
	end
	result = result or {}
	result.updateInspire = true;
	local freeCount = util.mvn(map, "inspire_free_count");
	if freeCount < freeNum then
		result.inspireCost = 0;
	else
		result.inspireCost = price;
	end
	if first then
		result.inspireCD = 0;
	else
		result.inspireCD = inspireCD;
	end
	return result;
	-- player:push_lua_table("defendInfo", util.encode(result));
end

function getSlaveState(player, result, map, first)
	map = map or player:get_map();
	if (not map) or map:get_def_id() ~= mapID then
		return;
	end
	result = result or {}

	result.updateSlave = true;
	local freeCount = util.mvn(map, "call_fashi_free_count");
	if freeCount < freeNum then
		result.slaveCost = 0;
	else
		result.slaveCost = price;
	end
	if first then
		result.slaveCD = 0;
	else
		result.slaveCD = callFashiCD;
	end
	return result;
	-- player:push_lua_table("defendInfo", util.encode(result));
end

--加战力buf
function addPower(player)
	-- body
	-- 免费 38001
	local map = player:get_map();
	if (not map) or map:get_def_id() ~= mapID then return; end
	local inspireCount = util.mvn(map,"inspire_count");
	if inspireCount >= maxInspireNum then return player:alert(1,1,"鼓舞次数已达上限"); end

	local lastInspireTime = util.mvn(map, "last_inspire_time");
	if lastInspireTime + inspireCD > os.time() then return player:alert(1, 1, "鼓舞冷却中！") end

	local freeCount = util.mvn(map, "inspire_free_count");

	if freeCount < freeNum or player:get_vcoin() >= price then
		if freeCount < freeNum  then
			freeCount = freeCount + 1;
			map:set_var("inspire_free_count", freeCount);
			player:set_buff(38001);
		else
			player:sub_vcoin(price, "守护女神鼓舞");
			player:set_buff(38002);
		end
		inspireCount = inspireCount + 1
		map:set_var("inspire_count", inspireCount);
		player:alert(1,1,"最多只能鼓舞"..maxInspireNum.."次,剩余召唤"..(maxInspireNum-inspireCount).."次");
		map:set_var("last_inspire_time", os.time());

		local result = {}
		result = getInspireState(player, result, map);
		player:push_lua_table("defendInfo", util.encode(result));
	else
		return player:alert(1,1,"元宝不足,无法鼓舞");
	end
end

-- 304002
function callSlave(player)
	-- 刷新界面
	local map = player:get_map();
	if (not map) or map:get_def_id() ~= mapID then return; end
	local callCount = util.mvn(map,"call_fashi_count");
	if callCount >= maxFashiNum then return player:alert(1,1,"召唤次数已达上限"); end

	local lastCallTime = util.mvn(map, "last_call_fashi_time");
	if lastCallTime + callFashiCD > os.time() then return player:alert(1, 1, "召唤守卫冷却中！") end

	local freeCount = util.mvn(map, "call_fashi_free_count");
	if freeCount < freeNum or player:get_vcoin() >= price then
		local slaveId = 304002;
		local level = util.mvn(map, "monster_level");
		local levelInfo = tab.defendFirstAward[level];
		if not levelInfo then return; end
		slaveId = levelInfo.Magic or slaveId;

		-- print("callSlave and slaveId is: ", slaveId)

		local slaveX = 0;
		local slaveY = 0;
		if slaveGen[callCount + 1] then
			slaveX = slaveGen[callCount + 1].x;
			slaveY = slaveGen[callCount + 1].y;
		end

		if freeCount < freeNum  then
			freeCount = freeCount + 1;
			map:set_var("call_fashi_free_count", freeCount);
			player:gen_slave_fashi(slaveId, slaveX, slaveY, "nomove,nofire,");
		else
			player:sub_vcoin(price, "召唤宝石守卫");
			player:gen_slave_fashi(slaveId, slaveX, slaveY, "nomove,nofire,");
		end
		callCount = callCount + 1
		map:set_var("call_fashi_count", callCount);
		player:alert(1,1,"最多只能召唤"..maxFashiNum.."次,剩余召唤"..(maxFashiNum-callCount).."次");
		map:set_var("last_call_fashi_time", os.time());

		local result = {}
		result = getSlaveState(player, result, map);
		player:push_lua_table("defendInfo", util.encode(result));
	else
		return player:alert(1,1,"元宝不足,无法召唤法师");
	end
end

function exitDefend(player)
	local map = player:get_map();
	if not map then return; end
	local mapid = map:get_def_id();
	if mapid == "nvshen" then
		-- player:go_home();
		local result = {
			name = "confirm",
			lblConfirm = "是否退出当前副本",
			confirmTitle = "确定",
			cancelTitle = "取消",
			svrPath = "map.defend.onClientData",
			confirmAction = "confirmExit",
			cancelAction = "cancelExit",
		};

		player:push_lua_table("openTips", util.encode(result));
	end
end

ckpanel.add_listener("map.defend.onClientData", map.defend.onClientData)