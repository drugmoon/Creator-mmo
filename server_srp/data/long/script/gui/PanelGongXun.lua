module(... ,package.seeall)

local gxTable = {
	[1]={gxNum=30, con=const.PP_GONGXUN_GETBOX1,
			awards={
				{typeId = 27000001,	num = 10,bind = 1},
			}
		},
	[2]={gxNum=60, con=const.PP_GONGXUN_GETBOX2,
			awards={
				{typeId = 27000001,	num = 10,bind = 1},
			}
	},
	[3]={gxNum=90, con=const.PP_GONGXUN_GETBOX3,
			awards={
				{typeId = 27000001,	num = 10,bind = 1},
				{typeId = 23040003,	num = 2, bind = 1},
			}
	},
	[4]={gxNum=120,con=const.PP_GONGXUN_GETBOX4,
			awards={
				{typeId = 27000001,	num = 10,bind = 1},
				{typeId = 23040003,	num = 3, bind = 1},
			}
	},
	-- [5]={gxNum=150,con=const.PP_GONGXUN_GETBOX5,
	-- 		awards={
	-- 			{typeId = 23040003,	num = 5, bind = 1},
	-- 			{typeId = 27000001,	num = 10,bind = 1},
	-- 		}
	-- },
}

--概率必须从大到小排序
local awardsTable = {
	[1]={pro=0.4, num=1, name="金疮药(小)", id=20001004, bind=1},
	[2]={pro=0.3, num=1, name="金疮药(中)", id=20001005, bind=1},
	[3]={pro=0.3, num=1, name="金疮药(大)", id=20001006, bind=1},
}

function handlePanelData(player,commond)
	-- player:set_param(const.PP_DAY_GONGXUN_NUM,util.ppn(player,const.PP_DAY_GONGXUN_NUM)+10)--测试功勋值
	-- addGongXunScore(player,10)
		-- player:set_param(const.PP_DAY_GONGXUN_NUM,0)--清理功勋值
		-- player:set_param(const.PP_GONGXUN_RANK,0)--清理功勋排名
		-- for i=0,4 do
		-- 	player:set_param(const.PP_GONGXUN_GETBOX1+i,0)
		-- end
		-- player:set_param(const.PP_GONGXUN_BOXNUM,util.ppn(player,const.PP_GONGXUN_BOXNUM)+100)
	local data = util.decode(commond)
	if data.actionid == "reqListData" then
		-- senderGongXunData(player)
		senderTaskData(player)
	elseif data.actionid == "buyGongXun" then 
		buyGongXunItem(player,data.params.index)
	-- elseif data.actionid == "reqGongXunTask" then 
		-- senderTaskData(player)
	elseif data.actionid == "reqTaskChuan" then
		taskChuan(player,data.params.index)
	elseif data.actionid == "reqGongXunData" then
		if util.ppn(player, const.PP_REDDOT_EXPLOIT_PANEL) == 0 then
			player:set_param(const.PP_REDDOT_EXPLOIT_PANEL, 1);
		end
		sendToClient(player)
		gui.moduleRedPoint.checkExploitUsable(player)
	elseif data.actionid == "addGongXun" then
		-- changeGongXun(player,90)
		-- player:set_param(const.PP_DAY_GONGXUN_NUM,0)--清理功勋值
		-- player:set_param(const.PP_GONGXUN_RANK,0)--清理功勋排名
		-- for i=0,4 do
		-- 	player:set_param(const.PP_GONGXUN_GETBOX1+i,0)
		-- end
		-- player:set_param(const.PP_GONGXUN_BOXNUM,util.ppn(player,const.PP_GONGXUN_BOXNUM)+100)
	elseif data.actionid == "repOpenBox" then
		openBoxAward(player)
	elseif data.actionid == "repLingBox" then
		getBoxAward(player,data.params.index)

	elseif data.actionid == "clearDataSuccess" then
		player:set_param(const.PP_DAY_CLEAR_BOX,1)
	end
