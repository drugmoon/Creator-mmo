module(..., package.seeall)

local date_shiwang_timer={
	[1]={con=const.PP_TIMER_INDEX100,timer=100},
	[2]={con=const.PP_TIMER_INDEX101,timer=101},
	[3]={con=const.PP_TIMER_INDEX102,timer=102},
	[4]={con=const.PP_TIMER_INDEX103,timer=103},
	[5]={con=const.PP_TIMER_INDEX104,timer=104},
	[6]={con=const.PP_TIMER_INDEX105,timer=105},
	[7]={con=const.PP_TIMER_INDEX106,timer=106},
	[8]={con=const.PP_TIMER_INDEX107,timer=107},
	[9]={con=const.PP_TIMER_INDEX108,timer=108},
	[10]={con=const.PP_TIMER_INDEX109,timer=109},
	[11]={con=const.PP_TIMER_INDEX110,timer=110},
	[12]={con=const.PP_TIMER_INDEX111,timer=111},
}


function onPlayerLeave(map,player)
	local num=player:num_item(26040001)
	if num>0 then
		if not player:drop_item(26040001) then
			server.info(10010,0,"玩家<font color='#01ebff'>"..player:get_name().."</font>已离开地图，<font color=#ff1fec>封印技能书</font>掉落在地")
			
		end
	end
	
	for i=1,#date_shiwang_timer do 
		--print(util.ppn(player,date_shiwang_timer[i].con))
		player:set_param(date_shiwang_timer[i].con,0)
		-- player:set_timer(date_shiwang_timer[i].timer,0);
	end 
	player:set_param(const.PP_SHIWANGTIME_FLAG,0)----出地图小时倒计时
	npc.fenghao.updateAllTitle(player)
end

function allPlayerMap(player)
	go.home(player);
end

