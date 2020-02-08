module(..., package.seeall)
-- ID	消耗筑基值	战士改变属性类型	战士改变属性值	法师改变属性类型	法师改变属性值	道士改变属性类型	道士改变属性值	阶数	层数图标
-- ID	BasisVal	WarriorAttrType	WarriorAttrData	MasterAttrType	MasterAttrData	PriestAttrType	PriestAttrData	TeamID	Icon
-- int	int	int[]	int[]	int[]	int[]	int[]	int[]	int	string
local keys = {"id","BasisVal","WarriorAttrType","WarriorAttrData","MasterAttrType","MasterAttrData","PriestAttrType","PriestAttrData","TeamID","Icon"};
local types ={"int","int","int[]","int[]","int[]","int[]","int[]","int[]","int","string"};
local temp = tab.parseCsvFile(server.load_csv_file("wuhunlevel.csv"),keys,types)
wuhunlevel = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		wuhunlevel[tonumber(k)] = v
	end
end

function handlePanelData(player,commond)
	-- player:set_param(const.PP_WUHUN_LEVEL,0)
	-- player:set_param(const.PP_WUHUN_SCORE,1e6)
	-- player:set_param(const.PP_WUHUN_SCORE,1000000)
	local data = util.decode(commond)
	if data.actionid == "freshwuhun" then
		freshWuhunPanel(player)
	elseif data.actionid == "upwuhun" then
		upgradeWuhun(player)
	elseif data.actionid == "checkRedPoint" then
		pushRedPoint(player)
	end
end
ckpanel.add_listener("gui.PanelWuHun.handlePanelData",gui.PanelWuHun.handlePanelData)

function freshWuhunPanel(player)
	local level = util.ppn(player, const.PP_WUHUN_LEVEL)
	local score = util.ppn(player, const.PP_WUHUN_SCORE)

	local result = {};
	result.cmd = "freshwuhun";
	result.level = level;
	result.score = score;
	local jobId = util.getJobId(player)
	if wuhunlevel[level] then
		--当前总属性
		if jobId == 100 then
			result.attrTypeAll = wuhunlevel[level].WarriorAttrType
			result.attrDataAll = wuhunlevel[level].WarriorAttrData
		elseif jobId == 101 then
			result.attrTypeAll = wuhunlevel[level].MasterAttrType
			result.attrDataAll = wuhunlevel[level].MasterAttrData
		elseif jobId == 102 then
			result.attrTypeAll = wuhunlevel[level].PriestAttrType
			result.attrDataAll = wuhunlevel[level].PriestAttrData
		end
	end
	
	result.percentage = 100
	local jie = level%7==0 and level/7+1 or math.ceil((level+1)/7)
	jie = util.bound(1,jie,7)
	local ji = level>0 and (level%7==0 and 7 or level%7) or 1
	local nextji = level%7==0 and 1 or level%7+1
	if level == #wuhunlevel then nextji = 8 end
	result.jie = jie
	result.ji = ji
	result.nextji = nextji

	if wuhunlevel[level+1] then
		--进度条
		result.needValue = wuhunlevel[level+1].BasisVal
		if result.needValue<score then
			result.percentage = (nextji-1)*100/6
		else
			result.percentage = (ji-1)*100/6 + 100*score/wuhunlevel[level+1].BasisVal/6
		end
	end
	--每级单独属性
	local attrTotal = {}
	for i,v in ipairs(wuhunlevel) do
		if v.TeamID == jie then
			local attrType,attrData = getUpgradeValue(jobId,i)
			table.insert(attrTotal,{
				attrType = attrType,
				attrData = attrData,
			})
		end
	end
	result.attrTotal = attrTotal;

	player:push_lua_table("PanelWuHun",util.encode(result));
end

function upgradeWuhun(player)
	local nextLev = util.ppn(player, const.PP_WUHUN_LEVEL) + 1
	if wuhunlevel[nextLev] then
		local score = util.ppn(player, const.PP_WUHUN_SCORE)
		if score >= wuhunlevel[nextLev].BasisVal then
			player:set_param(const.PP_WUHUN_SCORE,score - wuhunlevel[nextLev].BasisVal)
			player:set_param(const.PP_WUHUN_LEVEL,nextLev)
			setWuhunAttr(player,true)
			freshWuhunPanel(player)
			pushRedPoint(player)

			gui.moduleRedPoint.checkDiceUsable(player)--领取后判断光圈
		else
			util.alert_msg(4, "武魂值不足，无法升级！", player)
		end
	else
		util.alert_msg(4, "武魂已满级！", player)
	end
