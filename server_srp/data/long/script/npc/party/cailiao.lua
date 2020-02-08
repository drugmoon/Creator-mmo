module(... ,package.seeall)
util.use_function(_M,task.task1000);

--------------------------
--没有在每天清空技能如地图次数
--
--------------
function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("open",util.encode({name = "panel_cailiao"}))
end

local nameTable = {
	[1]={index = 1,title="沃玛秘境",	},
	[2]={index = 2,title="香石墓穴",	},
	[3]={index = 3,title="牛魔寺庙",	},
	[4]={index = 4,title="死亡山谷",	},
	[5]={index = 5,title="沙漠荒原",	},
}

local  award_table={
	[1]={index = 1,times =const.PP_CLFU_TIME1,time=30*60*1000,juan=1,needvcoin=188,monnum=25,flyid=4030001,lv=60,
		award={
				[1]={name="人物经验",id=40000001,num=1500000,},
				[2]={name="护盾碎片（小）",id=23010001,num=100,},

			},
		mon={
				[1]={name="跳跳蜂",num=8,id=7001001,x=18,y=31},
				[2]={name="剧毒蜈蚣",num=8,id=7001002,x=21,y=34},
				[3]={name="蠕虫",num=9,id=7001003,x=24,y=37},

			}
	},
	[2]={index = 2,times = const.PP_CLFU_TIME2,time=30*60*1000,juan=1,needvcoin=188,monnum=30,flyid=4030002,lv=60,
		award={
				[1]={name="人物经验",id=40000001,num=500000,},
				[2]={name="羽毛",id=24030001,num=12,},

			},
			mon={
				[1]={name="小盔甲虫",num=8,id=7002001,x=18,y=32},
				[2]={name="沙漠苍狼",num=8,id=7002002,x=20,y=36},
				[3]={name="沙漠鹰",num=7,id=7002003,x=23,y=38},
				[4]={name="穿山甲",num=7,id=7002004,x=25,y=34},

			},
	},
	[3]={index = 3,times =const.PP_CLFU_TIME3,time=30*60*1000,juan=1,needvcoin=188,monnum=30,flyid=4030003,lv=66,
		award={
				[1]={name="人物经验",id=40000001,num=500000,},
				[2]={name="龙心碎片（小）",id=23020001,num=12,},
			},
			mon={
				[1]={name="红野猪",num=10,id=7003001,x=17,y=32},
				[2]={name="蛇蝎",num=10,id=7003002,x=19,y=35},
				[3]={name="墓穴恶蛆",num=10,id=7003003,x=24,y=34},

			},
	},
	[4]={index = 4,times = const.PP_CLFU_TIME4,time=30*60*1000,juan=1,needvcoin=188,monnum=35,flyid=4030004,lv=70,
		award={
			[1]={name="人物经验",id=40000001,num=1500000,},
			[2]={name="狼牙碎片（小）",id=23030001,num=2,},
			},
			mon={
				[1]={name="邪恶战士",num=12,id=7004001,x=35,y=33},
				[2]={name="邪恶守卫",num=12,id=7004002,x=29,y=39},
				[3]={name="邪恶祭司",num=11,id=7004003,x=22,y=41},

			},
	},
	[5]={index = 5,times =const.PP_CLFU_TIME5,time=30*60*1000,juan=1,needvcoin=188,monnum=30,flyid=4030005,lv=75,
		award={
				[1]={name="人物经验",id=40000001,num=500000,},
				[2]={name="狼牙碎片（小）",id=23030001,num=2,},
			},
			mon={
				[1]={name="摩萨卫士",num=10,id=7005001,x=24,y=33},
				[2]={name="巨鼠",num=10,id=7005002,x=18,y=36},
				[3]={name="摩萨战士",num=10,id=7005003,x=21,y=39},

			},

	},

}
--clfb100
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
					allTimes=2,
					useTimes=util.ppn(player,award_table[k].times),
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
		result.str = "退出副本后,将失去即将获得的奖励，是否继续?"
		result.labelConfirm = "确定"
		result.labelCancel = "取消"
		result.callFunc = "npc.party.cailiao.btnSure"
		result.book = ""
		player:push_lua_table("showConfirm",util.encode(result))

	end
	if data.actionid == "single" then
		get_award_single(player)
	end
	if data.actionid == "double" then
		get_award_double(player)
	end
end

ckpanel.add_listener("npc.party.cailiao.handlePanelData",npc.party.cailiao.handlePanelData)

function btnSure(player)
	posmap.fly(player,2020011)
	--player:enter_map("tucheng", 39, 42)
end
ckpanel.add_listener("npc.party.cailiao.btnSure", npc.party.cailiao.btnSure);

function get_award_single(player)
	local map = player:get_map()
	local index=1
	if map then
		local mapid = map:get_id()
		if string.sub(mapid,2,5) == "clfb" then
			index=tonumber(string.sub(mapid,1,1))
		end
	end
	if player:num_bag_black() < #award_table[index].award then
		player:alert(1,1,"背包空间不足!")
		return
	else
		for i=1,# award_table[index].award do
			player:add_item(award_table[index].award[i].id, award_table[index].award[i].num,1)
		end
		btnSure(player)
	end
end

function get_award_double(player)
	local map = player:get_map()
	local index=1
	if map then
		local mapid = map:get_id()
		if string.sub(mapid,2,5) == "clfb" then
			index=tonumber(string.sub(mapid,1,1))
		end
	end
	--print("----",index)
	if player:get_vcoin()>=award_table[index].needvcoin then
		if player:num_bag_black() >=#award_table[index].award then
			player:sub_vcoin(award_table[index].needvcoin, "材料副本")
			for i=1,#award_table[index].award do
				player:add_item(award_table[index].award[i].id, award_table[index].award[i].num*2,1)
			end
			btnSure(player)
		else
			player:alert(1,1,"背包空间不足!")
			return
		end
	else
		showChongzhi(player)
		--player:alert(1,1,"元宝不足")
		return
	end
