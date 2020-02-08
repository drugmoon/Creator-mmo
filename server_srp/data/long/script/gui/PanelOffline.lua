module(...,package.seeall)
local MAX_TIME = 7*24*60*60;

local maps = {
	{mapid = "v001",mapname ="历练秘教", condition = "开启条件:70级", lvLimit = 70,
		dropItem = {
			{id = 40000001, name = "经验", },
			{id = 40000008, name = "内功经验", },
		},
		monConf = {
			{monid=1005001,	hp = 6000,	monname="红蛇", 	exp = 60, innerpowerExp=12,	second = 4,	},
			{monid=1005003,	hp = 8150,	monname="毒蜘蛛", 	exp = 81, innerpowerExp=16,	second = 4,	},
			{monid=1005002,	hp = 6000,	monname="食人花", 	exp = 60, innerpowerExp=12,	second = 4,	},
			{monid=1006001,	hp = 8150,	monname="虎蛇", 	exp = 81, innerpowerExp=16, second = 4,	},
		}},
	{mapid = "v002",mapname ="金矿古墓", condition = "开启条件:90级或VIP1", lvLimit = 90, vipLv =1,
		dropItem = {
			{id = 40000001, name = "经验",},
			{id = 40000008, name = "内功经验", },
			{id = 40000003, name = "绑定金币",},
		},
		monConf = {
			{monid=1005001,	hp = 7720,	monname="钳虫", 	exp = 77, innerpowerExp=15,second = 4,	coinbind = 10, prob = 600},
			{monid=1005002,	hp = 9870,	monname="剧毒蜈蚣", exp = 98, innerpowerExp=19,second = 4,	coinbind = 10, prob = 600},
			{monid=1005003,	hp = 7720,	monname="黑色恶蛆", exp = 77, innerpowerExp=15,second = 4,	coinbind = 10, prob = 600},
			{monid=1005004,	hp = 9870,	monname="跳跳蜂", 	exp = 98, innerpowerExp=19,second = 4,	coinbind = 10, prob = 600},
		}
	},
	{mapid = "v003",mapname ="神秘猪洞", condition = "开启条件:100级或VIP1", lvLimit = 100,vipLv =1,
		dropItem = {
			{id = 40000001, name = "经验", },
			{id = 40000008, name = "内功经验", },
			{id = 23000001, name = "玉佩碎片", },
		},
		monConf = {
			{monid=1005001,	hp = 9440,	monname="红野猪", 	exp = 94,	 innerpowerExp=18,second = 4,	itemid = 23000001,prob = 100},
			{monid=1005002,	hp = 11860,	monname="黑野猪", 	exp = 118,	 innerpowerExp=23,second = 4,	itemid = 23000001,prob = 100},
			{monid=1005003,	hp = 9440,	monname="蛇蝎", 	exp = 94,	 innerpowerExp=18,second = 4,	itemid = 23000001,prob = 100},
			{monid=1005004,	hp = 11860,	monname="墓穴恶蛆", exp = 118,	 innerpowerExp=23,second = 4,	itemid = 23000001,prob = 100},
		}
	}
}
local shop = {}--tab.Store.getItemsByType(player,9)

local VIP_EXP_TIMES = function(player) return tab.Vip.getOfflineExpTimes(player) end
function checklevel(player,mapConf)
	if mapConf.vipLv and util.ppn(player,const.PP_VIP_LEVEL)>=mapConf.vipLv then
		return true
	end
	return player:get_level()>=mapConf.lvLimit
end

function onPanelData(player,commond)
	local data = util.decode(commond);
	if not data  then return end
	if data.actionid == "check" then
		checkOffline(player)
	elseif data.actionid == "fresh" then
		freshPanel(player,"fresh");
	elseif data.actionid == "changeMap" then
		changeOfflineMap(player,data.params[1])
	elseif data.actionid == "buy" then
		buyItem(player, data.params[1])
	elseif data.actionid == "shop" then
		freshShopPanel(player)
	elseif data.actionid == "start" then
		startOffline(player)
	elseif data.actionid == "stop" then
		-- syncAward(player,data.params[1],data.params[2])
		stopOffline(player)
	elseif data.actionid == "quickget" then
		getVipAward(player)
	elseif data.actionid == "get" then
		getAward(player)
	end
