module(..., package.seeall)

local furnaceType = {
	[1] = {name = "玉佩", pos = const.ITEM_JADE_PENDANT_POSITION,	stone=const.PP_YUPEI_POOL , funcId = 10031},
	[2] = {name = "护盾", pos = const.ITEM_SHIELD_POSITION,			stone=const.PP_HUDUN_POOL , funcId = 10032},
	[3] = {name = "龙心", pos = const.ITEM_DRAGON_HEART_POSITION,	stone=const.PP_LONGXIN_POOL,funcId = 10033},
	[4] = {name = "狼牙", pos = const.ITEM_WOLFANG_POSITION,		stone=const.PP_LANGYA_POOL, funcId = 10034},
	[5] = {name = "勋章", pos = const.ITEM_ACHIEVE_MEDAL_POSITION,	stone=const.PP_ACHIEVE_INTEGRAL, funcId = 10071},
}

local jobMargin = {["warrior"]=0,["wizard"]=25,["taoist"]=50}
local furnaceState = {
	[1] = "可以合成",
	[2] = "碎片不足，无法领取！",
	[3] = "碎片不足，无法升级！",
	[4] = "装备已满级，无法继续升级！",
	[5] = "等级不足，无法升级！",
}

function getData(player,eType)
	if eType ==1 then
		local job = util.getJobId(player)-99
		local t = {}
		for k,v in pairs(tab.Furnace.furnaceData[eType]) do
			if v.job ==job or v.job ==0 then
				table.insert(t,v)
			end
		end
		return t
	else
		return tab.Furnace.furnaceData[eType]
	end
end
function onPanelData(player,commond)
	local data = util.decode(commond);
	if not data  then return end
	if data.actionid == "fresh" then
		freshPanel(player,data.tab);

	elseif data.actionid == "upgrade" then
		onUpgrade(player,data.param[1])
	elseif data.actionid == "buy" then
		onShopBuy(player,data.param[1],data.param[2])
	end
end

ckpanel.add_listener("gui.PanelFurnace.onPanelData",gui.PanelFurnace.onPanelData)

function freshPanel(player,index,up)
	local result = {}
	result.stonePool = util.ppn(player, furnaceType[index].stone)
	result.openFunc = {}
	for i,v in ipairs(furnaceType) do
		if not gui.moduleGuiButton.checkFuncOpened(player, v.funcId) then
			table.insert(result.openFunc,1,i)
		end
	end
	result.curId=0
	result.nextId=0
	result.needNum=0
	result.ids = {};
	result.cmd = "fresh"

	result.longxinShop = tab.Store.getItemsByType(player,const.storeType.longxin)
	result.langyaShop = tab.Store.getItemsByType(player,const.storeType.langya)
	result.up = up

	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
		itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount,mZLevel,mFuncId,mFuncParams = player:get_item_data_by_pos(furnaceType[index].pos)
	-- print(ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
	-- 	itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount,mZLevel,mFuncId,mFuncParams)	
	local typeData = getData(player,index)
	ittype_id = ittype_id or 0
	local curidx = #typeData
	result.curId = ittype_id

	for k,v in ipairs(typeData) do
		if v.needItemId == ittype_id then
			curidx = k
			result.nextId = v.nextId
			result.needNum = v.needValue
			break;
		end
	end

	curidx = util.bound(3,curidx,#typeData-3)
	for k,v in ipairs(typeData) do
		if k>curidx-3 and k<=curidx+3 then
			local name = furnaceType[index].name.."%d级"
			name = string.format(name,k)
			table.insert(result.ids,{id=v.nextId,name=name})
		end
	end
	result.breath = checkFurnace(player,index)
	
	player:push_lua_table("PanelFurnace",util.encode(result));
end

function checkUpgradeEnabled(player,eType)
	local state = 5
	local pos = furnaceType[eType].pos
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
			itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos)
	local nextid = 1
	local needNum = 0
	local jobName = player:get_job_name()
	local typeData = getData(player,eType)
	local lv = #typeData+1
	if not ittype_id then ittype_id = 0 end
	for i,v in ipairs(typeData) do
		if v.needItemId == ittype_id then
			nextid = v.nextId
			needNum = v.needValue
			lv = i
			break
		end
	end
	if lv <= #typeData then
		local stoneNum = util.ppn(player,furnaceType[eType].stone)
		if stoneNum>=needNum then
			state = 1
			local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice,reinLv,funcId,funcParams = player:get_item_base_data(nextid)
			if player:get_level()<itneedparam then
				state = 5
			end
		elseif ittype_id==0 then
			state = 2
		else
			state = 3
		end
	else
		state = 4
	end
	return state,ittype_id,nextid,needNum,lv
