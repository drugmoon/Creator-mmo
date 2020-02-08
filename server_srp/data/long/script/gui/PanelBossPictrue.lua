module(... ,package.seeall)

local LH_MAX_LEVEL = 40--炼魂最高等级

--startid= endid 用途每次刷新对应图的时候去扫描背包有无对应碎片（做单块呼吸灯）
--picsCon--记录九张图的激活状态
local pictrueTable = {
	[1 ]={index=1 , name="尸王",     startid=33000001, endid=33000009, picsCon=const.PP_BOSS_PICTRUE1 },--数据存贮形式：{0,0,0,0,0,0,0,0,0,0}--1-9图片信息10:是否激活
	[2 ]={index=2 , name="娜迦",     startid=33000101, endid=33000109, picsCon=const.PP_BOSS_PICTRUE2 },
	[3 ]={index=3 , name="触龙邪神", startid=33000201, endid=33000209, picsCon=const.PP_BOSS_PICTRUE3 },
	[4 ]={index=4 , name="荒漠霸主", startid=33000301, endid=33000309, picsCon=const.PP_BOSS_PICTRUE4 },
	[5 ]={index=5 , name="血炼猪魔", startid=33000401, endid=33000409, picsCon=const.PP_BOSS_PICTRUE5 },
	[6 ]={index=6 , name="沃玛教主", startid=33000501, endid=33000509, picsCon=const.PP_BOSS_PICTRUE6 },
	[7 ]={index=7 , name="祖玛教主", startid=33000601, endid=33000609, picsCon=const.PP_BOSS_PICTRUE7 },
	[8 ]={index=8 , name="赤月老魔", startid=33000701, endid=33000709, picsCon=const.PP_BOSS_PICTRUE8 },
	[9 ]={index=9 , name="牛魔王",   startid=33000801, endid=33000809, picsCon=const.PP_BOSS_PICTRUE9 },
	[10]={index=10, name="骨魔教主", startid=33000901, endid=33000909, picsCon=const.PP_BOSS_PICTRUE10},
	[11]={index=11, name="冰封狮王", startid=33001001, endid=33001009, picsCon=const.PP_BOSS_PICTRUE11},
	[12]={index=12, name="冰宫祭司", startid=33001101, endid=33001109, picsCon=const.PP_BOSS_PICTRUE12},
	[13]={index=13, name="冰封魔王", startid=33001201, endid=33001209, picsCon=const.PP_BOSS_PICTRUE13},
	[14]={index=14, name="幽灵教主",     startid=33001301, endid=33001309, picsCon=const.PP_BOSS_PICTRUE14},
	[15]={index=15, name="蛮荒巨兽", startid=33001401, endid=33001409, picsCon=const.PP_BOSS_PICTRUE15},
	[16]={index=16, name="火龙王",   startid=33001501, endid=33001509, picsCon=const.PP_BOSS_PICTRUE16},
	[17]={index=17, name="九尾妖狐", startid=33001601, endid=33001609, picsCon=const.PP_BOSS_PICTRUE17},
	[18]={index=18, name="至尊帝皇", startid=33001701, endid=33001709, picsCon=const.PP_BOSS_PICTRUE18},
	[19]={index=19, name="深海幽龙王", startid=33001801, endid=33001809, picsCon=const.PP_BOSS_PICTRUE19},
}

