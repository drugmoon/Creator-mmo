
function OncientUse(use,base)
	for funcname, func in pairs(base) do
		if string.sub(funcname, 1,2) == "on" or string.sub(funcname, 1,2) == "do" then
			use[funcname] = func
		end
	end
end

function get_day_YMD()
	return tonumber(os.date("%Y%m%d",os.time()));
end

function get_day_YMDHM()
	return tonumber(os.date("%Y%m%d%H%M",os.time()));
end

function get_hour_minute()
	return tonumber(os.date("%H%M", os.time()))
end

function get_month_day()
	return tonumber(os.date("%m%d", os.time()))
end

function get_day(t)  --+8小时转为东八区北京时间
	return math.floor((t+8*60*60)/(60*60*24));
end

function hour()
	return math.floor((os.time()+8*60*60)/(60*60));
end

function today()
	return get_day(os.time());   --os.time():得到是当前时间的距离1970.1.1.08:00时间的秒数
end
function today_bless()
	return get_day(os.time() - 6 * 60 * 60);
end

function week()
	return math.floor((today()+4)/7)
end

function weekDay()
	local weekDay = tonumber(os.date("%w", os.time()))
	if weekDay == 0 then weekDay = 7 end
	return weekDay
end

function server_start_time()
	local st = server.get_var("server_start_time");
	if st ~= "" then
		return os.time()-tonumber(st);
	end
	return 0;
end

function param_index()
	return 0;
end

function sever_index()
	return 2;
end

function sever_index_jingcai()
	return 2;
end

function server_start_day()
	local st = server.get_var("server_start_time");
	if st ~= "" then
		return today()-get_day(tonumber(st));
	end
	return 0;
end

function role_create_day(player)
	local createTime = player:get_create_time();
	if createTime then
		return today() - get_day(createTime);
	end
	return 0;
end

function reset_server_start_day(day)
	server.set_var("server_start_time",os.time()-(day-1)*24*60*60);
end

function is_merge_server()
	return "" ~= server.get_var("server_merge_tag");
end

function server_merge_time()
	if is_merge_server() then
		local st = server.get_var("server_merge_time");
		if st ~= "" then
			return os.time()-tonumber(st);
		end
		return 0;
	else
		return -1;
	end
end

function server_merge_day()
	if is_merge_server() then
		local st = server.get_var("server_merge_time");
		if st ~= "" then
			return today()-get_day(tonumber(st));
		end
		return 0;
	else
		return -1;
	end
end

function table.indexof(array, value, begin)
    for i = begin or 1, #array do
        if array[i] == value then return i end
    end
	return false
end

function table.removebyvalue(array, value, removeall)
    local c, i, max = 0, 1, #array
    while i <= max do
        if array[i] == value then
            table.remove(array, i)
            c = c + 1
            i = i - 1
            max = max - 1
            if not removeall then break end
        end
        i = i + 1
    end
    return c
end

function table.insertto(dest, src, begin)
	begin = tonumber(begin) or 0
	begin = math.floor(begin + 0.5)
	if begin <= 0 then
		begin = #dest + 1
	end
	local len = #src
	for i = 0, len - 1 do
		dest[i + begin] = src[i + 1]
	end
end

function table.nums(t)
    local count = 0
    for k, v in pairs(t) do
        count = count + 1
    end
    return count
end

