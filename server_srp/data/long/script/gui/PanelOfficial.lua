module(...,package.seeall)

--官位配置表
local gw_data={
	[0] ={needSw=0,        dc=0,   dcmax=0,   mc=0,   mcmax=0,   sc=0,    scmax=0,   ac=0, acmax=0, mac=0, macmax=0, imgName="gw1", giveMoney=0      },
	[1] ={needSw=3000,     dc=22,  dcmax=6,   mc=22,  mcmax=6,   sc=22,   scmax=6,   ac=0, acmax=0, mac=0, macmax=0, imgName="gw1", giveMoney=100000 },
	[2] ={needSw=230000,   dc=40,  dcmax=11,  mc=40,  mcmax=11,  sc=40,   scmax=11,  ac=0, acmax=0, mac=0, macmax=0, imgName="gw2", giveMoney=200000 },
	[3] ={needSw=550000,   dc=68,  dcmax=19,  mc=68,  mcmax=19,  sc=68,   scmax=19,  ac=0, acmax=0, mac=0, macmax=0, imgName="gw3", giveMoney=300000 },
	[4] ={needSw=870000,   dc=103, dcmax=28,  mc=103, mcmax=28,  sc=103,  scmax=28,  ac=0, acmax=0, mac=0, macmax=0, imgName="gw4", giveMoney=400000 },
	[5] ={needSw=950000,   dc=143, dcmax=40,  mc=143, mcmax=40,  sc=143,  scmax=40,  ac=0, acmax=0, mac=0, macmax=0, imgName="gw5", giveMoney=500000 },
	[6] ={needSw=1380000,  dc=189, dcmax=52,  mc=189, mcmax=52,  sc=189,  scmax=52,  ac=0, acmax=0, mac=0, macmax=0, imgName="gw6", giveMoney=600000 },
	[7] ={needSw=2590000,  dc=240, dcmax=67,  mc=240, mcmax=67,  sc=240,  scmax=67,  ac=0, acmax=0, mac=0, macmax=0, imgName="gw7", giveMoney=700000 },
	[8] ={needSw=3450000,  dc=298, dcmax=83,  mc=298, mcmax=83,  sc=298,  scmax=83,  ac=0, acmax=0, mac=0, macmax=0, imgName="gw8", giveMoney=800000 },
	[9] ={needSw=4320000,  dc=372, dcmax=104, mc=372, mcmax=104, sc=372,  scmax=104, ac=0, acmax=0, mac=0, macmax=0, imgName="gw9", giveMoney=900000 },
	[10]={needSw=7120000,  dc=453, dcmax=126, mc=453, mcmax=126, sc=453,  scmax=126, ac=0, acmax=0, mac=0, macmax=0, imgName="gw10",giveMoney=1000000},
	[11]={needSw=9920000,  dc=534, dcmax=148, mc=534, mcmax=148, sc=534,  scmax=148, ac=0, acmax=0, mac=0, macmax=0, imgName="gw11",giveMoney=1100000},
	[12]={needSw=12720000, dc=615, dcmax=170, mc=615, mcmax=170, sc=615,  scmax=170, ac=0, acmax=0, mac=0, macmax=0, imgName="gw12",giveMoney=1200000},
	[13]={needSw=15520000, dc=696, dcmax=192, mc=696, mcmax=192, sc=696,  scmax=192, ac=0, acmax=0, mac=0, macmax=0, imgName="gw13",giveMoney=1300000},
	[14]={needSw=18320000, dc=777, dcmax=214, mc=777, mcmax=214, sc=777,  scmax=214, ac=0, acmax=0, mac=0, macmax=0, imgName="gw14",giveMoney=1400000},
	[15]={needSw=21120000, dc=858, dcmax=236, mc=858, mcmax=236, sc=858,  scmax=236, ac=0, acmax=0, mac=0, macmax=0, imgName="gw15",giveMoney=1500000},
	[16]={needSw=23920000, dc=939, dcmax=258, mc=939, mcmax=258, sc=939,  scmax=258, ac=0, acmax=0, mac=0, macmax=0, imgName="gw16",giveMoney=1600000},
	[17]={needSw=26720000, dc=1020,dcmax=280, mc=1020,mcmax=280, sc=1020, scmax=280, ac=0, acmax=0, mac=0, macmax=0, imgName="gw17",giveMoney=1700000},
	[18]={needSw=29520000, dc=1101,dcmax=302, mc=1101,mcmax=302, sc=1101, scmax=302, ac=0, acmax=0, mac=0, macmax=0, imgName="gw18",giveMoney=1800000},
	[19]={needSw=32320000, dc=1182,dcmax=324, mc=1182,mcmax=324, sc=1182, scmax=324, ac=0, acmax=0, mac=0, macmax=0, imgName="gw19",giveMoney=1900000},
	[20]={needSw=35120000, dc=1263,dcmax=346, mc=1263,mcmax=346, sc=1263, scmax=346, ac=0, acmax=0, mac=0, macmax=0, imgName="gw20",giveMoney=2000000},
	[21]={needSw=0,        dc=0,   dcmax=0,   mc=0,   mcmax=0,   sc=0,    scmax=0,   ac=0, acmax=0, mac=0, macmax=0, imgName="gw20",giveMoney=0      },
 };

 --官印配置表
 local gy_data={
 	[1] ={name="太初官印", needRy=5000, buffId=36001, addS=200 ,addM=200 ,addAct=200 ,gwLevel=1,  effectid=50025, gwName="十品芝麻官",    levName="一阶"},
 	[2] ={name="两仪官印", needRy=10000,buffId=36002, addS=400 ,addM=400 ,addAct=400 ,gwLevel=3,  effectid=50025, gwName="九品仕长",      levName="二阶"},
 	[3] ={name="三才官印", needRy=15000,buffId=36003, addS=600 ,addM=600 ,addAct=600 ,gwLevel=5,  effectid=50026, gwName="八品百夫长",    levName="三阶"},
 	[4] ={name="四象官印", needRy=20000,buffId=36004, addS=800 ,addM=800 ,addAct=800 ,gwLevel=7,  effectid=50026, gwName="七品校尉",      levName="四阶"},
 	[5] ={name="五行官印", needRy=25000,buffId=36005, addS=1000,addM=1000,addAct=1000,gwLevel=9,  effectid=50027, gwName="六品护军",      levName="五阶"},
 	[6] ={name="六合官印", needRy=30000,buffId=36006, addS=1200,addM=1200,addAct=1200,gwLevel=11, effectid=50027, gwName="五品先锋",      levName="六阶"},
 	[7] ={name="七星官印", needRy=35000,buffId=36007, addS=1400,addM=1400,addAct=1400,gwLevel=13, effectid=50028, gwName="四品中郎将",    levName="七阶"},
 	[8] ={name="八卦官印", needRy=40000,buffId=36008, addS=1600,addM=1600,addAct=1600,gwLevel=15, effectid=50028, gwName="三品镇远将军",  levName="八阶"},
 	[9] ={name="九宫官印", needRy=45000,buffId=36009, addS=1800,addM=1800,addAct=1800,gwLevel=17, effectid=50029, gwName="二品镇国大将军",levName="九阶"},
 	[10]={name="十方官印", needRy=50000,buffId=36010, addS=2000,addM=2000,addAct=2000,gwLevel=19, effectid=50029, gwName="一品大都督",    levName="十阶"},
}