local openTable = {
	[33000001]={key=1,index=1},
	[33000002]={key=1,index=2},
	[33000003]={key=1,index=3},
	[33000004]={key=1,index=4},
	[33000005]={key=1,index=5},
	[33000006]={key=1,index=6},
	[33000007]={key=1,index=7},
	[33000008]={key=1,index=8},
	[33000009]={key=1,index=9},

	[33000101]={key=2,index=1},
	[33000102]={key=2,index=2},
	[33000103]={key=2,index=3},
	[33000104]={key=2,index=4},
	[33000105]={key=2,index=5},
	[33000106]={key=2,index=6},
	[33000107]={key=2,index=7},
	[33000108]={key=2,index=8},
	[33000109]={key=2,index=9},

	[33000201]={key=3,index=1},
	[33000202]={key=3,index=2},
	[33000203]={key=3,index=3},
	[33000204]={key=3,index=4},
	[33000205]={key=3,index=5},
	[33000206]={key=3,index=6},
	[33000207]={key=3,index=7},
	[33000208]={key=3,index=8},
	[33000209]={key=3,index=9},

	[33000301]={key=4,index=1},
	[33000302]={key=4,index=2},
	[33000303]={key=4,index=3},
	[33000304]={key=4,index=4},
	[33000305]={key=4,index=5},
	[33000306]={key=4,index=6},
	[33000307]={key=4,index=7},
	[33000308]={key=4,index=8},
	[33000309]={key=4,index=9},

	[33000401]={key=5,index=1},
	[33000402]={key=5,index=2},
	[33000403]={key=5,index=3},
	[33000404]={key=5,index=4},
	[33000405]={key=5,index=5},
	[33000406]={key=5,index=6},
	[33000407]={key=5,index=7},
	[33000408]={key=5,index=8},
	[33000409]={key=5,index=9},

	[33000501]={key=6,index=1},
	[33000502]={key=6,index=2},
	[33000503]={key=6,index=3},
	[33000504]={key=6,index=4},
	[33000505]={key=6,index=5},
	[33000506]={key=6,index=6},
	[33000507]={key=6,index=7},
	[33000508]={key=6,index=8},
	[33000509]={key=6,index=9},

	[33000601]={key=7,index=1},
	[33000602]={key=7,index=2},
	[33000603]={key=7,index=3},
	[33000604]={key=7,index=4},
	[33000605]={key=7,index=5},
	[33000606]={key=7,index=6},
	[33000607]={key=7,index=7},
	[33000608]={key=7,index=8},
	[33000609]={key=7,index=9},

	[33000701]={key=8,index=1},
	[33000702]={key=8,index=2},
	[33000703]={key=8,index=3},
	[33000704]={key=8,index=4},
	[33000705]={key=8,index=5},
	[33000706]={key=8,index=6},
	[33000707]={key=8,index=7},
	[33000708]={key=8,index=8},
	[33000709]={key=8,index=9},

	[33000801]={key=9,index=1},
	[33000802]={key=9,index=2},
	[33000803]={key=9,index=3},
	[33000804]={key=9,index=4},
	[33000805]={key=9,index=5},
	[33000806]={key=9,index=6},
	[33000807]={key=9,index=7},
	[33000808]={key=9,index=8},
	[33000809]={key=9,index=9},

	[33000901]={key=10,index=1},
	[33000902]={key=10,index=2},
	[33000903]={key=10,index=3},
	[33000904]={key=10,index=4},
	[33000905]={key=10,index=5},
	[33000906]={key=10,index=6},
	[33000907]={key=10,index=7},
	[33000908]={key=10,index=8},
	[33000909]={key=10,index=9},

	[33001001]={key=11,index=1},
	[33001002]={key=11,index=2},
	[33001003]={key=11,index=3},
	[33001004]={key=11,index=4},
	[33001005]={key=11,index=5},
	[33001006]={key=11,index=6},
	[33001007]={key=11,index=7},
	[33001008]={key=11,index=8},
	[33001009]={key=11,index=9},

	[33001101]={key=12,index=1},
	[33001102]={key=12,index=2},
	[33001103]={key=12,index=3},
	[33001104]={key=12,index=4},
	[33001105]={key=12,index=5},
	[33001106]={key=12,index=6},
	[33001107]={key=12,index=7},
	[33001108]={key=12,index=8},
	[33001109]={key=12,index=9},

	[33001201]={key=13,index=1},
	[33001202]={key=13,index=2},
	[33001203]={key=13,index=3},
	[33001204]={key=13,index=4},
	[33001205]={key=13,index=5},
	[33001206]={key=13,index=6},
	[33001207]={key=13,index=7},
	[33001208]={key=13,index=8},
	[33001209]={key=13,index=9},

	[33001301]={key=14,index=1},
	[33001302]={key=14,index=2},
	[33001303]={key=14,index=3},
	[33001304]={key=14,index=4},
	[33001305]={key=14,index=5},
	[33001306]={key=14,index=6},
	[33001307]={key=14,index=7},
	[33001308]={key=14,index=8},
	[33001309]={key=14,index=9},

	[33001401]={key=15,index=1},
	[33001402]={key=15,index=2},
	[33001403]={key=15,index=3},
	[33001404]={key=15,index=4},
	[33001405]={key=15,index=5},
	[33001406]={key=15,index=6},
	[33001407]={key=15,index=7},
	[33001408]={key=15,index=8},
	[33001409]={key=15,index=9},

	[33001501]={key=16,index=1},
	[33001502]={key=16,index=2},
	[33001503]={key=16,index=3},
	[33001504]={key=16,index=4},
	[33001505]={key=16,index=5},
	[33001506]={key=16,index=6},
	[33001507]={key=16,index=7},
	[33001508]={key=16,index=8},
	[33001509]={key=16,index=9},

	[33001601]={key=17,index=1},
	[33001602]={key=17,index=2},
	[33001603]={key=17,index=3},
	[33001604]={key=17,index=4},
	[33001605]={key=17,index=5},
	[33001606]={key=17,index=6},
	[33001607]={key=17,index=7},
	[33001608]={key=17,index=8},
	[33001609]={key=17,index=9},

	[33001701]={key=18,index=1},
	[33001702]={key=18,index=2},
	[33001703]={key=18,index=3},
	[33001704]={key=18,index=4},
	[33001705]={key=18,index=5},
	[33001706]={key=18,index=6},
	[33001707]={key=18,index=7},
	[33001708]={key=18,index=8},
	[33001709]={key=18,index=9},

	[33001801]={key=19,index=1},
	[33001802]={key=19,index=2},
	[33001803]={key=19,index=3},
	[33001804]={key=19,index=4},
	[33001805]={key=19,index=5},
	[33001806]={key=19,index=6},
	[33001807]={key=19,index=7},
	[33001808]={key=19,index=8},
	[33001809]={key=19,index=9},
}

