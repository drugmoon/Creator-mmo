module(..., package.seeall)
local task_id=const.TASK_ID_MAIN;
-- local task_data={
-- 	ttype="主线",		focus=true,		autogo=true,
-- 	[1	]={
-- 		tid=1,		nid=64,		need_level=1,		name="捉鸡真捉急",	accepter="诗人",	doner="张亮",
-- 		npchead = 14,
-- 		need_type="mon",	need_target="鸡",			need_num=1,
-- 		award={
-- 			{name="经验", 	itemId = 19001,		num=700},
-- 			{name="木剑",	num=1,itemId = 20001,flag=10,},
-- 			{name="布甲",	num=1,gender='male',itemId = 30001,flag=10,},
-- 			{name="布衣",	num=1,gender='female',itemId = 35001,flag=10,},
-- 			{name="魔血石(小)",	num=1,bind=1, use=true,itemId = 10026,},
-- 		},
-- 		accept_talk=function (npc,player,td) return "重新出发吧"..player:get_name().."，没有什么可以阻挡你前进的脚步。";end;
-- 		done_talk  =function (npc,player,td) return "感谢你的鸡，祝你好运！";end;
-- 		-- accept_hook = function (npc,player,td) posnpc.showFlyTips(player,td.doner); end;
-- 	},
-- };

local task_data = tab.TaskData1000;
task_data.ttype="主线";
task_data.focus=true;
task_data.autogo=true;

local task_state_data={};
function showTaskInfo(player)
	return task.util.show_task_info(player,player,task_id,task_state_data,task_data);
end

function onGetTaskShortDesp(player)		task.util.show_task_short_desp(player,player,task_id,task_state_data,task_data);end

function show_task(npc,player)			task.util.show_main_task_talk(npc,player,task_id,task_state_data,task_data);end

function get_npc_flags(npc,player)		return task.util.get_npc_flags(npc,player,task_id,task_state_data,task_data);end

function check_mon_kill(player,mon)		task.util.check_mon_kill(player,mon,task_id,task_state_data,task_data);end
function check_item_change(player,i_name,i_id)	task.util.check_item_change(player,i_name,i_id,task_id,task_state_data,task_data);end
function check_level(player,lv)			task.util.check_level(player,lv,task_id,task_state_data,task_data);end
function check_zslevel(player,zslv)			task.util.check_zslevel(player,zslv,task_id,task_state_data,task_data);end
function check_enter_copy(player,map)	task.util.check_enter_copy(player,map,task_id,task_state_data,task_data);end
function check_leave_copy(player,map)	task.util.check_leave_copy(player,map,task_id,task_state_data,task_data);end

function check_enter_map(player,map) 	task.util.check_enter_map(player,map,task_id,task_state_data,task_data);end
function check_join_guild(player) 		task.util.check_join_guild(player,task_id,task_state_data,task_data);end
function check_join_group(player) 		task.util.check_join_group(player,task_id,task_state_data,task_data);end
function check_add_friend(player) 		task.util.check_add_friend(player,task_id,task_state_data,task_data);end
function check_get_exploit(player,exploit)	task.util.check_get_exploit(player,exploit,task_id,task_state_data,task_data);end
function check_inner_power_level(player,lv)			task.util.check_inner_power_level(player,lv,task_id,task_state_data,task_data);end

function check_complete_richang(player, richangType)
	local s = player:get_task_state(task_id);
	local b = math.floor(s / const.TASK_STATE_NUM);
	local d = math.fmod(s,const.TASK_STATE_NUM);
	local k = task_data[b];
	if not k then return end
	if k.conditionType ~= const.TASK_CONDITION.RICHANG or d ~= const.TSACED then return end
	if richangType ~= k.checkID then return end
	player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
	if player:get_task_param(task_id,1) >= k.checkCount then
		player:set_task_state(task_id,task.util.ts(k.tid,const.TSCOMP)); -- 脚本调用set_task_state 引擎会自动回调脚本刷新任务描述
		if k.complate_hook then k.complate_hook(npc,player,k);end;
		if task_data.autogo and (not k.noautogo) then
			posnpc.go(player,k.doner);
		end
	end
	player:push_task_data(task_id,0);
end

function check_mon_caiji(player,mon,group)
	-- if group then return;end;
	-- local s = player:get_task_state(task_id);
	-- local b = math.floor(s / const.TASK_STATE_NUM);
	-- local k = task_data[b]
	-- if k then
	-- 	if b == 8 then
	-- 		if s ==  task.util.ts(8,const.TSCOMP) then
	-- 			player:set_task_state(task_id,task.util.ts(8,const.TSCOMP));
	-- 			posnpc.go(player,"慕容仲伯");
	-- 			-- focus.f(player,"mini_task");
	-- 		end
	-- 		player:push_task_data(task_id,0);
	-- 	end
	-- 	if b == 84 then
	-- 		if s ==  task.util.ts(84, const.TSACED) then
	-- 			-- player:set_task_param(task_id, 1, player:get_task_param(task_id,1)+1);
	-- 			if player:get_task_param(task_id, 1) >= k.need_num then
	-- 				player:set_task_state(task_id,task.util.ts(84, const.TSCOMP));
	-- 				if k.complate_hook then k.complate_hook(npc,player,k);end;
	-- 				if task_data.autogo and (not k.noautogo) then posnpc.go(player,k.doner);end
	-- 			end
	-- 			-- player:set_task_state(task_id,task.util.ts(84,const.TSCOMP));
	-- 			-- posnpc.go(player,"孟庆");
	-- 			-- focus.f(player,"mini_task");
	-- 		end
	-- 		player:push_task_data(task_id,0);
	-- 	end
	-- 	if b == 76 then
	-- 		if s ==  task.util.ts(76,const.TSACED) then
	-- 			if mon:get_name() == "鱼1" then
	-- 				if player:num_bag_black() >= 1 then
	-- 					player:add_item("青鱼",1,1);
	-- 					player:alert(11,1,"成功捕获:<font color='#ff0000'>青鱼</font>");
	-- 				end
	-- 			end
	-- 		end
	-- 		if s == task.util.ts(76,const.TSCOMP) then
	-- 			posnpc.fly(player,"温泉老板娘");
	-- 		end
	-- 		player:push_task_data(task_id,0);
	-- 	end
	-- end
