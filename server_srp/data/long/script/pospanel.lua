module(..., package.seeall)
local data={
	-- ["强化"]=				{ name="强化",	panel_name="menu_forge", guideLv = 13},
	-- ["日常"]=				{ name="日常",	panel_name="extend_richang", guideLv = 17},
	-- ["邪恶领地[副本]"]=		{ mapname="邪恶领地[副本]",	map="v101",	map_pre = "fbzx",	fuben_index = 1,		min = 201,	max = 299,	enter_x=27,	enter_y=46,	fly=5000,
	-- 							mg = {
	-- 								{mgname = "万恶龙神",	mgx = 27,		mgy = 46,		mgnum = 1,	mgrange = 5},
	-- 							},
	-- 						},
	-- ["白猪巢穴[副本]"]=		{ mapname="白猪巢穴[副本]",	map="v103",	map_pre = "fbzx",	fuben_index = 2,		min = 301,	max = 399,	enter_x=35,	enter_y=44,	fly=5001,
	-- 							mg = {
	-- 								{mgname = "白猪妖",		mgx = 35,		mgy = 44,		mgnum = 1,	mgrange = 5},
	-- 							},
	-- 						},
	-- ["回忆王城[副本]"]=		{ mapname="回忆王城[副本]",	map="v208",	map_pre = "fbzx",	fuben_index = 3,		min = 401,	max = 499,	enter_x=15,	enter_y=34,	fly=5002,
	-- 							mg = {
	-- 								{mgname = "天下第一战士",		mgx = 15,		mgy = 34,		mgnum = 1,	mgrange = 5},
	-- 								{mgname = "天下第一法师",		mgx = 17,		mgy = 36,		mgnum = 1,	mgrange = 5},
	-- 								{mgname = "天下第一道士",		mgx = 13,		mgy = 32,		mgnum = 1,	mgrange = 5}, 
	-- 							},
	-- 						},
	-- ["未知暗殿"]=			{ mapname="未知暗殿",	map="v209",	map_pre = "fbzx",	fuben_index = 4,		min = 501,	max = 550,	enter_x=26,	enter_y=40,	fly=5003,
	-- 							mg = {
	-- 								{mgname = "极品猛犸卫士",		mgx = 21,		mgy = 37,		mgnum = 1,	mgrange = 5},
	-- 							},
	-- 						},
	-- ["回收"]=				{ name="回收",		panel_name="menu_recycle", guideLv = 14},
	-- -- ["再做日常"]=			{ name="再做日常",	panel_name="extend_richang", guideLv = 15},
	-- -- ["初试回收"]=			{ name="初试回收",	panel_name="menu_recycle", guideLv = 14},
	-- ["获取装备"]=				{ name="获取装备",	panel_name="menu_getequip", guideLv = 0},
	-- ["勋章"]=				{ name="升级勋章",	panel_name="menu_medal", guideLv = 15},
	-- ["个人Boss"]=			{ name="个人Boss", 	guideLv = 20},
};
	

-- local data = np;
-- for i,v in pairs(data) do
-- 	if v.fly and v.fly >=5000 and v.fly<6000 then
-- 		server.set_fly_node(v.fly,v.map,v.enter_x,v.enter_y);
-- 	end
-- end
function sp(panel)
	local d = data[panel];
	if d.name == "个人Boss" then 
		return "guide_"..d.guideLv	
	else
		return "open_"..d["panel_name"].."_"..d["guideLv"] 
	end
end
function fp(map_name)
	local d = data[map_name];
	if d then return "fly_"..d["fly"] end
	return  ""..map_name.."";
end
function go(player,panel)
	local d = data[panel];
	-- if d then player:push_lua_table("open",util.encode({name = d["panel_name"], extend = {guideLv = d["guideLv"]}})) end
	if d then 
		if d.name == "回收" then
			player:push_lua_table("open",util.encode({name = d["panel_name"], extend = {guideLv = d["guideLv"]}}))
		else
			player:push_lua_table("guide",util.encode({lv = d["guideLv"]})); 
		end
		
	end


end

function enter(player,mapname)
	local d = data[mapname];
	if d then
		-- local cur_pre = string.sub(player:get_map():get_id(),1,4)
		-- if cur_pre == d["map_pre"] then return end
		local map=nil;
		-- for i=d["min"],d["max"] do
		map = server.find_map(d["map_pre"]);
		-- if map and map:num_player() < 1 then
		-- 	break;
		-- else
		-- 	map=nil;
		-- end
		-- end
		if map then
			map:set_timer(1,30*60*1000);
			map:clear_mon();map:clear_item();map:clear_mininpc();
			local mg = d["mg"];
			local job=player:get_job_name();
			
			for g = 1,#mg do
				local monname = mg[g].mgname
				if job=="warrior" then
					monname = monname.."1";
				end
				if job=="wizard" then
					monname = monname.."2";
				end
				if job=="taoist" then
					monname = monname.."3";
				end
				map:mon_gen(mg[g].mgx,mg[g].mgy,monname,mg[g].mgnum,mg[g].mgrange,1,1,"show_name");
			end

			player:enter_map(map:get_id(),d["enter_x"],d["enter_y"]);player:find_road_goto(map:get_id(),d["enter_x"],d["enter_y"],"autofightstart");
			player:alert(11,0,"您只有30分钟时间通过本副本!")

			player:set_task_state(const.TASK_ID_FUBEN_MAIN,task.util.ts(d["fuben_index"],const.TSACED));
			player:push_task_data(const.TASK_ID_FUBEN_MAIN,0);
			player:set_task_param(const.TASK_ID_FUBEN_MAIN,1,player:get_map():num_monster());
		else
			player:alert(1, 1, "副本地图爆满,请稍后再试!")
		end
	end
end