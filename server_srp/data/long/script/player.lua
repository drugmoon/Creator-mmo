module(..., package.seeall)
OncientUse(_M,gm.gm)

--此文件主要是由c++中玩家各种信息变化时，主动触发调用的lua方法。

local skill_table = {
	["warrior"]={
				{skill_id=101,	skill_name="基础剑法",},
				{skill_id=103,	skill_name="穿刺剑法",},
				{skill_id=104,	skill_name="弧月斩",},
				{skill_id=105,	skill_name="战神冲撞",},
				{skill_id=106,	skill_name="烈焰剑法",},
				{skill_id=109,	skill_name="逐日剑法",},
	},
	["wizard"]={
				{skill_id=402,	skill_name="抗拒火环",},
				{skill_id=405,	skill_name="雷电术",},
				{skill_id=409,	skill_name="火墙术",},
				{skill_id=411,	skill_name="地狱雷光",},
				{skill_id=412,	skill_name="魔法盾",},
				{skill_id=414,	skill_name="冰咆哮",},
	},
	["taoist"]={
				{skill_id=502,	skill_name="精神力战法",},
				{skill_id=503,	skill_name="群体腐蚀术",},
				{skill_id=504,	skill_name="圣灵符法",},
				{skill_id=507,	skill_name="群体隐遁术",},
				{skill_id=508,	skill_name="幽灵魔甲术",},
				{skill_id=512,	skill_name="群体治疗术",},
				{skill_id=513,	skill_name="召唤虎卫",},
	},
};

function onLoad(player)
    player:alert(110,0,"欢迎来到"..server.get_server_name().."!")
	player:alert(10,0,"健康游戏忠告:<br>抵制不良游戏 拒绝盗版游戏<br>注意自我保护 谨防受骗上当<br>适度游戏益脑 沉迷游戏伤身<br>合理安排时间 享受健康生活")

	genxxx.gx(player)
	genxxx.set_bless(player)
	if( player:get_task_state(1) == 0 ) then
		player:set_task_state(1,1)
		login.first_login(player)
		player:add_skill(100)
		-- player:set_param(const.PP_HUICHENG,50)
		-- player:set_param(const.PP_RANDMOVE,50)

		local job = player:get_job_name();
		if job == "warrior" then  --战士
			player:add_skill(101);
		elseif job == "wizard" then --法师
			player:add_skill(405);
			player:set_short_cut(1, 2, 405);
		elseif job == "taoist" then --道士
			player:add_skill(504);
			player:set_short_cut(1, 2, 504);
		end
	end
	-- player:set_max_power(0,0,0)
	-- player:set_param(const.PP_NEW_ZUOQI_JIE_LEVEL,1)
	-- player:set_param(const.PP_NEW_ZUOQI_XING_LEVEL,1)

	gui.PanelInnerPower.setInnerPower(player, 0)
	player:set_model(11,util.ppn(player,const.PP_WING_LEVEL))
	-- player:set_model(5,util.ppn(player,const.PP_VIP_LEVEL))
	-- if util.ppn(player,const.PP_WING_LEVEL)>0 then
	-- 	player:set_wing_lv(1000+util.ppn(player,const.PP_WING_LEVEL))
	-- end
	login.enter_game(player)
	-- task.task2000.check_richang_day(player)
	-- player:set_param(const.PP_HUICHENG,util.ppn(player,const.PP_HUICHENG))
	-- player:set_param(const.PP_RANDMOVE,util.ppn(player,const.PP_RANDMOVE))
	if server.get_temp_var("isActivityNotice") ~= "true" then
		server.set_timer(2,3000)
		-- print("start activity notice")
		server.set_temp_var("isActivityNotice","true")
	end

	local dart_id = util.ppn(player,const.PP_DART_ID)
	if dart_id > 0 then
		if not player:bind_dart(dart_id) then
			player:set_param(const.PP_DART_ID,0)
		end
	end
end

function onUnload(player)
	login.unload_player(player)
end

function onEnterGame(player)
	enter_game_triger_fun(player);-------进游戏触发的一些方法汇总
	local attackMode = util.ppn(player, const.PP_ATTACK_MODE);
	if attackMode == 0 then attackMode = 101; end
	player:set_attack_mode(attackMode);
	player:set_param(const.PP_ATTACK_MODE,attackMode)

	if util.ppn(player,const.PP_WING_LEVEL) > 0 then
		player:set_wing_lv(70001+util.ppn(player,const.PP_WING_LEVEL));-----翅膀
	end
	if player:get_name() == server.get_var("wlmz") then
		--server.info(10,0,"<font color='#ff00ff'>尊敬的武林盟主[<font color='#ff0000'>"..player:get_name().."</font>]上线了!</font>");
		-- npc.fenghao.youxifh(player);
	else
		-- npc.fenghao.youxifh(player);
	end
	local startTime = util.ppn(player, const.PP_OFFLINE_START_TIME)
	local second = player:get_last_login_time()-player:get_last_logout_time()
	local mapSelect = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if mapSelect>0 then
		local MAX_TIME = 7*24*60*60
		player:set_param(const.PP_OFFLINE_START_TIME,0)
		second = second + player:get_last_logout_time() - startTime
		second = util.bound(0,second,MAX_TIME);
		player:set_param(const.PP_OFFLINE_TIME,util.ppn(player,const.PP_OFFLINE_TIME)+second)
		player:set_param(const.PP_OFFLINE_STATE,0) --挂机结束
		gui.PanelOffline.CalculateAward(player)
	end

	local kingInfo = server.get_var("kingInfo")
	local king_tab = util.decode(kingInfo) or {}
	local king_guild = server.get_king_guild()
	local leaderOfKing = ""
	if king_guild and king_guild ~= "" then
		local guild_temp = server.find_guild(king_guild)
		if guild_temp then
			leaderOfKing = guild_temp:get_leader_name()
			if  player:get_name()==leaderOfKing then
				if tonumber(os.date("%H%M", os.time())) <=2030 or tonumber(os.date("%H%M", os.time()))>=2100  then
					server.info(10000,0,"<font color='#ff0000'>传奇主宰者</font>  <font color='#01ebff'>"..leaderOfKing.."</font>  上线了！")
				end
			end
		end
	end
	if king_guild and king_guild ~= "" and not king_tab.guild then
		king_tab.name = leaderOfKing
		king_tab.guild = king_guild
		server.set_var("kingInfo",util.encode(king_tab))
	end
	if leaderOfKing~="" and player:get_name() == leaderOfKing and not king_tab.job then
		king_tab.job = player:get_job_name()
		king_tab.gender = player:get_gender_name()
		king_tab.fight = player:get_fightpoint()
		king_tab.level = player:get_level()
		king_tab.weapon = player:get_item_type_by_pos(const.ITEM_WEAPON_POSITION)
		king_tab.cloth = player:get_item_type_by_pos(const.ITEM_CLOTH_POSITION)
		server.set_var("kingInfo",util.encode(king_tab))
	end

	if util.ppn(player,const.PP_CHIP_SJ)>=100 then
		gui.PanelShenJiang.enterZhaoHuan(player)
	end

	-- player:set_timer(9, 120 * 1000)

	-- player:set_timer(13,1*1000)

	player:set_timer(15,10*1000)

	player:set_timer(16,60*1000)

	tab.pushItemLimitToClient(player)
end

function onLeaveGame( player )
	local pmap = player:get_map()
	if pmap then
		local mapId = pmap:get_id()
		player:set_param(const.PP_OFFLINE_MAPID,mapId)

		if mapId == "kinghome" then
			pmap:set_timer(3,300)--死皇宫
		elseif mapId =="yxbiqi" then
			if server.get_temp_var("yxbiqiState") == "start" then
				map.yxbiqi.addScroe(player,"leave")
			end
		end
	end

	-- 记录镖车ID
	player:set_param(const.PP_DART_ID,player:get_dart_id())

	local mapSelect = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if util.ppn(player, const.PP_OFFLINE_STATE) ==0 and mapSelect>0 then
		player:set_param(const.PP_OFFLINE_STATE,1)
		if util.ppn(player, const.PP_OFFLINE_START_TIME) ==0 then
			player:set_param(const.PP_OFFLINE_START_TIME,os.time())
		end
		gui.PanelOffline.recordBuffInfo(player)
	end

	local attackMode = player:get_attack_mode();
	if attackMode == 105 then attackMode = 100; end
	player:set_param(const.PP_ATTACK_MODE, attackMode);

	login.leave_game(player)
