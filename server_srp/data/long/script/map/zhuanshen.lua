module(..., package.seeall)


function onPlayerLeave(map,player)
	player:set_param(const.PP_ZSSD_DEADTIMES,0)
	player:clear_status(23)
	player:clear_status(10)
	player:clear_status(11)
	player:clear_status(12)
end


