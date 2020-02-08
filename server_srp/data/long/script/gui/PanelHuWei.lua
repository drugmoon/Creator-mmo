module(... ,package.seeall)
local MAX_LEVEL = 160
local INIT_TIMES = 10--初始召唤次数
local RUN_TIMES = 2--时间成长最大次数

--name:本阶护卫名称 danId:进阶到下届丹id danNum:进阶到下阶丹的数量 pro:进阶成功率 min:最小成功次数 max:必成 needLevel:进阶到下阶需要的等级
local heroData = {
	[1]= {name="护卫1阶",  huWeiId=303001, weapRes=11106011,clothRes=11106021, danId=24000001,danNum=1,  pro=10000,min=0, max=10, needLevel=20 , con=const.PP_HUWEI_JINJIE1},
	[2]= {name="护卫2阶",  huWeiId=303002, weapRes=11107011,clothRes=11107021, danId=24000001,danNum=2,  pro=10000,min=1, max=10, needLevel=40 , con=const.PP_HUWEI_JINJIE2},
	[3]= {name="护卫3阶",  huWeiId=303003, weapRes=11108011,clothRes=11108021, danId=24000001,danNum=5,  pro=5000, min=2, max=10, needLevel=60 , con=const.PP_HUWEI_JINJIE3},
	[4]= {name="护卫4阶",  huWeiId=303004, weapRes=11109011,clothRes=11109021, danId=24000001,danNum=10, pro=2500, min=3, max=10, needLevel=80 , con=const.PP_HUWEI_JINJIE4},
	[5]= {name="护卫5阶",  huWeiId=303005, weapRes=11110011,clothRes=11110021, danId=24000001,danNum=20, pro=1000, min=4, max=10, needLevel=100, con=const.PP_HUWEI_JINJIE5},
	[6]= {name="护卫6阶",  huWeiId=303006, weapRes=11111011,clothRes=11111021, danId=24000001,danNum=35, pro=834,  min=5, max=10, needLevel=120, con=const.PP_HUWEI_JINJIE6},
	[7]= {name="护卫7阶",  huWeiId=303007, weapRes=11112011,clothRes=11112021, danId=24000001,danNum=65, pro=334,  min=6, max=10, needLevel=140, con=const.PP_HUWEI_JINJIE7},
	[8]= {name="护卫8阶",  huWeiId=303008, weapRes=11113011,clothRes=11113021, danId=24000001,danNum=105,pro=122,  min=7, max=10, needLevel=160, con=const.PP_HUWEI_JINJIE8},
	[9]= {name="护卫9阶",  huWeiId=303009, weapRes=11114011,clothRes=11114021, danId=24000001,danNum=160,pro=96,   min=8, max=10, needLevel=180, con=const.PP_HUWEI_JINJIE9},
    [10]={name="护卫10阶", huWeiId=303010, weapRes=11115011,clothRes=11115021, danId=24000001,danNum=220,pro=48,   min=9, max=10, needLevel=200, con=const.PP_HUWEI_JINJIE10},
}

function handlePanelData(player,commond)
	-- player:set_param(const.PP_HUWEI_LEVEL,1)
	-- player:set_param(const.PP_HUWEI_CUREXP,0)
	-- for i=0,#heroData-1 do
	-- 	player:set_param(const.PP_HUWEI_JINJIE1+i,0)
	-- end

	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "reqBuyExpData" then
		senderBuyExpData(player,true)
	elseif data.actionid == "reqBuyTheExp" then 
		buyUseExp(player,data.params.index)
	elseif data.actionid == "reqBuyDanData" then 
		senderBuyDanData(player)
	elseif data.actionid == "reqBuyTheDan" then
		buyJieDan(player,data.params.index)
	elseif data.actionid == "reqZhaoHuan" then
		callHuWei(player,true)
	elseif data.actionid == "reqZhaoHui" then--招回
		callBackHuWei(player)
		-- addHuweiExp(player,1000)
	elseif data.actionid=="reqPanelData" then
		sendToClient(player)
		player:set_param(const.PP_HUWEI_OPEN,1)--护卫面板打开
		updateBtnEff(player)
	elseif data.actionid=="reqJinJie" then
		startJinJie(player)
	elseif data.actionid=="reqExpDanUse" then--经验丹使用
		useHuWeiDan(player)
	elseif data.actionid=="panelClose" then--护卫面板关闭
		player:set_param(const.PP_HUWEI_OPEN,0)
	end