end

function sendToClient(player)
	local result = {
		cmd="updateGongXun",
		curGx = util.ppn(player,const.PP_DAY_GONGXUN_NUM),
		curBoxNum = util.ppn(player,const.PP_GONGXUN_BOXNUM),
		-- curBoxNum=player:num_item(27000001),--功勋宝箱
		curRank = util.ppn(player,const.PP_GONGXUN_RANK),
		needClear = util.ppn(player,const.PP_DAY_CLEAR_BOX),
		stateTable={},
	}
	for i=1,#gxTable do
		table.insert(result.stateTable,util.ppn(player,gxTable[i].con))
	end
	player:push_lua_table("PanelGongXun",util.encode(result))
end

--领取功勋宝箱
function getBoxAward(player,index)
	local curGx = util.ppn(player,const.PP_DAY_GONGXUN_NUM)
	local itemData = gxTable[index]
	if not itemData then 
		player:alert(1,0,"数据异常！")
		return true
	end
	if util.ppn(player,itemData.con)>=1 then
		player:alert(1,0,"已领取，不能重复领取！")
		return true
	end
	if curGx>=itemData.gxNum then
		-- local curBoxNum = player:num_item(27000001)--功勋宝箱
		local curBoxNum = util.ppn(player,const.PP_GONGXUN_BOXNUM)
		if (curBoxNum+itemData.awards[1].num)>100 then
			player:alert(1,0,"宝箱最大累计数量100个！")
			return true
		end
		player:set_param(itemData.con,1)
		for i=1,#itemData.awards do
			local awardItem = itemData.awards[i]
			if awardItem.typeId==27000001 then
				player:set_param(const.PP_GONGXUN_BOXNUM,curBoxNum+awardItem.num)
			else
				player:add_item(awardItem.typeId,awardItem.num,awardItem.bind)
			end
		end
		sendToClient(player)
		gui.moduleRedPoint.checkExploitUsable(player)
	end

end

--功勋值变化刷新--手动领取
-- function changeGongXun(player)
-- 	local curGx = util.ppn(player,const.PP_DAY_GONGXUN_NUM)
-- 	for i=1,#gxTable do
-- 		local itemData = gxTable[i]
-- 		if util.ppn(player,itemData.con)<=0 and curGx>=itemData.gxNum then
-- 			player:set_param(itemData.con,1)
-- 			local curBoxNum = player:num_item(27000001)--功勋宝箱
-- 			for i=1,#itemData.awards do
-- 				local awardItem = itemData.awards[i]
-- 				if awardItem.typeId==27000001 then
-- 					if (curBoxNum+awardItem.num)<=100 then
-- 						player:add_item(awardItem.typeId,awardItem.num,awardItem.bind)
-- 					else
-- 						if curBoxNum<100 then
-- 							player:add_item(awardItem.typeId,100-curBoxNum,awardItem.bind)
-- 						end
-- 					end
-- 				else
-- 					player:add_item(awardItem.typeId,awardItem.num,awardItem.bind)
-- 				end
-- 			end
-- 			gui.moduleRedPoint.checkExploitUsable(player)
-- 		end
-- 	end
-- 	sendToClient(player)
-- end

function addGongXunScore(player,exp)
	if type(exp) == "number" then
		player:set_param(const.PP_DAY_GONGXUN_NUM,util.ppn(player,const.PP_DAY_GONGXUN_NUM)+exp);
		-- changeGongXun(player)
		player:alert(1,0,"增加功勋值"..exp)
		gui.moduleRedPoint.checkExploitUsable(player)
		--功勋任务
		task.task1000.check_get_exploit(player, exp);
		task.task1100.check_get_exploit(player, exp);
		return true
	end
