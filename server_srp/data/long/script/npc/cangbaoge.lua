module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>藏宝阁内BOSS众多,掉落高级转生装备。藏宝阁共两张地图,沙城行会和非沙城行会成员进入的地图不同,非行会成员不可进入。</font>",
	},
	strs2={
		"<font color=#b2a58b>进入条件：</font><font color=#30ff00> 开服第五天,有帮会的玩家</font>",
		"<font color=#b2a58b>沙城行会：<font color=#30ff00> 4个BOSS</font></font>",
		"<font color=#b2a58b>非沙行会：<font color=#30ff00> 6个BOSS</font></font>",
		"<font color=#b2a58b>刷新时间：</font><font color=#30ff00> 120-360</font><font color=#b2a58b>分钟</font>",
		"<font color=#b2a58b>掉落材料：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>材料</font>",
		"<font color=#b2a58b>掉落装备：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>装备</font>",
	},
	resData={talkTitle="藏宝阁",btnName = "进入藏宝阁",},
	func = "npc.cangbaoge.btnEnter",
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

function btnEnter(player)
	local name=player:get_name();
	if server_start_day() >= 5 then
		if server.is_war() == 0 and server.get_king_guild() ~= "" then
			local guild = player:get_guild();
			if guild == server.get_king_guild() then
				player:enter_map("cangbaoge1",37,67)
				server.info(10,0,"玩家<font color='#01ebff'>"..name.."</font>进入<font color='#1eb8ff'>".."藏宝阁".."</font>,藏宝阁可以从比奇藏宝阁传送员进入</font><a color='#00ff00' href='event:local_FlyTo_4005007' underline='#00ff00'>我也要去</a></font>");
			    return;
			elseif guild ~= "" then
				player:enter_map("cangbaoge2",18,67)
				server.info(10,0,"玩家<font color='#01ebff'>"..name.."</font>进入<font color='#1eb8ff'>".."藏宝阁".."</font>,藏宝阁可以从比奇藏宝阁传送员进入</font><a color='#00ff00' href='event:local_FlyTo_4005007' underline='#00ff00'>我也要去</a></font>");
			else
				player:alert(1,1,"未加入行会不可以进入藏宝阁!")
			end
		else
			player:alert(1,1,"皇城战期间或皇城无归属藏宝阁不可进入!")
		end

	else
		player:alert(1,1,"藏宝阁开服第五天开启！")
	end
end


ckpanel.add_listener("npc.cangbaoge.btnEnter",npc.cangbaoge.btnEnter)

