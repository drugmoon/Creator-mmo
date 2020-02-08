module(..., package.seeall)

local NEEDVCOIN = 200;

function onPanelData(player,commond)
	local data = util.decode(commond);
	if data.actionid =="reliveback" then
		relive_back(player);
	elseif data.actionid =="relivelocal" then
		relive_local(player);
	elseif data.actionid =="relivelocal_nouse" then
		relive_local_nouse(player);
	end
end
ckpanel.add_listener("gui.PanelRelive.onPanelData",gui.PanelRelive.onPanelData)

function openPanel( player, name )------------引擎>后端脚本>复活面板
	local mapId=player:get_map():get_id()

	if not npc.txdy.checkMapTime(player) then -- 天下第一活动时候不全服公告
		local pmap = player:get_map();
		if pmap  and not tab.checkMonName(name) then
			if string.sub(pmap:get_id(),1,4)=="swmy" then 
				server.info(100,0,"玩家<font color='#01ebff'>"..name.."</font>在<font color='#1eb8ff'>".."神威魔狱".."</font>击败了玩家<font color='#01ebff'>"..player:get_name().."</font>");
			else
				server.info(100,0,"玩家<font color='#01ebff'>"..name.."</font>在<font color='#1eb8ff'>"..pmap:get_name().."</font>击败了玩家<font color='#01ebff'>"..player:get_name().."</font>");
			end 
			
		end
	end

	if mapId and (mapId == "diyi" or string.find(mapId,"ctg")) then
		return
	-- elseif server.is_war()==1 and( mapId=="kingcity" or mapId =="kinghome") then
	-- 	return
	end
	-- ---暂时屏蔽复仇buff
	-- if string.sub(map,1,4)=="zssd" then 	
	-- 	--openPanel_zssd( player, name)
	-- else


		local x,y=player:get_pos()
		-- if util.ppn(player,const.PP_FREERELIVE_DATE) ~= today() then
		-- 	player:set_param(const.PP_FREERELIVE_DATE,today());
		-- 	-- player:set_param(const.PP_FREERELIVE_TIMES,gui.PanelVip.getVipValue(player,"freerelive"));
		-- end
		local result = {};
		local enemy = server.find_player(name);
		if enemy then
			local dieRecord = {
				name = name,
				x= x,
				y = y,
				map = player:get_map():get_name(),
				--time = os.time(),
				power = enemy:get_fightpoint(),
				level = enemy:get_level(),
				guild = enemy:get_guild(),
				job   = util.getJobName(enemy),
			}
			result = {
				data = dieRecord
			}
			player:push_lua_table("dieRecord",util.encode(result));
		end
		result = {}
		result.name = "panel_relive";
		local time = 30;
		local freeTimes = util.ppn(player,const.PP_FREERELIVE_TIMES)
		result.extend = {
			enemy = enemy and name or string.gsub(name,"%d$",""),
			freeTimes = freeTimes,
			time = time,
			reliveflag=1,
			needVcoin = NEEDVCOIN,
			x = x,
			y = y,
			map = player:get_map():get_name(),
			btnName1 = "回城复活",
			btnName2 = "原地复活",
		};
		if mapId=="yxbiqi" or mapId =="cifu" then
			time = 5;
			result.extend.btnName1 =  "随机复活"
			result.extend.btnName2 = nil;
			result.extend.time = time;
		elseif server.is_war()==1 and (mapId=="kingcity" or mapId =="kinghome")  then
			time = 5;
			result.extend.btnName1 = "回城复活"
			result.extend.btnName2 = nil;
			result.extend.time = time;
		end
		result.extend.time = time;
		player:set_timer(20,time*1000)

		player:push_lua_table("open",util.encode(result));
	--end
end


function openPanel_zssd( player, name )------------引擎>后端脚本>复活面板
	local data={15,21,27,33,39,45}
	local remaintime=30
	local Reliveflag=1
	local flag=util.ppn(player,const.PP_ZSSD_DEADTIMES)+1
	if data[flag] then 
		remaintime=data[flag]
		Reliveflag=100
	end
	
	local x,y=player:get_pos()
	-- if util.ppn(player,const.PP_FREERELIVE_DATE) ~= today() then
	-- 	player:set_param(const.PP_FREERELIVE_DATE,today());
	-- 	-- player:set_param(const.PP_FREERELIVE_TIMES,gui.PanelVip.getVipValue(player,"freerelive"));
	-- end
	local result = {};
	local enemy = server.find_player(name);
	if enemy then
		local dieRecord = {
			name = name,
			x= x,
			y = y,
			map = player:get_map():get_name(),
			--time = os.time(),
			power = enemy:get_fightpoint(),
			level = enemy:get_level(),
			guild = enemy:get_guild(),
			job   = util.getJobName(enemy),
		}
		result = {
			data = dieRecord
		}
		player:push_lua_table("dieRecord",util.encode(result));
	end
	result = {}
	result.name = "panel_relive";

	result.extend = {
		enemy = string.gsub(name,"%d+$",""),
		freeTimes = util.ppn(player,const.PP_FREERELIVE_TIMES),
		time = remaintime,
		reliveflag=Reliveflag,
		needVcoin = NEEDVCOIN,
		x = x,
		y = y,
		map = player:get_map():get_name(),
	};
	player:push_lua_table("open",util.encode(result));
end

function relive_back( player )
	if player:is_dead() then
		local curMap = player:get_map()
		if curMap then
			local mapId = curMap:get_id()
			if mapId == "yxbiqi" then
				local x,y = curMap:gen_rand_position()
				player:relive(100);
				player:move(x,y);
				player:setHpMpPercent(100)
			elseif mapId=="cifu" then
				local x,y = map.cifu.getRelivePos(player)
				player:relive(100);
				player:move(x,y);
			else
				player:relive(100);
			end
		end
	end
	player:set_timer(20,0)
end

function relive_local(player)
	if player:is_dead() then
		player:relive(101);
	end
	player:set_timer(20,0)
end

function relive_local_nouse(player )
	if player:is_dead() then
		local map = player:get_map():get_name()
		local x,y=player:get_pos()
		player:relive(120,map,x,y)
		player:set_timer(20,0)
	end
end

function rem_freerelive_times( player )
	local times = util.ppn(player,const.PP_FREERELIVE_TIMES);
	if times > 0 then
		player:set_param(const.PP_FREERELIVE_TIMES,times - 1);
		return 1;
	else
		return 0;
	end
end