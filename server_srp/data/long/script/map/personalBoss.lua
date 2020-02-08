module(..., package.seeall)
function onPlayerEnter(map,player)
	map:set_timer(1,5*1000)
	player:alert(1,1,"您已进入副本地图(时长30分钟),尽情享受战斗的乐趣吧！")
end

function onPlayerLeave(map,player)
	-- map:set_timer(1,0)
	-- map:clear_mon()
	-- map:clear_item()
	-- map:clear_mininpc()
end

function onTimer1(map)
	map:map_exe("map.personalBoss.recordTime")
	map:set_timer(1,5*1000)
end

function recordTime(player)
	local passTime = util.ppn(player,const.PP_GEREN_FUBEN_COUNT)
	player:set_param(const.PP_GEREN_FUBEN_COUNT,passTime+5)
	if (passTime+5)>=30*60 then
		go.home(player)
	end
end


