module(..., package.seeall)
local data={
	-- ["v201"]=	{ 
	-- 	mapname="矿洞二层",	
	-- 	map="v201",
	-- 	pos={
	-- 		{posx=35,	posy=39, offSet = 2, level = 1, fly=40304}, 
	-- 		{posx=27,	posy=60, offSet = 2, level = 1, fly=40305},
	-- 		{posx=49,	posy=66, offSet = 2, level = 1, fly=40306},
	-- 		{posx=43,	posy=34, offSet = 2, level = 1, fly=40307}, 
	-- 	},
	-- },
	-- ["v251"]=	{ 
	-- 	mapname="沃玛寺庙二层",	
	-- 	map="v251",
	-- 	pos={
	-- 		{posx=21,	posy=21, offSet = 3, level = 1, fly=200301}, 
	-- 		{posx=42,	posy=51, offSet = 3, level = 1, fly=200302},
	-- 		{posx=59,	posy=41, offSet = 3, level = 1, fly=200303},
	-- 		{posx=84,	posy=55, offSet = 3, level = 1, fly=200304},
	-- 		{posx=58,	posy=83, offSet = 3, level = 1, fly=200305}, 
	-- 	},
	-- },
};
-- for i,v in pairs(data) do
-- 	for j,w in pairs(v.pos) do
-- 		if w.fly >=4000 and w.fly<5000 then
-- 			server.set_fly_node(w.fly,v.map,w.posx,w.posy);
-- 		end
-- 	end
-- end

local data = {}

for k,v in pairs(tab.DirectFly) do
	data[tonumber(k)] = {name=v.name,mapname=v.desp,map=v.destid,posx=tonumber(v.destx),posy=tonumber(v.desty),fly=tonumber(k)}
end

-- print("util.encode", util.encode(data))

function mp(telePortID)
	local d = data[telePortID];
	if d then 
		-- local k = math.random(1,#d.pos);
		return "<a href='event:local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."'>"..d["mapname"].."</a><a href='event:fly_"..d["fly"].."'>[传]</a>";end--
		return  ""..telePortID.."";
end
function p(telePortID)
	local d = data[telePortID];
	if d then 
		-- local k = math.random(1,#d.pos);
		return "<a href='event:local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."'>"..d["mapname"].."</a><a href='event:fly_"..d["fly"].."'>[传]</a>";end--
		return  ""..telePortID.."";
end

function fp(telePortID)
	local d = data[telePortID];
	if d then return "<a href='event:fly_"..d["fly"].."'>"..d["mapname"].."</a>的<a href='event:fly_"..d["fly"].."'>"..d["name"].."</a>";end
	return  "<s='r'>"..telePortID.."</s>";
end

function sp(telePortID, goto)
	if util.isTable(telePortID) then
		telePortID = telePortID[math.random(1,#telePortID)]
	end
	local d = data[telePortID];
	-- print("posmap sp", telePortID)
	if d then 
		if goto then
			return "local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..goto;
		elseif d["name"] and d["name"]~="" then
			return "local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..d["name"];
		end
	end
end

function fid(telePortID)
	if util.isTable(telePortID) then
		telePortID = telePortID[math.random(1,#telePortID)]
	end
	local d = data[telePortID];
	if d then return "fly_"..d["fly"] end
end

function go(player,telePortID)
	local d = data[telePortID];
	if d then
		player:find_road_goto(d["map"],d["posx"],d["posy"],d["name"]); 
	end
end

function fly(player,telePortID,goto)
	local d = data[tonumber(telePortID)];
	if d then
		local offset = tonumber(d["destsize"]) and tonumber(d["destsize"]) or 1
		player:enter_map(d["map"], d["posx"], d["posy"],offset)
		if goto then
			player:find_road_goto(d["map"], d["posx"], d["posy"], goto)
		elseif d["name"] and d["name"]~="" then
			player:find_road_goto(d["map"], d["posx"], d["posy"], d["name"])
		end
		return true
	end
end

function directFly(player,telePortID)
	local d = data[telePortID];
	if d then
		-- local k = math.random(1,#d.pos);
		player:enter_map(d["map"],d["posx"],d["posy"]); 
	end
end