module(..., package.seeall)
--------翅膀-------
local pos = -13*2; -- 翅膀pos

local COMFIRM_TAG = {
	FEATHER_LACK = 1,
	LINGYIGU_LACK = 2,
	VCOIN_LACK = 3,
}
local MAX_WING_LV = 12;
local VCOIN_PER_FEATHER = tonumber(tab.store[tostring(11001)].money);

--下面表格为了兼容之前升级数据，祝福值转成现在的星级

-- local upgrade_table = {
-- 	[1] = {FPT =5,		maxValue = 18,		timesMin=3,		timesMax=3,		blessMin=4,		blessMax=6,		random=5000,},
-- 	[2] = {FPT =5,		maxValue = 72,		timesMin=8,		timesMax=12,	blessMin=4,		blessMax=6,		random=834,},
-- 	[3] = {FPT =10,		maxValue = 360,		timesMin=20,	timesMax=30,	blessMin=8,		blessMax=12,	random=334,},
-- 	[4] = {FPT =20,		maxValue = 960	,	timesMin=28,	timesMax=40,	blessMin=16,	blessMax=24,	random=122,},
-- 	[5] = {FPT =30,		maxValue = 2520	,	timesMin=50,	timesMax=70,	blessMin=24,	blessMax=36,	random=96,},
-- 	[6] = {FPT =40,		maxValue = 5040	,	timesMin=70,	timesMax=105,	blessMin=32,	blessMax=48,	random=48,},
-- 	[7] = {FPT =50,		maxValue = 6120	,	timesMin=66,	timesMax=102,	blessMin=40,	blessMax=60,	random=60,},
-- 	[8] = {FPT =60,		maxValue = 9000	,	timesMin=85,	timesMax=125,	blessMin=48,	blessMax=72,	random=40,},
-- 	[9] = {FPT =80,		maxValue = 15072,	timesMin=105,	timesMax=157,	blessMin=64,	blessMax=96,	random=32,},
-- 	[10] = {FPT =100,	maxValue = 21120,	timesMin=116,	timesMax=176,	blessMin=80,	blessMax=120,	random=23,},
-- 	[11] = {FPT =120,	maxValue = 23040,	timesMin=106,	timesMax=160,	blessMin=96,	blessMax=144,	random=21,},
-- 	[12] = {FPT =150,	maxValue = 28990,	timesMin=107,	timesMax=161,	blessMin=120,	blessMax=180,	random=21,},
-- 	[13] = {FPT =0,	maxValue = 0,	timesMin=0,	timesMax=0,	blessMin=0,	blessMax=0,	random=0,},
-- }

local wingData = {
	[0]  = { name =0,	res=7000000},	
	[1]  = { name =1,	res=7000100},	
	[2]  = { name =2,	res=70000},	
	[3]  = { name =3,	res=70000},	
	[4]  = { name =4,	res=70000},	
	[5]  = { name =5,	res=70000},	
	[6]  = { name =6,	res=70000},	
	[7]  = { name =7,	res=70000},	
	[8]  = { name =8,	res=70000},	
	[9]  = { name =9,	res=70000},	
	[10] = { name =10,	res=70000},	
	[11] = { name =11,  res=70000},	
	[12] = { name =12,  res=70000},	

}


function onPanelData(player, commond)
	local data = util.decode(commond);
	if data.actionid == "panel" then
		onUpdatePanel(player)
	elseif  data.actionid == "upgrade" then
		onUpgrade(player,data.param)
	elseif  data.actionid == "upgradeato" then
		onUpgrade(player,data.param)
	elseif data.actionid == "pay" then
		player:push_lua_table("open",util.encode({name = "panel_chongzhi", from = "menu_wing",}))
	else

	end
end
ckpanel.add_listener("gui.PanelWing.onPanelData",gui.PanelWing.onPanelData)


