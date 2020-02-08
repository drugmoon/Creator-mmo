module(..., package.seeall)
--随身商店配置

local bagStore = tab.Store.storeType[8] or {}

-- local tableShop = {
-- 	[1]={id=20001004,name="金疮药(小)",	coin=1000,	bind = 1,	num = 6,sell = function() return server_start_day()>=1 and server_start_day()<=3 end},
-- 	[2]={id=20001005,name="金疮药(中)",	coin=2000,	bind = 1,	num = 6,sell = function() return server_start_day()>3 and server_start_day()<=7	end},
-- 	[3]={id=20001006,name="金疮药(大)",	coin=3000,	bind = 1,	num = 6,sell = function() return server_start_day()>7 end},
-- 	[4]={id=20001001,name="太阳水",		coin=1000,	bind = 1,	num = 6,sell = function() return true	end},
-- 	[5]={id=20001002,name="强效太阳水",	vcoin=100,	bind = 1,	num = 6,sell = function() return true	end},
-- 	[6]={id=20001003,name="万年雪霜",	vcoin=50,	bind = 0,	num = 6,sell = function() return true	end},
-- }
local totalBagNum = 75;
--背包格子对应开放时间表
local BagTable = {
	[26]={ needCd=2		,needVcoin=2	,	exp = 0},
	[27]={ needCd=6		,needVcoin=6	,	exp = 0},
	[28]={ needCd=12	,needVcoin=12	,	exp = 0},
	[29]={ needCd=20	,needVcoin=20	,	exp = 0},
	[30]={ needCd=40	,needVcoin=40	,	exp = 0},
	[31]={ needCd=54	,needVcoin=54	,	exp = 0},
	[32]={ needCd=70	,needVcoin=70	,	exp = 0},
	[33]={ needCd=88	,needVcoin=88	,	exp = 0},
	[34]={ needCd=108	,needVcoin=108	,	exp = 0},
    [35]={ needCd=130	,needVcoin=130	,	exp = 0},
	[36]={ needCd=154	,needVcoin=154	,	exp = 0},
	[37]={ needCd=180	,needVcoin=180	,	exp = 0},
	[38]={ needCd=208	,needVcoin=208	,	exp = 0},
	[39]={ needCd=238	,needVcoin=238	,	exp = 0},
	[40]={ needCd=270	,needVcoin=270	,	exp = 0},
	[41]={ needCd=304	,needVcoin=304	,	exp = 0},
	[42]={ needCd=340	,needVcoin=340	,	exp = 0},
	[43]={ needCd=378	,needVcoin=378	,	exp = 0},
	[44]={ needCd=418	,needVcoin=418	,	exp = 0},
	[45]={ needCd=460	,needVcoin=460	,	exp = 0},
	[46]={ needCd=504	,needVcoin=504	,	exp = 0},
	[47]={ needCd=550	,needVcoin=550	,	exp = 0},
	[48]={ needCd=598	,needVcoin=598	,	exp = 0},
	[49]={ needCd=648	,needVcoin=648	,	exp = 0},
	[50]={ needCd=700	,needVcoin=700	,	exp = 0},
	[51]={ needCd=754	,needVcoin=754	,	exp = 0},
	[52]={ needCd=810	,needVcoin=810	,	exp = 0},
	[53]={ needCd=868	,needVcoin=868	,	exp = 0},
	[54]={ needCd=928	,needVcoin=928	,	exp = 0},
	[55]={ needCd=990	,needVcoin=990	,	exp = 0},
	[56]={ needCd=1116	,needVcoin=1116,	exp = 0},
	[57]={ needCd=1184	,needVcoin=1184,	exp = 0},
	[58]={ needCd=1254	,needVcoin=1254,	exp = 0},
	[59]={ needCd=1326	,needVcoin=1326,	exp = 0},
	[60]={ needCd=1400	,needVcoin=1400,	exp = 0},
	[61]={ needCd=1476	,needVcoin=1476,	exp = 0},
	[62]={ needCd=1554	,needVcoin=1554,	exp = 0},
	[63]={ needCd=1634	,needVcoin=1634,	exp = 0},
	[64]={ needCd=1716	,needVcoin=1716,	exp = 0},
	[65]={ needCd=1800	,needVcoin=1800,	exp = 0},
	[66]={ needCd=1886	,needVcoin=1886,	exp = 0},
	[67]={ needCd=1974	,needVcoin=1974,	exp = 0},
	[68]={ needCd=2064	,needVcoin=2064,	exp = 0},
	[69]={ needCd=2156	,needVcoin=2156,	exp = 0},
	[70]={ needCd=2250	,needVcoin=2250,	exp = 0},
	[71]={ needCd=2346	,needVcoin=2346,	exp = 0},
	[72]={ needCd=2444	,needVcoin=2444,	exp = 0},
	[73]={ needCd=2544	,needVcoin=2544,	exp = 0},
	[74]={ needCd=2646	,needVcoin=2646,	exp = 0},
	[75]={ needCd=2750	,needVcoin=2750,	exp = 0},
}

function showConfirm(player)
	local result =  {
		panel = "confirm",
		visible = true,
		lblConfirm = "是否花费400元宝开启4个背包格子？",
		path = "gui.PanelBag.handlePanelData",
		actionid = "openBagGe",
		-- strMsg="wingly",
		param = {},
	}
	if result then player:push_lua_table("alert",util.encode(result)) end
end

