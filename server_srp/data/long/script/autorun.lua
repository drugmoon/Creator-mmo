module(..., package.seeall)

function gxxx()  --每日零点会执行这个函数。 autorun.txt里面有配置。
	gui.AwardHall_sevenday.sendRankAwards() --七日狂欢充值排行邮件
	server.server_exe("genxxx.gx");   --这里使用server_exe方法，主要是为了能够使每个玩家都能清理一遍
	server.server_exe("task.task2000.check_richang_day");
	clearActivityVar()
	count.resetCount()
	-- PushStartDaysToServer()----每天推送一下当前的开区天数还有合服天数

	--扣除帮会维护资金
	-- server.server_gexe("gui.PanelGuild.reduceGuildAssets")
	npc.huangcheng.checkAutoStart()
	

end

function clearActivityVar()
	------------------以下是活动变量重置，保证未正常关闭的活动重置-----------------------
	server.set_var("KHcandidate", "")--
	-- server.set_temp_var("dbqb","");--夺宝奇兵
	-- server.set_temp_var("wlzb",0);--武林争霸
	server.set_temp_var("autoAddExp", "close")--城主膜拜
	-- server.set_var("kamuiPrison", "close")--神威狱

	----------------------------------------------
	-- print("00:01")
	server.set_var("shiwangTimes",0)--尸王波数
	server.set_var("shiwangTime",0)----活动倒计时
	server.set_temp_var("shiwangstate","close")----尸王
	server.set_temp_var("shengweistate","close")----神威狱
	server.set_var("shengweiTime","0")----神威狱活动倒计时
	server.set_temp_var("txdystate", "end")------天下第一
	server.set_var("TxdyTime","0")----天下第一倒计时
	server.set_var("dartstart",0) --押镖
	server.set_var("citybishi",0)--鄙视人数
	server.set_var("citymobai",0)--膜拜人数
	server.set_var("txdy","")---天下第一
	server.set_var("huanggongkill",0);--皇城雕像
	server.set_temp_var("jygopen","")

	server.set_var("warAward","0")

end

function noticewarprestart()
	npc.huangcheng.checkAutoStart()
	if npc.huangcheng.isWarOpenToday() then
		server.server_exe("map.kinghome.noticeWarPreStart")
	end
end

function checkwarstart()
	npc.huangcheng.checkAutoStart()
	if npc.huangcheng.isWarOpenToday() then
		server.set_temp_var("activityName","皇城战")
		server.start_war() --执行 war.onStart()
		server.set_var("KHcandidate", "")
		server.set_var("KHtimes",util.svn("KHtimes")+1)
	end
end
--8:30
function warEightHalfFunc()
	if server.is_war()>0 then
		local kc = server.find_map("kingcity");--皇城
		if kc then
			-- kc:set_timer(1,1000);--8:30分钟进入皇宫
			map.wangcheng.onTimer1(kc)
		end
	end
end
--8:35
function warEightHalfFiveFunc()
	if server.is_war()>0 then
		local km = server.find_map("kinghome")--开始5分钟检查一次，之后玩家进入或者退出检查一次
		if km then
			-- km:set_timer(1,1000)--8:35分钟首次检查占领行会
			map.kinghome.onTimer1(km)
		end
	end
end

function checkwarstop()
	if server.is_war() > 0 then
		server.stop_war() --执行 war.onStop()
	end
end

------------------------精英阁--------------------------
function jingyinggestart()
	server.set_temp_var("activityName","精英阁")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	server.info(10000,1,"精英阁活动开启！")
	server.set_temp_var("jygopen","open")
	server.set_var("jygStartTime",os.time())
	for i=1,10 do
		server.set_var("jyg"..i,"")
	end
	map.jyg.drawSendPoints()
	map.jyg.resetChart()
	local mapjy = server.find_map("jingying10")
	if mapjy then
		mapjy:clear_item()
		local boss = {name="魔王",id=11001002,num=1,x=20,y=30,range=5}
		mapjy:clear_mon_by_id(boss.id)
		mapjy:mon_gen(boss.x,boss.y,boss.id,boss.num,boss.range,1,1);--x,y,name,num,range,time,nolive,flags,tip,limitlv
		-- print("num_monster----------",mapjy:num_monster(11001002))
	end
end