-----------------------------------------------------------拼图材料使用处理---------------------------------------------------------
function usePictrue(player,typeid)
	if not openTable[typeid] then return false end
	local itemData = openTable[typeid]
	if pictrueTable[itemData.key] then
		local itemData2 = pictrueTable[itemData.key]
		local result = player:get_param(itemData2.picsCon)
		if result==nil or result=="" then
			result={0,0,0,0,0,0,0,0,0,0}
		else
			result=util.decode(result)
		end
		if result[itemData.index]>=1 then
			player:alert(1,0,"该拼图已被使用过，不能重复使用！")
			return false
		end
		addPictureValue(player,typeid)
		result[itemData.index]=1
		player:set_param(itemData2.picsCon,util.encode(result))--从新写入修改的数据
		return true
	end
	return false
end

--使用掉图块后计算累计的属性
function addPictureValue(player,typeid)
	local ac   = util.checknumber(tab.itemdef[typeid].ac)
	local ac2  = util.checknumber(tab.itemdef[typeid].ac2)
	local mac  = util.checknumber(tab.itemdef[typeid].mac)
	local mac2 = util.checknumber(tab.itemdef[typeid].mac2)
	local dc   = util.checknumber(tab.itemdef[typeid].dc)
	local dc2  = util.checknumber(tab.itemdef[typeid].dc2)
	local mc   = util.checknumber(tab.itemdef[typeid].mc)
	local mc2  = util.checknumber(tab.itemdef[typeid].mc2)
	local sc   = util.checknumber(tab.itemdef[typeid].sc)
	local sc2  = util.checknumber(tab.itemdef[typeid].sc2)
	local hp   = util.checknumber(tab.itemdef[typeid].max_hp)
	-- print(ac,ac2,mac,mac2,dc,dc2,mc,mc2,sc,sc2,hp)
	if ac  >0 then player:set_param(const.PP_PIBTRUE_WF_MIN,util.ppn(player,const.PP_PIBTRUE_WF_MIN)+ac  ) end
	if ac2 >0 then player:set_param(const.PP_PIBTRUE_WF_Max,util.ppn(player,const.PP_PIBTRUE_WF_Max)+ac2 ) end
	if mac >0 then player:set_param(const.PP_PIBTRUE_MF_MIN,util.ppn(player,const.PP_PIBTRUE_MF_MIN)+mac ) end
	if mac2>0 then player:set_param(const.PP_PIBTRUE_MF_MAX,util.ppn(player,const.PP_PIBTRUE_MF_MAX)+mac2) end
	if dc  >0 then player:set_param(const.PP_PIBTRUE_WG_MIN,util.ppn(player,const.PP_PIBTRUE_WG_MIN)+dc  ) end
	if dc2 >0 then player:set_param(const.PP_PIBTRUE_WG_Max,util.ppn(player,const.PP_PIBTRUE_WG_Max)+dc2 ) end
	if mc  >0 then player:set_param(const.PP_PIBTRUE_MG_MIN,util.ppn(player,const.PP_PIBTRUE_MG_MIN)+mc  ) end
	if mc2 >0 then player:set_param(const.PP_PIBTRUE_MG_Max,util.ppn(player,const.PP_PIBTRUE_MG_Max)+mc2 ) end
	if sc  >0 then player:set_param(const.PP_PIBTRUE_DG_MIN,util.ppn(player,const.PP_PIBTRUE_DG_MIN)+sc  ) end
	if sc2 >0 then player:set_param(const.PP_PIBTRUE_DG_Max,util.ppn(player,const.PP_PIBTRUE_DG_Max)+sc2 ) end
	if hp  >0 then player:set_param(const.PP_PIBTRUE_HP    ,util.ppn(player,const.PP_PIBTRUE_HP    )+hp  ) end

	--设置使用后的属性
	local maxhp = player:get_script_attr(2)
	local minac=player:get_script_attr(3)
	local maxac=player:get_script_attr(4)
	local minmac=player:get_script_attr(5)
	local maxmac=player:get_script_attr(6)

	local mindc=player:get_script_attr(7)
	local maxdc=player:get_script_attr(8)
	local minmc=player:get_script_attr(9)
	local maxmc=player:get_script_attr(10)
	local minsc=player:get_script_attr(11)
	local maxsc=player:get_script_attr(12)

	player:set_script_attr(3,minac+ac);--增加最小物理防御
	player:set_script_attr(4,maxac+ac2);--增加最大物理防御
	player:set_script_attr(5,minmac+mac);--增加最小魔法防御
	player:set_script_attr(6,maxmac+mac2);--增加最大魔法防御
		--if job == "warrior" then
	player:set_script_attr(7,mindc+dc);--增加最小物理攻击
	player:set_script_attr(8,maxdc+dc2);--增加最大物理攻击
	--elseif job == "wizard" then
	player:set_script_attr(9, minmc+mc);--增加最小魔法攻击
	player:set_script_attr(10,maxmc+mc2);--增加最大魔法攻击
	--elseif job == "taoist" then
	player:set_script_attr(11,minsc+sc);--增加最小道术攻击
	player:set_script_attr(12,maxsc+sc2);--增加最大道术攻击

	player:set_script_attr(2,maxhp+hp);--血量
	player:recal_attr()
	player:push_lua_table("PanelBossPictrue",util.encode({cmd="pinTuUseSuccessed"}))
