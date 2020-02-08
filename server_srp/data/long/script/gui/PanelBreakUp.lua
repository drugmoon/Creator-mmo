module(..., package.seeall)

local ticket = {id = 32000001,name = "副本卷轴"}

local CUSTOMSPASS = tab.Breakup.CUSTOMSPASS
local awardStar = tab.Breakup.awardStar
local genPos = tab.Breakup.genPos
local Config = tab.Breakup.Config

local CUR_PASS_CHAPTER = const.PP_CUR_PASS_CHAPTER
local CUR_PASS_SECTION = const.PP_CUR_PASS_SECTION
local getMonster = tab.Breakup.getMonster

function getMap( chapter,section )
	local index = chapter*10 + 6
	local map = server.find_map(string.format("ctg%03d", index))
	if map and map:num_player() < 1 then
		return map
	end
end
function getSectionData(player,chapter,section)
	if CUSTOMSPASS[chapter] and CUSTOMSPASS[chapter].conf[section] then
		return CUSTOMSPASS[chapter].conf[section];
	end
end
function getSectionCon(player,chapter,section)
	local sectionData = getSectionData(player,chapter,section)
	local t = player:get_param(sectionData.con)
	t = util.decode(t)
	if not t or type(t)~="table" then
		t = {
			stars=0, 
			isfirst=true,
			passtimes=0,
		}
		player:set_param(sectionData.con,util.encode(t))
	end
	return t
end

function getChapterStars(player,chapter)
	local stars = 0
	if CUSTOMSPASS[chapter] then
		for k,v in pairs(CUSTOMSPASS[chapter].conf) do
			local con = getSectionCon(player,chapter,k)
			if con then
				stars = stars + con.stars
			end
		end
	end
	return stars
end

function getSectionStars(player,chapter,section)
	local sectionData = getSectionData(player,chapter,section)
	local t = player:get_param(sectionData.con)
	t = util.decode(t)
	if type(t) =="table" then
		return t.stars or 0
	end
	return 0
end

function setSectionStar( player,chapter,section,star )
	local sectionData = getSectionData(player,chapter,section)
	local t = player:get_param(sectionData.con)
	t = util.decode(t)
	if type(t) =="table" then
		t.stars = t.stars and math.max(t.stars,star) or star
		t.isfirst = false
		player:set_param(sectionData.con,util.encode(t))
	end
end

function isFirstPass(player,chapter,section)
	local sectionData = getSectionData(player,chapter,section)
	local t = player:get_param(sectionData.con)
	t = util.decode(t)
	if type(t) =="table" then
		return t.isfirst
	end
end

function addPassTimes(player,chapter,section,times)
	local sectionData = getSectionData(player,chapter,section)
	if sectionData then
		local contable = getSectionCon(player,chapter,section)
		if times then
			contable.passtimes = times
		else
			contable.passtimes = contable.passtimes+1
		end
		player:set_param(sectionData.con,util.encode(contable))
	end
end

function onPanelData(player,commond)
	-- for i=1,16 do
	-- 	for j=1,6 do
	-- 		player:set_param(const["CUSTOM_PASS_"..i.."_"..j],"")
	-- 	end
	-- 	player:set_param(const["AWARD_BOX_CON"..i],"")
	-- 	player:set_param(const["PP_BREAKUP_FIRST"..i],"")
	-- end
		-- player:set_param(CUR_PASS_CHAPTER,5)
		-- player:set_param(CUR_PASS_SECTION,6)

	local data = util.decode(commond);
	if not data  then return end
	if data.actionid == "fresh" then
		freshPanel(player);
		local firstclick = util.ppn(player, const.PP_BREAKUP_FIRST_CLICK)
		if firstclick ==0 then
			player:set_param(const.PP_BREAKUP_FIRST_CLICK,1)
		end
		gui.moduleRedPoint.checkBreakUpUsable(player)
	elseif data.actionid == "getTaskInfo" then
		getTaskInfo(player,data.params)
	elseif data.actionid == "box" then
		freshBox(player,data.params[1],data.params[2],data.first)
	elseif data.actionid == "getbox" then
		getBoxAward(player,data.params[1],data.params[2])
	elseif data.actionid == "getfirstbox" then
		getBoxAward(player,data.params[1],data.params[2],true)-- 路线上宝箱
	elseif data.actionid == "enterMap" then
		enterMap(player,data.params[1],data.params[2])
	elseif data.actionid == "exitMap" then
		exitMap(player)
	elseif data.actionid == "exitMapSure" then
		exitMapSure(player)
	end