function onUpdatePanel(player,flag)

	local job = player:get_job_name();
	local wingLv = util.ppn(player,const.PP_WING_LEVEL)
	local nextLv= wingLv + 1;
	local result = {}
	result.cmd = "update"
	result.wingAttr = {}
	result.nextAttr = {}
	for i=1,5 do
		table.insert(result.wingAttr,"0-0")
		table.insert(result.nextAttr,"")
	end
	result.wingLv = wingLv
	result.nostuff = flag
	result.isaction=util.ppn(player, const.WING_ACT_FLAG)
	result.levelName = wingData[wingLv].name

	if tab.Key_Wing[tostring(nextLv+1)] then 
		result.maxValue = tonumber(tab.Key_Wing[tostring(nextLv+1)].maxbless);----最大祝福值
		result.need_num_of_lingyu =tonumber(tab.Key_Wing[tostring(nextLv+1)].num)-----需要羽毛
	else
		result.maxValue=0
		result.need_num_of_lingyu=0
	end 
	result.blessvalue = util.ppn(player,const.PP_WING_BLESS_VALUE);---当前祝福值
	result.num_of_lingyu= player:num_item(24030001);
	
	--print(tab.Key_Wing[tostring(nextLv+1)].num)
	if wingLv>=0 and wingLv<=12 then
		
		for k,v in pairs(tab.Key_Wing) do
			if tonumber(v.lv)==wingLv then
				if job == "warrior" then
					result.wingAttr[1] = v.mindc.."-"..v.maxdc;
				elseif job == "wizard" then
					result.wingAttr[1] = v.minmc.."-"..v.maxmc;
				elseif job == "taoist" then
					result.wingAttr[1] = v.minsc.."-"..v.maxsc;
				end
				result.wingAttr[4] = v.minac.."-"..v.maxac;
				result.wingAttr[5] = v.minmac.."-"..v.maxmac;
			end 	
		end
	
	end
	if nextLv and nextLv <= 12 then
		for k,v in pairs(tab.Key_Wing) do
			if tonumber(v.lv)==nextLv then
				if job == "warrior" then
					result.nextAttr[1] = v.mindc.."-"..v.maxdc;
				elseif job == "wizard" then
					result.nextAttr[1] = v.minmc.."-"..v.maxmc;
				elseif job == "taoist" then
					result.nextAttr[1] = v.minsc.."-"..v.maxsc;
				end
				result.nextAttr[4] = v.minac.."-"..v.maxac;
				result.nextAttr[5] = v.minmac.."-"..v.maxmac;
			end 	
		end
	end
	player:push_lua_table("PanelWing",util.encode(result));
end

function onUpgrade(player, methods)
	player:set_param(const.WING_ACT_FLAG,0)

	local name=player:get_name()
	local nostuff=0
	local wingLv = util.ppn(player, const.PP_WING_LEVEL)+1
		if wingLv < 13 then

			local numFeather = tonumber(tab.Key_Wing[tostring(wingLv+1)].num)-----需要的羽毛

			local stuffEnough = false

			local num = player:num_item(24030001)

			if  methods then ---不足时扣元宝
				if player:num_item(24030001) < numFeather then
					if player:get_vcoin() < ( numFeather - num ) * VCOIN_PER_FEATHER then	
						showChongzhi(player)
						nostuff=1
					else
						player:remove_item(24030001, num);
						player:sub_vcoin(( numFeather - num ) * VCOIN_PER_FEATHER, "翅膀升级")
						stuffEnough = true
					end
				else
					player:remove_item(24030001, numFeather);
					stuffEnough = true
				end
			else
				if player:num_item(24030001) < numFeather then
					--showConfirm(player, COMFIRM_TAG.FEATHER_LACK,numFeather-player:num_item(24030001))
					player:alert(1,1,"羽毛数量不足");
					nostuff=1
					--posmap.fly(player, 4010003);
					--print("0000")
					--player:set_status(86,30,2,1,11)
					--npc.shengwei.mon_gen()
					--player:enter_map("v390",50,50)
					--DirectFly
					-- for k,v in pairs(tab.Key_Wing) do
					-- 	print(v.lv)

					-- end
					--player:go_home()
					--print(tab.Key_Wing[tostring(wingLv)].icoin)
					--CalculateAttribute(player)
					--map.dushe3jia.enter_map(player)
				else
					player:remove_item(24030001, numFeather);
					stuffEnough = true
				end
			end


			if stuffEnough then
				if verifyUpgradeSucceed(player,wingLv) then --升级成功
					player:set_param(const.PP_WING_BLESS_VALUE, 0);
					player:set_param(const.PP_WING_UP_TIMES, 0);--升级次数
					player:set_param(const.PP_WING_LEVEL,util.ppn(player, const.PP_WING_LEVEL) + 1 );
					gui.AwardHall_jingji.setRankWing(player)
					
					if util.ppn(player,const.PP_WING_LEVEL)>0 then
						player:set_wing_lv(70001+util.ppn(player,const.PP_WING_LEVEL));
					end
					
					player:set_param(const.PP_WING_TIYAN,1 )
					local next_wing=util.ppn(player, const.PP_WING_LEVEL)
					--player:alert(1,1,"将翅膀培养至"..next_wing.."级");
					local numChinese=util.numberToChinese(next_wing)
					------实力大增  超链接（翅膀）
					------3级公告 坐下角

					server.info(10000,0,"<font color='#01ebff'>"..name.."</font>将翅膀成功培养至<font color='#01ebff'>"..numChinese.."阶</font>实力大增！");
   					server.info(10,0,"<font color='#01ebff'>"..name.."</font>将翅膀成功培养至<font color='#01ebff'>"..numChinese.."阶</font>实力大增！<a color='#00ff00' href='event:local_OpenPanel_act35' underline='#00ff00'>我要提升</a></font>");

					--player:alert(1,1,"将翅膀培养至<font color='#01ebff'>"..numChinese.."阶</font>实力大增！");
					gui.PanelAchieve.triggerTaskEvent(player, "wing", next_wing)--翅膀成就
					-- player:set_wing_lv(70000)
					player:set_param(const.WING_ACT_FLAG, util.ppn(player, const.WING_ACT_FLAG)+1)
					if wingLv>0 then
						CalculateAttribute(player)
					end
				else
					--print(tab.Key_Wing[tostring(wingLv+1)].blessMin)
					local blessvalue=math.random(tonumber(tab.Key_Wing[tostring(wingLv+1)].blessMin),tonumber(tab.Key_Wing[tostring(wingLv+1)].blseemax)) 
					player:set_param(const.PP_WING_BLESS_VALUE, util.ppn(player, const.PP_WING_BLESS_VALUE)+blessvalue)
					player:alert(1,1,"祝福值增加"..blessvalue);
				end
			end
			onUpdatePanel(player,nostuff)
		else
			player:alert(1,1,"翅膀已满级");
			--player:go_home()
		end
