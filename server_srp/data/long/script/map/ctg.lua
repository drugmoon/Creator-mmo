module(..., package.seeall)

function onPlayerEnter(ctgmap,player)

	ctgmap:set_var("playerName",player:get_name())
end

function onPlayerLeave(ctgmap,player)
	player:push_lua_table("custompass",util.encode({hide = true }));

	ctgmap:set_var("bossName","")
	ctgmap:set_var("bossNum","")
	ctgmap:set_var("monName","")
	ctgmap:set_var("monNum","")
	ctgmap:set_var("time","")
	ctgmap:set_var("chapter","")
	ctgmap:set_var("section","")
	ctgmap:set_var("useTime","")
	ctgmap:set_var("playerName","")
	ctgmap:set_var("total","")
	ctgmap:set_timer(1,0)
	ctgmap:clear_mon()
	ctgmap:clear_item()
end

function onTimer1(ctgmap)
	local player = ctgmap:get_rand_player(1)
	if player then
		gui.PanelBreakUp.exitMapSure(player,"email")
	else
		ctgmap:map_exe("go.home");
	end
end

-- function updateLeftTips(player)

-- 	local result = {}
-- 	local monTotal = map:num_monster();
-- 	local result = {}

-- 	local time = map:get_var("time")
-- 	local chapter = map:get_var("chapter")
-- 	local section = map:get_var("section")
-- 	local monConf = gui.PanelBreakUp.getMon(chapter,section)
-- 	if not monConf then 
-- 		return player:go_home()
-- 	end
-- 	local award = tab.Breakup.getSectionAward(chapter, section)
-- 	local result = {
-- 		monTotal = monTotal,
-- 		icon1 = award[1].id,
-- 		icon2 = award[2].id,
-- 		second = awardStar[config.name].stars[1]*60,
-- 		countDown = awardStar[config.name].countDown,
-- 		stars = awardStar[config.name].stars,
-- 	}
-- 	local bossName = map:get_var("bossName")
-- 	if #bossName>0 then
-- 		result.bossName = string.gsub(monConf.bossname,"%d","")
-- 		result.boss = (tonumber(map:get_var("bossNum")) - map:num_monster(map:get_var("bossName"))).."/"..map:get_var("bossNum")
-- 	end
-- 	if map:get_var("monNum") and map:get_var("monNum")~="" then
-- 		result.mon =  (tonumber(map:get_var("monNum")) - map:num_monster(map:get_var("monName"))).."/"..map:get_var("monNum")
-- 	end

-- 	player:push_lua_table("custompass",util.encode(result));
-- end