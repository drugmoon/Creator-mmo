module(... ,package.seeall)

local rankTable = {
	[1]={--等级竞技
		[1]={name="第1名",minLev=90,con=const.PP_JINGJIN_RANK1,
			award={
				{id=27020002,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=5,bind=1,effectRes = 65078},
			}
		},
		[2]={name="第2-5名",minLev=90,con=const.PP_JINGJIN_RANK2,
			award={
				{id=27020001,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=2,bind=1,effectRes = 65078},
			}
		},
	},
	[2]={--神翼竞技
		[1]={name="第1名",minLev=3,con=const.PP_JINGJIN_RANK3,
			award={
				{id=27020002,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=5,bind=1,effectRes = 65078},
			}
		},
		[2]={name="第2-5名",minLev=3,con=const.PP_JINGJIN_RANK4,
			award={
				{id=27020001,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=2,bind=1,effectRes = 65078},
			}
		},
	},
	[3]={--龙心竞技
		[1]={name="第1名",minLev=3,con=const.PP_JINGJIN_RANK5,
			award={
				{id=27020002,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=5,bind=1,effectRes = 65078},
			}
		},
		[2]={name="第2-5名",minLev=3,con=const.PP_JINGJIN_RANK6,
			award={
				{id=27020001,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=2,bind=1,effectRes = 65078},
			}
		},
	},
	[4]={--狼牙竞技
		[1]={name="第1名",minLev=3,con=const.PP_JINGJIN_RANK7,
			award={
				{id=27020002,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=5,bind=1,effectRes = 65078},
			}
		},
		[2]={name="第2-5名",minLev=3,con=const.PP_JINGJIN_RANK8,
			award={
				{id=27020001,num=1,bind=0,effectRes = 65078},
				{id=23050003,num=2,bind=1,effectRes = 65078},
			}
		},
	},

}