end

function set_wing_tiyan(player)
	player:set_wing_lv(70001+util.ppn(player,const.PP_WING_LEVEL));
	player:set_param(const.PP_WING_TIYAN,1 )
	CalculateAttributeWing(player)
end

function CalculateAttributeWing(player)
	if util.ppn(player, const.PP_WING_TIYAN) ==1 then 
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
		
		local lv=util.ppn(player,const.PP_WING_LEVEL)

		local job = player:get_job_name();
		for k,v in pairs(tab.Key_Wing) do
			if tonumber(v.lv)==lv then
				if job == "warrior" then
					player:set_script_attr(7,v.mindc+mindc);--增加最小物理攻击
					player:set_script_attr(8,v.maxdc+maxdc);--增加最大物理攻击
				elseif job == "wizard" then
					player:set_script_attr(9,v.minmc+minmc);--增加最小魔法攻击
					player:set_script_attr(10,v.maxmc+maxmc);--增加最大魔法攻击
				elseif job == "taoist" then
					player:set_script_attr(11,v.minsc+minsc);--增加最小道术攻击
					player:set_script_attr(12,v.maxsc+maxsc);--增加最大道术攻击
				end
				player:set_script_attr(3,v.minac+minac);--增加最小物理防御
				player:set_script_attr(4,v.maxac+maxac);--增加最大物理防御
				player:set_script_attr(5,v.minmac+minmac);--增加最小魔法防御
				player:set_script_attr(6,v.maxmac+maxmac);--增加最大魔法防御
			end 	
		end
		player:recal_attr()
	end
end
login.add_login_listener(CalculateAttributeWing)-------上线重设buff

