module(..., package.seeall)
local data={};
local all_data={};

function add_all_listener(fun)
	all_data[fun]=fun;
end

function triger_join_group(player, map)
	for i,v in pairs(all_data) do
		v(player);
	end
end