end

--上线设置拼图的累计属性
function setPinTuAllValue(player)
	local ac   = util.ppn(player,const.PP_PIBTRUE_WF_MIN)
	local ac2  = util.ppn(player,const.PP_PIBTRUE_WF_Max)
	local mac  = util.ppn(player,const.PP_PIBTRUE_MF_MIN)
	local mac2 = util.ppn(player,const.PP_PIBTRUE_MF_MAX)
	local dc   = util.ppn(player,const.PP_PIBTRUE_WG_MIN)
	local dc2  = util.ppn(player,const.PP_PIBTRUE_WG_Max)
	local mc   = util.ppn(player,const.PP_PIBTRUE_MG_MIN)
	local mc2  = util.ppn(player,const.PP_PIBTRUE_MG_Max)
	local sc   = util.ppn(player,const.PP_PIBTRUE_DG_MIN)
	local sc2  = util.ppn(player,const.PP_PIBTRUE_DG_Max)
	local hp   = util.ppn(player,const.PP_PIBTRUE_HP    )

	local maxhp = player:get_script_attr(2)
	local minac=player:get_script_attr(3)
	local maxac=player:get_script_attr(4)
	local minmac=player:get_script_attr(5)
	local maxmac=player:get_script_attr(6)

	local mindc=player:get_script_attr(7)
	local maxdc=player:get_script_attr(8)
	local minmc=player:get_script_attr(9)
	local maxmc=player:get_script_attr(10)
	local minsc=player:get_script_attr(11)
	local maxsc=player:get_script_attr(12)

	player:set_script_attr(3,minac+ac);--增加最小物理防御
	player:set_script_attr(4,maxac+ac2);--增加最大物理防御
	player:set_script_attr(5,minmac+mac);--增加最小魔法防御
	player:set_script_attr(6,maxmac+mac2);--增加最大魔法防御
		--if job == "warrior" then
	player:set_script_attr(7,mindc+dc);--增加最小物理攻击
	player:set_script_attr(8,maxdc+dc2);--增加最大物理攻击
	--elseif job == "wizard" then
	player:set_script_attr(9, minmc+mc);--增加最小魔法攻击
	player:set_script_attr(10,maxmc+mc2);--增加最大魔法攻击
	--elseif job == "taoist" then
	player:set_script_attr(11,minsc+sc);--增加最小道术攻击
	player:set_script_attr(12,maxsc+sc2);--增加最大道术攻击

	player:set_script_attr(2,maxhp+hp);--血量
	player:recal_attr()