end

--护卫2个购买按钮的光圈
function updateBtnEff(player)
	local result = {
		cmd="updateBtnHalo",
		haloExp=util.ppn(player,const.PP_HUWEI_BTN1),
		haloDan=util.ppn(player,const.PP_HUWEI_BTN2),
	}
	player:push_lua_table("PanelHuWei",util.encode(result))
end

--护卫能否进阶
function checkMarsUpgrade(player)
	local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL);--当前等级
	if heroLevel==0 then
		player:set_param(const.PP_HUWEI_LEVEL,1);
		heroLevel=1;
	end
	local curJie = getCurJie(heroLevel);
	local needLevel;
	if util.ppn(player,heroData[curJie].con)>0 and curJie<#heroData then
		needLevel = heroData[curJie+1].needLevel;--进阶需要的等级
	else
		needLevel = heroData[curJie].needLevel;--进阶需要的等级
	end
	if not (needLevel and heroLevel >= needLevel) then
		return false;
	end

	local needDanNum=heroData[curJie+1].danNum;
	local curDanNum=player:num_item(heroData[curJie].danId) or 0;
	if needDanNum > curDanNum then
		return false;
	end
	return true;
end

function getAddFightPoint(curData)
	local fight =0;
	fight = fight + curData.wgmin * 1.0 * 32000 / 10000;
	fight = fight + curData.wgmax * 1.0 * 16000 / 10000;
	fight = fight + curData.wfmin * 1.0 * 7000 / 10000;
	fight = fight + curData.wfmax * 1.0 * 5000 / 10000;
	return math.floor(fight);
end

function sendToClient(player,up)
	local result = {
		cmd="updateHuWei",
		curData={},
		nextData={},
	}
	local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)--当前等级
	if heroLevel==0 then
		player:set_param(const.PP_HUWEI_LEVEL,1)
		heroLevel=1
	end
	local curUseNum = util.ppn(player,const.PP_HUWEI_USENUM)--今日已召唤的次数
	local runTime = util.ppn(player,const.PP_HUWEI_RUNTIME)--今日累计在线时间（满一个小时增加一次召唤次数）
	result.curLevel=heroLevel
	if heroLevel>0 then
		result.curJie=getCurJie(heroLevel)
		result.name=heroData[result.curJie].name
		if heroLevel%20==0 and util.ppn(player,heroData[result.curJie].con)>=1 and (result.curJie+1)<=#heroData then--刚进阶成功且未升级
			result.name=heroData[result.curJie+1].name
			result.skillNum=result.curJie+1
		end
	else
		result.curJie=1
		result.name="暂无神将"
	end
	local curData = tab.HuWeiLevel[tostring(heroLevel)]
	local nextData = tab.HuWeiLevel[tostring(heroLevel+1)]

	-- print(util.encode(curData),util.encode(nextData))

	result.curData=curData--{"id","level","needexp","maxhp","wgmin","wgmax","wfmin","wfmax","mfmin","mfmax","huixue"}
	result.addFightPoint = getAddFightPoint(curData);

	result.nextData=nextData

	result.weapRes=result.skillNum and heroData[result.curJie+1].weapRes or heroData[result.curJie].weapRes--武器
	result.clothRes=result.skillNum and heroData[result.curJie+1].clothRes or heroData[result.curJie].clothRes--衣服

	result.failNum=util.ppn(player,const.PP_HUWEI_DEFEAT)--以失败的次数
	if util.ppn(player,heroData[result.curJie].con)>0 and result.curJie<#heroData then
		result.needLevel=heroData[result.curJie+1].needLevel--进阶需要的等级
	else
		result.needLevel=heroData[result.curJie].needLevel--进阶需要的等级
	end
	result.needDanNum=heroData[result.curJie+1].danNum
	result.curDanNum=player:num_item(heroData[result.curJie].danId) or 0
	result.curExp=util.ppn(player,const.PP_HUWEI_CUREXP)
	result.needExp=nextData["needexp"]
	-- result.expPro=curData["needexp"]--当前等级满级需要的经验
	result.enableNum=INIT_TIMES+(runTime-runTime%1200)/1200-curUseNum
	result.countTimes=1200-runTime%1200--多少秒后增加1次

	result.state=player:have_slave(10)--当前有无护卫
	result.curCD=util.ppn(player,const.PP_HUWEI_CALL_CD)

	result.canUpgrade = checkMarsUpgrade(player);
	result.up = up
	player:push_lua_table("PanelHuWei",util.encode(result))