--竞技的全名奖励
local mingTable = {
	--等级竞技全名 svrNum记录全服已领取数
	[1]={
		[1]={needLev=80,needZLev=0,allNum=800,svrNum="levelJinJi_1",con=const.PP_JINGJIN_MING1,
			award={
				{id=0,       num=1,bind=0,effectRes = 65080},--装备
				{id=21000004,num=1,bind=1,effectRes = 65079},
				{id=32000001,num=2,bind=1,effectRes = 0},
			}
		},
		[2]={needLev=90,needZLev=0,allNum=200,svrNum="levelJinJi_2",con=const.PP_JINGJIN_MING2,
			award={
				{id=0,       num=1,bind=0,effectRes = 65080},--装备
				{id=21000004,num=1,bind=1,effectRes = 65079},
				{id=32000001,num=2,bind=1,effectRes = 0},
			}
		},
		[3]={needLev=0,needZLev=2,allNum=50,svrNum="levelJinJi_3",con=const.PP_JINGJIN_MING3,
			award={
				{id=0,       num=1,bind=0,effectRes = 65080},--装备
				{id=21000004,num=1,bind=1,effectRes = 65079},
				{id=32000002,num=2,bind=1,effectRes = 0},
			}
		},
		[4]={needLev=0,needZLev=4,allNum=15,svrNum="levelJinJi_4",con=const.PP_JINGJIN_MING4,
			award={
				{id=0,       num=1,bind=0,effectRes = 65080},--装备
				{id=21000004,num=1,bind=1,effectRes = 65079},
				{id=32000002,num=2,bind=1,effectRes = 0},
			}
		},
		[5]={needLev=0,needZLev=6,allNum=3,svrNum="levelJinJi_5",con=const.PP_JINGJIN_MING5,
			award={
				{id=0,       num=1,bind=0,effectRes = 65080},--装备
				{id=21000004,num=1,bind=1,effectRes = 65079},
				{id=32000003,num=2,bind=1,effectRes = 0},
			}
		},
	},
	--神翼竞技全名 svrNum记录全服已领取数
	[2]={
		[1]={needLev=2,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING6,
			award={
				{id=24030001,num=40,bind=1,effectRes = 65078},
				{id=32000001,num=2 ,bind=1,effectRes = 0},
				{id=23060001,num=2 ,bind=1,effectRes = 0},
			}
		},
		[2]={needLev=3,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING7,
			award={
				{id=24030001,num=200,bind=1,effectRes = 65078},
				{id=32000006,num=2  ,bind=1,effectRes = 65078},
				{id=23060001,num=4  ,bind=1,effectRes = 0},
			}
		},
		[3]={needLev=5,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING8,
			award={
				{id=24030001,num=500,bind=1,effectRes = 65078},
				{id=32000006,num=3  ,bind=1,effectRes = 65078},
				{id=23060001,num=6  ,bind=1,effectRes = 0},
			}
		},
		[4]={needLev=7,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING9,
			award={
				{id=24030001,num=800,bind=1,effectRes = 65078},
				{id=23080011,num=1  ,bind=1,effectRes = 65078},
				{id=23060001,num=8  ,bind=1,effectRes = 0},
			}
		},
		[5]={needLev=9,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING10,
			award={
				{id=24030001,num=1500,bind=1,effectRes = 65078},
				{id=23080012,num=1   ,bind=1,effectRes = 65078},
				{id=23060001,num=10  ,bind=1,effectRes = 0},
			}
		},
	},
	--龙心竞技全名 svrNum记录全服已领取数
	[3]={
		[1]={needLev=3,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING11,
			award={
				{id=23020001,num=20,bind=1,effectRes = 0},
				{id=32000001,num=2 ,bind=1,effectRes = 0},
				{id=23060001,num=2 ,bind=1,effectRes = 0},
			}
		},
		[2]={needLev=5,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING12,
			award={
				{id=23020002,num=5,bind=1,effectRes = 65080},
				{id=32000006,num=2,bind=1,effectRes = 65078},
				{id=23060001,num=4,bind=1,effectRes = 0},
			}
		},
		[3]={needLev=6,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING13,
			award={
				{id=23020002,num=10,bind=1,effectRes = 65080},
				{id=32000006,num=3 ,bind=1,effectRes = 65078},
				{id=23060001,num=6 ,bind=1,effectRes = 0},
			}
		},
		[4]={needLev=7,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING14,
			award={
				{id=23020002,num=15,bind=1,effectRes = 65080},
				{id=23080011,num=1 ,bind=1,effectRes = 65078},
				{id=23060001,num=8 ,bind=1,effectRes = 0},
			}
		},
		[5]={needLev=9,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING15,
			award={
				{id=23020002,num=20,bind=1,effectRes = 65080},
				{id=23080012,num=1 ,bind=1,effectRes = 65078},
				{id=23060001,num=10,bind=1,effectRes = 0},
			}
		},
	},
	--狼牙竞技全名 svrNum记录全服已领取数
	[4]={
		[1]={needLev=3,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING16,
			award={
				{id=23030002,num=2,bind=1,effectRes = 65080},
				{id=32000001,num=2,bind=1,effectRes = 0},
				{id=23060001,num=2,bind=1,effectRes = 0},
			}
		},
		[2]={needLev=5,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING17,
			award={
				{id=23030002,num=10,bind=1,effectRes = 65080},
				{id=32000006,num=2 ,bind=1,effectRes = 65078},
				{id=23060001,num=4 ,bind=1,effectRes = 0},
			}
		},
		[3]={needLev=6,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING18,
			award={
				{id=23030002,num=15,bind=1,effectRes = 65080},
				{id=32000006,num=3 ,bind=1,effectRes = 65078},
				{id=23060001,num=6 ,bind=1,effectRes = 0},
			}
		},
		[4]={needLev=7,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING19,
			award={
				{id=23030002,num=20,bind=1,effectRes = 65080},
				{id=23080011,num=1 ,bind=1,effectRes = 65078},
				{id=23060001,num=8 ,bind=1,effectRes = 0},
			}
		},
		[5]={needLev=9,allNum=nil,svrNum=nil,con=const.PP_JINGJIN_MING20,
			award={
				{id=23030002,num=40,bind=1,effectRes = 65080},
				{id=23080012,num=1 ,bind=1,effectRes = 65078},
				{id=23060001,num=10,bind=1,effectRes = 0},
			}
		},
	},
}

