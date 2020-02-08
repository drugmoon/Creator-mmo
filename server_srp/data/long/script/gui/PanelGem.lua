module(... ,package.seeall)

local posTable={
	["-4"]= {con=const.PP_WEAPON_QH_LEV,  type=110, index=1},
	["-8"]= {con=const.PP_HAT_QH_LEV,     type=140, index=2},
	["-12"]={con=const.PP_GLOVE1_QH_LEV,  type=150, index=3},
	["-10"]={con=const.PP_RING1_QH_LEV,   type=160, index=4},
	["-18"]={con=const.PP_BELT_QH_LEV,    type=170, index=5},
	["-6"]= {con=const.PP_CLOTH_QH_LEV,   type=120, index=6},
	["-14"]={con=const.PP_NECKLACE_QH_LEV,type=130, index=7},
	["-13"]={con=const.PP_GLOVE2_QH_LEV,  type=150, index=8},
	["-11"]={con=const.PP_RING2_QH_LEV,   type=160, index=9},
	["-20"]={con=const.PP_BOOT_QH_LEV,    type=180, index=10},
}

local indexTable={
	[1]= {con=const.PP_WEAPON_QH_LEV,  jie=const.PP_WEAPON_JJ_LEV,  pos=-4, gem=const.PP_GEM_WEAPON},
	[2]= {con=const.PP_HAT_QH_LEV,     jie=const.PP_HAT_JJ_LEV,     pos=-8, gem=const.PP_GEM_HAT},
	[3]= {con=const.PP_GLOVE1_QH_LEV,  jie=const.PP_GLOVE1_JJ_LEV,  pos=-12,gem=const.PP_GEM_GLOVE1},
	[4]= {con=const.PP_RING1_QH_LEV,   jie=const.PP_RING1_JJ_LEV,   pos=-10,gem=const.PP_GEM_RING1},
	[5]= {con=const.PP_BELT_QH_LEV,    jie=const.PP_BELT_JJ_LEV,    pos=-18,gem=const.PP_GEM_BELT},
	[6]= {con=const.PP_CLOTH_QH_LEV,   jie=const.PP_CLOTH_JJ_LEV,   pos=-6, gem=const.PP_GEM_CLOTH},
	[7]= {con=const.PP_NECKLACE_QH_LEV,jie=const.PP_NECKLACE_JJ_LEV,pos=-14,gem=const.PP_GEM_NECKLACE},
	[8]= {con=const.PP_GLOVE2_QH_LEV,  jie=const.PP_GLOVE2_JJ_LEV,  pos=-13,gem=const.PP_GEM_GLOVE2},
	[9]= {con=const.PP_RING2_QH_LEV,   jie=const.PP_RING2_JJ_LEV,   pos=-11,gem=const.PP_GEM_RING2},
	[10]={con=const.PP_BOOT_QH_LEV,    jie=const.PP_BOOT_JJ_LEV,    pos=-20,gem=const.PP_GEM_BOOT},
}

local openNeed = {
	[1]=0,--武器
	[2]=2,--衣服
	[3]=5,--手镯
	[4]=9,--左戒指
	[5]=13,--鞋子
	[6]=1,--头盔
	[7]=3,--项链
	[8]=7,--右手镯
	[9]=11,--右戒指
	[10]=15,--腰带
}

local UPGRADE_COST = 0;
local PRICE_GEM = 100/9;

function handlePanelData(player,commond)
	local data = util.decode(commond)
	if not data then return end
	if data.actionid == "itemPos" then --当前选中的装备item
		-- print(">>>>>comeing server",util.encode(data))
		-- posData(player,data.params.pos)
-- 		player:set_param(const.PP_QIANGHUA_CUR_INDEX,data.params.index)
		getEquipGemInfo(player,data.params.index)
