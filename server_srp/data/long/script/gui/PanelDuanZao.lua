module(... ,package.seeall)
local MAXLEVEL = 20

function handlePanelData(player,commond)
	-- print(player:get_monpoint_data(110,10))
	-- getEquipType(player,11100001)
	-- getCurLevelAdd(player,1,2)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "curQiangHuaEquip" then
		sendToClient(player,data.params.pos)	
	elseif data.actionid == "startQiangHua" then 
		startQiangHua(player,data.params.pos,data.params.flag1,data.params.flag2)

	elseif data.actionid == "curJiChengEquip" then 
		sendToClientTransfer(player,data.params.pos)
	elseif data.actionid == "reqNewLevelShow" then
		updateTransfered(player,data.params.pos,data.params.level)
	elseif data.actionid == "startJiCheng" then
		onShiftItem(player,data.params)

	elseif data.actionid == "reqBuyData" then
		senderShopData(player)
	elseif data.actionid == "shopBuy" then
		buyShopItem(player,data.params.index)
	elseif data.actionid == "quickQianghua" then
		quickQianghua(player,data.pos,data.nextlevel,data.id)
	end
end

----------------------------------------------强化装备------------------------------------------------
--chipName:材料名 
--chipNum：需要的材料数 
--chipPrice：材料单价
--needMoney：需要的金币数 
--needVcion：100%成功需要的元宝数 
--winPro：强化基础成功率
local qhTable = {
	--前10级概率提升
	[1] ={chipName="黑铁矿石",chipTypeId=24020001,chipNum=1,  chipPrice=50, needMoney=200000,  needVcion=0,   winPro=1},
	[2] ={chipName="黑铁矿石",chipTypeId=24020001,chipNum=2,  chipPrice=50, needMoney=400000,  needVcion=9,   winPro=0.94},
	[3] ={chipName="黑铁矿石",chipTypeId=24020001,chipNum=3,  chipPrice=50, needMoney=1000000, needVcion=28,  winPro=0.89},
	[4] ={chipName="黑铁矿石",chipTypeId=24020001,chipNum=4,  chipPrice=50, needMoney=1600000, needVcion=84,  winPro=0.84},
	[5] ={chipName="黑铁矿石",chipTypeId=24020001,chipNum=5,  chipPrice=50, needMoney=3400000, needVcion=252, winPro=0.79},
	[6] ={chipName="青铜矿石",chipTypeId=24020002,chipNum=1,  chipPrice=100,needMoney=4200000, needVcion=758, winPro=0.74},
	[7] ={chipName="青铜矿石",chipTypeId=24020002,chipNum=3,  chipPrice=100,needMoney=5200000, needVcion=1517,winPro=0.69},
	[8] ={chipName="青铜矿石",chipTypeId=24020002,chipNum=6,  chipPrice=100,needMoney=6200000, needVcion=3034,winPro=0.64},
	[9] ={chipName="青铜矿石",chipTypeId=24020002,chipNum=10, chipPrice=100,needMoney=7400000, needVcion=6069,winPro=0.59},
	[10]={chipName="青铜矿石",chipTypeId=24020002,chipNum=15, chipPrice=100,needMoney=8600000, needVcion=9104,winPro=0.54},
	--后10级养成
	[11]={chipName="白银矿石",chipTypeId=24020003,chipNum=12, chipPrice=200,needMoney=10200000,needVcion=0,winPro=1, maxUp=60},--maxUp:养成值达到次便自动强化升级
	[12]={chipName="白银矿石",chipTypeId=24020003,chipNum=15, chipPrice=200,needMoney=12000000,needVcion=0,winPro=1, maxUp=75},
	[13]={chipName="白银矿石",chipTypeId=24020003,chipNum=19, chipPrice=200,needMoney=14000000,needVcion=0,winPro=1, maxUp=95},
	[14]={chipName="白银矿石",chipTypeId=24020003,chipNum=24, chipPrice=200,needMoney=16200000,needVcion=0,winPro=1, maxUp=120},
	[15]={chipName="白银矿石",chipTypeId=24020003,chipNum=30, chipPrice=200,needMoney=18400000,needVcion=0,winPro=1, maxUp=150},
	[16]={chipName="紫金矿石",chipTypeId=24020004,chipNum=12, chipPrice=500,needMoney=20600000,needVcion=0,winPro=1, maxUp=70},
	[17]={chipName="紫金矿石",chipTypeId=24020004,chipNum=16, chipPrice=500,needMoney=23000000,needVcion=0,winPro=1, maxUp=80},
	[18]={chipName="紫金矿石",chipTypeId=24020004,chipNum=18, chipPrice=500,needMoney=25600000,needVcion=0,winPro=1, maxUp=90},
	[19]={chipName="紫金矿石",chipTypeId=24020004,chipNum=21, chipPrice=500,needMoney=28400000,needVcion=0,winPro=1, maxUp=105},
	[20]={chipName="紫金矿石",chipTypeId=24020004,chipNum=27, chipPrice=500,needMoney=30000000,needVcion=0,winPro=1, maxUp=135},

	[21]={chipName="紫金矿石",chipTypeId=24020004,chipNum=12,chipPrice=500,needMoney=30000000,needVcion=100,winPro=0, maxUp=60},
}

