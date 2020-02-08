module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>BOSS之家，巅峰传奇是最多的BOSS的地方，数量多，刷新快，掉落全系装备。</font>",
	},
	strs2={
		"<font color=#b2a58b>进入条件：</font><font color=#30ff00> 成为VIP</font>",
		"<font color=#b2a58b>一层BOSS：<font color=#30ff00> 10</font>个</font>",
		"<font color=#b2a58b>二层BOSS：<font color=#30ff00> 5</font>个</font>",
		"<font color=#b2a58b>三层BOSS：<font color=#30ff00> 3</font>个</font>",
		"<font color=#b2a58b>刷新时间：</font><font color=#30ff00> 150-360</font><font color=#b2a58b>分钟</font>",
		"<font color=#b2a58b>掉落材料：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>材料</font>",
		"<font color=#b2a58b>掉落装备：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>装备</font>",
	},
	resData={talkTitle="BOSS之家",btnName = "进入boss之家",},
	func = "npc.boss.btnEnter",
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
	local name=player:get_name()
	if player:get_level() >= 50 then
		local vipLev = util.ppn(player,const.PP_VIP_LEVEL)
		if vipLev>0 then
			--print("1111111111")
			player:enter_map("bosshome1f",42,44)
			server.info(10,0,"VIP玩家<font color='#01ebff'>"..name.."</font>进入<font color='#1eb8ff'>".."BOSS之家".."</font>,BOSS之家可以从盟重BOSS之家传送员进入</font><a color='#00ff00' href='event:local_SeverOpen_a' underline='#00ff00'>我也要去</a></font>");
		else
			player:alert(1,1,"成为VIP方可进入BOSS之家！")
		end
	else
		player:alert(1,1,"等级达50级方可进去BOSS之家！")
	end
end


ckpanel.add_listener("npc.boss.btnEnter",npc.boss.btnEnter)

