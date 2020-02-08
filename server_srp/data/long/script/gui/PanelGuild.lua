module(..., package.seeall)
--帮会

local guildInfos = {
	[1]={level = 1, need_exp=1000000,	opex=300,	upper_limit=100,dc=11,	buff_id=29001,},
	[2]={level = 2, need_exp=5000000,	opex=400,	upper_limit=90, dc=16,	buff_id=29002,},
	[3]={level = 3, need_exp=20000000,	opex=500,	upper_limit=80, dc=21,	buff_id=29003,},
	[4]={level = 4, need_exp=50000000,	opex=600,	upper_limit=70,	dc=26,	buff_id=29004,},
	[5]={level = 5, need_exp=100000000,	opex=800,	upper_limit=60,	dc=31,	buff_id=29005,},
	[6]={level = 6, need_exp=300000000,	opex=1000,	upper_limit=55,	dc=36,	buff_id=29006,},
	[7]={level = 7, need_exp=0,			opex=1200,	upper_limit=50,	dc=41,	buff_id=29007,},
};

local GUILD_TITLE = {
	OUT=100,
	ENTERING=101,
	NORMAL=102,
	ADV=200,
	VICEADMIN=300,
	ADMIN=1000,
}

local MAX_GUILD_LEVEL = #guildInfos;
local MAX_DONATE_NUM = 21000;

local MAX_GUILD_VICENUM = 1;
local MAX_GUILD_ADV_NUM = 3

function getGuild(player)
	local guildName = player:get_guild();
	if guildName == "" then return; end
	local guild = server.find_guild(guildName);
	return guild;
end

function onPanelData(player,commond)
	local data = util.decode(commond);
	if not data  then return end
	if data.actionid == "guildDonate" then
		doGuildDonate(player, data.param);
	elseif data.actionid == "guildAssets" then
		pushGuildAssets(player);
	elseif data.actionid == "guildPrays" then
		pushGuildPrays(player);
	elseif data.actionid == "myGuildDonate" then
		pushMyGuildDonate(player);
	elseif data.actionid == "donateItem" then
		if data.pos >= 0 and data.typeID > 0 then
			donateItemToGuild(player, data.pos, data.typeID);
		end
	elseif data.actionid == "convertItem" then
		-- print("convertItem", data.pos, data.typeID)
		if data.pos >= 0 and data.typeID > 0 then
			convertGuildItem(player, data.pos, data.typeID);
		end
	elseif data.actionid == "destoryItem" then
		-- print("destoryItem", data.pos, data.typeID)
		if data.pos >= 0 and data.typeID > 0 then
			destoryGuildItem(player, data.pos, data.typeID);
		end
	elseif data.actionid == "hongbaoInfo" then
		pushGuildHongBaoInfo(player);
		player:push_lua_table("PanelGuild",util.encode({cmd = "redPacket", showRedPoint = checkGuildHongBao(player)}));
	elseif data.actionid == "sendHongbao" then
		if data.vcoin > 0 and data.num > 0 then
			sendGuildHongBao(player, data.vcoin, data.num);
		end
	elseif data.actionid == "grabHongbao" then
		grabGuildHongBao(player);
	elseif data.actionid == "dismissGuild" then
		doDismissGuild(player);
	elseif data.actionid == "demiseAdmin" then
		if data.memberName then
			demiseGuildAdmin(player, data.memberName)
		end
	elseif data.actionid == "appointVice" then
		if data.memberName then
			appointGuildVice(player, data.memberName);
		end
	elseif data.actionid == "appointAdv" then
		if data.memberName then
			appointGuildAdv(player, data.memberName);
		end
	elseif data.actionid == "dismissPost" then
		if data.memberName then
			dismissGuildPost(player, data.memberName);
		end
	elseif data.actionid == "kickOut" then
		if data.memberName then
			kickOutGuildMember(player, data.memberName);
		end
	elseif data.actionid == "agreeJoin" then
		if data.memberName then
			agreeJoinGuild(player, data.memberName);
		end
	elseif data.actionid == "refuseJoin" then
		if data.memberName then
			refuseJoinGuild(player, data.memberName);
		end
	elseif data.actionid == "guildWar" then
		if data.guildId then
			declareGuildWar(player, data.guildId);
		end
	elseif data.actionid == "guildWarAll" then
		declareGuildWar(player);
	elseif data.actionid == "prayNeedDonate" then
		pushPrayNeedDonate(player, data.index);
	elseif data.actionid == "guildPray" then
		upgradeGuildPray(player, data.index);
	elseif data.actionid == "refuseAll" then
		if util.isTable(data.applicants) then
			refuseAllApplicants(player, data.applicants)
		end
	elseif data.actionid == "agreeAll" then
		if util.isTable(data.applicants) then
			agreeAllApplicants(player, data.applicants)
		end
	elseif data.actionid == "checkRedPacket" then
		player:push_lua_table("PanelGuild",util.encode({cmd = "redPacket", showRedPoint = checkGuildHongBao(player)}));
	end
end

-- btn_appoint_vice
-- btn_appoint_elder
-- btn_dismiss_post
-- btn_kick_out

ckpanel.add_listener("gui.PanelGuild.onPanelData",gui.PanelGuild.onPanelData)