-- 	elseif data.actionid == "reqAllValue" then --请求总属性
-- 		player:recal_attr()
-- 		getPosLevel(player)
-- 		updateBtnJie(player)
-- 		checkGemRedPoint(player)
-- 	elseif data.actionid == "btnQiangHua" then --强化
-- 		startQiangHua(player,data.params.curPos,1)
-- 	elseif data.actionid == "btnAuto" then --自动强化
-- 		-- startQiangHua(player,data.params.curPos,2)
-- 		onekeyQiangHua(player)
-- 	elseif data.actionid == "reqDefaultIndex" then
-- 		getDefaultIndex(player)
-- 	elseif data.actionid == "btnJin" then
-- 		showConfirm(player,data.params.tag)
-- 	elseif data.actionid =="jinJie" then
-- 		confirmJinJie(player,data.param.equipTag)
	elseif data.actionid == "reqGemOpenState" then --开启条件
		player:push_lua_table("PanelGem", util.encode({actionid = "updateGemOpenState",openData=openNeed,nsLevel=util.ppn(player,const.PP_MAX_DEFEND_LEVEL)}))

-- -------------------------------------------宝石镶嵌消息-------------------------------------------------------
	-- elseif data.actionid =="reqCurEquipGem" then--当前装备位置宝石信息

	elseif data.actionid =="insertGem" then--开始镶嵌
		startInsertGem(player,data.params)
	elseif data.actionid == "gemRemove" then--摘除
		removeEquipGem(player,data.params.index,data.params.kongIndex)
	elseif data.actionid == "getServerDay" then
		getServerDay(player)
-- 	elseif data.actionid == "reqComGem" then--合成
-- 		startCompound(player,data.params.typeid)
-- 	elseif data.actionid == "comOneKey" then--一键合成
-- 		-- startCompoundOneKey(player,data.params.gemType,0,0)
-- 		onekeyComConfirm(player,data.params.gemType)
-- 	elseif data.actionid == "okcomOneKey" then --确定一键合成
-- 		startCompoundOneKey(player,data.param.gemType,0,0)

-- -------------------------------------------------客户端请求等级-------------------------------------------------
-- 	elseif data.actionid == "geQiangHuaLev" then --强化等级存到客户端tip使用
-- 		senderToClient(player)

-- ------------------------------------------------检测宝石红点--------------------------------------------------
-- 	elseif data.actionid == "chechGemRed" then
-- 		-- checkGemRedPoint(player)
-- 		gui.moduleRedPoint.checkGemUsable(player)
	end
end

-- -------------------------------------------宝石镶嵌-------------------------------------------------------
--获的装备格子的宝石信息
function getEquipGemInfo(player,index)
	local curGemStr = player:get_param(indexTable[index].gem)
	if curGemStr=="" or curGemStr==nil then
		curGemStr="0-0-0-0-0-0"
	end
	local tabGem = transformStr(curGemStr)
	player:push_lua_table("insertsucceed",util.encode(tabGem))
end

--执行宝石镶嵌操作
function startInsertGem(player,data)
	local curGemStr = player:get_param(indexTable[data.index].gem)
	if curGemStr=="" or curGemStr==nil then
		curGemStr="0-0-0-0-0-0"
	end
	local tabGem = transformStr(curGemStr)
	if holeSameTypeCondtion(player,tabGem[data.kongIndex],data.typeId)==false then
		if sameGemCondtion(player,tabGem,data.typeId)==false then  return end
	end
	if player:remove_item_by_type_pos(data.typeId, data.pos, 1)==0 then
		if tabGem[data.kongIndex]>0 then
			player:add_item(tabGem[data.kongIndex],1,0,nil,112)--摘除获取宝石的记录111
		end
		tabGem[data.kongIndex]=data.typeId
		local newGemStr = tostring(tabGem[1].."-"..tabGem[2].."-"..tabGem[3].."-"..tabGem[4].."-"..tabGem[5].."-"..tabGem[6])
		player:push_lua_table("insertsucceed",util.encode(tabGem))
		player:set_param(indexTable[data.index].gem,newGemStr)
		player:alert(1, 1, "镶嵌成功！")
		player:recal_attr()
	end
