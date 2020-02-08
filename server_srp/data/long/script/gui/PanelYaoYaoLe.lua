module(... ,package.seeall)

local DAILY_YAO_MAX = 2--每日摇骰子次数
local DAILY_GY_MAX = 10--每日免费改运次数

--奖励表(多少个6对应的给的武魂值)
local wuHunAward = {
	[0]=50,
	[1]=100,
	[2]=150,
	[3]=250,
	[4]=500,
	[5]=1500,
	[6]=5000,
}

--每次摇骰子获得多少个6的关系表
local proTable = {
	[1]={sixnum=0,prob=10},
	[2]={sixnum=1,prob=55},
	[3]={sixnum=2,prob=35},
	[4]={sixnum=3,prob=0},
	[5]={sixnum=4,prob=0},
	[6]={sixnum=5,prob=0},
	[7]={sixnum=6,prob=0},
}

local shaiTable = {
	[1]={con=const.PP_YYL_SHAI1,fminSix=1,fmaxSix=2,vminSix=1,vmaxSix=1,
		shaiPro={
			[1]={prob=1},
			[2]={prob=1},
			[3]={prob=1},
			[4]={prob=1},
			[5]={prob=1},
			[6]={prob=1},
		}
	},
	[2]={con=const.PP_YYL_SHAI2,fminSix=1,fmaxSix=4,vminSix=1,vmaxSix=2,
		shaiPro={
			[1]={prob=1},
			[2]={prob=1},
			[3]={prob=1},
			[4]={prob=1},
			[5]={prob=1},
			[6]={prob=5},
		}
	},
	[3]={con=const.PP_YYL_SHAI3,fminSix=2,fmaxSix=5,vminSix=1,vmaxSix=4,
		shaiPro={
			[1]={prob=1},
			[2]={prob=1},
			[3]={prob=1},
			[4]={prob=1},
			[5]={prob=1},
			[6]={prob=5},
		}
	},
	[4]={con=const.PP_YYL_SHAI4,fminSix=3,fmaxSix=100,vminSix=1,vmaxSix=6,
		shaiPro={
			[1]={prob=2},
			[2]={prob=2},
			[3]={prob=2},
			[4]={prob=2},
			[5]={prob=2},
			[6]={prob=2},
		}
	},
	[5]={con=const.PP_YYL_SHAI5,fminSix=7,fmaxSix=500,vminSix=1,vmaxSix=10,
		shaiPro={
			[1]={prob=2},
			[2]={prob=2},
			[3]={prob=2},
			[4]={prob=2},
			[5]={prob=2},
			[6]={prob=2},
		}
	},
	[6]={con=const.PP_YYL_SHAI6,fminSix=8,fmaxSix=1000,vminSix=6,vmaxSix=20,
		shaiPro={
			[1]={prob=12},
			[2]={prob=12},
			[3]={prob=12},
			[4]={prob=11},
			[5]={prob=11},
			[6]={prob=1},
		}
	},

}


function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(data.actionid)
	if data.actionid == "reqYaoYaoLeTimes" then
		senderToClient(player)
	elseif data.actionid == "startYaoShaiZi" then
		startYaoShaiZi(player)
	elseif data.actionid == "startGaiYun" then
		startGaiYun(player)
	elseif data.actionid=="reqLingWuHun" then
		getWuHunAward(player)
	elseif data.actionid=="reqStartYao" then
		checkYaoTimes(player)
	elseif data.actionid=="reqStartGaiYun" then
		checkGaiYunTimes(player)
	end
end
ckpanel.add_listener("gui.PanelYaoYaoLe.handlePanelData",gui.PanelYaoYaoLe.handlePanelData)

--推送在线奖励数据
function senderToClient(player)
	local result = {
		cmd="updateTimesShow",
		yuTimes=0,--剩余摇骰次数
		mfgyTimes=0,--剩余免费改运次数
		wuHunNum=util.ppn(player,const.PP_JINRI_WUHUN),
		sixNum=0,
		curResult=getResult(player),
		curWnNum=util.ppn(player,const.PP_YYL_RECORD_WUHUN),
		isShowBtn=gui.PanelWuHun.isRedPointShow(player),
	}
	if util.ppn(player,const.PP_DAILY_YAO_NUM)<DAILY_YAO_MAX then
		result.yuTimes=DAILY_YAO_MAX-util.ppn(player,const.PP_DAILY_YAO_NUM)
	end
	if util.ppn(player,const.PP_DAILY_GY_NUM)<DAILY_GY_MAX then
		result.mfgyTimes=DAILY_GY_MAX-util.ppn(player,const.PP_DAILY_GY_NUM)
	end
	player:push_lua_table("PanelWuHun",util.encode(result))
