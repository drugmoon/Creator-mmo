module(..., package.seeall)
local upgradeTable = {
	[1] =  {prob = 100,	needMoney = 100000, 	needVcoin = 0,		buyVcoin = 0, 	buyMoney = 0,		shiftMoney=30000,	shiftVcoin=0,	},
	[2] =  {prob = 90,	needMoney = 300000, 	needVcoin = 8,		buyVcoin = 0, 	buyMoney = 200000,	shiftMoney=40000,	shiftVcoin=0,	},
	[3] =  {prob = 80,	needMoney = 750000, 	needVcoin = 24,		buyVcoin = 0, 	buyMoney = 500000,	shiftMoney=50000,	shiftVcoin=0,	},
	[4] =  {prob = 70,	needMoney = 1500000, 	needVcoin = 72,		buyVcoin = 0, 	buyMoney = 980000,	shiftMoney=100000,	shiftVcoin=0,	},
	[5] =  {prob = 60,	needMoney = 2700000, 	needVcoin = 216,	buyVcoin = 0, 	buyMoney = 1380000,	shiftMoney=150000,	shiftVcoin=0,	},
	[6] =  {prob = 50,	needMoney = 4320000, 	needVcoin = 648,	buyVcoin = 0, 	buyMoney = 1930000,	shiftMoney=200000,	shiftVcoin=0,	},
	[7] =  {prob = 40,	needMoney = 6480000, 	needVcoin = 1296,	buyVcoin = 0, 	buyMoney = 2980000,	shiftMoney=0,		shiftVcoin=50,	},
	[8] =  {prob = 30,	needMoney = 9400000, 	needVcoin = 2592,	buyVcoin = 102, buyMoney = 0,		shiftMoney=0,		shiftVcoin=50,	},
	[9] =  {prob = 20,	needMoney = 13150000, 	needVcoin = 5184,	buyVcoin = 154, buyMoney = 0,		shiftMoney=0,		shiftVcoin=50,	},
	[10] = {prob = 10,	needMoney = 18150000, 	needVcoin = 7776,	buyVcoin = 227, buyMoney = 0,		shiftMoney=0,		shiftVcoin=100,	},
	[11] = {prob = 5,	needMoney = 24690000, 	needVcoin = 11664,	buyVcoin = 326, buyMoney = 0,		shiftMoney=0,		shiftVcoin=100,	},
	[12] = {prob = 3,	needMoney = 32000000, 	needVcoin = 17496,	buyVcoin = 528, buyMoney = 0,		shiftMoney=0,		shiftVcoin=200,	},
}
----合成配置表---
local equip = {
	[60]={
		40014,70014,90014,60014,100014,50014,
		40015,70015,90015,60015,100015,50015,
		40016,70016,90016,60016,100016,50016,
		},
	[70]={
		40017,70017,90017,60017,100017,50017,
		40018,70018,90018,60018,100018,50018,
		40019,70019,90019,60019,100019,50019,
		},
}

--------强化-------
local MAX_UPGRADE_LEVEL = 12

function onPanelData(player,commond)
	local data =util.decode(commond);
	if data.actionid == "upgrade" then ---强化
		if data.param and data.param[1] then
			upgradeEquip(player, data.param[1],data.param[2]);
		end
	elseif data.actionid == "lotteryhuishou" then
		npc.huishou.getEablePos(player)

	elseif data.actionid=="actStop" then
		npc.huishou.giveExp(player)

	elseif data.actionid == "huishou" then ----回收
		if data.param then
			huishouFunc(player,data.param)
		end
		player:push_item_panel_fresh(0);
		gui.moduleGuide.checkGuideSpecial(player, "menu_recycle");
	elseif data.actionid == "autohuishou" then
		if data.param then
			autohuishouFunc(player,data.param)
		end
	elseif data.actionid == "updateLabel" then -----刷新成功几率和元宝值，星级
		if data.param then
			onUpdateLabel(player,data.param[1])
		end
	elseif data.actionid == "buyUptimes" then ----购买次数
		if data.param then
			onBuyUpTimes(player,data.param[1])
		end
	elseif data.actionid == "pay" then
		player:push_lua_table("open",util.encode({name = "panel_chongzhi"}))
	elseif data.actionid == "shiftMoney" then 	-----转移money
		if data.param then
			onShiftMoney(player,data.param)
		end
	elseif data.actionid == "shiftItem" then 	-----转移
		if data.param then
			onShiftItem(player,data.param)
		end
	elseif data.actionid == "compound" then 	-----合成
		if data.param then
			onCompound(player,data.param)
		end
	end
