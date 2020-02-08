module(..., package.seeall)
function task_state(base,state) return base*const.TASK_STATE_NUM+state;end;ts=task_state;
function fix_npc_flags(flags)
	local ret = 0;
	if math.fmod(flags,10)>=1 then ret = ret + 1; end
	if math.fmod(flags/10,10)>=1 then ret = ret + 10; end
	if math.fmod(flags/100,10)>=1 then ret = ret + 100; end
	if math.fmod(flags/1000,10)>=1 then ret = ret + 1000; end
	if flags < 0 then ret = flags; end
	return ret;
end

local function isNewVersion(td)
	if td.conditionType then
		return true
	end
end

function get_task_state(player, task_id)
	local s = player:get_task_state(task_id)
	local ts = math.fmod(s,const.TASK_STATE_NUM)
	local tid = math.floor(s/const.TASK_STATE_NUM)
	return tid, ts;
end

-- 主线第一次降妖除魔 
function checkMainRiChang(player)
	local s = player:get_task_state(1000)
	local d = math.fmod(s,const.TASK_STATE_NUM)
	local b = math.floor(s/const.TASK_STATE_NUM)
	if b == 10036 and d == const.TSACED then
		return true;
	end
end

-- 依据职业性别获得任务奖励
local function getJobGenderAward(player, td)
	local job = player:get_job_name();
	local gender = player:get_gender_name();
	local jobGenderAward = 0;
	if job == "warrior" then
		if gender == "male" then
			jobGenderAward = td.zhanMaleID;
		else
			jobGenderAward = td.zhanFemaleID;
		end
	elseif job == "wizard" then
		if gender == "male" then
			jobGenderAward = td.faMaleID;
		else
			jobGenderAward = td.faFemaleID;
		end
	elseif job == "taoist" then
		if gender == "male" then
			jobGenderAward = td.daoMaleID;
		else
			jobGenderAward = td.daoFemaleID;
		end
	end
	return jobGenderAward
end

--判断背包格子是否能放下奖励
function canTaskDone(player, td)
	local needBag = 0
	if td.itemCount and td.itemCount > 0 then
		if not util.isSkillItem(td.itemBaseID) then
			needBag = needBag + 1;		
		end
	end

	local jobGenderAward = getJobGenderAward(player, td);
	if jobGenderAward > 0 then
		if not util.isSkillItem(td.itemBaseID) then
			needBag = needBag + 1;		
		end
	end
	return player:num_bag_black() >= needBag;
end

function give_award(player,td, mul)
	if not mul then mul = 1;end;

	local job = player:get_job_name();
	local gender = player:get_gender_name();

	if td.awardGoldBind and td.awardGoldBind > 0 then
		player:add_gamemoney_bind(td.awardGoldBind * mul);
	end
	if td.awardVcoinBind and td.awardVcoinBind > 0 then
		player:add_vcoin_bind(td.awardVcoinBind * mul);
	end
	if td.exp and td.exp > 0 then
		player:add_exp(td.exp * mul);
	end
	if td.shengwang and td.shengwang > 0 then
		player:set_param(const.PP_GUANZHI_SW,util.ppn(player,const.PP_GUANZHI_SW)+td.shengwang * mul);
	end

	if td.itemCount and td.itemCount > 0 then
		if util.isSkillItem(td.itemBaseID) then
			util.addSkill(player, td.itemBaseID);
		elseif util.isViceEquipMent(td.itemBaseID) then -- 针对副装特特殊处理
			player:add_item(td.itemBaseID,td.itemCount,td.itemBind,0,nil,nil,nil,nil,nil,nil,nil,nil,nil,td.flag or 2);
			player:use_item(td.itemBaseID);
			player:push_lua_table("fakeShowItem",util.encode({mTypeID = td.itemBaseID}));
		else
			if player:num_bag_black() <= 0 then player:alert(1,1,"背包已满,请去仓库获取任务奖励物品!"); end
			player:add_item(td.itemBaseID,td.itemCount,td.itemBind,0,nil,nil,nil,nil,nil,nil,nil,nil,nil,td.flag or 2);
		end
	end

	local jobGenderAward = getJobGenderAward(player, td);
	if jobGenderAward > 0 then
		if util.isSkillItem(jobGenderAward) then
			util.addSkill(player, jobGenderAward);
		elseif util.isViceEquipMent(jobGenderAward) then -- 针对副装特特殊处理
			-- print("hahahhahaha", jobGenderAward);
			player:add_item(jobGenderAward,1,1,0,nil,nil,nil,nil,nil,nil,nil,nil,nil,td.flag or 2);
			player:use_item(jobGenderAward);
			player:push_lua_table("fakeShowItem",util.encode({mTypeID = jobGenderAward}));
		else
			if player:num_bag_black() <= 0 then player:alert(1,1,"背包已满,请去仓库获取任务奖励物品!"); end
			player:add_item(jobGenderAward,1,1,0,nil,nil,nil,nil,nil,nil,nil,nil,nil,td.flag or 2);
		end
	end
end

function show_task_name(target,player,task_id,data)
	local s = player:get_task_state(task_id);
	local b = math.floor(s / const.TASK_STATE_NUM);
	local d = math.fmod(s,const.TASK_STATE_NUM);
	local k = data[b];
	local n = const.TASK_STATE_NAME[d];
	if k and n then
		target:echo(k.name);
		target:echo(""..n.."");
	end
end