function onPanelData(player,commond)
	-- player:set_param(const.PP_GUANWEI_LEVEL,0)
	local data = util.decode(commond);
	if not data  then return end
	if data.actionid == "reqSwData" then
		senderShopWsData(player)
		updateGuanWei(player)
	elseif data.actionid == "swBuy" then 
		buyShopSw(player,data.params.index)
	elseif data.actionid == "exchangeSw" then 
		levelExchangeSw(player)
	elseif data.actionid=="reqShengGuan" then
		startShengGuan(player)

	elseif data.actionid=="reqGyData" then
		-- updateGuanYin(player)
		updateGuanYinList(player)
	elseif data.actionid=="reqShengYin" then
		if data.params.pass>=1 then
			startShengYin(player,data.params.level)
		else
			if replaceCurBuff(player) then
				player:push_lua_table("PanelOfficial",util.encode({cmd="showReplaceTip"}))
			else
				startShengYin(player,data.params.level)
			end
		end
	elseif data.actionid=="reqBuyHonourData" then
		senderBuyHonourData(player)
	elseif data.actionid=="reqBuyHonour" then
		buyHonour(player,data.params.index)
	end
end
ckpanel.add_listener("gui.PanelOfficial.onPanelData",gui.PanelOfficial.onPanelData)




---------------------------------------------------------官位-----------------------------------------------------------
--等级兑换声望
function levelExchangeSw(player)
	local level = player:get_level()
	local lastLevelExp = levelToSw(player)
	local exchangeExp = lastLevelExp
	local needMoney=math.floor(exchangeExp)
	if level>100 then
		-- if util.ppn(player,const.PP_ZHSH_NUM) < MAX_TIMES then
			-- if  player:get_gamemoney()>=needMoney then
				-- player:sub_gamemoney(needMoney)
				player:set_level(level-1)
				-- player:set_param(const.PP_ZHSH_NUM,util.ppn(player,const.PP_ZHSH_NUM)+1)
				player:set_param(const.PP_GUANZHI_SW,util.ppn(player,const.PP_GUANZHI_SW)+exchangeExp)
				player:alert(1,1,"成功兑换声望:"..exchangeExp.."点！")
				updateClientSw(player)
				gui.moduleRedPoint.checkOfficialPostUsable(player)
			-- else
			-- 	player:alert(1,1,"金币不足无法兑换修为！")
			-- end
		-- else
		-- 	player:alert(1,1,"今日"..MAX_TIMES.."次等级兑换修为已用完！")
		-- end
	else
		player:alert(1,1,"声望兑换需超过:100级！")
	end
