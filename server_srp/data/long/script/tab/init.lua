module(..., package.seeall)
local check = {
	["int"] = function(s) return tonumber(s,10) or 0 end,
	["int16"] = function(s) return tonumber(s,16) or 0 end,
	["bool"] = function(s) return s~="0" and s~="false" and s~="" and s~=nil and true or false; end,
	["string"] = function(s) return tostring(s) or "" end,
	["table_"] = function(s) return string.split(s,"_") end,
	["string[]"] = function(s) return string.split(s,";") end,
	["int[]"] = function(s) local ss=string.split(s,";"); table.map(ss,function(v,k) return checknumber(v) end);  return ss;end,
}

function parseCsvFile(data,keys,types)
	local result = {}
	if not data or string.len(data)<=0 then 
		return result 
	end
	types = types or {}
	local lines = string.split(data,'\r\n')
	local l = #lines
	local v,f,s,d,ls,lk,vt
	local k = false
	if keys then
		if type(keys) == "table" and #keys >0 then
			k = true
			lk = #keys
		end
	end
	for i=1,l do
		v = lines[i]
		if string.len(v)>0 then
			f = string.sub(v,1,1)
			if f ~= '#' and f ~= ',' and f ~= ' ' then
				s = string.split(v,',')
				ls = #s
				if s and ls >0 then
					if not k then
						result[s[1]]=s
					else
						vt = {}
						for j=1,lk do
							vt[keys[j]] = not types[j] and s[j] or check[types[j]](s[j])
						end
						result[s[1]]= vt
					end
				end
			end
		end
	end

	return result
end

function parseTxtFile(data,keys,types)
	local result = {}
	if not data or string.len(data)<=0 then 
		return result 
	end
	types = types or {}
	local lines = string.split(data,'\r\n')
	local l = #lines
	local v,f,s,d,ls,lk,vt
	local k = false
	lk = #keys
	for i=1,l do
		v = lines[i]
		if string.len(v)>0 then
			f = string.sub(v,1,1)
			if f ~= '#' and f ~= ',' and f ~= ' ' then
				s = string.split(v,' ')
				ls = #s
				if s and ls >0 then
					vt = {}
					for j=1,lk do
						vt[keys[j]] = not types[j] and s[j] or check[types[j]](s[j])
					end
					table.insert(result,vt)
				end
			end
		end
	end
	return result
end

local keys = {"id","name","script","npcshop","mapid","x","y","model","dir","flyid"}
local types = {}
NpcGen=parseCsvFile(server.load_csv_file("npcgen.csv"),keys)

keys = {"id","desp","name","destid","destx","desty","destsize","type","show","srcid","srcx","srcy","monlv"}
DirectFly=parseCsvFile(server.load_csv_file("directfly.csv"),keys)
																																													
-- ID	等级	升级所需经验	兑换转生值	兑换声望值	最大血量（战士）	最大魔力（战士）	最小物攻（战士）	最大物攻（战士）	最小法攻（战士）	最大法攻（战士）	最小道攻（战士）	最大道攻（战士）	最小物防（战士）	最大物防（战士）	最小法防（战士）	最大法防（战士）	准确值（战士）	闪避值（战士）	最大血量（法师）	最大魔力（法师）	最小物攻（法师）	最大物攻（法师）	最小法攻（法师）	最大法攻（法师）	最小道攻（法师）	最大道攻（法师）	最小物防（法师）	最大物防（法师）	最小法防（法师）	最大法防（法师）	准确值（法师）	闪避值（法师）	最大血量（道士）	最大魔力（道士）	最小物攻（道士）	最大物攻（道士）	最小法攻（道士）	最大法攻（道士）	最小道攻（道士）	最大道攻（道士）	最小物防（道士）	最大物防（道士）	最小法防（道士）	最大法防（道士）	准确值（道士）	闪避值（道士）
-- int	int	long	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	int