end
login.add_login_listener(setPinTuAllValue)

-----------------------------------------------------------拼图材料使用end---------------------------------------------------------
function handlePanelData(player,commond)
	local data = util.decode(commond)
	-- print(data.actionid,commond)
	if data.actionid == "reqLinHunBuyData" then--炼魂购买
		senderLinHunData(player)
	elseif data.actionid == "reqPinTuData" then
		senderToClient(player,data.params.index)
	elseif data.actionid == "reqFristPinTuData" then
		senderToClient(player,findNotOpenIndex(player))

	elseif data.actionid == "reqJiHuoPictrue" then--激活指定拼图
		startJiHuoPictrue(player,data.params.index)
	elseif data.actionid=="repOpenPinTuMask" then
		openPinTuMask(player,data.params.index,data.params.maskid)
	elseif data.actionid=="reqCheckRedPoint" then
		gui.moduleRedPoint.checkPinTuUsable(player)

	----------------------------------炼魂通讯---------------------------------------
	elseif data.actionid=="reqLianHunData" then
		updateLianHunClient(player,false)
	elseif data.actionid=="shengJiLianHun" then
		startUpLianHun(player)

	----------------------------------拼图回收---------------------------------------
	elseif data.actionid=="startHuiShow" then
		huiShouPinTu(player,data.params.posArr)
	end
end
ckpanel.add_listener("gui.PanelBossPictrue.handlePanelData",gui.PanelBossPictrue.handlePanelData)

--刷新拼图数据
function senderToClient(player,index,upSucceed)
	local result = {
		cmd="updatePinTuData",
		ac  = util.ppn(player,const.PP_PIBTRUE_WF_MIN),
		ac2 = util.ppn(player,const.PP_PIBTRUE_WF_Max),
		mac = util.ppn(player,const.PP_PIBTRUE_MF_MIN),
		mac2= util.ppn(player,const.PP_PIBTRUE_MF_MAX),
		dc  = util.ppn(player,const.PP_PIBTRUE_WG_MIN),
		dc2 = util.ppn(player,const.PP_PIBTRUE_WG_Max),
		mc  = util.ppn(player,const.PP_PIBTRUE_MG_MIN),
		mc2 = util.ppn(player,const.PP_PIBTRUE_MG_Max),
		sc  = util.ppn(player,const.PP_PIBTRUE_DG_MIN),
		sc2 = util.ppn(player,const.PP_PIBTRUE_DG_Max),
		hp  = util.ppn(player,const.PP_PIBTRUE_HP    ),
		upSucceed=upSucceed or false,
		index=index,
		redData=checkRedPointChange(player),
	}
	if not index or index<1 or index>#pictrueTable then
		index=1
	end
	local infoTable = player:get_param(pictrueTable[index].picsCon)
	-- print(infoTable,"11111111111111")
	if infoTable==nil or infoTable=="" then
		infoTable={0,0,0,0,0,0,0,0,0,0}
	else
		infoTable=util.decode(infoTable)
	end
	local haveState = {}
	for i=pictrueTable[index].startid,pictrueTable[index].endid do
		local j = i-pictrueTable[index].startid+1
		if player:num_item(i)>0 and infoTable[j]==0 then
			haveState[j]=i
		else
			haveState[j]=0
		end
	end
	result.bossName=pictrueTable[index].name
	result.picName=pictrueTable[index].index
	result.picInfo=infoTable
	result.haveState=haveState
	player:push_lua_table("PanelBossPictrue",util.encode(result))
end

