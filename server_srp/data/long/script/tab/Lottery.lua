module(..., package.seeall)

--type说明
local lotteryType = {
	[1] = "神兵",
	[2] = "神甲",
	[3] = "转换",
	[4] = "首饰",
	[5] = "其他",
}

-- ID	组别	物品ID	权重	是否绑定	是否公告	是否可回收
-- ID	group	typeId	weight	isbind	isshow	canremove
local keys = {"id","group","typeId","prob","bind","isshow","canremove"}
local types ={"int","int","int","int","int","int","int",}
lotteryItem = tab.parseCsvFile(server.load_csv_file("lottoryItems.csv"),keys,types)


keys = {"id","getnum",}
types ={"int","int",}
lotteryGroup = tab.parseCsvFile(server.load_csv_file("lottoryGroupTimes.csv"),keys,types)


-- Items = {}

-- for k,v in pairs(Data) do
-- 	local tp = util.checknumber(v.type)
-- 	if exchangeType[tp] then
-- 		if not exchangeData[tp] then exchangeData[tp] = {} end
-- 		v.name = tab.getItemName(v.typeId);
-- 		v.needName = tab.getItemName(v.needId);
-- 		table.insert(exchangeData[tp],v)
-- 	end
-- end

-- for k,v in pairs(exchangeData) do
-- 	table.sort(v,function(a,b)
-- 		return a.id<b.id
-- 	end)
-- end