function get_npc_flags(target,player,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local b=math.floor(s / const.TASK_STATE_NUM);
	local n=td[b];
	if not n then return 0;end;
	if d == const.TSACCE then
		if n.accepter==0 or n.accepter == target:get_def_id() then
			return 1;
		end
	end
	if d == const.TSCOMP then
		if n.doner==0 or n.doner == target:get_def_id() then
			return 10;
		end
	end
	if d == const.TSACED then
		if n.doner == target:get_def_id() then
			return -10
		end
	end
	return 0;
end

---------------------------------luatable-----------------------------------
--显示客户端左边面板上的任务信息
function show_task_short_desp(target,player,task_id,data,td)
	local s=player:get_task_state(task_id)
	local b=math.floor(s/const.TASK_STATE_NUM)
	local d=math.fmod(s,const.TASK_STATE_NUM)
	
	local k=td[b]
	if k then
		local tsd={}
		tsd.task_type=td.ttype
		tsd.task_state=const.TASK_STATE_NAME[d]
		tsd.task_name=k.name
		if d == const.TSACCE then  --任务可以接受
			tsd.task_target = posnpc.sp(k.accepter)
			tsd.target_fly=posnpc.fid(k.accepter)
			-- tsd.task_type="npc";
			--if task_id == const.TASK_ID_RICHANG then
				tsd.desc = "<font color=#ffffff>寻找:<font color=#30ff00>"..player:get_npc_name(k.accepter).."</font>接任务</font>";
			--end
		elseif d == const.TSACED then   --任务已经接受
			if isNewVersion(k) then
				if k.conditionType == const.TASK_CONDITION.MON or k.conditionType == const.TASK_CONDITION.MAPMON then
					tsd.target_num = k.checkCount
					tsd.target_type="mon"
					tsd.target_name=k.checkID
					if k.checkCount then
						if player:get_task_param(task_id,1) >= k.checkCount then
							tsd.task_target = posnpc.sp(k.doner)
							tsd.target_fly = posnpc.fid(k.doner)
						else
							if k.conditionType == const.TASK_CONDITION.MON then
								tsd.task_target = posmon.sp(player, k.checkID);
								tsd.target_fly = posmon.fid(player, k.checkID);

								-- tsd.task_target = posmon.sp(player,k.teleportID,k.checkID)
								-- tsd.target_fly = posmon.fid(player,k.teleportID,k.checkID)
							else
								-- print("task_target", k.teleportID);
								tsd.task_target = posmap.sp(k.teleportID, "autofightstart");
								tsd.target_fly = posmap.fid(k.teleportID);
							end
						end
					end
				elseif k.conditionType == const.TASK_CONDITION.LEVELUP then
					--print("////////////////////////////////////")
					tsd.target_num = k.checkID
					tsd.target_type="level"
					tsd.target_name=k.checkCount
					-- 特殊处理，卡等级的时候，找降妖除魔npc
					tsd.task_target = posnpc.sp(4002015)
					tsd.target_fly = posnpc.fid(4002015)
				elseif k.conditionType == const.TASK_CONDITION.OWNEQUIP then
					tsd.target_num = k.checkCount
					tsd.target_type="ownequip"
					tsd.target_name=k.checkID
					if k.exp and k.exp > 0 then
						tsd.desc = "<font color=#ffffff>奖励:<font color=#ff1fec>"..math.floor(k.exp/10000).."万经验</font></font>";
					end
				elseif k.conditionType == const.TASK_CONDITION.NPCTALK then
					tsd.task_target = posnpc.sp(k.doner)
				elseif k.conditionType == const.TASK_CONDITION.COPY then
					tsd.desc = "<font color='#FFFFFF'>击杀<font color='#30FF00'>地图内所有怪物</font></font>"
					tsd.task_target ="autofight"
				elseif k.conditionType == const.TASK_CONDITION.RICHANG then
					tsd.task_target = posnpc.sp(k.doner)
					tsd.target_fly = posnpc.fid(k.doner)
				elseif k.conditionType == const.TASK_CONDITION.JOINGUILD then
					-- tsd.target_fly = posnpc.fid(k.doner);
					tsd.task_target = "click_open|guild";
					tsd.desc = "<font color=#ffffff>创建或加入帮会</font>"
				elseif k.conditionType == const.TASK_CONDITION.JOINGROUP then
					-- tsd.target_fly = posnpc.fid(k.doner);
					tsd.task_target = "click_open|group";
					tsd.desc = "<font color=#ffffff>创建或加入队伍</font>"
				elseif k.conditionType == const.TASK_CONDITION.ADDFRIEND then
					-- tsd.target_fly = posnpc.fid(k.doner);
					tsd.task_target = "click_open|friend";
					tsd.target_num = k.checkCount;
					tsd.desc = "<font color=#ffffff>添加：</font><font color=#30ff00>("..player:get_task_param(task_id,1).."/"..k.checkCount..")名好友</font>"
				elseif k.conditionType == const.TASK_CONDITION.ENTERMAP then
					if k.checkID == "mysd2f" or k.checkID ==  "mysd4f" then
						tsd.task_target = "click_goto|npc|4002024";
					elseif k.checkID == "2clfb" then
						tsd.task_target = "click_goto|npc|4002011";
					end
					tsd.desc = "<font color=#ffffff>探索：</font><font color=#30ff00>"..tab.getMapName(k.checkID).."</font>"
				elseif k.conditionType == const.TASK_CONDITION.GETEXPLOIT then
					tsd.task_target = "click_open|exploit";
					tsd.target_num = k.checkCount;
					tsd.desc = "<font color=#ffffff>获得：</font><font color=#30ff00>("..player:get_task_param(task_id,1).."/"..k.checkCount..")点功勋</font>"
				elseif k.conditionType == const.TASK_CONDITION.INNERPOWERLEVELUP then
					tsd.target_num = k.checkID
					tsd.target_type="innerpowerlevel"
					tsd.target_name=k.checkCount
					tsd.task_target = "click_open|innerpower"
				end
				-- if task_id == const.TASK_ID_MAIN then
				-- 	tsd.unSelected = true;
				-- end
			end
		elseif d == const.TSCOMP then  --任务完成
			if task_id == const.TASK_ID_RICHANG then
				tsd.task_target = posnpc.sp(k.doner)
			else
				if k.dir_fly then
					tsd.task_target = posnpc.fid(k.doner)
				else
			    	tsd.task_target = posnpc.sp(k.doner)
			    end
			end
			tsd.target_fly = posnpc.fid(k.doner)
			if isNewVersion(k) then
				tsd.task_state = tsd.task_state
				if k.conditionType == const.TASK_CONDITION.OWNEQUIP then
					if k.exp and k.exp > 0 then
						tsd.desc = {
							"<font color='#FFFFFF'>提交：<font color='#30FF00'>"..player:get_npc_name(k.doner).."</font></font>",
							"<font color=#ffffff>奖励:<font color=#ff1fec>"..math.floor(k.exp/10000).."万经验</font></font>",
						};
					end
				else
					tsd.desc = "<font color='#FFFFFF'>提交：<font color='#30FF00'>"..player:get_npc_name(k.doner).."</font></font>";
				end
				tsd.selected = true;
				if task_id == const.TASK_ID_BRANCH then
					if task.task1100.isFirstTask(k.tid) then
						tsd.selected = false;
					end
				end
			end
		elseif d == const.TSUNAC then
			tsd.need_level=k.need_level;
			-- tsd.task_target = pospanel.sp("日常")
		end
		if tsd.target_type == "mon" then
			if k.conditionType == const.TASK_CONDITION.MON then
				if tsd.target_name == 0 then
					tsd.target_name = "任意怪物"
				else
					tsd.target_name = tab.getMonName(tsd.target_name);
					tsd.target_map=posmon.getMapId(k.checkID)
				end
			elseif k.conditionType == const.TASK_CONDITION.MAPMON then
				if tsd.target_name == 0 then
					tsd.target_name = "任意怪物"
				else
					tsd.target_name = tab.MapInfo[tsd.target_name].mapname;
					-- tsd.desc = "<font color=#ffffff>获得:<font color=#30ff00>大量经验</font></font>";
				end
			end
		end

		-- print("tsd", util.encode(tsd));
		target:echo(util.encode(tsd))
	end
end

function show_main_task_talk(target, player, task_id, data, td)
	local s=player:get_task_state(task_id)
	local b=math.floor(s/const.TASK_STATE_NUM)
	local d=math.fmod(s,const.TASK_STATE_NUM)
	local k=data[s]
	local n=td[b]
	local need_npc
	local task_str = ""
	if d == const.TSACCE then
		need_npc = n.accepter
	end
	if d == const.TSCOMP then
		need_npc = n.doner
	end

	if k and need_npc then
		local tsd={};
		tsd.flag="m_tasknpc"
		tsd.model="tasknpc"
		tsd.task_name=n.name
		tsd.task_type=td.ttype
		tsd.npc=need_npc
		if type(k.talk)=="string" then
			tsd.desc=k.talk
		end
		if type(k.talk)=="function" then
			tsd.desc=k.talk(target,player,n)
		end
		tsd.award={}
		tsd.npchead = n.npchead
		if isNewVersion(n) then
			--基础奖励
			if n.itemBaseID and n.itemBaseID > 0 then
				table.insert(tsd.award, {
					itemId = n.itemBaseID,
					num = n.itemCount,
					bind = n.itemBind,
				});
			end
			--职业性别奖励
			local jobGenderAward = getJobGenderAward(player, n)
			if jobGenderAward > 0 then
				table.insert(tsd.award, {
					itemId = jobGenderAward,
					num = 1,
					bind = 1,
				});
			end
			-- exp
			if n.exp and n.exp > 0 then
				table.insert(tsd.award, {
					itemId = 40000001,
					num = n.exp,
					bind = 0,
				});
			end
			if n.shengwang and n.shengwang > 0 then
				table.insert(tsd.award, {
					itemId = 40000007,
					num = n.shengwang,
					bind = 0,
				});
			end
			if n.awardGoldBind and n.awardGoldBind > 0 then
				table.insert(tsd.award, {
					itemId = 40000004,
					num = n.awardGoldBind,
					bind = 0,
				});
			end
			if n.awardVcoinBind and n.awardVcoinBind > 0 then
				table.insert(tsd.award, {
					itemId = 40000006,
					num = n.awardVcoinBind,
					bind = 0,
				});
			end
		else
			if n.award then
				for i=1,#n.award do
					if n.award[i].job and n.award[i].gender then
						if n.award[i].job==player:get_job_name() and n.award[i].gender==player:get_gender_name() then
							table.insert(tsd.award,n.award[i])
						end
					elseif n.award[i].job then
						if n.award[i].job==player:get_job_name() then
							table.insert(tsd.award,n.award[i])
						end
					elseif n.award[i].gender then
						if n.award[i].gender==player:get_gender_name() then
							table.insert(tsd.award,n.award[i])
						end
					else
						table.insert(tsd.award,n.award[i])
					end
				end
			end
		end

		tsd.event="T" .. task_id .. "Do"
		if d == const.TSACCE then tsd.is_done=false end
		if d == const.TSCOMP then tsd.is_done=true end
		target:echo(util.encode(tsd))
	end
end

---------------------------------luatable-----------------------------------

function task_done(npc,player,task_id,data,td)
	local ret = 100;
	local s = player:get_task_state(task_id);
	local d=math.fmod(s,const.TASK_STATE_NUM);
	local b=math.floor(s/const.TASK_STATE_NUM);
	local n=td[b];
	local k = data[s];
	local need_npc;
	if d == const.TSACCE then
		need_npc = n.accepter;
	end
	if d == const.TSCOMP then
		need_npc = n.doner;
	end
	if k and need_npc and (need_npc==0 or (not npc) or need_npc == npc:get_def_id() ) then
		if k and k.done then ret = k.done(npc,player);end

		if task_id == const.TASK_ID_MAIN then
			if d == const.TSCOMP then 
				-- gui.moduleGuide.handleMainTaskDone(player, b)
				-- player:push_lua_table("effect_one",util.encode({effect_type=60071}))
				-- cktask.triger_task(player, task_id, b)
				-- gui.moduleGuiButton.onTaskChange(player);
			elseif d == const.TSACCE then
				-- player:push_lua_table("effect_one",util.encode({effect_type="task_acc"}));
				-- gui.moduleGuide.addSkillByTid(player, b)
			end
		end
		player:refresh_npc_show_flags_inview();
	end
	return ret;
end
function check_mon_kill(player,mon,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	-- print(task_id,"check_mon_kill",s,d)
	if d and d.on_mon_kill then
		d.on_mon_kill(mon,player);
	end
end
function check_mon_kill_all(player,mon,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_mon_kill_all then
		d.on_mon_kill_all(mon,player);
	end
end
function check_item_change(player,item_name,item_id,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_item_change then
		d.on_item_change(player,item_name,item_id);
	end
end
function check_level(player,lv,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_level_change then
		d.on_level_change(player,lv);
	end
end

function check_zslevel(player, zslv, task_id, data, td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_zslevel_change then
		d.on_zslevel_change(player, zslv);
	end
end

function check_enter_copy(player,map,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_enter_copy then
		d.on_enter_copy(player,map);
	end
end

function check_leave_copy(player, map, task_id, data, td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_leave_copy then
		d.on_leave_copy(player,map);
	end
end

function check_enter_map(player,map,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	-- print("check_enter_map")
	if d and d.on_enter_map then
		-- print("check_enter_map", d.on_enter_map)
		d.on_enter_map(player,map);
	end
end

function check_join_guild(player, task_id, data, td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_join_guild then
		d.on_join_guild(player);
	end
end

function check_join_group(player, task_id, data, td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_join_group then
		d.on_join_group(player);
	end
end

function check_add_friend(player, task_id, data, td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	-- print("check_add_friend", task_id, s, d.on_add_friend)
	if d and d.on_add_friend then
		d.on_add_friend(player);
	end
end

function check_get_exploit(player, exploit, task_id, data, td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_get_exploit then
		d.on_get_exploit(player, exploit);
	end
end

function check_inner_power_level(player,lv,task_id,data,td)
	local s = player:get_task_state(task_id);
	local d = data[s];
	if d and d.on_inner_power_level_change then
		d.on_inner_power_level_change(player,lv);
	end
end

local function check_task_acceptable(player, td)
	if td.isClose then
		return false
	end

	local acceptable = true
	if td.need_level and player:get_level() < td.need_level then
		acceptable = false
	end
	if acceptable and td.need_zslevel and util.getZsLevel(player) < td.need_zslevel then
		acceptable = false
	end
	return acceptable
end

--目前单线，不支持多个条件
local function check_task_done(player, td)
	if td.isClose then
		return false
	end

	local acceptable = true
	if td.need_level and player:get_level() < td.need_level then
		acceptable = false
	end
	if acceptable and td.need_zslevel and util.getZsLevel(player) < td.need_zslevel then
		acceptable = false
	end
	return acceptable
end

local equipPos = {
	const.ITEM_WEAPON_POSITION,
	const.ITEM_CLOTH_POSITION,
	const.ITEM_GLOVE1_POSITION,
	const.ITEM_RING1_POSITION,
	const.ITEM_BOOT_POSITION,
	const.ITEM_HAT_POSITION,
	const.ITEM_NICKLACE_POSITION,
	const.ITEM_GLOVE2_POSITION,
	const.ITEM_RING2_POSITION,
	const.ITEM_BELT_POSITION,
	const.ITEM_JADE_PENDANT_POSITION,
	const.ITEM_SHIELD_POSITION,
	const.ITEM_MIRROR_ARMOUR_POSITION,
	const.ITEM_FACE_CLOTH_POSITION,
	const.ITEM_DRAGON_HEART_POSITION,
	const.ITEM_WOLFANG_POSITION,
	const.ITEM_DRAGON_BONE_POSITION,
	const.ITEM_CATILLA_POSITION,
};

local function get_equip_count(player, needparam, count)
	local sum = 0;
	local jobName = player:get_job_name();
	for i,v in pairs(equipPos) do -- 先查找身上，再查找背包
		local typeId = player:get_item_type_by_pos(v);
		if typeId and typeId > 0 then
			if util.checkEquipNeedLevel(typeId, jobName, needparam) then 
				sum = sum + 1;
				if sum >= count then break; end
			end
		end
	end
	if sum < count then
		for pos = const.ITEM_BAG_BEGIN, (const.ITEM_BAG_MAX - 1) do
			local typeId = player:get_item_type_by_pos(pos);
			if typeId and typeId > 0 then
				if util.checkEquipNeedLevel(typeId, jobName, needparam) then 
					sum = sum + 1;
				if sum >= count then break; end
				end
			end
		end
	end
	return sum;
end

function build_task_state(task_id,td,task_data,task_state_data)
	task_state_data[task.util.ts(td.tid,const.TSUNAC)]={
		shortdesp="人物达到"..(td.need_level or td.need_zslevel.."转"),
		
		on_level_change=function (player,lv)
			if check_task_acceptable(player, td) then
				player:set_task_state(task_id,task.util.ts(td.tid,const.TSACCE));
				player:push_task_data(task_id,0);
			end
		end,

		on_zslevel_change=function (player, zslv)
			if check_task_acceptable(player, td) then
				player:set_task_state(task_id,task.util.ts(td.tid,const.TSACCE));
				player:push_task_data(task_id,0);
			end
		end,
	};
	task_state_data[task.util.ts(td.tid,const.TSACCE)]={
		shortdesp="找"..posnpc.mp(td.accepter),
		done=function (npc,player) --接取任务时执行的回调
			-- print("task accept done")
			local ret=100;
			if td.accept_hook then td.accept_hook(npc,player,td);ret=nil;end;
			if isNewVersion(td) then -- 新版本类型
				if td.conditionType == const.TASK_CONDITION.LEVELUP then --升级任务检测
					local level = player:get_level();
					player:set_task_param(task_id,1,math.min(level, td.checkID) );
					if level >= td.checkID then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
					else
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));ret=nil;
						if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
					end
				elseif td.conditionType == const.TASK_CONDITION.INNERPOWERLEVELUP then --升级任务检测
					local level = gui.PanelInnerPower.getInnerPowerLevel(player)
					player:set_task_param(task_id,1,math.min(level, td.checkID) );
					if level >= td.checkID then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
					else
						local openPanel = false
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));ret=nil;
						if td.tid then
							openPanel = gui.moduleGuide.checkOpenPanel(player, td.tid);
						end
						if openPanel then
							ret=nil;
						else
							if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
						end
					end
				elseif td.conditionType == const.TASK_CONDITION.OWNEQUIP then --获得指定装备任务检测
					local count = get_equip_count(player, td.checkID, td.checkCount);
					player:set_task_param(task_id,1,math.min(count, td.checkCount) );
					if count >= td.checkCount then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
					else
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));

						local openPanel = false
						if td.tid then
							openPanel = gui.moduleGuide.checkOpenPanel(player, td.tid);
						end
						if openPanel then
							ret=nil;
						else
							if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
						end

					end
				elseif td.conditionType == const.TASK_CONDITION.COPY then
					--直接进入副本 任务状态设为已经完成
					player:set_task_param(task_id,1,0);
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
					player:push_task_data(task_id,0);
					if td.checkID == "kuang3jia" then
						map.kuang3jia.enter_map(player);
					elseif td.checkID == "dushe3jia" then
						map.dushe3jia.enter_map(player);
					elseif td.checkID == "kingjia" then
						map.kingjia.enter_map(player);
					end
					ret = nil; -- 不调用npc的onTalk100
					-- if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
				elseif td.conditionType == const.TASK_CONDITION.MON or td.conditionType == const.TASK_CONDITION.MAPMON then
					player:set_task_param(task_id,1,0);
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
					player:push_task_data(task_id,0);
					
					local openPanel = false
					if td.tid then
						openPanel = gui.moduleGuide.checkOpenPanel(player, td.tid);
					end
					if openPanel then
						ret=nil;
					else
						if task_data.autogo and (not td.noautogo) then
							if task_id == const.TASK_ID_MAIN then
								posmon.go(player,td.checkID);
							-- else
							-- 	posmon.go(player,td.teleportID,td.checkID);
							end
							ret=nil;
						end
					end
					if td.needFly then
						player:push_lua_table("openTips",util.encode({name = "taskFly", time = 3, flyId = posnpc.getFlyId(td.doner)}))
					end
				elseif td.conditionType == const.TASK_CONDITION.NPCTALK then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					--print("////////////////////////////", td.teleportID);
					-- 主线三个假副本之后的回收任务
					local openPanel = false
					if td.tid then
						openPanel = gui.moduleGuide.checkOpenPanel(player, td.tid);
					end
					--print("111111111, openPanel", openPanel);
					if openPanel then
						ret=nil;
					else
						if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
					end
					if td.needFly then
						player:push_lua_table("openTips",util.encode({name = "taskFly", time = 3, flyId = posnpc.getFlyId(td.doner)}))
					end

				elseif td.conditionType == const.TASK_CONDITION.RICHANG then
					player:set_task_param(task_id,1,0);
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
					player:push_task_data(task_id,0);
					if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);ret=nil;end
				elseif td.conditionType == const.TASK_CONDITION.JOINGUILD then
					player:set_task_param(task_id,1,0);
					local title = player:get_guild_title();
					-- 有帮会情况下直接完成
					if title >= 102 then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
					else
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));ret=nil;
						if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
					end
				elseif td.conditionType == const.TASK_CONDITION.JOINGROUP then
					player:set_task_param(task_id,1,0);
					local groupId = player:get_group_id();
					-- 有队伍情况下直接完成
					if groupId > 0 then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
					else
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));ret=nil;
						if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
					end
				elseif td.conditionType == const.TASK_CONDITION.ADDFRIEND then
					player:set_task_param(task_id,1,0);
					-- print(task_id, td.tid, const.TSACED, task.util.ts(td.tid,const.TSACED))
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));ret=nil;
					if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
				elseif td.conditionType == const.TASK_CONDITION.ENTERMAP then
					player:set_task_param(task_id,1,0);
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));ret=nil;
					if td.checkID == "2clfb" then
						player:push_lua_table("open",util.encode({name = "panel_cailiao",tab = 2}))
					end
					if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
				elseif td.conditionType == const.TASK_CONDITION.GETEXPLOIT then
					player:set_task_param(task_id,1,0);
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));ret=nil;
					if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
				else -- 未定义类型，接取任务即完成
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
				end

				--任务tips
				if td.acceptTips and td.acceptTips ~= "" then
					player:push_lua_table("showTaskTips",util.encode({tips = td.acceptTips}));
				end
			else
				if td.need_type == "item" then
					local num = player:num_item(td.need_item);
					player:set_task_param( task_id,1,math.min(num,td.need_num) );
					if num >= td.need_num then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
					else
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
						if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
					end
				elseif td.need_type == "panel" or td.need_type == "daily" or td.need_type == "fuben" or td.need_type == "equip" then
					player:set_task_param(task_id,1,0);
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
					player:push_task_data(task_id,0);
					if task_data.autogo and (not td.noautogo) then
						if td.need_type == "fuben" then
							pospanel.enter(player,td.need_target)
							ret=nil
						elseif td.need_type == "equip" then
							posnpc.go(player, td.doner)
							ret=nil
						else
							pospanel.go(player,td.need_target)
							ret=nil
						end
					end
				elseif td.need_type == "elitemon" then
					player:set_task_param(task_id,1,0);
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
					player:push_task_data(task_id,0);
					if task_data.autogo and (not td.noautogo) then
						posnpc.go(player, td.doner)
						ret=nil
					end				
				else
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
				end
			end
			player:push_task_data(task_id,0);
			if td.accepted_hook then td.accepted_hook(npc,player,td);end;
			if ret then return ret;end
		end,
	};
	if isNewVersion(td) then
		if td.dialog then task_state_data[task.util.ts(td.tid,const.TSACCE)].talk = td.dialog;end;
	else
		if td.accept_talk then task_state_data[task.util.ts(td.tid,const.TSACCE)].talk = td.accept_talk;end;
	end
	task_state_data[task.util.ts(td.tid,const.TSACED)]={
		on_item_change=function (player,item_name,item_id)
			if isNewVersion(td) then
				if td.conditionType == const.TASK_CONDITION.OWNEQUIP then
					if util.checkEquipNeedLevel(item_id, player:get_job_name(), td.checkID) then
						local count = get_equip_count(player, td.checkID, td.checkCount);
						player:set_task_param(task_id,1,math.min(count, td.checkCount) );
						if count >= td.checkCount then
							player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
							-- 特殊处理，卡装备任务完成后不自动找npc
							-- if task_data.autogo then posnpc.go(player,td.doner);ret=nil;end
						else
							player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
							if task_data.autogo and (not td.noautogo) then posmon.go(player,td.need_target);ret=nil;end
						end
						player:push_task_data(task_id,0);
					end
				end
			else
				if td.need_type == "item" then
					local num = player:num_item(td.need_item);
					player:set_task_param( task_id,1,math.min(num,td.need_num) );
					if num >= td.need_num then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if td.complate_hook then td.complate_hook(npc,player,td);end;
						if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
						-- if td.focus then focus.f(player,"mini_task");end
					end
					player:push_task_data(task_id,0);
				end
			end
		end,

		on_mon_kill=function (mon,player,group)
			local killRight = false
			if td.conditionType == const.TASK_CONDITION.MON then --击杀怪物 
				if group and td.no_group then
					return;
				end
				if td.checkID > 0 and mon:get_def_id() == td.checkID then
					killRight = true
				elseif td.checkID == 0 then
					killRight = true
				end
			elseif td.conditionType == const.TASK_CONDITION.MAPMON then -- 指定地图击杀怪物 

				if group then
					if not PLATFORM_BANSHU then
						if not (task_id == const.TASK_ID_RICHANG and util.ppn(player, const.PP_MAIN_RICHANG) == 1 and checkMainRiChang(player)) then
							if td.no_group then return; end
						end
					else
						if td.no_group then return; end
					end
				end
				
				local map = mon:get_map();
				if td.checkID == map:get_def_id() then
					killRight = true;
				end
			end
			if killRight then
				player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);

				-- 第一次主线日常除魔特殊处理
				if PLATFORM_BANSHU then
					if task_id == const.TASK_ID_RICHANG and util.ppn(player, const.PP_MAIN_RICHANG) == 1 and checkMainRiChang(player) then
						player:set_task_param(task_id,1, td.checkCount);
						player:set_param(const.PP_MAIN_RICHANG, 2);
					end
				end

				if td.kill_talk then td.kill_talk(player,td) end
				local hideFly = false;
				if player:get_task_param(task_id,1) >= td.checkCount then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					if td.killmon_complate_hook then td.killmon_complate_hook(npc,player,td);end;
					if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
					player:refresh_npc_show_flags_inview();
					if task_id == const.TASK_ID_RICHANG then
						hideFly = true;
					end
					-- if td.focus then focus.f(player,"mini_task");end
				end
				player:push_task_data(task_id,0);
				if hideFly then player:push_lua_table("hideFly",""); end
			end
		end,

		on_mon_kill_all=function (mon,player,group)
			if td.conditionType == const.TASK_CONDITION.MON then
			-- if td.need_type == "mon" then
				if group and td.no_group then
					return;
				end
				local changed=false;
				if td.need_target_lv then
					if mon:get_level() >= td.need_target_lv then
						player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
						changed = true;
					end
				else
					if td.need_map and nil ~= string.find(mon:get_map():get_id(),td.need_map) then
						player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
						changed = true;
					end
				end
				if changed then
					if player:get_task_param(task_id,1) >= td.need_num then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if td.complate_hook then td.complate_hook(npc,player,td);end; 
						if td.killmon_complate_hook then td.killmon_complate_hook(npc,player,td);end;
						if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
						player:refresh_npc_show_flags_inview();						
						-- if td.focus then focus.f(player,"mini_task");end
					end
					player:push_task_data(task_id,0);
				end
			end
		end,

		on_level_change=function (player,lv)
			if td.conditionType == const.TASK_CONDITION.LEVELUP then --升级
			-- if td.need_type == "level" then
				player:set_task_param(task_id,1,math.min(lv, td.checkID));
				if lv >= td.checkID then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
					-- if td.focus then focus.f(player,"mini_task");end
				end
				player:push_task_data(task_id,0);
			end
		end,

		on_zslevel_change=function (player, zslv)
			if td.need_type == "zslevel" then
				player:set_task_param(task_id,1,math.min(zslv, td.need_num) );
				if zslv >= td.need_num then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
					-- if td.focus then focus.f(player,"mini_task");end
				end
				player:push_task_data(task_id,0);
			end
		end,

		on_inner_power_level_change=function (player,lv)
			if td.conditionType == const.TASK_CONDITION.INNERPOWERLEVELUP then --升级
			-- if td.need_type == "level" then
				player:set_task_param(task_id,1,math.min(lv, td.checkID));
				if lv >= td.checkID then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
					-- if td.focus then focus.f(player,"mini_task");end
				end
				player:push_task_data(task_id,0);
			end
		end,

		on_enter_copy=function (player, map)
			if td.conditionType == const.TASK_CONDITION.COPY then
				local mapid = map:get_def_id();
				-- print("on_enter_copy", td.checkID, mapid, td.checkID == mapid)
				if td.checkID == mapid then
					-- player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
					-- if td.kill_talk then td.kill_talk(player,td) end
					-- if player:get_task_param(task_id,1) >= td.checkCount then

					-- 	player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						
					-- 	if td.complate_hook then td.complate_hook(npc,player,td);end;
					-- 	if td.killmon_complate_hook then td.killmon_complate_hook(npc,player,td);end;
					-- 	-- if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
					-- 	player:refresh_npc_show_flags_inview();
					-- 	-- if td.focus then focus.f(player,"mini_task");end
					-- 	-- show_task_short_desp(player, player, task_id, data, td)
					-- 	-- print("*********************")
					-- end
					-- player:push_task_data(task_id,0);

					-- 假任务描述
					-- show_task_short_desp(player, player, task_id, data, td)
				end
			end
		end,

		on_leave_copy=function (player, map)
			if td.conditionType == const.TASK_CONDITION.COPY then
				local mapid = map:get_def_id();
				-- print("on_enter_copy", td.checkID, mapid, td.checkID == mapid)
				if td.checkID == mapid then
					player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
					if player:get_task_param(task_id,1) >= td.checkCount then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if td.complate_hook then td.complate_hook(npc,player,td);end;
						player:refresh_npc_show_flags_inview();
						if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
					end
					player:push_task_data(task_id,0);
				end
			end
		end,

		on_enter_map=function (player, map)
			if td.conditionType == const.TASK_CONDITION.ENTERMAP then
				local mapid = map:get_def_id();
				-- print("on_enter_map", td.checkID, mapid, td.checkID == mapid)
				if td.checkID == mapid then
					player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
					if player:get_task_param(task_id,1) >= td.checkCount then
						player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
						if td.complate_hook then td.complate_hook(npc,player,td);end;
						player:refresh_npc_show_flags_inview();
						if task_data.autogo and (not td.noautogo) then posnpc.go(player,td.doner);end
					end
					player:push_task_data(task_id,0);
				end
			end
		end,

		on_join_guild=function (player)
			if td.conditionType == const.TASK_CONDITION.JOINGUILD then
				player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
				if player:get_task_param(task_id,1) >= td.checkCount then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					player:refresh_npc_show_flags_inview();
					if task_data.autogo and (not td.noautogo) then 
						posnpc.go(player,td.doner);
						player:push_lua_table("showFly",util.encode({flyInfo = posnpc.getFlyId(td.doner)}));
					end
				end
				player:push_task_data(task_id,0);
			end
		end,

		on_join_group=function (player)
			if td.conditionType == const.TASK_CONDITION.JOINGROUP then
				player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
				if player:get_task_param(task_id,1) >= td.checkCount then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					player:refresh_npc_show_flags_inview();
					if task_data.autogo and (not td.noautogo) then 
						posnpc.go(player,td.doner);
						player:push_lua_table("showFly",util.encode({flyInfo = posnpc.getFlyId(td.doner)}));
					end
				end
				player:push_task_data(task_id,0);
			end
		end,

		on_add_friend=function (player)
			-- print("on_add_friend", td.conditionType, const.TASK_CONDITION.ADDFRIEND)
			if td.conditionType == const.TASK_CONDITION.ADDFRIEND then
				player:set_task_param(task_id,1,player:get_task_param(task_id,1)+1);
				--刷新任务描述
				player:refresh_task_name(task_id);
				if player:get_task_param(task_id,1) >= td.checkCount then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					player:refresh_npc_show_flags_inview();
					if task_data.autogo and (not td.noautogo) then 
						posnpc.go(player,td.doner);
						player:push_lua_table("showFly",util.encode({flyInfo = posnpc.getFlyId(td.doner)}));
					end
				end
				player:push_task_data(task_id,0);
			end
		end,

		on_get_exploit=function (player, exploit)
			if td.conditionType == const.TASK_CONDITION.GETEXPLOIT then
				player:set_task_param(task_id,1, player:get_task_param(task_id,1) + exploit);
				if player:get_task_param(task_id,1) > td.checkCount then player:set_task_param(task_id,1, td.checkCount) end
				--刷新任务描述
				player:refresh_task_name(task_id);
				if player:get_task_param(task_id,1) >= td.checkCount then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSCOMP));
					if td.complate_hook then td.complate_hook(npc,player,td);end;
					player:refresh_npc_show_flags_inview();
					if task_data.autogo and (not td.noautogo) then 
						posnpc.go(player,td.doner);
						player:push_lua_table("showFly",util.encode({flyInfo = posnpc.getFlyId(td.doner)}));
					end
				end
				player:push_task_data(task_id,0);
			end
		end,
	};
	task_state_data[task.util.ts(td.tid,const.TSCOMP)]={
		shortdesp="找"..posnpc.mp(td.doner),
		on_item_change=function (player,item_name,item_id)
			if td.need_type == "item" then
				local num = player:num_item(td.need_item);
				player:set_task_param( task_id,1,math.min(num,td.need_num) );
				if num < td.need_num then
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
					if task_data.autogo then posmon.go(player,td.need_target);ret=nil;end
				end
				player:push_task_data(task_id,0);
			end
		end,
		done=function (npc,player)
			-- print("task complete done")
			local ret=100;
			if td.need_type == "item" then
				local num = player:num_item(td.need_item);
				if num < td.need_num then
					player:set_task_param( task_id,1,math.min(num,td.need_num) );
					player:set_task_state(task_id,task.util.ts(td.tid,const.TSACED));
					player:push_task_data(task_id,0);
					if task_data.autogo then posmon.go(player,td.need_target);ret=nil;end
					return;
				else
					player:remove_item(td.need_item,td.need_num);
				end
			end
			if not canTaskDone(player, td) then
				player:alert(1, 1, "背包空间不足,请清理后再来领取奖励!");
				return;
			end

			player:push_lua_table("effect_one",util.encode({effect_type=60071}))
			cktask.triger_task(player, task_id, b)

			task.util.give_award(player,td);
			if td.nid and td.nid > 0 then
				player:set_task_state(task_id,task.util.ts(td.nid,const.TSUNAC));ret=nil;
			else
				-- print("1111111", td.tid, const.TSPASS, td.tid,const.TSPASS)
				player:set_task_state(task_id,task.util.ts(td.tid,const.TSPASS));ret=nil;
			end
			if td.nid > 0 and task_data[td.nid] then
				if player:get_level() >= task_data[td.nid].need_level then
					player:set_task_state(task_id,task.util.ts(td.nid,const.TSACCE));
					-- if (not td.direct_next) and npc:get_name()==task_data[td.nid].accepter then
					if npc:get_def_id()==task_data[td.nid].accepter then
						-- Was the modify no effect??
						task.util.task_done(npc,player,task_id,task_state_data,task_data);ret=nil;
						-- task.util.show_main_task_talk(npc,player,task_id,task_state_data,task_data);ret=nil;
						-- task.util.show_task_talk(npc,player,task_id,task_state_data,task_data);ret=nil;
					else
						if task_data.autogo then posnpc.go(player,task_data[td.nid].accepter);ret=nil;end
					end
				end
			end
			player:push_task_data(task_id,0);
			if td.done_hook then td.done_hook(npc,player,td);ret=nil;end;
			if td.complate_hook then td.complate_hook(npc,player,td);end;
			
			if ret then return ret;end
		end,
	};
	if isNewVersion(td) then
		if td.enddialog then task_state_data[task.util.ts(td.tid,const.TSCOMP)].talk = td.enddialog;end;
	else
		if td.done_talk then task_state_data[task.util.ts(td.tid,const.TSCOMP)].talk = td.done_talk;end;
	end
