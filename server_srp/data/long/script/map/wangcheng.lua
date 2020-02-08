module(..., package.seeall)
function onPlayerEnter(pmap,player)
	checkStatue(pmap)
	recordAttackMode(player)
	changeModeToGuild(player)
	player:push_lua_table("lockExtendBtns",util.encode({lock = "lock",visible=false}));
end

function checkStatue(pmap)
	if pmap:get_id() == "kingcity" then
		if server.is_war() == 0 then
			pmap:clear_mon_by_id(11006001);--有血
			if pmap:num_monster(11006002) == 0 then
				pmap:mon_gen(31,36,11006002,1,1,1,1,"dirfixed(4),landification",0,1);
			end
		elseif server.is_war() == 1 then
			pmap:clear_mon_by_id(11006002);--无敌
			if util.svn("huanggongkill") == 0 then
				if pmap:num_monster(11006001) == 0 then
					pmap:mon_gen(31,36,11006001,1,1,1,1,"dirfixed(4)",0,1);
				end
			end
		end
	end
end

function onPlayerLeave(pmap,player)
	-- changeModeToPeace(player)
	player:push_lua_table("lockExtendBtns",util.encode({lock = "unlock",visible=true}));

end

--20:30
function onTimer1(pmap)
	if util.svn("huanggongkill") == 0 then
		autorun.checkwarstop()--30分钟雕像未被打碎，直接宣布结束
	else
		server.set_var("huanggongopen",1)--标记皇宫进入标志
		server.info(10000,0,"皇宫入口已开放！")
	end
end

function changeModeToGuild(player)
	local attackMode = player:get_attack_mode();
	if attackMode~=103 then
		player:set_attack_mode(103) -- 行会模式
	end
	player:set_lock_attack_mode(1)
end

function changeModeToPeace(player)
	player:set_lock_attack_mode(0)
	player:set_attack_mode(101) -- 和平模式
end

function recordAttackMode(player)
	local attackMode = player:get_attack_mode()
	if attackMode == 0 then attackMode = 101; end
	player:set_param(const.PP_ATTACK_MODE,attackMode)
end

function revertAttackMode(player)
	local attackMode = util.ppn(player, const.PP_ATTACK_MODE);
	if attackMode == 0 then attackMode = 101; end
	player:set_lock_attack_mode(0)
	player:set_attack_mode(attackMode)
end