end

--召唤次数增加1后刷新客户端次数显示和倒计时
function updateEnableTimes(player)
	local result = {
		cmd="updateTimes",
	}
	local runTime = util.ppn(player,const.PP_HUWEI_RUNTIME)--今日累计在线时间（满一个小时增加一次召唤次数）
	local curUseNum = util.ppn(player,const.PP_HUWEI_USENUM)--今日已召唤的次数
	result.enableNum=INIT_TIMES+(runTime-runTime%1200)/1200-curUseNum
	result.countTimes=1200-runTime%1200--多少秒后增加1次
	player:push_lua_table("PanelHuWei",util.encode(result))
end

--获取护卫名字
function getHuWeiName(player,level)
	local jie = 0
	local name = "暂无护卫"
	if level>0 then
		jie=((level-1)-(level-1)%20)/20+1
		name=heroData[jie].name
	end
	return name
end

--召唤护卫操作
function callHuWei(player,tip)
	if not gui.moduleGuiButton.checkFuncOpened(player, 20121) then return end
	local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)--当前等级
	if heroLevel>0 then
		-- if checkEnableTimes(player) or notime then
			if util.ppn(player,const.PP_HUWEI_CALL_CD)>0 then
				player:alert(1,1,"CD未结束不能召唤")
				return
			end
			local curJie = getCurJie(heroLevel)
			if heroLevel%20==0 and util.ppn(player,heroData[curJie].con)>=1 and (curJie+1)<=#heroData then--刚进阶成功且未升级
				curJie=curJie+1
			end
			local itemData = heroData[curJie]
			local map = player:get_map()
			if map:disable_huwei() > 0 and tip then
				player:alert(1,1,"当前地图不可召唤护卫")
				return
			end
			player:gen_slave(10,itemData.huWeiId,4)

			setHuWeiSkills(player,curJie)
			-- if not notime then
				-- player:set_param(const.PP_HUWEI_USENUM,util.ppn(player,const.PP_HUWEI_USENUM)+1)
			-- end
			if tip then
				player:alert(1,1,"成功召唤:"..itemData.name)
			end
			-- startCount(player)
			-- sendToClient(player,true)
			gui.moduleRedPoint.checkMarsUsable(player)
			player:push_lua_table("PanelHuWei",util.encode({cmd="huWeiZhaoHuan"}))
		-- else
		-- 	player:alert(1,1,"当前召唤次数已用完！")
		-- end
	else
		player:alert(1,1,"暂无神将无法召唤！")
	end
end