---------------我的帮会贡献---------------
function pushMyGuildDonate(player)
	local result ={};
	result.cmd = "contribute";
	result.contribute = player:get_guild_pt();
	player:push_lua_table("PanelGuild",util.encode(result));
end

--帮会捐献只捐金币，num单位万 1w金币增加150贡献度，150点帮会会财富
function doGuildDonate(player,num)
	-- print("doGuildDonate", num);
	local guild = getGuild(player);
	if not guild then return end
	if num > MAX_DONATE_NUM then num = MAX_DONATE_NUM; end
	if player:get_gamemoney() >= num * 10000  then
		player:sub_gamemoney(num * 10000,"帮会捐献");

		-- player:set_param(const.PP_GUILD_DONATE,util.ppn(player,const.PP_GUILD_DONATE) + num * 150);
		-- player:set_param(const.PP_GUILD_DONATE_ALL,util.ppn(player,const.PP_GUILD_DONATE_ALL) + num * 150);
		player:add_guild_pt(num * 150);
		pushMyGuildDonate(player);
		if guild:set_guild_point(guild:get_guild_point() + num * 150) then
			player:alert(1,1,"您捐赠了"..num.."万金币,增加"..(num * 150).."点帮会财富");
			checkGuildUpgrade(player, guild);
			pushGuildAssets(player, guild);
			gui.PanelGongXun.checkTaskComplete(player,4)
		end
	else
		player:alert(1,1,"金币不足!")
	end
end

--帮会捐献后自动检测升级
function checkGuildUpgrade(player, guild)
	if not guild then return end
	local guildLevel = guild:get_guild_level();
	local guildExp = guild:get_guild_point();
	local guildName = player:get_guild();
	if guildLevel < MAX_GUILD_LEVEL then
		local needExp = guildInfos[guildLevel].need_exp
		if guildExp >= needExp then
			if guild:set_guild_point(guildExp - needExp) then
				guild:set_guild_level(guildLevel + 1);
				player:guild_exe("gui.PanelGuild.freshGuildState")
				server.info(10000,0,"恭喜["..guildName.."]在全体成员的努力下发展壮大，提升至【"..guild:get_guild_level().."级】!");
			end
			-- freshPanel(player);
		-- else
		-- 	player:alert(1,1,"当前帮会财富不足"..needExp.."，无法升级帮会!");return;
		end
	-- else
		-- player:alert(1,1,"当前帮会已达最高等级,下级暂未开放!");return;
	end
end

--推送帮会资产
function pushGuildAssets(player, guild)
	guild = guild or getGuild(player);
	if not guild then return end
	local result = {
		guildLevel = 1,
		guildExp = 0,
		needExp = 0,
		opex = 0,
		memberNum = 1,
	};
	result.cmd = "guildAssets";
	result.guildLevel = guild:get_guild_level();
	result.guildExp = guild:get_guild_point();
	result.memberNum = guild:get_member_num();
	local guildInfo = guildInfos[result.guildLevel];
	if guildInfo then
		result.needExp = guildInfo.need_exp;
		result.opex = guildInfo.opex;
		result.memberMax = guildInfo.upper_limit;

	end
	player:push_lua_table("PanelGuild",util.encode(result));
end

function doDismissGuild(player)
	if GUILD_TITLE.ADMIN == player:get_guild_title() then
		local guild = getGuild(player);
		if guild then
			if guild:get_member_num() > 1 then
				return player:alert(1, 1, "帮会总人数大于1,不可以解散帮会解散失败！")
			end
			guild:dissmiss_guild(); -- 解散帮会
			player:alert(1, 1, "帮会解散成功！")
		end
	end
end

---------------------------------帮会仓库-------------------------
function donateItemToGuild( player,pos,typeID )
	local guild = getGuild(player);
	if not guild then return; end
	local item = player:get_item(typeID,pos);
	if item then
		local contribute = player:get_item_contribute(typeID);
		if contribute > 0 then
			if item:check_bind() == 0 then

				if guild:is_guild_depot_full() then return player:alert(1,1,"帮会仓库已满！"); end

				if player:remove_item_by_type_pos(typeID,pos) == 0 then
					player:add_guild_pt(contribute);
					
					pushMyGuildDonate(player);
					local name,ac,acmax,mac,macmax,dc,dcmax,mc,mcmax,sc,scmax = player:get_item_base_data(typeID)
					player:alert(1,1,"您捐赠["..name.."]得到["..contribute.."]贡献值");
					guild:add_item_op_record(player:get_name(), typeID, 0);
					if guild:add_item(typeID,item:get_updlevel(),item:get_upzlevel()) == 1 then
						-- player:update_guild_depot();
						player:guild_exe("gui.PanelGuild.freshGuildDepot");
						-- player:set_param(const.PP_GUILD_CONTRIBUTE,util.ppn(player,const.PP_GUILD_CONTRIBUTE)+contribute)
					else
						player:alert(1,1,"帮会仓库已满！");
					end
				else
					player:alert(1,1,"捐献失败，请重试！")
				end
				
			else
				player:alert(1,1,"绑定物品无法捐献！");
			end
		else
			player:alert(1,1,"此物品无法捐献！");
		end
	else
		player:alert(1,1,"不存在此装备");
	end
end

-- 刷新仓库
function freshGuildDepot(player)
	player:update_guild_depot();
end

