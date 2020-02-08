module(..., package.seeall)
function use_function(use,base)
	for funcname, func in pairs(base) do
		if string.sub(funcname, 1,6) == "onTalk" then
			use[funcname] = func
		end
	end
end
function ppn(player,id)
	if not id then
		print(debug.traceback())
	end
	local v = player:get_param(id);
	if v == nil or v == "" then return 0;end
	return tonumber(v);
end
function ptpn(player,id)
	local v = player:get_temp_param(id);
	if v == nil or v == "" then return 0;end
	return tonumber(v);
end
function mvn(map,id)
	local v = map:get_var(id);
	if v == nil or v == "" then return 0;end
	return tonumber(v);
end
function svn(id)
	local v = server.get_var(id);
	if v == nil or v == "" then return 0;end
	return tonumber(v);
end
function stvn(id)
	local v = server.get_temp_var(id);
	if v == nil or v == "" then return 0;end
	return tonumber(v);
end

local cjson = server.cjson()

function encode(var)
    local status, result = pcall(cjson.encode, var)
    if status then return result else return "" end
end
function decode(text)
    local status, result = pcall(cjson.decode, text)
    if status then return result else return {} end
end

--[[
@param mixed object 要克隆的值

@return mixed

]]
function clone(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end

function getDate()
	return tonumber(os.date("%d", os.time()))
end

function getWeekday(time)
	time = tonumber(time) or os.time()
	weekDay = tonumber(os.date("%w", time))
	if weekDay == 0 then weekDay = 7 end
	return weekDay
end

--二进制转十进制
function binary2decimal(binaryNum)
	return tonumber(binaryNum, 2)
end

--十进制转二进制
function decimal2binary(decimalNum)
	if decimalNum == nil then
		decimalNum = 0
	end

	decimalNum = tonumber(decimalNum)
	if decimalNum <= 1 then return decimalNum
	else
		return decimal2binary(math.floor(decimalNum/2))..tostring(decimalNum%2)
	end
end

function splitString( strings,separator )
	if not separator then separator = "_" end
	local map ={};
	local str,n
	while strings~="" do
		n = string.find(strings,separator);
		if n then
			str = string.sub(strings,1,n-1);
			if str and str ~= "" then
				table.insert(map,#map+1,str);
			end
			strings = string.sub(strings,n+1);
		else
			if strings~="" then
				table.insert(map,#map+1,strings);
			end
			break;
		end
	end
	return map;
end

function getRandomNum(start, finish, amount)
	local randomIndexes = {}
	while #randomIndexes < amount do
		local randomNum = math.random(start, finish)
		if not table.indexof(randomIndexes, randomNum) then
			table.insert(randomIndexes, randomNum)
		end
	end
	return randomIndexes
end

--[[
	有两种使用方法，一种是player通知，一直是server通知
	player通知用法：util.alert_msg(mtype,"哈哈哈",player, flag)
	server:util.alert_msg(mtype,"哈哈",flag)
	mtype写数字，只要里面包含1，2，3，4，5就会在屏幕对应区域显示，比如12345
	1,2,3,4分别对应屏幕从上到下的4块提示,5对应屏幕右侧的提示
	flag为1表示立刻顶掉其他的消息
]]
function alert_msg(mtype, msg, player, flag)
	mtype = tostring(mtype)

	local msgWeights = {
		[1] = 10000,
		[2] = 100,
		[3] = 1000,
		[4] = 1,
		[5] = 100000,
		[6] = 10,
	}

	local value = 0
	for i,v in ipairs(msgWeights) do
		if string.find(mtype, tostring(i)) then
			value = value + msgWeights[i]
		end
	end

	if type(player) == "userdata" then--服务器发
		player:alert(value, flag or 0,msg)
	else
		server.info(value, checknumber(player) or 0, msg)
	end
end

--用来排序用下划线分割的字符串
function sortString(a, b, isNeedGreaterThan)
	a = splitString(a)
	b = splitString(b)
	local length = #a>#b and #a or #b
	local greater = false
	for i = 1,length do
		local num1 = checknumber(a[i])
		local num2 = checknumber(b[i])
		if num1 ~= num2 then
			greater = isNeedGreaterThan and (num1 >= num2) or (num1 > num2)
			break
		end
	end
	return greater
end

function checknumber(value, base)
    return tonumber(value, base) or 0
end

--获取单个物品需要的格子数
function getNeedBags(player,typeId, amount)
	local noStacks = {
		40000001,   --人物经验
		40000002,   --护卫经验
		40000003,   --金币
		40000004,   --绑定金币
		40000005,   --元宝
		40000006,   --绑定元宝
		40000007,   --声望值
		40000008,   --内功经验
	}

	local needBags = 0
	if typeId then
		if not table.indexof(noStacks, typeId) then
			local stack = player:get_item_stack(typeId)
			needBags = math.ceil(amount / stack)			
		end
	else
		if not amount then
			print(debug.traceback())
		end
		needBags = amount
	end
	return needBags
end

function getXJBags(player,awardInfo)--获得玄晶宝石背包格子数量
	local needBags = 0
	for i,v in pairs(awardInfo) do
		if v.id >= 17020 and v.id <= 17067 then
			needBags = needBags + getNeedBags(player,v.id, v.amount or v.num)
		end
	end
	return needBags
end
--获award数组里所有物品需要的格子数
function getAwardNeedBags(player,awardInfo)
	local needBags = 0
	for i,v in ipairs(awardInfo) do
		needBags = needBags + getNeedBags(player,v.id, v.amount or v.num)
	end
	return needBags
end

--获得玩家内观信息
function getInnerLooksInfos(player)
	local result = {}
	result[1] = player:get_guild_title()
	result[2] = player:get_item_type_by_pos(const.ITEM_WEAPON_POSITION)
	result[3] = player:get_item_type_by_pos(const.ITEM_CLOTH_POSITION)
	result[4] = ppn(player, const.PP_WING_LEVEL)
	result[5] = player:get_gender_name()
	return result
end

function checkMainTaskTid(player, tidTable) -- 检测主线是否在指定任务
	local s = player:get_task_state(1000);
	local b = math.floor(s / const.TASK_STATE_NUM);
	local d = math.fmod(s,const.TASK_STATE_NUM);
	if b then
		for i,v in ipairs(tidTable) do
			if b == v then return v end
		end
	end
end

-- 检测经验丹使用限制(每日最多使用100个)
-- function checkExpDanUseNum(player,num)
-- 	local yuNum = 100-util.ppn(player,const.PP_EXPDAN_USENUM)--剩余次数
-- 	if yuNum>0 then
-- 		if num<=yuNum then
-- 			player:alert(1,1,"您今日还可使用经验丹"..yuNum-num.."次")
-- 			return num
-- 		else
-- 			player:alert(1,1,"您今日经验丹的使用次数已用完")
-- 			return yuNum
-- 		end
-- 	else
-- 		player:alert(1,1,"对不起,您今日经验丹的使用次数已用完")
-- 		return 0
-- 	end
-- end
--根据概率选出一个
function produceProb(tb,begin,endi)
	if type(tb) == "table" then
		if not begin then begin = 1 end
		if not endi then endi = #tb end
		local all = 0
		local newT = {}
		for k,v in ipairs(tb) do
			if k>=begin and k<= endi then
				if type(v) == "number" then
					all = all + v
					table.insert(newT,v)
				elseif type(v) == "table" and v.prob then
					all = all + v.prob
					table.insert(newT,v.prob)
				else
					all = all +1
					table.insert(newT,1,1)
				end
			end
		end
		local seed = math.random()
		local prob = 0
		for i=1,#newT do
			prob = prob + newT[i]/ all
			if seed <= prob then
				return i + begin - 1
			end
		end
	end
end

--根据概率选出n个
function chooseSomeFromTable(tb,n,begin,endi)
	if type(tb) == "table" and type(n) == "number" then
		local choosetable,result = {},{}
		if n == #tb then
			return tb;
		else
			while #choosetable<n do
				local c = produceProb(tb,begin,endi)
				if not table.indexof(choosetable, c) then
					table.insert(choosetable,c);
				end
			end
			for k,v in pairs(choosetable) do
				table.insert(result,tb[v]);
			end
			return result;
		end
	end
end

function merge(dest, src)
    for k, v in pairs(src) do
        dest[k] = v
    end
end

---从二维表里每组选一个出来组成单表
function chooseFunc(tb)
	local cardAward = {} --卡牌奖励
	local selected = produceProb(tb)
	local temp
	for i,v in ipairs(tb) do
		temp = produceProb(v)
		table.insert(cardAward,#cardAward+1,v[temp])
	end
	
	return selected,cardAward
end

function removeItemAndReturnNum(player, item, total) -- 参数total是扣除物品总数
	local numbind = player:num_item(item, 1, 1)
	numbind = math.min(numbind, total)
	numbind = player:remove_item(item, numbind, 1)
	total = (total - numbind > 0) and player:remove_item(item, total - numbind, 0) or 0
	return numbind, total -- numbind 扣除绑定物品数量，total 扣除非绑定物品数量
end

-- 副本完成后判断有无回城石
function giveHomeStone(player)
	if player:num_item(10045, 1)<=0 then
		player:add_item(10045, 1, 1)
	end
end

function isLeapYear(year)
	if year%400 == 0 or (year%4 == 0 and not (year%100 == 0)) then
		return true
	else
		return false
	end
end

function calc2DaySep( day1AtSecond,day2AtSecond ) --计算两个日期（s）相差几天
	if day1AtSecond > day2AtSecond then day1AtSecond,day2AtSecond = day2AtSecond,day1AtSecond end
	local t1 = os.date("*t",day1AtSecond)
	local t2 = os.date("*t",day2AtSecond)
	local yd = 0
	if t1.year ~= t2.year then
		for i=t1.year,t2.year-1 do
			yd = yd + (isLeapYear(i) and 366 or 365)
		end
	end
	return yd + t2.yday - t1.yday
end

--四舍五入
function round(value)
    value = checknumber(value)
    return math.floor(value + 0.5)
end

function checkint(value)
    return round(checknumber(value))
end

-- function getMonstersOfMap(mapName,monName)
-- 	local map = server.find_map(mapName)
-- 	if map then
-- 		return map:num_monster(monName)
-- 	end
-- 	return 0
-- end
--过滤string中数字
function filterNumberOfString(str)
	if str and #str>0 then
		str,_ = string.gsub(str,"%d","")
	end
	return str
end

--除魔难度系数及加成
function getChuMoNanDu(player)
	local mTabel = {
		[1]={xishu=0,  needLev=0, zsLev=0,expAdd=0,   index=1},
		[2]={xishu=0.5,needLev=55,zsLev=0,expAdd=0.2, index=2},
		[3]={xishu=1,  needLev=68,zsLev=0,expAdd=0.35,index=3},
		[4]={xishu=2,  needLev=80,zsLev=0,expAdd=0.5, index=4},
		[5]={xishu=3,  needLev=0, zsLev=1,expAdd=0.7, index=5},
		[6]={xishu=4,  needLev=0, zsLev=3,expAdd=0.9, index=6},
		[7]={xishu=5,  needLev=0, zsLev=5,expAdd=1.2, index=7},
	}
	local level = player:get_level()
	local zsLevel = util.ppn(player,const.PP_ZHSH_LEVEL)
	local maxNum = 1
	for i=#mTabel,1,-1 do
		if mTabel[i].needLev==0 then
			if zsLevel>=mTabel[i].zsLev then
				maxNum = i
				break
			end
		else
			if level>=mTabel[i].needLev then
				maxNum = i
				break
			end
		end
	end
	local tempTabel = {}
	tempTabel.maxNum = maxNum
	tempTabel.table = mTabel
	return tempTabel
end

function getNiChen(player)
	local nichen = {["male"]="少侠",["female"]="女侠"}
	return nichen[player:get_gender_name()]
end

--	数字转换成日期 	20160405 -> 2016/4/5
function formatDate(date)
	local year = math.floor(date/10000)
	local month = math.floor(date%10000/100)
	local day = date%100
	return year.."/"..month.."/"..day;
end

function bound(min,value,max)
	return math.max(min,math.min(value,max));
end

function getJobId(pp)
	local jobId = {
		["warrior"] = 100,
		["wizard"] = 101,
		["taoist"] = 102,
	}
	if isUserData(pp) then
		return jobId[pp:get_job_name()]
	elseif isString(pp) then
		return jobId[pp]
	end
end

function getJobName(pp)
	local jobName = {
		["warrior"] = "战士",
		["wizard"] = "法师",
		["taoist"] = "道士",
	}
	if isUserData(pp) then
		return jobName[pp:get_job_name()]
	elseif isString(pp) then
		return jobName[pp]
	end
end

function numberToChinese(num)
	local enum = {["0"] = "零", ["1"] = "一", ["2"] = "二", ["3"] = "三", ["4"] = "四", ["5"] = "五", ["6"] = "六", ["7"] = "七", ["8"] = "八", ["9"] = "九", }
	local struct = {[1] = "",[2] = "十",[3] = "百",[4] = "千",[5] = "万",[6] = "十",[7] = "百",[8] = "千",[9] = "亿",[10] = "十",[11] = "百",[12] = "千"}
	local n,s,str = tonumber(num),"",""
	if n then
		n = math.modf(n)--取整数部分
		s = tostring(n)
		local zero,wan = 0,0
		local weizhi,char,hanzi,danwei
		for i=#s-1,0,-1 do
			weizhi = #s-i
			char = string.sub(s,i+1,i+1)
			hanzi = enum[char]
			danwei = struct[weizhi] or ""
			if char =="0" then
				zero = zero + 1
				if weizhi ==1 then
					hanzi = "" --消除末尾的零
				elseif weizhi%4==1 then
					hanzi = "" --消除 零万 前面的零
					danwei = "" --遇到万 单位则交给前面的数字处理单位
				else
					if zero >1 then
						hanzi = "" --如果连续出现两个以上零，只记一个
					end
					danwei = ""
				end
			else
				zero = 0
				if char == "1" and weizhi%4 == 2 then
					hanzi = "";	--消除一十万前面一
				end
				if math.floor(weizhi/4) ~=wan then
					wan = wan + 1
					if weizhi%4 > 1 then
						danwei = danwei .. struct[math.floor(weizhi/4)*4+1] --添加单位
					end
				end
			end
			str = hanzi..danwei..str
		end
	end
	return str;
end

function isSame(a,b)
	if type(a) == type(b) then
		if type(a) == "table" then
			for k,v in pairs(a) do
				if not isSame(v,b[k]) then
					return false
				end
			end
			return true
		else
			return a == b
		end
	else
		return false
	end
end

function getZsLevel(player)
	return ppn(player, const.PP_ZHSH_LEVEL);
end

local job2Enum = {
	["warrior"] = 1,
	["wizard"] = 2,
	["taoist"] = 3,
};

function checkEquipNeedLevel(typeId, job, needlevel)
	local id = tab.itemdef[tonumber(typeId)]
	if id then
		if id.job == 0 or id.job == job2Enum[job] then
			if id.needlevel >= needlevel then
				return true;
			end
		end
	end
	return false;
end

function getItemNeedParam(player, typeId)
	local itname,itac,itacmax,itmac,itmacmax,itdc,itdcmax,itmc,itmcmax,itsc,itscmax,itaccu,itdodge,itneedtype,itneedparam,itprice = player:get_item_base_data(typeId);
	return itneedparam;
end

function isType(target, mType)
	return type(target) == mType
end

function isNumber(target)
	return isType(target, "number")
end

function isTable(target)
	return isType(target, "table")
end

function isBool(target)
	return isType(target, "boolean")
end

function util.isFunction(target)
	return isType(target, "function")
end

function isString(target)
	return isType(target, "string")
end

function isUserData(target)
	return isType(target, "userdata")
end

function getAward(player, award)
	if #award >= 1 then
		for i,v in ipairs(award) do
			if not v.id then
				return false
			end
			if v.level then
				player:add_item2(v.id, 1, 100, "level("..v.level..")")
			else
				player:add_item(v.id, v.amount or v.num, v.bind or 0)
			end
		end
		return true
	else
		if not award.id then
			return false
		end
		if award.level then
			player:add_item2(award.id, 1, 100, "level("..award.level..")")
		else
			player:add_item(award.id, award.amount or v.num,v.bind or 1)
		end
		return true
	end
end

function getItemType(type_id)
	local id = tab.itemdef[tonumber(type_id)]
	if id then
		return tonumber(id.sub_type);
	end
	return 0;
end

function isSkillItem(type_id)
	return getItemType(type_id) == const.ITEM_TYPE_SKILLITEM;
end

function isEquipMent(type_id)
	return getItemType(type_id) == const.ITEM_TYPE_EQUIP;
end

local skill_func = {
	[26010001] = function (player) 
		item.skill.JiChuJianShu(player);
	end,
	[26010011] = function (player) 
		item.skill.CiShaJianShu(player);
	end,
	[26010021] = function (player) 
		item.skill.BanYueWanDao(player);
	end,
	[26010031] = function (player) 
		item.skill.YeManChongZhuang(player);
	end,
	[26010041] = function (player) 
		item.skill.LieHuoJianFa(player);
	end,
	[26010051] = function (player) 
		item.skill.ZhuRiJianFa(player);
	end,
	[26020001] = function (player) 
		item.skill.LeiDianShu(player);
	end,
	[26020011] = function (player) 
		item.skill.MoFaDun(player);
	end,
	[26020021] = function (player) 
		item.skill.DiYuLeiGuang(player);
	end,
	[26020031] = function (player) 
		item.skill.HuoQiang(player);
	end,
	[26020041] = function (player) 
		item.skill.KangJuHuoHuan(player);
	end,
	[26020051] = function (player) 
		item.skill.BingPaoXiao(player);
	end,
	[26030001] = function (player) 
		item.skill.LingHunHuoFu(player);
	end,
	[26030011] = function (player) 
		item.skill.ZhaoHuanShenShou(player);
	end,
	[26030021] = function (player) 
		item.skill.YouLingDun(player);
	end,
	[26030031] = function (player) 
		item.skill.ShiDuShu(player);
	end,
	[26030041] = function (player) 
		item.skill.QunTiZhiLiao(player);
	end,
	[26030051] = function (player) 
		item.skill.JiTiYinShenShu(player);
	end,
	[26030061] = function (player) 
		item.skill.JinShenLiZhanFa(player);	
	end,
}

function addSkill(player, type_id)
	if skill_func[type_id] then
		skill_func[type_id](player);
	end
end


function isViceEquipMent(type_id)
	local id = tab.itemdef[tonumber(type_id)]
	if id then
		if tonumber(id.sub_type) == const.ITEM_TYPE_EQUIP then
			if tonumber(id.equip_type) >= const.EQUIP_TYPE_JADE_PENDANT and tonumber(id.equip_type) <= const.EQUIP_TYPE_CATILLA then
				return true;
			end
		end
	end
	return false;
end

function sendMail(target,title,content,award)
	local t = {}
	for k,v in pairs(award) do
		if v.id and v.num then
			table.insert(t,checknumber(v.id))
			table.insert(t,checknumber(v.num))
		end
	end
	if isUserData(target) then
		target:post_mail(title,content,unpack(t))
	elseif isString(target) then
		server.post_mail_to_one(target,title,content,unpack(t))--target is seed_name
	end
end

--[[
	date 参数	默认当前时间值
	year,month,day,hour,min,sec,isdst--夏令时
--]]
function convertToTime(date)
	date = date or {}
	local now = os.date("*t",os.time())
	for k,v in pairs(now) do
		if date[k] then
			now[k] = date[k]
		end
	end
	return os.time(now)
end
--获取数字小数点左边有多少位
function getDigitCapacity(num)
	return num == 0 and 1 or math.ceil(math.log10(math.abs(num)))
end