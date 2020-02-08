module(..., package.seeall)
local data={};
function add_listener(fun)
	data[fun] = fun;
end
function triger_zslevel(player,zslv)
	for i,v in pairs(data) do
		v(player,zslv);
	end
	check_zslevel(player,zslv);
end
function check_zslevel(player,zslv)
end