end

--点击任务栏回调(task_target类型为click_xxx)
local taskOpenPanel = {
	["guild"] = {"main_guild"},
	["group"] = {"main_group"},
	["friend"] = {"main_friend"},
	["exploit"] = {"extend_exploit"},
	["innerpower"] = {"main_avatar",3},
}

function check_client_talk(player, paramstr)
	if util.isString(paramstr) then
		local param=string.split(paramstr,"|");
		if not param  then return; end
		if not (#param > 0) then return; end
		if #param == 3 then
			if param[1] == "goto" then
				if param[2] == "npc" then
					local npcId = tonumber(param[3]);
					--玛雅神殿
					if npcId == 4002024 or npcId == 4002011 then
						local map = player:get_map();
						if not map then return; end
						local mapid = map:get_def_id();
						mapid = string.sub(mapid, 1, 4);
						if mapid ~= "mysd" then
							posnpc.go(player, npcId);
							player:push_lua_table("showFly",util.encode({flyInfo = posnpc.getFlyId(npcId)}));
						else
							player:push_lua_table("hideFly","")
						end

					end
				end
			end
		elseif #param == 2 then
			if param[1] == "open" then
				if taskOpenPanel[param[2]] then
					if #taskOpenPanel[param[2]] > 1 then
						player:push_lua_table("open",util.encode({name = taskOpenPanel[param[2]][1],tab = taskOpenPanel[param[2]][2]}))
					else
						player:push_lua_table("open",util.encode({name = taskOpenPanel[param[2]][1]}))
					end
				end
			end
		end
	end
end

cktalk.add_all_listener(check_client_talk)

