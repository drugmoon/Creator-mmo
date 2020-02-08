module(... ,package.seeall)
local MAX_ZSLEV = 13--转生最大等级
local MAX_TIMES = 3--每日等级兑换修为次数
local zs_data={
	[0] ={needexp=0,       needlevel=0,  dc=0,    dcmax=0,    mc=0,    mcmax=0,    sc=0,    scmax=0,    ac=0,   acmax=0,   mac=0,   macmax=0,   zHp=0,    fHp=0,   dHp=0,    mpmax=0},
	[1] ={needexp=48600,   needlevel=90, dc=31,   dcmax=65,   mc=31,   mcmax=65,   sc=31,   scmax=65,   ac=8,   acmax=16,  mac=8,   macmax=16,  zHp=236,  fHp=84,  dHp=129,  mpmax=0},
	[2] ={needexp=133200,  needlevel=90, dc=71,   dcmax=148,  mc=71,   mcmax=148,  sc=71,   scmax=148,  ac=18,  acmax=38,  mac=18,  macmax=38,  zHp=531,  fHp=191, dHp=292,  mpmax=0},
	[3] ={needexp=287300,  needlevel=90, dc=126,  dcmax=263,  mc=126,  mcmax=263,  sc=126,  scmax=263,  ac=32,  acmax=67,  mac=32,  macmax=67,  zHp=944,  fHp=339, dHp=519,  mpmax=0},
	[4] ={needexp=582500,  needlevel=90, dc=197,  dcmax=411,  mc=197,  mcmax=411,  sc=197,  scmax=411,  ac=50,  acmax=105, mac=50,  macmax=105, zHp=1475, fHp=531, dHp=811,  mpmax=0},
	[5] ={needexp=803600,  needlevel=90, dc=284,  dcmax=593,  mc=284,  mcmax=593,  sc=284,  scmax=593,  ac=73,  acmax=152, mac=73,  macmax=152, zHp=2124, fHp=764, dHp=1168, mpmax=0},
	[6] ={needexp=1368500, needlevel=90, dc=387,  dcmax=807,  mc=387,  mcmax=807,  sc=387,  scmax=807,  ac=99,  acmax=207, mac=99,  macmax=207, zHp=2891, fHp=1041,dHp=1590, mpmax=0},
	[7] ={needexp=2212600, needlevel=90, dc=506,  dcmax=1054, mc=506,  mcmax=1054, sc=506,  scmax=1054, ac=130, acmax=271, mac=130, macmax=271, zHp=3776, fHp=1359,dHp=2077, mpmax=0},
	[8] ={needexp=3008800, needlevel=90, dc=640,  dcmax=1334, mc=640,  mcmax=1334, sc=640,  scmax=1334, ac=164, acmax=343, mac=164, macmax=343, zHp=4780, fHp=1720,dHp=2629, mpmax=0},
	[9] ={needexp=4882300, needlevel=90, dc=791,  dcmax=1647, mc=791,  mcmax=1647, sc=791,  scmax=1647, ac=203, acmax=423, mac=203, macmax=423, zHp=5901, fHp=2124,dHp=3245, mpmax=0},
	[10]={needexp=6543200, needlevel=90, dc=957,  dcmax=1994, mc=957,  mcmax=1994, sc=957,  scmax=1994, ac=245, acmax=512, mac=245, macmax=512, zHp=7140, fHp=2570,dHp=3927, mpmax=0},
	[11]={needexp=8764600, needlevel=90, dc=1139, dcmax=2373, mc=1139, mcmax=2373, sc=1139, scmax=2373, ac=292, acmax=609, mac=292, macmax=609, zHp=8498, fHp=3059,dHp=4674, mpmax=0},
	[12]={needexp=12101600,needlevel=90, dc=1336, dcmax=2785, mc=1336, mcmax=2785, sc=1336, scmax=2785, ac=343, acmax=715, mac=343, macmax=715, zHp=9973, fHp=3590,dHp=5485, mpmax=0},
	[13]={needexp=19129600,needlevel=90, dc=1550, dcmax=3230, mc=1550, mcmax=3230, sc=1550, scmax=3230, ac=398, acmax=830, mac=398, macmax=830, zHp=11567,fHp=4164,dHp=6361, mpmax=0},
 }