function table.slice(t,first,last)
	assert(type(t) =="table","table.slice param 1 is not table");
	assert(type(first) =="number","table.slice param 2 is not number");
	first = first or 0
	last = last or #t
	last = math.min(last,#t)
	if last <0 then last = #t+last end
	local temp = {}
	for i=first,last do
		if t[i] then
			table.insert(temp,t[i])
		end
	end
	return temp
end

function server_param_exp_open()
	if tonumber(server.get_exp_mul()) <= 1 then
		server.set_exp_mul(2);
		server.info(10010,0,"全服双倍经验已经开启!");
	end
end
function server_param_exp_close()
	if tonumber(server.get_exp_mul()) >= 2 then
		server.set_exp_mul(1);
		server.info(10010,0,"全服双倍经验已经关闭!");
	end
end

function OnInit()
	if "" == server.get_var("server_start_time") then
		server.set_var("server_start_time",tostring(os.time()));
	end
end



function setTimeFormat(milliSecond , typeT)
	local Time=milliSecond*1000
	local day = math.floor(Time / 3600000 / 24)
	local hour = math.floor(Time%(3600000*24) / 3600000)
	local minute = math.floor(Time%3600000 / 60000)
	local second = math.floor(Time % 60000 / 1000)
	local milliSecondStr = math.floor(Time % 1000)
	local str = ""
	if not typeT then
		if day >0 then str = str..day.."天" end
		if hour>0 then str = str..hour.."时" end
		if minute>0 then str = str..minute.."分" end
		if second>0 then str = str..second.."秒" end
	elseif typeT == 1 then
		str = minute.."分"..second.."秒"
	elseif typeT == 2 then
		str = string.format("%02d:%02d:%02d",hour,minute,second)
		-- str = (hour >= 10 and hour or "0" ..hour) ..":"..(minute >= 10 and minute or "0" ..minute)..":"..(second >= 10 and second or "0" ..second)
	elseif typeT == 3 then
		str = string.format("%02d:%02d",minute,second)
		-- str = (minute >= 10 and minute or "0" ..minute)..":"..(second >= 10 and second or "0" ..second)
	elseif typeT == 4 then
		str = string.format("%02d时%02d分%02d秒",hour,minute,second)
		-- str = hour.."时"..minute.."分"..second.."秒"
	elseif typeT == 5 then
		str = {day , hour , minute , second , milliSecondStr}
	elseif typeT == 6 then
		str = string.format("%02d天%02d时%02d分",day,hour,minute)
		-- str = day.."天"..(hour%24) .."时"..minute.."分"
	elseif typeT == 7 then
		str = string.format("%02d时%02d分",hour,minute)
		-- str = hour.."时"..minute.."分"
	elseif typeT == 8 then
		if day>0 then
			str = day.."天"..(hour >= 10 and hour or "0" ..hour) .."小时"..(minute >= 10 and minute or "0" ..minute).."分"..(second >= 10 and second or "0" ..second).."秒"
		else
			str = (hour >= 10 and hour or "0" ..hour) .."小时"..(minute >= 10 and minute or "0" ..minute).."分"..(second >= 10 and second or "0" ..second).."秒"
		end
	end	
	return str
end


require "functions"
require "util"
require "const"
require "tab"
require "posnpc"
require "posmon"
require "posmap"
require "pospanel"
require "go"
require "ckdead"
require "ckmon"
require "cklevel"
require "ckpanel"
require "cktalk"
require "ckitem"
require "ckzslevel"
require "ckcopy"
require "cktask"
require "ckmap"
require "ckguild"
require "ckgroup"
require "ckinnerpowerlevel"

require "login"
require "ckattr"
require "item"
require "gui"
require "task"
require "gm"
require "player"
require "mon"
require "npc"
require "map"
require "autorun"
require "genxxx"
require "vcoinshop"
require "war"
require "slave"
require "count"
-- require "battle"
print("SCRIPT_VERSION_TAG")


--充值统一接口
function checkFirstCharge(player)
	if util.ppn(player, const.PP_SUM_RECHARGE) ==0 then
		player:push_lua_table("open",util.encode({name = "extend_firstPay"}))
		return true
	end
	return false
end
function showChongzhi(player,num)
	num = num or ""
	if checkFirstCharge(player) then return end
	local result = {
		book = util.encode({actionid = "openchongzhi"}),
		str = "很抱歉,您元宝不足"..num,
		labelConfirm = "充值",labelCancel = "取消",
		callFunc = "server.showChongzhi",
	}
	player:push_lua_table("showConfirm",util.encode(result))
end

function handlePanelData(player,commond)
	if commond =="check" then
		showChongzhi(player)
		return
	end
	player:push_lua_table("open",util.encode({name = "panel_charge"}))
end
ckpanel.add_listener("server.showChongzhi",handlePanelData)


print(os.date("V0.9.0043 (%Y-%m-%d %H:%M:%S)",os.time()));

PLATFORM_BANSHU = false;