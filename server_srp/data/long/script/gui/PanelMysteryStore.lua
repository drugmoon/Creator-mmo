module(..., package.seeall)

local TIME=60*60*2

local showItems = {
	[1] ={id=23050003,index=1, num=5,money=103,moneyNum=10000,bind=1,con=const.PP_MYSTERY_ITEM1},
	[2] ={id=23020002,index=2, num=2,money=103,moneyNum=8000 ,bind=1,con=const.PP_MYSTERY_ITEM2},
	[3] ={id=23030002,index=3, num=2,money=103,moneyNum=8000 ,bind=1,con=const.PP_MYSTERY_ITEM3},
	[4] ={id=32000002,index=4, num=2,money=103,moneyNum=1600 ,bind=1,con=const.PP_MYSTERY_ITEM4},
	[5] ={id=23060003,index=5, num=4,money=103,moneyNum=1408 ,bind=1,con=const.PP_MYSTERY_ITEM5},
	[6] ={id=32000006,index=6, num=3,money=103,moneyNum=1188 ,bind=1,con=const.PP_MYSTERY_ITEM6},
	[7] ={id=23060002,index=7, num=3,money=103,moneyNum=792  ,bind=1,con=const.PP_MYSTERY_ITEM7},
	  
	[8] ={id=24030001,index=8, num=50,money=101,moneyNum=36000000,bind=1,con=const.PP_MYSTERY_ITEM8},
	[9] ={id=23040001,index=9, num=1 ,money=101,moneyNum=24000000,bind=1,con=const.PP_MYSTERY_ITEM9},
	[10]={id=32000006,index=10, num=5 ,money=101,moneyNum=11880000,bind=1,con=const.PP_MYSTERY_ITEM10},
	[11]={id=23080008,index=11, num=1 ,money=101,moneyNum=9600000 ,bind=1,con=const.PP_MYSTERY_ITEM11},
	[12]={id=32000001,index=12, num=5 ,money=101,moneyNum=7200000 ,bind=1,con=const.PP_MYSTERY_ITEM12},
	[13]={id=23080004,index=13, num=1 ,money=101,moneyNum=3200000 ,bind=1,con=const.PP_MYSTERY_ITEM13},
	 
	[14]={id=24060001,index=14, num=20 ,money=102,moneyNum=3200,bind=1,con=const.PP_MYSTERY_ITEM14},
	[15]={id=24030001,index=15, num=100,money=102,moneyNum=2400,bind=1,con=const.PP_MYSTERY_ITEM15},
	[16]={id=23020003,index=16, num=1  ,money=102,moneyNum=7910,bind=1,con=const.PP_MYSTERY_ITEM16},
	[17]={id=23030003,index=17, num=1  ,money=102,moneyNum=7910,bind=1,con=const.PP_MYSTERY_ITEM17},
	[18]={id=23000003,index=18, num=20 ,money=102,moneyNum=1408,bind=1,con=const.PP_MYSTERY_ITEM18},
	[19]={id=23040002,index=19, num=1  ,money=102,moneyNum=7910,bind=1,con=const.PP_MYSTERY_ITEM19},
	[20]={id=23140003,index=20, num=2  ,money=102,moneyNum=800, bind=1,con=const.PP_MYSTERY_ITEM20},

	[21]={id=24060001,index=21, num=20 ,money=102,moneyNum=3200,bind=1,con=const.PP_MYSTERY_ITEM21},
	[22]={id=24030001,index=22, num=100,money=102,moneyNum=2400,bind=1,con=const.PP_MYSTERY_ITEM22},
	[23]={id=23020003,index=23, num=1  ,money=102,moneyNum=7910,bind=1,con=const.PP_MYSTERY_ITEM23},
	[24]={id=23030003,index=24, num=1  ,money=102,moneyNum=7910,bind=1,con=const.PP_MYSTERY_ITEM24},
	[25]={id=23000003,index=25, num=20 ,money=102,moneyNum=1408,bind=1,con=const.PP_MYSTERY_ITEM25},
	[26]={id=23040002,index=26, num=1  ,money=102,moneyNum=7910,bind=1,con=const.PP_MYSTERY_ITEM26},
	[27]={id=23140003,index=27, num=2  ,money=102,moneyNum=800, bind=1,con=const.PP_MYSTERY_ITEM27},
	-- [28]={id=13102051,index=1, num=1,money=102,moneyNum=100,bind=1,con=const.PP_MYSTERY_ITEM28},
	-- [29]={id=13102061,index=1, num=2,money=102,moneyNum=100,bind=1,con=const.PP_MYSTERY_ITEM29},
	-- [30]={id=13102071,index=1, num=1,money=102,moneyNum=100,bind=1,con=const.PP_MYSTERY_ITEM30},
	-- [31]={id=13102081,index=1, num=1,money=102,moneyNum=100,bind=1,con=const.PP_MYSTERY_ITEM31},
	-- [32]={id=12102071,index=1, num=1,money=102,moneyNum=100,bind=1,con=const.PP_MYSTERY_ITEM32},

}

