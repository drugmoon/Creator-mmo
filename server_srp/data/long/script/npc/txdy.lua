module(..., package.seeall)
util.use_function(_M,task.task1000);

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

local bosssInfo =	{
	strs1={
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b>慕名而来的勇士们，为了争夺天下第一的位置，一个个走上了充满杀气的擂台...</font>",

	},
	strs2={
		"<font color=#b2a58b></font>",
		"<font color=#b2a58b>1. 报名时间：<font color=#30ff00>12:40-12:50</font></font>",
		"<font color=#b2a58b>   比赛时间：<font color=#30ff00>12:50-13:20</font></font>",
		"<font color=#b2a58b>2. 报名期间<font color=#30ff00>自由进入，禁止pk</font></font>",
		"<font color=#b2a58b>3. 参与可获得大量<font color=#30ff00>绑定金币</font></font>",
		"<font color=#b2a58b>4. <font color=#30ff00> 唯一存活</font>者可获得元宝，</font>",
		"<font color=#b2a58b>   最后存活人数<font color=#30ff00>大于1</font>，则所</font>",
		"<font color=#b2a58b>   有人获得额外<font color=#30ff00>绑定金币</font></font>",
		"<font color=#b2a58b>5. <font color=#30ff00> 死亡不掉落</font>自身物品</font>",		
		
	},
	resData={talkTitle="天下第一",btnName = "参与天下第一",},
	func = "npc.txdy.btnEnter",
}

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="common",talk_str = bosssInfo}));
end

function btnEnter(player)
	if server.get_temp_var("txdystate") == "start" then
		player:alert(11,1,"活动已经开始，入口关闭");
		return
	end
	if server.get_temp_var("txdystate") == "end" then
		player:alert(11,1,"活动未开始");
		return
	end
	if server.get_temp_var("txdystate") == "ready" then		
		local x,y
		local map = server.find_map("diyi")
		x,y=map:gen_rand_position()
		
		if player:get_level()>=50 then
			local attackMode = player:get_attack_mode();
			player:set_param(const.PP_ATTACK_MODE, attackMode);
			--print(util.ppn(player,const.PP_ATTACK_MODE))

			player:enter_map("diyi",x,y)
			player:set_param(const.PP_TXDY_AWARD1,1)
			updataUIleftTips(player)
			gui.PanelGongXun.checkTaskComplete(player,17)
		else
			player:alert(11,1,"等级不足50");
		end
	end
end

function txdyAward1(player)
	if util.ppn(player, const.PP_TXDY_AWARD1) ==1  then
		local info="<font color='#EE9A00'>【亲爱的玩家】</font>" .. "<br>".."<font color='#b2a58b'>恭喜获得天下第一参与奖励</font>" .. "<br>"
		player:post_mail("天下第一", info,23060001,1,40000001,2000000)---金条 ，2000000exp
 		player:set_param(const.PP_TXDY_AWARD1,0)------活动结束后发放参与奖励
 	end
end 
function checkMapTime(player)
	local flag = false
	if  player:get_map():get_id() == "diyi" and (server.get_temp_var("txdystate") == "start"  or server.get_temp_var("txdystate") == "ready") then 
		flag=true
	end		
	return  flag
end
----1地图内有玩家死亡，2有人离开地图，3活动结束(当没有winer地图内所有玩家)
function checkTxdyplayer( player)
	local map = server.find_map("diyi")
	if map:num_player() == 1 then
		if player:get_map():get_id() == "diyi" then
			server.set_var("txdy",player:get_name())
			server.info(10,0,"恭喜<font color='#01ebff'>"..player:get_name().."</font>成为天下第一！获得元宝1000！")
			server.info(10000,0,"恭喜<font color='#01ebff'>"..player:get_name().."</font>成为天下第一！获得元宝1000！")
			local info="<font color='#EE9A00'>【亲爱的玩家】</font>" .. "<br>".."<font color='#b2a58b'>恭喜获得天下第一胜者奖励</font>" .. "<br>"
			player:post_mail("天下第一", info,40000005,1000)
			--player:set_param(const.PP_TXDY_AWARD1,0)
			player:set_timer(6,1000)--回城
		end
	elseif map:num_player() == 0  then
		server.info(10,0,"本场天下第一无人参加,活动提前结束")
	end

end
function Awardtxdy(player)
	if server.get_temp_var("txdystate") == "start" then
		local map = server.find_map("diyi")
		if map:num_player() == 1 then
			if player:get_map():get_id() == "diyi" then
				--if util.ppn(player,const.PP_WLZBRY) ~= today() and server.get_var("wlmz") =="" then
					--player:set_param(const.PP_WLZBRY,today())
					server.set_var("txdy",player:get_name())
					
					
					--npc.fenghao.youxifh(player)
				server.info(10,0,"恭喜<font color='#01ebff'>"..player:get_name().."</font>成为天下第一！获得元宝1000！")
				server.info(10000,0,"恭喜<font color='#01ebff'>"..player:get_name().."</font>成为天下第一！获得元宝1000！")
					local info="<font color='#EE9A00'>【亲爱的玩家】</font>" .. "<br>".."<font color='#b2a58b'>恭喜获得天下第一胜者奖励</font>" .. "<br>"
					player:post_mail("天下第一", info,40000005,1000)
					--player:set_param(const.PP_TXDY_AWARD1,0)
				--else
				--	player:alert(11,1,"一天只可成为一次武林盟主哦!")
			--	end
			end
		else
			player:alert(11,1,"很抱歉,本场天下第一没有决出天下第一,请明日再来吧!")
			local info="<font color='#EE9A00'>【亲爱的玩家】</font>" .. "<br>".."<font color='#b2a58b'>恭喜获得天下第一普惠奖励（无胜者）</font>" .. "<br>"
			player:post_mail("天下第一", info,23060001,1,40000001,2000000)
			player:set_param(const.PP_TXDY_AWARD1,0)
			server.set_var("txdy","nowiner")
		end
		player:set_timer(6,1000)--回城
	end
end			


function updataUIleftTips(player)
	local result={}
	result.txt="1.活动前10分钟为准备时间，准备时间内无法pk，但可自由出入\n2.12:50进入pk阶段，死亡或退出地图将失去胜利机会\n3.活动最后胜利者将获得1000元宝奖励"
	--result.time=(600-(os.time()-tonumber(server.get_var("TxdyTime"))))*1000---

	result.time=(600+tonumber(server.get_var("TxdyTime")))
	--print(result.time,tonumber(server.get_var("TxdyTime")))
	player:push_lua_table("showtxdy",util.encode(result))
end
function back_home(player)
	local result={}
	result.str = "即将退出活动,是否继续?"
	result.labelConfirm = "确定"
	result.labelCancel = "取消"
	result.callFunc = "npc.txdy.btnSure"
	result.book = ""
	player:push_lua_table("showConfirm",util.encode(result))
end


function btnSure(player)
	player:go_home()
end
ckpanel.add_listener("npc.txdy.back_home", npc.txdy.back_home);
ckpanel.add_listener("npc.txdy.btnSure",npc.txdy.btnSure)


ckpanel.add_listener("npc.txdy.btnEnter", npc.txdy.btnEnter);