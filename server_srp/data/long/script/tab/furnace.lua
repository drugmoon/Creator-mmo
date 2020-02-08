module(..., package.seeall)

local furnaceType = {
	[1] = "玉佩",
	[2] = "护盾",
	[3] = "龙心",
	[4] = "狼牙",
	[5] = "勋章",
}
local needAttrId = {
	[551] = const.PP_YUPEI_POOL ,
	[552] = const.PP_HUDUN_POOL ,
	[553] = const.PP_LONGXIN_POOL,
	[554] = const.PP_LANGYA_POOL,
	[555] = const.PP_LANGYA_POOL,
}
local job = {
	[1] = "战士",
	[2] = "法师",
	[3] = "道士",
	[0] = "通用",
}
--按 type 分类
furnaceData = {}

local keys = {"index","job","id","needItemId","type","lv","needAttrId","needValue","nextId"}
local types ={"int","int","int","int","int","int","int","int","int",}
local furnaceCsv = tab.parseCsvFile(server.load_csv_file("furnace.csv"),keys,types)

for k,v in pairs(furnaceCsv) do
	local tp = util.checknumber(v.type)
	if furnaceType[tp] then
		if not furnaceData[tp] then furnaceData[tp] = {} end
		table.insert(furnaceData[tp],v)
	end
end
for k,v in pairs(furnaceData) do
	table.sort(v,function(a,b)
		return a.id<b.id
	end)
end
-- keys = {"index","id","needItemId","type","lv","needAttrId","needValue","nextId"}
-- for i,typeT in pairs(furnaceData) do
-- 	for k,v in pairs(typeT) do
-- 		print(i,"--",v.id.." "..v.needItemId.." "..v.type.." "..v.lv.." "..v.needAttrId.." "..v.needValue.." "..v.nextId)
-- 	end
-- end

