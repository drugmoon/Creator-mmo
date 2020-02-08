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
		"<font color=#b2a58b>阴森但恢宏庞大的大殿中有数不尽的尸王，没有人知道它们为什么在这里...</font>",
	},
	strs2={
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b>1. 进入时间：<font color=#30ff00>15:00-15:30</font></font>",
		"<font color=#b2a58b>2. 每<font color=#30ff00>5分钟</font>刷新一波尸王、<font color=#30ff00>真尸王</font>，</font>",
		"<font color=#b2a58b>   击杀<font color=#30ff00>真尸王</font>掉落<font color=#30ff00>封印的技能书</font></font>",
		"<font color=#b2a58b>3. 持有<font color=#30ff00> 封印的技能书 3分钟</font> 即</font>",
		"<font color=#b2a58b>   可获得<font color=#30ff00> 解封的技能书</font></font>",
		"<font color=#b2a58b>4. <font color=#30ff00> 死亡不掉落</font>自身物品</font>",

	},
	resData={talkTitle="尸王殿",btnName = "进入活动",},
	func = "npc.shiwang.btnEnter",
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

function btnEnter(player)

	local clock = tonumber(os.date("%H%M",os.time()))
	if clock >= 1500 and clock <= 1530 then

	else
		server.set_temp_var("shiwangstate","close")
		return player:alert(11,1,"活动时间未到");
	end

	local x,y
	local map = server.find_map("shiwang")
	x,y=map:gen_rand_position()
	if server.get_temp_var("shiwangstate") == "open" then
		if player:get_level()>=60 then
			player:enter_map("shiwang",x,y)
			updataUIleftTips(player)
			gui.PanelGongXun.checkTaskComplete(player,14)
		else
			player:alert(11,1,"等级不足");
		end
	else
		player:alert(11,1,"活动时间未到");
	end
end
function onMonKill(player,pmap)
	if pmap then
		pmap:map_exe("npc.shiwang.updataUIleftTips")
	end
end

function updataUIleftTips(player)

	local award2={
				[1]={name="封印技能书",id=26040001,num=1,},
				--[2]={name="封印技能书",id=26040001,num=1,},			
			}
	local map = player:get_map()
	local result={}
	result.killmon=tonumber(server.get_var("shiwangTimes"))
	result.mon=6
	result.killboss=map:num_monster("尸王")
	result.boss=15
	result.title="尸王殿"
	
	result.time=30*60+tonumber(server.get_var("shiwangTime"))
	--print(result.time)
	result.award=award2
	player:push_lua_table("showshiwang",util.encode(result))
end

function  jiechufengyin(player)
	local num=player:num_item(26040001)
	if num>0 then
		for i=1,num do 
			player:remove_item(26040001,1);----随机给物品
			player:add_item(26010001, 1)		
		end 	
	end	
end

function back_home(player)
	local result={}
	result.str = "即将退出副本,是否继续?"
	result.labelConfirm = "确定"
	result.labelCancel = "取消"
	result.callFunc = "npc.shiwang.btnSure"
	result.book = ""
	player:push_lua_table("showConfirm",util.encode(result))
end


function btnSure(player)
	player:go_home()
end
ckpanel.add_listener("npc.shiwang.back_home", npc.shiwang.back_home);
ckpanel.add_listener("npc.shiwang.btnSure",npc.shiwang.btnSure)
ckpanel.add_listener("npc.shiwang.btnEnter", npc.shiwang.btnEnter);
