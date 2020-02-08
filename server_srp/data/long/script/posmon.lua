module(..., package.seeall)
local data={
	-- ["稻草人"]=   {
	-- 	name="稻草人",
	-- 	mapname="银杏山谷",
	-- 	map="v001",
	-- 	pos={
	-- 		{posx=88,    posy=182,    fly=3000},
	-- 	},
	-- },
	-- ["鹿"]=   {
	-- 	name="鹿",
	-- 	mapname="银杏山谷",
	-- 	map="v001",
	-- 	pos={
	-- 		{posx=124,    posy=197,    fly=3001},
	-- 	},
	-- },
};
-- for i,v in pairs(data) do
-- 	for j,w in pairs(v.pos) do
-- 		if w.fly >=3000 and w.fly<4000 then
-- 			server.set_fly_node(w.fly,v.map,w.posx,w.posy,v.name);
-- 		end
-- 	end
-- end
local data = {}

for k,v in pairs(tab.DirectFly) do
	data[tonumber(k)] = {name=v.name,mapname=v.desp,map=v.destid,posx=tonumber(v.destx),posy=tonumber(v.desty),fly=tonumber(k)}
end

function getDirectFlyInfo(player, flyId)
	-- print("getDirectFlyInfo",  flyId)
	if flyId and flyId > 0 then
		local map, x, y, name = player:get_directfly_info(flyId);
		if map then
			return {
				map = map,
				name = name,
				pos={
					{posx=x,    posy=y,    fly=flyId},
				}
			};
		end
	end
end


function pos(player,mn)
	local d = data[mn];
	if d then
		local k = math.random(1,#d.pos);
		return "<a href='event:local_goto_"..d["map"].."_"..d.pos[k]["posx"].."_"..d.pos[k]["posy"].."_"..d["name"].."'>"..d["name"].."</a>";--<a href='event:fly_"..d.pos[k]["fly"].."'>[传]</a>
	end
	return "";
end
function fpos(player,mn)
	local d = data[mn];
	if d then
		local k = math.random(1,#d.pos);
		return "<a href='event:local_goto_"..d["map"].."_"..d.pos[k]["posx"].."_"..d.pos[k]["posy"].."_"..d["name"].."'>"..d["mapname"].."</a>的<a href='event:local_goto_"..d["map"].."_"..d.pos[k]["posx"].."_"..d.pos[k]["posy"].."_"..d["name"].."'>"..d["name"].."</a>";--<a href='event:fly_"..d.pos[k]["fly"].."'>[传]</a>
	end
	return "";
end
function sp(player,monId)
	local telePortID = tab.getMonTelePortID(monId);
	local d = data[telePortID];
	if d then
		local monName = tab.getMonName(monId);
		if d then return "local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..monName end
	end
end
function fid(player, monId)
	local telePortID = tab.getMonTelePortID(monId)
	local d = data[telePortID]
	if d then return "fly_"..d["fly"] end
end
function mp(player,mn)
	local d = data[mn];
	if d then
		local k = math.floor(player:get_create_time()%#d.pos)+1;--math.random(1,#d.pos);
		return "<a href='event:local_goto_"..d["map"].."_"..d.pos[k]["posx"].."_"..d.pos[k]["posy"].."_"..d["name"].."'>"..d["mapname"].."</a>的<a href='event:local_goto_"..d["map"].."_"..d.pos[k]["posx"].."_"..d.pos[k]["posy"].."_"..d["name"].."'>"..string.gsub(d["name"],"%d","").."</a><a href='event:fly_"..d.pos[k]["fly"].."'>[传]</a>";
	end
	return ""..string.gsub(mn,"%d","").."";
end
function mxx(player,mn)
	local d = data[mn];
	if d then
		return "<font color='#00ff00'>"..d["mapname"].."</font>的<font color='#00ff00'>"..string.gsub(d["name"],"%d","").."</font>";
	end
	return string.gsub(mn,"%d","");
end
function nxx(player,mn)
	local d = data[mn];
	if d then
		return d["mapname"].."|"..string.gsub(d["name"],"%d","");
	end
	return string.gsub(mn,"%d","");
end
function p(player,mn)
	local d = data[mn];
	if d then
		local k = math.floor(player:get_create_time()%#d.pos)+1;--math.random(1,#d.pos);
		return "<a href='event:local_goto_"..d["map"].."_"..d.pos[k]["posx"].."_"..d.pos[k]["posy"].."_"..d["name"].."'>"..string.gsub(d["name"],"%d","").."</a><a href='event:fly_"..d.pos[k]["fly"].."'>[传]</a>";--
	end
	return ""..string.gsub(mn,"%d","").."";
end
function go(player,monId)
	local telePortID = tab.getMonTelePortID(monId)
	local d = data[telePortID]
	if d then 
		local monName = tab.getMonName(monId);
		player:find_road_goto(d["map"],d["posx"],d["posy"],monName);
	end
end

function fly(player, monId)
	local telePortID = tab.getMonTelePortID(monId)
	local d = data[telePortID]
	if d then 
		local monName = tab.getMonName(monId);
		player:enter_map(d["map"],d["posx"],d["posy"]+1,monName);
		player:find_road_goto(d["map"],d["posx"],d["posy"],monName);
	end
	return "";
end

function n(mn)
	local d = data[mn];
	if d then
		return d.name;
	end
	return mn;
end

function getMapId(monId)
	local telePortID = tab.getMonTelePortID(monId);
	local d = data[telePortID];
	if d then
		return d["map"]
	end
end