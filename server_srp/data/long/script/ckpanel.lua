module(..., package.seeall)

--客户端请求服务器lua脚本方法时， 需要先把服务器脚本对应的方法在这里注册， 前后端的key要一样。
--onLuaFunction方法，在c++中接收到前端的请求信息，会执行这个方法。
----------------------------
local data={};

function add_listener(param,fun)
	if not data[param] then
		data[param] = fun;
	end
end

function onLuaFunction(player,func,tabvalue)
	if data[func] then
		data[func](player,tabvalue)
	end
end