end
ckpanel.add_listener("gui.PanelOffline.onPanelData",gui.PanelOffline.onPanelData)

--等级够了自动开启离线挂机
function checkUpLevel(player, lv)
	local mapSelect = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if mapSelect<1 then
		for i,v in ipairs(maps) do
			if checklevel(player,v) then
				player:set_param(const.PP_OFFLINE_MAPIDX,i)
				break;
			end
		end
	end
end
cklevel.add_listener(checkUpLevel)

function checkOffline(player)
	local mapSelect = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if mapSelect<1 then
		for i,v in ipairs(maps) do
			if checklevel(player,v) then
				mapSelect = i;break;
			end
		end
	end
	if maps[mapSelect] then
		local record = util.decode(player:get_param(const.PP_OFFLINE_AWARD)) or {}
		if checknumber(record.monNum)>0 then
			if os.time()-player:get_last_logout_time() >= 60*5 then
				player:push_lua_table("open",util.encode({name = "extend_offline"}));
			end
		end
	end
end

function changeOfflineMap(player,index)
	if maps[index] then
		local state = util.ppn(player, const.PP_OFFLINE_STATE)
		if state == 1 then
			return util.alert_msg(4, "挂机已开始，无法更改地图！", player)
		end
		if checklevel(player,maps[index]) then
			if util.ppn(player, const.PP_OFFLINE_MAPIDX)~=index then
				player:set_param(const.PP_OFFLINE_MAPIDX,index)
				player:push_lua_table("PanelOffline",util.encode({
					cmd = "changeMap",
					mapSelect = index,
				}));
			end
		else
			util.alert_msg(4, "不满足该地图的挂机条件！", player)
		end
	end
end

function getMultiExpRemainTime(player)
	for i=24001,24012 do
		if player:have_buff(i) ~= 1 then
			return player:get_buff_valid_value(i)
		end
	end
	return 0
end

function freshPanel(player,state)
	local result = {}
	result.cmd = "fresh"
	result.mapSelect = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if result.mapSelect<1 then
		for i,v in ipairs(maps) do
			if checklevel(player,v) then
				result.mapSelect = i;break;
			end
		end
		player:set_param(const.PP_OFFLINE_MAPIDX,result.mapSelect)
	end
	if state =="fresh" then
		result.mapData = {}
		for i,v in ipairs(maps) do
			table.insert(result.mapData,{
				mapid = v.mapid,
				condition = v.condition, 
				mapname = v.mapname,
				dropItem = v.dropItem,
				monConf = v.monConf,
			})
		end
	end

	result.state = util.ppn(player, const.PP_OFFLINE_STATE)
	result.multiExpTime = getMultiExpRemainTime(player)
	result.record = util.decode(player:get_param(const.PP_OFFLINE_AWARD))
	result.vipExpAdd = util.ppn(player,const.PP_OFFLINE_AWARD_VIP)

	player:push_lua_table("PanelOffline",util.encode(result));
end

function freshShopPanel(player)
	local result = {}
	result.cmd = "shop"
	result.shop = tab.Store.getItemsByType(player,9)
	player:push_lua_table("PanelOffline",util.encode(result));
end

function buyItem(player, storeId)
	local itemData = tab.Store.buyShopItem(player,storeId,"离线经验",1,"notinbag")
	if itemData then


		-- freshPanel(player)
	end
end