end
--点击开功勋宝箱获得物品
function openBoxAward(player)
	local curBoxNum = util.ppn(player,const.PP_GONGXUN_BOXNUM)
	-- local curBoxNum=player:num_item(27000001)--功勋宝箱
	if curBoxNum<=0 then
		player:alert(1,1,"您当前没有宝箱！")
		return
	end
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	player:set_param(const.PP_GONGXUN_BOXNUM,curBoxNum-1)
	item.chufa.gxbaoxiang(player)
	gui.moduleRedPoint.checkExploitUsable(player)
	-- player:use_item(27000001)
	-- local random = math.random(1,100)
	-- local count = 0
	-- for i=1,#awardsTable do
	-- 	count=count+awardsTable[i].pro*100
	-- 	if random>(100-count) then
	-- 		player:add_item(awardsTable[i].name,awardsTable[i].num,awardsTable[i].bind)
	-- 		player:alert(1,1,"通过开宝箱获得:"..awardsTable[i].name)
	-- 		player:push_lua_table("PanelGongXun",util.encode({
	-- 			cmd="updateGetRecord",
	-- 			record=awardsTable[i].name.."*"..awardsTable[i].num,
	-- 			boxNum=curBoxNum-1,
	-- 		}))
	-- 		player:set_param(const.PP_GONGXUN_BOXNUM,curBoxNum-1)
	-- 		gui.moduleRedPoint.checkExploitUsable(player)
	-- 		break
	-- 	end
	-- end
end

function openBoxRecord(player,desp)
	player:push_lua_table("PanelGongXun",util.encode({
		cmd="updateGetRecord",
		record="恭喜获得："..desp.."*"..1,
		boxNum=util.ppn(player,const.PP_GONGXUN_BOXNUM),
	}))
end


-----------------------------------------------------功勋快捷购买-----------------------------------------------
local goodsTable={
	[1]={id=20001006,name="金疮药(大)",vcion=100},
	[2]={id=20001006,name="金疮药(大)",vcion=100},
	[3]={id=20001006,name="金疮药(大)",vcion=100},
	[4]={id=20001006,name="金疮药(大)",vcion=100},
	[5]={id=20001006,name="金疮药(大)",vcion=100},
}

function senderGongXunData(player)
	local result = {
		cmd="senderGongXunData",
		data=util.clone(goodsTable),
	}
	player:push_lua_table("PanelGongXun",util.encode(result))
end

function buyGongXunItem(player,index)
	if not goodsTable[index] then return end
	local itemData = goodsTable[index]
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	if player:get_vcoin()>=itemData.vcion then
		player:sub_vcoin(itemData.vcion,"功勋道具")
		player:add_item(itemData.name,1,1)
		player:use_item(itemData.name)
		player:alert(1, 1, "成功购买"..itemData.name.."并使用")
	else
		-- player:alert(1,1,"元宝不足");
		showChongzhi(player)
	end
end

