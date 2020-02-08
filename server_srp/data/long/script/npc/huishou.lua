module(..., package.seeall)
util.use_function(_M,task.task1000);
local BASEEXP = 2E9

function onRefreshShowFlags(npc,player)
	local flags = 0;
	flags = flags + task.task1000.get_npc_flags(npc,player);flags=task.util.fix_npc_flags(flags);
	player:show_npc_flags(npc,flags);
end

function onTalk100(npc,player)
	local task_state = task.task1000.get_npc_flags(npc,player);
	if task_state > 0 then
		task.task1000.show_task(npc,player);return;
	end
	player:push_lua_table("open",util.encode({name = "menu_recycle"}))
end

--全名领取状态排序
local sortFunc = function(a,b)
	
end

-- equip_level

function upToLevel(player,extraExp,lev)
	-- print(player:get_exp(),"===11111111111111111")
	local curExp = extraExp+player:get_exp()
	if curExp>=player:get_level_exp(lev) then
		curExp=curExp-player:get_level_exp(lev)
		lev = lev+1
		return upToLevel(player,curExp,lev)
	else
		return lev
	end
end

--回收增加经验
function giveExp(player)
	local exp = util.ppn(player, const.PP_HUISHOU_EXPNUM)
	if exp<=0 then return end
	if exp>1E9 then
		local m = (exp-exp%1E9)/1E9
		for i=1,m do
			player:add_exp(1E9)
		end
		player:add_exp(exp%1E9)
	else
		player:add_exp(exp)
	end
	player:set_param(const.PP_HUISHOU_EXPNUM,0)
end

--计算累计经验
function allPosExp(player,table1)
	local exp = 0
	for k,v in pairs(table1) do
		local itemId = player:get_item_type_by_pos(v)
		local item = player:get_item_by_pos(v)
		local num=1
		if item then
			num = item:get_number()
		end
		if itemId and itemId > 0 then
			exp = exp + util.checknumber(tab.itemdef[itemId].recycle_exp)*num
		end
	end
	return exp
end

--回收部分
function onHuishouFunc(player,table1,startSort)
	table.sort(table1,function (a, b)
		local itemIda = player:get_item_type_by_pos(a)
		local itemIdb = player:get_item_type_by_pos(b)
		if not tab.itemdef[itemIda] or not tab.itemdef[itemIdb] then return end
		if tab.itemdef[itemIda].need_zslevel ~= tab.itemdef[itemIdb].need_zslevel then
			return tab.itemdef[itemIda].need_zslevel < tab.itemdef[itemIdb].need_zslevel
		else
			return tab.itemdef[itemIda].equip_level < tab.itemdef[itemIdb].equip_level
		end
	end)

	local posTable = table1
	if allPosExp(player,table1)>0 then
		posTable = accumulationExp(player,table1)
	end
	local totalExp = util.ppn(player, const.PP_RECYCLE_TOTAL_EXP)
	local addExp = util.ppn(player, const.PP_RECYCLE_ADD_EXP)
	if (BASEEXP + addExp <= totalExp) and allPosExp(player,table1)>0 then
		local result = {
			book = util.encode({actionid = ""}),
			str = "<font color=#b2a58b>       您今日回收经验已达上限!</font><br>          <font color=#b2a58b size=16>回收上限为：</font><font color=#00ff00 size=16>"..(BASEEXP + addExp).."</font>",
			labelConfirm = "确定",
			callFunc = "",
		}
		player:push_lua_table("showAlert",util.encode(result))
		return
	end
	local exp,xuefu,money= 0,0,0
	for k,v in pairs(posTable) do
		local itemId = player:get_item_type_by_pos(v)
		local item = player:get_item_by_pos(v)
		local num=1
		if item then
			num = item:get_number()
		end
		if itemId and itemId > 0 then
			if 0 == player:remove_item_by_type_pos(itemId,v,num) then
				exp = exp + util.checknumber(tab.itemdef[itemId].recycle_exp)*num
				xuefu = xuefu + util.checknumber(tab.itemdef[itemId].recycle_xuefu)*num
				money = money + util.checknumber(tab.itemdef[itemId].recycle_money)*num
			end
		end
	end

	local upLevel = upToLevel(player,exp,player:get_level())-player:get_level()--计算给的经验能升多少级

	player:set_param(const.PP_HUISHOU_EXPNUM,util.ppn(player, const.PP_HUISHOU_EXPNUM)+exp)

	player:set_param(const.PP_RECYCLE_TOTAL_EXP,totalExp + exp)
	-- player:set_param(const.PP_YUPEI_POOL,util.ppn(player,const.PP_YUPEI_POOL) + xuefu)
	local str = ""
	if exp>0 then 
		str="本次回收获得经验："..exp
	end
	if xuefu>0 then
		str=str.." 玉佩:"..xuefu
		gui.PanelFurnace.addFurnaceScore(player,1,xuefu)
	end
	if money>0 then
		str=str.." 绑定金币:"..money
		player:add_item("绑定金币",money,1)
	end
	player:alert(1, 1, str)
	if exp>0 then
		if upLevel>0 then
			player:push_lua_table("open",util.encode({name = "panel_levelTip",extend={exp=exp,upLevel=upLevel}}))
			giveExp(player)
		else
			player:push_lua_table("PanelRecycle", util.encode({cmd="recycleExp",exp=exp}))
		end
	end
	
	if startSort then
		player:push_lua_table("PanelLottery",util.encode({cmd="startSort"}))
		giveExp(player)
	end
end

function addRecycleLimit(player,value)
	local addExp = util.ppn(player, const.PP_RECYCLE_ADD_EXP)
	player:set_param(const.PP_RECYCLE_ADD_EXP, addExp + value)
	player:alert(1,1,"回收上限增加"..value.."！")
end

function addDeoptSize(player,value)
	player:add_depot_slot(value)
	player:alert(1,1,"仓库开启"..value.."格！")
end

--获得可回收的pos
function getEablePos(player)
	local startDay = server_start_day()+1
	local zLev = 0
	if startDay>=8 then 
		zLev=2
	end
	local result = {}
	for i=3000,3300 do
		local itemId = player:get_item_type_by_pos(i)
		if itemId and itemId>0 and tab.itemdef[itemId].equip_type>0 and tab.itemdef[itemId].equip_type<11 and tab.itemdef[itemId].need_zslevel<=zLev then  
			table.insert(result,#result+1,i)
		end
	end
	if #result>0 then
		onHuishouFunc(player,result,"startSort")
	else
		player:alert(1,1,"寻宝背包无可回收装备！")
	end
end

--计算累计经验超过限定的pos位不回收
function accumulationExp(player,posArr)
	local tempArr = {}
	local totalExp = util.ppn(player, const.PP_RECYCLE_TOTAL_EXP)
	local addExp = util.ppn(player, const.PP_RECYCLE_ADD_EXP)
	local exp=0
	for i=1,#posArr do
		local pos = posArr[i]
		local itemId = player:get_item_type_by_pos(pos)
		local item = player:get_item_by_pos(pos)
		local num=1
		if item then
			num = item:get_number()
		end
		if itemId and itemId > 0 then
			if (totalExp+exp)<(BASEEXP+addExp) then
				exp = exp + util.checknumber(tab.itemdef[itemId].recycle_exp)*num
				table.insert(tempArr,pos)
			end
		end
		if (totalExp+exp)>=(BASEEXP+addExp) then	
			break
		end
	end
	return tempArr
end