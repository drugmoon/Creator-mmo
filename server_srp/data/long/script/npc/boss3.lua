module(..., package.seeall)
util.use_function(_M,task.task1000);

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>三层刷新3个超级BOSS，只有强力玩家才能击杀，获得丰厚材料和极品装备。</font>",
	},
	strs2={
		"<font color=#b2a58b>1. 进入条件：<font color=#30ff00>VIP3</font>免费进入</font>",
		"<font color=#b2a58b>            VIP1-2消耗<font color=#30ff00>100元宝</font>",
		"<font color=#b2a58b>2. BOSS数量：<font color=#30ff00>3个</font></font>",
		"<font color=#b2a58b>3. 刷新时间：</font><font color=#30ff00>240-360</font><font color=#b2a58b>分钟</font>",
		"<font color=#b2a58b>4. 掉落材料：</font><font color=#30ff00>全系列</font><font color=#b2a58b>材料</font>",
		"<font color=#b2a58b>5. 掉落装备：</font><font color=#30ff00>全系列</font><font color=#b2a58b>装备</font>",
	},
	resData={titleRes="BOSS之家",btnName = "进入boss之家",},
	func = "npc.boss3.btnEnter",
}

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	if PLATFORM_BANSHU then
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str=npc:get_dialog(),npcName=npc:get_name()}));
	else
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo}));
	end
end

function btnEnter(player,data)
	if player:get_level() >= 50 then
		local vipLev = util.ppn(player,const.PP_VIP_LEVEL)
		if vipLev>=3 then
			player:enter_map("bosshome3f",7,39)
		elseif vipLev==1 or vipLev==2 then
			if player:get_vcoin()>=100 then
				player:sub_vcoin(100,"BOSS之家")
				player:enter_map("bosshome3f",7,39)
			else
				-- player:alert(1,1,"元宝不足！")
				showChongzhi(player)
			end
		else
			player:alert(1,1,"成为VIP2或者VIP1-2+100元宝方可进入三层！")
		end
	else
		player:alert(1,1,"等级达50级方可进去BOSS之家！")
	end
end


ckpanel.add_listener("npc.boss3.btnEnter",npc.boss3.btnEnter)

