module(..., package.seeall)

--玩家等级变化时会触发triger_level方法
--需要监听升级变化回调的可以在这个理处理。


local data={};
function add_listener(fun)
	data[fun] = fun;
end
function triger_level(player,lv)
	for i,v in pairs(data) do
		v(player,lv);
	end
	check_level(player,lv);
end
function check_level(player,lv)
end