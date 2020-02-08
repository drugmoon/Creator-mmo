module(..., package.seeall)

bossPersonal = {
	[1] ={index=1,monName="触龙邪神",taskMonId=12004003,monId=5001004,monLev=60,killJf=100,monImg=31599,mapName="v222",limitTime=30,namePre="fbgr1c",min=1,max=10,times=1,x=35,y=32,
			con=const.PP_BOSS_FUBEN1,mapId=4020001,
			drop={11106031,12106031,13106031,11106041,12106041,13106041},
			awards={[1]={name="经验",id=40000001,num=500000}},
			minMon={
				[1]={name="邪恶钳虫",id=5001001,num=1,x=32,y=33,range=5},
				[2]={name="钳虫",    id=5001002,num=1,x=33,y=35,range=5},
				[3]={name="剧毒蜈蚣",id=5001003,num=1,x=36,y=36,range=5},
			},
			condition={level=62,zlevel=0,mateId=0,mateNum=0},--副本进入条件
	},
	[2] ={index=2,monName="荒漠霸主",taskMonId=12004004,monId=5002004,monLev=70,killJf=200,monImg=31153,mapName="v232",limitTime=30,namePre="fbgr2c",min=51,max=60,times=1,x=23,y=33,
			con=const.PP_BOSS_FUBEN2,mapId=4020002,
			drop={11107031,12107031,13107031,11107041,12107041,13107041},
			awards={[1]={name="经验",id=40000001,num=500000}},
			minMon={
				[1]={name="沙漠苍狼",  id=5002001,num=1,x=20,y=35,range=5},
				[2]={name="穿山甲",    id=5002002,num=1,x=22,y=37,range=5},
				[3]={name="黄沙统领",  id=5002003,num=1,x=24,y=37,range=5},
			},
			condition={level=62,zlevel=0,mateId=0,mateNum=0},--副本进入条件
	},
	[3] ={index=3,monName="血炼猪魔",monId=5003004,monLev=80,killJf=300,monImg=31042,mapName="v242",limitTime=30,namePre="fbgr3c",min=101,max=110,times=1,x=24,y=30,
			con=const.PP_BOSS_FUBEN3,mapId=4020003,
			drop={11108031,12108031,13108031,11108041,12108041,13108041},
			awards={[1]={name="经验",id=40000001,num=500000}},
			minMon={
				[1]={name="黑野猪",  id=5003001,num=1,x=22,y=31,range=5},
				[2]={name="蛇蝎",    id=5003002,num=1,x=24,y=33,range=5},
				[3]={name="血炼猪卫",id=5003003,num=1,x=22,y=33,range=5},
			},
			condition={level=63,zlevel=0,mateId=0,mateNum=0},--副本进入条件
	},
	[4] ={index=4,monName="沃玛教主",monId=5004004,monLev=100,killJf=400,monImg=31096,mapName="v252",limitTime=30,namePre="fbgr4c",min=151,max=160,times=1,x=21,y=36,
			con=const.PP_BOSS_FUBEN4,mapId=4020004,
			drop={11110031,12110031,13110031,11110041,12110041,13110041},
			awards={[1]={name="经验",id=40000001,num=1000000}},
			minMon={
				[1]={name="沃玛卫士",id=5004001,num=1,x=21,y=34,range=5},
				[2]={name="沃玛守卫",id=5004002,num=1,x=20,y=32,range=5},
				[3]={name="火焰沃玛",id=5004003,num=1,x=26,y=35,range=5},
			},
			condition={level=80,zlevel=0,mateId=32000001,mateNum=1},--副本进入条件
	},
	[5] ={index=5,monName="骨魔教主",monId=5005004,monLev=110,killJf=500,monImg=31071,mapName="v292",limitTime=30,namePre="fbgr5c",min=201,max=210,times=1,x=17,y=36,
			con=const.PP_BOSS_FUBEN5,mapId=4020005,
			drop={11112031,12112031,13112031,11112041,12112041,13112041},
			awards={[1]={name="经验",id=40000001,num=1000000}},
			minMon={
				[1]={name="白骨锤兵",id=5005001,num=1,x=20,y=37,range=5},
				[2]={name="烈焰亲卫",id=5005002,num=1,x=16,y=33,range=5},
				[3]={name="白骨队长",id=5005003,num=1,x=20,y=35,range=5},
			},
			condition={level=0,zlevel=3,mateId=32000001,mateNum=2},--副本进入条件
	},
	[6] ={index=6,monName="火龙王",monId=5006004,monLev=130,killJf=600,monImg=31412,mapName="v332",limitTime=30,namePre="fbgr6c",min=251,max=260,times=1,x=33,y=15,
			con=const.PP_BOSS_FUBEN6,mapId=4020006,
			drop={11114031,12114031,13114031,11114041,12114041,13114041},
			awards={[1]={name="经验",id=40000001,num=1000000}},
			minMon={
				[1]={name="金杖蜘蛛",id=5006001,num=1,x=31,y=15,range=5},
				[2]={name="圣殿卫士",id=5006002,num=1,x=33,y=18,range=5},
				[3]={name="巨镰蜘蛛",id=5006003,num=1,x=36,y=19,range=5},
			},
			condition={level=0,zlevel=5,mateId=32000001,mateNum=3},--副本进入条件
	},
}

