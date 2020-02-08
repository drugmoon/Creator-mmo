module(..., package.seeall)

util.use_function(_M,task.task1000);

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
	showPanelTelePort(player)
end

local btnHelfSize = 197 --按钮控件的宽的一半
local tempA = -150
local offX = 210
local offY = -65
local baseH = 470
local startPosy = 445
local talk_tab = {
	func = "npc.mapmove2.btnCall",

	scrollHeight = baseH+tempA,
	fixed_tab = {
		{mtype="button", name="新手村",	level = 1, fontSize = 20,	w=150, funcIndex="101",  res="btn_new21_sel",	posX=124,	posY=490, telePortId = 4010001},
		{mtype="button", name="盟重",	level = 20, fontSize = 20,	w=150, funcIndex="103",  res="btn_new21_sel",	posX=321,	posY=490, telePortId = 4010003},
		{mtype="button", name="比奇城",	level = 50, fontSize = 20,	w=150, funcIndex="102",  res="btn_new21_sel",	posX=518,	posY=490, telePortId = 4010002},
		{mtype="button", name="沙巴克",	level = 60, fontSize = 20,	w=150, funcIndex="104",  res="btn_new21_sel",	posX=715,	posY=490, telePortId = 4010004},
		
		{mtype="button", funcIndex="105",  res="btn_normal_hook",	posX=422,	posY=58, telePortId = 4170001},
		-- {mtype="button", funcIndex="106",  res="btn_vip_hook",		posX=624,	posY=58, telePortId = {4170002, 4170003, 4170004, 4170005}},
	},
	con_tab = {	 
		{mtype="button", name="矿洞",	  	level=30, fontSize = 18,	w=150, funcIndex="1",  res="btn_new21_sel",	posX=100 + 0* btnHelfSize,	posY=startPosy+(0 * offY)+tempA, telePortId = 4010005},
		{mtype="button", name="毒蛇山谷", 	level=40, fontSize = 18,	w=150, funcIndex="2",  res="btn_new21_sel",	posX=100 + 1* btnHelfSize,	posY=startPosy+(0 * offY)+tempA, telePortId = 4010006},
		{mtype="button", name="死亡山谷", 	level=50, fontSize = 18,	w=150, funcIndex="3",  res="btn_new21_sel",	posX=100 + 2* btnHelfSize,	posY=startPosy+(0 * offY)+tempA, telePortId = 4010007},
		{mtype="button", name="沙漠",	  	level=60, fontSize = 18,	w=150, funcIndex="4",  res="btn_new21_sel",	posX=100 + 3* btnHelfSize,	posY=startPosy+(0 * offY)+tempA, telePortId = 4010008},
 
		{mtype="button", name="猪洞",		level=70, fontSize = 18,	w=150, funcIndex="5",  res="btn_new21_sel",	posX=100 + 0* btnHelfSize,	posY=startPosy+(1 * offY)+tempA, telePortId = 4010009},
		{mtype="button", name="沃玛寺庙",	level=70, fontSize = 18,	w=150, funcIndex="6",  res="btn_new21_sel",	posX=100 + 1* btnHelfSize,	posY=startPosy+(1 * offY)+tempA, telePortId = 4010010},
		{mtype="button", name="祖玛寺庙",	level=80, fontSize = 18,	w=150, funcIndex="7",  res="btn_new21_sel",	posX=100 + 2* btnHelfSize,	posY=startPosy+(1 * offY)+tempA, telePortId = 4010011},
		{mtype="button", name="赤月",		level=80, fontSize = 18,	w=150, funcIndex="8",  res="btn_new21_sel",	posX=100 + 3* btnHelfSize,	posY=startPosy+(1 * offY)+tempA, telePortId = 4010012},
 
		{mtype="button", name="牛魔寺庙",	zslevel=1, fontSize = 18,	w=150, funcIndex="9",  res="btn_new21_sel",	posX=100 + 0* btnHelfSize,	posY=startPosy+(2 * offY)+tempA, telePortId = 4010013},
		{mtype="button", name="雪域冰宫",	zslevel=1, fontSize = 18,	w=150, funcIndex="10",  res="btn_new21_sel",posX=100 + 1* btnHelfSize,	posY=startPosy+(2 * offY)+tempA, telePortId = 4010014},
		{mtype="button", name="幽灵船",		zslevel=2, fontSize = 18,	w=150, funcIndex="11",  res="btn_new21_sel",posX=100 + 2* btnHelfSize,	posY=startPosy+(2 * offY)+tempA, telePortId = 4010015},
		{mtype="button", name="魔龙血域",	zslevel=2, fontSize = 18,	w=150, funcIndex="12",  res="btn_new21_sel",posX=100 + 3* btnHelfSize,	posY=startPosy+(2 * offY)+tempA, telePortId = 4010016},
 
		{mtype="button", name="雷炎",		zslevel=3, fontSize = 18,	w=150, funcIndex="13",  res="btn_new21_sel",posX=100 + 0* btnHelfSize,	posY=startPosy+(3 * offY)+tempA, telePortId = 4010017},
		{mtype="button", name="妖狐巢穴",	zslevel=3, fontSize = 18,	w=150, funcIndex="14",  res="btn_new21_sel",posX=100 + 1* btnHelfSize,	posY=startPosy+(3 * offY)+tempA, telePortId = 4010018},
		{mtype="button", name="皇陵",		zslevel=3, fontSize = 18,	w=150, funcIndex="15",  res="btn_new21_sel",posX=100 + 2* btnHelfSize,	posY=startPosy+(3 * offY)+tempA, telePortId = 4010019},
		{mtype="button", name="海底",		zslevel=3, fontSize = 18,	w=150, funcIndex="16",  res="btn_new21_sel",posX=100 + 3* btnHelfSize,	posY=startPosy+(3 * offY)+tempA, telePortId = 4010020},
		{mtype="button", name="地狱牢笼",	zslevel=3, fontSize = 18,	w=150, funcIndex="17",  res="btn_new21_sel",posX=100 + 0* btnHelfSize,	posY=startPosy+(4 * offY)+tempA, telePortId = 4010026},
	},
}

