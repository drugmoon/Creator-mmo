module(..., package.seeall)

-- 活动类型	货币类型	货币数目	总奖励物品ID	物品数目	活动名称	活动名称图标	总价值
-- ID	MoneyKind	MoneyValue	ItemBaseID	ItemBaseCount	Name	IconName	TotalValue
-- int	int	int	int[]	int[]	string	string	int
local keys = {"id","MoneyKind","MoneyValue","ItemBaseID","ItemBaseCount","Name","IconName","TotalValue"}
local types ={"int","int","int","int[]","int[]","string","string","int"}
local temp = tab.parseCsvFile(server.load_csv_file("invest_type.csv"),keys,types)
invest_type = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		invest_type[tonumber(k)] = v;
	end
end
-- id	物品组	所需天数	物品ID	物品数目	物品绑定状态
-- ID	Type	NeedDay	ItemBaseID	ItemNum	ItemBind
-- int	int	int	int[]	int[]	int[]
keys = {"id","Type","NeedDay","ItemBaseID","ItemNum","ItemBind"}
types ={"int","int","int","int[]","int[]","int[]"}
temp = tab.parseCsvFile(server.load_csv_file("invest_award.csv"),keys,types)
invest_award = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		invest_award[tonumber(k)] = v;
	end
end

function getTypeData(idx)
	if idx then
		return invest_type[idx]
	end
	return invest_type
end

function getAwardDataByIndex(typ,index)
	local data = {};
	for k,v in pairs(invest_award) do
		if v.Type == typ and v.NeedDay == index then
			table.insert(data,v)
		end
	end
	return data;
end