--[[
	激战boss
		玛雅神殿，boss之家，野外boss
		玛雅神殿和boss之家传送到npc，野外boss传送到地图第一层
]]

--map1:野外 map2:boss之家 map3:玛雅神殿
--dropId：掉落组 到 简单掉落表找对应显示
--{"kuangdong3",0,"矿洞",4010005}=={boss刷新地图,是否存活,野外地图名,传送到野外一层}
WildBoss = {
	{monName = "尸王",		index=1,  monLev = 50 , modelId = 0, monImg=31016, freshTime = 780000,   dropId=20001, drop = {}, map1 ={"kuangdong3",     0,"矿洞"    ,4010005}, map2={"bosshome1f",0}, map3={nil,0} },
	{monName = "娜迦",		index=2,  monLev = 55 , modelId = 0, monImg=31021, freshTime = 780000,   dropId=20003, drop = {}, map1 ={"dusheshangu3",   0,"毒蛇山谷",4010006}, map2={"bosshome1f",0}, map3={"mysd1f",0} },
	{monName = "触龙邪神",	index=3,  monLev = 60 , modelId = 0, monImg=31599, freshTime = 1680000,  dropId=20004, drop = {}, map1 ={"siwangshangu3",  0,"死亡山谷",4010007}, map2={nil,0         }, map3={"mysd1f",0} },
	{monName = "荒漠霸主",	index=4,  monLev = 70 , modelId = 0, monImg=31153, freshTime = 1680000,  dropId=20005, drop = {}, map1 ={"shamo3",         0,"沙漠"    ,4010008}, map2={"bosshome1f",0}, map3={"mysd1f",0} },
	{monName = "血炼猪魔",	index=5,  monLev = 80 , modelId = 0, monImg=31042, freshTime = 1680000,  dropId=20006, drop = {}, map1 ={"zhudong3",       0,"猪洞"    ,4010009}, map2={"bosshome1f",0}, map3={"mysd1f",0} },
	{monName = "沃玛教主",	index=6,  monLev = 85 , modelId = 0, monImg=31096, freshTime = 3480000,  dropId=20008, drop = {}, map1 ={"womasimiao3",    0,"沃玛寺庙",4010010}, map2={"bosshome1f",0}, map3={"mysd2f",0} },
	{monName = "祖玛教主",	index=7,  monLev = 90 , modelId = 0, monImg=31091, freshTime = 5280000,  dropId=20009, drop = {}, map1 ={"zumasimiao3",    0,"祖玛寺庙",4010011}, map2={"bosshome1f",0}, map3={nil,0} },
	{monName = "赤月老魔",	index=8,  monLev = 100, modelId = 0, monImg=31066, freshTime = 7080000,  dropId=20009, drop = {}, map1 ={"chiyue3",        0,"赤月"    ,4010012}, map2={"bosshome1f",0}, map3={"mysd2f",0} },
	{monName = "牛魔王",	index=9,  monLev = 105, modelId = 0, monImg=31123, freshTime = 7080000,  dropId=20009, drop = {}, map1 ={"niumosimiao3",   0,"牛魔寺庙",4010013}, map2={"bosshome1f",0}, map3={nil,0} },
	{monName = "骨魔教主",	index=10, monLev = 110, modelId = 0, monImg=31071, freshTime = 8880000,  dropId=20010, drop = {}, map1 ={"diyulaolong3",   0,"地狱牢笼",4010026}, map2={nil,0         }, map3={nil,0} },
--	{monName = "冰封狮王",	index=11, monLev = 120, modelId = 0, monImg=31102, freshTime = 8880000,  dropId=20010, drop = {}, map1 ={nil,              0                   }, map2={nil,0         }, map3={"mysd3f",0} },
--	{monName = "冰宫祭祀",	index=12, monLev = 120, modelId = 0, monImg=31083, freshTime = 8880000,  dropId=20010, drop = {}, map1 ={nil,              0                   }, map2={nil,0         }, map3={"mysd3f",0} },
	{monName = "冰封魔王",	index=11, monLev = 120, modelId = 0, monImg=31084, freshTime = 8880000,  dropId=20010, drop = {}, map1 ={"shourenchaoxue3",0,"兽人巢穴",4010014}, map2={nil,0         }, map3={"mysd3f",0} },
	{monName = "幽灵教主",		index=12, monLev = 130, modelId = 0, monImg=31077, freshTime = 8880000,  dropId=20010, drop = {}, map1 ={"mayidongxue3",   0,"蚂蚁洞穴",4010015}, map2={"bosshome1f",0}, map3={nil,0} },
	{monName = "蛮荒巨兽",	index=13, monLev = 140, modelId = 0, monImg=31546, freshTime = 10680000, dropId=20011, drop = {}, map1 ={"manhuangdigong3",0,"蛮荒地宫",4010016}, map2={"bosshome2f",0}, map3={nil,0} },
	{monName = "火龙王",	index=14, monLev = 150, modelId = 0, monImg=31412, freshTime = 10680000, dropId=20012, drop = {}, map1 ={"leiyan3",        0,"雷炎"    ,4010017}, map2={"bosshome2f",0}, map3={nil,0} },
	{monName = "九尾妖狐",	index=15, monLev = 150, modelId = 0, monImg=31115, freshTime = 10680000, dropId=20012, drop = {}, map1 ={"miwuzhaoze3",    0,"迷雾沼泽",4010018}, map2={nil,0         }, map3={"mysd4f",0} },
	{monName = "至尊帝皇",	index=16, monLev = 160, modelId = 0, monImg=20014, freshTime = 10680000, dropId=20012, drop = {}, map1 ={"huangling3",     0,"皇陵"    ,4010019}, map2={"bosshome1f",0}, map3={"mysd4f",0} },
	{monName = "巫妖王",	index=17, monLev = 160, modelId = 0, monImg=31566, freshTime = 10680000, dropId=20012, drop = {}, map1 ={nil,              0                   }, map2={"bosshome2f",0}, map3={nil,0} },
	{monName = "深海巨魔",	index=18, monLev = 160, modelId = 0, monImg=31567, freshTime = 10680000, dropId=20012, drop = {}, map1 ={nil,              0                   }, map2={"bosshome2f",0}, map3={nil,0} },
	{monName = "深海幽龙王",	index=19, monLev = 170, modelId = 0, monImg=31568, freshTime = 10680000, dropId=20012, drop = {}, map1 ={"haidi3",         0,"海底"    ,2020013}, map2={"bosshome2f",0}, map3={"mysd4f",0} },
	{monName = "嗜魔神",	index=20, monLev = 180, modelId = 0, monImg=31110, freshTime = 14280000, dropId=20013, drop = {}, map1 ={nil,              0                   }, map2={"bosshome3f",0}, map3={nil,0} },
	{monName = "幽冥至尊",	index=21, monLev = 180, modelId = 0, monImg=31134, freshTime = 14280000, dropId=20013, drop = {}, map1 ={nil,              0                   }, map2={"bosshome3f",0}, map3={nil,0} },
	{monName = "冰火帝王",	index=22, monLev = 200, modelId = 0, monImg=31547, freshTime = 21480000, dropId=20013, drop = {}, map1 ={nil,              0                   }, map2={"bosshome3f",0}, map3={nil,0} },
	-- {monName = "蛮荒飞蛾",	index=25, monLev = 140, modelId = 0, monImg=65025, freshTime = 10680000, dropId=20012, drop = {}, map1 ={nil,0},               map2={nil,0},          map3={nil,0} },
	-- {monName = "蛮荒力士",	index=26, monLev = 160, modelId = 0, monImg=65026, freshTime = 10680000, dropId=20012, drop = {}, map1 ={nil,0},               map2={nil,0},          map3={nil,0} },
	-- {monName = "蛮荒战将",	index=27, monLev = 160, modelId = 0, monImg=65027, freshTime = 10680000, dropId=20012, drop = {}, map1 ={nil,0},               map2={nil,0},          map3={nil,0} },
	-- {monName = "蛮荒教主",	index=28, monLev = 200, modelId = 0, monImg=65028, freshTime = 21480000, dropId=20012, drop = {}, map1 ={nil,0},               map2={nil,0},          map3={nil,0} },
}

