module(..., package.seeall)
local LVMAX = 12

-- ID	vip等级	需要充值元宝数	每日使用金砖个数	封魔领最大倍数	每日经验buff倍数	每日经验buff小时数	经验buffID	攻击buffID	
-- 原地免费复活次数	离线挂机经验倍数 奖励物品	奖励物品数	绑定状态	挂机地图
-- int	int	int	int	int	int	int	int		int		int[]	int[]	int[]	string

local keys = {"id","vip","needVcoin","dailyJinzhuan","fengmoTimes","expBuffTimes","expBuffHour","expBuffId", "attackBuffId",
	"reliveLocalTimes","offlineExpTimes","award","awardNum","bind","offlineMap"}
local types ={"int","int","int","int","int","int","int","int","int","int","int","int[]","int[]","int[]","string[]"}

-- print(#keys,#types)
local temp = tab.parseCsvFile(server.load_csv_file("vip.csv"),keys,types)
vipData = {}

for i,vipConf in pairs(temp) do
	if tonumber(i) then
		local lv = vipConf.vip
		vipData[lv] = vipData[lv] or {}
		vipData[lv].id = vipConf.id
		vipData[lv].vip = vipConf.vip
		vipData[lv].needVcoin = vipConf.needVcoin
		vipData[lv].dailyJinzhuan = vipConf.dailyJinzhuan
		vipData[lv].fengmoTimes = vipConf.fengmoTimes/10000
		vipData[lv].expBuffTimes = vipConf.expBuffTimes
		vipData[lv].expBuffHour = vipConf.expBuffHour
		vipData[lv].expBuffId = vipConf.expBuffId
		vipData[lv].attackBuffId = vipConf.attackBuffId
		vipData[lv].reliveLocalTimes = vipConf.reliveLocalTimes
		vipData[lv].offlineExpTimes = vipConf.offlineExpTimes/10000
		vipData[lv].award = vipData[lv].award or {}

		for j=1,#vipConf.award do
			if vipConf.award[j] and vipConf.award[j]>0 then
				local award = {}
				award.id = vipConf.award[j]
				award.num = vipConf.awardNum[j]
				award.bind = 2-vipConf.bind[j]
				table.insert(vipData[lv].award,award)
			end
		end
		vipData[lv].offlineMap = vipConf.offlineMap
	end
end

-- for kk,vv in pairs(vipData) do
-- 	print("------------------")
-- 	for k,v in pairs(vv) do
-- 		print(k,v)
-- 	end
-- end
-- 编号	显示名称	vip1	vip2	vip3	vip4	vip5	vip6	vip7	vip8	vip9	vip10	vip11	vip12
-- int	string	string	string	string	string	string	string	string	string	string	string	string	string

keys = {"id","content","vip1","vip2","vip3","vip4","vip5","vip6","vip7","vip8","vip9","vip10","vip11","vip12"}
types ={"int","string","string","string","string","string","string","string","string","string","string","string","string","string"}

local temp = tab.parseCsvFile(server.load_csv_file("vipShow.csv"),keys,types)
vipShow = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		table.insert(vipShow,temp[tostring(k)])
	end
end

function getVipConf(vip)
	local configs,t = {},{}
	for k,v in pairs(vipData) do
		t = {
				name = "VIP"..v.vip,
				lv = v.vip,
				needVcoin = v.needVcoin,
			}
		if not vip then
			if v.vip>0 then
				table.insert(configs,t)
			end
		else
			if vip == v.vip then
				return v
			end
		end
	end
	table.sort(configs,function ( a,b )
		return a.lv<b.lv;
	end)
	return configs
end

function getVipAward(vip)
	if vipData[vip] then
		return vipData[vip].award
	end
	return {}
end

function getVipInfo(vip)
	local info = {}
	if vip>=1 and vip<=LVMAX then
		for k,v in pairs(vipShow) do
			if #v["vip"..vip]>0 then
				table.insert(info,v.content.."<font color=#18d129>"..v["vip"..vip].."</font>")
			end
		end
	else
		info = {'',''}
	end
	return info
end

function getOfflineExpTimes(player)
	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
	if vipData[vipLv] then
		return vipData[vipLv].offlineExpTimes
	end
	return 1
end

function getJinZhuanTimes(player)
	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
	if vipData[vipLv] then
		return vipData[vipLv].dailyJinzhuan
	end
	return 0
end

function getFreeReliveTimes(player)
	local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
	if vipData[vipLv] then
		return vipData[vipLv].reliveLocalTimes
	end
	return 0
end