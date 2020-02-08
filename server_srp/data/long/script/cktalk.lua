module(..., package.seeall)
local data={};
local all_data={};
function add_listener(param,fun)
	if not data[param] then
		data[param] = {};
	end
	local d = data[param];
	d[fun] = fun;
end
function add_all_listener(fun)
	all_data[fun]=fun;
end
function triger_talk(player,param)
	for i,v in pairs(all_data) do
		v(player,param);
	end
	local d = data[param];
	if d then
		for i,v in pairs(d) do
			v(player,param);
		end
	end
end