end

function onStartCaiji(mon,player)
	ckmon.triger_caijistart(player,mon)
end

function onCaiji(mon,player)
	ckmon.triger_caiji(player,mon)
end

function onReliveAfter(player,stand)
	---暂时屏蔽复仇buff
	-- local mapid=player:get_map():get_id()
	-- if string.sub(mapid,1,4) == "zssd" then
	-- 	npc.zhuanshen.onrelive(player)
	-- end
	-- else
	local pmap = player:get_map()
	if pmap then
		if pmap:get_id() =="yxbiqi" or util.ptpn(player, 888)=="ctg" then
			player:setHpMpPercent(100)
			player:set_temp_param(888,0)
		end
	end
	if stand == 0 then
		player:setHpMpPercent(70)
	end
	player:set_wing_lv(70001+util.ppn(player,const.PP_WING_LEVEL));
end

function onLevelUp(player)
	cklevel.triger_level(player,player:get_level())

	-- gui.moduleLevelUp.handleLevelUp(player)

	-- gui.AwardSevenDayRank.checkFunc(player, "Level")--首周冲榜

	-- gui.PanelEquipPreview.onPlayerLevelUp(player)--
	--gui.PanelWing.CalculateAttributeWing(player)----翅膀
	local level = player:get_level();
	if  level>=58 and  util.ppn(player,const.PP_GUILD_BOX_FLAG) < 1 then
		--player:add_item(27000003,1,1)	----帮会宝箱
		local info="<font color='#EE9A00'>亲爱的玩家：</font>" .. "<br>".."<font color='#b2a58b'>一个篱笆三个桩，一个好汉三个帮</font>" .. "<br>".."<font color='#b2a58b'>争霸大陆的战斗才刚开始！</font>" .. "<br>".."<font color='#b2a58b'>加入帮会大家庭，让我们一起征战大陆！</font>" .. "<br>"
		player:post_mail("帮会福利", info,27000003,1)
		player:set_param(const.PP_GUILD_BOX_FLAG,1);
		--util.alert_msg(4, "恭喜获得帮会宝箱*1", player)
	end
	if  level>=65 and  util.ppn(player,const.PP_FANLI_BOX_FLAG) < 1 then

		local info="<font color='#EE9A00'>亲爱的玩家：</font>" .. "<br>".."<font color='#b2a58b'>只需1元宝，即可升级龙心，获得2000倍返利！</font>" .. "<br>".."<font color='#b2a58b'>战神附体，攻击爆升！</font>"
		player:post_mail("超级返利宝箱", info,27000004,1)
		player:set_param(const.PP_FANLI_BOX_FLAG,1);
		--util.alert_msg(4, "恭喜获得帮会宝箱*1", player)
	end
	local mail_lv = math.floor(level / 10);
	if mail_lv >= 1 then
		for i=1,mail_lv do
			if util.ppn(player,const.PP_LEVELUP_MAIL_NUM) < i then
				player:set_param(const.PP_LEVELUP_MAIL_NUM,i);
				local lv = i*10;
				--player:post_mail("升级福利","恭喜您升级到"..lv.."级获得大量奖励!\n 80级可以转生，即可获得更高级的奖励\n 请领取邮件奖励！继续加油哦\n 提示：除了日常任务以外，定时活动和未知暗殿都是升级的好去处哦。",10104,2,10105,1,19009,300,10007,5);
			end
		end
	end
	gui.AwardHall_jingji.setRankLevel(player)
end

--转生回调
function onZsLevelUp(player)
	ckzslevel.triger_zslevel(player, util.ppn(player, const.PP_ZHSH_LEVEL))
end

local mapidTable = {"taskad","taskad2"}
function onDropAlert(player,target,item_str)
	local pmap = player:get_map()
	if pmap then
		local mapname = pmap:get_name()
		local item_tab = util.splitString(item_str,"_")
		-- local tempStr = "";
		-- local colorName = "";
		local mapid = pmap:get_id()
		if string.sub(mapid,1,4) == "swmy" then
			mapname="神威魔狱"..mapname
		end
		---------------------不需要提示的地图-------------------------
		if table.indexof(mapidTable, mapid) then return end
		if #item_tab > 0 then
			for i=1,#item_tab do
				-- colorName = tab.getItemColorName(item_tab[i]);
				-- tempStr = tempStr.."["..colorName.."]";
				-- if colorName ~= "" then
					server.info(10,0,"<font color='#1eb8ff'>"..mapname.."</font>的<font color='#ff0000'>"..string.gsub(target,"(.-)(%d-)(%(Lv:%d*%))","%1").."</font>被消灭,掉落<p>##"..item_tab[i].."##</p>")
				-- end
			end
		end
	end
end

local mainTaskMons = {
	12001002, 12002003, 12004003,12004004
};

function onKill(mon1,player)
	local h = tonumber(os.date("%H%M",os.time()));
	ckmon.triger_mon(player,mon1,false)
	local pmap = player:get_map()
	if pmap then
		local mapid = pmap:get_id()
		-- if string.sub(mapid,1,3) == "xmt" then
		-- 	npc.xinmota.onMonKill(player)
		-- elseif string.sub(mapid,1,4) == "fbgd" then
		-- 	gui.PanelGuild.onMonKill(player)
		if string.sub(mapid,2,5) == "clfb" then
			local index=tonumber(string.sub(mapid,1,1))
			npc.party.cailiao.onMonKill(player,index)
		elseif string.sub(mapid,1,4) == "fbgr" then
			gui.PanelBoss.onKillMon(player,pmap)
		elseif string.sub(mapid,1,6) == "weizhi" then--暗殿暗殿
			mon.bossRefresh.setBossInfo(mon1,pmap,player)
		elseif string.sub(mapid,1,7) == "taskad2" then--暗殿暗殿
			mon.bossRefresh.setBossInfo(mon1,pmap,player)
		elseif mapid == "v206" or mapid == "v207" then--boss之家

		elseif string.sub(mapid,1,4) == "fbzx" then --主线未知暗殿
			map.raid_main_task4.onMonKill(player, mon1)
		elseif string.sub(mapid,1,3) == "cbt" then
			npc.cangbaonpc.onMonKill(player, mon1)
		elseif string.sub(mapid,1,3) == "ctg" then--闯天关
			gui.PanelBreakUp.onMonKill(player, mon1)
		elseif string.sub(mapid,1,7) == "shiwang" then--尸王
			npc.shiwang.onMonKill(player, pmap)
		elseif string.sub(mapid,1,4) == "swmy" then--击杀boss后当前地图player刷新
			local index=tonumber(string.sub(mapid,5,6))
			npc.shengwei.onMonKill(player,pmap)
		elseif string.sub(mapid,1,7) == "shaozhu" then
			map.shaozhu.onMonKill(player, mon1)
		elseif mapid == 'yxbiqi' then
			map.yxbiqi.addScroe(player,"killMon")
		end

		--野外BOSS 个人BOSS BOSS之家 玛雅神殿
		-- 0126加入主线boss(mainTaskMons)
		local monId = mon1:get_def_id();
		if (monId>=3001001 and monId<=6011068) or table.indexof(mainTaskMons, monId) then
			gui.AwardHall_boss.killMon(player,mon1:get_name())
		end
	end
	if player:have_slave(10) > 0 then
		local exp_award = mon1:get_exp_award()
		local expState = server.get_temp_var("double_exp")
		if expState=="open" then--双倍经验时护卫经验也双倍
			exp_award=exp_award*2
		end
		gui.PanelHuWei.addHuweiExp(player, exp_award)
	end

	local exp = mon1:get_power_exp()
	if exp and exp>0 then
		gui.PanelInnerPower.addInnerPower(player, exp)
	end
	-- if mon1:get_name()=="白龙" then
	-- 	gui.AwardHeQu.checkMonKilled(player, mon1)
	-- end

	-- gui.AwardNationalBoss.onKillBoss(player, mon1)

	--gui.AwardExercise.killBossNum(player,mon1)
	-- map.tucheng.checkMonKilled(player, mon1)
	if mon1:isboss()==1 then
		-- tab.Boss.onBossDead(mon1,player)
		gui.PanelGongXun.checkTaskComplete(player,5)
	end
	if util.ppn(player,const.PP_CAILIAO_INDEX)>0 and util.ppn(player,const.PP_CAILIAO_INDEX)<11 then
		npc.cailiao.cailiao_task_mon(mon1,player)
	end

	gui.PanelGongXun.checkTaskComplete(player,2)

	player:set_param(const.PP_DAY_KILL_NUM,util.ppn(player, const.PP_DAY_KILL_NUM)+1)