end

function upgradeSucceed(player, pos, upgradelv)
	
	-- task.task1000.check_task_done(player,"强化");
			-----首周冲榜
	player:set_param(const.PP_UPGRADE_TIMES, util.ppn(player, const.PP_UPGRADE_TIMES)+1)
	if upgradelv > util.ppn(player, const.PP_UPGRADE_SINGLE_MAX_TIMES) then
		player:set_param(const.PP_UPGRADE_SINGLE_MAX_TIMES, upgradelv)
		player:set_param(const.PP_UPGRADE_SINGLE_MAX_TIMES_AMOUNT, 1)
	elseif upgradelv == util.ppn(player, const.PP_UPGRADE_SINGLE_MAX_TIMES) then
		player:set_param(const.PP_UPGRADE_SINGLE_MAX_TIMES_AMOUNT, util.ppn(player, const.PP_UPGRADE_SINGLE_MAX_TIMES_AMOUNT)+1)
	end
	local typeId = player:get_item_type_by_pos(pos)
	local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice = player:get_item_base_data(typeId)
	if upgradelv >= 10 then
		util.alert_msg(26,"玩家<font color='#ff0000'>["..player:get_name().."]</font>将["..itname.."]成功强化至["..upgradelv.."星],极大的提升了属性!<a color='#00ff00' href='event:local_OpenPanel_act2'>[我也要强化]</a>",1);
	end
	gui.PanelAchieve.triggerTaskEvent(player, "upgradeTimes",upgradelv)--强化成就

	gui.PanelTitle.onEquipUpgraded(player, upgradelv)
	-------------强化成就-------------
	-- gui.PanelAchieve.checkAchieveUpgrade(player, upgradelv)

	---------------强化成功特效---------------
	-- player:push_lua_table("effect_one",util.encode({effect_type=990022}))
end

function upgradeEquip(player, pos, marksure)
	local typeId
	if pos then typeId = player:get_item_type_by_pos(pos) end
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos);
	if not typeId then return end
	if not isEquipMent(typeId) then return end
	local remainTimes = mUpdMaxCount - mUpdFailedCount - qianghualv -- 取装备强化次数
	if remainTimes <= 0 and qianghualv < MAX_UPGRADE_LEVEL then 
		player:alert(1,1,"可用强化次数不足，请先购买！") return 
	elseif qianghualv == MAX_UPGRADE_LEVEL then
		player:alert(1,1,"你已经强化12次了，无法继续强化！") return 		
	end
	-- local equipLv = 3 -- 取装备强化等级
	if qianghualv < 5 and guideUpgrade(player, pos, qianghualv) then return end -- 主线强化
	
	local param = upgradeTable[qianghualv+1]
	if param then
		local upgraded = false
		if player:get_gamemoney() < param.needMoney then player:alert(1,1,"可用金币不足，请先购买！") return end
		if marksure and player:get_vcoin() < param.needVcoin then showConfirm(player,1,param.needVcoin,"upgradecoinbuy");player:alert(1,1,"可用元宝不足，请先充值！") return end
		local n = math.random(100)
		if marksure then
			player:sub_vcoin(param.needVcoin,"强化装备");
			n = 0;
		end
		if n <= param.prob then
			player:sub_gamemoney(param.needMoney)
			-- 强化成功,
			player:item_upgrade(pos,0,0,0,0,0)
			upgraded = true
			upgradeSucceed(player, pos, qianghualv+1)
		else
			-- 强化失败
			player:item_add_upd_failed(pos, 1)
		end
		onUpdateLabel(player, pos, upgraded)
	end
end

function isEquipMent(typeId)
	local isEquip = false
	if typeId > 20000 and typeId < 199999 then isEquip = true end
	return isEquip
