module(..., package.seeall)
local data={};
local all_data={};
-- 此处taskid是任务类型（比如主线1000）
function add_listener(taskid, fun)
	if not data[taskid] then
		data[taskid] = {};
	end
	local d = data[taskid];
	d[fun] = fun;
end
function add_all_listener(fun)
	all_data[fun]=fun;
end
function triger_task(player, taskid, tid)
	for i,v in pairs(all_data) do
		v(player);
	end
	local d = data[taskid];
	if d then
		for i,v in pairs(d) do
			v(player, tid);
		end
	end
end