end

--等级兑换成声望的计算方式
function levelToSw(player)
	local level = player:get_level()
	local sw = tab.ZsAndSw[tostring(level)].shengwang
	return sw
end

--兑换成功后刷新客户端显示
function updateClientSw(player)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local itemData = gw_data[curGwLev+1]
	local result = {
		cmd="exchangeSwSucceed",
		curSw=util.ppn(player,const.PP_GUANZHI_SW),
		curLevel=player:get_level(),
		exchangeSw=levelToSw(player),
		upNeedSw=itemData.needSw,
		curGwLev=util.ppn(player,const.PP_GUANWEI_LEVEL),
	}
	player:push_lua_table("PanelOfficial",util.encode(result))
end

--打开官位刷新官位数据
function updateGuanWei(player,up)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local itemData = gw_data[curGwLev+1]
	local result = {
		cmd="updateGuanWei",
		dataSw={},
		curData=util.clone(gw_data[curGwLev]),
		nextData=util.clone(itemData),
		curGwLev=curGwLev,
		curGyName=gy_data[((curGwLev+1)-(curGwLev+1)%2)/2] and gy_data[((curGwLev+1)-(curGwLev+1)%2)/2].name or "无",
		nextGyName=gy_data[((curGwLev+2)-(curGwLev+2)%2)/2] and gy_data[((curGwLev+2)-(curGwLev+2)%2)/2].name or "无"
	}
	result.dataSw.curSw=util.ppn(player,const.PP_GUANZHI_SW)
	result.dataSw.curLevel=player:get_level()
	result.dataSw.exchangeSw=levelToSw(player)
	result.dataSw.upNeedSw=itemData.needSw
	result.dataSw.curGwLev=curGwLev
	result.up = up
	player:push_lua_table("PanelOfficial",util.encode(result))
end

--官位升级操作
function startShengGuan(player)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	if curGwLev>=(#gw_data-1) then
		player:alert(1,1,"您已将官位升至满级！")
		return
	end
	local curSw=util.ppn(player,const.PP_GUANZHI_SW)
	local itemData = gw_data[curGwLev+1]
	local upNeedSw=itemData.needSw
	if curSw>=upNeedSw then
		player:set_param(const.PP_GUANWEI_LEVEL,curGwLev+1)
		player:set_param(const.PP_GUANZHI_SW,curSw-upNeedSw)
		updateGuanWei(player,true)
		setGuanweiattr(player)
		player:alert(1,1,"成功升级官位！")
		npc.fenghao.updateAllTitle(player)
		gui.moduleRedPoint.checkOfficialPostUsable(player)
	else
		player:alert(1,1,"声望不足！")
	end
end

--每日10点邮件发放俸禄
function sendFengLu(player)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local itemData = gw_data[curGwLev]
	if itemData and itemData.giveMoney>0 then
		player:post_mail("俸禄邮件","每日俸禄已成功发放，请及时领取。",40000003,itemData.giveMoney)
	end
end

---------------------------------------------------------官印-----------------------------------------------------------
function updateGuanYin(player)
	local curGyLev = util.ppn(player,const.PP_GUANYIN_LEVEL)
	local itemData = gy_data[curGyLev+1]
	local buffId,buffIndex=getBuffTime(player)
	local result = {
		cmd="updateGuanYin",
		dataGy=util.clone(gy_data),
		curGyLev=curGyLev,
		curRy=util.ppn(player,const.PP_GUANZHI_RY),
		curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL),
		buffId = buffId,
		buffIndex = buffIndex,
	}
	player:push_lua_table("PanelOfficial",util.encode(result))
