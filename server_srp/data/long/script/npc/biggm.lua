module(..., package.seeall)

local talk_tab = {
	func = "npc.biggm.btnCall",scrollHeight = 550,title = {str="调试功能(开区"..server_start_day().."天)",fontSize = 24,color=0xFFFFFF,},
	con_tab = {	[1]  = {mtype="button",	name="装备领取",		funcIndex="zbck",				res="btn_transmit-button",	posX=003+84,	posY=350+30+140, haveNext = true},
				[2]  = {mtype="button",	name="升级",			funcIndex="lvup",				res="btn_transmit-button",	posX=178+84,	posY=350+30+140, haveNext = true},
				[3]  = {mtype="button",	name="坐骑丹/灵羽",		funcIndex="wupin",				res="btn_transmit-button",	posX=353+84,	posY=350+30+140, haveNext = true},
				[4]  = {mtype="button",	name="开启全部功能",	funcIndex="openfunc",			res="btn_transmit-button",	posX=528+84,	posY=350+30+140,	},
				[5]  = {mtype="button",	name="测试vip",			funcIndex="viplevel",			res="btn_transmit-button",	posX=003+84,	posY=280+30+140, haveNext = true},
				[6]  = {mtype="button",	name="测试充值",		funcIndex="recharge",			res="btn_transmit-button",	posX=178+84,	posY=280+30+140, haveNext = true},
				[7]  = {mtype="button",	name="测试公告",		funcIndex="testAlert",			res="btn_transmit-button",	posX=353+84,	posY=280+30+140, haveNext = true},
				[8]  = {mtype="button",	name="领取金币",		funcIndex="getMoney",			res="btn_transmit-button",	posX=528+84,	posY=280+30+140,	},
				[9]  = {mtype="button",	name="重置开区时间",	funcIndex="kqsj",				res="btn_transmit-button",	posX=003+84,	posY=210+30+140, haveNext = true},
				[10] = {mtype="button",	name="领取测试武器",	funcIndex="getWeapon",			res="btn_transmit-button",	posX=178+84,	posY=210+30+140,	},
				[11] = {mtype="button",	name="领取元宝",		funcIndex="getVcoin",			res="btn_transmit-button",	posX=353+84,	posY=210+30+140,	},
				[12] = {mtype="button",	name="增加贡献",		funcIndex="addDonate",			res="btn_transmit-button",	posX=528+84,	posY=210+30+140,	},
				[13] = {mtype="button",	name="发奖励邮件",		funcIndex="sendMail",			res="btn_transmit-button",	posX=003+84,	posY=140+30+140,	},
				[14] = {mtype="button",	name="发文字邮件",		funcIndex="sendTextMail",		res="btn_transmit-button",	posX=178+84,	posY=140+30+140,	},
				[15] = {mtype="button",	name="开启泡点",		funcIndex="openAutoAddExp",		res="btn_transmit-button",	posX=353+84,	posY=140+30+140,	},
				[16] = {mtype="button",	name="关闭泡点",		funcIndex="closeAutoAddExp",	res="btn_transmit-button",	posX=528+84,	posY=140+30+140,	},
				[17] = {mtype="button",	name="开启神威狱",		funcIndex="openKamuiPrison",	res="btn_transmit-button",	posX=003+84,	posY=070+30+140,	},
				[18] = {mtype="button",	name="关闭神威狱",		funcIndex="closeKamuiPrison",	res="btn_transmit-button",	posX=178+84,	posY=070+30+140,	},
				[19] = {mtype="button",	name="刷元宝boss",		funcIndex="refreshMoneyBoss",	res="btn_transmit-button",	posX=353+84,	posY=070+30+140,	},
				[20] = {mtype="button",	name="清元宝boss",		funcIndex="clearMoneyBoss",		res="btn_transmit-button",	posX=528+84,	posY=070+30+140,	},
				[21] = {mtype="button",	name="开启蚩尤战场",	funcIndex="openChiwooBattle",	res="btn_transmit-button",	posX=003+84,	posY=000+30+140,	},
				[22] = {mtype="button",	name="结束蚩尤战场",	funcIndex="closeChiwooBattle",	res="btn_transmit-button",	posX=178+84,	posY=000+30+140,	},
				[23] = {mtype="button",	name="增加灵力值",		funcIndex="addSoulExp",			res="btn_transmit-button",	posX=353+84,	posY=000+30+140,	},
				[24] = {mtype="button",	name="增加BOSS积分",	funcIndex="addBossJiFen",		res="btn_transmit-button",	posX=528+84,	posY=000+30+140,	},
				[25] = {mtype="button",	name="增加转生等级",	funcIndex="addZSDJ",			res="btn_transmit-button",	posX=003+84,	posY=000+30+70,	haveNext = true},
				[26] = {mtype="button",	name="设置合区时间",	funcIndex="addHQSJ",			res="btn_transmit-button",	posX=178+84,	posY=000+30+70,	haveNext = true},
				[27] = {mtype="button",	name="清理首周冲榜",    funcIndex="addQLSCBL",			res="btn_transmit-button",	posX=353+84,	posY=000+30+70,	},
				[28] = {mtype="button",	name="清空背包",        funcIndex="clearBag",			res="btn_transmit-button",	posX=528+84,	posY=000+30+70,	},
				[29] = {mtype="button",	name="修改帮会信息",    funcIndex="guildLevelChange",	res="btn_transmit-button",	posX=003+84,	posY=000+30,	haveNext = true},
				[30] = {mtype="button",	name="清元神塔排名",    funcIndex="resetYuanShen",		res="btn_transmit-button",	posX=178+84,	posY=000+30,	haveNext = true},
				[31] = {mtype="button",	name="开启比奇夜战",    funcIndex="openBqyz",	res="btn_transmit-button",	posX=353+84,	posY=000+30,	haveNext = true},
				[32] = {mtype="button",	name="关闭比奇夜战",    funcIndex="closeBqyz",		res="btn_transmit-button",	posX=528+84,	posY=000+30,	},
				},
}

