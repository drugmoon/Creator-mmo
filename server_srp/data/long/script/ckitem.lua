module(..., package.seeall)
local data={};
local all_data={};
function add_listener(item_name,fun)
	if not data[item_name] then
		data[item_name] = {};
	end
	local d = data[item_name];
	d[fun] = fun;
end
function add_all_listener(fun)
	all_data[fun]=fun;
end
function triger_item(player,item_name,item_id)
	for i,v in pairs(all_data) do
		v(player,item_name,item_id);
	end
	local d = data[item_name];
	if d then
		for i,v in pairs(d) do
			v(player,item_name,item_id);
		end
	end
end
