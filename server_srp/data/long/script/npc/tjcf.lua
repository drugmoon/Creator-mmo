module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local Info =	{
	strs1={
		"<font color=#b2a58b>活动时间:</font><font color=#30ff00>17:30-18:30</font>",
		"<font color=#b2a58b>活动期间可自由进入活动地图</font>",
	},
	strs2={
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b>1. 在图中随机获得大量奖励</font>",
		"<font color=#b2a58b>2. 共8个<font color=#30ff00>双倍光圈</font>，<font color=#30ff00>3分钟</font>随机换位</font></font>",
		"<font color=#b2a58b>3. 自由PK,<font color=#30ff00>不掉装备</font></font>",
		"<font color=#b2a58b>4. 击杀玩家获得<font color=#30ff00>荣誉值</font><font color=#ff0000>(击杀</font>",
		"<font color=#b2a58b><font color=#ff0000>   低于自身等级10级以上的玩</font></font>",
		"<font color=#b2a58b><font color=#ff0000>   家时，不获得荣誉值）</font></font>",
		"<font color=#b2a58b>5. 死亡后在图中<font color=#30ff00>随机位置复活</font></font>",
	},
	resData={talkTitle="天降赐福",btnName = "进入活动",},
	func = "npc.tjcf.btnEnter",
}
local ACTIVITY_NAME = '天降赐福';

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	
	local openTime = gui.PanelActivityGuide.getNextOpenTimeString(ACTIVITY_NAME)
	Info.strs1[1] = "<font color=#b2a58b>活动时间:</font><font color=#30ff00>"..openTime.."</font>"
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = Info}));
end

function acitvityCloseAlert(player)
	util.alert_msg(4, "天降赐福活动已结束", player)
end

function btnGo(player,extend)
	posnpc.fly(player,4001018)
end
ckpanel.add_listener("npc.tjcf.btnGo", npc.tjcf.btnGo);

function btnEnter(player)
	if server.get_temp_var("tjcfopen")~="open" then
	-- if not gui.PanelActivityGuide.isActivityOpen(ACTIVITY_NAME) then
		return util.alert_msg(4, "活动未开始！", player)
	end

	local x,y
	local map = server.find_map("cifu")--天降赐福
	if map then
		x,y=map:gen_rand_position()
		
		if player:get_level()>=gui.PanelActivityGuide.getActivityLevelLimit(ACTIVITY_NAME) then
			
			local lastJoinTime = player:get_param(const.PP_TJCF_HONOR_JOIN_TIME)
			if lastJoinTime ~= server.get_var("tjcfStartTime") then
				player:set_param(const.PP_TJCF_HONOR_JOIN_TIME,server.get_var("tjcfStartTime"))
				player:set_param(const.PP_TJCF_HONOR_VALUE,0)
			end
			player:enter_map(map:get_id(),x,y)
			gui.PanelGongXun.checkTaskComplete(player,13)
		else
			player:alert(11,1,"等级不足");
		end
	end
end
ckpanel.add_listener("npc.tjcf.btnEnter", npc.tjcf.btnEnter);

function btnExit(player)
	go.home(player)
end
ckpanel.add_listener("npc.tjcf.btnExit", npc.tjcf.btnExit);