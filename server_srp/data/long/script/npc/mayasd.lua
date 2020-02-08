module(..., package.seeall)

util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local bosssInfo =	{
	strs1={
		"<font color=#30ff00></font>",
		"<font color=#b2a58b>  世上真的有长生不老药么？天晓得</font>",
	},
	strs2={
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b>1.进入条件：1个</font><font color=#30ff00>玛雅卷轴</font>",
		"<font color=#b2a58b>2.每层BOSS：</font><font color=#30ff00> 3个</font>",
		"<font color=#b2a58b>3.刷新时间：</font><font color=#30ff00> 30-180</font><font color=#b2a58b>分钟</font>",
		"<font color=#b2a58b>4.掉落材料：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>材料</font>",
		"<font color=#b2a58b>5.掉落装备：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>装备</font>",
	},
	resData={talkTitle="玛雅神殿",btnName = "进入神殿"},
	func = "npc.mayasd.btnEnter",
}

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo}));
end

function btnEnter(player)
	local name=player:get_name()
	local num = player:num_item(32000002)
	local x,y
	local map = server.find_map("mysd1f")
	x,y=map:gen_rand_position()
	if player:get_level()>=60 then
		if num>=1 then 
			--4070001
			posmap.fly(player, 4070001,"autofightstart") 			
			--player:enter_map("mysd1f",x,y)
			player:remove_item(32000002,1)
			server.info(10,0,"玩家<font color='#01ebff'>"..name.."</font>进入<font color='#1eb8ff'>".."玛雅神殿".."</font>,玛雅神殿可以从盟重玛雅神殿传送员进入</font><a color='#00ff00' href='event:local_SeverOpen_b' underline='#00ff00'>我也要去</a></font>");
		else
			player:alert(11,1,"玛雅卷轴不足");
		end
	else
		player:alert(11,1,"等级不足60级");
	end
end
ckpanel.add_listener("npc.mayasd.btnEnter", npc.mayasd.btnEnter);