end

ckpanel.add_listener("gui.PanelBreakUp.onPanelData",gui.PanelBreakUp.onPanelData)

function freshPanel(player)
	local result = {}
	result.cmd = "fresh"
	result.data= util.clone(Config)

	for chapter,v in ipairs(result.data) do
		for section,m in ipairs(v.conf) do
			m.con = getSectionCon(player, chapter, section)
			local monConf = getMonster(chapter,section)
			if monConf then
				m.bossname = monConf.bossname or ""
				m.bosslv = monConf.bosslv
			else
				m.bossname = ""
			end
		end
		local boxCon = player:get_param(v.boxCon)
		if not boxCon or boxCon == "" then boxCon = "000";	player:set_param(v.boxCon,boxCon) end
		v.boxCon = boxCon
		local firstCon = player:get_param(v.firstCon)
		if not firstCon or firstCon == "" then firstCon = "000000";player:set_param(v.firstCon,firstCon) end
		v.firstCon = firstCon
		v.stars = getChapterStars(player,chapter)
	end
	result.curChapter = util.ppn(player, CUR_PASS_CHAPTER);
	result.curSection = util.ppn(player, CUR_PASS_SECTION);

	if result.curChapter + result.curSection ==0 then
		result.curChapter = 1
		result.curSection = 0
		player:get_param(CUR_PASS_CHAPTER,1)
		player:get_param(CUR_PASS_SECTION,0)
	end
	result.ticket = player:num_item(ticket.id)

	player:push_lua_table("PanelBreakUp",util.encode(result));
end

function getTaskInfo(player,params)
	local chapter,section = tonumber(params[1]),tonumber(params[2])
	chapter = util.bound(1,chapter,chapter)
	section = util.bound(1,section,6)
	if chapter and section then
		local result = {}
		result.cmd = "getTaskInfo"
		result.config= util.clone(Config[chapter].conf[section])
		result.config.con = player:get_param(result.config.con) --util.ppn(player, result.config.con)
		result.config.dropout = tab.Breakup.getSectionDropout(chapter,section)
		result.config.award = tab.Breakup.getSectionAward(chapter,section)--awardStar[result.config.name].award;
		result.config.stars = awardStar[result.config.name].stars;

		player:push_lua_table("PanelBreakUp",util.encode(result));
	end
end
--first==true 首次通关宝箱，其余是星数宝箱
function freshBox(player,index,chapter,first)
	if CUSTOMSPASS[chapter] then
		local result = {}
		result.cmd = "boxData"
		result.first= first
		local con,firstCon,boxCon
		if first then
			if isSectionPass(player,chapter,index) then
				firstCon = player:get_param(CUSTOMSPASS[chapter].firstCon)
				if #firstCon<6 then firstCon = "000000";player:set_param(CUSTOMSPASS[chapter].firstCon,firstCon) end
				con = string.sub(firstCon,index,index)
			end
			if checknumber(con) > 0 then
				result.taskName = "您已领取过该奖励！"
			else
				local monConf = getMonster(chapter,index)
				if monConf and monConf.bossname then
					result.taskName = "完成 "..monConf.bossname.." 后可领取"
				end
			end
			result.award = tab.Breakup.getSectionFirstAward(chapter,index)
		else
			boxCon = player:get_param(CUSTOMSPASS[chapter].boxCon)
			if #boxCon<3 then
				boxCon = "000"
				player:set_param(CUSTOMSPASS[chapter].boxCon,boxCon)
			end
			con = string.sub(boxCon,index,index)
			result.award = tab.Breakup.getChapterBoxAward(index,chapter)
			result.awardBox = tab.Breakup.getChapterBox(chapter)--util.clone(awardBox[index])
			if checknumber(con) > 0 then
				result.taskName = "您已领取过该奖励！"
			else
				local stars = getChapterStars(player,chapter)
				local needstars = tab.Breakup.getNeedStar(index,chapter)
				if not needstars or stars<needstars then
					con="1"
				end
				result.taskName = "累计星数达到 "..result.awardBox[index].star.." 后领取"
			end
		end
		result.star = getChapterStars(player,chapter)
		result.btnState = con=="0"
		result.index = index
		result.chapter = chapter
		player:push_lua_table("PanelBreakUp",util.encode(result));
	end
