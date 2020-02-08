module(..., package.seeall)

--战士表
local awardTab = {
	[1] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_1,
		aType = 1,
		awards = {
			{typeId = 26010051,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32010002,	num = 10,bind = 1},
			{typeId = 20003003,	num = 1,bind = 1},
		},
	},
	[2] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_2,
		aType = 2,
		awards = {
			{typeId = 24030001,	num = 30,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 20002002,	num = 1,bind = 1},
		},
	},
	[3] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_3,
		aType = 3,
		awards = {
			{typeId = 26010012,	num = 1,bind = 1},
			{typeId = 32000005,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[4] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_4,
		aType = 4,
		awards = {
			{typeId = 21000005,	num = 5,bind = 1},
			{typeId = 32000003,	num = 3,bind = 1},
			{typeId = 23060001,	num = 2,bind = 1},
			{typeId = 23080001,	num = 2,bind = 1},
			{typeId = 23080003,	num = 2,bind = 1},
		},
	},
	[5] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_5,
		aType = 5,
		awards = {
			{typeId = 23060003,	num = 5,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[6] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_6,
		aType = 6,
		awards = {
			{typeId = 23150003,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
		},
	},
	[7] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_7,
		aType = 7,
		awards = {
			{typeId = 15200015,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 23060001,	num = 3,bind = 1},
		},
	},
	[8] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_8,
		aType = 8,
		awards = {
			{typeId = 23100006,	num = 1,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 23090002,	num = 1,bind = 1},
		},
	},
	[9] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_9,
		aType = 9,
		awards = {
			{typeId = 25010005,	num = 1,bind = 1},
			{typeId = 23060001,	num = 3,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[10] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_10,
		aType = 10,
		awards = {
			{typeId = 23110002,	num = 1,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[11] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_11,
		aType = 11,
		awards = {
			{typeId = 23020002,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 23080003,	num = 1,bind = 1},
		},
	},
	[12] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_12,
		aType = 12,
		awards = {
			{typeId = 23000003,	num = 50,bind = 1},
			{typeId = 32000003,	num = 1,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[13] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_13,
		aType = 13,
		awards = {
			{typeId = 23030002,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 23060001,	num = 4,bind = 1},
		},
	},
	[14] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_14,
		aType = 14,
		awards = {
			{typeId = 23010003,	num = 50,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[15] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_15,
		aType = 15,
		awards = {
			{typeId = 26010042,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 23080003,	num = 1,bind = 1},
		},
	}
}

--法师表
local awardTab2 = {
	[1] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_1,
		aType = 1,
		awards = {
			{typeId = 26020051,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32010002,	num = 10,bind = 1},
			{typeId = 20003003,	num = 1,bind = 1},
		},
	},
	[2] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_2,
		aType = 2,
		awards = {
			{typeId = 24030001,	num = 30,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 20002002,	num = 1,bind = 1},
		},
	},
	[3] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_3,
		aType = 3,
		awards = {
			{typeId = 26020012,	num = 1,bind = 1},
			{typeId = 32000005,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[4] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_4,
		aType = 4,
		awards = {
			{typeId = 21000005,	num = 5,bind = 1},
			{typeId = 32000003,	num = 3,bind = 1},
			{typeId = 23060001,	num = 2,bind = 1},
			{typeId = 23080001,	num = 2,bind = 1},
			{typeId = 23080003,	num = 2,bind = 1},
		},
	},
	[5] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_5,
		aType = 5,
		awards = {
			{typeId = 23060003,	num = 5,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[6] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_6,
		aType = 6,
		awards = {
			{typeId = 23150003,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
		},
	},
	[7] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_7,
		aType = 7,
		awards = {
			{typeId = 15200015,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 23060001,	num = 3,bind = 1},
		},
	},
	[8] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_8,
		aType = 8,
		awards = {
			{typeId = 23100006,	num = 1,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 23090002,	num = 1,bind = 1},
		},
	},
	[9] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_9,
		aType = 9,
		awards = {
			{typeId = 25010005,	num = 1,bind = 1},
			{typeId = 23060001,	num = 3,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[10] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_10,
		aType = 10,
		awards = {
			{typeId = 23110002,	num = 1,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[11] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_11,
		aType = 11,
		awards = {
			{typeId = 23020002,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 23080003,	num = 1,bind = 1},
		},
	},
	[12] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_12,
		aType = 12,
		awards = {
			{typeId = 23000003,	num = 50,bind = 1},
			{typeId = 32000003,	num = 1,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[13] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_13,
		aType = 13,
		awards = {
			{typeId = 23030002,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 23060001,	num = 4,bind = 1},
		},
	},
	[14] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_14,
		aType = 14,
		awards = {
			{typeId = 23010003,	num = 50,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[15] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_15,
		aType = 15,
		awards = {
			{typeId = 26020042,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 23080003,	num = 1,bind = 1},
		},
	}
}

--道士表
local awardTab3 = {
	[1] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_1,
		aType = 1,
		awards = {
			{typeId = 26030011,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32010002,	num = 10,bind = 1},
			{typeId = 20003003,	num = 1,bind = 1},
		},
	},
	[2] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_2,
		aType = 2,
		awards = {
			{typeId = 24030001,	num = 30,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 20002002,	num = 1,bind = 1},
		},
	},
	[3] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_3,
		aType = 3,
		awards = {
			{typeId = 26030002,	num = 1,bind = 1},
			{typeId = 32000005,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[4] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_4,
		aType = 4,
		awards = {
			{typeId = 21000005,	num = 5,bind = 1},
			{typeId = 32000003,	num = 3,bind = 1},
			{typeId = 23060001,	num = 2,bind = 1},
			{typeId = 23080001,	num = 2,bind = 1},
			{typeId = 23080003,	num = 2,bind = 1},
		},
	},
	[5] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_5,
		aType = 5,
		awards = {
			{typeId = 23060003,	num = 5,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[6] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_6,
		aType = 6,
		awards = {
			{typeId = 23150003,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
		},
	},
	[7] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_7,
		aType = 7,
		awards = {
			{typeId = 15200015,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 23060001,	num = 3,bind = 1},
		},
	},
	[8] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_8,
		aType = 8,
		awards = {
			{typeId = 23100006,	num = 1,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 23090002,	num = 1,bind = 1},
		},
	},
	[9] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_9,
		aType = 9,
		awards = {
			{typeId = 25010005,	num = 1,bind = 1},
			{typeId = 23060001,	num = 3,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[10] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_10,
		aType = 10,
		awards = {
			{typeId = 23110002,	num = 1,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[11] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_11,
		aType = 11,
		awards = {
			{typeId = 23020002,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 23080003,	num = 1,bind = 1},
		},
	},
	[12] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_12,
		aType = 12,
		awards = {
			{typeId = 23000003,	num = 50,bind = 1},
			{typeId = 32000003,	num = 1,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
		},
	},
	[13] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_13,
		aType = 13,
		awards = {
			{typeId = 23030002,	num = 1,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 23060001,	num = 4,bind = 1},
		},
	},
	[14] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_14,
		aType = 14,
		awards = {
			{typeId = 23010003,	num = 50,bind = 1},
			{typeId = 32000005,	num = 2,bind = 1},
			{typeId = 32000001,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
		},
	},
	[15] = {
		con=const.PP_FIFTEEN_SIGNIN_RECEIVE_15,
		aType = 15,
		awards = {
			{typeId = 26030032,	num = 1,bind = 1},
			{typeId = 21000004,	num = 2,bind = 1},
			{typeId = 32000002,	num = 2,bind = 1},
			{typeId = 32000003,	num = 2,bind = 1},
			{typeId = 23080003,	num = 1,bind = 1},
		},
	}
}

function onPanelData(player, commond)
	local data =util.decode(commond);
	if data.actionid == "init" then
		updateList(player);
	elseif data.actionid == "receive" then
		receiveAward(player,data.tag)
	end
end

ckpanel.add_listener("gui.PanelFifteenSignIn.onPanelData",gui.AwardHall_fifteen.onPanelData)

--根据职业取表
function getNeedTable(player)
	local job = player:get_job_name()
	if job=="warrior" then
		return util.clone(awardTab)
	elseif job=="wizard" then
		return util.clone(awardTab2)
	elseif job=="taoist" then
		return util.clone(awardTab3)
	end
end

function updateList(player,noReplace)
	local result = {cmd = "fifteen",childCmd = "updateList"}
	local awardTabs = getNeedTable(player)
	local serverStart=util.ppn(player,const.PP_LOGIN_DAY);--累计登录天数
	for k,v in pairs(awardTabs) do
		local thisReceiveNum = util.ppn(player, v.con);
		local receiveState;
		if thisReceiveNum>0 then
			receiveState = thisReceiveNum;
		else
			receiveState = (serverStart>=k) and -1 or 0
		end
		v.state = receiveState;
	end
	result.table = awardTabs;
	result.noReplace = noReplace;
	result.jumpNum=getNearIndex(player)
	player:push_lua_table("PanelRewardHall",util.encode(result))
end

function receiveAward(player,tag)
	if tag and tag>0 then
		local awards = getNeedTable(player)
		local nowAward = awards[tag];
		if nowAward then
			if util.ppn(player, nowAward.con) <=0 then
				local serverStart=util.ppn(player,const.PP_LOGIN_DAY);--累计登录天数
				if serverStart>=tag then

					local bagnum = player:num_bag_black();
					local needbags = 0;
					for i,v in ipairs(nowAward.awards) do
						needbags = needbags +util.getNeedBags(player,v.typeId,v.num);
					end
					if needbags<=bagnum then
						player:set_param(nowAward.con, util.ppn(player, nowAward.con)+1);
						for i,v in ipairs(nowAward.awards) do
							player:add_item(v.typeId,v.num,v.bind);
							-- player:alert(1, 1, "已成功领取"..v.num.."个"..v.name);
						end
						local result = {cmd = "fifteen",childCmd = "updateListOnce",tag = tag}
						result.state = util.ppn(player, nowAward.con);
						player:push_lua_table("PanelRewardHall",util.encode(result))

						gui.moduleRedPoint.checkAwardFifteenUsable(player);
					else
						player:alert(1, 1, "背包空间不足，请清理背包后重试！");
					end

				else
					player:alert(1, 1, "登录天数不满"..tag.."天，不可领取！");
				end
			else
				player:alert(1, 1, "您已领取过该奖励，不可重复领取！");
			end
		end
	end
end

--寻找最近一个没有领取的
function getNearIndex(player)
	for i=1,#awardTab do
		local itemData = awardTab[i]
		if util.ppn(player,itemData.con)<=0 then
			return i
		end
	end
	return #awardTab
end

--15天登录红点检测
function checkRedPoint(player)
	local serverStart=util.ppn(player,const.PP_LOGIN_DAY);--累计登录天数
	for i=1,#awardTab do
		local itemData = awardTab[i]
		if util.ppn(player,itemData.con)<=0 and serverStart>=i then
			return true
		end
	end
	return false
end