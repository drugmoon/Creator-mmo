module(..., package.seeall)

-- function guildlb(player)
-- 	if util.ppn(player,const.PP_GUILD_LIBAO) < 1 then
-- 		if player:get_level() >= 50 then
-- 			if player:num_bag_black() >= 4 then
-- 				player:set_param(const.PP_GUILD_LIBAO,1)
-- 				player:add_vcoin_bind(1000);
-- 				player:add_item("万年雪霜",2,1);
-- 				player:add_item("金条",5,1);
-- 				player:add_item("魔血石(中)",1,1);
-- 				player:add_item("玛雅传送卷",1,1);
-- 				player:add_item("转生丹(小)",2,1);
-- 			else
-- 				player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
-- 				return 0;
-- 			end
-- 		else
-- 			player:alert(1,1,"您的等级不足50级，请升级后再领取奖励！");
-- 			return 0;
-- 		end
-- 	else
-- 		player:alert(1,1,"您已领取过该奖励！");
-- 		return 0;
-- 	end
-- end

-- 使用后获得宝藏钥匙24060001，金砖（大）23060003，玛雅卷轴32000002，转生神殿卷32000003

--规则待完善
function ShouChong(player)
	-- if util.ppn(player,const.PP_GUILD_LIBAO) < 1 then
		-- if player:get_level() >= 50 then
			if player:num_bag_black() >= 4 then
				-- player:set_param(const.PP_GUILD_LIBAO,1)
				player:add_item(24060001,1,1);
				player:add_item(23060003,1,1);
				player:add_item(32000002,1,1);
				player:add_item(32000003,1,1);
			else
				player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
				return 0;
			end
		-- else
		-- 	player:alert(1,1,"您的等级不足50级，请升级后再领取奖励！");
		-- 	return 0;
		-- end
	-- else
	-- 	player:alert(1,1,"您已领取过该奖励！");
	-- 	return 0;
	-- end
end

--新手礼包
function xinshou(player)
	if util.ppn(player,const.PP_xinshou_LIBAO) < 1 then
		if player:num_bag_black() >= 4 then
			player:set_param(const.PP_xinshou_LIBAO,1)
			player:add_item("羽毛",40,1)
			player:add_item("护卫进阶丹",10,1)
			player:add_item("金砖（大）",2,1)
			player:add_item("副本卷轴",3,1)
		else
			player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
			return 0;
			end
	 else
		player:alert(1,1,"该礼包只能使用一次");
		return 0;
		end
end

--官网推广礼包
function guanwang(player)
	if util.ppn(player,const.PP_guanwang_LIBAO) < 1 then
		if player:num_bag_black() >= 4 then
			player:set_param(const.PP_guanwang_LIBAO,1)
			player:add_item("龙心碎片（中）",5,1)
			player:add_item("高级成就令牌",8,1)
			player:add_item("金砖（大）",5,1)
			player:add_item("副本卷轴",3,1)
		else
			player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
			return 0;
			end
	 else
		player:alert(1,1,"该礼包只能使用一次");
		return 0;
		end
end

--微信推广礼包
function weixin(player)
	if util.ppn(player,const.PP_weixin_LIBAO) < 1 then
		if player:num_bag_black() >= 4 then
			player:set_param(const.PP_weixin_LIBAO,1)
			player:add_item("声望卷（大）",8,1)
			player:add_item("白银矿石",20,1)
			player:add_item("金砖（大）",5,1)
			player:add_item("护卫经验丹（中）",5,1)
		else
			player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
			return 0;
			end
	 else
		player:alert(1,1,"该礼包只能使用一次");
		return 0;
		end
end

--VIP豪华礼包
function viphaohua(player)
	if util.ppn(player,const.PP_viphaohua_LIBAO) < 1 then
		if player:num_bag_black() >= 4 then
			player:set_param(const.PP_viphaohua_LIBAO,1)
			player:add_item("龙心碎片（中）",8,1)
			player:add_item("高级成就令牌",8,1)
			player:add_item("声望卷（大）",8,1)
			player:add_item("护卫进阶丹",15,1)
		else
			player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
			return 0;
			end
	 else
		player:alert(1,1,"该礼包只能使用一次");
		return 0;
		end
end
---使用后获得：<br /><a color='30ff00'>玉佩碎片（中）×20</a><br />雷霆法神令牌×2<br />金砖（中）×1<br />副本卷轴×2<br />500绑元×1<br />玛雅卷轴×2
function weihubuchang(player)
	if player:num_bag_black() >= 6 then
		player:add_item("玉佩碎片（中）",20,1)
		player:add_item("雷霆法神令牌",2,1)
		player:add_item("金砖（中）",1,1)
		player:add_item("副本卷轴",2,1)
		player:add_item("500绑元",1,1)
		player:add_item("玛雅卷轴",2,1)
	else
		player:alert(1,1,"您的背包不足6格，请清理背包后再领取奖励");
		return 0;
	end
end

function buchang2(player)
	if player:num_bag_black() >= 6 then
		player:add_item("500绑元",1,1)
		player:add_item("金砖（大）",1,1)
		player:add_item("雷霆法神令牌",2,1)
		player:add_item("副本卷轴",2,1)
		player:add_item("玛雅卷轴",2,1)
		player:add_item("转生神殿卷",1,1)
	else
		player:alert(1,1,"您的背包不足6格，请清理背包后再领取奖励");
		return 0;
	end
end

--VIP至尊礼包
function vipzhizun(player)
		if player:num_bag_black() >= 4 then
			player:add_item("狼牙碎片（中）",8,1)
			player:add_item("高级转生丹",8,1)
			player:add_item("龙心碎片（中）",8,1)
			player:add_item("声望卷（大）",10,1)
		else
			player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
			return 0;
			end
end