end
--摘除选中装备的宝石
function removeEquipGem(player,index,gemIndex)
	local curGemStr = player:get_param(indexTable[index].gem)
	if curGemStr=="" or curGemStr==nil then
		curGemStr="0-0-0-0-0-0"
	end
	local tabGem = transformStr(curGemStr)
	if tabGem[gemIndex]>0 then
		player:add_item(tabGem[gemIndex],1,0,nil,110)--摘除获取宝石的记录110
		tabGem[gemIndex]=0
		local newGemStr = tostring(tabGem[1].."-"..tabGem[2].."-"..tabGem[3].."-"..tabGem[4].."-"..tabGem[5].."-"..tabGem[6])
		player:set_param(indexTable[index].gem,newGemStr)
		player:push_lua_table("insertsucceed",util.encode(tabGem))
		player:alert(1, 1, "摘除成功！")
		player:recal_attr()
	else
		player:alert(1, 1, "无宝石摘除无效！")
	end
end

--当前镶嵌的宝石是否跟孔上的宝石是一个类型 是：替换（不提示该类型宝石已镶嵌）
function holeSameTypeCondtion(player,curtypeid,nexttypeid)
	local a = math.floor((tonumber(curtypeid)-17020)/12)
	local b = math.floor((tonumber(nexttypeid)-17020)/12)
	if a==b then
		-- player:alert(1, 1, "同类宝石型替换成功！")
		return true
	end
	return false
end

--判断有无同类型宝石已被镶嵌
function sameGemCondtion(player,tabGem,typeId)
	for i=1,#tabGem do
		local a = math.floor((tonumber(tabGem[i])-17020)/12)
		local b = math.floor((tonumber(typeId)-17020)/12)
		if a==b then
			player:alert(1, 1, "该类型宝石已镶嵌！")
			return false
		end
	end
	return true
end

--"17020-17021-17022-0" 转化成表 [17020,17021,17022,0]
function transformStr(str)
	local tabGem = {}
	if str==0 then str="0-0-0-0-0-0" end
	local j = 1
	for i in string.gmatch(str, "%d+")  do
		tabGem[j] = tonumber(i);
		j = j+1;
	end
	return tabGem
end

function getServerDay(player)
	local severinfo = {}
	severinfo.dayNum = server_start_day()
	player:push_lua_table("server_start_day",util.encode(severinfo))
end




ckpanel.add_listener("gui.PanelGem.handlePanelData", gui.PanelGem.handlePanelData)

--计算宝石的总属性
function getAllGemValue(player)
	local result = {
		[1] ={wfmin=0,wfmax=0},--物防
		[2] ={mfmin=0,mfmax=0},--魔防
		[3] ={wgmin=0,wgmax=0},--物攻
		[4] ={mgmin=0,mgmax=0},--魔攻
		[5] ={dgmin=0,dgmax=0},--道攻
		[6] ={value=0},--HP上限百分比
		[7] ={value=0},--MP上限百分比
		[8] ={value=0},--神圣攻击
		[9] ={value=0},--暴击几率
		[10]={value=0},--暴击力
	}
	for pos=-666,-502 do
		local itemId = player:get_item_type_by_pos(pos)
		if itemId and itemId>0 then
			result[1].wfmin = result[1].wfmin + util.checknumber(tab.itemdef[itemId].ac)
			result[1].wfmax = result[1].wfmax + util.checknumber(tab.itemdef[itemId].ac2)
			result[2].mfmin = result[2].mfmin + util.checknumber(tab.itemdef[itemId].mac)
			result[2].mfmax = result[2].mfmax + util.checknumber(tab.itemdef[itemId].mac2)
			result[3].wgmin = result[3].wgmin + util.checknumber(tab.itemdef[itemId].dc)
			result[3].wgmax = result[3].wgmax + util.checknumber(tab.itemdef[itemId].dc2)
			result[4].mgmin = result[4].mgmin + util.checknumber(tab.itemdef[itemId].mc)
			result[4].mgmax = result[4].mgmax + util.checknumber(tab.itemdef[itemId].mc2)
			result[5].dgmin = result[5].dgmin + util.checknumber(tab.itemdef[itemId].sc)
			result[5].dgmax = result[5].dgmax + util.checknumber(tab.itemdef[itemId].sc2)
			result[6].value = result[6].value + util.checknumber(tab.itemdef[itemId].max_hp_pres)
			result[7].value = result[7].value + util.checknumber(tab.itemdef[itemId].max_mp_pres)
			result[8].value = result[8].value + util.checknumber(tab.itemdef[itemId].holy_damage)
			result[9].value = result[9].value + util.checknumber(tab.itemdef[itemId].baoji_prob)
			result[10].value= result[10].value+ util.checknumber(tab.itemdef[itemId].baoji_pres)
		end
	end
	local result = {
		actionid = "updateTipsValue",
		tipsData = result,
	}
	player:push_lua_table("PanelGem", util.encode(result))