--召唤后设置技能
function setHuWeiSkills(player,curJie,heroLevel)
	setBaseValue(player)
	local slave = player:get_slave(10)
	if slave == nil then
		return
	end
	if curJie>=1 then
		---半月
		slave:set_atk_skill(70006)
	end
	---增防50%
	if curJie>=2 then
		local ac = slave:get_attr(3)
		local acmax = slave:get_attr(4)
		local mac = slave:get_attr(5)
		local macmax = slave:get_attr(6)
		ac = ac * 1.5
		acmax = acmax * 1.5
		mac = mac * 1.5
		macmax = macmax * 1.5
		slave:set_attr(3, ac)
		slave:set_attr(4, acmax)
		slave:set_attr(5, mac)
		slave:set_attr(6, macmax)
	end
	-- 增血50%
	if curJie>=3 then
		local hp = slave:get_attr(2)
		hp = hp * 1.5
		slave:set_attr(2, hp)
		slave:set_hp(hp)
	end
	-- 增伤50%
	if curJie>=4 then
		slave:set_attr(42, 5000)
	end
	-- 伤害减少
	if curJie>=5 then
		slave:set_attr(43, 5000)
	end
	--圆月
	if curJie>=6 then
		slave:set_atk_skill(70005)
	end
	--神圣攻击50%
	if curJie>=7 then
		slave:set_attr(41,5000 )	
	end
	-- 暴击20%
	if curJie>=8 then
		slave:set_attr(31, 2000)	
	end
	-- 守护
	if curJie>=9 then
		---人物+10%免伤
		local DamReduce = player:get_script_attr(43)
		DamReduce=DamReduce+1000
		player:set_script_attr(43,DamReduce);--增加最小物理攻击
	end
	-- 聚力
	if curJie>=10 then
		---把护卫攻击属性的5% +到人物
		local dc = slave:get_attr(7)
		local dcmax = slave:get_attr(8)
		local mc = slave:get_attr(9)
		local mcmax = slave:get_attr(10)
		local sc = slave:get_attr(11)
		local scmax = slave:get_attr(12)
		dc = dc * 1.05
		dcmax = dcmax * 1.05
		mc = mc * 1.05
		mcmax = mcmax * 1.05
		sc = sc * 1.05
		scmax = scmax * 1.05
		local mindc=player:get_script_attr(7)
		local maxdc=player:get_script_attr(8)
		local minmc=player:get_script_attr(9)
		local maxmc=player:get_script_attr(10)
		local minsc=player:get_script_attr(11)
		local maxsc=player:get_script_attr(12)
		player:set_script_attr(7,mindc+dc);--增加最小物理攻击
		player:set_script_attr(8,maxdc+dcmax);--增加最大物理攻击
		player:set_script_attr(9,minmc+mc);--增加最小魔法攻击
		player:set_script_attr(10,maxmc+mcmax);--增加最大魔法攻击
		player:set_script_attr(11,minsc+sc);--增加最小道术攻击
		player:set_script_attr(12,maxsc+scmax);--增加最大道术攻击	
	end

	slave:recal_attr()
end

--升级或者召唤时设置基础属性
function setBaseValue(player)
	local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)--当前等级
	local curData = tab.HuWeiLevel[tostring(heroLevel)]

	local slave = player:get_slave(10)
	if slave ~= nil then
		slave:set_attr(2, curData["maxhp"])
		slave:set_attr(3, curData["wfmin"])
		slave:set_attr(4, curData["wfmax"])
		slave:set_attr(5, curData["mfmin"])
		slave:set_attr(6, curData["mfmax"])
		slave:set_attr(7, curData["wgmin"])
		slave:set_attr(8, curData["wgmax"])
		slave:set_hp(curData["maxhp"])
		-- slave:add_hp(curData["huixue"])
		slave:recal_attr()
		setHuiXue(player)
	end
end

--设置护卫回血
function setHuiXue(player)
	local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)--当前等级
	local curData = tab.HuWeiLevel[tostring(heroLevel)]
	local slave = player:get_slave(10)
	if slave ~= nil then
		slave:add_hp(curData["huixue"])
		slave:recal_attr()
		player:set_timer(23,1*1000)
	else
		player:set_timer(23,0)
	end
end

--招回护卫操作
function callBackHuWei(player)
	player:set_param(const.PP_HUWEI_CALL_CD,27)
	player:set_timer(19,1*1000)
	player:kill_slave(10)
	player:alert(1,1,"护卫被成功召回！")
	player:push_lua_table("PanelHuWei",util.encode({cmd="huWeiZhaoHui",curCD=30}))
	gui.moduleRedPoint.checkMarsUsable(player)
