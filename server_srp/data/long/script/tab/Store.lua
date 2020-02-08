module(..., package.seeall)

--type说明
local quickBuyType = {
	[0] = "商城",
	[1] = "龙心购买",
	[2] = "狼牙购买",
	[3] = "转生购买",
	[4] = "宝石购买",
	[5] = "战神升阶购买",
	[6] = "声望购买",
	[7] = "强化购买",
	[8] = "背包购买",
	[9] = "挂机购买",
	[10] = "战神经验购买",
	[11] = "勋章购买",
	[12] = "荣誉购买",
	[13] = "炼魂购买",
}
local typeStore= {
	[1] = "常用道具",
	[2] = "成长变强",
	[3] = "绑元商品",
	[4] = "绑金商品",
}
local MoneyKind = {
	[100] = "金币",
	[101] = "绑定金币",
	[102] = "元宝",
	[103] = "绑定元宝",
}
local bind = {
	[1] = "绑定",
	[2] = "非绑定",
}
--按 quickBuyType 分类
storeType = {}
for k,v in pairs(tab.store) do
	local typet = v.quickBuyType
	if quickBuyType[typet] then
		if not storeType[typet] then storeType[typet] = {} end
		v.storeId = checknumber(k);
		tab.store[k].storeId = checknumber(k);
		-- v.name = tab.getItemName(v.itemBaseId);
		table.insert(storeType[typet],v);
	end
end
--排序
for k,v in pairs(storeType) do
	table.sort(v,function(a,b)
		if a.quickBuyNum == b.quickBuyNum then
			return a.storeId<b.storeId
		else
			return a.quickBuyNum < b.quickBuyNum
		end
	end)
end
local sellOnce = {
	[23020004] = {con = const.PP_STORE_ITEM_LIMIT1},
	[23030004] = {con = const.PP_STORE_ITEM_LIMIT2}
}
function isItemSell(player,item)
	local startLv = util.checkint(item.startLv)
	local endLv = util.checkint(item.endLv)
	local startTime = util.checkint(item.startTime)
	local endTime = util.checkint(item.endTime)
	local serverData = server_start_day()
	local plv = player:get_level()
	local sell = false
	if (startTime==0 and endTime == 0 or startTime<=serverData and (serverData<endTime or endTime ==0)) and (endLv ==0 or startLv <=plv and plv<=endLv) then 
		sell = true
	end
	if sellOnce[item.itemBaseId] then
		sell = util.ppn(player,sellOnce[item.itemBaseId].con)==0
	end
	return sell
end
--快捷购买
function getItemsByType(player,type)
	local t = {}
	if storeType[type] then
		for k,v in ipairs(storeType[type]) do
			if isItemSell(player,v) then
				local new = {
					id = v.itemBaseId,
					num = v.itemNum,
					bind = 2-v.bind,--转成01
					money = v.money,
					MoneyKind = v.MoneyKind,
					name = v.name,
					storeId = v.storeId,
				}
				table.insert(t,new)
			end
		end
	end
	return t
end
--商城
function getStoreByType(player,typed)
	local t = {}
	for k,v in pairs(tab.store) do
		if v.type == typed and isItemSell(player,v) and not v.notShowInStore then
			table.insert(t,v)
		end
	end
	return t
end

--notinbag= true 直接使用的道具
function buyShopItem(player,storeId,useType,num,notinbag)
	num = tonumber(num) or 1
	if not tab.store[tostring(storeId)] then print("no storeId==",storeId) return false end
	useType = useType or "商店"
	local realnum = num
	local itemData = tab.store[tostring(storeId)]
	if itemData.maxCount>0 then
		if util.ppn(player,itemData.storeId)>=itemData.maxCount then
			return util.alert_msg(4, "已达今日最大购买次数！", player)
		else
			if itemData.maxCount - util.ppn(player, itemData.storeId)<realnum then
				realnum = itemData.maxCount - util.ppn(player, itemData.storeId)
			end
		end
	end
	local leftUseItmes = tab.getItemDailyLimit(player,itemData.itemBaseId)
	if leftUseItmes<num then
		util.alert_msg(4, "超过每日最大使用次数", player)
		return false
	end
	if not notinbag then
		if player:num_bag_black() < util.getNeedBags(player,itemData.itemBaseId,realnum*itemData.itemNum) then
			player:alert(1,1,"背包空间不足！")
			return false
		end
	end
	if sellOnce[itemData.itemBaseId] and util.ppn(player,sellOnce[itemData.itemBaseId].con)>0 then
		player:alert(1,1,"您只能购买"..tab.getItemName(itemData.itemBaseId).."一次！")
		return false
	end
	local buysuccess = false
	if itemData.MoneyKind ==102 then
		if player:sub_vcoin(itemData.money*realnum,useType)==1 then
			buysuccess = true
		else
			showChongzhi(player)
		end
	elseif itemData.MoneyKind ==103 then
		if player:sub_vcoin_bind(itemData.money*realnum)==1 then
			buysuccess = true
		else
			player:alert(1,1,"绑定元宝不足！")
		end
	elseif itemData.MoneyKind ==100 then
		if player:sub_gamemoney(itemData.money*realnum)==1 then
			buysuccess = true
		else
		 	player:alert(1,1,"金币不足！")
		end 
	elseif itemData.MoneyKind ==101 then
		if 1==player:sub_gamemoney_bind(itemData.money*realnum) then
			buysuccess = true
		else
			local coin = player:get_gamemoney()
			local coinbind = player:get_gamemoney_bind()
			if coin + coinbind >= itemData.money*realnum then
				player:sub_gamemoney_bind(coinbind)
				player:sub_gamemoney(itemData.money*realnum-coinbind)
				buysuccess = true
			else
				player:alert(1,1,"绑定金币不足！")
			end
		end
	end
	local ret
	if buysuccess then
		if itemData.maxCount >0 then
			player:set_param(itemData.storeId,util.ppn(player, itemData.storeId)+realnum)
		end
		if not notinbag then
			player:add_item(itemData.itemBaseId,itemData.itemNum*realnum,2-itemData.bind)
			player:alert(1, 1, "成功购买"..realnum.."个"..itemData.name)
			setLimitItemCon(player,itemData.itemBaseId)
			return itemData,realnum
		else
			ret = item.chufa.onTriggerItem(player, itemData.name, itemData.itemBaseId, itemData.itemNum*realnum)
			if ret then
				setLimitItemCon(player,itemData.itemBaseId)
				player:alert(1, 1, "成功购买并使用"..realnum.."个"..itemData.name)
				return itemData,realnum
			end
		end
	end
	return false
end
function setLimitItemCon(player,itemId)
	if sellOnce[itemId] then
		player:set_param(sellOnce[itemId].con,util.ppn(player, sellOnce[itemId].con)+1)
	end
end
--有每日限购的变量清理--以storeId为键
function resetDailyMaxCount(player)
	for k,v in pairs(tab.store) do
		if v.maxCount>0 then
			player:set_param(v.id,0)
		end
	end
end