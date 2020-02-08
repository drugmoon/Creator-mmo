module(..., package.seeall)

function reqRedPointInfo(player)
	checkAchieveUsable(player);
	checkXunZhangUsable(player);
	
	checkZhuanShengUsable(player);
	checkInnerPowerUsable(player);
	checkXuefuUsable(player);
	checkHudunUsable(player);
	checkLangYaUsable(player);
	checkLongXinUsable(player);

	checkVipUsable(player);

	checkFirstRechargeUsable(player);

	checkMakeExpUsable(player);
	checkAwardSignUsable(player);
	checkAwardOnlineUsable(player);
	checkAwardFifteenUsable(player);
	checkOpenSvrSaleUsable(player);
	checkOpenSvrBossUsable(player);
	checkWordAcitivityUsable(player);
	checkMarsUsable(player);
	checkExploitUsable(player);
	checkBreakUpUsable(player);
	checkOfficialPostUsable(player);
	checkOfficialChopUsable(player);

	checkLevelRanking(player);
	checkWingRanking(player);
	checkLongXinRanking(player);
	checkLangYaRanking(player);
	checkConsignUsable(player);

	checkLoginYouLi(player);
	checkRechargeYouLi(player);
	checkDailyRecharge(player);

	checkHeQuAct(player);

	checkGuildHongBao(player);
	checkEventsUsable(player);
	checkInvestUsable(player);
	checkDiceUsable(player);

	checkKfLeiChong(player)

	checkKfLxRecharge(player)
	checkSevenDay(player)

	checkDownLoadUsable(player)
	checkLianHunUsable(player)
	checkPinTuUsable(player)
	checkSkillUsable(player)
end

----------------------------------成就领取----------------------------------
function checkAchieveUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10071) then
		pushRedPointMsg(player, {lv = 2036, index = 1, visible = gui.PanelAchieve.checkAchieveUsable(player)})
	end
end

----------------------------------转生----------------------------------
function checkZhuanShenUsable(player,vis)
	if gui.moduleGuiButton.checkFuncOpened(player, 10014) then
		pushRedPointMsg(player, {lv = 47, index = 1, visible = vis })
	end
end

--下载有礼
function checkDownLoadUsable(player)
	pushRedPointMsg(player, {lv = 2084, index = 1, visible = gui.PanelDownLoad.checkRedPointDownLoad(player) })
end

--首充检测
function checkFirstRechargeUsable(player)
	pushRedPointMsg(player, {lv = 1011, index = 1, visible = gui.PanelFirstRecharge.checkRedPointRecharge(player) })
	pushRedPointMsg(player, {lv = 1012, index = 1, visible = gui.PanelFirstRecharge.checkRedPointAward(player) })
end

--经验炼制
function checkMakeExpUsable(player)
	pushRedPointMsg(player, {lv = 1021, index = 1, visible = gui.PanelMakeExp.checkRedPoint(player) })
end

--奖励大厅--签到
function checkAwardSignUsable(player)
	pushRedPointMsg(player, {lv = 1031, index = 1, visible = gui.AwardHall_daily.checkRedPoint(player) })
end

--奖励大厅--在线
function checkAwardOnlineUsable(player)
	pushRedPointMsg(player, {lv = 1032, index = 1, visible = gui.AwardHall_online.checkRedPoint(player) })
end

--奖励大厅--15日
function checkAwardFifteenUsable(player)
	pushRedPointMsg(player, {lv = 1033, index = 1, visible = gui.AwardHall_fifteen.checkRedPoint(player) })
end

--开服活动--特惠
function checkOpenSvrSaleUsable(player)
	pushRedPointMsg(player, {lv = 1061, index = 1, visible = gui.AwardHall_tehui.checkRedPoint(player) })
end

--开服活动--全民boss
function checkOpenSvrBossUsable(player)
	local isShow = gui.AwardHall_boss.checkRedPoint(player)
	pushRedPointMsg(player, {lv = 1062, index = 1, visible = isShow })
	gui.PanelActivities.updateTabRedShow(player,2,isShow)
end

--世界活动
function checkWordAcitivityUsable(player)
	pushRedPointMsg(player, {lv = 1081, index = 1, visible = gui.PanelActivityGuide.checkRedPoint(player) })
end

--护卫
function checkMarsUsable(player)
	pushRedPointMsg(player, {lv = 1091, index = 1, visible = gui.PanelHuWei.checkRedPoint(player) })
end

--功勋
function checkExploitUsable(player)
	pushRedPointMsg(player, {lv = 1101, index = 1, visible = gui.PanelGongXun.checkRedPoint(player) })
end