end

--小于12次的时候开始倒计时
function startCount(player)
	local curUseNum = util.ppn(player,const.PP_HUWEI_USENUM)--今日已召唤的次数
	local runTime = util.ppn(player,const.PP_HUWEI_RUNTIME)--今日累计在线时间（满一个小时增加一次召唤次数）
	local enableNum=INIT_TIMES+(runTime-runTime%1200)/1200-curUseNum
	if enableNum>=11 then
		player:set_timer(15,10*1000)
	end
end

--判定当前可召唤次数
function checkEnableTimes(player)
	local curUseNum = util.ppn(player,const.PP_HUWEI_USENUM)--今日已召唤的次数
	local runTime = util.ppn(player,const.PP_HUWEI_RUNTIME)--今日累计在线时间（满一个小时增加一次召唤次数）
	if curUseNum>=(INIT_TIMES+(runTime-runTime%1200)/1200) then
		return false
	end
	return true
end

--护卫进阶操作
function startJinJie(player)
	local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)--当前等级
	if heroLevel==0 or (heroLevel%20)~=0 then
		player:alert(1,1,"未满足进阶条件！")
		return
	end
	local failNum = util.ppn(player,const.PP_HUWEI_DEFEAT)--以失败的次数
	local curJie = getCurJie(heroLevel)
	if curJie>=#heroData then
		player:alert(1,1,"护卫已达最大等级！")
		return
	end
	local curData = heroData[curJie]
	if util.ppn(player,curData.con)==1 then
		player:alert(1,1,"已进阶无需重复进阶！")
		return
	end
	local nextData = heroData[curJie+1]
	if player:num_item(nextData.danId)>=nextData.danNum then
		local randNum = math.random(1,10000)
		local up = false
		-- print(randNum,nextData.pro,failNum,nextData.max,nextData.min)
		if (randNum<=nextData.pro or failNum==nextData.max-1) and failNum>=(nextData.min-1) then
			player:set_param(curData.con,1)--当前等级满之后要进阶后才能继续升级（0：未进阶 1：已进阶）
			player:set_param(const.PP_HUWEI_DEFEAT,0)
			player:alert(1,1,"进阶成功！")
			addHuweiExp(player,0)--进阶成功后自动掉升级
			callHuWei(player)
			up = true
		else
			player:set_param(const.PP_HUWEI_DEFEAT,failNum+1)
			player:alert(1,1,"进阶失败！")
		end
		player:remove_item(nextData.danId,nextData.danNum)
		sendToClient(player,up)
		gui.moduleRedPoint.checkMarsUsable(player);
	else
		player:alert(1,1,"护卫进阶丹不足！")
	end

end

--根据等级计算当前阶
function getCurJie(level)
	local curJie = ((level-1)-(level-1)%20)/20+1
	return curJie
end

--护卫增加经验 isbag:是背包使用
function addHuweiExp(player,exp,isbag)
	local openState = util.ppn(player,const.PP_HUWEI_OPEN)--面板是否处于打开状态
	local heroLevel = util.ppn(player,const.PP_HUWEI_LEVEL)--当前等级
	local nextData = tab.HuWeiLevel[tostring(heroLevel+1)]
	if heroLevel==0 then
		player:set_param(const.PP_HUWEI_LEVEL,1)
		heroLevel=1
	end	
	local curJie = getCurJie(heroLevel)
	if util.ppn(player,heroData[curJie].con)==0 and heroLevel==heroData[curJie].needLevel then
		if exp>0 and openState==1 then sendToClient(player) end
		if isbag then
			player:alert(1,1,"当前需要进阶方可使用经验丹")
		end
		return "当前需要进阶方可提升护卫经验"
		--需要进阶才能自动升级 且不增加护卫经验
	end
	if exp>0 then
		player:alert(100000,1,"获得护卫经验:"..exp)
	end
	if not isbag and player:have_slave(10)==0 then return end--没有护卫打怪不加经验
	player:set_param(const.PP_HUWEI_CUREXP,util.ppn(player,const.PP_HUWEI_CUREXP)+exp)

	local curExp = util.ppn(player,const.PP_HUWEI_CUREXP)
	if curExp>=tonumber(nextData["needexp"]) then
		player:set_param(const.PP_HUWEI_LEVEL,heroLevel+1)
		player:set_param(const.PP_HUWEI_CUREXP,curExp-tonumber(nextData["needexp"]))
		addHuweiExp(player,0,isbag)
		setBaseValue(player)
		shengJie(player,heroLevel)
	end
	if exp>0 and openState==1 then
		sendToClient(player)
	end
	return nil