function handlePanelData(player,commond)
	local data = util.decode(commond);
	if data.actionid == "buy_mystery" then---
		BuyStoreItem(player,data.param)
	elseif data.actionid == "getPanelData" then---打开面板
		if os.time()+1-util.ppn(player,const.PP_MYSTERYSTORE_TIME) >= TIME then -------超过了刷新时间
			OpenrefreshStore(player)
		else
			SendStoreInfo(player)
		end
		
	elseif data.actionid == "buy_fresh" then---购买刷新
		BuyrefreshStore(player)
	elseif data.actionid == "time_fresh" then---购买刷新
		TimerefreshStore(player)
		
	end
end



----发送面板数据
local storeRand = {
	[1] = {con = const.PP_MYSTERYSTORE_RAND1},
	[2] = {con = const.PP_MYSTERYSTORE_RAND2},
	[3] = {con = const.PP_MYSTERYSTORE_RAND3},
	[4] = {con = const.PP_MYSTERYSTORE_RAND4},
}

-------发送数据
function SendStoreInfo(player)

	local result={}
	result.cmd="updateupdateSmsd"
	result.iteminfo={}
	--result.time=util.ppn(player,const.PP_MYSTERYSTORE_TIME)
	--if os.time()+1-util.ppn(player,const.PP_MYSTERYSTORE_TIME) >= TIME then -------超过了刷新时间
	--	local timer=os.time()+1-util.ppn(player,const.PP_MYSTERYSTORE_TIME)
	--	result.time=os.time()+(TIME-(timer%TIME))
		--print( ">>>>>>>>>>",timer)
	--else 
	result.time=(TIME+util.ppn(player,const.PP_MYSTERYSTORE_TIME))
	--end
	if not gui.PanelHeQuAct.mergeTabOpenState("神秘商店") then
		result.time=0
	end

	local H = tonumber(os.date("%H",os.time()))
	local M = tonumber(os.date("%M",os.time()))
	local S = tonumber(os.date("%S",os.time()))
	result.timei=(3-server_merge_day()-1)*24*3600+(24-H)*3600-M*60-S

	for i=1,4 do
		result.iteminfo[i] = {}
		--table.insert(result.iteminfo, {id = showItems[util.ppn(player, storeRand[i].con)].id})
		--print(">>>>",util.ppn(player, storeRand[i].con),i)
		result.iteminfo[i].id=showItems[util.ppn(player, storeRand[i].con)].id
		result.iteminfo[i].index=showItems[util.ppn(player, storeRand[i].con)].index
		result.iteminfo[i].num=showItems[util.ppn(player, storeRand[i].con)].num
		result.iteminfo[i].money=showItems[util.ppn(player, storeRand[i].con)].money
		result.iteminfo[i].moneyNum=showItems[util.ppn(player, storeRand[i].con)].moneyNum
		result.iteminfo[i].con=util.ppn(player, showItems[util.ppn(player, storeRand[i].con)].con)
	end
	--print(result.iteminfo[1].index,result.iteminfo[2].index,result.iteminfo[3].index,result.iteminfo[4].index)
	--print(result.iteminfo[1].con,result.iteminfo[2].con,result.iteminfo[3].con,result.iteminfo[4].con)
	--print(result.iteminfo[1].id,result.iteminfo[2].id,result.iteminfo[3].id,result.iteminfo[4].id)
	player:push_lua_table("PanelHeQuAct",util.encode(result))
end

---购买刷新
function BuyrefreshStore(player)
	if not gui.PanelHeQuAct.mergeTabOpenState("神秘商店") then
		return util.alert_msg(4, "活动已结束!", player)
	end
	if player:get_vcoin()>=399  then
		player:sub_vcoin(399,"神秘商店")
		refreshStore(player)
		util.alert_msg(4, "刷新成功!", player)
	else
		-- util.alert_msg(4, "元宝不足!", player)
		showChongzhi(player)
		return 0
	end	

	