--号令沙城礼包
function haolingshacheng(player)
	if util.ppn(player,const.PP_haolingshacheng_LIBAO) < 1 then
		if player:num_bag_black() >= 4 then
			player:set_param(const.PP_haolingshacheng_LIBAO,1)
			player:add_item("龙心碎片（中）",5,1)
			player:add_item("羽毛",40,1)
			player:add_item("声望卷（大）",8,1)
			player:add_item("金砖（大）",5,1)
		else
			player:alert(1,1,"您的背包不足4格，请清理背包后再领取奖励");
			return 0;
			end
	 else
		player:alert(1,1,"该礼包只能使用一次");
		return 0;
		end
end

--------1级宝石包

function gem_libao_level1( player )
	if player:num_bag_black() >= 1 then
		local gem_libao_level1={
			[1]={pro=2,id=25010001},
			[2]={pro=3,id=25020001},
			[3]={pro=3,id=25030001},
			[4]={pro=3,id=25040001},
			[5]={pro=3,id=25050001},
		}
		local pro=0
		local add=0
		for i=1,#gem_libao_level1 do
			pro=gem_libao_level1[i].pro+pro
		end
		local rand=math.random(1,pro)
		for i=1,#gem_libao_level1 do
			add=gem_libao_level1[i].pro+add
			if rand <=add then
				player:add_item(gem_libao_level1[i].id,1,1)
				print(add,rand)
				break
			end
		end
	else
		player:alert(1,1,"您的背包不足1格，请清理背包");
		return 0;
	end
end
--------2级宝石包


function gem_libao_level2( player )
	if player:num_bag_black() >= 1 then
		local gem_libao_level2={
			[1]=	{pro=200,id=25010001},
			[2]=	{pro=300,id=25020001},
			[3]=	{pro=300,id=25030001},
			[4]=	{pro=300,id=25040001},
			[5]=	{pro=300,id=25050001},
			[6]=	{pro=20, id=25010002},
			[7]=	{pro=20, id=25020002},
			[8]=	{pro=20, id=25030002},
			[9]=	{pro=20, id=25040002},
			[10]=	{pro=20, id=25050002},
		}
		local pro=0
		local add=0
		for i=1,#gem_libao_level2 do
			pro=gem_libao_level2[i].pro+pro
		end
		local rand=math.random(1,pro)
		for i=1,#gem_libao_level2 do
			add=gem_libao_level2[i].pro+add
			if rand <=add then
				player:add_item(gem_libao_level2[i].id,1,1)
				print(add,rand)
				break
			end
		end
	else
		player:alert(1,1,"您的背包不足1格，请清理背包");
		return 0;
	end
end

--------3级宝石包



function gem_libao_level3( player )
	if player:num_bag_black() >= 1 then
		local gem_libao_level3={
			[1]=	{pro=200,id=25010001},
			[2]=	{pro=300,id=25020001},
			[3]=	{pro=300,id=25030001},
			[4]=	{pro=300,id=25040001},
			[5]=	{pro=300,id=25050001},
			[6]=	{pro=15, id=25010003},
			[7]=	{pro=15, id=25020003},
			[8]=	{pro=15, id=25030003},
			[9]=	{pro=15, id=25040003},
			[10]=	{pro=15, id=25050003},
		}
		local pro=0
		local add=0
		for i=1,#gem_libao_level3 do
			pro=gem_libao_level3[i].pro+pro
		end
		local rand=math.random(1,pro)
		for i=1,#gem_libao_level3 do
			add=gem_libao_level3[i].pro+add
			if rand <=add then
				player:add_item(gem_libao_level3[i].id,1,1)
				print(add,rand)
				break
			end
		end
	else
		player:alert(1,1,"您的背包不足1格，请清理背包");
		return 0;
	end
end


--------4级宝石包

function gem_libao_level4( player )
	if player:num_bag_black() >= 1 then
		local gem_libao_level4={
			[1]=	{pro=200, id=25010001},
			[2]=	{pro=300, id=25020001},
			[3]=	{pro=300, id=25030001},
			[4]=	{pro=300, id=25040001},
			[5]=	{pro=300, id=25050001},
			[6]=	{pro=12,  id=25010004},
			[7]=	{pro=12,  id=25020004},
			[8]=	{pro=12,  id=25030004},
			[9]=	{pro=12,  id=25040004},
			[10]=	{pro=12,  id=25050004},
		}
		local pro=0
		local add=0
		for i=1,#gem_libao_level4 do
			pro=gem_libao_level4[i].pro+pro
		end
		--print(pro)
		local rand=math.random(1,pro)
		for i=1,#gem_libao_level4 do
			add=gem_libao_level4[i].pro+add
			if rand <=add then
				player:add_item(gem_libao_level4[i].id,1,1)
				break
			end
		end
	else
		player:alert(1,1,"您的背包不足1格，请清理背包");
		return 0;
	end
end

-----------5级礼包
function gem_libao_level5( player )
	if player:num_bag_black() >= 1 then
		local gem_libao_level5={
			[1]=	{pro=12,  id=25010005},
			[2]=	{pro=12,  id=25020005},
			[3]=	{pro=12,  id=25030005},
			[4]=	{pro=12,  id=25040005},
			[5]=	{pro=12,  id=25050005},
		}
		local pro=0
		local add=0
		for i=1,#gem_libao_level5 do
			pro=gem_libao_level5[i].pro+pro
		end
		--print(pro)
		local rand=math.random(1,pro)
		for i=1,#gem_libao_level5 do
			add=gem_libao_level5[i].pro+add
			if rand <=add then
				player:add_item(gem_libao_level5[i].id,1,1)
				break
			end
		end
	else
		player:alert(1,1,"您的背包不足1格，请清理背包");
		return 0;
	end
end