function showConfirm2(player)
	local result =  {
		panel = "confirm",
		visible = true,
		lblConfirm = "是否花费500元宝开启5个仓库格子？",
		path = "gui.PanelBag.handlePanelData",
		actionid = "openDepotGe",
		-- strMsg="wingly",
		param = {},
	}
	if result then player:push_lua_table("alert",util.encode(result)) end
end

--背包开启操作
function startOpenGe(player,isbuy,num)
	if not num then num = 1 end
	local bagnum = player:num_bag()
	num = util.bound(0, num, totalBagNum - bagnum);
	if isbuy then
		-- local countMin = math.floor(util.ppn(player, const.PP_BAG_GE_COUNT)/60)
		local needVcoin = 0
		for i=bagnum+1,bagnum+num do
			needVcoin = needVcoin + BagTable[i].needVcoin
		end
		if player:get_vcoin()<needVcoin then
			-- player:alert(1, 1, "元宝不足"..needVcoin.."，无法开启!")
			showChongzhi(player)
			return
		end
		player:sub_vcoin(needVcoin,"仓库格子")
	end
	player:set_param(const.PP_BAG_GE_COUNT,0)
	player:add_bag_slot(num);
	player:alert(1, 1, "成功开启"..num.."个背包格子！")
	if player:num_bag()<totalBagNum then
		getBagNum(player,num)
	end
end
function getCurCellCountTime(player)
	local bagnum = player:num_bag()
	local time = 0
	if BagTable[bagnum+1] then
		time = BagTable[bagnum+1].needCd * 60
	end
	return time
end
--推送新开的格子数
function getBagNum(player,num)
	local bagnum = player:num_bag()
	if BagTable[bagnum+1] then
		local count_time = util.ppn(player, const.PP_BAG_GE_COUNT)
		local time = BagTable[bagnum+1].needCd * 60
		local timeTable = {}
		for i=bagnum+1,75 do
			table.insert(timeTable,BagTable[i])
		end
		local result = {
			cmd="curBagNum",
			bagnum = bagnum,
			lefttime = time - count_time,
			totalTime = time,
			needVcoin = BagTable[bagnum+1].needVcoin,
			timeTable = timeTable,
			opennum = num or 0
		}
		player:push_lua_table("PanelBag",util.encode(result))
	end
end

--主控里面的打开仓库条件判断
function openDepot(player)
	local vipLev = util.ppn(player, const.PP_VIP_LEVEL)
	local zsLev = util.ppn(player,const.PP_ZHSH_LEVEL)
	-- if vipLev>=2 or zsLev>=3 then
		player:push_lua_table("open", util.encode({name = "panel_depot"}))
	-- else
	-- 	player:alert(1, 1, "需要VIP2或者3转才可激活该功能!")
	-- end
end
function isItemSell(startTime,endTime)
	startTime =  util.checkint(startTime)
	endTime =  util.checkint(endTime)
	local serverData = server_start_day()
	if endTime == 0 or startTime<=serverData and serverData<=endTime then 
		return true
	end
end

function senderShopData(player)
	local result = {
		cmd="senderShopData",
		data=tab.Store.getItemsByType(player,const.storeType.bag),
	}
	player:push_lua_table("PanelBag",util.encode(result))
end

function buyShopItem(player,storeId)
	tab.Store.buyShopItem(player,storeId,"随身商店",1)
end

function useRandomStone(player)
	local num = player:num_item("随机传送石")
	if num>0 then
		player:use_item("随机传送石")
	else
		player:alert(1,1,"背包无随机传送石！")
	end
end

---------------------------------------------------------数据接受------------------------------------------------------
function handlePanelData(player,commond) 
	local data = util.decode(commond)
	if data.actionid == "openBagGe" then
		local num = data.param[1] or 1;
		-- startOpenGe(player,true,num)
	elseif data.actionid == "reqGeOpenNeedTime" then
		-- getBagNum(player)
	elseif data.actionid == "useRandomStone" then
		useRandomStone(player)
	----------------------------------------仓库开启-------------------------------------------
	elseif data.actionid == "reqOpenDepotGe" then
		showConfirm2(player)
	elseif data.actionid == "openDepotGe" then
		player:push_lua_table("PanelDepot",util.encode({cmd="openDepotGeZi"}))
	----------------------------------------打开仓库-------------------------------------------
	elseif data.actionid == "reqOpenDepot" then
		-- openDepot(player)
	----------------------------------------随身商店--------------------------------------------
	elseif data.actionid == "reqShopData" then
		senderShopData(player)
	elseif data.actionid == "shopBuy" then
		buyShopItem(player,data.params.storeId)
		
	elseif data.actionid == "flyHuiShou" then
		if string.find(player:get_map():get_id(),"ctg") then
			gui.PanelBreakUp.exitMapSure(player)
		end
		posnpc.fly(player,4002014)--"回收使者"
	elseif data.actionid == "flyHeCheng" then
		if string.find(player:get_map():get_id(),"ctg") then
			gui.PanelBreakUp.exitMapSure(player)
		end
		posnpc.fly(player,4002017)--"合成使者"
	elseif data.actionid == "flyAnDianNpc" then
		if string.find(player:get_map():get_id(),"ctg") then
			gui.PanelBreakUp.exitMapSure(player)
		end
		posnpc.fly(player,4002019)--"暗殿使者"
	end
end
ckpanel.add_listener("gui.PanelBag.handlePanelData",gui.PanelBag.handlePanelData)
