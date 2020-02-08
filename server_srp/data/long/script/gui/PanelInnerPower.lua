module(..., package.seeall)

local keys = {"id","level","exp","valWarrior","valMage","valPriests","dtr","addvalWarrior","addvalMage","addvalPriests"}
local types = {"int","int","int","int","int","int","int","int","int","int"}

local temp = tab.parseCsvFile(server.load_csv_file("innerPower.csv"),keys,types)
local Data = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		table.insert(Data,v)
	end
end
table.sort(Data,function(a,b)
	return a.id<b.id
end)

local job = {
	["warrior"] = {val = "valWarrior", 	recover= "addvalWarrior"},
	["wizard"] 	= {val = "valMage", 	recover= "addvalMage"},
	["taoist"] 	= {val = "valPriests", 	recover= "addvalPriests"},
}
function getInnerPowerVal(player,lv, tp)
	local playerjob = player:get_job_name()
	if lv >= 0 and lv <= #Data then
		local valname =job[playerjob][tp]
		if valname then
			local d = Data[lv]
			if d and valname then
				return d[valname]
			end
		end
	end
	return 0
end

function onPanelData(player,command)
	local data = util.decode(command)
	if data.actionid == "fresh" then
		freshPanel(player)
	elseif data.actionid == "upgrade" then
		onUpgrade(player)
	elseif data.actionid == "upgradeOnekey" then
		onUpgradeOnekey(player)
	end
end
ckpanel.add_listener("gui.PanelInnerPower.onOpenPanel",gui.PanelInnerPower.onPanelData)

function freshPanel(player,up)
	local result = {}
	-- player:set_param(const.PP_INNERPOWER_LEVEL,0)
	-- player:set_param(const.PP_INNERPOWER_EXP,1110010)
	local level = util.ppn(player,const.PP_INNERPOWER_LEVEL)
	if level <1 then
		level = 1
		player:set_param(const.PP_INNERPOWER_LEVEL,1)
		local exp = getInnerPowerVal(player,1,"val")
		if exp>0 then
			player:set_max_power(exp,1,Data[1].dtr)
		end
		local recover = getInnerPowerVal(player,1, "recover")
		if recover and player.set_power_recover then
			player:set_power_recover(recover)
		end
	end
	local expPool = util.ppn(player,const.PP_INNERPOWER_EXP)
	result.cmd = "update";
	result.Data = Data[level] and util.clone(Data[level]) or Data[1]
	result.level = level;
	result.expPool = expPool;

	result.up = up
	player:push_lua_table("PanelInnerPower", util.encode(result))
end

function onUpgradeOnekey(player)
	local level = util.ppn(player,const.PP_INNERPOWER_LEVEL)
	local expPool = util.ppn(player,const.PP_INNERPOWER_EXP)
	if Data[level] then
		if expPool >= Data[level].exp then
			onUpgrade(player)
			onUpgradeOnekey(player)
		end
	end
end

function onUpgrade(player)
	local level = util.ppn(player,const.PP_INNERPOWER_LEVEL)
	local nextLevel = level +1
	local expPool = util.ppn(player,const.PP_INNERPOWER_EXP)
	if Data[nextLevel] then
		if expPool >= Data[level].exp then
			player:set_param(const.PP_INNERPOWER_EXP,expPool - Data[level].exp)
			player:set_param(const.PP_INNERPOWER_LEVEL,nextLevel)
			ckinnerpowerlevel.triger_inner_power_level(player,nextLevel)
			local exp = getInnerPowerVal(player,nextLevel,"val")
			if exp>0 then
				player:set_max_power(exp,1,Data[nextLevel].dtr)
			end
			local recover = getInnerPowerVal(player,nextLevel, "recover")

			if recover and player.set_power_recover then
				player:set_power_recover(recover)
			end

			gui.moduleRedPoint.checkInnerPowerUsable(player)	
			freshPanel(player,true)
		else
			util.alert_msg(4, "内功值不足！", player)	
		end
	else
		util.alert_msg(4, "内功已升至顶级", player)
	end
end

function setInnerPower(player,tag)
	local level = util.ppn(player,const.PP_INNERPOWER_LEVEL)
	if level>0 and Data[level] then
		local exp = getInnerPowerVal(player,level,"val")
		if exp>0 then
			player:set_max_power(exp,tag,Data[level].dtr)
		end
		local recover = getInnerPowerVal(player,level, "recover")
		if recover and player.set_power_recover then
			player:set_power_recover(recover)
		end
	end
end

function checkUpgradeUsable(player)
	local level = util.ppn(player,const.PP_INNERPOWER_LEVEL)
	-- local nextLevel = level +1
	local expPool = util.ppn(player,const.PP_INNERPOWER_EXP)
	if Data[level] and expPool >= Data[level].exp then
		return true
	end
end

function getInnerPowerLevel(player)
	return util.ppn(player,const.PP_INNERPOWER_LEVEL)
end

function checkRedPoint(player)
	if checkUpgradeUsable(player) then
		return true
	end
end

function addInnerPower(player, add)
	player:set_param(const.PP_INNERPOWER_EXP,util.ppn(player,const.PP_INNERPOWER_EXP)+add)	
	player:alert(100000,1,"获得内功:"..add)

	gui.moduleRedPoint.checkInnerPowerUsable(player)
	freshPanel(player)
end

function onLevelUp(player, lv)
	gui.moduleRedPoint.checkInnerPowerUsable(player)
end

cklevel.add_listener(onLevelUp)