end
ckpanel.add_listener("gui.PanelUpgrade.onPanelData",gui.PanelUpgrade.onPanelData)

function autohuishouFunc(player,param)
	if param.pos and param.typeid then
		local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
			qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(param.pos);
		if ittype_id == tonumber(param.typeid) then
			local posTable = {param.pos}
			npc.huishou.onHuishouFunc(player,posTable)
		end
	end
end

function huishouFunc(player,posTable)
	if #posTable>0 then
		npc.huishou.onHuishouFunc(player,posTable)
	end
	-- task.task1000.check_task_done(player,"回收") -- 修改不卡主线
end

function onUpdateLabel(player, pos, upgraded)
	local result = {}
	result.cmd = "updateLabel";
	result.upgraded = upgraded
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos);
	if qianghualv then
		result.qianghualv = qianghualv;
		local param = upgradeTable[qianghualv+1]
		if param then
			result.prob = param.prob;
			result.needMoney = param.needMoney;
			result.needVcoin = param.needVcoin;
			if param.buyMoney>0 then
				result.buyMoney	= "需金币:"..param.buyMoney;
			elseif param.buyVcoin>0 then
				result.buyMoney	= "需元宝:"..param.buyVcoin;
			end
		end
		player:push_lua_table("PanelUpgrade",util.encode(result));
	end
end

function onBuyUpTimes(player,pos)
	if pos then
		local typeId = player:get_item_type_by_pos(pos);
		if not typeId then return end
		local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
		qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos);
		local remainTimes = mUpdMaxCount - mUpdFailedCount - qianghualv -- 取装备强化次数
		if remainTimes==0 then
			if mUpdFailedCount>0 and mUpdFailedCount<=mUpdMaxCount then
				local param = upgradeTable[qianghualv+1];
				if player:get_vcoin()>=param.buyVcoin and player:get_gamemoney()>=param.buyMoney then
					player:item_add_upd_failed(pos, -1);
					player:sub_vcoin(param.buyVcoin,"购买强化装备次数")
					player:sub_gamemoney(param.buyMoney)
					player:alert(11,1,"恭喜你，购买成功！");
					onUpdateLabel(player,pos);
				else
					if param.buyVcoin>0 then
						showConfirm(player,1,param.buyVcoin,"upgradebuytimes")
						-- player:alert(10,1,"元宝不足"..param.buyVcoin);
					end
					if param.buyMoney>0 then
						player:alert(10,1,"金币不足"..param.buyMoney);
					end
				end
			end
		end
	end
end

function showConfirm(player,pTag,Num,msg)
	local result
	if pTag == 1 then
		result = {
			panel = "confirm",
			visible = true,
			lblConfirm = "元宝数量不足"..(Num or 100)..",是否充值?",
			path = "gui.PanelUpgrade.onPanelData",
			actionid = "pay",
			strMsg = msg,
			num = Num,
		}
	end
	if result then player:push_lua_table("alert",util.encode(result)) end
end

function onShiftMoney(player,param)
	local result ={}
	local oldPos = param[1]
	if oldPos then
		-- local typeId = player:get_item_type_by_pos(oldPos)
		local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
		qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(oldPos);
		if not qianghualv then return end
		result.money = upgradeTable[qianghualv].shiftMoney
		result.vcoin = upgradeTable[qianghualv].shiftVcoin
		result.cmd="shiftMoney"
		player:push_lua_table("PanelTranslate",util.encode(result))
	end
end