end
-- ckmon.add_caiji_listener("霜草",check_mon_caiji);
-- ckmon.add_caiji_listener("铁矿",check_mon_caiji);
-- ckmon.add_caiji_listener("鱼1",check_mon_caiji);

function check_task_done(player,panel_name)
	local s = player:get_task_state(task_id);
	local b = math.floor(s / const.TASK_STATE_NUM);
	local d = math.fmod(s,const.TASK_STATE_NUM);
	local k = task_data[b]
	if k then
		if k.conditionType == const.TASK_CONDITION.OWNEQUIP then
			if d == const.TSCOMP then
				if task_data.autogo then 
					--posnpc.go(player,k.doner);
					player:push_lua_table("continueTask","");
				end
			end
		end
	end
end

function startkuang(player,mon)
	player:start_progress_bar(mon:get_caiji_duration(),"正在采集中...");
end
function startbuzhuo(player,mon)
	player:start_progress_bar(mon:get_caiji_duration(),"正在捕捉中...");
end
-- ckmon.add_caijistart_listener("霜草",startkuang);
-- ckmon.add_caijistart_listener("铁矿",startkuang);
-- ckmon.add_caijistart_listener("鱼1",startbuzhuo);

_M['onTalkT'..task_id..'Show']=function (npc,player) task.util.show_main_task_talk(npc,player,task_id,task_state_data,task_data); end
_M['onTalkT'..task_id..'Do']=function (npc,player) return task.util.task_done(npc,player,task_id,task_state_data,task_data); end


-- for i = 1, #task_data do
for i,v in pairs(task_data) do
	if util.isTable(task_data[i]) then
		task.util.build_task_state(task_id,task_data[i],task_data,task_state_data);
		if task_data[i].need_type == "mon" then
			ckmon.add_all_listener(check_mon_kill);
		end

		if task_data[i].conditionType == const.TASK_CONDITION.MON or task_data[i].conditionType == const.TASK_CONDITION.MAPMON then
			task_data[i].no_group = true;
			ckmon.add_all_listener(check_mon_kill);
		end

		if task_data[i].need_type == "item" then
			ckitem.add_listener(task_data[i].need_item,check_item_change);
		end
		if task_data[i].conditionType == const.TASK_CONDITION.OWNEQUIP then
			ckitem.add_all_listener(check_item_change);
		end
		if task_data[i].conditionType == const.TASK_CONDITION.COPY then
			ckcopy.add_listener(task_data[i].checkID, check_enter_copy);
		end
		if task_data[i].conditionType == const.TASK_CONDITION.ENTERMAP then
			-- print('ckmap.add_listener', task_data[i].checkID)
			ckmap.add_listener(task_data[i].checkID, check_enter_map);
		end
		if task_data[i].conditionType == const.TASK_CONDITION.JOINGUILD then
			ckguild.add_all_listener(check_join_guild);
		end
		if task_data[i].conditionType == const.TASK_CONDITION.JOINGROUP then
			ckgroup.add_all_listener(check_join_group);
		end
		ckinnerpowerlevel.add_listener(check_inner_power_level)
		cklevel.add_listener(check_level);
		ckzslevel.add_listener(check_zslevel);
	end
end;

for i=1,100 do if _M['init_task_'..i] then _M['init_task_'..i](); end; end;

function first_login(player)
	player:set_task_state(task_id,task.util.ts(10001, const.TSCOMP));
	player:push_task_data(task_id,1);
end
login.add_first_login_listener(first_login);

--主线tips
function reqTaskTips(player)
	local s = player:get_task_state(task_id);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local b=math.floor(s/const.TASK_STATE_NUM);
	local n=task_data[b];
	if n.acceptTips and n.acceptTips ~= "" then
		player:push_lua_table("showTaskTips",util.encode({tips = n.acceptTips}));
	end
end

ckpanel.add_listener("task.task1000.reqTaskTips", task.task1000.reqTaskTips)

function isLastTask(tid)
	local lastTask = false;
	local td = task_data[tid];
	if td and td.nid == 0 then
		lastTask = true;
	end
	return lastTask;
end


function process_task(player)
	local s = player:get_task_state(task_id);
	local b = math.floor(s / const.TASK_STATE_NUM);
	local d = math.fmod(s,const.TASK_STATE_NUM);
	local k = task_data[b];
	if not k then return end
	if b == 10038 or b == 10062 or b == 10050 then
		player:set_task_state(task_id,task.util.ts(b, const.TSCOMP));
		player:push_task_data(task_id,0);
	end
end

login.add_login_listener(process_task)