end

--摇骰子判断
function checkYaoTimes(player)
	if (DAILY_YAO_MAX-util.ppn(player,const.PP_DAILY_YAO_NUM))<=0 then
		return player:alert(1,0,"今日摇骰子次数已用完！")
	end
	player:push_lua_table("PanelWuHun",util.encode({cmd="startYaoAction",}))
end

--改运判断
function checkGaiYunTimes(player)
	local gyTimes = DAILY_GY_MAX-util.ppn(player,const.PP_DAILY_GY_NUM)--剩余改运次数
	if gyTimes<=0 then
		local curSeInfo = getResult(player)
		local flag = true
		for i=1,#curSeInfo do
			if curSeInfo[i]~=6 then
				flag=false
				break
			end
		end
		if flag then
			player:push_lua_table("PanelWuHun",util.encode({cmd="setBtnEnable",}))
			return player:alert(1,0,"当前已达最佳结果，无需改运！")
		end
		if player:get_vcoin()<50 then
			player:push_lua_table("PanelWuHun",util.encode({cmd="setBtnEnable",}))
			return showChongzhi(player)--元宝不足
		end
	end
	player:push_lua_table("PanelWuHun",util.encode({cmd="startGaiYunAction",}))
end

--获取当前骰子结果记录
function getResult(player)
	local result = player:get_param(const.PP_YYL_SHAI_INFO)
	if result==nil or result=="" then
		result={0,0,0,0,0,0}
	else
		result=util.decode(result)
	end
	return result
end

--首次摇骰子结果
function startYaoShaiZi(player)
	if (DAILY_YAO_MAX-util.ppn(player,const.PP_DAILY_YAO_NUM))<=0 then
		return player:alert(1,0,"今日摇骰子次数已用完！")
	end
	local index = util.produceProb(proTable)
	if not proTable[index] then return end
	local sixNum = proTable[index].sixnum

	local curResult=getResult(player)
	for i=1,#curResult do
		if i<=sixNum and sixNum>0 then
			curResult[i]=6
		else
			curResult[i]=math.random(1,5)
		end
	end
	player:set_param(const.PP_YYL_SHAI_INFO,util.encode(curResult))--存贮骰子结果
	player:set_param(const.PP_DAILY_YAO_NUM,util.ppn(player,const.PP_DAILY_YAO_NUM)+1)
	local result = {
		cmd="updateShowInfo",
		curResult=curResult,
		curWnNum=wuHunAward[sixNum],
		sixNum=sixNum,
		yuTimes=0,--剩余摇骰次数
		mfgyTimes=0,--剩余免费改运次数
	}
	if util.ppn(player,const.PP_DAILY_YAO_NUM)<DAILY_YAO_MAX then
		result.yuTimes=DAILY_YAO_MAX-util.ppn(player,const.PP_DAILY_YAO_NUM)
	end
	if util.ppn(player,const.PP_DAILY_GY_NUM)<DAILY_GY_MAX then
		result.mfgyTimes=DAILY_GY_MAX-util.ppn(player,const.PP_DAILY_GY_NUM)
	end
	player:set_param(const.PP_YYL_RECORD_WUHUN,wuHunAward[sixNum])

	player:push_lua_table("PanelWuHun",util.encode(result))

	-- print(util.encode(curResult),util.ppn(player,const.PP_DAILY_YAO_NUM))
end

