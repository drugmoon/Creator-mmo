-- exchange.lua
module(..., package.seeall)

--type说明
local exchangeType = {
	[1] = "神兵",
	[2] = "神甲",
	[3] = "转换",
	[4] = "首饰",
	[5] = "其他",
}
local job= {
	[1] = "战士",
	[2] = "法师",
	[3] = "道士",
	[0] = "通用",
}

-- 编号	职业	类别	兑换物品ID	所需物品ID	所需积分数	所需转生等级	是否绑定
-- ID	JOB	    Type	  ItemID	NeedItemID	 Needcon	  NeedRELev	     Isbind
local keys = {"id","job","type","typeId","needId","needScore","needZslv","bind"}
local types ={"int","int","int","int","int","int","int","int",}
Data = tab.parseCsvFile(server.load_csv_file("exchange.csv"),keys,types)

exchangeData = {}

for k,v in pairs(Data) do
	local tp = util.checknumber(v.type)
	if exchangeType[tp] then
		if not exchangeData[tp] then exchangeData[tp] = {} end
		v.name = tab.getItemName(v.typeId);
		v.needName = tab.getItemName(v.needId);
		table.insert(exchangeData[tp],v)
	end
end

for k,v in pairs(exchangeData) do
	table.sort(v,function(a,b)
		return a.id<b.id
	end)
end