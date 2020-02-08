module(..., package.seeall)

--1级攻击宝石  1级物防宝石 1级魔防宝石 1级生命宝石 1级魔法宝石 1级神圣宝石 1级暴击宝石 1级暴伤宝石

function onelevelgem(player)
	local gemTable = {"1级攻击宝石", "1级物防宝石", "1级魔防宝石", "1级生命宝石", "1级魔法宝石", "1级神圣宝石", "1级暴击宝石", "1级暴伤宝石"}
	if player:num_bag_black()>1 then
		local random = math.random(1,#gemTable)
		player:add_item(gemTable[random],1,0)
		player:alert(1,1,"成功获得："..gemTable[random])
	else
		player:alert(1,1,"背包空间不足！")
		return 0
	end
end

function twolevelgem(player)
	local gemTable = {"2级攻击宝石", "2级物防宝石", "2级魔防宝石", "2级生命宝石", "2级魔法宝石", "2级神圣宝石", "2级暴击宝石", "2级暴伤宝石"}
	if player:num_bag_black()>1 then
		local random = math.random(1,#gemTable)
		player:add_item(gemTable[random],1,0)
		player:alert(1,1,"成功获得："..gemTable[random])
	else
		player:alert(1,1,"背包空间不足！")
		return 0
	end
end

function threelevelgem(player)
	local gemTable = {"3级攻击宝石", "3级物防宝石", "3级魔防宝石", "3级生命宝石", "3级魔法宝石", "3级神圣宝石", "3级暴击宝石", "3级暴伤宝石"}
	if player:num_bag_black()>1 then
		local random = math.random(1,#gemTable)
		player:add_item(gemTable[random],1,0)
		player:alert(1,1,"成功获得："..gemTable[random])
	else
		player:alert(1,1,"背包空间不足！")
		return 0
	end
end

function fourlevelgem(player)
	local gemTable = {"4级攻击宝石", "4级物防宝石", "4级魔防宝石", "4级生命宝石", "4级魔法宝石", "4级神圣宝石", "4级暴击宝石", "4级暴伤宝石"}
	if player:num_bag_black()>1 then
		local random = math.random(1,#gemTable)
		player:add_item(gemTable[random],1,0)
		player:alert(1,1,"成功获得："..gemTable[random])
	else
		player:alert(1,1,"背包空间不足！")
		return 0
	end
end

