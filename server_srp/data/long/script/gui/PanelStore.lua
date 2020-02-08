module(..., package.seeall)
-- PanelStore.lua

function onPanelData(player, commondData)
	local data =util.decode(commondData);
	if data.actionid == "fresh" then
		freshStore(player,data.tab)
	elseif data.actionid == "buy" then
		buyItem(player,data.id,data.tab)
	elseif data.actionid == "getAward" then
		-- getVipAward(player,data.vip)
	end
end
ckpanel.add_listener("gui.PanelStore.onPanelData",gui.PanelStore.onPanelData)

function freshStore(player,tabIdx)
   	local result = {}
   	result.cmd = "fresh"
   	result.storeData = tab.Store.getStoreByType(player,tabIdx)

   	player:push_lua_table("PanelStore",util.encode(result));
end

function buyItem(player,id,tabindex)
	local itemData,realnum = tab.Store.buyShopItem(player,id,"商店")
	local curMapId = player:get_map():get_id()
	gui.PanelGongXun.checkTaskComplete(player,8)

	if itemData and (itemData.itemBaseId == 23030004 or itemData.itemBaseId == 23020004) then
		if tabindex then
			freshStore(player,tabindex)
		end
	end
	if string.find(curMapId,"shaozhu") == nil then
		return
	end

	if itemData == false or itemData == nil then
		return
	end
	if itemData.itemBaseId == 32000005 or itemData.itemBaseId == 32000006 then
		map.shaozhu.freshCallFashiCount(player)
	end
end