function sendToClient(player,pos)
	local result = {
		cmd="updateQiangHua",
		dataTable={},
		
	}
	local level = getQiangHuaLevel(player,pos) or 0--强化等级
	local itemData = qhTable[level+1]
	local data = {}
	data.chipTypeId=itemData.chipTypeId
	data.chipNum=itemData.chipNum
	data.ownNum=player:num_item(itemData.chipName)
	data.needMoney=itemData.needMoney
	data.needVcion=itemData.needVcion
	data.winPro=itemData.winPro
	data.level=level
	data.stoneName=itemData.chipName
	if itemData.maxUp then
		data.maxUp=itemData.maxUp
	end
	data.yangLev=math.abs(getYangChengJie(player,pos))
	if level>=1 and level<=19 then
		data.curValues=getCurLevelAdd(player,pos,level)
		data.nextValues=getCurLevelAdd(player,pos,level+1)
	end
	if level==0 then
		data.nextValues,data.curValues=getCurLevelAdd(player,pos,level+1,true)
	end
	if level==20 then
		data.curValues,data.nextValues=getCurLevelAdd(player,pos,level,true)
	end 
	result.dataTable=data
	player:push_lua_table("PanelDuanZao",util.encode(result))
end

--根据强化等级返回加成的数据
function getCurLevelAdd(player,pos,level,double)
	local result = {}
	local result2 = {}
	local id = getQiangHuaLevel_id(player,pos)
	local equipType = getEquipType(player,id)
	local job = player:get_job_name()
	local dc,dcmax,ac,acmax,sc,scmax,mc,mcmax,mac,macmax=0,0,0,0,0,0,0,0,0,0
	local dc2,dcmax2,ac2,acmax2,sc2,scmax2,mc2,mcmax2,mac2,macmax2=0,0,0,0,0,0,0,0,0,0
	if level>=1 and level<=20 then
		dc,dcmax,ac,acmax,sc,scmax,mc,mcmax,mac,macmax=player:get_monpoint_data(equipType,level)
		-- if (level-1)>0 then
		-- 	dc2,dcmax2,ac2,acmax2,sc2,scmax2,mc2,mcmax2,mac2,macmax2=player:get_monpoint_data(equipType,level-1)
		-- end
		-- print("level=",level)
		-- print(player:get_monpoint_data(equipType,level))
		if job=="warrior" and (dc>0 or dcmax>0) then
			table.insert(result,"物理攻击："..(dc-dc2).."-"..(dcmax-dcmax2))
			if double then
				if level==1 then
					table.insert(result2,"物理攻击：0-0")
				elseif level==20 then
					table.insert(result2,"强化等级已满")
				end
			end
		end
		if job=="wizard" and (ac>0 or acmax>0) then
			table.insert(result,"魔法攻击："..(ac-ac2).."-"..(acmax-acmax2))
			if double then
				if level==1 then
					table.insert(result2,"魔法攻击：0-0")
				elseif level==20 then
					table.insert(result2,"强化等级已满")
				end
			end
		end
		if job=="taoist" and (sc>0 or scmax>0) then
			table.insert(result,"道术攻击："..(sc-sc2).."-"..(scmax-scmax2))
			if double then
				if level==1 then
					table.insert(result2,"道术攻击：0-0")
				elseif level==20 then
					table.insert(result2,"强化等级已满")
				end
			end
		end
		if mc>0 or mcmax>0 then
			table.insert(result,"物理防御："..(mc-mc2).."-"..(mcmax-mcmax2))
			if double then
				if level==1 then
					table.insert(result2,"物理防御：0-0")
				elseif level==20 then
					table.insert(result2,"强化等级已满")
				end
			end
		end
		if mac>0 or macmax>0 then
			table.insert(result,"魔法防御："..(mac-mac2).."-"..(macmax-macmax2))
			if double then
				if level==1 then
					table.insert(result2,"魔法防御：0-0")
				elseif level==20 then
					table.insert(result2,"强化等级已满")
				end
			end
		end
	end
	return result,result2