function CalculateAttribute(player)
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
	
	local lv=util.ppn(player,const.PP_WING_LEVEL)
	local job = player:get_job_name();
	for k,v in pairs(tab.Key_Wing) do
		if tonumber(v.lv)==lv then
			if job == "warrior" then
				player:set_script_attr(7,v.mindc+mindc);--增加最小物理攻击
				player:set_script_attr(8,v.maxdc+maxdc);--增加最大物理攻击
			elseif job == "wizard" then
				player:set_script_attr(9,v.minmc+minmc);--增加最小魔法攻击
				player:set_script_attr(10,v.maxmc+maxmc);--增加最大魔法攻击
			elseif job == "taoist" then
				player:set_script_attr(11,v.minsc+minsc);--增加最小道术攻击
				player:set_script_attr(12,v.maxsc+maxsc);--增加最大道术攻击
			end
			player:set_script_attr(3,v.minac+minac);--增加最小物理防御
			player:set_script_attr(4,v.maxac+maxac);--增加最大物理防御
			player:set_script_attr(5,v.minmac+minmac);--增加最小魔法防御
			player:set_script_attr(6,v.maxmac+maxmac);--增加最大魔法防御
		end
	end
	for k,v in pairs(tab.Key_Wing) do
		if tonumber(v.lv)==lv-1 and lv ~= 1 then
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
			if job == "warrior" then
				player:set_script_attr(7,mindc-v.mindc);--增加最小物理攻击
				player:set_script_attr(8,maxdc-v.maxdc);--增加最大物理攻击
			elseif job == "wizard" then
				player:set_script_attr(9,minmc-v.minmc);--增加最小魔法攻击
				player:set_script_attr(10,maxmc-v.maxmc);--增加最大魔法攻击
			elseif job == "taoist" then
				player:set_script_attr(11,minsc-v.minsc);--增加最小道术攻击
				player:set_script_attr(12,maxsc-v.maxsc);--增加最大道术攻击
			end
			player:set_script_attr(3,minac-v.minac);--增加最小物理防御
			player:set_script_attr(4,maxac-v.maxac);--增加最大物理防御
			player:set_script_attr(5,minmac-v.minmac);--增加最小魔法防御
			player:set_script_attr(6,maxmac-v.maxmac);--增加最大魔法防御
		end
	end
	player:recal_attr()
end



-- function showConfirm(player,pTag,numFeather)
-- 	local result
-- 	if pTag ==  COMFIRM_TAG.FEATHER_LACK then
-- 		result =  {
-- 			panel = "confirm",
-- 			visible = true,
-- 			lblConfirm = "灵羽数量不足，是否使用元宝补全?\n花费元宝"..numFeather*VCOIN_PER_FEATHER.."购买"..numFeather.."灵羽",
-- 			path = "gui.PanelWing.onPanelData",
-- 			actionid = "upgrade",
-- 			strMsg="wingly",
-- 			param = {"mix"},
-- 		}
-- 	elseif pTag == COMFIRM_TAG.VCOIN_LACK then
-- 		result = {
-- 			panel = "confirm",
-- 			visible = true,
-- 			lblConfirm = "元宝数量不足，是否充值?",
-- 			path = "gui.PanelWing.onPanelData",
-- 			actionid = "pay",
-- 			strMsg="wingpay",
-- 		}
-- 	end
-- 	if result then player:push_lua_table("alert",util.encode(result)) end
-- end

-----是否满足up条件
function verifyUpgradeSucceed(player,wingLv)---

	local rand=math.random(1,10000) 
	local flag=false
	if util.ppn(player,const.PP_WING_UP_TIMES)+1< tonumber(tab.Key_Wing[tostring(wingLv+1)].min) then 
		flag= false
	else 
		if util.ppn(player,const.PP_WING_UP_TIMES)+1>=tonumber(tab.Key_Wing[tostring(wingLv+1)].max) then 
			flag= true
		else
			if  util.ppn(player, const.PP_WING_BLESS_VALUE)>=tonumber(tab.Key_Wing[tostring(wingLv+1)].maxbless) then 
				flag= true
			else
				if  rand <=tonumber(tab.Key_Wing[tostring(wingLv+1)].pro)	then
					flag= true
				else
					flag= false	
				end 		
			end 
		end
	end  
	player:set_param(const.PP_WING_UP_TIMES, util.ppn(player, const.PP_WING_UP_TIMES) + 1)
	return flag 
end
function getWingMaxLevel()
	return MAX_WING_LV
end

-- function showConfirm(player,pTag,numFeather)
-- 	local result
-- 	if pTag ==  COMFIRM_TAG.FEATHER_LACK then
-- 		result =  {
-- 			panel = "confirm",
-- 			visible = true,
-- 			lblConfirm = "灵羽数量不足，是否使用元宝补全?\n花费元宝"..numFeather*VCOIN_PER_FEATHER.."购买"..numFeather.."灵羽",
-- 			path = "gui.PanelWing.onPanelData",
-- 			actionid = "upgrade",
-- 			strMsg="wingly",
-- 			param = {"mix"},
-- 		}
-- 	elseif pTag == COMFIRM_TAG.VCOIN_LACK then
-- 		result = {
-- 			panel = "confirm",
-- 			visible = true,
-- 			lblConfirm = "元宝数量不足，是否充值?",
-- 			path = "gui.PanelWing.onPanelData",
-- 			actionid = "pay",
-- 			strMsg="wingpay",
-- 		}
-- 	end
-- 	if result then player:push_lua_table("alert",util.encode(result)) end
-- end