function onTalkaddHQSJ(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",scrollHeight = 340,title = {str="设置合区时间",fontSize = 24,color=0xFFFFFF,},
		con_tab = {
			[1]  = {mtype="button",	name="第1天",	funcIndex="mergeSvrResetTime0",			res="btn_transmit-button",	posX=003+84,	posY=350+30+70-140,	},
			[2]  = {mtype="button",	name="第2天",	funcIndex="mergeSvrResetTime1",			res="btn_transmit-button",	posX=178+84,	posY=350+30+70-140,	},
			[3]  = {mtype="button",	name="第3天",	funcIndex="mergeSvrResetTime2",			res="btn_transmit-button",	posX=353+84,	posY=350+30+70-140,	},
			[4]  = {mtype="button",	name="第4天",	funcIndex="mergeSvrResetTime3",			res="btn_transmit-button",	posX=528+84,	posY=350+30+70-140,	},
			[5]  = {mtype="button",	name="第5天",	funcIndex="mergeSvrResetTime4",			res="btn_transmit-button",	posX=003+84,	posY=280+30+70-140,	},
			[6]  = {mtype="button",	name="第6天",	funcIndex="mergeSvrResetTime5",			res="btn_transmit-button",	posX=178+84,	posY=280+30+70-140,	},
			[7]  = {mtype="button",	name="第7天",	funcIndex="mergeSvrResetTime6",			res="btn_transmit-button",	posX=353+84,	posY=280+30+70-140,	},
			[8]  = {mtype="button",	name="第8天",	funcIndex="mergeSvrResetTime7",			res="btn_transmit-button",	posX=528+84,	posY=280+30+70-140,	},
			[9]  = {mtype="button",	name="取消合区",funcIndex="mergeSvrResetTime8",			res="btn_transmit-button",	posX=003+84,	posY=210+30+70-140,	},
			[10] = {mtype="button",	name="返回",	funcIndex="BackToMainPage",				res="btn_transmit-button",	posX=003+84,	posY=140+30+70-140, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalkmergeSvrResetTime0(player)
	reset_server_merge(0)
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime1(player)
	reset_server_merge(1)
	player:alert(1,1,"合区信息已重置")
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime2(player)
	reset_server_merge(2)
	player:alert(1,1,"合区信息已重置")
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime3(player)
	reset_server_merge(3)
	player:alert(1,1,"合区信息已重置")
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime4(player)
	reset_server_merge(4)
	player:alert(1,1,"合区信息已重置")
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime5(player)
	reset_server_merge(5)
	player:alert(1,1,"合区信息已重置")
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime6(player)
	reset_server_merge(6)
	player:alert(1,1,"合区信息已重置")
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime7(player)
	reset_server_merge(7)
	player:alert(1,1,"合区信息已重置")
	server.PushMergeStartDay();
end
function onTalkmergeSvrResetTime8(player)
	reset_server_merge2()
	server.PushMergeStartDay();
end

function reset_server_merge(i)
	server.set_var("server_merge_tag",1)
	server.set_var("server_merge_time",tostring(os.time()-i*24*60*60))
	server.server_exe("npc.biggm.reset_hfbianl");
end

function reset_server_merge2()
	server.set_var("server_merge_tag","")
	server.set_var("server_merge_time","")
end

--合区重启
function resetHeQu()
	if util.svn("last_merge_time") ~= util.svn("server_merge_time") then
		--合区活动沙城争霸奖励领取变量
		for i=1,3 do
			server.set_var("svr_sczb_ling"..i,"")
		end
		server.set_var("last_merge_time",util.svn("server_merge_time"))
	end
end
resetHeQu()

function reset_hfbianl(player)
	if util.ppn(player,const.PP_HEQU_TIME) ~= util.svn("server_merge_time") then
		player:set_param(const.PP_HEQU_TIME,util.svn("server_merge_time"));
		-----合区后设置
		gui.PanelMysteryStore.refreshStore(player)
		--下面是多次合区后需要刷新的变量
	--	player:set_param(757,0);
		-- for i=const.PP_NEW_YEAR_AWARD1,const.PP_NEW_YEAR_AWARD1+6 do
		-- 	player:set_param(i,0)
		-- end
		-- player:set_param(const.PP_HEQU_RECHARGE_TOTAL,0)--重置合区前七天累计充值
		-- for j=const.PP_HEQU_RECHARGE_TOTAL1,const.PP_HEQU_RECHARGE_TOTAL1+26 do
		-- 	player:set_param(j,0)
		-- end
		-- genxxx.clearOnceTimesVar(player)

		--清理合区活动里的变量
		for i=0,11 do
			player:set_param(const.PP_LANGYA_USEB_NUM+i,0)
		end
		--合区活动沙城争霸奖励领取变量
		for i=1,3 do
			server.set_var("svr_sczb_ling"..i,"")
		end
	end
end

function onTalkaddZSDJ(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",
		con_tab = {
			[1]  = {mtype="button",	name="0",		funcIndex="ZS0",			res="btn_transmit-button",	posX=003+84,	posY=350+30+70,	},
			[2]  = {mtype="button",	name="1",		funcIndex="ZS1",			res="btn_transmit-button",	posX=178+84,	posY=350+30+70,	},
			[3]  = {mtype="button",	name="2",		funcIndex="ZS2",			res="btn_transmit-button",	posX=353+84,	posY=350+30+70,	},
			[4]  = {mtype="button",	name="3",		funcIndex="ZS3",			res="btn_transmit-button",	posX=528+84,	posY=350+30+70,	},

			[5]  = {mtype="button",	name="4",		funcIndex="ZS4",			res="btn_transmit-button",	posX=003+84,	posY=280+30+70,	},
			[6]  = {mtype="button",	name="5",		funcIndex="ZS5",			res="btn_transmit-button",	posX=178+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="6",		funcIndex="ZS6",			res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[8]  = {mtype="button",	name="7",		funcIndex="ZS7",			res="btn_transmit-button",	posX=528+84,	posY=280+30+70,	},

			[9]  = {mtype="button",	name="8",		funcIndex="ZS8",			res="btn_transmit-button",	posX=003+84,	posY=210+30+70,	},
			[10] = {mtype="button",	name="9",		funcIndex="ZS9",			res="btn_transmit-button",	posX=178+84,	posY=210+30+70,	},
			[11] = {mtype="button",	name="10",		funcIndex="ZS10",			res="btn_transmit-button",	posX=353+84,	posY=210+30+70,	},
			[12] = {mtype="button",	name="11",		funcIndex="ZS11",			res="btn_transmit-button",	posX=528+84,	posY=210+30+70,	},

			[13] = {mtype="button",	name="12",		funcIndex="ZS12",			res="btn_transmit-button",	posX=003+84,	posY=140+30+70,	},
			[14] = {mtype="button",	name="13",		funcIndex="ZS13",			res="btn_transmit-button",	posX=178+84,	posY=140+30+70,	},
			[15] = {mtype="button",	name="14",		funcIndex="ZS14",			res="btn_transmit-button",	posX=353+84,	posY=140+30+70,	},
			[16] = {mtype="button",	name="返回",	funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=528+84,	posY=140+30+70, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalkZS0(player)
	player:set_zslevel(0)
	player:alert(1,1,"转生等级已设置为0级！")
end

function onTalkZS1(player)
	player:set_zslevel(1)
	player:alert(1,1,"转生等级已设置为1级！")
end


function onTalkZS2(player)
	player:set_zslevel(2)
	player:alert(1,1,"转生等级已设置为2级！")
end

function onTalkZS3(player)
	player:set_zslevel(3)
	player:set_zslevel(10)
	player:alert(1,1,"转生等级已设置为10级！")
end

function onTalkZS4(player)
	player:set_zslevel(4)
	player:alert(1,1,"转生等级已设置为4级！")
end

function onTalkZS5(player)
	player:set_zslevel(5)
	player:alert(1,1,"转生等级已设置为5级！")
end

function onTalkZS6(player)
	player:set_zslevel(6)
	player:alert(1,1,"转生等级已设置为级6！")
end

function onTalkZS7(player)
	player:set_zslevel(7)
	player:alert(1,1,"转生等级已设置为7级！")
end

function onTalkZS8(player)
	player:set_zslevel(8)
	player:alert(1,1,"转生等级已设置为8级！")
end

function onTalkZS9(player)
	player:set_zslevel(9)
	player:alert(1,1,"转生等级已设置为9级！")
end

function onTalkZS10(player)
	player:set_zslevel(10)
	player:alert(1,1,"转生等级已设置为10级！")
end

function onTalkZS11(player)
	player:set_zslevel(11)
	player:alert(1,1,"转生等级已设置为11级！")
end

function onTalkZS12(player)
	player:set_zslevel(12)
	player:alert(1,1,"转生等级已设置为12级！")
end

function onTalkZS13(player)
	player:set_zslevel(13)
	player:alert(1,1,"转生等级已设置为13级！")
end

function onTalkZS14(player)
	player:set_zslevel(14)
	player:alert(1,1,"转生等级已设置为14级！")
end


function onTalk100(npc, player)
	talk_tab.title.str = "调试功能(开区"..(server_start_day()+1).."天)";
	player:push_lua_table("talk_npc",util.encode(talk_tab));
end

function onTalkBackToMainPage(player)
	talk_tab.title.str = "调试功能(开区"..(server_start_day()+1).."天)";
	player:push_lua_table("fresh_npc",util.encode(talk_tab));
end

function btnCall(player,data)
	local func = getfenv()["onTalk"..data]
	if func and type(func) == "function" then
		func(player)
	end
end
ckpanel.add_listener("npc.biggm.btnCall",npc.biggm.btnCall)


function onTalkaddSoulExp(player)
	player:set_param(const.PP_UPGRADESOUL_EXP,util.ppn(player,const.PP_UPGRADESOUL_EXP)+5000)
end

function onTalkaddBossJiFen(player)
	player:set_param(const.PP_BOSS_CUR_JIFEN,util.ppn(player,const.PP_BOSS_CUR_JIFEN)+100000000000000)
	player:alert(1,1,"BOSS积分增加1000000！")
end


function onTalkopenChiwooBattle(player)
	autorun.openChiwooBattle()
end

function onTalkcloseChiwooBattle(player)
	autorun.closeChiwooBattle()
end

function onTalkrefreshMoneyBoss(player)
	autorun.startMoneyBoss()
end

function onTalkclearMoneyBoss(player)
	autorun.clearMoneyBoss()
end

function onTalkopenKamuiPrison(player)
end

function onTalkcloseKamuiPrison(player)
end

function onTalkopenAutoAddExp(player)

end

function onTalkcloseAutoAddExp(player)

end

function onTalksendMail(player)
	player:post_mail(os.date(), os.time(), 20001, math.random(1, 10), 19000,1)
end

function onTalksendTextMail(player)
	player:post_mail(os.date(), os.time())
end

function onTalkgetVcoin(player)
	player:add_vcoin_best_enable(2000000,"测试脚本")
end

function onTalkgetWeapon(player)
	player:add_item("测试武器",1,1);
end

function onTalkgetMoney(player)
	player:add_gamemoney(2000000);
end


function onTalkvip(player)
	util.alert_msg(1,"恭喜玩家"..player:get_name().."成为VIP")
	onTalktestAlert(player)
end

function onTalkaddDonate(player)
	player:set_param(const.PP_GUILD_DONATE,util.ppn(player,const.PP_GUILD_DONATE)+10000)
	player:set_param(const.PP_GUILD_DONATE_ALL,util.ppn(player,const.PP_GUILD_DONATE_ALL)+10000)
end

function onTalktestAlert(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",scrollHeight = 340,title = {str="测试公告",fontSize = 24,color=0xFFFFFF,},
		con_tab = {
			[1]  = {mtype="button",	name="vip公告",			funcIndex="vip",			res="btn_transmit-button",	posX=003+84,	posY=350-40+70,	},
			[2]  = {mtype="button",	name="王城公告",		funcIndex="wc",				res="btn_transmit-button",	posX=178+84,	posY=350-40+70,	},
			[3]  = {mtype="button",	name="无法参加活动",	funcIndex="activity",		res="btn_transmit-button",	posX=353+84,	posY=350-40+70,	},
			[4]  = {mtype="button",	name="无法领取奖励",	funcIndex="gift",			res="btn_transmit-button",	posX=528+84,	posY=350-40+70,	},
			[5]  = {mtype="button",	name="无法进入地图",	funcIndex="map",			res="btn_transmit-button",	posX=003+84,	posY=280-40+70,	},
			[6]  = {mtype="button",	name="加入帮会",		funcIndex="guild1",			res="btn_transmit-button",	posX=178+84,	posY=280-40+70,	},
			[7]  = {mtype="button",	name="点击打开",		funcIndex="guild2",			res="btn_transmit-button",	posX=353+84,	posY=280-40+70,	},
			[8]  = {mtype="button",	name="添加好友",		funcIndex="friend",			res="btn_transmit-button",	posX=528+84,	posY=280-40+70,	},
			[9]  = {mtype="button",	name="组队",			funcIndex="team",			res="btn_transmit-button",	posX=003+84,	posY=210-40+70,	},
			[10] = {mtype="button",	name="和平被强p",		funcIndex="p",				res="btn_transmit-button",	posX=178+84,	posY=210-40+70,	},
			[11] = {mtype="button",	name="活动开启",		funcIndex="hd",				res="btn_transmit-button",	posX=353+84,	posY=210-40+70,	},
			[12] = {mtype="button",	name="争霸报名",		funcIndex="zbbm",			res="btn_transmit-button",	posX=528+84,	posY=210-40+70,	},
			[13] = {mtype="button",	name="开始争霸",		funcIndex="kszb",			res="btn_transmit-button",	posX=003+84,	posY=140-40+70,	},
			[14] = {mtype="button",	name="[武林盟主]",		funcIndex="wlzb",			res="btn_transmit-button",	posX=178+84,	posY=140-40+70,	},
			[15] = {mtype="button",	name="关闭争霸",		funcIndex="gbzb",			res="btn_transmit-button",	posX=353+84,	posY=140-40+70,	},
			[16] = {mtype="button",	name="开启夺宝",		funcIndex="dbqb",			res="btn_transmit-button",	posX=528+84,	posY=140-40+70,	},
			[17] = {mtype="button",	name="进入夺宝",		funcIndex="enterdb",		res="btn_transmit-button",	posX=003+84,	posY=070-40+70,	},
			[18] = {mtype="button",	name="返回",			funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=178+84,	posY=070-40+70, haveNext = true},
		},
	}
	player:push_lua_table("talk_npc",util.encode(talkInfos))
end

function onTalkwc(player)
	util.alert_msg(1,"王城已经结束")
	onTalktestAlert(player)
end
function onTalkmap(player)
	util.alert_msg(3,"无法进入地图",player)
	onTalktestAlert(player)
end
function onTalkactivity(player)
	util.alert_msg(3,"无法参加活动",player)
	onTalktestAlert(player)
end
function onTalkgift(player)
	util.alert_msg(3,"无法领取奖品",player)
	onTalktestAlert(player)
end
function onTalkguild1(player)
	util.alert_msg(4,"加入帮会",player)
	onTalktestAlert(player)
end
function onTalkguild2(player)
	local msg = {
		[1] = {"<a href='event:panel_panel_menu'>点击打开菜单面板</a>", "11FF11"},
	}
	-- local msg = {
	-- 	[1] = {"一二三四五六,七八九十一二三四五六七八九十", "00FF00"},
	-- }
	util.alert_msg(4,util.encode(msg),player)
	onTalktestAlert(player)
end

function onTalkfriend(player)
	util.alert_msg(4,"添加好友",player)
	onTalktestAlert(player)
end
function onTalkteam(player)
	local msg = {
		[1] = {"能力大大提升!"},
		[2] = {"<a color='#11ff11' href='event:local_OpenPanel_act22'>我也想领取</a>"},
	}
	util.alert_msg(2,util.encode(msg),1)
	onTalktestAlert(player)
end
function onTalkp(player)
	util.alert_msg(5,"能力大大提升![<a color='#00ff00' href='event:local_OpenPanel_act22'>我也想领取</a>]",1)
	onTalktestAlert(player)
end
function onTalkhd(player)
	local msg = {
		[1] = {"哈哈哈", "ff11ff"},
		[2] = {"<a href='event:panel_panel_menu'>点击打开菜单</a>", "11FF11"},
	}
	util.alert_msg(5,util.encode(msg),player)
	onTalktestAlert(player)
end

function onTalkopenfunc(player)
	-- player:set_param(const.PP_FUNCINDEX_BY_TASK, 30)
	player:set_param(const.PP_FUNCINDEX_BY_LEVEL,40)
	player:set_param(const.PP_FUNCINDEX_BY_OPENSVR,10)
	
	genxxx.updateCocosGuiState(player)
end

function onTalkrecharge(player)
	const.PP_TEMP = 3000
	local talkInfos = {
	func = "npc.biggm.btnCall",
		con_tab = {
			[1]  = {mtype="button",	name="1",		funcIndex="cz1",			res="btn_transmit-button",	posX=003+84,	posY=350-40+70,	},
			[2]  = {mtype="button",	name="10",		funcIndex="cz10",			res="btn_transmit-button",	posX=178+84,	posY=350-40+70,	},
			[3]  = {mtype="button",	name="100",		funcIndex="cz100",			res="btn_transmit-button",	posX=353+84,	posY=350-40+70,	},
			[4]  = {mtype="button",	name="1000",	funcIndex="cz1000",			res="btn_transmit-button",	posX=528+84,	posY=350-40+70,	},
			[5]  = {mtype="button",	name="10000",	funcIndex="cz10000",		res="btn_transmit-button",	posX=003+84,	posY=280-40+70,	},
			[6]  = {mtype="button",	name="返回",	funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=178+84,	posY=280-40+70, haveNext = true},
		},
	}
	player:push_lua_table("talk_npc",util.encode(talkInfos))
end

function onTalkcz1(tplayer)
	tplayer:set_param(const.PP_TEMP, util.ppn(tplayer, const.PP_TEMP)+1)
	player.onCongzi(tplayer, 1, 0, util.ppn(tplayer, const.PP_TEMP))
	onTalkrecharge(tplayer)
end

function onTalkcz10(tplayer)
	tplayer:set_param(const.PP_TEMP, util.ppn(tplayer, const.PP_TEMP)+10)
	player.onCongzi(tplayer, 10, 0, util.ppn(tplayer, const.PP_TEMP))
	onTalkrecharge(tplayer)
end

function onTalkcz100(tplayer)
	tplayer:set_param(const.PP_TEMP, util.ppn(tplayer, const.PP_TEMP)+100)
	player.onCongzi(tplayer, 100, 0, util.ppn(tplayer, const.PP_TEMP))
	onTalkrecharge(tplayer)
end

function onTalkcz1000(tplayer)
	tplayer:set_param(const.PP_TEMP, util.ppn(tplayer, const.PP_TEMP)+1000)
	player.onCongzi(tplayer, 1000, 0, util.ppn(tplayer, const.PP_TEMP))
	onTalkrecharge(tplayer)
end

function onTalkcz10000(tplayer)
	tplayer:set_param(const.PP_TEMP, util.ppn(tplayer, const.PP_TEMP)+10000)
	player.onCongzi(tplayer, 10000, 0, util.ppn(tplayer, const.PP_TEMP))
	onTalkrecharge(tplayer)
end

function onTalkviplevel(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",
		con_tab = {
			[1]  = {mtype="button",	name="0",		funcIndex="vip0",			res="btn_transmit-button",	posX=003+84,	posY=350+30+70,	},
			[2]  = {mtype="button",	name="1",		funcIndex="vip1",			res="btn_transmit-button",	posX=178+84,	posY=350+30+70,	},
			[3]  = {mtype="button",	name="2",		funcIndex="vip2",			res="btn_transmit-button",	posX=353+84,	posY=350+30+70,	},
			[4]  = {mtype="button",	name="3",		funcIndex="vip3",			res="btn_transmit-button",	posX=528+84,	posY=350+30+70,	},
			[5]  = {mtype="button",	name="4",		funcIndex="vip4",			res="btn_transmit-button",	posX=003+84,	posY=280+30+70,	},
			[6]  = {mtype="button",	name="5",		funcIndex="vip5",			res="btn_transmit-button",	posX=178+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="6",		funcIndex="vip6",			res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[8]  = {mtype="button",	name="7",		funcIndex="vip7",			res="btn_transmit-button",	posX=528+84,	posY=280+30+70,	},
			[9]  = {mtype="button",	name="8",		funcIndex="vip8",			res="btn_transmit-button",	posX=003+84,	posY=210+30+70,	},
			[10] = {mtype="button",	name="9",		funcIndex="vip9",			res="btn_transmit-button",	posX=178+84,	posY=210+30+70,	},
			[11] = {mtype="button",	name="10",		funcIndex="vip10",			res="btn_transmit-button",	posX=353+84,	posY=210+30+70,	},
			[12] = {mtype="button",	name="返回",	funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=528+84,	posY=210+30+70, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalkkqsj(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",
		con_tab = {
			[1]  = {mtype="button",	name="1",		funcIndex="kqsj1",				res="btn_transmit-button",	posX=003+84,	posY=350+30+70,	},
			[2]  = {mtype="button",	name="2",		funcIndex="kqsj2",				res="btn_transmit-button",	posX=178+84,	posY=350+30+70,	},
			[3]  = {mtype="button",	name="3",		funcIndex="kqsj3",				res="btn_transmit-button",	posX=353+84,	posY=350+30+70,	},
			[4]  = {mtype="button",	name="4",		funcIndex="kqsj4",				res="btn_transmit-button",	posX=528+84,	posY=350+30+70,	},
			[5]  = {mtype="button",	name="5",		funcIndex="kqsj5",				res="btn_transmit-button",	posX=003+84,	posY=280+30+70,	},
			[6]  = {mtype="button",	name="6",		funcIndex="kqsj6",				res="btn_transmit-button",	posX=178+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="7",		funcIndex="kqsj7",				res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[8]  = {mtype="button",	name="8",		funcIndex="kqsj8",				res="btn_transmit-button",	posX=528+84,	posY=280+30+70,	},
			[9]  = {mtype="button",	name="返回",	funcIndex="BackToMainPage",		res="btn_transmit-button",	posX=003+84,	posY=210+30+70, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalkkqsj1(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time()));
	player:alert(1,1,"开区时间已被初始化为1!")
end

function onTalkkqsj2(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time())-1*24*60*60);
	player:alert(1,1,"开区时间已被初始化为2!")
end

function onTalkkqsj3(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time())-2*24*60*60);
	player:alert(1,1,"开区时间已被初始化为3!")
end

function onTalkkqsj4(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time())-3*24*60*60);
	player:alert(1,1,"开区时间已被初始化为4!")
end

function onTalkkqsj5(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time())-4*24*60*60);
	player:alert(1,1,"开区时间已被初始化为5!")
end

function onTalkkqsj6(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time())-5*24*60*60);
	player:alert(1,1,"开区时间已被初始化为6!")
end

function onTalkkqsj7(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time())-6*24*60*60);
	player:alert(1,1,"开区时间已被初始化为7!")
end

function onTalkkqsj8(player)
	reset_server_merge2()
	server.set_var("server_start_time",tostring(os.time())-7*24*60*60);
	player:alert(1,1,"开区时间已被初始化为8!")
end

function onTalkvip0(player)
	setPlayerVip(player, 0)
	--player:set_param(const.PP_VIP_LEVEL,0)
	--gui.moduleRedPoint.checkVipUsable(player)
end

function onTalkvip1(player)
	setPlayerVip(player, 1)
	--player:set_param(const.PP_VIP_LEVEL,1)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip2(player)
	setPlayerVip(player, 2)
	--player:set_param(const.PP_VIP_LEVEL,2)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip3(player)
	setPlayerVip(player, 3)
	--player:set_param(const.PP_VIP_LEVEL,3)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip4(player)
	setPlayerVip(player, 4)
	--player:set_param(const.PP_VIP_LEVEL,4)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip5(player)
	setPlayerVip(player, 5)
	--player:set_param(const.PP_VIP_LEVEL,5)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip6(player)
	setPlayerVip(player, 6)
	--player:set_param(const.PP_VIP_LEVEL,6)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip7(player)
	setPlayerVip(player, 7)
	--player:set_param(const.PP_VIP_LEVEL,7)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip8(player)
	setPlayerVip(player, 8)
	--player:set_param(const.PP_VIP_LEVEL,8)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip9(player)
	setPlayerVip(player, 9)
	--player:set_param(const.PP_VIP_LEVEL,9)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip10(player)
	setPlayerVip(player, 10)
	--player:set_param(const.PP_VIP_LEVEL,10)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip11(player)
	setPlayerVip(player, 11)
	--player:set_param(const.PP_VIP_LEVEL,11)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip12(player)
	setPlayerVip(player, 12)
	--player:set_param(const.PP_VIP_LEVEL,12)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip13(player)
	setPlayerVip(player, 13)
	--player:set_param(const.PP_VIP_LEVEL,13)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip14(player)
	setPlayerVip(player, 14)
	--player:set_param(const.PP_VIP_LEVEL,14)
	--gui.moduleRedPoint.checkVipUsable(player)
end
function onTalkvip15(player)
	setPlayerVip(player, 15)
	--player:set_param(const.PP_VIP_LEVEL,15)
	--gui.moduleRedPoint.checkVipUsable(player)
end

function setPlayerVip(player, level)
	player:set_param(const.PP_VIP_LEVEL,level)
    player:set_status(39,86400,level,1,1)
    player:set_model(5,level)
    gui.moduleRedPoint.checkVipUsable(player)
end


function onTalkzbck(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",
		con_tab = {
			[1]  = {mtype="button",	name="30",		funcIndex="zbck30",			res="btn_transmit-button",	posX=003+84,	posY=350+30+70,	},
			[2]  = {mtype="button",	name="40",		funcIndex="zbck40",			res="btn_transmit-button",	posX=178+84,	posY=350+30+70,	},
			[3]  = {mtype="button",	name="50",		funcIndex="zbck50",			res="btn_transmit-button",	posX=353+84,	posY=350+30+70,	},
			[4]  = {mtype="button",	name="60",		funcIndex="zbck60",			res="btn_transmit-button",	posX=528+84,	posY=350+30+70,	},
			[5]  = {mtype="button",	name="65",		funcIndex="zbck65",			res="btn_transmit-button",	posX=003+84,	posY=280+30+70,	},
			[6]  = {mtype="button",	name="70",		funcIndex="zbck70",			res="btn_transmit-button",	posX=178+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="75",		funcIndex="zbck75",			res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[8]  = {mtype="button",	name="80",		funcIndex="zbck80",			res="btn_transmit-button",	posX=528+84,	posY=280+30+70,	},
			[9]  = {mtype="button",	name="1z",		funcIndex="zbck1z",			res="btn_transmit-button",	posX=003+84,	posY=210+30+70,	},
			[10] = {mtype="button",	name="2z",		funcIndex="zbck2z",			res="btn_transmit-button",	posX=178+84,	posY=210+30+70,	},
			[11] = {mtype="button",	name="3z",		funcIndex="zbck3z",			res="btn_transmit-button",	posX=353+84,	posY=210+30+70,	},
			[12] = {mtype="button",	name="5z",		funcIndex="zbck5z",			res="btn_transmit-button",	posX=528+84,	posY=210+30+70,	},
			[13] = {mtype="button",	name="7z",		funcIndex="zbck7z",			res="btn_transmit-button",	posX=003+84,	posY=140+30+70,	},
			[14] = {mtype="button",	name="9z",		funcIndex="zbck9z",			res="btn_transmit-button",	posX=178+84,	posY=140+30+70,	},
			[15] = {mtype="button",	name="11z",		funcIndex="zbck11z",		res="btn_transmit-button",	posX=353+84,	posY=140+30+70,	},
			[16] = {mtype="button",	name="返回",	funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=528+84,	posY=140+30+70, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalklvup(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",title = {str="升级",fontSize = 24,color=0xFFFFFF,},
		con_tab = {
			[1]  = {mtype="button",	name="直升30",		funcIndex="lvup30",			res="btn_transmit-button",	posX=003+84,	posY=350+30+70,	},
			[2]  = {mtype="button",	name="直升40",		funcIndex="lvup40",			res="btn_transmit-button",	posX=178+84,	posY=350+30+70,	},
			[3]  = {mtype="button",	name="直升50",		funcIndex="lvup50",			res="btn_transmit-button",	posX=353+84,	posY=350+30+70,	},
			[4]  = {mtype="button",	name="直升60",		funcIndex="lvup60",			res="btn_transmit-button",	posX=528+84,	posY=350+30+70,	},
			[5]  = {mtype="button",	name="直升65",		funcIndex="lvup65",			res="btn_transmit-button",	posX=003+84,	posY=280+30+70,	},
			[6]  = {mtype="button",	name="直升70",		funcIndex="lvup70",			res="btn_transmit-button",	posX=178+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="直升75",		funcIndex="lvup75",			res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[8]  = {mtype="button",	name="直升80",		funcIndex="lvup80",			res="btn_transmit-button",	posX=528+84,	posY=280+30+70,	},
			[9]  = {mtype="button",	name="直升90",		funcIndex="lvup90",			res="btn_transmit-button",	posX=003+84,	posY=210+30+70,	},
			[10] = {mtype="button",	name="返回",		funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=178+84,	posY=210+30+70, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalkwupin(player)
	local talkInfos = {
	func = "npc.biggm.btnCall",
		con_tab = {
			[1]  = {mtype="button",	name="10绑坐骑丹",			funcIndex="bzqd10",			res="btn_transmit-button",	posX=003+84,	posY=350+30+70,	},
			[2]  = {mtype="button",	name="10非绑坐骑丹",		funcIndex="zqd10",			res="btn_transmit-button",	posX=178+84,	posY=350+30+70,	},
			[3]  = {mtype="button",	name="100绑坐骑丹",			funcIndex="bzqd100",		res="btn_transmit-button",	posX=353+84,	posY=350+30+70,	},
			[4]  = {mtype="button",	name="100非绑坐骑丹",		funcIndex="zqd100",			res="btn_transmit-button",	posX=528+84,	posY=350+30+70,	},
			[5]  = {mtype="button",	name="300绑坐骑丹",			funcIndex="bzqd300",		res="btn_transmit-button",	posX=003+84,	posY=280+30+70,	},
			[6]  = {mtype="button",	name="300非绑坐骑丹",		funcIndex="zqd300",			res="btn_transmit-button",	posX=178+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="10灵羽",				funcIndex="ly10",			res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[8]  = {mtype="button",	name="100灵羽",				funcIndex="ly100",			res="btn_transmit-button",	posX=528+84,	posY=280+30+70,	},
			[9]  = {mtype="button",	name="1000灵羽",			funcIndex="ly1000",			res="btn_transmit-button",	posX=003+84,	posY=210+30+70,	},
			[10] = {mtype="button",	name="返回",				funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=178+84,	posY=210+30+70, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalkzbck30(player) onTalkzbckall(player,"30j"); onTalkzbck(player);end
function onTalkzbck40(player) onTalkzbckall(player,"40j"); onTalkzbck(player);end
function onTalkzbck50(player) onTalkzbckall(player,"50j"); onTalkzbck(player);end
function onTalkzbck60(player) onTalkzbckall(player,"60j"); onTalkzbck(player);end
function onTalkzbck65(player) onTalkzbckall(player,"65j"); onTalkzbck(player);end
function onTalkzbck70(player) onTalkzbckall(player,"70j"); onTalkzbck(player);end
function onTalkzbck75(player) onTalkzbckall(player,"75j"); onTalkzbck(player);end
function onTalkzbck80(player) onTalkzbckall(player,"80j"); onTalkzbck(player);end
function onTalkzbck1z(player) onTalkzbckall(player,"1z"); onTalkzbck(player);end
function onTalkzbck2z(player) onTalkzbckall(player,"2z"); onTalkzbck(player);end
function onTalkzbck3z(player) onTalkzbckall(player,"3z"); onTalkzbck(player);end
function onTalkzbck5z(player) onTalkzbckall(player,"5z"); onTalkzbck(player);end
function onTalkzbck7z(player) onTalkzbckall(player,"7z"); onTalkzbck(player);end
function onTalkzbck9z(player) onTalkzbckall(player,"9z"); onTalkzbck(player);end
function onTalkzbck11z(player) onTalkzbckall(player,"11z"); onTalkzbck(player);end



function onTalklvup30(player) onTalklvupall(player,30); onTalklvup(player);end
function onTalklvup40(player) onTalklvupall(player,40); onTalklvup(player);end
function onTalklvup50(player) onTalklvupall(player,50); onTalklvup(player);end
function onTalklvup60(player) onTalklvupall(player,60); onTalklvup(player);end
function onTalklvup65(player) onTalklvupall(player,65); onTalklvup(player);end
function onTalklvup70(player) onTalklvupall(player,70); onTalklvup(player);end
function onTalklvup75(player) onTalklvupall(player,75); onTalklvup(player);end
function onTalklvup80(player) onTalklvupall(player,80); onTalklvup(player);end
function onTalklvup90(player) onTalklvupall(player,90); onTalklvup(player);end

function onTalkbzqd10(player) 	onTalkwupinall(player,"bzqd10"); onTalkwupin(player);	end
function onTalkbzqd100(player) 	onTalkwupinall(player,"bzqd100"); onTalkwupin(player);	end
function onTalkbzqd300(player) 	onTalkwupinall(player,"bzqd300"); onTalkwupin(player);	end
function onTalkzqd10(player) 	onTalkwupinall(player,"zqd10"); onTalkwupin(player);	end
function onTalkzqd100(player) 	onTalkwupinall(player,"zqd100"); onTalkwupin(player);	end
function onTalkzqd300(player) 	onTalkwupinall(player,"zqd300"); onTalkwupin(player);	end

function onTalkly10(player) 	onTalkwupinall(player,"ly10"); onTalkwupin(player);		end
function onTalkly100(player) 	onTalkwupinall(player,"ly100"); onTalkwupin(player);	end
function onTalkly1000(player) 	onTalkwupinall(player,"ly1000"); onTalkwupin(player);	end

function onTalkzbckall(player,lv)
	local zb = {
	["30j"] = {"炼狱斧","魔杖","天尊银蛇","战士铠甲","战士铠衣","法师魔袍","法师魔衣","道士长袍","道士长衣",};
	["40j"] = {"审判之杖","骨玉法杖","无极刀","天圣战甲","天圣战衣","魔王披风","魔王羽衣","天师道袍","天师道衣",};
	
	["50j"] = {"战将屠龙","法灵龙牙","道尊灵扇","战将铠甲","战将铠衣","法灵魔袍","法灵魔衣","道尊长袍","道尊长衣",};
	
	["60j"] = {"武魂战刃","魔魂法杖","玄魂道剑","武魂战甲","武魂战衣","魔魂法袍","魔魂法衣","玄魂道袍","玄魂道衣",};
	["70j"] = {"战圣之刃","魔圣法仗","道圣之扇","战圣铠甲","战圣铠衣","魔圣魔甲","魔圣魔衣","道圣道甲","道圣道衣",};

	["80j"] = {"噬圣刀","噬魔杖","噬魂扇","噬圣战甲","噬圣战衣","噬魔法袍","噬魔法衣","噬魂道甲","噬魂道衣",
	          };
	["1z"] = {"惊龙斩","惊世杖","惊天剑","惊龙战甲","惊龙战衣","惊世魔甲","惊世魔衣","惊天道甲","惊天道衣",
	           };
	["2z"] = {"圣武斩","圣魔仗","圣灵剑","圣武战甲","圣武战衣","圣魔法袍","圣魔法衣","圣灵道袍","圣灵道衣",
	           };
	["3z"] = {"天神刀","魔神杖","尊神剑","天神盔","魔神盔","尊神盔","天神项链","魔神项链","尊神项链","天神护腕","魔神护腕","尊神护腕","天神戒指","魔神戒指","尊神戒指",
	           "天神腰带","魔神腰带","尊神腰带","天神靴","魔神靴","尊神靴","天神战甲","天神战衣","魔神羽衣","魔神法袍","尊神道甲","尊神道衣",};
	["7z"] = {"玄武天惊刀","朱雀九州杖","青龙雷鸣剑","玄武天惊盔","朱雀九州盔","青龙雷鸣盔","玄武天惊项链","朱雀九州项链","青龙雷鸣项链","玄武天惊护腕","朱雀九州护腕","青龙雷鸣护腕","玄武天惊戒指","朱雀九州戒指","青龙雷鸣戒指",
	           "玄武天惊腰带","朱雀九州腰带","青龙雷鸣腰带","玄武天惊靴","朱雀九州靴","青龙雷鸣靴","玄武天惊甲","玄武天惊衣","朱雀九州甲","朱雀九州衣","青龙雷鸣甲","青龙雷鸣衣",};
	["5z"] = {"潜龙震世斩","狂龙霸天杖","御龙惊风剑","潜龙震世盔","狂龙霸天盔","御龙惊风盔","潜龙震世项链","狂龙霸天项链","御龙惊风项链","潜龙震世护腕","狂龙霸天护腕","御龙惊风护腕","潜龙震世戒指","狂龙霸天戒指","御龙惊风戒指",
	           "潜龙震世腰带","狂龙霸天腰带","御龙惊风腰带","潜龙震世靴","狂龙霸天靴","御龙惊风靴","潜龙震世甲","潜龙震世衣","狂龙霸天甲","狂龙霸天衣","御龙惊风甲","御龙惊风衣",};
	["9z"] = {"无极真皇剑","九转魔王杖","万象至尊斩","无极真皇盔","九转魔王盔","万象至尊盔","无极真皇项链","九转魔王项链","万象至尊项链","无极真皇护腕","九转魔王护腕","万象至尊护腕","无极真皇戒指","九转魔王戒指","万象至尊戒指",
	           "无极真皇腰带","九转魔王腰带","万象至尊腰带","无极真皇靴","九转魔王靴","万象至尊靴","无极真皇甲","无极真皇衣","九转魔王甲","九转魔王衣","万象至尊甲","万象至尊衣",}; 
	["11z"] = {"神龙尊武斩","神龙魔圣仗","神龙道尊剑","神龙尊武甲","神龙尊武衣","神龙魔圣袍","神龙魔圣衣","神龙道尊甲","神龙道尊衣",
	          };     
	};

	for i=1,#zb[lv] do
		player:add_item(zb[lv][i],1);
	end
end


function onTalklvupall(player,lv)
	player:set_level(lv);
end

function onTalkwupinall( player,lv )
	local wu ={
		["bzqd10"] = 	{name = "坐骑丹",		bind = 1, 	num=10 };
		["bzqd100"] = 	{name = "坐骑丹",		bind = 1, 	num=100 };
		["bzqd300"] = 	{name = "坐骑丹",		bind = 1, 	num=300 };
		["zqd10"] = 	{name = "坐骑丹",		bind = 0, 	num=10 };
		["zqd100"] = 	{name = "坐骑丹",		bind = 0, 	num=100 };
		["zqd300"] = 	{name = "坐骑丹",		bind = 0, 	num=300 };
		["ly10"] = 		{name = "灵羽",		bind = 1, 	num=10 };
		["ly100"] = 	{name = "灵羽",		bind = 1, 	num=100 };
		["ly1000"] = 	{name = "灵羽",		bind = 1, 	num=1000 };

	}
	player:add_item(wu[lv].name,wu[lv].num,wu[lv].bind);

end

function onTalkwlzb(player)
	npc.wlzb.onWlzb(player,"attend")
end

function onTalkzbbm(player)
	autorun.wlzbgg()
end

function onTalkkszb(player)
	autorun.wlzbkq()
end

function onTalkgbzb(player)
	autorun.wlzbgb()
end

function onTalkdbqb(player)
	autorun.dbstart()
end

function onTalkenterdb(player)
	npc.dbqb.onDuobao(player,"attend")
end

function onTalkaddQLSCBL(player)
	local typeTable = {
		"Level","Mount","Wing","Reborn","Medal","Upgrade","Jewel","VIP",
	}
	for k,v in pairs(typeTable) do
		for i=1,4 do
			server.set_var("DashListName"..v..i, "");
			server.set_var("DashListValue"..v..i, "");
			server.set_var("DashListSeedName"..v..i, "");
			server.set_var("DashListJob"..v..i, "");
		end
	end
	util.alert_msg(4, "清理成功", player);
end

function onTalkclearBag(player)
	-- player:danger_clear_item()
	--清空背包
	removeAllItems(player)

end

function removeAllItems(player)
	local num,typeid,ret
	for i=0,75 do
		num,typeid = getItemNum(player,i,1)--背包
		if typeid then
			player:remove_item_by_type_pos(typeid, i,num and num or 1)
		end
	end
	for i=1000,1060 do
		num,typeid = getItemNum(player,i,100) --仓库
		if typeid then
			ret = player:remove_item_by_type_pos(typeid, i, num and num or 1)
		end
	end
	for i=3000,3300 do
		num,typeid = getItemNum(player,i,10000)--寻宝背包
		if typeid then
			ret = player:remove_item_by_type_pos(typeid, i, num and num or 1)
		end
	end
end

function getItemNum(player,pos,flag)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(pos);
	local num
	if ittype_id and tonumber(ittype_id)>0  then
		num=player:num_item(ittype_id,flag)
	end 
	return num,ittype_id
end

function onTalkopenBqyz(player)
	autorun.yxbiqistart();
	gui.PanelActivityGuide.startFlyTarget(player,"夜袭比奇");
end

function onTalkcloseBqyz(player)
	autorun.yxbiqiclose();
end

function onTalkguildLevelChange(player)
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	local talkInfos = {
		func = "npc.biggm.btnCall",
		con_tab = {
			[1]  = {mtype="button",	name="修改为2级帮会",		funcIndex="guildLv2",		res="btn_transmit-button",	posX=003+84,	posY=350+30+70,	},
			[2]  = {mtype="button",	name="修改为3级帮会",		funcIndex="guildLv3",		res="btn_transmit-button",	posX=178+84,	posY=350+30+70,	},
			[3]  = {mtype="button",	name="修改为4级帮会",		funcIndex="guildLv4",		res="btn_transmit-button",	posX=353+84,	posY=350+30+70,	},
			[4]  = {mtype="button",	name="修改为5级帮会",		funcIndex="guildLv5",		res="btn_transmit-button",	posX=528+84,	posY=350+30+70,	},
			[5]  = {mtype="button",	name="修改为6级帮会",		funcIndex="guildLv6",		res="btn_transmit-button",	posX=003+84,	posY=280+30+70,	},
			[6]  = {mtype="button",	name="增加1000贡献",		funcIndex="addguildpt1",	res="btn_transmit-button",	posX=178+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="增加10000贡献",		funcIndex="addguildpt2",	res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[7]  = {mtype="button",	name="增加100000贡献",		funcIndex="addguildpt3",	res="btn_transmit-button",	posX=353+84,	posY=280+30+70,	},
			[8] = {mtype="button",	name="返回",				funcIndex="BackToMainPage",	res="btn_transmit-button",	posX=178+84,	posY=210+30+70, haveNext = true},
		},
	}
	player:push_lua_table("fresh_npc",util.encode(talkInfos))
end

function onTalkguildLv2(player)
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	guild:set_guild_level(2)
	player:guild_exe("gui.PanelGuild.freshGuildState")
	server.info(10000,0,"修改帮会等级为"..guild:get_guild_level().."级!")
end

function onTalkguildLv3(player)
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	guild:set_guild_level(3)
	player:guild_exe("gui.PanelGuild.freshGuildState")
	server.info(10000,0,"修改帮会等级为"..guild:get_guild_level().."级!")
end

function onTalkguildLv4(player)

	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	guild:set_guild_level(4)
	player:guild_exe("gui.PanelGuild.freshGuildState")
	server.info(10000,0,"修改帮会等级为"..guild:get_guild_level().."级!")
end

function onTalkguildLv5(player)

	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	guild:set_guild_level(5)
	player:guild_exe("gui.PanelGuild.freshGuildState")
	server.info(10000,0,"修改帮会等级为"..guild:get_guild_level().."级!")
end

function onTalkguildLv6(player)
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	guild:set_guild_level(6)
	player:guild_exe("gui.PanelGuild.freshGuildState")
	server.info(10000,0,"修改帮会等级为"..guild:get_guild_level().."级!")
end

function onTalkaddguildpt1(player)
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	player:set_param(const.PP_GUILD_DONATE,util.ppn(player,const.PP_GUILD_DONATE)+1000)
	player:set_param(const.PP_GUILD_DONATE_ALL,util.ppn(player,const.PP_GUILD_DONATE_ALL)+1000)
	guild:set_guild_point(guild:get_guild_point()+1000)
	player:alert(1,1,"您增加1000帮会财富");
end

function onTalkaddguildpt2(player)
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	player:set_param(const.PP_GUILD_DONATE,util.ppn(player,const.PP_GUILD_DONATE)+10000)
	player:set_param(const.PP_GUILD_DONATE_ALL,util.ppn(player,const.PP_GUILD_DONATE_ALL)+10000)
	guild:set_guild_point(guild:get_guild_point()+10000)
	player:alert(1,1,"您增加10000帮会财富");
end

function onTalkaddguildpt3(player)
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	player:set_param(const.PP_GUILD_DONATE,util.ppn(player,const.PP_GUILD_DONATE)+100000)
	player:set_param(const.PP_GUILD_DONATE_ALL,util.ppn(player,const.PP_GUILD_DONATE_ALL)+100000)
	guild:set_guild_point(guild:get_guild_point()+100000)
	player:alert(1,1,"您增加100000帮会财富");
end

--清空元神塔排名
function onTalkresetYuanShen(player)
	for i=1,50 do
		server.set_var("yuanshenChart"..i,"");
	end
end