end

--pos：强化该pos位上的装备
--flag1：是否勾选100%成功
--flag2：是否强化石不足元宝代替
function startQiangHua(player,pos,flag1,flag2)
	local level = getQiangHuaLevel(player,pos)
	local id = getQiangHuaLevel_id(player,pos)
	if level>=MAXLEVEL then
		player:alert(1, 1, "该装备已达最大强化等级！")
		return
	end
	local nextLev=level+1
	local itemData = qhTable[nextLev]
	if getAllMoney(player)<itemData.needMoney then
		player:alert(1, 1, "金币不足！")
		return
	end
	if player:num_item(itemData.chipName)>=itemData.chipNum then
		qiangHuaResult(player,0,itemData,pos,level,id,flag1,flag2)
	else
		if flag2 then
			local quNum=itemData.chipNum-player:num_item(itemData.chipName)
			qiangHuaResult(player,quNum,itemData,pos,level,id,flag1,flag2)
		else
			player:alert(1, 1, "强化石不足！")
		end
	end
end
--10级后 强化养成逻辑
function qiangHuaYangCheng(player,id,pos,nextlv)--player:item_add_upd_failed(pos, -1);
	local nextData = qhTable[nextLev]
	local jie = getYangChengJie(player,pos)
	if (jie-1)==-5 then
		-- print("2222222222",nextlv,jie)
		player:item_add_upd_failed(pos,-100)
		player:set_item_uplv(id,pos,nextlv)
		severTip(player,nextlv,id)
		player:item_add_upd_failed(pos,104)--升级成功后重置养成我0
		player:push_lua_table("PanelDuanZao",util.encode({cmd="qiangHuaSucceed",}))
	else
		player:item_add_upd_failed(pos,-1)--提升养成阶段
		-- print("3333333333",nextlv,jie,getYangChengJie(player,pos))
	end
end

--强化逻辑
function qiangHuaResult(player,quNum,itemData,pos,lv,id,flag1,flag2)
	local nextlv=lv+1
	if player:get_vcoin()<(quNum*itemData.chipPrice+(flag1 and itemData.needVcion or 0)) then 
		-- player:alert(1, 1, "元宝不足！")
		showChongzhi(player)
		return
	end
	player:remove_item(itemData.chipName,itemData.chipNum-quNum)
	-- player:sub_gamemoney(itemData.needMoney)
	subMoneyOrBind(player,itemData.needMoney)
	if flag2 and quNum>0 then
		player:sub_vcoin(quNum*itemData.chipPrice,"元宝代石")
	end

	if flag1 then--走扣元宝必成
		
		player:sub_vcoin(itemData.needVcion,"装备强化")
		if nextlv>10 then
			qiangHuaYangCheng(player,id,pos,nextlv)
		else
			player:set_item_uplv(id,pos,nextlv)
			severTip(player,nextlv,id)
			player:push_lua_table("PanelDuanZao",util.encode({cmd="qiangHuaSucceed",}))
		end
		-- player:alert(1, 1, "强化成功！")
		gui.PanelGongXun.checkTaskComplete(player,12)
	else--走概率
		local randNum = math.random(1,100)
		if randNum<=itemData.winPro*100 then
			if nextlv>10 then
				qiangHuaYangCheng(player,id,pos,nextlv)
			else
				player:set_item_uplv(id,pos,nextlv)
				severTip(player,nextlv,id)
				player:push_lua_table("PanelDuanZao",util.encode({cmd="qiangHuaSucceed",}))
			end
			-- player:alert(1, 1, "强化成功！")
			gui.PanelGongXun.checkTaskComplete(player,12)
		else
			-- player:alert(1, 1, "强化失败！")
			player:push_lua_table("PanelDuanZao",util.encode({cmd="qiangHuaFailed",}))
		end
	end
	player:recal_attr()--刷新数据
	sendToClient(player,pos)--强化结果后刷新客户端
end