function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(commond)
	if data.actionid == "reqDanData" then
		senderShopData(player)
	elseif data.actionid == "danBuy" then 
		buyShopDan(player,data.params.index)
	elseif data.actionid == "reqZsData" then
		senderToClient(player)
	elseif data.actionid == "reqZhuanSheng" then
		startZhuanSheng(player)
	elseif data.actionid == "exchangeExp" then
		levelExchangeExp(player)
	end
	
end

ckpanel.add_listener("gui.PanelZhuanSheng.handlePanelData",gui.PanelZhuanSheng.handlePanelData)

function senderToClient(player,up)
	local result = {
		cmd="updateZhuanSheng",
	}
	local level = player:get_level()<=130 and player:get_level() or 130
	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	result.curData=zs_data[zsLevel]
	if zsLevel>=MAX_ZSLEV then
		result.nextData=zs_data[MAX_ZSLEV]
	else
		result.nextData=zs_data[zsLevel+1]
	end
	result.needLevel=result.nextData.needlevel
	result.needExp=result.nextData.needexp
	result.ownExp=util.ppn(player,const.PP_ZHSH_EXP)
	result.curLevel=zsLevel
	result.level=level
	result.yuTimes=MAX_TIMES-util.ppn(player,const.PP_ZHSH_NUM)
	local lastLevelExp = player:get_level_exp(level-1)
	local exchangeExp = lastLevelExp
	result.exchangeExp=tab.ZsAndSw[tostring(level)].zhuangsheng
	result.needMoney=math.floor(5000000/10000)
	result.up = up
	player:push_lua_table("PanelZhuanSheng",util.encode(result))
end

--等级兑换修为
function levelExchangeExp(player)
	local level = player:get_level()
	local lastLevelExp = player:get_level_exp(level-1)
	local exchangeExp = tonumber(tab.ZsAndSw[tostring(level)].zhuangsheng)
	local needMoney=math.floor(exchangeExp)
	if exchangeExp>0 then
		if util.ppn(player,const.PP_ZHSH_NUM) < MAX_TIMES then
			if  getAllMoney(player)>=5000000 then
				-- player:sub_gamemoney(5000000)
				subMoneyOrBind(player,5000000)
				player:set_level(level-1)
				player:set_param(const.PP_ZHSH_NUM,util.ppn(player,const.PP_ZHSH_NUM)+1)
				player:set_param(const.PP_ZHSH_EXP,util.ppn(player,const.PP_ZHSH_EXP)+exchangeExp)
				senderToClient(player)
				gui.moduleRedPoint.checkZhuanShengUsable(player)
			else
				player:alert(1,1,"金币不足无法兑换修为！")
			end
		else
			player:alert(1,1,"今日"..MAX_TIMES.."次等级兑换修为已用完！")
		end
	else
		player:alert(1,1,"当前等级不足已降级获得经验！")
	end
end