keys = {"id","level","needexp","zhuangsheng","shengwang",
	"zs_hpmax","zs_mpmax","zs_dc","zs_dcmax","zs_mc","zs_mcmax","zs_sc","zs_scmax","zs_ac","zs_acmax","zs_mac","zs_macmax","zs_accuracy","zs_dodge",
	"fs_hpmax","fs_mpmax","fs_dc","fs_dcmax","fs_mc","fs_mcmax","fs_sc","fs_scmax","fs_ac","fs_acmax","fs_mac","fs_macmax","fs_accuracy","fs_dodge",
	"ds_hpmax","ds_mpmax","ds_dc","ds_dcmax","ds_mc","ds_mcmax","ds_sc","ds_scmax","ds_ac","ds_acmax","ds_mac","ds_macmax","ds_accuracy","ds_dodge",
}
types = {"int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int"}
local temp=parseCsvFile(server.load_csv_file("levelinfo.csv"),keys,types)
ZsAndSw = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		ZsAndSw[k] = v
	end
end
--简单掉落表
keys = {"id","drop1","drop2","drop3","drop4","drop5","drop6"}
simpleDrop=parseCsvFile(server.load_csv_file("simpleDrop.csv"),keys)

--护卫升级表
keys = {"id","level","needexp","maxhp","wgmin","wgmax","wfmin","wfmax","mfmin","mfmax","huixue"}
types = {"int","int","int","int","int","int","int","int","int","int","int"}
HuWeiLevel=parseCsvFile(server.load_csv_file("huweilevel.csv"),keys,types)
function getGuardInfo(lv)
	return HuWeiLevel[tostring(lv)]
end

keys = {"id","position","type","itemBaseId","itemNum","MoneyKind","money","bind","maxCount","startTime","endTime","startLv","endLv","quickBuyType","quickBuyNum","notShowInStore","name"}
types ={"int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","bool","string"}
temp=parseCsvFile(server.load_csv_file("store.csv"),keys,types)
store = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		store[k] = v
	end
end
keys={"id","lv","item","priceid","num","pro","blsee","icoin","nid","wid","min","max","blessMin","blseemax","maxbless","info","mindc","maxdc","minmc","maxmc","minsc","maxsc","minac","maxac","minmac","maxmac"}	
types = {"int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","string","int","int","int","int","int","int","int","int","int","int","int[]","int[]"}
Key_Wing=parseCsvFile(server.load_csv_file("wingdef.csv"),keys,types)
function getWingDefByLv(lv)
	return Key_Wing[tostring(lv+1)]
end
--[[
#地图编号	地图文件	地图名字	小地图编号	脚本触发	1主城2野外3副本	0独立1公共2个人		名字后面加1-200	回城点	复活点	进入最低等级	进入最低转生	进入离开绑定脚本	任务共享	允许穿怪	允许穿人	禁止PK	允许PK(不加PK值)	保留位置	死亡不掉东西	不保护新手	禁止传送	禁止扔东西	屏蔽玩家名字，禁止聊天，禁止查看角色信息	禁止坐骑	禁止攻击和使用技能	游荡挂机	禁止战神进入	自动复活	禁止原地复活	匿名	禁止使用某个物品	禁止使用某个技能
#代表本行注释					#type	副本类型	副本承载上限	副本复用数量			#minLv	#minLvR	#trigerenterleave	#taskshare	#runmon	#runplayer	#pkprohibit	#pkallow	#autoenter	#nodiedropitem	#noprotectnewer	#nodirectfly	#nodropitem	#nointeract	#nomount	#lockaction	#wanderhangup	#nogod	#autoalive	#norelivestand	#anonym	di	ds
]]

keys = {"mapid", "file", "mapname","minimap", "script", "mapType", "fubentype", "homepos", "relivepos", "maxplayer", "copynum", "minLv", "minLvR", "trigerenterleave", 
	"taskshare", "runmon", "runplayer", "pkprohibit", "pkallow", "autoenter", "nodiedropitem", "noprotectnewer", "nodirectfly", "nodropitem", "nointeract",
		"nomount", "lockaction", "wanderhangup", "nogod", "autoalive", "norelivestand", "anonym","nocall","noEnterCopy"}
types ={"string","string","string","int","string","int","int","int","int","int","int","int","int","int","int","int","int"
	,"int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int"}