--改运计算方式
function startGaiYun(player)
	local curResult=getResult(player)
	local gyTimes = DAILY_GY_MAX-util.ppn(player,const.PP_DAILY_GY_NUM)--剩余改运次数

	if gyTimes<=0 then
		if player:get_vcoin()<50 then
			return showChongzhi(player)--元宝不足
		end
		player:sub_vcoin(50,"元宝改运")
	end
	player:set_param(const.PP_DAILY_GY_NUM,util.ppn(player,const.PP_DAILY_GY_NUM)+1)

	for i=1,#shaiTable do
		if curResult[i] and curResult[i]~=6 then
			local itemData = shaiTable[i]
			local curTimes = util.ppn(player,itemData.con)+1
			player:set_param(itemData.con,curTimes)
			local tempTable = util.clone(itemData.shaiPro)
			if gyTimes<=0 then--元宝改运
				if itemData.vmaxSix>0 and (curTimes%itemData.vmaxSix)==0 then
					curResult[i]=6
					break
				else
					if (curTimes%itemData.vminSix)==0 then
						local index = util.produceProb(tempTable)
						curResult[i]=index
						if index==6 then break end
					else
						local newTable = {tempTable[1],tempTable[2],tempTable[3],tempTable[4],tempTable[5]}
						local index = util.produceProb(newTable)
						curResult[i]=index
						if index==6 then break end
					end
				end
			else--免费改运
				if itemData.fmaxSix>0 and (curTimes%itemData.fmaxSix)==0 then
					curResult[i]=6
					break
				else
					if (curTimes%itemData.fminSix)==0 then
						local index = util.produceProb(tempTable)
						curResult[i]=index
						if index==6 then break end
					else
						local newTable = {tempTable[1],tempTable[2],tempTable[3],tempTable[4],tempTable[5]}
						local index = util.produceProb(newTable)
						curResult[i]=index
						if index==6 then break end
					end
				end
			end
		end
	end
	player:set_param(const.PP_YYL_SHAI_INFO,util.encode(curResult))--存贮改运骰子结果

	local sixNum = 0
	for i=1,#curResult do
		if curResult[i]==6 then sixNum=sixNum+1 end
	end

	local result = {
		cmd="updateShowInfo",
		curResult=curResult,
		curWnNum=wuHunAward[sixNum],
		sixNum=sixNum,
		yuTimes=0,--剩余摇骰次数
		mfgyTimes=0,--剩余免费改运次数
	}
	if util.ppn(player,const.PP_DAILY_YAO_NUM)<DAILY_YAO_MAX then
		result.yuTimes=DAILY_YAO_MAX-util.ppn(player,const.PP_DAILY_YAO_NUM)
	end
	if util.ppn(player,const.PP_DAILY_GY_NUM)<DAILY_GY_MAX then
		result.mfgyTimes=DAILY_GY_MAX-util.ppn(player,const.PP_DAILY_GY_NUM)
	end
	player:set_param(const.PP_YYL_RECORD_WUHUN,wuHunAward[sixNum])

	player:push_lua_table("PanelWuHun",util.encode(result))

	-- print(util.encode(curResult),"gaiyun")
end

--领取武魂值奖励
function getWuHunAward(player)
	local curWnNum=util.ppn(player,const.PP_YYL_RECORD_WUHUN)
	if curWnNum<=0 then
		return player:alert(1,0,"当前无武魂可领！")
	end
	player:set_param(const.PP_WUHUN_SCORE,util.ppn(player,const.PP_WUHUN_SCORE)+curWnNum)
	player:set_param(const.PP_JINRI_WUHUN,util.ppn(player,const.PP_JINRI_WUHUN)+curWnNum)
	player:set_param(const.PP_YYL_RECORD_WUHUN,0)
	player:set_param(const.PP_YYL_SHAI_INFO,util.encode({0,0,0,0,0,0}))--领取后重置点数
	player:alert(1,0,"成功领取武魂*"..curWnNum)
	if curWnNum>=wuHunAward[6] then
		server.info(10, 1, "<font color='#fff843'>恭喜玩家<font color='#00ccff'>"..player:get_name().."</font>摇到六六大顺获得<font color='#ff0000'>"..curWnNum.."武魂值</font><a color='#00ff00' href='event:local_OpenPanel_act42' underline='#00ff00'>我也要摇</a></font>")
	end
	local result = {
		cmd="hideLingBox",
		wuHunNum=util.ppn(player,const.PP_JINRI_WUHUN),
		isShowBtn=gui.PanelWuHun.isRedPointShow(player),
		yuTimes=0,--剩余摇骰次数
	}
	if util.ppn(player,const.PP_DAILY_YAO_NUM)<DAILY_YAO_MAX then
		result.yuTimes=DAILY_YAO_MAX-util.ppn(player,const.PP_DAILY_YAO_NUM)
	end
	gui.PanelWuHun.pushRedPoint(player)
	player:push_lua_table("PanelWuHun",util.encode(result))

	gui.moduleRedPoint.checkDiceUsable(player)--领取后判断光圈
end

--按钮光圈显示
function checkHalo(player)
	if (DAILY_YAO_MAX-util.ppn(player,const.PP_DAILY_YAO_NUM))>0 then
		return true
	end
	if util.ppn(player,const.PP_YYL_RECORD_WUHUN)>0 then
		return true
	end
	if gui.PanelWuHun.isRedPointShow( player ) then
		return true
	end
	return false
end