function setZhuanShengAttr(player, lv)
	local job = player:get_job_name()
	if lv > 0 and lv <= 13 then
		if job == "warrior" then
			local dc1 = player:get_script_attr(7)
			local dc2 = player:get_script_attr(8)
			local ac1 = player:get_script_attr(3)
			local ac2 = player:get_script_attr(4)
			local mac1 = player:get_script_attr(5) 
			local mac2 = player:get_script_attr(6)
			local hp = player:get_script_attr(2)
			local mp = player:get_script_attr(1)

			-- 减去前一级的
			dc1 = dc1 - zs_data[lv - 1].dc
			dc2 = dc2 - zs_data[lv - 1].dcmax
			ac1 = ac1 - zs_data[lv - 1].ac
			ac2 = ac2 - zs_data[lv - 1].acmax
			mac1 = mac1 - zs_data[lv - 1].mac
			mac2 = mac2 - zs_data[lv - 1].macmax
			hp = hp - zs_data[lv - 1].zHp
			mp = mp - zs_data[lv - 1].mpmax

			-- 加上当前级的
			dc1 = dc1 + zs_data[lv].dc
			dc2 = dc2 + zs_data[lv].dcmax
			ac1 = ac1 + zs_data[lv].ac
			ac2 = ac2 + zs_data[lv].acmax
			mac1 = mac1 + zs_data[lv].mac
			mac2 = mac2 + zs_data[lv].macmax
			hp = hp + zs_data[lv].zHp
			mp = mp + zs_data[lv].mpmax	

			player:set_script_attr(7, dc1)	
			player:set_script_attr(8, dc2)
			player:set_script_attr(3, ac1)	
			player:set_script_attr(4, ac2)
			player:set_script_attr(5, mac1)	
			player:set_script_attr(6, mac2)
			player:set_script_attr(2, hp)	
			player:set_script_attr(1, mp)
												
			player:recal_attr()	
			return
		end
		
		if job == "wizard" then
			local mc1 = player:get_script_attr(9)
			local mc2 = player:get_script_attr(10)
			local ac1 = player:get_script_attr(3)
			local ac2 = player:get_script_attr(4)
			local mac1 = player:get_script_attr(5) 
			local mac2 = player:get_script_attr(6)
			local hp = player:get_script_attr(2)
			local mp = player:get_script_attr(1)

			-- 减去前一级的
			mc1 = mc1 - zs_data[lv - 1].mc
			mc2 = mc2 - zs_data[lv - 1].mcmax
			ac1 = ac1 - zs_data[lv - 1].ac
			ac2 = ac2 - zs_data[lv - 1].acmax
			mac1 = mac1 - zs_data[lv - 1].mac
			mac2 = mac2 - zs_data[lv - 1].macmax
			hp = hp - zs_data[lv - 1].fHp
			mp = mp - zs_data[lv - 1].mpmax

			-- 加上当前级的
			mc1 = mc1 + zs_data[lv].mc
			mc2 = mc2 + zs_data[lv].mcmax
			ac1 = ac1 + zs_data[lv].ac
			ac2 = ac2 + zs_data[lv].acmax
			mac1 = mac1 + zs_data[lv].mac
			mac2 = mac2 + zs_data[lv].macmax
			hp = hp + zs_data[lv].fHp
			mp = mp + zs_data[lv].mpmax	

			player:set_script_attr(9, mc1)	
			player:set_script_attr(10, mc2)
			player:set_script_attr(3, ac1)	
			player:set_script_attr(4, ac2)
			player:set_script_attr(5, mac1)	
			player:set_script_attr(6, mac2)
			player:set_script_attr(2, hp)	
			player:set_script_attr(1, mp)	

			player:recal_attr()	
			return
		end

		if job == "taoist" then
			local sc1 = player:get_script_attr(11)
			local sc2 = player:get_script_attr(12)
			local ac1 = player:get_script_attr(3)
			local ac2 = player:get_script_attr(4)
			local mac1 = player:get_script_attr(5) 
			local mac2 = player:get_script_attr(6)
			local hp = player:get_script_attr(2)
			local mp = player:get_script_attr(1)

			-- 减去前一级的
			sc1 = sc1 - zs_data[lv - 1].sc
			sc2 = sc2 - zs_data[lv - 1].scmax
			ac1 = ac1 - zs_data[lv - 1].ac
			ac2 = ac2 - zs_data[lv - 1].acmax
			mac1 = mac1 - zs_data[lv - 1].mac
			mac2 = mac2 - zs_data[lv - 1].macmax
			hp = hp - zs_data[lv - 1].dHp
			mp = mp - zs_data[lv - 1].mpmax

			-- 加上当前级的
			sc1 = sc1 + zs_data[lv].sc
			sc2 = sc2 + zs_data[lv].scmax
			ac1 = ac1 + zs_data[lv].ac
			ac2 = ac2 + zs_data[lv].acmax
			mac1 = mac1 + zs_data[lv].mac
			mac2 = mac2 + zs_data[lv].macmax
			hp = hp + zs_data[lv].dHp
			mp = mp + zs_data[lv].mpmax		

			player:set_script_attr(11, sc1)	
			player:set_script_attr(12, sc2)
			player:set_script_attr(3, ac1)	
			player:set_script_attr(4, ac2)
			player:set_script_attr(5, mac1)	
			player:set_script_attr(6, mac2)
			player:set_script_attr(2, hp)	
			player:set_script_attr(1, mp)	

			player:recal_attr()	
			return
		end	
	end
