module(..., package.seeall)

-- local keys = {"id","level","exp","valWarrior","valMage","valPriests","dtr","addvalWarrior","addvalMage","addvalPriests"}
-- local types = {"int","int","int","int","int","int","int","int","int","int"}

-- local temp = tab.parseCsvFile(server.load_csv_file("innerPower.csv"),keys,types)
-- local Data = {}
-- for k,v in pairs(temp) do
-- 	if tonumber(k) then
-- 		table.insert(Data,v)
-- 	end
-- end
-- table.sort(Data,function(a,b)
-- 	return a.id<b.id
-- end)

local fashion_preview = {
	{id=15200022,name="清凉一夏（30天）",accessway="    开服活动-累计充值",},
	{id=15200016,name="西部牛仔（7天）",accessway="    奖励大厅-15日登录",},
	{id=15200040,name="海军上将（30天）",accessway="    超值回馈-累计充值",},
	{id=15200006,name="饿狼传说（7天）",accessway="    产出不详",},
	{id=15200032,name="黑客帝国（7天）",accessway="    产出不详",},
	{id=15200038,name="恶棍天使（7天）",accessway="    产出不详",},
	{id=15200050,name="神雕侠侣（7天）",accessway="    产出不详",},
	{id=15200056,name="剑侠情缘（7天）",accessway="    产出不详",},
	{id=15200062,name="旺角一霸（7天）",accessway="    产出不详",},
	{id=15200068,name="黄金圣战（7天）",accessway="    产出不详",},
}

function onPanelData(player,command)
	local data = util.decode(command)
	if data.actionid == "fresh" then
		freshPanel(player)
	end
end
ckpanel.add_listener("gui.PanelFashion.onOpenPanel",gui.PanelFashion.onPanelData)

function freshPanel(player,up)
	local result = {}

	result.cmd = "getFashionPreview";
	result.Data = util.clone(fashion_preview);

	result.up = up
	player:push_lua_table("PanelFashion", util.encode(result))
end