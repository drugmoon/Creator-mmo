-- module(..., package.seeall)
-- util.use_function(_M,task.task1000);
-- local ACTIVITY_NAME = "玛雅神殿";

-- function onRefreshShowFlags(npc,player)
-- 	local flags = 0;
-- 	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
-- 	-- print(flags)
-- 	player:show_npc_flags(npc,flags);
-- end

-- function onTalk100(npc,player)
-- 	local task_state = task.task1000.get_npc_flags(npc,player);
-- 	-- print(task_state)
-- 	if task_state > 0 then
-- 		task.task1000.show_task(npc,player);return;
-- 	end
-- 	npcPanel(player)
-- end

-- function panelJump(player)
-- 	player:enter_map("v002",92,107)
-- 	npcPanel(player)
-- end

-- function npcPanel(player)
-- 	local times,awards,weekDay,durationTimes,level = gui.PanelActivityGuide.getActivityInfo(ACTIVITY_NAME)
-- 	local talk_tab = {
-- 		func = "npc.maya.btnCall",
-- 		title = {str=ACTIVITY_NAME,fontSize = 24,color=0xFFFFFF},
-- 		con_tab = {
-- 			{mtype="text",		fontSize = 24,	posX=010,posY=280,color="0xC8C8C8",	str="神殿守卫:",},
-- 			{mtype="text",		fontSize = 22,	posX=010,posY=250,color="0xC8C8C8",	str="    古老的邪恶从远古深渊中醒来，大陆的守护法神，用自己的生命将它",},
-- 			{mtype="text",		fontSize = 22,	posX=010,posY=220,color="0xC8C8C8",	str="们禁锢在了残破的神殿。从此大陆上有了一座禁锢众多邪恶君主与其爪",		},
-- 			{mtype="text",		fontSize = 22,	posX=010,posY=190,color="0xC8C8C8",	str="牙的玛雅神殿。年轻的勇者，你们准备好了去征服它们了么?",		},
-- 			--{mtype="text",		str="奖励："..awards,											fontSize = 22,	posX=010,posY=35,color="0x00FF00"},			
-- 			{mtype="button",name="查看爆率",  funcIndex="check",  res="btn_transmit-button", haveNext = true, posX=400,posY=35},
-- 			{mtype="button",name="进入",  funcIndex="enter", res="btn_transmit-button", posX=600,posY=35},
-- 			{mtype="text",str="进入条件："..(level == 0 and "没有限制" or "等级达到"..level.."级"),fontSize = 22,posX=010,posY=95,color="0xff0000"},
-- 			{mtype="image",		res="img_separator_horizontal",	posX=350,posY=75, imgState = "rule",anch = 1},
-- 		}
-- 	}
-- 	local inActivity = {}
-- 	local weekDayLbl,inTimesLbl = "",""
-- 	if #weekDay >0 then
-- 		for i = 1,#weekDay do
-- 			weekDayLbl = weekDayLbl.."周"..weekDay[i]..(i == #weekDay and "的" or "，")		
-- 		end
-- 	end
-- 	for i,v in ipairs(times) do
-- 		inTimesLbl = inTimesLbl..v.startHour..":"..v.startMinute.."~"..v.endHour..":"..v.endMinute..(i == #times and "" or "；")
-- 	end
-- 	local posY,titleText = 125,"活动时间："
-- 	if #durationTimes >0 then
-- 		local durationTimesLbl = ""
-- 		for i,v in ipairs(durationTimes) do
-- 			durationTimesLbl = durationTimesLbl..v..(i == #times and "" or "；")
-- 		end
-- 		table.insert(talk_tab.con_tab,{
-- 			mtype="text",
-- 			str="持续时间："..durationTimesLbl,
-- 			fontSize = 22,
-- 			posX=010,
-- 			posY=125,
-- 			color="0xffa200",
-- 		})
-- 		posY = 155
-- 		titleText = "入口开启："
-- 	end
-- 	inActivity = {
-- 		mtype="text",
-- 		str=titleText..weekDayLbl..inTimesLbl,
-- 		fontSize = 22,
-- 		posX=010,
-- 		posY=posY,
-- 		color="0xffa200",
-- 	}
-- 	table.insert(talk_tab.con_tab,inActivity)

-- 	player:push_lua_table("talk_npc",util.encode(talk_tab))
-- end

-- function alertClient(player)
-- 	-- if player:get_level() < gui.PanelActivityGuide.getLevelLimit(ACTIVITY_NAME) then return end

-- 	-- player:push_lua_table("activity", util.encode({msg = ACTIVITY_NAME.."开启，点击传送至"..ACTIVITY_NAME, flyId = 1003, duration = 30*60,type=9}))
-- end

-- function endMaya(player) 
-- 	go.home(player)
-- end

-- function btnCall(player,data)
-- 	if data == "enter" then
-- 		local levelLimit = gui.PanelActivityGuide.getLevelLimit(ACTIVITY_NAME)
-- 		local isOpen = gui.PanelActivityGuide.isActivityOpen(ACTIVITY_NAME)
-- 		if player:get_level() >= levelLimit then
-- 			if isOpen then
-- 				player:enter_map("v203",38,109);
-- 				player:set_param(const.PP_MAYA_ENTER,1)
-- 			else
-- 				player:alert(1,0,"当前神殿已关闭，不能进入哦！");
-- 		    end
-- 			-- mon.bossRefresh.updateData(player)
-- 			count.countTimes("玛雅神殿")
-- 		else
-- 			player:alert(1,1,"等级过低无法进入！");
-- 		end
		
-- 	elseif data == "check" then
-- 		local param = {
-- 		panel = "hint", visible = true, lblAlert1 = "玛雅神殿爆率查看", lblAlert2 = {
-- 			"有几率获得<font color=#00ff00>9转以下(包括9转)的所有装备</font>以及各类宝箱等道具.",
-- 		},
-- 		alertTitle = "好的,我知道了",
-- 				}
-- 		player:push_lua_table("alert",util.encode(param))
-- 	end

-- end

-- ckpanel.add_listener("npc.maya.btnCall",npc.maya.btnCall)