end


----------------------------

local UPGRADE_NEED_GEM = 3;
local MAX_GEM_LEVEL = 15;

local attr_str = {
	["ac"] = "物理防御：",
	["mac"] = "魔法防御：",
	["dc"] = "物理攻击：",
	["mc"] = "魔法攻击：",
	["sc"] = "道术攻击：",
	["hp"] = "生命上限：",
	["mp"] = "魔法上限：",
	["holyDamage"] = "神圣伤害：",
	["baojiProb"] = "暴击几率：",
	["baojiPres"] = "暴击伤害：",
};

local gemOffset = {
	{offSet = 500,	attr = "ATTACK", },
	{offSet = 530,	attr = "AC"},
	{offSet = 560,	attr = "MAC"},
	{offSet = 590,	attr = "HP"},
	{offSet = 620,	attr = "MP"},
	{offSet = 650,	attr = "SPECIAL"},
}

local gemShop = {
	[1]={id=25010001,name="攻击宝石",vcoin=100},
	[2]={id=25020001,name="物防宝石",vcoin=100},
	[3]={id=25030001,name="魔防宝石",vcoin=100},
	[4]={id=25040001,name="生命宝石",vcoin=100},
	[5]={id=25050001,name="魔法宝石",vcoin=100},
}

function getGemEquipPos(gemPos)
	local equipPos = gemPos + 500;
	for i,v in ipairs(gemOffset) do
		-- print("getGemEquipPos", i, v.offSet, gemPos, equipPos)
		if v.offSet + gemPos >= 0 then
			return equipPos;
		end
		equipPos = gemPos + v.offSet;
	end
	return equipPos;
end

function updateGemShop(player)
	local result = {
		actionid = "updateGemShop",
		gemShop = {},
	};
	local temp = tab.Store.getItemsByType(player,const.storeType.baoshi)
	for i,v in ipairs(temp) do
		table.insert(result.gemShop, {id = v.id, name = v.name, vcoin = v.money})
	end
	player:push_lua_table("PanelGem", util.encode(result));
end

function onPanelData(player,commond)
	-- print("onPanelData", commond);
	local data = util.decode(commond);
	if not data then return end
	if data.actionid == "insetGem" then --当前选中的装备item
		-- print("////////////////", data.equipPos, data.gemPos);
		if data.equipPos < 0 and data.gemPos >= 0 then
			insetGem(player, data.equipPos, data.gemPos);
		end
	elseif data.actionid == "undressGem" then
		if data.gemPos < 0 then
			undressGem(player, data.gemPos);
		end
	elseif data.actionid == "updateUpgrade" then
		if data.gemPos < 0 then
			updateBoxUpgradeGem(player, data.gemPos);
		end
	elseif data.actionid == "upgradeGem" then
		if data.gemPos < 0 then
			upgradeGem(player, data.gemPos, data.autoVcoin);
		end
	elseif data.actionid == "reqGemShop" then
		updateGemShop(player);
	elseif data.actionid == "buyGem" then
		if data.index and data.index > 0 and data.num > 0 then
			buyGem(player, data.index, data.num);
		end
	elseif data.actionid=="reqGemTipsAll" then
		getAllGemValue(player)
	end
end

