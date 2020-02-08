module(..., package.seeall)

--暗殿一层
--upGap-upGap5:开区第一天到第五天每天的BOSS死亡时间刷新不同
local bossTable = {
	[1] ={bossName="暗之骷髅精灵" , upGap=360, upGap2=600, upGap3=720, upGap4=1200, upGap5=1800, tag=1, dieTime=0, upTime=0, x=40,  y=135},
	[2] ={bossName="暗之娜迦" ,     upGap=360, upGap2=600, upGap3=720, upGap4=1200, upGap5=1800, tag=2, dieTime=0, upTime=0, x=57,  y=100},
	[3] ={bossName="暗之触龙邪神" , upGap=360, upGap2=600, upGap3=720, upGap4=1200, upGap5=1800, tag=3, dieTime=0, upTime=0, x=67, y=164},
	[4] ={bossName="暗之荒漠霸主",  upGap=360, upGap2=600, upGap3=720, upGap4=1200, upGap5=1800, tag=4, dieTime=0, upTime=0, x=82,  y=30},
	[5] ={bossName="暗之血炼猪魔" , upGap=360, upGap2=600, upGap3=720, upGap4=1200, upGap5=1800, tag=5, dieTime=0, upTime=0, x=37, y=68},
	[6] ={bossName="暗之沃玛教主" , upGap=420, upGap2=720, upGap3=900, upGap4=1800, upGap5=3600, tag=6, dieTime=0, upTime=0, x=87,  y=138},
	[7] ={bossName="暗之祖玛教主" , upGap=420, upGap2=720, upGap3=900, upGap4=1800, upGap5=3600, tag=7, dieTime=0, upTime=0, x=112,  y=38},
	[8] ={bossName="暗之牛魔王" ,   upGap=420, upGap2=720, upGap3=900, upGap4=1800, upGap5=3600, tag=8, dieTime=0, upTime=0, x=15, y=109},
	[9] ={bossName="暗之双头蛇王" , upGap=420, upGap2=720, upGap3=900, upGap4=1800, upGap5=3600, tag=9, dieTime=0, upTime=0, x=117, y=73},
	[10]={bossName="暗之冰封魔王" , upGap=420, upGap2=720, upGap3=900, upGap4=1800, upGap5=3600, tag=10, dieTime=0, upTime=0, x=84,  y=67},
}

-- function updateCurMapBoss(player)
-- 	local mapId = player:get_map():get_id()
-- 	if mapId=="v200" then
-- 		updateData(player)
-- 	elseif mapId=="v401" then
-- 		updateData2(player)
-- 	elseif mapId=="v402" then
-- 		updateData3(player)
-- 	end
-- end

local bossAndDirectFly = {
	--boss之家
	[4002012] = {
		6005021,6005022,6005023,6005024,6005025,6005026,6005027,6005028,6005029,6005030,
		6006001,6006002,6006003,6006004,6006005,
		6007001,6007002,6007003,
	},
	[4002019] = {
		6011019,6011020,6011021,6011022,6011023,6011024,6011025,6011026,6011027,6011028,
		6011029,6011030,6011031,6011032,6011033,6011034,6011035,6011036,6011037,6011038,
		6011039,6011040,6011041,6011042,6011043,6011044,6011045,6011046,6011047,6011048,
		6011049,6011050,6011051,6011052,6011053,6011054,6011055,6011056,6011057,6011058,
		6011059,6011060,6011061,6011062,6011063,6011064,6011065,6011066,6011067,6011068,
	},
	[4002020] = {
		6008001,6008002,6008003,6008004,6008005,6008006,6008007,
	},
	[4002024] = {
		6001010,6001011,6001012,
		6002005,6002006,
		6003004,6003005,6003006,
		6004007,6004008,6004009,
	},
}

function flyToMon(player, monId)
	for k,v in pairs(bossAndDirectFly) do
		if table.indexof(v, monId) then
			return posnpc.fly(player, k);
		end
	end
	gui.PanelBoss.flyToBoss(player, monId);
end