end

function onKillAssist(player, deadPlayer)
	if player:get_map():get_id() =="yxbiqi" then
		map.yxbiqi.addScroe(player, "assist")
	end
end

function onKillGroup(mon1,player)
	ckmon.triger_mon(player,mon1,true)
end

function onTimer1(player) go.timer(player) end

function onTimer2(player)
	-- gui.PanelBreakUp.goToPrevious(player)
end

function onTimer3(player)
	if server.get_temp_var("autoAddExp") ~= "open" then return end
	--在土城安全区
	if player:get_safearea_id() == 1 then
		local lv = player:get_level()
		local exp,zhanshenexp
		if lv>=50 and lv<70 then
			exp,zhanshenexp = 10000,10000
		elseif lv>=70 and lv<86 then
			exp,zhanshenexp = 20000,20000
		elseif lv>=86 and lv<91 then
			exp,zhanshenexp = 30000,30000
		elseif lv>=91 and lv<=999 then
			exp,zhanshenexp = 50000,50000
		end
		if exp and zhanshenexp then
			player:add_exp(exp)
			gui.PanelHuWei.addHuweiExp(player, zhanshenexp)
			player:set_timer(3,5*1000);
			util.alert_msg(4, "城主膜拜获得"..exp.."经验,"..zhanshenexp.."护卫经验", player)
		end
	end
end

function onTimer4(player)--擂台定时器
	-- if npc.arenaWar.giveArenaWarGift then npc.arenaWar.giveArenaWarGift(player) end
end

function onTimer6(player)
	player:go_home()
	if player:is_dead() then
		-- player:set_timer(5,0)
		player:relive(100)
		return
	end
end

function onTimer7(player)
	if util.svn("maponitemopen") == 0 then return end
	local sy_time = 180 + util.svn("mapitemtime") - tonumber(os.time());
	if sy_time > 0 then------3分钟的等待
		local num_min = math.floor(sy_time / 60);
		local num_sec = math.floor(sy_time % 60);
		player:alert(1,1,"剩余"..num_min.."分"..num_sec.."秒可以捡取");
		player:set_timer(7,10*1000);
	else
		player:set_timer(7,0);
	end
end

function onTimer8(player)
	player:go_home()
	player:alert(1,1,"累计超过3小时自动传出！");
end

-- function onTimer9(player)--夺宝历练定时器
-- 	if gui.AwardExercise.refreshAward then gui.AwardExercise.refreshAward(player) end
-- end

function onTimer10(player)--藏宝图挖宝变装定时器
	player:Become_Monster(0, "")
end

function onTimer11(player)--藏宝图自动下一个任务
	-- if util.ppn(player, const.PP_CANGBAOTU_TASK_NUM)<20 then
	-- 	npc.cangbaonpc.panelJump(player)
	-- end
end

function onTimer12(player)
	player:go_home()
	player:alert(1,1,"累计超过2小时自动传出！");
end

--记录背包开格子倒计时
function onTimer13(player)
	local totalTime = util.ppn(player,const.PP_BAG_GE_COUNT)
	local needTime = gui.PanelBag.getCurCellCountTime(player)--取这个格子开放需要多少秒
	if needTime>0 then
		if totalTime>=needTime then
			gui.PanelBag.startOpenGe(player)
		else
			player:set_param(const.PP_BAG_GE_COUNT,totalTime+1)
		end
		player:set_timer(13,1*1000)
	end
end

--护卫增加次数在（记录当天在线时间）
function onTimer15(player)
	local totalTime = util.ppn(player,const.PP_HUWEI_RUNTIME)
	player:set_param(const.PP_HUWEI_RUNTIME,totalTime+10)
	local oldNum = (totalTime-totalTime%1200)/1200
	local newNum = ((totalTime+10)-(totalTime+10)%1200)/1200
	if oldNum~=newNum then
		--刷新客户端显示的召唤次数
		gui.PanelHuWei.updateEnableTimes(player)
	end
	if 10+newNum-util.ppn(player,const.PP_HUWEI_USENUM)<12 then
		player:set_timer(15,10*1000)
	end
end

--每分钟记录一次累计在线奖励
function onTimer16(player)
	gui.AwardHall_online.updateWeekOnline(player)
	player:set_timer(16,60*60*1000)
end

------------烧猪定时器-------------
function onTimer17(player)
	map.shaozhu.genMonNextWave(player)
end

function onTimer18(player)
	posnpc.fly(player, 4002021)
end

--护卫召唤冷却
function onTimer19(player)
	local CD = util.ppn(player,const.PP_HUWEI_CALL_CD)-1
	player:set_param(const.PP_HUWEI_CALL_CD,CD)
	if CD>0 then
		player:set_timer(19,1*1000)
	end
end
-----------------------------------
--死亡复活倒计时
function onTimer20(player)
	if player:is_dead() then
		gui.PanelRelive.relive_back(player)
		player:push_lua_table("close",util.encode({name = "panel_relive"}));
	end
end

--自动押镖定时器
function onTimer21(player)
	gui.PanelDart.autoDart(player);
end

--合服活动 (神秘商店)
function onTimer22(player)
	-- print("time   done")
	gui.PanelMysteryStore.refreshStore(player);
end

--护卫回血
function onTimer23(player)
	gui.PanelHuWei.setHuiXue(player)
end

--守护女神定时器
function onTimer24(player)
	map.defend.onTimerOut(player);
end

--后3个参数分别为 增加的，拥有的，总充值(单位都是元宝)
function onCongzi(player,add,vcoin,accu)
	gui.PanelVip.upgradeVipLevel( player, accu )
	-- gui.AwardSevenDayRank.checkFunc(player, "VIP")--首周冲榜
	if add > 0 then
		if add == 600 then
			player:set_param(const.PP_CHARGE6_LIMIT,1)
		end
		gui.PanelCharge.freshPanel(player)
		gui.PanelCharge.onPlayCz(player, add)  --触发客户端播放充值动画
		-- local ewnum = math.floor(add/10);
		-- if ewnum > 500 then
		-- 	player:add_vcoin_best_enable(ewnum,111);----111表示充值额外赠送10%的元宝
		-- 	player:alert(11,1,"额外赠送元宝:"..ewnum);
		-- end

		-- if tonumber(server_start_day()) <= 6 then------开区7天充值排名
		-- 	player:set_param(const.PP_KQ7T_RECHARGE_NUM,util.ppn(player,const.PP_KQ7T_RECHARGE_NUM)+add);
		-- 	gui.AwardRecharge7DRank.Recharge_kq7d(player,util.ppn(player,const.PP_KQ7T_RECHARGE_NUM));
		-- end

		-- if server_merge_day()>=0 and server_merge_day()<7 then
		-- 	player:set_param(const.PP_HEQU_RECHARGE_TOTAL,util.ppn(player,const.PP_HEQU_RECHARGE_TOTAL)+add);
		-- end
		-- if get_day_YMD() >= 20160618 and get_day_YMD() <= 20160622 then--夏至充值返利
		-- 	-- player:set_param(const.PP_JUNE_JIFEN,util.ppn(player,const.PP_JUNE_JIFEN)+math.floor(add*0.01));--六月充值积分
		-- 	if add>5000 then
		-- 		player:add_vcoin_best_enable(math.floor(add*0.4),111);----夏至充值返利额外赠送40%的元宝
		-- 		player:alert(11,1,"活动期间额外赠送元宝:"..math.floor(add*0.4));
		-- 	end
		-- end
		gui.PanelJingCaiAct.checkjscz(player,add)
		gui.PanelJingCaiAct.checkljcz(player,add)
		gui.PanelJingCaiAct.checkczph(player,add) --精彩活动充值需要刷新下数据
		gui.AwardHall_sevenday.checkSevenDay(player,add)
	end
	player:set_param(const.PP_SUM_RECHARGE, accu)
	player:set_model(19,accu);--充值总额

	--充值时候检测刷新

	if util.ppn(player,const.PP_RCHARGE_FLAG)<=0 then
		local startDay = server_start_day()+1
		player:set_param(const.PP_RCHARGE_FLAG,startDay)
	end

	gui.PanelZhuanPan.addLotteryTimes(player,add)--活动期间充值计算转盘次数

	--记录合区后的每日充值
	if server_merge_day()>=0 and server_merge_day()<=2 then
		player:set_param(const.PP_HEQU_DAILY_CZ,util.ppn(player,const.PP_HEQU_DAILY_CZ)+add)
	end

	if util.ppn(player,const.PP_RECHARGE_DAY) ~= today() then
		player:set_param(const.PP_DAILY_RECHARGE,add)
		player:set_param(const.PP_RECHARGE_DAY,today())
		gui.moduleRedPoint.checkHeQuAct(player)
		gui.AwardHall_leichong.isActing(player)
	else
		player:set_param(const.PP_DAILY_RECHARGE,util.ppn(player,const.PP_DAILY_RECHARGE)+add)
		gui.moduleRedPoint.checkHeQuAct(player)
		gui.AwardHall_leichong.isActing(player)
	end


	-- gui.PanelVip.refreshPanel(player,0)
	gui.moduleRedPoint.checkFirstRechargeUsable(player);
	gui.moduleRedPoint.checkRechargeYouLi(player)
	gui.AwardHall_lxrecharge.setLxczDayNum(player)
