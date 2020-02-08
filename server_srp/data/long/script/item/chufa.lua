module(..., package.seeall)
-- function jintiao(player,item_name,item_id,num,pos)
-- 	if util.ppn(player,const.PP_GOLDBRICKUSE_DATE) ~= today() then
-- 		player:set_param(const.PP_GOLDBRICKUSE_DATE,today())
-- 		player:set_param(const.PP_GOLDBRICKUSE_TIMES,0)
-- 	end
-- 	local limit = gui.PanelVip.getVipValue(player,"goldbrickuse")
-- 	local used = util.ppn(player, const.PP_GOLDBRICKUSE_TIMES)
-- 	local enableNum = limit-used
-- 	if enableNum<=0 then
-- 		player:alert(1, 1, "今日金条使用次数已用完！")
-- 		return 0
-- 	end
-- 	num = (num + used <= limit) and num or (limit - used)
-- 	if num > 1 then
-- 		local ret = player:remove_item_by_type_pos(item_id, pos, num)--ret大于0 说明实际数量不足
-- 		player:set_param(const.PP_GOLDBRICKUSE_TIMES, used + (num - ret))
-- 		player:add_gamemoney(1000000 * (num - ret))
-- 		player:alert(1, 1, "金条(金砖)已使用"..used + (num - ret).."次,今日剩余"..limit - used - (num - ret).."次！")
-- 		return 0
-- 	else
-- 		player:set_param(const.PP_GOLDBRICKUSE_TIMES, used + 1)
-- 		player:add_gamemoney(1000000)
-- 	end
-- end

--------------------药品相关代码--------------------
local status_drug = {
	[const.STATUS_TYPE.CONTINUATE_DRUG1] = 20001004,
	[const.STATUS_TYPE.CONTINUATE_DRUG2] = 20001005,
	[const.STATUS_TYPE.CONTINUATE_DRUG3] = 20001006,
}

function isDrugCD(player)
	player:is_status(const.STATUS_TYPE.DRUG);
end

--立即回血
function getDrugConf(player, id)
	local itname,addhp,itacmax,addmp,itmacmax,cd,statusid,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice = player:get_item_base_data(id)
	return addhp, addmp, cd, statusid;
end

function useDrug (player, item_name, item_id, num, pos)
	if not player:is_dead() then
		local addhp, addmp, cd, statusid = getDrugConf(player, item_id);
		-- print("instantDrug", item_id, addhp, addmp, cd, statusid);
		if not isDrugCD(player) and not player:is_status(statusid) then
			player:add_hp(addhp);
			player:add_mp(addmp);
			player:notify_hpmpchange(addhp,addmp);
			player:set_status(const.STATUS_TYPE.DRUG, 1, 0, 1, 1);
			player:set_status(statusid, cd, item_id, 1, 1);
		else
			player:alert(1,0,"药品冷却中,请稍后使用");
			return 0
		end
	else
		player:alert(1,0,"死亡状态下不可使用药品");
		return 0
	end
end


--持续性药品药效
function onAddDrugEffect(player, statusid)
	-- local item_id = status_drug[statusid];
	local item_id = player:get_status_param(statusid);
	-- print("onAddDrugEffect", statusid, item_id)
	local addhp, addmp, cd, statusid = getDrugConf(player, item_id);
	-- print("getDrugConf", statusid, item_id, addhp, addmp, cd, statusid)
	if addhp and addmp then
		player:add_hp(addhp);
		player:add_mp(addmp);
		player:notify_hpmpchange(addhp,addmp);
	end
end

function gorand(player)
	local map = server.find_map("v202")
	if map then
		local x,y = map:gen_rand_position()
		player:enter_map(map:get_id(),x,y)
	end
	return 0
end

function chuansong(player)
	npc.mapmove2.showPanelTelePort(player,"stone");
	return 0
end

function huicheng(player)
	if player:get_safearea_id() == 1 then
		posmap.fly(player,4010003)--土城
	elseif player:get_safearea_id() == 2 then
		posmap.fly(player,4010002)--比奇
	else
		player:go_home()
	end
	-- return 0
end

function suijishi(player)
	if server.is_war()>0 and player:get_map():get_id()=="kingcity" then
		player:alert(1,1,"皇城战期间禁止在沙巴克使用随机石！")
		return 0
	end
	go.rand(player)
	-- return 0
end


