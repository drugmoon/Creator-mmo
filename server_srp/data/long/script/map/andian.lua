module(..., package.seeall)

--新传奇的暗殿绑定脚本
function onPlayerEnter(map,player)
	mon.bossRefresh.updateBossTime(map)
	mon.bossRefresh.updateData(player)
end

function onPlayerLeave(map,player)
	-- player:set_timer(8,0)--15秒后首次boss信息面板
	player:push_lua_table("hideWeiZhiAnDian",util.encode(result))
end

function updateTimeShow(player)
	local result={
		count=10800-util.ppn(player,const.PP_ANDIAN_ALL_TIME),
		title="未知暗殿",
	}
	player:push_lua_table("anDianCount",util.encode(result))
end