--激活指定拼图
function startJiHuoPictrue(player,index)
	if not pictrueTable[index] then
		return player:alert(1,0,"数据异常！操作失败！")
	end
	local infoTable = player:get_param(pictrueTable[index].picsCon)
	if infoTable==nil or infoTable=="" then
		infoTable={0,0,0,0,0,0,0,0,0,0}
	else
		infoTable=util.decode(infoTable)
	end
	if infoTable[10]>=1 then
		return player:alert(1,0,"已激活！不能重复操作！")
	end
	for i=1,9 do
		if infoTable[i]<=0 then
			return player:alert(1,0,"集齐9张拼图才可激活！")
		end
	end
	infoTable[10]=1--标记为激活
	player:set_param(pictrueTable[index].picsCon,util.encode(infoTable))--从新写入修改的数据
	player:alert(1,0,"成功激活该拼图！")
	senderToClient(player,index,true)
	gui.moduleRedPoint.checkPinTuUsable(player)
end

--点击遮罩快开启遮罩
function openPinTuMask(player,index,typeid)
	if typeid and typeid>=33000001 and typeid<=33001809 then
		-- player:use_item(typeid)
		for i=0,60 do
			local itemId = player:get_item_type_by_pos(i)
			if itemId and itemId==typeid then
				if 0 == player:remove_item_by_type_pos(typeid,i,1) then
					usePictrue(player,typeid)
					gui.moduleRedPoint.checkPinTuUsable(player)
					break
				end
			end
		end
	end
end

--首次打开寻找第一个未激活的拼图index
function findNotOpenIndex(player)
	for i=1,#pictrueTable do
		local itemData = pictrueTable[i]
		local result = player:get_param(itemData.picsCon)
		if result==nil or result=="" then
			result={0,0,0,0,0,0,0,0,0,0}
		else
			result=util.decode(result)
		end
		if result[10]<=0 then
			return i
		end
	end
	return 1
end

----------------------------------------------炼魂模块------------------------------------------------
--刷新炼魂数据
function updateLianHunClient(player,isSucceed)
	local result = {
		cmd="updateLianHunData",
		curLevel= util.ppn(player,const.PP_LIANHUN_LEVEL),
		ownExp= util.ppn(player,const.PP_LIANHUN_OWNEXP),
		upSucceed=isSucceed,
	}
	if result.curLevel>0 then
		result.dataTable=tab.lianHunCsv[tostring(result.curLevel)]
	end
	if (result.curLevel+1)<=40 then
		result.needExp=tab.lianHunCsv[tostring(result.curLevel+1)].need
	else
		result.needExp=tab.lianHunCsv[tostring(LH_MAX_LEVEL)].need
	end
	-- print(util.encode(result))

	player:push_lua_table("PanelBossPictrue",util.encode(result))
end

--炼魂升级逻辑
function startUpLianHun(player)
	local curLevel= util.ppn(player,const.PP_LIANHUN_LEVEL)
	if curLevel>=LH_MAX_LEVEL then
		return player:alert(1,0,"炼魂已达最大等级！")
	end
	local ownExp= util.ppn(player,const.PP_LIANHUN_OWNEXP)
	local curData = {}
	if curLevel>0 then
		curData=tab.lianHunCsv[tostring(curLevel)]
	end
	local nextData = tab.lianHunCsv[tostring(curLevel+1)]
	if ownExp<tonumber(nextData.need) then
		return player:alert(1,0,"炼魂值不足,无法升级！")
	end
	player:set_param(const.PP_LIANHUN_LEVEL,curLevel+1)
	player:set_param(const.PP_LIANHUN_OWNEXP,ownExp-tonumber(nextData.need))
	updateLianHunClient(player,true)
	succeedSetLianHunValue(player,curData,nextData)
	gui.moduleRedPoint.checkLianHunUsable(player)
end