local funcId = {
	[0] = {desp = "	无功能",		},
	[1] = {desp = "	影响数值", 		paramDesp = "属性类型, 属性值"},
	[2] = {desp = "	增加BUF	", 		paramDesp = "BUF编号，BUF几率"},
	[3] = {desp = "	本地图随机移动", paramDesp = "随机有效次数"},
	[4] = {desp = "	召唤宠物",		paramDesp = "怪物编号"},
	[5] = {desp = "	开启礼包",		paramDesp = "物品ID，物品数量"},
	[6] = {desp = "	开启宝箱",		paramDesp = "宝箱类型ID,宝箱ID	1--宝箱，2--职业宝箱，3--帮会宝箱，4--男女宝箱，5--职业男女宝箱"},
	[7] = {desp = "	随机召唤boss", 	paramDesp = "单位ID"},
	[8] = {desp = "	圣兽升级卷", 	paramDesp = "1,增加的最大等级"},
	[9] = {desp = "	帮会红包", 		paramDesp = "钱数，元宝数"},
}
--属性枚举表
local funcParam = {
	[100]={name = "金币",		func = function(player,value) player:add_gamemoney(value)	end 	},
	[101]={name = "绑定金币",	func = function(player,value) player:add_gamemoney_bind(value) end 	},
	[102]={name = "元宝",		func = function(player,value) player:add_vcoin_best_enable(value,"使用物品获得",888);player:alert(11,1,"获得元宝:"..value) end 	},
	[103]={name = "绑定元宝",	func = function(player,value) player:add_vcoin_bind(value,"物品使用") end 	},
	[104]={name = "等级",		},
	[105]={name = "转生等级",	},
	[106]={name = "经验值",		func = function(player,value) player:add_exp(value) end 	},
	[107]={name = "战神经验值", check = function (player,value) return gui.PanelHuWei.addHuweiExp( player,value,true) end,	func = function(player,value) end 	},
	[108]={name = "内功等级",	},
	[109]={name = "内功经验",	func = function(player,value) gui.PanelInnerPower.addInnerPower( player,value ) end 	},
	[110]={name = "内功经验",	func = function(player,value) gui.PanelInnerPower.addInnerPower( player,value ) end 	},
	[111]={name = "内功当前值",	},
	[112]={name = "内功最大值",	},
	[113]={name = "vip等级",	},
	[114]={name = "转生值",		func = function(player,value) gui.PanelZhuanSheng.addZSExp( player,value ) end 	},
	[115]={name = "韧性",	},
	[116]={name = "战士对法师的伤害",	},
	[117]={name = "法师对道士的伤害",	},
	[118]={name = "道士对法师的伤害",	},
	[119]={name = "充值记录",	},
	[120]={name = "vip复活使用次数",	},
	[121]={name = "宝藏积分",	func = function(player,value) gui.PanelLottery.addLotteryScore( player,value ) end 	},
	[122]={name = "物品回收积分",	},
	[123]={name = "功勋值",		func = function(player,value) gui.PanelGongXun.addGongXunScore( player,value ) end 	},
	[124]={name = "功勋称号",	},
	[125]={name = "杀怪经验倍数",	},
	[126]={name = "角色杀怪伤害",	},
	[127]={name = "战神杀怪伤害",	},
	[128]={name = "装备回收经验上限",	func = function(player,value) npc.huishou.addRecycleLimit(player,value) end},
	[129]={name = "仓库格子上限",	func = function(player,value) npc.huishou.addDeoptSize(player,paramValue) end},
	[130]={name = "灭魔值",	},
	[131]={name = "灭魔值上限",	},
	[132]={name = "对角色的伤害",	},
	[133]={name = "针对角色的物理免伤",	},
	[134]={name = "针对角色的魔法免伤",	},
	[135]={name = "声望值",	func = function(player,value) gui.PanelOfficial.addsw(player,value) end},
	[136]={name = "PK值",	func = function(player,value) change_pk_value(player,value) end,check = function (player,value) if player:get_pk_value()<=0 then util.alert_msg(4, "当前PK值不可减少", player); return true; end end},
	[551]={name = "玉佩值",	func = function(player,value) gui.PanelFurnace.addFurnaceScore(player,1,value) end 	},
	[552]={name = "护盾值",	func = function(player,value) gui.PanelFurnace.addFurnaceScore(player,2,value) end 	},
	[553]={name = "龙心值",	func = function(player,value) gui.PanelFurnace.addFurnaceScore(player,3,value) end 	},
	[554]={name = "狼牙值",	func = function(player,value) gui.PanelFurnace.addFurnaceScore(player,4,value) end 	},
	[556]={name = "成就值",	func = function(player,value) gui.PanelAchieve.addAchieveScore(player,value) end 	},
	[557]={name = "荣誉值",	 func = function(player,value) gui.PanelOfficial.addry(player,value) end 		},
	[559]={name = "炼魂值",	 func = function(player,value) gui.PanelBossPictrue.addLianHun(player,value) end 		},
	[1000]={name = "最大生命值",	},
	[1001]={name = "当前生命值",	},
	[1002]={name = "最大魔法值",	},
	[1003]={name = "当前魔法值",	},
	[1004]={name = "最小物攻",	},
	[1005]={name = "最大物攻",	},
	[1006]={name = "最小魔攻",	},
	[1007]={name = "最大魔攻",	},
	[1008]={name = "最小道攻",	},
	[1009]={name = "最大道攻",	},
	[1010]={name = "最小物防",	},
	[1011]={name = "最大物防",	},
	[1012]={name = "最小魔防",	},
	[1013]={name = "最大魔防",	},
	[1014]={name = "暴击率",	},
	[1015]={name = "暴击值",	},
	[1016]={name = "命中",	},
	[1017]={name = "闪避",	},
	[1018]={name = "幸运",	},
	[1019]={name = "神圣攻击",	},
	[1020]={name = "神圣防御",	},
	[1021]={name = "物理减伤比例",	},
	[1022]={name = "魔法减伤比例",	},
	[1023]={name = "最大生命百分比",	},
	[1024]={name = "最大魔法百分比",	},
	[1025]={name = "最小物防百分比",	},
	[1026]={name = "最大物防百分比",	},
	[1027]={name = "最小魔防百分比",	},
	[1028]={name = "最大魔防百分比",	},
	[1029]={name = "最小物攻百分比",	},
	[1030]={name = "最大物攻百分比",	},
	[1031]={name = "最小魔攻百分比",	},
	[1032]={name = "最大魔攻百分比",	},
	[1033]={name = "最小道攻百分比",	},
	[1034]={name = "最大道攻百分比",	},
}

function getWnegInfo(item_id, lv)
	for k,v in pairs(tab.WnegInfoCsv) do
		if v.item_id == item_id and v.lv == lv then
			return v
		end
	end
end

function RandomUpLuckRes(WnegInfo)
	local total = WnegInfo.addrate + WnegInfo.stayrate + WnegInfo.reducerate
	local rand = math.random(1,total)

	if rand <= WnegInfo.addrate then
		return 0
	end

	if rand <= WnegInfo.addrate + WnegInfo.stayrate then
		return 1
	end

	if rand <= total then
		return 2
	end

end

function onUseZhufu( player, item_name, item_id, num, pos)
	if item_id == 32000007 and num >= 1 then
		-- 幸运卷轴
		-- local ret = player:remove_item_by_type_pos(item_id, pos, 1)
		player:set_buff(37001)
		return 1
	end

	if player:check_euqip(const.ITEM_WEAPON_POSITION) == 0 then
		util.alert_msg(4, "您没有穿戴武器,不能使用", player)
		return 0
	end

	if num >= 1 then
		local luck = player:get_weapon_luck()

		-- 幸运罐
		if item_id == 24040004 then
			if luck >= 7 then
				util.alert_msg(4, "您的武器幸运值高于或等于7,使用无效", player)
				return 0
			end

			-- local ret = player:remove_item_by_type_pos(item_id, pos, 1)
			player:set_weapon_luck(7)

			return 1
		end

		if luck >= 9 then
			util.alert_msg(4, "您的武器幸运值已达到最高,不能使用", player)
			return 0
		end
		local WnegInfo = getWnegInfo(item_id, luck + 1)

		if WnegInfo ~= nil then
			-- local ret = player:remove_item_by_type_pos(item_id, pos, 1)

			local res = RandomUpLuckRes(WnegInfo)

			if res == 0 then
				-- 增加
				player:set_weapon_luck(luck + 1)
				util.alert_msg(4, "您的武器获得了祝福,幸运+1", player)
			elseif res == 2 and luck >= 1 then
				-- 减少
				player:set_weapon_luck(luck - 1)
				util.alert_msg(4, "您的武器被诅咒了,幸运-1", player)
			else
				-- 不变
				util.alert_msg(4, "您的武器幸运值不变,使用无效", player)
			end
		end
	end