end
--时间到了刷
function TimerefreshStore(player)
	if os.time()+1-util.ppn(player,const.PP_MYSTERYSTORE_TIME) >= (TIME-20) then ---20秒防止客户端调刷新这边时间还没到
		refreshStore(player)
	end
end

--购买物品
function BuyStoreItem(player,index)
	-- 100coin
	-- 101coin_bind
	-- 102vcoin
	-- 103vcoin_bind
	print(index,showItems[index].id)
	if not gui.PanelHeQuAct.mergeTabOpenState("神秘商店") then
		return util.alert_msg(4, "活动已结束!", player)
	end
	if util.ppn(player,showItems[index].con) ==0 then 
		if player:num_bag_black()>=3 then

			if showItems[index].money==100 then
				if player:get_gamemoney()>=showItems[index].moneyNum  then
					player:sub_gamemoney(showItems[index].moneyNum,"神秘商店")
				else
					util.alert_msg(4, "金币不足!", player)
					return 
				end	
			elseif showItems[index].money==101 then 
				if player:get_gamemoney_bind()>=showItems[index].moneyNum  then
					player:sub_gamemoney_bind(showItems[index].moneyNum,"神秘商店")
				else
					util.alert_msg(4, "绑定金币不足!", player)
					return 
				end	

			elseif showItems[index].money==102 then 
				if player:get_vcoin()>=showItems[index].moneyNum  then
					player:sub_vcoin(showItems[index].moneyNum,"神秘商店")
				else
					-- util.alert_msg(4, "元宝不足!", player)
					showChongzhi(player)
					return 
				end	
			elseif showItems[index].money==103 then 
				if player:get_vcoin_bind()>=showItems[index].moneyNum  then
					player:sub_vcoin_bind(showItems[index].moneyNum,"神秘商店")
				else
					util.alert_msg(4, "绑定元宝不足!", player)
					return 
				end	
			end
			player:add_item(showItems[index].id,showItems[index].num,1)
			player:set_param(showItems[index].con,1)
			SendStoreInfo(player)
		else
			return util.alert_msg(4, "背包格子不足！", player)
		end
	else
		util.alert_msg(4, "本时间段已经购买", player)
	end 
end




--0点所有玩家触发一次，不在线的玩家，上线的时候在触发一次

--点击刷新(重设数据)
function refreshStore(player)
	--player:set_timer(22,60*1000);---1hour
	if not gui.PanelHeQuAct.mergeTabOpenState("神秘商店") then
		return 
	end
	player:set_param(const.PP_MYSTERYSTORE_TIME,os.time())--记录当前时间
	local rand1=math.random(1,7)
	player:set_param(const.PP_MYSTERYSTORE_RAND1,rand1)

	local rand2=math.random(8,13)
	player:set_param(const.PP_MYSTERYSTORE_RAND2,rand2)

	local rand3=math.random(14,20)
	player:set_param(const.PP_MYSTERYSTORE_RAND3,rand3)

	local rand4=math.random(21,27)
	player:set_param(const.PP_MYSTERYSTORE_RAND4,rand4)
	for i=1,#showItems do
		player:set_param(showItems[i].con,0)
	end
	SendStoreInfo(player)
end


--打开刷新(重设数据)
function OpenrefreshStore(player)
	--player:set_timer(22,60*1000);---1hour
	--player:set_param(const.PP_MYSTERYSTORE_TIME,os.time())--记录当前时间
	---记录上次刷新的时间
	-- if not gui.PanelHeQuAct.mergeTabOpenState("神秘商店") then
	-- 	return 
	-- end
	local timer=os.time()+1-util.ppn(player,const.PP_MYSTERYSTORE_TIME)
	local lasttime=util.ppn(player,const.PP_MYSTERYSTORE_TIME)+(TIME*math.floor(timer/TIME))
	player:set_param(const.PP_MYSTERYSTORE_TIME,lasttime)

	local rand1=math.random(1,7)
	player:set_param(const.PP_MYSTERYSTORE_RAND1,rand1)

	local rand2=math.random(8,13)
	player:set_param(const.PP_MYSTERYSTORE_RAND2,rand2)

	local rand3=math.random(14,20)
	player:set_param(const.PP_MYSTERYSTORE_RAND3,rand3)

	local rand4=math.random(21,27)
	player:set_param(const.PP_MYSTERYSTORE_RAND4,rand4)
	for i=1,#showItems do
		player:set_param(showItems[i].con,0)
	end
	SendStoreInfo(player)
end

ckpanel.add_listener("gui.PanelMysteryStore.handlePanelData",gui.PanelMysteryStore.handlePanelData)

---------------









	