function getGemBaseData(player, typeId)
	local data;
	if PLATFORM_BANSHU then
		local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice,reinLv,funcId,funcParams,mEquipType,mJob, mMaxHp, mMaxMp, mHolyDamage, mBaojiProb, mBaojiPres = player:get_item_base_data(typeId)
		data = {
			name = itname,
			gemLevel = typeId%100,
			ac = itac,
			mac = itmac,
			dc = itdc,
			mc = itmc,
			sc = itsc,
			hp = mMaxHp,
			mp = mMaxMp,
			holyDamage = mHolyDamage,
			baojiProb = mBaojiProb,
			baojiPres = mBaojiPres,
		};
	else
		local id = tab.itemdef[typeId]
		if id and id.sub_type == const.ITEM_TYPE_GEM then
			data = {
				name = id.name,
				gemLevel = typeId%100,
				ac = id.ac,
				mac = id.mac,
				dc = id.dc,
				mc = id.mc,
				sc = id.sc,
				hp = (id.max_hp_pres/100).."%",
				mp = (id.max_mp_pres/100).."%",
				holyDamage = id.holy_damage,
				baojiProb = id.baoji_prob,
				baojiPres = id.baoji_pres,
			};
		end
	end
	return data;
end

function getUpgradeAttr(attrCur, attrNext)
	-- print(util.encode(attrCur))
	-- print(util.encode(attrNext))
	for k,v in pairs(attr_str) do
		if (tostring(attrCur[k]) ~= "0" and tostring(attrCur[k]) ~= "0%")  or (tostring(attrNext[k]) ~= "0" and tostring(attrNext[k]) ~= "0%")then
			return {
				attr = v,
				from = attrCur[k],
				to = attrNext[k],
			}
		end
	end
end

local function calculateVcoin(player, itemType, attr)
	local mLevel = attr.gemLevel;
	local gemTypeId;
	local total = math.pow(3, mLevel);
	local own = 0;
	local num;
	for i = 0, mLevel - 1 do
		gemTypeId = itemType - i;
		num = player:num_item(gemTypeId, 1);
		if i == 0 then
			num = num + 1;
		end
		own = own + math.pow(3, mLevel - i - 1) * num;
	end
	if own < total then
		return (total - own) * PRICE_GEM;
	else
		return 0;
	end
end

function updateBoxUpgradeGem(player, itemPos)
	local result = {
		actionid = "updateUpgradeGem",
	};
	local itemType = player:get_item_type_by_pos(itemPos);
	if itemType and itemType > 0 then
		result.needGem = UPGRADE_NEED_GEM;
		result.curGem=(1 + player:num_item(itemType, 1))
		result.needId = itemType;
		local attrCur = getGemBaseData(player, itemType);
		if attrCur and attrCur.gemLevel < MAX_GEM_LEVEL then -- 非顶级取下阶属性
			local attrNext = getGemBaseData(player, itemType + 1);
			result.nextGem = {
				name = attrNext.name,
				typeId = itemType + 1
			};
			result.needVcoin = "消耗:"..UPGRADE_COST.."元宝手续费";
			result.upgradeAttr = getUpgradeAttr(attrCur, attrNext);
			result.replaceVcion = calculateVcoin(player, itemType, attrCur)
		end
	end
	player:push_lua_table("PanelGem", util.encode(result))
end

function insetGem(player, equipPos, gemPos)
	-- print("////////////////");
	local ret = player:use_gem(equipPos, gemPos);
	-- print("insetGem", ret)
	if ret == 1 then
		player:push_lua_table("PanelGem", util.encode({actionid = "updateGemBag"}))
		return true;
	end
end

function undressGem(player, gemPos)
	local ret = player:undress_gem(gemPos);
	-- print("undressGem", ret)
	if ret == 1 then
		player:push_lua_table("PanelGem", util.encode({actionid = "updateGemBag"}))
	elseif ret == -1 then
		player:alert(1, 1, "背包已满，无法卸下宝石！")
	end
end



