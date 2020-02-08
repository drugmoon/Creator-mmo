module(..., package.seeall)

function onHuweiDead(slave)
	local player = slave:get_owner()
	if player ~= nil then
		player:set_param(const.PP_HUWEI_MAP_KILL_FLAG,1)
		gui.moduleRedPoint.checkMarsUsable(player)
	end
end

function onSlaveDead(slave)
	map.defend.onSlaveDead(slave);
end