function convertGuildItem( player,pos,typeID )
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	if player:num_bag_black() < 1 then
		return player:alert(1, 1, "背包已满,无法兑换物品!")
	end

	local _pos,_typeId,_level,_zlevel = guild:get_item(pos);
	if _typeId then
		local contribute = player:get_item_contribute(_typeId);
		if player:get_guild_pt() >= contribute then
			player:add_item2(_typeId,1,100,"level(".._level.."),zlevel(".._zlevel..")",1);
			guild:rem_item(pos);
			player:sub_guild_pt(contribute);
			-- player:update_guild_depot();
			pushMyGuildDonate(player);
			player:guild_exe("gui.PanelGuild.freshGuildDepot");
			guild:add_item_op_record(player:get_name(), _typeId, 1);
			local name,ac,acmax,mac,macmax,dc,dcmax,mc,mcmax,sc,scmax = player:get_item_base_data(_typeId)
			player:alert(1,1,"您消耗"..contribute.."贡献值兑换["..name.."]成功");

			player:push_lua_table("PanelGuild",util.encode({cmd = "converSucceed"}));
		else
			player:alert(1,1,"贡献值不足");
		end
	else
		player:alert(1,1,"不存在此装备");
	end
end

--摧毁物品
function destoryGuildItem( player,pos,typeID )
	local guild_name = player:get_guild()
	if guild_name == "" then return end
	local guild = server.find_guild(guild_name)
	if not guild then return end
	if player:get_guild_title() < GUILD_TITLE.VICEADMIN then
		return player:alert(1,1,"副帮主以上才能摧毁物品！");
	end

	local _pos,_typeId,_level,_zlevel = guild:get_item(pos);
	if _typeId then
		guild:rem_item(pos);
		-- player:update_guild_depot();
		player:guild_exe("gui.PanelGuild.freshGuildDepot");
		guild:add_item_op_record(player:get_name(), _typeId, 2);
		player:push_lua_table("PanelGuild",util.encode({cmd = "destorySucceed"}));
	else
		player:alert(1,1,"不存在此装备");
	end
end
---------------------------------帮会红包-------------------------
local MIN_HONGBAO_NUM = 10;
local MAX_HONGBAO_NUM = 50;
local MIN_HONGBAO_VCOIN = 1000;
local MAX_HONGBAO_VCOIN = 2100000000;

-- get_red_packet_info
-- 获取红包信息 
-- 参数:没有
-- 返回值数量 0  没有人发红包
-- 返回值数量 4个   发红包角色名   剩余元宝   剩余数量  剩余时间
function pushGuildHongBaoInfo(player, guild)
	guild = guild or getGuild(player);
	if not guild then return end
	local owner, vcoinRemain, numRemain, timeRemain = guild:get_red_packet_info()
	-- print("owner, vcoinRemain, numRemain, timeRemain",owner, vcoinRemain, numRemain, timeRemain)
	local result = {
		cmd = "guildHongbao",
		numRemain = 0,
		timeRemain = 0,
		vcoinRemain = 0,
	}
	if owner then
		result.numRemain = numRemain;
		result.timeRemain = timeRemain;
		result.vcoinRemain = vcoinRemain;
	end
	player:push_lua_table("PanelGuild",util.encode(result));
end

-- set_red_packet_info
-- 设置红包信息
-- 参数:
-- 1 角色名
-- 2 红包元宝数
-- 3 数量
function alertHongBaoInfo(player)
	local guild = getGuild(player);
	if not guild then return; end
	local ownerName = guild:get_var("hongbao_owner");
	if player:get_name() == ownerName then
		return;
	end
	player:alert(10, 1, "[guild]<font color='#fff843'>玩家<font color='#01ebff'>"..ownerName.."</font>豪气冲天!发红包了!大家快去领取!<a color='#00ff00' href='event:local_OpenPanel_act24' underline='#00ff00'>我要领取</a></font>")
end

function sendGuildHongBao(player, vcoin, num)
	local guild = getGuild(player);
	if not guild then return end
	local owner, vcoinRemain, numRemain, timeRemain = guild:get_red_packet_info();
	-- print("sendGuildHongBao", owner, vcoinRemain, numRemain, timeRemain)
	if owner and numRemain ~= 0 and vcoinRemain ~= 0 then
		return player:alert(1, 1, "帮会内任有未领取红包!");
	end
	if vcoin < MIN_HONGBAO_VCOIN and vcoin > MAX_HONGBAO_VCOIN then
		return player:alert(1, 1, "请重新输入发放总元宝!");
	end
	if num < MIN_HONGBAO_NUM and num > MAX_HONGBAO_NUM then
		return player:alert(1, 1, "请重新输入发放红包数!");
	end
	if (player:get_vcoin()-player:get_vcoin_gs()) < vcoin then
		return player:alert(1, 1, "元宝不足"..vcoin.."!");
	end
	if 1 == player:sub_vcoin(vcoin,"发帮会红包") then
		guild:set_red_packet_info(player:get_name(), vcoin, num);
		player:alert(1,1, "成功发放帮会红包,总元宝"..vcoin..",总红包数"..num.."!");
		pushGuildHongBaoInfo(player, guild);
		--全帮会公告领取
		-- server.info(10,0,"[system]恭喜<font color='#01ebff'>"..player:get_name().."</font>创建帮会<font color='#01ebff'>"..player:get_guild().."</font><a color='#00ff00' href='event:local_OpenPanel_act7' underline='#00ff00'>加入帮会</a>");
		-- guild:alert(10, 1, "[guild]<font color='#fff843'>玩家<font color='#01ebff'>"..player:get_name().."</font>豪气冲天!发红包了!大家快去领取!<a color='#00ff00' href='event:local_OpenPanel_act24' underline='#00ff00'>我要领取</a></font>")
		guild:set_var("hongbao_owner", player:get_name());
		player:guild_exe("gui.PanelGuild.alertHongBaoInfo")
		player:guild_exe("gui.PanelGuild.pushRedPacketInfoToMember");
	else
		return player:alert(1, 1, "元宝不足"..vcoin.."!");
	end