function jingyinggeclose()
	map.jyg.resetChart()
	for i=1,10 do
		server.set_var("jyg"..i,"")
	end
	server.set_var("jygStartTime","")
	server.set_temp_var("jygopen","")
	server.info(10000,1,"精英阁活动关闭！")
	for i=1,10 do
		local mapjy = server.find_map("jingying"..i)
		if mapjy then
			mapjy:map_exe("go.home")
		end
	end
	map.jyg.removeSendPoints()
end
------------------天降赐福---------------------
function cifustart()
	server.set_temp_var("activityName","天降赐福")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	server.set_var("tjcfStartTime",os.time())
	server.set_temp_var("tjcfopen","open")
	local mapcifu = server.find_map("cifu")
	if mapcifu then
		mapcifu:set_timer(1,5*1000)--add exp
		mapcifu:set_timer(2,3*60*1000)--change double point
		map.cifu.changeDoubleArea(mapcifu)
	end
end

function cifuclose()

	server.set_var("tjcfStartTime","")
	server.set_temp_var("tjcfopen","")
	server.set_var("trap_t","")
	local mapcifu = server.find_map("cifu")
	if mapcifu then
		mapcifu:set_timer(1,0)--add exp
		mapcifu:set_timer(2,0)--change double point
		mapcifu:map_exe("go.home")
		map.cifu.removeTrap(mapcifu)
	end
	-- server.server_exe("npc.tjcf.acitvityCloseAlert")
	server.info(10000,1,"天降赐福活动已结束!")
end
--------------------夜袭比奇------------------
function yxbiqistart()
	server.set_temp_var("activityName","夜袭比奇")
	server.server_exe("gui.PanelActivityGuide.startNotice")

	server.set_temp_var("yxbiqiState","start")
	server.set_var("yxbiqiStartTime",os.time())
	local yxbiqimap = server.find_map("yxbiqi")
	if yxbiqimap then
		for i=1,10 do
			server.set_var("scoreChart"..i,"")
		end
		yxbiqimap:set_timer(1,5*60*1000)--changeCamp
		yxbiqimap:set_timer(2,10*1000)--生存奖励计时
		yxbiqimap:set_timer(3,1*1000)--刷怪
		yxbiqimap:set_timer(4,10*1000)--刷buff
		map.yxbiqi.genMonster()
		map.yxbiqi.resetChartList(yxbiqimap)
	end
end

function yxbiqiclose()
	server.set_temp_var("yxbiqiState","close")
	local yxbiqimap = server.find_map("yxbiqi")
	if yxbiqimap then
		yxbiqimap:set_timer(1,0)--add exp
		yxbiqimap:set_timer(2,0)--生存奖励计时
		yxbiqimap:set_timer(3,0)--刷怪
		yxbiqimap:set_timer(4,0)--刷buff
		yxbiqimap:clear_mon()
		yxbiqimap:map_exe("map.yxbiqi.showChartPanel")--最后一分钟看排行榜，只给出不给进
		map.yxbiqi.resetChartList(yxbiqimap)
	end
	server.server_exe("map.yxbiqi.sendAwardEmail")
	server.server_exe("npc.yxshacheng.acitvityCloseAlert")
end

-----翅膀祝福值每天6点清空
function wingvalue()
	server.server_exe("genxxx.set_bless");
end


----------尸王殿------------
function shiwangopen()
	server.set_temp_var("activityName","尸王殿")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	server.set_temp_var("shiwangstate", "open")
	server.set_var("shiwangTimes",0)--尸王波数
	server.set_var("shiwangTime",0)----活动倒计时
end
function shiwangclose()
	server.set_temp_var("shiwangstate", "close")
	local map = server.find_map("shiwang")

	if map then
		--map:map_exe("npc.shiwang.jiechufengyin")
		map:map_exe("go.home")--出地图掉落
	end

end
function shiwangupdata()
	local shiwangTime = server.get_var("shiwangTime")---时间
	if not shiwangTime or shiwangTime == "" or shiwangTime == "0" then
		server.set_var("shiwangTime",os.time())
	end

	local shiwangTimes = server.get_var("shiwangTimes")

	if not shiwangTimes or shiwangTimes == "" then
		shiwangTimes = 0
	end
	shiwangTimes = tonumber(shiwangTimes)
	server.set_var("shiwangTimes",shiwangTimes+1)

	local map = server.find_map("shiwang");
	local x,y
	x,y=map:gen_rand_position()
	if map then

		map:clear_mon()
		map:mon_gen(21,31,11003002,2,7,1,1,"no_owner");--真2（x，y，id，num，范围，1,1）
		map:mon_gen(21,31,11003001,13,7,1,1,"no_owner");--假13
		map:map_exe("npc.shiwang.updataUIleftTips")
	end