end

function stringSubByPos(str,spos,epos,reps)
	local s = reps or ""
	if epos<spos then spos,epos = epos,spos end
	local sstr = spos>1 and string.sub(str,1,spos-1) or ""
	local estr = string.sub(str,epos+1)
	return sstr..s..estr
end

--领取每章节宝箱奖励
function getBoxAward(player,index,chapter,first)
	if CUSTOMSPASS[chapter] then
		local awardInfo,con,needstars = {},"",0
		local firstCon = player:get_param(CUSTOMSPASS[chapter].firstCon)
		if #firstCon <6 then 
			firstCon = "000000";
			player:set_param(CUSTOMSPASS[chapter].firstCon,firstCon) 
		end
		local stars = getChapterStars(player,chapter)
		local boxCon = player:get_param(CUSTOMSPASS[chapter].boxCon)
		if #boxCon<3 then 
			boxCon = "000";	
			player:set_param(CUSTOMSPASS[chapter].boxCon,boxCon) 
		end
		if first then
			con = checknumber(string.sub(firstCon,index,index))
			if con >0 then
				return util.alert_msg(4, "首次通关奖励已领取！", player)
			end
			if not isSectionPass(player,chapter,index) then
				return util.alert_msg(4, "通关之前所有关卡后领取！", player);
			end
	
			awardInfo = tab.Breakup.getSectionFirstAward(chapter,index)
		else
			con = tonumber(string.sub(boxCon,index,index)) or 1
			if con>0 then
				return util.alert_msg(4, "宝箱奖励已领取！", player)
			end
			needstars = tab.Breakup.getNeedStar(index,chapter)
			if not needstars or stars<needstars then
				return util.alert_msg(4, "星数不足，无法领取！", player)
			end
			awardInfo = tab.Breakup.getChapterBoxAward(index,chapter)--awardBox[index].award
		end

		if util.getAwardNeedBags(player,awardInfo) > player:num_bag_black() then
			return util.alert_msg(4, "背包格子不足", player)
		end	
		for k,v in pairs(awardInfo) do
			player:add_item(v.id,v.num ,v.bind or 1);
		end
		if first then
			if string.sub(firstCon,index,index) == "0" then
				firstCon = stringSubByPos(firstCon,index,index,"1")
				player:set_param(CUSTOMSPASS[chapter].firstCon,firstCon)
			end
		else
			if string.sub(boxCon,index,index) == "0" then
				boxCon = stringSubByPos(boxCon,index,index,"1")
				player:set_param(CUSTOMSPASS[chapter].boxCon,boxCon)
			end
		end
		if first then
			util.alert_msg(4, "成功领取首次通关宝箱奖励", player)
		else
			util.alert_msg(4, "成功领取"..needstars.."星宝箱奖励", player)
		end
		player:push_lua_table("PanelBreakUp",util.encode({ 
			cmd = "freshBoxData",
			chapter = chapter,
			boxCon = boxCon,
			index = index,
			chapter = chapter,
			first = first,
			stars = stars,
			firstCon = firstCon,
		}))
		
		gui.moduleRedPoint.checkBreakUpUsable(player)
	end
end