end

function pushRedPacketInfoToMember(player)
	player:push_lua_table("tipsMsg",util.encode({tipType = "tip_red_packet", visible = true}));
	--发红包的时候帮会成员检测红包红点 
	gui.moduleRedPoint.checkGuildHongBao(player);
end

-- take_red_packet 
-- 领取红包
-- 参数:
-- 1 角色名
-- 返回值数量 1个 领取到的元宝数
function grabGuildHongBao(player)
	local guild = getGuild(player);
	if not guild then return end
	local owner, vcoinRemain, numRemain, timeRemain = guild:get_red_packet_info();
	if owner then
		local vcoin = guild:take_red_packet(player:get_name());
		if vcoin > 0 then
			player:add_vcoin_best_enable(vcoin, "帮会抢红包");
			player:alert(1, 1, "成功领取"..vcoin.."元宝帮会红包!");
			player:push_lua_table("PanelGuild",util.encode({cmd = "getHongBao", vcoin = vcoin}));
			
			onGrabGuildHongBao(player);
		elseif vcoin == 0 then
			player:alert(1, 1, "抱歉,帮会红包已全部领取完毕!");
		elseif vcoin < 0 then
			player:alert(1, 1, "已领取过该帮会红包,不可重复领取!");
		end
	end
end

function onGrabGuildHongBao(player)
	local guild = getGuild(player);
	if not guild then return end
	local owner, vcoinRemain, numRemain, timeRemain = guild:get_red_packet_info();
	if numRemain == 0 then
		--红包抢完的时候帮会成员检测红包红点 
		player:guild_exe("gui.moduleRedPoint.checkGuildHongBao")
	else
		gui.moduleRedPoint.checkGuildHongBao(player)
	end
	player:guild_exe("gui.PanelGuild.pushGuildHongBaoInfo");
end

--local ret = guild:check_red_packet(playerName) ret:-1(已领取) -2(过期) else(剩余红包数量)
function checkGuildHongBao(player)
	local visible = false;
	local guild = getGuild(player);
	if guild and guild:check_red_packet(player:get_name()) > 0 then
		visible = true;
	end
	return visible;
end

---------------------------------帮会维护----------------------------------
--零点扣除维护资金
function reduceGuildAssets(guild)
	if guild then
		local guildLevel = guild:get_guild_level();
		local guildInfo = guildInfos[guildLevel];
		if guildInfo then
			local guildPoint = guild:get_guild_point();
			if guildPoint > guildInfo.opex then
				guild:set_guild_point(guildPoint - guildInfo.opex);
			else
				guild:set_guild_point(0);
			end
			-- 后续补充扣除财富降级
		end
	end
end

---------------------------------帮会成员----------------------------------
function isGuildAdmin(player)
	if GUILD_TITLE.ADMIN == player:get_guild_title() then
		return true
	end
end

function isGuildRealMember(guild, memberName)
	return guild:get_member_title(memberName) >  GUILD_TITLE.NORMAL
end

function isGuildEnteringMember(guild, memberName)
	return guild:get_member_title(memberName) ==  GUILD_TITLE.ENTERING
end

--验证帮会属性是否合理
function checkGuildTitle(player, memberName)
	local guild = getGuild(player);
	if guild then
		local myTitle = player:get_guild_title();
		local memberTitle = guild:get_member_title(memberName);
		-- print("checkGuildTitle", memberTitle > GUILD_TITLE.OUT)
		-- print("checkGuildTitle", myTitle > memberTitle)
		if memberTitle > GUILD_TITLE.OUT and myTitle > memberTitle then
			return guild, myTitle, memberTitle 
		end
		if memberTitle == GUILD_TITLE.OUT then
			player:alert(1,1,"抱歉,对方尚未申请入会")
		elseif myTitle < memberTitle then
			player:alert(1,1,"抱歉,对方帮会职务比你高")
		end
	end
end
-- btn_appoint_vice
-- btn_appoint_elder
-- btn_dismiss_post
-- btn_kick_out

--禅让帮主
function demiseGuildAdmin(player, memberName)
	local guild, myTitle, memberTitle = checkGuildTitle(player, memberName);
	if guild then
		local myName = player:get_name();
		if myTitle == GUILD_TITLE.ADMIN then
			guild:set_member_title(myName, memberTitle);
			guild:set_member_title(memberName, GUILD_TITLE.ADMIN);
			player:alert(1, 1, "您已将帮主之位传于"..memberName);
			guild:notify_member_change();
		else
			player:alert(1,1,"只有帮主可以禅让帮主职务");
		end
	end
