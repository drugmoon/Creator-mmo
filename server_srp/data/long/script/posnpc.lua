module(..., package.seeall)

local data = {}
local f
for k,v in pairs(tab.NpcGen) do
	if v and v.flyid then
		if tab.DirectFly[v.flyid] then
			f = tab.DirectFly[v.flyid]
			if f then
				data[tonumber(k)] = {name=v.name,mapname=f.desp,map=f.destid,posx=tonumber(f.destx),posy=tonumber(f.desty),fly=tonumber(v.flyid)}
				-- print(v.name,fly.desp,fly.destid,fly.destx,fly.desty,v.flyid)
			end
		end
	end
end

function pos(npc_id)
	local d = data[npc_id];
	if d then return "<a href='event:local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..d["name"].."'>"..d["mapname"].."</a>的<a href='event:local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..d["name"].."'>"..d["name"].."</a>";end
	return "";
end
function mp(npc_id)
	local d = data[npc_id];
	if d then return "<a href='event:local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..d["name"].."'>"..d["mapname"].."</a>的<a href='event:local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..d["name"].."'>"..d["name"].."</a><a href='event:fly_"..d["fly"].."'><s='r'>[传]</s></a>";end
	return  "<s='r'>"..npc_id.."</s>";
end
function fp(npc_id)
	local d = data[npc_id];
	if d then return "<a href='event:fly_"..d["fly"].."'>"..d["mapname"].."</a>的<a href='event:fly_"..d["fly"].."'>"..d["name"].."</a>";end
	return  "<s='r'>"..npc_id.."</s>";
end
function sp(npc_id)
	local d = data[npc_id];
	if d then return "local_goto_"..d["map"].."_"..d["posx"].."_"..d["posy"].."_"..d["name"] end
end
function fid(npc_id)
	local d = data[npc_id];
	if d then return "fly_"..d["fly"] end
end
function go(player,npc_id)
	local d = data[npc_id];
	--print("posnpc.go", npc_id, d);
	if d then 
		player:find_road_goto(d["map"],d["posx"],d["posy"],d["name"],2); 
	end
end
function fly(player,npc_id)
	local d = data[npc_id];
	if d then player:enter_map(d["map"],d["posx"],d["posy"],d["name"]);player:find_road_goto(d["map"],d["posx"],d["posy"],d["name"],2); end
end

function showFlyTips(player, npc_id)
	local d = data[npc_id];
	if d then 
		local param = {
			link = "fly_"..d["fly"],
			times = util.ppn(player, const.PP_FREE_FLY_TIMES)
			-- path = "gui.PanelVip.onPanelData",
			-- actionid = "mainTaskFly",
			-- flytarget = npc_id,
			-- times = (util.ppn(player, const.PP_VIP_LEVEL) == 0) and (30 - util.ppn(player, const.PP_FREE_FLY_TIMES) or nil),
		}
		-- player:push_lua_table("flyTips", util.encode(param)) 
		player:push_lua_table("open",util.encode{name = "panel_fly", extend = param})
	end
end

function getFlyId(npc_id)
	local d = data[npc_id];
	if d then return d["fly"] end
end