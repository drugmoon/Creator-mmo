module(...,package.seeall)

local COMPOSE = {
	{
		{str = "70级装备合成",	typeId = 12110091,	equipType = 11,	},
		{str = "80级装备合成",	typeId = 12110091,	equipType = 12,	},
		{str = "90级装备合成",	typeId = 12110091,	equipType = 13,	},
		{str = "1转装备合成",	typeId = 12110091,	equipType = 14,	},
		{str = "2转装备合成",	typeId = 12110091,	equipType = 15,	},
	},{
		{str = "4转神装合成",	typeId = 12110091,	equipType = 21,	zsLv= 4},
		{str = "6转神装合成",	typeId = 12110091,	equipType = 22,	zsLv= 6},
		{str = "8转神装合成",	typeId = 12110091,	equipType = 23,	zsLv= 8},
		{str = "10转神装合成",	typeId = 12110091,	equipType = 24,	zsLv= 10},
		{str = "12转神装合成",	typeId = 12110091,	equipType = 25,	zsLv= 12},
	}
}

function handlePanelData(player,commond)
	local data = util.decode(commond)
	if not data then return end
	if data.actionid == "reqTabView" then--合成
		getComponseData(player,data.tab,data.page)
	elseif data.actionid == "reqPageData" then--合成
		getPageData(player,data.tab,data.page)
	elseif data.actionid == "startCom" then
		startCompound(player,data.id,data.tab,data.page)
	end
end

ckpanel.add_listener("gui.PanelCompose.handlePanelData",gui.PanelCompose.handlePanelData)

function _getEquipNumByTypeAndLv(bagData,conf)
	local items = {}
	local equip_group = conf.needEquipGroup
	local pJob = conf.needItemJob
	local pos = conf.needItemPos
	for k,v in pairs(bagData) do
		if v.equip_group == equip_group and ( pJob == 0 or pJob == v.job) and( pos == 0 or pos == v.equipPos) then
			table.insert(items,v)
		end
	end
	return items
end

function _getBagData( player )
	local bag_num = player:num_bag()
	local bagData = {}
	for i=0,bag_num do
		local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
			itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount,mZLevel,mFuncId,mFuncParams = player:get_item_data_by_pos(i)
		if ittype_id then
			-- local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice,reinLv,funcId,funcParams,mEquipType,mJob = player:get_item_base_data(ittype_id)
			local itemDef = tab.itemdef[ittype_id]
			if itemDef and itemDef.sub_type==1 then
				table.insert(bagData,{
					id = ittype_id,
					bind = itbind,
					qianghualv = qianghualv,
					pos = i,
					equipPos = itemDef.equip_type,
					equip_group = checknumber(itemDef.equip_group),
					needLv = checknumber(itemDef.needlevel),
					needzslv = mZLevel,
					job = checknumber(itemDef.job),
				})
			end
		end
	end
	table.sort(bagData,function(A,B)
		if A.qianghualv == B.qianghualv then
			return A.pos < B.pos
		end
		return A.qianghualv < B.qianghualv
	end)
	return bagData
end

--同时刷新左边tableview和右边page
function getComponseData(player,tabIndex,pageIndex)
	if not tab.Compose.composeData[tabIndex] then return end
	local result = {}
	result.cmd = "getComposeData"
	result.leftData = COMPOSE[tabIndex]
	result.zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)

	player:push_lua_table("PanelCompose",util.encode(result))
end

function isSellDay(conf)
	if conf.startDay==0 or conf.startDay<= server_start_day()+1 then
		return true
	end
end

--只刷新右边page
function getPageData(player,tabIndex,pageIndex)
	if not tab.Compose.composeData[tabIndex] then return end
	local result = {}
	result.cmd = "getComposeData"
	result.rightData = {}
	local gender = {["male"]=1,["female"]=2}
	local gendernumber = gender[player:get_gender_name()]
	local jobId = util.getJobId(player)-99
	for k,v in pairs(tab.Compose.composeData[tabIndex]) do
		-- print(tab.getItemJob(v.typeId),v.equipType ,COMPOSE[tabIndex][pageIndex].equipType)
		if (tab.getItemJob(v.typeId)==0 or tab.getItemJob(v.typeId)==jobId) then
			if (tab.getItemGender(v.typeId)==0 or tab.getItemGender(v.typeId)==gendernumber) then
				if v.equipType == COMPOSE[tabIndex][pageIndex].equipType  then
					table.insert(result.rightData,v)
				end
			end
		end
	end
	-- print("result.rightData",#result.rightData)
	local bagData = _getBagData(player)
	for k,v in pairs(result.rightData) do
		result.rightData[k].num = #_getEquipNumByTypeAndLv(bagData, v)
		if player:num_item(result.rightData[k].typeId)>0 then
			result.rightData[k].has = 1
		else
			result.rightData[k].has = 0
		end

	end
	result.zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	if COMPOSE[tabIndex][pageIndex].zsLv and result.zsLevel<COMPOSE[tabIndex][pageIndex].zsLv then
		result.notice =string.format("转生等级达到%d级开启功能",COMPOSE[tabIndex][pageIndex].zsLv)
	end
	result.serverDay = checknumber(server_start_day())
	player:push_lua_table("PanelCompose",util.encode(result))
end

function startCompound(player,id,tabIndex,pageIndex)
	local conf = tab.Compose.getConf(id)
	if not conf then
		util.alert_msg(4, "合成装备失败！", player)
		return
	end
	if conf.playerZslv>0 and util.ppn(player,const.PP_ZHSH_LEVEL)<conf.playerZslv then
		return util.alert_msg(4, "转生等级不足"..conf.playerZslv.."转!", player)
	end
	if conf.playerLv>0 and player:get_level()<conf.playerLv then
		return util.alert_msg(4, "等级不足"..conf.playerLv.."级!", player)
	end
	local bagData = _getBagData(player)
	local equips = _getEquipNumByTypeAndLv(bagData,conf)

	local bind,qianghualv = 0,0
	if #equips<conf.needNum then
		return util.alert_msg(4, "缺少参与合成装备！", player)
	end
	if not isSellDay(conf) then
		return util.alert_msg(4, "物品合成时间未到！", player)
	end
	table.sort(equips,function(a,b)
		return a.pos<b.pos
	end)
	for i=1,conf.needNum do
		bind = math.max(bind,equips[i].bind)
		qianghualv = math.max(qianghualv,equips[i].qianghualv)
		player:remove_item_by_type_pos(equips[i].id, equips[i].pos, 1)
	end
	local pos = player:add_item(conf.typeId,1,bind)
	if qianghualv>0 then
		for i=1,qianghualv do
			player:item_upgrade(pos,0,0,0,0,0)
		end
	end
	local itname = player:get_item_base_data(conf.typeId);
	util.alert_msg(4, "成功合成【"..itname.."】1件！", player);
	if conf.isNotice then
		server.info(10000,1,player:get_name().."合成了一件"..tab.getItemName(conf.typeId))
	end
	getPageData(player,tabIndex,pageIndex)
end