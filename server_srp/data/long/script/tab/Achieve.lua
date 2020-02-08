-- Achieve.lua
module(..., package.seeall)

local achieveType = {
	[1] = "经验加成",
	[2] = "伤害提升",
	[3] = "回收上限",
	[4] = "仓位提升",
	[5] = "灭魔上限",
}
local countType = {
	[0] = "保留上个数据继续计数",
	[1] = "重新计数",
}
-- taskId	分类	前置ID	后续ID	计数模式	完成条件类型	计数	比对ID	花费金币	奖励成就值	功能ID1	功能ID2	成就名称	成就描述	完成条件描述	成就徽章图标	成就奖励艺术字																
-- int	int	int	int	int	int	int	int	int	int	int	int	string	string	string	string	string
local keys = {"taskId","type","preId","nextId","countType","completeType","num","compareId","coin","achieveValue","param1","param2","name","desp","completeDesp","huizhang","jiangli",}
local types ={"int","int","int","int","int","int","int","int","int","int","int","int","string","string","string","string","string",}

local temp = tab.parseCsvFile(server.load_csv_file("achieve.csv"),keys,types)

Data = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		Data[k] = v
	end
end

keys = {"lv","name","achieveValue","dc","MaxDc","mc","MaxMc","sc","MaxSc","icon"}
types ={"int","string","int","int","int","int","int","int","int","string"}

temp = tab.parseCsvFile(server.load_csv_file("achieveMedal.csv"),keys,types)
achieveMedal = {}
for k,v in pairs(temp) do
	if tonumber(k) then
		achieveMedal[tonumber(k)] = v;
	end
end

function getTaskConfig(taskId)
	return Data[tostring(taskId)]
end

function getMedalConfig()
	return achieveMedal
end

function getMedalValue(level)
	if achieveMedal[level] then
		return achieveMedal[level].achieveValue
	end
end