end

function updateGuanYinList(player)
	local result = {
		cmd="updateGuanYinList",
		dataList={},
	}
	for i=1,#gy_data do
		local itemData = gy_data[i]
		result.dataList[i]={}
		result.dataList[i].id=i
	end
	player:push_lua_table("PanelOfficial",util.encode(result))
	updateGuanYin(player)
end

--兑换官印的二次确认
function replaceCurBuff(player)
	local curGyLev = util.ppn(player,const.PP_GUANYIN_LEVEL)
	local itemData = gy_data[curGyLev]
	if itemData then
		local result = player:have_buff(itemData.buffId)
		if result>0 then
			return true
		end
		return false
	end
	return false
end

--获得buff的剩余时间
function getBuffTime(player)
	for i=1,#gy_data do
		local itemData = gy_data[i]
		local buffTime = player:get_buff_valid_value(itemData.buffId)
		-- print(buffTime,player:get_buff_valid_value(25101))
		if buffTime>0 then
			return itemData.buffId,i
		end
	end
	return 0,0
end

--升级官印操作
function startShengYin(player,level)
	local curGyLev = util.ppn(player,const.PP_GUANYIN_LEVEL)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local curRy=util.ppn(player,const.PP_GUANZHI_RY)--当前荣誉值
	-- if (curGyLev+1)>=#gy_data then
	-- 	player:alert(1,1,"官印已满级！")
	-- 	return
	-- end
	-- local itemData = gy_data[curGyLev+1]--官印升级对象
	local itemData = gy_data[level]
	if curGwLev>=itemData.gwLevel then
		if curRy>= itemData.needRy then
			if replaceCurBuff(player) then
				if level<=curGyLev then
					player:alert(1,1,"您已持有更高级的官印！")
					return
				end
			end
			player:set_param(const.PP_GUANYIN_LEVEL,level)
			player:set_param(const.PP_GUANZHI_RY,curRy-itemData.needRy)
			player:alert(1,1,"成功领取"..itemData.name)
			setCurLevelProperty(player)
			for i=1,#gy_data do
				local itemData = gy_data[i]
				local buffTime = player:get_buff_valid_value(itemData.buffId)
				-- print(buffTime,player:get_buff_valid_value(25101))
				if buffTime>0 then
					player:clear_buff(itemData.buffId)
				end
			end
			player:set_buff(itemData.buffId)
			updateGuanYin(player)
			gui.moduleRedPoint.checkOfficialChopUsable(player)
		else
			player:alert(1,1,"荣誉值不足！")
		end
	else
		player:alert(1,1,"官位等级未达成！")
	end
end

--兑换时寻找当前官印是什么等级
function getCurBuffLevel(player)
	local level = 0
	for i=1,#gy_data do
		local itemData = gy_data[i]
		local buffTime = player:get_buff_valid_value(itemData.buffId)
		if buffTime>0 then
			level=i
			break
		end
	end
	return level
end

--官印设置属性
function setCurLevelProperty(player)
	local curGyLev = util.ppn(player,const.PP_GUANYIN_LEVEL)
	local itemData = gy_data[curGyLev]
	if curGyLev<=0 or not itemData then return end
	local addS = player:get_script_attr(39)--增伤
	local addM = player:get_script_attr(40)--免伤

	local preLevel = getCurBuffLevel(player)
	if preLevel>0 then
		local preData = gy_data[curGyLev-preLevel]
		addS = addS + itemData.addS-preData.addS
		addM = addM + itemData.addM-preData.addM
	else
		addS = addS + itemData.addS
		addM = addM + itemData.addM
	end
	player:set_script_attr(39,addS)
	player:set_script_attr(40,addM)
end

--上线设置官印属性
function refreshActPro(player)
	local curGyLev = util.ppn(player,const.PP_GUANYIN_LEVEL)
	local itemData = gy_data[curGyLev]
	if curGyLev<=0 or not itemData then return end
	local addS = player:get_script_attr(39)--增伤
	local addM = player:get_script_attr(40)--免伤

	addS = addS + itemData.addS
	addM = addM + itemData.addM

	player:set_script_attr(39,addS)
	player:set_script_attr(40,addM)
