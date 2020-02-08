module(..., package.seeall)

util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local btnHelfSize = 84 --按钮控件的宽的一半
local tempA = -120
local row = function(r) return 3+ (r-1)*175 + btnHelfSize  end
local line = function(i) return 640-60*i+tempA end

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),talk_str=npc:get_dialog(),npcName=npc:get_name()}));

end

function panelJump(player)
	-- player:enter_map("v002",75,92)
	-- player:find_road_goto("v002", 76, 91, "传送员",2);
end

function push_server_day(player)
	local severinfo = {}
	severinfo.dayNum = server_start_day()
	player:push_lua_table("server_start_day",util.encode(severinfo))
end

function btnCall(player,data)
	local nor = tonumber(data)
	if nor == 1 then
		-- if player:get_gender_name() == "male" then
		-- 	player:set_avatar(0,2002,2003)
		-- end
		-- if player:get_gender_name() == "female" then
		-- 	player:set_avatar(0,1002,1003)
		-- end
		player:add_item(11101011, 1,0)
		player:add_item(11103021, 1,0)
		player:add_item(11103091, 1,0)

		player:add_item(12102011, 1,0)
		player:add_item(12103021, 1,0)
		player:add_item(12103091, 1,0)

		player:add_item(13101011, 1,0)
		player:add_item(13106021, 1,0)
		player:add_item(13106091, 1,0)
	elseif nor==2 then
		-- player:set_avatar(0,0,0)
		player:add_item(11101011, 1,0)
		player:add_item(11102011, 1,0)
		player:add_item(12101011, 1,0)
		player:add_item(12102011, 1,0)
		player:add_item(13101011, 1,0)
		player:add_item(13102011, 1,0)
		player:add_item(13300000, 1,0)
	elseif nor==3 then
		player:enter_map("tucheng",50,50)
	elseif nor==4 then
		player:enter_map("biqi",50,50)
	elseif nor==5 then
		player:set_level(100)
	elseif nor==6 then
		player:enter_map("v200",50,50)
	elseif nor==7 then
		player:enter_map("v210",50,50)
	elseif nor==8 then
		-- player:enter_map("v220",33,39)
		gui.PanelOfficial.addsw(player,1e7)
	elseif nor==9 then
		player:enter_map("v001",110,173)
		--player:item_upgrade(-2,0,0,0,0,0)
		
	elseif nor==10 then
		local info="<font color='#EE9A00'>【亲爱的玩家】</font>" .. "<br>"
		.."<font color='#b2a58b'>这是一封测试邮件1</font>" ..os.time().. "<br>"
	
		player:post_mail("测试邮件", info,20001001,1)
	elseif nor==11 then
		player:set_param(const.PP_INNERPOWER_EXP,util.ppn(player, const.PP_INNERPOWER_EXP)+10000)
	elseif nor==12 then
		reset_server_start_day(1)
		push_server_day(player)
	elseif nor==13 then
		reset_server_start_day(8)
		push_server_day(player)
	elseif nor==14 then
		reset_server_start_day(15)
		push_server_day(player)
	elseif nor==15 then
		npc.biggm.onTalk100(nil,player)
	elseif nor==16 then

		player:set_wing_lv(70001)
	elseif nor==17 then
		gui.PanelFurnace.addFurnaceScore(player,1,1e6)
	elseif nor==18 then
		gui.PanelFurnace.addFurnaceScore(player,2,1e6)
	elseif nor==19 then
		gui.PanelFurnace.addFurnaceScore(player,3,1e6)
	elseif nor==20 then
		gui.PanelFurnace.addFurnaceScore(player,4,1e6)
	elseif nor==21 then
		for i = 25010001, 25010015 do
			player:add_item(i, 1,0)
		end
		for i = 25020001, 25020015 do
			player:add_item(i, 1,0)
		end
		for i = 25030001, 25030015 do
			player:add_item(i, 1,0)
		end
		for i = 25040001, 25040015 do
			player:add_item(i, 1,0)
		end
		for i = 25050001, 25050015 do
			player:add_item(i, 1,0)
		end
		for i = 25060001, 25060015 do
			player:add_item(i, 1,0)
		end
		for i = 25070001, 25070015 do
			player:add_item(i, 1,0)
		end
		for i = 25080001, 25080015 do
			player:add_item(i, 1,0)
		end
	elseif nor ==22 then
		onTalktestActivity(player)
	end

end
function get_day(t)
	return math.floor((t+8*60*60)/(60*60*24));
end
function today()
	return get_day(os.time());
end
ckpanel.add_listener("npc.mapmove.btnCall",npc.mapmove.btnCall)