for i,v in ipairs(talk_tab.fixed_tab) do
	if v.level then
		v.name = v.name.."("..v.level.."级)";
	elseif v.zslevel then
		v.name = v.name.."(转"..v.zslevel..")";
	end
end
for i,v in ipairs(talk_tab.con_tab) do
	if v.level then
		v.name = v.name.."("..v.level.."级)";
	elseif v.zslevel then
		v.name = v.name.."(转"..v.zslevel..")";
	end
end

function showPanelTelePort(player,stone)
	if stone then
		talk_tab.func = "npc.mapmove2.btnCall2"
	else
		talk_tab.func = "npc.mapmove2.btnCall"
	end
	player:push_lua_table("talk_npc",util.encode(talk_tab))
end

function btnCall(player,data)
	-- print("btnCall", data)
	local nor = data
	local flyId;
	local conf;
	for i,v in ipairs(talk_tab.fixed_tab) do
		if v.funcIndex == nor then
			conf = v;
			flyId = v.telePortId;
			break;
		end
	end
	if not flyId then
		for i,v in ipairs(talk_tab.con_tab) do
			if v.funcIndex == nor then
				conf = v;
				flyId = v.telePortId;
			end
		end
	end
	if type(flyId) == "table" then
		local vipLv = util.ppn(player, const.PP_VIP_LEVEL)
		flyId = flyId[math.ceil(4*vipLv/12)]
	end
	if flyId and util.isNumber(flyId) then
		if conf.level and player:get_level() < conf.level then
			return player:alert(1, 1, conf.level.."级后才能进入该地图")
		end
		if conf.zslevel and util.ppn(player,const.PP_ZHSH_LEVEL) < conf.zslevel then
			return player:alert(1, 1, "转"..conf.zslevel.."后才能进入该地图")
		end

		return posmap.fly(player, flyId)
	end
end

function btnCall2(player,data)
	if player:num_item("传送石")>=1 then
		if btnCall(player,data) then
			player:remove_item("传送石",1)
		end
	end
end

ckpanel.add_listener("npc.mapmove2.btnCall",npc.mapmove2.btnCall)
ckpanel.add_listener("npc.mapmove2.btnCall2",npc.mapmove2.btnCall2)