end

----------------活动期间刷BOSS-----
function refreshBoss()
	server.set_temp_var("activityName","超级瑞兽")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	local map = server.find_map("biqi");
	if map then
		if map:num_monster(11001001) == 0 then
			map:mon_gen(37,81,11001001,1,1,1,1,"no_owner");
			server.info(10000,0,"超级瑞兽出现在比奇(37,81)!大家赶紧前往击杀");
		end
	end
end

--庆典活动的超级瑞兽
function refreshActBoss()
	local map = server.find_map("biqi");
	if map and gui.PanelCelebrationAct.formattime(gui.PanelCelebrationAct.startYMD,get_day_YMD())<=2 then
		if map:num_monster(11001003) == 0 then
			map:mon_gen(37,81,11001003,1,1,1,1,"no_owner");
			server.info(10000,0,"庆典活动的超级瑞兽出现在比奇(37,81)!大家赶紧前往击杀");
		end
		return
	end
	if map and gui.PanelHeQuAct.mergeTabOpenState("超级瑞兽") then
		if map:num_monster(11001003) == 0 then
			map:mon_gen(37,81,11001003,1,1,1,1,"no_owner");
			server.info(10000,0,"合服活动的超级瑞兽出现在比奇(37,81)!大家赶紧前往击杀");
		end
	end
	if map and gui.PanelJingCaiAct.RuiShouOpenState() then
		if map:num_monster(11001003) == 0 then
			map:mon_gen(37,81,11001003,1,1,1,1,"no_owner");
			server.info(10000,0,"精彩活动的超级瑞兽出现在比奇(37,81)!大家赶紧前往击杀");
		end
	end
end

--半个小时后清除庆典活动的超级瑞兽
function clearRuiShou()
	local map = server.find_map("biqi");
	if map and gui.PanelCelebrationAct.formattime(gui.PanelCelebrationAct.startYMD,get_day_YMD())<=2 then
		if map:num_monster(11001003) > 0 then
			map:clear_mon_by_name("庆典瑞兽")
		end
	end
	if map and gui.PanelHeQuAct.mergeTabOpenState("超级瑞兽") then
		if map:num_monster(11001003) > 0 then
			map:clear_mon_by_name("庆典瑞兽")
		end
	end
	if map and gui.PanelJingCaiAct.RuiShouOpenState() then
		if map:num_monster(11001003) > 0 then
			map:clear_mon_by_name("庆典瑞兽")
		end
	end
end

function clearBoss()

	local map = server.find_map("biqi");
	if map then

		map:clear_mon_by_name("超级瑞兽")
	end
end
-----神威魔狱开启
function shengweiopen()
	server.set_temp_var("activityName","神威魔狱")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	--server.server_exe("npc.shengwei.txdyAward1");
	server.set_temp_var("shengweistate", "open")
	-- print("--shengwei-",server.get_temp_var("shengweistate"))

	server.info(10000,0,"神威魔狱活动开启");
	--local shengweiTime = server.get_var("shengweiTime")---时间
	--if not shengweiTime or shengweiTime == "" or shengweiTime == "0" then
		server.set_var("shengweiTime",os.time())
	--end
	npc.shengwei.mon_gen()
end
-- -----神威魔狱（map:clear_mon()）
-- function shengweiclose()
-- 	server.set_temp_var("shengweistate", "end")
-- 	server.info(10000,0,"神威魔狱活动入口关闭");
-- end

function shengweiend()
	server.set_temp_var("shengweistate", "close")
	server.set_var("shengweiTime","0")----活动倒计时
	-- print(server.get_temp_var("shengweistate"))

	local map = server.find_map("v201")
	server.server_exe("npc.shengwei.clear_arrive_param")
	for i=1,10 do
		local map = server.find_map("swmy"..i);
		if map then
			map:map_exe("go.home")
		end
	end
end

