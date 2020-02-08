module(..., package.seeall)
local data={};
function add_listener(fun)
	data[fun] = fun;
end
function rem_listener(fun)
	data[fun]=nil;
end
function triger_dead(player,killer)
	for i,v in pairs(data) do
		v(player,killer);
	end
end