-- print(#keys,#types)
MapInfo=parseCsvFile(server.load_csv_file("mapinfo.csv"),keys,types)

function getMapName(mapId)
	if mapId and MapInfo[mapId] then
		return MapInfo[mapId].mapname or ""
	end
	return ""
end
function isMapCallLegal(mapId)
	if MapInfo[mapId] then
		return MapInfo[mapId].nocall==0
	end
end
function isMapEnterLegal(mapId)
	if MapInfo[mapId] then
		return MapInfo[mapId].noEnterCopy==0
	end
end
function getMapEnterLevel(player,mapId)
	if MapInfo[mapId] then
		return player:get_level()>= MapInfo[mapId].minLv and util.ppn(player, const.PP_ZHSH_LEVEL)>=MapInfo[mapId].minLvR
	end
	return false
end


keys ={"id","dropId","itemId",};
types ={"int","int","int",};
dropplus = {};
local tempdropplus=parseCsvFile(server.load_csv_file("dropplus.csv"),keys,types);
for k,v in pairs(tempdropplus) do
	if v.dropId then
		dropplus[v.dropId] = dropplus[v.dropId] or {}
		table.insert(dropplus[v.dropId],v.itemId)
	end
end
tempdropplus = nil

keys ={"id","dropId","minCoin","maxCoin","itemId","dropPlusId","probability","bind","ownerTime"};
types ={"int","int","int","string","int","int","int","int"};
mondrop = {};
local tempmondrop=parseCsvFile(server.load_csv_file("mondrop.csv"),keys,types);
for k,v in pairs(tempmondrop) do
	if tonumber(k) then
		-- v.dropItemIds = v.dropItemIds or {}
		-- if v.dropPlusId >0 and dropplus[v.dropPlusId] then
		-- 	for i,id in pairs(dropplus[v.dropPlusId]) do
		-- 		table.insert(v.dropItemIds,id)
		-- 	end
		-- end
		mondrop[tonumber(k)] = v
	end
end
tempmondrop = nil
--[[
keys = {"mapId","x","y","monId","range","num","reliveTime","config"}
types = {"string","int","int","int","int","int","int","string"}
mongen = parseTxtFile(server.load_csv_file("mongen.txt"),keys,types)
]]

--[[#物品编号	类型	男(外)观	女(外)观	图标编号	触发脚本	名字	颜色	负重	装备类别	
最小物防	最大物防	最小魔防	最大魔防	最小物攻	最大物攻	最小魔攻	
最大魔攻	最小道攻	最大道攻	幸运	诅咒	命中	闪避	魔法闪避	中毒闪避	
HP上限	MP上限	HP上限百分比	MP上限百分比	神圣攻击	暴击几率	暴击力	掉落守护	
限制类型	等级限制	出售价格	转生等级限制	品质	职业	性别	装备组	装备对比	
帮会捐献值	摧毁提示	装备内功	装备背景	回收经验	回收玉佩	能否销毁	能否存入仓库	能否推送	功能配置	功能参数	物品描述
]]
-- #物品编号	类型	图标编号	名字	颜色	负重	装备类别	最小物防	最大物防	最小魔防	最大魔防	最小物攻	最大物攻	最小魔攻	最大魔攻	最小道攻	最大道攻	幸运	诅咒	命中	闪避	魔法闪避	中毒闪避	HP上限	MP上限	HP上限百分比	MP上限百分比	神圣攻击	暴击几率	暴击力	掉落守护	限制类型	等级限制	出售价格	转生等级限制	品质	职业	性别	装备组	装备对比	帮会捐献值	摧毁提示	装备内功	装备背景	回收金币	回收经验	回收血符	是否可使用	能否销毁	能否存入仓库	能否推送	背包是否显示使用标志	限定次数用完之后是否显示推送	功能配置	功能参数	物品描述	获取途径
-- #type_id	sub_type	icon_id	name	color	weight	equip_type	ac(物防)	ac2	mac魔防	mac2	dc（战攻）	dc2	mc（魔攻）	mc2	sc（道攻）	sc2	luck	curse	accuracy	dodge	anti_magic	anti_poison	max_hp	max_mp	max_hp_pres	max_mp_pres	holy_damage	baoji_prob	baoji_pres	drop_luck	need	needlevel	price	need_zslevel	equip_level	job	gender	equip_group	equip_comp	equip_contribute	destory_show	add_power	item_bg	recycle_money	recycle_exp	recycle_xuefu	can_use	can_destroy	can_depot	can_push	bag_show	times_limit	func_id	func_params	desp	source

keys ={
	"type_id","sub_type","icon_id","name","color","weight","equip_type","ac","ac2","mac","mac2","dc","dc2","mc","mc2","sc","sc2","luck","curse","accuracy","dodge","anti_magic","anti_poison","max_hp","max_mp","max_hp_pres","max_mp_pres","holy_damage","baoji_prob","baoji_pres","drop_luck","need","needlevel","price","need_zslevel","equip_level","job","gender","equip_group","equip_comp","equip_contribute","destory_show","add_power","item_bg","recycle_money","recycle_exp","recycle_xuefu","can_use","can_destroy","can_depot","can_push", "bag_show", "times_limit", "func_id","func_params","desp","source"
};
types ={
	"int","int","int","string","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","string","string"
};
itemdef = {};
local tempItemDef=parseCsvFile(server.load_csv_file("itemdef.csv"),keys,types);
for k,v in pairs(tempItemDef) do
	if tonumber(k) then
		itemdef[tonumber(k)] = v
	end
end
-- tempItemDef = nil
function getItemDefByID( id )
	return itemdef[tonumber(id)]
end
keys = {
	"id","model","script","AI","name","effect_type","effect_res","target_effect","lev","zslevel","sight","exp","inside_power_exp","MHP","mp","MinDEF","MaxDEF","MinMDEF","MaxMDEF","MinATK","MaxATK","MinMATK","MaxMATK","DODGE","HIT","minStayTime","maxStayTime","moveTime","interval","head_icon","talkgap","subType","dropID","patrol","maxmove","canPush","switch_target_type","switch_target_interval","hatred_damage_ratio","hatred_distance_ratio","editdir","type","monGroup","teleportID","defaultAI","patrolTime","deadTime","refreshTime","dirFixed","CRIRate","DUCrate","ADTR","MR","HPup","isShowRefresh","isS","bossIntegral","funcid","funcparams","dialog","canSpeak","randomDialog","simpleDropID"
};
types ={
	"int","int","string","int","string","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","bool","int","int","int","int","string","int","int","int","string","int","int","int","int","int","int","int","int","int","bool","bool","int","string","string","string","bool","string[]","int"
};
local tempMondef=parseCsvFile(server.load_csv_file("mondef.csv"),keys,types);
mondefCsv = {};
name2Id = {};
for k,v in pairs(tempMondef) do
	if tonumber(k) then
		mondefCsv[tonumber(k)] = v

		name2Id[v.name] = v.id
	end
end
-- tempMondef = nil
function checkMonName(name)
	if name2Id[name] then 
		return true
	else
		return false
	end
end
function getMonTelePortID(monID)
	if monID and mondefCsv[monID] then
		return mondefCsv[monID].teleportID or 0;
	end
	return 0;
end

function getMonGroup(monID)
	if monID and mondefCsv[monID] then
		return mondefCsv[monID].monGroup or 0;
	end
	return 0;
end
function getMonName(monID)
	if monID and mondefCsv[monID] then
		return mondefCsv[monID].name or "";
	end
	return "";
end

function getMonLevel(monID)
	if monID and mondefCsv[monID] then
		return mondefCsv[monID].lev or 0;
	end
	return 0;
end

function getMonConf(monID)
	if monID and mondefCsv[monID] then
		return mondefCsv[monID]
	end
end

function getMonDropItems(monId)
	local itemIds = {}
	if mondefCsv[monId] then
		local dropId = mondefCsv[monId].dropID
		for k,v in pairs(mondrop) do
			if v.dropId == dropId then
				if v.itemId>0 and not table.indexof(itemIds,v.itemId) then
					table.insert(itemIds,v.itemId)
				end
				-- if v.dropItemIds and #v.dropItemIds>0 then
				-- 	for m,n in pairs(v.dropItemIds) do
				-- 		if not table.indexof(itemIds,v.itemId)  then
				-- 			table.insert(itemIds,n)
				-- 		end
				-- 	end
				-- end
			end
		end
	end
	return itemIds
end

function getMonReliveTime(monId)
	if monId and mondefCsv[monId] then
		return mondefCsv[monId].refreshTime/1000 or 0;
	end
	return 0
end

function getItemName(typeId)
	if itemdef[tonumber(typeId)] then
		return itemdef[tonumber(typeId)].name
	end
	return ""
end

function getItemJob(typeId)
	if itemdef[tonumber(typeId)] then
		return itemdef[tonumber(typeId)].job
	end
	return 0
end
function getItemGender(typeId)
	if itemdef[tonumber(typeId)] then
		return itemdef[tonumber(typeId)].gender
	end
	return 0
end

local qualityColor = {
	[0] = "#fff7ec",
	[1] = "#36de00",
	[2] = "#1eb8ff",
	[3] = "#ff1fec",
	[4] = "#ff0a00",
	[5] = "#fff843",
}

function getItemColor(quality)
	if quality and qualityColor[quality] then
		return qualityColor[quality];
	end
	return qualityColor[1];
end

function getItemColorName(typeId)
	local name = "";
	local id = itemdef[tonumber(typeId)];
	if id then
		local color = getItemColor(id.equip_level);
		if color then
			name = "<font color='"..color.."'>"..id.name.."</font>";
		else
			name = id.name;
		end
	end
	return name;
end

-- print("test to log a item name :", getItemColorName(12114071))

keys = {"itemId","groupTag","times","const"}
types = {"int","int","int","int"}
itemDailyUseLimit = parseCsvFile(server.load_csv_file("itemDailyUseLimit.csv"),keys,types);
--返回每日剩余使用次数
local jinzhuan = {
	{id = 23060001,	name = "金砖（小）"	,	con = const.PP_DAILY_ITEM_LIMIT3},
	{id = 23060002,	name = "金砖（中）"	,	con = const.PP_DAILY_ITEM_LIMIT3},
	{id = 23060003,	name = "金砖（大）"	,	con = const.PP_DAILY_ITEM_LIMIT3},
}
function getItemDailyLimit(player,itemId)
	local Alltimes = 0
	local leftTimes = 999
	if itemDailyUseLimit[tostring(itemId)] then
		if 23060001<=itemId and itemId<=23060003 then
			Alltimes = tab.Vip.getJinZhuanTimes(player)
		else
			Alltimes = itemDailyUseLimit[tostring(itemId)].times
		end
		leftTimes = Alltimes - util.ppn(player,itemDailyUseLimit[tostring(itemId)].const)
	end
	return leftTimes
end

function useItem(player,itemId,num)
	if itemDailyUseLimit[tostring(itemId)] then
		local con = itemDailyUseLimit[tostring(itemId)].const
		player:set_param(con,util.ppn(player,con)+num)
		util.alert_msg(4, "每日剩余使用"..getItemDailyLimit(player,itemId).."次数", player)
		pushItemLimitToClient(player,con)
	end
	if server_merge_day()<=4 and server_merge_day()>=0 then--合区前5天
		--记录狼牙碎片大使用个数
		if itemId==23030003 then 
			player:set_param(const.PP_LANGYA_USEB_NUM,util.ppn(player,const.PP_LANGYA_USEB_NUM)+num)
			gui.moduleRedPoint.checkHeQuAct(player)
		end
		--记录龙心碎片大使用个数
		if itemId==23020003 then 
			player:set_param(const.PP_LONGXIN_USEB_NUM,util.ppn(player,const.PP_LONGXIN_USEB_NUM)+num)
			gui.moduleRedPoint.checkHeQuAct(player)
		end
	end
end

function pushItemLimitToClient(player,con)
	local result = {data={}}
	if not con then
		for k,v in pairs(itemDailyUseLimit) do
			table.insert(result.data,{id = v.itemId,leftTimes = getItemDailyLimit(player,v.itemId),totalTimes = v.times})
		end
		player:push_lua_table("ItemDailyLimit",util.encode(result));
	else
		for k,v in pairs(itemDailyUseLimit) do
			if v.const == con then
				result.data = {
					{id = v.itemId,leftTimes = getItemDailyLimit(player,v.itemId),totalTimes = v.times}
				}
				player:push_lua_table("ItemDailyLimitSingle",util.encode(result));
			end
		end
	end
end

-----------功能开启表-----------
-- 功能ID	功能名称	需求转生等级	需求等级	需求任务id	是否显示推送	显示推送等级	显示推送图标ID
-- funcid	funcname	zslevel	level	taskid	push	pushlevel	pushicon
-- int	string	int	int	int	bool	int	string

keys = {
	"funcid", "funcname", "zslevel", "level", "taskid", "openSvr", "push", "pushlevel", "pushicon"
};
types ={
	"int", "string", "int", "int", "int", "int", "bool", "int", "string"
};

local tempOpenFunc=parseCsvFile(server.load_csv_file("openfunc.csv"),keys,types);

openFuncCsv = {};
for k,v in pairs(tempOpenFunc) do
	if tonumber(k) then
		table.insert(openFuncCsv, v)
	end
end

table.sort(openFuncCsv, function (a, b)
	return a.funcid < b.funcid
end)

--炼魂属性表
keys = {"id", "level", "need", "wgmin", "wgmax", "mgmin", "mgmax", "dgmin", "dgmax","wfmin", "wfmax", "mfmin", "mfmax", "zhanhp","fahp","daohp",}
lianHunCsv=parseCsvFile(server.load_csv_file("lianHunLevel.csv"),keys)

keys = {
	"id", "tid", "ts", "guideIndex", "once","followLv","showicon","funName"
};
types ={
	"int", "int", "int", "int", "int","bool","string","string"
};

local tempGuide=parseCsvFile(server.load_csv_file("guideServer.csv"),keys,types);

guideCsv = {};
for k,v in pairs(tempGuide) do
	if tonumber(k) then
		table.insert(guideCsv, v)
	end
end

table.sort(guideCsv, function (a, b)
	return a.id < b.id
end)
-- print(itemdef["11101081"].id,itemdef["11101081"].exp,itemdef["11101081"].xuefu)
-- print(itemdef["11102011"].id,itemdef["11102011"].exp,itemdef["11102011"].xuefu)

--读取属性枚举表
-- keys = {
-- 	"id", "tid", "ts", "guideIndex", "once"
-- };
-- types ={
-- 	"int", "int", "int", "int", "int"
-- };

-- local tempGuide=parseCsvFile(server.load_csv_file("attrEnumDef.csv"),keys,types);

--读取帮会祈祷表

keys = {
	"id", "level", "skillType", "name", "icon", "contribution", "attrs", "values", "warriorHp", "warriorMp", "wizardHp", "wizardMp", "taoistHp", "taoistMp", "needId",
};
types ={
	"int", "int", "int", "string", "string", "int", "int[]", "int[]", "int", "int", "int", "int", "int", "int", "int",
};

local tempPray=parseCsvFile(server.load_csv_file("guildPray.csv"), keys, types);
guildPray = {};
for k,v in pairs(tempPray) do
	if tonumber(k) then
		guildPray[tonumber(k)] = v
	end
end

-- 编号	功能ID	参数1	参数2	参数3	参数4	参数5	参数6	参数7	参数8	参数9	参数10	参数11	参数12	属性是否重新计算	说明
-- ID	FuncId	Param1	Param2	Param3	Param4	Param5	Param6	Param7	Param8	Param9	Param10	Param11	Param12		desc
-- int	int	int	int	int	int	int	int	int	int	int	int	int	int	int	string

keys = {
	"id", "FuncId", "Param1", "Param2", "Param3", "Param4", "Param5", "Param6", "Param7", "Param8", "Param9", "Param10", "Param11", "Param12", "reCal","desc",
};
types ={
	"int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int","int", "string",
};

local tempfuncs=parseCsvFile(server.load_csv_file("funcsdef.csv"), keys, types);
funcsdef = {};

for k,v in pairs(tempfuncs) do
	if tonumber(k) then
		funcsdef[tonumber(k)] = v
	end
end

keys = {
	"id", "item_id", "lv", "addrate", "stayrate", "reducerate"
};
types ={
	"int", "int", "int", "int", "int","int"
};

local tempWnegInfo=parseCsvFile(server.load_csv_file("wnegInfo.csv"),keys,types);

WnegInfoCsv = {};
for k,v in pairs(tempWnegInfo) do
	if tonumber(k) then
		table.insert(WnegInfoCsv, v)
	end
end

table.sort(WnegInfoCsv, function (a, b)
	return a.id < b.id
end)


keys = {
	"skill_id","name","icon_id","isShow","desp","job","level","need_level","next_skill","use_add_exp","upgrade_need_exp","skill_book","type",
	"consume_hp","consume_mp","skill_cd","public_cd","cast_way","select_target_way","shape","cast_distance_min","cast_distance_max","damage_range",
	"damage_num_max","target_type","target_relation","damage_effect","skill_attack_factor","skill_add_power","defense_damage_factor","trigger_buff1",
	"trigger_buff2","trigger_function1_prob","trigger_function1","trigger_function2_prob","trigger_function2,","param," ,"res_id","effect_type","aoe_action",
	"attack_delay","order_id","damage_desp","range_desp","cd_desp","extra_effect_desp"
};
types ={"int","string","string","bool","string","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int","int[]","int[]","int","int","int","int","int","int","int","int","int","int","int","int","int","bool","int","int","string","string","string","string"};

temp = parseCsvFile(server.load_csv_file("skilldef.csv"), keys, types);
skilldef = {};

local skillMaxLevel = {};
local skillType;
for k,v in pairs(temp) do
	if tonumber(k) then
		skilldef[tonumber(k)] = v
		skillType = math.floor(tonumber(k) / 100)
		if not skillMaxLevel[skillType] then
			skillMaxLevel[skillType] = v.level
		elseif v.level > skillMaxLevel[skillType] then
			skillMaxLevel[skillType] = v.level
		end
	end
end

function getSKillMaxLevel(skillType)
	return skillMaxLevel[skillType] or 10;
end

function getSkillDefById(id)
	return skilldef[id]
end
function getSkillDefWithLv(typ,lv)
	local id = typ*100+lv;
	return skilldef[id];
end


keys = {"typeId","res_male","res_female","script"};
types ={"int","int","int","string"};
temp = parseCsvFile(server.load_csv_file("itemprogram.csv"), keys, types);
itemprogram = {};
for k,v in pairs(temp) do
	if tonumber(k) then
		itemprogram[tonumber(k)] = v
	end
end
function getItemModelId(typeId,gender)
	if itemprogram[typeId] then
		return gender == 200 and itemprogram[typeId].res_male or itemprogram[typeId].res_female
	end
	return 0
end
function getItemScript(typeId)
	if itemprogram[typeId] then
		return itemprogram[typeId].script
	end
	return nil
end

--守护雕像
keys = {
	"ID", "Name", "MapID", "ItemID", "ItemCount", "ItemBindType", "SweepAwardCengID", "Power", "UnitID", "Magic"
};
types ={
	"int", "string", "int", "int", "int", "int", "int", "int", "int", "int"
};

temp = parseCsvFile(server.load_csv_file("defendFirstAward.csv"), keys, types);
defendFirstAward = {};
for k,v in pairs(temp) do
	if tonumber(k) then
		defendFirstAward[tonumber(k)] = v
	end
end

keys = {
	"ID", "Chong", "Guan", "ItemIDTotal", "ItemCountTotal", "ItemBindTypeTotal"
};
types ={
	"int", "int", "int", "int[]", "int[]", "int[]"
};

temp = parseCsvFile(server.load_csv_file("defendNormalAward.csv"), keys, types);
defendNormalAward = {};
local items;
for k,v in pairs(temp) do
	if tonumber(k) then
		items = {};
		for i,id in ipairs(v.ItemIDTotal) do
			if v.ItemCountTotal[i] and v.ItemBindTypeTotal[i] then
				table.insert(items, {id = id, count = v.ItemCountTotal[i], bind = v.ItemBindTypeTotal[i]})
			end
		end
		defendNormalAward[tonumber(k)] = {
			ID = v.ID,
			Chong = v.Chong,
			Guan = v.Guan,
			items = items,
		};
		-- print("////", k, util.encode(defendNormalAward[tonumber(k)]))
	end
end

local function getItemIndexInAward(itemId, awards)
	for i,v in ipairs(awards) do
		if v.id == itemId then
			return i;
		end
	end
end

function getDefendNormalAward(level, waveFrom, waveTo)
	local awards = {};
	if waveFrom > 0 and waveTo > 0 and waveTo >= waveFrom then
		--修改为不累加
		-- local award, index;
		-- local beginIndex = (level - 1) * 10 + waveFrom;
		local endIndex = (level - 1) * 10 + waveTo;
		-- for i = beginIndex, endIndex do
		-- 	award = defendNormalAward[i].items;
		-- 	for j,v in ipairs(award) do
		-- 		index = getItemIndexInAward(v.id, awards);
		-- 		if not index then
		-- 			table.insert(awards, {id = v.id, count = v.count, bind = v.bind});
		-- 		else
		-- 			awards[index].count = awards[index].count + v.count;
		-- 		end
		-- 	end
		-- end
		-- print("getDefendNormalAward", level, waveFrom, waveTo, util.encode(defendNormalAward[endIndex]))
		awards = defendNormalAward[endIndex].items;
	end
	return awards;
end


for i,v in ipairs(defendFirstAward) do
	v.noramlTotalAwards = getDefendNormalAward(i, 1, 10);
	-- print("//////", i, util.encode(v.noramlTotalAwards))
end

keys = {
	"ID", "unitID",
};
types ={
	"int", "int[]",
};

temp = parseCsvFile(server.load_csv_file("defendMonster.csv"), keys, types);
defendMonster = {};

local monsters;
for k,v in pairs(temp) do
	if tonumber(k) then
		monsters = {};
		for j,monId in ipairs(v.unitID) do
			if not monsters[monId] then
				monsters[monId] = 1
			else
				monsters[monId] = monsters[monId] + 1
			end
		end
		defendMonster[tonumber(k)] = monsters
		-- for id,num in pairs(monsters) do
		-- 	print(k, id, num)
		-- end
		-- print("////", k, util.encode(monsters))
	end
end

keys = {
	"id", "posEnum", "level", "posName"
};
types ={
	"int", "int", "int", "string"
};

temp = parseCsvFile(server.load_csv_file("openGem.csv"), keys, types);
openGem = {};

for k,v in pairs(temp) do
	if tonumber(v.level) then
		openGem[tonumber(v.level)] = v;
	end
end

keys = {
	"id", "type1", "type2", "targetid", "bing", "needid1", "neednum1", "needid2", "neednum2", "needid3", "neednum3", "job", "zhuanshen", "level", "starttime","endtime","isShow"
};
types ={
	"int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int","int", "int", "int", "int",
};

hecheng=parseCsvFile(server.load_csv_file("hechengcsv.csv"), keys, types);
--------------精彩活动openfunc
keys={"id","name","open","openweek","openday","start","end","time","sort"}
types={"int","string","int","int", "int[]", "int" ,"int" ,"int" ,"int" }

local  temp=parseCsvFile(server.load_csv_file("bargainActivity.csv"),keys,types)
eventsData = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		eventsData[tonumber(k)] = v
	end
end

--------------精彩活动ljcz
keys={"id","need_num","itemid","itemnum","itembind"}
types={"int","int","int[]","int[]","int[]"}
local temp=parseCsvFile(server.load_csv_file("bargain_recharge.csv"),keys,types)
bargain_recharge = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		bargain_recharge[tonumber(k)] = v
	end
end
--------------精彩活动ly
keys={"id","need_num","itemid","itemnum","itembind"}
types={"int","int","int","int","int"}
local temp=parseCsvFile(server.load_csv_file("bargain_wolfang.csv"),keys,types)
bargain_wolfang = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		bargain_wolfang[tonumber(k)] = v
	end
end
--------------精彩活动lx
keys={"id","need_num","itemid","itemnum","itembind"}
types={"int","int","int","int","int"}
local temp=parseCsvFile(server.load_csv_file("bargain_dragonHeart.csv"),keys,types)
bargain_dragonHeart = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		bargain_dragonHeart[tonumber(k)] = v
	end
end
--------------精彩活动rechagerank
keys={"id","rank","itemid","itemnum","itembind"}
types={"int","int","int[]","int[]","int[]"}
local temp=parseCsvFile(server.load_csv_file("bargain_rechargeRank.csv"),keys,types)
bargain_rechargerank = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		bargain_rechargerank[tonumber(k)] = v
	end
end
--------------精彩活动limitBuy
keys={"id","pos","itemid","itemnum","moneyType","nowMoneyNum","oldMoneyNum","openday","limittimes"}
types={"int","int","int","int","int","int","int","int","int"}
local temp=parseCsvFile(server.load_csv_file("bargain_limitBuy.csv"),keys,types)
bargain_limitBuy = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		bargain_limitBuy[tonumber(k)] = v
	end
end
-- print("/////////", util.encode(openGem));


-- TabBoss = require("tab.TabBoss")
Breakup = require("tab.Breakup")
require("tab.Store")

TaskData1000 = require("tab.TaskData1000")
TaskData1100 = require("tab.TaskData1100")
TaskData2000 = require("tab.TaskData2000")

MixData = require("tab.mixData")
Furnace = require("tab.furnace")
Compose = require("tab.Compose")
Exchange = require("tab.Exchange")
Lottery = require("tab.Lottery")
Achieve = require("tab.Achieve")
Boss = require("tab.Boss")
Vip = require("tab.Vip")

invest = require("tab.invest")
-- menpai = require("tab.menpai")