function onShiftItem(player, param)
	local result ={}
	local oldId,oldPos = param[1],param[2];
	local newId,newPos = param[3],param[4];
	if oldId and oldPos and newId and newPos then
		if oldId == 29002 or newId == 29002 then 
			player:alert(1,1,"试用武器不可操作！")
			return 
		end 
		if math.floor(oldId/10000)~=math.floor(newId/10000) then player:alert(1,1,"装备类型不一致，不可转移！") return end
		local oldItem = player:get_item(oldId,oldPos)
		local newItem = player:get_item(newId,newPos)
		if not oldItem or not newItem then return end
		local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
		qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(oldPos);

		local nittype_id,nitbind,nituse_bind,nitjpin,nitdura,nitdurmax,nitaddac,nitaddmac,nitadddc,nitaddmc,nitaddsc,nitluck,nqianghualv,
		nqianghuacount,nitupdad,nitupdmac,nitupddc,nitupdmc,nitupdsc,nprotect,naddhp,naddmp,nremaintime,nmUpdMaxCount,nmUpdFailedCount = player:get_item_data_by_pos(newPos);
		if qianghualv and qianghualv>0 and nqianghualv and nqianghualv==0 then
			local money = upgradeTable[qianghualv].shiftMoney
			local vcoin = upgradeTable[qianghualv].shiftVcoin
			if money>0 then
				if  player:get_gamemoney()>money then 
					player:sub_gamemoney(money) 
				else
					player:alert(1,1,"金币不足，无法转移！")
					return
				end
			end
			if vcoin>0 then
				if player:get_vcoin()>vcoin then 
					player:sub_vcoin(vcoin,"强化转移")
				else 
					player:alert(1,1,"元宝不足，无法转移！")
					return
				end
			end
			for i=1,qianghualv do
				player:item_upgrade(newPos,0,0,0,0,0);
			end
			player:item_add_upd_failed(oldPos, nmUpdFailedCount - mUpdFailedCount)
			player:item_add_upd_failed(newPos, mUpdFailedCount - nmUpdFailedCount)
			player:alert(1,1,"转移成功！")
			player:set_item_data(ittype_id,oldPos,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,0,qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp);
			result.cmd="shiftItem"
			player:push_lua_table("PanelTranslate",util.encode(result))
		end
	end
end
--------------引导强化--------------
local mainTaskUpgrade = {
	[28] = {con = const.PP_GUIDE_UPGRADE1, maxlv = 3},
	[36] = {con = const.PP_GUIDE_UPGRADE2, maxlv = 2},
	[61] = {con = const.PP_GUIDE_UPGRADE3, maxlv = 5},
}

function guideUpgrade(player, pos, qianghualv)

	local s = player:get_task_state(1000)
	local b = math.floor(s / const.TASK_STATE_NUM)
 	local d = math.fmod(s,const.TASK_STATE_NUM)
 	if mainTaskUpgrade[b] then
 		local index = util.ppn(player, mainTaskUpgrade[b].con)
 		if index < mainTaskUpgrade[b].maxlv then
	 		player:set_param(mainTaskUpgrade[b].con, index + 1)
	 		player:item_upgrade(pos,0,0,0,0,0)
			-- if index == 0 then task.task1000.check_task_done(player,"强化"); end
			upgradeSucceed(player, pos, qianghualv+1)
			onUpdateLabel(player,pos, true)
	 		return true
	 	end
 	end
end

function onCompound(player,param)
	if #param>=2 then
		local level = 0
		for k,v in pairs(equip) do
			if table.indexof(v, param[1]) then
				level = k
			end
		end
		if level==0 then
			util.alert_msg(4, "无法合成该物品!", player)
			return
		end
		if type(param[2])=="table" then
			local isExist = true
			for k,v in pairs(param[2]) do
				local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
					itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(v)
				if not ittype_id then
					isExist = false
				end
			end
			if not isExist then
				util.alert_msg(4, "部分消耗装备不在背包，无法合成！", player)
				return
			end
			local isAllRemoved = true
			for k,v in pairs(param[2]) do
				local ittype_id = player:get_item_data_by_pos(v)
				local result = player:remove_item_by_type_pos(ittype_id,v,1)
				if result ~=0 then
					isAllRemoved = false
				end
			end
			if isAllRemoved then
				player:add_item(param[1],1);
				util.alert_msg(4, "合成成功！", player)
				local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice = player:get_item_base_data(param[1])
				-- util.alert_msg(26,"玩家<font color='#ff0000'>["..player:get_name().."]</font>成功合成了["..itname.."]，离一身极品更近一步了!<a color='#00ff00' href='event:local_OpenPanel_act4'>[我也要合成]</a>",1);
				player:push_lua_table("PanelCompound",util.encode({cmd="compound"}));
			end
		end
	end
end