function recordBuffInfo(player)
	local buff,buffvip
	for i=24001,24012 do
		if player:have_buff(i) ~= 1 then
			buff = true
			player:set_param(const.PP_EXP_MULTI_BUFFID,i)
			player:set_param(const.PP_EXP_MULTI_BUFFID_REMAINTIME,player:get_buff_valid_value(i))
			break
		end
	end
	for i=25001,25012 do
		if player:have_buff(i) ~= 1 then
			buffvip = true
			player:set_param(const.PP_EXP_MULTI_BUFFID_VIP,i)
			player:set_param(const.PP_EXP_MULTI_BUFFID_REMAINTIME_VIP,player:get_buff_valid_value(i))
			break
		end
	end
	if not buff then
		player:set_param(const.PP_EXP_MULTI_BUFFID,0)
		player:set_param(const.PP_EXP_MULTI_BUFFID_REMAINTIME,0)
	end
	if not buffvip then
		player:set_param(const.PP_EXP_MULTI_BUFFID_VIP,0)
		player:set_param(const.PP_EXP_MULTI_BUFFID_REMAINTIME_VIP,0)
	end
end

function startOffline(player)
	local index = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if maps[index] then
		player:set_param(const.PP_OFFLINE_START_TIME,os.time())
		player:set_param(const.PP_OFFLINE_STATE,1)
		recordBuffInfo(player)

		player:push_lua_table("PanelOffline",util.encode({cmd = "start"}));
	else
		util.alert_msg(4, "请先选择挂机地图！", player)
	end
end

function stopOffline(player)
	local index = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if maps[index] then
		local offlineTime = os.time() - util.ppn(player,const.PP_OFFLINE_START_TIME)
		player:set_param(const.PP_OFFLINE_TIME,util.ppn(player, const.PP_OFFLINE_TIME)+offlineTime)
		player:set_param(const.PP_EXP_MULTI_BUFFID_REMAINTIME,math.max(0,util.ppn(player, const.PP_EXP_MULTI_BUFFID_REMAINTIME)-offlineTime))
		player:set_param(const.PP_EXP_MULTI_BUFFID_REMAINTIME_VIP,math.max(0,util.ppn(player, const.PP_EXP_MULTI_BUFFID_REMAINTIME_VIP)-offlineTime))


		player:set_param(const.PP_OFFLINE_START_TIME,0)
		player:set_param(const.PP_OFFLINE_STATE,0)
		CalculateAward(player)
		player:push_lua_table("PanelOffline",util.encode({cmd = "stop"}));--停止挂机计时器
		freshPanel(player,"stop")
	else
		util.alert_msg(4, "请先选择挂机地图！", player)
	end
end

function getVipAward(player)
	local state = util.ppn(player, const.PP_OFFLINE_STATE)
	if state ==1 then return player:alert(1,0,"挂机中，无法领取！") end
	local vipExpAdd = util.ppn(player,const.PP_OFFLINE_AWARD_VIP)
	if vipExpAdd>0 then
		player:add_exp(vipExpAdd)
		player:set_param(const.PP_OFFLINE_AWARD_VIP,0)
		freshPanel(player)
	else
		if util.ppn(player, const.PP_VIP_LEVEL)>0 then
			player:alert(1,0,"VIP经验已领取！")
		else
			player:alert(1,0,"VIP等级不足，无法领取！")
		end
	end
end

function getAward(player)
	local record = util.decode(player:get_param(const.PP_OFFLINE_AWARD))
	if record then
		record.award = record.award or {}
		player:set_param(const.PP_OFFLINE_START_TIME,0)
		player:set_param(const.PP_OFFLINE_TIME,0)
		player:set_param(const.PP_OFFLINE_STATE,0)
		for k,v in pairs(record.award) do
			if v.id == 40000001 then
				player:add_exp(v.num)
			elseif v.id == 40000003 then
				player:add_gamemoney_bind(v.num)
			elseif v.id == 40000008 then
				gui.PanelInnerPower.addInnerPower(player,v.num)
			elseif v.id == 23000001 then
				player:add_item(v.id,v.num,1)
			end
		end
		player:set_param(const.PP_OFFLINE_AWARD,"")
		-- freshPanel(player)
	end
	player:push_lua_table("PanelOffline",util.encode({cmd="close"}))