end

--任命副帮主
function appointGuildVice(player, memberName)
	local guild, myTitle, memberTitle = checkGuildTitle(player, memberName);
	if guild then
		local myName = player:get_name();
		if not(myTitle == GUILD_TITLE.ADMIN) then
			return player:alert(1,1,"只有帮主可以任命副帮主职务");
		end
		if memberTitle == GUILD_TITLE.VICEADMIN then
			return player:alert(1,1,"抱歉,"..memberName.."已经是副帮主");
		end
		if guild:get_viceadmin_count() >= MAX_GUILD_VICENUM then
			return player:alert(1,1,"抱歉,副帮主人数已经达到上限");
		end
		guild:set_member_title(memberName, GUILD_TITLE.VICEADMIN);
		guild:notify_member_change();
	end
end

--任命长老
function appointGuildAdv(player, memberName)
	local guild, myTitle, memberTitle = checkGuildTitle(player, memberName);
	if guild then
		if not(myTitle > GUILD_TITLE.ADV) then
			return player:alert(1,1,"只有帮主和副帮主可以任命长老职务");
		end
		if memberTitle == GUILD_TITLE.ADV then
			return player:alert(1,1,"抱歉"..memberName.."已经是长老");
		end
		if guild:get_adv_count() >= MAX_GUILD_ADV_NUM then
			return player:alert(1,1,"抱歉,长老人数已经达到上限");
		end
		guild:set_member_title(memberName, GUILD_TITLE.ADV);
		guild:notify_member_change();
	end
end

--解除帮会职务
function dismissGuildPost(player, memberName)
	local guild, myTitle, memberTitle = checkGuildTitle(player, memberName);
	if guild then
		if memberTitle <= GUILD_TITLE.NORMAL then
			return player:alert(1,1,memberName.."只是普通成员,没有职务可以解除")
		end
		guild:set_member_title(memberName, GUILD_TITLE.NORMAL);
		player:alert(1,1,"成功解除"..memberName.."的职务");
		guild:notify_member_change();
	end
end

--踢出帮会
function kickOutGuildMember(player, memberName)
	local guild, myTitle, memberTitle = checkGuildTitle(player, memberName);
	if guild then
		if myTitle < GUILD_TITLE.VICEADMIN then
			return player:alert(1,1,"副帮主以上才能驱逐帮会成员")
		end
		guild:set_member_title(memberName, GUILD_TITLE.OUT);
		player:alert(1,1,"成功将"..memberName.."踢出帮会");
		guild:notify_member_change();
	end
end

--同意帮会申请
function agreeJoinGuild(player, memberName)
	local guild, myTitle, memberTitle = checkGuildTitle(player, memberName);
	-- print("agreeJoinGuild", myTitle, memberTitle, memberName);
	if guild then
		if myTitle < GUILD_TITLE.ADV then
			return player:alert(1,1,"抱歉,您没有同意权限");
		end
		if memberTitle >= GUILD_TITLE.NORMAL then
			return player:alert(1,1, memberName.."已经是帮会成员");
		end
		local guildLevel = guild:get_guild_level();
		local guildInfo = guildInfos[guildLevel];
		if guildInfo then
			local realMemberNum = guild:get_member_num();
			if not (realMemberNum < guildInfo.upper_limit) then
				return player:alert(1, 1, "帮会人数已满");
			end
			guild:set_member_title(memberName, GUILD_TITLE.NORMAL);
			guild:notify_member_change();
		end
	end
end

--拒绝帮会申请
function refuseJoinGuild(player, memberName)
	local guild, myTitle, memberTitle = checkGuildTitle(player, memberName);
	if guild then
		if myTitle < GUILD_TITLE.ADV then
			return player:alert(1,1,"抱歉,您没有拒绝权限");
		end
		if memberTitle >= GUILD_TITLE.NORMAL then
			return player:alert(1,1, memberName.."已经是帮会成员");
		end
		if GUILD_TITLE.OUT == guild:set_member_title(memberName, GUILD_TITLE.OUT) then
			guild:notify_member_change();
		end
	end
end

---------------------帮会列表----------------------
--帮会宣战
function declareGuildWar(player, guildId)
	if player:get_guild_title() < GUILD_TITLE.VICEADMIN then
		return player:alert(1,1,"副帮主以上才能开启帮会战");
	end
	local guild = getGuild(player);
	if not guild then return end
	if guildId then
		local ret = guild:start_war(guildId);
		if ret == 0 then
			player:alert(1,1,"成功开启帮会战");
		end
	else
		guild:start_all_war();
		player:alert(1,1,"成功一键宣战");
	end
end


---------------------一键同意-----------------------------
function agreeAllApplicants(player, applicants)
	local guild = getGuild(player);
	if guild then
		local myTitle = player:get_guild_title()
		if myTitle < GUILD_TITLE.ADV then
			return player:alert(1,1,"抱歉,您没有同意权限");
		end
		-- if guild:agree_all() > 0 then
		-- 	guild:notify_member_change();	
		-- end

		local guildLevel = guild:get_guild_level();
		local guildInfo = guildInfos[guildLevel];
		if guildInfo then
			local realMemberNum = guild:get_member_num();
			if realMemberNum >= guildInfo.upper_limit then
				return player:alert(1, 1, "帮会人数已满");
			end
			
			if guild:agree_all() > 0 then
				guild:notify_member_change();	
			end


			-- local count = guildInfo.upper_limit - realMemberNum;
			-- if count > #applicants then count = #applicants; end
			-- for i=1,count do
			-- 	memTitle = guild:get_member_title(applicants[i]);
			-- 	if memTitle == GUILD_TITLE.ENTERING then
			-- 		guild:set_member_title(applicants[i], GUILD_TITLE.NORMAL);
			-- 	end
			-- end
			-- guild:notify_member_change();
		end
	end
