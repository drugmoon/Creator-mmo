--皇城管理员
module(..., package.seeall)

util.use_function(_M,task.task1000);
local NEED_LEVEL = 60
local warStartDay = 4;--开服第四天开启
local mergeFirstWarDay = 3;--合服第四天开启
local mergeSecondWarDay = 5;--合服第四天开启

local leaderAwards = {
	con = const.PP_KING_CITY_GUILD_LEADER_AWARD,
	award = {
		{name="元宝", id = 40000005, num = 10000},
	}
}

local memberAwards = {
	con = const.PP_KING_CITY_GUILD_MEMBER_AWARD,
	award = {
	}
}

local IMPERICAL_TIME = "20:00-21:00"

local cityInfo = {
	strs1 ={
		"<font color=#b2a58b>    群雄赴高地，热血战皇城！决胜皇城，让我们战个痛快！</font>",
	},
	strs2 ={
		"<font color=#b2a58b>1. 进入条件：<font color=#30ff00>60级</font>，限时<font color=#30ff00>1</font>小时</font>",
		"<font color=#b2a58b>2. 开启时间：开服<font color=#30ff00>第4天</font></font>",
		"<font color=#b2a58b>             合服<font color=#30ff00>第3、5天</font></font>",
		"<font color=#b2a58b>3. 攻城时间:</font><font color=#30ff00>20:00-21:00</font>",
	    "<font color=#b2a58b>4.<font color=#30ff00> 任意</font>帮会申请，<font color=#30ff00>所有</font>帮会可参战</font>",
	    "<font color=#b2a58b>5. 申请需<font color=#30ff00>1个攻城号角</font>，次日开战</font>",
	    "<font color=#b2a58b>6.<font color=#30ff00> 摧毁雕像</font>才能进入皇宫，最终</font>",
		"<font color=#b2a58b>   占领皇宫的帮会获胜！</font>",
		}
}
function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="city",talk_str = cityInfo}));
end

function getPlayerInfo(...)
	local t = {...}
	-- print("----",util.encode(t))
	local paramsNum = #t
	local info = {}
	if paramsNum>=2 then
		local playerNum = t[1]
		if playerNum and checknumber(playerNum)>0 then
			for i=1,playerNum do
				local viceinfo = {}
				viceinfo.name = t[5*(i-1)+1+1];
				viceinfo.cloth = t[5*(i-1)+2+1];
				viceinfo.weapon = t[5*(i-1)+3+1];
				viceinfo.wing = t[5*(i-1)+4+1];
				viceinfo.gender = t[5*(i-1)+5+1];
				table.insert(info,viceinfo)
			end
			-- server.set_var(var,util.encode(info))
		end
	end
	return info
end

local weekDay = {"周一","周二","周三","周四","周五","周六","周日"}
function freshImpericalCity(player)
	local result = {}
	result.cmd = "panel"
	if isWarOpen() then
		local week = util.getWeekday(server.get_var("kingCityOpenTime"))
		result.openTime = weekDay[week].. IMPERICAL_TIME
	else
		result.openTime = "无人问津"
	end
	if server.is_war()~=1 then
		local guildName = server.get_king_guild()
		if guildName and guildName~="" then
			result.guildName = guildName
			local kingGuild = server.find_guild(guildName)
			if kingGuild then
				result.kingInfo = getPlayerInfo(kingGuild:get_admin_info())
				result.ViceAdminInfo = getPlayerInfo(kingGuild:get_vice_admin_info())
				result.ViceInfo = getPlayerInfo(kingGuild:get_adv_info())
			end
		end
		-- result.kingInfo = util.decode(server.get_var("kingInfo"))
		-- result.ViceAdminInfo = util.decode(server.get_var("kingGuildViceAdmin"))
		-- result.ViceInfo = util.decode(server.get_var("kingGuildVice"))
	else
		result.guildName = ""
		result.kingInfo = ""
		result.ViceAdminInfo =""
		result.ViceInfo = ""
	end
	player:push_lua_table("PanelImpericalCity",util.encode(result))
end

