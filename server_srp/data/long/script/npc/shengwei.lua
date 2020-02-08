module(..., package.seeall)
util.use_function(_M,task.task1000);

local award_data={
[1]={mon=11002038,x=29,y=45,boss=11002038,card=1,flyid=4090001,award={----------第一层显示
				[1]={name="白银矿石",id=24020003,num=1,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[2]={mon=11002039,x=16,y=18,boss=11002039,card=3,flyid=4090002,award={
				[1]={name="白银矿石",id=24020003,num=1,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[3]={mon=11002040,x=24,y=73,boss=11002040,card=5,flyid=4090003,award={
				[1]={name="白银矿石",id=24020003,num=1,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[4]={mon=11002041,x=73,y=36,boss=11002041,card=7,flyid=4090004,award={----------第一层显示
				[1]={name="白银矿石",id=24020003,num=1,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[5]={mon=11002042,x=42,y=96,boss=11002042,card=9,flyid=4090005,award={
				[1]={name="白银矿石",id=24020003,num=2,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[6]={mon=11002043,x=63,y=50,boss=11002043,card=12,flyid=4090006,award={
				[1]={name="白银矿石",id=24020003,num=2,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[7]={mon=11002044,x=49,y=37,boss=11002044,card=15,flyid=4090007,award={----------第一层显示
				[1]={name="白银矿石",id=24020003,num=2,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[8]={mon=11002045,x=12,y=12,boss=11002045,card=18,flyid=4090009,award={
				[1]={name="白银矿石",id=24020003,num=2,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[9]={mon=11002046,x=59,y=34,boss=11002046,card=21,flyid=4090009,award={
				[1]={name="白银矿石",id=24020003,num=3,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},
[10]={mon=11002047,x=33,y=20,boss=11002047,card=0,flyid=4090010,award={----------第一层显示
				[1]={name="白银矿石",id=24020003,num=1,},
				[2]={name="金砖(小)",id=23060001,num=1,},

			},},

}

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>  囚禁万千神魔的地底监狱，漆黑的入口带着神秘的力量让人不自觉的向前走去...</font>",
	},
	strs2={
		"<font color=#b2a58b>1. 进入条件：</font>",
		"<font color=#b2a58b><font color=#30ff00>   80级</font>，消耗<font color=#30ff00>神威魔狱通行证</font></font>",
		"<font color=#b2a58b>   再次进入继续<font color=#30ff00>上次层数，无消耗</font>",
		"<font color=#b2a58b>2. 开放时间： </font><font color=#30ff00>10:00-11:00 </font>",
		"<font color=#b2a58b><font color=#30ff00>             14:00-15:00</font> </font>",
		"<font color=#b2a58b><font color=#30ff00>             22:00-23:00</font> </font>",
		"<font color=#b2a58b>3. 共10层，boss掉落<font color=#30ff00>稀有装备</font></font>",
		"<font color=#b2a58b>4. <font color=#30ff00> 每进一层</font>获得大量奖励</font>",
		"<font color=#b2a58b>5. 击败<font color=#30ff00>神威魔狱统领</font>获得<font color=#30ff00>神威</font>",
		"<font color=#b2a58b><font color=#30ff00>   魔狱通行证</font>才能进入下一层</font>",
		"<font color=#b2a58b>6. <font color=#30ff00> 死亡不掉落</font>自身物品</font>",
		"<font color=#b2a58b>       </font>",
	},
	resData={talkTitle="神威魔域",btnName = "进入活动",},
	func = "npc.shengwei.btnEnter",
}


function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end
function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	elseif PLATFORM_BANSHU then
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str=npc:get_dialog(),npcName=npc:get_name()}));
	elseif string.sub(player:get_map():get_id(),1,4)=="swmy" then
		local NOWMAP=tonumber(string.sub(player:get_map():get_id(),5,6))
		local CARD=award_data[NOWMAP].card
		local bosssInfo2 =	{
			strs1={
			"<font color=#b2a58b>神威魔狱，刷前期小boss，刷新快，掉落1转之前全职业装备和副本卷轴</font>",
			"<font color=#b2a58b>再次进入到过的层数时无需消耗神威魔狱通行证</font>",
			},
			strs2={
				"<font color=#b2a58b>说明：</font>",
				"<font color=#b2a58b>当前层数<font color=#30ff00>"..NOWMAP.."</font>/10</font>",
				"<font color=#b2a58b>进入下一层需要神威魔狱通行证<font color=#30ff00>"..CARD.."</font>个</font>",
				"<font color=#b2a58b>击杀神威魔狱精英可或得神威魔狱通行证</font>",

			},
			resData={talkTitle="神威魔域",btnName = "进入下一层",},
			func = "npc.shengwei.btnEnter",
			}
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo2}));
	else
		player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo}));
	end
end

function mon_gen()
	-- local x,y
	for i=1,10 do
		local map = server.find_map("swmy"..i);
		-- x,y=map:gen_rand_position()
		if map then
			map:clear_mon_by_id(award_data[i].mon)
			map:mon_gen(award_data[i].x,award_data[i].y,award_data[i].mon,1,1,1,1,"no_owner");
			--server.info(10000,0,"神威魔狱活动!大家赶紧前往击杀");
		end
		--map:map_exe("npc.shiwang.updataUIleftTips")
	end
end


function btnEnter(player)
	if server.get_temp_var("shengweistate")=="close" then
		player:alert(11,1,"活动时间未到");
		return
	end
	--local clock = tonumber(os.date("%H%M",os.time()))
	--if clock >= 1000 and clock <= 1100 then
	--	elseif clock >= 1400 and clock <= 1500 then
	--	elseif clock >= 2200 and clock <= 2300 then
	--else
	--	server.set_temp_var("shengweistate","close")
	--	return player:alert(11,1,"活动时间未到");
	--end

	if player:get_level()<80 then
		player:alert(11,1,"等级不足80级");
		return
	end
	local x,y
	local map = server.find_map("swmy1")
	x,y=map:gen_rand_position()
	if string.sub(player:get_map():get_id(),1,4)=="swmy" then------已经在神威
		local cur_map = tonumber(string.sub(player:get_map():get_id(),5,6))
		local index=cur_map+1

		if player:get_map():num_monster(award_data[cur_map].mon) == 0 then
			if util.ppn(player,const.PP_SHENGWEI_ARRIVR) >cur_map then ----已经到达过
				--player:enter_map("swmy"..index,x,y)
				posmap.fly(player, award_data[index].flyid,"autofightstart")
				updataUIleftTips(player)
			else
				if  player:num_item(32000004) >=award_data[cur_map].card then
					player:remove_item(32000004,award_data[cur_map].card);----移除card
					--print(cur_map)
					for i=1,#award_data[cur_map].award do
						player:add_item(award_data[cur_map].award[i].id,award_data[cur_map].award[i].num,1)----获取上层奖励
					end
					player:set_param(const.PP_SHENGWEI_ARRIVR,util.ppn(player,const.PP_SHENGWEI_ARRIVR)+1);----记录到过的层数
					--player:enter_map("swmy"..index,x,y)
					posmap.fly(player, award_data[index].flyid )
					updataUIleftTips(player)
				else
					player:alert(11,1,"通行证不足");
				end
			end
		else
			player:alert(11,1,"BOSS未击杀不可进入");
			return
		end
	else
		-- if server.get_temp_var("shengweistate")=="end" then
		-- 	player:alert(11,1,"活动入口关闭");
		-- 	return
		-- end
		posmap.fly(player, award_data[1].flyid,"autofightstart")
		if  util.ppn(player,const.PP_SHENGWEI_ARRIVR)==0 then
			-- for i=1,#award_data[1].award do
			-- 	player:add_item(award_data[1].award[i].id,award_data[1].award[i].num)----获取上层奖励
			-- end
			player:set_param(const.PP_SHENGWEI_ARRIVR,1)
			player:set_param(const.PP_SHENGWEI_ARRIVR_HOUR,hour());
			gui.PanelGongXun.checkTaskComplete(player,16)
		end
		updataUIleftTips(player)
	end
end
function onMonKill(player,pmap)
	if pmap then
	-- 	if pmap:num_monster(award_data[index].mon)==0 then
		pmap:map_exe("npc.shengwei.freshUIleftTips")
	end
	-- end
end
function clear_arrive_param(player)
	--print("clear_arrive_param")
	player:set_param(const.PP_SHENGWEI_ARRIVR,0);
end

--处理客户端数据发送(叶签)
function handlePanelData(player,commond)

	local data = util.decode(commond)
	if data.actionid == "open_panel" then
		local data_table={}
		for k,v in ipairs(nameTable) do
			if not v.openFunc or (v.openFunc and v.openFunc()) then
				table.insert(data_table,{
					index = v.index,
					title=v.title,
					name=v.name,
					uif=v.uif,
				})
			end
		end
		local result={
			cmd="req_open",
			data=data_table,
		}
		player:push_lua_table("PanelCaiLiao",util.encode(result))
	end
	if data.actionid == "back" then
		local result={}
		result.str = "退出活动后,将失去即将获得的奖励，是否继续?"
		result.labelConfirm = "确定"
		result.labelCancel = "取消"
		result.callFunc = "npc.shengwei.btnSure"
		result.book = ""
		player:push_lua_table("showConfirm",util.encode(result))

	end

end

ckpanel.add_listener("npc.shengwei.handlePanelData",npc.shengwei.handlePanelData)

function btnSure(player)
	--posmap.fly(player,2020011)
	--player:enter_map("tucheng", 39, 42)
	posmap.fly(player,2020006)
end
ckpanel.add_listener("npc.shengwei.btnSure", npc.shengwei.btnSure);

function clearhour( player )
	local hour=hour()
	if hour - util.ppn(player,const.PP_SHENGWEI_ARRIVR_HOUR) >1 then
		player:set_param(const.PP_SHENGWEI_ARRIVR,0);
	end
end
login.add_login_listener(clearhour);

function updataUIleftTips(player)----------捡到card刷新
	local index = tonumber(string.sub(player:get_map():get_id(),5,6))
	local map = player:get_map()

	local result={}
	-- print(map:num_monster(11002038))
	-- print(map:num_monster("摩萨教主"))
	result.killmon=map:num_monster(award_data[index].mon)
	result.mon=1
	result.killboss=player:num_item(32000004)
	result.boss=award_data[index].card
	--result.time=(60*60-(os.time()-tonumber(server.get_var("shengweiTime"))))----如果有问题可在打怪时不刷新时间
	result.time=60*60+tonumber(server.get_var("shengweiTime"))
	result.award=award_data[index].award
	result.falg=0
	player:push_lua_table("showshengwei",util.encode(result))

end

function freshUIleftTips(player)----------捡到card刷新
	local index = tonumber(string.sub(player:get_map():get_id(),5,6))
	local map = player:get_map()

	local result={}
	-- print(map:num_monster(11002038))
	-- print(map:num_monster("摩萨教主"))
	result.killmon=map:num_monster(award_data[index].mon)
	result.mon=1
	result.killboss=player:num_item(32000004)
	result.boss=award_data[index].card
	result.time=60*60+tonumber(server.get_var("shengweiTime"))
	result.award=award_data[index].award
	result.falg=1
	player:push_lua_table("showshengwei",util.encode(result))

end
ckpanel.add_listener("npc.shengwei.btnEnter", npc.shengwei.btnEnter);