end

function refuseAllApplicants(player, applicants)
	local guild = getGuild(player);
	if guild then
		local myTitle = player:get_guild_title()
		if myTitle < GUILD_TITLE.VICEADMIN then
			return player:alert(1,1,"抱歉,您没有拒绝权限");
		end
		if guild:refuse_all() > 0 then
			guild:notify_member_change();	
		end
		-- for i=1,#applicants do
		-- 	if guild:get_member_title(applicants[i]) == GUILD_TITLE.ENTERING then
		-- 		guild:set_member_title(applicants[i], GUILD_TITLE.OUT);
		-- 	end
		-- end
		-- guild:notify_member_change();
	end
end

--设置buff
function onEnterGuild(player)
	-- print("onEnterGuild")
	freshGuildState(player);
	refreshGuildPrayAttr(player);
end

--清除buff
function onLeaveGuild(player)
	-- print("onLeaveGuild")
	for i,v in ipairs(guildInfos) do
		player:clear_buff(v.buff_id);
	end
	clearGuildPrayAttr(player);
end

--帮会升级重设buff
function freshGuildState(player)
	local guild = getGuild(player);
	if not guild then return; end
	local guildLevel = guild:get_guild_level();
	local guildInfo = guildInfos[guildLevel];
	if guildInfo then
		player:set_buff(guildInfo.buff_id);
	end
end


---------------------帮会祈祷---------------------

---帮会祈祷buff
-- 需要12个，分别攻击下限(上限)，物防下限(上限)，魔防上限(下限)，生命，魔法，神圣攻击，暴击力，暴伤，韧性
---------------------------------帮会技能-------------------------
--帮会祈祷buff
local guildPrays = {
	[1] = {
		-- name = "攻击下限",
		con = const.PP_GUILD_LEVEL_ATK, 		
	},
	[2] = {
		-- name = "攻击上限",
		con = const.PP_GUILD_LEVEL_ATKMAX, 	
	},
	[3] = {
		-- name = "物防下限",
		con = const.PP_GUILD_LEVEL_AC, 		
	},
	[4] = {
		-- name = "物防上限",
		con = const.PP_GUILD_LEVEL_ACMAX, 	
	},
	[5] = {
		-- name = "魔防下限",
		con = const.PP_GUILD_LEVEL_MAC, 		
	},
	[6] = {
		-- name = "魔防上限",
		con = const.PP_GUILD_LEVEL_MACMAX, 	
	},
	[7] = {
		-- name = "生命",
		con = const.PP_GUILD_LEVEL_HPMAX, 	
	},
	[8] = {
		-- name = "魔法",
		con = const.PP_GUILD_LEVEL_MPMAX, 	
	},
	[9] = {
		-- name = "神圣",
		con = const.PP_GUILD_LEVEL_SACREDDAM,
		-- openCon = {const.PP_GUILD_LEVEL_HPMAX, 40, "生命达到40级解锁"}
	},
	[10]= {
		-- name = "韧性",
		con = const.PP_GUILD_LEVEL_TENACITY, 
		-- openCon = {const.PP_GUILD_LEVEL_SACREDDAM, 40, "神圣达到40级解锁"}
	},
	[11]= {
		-- name = "暴击力",
		con = const.PP_GUILD_LEVEL_BOJIPROP, 
		-- openCon = {const.PP_GUILD_LEVEL_SACREDDAM, 40, "神圣达到40级解锁"}
	},
	[12]= {
		-- name = "暴伤",
		con = const.PP_GUILD_LEVEL_BOJIPRES, 
		-- openCon = {const.PP_GUILD_LEVEL_SACREDDAM, 40, "神圣达到40级解锁"}
	},
}

local prayConf;

for k,v in pairs(tab.guildPray) do
	if guildPrays[v.skillType] then
		if not guildPrays[v.skillType].name then
			guildPrays[v.skillType].name = v.name
		end
		
		if not (guildPrays[v.skillType].maxLevel and guildPrays[v.skillType].maxLevel > v.level) then
			guildPrays[v.skillType].maxLevel = v.level
		end

		if v.needId and tab.guildPray[v.needId] then
			prayConf = tab.guildPray[v.needId]
			if guildPrays[prayConf.skillType] then
				guildPrays[v.skillType].openCon = {guildPrays[prayConf.skillType].con, prayConf.level, prayConf.name..prayConf.level.."级解锁"}
			end
		end
	end
end

local PRAY_LEVEL_LIMIT = 2;
local PRAY_LEVEL_MAX = 100;

local function getPrayLockedDesp(player, index)
	local prayInfo = guildPrays[index];
	-- print("getPrayLockedDesp", index, util.encode(prayInfo))
	if prayInfo then
		local openCon = prayInfo.openCon
		if openCon then
			if util.ppn(player, openCon[1]) < openCon[2] then
				return openCon[3];
			end
		end
	end