--给装备的部分
local equipaWards = {
	[200]={
		["warrior"]={11108051,11109031,11110021,11111021,11112011},
		["wizard"] ={12108051,12109031,12110021,12111021,12112011},
		["taoist"] ={13108051,13109031,13110021,13111021,13112011},
	},
	[201]={
		["warrior"]={11108051,11109031,11110091,11111091,11112011},
		["wizard"] ={12108051,12109031,12110091,12111091,12112011},
		["taoist"] ={13108051,13109031,13110091,13111091,13112011},
	},
}
local equipaWardsRes = {
	[200]={
		["warrior"]={65079,65079,65079,65079,65079},
		["wizard"] ={65079,65079,65079,65079,65079},
		["taoist"] ={65079,65079,65079,65079,65079},
	},
	[201]={
		["warrior"]={65079,65079,65079,65079,65079},
		["wizard"] ={65079,65079,65079,65079,65079},
		["taoist"] ={65079,65079,65079,65079,65079},
	},
}

--处理返回等级竞技的全名奖励表
function getNeedTable(player)
	local result = util.clone(mingTable[1])
	local job = player:get_job_name()
	local gender = player:get_gender()
	for i=1,#result do
		local itemData = result[i].award[1]
		itemData.id=equipaWards[gender][job][i]
		itemData.effectRes=equipaWardsRes[gender][job][i]
		result[i].lingNum=util.svn(result[i].svrNum)--该档次全服已领取数目
	end
	return result
end


function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(commond)
	---------数据显示
	if data.actionid == "reqLevelJingJi" then
		senderToClientLevel(player)
	elseif data.actionid == "reqShenYiJingJi" then
		senderToClientShenYi(player)
	elseif data.actionid == "reqLongXinJingJi" then
		senderToClientLongXin(player)
	elseif data.actionid == "reqLangYaJingJi" then
		senderToClientLangYa(player)
	---------奖励领取
	elseif data.actionid == "reqLevelAwrads" then
		getLevelAwards(player,data.params.index)
	elseif data.actionid == "reqShenYiAwrads" then
		getShenYiAwards(player,data.params.index)
	elseif data.actionid == "reqLongXinAwrads" then
		getLongXinAwards(player,data.params.index)
	elseif data.actionid == "reqLangYaAwrads" then
		getLangYaAwards(player,data.params.index)
	elseif data.actionid == "reqRankInfo" then
		sendToClientRank(player,data.params.index)
	----刷新红点
	elseif data.actionid == "updateRedpoint" then
		gui.moduleRedPoint.checkLevelRanking(player)
		gui.moduleRedPoint.checkWingRanking(player)
		gui.moduleRedPoint.checkLongXinRanking(player)
		gui.moduleRedPoint.checkLangYaRanking(player)
		gui.moduleRedPoint.checkOpenSvrBossUsable(player)
		gui.moduleRedPoint.checkKfLeiChong(player)
		gui.moduleRedPoint.checkKfLxRecharge(player)
		gui.moduleRedPoint.checkSevenDay(player)
	end
end

--全名领取状态排序
local sort = function(a,b)
	if a.ling~=b.ling then
		return a.ling < b.ling
	else
		return a.index < b.index
	end
end

--推送排名显示
local svrTable = {"svr_rank_name_","wing_rank_name_","longxin_rank_name_","langya_rank_name_"}
function sendToClientRank(player,index)
	local result = {
		cmd="updateRankData",
		rankTable={},
	}
	for i=1,5 do
		table.insert(result.rankTable,server.get_var(svrTable[index]..i))
	end
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--转换活动时间描述 startday开区第几天开始 daynum:持续几天
function getNeedTimeFormat(startday,daynum)
	local str1 = os.date("%y年%m月%d日",os.time()-24*3600*server_start_day()+(startday-1)*24*3600)
	local str2 = os.date("%y年%m月%d日",os.time()-24*3600*server_start_day()+(startday-1+daynum-1)*24*3600)
	return str1.."-"..str2
end