end 
login.add_login_listener(refreshActPro)

------设置官位属性
------ dc=0,   dcmax=0,    ac=0,   acmax=0,    sc=0,   scmax=0,    mc=0, mcmax=0, mac=0, macmax=0, 
function setGuanweiattr(player)
	--print("0000")
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local job = player:get_job_name()
	local itemData = gw_data[curGwLev]

	local minac=player:get_script_attr(3)
	local maxac=player:get_script_attr(4)
	local minmac=player:get_script_attr(5)
	local maxmac=player:get_script_attr(6)

	local mindc=player:get_script_attr(7)
	local maxdc=player:get_script_attr(8)
	local minmc=player:get_script_attr(9)
	local maxmc=player:get_script_attr(10)
	local minsc=player:get_script_attr(11)
	local maxsc=player:get_script_attr(12)
	local addminac,addmaxac,addminmac,addmaxmac,addmindc,addmaxdcadd,addminmc,addmaxmc,addminsc,addmaxsc=0,0,0,0,0,0,0,0,0,0
	if curGwLev>1 then
		local preData = gw_data[curGwLev-1]
		addminac = itemData.ac-preData.ac
		addmaxac = itemData.acmax-preData.acmax
		addminmac= itemData.mac-preData.mac
		addmaxmac= itemData.macmax-preData.macmax
		addmindc= itemData.dc-preData.dc
		addmaxdc= itemData.dcmax-preData.dcmax
		addminmc= itemData.mc-preData.mc
		addmaxmc= itemData.mcmax-preData.mcmax
		addminsc= itemData.sc-preData.sc
		addmaxsc= itemData.scmax-preData.scmax
	else
		addminac = itemData.ac
		addmaxac = itemData.acmax
		addminmac= itemData.mac
		addmaxmac= itemData.macmax
		addmindc= itemData.dc
		addmaxdc= itemData.dcmax
		addminmc= itemData.mc
		addmaxmc= itemData.mcmax
		addminsc= itemData.sc
		addmaxsc= itemData.scmax
	end

	--if job == "warrior" then
		player:set_script_attr(7,addmindc+mindc);--增加最小物理攻击
		player:set_script_attr(8,addmaxdc+maxdc);--增加最大物理攻击
	--elseif job == "wizard" then
		player:set_script_attr(9,addminmc+minmc);--增加最小魔法攻击
		player:set_script_attr(10,addmaxmc+maxmc);--增加最大魔法攻击
	--elseif job == "taoist" then
		player:set_script_attr(11,addminsc+minsc);--增加最小道术攻击
		player:set_script_attr(12,addmaxsc+maxsc);--增加最大道术攻击
	--end
	--print("22222",addminac+minac)
	player:set_script_attr(3,addminac+minac);--增加最小物理防御
	player:set_script_attr(4,addmaxac+maxac);--增加最大物理防御
	player:set_script_attr(5,addminmac+minmac);--增加最小魔法防御
	player:set_script_attr(6,addmaxmac+maxmac);--增加最大魔法防御
	player:recal_attr()
end 

function refreshGuanweiattr(player)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local job = player:get_job_name()
	local itemData = gw_data[curGwLev]
	local minac=player:get_script_attr(3)
	local maxac=player:get_script_attr(4)
	local minmac=player:get_script_attr(5)
	local maxmac=player:get_script_attr(6)
	local mindc=player:get_script_attr(7)
	local maxdc=player:get_script_attr(8)
	local minmc=player:get_script_attr(9)
	local maxmc=player:get_script_attr(10)
	local minsc=player:get_script_attr(11)
	local maxsc=player:get_script_attr(12)
	--if job == "warrior" then
		player:set_script_attr(7,itemData.dc+mindc);--增加最小物理攻击
		player:set_script_attr(8,itemData.dcmax+maxdc);--增加最大物理攻击
	--elseif job == "wizard" then
		player:set_script_attr(9,itemData.mc+minmc);--增加最小魔法攻击
		player:set_script_attr(10,itemData.mcmax+maxmc);--增加最大魔法攻击
	--elseif job == "taoist" then
		player:set_script_attr(11,itemData.sc+minsc);--增加最小道术攻击
		player:set_script_attr(12,itemData.scmax+maxsc);--增加最大道术攻击
	--end
	player:set_script_attr(3,itemData.ac+minac);--增加最小物理防御
	player:set_script_attr(4,itemData.acmax+maxac);--增加最大物理防御
	player:set_script_attr(5,itemData.mac+minmac);--增加最小魔法防御
	player:set_script_attr(6,itemData.macmax+maxmac);--增加最大魔法防御
	player:recal_attr()
