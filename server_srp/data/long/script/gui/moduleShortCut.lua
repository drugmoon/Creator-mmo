module(..., package.seeall)
-------------快捷使用-------------
local CONFIRM_TAG = {
	TRANSFER = 1,
	MEDICINE = 2,
	EXIT = 3,
}
local PRICE = { ---快捷功能价格
	TRANSFER = 1, ---传送
	MEDICINE = 1, ---血药
}
local MEDICINE_HP = 1000 -- 使用回血功能的回血量
------------------------喝血药------------------------
function reqShortCut(player, commond)
	local data = util.decode(commond); 
	if data.actionid == "transfer" then
		if player:num_item(10045, 1) then
			player:go_home();
		else
			showConfirm(player, CONFIRM_TAG.TRANSFER)
		end
	elseif data.actionid == "random" then
		if player:num_item(10046,1)>0 then
			player:use_item(10046)
		end
	elseif data.actionid == "medicine" then
		showConfirm(player, CONFIRM_TAG.MEDICINE)
	elseif data.actionid == "useTransfer" then
		useTransfer(player)
	elseif data.actionid == "useMedicine" then
		useMedicine(player)

	elseif data.actionid == "exitCopy" then
		showConfirm(player, CONFIRM_TAG.EXIT)
	elseif data.actionid == "useExit" then
		useExit(player)
	end
end

function useTransfer(player)
	player:gen_slave(0,"圣灵勇士",1);
	if player:get_vcoin() >= PRICE.TRANSFER then
		player:sub_vcoin(PRICE.TRANSFER, "快捷回城")
		player:alert(1, 1, "使用快捷传送,消耗"..PRICE.TRANSFER.."元宝")
		if string.sub(player:get_map():get_id(),1,3) =="xmt" then
			local level = tonumber(player:get_map():get_var("talevel"))
			npc.xinmota.ongetCurLevelAward(player,level)
		end
		player:go_home()

		player:push_lua_table("Transfer","");
	else
		player:alert(1, 1, "您的元宝不足,无法使用快捷回城")
	end
end

function useMedicine(player)
	if player:get_vcoin() >= PRICE.MEDICINE then
		player:sub_vcoin(PRICE.TRANSFER, "快捷回血")
		player:alert(1, 1, "使用快捷回血,消耗"..PRICE.MEDICINE.."元宝")

		player:push_lua_table("Medicine","");
		-- 回血方法
		player:add_hp(MEDICINE_HP)
		player:notify_hpmpchange(MEDICINE_HP)
	else
		player:alert(1, 1, "您的元宝不足,无法使用快捷回血")
	end
end

function useExit(player)
	player:go_home()
end
------------------------回城传送------------------------


function showConfirm(player, pTag)
	local result
	if pTag ==  CONFIRM_TAG.TRANSFER then
		result =  {
			panel = "confirm",
			visible = true,
			lblConfirm = "使用传送功能将消耗"..PRICE.TRANSFER.."元宝",
			path = "gui.moduleShortCut.reqShortCut",
			actionid = "useTransfer",
			strMsg = "shortCutTransfer",
		}
	elseif pTag == CONFIRM_TAG.MEDICINE then
		result = {
			panel = "confirm",
			visible = true,
			lblConfirm = "使用回血功能将消耗"..PRICE.MEDICINE.."元宝",
			path = "gui.moduleShortCut.reqShortCut",
			actionid = "useMedicine",
			strMsg = "shortCutMedicine",
		}
	elseif pTag == CONFIRM_TAG.EXIT then
		result = {
			panel = "confirm",
			visible = true,
			lblConfirm = "是否确认退出地图？",
			path = "gui.moduleShortCut.reqShortCut",
			actionid = "useExit",
			strMsg = "shortCutExit",
		}
	end
	if result then player:push_lua_table("alert",util.encode(result)) end
end

ckpanel.add_listener("gui.moduleShortCut.reqShortCut",gui.moduleShortCut.reqShortCut)