--领取每关卡奖励
function getsectionAward( player,chapter ,section,useTime,email)
	if CUSTOMSPASS[chapter] then
		local sectionData = CUSTOMSPASS[chapter].conf[section];
		local name = sectionData.name
		local awardInfo = tab.Breakup.getSectionAward(chapter,section)
		if util.getAwardNeedBags(player,awardInfo) > player:num_bag_black() then
			if email then
				util.sendMail(player, "闯天关奖励", "您通过了闯天关第"..chapter.."章第"..section.."关，获得如下奖励：", awardInfo)
			else
				util.alert_msg(4, "背包格子不足", player)
				return false
			end
		end
		for k,v in pairs(awardInfo) do
			player:add_item(v.id,v.num,v.bind);
		end
		local stars = awardStar[name].stars
		for i=#stars,1,-1 do
			if useTime <= stars[i]*60 then
				setSectionStar( player,chapter,section,i)
				break
			end 
		end
		util.alert_msg(4, string.format("成功领取第%d章节第%d关奖励",chapter,section), player)
		setChartStar(player)
		gui.moduleRedPoint.checkBreakUpUsable(player)
		return true
	end
end

function setChartStar(player)
	local allstars = 0
	local curChapter = util.ppn(player, CUR_PASS_CHAPTER)
	for i=1,curChapter do
		for j=1,6 do
			if isSectionPass(player, i, j) then
				allstars = allstars + getSectionStars(player,i,j)
			end
		end
	end
	player:set_param(const.PP_TIANGUAN_ALLSTARS,allstars) 
	player:set_tianguan_lv(allstars);	-----天关排行榜
end

function getMaxSection(player)
	local curChapter = util.ppn(player, CUR_PASS_CHAPTER)
	curChapter = math.max(1,curChapter)
	return curChapter*6 - 6 + util.ppn(player, CUR_PASS_SECTION)
end
function isSectionOpen(player,chapter,section)
	local curMax = getMaxSection(player)
	local max = chapter*6 + section-6
	if max <= curMax+1 then
		return true
	end
end
function isSectionPass(player,chapter,section)
	local curMax = getMaxSection(player)
	local max = chapter*6 + section-6
	if max <= curMax then
		return true
	end
end

