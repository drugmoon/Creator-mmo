module(..., package.seeall)

local composeType = {
	[1] = "套装合成",
	[2] = "神装合成",
	[3] = "首饰合成",
}
local needItemJob = {
	[0] = "通用",
	[1] = "战士",
	[2] = "法师",
	[3] = "道士",
}
--按 type 分类
composeData = {}

--#ID,合成类型,装备分类,合成物品ID,所需物品数量,所需装备套数,所需物品职业限制,所需物品部位限制,所需人物转生等级,所需人物等级,开始时间,结束时间,显示文本,是否公告
local keys = {"id","type","equipType","typeId","needNum","needEquipGroup","needItemJob","needItemPos","playerZslv","playerLv","startDay","endDay","info","isNotice"}
local types ={"int","int","int","int","int","int","int","int","int","int","int","int","string","bool"}

local temp = tab.parseCsvFile(server.load_csv_file("compose.csv"),keys,types)
Data ={}
for k,v in pairs(temp) do
	if tonumber(k) then
		table.insert(Data,v)
	end
end
for k,v in pairs(Data) do
	local tp = util.checknumber(v.type)
	if composeType[tp] then
		if not composeData[tp] then composeData[tp] = {} end
		table.insert(composeData[tp],v)
	end
end

for k,v in pairs(composeData) do
	table.sort(v,function(a,b)
		return a.id<b.id
	end)
end
function getConf(id)
	for i,v in ipairs(Data) do
		if v.id == id then
			return v
		end
	end
end

-- for i,typeT in pairs(composeData) do
-- 	for k,v in pairs(typeT) do
-- 		print(i,"--",v.id.." "..v.type.." "..v.equipType.." "..v.targetId.." "..v.needNum.." "..v.needEquipType.." "..v.needItemJob,v.isNotice)
-- 		-- for m,n in pairs(v) do
-- 		-- 	print(m,n)
-- 		-- end
-- 	end
-- end

