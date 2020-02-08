module(..., package.seeall)

function i(player)
	npc.biggm.onTalk100(nil,player)
end

function mount2(player)
	-- body 2008
	player:set_mount_info(1,2009,130171);
	player:set_mount_show(0);
	player:set_mount_show(1);
	if player:get_level() <= 40 then
	-- player:set_mount_show_lock(1);
	player:alert(1,1,"<font color='#ff0000'>坐骑体验中</font>");
	end
	player:set_timer(const.PT_UMOUNT,1000*180);
end