--闯天关
function checkBreakUpUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 20101) then
		pushRedPointMsg(player, {lv = 1111, index = 1, visible = gui.PanelBreakUp.checkBreakUpRedPoint(player) })
	end
end

--官位
function checkOfficialPostUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10041) then
		pushRedPointMsg(player, {lv = 2011, index = 1, visible = gui.PanelOfficial.checkRedPointGw(player) })
	end
end

--官印
function checkOfficialChopUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10041) then
		pushRedPointMsg(player, {lv = 2012, index = 1, visible = gui.PanelOfficial.checkRedPointGy(player) })
	end
end

-------------------内功检测--------------------------------------------
function checkInnerPowerUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10012) then
		pushRedPointMsg(player, {lv = 2021, index = 1, visible = gui.PanelInnerPower.checkRedPoint(player) })
	end
end

-------------------转生检测--------------------------------------------
function checkZhuanShengUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10014) then
		pushRedPointMsg(player, {lv = 2023, index = 1, visible = gui.PanelZhuanSheng.checkRedPoint(player) })
	end
end

--每日充值
function checkDailyRecharge(player)
	pushRedPointMsg(player, {lv = 2081, index = 1, visible = (not gui.PanelFirstRecharge.checkRedPointRecharge(player) and not gui.PanelDailyRecharge.getAwardsFinish(player)) })
end
--------寄售----------------------------------------------
function checkConsignVcoin( player )
	if gui.moduleGuiButton.checkFuncOpened(player, 10121) then
		if player:get_consignment_vcoin()>0  then 
			return true
		else
			return false
		end 
	end
end
function checkConsignUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10121) then
		local visible = checkConsignVcoin(player)
		pushRedPointMsg(player, {lv = 2062, index = 1, visible =visible})
		player:push_lua_table("PanelConsignRedPoint",util.encode({visible=visible}))
	end
end
login.add_login_listener(checkConsignUsable)
----------------------------------玉佩----------------------------------
function checkXuefuUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10031) then
		pushRedPointMsg(player, {lv = 2031, index = 1, visible = gui.PanelFurnace.checkXuefuUsable(player)})
	end
end
----------------------------------护盾----------------------------------
function checkHudunUsable(player) 
	if gui.moduleGuiButton.checkFuncOpened(player, 10032) then
		pushRedPointMsg(player, {lv = 2032, index = 1, visible = gui.PanelFurnace.checkHudunUsable(player)})
	end
end
----------------------------------龙心----------------------------------
function checkLongXinUsable(player) 
	if gui.moduleGuiButton.checkFuncOpened(player, 10033) then
		pushRedPointMsg(player, {lv = 2033, index = 1, visible = gui.PanelFurnace.checkLongXinUsable(player)})
	end
end

----------------------------------狼牙----------------------------------
function checkLangYaUsable(player) 
	if gui.moduleGuiButton.checkFuncOpened(player, 10034) then
		pushRedPointMsg(player, {lv = 2034, index = 1, visible = gui.PanelFurnace.checkLangYaUsable(player)})
	end
end
----------------------------------勋章----------------------------------
function checkXunZhangUsable(player) 
	if gui.moduleGuiButton.checkFuncOpened(player, 10071) then
		pushRedPointMsg(player, {lv = 2035, index = 1, visible = gui.PanelFurnace.checkXunZhangUsable(player)})
	end
end
----------------------------------vip----------------------------------
function checkVipUsable(player)
	pushRedPointMsg(player, {lv = 3011, index = 1, visible = gui.PanelVip.checkRedPoint(player)})
end

----------------------------------等级竞技----------------------------------
function checkLevelRanking(player)
	local isShow = gui.AwardHall_jingji.checkLevelRedPoint(player)
	pushRedPointMsg(player, {lv = 1112, index = 1, visible = isShow})
	gui.PanelActivities.updateTabRedShow(player,3,isShow)
end

----------------------------------神翼竞技----------------------------------
function checkWingRanking(player)
	local isShow = gui.AwardHall_jingji.checkWingRedPoint(player)
	pushRedPointMsg(player, {lv = 1113, index = 1, visible = isShow})
	gui.PanelActivities.updateTabRedShow(player,4,isShow)
end

----------------------------------龙心竞技----------------------------------
function checkLongXinRanking(player)
	local isShow = gui.AwardHall_jingji.checkLxRedPoint(player)
	pushRedPointMsg(player, {lv = 1114, index = 1, visible = isShow})
	gui.PanelActivities.updateTabRedShow(player,5,isShow)
end