end
local buffDef = {
	[24001]=	"双倍经验4小时",
	[24002]=	"双倍经验8小时",
	[24003]=	"三倍经验2小时",
	[24004]=	"三倍经验4小时",
	[24005]=	"三倍经验8小时",
	[24006]=	"四倍经验2小时",
	[24007]=	"四倍经验4小时",
	[24008]=	"四倍经验8小时",
	[24009]=	"双倍经验1小时",
	[24010]=	"双倍经验2小时",
	[24011]=	"三倍经验1小时",
	[24012]=	"四倍经验1小时",
	[25001]=	"vip双倍经验1小时",
	[25002]=	"vip双倍经验2小时",
	[25003]=	"vip双倍经验3小时",
	[25004]=	"vip双倍经验4小时",
	[25005]=	"vip三倍经验2小时",
	[25006]=	"vip三倍经验3小时",
	[25007]=	"vip三倍经验4小时",
	[25008]=	"vip三倍经验5小时",
	[25009]=	"vip四倍经验3小时",
	[25010]=	"vip四倍经验4小时",
	[25011]=	"vip四倍经验5小时",
	[25012]=	"vip四倍经验6小时",
}

function getMultiTimes(player)
	local buffId = util.ppn(player,const.PP_EXP_MULTI_BUFFID)
	local vipBuffId = util.ppn(player,const.PP_EXP_MULTI_BUFFID_VIP)

	local itemName = buffDef[buffId]
	local multi,multivip = 0,0
	if buffId>0 and itemName then
		if string.find(itemName,"双") then
			multi = 1
		elseif string.find(itemName,"三") then
			multi = 2
		elseif string.find(itemName,"四") then
			multi = 3
		end
	end
	itemName = buffDef[vipBuffId]
	if vipBuffId>0 and itemName then
		if string.find(itemName,"双") then
			multivip = 1
		elseif string.find(itemName,"三") then
			multivip = 2
		elseif string.find(itemName,"四") then
			multivip = 3
		end
	end
	return multi,multivip
end
local getRand = function(prob) return math.random(1,10000)<prob and 1 or 0 end

