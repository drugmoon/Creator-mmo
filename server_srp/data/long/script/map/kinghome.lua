module(..., package.seeall)

local ACTIVITY_NAME = "皇城战"
local NEED_LEVEL = 60

local GUILD_TITLE = {
	OUT=100,
	ENTERING=101,
	NORMAL=102,
	ADV=200,
	VICEADMIN=300,
	ADMIN=1000,
}
function onPlayerEnter(pmap,player)
	setPlayerWarState(player)
	-- checkKingGuild(pmap) --此时遍历map不算player在内
	player:push_lua_table("lockExtendBtns",util.encode({lock = "lock",visible=false}));

end

function setPlayerWarState(player)
	if server.is_war() > 0 then

		-- go.rand(player)
		map.wangcheng.changeModeToGuild(player)
		if util.ppn(player, const.PP_KING_CITY_WAR) == 0 then
			player:set_param(const.PP_KING_CITY_WAR, 1)

			showPrefix(player)
		end
	end
end

function onPlayerLeave(pmap,player)
	showPrefix(player)
	-- map.wangcheng.changeModeToPeace(player)
	player:push_lua_table("lockExtendBtns",util.encode({lock = "unlock",visible=true}));
end

function checkKingGuild(pmap)
	if pmap:get_id() == "kinghome" and pmap:get_var("check5min")=="open" and server.is_war()>0 then
		local kingGuildName = pmap:find_hold_guild()--地图上的唯一帮会
		local KHcandidate = server.get_var("KHcandidate")
		if kingGuildName and kingGuildName~="" then
			server.set_king_guild(kingGuildName)--修改玩家称号和皇城帮会名称、颜色
			recordKingGuildInfo()
			if KHcandidate~= kingGuildName then
				server.info(10010,0,"皇宫被[<font color='#ff0000'>"..kingGuildName.."</font>]帮会占领！")
				server.set_var("KHcandidate", kingGuildName)
				server.server_exe("map.kinghome.showPrefix")
			end
		end
	end
end
--20:35
function onTimer1(pmap)
	pmap:set_var("check5min","open")--5分钟后可产生占领者
	checkKingGuild(pmap)
end
--掉线
function onTimer3(pmap)
	checkKingGuild(pmap)
end

function recordPlayer(var,...)
	local t = {...}
	-- print("----",var,util.encode(t))
	local paramsNum = select("#",...)
	if paramsNum>=2 then
		local playerNum = select(1,...)
		if playerNum and checknumber(playerNum)>0 then
			local kingguildViceInfo = {}
			for i=1,playerNum do
				local viceinfo = {}
				viceinfo.name = select(5*(i-1)+1+1,...);
				viceinfo.cloth = select(5*(i-1)+2+1,...);
				viceinfo.weapon = select(5*(i-1)+3+1,...);
				viceinfo.wing = select(5*(i-1)+4+1,...);
				viceinfo.gender = select(5*(i-1)+5+1,...);
				table.insert(kingguildViceInfo,viceinfo)
			end
			server.set_var(var,util.encode(kingguildViceInfo))
		end
	end
end

function recordKingGuildInfo()
	local candidate = server.get_king_guild()
	if candidate and candidate ~= ""  then
		local kingGuild = server.find_guild(candidate)
		local leaderOfKing = ""
		if kingGuild then
			leaderOfKing = kingGuild:get_leader_name()
		end

		recordPlayer("kingGuildViceAdmin",kingGuild:get_vice_admin_info())
		recordPlayer("kingGuildVice",kingGuild:get_adv_info())
		-- vice("kingGuildAdmin",kingGuild:get_admin_info())

		local king = server.find_player(leaderOfKing)
		local king_tab = {}
		king_tab.name = leaderOfKing
		king_tab.guild = candidate
		if king then
			king_tab.name = king:get_name()
			king_tab.guild = candidate
			king_tab.job = king:get_job_name()
			king_tab.gender = king:get_gender_name()
			king_tab.fight = king:get_fightpoint()
			king_tab.level = king:get_level()
			king_tab.weapon = king:get_item_type_by_pos(const.ITEM_WEAPON_POSITION)
			king_tab.cloth = king:get_item_type_by_pos(const.ITEM_CLOTH_POSITION)
			king_tab.wing = util.ppn(king,const.PP_WING_LEVEL)
			server.set_var("kingInfo",util.encode(king_tab))
		else
			recordPlayer("kingInfo",kingGuild:get_admin_info())
		end
	else
		server.set_var("kingInfo","")
		server.set_var("kingGuildVice","")
		server.set_var("kingGuildViceAdmin","")
	end