function enterMap(player,chapter,section)
	local curMapId = player:get_map():get_id()
	if curMapId == "kingcity" and server.is_war()~=0 or not tab.isMapEnterLegal(curMapId) then
		return util.alert_msg(4, "您当前已在副本中，无法传送！", player);
	end
	if not isSectionOpen(player,chapter,section) then
		return util.alert_msg(4, "通关之前所有关卡后开启", player);
	end
	
	if CUSTOMSPASS[chapter] then
		local config = CUSTOMSPASS[chapter].conf[section]
		if config then
			local conTable = getSectionCon(player, chapter, section)
			local map = getMap(chapter,section)
			if map then
				if conTable.passtimes>=config.challengeTimes then
					return util.alert_msg(4, "当前关卡今日挑战次数已用完，请您明天再来！！", player)
				end
				if player:num_item(ticket.id)<config.ticket then
					return util.alert_msg(4, ticket.name.."不足！", player)
				end
				if player:get_level()<config.limitlv then
					return util.alert_msg(4, "等级不足！", player)
				end
				-- player:remove_item(ticket.id,config.ticket) --扣除副本卷轴
				-- addPassTimes(player,chapter,section)-- 挑战次数+1
				gui.PanelGongXun.checkTaskComplete(player,9)
				
				local posx,posy = player:get_pos()
				player:set_param(const.PP_LAST_MAP_CONF,curMapId.."_"..posx.."_"..posy) --记录进入地图前位置

				map:clear_mon()
				map:clear_item()
				local monConf = getMonster(chapter,section) or {}
				local award = tab.Breakup.getSectionAward(chapter, section)
				local result = {
					icon1 = award[1].id,
					icon2 = award[2].id,
					num1 = award[1].num,
					num2 = award[2].num,
					second = awardStar[config.name].stars[1]*60,
					countDown = awardStar[config.name].countDown,
					serverTime = os.time(),
					stars = awardStar[config.name].stars,
					isBoss = false,
					imgindex = table.indexof(table.keys(awardStar),config.name),
				}
				local posX,posY,total = 0,0,0
				if monConf.monid then
					local monPos = genPos[chapter].monPos
					for k,num in pairs(monConf.monnum) do
						total = total + num
						for i=1,num do
							local r = math.random(1,#monPos)
							local x,y = monPos[r].x,monPos[r].y
							posX,posY = x,y
							map:mon_gen(x,y,monConf.monid[k],1,5,1,1);--x,y,name,num,range,time,nolive,flags,tip,limitlv
						end
					end
					map:set_var("monName",table.concat(monConf.monname,";"))
					map:set_var("monNum",total)
					result.mon = "0/"..total
				end
				if monConf.bossid and monConf.bossid>0 then
					result.isBoss = true;
					local bossPos = genPos[chapter].bossPos
					posX,posY = bossPos.x,bossPos.y
					monConf.bossnum = monConf.bossnum or 1
					map:mon_gen(bossPos.x,bossPos.y,monConf.bossid,monConf.bossnum,5,1,1);
					player:notify_mini_npc()
					map:set_var("bossName",monConf.bossname)
					map:set_var("bossNum",monConf.bossnum)
					result.boss = "0/"..monConf.bossnum
					total = total + monConf.bossnum
					-- result.bossName = string.gsub(monConf.bossname,"%d","")
				end
				result.bossName = "闯天关"
				map:set_var("time",os.time())
				map:set_var("chapter",chapter)
				map:set_var("section",section)
				map:set_var("total",total)

				local playerPos = genPos[chapter].playerPos
				player:enter_map(map:get_id(),playerPos.x,playerPos.y)
				player:find_road_goto(map:get_id(),posX,posY, "autofightstart")

				map:set_timer(1,awardStar[config.name].stars[1]*60*1000)
				player:push_lua_table("custompass",util.encode(result));

				player:push_lua_table("close",util.encode({name = "extend_breakup"}))
			else
				util.alert_msg(4, "闯天关人数太多，请稍后再来", player)
			end
		end
	end
end

function exitMap(player)
	local pmap = player:get_map()
	local curMapId = pmap:get_id()
	if string.find(curMapId,"ctg") then
		local monNum = pmap:num_monster()
		if monNum>0 then
			player:push_lua_table("custompass",util.encode({showExitTips = true }));
			return
		end
		if pmap:num_item()>0 then
			player:push_lua_table("custompass",util.encode({showPickUpTips = true }));
			return
		end

		exitMapSure(player)
	else
		player:go_home()
	end
end

function exitMapSure(player,email)
	local pmap = player:get_map()
	local curMapId = pmap:get_id()
	if string.find(curMapId,"ctg") then
		local time = tonumber(pmap:get_var("useTime") or "")
		if not time then
			time = os.time() - tonumber(pmap:get_var("time"))
		end
		local chapter = checknumber(pmap:get_var("chapter"))
		local section = checknumber(pmap:get_var("section"))
		if pmap:num_monster()==0 and chapter>0 then--自动退出也给奖励
			if getsectionAward(player,chapter,section,time,email) then
				addPassTimes(player,chapter,section)-- 挑战次数+1
				gui.moduleRedPoint.checkBreakUpUsable(player)
			else
				return
			end
		end
		goToPrevious(player)
		-- player:set_timer(2,30)
	else
		player:push_lua_table("custompass",util.encode({hide = true }));
	end
end

function showPassStar(player,chapter,section,useTime)
	local name = CUSTOMSPASS[chapter].conf[section].name
	local starTime = awardStar[name].stars
	for i=#starTime,1,-1 do
		if useTime <= starTime[i]*60 then
			--显示通关星级
			if i == 3 then
				player:push_lua_table("showPassStar",util.encode({passStar = i}));
			end
			break;
		end
	end
end

function onMonKill(player, mon1)
	local map = player:get_map()
	local monTotal = map:num_monster();

	if monTotal == 0 then
		--杀死所有怪物后暂停左边tips计时，开始副本结束倒计时30秒
		local useTime = os.time() - tonumber(map:get_var("time") or 0)
		map:set_var("useTime",useTime)
		local chapter = tonumber(map:get_var("chapter"))
		local section = tonumber(map:get_var("section"))
		if chapter and section then
			local curMapIndex = getMaxSection(player)
			local nowMapIndex = chapter*6 + section - 6
			if curMapIndex < nowMapIndex then
				player:set_param(CUR_PASS_CHAPTER,chapter);
				player:set_param(CUR_PASS_SECTION,section);
				--成就触发
				gui.PanelAchieve.triggerTaskEvent(player, "Breakup", nowMapIndex)
			end
		end
		showPassStar(player,chapter,section,useTime)
		notifyPassSection(player,chapter,section)
		local config = CUSTOMSPASS[chapter].conf[section]
		if config then
			player:remove_item(ticket.id,config.ticket) --扣除副本卷轴
		end
		map:set_timer(1,30*1000)--30秒倒计时
	end
	local result = {monTotal = monTotal}
	result.bossName = "闯天关"--string.gsub(map:get_var("bossName"),"%d","")
	local bossNum = 0
	if map:get_var("bossNum") and map:get_var("bossNum")~="" then
		bossNum = map:num_monster(map:get_var("bossName"))
		result.boss = (checknumber(map:get_var("bossNum")) - bossNum).."/"..map:get_var("bossNum")
	end
	if map:get_var("monNum") and map:get_var("monNum")~="" then
		result.mon =  (checknumber(map:get_var("total")) -checknumber(map:get_var("bossNum")) -monTotal+ bossNum).."/"..map:get_var("monNum")
	end
	player:push_lua_table("custompass",util.encode(result));
end

function goToPrevious(player)
	local mapInfos = player:get_param(const.PP_LAST_MAP_CONF)
	if mapInfos and mapInfos~="" then
		mapInfos = util.splitString(mapInfos, "_")
		if #mapInfos == 3 then
			if player:is_dead() then
				player:relive(120,mapInfos[1],tonumber(mapInfos[2]),tonumber(mapInfos[3]))
				player:set_temp_param(888,"ctg")
			else
				player:enter_map(mapInfos[1],tonumber(mapInfos[2]),tonumber(mapInfos[3]))
			end
		end
		player:set_param(const.PP_LAST_MAP_CONF,"")
	else
		player:go_home()
	end
end

function checkBreakUpRedPoint(player)
	local firstclick = util.ppn(player, const.PP_BREAKUP_FIRST_CLICK)
	if firstclick ==0 then
		return true
	end
	local curChapter = util.ppn(player, CUR_PASS_CHAPTER)
	local maxSection = getMaxSection(player)
	for k,v in pairs(Config) do
		local chapter = k
		if chapter<=curChapter then
			local boxCon = player:get_param(v.boxCon)
			if #boxCon>0  then
				local stars = getChapterStars(player,chapter)
				for i=1,3 do
					local con = string.sub(boxCon,i,i)
					local needstars = tab.Breakup.getNeedStar(i,chapter)
					if con and con == "0" and needstars and stars>=needstars then
						return true
					end
				end
			end
			local firstCon = player:get_param(v.firstCon)
			if #firstCon>0 then
				for i=1,#firstCon do
					if i%2 ==0 and isSectionPass(player, chapter, i) then
						if string.sub(firstCon,i,i) =="0" then
							return true
						end
					end
				end
			end
		end
	end
	return false
end

function notifyPassSection(player,chapter,section)
	if not isFirstPass(player,chapter,section) then return end
	local str
	if chapter<=4 then
		if section==6 then
			str = "<font color='#fff843'>恭喜<font color=#01ebff>"..player:get_name().."</font>成功闯过天关第<font color=#30ff00>"..util.numberToChinese(chapter).."</font>章!</font>"
			server.info(10000,0,str);
			str = str .."<a color='#00ff00' href='event:local_OpenPanel_act41' underline='#00ff00'>我要闯天关</a>"
			server.info(10,0,str);
		end
	else
		if section%2==0 then
			local monConf = getMonster(chapter,section)
			if monConf then
				str = "<font color='#fff843'>恭喜<font color=#01ebff>"..player:get_name().."</font>成功击杀<font color=#ff0000>"..monConf.bossname.."</font>，闯过天关第<font color=#30ff00>"..util.numberToChinese(chapter).."</font>章第<font color=#30ff00>"..section.."</font>关!</font>"
				server.info(10000,0,str);
				str = str .."<a color='#00ff00' href='event:local_OpenPanel_act41' underline='#00ff00'>我要闯天关</a>"
				server.info(10,0,str);
			end
		end
	end
end