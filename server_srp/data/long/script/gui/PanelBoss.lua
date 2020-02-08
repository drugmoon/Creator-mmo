module(..., package.seeall)

--个人BOSS配置表
local bossPersonal = tab.Boss.bossPersonal;

tab.Boss.setDropInfo()--设置掉落信息

function onPanelData(player,commond)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "btnKill" then
		getKillBoss(player)
	elseif data.actionid == "btnYeWai" then
		senderWorldBoss(player)
	elseif data.actionid == "reqYeWaiBossOne" then
		senderWorldBossDesp(player,data.params.index)
	elseif data.actionid == "btnGeRen" then--个人boss名字列表
		senderPersonalBoss(player)
	elseif data.actionid == "reqGeRenBossOne" then--个人boss指定条目详细信息
		senderPersonalBossDesp(player,data.params.index)
	elseif data.actionid=="reqEnterBossFuBen" then
		enterPersonMap(player,data.params.index)
	elseif data.actionid=="single" then
		getPersonalBossAwards(player,1)
	elseif data.actionid=="double" then
		getPersonalBossAwards(player,2)
	elseif data.actionid=="back" then
		player:go_home()
	---------------------------------------
	elseif data.actionid=="enterZhiJia" then
		posnpc.fly(player,4002012)
		player:push_lua_table("close",util.encode({name = "btn_main_boss"}))
	elseif data.actionid=="enterMaYa" then
		posnpc.fly(player,4002024)
		player:push_lua_table("close",util.encode({name = "btn_main_boss"}))
	elseif data.actionid=="enterYeWai" then
		enterYeWai(player,data.params.index)

	---------------------------------------BOSS血条上的掉落显示
	elseif data.actionid=="reqDropItems" then
		getBossDrops(player,data.params.monid)

	elseif data.actionid=="showLeftTip" then--上线在个人boss副本里刷新左侧提示
		local curMapId = player:get_map():get_id()
		if string.find(curMapId,"fbgr") then
			gui.PanelBoss.updateFuBenInfo(player,nil)
		end
		if string.sub(curMapId,2,5) == "clfb" then
			local index=tonumber(string.sub(curMapId,1,1))
			npc.party.cailiao.show_leftTips(player,index)
		end
	end 
end

--筛选出可击杀的野外boss
function getKillBoss(player)
	local result = {
		cmd="updateKeKill",
		monTable={},
	}
	local bossTable = tab.Boss.WildBoss
	for i=1,#bossTable do
		local itemData = bossTable[i]
		local flag = 0
		if itemData.map1 and itemData.map1[1] then
			local map = server.find_map(itemData.map1[1])
			if map and map:num_monster(itemData.monName)~=0 then
				itemData.map1[2]=1
				flag = 1
			else
				itemData.map1[2]=0
			end
		end
		if itemData.map2 and itemData.map2[1] then
			local map = server.find_map(itemData.map2[1])
			if map and map:num_monster(itemData.monName)~=0 then
				itemData.map2[2]=1
				flag = 1
			else
				itemData.map2[2]=0
			end
			-- if i==23 then
			-- 	print(map:num_monster(itemData.monName),itemData.map2[2])
			-- end
		end
		if itemData.map3 and itemData.map3[1] then
			local map = server.find_map(itemData.map3[1])
			if map and map:num_monster(itemData.monName)~=0 then
				itemData.map3[2]=1
				flag = 1
			else
				itemData.map3[2]=0
			end

		end
		if flag==1 then
			table.insert(result.monTable,{monName=itemData.monName,index=itemData.index})
		end
	end
	player:push_lua_table("PanelBoss",util.encode(result))
end

--推送所有野外BOSS Name信息
function senderWorldBoss(player)
	local bossTable = util.clone(tab.Boss.WildBoss)
	local result = {
		cmd="updateWorldBoss",
		monTable={},
	}
	for i=1,#bossTable do
		result.monTable[i]={}
		result.monTable[i].monName=bossTable[i].monName
		result.monTable[i].index=bossTable[i].index
	end
	player:push_lua_table("PanelBoss",util.encode(result))
end

--推送野外BOSS指定条目详情
function senderWorldBossDesp(player,index)
	local result = {
		cmd="senderWorldBossDesp",
		monTable={},
	}
	local tempTable = util.clone(tab.Boss.WildBoss)
	if tempTable[index] then
		result.monTable=tempTable[index]
	end
	player:push_lua_table("PanelBoss",util.encode(result))
end

