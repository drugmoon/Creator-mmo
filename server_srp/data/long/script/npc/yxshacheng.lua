module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end
local ACTIVITY_NAME = '夜袭比奇';

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b>月黑风高，黄沙漫漫，各方勇士为了各自的新年而齐聚比奇，大战一触即发</font>",
	},
	strs2={
		"<font color=#b2a58b>1. 进入条件：<font color=#30ff00> 60级</font>",
		"<font color=#b2a58b>2. 开放时间：<font color=#30ff00> 21:20-21:40</font></font>",
		"<font color=#b2a58b>3. 随机分配阵营，5分钟重新分配</font>",
		"<font color=#b2a58b>4. 获得<font color=#30ff00>个人积分</font>赢取奖励</font>",
		"<font color=#b2a58b>5. 达成<font color=#30ff00>参与5分钟、杀10人<font color=#b2a58b>或</font> </font>",
		"<font color=#b2a58b>   <font color=#30ff00> 被杀一次</font>后获得额外奖励</font>",
		"<font color=#b2a58b>6. <font color=#30ff00> 死亡不掉落</font>自身物品</font>",
	},
	resData={talkTitle="夜袭比奇城",btnName = "进入活动",},
	func = "npc.yxshacheng.btnEnter",
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
	local start = server.get_temp_var("yxbiqiState")
	-- if not gui.PanelActivityGuide.isActivityOpen(ACTIVITY_NAME) then
	if start~="start" then
		return util.alert_msg(4, "活动未开始！", player)
	end
	if player:get_level()<gui.PanelActivityGuide.getActivityLevelLimit(ACTIVITY_NAME) then
		player:alert(11,1,"等级不足!");
		return
	end
	
	local map = server.find_map("yxbiqi")
	local x,y = map:gen_rand_position()
	
	player:enter_map(map:get_id(),x,y)
end
ckpanel.add_listener("npc.yxshacheng.btnEnter", npc.yxshacheng.btnEnter);

function updataUIleftTips(player)
	local result={}
	result.btnAction = "npc.yxshacheng.btnExit"
	local openTime = tonumber(server.get_var("yxbiqiStartTime")) or os.time()
	result.second = gui.PanelActivityGuide.getActivityTotalSecond( ACTIVITY_NAME ) - os.time() + openTime
	result.serverTime = os.time()
	result.scoreChart = {}
	for i=1,10 do
		local scoreCharti = server.get_var("scoreChart"..i)
		scoreCharti = util.decode(scoreCharti)
		if type(scoreCharti) =="table" then
			table.insert(result.scoreChart,scoreCharti)
		end
	end
	result.score = util.ppn(player, const.PP_YXBIQI_SCORE)
	player:push_lua_table("showyxshacheng",util.encode(result))
end

function btnExit(player)
	local result={}
	result.str = "即将退出副本，是否继续？"
	result.labelConfirm = "确定"
	result.labelCancel = "取消"
	result.callFunc = "npc.yxshacheng.btnExitSure"
	result.book = ""
	player:push_lua_table("showConfirm",util.encode(result))
end
ckpanel.add_listener("npc.yxshacheng.btnExit", npc.yxshacheng.btnExit);

function btnExitSure(player)
	if player:is_dead() then
		player:relive(100)
	end
	player:go_home()
end
ckpanel.add_listener("npc.yxshacheng.btnExitSure", npc.yxshacheng.btnExitSure);


function acitvityCloseAlert(player)
	util.alert_msg(4, ACTIVITY_NAME.."活动已结束", player)
	-- if player:get_map():get_id() =="yxbiqi" then
	-- 	btnExitSure(player)
	-- end
end