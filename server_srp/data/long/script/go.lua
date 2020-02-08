module(..., package.seeall)
function home(player)
	player:set_temp_param(const.PTP_ID_TIMER_GO,'h');
	player:set_timer(const.PT_GO,1);
end
function rand(player)
	player:set_temp_param(const.PTP_ID_TIMER_GO,'r');
	player:set_timer(const.PT_GO,1);
end
function stop(player)
	player:set_timer(const.PT_GO,0);
end
function timer(player)
	player:set_timer(const.PT_GO,0);
	local t = player:get_temp_param(const.PTP_ID_TIMER_GO);
	if t == 'h' then player:go_home();end
	if t == 'r' then local x,y;x,y=player:gen_rand_position();if( x and y )then player:move(x,y); end; end
end