--推送所有个人BOSS的Name
function senderPersonalBoss(player)
	local result = {
		cmd="updatePersonalBossName",
		monTable={},
	}
	local tempTable = util.clone(bossPersonal)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		result.monTable[i]={}
		result.monTable[i].monName=itemData.monName.."("..(util.ppn(player,itemData.con)<=1 and util.ppn(player,itemData.con) or 1).."/"..itemData.times..")"
		result.monTable[i].index=itemData.index
		result.monTable[i].allTimes=itemData.times
		result.monTable[i].useTimes=util.ppn(player,itemData.con)
	end
	player:push_lua_table("PanelBoss",util.encode(result))
end

--推送个人BOSS指定条目详情
function senderPersonalBossDesp(player,index)
	local result = {
		cmd="senderPersonalBossDesp",
		monTable={},
	}
	local tempTable = util.clone(bossPersonal)
	if tempTable[index] then
		result.monTable=tempTable[index]
		result.needDesp=enterConditon(player,tempTable[index])
	end
	player:push_lua_table("PanelBoss",util.encode(result))
end

--个人副本进入条件
function enterConditon(player,curTable)
	local result = {}
	local level = player:get_level()
	local zLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	local condition = curTable.condition
	if condition.level>0 then
		if level>=condition.level then
			table.insert(result,"最低挑战等级:"..condition.level.."级")
		else
			table.insert(result,"最低挑战等级:<font color=#ff0000>"..condition.level.."级</font>")
		end
	end
	if condition.zlevel>0 then
		if zLevel>=condition.zlevel then
			table.insert(result,"最低挑战等级:"..condition.zlevel.."转")
		else
			table.insert(result,"最低挑战等级:<font color=#ff0000>"..condition.zlevel.."转</font>")
		end
	end
	if condition.mateId>0 then
		local curNum = player:num_item(condition.mateId)
		if curNum>=condition.mateNum then
			table.insert(result,"需要副本卷轴:"..condition.mateNum.."(拥有"..curNum.."个)")
		else
			table.insert(result,"需要副本卷轴:"..condition.mateNum.."(拥有<font color=#ff0000>"..curNum.."</font>个)")
		end
	end
	return result
end

--野外地图进入逻辑
function enterYeWai(player,index)
	local bossTable = util.clone(tab.Boss.WildBoss)
	local bossData = bossTable[index]
	-- print(util.encode(bossData))
	-- if bossData then
	-- 	local map = server.find_map(bossData.map1[1])
	-- 	if map then
	-- 		local x,y = map:gen_rand_position()
	-- 		player:enter_map(map:get_id(),x,y)
	-- 		player:push_lua_table("close",util.encode({name = "btn_main_boss"}))
	-- 	end
	-- end
	if bossData and bossData.map1[4] then
		posmap.fly(player, bossData.map1[4])
		player:push_lua_table("close",util.encode({name = "btn_main_boss"}))
	end
end

--野外boss传送
function flyToBoss(player,bossid)
	local index = tonumber(bossid)-3001000--尸王3001001
	local bossTable = util.clone(tab.Boss.WildBoss)
	local bossData = bossTable[index]
	if bossData and bossData.map1[4] then
		posmap.fly(player, bossData.map1[4])
		-- player:push_lua_table("close",util.encode({name = "btn_main_boss"}))
	end
end

--进个人副本条件判断
function enterGeRenFuBen(player,itemData)
	local level = player:get_level()
	local zLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	local condition = itemData.condition
	if condition.level>0 then
		if level<condition.level then
			player:alert(1,1,"未达最低挑战等级！")
			return false
		end
	end
	if condition.zlevel>0 then
		if zLevel<condition.zlevel then
			player:alert(1,1,"未达最低挑战等级！")
			return false
		end
	end
	if condition.mateId>0 then
		local curNum = player:num_item(condition.mateId)
		if curNum<condition.mateNum then
			player:alert(1,1,"副本卷轴不足！")
			return false
		else
			player:remove_item(condition.mateId,condition.mateNum)
		end
	end
	return true
end

--判断是不是主线个人副本
function isTaskFuBen(player)
	local tid, ts = task.util.get_task_state(player, const.TASK_ID_MAIN);
	if tid == 10080 or tid == 10083 then
		return true
	end
end

