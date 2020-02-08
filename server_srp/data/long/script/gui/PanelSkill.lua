module(... ,package.seeall)

local jobSKills = {
	["warrior"] = {
		100,101,102,103,104,105,106,107,109,
	},

	["wizard"] = {
		401,402,403,404,405,406,407,408,409,410,411,412,413,414,416,417,
	},

	["taoist"] = {
		501,502,503,504,505,506,507,508,509,510,511,512,513,514,518,
	}
}

-- 
local jobSKills2 = {
	["warrior"] = {
		103, 104, 105, 106, 109
	},

	["wizard"] = {
		402, 405, 409, 412, 414
	},

	["taoist"] = {
		503, 504, 508, 512, 513
	}
}

local previewSkills = {
	["warrior"] = {
		103, 104, 105, 106, 109
	},

	["wizard"] = {
		405, 412, 411, 409, 402, 414
	},

	["taoist"] = {
		504, 513, 508, 503, 512, 507
	}
}

local SKILL_LEVEL_MAX = 10
local SKILL_LEVEL_EXT_MAX = 11

-- 互斥
-- 414 冰咆哮
-- 411 雷光

local function checkPlayerSkill(player, skillType)
	if player and skillType then
		local job = player:get_job_name()
		if jobSKills[job] and table.indexof(jobSKills[job], skillType) then
			return true
		end
	end
end

local function getAutoCastSkills(player)
	local autoSkills = {}
	local param = player:get_param(const.PP_AUTO_CAST_SKILLS)
	if param and param ~= "" then
		autoSkills = util.decode(param)
	end
	return autoSkills
end

function onPanelData(player, commond)
	local param = util.decode(commond)
	if param and param.skillType and checkPlayerSkill(player, param.skillType) then
		if param.cmd == "levelUp" then
			checkSkillLevelUp(player, param.skillType,param.skillLevel);
		elseif param.cmd == "autoCast" then
			setAutoCastSkills(player, param.skillType, param.state);
			if param.state == true then
				if param.skillType == 411 then
					setAutoCastSkills(player, 414, false);
				elseif param.skillType == 414 then
					setAutoCastSkills(player, 411, false);
				elseif param.skillType == 109 then
					setAutoCastSkills(player, 106, false);
				elseif param.skillType == 106 then
					setAutoCastSkills(player, 109, false);
				end
			end
		end
	elseif param.cmd == "skillPreview" then
		local job = player:get_job_name();
		local skills = previewSkills[job];
		local skillsPreview = {};

		local nsd;
		for _,v in ipairs(skills) do
			nsd = tab.getSkillDefWithLv(v, 1);
			if nsd then
				table.insert(skillsPreview, {name = nsd.name, skill_id = math.floor(nsd.skill_id / 100), mEffectResID = nsd.res_id, desp = nsd.desp, level = 1});
			end
			nsd = tab.getSkillDefWithLv(v, 11);
			if nsd then
				table.insert(skillsPreview, {name = nsd.name, skill_id = math.floor(nsd.skill_id /100), mEffectResID = nsd.res_id, desp = nsd.desp, level = 11, angrySkill = true});
			end
		end
		player:push_lua_table("PanelSkill", util.encode({cmd = "preview", skills = skillsPreview}));
	end
end

local ext_skill_up_need = {
	-- [101]=26010002,
	[103]={item=26010012,name="怒之刺杀剑法"},
	[104]={item=26010022,name="怒之半月弯刀"},
	[105]={item=26010032,name="怒之野蛮冲撞"},
	[106]={item=26010042,name="怒之烈火剑法"},
	[109]={item=26010052,name="怒之逐日剑法"},
	[405]={item=26020002,name="怒之雷电术"},
	[412]={item=26020012,name="怒之魔法盾"},
	[411]={item=26020022,name="怒之地狱雷光"},
	[409]={item=26020032,name="怒之火墙"},
	[402]={item=26020042,name="怒之抗拒火环"},
	[414]={item=26020052,name="怒之冰咆哮"},
	[504]={item=26030002,name="怒之灵魂火符"},
	[513]={item=26030012,name="怒之召唤虎卫"},
	[508]={item=26030022,name="怒之圣甲术"},
	[503]={item=26030032,name="怒之群体施毒术"},
	[512]={item=26030042,name="怒之群体治疗术"},
	[507]={item=26030052,name="怒之群体隐身术"},
	-- [502]=26030062,
}

