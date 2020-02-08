--皇宫守卫
module(..., package.seeall)

local cityInfo =	{
	strs1 ={
		"<font color=#b2a58b>活动时间:</font><font color=#30ff00>20:30-21:00</font>",
		"<font color=#b2a58b><font color=#30ff00>皇城雕像</font>被<font color=#30ff00>摧毁后</font>才能进入皇宫！</font>",
	},
	strs2 ={
		"<font color=#b2a58b>1. 进入条件：<font color=#30ff00>60级</font>，限时<font color=#30ff00>25分钟</font></font>",
		"<font color=#b2a58b>2. 拥有<font color=#30ff00>帮会</font>才可进入皇宫</font>",
	    "<font color=#b2a58b>3. <font color=#30ff00> 20:30-20:35</font>期间，皇宫无归属</font>",
	    "<font color=#b2a58b>4. <font color=#30ff00> 20:35-21:00</font>期间，皇宫中只有</font>",
		"<font color=#b2a58b><font color=#30ff00>   唯一</font>帮会的成员时，该帮获得</font>",
		"<font color=#b2a58b><font color=#30ff00>   皇宫归属权</font>，在此期间归属权</font>",
		"<font color=#b2a58b>   的变更<font color=#30ff00>无限制</font></font>",
	    "<font color=#b2a58b>5. 结束时获得<font color=#30ff00>归属权</font>的帮会胜利</font>",
	}
}

function onTalk100(npc,player)
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="huanggong",talk_str = cityInfo}));
end