----------------------------------------个人BOSS地图进入于刷怪-----------------------------------------------
function enterPersonMap(player,id)
	local curMapId = player:get_map():get_id()
	if not tab.isMapEnterLegal(curMapId) then
		return util.alert_msg(4, "您当前已在副本中，无法传送！", player);
	end
	-- if string.find(curMapId,"fbgr") then
	-- 	player:alert(1,1,"当前在副本中无法再进入副本！")
	-- 	return
	-- end
	local data = bossPersonal[id];
	if data then
		if not isTaskFuBen(player) then
			if util.ppn(player,data.con)>=data.times then
				player:alert(1,1,"该副本今日次数已用完！")
				return
			end
		end
		if not enterGeRenFuBen(player,data) then return end

		local map = server.find_map(data.namePre)
		if map then
			map:clear_mon()
			map:clear_item()
			player:set_param(const.PP_GEREN_FUBEN_COUNT,0)
		end
		if posmap.fly(player,data.mapId,"autofightstart") then
			local map = player:get_map()

			if isTaskFuBen(player) and data.taskMonId then
				map:mon_gen(data.x,data.y,data.taskMonId,1,1,1,1,"show_name")
			else
				map:mon_gen(data.x,data.y,data.monId,1,1,1,1,"show_name")
			end
			for i=1,#data.minMon do
				local monData = data.minMon[i]
				map:mon_gen(monData.x,monData.y,monData.id,monData.num,monData.range,1,1,"show_name")
			end
			-- map:set_timer(1,30*60*1000);--------设置本地图中出去的时间
			-- player:find_road_goto(map:get_id(),data.x,data.y,"autofightstart")
			player:notify_mini_npc()
			player:push_lua_table("close",util.encode({name = "btn_main_boss"}))
			player:set_param(const.PP_BOSS_INDEX,id)--记录下当前进入副本在表中的index
			updateFuBenInfo(player,map)
			player:set_param(data.con,util.ppn(player,data.con)+1);
		else
			player:alert(1, 1, "副本地图爆满,请稍后再试!")
		end
	end
end

--进地图推送左侧副本慢板信息
function updateFuBenInfo(player,map)
	if not map then map=player:get_map() end

	local index = util.ppn(player,const.PP_BOSS_INDEX)
	local yuTime = 30*60-util.ppn(player,const.PP_GEREN_FUBEN_COUNT)+os.time()
	
	local curData = bossPersonal[index]
	if not curData then return end
	local result = {}
	result.killboss=1 - map:num_monster(curData.monName)
	result.boss=1
	if result.killboss==1 then
		result.killmon=3-map:num_monster()
	else
		result.killmon=3-map:num_monster()+1
	end
	result.mon=3
	-- result.time=curData.limitTime*60*1000
	result.time=yuTime
	result.award=curData.awards
	result.vcoin=100
	result.title=curData.monName
	result.isTask=isTaskFuBen(player) or false
	result.func="gui.PanelBoss.onPanelData"
	player:push_lua_table("showGrBoss",util.encode(result))
end

function onKillMon(player,map)
	updateFuBenInfo(player,map)
end

--完成个人BOSS副本领取奖励times倍数
function getPersonalBossAwards(player,times)
	local index = util.ppn(player,const.PP_BOSS_INDEX)
	local curData = bossPersonal[index]
	if not curData then return end
	if player:num_bag_black()>=0 then
		if times>1 then
			if player:get_vcoin()>=100 then
				player:sub_vcoin(100,"双倍领取")
			else
				-- player:alert(1, 1, "元宝不足双倍领取，自动转换为单倍！")
				-- times=1
				showChongzhi(player)
				return
			end
		end
		for i=1,#curData.awards do
			player:add_item(curData.awards[i].name,times*curData.awards[i].num,1)
			player:alert(1, 1, "获得"..curData.awards[i].name.."*"..times*curData.awards[i].num)
		end
		player:go_home()
	else
		player:alert(1, 1, "背包空间不足！")
		return
	end
end


-----------------------------------------------------处理BOSS血条上的掉落显示----------------------------------------------------
function getBossDrops(player,monid)
	local map = player:get_map()
	local mon = map:get_monster(monid)
	if mon then
		monid = mon:get_def_id() or 0
	end
	local dropId = 0
	if tab.mondefCsv[monid] then
		dropId=tab.mondefCsv[monid]["simpleDropID"]
	end
	local dropData = {}
	dropData = tab.simpleDrop[tostring(dropId)] or {}
	-- print(util.encode(dropData),dropId,monid,"---------")
	player:push_lua_table("showBossDropItems",util.encode(dropData))
end



ckpanel.add_listener("gui.PanelBoss.onPanelData",gui.PanelBoss.onPanelData);










