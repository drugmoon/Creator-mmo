module(..., package.seeall)

function onPlayerEnter(map,player)
	-- player:set_param(const.PP_CITYWAR_JOIN_STATE,1)
	-- if server.get_temp_var("wlzb") ~= "1" then
	-- 	go.home(player)
	-- 	util.alert_msg(4, "武林争霸入口时间已关闭，请下次再来！", player)
	-- end
end

function onPlayerLeave(map,player)
	local map = server.find_map("diyi");
	if server.get_temp_var("txdystate") == "start" then
		if map then
			if map:num_player() == 1 then
				local txdy = map:get_rand_player(1)
				if txdy  then
					npc.txdy.Awardtxdy(txdy)
				end
			---else
				--print("godie....")
			end
		end
	end
end