end
function goto(player,commond)
	local data = util.decode(commond)
	local index=tonumber(data.actionid)
	for i=1,#award_table do
		if data.actionid==award_table[i].index then
			enter_fbmap(player,index)
			player:push_lua_table("close",util.encode({name = "panel_cailiao"}))
		end
	end
end
ckpanel.add_listener("npc.party.cailiao.goto",npc.party.cailiao.goto)

function enter_fbmap(player,index)
	local map=find_empty_room(index)
	local curMapId = player:get_map():get_id()
	local mon_data=award_table[index]

	local s = player:get_task_state(const.TASK_ID_MAIN);
	local b = math.floor(s / const.TASK_STATE_NUM);
	local d = math.fmod(s,const.TASK_STATE_NUM);

	local td = tab.TaskData1000[b]
	if map == nil then
		player:alert(1,1,"请稍等,副本正在加载中")
		return
	end
	if td.conditionType ~= 30 or td.checkID~="2clfb" then
		local x,y
		x,y=map:gen_rand_position()
		if string.find(curMapId, "clfb") then
			player:alert(1,1,"当前在副本中无法再出入副本！")
			return
		end
		if util.ppn(player, award_table[index].times)>=2 then
			player:alert(1,1,"今日挑战次数已用完")
			return
		end
		if player:get_level() <award_table[index].lv then
			player:alert(1,1,"等级不足，暂不可进入")
			return
		end
		if player:num_item(32000001,1) < award_table[index].juan then
			player:alert(1,1,"副本卷轴不足！")
			return
		end
	end

	map:clear_mon();
	map:clear_item();
	map:clear_mininpc();

	map:set_timer(1, award_table[index].time);--------设置本地图中出去的时间
	for i=1,#mon_data.mon do
		--local a,b=map:gen_rand_position()
		map:mon_gen(mon_data.mon[i].x, mon_data.mon[i].y, mon_data.mon[i].id,mon_data.mon[i].num,7,1,1,"show_name");-----
	end
	posmap.fly(player, award_table[index].flyid, "autofightstart")
	--player:enter_map(map:get_id(),x,y);
	show_leftTips(player,index)
	player:set_param(award_table[index].times,util.ppn(player, award_table[index].times)+1)
	--player:set_param(const.PP_CAILAIO_TIME_FLAG,os.time()+30*60)
	task.simulateTask.showTaskDesp(player, 6001);
	if td.conditionType ~= 30 or td.checkID~="2clfb" then
		player:remove_item(32000001,award_table[index].juan);
		gui.PanelGongXun.checkTaskComplete(player,3)
	end
end

function find_empty_room(index)
	local map = nil;
	-- for i=1,100 do
	map = server.find_map(index.."clfb");
	if map and map:num_player()<1 then
		return map;
	end
	-- end
	return map;
end
function show_leftTips(player,index)

	local mon_data=award_table[index]
	local map = player:get_map()
	local result={}
	--print(map:num_monster(mon_data.mon[1].id),map:num_monster(mon_data.mon[2].name))
	result.killmon=mon_data.monnum - map:num_monster()
	result.mon=mon_data.monnum
	result.killboss=0
	result.boss=0
	result.time=os.time()+1800
	--print(result.time)
	--print(">>>>>",result.time)
	result.vcoin=mon_data.needvcoin
	result.award=mon_data.award
	result.title=nameTable[index].title
	result.falg=0
	result.func="npc.party.cailiao.handlePanelData"
	player:push_lua_table("showCaiLiao",util.encode(result))
end

function refrash_leftTips(player,index)

	local mon_data=award_table[index]
	local map = player:get_map()
	local result={}
	--print(map:num_monster(mon_data.mon[1].id),map:num_monster(mon_data.mon[2].name))
	result.killmon=mon_data.monnum - map:num_monster()
	result.mon=mon_data.monnum
	result.killboss=0
	result.boss=0
	result.time=os.time()+1800
	--print(">>>>>",result.time)
	result.vcoin=mon_data.needvcoin
	result.award=mon_data.award
	result.title=nameTable[index].title
	result.falg=1
	result.func="npc.party.cailiao.handlePanelData"
	player:push_lua_table("showCaiLiao",util.encode(result))
end

function onMonKill(player,index)
	if index > 0 and index <= #award_table then
		refrash_leftTips(player,index)
	end
end

function checkTimesComplete(player)
	local flag = false
	for i=1,#award_table  do
		if util.ppn(player, award_table[i].times)<=1 then
			flag= true
			break
		end
	end
	return flag
end

function show_count( player,commond )
	local result = {}
	local data = util.decode(commond)
	local times= 2-util.ppn(player, award_table[data.actionid].times)<=0 and 0 or  2-util.ppn(player, award_table[data.actionid].times)
	local num_juan=player:num_item(32000001,1)
 	for i=1,#award_table do ---------------人物面板
		if data.actionid==award_table[i].index then
			result.award = award_table[i].award
			result.times = times
			result.lv = award_table[i].lv
			result.juan = award_table[i].juan.."(拥有"..num_juan.."个)"
			result.time = award_table[i].time/60/1000
			result.index = award_table[i].index
			player:push_lua_table("PanelCaiLiao_go",util.encode(result))
		end
	end
 end
ckpanel.add_listener("npc.party.cailiao.show_count",npc.party.cailiao.show_count)