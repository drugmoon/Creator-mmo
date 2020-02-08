module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>活动时间:</font><font color=#30ff00>%s</font>",
		"<font color=#b2a58b>活动期间玩家可以自由进入活动地图</font>",
	},
	strs2={
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b>1. 阁内共<font color=#30ff00>10层</font>，通过传送阵</font>",
		"<font color=#b2a58b>   前往下一层，传送层数<font color=#30ff00>随机</font></font>",
		"<font color=#b2a58b>2.<font color=#30ff00> 9层</font>可领取奖励，9层传</font>",
		"<font color=#b2a58b>   送员可传送到10层<font color=#30ff00>击杀BOSS</font></font>",
		"<font color=#b2a58b>3.<font color=#30ff00> 前9层</font>禁止PK，第10层自</font>",
		"<font color=#b2a58b>   由PK，被杀几率<font color=#30ff00>掉落装备</font></font>",
		"<font color=#b2a58b>4. 死亡后在第9层<font color=#30ff00>复活</font></font>",
		"<font color=#b2a58b>5. 原地复活消耗<font color=#30ff00>复活次数</font></font>",
	},
	resData={talkTitle="精英阁",btnName="进入精英阁",},
	func = "npc.jyg.btnEnter",
}
local ACTIVITY_NAME = '精英阁';

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	local openTime = gui.PanelActivityGuide.getNextOpenTimeString(ACTIVITY_NAME)
	bosssInfo.strs1[1] = "<font color=#b2a58b>活动时间:</font><font color=#30ff00>"..openTime.."</font>"
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo}));
end

function btnEnter(player)
	if server.get_temp_var("jygopen") ~="open" then
	-- if not gui.PanelActivityGuide.isActivityOpen(ACTIVITY_NAME) then
		return util.alert_msg(4, "活动未开始！", player)
	end
	local mapjy1 = server.find_map("jingying1")
	if mapjy1 then
		if player:get_level()>=gui.PanelActivityGuide.getActivityLevelLimit(ACTIVITY_NAME) then
			player:set_temp_param(999,0)
			player:enter_map(mapjy1:get_id(),21,31)
			gui.PanelGongXun.checkTaskComplete(player,15)
		else
			player:alert(11,1,"等级不足");
		end
	end
end

function btnExit(player)
	local result={}
	result.str = "即将退出副本，是否继续？"
	result.labelConfirm = "确定"
	result.labelCancel = "取消"
	result.callFunc = "npc.jyg.btnExitSure"
	result.book = ""
	player:push_lua_table("showConfirm",util.encode(result))
end

function btnExitSure(player)
	if player:is_dead() then
		player:relive(100)
	end
	player:go_home()
end
ckpanel.add_listener("npc.jyg.btnExit", npc.jyg.btnExit);
ckpanel.add_listener("npc.jyg.btnExitSure", npc.jyg.btnExitSure);
ckpanel.add_listener("npc.jyg.btnEnter", npc.jyg.btnEnter);

