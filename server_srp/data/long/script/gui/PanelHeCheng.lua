module(... ,package.seeall)
local panel_name = "PanelHeCheng"
function handlePanelData(player, commond)
	local param = util.decode(commond)
	if not param then return end
	if param.actionid == "startHeCheng" then
		startHeCheng(player,param.key,param.num)
	elseif param.actionid == "getData" then
		getData(player,param.tab)
	elseif param.actionid == "get_item_data" then
		get_item_data(player,param.id)
	end
end

ckpanel.add_listener("gui.PanelHeCheng.handlePanelData", gui.PanelHeCheng.handlePanelData)

--开始合成
function startHeCheng(player,key,num)
	--合成检测
	local data = tab.hecheng[key..""]
	for i=1,3 do
		if data["needid"..i]~=0 then
			if player:num_item(tonumber(data["needid"..i]))<num*tonumber(data["neednum"..i]) then
				player:alert(1,1,"合成材料不足！")
				return
			end
		end
	end
	for i=1,3 do
		if data["needid"..i]~=0 then
			player:remove_item(data["needid"..i],data["neednum"..i]*num)
		end
	end
	player:add_item(data.targetid,num,1)
	player:alert(1,1,"恭喜，合成成功！")
	--刷新
	get_item_data(player,key)
end

function getData(player,index)
	local hecheng = tab.hecheng
	local info ={}
	info.cmd = "getData"
	info.data = {}
	for k,v in pairs(tab.hecheng) do
		if tonumber(v.type1) == index and player:get_level()>=tonumber(v.level) and util.getZsLevel(player)>=tonumber(v.zhuanshen) and (util.getJobId(player)-99==tonumber(v.job) or tonumber(v.job)==0 ) then
			table.insert(info.data,v)
		end
	end
	local compFunc = function(a, b)
		return a.id<b.id
	end
	table.sort(info.data, compFunc )
	player:push_lua_table(panel_name, util.encode(info))
end

function get_item_data(player,id)
	local info ={}
	info.cmd = "get_item_data"
	local data = tab.hecheng[id..""]
	local num = 0
	local num1 = 0
	local num2 = 10000000
	local num3 = 10000000
	if data.needid1~=0 then
		info.num1 = player:num_item(tonumber(data.needid1))
		num1 = math.floor(info.num1/tonumber(data.neednum1))
	end
	if data.needid2~=0 then
		info.num2 = player:num_item(tonumber(data.needid2))
		num2 = math.floor(info.num2/tonumber(data.neednum2))
	end
	if data.needid3~=0 then
		info.num3 = player:num_item(tonumber(data.needid3))
		num3 = math.floor(info.num3/tonumber(data.neednum3))
	end
	if num1<=num2 and num1<=num3 then
		num = num1
	elseif num2<=num1 and num2<=num3 then
		num = num2
	elseif num3<=num1 and num3<=num2 then
		num = num3
	end
	info.max_num = num
	player:push_lua_table(panel_name, util.encode(info))
end