--天下第一--------------------
function tianxiadiyiready()
	server.set_temp_var("activityName","天下第一")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	local map = server.find_map("diyi");
	if map then
		map:set_flag_pkprohibit(1)----禁止pk
	end
	local TxdyTime = server.get_var("TxdyTime")---时间
	if not TxdyTime or TxdyTime == "" or TxdyTime == "0" then
		server.set_var("TxdyTime",os.time())
	end
	server.set_temp_var("txdystate", "ready")
	server.info(10000,0,"天下第一活动开启报名");
end

function tianxiadiyistart()
	server.set_temp_var("txdystate", "start")
	server.info(10000,0,"天下第一活动开启比赛");
	local map = server.find_map("diyi");
	if map then
		map:set_flag_pkprohibit(0);---可以pk
		map:set_flag_pkallow(1)--pk不红名
		map:map_exe("npc.txdy.checkTxdyplayer");
	end
end
function tianxiadiyiend()
	local map = server.find_map("diyi");
	if map then
		map:set_flag_pkprohibit(1)----禁止pk
		map:set_flag_pkallow(0)--pk红名
		--if server.get_var("txdy") =="nowiner" then --没有盟主，给普惠奖励
			map:map_exe("npc.txdy.Awardtxdy");
	--	end
	end
	server.server_exe("npc.txdy.txdyAward1");
	server.set_temp_var("txdystate", "end")
	server.info(10000,0,"天下第一活动结束");
end

-----全服全民押镖
function doubleDartopen()--全服双倍押镖
	server.set_temp_var("activityName","全民押镖")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	server.info(10010,0,"全民押镖已经开启!")
end

function doubleDartclose()--关闭全服双倍押镖
	server.info(10010,0,"全民押镖已经关闭!")
end
--全服双倍经验-活动期间多倍
function doubleExp()
	server.set_temp_var("activityName","双倍经验")
	server.set_temp_var("double_exp","open")
	-- server.server_exe("gui.PanelActivityGuide.startNotice")
	server.set_exp_mul(2)
	server.info(10010,0,"全服双倍经验已经开启!")
	--end
end

function doubleExp2()  --判断是否有双倍经验活动开启。
	if gui.PanelCelebrationAct.checkActIng() then
		server.set_temp_var("double_exp","open")
		server.set_exp_mul(2)
		server.info(10010,0,"庆典活动开启全服双倍经验!")
	end
	if gui.PanelHeQuAct.mergeTabOpenState("全服双倍") then
		server.set_temp_var("double_exp","open")
		server.set_exp_mul(2)
		server.info(10010,0,"合服活动开启全服双倍经验!")
	end
	if gui.PanelJingCaiAct.Exp2OpenState() then
		server.set_temp_var("double_exp","open")
		server.set_exp_mul(2)
		server.info(10010,0,"超值回馈开启全服双倍经验!")
	end
end
doubleExp2()

function singleExp()--关闭全服双倍经验
	if gui.PanelCelebrationAct.checkActIng() then
		return
	end
	if gui.PanelHeQuAct.mergeTabOpenState("全服双倍") then
		return
	end
	if gui.PanelJingCaiAct.Exp2OpenState() then
		return
	end
	server.set_temp_var("double_exp","close")
	server.set_exp_mul(1)
	server.info(10010,0,"全服双倍经验已经关闭!")
end

function checkPlayerInSafeArea(player)
	if util.isUserData(player) and player:get_safearea_id()==1 then
		player:set_timer(3,5000);
	end
end
--城主膜拜
function startMoBai()
	server.set_temp_var("activityName","膜拜城主")
	server.server_exe("gui.PanelActivityGuide.startNotice")
	server.set_temp_var("autoAddExp","open")
	server.set_var("worshipStartTime",os.time())
	server.server_exe("npc.statue.autoCheckTimes")
	server.set_var("citybishi",0)
	server.set_var("citymobai",0)
	local tucheng = server.find_map("tucheng")
	if tucheng then
		tucheng:map_exe("autorun.checkPlayerInSafeArea")
	end
end

function closeMoBai()
	server.set_temp_var("autoAddExp","close")
end

--每日10点准时给所有玩家发放俸禄
function sendFengLuToAll()
	server.server_exe("gui.PanelOfficial.sendFengLu")
end

function menpaiRankRecord()
	--结算未领取的奖励
	server.check_challenger_award()
	--记录排名
	server.record_challenger_rank()
end