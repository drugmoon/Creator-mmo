module(..., package.seeall)
local task_id=const.TASK_ID_RICHANG;

local task_data = tab.TaskData2000;
task_data.ttype="经验";
task_data.focus=false;
task_data.autogo=false;

local task_state_data={};

local MIN_TID = 0;
local MAX_TID = 0;

local function parseTeleportID(teleportID)
	if util.isString(teleportID) then
		local tempA = string.split(teleportID, ";");
		local tempB = {};
		-- print("before tonumber", util.encode(tempA))

		for i,v in ipairs(tempA) do
			-- v = tonumber(v)
			table.insert(tempB, tonumber(v))
		end
		teleportID = tempB
		-- print("after tonumber", util.encode(tempB))
		return teleportID;
	end
	return teleportID;
end

for i,v in pairs(task_data) do
	if util.isTable(task_data[i]) then
		if MIN_TID == 0 then
			MIN_TID = i;
		else
			MIN_TID = MIN_TID > i and i or MIN_TID;
		end
		if MAX_TID == 0 then
			MAX_TID = i;
		else
			MAX_TID = MAX_TID < i and i or MAX_TID;
		end
		task_data[i].teleportID = parseTeleportID(task_data[i].teleportID);
		-- print("task2000", i, type(task_data[i].teleportID))
		task_data[i].kill_talk=function (player,td) player:alert(1,0,"降妖除魔："..player:get_task_param(task_id,1).."/"..td.checkCount) end
		task_data[i].complate_hook=function(n,player,td) 
			player:alert(1,1,"您已完成本次降妖除魔任务!");
			-- player:go_home(); 
			posnpc.fly(player,td.doner);
			-- player:push_lua_table("open",util.encode({name = "extend_richang", extend = {tab=1}}))
		 end
		-- task_data[i].accept_hook=function (npc,player,td) posmon.fly(player,td.need_target); end;
	end
end

function onGetTaskShortDesp(player)		
	--print("onGetTaskShortDesp", task_id)
	task.util.show_task_short_desp(player,player,task_id,task_state_data,task_data);
end
function check_mon_kill(player,mon)		task.util.check_mon_kill(player,mon,task_id,task_state_data,task_data);end
function check_item_change(player,i_name,i_id)	task.util.check_item_change(player,i_name,i_id,task_id,task_state_data,task_data);end
function get_npc_flags(npc,player)		return task.util.get_npc_flags(npc,player,task_id,task_state_data,task_data);end

function check_level(player,lv,index)
	check_richang_day(player);
	if player:get_task_state(task_id) == 0 then
		if player:get_level() >= 40 then
			local nid = gen_next_id(player);
			player:set_task_state(task_id,task.util.ts(nid,const.TSACCE));
			player:push_task_data(task_id,0);
		end
	end
	task.util.check_level(player,lv,task_id,task_state_data,task_data);
end
function check_richang_day(player)
	if util.ppn(player,const.PP_XYCM_TASKLV) == 0 then
		player:set_param(const.PP_XYCM_TASKLV,1)
	end
	player:set_param(const.PP_RICHANG_LV,1);
	if util.ppn(player,const.PP_RICHANG_DAY) ~= today() then
		player:set_param(const.PP_RICHANG_DAY,today());
		player:set_param(const.PP_RICHANG_NUM,0);
		if util.ppn(player,const.PP_HUIYUAN) >= today() then player:set_param(const.PP_RICHANG_NUM,util.ppn(player,const.PP_RICHANG_NUM)-1); end;
		
		if math.fmod(player:get_task_state(task_id), const.TASK_STATE_NUM) == const.TSNULL then
			if player:get_level() > 40 then
				local nid = gen_next_id(player);
				player:set_task_state(task_id,task.util.ts(nid,const.TSACCE));
			end
		else
			player:set_task_state(task_id,player:get_task_state(task_id));
		end

		player:push_task_data(task_id,0);
		player:refresh_npc_show_flags_inview();
	end
end