function onPanelData(player, data)
	local d = util.decode(data)
	if not d then return end
	if d.cmd == "cityPanel" then
		checkAutoStart()
		freshImpericalCity(player)
	elseif d.cmd =="openKingCity" then
		checkAutoStart()
		local guildName = player:get_guild();
		local guild = nil;
		if guildName ~= "" then
			guild = server.find_guild(guildName);
		end
		if isWarOpen() or server.is_war()>0 then --是否有会长开启皇城战
			-- if server.is_war()==0 then
			-- 	if isWarOpenToday() then
			-- 		return util.alert_msg(4, "皇城战将于今晚20:00开启！", player)
			-- 	else
			-- 		return util.alert_msg(4, "皇城战将于明晚20:00开启！", player)
			-- 	end
			-- else
				return util.alert_msg(4, "皇城战已开启！", player)
			-- end
		end
		local isMergeServer = is_merge_server()
		if isMergeServer then
			if server_merge_day()+1<mergeFirstWarDay or server_merge_day()+1==mergeFirstWarDay and not isWarClose() then
				return util.alert_msg(4, "合服第三天自动开启皇城战", player)
			elseif server_merge_day()+1==mergeFirstWarDay and isWarClose() or server_merge_day()+1<=mergeSecondWarDay and not isWarClose() then
				return util.alert_msg(4, "合服第五天自动开启皇城战", player)
			end
		else
			if server_start_day()+1<warStartDay or server_start_day()+1==warStartDay and not isWarClose() then
				return util.alert_msg(4, "开服第四天自动开启皇城战", player)
			end
		end
		if not guild or guild:get_leader_name() ~= player:get_name() then
			return util.alert_msg(4, "您不是帮会会长，无法申请皇城战！", player)
		end
		if player:remove_item("攻城号角",1)==1 then
			server.set_var("kingCityOpenTime",os.time()+24*3600)
			server.info(10000,0,"<font color=#01ebff>"..player:get_guild().."</font>帮会的帮主<font color=#01ebff>"..player:get_name().."</font>成功申请了皇城战，皇城战将于明晚20:00开启！")
			util.alert_msg(4, "皇城战将于明晚20:00开启！", player)
		else
			return util.alert_msg(4, "开启皇城战需要1个攻城号角！", player)
		end
	elseif d.cmd == "gokinghome" then
		posmap.fly(player,4010004)--前往沙巴克
	elseif d.cmd == "deliver" then
		posmap.fly(player,4010004)--前往沙巴克
	elseif d.cmd == "enterKingCity" then --进入皇宫
		local guild = player:get_guild()
		if not guild or guild =="" then
			return util.alert_msg(4, "没有行会无法进入皇宫", player)
		end
		if server.is_war() > 0 then
			if server.get_var("huanggongkill") == "1" then
				local km = server.find_map("kinghome")
				if km then
					if util.svn("huanggongopen")==1 then
						local x,y = km:gen_rand_position()
						player:enter_map(km:get_id(),x,y)
					else
						util.alert_msg(4, "皇宫将于20:30开启！", player)
					end
				end
			else
				util.alert_msg(4, "雕像尚未摧毁！", player)
			end
		else
			util.alert_msg(4, "皇城战尚未开始！", player)
		end
	end
end

ckpanel.add_listener("npc.huangcheng.onPanelData",npc.huangcheng.onPanelData)

function sendLeaderAwardEmail()
	if is_merge_server() then
		return
	end
	if server_start_day()+1 > warStartDay then return end
	local t,award ={}
	local king_guild = server.get_king_guild()
	local guild_king = server.find_guild(king_guild)
	if guild_king then
		local leaderSeed = guild_king:get_leader_seed()
		award = leaderAwards.award
		t = {}
		for k,v in pairs(award) do
			table.insert(t,checknumber(v.id))
			table.insert(t,checknumber(v.num))
		end
		if #t>0 then
			server.post_mail_to_one(leaderSeed,"皇城战奖","恭喜您成为沙巴克城主，这是您的奖励。",unpack(t))
		end
	end
end
--开服第四天自动开启，合服第三，第五天自动开启
function checkAutoStart()
	if is_merge_server() then
		local mergeday = server_merge_day()+1
		if mergeday<mergeFirstWarDay or mergeday==mergeFirstWarDay and not isWarClose() then
			server.set_var("kingCityOpenTime",os.time()+(mergeFirstWarDay-mergeday)*24*3600)
		elseif mergeday==mergeFirstWarDay and isWarClose() or mergeday>mergeFirstWarDay and mergeday<=mergeSecondWarDay and not isWarClose() then
			server.set_var("kingCityOpenTime",os.time()+(mergeSecondWarDay-1-mergeday)*24*3600)
		end
	else
		local serverDay = server_start_day()+1
		if serverDay<warStartDay or serverDay==warStartDay and not isWarClose() then
			server.set_var("kingCityOpenTime",os.time()+(warStartDay-serverDay)*24*3600)
		end
	end
end
function printTime(time)
	print(os.date("%Y-%m-%d %H:%M:%S",tonumber(time) or os.time()));
end
function war_start_day()
	local st
	if is_merge_server() then
		st = server.get_var("server_merge_time");
	else
		st = server.get_var("server_start_time");
	end
	if st ~= "" then
		return get_day(tonumber(util.svn("kingCityOpenTime")))-get_day(tonumber(st));
	end
	return 0;
end

function isWarClose()
	return tonumber(os.date("%H", os.time()))>=21
end

function isWarStart()
	return tonumber(os.date("%H", os.time()))==20
end

function isWarOpenToday()
	if isWarClose() then return false end
	if is_merge_server() then
		if server_merge_day()+1==mergeFirstWarDay or server_merge_day()+1==mergeSecondWarDay then return true end
	else
		if server_start_day()+1==warStartDay then return true end
	end
	local openTime = util.svn("kingCityOpenTime")
	return tonumber(os.date("%Y%m%d", openTime)) == tonumber(os.date("%Y%m%d", os.time()))
end
function isWarOpenTomorrow()
	if is_merge_server() then
		return war_start_day() == server_merge_day() +1
	else
		return war_start_day() == server_start_day() +1
	end
end
--是否开启
function isWarOpen()
	if is_merge_server() then
		if server_merge_day()+1<mergeSecondWarDay or server_merge_day()+1==mergeSecondWarDay and not isWarClose() then return true end
	else
		if server_start_day()+1<warStartDay or server_start_day()+1==warStartDay and not isWarClose() then return true end
	end
	return isWarOpenToday() or isWarOpenTomorrow()
end