end

function onUpgrade(player,eType)
	local state,ittype_id,nextid,needNum,lv = checkUpgradeEnabled(player,eType)
	if state==1 then
		local pos = furnaceType[eType].pos
		if ittype_id>0 and player:remove_item_by_type_pos(ittype_id, pos, 1)~=0 then
			return
		end
		player:add_and_dress_equip(nextid,pos,1)

		local stoneNum = util.ppn(player,furnaceType[eType].stone)
		player:set_param(furnaceType[eType].stone,stoneNum- needNum);
		if eType<5 then
			freshPanel(player,eType,"up")
		end

		if eType == 1 then
			gui.moduleRedPoint.checkXuefuUsable(player)
		elseif eType == 2 then
			gui.moduleRedPoint.checkHudunUsable(player)
		elseif eType == 3 then--龙心
			gui.moduleRedPoint.checkLongXinUsable(player)
			gui.AwardHall_jingji.setRankLongXin(player)
		elseif eType == 4 then--狼牙
			gui.moduleRedPoint.checkLangYaUsable(player)
			gui.AwardHall_jingji.setRankLangYa(player)
		elseif eType == 5 then--勋章
			gui.moduleRedPoint.checkXunZhangUsable(player)
		end
		triggerTaskEvent(player, eType, lv)
		return true
	else
		local msg = furnaceState[state]
		if eType == 5 then
			msg = string.gsub(msg,"碎片","积分")
		end
		msg = string.gsub(msg,"装备",furnaceType[eType].name)

		util.alert_msg(4,msg , player)
	end
end

----成就
function triggerTaskEvent(player, eType, value)
	local types = {"yupei","hudun","longxin","langya","xunzhang"}
	gui.PanelAchieve.triggerTaskEvent(player, types[eType], value)
end

function checkXuefuUsable(player)
	return checkUpgradeEnabled(player,1) == 1
end

function checkHudunUsable(player)
	return checkUpgradeEnabled(player,2) == 1
end

function checkLongXinUsable(player)
	return checkUpgradeEnabled(player,3) == 1
end

function checkLangYaUsable(player)
	return checkUpgradeEnabled(player,4) == 1
end

function checkXunZhangUsable(player)
	return checkUpgradeEnabled(player,5) == 1
end

function onShopBuy(player,shoptype ,storeId)
	if not furnaceType[shoptype] then return end
	local itemData = tab.Store.buyShopItem(player,storeId,"神炉购买"..furnaceType[shoptype].name,1,"notinbag")
	if itemData then
		freshPanel(player,shoptype)
		if shoptype == 3 then
			gui.moduleRedPoint.checkLongXinUsable(player)
		elseif shoptype == 4 then
			gui.moduleRedPoint.checkLangYaUsable(player)
		end
	end
end

function checkFurnace(player,etype)
	if etype == 1 then
		return checkXuefuUsable(player)
	elseif etype == 2 then
		return checkHudunUsable(player)
	elseif etype == 3 then
		return checkLongXinUsable(player)
	elseif etype == 4 then
		return checkLangYaUsable(player)
	elseif etype == 5 then
		return checkXunZhangUsable(player)
	end
end

function addFurnaceScore(player,types,exp)
	if furnaceType[types] and type(exp) == "number" then
		local con = furnaceType[types].stone
		player:set_param(con,util.ppn(player,con) + exp)
		player:alert(1,1,"增加"..exp..furnaceType[types].name.."值")
		if types == 1 then
			gui.moduleRedPoint.checkXuefuUsable(player)
		elseif types == 2 then
			gui.moduleRedPoint.checkHudunUsable(player)
		elseif types == 3 then
			gui.PanelJingCaiAct.GetFurnaceScore(player,3,exp)
			gui.moduleRedPoint.checkLongXinUsable(player)
		elseif types == 4 then
			gui.PanelJingCaiAct.GetFurnaceScore(player,4,exp)
			gui.moduleRedPoint.checkLangYaUsable(player)
		end
		return true
	end
end

function getFurnaceLevelByType(player,index)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
		itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount,mZLevel,mFuncId,mFuncParams = player:get_item_data_by_pos(furnaceType[index].pos)
	if ittype_id then
		return ittype_id%100
	end
	return 0
end