end

--升阶时自动召唤且不扣召唤次数
function shengJie(player,curLevel)
	local slave = player:get_slave(10)
	if slave==nil then return end
	local curLevel=curLevel
	local nextLevel = curLevel+1
	if getCurJie(curLevel)~=getCurJie(nextLevel) then
		callHuWei(player,true)
	end
end


-----------------------------------------------------快捷购买进阶丹-----------------------------------------------
local dansTable={
	-- [1]={id=24000001,num=1,  name="战神进阶丹",vcion=0,     bvcion=100,maxNum=20, con=const.PP_HUWEI_BUYJIE},
	[1]={id=24000001, bagNum=1, num=1,   name="护卫进阶丹", vcion=300,   bvcion=0, maxNum=0,  },
	[2]={id=24000001, bagNum=1, num=10,  name="护卫进阶丹", vcion=3000,  bvcion=0, maxNum=0,  },
	[3]={id=24000001, bagNum=2, num=100, name="护卫进阶丹", vcion=28888, bvcion=0, maxNum=0,  },
}

function senderBuyDanData(player)
	local result = {
		cmd="senderDanData",
		data={},
	}
	local tempData = util.clone(dansTable)
	for i=1,#tempData do
		local itemData = tempData[i]
		if itemData.con then
			itemData.con=util.ppn(player,itemData.con)
		end
	end
	result.data=tempData
	player:push_lua_table("PanelHuWei",util.encode(result))
	if util.ppn(player,const.PP_HUWEI_BTN2)<=0 then
		player:set_param(const.PP_HUWEI_BTN2,1)
		updateBtnEff(player)
	end
end

function buyJieDan(player,index)
	if not dansTable[index] then return end
	local itemData = dansTable[index]
	if player:num_bag_black()<itemData.bagNum then
		player:alert(1,1,"背包空间不足！")
		return
	end
	if itemData.vcion>0 then
		if player:get_vcoin()>=itemData.vcion then
			player:sub_vcoin(itemData.vcion,"购买护卫突破丹")
			player:add_item(itemData.name,itemData.num,1)
			player:alert(1, 1, "成功购买"..itemData.name.."*"..itemData.num)
			sendToClient(player)
		else
			-- player:alert(1,1,"元宝不足！");
			showChongzhi(player)
		end
	end
	-- if itemData.bvcion>0 then
	-- 	local buyNum = util.ppn(player,itemData.con)
	-- 	if buyNum<itemData.maxNum then
	-- 		if player:get_vcoin_bind()>=itemData.bvcion then
	-- 			player:set_param(itemData.con,buyNum+1)
	-- 			player:sub_vcoin_bind(itemData.bvcion)
	-- 			player:add_item(itemData.name,1,1)
	-- 			player:alert(1, 1, "成功购买"..itemData.name)
	-- 		else
	-- 			player:alert(1,1,"绑定元宝不足！");
	-- 		end
	-- 	else
	-- 		player:alert(1,1,"今日购买次数已用完！");
	-- 	end
	-- end
end