local function getSkillMaxLevel(skillType)
	-- body
end

---------技能升级
function checkSkillLevelUp(player, skillType, skillLevel)
	-- print("checkSkillLevelUp", skillType);
	-- if skillLevel>=SKILL_LEVEL_EXT_MAX then
	-- 	player:alert(1,1, "技能已达最大等级！");
	-- 	return
	-- end
	-- if skillLevel >= SKILL_LEVEL_MAX and not ext_skill_up_need[skillType] then
	-- 	player:alert(1,1, "技能已达最大等级！");
	-- 	return
	-- end
	if not skillType then return; end
	local maxLevel = tab.getSKillMaxLevel(skillType);
	if skillLevel >= maxLevel then return player:alert(1, 1, "技能已达最大等级！"); end

	if skillLevel == SKILL_LEVEL_MAX then
		if player:num_item(ext_skill_up_need[skillType].item,1)>0 then
			if player:check_skill_level_up(skillType) > 0 then
				player:remove_item(ext_skill_up_need[skillType].item,1)
				player:alert(1,1, "恭喜您，成功升级"..ext_skill_up_need[skillType].name.."！");
				gui.moduleRedPoint.checkSkillUsable(player)
			end
		else
			player:alert(1,1, "缺少"..ext_skill_up_need[skillType].name.."技能书，无法升级");
		end
	else
		if player:check_skill_level_up(skillType) > 0 then
			player:alert(1,1, "恭喜您，技能升级成功！");
			gui.moduleRedPoint.checkSkillUsable(player)
		end 
	end
end

---------设置技能自动释放
function setAutoCastSkills(player, skillType, state)
	local autoSkills = getAutoCastSkills(player);
	local change;
	if state and not table.indexof(autoSkills, skillType) then
		table.insert(autoSkills, skillType)
		change = true
	elseif not state and table.indexof(autoSkills, skillType) then
		table.removebyvalue(autoSkills, skillType)
		change = true
	end
	-- print("setAutoCastSkills", change, skillType, state, util.encode(autoSkills))
	if change then
		player:set_param(const.PP_AUTO_CAST_SKILLS, util.encode(autoSkills))
		-- pushAutoCastSkills(player)
		player:push_lua_table("switchAutoCast", util.encode({skillType = skillType, state = state}))
	end
end

ckpanel.add_listener("gui.PanelSkill.onPanelData", gui.PanelSkill.onPanelData)

function pushAutoCastSkills(player)
	player:push_lua_table("autoCast", player:get_param(const.PP_AUTO_CAST_SKILLS))
end

function pushSkillDesp(player)
	local job = player:get_job_name()
	if jobSKills[job] then
		for i,v in ipairs(jobSKills[job]) do
			player:push_skill_def(v, 1);
		end
	end
end

--技能展示
function pushAllSkillDesp(player)
	local job = player:get_job_name()
	if jobSKills2[job] then
		for i,v in ipairs(jobSKills2[job]) do
			player:push_skill_def(v, 1)
			player:push_skill_def(v, 11)
		end
	end
end

function setDefaultAutoUse(player)
	local job = player:get_job_name()
	local skills = util.clone(jobSKills[job])
	if job == "warrior" then
		table.removebyvalue(skills, 109, true);
	elseif job == "wizard" then
		table.removebyvalue(skills, 414, true);
	end
	player:set_param(const.PP_AUTO_CAST_SKILLS, util.encode(skills))
end

login.add_first_login_listener(setDefaultAutoUse);
login.add_login_listener(pushAutoCastSkills);
-- login.add_login_listener(pushAllSkillDesp);

--检测小红点
function checkRedPoint(player)
	local job = player:get_job_name();
	local skills = previewSkills[job];
	local skillsPreview = {};
	local nsd;
	local is_red = false
	for _,v in ipairs(skills) do
		local skillLevel = player:skill_level(v)
		local maxLevel = tab.getSKillMaxLevel(v)
		if skillLevel < maxLevel then
			if skillLevel == SKILL_LEVEL_MAX then
				if player:num_item(ext_skill_up_need[v].item,1)>0 then
					if player:check_skill_can_level_up(v) > 0 then
						is_red=true
					end
				end
			else
				if player:check_skill_can_level_up(v) > 0 then  --check_skill_can_level_up方法为空默认传了0， 小bug需要修改
					is_red=true
				end
			end
		end
	end 
	return is_red
end