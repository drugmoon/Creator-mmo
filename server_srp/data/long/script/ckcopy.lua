module(..., package.seeall)
local data={};
local all_data={};
function add_listener(mapid, fun)
	if not data[mapid] then
		data[mapid] = {};
	end
	local d = data[mapid];
	d[fun] = fun;
end
function add_all_listener(fun)
	all_data[fun]=fun;
end
function triger_enter_copy(player, map)
	local mapid = map:get_def_id();
	for i,v in pairs(all_data) do
		v(player, map);
	end
	-- local len = string.len(mapid);
	-- mapid = string.sub(mapid, 1, len -3);
	local d = data[mapid];
	if d then
		for i,v in pairs(d) do
			v(player, map);
		end
	end
end
