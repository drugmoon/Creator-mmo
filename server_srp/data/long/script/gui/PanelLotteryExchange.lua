module(... ,package.seeall)

function handlePanelData(player,commond)
	local data = util.decode(commond)
	if data.actionid == "fresh" then
		freshPanel(player,data.tabIdx)
	elseif data.actionid == "buy" then
		buyItem(player,data.id)
	end
end
ckpanel.add_listener("gui.PanelLotteryExchange.handlePanelData",gui.PanelLotteryExchange.handlePanelData)

function freshPanel(player,tabIdx)
   	local result = {}
   	result.cmd = "fresh"
   	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
   	local jobId = util.getJobId(player) - 99
   	result.exchangeData = {}
   	for i,v in ipairs(tab.Exchange.exchangeData[tabIdx]) do
   		if (v.job == 0 or v.job == jobId) and zsLevel>=v.needZslv then
   			table.insert(result.exchangeData, v)
   		end
   	end
	result.curJiFen = util.ppn(player,const.PP_XUNBAO_JIFEN),--寻宝积分
   	player:push_lua_table("PanelLotteryExchange",util.encode(result));

   	updateExchangeRecord(player,nil)
end

--推送积分和寻宝日志
function updateExchangeRecord(player,myRecord)
	local result = {
		cmd="updateLotteryRecord",
		curJiFen=util.ppn(player,const.PP_XUNBAO_JIFEN),--寻宝积分
	}
	if not myRecord then
		result.curWorldRecord=util.decode(server.get_var("exchange_world_record")) or {}
	else
		result.myRecord=myRecord		
	end
	player:push_lua_table("PanelLotteryExchange",util.encode(result))
end

--存全服抽奖记录
function saveWorldRecord(player,itemName)
	local record = {player:get_name(),itemName}
	local serverRecord = util.decode(server.get_var("exchange_world_record"))
	if not serverRecord then serverRecord = {} end
	table.insert(serverRecord,1,record)
	table.remove(serverRecord,10)
	server.set_var("exchange_world_record",util.encode(serverRecord))
	updateExchangeRecord(player,record)
end

function buyItem(player,id)
	local itemData = tab.Exchange.Data[tostring(id)]
	if itemData then
		if util.ppn(player,const.PP_XUNBAO_JIFEN)>=itemData.needScore then
			if util.ppn(player, const.PP_ZHSH_LEVEL)<itemData.needZslv then
				return util.alert_msg(4, "转生等级不足！", player)
			end

			if player:num_bag_black()<1 then			
				return util.alert_msg(4, "背包空间不足，无法购买！", player)
			end
			-- if itemData.needId>0  then
			-- 	if player:num_item(itemData.needId)<1 then
			-- 		return util.alert_msg(4, "缺少兑换装备！", player)
			-- 	else
			-- 		player:remove_item(itemData.needId,1)
			-- 		util.alert_msg(4, "成功扣除 "..tab.getItemName(itemData.needId), player)
			-- 	end
			-- end
			-- player:set_param(const.PP_XUNBAO_JIFEN,util.ppn(player,const.PP_XUNBAO_JIFEN)- itemData.needScore);
			-- player:add_item(itemData.typeId,1,2-itemData.bind)
			-- util.alert_msg(4, "成功兑换 "..tab.getItemName(itemData.typeId), player)
			if itemData.needId>0 and player:num_item(itemData.needId)<1 then
				return util.alert_msg(4, "缺少兑换装备！", player)
			end

			local qlevel = 0
			local needpos = -1
			if itemData.needId>0 and getNeedPos(player,itemData.needId) then
				qlevel,needpos=getNeedPos(player,itemData.needId)
			end

			-- if itemData.needId>0 and player:remove_item(itemData.needId, 1)~=1 then
			-- 	return util.alert_msg(4, "扣除兑换装备失败！", player)
			-- end

			if itemData.needId>0 and needpos>=0 and player:remove_item_by_type_pos(itemData.needId, needpos, 1)~=0 then
				return util.alert_msg(4, "扣除兑换装备失败！", player)
			end

			player:set_param(const.PP_XUNBAO_JIFEN,util.ppn(player,const.PP_XUNBAO_JIFEN)- itemData.needScore);
			local targetpos = player:add_item2(itemData.typeId,1,2-itemData.bind)
			if qlevel>0 then
				player:set_item_uplv(itemData.typeId,targetpos,qlevel)
			end
			player:alert(1, 1, "成功购买"..tab.getItemName(itemData.itemBaseId))

			saveWorldRecord(player,itemData.name)

		else
			util.alert_msg(4, "寻宝积分不足"..itemData.needScore.."！", player) 
		end
	else
		util.alert_msg(4, "兑换失败", player)
	end
end

--寻找对应id的装备pos
function getNeedPos(player,needId)
	for i=0,59 do
		local itemId = player:get_item_type_by_pos(i)
		if itemId==needId then
			local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
			qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(i)
			return qianghualv,i
		end
	end
	return false
end