end

function getPrayAttrValue(player, skillType, prayLevel)
	-- print("getGuildPrayDonate1111", skillType, prayLevel)
	if not (guildPrays[skillType] and guildPrays[skillType].con) then return 0; end
	-- print("getGuildPrayDonate22222", guildPrays[skillType].con)
	prayLevel = prayLevel or util.ppn(player, guildPrays[skillType].con) + 1;
	local id = skillType * 10000 + prayLevel;
	local prayConf = tab.guildPray[id];
	-- print("getGuildPrayDonate33333", id, prayConf)
	if not prayConf then return 0 end
	-- 生命和魔法取其他字段
	if skillType == 8 or skillType == 7 then
		local jobName = player:get_job_name();
		if jobName == "warrior" then
			if skillType == 8 and prayConf.warriorMp and prayConf.warriorMp > 0 then
				return prayConf.warriorMp;
			end
			if skillType == 7 and prayConf.warriorHp and prayConf.warriorHp > 0 then
				return prayConf.warriorHp;
			end
		elseif jobName == "wizard" then
			if skillType == 8 and prayConf.wizardMp and prayConf.wizardMp > 0 then
				return prayConf.wizardMp;
			end
			if skillType == 7 and prayConf.wizardHp and prayConf.wizardHp > 0 then
				return prayConf.wizardHp;
			end
		elseif jobName == "taoist" then
			if skillType == 8 and prayConf.taoistMp and prayConf.taoistMp > 0 then
				return prayConf.taoistMp;
			end
			if skillType == 7 and prayConf.taoistHp and prayConf.taoistHp > 0 then
				return prayConf.taoistHp;
			end
		end
		return 0;
	else
		if not (prayConf.values and prayConf.values[1]) then return 0; end
		return prayConf.values[1];
	end
end

--推送帮会祈祷数据
function pushGuildPrays(player)
	-- for i,v in ipairs(guildPrays) do
	-- 	player:set_param(v.con, 0);--util.ppn(player, v.con);
	-- end

	local guild = getGuild(player);
	local guildLevel = guild:get_guild_level();
	if guildLevel < PRAY_LEVEL_LIMIT then
		-- return player:alert(1,1,"帮会等级达到"..PRAY_LEVEL_LIMIT.."级开启帮会祈祷");
	end
	local result ={};
	result.cmd = "guildPrays";
	local infos, info, level = {}, nil, nil, nil;
	for i,v in ipairs(guildPrays) do
		level = util.ppn(player, v.con);
		info = {
			level = level,
			attrName = v.name,
			attrValue = 0,
			lockedDesp = getPrayLockedDesp(player, i) or "",
			maxLevel = v.maxLevel,
		}
		if info.lockedDesp == "" then
			info.attrValue = getPrayAttrValue(player, i, level);
			-- info.attrValue = util.ppn(player, v.con) * 100;
		end
		if level < v.maxLevel then
			info.attrNext = getPrayAttrValue(player, i, level + 1);
		end
		-- print(i, util.encode(info))

		table.insert(infos, info);
	end
	result.infos = infos;
	player:push_lua_table("PanelGuild",util.encode(result));
end

function getGuildPrayDonate(player, skillType, prayLevel)
	-- print("getGuildPrayDonate1111", skillType, prayLevel)
	if not (guildPrays[skillType] and guildPrays[skillType].con) then return 0; end
	-- print("getGuildPrayDonate22222", guildPrays[skillType].con)
	prayLevel = prayLevel or util.ppn(player, guildPrays[skillType].con) + 1;
	local id = skillType * 10000 + prayLevel;
	local prayConf = tab.guildPray[id];
	-- print("getGuildPrayDonate33333", id, prayConf)
	if not prayConf then return 0; end
	
	return prayConf.contribution;
end

--升级帮会祈祷
function upgradeGuildPray(player,index)
	local lockedDesp = getPrayLockedDesp(player, index);
	if lockedDesp then
		return player:alert(1, 1, "该祈祷尚未开放");
	end
	local guildPray = guildPrays[index];
	if guildPray then
		local mLevel = util.ppn(player, guildPray.con);
		if mLevel < PRAY_LEVEL_MAX then
			local need_donate = getGuildPrayDonate(player, index, mLevel + 1);
			if player:get_guild_pt() >= need_donate then
				player:sub_guild_pt(need_donate);
				player:set_param(guildPray.con, mLevel+1);
				pushGuildPrays(player);
				pushMyGuildDonate(player);
				onUpgradeGuildPray(player, index);
				pushPrayNeedDonate(player, index);
			else
				player:alert(1,1,"帮会贡献度不足"..need_donate..",无法升级帮会祈祷！");
			end
		else
			player:alert(1,1,"该祈祷已经满级！");
		end
	end
end



function pushPrayNeedDonate(player, index)
	local result = {
		cmd = "prayNeedDonate",
		needDonate = getGuildPrayDonate(player, index),
	};
	player:push_lua_table("PanelGuild",util.encode(result));
end