----------------------------------狼牙竞技----------------------------------
function checkLangYaRanking(player)
	local isShow = gui.AwardHall_jingji.checkLyRedPoint(player)
	pushRedPointMsg(player, {lv = 1115, index = 1, visible = isShow })
	gui.PanelActivities.updateTabRedShow(player,6,isShow)
end

----------------------------------开服活动-累计充值----------------------------------
function checkKfLeiChong(player)
	local isShow = gui.AwardHall_leichong.checkRedPoint(player)
	pushRedPointMsg(player, {lv = 1118, index = 1, visible = isShow })
	gui.PanelActivities.updateTabRedShow(player,8,isShow)
end

----------------------------------开服活动-连续充值----------------------------------
function checkKfLxRecharge(player)
	local isShow = gui.AwardHall_lxrecharge.checkRedPoint(player)
	pushRedPointMsg(player, {lv = 1119, index = 1, visible = isShow })
	gui.PanelActivities.updateTabRedShow(player,9,isShow)
end

----------------------------------七日狂欢----------------------------------
function checkSevenDay(player)
	local isShow = gui.AwardHall_sevenday.checkRedPoint(player)
	pushRedPointMsg(player, {lv = 1120, index = 1, visible = isShow })
	gui.PanelActivities.updateTabRedShow(player,10,isShow)
end


----------------------------------登录有礼----------------------------------
function checkLoginYouLi(player)
	pushRedPointMsg(player, {lv = 1116, index = 1, visible = gui.PanelCelebrationAct.checkRedPointLogin(player) })
end

----------------------------------充值有礼----------------------------------
function checkRechargeYouLi(player)
	pushRedPointMsg(player, {lv = 1117, index = 1, visible = gui.PanelCelebrationAct.checkRedPointRecharge(player) })
end


----------------------------------合区活动----------------------------------
function checkHeQuAct(player)
	local isShow = false
	if server_merge_day()>=0 and server_merge_day()<=4 then
		local result1 = gui.PanelHeQuAct.checkRedPointLangYa(player)
		local result2 = gui.PanelHeQuAct.checkRedPointLongXin(player)
		local result3 = gui.PanelHeQuAct.checkRedPointRecharge(player)
		if result1 or result2 or result3 then
			isShow = true
		end
	end
	pushRedPointMsg(player, {lv = 2082, index = 1, visible = isShow })
end

----------------------------------帮会红包----------------------------------
function checkGuildHongBao(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10101) then
		local visible = gui.PanelGuild.checkGuildHongBao(player);
		pushRedPointMsg(player, {lv = 2091, index = 1, visible = visible});
		player:push_lua_table("PanelGuild",util.encode({cmd = "redPacket", showRedPoint = visible}));
	end
end

----------------------------------精彩活动----------------------------------
function checkEventsUsable(player)
	pushRedPointMsg(player, {lv = 1121, index = 1, visible = gui.PanelJingCaiAct.checkJingCaiRedPoint(player)});
end
----------------------------------投资计划----------------------------------
function checkInvestUsable(player)
	pushRedPointMsg(player, {lv = 1131, index = 1, visible = gui.PanelInvest.isRedPointShow(player)});
end
----------------------------------精彩活动----------------------------------
function checkDiceUsable(player)
	pushRedPointMsg(player, {lv = 1141, index = 1, visible = gui.PanelYaoYaoLe.checkHalo(player)});
end
----------------------------------炼魂----------------------------------
function checkLianHunUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10141) then
		pushRedPointMsg(player, {lv = 3012, index = 1, visible = gui.PanelBossPictrue.checkRedPointLianHun(player)})
	end
end
----------------------------------拼图----------------------------------
function checkPinTuUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player, 10141) then
		pushRedPointMsg(player, {lv = 3013, index = 1, visible = gui.PanelBossPictrue.checkRedPointPinTu(player)})
	end
end

function checkSkillUsable(player)
	if gui.moduleGuiButton.checkFuncOpened(player,10013) then
		pushRedPointMsg(player, {lv = 2022, index = 1, visible = gui.PanelSkill.checkRedPoint(player)})
	end
end
------------------------------

-----------------------通知客户端-----------------------
function pushRedPointMsg(player, param)
	player:push_lua_table("redPoint", util.encode(param))
end

function handlePanelData(player,commond)
	local data = util.decode(commond)
	if data.actionid == "ConsignRedPoint" then--宝石兑换数据
		checkConsignUsable(player)
	end 
end

ckpanel.add_listener("gui.moduleRedPoint.reqRedPointInfo",gui.moduleRedPoint.reqRedPointInfo)
ckpanel.add_listener("gui.moduleRedPoint.handlePanelData",gui.moduleRedPoint.handlePanelData)