------------------------------暗殿一层左侧tip----------------------------------------------
--暗殿BOSS死亡记录其死亡时间
function setBossInfo(mon2,map,player)
	local monType = "mon";
	for i=1,#bossTable do
		if string.find(mon2:get_name(),bossTable[i].bossName) then
			map:set_var("eightBoss"..i,os.time())
			map:map_exe("mon.bossRefresh.updateData")
			monType = "boss"
			-- player:set_param(const.PP_ANDIAN_KILL_BOSS_NUM,1)
			-- map:map_exe("mon.bossRefresh.killBossAward")
			break
		end
	end
end

--进入地图时检测下该地图的boss存活情况
function updateBossTime(map)
	if not map then return end
	for i=1,#bossTable do
		local itemData = bossTable[i]
		if map:num_monster(itemData.bossName)>0 then
			map:set_var("eightBoss"..i,"")
		end
	end
end

function updateData(player)
	local map = player:get_map()
	if not map then return end
	local result = {}
	for i=1,#bossTable do
		if tonumber(map:get_var("eightBoss"..i) or 0)~=0 then
			bossTable[i].dieTime = tonumber(map:get_var("eightBoss"..i) or 0)
		else
			bossTable[i].dieTime = 0
		end
		local flag = 0
		local dieTime = tonumber(map:get_var("eightBoss"..i) or 0)
		local serverDay = server_start_day()+1
		if serverDay>=5 then
			flag = dieTime+bossTable[i].upGap5-tonumber(os.time())
		elseif serverDay>=4 then
			flag = dieTime+bossTable[i].upGap4-tonumber(os.time())
		elseif serverDay>=3 then
			flag = dieTime+bossTable[i].upGap3-tonumber(os.time())
		elseif serverDay>=2 then
			flag = dieTime+bossTable[i].upGap2-tonumber(os.time())
		else
			flag = dieTime+bossTable[i].upGap-tonumber(os.time())
		end
		if flag>0 and dieTime~=0 then
			bossTable[i].upTime = flag--倒计时中
		else
			bossTable[i].upTime = 0--可击杀
		end
		result[i]={}
		result[i].bossName=bossTable[i].bossName
		result[i].upTime=bossTable[i].upTime
		result[i].index=bossTable[i].tag
	end
	player:push_lua_table("weiZhiAnDian",util.encode(result))
end

function onPanelData(player,commond)
	local data = util.decode(commond)
	if data.actionid=="reqGotoTarget" then
		gotoTarget(player,data.index)
	elseif data.actionid == "flyToMon" then
		flyToMon(player, data.monId);
		-- print("/////////////flyToMon////////////", data.monId)
	end
end

function gotoTarget(player,index)
	local itemData = bossTable[index]
	if itemData then
		local map = player:get_map()
		player:find_road_goto(map:get_id(),itemData.x,itemData.y,"autofightstart")
	end
end

ckpanel.add_listener("mon.bossRefresh.onPanelData",mon.bossRefresh.onPanelData);

--关闭暗殿BOSS信息面板
function closePanel()

end


--局部变量，所有玩家通用
local freshBossTips, monsterId

function showRefreshBoss(player)
	-- print("//////////////showRefreshBoss//////////////", monsterId)
	if freshBossTips and monsterId then
		player:push_lua_table("showRefreshBoss",util.encode({
			tips = freshBossTips,
			monId = monsterId
		}));
	end
end

--激战boss
function onRelive(mon)
	-- tab.Boss.onBossRelive(mon)
	local map = mon:get_map();
	if map then
		local name = mon:get_name();
		freshBossTips = "<font color='#ff0000'>BOSS "..name.."</font>出现在<font color='#1eb8ff'>"..map:get_name().."</font>";
		monsterId = mon:get_def_id();
		server.info(10, 1, "<font color='#ff0000'>"..name.."</font>出现在<font color='#1eb8ff'>"..map:get_name().."</font>");
		server.server_exe("mon.bossRefresh.showRefreshBoss");
	else
		freshBossTips = nil;
		monsterId = nil
	end
end