local bosshome = {"bosshome1f", "bosshome2f", "bosshome3f", }
local yama = {"mysd1f", "mysd2f", "mysd3f", "mysd4f", }

--设置野外boss掉落信息
function setDropInfo()
	for i=1,#WildBoss do
		local itemData = WildBoss[i]
		local dropData = tab.simpleDrop[tostring(itemData.dropId)]
		if dropData then
			itemData.drop={dropData.drop1,dropData.drop2,dropData.drop3,dropData.drop4,dropData.drop5,dropData.drop6}
		end
	end
end

--取野外BOSS表数据



-- local init = false
-- --加mapIds
-- for k,v in pairs(WildBoss) do
-- 	for m,n in pairs(tab.mongen) do
-- 		local idx = table.indexof(v.ids,n.monId)
-- 		if idx and not table.indexof(v.mapIds,n.mapId) then
-- 			table.insert(WildBoss[k].mapIds,n.mapId)

-- 		end
-- 	end
-- 	WildBoss[k].monLev = tab.getMonLevel(v.ids[1])
-- 	WildBoss[k].dropItems = tab.getMonDropItems(v.ids[1])
-- 	local monConf = tab.getMonConf(v.ids[1])
-- 	if monConf then
-- 		WildBoss[k].modelId = monConf.model
-- 		WildBoss[k].freshTime = monConf.refreshtime
-- 	end
-- 	-- print(v.ids[1],util.encode(WildBoss[k].itemIds))
-- end
-- local function getMaps(mapIds)
-- 	local t ={}
-- 	for k,v in pairs(mapIds) do
-- 		table.insert(t,tab.getMapName(v))
-- 	end
-- 	return t
-- end
-- function getBossData(alive)
-- 	local bossData = {}
-- 	for k,v in pairs(WildBoss) do
-- 		if not init then
-- 			for _,mapId in pairs(v.mapIds) do
-- 				map = server.find_map(mapId)
-- 				WildBoss[k].exist = WildBoss[k].exist or {}
-- 				if map then
-- 					local bossNum = map:num_monster(v.name)
-- 					print(v.name,mapId,bossNum)
-- 					if bossNum>0 and not table.indexof(WildBoss[k].exist,mapId) then
-- 						table.insert(WildBoss[k].exist,mapId)
-- 					end
-- 				end
-- 			end
-- 			init = true
-- 		end
-- 		if not alive or table.nums(WildBoss[k].exist)>0 then

-- 			table.insert(bossData,{
-- 				monName = v.name,
-- 				monLev = v.monLev,
-- 				drop = v.dropItems,
-- 				maps = WildBoss[k].exist,
-- 				monImg = WildBoss[k].modelId,
-- 				mapName = getMaps(WildBoss[k].exist),
-- 				freshTime = WildBoss[k].freshTime,
-- 			})
-- 		end
-- 	end
-- 	return bossData
-- end

function onBossRelive(mon)
-- 	local monName = mon:get_name()
-- 	local monMap = mon:get_map()
-- 	for k,v in pairs(WildBoss) do
-- 		if v.name == monName and not table.indexof(v.exist,monMap) then
-- 			table.insert(WildBoss[k].exist,monMap)

-- 			break
-- 		end
-- 	end
end

function onBossDead(mon,player)
-- 	local monName = mon:get_name()
-- 	local monMap = mon:get_map()
-- 	for k,v in pairs(WildBoss) do
-- 		if v.name == monName then
-- 			WildBoss[k].exist = table.removeByValue(v.exist,monMap,true)

-- 			break
-- 		end
-- 	end
end