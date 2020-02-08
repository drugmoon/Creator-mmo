module(..., package.seeall)

local dailyData = {

	[1]={
			[1]={exp=90000000 , vcion=888},

			[2]={exp=180000000 , vcion=2888},

			[3]={exp=90000000 , vcion=888},

			[4]={exp=180000000, vcion=2888},
			
		},
	[2]={
			[1]={exp=150000000 , vcion=1888},

			[2]={exp=290000000 , vcion=4888},

			[3]={exp=150000000 , vcion=1888},

			[4]={exp=290000000, vcion=4888}
		},
	[3]={
			[1]={exp=150000000 , vcion=1888},

			[2]={exp=290000000 , vcion=4888},

			[3]={exp=150000000 , vcion=1888},

			[4]={exp=290000000, vcion=4888}
		},
}

local expData={}


-- function handlePanelData(player,commond)
-- 	if commond == "getMakeExp" then
-- 		checkCondition(player)
-- 	elseif commond == "getPanelData" then
-- 		-- local severDay = tonumber(server_start_day())+1
-- 		-- expData={}
-- 		-- -- print(severDay,severDay,#dailyData)
-- 		-- if severDay>#dailyData then
-- 		-- 	return 
-- 		-- else
-- 		-- 	expData = dailyData[severDay]
-- 		-- end
-- 		---local data=getNeedPanelData(player)
-- 		local st = tonumber(server.get_var("server_start_time"))+48*60*60-os.time()
	
--  		local data={exp=123456 , vcion=1888,times=3,time=1000*st}
-- 		player:push_lua_table("panelNeedData",util.encode(data))
-- 	end
-- end

function handlePanelData(player,commond)
	if commond == "getMakeExp" then----购买
		checkCondition(player)
	elseif commond == "getPanelData" then---获取数据
		
		local severDay = tonumber(server_start_day())+1
		expData={}
		-- print(severDay,severDay,#dailyData)
		if severDay>#dailyData then
			player:alert(1,1,"开区前"..severDay.."天该功能不可用！")
			return
		else
			expData = dailyData[severDay]
		end
		local data=getNeedPanelData(player)
		player:push_lua_table("panelNeedData",util.encode(data))
	--elseif commond == "confirmCallBack" then		
	end
end

function getNeedPanelData(player)
	--print(os.time(),tonumber(server.get_var("server_start_time")))
	local server_time=tonumber(server.get_var("server_start_time"))
	local todaytime=(24*60*60-(server_time+8*60*60))%(24*60*60)
	local makeExpTime=todaytime+(48*60*60)+server_time
	local time=(makeExpTime-os.time())*1000
	--print(todaytime)
	--(((24*60*60)-(tonumber(server.get_var("server_start_time"))+8*60*60))%(24*60*60))+(48*60*60))-os.time())*1000)
	--print(util.ppn(player, const.PP_EXP_DAILY_TIMES))
	local data = {}
	local day = tonumber(server_start_day())+1
	local times = tonumber(util.ppn(player, const.PP_EXP_DAILY_TIMES))
	local hourDay=24-tonumber(os.date("%H", tonumber(server.get_var("server_start_time"))))-----------开区当天，距离24点还有多少小时
	--print("_____",hourDay)
	-- if times=="" then times=0 end
	-- if times<#expData then
	-- 	i=times+1
	-- else
	-- 	i=times
	-- end
	if expData[times+1] then 
		data.exp = expData[times+1].exp
		data.vcion = expData[times+1].vcion
	else
		data.exp = expData[times].exp
		data.vcion = expData[times].vcion
	end
	data.upLev = upToLevel(player,expData[day].exp+player:get_exp(),player:get_level())
	data.times = #expData-times
	data.time = time
	return data
end

function checkCondition(player)
	local times = util.ppn(player, const.PP_EXP_DAILY_TIMES)
	if tonumber(server_start_day()) >#dailyData then
		player:alert(1,1,"开区前"..#dailyData.."天才可使用该功能！")
		return
	end
	--print(#expData,tonumber(server_start_day())+1)
	
	--print(tonumber(server_start_day()))
	if times>=#expData then
		player:alert(1,1,"您的今日的经验炼制次数已用完，谢谢！")
		player:push_lua_table("close",util.encode({name = "extend_makeExp"}))
		return
	else
		if player:get_vcoin()>=expData[times+1].vcion then
			btnSure(player)
			checkRedPoint(player)
			gui.moduleGuiButton.checkOpenFunc(player);
			--gui.moduleGuiButton.checkFuncOpened(player, 20131)
			-- local result={}
			-- result.str = "是否使用"..expData[times+1].vcion.."元宝购买"..expData[times+1].exp.."经验？"
			-- result.labelConfirm = "确定"
			-- result.labelCancel = "取消"
			-- result.callFunc = "gui.PanelMakeExp.btnSure"
			-- result.book = ""
			-- player:push_lua_table("showConfirm",util.encode(result))
		else
			showChongzhi(player,expData[times+1].vcion)
		end
	end

end

function btnSure(player)
	--print("0000000000")
	local times = util.ppn(player, const.PP_EXP_DAILY_TIMES)
	
	player:add_exp(expData[times+1].exp)
	player:sub_vcoin(expData[times+1].vcion, "经验炼制")
	player:set_param(const.PP_EXP_DAILY_TIMES,times+1)
	player:push_lua_table("panelNeedData",util.encode(getNeedPanelData(player)))	
	if util.ppn(player, const.PP_EXP_DAILY_TIMES)==4 then 
		player:push_lua_table("close",util.encode({name = "extend_makeExp"}))
	end
	gui.moduleRedPoint.checkMakeExpUsable(player);
	
end
ckpanel.add_listener("gui.PanelMakeExp.btnSure", gui.PanelMakeExp.btnSure);

function upToLevel(player,extraExp,lev)

	if extraExp>=player:get_level_exp(lev) then
		extraExp=extraExp-player:get_level_exp(lev)
		lev = lev+1
		return upToLevel(player,extraExp,lev)
	else
		return lev
	end
end

ckpanel.add_listener("gui.PanelMakeExp.handlePanelData",gui.PanelMakeExp.handlePanelData)

--红点检测
function checkRedPoint(player)
	local flag=gui.moduleGuiButton.checkFuncOpened(player, 20131)
	if flag and util.ppn(player, const.PP_EXP_DAILY_TIMES)  <4 then 
		return true
	else
		return false
	end 
end