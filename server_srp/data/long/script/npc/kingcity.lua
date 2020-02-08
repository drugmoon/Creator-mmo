--皇宫守卫
module(..., package.seeall)

local cityInfo = {
	strs1 ={
		"<font color=#b2a58b>    群雄赴高地，热血战皇城！决胜皇城，让我们战个痛快！</font>",
	},
	strs2 ={
		"<font color=#b2a58b>1. 进入条件：<font color=#30ff00>60级</font>，限时<font color=#30ff00>1</font>小时</font>",
		"<font color=#b2a58b>2. 开启时间：开服<font color=#30ff00>第4天</font></font>",
		"<font color=#b2a58b>             合服<font color=#30ff00>第3、5天</font></font>",
		"<font color=#b2a58b>3. 攻城时间:</font><font color=#30ff00>20:00-21:00</font>",
	    "<font color=#b2a58b>4.<font color=#30ff00> 任意</font>帮会申请，<font color=#30ff00>所有</font>帮会可参战</font>",
	    "<font color=#b2a58b>5. 申请需<font color=#30ff00>1个攻城号角</font>，次日开战</font>",
	    "<font color=#b2a58b>6.<font color=#30ff00> 摧毁雕像</font>才能进入皇宫，最终</font>",
		"<font color=#b2a58b>   占领皇宫的帮会获胜！</font>",
	}
}

function onTalk100(npc,player)
	player:push_lua_table("npc_echo",util.encode({id=npc:get_id(),panelType="city",talk_str = cityInfo}));
end