function gen_next_id(player)
	local a={};
	local pl = player:get_level();
	local taskLv = util.ppn(player, const.PP_XYCM_TASKLV);
	local minLv = 0;
	local td;
	for i = MIN_TID, MAX_TID do
		td = task_data[i];
		-- 依据任务星级筛选
		if taskLv == td.star then
			if pl >= td.need_level then --依据任务需要等级筛选
				if minLv ~= td.need_level then -- 切换区间等级区间
					a = {};
					minLv= td.need_level
				end
				a[#a+1]=i;
			end
		end
	end
	if #a > 0 then return a[math.random(1,#a)]; end;
	return MIN_TID;
end

-- if task_data[i].conditionType == const.TASK_CONDITION.MON or task_data[i].conditionType == const.TASK_CONDITION.MAPMON then
-- if task_data[i].conditionType == const.TASK_CONDITION.OWNEQUIP then
-- if task_data[i].conditionType == const.TASK_CONDITION.COPY then
-- _M['onTalkT'..task_id..'Show']=function (npc,player) task.util.show_task_talk(npc,player,task_id,task_state_data,task_data); end
-- _M['onTalkT'..task_id..'Do']=function (npc,player) return task.util.task_done(npc,player,task_id,task_state_data,task_data); end

cklevel.add_listener(check_level);
-- for i=1,#task_data do
for i = MIN_TID, MAX_TID do 
	local td = task_data[i];
	task.util.build_task_state(task_id,task_data[i],task_data,task_state_data);
	if task_data[i].conditionType == const.TASK_CONDITION.MAPMON then
		ckmon.add_all_listener(check_mon_kill);
	end
	
	task_state_data[task.util.ts(td.tid,const.TSACCE)].done2 = task_state_data[task.util.ts(td.tid,const.TSACCE)].done;
	task_state_data[task.util.ts(td.tid,const.TSACCE)].done=function(npc,player)
		if util.ppn(player,const.PP_RICHANG_NUM) < const.RICHANG_MAX_NUM then
			player:set_param(const.PP_RICHANG_NUM,util.ppn(player,const.PP_RICHANG_NUM)+1);
			player:push_lua_table("effect_one",util.encode({effect_type=60070}))
			return task_state_data[task.util.ts(td.tid,const.TSACCE)].done2(npc,player);
		else
			player:alert(1,0,"对不起,您今天的降妖除魔任务已经全部完成!");
			return 100;
		end
	end

	task_state_data[task.util.ts(td.tid,const.TSCOMP)].alltalk=function (target,player,n)
		local s=player:get_task_state(task_id);
		local b=math.floor(s / const.TASK_STATE_NUM);
		local d=math.fmod(s,const.TASK_STATE_NUM);
		local k = task_state_data[task.util.ts(td.tid,const.TSCOMP)];
		task.util.show_task_type(target,player,task_id,task_data);
		task.util.show_task_name(target,player,task_id,task_data);
		if n.award then
			task.util.show_award(target,player,n.award);
		end
		return 1;
	end;
	task_state_data[task.util.ts(td.tid,const.TSCOMP)].shortdesp=posnpc.fp(td.doner);
	task_state_data[task.util.ts(td.tid,const.TSCOMP)].done=function (npc,player)
		player:alert(1,1,"成功完成[降妖除魔]任务,获得奖励:");
		task.util.give_award(player,td, util.ppn(player,const.PP_RICHANG_LV));
		-- player:set_param(const.PP_RICHANG_NUM,util.ppn(player,const.PP_RICHANG_NUM)+1);
		fresh_task_level(player, true);
		-- local nid = gen_next_id(player);
		-- if util.ppn(player,const.PP_RICHANG_NUM) < const.RICHANG_MAX_NUM then
		-- 	player:set_task_state(task_id,task.util.ts(nid,const.TSACCE));
		-- else
		-- 	player:set_task_state(task_id,task.util.ts(nid,const.TSNULL));
		-- end

		player:push_task_data(task_id,0);
		player:set_param(const.PP_RICHANG_LV,1);
		-- player:set_param(const.PP_XYCM_TASKLV,1);
		-- task.task1000.check_task_done(player,"日常")
		task.task1000.check_complete_richang(player, const.RICHANG_TYPE.CHUMO);
		player:push_lua_table("effect_one",util.encode({effect_type=60071}))
		return 100;
	end;
end;
-- for i=1,100 do if _M['init_task_'..i] then _M['init_task_'..i](); end; end;
--下面这个函数可以用来确保任务还有多少次.用在主线或支线里面放置任务做完了卡住.
function maintask_fix(player,num)
	if util.ppn(player,const.PP_RICHANG_NUM) >= const.RICHANG_MAX_NUM then
		player:set_param(const.PP_RICHANG_NUM,util.ppn(player,const.PP_RICHANG_NUM)-num);
		task.util.check_level(player,player:get_level(),task_id,task_state_data,task_data);
	end
end

function fresh_task_level(player, auto)
	local s=player:get_task_state(task_id)
	local d=math.fmod(s,const.TASK_STATE_NUM)
	local n = util.ppn(player,const.PP_RICHANG_NUM)
	if n < const.RICHANG_MAX_NUM or d == const.TSACCE then

		-- if util.ppn(player, const.PP_MAIN_RICHANG) == 0 and task.util.checkMainRiChang(player) then
		if not auto and player:get_level() < 80 then
			player:set_param(const.PP_XYCM_TASKLV, 10);
			-- player:set_param(const.PP_MAIN_RICHANG, 1);
		else
			local dt = math.random(10,109);
			player:set_param(const.PP_XYCM_TASKLV,math.floor(dt/10));
		end

		local nid = gen_next_id(player);
		player:set_task_state(task_id,task.util.ts(nid,const.TSACCE));
	else
		local nid = gen_next_id(player);
		player:set_task_state(task_id,task.util.ts(nid,const.TSNULL));
	end
end

function task_done(npc,player)
	local ret = 100;
	local s = player:get_task_state(task_id);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local n=task_data[b];
	local k = task_state_data[s];
	-- print("task_done", s, d, b, util.encode(n), util.encode(k))
	local need_npc;
	if d == const.TSACCE then
		need_npc = n.accepter;
	end
	if d == const.TSCOMP then
		need_npc = n.doner;
	end
	if k and need_npc then
		if k and k.done then ret = k.done(npc,player);end
		player:refresh_npc_show_flags_inview();
	end
	return ret;
end

function direct_fly_target(npc,player)
	local s=player:get_task_state(task_id);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local k=task_state_data[s];
	local n=task_data[b];
	if d == const.TSACED then
		if n then
			-- print("direct_fly_target", n.checkID)
			if n.conditionType == const.TASK_CONDITION.MAPMON then
				-- print(11111)
				if util.isTable(n.teleportID) then
					-- print(2222)
					posmap.fly(player, n.teleportID[math.random(1,#n.teleportID)], "autofightstart");
				end
			elseif n.conditionType == const.TASK_CONDITION.MON then
				-- print("333333333");
				posmon.fly(player, n.checkID);
			end
		end
	end
end
function task_dirdone(npc,player)
	local s=player:get_task_state(task_id);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local k=task_state_data[s];
	local n=task_data[b];
	if k and n then
		if d == const.TSACED then
			if 1== player:sub_vcoin(get_direct_comp_price(player), "task2000直接完成") then
				player:set_param(const.PP_RICHANG_LV,3);
				player:set_task_state(task_id,task.util.ts(b,const.TSCOMP));
				player:push_task_data(task_id,0);
				task_done(npc,player);
			else
				-- player:alert(1,1,"元宝不足"..get_direct_comp_price(player).."个,无法直接完成");
				gui.PanelBuyvcoin.show_vcoin_num(player,"元宝不足"..get_direct_comp_price(player).."个,无法直接完成");
				return 100;
			end
		else
			player:alert(1,1,"不在已接受任务状态");
			return 100;
		end
	end
end
function get_direct_comp_price(player)
	return 50;
end

function get_mon_num( player )
	local s=player:get_task_state(const.TASK_ID_RICHANG);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local n=task_data[b];
	if n then
		return util.filterNumberOfString(n.need_target).." "..n.need_num
	end
end


function check_task_avariable(player)
	local s=player:get_task_state(const.TASK_ID_RICHANG);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local n=task_data[b];
	if not n then
		local nid = gen_next_id(player);
		player:set_task_state(task_id,task.util.ts(nid,const.TSACCE));
		player:push_task_data(task_id,0);
	end
end

function get_award_desp(player)
	local s = player:get_task_state(task_id);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local n=task_data[b];
	local award = {exp = 0, reputation = 0};
	if n then
		award = {
			exp = n.exp, 
			reputation = n.shengwang
		}
	end
	return award
end

function get_task_desp(player)
	local s = player:get_task_state(task_id);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local n=task_data[b];
	local desp = "降妖除魔";
	if n then
		if d == const.TSCOMP then
			desp = n.enddialog
		else
			desp = n.dialog
		end
	end
	return desp
end

-- 新版降妖除魔任务，任务星级和任务具体任务内容挂钩