end

function addkingguildinfo(player)
	local guildName = player:get_guild()
	local candidate = server.get_king_guild()
	if guildName and candidate ~= "" and guildName == candidate  then
		local kingGuild = server.find_guild(candidate)
		local title = player:get_guild_title()
		if kingGuild then
			if title == GUILD_TITLE.ADMIN then
				recordPlayer("kingInfo",kingGuild:get_admin_info())
			elseif title == GUILD_TITLE.VICEADMIN then
				recordPlayer("kingGuildViceAdmin",kingGuild:get_vice_admin_info())
			elseif title == GUILD_TITLE.ADV then
				recordPlayer("kingGuildVice",kingGuild:get_adv_info())
			end
		end
	end
end
-- login.add_login_listener(addkingguildinfo)
-- login.add_logout_listener(addkingguildinfo)

--检查占领者21:00
function checkResult(pmap)

	if not pmap then return end

	local candidate = server.get_king_guild()
	if candidate and candidate ~= ""  then
		npc.huangcheng.sendLeaderAwardEmail();
		local kingGuild = server.find_guild(candidate)
		local leaderOfKing = ""
		if kingGuild then
			leaderOfKing = kingGuild:get_leader_name()
			kingGuild:fresh_member_info()
		end
		server.info(10000,0,"皇城被[<font color='#ff0000'>"..candidate.."</font>]帮会夺下！[<font color='#ff0000'>"..leaderOfKing.."</font>]成为皇城城主！")
		server.info(10000, 1, "恭喜玩家<font color='#01ebff'>"..leaderOfKing.."</font>获得了<font color='#ff0000'>传奇主宰者</font>的称号")
	else
		if util.svn("huanggongkill")==0 then
			server.info(10000,0,"本次皇城战雕像未被打碎,活动提前结束！")
		else
			server.set_var("huanggongkill",0)
			server.info(10000,0,"皇城战结束了，但并没有帮会将皇城占领")
		end
	end

	server.server_exe("map.kinghome.showPrefix")
end

--修改名称等字体颜色
function showPrefix(player)
	player:push_lua_table("KHcandidate", util.encode({KHcandidate = server.get_king_guild()}));
end

function SetGuildOwnerData(player)--引擎掉--已注释
	local king_guild = server.get_king_guild()
	local myguild = player:get_guild();
	if king_guild ~= "" and king_guild == myguild then-----若有王城城主,并且我就是那个帮会的
		local king_tab = {}
		king_tab.name = player:get_name()
		king_tab.guild = king_guild
		king_tab.job = player:get_job_name()
		king_tab.gender = player:get_gender_name()
		king_tab.fight = player:get_fightpoint()
		king_tab.level = player:get_level()
		king_tab.weapon = player:get_item_type_by_pos(const.ITEM_WEAPON_POSITION)
		king_tab.cloth = player:get_item_type_by_pos(const.ITEM_CLOTH_POSITION)
		king_tab.wing = util.ppn(player,const.PP_WING_LEVEL)
		server.set_var("kingInfo",util.encode(king_tab))
		server.server_exe("map.kinghome.showPrefix")
	end
end

function noticeWarStart(player)
	if player:get_level() < NEED_LEVEL then return end
	if player:get_map():get_id()=="kingcity" then return end
	local result = {
		str="tip_king",
		activity = "皇城战",msg = "皇城战活动已开启，是否前往？",
		callFunc ="npc.huangcheng.onPanelData",
		book = util.encode({cmd = "gokinghome"})
	}
	player:push_lua_table("showBottomMsg",util.encode(result))
end

function hideWarNotice(player)
	if player:get_level() < NEED_LEVEL then return end
	local result = {
		str="tip_king",
		hide = true,
	}
	player:push_lua_table("showBottomMsg",util.encode(result))
end

function noticeWarPreStart(player)
	if player:get_level() < NEED_LEVEL then return end
	player:alert(10000,0,"皇城战将在10分钟后开启!")
end