function autoVcoinUpgrade(player, itemType, gemPos, attr)
	local mLevel = attr.gemLevel;
	local gemTypeId;
	local total = math.pow(3, mLevel);
	local own = 0;
	local num;

	local ret;
	for i = 0, mLevel - 1 do
		if own < total then
			gemTypeId = itemType - i;
			num = player:num_item(gemTypeId, 1);
			--num = player:remove_item(gemTypeId, num);
			-- print(gemTypeId, num, ret)
			-- num = num - ret;
			if i == 0 then
				ret = player:remove_item_by_type_pos(gemTypeId, gemPos, 1);
				if ret == 0 then
					num = num + 1;
				end
			end
			own = own + math.pow(3, mLevel - i - 1) * num;
			if own < total then
				player:remove_item(gemTypeId, num);
			else
				player:remove_item(gemTypeId, num-(own-total)/(math.pow(3, mLevel - i - 1)))
			end
		end
	end
	if own < total then
		local vcoin = (total - own) * PRICE_GEM;
		-- print("autoVcoinUpgrade", vcoin);
		if player:get_vcoin() >= vcoin and 1== player:sub_vcoin(vcoin, "元宝代替宝石") then
			return true;
		end
	else
		return true;
	end
end

function upgradeGem(player, gemPos, autoVcoin)
	-- print("////////////////////upgradeGem////////////////////", gemPos)
	local succeed = false;
	local gemLacked = false;
	local itemType = player:get_item_type_by_pos(gemPos);
	if itemType and itemType > 0 then
		local attrCur = getGemBaseData(player, itemType);
		if not attrCur then return; end
		if attrCur.gemLevel >= MAX_GEM_LEVEL then
			return player:alert(1, 1, "当前宝石已升级至最大级");
		end
		local equipPos = getGemEquipPos(gemPos);

		if player:num_item(itemType, 1) < 2 then
			gemLacked = true;
		end

		if gemLacked and autoVcoin then
			-- 扣除元宝
			local mVcoin = calculateVcoin(player, itemType, attrCur);
			if mVcoin and player:get_vcoin() >= mVcoin then
				-- if 1 == player:sub_vcoin(UPGRADE_COST, "宝石升级") then
					succeed = autoVcoinUpgrade(player, itemType, gemPos, attrCur)
				-- end
			else
				return player:alert(1, 1, "元宝数量不足，升级失败");
			end
		else
			--if not (player:get_vcoin() >= UPGRADE_COST) then
			--	return player:alert(1, 1, "元宝数量不足，升级失败");
			--end

			if player:num_item(itemType, 1) < 2 then
				return player:alert(1, 1, "宝石数量不足，无法升级!");
			end

			--if 1 ~= player:sub_vcoin(UPGRADE_COST, "宝石升级") then
			--	return player:alert(1, 1, "元宝数量不足，升级失败");
			--end

			local ret = player:remove_item_by_type_pos(itemType, gemPos, 1);--(先移除身上宝石) ret大于0 说明实际数量不足
			if ret > 0 then
				return player:alert(1, 1, "身上宝石扣除失败，请稍后重试!");
			end

			if player:remove_item(itemType, 2) == 2 then -- 扣除成功
				succeed = true;
			end
		end

		if succeed then
			local newPos = player:add_item2(itemType + 1, 1, 250);
			if insetGem(player, equipPos, newPos) then
				player:push_lua_table("PanelGem", util.encode({actionid = "upgradeSucceed"}))
			end
		end
	end
end

function buyGem(player, index, num)
	local gemShop = {}
	local temp = tab.Store.getItemsByType(player,const.storeType.baoshi)
	for i,v in ipairs(temp) do
		table.insert(gemShop, {id = v.id, name = v.name, vcoin = v.money})
	end
	local gemData = gemShop[index];

	if gemData then
		if player:get_vcoin() < gemData.vcoin * num then
			showChongzhi(player)
			return --player:alert(1, 1, "元宝不足"..gemData.vcoin * num.."!");
		end
		if player:num_bag_black() < num then
			return player:alert(1, 1, "背包剩余格子不足"..num.."格！");
		end
		if 1 == player:sub_vcoin(gemData.vcoin * num, "快捷购买宝石") then
			player:add_item(gemData.id, num);
			player:alert(1, 1, "成功购买"..num.."个"..gemData.name.."！");
		end
	end
end

ckpanel.add_listener("gui.PanelGem.onPanelData", gui.PanelGem.onPanelData)