--升级成功设置属性
function succeedSetLianHunValue(player,curData,nextData)
	local maxhp =player:get_script_attr(2)
	local minac =player:get_script_attr(3)
	local maxac =player:get_script_attr(4)
	local minmac=player:get_script_attr(5)
	local maxmac=player:get_script_attr(6)

	local mindc=player:get_script_attr(7)
	local maxdc=player:get_script_attr(8)
	local minmc=player:get_script_attr(9)
	local maxmc=player:get_script_attr(10)
	local minsc=player:get_script_attr(11)
	local maxsc=player:get_script_attr(12)

	local job = player:get_job_name()

	player:set_script_attr(3,minac +(nextData.wfmin-(curData.wfmin or 0)));--增加最小物理防御
	player:set_script_attr(4,maxac +(nextData.wfmax-(curData.wfmax or 0)));--增加最大物理防御
	player:set_script_attr(5,minmac+(nextData.mfmin-(curData.mfmin or 0)));--增加最小魔法防御
	player:set_script_attr(6,maxmac+(nextData.mfmax-(curData.mfmax or 0)));--增加最大魔法防御
		--if job == "warrior" then
	player:set_script_attr(7,mindc+(nextData.wgmin-(curData.wgmin or 0)));--增加最小物理攻击
	player:set_script_attr(8,maxdc+(nextData.wgmax-(curData.wgmax or 0)));--增加最大物理攻击
	--elseif job == "wizard" then
	player:set_script_attr(9, minmc+(nextData.mgmin-(curData.mgmin or 0)));--增加最小魔法攻击
	player:set_script_attr(10,maxmc+(nextData.mgmax-(curData.mgmax or 0)));--增加最大魔法攻击
	--elseif job == "taoist" then
	player:set_script_attr(11,minsc+(nextData.dgmin-(curData.dgmin or 0)));--增加最小道术攻击
	player:set_script_attr(12,maxsc+(nextData.dgmax-(curData.dgmax or 0)));--增加最大道术攻击
	if job == "warrior" then
		player:set_script_attr(2,maxhp+(nextData.zhanhp-(curData.zhanhp or 0)));--血量
	elseif job == "wizard" then
		player:set_script_attr(2,maxhp+(nextData.fahp-(curData.fahp or 0)));--血量
	elseif job == "taoist" then
		player:set_script_attr(2,maxhp+(nextData.daohp-(curData.daohp or 0)));--血量
	end
	player:recal_attr()
end

--上线设置炼魂属性
function setLianHunValue(player)
	local curLevel= util.ppn(player,const.PP_LIANHUN_LEVEL)
	if curLevel<=0 then return end
	local curData=tab.lianHunCsv[tostring(curLevel)]

	local maxhp =player:get_script_attr(2)
	local minac =player:get_script_attr(3)
	local maxac =player:get_script_attr(4)
	local minmac=player:get_script_attr(5)
	local maxmac=player:get_script_attr(6)

	local mindc=player:get_script_attr(7)
	local maxdc=player:get_script_attr(8)
	local minmc=player:get_script_attr(9)
	local maxmc=player:get_script_attr(10)
	local minsc=player:get_script_attr(11)
	local maxsc=player:get_script_attr(12)

	local job = player:get_job_name()

	player:set_script_attr(3,minac + curData.wfmin);--增加最小物理防御
	player:set_script_attr(4,maxac + curData.wfmax);--增加最大物理防御
	player:set_script_attr(5,minmac+ curData.mfmin);--增加最小魔法防御
	player:set_script_attr(6,maxmac+ curData.mfmax);--增加最大魔法防御
		--if job == "warrior" then
	player:set_script_attr(7,mindc+curData.wgmin);--增加最小物理攻击
	player:set_script_attr(8,maxdc+curData.wgmax);--增加最大物理攻击
	--elseif job == "wizard" then
	player:set_script_attr(9, minmc+curData.mgmin);--增加最小魔法攻击
	player:set_script_attr(10,maxmc+curData.mgmax);--增加最大魔法攻击
	--elseif job == "taoist" then
	player:set_script_attr(11,minsc+curData.dgmin);--增加最小道术攻击
	player:set_script_attr(12,maxsc+curData.dgmax);--增加最大道术攻击
	if job == "warrior" then
		player:set_script_attr(2,maxhp+curData.zhanhp);--血量
	elseif job == "wizard" then
		player:set_script_attr(2,maxhp+curData.fahp);--血量
	elseif job == "taoist" then
		player:set_script_attr(2,maxhp+curData.daohp);--血量
	end
	player:recal_attr()
end
login.add_login_listener(setLianHunValue)