-----------------------------------------------------功勋任务-----------------------------------------------
local taskTable={
	{id=1 , taskAward=5 , needNum=1   , panelName=nil,              npcId=nil,     con=const.PP_GONGXUN_TASK1 , con2=const.PP_GX_TSAK_RECORD1 , taskName="登录游戏",needlv = 1, taskDesp="登录游戏",           },--已添加
	{id=2 , taskAward=15, needNum=5000, panelName=nil,              npcId=4002013, con=const.PP_GONGXUN_TASK2 , con2=const.PP_GX_TSAK_RECORD2 , taskName="击杀怪物",needlv = 1, taskDesp="击杀(%s/5000)只任意怪物", },--已添加
	{id=3 , taskAward=10, needNum=5   , panelName=nil,              npcId=4002011, con=const.PP_GONGXUN_TASK3 , con2=const.PP_GX_TSAK_RECORD3 , taskName="材料副本",needlv = 1, taskDesp="进入材料副本(%s/5)次",    },--已添加
	{id=4 , taskAward=5 , needNum=1   , panelName="main_guild",     npcId=nil,     con=const.PP_GONGXUN_TASK4 , con2=const.PP_GX_TSAK_RECORD4 , taskName="帮会捐献",needlv = 1, taskDesp="帮会捐献(%s/1)次金币",    },--已添加
	{id=5 , taskAward=15, needNum=10  , panelName="btn_main_boss",  npcId=nil,     con=const.PP_GONGXUN_TASK5 , con2=const.PP_GX_TSAK_RECORD5 , taskName="击杀BOSS",needlv = 1, taskDesp="击杀(%s/10)只任意BOSS",   },
	{id=6 , taskAward=5 , needNum=1   , panelName="extend_lottory", npcId=nil,     con=const.PP_GONGXUN_TASK6 , con2=const.PP_GX_TSAK_RECORD6 , taskName="每日挖宝",needlv = 1, taskDesp="每日挖宝(%s/1)次",        },--已添加
	{id=7 , taskAward=10, needNum=3   , panelName=nil,              npcId=4002021, con=const.PP_GONGXUN_TASK7 , con2=const.PP_GX_TSAK_RECORD7 , taskName="烧猪洞"  ,needlv = 1, taskDesp="完成烧猪洞(%s/3)次",      },--已添加
	{id=8 , taskAward=5 , needNum=1   , panelName="extend_store",   npcId=nil,     con=const.PP_GONGXUN_TASK8 , con2=const.PP_GX_TSAK_RECORD8 , taskName="商城购买",needlv = 1, taskDesp="商城购买任意物品(%s/1)次",  },--已添加
	{id=9,  taskAward=15, needNum=3   , panelName="extend_breakup", npcId=nil,     con=const.PP_GONGXUN_TASK9 , con2=const.PP_GX_TSAK_RECORD9 , taskName="闯天关"  ,needlv = 80,lvdesp = "闯天关将在80级开启！", taskDesp="参与(%s/3)次闯天关",      },--已添加
	{id=10, taskAward=10, needNum=3   , panelName=nil,              npcId=4002005, con=const.PP_GONGXUN_TASK10, con2=const.PP_GX_TSAK_RECORD10, taskName="挖矿任务",needlv = 1, taskDesp="完成挖矿任务(%s/3)次",    },--已添加
	{id=11, taskAward=5 , needNum=1   , panelName=nil,              npcId=4005009, con=const.PP_GONGXUN_TASK11, con2=const.PP_GX_TSAK_RECORD11, taskName="每日押镖",needlv = 1, taskDesp="押镖(%s/1)次",            },--已添加
	{id=12, taskAward=5 , needNum=1   , panelName="main_forge",     npcId=nil,     con=const.PP_GONGXUN_TASK12, con2=const.PP_GX_TSAK_RECORD12, taskName="强化装备",needlv = 1, taskDesp="装备强化(%s/1)次",        },--已添加
	{id=13, taskAward=5 , needNum=1   , panelName=nil,              npcId=4005004, con=const.PP_GONGXUN_TASK13, con2=const.PP_GX_TSAK_RECORD13, taskName="天降赐福",needlv = 1, taskDesp="参与天降赐福(%s/1)次",    },--已添加
	{id=14, taskAward=5 , needNum=1   , panelName=nil,              npcId=4005003, con=const.PP_GONGXUN_TASK14, con2=const.PP_GX_TSAK_RECORD14, taskName="尸王殿"  ,needlv = 1, taskDesp="参与尸王殿(%s/1)次",      },--已添加
	{id=15, taskAward=5 , needNum=1   , panelName=nil,              npcId=4005006, con=const.PP_GONGXUN_TASK15, con2=const.PP_GX_TSAK_RECORD15, taskName="精英阁"  ,needlv = 1, taskDesp="参与精英阁(%s/1)次",      },--已添加
	{id=16, taskAward=5 , needNum=1   , panelName=nil,              npcId=4002006, con=const.PP_GONGXUN_TASK16, con2=const.PP_GX_TSAK_RECORD16, taskName="神威魔狱",needlv = 1, taskDesp="参与神威魔狱(%s/1)次",    },--已添加
	{id=17, taskAward=5 , needNum=1   , panelName=nil,              npcId=4005005, con=const.PP_GONGXUN_TASK17, con2=const.PP_GX_TSAK_RECORD17, taskName="天下第一",needlv = 1, taskDesp="参与天下第一(%s/1)次",    },--已添加
	-- {id=18, taskAward=10, needNum=12  , panelName=nil,              npcId=nil,     con=const.PP_GONGXUN_TASK18, con2=const.PP_GX_TSAK_RECORD18, taskName="声望任务", taskDesp="完成日常声望任务",   },
}

