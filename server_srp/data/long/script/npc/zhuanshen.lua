module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>神殿内刷新转生BOSS，玩家可进入符合当前等级的地图，实力一样，抢BOSS更公平，每次进入消耗一个转生神殿卷</font>",
	},
	strs2={
		"<font color=#30ff00>转生神殿一层  (70级以下)</font>",
		"<font color=#30ff00>转生神殿二层  (70级-转1)</font>",
		"<font color=#30ff00>转生神殿三层  (转1-转2)</font>",
		"<font color=#30ff00>转生神殿四层  (转3-转4)</font>",
		"<font color=#30ff00>转生神殿五层  (转5-转6)</font>",
		"<font color=#30ff00>转生神殿六层  (转7-转8)</font>",
		"<font color=#30ff00>转生神殿七层  (转9以上)</font>",
		"<font color=#b2a58b>  </font>",
		"<font color=#b2a58b>1.进入条件：1个</font><font color=#30ff00> 转生神殿卷</font>",
		"<font color=#b2a58b>2.刷新时间：</font><font color=#30ff00> 30-180</font><font color=#b2a58b>分钟</font>",
		"<font color=#b2a58b>3.掉落材料：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>材料</font>",
		"<font color=#b2a58b>4.掉落装备：</font><font color=#30ff00> 全系列</font><font color=#b2a58b>装备</font>",
		"<font color=#b2a58b>  </font>",
	},
	resData={talkTitle="转生神殿",btnName = "进入神殿",},
	func = "npc.zhuanshen.btnEnter",
}

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo}));
end

function btnEnter(player)
	local map_data={
	[1]={map="zssd7",level=70,zsLevel=9,flyid=4080007,floor=7},---9转以上
	[2]={map="zssd6",level=70,zsLevel=7,flyid=4080006,floor=6},---7--8
	[3]={map="zssd5",level=70,zsLevel=5,flyid=4080005,floor=5},---5--6
	[4]={map="zssd4",level=70,zsLevel=3,flyid=4080004,floor=4},---3--4
	[5]={map="zssd3",level=70,zsLevel=1,flyid=4080003,floor=3},---1--2
	[6]={map="zssd2",level=70,zsLevel=0,flyid=4080002,floor=2},---70级以上 0转
	[7]={map="zssd1",level=0,zsLevel=0,flyid=4080001,floor=1},---70级以下
}
	local name=player:get_name()
	local num = player:num_item(32000003)
	local level=player:get_level()
	local zslevel=util.ppn(player,const.PP_ZHSH_LEVEL)
	local x,y
	local map = server.find_map("zssd1")
	x,y=map:gen_rand_position()
	if num>=1 then 
		for i=1,#map_data do
			if zslevel>=map_data[i].zsLevel then 
				if level>=map_data[i].level then
					posmap.fly(player, map_data[i].flyid,"autofightstart") 			
					--player:enter_map(map_data[i].map,x,y)
					local floor=map_data[i].floor
					server.info(10,0,"玩家<font color='#01ebff'>"..name.."</font>进入<font color='#1eb8ff'>".."转生神殿"..floor.."层</font>,大量宝藏闪瞎了他的眼</font><a color='#00ff00' href='event:local_SeverOpen_c' underline='#00ff00'>我也要去</a></font>");
					player:remove_item(32000003,1)				
					return
				end
			end
		end	
	else
		player:alert(11,1,"转生神殿卷不足");
	end
end



function ondead(player)
	player:set_param(const.PP_ZSSD_DEADTIMES,util.ppn(player,const.PP_ZSSD_DEADTIMES)+1)
end

function onrelive( player )
	
	local param=util.ppn(player,const.PP_ZSSD_DEADTIMES)
	--print(param)
	local hp=player:get_hp_max()
	local sub_hp=param*0.06*hp
	local job = player:get_job_name();
	if param<=5 then 
		--player:set_status(86,86400,param,1,11)
		player:set_status(23,86400,-sub_hp,1,11)
		if job == "warrior" then
			local a,b = player:get_dc();
			player:set_status(10,86400,math.floor(b*0.1*param),1,11,900);
		elseif job == "wizard" then
			local a,b = player:get_mc();
			player:set_status(11,86400,math.floor(b*0.1*param),1,11,900);
		elseif job == "taoist" then
			local a,b = player:get_sc();
			player:set_status(12,86400,math.floor(b*0.1*param),1,11,900);
		end
		player:alert(11,1,"恭喜您成功获得0."..param.."倍攻击力加成");	
	end
end
ckpanel.add_listener("npc.zhuanshen.btnEnter", npc.zhuanshen.btnEnter);


-- local mapid = player:get_map():get_id();
-- 	if mapid == "v390" then 
-- 		npc.zhuanshen.ondead(player) 
-- 	end
-- 	if string.sub(mapid,1,3)=="zmt" then