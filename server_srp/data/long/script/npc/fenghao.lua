module(..., package.seeall)

local highplayTitle = {
	["warrior"] = {index=2, title = "第一战士"},
	["wizard"]  = {index=3, title = "第一法师"},
	["taoist"]  = {index=4, title = "第一道士"},
}

function updateAllTitle(player)
	local pre = "";
	local gwLev = util.ppn(player,const.PP_GUANWEI_LEVEL)
	local shiwangTime =util.ppn(player,const.PP_SHIWANGTIME_FLAG)
	if gwLev > 0 then
		pre = pre.."|guanwei_"..gwLev;
	end
	if shiwangTime > 0 then
		pre = pre.."|shiwangTime_"..shiwangTime;
	end
	
	local job = player:get_job_name()
	local serverVar = "fight"..job
	if player:get_seed_name() == server.get_var(serverVar.."c") and server.get_var(serverVar.."c") ~= nil and server.get_var(serverVar.."c")~= "" then
		pre = pre.."|highplay_"..highplayTitle[job].index;
	end

	player:set_name_pre(pre);
end

function clearfenghao()
	-- for k,v in pairs(highplayTitle) do
	-- 	server.set_var("fight"..k.."male".."a", "")
	-- 	server.set_var("fight"..k.."male".."b", "")
	-- 	server.set_var("fight"..k.."male".."d", "")
	-- 	server.set_var("fight"..k.."male".."f", "")

	-- 	server.set_var("fight"..k.."female".."a", "")
	-- 	server.set_var("fight"..k.."female".."b", "")
	-- 	server.set_var("fight"..k.."female".."d", "")
	-- 	server.set_var("fight"..k.."female".."f", "")
	-- end
end

function fightRanking(player, fight)
	if fight then
		local a = player:get_name()
		local b = fight
		local c = player:get_seed_name()
		local d = player:get_level()
		local f = player:get_guild()
		local job = player:get_job_name()
		local serverVar = "fight"..job
		if server.get_var(serverVar.."c") == c then
			server.set_var(serverVar.."a", a)
			server.set_var(serverVar.."b", b)
			server.set_var(serverVar.."d", d)
			server.set_var(serverVar.."f", f)
		elseif util.svn(serverVar.."b") < b then
			local old_name = server.get_var(serverVar.."a")
			local old
			if server.get_var(serverVar.."c") ~= "" then
				old = server.find_player_by_seed(server.get_var(serverVar.."c"))
			end
			server.set_var(serverVar.."a", a)
			server.set_var(serverVar.."b", b)
			server.set_var(serverVar.."c", c)
			server.set_var(serverVar.."d", d)
			server.set_var(serverVar.."f", f)
			--设置第一战法道buff
			updateAllTitle(player)
			if old then
				--old:clear()--清除第一战法道buff
				updateAllTitle(old)
			end
			if old_name and old_name ~= "" then
				server.info(10000, 1, "恭喜玩家<font color='#01ebff'>"..a.."</font>顶替<font color='#01ebff'>"..old_name.."</font>获得了<font color='#ff0000'>"..highplayTitle[job].title.."</font>的称号")
			else
				server.info(10000, 1, "恭喜玩家<font color='#01ebff'>"..a.."</font>获得了<font color='#ff0000'>"..highplayTitle[job].title.."</font>的称号")
			end
		end
	end
end