function onTalktestActivity(player)
	local row = function(r) return 3+ (r-1)*175 + 84  end
	local line = function(i) return 420-70*i+30 end
	local talkInfos = {
	func = "npc.mapmove.btnActivity",scrollHeight = 140,title = {str="测试活动",fontSize = 24,color=0xFFFFFF,},
		con_tab = {
			{mtype="button",	name="夜袭比奇城",		funcIndex="yxbiqi",			res="btn_transmit-button",	posX=row(1),	posY=line(1),	},
			{mtype="button",	name="结束夜袭",		funcIndex="closeyxbiqi",	res="btn_transmit-button",	posX=row(2),	posY=line(1),	},
			{mtype="button",	name="皇城战",			funcIndex="kingwar",		res="btn_transmit-button",	posX=row(3),	posY=line(1),	},
			{mtype="button",	name="20:30开启皇宫",	funcIndex="openkinghome",	res="btn_transmit-button",	posX=row(4),	posY=line(1),	},
			{mtype="button",	name="20:35检查皇城",	funcIndex="checkkinghome",	res="btn_transmit-button",	posX=row(1),	posY=line(2),	},
			{mtype="button",	name="21:00结束皇城",	funcIndex="closekinghome",	res="btn_transmit-button",	posX=row(2),	posY=line(2),	},
			{mtype="button",	name="皇战公告",		funcIndex="noticekingwar",	res="btn_transmit-button",	posX=row(3),	posY=line(2),	},
			{mtype="button",	name="开始天降赐福",	funcIndex="startcifu",		res="btn_transmit-button",	posX=row(4),	posY=line(2),	},
			{mtype="button",	name="结束天降赐福",	funcIndex="closecifu",		res="btn_transmit-button",	posX=row(1),	posY=line(3),	},
			{mtype="button",	name="开始精英阁",		funcIndex="startjyg",		res="btn_transmit-button",	posX=row(2),	posY=line(3),	},
			{mtype="button",	name="结束精英阁",		funcIndex="closejyg",		res="btn_transmit-button",	posX=row(3),	posY=line(3),	},
			{mtype="button",	name="重置皇城帮会",	funcIndex="resetking",		res="btn_transmit-button",	posX=row(4),	posY=line(3),	},
		},
	}
	player:push_lua_table("talk_npc",util.encode(talkInfos))
end

function btnActivity(player,data)
	local time = 0
	if data=="yxbiqi" then
		autorun.yxbiqistart()
		time = 20*60
		server.set_timer(3,time*1000)
	elseif data =="resetking" then
		server.set_king_guild("")
		server.set_var("KHcandidate","")
	elseif data =="closeyxbiqi" then
		autorun.yxbiqiclose();
		server.set_timer(3,0);
	elseif data == "kingwar" then
		server.set_var("huanggongkill",0)--击杀标志
		server.set_var("huanggongopen",0)--30分钟
		autorun.checkwarstart()
		time = 60*60
		server.set_timer(3,time*1000)
	elseif data =="openkinghome" then
		if server.is_war()==1 then
			local km = server.find_map("kinghome")
			if km then
				km:set_timer(1,5*60*1000)
			end
			local kc = server.find_map("kingcity")
			if kc then
				kc:set_timer(1,1000)
			end
		else
			util.alert_msg(4, "未开启皇城战", player)
		end
	elseif data =="closekinghome" then
		if server.is_war()==1 then
			autorun.checkwarstop()
		else
			util.alert_msg(4, "未开启皇城战", player)
		end
	elseif data =="checkkinghome" then
		local km = server.find_map("kinghome")
		if km then
			km:set_timer(1,1000)
			-- map.kinghome.onTimer1(km)
		end
	elseif data=="noticekingwar" then
		autorun.noticewarprestart()
	elseif data=="startcifu" then
		autorun.cifustart()
		time = 20*60
	elseif data=="closecifu" then
		autorun.cifuclose()
	elseif data=="startjyg" then
		autorun.jingyinggestart()
		time = 30*60
	elseif data=="closejyg" then
		autorun.jingyinggeclose()
	else
		player:alert(1,0,"<pic src=\'img_i_have_no_idea\'/>对不起我不会    ")		
	end
	-- print("testActivity----",data)
	server.set_var("testActivity",data)
	if time>0 then
		server.set_var("test_"..data,time+os.time())
	end
end

ckpanel.add_listener("npc.mapmove.btnActivity",npc.mapmove.btnActivity)

function checkActivityClose()
	local activity = server.get_var("testActivity")
	-- print("activity close----"..activity)
	if util.svn("test_yxbiqi")>0 and util.svn("test_yxbiqi")<=os.time() then
		autorun.yxbiqiclose()
		server.set_var("test_yxbiqi","")
	elseif util.svn("test_kingwar")>0 and util.svn("test_kingwar")<=os.time() then
		autorun.checkwarstop()
		server.set_var("test_kingwar","")
	end
	server.set_var("testActivity","")
end