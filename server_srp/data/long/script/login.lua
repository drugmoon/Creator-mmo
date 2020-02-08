module(..., package.seeall)
local first_login_data={};
function add_first_login_listener(fun)
	first_login_data[fun] = fun;
end
function first_login(player)
	for i,v in pairs(first_login_data) do
		v(player);
	end
end
--------------------------------------------------------------------
local everyday_login_data={};
function add_everyday_login_listener(fun) everyday_login_data[fun] = fun; end;
function everyday_first_login(player) for i,v in pairs(everyday_login_data) do v(player); end; end;
--------------------------------------------------------------------
function load_player(player)
end
--------------------------------------------------------------------
local login_data={};
function add_login_listener(fun)
	login_data[fun] = fun;
end
function enter_game(player)
	for i,v in pairs(login_data) do
		v(player);
	end
	cklevel.triger_level(player,player:get_level());
end
--------------------------------------------------------------------
local logout_data={};
function add_logout_listener(fun)
	logout_data[fun] = fun;
end
function leave_game(player)
	for i,v in pairs(logout_data) do
		v(player);
	end
end
--------------------------------------------------------------------
function unload_player(player)
end

function process_first_login(player)
	-- player:set_free_direct_fly(200);
	player:set_param(const.PP_FIRST_LOGIN,os.time())
end
add_first_login_listener(process_first_login);

function push_task_info(player)
	player:set_task_state(1,1);
end
add_first_login_listener(push_task_info);

function process_status(player)
	if util.ppn(player,const.PP_ZHSH_LEVEL) > 0 then
		player:set_status(50,86400,util.ppn(player,const.PP_ZHSH_LEVEL),1,1);
		player:set_zslevel(util.ppn(player,const.PP_ZHSH_LEVEL));
	end
	if util.ppn(player,const.PP_VIP_LEVEL) > 0 then
		player:set_status(39,86400,util.ppn(player, const.PP_VIP_LEVEL),1,1)
   		player:set_model(5,util.ppn(player, const.PP_VIP_LEVEL))
	end
	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	if wingLv>0 and wingLv<=gui.PanelWing.getWingMaxLevel() then
		if player:is_status(68) then
			player:clear_status(68);
		end
		local star = util.ppn(player,const.PP_WING_STAR);
		local param = (wingLv-1)*10+star;
		player:set_status(68,86400,param,1,1);
		player:set_model(11,wingLv);
	end
	local guild_name = player:get_guild();
	if guild_name and guild_name ~= "" then
		local guild = server.find_guild(guild_name)
		if guild then
			local guild_level = guild:get_guild_level()
			player:clear_status(40);
			player:set_status(40,86400,guild_level,1,1);
		end
	elseif player:is_status(40) then
		player:clear_status(40);
	end
	
	-- if player:is_status(55) then
	-- 	player:clear_status(55)
	-- end
	-- local zjie = util.ppn(player,const.PP_NEW_ZUOQI_JIE_LEVEL);
	-- local zxing = util.ppn(player,const.PP_NEW_ZUOQI_XING_LEVEL);
	-- if zjie > 0 and zxing > 0 then
	-- 	player:set_status(55,86400,(zjie-1)*10+zxing,1,1);
	-- 	player:set_model(7,zjie);
	-- end
	if player:is_status(79) then
		player:clear_status(79);
	end
	if util.ppn(player,const.PP_GUILD_SKILL1)>0 or util.ppn(player,const.PP_GUILD_SKILL2)>0 or 
		util.ppn(player,const.PP_GUILD_SKILL3)>0 or util.ppn(player,const.PP_GUILD_SKILL4)>0 then
		player:set_status_plus(79,86400,1,1,1,util.ppn(player,const.PP_GUILD_SKILL1),util.ppn(player,const.PP_GUILD_SKILL2)
			,util.ppn(player,const.PP_GUILD_SKILL3),util.ppn(player,const.PP_GUILD_SKILL4),0,0);
	end
	-- local ysjie = util.ppn(player,const.PP_YUANSHEN_LEVEL_JIE)
	-- local ysxing = util.ppn(player,const.PP_YUANSHEN_LEVEL_XING)
	-- if ysjie > 0 and ysxing> 0 then
	-- 	if player:is_status(80) then player:clear_status(80); end
	-- 	player:set_status(80,86400,10*(ysjie-1)+ysxing,1,1)
 --   		-- player:set_model(21,10*(ysjie-1)+ysxing)
	-- end

	if player:is_status(81) then
		player:clear_status(81);
	end
	if util.ppn(player, const.PP_TITLE_INDEX) > 0 then
		player:set_status(81, 86400, util.ppn(player, const.PP_TITLE_INDEX), 1, 1);
	end

	-----元神突破
	-- local tpJie = util.ppn(player, const.PP_YUANSHEN_TOPO_LV_JIE);
	-- local tpXing= util.ppn(player, const.PP_YUANSHEN_TOPO_LV_XING);
	-- local m = (tpJie-1)*10 + tpXing > 0 and (tpJie-1)*10 + tpXing or 0;
	-- if m > 0 and m<=100 then
	-- 	if player:is_status(84) then player:clear_status(84); end
	-- 	player:set_status(84,86400,m,1,1)
	-- end
	npc.fenghao.updateAllTitle(player)
	----[[
	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
	if vipLv>0  then
		local vipconf = tab.Vip.getVipConf(vipLv)
		if vipconf and player:have_buff(vipconf.attackBuffId) ~= 1 then
			player:set_buff(vipconf.attackBuffId)
		end
	end
	--]]

	-- player:set_buff(26003)
	player:set_model(19,util.ppn(player, const.PP_SUM_RECHARGE));--充值总额

end
add_login_listener(process_status);
