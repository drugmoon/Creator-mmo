module(..., package.seeall)
local kuangConf = 
{
	{item_id = 24020002, item_count = 1, bind_flag = 1, weight = 15},
	{item_id = 24020001, item_count = 1, bind_flag = 1, weight = 500},
	{item_id = 24020007, item_count = 1, bind_flag = 1, weight = 1150},
	{item_id = 24020006, item_count = 1, bind_flag = 1, weight = 2300},
	{item_id = 24020005, item_count = 1, bind_flag = 1, weight = 6035},
}

local totalweght = 10000

function kuang(player,mon,group)
	if player:get_level() >= 70 then
		if player:num_bag_black() >= 1 then
			local n = math.random(1,totalweght);
			local curWeight = 0

			for i,v in ipairs(kuangConf) do
				curWeight = curWeight + v.weight
				if n <= curWeight then
					player:add_item(v.item_id,v.item_count,v.bind_flag);
					break
				end
			end
			player:set_param(const.PP_WAKUANG_ACTION_COUNT, util.ppn(player,const.PP_WAKUANG_ACTION_COUNT) + 1)
			player:alert(1,1,"采矿成功");
			task.simulateTask.showTaskDesp(player, 4000, nil, true);
		else
			player:alert(1,1,"背包空格不足1格,未获得矿石!");
		end
	else
		player:alert(1,1,"以你现在的实力,还不能采集矿石!");
	end
end
ckmon.add_caiji_listener("矿石",kuang);

function startkuang(player,mon)
	--print("startkuang",mon:get_caiji_duration())
	player:start_progress_bar(mon:get_caiji_duration(),"正在采矿中...");
end
ckmon.add_caijistart_listener("矿石",startkuang);



-- function kuang2(player,mon,group)
-- 	if player:get_level() >= 40 then
-- 		player:alert(11,1,"<font color='#ff0000'>此为任务矿,采矿请到[采矿区域]</font>");
-- 	end
-- end
-- ckmon.add_caiji_listener("铁矿",kuang2);

-- function startkuang2(player,mon)
-- 	player:start_progress_bar(mon:get_caiji_duration(),"正在采矿中...");
-- end
-- ckmon.add_caijistart_listener("铁矿",startkuang2);

function breakStatue(player,mon)
	if util.svn("huanggongkill")<1 then
		server.set_var("huanggongkill",1);
		server.info(10000,0,"沙巴克雕像已被摧毁，将于20:30开启皇宫争夺战！")
	end
end
ckmon.add_listener(11006001,breakStatue);