local attrSap = {
	[1004] = 7, --战
	[1005] = 8,
	[1006] = 9, --法
	[1007] = 10,
	[1008] = 11, --道
	[1009] = 12,
	[1010] = 3, --物防
	[1011] = 4,
	[1012] = 5, --魔防
	[1013] = 6,
	[1019] = 41, --神圣
	[115] = 44, --韧性
	[1015] = 32, --暴击伤害
	[1014] = 31, --暴击率
}

-- local sap2Desp = {
-- 	[7] = "",
-- 	[8] = "",
-- 	[9] = "",
-- 	[10] = "",
-- 	[11] = "",
-- 	[12] = "",
-- 	[3] = "",
-- 	[4] = "",
-- 	[5] = "",
-- 	[6] = "",
-- 	[41] = "",
-- 	[44] = "",
-- 	[31] = "",
-- 	[32] = "",
-- }

--获得属性表 sapid:value
function getPrayAttrs(player, skillType, prayLevel)
	if not (guildPrays[skillType] and guildPrays[skillType].con) then return; end
	prayLevel = prayLevel or util.ppn(player, guildPrays[skillType].con);
	local id = skillType * 10000 + prayLevel;
	local prayConf = tab.guildPray[id];
	if not prayConf then return; end
	local prayAttrs = {};
	local jobName = player:get_job_name();
	if jobName == "warrior" then
		if prayConf.warriorMp and prayConf.warriorMp > 0 then
			prayAttrs[1] = prayConf.warriorMp;
		end
		if prayConf.warriorHp and prayConf.warriorHp > 0 then
			prayAttrs[2] = prayConf.warriorHp;
		end
	elseif jobName == "wizard" then
		if prayConf.wizardMp and prayConf.wizardMp > 0 then
			prayAttrs[1] = prayConf.wizardMp;
		end
		if prayConf.wizardHp and prayConf.wizardHp > 0 then
			prayAttrs[2] = prayConf.wizardHp;
		end
	elseif jobName == "taoist" then
		if prayConf.taoistMp and prayConf.taoistMp > 0 then
			prayAttrs[1] = prayConf.taoistMp;
		end
		if prayConf.taoistHp and prayConf.taoistHp > 0 then
			prayAttrs[2] = prayConf.taoistHp;
		end
	end
	if prayConf.attrs and prayConf.values then
		for i,v in ipairs(prayConf.attrs) do
			if attrSap[v] and prayConf.values[i] then
				prayAttrs[attrSap[v]] = prayConf.values[i];
			end
		end
	end
	return prayAttrs;
end

function compareAttrs(attrsA, attrsB)
	if not attrsB then return attrsA; end
	local attrs = {};
	for k,v in pairs(attrsA) do
		if not attrsB[k] then
			attrs[k] = v;
		else
			attrs[k] = v - attrsB[k];
		end
	end
	return attrs;
end

function onUpgradeGuildPray(player, skillType)
	if not (guildPrays[skillType] and guildPrays[skillType].con) then return; end
	local prayLevel = util.ppn(player, guildPrays[skillType].con);
	local curAttrs = getPrayAttrs(player, skillType, prayLevel);
	-- print("onUpgradeGuildPray", util.encode(curAttrs))
	if curAttrs and prayLevel > 1 then
		curAttrs = compareAttrs(curAttrs, getPrayAttrs(player, skillType, prayLevel - 1));
	end

	if curAttrs then
		for k,v in pairs(curAttrs) do
			-- print("refreshGuildPrayAttr", k, v, player:get_script_attr(k))
			player:set_script_attr(k, player:get_script_attr(k) + v);
		end
	end

	player:recal_attr()
end

--上线设置帮会祈祷属性(仅仅有帮会的时候)
function refreshGuildPrayAttr(player)
	if player:get_guild_title() < GUILD_TITLE.NORMAL then return; end

	local prayAttrs;
	for i,v in ipairs(guildPrays) do
		prayAttrs = getPrayAttrs(player, i);
		if prayAttrs then
			for k,v in pairs(prayAttrs) do
				-- print("refreshGuildPrayAttr", k, v, player:get_script_attr(k))
				player:set_script_attr(k, player:get_script_attr(k) + v);
			end
		end
	end

	player:recal_attr()
end

--清除祈祷属性
function clearGuildPrayAttr(player)
	-- print("1111111", player:get_guild_title(), GUILD_TITLE.NORMAL)
	if player:get_guild_title() < GUILD_TITLE.NORMAL then return; end
	-- print("clearGuildPrayAttr")

	local prayAttrs;
	for i,v in ipairs(guildPrays) do
		prayAttrs = getPrayAttrs(player, i);
		if prayAttrs then
			for k,v in pairs(prayAttrs) do
				-- print("clearGuildPrayAttr", k, v, player:get_script_attr(k))
				player:set_script_attr(k, player:get_script_attr(k) - v);
				-- print("clearGuildPrayAttr", k, v, player:get_script_attr(k))
			end
		end
	end

	player:recal_attr()
end

function checkGuildBuff(player)
	for i,v in ipairs(guildInfos) do
		player:clear_buff(v.buff_id);
	end
	local guild = getGuild(player);
	if not guild then return; end
	local guildLevel = guild:get_guild_level();
	local guildInfo = guildInfos[guildLevel];
	if guildInfo then
		player:set_buff(guildInfo.buff_id);
	end
end

login.add_login_listener(refreshGuildPrayAttr);
login.add_login_listener(checkGuildBuff);