function severTip(player,level,id)
	if level>=8 then
		server.info(10, 1, "<font color='#fff843'>玩家<font color='#00ccff'>"..player:get_name().."</font>将"..tab.getItemColorName(id).."强化至"..level.."级！<a color='#00ff00' href='event:local_OpenPanel_act25' underline='#00ff00'>我要强化</a></font>")
	end
	player:set_param(const["PP_UPGRADE_EQUIP_NUM"..level],util.ppn(player, const["PP_UPGRADE_EQUIP_NUM"..level])+1)--强化到该等级的装备数量加1
	gui.PanelAchieve.triggerTaskEvent(player,"upEquipToLevel",0)
end

--取当前pos位装备的强化等级
function getQiangHuaLevel(player,pos)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos);
	return qianghualv
end
function getQiangHuaLevel_id(player,pos)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos);
	return ittype_id
end

--返回养成的阶段 分5个阶段 第5阶段满就自动升级 后重置阶段为0
function getYangChengJie(player,pos)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos);
	return mUpdFailedCount
end

--1:武器 2:衣服 3:头盔 4:项链 5:手镯 6:戒指 7:腰带 8:靴子, 11:玉佩 12:护盾 13:护心镜 14:面巾 15:龙心 16:狼牙 17:龙骨 18:虎符
local equipType={
	[1]=100,
	[2]=110,
	[3]=120,
	[4]=130,
	[5]=140,
	[6]=160,
	[7]=190,
	[8]=180,

	[11]=200,
	[12]=210,
	[13]=220,
	[14]=230,
	[15]=240,
	[16]=250,
	[17]=260,
	[18]=270,
}

function getEquipType(player,id)
	local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice,itneedreinlv,itfuncid,itfuncparam,itequiptype = player:get_item_base_data(id)
	-- print(itequiptype,"====",equipType[itequiptype])
	return equipType[itequiptype]
end

----------------------------------------------强化转移------------------------------------------------
--继承消耗表
local shiftTable = {
	[1] ={needMoney=2000,  needVcion=0},
	[2] ={needMoney=5000,  needVcion=0},
	[3] ={needMoney=10000, needVcion=0},
	[4] ={needMoney=30000, needVcion=0},
	[5] ={needMoney=100000,needVcion=0},
	[6] ={needMoney=200000,needVcion=0},
	[7] ={needMoney=400000,needVcion=0},
	[8] ={needMoney=600000,needVcion=0},
	[9] ={needMoney=0,     needVcion=60},
	[10]={needMoney=0,     needVcion=90},
	[11]={needMoney=0,     needVcion=136},
	[12]={needMoney=0,     needVcion=204},
	[13]={needMoney=0,     needVcion=274},
	[14]={needMoney=0,     needVcion=356},
	[15]={needMoney=0,     needVcion=450},
	[16]={needMoney=0,     needVcion=556},
	[17]={needMoney=0,     needVcion=674},
	[18]={needMoney=0,     needVcion=804},
	[19]={needMoney=0,     needVcion=946},
	[20]={needMoney=0,     needVcion=1100},
}

function sendToClientTransfer(player,pos)
	local result = {
		cmd="updateTransfer",
		dataTable={},
	}
	local level = getQiangHuaLevel(player,pos) or 1--强化等级
	local itemData = shiftTable[level]
	itemData.vcion=player:get_vcoin()
	itemData.oldValues=getCurLevelAdd(player,pos,level)

	result.dataTable=itemData
	player:push_lua_table("PanelDuanZao",util.encode(result))
end

--刷新继承者继承后的强化属性
function updateTransfered(player,pos,level)
	-- print("33333==",level)
	local result = {
		cmd="updateNewValue",
		newTable=getCurLevelAdd(player,pos,level),
	}
	player:push_lua_table("PanelDuanZao",util.encode(result))
end

