module(..., package.seeall)

local BOOK_EXP = 3000;
local BOOK_EXT_EXP = 10000;
local SKILL_LEVEL_MAX = 10

function checkJob(player,needJob)
	if( player:get_job_name() == needJob ) then
		return true;
	end
	player:alert(1,0,"您的职业不可以学习该技能");
	return false;
end
function YiBanGongJi(player)
	player:add_skill(100);
end

--战士技能
function JiChuJianShu(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(101) == 0 then
		player:add_skill(101);
		-- player:alert(10,1,"恭喜您成功修炼[基础剑法]");
		-- if player:get_task_state(100) == 19 then
		-- 	player:set_task_state(100,20);
		-- 	player:push_task_data(100,1);
		-- 	player:refresh_npc_show_flags_inview();
		-- end
	else
		player:add_skill_exp(101, BOOK_EXP);
		player:alert(1,1,"技能书使用成功,基础剑法获得"..BOOK_EXP.."技能熟练度");
	end
end
-- function GongShaJianShu(player)
-- 	if( not checkJob(player,"warrior") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(102) == 0 then
-- 		player:alert(10,1,"恭喜您成功修炼[强攻剑术]");
-- 		player:add_skill(102);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
function CiShaJianShu(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(103) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[刺杀剑法]");
		player:add_skill(103);
	else
		player:add_skill_exp(103, BOOK_EXP);
		if player:skill_level(103) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之刺杀剑法获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,刺杀剑法获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function CiShaJianShuExt(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(103) > SKILL_LEVEL_MAX then
		player:add_skill_exp(103, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之刺杀剑法获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之刺杀剑法,无法使用");
		return 0;
	end
end
function BanYueWanDao(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(104) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[半月弯刀]");
		player:add_skill(104);
	else
		player:add_skill_exp(104, BOOK_EXP);
		if player:skill_level(104) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之半月弯刀获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,半月弯刀获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function BanYueWanDaoExt(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(104) > SKILL_LEVEL_MAX then
		player:add_skill_exp(104, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之半月弯刀获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之半月弯刀,无法使用");
		return 0;
	end
end
function YeManChongZhuang(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(105) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[野蛮冲撞]");
		player:add_skill(105);
	else
		player:add_skill_exp(105, BOOK_EXP);
		if player:skill_level(105) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之野蛮冲撞获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,野蛮冲撞获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function YeManChongZhuangExt(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(105) > SKILL_LEVEL_MAX then
		player:add_skill_exp(105, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之野蛮冲撞获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之野蛮冲撞,无法使用");
		return 0;
	end
end
function LieHuoJianFa(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(106) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[烈火剑法]");
		player:add_skill(106);
	else
		player:add_skill_exp(106, BOOK_EXP);
		if player:skill_level(106) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之烈火剑法获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,烈火剑法获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function LieHuoJianFaExt(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(106) > SKILL_LEVEL_MAX then
		player:add_skill_exp(106, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之烈火剑法获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之烈火剑法,无法使用");
		return 0;
	end
end
function ZhuRiJianFa(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(109) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[逐日剑法]");
		player:add_skill(109);

		-- 关闭烈火的自动释放
		-- gui.PanelSkill.setAutoCastSkills(player, 106, false);
	else
		player:add_skill_exp(109, BOOK_EXP);
		if player:skill_level(109) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之逐日剑法获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,逐日剑法获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function ZhuRiJianFaExt(player)
	if( not checkJob(player,"warrior") ) then
		return 0;
	end
	if player:skill_level(109) > SKILL_LEVEL_MAX then
		player:add_skill_exp(109, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之逐日剑法获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之逐日剑法,无法使用");
		return 0;
	end
end

--法师技能
function LeiDianShu(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(405) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[雷电术]");
		player:add_skill(405);
	else
		player:add_skill_exp(405, BOOK_EXP);
		if player:skill_level(405) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之雷电术获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,雷电术获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function ZLeiDianShuExt(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(405) > SKILL_LEVEL_MAX then
		player:add_skill_exp(405, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之雷电术获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之雷电术,无法使用");
		return 0;
	end
end
function MoFaDun(player)
	if( not checkJob(player,"wizard") ) then
	return 0;
	end
	if player:skill_level(412) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[魔法盾]");
		player:add_skill(412);
	else
		player:add_skill_exp(412, BOOK_EXP);
		if player:skill_level(412) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之魔法盾获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,魔法盾获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function MoFaDunExt(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(412) > SKILL_LEVEL_MAX then
		player:add_skill_exp(412, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之魔法盾获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之魔法盾,无法使用");
		return 0;
	end
end
function DiYuLeiGuang(player)
	if( not checkJob(player,"wizard") ) then
	return 0;
	end
	if player:skill_level(411) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[地狱雷光]");
		player:add_skill(411);
	else
		player:add_skill_exp(411, BOOK_EXP);
		if player:skill_level(411) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之地狱雷光获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,地狱雷光获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function DiYuLeiGuangExt(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(411) > SKILL_LEVEL_MAX then
		player:add_skill_exp(411, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之地狱雷光获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之地狱雷光,无法使用");
		return 0;
	end
end
-- function HuoQiuShu(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(401) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[火焰之球]");
-- 		player:add_skill(401);
-- 		-- if player:get_task_state(100) == 19 then
-- 		-- 	player:set_task_state(100,20);
-- 		-- 	player:push_task_data(100,1);
-- 		-- 	player:refresh_npc_show_flags_inview();
-- 		-- end
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end

function KangJuHuoHuan(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(402) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[抗拒火环]");
		player:add_skill(402);
	else
		player:add_skill_exp(402, BOOK_EXP);
		if player:skill_level(402) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之抗拒火环获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,抗拒火环获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function KangJuHuoHuanExt(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(402) > SKILL_LEVEL_MAX then
		player:add_skill_exp(402, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之抗拒火环获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之抗拒火环,无法使用");
		return 0;
	end
end
-- function YouHuoZhiGuang(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(403) == 0 then
-- 		player:add_skill(403);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end

-- function DiYuHuo(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(404) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[黑狱之火]");
-- 		player:add_skill(404);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end

-- function ShunJianYiDong(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(406) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[空间穿梭]");
-- 		player:add_skill(406);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
-- function DaHuoQiu(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(407) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[火焰飞轮]");
-- 		player:add_skill(407);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
-- function BaoLieHuoYan(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(408) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[爆焰火球]");
-- 		player:add_skill(408);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
function HuoQiang(player)
	if( not checkJob(player,"wizard") ) then
	return 0;
	end
	if player:skill_level(409) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[火墙]");
		player:add_skill(409);
	else
		player:add_skill_exp(409, BOOK_EXP);
		if player:skill_level(409) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之火墙获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,火墙获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function HuoQiangExt(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(409) > SKILL_LEVEL_MAX then
		player:add_skill_exp(409, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之火墙获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之火墙,无法使用");
		return 0;
	end
end
-- function JiGuangDianYing(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(410) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[黑狱雷电]");
-- 		player:add_skill(410);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
-- function ShengYanShu(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		-- player:alert(10,1,"恭喜您成功修炼[魔神指引]");
-- 	return 0;
-- 	end
-- 	if player:skill_level(413) == 0 then
-- 		player:add_skill(413);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
function BingPaoXiao(player)
	if( not checkJob(player,"wizard") ) then
	return 0;
	end
	if player:skill_level(414) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[冰咆哮]");
		player:add_skill(414);
		-- 关闭地狱雷光的自动释放
		-- gui.PanelSkill.setAutoCastSkills(player, 411, false);
	else
		player:add_skill_exp(414, BOOK_EXP);
		if player:skill_level(414) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之冰咆哮获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,冰咆哮获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function BingPaoXiaoExt(player)
	if( not checkJob(player,"wizard") ) then
		return 0;
	end
	if player:skill_level(414) > SKILL_LEVEL_MAX then
		player:add_skill_exp(414, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之冰咆哮获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之冰咆哮,无法使用");
		return 0;
	end
end

--道士技能
-- function ZhiYuShu(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(501) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[回春之术]");
-- 		player:add_skill(501);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
function JinShenLiZhanFa(player)
	if( not checkJob(player,"taoist") ) then
	return 0;
	end
	if player:skill_level(502) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[精神力战法]");
		player:add_skill(502);
	else
		player:add_skill_exp(502, BOOK_EXP);
		player:alert(1,1,"技能书使用成功,精神力战法获得"..BOOK_EXP.."技能熟练度");
	end
end
function ShiDuShu(player)
	if( not checkJob(player,"taoist") ) then
	return 0;
	end
	if player:skill_level(503) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[群体施毒术]");
		player:add_skill(503);
	else
		player:add_skill_exp(503, BOOK_EXP);
		if player:skill_level(503) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之群体施毒术获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,群体施毒术获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function ShiDuShuExt(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(503) > SKILL_LEVEL_MAX then
		player:add_skill_exp(503, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之群体施毒术获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之群体施毒术,无法使用");
		return 0;
	end
end
function LingHunHuoFu(player)
	if( not checkJob(player,"taoist") ) then
	return 0;
	end
	if player:skill_level(504) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[灵魂火符]");
		player:add_skill(504);
		-- if player:get_task_state(100) == 19 then
		-- 	player:set_task_state(100,20);
		-- 	player:push_task_data(100,1);
		-- 	player:refresh_npc_show_flags_inview();
		-- end
	else
		player:add_skill_exp(504, BOOK_EXP);
		if player:skill_level(504) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之灵魂火符获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,灵魂火符获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function LingHunHuoFuExt(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(504) > SKILL_LEVEL_MAX then
		player:add_skill_exp(504, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之灵魂火符获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之灵魂火符,无法使用");
		return 0;
	end
end
-- function ZhaoHuanKuLou(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(505) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[亡灵召唤]");
-- 		player:add_skill(505);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
-- function YinShenShu(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(506) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[隐身之术]");
-- 		player:add_skill(506);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
function JiTiYinShenShu(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(507) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[群体隐身术]");
		player:add_skill(507);
	else
		player:add_skill_exp(507, BOOK_EXP);
		if player:skill_level(507) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之群体隐身术获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,群体隐身术获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function JiTiYinShenShuExt(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(507) > SKILL_LEVEL_MAX then
		player:add_skill_exp(507, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之群体隐身术获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之群体隐身术,无法使用");
		return 0;
	end
end
function YouLingDun(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(508) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[圣甲术]");
		player:add_skill(508);
	else
		-- player:alert(1,1,"您已经学习了该技能,无法再次学习")
		player:add_skill_exp(508, BOOK_EXP);
		if player:skill_level(508) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之圣甲术获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,圣甲术获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function YouLingDunExt(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(508) > SKILL_LEVEL_MAX then
		player:add_skill_exp(508, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之圣甲术获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之圣甲术,无法使用");
		return 0;
	end
end
-- function ShenShengZhanJiaShu(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(509) == 0 then
-- 		-- player:alert(10,1,"恭喜您成功修炼[斗神庇护]");
-- 		player:add_skill(509);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
-- function XinLingQiShi(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(510) == 0 then
-- 		player:add_skill(510);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
-- function KunMoZhou(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 	return 0;
-- 	end
-- 	if player:skill_level(511) == 0 then
-- 		player:add_skill(511);
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end
function QunTiZhiLiao(player)
	if( not checkJob(player,"taoist") ) then
	return 0;
	end
	if player:skill_level(512) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[群体治疗术]");
		player:add_skill(512);
	else
		player:add_skill_exp(512, BOOK_EXP);
		if player:skill_level(512) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之群体治疗术获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,群体治疗术获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function QunTiZhiLiaoExt(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(512) > SKILL_LEVEL_MAX then
		player:add_skill_exp(512, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之群体治疗术获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之群体治疗术,无法使用");
		return 0;
	end
end
function ZhaoHuanShenShou(player)
	if( not checkJob(player,"taoist") ) then
	return 0;
	end
	if player:skill_level(513) == 0 then
		-- player:alert(10,1,"恭喜您成功修炼[召唤虎卫]");
		player:add_skill(513);
		player:use_skill(513);
	else
		player:add_skill_exp(513, BOOK_EXP);
		if player:skill_level(513) > SKILL_LEVEL_MAX then
			player:alert(1,1,"技能书使用成功,怒之召唤虎卫获得"..BOOK_EXP.."技能熟练度");
		else
			player:alert(1,1,"技能书使用成功,召唤神兽获得"..BOOK_EXP.."技能熟练度");
		end
	end
end
function ZhaoHuanShenShouExt(player)
	if( not checkJob(player,"taoist") ) then
		return 0;
	end
	if player:skill_level(513) > SKILL_LEVEL_MAX then
		player:add_skill_exp(513, BOOK_EXT_EXP);
		player:alert(1,1,"技能书使用成功,怒之召唤虎卫术获得"..BOOK_EXT_EXP.."技能熟练度");
	else
		player:alert(1, 1, "技能等级不足，当前技能10级后方可学习");
		--player:alert(1,1,"尚未升级怒之群体治疗术,无法使用");
		return 0;
	end
end
-- function skillpotian1(player)
-- 	if( not checkJob(player,"warrior") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(106) == 3 then
-- 		player:set_skill_level(106,0);
-- 		if player:skill_level(107) == 0 and player:skill_level(109) == 0 then
-- 			player:set_skill_level(109,0);
-- 			player:add_skill(107);
-- 			player:alert(10,1,"恭喜您成功修炼[破天斩(1级)]");
-- 		else
-- 			player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 			return 0;
-- 		end
-- 	else
-- 		player:alert(1,1,"修炼[破天斩(1级)]需要达到烈火剑法(3级)");
-- 		return 0;
-- 	end
-- end
----------------------------转生技能
-- function skillpotian2(player)
-- 	if( not checkJob(player,"warrior") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(107) == 1 then
-- 		player:set_skill_level(106,0);
-- 		player:set_skill_level(109,0);
-- 		player:set_skill_level(107,2);
-- 		-- player:alert(10,1,"恭喜您成功修炼[破天斩(2级)]");
-- 	else
-- 		player:alert(1,1,"修炼[破天斩(2级)]需要达到破天斩(1级)");
-- 		return 0;
-- 	end
-- end

-- function skillpotian3(player)
-- 	if( not checkJob(player,"warrior") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(107) == 2 then
-- 		player:set_skill_level(106,0);
-- 		player:set_skill_level(109,0);
-- 		player:set_skill_level(107,3);
-- 		-- player:alert(10,1,"恭喜您成功修炼[破天斩(3级)]");
-- 	else
-- 		player:alert(1,1,"修炼[破天斩(3级)]需要达到破天斩(2级)");
-- 		return 0;
-- 	end
-- end

-- function skillbenlei1(player)
-- 	if( not checkJob(player,"warrior") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(107) == 3 then
-- 		player:set_skill_level(107,0);
-- 		if player:skill_level(109) == 0 then
-- 			player:set_skill_level(106,0);
-- 			player:add_skill(109);
-- 			player:alert(10,1,"恭喜您成功修炼[奔雷剑法(1级)]");
-- 		else
-- 			player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 		return 0;
-- 		end
-- 	else
-- 		player:alert(1,1,"修炼[奔雷剑法(1级)]需要达到破天斩(3级)");
-- 		return 0;
-- 	end
-- end

-- function skillbenlei2(player)
-- 	if( not checkJob(player,"warrior") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(109) == 1 then
-- 		player:set_skill_level(106,0);
-- 		player:set_skill_level(107,0);
-- 		player:set_skill_level(109,2);
-- 		-- player:alert(10,1,"恭喜您成功修炼[奔雷剑法(2级)]");
-- 	else
-- 		player:alert(1,1,"修炼[奔雷剑法(2级)]需要达到奔雷剑法(1级)");
-- 		return 0;
-- 	end
-- end

-- function skillbenlei3(player)
-- 	if( not checkJob(player,"warrior") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(109) == 2 then
-- 		player:set_skill_level(106,0);
-- 		player:set_skill_level(107,0);
-- 		player:set_skill_level(109,3);
-- 		-- player:alert(10,1,"恭喜您成功修炼[奔雷剑法(3级)]");
-- 	else
-- 		player:alert(1,1,"修炼[奔雷剑法(3级)]需要达到奔雷剑法(2级)");
-- 		return 0;
-- 	end
-- end

-- function skillqiyan1(player)
-- 	if not checkJob(player,"wizard") then
-- 		return 0;
-- 	end
-- 	if player:skill_level(414) == 3 then
-- 		player:set_skill_level(414,0);
-- 		if player:skill_level(416) == 0 and player:skill_level(417) == 0  then
-- 			player:add_skill(416);
-- 			player:set_skill_level(417,0);
-- 			player:alert(10,1,"恭喜您成功修炼[龙息气焰(1级)]");
-- 		else
-- 			player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 		return 0;
-- 		end
-- 	else
-- 		player:alert(1,1,"修炼[龙息气焰(1级)]需要达到冰咆哮(3级)");
-- 		return 0;
-- 	end
-- end

-- function skillqiyan2(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(416) == 1 then
-- 		player:set_skill_level(414,0);
-- 		player:set_skill_level(417,0);
-- 		player:set_skill_level(416,2);
-- 		-- player:alert(10,1,"恭喜您成功修炼[龙息气焰(2级)]");
-- 	else
-- 		player:alert(1,1,"修炼[龙息气焰(2级)]需要达到龙息气焰(1级)");
-- 		return 0;
-- 	end
-- end

-- function skillqiyan3(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(416) == 2 then
-- 		player:set_skill_level(414,0);
-- 		player:set_skill_level(417,0);
-- 		player:set_skill_level(416,3);
-- 		-- player:alert(10,1,"恭喜您成功修炼[龙息气焰(3级)]");
-- 	else
-- 		player:alert(1,1,"修炼[龙息气焰(3级)]需要达到龙息气焰(2级)");
-- 		return 0;
-- 	end
-- end

-- function skillhuoyu1(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(416) == 3 then
-- 		player:set_skill_level(416,0);
-- 		if player:skill_level(417) == 0  then
-- 			player:add_skill(417);
-- 			player:set_skill_level(414,0);
-- 			player:alert(10,1,"恭喜您成功修炼[烈焰火雨(1级)]");
-- 		else
-- 			player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 		return 0;
-- 		end
-- 	else
-- 		player:alert(1,1,"修炼[烈焰火雨(1级)]需要达到龙息气焰(3级)");
-- 		return 0;
-- 	end
-- end

-- function skillhuoyu2(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(417) == 1 then
-- 		player:set_skill_level(414,0);
-- 		player:set_skill_level(416,0);
-- 		player:set_skill_level(417,2);
-- 		-- player:alert(10,1,"恭喜您成功修炼[烈焰火雨(2级)]");
-- 	else
-- 		player:alert(1,1,"修炼[烈焰火雨(2级)]需要达到烈焰火雨(1级)");
-- 		return 0;
-- 	end
-- end

-- function skillhuoyu3(player)
-- 	if( not checkJob(player,"wizard") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(417) == 2 then
-- 		player:set_skill_level(414,0);
-- 		player:set_skill_level(416,0);
-- 		player:set_skill_level(417,3);
-- 		-- player:alert(10,1,"恭喜您成功修炼[烈焰火雨(3级)]");
-- 	else
-- 		player:alert(1,1,"修炼[烈焰火雨(3级)]需要达到烈焰火雨(2级)");
-- 		return 0;
-- 	end
-- end

-- function skillqigbo1(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(514) == 0 then
-- 		player:add_skill(514,1);
-- 		-- player:alert(10,1,"恭喜您成功修炼[气功波(1级)]");
-- 	else
-- 		player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 	return 0;
-- 	end
-- end

-- function skillqigbo2(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(514) == 1 then
-- 		player:set_skill_level(514,2);
-- 		-- player:alert(10,1,"恭喜您成功修炼[气功波(2级)]");
-- 	else
-- 		player:alert(1,1,"修炼[气功波(2级)]需要达到气功波(1级)");
-- 	return 0;
-- 	end
-- end

-- function skillqigbo3(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(514) == 2 then
-- 		player:set_skill_level(514,3);
-- 		-- player:alert(10,1,"恭喜您成功修炼[气功波(3级)]");
-- 	else
-- 		player:alert(1,1,"修炼[气功波(3级)]需要达到气功波(2级)");
-- 	return 0;
-- 	end
-- end


-- function skillshenlg1(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(513) >= 1 then
-- 		player:set_skill_level(513,0);
-- 		if player:skill_level(518) == 0  then
-- 			player:add_skill(518);
-- 			player:alert(10,1,"恭喜您成功修炼[炎龙召唤(1级)]");
-- 		else
-- 			player:alert(1,1,"您已经学习了该技能,无法再次学习")
-- 		return 0;
-- 		end
-- 	else
-- 		player:alert(1,1,"修炼[炎龙召唤(1级)]需要达到无极召唤(1级)");
-- 		return 0;
-- 	end
-- end

-- function skillshenlg2(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(518) == 1 then
-- 		player:set_skill_level(518,2);
-- 		-- player:alert(10,1,"恭喜您成功修炼[炎龙召唤(2级)]");
-- 	else
-- 		player:alert(1,1,"修炼[炎龙召唤(2级)]需要达到炎龙召唤(1级)");
-- 	return 0;
-- 	end
-- end

-- function skillshenlg3(player)
-- 	if( not checkJob(player,"taoist") ) then
-- 		return 0;
-- 	end
-- 	if player:skill_level(518) == 2 then
-- 		player:set_skill_level(518,3);
-- 		-- player:alert(10,1,"恭喜您成功修炼[炎龙召唤(3级)]");
-- 	else
-- 		player:alert(1,1,"修炼[炎龙召唤(3级)]需要达到炎龙召唤(2级)");
-- 	return 0;
-- 	end
-- end