--推送等级排名和等级全名
function senderToClientLevel(player)
	local result = {
		cmd="updateLevelJingJi",
		actTime=getNeedTimeFormat(1,7),
		actDesp="活动期间，人物满足相应条件即可领取奖励，数量有限，先到先得！",
		rankTable=util.clone(rankTable[1]),
		mingTable=getNeedTable(player),
		todaytime=(24*60*60-(os.time()+8*60*60))%(24*60*60),
	}
	local severDay = server_start_day()+1
	if severDay>1 then result.todaytime=0 end
	local level = player:get_level()
	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	for i=1,#result.mingTable do
		local itemData = result.mingTable[i]
		itemData.index=i
		if itemData.needLev>0 then
			if itemData.needLev>level then
				itemData.ling=2--未完成
			else
				if util.ppn(player,itemData.con)<=0 then
					itemData.ling=1--可领取
				else
					itemData.ling=3--已领取
				end
			end
		else
			if itemData.needZLev>zsLevel then
				itemData.ling=2--未完成
			else
				if util.ppn(player,itemData.con)<=0 then
					itemData.ling=1--可领取
				else
					itemData.ling=3--已领取
				end
			end
		end
	end
	table.sort(result.mingTable,sort)
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--推送神翼排名和神翼全民
function senderToClientShenYi(player)
	local result = {
		cmd="updateShenYiJingJi",
		actTime=getNeedTimeFormat(2,6),
		actDesp="活动期间，人物满足相应条件即可领取奖励",
		rankTable=util.clone(rankTable[2]),
		mingTable=util.clone(mingTable[2]),
		todaytime=(24*60*60-(os.time()+8*60*60))%(24*60*60),
	}
	local severDay = server_start_day()+1
	if severDay>2 then result.todaytime=0 end
	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	for i=1,#result.mingTable do
		local itemData = result.mingTable[i]
		itemData.index=i
		if itemData.needLev>wingLv then
			itemData.ling=2--未完成
		else
			if util.ppn(player,itemData.con)<=0 then
				itemData.ling=1--可领取
			else
				itemData.ling=3--已领取
			end
		end
	end
	table.sort(result.mingTable,sort)
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--推送龙心排名和龙心全民
function senderToClientLongXin(player)
	local result = {
		cmd="updateLongXinJingJi",
		actTime=getNeedTimeFormat(3,5),
		actDesp="活动期间，人物满足相应条件即可领取奖励",
		rankTable=util.clone(rankTable[3]),
		mingTable=util.clone(mingTable[3]),
		todaytime=(24*60*60-(os.time()+8*60*60))%(24*60*60),
	}
	local severDay = server_start_day()+1
	if severDay>3 then result.todaytime=0 end
	local lxLv = getLongXinLevel(player)
	for i=1,#result.mingTable do
		local itemData = result.mingTable[i]
		itemData.index=i
		if itemData.needLev>lxLv then
			itemData.ling=2--未完成
		else
			if util.ppn(player,itemData.con)<=0 then
				itemData.ling=1--可领取
			else
				itemData.ling=3--已领取
			end
		end
	end
	table.sort(result.mingTable,sort)
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

--推送狼牙排名和狼牙全民
function senderToClientLangYa(player)
	local result = {
		cmd="updateLangYaJingJi",
		actTime=getNeedTimeFormat(4,4),
		actDesp="活动期间，人物满足相应条件即可领取奖励",
		rankTable=util.clone(rankTable[4]),
		mingTable=util.clone(mingTable[4]),
		todaytime=(24*60*60-(os.time()+8*60*60))%(24*60*60),
	}
	local severDay = server_start_day()+1
	if severDay>4 then result.todaytime=0 end
	local lyLv = getLangYaLevel(player)
	for i=1,#result.mingTable do
		local itemData = result.mingTable[i]
		itemData.index=i
		if itemData.needLev>lyLv then
			itemData.ling=2--未完成
		else
			if util.ppn(player,itemData.con)<=0 then
				itemData.ling=1--可领取
			else
				itemData.ling=3--已领取
			end
		end
	end
	table.sort(result.mingTable,sort)
	player:push_lua_table("PanelRewardHall",util.encode(result))
end


--取龙心的等级
function getLongXinLevel(player)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(const.ITEM_DRAGON_HEART_POSITION);
	local level = 0
	if ittype_id and tonumber(ittype_id)>0  then
		level= tonumber(ittype_id)-14300001+1
	end
	-- print("longxin",level)
	return level
end

--取狼牙的等级
function getLangYaLevel(player)
	local ittype_id,itbind,ituse_bind,itjpin,itdura,itdurmax,itaddac,itaddmac,itadddc,itaddmc,itaddsc,itluck,qianghualv,
	qianghuacount,itupdad,itupdmac,itupddc,itupdmc,itupdsc,protect,addhp,addmp,remaintime,mUpdMaxCount,mUpdFailedCount = player:get_item_data_by_pos(const.ITEM_WOLFANG_POSITION);
	local level = 0
	if ittype_id and tonumber(ittype_id)>0  then
		level= tonumber(ittype_id)-14400001+1
	end
	-- print("langya",level)
	return level
