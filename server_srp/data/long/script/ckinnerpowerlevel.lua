module(..., package.seeall)
local data={};
function add_listener(fun)
	data[fun] = fun;
end
function triger_inner_power_level(player,lv)
	for i,v in pairs(data) do
		v(player,lv);
	end
	check_level(player,lv);
end
function check_level(player,lv)
end