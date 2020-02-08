module(..., package.seeall)

local jobNum = {
	["warrior"] = 102,
	["wizard"] = 101,
	["taoist"] = 100,
}

function onPanelData(player, commondData)
	local data =util.decode(commondData);
	if data.actionid == "recommendPlayer" then
		if data.param and data.param[1] then
			updateRecommendPlayerList(player);
		end
	elseif data.actionid == "searchFriends" then
		if data.param and data.param[1] then
			onQueryPlayer(player, data.param[1]);
		end
	elseif data.actionid == "callFriend" then
		if data.param and data.param[1] then
			onCallPlayer(player, data.param[1]);
		end
	elseif data.actionid == "gotoFriend" then
		if data.param then
			onGotoPlayer(player, data.param);
		end	
	elseif data.actionid == "getRevenge" then
		local str = player:get_param(const.PP_REVENGE_STR)
		if str=="" then
			str ="此仇不报非君子！"
			player:set_param(const.PP_REVENGE_STR,str)
		end
		player:push_lua_table("PanelFriend",util.encode({cmd = "getRevenge",str = str}))
	elseif data.actionid == "revengeChange" then
		player:set_param(const.PP_REVENGE_STR,data.str)
	elseif data.actionid == "enemyTrack" then
		enemyTrack(player,data.enemyName)
	elseif data.actionid == "gotoEnemy" then
		gotoEnemy(player,data.enemyName)
	end
end

function enemyTrack( player ,enemyName)
	local enemy = server.find_player(enemyName)
	if enemy then
		if 1== player:sub_vcoin(200,"仇人追踪") then
			local result = {}
			local enemymap = enemy:get_map()
			result.map = enemymap:get_name()
			result.x,result.y = enemy:get_pos()
			player:push_lua_table("enemyTrack", util.encode(result))
			player:set_temp_param(6888,enemymap:get_id()..";"..result.x..";"..result.y)
		else
			gui.PanelBag.showChongzhi(player)
		end
	else
		util.alert_msg(4, "仇人不在线", player)
	end
end
--排除副本地图
function gotoEnemy( player ,enemyName)
	local maprecord = player:get_temp_param(6888)
	if string.find(maprecord,";") then
		local params = string.split(maprecord,";")
		if #params==3 then
			local mapId = params[1]
			if tab.MapInfo[mapId].mapType == "3" then
				return util.alert_msg(4, "仇人正在副本当中，无法前往", player)
			end
			local x,y = checknumber(params[2]),checknumber(params[3])
			player:enter_map(mapId,x,y);
		end
		player:set_temp_param(6888,"")
	else
		util.alert_msg(4, "请先追踪仇人", player)
	end
end

--服务器端获得推荐好友的名字，然后根据名字去取玩家信息再推到客户端
function updateRecommendPlayerList (player, playerName, level)
	local num = 13
	--playerName:玩家自己的名字；num:要获取的数量；level:玩家等级
	level = level or player:get_level()
	playerName = playerName or player:get_name()
	local tempNames = server.server_exe_rand(playerName,num,level)
	local recommendPlayerNamesTable = util.splitString( tempNames,"_" )

	local result = {
		cmd = "recommendPlayer",
		allPlayerNum = #recommendPlayerNamesTable,
		data = {},
	}
	if #recommendPlayerNamesTable == num then
		table.remove(recommendPlayerNamesTable)
	end

	for i,v in ipairs(recommendPlayerNamesTable) do
		local recommendPlayer =  server.find_player(v)
		if recommendPlayer then
			result.data[i] = {
				name = v,
				online_state = 1,
				gender = recommendPlayer:get_gender(),
				job = jobNum[recommendPlayer:get_job_name()],
				level = recommendPlayer:get_level(),
				power = recommendPlayer:get_fightpoint(),
			}
		end
	end
	player:push_lua_table("PanelFriend", util.encode(result))

end

function onQueryPlayer (player, name)
	local queryPlayer =  server.find_player(name)
	local result = {
		cmd = "searchFriends",
		data = {},
		isFind = true
	}
	if not queryPlayer then 
		result.isFind = false;
	else
		result.data = {
			name = name,
			online_state = 1,
			gender = queryPlayer:get_gender(),
			job = jobNum[queryPlayer:get_job_name()],
			level = queryPlayer:get_level(),
			power = queryPlayer:get_fightpoint(),
			guild = queryPlayer:get_guild() or "暂无"
		}
	end
	player:push_lua_table("PanelFriend", util.encode(result))
end

function checkCallFriend(player)
	local mapId = player:get_map():get_id()
	return tab.isMapCallLegal(mapId)
end

function onCallPlayer (player, data)
	local queryPlayer =  server.find_player(data)

	if not queryPlayer then 
		util.alert_msg(4, "好友不在线，无法召唤！", player)
	else
		if not checkCallFriend(player) then
			return util.alert_msg(4, "当前地图，无法召唤！", player)
		elseif not tab.getMapEnterLevel(queryPlayer,player:get_map():get_id()) then
			return util.alert_msg(4, "好友无法进入当前地图，无法召唤！", player)
		end
		if player:sub_vcoin_bind(1000)==1 then
			queryPlayer:push_lua_table("callFriend", util.encode({
				call = true, friendName = player:get_name()
			}))
			player:push_lua_table("callFriendIntime", util.encode({}))--客户端计时
		else
			util.alert_msg(4, "绑定元宝不足，无法召唤！", player)
		end
	end
end

function onGotoPlayer( player,param )
	local playerName = param[1]
	local operate = param[2]
	local whocallme = server.find_player(playerName)
	if whocallme then
		if operate == "agree" then
			if checkCallFriend(whocallme) then
				local mapId = whocallme:get_map():get_id()
				local x,y = whocallme:get_pos()
				player:enter_map(mapId,x,y);
			else
				player:alert(1,1,"好友当前地图无法传送！")
			end
		elseif operate == "refuse" then
			whocallme:alert(1,1,"好友["..player:get_name().."]拒绝召唤！")
		elseif operate == "noOperate" then
			whocallme:alert(1,1,"好友["..player:get_name().."]未响应！")
		end
	end
end

ckpanel.add_listener("gui.PanelFriend.onPanelData",gui.PanelFriend.onPanelData)