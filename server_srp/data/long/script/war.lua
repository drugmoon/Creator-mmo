module(..., package.seeall)
function onStart()
	server.info(10010,0,"皇城战已经开始，请广大玩家前往沙巴克参与！")
	server.set_king_guild("") -- 清除王城帮会
	server.set_var("KHcandidate","")
	map.kinghome.recordKingGuildInfo()--删除雕像的帮会信息
	server.server_exe("map.kinghome.showPrefix")
	server.set_var("huanggongkill",0)
	server.set_var("huanggongopen",0)
	server.set_var("warAward",0)

	local kc = server.find_map("kingcity");--皇城
	if kc then
		kc:set_flag_pkallow(1);
		kc:set_flag_autoalive(0);
		kc:set_auto_enter(0);
		-- kc:set_timer(1,30*60*1000);--30分钟后进入皇宫
		map.wangcheng.checkStatue(kc)--切换雕像
		kc:map_exe("map.wangcheng.changeModeToGuild")--切换已在地图上玩家的攻击模式
	end
	local km = server.find_map("kinghome")--开始5分钟检查一次，之后玩家进入或者退出检查一次
	if km then
		-- km:set_timer(1,35*60*1000)--5分钟后首次检查占领行会
		km:set_flag_pkallow(1);
		km:set_flag_autoalive(0);
		km:set_var("check5min","")
	end
	server.server_exe("map.kinghome.noticeWarStart")
end

function onStop()
	server.set_var("kingCityOpenTime",0)
	local kc = server.find_map("kingcity");
	if kc then
		kc:set_flag_autoalive(0);
		server.set_var("huanggongopen",0)--标记皇宫进入标志
		-- kc:set_timer(1,0)
		map.wangcheng.checkStatue(kc)--切换雕像
	end
	local km = server.find_map("kinghome");
	if km then
		km:set_auto_enter(0);
		km:set_flag_autoalive(0);
		-- km:set_timer(1,0);
		map.kinghome.checkResult(km)
		km:map_exe("go.home")
	end
	server.server_exe("map.kinghome.hideWarNotice")
	server.server_exe("gui.PanelHeQuAct.sendMailSczb")
end

function onTimer1()

end

--活动消息提示，不定长计时器
function onTimer2()
	local second = gui.PanelActivityGuide.checkPreStartActivity()
	-- print("next Activity second is", second)
	if second <0 then second = 60 end --返回值错误下一分钟重新计算
	server.set_timer(2,second*1000)
end

--测试活动计时器
function onTimer3()
	npc.mapmove.checkActivityClose()
end