end

function CalculateAttributeZS(player)
	local lv = util.ppn(player,const.PP_ZHSH_LEVEL)
	local job = player:get_job_name()
	if lv > 0 and lv <= 13 then
		if job == "warrior" then
			local dc1 = player:get_script_attr(7)
			local dc2 = player:get_script_attr(8)
			local ac1 = player:get_script_attr(3)
			local ac2 = player:get_script_attr(4)
			local mac1 = player:get_script_attr(5) 
			local mac2 = player:get_script_attr(6)
			local hp = player:get_script_attr(2)
			local mp = player:get_script_attr(1)
			-- 加上当前级的
			dc1 = dc1 + zs_data[lv].dc
			dc2 = dc2 + zs_data[lv].dcmax
			ac1 = ac1 + zs_data[lv].ac
			ac2 = ac2 + zs_data[lv].acmax
			mac1 = mac1 + zs_data[lv].mac
			mac2 = mac2 + zs_data[lv].macmax
			hp = hp + zs_data[lv].zHp
			mp = mp + zs_data[lv].mpmax	

			player:set_script_attr(7, dc1)	
			player:set_script_attr(8, dc2)
			player:set_script_attr(3, ac1)	
			player:set_script_attr(4, ac2)
			player:set_script_attr(5, mac1)	
			player:set_script_attr(6, mac2)
			player:set_script_attr(2, hp)	
			player:set_script_attr(1, mp)
												
			player:recal_attr()	
			return
		end
		
		if job == "wizard" then
			local mc1 = player:get_script_attr(9)
			local mc2 = player:get_script_attr(10)
			local ac1 = player:get_script_attr(3)
			local ac2 = player:get_script_attr(4)
			local mac1 = player:get_script_attr(5) 
			local mac2 = player:get_script_attr(6)
			local hp = player:get_script_attr(2)
			local mp = player:get_script_attr(1)

			-- 加上当前级的
			mc1 = mc1 + zs_data[lv].mc
			mc2 = mc2 + zs_data[lv].mcmax
			ac1 = ac1 + zs_data[lv].ac
			ac2 = ac2 + zs_data[lv].acmax
			mac1 = mac1 + zs_data[lv].mac
			mac2 = mac2 + zs_data[lv].macmax
			hp = hp + zs_data[lv].fHp
			mp = mp + zs_data[lv].mpmax	

			player:set_script_attr(9, mc1)	
			player:set_script_attr(10, mc2)
			player:set_script_attr(3, ac1)	
			player:set_script_attr(4, ac2)
			player:set_script_attr(5, mac1)	
			player:set_script_attr(6, mac2)
			player:set_script_attr(2, hp)	
			player:set_script_attr(1, mp)	

			player:recal_attr()	
			return
		end

		if job == "taoist" then
			local sc1 = player:get_script_attr(11)
			local sc2 = player:get_script_attr(12)
			local ac1 = player:get_script_attr(3)
			local ac2 = player:get_script_attr(4)
			local mac1 = player:get_script_attr(5) 
			local mac2 = player:get_script_attr(6)
			local hp = player:get_script_attr(2)
			local mp = player:get_script_attr(1)

			-- 加上当前级的
			sc1 = sc1 + zs_data[lv].sc
			sc2 = sc2 + zs_data[lv].scmax
			ac1 = ac1 + zs_data[lv].ac
			ac2 = ac2 + zs_data[lv].acmax
			mac1 = mac1 + zs_data[lv].mac
			mac2 = mac2 + zs_data[lv].macmax
			hp = hp + zs_data[lv].dHp
			mp = mp + zs_data[lv].mpmax		

			player:set_script_attr(11, sc1)	
			player:set_script_attr(12, sc2)
			player:set_script_attr(3, ac1)	
			player:set_script_attr(4, ac2)
			player:set_script_attr(5, mac1)	
			player:set_script_attr(6, mac2)
			player:set_script_attr(2, hp)	
			player:set_script_attr(1, mp)	

			player:recal_attr()	
			return
		end	
	end