end

function onRemVcoin(player,num,dayuse,flag)
	local now = tonumber(os.date("%Y%m%d",os.time()))
	local startDay = server_start_day()+1
	if flag ~= 108 then -- 交易的不计算
		if gui.PanelCelebrationAct.checkActIng() then--庆典活动-消费有礼
			local curXiaoFei = util.ppn(player, const.PP_XIAOFEI_RANK)+num
			player:set_param(const.PP_XIAOFEI_RANK,curXiaoFei)
			gui.PanelCelebrationAct.setXiaoFeiRank(player)
		end
		gui.PanelJingCaiAct.checkljxf(player,num)

		-- gui.June.setXiaoFeiRank(player)
	    -- server.set_var("dailyExpense", tonumber(server.get_var("dailyExpense")) + num)--每日0点清空
	end
	-- if now < 20160527 then
	-- 	for i=1,4 do
	-- 		if server.get_var("svr_consume"..i) ~= "" then
	-- 			server.set_var("svr_consume"..i,"")
	-- 		end
	-- 	end
	-- end
end

function onRemVcoinBind(player,num,dayuse,flag)
	-- server.set_var("dailyExpense", tonumber(server.get_var("dailyExpense")) + num)--每日0点清空
end

local HPMPBUFF_HP_NUM=100--每次回血数量
local HPMPBUFF_MP_NUM=300--每次回蓝数量
function addHpMpByItemBuff( player,hpGap,mpGap)
	local _num = util.ppn(player,const.PP_STATUS_YUANQIBUFF_SUM)
	if _num <= 0 then
		player:clear_status(30)
		return
	end
	local _hp = HPMPBUFF_HP_NUM
	local _mp = HPMPBUFF_MP_NUM
	if hpGap < HPMPBUFF_HP_NUM then
		_hp = hpGap
	end
	if mpGap < _mp then
		_mp = mpGap
	end
	if _num > 0 then
		if _num > _hp then
			player:add_hp(_hp)
			_num=_num-_hp
		else
			player:add_hp(_num)
			_num=0
		end
	end
	if _num > 0 then
		if _num > _mp then
			player:add_mp(_mp)
			_num=_num-_mp
		else
			player:add_mp(_num)
			_num=0
		end
	end
	player:set_param(const.PP_STATUS_YUANQIBUFF_SUM,_num)
	player:notify_hpmpchange(_num,_num)
	if _num > 0 then
		player:set_status(30,86400,_num,1,1)
	else
		player:clear_status(30)
	end
end

function reqDirectFly(player,command)
	-- print("reqDirectFly", command)
	local data = util.decode(command);
	if data and data.flyId then
		posmap.directFly(player, data.flyId);
		-- local p = data.param
		-- if server.find_map(p.map_id) then player:enter_map(p.map_id, p.x, p.y) end
	end
end
ckpanel.add_listener("player.reqDirectFly",player.reqDirectFly)

function checkDirectFly(player)
	player:echo("true")
	-- if util.ppn(player,const.PP_HUIYUAN) >= today() then
	-- 	player:alert(100,1,"您是尊贵的VIP玩家,享受了免费传送")
	-- 	player:echo("true")
	-- 	return
	-- end
	-- local f = player:get_free_direct_fly()
	-- if f > 0 then
	-- 	player:sub_free_fly(1)
	-- 	if f <= 20 then
	-- 		player:alert(100,1,"您的免费飞行次数剩余".. f .."次")
	-- 	end
	-- 	local r = math.random(1,100)
	-- 	if r <= 20 then
	-- 		player:alert(110,1,"成为VIP会员可享受无限制免费传送")
	-- 	end
	-- 	player:echo("true")
	-- else
	-- 	player:alert(110,1,"您的免费飞行次数已用完")
	-- 	player:alert(110,1,"成为VIP可享受无限制免费传送")
	-- end
end

function onClientTalk(player,param100)
	cktalk.triger_talk(player,param100)
end