----------------------------------------------拼图回收------------------------------------------------
function huiShouPinTu(player,posTable)
	if not posTable or #posTable<=0 then return end
	local wuHunNum= 0
	for k,v in pairs(posTable) do
		local itemId = player:get_item_type_by_pos(v)
		local item = player:get_item_by_pos(v)
		local num=1
		if item then
			num = item:get_number()
		end
		if itemId and itemId>=33000001 and itemId<=33001809 then
			if 0 == player:remove_item_by_type_pos(itemId,v,num) then
				wuHunNum = wuHunNum + util.checknumber(tab.itemdef[itemId].func_params)*num
			end
		end
		-- print(itemId,wuHunNum,"====1111111111")
	end
	if wuHunNum>0 then
		player:set_param(const.PP_LIANHUN_OWNEXP,util.ppn(player,const.PP_LIANHUN_OWNEXP)+wuHunNum)
		player:alert(1,0,"碎片回收成功！获得武魂值："..wuHunNum)
		player:push_lua_table("PanelBossPictrue",util.encode({cmd="huiShouSuccessed"}))
		gui.moduleRedPoint.checkLianHunUsable(player)
		gui.moduleRedPoint.checkPinTuUsable(player)
		updateLianHunClient(player,false)
	end
end

----------------------------------------------炼魂购买------------------------------------------------
function senderLinHunData(player)
	local result = {
		cmd="senderLinHunData",
		data=tab.Store.getItemsByType(player,13),
	}
	player:push_lua_table("PanelBossPictrue",util.encode(result))
end

function buyLinHun(player,storeId)
	-- print(util.encode(tab.Store.getItemsByType(player,6)))
	local itemData = tab.Store.buyShopItem(player,storeId,"炼魂购买",1,"notinbag")
	if itemData then
		-- updateClientSw(player)
	end
end

function addLianHun(player,value)
	player:set_param(const.PP_LIANHUN_OWNEXP,util.ppn(player,const.PP_LIANHUN_OWNEXP)+value)
	player:alert(1,0,"获得武魂值："..value)
	updateLianHunClient(player,false)
	gui.moduleRedPoint.checkLianHunUsable(player)
end


----------------------------------------------红点检测------------------------------------------------
--炼魂红点检测
function checkRedPointLianHun(player)
	local curLevel= util.ppn(player,const.PP_LIANHUN_LEVEL)
	if curLevel>=LH_MAX_LEVEL then
		return false
	end
	local ownExp= util.ppn(player,const.PP_LIANHUN_OWNEXP)
	local nextData = tab.lianHunCsv[tostring(curLevel+1)]
	if ownExp>=tonumber(nextData.need) then
		return true
	end
	return false
end

--拼图红点
function checkRedPointPinTu(player)
	for i=1,#pictrueTable do
		local itemData = pictrueTable[i]
		local result = player:get_param(itemData.picsCon)
		if result==nil or result=="" then
			result={0,0,0,0,0,0,0,0,0,0}
		else
			result=util.decode(result)
		end
		for i=1,9 do
			local typeid = itemData.startid+i-1
			local ishave = player:num_item(typeid)
			if ishave>0 and result[i]<=0 then
				return true
			end
		end
		if result[10]==0 then
			local openNum = 0
			for i=1,9 do
				if result[i]>0 then
					openNum=openNum+1
				end
			end
			if openNum>=9 then
				return true
			end
		end
	end
	return false
	-- [2 ]={index=2 , name="娜迦",     startid=33000101, endid=33000109, picsCon=const.PP_BOSS_PICTRUE2 },
end

--拼图左右键红点检测
function checkRedPointChange(player)
	local redInfo={}
	for index=1,#pictrueTable do
		local infoTable = player:get_param(pictrueTable[index].picsCon)
		-- print(infoTable,"11111111111111")
		if infoTable==nil or infoTable=="" then
			infoTable={0,0,0,0,0,0,0,0,0,0}
		else
			infoTable=util.decode(infoTable)
		end
		for i=pictrueTable[index].startid,pictrueTable[index].endid do
			local j = i-pictrueTable[index].startid+1
			if player:num_item(i)>0 and infoTable[j]==0 then
				redInfo[index]=i--表示改图有可以激活的
				break
			else
				redInfo[index]=0
			end
		end
		if redInfo[index]==0 and infoTable[10]==0 then
			local openNum = 0
			for i=1,9 do
				if infoTable[i]>0 then
					openNum=openNum+1
				end
			end
			if openNum>=9 then
				redInfo[index]=1
			end
		end
	end
	return redInfo
end