end
login.add_login_listener(CalculateAttributeZS)-------上线重设buff

--执行转生操作
function startZhuanSheng(player)
	local level = player:get_level()
	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	if zsLevel>=MAX_ZSLEV then
		player:alert(1,1,"转生已达最大等级！") return
	end
	local itemData = zs_data[zsLevel+1]
	local ownExp=util.ppn(player,const.PP_ZHSH_EXP)
	if ownExp>=itemData.needexp then
		player:set_param(const.PP_ZHSH_EXP,ownExp-itemData.needexp)
		player:set_param(const.PP_ZHSH_LEVEL,zsLevel+1)
		player:set_zslevel(zsLevel+1)
		setZhuanShengAttr(player, zsLevel + 1)
		-- player:clear_status(50)
		-- player:set_status(50,86400,util.ppn(player,const.PP_ZHSH_LEVEL),1,1)
		player:alert(11,1,"转生成功!恭喜您完成[<font color='#ff0000'>"..util.ppn(player,const.PP_ZHSH_LEVEL).."级转生</font>]")
		senderToClient(player,true)
		gui.PanelAchieve.triggerTaskEvent(player, "zhuansheng", zsLevel+1)
		gui.AwardHall_jingji.setRankLevel(player)
		gui.moduleRedPoint.checkZhuanShengUsable(player)
	else
		player:alert(1,1,"您的修为不足，无法转生！")
	end
end

----------------------------------------------降级关于金币的操作-------------------------------------------------
--计算金币+绑定金币
function getAllMoney(player)
	return (player:get_gamemoney()+player:get_gamemoney_bind())
end

--关于扣金币或者绑定金币
function subMoneyOrBind(player,num)
	local money = player:get_gamemoney()
	local bindMoney = player:get_gamemoney_bind()
	if bindMoney>=num then
		player:sub_gamemoney_bind(num)
	else
		player:sub_gamemoney_bind(bindMoney)
		player:sub_gamemoney(num-bindMoney)
	end
end

----------------------------------------------转生丹快捷购买------------------------------------------------
function senderShopData(player)
	local result = {
		cmd="senderShopData",
		data=tab.Store.getItemsByType(player,3),
	}
	player:push_lua_table("PanelZhuanSheng",util.encode(result))
end

function buyShopDan(player,storeId)
	local itemData = tab.Store.buyShopItem(player,storeId,"转生经验",1,"notinbag")
	if itemData then
		senderToClient(player)
	end
end

function addZSExp(player,exp)
	if type(exp) == "number" then
		player:set_param(const.PP_ZHSH_EXP,util.ppn(player,const.PP_ZHSH_EXP)+exp)
		player:alert(1,0,"增加转生经验"..exp)
		gui.moduleRedPoint.checkZhuanShengUsable(player)
		return true
	end
end

--红点检测
function checkRedPoint(player)
	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	if zsLevel>=MAX_ZSLEV then
		return false
	end
	local ownExp=util.ppn(player,const.PP_ZHSH_EXP)
	local itemData = zs_data[zsLevel+1]
	if ownExp>=itemData.needexp then
		return true
	end
	return false
end

function getZsInfoWithLevel(lv)
	return zs_data[lv]
end