end

--等级全名奖励领取部分
function getLevelAwards(player,index)
	local level = player:get_level()
	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	local mingTable = getNeedTable(player)
	local itemData = mingTable[index]
	if not itemData then
		player:alert(1,1,"数据出现了异常！")
		return
	end
	if util.ppn(player,itemData.con)>=1 then
		player:alert(1,1,"不能重复领取！")
		return
	end
	if itemData.allNum and itemData.lingNum>=itemData.allNum then
		player:alert(1,1,"该档次奖励已被领取完！")
		return
	end
	if level<itemData.needLev or zsLevel<itemData.needZLev then
		player:alert(1,1,"等级未达标！")
		return
	end
	if player:num_bag_black() < 3 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#itemData.award do
		local awardData = itemData.award[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(itemData.con,1)
	server.set_var(itemData.svrNum,itemData.lingNum+1)--领取数量+1
	player:alert(1,1,"等级奖励领取成功！")
	senderToClientLevel(player)
	gui.moduleRedPoint.checkLevelRanking(player)
	if itemData.needLev>0 then
		server.info(100,0,"<font color='#00ccff'>"..player:get_name().."</font>成功领取<font color='#ff0000'>"..itemData.needLev.."级等级礼包</font>,获得海量奖励！")
	elseif itemData.needZLev>0 then
		server.info(100,0,"<font color='#00ccff'>"..player:get_name().."</font>成功领取<font color='#ff0000'>"..itemData.needZLev.."转等级礼包</font>,获得海量奖励！")
	end
end

--神翼全名奖励领取部分
function getShenYiAwards(player,index)
	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	local mingTable = util.clone(mingTable[2])
	local itemData = mingTable[index]
	if not itemData then
		player:alert(1,1,"数据出现了异常！")
		return
	end
	if util.ppn(player,itemData.con)>=1 then
		player:alert(1,1,"不能重复领取！")
		return
	end
	-- if itemData.allNum and itemData.lingNum>=itemData.allNum then
	-- 	player:alert(1,1,"该档次奖励已被领取完！")
	-- 	return
	-- end
	if wingLv<itemData.needLev then
		player:alert(1,1,"等级未达标！")
		return
	end
	if player:num_bag_black() < 3 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#itemData.award do
		local awardData = itemData.award[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(itemData.con,1)
	player:alert(1,1,"神翼奖励领取成功！")
	senderToClientShenYi(player)
	gui.moduleRedPoint.checkWingRanking(player)
	if itemData.needLev>0 then
		server.info(100,0,"<font color='#00ccff'>"..player:get_name().."</font>成功领取<font color='#ff0000'>"..itemData.needLev.."阶翅膀礼包</font>,获得海量奖励！")
	end
end

--龙心全名奖励领取部分
function getLongXinAwards(player,index)
	local lxLv = getLongXinLevel(player)
	local mingTable = util.clone(mingTable[3])
	local itemData = mingTable[index]
	if not itemData then
		player:alert(1,1,"数据出现了异常！")
		return
	end
	if util.ppn(player,itemData.con)>=1 then
		player:alert(1,1,"不能重复领取！")
		return
	end
	-- if itemData.allNum and itemData.lingNum>=itemData.allNum then
	-- 	player:alert(1,1,"该档次奖励已被领取完！")
	-- 	return
	-- end
	if lxLv<itemData.needLev then
		player:alert(1,1,"等级未达标！")
		return
	end
	if player:num_bag_black() < 3 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#itemData.award do
		local awardData = itemData.award[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(itemData.con,1)
	player:alert(1,1,"龙心奖励领取成功！")
	senderToClientLongXin(player)
	gui.moduleRedPoint.checkLongXinRanking(player)
	if itemData.needLev>0 then
		server.info(100,0,"<font color='#00ccff'>"..player:get_name().."</font>成功领取<font color='#ff0000'>"..itemData.needLev.."级龙心礼包</font>,获得海量奖励！")
	end
end

--狼牙全名奖励领取部分
function getLangYaAwards(player,index)
	local lyLv = getLangYaLevel(player)
	local mingTable = util.clone(mingTable[4])
	local itemData = mingTable[index]
	if not itemData then
		player:alert(1,1,"数据出现了异常！")
		return
	end
	if util.ppn(player,itemData.con)>=1 then
		player:alert(1,1,"不能重复领取！")
		return
	end
	-- if itemData.allNum and itemData.lingNum>=itemData.allNum then
	-- 	player:alert(1,1,"该档次奖励已被领取完！")
	-- 	return
	-- end
	if lyLv<itemData.needLev then
		player:alert(1,1,"等级未达标！")
		return
	end
	if player:num_bag_black() < 3 then
		player:alert(1,1,"背包空间不足！")
		return
	end
	for i=1,#itemData.award do
		local awardData = itemData.award[i]
		player:add_item(awardData.id,awardData.num,awardData.bind)
	end
	player:set_param(itemData.con,1)
	player:alert(1,1,"狼牙奖励领取成功！")
	senderToClientLangYa(player)
	gui.moduleRedPoint.checkLangYaRanking(player)
	if itemData.needLev>0 then
		server.info(100,0,"<font color='#00ccff'>"..player:get_name().."</font>成功领取<font color='#ff0000'>"..itemData.needLev.."级狼牙礼包</font>,获得海量奖励！")
	end
end

--计算等级排名前五的
function setRankLevel(player)
	local curLev = player:get_level()
	local zsLev = util.ppn(player,const.PP_ZHSH_LEVEL)
	local serverDay = server_start_day()+1
	if serverDay<=1 and curLev>=90 then--只计算开区第一天且参与最小等级90
		local rankData={
			{name=server.get_var("svr_rank_name_1"),level=util.svn("svr_rank_level_1"),zslevel=util.svn("svr_rank_zslevel_1")},--1
			{name=server.get_var("svr_rank_name_2"),level=util.svn("svr_rank_level_2"),zslevel=util.svn("svr_rank_zslevel_2")},--2
			{name=server.get_var("svr_rank_name_3"),level=util.svn("svr_rank_level_3"),zslevel=util.svn("svr_rank_zslevel_3")},--3
			{name=server.get_var("svr_rank_name_4"),level=util.svn("svr_rank_level_4"),zslevel=util.svn("svr_rank_zslevel_4")},--4
			{name=server.get_var("svr_rank_name_5"),level=util.svn("svr_rank_level_5"),zslevel=util.svn("svr_rank_zslevel_5")},--5
		}
		local tempTable = util.clone(rankData)
		for i=1,#tempTable do
			local itemData = tempTable[i]
			if itemData.name==player:get_name() then
				table.remove(tempTable,i)
				break
			end
		end
		for j=1,5 do
			if tempTable[j] then
				if zsLev>tempTable[j].zslevel then
					table.insert(tempTable,j,{name=player:get_name(),level=curLev,zslevel=zsLev})
					break
				else
					if zsLev==tempTable[j].zslevel and curLev>tempTable[j].level then
						table.insert(tempTable,j,{name=player:get_name(),level=curLev,zslevel=zsLev})
						break
					end
				end
			else
				table.insert(tempTable,j,{name=player:get_name(),level=curLev,zslevel=zsLev})
				break
			end
		end
		for n=1,5 do
			if tempTable[n] then
				server.set_var("svr_rank_name_"..n,tempTable[n].name)
				server.set_var("svr_rank_level_"..n,tempTable[n].level)
				server.set_var("svr_rank_zslevel_"..n,tempTable[n].zslevel)
			else
				server.set_var("svr_rank_name_"..n,"")
				server.set_var("svr_rank_level_"..n,0)
				server.set_var("svr_rank_zslevel_"..n,0)
			end
		end
	end
	gui.moduleRedPoint.checkLevelRanking(player)
end

--计算神翼排名前五的
function setRankWing(player)
	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	local serverDay = server_start_day()+1
	if serverDay<=2 and wingLv>=3 then
		local rankData={
			{name=server.get_var("wing_rank_name_1"),level=util.svn("wing_rank_level_1")},--1
			{name=server.get_var("wing_rank_name_2"),level=util.svn("wing_rank_level_2")},--2
			{name=server.get_var("wing_rank_name_3"),level=util.svn("wing_rank_level_3")},--3
			{name=server.get_var("wing_rank_name_4"),level=util.svn("wing_rank_level_4")},--4
			{name=server.get_var("wing_rank_name_5"),level=util.svn("wing_rank_level_5")},--5
		}
		local tempTable = util.clone(rankData)
		for i=1,#tempTable do
			local itemData = tempTable[i]
			if itemData.name==player:get_name() then
				table.remove(tempTable,i)
				break
			end
		end
		for j=1,5 do
			if not tempTable[j] or wingLv>tempTable[j].level then
				table.insert(tempTable,j,{name=player:get_name(),level=wingLv})
				break
			end
		end
		for n=1,5 do
			if tempTable[n] then
				server.set_var("wing_rank_name_"..n,tempTable[n].name)
				server.set_var("wing_rank_level_"..n,tempTable[n].level)
			else
				server.set_var("wing_rank_name_"..n,"")
				server.set_var("wing_rank_level_"..n,0)
			end
		end
	end
	gui.moduleRedPoint.checkWingRanking(player)
end

--计算龙心排名前五的
function setRankLongXin(player)
	local lxLv = getLongXinLevel(player)
	local serverDay = server_start_day()+1
	if serverDay<=3 and lxLv>=3 then
		local rankData={
			{name=server.get_var("longxin_rank_name_1"),level=util.svn("longxin_rank_level_1")},--1
			{name=server.get_var("longxin_rank_name_2"),level=util.svn("longxin_rank_level_2")},--2
			{name=server.get_var("longxin_rank_name_3"),level=util.svn("longxin_rank_level_3")},--3
			{name=server.get_var("longxin_rank_name_4"),level=util.svn("longxin_rank_level_4")},--4
			{name=server.get_var("longxin_rank_name_5"),level=util.svn("longxin_rank_level_5")},--5
		}
		local tempTable = util.clone(rankData)
		for i=1,#tempTable do
			local itemData = tempTable[i]
			if itemData.name==player:get_name() then
				table.remove(tempTable,i)
				break
			end
		end
		for j=1,5 do
			if not tempTable[j] or lxLv>tempTable[j].level then
				table.insert(tempTable,j,{name=player:get_name(),level=lxLv})
				break
			end
		end
		for n=1,5 do
			if tempTable[n] then
				server.set_var("longxin_rank_name_"..n,tempTable[n].name)
				server.set_var("longxin_rank_level_"..n,tempTable[n].level)
			else
				server.set_var("longxin_rank_name_"..n,"")
				server.set_var("longxin_rank_level_"..n,0)
			end
		end
	end
	gui.moduleRedPoint.checkLongXinRanking(player)
end

--计算狼牙排名前五的
function setRankLangYa(player)
	local lyLv = getLangYaLevel(player)
	local serverDay = server_start_day()+1
	if serverDay<=4 and lyLv>=3 then
		local rankData={
			{name=server.get_var("langya_rank_name_1"),level=util.svn("langya_rank_level_1")},--1
			{name=server.get_var("langya_rank_name_2"),level=util.svn("langya_rank_level_2")},--2
			{name=server.get_var("langya_rank_name_3"),level=util.svn("langya_rank_level_3")},--3
			{name=server.get_var("langya_rank_name_4"),level=util.svn("langya_rank_level_4")},--4
			{name=server.get_var("langya_rank_name_5"),level=util.svn("langya_rank_level_5")},--5
		}
		local tempTable = util.clone(rankData)
		for i=1,#tempTable do
			local itemData = tempTable[i]
			if itemData.name==player:get_name() then
				table.remove(tempTable,i)
				break
			end
		end
		for j=1,5 do
			if not tempTable[j] or lyLv>tempTable[j].level then
				table.insert(tempTable,j,{name=player:get_name(),level=lyLv})
				break
			end
		end
		for n=1,5 do
			if tempTable[n] then
				server.set_var("langya_rank_name_"..n,tempTable[n].name)
				server.set_var("langya_rank_level_"..n,tempTable[n].level)
			else
				server.set_var("langya_rank_name_"..n,"")
				server.set_var("langya_rank_level_"..n,0)
			end
		end
	end
	gui.moduleRedPoint.checkLangYaRanking(player)
end

--邮件发放奖励  local svrTable = {"svr_rank_name_","wing_rank_name_","longxin_rank_name_","langya_rank_name_"}
function sendRankAwards(player)
	local serverDay = server_start_day()+1
	local myName = player:get_name()
	--等级竞技
	if serverDay==2 and util.ppn(player,const.PP_RANK_MAIL_AWARD1)<=0 then
		if myName==server.get_var("svr_rank_name_1") then
			util.sendMail(player,"等级竞技", "您在开服第1天的等级竞技中名列前茅\n获得第1名奖励", rankTable[1][1].award)
		end
		for i=2,5 do
			if myName==server.get_var("svr_rank_name_"..i) then
				util.sendMail(player,"等级竞技", "您在开服第1天的等级竞技中名列前茅\n获得第"..i.."名奖励", rankTable[1][2].award)
			end
		end
		player:set_param(const.PP_RANK_MAIL_AWARD1,1)
	end
	--神翼竞技
	if serverDay==3 and util.ppn(player,const.PP_RANK_MAIL_AWARD2)<=0 then
		if myName==server.get_var("wing_rank_name_1") then
			util.sendMail(player,"神翼竞技", "您在开服第2天的神翼竞技中名列前茅\n获得第1名奖励", rankTable[2][1].award)
		end
		for i=2,5 do
			if myName==server.get_var("wing_rank_name_"..i) then
				util.sendMail(player,"神翼竞技", "您在开服第2天的神翼竞技中名列前茅\n获得第"..i.."名奖励", rankTable[2][2].award)
			end
		end
		player:set_param(const.PP_RANK_MAIL_AWARD2,1)
	end
	--龙心竞技
	if serverDay==4 and util.ppn(player,const.PP_RANK_MAIL_AWARD3)<=0 then
		if myName==server.get_var("longxin_rank_name_1") then
			util.sendMail(player,"龙心竞技", "您在开服第3天的龙心竞技中名列前茅\n获得第1名奖励", rankTable[3][1].award)
		end
		for i=2,5 do
			if myName==server.get_var("longxin_rank_name_"..i) then
				util.sendMail(player,"龙心竞技", "您在开服第3天的龙心竞技中名列前茅\n获得第"..i.."名奖励", rankTable[3][2].award)
			end
		end
		player:set_param(const.PP_RANK_MAIL_AWARD3,1)
	end
	--狼牙竞技
	if serverDay==5 and util.ppn(player,const.PP_RANK_MAIL_AWARD4)<=0 then
		if myName==server.get_var("langya_rank_name_1") then
			util.sendMail(player,"狼牙竞技", "您在开服第4天的狼牙竞技中名列前茅\n获得第1名奖励", rankTable[4][1].award)
		end
		for i=2,5 do
			if myName==server.get_var("langya_rank_name_"..i) then
				util.sendMail(player,"狼牙竞技", "您在开服第4天的狼牙竞技中名列前茅\n获得第"..i.."名奖励", rankTable[4][2].award)
			end
		end
		player:set_param(const.PP_RANK_MAIL_AWARD4,1)
	end
end


ckpanel.add_listener("gui.AwardHall_jingji.handlePanelData",gui.AwardHall_jingji.handlePanelData)

--竞技奖励领取2级公告提示
local tipsArr = {"等级竞技","神翼竞技","龙心竞技","狼牙竞技"}
function getMailTips(player,mailName)
	if table.indexof(tipsArr,tostring(mailName)) then
		server.info(100,0,"<font color='#00ccff'>"..player:get_name().."</font>成功领取<font color='#ff0000'>"..mailName.."</font>".."礼包,获得海量奖励！")
	end
end


--检测等级竞技红点
function checkLevelRedPoint(player)
	local tempTable = util.clone(mingTable[1])
	local level = player:get_level()
	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if util.ppn(player,itemData.con)<=0 and itemData.allNum>util.svn(itemData.svrNum) and level>=itemData.needLev and zsLevel>=itemData.needZLev then
			return true
		end
	end
	return false
end

--检测翅膀竞技红点
function checkWingRedPoint(player)
	local tempTable = util.clone(mingTable[2])
	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if util.ppn(player,itemData.con)<=0 and  wingLv>=itemData.needLev then
			return true
		end
	end
	return false
end

--检测龙心竞技红点
function checkLxRedPoint(player)
	local tempTable = util.clone(mingTable[3])
	local lxLv = getLongXinLevel(player)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if util.ppn(player,itemData.con)<=0 and  lxLv>=itemData.needLev then
			return true
		end
	end
	return false
end

--检测狼牙竞技红点
function checkLyRedPoint(player)
	local tempTable = util.clone(mingTable[4])
	local lyLv = getLangYaLevel(player)
	for i=1,#tempTable do
		local itemData = tempTable[i]
		if util.ppn(player,itemData.con)<=0 and  lyLv>=itemData.needLev then
			return true
		end
	end
	return false
end

