--新游戏暗殿地图绑定脚本
module(..., package.seeall)
util.use_function(_M,task.task1000);
util.use_function(_M,task.task1100);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);

	if flags < 1 then
		local flagB = task.task1100.get_npc_flags(npc,player);flagB=task.util.fix_npc_flags(flagB);
		if flagB > 0 then flags = flagB; end
	end

	player:show_npc_flags(npc,flags);
end

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>殿内刷新暗之BOSS,数量多,刷新快,易击杀,掉落60级-转1装备和大量装备副卷，前期必爆</font>",
	},
	strs2={
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b>1.进入条件：</font><font color=#30ff00> 每次消耗500绑定元宝</font>",
		"<font color=#b2a58b>2.BOSS数量：<font color=#30ff00> 10个</font>",
		"<font color=#b2a58b>3.刷新时间：</font><font color=#30ff00> 8-60</font><font color=#b2a58b>分钟</font>",
		"<font color=#b2a58b>4.掉落材料：</font></font><font color=#30ff00> 海量</font><font color=#b2a58b>副本卷</font>",
		"<font color=#b2a58b>5.掉落装备：</font><font color=#30ff00> 70级</font><font color=#b2a58b>以下全系列装备</font>",
	},
	resData={talkTitle = "未知神殿",btnName = "进入未知神殿"},
	func = "npc.andian.btnEnter",
}

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	task_state = task.task1100.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1100.show_task(npc,player);return;
	end
	if PLATFORM_BANSHU then
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str=npc:get_dialog(),npcName=npc:get_name()}));
	else
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo}));
	end
end

local taskAnDianID = {
	10086, 10088, 10091
}

function btnEnter(player,data)
	local name=player:get_name()
	local s = player:get_task_state(const.TASK_ID_MAIN);
	local b = math.floor(s / const.TASK_STATE_NUM);
	local d = math.fmod(s,const.TASK_STATE_NUM);
	--print("btnEnter", b, d)
	local serverday = server_start_day()+1;
	if b == 10083 and d == const.TSACED then -- 主线未知暗殿，特殊处理
		return enterTaskAnDian(player);
	elseif table.indexof(taskAnDianID, b) and d == const.TSACED and serverday <= 2 then
		return enterTaskAnDian2(player);
	elseif b == 10082 and d == const.TSACED then
		return enterTaskAnDian(player)
	end

	if player:get_level() >= 60 then
		local vipLev = player:get_vcoin_bind();
		if player:get_vcoin_bind()>=500 then
			player:sub_vcoin_bind(500);
			local map = server.find_map("weizhi2");
			local x,y = map:gen_rand_position();
			-- local serverday = server_start_day()+1;
			if serverday>=5 then
				player:enter_map("weizhi5",x,y);
			elseif serverday>=4 then
				player:enter_map("weizhi4",x,y);
			elseif serverday>=3 then
				player:enter_map("weizhi3",x,y);
			elseif serverday>=2 then
				player:enter_map("weizhi2",x,y);
			else
				player:enter_map("weizhi1",x,y);
			end
			server.info(10,0,"玩家<font color='#01ebff'>"..name.."</font>进入<font color='#1eb8ff'>".."未知暗殿".."</font>,能不能打到好装备,就看他的运气了</font>");
		else
			player:alert(1,1,"绑元不足！绑元可通过每日签到、参加活动获得")
		end
	else
		player:alert(1,1,"等级达60级方可进入未知暗殿！")
	end
end

function enterTaskAnDian(player)
	player:enter_map("taskad",34,135)
	player:find_road_goto("taskad", 39,136,"autofightstart")
end

function enterTaskAnDian2(player)
	local map = server.find_map("taskad2");
	local x,y = map:gen_rand_position();
	if player:get_vcoin_bind()>=500 then
		player:sub_vcoin_bind(500);
		player:enter_map("taskad2", x, y);
	else
		player:alert(1,1,"绑元不足！绑元可通过每日签到、参加活动获得");
	end
end

ckpanel.add_listener("npc.andian.btnEnter",npc.andian.btnEnter)