function onDead( player )
	---暂时屏蔽复仇buff
	gui.moduleRedPoint.checkMarsUsable(player)
	local pmap = player:get_map()
	local mapid = pmap:get_id();
	if string.sub(mapid,1,4) == "zssd" then
		--npc.zhuanshen.ondead(player)
	elseif mapid == 'yxbiqi' then
		map.yxbiqi.addScroe(player,"dead")
	elseif mapid == "kinghome" then
		map.kinghome.checkKingGuild(pmap)--死皇宫
	elseif mapid == "diyi" then
		player:relive(120,"biqi",52,40)
	end
	-- if string.sub(mapid,1,3)=="zmt" then
	-- 	if util.ppn(player,const.PP_ZHENMOTA_EXP_POOL)>0 then
	-- 		player:push_lua_table("open",util.encode({name = "extend_richang", extend = {tab=1,sub="dailyItem2",btn="btnLingQu"}}))
	-- 	end
	-- end
	-- if mapid == "v202" then
	-- 	player:relive(120,"v002",77,99)
	-- 	player:set_param(const.PP_WLZB_DEAD_CAPACITY,os.date("%Y%m%d%H",os.time()))
	-- end
	-- local map = server.find_map("diyi")
	-- if map then
	-- 	if map:num_player() == 1 then
	-- 		local txdy = map:get_rand_player(1)
	-- 		if txdy  then
	-- 			npc.txdy.Awardtxdy(txdy)
	-- 		end
	-- 	end
	-- end
	-- if string.gsub(mapid,1,3) == "cbt" then
	-- 	player:relive(120,"v002",77,99)
	-- end
	-- if server.is_war() == 1 then
	-- 	if mapid == "kingcity" or mapid == "kinghome" then
	-- 		if player:is_dead() then
	-- 			player:relive(100)
	-- 		end
	-- 	end
	-- end
	if string.sub(mapid,1,3)=="ctg" then
		gui.PanelBreakUp.goToPrevious(player)
	end

	-- if mapid == "chiwooBattle" then
	-- 	local team_name = player:get_team_name()
	-- 	if team_name == "轩辕阵营" then
	-- 		player:relive(120,"chiwooBattle",101,45)
	-- 	elseif team_name == "蚩尤阵营" then
	-- 		player:relive(120,"chiwooBattle",18,129)
	-- 	end
	-- end
	if map_id == "shiwang" then
		if player:is_dead() then
			if player:drop_item(26040001) then
				server.info(10010,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>已死亡，<font color='#ff1fec'>封印技能书</font>掉落在地")
			end
		end
	end
	player:set_wing_lv(70001, 1);
end

function onPKDead( killed, killer )
	--自动加为仇人
	if killed:is_friend(killer:get_name()) ~= 1 then
		if killed:is_enemy(killer:get_name()) ~= 1 then
			killed:friend_change(killer:get_name(), 101)
		end
	end
	if killer:get_map():get_id() =="yxbiqi" then
		map.yxbiqi.killed(killed)
		if killer.get_owner then
			map.yxbiqi.killer(killer:get_owner())
		else
			map.yxbiqi.killer(killer)
		end
	end
	-- killer:alert(1,1,"你将"..killed:get_name().."杀死,当前战斗值为"..killer:get_pk_value().."点！")

	--[[--暂时屏蔽杀人邮件
	if killed:is_enemy(killer:get_name()) ==1 then
		local strRevenge = killed:get_param(const.PP_REVENGE_STR)
		strRevenge = #strRevenge>0 and strRevenge or "此仇不报非君子！"
		killed:post_mail("复仇宣言",killer:get_name()..":"..strRevenge)
	end
	]]
	--夜袭比奇
	local curMap = killer:get_map()
	if curMap then
		local mapId = curMap:get_id()
		if mapId == "yxbiqi" then
			map.yxbiqi.addScroe(killer,"killPlayer")
		elseif mapId =="cifu" then
			map.cifu.killman(killer,killed)
		elseif mapId =="shiwang" then
			if killed:is_dead() then
				if killed:drop_item(26040001) then
					server.info(10010,0,"玩家<font color='#01ebff'>"..killed:get_name().."</font>已死亡,<font color='#ff1fec'>封印技能书</font>掉落在地")
				end
			end
		end
	end

end

local join_guild_data={
	[1]={
			[1]={time=300},[2]={time=1800},[3]={time=3600},
		},
	[2]={
			[1]={time=600},[2]={time=1800},[3]={time=3600},
		},
	[3]={
			[1]={time=21600},
		},
	}

function checkCreateGuild(player)
	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
	local leaveTime=util.ppn(player,const.PP_GUILD_LEAVE_TIME)
	local start=tonumber(server_start_day())+1
	local merge=tonumber(server_merge_day())+1
	local time=os.time()
	for i=1,#join_guild_data do
		if start <=3 then
			time=join_guild_data[1][start].time
		elseif is_merge_server() and merge<=3 then
			time=join_guild_data[2][merge].time
		else
			time=join_guild_data[3][1].time
		end
	end

	if leaveTime+time<=os.time() then
		if player:get_level() >= 70 then
			if player:get_vcoin() >= 1000 then
				player:sub_vcoin(1000, "创建帮会")
				player:echo("true")
			else
				player:alert(1,1,"建立帮会需要缴纳1000元宝!")
			end
		else
			player:alert(1,1,"建立帮会需要等级达到70级!")
		end
	else
		local clock =setTimeFormat(leaveTime+time-os.time())
		player:alert(1,1,"帮会冷却中,"..clock.."后方创建帮会")
	end
end

function checkJoinGuild(player)
	local leaveTime=util.ppn(player,const.PP_GUILD_LEAVE_TIME)
	local start=tonumber(server_start_day())+1
	local merge=tonumber(server_merge_day())+1
	local time=os.time()

	for i=1,#join_guild_data do
		if start <=3 then
			time=join_guild_data[1][start].time
		elseif is_merge_server() and merge<=3 then
			time=join_guild_data[2][merge].time
		else
			time=join_guild_data[3][1].time
		end
	end

	if leaveTime+time<=os.time() then
		player:echo("true")
	else
		local clock =setTimeFormat(leaveTime+time-os.time())
		player:alert(1,1,"帮会冷却中,"..clock.."后方可再次加入帮会")
	end
end

function ReallyCreateGuild(player)
--	if player:num_item("奴玛号角",1)>=1 then
--		player:remove_item("奴玛号角",1);
		-- util.alert_msg(26,"玩家<font color='#ff0000'>["..player:get_name().."]</font>创建了帮会<font color='#ff0000'>["..player:get_guild().."]</font>!正在向王城战蓄势待发!<a color='#00ff00' href='event:local_OpenPanel_act7'>[我也要创建]</a>",1)
		-- util.alert_msg(26,"玩家<font color='#ff0000'>["..player:get_name().."]</font>创建了帮会<font color='#ff0000'>["..player:get_guild().."]</font>!正在向王城战蓄势待发!<a color='#00ff00' href='event:local_OpenPanel_act7'>[我也要创建]</a>",1)
		server.info(10000,0,"恭喜<font color='#01ebff'>"..player:get_name().."</font>创建帮会<font color='#01ebff'>"..player:get_guild().."</font>");
		server.info(10,0,"[system]恭喜<font color='#01ebff'>"..player:get_name().."</font>创建帮会<font color='#01ebff'>"..player:get_guild().."</font><a color='#00ff00' href='event:local_OpenPanel_act7' underline='#00ff00'>加入帮会</a>");
		player:push_lua_table("PanelGuild",util.encode({cmd = "createGuild"}));
		gui.PanelAchieve.triggerTaskEvent(player, "joinOrCreateGuild")--加入或创建公会成就
--	else
--		util.alert_msg(4, "创建帮会需要奴玛号角!", player)
--	end
end

function onEnterGuild(player)
	-- print("//////////////onEnterGuild//////////////", player:get_guild_title())
	if player:get_guild_title() > 0 then
		local winglv=70000+util.ppn(player,const.PP_WING_LEVEL)
		local tianguan=util.ppn(player,const.PP_TIANGUAN_ALLSTARS)
		player:updataChart(winglv,tianguan)
		if gui.PanelGuild.onEnterGuild then gui.PanelGuild.onEnterGuild(player); end

		--触发加入帮会任务
		ckguild.triger_join_guild(player);
	end
end

function onEnterGroup(player)
	ckgroup.triger_join_group(player);
end

function onApplyFriend(player)
	--print("onApplyFriend")
	task.task1000.check_add_friend(player);
	task.task1100.check_add_friend(player, exp);
end

function onLeaveGuild(player, param0, param1, param2, param3)
	if param0 == 1 then
		player:alert(10,1,"您所在帮会已解散");
	elseif param0 == 2 then
		player:alert(10,1,"您已被踢出帮会");
	elseif param0 == 3 then
		player:alert(10,1,"您已离开帮会");
	end

	if player:get_map():get_id() =="kinghome" then
		if player:is_dead() then
			player:relive(100)
		else
			go.home(player)
		end
	end
	--清除帮会buff
	local winglv=70000+util.ppn(player,const.PP_WING_LEVEL)
	local tianguan=util.ppn(player,const.PP_TIANGUAN_ALLSTARS)
	player:updataChart(winglv,tianguan)
	player:set_param(const.PP_GUILD_LEAVE_TIME,os.time())--记录离开帮会的时间

	if gui.PanelGuild.onLeaveGuild then gui.PanelGuild.onLeaveGuild(player); end
end
-------------------尸王殿--------------------
local date_shiwang_timer={
	[1]={con=const.PP_TIMER_INDEX100,timer=100},
	[2]={con=const.PP_TIMER_INDEX101,timer=101},
	[3]={con=const.PP_TIMER_INDEX102,timer=102},
	[4]={con=const.PP_TIMER_INDEX103,timer=103},
	[5]={con=const.PP_TIMER_INDEX104,timer=104},
	[6]={con=const.PP_TIMER_INDEX105,timer=105},
	[7]={con=const.PP_TIMER_INDEX106,timer=106},
	[8]={con=const.PP_TIMER_INDEX107,timer=107},
	[9]={con=const.PP_TIMER_INDEX108,timer=108},
	[10]={con=const.PP_TIMER_INDEX109,timer=109},
	[11]={con=const.PP_TIMER_INDEX110,timer=110},
	[12]={con=const.PP_TIMER_INDEX111,timer=111},
}

function onGetItem(player,item_name,item_id,num,pos,seedId)
	--set_per(player)
	if item_id==26040001 and player:get_map():get_id() == "shiwang"then
		for i=1,#date_shiwang_timer do
			--print("i",i)
			if player:get_param(date_shiwang_timer[i].con) == "0" then
			--if util.ppn(player,date_shiwang_timer[i].con) ==0 then
				player:set_param(date_shiwang_timer[i].con,seedId)
				player:set_timer(date_shiwang_timer[i].timer,180*1000);--------30秒cd
				server.info(10010,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>捡取了<font color=#ff1fec>封印技能书</font>")
				set_per(player)
				break
			end
		end
	end
	if item_id==32000004 and string.sub(player:get_map():get_id(),1,4) == "swmy" then ----通行证
		local index=tonumber(string.sub(player:get_map():get_id(),5,6))
		npc.shengwei.updataUIleftTips(player,index)
	end
	ckitem.triger_item(player,item_name,item_id)
	-- gui.PanelLotteryExchange.onGetItem(player, item_id)--用装备和寻宝积分兑换物品
end
function onRemItem(player,item_name,item_id,num,pos,seedId)
	local num=player:num_item(26040001)
	if item_id==26040001 then
		if num>0 then

		else
			set_per(player)
		end
	end
end
--------尸王殿  倒计时  称号----
function set_per(player)

	local num=player:num_item(26040001)
	--print("num",num)
	if num>0 then
		player:set_param(const.PP_SHIWANGTIME_FLAG,os.time())
	else
		player:set_param(const.PP_SHIWANGTIME_FLAG,0)
	end
	--print("00000000000",util.ppn(player,const.PP_SHIWANGTIME_FLAG))
	npc.fenghao.updateAllTitle(player)
end

function onDropItem(player,item_name,item_id,num,pos,seedId)
	--print(">>>>>>>")
	local num=player:num_item(26040001)
	if item_id==26040001 then
		for i=1,#date_shiwang_timer do
			--if util.ppn(player,date_shiwang_timer[i].con) ==seedId then
			if player:get_param(date_shiwang_timer[i].con) == seedId then
				player:set_param(date_shiwang_timer[i].con,"0")
				player:set_timer(date_shiwang_timer[i].timer,0);
				break
			end
		end
		if num==0 then
			set_per(player)
			--player:set_param(const.PP_SHIWANGTIME_FLAG,0)
		end
			--set_per(player)
	end
end

function get_shiwang_award( player )
	--print("get_shiwang_award")
	if player:get_map():get_id() == "shiwang" then

		server.info(10010,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>成功解封了技能书")
		player:remove_item(26040001,1)
		player:add_item(26040002,1,1);
	end
end

function onTimer100(player)--尸王定时器
	get_shiwang_award(player)
	--player:set_param(date_shiwang_timer[i].con,0)
end
function onTimer101(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer102(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer103(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer104(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer105(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer106(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer107(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer108(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer109(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer110(player)--尸王定时器
	get_shiwang_award(player)
end
function onTimer111(player)--尸王定时器
	get_shiwang_award(player)
end


--------------------------------------------------------------
function onEquipItem(player,item_name,item_id)
	-- gui.PanelAchieve.checkAchieveFirst(player, "equip") -- 穿戴装备
	-- gui.moduleGuide.showShiftEquip(player, item_name)
-- 	local s = player:get_task_state(1000)
-- 	local d = math.fmod(s,const.TASK_STATE_NUM)
-- 	local b = math.floor(s/const.TASK_STATE_NUM)
-- 	if b == 59 and d >= const.TSACED and d < const.TSCOMP then -- 主线未知暗殿
-- 		gui.moduleGuide.checkMainTaskEquip(player)
-- 	end
end

--[[
status:状态id，详情查看const.STATUS_TYPE
duration:状态持续的时间
param:参数，具体用法见服务器代码
gap:间隔，有些状态每隔一段时间会执行一次操作
flags:暂时未看懂有什么用

function set_status(status, duration, param, gap, flags)
end
--]]

function onEnterSafeArea(player)
	local map = player:get_map()
	if map then
		local mapid = map:get_id()
		if mapid and mapid =="tucheng" then
			if server.get_temp_var("autoAddExp") == "open" then
				if player:get_level() >= 50 then
					local time = 5
					util.alert_msg(4, "进入膜拜区域，"..time.."秒后方可获取经验", player)
					player:set_timer(3,time*1000);
				end
			end
		end
	end
	player:alert(1,1,"进入安全区")
end


function onLeaveSafeArea(player)
	if server.get_temp_var("autoAddExp") == "open" then
		local map = player:get_map()
		if map then
			local mapid = map:get_id()
			if mapid and mapid =="tucheng" then
				if player:get_level() >= 50 then
					util.alert_msg(4, "您已经离开城主膜拜区域，想继续获得经验请回安全区", player)
					player:set_timer(3,0);
				end
			end
		end
	end
	player:alert(1,1,"离开安全区")
end

function onCalculateFight(player,fight,a,b,c)
	local level = util.ppn(player,const.PP_INNERPOWER_LEVEL)
	local job = player:get_job_name()
	local val = gui.PanelInnerPower.getInnerPowerVal(player,level,"val")
	local rate = 0
	if job == "warrior" then
		rate = 900
	end
	if job == "wizard" then
		rate = 2400
	end
	if job == "taoist" then
		rate = 1500
	end
	val = val * rate / 10000

	player:set_fight_point(fight + val)
	if player:get_level() >= 70 then
		npc.fenghao.fightRanking(player, fight + val)
	end
end

function getNpcName(player,npc_name)
	-- local first_name = {
	-- 	["城主雕像"] = server.get_var("kingofking"),
	-- 	["第一男战士"] = server.get_var("fightwarriormalea"),
	-- 	["第一女战士"] = server.get_var("fightwarriorfemalea"),
	-- 	["第一男法师"] = server.get_var("fightwizardmalea"),
	-- 	["第一女法师"] = server.get_var("fightwizardfemalea"),
	-- 	["第一男道士"] = server.get_var("fighttaoistmalea"),
	-- 	["第一女道士"] = server.get_var("fighttaoistfemalea"),
	-- }
	-- if npc_name ~= "" and first_name[npc_name] then
	-- 	return first_name[npc_name]
	-- end
end

function enter_game_triger_fun(player)-------进游戏触发的一些方法汇总
	--------------------------------------------------------------------------------------------------------------触发function 1
	if util.ppn(player,const.PP_BOSS_CUR_JIE) >= 1 then
		player:set_monpoint_level(util.ppn(player,const.PP_BOSS_CUR_JIE));----设置玩家的boss积分等级
	end

	--------------------------------------------------------------------------------------------------------------触发function 2(服务器重启之后检测暗殿地图上面还有没有相应的怪物，将怪物的刷新显示恢复正常)
	-- local map = server.find_map("v200");
	-- if map then
	-- 	local mon_data=mon.bossRefresh.GetBossTable();
	-- 	local result = false;
	-- 	if mon_data then
	-- 		for i=1,#mon_data do
	-- 			if map:num_monster(mon_data[i].bossRealName) >= 1 then
	-- 				server.set_var("eightBoss"..i,0)
	-- 				result = true;
	-- 			end
	-- 		end
	-- 	end
	-- 	if result then
	-- 		map:map_exe("mon.bossRefresh.updateData")
	-- 	end

	-- end
	--------------------------------------------------------------------------------------------------------------触发function 3
	-- if tonumber(server_start_day()) <= 6 then------开区7天充值排名
	-- 	gui.AwardRecharge7DRank.Recharge_kq7d(player,util.ppn(player,const.PP_KQ7T_RECHARGE_NUM));
	-- end
	-- --------------------------------------------------------------------------------------------------------------触发function 4
	-- if tonumber(server_start_day()) >= 8 then------开区九天之后通过邮件发送奖励
	-- 	gui.AwardRecharge7DRank.Send7DRechargeAward(player)
	-- end
	-- --------------------------------------------------------------------------------------------------------------触发function 5
	-- if get_day_YMD() >=20160207 and get_day_YMD() <=20160213 then
	-- 	gui.AwardHappyNewYear.onInitConst(player);
	-- end
	--------------------------------------------------------------------------------------------------------------触发function 6
	-- if is_merge_server()  then
	-- 	npc.biggm.reset_hfbianl(player);
	-- end
	--------------------------------------------------------------------------------------------------------------触发function 7
	-- player:notify_status_desp(68,1,50);----翅膀数据量太大分开发送
	-- player:notify_status_desp(68,51,100);
	-- player:notify_status_desp(68,101,120);
	-- player:notify_status_desp(80,1,50);----元神数据量太大分开发送
	-- player:notify_status_desp(80,51,100);
	-- player:notify_status_desp(84,1,50);----元神2数据量太大分开发送
	-- player:notify_status_desp(84,51,100);
	-- player:notify_status_desp(55,1,50);----坐骑数据量太大分开发送
	-- player:notify_status_desp(55,51,100);
	-- player:notify_status_desp(55,101,150);
	--------------------------------------------------------------------------------------------------------------触发function 8
	--元神塔排行重新记录
	-- if util.svn("yuanshenReNote") == 0 then
	-- 	server.set_var("yuanshenReNote",1);
	-- 	server.set_var("yuanshenCharts","")----将溢出的元神排名置空
	-- end

	-- if util.ppn(player,const.PP_YS_RESET2_LEV)==0 then------------降低刷战力导致元神塔等级过高
	-- 	local level = util.ppn(player,const.PP_XINMOTA_MAX_LEVEL)
	-- 	local maxNum = 0
	-- 	if level>36 then
	-- 		player:set_param(const.PP_XINMOTA_MAX_LEVEL,36)
	-- 		maxNum=36
	-- 	else
	-- 		if level>1 then
	-- 			player:set_param(const.PP_XINMOTA_MAX_LEVEL,level-1)
	-- 			maxNum=level-1
	-- 		end
	-- 	end
	-- 	player:set_param(const.PP_YS_RESET2_LEV,1)
	-- 	local str=""
	-- 	for i=1,maxNum do
	-- 		if i==1 then
	-- 			str="1"
	-- 		else
	-- 			str=str.."_"..i
	-- 		end
	-- 	end
	-- 	player:set_param(const.PP_YUANSHEN_PASSED_TOWER,str)
	-- 	player:post_mail("测试邮件","测试邮件！",20001001,1)
	-- end
end


function onGetServerStartDay()
	return tonumber(server_start_day());
end

function onGetMergeStartDay()
	return tonumber(server_merge_day());
end
--1300, 1400},	{2200, 2300}
function onEnterEventArea(player,param)
	-- if param == "itempick" then------捡取物品的提示
	-- 	if util.svn("maponitemopen") == 1 then------如果是开启状态
	-- 		player:set_timer(7,100);
	-- 		if util.ppn(player, const.PP_FIGHT_FOR_TREASURE) == 0 then
	-- 			local time = tonumber(os.date("%H%M", os.time()))
	-- 			if time >= 2050 and time <= 2105 then
	-- 				player:set_param(const.PP_FIGHT_FOR_TREASURE, 1)
	-- 				count.countTimes("宝物争夺")
	-- 			end
	-- 		end
	-- 	end
	-- end
	-- if param == "leitai" then--------擂台上
	-- 	if server.get_var("arenaWar") == "open" then
	-- 		player:alert(1,1,"进入擂台区域10秒钟后将会获取经验和物品");
	-- 		player:set_timer(4, 10*1000)

	-- 		if util.ppn(player, const.PP_RING_CHALLENGE1) == 0 then
	-- 			local time = tonumber(os.date("%H%M", os.time()))
	-- 			if time >= 1300 and time <= 1400 then
	-- 				player:set_param(const.PP_RING_CHALLENGE1, 1)
	-- 				count.countTimes("擂台挑战")
	-- 			end
	-- 		end
	-- 		if util.ppn(player, const.PP_RING_CHALLENGE2) == 0 then
	-- 			local time = tonumber(os.date("%H%M", os.time()))
	-- 			if time >= 2200 and time <= 2300 then
	-- 				player:set_param(const.PP_RING_CHALLENGE2, 1)
	-- 				count.countTimes("擂台挑战")
	-- 			end
	-- 		end
	-- 	end
	-- end
end

function onLeaveEventArea(player,param)
	-- if param == "1" then------出捡取物品区域
	-- 	player:set_timer(7,0);
	-- end
	-- if param == "2" then------出擂台区域
	-- 	if server.get_var("arenaWar") == "open" then
	-- 		player:alert(1,1,"您已出擂台范围,将不会自动获取经验及道具");
	-- 		player:set_timer(4,0)
	-- 	end
	-- end
end

--进入副本回调
function onEnterCopy(map, player)
	ckcopy.triger_enter_copy(player, map);
end

----------镖车

function onDartAttacked(player,Attacker)---被攻击

	local alert_tick = util.ppn(player,const.PP_DART_HURT_ALERT_TICK)
	local now = os.time()
	if now - alert_tick > 10 then
		player:alert(1,1,"您的镖车正在受到"..Attacker:get_name().."的攻击,请及时救援");
		player:set_param(const.PP_DART_HURT_ALERT_TICK, now)
	end

	local hp,maxhp = player:get_dart_hp()
	local result = {}
	result.dart_hp = hp
	result.dart_maxhp = maxhp
	player:push_lua_table("dartInfo",util.encode(result))
end

function onDartDestoryed(player,Attacker)---被摧毁
	local result = {
		book = util.encode({actionid = ""}),
		str = "<font color=#b2a58b>您的镖车被</font>".."<font color=#ff3e3e>"..Attacker:get_name().."</font><font color=#b2a58b>击碎,已将对方列入您的仇恨列表</font>",
		labelConfirm = "确定",labelCancel = "",
		callFunc = "",
	}

	player:push_lua_table("showAlert",util.encode(result))

	if player:is_friend(Attacker:get_name()) ~= 1 then
		if player:is_enemy(Attacker:get_name()) ~= 1 then
			player:friend_change(Attacker:get_name(), 101)
		end
	end
	gui.PanelDart.getDartAwards(player, util.ppn(player,const.PP_DART_CURLEVEL), 0.7)

	local result1 = {}
	result1.hide = 1
	player:push_lua_table("dartInfo",util.encode(result1))
	player:set_param(const.PP_DART_CURLEVEL,0)
end

function onDartReached(player)--到达
	player:alert(1,1,"您的镖车到达目的地");
	player:kill_dart()
	player:set_param(const.PP_DART_SUCCEED_LEVEL, util.ppn(player,const.PP_DART_CURLEVEL))
	player:set_param(const.PP_DART_CURLEVEL,0)

	local result = {}
	result.hide = 1
	player:push_lua_table("dartInfo",util.encode(result))
	posnpc.go(player, 4002003)
end

function onDartOutRange(player)---离开范围
	local result = {}
	result.dart_state = 0
	player:push_lua_table("dartInfo",util.encode(result))

end

function onDartInRange(player)---进入范围
	local result = {}
	result.dart_state = 1
	player:push_lua_table("dartInfo",util.encode(result))

end

function onDartTimeOut(player)---时间到
	player:set_param(const.PP_DART_CURLEVEL,0)
	local result1 = {}
	result1.hide = 1
	player:push_lua_table("dartInfo",util.encode(result1))

	local result = {
		book = util.encode({actionid = ""}),
		str = "<font color=#b2a58b>您没能在规定时间内完成押镖任务,</font><font color=#ff3e3e>".."任务失败!".."</font>",
		labelConfirm = "确定",labelCancel = "",
		callFunc = "",
	}
	player:push_lua_table("showAlert",util.encode(result))
end

function onMapEnterCheck(player,mapid)
	local level = player:get_level();
	local zslv =  util.ppn(player,const.PP_ZHSH_LEVEL);
	local map = server.find_map(mapid);
	if map then
		local maplv = map:get_minlv();
		local maplvR = map:get_minlvR();

		if maplv>0 and level < maplv then
			player:alert(1,1,"等级不足"..maplv.."级,无法进入地图");
			return 0;
		end
		if maplvR>0 and zslv < maplvR then
			player:alert(1,1,"转生等级不足"..maplvR.."级,无法进入地图");
			return 0;
		end

		local curmap = player:get_map()
		if curmap then
			local curmapid = curmap:get_id()
			if string.gsub(curmapid,"%d+","") ~= string.gsub(mapid,"%d+","") and not tab.isMapEnterLegal(curmapid) and not tab.isMapEnterLegal(mapid) then
				if curmapid == 'kinghome' or curmapid == 'kingcity' then
					return 1
				end

				player:alert(1,1,"您当前已在副本中，无法传送！")
				return 0;
			elseif string.find(mapid,"ctg") and curmapid =="kingcity" and server.is_war()==0 then--非皇城战期间可以进入沙巴克
				return 1
			end
		end

		return 1;
	end
	return 0;
end

function onMapEnterd(player,premapid)
	local pmap = player:get_map()
	local premap = server.find_map(premapid)
	local mapid = pmap:get_id();
	ckmap.triger_enter_map(player, pmap)
	if pmap then
		if pmap:disable_huwei() == 1 and player:have_slave(10) > 0 then
			-- 进入不可召唤护卫地图,干掉护卫
			player:kill_slave(10)
			player:set_param(const.PP_HUWEI_MAP_KILL_FLAG,1)
		end
		if mapid =="kingcity" then
			map.kinghome.hideWarNotice(player)
		elseif mapid == "kinghome" then
			-- print("player enter kinghome",player:get_id())
			map.kinghome.checkKingGuild(pmap)--进皇宫
		elseif mapid == premapid and string.find(mapid,"jingying") then
			player:alert(1,0,"进层失败，你已返回第"..string.gsub(mapid,"[^%d+]","").."层！")
		end

		-- 天下第一,禁用气血石
		if mapid == "diyi" then
			player:enable_buff(26001, 0)
			player:enable_buff(26002, 0)
			player:enable_buff(26003, 0)
		end
	end
	if premap then
		if premapid == "diyi" then
			player:enable_buff(26001, 1)
			player:enable_buff(26002, 1)
			player:enable_buff(26003, 1)
			--print("============")
			player:set_attack_mode(util.ppn(player,const.PP_ATTACK_MODE)) --
			--print(util.ppn(player,const.PP_ATTACK_MODE))
		end

		if premapid == "kinghome" then
			-- print("player leave kinghome",player:get_id())
			map.kinghome.checkKingGuild(premap)--出皇宫
		elseif premapid == "yxbiqi" then
			if server.get_temp_var("yxbiqiState") == "start" then
				map.yxbiqi.addScroe(player,"leave")
			end
		end
		local m = {"kinghome","kingcity"}
		if table.indexof(m, premapid) and not table.indexof(m, mapid) then
			map.wangcheng.revertAttackMode(player)
		end
	end
	-- 从不可召唤地图出来,把之前干掉的护卫召出来
	if premap and premap:disable_huwei() == 1 then
		if util.ppn(player,const.PP_HUWEI_MAP_KILL_FLAG) > 0 then
			local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)
			if heroLevel>0 and pmap and pmap:disable_huwei() == 0 then
				gui.PanelHuWei.callHuWei(player)
				player:set_param(const.PP_HUWEI_MAP_KILL_FLAG,0)
			end
		end
	end

	if premap == nil and pmap and pmap:disable_huwei() == 0 then
		local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)
			if heroLevel>0 then
			gui.PanelHuWei.callHuWei(player)
			player:set_param(const.PP_HUWEI_MAP_KILL_FLAG,0)
		end
	end
	--通知客户端收起和显示右上角
	if not tab.isMapCallLegal(mapid) then
		player:push_lua_table("lockExtendBtns",util.encode({visible=false, lock="lock"}));
	else
		player:push_lua_table("lockExtendBtns",util.encode({visible=true}));
	end
end

------满足条件推送1 不满足推送0
function onMapConnCheck(player,mapid)----返回值为0表示不可传送 1表示可以传送
	-- local level = player:get_level();
	-- local zslv =  util.ppn(player,const.PP_ZHSH_LEVEL);
	-- if mapid == "v101" then
	-- 	if level < 30 then
	-- 		player:alert(1,1,"邪恶领地30级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v103" then
	-- 	if level < 40 then
	-- 		player:alert(1,1,"白猪巢穴40级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "kingcity" then
	-- 	if level < 50 then
	-- 		player:alert(1,1,"玛法阁50级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v107" then
	-- 	if level < 60 then
	-- 		player:alert(1,1,"赤月谷60级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v109" then
	-- 	if level < 70 then
	-- 		player:alert(1,1,"蛮荒祭坛70级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v116" then
	-- 	if zslv < 1 then
	-- 		player:alert(1,1,"魔龙秘殿转生1级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v119" then
	-- 	if zslv < 2 then
	-- 		player:alert(1,1,"火龙之穴转生2级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v121" then
	-- 	if zslv < 2 then
	-- 		player:alert(1,1,"仙狐幻境转生2级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v111" then
	-- 	if zslv < 3 then
	-- 		player:alert(1,1,"皇陵墓室3转方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v113" then
	-- 	if zslv < 3 then
	-- 		player:alert(1,1,"船舱底层转生3级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- if mapid == "v115" then
	-- 	if zslv < 5 then
	-- 		player:alert(1,1,"雪域仙境转生5级方可进入");
	-- 		return 0;
	-- 	end
	-- end
	-- return 1;
end

-- 技能升级
function checkSkillLevelUp(player, skill_type)
	-- print("checkSkillLevelUp", skill_type);
	if skill_type then
		if player:check_skill_level_up(skill_type) > 0 then
			player:alert(1,1, "恭喜您，技能升级成功！");
		end
		-- print("check_skill_level_up", ret)
	end
end

function onFunction(player,paramId,paramValue)
	if paramId == 128 then
		npc.huishou.addRecycleLimit(player,paramValue)
	elseif paramId == 129 then
		npc.huishou.addDeoptSize(player,paramValue)
	else
		-- print("onFunction-", player,paramId,paramValue)
	end
end

function onFunctionRemove(player,paramId,paramValue)
	-- print("2", player,paramId,paramValue)
end

function onApplyJoinGuild(player, player_apply)
	player:push_lua_table("tip_guild", util.encode({name = "tip_guild"}))
	player:notify_guild_member_change()
end

function onCancelJoinGuild(player, player_apply)
	player:notify_guild_member_change()
end

function onBuyConsignItem(player,item_name,item_id,num,nbenefits,seller_seed)
	local info="<font color='#EE9A00'>亲爱的玩家：</font>" .. "<br>".."<font color='#b2a58b'>您的<font color=#ff1fec>"..item_name.."</font>已经被其他玩家购买，您获得收益"..nbenefits.."元宝,请在寄售行<a color='#00ff00' href='event:local_OpenPanel_act40' underline='#00ff00'>我的货架</a>中提取收益</font>"
	--seller_seed:post_mail("收益提醒", info)
	server.post_mail_to_one(seller_seed,"收益提醒",info)
end

function onItemLastTime(player,item_name)
	local info="<font color='#EE9A00'>亲爱的玩家：</font>" .. "<br>".."<font color='#b2a58b'>您的<font color=#ff1fec>"..item_name.."</font>已经过期！"
	player:post_mail("过期提示", info)
end

function onConsignItemSold(player)
	gui.moduleRedPoint.checkConsignUsable(player);
end

function onTakeConsignVCoin(player)
	gui.moduleRedPoint.checkConsignUsable(player);
end

function onGetMailAward(player,title)
	-- gui.AwardHall_jingji.getMailTips(player,title)
end

ckpanel.add_listener("player.checkSkillLevelUp",player.checkSkillLevelUp)

function onClientData(player,commond)
	local data = util.decode(commond)
	if data.actionid == "enterGame" then
		--检测地图推送客户端显示收起extend
		local map = player:get_map();
		if map then
			player:push_lua_table("extendVisible",util.encode({visible=tab.isMapCallLegal(map:get_id())}));
		end
	end
end

ckpanel.add_listener("player.onClientData",player.onClientData)


--门派奖励结算
function onCheckChallengerAward(seedName,recordRank)
	local player = server.find_player_by_seed(seedName)
	if player then
		local award,firstidx = tab.menpai.getChartAwardByRank(player,recordRank)

		util.sendMail(player,"门派挑战排名奖","您上次排名第"..recordRank.."名，获得如下奖励",award)

		if firstidx>0 then
			local str = player:get_param(const.PP_MENPAI_FIRST_AWARD)
			local firstGotTable = string.split(str,",")
			table.insert(firstGotTable,firstidx)
			player:set_param(const.PP_MENPAI_FIRST_AWARD,string.concat(firstGotTable,","));
		end
	else
		local award = tab.menpai.getNormalChartAwardByRank(recordRank)
		util.sendMail(seedName,"门派挑战排名奖","您上次排名第"..recordRank.."名，获得如下奖励",award)
	end
end

function onEnterTrap( player,id )
	-- print("onEnterTrap",id)
	local pmap = player:get_map()
	if pmap then
		local mapId = pmap:get_id()
		if mapId =="yxbiqi" and id>10000 then
			map.yxbiqi.onEnterTrap(player,id)
		end
	end
end
function onLeaveTrap( player,id )
	-- print("onLeaveTrap",id)
	local pmap = player:get_map()
	if pmap then
		local mapId = pmap:get_id()
		if mapId =="yxbiqi" and id>10000 then
			map.yxbiqi.onLeaveTrap(player,id)
		end
	end
end
function onAddSkillExp(player,skillid,skillexp)
	gui.moduleRedPoint.checkSkillUsable(player)
end