--转移逻辑
function onShiftItem(player,param)
	local result ={}
	local oldId,oldPos = param.oldTypeid,param.oldPos;
	local newId,newPos = param.newTypeid,param.newPos;
	if oldId and oldPos and newId and newPos then
		if oldId == 29002 or newId == 29002 then player:alert(1,1,"试用武器不可操作！") return end 
		-- if math.floor(oldId/10000)~=math.floor(newId/10000) then player:alert(1,1,"装备类型不一致，不可转移！") return end
		local oldItem = player:get_item(oldId,oldPos)
		local newItem = player:get_item(newId,newPos)
		if not oldItem or not newItem then return end
		local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
		qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(oldPos);

		local nittype_id,nitbind,nituse_bind,nitjpin,nitdura,nitdurmax,nitaddac,nitaddmac,nitadddc,nitaddmc,nitaddsc,nitluck,nqianghualv,
		nqianghuacount,nitupdad,nitupdmac,nitupddc,nitupdmc,nitupdsc,nprotect,naddhp,naddmp,nremaintime,nmUpdMaxCount,nmUpdFailedCount = player:get_item_data_by_pos(newPos);

		if qianghualv and qianghualv>0 and nqianghualv and nqianghualv==0 then
			local itemData = shiftTable[qianghualv]
			local money = itemData.needMoney
			local vcoin = itemData.needVcion
			if money>0 then
				if getAllMoney(player)>money then 
					-- player:sub_gamemoney(money)
					subMoneyOrBind(player,money)
				else
					player:alert(1,1,"金币不足，无法转移！")
					return
				end
			end
			if vcoin>0 then
				if player:get_vcoin()>vcoin then 
					player:sub_vcoin(vcoin,"强化转移")
				else 
					-- player:alert(1,1,"元宝不足，无法转移！")
					showChongzhi(player)
					return
				end
			end
			-- for i=1,qianghualv do
			-- 	player:item_upgrade(newPos,0,0,0,0,0);
			-- end
			player:set_item_uplv(nittype_id,newPos,qianghualv)
			player:set_item_uplv(ittype_id,oldPos,0)
			player:item_add_upd_failed(oldPos,math.abs(mUpdFailedCount))
			player:item_add_upd_failed(newPos,mUpdFailedCount)
			player:alert(1,1,"转移成功！")
			-- player:set_item_data(ittype_id,oldPos,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,0,qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp);
			player:push_lua_table("PanelDuanZao",util.encode({cmd="successTransfer"}))
			player:recal_attr()--刷新数据
		end
	end
end

----------------------------------------------随身商店------------------------------------------------
local tableShop = {
	[1]={id=23060003,name="金砖（大）",bmoney=3000000,vcion=88},
}

function senderShopData(player)
	local result = {
		cmd="senderShopData",
		data=util.clone(tableShop),
		tab.Store.getItemsByType(player,6),
	}
	player:push_lua_table("PanelDuanZao",util.encode(result))
end

function buyShopItem(player,index)
	if not tableShop[index] then return end
	local itemData = tableShop[index]
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	if player:get_vcoin()>=itemData.vcion then
		local yuTimes = tab.getItemDailyLimit(player,itemData.id)
		if yuTimes>0 then
			player:sub_vcoin(itemData.vcion,"随身商店")
			player:add_gamemoney_bind(itemData.bmoney)
			tab.useItem(player,itemData.id,1)
		else
			player:alert(1,1,"超过今日最大使用次数！")
		end
		-- player:add_item(itemData.id,1,1)
		-- player:use_item(itemData.id)
		-- player:alert(1, 1, "成功购买"..itemData.name)
	else
		-- player:alert(1,1,"元宝不足");
		showChongzhi(player)
	end
end


----------------------------------------------强化-继承关于金币的操作-------------------------------------------------
--计算金币+绑定金币
function getAllMoney(player)
	return (player:get_gamemoney()+player:get_gamemoney_bind())
end

--关于扣金币或者绑定金币
function subMoneyOrBind(player,num)
	local money = player:get_gamemoney()
	local bindMoney = player:get_gamemoney_bind()
	if bindMoney>=num then
		player:sub_gamemoney_bind(num)
	else
		player:sub_gamemoney_bind(bindMoney)
		player:sub_gamemoney(num-bindMoney)
	end
end

function quickQianghua(player,pos,nextlevel,itemid)
	local level = getQiangHuaLevel(player,pos)
	local id = getQiangHuaLevel_id(player,pos)
	if not id then
		player:alert(1, 1, "没有装备此道具")
		return
	end
	if level>=MAXLEVEL then
		player:alert(1, 1, "该装备已达最大强化等级！")
		return
	end
	if nextlevel<=level then
		player:alert(1, 1, "该装备强化等级已经大于"..nextlevel.."级！")
		return
	end

	--判断道具
	if player:num_item(itemid)<1 then 
		player:alert(1,1,"道具不足")
		return
	end
	player:remove_item(itemid,1)
	
	player:set_item_uplv(id,pos,nextlevel)
	player:alert(1, 1, "强化成功！")
end

ckpanel.add_listener("gui.PanelDuanZao.handlePanelData",gui.PanelDuanZao.handlePanelData)




