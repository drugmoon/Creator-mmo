module(..., package.seeall)
local data={};
local all_data={};
function add_listener(mon_name,fun)
	if not data[mon_name] then
		data[mon_name] = {};
	end
	local d = data[mon_name];
	d[fun] = fun;
end

function remove_listener(mon_name,fun)
	if data[mon_name] then
		if fun then
			data[mon_name][fun] = nil;
		else
			data[mon_name] = nil
		end
	end
end

function add_all_listener(fun)
	all_data[fun]=fun;
end
function remove_all_listener(fun)
	all_data[fun] = nil;
end

function triger_mon(player,mon,group)
	for _,v in pairs(all_data) do
		v(player,mon,group);
	end
	local d = data[mon:get_def_id()];
	if d then
		for _,v in pairs(d) do
			v(player,mon,group);
		end
	end
end
local cj_data={};
local cj_all_data={};
function add_caiji_listener(mon_name,fun)
	if not cj_data[mon_name] then
		cj_data[mon_name] = {};
	end
	local d = cj_data[mon_name];
	d[fun] = fun;
end
function add_caiji_all_listener(fun)
	cj_all_data[fun]=fun;
end
function triger_caiji(player,mon)
	for _,v in pairs(cj_all_data) do
		v(player,mon);
	end
	local d = cj_data[mon:get_name()];
	if d then
		for _,v in pairs(d) do
			v(player,mon);
		end
	end
end
---采集开始调用
local cjstart_data={};
local cjstart_all_data={};
function add_caijistart_listener(mon_name,fun)
	if not cjstart_data[mon_name] then
		cjstart_data[mon_name] = {};
	end
	local d = cjstart_data[mon_name];
	d[fun] = fun;
end
function add_caijistart_all_listener(fun)
	cjstart_all_data[fun]=fun;
end
function triger_caijistart(player,mon)
	for _,v in pairs(cjstart_all_data) do
		v(player,mon);
	end
	local d = cjstart_data[mon:get_name()];
	if d then
		for _,v in pairs(d) do
			v(player,mon);
		end
	end
end