end

function change_pk_value( player,value )
	if player:get_pk_value()>=value then
		player:set_pk_value(player:get_pk_value()+value)
		util.alert_msg(4, "PK值减少"..math.abs(value).."点", player)
	end
end

function checkPK_value( player,value )
	return player:get_pk_value()<=0
end
-- set_pk_value
-- get_pk_value
-- sub_pk_value
function onUseItem( player, item_name, item_id, num, pos)
	-- local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,qianghuacount,itupdad,itupdmac,
	-- 	itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount,mZLevel,mFuncId,mFuncParams = player:get_item_data_by_pos(pos)
	local itname,addhp,itacmax,addmp,itmacmax,cd,statusid,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,
	itneedtype,itneedparam,itprice,mNeedReinLv,mFuncId,mFuncParams,mEquipType,mJob,mMax_hp,mMax_mp,mHolyDam,mBaoji_prob,mBaoji_pres = player:get_item_base_data(item_id)
	local params,paramId,add ={},0,0
	local leftUseItmes = tab.getItemDailyLimit(player,item_id)
	if leftUseItmes<num and leftUseItmes <= 0 then
		util.alert_msg(4, "超过每日最大使用次数", player)
		return 0
	elseif leftUseItmes>0 and leftUseItmes<num then
		num = leftUseItmes;
	end
	if mFuncId >0 and #mFuncParams>0 then
		params = util.splitString(mFuncParams, ";")
		if #params>=2 then
			paramId = tonumber(params[1]) or 0
			add = tonumber(params[2]) or 0
		end
	end
	-- print("useItem--",mFuncId,paramId,add)
	if mFuncId==1 and funcParam[paramId] and funcParam[paramId].func then
		if funcParam[paramId].check and funcParam[paramId].check(player,add*num) then return 0 end
		if num > 1 then
			local ret = player:remove_item_by_type_pos(item_id, pos, num)--ret大于0 说明实际数量不足
			funcParam[paramId].func(player,add * (num - ret))
			tab.useItem(player,item_id,num - ret)
			return 0
		else
			funcParam[paramId].func(player,add)
			tab.useItem(player,item_id,num)
		end
	elseif mFuncId==2 then
		if num>1 then
			local ret = player:remove_item_by_type_pos(item_id, pos, num)
			for i=1,num-ret do
				local rand = math.random(1,10000)
				if rand<=add then
					player:set_buff(paramId)
				else
					util.alert_msg(4, "增加buff失败", player)
				end
			end
			tab.useItem(player,item_id,num-ret)
			return 0
		else
			tab.useItem(player,item_id,num)
			local rand = math.random(1,10000)
			if rand<=add then
				player:set_buff(paramId)
			else
				util.alert_msg(4, "增加buff失败", player)
			end
		end
	else
		return 0
	end
end

function onTriggerItem(player, item_name, item_id, num)
	local itname,addhp,itacmax,addmp,itmacmax,cd,statusid,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,
	itneedtype,itneedparam,itprice,mNeedReinLv,mFuncId,mFuncParams,mEquipType,mJob,mMax_hp,mMax_mp,mHolyDam,mBaoji_prob,mBaoji_pres = player:get_item_base_data(item_id)
	local params,paramId,add ={},0,0
	local leftUseItmes = tab.getItemDailyLimit(player,item_id)
	if leftUseItmes<num and leftUseItmes <= 0 then
		util.alert_msg(4, "超过每日最大使用次数", player)
		return 0
	elseif leftUseItmes>0 and leftUseItmes<num then
		num = leftUseItmes;
	end
	if mFuncId >0 and #mFuncParams>0 then
		params = util.splitString(mFuncParams, ";")
		if #params>=2 then
			paramId = tonumber(params[1]) or 0
			add = tonumber(params[2]) or 0
		end
	end
	-- print("useItem--",mFuncId,paramId,add)
	if mFuncId==1 and funcParam[paramId] and funcParam[paramId].func then
		if funcParam[paramId].check and funcParam[paramId].check(player,add) then return 0 end
		if num > 1 then
			local ret = 0-- player:remove_item_by_type_pos(item_id, pos, num)--ret大于0 说明实际数量不足
			funcParam[paramId].func(player,add * (num - ret))
			tab.useItem(player,item_id,num - ret)
		else
			funcParam[paramId].func(player,add)
			tab.useItem(player,item_id,num)
		end
		return true
	elseif mFuncId==2 then
		if num>1 then
			local ret = 0--player:remove_item_by_type_pos(item_id, pos, num)
			for i=1,num-ret do
				local rand = math.random(1,10000)
				if rand<=add then
					player:set_buff(paramId)
				else
					util.alert_msg(4, "增加buff失败", player)
				end
			end
			tab.useItem(player,item_id,num-ret)
			return true
		else
			tab.useItem(player,item_id,num)
			local rand = math.random(1,10000)
			if rand<=add then
				player:set_buff(paramId)
				return true
			else
				util.alert_msg(4, "增加buff失败", player)
			end
		end
	else
		return false
	end
end