--任务状态排序
local sort = function(a,b)
	if a.isCom~=b.isCom then
		return a.isCom < b.isCom
	else
		return a.id < b.id 
	end
end

function senderTaskData(player)
	local cloneData = util.clone(taskTable);
	for i,v in ipairs(cloneData) do
		if string.find(v.taskDesp, "/") then
			if util.ppn(player, v.con2) >= v.needNum then
				v.taskDesp = string.format(v.taskDesp, "<a color='#00ff00'>"..v.needNum.."</a>")
			else
				v.taskDesp = string.format(v.taskDesp, "<a color='#ff0000'>"..util.ppn(player, v.con2).."</a>")
			end
		end
	end
	local result = {
		cmd="senderTaskData",
		data=cloneData,
	}

	for i=1,#result.data do
		local taskData = result.data[i]
		taskData.isCom = util.ppn(player,taskData.con)
	end
	table.sort(result.data,sort)
	player:push_lua_table("PanelGongXun",util.encode(result))
end

function taskChuan(player,index)
	local taskData = taskTable[index]
	if not taskData then
		player:alert(1,1,"数据异常！")
		return
	end
	local lv = player:get_level()
	if lv < taskData.needlv then
		if taskData.lvdesp then
			player:alert(1,1,taskData.lvdesp)
		else
			player:alert(1,1,"等级不足,功能暂未开启")
		end
		return
	end
	if taskData.panelName then
		player:push_lua_table("open",util.encode({name = taskData.panelName}))
	end
	if taskData.npcId then
		posnpc.fly(player, taskData.npcId)
		player:push_lua_table("close",util.encode({name = "btn_main_boss"}))
	end

end

--gui.PanelGongXun.checkTaskComplete(player,index)
--做任务检测是否完成
function checkTaskComplete(player,index)
	-- print(index)
	local taskData = taskTable[index]
	if not taskData then
		player:alert(1,1,"数据异常！")
		return
	end
	if util.ppn(player,taskData.con)>=1 then
		-- player:alert(1,1,"该任务今日已完成！")
		return
	end
	local curTimes = util.ppn(player,taskData.con2)
	local newTimes = curTimes+1
	player:set_param(taskData.con2,newTimes)--累计参与次数
	if newTimes>=taskData.needNum then
		addGongXunScore(player,taskData.taskAward)
		player:set_param(taskData.con,1)--累计参与次数
	end

end





ckpanel.add_listener("gui.PanelGongXun.handlePanelData",gui.PanelGongXun.handlePanelData)



--功勋红点检测
function checkRedPoint(player)
	local boxNum = util.ppn(player,const.PP_GONGXUN_BOXNUM)--功勋宝箱数
	if boxNum>0 then
		return true
	end
	local curGx = util.ppn(player,const.PP_DAY_GONGXUN_NUM)
	for i=1,#gxTable do
		local itemData = gxTable[i]
		if util.ppn(player,itemData.con)<=0 and curGx>=itemData.gxNum then
			return true
		end
	end
	if util.ppn(player, const.PP_REDDOT_EXPLOIT_PANEL) == 0 then
		return true
	end
	return false
end