function CalculateAward(player)
	local mapidx = util.ppn(player, const.PP_OFFLINE_MAPIDX)
	if maps[mapidx] then
		local offlineTime = util.ppn(player,const.PP_OFFLINE_TIME)
		--最大收益不超过24小时
		if offlineTime > MAX_TIME then
			offlineTime = MAX_TIME
		end

		local monConf = maps[mapidx].monConf
		local monNum,rand = 0,0,0,0
		local record = util.decode(player:get_param(const.PP_OFFLINE_AWARD)) or {}
		local award = {}
		award[1] = record.award and record.award[1] or {id = 40000001, name = "经验", num = 0}
		award[2] = record.award and record.award[2] or {id = 40000003, name = "绑定金币", num = 0}
		award[3] = record.award and record.award[3] or {id = 40000008, name = "内功经验", num = 0}
		award[4] = record.award and record.award[4] or {id = 23000001, name = "玉佩碎片", num = 0}
			
		monNum = record.monNum or 0
		local multi,multivip = getMultiTimes(player)
		local multiTime = util.ppn(player, const.PP_EXP_MULTI_BUFFID_REMAINTIME)
		local vipMultiTime = util.ppn(player, const.PP_EXP_MULTI_BUFFID_REMAINTIME_VIP)
		local expAdd = 0
		if offlineTime>4 then
			-- local fitht = player:get_fightpoint()
			local gap = 3600
			local mtag = 900
			local step = math.floor(offlineTime/gap)
			if step > 1 and step <=24  then
				-- 避免直接整除出现整数收益，所以此处少算1小时
				for i=1,step-1 do
					monNum = monNum + mtag
					rand = math.random(1,#monConf)
					offlineTime = offlineTime - gap
					expAdd = expAdd + monConf[rand].exp*(1+multi+multivip)*mtag
					if multiTime>0 then
						multiTime = multiTime - gap
					else
						multi = 1
					end
					if vipMultiTime>0 then
						vipMultiTime = vipMultiTime - gap
					else
						multivip = 1
					end
					if monConf[rand].coinbind then
						award[2].num = award[2].num + monConf[rand].coinbind*math.round(mtag*monConf[rand].prob/10000)
					end
					if monConf[rand].innerpowerExp then
						award[3].num = award[3].num + monConf[rand].innerpowerExp*mtag
					end
					if monConf[rand].itemid then
						award[4].num = award[4].num + math.round(mtag*monConf[rand].prob/10000)
					end
				end
			end

			gap = 60
			mtag = 15
			step = math.floor(offlineTime/gap)
			if step > 0 and step <=60  then
				for i=1,step do
					monNum = monNum + mtag
					rand = math.random(1,#monConf)
					offlineTime = offlineTime - gap
					expAdd = expAdd + monConf[rand].exp*(1+multi+multivip)*mtag
					if multiTime>0 then
						multiTime = multiTime - gap
					else
						multi = 1
					end
					if vipMultiTime>0 then
						vipMultiTime = vipMultiTime - gap
					else
						multivip = 1
					end
					if monConf[rand].coinbind then
						award[2].num = award[2].num + monConf[rand].coinbind*math.round(mtag*monConf[rand].prob/10000)
					end
					if monConf[rand].innerpowerExp then
						award[3].num = award[3].num + monConf[rand].innerpowerExp*mtag
					end
					if monConf[rand].itemid then
						award[4].num = award[4].num + math.round(mtag*monConf[rand].prob/10000)
					end
				end
			end

			gap = 4
			mtag = 1
			step = math.floor(offlineTime/gap)
			if step > 0 and step <=60  then
				for i=1,step do
					monNum = monNum + mtag
					rand = math.random(1,#monConf)
					offlineTime = offlineTime - gap
					expAdd = expAdd + monConf[rand].exp*(1+multi+multivip)*mtag
					if multiTime>0 then
						multiTime = multiTime - gap
					else
						multi = 1
					end
					if vipMultiTime>0 then
						vipMultiTime = vipMultiTime - gap
					else
						multivip = 1
					end
					if monConf[rand].coinbind then
						award[2].num = award[2].num + monConf[rand].coinbind*getRand(mtag*monConf[rand].prob)
					end
					if monConf[rand].innerpowerExp then
						award[3].num = award[3].num + monConf[rand].innerpowerExp*mtag
					end
					if monConf[rand].itemid then
						award[4].num = award[4].num + getRand(mtag*monConf[rand].prob)
					end
				end
			end

			player:set_param(const.PP_OFFLINE_TIME,0)
			player:set_param(const.PP_EXP_MULTI_BUFFID_REMAINTIME,math.max(0,multiTime))
			award[1].num = award[1].num + expAdd
			
			if util.ppn(player, const.PP_VIP_LEVEL)>0 then
				player:set_param(const.PP_OFFLINE_AWARD_VIP,util.ppn(player,const.PP_OFFLINE_AWARD_VIP)+expAdd*0.3)
			end

			player:set_param(const.PP_OFFLINE_AWARD,util.encode({
				award = award,monNum = monNum
			}))
			return award,monNum
		end
	end
end

function syncAward(player,clientaward,monNum)
	if type(clientaward) == "table" then
		local record = util.decode(player:get_param(const.PP_OFFLINE_AWARD)) or {}
		local award = record.award or {
			{id = 40000001, name = "经验", num = 0},
			{id = 40000003, name = "绑定金币", num = 0},	
			{id = 40000008, name = "内功经验", num = 0},
			{id = 23000001, name = "玉佩碎片", num = 0}	
		}
		monNum = record.monNum or 0
		award[1].num = award[1].num + clientaward.exp
		award[2].num = award[2].num + clientaward.coinbind

		player:set_param(const.PP_OFFLINE_AWARD,util.encode({
			award = award,monNum = monNum
		}))
	end
end

-- print("---",string.find("双倍小122时","小")) --23
-- print("2222",string.find("双倍","双"))