function WingTicket(player)

	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	if not gui.moduleGuiButton.checkFuncOpened(player, 10061) then
		util.alert_msg(4, "翅膀功能未开启无法使用", player)
		return 0
	end

	if wingLv < 2 then
		player:set_param(const.PP_WING_LEVEL, 2)
		player:set_wing_lv(70001+util.ppn(player,const.PP_WING_LEVEL));
		player:alert(1,1,"将翅膀培养至<font color='#01ebff'>2阶</font>实力大增！");
		server.info(10000,0,"<font color='#01ebff'>"..player:get_name().."</font>将翅膀成功培养至<font color='#01ebff'>"..(2).."阶</font>实力大增！");
   		server.info(10,0,"<font color='#01ebff'>"..player:get_name().."</font>将翅膀成功培养至<font color='#01ebff'>"..(2).."阶</font>实力大增！<a color='#00ff00' href='event:local_OpenPanel_act35' underline='#00ff00'>我要提升</a></font>");
	else
		player:add_item(24030001, 50, 0)
	end
end

function LvUpDrug(player)
	local lv = player:get_level()
	if lv < 90 then
		player:set_level(lv + 1)
	else
		player:add_exp(10000000)
	end
end

-- local  skill_fydata={
-- [1]={26010011,26010021,26010031,26010031,26010041,26010041,26010051,26010051},
-- [2]={26020001,26020021,26020041,26020041,26020051,26020051,26020011,26020031,26020031},
-- [3]={26030001,26030031,26030011,26030011,26030051,26030051,26030041,26030041,26030021},
-- }

local skill_fydata={
[1]={
	[1]={name="刺杀剑法",		id=26010011,pro=10},
	[2]={name="半月剑法",		id=26010021,pro=10},
	[3]={name="野蛮冲撞",		id=26010031,pro=10},
	[4]={name="烈火剑法",		id=26010041,pro=4},
	[5]={name="逐日剑法",		id=26010051,pro=2},
	[6]={name="怒之刺杀剑法",	id=26010012,pro=1},
	[7]={name="怒之半月剑法",	id=26010022,pro=1},
	[8]={name="怒之野蛮冲撞",	id=26010032,pro=1},
},
[2]={
	[1]= {name="雷电术",		id=26020001,pro=10},
	[2]= {name="地狱雷光",		id=26020021,pro=10},
	[3]= {name="抗拒火环",		id=26020041,pro=10},
	[4]= {name="冰咆哮",		id=26020051,pro=2},
	[5]= {name="魔法盾",		id=26020011,pro=8},
	[6]= {name="火墙",			id=26020031,pro=8},
	[7]= {name="怒之雷电术",	id=26020002,pro=1},
	[8]= {name="怒之地狱雷光",	id=26020022,pro=1},
	[9]= {name="怒之抗拒火环",	id=26020042,pro=1},
	[10]={name="怒之火墙",		id=26020032,pro=1},
},
[3]={
	[1]= {name="灵魂火符"		,id=26030001,pro=8},
	[2]= {name="群体施毒术"		,id=26030031,pro=8},
	[3]= {name="召唤神兽"		,id=26030011,pro=2},
	[4]= {name="群体隐身术"		,id=26030051,pro=10},
	[5]= {name="群体治愈术"		,id=26030041,pro=10},
	[6]= {name="圣甲术"		,id=26030021,pro=10},
	[7]= {name="怒之灵魂火符"	,id=26030002,pro=1},
	[8]= {name="怒之群体隐身术"	,id=26030052,pro=1},
	[9]= {name="怒之群体治愈术"	,id=26030042,pro=1},
	[10]={name="怒之神圣战甲术"	,id=26030022,pro=1},
},

}


--封印技能书
function fyjns(player)
	local job = player:get_job_name()
	if job=="warrior" then
		local pro=0
		local add=0
		for i=1,#skill_fydata[1] do
			pro=skill_fydata[1][i].pro+pro
		end

		local rand=math.random(1,pro)
		for i=1,#skill_fydata[1]  do
			add=skill_fydata[1][i].pro+add
			if rand <=add then

				player:add_item(skill_fydata[1][i].id,1,0)
				break
			end
		end
	end
	if job=="wizard" then
		local pro=0
		local add=0
		for i=1,#skill_fydata[2] do
			pro=skill_fydata[2][i].pro+pro
		end

		local rand=math.random(1,pro)
		for i=1,#skill_fydata[2]  do
			add=skill_fydata[2][i].pro+add
			if rand <=add then
				player:add_item(skill_fydata[2][i].id,1,0)
				break
			end
		end
	end
	if job=="taoist" then
		local pro=0
		local add=0
		for i=1,#skill_fydata[3] do
			pro=skill_fydata[3][i].pro+pro
		end

		local rand=math.random(1,pro)
		for i=1,#skill_fydata[3]  do
			add=skill_fydata[3][i].pro+add
			if rand <=add then
				player:add_item(skill_fydata[3][i].id,1,0)
				break
			end
		end
	end
end