end
function getUpgradeValue(jobId,level)
	local attrType,attrData,preAttrType,preAttrData
	if wuhunlevel[level] then
		if jobId == 100 then
			attrType = wuhunlevel[level].WarriorAttrType
			attrData = wuhunlevel[level].WarriorAttrData
		elseif jobId == 101 then
			attrType = wuhunlevel[level].MasterAttrType
			attrData = wuhunlevel[level].MasterAttrData
		elseif jobId == 102 then
			attrType = wuhunlevel[level].PriestAttrType
			attrData = wuhunlevel[level].PriestAttrData
		end
	end
	if wuhunlevel[level-1] then
		if jobId == 100 then
			preAttrType = wuhunlevel[level-1].WarriorAttrType
			preAttrData = wuhunlevel[level-1].WarriorAttrData
		elseif jobId == 101 then
			preAttrType = wuhunlevel[level-1].MasterAttrType
			preAttrData = wuhunlevel[level-1].MasterAttrData
		elseif jobId == 102 then
			preAttrType = wuhunlevel[level-1].PriestAttrType
			preAttrData = wuhunlevel[level-1].PriestAttrData
		end
	end
	local newAttrType,newAttrData = {},{}
	for k,v in pairs(attrType) do
		if not preAttrType then
			table.insert(newAttrType,v)
			table.insert(newAttrData,attrData[k])
		else
			local idx = table.indexof(preAttrType,v)
			if not idx then
				table.insert(newAttrType,v)
				table.insert(newAttrData,attrData[k])
			elseif attrData[k] - preAttrData[idx] >0 then
				table.insert(newAttrType,v)
				table.insert(newAttrData,attrData[k] - preAttrData[idx])
			end
		end
	end
	return newAttrType,newAttrData
end

local attrType = {
	[1000]={name = "最大生命值",		index = 2},
	[1001]={name = "当前生命值",		index = nil},
	[1002]={name = "最大魔法值",		index = 1},
	[1003]={name = "当前魔法值",		index = nil},
	[1004]={name = "最小物攻",			index = 7},
	[1005]={name = "最大物攻",			index = 8},
	[1006]={name = "最小魔攻",			index = 9},
	[1007]={name = "最大魔攻",			index = 10},
	[1008]={name = "最小道攻",			index = 11},
	[1009]={name = "最大道攻",			index = 12},
	[1010]={name = "最小物防",			index = 3},
	[1011]={name = "最大物防",			index = 4},
	[1012]={name = "最小魔防",			index = 5},
	[1013]={name = "最大魔防",			index = 6},
	[1014]={name = "暴击率",			index = 31},
	[1015]={name = "暴击值",			index = 32},
	[1016]={name = "命中",				index = 13},
	[1017]={name = "闪避",				index = 14},
	[1018]={name = "幸运",				index = 15},
	[1019]={name = "神圣攻击",			index = 41},
	[1020]={name = "神圣防御",			index = 43},
}

function setWuhunAttr(player,reduceOld)
	local level = util.ppn(player, const.PP_WUHUN_LEVEL)
	if wuhunlevel[level] then
		local jobId = util.getJobId(player)
		if jobId == 100 then
			for i,attrkey in ipairs(wuhunlevel[level].WarriorAttrType) do
				if attrType[attrkey] and attrType[attrkey].index then

					local attrvalue = player:get_script_attr(attrType[attrkey].index)
					local addvalue = wuhunlevel[level].WarriorAttrData[i]
					if reduceOld and wuhunlevel[level-1] then
						for ii,attrkeyold in ipairs(wuhunlevel[level-1].WarriorAttrType) do
							if attrkeyold == attrkey then
								addvalue = addvalue - wuhunlevel[level-1].WarriorAttrData[ii]
								break;
							end
						end
					end
					if addvalue>0 then
						player:set_script_attr(attrType[attrkey].index,attrvalue + addvalue);
					end
				else
					print("attrType nil  ",attrkey)
				end
			end
		elseif jobId == 101 then
			for i,attrkey in ipairs(wuhunlevel[level].MasterAttrType) do
				if attrType[attrkey] and attrType[attrkey].index then
					
					local attrvalue = player:get_script_attr(attrType[attrkey].index)
					local addvalue = wuhunlevel[level].MasterAttrData[i]
					if reduceOld and wuhunlevel[level-1] then
						for ii,attrkeyold in ipairs(wuhunlevel[level-1].MasterAttrType) do
							if attrkeyold == attrkey then
								addvalue = addvalue - wuhunlevel[level-1].MasterAttrData[ii]
								break;
							end
						end
					end
					if addvalue>0 then
						player:set_script_attr(attrType[attrkey].index,attrvalue + addvalue);
					end
				else
					print("attrType nil  ",attrkey)
				end
			end
		elseif jobId == 102 then
			for i,attrkey in ipairs(wuhunlevel[level].PriestAttrType) do
				if attrType[attrkey] and attrType[attrkey].index then
					
					local attrvalue = player:get_script_attr(attrType[attrkey].index)
					local addvalue = wuhunlevel[level].PriestAttrData[i]
					if reduceOld and wuhunlevel[level-1] then
						for ii,attrkeyold in ipairs(wuhunlevel[level-1].PriestAttrType) do
							if attrkeyold == attrkey then
								addvalue = addvalue - wuhunlevel[level-1].PriestAttrData[ii]
								break;
							end
						end
					end
					if addvalue>0 then
						player:set_script_attr(attrType[attrkey].index,attrvalue + addvalue);
					end
				else
					print("attrType nil  ",attrkey)
				end
			end
		end
		player:recal_attr()
	end
end
login.add_login_listener(setWuhunAttr)
function pushRedPoint(player)
	player:push_lua_table("PanelWuHun",util.encode({
		cmd = "checkRedPoint",
		red = isRedPointShow( player )
	}));
end
function isRedPointShow( player )
	local nextLev = util.ppn(player, const.PP_WUHUN_LEVEL) + 1
	if wuhunlevel[nextLev] then
		local score = util.ppn(player, const.PP_WUHUN_SCORE)
		if score >= wuhunlevel[nextLev].BasisVal then
			return true
		end
	end
	return false
end