-----------------------------------------------------快捷购买护卫经验丹-----------------------------------------------
local goodsTable={
	-- [1]={id=20001006,num=1, name="金疮药(大)",vcion=0,   bvcion=100,maxNum=20, con=const.PP_HUWEI_BUYZHU},
	[1]={id=23090001,num=1, name="护卫经验丹（小）",exp=100000,  vcion=30,   bvcion=0,  maxNum=0,  },
	[2]={id=23090002,num=1, name="护卫经验丹（中）",exp=1000000, vcion=300,  bvcion=0,  maxNum=0,  },
	[3]={id=23090003,num=1, name="护卫经验丹（大）",exp=10000000,vcion=2888, bvcion=0,  maxNum=0,  },
}

function senderBuyExpData(player,flag)
	local result = {
		cmd="senderExpData",
		data={},
		ownData=getOwnExpDan(player),
	}
	local tempData = util.clone(goodsTable)
	local fresh = false
	for i=1,#tempData do
		local itemData = tempData[i]
		if itemData.con then
			itemData.con=util.ppn(player,itemData.con)
			fresh = true
		end
	end
	if fresh or flag then
		result.data=tempData
	end
	player:push_lua_table("PanelHuWei",util.encode(result))
	if util.ppn(player,const.PP_HUWEI_BTN1)<=0 then
		player:set_param(const.PP_HUWEI_BTN1,1)
		updateBtnEff(player)
	end
end

function buyUseExp(player,index)
	if not goodsTable[index] then return end
	local itemData = goodsTable[index]
	if player:num_bag_black() < 1 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	if addHuweiExp(player,0) then
		player:alert(1,1,"当前需要进阶后方可购买并使用")
		return
	end
	if itemData.vcion>0 then
		if player:get_vcoin()>=itemData.vcion then
			player:sub_vcoin(itemData.vcion,"购买护卫经验丹")
			-- local pos=player:add_item2(itemData.name,1,1)
			-- player:use_item(itemData.name)
			-- player:use_item_type_pos(itemData.id,pos,1)
			--player:alert(1, 1, "成功购买并使用"..itemData.name)
			addHuweiExp(player,itemData.exp,true)
			senderBuyExpData(player)
		else
			-- player:alert(1,1,"元宝不足！");
			showChongzhi(player)
		end
	end
	-- if itemData.bvcion>0 then
	-- 	local buyNum = util.ppn(player,itemData.con)
	-- 	if buyNum<itemData.maxNum then
	-- 		if player:get_vcoin_bind()>=itemData.bvcion then
	-- 			player:set_param(itemData.con,buyNum+1)
	-- 			player:sub_vcoin_bind(itemData.bvcion)
	-- 			player:add_item(itemData.name,1,1)
	-- 			player:use_item(itemData.name)
	-- 			player:alert(1, 1, "成功购买并使用"..itemData.name)
	-- 		else
	-- 			player:alert(1,1,"绑定元宝不足！");
	-- 		end
	-- 	else
	-- 		player:alert(1,1,"今日购买次数已用完！");
	-- 	end
	-- end
end

--获取身上可使用的护卫经验丹(小 中 大 顺序)
local itemNames = {"护卫经验丹（小）","护卫经验丹（中）","护卫经验丹（大）"}
function getOwnExpDan(player)
	local itemData={num=0,itemName="护卫经验丹（小）"}
	for i=1,#itemNames do
		local name = itemNames[i]
		num=player:num_item(name)
		if num>0 then
			itemData.num=num
			itemData.itemName=name
			break
		end
	end
	return itemData
end

--使用护卫经验丹
function useHuWeiDan(player)
	local flag = false
	for i=1,#itemNames do
		local name = itemNames[i]
		num=player:num_item(name)
		if num>0 then
			player:use_item(name)
			senderBuyExpData(player)
			flag=true
			break
		end
	end
	if not flag then
		player:alert(1,1,"无可用的护卫经验丹！")
	end
end


ckpanel.add_listener("gui.PanelHuWei.handlePanelData",gui.PanelHuWei.handlePanelData)


--护卫红点检测
function checkRedPoint(player)
	local isHas = player:have_slave(10)--当前有无护卫
	local slave = player:get_slave(10)
	if isHas>0 and (slave and slave:get_hp()>0) then
		-- return checkMarsUpgrade(player);
		return false
	end
	return true
end