---------------1转首饰box
local data_zhuan1Jewelrybox={
[1]={11110031,11110041,11110051,11110051,11110061,11110061,11110071,11110081,},
[2]={12110031,12110041,12110051,12110051,12110061,12110061,12110071,12110081,},
[3]={13110031,13110041,13110051,13110051,13110061,13110061,13110071,13110081,},
}
function zhuan1Jewelrybox(player)
	local job = player:get_job_name()
	local id=0
	if job=="warrior" then
		id=math.random(1,#data_zhuan1Jewelrybox[1])
		player:add_item(data_zhuan1Jewelrybox[1][id],1,0)
	end
	if job=="wizard" then
		id=math.random(1,#data_zhuan1Jewelrybox[2])
		player:add_item(data_zhuan1Jewelrybox[2][id],1,0)
	end
	if job=="taoist" then
		id=math.random(1,#data_zhuan1Jewelrybox[3])
		player:add_item(data_zhuan1Jewelrybox[3][id],1,0)
	end
end


---------------2转首饰box
local data_zhuan2Jewelrybox={
[1]={11111031,11111041,11111051,11111051,11111061,11111061,11111071,11111081,},
[2]={12111031,12111041,12111051,12111051,12111061,12111061,12111071,12111081,},
[3]={13111031,13111041,13111051,13111051,13111061,13111061,13111071,13111081,},
}
function zhuan2Jewelrybox(player)
	local job = player:get_job_name()
	local id=0
	if job=="warrior" then
		id=math.random(1,#data_zhuan2Jewelrybox[1])
		player:add_item(data_zhuan2Jewelrybox[1][id],1,0)
	end
	if job=="wizard" then
		id=math.random(1,#data_zhuan2Jewelrybox[2])
		player:add_item(data_zhuan2Jewelrybox[2][id],1,0)
	end
	if job=="taoist" then
		id=math.random(1,#data_zhuan2Jewelrybox[3])
		player:add_item(data_zhuan2Jewelrybox[3][id],1,0)
	end
end


---------------4转首饰box
local data_zhuan4Jewelrybox={
[1]={11112031,11112041,11112051,11112051,11112061,11112061,11112071,11112081},
[2]={12112031,12112041,12112051,12112051,12112061,12112061,12112071,12112081},
[3]={13112031,13112041,13112051,13112051,13112061,13112061,13112071,13112081},
}
function zhuan4Jewelrybox(player)
	local job = player:get_job_name()
	local id=0
	if job=="warrior" then
		id=math.random(1,#data_zhuan4Jewelrybox[1])
		player:add_item(data_zhuan4Jewelrybox[1][id],1,0)
	end
	if job=="wizard" then
		id=math.random(1,#data_zhuan4Jewelrybox[2])
		player:add_item(data_zhuan4Jewelrybox[2][id],1,0)
	end
	if job=="taoist" then
		id=math.random(1,#data_zhuan4Jewelrybox[3])
		player:add_item(data_zhuan4Jewelrybox[3][id],1,0)
	end
end

---------------6转首饰box
local data_zhuan6Jewelrybox={
[1]={11113031,11113041,11113051,11113051,11113061,11113061,11113071,11113081,},
[2]={12113031,12113041,12113051,12113051,12113061,12113061,12113071,12113081,},
[3]={13113031,13113041,13113051,13113051,13113061,13113061,13113071,13113081,},
}
function zhuan6Jewelrybox(player)
	local job = player:get_job_name()
	local id=0
	if job=="warrior" then
		id=math.random(1,#data_zhuan6Jewelrybox[1])
		player:add_item(data_zhuan6Jewelrybox[1][id],1,0)
	end
	if job=="wizard" then
		id=math.random(1,#data_zhuan6Jewelrybox[2])
		player:add_item(data_zhuan6Jewelrybox[2][id],1,0)
	end
	if job=="taoist" then
		id=math.random(1,#data_zhuan6Jewelrybox[3])
		player:add_item(data_zhuan6Jewelrybox[3][id],1,0)
	end
end

---------------8转首饰box
local data_zhuan8Jewelrybox={
[1]={11114031,11114041,11114051,11114051,11114061,11114061,11114071,11114081,},
[2]={12114031,12114041,12114051,12114051,12114061,12114061,12114071,12114081,},
[3]={13114031,13114041,13114051,13114051,13114061,13114061,13114071,13114081,},
}
function zhuan8Jewelrybox(player)
	local job = player:get_job_name()
	local id=0
	if job=="warrior" then
		id=math.random(1,#data_zhuan8Jewelrybox[1])
		player:add_item(data_zhuan8Jewelrybox[1][id],1,0)
	end
	if job=="wizard" then
		id=math.random(1,#data_zhuan8Jewelrybox[2])
		player:add_item(data_zhuan8Jewelrybox[2][id],1,0)
	end
	if job=="taoist" then
		id=math.random(1,#data_zhuan8Jewelrybox[3])
		player:add_item(data_zhuan8Jewelrybox[3][id],1,0)
	end
end

---------------10转首饰box
local data_zhuan10Jewelrybox={
[1]={11115031,11115041,11115051,11115051,11115061,11115061,11115071,11115081,},
[2]={12115031,12115041,12115051,12115051,12115061,12115061,12115071,12115081,},
[3]={13115031,13115041,13115051,13115051,13115061,13115061,13115071,13115081,},
}
function zhuan10Jewelrybox(player)
	local job = player:get_job_name()
	local id=0
	if job=="warrior" then
		id=math.random(1,#data_zhuan10Jewelrybox[1])
		player:add_item(data_zhuan10Jewelrybox[1][id],1,0)
	end
	if job=="wizard" then
		id=math.random(1,#data_zhuan10Jewelrybox[2])
		player:add_item(data_zhuan10Jewelrybox[2][id],1,0)
	end
	if job=="taoist" then
		id=math.random(1,#data_zhuan10Jewelrybox[3])
		player:add_item(data_zhuan10Jewelrybox[3][id],1,0)
	end
end
---------------12转首饰box
local data_zhuan12Jewelrybox={
[1]={11116031,11116041,11116051,11116051,11116061,11116061,11116071,11116081,},
[2]={12116031,12116041,12116051,12116051,12116061,12116061,12116071,12116081,},
[3]={13116031,13116041,13116051,13116051,13116061,13116061,13116071,13116081,},
}
function zhuan12Jewelrybox(player)
	local job = player:get_job_name()
	local id=0
	if job=="warrior" then
		id=math.random(1,#data_zhuan12Jewelrybox[1])
		player:add_item(data_zhuan12Jewelrybox[1][id],1,0)
	end
	if job=="wizard" then
		id=math.random(1,#data_zhuan12Jewelrybox[2])
		player:add_item(data_zhuan12Jewelrybox[2][id],1,0)
	end
	if job=="taoist" then
		id=math.random(1,#data_zhuan12Jewelrybox[3])
		player:add_item(data_zhuan12Jewelrybox[3][id],1,0)
	end
end

-----------------------1转武器box
local data_zhuan1Weaponbox={
[1]={id=11110011,job="warrior"},
[2]={id=12110011,job="wizard"},
[3]={id=13110011,job="taoist"},
}
function zhuan1Weaponbox(player)
	local id=math.random(1,#data_zhuan1Weaponbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan1Weaponbox do
			if player:get_job_name()==data_zhuan1Weaponbox[i].job then
				player:add_item(data_zhuan1Weaponbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------------------2转武器box
local data_zhuan2Weaponbox={
[1]={id=11111011,job="warrior"},
[2]={id=12111011,job="wizard"},
[3]={id=13111011,job="taoist"},
}
function zhuan2Weaponbox(player)
	local id=math.random(1,#data_zhuan2Weaponbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan2Weaponbox do
			if player:get_job_name()==data_zhuan2Weaponbox[i].job then
				player:add_item(data_zhuan2Weaponbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------------------3转武器box
local data_zhuan3Weaponbox={
[1]={id=11112011,job="warrior"},
[2]={id=12112011,job="wizard"},
[3]={id=13112011,job="taoist"},
}
function zhuan3Weaponbox(player)
	local id=math.random(1,#data_zhuan3Weaponbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan3Weaponbox do
			if player:get_job_name()==data_zhuan3Weaponbox[i].job then
				player:add_item(data_zhuan3Weaponbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------------------5转武器box
local data_zhuan5Weaponbox={
[1]={id=11113011,job="warrior"},
[2]={id=12113011,job="wizard"},
[3]={id=13113011,job="taoist"},
}
function zhuan5Weaponbox(player)
	local id=math.random(1,#data_zhuan5Weaponbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan5Weaponbox do
			if player:get_job_name()==data_zhuan5Weaponbox[i].job then
				player:add_item(data_zhuan5Weaponbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------------------7转武器box
local data_zhuan7Weaponbox={
[1]={id=11114011,job="warrior"},
[2]={id=12114011,job="wizard"},
[3]={id=13114011,job="taoist"},
}
function zhuan7Weaponbox(player)
	local id=math.random(1,#data_zhuan7Weaponbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan7Weaponbox do
			if player:get_job_name()==data_zhuan7Weaponbox[i].job then
				player:add_item(data_zhuan7Weaponbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------------------9转武器box
local data_zhuan9Weaponbox={
[1]={id=11115011,job="warrior"},
[2]={id=12115011,job="wizard"},
[3]={id=13115011,job="taoist"},
}
function zhuan9Weaponbox(player)
	local id=math.random(1,#data_zhuan9Weaponbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan9Weaponbox do
			if player:get_job_name()==data_zhuan9Weaponbox[i].job then
				player:add_item(data_zhuan9Weaponbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------------------11转武器box
local data_zhuan11Weaponbox={
[1]={id=11116011,job="warrior"},
[2]={id=12116011,job="wizard"},
[3]={id=13116011,job="taoist"},
}
function zhuan11Weaponbox(player)
	local id=math.random(1,#data_zhuan11Weaponbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan11Weaponbox do
			if player:get_job_name()==data_zhuan11Weaponbox[i].job then
				player:add_item(data_zhuan11Weaponbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end








-----------1转衣服box
local data_zhuan1Clothbox={
[1]={id=11110021,job="warrior",gender="male"},
[2]={id=11110091,job="warrior",gender="female"},
[3]={id=12110021,job="wizard",gender="male"},
[4]={id=12110091,job="wizard",gender="female"},
[5]={id=13110021,job="taoist",gender="male"},
[6]={id=13110091,job="taoist",gender="female"},
}
function zhuan1Clothbox(player)
	local id=math.random(1,#data_zhuan1Clothbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan1Clothbox do
			if player:get_job_name()==data_zhuan1Clothbox[i].job and data_zhuan1Clothbox[i].gender == player:get_gender_name() then
				player:add_item(data_zhuan1Clothbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------2转衣服box
local data_zhuan2Clothbox={
[1]={id=11111021,job="warrior",gender="male"},
[2]={id=11111091,job="warrior",gender="female"},
[3]={id=12111021,job="wizard",gender="male"},
[4]={id=12111091,job="wizard",gender="female"},
[5]={id=13111021,job="taoist",gender="male"},
[6]={id=13111091,job="taoist",gender="female"},
}
function zhuan2Clothbox(player)
	local id=math.random(1,#data_zhuan2Clothbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan2Clothbox do
			if player:get_job_name()==data_zhuan2Clothbox[i].job and data_zhuan2Clothbox[i].gender == player:get_gender_name() then
				player:add_item(data_zhuan2Clothbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------3转衣服box
local data_zhuan3Clothbox={
[1]={id=11112021,job="warrior",gender="male"},
[2]={id=11112091,job="warrior",gender="female"},
[3]={id=12112021,job="wizard",gender="male"},
[4]={id=12112091,job="wizard",gender="female"},
[5]={id=13112021,job="taoist",gender="male"},
[6]={id=13112091,job="taoist",gender="female"},
}
function zhuan3Clothbox(player)
	local id=math.random(1,#data_zhuan3Clothbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan3Clothbox do
			if player:get_job_name()==data_zhuan3Clothbox[i].job and data_zhuan3Clothbox[i].gender == player:get_gender_name() then
				player:add_item(data_zhuan3Clothbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------5转衣服box
local data_zhuan5Clothbox={
[1]={id=11113021,job="warrior",gender="male"},
[2]={id=11113091,job="warrior",gender="female"},
[3]={id=12113021,job="wizard",gender="male"},
[4]={id=12113091,job="wizard",gender="female"},
[5]={id=13113021,job="taoist",gender="male"},
[6]={id=13113091,job="taoist",gender="female"},
}
function zhuan5Clothbox(player)
	local id=math.random(1,#data_zhuan5Clothbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan5Clothbox do
			if player:get_job_name()==data_zhuan5Clothbox[i].job and data_zhuan5Clothbox[i].gender == player:get_gender_name() then
				player:add_item(data_zhuan5Clothbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------7转衣服box
local data_zhuan7Clothbox={
[1]={id=11114021,job="warrior",gender="male"},
[2]={id=11114091,job="warrior",gender="female"},
[3]={id=12114021,job="wizard",gender="male"},
[4]={id=12114091,job="wizard",gender="female"},
[5]={id=13114021,job="taoist",gender="male"},
[6]={id=13114091,job="taoist",gender="female"},
}
function zhuan7Clothbox(player)
	local id=math.random(1,#data_zhuan7Clothbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan7Clothbox do
			if player:get_job_name()==data_zhuan7Clothbox[i].job and data_zhuan7Clothbox[i].gender == player:get_gender_name() then
				player:add_item(data_zhuan7Clothbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------9转衣服box
local data_zhuan9Clothbox={
[1]={id=11115021,job="warrior",gender="male"},
[2]={id=11115091,job="warrior",gender="female"},
[3]={id=12115021,job="wizard",gender="male"},
[4]={id=12115091,job="wizard",gender="female"},
[5]={id=13115021,job="taoist",gender="male"},
[6]={id=13115091,job="taoist",gender="female"},
}
function zhuan9Clothbox(player)
	local id=math.random(1,#data_zhuan9Clothbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan9Clothbox do
			if player:get_job_name()==data_zhuan9Clothbox[i].job and data_zhuan9Clothbox[i].gender == player:get_gender_name() then
				player:add_item(data_zhuan9Clothbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end


-----------11转衣服box
local data_zhuan11Clothbox={
[1]={id=11116021,job="warrior",gender="male"},
[2]={id=11116091,job="warrior",gender="female"},
[3]={id=12116021,job="wizard",gender="male"},
[4]={id=12116091,job="wizard",gender="female"},
[5]={id=13116021,job="taoist",gender="male"},
[6]={id=13116091,job="taoist",gender="female"},
}
function zhuan11Clothbox(player)
	local id=math.random(1,#data_zhuan11Clothbox)
	if player:num_bag_black()>=1 then
		for i=1,#data_zhuan11Clothbox do
			if player:get_job_name()==data_zhuan11Clothbox[i].job and data_zhuan11Clothbox[i].gender == player:get_gender_name() then
				player:add_item(data_zhuan11Clothbox[i].id,1);
			end
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end
-----功勋宝箱
local data_gxbaoxiang={
[1]={id=32000001,pro=10},
[2]={id=23090001,pro=1},
[3]={id=23060001,pro=1},
[4]={id=32000002,pro=1},
[5]={id=21000003,pro=1},--
[6]={id=23010001,pro=1},
}
function gxbaoxiang(player)
	local count = 0
	local rand = math.random(1,15)
	for k,v in ipairs(data_gxbaoxiang) do
		count=count+v.pro
		if rand<=count then
			player:add_item(v.id,1,1)
			gui.PanelGongXun.openBoxRecord(player,tab.getItemName(v.id))
			player:push_lua_table("PanelGongXun",util.encode({cmd="updateGongXunFly",typeId=v.id}))
			break
		end
	end
end

function openPanelWing(player)
	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	if not gui.moduleGuiButton.checkFuncOpened(player, 10061) then
		util.alert_msg(4, "翅膀功能未开启无法使用", player)
	else
		player:push_lua_table("open",util.encode({name = "btn_main_wing"}))

	end
	return 0
end

function lockSkillBookTip(player)
	util.alert_msg(4, "技能书未解封，暂时不可使用", player)
	return 0
end


local data_GuildBox={
[1]={name="羽毛",id=24030001,num=2},
[2]={name="金条(中)",id=23060002,num=1},
[3]={name="绑定元宝",id=40000006,num=500},
[4]={name="副本卷轴",id=32000001,num=2},
[5]={name="玛雅卷轴",id=32000002,num=1},--
[6]={name="龙心碎片(小)",id=23020001,num=1},
[7]={name="狼牙碎片(小)",id=23030001,num=1},
[8]={name="雷霆法神令牌",id=32000005,num=1},
[9]={name="皓月法神令牌",id=32000006,num=1},
[10]={name="双倍经验(8小时)",id=23080004,num=1},

}
function onUseGuildBox(player)
	local guild=player:get_guild()
	local rand = math.random(1,10)
	if player:num_bag_black()>=1 then
		if guild~="" then
			player:add_item(data_GuildBox[rand].id,data_GuildBox[rand].num,1)
			--br /><a color='969696'>羽毛 X 2<br />金条(中) X 1<br />500绑定元宝 X 1<br />副本卷轴 X 2<br />玛雅卷轴 X 1<br />龙心碎片(小) X 1<br />狼牙碎片(小) X 1<br />雷霆法神令牌 X 1<br />皓月法神令牌 X 1<br />双倍经验(8小时) X 1</font>
		else
			util.alert_msg(4, "暂未加入帮会，无法使用", player)
			return 0
		end
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end
end

local data_Cjflbx={
[1]={name="龙心碎片(中)",id=23020002,num=1},
[2]={name="金条(大)",id=23060003,num=1},
[3]={name="绑定元宝",id=40000006,num=500},
[4]={name="副本卷轴",id=32000001,num=2},
[5]={name="玛雅卷轴",id=32000002,num=1},--
[6]={name="雷霆法神令牌",id=32000005,num=1},
[7]={name="皓月法神令牌",id=32000006,num=1},
[8]={name="双倍经验(8小时)",id=23080004,num=1},
}
function Cjflbx(player)
	if player:get_vcoin()>=1  then
		for i=1,#data_Cjflbx do
			player:add_item(data_Cjflbx[i].id,data_Cjflbx[i].num,1)
		end
		player:sub_vcoin(1,"超级返利箱")
	else
		util.alert_msg(4, "元宝不足!", player)
		return 0
	end

end

local Sqbjbx_data={
[1]={name="热血护心镜",id=11240011,num=1},
[2]={name="热血面巾",id=11240012,num=1},
[3]={name="热血龙骨",id=11240013,num=1},
[4]={name="热血虎符",id=11240014,num=1},
}
function Sqbjbx(player)
	if player:num_bag_black()>=1 then
		local rand = math.random(1,4)
		player:add_item(Sqbjbx_data[rand].id,Sqbjbx_data[rand].num)
	else
		util.alert_msg(4, "背包空间不足!", player)
		return 0
	end

end

-- BOSS拼图触发
function bosspictrue(player,item_name,item_id,num,pos)
	if not gui.PanelBossPictrue.usePictrue(player,item_id) then
		return 0
	end
end

-----------------------------------------时装模块----------------------------

local use_fashion={
---武器
	[15100011]={give=15100012},
	[15100013]={give=15100014},
	[15100022]={give=15100012},
	[15100023]={give=15100013},
	[15100024]={give=15100014},
	[15100025]={give=15100015},
---衣服
	[15200001]={give=15200002},
	[15200003]={give=15200004},
	[15200005]={give=15200006},
	[15200011]={give=15200012},
	[15200013]={give=15200014},
	[15200015]={give=15200016},
	[15200021]={give=15200022},
	[15200023]={give=15200024},
	[15200025]={give=15200026},
	[15200027]={give=15200028},
	[15200029]={give=15200030},
	[15200031]={give=15200032},
	[15200033]={give=15200034},
	[15200035]={give=15200036},
	[15200037]={give=15200038},
	[15200039]={give=15200040},
	[15200041]={give=15200042},
	[15200043]={give=15200044},
	[15200045]={give=15200046},
	[15200047]={give=15200048},
	[15200049]={give=15200050},
	[15200051]={give=15200052},
	[15200053]={give=15200054},
	[15200055]={give=15200056},
	[15200057]={give=15200058},
	[15200059]={give=15200060},
	[15200061]={give=15200062},
	[15200063]={give=15200064},
	[15200065]={give=15200066},
	[15200067]={give=15200068},

--翅膀
	[15300020]={give=15300010},
	[15300021]={give=15300011},
	[15300022]={give=15300012},
	[15300023]={give=15300013},
	[15300024]={give=15300014},
	[15300025]={give=15300015},


}

function fashion (player, item_name, item_id, num, pos)
	--print("000000000")
	local v = use_fashion[item_id]
	if v then
		--判断
		local ret,position= player:check_item_fashion(v.give)---b=pos

		if ret==1  then --同一限时物品
			if position>=5000  or position<0 then
				local id,creattime,lasttime=player:get_item_crtaetime_by_pos(position)
				local nowlasttime=player:get_item_lasttime(v.give)
				player:set_item_lasttime(position,id,lasttime+nowlasttime);
				player:remove_item_by_type_pos(item_id, pos, 1)
				--print("------",ret,pos,itionid,creattime,lasttime,nowlasttime)
				util.alert_msg(4, "限时时装时间增加", player)
			end
		elseif ret==2 then --衣橱限时  背包不限时
			-- print("------",ret,position)---把衣橱的限时物品移除  给新的永久物品
			local id,creattime,lasttime=player:get_item_crtaetime_by_pos(position)
			local result = {
				str = "是否将限时时装变更为永久时装",
				labelConfirm = "确定",labelCancel = "取消",
				callFunc = "item.chufa.fashion",

				book = util.encode({
					actionid = "fashion",
					id = id,
					position = position,
					give = v.give,
					pos = pos,
					item_id=item_id,
				}),
			}

			player:push_lua_table("showConfirm",util.encode(result))
			return 0

		elseif ret==3 then --衣橱不限时  背包限时

			util.alert_msg(4, "你已拥有永久时装无法使用限时时装", player)
			return 0
		elseif ret==4 then --都不限时
			util.alert_msg(4, "你已拥有该时装无法重复使用", player)
			return 0
		elseif ret==5 then --没有同id的物品
			player:add_item(v.give,1,1)
			player:remove_item_by_type_pos(item_id, pos, 1)
			--util.alert_msg(4, "放入衣橱", player)
		elseif ret==6 then --空衣橱
			player:add_item(v.give,1,1)
			player:remove_item_by_type_pos(item_id, pos, 1)
			--util.alert_msg(4, "放入空衣橱", player)
		end
		-- if position>=5000  or position<0 then
		-- 	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
		-- 	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,fun,mm,funid1,funid1,last= player:get_item_data_by_pos(position);
		-- 	print("0-0-0====",last)
		-- 	player:set_item_lasttime(position,ittype_id,last+600);

		-- 	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
		-- 	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,fun,mm,funid1,funid1,last2= player:get_item_data_by_pos(position);
		-- 	print("1-2-2====",last2)
		-- end

	end
end

----------时装
function getfashionlook(player,commond)
	local result = {}
	local data = {
		[1] = {id=15200002,male = 25001, female = 15001},
		[2] = {id=15200004,male = 25001, female = 15001},
		[3] = {id=15200006,male = 25001, female = 15001},
		[4] = {id=15200012,male = 25002, female = 15002},
		[5] = {id=15200014,male = 25002, female = 15002},
		[6] = {id=15200016,male = 25002, female = 15002},
		[7] = {id=15200022,male = 25000, female = 15000},
		[8] = {id=15200024,male = 25000, female = 15000},
		[9] = {id=15200026,male = 25000, female = 15000},
		[10] = {id=15200028,male = 25003, female = 15003},
		[11] = {id=15200030,male = 25003, female = 15003},
		[12] = {id=15200032,male = 25003, female = 15003},
		[13] = {id=15200034,male = 25004, female = 15004},
		[14] = {id=15200036,male = 25004, female = 15004},
		[15] = {id=15200038,male = 25004, female = 15004},
		[16] = {id=15200040,male = 25005, female = 15005},
		[17] = {id=15200042,male = 25005, female = 15005},
		[18] = {id=15200044,male = 25005, female = 15005},
		[19] = {id=15200046,male = 25006, female = 15006},
		[20] = {id=15200048,male = 25006, female = 15006},
		[21] = {id=15200050,male = 25006, female = 15006},
		[22] = {id=15200052,male = 25007, female = 15007},
		[23] = {id=15200054,male = 25007, female = 15007},
		[24] = {id=15200056,male = 25007, female = 15007},
		[25] = {id=15200058,male = 25008, female = 15008},
		[26] = {id=15200060,male = 25008, female = 15008},
		[27] = {id=15200062,male = 25008, female = 15008},
		[28] = {id=15200064,male = 25009, female = 15009},
		[29] = {id=15200066,male = 25009, female = 15009},
		[30] = {id=15200068,male = 25009, female = 15009},

	}
	local pData = util.decode(commond)
	if pData.actionid == "reqfashionData" then

		result.cmd="dress_fashion"
		result.data=data
		player:push_lua_table("Fashion",util.encode(result))
	end
end
ckpanel.add_listener("item.chufa.getfashionlook",item.chufa.getfashionlook)


function ChangeItem_lasttiem(player,command)
	local pData = util.decode(command)
	if pData.actionid == "fashion" then
		local id = pData.id
		local position = pData.position
		local give = pData.give
		local pos=pData.pos
		local item_id=pData.item_id
		player:remove_item_by_type_pos(id, position, 1)---移除限时物品
		player:add_item(give,1,1)
		--player:set_item_lasttime(position,id,0);
		player:remove_item_by_type_pos(item_id, pos, 1)
		--util.alert_msg(4, "衣橱物品替换为永久时间", player)
	else

	end
end

ckpanel.add_listener("item.chufa.fashion",item.chufa.ChangeItem_lasttiem)







----------------------------









