end
login.add_login_listener(refreshGuanweiattr);



----------------------------------------------官印-荣誉购买------------------------------------------------
-- local honourTable = {
-- 	[1]={id=20001001,name="太阳水",honour=10,vcion=100,useTimes=10,con=const.PP_MAXBUYNUM_RY1},--低级荣誉宝箱
-- 	[2]={id=20001001,name="太阳水",honour=50,vcion=200,useTimes=10,con=const.PP_MAXBUYNUM_RY2},--高级荣誉宝箱
-- }

function senderBuyHonourData(player)
	local result = {
		cmd="senderBuyHonourData",
		data=tab.Store.getItemsByType(player,12),
		curVcion=player:get_vcoin(),
		curBVcion = player:get_vcoin_bind(),
	}
	player:push_lua_table("PanelOfficial",util.encode(result))
end

function buyHonour(player,storeId)
	-- if not honourTable[index] then return end
	-- local itemData = honourTable[index]
	-- if util.ppn(player,itemData.con)<itemData.useTimes then
	-- 	if player:get_vcoin()>=itemData.vcion then
	-- 		player:set_param(itemData.con,util.ppn(player,itemData.con)+1)
	-- 		player:sub_vcoin(itemData.vcion,"购买荣誉宝箱")
	-- 		player:add_item(itemData.id,1,1)
	-- 		-- player:set_param(const.PP_GUANZHI_SW,util.ppn(player,const.PP_GUANZHI_SW)+itemData.honour)
	-- 		player:alert(1,1,"成功购买"..itemData.name)
	-- 		senderBuyHonourData(player)
	-- 	else
	-- 		-- player:alert(1,1,"元宝不足")
	-- 		showChongzhi(player)
	-- 	end
	-- else
	-- 	player:alert(1,1,"今日"..itemData.name.."的购买次数已用完！")
	-- end
	local itemData = tab.Store.buyShopItem(player,storeId,"官位荣誉",1,"notinbag")
	if itemData then
		senderBuyHonourData(player)
		updateGuanYin(player)
	end
end

----------------------------------------------官位-声望购买------------------------------------------------
function senderShopWsData(player)
	local result = {
		cmd="senderShopWsData",
		data=tab.Store.getItemsByType(player,6),
	}
	player:push_lua_table("PanelOfficial",util.encode(result))
end

function buyShopSw(player,storeId)
	-- print(util.encode(tab.Store.getItemsByType(player,6)))
	local itemData = tab.Store.buyShopItem(player,storeId,"官位声望",1,"notinbag")
	if itemData then
		updateClientSw(player)
	end
end

function addsw(player,value)
	player:set_param(const.PP_GUANZHI_SW,util.ppn(player,const.PP_GUANZHI_SW)+value)
	player:alert(100000,1,"获得声望:"..value)
	gui.moduleRedPoint.checkOfficialPostUsable(player)
end

function addry(player,value)
	player:set_param(const.PP_GUANZHI_RY,util.ppn(player,const.PP_GUANZHI_RY)+value)
	player:alert(100000,1,"获得荣誉:"..value)
	gui.moduleRedPoint.checkOfficialChopUsable(player)
end

--官位能否升级
function checkRedPointGw(player)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local itemData = gw_data[curGwLev+1]
	local curSw=util.ppn(player,const.PP_GUANZHI_SW)
	if curGwLev<20 and curSw>=itemData.needSw then
		return true--官位可以升级
	end
	return false
end

--官印能否升级
function checkRedPointGy(player)
	local curGyLev = util.ppn(player,const.PP_GUANYIN_LEVEL)
	local curGwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local curRy=util.ppn(player,const.PP_GUANZHI_RY)--当前荣誉值
	local itemData = gy_data[curGyLev+1]--官印升级对象
	if curGyLev<#gy_data and curGwLev>=itemData.gwLevel and curRy>= itemData.needRy then
		return true--官印可以升级
	end
	return false
end